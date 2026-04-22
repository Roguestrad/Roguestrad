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

#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

/*
===============================================================================

  Matrix classes, all matrices are row-major except idMat3

===============================================================================
*/

#define MATRIX_INVERSE_EPSILON 1e-14
#define MATRIX_EPSILON		   1e-6

class idAngles;
class idQuat;
class idCQuat;
class idRotation;
class idMat4;

/*!
	\class idMat2
	\brief A 2x2 matrix class supporting various linear algebra operations.

	This class represents a 2x2 matrix and provides comprehensive support for matrix arithmetic, including construction from various data sources, element access, and mathematical operations such as
   addition, subtraction, multiplication, transposition, and inversion. It offers both in-place and non-in-place variants of operations, along with utilities for checking matrix properties such as
   symmetry, diagonal nature, and identity status. The class supports conversion to and from float arrays, and provides mechanisms for comparing matrices with tolerance-based equality checks. It is
   designed for use in 2D geometric computations and linear algebra applications.

*/
class idMat2
{
public:
	//! Constructs an uninitialized 2x2 matrix.
	idMat2();

	//! Constructs a 2x2 matrix from two 2D vectors representing the rows.
	explicit idMat2( const idVec2& x, const idVec2& y );

	//! Constructs a 2x2 matrix with the specified components.
	explicit idMat2( const float xx, const float xy, const float yx, const float yy );

	//! Constructs a 2x2 matrix from a 2D float array.
	explicit idMat2( const float src[2][2] );

	//! Returns a constant reference to the idVec2 at the specified index in the idMat2 matrix.
	const idVec2&  operator[]( int index ) const;

	//! Provides indexed access to the rows of the 2x2 matrix.
	idVec2&		   operator[]( int index );

	//! Returns the negation of this 2x2 matrix.
	idMat2		   operator-() const;

	//! Returns a new matrix with each element multiplied by the given scalar value.
	idMat2		   operator*( const float a ) const;

	//! Returns the product of this 2x2 matrix and the given 2D vector.
	idVec2		   operator*( const idVec2& vec ) const;

	//! Returns the matrix product of this matrix and matrix a.
	idMat2		   operator*( const idMat2& a ) const;

	//! Returns a new 2x2 matrix that is the result of adding this matrix to another matrix.
	idMat2		   operator+( const idMat2& a ) const;

	//! Returns a new matrix that is the result of subtracting the input matrix from this matrix.
	idMat2		   operator-( const idMat2& a ) const;

	//! Multiplies all elements of the matrix by the given scalar value and returns a reference to the matrix.
	idMat2&		   operator*=( const float a );

	//! Multiplies this 2x2 matrix by another 2x2 matrix and assigns the result to this matrix.
	idMat2&		   operator*=( const idMat2& a );

	//! Adds the elements of another 2x2 matrix to this matrix and returns a reference to this matrix.
	idMat2&		   operator+=( const idMat2& a );

	//! Subtracts the elements of the input matrix from this matrix in place and returns a reference to this matrix.
	idMat2&		   operator-=( const idMat2& a );

	friend idMat2  operator*( const float a, const idMat2& mat );
	friend idVec2  operator*( const idVec2& vec, const idMat2& mat );
	friend idVec2& operator*=( idVec2& vec, const idMat2& mat );

	//! Compares this matrix with another matrix for exact equality.
	bool		   Compare( const idMat2& a ) const;

	//! Compares this matrix with another matrix using the specified epsilon value.
	bool		   Compare( const idMat2& a, const float epsilon ) const;

	//! Compares two idMat2 matrices for exact equality.
	bool		   operator==( const idMat2& a ) const;

	//! Returns true if this matrix is not equal to the given matrix.
	bool		   operator!=( const idMat2& a ) const;

	//! Sets all elements of the 2x2 matrix to zero.
	void		   Zero();

	//! Sets the matrix to the 2x2 identity matrix.
	void		   Identity();

	//! Checks if the matrix is equal to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the 2x2 matrix is symmetric within a given epsilon tolerance.
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the 2x2 matrix is diagonal within the specified epsilon tolerance.
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns the trace of the 2x2 matrix, which is the sum of its diagonal elements.
	float		   Trace() const;

	//! Calculates and returns the determinant of the 2x2 matrix.
	float		   Determinant() const;

	//! Returns the transpose of this 2x2 matrix
	idMat2		   Transpose() const;

	//! Transposes the matrix in-place and returns a reference to itself.
	idMat2&		   TransposeSelf();

	//! Returns the inverse of the matrix.
	idMat2		   Inverse() const;

	//! Inverts the 2x2 matrix in place and returns true if successful, false if the determinant is zero
	bool		   InverseSelf();

	//! Returns the inverse of the matrix
	idMat2		   InverseFast() const;

	//! Inverts the matrix in place and returns true if the operation was successful, false if the determinant is zero.
	bool		   InverseFastSelf();

	//! Returns the dimension of the 2x2 matrix.
	int			   GetDimension() const;

	//! Returns a pointer to the float representation of the matrix data
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the float representation of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision.
	const char*	   ToString( int precision = 2 ) const;

private:
	idVec2 mat[2];
};

extern idMat2 mat2_zero;
extern idMat2 mat2_identity;
#define mat2_default mat2_identity

ID_INLINE idMat2::idMat2()
{
}

ID_INLINE idMat2::idMat2( const idVec2& x, const idVec2& y )
{
	mat[0].x = x.x;
	mat[0].y = x.y;
	mat[1].x = y.x;
	mat[1].y = y.y;
}

ID_INLINE idMat2::idMat2( const float xx, const float xy, const float yx, const float yy )
{
	mat[0].x = xx;
	mat[0].y = xy;
	mat[1].x = yx;
	mat[1].y = yy;
}

ID_INLINE idMat2::idMat2( const float src[2][2] )
{
	memcpy( mat, src, 2 * 2 * sizeof( float ) );
}

ID_INLINE const idVec2& idMat2::operator[]( int index ) const
{
	// assert( ( index >= 0 ) && ( index < 2 ) );
	return mat[index];
}

ID_INLINE idVec2& idMat2::operator[]( int index )
{
	// assert( ( index >= 0 ) && ( index < 2 ) );
	return mat[index];
}

ID_INLINE idMat2 idMat2::operator-() const
{
	return idMat2( -mat[0][0], -mat[0][1], -mat[1][0], -mat[1][1] );
}

ID_INLINE idVec2 idMat2::operator*( const idVec2& vec ) const
{
	return idVec2( mat[0].x * vec.x + mat[0].y * vec.y, mat[1].x * vec.x + mat[1].y * vec.y );
}

ID_INLINE idMat2 idMat2::operator*( const idMat2& a ) const
{
	return idMat2( mat[0].x * a[0].x + mat[0].y * a[1].x, mat[0].x * a[0].y + mat[0].y * a[1].y, mat[1].x * a[0].x + mat[1].y * a[1].x, mat[1].x * a[0].y + mat[1].y * a[1].y );
}

ID_INLINE idMat2 idMat2::operator*( const float a ) const
{
	return idMat2( mat[0].x * a, mat[0].y * a, mat[1].x * a, mat[1].y * a );
}

ID_INLINE idMat2 idMat2::operator+( const idMat2& a ) const
{
	return idMat2( mat[0].x + a[0].x, mat[0].y + a[0].y, mat[1].x + a[1].x, mat[1].y + a[1].y );
}

ID_INLINE idMat2 idMat2::operator-( const idMat2& a ) const
{
	return idMat2( mat[0].x - a[0].x, mat[0].y - a[0].y, mat[1].x - a[1].x, mat[1].y - a[1].y );
}

ID_INLINE idMat2& idMat2::operator*=( const float a )
{
	mat[0].x *= a;
	mat[0].y *= a;
	mat[1].x *= a;
	mat[1].y *= a;

	return *this;
}

ID_INLINE idMat2& idMat2::operator*=( const idMat2& a )
{
	float x, y;
	x		 = mat[0].x;
	y		 = mat[0].y;
	mat[0].x = x * a[0].x + y * a[1].x;
	mat[0].y = x * a[0].y + y * a[1].y;
	x		 = mat[1].x;
	y		 = mat[1].y;
	mat[1].x = x * a[0].x + y * a[1].x;
	mat[1].y = x * a[0].y + y * a[1].y;
	return *this;
}

ID_INLINE idMat2& idMat2::operator+=( const idMat2& a )
{
	mat[0].x += a[0].x;
	mat[0].y += a[0].y;
	mat[1].x += a[1].x;
	mat[1].y += a[1].y;

	return *this;
}

ID_INLINE idMat2& idMat2::operator-=( const idMat2& a )
{
	mat[0].x -= a[0].x;
	mat[0].y -= a[0].y;
	mat[1].x -= a[1].x;
	mat[1].y -= a[1].y;

	return *this;
}

//! Multiplies a 2D vector by a 2x2 matrix and returns the resulting vector.
ID_INLINE idVec2 operator*( const idVec2& vec, const idMat2& mat )
{
	return mat * vec;
}

//! Returns the result of multiplying a scalar with a 2x2 matrix.
ID_INLINE idMat2 operator*( const float a, idMat2 const& mat )
{
	return mat * a;
}

//! Multiplies a 2D vector by a 2x2 matrix and assigns the result back to the vector.
ID_INLINE idVec2& operator*=( idVec2& vec, const idMat2& mat )
{
	vec = mat * vec;
	return vec;
}

ID_INLINE bool idMat2::Compare( const idMat2& a ) const
{
	if( mat[0].Compare( a[0] ) && mat[1].Compare( a[1] ) ) { return true; }
	return false;
}

ID_INLINE bool idMat2::Compare( const idMat2& a, const float epsilon ) const
{
	if( mat[0].Compare( a[0], epsilon ) && mat[1].Compare( a[1], epsilon ) ) { return true; }
	return false;
}

ID_INLINE bool idMat2::operator==( const idMat2& a ) const
{
	return Compare( a );
}

ID_INLINE bool idMat2::operator!=( const idMat2& a ) const
{
	return !Compare( a );
}

ID_INLINE void idMat2::Zero()
{
	mat[0].Zero();
	mat[1].Zero();
}

ID_INLINE void idMat2::Identity()
{
	*this = mat2_identity;
}

ID_INLINE bool idMat2::IsIdentity( const float epsilon ) const
{
	return Compare( mat2_identity, epsilon );
}

ID_INLINE bool idMat2::IsSymmetric( const float epsilon ) const
{
	return ( idMath::Fabs( mat[0][1] - mat[1][0] ) < epsilon );
}

ID_INLINE bool idMat2::IsDiagonal( const float epsilon ) const
{
	if( idMath::Fabs( mat[0][1] ) > epsilon || idMath::Fabs( mat[1][0] ) > epsilon ) { return false; }
	return true;
}

ID_INLINE float idMat2::Trace() const
{
	return ( mat[0][0] + mat[1][1] );
}

ID_INLINE float idMat2::Determinant() const
{
	return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

ID_INLINE idMat2 idMat2::Transpose() const
{
	return idMat2( mat[0][0], mat[1][0], mat[0][1], mat[1][1] );
}

ID_INLINE idMat2& idMat2::TransposeSelf()
{
	float tmp;

	tmp		  = mat[0][1];
	mat[0][1] = mat[1][0];
	mat[1][0] = tmp;

	return *this;
}

ID_INLINE idMat2 idMat2::Inverse() const
{
	idMat2 invMat;

	invMat = *this;
	verify( invMat.InverseSelf() );
	return invMat;
}

ID_INLINE idMat2 idMat2::InverseFast() const
{
	idMat2 invMat;

	invMat = *this;
	verify( invMat.InverseFastSelf() );
	return invMat;
}

ID_INLINE int idMat2::GetDimension() const
{
	return 4;
}

ID_INLINE const float* idMat2::ToFloatPtr() const
{
	return mat[0].ToFloatPtr();
}

ID_INLINE float* idMat2::ToFloatPtr()
{
	return mat[0].ToFloatPtr();
}

/*!
	\class idMat3
	\brief A 3x3 matrix class for representing and manipulating 3D transformations.

	This class implements a 3x3 matrix designed for 3D geometric transformations, supporting both column-major storage and common matrix operations. It provides constructors for various initialization
   methods including from vectors, arrays, and individual components. The class supports standard arithmetic operations such as addition, subtraction, scalar multiplication, and matrix multiplication,
   along with in-place modification operators. It includes functionality for matrix decomposition and conversion to other rotational representations including angles, quaternions, and rotation
   objects. Additional methods handle special cases like orthonormalization, transposition, and inversion, with both exact and fast approximation variants. The class is intended for use in 3D graphics
   and physics simulations where efficient matrix manipulation is required.

*/
class idMat3
{
public:
	//! Constructs an uninitialized 3x3 matrix.
	idMat3();

	//! Initializes a 3x3 matrix using three vector components as rows.
	explicit idMat3( const idVec3& x, const idVec3& y, const idVec3& z );

	//! Constructs a 3x3 matrix with the specified components.
	explicit idMat3( const float xx, const float xy, const float xz, const float yx, const float yy, const float yz, const float zx, const float zy, const float zz );

	//! Constructs an idMat3 object from a 3x3 float array.
	explicit idMat3( const float src[3][3] );

	//! Returns a const reference to the idVec3 at the specified index in the matrix.
	const idVec3&  operator[]( int index ) const;

	//! Returns a reference to the row vector at the specified index in the matrix.
	idVec3&		   operator[]( int index );

	//! Returns the negation of this matrix.
	idMat3		   operator-() const;

	//! Returns a new matrix that is the result of multiplying each element of this matrix by the given scalar value.
	idMat3		   operator*( const float a ) const;

	//! Multiplies this 3x3 matrix with the given 3D vector and returns the resulting vector.
	idVec3		   operator*( const idVec3& vec ) const;

	//! Returns the matrix product of this matrix and matrix a.
	idMat3		   operator*( const idMat3& a ) const;

	//! Returns a new matrix that is the sum of this matrix and matrix a.
	idMat3		   operator+( const idMat3& a ) const;

	//! Returns a new matrix that is the result of subtracting the input matrix from this matrix.
	idMat3		   operator-( const idMat3& a ) const;

	//! Multiplies all elements of the matrix by the given scalar value and returns a reference to the matrix.
	idMat3&		   operator*=( const float a );

	//! Multiplies this matrix by the given matrix and assigns the result to this matrix.
	idMat3&		   operator*=( const idMat3& a );

	//! Adds the elements of the given matrix to this matrix and returns a reference to this matrix.
	idMat3&		   operator+=( const idMat3& a );

	//! Subtracts the components of the given matrix from this matrix in place and returns a reference to this matrix.
	idMat3&		   operator-=( const idMat3& a );

	friend idMat3  operator*( const float a, const idMat3& mat );
	friend idVec3  operator*( const idVec3& vec, const idMat3& mat );
	friend idVec3& operator*=( idVec3& vec, const idMat3& mat );

	//! Compares this matrix with another matrix for exact equality.
	bool		   Compare( const idMat3& a ) const;

	//! Compares this matrix with another matrix using the specified epsilon value for floating-point comparisons
	bool		   Compare( const idMat3& a, const float epsilon ) const;

	//! Compares this matrix with another matrix for equality using exact matching.
	bool		   operator==( const idMat3& a ) const;

	//! Returns true if this matrix is not equal to the given matrix.
	bool		   operator!=( const idMat3& a ) const;

	//! Sets all elements of the 3x3 matrix to zero.
	void		   Zero();

	//! Sets the matrix to the 3x3 identity matrix.
	void		   Identity();

	//! Checks if the matrix is approximately equal to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the 3x3 matrix is symmetric within a given epsilon tolerance.
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is diagonal, considering the specified epsilon tolerance.
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is not equal to the identity matrix, indicating a rotation has been applied.
	bool		   IsRotated() const;

	//! Projects a 3D vector using the matrix components.
	void		   ProjectVector( const idVec3& src, idVec3& dst ) const;

	//! Unprojects a 3D vector using the matrix and stores the result in the destination vector.
	void		   UnprojectVector( const idVec3& src, idVec3& dst ) const;

	//! Fixes degenerate axial cases in the matrix by addressing degenerate normals in its rows.
	bool		   FixDegeneracies();

	//! Fixes denormalized floating-point numbers in the matrix by setting tiny values to zero.
	bool		   FixDenormals();

	//! Returns the trace of the 3x3 matrix, which is the sum of its diagonal elements.
	float		   Trace() const;

	//! Computes the determinant of the 3x3 matrix.
	float		   Determinant() const;

	//! Returns an orthonormalized copy of this matrix.
	idMat3		   OrthoNormalize() const;

	//! Normalizes the rows of the matrix to be orthonormal.
	idMat3&		   OrthoNormalizeSelf();

	//! Returns the transpose of this matrix
	idMat3		   Transpose() const;

	//! Transposes the matrix in place and returns a reference to itself.
	idMat3&		   TransposeSelf();

	//! Returns the inverse of this matrix
	idMat3		   Inverse() const;

	//! Computes the inverse of the matrix in-place and returns true if successful, false if the matrix is singular.
	bool		   InverseSelf();

	//! Returns the inverse of this matrix
	idMat3		   InverseFast() const;

	//! Inverts the matrix in place and returns true if successful, false if the determinant is zero.
	bool		   InverseFastSelf();

	//! Returns the matrix product of the transpose of this matrix and matrix b.
	idMat3		   TransposeMultiply( const idMat3& b ) const;

	//! Computes the inertia matrix translated by a given mass and center of mass offset.
	idMat3		   InertiaTranslate( const float mass, const idVec3& centerOfMass, const idVec3& translation ) const;

	//! Updates the inertia matrix by translating it according to the given mass, center of mass, and translation vector.
	idMat3&		   InertiaTranslateSelf( const float mass, const idVec3& centerOfMass, const idVec3& translation );

	//! Returns the inertia tensor rotated by the specified rotation matrix.
	idMat3		   InertiaRotate( const idMat3& rotation ) const;

	//! Rotates the inertia matrix by the provided rotation matrix and returns a reference to itself.
	idMat3&		   InertiaRotateSelf( const idMat3& rotation );

	//! Returns the dimension of the 3x3 matrix, which is always 9 elements.
	int			   GetDimension() const;

	//! Converts a 3x3 matrix to Euler angles.
	idAngles	   ToAngles() const;

	//! Converts a 3x3 matrix to a quaternion representation.
	idQuat		   ToQuat() const;

	//! Converts this rotation matrix to a unit vector quaternion.
	idCQuat		   ToCQuat() const;

	//! Converts a 3x3 matrix to a rotation object.
	idRotation	   ToRotation() const;

	//! Converts a 3x3 matrix to a 4x4 matrix.
	idMat4		   ToMat4() const;

	//! Converts a rotation matrix to an angular velocity vector.
	idVec3		   ToAngularVelocity() const;

	//! Returns a pointer to the float representation of the matrix data.
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the float representation of the matrix data.
	float*		   ToFloatPtr();

	//! Converts the matrix to a string representation with the specified precision
	const char*	   ToString( int precision = 2 ) const;

	friend void	   TransposeMultiply( const idMat3& inv, const idMat3& b, idMat3& dst );
	friend idMat3  SkewSymmetric( idVec3 const& src );

private:
	idVec3 mat[3];
};

extern idMat3 mat3_zero;
extern idMat3 mat3_identity;
#define mat3_default mat3_identity

ID_INLINE idMat3::idMat3()
{
}

ID_INLINE idMat3::idMat3( const idVec3& x, const idVec3& y, const idVec3& z )
{
	mat[0].x = x.x;
	mat[0].y = x.y;
	mat[0].z = x.z;
	mat[1].x = y.x;
	mat[1].y = y.y;
	mat[1].z = y.z;
	mat[2].x = z.x;
	mat[2].y = z.y;
	mat[2].z = z.z;
}

ID_INLINE idMat3::idMat3( const float xx, const float xy, const float xz, const float yx, const float yy, const float yz, const float zx, const float zy, const float zz )
{
	mat[0].x = xx;
	mat[0].y = xy;
	mat[0].z = xz;
	mat[1].x = yx;
	mat[1].y = yy;
	mat[1].z = yz;
	mat[2].x = zx;
	mat[2].y = zy;
	mat[2].z = zz;
}

ID_INLINE idMat3::idMat3( const float src[3][3] )
{
	memcpy( mat, src, 3 * 3 * sizeof( float ) );
}

ID_INLINE const idVec3& idMat3::operator[]( int index ) const
{
	// assert( ( index >= 0 ) && ( index < 3 ) );
	return mat[index];
}

ID_INLINE idVec3& idMat3::operator[]( int index )
{
	// assert( ( index >= 0 ) && ( index < 3 ) );
	return mat[index];
}

ID_INLINE idMat3 idMat3::operator-() const
{
	return idMat3( -mat[0][0], -mat[0][1], -mat[0][2], -mat[1][0], -mat[1][1], -mat[1][2], -mat[2][0], -mat[2][1], -mat[2][2] );
}

ID_INLINE idVec3 idMat3::operator*( const idVec3& vec ) const
{
	return idVec3( mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x * vec.z, mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y * vec.z, mat[0].z * vec.x + mat[1].z * vec.y + mat[2].z * vec.z );
}

ID_INLINE idMat3 idMat3::operator*( const idMat3& a ) const
{
	int			 i, j;
	const float *m1Ptr, *m2Ptr;
	float*		 dstPtr;
	idMat3		 dst;

	m1Ptr  = reinterpret_cast<const float*>( this );
	m2Ptr  = reinterpret_cast<const float*>( &a );
	dstPtr = reinterpret_cast<float*>( &dst );

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[0 * 3 + j] + m1Ptr[1] * m2Ptr[1 * 3 + j] + m1Ptr[2] * m2Ptr[2 * 3 + j];
			dstPtr++;
		}
		m1Ptr += 3;
	}
	return dst;
}

ID_INLINE idMat3 idMat3::operator*( const float a ) const
{
	return idMat3( mat[0].x * a, mat[0].y * a, mat[0].z * a, mat[1].x * a, mat[1].y * a, mat[1].z * a, mat[2].x * a, mat[2].y * a, mat[2].z * a );
}

ID_INLINE idMat3 idMat3::operator+( const idMat3& a ) const
{
	return idMat3( mat[0].x + a[0].x, mat[0].y + a[0].y, mat[0].z + a[0].z, mat[1].x + a[1].x, mat[1].y + a[1].y, mat[1].z + a[1].z, mat[2].x + a[2].x, mat[2].y + a[2].y, mat[2].z + a[2].z );
}

ID_INLINE idMat3 idMat3::operator-( const idMat3& a ) const
{
	return idMat3( mat[0].x - a[0].x, mat[0].y - a[0].y, mat[0].z - a[0].z, mat[1].x - a[1].x, mat[1].y - a[1].y, mat[1].z - a[1].z, mat[2].x - a[2].x, mat[2].y - a[2].y, mat[2].z - a[2].z );
}

ID_INLINE idMat3& idMat3::operator*=( const float a )
{
	mat[0].x *= a;
	mat[0].y *= a;
	mat[0].z *= a;
	mat[1].x *= a;
	mat[1].y *= a;
	mat[1].z *= a;
	mat[2].x *= a;
	mat[2].y *= a;
	mat[2].z *= a;

	return *this;
}

ID_INLINE idMat3& idMat3::operator*=( const idMat3& a )
{
	int			 i, j;
	const float* m2Ptr;
	float *		 m1Ptr, dst[3];

	m1Ptr = reinterpret_cast<float*>( this );
	m2Ptr = reinterpret_cast<const float*>( &a );

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			dst[j] = m1Ptr[0] * m2Ptr[0 * 3 + j] + m1Ptr[1] * m2Ptr[1 * 3 + j] + m1Ptr[2] * m2Ptr[2 * 3 + j];
		}
		m1Ptr[0] = dst[0];
		m1Ptr[1] = dst[1];
		m1Ptr[2] = dst[2];
		m1Ptr += 3;
	}
	return *this;
}

ID_INLINE idMat3& idMat3::operator+=( const idMat3& a )
{
	mat[0].x += a[0].x;
	mat[0].y += a[0].y;
	mat[0].z += a[0].z;
	mat[1].x += a[1].x;
	mat[1].y += a[1].y;
	mat[1].z += a[1].z;
	mat[2].x += a[2].x;
	mat[2].y += a[2].y;
	mat[2].z += a[2].z;

	return *this;
}

ID_INLINE idMat3& idMat3::operator-=( const idMat3& a )
{
	mat[0].x -= a[0].x;
	mat[0].y -= a[0].y;
	mat[0].z -= a[0].z;
	mat[1].x -= a[1].x;
	mat[1].y -= a[1].y;
	mat[1].z -= a[1].z;
	mat[2].x -= a[2].x;
	mat[2].y -= a[2].y;
	mat[2].z -= a[2].z;

	return *this;
}

//! Returns the result of multiplying a vector by a matrix.
ID_INLINE idVec3 operator*( const idVec3& vec, const idMat3& mat )
{
	return mat * vec;
}

//! Returns the result of multiplying a scalar with a 3x3 matrix.
ID_INLINE idMat3 operator*( const float a, const idMat3& mat )
{
	return mat * a;
}

//! Multiplies the given vector by the given matrix and assigns the result back to the vector
ID_INLINE idVec3& operator*=( idVec3& vec, const idMat3& mat )
{
	float x = mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x * vec.z;
	float y = mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y * vec.z;
	vec.z	= mat[0].z * vec.x + mat[1].z * vec.y + mat[2].z * vec.z;
	vec.x	= x;
	vec.y	= y;
	return vec;
}

ID_INLINE bool idMat3::Compare( const idMat3& a ) const
{
	if( mat[0].Compare( a[0] ) && mat[1].Compare( a[1] ) && mat[2].Compare( a[2] ) ) { return true; }
	return false;
}

ID_INLINE bool idMat3::Compare( const idMat3& a, const float epsilon ) const
{
	if( mat[0].Compare( a[0], epsilon ) && mat[1].Compare( a[1], epsilon ) && mat[2].Compare( a[2], epsilon ) ) { return true; }
	return false;
}

ID_INLINE bool idMat3::operator==( const idMat3& a ) const
{
	return Compare( a );
}

ID_INLINE bool idMat3::operator!=( const idMat3& a ) const
{
	return !Compare( a );
}

ID_INLINE void idMat3::Zero()
{
	memset( mat, 0, sizeof( idMat3 ) );
}

ID_INLINE void idMat3::Identity()
{
	*this = mat3_identity;
}

ID_INLINE bool idMat3::IsIdentity( const float epsilon ) const
{
	return Compare( mat3_identity, epsilon );
}

ID_INLINE bool idMat3::IsSymmetric( const float epsilon ) const
{
	if( idMath::Fabs( mat[0][1] - mat[1][0] ) > epsilon ) { return false; }
	if( idMath::Fabs( mat[0][2] - mat[2][0] ) > epsilon ) { return false; }
	if( idMath::Fabs( mat[1][2] - mat[2][1] ) > epsilon ) { return false; }
	return true;
}

ID_INLINE bool idMat3::IsDiagonal( const float epsilon ) const
{
	if( idMath::Fabs( mat[0][1] ) > epsilon || idMath::Fabs( mat[0][2] ) > epsilon || idMath::Fabs( mat[1][0] ) > epsilon || idMath::Fabs( mat[1][2] ) > epsilon ||
		idMath::Fabs( mat[2][0] ) > epsilon || idMath::Fabs( mat[2][1] ) > epsilon ) {
		return false;
	}
	return true;
}

ID_INLINE bool idMat3::IsRotated() const
{
	return !Compare( mat3_identity );
}

ID_INLINE void idMat3::ProjectVector( const idVec3& src, idVec3& dst ) const
{
	dst.x = src * mat[0];
	dst.y = src * mat[1];
	dst.z = src * mat[2];
}

ID_INLINE void idMat3::UnprojectVector( const idVec3& src, idVec3& dst ) const
{
	dst = mat[0] * src.x + mat[1] * src.y + mat[2] * src.z;
}

ID_INLINE bool idMat3::FixDegeneracies()
{
	bool r = mat[0].FixDegenerateNormal();
	r |= mat[1].FixDegenerateNormal();
	r |= mat[2].FixDegenerateNormal();
	return r;
}

ID_INLINE bool idMat3::FixDenormals()
{
	bool r = mat[0].FixDenormals();
	r |= mat[1].FixDenormals();
	r |= mat[2].FixDenormals();
	return r;
}

ID_INLINE float idMat3::Trace() const
{
	return ( mat[0][0] + mat[1][1] + mat[2][2] );
}

ID_INLINE idMat3 idMat3::OrthoNormalize() const
{
	idMat3 ortho;

	ortho = *this;
	ortho[0].Normalize();
	ortho[2].Cross( mat[0], mat[1] );
	ortho[2].Normalize();
	ortho[1].Cross( mat[2], mat[0] );
	ortho[1].Normalize();
	return ortho;
}

ID_INLINE idMat3& idMat3::OrthoNormalizeSelf()
{
	mat[0].Normalize();
	mat[2].Cross( mat[0], mat[1] );
	mat[2].Normalize();
	mat[1].Cross( mat[2], mat[0] );
	mat[1].Normalize();
	return *this;
}

ID_INLINE idMat3 idMat3::Transpose() const
{
	return idMat3( mat[0][0], mat[1][0], mat[2][0], mat[0][1], mat[1][1], mat[2][1], mat[0][2], mat[1][2], mat[2][2] );
}

ID_INLINE idMat3& idMat3::TransposeSelf()
{
	float tmp0, tmp1, tmp2;

	tmp0	  = mat[0][1];
	mat[0][1] = mat[1][0];
	mat[1][0] = tmp0;
	tmp1	  = mat[0][2];
	mat[0][2] = mat[2][0];
	mat[2][0] = tmp1;
	tmp2	  = mat[1][2];
	mat[1][2] = mat[2][1];
	mat[2][1] = tmp2;

	return *this;
}

ID_INLINE idMat3 idMat3::Inverse() const
{
	idMat3 invMat;

	invMat = *this;
	verify( invMat.InverseSelf() );
	return invMat;
}

ID_INLINE idMat3 idMat3::InverseFast() const
{
	idMat3 invMat;

	invMat = *this;
	verify( invMat.InverseFastSelf() );
	return invMat;
}

ID_INLINE idMat3 idMat3::TransposeMultiply( const idMat3& b ) const
{
	return idMat3( mat[0].x * b[0].x + mat[1].x * b[1].x + mat[2].x * b[2].x,
		mat[0].x * b[0].y + mat[1].x * b[1].y + mat[2].x * b[2].y,
		mat[0].x * b[0].z + mat[1].x * b[1].z + mat[2].x * b[2].z,
		mat[0].y * b[0].x + mat[1].y * b[1].x + mat[2].y * b[2].x,
		mat[0].y * b[0].y + mat[1].y * b[1].y + mat[2].y * b[2].y,
		mat[0].y * b[0].z + mat[1].y * b[1].z + mat[2].y * b[2].z,
		mat[0].z * b[0].x + mat[1].z * b[1].x + mat[2].z * b[2].x,
		mat[0].z * b[0].y + mat[1].z * b[1].y + mat[2].z * b[2].y,
		mat[0].z * b[0].z + mat[1].z * b[1].z + mat[2].z * b[2].z );
}

//! Computes the matrix product of the transpose of one matrix and another matrix.
ID_INLINE void TransposeMultiply( const idMat3& transpose, const idMat3& b, idMat3& dst )
{
	dst[0].x = transpose[0].x * b[0].x + transpose[1].x * b[1].x + transpose[2].x * b[2].x;
	dst[0].y = transpose[0].x * b[0].y + transpose[1].x * b[1].y + transpose[2].x * b[2].y;
	dst[0].z = transpose[0].x * b[0].z + transpose[1].x * b[1].z + transpose[2].x * b[2].z;
	dst[1].x = transpose[0].y * b[0].x + transpose[1].y * b[1].x + transpose[2].y * b[2].x;
	dst[1].y = transpose[0].y * b[0].y + transpose[1].y * b[1].y + transpose[2].y * b[2].y;
	dst[1].z = transpose[0].y * b[0].z + transpose[1].y * b[1].z + transpose[2].y * b[2].z;
	dst[2].x = transpose[0].z * b[0].x + transpose[1].z * b[1].x + transpose[2].z * b[2].x;
	dst[2].y = transpose[0].z * b[0].y + transpose[1].z * b[1].y + transpose[2].z * b[2].y;
	dst[2].z = transpose[0].z * b[0].z + transpose[1].z * b[1].z + transpose[2].z * b[2].z;
}

//! Creates a skew-symmetric matrix from the given 3D vector.
ID_INLINE idMat3 SkewSymmetric( idVec3 const& src )
{
	return idMat3( 0.0f, -src.z, src.y, src.z, 0.0f, -src.x, -src.y, src.x, 0.0f );
}

ID_INLINE int idMat3::GetDimension() const
{
	return 9;
}

ID_INLINE const float* idMat3::ToFloatPtr() const
{
	return mat[0].ToFloatPtr();
}

ID_INLINE float* idMat3::ToFloatPtr()
{
	return mat[0].ToFloatPtr();
}

/*!
	\class idMat4
	\brief A 4x4 matrix class supporting various construction methods, transformations, and mathematical operations.

	This class represents a 4x4 matrix commonly used for 3D transformations, including rotations, translations, and projections. It provides multiple constructors for flexible initialization from
   different data sources such as vectors, arrays, or existing matrices. The class supports standard matrix operations like multiplication, addition, subtraction, transposition, and inversion. It also
   includes utility functions for extracting components like translation and rotation, as well as checking matrix properties such as identity, symmetry, and diagonality. The class is designed to work
   seamlessly with vector types for efficient geometric computations.

*/
class idMat4
{
public:
	//! Constructs an identity matrix.
	idMat4();

	//! Constructs a 4x4 matrix from four 4D vectors representing the rows or columns of the matrix
	explicit idMat4( const idVec4& x, const idVec4& y, const idVec4& z, const idVec4& w );

	//! Constructs a 4x4 matrix from 16 individual float values
	explicit idMat4( const float xx,
		const float				 xy,
		const float				 xz,
		const float				 xw,
		const float				 yx,
		const float				 yy,
		const float				 yz,
		const float				 yw,
		const float				 zx,
		const float				 zy,
		const float				 zz,
		const float				 zw,
		const float				 wx,
		const float				 wy,
		const float				 wz,
		const float				 ww );

	//! Constructs a 4x4 matrix from a 3x3 rotation matrix and a 3D translation vector.
	explicit idMat4( const idMat3& rotation, const idVec3& translation );

	//! Constructs a 4x4 matrix from a 4x4 array of floats.
	explicit idMat4( const float src[4][4] );

	//! Returns a const reference to the idVec4 row at the specified index in the matrix
	const idVec4&  operator[]( int index ) const;

	//! Provides indexed access to the rows of the 4x4 matrix.
	idVec4&		   operator[]( int index );

	//! Returns a new matrix that is the result of multiplying this matrix by a scalar value.
	idMat4		   operator*( const float a ) const;

	//! Multiplies this matrix by the given vector and returns the resulting vector.
	idVec4		   operator*( const idVec4& vec ) const;

	//! Multiplies this 4x4 matrix by a 3D vector and returns the transformed vector.
	idVec3		   operator*( const idVec3& vec ) const;

	//! Returns the matrix product of this matrix and matrix a.
	idMat4		   operator*( const idMat4& a ) const;

	//! Returns a new matrix that is the element-wise sum of this matrix and matrix a.
	idMat4		   operator+( const idMat4& a ) const;

	//! Returns the element-wise difference between this matrix and another matrix.
	idMat4		   operator-( const idMat4& a ) const;

	//! Multiplies all elements of the matrix by the given scalar value and returns a reference to the matrix.
	idMat4&		   operator*=( const float a );

	//! Multiplies this matrix by the given matrix and assigns the result to this matrix.
	idMat4&		   operator*=( const idMat4& a );

	//! Adds the elements of the given matrix to this matrix and returns a reference to this matrix.
	idMat4&		   operator+=( const idMat4& a );

	//! Subtracts the elements of the given matrix from this matrix in place and returns a reference to this matrix.
	idMat4&		   operator-=( const idMat4& a );

	friend idMat4  operator*( const float a, const idMat4& mat );
	friend idVec4  operator*( const idVec4& vec, const idMat4& mat );
	friend idVec3  operator*( const idVec3& vec, const idMat4& mat );
	friend idVec4& operator*=( idVec4& vec, const idMat4& mat );
	friend idVec3& operator*=( idVec3& vec, const idMat4& mat );

	//! Compares this matrix with another matrix for equality.
	bool		   Compare( const idMat4& a ) const;

	//! Compares this matrix with another matrix using the specified epsilon tolerance.
	bool		   Compare( const idMat4& a, const float epsilon ) const;

	//! Compares two idMat4 matrices for exact equality.
	bool		   operator==( const idMat4& a ) const;

	//! Returns true if this matrix is not equal to the given matrix.
	bool		   operator!=( const idMat4& a ) const;

	//! Sets all elements of the matrix to zero.
	void		   Zero();

	//! Sets the matrix to the 4x4 identity matrix.
	void		   Identity();

	//! Checks if the matrix is equal to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the matrix is symmetric within a given epsilon tolerance.
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is diagonal, within the specified epsilon tolerance.
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix has a non-zero rotation component.
	bool		   IsRotated() const;

	//! Projects a vector using the matrix.
	void		   ProjectVector( const idVec4& src, idVec4& dst ) const;

	//! Transforms a vector using the matrix.
	void		   UnprojectVector( const idVec4& src, idVec4& dst ) const;

	//! Returns the trace of the 4x4 matrix, which is the sum of its diagonal elements.
	float		   Trace() const;

	//! Computes and returns the determinant of this 4x4 matrix.
	float		   Determinant() const;

	//! Returns the transpose of the matrix.
	idMat4		   Transpose() const;

	//! Transposes the matrix in place and returns a reference to itself.
	idMat4&		   TransposeSelf();

	//! Returns the inverse of this matrix
	idMat4		   Inverse() const;

	//! Computes the inverse of the matrix in place and returns true if successful, false if the matrix is singular.
	bool		   InverseSelf();

	//! Returns the inverse of this matrix
	idMat4		   InverseFast() const;

	//! Computes the fast inverse of the matrix in-place and returns whether the operation was successful.
	bool		   InverseFastSelf();
	idMat4		   TransposeMultiply( const idMat4& b ) const;

	//! Returns the dimension of the 4x4 matrix, which is always 16 elements.
	int			   GetDimension() const;

	//! Returns a pointer to the float representation of the matrix data.
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the first element of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision
	const char*	   ToString( int precision = 2 ) const;

	//! Converts a 4x4 matrix to a 3x3 matrix by extracting the rotational components.
	idMat3		   ToMat3() const;

	//! Retrieves the translation component from a 4x4 transformation matrix.
	idVec3		   GetTranslation() const;

private:
	idVec4 mat[4];
};

extern idMat4 mat4_zero;
extern idMat4 mat4_identity;
#define mat4_default mat4_identity

ID_INLINE idMat3 idMat4::ToMat3() const
{
	idMat3 m;

	// RB - NOTE: idMat3 is transposed because it is column-major
	m[0][0] = mat[0][0];
	m[0][1] = mat[1][0];
	m[0][2] = mat[2][0];
	m[1][0] = mat[0][1];
	m[1][1] = mat[1][1];
	m[1][2] = mat[2][1];
	m[2][0] = mat[0][2];
	m[2][1] = mat[1][2];
	m[2][2] = mat[2][2];

	return m;
}

ID_INLINE idVec3 idMat4::GetTranslation() const
{
	idVec3 pos;

	pos.x = mat[0][3];
	pos.y = mat[1][3];
	pos.z = mat[2][3];

	return pos;
}

ID_INLINE idMat4::idMat4()
{
}

ID_INLINE idMat4::idMat4( const idVec4& x, const idVec4& y, const idVec4& z, const idVec4& w )
{
	mat[0] = x;
	mat[1] = y;
	mat[2] = z;
	mat[3] = w;
}

ID_INLINE idMat4::idMat4( const float xx,
	const float						  xy,
	const float						  xz,
	const float						  xw,
	const float						  yx,
	const float						  yy,
	const float						  yz,
	const float						  yw,
	const float						  zx,
	const float						  zy,
	const float						  zz,
	const float						  zw,
	const float						  wx,
	const float						  wy,
	const float						  wz,
	const float						  ww )
{
	mat[0][0] = xx;
	mat[0][1] = xy;
	mat[0][2] = xz;
	mat[0][3] = xw;
	mat[1][0] = yx;
	mat[1][1] = yy;
	mat[1][2] = yz;
	mat[1][3] = yw;
	mat[2][0] = zx;
	mat[2][1] = zy;
	mat[2][2] = zz;
	mat[2][3] = zw;
	mat[3][0] = wx;
	mat[3][1] = wy;
	mat[3][2] = wz;
	mat[3][3] = ww;
}

ID_INLINE idMat4::idMat4( const idMat3& rotation, const idVec3& translation )
{
	// NOTE: idMat3 is transposed because it is column-major
	mat[0][0] = rotation[0][0];
	mat[0][1] = rotation[1][0];
	mat[0][2] = rotation[2][0];
	mat[0][3] = translation[0];
	mat[1][0] = rotation[0][1];
	mat[1][1] = rotation[1][1];
	mat[1][2] = rotation[2][1];
	mat[1][3] = translation[1];
	mat[2][0] = rotation[0][2];
	mat[2][1] = rotation[1][2];
	mat[2][2] = rotation[2][2];
	mat[2][3] = translation[2];
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
}

ID_INLINE idMat4::idMat4( const float src[4][4] )
{
	memcpy( mat, src, 4 * 4 * sizeof( float ) );
}

ID_INLINE const idVec4& idMat4::operator[]( int index ) const
{
	// assert( ( index >= 0 ) && ( index < 4 ) );
	return mat[index];
}

ID_INLINE idVec4& idMat4::operator[]( int index )
{
	// assert( ( index >= 0 ) && ( index < 4 ) );
	return mat[index];
}

ID_INLINE idMat4 idMat4::operator*( const float a ) const
{
	return idMat4( mat[0].x * a,
		mat[0].y * a,
		mat[0].z * a,
		mat[0].w * a,
		mat[1].x * a,
		mat[1].y * a,
		mat[1].z * a,
		mat[1].w * a,
		mat[2].x * a,
		mat[2].y * a,
		mat[2].z * a,
		mat[2].w * a,
		mat[3].x * a,
		mat[3].y * a,
		mat[3].z * a,
		mat[3].w * a );
}

ID_INLINE idVec4 idMat4::operator*( const idVec4& vec ) const
{
	return idVec4( mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w * vec.w,
		mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w * vec.w,
		mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w * vec.w,
		mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w * vec.w );
}

ID_INLINE idVec3 idMat4::operator*( const idVec3& vec ) const
{
	float s = mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w;
	if( s == 0.0f ) { return idVec3( 0.0f, 0.0f, 0.0f ); }
	if( s == 1.0f ) {
		return idVec3( mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w,
			mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w,
			mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w );
	} else {
		float invS = 1.0f / s;
		return idVec3( ( mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w ) * invS,
			( mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w ) * invS,
			( mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w ) * invS );
	}
}

ID_INLINE idMat4 idMat4::operator*( const idMat4& a ) const
{
	int			 i, j;
	const float *m1Ptr, *m2Ptr;
	float*		 dstPtr;
	idMat4		 dst;

	m1Ptr  = reinterpret_cast<const float*>( this );
	m2Ptr  = reinterpret_cast<const float*>( &a );
	dstPtr = reinterpret_cast<float*>( &dst );

	for( i = 0; i < 4; i++ ) {
		for( j = 0; j < 4; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[0 * 4 + j] + m1Ptr[1] * m2Ptr[1 * 4 + j] + m1Ptr[2] * m2Ptr[2 * 4 + j] + m1Ptr[3] * m2Ptr[3 * 4 + j];
			dstPtr++;
		}
		m1Ptr += 4;
	}
	return dst;
}

ID_INLINE idMat4 idMat4::operator+( const idMat4& a ) const
{
	return idMat4( mat[0].x + a[0].x,
		mat[0].y + a[0].y,
		mat[0].z + a[0].z,
		mat[0].w + a[0].w,
		mat[1].x + a[1].x,
		mat[1].y + a[1].y,
		mat[1].z + a[1].z,
		mat[1].w + a[1].w,
		mat[2].x + a[2].x,
		mat[2].y + a[2].y,
		mat[2].z + a[2].z,
		mat[2].w + a[2].w,
		mat[3].x + a[3].x,
		mat[3].y + a[3].y,
		mat[3].z + a[3].z,
		mat[3].w + a[3].w );
}

ID_INLINE idMat4 idMat4::operator-( const idMat4& a ) const
{
	return idMat4( mat[0].x - a[0].x,
		mat[0].y - a[0].y,
		mat[0].z - a[0].z,
		mat[0].w - a[0].w,
		mat[1].x - a[1].x,
		mat[1].y - a[1].y,
		mat[1].z - a[1].z,
		mat[1].w - a[1].w,
		mat[2].x - a[2].x,
		mat[2].y - a[2].y,
		mat[2].z - a[2].z,
		mat[2].w - a[2].w,
		mat[3].x - a[3].x,
		mat[3].y - a[3].y,
		mat[3].z - a[3].z,
		mat[3].w - a[3].w );
}

ID_INLINE idMat4& idMat4::operator*=( const float a )
{
	mat[0].x *= a;
	mat[0].y *= a;
	mat[0].z *= a;
	mat[0].w *= a;
	mat[1].x *= a;
	mat[1].y *= a;
	mat[1].z *= a;
	mat[1].w *= a;
	mat[2].x *= a;
	mat[2].y *= a;
	mat[2].z *= a;
	mat[2].w *= a;
	mat[3].x *= a;
	mat[3].y *= a;
	mat[3].z *= a;
	mat[3].w *= a;
	return *this;
}

ID_INLINE idMat4& idMat4::operator*=( const idMat4& a )
{
	*this = ( *this ) * a;
	return *this;
}

ID_INLINE idMat4& idMat4::operator+=( const idMat4& a )
{
	mat[0].x += a[0].x;
	mat[0].y += a[0].y;
	mat[0].z += a[0].z;
	mat[0].w += a[0].w;
	mat[1].x += a[1].x;
	mat[1].y += a[1].y;
	mat[1].z += a[1].z;
	mat[1].w += a[1].w;
	mat[2].x += a[2].x;
	mat[2].y += a[2].y;
	mat[2].z += a[2].z;
	mat[2].w += a[2].w;
	mat[3].x += a[3].x;
	mat[3].y += a[3].y;
	mat[3].z += a[3].z;
	mat[3].w += a[3].w;
	return *this;
}

ID_INLINE idMat4& idMat4::operator-=( const idMat4& a )
{
	mat[0].x -= a[0].x;
	mat[0].y -= a[0].y;
	mat[0].z -= a[0].z;
	mat[0].w -= a[0].w;
	mat[1].x -= a[1].x;
	mat[1].y -= a[1].y;
	mat[1].z -= a[1].z;
	mat[1].w -= a[1].w;
	mat[2].x -= a[2].x;
	mat[2].y -= a[2].y;
	mat[2].z -= a[2].z;
	mat[2].w -= a[2].w;
	mat[3].x -= a[3].x;
	mat[3].y -= a[3].y;
	mat[3].z -= a[3].z;
	mat[3].w -= a[3].w;
	return *this;
}

//! Returns the result of multiplying a scalar value with a 4x4 matrix.
ID_INLINE idMat4 operator*( const float a, const idMat4& mat )
{
	return mat * a;
}

//! Multiplies a vector by a matrix, returning the transformed vector.
ID_INLINE idVec4 operator*( const idVec4& vec, const idMat4& mat )
{
	return mat * vec;
}

//! Multiplies a vector by a matrix.
ID_INLINE idVec3 operator*( const idVec3& vec, const idMat4& mat )
{
	return mat * vec;
}

//! Multiplies a vector by a matrix and assigns the result back to the vector.
ID_INLINE idVec4& operator*=( idVec4& vec, const idMat4& mat )
{
	vec = mat * vec;
	return vec;
}

//! Multiplies the given vector by the matrix and assigns the result back to the vector.
ID_INLINE idVec3& operator*=( idVec3& vec, const idMat4& mat )
{
	vec = mat * vec;
	return vec;
}

ID_INLINE bool idMat4::Compare( const idMat4& a ) const
{
	dword		 i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float*>( mat );
	ptr2 = reinterpret_cast<const float*>( a.mat );
	for( i = 0; i < 4 * 4; i++ ) {
		if( ptr1[i] != ptr2[i] ) { return false; }
	}
	return true;
}

ID_INLINE bool idMat4::Compare( const idMat4& a, const float epsilon ) const
{
	dword		 i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float*>( mat );
	ptr2 = reinterpret_cast<const float*>( a.mat );
	for( i = 0; i < 4 * 4; i++ ) {
		if( idMath::Fabs( ptr1[i] - ptr2[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idMat4::operator==( const idMat4& a ) const
{
	return Compare( a );
}

ID_INLINE bool idMat4::operator!=( const idMat4& a ) const
{
	return !Compare( a );
}

ID_INLINE void idMat4::Zero()
{
	memset( mat, 0, sizeof( idMat4 ) );
}

ID_INLINE void idMat4::Identity()
{
	*this = mat4_identity;
}

ID_INLINE bool idMat4::IsIdentity( const float epsilon ) const
{
	return Compare( mat4_identity, epsilon );
}

ID_INLINE bool idMat4::IsSymmetric( const float epsilon ) const
{
	for( int i = 1; i < 4; i++ ) {
		for( int j = 0; j < i; j++ ) {
			if( idMath::Fabs( mat[i][j] - mat[j][i] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMat4::IsDiagonal( const float epsilon ) const
{
	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; j < 4; j++ ) {
			if( i != j && idMath::Fabs( mat[i][j] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMat4::IsRotated() const
{
	if( !mat[0][1] && !mat[0][2] && !mat[1][0] && !mat[1][2] && !mat[2][0] && !mat[2][1] ) { return false; }
	return true;
}

ID_INLINE void idMat4::ProjectVector( const idVec4& src, idVec4& dst ) const
{
	dst.x = src * mat[0];
	dst.y = src * mat[1];
	dst.z = src * mat[2];
	dst.w = src * mat[3];
}

ID_INLINE void idMat4::UnprojectVector( const idVec4& src, idVec4& dst ) const
{
	dst = mat[0] * src.x + mat[1] * src.y + mat[2] * src.z + mat[3] * src.w;
}

ID_INLINE float idMat4::Trace() const
{
	return ( mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3] );
}

ID_INLINE idMat4 idMat4::Inverse() const
{
	idMat4 invMat;

	invMat = *this;
	verify( invMat.InverseSelf() );
	return invMat;
}

ID_INLINE idMat4 idMat4::InverseFast() const
{
	idMat4 invMat;

	invMat = *this;
	verify( invMat.InverseFastSelf() );
	return invMat;
}

ID_INLINE idMat4 idMat3::ToMat4() const
{
	// NOTE: idMat3 is transposed because it is column-major
	return idMat4( mat[0][0], mat[1][0], mat[2][0], 0.0f, mat[0][1], mat[1][1], mat[2][1], 0.0f, mat[0][2], mat[1][2], mat[2][2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
}

ID_INLINE int idMat4::GetDimension() const
{
	return 16;
}

ID_INLINE const float* idMat4::ToFloatPtr() const
{
	return mat[0].ToFloatPtr();
}

ID_INLINE float* idMat4::ToFloatPtr()
{
	return mat[0].ToFloatPtr();
}

/*!
	\class idMat5
	\brief A 5x5 matrix class with support for common matrix operations and conversions.

	The idMat5 class represents a 5x5 matrix and provides a comprehensive set of operations for matrix manipulation. It supports construction from various sources including individual vectors, raw
   float arrays, and initialization as identity or zero matrices. The class offers both element-wise and matrix-wise arithmetic operations, including multiplication, addition, and subtraction, with
   corresponding assignment operators for in-place modifications. It includes methods for checking matrix properties such as identity, symmetry, and diagonal nature, along with computation of trace,
   determinant, transpose, and inverse operations. The class provides indexed access to rows through operator[], and supports conversion to and from float pointers and string representations for ease
   of integration with other systems.

*/
class idMat5
{
public:
	//! Constructs an uninitialized idMat5 matrix.
	idMat5();

	//! Constructs a 5x5 matrix using five 5-dimensional vectors as rows.
	explicit idMat5( const idVec5& v0, const idVec5& v1, const idVec5& v2, const idVec5& v3, const idVec5& v4 );

	//! Constructs an idMat5 object by copying data from a 5x5 float array.
	explicit idMat5( const float src[5][5] );

	//! Returns a const reference to the idVec5 at the specified index in the idMat5 matrix
	const idVec5&  operator[]( int index ) const;

	//! Provides indexed access to the rows of the 5x5 matrix.
	idVec5&		   operator[]( int index );

	//! Returns a new idMat5 matrix with each element multiplied by the given scalar value.
	idMat5		   operator*( const float a ) const;

	//! Multiplies this 5x5 matrix by a 5D vector and returns the resulting 5D vector.
	idVec5		   operator*( const idVec5& vec ) const;

	//! Returns the matrix product of this matrix and the given matrix.
	idMat5		   operator*( const idMat5& a ) const;

	//! Returns the element-wise sum of this matrix and matrix a.
	idMat5		   operator+( const idMat5& a ) const;

	//! Returns the element-wise difference between this matrix and matrix a.
	idMat5		   operator-( const idMat5& a ) const;

	//! Multiplies all elements of the matrix by the given scalar value and returns a reference to the matrix.
	idMat5&		   operator*=( const float a );

	//! Multiplies this matrix by the given matrix and assigns the result to this matrix.
	idMat5&		   operator*=( const idMat5& a );

	//! Adds the elements of the given matrix to this matrix and returns a reference to this matrix.
	idMat5&		   operator+=( const idMat5& a );

	//! Subtracts the elements of the given matrix from this matrix in-place and returns a reference to this matrix.
	idMat5&		   operator-=( const idMat5& a );

	friend idMat5  operator*( const float a, const idMat5& mat );
	friend idVec5  operator*( const idVec5& vec, const idMat5& mat );
	friend idVec5& operator*=( idVec5& vec, const idMat5& mat );

	//! Compares this matrix with another matrix for equality.
	bool		   Compare( const idMat5& a ) const;

	//! Compares this matrix with another matrix using the specified epsilon tolerance
	bool		   Compare( const idMat5& a, const float epsilon ) const;

	//! Compares two idMat5 matrices for equality.
	bool		   operator==( const idMat5& a ) const;

	//! Compares two idMat5 matrices for inequality.
	bool		   operator!=( const idMat5& a ) const;

	//! Sets all elements of the matrix to zero.
	void		   Zero();

	//! Sets the matrix to the 5x5 identity matrix.
	void		   Identity();

	//! Checks if the matrix is equal to the identity matrix within a given epsilon threshold.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the matrix is symmetric within a given epsilon tolerance.
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the matrix is diagonal within the specified epsilon tolerance.
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Computes the trace of the 5x5 matrix by summing its diagonal elements.
	float		   Trace() const;

	//! Calculates and returns the determinant of the 5x5 matrix.
	float		   Determinant() const;

	//! Returns the transpose of this 5x5 matrix.
	idMat5		   Transpose() const;

	//! Transposes the matrix in place and returns a reference to itself.
	idMat5&		   TransposeSelf();

	//! Returns the inverse of the matrix.
	idMat5		   Inverse() const;

	//! Computes the inverse of the 5x5 matrix in-place and returns true if successful, false if the matrix is singular.
	bool		   InverseSelf();

	//! Returns the inverse of this matrix.
	idMat5		   InverseFast() const;

	//! Computes the inverse of the matrix in place and returns false if the matrix is singular.
	bool		   InverseFastSelf();

	//! Returns the dimension of the matrix, which is 25 for a 5x5 matrix.
	int			   GetDimension() const;

	//! Returns a pointer to the float representation of the matrix data.
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the first element of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision.
	const char*	   ToString( int precision = 2 ) const;

private:
	idVec5 mat[5];
};

extern idMat5 mat5_zero;
extern idMat5 mat5_identity;
#define mat5_default mat5_identity

ID_INLINE idMat5::idMat5()
{
}

ID_INLINE idMat5::idMat5( const float src[5][5] )
{
	memcpy( mat, src, 5 * 5 * sizeof( float ) );
}

ID_INLINE idMat5::idMat5( const idVec5& v0, const idVec5& v1, const idVec5& v2, const idVec5& v3, const idVec5& v4 )
{
	mat[0] = v0;
	mat[1] = v1;
	mat[2] = v2;
	mat[3] = v3;
	mat[4] = v4;
}

ID_INLINE const idVec5& idMat5::operator[]( int index ) const
{
	// assert( ( index >= 0 ) && ( index < 5 ) );
	return mat[index];
}

ID_INLINE idVec5& idMat5::operator[]( int index )
{
	// assert( ( index >= 0 ) && ( index < 5 ) );
	return mat[index];
}

ID_INLINE idMat5 idMat5::operator*( const idMat5& a ) const
{
	int			 i, j;
	const float *m1Ptr, *m2Ptr;
	float*		 dstPtr;
	idMat5		 dst;

	m1Ptr  = reinterpret_cast<const float*>( this );
	m2Ptr  = reinterpret_cast<const float*>( &a );
	dstPtr = reinterpret_cast<float*>( &dst );

	for( i = 0; i < 5; i++ ) {
		for( j = 0; j < 5; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[0 * 5 + j] + m1Ptr[1] * m2Ptr[1 * 5 + j] + m1Ptr[2] * m2Ptr[2 * 5 + j] + m1Ptr[3] * m2Ptr[3 * 5 + j] + m1Ptr[4] * m2Ptr[4 * 5 + j];
			dstPtr++;
		}
		m1Ptr += 5;
	}
	return dst;
}

ID_INLINE idMat5 idMat5::operator*( const float a ) const
{
	return idMat5( idVec5( mat[0][0] * a, mat[0][1] * a, mat[0][2] * a, mat[0][3] * a, mat[0][4] * a ),
		idVec5( mat[1][0] * a, mat[1][1] * a, mat[1][2] * a, mat[1][3] * a, mat[1][4] * a ),
		idVec5( mat[2][0] * a, mat[2][1] * a, mat[2][2] * a, mat[2][3] * a, mat[2][4] * a ),
		idVec5( mat[3][0] * a, mat[3][1] * a, mat[3][2] * a, mat[3][3] * a, mat[3][4] * a ),
		idVec5( mat[4][0] * a, mat[4][1] * a, mat[4][2] * a, mat[4][3] * a, mat[4][4] * a ) );
}

ID_INLINE idVec5 idMat5::operator*( const idVec5& vec ) const
{
	return idVec5( mat[0][0] * vec[0] + mat[0][1] * vec[1] + mat[0][2] * vec[2] + mat[0][3] * vec[3] + mat[0][4] * vec[4],
		mat[1][0] * vec[0] + mat[1][1] * vec[1] + mat[1][2] * vec[2] + mat[1][3] * vec[3] + mat[1][4] * vec[4],
		mat[2][0] * vec[0] + mat[2][1] * vec[1] + mat[2][2] * vec[2] + mat[2][3] * vec[3] + mat[2][4] * vec[4],
		mat[3][0] * vec[0] + mat[3][1] * vec[1] + mat[3][2] * vec[2] + mat[3][3] * vec[3] + mat[3][4] * vec[4],
		mat[4][0] * vec[0] + mat[4][1] * vec[1] + mat[4][2] * vec[2] + mat[4][3] * vec[3] + mat[4][4] * vec[4] );
}

ID_INLINE idMat5 idMat5::operator+( const idMat5& a ) const
{
	return idMat5( idVec5( mat[0][0] + a[0][0], mat[0][1] + a[0][1], mat[0][2] + a[0][2], mat[0][3] + a[0][3], mat[0][4] + a[0][4] ),
		idVec5( mat[1][0] + a[1][0], mat[1][1] + a[1][1], mat[1][2] + a[1][2], mat[1][3] + a[1][3], mat[1][4] + a[1][4] ),
		idVec5( mat[2][0] + a[2][0], mat[2][1] + a[2][1], mat[2][2] + a[2][2], mat[2][3] + a[2][3], mat[2][4] + a[2][4] ),
		idVec5( mat[3][0] + a[3][0], mat[3][1] + a[3][1], mat[3][2] + a[3][2], mat[3][3] + a[3][3], mat[3][4] + a[3][4] ),
		idVec5( mat[4][0] + a[4][0], mat[4][1] + a[4][1], mat[4][2] + a[4][2], mat[4][3] + a[4][3], mat[4][4] + a[4][4] ) );
}

ID_INLINE idMat5 idMat5::operator-( const idMat5& a ) const
{
	return idMat5( idVec5( mat[0][0] - a[0][0], mat[0][1] - a[0][1], mat[0][2] - a[0][2], mat[0][3] - a[0][3], mat[0][4] - a[0][4] ),
		idVec5( mat[1][0] - a[1][0], mat[1][1] - a[1][1], mat[1][2] - a[1][2], mat[1][3] - a[1][3], mat[1][4] - a[1][4] ),
		idVec5( mat[2][0] - a[2][0], mat[2][1] - a[2][1], mat[2][2] - a[2][2], mat[2][3] - a[2][3], mat[2][4] - a[2][4] ),
		idVec5( mat[3][0] - a[3][0], mat[3][1] - a[3][1], mat[3][2] - a[3][2], mat[3][3] - a[3][3], mat[3][4] - a[3][4] ),
		idVec5( mat[4][0] - a[4][0], mat[4][1] - a[4][1], mat[4][2] - a[4][2], mat[4][3] - a[4][3], mat[4][4] - a[4][4] ) );
}

ID_INLINE idMat5& idMat5::operator*=( const float a )
{
	mat[0][0] *= a;
	mat[0][1] *= a;
	mat[0][2] *= a;
	mat[0][3] *= a;
	mat[0][4] *= a;
	mat[1][0] *= a;
	mat[1][1] *= a;
	mat[1][2] *= a;
	mat[1][3] *= a;
	mat[1][4] *= a;
	mat[2][0] *= a;
	mat[2][1] *= a;
	mat[2][2] *= a;
	mat[2][3] *= a;
	mat[2][4] *= a;
	mat[3][0] *= a;
	mat[3][1] *= a;
	mat[3][2] *= a;
	mat[3][3] *= a;
	mat[3][4] *= a;
	mat[4][0] *= a;
	mat[4][1] *= a;
	mat[4][2] *= a;
	mat[4][3] *= a;
	mat[4][4] *= a;
	return *this;
}

ID_INLINE idMat5& idMat5::operator*=( const idMat5& a )
{
	*this = *this * a;
	return *this;
}

ID_INLINE idMat5& idMat5::operator+=( const idMat5& a )
{
	mat[0][0] += a[0][0];
	mat[0][1] += a[0][1];
	mat[0][2] += a[0][2];
	mat[0][3] += a[0][3];
	mat[0][4] += a[0][4];
	mat[1][0] += a[1][0];
	mat[1][1] += a[1][1];
	mat[1][2] += a[1][2];
	mat[1][3] += a[1][3];
	mat[1][4] += a[1][4];
	mat[2][0] += a[2][0];
	mat[2][1] += a[2][1];
	mat[2][2] += a[2][2];
	mat[2][3] += a[2][3];
	mat[2][4] += a[2][4];
	mat[3][0] += a[3][0];
	mat[3][1] += a[3][1];
	mat[3][2] += a[3][2];
	mat[3][3] += a[3][3];
	mat[3][4] += a[3][4];
	mat[4][0] += a[4][0];
	mat[4][1] += a[4][1];
	mat[4][2] += a[4][2];
	mat[4][3] += a[4][3];
	mat[4][4] += a[4][4];
	return *this;
}

ID_INLINE idMat5& idMat5::operator-=( const idMat5& a )
{
	mat[0][0] -= a[0][0];
	mat[0][1] -= a[0][1];
	mat[0][2] -= a[0][2];
	mat[0][3] -= a[0][3];
	mat[0][4] -= a[0][4];
	mat[1][0] -= a[1][0];
	mat[1][1] -= a[1][1];
	mat[1][2] -= a[1][2];
	mat[1][3] -= a[1][3];
	mat[1][4] -= a[1][4];
	mat[2][0] -= a[2][0];
	mat[2][1] -= a[2][1];
	mat[2][2] -= a[2][2];
	mat[2][3] -= a[2][3];
	mat[2][4] -= a[2][4];
	mat[3][0] -= a[3][0];
	mat[3][1] -= a[3][1];
	mat[3][2] -= a[3][2];
	mat[3][3] -= a[3][3];
	mat[3][4] -= a[3][4];
	mat[4][0] -= a[4][0];
	mat[4][1] -= a[4][1];
	mat[4][2] -= a[4][2];
	mat[4][3] -= a[4][3];
	mat[4][4] -= a[4][4];
	return *this;
}

//! Multiplies a 5D vector by a 5x5 matrix and returns the resulting vector.
ID_INLINE idVec5 operator*( const idVec5& vec, const idMat5& mat )
{
	return mat * vec;
}

//! Returns the result of multiplying a scalar value with a 5x5 matrix.
ID_INLINE idMat5 operator*( const float a, idMat5 const& mat )
{
	return mat * a;
}

//! Multiplies the vector by the matrix and assigns the result back to the vector
ID_INLINE idVec5& operator*=( idVec5& vec, const idMat5& mat )
{
	vec = mat * vec;
	return vec;
}

ID_INLINE bool idMat5::Compare( const idMat5& a ) const
{
	dword		 i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float*>( mat );
	ptr2 = reinterpret_cast<const float*>( a.mat );
	for( i = 0; i < 5 * 5; i++ ) {
		if( ptr1[i] != ptr2[i] ) { return false; }
	}
	return true;
}

ID_INLINE bool idMat5::Compare( const idMat5& a, const float epsilon ) const
{
	dword		 i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float*>( mat );
	ptr2 = reinterpret_cast<const float*>( a.mat );
	for( i = 0; i < 5 * 5; i++ ) {
		if( idMath::Fabs( ptr1[i] - ptr2[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idMat5::operator==( const idMat5& a ) const
{
	return Compare( a );
}

ID_INLINE bool idMat5::operator!=( const idMat5& a ) const
{
	return !Compare( a );
}

ID_INLINE void idMat5::Zero()
{
	memset( mat, 0, sizeof( idMat5 ) );
}

ID_INLINE void idMat5::Identity()
{
	*this = mat5_identity;
}

ID_INLINE bool idMat5::IsIdentity( const float epsilon ) const
{
	return Compare( mat5_identity, epsilon );
}

ID_INLINE bool idMat5::IsSymmetric( const float epsilon ) const
{
	for( int i = 1; i < 5; i++ ) {
		for( int j = 0; j < i; j++ ) {
			if( idMath::Fabs( mat[i][j] - mat[j][i] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMat5::IsDiagonal( const float epsilon ) const
{
	for( int i = 0; i < 5; i++ ) {
		for( int j = 0; j < 5; j++ ) {
			if( i != j && idMath::Fabs( mat[i][j] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE float idMat5::Trace() const
{
	return ( mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3] + mat[4][4] );
}

ID_INLINE idMat5 idMat5::Inverse() const
{
	idMat5 invMat;

	invMat = *this;
	verify( invMat.InverseSelf() );
	return invMat;
}

ID_INLINE idMat5 idMat5::InverseFast() const
{
	idMat5 invMat;

	invMat = *this;
	verify( invMat.InverseFastSelf() );
	return invMat;
}

ID_INLINE int idMat5::GetDimension() const
{
	return 25;
}

ID_INLINE const float* idMat5::ToFloatPtr() const
{
	return mat[0].ToFloatPtr();
}

ID_INLINE float* idMat5::ToFloatPtr()
{
	return mat[0].ToFloatPtr();
}

/*!
	\class idMat6
	\brief A 6x6 matrix class supporting various mathematical operations and conversions.

	This class represents a 6x6 matrix with support for construction from various data sources including vectors, sub-matrices, and raw arrays. It provides comprehensive arithmetic operations such as
   multiplication with scalars and other matrices, addition, subtraction, and element-wise comparisons. The class includes methods for matrix properties verification like identity, symmetry, and
   diagonal status, as well as operations for retrieving sub-matrices, computing determinants, traces, and inverses. It supports both in-place and return-by-value operations for matrix transformations
   and offers conversion utilities to and from floating-point pointers and string representations. The design allows for efficient matrix manipulation in mathematical computations and transformations.

*/
class idMat6
{
public:
	//! Constructs an uninitialized 6x6 matrix.
	idMat6();

	//! Constructs a 6x6 matrix using six 6D vectors as rows.
	explicit idMat6( const idVec6& v0, const idVec6& v1, const idVec6& v2, const idVec6& v3, const idVec6& v4, const idVec6& v5 );

	//! Constructs a 6x6 matrix from four 3x3 matrices by combining their rows.
	explicit idMat6( const idMat3& m0, const idMat3& m1, const idMat3& m2, const idMat3& m3 );

	//! Constructs a 6x6 matrix from a 2D array of floats.
	explicit idMat6( const float src[6][6] );

	//! Returns a const reference to the idVec6 element at the specified index in the idMat6 matrix
	const idVec6&  operator[]( int index ) const;

	//! Provides indexed access to the rows of the 6x6 matrix
	idVec6&		   operator[]( int index );

	//! Multiplies this 6x6 matrix by a scalar value and returns the result.
	idMat6		   operator*( const float a ) const;

	//! Multiplies this 6x6 matrix by the given 6D vector and returns the resulting vector.
	idVec6		   operator*( const idVec6& vec ) const;

	//! Returns the matrix product of this matrix and the given matrix.
	idMat6		   operator*( const idMat6& a ) const;

	//! Returns the result of adding this matrix to another matrix.
	idMat6		   operator+( const idMat6& a ) const;

	//! Returns the element-wise difference between this matrix and another matrix.
	idMat6		   operator-( const idMat6& a ) const;

	//! Multiplies all elements of the 6x6 matrix by the given scalar value and returns a reference to the modified matrix.
	idMat6&		   operator*=( const float a );

	//! Multiplies this matrix by another matrix and assigns the result to this matrix
	idMat6&		   operator*=( const idMat6& a );

	//! Adds the elements of the given matrix to this matrix and returns a reference to this matrix.
	idMat6&		   operator+=( const idMat6& a );

	//! Subtracts each element of the input matrix from the corresponding element of this matrix and returns a reference to this matrix.
	idMat6&		   operator-=( const idMat6& a );

	friend idMat6  operator*( const float a, const idMat6& mat );
	friend idVec6  operator*( const idVec6& vec, const idMat6& mat );
	friend idVec6& operator*=( idVec6& vec, const idMat6& mat );

	//! Compares this matrix with another matrix for equality.
	bool		   Compare( const idMat6& a ) const;

	//! Compares this matrix with another matrix using the specified epsilon tolerance
	bool		   Compare( const idMat6& a, const float epsilon ) const;

	//! Compares two idMat6 matrices for equality.
	bool		   operator==( const idMat6& a ) const;

	//! Returns true if this matrix is not equal to the given matrix.
	bool		   operator!=( const idMat6& a ) const;

	//! Initializes all elements of the matrix to zero.
	void		   Zero();

	//! Sets the matrix to the 6x6 identity matrix.
	void		   Identity();

	//! Checks if the matrix is identity within a specified epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the 6x6 matrix is symmetric within a given epsilon tolerance.
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the 6x6 matrix is diagonal within the given epsilon tolerance.
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns a 3x3 sub-matrix from this 6x6 matrix at the specified index.
	idMat3		   SubMat3( int n ) const;

	//! Returns the trace of the 6x6 matrix, which is the sum of its diagonal elements.
	float		   Trace() const;

	//! Computes and returns the determinant of the 6x6 matrix.
	float		   Determinant() const;

	//! Returns the transpose of this 6x6 matrix
	idMat6		   Transpose() const;

	//! Transposes the matrix in place and returns a reference to itself.
	idMat6&		   TransposeSelf();

	//! Returns the inverse of this matrix
	idMat6		   Inverse() const;

	//! Computes the inverse of the 6x6 matrix in-place and returns true if successful, false if the matrix is singular.
	bool		   InverseSelf();

	//! Returns the inverse of the matrix
	idMat6		   InverseFast() const;

	//! Computes the inverse of the 6x6 matrix in-place and returns true if successful, false if the matrix is singular.
	bool		   InverseFastSelf();

	//! Returns the dimension of the 6x6 matrix, which is always 36.
	int			   GetDimension() const;

	//! Returns a pointer to the float representation of the matrix data
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the first element of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision
	const char*	   ToString( int precision = 2 ) const;

private:
	idVec6 mat[6];
};

extern idMat6 mat6_zero;
extern idMat6 mat6_identity;
#define mat6_default mat6_identity

ID_INLINE idMat6::idMat6()
{
}

ID_INLINE idMat6::idMat6( const idMat3& m0, const idMat3& m1, const idMat3& m2, const idMat3& m3 )
{
	mat[0] = idVec6( m0[0][0], m0[0][1], m0[0][2], m1[0][0], m1[0][1], m1[0][2] );
	mat[1] = idVec6( m0[1][0], m0[1][1], m0[1][2], m1[1][0], m1[1][1], m1[1][2] );
	mat[2] = idVec6( m0[2][0], m0[2][1], m0[2][2], m1[2][0], m1[2][1], m1[2][2] );
	mat[3] = idVec6( m2[0][0], m2[0][1], m2[0][2], m3[0][0], m3[0][1], m3[0][2] );
	mat[4] = idVec6( m2[1][0], m2[1][1], m2[1][2], m3[1][0], m3[1][1], m3[1][2] );
	mat[5] = idVec6( m2[2][0], m2[2][1], m2[2][2], m3[2][0], m3[2][1], m3[2][2] );
}

ID_INLINE idMat6::idMat6( const idVec6& v0, const idVec6& v1, const idVec6& v2, const idVec6& v3, const idVec6& v4, const idVec6& v5 )
{
	mat[0] = v0;
	mat[1] = v1;
	mat[2] = v2;
	mat[3] = v3;
	mat[4] = v4;
	mat[5] = v5;
}

ID_INLINE idMat6::idMat6( const float src[6][6] )
{
	memcpy( mat, src, 6 * 6 * sizeof( float ) );
}

ID_INLINE const idVec6& idMat6::operator[]( int index ) const
{
	// assert( ( index >= 0 ) && ( index < 6 ) );
	return mat[index];
}

ID_INLINE idVec6& idMat6::operator[]( int index )
{
	// assert( ( index >= 0 ) && ( index < 6 ) );
	return mat[index];
}

ID_INLINE idMat6 idMat6::operator*( const idMat6& a ) const
{
	int			 i, j;
	const float *m1Ptr, *m2Ptr;
	float*		 dstPtr;
	idMat6		 dst;

	m1Ptr  = reinterpret_cast<const float*>( this );
	m2Ptr  = reinterpret_cast<const float*>( &a );
	dstPtr = reinterpret_cast<float*>( &dst );

	for( i = 0; i < 6; i++ ) {
		for( j = 0; j < 6; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[0 * 6 + j] + m1Ptr[1] * m2Ptr[1 * 6 + j] + m1Ptr[2] * m2Ptr[2 * 6 + j] + m1Ptr[3] * m2Ptr[3 * 6 + j] + m1Ptr[4] * m2Ptr[4 * 6 + j] + m1Ptr[5] * m2Ptr[5 * 6 + j];
			dstPtr++;
		}
		m1Ptr += 6;
	}
	return dst;
}

ID_INLINE idMat6 idMat6::operator*( const float a ) const
{
	return idMat6( idVec6( mat[0][0] * a, mat[0][1] * a, mat[0][2] * a, mat[0][3] * a, mat[0][4] * a, mat[0][5] * a ),
		idVec6( mat[1][0] * a, mat[1][1] * a, mat[1][2] * a, mat[1][3] * a, mat[1][4] * a, mat[1][5] * a ),
		idVec6( mat[2][0] * a, mat[2][1] * a, mat[2][2] * a, mat[2][3] * a, mat[2][4] * a, mat[2][5] * a ),
		idVec6( mat[3][0] * a, mat[3][1] * a, mat[3][2] * a, mat[3][3] * a, mat[3][4] * a, mat[3][5] * a ),
		idVec6( mat[4][0] * a, mat[4][1] * a, mat[4][2] * a, mat[4][3] * a, mat[4][4] * a, mat[4][5] * a ),
		idVec6( mat[5][0] * a, mat[5][1] * a, mat[5][2] * a, mat[5][3] * a, mat[5][4] * a, mat[5][5] * a ) );
}

ID_INLINE idVec6 idMat6::operator*( const idVec6& vec ) const
{
	return idVec6( mat[0][0] * vec[0] + mat[0][1] * vec[1] + mat[0][2] * vec[2] + mat[0][3] * vec[3] + mat[0][4] * vec[4] + mat[0][5] * vec[5],
		mat[1][0] * vec[0] + mat[1][1] * vec[1] + mat[1][2] * vec[2] + mat[1][3] * vec[3] + mat[1][4] * vec[4] + mat[1][5] * vec[5],
		mat[2][0] * vec[0] + mat[2][1] * vec[1] + mat[2][2] * vec[2] + mat[2][3] * vec[3] + mat[2][4] * vec[4] + mat[2][5] * vec[5],
		mat[3][0] * vec[0] + mat[3][1] * vec[1] + mat[3][2] * vec[2] + mat[3][3] * vec[3] + mat[3][4] * vec[4] + mat[3][5] * vec[5],
		mat[4][0] * vec[0] + mat[4][1] * vec[1] + mat[4][2] * vec[2] + mat[4][3] * vec[3] + mat[4][4] * vec[4] + mat[4][5] * vec[5],
		mat[5][0] * vec[0] + mat[5][1] * vec[1] + mat[5][2] * vec[2] + mat[5][3] * vec[3] + mat[5][4] * vec[4] + mat[5][5] * vec[5] );
}

ID_INLINE idMat6 idMat6::operator+( const idMat6& a ) const
{
	return idMat6( idVec6( mat[0][0] + a[0][0], mat[0][1] + a[0][1], mat[0][2] + a[0][2], mat[0][3] + a[0][3], mat[0][4] + a[0][4], mat[0][5] + a[0][5] ),
		idVec6( mat[1][0] + a[1][0], mat[1][1] + a[1][1], mat[1][2] + a[1][2], mat[1][3] + a[1][3], mat[1][4] + a[1][4], mat[1][5] + a[1][5] ),
		idVec6( mat[2][0] + a[2][0], mat[2][1] + a[2][1], mat[2][2] + a[2][2], mat[2][3] + a[2][3], mat[2][4] + a[2][4], mat[2][5] + a[2][5] ),
		idVec6( mat[3][0] + a[3][0], mat[3][1] + a[3][1], mat[3][2] + a[3][2], mat[3][3] + a[3][3], mat[3][4] + a[3][4], mat[3][5] + a[3][5] ),
		idVec6( mat[4][0] + a[4][0], mat[4][1] + a[4][1], mat[4][2] + a[4][2], mat[4][3] + a[4][3], mat[4][4] + a[4][4], mat[4][5] + a[4][5] ),
		idVec6( mat[5][0] + a[5][0], mat[5][1] + a[5][1], mat[5][2] + a[5][2], mat[5][3] + a[5][3], mat[5][4] + a[5][4], mat[5][5] + a[5][5] ) );
}

ID_INLINE idMat6 idMat6::operator-( const idMat6& a ) const
{
	return idMat6( idVec6( mat[0][0] - a[0][0], mat[0][1] - a[0][1], mat[0][2] - a[0][2], mat[0][3] - a[0][3], mat[0][4] - a[0][4], mat[0][5] - a[0][5] ),
		idVec6( mat[1][0] - a[1][0], mat[1][1] - a[1][1], mat[1][2] - a[1][2], mat[1][3] - a[1][3], mat[1][4] - a[1][4], mat[1][5] - a[1][5] ),
		idVec6( mat[2][0] - a[2][0], mat[2][1] - a[2][1], mat[2][2] - a[2][2], mat[2][3] - a[2][3], mat[2][4] - a[2][4], mat[2][5] - a[2][5] ),
		idVec6( mat[3][0] - a[3][0], mat[3][1] - a[3][1], mat[3][2] - a[3][2], mat[3][3] - a[3][3], mat[3][4] - a[3][4], mat[3][5] - a[3][5] ),
		idVec6( mat[4][0] - a[4][0], mat[4][1] - a[4][1], mat[4][2] - a[4][2], mat[4][3] - a[4][3], mat[4][4] - a[4][4], mat[4][5] - a[4][5] ),
		idVec6( mat[5][0] - a[5][0], mat[5][1] - a[5][1], mat[5][2] - a[5][2], mat[5][3] - a[5][3], mat[5][4] - a[5][4], mat[5][5] - a[5][5] ) );
}

ID_INLINE idMat6& idMat6::operator*=( const float a )
{
	mat[0][0] *= a;
	mat[0][1] *= a;
	mat[0][2] *= a;
	mat[0][3] *= a;
	mat[0][4] *= a;
	mat[0][5] *= a;
	mat[1][0] *= a;
	mat[1][1] *= a;
	mat[1][2] *= a;
	mat[1][3] *= a;
	mat[1][4] *= a;
	mat[1][5] *= a;
	mat[2][0] *= a;
	mat[2][1] *= a;
	mat[2][2] *= a;
	mat[2][3] *= a;
	mat[2][4] *= a;
	mat[2][5] *= a;
	mat[3][0] *= a;
	mat[3][1] *= a;
	mat[3][2] *= a;
	mat[3][3] *= a;
	mat[3][4] *= a;
	mat[3][5] *= a;
	mat[4][0] *= a;
	mat[4][1] *= a;
	mat[4][2] *= a;
	mat[4][3] *= a;
	mat[4][4] *= a;
	mat[4][5] *= a;
	mat[5][0] *= a;
	mat[5][1] *= a;
	mat[5][2] *= a;
	mat[5][3] *= a;
	mat[5][4] *= a;
	mat[5][5] *= a;
	return *this;
}

ID_INLINE idMat6& idMat6::operator*=( const idMat6& a )
{
	*this = *this * a;
	return *this;
}

ID_INLINE idMat6& idMat6::operator+=( const idMat6& a )
{
	mat[0][0] += a[0][0];
	mat[0][1] += a[0][1];
	mat[0][2] += a[0][2];
	mat[0][3] += a[0][3];
	mat[0][4] += a[0][4];
	mat[0][5] += a[0][5];
	mat[1][0] += a[1][0];
	mat[1][1] += a[1][1];
	mat[1][2] += a[1][2];
	mat[1][3] += a[1][3];
	mat[1][4] += a[1][4];
	mat[1][5] += a[1][5];
	mat[2][0] += a[2][0];
	mat[2][1] += a[2][1];
	mat[2][2] += a[2][2];
	mat[2][3] += a[2][3];
	mat[2][4] += a[2][4];
	mat[2][5] += a[2][5];
	mat[3][0] += a[3][0];
	mat[3][1] += a[3][1];
	mat[3][2] += a[3][2];
	mat[3][3] += a[3][3];
	mat[3][4] += a[3][4];
	mat[3][5] += a[3][5];
	mat[4][0] += a[4][0];
	mat[4][1] += a[4][1];
	mat[4][2] += a[4][2];
	mat[4][3] += a[4][3];
	mat[4][4] += a[4][4];
	mat[4][5] += a[4][5];
	mat[5][0] += a[5][0];
	mat[5][1] += a[5][1];
	mat[5][2] += a[5][2];
	mat[5][3] += a[5][3];
	mat[5][4] += a[5][4];
	mat[5][5] += a[5][5];
	return *this;
}

ID_INLINE idMat6& idMat6::operator-=( const idMat6& a )
{
	mat[0][0] -= a[0][0];
	mat[0][1] -= a[0][1];
	mat[0][2] -= a[0][2];
	mat[0][3] -= a[0][3];
	mat[0][4] -= a[0][4];
	mat[0][5] -= a[0][5];
	mat[1][0] -= a[1][0];
	mat[1][1] -= a[1][1];
	mat[1][2] -= a[1][2];
	mat[1][3] -= a[1][3];
	mat[1][4] -= a[1][4];
	mat[1][5] -= a[1][5];
	mat[2][0] -= a[2][0];
	mat[2][1] -= a[2][1];
	mat[2][2] -= a[2][2];
	mat[2][3] -= a[2][3];
	mat[2][4] -= a[2][4];
	mat[2][5] -= a[2][5];
	mat[3][0] -= a[3][0];
	mat[3][1] -= a[3][1];
	mat[3][2] -= a[3][2];
	mat[3][3] -= a[3][3];
	mat[3][4] -= a[3][4];
	mat[3][5] -= a[3][5];
	mat[4][0] -= a[4][0];
	mat[4][1] -= a[4][1];
	mat[4][2] -= a[4][2];
	mat[4][3] -= a[4][3];
	mat[4][4] -= a[4][4];
	mat[4][5] -= a[4][5];
	mat[5][0] -= a[5][0];
	mat[5][1] -= a[5][1];
	mat[5][2] -= a[5][2];
	mat[5][3] -= a[5][3];
	mat[5][4] -= a[5][4];
	mat[5][5] -= a[5][5];
	return *this;
}

//! Multiplies a 6D vector by a 6x6 matrix and returns the resulting vector.
ID_INLINE idVec6 operator*( const idVec6& vec, const idMat6& mat )
{
	return mat * vec;
}

//! Multiplies a 6x6 matrix by a scalar value.
ID_INLINE idMat6 operator*( const float a, idMat6 const& mat )
{
	return mat * a;
}

//! Multiplies the given vector by the matrix and assigns the result back to the vector.
ID_INLINE idVec6& operator*=( idVec6& vec, const idMat6& mat )
{
	vec = mat * vec;
	return vec;
}

ID_INLINE bool idMat6::Compare( const idMat6& a ) const
{
	dword		 i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float*>( mat );
	ptr2 = reinterpret_cast<const float*>( a.mat );
	for( i = 0; i < 6 * 6; i++ ) {
		if( ptr1[i] != ptr2[i] ) { return false; }
	}
	return true;
}

ID_INLINE bool idMat6::Compare( const idMat6& a, const float epsilon ) const
{
	dword		 i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float*>( mat );
	ptr2 = reinterpret_cast<const float*>( a.mat );
	for( i = 0; i < 6 * 6; i++ ) {
		if( idMath::Fabs( ptr1[i] - ptr2[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idMat6::operator==( const idMat6& a ) const
{
	return Compare( a );
}

ID_INLINE bool idMat6::operator!=( const idMat6& a ) const
{
	return !Compare( a );
}

ID_INLINE void idMat6::Zero()
{
	memset( mat, 0, sizeof( idMat6 ) );
}

ID_INLINE void idMat6::Identity()
{
	*this = mat6_identity;
}

ID_INLINE bool idMat6::IsIdentity( const float epsilon ) const
{
	return Compare( mat6_identity, epsilon );
}

ID_INLINE bool idMat6::IsSymmetric( const float epsilon ) const
{
	for( int i = 1; i < 6; i++ ) {
		for( int j = 0; j < i; j++ ) {
			if( idMath::Fabs( mat[i][j] - mat[j][i] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMat6::IsDiagonal( const float epsilon ) const
{
	for( int i = 0; i < 6; i++ ) {
		for( int j = 0; j < 6; j++ ) {
			if( i != j && idMath::Fabs( mat[i][j] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE idMat3 idMat6::SubMat3( int n ) const
{
	assert( n >= 0 && n < 4 );
	int b0 = ( ( n & 2 ) >> 1 ) * 3;
	int b1 = ( n & 1 ) * 3;
	return idMat3(
		mat[b0 + 0][b1 + 0], mat[b0 + 0][b1 + 1], mat[b0 + 0][b1 + 2], mat[b0 + 1][b1 + 0], mat[b0 + 1][b1 + 1], mat[b0 + 1][b1 + 2], mat[b0 + 2][b1 + 0], mat[b0 + 2][b1 + 1], mat[b0 + 2][b1 + 2] );
}

ID_INLINE float idMat6::Trace() const
{
	return ( mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3] + mat[4][4] + mat[5][5] );
}

ID_INLINE idMat6 idMat6::Inverse() const
{
	idMat6 invMat;

	invMat = *this;
	verify( invMat.InverseSelf() );
	return invMat;
}

ID_INLINE idMat6 idMat6::InverseFast() const
{
	idMat6 invMat;

	invMat = *this;
	verify( invMat.InverseFastSelf() );
	return invMat;
}

ID_INLINE int idMat6::GetDimension() const
{
	return 36;
}

ID_INLINE const float* idMat6::ToFloatPtr() const
{
	return mat[0].ToFloatPtr();
}

ID_INLINE float* idMat6::ToFloatPtr()
{
	return mat[0].ToFloatPtr();
}

#endif /* !__MATH_MATRIX_H__ */
