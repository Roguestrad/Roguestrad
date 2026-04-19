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

#ifndef __SURFACE_H__
#define __SURFACE_H__

/*
===============================================================================

	Surface base class.

	A surface is tesselated to a triangle mesh with each edge shared by
	at most two triangles.

===============================================================================
*/

typedef struct surfaceEdge_s {
	int verts[2]; // edge vertices always with ( verts[0] < verts[1] )
	int tris[2];  // edge triangles
} surfaceEdge_t;

class idSurface
{
public:
	/*!
		\brief Constructs an empty surface.

		This is the default constructor for the idSurface class. It initializes an empty surface with no vertices, no triangles, and no associated data. The surface is left in a valid but empty state,
	   ready to be populated with geometry data.

	*/
	idSurface();

	/*!
		\brief Constructs a new idSurface object as a copy of an existing idSurface object.

		This constructor performs a deep copy of the provided idSurface object, initializing the new object with the same vertex data, index data, edge data, and edge index data.

		\param surf The idSurface object to be copied.
	*/
	explicit idSurface( const idSurface& surf );
	explicit idSurface( const idDrawVert* verts, const int numVerts, const int* indexes, const int numIndexes );
	~idSurface();

	const idDrawVert&	 operator[]( const int index ) const;
	idDrawVert&			 operator[]( const int index );
	idSurface&			 operator+=( const idSurface& surf );

	//! Returns the number of indexes in the surface.
	int					 GetNumIndexes() const { return indexes.Num(); }

	//! Returns a pointer to the index array used by the surface.
	const int*			 GetIndexes() const { return indexes.Ptr(); }

	//! Returns the number of vertices in the surface.
	int					 GetNumVertices() const { return verts.Num(); }

	//! Returns a pointer to the array of vertices for this surface.
	const idDrawVert*	 GetVertices() const { return verts.Ptr(); }

	//! Returns a pointer to the array of edge indexes for this surface.
	const int*			 GetEdgeIndexes() const { return edgeIndexes.Ptr(); }

	//! Returns a pointer to the array of edges that define the surface.
	const surfaceEdge_t* GetEdges() const { return edges.Ptr(); }

	//! Clears all vertex, index, and edge data from the surface.
	void				 Clear();

	/*!
		\brief Translates all vertices of the surface by the specified translation vector.

		This function modifies the positions of all vertices in the surface by adding the given translation vector to each vertex. It iterates through all vertices in the verts array and updates their
	   XYZ coordinates accordingly. The translation is applied in-place, meaning the original vertex data is modified directly.

		\param translation The translation vector to be added to each vertex's position
	*/
	void				 TranslateSelf( const idVec3& translation );

	/*!
		\brief Rotates the vertices of the surface by the provided rotation matrix.

		This function applies the given rotation matrix to all vertices in the surface. For each vertex, it transforms the position, normal, and tangent vectors using the rotation matrix. The
	   transformation modifies the vertex data in place.

		\param rotation The rotation matrix to apply to the vertices
	*/
	void				 RotateSelf( const idMat3& rotation );

	/*!
		\brief Splits the surface into front and back parts based on a clipping plane and returns the side occupied by the surface.

		This function splits a surface into two parts, front and back, using a clipping plane. It determines which side of the plane each vertex lies on and handles cases where vertices are on the
	   plane itself. The function allocates new surfaces for the front and back parts, copying relevant vertex and index data. If the surface is coplanar with the plane, it assigns the surface to
	   either front or back based on the orientation of the surface normal relative to the plane normal. The function also optionally records the indexes of edges that lie exactly on the splitting
	   plane for both front and back surfaces.

		\param plane The clipping plane used to split the surface
		\param epsilon Tolerance for determining if a vertex is on the plane
		\param front Output parameter that receives a pointer to the front surface
		\param back Output parameter that receives a pointer to the back surface
		\param frontOnPlaneEdges Optional output parameter to store indexes of edges on the plane for the front surface
		\param backOnPlaneEdges Optional output parameter to store indexes of edges on the plane for the back surface
		\return The side of the plane that the surface occupies, which can be SIDE_FRONT, SIDE_BACK, or SIDE_ON.
	*/
	int					 Split( const idPlane& plane, const float epsilon, idSurface** front, idSurface** back, int* frontOnPlaneEdges = NULL, int* backOnPlaneEdges = NULL ) const;

	/*!
		\brief Clips the surface by the given plane and returns true if any part of the surface remains on the front side of the plane.

		This function modifies the surface in place by clipping it against the provided plane. It determines which vertices are on which side of the plane and splits edges that cross the plane
	   boundary. The function handles various cases including when vertices are on the plane, when edges are split, and when triangles need to be reconstructed after clipping. If the entire surface is
	   on the back side of the plane, the surface is cleared and the function returns false. If the surface is entirely on the front side, the function returns true without modification. The epsilon
	   parameter controls the tolerance for determining if a vertex is on the plane, and the keepOn parameter controls whether triangles that are coplanar with the plane should be kept.

		\param plane The clipping plane to use for cutting the surface
		\param epsilon Tolerance for determining if a vertex is on the plane
		\param keepOn Whether to keep triangles that are coplanar with the plane
		\return True if some part of the surface remains on the front side of the plane, false if the entire surface is on the back side or if the surface is cleared due to being completely clipped
	*/
	bool				 ClipInPlace( const idPlane& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	//! Returns true if all triangles in the surface can be reached from any other triangle by traversal
	bool				 IsConnected() const;

	//! Returns true if the surface is closed.
	bool				 IsClosed() const;

	/*!
		\brief Checks if the surface forms a convex hull by verifying that all vertices lie on or behind every face plane.

		This function determines whether the surface represents a convex polytope by testing if all vertices are on the back side or on the plane of each triangular face. It first checks if the
	   surface is closed, and if not, it returns false immediately. For each face defined by three consecutive indices in the index array, it calculates the plane equation and then verifies that no
	   vertex lies in front of that plane within the specified epsilon tolerance. If any vertex is found in front of a face plane, the function returns false, indicating the surface is not a convex
	   hull. If all vertices pass the test for all faces, it returns true.

		\param epsilon tolerance value for plane side calculation
		\return true if the surface is a convex hull, false otherwise
	*/
	bool				 IsPolytope( const float epsilon = 0.1f ) const;

	/*!
		\brief Calculates the minimum distance from the surface to a plane, returning zero if the surface intersects the plane

		This function determines the distance from a surface to a given plane by examining the distances from all vertices of the surface to the plane. It returns the minimum distance if all vertices
	   are on the same side of the plane, or zero if the surface intersects the plane. The function handles edge cases where vertices are on opposite sides of the plane by returning zero immediately
	   when such a condition is detected. The result indicates whether the surface is fully in front of, fully behind, or intersecting the plane

		\param plane the plane to calculate the distance to
		\return the minimum distance from the surface to the plane, or zero if the surface intersects the plane
	*/
	float				 PlaneDistance( const idPlane& plane ) const;

	/*!
		\brief Determines on which side of a plane the surface is located, considering an epsilon tolerance for floating-point comparisons.

		This function evaluates the position of a surface relative to a given plane by checking the distance of each vertex from the plane. It returns SIDE_FRONT if all vertices are in front of the
	   plane, SIDE_BACK if all vertices are behind the plane, SIDE_ON if all vertices lie on the plane, and SIDE_CROSS if vertices are on both sides of the plane. The epsilon parameter controls the
	   tolerance for determining if a vertex lies on the plane.

		\param plane The plane to test the surface against
		\param epsilon Tolerance for determining if a vertex is on the plane
		\return An integer value indicating the side of the plane the surface is on: SIDE_FRONT, SIDE_BACK, SIDE_ON, or SIDE_CROSS
	*/
	int					 PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	/*!
		\brief Checks if a line intersects any triangle in the surface

		This function determines whether a line segment defined by two points intersects with any of the triangles that make up the surface. It uses the ray intersection test with a parametric scale
	   value to determine if the intersection point lies within the line segment. The back face culling parameter can be used to ignore intersections with triangles facing away from the line segment.

		\param start The starting point of the line segment
		\param end The ending point of the line segment
		\param backFaceCull Whether to cull back facing triangles
		\return True if the line segment intersects any triangle in the surface, false otherwise
	*/
	bool				 LineIntersection( const idVec3& start, const idVec3& end, bool backFaceCull = false ) const;

	/*!
		\brief Checks if a ray intersects with the surface and returns the scale factor for the intersection point

		This function determines whether a ray defined by a starting point and direction intersects with the surface. It calculates the intersection point and returns the scale factor that represents
	   the distance along the ray to the intersection. The function supports backface culling to optionally ignore intersections from the back side of triangles. The ray intersection test uses
	   pluecker coordinates for efficient computation. The scale parameter is updated with the distance to the closest intersection point found.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale Reference to store the scale factor of the intersection point
		\param backFaceCull If true, backfaces are not considered for intersection
		\return True if an intersection was found, false otherwise
	*/
	bool				 RayIntersection( const idVec3& start, const idVec3& dir, float& scale, bool backFaceCull = false ) const;

protected:
	idList<idDrawVert, TAG_IDLIB_LIST_SURFACE>	  verts;	   // vertices
	idList<int, TAG_IDLIB_LIST_SURFACE>			  indexes;	   // 3 references to vertices for each triangle
	idList<surfaceEdge_t, TAG_IDLIB_LIST_SURFACE> edges;	   // edges
	idList<int, TAG_IDLIB_LIST_SURFACE>			  edgeIndexes; // 3 references to edges for each triangle, may be negative for reversed edge

protected:
	/*!
		\brief Generates edge indexes for the surface by processing triangle indices and building edge connectivity information.

		This function processes the triangle indexes of the surface to construct a list of unique edges and their connectivity. For each triangle, it creates three edges and ensures that each edge is
	   only added once to the edge list. The function tracks which triangles share edges and maintains a mapping from triangle indices to edge indexes. Edge indexes are stored in the edgeIndexes
	   array, where positive values indicate the edge index and negative values indicate the edge index with inverted orientation.

		\throws assertion failure if an edge is shared by more than two triangles
	*/
	void GenerateEdgeIndexes();

	/*!
		\brief Finds an edge in the surface defined by two vertices and returns its index with sign indicating vertex order.

		The function searches for an edge in the surface that connects the two specified vertices. It ensures the vertices are ordered consistently by comparing their indices, then iterates through
	   the edges to find a match. If found, it returns the edge index with a positive sign if v1 is less than v2, or negative if v2 is less than v1. If no matching edge is found, it returns zero.

		\param v1 First vertex index
		\param v2 Second vertex index
		\return Index of the edge if found, with positive or negative sign to indicate vertex order, or zero if not found.
	*/
	int	 FindEdge( int v1, int v2 ) const;
};

ID_INLINE idSurface::idSurface()
{
}

ID_INLINE idSurface::idSurface( const idDrawVert* verts, const int numVerts, const int* indexes, const int numIndexes )
{
	assert( verts != NULL && indexes != NULL && numVerts > 0 && numIndexes > 0 );
	this->verts.SetNum( numVerts );
	memcpy( this->verts.Ptr(), verts, numVerts * sizeof( verts[0] ) );
	this->indexes.SetNum( numIndexes );
	memcpy( this->indexes.Ptr(), indexes, numIndexes * sizeof( indexes[0] ) );
	GenerateEdgeIndexes();
}

ID_INLINE idSurface::idSurface( const idSurface& surf )
{
	this->verts		  = surf.verts;
	this->indexes	  = surf.indexes;
	this->edges		  = surf.edges;
	this->edgeIndexes = surf.edgeIndexes;
}

ID_INLINE idSurface::~idSurface()
{
}

/*
=================
idSurface::operator[]
=================
*/
ID_INLINE const idDrawVert& idSurface::operator[]( const int index ) const
{
	return verts[index];
};

/*
=================
idSurface::operator[]
=================
*/
ID_INLINE idDrawVert& idSurface::operator[]( const int index )
{
	return verts[index];
};

/*
=================
idSurface::operator+=
=================
*/
ID_INLINE idSurface& idSurface::operator+=( const idSurface& surf )
{
	int i, m, n;
	n = verts.Num();
	m = indexes.Num();
	verts.Append( surf.verts ); // merge verts where possible ?
	indexes.Append( surf.indexes );
	for( i = m; i < indexes.Num(); i++ ) {
		indexes[i] += n;
	}
	GenerateEdgeIndexes();
	return *this;
}

ID_INLINE void idSurface::Clear()
{
	verts.Clear();
	indexes.Clear();
	edges.Clear();
	edgeIndexes.Clear();
}

ID_INLINE void idSurface::TranslateSelf( const idVec3& translation )
{
	for( int i = 0; i < verts.Num(); i++ ) {
		verts[i].xyz += translation;
	}
}

ID_INLINE void idSurface::RotateSelf( const idMat3& rotation )
{
	for( int i = 0; i < verts.Num(); i++ ) {
		verts[i].xyz *= rotation;
		verts[i].SetNormal( verts[i].GetNormal() * rotation );
		verts[i].SetTangent( verts[i].GetTangent() * rotation );
	}
}

#endif /* !__SURFACE_H__ */
