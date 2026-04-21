/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2020 Robert Beckebans

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

#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

/*
===============================================================================

  Vector classes

===============================================================================
*/

#include "../containers/Array.h" // for idTupleSize

#define VECTOR_EPSILON 0.001f

class idAngles;
class idPolar3;
class idMat3;

/*!
	\class idVec2
	\brief A 2D vector class for representing and manipulating two-dimensional mathematical vectors within the engine.

	The idVec2 class provides a comprehensive set of operations for 2D vector mathematics, including initialization, component access, arithmetic operations, normalization, and utility functions. It
   is designed to support efficient vector calculations commonly used in graphics, physics, and game logic within the engine. The class provides both precise and fast approximation methods for length
   calculations, as well as functions for scaling, clamping, and interpolation. The implementation follows standard vector mathematics conventions and integrates well with other mathematical types in
   the engine's math library.

*/
class idVec2
{
public:
	float x;
	float y;

	//! Initializes an idVec2 object with default values.
	idVec2();

	//! Initializes a new idVec2 instance with the specified x and y coordinates.
	explicit idVec2( const float x, const float y );

	//! Sets the x and y components of the vector to the specified values.
	void		  Set( const float x, const float y );

	//! Sets both components of the vector to zero.
	void		  Zero();

	float		  operator[]( int index ) const;
	float&		  operator[]( int index );
	idVec2		  operator-() const;
	float		  operator*( const idVec2& a ) const;
	idVec2		  operator*( const float a ) const;
	idVec2		  operator/( const float a ) const;
	idVec2		  operator/( const idVec2& a ) const;
	idVec2		  operator+( const idVec2& a ) const;
	idVec2		  operator-( const idVec2& a ) const;
	idVec2&		  operator+=( const idVec2& a );
	idVec2&		  operator-=( const idVec2& a );
	idVec2&		  operator/=( const idVec2& a );
	idVec2&		  operator/=( const float a );
	idVec2&		  operator*=( const float a );

	friend idVec2 operator*( const float a, const idVec2 b );

	//! Returns a new vector with components scaled by the corresponding components of the input vector.
	idVec2		  Scale( const idVec2& a ) const;

	//! Compares two idVec2 objects for exact equality without epsilon.
	bool		  Compare( const idVec2& a ) const;

	//! Compares this vector with another vector using the specified epsilon threshold.
	bool		  Compare( const idVec2& a, const float epsilon ) const;
	bool		  operator==( const idVec2& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idVec2& a ) const; // exact compare, no epsilon

	//! Returns the Euclidean length of the vector.
	float		  Length() const;

	//! Computes an fast approximation of the vector's length using inverse square root.
	float		  LengthFast() const;

	//! Returns the squared length of this 2D vector.
	float		  LengthSqr() const;

	//! Normalizes the vector and returns the length of the original vector.
	float		  Normalize();

	//! Normalizes the vector and returns its length.
	float		  NormalizeFast();

	//! Truncates the vector to the specified length.
	idVec2		  Truncate( float length ) const;

	//! Clamps the vector components between the given minimum and maximum values.
	void		  Clamp( const idVec2& min, const idVec2& max );

	//! Rounds the x and y components to the nearest integer values.
	void		  Snap();

	//! Rounds the x and y components of the vector towards the nearest integer values.
	void		  SnapInt();

	//! Returns the dimension of the vector, which is 2.
	int			  GetDimension() const;

	//! Returns a pointer to the float representation of this vector.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the float representation of this idVec2 object
	float*		  ToFloatPtr();

	//! Returns a string representation of the vector with the specified precision.
	const char*	  ToString( int precision = 2 ) const;

	//! Computes a linear interpolation between two 2D vectors based on a scalar factor
	void		  Lerp( const idVec2& v1, const idVec2& v2, const float l );

	//! Multiplies the vector components with the corresponding components of another vector.
	void		  MulCW( const idVec2& a );
};

extern idVec2 vec2_origin;
#define vec2_zero vec2_origin
extern idVec2 vec2_one;

ID_INLINE	  idVec2::idVec2()
{
}

ID_INLINE idVec2::idVec2( const float x, const float y )
{
	this->x = x;
	this->y = y;
}

ID_INLINE void idVec2::Set( const float x, const float y )
{
	this->x = x;
	this->y = y;
}

ID_INLINE void idVec2::Zero()
{
	x = y = 0.0f;
}

ID_INLINE bool idVec2::Compare( const idVec2& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) );
}

ID_INLINE bool idVec2::Compare( const idVec2& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon ) { return false; }

	if( idMath::Fabs( y - a.y ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idVec2::operator==( const idVec2& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec2::operator!=( const idVec2& a ) const
{
	return !Compare( a );
}

ID_INLINE float idVec2::operator[]( int index ) const
{
	return ( &x )[index];
}

ID_INLINE float& idVec2::operator[]( int index )
{
	return ( &x )[index];
}

ID_INLINE float idVec2::Length() const
{
	return ( float )idMath::Sqrt( x * x + y * y );
}

ID_INLINE float idVec2::LengthFast() const
{
	float sqrLength;

	sqrLength = x * x + y * y;
	return sqrLength * idMath::InvSqrt( sqrLength );
}

ID_INLINE float idVec2::LengthSqr() const
{
	return ( x * x + y * y );
}

ID_INLINE float idVec2::Normalize()
{
	float sqrLength, invLength;

	sqrLength = x * x + y * y;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	return invLength * sqrLength;
}

ID_INLINE float idVec2::NormalizeFast()
{
	float lengthSqr, invLength;

	lengthSqr = x * x + y * y;
	invLength = idMath::InvSqrt( lengthSqr );
	x *= invLength;
	y *= invLength;
	return invLength * lengthSqr;
}

ID_INLINE idVec2 idVec2::Truncate( float length ) const
{
	if( length < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		return vec2_zero;
	} else {
		float length2 = LengthSqr();
		if( length2 > length * length ) {
			float ilength = length * idMath::InvSqrt( length2 );
			return *this * ilength;
		}
	}
	return *this;
}

ID_INLINE void idVec2::Clamp( const idVec2& min, const idVec2& max )
{
	if( x < min.x ) {
		x = min.x;
	} else if( x > max.x ) {
		x = max.x;
	}
	if( y < min.y ) {
		y = min.y;
	} else if( y > max.y ) {
		y = max.y;
	}
}

ID_INLINE void idVec2::Snap()
{
	x = floorf( x + 0.5f );
	y = floorf( y + 0.5f );
}

ID_INLINE void idVec2::SnapInt()
{
	x = float( int( x ) );
	y = float( int( y ) );
}

ID_INLINE idVec2 idVec2::operator-() const
{
	return idVec2( -x, -y );
}

ID_INLINE idVec2 idVec2::operator-( const idVec2& a ) const
{
	return idVec2( x - a.x, y - a.y );
}

ID_INLINE float idVec2::operator*( const idVec2& a ) const
{
	return x * a.x + y * a.y;
}

ID_INLINE idVec2 idVec2::operator*( const float a ) const
{
	return idVec2( x * a, y * a );
}

ID_INLINE idVec2 idVec2::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idVec2( x * inva, y * inva );
}

ID_INLINE idVec2 idVec2::operator/( const idVec2& a ) const
{
	return idVec2( x / a.x, y / a.y );
}

ID_INLINE idVec2 operator*( const float a, const idVec2 b )
{
	return idVec2( b.x * a, b.y * a );
}

ID_INLINE idVec2 idVec2::operator+( const idVec2& a ) const
{
	return idVec2( x + a.x, y + a.y );
}

ID_INLINE idVec2& idVec2::operator+=( const idVec2& a )
{
	x += a.x;
	y += a.y;

	return *this;
}

ID_INLINE idVec2& idVec2::operator/=( const idVec2& a )
{
	x /= a.x;
	y /= a.y;

	return *this;
}

ID_INLINE idVec2& idVec2::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;

	return *this;
}

ID_INLINE idVec2& idVec2::operator-=( const idVec2& a )
{
	x -= a.x;
	y -= a.y;

	return *this;
}

ID_INLINE idVec2& idVec2::operator*=( const float a )
{
	x *= a;
	y *= a;

	return *this;
}

ID_INLINE idVec2 idVec2::Scale( const idVec2& a ) const
{
	return idVec2( x * a.x, y * a.y );
}

ID_INLINE int idVec2::GetDimension() const
{
	return 2;
}

ID_INLINE const float* idVec2::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec2::ToFloatPtr()
{
	return &x;
}

ID_INLINE idVec2& operator/( float lhs, idVec2& rhs )
{
	rhs.x = lhs / rhs.x;
	rhs.y = lhs / rhs.y;
	return rhs;
}

/*!
	\class idVec3
	\brief A 3-dimensional vector class used for representing positions, directions, and other geometric data in the engine.

	The idVec3 class provides a comprehensive set of operations for 3D vector mathematics including basic arithmetic, normalization, geometric projections, and conversions between different coordinate
   systems. It is designed to be a core building block for engine geometry calculations, physics simulations, and rendering operations. The class supports both precise and fast mathematical
   operations, with methods for handling degenerate cases and normalization. It provides conversions to and from other vector and matrix types, including polar coordinates, Euler angles, and rotation
   matrices. The implementation includes convenience methods for clamping, truncating, and snapping vector values, as well as linear and spherical interpolation between vectors. The class is optimized
   for performance with inline implementations of common operations and provides both exact and epsilon-based comparison methods.

*/
class idVec3
{
public:
	float x;
	float y;
	float z;

	//! Constructs a new idVec3 object with default values.
	idVec3();

	//! Constructs a vector with all components set to the given scalar value.
	explicit idVec3( const float xyz ) { Set( xyz, xyz, xyz ); }

	//! Constructs an idVec3 object with the specified x, y, and z components.
	explicit idVec3( const float x, const float y, const float z );

	//! Sets the components of the vector to the specified x, y, and z values.
	void		  Set( const float x, const float y, const float z );

	//! Sets all components of the vector to zero.
	void		  Zero();

	float		  operator[]( const int index ) const;
	float&		  operator[]( const int index );
	idVec3		  operator-() const;
	idVec3&		  operator=( const idVec3& a ); // required because of a msvc 6 & 7 bug
	float		  operator*( const idVec3& a ) const;
	idVec3		  operator*( const float a ) const;
	idVec3		  operator/( const float a ) const;
	idVec3		  operator+( const idVec3& a ) const;
	idVec3		  operator-( const idVec3& a ) const;
	idVec3&		  operator+=( const idVec3& a );
	idVec3&		  operator-=( const idVec3& a );
	idVec3&		  operator/=( const idVec3& a );
	idVec3&		  operator/=( const float a );
	idVec3&		  operator*=( const float a );

	friend idVec3 operator*( const float a, const idVec3 b );

	//! Returns true if this vector is exactly equal to the given vector, using exact comparison with no epsilon.
	bool		  Compare( const idVec3& a ) const;

	//! Compares this vector with another vector using the specified epsilon value
	bool		  Compare( const idVec3& a, const float epsilon ) const;
	bool		  operator==( const idVec3& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idVec3& a ) const; // exact compare, no epsilon

	//! Fixes degenerate axial cases in the normal vector.
	bool		  FixDegenerateNormal();

	//! Fixes denormalized floating-point values in the vector by setting very small numbers to zero.
	bool		  FixDenormals();

	//! Multiplies the vector components with the corresponding components of another vector.
	idVec3&		  MulCW( const idVec3& a );

	//! Divides the vector components by the corresponding components of the given vector.
	idVec3&		  DivCW( const idVec3& a );

	//! Returns the maximum component value of the vector.
	float		  Max() const;

	//! Returns the smallest component value of the vector.
	float		  Min() const;

	//! Computes the cross product of this vector with the given vector a.
	idVec3		  Cross( const idVec3& a ) const;

	//! Computes the cross product of two vectors and stores the result in this vector
	idVec3&		  Cross( const idVec3& a, const idVec3& b );

	//! Returns the Euclidean length of the vector.
	float		  Length() const;

	//! Calculates and returns the squared length of this vector.
	float		  LengthSqr() const;

	//! Computes an fast approximation of the vector's length using inverse square root.
	float		  LengthFast() const;

	//! Normalizes the vector and returns the length of the original vector.
	float		  Normalize();

	//! Normalizes the vector and returns its length.
	float		  NormalizeFast();

	//! Truncates the vector to a specified maximum length.
	idVec3		  Truncate( float length ) const;

	//! Clamps the vector components between the given minimum and maximum values.
	void		  Clamp( const idVec3& min, const idVec3& max );

	//! Rounds the vector components to the nearest integer values.
	void		  Snap();

	//! Snaps the vector components towards the nearest integer values using floor operation.
	void		  SnapInt();

	//! Returns the dimension of the vector, which is always 3 for idVec3.
	int			  GetDimension() const;

	//! Computes the yaw angle in degrees from the vector's x and y components.
	float		  ToYaw() const;

	//! Returns the pitch angle in degrees computed from the vector components.
	float		  ToPitch() const;

	//! Converts a vector to Euler angles.
	idAngles	  ToAngles() const;

	//! Converts a Cartesian 3D vector to polar coordinates.
	idPolar3	  ToPolar() const;

	//! Converts a normalized vector to a 3x3 rotation matrix.
	idMat3		  ToMat3() const;

	//! Returns a const reference to the 2D vector component of this 3D vector.
	const idVec2& ToVec2() const;

	//! Returns a reference to the x and y components of this vector as an idVec2.
	idVec2&		  ToVec2();

	//! Returns a pointer to the float array representation of this vector.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the first element of the vector.
	float*		  ToFloatPtr();

	//! Returns a string representation of the vector with the specified decimal precision
	const char*	  ToString( int precision = 2 ) const;

	//! Converts a normalized 3D vector to its octahedral representation on the [-1, +1] square.
	idVec2		  ToOctahedral() const;

	//! Converts an octahedral vector on the [-1, +1] square to a 3D unit vector.
	void		  FromOctahedral( const idVec2& v );

	//! Computes two perpendicular vectors to this vector, storing them in left and down.
	void		  NormalVectors( idVec3& left, idVec3& down ) const;

	//! Computes an orthogonal basis from this vector, filling the provided left and up vectors.
	void		  OrthogonalBasis( idVec3& left, idVec3& up ) const;

	//! Projects this vector onto a plane defined by the given normal, with optional over-bounce factor.
	void		  ProjectOntoPlane( const idVec3& normal, const float overBounce = 1.0f );

	//! Projects the vector along a plane defined by the normal vector, returning false if the vector is parallel to the plane.
	bool		  ProjectAlongPlane( const idVec3& normal, const float epsilon, const float overBounce = 1.0f );

	//! Projects this vector onto a sphere of the given radius.
	void		  ProjectSelfOntoSphere( const float radius );

	//! Computes a linear interpolation between two 3D vectors based on a scalar factor
	void		  Lerp( const idVec3& v1, const idVec3& v2, const float l );

	//! Performs spherical linear interpolation between two vectors.
	void		  SLerp( const idVec3& v1, const idVec3& v2, const float l );
};

extern idVec3 vec3_origin;
#define vec3_zero vec3_origin
extern idVec3 vec3_one;

ID_INLINE	  idVec3::idVec3()
{
}

ID_INLINE idVec3::idVec3( const float x, const float y, const float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ID_INLINE float idVec3::operator[]( const int index ) const
{
	return ( &x )[index];
}

ID_INLINE float& idVec3::operator[]( const int index )
{
	return ( &x )[index];
}

ID_INLINE void idVec3::Set( const float x, const float y, const float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ID_INLINE void idVec3::Zero()
{
	x = y = z = 0.0f;
}

ID_INLINE idVec3 idVec3::operator-() const
{
	return idVec3( -x, -y, -z );
}

ID_INLINE idVec3& idVec3::operator=( const idVec3& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

ID_INLINE idVec3 idVec3::operator-( const idVec3& a ) const
{
	return idVec3( x - a.x, y - a.y, z - a.z );
}

ID_INLINE float idVec3::operator*( const idVec3& a ) const
{
	return x * a.x + y * a.y + z * a.z;
}

ID_INLINE idVec3 idVec3::operator*( const float a ) const
{
	return idVec3( x * a, y * a, z * a );
}

ID_INLINE idVec3 idVec3::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idVec3( x * inva, y * inva, z * inva );
}

ID_INLINE idVec3 operator*( const float a, const idVec3 b )
{
	return idVec3( b.x * a, b.y * a, b.z * a );
}

ID_INLINE idVec3 operator/( const float a, const idVec3 b )
{
	return idVec3( a / b.x, a / b.y, a / b.z );
}

ID_INLINE idVec3 idVec3::operator+( const idVec3& a ) const
{
	return idVec3( x + a.x, y + a.y, z + a.z );
}

ID_INLINE idVec3& idVec3::operator+=( const idVec3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;

	return *this;
}

ID_INLINE idVec3& idVec3::operator/=( const idVec3& a )
{
	x /= a.x;
	y /= a.y;
	z /= a.z;

	return *this;
}

ID_INLINE idVec3& idVec3::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;

	return *this;
}

ID_INLINE idVec3& idVec3::operator-=( const idVec3& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;

	return *this;
}

ID_INLINE idVec3& idVec3::operator*=( const float a )
{
	x *= a;
	y *= a;
	z *= a;

	return *this;
}

ID_INLINE bool idVec3::Compare( const idVec3& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) );
}

ID_INLINE bool idVec3::Compare( const idVec3& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon ) { return false; }

	if( idMath::Fabs( y - a.y ) > epsilon ) { return false; }

	if( idMath::Fabs( z - a.z ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idVec3::operator==( const idVec3& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec3::operator!=( const idVec3& a ) const
{
	return !Compare( a );
}

ID_INLINE float idVec3::NormalizeFast()
{
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return invLength * sqrLength;
}

ID_INLINE bool idVec3::FixDegenerateNormal()
{
	if( x == 0.0f ) {
		if( y == 0.0f ) {
			if( z > 0.0f ) {
				if( z != 1.0f ) {
					z = 1.0f;
					return true;
				}
			} else {
				if( z != -1.0f ) {
					z = -1.0f;
					return true;
				}
			}
			return false;
		} else if( z == 0.0f ) {
			if( y > 0.0f ) {
				if( y != 1.0f ) {
					y = 1.0f;
					return true;
				}
			} else {
				if( y != -1.0f ) {
					y = -1.0f;
					return true;
				}
			}
			return false;
		}
	} else if( y == 0.0f ) {
		if( z == 0.0f ) {
			if( x > 0.0f ) {
				if( x != 1.0f ) {
					x = 1.0f;
					return true;
				}
			} else {
				if( x != -1.0f ) {
					x = -1.0f;
					return true;
				}
			}
			return false;
		}
	}
	if( idMath::Fabs( x ) == 1.0f ) {
		if( y != 0.0f || z != 0.0f ) {
			y = z = 0.0f;
			return true;
		}
		return false;
	} else if( idMath::Fabs( y ) == 1.0f ) {
		if( x != 0.0f || z != 0.0f ) {
			x = z = 0.0f;
			return true;
		}
		return false;
	} else if( idMath::Fabs( z ) == 1.0f ) {
		if( x != 0.0f || y != 0.0f ) {
			x = y = 0.0f;
			return true;
		}
		return false;
	}
	return false;
}

ID_INLINE bool idVec3::FixDenormals()
{
	bool denormal = false;
	if( fabs( x ) < 1e-30f ) {
		x		 = 0.0f;
		denormal = true;
	}
	if( fabs( y ) < 1e-30f ) {
		y		 = 0.0f;
		denormal = true;
	}
	if( fabs( z ) < 1e-30f ) {
		z		 = 0.0f;
		denormal = true;
	}
	return denormal;
}

ID_INLINE idVec3 idVec3::Cross( const idVec3& a ) const
{
	return idVec3( y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x );
}

ID_INLINE idVec3& idVec3::Cross( const idVec3& a, const idVec3& b )
{
	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;

	return *this;
}

ID_FORCE_INLINE idVec3& idVec3::MulCW( const idVec3& a )
{
	x *= a.x;
	y *= a.y;
	z *= a.z;

	return *this;
}

ID_FORCE_INLINE idVec3& idVec3::DivCW( const idVec3& a )
{
	x /= a.x;
	y /= a.y;
	z /= a.z;

	return *this;
}

ID_INLINE float idVec3::Max() const
{
	return idMath::Fmax( x, idMath::Fmax( y, z ) );
}

ID_INLINE float idVec3::Min() const
{
	return idMath::Fmin( x, idMath::Fmin( y, z ) );
}

ID_INLINE float idVec3::Length() const
{
	return ( float )idMath::Sqrt( x * x + y * y + z * z );
}

ID_INLINE float idVec3::LengthSqr() const
{
	return ( x * x + y * y + z * z );
}

ID_INLINE float idVec3::LengthFast() const
{
	float sqrLength;

	sqrLength = x * x + y * y + z * z;
	return sqrLength * idMath::InvSqrt( sqrLength );
}

ID_INLINE float idVec3::Normalize()
{
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return invLength * sqrLength;
}

ID_INLINE idVec3 idVec3::Truncate( float length ) const
{
	if( length < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		return vec3_zero;
	} else {
		float length2 = LengthSqr();
		if( length2 > length * length ) {
			float ilength = length * idMath::InvSqrt( length2 );
			return *this * ilength;
		}
	}
	return *this;
}

ID_INLINE void idVec3::Clamp( const idVec3& min, const idVec3& max )
{
	if( x < min.x ) {
		x = min.x;
	} else if( x > max.x ) {
		x = max.x;
	}
	if( y < min.y ) {
		y = min.y;
	} else if( y > max.y ) {
		y = max.y;
	}
	if( z < min.z ) {
		z = min.z;
	} else if( z > max.z ) {
		z = max.z;
	}
}

ID_INLINE void idVec3::Snap()
{
	x = floorf( x + 0.5f );
	y = floorf( y + 0.5f );
	z = floorf( z + 0.5f );
}

ID_INLINE void idVec3::SnapInt()
{
	x = float( int( x ) );
	y = float( int( y ) );
	z = float( int( z ) );
}

ID_INLINE int idVec3::GetDimension() const
{
	return 3;
}

ID_INLINE const idVec2& idVec3::ToVec2() const
{
	return *reinterpret_cast<const idVec2*>( this );
}

ID_INLINE idVec2& idVec3::ToVec2()
{
	return *reinterpret_cast<idVec2*>( this );
}

ID_INLINE const float* idVec3::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec3::ToFloatPtr()
{
	return &x;
}

ID_INLINE void idVec3::NormalVectors( idVec3& left, idVec3& down ) const
{
	float d;

	d = x * x + y * y;
	if( !d ) {
		left[0] = 1;
		left[1] = 0;
		left[2] = 0;
	} else {
		d		= idMath::InvSqrt( d );
		left[0] = -y * d;
		left[1] = x * d;
		left[2] = 0;
	}
	down = left.Cross( *this );
}

ID_INLINE void idVec3::OrthogonalBasis( idVec3& left, idVec3& up ) const
{
	float l, s;

	if( idMath::Fabs( z ) > 0.7f ) {
		l		= y * y + z * z;
		s		= idMath::InvSqrt( l );
		up[0]	= 0;
		up[1]	= z * s;
		up[2]	= -y * s;
		left[0] = l * s;
		left[1] = -x * up[2];
		left[2] = x * up[1];
	} else {
		l		= x * x + y * y;
		s		= idMath::InvSqrt( l );
		left[0] = -y * s;
		left[1] = x * s;
		left[2] = 0;
		up[0]	= -z * left[1];
		up[1]	= z * left[0];
		up[2]	= l * s;
	}
}

ID_INLINE void idVec3::ProjectOntoPlane( const idVec3& normal, const float overBounce )
{
	float backoff;

	backoff = *this * normal;

	if( overBounce != 1.0 ) {
		if( backoff < 0 ) {
			backoff *= overBounce;
		} else {
			backoff /= overBounce;
		}
	}

	*this -= backoff * normal;
}

ID_INLINE bool idVec3::ProjectAlongPlane( const idVec3& normal, const float epsilon, const float overBounce )
{
	idVec3 cross;
	float  len;

	cross = this->Cross( normal ).Cross( ( *this ) );
	// normalize so a fixed epsilon can be used
	cross.Normalize();
	len = normal * cross;
	if( idMath::Fabs( len ) < epsilon ) { return false; }
	cross *= overBounce * ( normal * ( *this ) ) / len;
	( *this ) -= cross;
	return true;
}

ID_INLINE idVec3& operator/( float lhs, idVec3& rhs )
{
	rhs.x = rhs.x / lhs;
	rhs.y = rhs.y / lhs;
	rhs.z = rhs.z / lhs;
	return rhs;
}

/*!
	\struct idTupleSize< idVec3 >
	\brief Template specialization for computing tuple size of idVec3 type.

*/
template<>
struct idTupleSize<idVec3> {
	enum { value = 3 };
};

/*!
	\class idVec4
	\brief A 4-dimensional vector class for handling geometric computations in the engine.

	The idVec4 class represents a 4-dimensional vector commonly used for homogeneous coordinates in graphics and geometric calculations within the engine. It provides standard vector arithmetic
   operations, component access through array-style indexing, and conversion methods to lower-dimensional vector types. The class supports both exact and epsilon-based comparisons, normalization
   operations, and linear interpolation between vectors. Memory layout is designed for efficient access and conversion to other vector types. The implementation includes both precise and fast
   normalization methods to balance accuracy and performance requirements of the engine's graphics and physics systems.

*/
class idVec4
{
public:
	float x;
	float y;
	float z;
	float w;

	//! Initializes an empty idVec4 object.
	idVec4() { }

	//! Creates a vector with all components set to the given float value.
	explicit idVec4( const float x ) { Set( x, x, x, x ); }

	//! Constructs a new idVec4 object with the specified x, y, z, and w components.
	explicit idVec4( const float x, const float y, const float z, const float w ) { Set( x, y, z, w ); }

	//! Sets the components of the vector to the specified x, y, z, and w values.
	void		  Set( const float x, const float y, const float z, const float w );

	//! Sets all components of the vector to zero.
	void		  Zero();

	float		  operator[]( const int index ) const;
	float&		  operator[]( const int index );
	idVec4		  operator-() const;
	float		  operator*( const idVec4& a ) const;
	idVec4		  operator*( const float a ) const;
	idVec4		  operator/( const float a ) const;
	idVec4		  operator+( const idVec4& a ) const;
	idVec4		  operator-( const idVec4& a ) const;
	idVec4&		  operator+=( const idVec4& a );
	idVec4&		  operator-=( const idVec4& a );
	idVec4&		  operator/=( const idVec4& a );
	idVec4&		  operator/=( const float a );
	idVec4&		  operator*=( const float a );

	friend idVec4 operator*( const float a, const idVec4 b );

	//! Returns a new vector with each component multiplied by the corresponding component of another vector.
	idVec4		  Multiply( const idVec4& a ) const;

	//! Compares this vector with another vector for exact equality.
	bool		  Compare( const idVec4& a ) const;

	//! Compares this vector with another vector using the specified epsilon tolerance.
	bool		  Compare( const idVec4& a, const float epsilon ) const;
	bool		  operator==( const idVec4& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idVec4& a ) const; // exact compare, no epsilon

	//! Returns the Euclidean length of this 4-dimensional vector.
	float		  Length() const;

	//! Returns the squared length of this vector.
	float		  LengthSqr() const;

	//! Normalizes the vector and returns the length of the vector.
	float		  Normalize();

	//! Normalizes the vector and returns the length of the vector.
	float		  NormalizeFast();

	//! Returns the dimension of the vector, which is 4.
	int			  GetDimension() const;

	//! Returns a const reference to the x and y components of this idVec4 as an idVec2
	const idVec2& ToVec2() const;

	//! Returns a reference to the x and y components of this vector as an idVec2.
	idVec2&		  ToVec2();

	//! Converts this idVec4 to an idVec3 by reinterpreting its memory.
	const idVec3& ToVec3() const;

	//! Converts this idVec4 to an idVec3 by reinterpreting its memory.
	idVec3&		  ToVec3();

	//! Returns a pointer to the internal float array representing the vector components.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the float data of the vector.
	float*		  ToFloatPtr();

	//! Converts the vector to a string representation with the specified decimal precision
	const char*	  ToString( int precision = 2 ) const;

	//! Computes a linear interpolation between two 4D vectors based on a interpolation factor
	void		  Lerp( const idVec4& v1, const idVec4& v2, const float l );
};

extern idVec4 vec4_origin;
#define vec4_zero vec4_origin
extern idVec4  vec4_one;

ID_INLINE void idVec4::Set( const float x, const float y, const float z, const float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

ID_INLINE void idVec4::Zero()
{
	x = y = z = w = 0.0f;
}

ID_INLINE float idVec4::operator[]( int index ) const
{
	return ( &x )[index];
}

ID_INLINE float& idVec4::operator[]( int index )
{
	return ( &x )[index];
}

ID_INLINE idVec4 idVec4::operator-() const
{
	return idVec4( -x, -y, -z, -w );
}

ID_INLINE idVec4 idVec4::operator-( const idVec4& a ) const
{
	return idVec4( x - a.x, y - a.y, z - a.z, w - a.w );
}

ID_INLINE float idVec4::operator*( const idVec4& a ) const
{
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

ID_INLINE idVec4 idVec4::operator*( const float a ) const
{
	return idVec4( x * a, y * a, z * a, w * a );
}

ID_INLINE idVec4 idVec4::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idVec4( x * inva, y * inva, z * inva, w * inva );
}

ID_INLINE idVec4 operator*( const float a, const idVec4 b )
{
	return idVec4( b.x * a, b.y * a, b.z * a, b.w * a );
}

ID_INLINE idVec4 idVec4::operator+( const idVec4& a ) const
{
	return idVec4( x + a.x, y + a.y, z + a.z, w + a.w );
}

ID_INLINE idVec4& idVec4::operator+=( const idVec4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	return *this;
}

ID_INLINE idVec4& idVec4::operator/=( const idVec4& a )
{
	x /= a.x;
	y /= a.y;
	z /= a.z;
	w /= a.w;

	return *this;
}

ID_INLINE idVec4& idVec4::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	w *= inva;

	return *this;
}

ID_INLINE idVec4& idVec4::operator-=( const idVec4& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	return *this;
}

ID_INLINE idVec4& idVec4::operator*=( const float a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return *this;
}

ID_INLINE idVec4 idVec4::Multiply( const idVec4& a ) const
{
	return idVec4( x * a.x, y * a.y, z * a.z, w * a.w );
}

ID_INLINE bool idVec4::Compare( const idVec4& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) && w == a.w );
}

ID_INLINE bool idVec4::Compare( const idVec4& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon ) { return false; }

	if( idMath::Fabs( y - a.y ) > epsilon ) { return false; }

	if( idMath::Fabs( z - a.z ) > epsilon ) { return false; }

	if( idMath::Fabs( w - a.w ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idVec4::operator==( const idVec4& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec4::operator!=( const idVec4& a ) const
{
	return !Compare( a );
}

ID_INLINE float idVec4::Length() const
{
	return ( float )idMath::Sqrt( x * x + y * y + z * z + w * w );
}

ID_INLINE float idVec4::LengthSqr() const
{
	return ( x * x + y * y + z * z + w * w );
}

ID_INLINE float idVec4::Normalize()
{
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z + w * w;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return invLength * sqrLength;
}

ID_INLINE float idVec4::NormalizeFast()
{
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z + w * w;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return invLength * sqrLength;
}

ID_INLINE int idVec4::GetDimension() const
{
	return 4;
}

ID_INLINE const idVec2& idVec4::ToVec2() const
{
	return *reinterpret_cast<const idVec2*>( this );
}

ID_INLINE idVec2& idVec4::ToVec2()
{
	return *reinterpret_cast<idVec2*>( this );
}

ID_INLINE const idVec3& idVec4::ToVec3() const
{
	return *reinterpret_cast<const idVec3*>( this );
}

ID_INLINE idVec3& idVec4::ToVec3()
{
	return *reinterpret_cast<idVec3*>( this );
}

ID_INLINE const float* idVec4::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec4::ToFloatPtr()
{
	return &x;
}

/*!
	\class idVec5
	\brief A 5-dimensional vector class used for representing positions with texture coordinates in the engine.

	The idVec5 class provides a data structure for handling 5-dimensional vectors, which are commonly used in the engine for representing positions with texture coordinates. It supports construction
   from various component types, including idVec3 and idVec2, as well as direct component initialization. The class provides access to its components through array-style indexing and conversion
   methods to and from idVec3 and float array representations. The class supports linear interpolation between vectors and can convert to string representation for debugging and logging purposes. The
   implementation uses inline methods for performance-critical operations and provides both const and non-const access to its underlying data.

*/
class idVec5
{
public:
	float x;
	float y;
	float z;
	float s;
	float t;

	//! Constructs a new idVec5 object with default values.
	idVec5();

	//! Constructs an idVec5 object using the components of an idVec3 and an idVec2.
	explicit idVec5( const idVec3& xyz, const idVec2& st );

	//! Constructs an idVec5 object with the specified x, y, z, s, and t components.
	explicit idVec5( const float x, const float y, const float z, const float s, const float t );

	float		  operator[]( int index ) const;
	float&		  operator[]( int index );
	idVec5&		  operator=( const idVec3& a );

	//! Returns the dimension of the vector, which is 5.
	int			  GetDimension() const;

	//! Returns a const reference to the Vec3 component of this Vec5.
	const idVec3& ToVec3() const;

	//! Returns a reference to the Vec3 component of this Vec5.
	idVec3&		  ToVec3();

	//! Returns a pointer to the float array representation of this vector.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the first element of the vector.
	float*		  ToFloatPtr();

	//! Converts the vector to a string representation with the specified precision.
	const char*	  ToString( int precision = 2 ) const;

	//! Computes a linear interpolation between two 5D vectors based on a scalar factor
	void		  Lerp( const idVec5& v1, const idVec5& v2, const float l );
};

extern idVec5 vec5_origin;
#define vec5_zero vec5_origin

ID_INLINE idVec5::idVec5()
{
}

ID_INLINE idVec5::idVec5( const idVec3& xyz, const idVec2& st )
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	s = st[0];
	t = st[1];
}

ID_INLINE idVec5::idVec5( const float x, const float y, const float z, const float s, const float t )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->s = s;
	this->t = t;
}

ID_INLINE float idVec5::operator[]( int index ) const
{
	return ( &x )[index];
}

ID_INLINE float& idVec5::operator[]( int index )
{
	return ( &x )[index];
}

ID_INLINE idVec5& idVec5::operator=( const idVec3& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	s = t = 0;
	return *this;
}

ID_INLINE int idVec5::GetDimension() const
{
	return 5;
}

ID_INLINE const idVec3& idVec5::ToVec3() const
{
	return *reinterpret_cast<const idVec3*>( this );
}

ID_INLINE idVec3& idVec5::ToVec3()
{
	return *reinterpret_cast<idVec3*>( this );
}

ID_INLINE const float* idVec5::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec5::ToFloatPtr()
{
	return &x;
}

/*!
	\class idVec6
	\brief A 6-dimensional vector class used for representing spatial coordinates and other vector-based data in the engine.

	The idVec6 class represents a 6-dimensional vector commonly used in the RogueStrad engine for handling spatial data and mathematical operations in 6D space. It provides constructors for
   initializing vectors from arrays or individual components, as well as comprehensive operator overloads for arithmetic operations and comparisons. The class supports normalization, length
   calculations, and provides access to sub-vectors of 3D components, making it suitable for representing transformations, physics data, or other multi-dimensional vector quantities within the
   engine's architecture. The implementation follows standard vector mathematics conventions with efficient inline operations for performance-critical code paths.

*/
class idVec6
{
public:
	//! Initializes a new instance of the idVec6 class with default values.
	idVec6();

	//! Initializes a new idVec6 instance with values from the provided float array.
	explicit idVec6( const float* a );

	//! Initializes a new idVec6 instance with the specified six float values.
	explicit idVec6( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );

	//! Sets the components of the vector to the provided values.
	void		  Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );

	//! Sets all components of the vector to zero.
	void		  Zero();

	float		  operator[]( const int index ) const;
	float&		  operator[]( const int index );
	idVec6		  operator-() const;
	idVec6		  operator*( const float a ) const;
	idVec6		  operator/( const float a ) const;
	float		  operator*( const idVec6& a ) const;
	idVec6		  operator-( const idVec6& a ) const;
	idVec6		  operator+( const idVec6& a ) const;
	idVec6&		  operator*=( const float a );
	idVec6&		  operator/=( const float a );
	idVec6&		  operator+=( const idVec6& a );
	idVec6&		  operator-=( const idVec6& a );

	friend idVec6 operator*( const float a, const idVec6 b );

	//! Compares this vector with another vector for exact equality.
	bool		  Compare( const idVec6& a ) const;

	//! Compares this vector with another vector using the specified epsilon tolerance.
	bool		  Compare( const idVec6& a, const float epsilon ) const;
	bool		  operator==( const idVec6& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idVec6& a ) const; // exact compare, no epsilon

	//! Returns the Euclidean length of this 6D vector.
	float		  Length() const;

	//! Returns the squared length of this 6D vector.
	float		  LengthSqr() const;

	//! Normalizes the vector and returns the inverse length.
	float		  Normalize();

	//! Normalizes the vector and returns the inverse length.
	float		  NormalizeFast();

	//! Returns the dimension of the vector, which is 6.
	int			  GetDimension() const;

	//! Returns a const reference to a sub-vector of three consecutive components starting at the specified index.
	const idVec3& SubVec3( int index ) const;

	//! Returns a reference to a sub-vector of three consecutive elements starting at the specified index.
	idVec3&		  SubVec3( int index );

	//! Returns a pointer to the internal float array representing the vector components.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the float array representing the vector components.
	float*		  ToFloatPtr();

	//! Converts the vector to a string representation with the specified precision.
	const char*	  ToString( int precision = 2 ) const;

private:
	float p[6];
};

extern idVec6 vec6_origin;
#define vec6_zero vec6_origin
extern idVec6 vec6_infinity;

ID_INLINE	  idVec6::idVec6()
{
}

ID_INLINE idVec6::idVec6( const float* a )
{
	memcpy( p, a, 6 * sizeof( float ) );
}

ID_INLINE idVec6::idVec6( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE idVec6 idVec6::operator-() const
{
	return idVec6( -p[0], -p[1], -p[2], -p[3], -p[4], -p[5] );
}

ID_INLINE float idVec6::operator[]( const int index ) const
{
	return p[index];
}

ID_INLINE float& idVec6::operator[]( const int index )
{
	return p[index];
}

ID_INLINE idVec6 idVec6::operator*( const float a ) const
{
	return idVec6( p[0] * a, p[1] * a, p[2] * a, p[3] * a, p[4] * a, p[5] * a );
}

ID_INLINE float idVec6::operator*( const idVec6& a ) const
{
	return p[0] * a[0] + p[1] * a[1] + p[2] * a[2] + p[3] * a[3] + p[4] * a[4] + p[5] * a[5];
}

ID_INLINE idVec6 idVec6::operator/( const float a ) const
{
	float inva;

	assert( a != 0.0f );
	inva = 1.0f / a;
	return idVec6( p[0] * inva, p[1] * inva, p[2] * inva, p[3] * inva, p[4] * inva, p[5] * inva );
}

ID_INLINE idVec6 idVec6::operator+( const idVec6& a ) const
{
	return idVec6( p[0] + a[0], p[1] + a[1], p[2] + a[2], p[3] + a[3], p[4] + a[4], p[5] + a[5] );
}

ID_INLINE idVec6 idVec6::operator-( const idVec6& a ) const
{
	return idVec6( p[0] - a[0], p[1] - a[1], p[2] - a[2], p[3] - a[3], p[4] - a[4], p[5] - a[5] );
}

ID_INLINE idVec6& idVec6::operator*=( const float a )
{
	p[0] *= a;
	p[1] *= a;
	p[2] *= a;
	p[3] *= a;
	p[4] *= a;
	p[5] *= a;
	return *this;
}

ID_INLINE idVec6& idVec6::operator/=( const float a )
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

ID_INLINE idVec6& idVec6::operator+=( const idVec6& a )
{
	p[0] += a[0];
	p[1] += a[1];
	p[2] += a[2];
	p[3] += a[3];
	p[4] += a[4];
	p[5] += a[5];
	return *this;
}

ID_INLINE idVec6& idVec6::operator-=( const idVec6& a )
{
	p[0] -= a[0];
	p[1] -= a[1];
	p[2] -= a[2];
	p[3] -= a[3];
	p[4] -= a[4];
	p[5] -= a[5];
	return *this;
}

ID_INLINE idVec6 operator*( const float a, const idVec6 b )
{
	return b * a;
}

ID_INLINE bool idVec6::Compare( const idVec6& a ) const
{
	return ( ( p[0] == a[0] ) && ( p[1] == a[1] ) && ( p[2] == a[2] ) && ( p[3] == a[3] ) && ( p[4] == a[4] ) && ( p[5] == a[5] ) );
}

ID_INLINE bool idVec6::Compare( const idVec6& a, const float epsilon ) const
{
	if( idMath::Fabs( p[0] - a[0] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[1] - a[1] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[2] - a[2] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[3] - a[3] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[4] - a[4] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[5] - a[5] ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idVec6::operator==( const idVec6& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec6::operator!=( const idVec6& a ) const
{
	return !Compare( a );
}

ID_INLINE void idVec6::Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE void idVec6::Zero()
{
	p[0] = p[1] = p[2] = p[3] = p[4] = p[5] = 0.0f;
}

ID_INLINE float idVec6::Length() const
{
	return ( float )idMath::Sqrt( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5] );
}

ID_INLINE float idVec6::LengthSqr() const
{
	return ( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5] );
}

ID_INLINE float idVec6::Normalize()
{
	float sqrLength, invLength;

	sqrLength = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5];
	invLength = idMath::InvSqrt( sqrLength );
	p[0] *= invLength;
	p[1] *= invLength;
	p[2] *= invLength;
	p[3] *= invLength;
	p[4] *= invLength;
	p[5] *= invLength;
	return invLength * sqrLength;
}

ID_INLINE float idVec6::NormalizeFast()
{
	float sqrLength, invLength;

	sqrLength = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5];
	invLength = idMath::InvSqrt( sqrLength );
	p[0] *= invLength;
	p[1] *= invLength;
	p[2] *= invLength;
	p[3] *= invLength;
	p[4] *= invLength;
	p[5] *= invLength;
	return invLength * sqrLength;
}

ID_INLINE int idVec6::GetDimension() const
{
	return 6;
}

ID_INLINE const idVec3& idVec6::SubVec3( int index ) const
{
	return *reinterpret_cast<const idVec3*>( p + index * 3 );
}

ID_INLINE idVec3& idVec6::SubVec3( int index )
{
	return *reinterpret_cast<idVec3*>( p + index * 3 );
}

ID_INLINE const float* idVec6::ToFloatPtr() const
{
	return p;
}

ID_INLINE float* idVec6::ToFloatPtr()
{
	return p;
}

/*!
	\class idPolar3
	\brief Represents a 3D polar coordinate system with radius, theta, and phi components.

	The idPolar3 class provides a mathematical representation of 3D polar coordinates, which are commonly used in engine calculations involving angular and radial positioning. It supports construction
   with specific radius, theta, and phi values, as well as conversion to Cartesian vector representation. The class is designed for use in engine systems that require polar coordinate transformations,
   particularly for positioning and orientation calculations in 3D space. The class provides direct access to its components through array-style indexing and supports basic arithmetic operations for
   coordinate manipulation.

*/
class idPolar3
{
public:
	float radius, theta, phi;

	//! Creates a new polar coordinate object with default values.
	idPolar3();

	//! Constructs a new polar coordinate with the specified radius, theta, and phi values.
	explicit idPolar3( const float radius, const float theta, const float phi );

	//! Sets the radius, theta, and phi values for the polar coordinate.
	void	  Set( const float radius, const float theta, const float phi );

	float	  operator[]( const int index ) const;
	float&	  operator[]( const int index );
	idPolar3  operator-() const;
	idPolar3& operator=( const idPolar3& a );

	//! Converts a polar coordinate representation to a Cartesian vector.
	idVec3	  ToVec3() const;
};

ID_INLINE idPolar3::idPolar3()
{
}

ID_INLINE idPolar3::idPolar3( const float radius, const float theta, const float phi )
{
	assert( radius > 0 );
	this->radius = radius;
	this->theta	 = theta;
	this->phi	 = phi;
}

ID_INLINE void idPolar3::Set( const float radius, const float theta, const float phi )
{
	assert( radius > 0 );
	this->radius = radius;
	this->theta	 = theta;
	this->phi	 = phi;
}

ID_INLINE float idPolar3::operator[]( const int index ) const
{
	return ( &radius )[index];
}

ID_INLINE float& idPolar3::operator[]( const int index )
{
	return ( &radius )[index];
}

ID_INLINE idPolar3 idPolar3::operator-() const
{
	return idPolar3( radius, -theta, -phi );
}

ID_INLINE idPolar3& idPolar3::operator=( const idPolar3& a )
{
	radius = a.radius;
	theta  = a.theta;
	phi	   = a.phi;
	return *this;
}

ID_INLINE idVec3 idPolar3::ToVec3() const
{
	float sp, cp, st, ct;
	idMath::SinCos( phi, sp, cp );
	idMath::SinCos( theta, st, ct );
	return idVec3( cp * radius * ct, cp * radius * st, radius * sp );
}

namespace VectorUtil
{

//! Converts a vector4 color to a packed 32-bit integer color value.
inline uint32_t Vec4ToColorInt( const idVec4& vec )
{
	idVec4 vecCopy = 255.0f * vec;
	return ( ( uint32_t )vecCopy[0] << 28 ) | ( ( uint32_t )vecCopy[1] << 20 ) | ( ( uint32_t )vecCopy[2] << 12 ) | ( uint32_t )vecCopy[3];
}
}

/*
===============================================================================

	Old 3D vector macros, should no longer be used.

===============================================================================
*/

#define DotProduct( a, b )		  ( ( a )[0] * ( b )[0] + ( a )[1] * ( b )[1] + ( a )[2] * ( b )[2] )
#define VectorSubtract( a, b, c ) ( ( c )[0] = ( a )[0] - ( b )[0], ( c )[1] = ( a )[1] - ( b )[1], ( c )[2] = ( a )[2] - ( b )[2] )
#define VectorAdd( a, b, c )	  ( ( c )[0] = ( a )[0] + ( b )[0], ( c )[1] = ( a )[1] + ( b )[1], ( c )[2] = ( a )[2] + ( b )[2] )
#define VectorScale( v, s, o )	  ( ( o )[0] = ( v )[0] * ( s ), ( o )[1] = ( v )[1] * ( s ), ( o )[2] = ( v )[2] * ( s ) )
#define VectorMA( v, s, b, o )	  ( ( o )[0] = ( v )[0] + ( b )[0] * ( s ), ( o )[1] = ( v )[1] + ( b )[1] * ( s ), ( o )[2] = ( v )[2] + ( b )[2] * ( s ) )
#define VectorCopy( a, b )		  ( ( b )[0] = ( a )[0], ( b )[1] = ( a )[1], ( b )[2] = ( a )[2] )
#define VectorNegate( a, b )	  ( ( b )[0] = -( a )[0], ( b )[1] = -( a )[1], ( b )[2] = -( a )[2] )

#endif /* !__MATH_VECTOR_H__ */
