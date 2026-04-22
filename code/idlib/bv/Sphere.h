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

#ifndef __BV_SPHERE_H__
#define __BV_SPHERE_H__

/*!
	\class idSphere
	\brief A geometric primitive representing a sphere in 3D space with methods for manipulation and intersection testing.

	This class provides a complete implementation for representing and operating on spheres in three-dimensional space. It supports initialization with various parameters, mathematical operations such
   as translation and expansion, and geometric tests including point containment, sphere intersection, and line/plane intersections. The class is designed for efficient use in collision detection,
   spatial queries, and geometric computations. It includes convenience methods for constructing spheres from points, translations, and rotations, making it suitable for dynamic geometry operations.
   The sphere is internally represented by an origin point and a radius, with special handling for invalid or "cleared" states.

*/
class idSphere
{
public:
	//! Initializes an empty sphere.
	idSphere();

	//! Initializes a sphere with the given point as its origin and zero radius.
	explicit idSphere( const idVec3& point );

	//! Initializes a sphere with the given point as its origin and the specified radius.
	explicit idSphere( const idVec3& point, const float r );

	//! Returns the component of the sphere's origin at the specified index.
	float		  operator[]( const int index ) const;

	//! Provides access to the origin coordinates of the sphere using array indexing
	float&		  operator[]( const int index );

	//! Returns a sphere translated by the given vector.
	idSphere	  operator+( const idVec3& t ) const;

	//! Translates the sphere by adding the given vector to its origin.
	idSphere&	  operator+=( const idVec3& t );
	idSphere	  operator+( const idSphere& s ) const;
	idSphere&	  operator+=( const idSphere& s );

	//! Compares this sphere with another sphere for exact equality.
	bool		  Compare( const idSphere& a ) const;

	//! Compares this sphere with another sphere using the specified epsilon value for floating-point comparison
	bool		  Compare( const idSphere& a, const float epsilon ) const;

	//! Compares two sphere objects for exact equality.
	bool		  operator==( const idSphere& a ) const;

	//! Returns true if this sphere is not equal to the given sphere.
	bool		  operator!=( const idSphere& a ) const;

	//! Clears the sphere by setting its origin to zero and radius to negative one.
	void		  Clear();

	//! Sets the sphere to a single point at the origin with zero radius.
	void		  Zero();

	//! Sets the origin of the sphere to the specified 3D point.
	void		  SetOrigin( const idVec3& o );

	//! Sets the radius of the sphere to the specified value.
	void		  SetRadius( const float r );

	//! Returns the origin of the sphere
	const idVec3& GetOrigin() const;

	//! Returns the radius of the sphere.
	float		  GetRadius() const;

	//! Returns true if the sphere is inside out, indicating an invalid state.
	bool		  IsCleared() const;

	//! Adds a point to the sphere, expanding it if necessary, and returns true if the sphere was expanded.
	bool		  AddPoint( const idVec3& p );

	//! Adds the given sphere to this sphere and returns true if the sphere expanded
	bool		  AddSphere( const idSphere& s );

	//! Returns a sphere expanded in all directions by the given distance
	idSphere	  Expand( const float d ) const;

	//! Expands the sphere bounds in all directions by the specified distance.
	idSphere&	  ExpandSelf( const float d );

	//! Returns a new sphere translated by the specified vector from the current sphere.
	idSphere	  Translate( const idVec3& translation ) const;

	//! Moves the sphere by the specified translation vector and returns a reference to itself.
	idSphere&	  TranslateSelf( const idVec3& translation );

	//! Computes the distance from the sphere to a plane, considering the sphere's radius.
	float		  PlaneDistance( const idPlane& plane ) const;

	//! Determines which side of a plane the sphere resides on, considering an epsilon tolerance for floating-point comparisons.
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	//! Checks if a point is inside or on the boundary of the sphere.
	bool		  ContainsPoint( const idVec3& p ) const;

	//! Checks if this sphere intersects with another sphere, including when they are touching.
	bool		  IntersectsSphere( const idSphere& s ) const;

	//! Checks if a line segment intersects with the sphere.
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	/*!
		\brief Computes the intersection points of a ray with the sphere and returns the scale factors for the ray parameters.

		This function calculates where a ray intersects with the sphere defined by the origin and radius of this idSphere object. The ray is defined by its start point and direction vector. The
	   function returns true if the ray intersects the sphere, and false otherwise. If an intersection occurs, scale1 and scale2 contain the parametric distances along the ray where the intersection
	   points occur. These scale factors can be used to compute the actual intersection points by applying them to the ray equation.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale1 The first scale factor for the ray parameter (distance to first intersection point)
		\param scale2 The second scale factor for the ray parameter (distance to second intersection point)
		\return True if the ray intersects the sphere, false otherwise
	*/
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale1, float& scale2 ) const;

	//! Computes the tightest-fitting sphere that encloses all given points.
	void		  FromPoints( const idVec3* points, const int numPoints );

	//! Computes the tightest sphere that encompasses a translation from a given point.
	void		  FromPointTranslation( const idVec3& point, const idVec3& translation );

	//! Computes a new sphere from an initial sphere and a translation vector.
	void		  FromSphereTranslation( const idSphere& sphere, const idVec3& start, const idVec3& translation );

	//! Computes the minimal bounding sphere for a point rotated by a rotation.
	void		  FromPointRotation( const idVec3& point, const idRotation& rotation );

	//! Computes a new sphere from a rotated sphere, start point, and rotation.
	void		  FromSphereRotation( const idSphere& sphere, const idVec3& start, const idRotation& rotation );

	//! Projects the sphere onto an axis defined by the direction vector and returns the minimum and maximum values of the projection.
	void		  AxisProjection( const idVec3& dir, float& min, float& max ) const;

private:
	idVec3 origin;
	float  radius;
};

extern idSphere sphere_zero;

ID_INLINE		idSphere::idSphere()
{
}

ID_INLINE idSphere::idSphere( const idVec3& point )
{
	origin = point;
	radius = 0.0f;
}

ID_INLINE idSphere::idSphere( const idVec3& point, const float r )
{
	origin = point;
	radius = r;
}

ID_INLINE float idSphere::operator[]( const int index ) const
{
	return ( ( float* )&origin )[index];
}

ID_INLINE float& idSphere::operator[]( const int index )
{
	return ( ( float* )&origin )[index];
}

ID_INLINE idSphere idSphere::operator+( const idVec3& t ) const
{
	return idSphere( origin + t, radius );
}

ID_INLINE idSphere& idSphere::operator+=( const idVec3& t )
{
	origin += t;
	return *this;
}

ID_INLINE bool idSphere::Compare( const idSphere& a ) const
{
	return ( origin.Compare( a.origin ) && radius == a.radius );
}

ID_INLINE bool idSphere::Compare( const idSphere& a, const float epsilon ) const
{
	return ( origin.Compare( a.origin, epsilon ) && idMath::Fabs( radius - a.radius ) <= epsilon );
}

ID_INLINE bool idSphere::operator==( const idSphere& a ) const
{
	return Compare( a );
}

ID_INLINE bool idSphere::operator!=( const idSphere& a ) const
{
	return !Compare( a );
}

ID_INLINE void idSphere::Clear()
{
	origin.Zero();
	radius = -1.0f;
}

ID_INLINE void idSphere::Zero()
{
	origin.Zero();
	radius = 0.0f;
}

ID_INLINE void idSphere::SetOrigin( const idVec3& o )
{
	origin = o;
}

ID_INLINE void idSphere::SetRadius( const float r )
{
	radius = r;
}

ID_INLINE const idVec3& idSphere::GetOrigin() const
{
	return origin;
}

ID_INLINE float idSphere::GetRadius() const
{
	return radius;
}

ID_INLINE bool idSphere::IsCleared() const
{
	return ( radius < 0.0f );
}

ID_INLINE bool idSphere::AddPoint( const idVec3& p )
{
	if( radius < 0.0f ) {
		origin = p;
		radius = 0.0f;
		return true;
	} else {
		float r = ( p - origin ).LengthSqr();
		if( r > radius * radius ) {
			r = idMath::Sqrt( r );
			origin += ( p - origin ) * 0.5f * ( 1.0f - radius / r );
			radius += 0.5f * ( r - radius );
			return true;
		}
		return false;
	}
}

ID_INLINE bool idSphere::AddSphere( const idSphere& s )
{
	if( radius < 0.0f ) {
		origin = s.origin;
		radius = s.radius;
		return true;
	} else {
		float r = ( s.origin - origin ).LengthSqr();
		if( r > ( radius + s.radius ) * ( radius + s.radius ) ) {
			r = idMath::Sqrt( r );
			origin += ( s.origin - origin ) * 0.5f * ( 1.0f - radius / ( r + s.radius ) );
			radius += 0.5f * ( ( r + s.radius ) - radius );
			return true;
		}
		return false;
	}
}

ID_INLINE idSphere idSphere::Expand( const float d ) const
{
	return idSphere( origin, radius + d );
}

ID_INLINE idSphere& idSphere::ExpandSelf( const float d )
{
	radius += d;
	return *this;
}

ID_INLINE idSphere idSphere::Translate( const idVec3& translation ) const
{
	return idSphere( origin + translation, radius );
}

ID_INLINE idSphere& idSphere::TranslateSelf( const idVec3& translation )
{
	origin += translation;
	return *this;
}

ID_INLINE bool idSphere::ContainsPoint( const idVec3& p ) const
{
	if( ( p - origin ).LengthSqr() > radius * radius ) { return false; }
	return true;
}

ID_INLINE bool idSphere::IntersectsSphere( const idSphere& s ) const
{
	float r = s.radius + radius;
	if( ( s.origin - origin ).LengthSqr() > r * r ) { return false; }
	return true;
}

ID_INLINE void idSphere::FromPointTranslation( const idVec3& point, const idVec3& translation )
{
	origin = point + 0.5f * translation;
	radius = idMath::Sqrt( 0.5f * translation.LengthSqr() );
}

ID_INLINE void idSphere::FromSphereTranslation( const idSphere& sphere, const idVec3& start, const idVec3& translation )
{
	origin = start + sphere.origin + 0.5f * translation;
	radius = idMath::Sqrt( 0.5f * translation.LengthSqr() ) + sphere.radius;
}

ID_INLINE void idSphere::FromPointRotation( const idVec3& point, const idRotation& rotation )
{
	idVec3 end = rotation * point;
	origin	   = ( point + end ) * 0.5f;
	radius	   = idMath::Sqrt( 0.5f * ( end - point ).LengthSqr() );
}

ID_INLINE void idSphere::FromSphereRotation( const idSphere& sphere, const idVec3& start, const idRotation& rotation )
{
	idVec3 end = rotation * sphere.origin;
	origin	   = start + ( sphere.origin + end ) * 0.5f;
	radius	   = idMath::Sqrt( 0.5f * ( end - sphere.origin ).LengthSqr() ) + sphere.radius;
}

ID_INLINE void idSphere::AxisProjection( const idVec3& dir, float& min, float& max ) const
{
	float d;
	d	= dir * origin;
	min = d - radius;
	max = d + radius;
}

#endif /* !__BV_SPHERE_H__ */
