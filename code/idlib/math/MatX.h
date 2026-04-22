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

#ifndef __MATH_MATX_H__
#define __MATH_MATX_H__

/*
===============================================================================

idMatX - arbitrary sized dense real matrix

The matrix lives on 16 byte aligned and 16 byte padded memory.

NOTE: due to the temporary memory pool idMatX cannot be used by multiple threads.

===============================================================================
*/

#define MATX_MAX_TEMP  1024
#define MATX_QUAD( x ) ( ( ( ( x ) + 3 ) & ~3 ) * sizeof( float ) )
#define MATX_CLEAREND()               \
	int s = numRows * numColumns;     \
	while( s < ( ( s + 3 ) & ~3 ) ) { \
		mat[s++] = 0.0f;              \
	}
#define MATX_ALLOCA( n )			 ( ( float* )_alloca16( MATX_QUAD( n ) ) )
#define MATX_ALLOCA_CACHE_LINES( n ) ( ( float* )_alloca128( ( ( n ) * sizeof( float ) + CACHE_LINE_SIZE - 1 ) & ~( CACHE_LINE_SIZE - 1 ) ) )

#if defined( USE_INTRINSICS_SSE )
	#define MATX_SIMD
#endif

/*!
	\class idMatX
	\brief A dynamic matrix class supporting various mathematical operations and factorizations.

	The idMatX class provides a flexible matrix implementation capable of handling matrices of arbitrary dimensions. It supports standard matrix operations including addition, subtraction,
   multiplication, transposition, and inversion, along with advanced features such as various matrix factorizations including LU, QR, SVD, Cholesky, and LDLT decompositions. The class allows for
   dynamic resizing, initialization from arrays or other matrices, and provides utility functions for creating special matrices like identity, diagonal, and random matrices. Memory management is
   handled internally with methods to set data pointers and control allocation. The implementation includes methods for solving linear systems, computing eigenvalues and eigenvectors, and performing
   rank-one updates to matrices. The class is designed for both general matrix computations and specialized numerical algorithms requiring efficient memory access patterns and mathematical operations.

*/
class idMatX
{
public:
	//! Constructs an empty idMatX object with no rows, columns, or allocated memory.
	ID_INLINE idMatX();

	//! Constructs a new matrix as a copy of the provided matrix.
	ID_INLINE idMatX( const idMatX& other );

	//! Constructs an idMatX matrix with the specified number of rows and columns.
	ID_INLINE explicit idMatX( int rows, int columns );

	//! Constructs an idMatX object with specified rows and columns, initializing it with data from a source float array.
	ID_INLINE explicit idMatX( int rows, int columns, float* src );

	//! Destructor for the idMatX class that frees allocated memory.
	ID_INLINE ~idMatX();

	//! Sets the matrix size and copies data from a source array.
	ID_INLINE void			 Set( int rows, int columns, const float* src );

	//! Sets the current matrix to a 3x6 matrix by placing two 3x3 matrices side by side.
	ID_INLINE void			 Set( const idMat3& m1, const idMat3& m2 );

	/*!
		\brief Sets the current matrix to a 6x6 block matrix constructed from four 3x3 matrices.

		This function initializes a 6x6 matrix by arranging four 3x3 matrices in a block format. The first matrix m1 is placed in the top-left corner, m2 in the top-right, m3 in the bottom-left, and
	   m4 in the bottom-right. Each 3x3 matrix contributes its elements to the corresponding block of the 6x6 matrix.

		\param m1 The first 3x3 matrix to place in the top-left block of the 6x6 matrix
		\param m2 The second 3x3 matrix to place in the top-right block of the 6x6 matrix
		\param m3 The third 3x3 matrix to place in the bottom-left block of the 6x6 matrix
		\param m4 The fourth 3x3 matrix to place in the bottom-right block of the 6x6 matrix
	*/
	ID_INLINE void			 Set( const idMat3& m1, const idMat3& m2, const idMat3& m3, const idMat3& m4 );

	//! Returns a pointer to the specified row of the matrix.
	ID_INLINE const float*	 operator[]( int index ) const;

	//! Returns a pointer to the specified row in the matrix.
	ID_INLINE float*		 operator[]( int index );

	//! Assigns the contents of another matrix to this matrix.
	ID_INLINE idMatX&		 operator=( const idMatX& a );

	//! Returns a new matrix that is the result of multiplying this matrix by a scalar value.
	ID_INLINE idMatX		 operator*( const float a ) const;

	//! Multiplies this matrix by the given vector and returns the resulting vector.
	ID_INLINE idVecX		 operator*( const idVecX& vec ) const;

	//! Returns the matrix product of this matrix and matrix a
	ID_INLINE idMatX		 operator*( const idMatX& a ) const;

	//! Returns the sum of this matrix and another matrix.
	ID_INLINE idMatX		 operator+( const idMatX& a ) const;

	//! Returns a new matrix that is the result of subtracting the given matrix from this matrix.
	ID_INLINE idMatX		 operator-( const idMatX& a ) const;

	//! Multiplies all elements of the matrix by the given scalar value and returns a reference to the modified matrix.
	ID_INLINE idMatX&		 operator*=( const float a );

	//! Multiplies this matrix by another matrix and assigns the result to this matrix
	ID_INLINE idMatX&		 operator*=( const idMatX& a );

	//! Adds the elements of the given matrix to this matrix and returns a reference to this matrix.
	ID_INLINE idMatX&		 operator+=( const idMatX& a );

	//! Subtracts the elements of the given matrix from this matrix in place and returns a reference to this matrix.
	ID_INLINE idMatX&		 operator-=( const idMatX& a );

	friend ID_INLINE idMatX	 operator*( const float a, const idMatX& m );
	friend ID_INLINE idVecX	 operator*( const idVecX& vec, const idMatX& m );
	friend ID_INLINE idVecX& operator*=( idVecX& vec, const idMatX& m );

	//! Compares this matrix with another matrix for equality.
	ID_INLINE bool			 Compare( const idMatX& a ) const;

	//! Compares this matrix with another matrix using the specified epsilon tolerance.
	ID_INLINE bool			 Compare( const idMatX& a, const float epsilon ) const;

	//! Compares two idMatX matrices for exact equality.
	ID_INLINE bool			 operator==( const idMatX& a ) const;

	//! Compares two matrix objects for inequality.
	ID_INLINE bool			 operator!=( const idMatX& a ) const;

	//! Sets the number of rows and columns for the matrix.
	ID_INLINE void			 SetSize( int rows, int columns );

	//! Resizes the matrix to the specified number of rows and columns while preserving existing data
	void					 ChangeSize( int rows, int columns, bool makeZero = false );

	//! Changes the number of rows in the matrix to the specified value.
	ID_INLINE void			 ChangeNumRows( int rows )
	{
		ChangeSize( rows, numColumns ); // set the number of rows/columns
	}

	//! Returns the number of rows in the matrix.
	int GetNumRows() const
	{
		return numRows; // get the number of rows
	}

	//! Returns the number of columns in the matrix.
	int GetNumColumns() const
	{
		return numColumns; // get the number of columns
	}

	//! Sets the data pointer for the matrix and initializes its dimensions.
	ID_INLINE void			SetData( int rows, int columns, float* data );

	/*!
		\brief Sets the data pointer for the matrix with specified dimensions and optional clearing.

		This function initializes the matrix data pointer with the provided data buffer, ensuring the data is 128-byte aligned. It sets the matrix dimensions and optionally clears the memory using
	   cache line alignment. If the matrix already has allocated memory, it is freed before assigning the new data pointer.

		\param rows The number of rows in the matrix
		\param columns The number of columns in the matrix
		\param data Pointer to the data buffer, must be 128-byte aligned
		\param clear If true, clears the matrix data using cache line alignment
		\throws assertion failure if the data pointer is not 128-byte aligned
	*/
	ID_INLINE void			SetDataCacheLines( int rows, int columns, float* data, bool clear );

	//! Clears the matrix by setting all elements to zero.
	ID_INLINE void			Zero();

	//! Sets the matrix size and clears all elements to zero.
	ID_INLINE void			Zero( int rows, int columns );

	//! Sets the matrix to an identity matrix.
	ID_INLINE void			Identity();

	//! Sets the matrix size and initializes it as an identity matrix with the specified number of rows and columns.
	ID_INLINE void			Identity( int rows, int columns );

	//! Creates a diagonal matrix from the given vector.
	ID_INLINE void			Diag( const idVecX& v );

	//! Fills the matrix with random values using the provided seed and range.
	ID_INLINE void			Random( int seed, float l = 0.0f, float u = 1.0f );

	/*!
		\brief Initializes the matrix with random values within a specified range using a given seed.

		The function sets the matrix size to the specified number of rows and columns, then fills each element with a random float value within the range [l, u]. The random number generation is
	   initialized with the provided seed to ensure reproducible results. The lower and upper bounds are inclusive, and the random values are uniformly distributed within this range.

		\param rows Number of rows in the matrix
		\param columns Number of columns in the matrix
		\param seed Seed value for the random number generator
		\param l Lower bound for the random values
		\param u Upper bound for the random values
	*/
	ID_INLINE void			Random( int rows, int columns, int seed, float l = 0.0f, float u = 1.0f );

	//! Negates all elements of the matrix in place.
	ID_INLINE void			Negate();

	//! Clamps all matrix values to the specified range.
	ID_INLINE void			Clamp( float min, float max );

	//! Swaps two rows in the matrix.
	ID_INLINE idMatX&		SwapRows( int r1, int r2 );

	//! Swaps two columns in the matrix.
	ID_INLINE idMatX&		SwapColumns( int r1, int r2 );

	//! Swaps the specified rows and columns in the matrix.
	ID_INLINE idMatX&		SwapRowsColumns( int r1, int r2 );

	//! Removes the specified row from the matrix and returns a reference to the modified matrix.
	idMatX&					RemoveRow( int r );

	//! Removes a column from the matrix.
	idMatX&					RemoveColumn( int r );

	//! Removes a specified row and column from the matrix.
	idMatX&					RemoveRowColumn( int r );

	//! Clears the upper triangle of the matrix by setting all elements above the main diagonal to zero.
	ID_INLINE void			ClearUpperTriangle();

	//! Clears the lower triangle of the matrix by setting all elements below the diagonal to zero.
	ID_INLINE void			ClearLowerTriangle();

	//! Copies the lower triangle of the matrix to its upper triangle.
	void					CopyLowerToUpperTriangle();

	//! Copies a square sub-matrix from the input matrix to this matrix
	ID_INLINE void			SquareSubMatrix( const idMatX& m, int size );

	//! Returns the maximum absolute difference between corresponding elements of this matrix and another matrix.
	ID_INLINE float			MaxDifference( const idMatX& m ) const;

	//! Returns true if the matrix is square, false otherwise.
	ID_INLINE bool			IsSquare() const { return ( numRows == numColumns ); }

	//! Returns true if the matrix is zero within the specified epsilon tolerance.
	ID_INLINE bool			IsZero( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the matrix is identical to an identity matrix within a given epsilon tolerance.
	ID_INLINE bool			IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is diagonal, meaning all non-diagonal elements are zero within the given epsilon tolerance.
	ID_INLINE bool			IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the matrix is tri-diagonal within a given epsilon tolerance.
	ID_INLINE bool			IsTriDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Checks if the matrix is symmetric within a given epsilon tolerance.
	ID_INLINE bool			IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is orthogonal, meaning it is equal to its own transpose
	bool					IsOrthogonal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is orthonormal within the given epsilon tolerance.
	bool					IsOrthonormal( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is a P-matrix, meaning all its principal minors are positive.
	bool					IsPMatrix( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is a Z-matrix, where all non-diagonal elements are less than or equal to zero.
	bool					IsZMatrix( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is positive definite, using Gaussian elimination with the specified epsilon tolerance.
	bool					IsPositiveDefinite( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is symmetric positive definite
	bool					IsSymmetricPositiveDefinite( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is Positive Semi Definite
	bool					IsPositiveSemiDefinite( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns true if the matrix is Symmetric Positive Semi Definite
	bool					IsSymmetricPositiveSemiDefinite( const float epsilon = MATRIX_EPSILON ) const;

	//! Returns the trace of the matrix, which is the sum of the diagonal elements
	ID_INLINE float			Trace() const;

	//! Returns the determinant of the matrix
	ID_INLINE float			Determinant() const;

	//! Returns the transpose of this matrix
	ID_INLINE idMatX		Transpose() const;

	//! Transposes the matrix in place and returns a reference to itself.
	ID_INLINE idMatX&		TransposeSelf();

	//! Stores the transpose of this matrix in the provided destination matrix.
	ID_INLINE void			Transpose( idMatX& dst ) const;

	//! Returns the inverse of the matrix.
	ID_INLINE idMatX		Inverse() const;

	//! Computes the inverse of the matrix in-place and returns false if the matrix is singular.
	ID_INLINE bool			InverseSelf();

	//! Returns the inverse of the matrix
	ID_INLINE idMatX		InverseFast() const;

	//! Computes the fast inverse of the matrix in-place and returns false if the determinant is zero.
	ID_INLINE bool			InverseFastSelf();

	//! Computes the inverse of this matrix and stores the result in the provided destination matrix.
	ID_INLINE void			Inverse( idMatX& dst ) const;

	//! Computes the in-place inverse of a lower triangular matrix and returns false if the determinant is zero.
	bool					LowerTriangularInverse();

	//! Computes the in-place inverse of an upper triangular matrix and returns false if the determinant is zero.
	bool					UpperTriangularInverse();

	//! Subtracts the given matrix from this matrix in-place.
	ID_INLINE void			Subtract( const idMatX& a );

	//! Multiplies this matrix by the given vector and returns the result
	ID_INLINE idVecX		Multiply( const idVecX& vec ) const;

	//! Returns the result of multiplying the transpose of this matrix by the given vector.
	ID_INLINE idVecX		TransposeMultiply( const idVecX& vec ) const;

	//! Returns the matrix product of this matrix and matrix a
	ID_INLINE idMatX		Multiply( const idMatX& a ) const;

	//! Computes the transpose of this matrix multiplied by another matrix a
	ID_INLINE idMatX		TransposeMultiply( const idMatX& a ) const;

	//! Performs matrix-vector multiplication and stores the result in the provided destination vector.
	ID_INLINE void			Multiply( idVecX& dst, const idVecX& vec ) const;

	//! Computes dst += (*this) * vec
	ID_INLINE void			MultiplyAdd( idVecX& dst, const idVecX& vec ) const;

	//! Subtracts the product of this matrix and the given vector from the destination vector.
	ID_INLINE void			MultiplySub( idVecX& dst, const idVecX& vec ) const;

	//! Computes the product of the transposed matrix and a vector, storing the result in the destination vector.
	ID_INLINE void			TransposeMultiply( idVecX& dst, const idVecX& vec ) const;

	//! Adds the product of the transposed matrix and a vector to the destination vector
	ID_INLINE void			TransposeMultiplyAdd( idVecX& dst, const idVecX& vec ) const;

	//! Subtracts the matrix transpose multiplied by a vector from a destination vector
	ID_INLINE void			TransposeMultiplySub( idVecX& dst, const idVecX& vec ) const;

	//! Performs matrix multiplication of this matrix with matrix a and stores the result in dst
	ID_INLINE void			Multiply( idMatX& dst, const idMatX& a ) const;

	//! Computes the matrix product of the transpose of this matrix and matrix a, storing the result in dst.
	ID_INLINE void			TransposeMultiply( idMatX& dst, const idMatX& a ) const;

	//! Returns the total number of values in the matrix.
	ID_INLINE int			GetDimension() const;

	//! Returns a constant reference to a vec6 interpreted from the beginning of the specified row.
	ID_INLINE const idVec6& SubVec6( int row ) const;

	//! Returns a reference to the idVec6 starting at the specified row of the matrix.
	ID_INLINE idVec6&		SubVec6( int row );

	//! Returns a const idVecX representing a complete row of the matrix.
	ID_INLINE const idVecX	SubVecX( int row ) const;

	//! Returns a row from the matrix as a vector.
	ID_INLINE idVecX		SubVecX( int row );

	//! Returns a pointer to the const float array representation of the matrix.
	ID_INLINE const float*	ToFloatPtr() const;

	//! Returns a pointer to the float array representation of the matrix
	ID_INLINE float*		ToFloatPtr();

	//! Returns a string representation of the matrix with the specified decimal precision.
	const char*				ToString( int precision = 2 ) const;

	//! Updates the matrix by adding alpha times the outer product of vectors v and w
	void					Update_RankOne( const idVecX& v, const idVecX& w, float alpha );

	//! Updates the matrix by adding alpha times the outer product of vector v with itself.
	void					Update_RankOneSymmetric( const idVecX& v, float alpha );

	//! Updates a row and column of the matrix using vectors v and w at row index r.
	void					Update_RowColumn( const idVecX& v, const idVecX& w, int r );

	//! Updates a symmetric matrix row and column with values from a vector.
	void					Update_RowColumnSymmetric( const idVecX& v, int r );

	//! Updates the matrix by incrementally adding elements from vectors v and w.
	void					Update_Increment( const idVecX& v, const idVecX& w );

	//! Updates the matrix by incrementally adding a symmetric row and column based on the input vector
	void					Update_IncrementSymmetric( const idVecX& v );

	//! Updates the matrix by removing the specified row and column.
	void					Update_Decrement( int r );

	//! Computes the inverse of the matrix using Gauss-Jordan elimination and returns true if successful.
	bool					Inverse_GaussJordan();

	//! Updates the in-place inverse using the Sherman-Morrison formula
	bool					Inverse_UpdateRankOne( const idVecX& v, const idVecX& w, float alpha );

	//! Updates the inverse matrix by modifying a row and column using rank-one updates.
	bool					Inverse_UpdateRowColumn( const idVecX& v, const idVecX& w, int r );

	//! Updates the matrix inverse by incrementing its size and performing row/column operations.
	bool					Inverse_UpdateIncrement( const idVecX& v, const idVecX& w );

	//! Updates the in-place inverse to obtain the inverse of the matrix with row r and column r removed
	bool					Inverse_UpdateDecrement( const idVecX& v, const idVecX& w, int r );

	//! Solves the linear system Ax = b using the inverted matrix A.
	void					Inverse_Solve( idVecX& x, const idVecX& b ) const;

	//! Performs in-place LU factorization of the matrix with optional determinant calculation.
	bool					LU_Factor( int* index, float* det = NULL );

	/*!
		\brief Updates the in-place LU factorization to obtain the factors for the matrix LU + alpha * v * w

		This function performs an in-place rank-one update to an existing LU factorization of a matrix. It modifies the current matrix in-place to reflect the addition of a rank-one matrix alpha * v *
	   w to the original matrix. The update is performed using the LU decomposition technique, where the matrix is decomposed into a lower triangular matrix L and an upper triangular matrix U. The
	   function takes into account the indexing specified by the index parameter if provided, otherwise it uses default indexing. The function returns false if a zero diagonal element is encountered
	   during the process, which would indicate a failure in the factorization.

		\param v The vector v in the rank-one update alpha * v * w
		\param w The vector w in the rank-one update alpha * v * w
		\param alpha The scalar alpha in the rank-one update alpha * v * w
		\param index Optional array of indices for vector v, or NULL to use default indexing
		\return True if the update was successful, false if a zero diagonal element was encountered
	*/
	bool					LU_UpdateRankOne( const idVecX& v, const idVecX& w, float alpha, int* index );

	/*!
		\brief Updates a row and column of the matrix using rank-one update operations with the provided vectors and index array.

		This function performs an LU decomposition update on a row and column of the matrix. It uses two input vectors v and w, along with a row index r and an optional index array to modify the
	   matrix in place. The update is performed using rank-one operations to maintain the LU factorization structure. The function handles the case where an index array is provided for reordering
	   operations.

		\param v Input vector used for updating the matrix row
		\param w Input vector used for updating the matrix column
		\param r Row index for the update operation
		\param index Optional array for reordering operations, can be NULL
		\return True if the update operation was successful, false if a zero diagonal element was encountered during the update
	*/
	bool					LU_UpdateRowColumn( const idVecX& v, const idVecX& w, int r, int* index );

	//! Updates the LU decomposition with an incremental row and column
	bool					LU_UpdateIncrement( const idVecX& v, const idVecX& w, int* index );

	/*!
		\brief Updates the in-place LU factorization to obtain the factors for the matrix with row r and column r removed

		This function performs an in-place update of an LU factorization by removing a specified row and column from the matrix. It handles both cases where an index array is provided for permutation
	   tracking and when no permutation tracking is needed. The function ensures numerical stability by checking for pivot conditions and performing necessary row interchanges. It updates the
	   factorization data structures and physically removes the specified row and column from the matrix.

		\param v Vector used for updating the row
		\param w Vector used for updating the column
		\param u Vector used for rank-one update
		\param r Index of the row and column to remove
		\param index Pointer to the permutation index array, can be NULL
		\return True if the update was successful, false otherwise
	*/
	bool					LU_UpdateDecrement( const idVecX& v, const idVecX& w, const idVecX& u, int r, int* index );

	//! Solves the linear system Ax = b using LU decomposition.
	void					LU_Solve( idVecX& x, const idVecX& b, const int* index ) const;

	//! Calculates the inverse of the matrix using LU decomposition
	void					LU_Inverse( idMatX& inv, const int* index ) const;

	//! Unpacks the in-place LU factorization into separate lower and upper triangular matrices.
	void					LU_UnpackFactors( idMatX& L, idMatX& U ) const;

	//! Multiplies the factors of the in-place LU factorization to form the original matrix.
	void					LU_MultiplyFactors( idMatX& m, const int* index ) const;

	//! Performs in-place QR factorization of the matrix using the provided vectors for temporary storage.
	bool					QR_Factor( idVecX& c, idVecX& d );

	/*!
		\brief Updates the unpacked QR factorization to obtain the factors for the matrix QR + alpha * v * w

		Performs an in-place update of the QR factorization stored in matrix R to reflect the addition of a rank-one matrix alpha * v * w. The function processes the vectors v and w to compute the
	   necessary rotations and updates the R matrix accordingly. It assumes the input vectors are properly sized and the matrix R is in the correct format for the operation.

		\param R The matrix containing the QR factorization to be updated
		\param v The vector used in the rank-one update
		\param w The vector used in the rank-one update
		\param alpha The scalar multiplier for the rank-one update
		\return true if the update was successful
		\throws Assertion failure if vector v is smaller than the number of columns in R or if vector w is smaller than the number of rows in R
	*/
	bool					QR_UpdateRankOne( idMatX& R, const idVecX& v, const idVecX& w, float alpha );

	/*!
		\brief Performs a rank-one update on the QR decomposition matrix R using vectors v and w

		This function updates the QR decomposition matrix R by performing two rank-one updates. It takes vectors v and w and uses them to modify the matrix R in a way that preserves the QR
	   decomposition properties. The function first initializes a vector s with a 1 at position r, then applies two rank-one updates using the QR_UpdateRankOne helper function. The update is only
	   valid if the input parameters satisfy certain constraints, as enforced by assertions.

		\param R Reference to the QR decomposition matrix to be updated
		\param v Vector used in the first rank-one update operation
		\param w Vector used in the second rank-one update operation
		\param r Index position for the update, must be within the bounds of both rows and columns
		\return True if both rank-one updates succeed, false otherwise
	*/
	bool					QR_UpdateRowColumn( idMatX& R, const idVecX& v, const idVecX& w, int r );

	//! Performs an incremental QR update on the matrix R using vectors v and w
	bool					QR_UpdateIncrement( idMatX& R, const idVecX& v, const idVecX& w );

	/*!
		\brief Updates the unpacked QR factorization to obtain the factors for the matrix with row r and column r removed

		This function performs a rank-revealing QR decomposition update by removing a specified row and column from the matrix. It modifies the R matrix in place to reflect the removal of the
	   specified row and column. The function first prepares the necessary vectors for the update, then calls the internal row and column update function, and finally removes the specified row and
	   column from both the current matrix and the R matrix. The update is only performed if the row and column indices are valid and the matrix dimensions are consistent.

		\param R Reference to the R matrix to be updated
		\param v Vector representing the row to be removed
		\param w Vector representing the column to be removed
		\param r Index of the row and column to be removed
		\return True if the update was successful, false otherwise
	*/
	bool					QR_UpdateDecrement( idMatX& R, const idVecX& v, const idVecX& w, int r );

	//! Solves the linear system Ax = b using QR factorization
	void					QR_Solve( idVecX& x, const idVecX& b, const idVecX& c, const idVecX& d ) const;

	//! Solves the linear system Ax = b using QR factorization.
	void					QR_Solve( idVecX& x, const idVecX& b, const idMatX& R ) const;

	//! Computes the inverse of a matrix using QR decomposition
	void					QR_Inverse( idMatX& inv, const idVecX& c, const idVecX& d ) const;

	/*!
		\brief Unpacks an in-place QR factorization into the provided Q and R matrices

		This function reconstructs the original matrices Q and R from an in-place QR factorization stored within the current matrix. The factorization is unpacked such that Q is an orthogonal matrix
	   and R is an upper triangular matrix. The input vectors c and d contain the necessary scaling factors and diagonal elements for the reconstruction process. The operation modifies the provided Q
	   and R matrices in-place, with Q being initialized as an identity matrix and R being initialized as a zero matrix before reconstruction.

		\param Q Output matrix to store the orthogonal Q factor
		\param R Output matrix to store the upper triangular R factor
		\param c Vector containing scaling factors for the reconstruction
		\param d Vector containing diagonal elements for the R matrix
	*/
	void					QR_UnpackFactors( idMatX& Q, idMatX& R, const idVecX& c, const idVecX& d ) const;

	//! Multiplies the factors of the in-place QR factorization to form the original matrix.
	void					QR_MultiplyFactors( idMatX& m, const idVecX& c, const idVecX& d ) const;

	//! Performs singular value decomposition factorization on the matrix, returning the diagonal vector and right singular vectors.
	bool					SVD_Factor( idVecX& w, idMatX& V );

	//! Solves the linear system Ax = b using SVD factorization components.
	void					SVD_Solve( idVecX& x, const idVecX& b, const idVecX& w, const idMatX& V ) const;

	//! Computes the inverse of a matrix using its SVD decomposition components
	void					SVD_Inverse( idMatX& inv, const idVecX& w, const idMatX& V ) const;

	//! Multiplies the SVD factors to reconstruct the original matrix
	void					SVD_MultiplyFactors( idMatX& m, const idVecX& w, const idMatX& V ) const;

	//! Performs in-place Cholesky factorization on the matrix, returning true if successful.
	bool					Cholesky_Factor();

	//! Updates the Cholesky factorization by rank-one modification with the given vector and scalar alpha
	bool					Cholesky_UpdateRankOne( const idVecX& v, float alpha, int offset = 0 );

	//! Performs a Cholesky update of a row and column in the matrix.
	bool					Cholesky_UpdateRowColumn( const idVecX& v, int r );

	//! Performs an incremental Cholesky update using the provided vector
	bool					Cholesky_UpdateIncrement( const idVecX& v );

	//! Updates the in-place Cholesky factorization to obtain the factors for the matrix with row r and column r removed
	bool					Cholesky_UpdateDecrement( const idVecX& v, int r );

	//! Solves the linear system Ax = b using Cholesky factorization where A is factored in-place as LL'.
	void					Cholesky_Solve( idVecX& x, const idVecX& b ) const;

	//! Computes the inverse of a matrix using its Cholesky factorization and stores the result in the provided matrix.
	void					Cholesky_Inverse( idMatX& inv ) const;

	//! Multiplies the Cholesky factors to reconstruct the original matrix.
	void					Cholesky_MultiplyFactors( idMatX& m ) const;

	//! Performs in-place LDLT factorization of the matrix.
	bool					LDLT_Factor();

	//! Updates the LDL' factorization in-place to incorporate a rank-one correction scaled by alpha.
	bool					LDLT_UpdateRankOne( const idVecX& v, float alpha, int offset = 0 );

	//! Updates a row and column of the matrix using LDLT decomposition with the given vector and row index.
	bool					LDLT_UpdateRowColumn( const idVecX& v, int r );

	//! Updates the LDLT decomposition by incrementally adding a new row and column based on the provided vector.
	bool					LDLT_UpdateIncrement( const idVecX& v );

	//! Updates the in-place LDLT factorization by removing row and column r after applying a rank-one update.
	bool					LDLT_UpdateDecrement( const idVecX& v, int r );

	//! Solves the linear system Ax = b using the in-place LDL^T factorization of matrix A.
	void					LDLT_Solve( idVecX& x, const idVecX& b ) const;

	//! Computes the inverse of the matrix using LDLT factorization and stores the result in the provided matrix.
	void					LDLT_Inverse( idMatX& inv ) const;

	//! Unpacks the in-place LDLT factorization into separate lower triangular matrix L and diagonal matrix D
	void					LDLT_UnpackFactors( idMatX& L, idMatX& D ) const;

	//! Multiplies the factors of the in-place LDLT factorization to form the original matrix
	void					LDLT_MultiplyFactors( idMatX& m ) const;

	//! Clears all elements outside the main diagonal and the two adjacent diagonals in the matrix.
	void					TriDiagonal_ClearTriangles();

	//! Solves the linear system Ax = b where A is a tridiagonal matrix.
	bool					TriDiagonal_Solve( idVecX& x, const idVecX& b ) const;

	//! Calculates the inverse of a tri-diagonal matrix.
	void					TriDiagonal_Inverse( idMatX& inv ) const;

	//! Computes eigenvalues for a symmetric tridiagonal matrix and stores them in the provided vector.
	bool					Eigen_SolveSymmetricTriDiagonal( idVecX& eigenValues );

	//! Computes eigenvalues for a symmetric matrix and stores them in the provided vector.
	bool					Eigen_SolveSymmetric( idVecX& eigenValues );

	//! Computes the eigenvalues and eigenvectors for a square matrix.
	bool					Eigen_Solve( idVecX& realEigenValues, idVecX& imaginaryEigenValues );

	//! Sorts the eigenvalues in increasing order along with their corresponding eigenvectors.
	void					Eigen_SortIncreasing( idVecX& eigenValues );

	//! Sorts the eigenvalues in decreasing order
	void					Eigen_SortDecreasing( idVecX& eigenValues );

	//! Performs comprehensive tests on various matrix operations and their inverses.
	static void				Test();

private:
	int			  numRows;	  // number of rows
	int			  numColumns; // number of columns
	int			  alloced;	  // floats allocated, if -1 then mat points to data set with SetData
	float*		  mat;		  // memory the matrix is stored

	static float  temp[MATX_MAX_TEMP + 4]; // used to store intermediate results
	static float* tempPtr;				   // pointer to 16 byte aligned temporary memory
	static int	  tempIndex;			   // index into memory pool, wraps around

private:
	//! Sets the size of a temporary matrix allocation with the specified number of rows and columns.
	void  SetTempSize( int rows, int columns );

	//! Computes the determinant of the matrix using generic LU factorization.
	float DeterminantGeneric() const;

	//! Computes the inverse of the matrix in place using generic Gaussian elimination with partial pivoting.
	bool  InverseSelfGeneric();

	/*!
		\brief Performs a Jacobi rotation on the rows i and i+1 of the unpacked QR factors.

		This function applies a Givens rotation to the rows i and i+1 of the matrix R and to the columns i and i+1 of the current matrix. The rotation is determined by the parameters a and b, which
	   define the direction of the rotation in the plane formed by the two rows/columns. The function modifies the matrix R in place and also updates the current matrix with the corresponding column
	   rotations.

		\param R The matrix to be rotated, modified in place
		\param i The index of the first row/column to rotate
		\param a The first component of the rotation vector
		\param b The second component of the rotation vector
	*/
	void  QR_Rotate( idMatX& R, int i, float a, float b );

	//! Computes the square root of the sum of squares of two floating-point numbers without underflow or overflow.
	float Pythag( float a, float b ) const;

	//! Performs bidiagonalization for SVD computation on the matrix
	void  SVD_BiDiag( idVecX& w, idVecX& rv1, float& anorm );

	//! Initializes the W and V matrices for Singular Value Decomposition
	void  SVD_InitialWV( idVecX& w, idMatX& V, idVecX& rv1 );

	//! Performs Householder reduction to symmetric tridiagonal form on the matrix
	void  HouseholderReduction( idVecX& diag, idVecX& subd );

	//! Performs the QL algorithm with implicit shifts to compute eigenvalues and eigenvectors of a symmetric tridiagonal matrix.
	bool  QL( idVecX& diag, idVecX& subd );

	//! Performs Hessenberg reduction on the matrix H
	void  HessenbergReduction( idMatX& H );

	/*!
		\brief Performs complex scalar division on two complex numbers and stores the result in the provided output parameters

		This function computes the division of two complex numbers represented as (xr + xi*i) / (yr + yi*i) and stores the real and imaginary parts of the result in the provided output parameters. The
	   implementation uses a numerically stable approach to avoid overflow issues by choosing the larger absolute value component as the divisor during the computation.

		\param xr Real part of the numerator complex number
		\param xi Imaginary part of the numerator complex number
		\param yr Real part of the denominator complex number
		\param yi Imaginary part of the denominator complex number
		\param cdivr Output parameter for the real part of the division result
		\param cdivi Output parameter for the imaginary part of the division result
	*/
	void  ComplexDivision( float xr, float xi, float yr, float yi, float& cdivr, float& cdivi );

	//! Reduces a Hessenberg matrix to real Schur form and computes eigenvalues
	bool  HessenbergToRealSchur( idMatX& H, idVecX& realEigenValues, idVecX& imaginaryEigenValues );
};

ID_INLINE idMatX::idMatX()
{
	numRows = numColumns = alloced = 0;
	mat							   = NULL;
}

ID_INLINE idMatX::~idMatX()
{
	// if not temp memory
	if( mat != NULL && ( mat < idMatX::tempPtr || mat > idMatX::tempPtr + MATX_MAX_TEMP ) && alloced != -1 ) { Mem_Free16( mat ); }
}

ID_INLINE idMatX::idMatX( int rows, int columns )
{
	numRows = numColumns = alloced = 0;
	mat							   = NULL;
	SetSize( rows, columns );
}

ID_INLINE idMatX::idMatX( const idMatX& other )
{
	numRows = numColumns = alloced = 0;
	mat							   = NULL;
	Set( other.GetNumRows(), other.GetNumColumns(), other.ToFloatPtr() );
}

ID_INLINE idMatX::idMatX( int rows, int columns, float* src )
{
	numRows = numColumns = alloced = 0;
	mat							   = NULL;
	SetData( rows, columns, src );
}

ID_INLINE void idMatX::Set( int rows, int columns, const float* src )
{
	SetSize( rows, columns );
	memcpy( this->mat, src, rows * columns * sizeof( float ) );
}

ID_INLINE void idMatX::Set( const idMat3& m1, const idMat3& m2 )
{
	SetSize( 3, 6 );
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			mat[( i + 0 ) * numColumns + ( j + 0 )] = m1[i][j];
			mat[( i + 0 ) * numColumns + ( j + 3 )] = m2[i][j];
		}
	}
}

ID_INLINE void idMatX::Set( const idMat3& m1, const idMat3& m2, const idMat3& m3, const idMat3& m4 )
{
	SetSize( 6, 6 );
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			mat[( i + 0 ) * numColumns + ( j + 0 )] = m1[i][j];
			mat[( i + 0 ) * numColumns + ( j + 3 )] = m2[i][j];
			mat[( i + 3 ) * numColumns + ( j + 0 )] = m3[i][j];
			mat[( i + 3 ) * numColumns + ( j + 3 )] = m4[i][j];
		}
	}
}

ID_INLINE const float* idMatX::operator[]( int index ) const
{
	assert( ( index >= 0 ) && ( index < numRows ) );
	return mat + index * numColumns;
}

ID_INLINE float* idMatX::operator[]( int index )
{
	assert( ( index >= 0 ) && ( index < numRows ) );
	return mat + index * numColumns;
}

ID_INLINE idMatX& idMatX::operator=( const idMatX& a )
{
	SetSize( a.numRows, a.numColumns );
	int s = a.numRows * a.numColumns;
#ifdef MATX_SIMD
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( mat + i, _mm_load_ps( a.mat + i ) );
	}
#else
	memcpy( mat, a.mat, s * sizeof( float ) );
#endif
	idMatX::tempIndex = 0;
	return *this;
}

ID_INLINE idMatX idMatX::operator*( const float a ) const
{
	idMatX m;

	m.SetTempSize( numRows, numColumns );
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	__m128 va = _mm_load1_ps( &a );
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( m.mat + i, _mm_mul_ps( _mm_load_ps( mat + i ), va ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		m.mat[i] = mat[i] * a;
	}
#endif
	return m;
}

ID_INLINE idVecX idMatX::operator*( const idVecX& vec ) const
{
	assert( numColumns == vec.GetSize() );

	idVecX dst;
	dst.SetTempSize( numRows );
	Multiply( dst, vec );
	return dst;
}

ID_INLINE idMatX idMatX::operator*( const idMatX& a ) const
{
	assert( numColumns == a.numRows );

	idMatX dst;
	dst.SetTempSize( numRows, a.numColumns );
	Multiply( dst, a );
	return dst;
}

ID_INLINE idMatX idMatX::operator+( const idMatX& a ) const
{
	idMatX m;

	assert( numRows == a.numRows && numColumns == a.numColumns );
	m.SetTempSize( numRows, numColumns );
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( m.mat + i, _mm_add_ps( _mm_load_ps( mat + i ), _mm_load_ps( a.mat + i ) ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		m.mat[i] = mat[i] + a.mat[i];
	}
#endif
	return m;
}

ID_INLINE idMatX idMatX::operator-( const idMatX& a ) const
{
	idMatX m;

	assert( numRows == a.numRows && numColumns == a.numColumns );
	m.SetTempSize( numRows, numColumns );
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( m.mat + i, _mm_sub_ps( _mm_load_ps( mat + i ), _mm_load_ps( a.mat + i ) ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		m.mat[i] = mat[i] - a.mat[i];
	}
#endif
	return m;
}

ID_INLINE idMatX& idMatX::operator*=( const float a )
{
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	__m128 va = _mm_load1_ps( &a );
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( mat + i, _mm_mul_ps( _mm_load_ps( mat + i ), va ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		mat[i] *= a;
	}
#endif
	idMatX::tempIndex = 0;
	return *this;
}

ID_INLINE idMatX& idMatX::operator*=( const idMatX& a )
{
	*this			  = *this * a;
	idMatX::tempIndex = 0;
	return *this;
}

ID_INLINE idMatX& idMatX::operator+=( const idMatX& a )
{
	assert( numRows == a.numRows && numColumns == a.numColumns );
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( mat + i, _mm_add_ps( _mm_load_ps( mat + i ), _mm_load_ps( a.mat + i ) ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		mat[i] += a.mat[i];
	}
#endif
	idMatX::tempIndex = 0;
	return *this;
}

ID_INLINE idMatX& idMatX::operator-=( const idMatX& a )
{
	assert( numRows == a.numRows && numColumns == a.numColumns );
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( mat + i, _mm_sub_ps( _mm_load_ps( mat + i ), _mm_load_ps( a.mat + i ) ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		mat[i] -= a.mat[i];
	}
#endif
	idMatX::tempIndex = 0;
	return *this;
}

//! Multiplies a matrix by a scalar value.
ID_INLINE idMatX operator*( const float a, idMatX const& m )
{
	return m * a;
}

//! Multiplies a vector by a matrix and returns the resulting vector.
ID_INLINE idVecX operator*( const idVecX& vec, const idMatX& m )
{
	return m * vec;
}

//! Multiplies the vector by the matrix and assigns the result back to the vector.
ID_INLINE idVecX& operator*=( idVecX& vec, const idMatX& m )
{
	vec = m * vec;
	return vec;
}

ID_INLINE bool idMatX::Compare( const idMatX& a ) const
{
	assert( numRows == a.numRows && numColumns == a.numColumns );

	int s = numRows * numColumns;
	for( int i = 0; i < s; i++ ) {
		if( mat[i] != a.mat[i] ) { return false; }
	}
	return true;
}

ID_INLINE bool idMatX::Compare( const idMatX& a, const float epsilon ) const
{
	assert( numRows == a.numRows && numColumns == a.numColumns );

	int s = numRows * numColumns;
	for( int i = 0; i < s; i++ ) {
		if( idMath::Fabs( mat[i] - a.mat[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idMatX::operator==( const idMatX& a ) const
{
	return Compare( a );
}

ID_INLINE bool idMatX::operator!=( const idMatX& a ) const
{
	return !Compare( a );
}

ID_INLINE void idMatX::SetSize( int rows, int columns )
{
	if( rows != numRows || columns != numColumns || mat == NULL ) {
		assert( mat < idMatX::tempPtr || mat > idMatX::tempPtr + MATX_MAX_TEMP );
		int alloc = ( rows * columns + 3 ) & ~3;
		if( alloc > alloced && alloced != -1 ) {
			if( mat != NULL ) { Mem_Free16( mat ); }
			mat		= ( float* )Mem_Alloc16( alloc * sizeof( float ), TAG_MATH );
			alloced = alloc;
		}
		numRows	   = rows;
		numColumns = columns;
		MATX_CLEAREND();
	}
}

ID_INLINE void idMatX::SetTempSize( int rows, int columns )
{
	int newSize;

	newSize = ( rows * columns + 3 ) & ~3;
	assert( newSize < MATX_MAX_TEMP );
	if( idMatX::tempIndex + newSize > MATX_MAX_TEMP ) { idMatX::tempIndex = 0; }
	mat = idMatX::tempPtr + idMatX::tempIndex;
	idMatX::tempIndex += newSize;
	alloced	   = newSize;
	numRows	   = rows;
	numColumns = columns;
	MATX_CLEAREND();
}

ID_INLINE void idMatX::SetData( int rows, int columns, float* data )
{
	assert( mat < idMatX::tempPtr || mat > idMatX::tempPtr + MATX_MAX_TEMP );
	if( mat != NULL && alloced != -1 ) { Mem_Free16( mat ); }
	// RB: changed UINT_PTR to uintptr_t
	assert( ( ( ( uintptr_t )data ) & 15 ) == 0 ); // data must be 16 byte aligned
	// RB end
	mat		   = data;
	alloced	   = -1;
	numRows	   = rows;
	numColumns = columns;
	MATX_CLEAREND();
}

ID_INLINE void idMatX::SetDataCacheLines( int rows, int columns, float* data, bool clear )
{
	if( mat != NULL && alloced != -1 ) { Mem_Free( mat ); }

	// RB: changed UINT_PTR to uintptr_t
	assert( ( ( ( uintptr_t )data ) & 127 ) == 0 ); // data must be 128 byte aligned
	// RB end

	mat		   = data;
	alloced	   = -1;
	numRows	   = rows;
	numColumns = columns;

	if( clear ) {
		int size = numRows * numColumns * sizeof( float );
		for( int i = 0; i < size; i += CACHE_LINE_SIZE ) {
			ZeroCacheLine( mat, i );
		}
	} else {
		MATX_CLEAREND();
	}
}

ID_INLINE void idMatX::Zero()
{
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( mat + i, _mm_setzero_ps() );
	}
#else
	memset( mat, 0, numRows * numColumns * sizeof( float ) );
#endif
}

ID_INLINE void idMatX::Zero( int rows, int columns )
{
	SetSize( rows, columns );
	Zero();
}

ID_INLINE void idMatX::Identity()
{
	assert( numRows == numColumns );
	Zero();
	for( int i = 0; i < numRows; i++ ) {
		mat[i * numColumns + i] = 1.0f;
	}
}

ID_INLINE void idMatX::Identity( int rows, int columns )
{
	assert( rows == columns );
	SetSize( rows, columns );
	idMatX::Identity();
}

ID_INLINE void idMatX::Diag( const idVecX& v )
{
	Zero( v.GetSize(), v.GetSize() );
	for( int i = 0; i < v.GetSize(); i++ ) {
		mat[i * numColumns + i] = v[i];
	}
}

ID_INLINE void idMatX::Random( int seed, float l, float u )
{
	idRandom rnd( seed );

	float	 c = u - l;
	int		 s = numRows * numColumns;
	for( int i = 0; i < s; i++ ) {
		mat[i] = l + rnd.RandomFloat() * c;
	}
}

ID_INLINE void idMatX::Random( int rows, int columns, int seed, float l, float u )
{
	idRandom rnd( seed );

	SetSize( rows, columns );
	float c = u - l;
	int	  s = numRows * numColumns;
	for( int i = 0; i < s; i++ ) {
		mat[i] = l + rnd.RandomFloat() * c;
	}
}

ID_INLINE void idMatX::Negate()
{
	int s = numRows * numColumns;
#ifdef MATX_SIMD
	ALIGN16( const unsigned int signBit[4] ) = { IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK };
	for( int i = 0; i < s; i += 4 ) {
		_mm_store_ps( mat + i, _mm_xor_ps( _mm_load_ps( mat + i ), ( __m128& )signBit[0] ) );
	}
#else
	for( int i = 0; i < s; i++ ) {
		mat[i] = -mat[i];
	}
#endif
}

ID_INLINE void idMatX::Clamp( float min, float max )
{
	int s = numRows * numColumns;
	for( int i = 0; i < s; i++ ) {
		if( mat[i] < min ) {
			mat[i] = min;
		} else if( mat[i] > max ) {
			mat[i] = max;
		}
	}
}

ID_INLINE idMatX& idMatX::SwapRows( int r1, int r2 )
{
	float* ptr1 = mat + r1 * numColumns;
	float* ptr2 = mat + r2 * numColumns;
	for( int i = 0; i < numColumns; i++ ) {
		SwapValues( ptr1[i], ptr2[i] );
	}
	return *this;
}

ID_INLINE idMatX& idMatX::SwapColumns( int r1, int r2 )
{
	float* ptr = mat;
	for( int i = 0; i < numRows; i++, ptr += numColumns ) {
		SwapValues( ptr[r1], ptr[r2] );
	}
	return *this;
}

ID_INLINE idMatX& idMatX::SwapRowsColumns( int r1, int r2 )
{
	SwapRows( r1, r2 );
	SwapColumns( r1, r2 );
	return *this;
}

ID_INLINE void idMatX::ClearUpperTriangle()
{
	assert( numRows == numColumns );
	for( int i = numRows - 2; i >= 0; i-- ) {
		memset( mat + i * numColumns + i + 1, 0, ( numColumns - 1 - i ) * sizeof( float ) );
	}
}

ID_INLINE void idMatX::ClearLowerTriangle()
{
	assert( numRows == numColumns );
	for( int i = 1; i < numRows; i++ ) {
		memset( mat + i * numColumns, 0, i * sizeof( float ) );
	}
}

ID_INLINE void idMatX::SquareSubMatrix( const idMatX& m, int size )
{
	assert( size <= m.numRows && size <= m.numColumns );
	SetSize( size, size );
	for( int i = 0; i < size; i++ ) {
		memcpy( mat + i * numColumns, m.mat + i * m.numColumns, size * sizeof( float ) );
	}
}

ID_INLINE float idMatX::MaxDifference( const idMatX& m ) const
{
	assert( numRows == m.numRows && numColumns == m.numColumns );

	float maxDiff = -1.0f;
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numColumns; j++ ) {
			float diff = idMath::Fabs( mat[i * numColumns + j] - m[i][j] );
			if( maxDiff < 0.0f || diff > maxDiff ) { maxDiff = diff; }
		}
	}
	return maxDiff;
}

ID_INLINE bool idMatX::IsZero( const float epsilon ) const
{
	// returns true if (*this) == Zero
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numColumns; j++ ) {
			if( idMath::Fabs( mat[i * numColumns + j] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMatX::IsIdentity( const float epsilon ) const
{
	// returns true if (*this) == Identity
	assert( numRows == numColumns );
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numColumns; j++ ) {
			if( idMath::Fabs( mat[i * numColumns + j] - ( float )( i == j ) ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMatX::IsDiagonal( const float epsilon ) const
{
	// returns true if all elements are zero except for the elements on the diagonal
	assert( numRows == numColumns );
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numColumns; j++ ) {
			if( i != j && idMath::Fabs( mat[i * numColumns + j] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMatX::IsTriDiagonal( const float epsilon ) const
{
	// returns true if all elements are zero except for the elements on the diagonal plus or minus one column

	if( numRows != numColumns ) { return false; }
	for( int i = 0; i < numRows - 2; i++ ) {
		for( int j = i + 2; j < numColumns; j++ ) {
			if( idMath::Fabs( ( *this )[i][j] ) > epsilon ) { return false; }
			if( idMath::Fabs( ( *this )[j][i] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE bool idMatX::IsSymmetric( const float epsilon ) const
{
	// (*this)[i][j] == (*this)[j][i]
	if( numRows != numColumns ) { return false; }
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numColumns; j++ ) {
			if( idMath::Fabs( mat[i * numColumns + j] - mat[j * numColumns + i] ) > epsilon ) { return false; }
		}
	}
	return true;
}

ID_INLINE float idMatX::Trace() const
{
	float trace = 0.0f;

	assert( numRows == numColumns );

	// sum of elements on the diagonal
	for( int i = 0; i < numRows; i++ ) {
		trace += mat[i * numRows + i];
	}
	return trace;
}

ID_INLINE float idMatX::Determinant() const
{
	assert( numRows == numColumns );

	switch( numRows ) {
		case 1:
			return mat[0];
		case 2:
			return reinterpret_cast<const idMat2*>( mat )->Determinant();
		case 3:
			return reinterpret_cast<const idMat3*>( mat )->Determinant();
		case 4:
			return reinterpret_cast<const idMat4*>( mat )->Determinant();
		case 5:
			return reinterpret_cast<const idMat5*>( mat )->Determinant();
		case 6:
			return reinterpret_cast<const idMat6*>( mat )->Determinant();
		default:
			return DeterminantGeneric();
	}
}

ID_INLINE idMatX idMatX::Transpose() const
{
	idMatX transpose;

	transpose.SetTempSize( numColumns, numRows );

	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numColumns; j++ ) {
			transpose.mat[j * transpose.numColumns + i] = mat[i * numColumns + j];
		}
	}

	return transpose;
}

ID_INLINE idMatX& idMatX::TransposeSelf()
{
	*this = Transpose();
	return *this;
}

ID_INLINE void idMatX::Transpose( idMatX& dst ) const
{
	dst = Transpose();
}

ID_INLINE idMatX idMatX::Inverse() const
{
	idMatX invMat;

	invMat.SetTempSize( numRows, numColumns );
	memcpy( invMat.mat, mat, numRows * numColumns * sizeof( float ) );
	verify( invMat.InverseSelf() );
	return invMat;
}

ID_INLINE bool idMatX::InverseSelf()
{
	assert( numRows == numColumns );

	switch( numRows ) {
		case 1:
			if( idMath::Fabs( mat[0] ) < MATRIX_INVERSE_EPSILON ) { return false; }
			mat[0] = 1.0f / mat[0];
			return true;
		case 2:
			return reinterpret_cast<idMat2*>( mat )->InverseSelf();
		case 3:
			return reinterpret_cast<idMat3*>( mat )->InverseSelf();
		case 4:
			return reinterpret_cast<idMat4*>( mat )->InverseSelf();
		case 5:
			return reinterpret_cast<idMat5*>( mat )->InverseSelf();
		case 6:
			return reinterpret_cast<idMat6*>( mat )->InverseSelf();
		default:
			return InverseSelfGeneric();
	}
}

ID_INLINE idMatX idMatX::InverseFast() const
{
	idMatX invMat;

	invMat.SetTempSize( numRows, numColumns );
	memcpy( invMat.mat, mat, numRows * numColumns * sizeof( float ) );
	verify( invMat.InverseFastSelf() );
	return invMat;
}

ID_INLINE bool idMatX::InverseFastSelf()
{
	assert( numRows == numColumns );

	switch( numRows ) {
		case 1:
			if( idMath::Fabs( mat[0] ) < MATRIX_INVERSE_EPSILON ) { return false; }
			mat[0] = 1.0f / mat[0];
			return true;
		case 2:
			return reinterpret_cast<idMat2*>( mat )->InverseFastSelf();
		case 3:
			return reinterpret_cast<idMat3*>( mat )->InverseFastSelf();
		case 4:
			return reinterpret_cast<idMat4*>( mat )->InverseFastSelf();
		case 5:
			return reinterpret_cast<idMat5*>( mat )->InverseFastSelf();
		case 6:
			return reinterpret_cast<idMat6*>( mat )->InverseFastSelf();
		default:
			return InverseSelfGeneric();
	}
}

ID_INLINE void idMatX::Inverse( idMatX& dst ) const
{
	dst = InverseFast();
}

ID_INLINE void idMatX::Subtract( const idMatX& a )
{
	( *this ) -= a;
}

ID_INLINE idVecX idMatX::Multiply( const idVecX& vec ) const
{
	assert( numColumns == vec.GetSize() );

	idVecX dst;
	dst.SetTempSize( numRows );
	Multiply( dst, vec );
	return dst;
}

ID_INLINE idMatX idMatX::Multiply( const idMatX& a ) const
{
	assert( numColumns == a.numRows );

	idMatX dst;
	dst.SetTempSize( numRows, a.numColumns );
	Multiply( dst, a );
	return dst;
}

ID_INLINE idVecX idMatX::TransposeMultiply( const idVecX& vec ) const
{
	assert( numRows == vec.GetSize() );

	idVecX dst;
	dst.SetTempSize( numColumns );
	TransposeMultiply( dst, vec );
	return dst;
}

ID_INLINE idMatX idMatX::TransposeMultiply( const idMatX& a ) const
{
	assert( numRows == a.numRows );

	idMatX dst;
	dst.SetTempSize( numColumns, a.numColumns );
	TransposeMultiply( dst, a );
	return dst;
}

ID_INLINE void idMatX::Multiply( idVecX& dst, const idVecX& vec ) const
{
	dst.SetSize( numRows );
	const float* mPtr	= mat;
	const float* vPtr	= vec.ToFloatPtr();
	float*		 dstPtr = dst.ToFloatPtr();
	float*		 temp	= ( float* )_alloca16( numRows * sizeof( float ) );
	for( int i = 0; i < numRows; i++ ) {
		float sum = mPtr[0] * vPtr[0];
		for( int j = 1; j < numColumns; j++ ) {
			sum += mPtr[j] * vPtr[j];
		}
		temp[i] = sum;
		mPtr += numColumns;
	}
	for( int i = 0; i < numRows; i++ ) {
		dstPtr[i] = temp[i];
	}
}

ID_INLINE void idMatX::MultiplyAdd( idVecX& dst, const idVecX& vec ) const
{
	assert( dst.GetSize() == numRows );
	const float* mPtr	= mat;
	const float* vPtr	= vec.ToFloatPtr();
	float*		 dstPtr = dst.ToFloatPtr();
	float*		 temp	= ( float* )_alloca16( numRows * sizeof( float ) );
	for( int i = 0; i < numRows; i++ ) {
		float sum = mPtr[0] * vPtr[0];
		for( int j = 1; j < numColumns; j++ ) {
			sum += mPtr[j] * vPtr[j];
		}
		temp[i] = dstPtr[i] + sum;
		mPtr += numColumns;
	}
	for( int i = 0; i < numRows; i++ ) {
		dstPtr[i] = temp[i];
	}
}

ID_INLINE void idMatX::MultiplySub( idVecX& dst, const idVecX& vec ) const
{
	assert( dst.GetSize() == numRows );
	const float* mPtr	= mat;
	const float* vPtr	= vec.ToFloatPtr();
	float*		 dstPtr = dst.ToFloatPtr();
	float*		 temp	= ( float* )_alloca16( numRows * sizeof( float ) );
	for( int i = 0; i < numRows; i++ ) {
		float sum = mPtr[0] * vPtr[0];
		for( int j = 1; j < numColumns; j++ ) {
			sum += mPtr[j] * vPtr[j];
		}
		temp[i] = dstPtr[i] - sum;
		mPtr += numColumns;
	}
	for( int i = 0; i < numRows; i++ ) {
		dstPtr[i] = temp[i];
	}
}

ID_INLINE void idMatX::TransposeMultiply( idVecX& dst, const idVecX& vec ) const
{
	dst.SetSize( numColumns );
	const float* vPtr	= vec.ToFloatPtr();
	float*		 dstPtr = dst.ToFloatPtr();
	float*		 temp	= ( float* )_alloca16( numColumns * sizeof( float ) );
	for( int i = 0; i < numColumns; i++ ) {
		const float* mPtr = mat + i;
		float		 sum  = mPtr[0] * vPtr[0];
		for( int j = 1; j < numRows; j++ ) {
			mPtr += numColumns;
			sum += mPtr[0] * vPtr[j];
		}
		temp[i] = sum;
	}
	for( int i = 0; i < numColumns; i++ ) {
		dstPtr[i] = temp[i];
	}
}

ID_INLINE void idMatX::TransposeMultiplyAdd( idVecX& dst, const idVecX& vec ) const
{
	assert( dst.GetSize() == numColumns );
	const float* vPtr	= vec.ToFloatPtr();
	float*		 dstPtr = dst.ToFloatPtr();
	float*		 temp	= ( float* )_alloca16( numColumns * sizeof( float ) );
	for( int i = 0; i < numColumns; i++ ) {
		const float* mPtr = mat + i;
		float		 sum  = mPtr[0] * vPtr[0];
		for( int j = 1; j < numRows; j++ ) {
			mPtr += numColumns;
			sum += mPtr[0] * vPtr[j];
		}
		temp[i] = dstPtr[i] + sum;
	}
	for( int i = 0; i < numColumns; i++ ) {
		dstPtr[i] = temp[i];
	}
}

ID_INLINE void idMatX::TransposeMultiplySub( idVecX& dst, const idVecX& vec ) const
{
	assert( dst.GetSize() == numColumns );
	const float* vPtr	= vec.ToFloatPtr();
	float*		 dstPtr = dst.ToFloatPtr();
	float*		 temp	= ( float* )_alloca16( numColumns * sizeof( float ) );
	for( int i = 0; i < numColumns; i++ ) {
		const float* mPtr = mat + i;
		float		 sum  = mPtr[0] * vPtr[0];
		for( int j = 1; j < numRows; j++ ) {
			mPtr += numColumns;
			sum += mPtr[0] * vPtr[j];
		}
		temp[i] = dstPtr[i] - sum;
	}
	for( int i = 0; i < numColumns; i++ ) {
		dstPtr[i] = temp[i];
	}
}

ID_INLINE void idMatX::Multiply( idMatX& dst, const idMatX& a ) const
{
	assert( numColumns == a.numRows );
	assert( &dst != &a && &dst != this );

	dst.SetSize( numRows, a.numColumns );
	float*		 dstPtr = dst.ToFloatPtr();
	const float* m1Ptr	= ToFloatPtr();
	int			 k		= numRows;
	int			 l		= a.GetNumColumns();
	for( int i = 0; i < k; i++ ) {
		for( int j = 0; j < l; j++ ) {
			const float* m2Ptr = a.ToFloatPtr() + j;
			float		 sum   = m1Ptr[0] * m2Ptr[0];
			for( int n = 1; n < numColumns; n++ ) {
				m2Ptr += l;
				sum += m1Ptr[n] * m2Ptr[0];
			}
			*dstPtr++ = sum;
		}
		m1Ptr += numColumns;
	}
}

ID_INLINE void idMatX::TransposeMultiply( idMatX& dst, const idMatX& a ) const
{
	assert( numRows == a.numRows );
	assert( &dst != &a && &dst != this );

	dst.SetSize( numColumns, a.numColumns );
	float* dstPtr = dst.ToFloatPtr();
	int	   k	  = numColumns;
	int	   l	  = a.numColumns;
	for( int i = 0; i < k; i++ ) {
		for( int j = 0; j < l; j++ ) {
			const float* m1Ptr = ToFloatPtr() + i;
			const float* m2Ptr = a.ToFloatPtr() + j;
			float		 sum   = m1Ptr[0] * m2Ptr[0];
			for( int n = 1; n < numRows; n++ ) {
				m1Ptr += numColumns;
				m2Ptr += a.numColumns;
				sum += m1Ptr[0] * m2Ptr[0];
			}
			*dstPtr++ = sum;
		}
	}
}

ID_INLINE int idMatX::GetDimension() const
{
	return numRows * numColumns;
}

ID_INLINE const idVec6& idMatX::SubVec6( int row ) const
{
	assert( numColumns >= 6 && row >= 0 && row < numRows );
	return *reinterpret_cast<const idVec6*>( mat + row * numColumns );
}

ID_INLINE idVec6& idMatX::SubVec6( int row )
{
	assert( numColumns >= 6 && row >= 0 && row < numRows );
	return *reinterpret_cast<idVec6*>( mat + row * numColumns );
}

ID_INLINE const idVecX idMatX::SubVecX( int row ) const
{
	idVecX v;
	assert( row >= 0 && row < numRows );
	v.SetData( numColumns, mat + row * numColumns );
	return v;
}

ID_INLINE idVecX idMatX::SubVecX( int row )
{
	idVecX v;
	assert( row >= 0 && row < numRows );
	v.SetData( numColumns, mat + row * numColumns );
	return v;
}

ID_INLINE const float* idMatX::ToFloatPtr() const
{
	return mat;
}

ID_INLINE float* idMatX::ToFloatPtr()
{
	return mat;
}

#endif // !__MATH_MATRIXX_H__
