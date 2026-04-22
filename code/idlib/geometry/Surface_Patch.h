/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#ifndef __SURFACE_PATCH_H__
#define __SURFACE_PATCH_H__

/*!
	\class idSurface_Patch
	\brief A patch surface implementation for handling subdivision and manipulation of patch meshes.

	This class represents a patch surface that can be subdivided and manipulated to generate detailed mesh geometry. It inherits from idSurface and provides functionality for setting dimensions,
   subdividing patches based on error thresholds or explicit counts, placing points on curves, and managing vertex data. The class supports operations like expanding and collapsing patch surfaces,
   generating normals and indexes, and projecting points onto vectors for error calculation. It is designed to work with control points to define patch geometry and supports both implicit and explicit
   subdivision methods.

*/
class idSurface_Patch : public idSurface
{
public:
	//! Initializes a new instance of the idSurface_Patch class with default values.
	idSurface_Patch();

	//! Initializes a new patch surface with specified maximum dimensions.
	idSurface_Patch( int maxPatchWidth, int maxPatchHeight );

	//! Constructs a new idSurface_Patch object as a copy of an existing patch.
	idSurface_Patch( const idSurface_Patch& patch );
	~idSurface_Patch();

	//! Sets the width and height of the patch surface
	void SetSize( int patchWidth, int patchHeight );

	//! Returns the width of the patch surface
	int	 GetWidth() const;

	//! Returns the height dimension of the patch surface.
	int	 GetHeight() const;

	//! Subdivides a patch mesh based on specified error thresholds and maximum length constraints.
	void Subdivide( float maxHorizontalError, float maxVerticalError, float maxLength, bool genNormals = false );

	//! Subdivides the patch using explicit horizontal and vertical subdivision counts
	void SubdivideExplicit( int horzSubdivisions, int vertSubdivisions, bool genNormals, bool removeLinear = false );

protected:
	int	 width;		// width of patch
	int	 height;	// height of patch
	int	 maxWidth;	// maximum width allocated for
	int	 maxHeight; // maximum height allocated for
	bool expanded;	// true if vertices are spaced out

private:
	//! Places approximation points on the curve for the patch surface.
	void PutOnCurve();

	//! Removes columns and rows from the patch where all points lie on a straight line.
	void RemoveLinearColumnsRows();

	//! Resizes the vertex buffer for the expanded patch surface to the specified dimensions.
	void ResizeExpanded( int height, int width );

	//! Expands the patch to fill the maximum width and height dimensions
	void Expand();

	//! Collapses the patch by moving all points to the start of the verts buffer.
	void Collapse();

	//! Projects a point onto a vector to calculate maximum curve error.
	void ProjectPointOntoVector( const idVec3& point, const idVec3& vStart, const idVec3& vEnd, idVec3& vProj );

	//! Generates surface normals for all vertices in the patch
	void GenerateNormals();

	//! Generates triangle indexes for the patch surface.
	void GenerateIndexes();

	//! Linearly interpolates between two draw vertices and stores the result in an output vertex.
	void LerpVert( const idDrawVert& a, const idDrawVert& b, idDrawVert& out ) const;

	//! Samples a single point on a 3x3 patch given u and v coordinates
	void SampleSinglePatchPoint( const idDrawVert ctrl[3][3], float u, float v, idDrawVert* out ) const;

	//! Samples a single patch into the output vertex array
	void SampleSinglePatch( const idDrawVert ctrl[3][3], int baseCol, int baseRow, int width, int horzSub, int vertSub, idDrawVert* outVerts ) const;
};

ID_INLINE idSurface_Patch::idSurface_Patch()
{
	height = width = maxHeight = maxWidth = 0;
	expanded							  = false;
}

ID_INLINE idSurface_Patch::idSurface_Patch( int maxPatchWidth, int maxPatchHeight )
{
	width = height = 0;
	maxWidth	   = maxPatchWidth;
	maxHeight	   = maxPatchHeight;
	verts.SetNum( maxWidth * maxHeight );
	expanded = false;
}

/*
=================
idSurface_Patch::idSurface_Patch
=================
*/
ID_INLINE idSurface_Patch::idSurface_Patch( const idSurface_Patch& patch )
{
	( *this ) = patch;
}

/*
=================
idSurface_Patch::~idSurface_Patch
=================
*/
ID_INLINE idSurface_Patch::~idSurface_Patch()
{
}

ID_INLINE int idSurface_Patch::GetWidth() const
{
	return width;
}

ID_INLINE int idSurface_Patch::GetHeight() const
{
	return height;
}

#endif /* !__SURFACE_PATCH_H__ */
