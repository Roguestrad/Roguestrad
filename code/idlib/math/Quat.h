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

#ifndef __MATH_QUAT_H__
#define __MATH_QUAT_H__

/*
===============================================================================

	Quaternion

===============================================================================
*/

#include "../containers/Array.h" // for idTupleSize

class idVec3;
class idAngles;
class idRotation;
class idMat3;
class idMat4;
class idCQuat;

/*!
	\class idQuat
	\brief A class representing a quaternion for 3D rotation calculations.

	This class implements quaternion mathematics for efficient and accurate 3D rotation operations. It supports standard arithmetic operations including addition, subtraction, multiplication, and
   scalar operations. The class provides methods for converting between quaternions and other rotation representations such as Euler angles, rotation matrices, and rotation objects. It includes
   utility functions for normalization, inversion, and interpolation between quaternions. Quaternions are commonly used in 3D graphics and game engines to represent rotations without suffering from
   gimbal lock issues.

*/
class idQuat
{
public:
	float x;
	float y;
	float z;
	float w;

	//! Constructs a new identity quaternion.
	idQuat();

	//! Initializes a new quaternion with the specified x, y, z, and w components.
	idQuat( float x, float y, float z, float w );

	//! Sets the components of the quaternion to the specified x, y, z, and w values.
	void		  Set( float x, float y, float z, float w );

	//! Returns the component of the quaternion at the specified index
	float		  operator[]( int index ) const;

	//! Provides indexed access to the quaternion components
	float&		  operator[]( int index );

	//! Returns the negation of this quaternion.
	idQuat		  operator-() const;

	//! Assigns the values of another quaternion to this quaternion and returns a reference to this quaternion.
	idQuat&		  operator=( const idQuat& a );

	//! Returns the component-wise sum of this quaternion and another quaternion.
	idQuat		  operator+( const idQuat& a ) const;

	//! Adds the components of the given quaternion to this quaternion and returns a reference to this quaternion.
	idQuat&		  operator+=( const idQuat& a );

	//! Returns a new quaternion that is the component-wise difference between this quaternion and the given quaternion.
	idQuat		  operator-( const idQuat& a ) const;

	//! Subtracts the components of the given quaternion from this quaternion and returns a reference to this quaternion.
	idQuat&		  operator-=( const idQuat& a );

	//! Returns the quaternion product of this quaternion and another quaternion.
	idQuat		  operator*( const idQuat& a ) const;

	//! Returns the result of rotating a 3D vector by this quaternion.
	idVec3		  operator*( const idVec3& a ) const;

	//! Multiplies the quaternion by a scalar value and returns the result.
	idQuat		  operator*( float a ) const;

	//! Updates this quaternion by multiplying it with another quaternion and returns a reference to itself.
	idQuat&		  operator*=( const idQuat& a );

	//! Multiplies the quaternion components by the given scalar value and returns a reference to the quaternion.
	idQuat&		  operator*=( float a );

	friend idQuat operator*( const float a, const idQuat& b );
	friend idVec3 operator*( const idVec3& a, const idQuat& b );

	//! Compares this quaternion with another quaternion for exact equality.
	bool		  Compare( const idQuat& a ) const;

	//! Compares this quaternion with another quaternion using the specified epsilon tolerance
	bool		  Compare( const idQuat& a, const float epsilon ) const;

	//! Compares two idQuat objects for exact equality without using epsilon
	bool		  operator==( const idQuat& a ) const;

	//! Returns true if this quaternion is not equal to the given quaternion.
	bool		  operator!=( const idQuat& a ) const;

	//! Returns the inverse of this quaternion by negating the imaginary components.
	idQuat		  Inverse() const;

	//! Returns the length of the quaternion.
	float		  Length() const;

	//! Normalizes the quaternion by scaling its components to unit length
	idQuat&		  Normalize();

	//! Calculates the W component of a quaternion from its X, Y, and Z components.
	float		  CalcW() const;

	//! Returns the dimension of the quaternion, which is always 4.
	int			  GetDimension() const;

	//! Converts a quaternion to Euler angles.
	idAngles	  ToAngles() const;

	//! Converts a quaternion to a rotation object.
	idRotation	  ToRotation() const;

	//! Converts a quaternion to a 3x3 rotation matrix
	idMat3		  ToMat3() const;

	//! Converts this quaternion to a 4x4 matrix.
	idMat4		  ToMat4() const;

	//! Converts this quaternion to a complex quaternion with positive scalar part
	idCQuat		  ToCQuat() const;

	//! Converts a quaternion to its corresponding angular velocity vector.
	idVec3		  ToAngularVelocity() const;

	//! Returns a pointer to the float array representation of this quaternion
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the float representation of the quaternion components.
	float*		  ToFloatPtr();

	//! Converts the quaternion to a string representation with the specified precision
	const char*	  ToString( int precision = 2 ) const;

	//! Performs spherical linear interpolation between two quaternions.
	idQuat&		  Slerp( const idQuat& from, const idQuat& to, float t );

	//! Performs linear interpolation between two quaternions.
	idQuat&		  Lerp( const idQuat& from, const idQuat& to, const float t );
};

//! Performs spherical linear interpolation between two quaternions.
idQuat	  Slerp( const idQuat& from, const idQuat& to, const float t );

ID_INLINE idQuat::idQuat()
{
}

ID_INLINE idQuat::idQuat( float x, float y, float z, float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

ID_INLINE float idQuat::operator[]( int index ) const
{
	assert( ( index >= 0 ) && ( index < 4 ) );
	return ( &x )[index];
}

ID_INLINE float& idQuat::operator[]( int index )
{
	assert( ( index >= 0 ) && ( index < 4 ) );
	return ( &x )[index];
}

ID_INLINE idQuat idQuat::operator-() const
{
	return idQuat( -x, -y, -z, -w );
}

ID_INLINE idQuat& idQuat::operator=( const idQuat& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;

	return *this;
}

ID_INLINE idQuat idQuat::operator+( const idQuat& a ) const
{
	return idQuat( x + a.x, y + a.y, z + a.z, w + a.w );
}

ID_INLINE idQuat& idQuat::operator+=( const idQuat& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	return *this;
}

ID_INLINE idQuat idQuat::operator-( const idQuat& a ) const
{
	return idQuat( x - a.x, y - a.y, z - a.z, w - a.w );
}

ID_INLINE idQuat& idQuat::operator-=( const idQuat& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	return *this;
}

ID_INLINE idQuat idQuat::operator*( const idQuat& a ) const
{
	return idQuat( w * a.x + x * a.w + y * a.z - z * a.y, w * a.y + y * a.w + z * a.x - x * a.z, w * a.z + z * a.w + x * a.y - y * a.x, w * a.w - x * a.x - y * a.y - z * a.z );
}

ID_INLINE idVec3 idQuat::operator*( const idVec3& a ) const
{
#if 0
	// it's faster to do the conversion to a 3x3 matrix and multiply the vector by this 3x3 matrix
	return ( ToMat3() * a );
#else
	// result = this->Inverse() * idQuat( a.x, a.y, a.z, 0.0f ) * (*this)
	float xxzz = x * x - z * z;
	float wwyy = w * w - y * y;

	float xw2 = x * w * 2.0f;
	float xy2 = x * y * 2.0f;
	float xz2 = x * z * 2.0f;
	float yw2 = y * w * 2.0f;
	float yz2 = y * z * 2.0f;
	float zw2 = z * w * 2.0f;

	return idVec3( ( xxzz + wwyy ) * a.x + ( xy2 + zw2 ) * a.y + ( xz2 - yw2 ) * a.z,
		( xy2 - zw2 ) * a.x + ( y * y + w * w - x * x - z * z ) * a.y + ( yz2 + xw2 ) * a.z,
		( xz2 + yw2 ) * a.x + ( yz2 - xw2 ) * a.y + ( wwyy - xxzz ) * a.z );
#endif
}

ID_INLINE idQuat idQuat::operator*( float a ) const
{
	return idQuat( x * a, y * a, z * a, w * a );
}

//! Returns the result of multiplying a quaternion by a scalar value.
ID_INLINE idQuat operator*( const float a, const idQuat& b )
{
	return b * a;
}

//! Multiplies a vector by a quaternion.
ID_INLINE idVec3 operator*( const idVec3& a, const idQuat& b )
{
	return b * a;
}

ID_INLINE idQuat& idQuat::operator*=( const idQuat& a )
{
	*this = *this * a;

	return *this;
}

ID_INLINE idQuat& idQuat::operator*=( float a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return *this;
}

ID_INLINE bool idQuat::Compare( const idQuat& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) && ( w == a.w ) );
}

ID_INLINE bool idQuat::Compare( const idQuat& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon ) { return false; }
	if( idMath::Fabs( y - a.y ) > epsilon ) { return false; }
	if( idMath::Fabs( z - a.z ) > epsilon ) { return false; }
	if( idMath::Fabs( w - a.w ) > epsilon ) { return false; }
	return true;
}

ID_INLINE bool idQuat::operator==( const idQuat& a ) const
{
	return Compare( a );
}

ID_INLINE bool idQuat::operator!=( const idQuat& a ) const
{
	return !Compare( a );
}

ID_INLINE void idQuat::Set( float x, float y, float z, float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

ID_INLINE idQuat idQuat::Inverse() const
{
	return idQuat( -x, -y, -z, w );
}

ID_INLINE float idQuat::Length() const
{
	float len;

	len = x * x + y * y + z * z + w * w;
	return idMath::Sqrt( len );
}

ID_INLINE idQuat& idQuat::Normalize()
{
	float len;
	float ilength;

	len = this->Length();
	if( len ) {
		ilength = 1 / len;
		x *= ilength;
		y *= ilength;
		z *= ilength;
		w *= ilength;
	}
	return *this;
}

ID_INLINE float idQuat::CalcW() const
{
	// take the absolute value because floating point rounding may cause the dot of x,y,z to be larger than 1
	return sqrt( fabs( 1.0f - ( x * x + y * y + z * z ) ) );
}

ID_INLINE int idQuat::GetDimension() const
{
	return 4;
}

ID_INLINE const float* idQuat::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idQuat::ToFloatPtr()
{
	return &x;
}

/*
===============================================================================

	Specialization to get size of an idQuat generically.

===============================================================================
*/
template<>
struct idTupleSize<idQuat> {
	enum { value = 4 };
};

/*!
	\class idCQuat
	\brief A class representing a quaternion with basic operations and conversions.

	This class provides a quaternion implementation with constructors for identity and component-based initialization, as well as methods for accessing and comparing quaternion components. It supports
   conversion to various rotation representations including Euler angles, rotation matrices, and transformation matrices. The class is designed for use in 3D graphics and rotation calculations,
   offering both direct access to components and conversion utilities. Memory management is handled through standard C++ practices with no explicit ownership semantics.

*/
class idCQuat
{
public:
	float x;
	float y;
	float z;

	//! Constructs a new identity quaternion.
	idCQuat();

	//! Constructs a quaternion with the specified x, y, and z components.
	idCQuat( float x, float y, float z );

	//! Sets the x, y, and z components of the quaternion.
	void		 Set( float x, float y, float z );

	//! Returns the quaternion component at the specified index
	float		 operator[]( int index ) const;

	//! Returns a reference to the quaternion component at the specified index.
	float&		 operator[]( int index );

	//! Compares this quaternion with another quaternion for exact equality.
	bool		 Compare( const idCQuat& a ) const;

	//! Compares this quaternion with another quaternion using the specified epsilon tolerance.
	bool		 Compare( const idCQuat& a, const float epsilon ) const;

	//! Compares two quaternions for exact equality without using epsilon.
	bool		 operator==( const idCQuat& a ) const;

	//! Returns true if this quaternion is not equal to the given quaternion.
	bool		 operator!=( const idCQuat& a ) const;

	//! Returns the dimension of the quaternion, which is 3.
	int			 GetDimension() const;

	//! Converts a quaternion to Euler angles.
	idAngles	 ToAngles() const;

	//! Converts the quaternion to a rotation matrix.
	idRotation	 ToRotation() const;

	//! Converts the quaternion to a 3x3 matrix representation.
	idMat3		 ToMat3() const;

	//! Converts the quaternion to a 4x4 transformation matrix.
	idMat4		 ToMat4() const;

	//! Converts a normalized quaternion to a full quaternion representation
	idQuat		 ToQuat() const;

	//! Returns a pointer to the internal float array representation of the quaternion.
	const float* ToFloatPtr() const;

	//! Returns a pointer to the internal float array representing the quaternion components.
	float*		 ToFloatPtr();

	//! Returns a string representation of the quaternion with the specified decimal precision
	const char*	 ToString( int precision = 2 ) const;
};

ID_INLINE idCQuat::idCQuat()
{
}

ID_INLINE idCQuat::idCQuat( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ID_INLINE void idCQuat::Set( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ID_INLINE float idCQuat::operator[]( int index ) const
{
	assert( ( index >= 0 ) && ( index < 3 ) );
	return ( &x )[index];
}

ID_INLINE float& idCQuat::operator[]( int index )
{
	assert( ( index >= 0 ) && ( index < 3 ) );
	return ( &x )[index];
}

ID_INLINE bool idCQuat::Compare( const idCQuat& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) );
}

ID_INLINE bool idCQuat::Compare( const idCQuat& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon ) { return false; }
	if( idMath::Fabs( y - a.y ) > epsilon ) { return false; }
	if( idMath::Fabs( z - a.z ) > epsilon ) { return false; }
	return true;
}

ID_INLINE bool idCQuat::operator==( const idCQuat& a ) const
{
	return Compare( a );
}

ID_INLINE bool idCQuat::operator!=( const idCQuat& a ) const
{
	return !Compare( a );
}

ID_INLINE int idCQuat::GetDimension() const
{
	return 3;
}

ID_INLINE idQuat idCQuat::ToQuat() const
{
	// take the absolute value because floating point rounding may cause the dot of x,y,z to be larger than 1
	return idQuat( x, y, z, sqrt( fabs( 1.0f - ( x * x + y * y + z * z ) ) ) );
}

ID_INLINE const float* idCQuat::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idCQuat::ToFloatPtr()
{
	return &x;
}

/*
===============================================================================

	Specialization to get size of an idCQuat generically.

===============================================================================
*/
template<>
struct idTupleSize<idCQuat> {
	enum { value = 3 };
};

#endif /* !__MATH_QUAT_H__ */
