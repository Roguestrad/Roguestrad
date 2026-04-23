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
#ifndef COMMON_PASSES_H_
#define COMMON_PASSES_H_

#include <nvrhi/nvrhi.h>
#include <memory>
#include <unordered_map>

class BindingCache;
class ShaderFactory;

constexpr uint32_t c_MaxRenderPassConstantBufferVersions = 16;

enum class BlitSampler { Point, Linear, Sharpen };

struct BlitParameters {
	nvrhi::IFramebuffer*			targetFramebuffer = nullptr;
	nvrhi::Viewport					targetViewport;
	idVec4							targetBox = idVec4( 0.f, 0.f, 1.f, 1.f );

	nvrhi::ITexture*				sourceTexture	 = nullptr;
	uint32_t						sourceArraySlice = 0;
	uint32_t						sourceMip		 = 0;
	idVec4							sourceBox		 = idVec4( 0.f, 0.f, 1.f, 1.f );

	BlitSampler						sampler = BlitSampler::Linear;
	nvrhi::BlendState::RenderTarget blendState;
	nvrhi::Color					blendConstantColor = nvrhi::Color( 0.f );
};

struct BlitConstants {
	idVec2 sourceOrigin;
	idVec2 sourceSize;

	idVec2 targetOrigin;
	idVec2 targetSize;

	float  sharpenFactor;
};

/*!
	\class CommonRenderPasses
	\brief Manages common render passes and texture blitting operations for graphics rendering.
*/
class CommonRenderPasses
{
protected:
	nvrhi::DeviceHandle m_Device;

	struct PsoCacheKey {
		nvrhi::FramebufferInfoEx		fbinfo;
		nvrhi::IShader*					shader;
		nvrhi::BlendState::RenderTarget blendState;

		//! Compares this PSO cache key with another for equality based on framebuffer info, shader, and blend state
		bool							operator==( const PsoCacheKey& other ) const { return fbinfo == other.fbinfo && shader == other.shader && blendState == other.blendState; }

		//! Compares this PSO cache key with another for inequality
		bool							operator!=( const PsoCacheKey& other ) const { return !( *this == other ); }

		struct Hash {
			//! Computes a hash value for a PSO cache key using combined hash values of its components.
			size_t operator()( const PsoCacheKey& s ) const
			{
				size_t hash = 0;
				nvrhi::hash_combine( hash, s.fbinfo );
				nvrhi::hash_combine( hash, s.shader );
				nvrhi::hash_combine( hash, s.blendState );
				return hash;
			}
		};
	};

	std::unordered_map<PsoCacheKey, nvrhi::GraphicsPipelineHandle, PsoCacheKey::Hash> m_BlitPsoCache;

public:
	nvrhi::ShaderHandle		   m_RectVS;
	nvrhi::ShaderHandle		   m_BlitPS;
	nvrhi::ShaderHandle		   m_BlitArrayPS;
	nvrhi::ShaderHandle		   m_SharpenPS;
	nvrhi::ShaderHandle		   m_SharpenArrayPS;

	nvrhi::SamplerHandle	   m_PointClampSampler;
	nvrhi::SamplerHandle	   m_PointWrapSampler;
	nvrhi::SamplerHandle	   m_LinearClampSampler;
	nvrhi::SamplerHandle	   m_LinearBorderSampler; // D3 zeroClamp
	nvrhi::SamplerHandle	   m_LinearClampCompareSampler;
	nvrhi::SamplerHandle	   m_LinearWrapSampler;
	nvrhi::SamplerHandle	   m_AnisotropicWrapSampler;
	nvrhi::SamplerHandle	   m_AnisotropicClampEdgeSampler;

	nvrhi::BindingLayoutHandle m_BlitBindingLayout;

	//! Initializes a new instance of the CommonRenderPasses class with a null device pointer.
	CommonRenderPasses();

	//! Initializes the common render passes and sampler objects for the given device.
	void Init( nvrhi::IDevice* device );

	//! Releases all resources and resets the state of the common render passes system.
	void Shutdown();

	//! Copies texture data from a source texture to a target framebuffer using specified blit parameters.
	void BlitTexture( nvrhi::ICommandList* commandList, const BlitParameters& params, BindingCache* bindingCache = nullptr );

	/*!
		\brief Copies the entire source texture into the target framebuffer using a linear sampler

		This function performs a texture blit operation from a source texture to a target framebuffer. It simplifies the process by using the entire source texture at mip level 0 and slice 0, and
	   copies it to the entire target framebuffer. The operation uses a linear sampler for filtering. The function internally constructs a BlitParameters structure and delegates the actual blit
	   operation to the overloaded BlitTexture method that accepts these parameters.

		\param commandList The command list to record the blit operation commands into
		\param targetFramebuffer The framebuffer to blit the source texture into
		\param sourceTexture The texture to copy data from
		\param bindingCache Optional binding cache for managing shader resource bindings
		\throws assertion failure if commandList, targetFramebuffer, or sourceTexture is null
	*/
	void BlitTexture( nvrhi::ICommandList* commandList, nvrhi::IFramebuffer* targetFramebuffer, nvrhi::ITexture* sourceTexture, BindingCache* bindingCache = nullptr );
};

#endif
