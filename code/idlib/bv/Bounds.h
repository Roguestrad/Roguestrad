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
	\file idlib/bv/Bounds.h
	\brief Declare and inline‑implement the idBounds class, the engine’s fundamental tool for spatial extents.
	\note archgen: sha256=59d7e385174605c7bca742be12533eec379f29beb025f22f9cb95e0bc91f9710

	\par File Purpose
	- Declare and inline‑implement the idBounds class, the engine’s fundamental tool for spatial extents.
	- Provide a header‑only implementation to avoid multiple definition issues and to enable inlining for performance-critical paths.

	\par Core Responsibilities
	- Represent a 3‑D axis‑aligned bounding box with minimum and maximum corner vectors.
	- Provide constructors for emptiness, minima/maxima, single point, and inline zero/clear init.
	- Implement arithmetic operations: union, subtraction of a constant box, translation, and rotation.
	- Offer geometric queries: center, volume, radius, size, containment, intersection, and sphere conversion.
	- Expose low‑level helpers for transforming bounds, computing axis projections, and generating corner points.
	- Handle epsilon‑based comparison and clear/backward state checks.

	\par Key Types and Functions
	- idBounds — represents an axis‑aligned bounding box with min/max corners.
	- idBounds::idBounds() — default constructor (empty box).
	- idBounds::idBounds(const idVec3& mins, const idVec3& maxs) — initializes with explicit bounds.
	- idBounds::idBounds(const idVec3& point) — creates a zero‑volume box at a point.
	- idBounds::operator[](int) const/— — access min/max corners.
	- idBounds::operator+(const idVec3&) const — translate the bounds.
	- idBounds::operator+=(const idVec3&) — in‑place translation.
	- idBounds::operator*(const idMat3&) const — return rotated bounds.
	- idBounds::operator*=(const idMat3&) — in‑place rotation.
	- idBounds::operator+(const idBounds&) const — union of two bounds.
	- idBounds::operator+=(const idBounds&) — in‑place union.
	- idBounds::operator-(const idBounds&) const — subtract a constant box.
	- idBounds::operator-=(const idBounds&) — in‑place subtraction.
	- idBounds::Compare(const idBounds&) const — strict equality test.
	- idBounds::Compare(const idBounds&, float epsilon) const — epsilon‑based comparison.
	- idBounds::operator==(const idBounds&) const — alias for Compare.
	- idBounds::operator!=(const idBounds&) const — negated Compare.
	- idBounds::Clear() — set to inside‑out state for incremental accumulation.
	- idBounds::Zero() — set all extents to zero.
	- idBounds::GetCenter() const — midpoint of min and max.
	- idBounds::GetVolume() const — product of side lengths, zero if invalid.
	- idBounds::IsCleared() const — true if inside‑out.
	- idBounds::AddPoint(const idVec3&) — expand to include a point, return if expanded.
	- idBounds::AddBounds(const idBounds&) — expand to also include another bounds, return if expanded.
	- idBounds::Intersect(const idBounds&) const — return overlapping region.
	- idBounds::IntersectSelf(const idBounds&) — modify this bounds to the overlap.
	- idBounds::Expand(float d) const — return bounds expanded by distance.
	- idBounds::ExpandSelf(float d) — in‑place expansion.
	- idBounds::Translate(const idVec3&) const — return translated bounds.
	- idBounds::TranslateSelf(const idVec3&) — in‑place translation.
	- idBounds::Rotate(const idMat3&) const — return rotated bounds.
	- idBounds::RotateSelf(const idMat3&) — in‑place rotation.
	- idBounds::GetSize() const — vector of side lengths.
	- idBounds::IsBackwards() const — detect negative extents.
	- idBounds::ContainsPoint(const idVec3&) const — point containment test.
	- idBounds::IntersectsBounds(const idBounds&) const — overlap test including touch.
	- idBounds::LineIntersection(const idVec3&, const idVec3&) const — segment‑box intersection.
	- idBounds::RayIntersection(const idVec3&, const idVec3&, float&) const — ray‑box intersection with distance.
	- idBounds::FromTransformedBounds(const idBounds&, const idVec3&, const idMat3&) — compute AABB from transformed bounds.
	- idBounds::FromPoints(const idVec3*, int) — build minimal box around an array of points.
	- idBounds::FromPointTranslation(const idVec3&, const idVec3&) — box of a point’s swept volume.
	- idBounds::FromBoundsTranslation(const idBounds&, const idVec3&, const idMat3&, const idVec3&) — apply translation and optional rotation.
	- idBounds::FromPointRotation(const idVec3&, const idRotation&) — compute AABB of rotated point.
	- idBounds::FromBoundsRotation(const idBounds&, const idVec3&, const idMat3&, const idRotation&) — transform and return bounding box.
	- idBounds::ToPoints(idVec3[8]) const — write the 8 corner vertices.
	- idBounds::ToSphere() const — return minimal sphere that fully encloses the box.
	- idBounds::AxisProjection(const idVec3&, float&, float&) const — projection onto an axis.
	- idBounds::AxisProjection(const idVec3&, const idMat3&, const idVec3&, float&, float&) const — projected extent after transformation.
	- idBounds::GetDimension() const — always returns 6.
	- idBounds::ToFloatPtr() const/— — pointer to underlying float array.

	\par Control Flow
	- The idBounds object stores its extents in a fixed two‑element array of idVec3 (`b[2]`).
	- Most operations perform per‑component comparisons or arithmetic on these two vectors to update the box.
	- Methods that return a new bounds create a temporary idBounds, invoke a helper (e.g. `FromTransformedBounds`) or compute the result directly from the two corner vectors.
	- Transform functions (`Rotate`, `RotateSelf`, `FromTransformedBounds`) delegate to a generic routine that applies a matrix and optional translation and then recomputes the min/max corners.
	- Intersection and containment checks are implemented with straightforward per‑axis comparisons, producing boolean results or updated bounds.

	\par Dependencies
	- "idlib/vec3.h" (idVec3 definition)
	- "idlib/mat3.h" (idMat3 definition)
	- "idlib/plane.h" (idPlane definition)
	- "idlib/rotation.h" (idRotation definition)
	- "idlib/sphere.h" (idSphere definition)
	- "idlib/math.h" (idMath constants and functions)
	- "idlib/bv/Bounds.h" depends on the core numeric types and constants: `vec3_origin`, `ON_EPSILON`, `idMath::INFINITUM`.

	\par How It Fits
	- Forms the backbone of bounding‑volume hierarchical structures (BVHs) used for collision detection and visibility.
	- Supports frustum culling, ray‑casting, and light‑shadow computations by offering fast intersection tests.
	- Enables transformation of objects in the world by integrating rotation and translation into the bounding box.
	- Acts as a shared geometric primitive across many subsystems (geometry, physics, AI navigation, rendering).
*/

#ifndef __BV_BOUNDS_H__
#define __BV_BOUNDS_H__

/*
===============================================================================

	Axis Aligned Bounding Box

===============================================================================
*/

class idBounds
{
public:
	/*!
		\brief Default constructor for the idBounds class that initializes an empty bounding box.

		This constructor initializes an idBounds object with default values, creating an empty bounding box with no defined extent. The object is typically used to represent axis-aligned bounding
	   boxes in 3D space. It is marked as ID_INLINE, indicating it should be inlined for performance reasons.

	*/
	idBounds();

	/*!
		\brief Initializes a bounds object with minimum and maximum vector coordinates.

		This constructor sets up a bounding box using two vectors that define the minimum and maximum coordinates of the bounds. The bounds are stored in an array where b[0] represents the minimum
	   coordinates and b[1] represents the maximum coordinates.

		\param mins The vector defining the minimum coordinates of the bounds
		\param maxs The vector defining the maximum coordinates of the bounds
	*/
	explicit idBounds( const idVec3& mins, const idVec3& maxs );

	/*!
		\brief Constructs a bounds object initialized to a single point.

		This constructor initializes a bounds object with both the minimum and maximum points set to the provided point. The resulting bounds has zero volume and represents a single point in 3D space.

		\param point The point to initialize the bounds to
	*/
	explicit idBounds( const idVec3& point );

	const idVec3& operator[]( const int index ) const;
	idVec3&		  operator[]( const int index );
	idBounds	  operator+( const idVec3& t ) const; // returns translated bounds
	idBounds&	  operator+=( const idVec3& t );	  // translate the bounds
	idBounds	  operator*( const idMat3& r ) const; // returns rotated bounds
	idBounds&	  operator*=( const idMat3& r );	  // rotate the bounds
	idBounds	  operator+( const idBounds& a ) const;
	idBounds&	  operator+=( const idBounds& a );
	idBounds	  operator-( const idBounds& a ) const;
	idBounds&	  operator-=( const idBounds& a );

	/*!
		\brief Compares two idBounds objects for exact equality without using epsilon.

		This function performs an exact comparison of two idBounds objects. It checks if both the minimum and maximum corner vectors of the bounds are exactly equal, meaning no floating-point epsilon
	   tolerance is used. This is useful when precise equality checks are required, such as in debug scenarios or when comparing bounds that should be identical.

		\param a The idBounds object to compare against this bounds
		\return true if both the minimum and maximum vectors of the bounds are exactly equal, false otherwise
	*/
	bool		  Compare( const idBounds& a ) const;

	/*!
		\brief Compares this bounding box with another bounding box using the specified epsilon tolerance

		This function performs an element-wise comparison of the two bounding box corners, checking if the minimum and maximum points of both boxes are within the specified epsilon tolerance of each
	   other. The comparison uses the Compare method of the idVec3 class for each corner point.

		\param a The other bounding box to compare against
		\param epsilon The tolerance value for the comparison
		\return True if both corner points of the bounding boxes are within the epsilon tolerance, false otherwise
	*/
	bool		  Compare( const idBounds& a, const float epsilon ) const;
	bool		  operator==( const idBounds& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idBounds& a ) const; // exact compare, no epsilon

	/*!
		\brief Resets the bounds to an inside-out state where minimum values are set to positive infinity and maximum values to negative infinity

		This function initializes the bounds structure to an inside-out state, which is commonly used as a starting point for accumulating bounding boxes. The minimum coordinates are set to positive
	   infinity and maximum coordinates to negative infinity, ensuring any subsequent union operations will properly expand the bounds. This is useful for building up bounds incrementally from a set
	   of points or other bounding volumes.

	*/
	void		  Clear();

	/*!
		\brief Sets all components of the bounding box to zero, creating a single point at the origin.

		This function initializes the bounding box by setting all six boundary values to zero. The result is a degenerate bounding box that represents a single point located at the origin of the
	   coordinate system. This is commonly used to initialize bounds before performing calculations or to reset bounds to a neutral state.

	*/
	void		  Zero();

	//! Returns the center point of the bounding box.
	idVec3		  GetCenter() const;

	//! Returns the radius of the bounding box relative to its origin
	float		  GetRadius() const;

	/*!
		\brief Returns the radius of the bounding box relative to the given center point

		The function calculates the maximum distance from the provided center point to any corner of the bounding box. It computes the squared distance for each axis, taking the larger of the two
	   distances from the center to the respective faces of the bounding box, and then returns the square root of the sum of these squared distances

		\param center The center point relative to which the radius is calculated
		\return The calculated radius as a floating-point value
	*/
	float		  GetRadius( const idVec3& center ) const;

	//! Returns the volume of the bounding box.
	float		  GetVolume() const;

	//! Returns true if the bounds are inside out.
	bool		  IsCleared() const;

	/*!
		\brief Adds a point to the bounding volume and returns true if the bounds expanded.

		This function updates the bounding volume by expanding it to include the given point. It compares each coordinate of the input point with the current bounds and adjusts the bounds accordingly.
	   The function returns true if any of the bounds were expanded to accommodate the new point, or false if the point was already within the existing bounds.

		\param v The point to be added to the bounding volume
		\return True if the bounds were expanded to include the point, false otherwise
	*/
	bool		  AddPoint( const idVec3& v );

	/*!
		\brief Expands the current bounds to include the given bounds and returns whether the bounds were expanded.

		This function takes another bounds object and expands the current bounds to encompass both the current bounds and the provided bounds. It checks each corner of the provided bounds against the
	   current bounds and updates the current bounds if the provided bounds extend beyond the current ones. The function returns true if any of the bounds were expanded, indicating that the provided
	   bounds extended beyond the original bounds.

		\param a The bounds to be added to the current bounds
		\return True if the current bounds were expanded to include the provided bounds, false otherwise
	*/
	bool		  AddBounds( const idBounds& a );

	/*!
		\brief Returns the intersection of this bounds object with another bounds object.

		This function calculates the intersection of the current bounds object with the provided bounds object 'a'. The intersection is computed by taking the maximum of the minimum coordinates and
	   the minimum of the maximum coordinates from both bounds objects. The result is a new bounds object representing the overlapping region between the two bounds.

		\param a The bounds object to intersect with this bounds object
		\return A new bounds object representing the intersection of this bounds object and the provided bounds object 'a'.
	*/
	idBounds	  Intersect( const idBounds& a ) const;

	/*!
		\brief Intersects this bounding box with another bounding box and updates this instance to represent their intersection.

		This function modifies the current bounding box by taking the intersection with the provided bounding box 'a'. It adjusts the minimum and maximum coordinates of the current bounding box to
	   ensure it tightly encloses the overlapping region between the two bounding boxes. The intersection is computed component-wise for each dimension, updating the minimum coordinates if the other
	   box has a larger minimum value, and the maximum coordinates if the other box has a smaller maximum value. The function returns a reference to the modified current bounding box.

		\param a The bounding box to intersect with this bounding box
		\return A reference to the current bounding box after it has been modified to represent the intersection.
	*/
	idBounds&	  IntersectSelf( const idBounds& a );

	//! Returns bounds expanded in all directions by the given distance value
	idBounds	  Expand( const float d ) const;

	/*!
		\brief Expands the bounds by the specified distance in all directions and returns a reference to the modified bounds object.

		This function modifies the current bounds object by expanding it uniformly in all three dimensions. The minimum coordinates are decreased by the given distance, while the maximum coordinates
	   are increased by the same distance. The function returns a reference to the modified bounds object, allowing for method chaining.

		\param d The distance by which to expand the bounds in all directions
		\return A reference to the modified bounds object after expansion
	*/
	idBounds&	  ExpandSelf( const float d );

	//! Returns a new bounds object translated by the specified vector.
	idBounds	  Translate( const idVec3& translation ) const;

	/*!
		\brief Translates the bounds by the specified translation vector and returns a reference to the modified bounds.

		This function modifies the current bounds object by translating both the minimum and maximum points of the bounding box by the given translation vector. The translation is applied in-place,
	   and the function returns a reference to the modified bounds object to allow for method chaining.

		\param translation The vector by which to translate the bounds
		\return A reference to the modified bounds object after translation
	*/
	idBounds&	  TranslateSelf( const idVec3& translation );

	/*!
		\brief Returns a new bounds object that represents the current bounds rotated by the specified rotation matrix.

		This function creates a new bounds object by rotating the current bounds using the provided rotation matrix. It utilizes the FromTransformedBounds method to perform the transformation, passing
	   the current bounds, a translation vector of vec3_origin, and the specified rotation matrix. The result is a new bounds object that encompasses the rotated version of the original bounds.

		\param rotation The rotation matrix to apply to the bounds
		\return A new idBounds object representing the rotated bounds
	*/
	idBounds	  Rotate( const idMat3& rotation ) const;

	/*!
		\brief Rotates the bounds by the provided rotation matrix and returns a reference to itself.

		This function applies a rotation transformation to the current bounds object using the specified rotation matrix. It modifies the bounds in-place by transforming it according to the given
	   rotation. The function uses the FromTransformedBounds method to perform the actual transformation, passing the current bounds, a zero vector as the translation, and the provided rotation
	   matrix. The result is a rotated bounds object that represents the same spatial region but oriented according to the rotation matrix.

		\param rotation The rotation matrix to apply to the bounds
		\return A reference to the modified bounds object after rotation
	*/
	idBounds&	  RotateSelf( const idMat3& rotation );

	/*!
		\brief Computes the minimum distance from the bounding box to a plane, considering the box's orientation and size

		This function calculates the signed distance from the bounding box to the specified plane. The calculation accounts for the orientation and dimensions of the bounding box to determine whether
	   the box is in front of, behind, or intersects the plane. The result is a conservative estimate of the closest approach of the box to the plane.

		\param plane The plane to which the distance is calculated
		\return The minimum distance from the bounding box to the plane, which can be positive (box in front of plane), negative (box behind plane), or zero (box intersects plane)
	*/
	float		  PlaneDistance( const idPlane& plane ) const;

	/*!
		\brief Determines which side of a plane the bounding box resides on, using an epsilon value for floating-point comparison

		This function evaluates the spatial relationship between a bounding box and a plane by calculating the distance from the box center to the plane and comparing it with the maximum possible
	   distance due to the box dimensions. It returns one of three values indicating whether the box is entirely in front of the plane, entirely behind it, or if it crosses the plane. The epsilon
	   parameter controls the tolerance for determining if the box is close enough to the plane to be considered as crossing it.

		\param plane The plane to test against
		\param epsilon The tolerance value for determining if the box is close to the plane
		\return An integer indicating the side of the plane the bounding box is on: PLANESIDE_FRONT if entirely in front, PLANESIDE_BACK if entirely behind, or PLANESIDE_CROSS if spanning the plane
	*/
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	//! Returns the size of the bounding box as a vector.
	idVec3		  GetSize() const;

	//! Checks if any dimension of the bounding box has a negative size.
	bool		  IsBackwards() const;

	/*!
		\brief Checks if a point is contained within the bounding box.

		This function determines whether a given 3D point lies within the bounds of the object. The bounds are defined by two corner points, b[0] and b[1], which represent the minimum and maximum
	   coordinates of the bounding box. The function returns true if the point is inside or on the boundary of the box, and false otherwise. The check includes points that touch the boundary of the
	   box.

		\param p The 3D point to check for containment within the bounding box
		\return True if the point is within or on the boundary of the bounding box, false otherwise
	*/
	bool		  ContainsPoint( const idVec3& p ) const;

	/*!
		\brief Tests if this bounding box intersects with another bounding box, including cases where they touch.

		This function checks whether the current bounding box intersects with the provided bounding box 'a'. The intersection test includes cases where the boxes are touching but not overlapping. The
	   function returns false if any of the bounding box dimensions do not overlap, and true otherwise. This is commonly used in collision detection to quickly determine if two objects might be
	   colliding.

		\param a The other bounding box to check for intersection with this one
		\return True if the bounding boxes intersect or touch, false otherwise
	*/
	bool		  IntersectsBounds( const idBounds& a ) const;

	/*!
		\brief Checks if a line segment intersects with the bounding box.

		This function determines whether a line segment defined by two points intersects with the bounding box represented by the current object. It uses an algorithm based on the separating axis
	   theorem to test for intersection. The line segment is defined by the start and end points provided as parameters. The function returns true if the line segment intersects the bounding box, and
	   false otherwise.

		\param start The starting point of the line segment
		\param end The ending point of the line segment
		\return True if the line segment intersects the bounding box, false otherwise
	*/
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	/*!
		\brief Checks if a ray intersects with the bounding box and calculates the intersection scale factor.

		This function determines whether a ray starting at the specified start point and extending in the given direction intersects with the bounding box. If an intersection occurs, it calculates the
	   scale factor such that the intersection point can be found by moving along the ray from the start point by that scale factor. The function returns true if there is an intersection, and false
	   otherwise. The scale factor is only meaningful when the function returns true.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale The scale factor for the ray intersection, updated on return
		\return True if the ray intersects with the bounding box, false otherwise
	*/
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale ) const;

	/*!
		\brief Calculates the axis-aligned bounding box that encloses the input bounds after applying the specified rotation and translation.

		The function computes the new axis-aligned extents by projecting the rotated extents onto the coordinate axes and determines the new center by applying the rotation and translation to the
	   original center.

		\param bounds The source axis-aligned bounding box.
		\param origin The translation vector applied to the transformed center.
		\param axis The rotation matrix used to rotate the bounds.
	*/
	void		  FromTransformedBounds( const idBounds& bounds, const idVec3& origin, const idMat3& axis );

	/*!
		\brief Computes the tightest bounding box that contains all the given points.

		This function calculates the minimum and maximum coordinates across all dimensions for the provided set of points. The resulting bounds represent the smallest axis-aligned box that can contain
	   all input points. The calculation is performed using SIMD optimization for improved performance.

		\param points Array of points to compute the bounding box for
		\param numPoints Number of points in the array
	*/
	void		  FromPoints( const idVec3* points, const int numPoints );

	/*!
		\brief Calculates the tightest bounding box that contains both the initial point and the translated point.

		This function computes a bounding box that tightly encloses the space swept by a point moving according to a translation vector. For each axis, it determines the minimum and maximum
	   coordinates of the point's initial position and its final position after the translation. If the translation is in the negative direction along an axis, the starting point is the maximum,
	   otherwise the starting point is the minimum.

		\param point The initial position of the point
		\param translation The translation vector applied to the point
	*/
	void		  FromPointTranslation( const idVec3& point, const idVec3& translation );

	/*!
		\brief Computes a transformed bounding box taking into account origin, axis rotation, and translation.

		The function calculates a new bounding box by applying a transformation to the input bounds. If the axis is rotated, it uses a transformed bounds calculation. Otherwise, it directly applies
	   the origin translation to the bounds. Then, it adjusts the bounds based on the translation vector, ensuring that negative translation values affect the minimum corner and positive values affect
	   the maximum corner of the bounding box.

		\param bounds The input bounding box to be transformed
		\param origin The origin point to translate the bounds
		\param axis The rotation axis to potentially rotate the bounds
		\param translation The translation vector to adjust the bounds after origin and axis transformations
	*/
	void		  FromBoundsTranslation( const idBounds& bounds, const idVec3& origin, const idMat3& axis, const idVec3& translation );

	/*!
		\brief Computes the tightest bounding box for a point transformed by a rotation.

		This function calculates the bounding box that tightly encloses the result of applying a rotation to a given point. For small rotations (less than 180 degrees), it uses a specialized bounds
	   calculation. For larger rotations, it computes the bounds based on the distance from the rotation origin to the point, creating a cubic bounds centered at the origin with sides twice the
	   radius.

		\param point The point to be transformed by the rotation
		\param rotation The rotation to be applied to the point
	*/
	void		  FromPointRotation( const idVec3& point, const idRotation& rotation );

	/*!
		\brief Computes the bounding box for a rotated and translated bounds object.

		This function calculates the new bounding box that encloses the transformed bounds. It handles two cases: when the rotation angle is less than 180 degrees, it computes the bounds by
	   transforming each of the 8 corners of the input bounds and then finding the enclosing bounds. For angles of 180 degrees or more, it computes an approximate bounding box based on the center
	   point and rotation origin.

		\param bounds The input bounds to be transformed
		\param origin The translation vector to apply to the bounds
		\param axis The transformation matrix to apply to the bounds
		\param rotation The rotation to apply to the bounds
	*/
	void		  FromBoundsRotation( const idBounds& bounds, const idVec3& origin, const idMat3& axis, const idRotation& rotation );

	/*!
		\brief Computes the 8 corner points of the bounding box and stores them in the provided array.

		This function calculates the 8 corner points of the bounding box represented by the current object. Each corner point is derived from the minimum and maximum values of the bounding box along
	   each axis. The calculation uses bitwise operations to determine which min/max value to use for each component of the corner points.

		\param points Output array that will hold the 8 corner points of the bounding box
	*/
	void		  ToPoints( idVec3 points[8] ) const;

	//! Returns a sphere that contains the bounds.
	idSphere	  ToSphere() const;

	/*!
		\brief Computes the minimum and maximum projections of the bounds along a specified direction vector

		This function calculates the range of projections of the bounding box along a given direction vector. It uses the center and extents of the bounds to compute the projection without requiring a
	   transformation matrix. The calculation is done by projecting the center of the bounds along the direction vector and adding the sum of absolute projections of the extents along each axis.

		\param dir The direction vector along which to project the bounds
		\param min Output parameter for the minimum projection value
		\param max Output parameter for the maximum projection value
	*/
	void		  AxisProjection( const idVec3& dir, float& min, float& max ) const;

	/*!
		\brief Computes the minimum and maximum projections of the bounds along a specified direction after transformation by an origin and axis.

		This function calculates the projection of the bounds onto a line defined by the direction vector dir. The projection is computed after transforming the bounds by the given origin and axis.
	   The result is stored in the min and max parameters, representing the range of the projection.

		\param origin The origin point for the transformation
		\param axis The transformation matrix to apply to the bounds
		\param dir The direction vector along which to project the bounds
		\param min Output parameter for the minimum projection value
		\param max Output parameter for the maximum projection value
	*/
	void		  AxisProjection( const idVec3& origin, const idMat3& axis, const idVec3& dir, float& min, float& max ) const;

	//! Returns the fixed dimension value of 6 for the bounding box.
	int			  GetDimension() const;

	//! Returns a pointer to the first element of the bounds data as a float array.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the float representation of the bounds.
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
