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

#ifndef __WINDING2D_H__
#define __WINDING2D_H__

/*
===============================================================================

	A 2D winding is an arbitrary convex 2D polygon defined by an array of points.

===============================================================================
*/

#define MAX_POINTS_ON_WINDING_2D 16

class idWinding2D
{
public:
	/*!
		\brief Constructs an empty idWinding2D object with no points.

		This inline constructor initializes a new idWinding2D object and sets the number of points to zero, indicating that the winding is empty and ready for use. The winding can then be populated
	   with points using other member functions.

	*/
	idWinding2D();

	idWinding2D&  operator=( const idWinding2D& winding );
	const idVec2& operator[]( const int index ) const;
	idVec2&		  operator[]( const int index );

	//! Clears all points from the winding.
	void		  Clear();

	/*!
		\brief Adds a point to the winding and increments the point counter.
		\param point The 2D point to add to the winding
	*/
	void		  AddPoint( const idVec2& point );

	//! Returns the number of points in the winding.
	int			  GetNumPoints() const;

	/*!
		\brief Expands the winding by the given distance in all directions

		This function expands a 2D winding by moving each vertex outward along the normalized edge normals. For each edge of the winding, the normal is calculated and then scaled by the input
	   distance. The vertices are then adjusted by the sum of the normalized edge normals from the current and previous edges. This creates a uniformly expanded winding shape that maintains the
	   overall orientation and proportions of the original winding.

		\param d The distance to expand the winding by in all directions
	*/
	void		  Expand( const float d );

	/*!
		\brief Expands the winding to encompass an axial bounding box defined by the given bounds.

		This function modifies the current winding by expanding it to contain the specified axial bounding box. It calculates the planes of the winding edges and adds bevels to ensure proper
	   expansion. The function processes each edge of the winding, computes the corresponding plane, and applies the expansion based on the provided bounds. The expansion is performed by adjusting the
	   plane distances according to the specified bounds and then recalculating the intersection points to form the new winding.

		\param bounds The bounding box coordinates that define the expansion limits for the winding
		\throws assertion failure if the number of planes exceeds the maximum allowed or if the number of planes is zero
	*/
	void		  ExpandForAxialBox( const idVec2 bounds[2] );

	/*!
		\brief Splits the winding into front and back portions based on a clipping plane and returns which side the winding falls on.

		This function takes a winding and splits it into two new windings, one for the front side of the clipping plane and one for the back side. The original winding remains unchanged. The function
	   returns an integer value indicating whether the winding is entirely on the front side, entirely on the back side, or crosses the plane. It uses an epsilon value to determine if a point lies on
	   the plane. The front and back windings are allocated using new and their pointers are set in the output parameters.

		\param plane Clipping plane used to split the winding
		\param epsilon Epsilon value to determine if a point lies on the plane
		\param front Output parameter that will hold the pointer to the front winding
		\param back Output parameter that will hold the pointer to the back winding
		\return Integer value indicating the side of the winding relative to the clipping plane: SIDE_FRONT if the winding is entirely on the front side, SIDE_BACK if it is entirely on the back side,
	   and SIDE_CROSS if it crosses the plane.
	*/
	int			  Split( const idVec3& plane, const float epsilon, idWinding2D** front, idWinding2D** back ) const;

	/*!
		\brief Clips the winding by the given plane and modifies it in place, returning true if the resulting winding has points.

		This function performs a clipping operation on the winding using the specified plane. It modifies the winding in place by removing points that lie on the back side of the plane. The function
	   evaluates each vertex of the winding against the plane to determine which side it lies on. Vertices on the front side are kept, and vertices on the back side are discarded. When transitioning
	   from a front vertex to a back vertex (or vice versa), a new intersection point is calculated and added to the winding. The function handles special cases where the winding is entirely on the
	   plane, depending on the keepOn flag. If the entire winding lies on the back side of the plane, the function returns false and sets the number of points to zero. If the entire winding lies on
	   the front side, it returns true without modification.

		\param plane The plane used to clip the winding
		\param epsilon A small epsilon value used for floating-point comparisons
		\param keepOn If true and the winding is entirely on the plane, the function returns true; otherwise, it returns false
		\return True if the resulting winding has points, false if the winding is completely clipped away.
	*/
	bool		  ClipInPlace( const idVec3& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	/*!
		\brief Creates and returns a deep copy of the current winding object

		This function allocates a new idWinding2D object and copies all the point data from the current winding into the newly created object. The copy retains the same number of points and point
	   coordinates as the original winding. This is a standard copy operation that ensures the returned object is independent of the original. The function is typically used when a duplicate of a
	   winding is needed for operations that might modify the winding without affecting the original.

		\return A pointer to the newly created copy of the winding object
	*/
	idWinding2D*  Copy() const;

	/*!
		\brief Returns a new winding with the point order reversed from this winding

		This function creates a new idWinding2D object that contains the same points as this winding but in reverse order. The new winding is allocated using the TAG_IDLIB_WINDING memory tag. The
	   function is typically used when it's necessary to have a winding with opposite point traversal order, such as when creating portal windings for different area connections in the engine's
	   rendering system

		\return A pointer to a newly allocated idWinding2D object with the points in reverse order
	*/
	idWinding2D*  Reverse() const;

	//! Computes the area of the 2D winding by summing triangle areas.
	float		  GetArea() const;

	//! Returns the center point of the winding by averaging all its vertices.
	idVec2		  GetCenter() const;

	/*!
		\brief Calculates the radius of the winding as seen from the given center point

		This function computes the maximum distance from the specified center point to any vertex in the winding. It iterates through all vertices of the winding, calculates the squared distance to
	   the center for each vertex, and keeps track of the maximum squared distance. Finally, it returns the square root of the maximum squared distance, which represents the actual radius of the
	   winding from the given center point

		\param center The center point from which to calculate the radius
		\return The radius of the winding as measured from the given center point
	*/
	float		  GetRadius( const idVec2& center ) const;

	/*!
		\brief Calculates and returns the bounding box of the winding in 2D space.

		This function determines the minimum and maximum x and y coordinates of all points in the winding. If the winding has no points, it sets the bounds to infinity values. The result is stored in
	   the provided bounds array where bounds[0] represents the minimum coordinates and bounds[1] represents the maximum coordinates.

		\param bounds An array of two idVec2 points that will contain the minimum and maximum coordinates of the winding
	*/
	void		  GetBounds( idVec2 bounds[2] ) const;

	//! Determines whether the winding is considered tiny based on edge lengths.
	bool		  IsTiny() const;

	//! Checks if any point in the winding is outside the valid world coordinate range.
	bool		  IsHuge() const;

	/*!
		\brief Prints the coordinates of all points in the winding to the console

		This function iterates through all the points in the winding and prints their 2D coordinates in a formatted manner. Each point is displayed with five digits before the decimal point and one
	   digit after the decimal point. The output follows the format "(x, y)" with each point on a separate line.

	*/
	void		  Print() const;

	/*!
		\brief Calculates the minimum distance from the winding to a plane

		This function computes the distance from a 2D winding to a plane by checking the signed distances from each point in the winding to the plane. It returns the smallest signed distance, with
	   special handling for cases where the winding straddles the plane. The function returns zero if the winding crosses the plane, and the minimum or maximum distance otherwise. This is used in
	   collision detection to determine how far a winding is from a polygon plane.

		\param plane The plane to calculate the distance to, represented as a 3D vector with x, y, and z components
		\return The minimum signed distance from the winding to the plane, or zero if the winding straddles the plane
	*/
	float		  PlaneDistance( const idVec3& plane ) const;

	/*!
		\brief Determines which side of a plane the winding is on, considering an epsilon tolerance for floating-point comparisons.

		This function evaluates the position of the winding relative to the given plane. It returns SIDE_FRONT if all vertices are in front of the plane, SIDE_BACK if all vertices are behind the
	   plane, SIDE_CROSS if the winding crosses the plane, and SIDE_ON if all vertices lie exactly on the plane. The epsilon parameter allows for a small margin of error when determining whether a
	   point lies exactly on the plane.

		\param plane The plane to test the winding against
		\param epsilon A tolerance value for determining when a point is considered to be on the plane
		\return An integer indicating the side of the plane the winding is on, where SIDE_FRONT, SIDE_BACK, SIDE_CROSS, and SIDE_ON are the possible return values
	*/
	int			  PlaneSide( const idVec3& plane, const float epsilon = ON_EPSILON ) const;

	/*!
		\brief Checks if a point is inside the winding polygon considering an epsilon tolerance for floating-point comparisons.

		The function determines whether a given point lies within the boundaries of a 2D winding polygon. It does this by checking the point's position relative to each edge of the polygon. For each
	   edge, it calculates the plane equation and evaluates the point against this plane. If the point is on the wrong side of any edge (determined by the epsilon tolerance), the function returns
	   false. Otherwise, it returns true, indicating the point is inside the polygon.

		\param point The 2D point to test for inclusion in the winding polygon
		\param epsilon A small tolerance value used to account for floating-point precision errors during comparisons
		\return True if the point is inside the winding polygon, false otherwise
	*/
	bool		  PointInside( const idVec2& point, const float epsilon ) const;

	/*!
		\brief Checks if a line defined by start and end points intersects with the winding.

		This function determines whether a line segment defined by the start and end points intersects with the 2D winding object. It first calculates the plane equation from the line segment and then
	   classifies each vertex of the winding relative to this plane. If all vertices are on the same side of the line, there is no intersection. Otherwise, it finds two edges of the winding that cross
	   the line and performs additional checks to confirm intersection. The function returns true if the line intersects the winding, and false otherwise.

		\param start The starting point of the line segment to check for intersection
		\param end The ending point of the line segment to check for intersection
		\return True if the line segment intersects the winding, false otherwise
	*/
	bool		  LineIntersection( const idVec2& start, const idVec2& end ) const;

	/*!
		\brief Calculates the intersection of a ray with the 2D winding and returns the scale factors for the intersection points.

		This function determines where a ray starting at 'start' and extending in direction 'dir' intersects with the 2D winding. It computes two scale factors, scale1 and scale2, which represent the
	   distances along the ray to the two intersection points with the winding edges. The function returns true if the ray intersects the winding at two distinct points, and false otherwise. If the
	   edgeNums parameter is provided, it will be filled with the indices of the edges that were intersected. The function uses an epsilon value of 0.1f for determining whether a point lies on the
	   edge.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale1 Output parameter for the first intersection scale factor
		\param scale2 Output parameter for the second intersection scale factor
		\param edgeNums Optional output array to store the indices of the intersected edges
		\return True if the ray intersects the winding at two distinct points, false otherwise
	*/
	bool		  RayIntersection( const idVec2& start, const idVec2& dir, float& scale1, float& scale2, int* edgeNums = NULL ) const;

	/*!
		\brief Computes a 2D plane equation from two 2D points.

		This function calculates the coefficients of a plane equation in 2D space defined by two points. The plane equation is represented as ax + by + c = 0, where a and b are the normal vector
	   components and c is the distance from the origin. The normal vector is computed as the perpendicular to the vector formed by the two input points. If the normalize flag is true, the normal
	   vector is normalized to unit length. The z component of the returned vector represents the negative dot product of the start point with the normal vector.

		\param start The first point defining the plane
		\param end The second point defining the plane
		\param normalize Flag indicating whether to normalize the resulting normal vector
		\return A 3D vector representing the plane equation coefficients (a, b, c) where a*x + b*y + c = 0
	*/
	static idVec3 Plane2DFromPoints( const idVec2& start, const idVec2& end, const bool normalize = false );

	/*!
		\brief Computes a 2D plane equation from a starting point and direction vector

		This function calculates a plane equation in the form ax + by + c = 0 using a starting point and direction vector in 2D space. The plane normal is derived from the direction vector by rotating
	   it 90 degrees counterclockwise. If the normalize flag is true, the normal vector is normalized to unit length. The constant term c is calculated using the dot product of the starting point and
	   the normal vector, negated to satisfy the plane equation.

		\param start The starting point on the line in 2D space
		\param dir The direction vector of the line in 2D space
		\param normalize Flag indicating whether to normalize the resulting plane normal vector
		\return A 3D vector representing the plane equation coefficients (a, b, c) where ax + by + c = 0
	*/
	static idVec3 Plane2DFromVecs( const idVec2& start, const idVec2& dir, const bool normalize = false );

	/*!
		\brief Computes the intersection point of two 2D planes represented as idVec3 structures

		This function calculates the intersection point of two 2D planes defined by their normal vectors and distance from origin. The planes are represented as idVec3 where x and y components
	   represent the normal vector and z represents the distance from origin. The function uses a linear system solver based on Cramer's rule to find the intersection point. It returns false if the
	   planes are parallel or nearly parallel, indicating no unique intersection point exists.

		\param plane1 First plane represented as idVec3 with x,y normal components and z distance from origin
		\param plane2 Second plane represented as idVec3 with x,y normal components and z distance from origin
		\param point Output parameter that receives the intersection point coordinates
		\return True if the planes intersect at a unique point, false if the planes are parallel or nearly parallel
	*/
	static bool	  Plane2DIntersection( const idVec3& plane1, const idVec3& plane2, idVec2& point );

private:
	int	   numPoints;
	idVec2 p[MAX_POINTS_ON_WINDING_2D];
};

ID_INLINE idWinding2D::idWinding2D()
{
	numPoints = 0;
}

ID_INLINE idWinding2D& idWinding2D::operator=( const idWinding2D& winding )
{
	int i;

	for( i = 0; i < winding.numPoints; i++ ) {
		p[i] = winding.p[i];
	}
	numPoints = winding.numPoints;
	return *this;
}

ID_INLINE const idVec2& idWinding2D::operator[]( const int index ) const
{
	return p[index];
}

ID_INLINE idVec2& idWinding2D::operator[]( const int index )
{
	return p[index];
}

ID_INLINE void idWinding2D::Clear()
{
	numPoints = 0;
}

ID_INLINE void idWinding2D::AddPoint( const idVec2& point )
{
	p[numPoints++] = point;
}

ID_INLINE int idWinding2D::GetNumPoints() const
{
	return numPoints;
}

ID_INLINE idVec3 idWinding2D::Plane2DFromPoints( const idVec2& start, const idVec2& end, const bool normalize )
{
	idVec3 plane;
	plane.x = start.y - end.y;
	plane.y = end.x - start.x;
	if( normalize ) { plane.ToVec2().Normalize(); }
	plane.z = -( start.x * plane.x + start.y * plane.y );
	return plane;
}

ID_INLINE idVec3 idWinding2D::Plane2DFromVecs( const idVec2& start, const idVec2& dir, const bool normalize )
{
	idVec3 plane;
	plane.x = -dir.y;
	plane.y = dir.x;
	if( normalize ) { plane.ToVec2().Normalize(); }
	plane.z = -( start.x * plane.x + start.y * plane.y );
	return plane;
}

ID_INLINE bool idWinding2D::Plane2DIntersection( const idVec3& plane1, const idVec3& plane2, idVec2& point )
{
	float n00, n01, n11, det, invDet, f0, f1;

	n00 = plane1.x * plane1.x + plane1.y * plane1.y;
	n01 = plane1.x * plane2.x + plane1.y * plane2.y;
	n11 = plane2.x * plane2.x + plane2.y * plane2.y;
	det = n00 * n11 - n01 * n01;

	if( idMath::Fabs( det ) < 1e-6f ) { return false; }

	invDet	= 1.0f / det;
	f0		= ( n01 * plane2.z - n11 * plane1.z ) * invDet;
	f1		= ( n01 * plane1.z - n00 * plane2.z ) * invDet;
	point.x = f0 * plane1.x + f1 * plane2.x;
	point.y = f0 * plane1.y + f1 * plane2.y;
	return true;
}

#endif /* !__WINDING2D_H__ */
