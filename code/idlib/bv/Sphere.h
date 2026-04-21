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

/*!
	\file idlib/bv/Sphere.h
	\brief Defines the `idSphere` class, a basic geometric primitive used for bounding volumes.
	\note archgen: sha256=09254cdac7762dcfe99a7e8f43aefe23e7cf2ea052bd07d474c94e66b8604ecc

	\par File Purpose
	- Defines the `idSphere` class, a basic geometric primitive used for bounding volumes.
	- Provides functionality for spherical collision detection, spatial queries, and bounding sphere construction.

	\par Core Responsibilities
	- Representing a 3D sphere via an origin (`idVec3`) and a radius (`float`).
	- Performing intersection tests between spheres, lines, and rays.
	- Determining spatial relationships between spheres and planes.
	- Managing sphere expansion logic when adding points or other spheres to a bounding volume.
	- Constructing spheres from complex geometric transformations (rotation, translation, etc.).

	\par Key Types and Functions
	- idSphere — Represents a 3D bounding sphere defined by a center and radius.
	- idSphere::AddPoint(const idVec3& p) — Expands the sphere to encompass a new point, recalculating the center and radius if necessary.
	- idSphere::AddSphere(const idSphere& s) — Expands the sphere to encompass another sphere.
	- idSphere::IntersectsSphere(const idSphere& s) — Performs an efficient overlap test between two spheres using squared distance.
	- idSphere::PlaneSide(const idPlane& plane, const float epsilon) — Returns whether a sphere is in front of, behind, or intersecting a plane.
	- idSphere::RayIntersection(const idVec3& start, const idVec3& dir, float& scale1, float& scale2) — Calculates the entry and exit parameters of a ray passing through the sphere.
	- idSphere::FromPoints(const idVec3* points, const int numPoints) — Computes the minimal bounding sphere for a provided array of 3D points.
	- idSphere::Clear() — Sets the sphere to an 'inside out' or invalid state by setting radius to -1.0f.

	\par Control Flow
	- The sphere maintains an 'invalid' state when `radius < 0.0f` (triggered by `Clear()`), which forces subsequent `AddPoint` or `AddSphere` calls to initialize the sphere's origin based on the
   input.
	- Intersection logic prioritizes performance by utilizing squared distances to avoid expensive square root operations.
	- The `AddPoint` and `AddSphere` algorithms use weighted averages and geometric midpoints to expand the bounding volume while attempting to keep the sphere as tight as possible.
	- Transformation methods like `Translate` and `Expand` follow a functional pattern, returning new objects, while `TranslateSelf` and `ExpandSelf` provide in-place modification for efficiency.

	\par Dependencies
	- idVec3 — For 3D vector arithmetic and coordinate representation.
	- idPlane — For sphere-plane intersection and distance queries.
	- idRotation — For computing spheres resulting from rotational transformations.
	- idMath — For mathematical utilities such as `Sqrt` and `Fabs`.
	- ON_EPSILON — For floating-point tolerance in spatial comparisons.

	\par How It Fits
	- Part of the `idlib/bv` (Bounding Volume) system.
	- Acts as a primary primitive for the engine's collision detection pipeline.
	- Used by spatial partitioning structures (e.g., Octrees) to perform rapid rejection tests for visibility and physics queries.
*/

#ifndef __BV_SPHERE_H__
#define __BV_SPHERE_H__

/*!
	\class idSphere
	\brief A mathematical sphere class used for geometric calculations and spatial queries in the engine.

	The idSphere class represents a 3D geometric sphere with origin point and radius, designed for efficient spatial operations and collision detection. It provides methods for sphere construction,
   transformation, intersection testing, and geometric queries. The class supports both point and sphere inclusion tests, line and ray intersection calculations, and various projection operations. It
   is optimized for performance with inline methods and avoids expensive operations like square root calculations where possible. The sphere can be used for bounding volumes, collision detection, and
   spatial partitioning within the engine's geometric algorithms.

*/
class idSphere
{
public:
	//! Constructs an empty sphere with default initialization.
	idSphere();

	/*!
		\brief Constructs a sphere with the specified origin point and zero radius.
		\param point The origin point of the sphere
	*/
	explicit idSphere( const idVec3& point );

	/*!
		\brief Constructs a sphere with the specified origin point and radius.
		\param point The origin point of the sphere
		\param r The radius of the sphere
	*/
	explicit idSphere( const idVec3& point, const float r );

	float		  operator[]( const int index ) const;
	float&		  operator[]( const int index );
	idSphere	  operator+( const idVec3& t ) const; // returns tranlated sphere
	idSphere&	  operator+=( const idVec3& t );	  // translate the sphere
	idSphere	  operator+( const idSphere& s ) const;
	idSphere&	  operator+=( const idSphere& s );

	/*!
		\brief Compares this sphere with another sphere for exact equality, considering both origin and radius.

		This function performs an exact comparison between two spheres, checking if their origins are exactly equal and if their radii are equal. It does not use any epsilon tolerance for
	   floating-point comparisons, making it suitable for cases where exact equality is required. The function is marked as ID_INLINE, indicating it should be inlined for performance.

		\param a The sphere to compare with this sphere
		\return True if both the origin and radius of the two spheres are exactly equal, false otherwise
	*/
	bool		  Compare( const idSphere& a ) const;

	/*!
		\brief Compares this sphere with another sphere using the specified epsilon tolerance for both origin and radius.

		This function performs a component-wise comparison of the sphere's origin with the origin of another sphere, and checks if the absolute difference between the radii is less than or equal to
	   the provided epsilon value. The comparison is done using the idMath::Fabs function for floating-point absolute value calculation, ensuring that both the position and size of the spheres are
	   within the specified tolerance.

		\param a The other sphere to compare against
		\param epsilon The tolerance value for comparing both origin coordinates and radius
		\return True if the origins of the two spheres are within the epsilon tolerance and the absolute difference between their radii is less than or equal to epsilon, false otherwise
	*/
	bool		  Compare( const idSphere& a, const float epsilon ) const;
	bool		  operator==( const idSphere& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idSphere& a ) const; // exact compare, no epsilon

	//! Initializes the sphere to represent a point at the origin with zero radius.
	void		  Clear();

	//! Sets the sphere origin to zero and radius to zero.
	void		  Zero();

	/*!
		\brief Sets the origin point of the sphere to the specified position.

		This function updates the origin coordinate of the sphere to the given 3D position. The sphere's origin represents the center point of the sphere in world space.

		\param o The new origin position for the sphere as a 3D vector
	*/
	void		  SetOrigin( const idVec3& o );

	/*!
		\brief Sets the radius of the sphere to the specified value.

		This function updates the radius member variable of the sphere object with the provided value. The radius is stored as a floating-point number representing the distance from the center to the
	   edge of the sphere.

		\param r The new radius value for the sphere
	*/
	void		  SetRadius( const float r );

	//! Returns the origin of the sphere
	const idVec3& GetOrigin() const;

	//! Returns the radius of the sphere.
	float		  GetRadius() const;

	//! Returns true if the sphere is inside out, indicating an invalid state.
	bool		  IsCleared() const;

	/*!
		\brief Adds a point to the sphere and returns true if the sphere expanded.

		This function updates the sphere to include the given point. If the sphere is uninitialized (radius < 0.0f), it sets the origin to the point and initializes the radius to 0.0f. Otherwise, it
	   calculates the squared distance from the current origin to the point. If this distance exceeds the square of the current radius, the sphere is expanded to include the point. The expansion is
	   performed using a weighted average to update both the origin and radius. The function returns true if the sphere was expanded, false otherwise.

		\param p The point to be added to the sphere
		\return true if the sphere was expanded to include the point, false otherwise
	*/
	bool		  AddPoint( const idVec3& p );

	/*!
		\brief Adds a sphere to this sphere and returns true if the sphere expanded.

		This function adds the provided sphere to the current sphere. If the current sphere has a negative radius, it is replaced by the provided sphere. Otherwise, it checks if the provided sphere
	   extends beyond the current sphere's boundaries. If it does, the current sphere is expanded to encompass both spheres, and the function returns true. If the provided sphere is fully contained
	   within the current sphere, no expansion occurs and the function returns false.

		\param s the sphere to be added to this sphere
		\return true if the sphere was expanded to include the provided sphere, false otherwise
	*/
	bool		  AddSphere( const idSphere& s );

	//! Returns a copy of this sphere expanded by the specified distance in all directions
	idSphere	  Expand( const float d ) const;

	/*!
		\brief Expands the sphere's radius by the specified distance and returns a reference to itself.

		This function modifies the sphere by increasing its radius uniformly in all directions by the given distance value. The operation is performed in place, and the function returns a reference to
	   the modified sphere object. This allows for method chaining.

		\param d The distance to expand the sphere's radius in all directions
		\return A reference to the modified sphere object with the expanded radius.
	*/
	idSphere&	  ExpandSelf( const float d );

	//! Creates a new sphere at the translated origin while keeping the same radius.
	idSphere	  Translate( const idVec3& translation ) const;

	/*!
		\brief Moves the sphere by adding the translation vector to its origin.

		This function modifies the sphere's origin by translating it along the specified vector. It performs an in-place transformation of the sphere object and returns a reference to the modified
	   sphere, allowing for method chaining.

		\param translation The vector by which to translate the sphere's origin
		\return A reference to the modified sphere object after the translation has been applied.
	*/
	idSphere&	  TranslateSelf( const idVec3& translation );

	/*!
		\brief Calculates the distance from the sphere's origin to a plane, accounting for the sphere's radius.

		This function computes the signed distance from the sphere's center to the given plane. If the distance is greater than the sphere's radius, it returns the difference, indicating the sphere is
	   in front of the plane. If the distance is less than the negative radius, it returns the sum, indicating the sphere is behind the plane. If the sphere intersects the plane, it returns zero. This
	   is useful for collision detection and spatial queries.

		\param plane The plane to calculate the distance to.
		\return The signed distance from the sphere's origin to the plane, adjusted by the sphere's radius to account for intersection scenarios.
	*/
	float		  PlaneDistance( const idPlane& plane ) const;

	/*!
		\brief Determines on which side of a plane a sphere lies, considering an epsilon tolerance for floating-point comparisons

		This function evaluates the spatial relationship between a sphere and a plane by calculating the distance from the sphere's origin to the plane. It returns a value indicating whether the
	   sphere is entirely in front of the plane, entirely behind it, or crossing it. The epsilon parameter allows for a small margin of error in the comparison, which is useful for handling
	   floating-point precision issues. The function is commonly used in computational geometry and collision detection algorithms.

		\param plane The plane to test against
		\param epsilon A tolerance value for floating-point comparisons, defaults to ON_EPSILON
		\return PLANESIDE_FRONT if the sphere is completely in front of the plane, PLANESIDE_BACK if completely behind, and PLANESIDE_CROSS if the sphere crosses the plane
	*/
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	/*!
		\brief Checks if a given point is inside or on the surface of the sphere.

		This function determines whether a specified point lies within the bounds of the sphere, including points that are exactly on the surface. It calculates the squared distance from the point to
	   the sphere's origin and compares it with the squared radius to avoid expensive square root calculations.

		\param p The point to check against the sphere.
		\return True if the point is inside or on the surface of the sphere, false otherwise.
	*/
	bool		  ContainsPoint( const idVec3& p ) const;

	/*!
		\brief Checks if this sphere intersects with another sphere, including when they are touching.

		The function determines whether two spheres overlap or touch each other in 3D space. It calculates the squared distance between the origins of the two spheres and compares it to the squared
	   sum of their radii. This approach avoids computing square roots, which improves performance. The comparison includes the case where the spheres are just touching, meaning they intersect when
	   the distance between their centers equals the sum of their radii.

		\param s The other sphere to check intersection with
		\return True if the spheres intersect or touch, false otherwise
	*/
	bool		  IntersectsSphere( const idSphere& s ) const;

	/*!
		\brief Checks if a line segment intersects with the sphere.

		This function determines whether a line segment defined by two points, start and end, intersects with the sphere defined by the origin and radius of this idSphere object. The algorithm
	   computes the closest point on the line to the sphere center and checks if that point lies within the sphere. It handles three cases: when the closest point is at the start, when it's at the
	   end, and when it's somewhere in between the two points.

		\param start The starting point of the line segment
		\param end The ending point of the line segment
		\return True if the line segment intersects the sphere, false otherwise
	*/
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	/*!
		\brief Computes the intersection of a ray with the sphere and returns the scale values for the intersection points

		This function calculates the intersection points of a ray defined by a start point and direction with the sphere. The ray is parameterized as start + t * dir where t is the scale factor. The
	   function returns true if the ray intersects the sphere, and false otherwise. If an intersection occurs, the scale values scale1 and scale2 represent the two possible intersection points along
	   the ray. The sphere is defined by its origin and radius

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale1 The first scale factor for the intersection point
		\param scale2 The second scale factor for the intersection point
		\return True if the ray intersects the sphere, false otherwise
	*/
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale1, float& scale2 ) const;

	/*!
		\brief Computes the tightest fitting sphere that contains all provided points.

		This function calculates the minimum bounding sphere for a set of 3D points. It first determines the bounding box of all points to establish an initial origin. Then it iterates through all
	   points to find the maximum squared distance from the origin, which becomes the radius squared. The final radius is calculated as the square root of this value.

		\param points Array of 3D points to calculate the bounding sphere for
		\param numPoints Number of points in the points array
	*/
	void		  FromPoints( const idVec3* points, const int numPoints );

	/*!
		\brief Computes the minimal bounding sphere for a point translating along a vector.

		This function calculates the origin and radius of the smallest sphere that can contain a point as it translates along a given vector. The origin is positioned at the midpoint of the
	   translation vector added to the initial point, and the radius is derived from the length of the translation vector.

		\param point The starting position of the point
		\param translation The vector representing the translation of the point
	*/
	void		  FromPointTranslation( const idVec3& point, const idVec3& translation );

	/*!
		\brief Computes a new sphere based on an initial sphere, a start point, and a translation vector.

		The function calculates the origin of the new sphere by adding the start point, the original sphere's origin, and half the translation vector. The radius is computed as the square root of half
	   the squared length of the translation vector plus the original sphere's radius. This operation is typically used to determine how a sphere changes when translated from a start position.

		\param sphere The original sphere used to compute the new sphere's radius and origin.
		\param start The starting position from which the sphere translation begins.
		\param translation The vector representing the translation applied to the sphere.
	*/
	void		  FromSphereTranslation( const idSphere& sphere, const idVec3& start, const idVec3& translation );

	/*!
		\brief Computes the tightest sphere that encompasses a point after it has been rotated.

		This function calculates the sphere that tightly encloses a point after it has been transformed by a rotation. It determines the center of the sphere as the midpoint between the original point
	   and its rotated position, and sets the radius to half the distance between these two points. The sphere is constructed to be as small as possible while still containing both the original and
	   rotated positions.

		\param point The point in 3D space that will be rotated
		\param rotation The rotation to be applied to the point
	*/
	void		  FromPointRotation( const idVec3& point, const idRotation& rotation );

	/*!
		\brief Computes a new sphere that represents the result of rotating the input sphere around a given point

		This function takes an input sphere, a start point, and a rotation. It applies the rotation to the sphere's origin, then computes a new sphere that encompasses both the original sphere and its
	   rotated version. The new sphere's origin is set to the midpoint between the start point and the rotated sphere origin, and its radius is calculated based on the distance between the original
	   and rotated origins plus the original sphere's radius

		\param sphere Input sphere to be rotated
		\param start Point around which the rotation occurs
		\param rotation Rotation to be applied to the sphere's origin
	*/
	void		  FromSphereRotation( const idSphere& sphere, const idVec3& start, const idRotation& rotation );

	/*!
		\brief Computes the minimum and maximum projections of the sphere along a given direction vector.

		This function calculates the range of scalar projections of the sphere's surface onto a specified direction vector. It determines the minimum and maximum values of the dot product between the
	   direction vector and all points on the sphere's surface. The calculation uses the sphere's origin and radius to efficiently compute these values without iterating through individual points.

		\param dir The direction vector along which to project the sphere
		\param min Output parameter for the minimum projection value
		\param max Output parameter for the maximum projection value
	*/
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
