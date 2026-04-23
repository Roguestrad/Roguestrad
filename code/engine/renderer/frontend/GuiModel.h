/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2020 Robert Beckebans
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

#include "../ScreenRect.h"

struct guiModelSurface_t {
	const idMaterial* material;
	uint64			  glState;
	int				  firstIndex;
	int				  numIndexes;
	stereoDepthType_t stereoType;
	idScreenRect	  clipRect;
};

class idRenderMatrix;
class Framebuffer;

struct ImDrawData;

/*!
	\class idGuiModel
	\brief This class is a helper to batch 2D rendering surfaces in the renderer frontend.

*/
class idGuiModel
{
public:
	//! Initializes a new instance of the idGuiModel class with default values.
	idGuiModel();

	//! Clears all surfaces from the GUI model.
	void		Clear();

	//! Sets the view eye buffer for the GUI model.
	void		SetViewEyeBuffer( int veb );

	//! Returns the view eye buffer index used for GUI rendering.
	int			GetViewEyeBuffer() { return viewEyeBuffer; }

	//! Sets the GUI mode and triggers a full screen event if the mode changes.
	void		SetMode( guiMode_t a_mode );

	//! Returns the current GUI mode of the model
	guiMode_t	GetMode() { return mode; }

	//! Updates the virtual reality shell origin and axis based on VR system data
	bool		UpdateVRShell();

	//! Activates or deactivates the VR shell based on the boolean parameter.
	void		ActivateVRShell( bool b );

	//! Retrieves the VR shell origin and axis for GUI rendering.
	bool		GetVRShell( idVec3& origin, idMat3& axis );

	//! Sets the VR shell origin and axis for the GUI model.
	void		SetVRShell( const idVec3& origin, const idMat3& axis );

	//! Initializes the GUI model frame by allocating vertex and index buffers and clearing the state.
	void		BeginFrame();

	//! Emits the GUI model to the current view with the specified model matrix and depth hack option.
	void		EmitToCurrentView( float modelMatrix[16], bool depthHack );

	//! Creates a full-screen view and emits GUI surfaces for rendering
	void		EmitFullScreen( Framebuffer* renderTarget = nullptr );

	/*!
		\brief Emits GUI surfaces with optional stereo depth effects and depth hacking for rendering

		This function processes and submits GUI surfaces for rendering with support for stereo 3D effects and depth hacking. It allocates rendering space for each surface, sets up transformation
	   matrices, and handles surface linking when required. The function also evaluates shader registers for each surface and manages scissor rectangles for clipping. When stereo depth is enabled, it
	   adjusts surface sorting and modifies vertex z-coordinates to create depth effects for virtual reality systems.

		\param modelMatrix The model transformation matrix for the GUI surface
		\param modelViewMatrix The model-view transformation matrix for the GUI surface
		\param depthHack Flag to enable or disable depth hacking effect
		\param allowFullScreenStereoDepth Flag to enable full-screen stereo depth effects for 3D displays
		\param linkAsEntity Flag to determine if the GUI surface should be linked as an entity in the rendering system
	*/
	void		EmitSurfaces( float modelMatrix[16], float modelViewMatrix[16], bool depthHack, bool allowFullScreenStereoDepth, bool linkAsEntity );

	//! Emits ImGui draw data to the rendering system
	void		EmitImGui( ImDrawData* drawData );

	/*!
		\brief Allocates vertex data for triangle primitives with the specified parameters and returns a pointer to the allocated vertices.

		This function allocates memory for vertex data used in rendering triangle primitives. The returned pointer points to write-combined memory, which means only 32-bit contiguous writes should be
	   performed and reads are not recommended. The function handles clipping and uses an internal clipping rectangle. It validates input parameters and returns NULL if material is NULL or if
	   index/vertex limits are exceeded.

		\param numVerts Number of vertices to allocate
		\param indexes Array of triangle indices referencing the vertices
		\param numIndexes Number of indices in the index array
		\param material Pointer to the material used for rendering
		\param glState OpenGL state flags for the rendering operation
		\param stereoType Type of stereo depth for the rendering operation
		\return Pointer to the allocated idDrawVert array for vertex data, or NULL if allocation fails
		\throws No explicit throws, but may warn about exceeding MAX_INDEXES or MAX_VERTS limits
	*/
	idDrawVert* AllocTris( int numVerts, const triIndex_t* indexes, int numIndexes, const idMaterial* material, const uint64 glState, const stereoDepthType_t stereoType );

	/*!
		\brief Allocates vertex and index data for rendering triangles with specified material and rendering state

		This function allocates vertex and index data for rendering triangle primitives. It manages vertex and index buffers, handling buffer reallocation when necessary and ensuring proper alignment.
	   The function also handles surface state changes by breaking the current surface when material, OpenGL state, stereo type, or clip rectangle changes. It supports both aligned and unaligned index
	   writing for performance optimization. The function returns a pointer to the allocated vertex data that can be filled by the caller.

		\param numVerts Number of vertices already allocated in the current surface
		\param indexes Array of triangle indices to be added
		\param numIndexes Number of indices to be added
		\param material Material to be used for the rendered triangles
		\param glState OpenGL state flags for rendering
		\param stereoType Stereo depth type for the rendering
		\param clipRect Screen clipping rectangle for the surface
		\return Pointer to the allocated vertex data that can be filled by the caller
		\throws idLib::Warning when MAX_INDEXES or MAX_VERTS limits are exceeded
	*/
	idDrawVert* AllocTris(
		int numVerts, const triIndex_t* indexes, int numIndexes, const idMaterial* material, const uint64 glState, const stereoDepthType_t stereoType, const idScreenRect& clipRect );

	//---------------------------
private:
	//! Advances to the next surface in the GUI model, preparing it for rendering.
	void								 AdvanceSurf();

	// Leyland VR
	int									 viewEyeBuffer; // -1 = left eye, 1 = right eye, 0 = monoscopic view or GUI

	guiMode_t							 mode;

	bool								 vrShellActive;
	bool								 vrShellNeedsUpdate;
	idVec3								 vrShellOrigin;
	idMat3								 vrShellAxis;
	// Leyland end

	guiModelSurface_t*					 surf;

	float								 shaderParms[MAX_ENTITY_SHADER_PARMS];

	static const float					 STEREO_DEPTH_DISABLE; // Leyland VR
	static const float					 STEREO_DEPTH_NEAR;
	static const float					 STEREO_DEPTH_MID;
	static const float					 STEREO_DEPTH_FAR;

	// if we exceed these limits we stop rendering GUI surfaces
	static const int					 MAX_INDEXES = ( 20000 * 6 );
	static const int					 MAX_VERTS	 = ( 20000 * 4 );

	vertCacheHandle_t					 vertexBlock;
	vertCacheHandle_t					 indexBlock;
	idDrawVert*							 vertexPointer;
	triIndex_t*							 indexPointer;

	int									 numVerts;
	int									 numIndexes;

	idList<guiModelSurface_t, TAG_MODEL> surfaces;
};
