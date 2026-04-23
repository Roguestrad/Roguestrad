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

#ifndef RENDER_PASS_H_
#define RENDER_PASS_H_

#include "engine/sys/DeviceManager.h"

/*!
	\class IRenderPass
	\brief Interface for managing render passes in a graphics rendering system.

	Provides a common interface for implementing different rendering operations within a graphics pipeline. The class serves as a base for concrete render pass implementations that handle specific
   rendering tasks such as frontend rendering, direct framebuffer rendering, animation updates, and back buffer management. The interface integrates with a device manager to access graphics device
   resources and track frame indices. Implementations can override virtual methods to define custom rendering behavior while maintaining a consistent interaction pattern with the rendering system.

*/
class IRenderPass
{
private:
	DeviceManager* deviceManager;

public:
	//! Constructs an IRenderPass instance with the specified device manager.
	explicit IRenderPass( DeviceManager* deviceManager ) :
		deviceManager( deviceManager )
	{
	}

	virtual ~IRenderPass() = default;

	//! Placeholder virtual function for rendering frontend operations.
	virtual void				  RenderFrontend() { }

	//! Renders the pass content to the specified framebuffer.
	virtual void				  Render( nvrhi::IFramebuffer* framebuffer ) { }

	//! Performs animation updates with the specified elapsed time in seconds.
	virtual void				  Animate( float fElapsedTimeSeconds ) { }

	//! Notifies the render pass that the back buffer is resizing.
	virtual void				  BackBufferResizing() { }

	//! Handles the event when the back buffer is resized.
	virtual void				  BackBufferResized( const uint32_t width, const uint32_t height, const uint32_t sampleCount ) { }

	//! Returns the device manager associated with this render pass.
	[[nodiscard]] DeviceManager*  GetDeviceManager() const { return deviceManager; }

	//! Returns the device associated with this render pass.
	[[nodiscard]] nvrhi::IDevice* GetDevice() const { return deviceManager->GetDevice(); }

	//! Returns the current frame index from the device manager
	[[nodiscard]] uint32_t		  GetFrameIndex() const { return deviceManager->GetFrameIndex(); }
};

/*!
	\class BasicTriangle
	\brief A render pass for drawing basic triangle meshes with customizable shaders and materials.

	This class implements a render pass specifically designed for rendering basic triangle meshes. It inherits from IRenderPass and provides functionality for initializing graphics resources, handling
   back buffer resizing, animating the triangle, and performing the actual rendering. The class manages vertex and index buffers, sets up shaders and input layouts, and handles material binding. It is
   intended to be used as part of a graphics rendering pipeline where simple triangular geometry needs to be drawn with custom rendering properties. The implementation supports dynamic updates through
   animation and proper resource management during resize events.

*/
class BasicTriangle : public IRenderPass
{
private:
	nvrhi::ShaderHandle			  vertexShader;
	nvrhi::ShaderHandle			  pixelShader;
	nvrhi::GraphicsPipelineHandle pipeline;
	nvrhi::CommandListHandle	  commandList;
	nvrhi::InputLayoutHandle	  inputLayout;
	nvrhi::BindingLayoutHandle	  bindingLayout;
	nvrhi::BindingSetHandle		  bindingSet;

	vertCacheHandle_t			  vertexBlock;
	vertCacheHandle_t			  indexBlock;
	idDrawVert*					  vertexPointer;
	triIndex_t*					  indexPointer;
	const idMaterial*			  material;

	int							  numVerts;
	int							  numIndexes;

	//! Allocates vertex and index buffers for rendering a triangle mesh
	idDrawVert*					  AllocVerts( int vertCount, triIndex_t* tempIndexes, int indexCount );

public:
	using IRenderPass::IRenderPass;

	//! Initializes the basic triangle by setting up shaders, input layout, material, and binding sets.
	bool Init();

	//! Handles the back buffer resizing event by resetting the graphics pipeline.
	void BackBufferResizing() override;

	//! Updates the animation state of the basic triangle using the elapsed time since the last frame.
	void Animate( float fElapsedTimeSeconds ) override;

	//! Prepares and uploads vertex and index data for rendering a basic triangle.
	void RenderFrontend() override;

	//! Renders a basic triangle using the provided framebuffer with graphics pipeline setup and draw commands.
	void Render( nvrhi::IFramebuffer* framebuffer ) override;
};

#endif
