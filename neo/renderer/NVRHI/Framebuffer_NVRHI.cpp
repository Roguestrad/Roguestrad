/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2014-2022 Robert Beckebans
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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "../RenderCommon.h"
#include "../Framebuffer.h"

#include "sys/DeviceManager.h"

extern DeviceManager* deviceManager;

static void R_ListFramebuffers_f( const idCmdArgs& args )
{
	// TODO
}

Framebuffer::Framebuffer( const char* name, int w, int h )
	: fboName( name )
	, frameBuffer( 0 )
	, colorFormat( 0 )
	, depthBuffer( 0 )
	, depthFormat( 0 )
	, stencilFormat( 0 )
	, stencilBuffer( 0 )
	, width( w )
	, height( h )
	, msaaSamples( false )
{
	framebuffers.Append( this );
}

Framebuffer::Framebuffer( const char* name, const nvrhi::FramebufferDesc& desc )
	: fboName( name )
	, frameBuffer( 0 )
	, colorFormat( 0 )
	, depthBuffer( 0 )
	, depthFormat( 0 )
	, stencilFormat( 0 )
	, stencilBuffer( 0 )
	, msaaSamples( false )
{
	framebuffers.Append( this );
	apiObject = deviceManager->GetDevice()->createFramebuffer( desc );
	width = apiObject->getFramebufferInfo().width;
	height = apiObject->getFramebufferInfo().height;
}

Framebuffer::~Framebuffer()
{
	apiObject.Reset();
}

void Framebuffer::Init()
{
	cmdSystem->AddCommand( "listFramebuffers", R_ListFramebuffers_f, CMD_FL_RENDERER, "lists framebuffers" );

	// HDR
	ResizeFramebuffers();
}

void Framebuffer::CheckFramebuffers()
{
	//int screenWidth = renderSystem->GetWidth();
	//int screenHeight = renderSystem->GetHeight();
}

void Framebuffer::Shutdown()
{
	framebuffers.DeleteContents( true );
}

void Framebuffer::ResizeFramebuffers( bool reloadImages )
{
	backEnd.ClearCaches();

	// RB: FIXME I think allocating new Framebuffers lead to a memory leak
	framebuffers.DeleteContents( true );

	if( reloadImages )
	{
		ReloadImages();
	}

	uint32_t backBufferCount = deviceManager->GetBackBufferCount();
	globalFramebuffers.swapFramebuffers.Resize( backBufferCount );
	globalFramebuffers.swapFramebuffers.SetNum( backBufferCount );

	for( uint32_t index = 0; index < backBufferCount; index++ )
	{
		globalFramebuffers.swapFramebuffers[index] = new Framebuffer(
			va( "_swapChain%d", index ),
			nvrhi::FramebufferDesc()
			.addColorAttachment( deviceManager->GetBackBuffer( index ) ) );
	}

	for( int arr = 0; arr < 6; arr++ )
	{
		for( int mip = 0; mip < MAX_SHADOWMAP_RESOLUTIONS; mip++ )
		{
			globalFramebuffers.shadowFBO[mip][arr] = new Framebuffer( va( "_shadowMap%i_%i", mip, arr ),
					nvrhi::FramebufferDesc().setDepthAttachment(
						nvrhi::FramebufferAttachment()
						.setTexture( globalImages->shadowImage[mip]->GetTextureHandle().Get() )
						.setArraySlice( arr ) ) );
		}
	}

	globalFramebuffers.shadowAtlasFBO = new Framebuffer( "_shadowAtlas",
			nvrhi::FramebufferDesc()
			.setDepthAttachment( globalImages->shadowAtlasImage->texture ) );

	globalFramebuffers.ldrFBO = new Framebuffer( "_ldr",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->ldrImage->texture )
			.setDepthAttachment( globalImages->currentDepthImage->texture ) );

	globalFramebuffers.hdrFBO = new Framebuffer( "_hdr",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->currentRenderHDRImage->texture )
			.setDepthAttachment( globalImages->currentDepthImage->texture ) );

	globalFramebuffers.postProcFBO = new Framebuffer( "_postProc",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->currentRenderImage->texture ) );

	for( int i = 0; i < MAX_STEREO_BUFFERS; i++ )
	{
		globalFramebuffers.taaMotionVectorsFBO[i] = new Framebuffer( va( "_taaMotionVectors%i", i ),
				nvrhi::FramebufferDesc()
				.addColorAttachment( globalImages->taaMotionVectorsImage[i]->texture ) );
	}
	globalFramebuffers.taaResolvedFBO = new Framebuffer( "_taaResolved",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->taaResolvedImage->texture ) );

	globalFramebuffers.envprobeFBO = new Framebuffer( "_envprobeRender",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->envprobeHDRImage->texture )
			.setDepthAttachment( globalImages->envprobeDepthImage->texture ) );

	for( int i = 0; i < MAX_SSAO_BUFFERS; i++ )
	{
		globalFramebuffers.ambientOcclusionFBO[i] = new Framebuffer( va( "_aoRender%i", i ),
				nvrhi::FramebufferDesc()
				.addColorAttachment( globalImages->ambientOcclusionImage[i]->texture ) );
	}

	// HIERARCHICAL Z BUFFER
	for( int i = 0; i < MAX_HIERARCHICAL_ZBUFFERS; i++ )
	{
		globalFramebuffers.csDepthFBO[i] = new Framebuffer( va( "_csz%d", i ),
				nvrhi::FramebufferDesc().addColorAttachment(
					nvrhi::FramebufferAttachment()
					.setTexture( globalImages->hierarchicalZbufferImage->texture )
					.setMipLevel( i ) ) );
	}

	globalFramebuffers.geometryBufferFBO = new Framebuffer( "_gbuffer",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->gbufferNormalsRoughnessImage->texture )
			.setDepthAttachment( globalImages->currentDepthImage->texture ) );

	globalFramebuffers.smaaInputFBO = new Framebuffer( "_smaaInput",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->smaaInputImage->texture ) );

	globalFramebuffers.smaaEdgesFBO = new Framebuffer( "_smaaEdges",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->smaaEdgesImage->texture ) );

	globalFramebuffers.smaaBlendFBO = new Framebuffer( "_smaaBlend",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->smaaBlendImage->texture ) );

	for( int i = 0; i < MAX_BLOOM_BUFFERS; i++ )
	{
		globalFramebuffers.bloomRenderFBO[i] = new Framebuffer( va( "_bloomRender%i", i ),
				nvrhi::FramebufferDesc()
				.addColorAttachment( globalImages->bloomRenderImage[i]->texture ) );
	}

	globalFramebuffers.guiRenderTargetFBO = new Framebuffer( "_guiEdit",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->guiEdit->texture )
			.setDepthAttachment( globalImages->guiEditDepthStencilImage->texture ) );

	globalFramebuffers.accumFBO = new Framebuffer( "_accum",
			nvrhi::FramebufferDesc()
			.addColorAttachment( globalImages->accumImage->texture ) );

	if( vrSystem->IsActive() )
	{
		globalFramebuffers.vrPDAFBO = new Framebuffer( "_vrPDA",
				nvrhi::FramebufferDesc()
				.addColorAttachment( globalImages->vrPDAImage->texture ) );

		globalFramebuffers.vrHUDFBO = new Framebuffer( "_vrHUD",
				nvrhi::FramebufferDesc()
				.addColorAttachment( globalImages->vrHUDImage->texture ) );

		for( int i = 0; i < MAX_STEREO_BUFFERS; i++ )
		{
			globalFramebuffers.vrStereoFBO[i] = new Framebuffer( va( "_stereoRender%i", i ),
					nvrhi::FramebufferDesc()
					.addColorAttachment( globalImages->stereoRenderImages[i]->texture ) );

			globalFramebuffers.vrHmdEyeFBO[i] = new Framebuffer( va( "_hmdEye%i", i ),
					nvrhi::FramebufferDesc()
					.addColorAttachment( globalImages->hmdEyeImages[i]->texture )
					.setDepthAttachment( globalImages->currentDepthImage->texture ) );
		}
	}

	Framebuffer::Unbind();
}

void Framebuffer::ReloadImages()
{
	backEnd.commandList->open();
	globalImages->ldrImage->Reload( false, backEnd.commandList );
	globalImages->currentRenderImage->Reload( false, backEnd.commandList );
	globalImages->currentDepthImage->Reload( false, backEnd.commandList );
	globalImages->currentRenderHDRImage->Reload( false, backEnd.commandList );
	for( int i = 0; i < MAX_SSAO_BUFFERS; i++ )
	{
		globalImages->ambientOcclusionImage[i]->Reload( false, backEnd.commandList );
	}
	globalImages->hierarchicalZbufferImage->Reload( false, backEnd.commandList );
	globalImages->gbufferNormalsRoughnessImage->Reload( false, backEnd.commandList );

	for( int i = 0; i < MAX_STEREO_BUFFERS; i++ )
	{
		globalImages->taaMotionVectorsImage[i]->Reload( false, backEnd.commandList );
		globalImages->taaFeedback1Image[i]->Reload( false, backEnd.commandList );
		globalImages->taaFeedback2Image[i]->Reload( false, backEnd.commandList );
	}
	globalImages->taaResolvedImage->Reload( false, backEnd.commandList );

	globalImages->smaaInputImage->Reload( false, backEnd.commandList );
	globalImages->smaaEdgesImage->Reload( false, backEnd.commandList );
	globalImages->smaaBlendImage->Reload( false, backEnd.commandList );

	globalImages->shadowAtlasImage->Reload( false, backEnd.commandList );
	for( int i = 0; i < MAX_SHADOWMAP_RESOLUTIONS; i++ )
	{
		globalImages->shadowImage[i]->Reload( false, backEnd.commandList );
	}
	for( int i = 0; i < MAX_BLOOM_BUFFERS; i++ )
	{
		globalImages->bloomRenderImage[i]->Reload( false, backEnd.commandList );
	}
	globalImages->guiEdit->Reload( false, backEnd.commandList );
	globalImages->accumImage->Reload( false, backEnd.commandList );

	if( vrSystem->IsActive() )
	{
		globalImages->vrPDAImage->Reload( false, backEnd.commandList );
		globalImages->vrHUDImage->Reload( false, backEnd.commandList );

		for( int i = 0; i < MAX_STEREO_BUFFERS; i++ )
		{
			globalImages->stereoRenderImages[i]->Reload( false, backEnd.commandList );
			globalImages->hmdEyeImages[i]->Reload( false, backEnd.commandList );
		}
	}

	backEnd.commandList->close();
	deviceManager->GetDevice()->executeCommandList( backEnd.commandList );
}

void Framebuffer::Bind()
{
	if( backEnd.currentFrameBuffer != this )
	{
		backEnd.currentPipeline = nullptr;
	}

	backEnd.lastFrameBuffer = backEnd.currentFrameBuffer;
	backEnd.currentFrameBuffer = this;
}

bool Framebuffer::IsBound()
{
	return backEnd.currentFrameBuffer == this;
}

void Framebuffer::Unbind()
{
	globalFramebuffers.swapFramebuffers[deviceManager->GetCurrentBackBufferIndex()]->Bind();
}

bool Framebuffer::IsDefaultFramebufferActive()
{
	return backEnd.currentFrameBuffer == globalFramebuffers.swapFramebuffers[deviceManager->GetCurrentBackBufferIndex()];
}

Framebuffer* Framebuffer::GetActiveFramebuffer()
{
	return backEnd.currentFrameBuffer;
}

void Framebuffer::AddColorBuffer( int format, int index, int multiSamples )
{
}

void Framebuffer::AddDepthBuffer( int format, int multiSamples )
{
}

void Framebuffer::AddStencilBuffer( int format, int multiSamples )
{
}

void Framebuffer::AttachImage2D( int target, idImage* image, int index, int mipmapLod )
{
}

void Framebuffer::AttachImageDepth( int target, idImage* image )
{
}

void Framebuffer::AttachImageDepthLayer( idImage* image, int layer )
{
}

void Framebuffer::Check()
{
}

idScreenRect Framebuffer::GetViewPortInfo() const
{
	nvrhi::Viewport viewport = apiObject->getFramebufferInfo().getViewport();
	idScreenRect screenRect;
	screenRect.Clear();
	screenRect.AddPoint( viewport.minX, viewport.minY );
	screenRect.AddPoint( viewport.maxX, viewport.maxY );
	return screenRect;
}