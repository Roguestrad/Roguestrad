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

/*
===============================================================================

  Joint Quaternion

===============================================================================
*/

class idJointQuat
{
public:
	//! Returns a pointer to the float representation of the joint quaternion
	const float* ToFloatPtr() const { return q.ToFloatPtr(); }

	//! Returns a float pointer to the internal quaternion data.
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

/*
===============================================================================

	Joint Matrix

===============================================================================
*/

/*
================================================
idJointMat has the following structure:

	idMat3 m;
	idVec3 t;

	m[0][0], m[1][0], m[2][0], t[0]
	m[0][1], m[1][1], m[2][1], t[1]
	m[0][2], m[1][2], m[2][2], t[2]

================================================
*/
class idJointMat
{
public:
	/*!
		\brief Sets the rotation component of the joint matrix from a 3x3 matrix

		The function copies the elements of the provided 3x3 matrix into the rotation submatrix of the joint matrix. The input matrix is interpreted as column-major, which is consistent with the
	   internal storage format of idMat3. The transformation accounts for the difference in matrix layout between the input and the joint matrix representation.

		\param m The 3x3 matrix containing the rotation component to be set
	*/
	void		 SetRotation( const idMat3& m );

	//! Returns the rotation component of the joint matrix as a 3x3 matrix.
	idMat3		 GetRotation() const;

	/*!
		\brief Sets the translation component of the joint matrix from the provided vector.

		This function updates the translation part of the joint matrix by assigning the x, y, and z components of the input vector to the appropriate positions in the matrix. The matrix is assumed to
	   be a 4x4 transformation matrix where the last column contains the translation components.

		\param t The translation vector to set in the matrix
	*/
	void		 SetTranslation( const idVec3& t );

	//! Returns the translation component from the joint matrix.
	idVec3		 GetTranslation() const;

	idVec3		 operator*( const idVec3& v ) const; // only rotate
	idVec3		 operator*( const idVec4& v ) const; // rotate and translate

	idJointMat&	 operator*=( const idJointMat& a ); // transform
	idJointMat&	 operator/=( const idJointMat& a ); // untransform

	/*!
		\brief Compares this joint matrix with another joint matrix for equality.

		This function performs an exact comparison between the current joint matrix and the provided joint matrix without using any epsilon value for floating-point comparisons. It checks all 12
	   elements of the matrix and returns true only if all corresponding elements are exactly equal. The comparison is done element by element in a tight loop.

		\param a The joint matrix to compare against this matrix
		\return True if all 12 elements of the matrices are exactly equal, false otherwise
	*/
	bool		 Compare( const idJointMat& a ) const;

	/*!
		\brief Compares this joint matrix with another joint matrix using the specified epsilon tolerance

		This function performs an element-wise comparison of the current joint matrix with another joint matrix using a specified epsilon value for floating point tolerance. It iterates through all 12
	   elements of the matrices and checks if the absolute difference between corresponding elements is greater than the epsilon value. If any difference exceeds the epsilon threshold, the function
	   returns false. Otherwise, it returns true, indicating that the matrices are considered equal within the given tolerance.

		\param a The joint matrix to compare with this matrix
		\param epsilon The epsilon tolerance value for floating point comparison
		\return true if all corresponding elements of the matrices are within the epsilon tolerance, false otherwise
	*/
	bool		 Compare( const idJointMat& a, const float epsilon ) const;
	bool		 operator==( const idJointMat& a ) const; // exact compare, no epsilon
	bool		 operator!=( const idJointMat& a ) const; // exact compare, no epsilon

	/*!
		\brief Sets the joint matrix to the identity matrix.

		This function initializes the joint matrix to represent an identity transformation. It sets the diagonal elements to 1.0f and all off-diagonal elements to 0.0f, resulting in a matrix that
	   represents no transformation. The matrix is assumed to be a 3x4 matrix stored in row-major order.

	*/
	void		 Identity();

	/*!
		\brief Inverts the joint matrix by negating and reordering its translation and rotation components

		This function performs in-place inversion of a joint transformation matrix. It modifies the matrix to represent the inverse transformation. The implementation handles both the translation and
	   rotation components of the 4x4 matrix. First, it negates the translation part after rotating it by the inverse rotation. Then it transposes the 3x3 rotation sub-matrix to complete the inversion
	   operation

	*/
	void		 Invert();

	/*!
		\brief Initializes the joint matrix from a 4x4 matrix

		This function copies the values from a 4x4 matrix into the joint matrix representation. It extracts the first three rows and columns of the input matrix and stores them in the joint matrix.
	   The function also verifies that the fourth row of the input matrix matches the expected values for a homogeneous transformation matrix, specifically that the first three elements are zero and
	   the fourth element is one.

		\param m The 4x4 matrix to convert into a joint matrix
		\throws Assertion failures if the fourth row of the input matrix does not match the expected homogeneous transformation values
	*/
	void		 FromMat4( const idMat4& m );

	//! Converts the joint matrix to a 3x3 matrix representation.
	idMat3		 ToMat3() const;

	//! Converts the joint matrix to a 4x4 matrix representation.
	idMat4		 ToMat4() const;

	//! Converts the translation component of the joint matrix to a vector.
	idVec3		 ToVec3() const;

	//! Returns a pointer to the float array representing the joint matrix data.
	const float* ToFloatPtr() const { return mat; }

	//! Returns a pointer to the float array representation of the joint matrix.
	float*		 ToFloatPtr() { return mat; }

	//! Converts a 3x4 transformation matrix to a joint quaternion representation.
	idJointQuat	 ToJointQuat() const;

	/*!
		\brief Transforms a 3D vector using the joint matrix stored in this object

		This function performs a 3D vector transformation by applying the 4x4 matrix stored in this joint matrix object to the input vector. The transformation includes both rotation and translation
	   components. The result is stored in the provided output vector. This is commonly used in skeletal animation systems where joint transformations need to be applied to vertex positions or other
	   3D coordinates.

		\param result The output vector that will contain the transformed coordinates
		\param v The input vector to be transformed by this joint matrix
	*/
	void		 Transform( idVec3& result, const idVec3& v ) const;

	/*!
		\brief Rotates a 3D vector using the rotation component of the joint matrix

		This function performs a rotation transformation on the input vector v using only the rotational part of the joint matrix, ignoring any translation. The rotation is applied by multiplying the
	   vector with the 3x3 rotation submatrix extracted from the 4x4 joint matrix. The result is stored in the output parameter result

		\param result The output vector that will contain the rotated result
		\param v The input vector to be rotated
	*/
	void		 Rotate( idVec3& result, const idVec3& v ) const;

	/*!
		\brief Performs scalar multiplication of a matrix and stores the result in another matrix

		This function multiplies each element of the input matrix mat by the scalar value s and stores the result in the output matrix result. It operates on all elements of the 3x4 matrix, modifying
	   result in place. The operation is equivalent to result = s * mat for each element of the matrices.

		\param result The matrix to store the result of the scalar multiplication
		\param mat The matrix to be multiplied by the scalar
		\param s The scalar value to multiply with mat
	*/
	static void	 Mul( idJointMat& result, const idJointMat& mat, const float s );

	/*!
		\brief Performs matrix addition with scalar multiplication of mat and s, storing the result in result.

		This function adds the scalar product of matrix mat and scalar s to the matrix result. It operates on all elements of the 3x4 matrix, modifying result in place. The operation is equivalent to
	   result = result + s * mat for each element of the matrices.

		\param result The matrix to store the result of the operation
		\param mat The matrix to be scaled and added to result
		\param s The scalar value to multiply with mat before adding to result
	*/
	static void	 Mad( idJointMat& result, const idJointMat& mat, const float s );

	/*!
		\brief Performs matrix multiplication of two joint matrices and stores the result in a third matrix.

		This function computes the matrix multiplication of two input joint matrices m1 and m2, storing the result in the output matrix result. The implementation handles 3x4 matrices where each
	   element is computed as the dot product of rows from m1 with columns from m2. The function is marked as ID_INLINE, indicating it should be inlined for performance.

		\param result The matrix to store the result of the multiplication
		\param m1 The first matrix operand
		\param m2 The second matrix operand
	*/
	static void	 Multiply( idJointMat& result, const idJointMat& m1, const idJointMat& m2 );

	/*!
		\brief Performs inverse multiplication of two joint matrices and stores the result.

		This function computes the inverse multiplication of two joint matrices m1 and m2, storing the result in the provided result matrix. It handles the transformation of the rotational and
	   translational components of the matrices, where the rotational part is computed through standard matrix multiplication, and the translation part involves additional calculations to account for
	   the inverse transformation. The matrix is treated as a 3x4 transformation matrix where the last column represents translation.

		\param result The matrix to store the result of the inverse multiplication.
		\param m1 The first matrix in the multiplication.
		\param m2 The second matrix in the multiplication.
	*/
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

/*
========================
idJointMat::operator*
========================
*/
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

/*
========================
idJointMat::operator*=
========================
*/
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

/*
========================
idJointMat::operator/=
========================
*/
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

/*
========================
idJointMat::operator==
========================
*/
ID_INLINE bool idJointMat::operator==( const idJointMat& a ) const
{
	return Compare( a );
}

/*
========================
idJointMat::operator!=
========================
*/
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
