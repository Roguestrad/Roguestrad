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
	\brief A patch surface implementation for representing and manipulating subdivision surfaces in the engine.

	The idSurface_Patch class provides functionality for creating, subdividing, and rendering patch surfaces which are used to represent smooth curved surfaces in the engine. It inherits from
   idSurface and extends its capabilities to handle patch-specific operations like subdivision, normal generation, and vertex manipulation. The class supports both implicit subdivision based on error
   thresholds and explicit subdivision with fixed numbers of divisions. It manages vertex data through an expanded buffer system that allows for efficient memory allocation and processing. The patch
   can be manipulated through various operations like putting points on the curve, removing linear columns and rows, and generating triangle indexes for rendering. The implementation supports both
   uniform and non-uniform subdivision approaches while maintaining proper vertex interpolation and normal calculation for smooth surface representation.

*/
class idSurface_Patch : public idSurface
{
public:
	/*!
		\brief Initializes a new instance of the idSurface_Patch class with default values.

		This constructor initializes all member variables of the idSurface_Patch class to their default values. The width, height, maxHeight, and maxWidth are set to zero, indicating no dimensions
	   have been defined yet. The expanded flag is set to false, indicating that the patch has not been expanded or subdivided.

	*/
	idSurface_Patch();

	/*!
		\brief Constructs an idSurface_Patch object with specified maximum dimensions

		Initializes a patch surface with given maximum width and height parameters. The actual width and height are set to zero initially, and the vertex buffer is allocated with the maximum possible
	   size. The expanded flag is initialized to false.

		\param maxPatchWidth Maximum allowed width of the patch surface
		\param maxPatchHeight Maximum allowed height of the patch surface
	*/
	idSurface_Patch( int maxPatchWidth, int maxPatchHeight );

	/*!
		\brief Creates a new idSurface_Patch object as a copy of an existing patch object.

		This constructor initializes a new idSurface_Patch object by copying the data from an existing idSurface_Patch object. It uses the assignment operator to perform the copy operation,
	   effectively creating a deep copy of the source patch. This constructor is typically used when duplicating patch data for processing or modification while preserving the original patch's
	   properties and structure.

		\param patch The source idSurface_Patch object to copy data from
	*/
	idSurface_Patch( const idSurface_Patch& patch );
	~idSurface_Patch();

	/*!
		\brief Sets the width and height of the patch surface and initializes the vertex array accordingly.

		This function configures the dimensions of the patch surface by validating the provided width and height parameters against maximum allowed values. It then updates the internal width and
	   height members and resizes the vertex array to accommodate the new dimensions. The function will cause a fatal error if the provided dimensions are invalid.

		\param patchWidth The new width of the patch surface, must be between 1 and maxWidth inclusive
		\param patchHeight The new height of the patch surface, must be between 1 and maxHeight inclusive
		\throws Fatal error when patchWidth is less than 1 or greater than maxWidth, or when patchHeight is less than 1 or greater than maxHeight
	*/
	void SetSize( int patchWidth, int patchHeight );

	//! Returns the width of the patch surface.
	int	 GetWidth() const;

	//! Returns the height of the patch surface.
	int	 GetHeight() const;

	/*!
		\brief Subdivides a patch surface based on specified error thresholds and maximum edge length.

		This function recursively subdivides a patch surface in both horizontal and vertical directions to ensure the surface adheres to specified error tolerances. It operates on the control mesh of
	   the patch, first generating normals if requested, then checking edge lengths and deviation from the control points. If subdivision is needed, it inserts new vertices and reorganizes the mesh
	   structure. The process continues until all segments meet the error criteria. Finally, it cleans up the mesh by removing linear columns and rows and normalizes the normals if they were
	   generated.

		\param maxHorizontalError Maximum allowed horizontal deviation from the control mesh
		\param maxVerticalError Maximum allowed vertical deviation from the control mesh
		\param maxLength Maximum allowed edge length, or 0 to disable length checks
		\param genNormals Flag indicating whether to generate normals for the control mesh
	*/
	void Subdivide( float maxHorizontalError, float maxVerticalError, float maxLength, bool genNormals = false );

	/*!
		\brief Subdivides a patch surface into a specified number of horizontal and vertical subdivisions while optionally generating normals and removing linear columns and rows.

		This function takes a patch surface and subdivides it based on the provided horizontal and vertical subdivision counts. It first generates normals for the control mesh if requested, then
	   samples individual patches to create a new, finer subdivision of the surface. The resulting vertices replace the original ones, updating the surface dimensions. If the removeLinear flag is set,
	   it processes the expanded surface to remove linear columns and rows. Finally, it generates index data for the new subdivided surface.

		\param horzSubdivisions Number of subdivisions in the horizontal direction
		\param vertSubdivisions Number of subdivisions in the vertical direction
		\param genNormals Flag indicating whether to generate normals for the control mesh
		\param removeLinear Flag indicating whether to remove linear columns and rows after subdivision
	*/
	void SubdivideExplicit( int horzSubdivisions, int vertSubdivisions, bool genNormals, bool removeLinear = false );

protected:
	int	 width;		// width of patch
	int	 height;	// height of patch
	int	 maxWidth;	// maximum width allocated for
	int	 maxHeight; // maximum height allocated for
	bool expanded;	// true if vertices are spaced out

private:
	/*!
		\brief Moves approximation points to lie on the curve by averaging adjacent vertices.

		This function adjusts the positions of vertices in a patch surface to ensure they lie on the actual curve rather than just approximating it. It iterates through the patch grid and for each
	   vertex, computes the average of its neighboring vertices in both directions. The process is applied in two passes: first along the rows and then along the columns. This ensures that the surface
	   better approximates the underlying mathematical curve.

		\throws assertion failure if expanded is false
	*/
	void PutOnCurve();

	/*!
		\brief Removes linear columns and rows from the patch surface by checking if points lie on a straight line and eliminating those that do.

		This function processes the vertex data of a patch surface to identify and remove columns and rows where all vertices lie approximately on a straight line. It iterates through the width and
	   height dimensions of the patch, calculating the maximum deviation of vertices from their projection onto neighboring points. If the maximum deviation is below a threshold of 0.2 units squared,
	   the corresponding column or row is removed from the patch surface. The function modifies the width and height variables accordingly and adjusts vertex positions to fill the gaps created by the
	   removal. The surface must be in an expanded state for this function to execute, as indicated by an assertion check.

		\throws assertion failure if the surface is not in expanded state
	*/
	void RemoveLinearColumnsRows();

	/*!
		\brief Resizes the expanded vertex buffer for the patch surface to the specified dimensions

		This function resizes the vertex buffer for an expanded patch surface to accommodate the new height and width dimensions. It only performs resizing if the new dimensions exceed the current
	   maximum dimensions. The function preserves existing vertex data by copying it from the old buffer to the new buffer, maintaining the spatial layout of vertices. The function assumes that the
	   surface is currently in an expanded state and will assert if this precondition is not met.

		\param height The new height dimension for the vertex buffer
		\param width The new width dimension for the vertex buffer
		\throws assertion failure if the surface is not in expanded state
	*/
	void ResizeExpanded( int height, int width );

	/*!
		\brief Expands the patch by allocating memory for a maximum width and height and copying existing vertices to their correct positions in the expanded buffer.

		This function ensures that the patch has enough space allocated for its maximum dimensions. It first checks if the patch is already expanded andFatalErrors if so. It then sets the vertex
	   buffer size to the maximum width and height. If the current width is less than the maximum width, it copies vertices from the original buffer to the expanded buffer, ensuring proper positioning
	   in the larger buffer. The function is designed to prepare the patch for further modifications or rendering operations that might require the full allocated space.

		\throws FatalError if the patch is already expanded
	*/
	void Expand();

	/*!
		\brief Collapses the patch by moving all points to the start of the verts buffer.

		This function collapses the patch by moving all points to the start of the verts buffer. It first checks if the patch is already expanded, and if not, it will crash with a fatal error. After
	   that, it checks if the width is not equal to the maximum width and if so, it copies the vertices from the expanded buffer to the collapsed buffer. Finally, it sets the number of vertices to the
	   product of width and height.

		\throws FatalError if the patch is not expanded
	*/
	void Collapse();

	/*!
		\brief Projects a point onto a vector to calculate maximum curve error

		This function calculates the projection of a given point onto a vector defined by two endpoints. It performs vector arithmetic to determine the closest point on the vector to the input point.
	   The calculation involves subtracting the start point from the input point, normalizing the vector between start and end points, and then using dot product projection to find the projected
	   point.

		\param point The point to be projected onto the vector
		\param vStart The starting point of the vector
		\param vEnd The ending point of the vector
		\param vProj The output projected point on the vector
	*/
	void ProjectPointOntoVector( const idVec3& point, const idVec3& vStart, const idVec3& vEnd, idVec3& vProj );

	/*!
		\brief Generates surface normals for all vertices in the patch by analyzing neighboring points and computing face normals

		This function calculates normals for each vertex in a patch surface by examining the surrounding points and computing the average normal from the faces formed with neighboring vertices. It
	   first checks if all vertices are coplanar, and if so, sets all normals to the plane's normal. For non-coplanar patches, it handles wrap-around edges by checking if the patch wraps horizontally
	   or vertically and computes normals by averaging the cross products of adjacent edges. The function uses a neighbor lookup table to sample points in eight directions around each vertex for
	   normal calculation.

		\throws assertion failure if the patch has already been expanded
	*/
	void GenerateNormals();

	/*!
		\brief Generates triangle indexes for a patch surface.

		This function creates the triangle index data for a patch surface by iterating over the width and height dimensions of the patch. For each cell in the grid defined by the width and height, it
	   calculates four vertex indices and generates two triangles to represent that cell. The function first sets the number of indexes needed based on the dimensions, then populates the index array
	   with the appropriate triangle definitions. After generating the main triangle indexes, it calls GenerateEdgeIndexes to handle edge cases.

	*/
	void GenerateIndexes();

	/*!
		\brief Interpolates between two draw vertices to produce a new vertex at the midpoint.

		This function performs linear interpolation between two input draw vertices and stores the result in an output vertex. It calculates the midpoint position by averaging the XYZ coordinates of
	   the two input vertices. The normal and texture coordinate are also interpolated by averaging their respective values from the input vertices. The function is designed to work with patch surface
	   data.

		\param a First input draw vertex
		\param b Second input draw vertex
		\param out Output draw vertex that will contain the interpolated result
	*/
	void LerpVert( const idDrawVert& a, const idDrawVert& b, idDrawVert& out ) const;

	/*!
		\brief Samples a single point on a 3x3 patch surface using bilinear interpolation

		This function evaluates a point on a cubic Bézier patch defined by a 3x3 grid of control vertices. The function takes UV coordinates and computes the corresponding position, normal, and
	   texture coordinates for the sampled point. The implementation uses quadratic Bernstein polynomials to perform interpolation along the u direction first, then along the v direction. The control
	   points can contain position, normal, and texture coordinate data that are interpolated separately.

		\param ctrl Control points defining the 3x3 patch surface
		\param u U parameter for interpolation along the first axis
		\param v V parameter for interpolation along the second axis
		\param out Output vertex with interpolated position, normal, and texture coordinates
	*/
	void SampleSinglePatchPoint( const idDrawVert ctrl[3][3], float u, float v, idDrawVert* out ) const;

	/*!
		\brief Samples a single patch by generating vertices based on control points and subdivision parameters.

		This function takes a 3x3 grid of control points and subdivides the patch into a grid of vertices based on the specified subdivision parameters. It calculates the u and v parameters for each
	   subdivision point and calls SampleSinglePatchPoint to generate the actual vertex data. The resulting vertices are stored in the output array at the correct positions determined by baseCol,
	   baseRow, and width.

		\param ctrl The 3x3 control points defining the patch surface
		\param baseCol The base column index in the output array where vertices will be written
		\param baseRow The base row index in the output array where vertices will be written
		\param width The width of the output vertex grid
		\param horzSub The number of horizontal subdivisions for the patch
		\param vertSub The number of vertical subdivisions for the patch
		\param outVerts Pointer to the output array where sampled vertices will be stored
	*/
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

ID_INLINE idSurface_Patch::idSurface_Patch( const idSurface_Patch& patch )
{
	( *this ) = patch;
}

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
