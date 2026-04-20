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

#ifndef __MATH_PLUECKER_H__
#define __MATH_PLUECKER_H__

/*
===============================================================================

	Pluecker coordinate

===============================================================================
*/

class idPluecker
{
public:
	/*!
		\brief Initializes an empty Pluecker coordinate.

		This constructor initializes a Pluecker coordinate with default values, representing an empty or uninitialized coordinate in projective geometry.

	*/
	idPluecker();

	/*!
		\brief Constructs a Pluecker coordinate from a float array.

		The function initializes a Pluecker coordinate using the provided float array. The array is expected to contain exactly 6 float values that represent the Pluecker coordinates. The function
	   copies these values directly into the internal storage of the Pluecker coordinate object.

		\param a Pointer to a float array containing 6 values for the Pluecker coordinate
	*/
	explicit idPluecker( const float* a );

	/*!
		\brief Constructs a Pluecker coordinate from a line defined by a start and end point.

		This constructor initializes a Pluecker coordinate using the provided start and end points of a line. The Pluecker coordinate is a mathematical representation used in computational geometry to
	   describe lines in 3D space. The constructor internally calls the FromLine method to compute the Pluecker coordinates from the given line segment.

		\param start The starting point of the line segment
		\param end The ending point of the line segment
	*/
	explicit idPluecker( const idVec3& start, const idVec3& end );

	/*!
		\brief Constructs an idPluecker object with the specified six float values.

		This constructor initializes a Pluecker coordinate object using six float values. Pluecker coordinates are used to represent lines in 3D space, typically used in computer graphics and
	   geometric computations. The six values correspond to the components of the Pluecker coordinate representation.

		\param a1 First component of the Pluecker coordinate
		\param a2 Second component of the Pluecker coordinate
		\param a3 Third component of the Pluecker coordinate
		\param a4 Fourth component of the Pluecker coordinate
		\param a5 Fifth component of the Pluecker coordinate
		\param a6 Sixth component of the Pluecker coordinate
	*/
	explicit idPluecker( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );

	float		 operator[]( const int index ) const;
	float&		 operator[]( const int index );
	idPluecker	 operator-() const; // flips the direction
	idPluecker	 operator*( const float a ) const;
	idPluecker	 operator/( const float a ) const;
	float		 operator*( const idPluecker& a ) const; // permuted inner product
	idPluecker	 operator-( const idPluecker& a ) const;
	idPluecker	 operator+( const idPluecker& a ) const;
	idPluecker&	 operator*=( const float a );
	idPluecker&	 operator/=( const float a );
	idPluecker&	 operator+=( const idPluecker& a );
	idPluecker&	 operator-=( const idPluecker& a );

	/*!
		\brief Compares this Pluecker coordinate with another for exact equality.

		This function performs an exact comparison between the Pluecker coordinate represented by this object and another Pluecker coordinate provided as a parameter. It checks if all six components
	   of the two Pluecker coordinates are exactly equal. The comparison is done using the equality operator for each component.

		\param a The Pluecker coordinate to compare with this object
		\return True if all six components of this Pluecker coordinate are exactly equal to the corresponding components of the provided Pluecker coordinate, false otherwise
	*/
	bool		 Compare( const idPluecker& a ) const;

	/*!
		\brief Compares this Pluecker coordinate with another Pluecker coordinate within the given epsilon tolerance.

		This function performs an element-wise comparison between the Pluecker coordinates of the current object and the provided Pluecker coordinate. It checks if the absolute difference between each
	   corresponding component is within the specified epsilon tolerance. If any component differs by more than epsilon, the function returns false. Otherwise, it returns true, indicating that the two
	   Pluecker coordinates are considered equal within the given tolerance.

		\param a The Pluecker coordinate to compare against
		\param epsilon The tolerance value for the comparison
		\return true if all components of the Pluecker coordinates are within the epsilon tolerance of each other, false otherwise
	*/
	bool		 Compare( const idPluecker& a, const float epsilon ) const;
	bool		 operator==( const idPluecker& a ) const; // exact compare, no epsilon
	bool		 operator!=( const idPluecker& a ) const; // exact compare, no epsilon

	/*!
		\brief Sets the six components of the Pluecker coordinate to the specified values

		This function assigns the six provided float values to the internal components of the Pluecker coordinate. The Pluecker coordinate is a mathematical construct used in computational geometry,
	   typically representing a line in 3D space. Each component corresponds to a specific element of the coordinate system.

		\param a1 First component value
		\param a2 Second component value
		\param a3 Third component value
		\param a4 Fourth component value
		\param a5 Fifth component value
		\param a6 Sixth component value
	*/
	void		 Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );

	//! Sets all components of the Pluecker coordinate to zero.
	void		 Zero();

	/*!
		\brief Computes Pluecker coordinates from a line defined by two points.

		This function calculates the Pluecker coordinates for a line specified by a starting point and end point. Pluecker coordinates are a way to represent lines in 3D space using six components.
	   The function uses the mathematical relationship between the line's start and end points to compute these coordinates. The implementation follows the standard Pluecker coordinate formula where
	   the first three components represent the cross product of the start point with the direction vector, and the last three components represent the direction vector itself with appropriate sign
	   changes.

		\param start The starting point of the line in 3D space
		\param end The ending point of the line in 3D space
	*/
	void		 FromLine( const idVec3& start, const idVec3& end );

	/*!
		\brief Computes the Pluecker coordinates from a ray defined by a starting point and direction.

		This function calculates the Pluecker coordinates for a ray specified by a starting point and direction vector. The Pluecker coordinates are a way to represent lines in 3D space using six
	   components. The function uses the mathematical relationship between the ray's start point and direction to compute these coordinates. The implementation follows the standard Pluecker coordinate
	   formula where the first three components represent the cross product of the start point with the direction vector, and the last three components represent the direction vector itself with
	   appropriate sign changes.

		\param start The starting point of the ray in 3D space
		\param dir The direction vector of the ray
	*/
	void		 FromRay( const idVec3& start, const idVec3& dir );

	/*!
		\brief Initializes a Pluecker coordinate from the intersection of two planes

		This function computes the Pluecker coordinates representing the line formed by the intersection of two planes. The Pluecker coordinates are calculated using the plane coefficients, where each
	   coordinate is derived from the cross product of the plane normal vectors and their distance components. The function returns true if the resulting line is valid, which occurs when at least one
	   of the coordinates p[2], p[4], or p[5] is non-zero, indicating that the planes are not parallel.

		\param p1 First plane used to calculate the intersection line
		\param p2 Second plane used to calculate the intersection line
		\return True if the intersection line is valid, false if the planes are parallel and do not intersect
	*/
	bool		 FromPlanes( const idPlane& p1, const idPlane& p2 );

	/*!
		\brief Converts a Pluecker coordinate to a line defined by start and end points

		This function transforms a Pluecker coordinate representation into a line segment defined by a start point and an end point. The Pluecker coordinate is typically used to represent lines in 3D
	   space. The function calculates the start point as the cross product of two direction vectors divided by the squared length of one of the vectors. The end point is calculated as the start point
	   plus the direction vector. If the squared length of the direction vector is zero, the function returns false indicating that the Pluecker coordinate does not represent a valid line

		\param start Output parameter for the start point of the line
		\param end Output parameter for the end point of the line
		\return True if the conversion was successful and the Pluecker coordinate represents a valid line, false otherwise
	*/
	bool		 ToLine( idVec3& start, idVec3& end ) const;

	/*!
		\brief Converts a Pluecker coordinate to a ray representation with a start point and direction vector.

		This function takes a Pluecker coordinate and converts it into a ray representation consisting of a start point and a direction vector. The conversion involves computing intermediate vectors
	   and performing cross products to derive the ray parameters. The function returns false if the Pluecker coordinate does not represent a valid line, indicated by a zero dot product of the
	   direction vector with itself.

		\param start Output parameter for the start point of the ray.
		\param dir Output parameter for the direction vector of the ray.
		\return True if the conversion was successful and the Pluecker coordinate represents a valid line, false otherwise.
	*/
	bool		 ToRay( idVec3& start, idVec3& dir ) const;

	/*!
		\brief Converts the Pluecker coordinate to a direction vector by computing a permuted inner product.

		The function transforms the Pluecker coordinate representation into a direction vector. It computes a permuted inner product of the Pluecker coordinate with another Pluecker coordinate. The
	   resulting direction vector components are derived from specific indexed elements of the Pluecker coordinate.

		\param dir Output direction vector that will be set to the computed direction.
	*/
	void		 ToDir( idVec3& dir ) const;

	/*!
		\brief Computes the permuted inner product of this Pluecker coordinate with another Pluecker coordinate.

		The permuted inner product is a specialized calculation used in computational geometry, particularly in collision detection and 3D rendering. It combines the coordinates of two Pluecker
	   objects in a specific pattern to produce a scalar result. This operation is commonly used to determine spatial relationships between lines and planes in 3D space.

		\param a Another Pluecker coordinate to compute the permuted inner product with
		\return The scalar result of the permuted inner product calculation
	*/
	float		 PermutedInnerProduct( const idPluecker& a ) const;

	/*!
		\brief Computes the squared 3D distance between this Pluecker coordinate and another Pluecker coordinate.

		This function calculates the squared 3D distance between two Pluecker coordinates. It computes the direction vector between the lines represented by the Pluecker coordinates, and then
	   calculates the distance squared. Special handling is included for parallel lines, which return -1.0. The function uses a permutation of the inner product to compute the result.

		\param a Another Pluecker coordinate to compute the distance to
		\return The squared 3D distance between this Pluecker coordinate and the provided Pluecker coordinate, or -1.0 if the lines are parallel
	*/
	float		 Distance3DSqr( const idPluecker& a ) const;

	/*!
		\brief Returns the length of the Pluecker coordinate vector.

		This function calculates and returns the Euclidean length of the Pluecker coordinate vector represented by the object. It uses the square root of the sum of squares of three components of the
	   Pluecker vector.

		\return The length of the Pluecker coordinate vector as a floating-point value.
	*/
	float		 Length() const;

	/*!
		\brief Returns the squared length of the Pluecker coordinate.

		This function computes the squared length of a Pluecker coordinate by summing the squares of three specific components of the coordinate. It is a performance-optimized version that avoids
	   computing the square root, making it suitable for comparisons and distance calculations where the actual length is not needed.

		\return The squared length of the Pluecker coordinate.
	*/
	float		 LengthSqr() const;

	/*!
		\brief Returns a normalized copy of this Pluecker coordinate.

		This function normalizes the Pluecker coordinate by calculating its length squared and then using the inverse square root to scale all components. If the length squared is zero, the function
	   returns the original coordinate unchanged, as it does not represent a valid line. The normalization ensures that the Pluecker coordinate has a unit length, which is useful for geometric
	   calculations and comparisons.

		\return A new Pluecker coordinate that is normalized
	*/
	idPluecker	 Normalize() const;

	/*!
		\brief Normalizes the Pluecker coordinate in place and returns the scaling factor used.

		This function normalizes the Pluecker coordinate by scaling all components by the inverse square root of the squared length. If the coordinate represents a zero vector, it returns zero without
	   modification. The function returns the product of the scaling factor and the original squared length.

		\return The scaling factor used to normalize the Pluecker coordinate
	*/
	float		 NormalizeSelf();

	//! Returns the dimension of the Pluecker coordinate, which is always 6.
	int			 GetDimension() const;

	//! Returns a pointer to the internal float array representation of the Pluecker coordinate.
	const float* ToFloatPtr() const;

	//! Returns a pointer to the internal float array of the idPluecker object.
	float*		 ToFloatPtr();

	//! Returns a string representation of the Pluecker coordinate with the specified precision
	const char*	 ToString( int precision = 2 ) const;

private:
	float p[6];
};

extern idPluecker pluecker_origin;
#define pluecker_zero pluecker_origin

ID_INLINE idPluecker::idPluecker()
{
}

ID_INLINE idPluecker::idPluecker( const float* a )
{
	memcpy( p, a, 6 * sizeof( float ) );
}

ID_INLINE idPluecker::idPluecker( const idVec3& start, const idVec3& end )
{
	FromLine( start, end );
}

ID_INLINE idPluecker::idPluecker( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE idPluecker idPluecker::operator-() const
{
	return idPluecker( -p[0], -p[1], -p[2], -p[3], -p[4], -p[5] );
}

ID_INLINE float idPluecker::operator[]( const int index ) const
{
	return p[index];
}

ID_INLINE float& idPluecker::operator[]( const int index )
{
	return p[index];
}

ID_INLINE idPluecker idPluecker::operator*( const float a ) const
{
	return idPluecker( p[0] * a, p[1] * a, p[2] * a, p[3] * a, p[4] * a, p[5] * a );
}

ID_INLINE float idPluecker::operator*( const idPluecker& a ) const
{
	return p[0] * a.p[4] + p[1] * a.p[5] + p[2] * a.p[3] + p[4] * a.p[0] + p[5] * a.p[1] + p[3] * a.p[2];
}

ID_INLINE idPluecker idPluecker::operator/( const float a ) const
{
	float inva;

	assert( a != 0.0f );
	inva = 1.0f / a;
	return idPluecker( p[0] * inva, p[1] * inva, p[2] * inva, p[3] * inva, p[4] * inva, p[5] * inva );
}

ID_INLINE idPluecker idPluecker::operator+( const idPluecker& a ) const
{
	return idPluecker( p[0] + a[0], p[1] + a[1], p[2] + a[2], p[3] + a[3], p[4] + a[4], p[5] + a[5] );
}

ID_INLINE idPluecker idPluecker::operator-( const idPluecker& a ) const
{
	return idPluecker( p[0] - a[0], p[1] - a[1], p[2] - a[2], p[3] - a[3], p[4] - a[4], p[5] - a[5] );
}

ID_INLINE idPluecker& idPluecker::operator*=( const float a )
{
	p[0] *= a;
	p[1] *= a;
	p[2] *= a;
	p[3] *= a;
	p[4] *= a;
	p[5] *= a;
	return *this;
}

ID_INLINE idPluecker& idPluecker::operator/=( const float a )
{
	float inva;

	assert( a != 0.0f );
	inva = 1.0f / a;
	p[0] *= inva;
	p[1] *= inva;
	p[2] *= inva;
	p[3] *= inva;
	p[4] *= inva;
	p[5] *= inva;
	return *this;
}

ID_INLINE idPluecker& idPluecker::operator+=( const idPluecker& a )
{
	p[0] += a[0];
	p[1] += a[1];
	p[2] += a[2];
	p[3] += a[3];
	p[4] += a[4];
	p[5] += a[5];
	return *this;
}

ID_INLINE idPluecker& idPluecker::operator-=( const idPluecker& a )
{
	p[0] -= a[0];
	p[1] -= a[1];
	p[2] -= a[2];
	p[3] -= a[3];
	p[4] -= a[4];
	p[5] -= a[5];
	return *this;
}

ID_INLINE bool idPluecker::Compare( const idPluecker& a ) const
{
	return ( ( p[0] == a[0] ) && ( p[1] == a[1] ) && ( p[2] == a[2] ) && ( p[3] == a[3] ) && ( p[4] == a[4] ) && ( p[5] == a[5] ) );
}

ID_INLINE bool idPluecker::Compare( const idPluecker& a, const float epsilon ) const
{
	if( idMath::Fabs( p[0] - a[0] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[1] - a[1] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[2] - a[2] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[3] - a[3] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[4] - a[4] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[5] - a[5] ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idPluecker::operator==( const idPluecker& a ) const
{
	return Compare( a );
}

ID_INLINE bool idPluecker::operator!=( const idPluecker& a ) const
{
	return !Compare( a );
}

ID_INLINE void idPluecker::Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE void idPluecker::Zero()
{
	p[0] = p[1] = p[2] = p[3] = p[4] = p[5] = 0.0f;
}

ID_INLINE void idPluecker::FromLine( const idVec3& start, const idVec3& end )
{
	p[0] = start[0] * end[1] - end[0] * start[1];
	p[1] = start[0] * end[2] - end[0] * start[2];
	p[2] = start[0] - end[0];
	p[3] = start[1] * end[2] - end[1] * start[2];
	p[4] = start[2] - end[2];
	p[5] = end[1] - start[1];
}

ID_INLINE void idPluecker::FromRay( const idVec3& start, const idVec3& dir )
{
	p[0] = start[0] * dir[1] - dir[0] * start[1];
	p[1] = start[0] * dir[2] - dir[0] * start[2];
	p[2] = -dir[0];
	p[3] = start[1] * dir[2] - dir[1] * start[2];
	p[4] = -dir[2];
	p[5] = dir[1];
}

ID_INLINE bool idPluecker::ToLine( idVec3& start, idVec3& end ) const
{
	idVec3 dir1, dir2;
	float  d;

	dir1[0] = p[3];
	dir1[1] = -p[1];
	dir1[2] = p[0];

	dir2[0] = -p[2];
	dir2[1] = p[5];
	dir2[2] = -p[4];

	d = dir2 * dir2;
	if( d == 0.0f ) {
		return false; // pluecker coordinate does not represent a line
	}

	start = dir2.Cross( dir1 ) * ( 1.0f / d );
	end	  = start + dir2;
	return true;
}

ID_INLINE bool idPluecker::ToRay( idVec3& start, idVec3& dir ) const
{
	idVec3 dir1;
	float  d;

	dir1[0] = p[3];
	dir1[1] = -p[1];
	dir1[2] = p[0];

	dir[0] = -p[2];
	dir[1] = p[5];
	dir[2] = -p[4];

	d = dir * dir;
	if( d == 0.0f ) {
		return false; // pluecker coordinate does not represent a line
	}

	start = dir.Cross( dir1 ) * ( 1.0f / d );
	return true;
}

ID_INLINE void idPluecker::ToDir( idVec3& dir ) const
{
	dir[0] = -p[2];
	dir[1] = p[5];
	dir[2] = -p[4];
}

ID_INLINE float idPluecker::PermutedInnerProduct( const idPluecker& a ) const
{
	return p[0] * a.p[4] + p[1] * a.p[5] + p[2] * a.p[3] + p[4] * a.p[0] + p[5] * a.p[1] + p[3] * a.p[2];
}

ID_INLINE float idPluecker::Length() const
{
	return ( float )idMath::Sqrt( p[5] * p[5] + p[4] * p[4] + p[2] * p[2] );
}

ID_INLINE float idPluecker::LengthSqr() const
{
	return ( p[5] * p[5] + p[4] * p[4] + p[2] * p[2] );
}

ID_INLINE float idPluecker::NormalizeSelf()
{
	float l, d;

	l = LengthSqr();
	if( l == 0.0f ) {
		return l; // pluecker coordinate does not represent a line
	}
	d = idMath::InvSqrt( l );
	p[0] *= d;
	p[1] *= d;
	p[2] *= d;
	p[3] *= d;
	p[4] *= d;
	p[5] *= d;
	return d * l;
}

ID_INLINE idPluecker idPluecker::Normalize() const
{
	float d;

	d = LengthSqr();
	if( d == 0.0f ) {
		return *this; // pluecker coordinate does not represent a line
	}
	d = idMath::InvSqrt( d );
	return idPluecker( p[0] * d, p[1] * d, p[2] * d, p[3] * d, p[4] * d, p[5] * d );
}

ID_INLINE int idPluecker::GetDimension() const
{
	return 6;
}

ID_INLINE const float* idPluecker::ToFloatPtr() const
{
	return p;
}

ID_INLINE float* idPluecker::ToFloatPtr()
{
	return p;
}

#endif /* !__MATH_PLUECKER_H__ */
