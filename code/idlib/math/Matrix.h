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
	\brief A 2x2 matrix class for linear algebra operations in the engine.

	The idMat2 class represents a 2x2 matrix commonly used for 2D linear algebra operations within the engine. It provides multiple constructors for flexible initialization including from vectors,
   individual float values, and array data. The class supports standard matrix operations such as addition, subtraction, multiplication, transpose, and inversion. It includes utility methods for
   checking matrix properties like identity, symmetry, and diagonal nature. The implementation is designed for performance with inline methods for common operations and provides both safe and fast
   inversion methods. The class integrates with the engine's vector types and follows standard conventions for matrix representation and operations.

*/
class idMat2
{
public:
	/*!
		\brief Constructs an uninitialized 2x2 matrix.

		This constructor initializes a 2x2 matrix without setting any of its elements to specific values. The matrix will contain garbage values until explicitly initialized or assigned.

	*/
	idMat2();

	/*!
		\brief Initializes a 2x2 matrix using two 2D vectors as rows

		Constructs a 2x2 matrix by using the provided vectors as the rows of the matrix. The first vector x becomes the first row of the matrix, and the second vector y becomes the second row. Each
	   component of the vectors is directly assigned to the corresponding matrix element.

		\param x The first 2D vector to be used as the first row of the matrix
		\param y The second 2D vector to be used as the second row of the matrix
	*/
	explicit idMat2( const idVec2& x, const idVec2& y );

	/*!
		\brief Constructs a 2x2 matrix initialized with the specified float values.

		This constructor initializes a 2x2 matrix using four float values. The matrix is stored in row-major order where xx represents the value at row 0, column 0; xy represents the value at row 0,
	   column 1; yx represents the value at row 1, column 0; and yy represents the value at row 1, column 1.

		\param xx the value at row 0, column 0 of the matrix
		\param xy the value at row 0, column 1 of the matrix
		\param yx the value at row 1, column 0 of the matrix
		\param yy the value at row 1, column 1 of the matrix
	*/
	explicit idMat2( const float xx, const float xy, const float yx, const float yy );

	/*!
		\brief Initializes a 2x2 matrix by copying values from a 2D float array

		This constructor initializes a 2x2 matrix by copying the values from a 2D float array. The matrix is stored in row-major order and the constructor copies 4 float values from the source array
	   to the internal matrix representation.

		\param src A 2D float array containing the values to copy into the matrix
	*/
	explicit idMat2( const float src[2][2] );

	const idVec2&  operator[]( int index ) const;
	idVec2&		   operator[]( int index );
	idMat2		   operator-() const;
	idMat2		   operator*( const float a ) const;
	idVec2		   operator*( const idVec2& vec ) const;
	idMat2		   operator*( const idMat2& a ) const;
	idMat2		   operator+( const idMat2& a ) const;
	idMat2		   operator-( const idMat2& a ) const;
	idMat2&		   operator*=( const float a );
	idMat2&		   operator*=( const idMat2& a );
	idMat2&		   operator+=( const idMat2& a );
	idMat2&		   operator-=( const idMat2& a );

	friend idMat2  operator*( const float a, const idMat2& mat );
	friend idVec2  operator*( const idVec2& vec, const idMat2& mat );
	friend idVec2& operator*=( idVec2& vec, const idMat2& mat );

	/*!
		\brief Compares this matrix with another matrix for exact equality.

		This function performs an exact comparison between the current matrix and the provided matrix. It returns true if all elements of the two matrices are exactly equal, and false otherwise. The
	   comparison is done element by element using the Compare method of the underlying vector type.

		\param a The matrix to compare with this matrix
		\return true if all elements of the two matrices are exactly equal, false otherwise
	*/
	bool		   Compare( const idMat2& a ) const;

	/*!
		\brief Compares this matrix with another matrix using the specified epsilon value for floating-point tolerance.

		This function performs an element-wise comparison between the current matrix and the provided matrix using the given epsilon value to account for floating-point precision errors. It returns
	   true if all corresponding elements between the two matrices are within the specified epsilon tolerance, and false otherwise.

		\param a The matrix to compare against
		\param epsilon The tolerance value for floating-point comparison
		\return True if all elements of this matrix are within the epsilon tolerance of the corresponding elements in matrix 'a', false otherwise
	*/
	bool		   Compare( const idMat2& a, const float epsilon ) const;
	bool		   operator==( const idMat2& a ) const; // exact compare, no epsilon
	bool		   operator!=( const idMat2& a ) const; // exact compare, no epsilon

	//! Sets all elements of the 2x2 matrix to zero.
	void		   Zero();

	/*!
		\brief Sets the matrix to the 2x2 identity matrix.

		This function initializes the current matrix object to the standard 2x2 identity matrix, which has ones on the diagonal and zeros elsewhere. The operation is performed by assigning the
	   pre-defined identity matrix constant to the current object.

	*/
	void		   Identity();

	//! Checks if the matrix is equal to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric within the given epsilon tolerance by comparing off-diagonal elements.

		The function determines if a 2x2 matrix is symmetric by comparing the elements at positions [0][1] and [1][0]. If the absolute difference between these elements is less than the provided
	   epsilon tolerance, the matrix is considered symmetric. This is a common check for symmetric matrices in linear algebra operations.

		\param epsilon tolerance value for comparing off-diagonal matrix elements
		\return true if the matrix is symmetric within the specified epsilon tolerance, false otherwise
	*/
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is diagonal by comparing off-diagonal elements against the specified epsilon

		Returns true if the matrix is diagonal, meaning that the off-diagonal elements are smaller than or equal to the given epsilon threshold. The function compares the absolute values of the
	   off-diagonal elements (mat[0][1] and mat[1][0]) against the epsilon value. If either of these values exceeds epsilon, the matrix is not considered diagonal and the function returns false.
	   Otherwise, it returns true.

		\param epsilon The threshold value for comparison of off-diagonal elements
		\return True if the matrix is diagonal, false otherwise
	*/
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Returns the trace of the 2x2 matrix, which is the sum of its diagonal elements.

		This function computes the mathematical trace of a 2x2 matrix. The trace is defined as the sum of the diagonal elements of the matrix. For a matrix with elements mat[0][0], mat[0][1],
	   mat[1][0], and mat[1][1], the trace is calculated as mat[0][0] + mat[1][1]. This operation is commonly used in linear algebra and has applications in various mathematical and engineering
	   computations.

		\return The sum of the diagonal elements of the 2x2 matrix
	*/
	float		   Trace() const;

	/*!
		\brief Computes and returns the determinant of the 2x2 matrix.

		The function calculates the determinant using the standard formula for a 2x2 matrix, which is the product of the diagonal elements minus the product of the off-diagonal elements. The matrix is
	   represented as a 2x2 array of floats.

		\return The determinant value of the 2x2 matrix.
	*/
	float		   Determinant() const;

	//! Returns the transpose of this 2x2 matrix.
	idMat2		   Transpose() const;

	/*!
		\brief Transposes the matrix in-place and returns a reference to itself.
		\return A reference to the transposed matrix
	*/
	idMat2&		   TransposeSelf();

	/*!
		\brief Returns the inverse of the 2x2 matrix.

		This function computes the inverse of the current 2x2 matrix. It creates a copy of the matrix, attempts to invert it in place, and returns the inverted matrix. If the determinant of the matrix
	   is zero, the inversion cannot be performed and the function may return an invalid matrix.

		\return The inverse of the current 2x2 matrix.
		\throws This function may throw an assertion error if the matrix inversion fails due to a zero determinant.
	*/
	idMat2		   Inverse() const;

	/*!
		\brief Returns false if the determinant is zero, otherwise inverts the matrix in place and returns true.

		This function computes the inverse of the 2x2 matrix stored in the object. It calculates the determinant and checks if it is below a predefined epsilon value, indicating a singular matrix. If
	   the matrix is singular, it returns false. Otherwise, it computes the inverse in place and returns true. The inversion uses the standard formula for 2x2 matrices, involving the determinant and
	   cofactors.

		\return true if the matrix was successfully inverted, false if the matrix is singular (determinant is zero or close to zero)
	*/
	bool		   InverseSelf();

	/*!
		\brief Returns the inverse of this matrix by inverting it in place

		This function computes the inverse of the current 2x2 matrix by first creating a copy of the matrix and then calling the fast inverse method on the copy. It returns true if the operation was
	   successful, indicating that the determinant is not zero, and false if the matrix is singular and cannot be inverted. The function is marked as ID_INLINE, suggesting it is intended to be inlined
	   for performance.

		\return A new idMat2 matrix that is the inverse of the current matrix
	*/
	idMat2		   InverseFast() const;

	/*!
		\brief Inverts the 2x2 matrix in place and returns true if successful, false if the determinant is zero.

		This function performs a fast inversion of a 2x2 matrix stored in the object. It calculates the determinant and checks if it's non-zero using a epsilon threshold. If the determinant is zero,
	   the function returns false. Otherwise, it computes the inverse matrix in place and returns true. The function uses a more efficient algorithm than the alternative implementation which is
	   conditionally compiled.

		\return true if the matrix was successfully inverted, false if the determinant is zero
	*/
	bool		   InverseFastSelf();

	//! Returns the dimension of the matrix.
	int			   GetDimension() const;

	//! Returns a pointer to the internal float array representation of the matrix.
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

ID_INLINE idVec2 operator*( const idVec2& vec, const idMat2& mat )
{
	return mat * vec;
}

ID_INLINE idMat2 operator*( const float a, idMat2 const& mat )
{
	return mat * a;
}

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
	\brief A 3x3 matrix class for representing and manipulating 3D transformations in the engine.

	\note matrix is column-major

	The idMat3 class provides a comprehensive set of tools for working with 3x3 matrices, primarily used for 3D transformations including rotations, scaling, and projections. It supports multiple
   construction methods including from vectors, arrays, and individual elements, allowing flexibility in matrix initialization. The class offers standard arithmetic operations, comparison functions,
   and geometric utility methods such as trace, determinant, and various normalization techniques. It includes specialized methods for working with inertia tensors, which are essential for physics
   simulations, supporting translation and rotation operations. The class also provides conversion functions to represent the matrix in other geometric formats like Euler angles, quaternions, and
   rotation objects. Memory management is handled through standard C++ mechanisms with no explicit ownership semantics, and the class is designed for both performance and usability in the engine's
   graphics and physics systems.

*/
class idMat3
{
public:
	/*!
		\brief Constructs an uninitialized 3x3 matrix.

		This constructor creates a 3x3 matrix without initializing its values. The matrix is left in an undefined state and should be populated with data before use. It is typically used when the
	   matrix will be immediately assigned values or when the uninitialized state is acceptable for the specific use case.

	*/
	idMat3();

	/*!
		\brief Creates a 3x3 matrix from three 3D vectors representing the rows of the matrix

		Initializes a 3x3 matrix using the provided three 3D vectors as rows. Each vector contributes to one row of the matrix, with the first vector becoming the first row, the second vector becoming
	   the second row, and the third vector becoming the third row. The matrix is stored in row-major format where each row is represented as a vector.

		\param x First row of the matrix represented as a 3D vector
		\param y Second row of the matrix represented as a 3D vector
		\param z Third row of the matrix represented as a 3D vector
	*/
	explicit idMat3( const idVec3& x, const idVec3& y, const idVec3& z );

	/*!
		\brief Initializes a 3x3 matrix with the specified column-major components.

		This constructor initializes a 3x3 matrix using column-major order. The parameters represent the matrix elements in the following order: xx, xy, xz for the first column; yx, yy, yz for the
	   second column; and zx, zy, zz for the third column. The matrix is stored in column-major format where each column is represented as a vector.

		\param xx The element at row 0, column 0 of the matrix
		\param xy The element at row 1, column 0 of the matrix
		\param xz The element at row 2, column 0 of the matrix
		\param yx The element at row 0, column 1 of the matrix
		\param yy The element at row 1, column 1 of the matrix
		\param yz The element at row 2, column 1 of the matrix
		\param zx The element at row 0, column 2 of the matrix
		\param zy The element at row 1, column 2 of the matrix
		\param zz The element at row 2, column 2 of the matrix
	*/
	explicit idMat3( const float xx, const float xy, const float xz, const float yx, const float yy, const float yz, const float zx, const float zy, const float zz );

	/*!
		\brief Constructs a 3x3 matrix from a 2D array of floats.

		This constructor initializes a 3x3 matrix by copying the values from a given 2D array of floats. The input array is expected to be a 3x3 structure, and the constructor copies all elements into
	   the internal matrix representation.

		\param src A 2D array of floats representing the elements of the 3x3 matrix
	*/
	explicit idMat3( const float src[3][3] );

	const idVec3&  operator[]( int index ) const;
	idVec3&		   operator[]( int index );
	idMat3		   operator-() const;
	idMat3		   operator*( const float a ) const;
	idVec3		   operator*( const idVec3& vec ) const;
	idMat3		   operator*( const idMat3& a ) const;
	idMat3		   operator+( const idMat3& a ) const;
	idMat3		   operator-( const idMat3& a ) const;
	idMat3&		   operator*=( const float a );
	idMat3&		   operator*=( const idMat3& a );
	idMat3&		   operator+=( const idMat3& a );
	idMat3&		   operator-=( const idMat3& a );

	friend idMat3  operator*( const float a, const idMat3& mat );
	friend idVec3  operator*( const idVec3& vec, const idMat3& mat );
	friend idVec3& operator*=( idVec3& vec, const idMat3& mat );

	/*!
		\brief Compares this matrix with another matrix for exact equality.

		This function performs an exact comparison between the current matrix and the provided matrix. It checks if all three rows of the matrix are exactly equal using the Compare method of idVec3.
	   The function returns true only if all rows match exactly, otherwise it returns false.

		\param a The matrix to compare with this matrix
		\return true if all rows of the matrices are exactly equal, false otherwise
	*/
	bool		   Compare( const idMat3& a ) const;

	/*!
		\brief Compares this matrix with another matrix using the specified epsilon value for floating-point comparison.

		This function performs an element-wise comparison between the current matrix and the provided matrix using the specified epsilon value for floating-point tolerance. It returns true if all
	   corresponding elements of the two matrices are within the given epsilon tolerance, and false otherwise. The comparison is done on each row of the matrix.

		\param a The matrix to compare with this matrix
		\param epsilon The epsilon value to use for floating-point comparison tolerance
		\return true if all corresponding elements of the two matrices are within the given epsilon tolerance, false otherwise
	*/
	bool		   Compare( const idMat3& a, const float epsilon ) const;
	bool		   operator==( const idMat3& a ) const; // exact compare, no epsilon
	bool		   operator!=( const idMat3& a ) const; // exact compare, no epsilon

	//! Sets all elements of the matrix to zero.
	void		   Zero();

	/*!
		\brief Sets the matrix to the 3x3 identity matrix.

		This function initializes the current matrix object to the standard 3x3 identity matrix, where all diagonal elements are set to 1.0 and all off-diagonal elements are set to 0.0. The operation
	   is performed by assigning the predefined identity matrix constant mat3_identity to the current object.

	*/
	void		   Identity();

	//! Checks if the matrix is equal to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric by comparing off-diagonal elements within the specified epsilon tolerance.

		The function determines if a 3x3 matrix is symmetric by comparing each pair of off-diagonal elements. For a matrix to be symmetric, the element at position (i,j) must equal the element at
	   position (j,i) for all valid indices. The comparison uses the provided epsilon value to handle floating-point precision issues. If any pair of elements differs by more than epsilon, the
	   function returns false. Otherwise, it returns true.

		\param epsilon The tolerance value for floating-point comparison of matrix elements
		\return True if the matrix is symmetric within the given epsilon tolerance, false otherwise
	*/
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is diagonal by comparing off-diagonal elements against the specified epsilon threshold

		Returns true if all off-diagonal elements of the 3x3 matrix are smaller than the given epsilon value, indicating the matrix is effectively diagonal. The function examines all six off-diagonal
	   elements (mat[0][1], mat[0][2], mat[1][0], mat[1][2], mat[2][0], mat[2][1]) and returns false if any element exceeds the epsilon threshold

		\param epsilon threshold value for floating-point comparison of off-diagonal elements
		\return true if the matrix is diagonal within the given epsilon tolerance, false otherwise
	*/
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is not identical to the identity matrix.
	bool		   IsRotated() const;

	/*!
		\brief Projects a 3D vector onto the rows of this matrix by transforming the vector using the matrix rows.

		This function performs a vector transformation by multiplying the input vector with each row of the matrix. The result is stored in the destination vector where each component of the
	   destination corresponds to the dot product of the input vector with the respective row of the matrix.

		\param src The input 3D vector to be projected
		\param dst The output 3D vector that will contain the projected result
	*/
	void		   ProjectVector( const idVec3& src, idVec3& dst ) const;

	/*!
		\brief Transforms a vector using the matrix by multiplying the matrix columns with the vector components.

		This function performs a vector transformation by multiplying each column of the matrix with the corresponding component of the input vector. The result is stored in the destination vector.
	   The operation is equivalent to dst = mat[0] * src.x + mat[1] * src.y + mat[2] * src.z where mat[0], mat[1], and mat[2] represent the columns of the matrix.

		\param src The input vector to be transformed
		\param dst The output vector that will contain the transformed result
	*/
	void		   UnprojectVector( const idVec3& src, idVec3& dst ) const;

	/*!
		\brief Fixes degenerate normal cases in the matrix by setting tiny numbers to zero and returns whether any fixes were applied.

		This function addresses degenerate normal cases within the 3x3 matrix by processing each row vector to correct tiny numerical values that could cause issues in geometric computations. It
	   checks each of the three row vectors in the matrix and applies normalization fixes if necessary. The function returns true if any of the row vectors required correction, indicating that
	   degenerate cases were present and fixed. This is commonly used in computational geometry and 3D graphics applications to ensure matrix integrity when dealing with floating-point precision
	   issues.

		\return True if any of the matrix rows required degeneracy fixes, false otherwise
	*/
	bool		   FixDegeneracies();

	//! Sets tiny numbers in the matrix to zero.
	bool		   FixDenormals();

	/*!
		\brief Returns the trace of the 3x3 matrix, which is the sum of its diagonal elements.

		This function computes the trace of a 3x3 matrix by summing the diagonal elements mat[0][0], mat[1][1], and mat[2][2]. The trace is a fundamental property of square matrices and is often used
	   in linear algebra and graphics computations.

		\return The sum of the diagonal elements of the matrix
	*/
	float		   Trace() const;

	/*!
		\brief Calculates and returns the determinant of the 3x3 matrix.

		This function computes the determinant of a 3x3 matrix using the standard cofactor expansion method. It calculates the determinants of the 2x2 submatrices and combines them with the
	   corresponding elements of the first row to compute the final result. The calculation follows the formula: det(A) = a11*M11 - a12*M12 + a13*M13, where Mij represents the minor of element aij.

		\return The determinant value of the 3x3 matrix as a floating-point number.
	*/
	float		   Determinant() const;

	/*!
		\brief Returns an orthonormalized copy of this matrix using Gram-Schmidt orthonormalization.

		This function performs Gram-Schmidt orthonormalization on the rows of the matrix. It first normalizes the first row, then computes the third row as the cross product of the first and second
	   rows, normalizes it, and finally recomputes the second row as the cross product of the third and first rows, normalizing it. This ensures that the resulting matrix has orthonormal rows. The
	   function returns a copy of the orthonormalized matrix.

		\return A new idMat3 matrix that is orthonormalized
	*/
	idMat3		   OrthoNormalize() const;

	/*!
		\brief Normalizes the rows of the matrix to form an orthonormal basis and returns a reference to the matrix.

		This function performs Gram-Schmidt orthonormalization on the rows of the matrix. It first normalizes the first row, then computes the third row as the cross product of the first and second
	   rows, normalizes it, and finally recomputes the second row as the cross product of the third and first rows, normalizing it. This ensures that the resulting matrix has orthonormal rows. The
	   function returns a reference to the modified matrix.

		\return A reference to the matrix after orthonormalization
	*/
	idMat3&		   OrthoNormalizeSelf();

	//! Returns the transpose of this matrix.
	idMat3		   Transpose() const;

	/*!
		\brief Transposes the matrix in place and returns a reference to itself.
		\return A reference to the transposed matrix
	*/
	idMat3&		   TransposeSelf();

	/*!
		\brief Returns the inverse of the 3x3 matrix.

		This function computes the inverse of the current matrix in-place and returns the result. It uses a helper method InverseSelf() to perform the inversion and verifies that the operation was
	   successful. The function assumes the matrix is not singular, as indicated by the verification of the InverseSelf() result.

		\return A new idMat3 object containing the inverse of the original matrix.
		\throws This function may throw an assertion error if the matrix is singular and the InverseSelf() operation fails.
	*/
	idMat3		   Inverse() const;

	/*!
		\brief Computes the inverse of the 3x3 matrix in-place and returns true if successful, false if the matrix is singular.

		This function calculates the inverse of the 3x3 matrix stored in the object and updates the matrix elements in-place. The function uses cofactor expansion to compute the inverse and checks for
	   singularity by evaluating the determinant. If the absolute value of the determinant is less than MATRIX_INVERSE_EPSILON, the matrix is considered singular and the function returns false.
	   Otherwise, it computes the inverse using the determinant and updates the matrix with the inverted values.

		\return true if the matrix was successfully inverted, false if the matrix is singular
	*/
	bool		   InverseSelf();

	/*!
		\brief Returns the inverse of this 3x3 matrix using a fast calculation method

		This function computes the inverse of the current 3x3 matrix by first making a copy of the matrix and then calling the fast inverse method on that copy. The function uses a standard approach
	   for inverting 3x3 matrices involving computation of the cofactor matrix and division by the determinant. It modifies the matrix in place and returns true if the inversion was successful, or
	   false if the matrix is singular (determinant is zero or close to zero). The threshold for detecting a singular matrix is defined by MATRIX_INVERSE_EPSILON.

		\return A new idMat3 object containing the inverse of the original matrix
	*/
	idMat3		   InverseFast() const;

	/*!
		\brief Computes the inverse of the 3x3 matrix in place and returns false if the determinant is zero.

		This function calculates the inverse of the current 3x3 matrix by computing the cofactor matrix and dividing by the determinant. It modifies the matrix in place and returns true if the
	   inversion was successful, or false if the matrix is singular (determinant is zero or close to zero). The function uses a standard method for inverting 3x3 matrices involving computation of the
	   adjugate matrix and division by the determinant. The threshold for detecting a singular matrix is defined by MATRIX_INVERSE_EPSILON.

		\return true if the matrix was successfully inverted, false if the matrix is singular (determinant is zero or close to zero)
	*/
	bool		   InverseFastSelf();

	/*!
		\brief Returns the matrix product of the transpose of this matrix and matrix b.

		This function computes the matrix multiplication of the transpose of the current matrix with another matrix b. The result is a new matrix where each element is the dot product of a row from
	   the transposed matrix and a column from matrix b. This operation is commonly used in linear algebra and game development for transformations and inertia calculations.

		\param b The matrix to multiply with the transpose of this matrix
		\return A new idMat3 matrix representing the product of the transpose of this matrix and matrix b
	*/
	idMat3		   TransposeMultiply( const idMat3& b ) const;

	/*!
		\brief Computes the inertia matrix translated by a given amount using mass, center of mass, and translation vectors

		This function calculates the updated inertia matrix after translating a body by a specified amount. It takes into account the mass of the body, its original center of mass, and the translation
	   vector to compute the new inertia tensor. The calculation adjusts the inertia matrix components based on the displacement of the center of mass and returns the sum of the original inertia
	   matrix and the computed translation contribution

		\param mass The mass of the body being translated
		\param centerOfMass The original center of mass of the body before translation
		\param translation The translation vector by which the body is moved
		\return The updated inertia matrix after applying the translation
	*/
	idMat3		   InertiaTranslate( const float mass, const idVec3& centerOfMass, const idVec3& translation ) const;

	/*!
		\brief Updates the inertia matrix by translating it according to the given mass, center of mass, and translation vectors

		This function modifies the current inertia matrix in place by applying a translation transformation. It calculates a new inertia matrix based on the mass, original center of mass, and
	   translation vector, then adds this to the current matrix. The function is typically used when the center of mass of a rigid body changes due to translation, and the inertia tensor needs to be
	   updated accordingly. The calculation follows the parallel axis theorem to adjust the inertia tensor for the new position of the center of mass.

		\param mass The mass of the object being translated
		\param centerOfMass The original center of mass of the object before translation
		\param translation The translation vector applied to the object
		\return A reference to the updated inertia matrix
	*/
	idMat3&		   InertiaTranslateSelf( const float mass, const idVec3& centerOfMass, const idVec3& translation );

	/*!
		\brief Returns the inertia tensor rotated by the provided rotation matrix.

		This function applies a rotation to the inertia tensor using the standard rigid body dynamics formula. The rotation is applied by transforming the tensor with the provided rotation matrix,
	   where the inertia tensor is rotated as: R^T * I * R, with R being the rotation matrix and I being the current inertia tensor. The rotation matrix is assumed to be stored in column-major order
	   as per the engine's conventions.

		\param rotation The rotation matrix to apply to the inertia tensor
		\return The rotated inertia tensor as a new idMat3 object
	*/
	idMat3		   InertiaRotate( const idMat3& rotation ) const;

	/*!
		\brief Rotates the inertia matrix by the provided rotation matrix and returns a reference to itself.

		This function performs an inplace rotation of the inertia matrix using the provided rotation matrix. The operation follows the standard inertia matrix transformation formula where the current
	   matrix is multiplied by the transpose of the rotation matrix on the left and the rotation matrix itself on the right. The matrix is stored in column-major order as per the convention used in
	   the codebase.

		\param rotation The rotation matrix to be applied for transforming the inertia matrix
		\return A reference to the modified inertia matrix after the rotation has been applied
	*/
	idMat3&		   InertiaRotateSelf( const idMat3& rotation );

	//! Returns the dimension of the 3x3 matrix, which is always 9.
	int			   GetDimension() const;

	//! Converts a 3x3 matrix to Euler angles representation
	idAngles	   ToAngles() const;

	//! Converts a 3x3 rotation matrix to a quaternion representation.
	idQuat		   ToQuat() const;

	//! Converts this matrix to a canonical quaternion.
	idCQuat		   ToCQuat() const;

	//! Converts a 3x3 rotation matrix to an idRotation object.
	idRotation	   ToRotation() const;

	//! Converts a 3x3 matrix to a 4x4 matrix by transposing and adding homogeneous coordinates.
	idMat4		   ToMat4() const;

	//! Converts a 3x3 matrix to an angular velocity vector.
	idVec3		   ToAngularVelocity() const;

	//! Returns a pointer to the internal float representation of the matrix
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the float representation of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision.
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

ID_INLINE idVec3 operator*( const idVec3& vec, const idMat3& mat )
{
	return mat * vec;
}

ID_INLINE idMat3 operator*( const float a, const idMat3& mat )
{
	return mat * a;
}

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

/*!
	\brief Computes the matrix product of the transpose of one 3x3 matrix and another 3x3 matrix

	This function performs matrix multiplication where the first matrix is transposed before multiplication. It takes the transpose of the 'transpose' parameter and multiplies it by the 'b' parameter,
   storing the result in the 'dst' parameter. The operation is equivalent to dst = transpose^T * b, where transpose^T is the transpose of the transpose matrix

	\param transpose The first 3x3 matrix to be transposed before multiplication
	\param b The second 3x3 matrix to be multiplied with the transpose of the first matrix
	\param dst The destination 3x3 matrix to store the result of the multiplication
*/
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

//! Returns the skew-symmetric matrix of the given 3D vector.
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
	\brief A 4x4 matrix class used for 3D transformations and graphics operations in the engine.

	The idMat4 class represents a 4x4 transformation matrix commonly used in computer graphics for 3D operations such as rotations, translations, and scaling. It supports construction from various
   sources including identity, vectors, rotation matrices, and raw data arrays. The class provides comprehensive matrix arithmetic operations including multiplication, addition, and subtraction, as
   well as utility functions for common matrix operations like transpose, determinant calculation, and inversion. The matrix is stored internally in row-major order but can be constructed from
   column-major component parameters. It includes specialized methods for extracting transformation components such as translation vectors and converting between 4x4 and 3x3 matrix representations.
   The class is designed to support both mathematical operations and graphics transformations within the engine's rendering and physics systems.

*/
class idMat4
{
public:
	/*!
		\brief Constructs an identity matrix.

		This constructor initializes a 4x4 matrix as an identity matrix, where all diagonal elements are 1.0 and all off-diagonal elements are 0.0. The matrix is commonly used as a starting point for
	   transformations and operations that require an identity state.

	*/
	idMat4();

	/*!
		\brief Constructs a 4x4 matrix from four vector rows

		Initializes a 4x4 matrix using the provided four vector rows. The vectors are assigned to the matrix rows in the order x, y, z, and w.

		\param x First vector row of the matrix
		\param y Second vector row of the matrix
		\param z Third vector row of the matrix
		\param w Fourth vector row of the matrix
	*/
	explicit idMat4( const idVec4& x, const idVec4& y, const idVec4& z, const idVec4& w );

	/*!
		\brief Initializes a 4x4 matrix with the specified column-major components.

		This constructor initializes a 4x4 matrix using the provided column-major component values. The matrix is stored in row-major order internally, but the constructor parameters are specified in
	   column-major format. Each parameter represents a component of the matrix in the order xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww, where each pair of indices represents the
	   row and column of the matrix element. This constructor is typically used to construct transformation matrices from individual components.

		\param xx The element at row 0, column 0 of the matrix.
		\param xy The element at row 1, column 0 of the matrix.
		\param xz The element at row 2, column 0 of the matrix.
		\param xw The element at row 3, column 0 of the matrix.
		\param yx The element at row 0, column 1 of the matrix.
		\param yy The element at row 1, column 1 of the matrix.
		\param yz The element at row 2, column 1 of the matrix.
		\param yw The element at row 3, column 1 of the matrix.
		\param zx The element at row 0, column 2 of the matrix.
		\param zy The element at row 1, column 2 of the matrix.
		\param zz The element at row 2, column 2 of the matrix.
		\param zw The element at row 3, column 2 of the matrix.
		\param wx The element at row 0, column 3 of the matrix.
		\param wy The element at row 1, column 3 of the matrix.
		\param wz The element at row 2, column 3 of the matrix.
		\param ww The element at row 3, column 3 of the matrix.
	*/
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

	/*!
		\brief Constructs a 4x4 transformation matrix from a 3x3 rotation matrix and a 3D translation vector.

		This constructor initializes a 4x4 matrix using a 3x3 rotation matrix and a 3D translation vector. The rotation matrix is transposed to account for column-major storage, and the translation
	   vector is placed in the fourth column of the matrix. The last row of the matrix is set to [0, 0, 0, 1] to maintain the homogeneous transformation properties.

		\param rotation The 3x3 rotation matrix to be used in the transformation
		\param translation The 3D translation vector to be applied in the transformation
	*/
	explicit idMat4( const idMat3& rotation, const idVec3& translation );

	/*!
		\brief Constructs a new idMat4 matrix from a 4x4 float array

		This constructor initializes a new idMat4 matrix by copying data from a 4x4 float array. The matrix is constructed by directly copying the contents of the source array into the internal matrix
	   representation. The source array is expected to contain 16 float values organized in row-major order to form a valid 4x4 transformation matrix.

		\param src A pointer to a 4x4 float array containing the matrix data
	*/
	explicit idMat4( const float src[4][4] );

	const idVec4&  operator[]( int index ) const;
	idVec4&		   operator[]( int index );
	idMat4		   operator*( const float a ) const;
	idVec4		   operator*( const idVec4& vec ) const;
	idVec3		   operator*( const idVec3& vec ) const;
	idMat4		   operator*( const idMat4& a ) const;
	idMat4		   operator+( const idMat4& a ) const;
	idMat4		   operator-( const idMat4& a ) const;
	idMat4&		   operator*=( const float a );
	idMat4&		   operator*=( const idMat4& a );
	idMat4&		   operator+=( const idMat4& a );
	idMat4&		   operator-=( const idMat4& a );

	friend idMat4  operator*( const float a, const idMat4& mat );
	friend idVec4  operator*( const idVec4& vec, const idMat4& mat );
	friend idVec3  operator*( const idVec3& vec, const idMat4& mat );
	friend idVec4& operator*=( idVec4& vec, const idMat4& mat );
	friend idVec3& operator*=( idVec3& vec, const idMat4& mat );

	/*!
		\brief Compares this matrix with another matrix for equality.

		This function performs an element-by-element comparison of the current matrix with the provided matrix. It iterates through all elements of the matrices and returns false if any pair of
	   elements differs, otherwise it returns true.

		\param a The matrix to compare with this matrix
		\return true if all elements of the matrices are equal, false otherwise
	*/
	bool		   Compare( const idMat4& a ) const;

	/*!
		\brief Compares this matrix with another matrix using the specified epsilon tolerance

		This function performs an element-wise comparison between the current matrix and another matrix, using the provided epsilon value to determine if the matrices are considered equal. It iterates
	   through all elements of the matrices and checks if the absolute difference between corresponding elements is less than or equal to the epsilon value. If any pair of elements differs by more
	   than epsilon, the function returns false, indicating the matrices are not equal. Otherwise, it returns true, indicating the matrices are equal within the specified tolerance.

		\param a The matrix to compare with this matrix
		\param epsilon The tolerance value used for comparison
		\return True if all corresponding elements of the matrices are within the specified epsilon tolerance, false otherwise
	*/
	bool		   Compare( const idMat4& a, const float epsilon ) const;
	bool		   operator==( const idMat4& a ) const; // exact compare, no epsilon
	bool		   operator!=( const idMat4& a ) const; // exact compare, no epsilon

	//! Sets all elements of the matrix to zero.
	void		   Zero();

	/*!
		\brief Sets the matrix to the 4x4 identity matrix.

		This function initializes the current matrix object to the standard 4x4 identity matrix. The identity matrix has ones on the main diagonal and zeros elsewhere. This operation is typically used
	   to reset a matrix to its default state or to prepare it for transformations.

	*/
	void		   Identity();

	//! Checks if the matrix is identical to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric within a given epsilon tolerance.

		This function determines whether the matrix is symmetric by comparing each element above the diagonal with the corresponding element below the diagonal. The comparison uses the provided
	   epsilon value to account for floating-point precision errors. If any pair of elements differs by more than the epsilon threshold, the function returns false, indicating the matrix is not
	   symmetric.

		\param epsilon The tolerance threshold for floating-point comparisons
		\return true if the matrix is symmetric within the given epsilon tolerance, false otherwise
	*/
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is diagonal within the given epsilon threshold

		This function determines whether a 4x4 matrix is diagonal by verifying that all off-diagonal elements are smaller than the specified epsilon threshold. A matrix is considered diagonal if all
	   elements at positions (i,j) where i does not equal j are close to zero within the tolerance defined by epsilon. The function iterates through all elements of the matrix and returns false
	   immediately when any non-diagonal element exceeds the epsilon threshold, otherwise it returns true.

		\param epsilon tolerance threshold for considering an element as zero
		\return true if all off-diagonal elements are smaller than epsilon, false otherwise
	*/
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix has a non-zero rotation component.
	bool		   IsRotated() const;

	/*!
		\brief Projects a vector using the matrix by transforming it and storing the result in dst.

		This function applies a 4x4 matrix transformation to a source vector and stores the resulting vector in the destination parameter. Each component of the output vector is computed as the dot
	   product of the source vector with the corresponding row of the matrix.

		\param src The input vector to be transformed by the matrix
		\param dst The output vector that will contain the transformed result
	*/
	void		   ProjectVector( const idVec4& src, idVec4& dst ) const;

	/*!
		\brief Transforms a vector using the matrix and stores the result in dst.

		This function performs a vector transformation by multiplying the matrix with the input vector. The transformation is applied to the vector components x, y, z, and w, which are used as
	   multipliers for the corresponding matrix rows. The result is stored in the destination vector.

		\param src The input vector to be transformed
		\param dst The destination vector where the result of the transformation will be stored
	*/
	void		   UnprojectVector( const idVec4& src, idVec4& dst ) const;

	/*!
		\brief Returns the trace of the 4x4 matrix, which is the sum of its diagonal elements.

		This function computes the mathematical trace of a 4x4 matrix by summing the elements on the main diagonal. The trace is a fundamental property of square matrices and is often used in linear
	   algebra and graphics computations.

		\return The sum of the diagonal elements of the matrix
	*/
	float		   Trace() const;

	/*!
		\brief Computes and returns the determinant of the 4x4 matrix.

		This function calculates the determinant of a 4x4 matrix using cofactor expansion. It first computes various 2x2 sub-determinants from the first two rows, then uses these to calculate 3x3
	   sub-determinants from the first three rows. Finally, it combines these results with the elements from the fourth row to compute the full determinant. The implementation follows a standard
	   mathematical approach for calculating the determinant of a 4x4 matrix.

		\return The determinant value of the 4x4 matrix as a floating-point number
	*/
	float		   Determinant() const;

	/*!
		\brief Returns the transpose of the matrix.

		This function computes the transpose of the calling matrix by swapping rows with columns. It creates a new matrix where the element at position [i][j] in the original matrix becomes the
	   element at position [j][i] in the returned matrix. The operation is performed on a 4x4 matrix as the class represents a 4x4 transformation matrix.

		\return A new idMat4 object containing the transposed matrix values
	*/
	idMat4		   Transpose() const;

	//! Transposes the matrix in place and returns a reference to itself.
	idMat4&		   TransposeSelf();

	/*!
		\brief Returns the inverse of the matrix.

		Computes the inverse of the 4x4 matrix and returns true if successful, false if the matrix is singular. The function creates a copy of the current matrix, inverts it in place, and returns the
	   inverted matrix. This operation modifies the matrix in place and may fail if the matrix is singular.

		\return The inverse of the matrix
	*/
	idMat4		   Inverse() const;

	/*!
		\brief Computes the inverse of the 4x4 matrix in place and returns true if successful, false if the matrix is singular.

		This function calculates the inverse of a 4x4 matrix using cofactor expansion and determinant calculation. It first computes the determinant of the matrix to check if it is singular
	   (nonsingular matrices have a non-zero determinant). If the determinant is below the threshold MATRIX_INVERSE_EPSILON, the function returns false indicating that the matrix cannot be inverted.
	   Otherwise, it proceeds to compute the cofactors and adjugate matrix to form the inverse. The matrix is modified in place to store the inverse result.

		\return true if the matrix was successfully inverted, false if the matrix is singular and cannot be inverted
	*/
	bool		   InverseSelf();

	/*!
		\brief Returns the inverse of the matrix by computing the inverse in-place and ensuring the operation was successful.

		This function computes the inverse of the 4x4 matrix stored in this instance. It creates a copy of the matrix, performs an in-place inversion operation on the copy, and verifies that the
	   operation was successful. The function returns the inverted matrix.

		\return The inverted 4x4 matrix.
	*/
	idMat4		   InverseFast() const;

	/*!
		\brief Computes the inverse of this 4x4 matrix in-place and returns whether the operation was successful.

		This function calculates the inverse of the 4x4 matrix stored in this object. It uses a fast algorithm based on block matrix inversion. The function modifies the matrix in-place and returns
	   true if the inversion was successful, or false if the matrix is singular (not invertible). The algorithm checks for a determinant that is too close to zero using a predefined epsilon value.

		\return true if the matrix inverse was computed successfully, false if the matrix is singular and cannot be inverted
	*/
	bool		   InverseFastSelf();
	idMat4		   TransposeMultiply( const idMat4& b ) const;

	//! Returns the dimension of the matrix.
	int			   GetDimension() const;

	//! Returns a pointer to the float representation of the matrix data.
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the first element of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified decimal precision
	const char*	   ToString( int precision = 2 ) const;

	//! Converts a 4x4 matrix to a 3x3 matrix by extracting the upper-left 3x3 block.
	idMat3		   ToMat3() const;

	//! Returns the translation component of this 4x4 matrix as a 3D vector.
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

ID_INLINE idMat4 operator*( const float a, const idMat4& mat )
{
	return mat * a;
}

ID_INLINE idVec4 operator*( const idVec4& vec, const idMat4& mat )
{
	return mat * vec;
}

ID_INLINE idVec3 operator*( const idVec3& vec, const idMat4& mat )
{
	return mat * vec;
}

ID_INLINE idVec4& operator*=( idVec4& vec, const idMat4& mat )
{
	vec = mat * vec;
	return vec;
}

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
	\brief A 5x5 matrix class for linear algebra operations in the Roguestrad engine.

	The idMat5 class provides a comprehensive implementation of 5x5 matrix operations, including construction from various data sources, arithmetic operations, and mathematical functions. It supports
   standard matrix operations such as multiplication, addition, subtraction, transposition, and inversion, making it suitable for use in 3D graphics, physics simulations, and other engine components
   requiring matrix mathematics. The class is designed to integrate seamlessly with the engine's existing math library, providing both inline optimized operations and full-featured methods for complex
   matrix computations. The implementation includes methods for checking matrix properties such as identity, symmetry, and diagonality, as well as functions for computing determinants and traces.
   Memory management is handled through the standard C++ object model with no explicit allocation or deallocation required, as the class manages its internal 25-element float array automatically. The
   class is intended for use in performance-critical engine code where both speed and mathematical correctness are required.

*/
class idMat5
{
public:
	/*!
		\brief Constructs an uninitialized 5x5 matrix.

		This constructor creates an idMat5 object without initializing its contents. The resulting matrix will contain arbitrary values until explicitly set. It is typically used when the matrix will
	   be immediately assigned or populated with specific values.

	*/
	idMat5();

	/*!
		\brief Constructs a 5x5 matrix from five 5-dimensional vectors

		This constructor initializes a 5x5 matrix by setting each row of the matrix to the corresponding input vector. Each input vector represents a row in the resulting matrix, with the first vector
	   becoming the first row, the second vector the second row, and so on up to the fifth vector which becomes the fifth row of the matrix

		\param v0 First row of the matrix as a 5-dimensional vector
		\param v1 Second row of the matrix as a 5-dimensional vector
		\param v2 Third row of the matrix as a 5-dimensional vector
		\param v3 Fourth row of the matrix as a 5-dimensional vector
		\param v4 Fifth row of the matrix as a 5-dimensional vector
	*/
	explicit idMat5( const idVec5& v0, const idVec5& v1, const idVec5& v2, const idVec5& v3, const idVec5& v4 );

	/*!
		\brief Constructs an idMat5 object by copying data from a 5x5 float array.

		This constructor initializes a 5x5 matrix by copying the elements from a source 5x5 float array. The matrix is stored in row-major order, and all 25 elements are copied from the source array
	   to the internal matrix storage.

		\param src A pointer to a 5x5 float array containing the matrix data to be copied
	*/
	explicit idMat5( const float src[5][5] );

	const idVec5&  operator[]( int index ) const;
	idVec5&		   operator[]( int index );
	idMat5		   operator*( const float a ) const;
	idVec5		   operator*( const idVec5& vec ) const;
	idMat5		   operator*( const idMat5& a ) const;
	idMat5		   operator+( const idMat5& a ) const;
	idMat5		   operator-( const idMat5& a ) const;
	idMat5&		   operator*=( const float a );
	idMat5&		   operator*=( const idMat5& a );
	idMat5&		   operator+=( const idMat5& a );
	idMat5&		   operator-=( const idMat5& a );

	friend idMat5  operator*( const float a, const idMat5& mat );
	friend idVec5  operator*( const idVec5& vec, const idMat5& mat );
	friend idVec5& operator*=( idVec5& vec, const idMat5& mat );

	/*!
		\brief Compares this matrix with another matrix for equality.

		This function performs an element-wise comparison between the current matrix and the provided matrix 'a'. It returns true if all elements are equal, and false if any element differs. The
	   comparison is done by iterating through all 25 elements of the 5x5 matrices.

		\param a The matrix to compare with this matrix
		\return True if all elements of the matrices are equal, false otherwise
	*/
	bool		   Compare( const idMat5& a ) const;

	/*!
		\brief Compares this matrix with another matrix using the specified epsilon threshold for floating-point equality.

		This function performs an element-wise comparison between the current matrix and the provided matrix 'a' using the given epsilon value to determine if the matrices are equal within a specified
	   tolerance. It iterates through all elements of the 5x5 matrices and checks if the absolute difference between corresponding elements is less than or equal to epsilon. If any pair of elements
	   differs by more than epsilon, the function returns false. Otherwise, it returns true, indicating that the matrices are considered equal.

		\param a The matrix to compare against this matrix
		\param epsilon The epsilon threshold for floating-point comparison
		\return true if all corresponding elements of the two matrices are within the epsilon tolerance, false otherwise
	*/
	bool		   Compare( const idMat5& a, const float epsilon ) const;
	bool		   operator==( const idMat5& a ) const; // exact compare, no epsilon
	bool		   operator!=( const idMat5& a ) const; // exact compare, no epsilon

	//! Sets all elements of the matrix to zero.
	void		   Zero();

	/*!
		\brief Sets the matrix to the identity matrix.

		This function assigns the pre-defined identity matrix to the current matrix instance, effectively resetting it to its identity state. The identity matrix has ones on its main diagonal and
	   zeros elsewhere, which is commonly used as a starting point for matrix operations.

	*/
	void		   Identity();

	//! Checks if the matrix is equal to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric within a given epsilon tolerance

		The function determines if the matrix is symmetric by comparing each element mat[i][j] with its corresponding element mat[j][i]. If the absolute difference between these elements exceeds the
	   provided epsilon value, the function returns false. Otherwise, it returns true, indicating the matrix is symmetric within the specified tolerance.

		\param epsilon tolerance value for comparing matrix elements
		\return true if the matrix is symmetric within the specified epsilon tolerance, false otherwise
	*/
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is diagonal using the specified epsilon for comparison

		Returns true if all off-diagonal elements of the matrix are smaller than the given epsilon, indicating that the matrix is diagonal. The function iterates through all elements of the 5x5 matrix
	   and compares each off-diagonal element against the epsilon threshold.

		\param epsilon threshold for comparing off-diagonal elements
		\return true if the matrix is diagonal within the given epsilon, false otherwise
	*/
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Returns the trace of the 5x5 matrix, which is the sum of its diagonal elements.

		This function computes the trace of a 5x5 matrix by summing the elements on its main diagonal. The trace is a fundamental property of square matrices and is often used in linear algebra and
	   physics simulations.

		\return The sum of the diagonal elements of the matrix
	*/
	float		   Trace() const;

	/*!
		\brief Computes and returns the determinant of the 5x5 matrix

		This function calculates the determinant of a 5x5 matrix using cofactor expansion. It first computes various 2x2 sub-determinants, then uses those to calculate 3x3 sub-determinants, followed
	   by 4x4 sub-determinants, and finally combines these to obtain the full 5x5 determinant. The computation follows the standard mathematical approach for calculating determinants of larger
	   matrices by breaking them down into smaller sub-matrices.

		\return The determinant value of the 5x5 matrix as a floating-point number
	*/
	float		   Determinant() const;

	/*!
		\brief Returns the transpose of the 5x5 matrix.

		This function computes the transpose of the current 5x5 matrix by swapping its rows and columns. The transpose operation converts rows of the original matrix into columns of the new matrix,
	   and vice versa. The function creates a new idMat5 object to store the result and returns it without modifying the original matrix.

		\return A new idMat5 object containing the transposed matrix.
	*/
	idMat5		   Transpose() const;

	/*!
		\brief Transposes the matrix in place and returns a reference to itself.

		This function performs an in-place transposition of the 5x5 matrix. It swaps elements across the main diagonal, such that element at position (i,j) moves to position (j,i). The operation
	   modifies the matrix directly and returns a reference to the same matrix object.

		\return Reference to the transposed matrix
	*/
	idMat5&		   TransposeSelf();

	/*!
		\brief Returns the inverse of this 5x5 matrix.

		This function computes the inverse of the 5x5 matrix by first creating a copy of the current matrix and then calling InverseSelf() on the copy. The function returns true if the matrix
	   inversion is successful, and false if the matrix is singular and cannot be inverted. The original matrix remains unchanged.

		\return A new idMat5 object that represents the inverse of the current matrix.
	*/
	idMat5		   Inverse() const;

	/*!
		\brief Computes the inverse of the 5x5 matrix in place and returns true if successful, false if the matrix is singular.

		This function calculates the inverse of a 5x5 matrix stored in the object. It uses a cofactor expansion method to compute the determinant and adjugate matrix. The function first computes
	   various sub-determinants of the matrix to calculate the full 5x5 determinant. If the determinant is close to zero (within MATRIX_INVERSE_EPSILON), the matrix is considered singular and the
	   function returns false. Otherwise, it proceeds to compute the inverse matrix elements using the cofactors and the determinant. The computation involves multiple steps of sub-determinant
	   calculations for 2x2, 3x3, and 4x4 matrices, followed by the final inverse calculation. The original matrix is modified in place to contain the inverse matrix.

		\return true if the matrix was invertible and the inverse was computed successfully, false if the matrix is singular (determinant is zero or near-zero)
	*/
	bool		   InverseSelf();

	/*!
		\brief Returns the inverse of this matrix by computing the inverse in-place

		This method computes the inverse of the matrix without modifying the original matrix. It creates a copy of the current matrix, then calls the in-place inverse method to compute the inverse,
	   and finally returns the inverted matrix. The method returns false if the matrix is singular and cannot be inverted

		\return The inverse of this matrix
		\throws false if the matrix is singular and cannot be inverted
	*/
	idMat5		   InverseFast() const;

	/*!
		\brief Computes the inverse of the 5x5 matrix in-place and returns false if the matrix is singular.

		This function calculates the inverse of the matrix stored in the idMat5 object. It performs a direct computation of the inverse using cofactor expansion and determinant calculation. The
	   function modifies the matrix in-place and returns true if the inverse was successfully computed, or false if the matrix is singular (i.e., its determinant is zero or close to zero, within the
	   defined epsilon threshold). The matrix is assumed to be a 5x5 matrix and the function uses a numerically stable approach for inversion.

		\return true if the inverse was successfully computed and the matrix is non-singular, false otherwise
	*/
	bool		   InverseFastSelf();

	//! Returns the dimension of the idMat5 matrix which is 25.
	int			   GetDimension() const;

	//! Returns a pointer to the first element of the matrix data
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the first element of the matrix data.
	float*		   ToFloatPtr();

	//! Converts the matrix to a string representation with the specified decimal precision.
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

ID_INLINE idVec5 operator*( const idVec5& vec, const idMat5& mat )
{
	return mat * vec;
}

ID_INLINE idMat5 operator*( const float a, idMat5 const& mat )
{
	return mat * a;
}

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
	\brief A 6x6 matrix class designed for efficient linear algebra operations in 3D graphics and physics simulations.

	The idMat6 class provides a comprehensive implementation of 6x6 matrix operations commonly used in computer graphics and physics engines. It supports standard matrix constructors, arithmetic
   operations, and advanced functionalities such as determinant calculation, transposition, and inversion. The class is optimized for performance through inline implementations and provides both
   in-place and temporary operation variants. The matrix is primarily used for transformations and linear algebra computations in the engine's 3D rendering and physics systems, with support for
   various data initialization methods and comparison utilities. The class maintains a fixed 6x6 structure to accommodate specialized operations required by the engine's graphics pipeline,
   particularly for handling complex transformations and matrix-based calculations in 3D space.

*/
class idMat6
{
public:
	/*!
		\brief Default constructor for idMat6 that initializes the matrix to identity.

		Constructs a 6x6 matrix and initializes it to the identity matrix. This constructor does not take any parameters and sets all diagonal elements to 1.0 while all off-diagonal elements are set
	   to 0.0.

	*/
	idMat6();

	/*!
		\brief Constructs a 6x6 matrix from six 6D vectors representing the rows of the matrix.

		This constructor initializes a 6x6 matrix by taking six idVec6 vectors as input parameters. Each vector becomes a row of the resulting matrix. The matrix is constructed inline, meaning it is
	   implemented directly in the header file for performance reasons.

		\param v0 First row of the 6x6 matrix
		\param v1 Second row of the 6x6 matrix
		\param v2 Third row of the 6x6 matrix
		\param v3 Fourth row of the 6x6 matrix
		\param v4 Fifth row of the 6x6 matrix
		\param v5 Sixth row of the 6x6 matrix
	*/
	explicit idMat6( const idVec6& v0, const idVec6& v1, const idVec6& v2, const idVec6& v3, const idVec6& v4, const idVec6& v5 );

	/*!
		\brief Constructs a 6x6 matrix by concatenating rows from four 3x3 matrices

		This constructor initializes a 6x6 matrix by taking four 3x3 matrices as input and concatenating their rows. The first three rows of the resulting 6x6 matrix are formed by concatenating the
	   rows of the first 3x3 matrix (m0) with the corresponding rows of the second 3x3 matrix (m1). Similarly, the last three rows are formed by concatenating the rows of the third 3x3 matrix (m2)
	   with the corresponding rows of the fourth 3x3 matrix (m3). Each row of the 6x6 matrix contains six elements: the first three from the corresponding row of the first matrix and the last three
	   from the corresponding row of the second matrix

		\param m0 First 3x3 matrix whose rows will form the first three rows of the result
		\param m1 Second 3x3 matrix whose rows will form the first three rows of the result
		\param m2 Third 3x3 matrix whose rows will form the last three rows of the result
		\param m3 Fourth 3x3 matrix whose rows will form the last three rows of the result
	*/
	explicit idMat6( const idMat3& m0, const idMat3& m1, const idMat3& m2, const idMat3& m3 );

	/*!
		\brief Initializes a 6x6 matrix by copying values from a 2D float array.

		This constructor initializes a 6x6 matrix by copying the values from a 2D float array of size 6x6. The copy is performed using memcpy for efficiency.

		\param src A 2D float array of size 6x6 used to initialize the matrix
	*/
	explicit idMat6( const float src[6][6] );

	const idVec6&  operator[]( int index ) const;
	idVec6&		   operator[]( int index );
	idMat6		   operator*( const float a ) const;
	idVec6		   operator*( const idVec6& vec ) const;
	idMat6		   operator*( const idMat6& a ) const;
	idMat6		   operator+( const idMat6& a ) const;
	idMat6		   operator-( const idMat6& a ) const;
	idMat6&		   operator*=( const float a );
	idMat6&		   operator*=( const idMat6& a );
	idMat6&		   operator+=( const idMat6& a );
	idMat6&		   operator-=( const idMat6& a );

	friend idMat6  operator*( const float a, const idMat6& mat );
	friend idVec6  operator*( const idVec6& vec, const idMat6& mat );
	friend idVec6& operator*=( idVec6& vec, const idMat6& mat );

	/*!
		\brief Compares this matrix with another matrix for equality.

		This function performs an element-by-element comparison of the current matrix with the provided matrix 'a'. It returns true if all elements are equal, and false otherwise. The comparison is
	   done by iterating through all 36 elements (6x6) of the matrices and checking for equality.

		\param a The matrix to compare with this matrix
		\return true if all elements of the matrices are equal, false otherwise
	*/
	bool		   Compare( const idMat6& a ) const;

	/*!
		\brief Compares this matrix with another matrix using the specified epsilon tolerance.

		This function performs an element-wise comparison between the current matrix and the provided matrix 'a'. It checks if the absolute difference between corresponding elements is less than or
	   equal to the given epsilon value. If any pair of elements differs by more than epsilon, the function returns false. Otherwise, it returns true, indicating that the matrices are equal within the
	   specified tolerance.

		\param a The matrix to compare against
		\param epsilon The tolerance value for the comparison
		\return true if all corresponding elements of the matrices differ by no more than epsilon, false otherwise
	*/
	bool		   Compare( const idMat6& a, const float epsilon ) const;
	bool		   operator==( const idMat6& a ) const; // exact compare, no epsilon
	bool		   operator!=( const idMat6& a ) const; // exact compare, no epsilon

	//! Sets all elements of the matrix to zero.
	void		   Zero();

	/*!
		\brief Sets the matrix to the 6x6 identity matrix.

		This function initializes the current matrix object to the 6x6 identity matrix. It directly assigns the pre-defined identity matrix constant mat6_identity to the current object, effectively
	   resetting the matrix to its identity state where all diagonal elements are 1 and all off-diagonal elements are 0.

	*/
	void		   Identity();

	//! Checks if the matrix is identical to the identity matrix within a given epsilon tolerance.
	bool		   IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the 6x6 matrix is symmetric within a given epsilon tolerance

		The function determines if a 6x6 matrix is symmetric by comparing each element above the diagonal with its corresponding element below the diagonal. It uses the provided epsilon value to
	   account for floating-point precision errors. If any pair of elements differs by more than the epsilon tolerance, the function returns false. Otherwise, it returns true, indicating the matrix is
	   symmetric

		\param epsilon tolerance value for floating-point comparison
		\return true if the matrix is symmetric within the specified epsilon tolerance, false otherwise
	*/
	bool		   IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the 6x6 matrix is diagonal within the specified epsilon tolerance.

		This function determines whether the matrix is diagonal by verifying that all off-diagonal elements are smaller than the provided epsilon threshold. It iterates through all elements of the 6x6
	   matrix and returns false if any off-diagonal element exceeds the tolerance. If all off-diagonal elements are within the tolerance, it returns true.

		\param epsilon tolerance value for considering an element as zero
		\return true if the matrix is diagonal within the given epsilon tolerance, false otherwise
	*/
	bool		   IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Returns a 3x3 sub-matrix from this 6x6 matrix at the specified index.

		This function extracts a 3x3 sub-matrix from a 6x6 matrix based on the provided index. The index determines which 3x3 block to extract from the 6x6 matrix. The function uses bitwise operations
	   to calculate the starting indices for the rows and columns of the sub-matrix.

		\param n The index of the 3x3 sub-matrix to extract, must be between 0 and 3 inclusive
		\return A 3x3 matrix extracted from the 6x6 matrix at the specified index
		\throws assertion error if the index n is out of bounds (less than 0 or greater than or equal to 4)
	*/
	idMat3		   SubMat3( int n ) const;

	/*!
		\brief Returns the trace of the 6x6 matrix, which is the sum of the diagonal elements.

		This function computes the mathematical trace of a 6x6 matrix, which is defined as the sum of the elements on the main diagonal (from top-left to bottom-right). The trace is a scalar value
	   that provides important information about the matrix, such as its eigenvalues and determinant in certain contexts. This implementation directly accesses the diagonal elements of the matrix and
	   returns their sum.

		\return The scalar sum of all diagonal elements in the 6x6 matrix
	*/
	float		   Trace() const;

	/*!
		\brief Computes and returns the determinant of the 6x6 matrix.

		This function calculates the determinant of a 6x6 matrix by computing the determinants of various sub-matrices and combining them using cofactor expansion. The calculation involves multiple
	   steps of computing 2x2, 3x3, 4x4, and 5x5 determinants, which are then used in the final computation of the 6x6 determinant. This method is a direct implementation of the cofactor expansion
	   approach to determinant calculation.

		\return The determinant value of the 6x6 matrix as a floating-point number.
	*/
	float		   Determinant() const;

	/*!
		\brief Returns the transpose of this 6x6 matrix.

		This function computes the transpose of the current 6x6 matrix by swapping its rows and columns. The transpose operation converts rows into columns and columns into rows, creating a new matrix
	   that represents the transposed version of the original.

		\return A new idMat6 object containing the transposed matrix values.
	*/
	idMat6		   Transpose() const;

	/*!
		\brief Transposes the matrix in place and returns a reference to itself.

		This function performs an in-place transpose operation on the 6x6 matrix. It swaps elements across the diagonal, such that element at position (i,j) moves to position (j,i). The operation
	   modifies the matrix directly and returns a reference to the same matrix instance.

		\return Reference to the transposed matrix
	*/
	idMat6&		   TransposeSelf();

	/*!
		\brief Returns the inverse of the 6x6 matrix

		Computes the inverse of the 6x6 matrix and returns true if successful, false if the matrix is singular

		\return The inverse of the matrix
	*/
	idMat6		   Inverse() const;

	/*!
		\brief Computes the inverse of the 6x6 matrix in place and returns true if successful, false if the matrix is singular.

		This function calculates the inverse of the 6x6 matrix stored in the object. It uses a cofactor expansion method to compute the determinant and then calculates the adjugate matrix to form the
	   inverse. The computation involves multiple levels of sub-determinants from 2x2 up to 5x5 matrices. The function modifies the matrix in place and returns a boolean indicating whether the
	   inversion was successful. If the matrix is singular (determinant is zero or very close to zero), the function returns false and the matrix remains unchanged.

		\return true if the matrix was successfully inverted, false if the matrix is singular and cannot be inverted
	*/
	bool		   InverseSelf();

	/*!
		\brief Returns the inverse of this 6x6 matrix by computing it in place

		This function computes the inverse of the 6x6 matrix stored in this instance. It creates a copy of the matrix, then invokes the fast inverse method on the copy to compute the inverse in place.
	   The function returns true if the inversion is successful and false if the matrix is singular, indicating that the inverse cannot be computed

		\return A new idMat6 instance containing the inverse of the original matrix
	*/
	idMat6		   InverseFast() const;

	/*!
		\brief Computes the inverse of the 6x6 matrix in place and returns true if successful, false if the matrix is singular.

		This function attempts to compute the inverse of a 6x6 matrix stored in the object. The inversion is performed in place, meaning the original matrix data will be overwritten with the inverse
	   matrix. The function uses a computationally intensive method that involves calculating numerous sub-determinants to determine the determinant of the full matrix and then computing the adjugate
	   matrix. If the matrix is singular (i.e., its determinant is zero or very close to zero), the function returns false, indicating that the inversion could not be performed.

		\return True if the matrix was successfully inverted, false if the matrix is singular and cannot be inverted.
	*/
	bool		   InverseFastSelf();

	//! Returns the dimension of the 6x6 matrix, which is always 36.
	int			   GetDimension() const;

	//! Returns a pointer to the internal float array representation of the matrix.
	const float*   ToFloatPtr() const;

	//! Returns a pointer to the first element of the matrix data.
	float*		   ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision.
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

ID_INLINE idVec6 operator*( const idVec6& vec, const idMat6& mat )
{
	return mat * vec;
}

ID_INLINE idMat6 operator*( const float a, idMat6 const& mat )
{
	return mat * a;
}

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
