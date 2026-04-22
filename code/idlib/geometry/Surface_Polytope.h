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

/*!
	\class idSurface_Polytope
	\brief A surface implementation representing polyhedral shapes constructed from geometric primitives.

	This class extends the base idSurface functionality to provide specialized constructors and initialization methods for various polyhedral geometries including tetrahedron, hexahedron, octahedron,
   dodecahedron, icosahedron, cylinder, and cone. It supports the creation of polytope surfaces from arrays of planes and provides splitting capabilities for spatial partitioning. The class is
   designed to represent complex geometric shapes that can be used in rendering and collision detection systems. Memory management is handled through the inheritance from idSurface, with no explicit
   ownership semantics indicated in the interface.

*/
class idSurface_Polytope : public idSurface
{
public:
	//! Constructs an instance of the idSurface_Polytope class.
	idSurface_Polytope();

	//! Constructs an idSurface_Polytope object by copying data from another idSurface object.
	explicit idSurface_Polytope( const idSurface& surface ) :
		idSurface( surface )
	{
	}

	//! Constructs a polytope surface from an array of planes.
	void FromPlanes( const idPlane* planes, const int numPlanes );

	//! Initializes the tetrahedron surface using the provided bounding box.
	void SetupTetrahedron( const idBounds& bounds );

	//! Initializes the polytope surface as a hexahedron (cuboid) defined by the given bounds.
	void SetupHexahedron( const idBounds& bounds );

	//! Initializes the polytope as an octahedron based on the provided bounding box.
	void SetupOctahedron( const idBounds& bounds );

	//! Initializes the dodecahedron surface with the specified bounds.
	void SetupDodecahedron( const idBounds& bounds );

	//! Initializes the polytope structure with an icosahedron shape based on the provided bounds.
	void SetupIcosahedron( const idBounds& bounds );

	//! Initializes the polytope surface as a cylinder with the specified bounds and number of sides.
	void SetupCylinder( const idBounds& bounds, const int numSides );

	//! Initializes the polytope cone structure with specified bounds and number of sides.
	void SetupCone( const idBounds& bounds, const int numSides );

	//! Splits the polytope surface into front and back portions based on the provided plane.
	int	 SplitPolytope( const idPlane& plane, const float epsilon, idSurface_Polytope** front, idSurface_Polytope** back ) const;

protected:
};

ID_INLINE idSurface_Polytope::idSurface_Polytope()
{
}

#endif /* !__SURFACE_POLYTOPE_H__ */
