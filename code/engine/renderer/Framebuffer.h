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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

static const int MAX_SHADOWMAP_RESOLUTIONS = 5;
static const int MAX_BLOOM_BUFFERS		   = 2;
static const int MAX_GLOW_BUFFERS		   = 2;
static const int MAX_SSAO_BUFFERS		   = 2;
static const int MAX_STEREO_BUFFERS		   = 2;
static const int MAX_HIERARCHICAL_ZBUFFERS = 6; // native resolution + 5 MIP LEVELS

static const int ENVPROBE_CAPTURE_SIZE		= 256;
static const int RADIANCE_OCTAHEDRON_SIZE	= 512;
static const int IRRADIANCE_OCTAHEDRON_SIZE = 30 + 2;

// RB: shadow resolutions used in 1.3
// static	int shadowMapResolutions[MAX_SHADOWMAP_RESOLUTIONS] = { 2048, 1024, 512, 512, 256 };

// if we use higher resolutions than this than the shadow casting lights don't fit into the 16384^2 atlas anymore
static int		 shadowMapResolutions[MAX_SHADOWMAP_RESOLUTIONS] = { 1024, 512, 256, 256, 128 };

/*!
	\class Framebuffer
	\brief A framebuffer abstraction managing rendering targets and their attachments.

	This class provides a high-level interface for managing framebuffer objects within a rendering system. It handles the creation, configuration, and binding of framebuffers with various attachments
   including color, depth, and stencil buffers. The class supports both manual attachment of 2D images and automatic initialization with specified dimensions. Framebuffers can be resized, checked for
   errors, and queried for their properties such as dimensions and multisampling status. The implementation includes functionality for tracking active framebuffers, managing default framebuffer state,
   and coordinating with a global framebuffer registry for resource management and cleanup.

*/
class Framebuffer
{
public:
	//! Initializes a new framebuffer with the specified name and dimensions.
	Framebuffer( const char* name, int width, int height );

	//! Constructs a Framebuffer object with the specified name and framebuffer description
	Framebuffer( const char* name, const nvrhi::FramebufferDesc& desc );

	//! Destroys the Framebuffer object and resets its API object.
	virtual ~Framebuffer();

	//! Initializes the framebuffer system and registers the listFramebuffers console command.
	static void			Init();

	//! Shuts down the framebuffer system by deleting all framebuffers.
	static void			Shutdown();

	//! Checks the status of all framebuffers.
	static void			CheckFramebuffers();

	//! Returns a framebuffer with the specified name, or NULL if not found.
	static Framebuffer* Find( const char* name );

	//! Resizes all global framebuffers in the rendering system to match the current back buffer count and optionally reloads associated images.
	static void			ResizeFramebuffers( bool reloadImages = true );

	//! Reloads all images used in the framebuffer.
	static void			ReloadImages();

	//! Binds this framebuffer to the current rendering context.
	void				Bind();

	//! Checks whether the framebuffer is currently bound to the rendering context.
	bool				IsBound();

	//! Unbinds the current framebuffer and binds the back buffer.
	static void			Unbind();

	//! Checks if the default framebuffer is currently active.
	static bool			IsDefaultFramebufferActive();

	//! Returns a pointer to the currently active framebuffer object.
	static Framebuffer* GetActiveFramebuffer();

	//! Adds a color buffer to the framebuffer with the specified format, index, and optional multisamples.
	void				AddColorBuffer( int format, int index, int multiSamples = 0 );

	//! Adds a depth buffer to the framebuffer with the specified format and optional multisamples.
	void				AddDepthBuffer( int format, int multiSamples = 0 );

	//! Adds a stencil buffer to the framebuffer with the specified format and optional multi-sampling.
	void				AddStencilBuffer( int format, int multiSamples = 0 );

	/*!
		\brief Attaches a 2D image to the specified framebuffer target at the given index with optional mipmap level

		This function associates a 2D image resource with a specific target in the framebuffer object. The target parameter specifies which framebuffer attachment point to use, such as color, depth,
	   or stencil buffers. The image parameter provides the texture or image data to be attached, while index indicates the attachment point index. The mipmapLod parameter allows specifying a
	   particular mipmap level to attach, defaulting to level 0 if not specified.

		\param target Framebuffer attachment target such as color, depth, or stencil buffers
		\param image Pointer to the idImage resource to be attached
		\param index Attachment point index within the framebuffer
		\param mipmapLod Optional mipmap level to attach, defaults to 0
	*/
	void				AttachImage2D( int target, idImage* image, int index, int mipmapLod = 0 );

	//! Attaches a depth image to the framebuffer for the specified target.
	void				AttachImageDepth( int target, idImage* image );

	//! Attaches a depth layer from an image to the framebuffer.
	void				AttachImageDepthLayer( idImage* image, int layer );

	//! Checks for OpenGL errors. FIXME outdated
	void				Check();

	//! Returns the OpenGL framebuffer identifier associated with this framebuffer object.
	uint32_t			GetFramebuffer() const { return frameBuffer; }

	//! Returns the width of the framebuffer in pixels.
	int					GetWidth() const { return width; }

	//! Returns the height of the framebuffer in pixels
	int					GetHeight() const { return height; }

	//! Returns true if the framebuffer uses multi-sampled antialiasing.
	bool				IsMultiSampled() const { return msaaSamples; }

	//! Resizes the framebuffer to the specified width and height.
	void				Resize( int width_, int height_ )
	{
		width  = width_;
		height = height_;
	}

	//! Returns the NVRHI API object for this framebuffer
	nvrhi::IFramebuffer* GetApiObject() { return apiObject; }

	//! Returns the viewport information of the framebuffer as an idScreenRect.
	idScreenRect		 GetViewPortInfo() const;

private:
	idStr						fboName;

	// FBO object
	uint32_t					frameBuffer;

	uint32_t					colorBuffers[16];
	int							colorFormat;

	uint32_t					depthBuffer;
	int							depthFormat;

	uint32_t					stencilBuffer;
	int							stencilFormat;

	int							width;
	int							height;

	bool						msaaSamples;

	nvrhi::FramebufferHandle	apiObject;

	static idList<Framebuffer*> framebuffers;
};

struct globalFramebuffers_t {
	idList<Framebuffer*> swapFramebuffers;
	Framebuffer*		 shadowAtlasFBO;
	Framebuffer*		 shadowFBO[MAX_SHADOWMAP_RESOLUTIONS][6];
	Framebuffer*		 hdrFBO;
	Framebuffer*		 ldrFBO;
	Framebuffer*		 postProcFBO; // HDR16 used by 3D effects like heatHaze
	Framebuffer*		 taaMotionVectorsFBO[MAX_STEREO_BUFFERS];
	Framebuffer*		 taaResolvedFBO;
	Framebuffer*		 envprobeFBO;
	Framebuffer*		 bloomRenderFBO[MAX_BLOOM_BUFFERS];
	Framebuffer*		 glowFBO[MAX_GLOW_BUFFERS]; // unused
	Framebuffer*		 transparencyFBO;			// unused
	Framebuffer*		 ambientOcclusionFBO[MAX_SSAO_BUFFERS];
	Framebuffer*		 csDepthFBO[MAX_HIERARCHICAL_ZBUFFERS];
	Framebuffer*		 geometryBufferFBO;
	Framebuffer*		 smaaInputFBO;
	Framebuffer*		 smaaEdgesFBO;
	Framebuffer*		 smaaBlendFBO;
	Framebuffer*		 guiRenderTargetFBO;
	Framebuffer*		 accumFBO;
	Framebuffer*		 vrPDAFBO;
	Framebuffer*		 vrHUDFBO;
	Framebuffer*		 vrStereoFBO[MAX_STEREO_BUFFERS];
	Framebuffer*		 vrHmdEyeFBO[MAX_STEREO_BUFFERS];
};

extern globalFramebuffers_t globalFramebuffers;

#endif // __FRAMEBUFFER_H__
