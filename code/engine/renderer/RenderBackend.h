/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2016-2017 Dustin Land
Copyright (C) 2017-2023 Robert Beckebans
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

#ifndef __RENDERER_BACKEND_H__
#define __RENDERER_BACKEND_H__

#include "RenderLog.h"

#include "Passes/CommonPasses.h"
#include "Passes/MipMapGenPass.h"
#include "Passes/SsaoPass.h"
#include "Passes/TonemapPass.h"
#include "Passes/TemporalAntiAliasingPass.h"

#include "PipelineCache.h"

struct tmu_t {
	unsigned int current2DMap;
	unsigned int current2DArray;
	unsigned int currentCubeMap;
};

const int MAX_MULTITEXTURE_UNITS = 14;

enum stencilFace_t { STENCIL_FACE_FRONT, STENCIL_FACE_BACK, STENCIL_FACE_NUM };

struct gfxImpParms_t {
	int x; // ignored in fullscreen
	int y; // ignored in fullscreen
	int width;
	int height;
	int fullScreen; // 0 = windowed, otherwise 1 based monitor number to go full screen on
	// -1 = borderless window for spanning multiple displays
	int displayHz;
	int multiSamples;
};

#define MAX_DEBUG_LINES	   16384
#define MAX_DEBUG_TEXT	   512
#define MAX_DEBUG_POLYGONS 8192

struct debugLine_t {
	idVec4 rgb;
	idVec3 start;
	idVec3 end;
	bool   depthTest;
	int	   lifeTime;
};

struct debugText_t {
	idStr  text;
	idVec3 origin;
	float  scale;
	idVec4 color;
	idMat3 viewAxis;
	int	   align;
	int	   lifeTime;
	bool   depthTest;
};

struct debugPolygon_t {
	idVec4	  rgb;
	idWinding winding;
	bool	  depthTest;
	int		  lifeTime;
};

void RB_SetMVP( const idRenderMatrix& mvp );
void RB_SetVertexColorParms( stageVertexColor_t svc );

/*
===========================================================================

idRenderBackend

all state modified by the back end is separated from the front end state

===========================================================================
*/
struct ImDrawData;

class IRenderPass;
class ForwardShadingPass;

/*!
	\class idRenderBackend
	\brief Manages backend rendering operations including command execution, state management, and post-processing effects.

	The idRenderBackend class serves as the core interface for handling rendering operations within the engine's backend. It orchestrates the execution of rendering commands, manages OpenGL state and
   resources, and provides functionality for stereo rendering, shadow mapping, and various post-processing effects. The class handles both 3D and 2D view rendering, manages texture binding and shader
   stages, and supports advanced features like temporal anti-aliasing, motion vectors, bloom, and ambient occlusion. It also includes functions for clearing framebuffers, setting up projection and
   view matrices, and performing various lighting and shading operations.

*/
class idRenderBackend
{
	friend class Framebuffer;
	friend class fhImmediateMode;

public:
	//! Initializes the render backend object.
	idRenderBackend();
	~idRenderBackend();

	//! Initializes the render backend system and sets up graphics device resources.
	void		Init();

	//! Performs cleanup operations for the rendering backend before shutdown.
	void		Shutdown();

	//! Executes backend rendering commands for 3D and 2D views, buffer management, and post-processing effects.
	void		ExecuteBackEndCommands( const emptyCommand_t* cmds );

	//! Renders the draw list twice with modifications for left and right eye views in stereo rendering mode
	void		StereoRenderExecuteBackEndCommands( const emptyCommand_t* const allCmds );

	//! Performs a blocking swap of OpenGL buffers, ensuring GPU idleness at vsync
	void		GL_BlockingSwapBuffers();

	//! Checks for changes in console variables that affect rendering and updates the rendering state accordingly.
	void		CheckCVars();

	//! Clears all cached rendering pipeline data and resets associated resources.
	void		ClearCaches();

	//! Renders ImGui draw lists using the provided draw data.
	static void ImGui_RenderDrawLists( ImDrawData* draw_data );

	//! Renders a draw surface with optional shadow counter support
	void		DrawElementsWithCounters( const drawSurf_t* surf, bool shadowCounter = false );

private:
	//! Draws a flicker box when the r_drawFlickerBox console variable is enabled.
	void DrawFlickerBox();

	//! Sets up the binding layout for the specified binding layout type.
	void GetCurrentBindingLayout( int bindingLayoutType );

	//! Resizes the rendering images to match the current window size.
	void ResizeImages();

	//! Renders a view with optional stereo rendering support
	void DrawViewInternal( const viewDef_t* viewDef, const int stereoEye, const stereoOrigin_t stereoOrigin );

	//! Draws a view with optional stereo rendering based on the specified eye.
	void DrawView( const void* data, const int stereoEye );

	//! Copies part of the current framebuffer to the specified image.
	void CopyRender( const void* data );

	//! Binds the appropriate image for a texture stage, handling cinematic frames and default images
	void BindVariableStageImage( const textureStage_t* texture, const float* shaderRegisters, nvrhi::ICommandList* commandList );

	//! Prepares texturing state for a shader stage based on the texture generation type and surface properties.
	void PrepareStageTexturing( const shaderStage_t* pStage, const drawSurf_t* surf );

	//! Finalizes texturing operations for a shader stage and surface.
	void FinishStageTexturing( const shaderStage_t* pStage, const drawSurf_t* surf );

	//! Resets the viewport and scissor settings to match the default camera view.
	void ResetViewportAndScissorToDefaultCamera( const viewDef_t* _viewDef );

	//! Populates the depth buffer with geometry from the provided draw surfaces, skipping translucent surfaces.
	void FillDepthBufferGeneric( const drawSurf_t* const* drawSurfs, int numDrawSurfs );

	//! Fills the depth buffer quickly by processing draw surfaces with specific sorting and coverage rules
	void FillDepthBufferFast( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Performs the blending of light for a list of draw surfaces.
	void T_BlendLight( const drawSurf_t* drawSurfs, const viewLight_t* vLight );

	/*!
		\brief Performs dual-texture blending of falloff and projection textures for light rendering

		This function combines two texture stages to render lighting effects by blending falloff and projection textures onto the framebuffer. It handles multiple shader stages and applies appropriate
	   OpenGL states for each stage. The function supports stereo rendering through the stereoOrigin parameter and skips execution if blend lights are disabled or if the input draw surfaces are null.
	   It uses a specialized shader program for the blending operation and manages texture binding and state changes appropriately.

		\param drawSurfs First set of drawable surfaces to apply the light blending to
		\param drawSurfs2 Second set of drawable surfaces to apply the light blending to
		\param vLight View light structure containing light shader and rendering parameters
		\param stereoOrigin Stereo rendering origin for handling stereo rendering cases
	*/
	void BlendLight( const drawSurf_t* drawSurfs, const drawSurf_t* drawSurfs2, const viewLight_t* vLight, const stereoOrigin_t stereoOrigin );

	//! Applies basic fog effect to draw surfaces using specified fog planes and transformation matrix
	void T_BasicFog( const drawSurf_t* drawSurfs, const idPlane fogPlanes[4], const idRenderMatrix* inverseBaseLightProject );

	/*!
		\brief Applies a fog effect to the scene using the specified lighting and stereo rendering parameters

		This function implements the fog pass rendering technique by calculating fog parameters from the provided light shader registers and applying them to the scene surfaces. It handles both
	   regular draw surfaces and a special cube surface for lighting frustum planes. The function sets up texture coordinates and fog plane equations based on the view origin and fog properties, then
	   performs rendering operations with specific OpenGL state configurations to achieve the desired fog effect. The function supports stereo rendering through the stereoOrigin parameter and uses a
	   combination of texture stages for proper fog falloff and enter plane corrections

		\param drawSurfs First set of draw surfaces to apply fog to
		\param drawSurfs2 Second set of draw surfaces to apply fog to
		\param vLight Light definition containing fog shader and parameters
		\param stereoOrigin Stereo rendering origin selection
	*/
	void FogPass( const drawSurf_t* drawSurfs, const drawSurf_t* drawSurfs2, const viewLight_t* vLight, const stereoOrigin_t stereoOrigin );

	//! Applies fog effects from all lights in the current view, handling both fog and blend lights.
	void FogAllLights( const stereoOrigin_t stereoOrigin );

	/*!
		\brief Sets up texture matrix and color values for shader interaction based on surface stage parameters.

		This function configures the texture transformation matrix and color values used in shader interactions. It handles both animated and static texture matrices by extracting values from the
	   surface registers and applying clamping to prevent overflow. The function also computes the final color by combining surface color registers with light color values, ensuring the result stays
	   within the [0,1] range.

		\param surfaceStage Pointer to the shader stage definition containing texture and color register information
		\param surfaceRegs Array of surface register values used for texture matrix and color calculations
		\param lightColor Color values of the light source affecting the surface
		\param matrix Output matrix for texture coordinate transformations
		\param color Output color values for the shader interaction
	*/
	void SetupInteractionStage( const shaderStage_t* surfaceStage, const float* surfaceRegs, const float lightColor[4], idVec4 matrix[2], float color[4] );

	//! Renders all lighting interactions for the given view definition and stereo origin
	void DrawInteractions( const viewDef_t* _viewDef, const stereoOrigin_t stereoOrigin );

	/*!
		\brief Renders a single interaction with optional fast path and IBL handling

		This function processes and renders a single interaction defined by the drawInteraction_t structure. It handles various rendering states and conditions including diffuse, specular, and bump
	   mapping parameters. The function supports fast path optimization and image-based lighting (IBL) with light grid integration. When IBL is enabled and light grid is used, it sets up proper
	   uniforms and texture bindings for radiance and irradiance calculations. The function also manages shader binding based on whether the surface is skinned and the texture usage type.

		\param din Pointer to the draw interaction structure containing rendering parameters
		\param useFastPath Flag indicating whether to use fast path rendering
		\param useIBL Flag indicating whether to use image-based lighting
		\param setInteractionShader Flag controlling whether to set the interaction shader
	*/
	void DrawSingleInteraction( drawInteraction_t* din, bool useFastPath, bool useIBL, bool setInteractionShader );

	/*!
		\brief Processes and renders shader passes for a set of draw surfaces with support for stereo rendering and gui depth offset calculations.

		This function iterates through a list of draw surfaces and renders their ambient shader passes. It handles stereo rendering by checking if the shader's stereo eye setting matches the current
	   view's stereo eye. For GUI surfaces, it applies a screen offset based on the surface sort value and gui stereo screen offset. It also manages OpenGL state changes including model-view matrices,
	   scissor rectangles, and blending states. The function skips surfaces that do not have ambient lighting, are portal skies, or are suppressed in subviews. It also ensures that post-processing
	   shaders are drawn after fog lights are processed.

		\param drawSurfs Pointer to an array of pointers to drawSurf_t structures representing the surfaces to render
		\param numDrawSurfs Number of drawSurfaces in the drawSurfs array
		\param guiStereoScreenOffset Offset value used for GUI stereo rendering calculations based on surface sort and stereo separation
		\param stereoEye Current stereo eye index (0 for both eyes, 1 for left, 2 for right)
		\param stereoOrigin Stereo origin type used for view origin calculations
		\return The number of draw surfaces processed, or numDrawSurfs if skipping ambient lighting is enabled
	*/
	int	 DrawShaderPasses( const drawSurf_t* const* const drawSurfs, const int numDrawSurfs, const float guiStereoScreenOffset, const int stereoEye, const stereoOrigin_t stereoOrigin );

	/*!
		\brief Renders interactions between surfaces and a light source with configurable depth and stencil testing

		This function processes a list of surfaces that are lit by a specific light source, rendering their interactions with appropriate OpenGL state setup. It handles both fast-path and complex
	   surfaces separately for optimization, and manages shadow rendering with jittering and atlas placement when applicable. The function also sets up scissor test parameters and configures stencil
	   operations based on whether a stencil test is required.

		\param surfList List of surfaces to render interactions for
		\param vLight Light source definition and properties
		\param depthFunc OpenGL depth function to use for rendering
		\param performStencilTest Whether to perform stencil testing for shadow rendering
		\param useLightDepthBounds Whether to use light depth bounds for rendering
		\param stereoOrigin Stereo rendering origin for the light
	*/
	void RenderInteractions( const drawSurf_t* surfList, const viewLight_t* vLight, int depthFunc, bool performStencilTest, bool useLightDepthBounds, stereoOrigin_t stereoOrigin );

	/*!
		\brief Executes an ambient pass rendering operation for a set of draw surfaces, optionally filling the geometry buffer.

		This function performs the ambient pass rendering for a list of draw surfaces. It handles the setup of the rendering environment, including framebuffer binding and state configuration. The
	   function supports two modes: filling the geometry buffer or performing the ambient pass rendering. It includes logic to skip rendering under certain conditions such as when the GUI is active or
	   when ambient rendering is disabled. The function also manages the rendering of surfaces with proper material handling and shader parameter setup. It supports both regular and irradiance
	   rendering modes with different lighting calculations.

		\param drawSurfs Array of pointers to draw surfaces to render
		\param numDrawSurfs Number of draw surfaces in the array
		\param fillGbuffer Flag indicating whether to fill the geometry buffer
		\param stereoOrigin Stereo rendering origin information
	*/
	void AmbientPass( const drawSurf_t* const* drawSurfs, int numDrawSurfs, bool fillGbuffer, const stereoOrigin_t stereoOrigin );

	/*!
		\brief Sets up the view and projection matrices for shadow mapping for a specific light and shadow map side.

		This function configures the view and projection matrices used for rendering shadow maps. For parallel lights, it calculates the light's view matrix based on the light direction and the
	   current view position, then computes the projection matrix to fit the light's frustum bounds. For point lights, it sets up the view matrix for each of the six faces of the light's cubemap. The
	   function also applies necessary transformations to ensure the shadow maps are properly aligned with the light's perspective and covers the relevant scene geometry.

		\param vLight Pointer to the view light structure containing light properties
		\param side Index of the shadow map side (0-5 for point lights, 0-5 for parallel lights)
		\param lightProjectionRenderMatrix Output matrix representing the projection transformation for the shadow map
		\param lightViewRenderMatrix Output matrix representing the view transformation for the shadow map
		\param stereoOrigin Specifies which eye to use for stereo rendering (left or right)
		\throws Assertion failure if side is out of bounds
	*/
	void SetupShadowMapMatrices( viewLight_t* vLight, int side, idRenderMatrix& lightProjectionRenderMatrix, idRenderMatrix& lightViewRenderMatrix, const stereoOrigin_t stereoOrigin );

	/*!
		\brief Renders shadow maps for a given set of draw surfaces using a fast rendering path

		This function performs the shadow map rendering pass for a specific light source and side. It handles the setup of render states, projection and view matrices for shadow mapping, and iterates
	   through the provided draw surfaces to render them into the shadow map. The function skips rendering if shadow rendering is disabled, handles different culling modes based on configuration, and
	   properly manages both atlas and non-atlas shadow map rendering. It also handles special cases for perforated surfaces by deferring their rendering to a separate helper function.

		\param drawSurfs Array of draw surfaces to render into the shadow map
		\param vLight Light definition for which to generate the shadow map
		\param side The side of the shadow map to render (for cube maps or stereo rendering)
		\param atlas Whether to render into an atlas texture
		\param stereoOrigin Stereo rendering origin for stereo shadow maps
	*/
	void ShadowMapPassFast( const drawSurf_t* drawSurfs, viewLight_t* vLight, int side, bool atlas, const stereoOrigin_t stereoOrigin );

	/*!
		\brief Renders shadow maps for perforated surfaces using the specified lighting and matrix parameters

		This function processes a list of draw surfaces to generate shadow maps for perforated materials, which are surfaces with alpha-tested textures that may have multiple transparent stages. It
	   handles different lighting types including parallel (sun), spot, and point lights, and applies appropriate stencil and depth testing settings. The function processes each surface in the
	   provided draw array, setting up the correct rendering state and matrix transformations for shadow map generation. It specifically handles surfaces with MC_PERFORATED coverage type by iterating
	   through alpha-tested shader stages and rendering them appropriately while maintaining proper depth offset settings to prevent shadow acne.

		\param drawSurfs Array of draw surfaces to process for shadow map generation
		\param numDrawSurfs Number of draw surfaces in the array
		\param vLight Pointer to the view light structure containing lighting information
		\param side Side parameter for light type determination (negative for spot light, non-negative for point light)
		\param lightProjectionRenderMatrix Light projection matrix used for shadow map rendering
		\param lightViewRenderMatrix Light view matrix used for shadow map rendering
	*/
	void ShadowMapPassPerforated(
		const drawSurf_t** drawSurfs, int numDrawSurfs, viewLight_t* vLight, int side, const idRenderMatrix& lightProjectionRenderMatrix, const idRenderMatrix& lightViewRenderMatrix );

	//! Performs the shadow atlas pass for rendering shadows using an atlas texture.
	void ShadowAtlasPass( const viewDef_t* _viewDef, const stereoOrigin_t stereoOrigin );

	//! Draws motion vectors for temporal anti-aliasing and motion blur effects in stereo rendering.
	void DrawMotionVectors( const int stereoEye );

	//! Performs a temporal antialiasing pass for the given view definition and stereo eye.
	void TemporalAAPass( const viewDef_t* _viewDef, const int stereoEye );

	//! Applies bloom effect to the current render view
	void Bloom( const viewDef_t* viewDef );

	//! Draws screen-space ambient occlusion for the specified view definition.
	void DrawScreenSpaceAmbientOcclusion( const viewDef_t* _viewDef );

	//! Renders screen-space ambient occlusion using compute shaders for the specified view.
	void DrawScreenSpaceAmbientOcclusion2( const viewDef_t* _viewDef );

	//! Applies motion blur effect to the current render view
	void MotionBlur();

	//! Performs post-processing operations on rendered data including SMAA anti-aliasing and filmic post-processing effects.
	void PostProcess( const void* data );

	//! Applies CRT post-processing effects to the rendered image
	void CRTPostProcess();

private:
	//! Initializes the rendering backend for a new frame
	void GL_StartFrame();

	//! Ends the current frame and prepares for the next one.
	void GL_EndFrame();

public:
	//! Returns the current OpenGL state bits tracked by the render backend
	uint64				 GL_GetCurrentState() const;

	//! Returns the current pixel offset for temporal anti-aliasing based on the given frame index.
	idVec2				 GetCurrentPixelOffset( int frameIndex ) const;

	//! Returns the current command list used for rendering operations.
	nvrhi::ICommandList* GL_GetCommandList() const { return commandList; }

private:
	//! Returns the current OpenGL state with stencil bits cleared.
	uint64		   GL_GetCurrentStateMinusStencil() const;

	//! Resets OpenGL state to default values.
	void		   GL_SetDefaultState();

	//! Sets the OpenGL state bits for rendering operations, with optional force flag.
	void		   GL_State( uint64 stateBits, bool forceGlState = false );

	//! Selects the specified texture unit for subsequent OpenGL operations.
	void		   GL_SelectTexture( int unit );

	/*!
		\brief Clears the specified framebuffer attachments with the given color and depth/stencil values

		This function clears various framebuffer attachments based on the provided flags. It can clear color, depth, and stencil buffers, and supports additional clearing for HDR and VR framebuffers.
	   The color is specified as RGBA values, and the stencil value is used when stencil clearing is enabled. The function handles different stereo eye configurations for VR rendering

		\param color Indicates whether to clear the color buffer
		\param depth Indicates whether to clear the depth buffer
		\param stencil Indicates whether to clear the stencil buffer
		\param stencilValue The value to clear the stencil buffer with
		\param r Red component of the clear color
		\param g Green component of the clear color
		\param b Blue component of the clear color
		\param a Alpha component of the clear color
		\param clearHDR Indicates whether to clear HDR framebuffers
		\param clearVR Indicates whether to clear VR framebuffers
		\param stereoEye The stereo eye to clear, typically 0 for left eye, 1 for right eye
	*/
	void		   GL_Clear( bool color, bool depth, bool stencil, byte stencilValue, float r, float g, float b, float a, bool clearHDR = false, bool clearVR = false, const int stereoEye = 0 );

	//! Configures depth bounds testing with the specified minimum and maximum depth values.
	void		   GL_DepthBoundsTest( const float zmin, const float zmax );

	//! Sets the polygon offset scale and bias values for the render backend.
	void		   GL_PolygonOffset( float scale, float bias );

	/*!
		\brief Configures the OpenGL scissor rectangle for clipping rendering to a specified viewport area

		This function sets up the OpenGL scissor rectangle using the provided coordinates and dimensions. The scissor rectangle defines the rectangular region where rendering will occur, with all
	   pixels outside this region being clipped. The function calculates the scissor rectangle from the provided parameters and updates the context's scissor state. The coordinate system uses (0,0) at
	   the upper-left corner, and the function handles the conversion from typical graphics coordinates to OpenGL's coordinate system. It is commonly used for efficient rendering by limiting the area
	   that needs to be updated.

		\param x The x-coordinate of the lower-left corner of the scissor rectangle
		\param y The y-coordinate of the lower-left corner of the scissor rectangle
		\param w The width of the scissor rectangle
		\param h The height of the scissor rectangle
	*/
	void		   GL_Scissor( int x, int y, int w, int h );

	/*!
		\brief Sets the current viewport dimensions using the specified coordinates and dimensions

		This function configures the viewport by defining its boundaries using a left coordinate, bottom coordinate, width, and height. The viewport is stored in the currentViewport member variable
	   and is updated to encompass the specified rectangular area. The function clears any previous viewport settings and sets the new viewport based on the provided parameters.

		\param x The x-coordinate of the viewport's left edge
		\param y The y-coordinate of the viewport's bottom edge
		\param w The width of the viewport
		\param h The height of the viewport
	*/
	void		   GL_Viewport( int x, int y, int w, int h );

	//! Sets the scissor rectangle for rendering operations.
	ID_INLINE void GL_Scissor( const idScreenRect& rect ) { GL_Scissor( rect.x1, rect.y1, rect.x2 - rect.x1 + 1, rect.y2 - rect.y1 + 1 ); }

	//! Sets the OpenGL viewport to the specified screen rectangle.
	ID_INLINE void GL_Viewport( const idScreenRect& rect ) { GL_Viewport( rect.x1, rect.y1, rect.x2 - rect.x1 + 1, rect.y2 - rect.y1 + 1 ); }

	/*!
		\brief Sets both the viewport and scissor parameters for rendering.

		This function configures the OpenGL viewport and scissor area using the provided coordinates and dimensions. It calls GL_Viewport to set the viewport and GL_Scissor to set the scissor
	   rectangle, effectively defining the area of the screen where rendering will occur.

		\param x The x-coordinate of the viewport and scissor area
		\param y The y-coordinate of the viewport and scissor area
		\param w The width of the viewport and scissor area
		\param h The height of the viewport and scissor area
	*/
	ID_INLINE void GL_ViewportAndScissor( int x, int y, int w, int h )
	{
		GL_Viewport( x, y, w, h );
		GL_Scissor( x, y, w, h );
	}

	//! Sets both the viewport and scissor rectangle for rendering.
	ID_INLINE void GL_ViewportAndScissor( const idScreenRect& rect )
	{
		GL_Viewport( rect );
		GL_Scissor( rect );
	}

	/*!
		\brief Sets the color for subsequent rendering operations by clamping and storing RGBA color values.

		This function takes four floating-point color components (red, green, blue, and alpha) and clamps each to the range [0.0, 1.0]. The clamped values are then stored in a parameter array and
	   passed to the render program manager for use in subsequent rendering operations. The function does not perform any immediate rendering but instead prepares the color state for later use.

		\param r Red color component, will be clamped to [0.0, 1.0]
		\param g Green color component, will be clamped to [0.0, 1.0]
		\param b Blue color component, will be clamped to [0.0, 1.0]
		\param a Alpha color component, will be clamped to [0.0, 1.0]
	*/
	void		   GL_Color( float r, float g, float b, float a );

	//! Sets the color for subsequent OpenGL rendering operations.
	ID_INLINE void GL_Color( float r, float g, float b ) { GL_Color( r, g, b, 1.0f ); }

	//! Sets the current color for OpenGL rendering using the provided RGB values with full opacity.
	ID_INLINE void GL_Color( const idVec3& color ) { GL_Color( color[0], color[1], color[2], 1.0f ); }

	//! Sets the current color for OpenGL rendering using a 4-component vector.
	ID_INLINE void GL_Color( const idVec4& color ) { GL_Color( color[0], color[1], color[2], color[3] ); }

	//! Sets the render buffer for drawing the frame to the specified stereo eye.
	void		   SetBuffer( const void* data, const int stereoEye );

private:
	//! Sets up the rendering state for a draw surface including matrix and scissor settings.
	void DBG_SimpleSurfaceSetup( const drawSurf_t* drawSurf );

	//! Sets up the world space for debug rendering.
	void DBG_SimpleWorldSetup();

	//! Clears the entire screen with normal rasterization while disabling texturing.
	void DBG_PolygonClear();

	//! Configures OpenGL state to display destination alpha information for debugging purposes.
	void DBG_ShowDestinationAlpha();

	//! Scans the stencil buffer for debugging purposes.
	void DBG_ScanStencilBuffer();

	//! Counts stencil buffer values for overdraw analysis.
	void DBG_CountStencilBuffer();

	//! Sets the screen colors based on the contents of the stencil buffer.
	void DBG_ColorByStencilBuffer();

	//! Displays overdraw information by replacing materials on draw surfaces with an overdraw test material
	void DBG_ShowOverdraw();

	//! Debugging tool to visualize the dynamic range of a scene by showing the greatest RGB value at each pixel.
	void DBG_ShowIntensity();

	//! Displays the depth buffer as colors for debugging purposes.
	void DBG_ShowDepthBuffer();

	//! Displays the number of lights affecting each surface in the scene for debugging purposes
	void DBG_ShowLightCount();

	//! Renders a list of draw surfaces using a debug rendering function with specific matrix and color handling.
	void DBG_RenderDrawSurfListWithFunction( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays silhouette edges for debugging purposes
	void DBG_ShowSilhouette();

	//! Displays wireframe triangles for debugging purposes based on the specified visibility mode.
	void DBG_ShowTris( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays surface information for debugging purposes
	void DBG_ShowSurfaceInfo( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays view entity information for debugging purposes
	void DBG_ShowViewEntitys( viewEntity_t* vModels );

	//! Displays texture polarity information for surfaces by shading triangles red, green, or blue based on their texture area.
	void DBG_ShowTexturePolarity( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays debug information for materials using unsmoothed tangents
	void DBG_ShowUnsmoothedTangents( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Visualizes the tangent space of surfaces by shading triangles based on tangent and normal vectors
	void DBG_ShowTangentSpace( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays vertex colors for the specified draw surfaces.
	void DBG_ShowVertexColor( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Debugging tool to display surface normals.
	void DBG_ShowNormals( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Draws texture vectors at the center of each triangle in the provided draw surfaces.
	void DBG_ShowTextureVectors( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays lines from each vertex to the dominant triangle center for debugging purposes
	void DBG_ShowDominantTris( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Displays edges for the specified draw surfaces for debugging purposes.
	void DBG_ShowEdges( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Visualizes light volumes in the current scene based on the r_showLights console variable setting.
	void DBG_ShowLights();

	//! Displays the light grid visualization for debugging purposes.
	void DBG_ShowLightGrid();

	//! Displays view environment probes in the debug render output.
	void DBG_ShowViewEnvprobes();

	//! Displays shadow map levels of detail for debugging purposes
	void DBG_ShowShadowMapLODs();

	//! Displays portals in the render view for debugging purposes
	void DBG_ShowPortals();

	//! Displays debug text overlay in the render output.
	void DBG_ShowDebugText();

	//! Displays debug lines in the render backend.
	void DBG_ShowDebugLines();

	//! Displays debug polygons in the render backend.
	void DBG_ShowDebugPolygons();

	//! Displays the center of projection for debugging purposes.
	void DBG_ShowCenterOfProjection();

	//! Draws exact pixel lines to check pixel center sampling.
	void DBG_ShowLines();

	//! Tests gamma correction functionality.
	void DBG_TestGamma();

	//! Performs a gamma bias test operation.
	void DBG_TestGammaBias();

	//! Displays a test image or video over most of the screen for debugging purposes.
	void DBG_TestImage();

	//! Displays shadow maps for debugging purposes.
	void DBG_ShowShadowMaps();

	//! Displays trace information for the specified draw surfaces.
	void DBG_ShowTrace( drawSurf_t** drawSurfs, int numDrawSurfs );

	//! Renders debug visualization tools for the rendering backend
	void DBG_RenderDebugTools( drawSurf_t** drawSurfs, int numDrawSurfs );

public:
	backEndCounters_t pc;

	// surfaces used for code-based drawing
	drawSurf_t		  unitSquareSurface;
	drawSurf_t		  zeroOneCubeSurface;
	drawSurf_t		  zeroOneSphereSurface; // RB
	drawSurf_t		  testImageSurface;

	float			  slopeScaleBias;
	float			  depthBias;

private:
	uint64				glStateBits; // for all render APIs

	const viewDef_t*	viewDef;

	const viewEntity_t* currentSpace;	// for detecting when a matrix must change
	idScreenRect		currentScissor; // for scissor clipping, local inside renderView viewport

	bool				currentRenderCopied; // true if any material has already referenced _currentRender

	idRenderMatrix		prevMVP[2]; // world MVP from previous frame for motion blur
	bool				prevViewsValid[2];

	// RB begin
	// TODO remove
	float				hdrAverageLuminance;
	float				hdrMaxLuminance;
	float				hdrTime;
	float				hdrKey;

	// quad-tree for managing tiles within tiled shadow map
	TileMap				tileMap;

private:
	idScreenRect																					   stateViewport;
	idScreenRect																					   stateScissor;

	idScreenRect																					   currentViewport;
	nvrhi::BufferHandle																				   currentVertexBuffer;
	uint																							   currentVertexOffset;
	nvrhi::BufferHandle																				   currentIndexBuffer;
	uint																							   currentIndexOffset;
	nvrhi::BindingLayoutHandle																		   currentBindingLayout;
	nvrhi::IBuffer*																					   currentJointBuffer;
	uint																							   currentJointOffset;
	nvrhi::GraphicsPipelineHandle																	   currentPipeline;

	idStaticList<nvrhi::BindingSetHandle, nvrhi::c_MaxBindingLayouts>								   currentBindingSets;
	idStaticList<idStaticList<nvrhi::BindingSetDesc, nvrhi::c_MaxBindingLayouts>, NUM_BINDING_LAYOUTS> pendingBindingSetDescs;

	Framebuffer*																					   currentFrameBuffer;
	Framebuffer*																					   lastFrameBuffer;
	nvrhi::CommandListHandle																		   commandList;
	CommonRenderPasses																				   commonPasses;
	SsaoPass*																						   ssaoPass;
	MipMapGenPass*																					   hiZGenPass;
	TonemapPass*																					   toneMapPass;
	TemporalAntiAliasingPass*																		   taaPass[2]; // two separate history buffers for VR

	BindingCache																					   bindingCache;
	SamplerCache																					   samplerCache;
	PipelineCache																					   pipelineCache;

	nvrhi::InputLayoutHandle																		   inputLayout;

	nvrhi::ShaderHandle																				   vertexShader;
	nvrhi::ShaderHandle																				   pixelShader;

	int																								   prevBindingLayoutType;

public:
	//! Clears the pipeline cache.
	void				ResetPipelineCache();

	//! Sets the current image for rendering operations.
	void				SetCurrentImage( idImage* image );

	//! Returns the current image being processed by the render backend.
	idImage*			GetCurrentImage();

	//! Returns the image at the specified index from the render backend context.
	idImage*			GetImageAt( int index );

	//! Returns a reference to the common rendering passes object.
	CommonRenderPasses& GetCommonPasses() { return commonPasses; }
};

#endif
