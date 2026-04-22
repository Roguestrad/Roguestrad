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

/*!
	\class idWinding2D
	\brief Represents a 2D winding structure for polygonal shapes with various geometric operations.

	The idWinding2D class encapsulates a polygonal winding in 2D space, providing functionality for manipulation, geometric calculations, and spatial operations. It supports basic operations like
   adding points, clearing contents, and accessing vertices by index. The class offers methods for computing geometric properties such as area, center, and bounds, as well as advanced operations like
   clipping, splitting, and intersection detection. It can expand windings, check if they are tiny or huge, and determine spatial relationships with planes and points. The winding maintains an
   internal array of vertices and provides both const and non-const access to these vertices. Memory management is handled through copy and reverse operations that create new instances, while
   assignment and direct access maintain the existing object's state. The class is designed for efficient 2D polygonal operations commonly used in geometric algorithms and spatial partitioning.

*/
class idWinding2D
{
public:
	//! Initializes an empty winding object.
	idWinding2D();

	//! Assigns the contents of another winding to this winding.
	idWinding2D&  operator=( const idWinding2D& winding );

	//! Returns a const reference to the idVec2 element at the specified index in the winding
	const idVec2& operator[]( const int index ) const;

	//! Provides indexed access to the vertices of the winding.
	idVec2&		  operator[]( const int index );

	//! Clears the winding by setting the number of points to zero.
	void		  Clear();

	//! Adds a point to the winding
	void		  AddPoint( const idVec2& point );

	//! Returns the number of points in the winding.
	int			  GetNumPoints() const;

	//! Expands the winding by the specified distance in all directions.
	void		  Expand( const float d );

	//! Expands the winding to fit within the given axial bounding box.
	void		  ExpandForAxialBox( const idVec2 bounds[2] );

	/*!
		\brief Splits the winding into front and back portions based on a clipping plane and returns which side the winding falls on.

		This function takes a winding and splits it into two portions based on a clipping plane defined by a normal vector and a distance. The function also uses an epsilon value to determine if a
	   point lies on the plane. The results are stored in the front and back winding pointers. The function returns an integer value indicating whether the winding is on the front side, back side, or
	   crosses the plane.

		\param plane Normal vector and distance defining the clipping plane
		\param epsilon Epsilon value used to determine if a point lies on the plane
		\param front Pointer to store the front portion of the winding
		\param back Pointer to store the back portion of the winding
		\return Integer value indicating the side of the winding relative to the clipping plane: SIDE_FRONT, SIDE_BACK, or SIDE_CROSS.
	*/
	int			  Split( const idVec3& plane, const float epsilon, idWinding2D** front, idWinding2D** back ) const;

	//! Clips the winding by a plane and returns true if any part remains on the front side
	bool		  ClipInPlace( const idVec3& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	//! Creates a copy of the winding object
	idWinding2D*  Copy() const;

	//! Creates and returns a new winding with the same points but in reverse order.
	idWinding2D*  Reverse() const;

	//! Returns the area of the 2D winding by calculating the signed area using the cross product method.
	float		  GetArea() const;

	//! Calculates and returns the center point of the winding by averaging all vertex coordinates.
	idVec2		  GetCenter() const;

	//! Returns the radius of the winding from the given center point
	float		  GetRadius( const idVec2& center ) const;

	//! Computes and returns the bounding box of the 2D winding in the provided array.
	void		  GetBounds( idVec2 bounds[2] ) const;

	//! Returns true if the winding has less than three edges with significant length
	bool		  IsTiny() const;

	//! Checks if any point in the winding is outside the valid world coordinate range
	bool		  IsHuge() const;

	//! Prints the 2D winding points to the console
	void		  Print() const;

	//! Returns the distance from the winding to a plane
	float		  PlaneDistance( const idVec3& plane ) const;

	//! Determines on which side of a plane the winding lies, considering an epsilon tolerance for floating-point comparisons.
	int			  PlaneSide( const idVec3& plane, const float epsilon = ON_EPSILON ) const;

	//! Determines if a given point is inside the 2D winding, considering an epsilon tolerance for floating-point comparisons.
	bool		  PointInside( const idVec2& point, const float epsilon ) const;

	//! Checks if a line segment intersects with the winding
	bool		  LineIntersection( const idVec2& start, const idVec2& end ) const;

	/*!
		\brief Checks if a ray intersects with the 2D winding and calculates intersection parameters.

		This function determines whether a ray starting at a given point and extending in a specified direction intersects with the 2D winding. It computes two scale parameters that represent the
	   intersection points along the ray. The function also optionally returns the indices of the edges that were intersected. The ray intersection is calculated using plane equations derived from the
	   winding points and the ray direction. The function returns false if the ray does not intersect the winding or if it intersects at a degenerate point.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale1 The first intersection scale parameter
		\param scale2 The second intersection scale parameter
		\param edgeNums Optional pointer to an array to store the indices of the intersected edges
		\return True if the ray intersects the winding, false otherwise
	*/
	bool		  RayIntersection( const idVec2& start, const idVec2& dir, float& scale1, float& scale2, int* edgeNums = NULL ) const;

	//! Computes a 2D plane equation from two 2D points.
	static idVec3 Plane2DFromPoints( const idVec2& start, const idVec2& end, const bool normalize = false );

	//! Returns the plane equation for a 2D line defined by a start point and direction vector.
	static idVec3 Plane2DFromVecs( const idVec2& start, const idVec2& dir, const bool normalize = false );

	//! Calculates the intersection point of two 2D planes represented by idVec3 structures.
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
