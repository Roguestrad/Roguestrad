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

#ifndef __BV_BOUNDS_H__
#define __BV_BOUNDS_H__

/*!
	\class idBounds
	\brief Represents an axis-aligned bounding box with methods for manipulation and spatial queries.

	This class encapsulates an axis-aligned bounding box defined by minimum and maximum 3D vectors. It provides constructors for various initialization scenarios, including creation from points,
   bounds, or translation/rotation transformations. The class supports standard arithmetic operations such as addition, multiplication, intersection, and expansion, allowing for dynamic adjustment of
   the bounding volume. It includes methods for checking containment, intersection, and spatial relationships with other primitives like planes and rays. Additionally, it offers utility functions to
   compute the bounding box's center, radius, volume, and corner points, as well as projection operations along arbitrary directions.

*/
class idBounds
{
public:
	//! Initializes an empty bounding box.
	idBounds();

	//! Constructs an idBounds object with specified minimum and maximum vectors.
	explicit idBounds( const idVec3& mins, const idVec3& maxs );

	//! Constructs a bounds object initialized to a single point.
	explicit idBounds( const idVec3& point );

	//! Returns a const reference to the idVec3 at the specified index in the bounds
	const idVec3& operator[]( const int index ) const;

	//! Provides indexed access to the bounds data
	idVec3&		  operator[]( const int index );

	//! Returns bounds translated by the given vector
	idBounds	  operator+( const idVec3& t ) const;

	//! Translates the bounds by the given vector.
	idBounds&	  operator+=( const idVec3& t );

	//! Returns the bounds rotated by the given matrix.
	idBounds	  operator*( const idMat3& r ) const;

	//! Rotates the bounds by the given transformation matrix.
	idBounds&	  operator*=( const idMat3& r );

	//! Returns a new bounds object that is the result of adding the current bounds and the provided bounds.
	idBounds	  operator+( const idBounds& a ) const;

	//! Adds the bounds of another idBounds object to this bounds object and returns a reference to this object.
	idBounds&	  operator+=( const idBounds& a );

	//! Computes the difference between this bounding box and another, ensuring the result fits within the original bounds.
	idBounds	  operator-( const idBounds& a ) const;

	//! Subtracts the bounds of another idBounds object from this object.
	idBounds&	  operator-=( const idBounds& a );

	//! Compares this bounds object with another bounds object for exact equality.
	bool		  Compare( const idBounds& a ) const;

	//! Compares this bounds with another bounds using the specified epsilon value
	bool		  Compare( const idBounds& a, const float epsilon ) const;

	//! Compares two idBounds objects for exact equality.
	bool		  operator==( const idBounds& a ) const;

	//! Returns true if this bounds object is not equal to the given bounds object.
	bool		  operator!=( const idBounds& a ) const;

	//! Resets the bounds to an inside-out state where the minimum values are set to positive infinity and maximum values to negative infinity.
	void		  Clear();

	//! Sets all boundary coordinates to zero, representing a single point at the origin.
	void		  Zero();

	//! Returns the center point of the bounding box.
	idVec3		  GetCenter() const;

	//! Returns the radius of the bounding box relative to its origin.
	float		  GetRadius() const;

	//! Returns the radius of the bounding box relative to the specified center point.
	float		  GetRadius( const idVec3& center ) const;

	//! Returns the volume of the bounding box.
	float		  GetVolume() const;

	//! Returns true if the bounds are inside out, indicating an invalid or cleared state.
	bool		  IsCleared() const;

	//! Adds a point to the bounds, returning true if the bounds expanded.
	bool		  AddPoint( const idVec3& v );

	//! Adds the given bounds to this bounds and returns true if the bounds expanded.
	bool		  AddBounds( const idBounds& a );

	//! Returns the intersection of this bounding box with another bounding box.
	idBounds	  Intersect( const idBounds& a ) const;

	//! Intersects this bounds object with the given bounds, modifying this object to represent their intersection.
	idBounds&	  IntersectSelf( const idBounds& a );

	//! Returns bounds expanded in all directions by the specified distance
	idBounds	  Expand( const float d ) const;

	//! Expands the bounds by the specified distance in all directions.
	idBounds&	  ExpandSelf( const float d );

	//! Returns bounds translated by the specified vector.
	idBounds	  Translate( const idVec3& translation ) const;

	//! Translates the bounds by the specified translation vector and returns a reference to itself.
	idBounds&	  TranslateSelf( const idVec3& translation );

	//! Returns the bounding box rotated by the specified rotation matrix.
	idBounds	  Rotate( const idMat3& rotation ) const;

	//! Rotates the bounds by the provided rotation matrix and returns a reference to itself.
	idBounds&	  RotateSelf( const idMat3& rotation );

	//! Computes the signed distance from the bounds to a plane, considering the bounds' orientation and size.
	float		  PlaneDistance( const idPlane& plane ) const;

	//! Determines which side of a plane the bounding box resides on, using an epsilon for floating point tolerance.
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	//! Returns the size of the bounding box as a vector.
	idVec3		  GetSize() const;

	//! Checks if any dimension of the bounding box has negative size.
	bool		  IsBackwards() const;

	//! Checks if a given point is contained within the bounds, including points on the boundary.
	bool		  ContainsPoint( const idVec3& p ) const;

	//! Checks if this bounding box intersects with another bounding box, including cases where they touch.
	bool		  IntersectsBounds( const idBounds& a ) const;

	//! Tests if a line intersects with the bounding box.
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	//! Checks if a ray intersects with the bounding box and calculates the intersection scale.
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale ) const;

	//! Computes the tightest axis-aligned bounding box for a transformed bounds
	void		  FromTransformedBounds( const idBounds& bounds, const idVec3& origin, const idMat3& axis );

	//! Computes the tightest bounding box that contains all given points.
	void		  FromPoints( const idVec3* points, const int numPoints );

	//! Computes the tightest bounding box that encloses both the initial point and its translated position.
	void		  FromPointTranslation( const idVec3& point, const idVec3& translation );

	/*!
		\brief Initializes the bounding box from a transformed bounds with translation applied.

		The function sets up a bounding box by first applying a transformation consisting of an origin and axis to the input bounds. If the axis represents a rotation, it uses a specialized function
	   for transformed bounds. Otherwise, it directly applies the origin to both minimum and maximum points of the bounds. After this initial transformation, it applies the translation vector to
	   adjust the final bounding box, ensuring that negative translation values affect the minimum point and positive values affect the maximum point.

		\param bounds The input bounding box to be transformed
		\param origin The origin point for the transformation
		\param axis The axis matrix for the transformation, potentially representing rotation
		\param translation The translation vector to be applied after the initial transformation
	*/
	void		  FromBoundsTranslation( const idBounds& bounds, const idVec3& origin, const idMat3& axis, const idVec3& translation );

	//! Computes the tightest axis-aligned bounding box for a point rotated around an origin.
	void		  FromPointRotation( const idVec3& point, const idRotation& rotation );

	/*!
		\brief Computes the bounding box for a transformed bounds object using rotation.

		This function calculates the bounding box of a transformed bounds object by applying a rotation to the original bounds. It handles two cases: when the rotation angle is less than 180 degrees,
	   it computes the bounds by transforming each corner of the original bounds and taking the union of the resulting rotated bounds. When the rotation angle is 180 degrees or more, it computes a
	   conservative bounding box based on the radius of the original bounds and the rotation origin, which may result in a larger bounding box than necessary.

		\param bounds The original bounds to be transformed
		\param origin The origin point for the transformation
		\param axis The axis matrix for the transformation
		\param rotation The rotation to be applied to the bounds
	*/
	void		  FromBoundsRotation( const idBounds& bounds, const idVec3& origin, const idMat3& axis, const idRotation& rotation );

	//! Computes the 8 corner points of the bounding box and stores them in the provided array.
	void		  ToPoints( idVec3 points[8] ) const;

	//! Returns a sphere that contains the bounding box.
	idSphere	  ToSphere() const;

	//! Computes the minimum and maximum projections of the bounding box onto a given direction vector.
	void		  AxisProjection( const idVec3& dir, float& min, float& max ) const;

	/*!
		\brief Computes the minimum and maximum scalar projections of the bounding box along a specified direction after applying an axis transformation.

		This function calculates the projection interval of the bounding box onto a given direction vector. The projection is computed after transforming the box center and extents by the provided
	   axis matrix. The min and max values represent the range of the projection along the specified direction.

		\param origin The origin point to which the box center is offset before applying the axis transformation
		\param axis The transformation matrix to apply to the box center and extents
		\param dir The direction vector along which to compute the projection
		\param min Output parameter for the minimum projection value
		\param max Output parameter for the maximum projection value
	*/
	void		  AxisProjection( const idVec3& origin, const idMat3& axis, const idVec3& dir, float& min, float& max ) const;

	//! Returns the dimension of the bounds, which is always 6.
	int			  GetDimension() const;

	//! Returns a pointer to the first element of the bounds data as a float array.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the float representation of the bounds data.
	float*		  ToFloatPtr();

private:
	idVec3 b[2];
};

extern idBounds bounds_zero;
extern idBounds bounds_zeroOneCube;
extern idBounds bounds_unitCube;

ID_INLINE		idBounds::idBounds()
{
}

ID_INLINE idBounds::idBounds( const idVec3& mins, const idVec3& maxs )
{
	b[0] = mins;
	b[1] = maxs;
}

ID_INLINE idBounds::idBounds( const idVec3& point )
{
	b[0] = point;
	b[1] = point;
}

ID_INLINE const idVec3& idBounds::operator[]( const int index ) const
{
	return b[index];
}

ID_INLINE idVec3& idBounds::operator[]( const int index )
{
	return b[index];
}

ID_INLINE idBounds idBounds::operator+( const idVec3& t ) const
{
	return idBounds( b[0] + t, b[1] + t );
}

ID_INLINE idBounds& idBounds::operator+=( const idVec3& t )
{
	b[0] += t;
	b[1] += t;
	return *this;
}

ID_INLINE idBounds idBounds::operator*( const idMat3& r ) const
{
	idBounds bounds;
	bounds.FromTransformedBounds( *this, vec3_origin, r );
	return bounds;
}

ID_INLINE idBounds& idBounds::operator*=( const idMat3& r )
{
	this->FromTransformedBounds( *this, vec3_origin, r );
	return *this;
}

ID_INLINE idBounds idBounds::operator+( const idBounds& a ) const
{
	idBounds newBounds;
	newBounds = *this;
	newBounds.AddBounds( a );
	return newBounds;
}

ID_INLINE idBounds& idBounds::operator+=( const idBounds& a )
{
	idBounds::AddBounds( a );
	return *this;
}

ID_INLINE idBounds idBounds::operator-( const idBounds& a ) const
{
	assert( b[1][0] - b[0][0] > a.b[1][0] - a.b[0][0] && b[1][1] - b[0][1] > a.b[1][1] - a.b[0][1] && b[1][2] - b[0][2] > a.b[1][2] - a.b[0][2] );
	return idBounds( idVec3( b[0][0] + a.b[1][0], b[0][1] + a.b[1][1], b[0][2] + a.b[1][2] ), idVec3( b[1][0] + a.b[0][0], b[1][1] + a.b[0][1], b[1][2] + a.b[0][2] ) );
}

ID_INLINE idBounds& idBounds::operator-=( const idBounds& a )
{
	assert( b[1][0] - b[0][0] > a.b[1][0] - a.b[0][0] && b[1][1] - b[0][1] > a.b[1][1] - a.b[0][1] && b[1][2] - b[0][2] > a.b[1][2] - a.b[0][2] );
	b[0] += a.b[1];
	b[1] += a.b[0];
	return *this;
}

ID_INLINE bool idBounds::Compare( const idBounds& a ) const
{
	return ( b[0].Compare( a.b[0] ) && b[1].Compare( a.b[1] ) );
}

ID_INLINE bool idBounds::Compare( const idBounds& a, const float epsilon ) const
{
	return ( b[0].Compare( a.b[0], epsilon ) && b[1].Compare( a.b[1], epsilon ) );
}

ID_INLINE bool idBounds::operator==( const idBounds& a ) const
{
	return Compare( a );
}

ID_INLINE bool idBounds::operator!=( const idBounds& a ) const
{
	return !Compare( a );
}

ID_INLINE void idBounds::Clear()
{
	b[0][0] = b[0][1] = b[0][2] = idMath::INFINITUM;
	b[1][0] = b[1][1] = b[1][2] = -idMath::INFINITUM;
}

ID_INLINE void idBounds::Zero()
{
	b[0][0] = b[0][1] = b[0][2] = b[1][0] = b[1][1] = b[1][2] = 0;
}

ID_INLINE idVec3 idBounds::GetCenter() const
{
	return idVec3( ( b[1][0] + b[0][0] ) * 0.5f, ( b[1][1] + b[0][1] ) * 0.5f, ( b[1][2] + b[0][2] ) * 0.5f );
}

ID_INLINE float idBounds::GetVolume() const
{
	if( b[0][0] >= b[1][0] || b[0][1] >= b[1][1] || b[0][2] >= b[1][2] ) { return 0.0f; }
	return ( ( b[1][0] - b[0][0] ) * ( b[1][1] - b[0][1] ) * ( b[1][2] - b[0][2] ) );
}

ID_INLINE bool idBounds::IsCleared() const
{
	return b[0][0] > b[1][0];
}

ID_INLINE bool idBounds::AddPoint( const idVec3& v )
{
	bool expanded = false;
	if( v[0] < b[0][0] ) {
		b[0][0]	 = v[0];
		expanded = true;
	}
	if( v[0] > b[1][0] ) {
		b[1][0]	 = v[0];
		expanded = true;
	}
	if( v[1] < b[0][1] ) {
		b[0][1]	 = v[1];
		expanded = true;
	}
	if( v[1] > b[1][1] ) {
		b[1][1]	 = v[1];
		expanded = true;
	}
	if( v[2] < b[0][2] ) {
		b[0][2]	 = v[2];
		expanded = true;
	}
	if( v[2] > b[1][2] ) {
		b[1][2]	 = v[2];
		expanded = true;
	}
	return expanded;
}

ID_INLINE bool idBounds::AddBounds( const idBounds& a )
{
	bool expanded = false;
	if( a.b[0][0] < b[0][0] ) {
		b[0][0]	 = a.b[0][0];
		expanded = true;
	}
	if( a.b[0][1] < b[0][1] ) {
		b[0][1]	 = a.b[0][1];
		expanded = true;
	}
	if( a.b[0][2] < b[0][2] ) {
		b[0][2]	 = a.b[0][2];
		expanded = true;
	}
	if( a.b[1][0] > b[1][0] ) {
		b[1][0]	 = a.b[1][0];
		expanded = true;
	}
	if( a.b[1][1] > b[1][1] ) {
		b[1][1]	 = a.b[1][1];
		expanded = true;
	}
	if( a.b[1][2] > b[1][2] ) {
		b[1][2]	 = a.b[1][2];
		expanded = true;
	}
	return expanded;
}

ID_INLINE idBounds idBounds::Intersect( const idBounds& a ) const
{
	idBounds n;
	n.b[0][0] = ( a.b[0][0] > b[0][0] ) ? a.b[0][0] : b[0][0];
	n.b[0][1] = ( a.b[0][1] > b[0][1] ) ? a.b[0][1] : b[0][1];
	n.b[0][2] = ( a.b[0][2] > b[0][2] ) ? a.b[0][2] : b[0][2];
	n.b[1][0] = ( a.b[1][0] < b[1][0] ) ? a.b[1][0] : b[1][0];
	n.b[1][1] = ( a.b[1][1] < b[1][1] ) ? a.b[1][1] : b[1][1];
	n.b[1][2] = ( a.b[1][2] < b[1][2] ) ? a.b[1][2] : b[1][2];
	return n;
}

ID_INLINE idBounds& idBounds::IntersectSelf( const idBounds& a )
{
	if( a.b[0][0] > b[0][0] ) { b[0][0] = a.b[0][0]; }
	if( a.b[0][1] > b[0][1] ) { b[0][1] = a.b[0][1]; }
	if( a.b[0][2] > b[0][2] ) { b[0][2] = a.b[0][2]; }
	if( a.b[1][0] < b[1][0] ) { b[1][0] = a.b[1][0]; }
	if( a.b[1][1] < b[1][1] ) { b[1][1] = a.b[1][1]; }
	if( a.b[1][2] < b[1][2] ) { b[1][2] = a.b[1][2]; }
	return *this;
}

ID_INLINE idBounds idBounds::Expand( const float d ) const
{
	return idBounds( idVec3( b[0][0] - d, b[0][1] - d, b[0][2] - d ), idVec3( b[1][0] + d, b[1][1] + d, b[1][2] + d ) );
}

ID_INLINE idBounds& idBounds::ExpandSelf( const float d )
{
	b[0][0] -= d;
	b[0][1] -= d;
	b[0][2] -= d;
	b[1][0] += d;
	b[1][1] += d;
	b[1][2] += d;
	return *this;
}

ID_INLINE idBounds idBounds::Translate( const idVec3& translation ) const
{
	return idBounds( b[0] + translation, b[1] + translation );
}

ID_INLINE idBounds& idBounds::TranslateSelf( const idVec3& translation )
{
	b[0] += translation;
	b[1] += translation;
	return *this;
}

ID_INLINE idBounds idBounds::Rotate( const idMat3& rotation ) const
{
	idBounds bounds;
	bounds.FromTransformedBounds( *this, vec3_origin, rotation );
	return bounds;
}

ID_INLINE idBounds& idBounds::RotateSelf( const idMat3& rotation )
{
	FromTransformedBounds( *this, vec3_origin, rotation );
	return *this;
}

/**
 * Tels: Get the size of the bounds, that is b1 - b0
 */
ID_INLINE idVec3 idBounds::GetSize() const
{
	return idVec3( b[1].x - b[0].x, b[1].y - b[0].y, b[1].z - b[0].z );
}

ID_INLINE bool idBounds::IsBackwards() const
{
	return b[1].x < b[0].x || b[1].y < b[0].y || b[1].z < b[0].z;
}

ID_INLINE bool idBounds::ContainsPoint( const idVec3& p ) const
{
	if( p[0] < b[0][0] || p[1] < b[0][1] || p[2] < b[0][2] || p[0] > b[1][0] || p[1] > b[1][1] || p[2] > b[1][2] ) { return false; }
	return true;
}

ID_INLINE bool idBounds::IntersectsBounds( const idBounds& a ) const
{
	if( a.b[1][0] < b[0][0] || a.b[1][1] < b[0][1] || a.b[1][2] < b[0][2] || a.b[0][0] > b[1][0] || a.b[0][1] > b[1][1] || a.b[0][2] > b[1][2] ) { return false; }
	return true;
}

ID_INLINE idSphere idBounds::ToSphere() const
{
	idSphere sphere;
	sphere.SetOrigin( ( b[0] + b[1] ) * 0.5f );
	sphere.SetRadius( ( b[1] - sphere.GetOrigin() ).Length() );
	return sphere;
}

ID_INLINE void idBounds::AxisProjection( const idVec3& dir, float& min, float& max ) const
{
	float  d1, d2;
	idVec3 center, extents;

	center	= ( b[0] + b[1] ) * 0.5f;
	extents = b[1] - center;

	d1 = dir * center;
	d2 = idMath::Fabs( extents[0] * dir[0] ) + idMath::Fabs( extents[1] * dir[1] ) + idMath::Fabs( extents[2] * dir[2] );

	min = d1 - d2;
	max = d1 + d2;
}

ID_INLINE void idBounds::AxisProjection( const idVec3& origin, const idMat3& axis, const idVec3& dir, float& min, float& max ) const
{
	float  d1, d2;
	idVec3 center, extents;

	center	= ( b[0] + b[1] ) * 0.5f;
	extents = b[1] - center;
	center	= origin + center * axis;

	d1 = dir * center;
	d2 = idMath::Fabs( extents[0] * ( dir * axis[0] ) ) + idMath::Fabs( extents[1] * ( dir * axis[1] ) ) + idMath::Fabs( extents[2] * ( dir * axis[2] ) );

	min = d1 - d2;
	max = d1 + d2;
}

ID_INLINE int idBounds::GetDimension() const
{
	return 6;
}

ID_INLINE const float* idBounds::ToFloatPtr() const
{
	return &b[0].x;
}

ID_INLINE float* idBounds::ToFloatPtr()
{
	return &b[0].x;
}

#endif /* !__BV_BOUNDS_H__ */
