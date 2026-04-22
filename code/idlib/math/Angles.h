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

#ifndef __MATH_ANGLES_H__
#define __MATH_ANGLES_H__

/*
===============================================================================

	Euler angles

===============================================================================
*/

// angle indexes
#define PITCH 0 // up / down
#define YAW	  1 // left / right
#define ROLL  2 // fall over

class idVec3;
class idQuat;
class idRotation;
class idMat3;
class idMat4;

/*!
	\class idAngles
	\brief Represents and manipulates 3D Euler angles with common arithmetic and conversion operations.

	The idAngles class provides a comprehensive interface for working with Euler angles in three-dimensional space, supporting standard arithmetic operations, normalization, clamping, and conversion
   to other rotational representations such as quaternions, rotation matrices, and vectors. It is designed for use in 3D graphics and game development contexts where angular transformations are
   required. The class supports both const and non-const access to its components through indexed operators, and provides methods for converting the angle representation into various vector and matrix
   formats. The implementation includes normalization methods to keep angle values within standard ranges, as well as comparison functions with and without epsilon tolerance for floating-point
   precision handling.

*/
class idAngles
{
public:
	float pitch;
	float yaw;
	float roll;

	//! Default constructor for the idAngles class.
	idAngles();

	//! Initializes an idAngles object with specified pitch, yaw, and roll values.
	idAngles( float pitch, float yaw, float roll );

	//! Constructs an idAngles object from a given idVec3 vector.
	explicit idAngles( const idVec3& v );

	//! Sets the pitch, yaw, and roll angles.
	void			Set( float pitch, float yaw, float roll );

	//! Sets all angle components to zero and returns a reference to this object.
	idAngles&		Zero();

	//! Returns the angle component at the specified index.
	float			operator[]( int index ) const;

	//! Provides indexed access to the pitch, yaw, and roll components of the angles.
	float&			operator[]( int index );

	//! Returns the negated angles with inverted pitch, yaw, and roll values.
	idAngles		operator-() const;

	//! Assigns the values of another idAngles object to this object and returns a reference to this object.
	idAngles&		operator=( const idAngles& a );

	//! Returns a new idAngles object that is the result of adding the current object and the provided idAngles object component-wise.
	idAngles		operator+( const idAngles& a ) const;

	//! Adds the components of the given angles to this angles object and returns a reference to this object.
	idAngles&		operator+=( const idAngles& a );

	//! Returns a new idAngles object with each component subtracted from the corresponding component of the input angle object.
	idAngles		operator-( const idAngles& a ) const;

	//! Subtracts the components of the given angles from this angles and returns a reference to this object.
	idAngles&		operator-=( const idAngles& a );

	//! Returns a new idAngles object with each angle component scaled by the given float value.
	idAngles		operator*( const float a ) const;

	//! Multiplies each component of the angles by the given scalar value.
	idAngles&		operator*=( const float a );

	//! Returns a new idAngles object with each angle component divided by the given scalar value.
	idAngles		operator/( const float a ) const;

	//! Divides each component of the angle by the given scalar value.
	idAngles&		operator/=( const float a );

	friend idAngles operator*( const float a, const idAngles& b );

	//! Compares this angle set with another for exact equality.
	bool			Compare( const idAngles& a ) const;

	//! Compares this angle instance with another angle instance using the specified epsilon tolerance.
	bool			Compare( const idAngles& a, const float epsilon ) const;

	//! Compares two idAngles objects for equality without using epsilon.
	bool			operator==( const idAngles& a ) const;

	//! Returns true if this angle instance is not equal to another angle instance.
	bool			operator!=( const idAngles& a ) const;

	//! Normalizes the angles in this object to the range [0, 360) degrees.
	idAngles&		Normalize360();

	//! Normalizes the angle values to the range [-180, 180].
	idAngles&		Normalize180();

	//! Clamps the angle values to the specified minimum and maximum ranges.
	void			Clamp( const idAngles& min, const idAngles& max );

	//! Returns the dimension of the angles, which is always 3.
	int				GetDimension() const;

	//! Converts angles to forward, right, and up vectors.
	void			ToVectors( idVec3* forward, idVec3* right = NULL, idVec3* up = NULL ) const;

	//! Returns the forward vector corresponding to the yaw and pitch angles.
	idVec3			ToForward() const;

	//! Converts the angle representation to a quaternion.
	idQuat			ToQuat() const;

	//! Converts an angles object to a rotation object.
	idRotation		ToRotation() const;

	//! Converts the angle values to a 3x3 rotation matrix.
	idMat3			ToMat3() const;

	//! Converts the angles to a 4x4 transformation matrix.
	idMat4			ToMat4() const;

	//! Converts the angles to an angular velocity vector.
	idVec3			ToAngularVelocity() const;

	//! Returns a pointer to the internal float array representing the angles.
	const float*	ToFloatPtr() const;

	//! Returns a pointer to the internal float array representing the angles.
	float*			ToFloatPtr();

	//! Converts the angles to a string representation with the specified precision.
	const char*		ToString( int precision = 2 ) const;
};

extern idAngles ang_zero;

ID_INLINE		idAngles::idAngles()
{
}

ID_INLINE idAngles::idAngles( float pitch, float yaw, float roll )
{
	this->pitch = pitch;
	this->yaw	= yaw;
	this->roll	= roll;
}

ID_INLINE idAngles::idAngles( const idVec3& v )
{
	this->pitch = v[0];
	this->yaw	= v[1];
	this->roll	= v[2];
}

ID_INLINE void idAngles::Set( float pitch, float yaw, float roll )
{
	this->pitch = pitch;
	this->yaw	= yaw;
	this->roll	= roll;
}

ID_INLINE idAngles& idAngles::Zero()
{
	pitch = yaw = roll = 0.0f;
	return *this;
}

ID_INLINE float idAngles::operator[]( int index ) const
{
	assert( ( index >= 0 ) && ( index < 3 ) );
	return ( &pitch )[index];
}

ID_INLINE float& idAngles::operator[]( int index )
{
	assert( ( index >= 0 ) && ( index < 3 ) );
	return ( &pitch )[index];
}

ID_INLINE idAngles idAngles::operator-() const
{
	return idAngles( -pitch, -yaw, -roll );
}

ID_INLINE idAngles& idAngles::operator=( const idAngles& a )
{
	pitch = a.pitch;
	yaw	  = a.yaw;
	roll  = a.roll;
	return *this;
}

ID_INLINE idAngles idAngles::operator+( const idAngles& a ) const
{
	return idAngles( pitch + a.pitch, yaw + a.yaw, roll + a.roll );
}

ID_INLINE idAngles& idAngles::operator+=( const idAngles& a )
{
	pitch += a.pitch;
	yaw += a.yaw;
	roll += a.roll;

	return *this;
}

ID_INLINE idAngles idAngles::operator-( const idAngles& a ) const
{
	return idAngles( pitch - a.pitch, yaw - a.yaw, roll - a.roll );
}

ID_INLINE idAngles& idAngles::operator-=( const idAngles& a )
{
	pitch -= a.pitch;
	yaw -= a.yaw;
	roll -= a.roll;

	return *this;
}

ID_INLINE idAngles idAngles::operator*( const float a ) const
{
	return idAngles( pitch * a, yaw * a, roll * a );
}

ID_INLINE idAngles& idAngles::operator*=( float a )
{
	pitch *= a;
	yaw *= a;
	roll *= a;
	return *this;
}

ID_INLINE idAngles idAngles::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idAngles( pitch * inva, yaw * inva, roll * inva );
}

ID_INLINE idAngles& idAngles::operator/=( float a )
{
	float inva = 1.0f / a;
	pitch *= inva;
	yaw *= inva;
	roll *= inva;
	return *this;
}

//! Returns a new idAngles instance with each angle component scaled by the given float value.
ID_INLINE idAngles operator*( const float a, const idAngles& b )
{
	return idAngles( a * b.pitch, a * b.yaw, a * b.roll );
}

ID_INLINE bool idAngles::Compare( const idAngles& a ) const
{
	return ( ( a.pitch == pitch ) && ( a.yaw == yaw ) && ( a.roll == roll ) );
}

ID_INLINE bool idAngles::Compare( const idAngles& a, const float epsilon ) const
{
	if( idMath::Fabs( pitch - a.pitch ) > epsilon ) { return false; }

	if( idMath::Fabs( yaw - a.yaw ) > epsilon ) { return false; }

	if( idMath::Fabs( roll - a.roll ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idAngles::operator==( const idAngles& a ) const
{
	return Compare( a );
}

ID_INLINE bool idAngles::operator!=( const idAngles& a ) const
{
	return !Compare( a );
}

ID_INLINE void idAngles::Clamp( const idAngles& min, const idAngles& max )
{
	if( pitch < min.pitch ) {
		pitch = min.pitch;
	} else if( pitch > max.pitch ) {
		pitch = max.pitch;
	}
	if( yaw < min.yaw ) {
		yaw = min.yaw;
	} else if( yaw > max.yaw ) {
		yaw = max.yaw;
	}
	if( roll < min.roll ) {
		roll = min.roll;
	} else if( roll > max.roll ) {
		roll = max.roll;
	}
}

ID_INLINE int idAngles::GetDimension() const
{
	return 3;
}

ID_INLINE const float* idAngles::ToFloatPtr() const
{
	return &pitch;
}

ID_INLINE float* idAngles::ToFloatPtr()
{
	return &pitch;
}

#endif /* !__MATH_ANGLES_H__ */
