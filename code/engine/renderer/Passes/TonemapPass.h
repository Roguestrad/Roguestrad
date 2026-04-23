/*
 * Copyright (c) 2014-2021, NVIDIA CORPORATION. All rights reserved.
 * Copyright (C) 2022 Stephen Pridham (id Tech 4x integration)
 * Copyright (C) 2022-2023 Robert Beckebans (id Tech 4x integration)
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

#ifndef RENDERER_PASSES_TONEMAPPASS
#define RENDERER_PASSES_TONEMAPPASS

#include "CommonPasses.h"

struct ToneMappingParameters {
	float histogramLowPercentile  = 0.8f;
	float histogramHighPercentile = 0.95f;
	float eyeAdaptationSpeedUp	  = 1.f;
	float eyeAdaptationSpeedDown  = 0.5f;
	float minAdaptedLuminance	  = 0.02f;
	float maxAdaptedLuminance	  = 0.5f;
	float exposureBias			  = -0.5f;
	float whitePoint			  = 3.f;
	bool  enableColorLUT		  = true;
};

/*!
	\class TonemapPass
	\brief A class that handles tone mapping operations including histogram computation, exposure calculation, and final tonemap rendering.

	The TonemapPass class manages the complete tone mapping pipeline for rendering operations. It initializes GPU resources such as shaders, constant buffers, storage buffers, binding sets, and
   pipelines for both histogram computation and final tonemapping. The class supports loading and applying color lookup tables, and handles the complete flow from histogram accumulation to exposure
   adjustment and final tonemap rendering. It provides methods for rendering with full tone mapping capabilities as well as simplified rendering, exposure reset, histogram reset, and frame histogram
   addition. The class is designed to work with GPU command lists and framebuffer objects for efficient GPU-side processing of tone mapping operations.

*/
class TonemapPass
{
public:
	struct CreateParameters {
		bool			isTextureArray			  = false;
		uint32_t		histogramBins			  = 256;
		uint32_t		numConstantBufferVersions = 32;
		nvrhi::IBuffer* exposureBufferOverride	  = nullptr;
		idImage*		colorLUT				  = nullptr;
	};

	//! Initializes a new instance of the TonemapPass class.
	TonemapPass();

	/*!
		\brief Initializes the tonemap pass with the provided device handle, common passes, parameters, and sample framebuffer.

		This function sets up all necessary resources for the tonemap pass including shaders, constant buffers, storage buffers, binding sets, and pipelines. It configures the histogram and exposure
	   computation shaders as well as the final tonemapping shader. The function also handles the setup of the color lookup table if provided and initializes the render and compute pipelines for the
	   tonemapping operations. The function asserts that the histogram bins do not exceed 256.

		\param deviceHandle The device handle used for creating GPU resources
		\param commonPasses Pointer to the common render passes for shared resources
		\param params Structure containing creation parameters for the tonemap pass
		\param _sampleFramebuffer The framebuffer to be used for the sampling operation
		\throws assertion failure if histogramBins in params exceeds 256
	*/
	void Init( nvrhi::DeviceHandle deviceHandle, CommonRenderPasses* commonPasses, const CreateParameters& params, nvrhi::IFramebuffer* _sampleFramebuffer );

	/*!
		\brief Applies tone mapping to a source texture and renders the result to a target framebuffer.

		This function performs tone mapping on a given source texture using specified tone mapping parameters and renders the output to the provided framebuffer. It manages binding sets for shader
	   resources and ensures proper GPU state setup before drawing a full-screen quad. The function handles caching of binding sets based on source texture hash for performance optimization. The tone
	   mapping parameters control exposure, white point, and luminance adaptation, with optional color lookup table support.

		\param commandList The command list to record rendering commands into
		\param params The tone mapping parameters including exposure, white point, and luminance settings
		\param viewDef The view definition containing viewport and scissor information for rendering
		\param sourceTexture The input texture to apply tone mapping to
		\param targetFb The target framebuffer to render the tone mapped output to
	*/
	void Render( nvrhi::ICommandList* commandList, const ToneMappingParameters& params, const viewDef_t* viewDef, nvrhi::ITexture* sourceTexture, nvrhi::FramebufferHandle _targetFb );

	/*!
		\brief Performs tone mapping rendering by processing a source texture through a series of histogram-based exposure computations and final rendering steps.

		This function executes a complete tone mapping pass which includes resetting the histogram, adding the current frame to the histogram for exposure calculation, computing the exposure value
	   based on the provided parameters, and finally rendering the tone-mapped result to the specified framebuffer. It operates on a single frame of input texture data and updates the exposure
	   dynamically based on the scene's luminance distribution.

		\param commandList The command list to record rendering commands onto
		\param params Parameters controlling the tone mapping behavior
		\param viewDef The view definition for the current rendering context
		\param sourceTexture The input texture to apply tone mapping to
		\param _targetFb The framebuffer to render the tone-mapped result into
	*/
	void SimpleRender( nvrhi::ICommandList* commandList, const ToneMappingParameters& params, const viewDef_t* viewDef, nvrhi::ITexture* sourceTexture, nvrhi::FramebufferHandle _targetFb );

	//! Checks whether the tonemap pass has been successfully loaded.
	bool IsLoaded() const { return isLoaded; }

private:
	//! Resets the exposure by clearing the exposure buffer with the specified initial exposure value.
	void							ResetExposure( nvrhi::ICommandList* commandList, float initialExposure );

	//! Clears the histogram buffer using the provided command list.
	void							ResetHistogram( nvrhi::ICommandList* commandList );

	//! Adds a frame to the histogram using the provided command list, view definition, and source texture
	void							AddFrameToHistogram( nvrhi::ICommandList* commandList, const viewDef_t* viewDef, nvrhi::ITexture* sourceTexture );

	//! Computes the exposure for tone mapping using the provided parameters and command list.
	void							ComputeExposure( nvrhi::ICommandList* commandList, const ToneMappingParameters& params );

	bool							isLoaded;
	idImage*						colorLut;
	int								colorLutSize;
	CommonRenderPasses*				commonPasses;
	nvrhi::DeviceHandle				device;
	nvrhi::ShaderHandle				histogramShader;
	nvrhi::ShaderHandle				exposureShader;
	nvrhi::ShaderHandle				tonemapShader;
	nvrhi::BufferHandle				toneMappingCb;
	nvrhi::BufferHandle				histogramBuffer;
	nvrhi::BufferHandle				exposureBuffer;
	nvrhi::BindingLayoutHandle		renderBindingLayoutHandle;
	nvrhi::BindingLayoutHandle		histogramBindingLayoutHandle;
	nvrhi::ComputePipelineHandle	histogramPipeline;
	nvrhi::ComputePipelineHandle	exposurePipeline;
	nvrhi::GraphicsPipelineHandle	renderPipeline;
	nvrhi::BindingSetHandle			exposureBindingSet;
	idList<nvrhi::BindingSetHandle> histogramBindingSets;
	idHashIndex						histogramBindingHash;
	idList<nvrhi::BindingSetHandle> renderBindingSets;
	idHashIndex						renderBindingHash;
};

#endif
