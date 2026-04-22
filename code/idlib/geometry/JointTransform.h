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

#ifndef __JOINTTRANSFORM_H__
#define __JOINTTRANSFORM_H__

/*!
	\class idJointQuat
	\brief A class representing quaternion data for joint transformations in a skeletal animation system.
*/
class idJointQuat
{
public:
	//! Returns a const float pointer to the internal quaternion data.
	const float* ToFloatPtr() const { return q.ToFloatPtr(); }

	//! Returns a pointer to the floating-point representation of the joint quaternion.
	float*		 ToFloatPtr() { return q.ToFloatPtr(); }

	idQuat		 q;
	idVec3		 t;
	float		 w;
};

// offsets for SIMD code
#define JOINTQUAT_SIZE		 ( 8 * 4 ) // sizeof( idJointQuat )
#define JOINTQUAT_SIZE_SHIFT 5		   // log2( sizeof( idJointQuat ) )
#define JOINTQUAT_Q_OFFSET	 ( 0 * 4 ) // offsetof( idJointQuat, q )
#define JOINTQUAT_T_OFFSET	 ( 4 * 4 ) // offsetof( idJointQuat, t )

assert_sizeof( idJointQuat, JOINTQUAT_SIZE );
assert_sizeof( idJointQuat, ( 1 << JOINTQUAT_SIZE_SHIFT ) );
assert_offsetof( idJointQuat, q, JOINTQUAT_Q_OFFSET );
assert_offsetof( idJointQuat, t, JOINTQUAT_T_OFFSET );

/*!
	\class idJointMat
	\brief A joint transformation matrix class that encapsulates 3D rotation and translation operations.

	This class represents a joint transformation matrix that combines rotational and translational components for 3D transformations. It provides methods for setting and retrieving rotation and
   translation components, applying transformations to vectors, and performing matrix operations such as multiplication, inversion, and comparison. The class supports conversion between different
   matrix representations including 3x3, 4x4, and quaternion formats. It is designed for efficient manipulation of joint transformations in animation and 3D graphics applications, with inline methods
   for performance-critical operations. The matrix maintains a 3x3 rotation component and a 3D translation component, supporting both direct and transformed vector operations.

*/
class idJointMat
{
public:
	//! Sets the rotational component of the joint matrix from a 3x3 matrix.
	void		 SetRotation( const idMat3& m );

	//! Returns the rotation component of the joint matrix as an idMat3.
	idMat3		 GetRotation() const;

	//! Sets the translation component of the joint matrix from the provided 3D vector.
	void		 SetTranslation( const idVec3& t );

	//! Returns the translation component from the joint matrix.
	idVec3		 GetTranslation() const;

	//! Applies the rotational component of the joint matrix to the given vector.
	idVec3		 operator*( const idVec3& v ) const;

	//! Multiplies this joint matrix by a 4D vector and returns the resulting 3D vector.
	idVec3		 operator*( const idVec4& v ) const;

	//! Multiplies this joint matrix by another joint matrix in place
	idJointMat&	 operator*=( const idJointMat& a );

	//! Divides this joint matrix by another joint matrix in place.
	idJointMat&	 operator/=( const idJointMat& a );

	//! Compares this joint matrix with another for exact equality.
	bool		 Compare( const idJointMat& a ) const;

	//! Compares this joint matrix with another using the specified epsilon tolerance
	bool		 Compare( const idJointMat& a, const float epsilon ) const;

	//! Compares two idJointMat objects for equality.
	bool		 operator==( const idJointMat& a ) const;

	//! Compares two idJointMat objects for inequality.
	bool		 operator!=( const idJointMat& a ) const;

	//! Sets the joint matrix to the identity matrix.
	void		 Identity();

	//! Inverts the joint matrix by negating and rotating the translation part and transposing the rotation part.
	void		 Invert();

	//! Initializes the joint matrix from a 4x4 matrix.
	void		 FromMat4( const idMat4& m );

	//! Converts a joint matrix to a 3x3 matrix representation.
	idMat3		 ToMat3() const;

	//! Converts the joint matrix to a 4x4 matrix representation.
	idMat4		 ToMat4() const;

	//! Converts the translation component of the joint matrix to a vector.
	idVec3		 ToVec3() const;

	//! Returns a pointer to the float array representation of the joint matrix.
	const float* ToFloatPtr() const { return mat; }

	//! Returns a pointer to the float array representing the joint matrix data.
	float*		 ToFloatPtr() { return mat; }

	//! Converts a joint matrix to a joint quaternion representation.
	idJointQuat	 ToJointQuat() const;

	//! Transforms a 3D vector using the joint matrix.
	void		 Transform( idVec3& result, const idVec3& v ) const;

	//! Rotates a vector using the joint matrix.
	void		 Rotate( idVec3& result, const idVec3& v ) const;

	//! Multiplies each element of the input matrix by a scalar value and stores the result in the output matrix.
	static void	 Mul( idJointMat& result, const idJointMat& mat, const float s );

	//! Computes the matrix addition of result and the scalar multiplication of mat and s, storing the result in result.
	static void	 Mad( idJointMat& result, const idJointMat& mat, const float s );

	//! Multiplies two joint transformation matrices and stores the result in a third matrix.
	static void	 Multiply( idJointMat& result, const idJointMat& m1, const idJointMat& m2 );

	//! Performs inverse multiplication of two joint matrices and stores the result
	static void	 InverseMultiply( idJointMat& result, const idJointMat& m1, const idJointMat& m2 );

	float		 mat[3 * 4];
};

// offsets for SIMD code
#define JOINTMAT_SIZE ( 4 * 3 * 4 ) // sizeof( idJointMat )
assert_sizeof( idJointMat, JOINTMAT_SIZE );

#define JOINTMAT_TYPESIZE ( 4 * 3 )
ID_INLINE void idJointMat::SetRotation( const idMat3& m )
{
	// NOTE: idMat3 is transposed because it is column-major
	mat[0 * 4 + 0] = m[0][0];
	mat[0 * 4 + 1] = m[1][0];
	mat[0 * 4 + 2] = m[2][0];
	mat[1 * 4 + 0] = m[0][1];
	mat[1 * 4 + 1] = m[1][1];
	mat[1 * 4 + 2] = m[2][1];
	mat[2 * 4 + 0] = m[0][2];
	mat[2 * 4 + 1] = m[1][2];
	mat[2 * 4 + 2] = m[2][2];
}

ID_INLINE idMat3 idJointMat::GetRotation() const
{
	idMat3 m;
	m[0][0] = mat[0 * 4 + 0];
	m[1][0] = mat[0 * 4 + 1];
	m[2][0] = mat[0 * 4 + 2];
	m[0][1] = mat[1 * 4 + 0];
	m[1][1] = mat[1 * 4 + 1];
	m[2][1] = mat[1 * 4 + 2];
	m[0][2] = mat[2 * 4 + 0];
	m[1][2] = mat[2 * 4 + 1];
	m[2][2] = mat[2 * 4 + 2];
	return m;
}

ID_INLINE void idJointMat::SetTranslation( const idVec3& t )
{
	mat[0 * 4 + 3] = t[0];
	mat[1 * 4 + 3] = t[1];
	mat[2 * 4 + 3] = t[2];
}

ID_INLINE idVec3 idJointMat::GetTranslation() const
{
	idVec3 t;
	t[0] = mat[0 * 4 + 3];
	t[1] = mat[1 * 4 + 3];
	t[2] = mat[2 * 4 + 3];
	return t;
}

ID_INLINE idVec3 idJointMat::operator*( const idVec3& v ) const
{
	return idVec3( mat[0 * 4 + 0] * v[0] + mat[0 * 4 + 1] * v[1] + mat[0 * 4 + 2] * v[2],
		mat[1 * 4 + 0] * v[0] + mat[1 * 4 + 1] * v[1] + mat[1 * 4 + 2] * v[2],
		mat[2 * 4 + 0] * v[0] + mat[2 * 4 + 1] * v[1] + mat[2 * 4 + 2] * v[2] );
}

ID_INLINE idVec3 idJointMat::operator*( const idVec4& v ) const
{
	return idVec3( mat[0 * 4 + 0] * v[0] + mat[0 * 4 + 1] * v[1] + mat[0 * 4 + 2] * v[2] + mat[0 * 4 + 3] * v[3],
		mat[1 * 4 + 0] * v[0] + mat[1 * 4 + 1] * v[1] + mat[1 * 4 + 2] * v[2] + mat[1 * 4 + 3] * v[3],
		mat[2 * 4 + 0] * v[0] + mat[2 * 4 + 1] * v[1] + mat[2 * 4 + 2] * v[2] + mat[2 * 4 + 3] * v[3] );
}

ID_INLINE idJointMat& idJointMat::operator*=( const idJointMat& a )
{
	float tmp[3];

	tmp[0]		   = mat[0 * 4 + 0] * a.mat[0 * 4 + 0] + mat[1 * 4 + 0] * a.mat[0 * 4 + 1] + mat[2 * 4 + 0] * a.mat[0 * 4 + 2];
	tmp[1]		   = mat[0 * 4 + 0] * a.mat[1 * 4 + 0] + mat[1 * 4 + 0] * a.mat[1 * 4 + 1] + mat[2 * 4 + 0] * a.mat[1 * 4 + 2];
	tmp[2]		   = mat[0 * 4 + 0] * a.mat[2 * 4 + 0] + mat[1 * 4 + 0] * a.mat[2 * 4 + 1] + mat[2 * 4 + 0] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 0] = tmp[0];
	mat[1 * 4 + 0] = tmp[1];
	mat[2 * 4 + 0] = tmp[2];

	tmp[0]		   = mat[0 * 4 + 1] * a.mat[0 * 4 + 0] + mat[1 * 4 + 1] * a.mat[0 * 4 + 1] + mat[2 * 4 + 1] * a.mat[0 * 4 + 2];
	tmp[1]		   = mat[0 * 4 + 1] * a.mat[1 * 4 + 0] + mat[1 * 4 + 1] * a.mat[1 * 4 + 1] + mat[2 * 4 + 1] * a.mat[1 * 4 + 2];
	tmp[2]		   = mat[0 * 4 + 1] * a.mat[2 * 4 + 0] + mat[1 * 4 + 1] * a.mat[2 * 4 + 1] + mat[2 * 4 + 1] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 1] = tmp[0];
	mat[1 * 4 + 1] = tmp[1];
	mat[2 * 4 + 1] = tmp[2];

	tmp[0]		   = mat[0 * 4 + 2] * a.mat[0 * 4 + 0] + mat[1 * 4 + 2] * a.mat[0 * 4 + 1] + mat[2 * 4 + 2] * a.mat[0 * 4 + 2];
	tmp[1]		   = mat[0 * 4 + 2] * a.mat[1 * 4 + 0] + mat[1 * 4 + 2] * a.mat[1 * 4 + 1] + mat[2 * 4 + 2] * a.mat[1 * 4 + 2];
	tmp[2]		   = mat[0 * 4 + 2] * a.mat[2 * 4 + 0] + mat[1 * 4 + 2] * a.mat[2 * 4 + 1] + mat[2 * 4 + 2] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 2] = tmp[0];
	mat[1 * 4 + 2] = tmp[1];
	mat[2 * 4 + 2] = tmp[2];

	tmp[0]		   = mat[0 * 4 + 3] * a.mat[0 * 4 + 0] + mat[1 * 4 + 3] * a.mat[0 * 4 + 1] + mat[2 * 4 + 3] * a.mat[0 * 4 + 2];
	tmp[1]		   = mat[0 * 4 + 3] * a.mat[1 * 4 + 0] + mat[1 * 4 + 3] * a.mat[1 * 4 + 1] + mat[2 * 4 + 3] * a.mat[1 * 4 + 2];
	tmp[2]		   = mat[0 * 4 + 3] * a.mat[2 * 4 + 0] + mat[1 * 4 + 3] * a.mat[2 * 4 + 1] + mat[2 * 4 + 3] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 3] = tmp[0];
	mat[1 * 4 + 3] = tmp[1];
	mat[2 * 4 + 3] = tmp[2];

	mat[0 * 4 + 3] += a.mat[0 * 4 + 3];
	mat[1 * 4 + 3] += a.mat[1 * 4 + 3];
	mat[2 * 4 + 3] += a.mat[2 * 4 + 3];

	return *this;
}

ID_INLINE idJointMat& idJointMat::operator/=( const idJointMat& a )
{
	float tmp[3];

	mat[0 * 4 + 3] -= a.mat[0 * 4 + 3];
	mat[1 * 4 + 3] -= a.mat[1 * 4 + 3];
	mat[2 * 4 + 3] -= a.mat[2 * 4 + 3];

	tmp[0]		   = mat[0 * 4 + 0] * a.mat[0 * 4 + 0] + mat[1 * 4 + 0] * a.mat[1 * 4 + 0] + mat[2 * 4 + 0] * a.mat[2 * 4 + 0];
	tmp[1]		   = mat[0 * 4 + 0] * a.mat[0 * 4 + 1] + mat[1 * 4 + 0] * a.mat[1 * 4 + 1] + mat[2 * 4 + 0] * a.mat[2 * 4 + 1];
	tmp[2]		   = mat[0 * 4 + 0] * a.mat[0 * 4 + 2] + mat[1 * 4 + 0] * a.mat[1 * 4 + 2] + mat[2 * 4 + 0] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 0] = tmp[0];
	mat[1 * 4 + 0] = tmp[1];
	mat[2 * 4 + 0] = tmp[2];

	tmp[0]		   = mat[0 * 4 + 1] * a.mat[0 * 4 + 0] + mat[1 * 4 + 1] * a.mat[1 * 4 + 0] + mat[2 * 4 + 1] * a.mat[2 * 4 + 0];
	tmp[1]		   = mat[0 * 4 + 1] * a.mat[0 * 4 + 1] + mat[1 * 4 + 1] * a.mat[1 * 4 + 1] + mat[2 * 4 + 1] * a.mat[2 * 4 + 1];
	tmp[2]		   = mat[0 * 4 + 1] * a.mat[0 * 4 + 2] + mat[1 * 4 + 1] * a.mat[1 * 4 + 2] + mat[2 * 4 + 1] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 1] = tmp[0];
	mat[1 * 4 + 1] = tmp[1];
	mat[2 * 4 + 1] = tmp[2];

	tmp[0]		   = mat[0 * 4 + 2] * a.mat[0 * 4 + 0] + mat[1 * 4 + 2] * a.mat[1 * 4 + 0] + mat[2 * 4 + 2] * a.mat[2 * 4 + 0];
	tmp[1]		   = mat[0 * 4 + 2] * a.mat[0 * 4 + 1] + mat[1 * 4 + 2] * a.mat[1 * 4 + 1] + mat[2 * 4 + 2] * a.mat[2 * 4 + 1];
	tmp[2]		   = mat[0 * 4 + 2] * a.mat[0 * 4 + 2] + mat[1 * 4 + 2] * a.mat[1 * 4 + 2] + mat[2 * 4 + 2] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 2] = tmp[0];
	mat[1 * 4 + 2] = tmp[1];
	mat[2 * 4 + 2] = tmp[2];

	tmp[0]		   = mat[0 * 4 + 3] * a.mat[0 * 4 + 0] + mat[1 * 4 + 3] * a.mat[1 * 4 + 0] + mat[2 * 4 + 3] * a.mat[2 * 4 + 0];
	tmp[1]		   = mat[0 * 4 + 3] * a.mat[0 * 4 + 1] + mat[1 * 4 + 3] * a.mat[1 * 4 + 1] + mat[2 * 4 + 3] * a.mat[2 * 4 + 1];
	tmp[2]		   = mat[0 * 4 + 3] * a.mat[0 * 4 + 2] + mat[1 * 4 + 3] * a.mat[1 * 4 + 2] + mat[2 * 4 + 3] * a.mat[2 * 4 + 2];
	mat[0 * 4 + 3] = tmp[0];
	mat[1 * 4 + 3] = tmp[1];
	mat[2 * 4 + 3] = tmp[2];

	return *this;
}

ID_INLINE bool idJointMat::Compare( const idJointMat& a ) const
{
	int i;

	for( i = 0; i < 12; i++ ) {
		if( mat[i] != a.mat[i] ) { return false; }
	}
	return true;
}

/*
========================
idJointMat::Compare
========================
*/
ID_INLINE bool idJointMat::Compare( const idJointMat& a, const float epsilon ) const
{
	int i;

	for( i = 0; i < 12; i++ ) {
		if( idMath::Fabs( mat[i] - a.mat[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idJointMat::operator==( const idJointMat& a ) const
{
	return Compare( a );
}

ID_INLINE bool idJointMat::operator!=( const idJointMat& a ) const
{
	return !Compare( a );
}

ID_INLINE void idJointMat::Identity()
{
	mat[0 * 4 + 0] = 1.0f;
	mat[0 * 4 + 1] = 0.0f;
	mat[0 * 4 + 2] = 0.0f;
	mat[0 * 4 + 3] = 0.0f;
	mat[1 * 4 + 0] = 0.0f;
	mat[1 * 4 + 1] = 1.0f;
	mat[1 * 4 + 2] = 0.0f;
	mat[1 * 4 + 3] = 0.0f;
	mat[2 * 4 + 0] = 0.0f;
	mat[2 * 4 + 1] = 0.0f;
	mat[2 * 4 + 2] = 1.0f;
	mat[2 * 4 + 3] = 0.0f;
}

ID_INLINE void idJointMat::Invert()
{
	float tmp[3];

	// negate inverse rotated translation part
	tmp[0]		   = mat[0 * 4 + 0] * mat[0 * 4 + 3] + mat[1 * 4 + 0] * mat[1 * 4 + 3] + mat[2 * 4 + 0] * mat[2 * 4 + 3];
	tmp[1]		   = mat[0 * 4 + 1] * mat[0 * 4 + 3] + mat[1 * 4 + 1] * mat[1 * 4 + 3] + mat[2 * 4 + 1] * mat[2 * 4 + 3];
	tmp[2]		   = mat[0 * 4 + 2] * mat[0 * 4 + 3] + mat[1 * 4 + 2] * mat[1 * 4 + 3] + mat[2 * 4 + 2] * mat[2 * 4 + 3];
	mat[0 * 4 + 3] = -tmp[0];
	mat[1 * 4 + 3] = -tmp[1];
	mat[2 * 4 + 3] = -tmp[2];

	// transpose rotation part
	tmp[0]		   = mat[0 * 4 + 1];
	mat[0 * 4 + 1] = mat[1 * 4 + 0];
	mat[1 * 4 + 0] = tmp[0];
	tmp[1]		   = mat[0 * 4 + 2];
	mat[0 * 4 + 2] = mat[2 * 4 + 0];
	mat[2 * 4 + 0] = tmp[1];
	tmp[2]		   = mat[1 * 4 + 2];
	mat[1 * 4 + 2] = mat[2 * 4 + 1];
	mat[2 * 4 + 1] = tmp[2];
}

ID_INLINE idMat3 idJointMat::ToMat3() const
{
	return idMat3( mat[0 * 4 + 0], mat[1 * 4 + 0], mat[2 * 4 + 0], mat[0 * 4 + 1], mat[1 * 4 + 1], mat[2 * 4 + 1], mat[0 * 4 + 2], mat[1 * 4 + 2], mat[2 * 4 + 2] );
}

ID_INLINE idMat4 idJointMat::ToMat4() const
{
	return idMat4( mat[0 * 4 + 0],
		mat[0 * 4 + 1],
		mat[0 * 4 + 2],
		mat[0 * 4 + 3],
		mat[1 * 4 + 0],
		mat[1 * 4 + 1],
		mat[1 * 4 + 2],
		mat[1 * 4 + 3],
		mat[2 * 4 + 0],
		mat[2 * 4 + 1],
		mat[2 * 4 + 2],
		mat[2 * 4 + 3],
		0.0f,
		0.0f,
		0.0f,
		1.0f );
}

ID_INLINE void idJointMat::FromMat4( const idMat4& m )
{
	mat[0 * 4 + 0] = m[0][0], mat[0 * 4 + 1] = m[0][1], mat[0 * 4 + 2] = m[0][2], mat[0 * 4 + 3] = m[0][3];
	mat[1 * 4 + 0] = m[1][0], mat[1 * 4 + 1] = m[1][1], mat[1 * 4 + 2] = m[1][2], mat[1 * 4 + 3] = m[1][3];
	mat[2 * 4 + 0] = m[2][0], mat[2 * 4 + 1] = m[2][1], mat[2 * 4 + 2] = m[2][2], mat[2 * 4 + 3] = m[2][3];
	assert( m[3][0] == 0.0f );
	assert( m[3][1] == 0.0f );
	assert( m[3][2] == 0.0f );
	assert( m[3][3] == 1.0f );
}

ID_INLINE idVec3 idJointMat::ToVec3() const
{
	return idVec3( mat[0 * 4 + 3], mat[1 * 4 + 3], mat[2 * 4 + 3] );
}

ID_INLINE void idJointMat::Transform( idVec3& result, const idVec3& v ) const
{
	result.x = mat[0 * 4 + 0] * v.x + mat[0 * 4 + 1] * v.y + mat[0 * 4 + 2] * v.z + mat[0 * 4 + 3];
	result.y = mat[1 * 4 + 0] * v.x + mat[1 * 4 + 1] * v.y + mat[1 * 4 + 2] * v.z + mat[1 * 4 + 3];
	result.z = mat[2 * 4 + 0] * v.x + mat[2 * 4 + 1] * v.y + mat[2 * 4 + 2] * v.z + mat[2 * 4 + 3];
}

ID_INLINE void idJointMat::Rotate( idVec3& result, const idVec3& v ) const
{
	result.x = mat[0 * 4 + 0] * v.x + mat[0 * 4 + 1] * v.y + mat[0 * 4 + 2] * v.z;
	result.y = mat[1 * 4 + 0] * v.x + mat[1 * 4 + 1] * v.y + mat[1 * 4 + 2] * v.z;
	result.z = mat[2 * 4 + 0] * v.x + mat[2 * 4 + 1] * v.y + mat[2 * 4 + 2] * v.z;
}

ID_INLINE void idJointMat::Mul( idJointMat& result, const idJointMat& mat, const float s )
{
	result.mat[0 * 4 + 0] = s * mat.mat[0 * 4 + 0];
	result.mat[0 * 4 + 1] = s * mat.mat[0 * 4 + 1];
	result.mat[0 * 4 + 2] = s * mat.mat[0 * 4 + 2];
	result.mat[0 * 4 + 3] = s * mat.mat[0 * 4 + 3];
	result.mat[1 * 4 + 0] = s * mat.mat[1 * 4 + 0];
	result.mat[1 * 4 + 1] = s * mat.mat[1 * 4 + 1];
	result.mat[1 * 4 + 2] = s * mat.mat[1 * 4 + 2];
	result.mat[1 * 4 + 3] = s * mat.mat[1 * 4 + 3];
	result.mat[2 * 4 + 0] = s * mat.mat[2 * 4 + 0];
	result.mat[2 * 4 + 1] = s * mat.mat[2 * 4 + 1];
	result.mat[2 * 4 + 2] = s * mat.mat[2 * 4 + 2];
	result.mat[2 * 4 + 3] = s * mat.mat[2 * 4 + 3];
}

ID_INLINE void idJointMat::Mad( idJointMat& result, const idJointMat& mat, const float s )
{
	result.mat[0 * 4 + 0] += s * mat.mat[0 * 4 + 0];
	result.mat[0 * 4 + 1] += s * mat.mat[0 * 4 + 1];
	result.mat[0 * 4 + 2] += s * mat.mat[0 * 4 + 2];
	result.mat[0 * 4 + 3] += s * mat.mat[0 * 4 + 3];
	result.mat[1 * 4 + 0] += s * mat.mat[1 * 4 + 0];
	result.mat[1 * 4 + 1] += s * mat.mat[1 * 4 + 1];
	result.mat[1 * 4 + 2] += s * mat.mat[1 * 4 + 2];
	result.mat[1 * 4 + 3] += s * mat.mat[1 * 4 + 3];
	result.mat[2 * 4 + 0] += s * mat.mat[2 * 4 + 0];
	result.mat[2 * 4 + 1] += s * mat.mat[2 * 4 + 1];
	result.mat[2 * 4 + 2] += s * mat.mat[2 * 4 + 2];
	result.mat[2 * 4 + 3] += s * mat.mat[2 * 4 + 3];
}

ID_INLINE void idJointMat::Multiply( idJointMat& result, const idJointMat& m1, const idJointMat& m2 )
{
	result.mat[0 * 4 + 0] = m1.mat[0 * 4 + 0] * m2.mat[0 * 4 + 0] + m1.mat[0 * 4 + 1] * m2.mat[1 * 4 + 0] + m1.mat[0 * 4 + 2] * m2.mat[2 * 4 + 0];
	result.mat[0 * 4 + 1] = m1.mat[0 * 4 + 0] * m2.mat[0 * 4 + 1] + m1.mat[0 * 4 + 1] * m2.mat[1 * 4 + 1] + m1.mat[0 * 4 + 2] * m2.mat[2 * 4 + 1];
	result.mat[0 * 4 + 2] = m1.mat[0 * 4 + 0] * m2.mat[0 * 4 + 2] + m1.mat[0 * 4 + 1] * m2.mat[1 * 4 + 2] + m1.mat[0 * 4 + 2] * m2.mat[2 * 4 + 2];
	result.mat[0 * 4 + 3] = m1.mat[0 * 4 + 0] * m2.mat[0 * 4 + 3] + m1.mat[0 * 4 + 1] * m2.mat[1 * 4 + 3] + m1.mat[0 * 4 + 2] * m2.mat[2 * 4 + 3] + m1.mat[0 * 4 + 3];

	result.mat[1 * 4 + 0] = m1.mat[1 * 4 + 0] * m2.mat[0 * 4 + 0] + m1.mat[1 * 4 + 1] * m2.mat[1 * 4 + 0] + m1.mat[1 * 4 + 2] * m2.mat[2 * 4 + 0];
	result.mat[1 * 4 + 1] = m1.mat[1 * 4 + 0] * m2.mat[0 * 4 + 1] + m1.mat[1 * 4 + 1] * m2.mat[1 * 4 + 1] + m1.mat[1 * 4 + 2] * m2.mat[2 * 4 + 1];
	result.mat[1 * 4 + 2] = m1.mat[1 * 4 + 0] * m2.mat[0 * 4 + 2] + m1.mat[1 * 4 + 1] * m2.mat[1 * 4 + 2] + m1.mat[1 * 4 + 2] * m2.mat[2 * 4 + 2];
	result.mat[1 * 4 + 3] = m1.mat[1 * 4 + 0] * m2.mat[0 * 4 + 3] + m1.mat[1 * 4 + 1] * m2.mat[1 * 4 + 3] + m1.mat[1 * 4 + 2] * m2.mat[2 * 4 + 3] + m1.mat[1 * 4 + 3];

	result.mat[2 * 4 + 0] = m1.mat[2 * 4 + 0] * m2.mat[0 * 4 + 0] + m1.mat[2 * 4 + 1] * m2.mat[1 * 4 + 0] + m1.mat[2 * 4 + 2] * m2.mat[2 * 4 + 0];
	result.mat[2 * 4 + 1] = m1.mat[2 * 4 + 0] * m2.mat[0 * 4 + 1] + m1.mat[2 * 4 + 1] * m2.mat[1 * 4 + 1] + m1.mat[2 * 4 + 2] * m2.mat[2 * 4 + 1];
	result.mat[2 * 4 + 2] = m1.mat[2 * 4 + 0] * m2.mat[0 * 4 + 2] + m1.mat[2 * 4 + 1] * m2.mat[1 * 4 + 2] + m1.mat[2 * 4 + 2] * m2.mat[2 * 4 + 2];
	result.mat[2 * 4 + 3] = m1.mat[2 * 4 + 0] * m2.mat[0 * 4 + 3] + m1.mat[2 * 4 + 1] * m2.mat[1 * 4 + 3] + m1.mat[2 * 4 + 2] * m2.mat[2 * 4 + 3] + m1.mat[2 * 4 + 3];
}

ID_INLINE void idJointMat::InverseMultiply( idJointMat& result, const idJointMat& m1, const idJointMat& m2 )
{
	float dst[3];

	result.mat[0 * 4 + 0] = m1.mat[0 * 4 + 0] * m2.mat[0 * 4 + 0] + m1.mat[0 * 4 + 1] * m2.mat[0 * 4 + 1] + m1.mat[0 * 4 + 2] * m2.mat[0 * 4 + 2];
	result.mat[0 * 4 + 1] = m1.mat[0 * 4 + 0] * m2.mat[1 * 4 + 0] + m1.mat[0 * 4 + 1] * m2.mat[1 * 4 + 1] + m1.mat[0 * 4 + 2] * m2.mat[1 * 4 + 2];
	result.mat[0 * 4 + 2] = m1.mat[0 * 4 + 0] * m2.mat[2 * 4 + 0] + m1.mat[0 * 4 + 1] * m2.mat[2 * 4 + 1] + m1.mat[0 * 4 + 2] * m2.mat[2 * 4 + 2];

	result.mat[1 * 4 + 0] = m1.mat[1 * 4 + 0] * m2.mat[0 * 4 + 0] + m1.mat[1 * 4 + 1] * m2.mat[0 * 4 + 1] + m1.mat[1 * 4 + 2] * m2.mat[0 * 4 + 2];
	result.mat[1 * 4 + 1] = m1.mat[1 * 4 + 0] * m2.mat[1 * 4 + 0] + m1.mat[1 * 4 + 1] * m2.mat[1 * 4 + 1] + m1.mat[1 * 4 + 2] * m2.mat[1 * 4 + 2];
	result.mat[1 * 4 + 2] = m1.mat[1 * 4 + 0] * m2.mat[2 * 4 + 0] + m1.mat[1 * 4 + 1] * m2.mat[2 * 4 + 1] + m1.mat[1 * 4 + 2] * m2.mat[2 * 4 + 2];

	result.mat[2 * 4 + 0] = m1.mat[2 * 4 + 0] * m2.mat[0 * 4 + 0] + m1.mat[2 * 4 + 1] * m2.mat[0 * 4 + 1] + m1.mat[2 * 4 + 2] * m2.mat[0 * 4 + 2];
	result.mat[2 * 4 + 1] = m1.mat[2 * 4 + 0] * m2.mat[1 * 4 + 0] + m1.mat[2 * 4 + 1] * m2.mat[1 * 4 + 1] + m1.mat[2 * 4 + 2] * m2.mat[1 * 4 + 2];
	result.mat[2 * 4 + 2] = m1.mat[2 * 4 + 0] * m2.mat[2 * 4 + 0] + m1.mat[2 * 4 + 1] * m2.mat[2 * 4 + 1] + m1.mat[2 * 4 + 2] * m2.mat[2 * 4 + 2];

	dst[0] = -( m2.mat[0 * 4 + 0] * m2.mat[0 * 4 + 3] + m2.mat[1 * 4 + 0] * m2.mat[1 * 4 + 3] + m2.mat[2 * 4 + 0] * m2.mat[2 * 4 + 3] );
	dst[1] = -( m2.mat[0 * 4 + 1] * m2.mat[0 * 4 + 3] + m2.mat[1 * 4 + 1] * m2.mat[1 * 4 + 3] + m2.mat[2 * 4 + 1] * m2.mat[2 * 4 + 3] );
	dst[2] = -( m2.mat[0 * 4 + 2] * m2.mat[0 * 4 + 3] + m2.mat[1 * 4 + 2] * m2.mat[1 * 4 + 3] + m2.mat[2 * 4 + 2] * m2.mat[2 * 4 + 3] );

	result.mat[0 * 4 + 3] = m1.mat[0 * 4 + 0] * dst[0] + m1.mat[0 * 4 + 1] * dst[1] + m1.mat[0 * 4 + 2] * dst[2] + m1.mat[0 * 4 + 3];
	result.mat[1 * 4 + 3] = m1.mat[1 * 4 + 0] * dst[0] + m1.mat[1 * 4 + 1] * dst[1] + m1.mat[1 * 4 + 2] * dst[2] + m1.mat[1 * 4 + 3];
	result.mat[2 * 4 + 3] = m1.mat[2 * 4 + 0] * dst[0] + m1.mat[2 * 4 + 1] * dst[1] + m1.mat[2 * 4 + 2] * dst[2] + m1.mat[2 * 4 + 3];
}

#endif /* !__JOINTTRANSFORM_H__ */
