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

/*!
	\class idSurface
	\brief A surface class that manages vertices, indexes, and geometric operations for polygonal meshes.

	The idSurface class represents a polygonal mesh with vertices and indexes that can be manipulated through various geometric operations. It supports construction from vertex and index data,
   copying, and modification operations such as translation and rotation. The class provides methods for splitting, clipping, and intersection testing with planes and rays. It also includes
   functionality for generating edge information and determining surface topology properties such as connectivity, closure, and convexity. The surface can be cleared and modified in place, and offers
   access to its underlying vertex and index arrays for direct manipulation or rendering purposes.

*/
class idSurface
{
public:
	//! Constructs an empty idSurface object.
	idSurface();

	//! Constructs a new surface as a copy of an existing surface.
	explicit idSurface( const idSurface& surf );

	//! Initializes a surface with the specified vertices and indexes.
	explicit idSurface( const idDrawVert* verts, const int numVerts, const int* indexes, const int numIndexes );
	~idSurface();

	const idDrawVert&	 operator[]( const int index ) const;
	idDrawVert&			 operator[]( const int index );

	//! Appends the vertices and indexes of another surface to this surface
	idSurface&			 operator+=( const idSurface& surf );

	//! Returns the number of indexes in the surface.
	int					 GetNumIndexes() const { return indexes.Num(); }

	//! Returns a pointer to the array of vertex indexes used by the surface.
	const int*			 GetIndexes() const { return indexes.Ptr(); }

	//! Returns the number of vertices in the surface.
	int					 GetNumVertices() const { return verts.Num(); }

	//! Returns a pointer to the array of vertices for this surface.
	const idDrawVert*	 GetVertices() const { return verts.Ptr(); }

	//! Returns a pointer to the array of edge indexes for this surface.
	const int*			 GetEdgeIndexes() const { return edgeIndexes.Ptr(); }

	//! Returns a pointer to the array of surface edges.
	const surfaceEdge_t* GetEdges() const { return edges.Ptr(); }

	//! Clears all vertex, index, and edge data from the surface.
	void				 Clear();

	//! Moves all vertices of the surface by the specified translation vector.
	void				 TranslateSelf( const idVec3& translation );

	//! Rotates the vertices of the surface by the given rotation matrix.
	void				 RotateSelf( const idMat3& rotation );

	//! Splits the surface into front and back parts based on a clipping plane.
	int					 Split( const idPlane& plane, const float epsilon, idSurface** front, idSurface** back, int* frontOnPlaneEdges = NULL, int* backOnPlaneEdges = NULL ) const;

	//! Clips the surface by the given plane and returns true if any part remains on the front side
	bool				 ClipInPlace( const idPlane& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	//! Returns true if all triangles in the surface can be reached from any other triangle by traversal.
	bool				 IsConnected() const;

	//! Returns true if the surface is closed.
	bool				 IsClosed() const;

	//! Returns true if the surface is a convex hull.
	bool				 IsPolytope( const float epsilon = 0.1f ) const;

	//! Computes the distance from the surface to a plane, considering all vertices.
	float				 PlaneDistance( const idPlane& plane ) const;

	//! Determines which side of a plane the surface is on, considering an epsilon tolerance for floating-point comparisons.
	int					 PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	//! Returns true if a line intersects any triangle in the surface
	bool				 LineIntersection( const idVec3& start, const idVec3& end, bool backFaceCull = false ) const;

	//! Determines if a ray intersects with the surface and calculates the intersection scale factor.
	bool				 RayIntersection( const idVec3& start, const idVec3& dir, float& scale, bool backFaceCull = false ) const;

protected:
	idList<idDrawVert, TAG_IDLIB_LIST_SURFACE>	  verts;	   // vertices
	idList<int, TAG_IDLIB_LIST_SURFACE>			  indexes;	   // 3 references to vertices for each triangle
	idList<surfaceEdge_t, TAG_IDLIB_LIST_SURFACE> edges;	   // edges
	idList<int, TAG_IDLIB_LIST_SURFACE>			  edgeIndexes; // 3 references to edges for each triangle, may be negative for reversed edge

protected:
	//! Generates edge indexes for the surface triangles
	void GenerateEdgeIndexes();

	//! Finds and returns the index of an edge defined by two vertices in the surface.
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

ID_INLINE const idDrawVert& idSurface::operator[]( const int index ) const
{
	return verts[index];
};

ID_INLINE idDrawVert& idSurface::operator[]( const int index )
{
	return verts[index];
};

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
