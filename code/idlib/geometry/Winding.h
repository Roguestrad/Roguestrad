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

#ifndef __WINDING_H__
#define __WINDING_H__

/*!
	\class idWinding
	\brief A class representing a polygon winding defined by a sequence of 3D vertices.

	The idWinding class encapsulates a polygon winding structure composed of points in 3D space, typically used for representing planar polygons in geometric computations. It supports various
   operations such as construction from different sources, manipulation of points, splitting and clipping against planes, and geometric queries like area, center, and bounds. The class provides
   methods for validation, merging, and convex hull computations, making it suitable for use in rendering, collision detection, and BSP-related algorithms. Memory management is handled internally with
   automatic reallocation and cleanup.

*/
class idWinding
{
public:
	//! Initializes a new empty winding with no allocated points.
	idWinding();

	//! Constructs an idWinding object with space allocated for the specified number of points.
	explicit idWinding( const int n );

	//! Constructs a winding from an array of vertices
	explicit idWinding( const idVec3* verts, const int n );

	//! Creates a new winding from a plane normal and distance
	explicit idWinding( const idVec3& normal, const float dist );

	//! Creates a winding from a plane
	explicit idWinding( const idPlane& plane );

	//! Constructs a new winding as a copy of the provided winding.
	explicit idWinding( const idWinding& winding );

	//! Destructor for the idWinding class that releases the allocated memory.
	virtual ~idWinding();

	//! Assigns the contents of another winding to this winding
	idWinding&	  operator=( const idWinding& winding );

	//! Returns a constant reference to the idVec5 point at the specified index in the winding.
	const idVec5& operator[]( const int index ) const;

	//! Provides access to a point in the winding by index
	idVec5&		  operator[]( const int index );

	//! Appends a point to the end of the winding point array and returns a reference to the winding.
	idWinding&	  operator+=( const idVec3& v );

	//! Appends a point to the winding and returns a reference to the winding.
	idWinding&	  operator+=( const idVec5& v );

	//! Adds a point to the winding.
	void		  AddPoint( const idVec3& v );

	//! Adds a point to the winding.
	void		  AddPoint( const idVec5& v );

	//! Returns the number of points in the winding.
	int			  GetNumPoints() const;

	//! Sets the number of points in the winding to the specified value.
	void		  SetNumPoints( int n );

	//! Clears the winding data by resetting point count and freeing memory.
	virtual void  Clear();

	//! Creates a winding for a plane using the provided normal and distance.
	void		  BaseForPlane( const idVec3& normal, const float dist );

	//! Initializes the winding based on the provided plane equation.
	void		  BaseForPlane( const idPlane& plane );

	/*!
		\brief Splits the winding into front and back portions based on the provided plane and epsilon value.

		This function divides the winding into two new windings, one for the front side and one for the back side of the given plane. The epsilon parameter controls the tolerance for determining if a
	   point is on the plane. The function returns an integer indicating the relationship between the winding and the plane: SIDE_FRONT if the winding is entirely in front of the plane, SIDE_BACK if
	   it is entirely behind, and SIDE_CROSS if it is split by the plane. If the winding is coplanar with the plane, it is assigned to either front or back based on the orientation of the plane
	   normals.

		\param plane The plane to split the winding with
		\param epsilon The tolerance for determining if a point is on the plane
		\param front Output parameter for the front portion of the split
		\param back Output parameter for the back portion of the split
		\return An integer indicating the relationship between the winding and the plane: SIDE_FRONT, SIDE_BACK, or SIDE_CROSS.
		\throws FatalError if the number of points in the result exceeds the estimated maximum.
	*/
	int			  Split( const idPlane& plane, const float epsilon, idWinding** front, idWinding** back ) const;

	//! Returns the winding fragment at the front of the clipping plane, or NULL if nothing remains at the front.
	idWinding*	  Clip( const idPlane& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	//! Clips the winding by the provided plane and returns true if any part remains on the front side
	bool		  ClipInPlace( const idPlane& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	//! Returns a copy of this winding.
	idWinding*	  Copy() const;

	//! Creates a new winding with the point order reversed from the original winding.
	idWinding*	  Reverse() const;

	//! Reverses the order of points in the winding
	void		  ReverseSelf();

	//! Removes consecutive equal points from the winding within the specified epsilon threshold.
	void		  RemoveEqualPoints( const float epsilon = ON_EPSILON );

	//! Removes colinear points from the winding that are within the specified epsilon distance.
	void		  RemoveColinearPoints( const idVec3& normal, const float epsilon = ON_EPSILON );

	//! Removes a point from the winding at the specified index.
	void		  RemovePoint( int point );

	//! Inserts a point into the winding at the specified position.
	void		  InsertPoint( const idVec5& point, int spot );

	//! Inserts a point into the winding if it lies on an edge within the specified epsilon tolerance.
	bool		  InsertPointIfOnEdge( const idVec5& point, const idPlane& plane, const float epsilon = ON_EPSILON );

	//! Inserts a point into the winding if it lies on an edge within the specified epsilon tolerance
	bool		  InsertPointIfOnEdge( const idVec3& point, const idPlane& plane, const float epsilon = ON_EPSILON );

	//! Adds a winding to the convex hull
	void		  AddToConvexHull( const idWinding* winding, const idVec3& normal, const float epsilon = ON_EPSILON );

	//! Adds a point to the convex hull while maintaining the hull's integrity
	void		  AddToConvexHull( const idVec3& point, const idVec3& normal, const float epsilon = ON_EPSILON );

	//! Attempts to merge this winding with another winding along a shared edge, returning a new winding if successful.
	idWinding*	  TryMerge( const idWinding& w, const idVec3& normal, int keep = false ) const;

	//! Checks if the winding is valid and returns true if it passes all validation checks.
	bool		  Check( bool print = true ) const;

	//! Calculates and returns the area of the winding by summing the cross products of triangular segments.
	float		  GetArea() const;

	//! Returns the center point of the winding by averaging all its vertices.
	idVec3		  GetCenter() const;

	//! Calculates and returns the radius of the winding from a given center point.
	float		  GetRadius( const idVec3& center ) const;

	//! Computes and returns the plane normal and distance for the winding.
	void		  GetPlane( idVec3& normal, float& dist ) const;

	//! Calculates and sets the plane equation for the winding using its vertex points.
	void		  GetPlane( idPlane& plane ) const;

	//! Calculates and returns the bounding box of the winding in the provided bounds parameter
	void		  GetBounds( idBounds& bounds ) const;

	//! Returns true if the winding is considered tiny based on edge lengths.
	bool		  IsTiny() const;

	//! Returns true if any vertex coordinate of the winding is outside the valid world coordinate range.
	bool		  IsHuge() const;

	//! Prints the coordinates of each point in the winding to the console.
	void		  Print() const;

	//! Returns the minimum distance from the winding to the given plane.
	float		  PlaneDistance( const idPlane& plane ) const;

	//! Determines which side of a plane the winding resides on, using the specified epsilon for floating-point comparisons.
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	/*!
		\brief Determines whether the planes of two windings are concave relative to each other

		This function checks if the planes defined by two windings are concave with respect to each other. It examines each point of the first winding to see if any point lies at the back of the
	   second winding's plane, and vice versa. If either condition is met, the planes are considered concave. The function uses a small epsilon value WCONVEX_EPSILON to account for floating-point
	   precision issues when comparing distances to planes

		\param w2 The second winding to compare against
		\param normal1 Normal vector of the plane for the first winding
		\param normal2 Normal vector of the plane for the second winding
		\param dist1 Distance from origin to the plane of the first winding
		\param dist2 Distance from origin to the plane of the second winding
		\return true if the planes of the two windings are concave relative to each other, false otherwise
	*/
	bool		  PlanesConcave( const idWinding& w2, const idVec3& normal1, const idVec3& normal2, float dist1, float dist2 ) const;

	//! Checks if a point is inside the winding, using the specified normal and epsilon tolerance.
	bool		  PointInside( const idVec3& normal, const idVec3& point, const float epsilon ) const;

	/*!
		\brief Checks if a line intersects with the winding, optionally culling back faces

		This function determines whether a line segment defined by start and end points intersects with the winding plane. It first calculates the distance of both endpoints from the winding plane. If
	   both points are on the same side of the plane, no intersection occurs. If back face culling is enabled and the front point is on the back side of the plane, the function returns false.
	   Otherwise, it calculates the intersection point of the line with the plane and checks if this point lies within the winding using a point-in-winding test

		\param windingPlane The plane of the winding to test intersection against
		\param start The starting point of the line segment
		\param end The ending point of the line segment
		\param backFaceCull If true, back faces of the winding are culled from intersection tests
		\return True if the line intersects the winding, false otherwise
	*/
	bool		  LineIntersection( const idPlane& windingPlane, const idVec3& start, const idVec3& end, bool backFaceCull = false ) const;

	/*!
		\brief Checks if a ray intersects with the winding and calculates the intersection scale.

		This function determines whether a ray defined by a start point and direction intersects with the winding. It uses Pluecker coordinates to test for intersection with the edges of the winding.
	   If the ray intersects, the scale factor is calculated and stored in the scale parameter. The backFaceCull parameter determines whether back-facing intersections should be ignored.

		\param windingPlane The plane of the winding used for final intersection calculation
		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale Output parameter that receives the scale factor of the intersection
		\param backFaceCull If true, back-facing intersections are ignored
		\return True if the ray intersects the winding, false otherwise
	*/
	bool		  RayIntersection( const idPlane& windingPlane, const idVec3& start, const idVec3& dir, float& scale, bool backFaceCull = false ) const;

	//! Computes the area of a triangle defined by three 3D vertices.
	static float  TriangleArea( const idVec3& a, const idVec3& b, const idVec3& c );

protected:
	int			 numPoints; // number of points
	idVec5*		 p;			// pointer to point data
	int			 allocedSize;

	//! Ensures the winding has allocated space for at least n points, resizing if necessary.
	bool		 EnsureAlloced( int n, bool keep = false );

	//! Reallocates the winding point array to accommodate a specified number of points.
	virtual bool ReAllocate( int n, bool keep = false );
};

ID_INLINE idWinding::idWinding()
{
	numPoints = allocedSize = 0;
	p						= NULL;
}

ID_INLINE idWinding::idWinding( int n )
{
	numPoints = allocedSize = 0;
	p						= NULL;
	EnsureAlloced( n );
}

ID_INLINE idWinding::idWinding( const idVec3* verts, const int n )
{
	int i;

	numPoints = allocedSize = 0;
	p						= NULL;
	if( !EnsureAlloced( n ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < n; i++ ) {
		p[i].ToVec3() = verts[i];
		p[i].s = p[i].t = 0.0f;
	}
	numPoints = n;
}

ID_INLINE idWinding::idWinding( const idVec3& normal, const float dist )
{
	numPoints = allocedSize = 0;
	p						= NULL;
	BaseForPlane( normal, dist );
}

ID_INLINE idWinding::idWinding( const idPlane& plane )
{
	numPoints = allocedSize = 0;
	p						= NULL;
	BaseForPlane( plane );
}

ID_INLINE idWinding::idWinding( const idWinding& winding )
{
	int i;
	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
}

ID_INLINE idWinding::~idWinding()
{
	delete[] p;
	p = NULL;
}

ID_INLINE idWinding& idWinding::operator=( const idWinding& winding )
{
	int i;

	if( !EnsureAlloced( winding.numPoints ) ) {
		numPoints = 0;
		return *this;
	}
	for( i = 0; i < winding.numPoints; i++ ) {
		p[i] = winding.p[i];
	}
	numPoints = winding.numPoints;
	return *this;
}

ID_INLINE const idVec5& idWinding::operator[]( const int index ) const
{
	// assert( index >= 0 && index < numPoints );
	return p[index];
}

ID_INLINE idVec5& idWinding::operator[]( const int index )
{
	// assert( index >= 0 && index < numPoints );
	return p[index];
}

ID_INLINE idWinding& idWinding::operator+=( const idVec3& v )
{
	AddPoint( v );
	return *this;
}

ID_INLINE idWinding& idWinding::operator+=( const idVec5& v )
{
	AddPoint( v );
	return *this;
}

ID_INLINE void idWinding::AddPoint( const idVec3& v )
{
	if( !EnsureAlloced( numPoints + 1, true ) ) { return; }
	p[numPoints] = v;
	numPoints++;
}

ID_INLINE void idWinding::AddPoint( const idVec5& v )
{
	if( !EnsureAlloced( numPoints + 1, true ) ) { return; }
	p[numPoints] = v;
	numPoints++;
}

ID_INLINE int idWinding::GetNumPoints() const
{
	return numPoints;
}

ID_INLINE void idWinding::SetNumPoints( int n )
{
	if( !EnsureAlloced( n, true ) ) { return; }
	numPoints = n;
}

ID_INLINE void idWinding::Clear()
{
	numPoints = 0;
	delete[] p;
	p = NULL;
}

ID_INLINE void idWinding::BaseForPlane( const idPlane& plane )
{
	BaseForPlane( plane.Normal(), plane.Dist() );
}

ID_INLINE bool idWinding::EnsureAlloced( int n, bool keep )
{
	if( n > allocedSize ) { return ReAllocate( n, keep ); }
	return true;
}

/*
===============================================================================

	idFixedWinding is a fixed buffer size winding not using
	memory allocations.

	When an operation would overflow the fixed buffer a warning
	is printed and the operation is safely cancelled.

===============================================================================
*/

#define MAX_POINTS_ON_WINDING 64

/*!
	\class idFixedWinding
	\brief A fixed-size winding implementation that maintains pre-allocated storage for polygonal vertices.

	This class provides a specialized winding implementation that uses pre-allocated memory to store polygon vertices. It inherits from idWinding and is designed to avoid dynamic memory reallocations
   by maintaining a fixed buffer size. The class supports construction from various sources including arrays of vertices, plane definitions, and existing windings. The fixed buffer ensures consistent
   memory usage patterns which can be beneficial for performance-critical operations. Methods are provided for copying, clearing, and splitting windings, with the split operation allowing for
   plane-based partitioning of the winding geometry. The destructor is designed to prevent freeing of the fixed buffer, ensuring that memory management remains consistent throughout the object's
   lifetime.

*/
class idFixedWinding : public idWinding
{
public:
	//! Constructs an empty fixed winding with pre-allocated storage.
	idFixedWinding();

	//! Initializes a fixed winding with a specified maximum number of points.
	explicit idFixedWinding( const int n );

	//! Constructs a fixed winding from an array of vertices.
	explicit idFixedWinding( const idVec3* verts, const int n );

	//! Constructs a fixed winding from a plane normal and distance.
	explicit idFixedWinding( const idVec3& normal, const float dist );

	//! Constructs a fixed winding from a plane.
	explicit idFixedWinding( const idPlane& plane );

	//! Constructs a fixed winding from a regular winding by copying its points.
	explicit idFixedWinding( const idWinding& winding );

	//! Creates a new fixed winding as a copy of an existing winding.
	explicit idFixedWinding( const idFixedWinding& winding );

	//! Destructor for the idFixedWinding class that prevents freeing of the fixed buffer.
	virtual ~idFixedWinding();

	//! Assigns the contents of another winding to this winding
	idFixedWinding& operator=( const idWinding& winding );

	//! Clears all points from the fixed winding object.
	virtual void	Clear();

	//! Splits the winding by a plane into front and back parts, returning which side the winding falls on.
	int				Split( idFixedWinding* back, const idPlane& plane, const float epsilon = ON_EPSILON );

protected:
	idVec5		 data[MAX_POINTS_ON_WINDING]; // point data

	//! Resizes the winding to accommodate the specified number of points.
	virtual bool ReAllocate( int n, bool keep = false );
};

ID_INLINE idFixedWinding::idFixedWinding()
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
}

ID_INLINE idFixedWinding::idFixedWinding( int n )
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
}

ID_INLINE idFixedWinding::idFixedWinding( const idVec3* verts, const int n )
{
	int i;

	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	if( !EnsureAlloced( n ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < n; i++ ) {
		p[i].ToVec3() = verts[i];
		p[i].s = p[i].t = 0;
	}
	numPoints = n;
}

ID_INLINE idFixedWinding::idFixedWinding( const idVec3& normal, const float dist )
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	BaseForPlane( normal, dist );
}

ID_INLINE idFixedWinding::idFixedWinding( const idPlane& plane )
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	BaseForPlane( plane );
}

ID_INLINE idFixedWinding::idFixedWinding( const idWinding& winding )
{
	int i;

	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
}

ID_INLINE idFixedWinding::idFixedWinding( const idFixedWinding& winding )
{
	int i;

	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
}

ID_INLINE idFixedWinding::~idFixedWinding()
{
	p = NULL; // otherwise it tries to free the fixed buffer
}

ID_INLINE idFixedWinding& idFixedWinding::operator=( const idWinding& winding )
{
	int i;

	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return *this;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
	return *this;
}

ID_INLINE void idFixedWinding::Clear()
{
	numPoints = 0;
}
#endif /* !__WINDING_H__ */
