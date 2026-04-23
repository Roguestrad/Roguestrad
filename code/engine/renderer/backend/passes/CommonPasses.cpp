/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2022 Stephen Pridham

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#pragma hdrstop
#include "../../frontend/tr_local.h"

#include "CommonPasses.h"

CommonRenderPasses::CommonRenderPasses() :
	m_Device( nullptr )
{
}

//! Checks if the given texture dimension is supported for blitting operations.
static bool IsSupportedBlitDimension( nvrhi::TextureDimension dimension )
{
	return dimension == nvrhi::TextureDimension::Texture2D || dimension == nvrhi::TextureDimension::Texture2DArray || dimension == nvrhi::TextureDimension::TextureCube ||
		   dimension == nvrhi::TextureDimension::TextureCubeArray;
}

//! Checks if the given texture dimension represents a texture array type.
static bool IsTextureArray( nvrhi::TextureDimension dimension )
{
	return dimension == nvrhi::TextureDimension::Texture2DArray || dimension == nvrhi::TextureDimension::TextureCube || dimension == nvrhi::TextureDimension::TextureCubeArray;
}

void CommonRenderPasses::Init( nvrhi::IDevice* device )
{
	m_Device = device;

	int rectIndex = renderProgManager.FindShader( "builtin/rect", SHADER_STAGE_VERTEX, "", idList<shaderMacro_t>(), true, LAYOUT_DRAW_VERT );
	m_RectVS	  = renderProgManager.GetShader( rectIndex );

	idList<shaderMacro_t> shaderMacros;
	shaderMacros.Append( shaderMacro_t( "TEXTURE_ARRAY", "0" ) );
	int blitIndex = renderProgManager.FindShader( "builtin/blit", SHADER_STAGE_FRAGMENT, "", shaderMacros, true, LAYOUT_DRAW_VERT );
	m_BlitPS	  = renderProgManager.GetShader( blitIndex );

	shaderMacros[0].definition = "1";
	blitIndex				   = renderProgManager.FindShader( "builtin/blit", SHADER_STAGE_FRAGMENT, "", shaderMacros, true, LAYOUT_DRAW_VERT );
	m_BlitArrayPS			   = renderProgManager.GetShader( blitIndex );

	auto samplerDesc	= nvrhi::SamplerDesc().setAllFilters( false ).setAllAddressModes( nvrhi::SamplerAddressMode::Clamp );
	m_PointClampSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setAllAddressModes( nvrhi::SamplerAddressMode::Wrap );
	m_PointWrapSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setAllAddressModes( nvrhi::SamplerAddressMode::Clamp );
	samplerDesc.setAllFilters( true );
	m_LinearClampSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setAllAddressModes( nvrhi::SamplerAddressMode::Border );
	samplerDesc.setBorderColor( nvrhi::Color( 0.f, 0.f, 0.f, 1.f ) );
	m_LinearBorderSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setReductionType( nvrhi::SamplerReductionType::Comparison );
	m_LinearClampCompareSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setAllAddressModes( nvrhi::SamplerAddressMode::Wrap );
	m_LinearWrapSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setMaxAnisotropy( 16 );
	m_AnisotropicWrapSampler = m_Device->createSampler( samplerDesc );

	samplerDesc.setAllAddressModes( nvrhi::SamplerAddressMode::ClampToEdge );
	m_AnisotropicClampEdgeSampler = m_Device->createSampler( samplerDesc );

	{
		nvrhi::BindingLayoutDesc layoutDesc;
		layoutDesc.visibility = nvrhi::ShaderType::All;
		layoutDesc.bindings	  = { nvrhi::BindingLayoutItem::PushConstants( 0, sizeof( BlitConstants ) ), nvrhi::BindingLayoutItem::Texture_SRV( 0 ), nvrhi::BindingLayoutItem::Sampler( 0 ) };

		m_BlitBindingLayout = m_Device->createBindingLayout( layoutDesc );
	}
}

void CommonRenderPasses::Shutdown()
{
	// SRS - Delete the pipelines referenced by the blit cache
	for( auto& [key, pipeline] : m_BlitPsoCache ) {
		pipeline.Reset();
	}

	// SRS - These assets have automatic resource management with overloaded = operator
	m_RectVS		 = nullptr;
	m_BlitPS		 = nullptr;
	m_BlitArrayPS	 = nullptr;
	m_SharpenPS		 = nullptr;
	m_SharpenArrayPS = nullptr;

	m_PointClampSampler			  = nullptr;
	m_PointWrapSampler			  = nullptr;
	m_LinearClampSampler		  = nullptr;
	m_LinearBorderSampler		  = nullptr;
	m_LinearClampCompareSampler	  = nullptr;
	m_LinearWrapSampler			  = nullptr;
	m_AnisotropicWrapSampler	  = nullptr;
	m_AnisotropicClampEdgeSampler = nullptr;

	m_BlitBindingLayout = nullptr;

	// SRS - Remove reference to nvrhi::IDevice, otherwise won't clean up properly on shutdown
	m_Device = nullptr;
}

void CommonRenderPasses::BlitTexture( nvrhi::ICommandList* commandList, const BlitParameters& params, BindingCache* bindingCache )
{
	assert( commandList );
	assert( params.targetFramebuffer );
	assert( params.sourceTexture );

	const nvrhi::FramebufferDesc& targetFramebufferDesc = params.targetFramebuffer->getDesc();
	assert( targetFramebufferDesc.colorAttachments.size() == 1 );
	assert( targetFramebufferDesc.colorAttachments[0].valid() );
	assert( !targetFramebufferDesc.depthAttachment.valid() );

	const nvrhi::FramebufferInfoEx& fbinfo	   = params.targetFramebuffer->getFramebufferInfo();
	const nvrhi::TextureDesc&		sourceDesc = params.sourceTexture->getDesc();

	assert( IsSupportedBlitDimension( sourceDesc.dimension ) );
	bool			isTextureArray = IsTextureArray( sourceDesc.dimension );

	nvrhi::Viewport targetViewport = params.targetViewport;
	if( targetViewport.width() == 0 && targetViewport.height() == 0 ) {
		// If no viewport is specified, create one based on the framebuffer dimensions.
		// Note that the FB dimensions may not be the same as target texture dimensions, in case a non-zero mip level is used.
		targetViewport = nvrhi::Viewport( float( fbinfo.width ), float( fbinfo.height ) );
	}

	nvrhi::IShader* shader = nullptr;
	switch( params.sampler ) {
		case BlitSampler::Point:
		case BlitSampler::Linear:
			shader = isTextureArray ? m_BlitArrayPS : m_BlitPS;
			break;
		case BlitSampler::Sharpen:
			shader = isTextureArray ? m_SharpenArrayPS : m_SharpenPS;
			break;
		default:
			assert( false );
	}

	nvrhi::GraphicsPipelineHandle& pso = m_BlitPsoCache[PsoCacheKey { fbinfo, shader, params.blendState }];
	if( !pso ) {
		nvrhi::GraphicsPipelineDesc psoDesc;
		psoDesc.bindingLayouts = { m_BlitBindingLayout };
		psoDesc.VS			   = m_RectVS;
		psoDesc.PS			   = shader;
		psoDesc.primType	   = nvrhi::PrimitiveType::TriangleStrip;
		psoDesc.renderState.rasterState.setCullNone();
		psoDesc.renderState.depthStencilState.depthTestEnable = false;
		psoDesc.renderState.depthStencilState.stencilEnable	  = false;
		psoDesc.renderState.blendState.targets[0]			  = params.blendState;

		pso = m_Device->createGraphicsPipeline( psoDesc, params.targetFramebuffer );
	}

	nvrhi::BindingSetDesc bindingSetDesc;
	{
		auto sourceDimension = sourceDesc.dimension;
		if( sourceDimension == nvrhi::TextureDimension::TextureCube || sourceDimension == nvrhi::TextureDimension::TextureCubeArray ) {
			sourceDimension = nvrhi::TextureDimension::Texture2DArray;
		}

		auto sourceSubresources = nvrhi::TextureSubresourceSet( params.sourceMip, 1, params.sourceArraySlice, 1 );

		bindingSetDesc.bindings = { nvrhi::BindingSetItem::PushConstants( 0, sizeof( BlitConstants ) ),
			nvrhi::BindingSetItem::Texture_SRV( 0, params.sourceTexture ).setSubresources( sourceSubresources ).setDimension( sourceDimension ),
			nvrhi::BindingSetItem::Sampler( 0, params.sampler == BlitSampler::Point ? m_PointClampSampler : m_LinearClampSampler ) };
	}

	// If a binding cache is provided, get the binding set from the cache.
	// Otherwise, create one and then release it.
	nvrhi::BindingSetHandle sourceBindingSet;
	if( bindingCache ) {
		sourceBindingSet = bindingCache->GetOrCreateBindingSet( bindingSetDesc, m_BlitBindingLayout );
	} else {
		sourceBindingSet = m_Device->createBindingSet( bindingSetDesc, m_BlitBindingLayout );
	}

	nvrhi::GraphicsState state;
	state.pipeline	  = pso;
	state.framebuffer = params.targetFramebuffer;
	state.bindings	  = { sourceBindingSet };
	state.viewport.addViewport( targetViewport );
	state.viewport.addScissorRect( nvrhi::Rect( targetViewport ) );
	state.blendConstantColor = params.blendConstantColor;

	BlitConstants blitConstants = {};
	blitConstants.sourceOrigin	= idVec2( params.sourceBox.x, params.sourceBox.y );
	blitConstants.sourceSize	= idVec2( params.sourceBox.z, params.sourceBox.w );
	blitConstants.targetOrigin	= idVec2( params.targetBox.x, params.targetBox.y );
	blitConstants.targetSize	= idVec2( params.targetBox.z, params.targetBox.w );

	commandList->setGraphicsState( state );

	commandList->setPushConstants( &blitConstants, sizeof( blitConstants ) );

	nvrhi::DrawArguments args;
	args.instanceCount = 1;
	args.vertexCount   = 4;
	commandList->draw( args );
}

void CommonRenderPasses::BlitTexture( nvrhi::ICommandList* commandList, nvrhi::IFramebuffer* targetFramebuffer, nvrhi::ITexture* sourceTexture, BindingCache* bindingCache )
{
	assert( commandList );
	assert( targetFramebuffer );
	assert( sourceTexture );

	BlitParameters params;
	params.targetFramebuffer = targetFramebuffer;
	params.sourceTexture	 = sourceTexture;
	BlitTexture( commandList, params, bindingCache );
}
