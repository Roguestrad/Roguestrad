/*
 * Copyright (c) 2014-2021, NVIDIA CORPORATION. All rights reserved.
 * Copyright (C) 2022 Stephen Pridham (id Tech 4x integration)
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
#ifndef RENDERER_PASSES_MIPMAPGENPASS_H_
#define RENDERER_PASSES_MIPMAPGENPASS_H_

class CommonRenderPasses;

/*!
	\class MipMapGenPass
	\brief A pass for generating and displaying texture mipmaps.
*/
class MipMapGenPass
{
public:
	enum Mode : uint8_t {
		MODE_COLOR	= 0, // bilinear reduction of RGB channels
		MODE_MIN	= 1, // min() reduction of R channel
		MODE_MAX	= 2, // max() reduction of R channel
		MODE_MINMAX = 3, // min() and max() reductions of R channel into RG channels
	};

	//! Initializes a MipMapGenPass instance to generate mipmaps for the provided texture using the specified mode.
	MipMapGenPass( nvrhi::IDevice* device, nvrhi::TextureHandle texture, Mode mode = Mode::MODE_MAX );

	//! Dispatches the mipmap generation compute shader passes to generate texture mipmaps from LOD 0 up to the specified maximum LOD level
	void Dispatch( nvrhi::ICommandList* commandList, int maxLOD = -1 );

	//! Displays mip-map levels of a texture in a spiral pattern on the target framebuffer.
	void Display( CommonRenderPasses& commonPasses, nvrhi::ICommandList* commandList, nvrhi::IFramebuffer* target );

private:
	nvrhi::DeviceHandle				m_Device;
	nvrhi::ShaderHandle				m_Shader;
	nvrhi::TextureHandle			m_Texture;
	nvrhi::BufferHandle				m_ConstantBuffer;
	nvrhi::BindingLayoutHandle		m_BindingLayout;
	idList<nvrhi::BindingSetHandle> m_BindingSets;
	nvrhi::ComputePipelineHandle	m_Pso;

	// Set of unique dummy textures - see details in class implementation
	struct NullTextures;
	std::shared_ptr<NullTextures> m_NullTextures;

	BindingCache				  m_BindingCache;

	const uint32_t				  numConstantBufferVersions = 32;
};

#endif
