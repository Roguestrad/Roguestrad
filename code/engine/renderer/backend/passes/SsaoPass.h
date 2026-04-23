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
#ifndef RENDERER_PASSES_SSAOPASS_H_
#define RENDERER_PASSES_SSAOPASS_H_

/*!
	\class SsaoPass
	\brief Provides screen space ambient occlusion rendering capabilities using G-buffer inputs.
*/
class SsaoPass
{
private:
	struct SubPass {
		nvrhi::ShaderHandle					 Shader;
		nvrhi::BindingLayoutHandle			 BindingLayout;
		std::vector<nvrhi::BindingSetHandle> BindingSets;
		nvrhi::ComputePipelineHandle		 Pipeline;
	};

	SubPass				 m_Deinterleave;
	SubPass				 m_Compute;
	SubPass				 m_Blur;

	nvrhi::DeviceHandle	 m_Device;
	nvrhi::BufferHandle	 m_ConstantBuffer;
	CommonRenderPasses*	 commonRenderPasses;

	nvrhi::TextureHandle m_DeinterleavedDepth;
	nvrhi::TextureHandle m_DeinterleavedOcclusion;
	idVec2				 m_QuantizedGbufferTextureSize;

public:
	struct CreateParameters {
		idVec2 dimensions;
		bool   inputLinearDepth		= false;
		bool   octEncodedNormals	= false;
		bool   directionalOcclusion = false;
		int	   numBindingSets		= 1;
	};

	//! Constructs an SSAO pass with the specified device, parameters, and common render passes
	SsaoPass( nvrhi::IDevice* device, const CreateParameters& params, CommonRenderPasses* commonRenderPasses );

	/*!
		\brief Constructs an SSAO pass using the provided device and textures for depth, normals, and destination

		This constructor initializes an SSAO (Screen Space Ambient Occlusion) pass for rendering. It takes a device, common render passes, and three textures: depth, normals, and destination. It uses
	   the dimensions of the depth texture to create parameters for the pass, ensuring sample count and dimension compatibility between depth and normals textures. The constructor also sets up the
	   binding set for the provided textures.

		\param device The graphics device to use for creating the SSAO pass
		\param commonPasses Pointer to common render passes used by the SSAO pass
		\param gbufferDepth The depth texture from the G-buffer
		\param gbufferNormals The normals texture from the G-buffer
		\param destinationTexture The destination texture where the SSAO results will be written
		\throws assertion failures if the sample count or texture dimensions of depth and normals textures are not compatible
	*/
	SsaoPass( nvrhi::IDevice* device, CommonRenderPasses* commonPasses, nvrhi::ITexture* gbufferDepth, nvrhi::ITexture* gbufferNormals, nvrhi::ITexture* destinationTexture );

	/*!
		\brief Creates and sets up binding sets for SSAO pass operations using the provided textures and binding set index.

		This function configures three binding sets for different stages of the SSAO pass: deinterleaving, computing occlusion, and blurring. Each binding set is created with specific resource
	   bindings including constant buffers, texture shader resource views, and texture unordered access views. The binding sets are stored in arrays indexed by the provided bindingSetIndex parameter
	   for later use in rendering.

		\param gbufferDepth Input depth texture from the G-buffer
		\param gbufferNormals Input normals texture from the G-buffer
		\param destinationTexture Output texture where the final SSAO result will be written
		\param bindingSetIndex Index of the binding set array to store the created binding sets
	*/
	void CreateBindingSet( nvrhi::ITexture* gbufferDepth, nvrhi::ITexture* gbufferNormals, nvrhi::ITexture* destinationTexture, int bindingSetIndex = 0 );

	//! Renders the SSAO pass using the provided command list and view definition
	void Render( nvrhi::ICommandList* commandList, const viewDef_t* viewDef, int bindingSetIndex = 0 );
};

#endif
