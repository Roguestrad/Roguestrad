/*
 * Copyright (c) 2014-2021, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <nvrhi/nvrhi.h>
#include <memory>

class CommonRenderPasses;

enum class TemporalAntiAliasingJitter { None, MSAA, Halton, R2, WhiteNoise };

struct TemporalAntiAliasingParameters {
	float newFrameWeight		= 0.1f;
	float clampingFactor		= 1.0f;
	float maxRadiance			= 10000.f;
	bool  enableHistoryClamping = true;
};

/*!
	\class TemporalAntiAliasingPass
	\brief A pass for implementing temporal antialiasing in a graphics rendering pipeline.
*/
class TemporalAntiAliasingPass
{
private:
	CommonRenderPasses*			 m_CommonPasses;

	nvrhi::ShaderHandle			 m_TemporalAntiAliasingCS;
	nvrhi::SamplerHandle		 m_BilinearSampler;
	nvrhi::BufferHandle			 m_TemporalAntiAliasingCB;

	nvrhi::BindingLayoutHandle	 m_ResolveBindingLayout;
	nvrhi::BindingSetHandle		 m_ResolveBindingSet;
	nvrhi::BindingSetHandle		 m_ResolveBindingSetPrevious;
	nvrhi::ComputePipelineHandle m_ResolvePso;

	uint32_t					 m_FrameIndex;
	uint32_t					 m_StencilMask;

	idVec2						 m_R2Jitter;

public:
	struct CreateParameters {
		nvrhi::ITexture* sourceDepth			   = nullptr;
		nvrhi::ITexture* motionVectors			   = nullptr;
		nvrhi::ITexture* unresolvedColor		   = nullptr;
		nvrhi::ITexture* resolvedColor			   = nullptr;
		nvrhi::ITexture* feedback1				   = nullptr;
		nvrhi::ITexture* feedback2				   = nullptr;
		bool			 useCatmullRomFilter	   = true;
		uint32_t		 motionVectorStencilMask   = 0;
		uint32_t		 numConstantBufferVersions = 16;
	};

	//! Constructs a new TemporalAntiAliasingPass object with default initialization.
	TemporalAntiAliasingPass();

	/*!
		\brief Initializes the temporal anti-aliasing pass with the specified device, common passes, view definition, and creation parameters.

		This function sets up the necessary resources and state for performing temporal anti-aliasing, including creating compute shaders, samplers, constant buffers, and binding sets based on the
	   provided parameters. It also handles MSAA-specific shader selection and stencil buffer setup if motion vector stencil masking is enabled. The function asserts that feedback textures have
	   matching dimensions and formats, and that required UAV flags are set.

		\param device The device to create GPU resources on
		\param commonPasses Pointer to common render passes for shared resources
		\param viewDef Pointer to the view definition for the current frame
		\param params Creation parameters including textures and buffer configurations for TAA
		\throws common->Error() when the source depth texture format doesn't have a stencil plane
	*/
	void	 Init( nvrhi::IDevice* device, CommonRenderPasses* commonPasses, const viewDef_t* viewDef, const CreateParameters& params );

	/*!
		\brief Performs temporal antialiasing resolution using the provided command list and parameters.

		This function executes the temporal resolve pass for antialiasing, which combines current and historical frame data to produce a high-quality antialiased output. It sets up constant buffer
	   values based on viewport information and TAA parameters, configures the compute shader state, and dispatches the compute job with appropriate grid sizes.

		\param commandList The command list to record the rendering commands into
		\param params Configuration parameters for the temporal antialiasing process
		\param feedbackIsValid Flag indicating whether valid feedback from previous frame is available for blending
		\param viewDef Pointer to the view definition containing viewport and rendering context information
	*/
	void	 TemporalResolve( nvrhi::ICommandList* commandList, const TemporalAntiAliasingParameters& params, bool feedbackIsValid, const viewDef_t* viewDef );

	//! Advances the temporal anti-aliasing frame index and updates jitter parameters for R2 sequence.
	void	 AdvanceFrame();

	//! Returns the current pixel offset for temporal anti-aliasing based on the frame index and jittering method.
	idVec2	 GetCurrentPixelOffset( int frameIndex );

	//! Returns the current frame index used for temporal anti-aliasing.
	uint32_t GetFrameIndex() const { return m_FrameIndex; }
};
