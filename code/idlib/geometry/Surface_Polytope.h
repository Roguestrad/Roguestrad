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

#ifndef __SURFACE_POLYTOPE_H__
#define __SURFACE_POLYTOPE_H__

/*
===============================================================================

	Polytope surface.

	NOTE: vertexes are not duplicated for texture coordinates.

===============================================================================
*/

class idSurface_Polytope : public idSurface
{
public:
	//! Constructs an empty idSurface_Polytope object.
	idSurface_Polytope();

	/*!
		\brief Constructs an idSurface_Polytope object by copying data from another idSurface object.

		This constructor initializes an idSurface_Polytope instance by explicitly calling the base class idSurface constructor with the provided surface parameter. It performs a copy operation to
	   initialize the new object with the data from the input surface.

		\param surface The idSurface object to copy data from for initialization
	*/
	explicit idSurface_Polytope( const idSurface& surface ) :
		idSurface( surface )
	{
	}

	/*!
		\brief Constructs a polytope surface from an array of planes by computing their intersections and generating vertex and index data.

		This function takes an array of planes and constructs a polytope surface by computing the intersection of these planes. For each plane, it creates a winding using the plane's normal and
	   distance, then clips this winding against all other planes. Valid windings are then converted into vertices and indexed triangles. The resulting vertices and triangle indexes are stored in the
	   surface's internal data structures.

		\param planes Array of plane definitions used to construct the polytope surface
		\param numPlanes Number of planes in the planes array
	*/
	void FromPlanes( const idPlane* planes, const int numPlanes );

	/*!
		\brief Initializes the tetrahedron surface using the provided bounding box.

		This function sets up a tetrahedral surface structure based on the given bounding box. It calculates the vertices of a regular tetrahedron centered within the bounding box, with the vertices
	   positioned according to specific mathematical constants. The function also generates the triangle indexes for the tetrahedron's faces and computes the edge indexes for the surface structure.

		\param bounds The bounding box that defines the spatial extent and center of the tetrahedron
	*/
	void SetupTetrahedron( const idBounds& bounds );

	/*!
		\brief Sets up a hexahedron surface using the provided bounding box coordinates.

		This function initializes the vertex and index arrays to define a hexahedron (a six-faced polyhedron) based on the given bounding box. It calculates the center and scale of the bounding box to
	   position the eight vertices of the hexahedron. The indices are then set up to define twelve triangular faces that form the hexahedron. Finally, it calls GenerateEdgeIndexes to build the edge
	   data structure.

		\param bounds The bounding box that defines the spatial extent of the hexahedron
	*/
	void SetupHexahedron( const idBounds& bounds );

	/*!
		\brief Initializes the octahedron surface mesh using the specified bounding volume.

		Sets up the vertex positions and triangle indexes for an octahedron shape based on the provided bounding volume. The octahedron is centered at the bounding volume's center with vertices
	   extending along the x, y, and z axes according to the bounding volume's extents. The function generates 8 triangular faces that form the octahedron structure.

		\param bounds The bounding volume that defines the size and position of the octahedron
	*/
	void SetupOctahedron( const idBounds& bounds );

	/*!
		\brief Initializes the polytope as a dodecahedron using the specified bounding box.

		This function sets up the polytope geometry to represent a dodecahedron shape within the given bounds. The dodecahedron is a polyhedron with twelve flat faces, and this setup ensures the
	   polytope is configured correctly for rendering or collision detection purposes. The function does not perform any actual geometric calculations, and the implementation appears to be empty.

		\param bounds The bounding box that defines the extent of the dodecahedron
	*/
	void SetupDodecahedron( const idBounds& bounds );

	/*!
		\brief Initializes the polytope structure using the provided bounding box to define an icosahedron.
		\param bounds The bounding box that defines the spatial extent of the icosahedron
	*/
	void SetupIcosahedron( const idBounds& bounds );

	/*!
		\brief Initializes the polytope structure to represent a cylindrical shape defined by the given bounds and number of sides.

		This function sets up the internal data structures of the idSurface_Polytope object to form a cylinder. The cylinder is defined by the provided bounding volume and the number of sides used to
	   approximate the curved surface. The function does not perform any actual geometric computation in its current implementation, but serves as a placeholder for future implementation or as a setup
	   routine for subsequent operations.

		\param bounds The bounding volume that defines the cylinder's size and position
		\param numSides The number of sides to use for approximating the cylindrical surface
	*/
	void SetupCylinder( const idBounds& bounds, const int numSides );

	/*!
		\brief Initializes the polytope shape as a cone using the specified bounds and number of sides

		This function sets up the internal data structures of the polytope to represent a conical shape. The cone is defined by the given bounding box which determines its size and orientation, and
	   the number of sides specifies the resolution of the conical surface. The function does not perform any actual geometric calculations or vertex generation in the provided implementation.

		\param bounds The bounding box that defines the size and position of the cone
		\param numSides The number of sides used to approximate the curved surface of the cone
	*/
	void SetupCone( const idBounds& bounds, const int numSides );

	/*!
		\brief Splits this polytope surface into front and back portions based on a clipping plane.

		This function divides the polytope surface into two parts using the provided plane for clipping. The front and back portions are stored in the output pointers. The function returns the side of
	   the plane that the polytope resides on, or SIDE_CROSS if it is split by the plane. When the polytope is split, additional triangles are added to close off the surfaces.

		\param plane The clipping plane used to split the polytope
		\param epsilon A small value used for epsilon comparisons during the split operation
		\param front Pointer to store the front portion of the split polytope
		\param back Pointer to store the back portion of the split polytope
		\return The side of the plane the polytope resides on, or SIDE_CROSS if the polytope is split by the plane
	*/
	int	 SplitPolytope( const idPlane& plane, const float epsilon, idSurface_Polytope** front, idSurface_Polytope** back ) const;

protected:
};

ID_INLINE idSurface_Polytope::idSurface_Polytope()
{
}

#endif /* !__SURFACE_POLYTOPE_H__ */
