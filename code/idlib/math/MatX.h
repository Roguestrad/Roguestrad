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

class idMatX
{
public:
	/*!
		\brief Initializes an idMatX object with zero rows and columns.

		Constructs a new matrix with no allocated memory and zero row and column dimensions. The matrix is initialized to an empty state and can be used as a placeholder or default value before being
	   assigned actual data.

		\return ID_INLINE
	*/
	ID_INLINE idMatX();

	/*!
		\brief Constructs a new matrix as a copy of the given matrix.

		The constructor initializes a new matrix by copying the dimensions and values from another matrix object. It first resets the current matrix state by setting the row count, column count, and
	   allocated size to zero, and the matrix data pointer to NULL. Then it uses the Set method to properly initialize the matrix with the same dimensions and data as the provided matrix.

		\param other The matrix to copy from
	*/
	ID_INLINE idMatX( const idMatX& other );

	/*!
		\brief Initializes a new matrix with the specified number of rows and columns

		Constructs a new matrix object with the given dimensions. The matrix is initially empty and will allocate memory based on the specified row and column counts. The actual matrix data is
	   initialized through the SetSize method which handles the memory allocation and setup.

		\param rows The number of rows for the new matrix
		\param columns The number of columns for the new matrix
		\return ID_INLINE
	*/
	ID_INLINE explicit idMatX( int rows, int columns );

	/*!
		\brief Initializes a matrix with the specified number of rows and columns using the provided data

		This constructor initializes a matrix object with the given dimensions and populates it with data from the provided float array. The matrix will allocate memory for the data and set up
	   internal bookkeeping for the matrix dimensions. The src parameter should point to a contiguous block of memory containing the matrix elements in row-major order

		\param rows Number of rows in the matrix
		\param columns Number of columns in the matrix
		\param src Pointer to the source data array containing matrix elements in row-major order
	*/
	ID_INLINE explicit idMatX( int rows, int columns, float* src );

	/*!
		\brief Destructor for the idMatX class that frees allocated memory if it's not temporary memory.

		The destructor checks if the matrix data was allocated and is not part of temporary memory. If these conditions are met, it frees the allocated memory using Mem_Free16. Temporary memory is
	   identified by checking if the matrix pointer falls within the designated temporary memory range.

		\return ID_INLINE
	*/
	ID_INLINE ~idMatX();

	/*!
		\brief Initializes the matrix with specified dimensions and copies data from the source array.

		Sets the matrix size to the specified number of rows and columns, then copies the data from the provided source array into the matrix. The source array is expected to contain row-major ordered
	   float values.

		\param rows Number of rows in the matrix
		\param columns Number of columns in the matrix
		\param src Pointer to the source array containing float values
	*/
	ID_INLINE void			 Set( int rows, int columns, const float* src );

	/*!
		\brief Sets the matrix data from two 3x3 matrices arranged in a 2x2 block pattern.

		This function initializes a matrix with dimensions 3x6 and populates it by placing the first 3x3 matrix in the top-left block and the second 3x3 matrix in the top-right block. The resulting
	   matrix is structured as a 2x2 block arrangement where each block is a 3x3 matrix.

		\param m1 The first 3x3 matrix to be placed in the top-left block of the resulting matrix
		\param m2 The second 3x3 matrix to be placed in the top-right block of the resulting matrix
	*/
	ID_INLINE void			 Set( const idMat3& m1, const idMat3& m2 );

	/*!
		\brief Sets the current matrix to a 6x6 matrix composed of four 3x3 matrices arranged in a 2x2 block pattern.

		This function initializes a 6x6 matrix by arranging four 3x3 matrices in a block format. The first matrix m1 occupies the top-left block, m2 the top-right, m3 the bottom-left, and m4 the
	   bottom-right. The matrix is resized to 6x6 dimensions before population.

		\param m1 First 3x3 matrix to place in the top-left block of the 6x6 matrix
		\param m2 Second 3x3 matrix to place in the top-right block of the 6x6 matrix
		\param m3 Third 3x3 matrix to place in the bottom-left block of the 6x6 matrix
		\param m4 Fourth 3x3 matrix to place in the bottom-right block of the 6x6 matrix
	*/
	ID_INLINE void			 Set( const idMat3& m1, const idMat3& m2, const idMat3& m3, const idMat3& m4 );

	ID_INLINE const float*	 operator[]( int index ) const;
	ID_INLINE float*		 operator[]( int index );
	ID_INLINE idMatX&		 operator=( const idMatX& a );
	ID_INLINE idMatX		 operator*( const float a ) const;
	ID_INLINE idVecX		 operator*( const idVecX& vec ) const;
	ID_INLINE idMatX		 operator*( const idMatX& a ) const;
	ID_INLINE idMatX		 operator+( const idMatX& a ) const;
	ID_INLINE idMatX		 operator-( const idMatX& a ) const;
	ID_INLINE idMatX&		 operator*=( const float a );
	ID_INLINE idMatX&		 operator*=( const idMatX& a );
	ID_INLINE idMatX&		 operator+=( const idMatX& a );
	ID_INLINE idMatX&		 operator-=( const idMatX& a );

	friend ID_INLINE idMatX	 operator*( const float a, const idMatX& m );
	friend ID_INLINE idVecX	 operator*( const idVecX& vec, const idMatX& m );
	friend ID_INLINE idVecX& operator*=( idVecX& vec, const idMatX& m );

	/*!
		\brief Compares this matrix with another matrix for equality.

		This function performs an element-wise comparison between the current matrix and the provided matrix to determine if they are equal. It first asserts that both matrices have the same
	   dimensions before proceeding with the comparison. The function iterates through all elements of the matrices and returns false if any pair of elements differ. If all elements are equal, it
	   returns true.

		\param a The matrix to compare with this matrix
		\return True if the matrices are equal, false otherwise
		\throws assertion failure if the matrices have different dimensions
	*/
	ID_INLINE bool			 Compare( const idMatX& a ) const;

	/*!
		\brief Compares this matrix with another matrix using the specified epsilon tolerance.

		This function performs an element-wise comparison between the current matrix and the provided matrix 'a'. It returns true if all corresponding elements differ by less than the specified
	   epsilon value, indicating that the matrices are nearly equal within the given tolerance. The function asserts that both matrices have the same dimensions before performing the comparison.

		\param a The matrix to compare against
		\param epsilon The tolerance value for floating-point comparison
		\return True if all matrix elements are within the epsilon tolerance of each other, false otherwise
		\throws Assertion error if the matrices have different dimensions
	*/
	ID_INLINE bool			 Compare( const idMatX& a, const float epsilon ) const;
	ID_INLINE bool			 operator==( const idMatX& a ) const; // exact compare, no epsilon
	ID_INLINE bool			 operator!=( const idMatX& a ) const; // exact compare, no epsilon

	/*!
		\brief Sets the number of rows and columns for the matrix.

		Resizes the matrix to the specified number of rows and columns while preserving existing data when possible. This function changes the dimensions of the matrix to the specified number of rows
	   and columns. If the new size requires more memory allocation, it allocates new memory and copies the existing data to the new location. When the matrix is made smaller, data is truncated. When
	   the matrix is made larger, the new space is initialized to zero. The function preserves existing data as much as possible during the resizing operation.

		\param rows new number of rows for the matrix
		\param columns new number of columns for the matrix
	*/
	ID_INLINE void			 SetSize( int rows, int columns );

	/*!
		\brief Resizes the matrix to the specified number of rows and columns while preserving existing data when possible

		This function changes the dimensions of the matrix to the specified number of rows and columns. If the new size requires more memory allocation, it allocates new memory and copies the existing
	   data to the new location. When the matrix is made smaller, data is truncated. When the matrix is made larger, the new space can be optionally initialized to zero. The function preserves
	   existing data as much as possible during the resizing operation.

		\param rows new number of rows for the matrix
		\param columns new number of columns for the matrix
		\param makeZero if true, the newly allocated memory is initialized to zero
	*/
	void					 ChangeSize( int rows, int columns, bool makeZero = false );

	/*!
		\brief Changes the number of rows in the matrix to the specified value

		This function modifies the number of rows in the matrix while preserving the current number of columns. It calls ChangeSize internally to perform the actual resizing operation.

		\param rows the new number of rows for the matrix
	*/
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

	/*!
		\brief Sets the data pointer for the matrix and updates its dimensions while ensuring 16-byte alignment

		This function configures the matrix by assigning the provided data pointer and updating the row and column counts. It also validates that the data pointer is properly aligned to a 16-byte
	   boundary, which is required for memory access optimization. The function ensures that any previously allocated memory is freed if it exists, and sets the allocation flag to indicate that the
	   matrix is using externally provided memory rather than internally allocated storage.

		\param rows The number of rows in the matrix
		\param columns The number of columns in the matrix
		\param data Pointer to the float array containing the matrix data
		\throws assertion failure if the data pointer is not 16-byte aligned or if the matrix pointer is within the temporary memory range
	*/
	ID_INLINE void			SetData( int rows, int columns, float* data );

	/*!
		\brief Sets the data pointer and configuration for the matrix, ensuring 128-byte alignment and optionally clearing the matrix.

		This function configures the matrix with the provided data pointer and dimensions, ensuring that the data pointer is 128-byte aligned. If the clear flag is true, it clears the matrix by
	   setting all elements to zero. Otherwise, it clears the end of the matrix data.

		\param rows The number of rows in the matrix
		\param columns The number of columns in the matrix
		\param data Pointer to the matrix data, must be 128-byte aligned
		\param clear If true, clears the matrix by setting all elements to zero
		\throws assert failure if the data pointer is not 128-byte aligned
	*/
	ID_INLINE void			SetDataCacheLines( int rows, int columns, float* data, bool clear );

	/*!
		\brief Sets all elements of the matrix to zero.

		This function clears the matrix by setting all its elements to zero. It uses SIMD instructions when available to optimize the zeroing process, otherwise falling back to a standard memory set
	   operation. The function operates on the current matrix dimensions defined by numRows and numColumns member variables.

	*/
	ID_INLINE void			Zero();

	/*!
		\brief Sets the matrix size and clears all elements to zero.

		This function configures the matrix to have the specified number of rows and columns, and then initializes all elements in the matrix to zero. It first calls SetSize to adjust the matrix
	   dimensions and then uses the Zero() method to clear all elements.

		\param rows the number of rows for the matrix
		\param columns the number of columns for the matrix
	*/
	ID_INLINE void			Zero( int rows, int columns );

	/*!
		\brief Sets the matrix to an identity matrix.

		Initializes the matrix as an identity matrix by first zeroing all elements and then setting the diagonal elements to 1.0f. The matrix must be square for this operation to be valid.

		\throws assertion failure if the matrix is not square
	*/
	ID_INLINE void			Identity();

	/*!
		\brief Sets the matrix size and initializes it as an identity matrix.

		This function configures the matrix to the specified number of rows and columns, ensuring they are equal, and then initializes the matrix as an identity matrix. It asserts that the number of
	   rows equals the number of columns to maintain the square matrix property required for an identity matrix.

		\param rows The number of rows for the identity matrix
		\param columns The number of columns for the identity matrix
		\throws assertion failure if rows does not equal columns
	*/
	ID_INLINE void			Identity( int rows, int columns );

	/*!
		\brief Creates a diagonal matrix from the given vector by placing the vector elements on the diagonal.

		The function initializes a square matrix with zeros and then sets the diagonal elements to the corresponding values from the input vector. The size of the matrix matches the size of the input
	   vector.

		\param v The input vector containing diagonal elements for the matrix
	*/
	ID_INLINE void			Diag( const idVecX& v );

	/*!
		\brief Fills the matrix with random values using the provided seed and range.

		This function initializes all elements of the matrix with random floating-point values within a specified range. The random number generator is seeded using the provided integer seed to ensure
	   reproducible results. The lower and upper bounds of the range are defined by the l and u parameters respectively.

		\param seed The seed value for the random number generator
		\param l The lower bound of the random value range
		\param u The upper bound of the random value range
	*/
	ID_INLINE void			Random( int seed, float l = 0.0f, float u = 1.0f );

	/*!
		\brief Fills the matrix with random values in the specified range using the given seed.

		This function initializes a matrix with random floating-point values within a specified range. It uses a random number generator seeded with the provided value to ensure reproducible results.
	   The matrix is resized to the specified dimensions before filling.

		\param rows Number of rows in the matrix
		\param columns Number of columns in the matrix
		\param seed Seed value for the random number generator
		\param l Lower bound of the random value range
		\param u Upper bound of the random value range
		\return No return value
	*/
	ID_INLINE void			Random( int rows, int columns, int seed, float l = 0.0f, float u = 1.0f );

	/*!
		\brief Negates all elements of the matrix in place.

		This function performs in-place negation of all matrix elements. It uses SIMD optimizations when available to improve performance. The function iterates through all elements of the matrix and
	   applies the negation operation. When SIMD is enabled, it processes four elements at a time using SSE instructions. When SIMD is not enabled, it falls back to a simple loop that negates each
	   element individually. The operation modifies the original matrix data directly without creating a new matrix.

	*/
	ID_INLINE void			Negate();

	/*!
		\brief Clamps all matrix values to the specified range.

		This function iterates through all elements of the matrix and adjusts any values that fall outside the specified minimum and maximum range. Values less than the minimum are set to the minimum,
	   and values greater than the maximum are set to the maximum. The operation modifies the matrix in place.

		\param min The minimum value to clamp matrix elements to
		\param max The maximum value to clamp matrix elements to
		\return void
	*/
	ID_INLINE void			Clamp( float min, float max );

	/*!
		\brief Swaps two rows in the matrix.

		This function exchanges the elements of two specified rows in the matrix. It takes two row indices as parameters and swaps all corresponding elements between the two rows. The operation
	   modifies the matrix in place and returns a reference to the matrix itself.

		\param r1 Index of the first row to swap
		\param r2 Index of the second row to swap
		\return A reference to the matrix itself after the rows have been swapped
	*/
	ID_INLINE idMatX&		SwapRows( int r1, int r2 );

	/*!
		\brief Swaps two columns in the matrix.

		This function exchanges the values in two specified columns of the matrix. The columns are identified by their indices r1 and r2. The operation is performed in-place, modifying the matrix
	   directly. The function iterates through each row of the matrix and swaps the corresponding elements in the two columns.

		\param r1 index of the first column to swap
		\param r2 index of the second column to swap
		\return A reference to the modified matrix after the columns have been swapped
	*/
	ID_INLINE idMatX&		SwapColumns( int r1, int r2 );

	/*!
		\brief Swaps rows and columns at the specified indices in the matrix and returns a reference to the modified matrix.

		This function performs a simultaneous swap of rows and columns at the given indices within the matrix. It utilizes the existing SwapRows and SwapColumns methods to achieve this operation. The
	   function is designed to be inlined for performance optimization and returns a reference to the modified matrix to allow for method chaining.

		\param r1 First row and column index to swap
		\param r2 Second row and column index to swap
		\return Reference to the modified matrix after swapping rows and columns at the specified indices
	*/
	ID_INLINE idMatX&		SwapRowsColumns( int r1, int r2 );

	/*!
		\brief Removes the specified row from the matrix and returns a reference to the modified matrix.

		This function removes a row at the specified index from the matrix by shifting all subsequent rows upward to fill the gap. The matrix dimensions are adjusted accordingly, with the number of
	   rows decreased by one. The operation modifies the matrix in-place and returns a reference to allow for method chaining.

		\param r the index of the row to be removed
		\return a reference to the modified matrix after the row has been removed
		\throws assertion failure if the row index is out of bounds
	*/
	idMatX&					RemoveRow( int r );

	/*!
		\brief Removes the specified column from the matrix and returns a reference to the modified matrix.

		This function removes a column at the specified index from the matrix by shifting all subsequent columns to the left. The matrix is modified in-place and the function returns a reference to
	   the modified matrix. The operation is performed efficiently using memory moves to minimize data copying. The function asserts that the specified column index is valid.

		\param r the index of the column to be removed
		\return a reference to the modified matrix after the column has been removed
		\throws assertion failure if the column index is out of bounds
	*/
	idMatX&					RemoveColumn( int r );

	/*!
		\brief Removes a specified row and column from the matrix and returns a reference to the modified matrix

		This function removes the row and column specified by the index r from the matrix. It updates the matrix dimensions and shifts the remaining elements to fill the gap left by the removed row
	   and column. The function modifies the matrix in place and returns a reference to the modified matrix. The operation assumes that the row and column indices are valid and within the bounds of
	   the matrix dimensions. The function uses memory operations to efficiently shift elements and maintains the matrix structure after the removal.

		\param r The index of the row and column to be removed from the matrix
		\return A reference to the modified matrix after removing the specified row and column
		\throws assertion failure if the row index r is greater than or equal to the number of rows or columns
	*/
	idMatX&					RemoveRowColumn( int r );

	/*!
		\brief Clears the upper triangle of the matrix by setting all elements above the main diagonal to zero.

		This function modifies the matrix in-place by clearing all elements in the upper triangle, which consists of all elements above the main diagonal. The main diagonal elements remain unchanged.
	   The function assumes that the matrix is square, as indicated by the assertion checking that the number of rows equals the number of columns. It iterates from the second-to-last row upwards,
	   clearing the appropriate number of elements in each row.

		\throws assertion failure if the matrix is not square
	*/
	ID_INLINE void			ClearUpperTriangle();

	/*!
		\brief Clears the lower triangle of the matrix by setting all elements below the diagonal to zero.

		This function modifies the matrix in place by setting all elements below the main diagonal to zero. It assumes the matrix is square since it checks for equal number of rows and columns. The
	   function iterates through the rows starting from the second row and clears the corresponding elements in the lower triangle. The diagonal elements remain unchanged.

		\throws assertion failure if the matrix is not square
	*/
	ID_INLINE void			ClearLowerTriangle();

	/*!
		\brief Copies the lower triangle of a matrix to its upper triangle

		This function performs an in-place operation to fill the upper triangle of a matrix with values from its lower triangle. It uses SIMD optimizations when available to improve performance. The
	   function assumes the matrix is square or has more columns than rows, and it asserts that the number of columns is divisible by 4.

		\throws assertion failure if column count is not divisible by 4 or if rows exceed columns
	*/
	void					CopyLowerToUpperTriangle();

	/*!
		\brief Copies a square sub-matrix of the specified size from the input matrix to this matrix.

		This function extracts a square sub-matrix of the given size from the input matrix m and copies it to the current matrix. The sub-matrix is taken from the top-left corner of the input matrix.
	   The function asserts that the specified size does not exceed the dimensions of the input matrix. The current matrix is resized to match the specified size before copying.

		\param m The input matrix from which the sub-matrix is copied
		\param size The size of the square sub-matrix to copy
		\throws assertion failure if size exceeds the number of rows or columns in the input matrix
	*/
	ID_INLINE void			SquareSubMatrix( const idMatX& m, int size );

	/*!
		\brief Returns the maximum absolute difference between corresponding elements of this matrix and the provided matrix

		This function compares each element of the current matrix with the corresponding element of the provided matrix and returns the largest absolute difference found. The function asserts that
	   both matrices have the same dimensions before performing the comparison. If the matrices have different dimensions, the assertion will fail.

		\param m The matrix to compare against this matrix
		\return The maximum absolute difference found between corresponding elements of the two matrices
		\throws Assertion failure if the matrices have different dimensions
	*/
	ID_INLINE float			MaxDifference( const idMatX& m ) const;

	//! Returns true if the matrix is square, false otherwise.
	ID_INLINE bool			IsSquare() const { return ( numRows == numColumns ); }

	/*!
		\brief Returns true if all elements of the matrix are zero within the specified epsilon tolerance.

		This function checks whether the matrix is effectively zero by comparing each element against the provided epsilon threshold. If any element has an absolute value greater than epsilon, the
	   function returns false. Otherwise, it returns true, indicating the matrix is effectively zero.

		\param epsilon The tolerance value used to determine if matrix elements are effectively zero
		\return True if all elements of the matrix are zero within the specified epsilon tolerance, false otherwise
	*/
	ID_INLINE bool			IsZero( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is an identity matrix within the given epsilon tolerance.

		This function verifies whether the current matrix is an identity matrix by comparing each element against the expected identity matrix values. It returns true if all diagonal elements are
	   approximately 1.0 and all non-diagonal elements are approximately 0.0, within the specified epsilon tolerance. The function assumes the matrix is square, as indicated by the assertion checking
	   that the number of rows equals the number of columns.

		\param epsilon Tolerance value for floating-point comparisons
		\return True if the matrix is an identity matrix within the given epsilon tolerance, false otherwise
		\throws Assertion failure if the matrix is not square
	*/
	ID_INLINE bool			IsIdentity( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is diagonal, meaning all non-diagonal elements are zero within the given epsilon tolerance

		A matrix is considered diagonal if all elements outside the main diagonal are zero. This function compares each off-diagonal element against the provided epsilon tolerance to determine if it
	   should be treated as zero. The function assumes the matrix is square, as indicated by the assert statement checking that the number of rows equals the number of columns

		\param epsilon tolerance value for considering an element as zero
		\return true if the matrix is diagonal within the specified epsilon tolerance, false otherwise
		\throws assertion failure if the matrix is not square
	*/
	ID_INLINE bool			IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is tri-diagonal within the given epsilon tolerance

		Returns true if all elements are zero except for the elements on the diagonal plus or minus one column. The matrix must be square for this check to proceed. The function compares each element
	   against the provided epsilon tolerance to determine if it should be considered zero.

		\param epsilon The tolerance value used to determine if matrix elements are effectively zero
		\return True if the matrix is tri-diagonal within the given epsilon tolerance, false otherwise
	*/
	ID_INLINE bool			IsTriDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric within a given epsilon tolerance.

		This function determines if a matrix is symmetric by comparing each element with its corresponding transposed element. The matrix is considered symmetric if all elements satisfy the condition
	   mat[i][j] == mat[j][i] within the specified epsilon tolerance. The function first checks if the matrix is square, and if not, immediately returns false. It then compares each element with its
	   transpose, returning false as soon as any mismatch exceeding the epsilon tolerance is found.

		\param epsilon tolerance value for floating point comparison
		\return true if the matrix is symmetric within the given epsilon tolerance, false otherwise
	*/
	ID_INLINE bool			IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is orthogonal by verifying that its columns form orthonormal vectors within the specified epsilon tolerance.

		This function determines whether the current matrix is orthogonal by checking if its columns form a set of orthonormal vectors. It first verifies that the matrix is square, as only square
	   matrices can be orthogonal. For each column vector, it computes the dot product with every other column vector, including the dot product of the vector with itself. The result of these dot
	   products should equal 1 for the same vector and 0 for different vectors, with variations allowed within the provided epsilon tolerance. If any deviation exceeds the tolerance, the function
	   returns false. Otherwise, it returns true.

		\param epsilon tolerance value for determining if dot products are close enough to 1 or 0 for orthonormality
		\return true if the matrix is orthogonal within the given epsilon tolerance, false otherwise
	*/
	bool					IsOrthogonal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is orthonormal within the given epsilon tolerance.

		This function verifies whether the matrix is orthonormal, meaning that its rows and columns are unit vectors and mutually orthogonal. It performs two checks: first, it ensures that the dot
	   product of each row with each column equals 1 if the row and column indices are equal, and 0 otherwise. Second, it verifies that each row and column has a magnitude of 1. The tolerance for
	   these checks is defined by the epsilon parameter.

		\param epsilon The tolerance value used for comparing floating-point numbers in the orthonormality checks
		\return True if the matrix is orthonormal within the specified epsilon tolerance, false otherwise
	*/
	bool					IsOrthonormal( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is a P-matrix, meaning all principal minors are positive

		A P-matrix is a square matrix where all principal minors have positive determinants. This function recursively verifies this property by checking if the leading principal minors are positive
	   and if the matrix remains a P-matrix after certain transformations. The function uses an epsilon value to determine the threshold for considering values as zero

		\param epsilon threshold for considering values as zero, used to determine if matrix elements are effectively zero
		\return true if the matrix is a P-matrix with all principal minors positive, false otherwise
	*/
	bool					IsPMatrix( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is a zero matrix using the specified epsilon threshold.

		This function determines whether all elements of the matrix are zero, with a tolerance defined by the epsilon parameter. It first verifies that the matrix is square, then iterates through each
	   element to check if it exceeds the epsilon value when the row and column indices are not equal. If any such element is found, the function returns false. Otherwise, it returns true, indicating
	   the matrix is a zero matrix within the specified tolerance.

		\param epsilon The threshold value used to determine if matrix elements are effectively zero
		\return true if the matrix is a zero matrix within the specified epsilon tolerance, false otherwise
	*/
	bool					IsZMatrix( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is positive definite using Gaussian elimination with the specified epsilon threshold

		This function determines if the matrix is positive definite by performing Gaussian elimination with pivot steps. It first verifies that the matrix is square, then copies the matrix and adds
	   its transpose to ensure symmetry. The function then applies Gaussian elimination to check if all diagonal elements remain positive within the given epsilon tolerance. If any diagonal element
	   becomes non-positive during the elimination process, the matrix is not positive definite. The function returns true if the matrix passes all positive definiteness checks, and false otherwise.

		\param epsilon tolerance value used for checking positive definiteness
		\return true if the matrix is positive definite, false otherwise
	*/
	bool					IsPositiveDefinite( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric positive definite using Cholesky factorization

		This function determines if the matrix is symmetric positive definite by first verifying that the matrix is symmetric with the given epsilon tolerance. If the matrix is symmetric, it then
	   attempts to perform Cholesky factorization on a copy of the matrix. The ability to perform Cholesky factorization is both necessary and sufficient for positive definiteness. If the
	   factorization succeeds, the matrix is positive definite; otherwise, it is not.

		\param epsilon tolerance value used for checking symmetry
		\return true if the matrix is symmetric positive definite, false otherwise
	*/
	bool					IsSymmetricPositiveDefinite( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is positive semi-definite using Gaussian elimination with the specified epsilon tolerance

		Determines whether the matrix is symmetric positive semi-definite by performing Gaussian elimination with pivot steps. The matrix must be square for the operation to proceed. The function adds
	   the matrix to its transpose to ensure symmetry before testing for positive semi-definiteness. It uses the provided epsilon value to determine the tolerance for floating-point comparisons. If
	   any diagonal element becomes negative during the process, or if the matrix has linearly dependent rows, the function returns false

		\param epsilon tolerance value for floating-point comparisons
		\return true if the matrix is positive semi-definite, false otherwise
	*/
	bool					IsPositiveSemiDefinite( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Checks if the matrix is symmetric positive semi-definite

		Returns true if the matrix is both symmetric and positive semi-definite. The function first verifies that the matrix is symmetric within the given epsilon tolerance, and then checks if it is
	   positive semi-definite. If either condition fails, the function returns false.

		\param epsilon tolerance for symmetric check, default is MATRIX_EPSILON
		\return true if the matrix is symmetric positive semi-definite, false otherwise
	*/
	bool					IsSymmetricPositiveSemiDefinite( const float epsilon = MATRIX_EPSILON ) const;

	/*!
		\brief Returns the trace of the matrix, which is the sum of the diagonal elements

		This function calculates the trace of a square matrix by summing up all the diagonal elements. It asserts that the matrix is square before performing the calculation. The trace is a
	   fundamental property of square matrices and is used in various mathematical and computational applications.

		\return The sum of the diagonal elements of the matrix as a floating-point value
		\throws assertion failure if the matrix is not square
	*/
	ID_INLINE float			Trace() const;

	/*!
		\brief Returns the determinant of the matrix.

		This function calculates the determinant of a square matrix by using specialized implementations for small matrices (1x1 through 6x6) and a generic implementation for larger matrices. The
	   function asserts that the matrix is square before computation.

		\return The determinant of the matrix as a floating-point value.
		\throws assertion failure if the matrix is not square
	*/
	ID_INLINE float			Determinant() const;

	/*!
		\brief Returns the transpose of this matrix

		This function calculates and returns a new matrix that is the transpose of the current matrix. The transpose operation switches the rows and columns of the matrix, so that the element at
	   position (i, j) in the original matrix will be at position (j, i) in the transposed matrix. The function creates a new temporary matrix with dimensions swapped and populates it with the
	   transposed values.

		\return A new idMatX matrix that is the transpose of the current matrix
	*/
	ID_INLINE idMatX		Transpose() const;

	/*!
		\brief Transposes the matrix in place and returns a reference to itself.

		This function performs an in-place transposition of the matrix. It modifies the current matrix object by replacing its contents with the transpose of its original values. The operation is
	   performed by calling the Transpose() method and assigning the result back to the current object. The function then returns a reference to the modified matrix, allowing for method chaining.

		\return A reference to the transposed matrix
	*/
	ID_INLINE idMatX&		TransposeSelf();

	/*!
		\brief Transposes this matrix and stores the result in the provided destination matrix

		This function performs matrix transposition by copying the transposed version of the current matrix into the destination matrix provided as an argument. The transposition operation swaps the
	   rows and columns of the matrix. This is a common operation in linear algebra and is used in various graphics and collision detection calculations within the engine. The function is marked as
	   ID_INLINE, indicating it is intended to be inlined for performance reasons.

		\param dst The destination matrix where the transposed result will be stored
		\return void
	*/
	ID_INLINE void			Transpose( idMatX& dst ) const;

	/*!
		\brief Returns the inverse of the matrix

		Computes the inverse of the matrix in-place and returns true if successful, false if the matrix is singular

		\return A new matrix that is the inverse of the current matrix
	*/
	ID_INLINE idMatX		Inverse() const;

	/*!
		\brief Computes the inverse of the matrix in-place and returns true if successful, false if the matrix is singular.

		This function calculates the inverse of the matrix represented by the current object. It modifies the matrix in-place and returns a boolean indicating whether the inversion was successful. The
	   function handles special cases for small matrices (1x1 through 6x6) using optimized implementations, and falls back to a generic method for larger matrices. A singular matrix (one without an
	   inverse) will result in a false return value.

		\return true if the matrix was successfully inverted, false if the matrix is singular
	*/
	ID_INLINE bool			InverseSelf();

	/*!
		\brief Returns the inverse of the matrix by inverting it in-place and verifying the operation

		This function computes the inverse of the current matrix by first creating a temporary matrix with the same dimensions and copying the current matrix data into it. It then calls the in-place
	   inversion method on the temporary matrix and verifies that the inversion was successful. The function returns the inverted matrix

		\return A new matrix that is the inverse of the current matrix
	*/
	ID_INLINE idMatX		InverseFast() const;

	/*!
		\brief Inverts the matrix in-place and returns false if the determinant is zero

		This function computes the inverse of the matrix stored in the object and stores the result back in the same object. It performs the inversion in-place, modifying the original matrix data. The
	   function returns false if the matrix is singular (determinant is zero or very close to zero), indicating that the inversion could not be performed. For small matrices (1x1 through 6x6), it uses
	   specialized implementations for better performance. For larger matrices, it falls back to a generic implementation

		\return true if the matrix was successfully inverted, false if the matrix is singular (determinant is zero or very close to zero)
	*/
	ID_INLINE bool			InverseFastSelf();

	/*!
		\brief Computes the inverse of this matrix and stores it in the provided destination matrix

		This function calculates the inverse of the current matrix and stores the result in the destination matrix provided as an argument. It leverages the InverseFast method to perform the actual
	   computation. The function is marked as ID_INLINE, indicating it is intended to be inlined by the compiler for performance reasons.

		\param dst The destination matrix where the inverse will be stored
	*/
	ID_INLINE void			Inverse( idMatX& dst ) const;

	/*!
		\brief Computes the in-place inverse of a lower triangular matrix and returns false if the determinant is zero

		This function performs in-place inversion of a lower triangular matrix by utilizing a specific algorithm that processes the matrix from the first row to the last. It iterates through each
	   diagonal element, checking if it is zero and returning false if so. The function modifies the matrix in-place to store the inverse. The algorithm assumes the input matrix is lower triangular
	   with non-zero diagonal elements. If any diagonal element is zero, the function will return false indicating that the matrix is singular and cannot be inverted

		\return True if the matrix was successfully inverted, false if the determinant is zero
	*/
	bool					LowerTriangularInverse();

	/*!
		\brief Computes the in-place inverse of an upper triangular matrix and returns false if the determinant is zero

		This function performs in-place inversion of an upper triangular matrix by utilizing Gaussian elimination with backward substitution. It iterates through the matrix from the last row to the
	   first, computing the diagonal elements and off-diagonal elements. For each diagonal element, it checks if the value is zero, returning false if so. The function modifies the matrix in-place to
	   store the inverse. The algorithm assumes the input matrix is upper triangular with non-zero diagonal elements. If any diagonal element is zero, the function will return false indicating that
	   the matrix is singular and cannot be inverted.

		\return True if the matrix was successfully inverted, false if the determinant is zero
	*/
	bool					UpperTriangularInverse();

	/*!
		\brief Subtracts the given matrix from this matrix in-place.

		This function performs in-place subtraction of the matrix 'a' from the current matrix. It modifies the current matrix by subtracting each element of 'a' from the corresponding element of the
	   current matrix. The operation is equivalent to (*this) -= a.

		\param a The matrix to be subtracted from this matrix
	*/
	ID_INLINE void			Subtract( const idMatX& a );

	/*!
		\brief Multiplies this matrix with the given vector and returns the result.

		This function performs matrix-vector multiplication, computing the product of this matrix and the provided vector. It asserts that the number of columns in the matrix matches the size of the
	   input vector. The result is returned as a new idVecX object. The implementation internally allocates temporary memory for the result vector and then calls the Multiply method that takes a
	   pre-allocated vector as parameter.

		\param vec The vector to multiply with this matrix
		\return A new idVecX object containing the result of the matrix-vector multiplication
		\throws assertion failure if the matrix columns do not match the vector size
	*/
	ID_INLINE idVecX		Multiply( const idVecX& vec ) const;

	/*!
		\brief Computes the product of the transpose of this matrix and the given vector.

		This function calculates the matrix-vector product where the matrix is transposed before multiplication. It asserts that the number of rows in this matrix matches the size of the input vector
	   to ensure valid dimensions. The result is returned as a new idVecX object.

		\param vec The vector to multiply with the transpose of this matrix
		\return A new idVecX object containing the result of the matrix transpose multiplication
		\throws Assertion failure if the number of rows in this matrix does not match the size of the input vector
	*/
	ID_INLINE idVecX		TransposeMultiply( const idVecX& vec ) const;

	/*!
		\brief Returns the matrix product of this matrix and matrix a

		This function computes the matrix product of the current matrix and the input matrix a. It asserts that the number of columns in the current matrix matches the number of rows in matrix a to
	   ensure valid matrix multiplication. The result is returned as a new idMatX object.

		\param a The matrix to multiply with this matrix
		\return A new idMatX object containing the result of the matrix multiplication
		\throws assertion failure if the dimensions of the matrices are incompatible for multiplication
	*/
	ID_INLINE idMatX		Multiply( const idMatX& a ) const;

	/*!
		\brief Computes the matrix product of the transpose of this matrix and matrix a

		This function calculates the matrix product of the transpose of the current matrix and the provided matrix a. It asserts that the number of rows in both matrices match before performing the
	   operation. The result is stored in a newly created matrix with dimensions matching the transpose operation. The function internally uses a temporary matrix for computation and returns the
	   resulting matrix.

		\param a The matrix to multiply with the transpose of this matrix
		\return A new matrix containing the result of the transpose multiplication operation
		\throws assertion failure if the number of rows in this matrix does not match the number of rows in matrix a
	*/
	ID_INLINE idMatX		TransposeMultiply( const idMatX& a ) const;

	/*!
		\brief Performs matrix-vector multiplication and stores the result in the destination vector.

		This function multiplies the current matrix by the given vector and stores the result in the destination vector. It first ensures that the destination vector has the correct size based on the
	   number of rows in the matrix. The multiplication is performed by iterating through each row of the matrix and computing the dot product with the input vector. The intermediate results are
	   stored in a temporary buffer before being copied to the destination vector.

		\param dst The destination vector to store the result of the matrix-vector multiplication
		\param vec The input vector to be multiplied with the matrix
	*/
	ID_INLINE void			Multiply( idVecX& dst, const idVecX& vec ) const;

	/*!
		\brief Computes the matrix-vector product and adds it to the destination vector.

		This function performs matrix-vector multiplication where the result is added to the destination vector. It first calculates the matrix-vector product by multiplying each row of the matrix
	   with the input vector, then adds the result to the corresponding element of the destination vector. The operation is done in-place on the destination vector. The function uses a temporary
	   buffer to ensure correct computation before updating the destination vector.

		\param dst The destination vector that will be updated with the sum of the original vector and the matrix-vector product
		\param vec The input vector to be multiplied with this matrix
		\return NULL
		\throws assertion failure if the size of the destination vector does not match the number of rows in this matrix
	*/
	ID_INLINE void			MultiplyAdd( idVecX& dst, const idVecX& vec ) const;

	/*!
		\brief Subtracts the product of this matrix and the given vector from the destination vector.

		This function performs matrix-vector multiplication between this matrix and the provided vector, then subtracts the result from the destination vector. The operation is equivalent to dst = dst
	   - (this * vec). The function first computes the matrix-vector product, stores the intermediate results in a temporary buffer, and then updates the destination vector with the final computed
	   values. The function asserts that the destination vector has the correct size matching the number of rows in this matrix.

		\param dst The destination vector from which the matrix-vector product will be subtracted
		\param vec The vector to be multiplied with this matrix
		\throws assertion failure if the destination vector size does not match the number of rows in this matrix
	*/
	ID_INLINE void			MultiplySub( idVecX& dst, const idVecX& vec ) const;

	/*!
		\brief Performs matrix transpose multiplication of this matrix with the provided vector and stores the result in the destination vector.

		This function computes the product of the transpose of this matrix and the input vector, storing the result in the destination vector. The operation is equivalent to computing dst = this^T *
	   vec, where this^T represents the transpose of the matrix. The destination vector is resized to match the number of columns in this matrix. The implementation uses a temporary buffer for
	   intermediate calculations to ensure correctness and efficiency.

		\param dst The destination vector that will store the result of the transpose multiplication
		\param vec The input vector to be multiplied with the transpose of this matrix
	*/
	ID_INLINE void			TransposeMultiply( idVecX& dst, const idVecX& vec ) const;

	/*!
		\brief Computes the transpose of this matrix multiplied by the input vector and adds the result to the destination vector

		This function performs the operation dst = dst + (this^T * vec) where this^T is the transpose of the current matrix and vec is the input vector. The function first validates that the
	   destination vector has the correct size, then computes the matrix-vector product using the transpose, and finally adds the result to the destination vector. The computation is done in place
	   using a temporary buffer to avoid potential issues with aliasing.

		\param dst Destination vector that will be updated with the result of the operation
		\param vec Input vector to be multiplied with the transpose of this matrix
		\throws assertion failure if the destination vector size does not match the number of columns in the matrix
	*/
	ID_INLINE void			TransposeMultiplyAdd( idVecX& dst, const idVecX& vec ) const;

	/*!
		\brief Subtracts the product of the transposed matrix and a vector from a destination vector.

		This function performs the operation dst = dst - (transpose(this) * vec). It first computes the dot product of each row of the transposed matrix with the input vector, then subtracts the
	   result from the corresponding element in the destination vector. The function assumes that the destination vector has the same number of elements as the number of columns in the matrix.

		\param dst The destination vector from which the matrix-vector product will be subtracted
		\param vec The input vector to be multiplied with the transposed matrix
		\throws assertion failure if the size of the destination vector does not match the number of columns in the matrix
	*/
	ID_INLINE void			TransposeMultiplySub( idVecX& dst, const idVecX& vec ) const;

	/*!
		\brief Performs matrix multiplication of this matrix with matrix a and stores the result in dst

		This function computes the matrix product of the current matrix (this) with matrix a and stores the result in the destination matrix dst. The function asserts that the number of columns in
	   this matrix matches the number of rows in matrix a to ensure valid matrix multiplication. It also ensures that the destination matrix does not overlap with either of the operand matrices. The
	   result matrix dst is automatically resized to the appropriate dimensions based on the multiplication.

		\param dst The destination matrix where the result of the multiplication will be stored
		\param a The matrix to be multiplied with this matrix
		\throws assertion failure if the matrix dimensions are incompatible for multiplication or if dst overlaps with either this or a
	*/
	ID_INLINE void			Multiply( idMatX& dst, const idMatX& a ) const;

	/*!
		\brief Computes the matrix multiplication of the transposed current matrix with matrix a and stores the result in dst

		This function performs the operation dst = this^T * a where this^T is the transpose of the current matrix. The function asserts that the number of rows in the current matrix matches the number
	   of rows in matrix a. It also ensures that the destination matrix does not overlap with either the current matrix or matrix a. The result is stored in the destination matrix dst, which is
	   resized to have dimensions numColumns x a.numColumns.

		\param dst The matrix where the result of the multiplication will be stored
		\param a The second matrix in the multiplication operation
		\throws Assertion failures if numRows != a.numRows or if dst overlaps with this or a
	*/
	ID_INLINE void			TransposeMultiply( idMatX& dst, const idMatX& a ) const;

	//! Returns the total number of values in the matrix.
	ID_INLINE int			GetDimension() const;

	/*!
		\brief Returns a const reference to a idVec6 object interpreted from the beginning of the specified row

		The function provides access to a portion of the matrix data interpreted as a 6-dimensional vector. It assumes that the matrix has at least 6 columns and returns a reference to a idVec6 object
	   starting at the specified row. The returned reference is valid only as long as the matrix remains valid and unchanged.

		\param row The row index from which to interpret the idVec6 object
		\return A const reference to a idVec6 object representing the first 6 elements of the specified row in the matrix
		\throws asserts if the number of columns is less than 6 or if the row index is out of bounds
	*/
	ID_INLINE const idVec6& SubVec6( int row ) const;

	/*!
		\brief Returns a reference to a vector representing a row of the matrix interpreted as an idVec6

		This function provides access to a specific row of the matrix by interpreting it as an idVec6 vector. The function performs an assertion check to ensure that the matrix has at least 6 columns
	   and that the specified row index is valid. It then returns a reference to the idVec6 vector at the calculated memory location.

		\param row The index of the row to be accessed and interpreted as an idVec6
		\return A reference to an idVec6 vector representing the specified row of the matrix
		\throws assertion failure if numColumns is less than 6 or if row is out of bounds
	*/
	ID_INLINE idVec6&		SubVec6( int row );

	/*!
		\brief Returns a constant reference to a vector representing a row of the matrix.

		This function extracts a specific row from the matrix and returns it as a vector view. The returned vector references the same memory as the original matrix data, allowing for efficient access
	   without copying. The row index must be within valid bounds, as asserted by the implementation.

		\param row The index of the row to extract from the matrix.
		\return A constant reference to a vector that represents the specified row of the matrix.
		\throws assertion failure if the row index is out of bounds
	*/
	ID_INLINE const idVecX	SubVecX( int row ) const;

	/*!
		\brief Returns a vector view of a specified row in the matrix

		The function provides a vector view of a specific row from the matrix by creating a new idVecX object that references the data in the matrix. It asserts that the row index is valid before
	   proceeding. The returned vector shares the same memory as the original matrix data for the specified row.

		\param row the index of the row to be returned as a vector
		\return A vector view of the specified row in the matrix
		\throws assertion failure if the row index is out of bounds
	*/
	ID_INLINE idVecX		SubVecX( int row );

	//! Returns a pointer to the const float array representation of the matrix
	ID_INLINE const float*	ToFloatPtr() const;

	//! Returns a pointer to the float array representing the matrix data.
	ID_INLINE float*		ToFloatPtr();

	//! Returns a string representation of the matrix with the specified precision.
	const char*				ToString( int precision = 2 ) const;

	/*!
		\brief Updates the matrix with a rank-one symmetric update using the given vectors and scalar.

		This function performs a rank-one update on the matrix by modifying it with the outer product of vectors v and w scaled by alpha. The update follows the mathematical formula A = A + alpha * v
	   * w^T, where A is the matrix, v and w are the input vectors, and alpha is the scaling factor. The function ensures that the dimensions of the input vectors are adequate for the matrix
	   operations.

		\param v The first input vector used in the rank-one update
		\param w The second input vector used in the rank-one update
		\param alpha The scaling factor applied to the outer product of vectors v and w
		\throws assertion failure if vector v does not have enough elements or vector w does not have enough elements
	*/
	void					Update_RankOne( const idVecX& v, const idVecX& w, float alpha );

	/*!
		\brief Updates the matrix with a rank-one symmetric update using the provided vector and scalar value.

		This function performs a rank-one symmetric update on the matrix by modifying it with the formula A := A + alpha * v * v^T, where A is the current matrix, alpha is the scalar multiplier, and v
	   is the input vector. The update is applied in-place, modifying the matrix directly. The function assumes the matrix is square and the vector size is sufficient to match the matrix dimensions.
	   It is designed for symmetric matrices and updates all elements accordingly.

		\param v The input vector used in the rank-one update operation
		\param alpha The scalar value used to scale the vector in the update operation
		\throws assertion failure if the matrix is not square or if the vector size is insufficient
	*/
	void					Update_RankOneSymmetric( const idVecX& v, float alpha );

	/*!
		\brief Updates a specific row and column of the matrix using vectors v and w.

		This function modifies the matrix by updating a specific row and column based on the provided vectors. It adds the elements of vector v to the specified row and adds the elements of vector w
	   to the specified column. The function assumes that the element at index r in vector w is zero and performs assertions to verify this condition along with size checks for the vectors.

		\param v Vector used to update the specified row of the matrix
		\param w Vector used to update the specified column of the matrix
		\param r Index of the row and column to be updated
		\throws assertion failure if w[r] is not equal to 0.0f, or if v or w do not have sufficient size
	*/
	void					Update_RowColumn( const idVecX& v, const idVecX& w, int r );

	/*!
		\brief Updates a symmetric matrix row and column by adding values from a vector.

		This function modifies a symmetric matrix by updating the specified row and column with values from the provided vector. The operation is performed in-place, incrementally adding the vector
	   elements to the matrix. The matrix must be square for this operation to be valid. The function processes elements before and after the specified row index separately to maintain symmetry.

		\param v The vector containing values to add to the matrix row and column
		\param r The row and column index to update
		\throws assertion failure if the matrix is not square or if the vector size is insufficient
	*/
	void					Update_RowColumnSymmetric( const idVecX& v, int r );

	/*!
		\brief Updates the matrix by incrementally adding vectors v and w to expand its size and populate the last row and column.

		This function expands the matrix by one row and one column, then populates the last row with values from vector v and the last column with values from vector w. The function assumes the matrix
	   is square and performs assertions to verify the dimensions of the input vectors match the expected requirements. It modifies the matrix in place by changing its size and updating the relevant
	   elements.

		\param v vector used to populate the last column of the expanded matrix
		\param w vector used to populate the last row of the expanded matrix
		\throws assertion failure if numRows is not equal to numColumns, or if v or w do not have sufficient size
	*/
	void					Update_Increment( const idVecX& v, const idVecX& w );

	/*!
		\brief Updates the matrix by incrementally adding a new row and column based on the input vector.

		This function modifies the matrix by increasing its size by one row and one column. It populates the new row and column with values from the input vector. The function first verifies that the
	   matrix is square and that the input vector has sufficient elements. The new row is filled with values from the input vector, and the new column is also filled with values from the input vector.
	   This operation effectively extends the matrix while maintaining symmetry.

		\param v The input vector used to populate the new row and column of the matrix
		\throws _assertion failure if the matrix is not square or if the input vector size is insufficient
	*/
	void					Update_IncrementSymmetric( const idVecX& v );

	/*!
		\brief Removes the specified row and column from the matrix.

		This function removes a specified row and column from the matrix. It performs the removal by calling the RemoveRowColumn method with the provided row index.

		\param r the index of the row to be removed
	*/
	void					Update_Decrement( int r );

	/*!
		\brief Computes the inverse of the matrix using Gauss-Jordan elimination and returns true if successful.

		This function performs Gaussian-Jordan elimination with full pivoting to compute the inverse of a square matrix. It modifies the matrix in-place to store the inverse. The algorithm uses
	   partial pivoting to improve numerical stability and handles cases where the matrix is not invertible by returning false. The function assumes the matrix is square and uses a series of row
	   operations to transform the matrix into its inverse form.

		\return true if the matrix was successfully inverted, false if the matrix is not invertible
		\throws assertion failure if the matrix is not square
	*/
	bool					Inverse_GaussJordan();

	/*!
		\brief Updates the inverse matrix using rank-one update with vectors v and w and scalar alpha

		This function performs a rank-one update on an existing inverse matrix by modifying the matrix using two input vectors and a scalar value. It calculates intermediate vectors y and z, computes
	   a beta value, and checks if the update is valid. If the beta value is zero, the update fails and the function returns false. Otherwise, it proceeds with the update calculation and returns true
	   upon successful completion.

		\param v first input vector used in the rank-one update
		\param w second input vector used in the rank-one update
		\param alpha scalar value used in the rank-one update calculation
		\return true if the rank-one update succeeds, false if the update fails due to a zero beta value
		\throws assertion failure if matrix dimensions do not match the vector sizes
	*/
	bool					Inverse_UpdateRankOne( const idVecX& v, const idVecX& w, float alpha );

	/*!
		\brief Updates the inverse matrix by performing row and column operations using the provided vectors

		This function modifies an existing inverse matrix by applying two rank-one updates. It takes two vectors, v and w, and a row index r to perform the operations. The function first initializes a
	   vector s with a 1 at position r, then applies two inverse rank-one updates to the matrix. The first update uses vector v and vector s, and the second uses vector s and vector w. The function
	   returns false if either of the rank-one updates fails, indicating that the inverse could not be updated.

		\param v vector used in the first rank-one update
		\param w vector used in the second rank-one update
		\param r index of the row/column to be updated
		\return true if both rank-one updates succeed, false otherwise
		\throws assertion failure if dimensions do not match or if the row index is out of bounds
	*/
	bool					Inverse_UpdateRowColumn( const idVecX& v, const idVecX& w, int r );

	/*!
		\brief Updates the inverse matrix by incrementing its size and performing row/column operations.

		This function increments the size of the matrix by one row and one column, initializing the new row and column to form an identity block. It then performs an update operation on the matrix
	   inverse using the provided vectors v and w. The operation modifies the matrix to reflect the incremental change in the inverse.

		\param v A vector used to update the matrix inverse during the increment operation.
		\param w A vector used to update the matrix inverse during the increment operation.
		\return True if the update operation was successful, false otherwise.
		\throws assertion failure if the matrix is not square or if the vectors v and w do not have sufficient size.
	*/
	bool					Inverse_UpdateIncrement( const idVecX& v, const idVecX& w );

	/*!
		\brief Updates the matrix inverse by decrementing a row and column at the specified index

		This function modifies the matrix inverse by removing a row and column at the specified index. It first updates the row and column using the provided vectors v and w, then physically removes
	   the row and column from the matrix. The updates are performed using the Inverse_UpdateRowColumn function, and the actual removal is handled by Update_Decrement. The function returns true if the
	   operation succeeds, false otherwise. The vectors v and w must have sufficient size to cover the matrix dimensions, and the index r must be within valid bounds.

		\param v vector used to update the row of the matrix inverse
		\param w vector used to update the column of the matrix inverse
		\param r index of the row and column to decrement
		\return true if the update and decrement operations were successful, false otherwise
		\throws assertion failures if matrix dimensions or vector sizes are invalid, or if the index r is out of bounds
	*/
	bool					Inverse_UpdateDecrement( const idVecX& v, const idVecX& w, int r );

	/*!
		\brief Solves the linear system Ax = b using the matrix inverse.

		This function solves a linear system of equations where the matrix A is represented by the current object and the vector b is the right-hand side of the equation. The solution vector x is
	   computed by multiplying the matrix inverse with the vector b. The function performs in-place LU factorization of the matrix during the computation.

		\param x the solution vector that will be filled with the result of the linear system
		\param b the right-hand side vector of the linear system
	*/
	void					Inverse_Solve( idVecX& x, const idVecX& b ) const;

	/*!
		\brief Performs in-place LU factorization of the matrix using partial pivoting

		This function computes the LU decomposition of the matrix stored in this object, using partial pivoting to improve numerical stability. The decomposition is performed in-place, meaning the
	   original matrix data is modified to store the L and U factors. The diagonal elements of the U matrix and the permutation information are stored in the matrix itself, while the index array
	   tracks row permutations. The determinant can optionally be computed and returned if the det parameter is provided.

		\param index Pointer to an array that will store the row permutation indices
		\param det Pointer to a float where the determinant of the matrix will be stored, or NULL if not needed
		\return True if the LU factorization was successful, false if a zero pivot was encountered
	*/
	bool					LU_Factor( int* index, float* det = NULL );

	/*!
		\brief Performs a rank-one update on the LU decomposition of the matrix using the provided vectors and alpha value.

		This function updates the LU decomposition of a matrix by performing a rank-one update. It takes two vectors v and w, a scalar alpha, and an optional index array. The update modifies the
	   matrix in place by adjusting the diagonal elements and off-diagonal elements according to the rank-one update formula. The function handles the case where an index array is provided to specify
	   which elements of vector v to use. It returns true if the update was successful, or false if a zero diagonal element was encountered during the computation.

		\param v The first vector used in the rank-one update
		\param w The second vector used in the rank-one update
		\param alpha Scalar value multiplied with vector v before the update
		\param index Optional array specifying which elements of vector v to use, or NULL to use all elements
		\return true if the rank-one update was successful, false if a zero diagonal element was encountered during the computation
	*/
	bool					LU_UpdateRankOne( const idVecX& v, const idVecX& w, float alpha, int* index );

	/*!
		\brief Updates a row and column of the matrix using LU decomposition with given vectors and row index

		This function performs an incremental update of the LU decomposition by adding a row and column to the matrix. It takes two vectors v and w, and a row index r to update the existing LU
	   factorization. The function handles the necessary computations to maintain the decomposition properties after the update. The index parameter can be used to specify a permutation array for the
	   row updates. The function returns false if the update causes a zero pivot during the decomposition process, indicating that the matrix may be singular or nearly singular.

		\param v Vector used to update the row of the matrix
		\param w Vector used to update the column of the matrix
		\param r Index of the row to be updated
		\param index Optional array specifying row permutations for the update
		\return true if the update was successful, false if a zero pivot was encountered during the decomposition process
	*/
	bool					LU_UpdateRowColumn( const idVecX& v, const idVecX& w, int r, int* index );

	/*!
		\brief Updates the LU decomposition by incrementally adding a row and column to the matrix

		This function performs an incremental update to an existing LU decomposition by adding a new row and column to the matrix. It modifies the current matrix in-place to accommodate the new
	   dimensions and recalculates the appropriate LU factors. The function assumes that the matrix is square and handles the permutation index if provided.

		\param v The vector used to update the L matrix
		\param w The vector used to update the U matrix
		\param index Pointer to the permutation index array, or NULL if no permutation is used
		\return true if the update was successful
		\throws assertion failure if matrix dimensions are inconsistent or vectors are too small
	*/
	bool					LU_UpdateIncrement( const idVecX& v, const idVecX& w, int* index );

	/*!
		\brief Updates the LU decomposition by decrementing the matrix size after removing a row and column

		This function performs an update to an LU decomposition when a row and column are removed from the matrix. It handles the necessary computations to maintain the decomposition properties. The
	   function takes vectors v, w, and u, along with a row index r and an optional index array. When an index array is provided, it manages the permutation of rows and columns accordingly. The
	   function also updates the internal matrix data structure by decrementing its size. It returns false if the update process fails, and true upon successful completion.

		\param v Input vector used for updating the LU decomposition
		\param w Input vector used for updating the LU decomposition
		\param u Input vector used for updating the LU decomposition
		\param r Index of the row and column to be removed from the matrix
		\param index Optional array containing the permutation indices for rows and columns
		\return True if the LU decomposition update is successful, false otherwise
	*/
	bool					LU_UpdateDecrement( const idVecX& v, const idVecX& w, const idVecX& u, int r, int* index );

	/*!
		\brief Solves a linear system using LU decomposition with optional index array

		This function solves a linear system of equations Ax = b using LU decomposition. It takes the LU decomposition factors stored in the matrix and performs forward and backward substitution to
	   compute the solution vector x. The function supports an optional index array that can be used to reorder the rows during the forward substitution phase. The matrix must be square for this
	   operation to be valid, as asserted by the condition that the number of rows equals the number of columns.

		\param x The vector to store the solution of the linear system
		\param b The right-hand side vector of the linear system
		\param index Optional array of indices for row reordering during forward substitution
		\throws Assertion failure if the size of x does not match the number of columns or if the size of b does not match the number of rows
	*/
	void					LU_Solve( idVecX& x, const idVecX& b, const int* index ) const;

	/*!
		\brief Computes the inverse of the matrix using LU decomposition and stores the result in the provided matrix

		This function calculates the inverse of the matrix by utilizing LU decomposition. It takes the LU decomposition factors and unpacks them into separate lower and upper triangular matrices. The
	   result is stored in the provided matrix inv. The function requires the matrix to be square, as asserted by the numRows == numColumns condition.

		\param inv The matrix to store the computed inverse
		\param index An array of indices used during the LU decomposition process
		\throws assertion failure if the matrix is not square
	*/
	void					LU_Inverse( idMatX& inv, const int* index ) const;

	/*!
		\brief Unpacks the LU decomposition factors into separate lower and upper triangular matrices.

		This function decomposes the stored LU factorization of a matrix into two separate triangular matrices, L and U. The lower triangular matrix L has ones on its diagonal and contains the lower
	   triangular part of the original matrix, while the upper triangular matrix U contains the upper triangular part including the diagonal elements. The factorization is stored in the current matrix
	   object, and this function extracts those factors to the provided matrices L and U. The operation is commonly used in solving systems of linear equations where the matrix has been
	   pre-factorized.

		\param L The lower triangular matrix to be filled with the L factor of the LU decomposition.
		\param U The upper triangular matrix to be filled with the U factor of the LU decomposition.
	*/
	void					LU_UnpackFactors( idMatX& L, idMatX& U ) const;

	/*!
		\brief Multiplies the LU factors of this matrix with the given index array and stores the result in the provided matrix

		This function performs LU factorization multiplication using the stored LU factors and the provided index array. It sets the size of the output matrix to match the dimensions of the current
	   matrix and processes each row to compute the resulting matrix values. The index array is used to determine the row permutation for the output matrix. The function calculates the sum by adding
	   the corresponding element from the current matrix and the product of elements from the LU factors, handling special cases where the column index is less than the row index

		\param m The output matrix where the result of the LU multiplication will be stored
		\param index An array containing row permutation indices for the output matrix
	*/
	void					LU_MultiplyFactors( idMatX& m, const int* index ) const;

	/*!
		\brief Performs in-place QR factorization of a square matrix using Givens rotations and returns whether the matrix is non-singular

		This function factorizes the matrix in-place into an orthogonal matrix Q and an upper triangular matrix R using Givens rotations. The factorization is performed such that A = Q * R, where A is
	   the original matrix. The function modifies the matrix elements in place to store the R matrix, while the Q matrix is implicitly represented through the transformations applied. The vectors c
	   and d are used to store intermediate results during the factorization process. The function handles the case of singular matrices by setting appropriate flags and returning false when the
	   matrix cannot be decomposed into full rank components.

		\param c Vector to store intermediate results during the QR factorization process
		\param d Vector to store intermediate results during the QR factorization process
		\return True if the matrix is non-singular and the QR factorization was successful, false otherwise
	*/
	bool					QR_Factor( idVecX& c, idVecX& d );

	/*!
		\brief Performs a rank-one update on the QR decomposition of a matrix using the specified vectors and scalar

		This function updates the QR decomposition stored in matrix R by applying a rank-one modification using vectors v and w, along with a scalar alpha. The operation modifies the matrix R in place
	   to reflect the updated decomposition. The function handles the mathematical computation of the rank-one update through a series of Givens rotations and vector manipulations. It ensures
	   numerical stability by checking for zero elements and applying appropriate scaling factors during the rotation process.

		\param R The matrix containing the QR decomposition to be updated
		\param v The first vector used in the rank-one update calculation
		\param w The second vector used in the rank-one update calculation
		\param alpha The scalar value used to scale the update
		\return True if the rank-one update was successfully performed, false otherwise
	*/
	bool					QR_UpdateRankOne( idMatX& R, const idVecX& v, const idVecX& w, float alpha );

	/*!
		\brief Performs a rank-one update on the matrix R using vectors v and w for the specified row r.

		This function implements an incremental QR update on the matrix R by applying two rank-one updates. It uses the vectors v and w to modify the matrix R at the specified row r. The function
	   first initializes a vector s with a 1 at position r and then applies two QR rank-one updates: one with v and s, and another with s and w. The function returns false if either of the rank-one
	   updates fails, otherwise it returns true.

		\param R The matrix R to be updated
		\param v The vector v used in the first rank-one update
		\param w The vector w used in the second rank-one update
		\param r The row index for the update
		\return True if both rank-one updates succeed, false otherwise
	*/
	bool					QR_UpdateRowColumn( idMatX& R, const idVecX& v, const idVecX& w, int r );

	/*!
		\brief Performs an incremental QR update on the matrix R using vectors v and w.

		This function executes an incremental QR update operation on the matrix R by applying the given vectors v and w. It prepares updated versions of the input vectors to represent the row and
	   column changes, then performs the update using the QR_UpdateRowColumn function. The function returns true on successful completion, or false if the QR update fails.

		\param R Reference to the matrix R that will be updated
		\param v Vector v used for the incremental update
		\param w Vector w used for the incremental update
		\return True if the incremental QR update completed successfully, false otherwise
	*/
	bool					QR_UpdateIncrement( idMatX& R, const idVecX& v, const idVecX& w );

	/*!
		\brief Performs a decremental QR update on the matrix R using vectors v and w.

		This function executes a decremental QR update operation on the matrix R by applying the given vectors v and w. It first prepares updated versions of the input vectors to represent the row and
	   column changes, then performs the update using the QR_UpdateRowColumn function. After the update, it removes the specified row and column from both the matrix R and the current object. The
	   function returns true on successful completion, or false if the QR update fails.

		\param R Reference to the matrix R that will be updated
		\param v Vector v used for the decremental update
		\param w Vector w used for the decremental update
		\param r Index of the row and column to decrement
		\return True if the decremental QR update and subsequent row/column removal completed successfully, false otherwise
	*/
	bool					QR_UpdateDecrement( idMatX& R, const idVecX& v, const idVecX& w, int r );

	/*!
		\brief Solves a system of linear equations using QR decomposition with the provided vectors.

		This function solves a system of linear equations by applying QR decomposition. It takes the right-hand side vector b and uses the QR factors stored in the matrix to compute the solution
	   vector x. The vectors c and d contain the diagonal elements of the R matrix and the scaled Householder vectors respectively. The computation involves applying the transformation matrix Q
	   transpose to the vector b, followed by back substitution using the upper triangular matrix R.

		\param x output vector containing the solution of the linear system
		\param b right-hand side vector of the linear system
		\param c vector containing diagonal elements of the R matrix for scaling
		\param d vector containing the scaled Householder vectors for Q transformation
		\throws assertion failure if matrix dimensions are inconsistent or vectors are too small
	*/
	void					QR_Solve( idVecX& x, const idVecX& b, const idVecX& c, const idVecX& d ) const;

	/*!
		\brief Solves a system of linear equations using QR decomposition.

		This function solves the linear system Ax = b using QR decomposition where A is represented by the current matrix. The function takes the right-hand side vector b and the upper triangular
	   matrix R from the QR decomposition, and computes the solution vector x. The process involves two main steps: first applying the transpose of the Q matrix to the vector b, and then performing
	   back substitution using the R matrix. The matrix must be square for this operation to be valid.

		\param x the solution vector to be computed
		\param b the right-hand side vector of the linear system
		\param R the upper triangular matrix from QR decomposition
		\throws assertion failure if the matrix is not square
	*/
	void					QR_Solve( idVecX& x, const idVecX& b, const idMatX& R ) const;

	/*!
		\brief Computes the inverse of the matrix using QR decomposition.

		This function calculates the inverse of a matrix by using QR decomposition. It constructs the inverse by solving a series of linear systems with the matrix. For each column of the result, it
	   sets up a unit vector as the right-hand side, solves the system using the QR_Solve function, and stores the solution as a column of the inverse matrix.

		\param inv Output matrix that will contain the inverse of this matrix
		\param c Coefficient vector used in computing the Householder reflectors for the Q matrix
		\param d Diagonal vector used in constructing the upper triangular matrix R
		\throws assertion failure if the matrix is not square
	*/
	void					QR_Inverse( idMatX& inv, const idVecX& c, const idVecX& d ) const;

	/*!
		\brief Unpacks QR factors from this matrix into the provided matrices Q and R using the given coefficient vectors c and d.

		This function reconstructs the orthogonal matrix Q and upper triangular matrix R from the QR decomposition stored in this matrix. The coefficients c and d are used to compute the Householder
	   reflectors for the Q matrix, while the R matrix is constructed from the original matrix data and the diagonal vector d.

		\param Q Output orthogonal matrix resulting from the QR decomposition
		\param R Output upper triangular matrix resulting from the QR decomposition
		\param c Coefficient vector used in computing the Householder reflectors for matrix Q
		\param d Diagonal vector used in constructing the upper triangular matrix R
	*/
	void					QR_UnpackFactors( idMatX& Q, idMatX& R, const idVecX& c, const idVecX& d ) const;

	/*!
		\brief Performs QR factorization multiplication with the given vectors to produce a new matrix.

		This function applies the QR decomposition factors stored in the current matrix to the input vectors c and d to compute a new matrix m. It first constructs an identity matrix Q and then
	   applies the Householder reflections stored in the current matrix to update Q. Finally, it multiplies the updated Q matrix with the vector d to produce the result matrix m. The function handles
	   the case where the diagonal element c[i] is zero by skipping the computation for that iteration.

		\param m Output matrix that stores the result of the QR multiplication
		\param c Vector containing the diagonal elements of the R matrix from QR decomposition
		\param d Vector of diagonal elements to be multiplied with the Q matrix
	*/
	void					QR_MultiplyFactors( idMatX& m, const idVecX& c, const idVecX& d ) const;

	/*!
		\brief Performs singular value decomposition on the matrix and stores the diagonal vector in w and the right singular vectors in V

		This function factors the matrix into its singular value decomposition form. It computes the singular values and stores them in the vector w, and the right singular vectors in the matrix V.
	   The function returns true if the decomposition was successful, false otherwise. The algorithm uses a bi-diagonalization approach followed by iterative refinement to compute the singular values
	   and vectors.

		\param w Vector to store the computed singular values
		\param V Matrix to store the right singular vectors
		\return True if the singular value decomposition was successful, false otherwise
	*/
	bool					SVD_Factor( idVecX& w, idMatX& V );

	/*!
		\brief Solves a system of linear equations using singular value decomposition

		This function computes the solution to a system of linear equations using the singular value decomposition technique. It takes the right-hand side vector b, the singular values w, and the V
	   matrix from the SVD of the current matrix, and computes the solution vector x. The function first calculates intermediate values using the current matrix and the right-hand side vector, then
	   uses the V matrix to compute the final solution.

		\param x output vector that will contain the solution to the system of equations
		\param b right-hand side vector of the system of equations
		\param w vector containing the singular values of the matrix
		\param V matrix containing the right singular vectors of the matrix
		\throws assertion failure if the input vectors do not have sufficient size
	*/
	void					SVD_Solve( idVecX& x, const idVecX& b, const idVecX& w, const idMatX& V ) const;

	/*!
		\brief Computes the inverse of a matrix using singular value decomposition with the provided weight vector and V matrix

		This function calculates the inverse of a matrix by utilizing the singular value decomposition (SVD) approach. It takes the weight vector and V matrix from the SVD decomposition of the
	   original matrix to compute the inverse. The function first checks if the matrix is square using an assertion, then performs matrix operations to compute the inverse. The computation involves
	   multiplying the V matrix by a diagonal matrix formed from the reciprocal of the weight vector, and then multiplying by the transpose of the original matrix.

		\param inv Output matrix that will contain the computed inverse
		\param w Vector containing the singular values of the matrix
		\param V Matrix containing the right singular vectors of the matrix
		\throws assertion failure if the matrix is not square
	*/
	void					SVD_Inverse( idMatX& inv, const idVecX& w, const idMatX& V ) const;

	/*!
		\brief Multiplies the SVD factors of this matrix with the provided weight vector and V matrix

		This function performs a multiplication operation using the SVD (Singular Value Decomposition) factors of the current matrix. It takes the weight vector w and the V matrix as inputs and
	   computes the result by multiplying the current matrix rows with the corresponding entries in V, scaled by the weights. The function processes each row of the current matrix and calculates the
	   output matrix m based on the SVD components. Rows where the corresponding weight is below the epsilon threshold are set to zero in the result matrix.

		\param m Output matrix that will contain the result of the multiplication
		\param w Weight vector containing singular values for scaling
		\param V Matrix containing the right singular vectors
	*/
	void					SVD_MultiplyFactors( idMatX& m, const idVecX& w, const idMatX& V ) const;

	/*!
		\brief Performs in-place Cholesky factorization on the matrix and returns true if successful.

		This function implements the Cholesky decomposition algorithm for a square matrix. It performs in-place factorization, meaning the input matrix is modified to store the decomposition results.
	   The function returns false if the matrix is not positive definite, which would prevent a valid Cholesky decomposition. The algorithm computes the lower triangular matrix L such that A = LL^T,
	   where A is the input matrix. The factorization proceeds row by row, computing the necessary values for the lower triangular matrix while checking for positive definiteness at each step.

		\return true if the Cholesky factorization was successful, false if the matrix is not positive definite
		\throws assertion failure if the matrix is not square
	*/
	bool					Cholesky_Factor();

	/*!
		\brief Performs a rank-one update on the Cholesky decomposition of the matrix

		This function updates the Cholesky decomposition of a matrix by performing a rank-one update. It modifies the matrix in place by updating the diagonal elements and corresponding off-diagonal
	   elements. The update is performed starting from a specified offset row and column. The function returns false if the update would result in a non-positive diagonal element, indicating that the
	   matrix is no longer positive definite.

		\param v The vector to use for the rank-one update
		\param alpha The scalar multiplier for the update
		\param offset The starting row and column index for the update operation
		\return true if the update was successful, false if the update would result in a non-positive diagonal element
		\throws No explicit throws, but relies on assertions for validation of input parameters
	*/
	bool					Cholesky_UpdateRankOne( const idVecX& v, float alpha, int offset = 0 );

	/*!
		\brief Performs a Cholesky update of a row and column in the matrix, updating the Cholesky decomposition with a new row and column increment.

		This function updates a Cholesky decomposition by adding a new row and column to the matrix. It handles special cases for the first row/column and the last row/column of the matrix. For
	   intermediate rows/columns, it calculates the necessary updates and performs a simultaneous update/downdate of the submatrix starting at the given row index. The function uses a rank-one update
	   approach to maintain numerical stability.

		\param v The vector representing the new row and column to be added to the matrix
		\param r The row index where the update will be applied
		\return true if the matrix was successfully updated, false if the update would result in a non-positive diagonal element
	*/
	bool					Cholesky_UpdateRowColumn( const idVecX& v, int r );

	/*!
		\brief Updates the Cholesky decomposition by incrementing the matrix with a new row and column.

		This function performs an incremental Cholesky update by adding a new row and column to the existing Cholesky decomposition. It modifies the matrix in place to reflect the addition of the new
	   data represented by the input vector v. The function solves for the new row of the lower triangular matrix L and calculates the diagonal entry. If the calculation results in a non-positive
	   value for the diagonal entry, the update fails and the function returns false. Otherwise, it successfully updates the decomposition and returns true.

		\param v The vector representing the new row and column to be added to the Cholesky decomposition
		\return true if the Cholesky decomposition was successfully updated with the new row and column, false if the update failed due to a non-positive diagonal entry
		\throws assertion failure if the matrix is not square or if the input vector size is insufficient
	*/
	bool					Cholesky_UpdateIncrement( const idVecX& v );

	/*!
		\brief Performs a decremental Cholesky update by removing a row and column from the matrix.

		This function removes a specified row and column from a matrix that is represented by its Cholesky decomposition. It first updates the matrix to reflect the removal by calling
	   Cholesky_UpdateRowColumn with a modified vector, and then physically removes the row and column using Update_Decrement. The function assumes that the matrix is square and that the specified row
	   index is valid.

		\param v The vector used to update the matrix before removing the row and column
		\param r The index of the row and column to be removed
		\return True if the operation was successful, false otherwise
	*/
	bool					Cholesky_UpdateDecrement( const idVecX& v, int r );

	/*!
		\brief Solves a system of linear equations using Cholesky decomposition.

		This function solves the linear system Ax = b using Cholesky decomposition where the matrix A is stored in this object. The decomposition is performed in-place and the solution vector x is
	   computed by forward and backward substitution. The matrix must be symmetric and positive definite for the Cholesky decomposition to be valid.

		\param x the solution vector that will be filled with the result of solving the linear system
		\param b the right-hand side vector of the linear system
		\throws assertion failure if the matrix is not square or if the vectors x and b do not have sufficient size
	*/
	void					Cholesky_Solve( idVecX& x, const idVecX& b ) const;

	/*!
		\brief Computes the inverse of a matrix using Cholesky decomposition and stores the result in the provided matrix

		This function calculates the inverse of the matrix by utilizing Cholesky decomposition. It sets up temporary vectors for solving linear systems and iterates through each column of the matrix.
	   For each column, it solves a linear system using the Cholesky solver and stores the resulting column in the output matrix. The process effectively constructs the inverse matrix by solving
	   multiple linear systems.

		\param inv The matrix that will contain the inverse of the current matrix
		\throws assertion failure if the matrix is not square
	*/
	void					Cholesky_Inverse( idMatX& inv ) const;

	/*!
		\brief Multiplies the Cholesky factors and stores the result in the provided matrix

		This function performs the multiplication of the Cholesky factors stored in the current matrix and writes the resulting matrix into the provided matrix parameter m. It is used to reconstruct
	   the original matrix from its Cholesky decomposition.

		\param m The matrix that will contain the result of multiplying the Cholesky factors
	*/
	void					Cholesky_MultiplyFactors( idMatX& m ) const;

	/*!
		\brief Performs in-place LDLT factorization on the matrix

		This function computes the LDLT factorization of a symmetric matrix in-place. The factorization decomposes the matrix into the product of a lower triangular matrix L, a diagonal matrix D, and
	   the transpose of the lower triangular matrix L. The function modifies the matrix itself to store the L and D factors. It returns false if the factorization fails due to a zero pivot element,
	   indicating that the matrix is singular or not positive definite.

		\return true if the factorization was successful, false if a diagonal element becomes zero during the computation
		\throws assertion failure if the matrix is not square
	*/
	bool					LDLT_Factor();

	/*!
		\brief Performs a rank-one update on the LDLT decomposition of the matrix using the provided vector and scaling factor.

		This function updates the LDLT decomposition of a matrix by performing a rank-one modification. It takes a vector v, a scaling factor alpha, and an offset to determine which part of the matrix
	   to update. The function modifies the matrix in-place and returns true if the update was successful, or false if a diagonal element becomes zero during the process.

		\param v The input vector used for the rank-one update
		\param alpha The scaling factor applied to the outer product of the vector with itself
		\param offset The starting index for the rank-one update operation
		\return true if the rank-one update was successful, false if a diagonal element becomes zero during the computation
	*/
	bool					LDLT_UpdateRankOne( const idVecX& v, float alpha, int offset = 0 );

	/*!
		\brief Performs an LDLT update of a row and column in the matrix using the provided vector and row index.

		This function updates an LDLT decomposition by incrementally adding a new row and column to the matrix. It handles the case where the first row/column is updated separately from the general
	   case. For general updates, it calculates the original row/column of the matrix, solves for y in the equation L * y = original + v, and updates the row of L accordingly. If the last row/column
	   is being updated, it only calculates the new diagonal element. Otherwise, it calculates the row/column to be added to the lower right submatrix starting at (r, r) and performs a simultaneous
	   update/downdate of that submatrix. The function returns false if a diagonal element becomes zero during the update, indicating a potential numerical issue.

		\param v The vector used to update the row and column
		\param r The row index of the row and column to update
		\return True if the update was successful, false if a diagonal element becomes zero during the update
	*/
	bool					LDLT_UpdateRowColumn( const idVecX& v, int r );

	/*!
		\brief Updates the LDLT decomposition by incrementally adding a new row and column to the matrix.

		This function performs an incremental update of the LDLT decomposition when a new row and column are added to the matrix. It solves for the new decomposition values using the previously
	   computed lower triangular matrix L and diagonal matrix D. The function modifies the matrix in-place to reflect the updated decomposition. The input vector v should have at least numRows + 1
	   elements, where the first numRows elements represent the new row/column values and the last element is used for diagonal calculation. The function returns false if the diagonal entry becomes
	   zero, indicating that the matrix is singular or nearly singular.

		\param v The vector containing the new row and column values for the incremental update
		\return true if the update was successful and the diagonal entry is non-zero, false if the diagonal entry is zero indicating a singular matrix
	*/
	bool					LDLT_UpdateIncrement( const idVecX& v );

	/*!
		\brief Updates the LDLT decomposition by decrementing the specified row and column

		This function performs an update on the LDLT decomposition by decrementing a specified row and column. It first prepares a modified vector by negating the input vector and incrementing the
	   diagonal element at the specified index. Then it calls LDLT_UpdateRowColumn to update the decomposition with this modified vector. After the update, it physically removes the specified row and
	   column from the matrix. The function returns false if the decomposition update fails, otherwise it returns true.

		\param v The input vector to be used for the update
		\param r The row and column index to decrement
		\return True if the update and decrement operations were successful, false otherwise
	*/
	bool					LDLT_UpdateDecrement( const idVecX& v, int r );

	/*!
		\brief Solves a system of linear equations using LDLT decomposition.

		This function performs LDLT decomposition based solving of linear equations. It first forward solves the lower triangular matrix L, then applies the diagonal matrix D, and finally backward
	   solves the upper triangular matrix Lt. The input vector b contains the right-hand side of the equation, and the result is stored in vector x.

		\param x vector to store the solution of the linear system
		\param b right-hand side vector of the linear system
		\throws assertion failure if the matrix is not square or if the vectors do not have sufficient size
	*/
	void					LDLT_Solve( idVecX& x, const idVecX& b ) const;

	/*!
		\brief Computes the inverse of the matrix using LDLT decomposition and stores the result in the provided matrix

		This function calculates the inverse of the matrix by utilizing the LDLT decomposition that was previously computed and stored in the object. It solves a system of linear equations for each
	   column of the identity matrix to determine the corresponding column of the inverse matrix. The function assumes that the matrix is square and that the LDLT factorization has been computed.

		\param inv The matrix that will store the computed inverse
		\throws assertion failure if the matrix is not square
	*/
	void					LDLT_Inverse( idMatX& inv ) const;

	/*!
		\brief Unpacks the LDLT factorization of this matrix into a lower triangular matrix L and a diagonal matrix D.

		This function decomposes the stored LDLT factorization into two separate matrices. The lower triangular matrix L contains the lower triangular elements of the factorization with 1.0f on the
	   diagonal, while the diagonal matrix D contains the diagonal elements of the original matrix. The factorization is assumed to be stored in the current object in a packed format where the lower
	   triangular part (excluding diagonal) and diagonal elements are stored.

		\param L Output lower triangular matrix containing the packed factorization
		\param D Output diagonal matrix containing the diagonal elements of the original matrix
	*/
	void					LDLT_UnpackFactors( idMatX& L, idMatX& D ) const;

	/*!
		\brief Multiplies the LDLT factors of this matrix with the provided matrix m and clears all triangle elements in the result

		This function performs matrix multiplication using the LDLT factorization components stored in this matrix. It processes each row of the matrix and computes the product of the factorization
	   components with the input matrix m. The operation modifies the matrix m in place, clearing all triangular elements as part of the computation process.

		\param m The matrix to multiply the LDLT factors with, which will be modified in place
	*/
	void					LDLT_MultiplyFactors( idMatX& m ) const;

	/*!
		\brief Clears all triangle elements in the matrix by setting them to zero.

		This function zeros out all elements that are not part of the main diagonal or the immediate diagonals above and below it. It operates on a square matrix and ensures that only the central
	   diagonal and the two adjacent diagonals retain their values, while all other elements are set to zero. The function iterates through the matrix to clear the off-triangular elements.

		\throws assertion failure if the matrix is not square
	*/
	void					TriDiagonal_ClearTriangles();

	/*!
		\brief Solves a tridiagonal system of linear equations and returns true if successful

		This function performs forward elimination followed by backward substitution to solve a tridiagonal system of linear equations represented by the current matrix. The system is represented as
	   Ax = b where A is a tridiagonal matrix. The function modifies the x vector in place with the solution. It returns false if a zero pivot is encountered during the elimination process, indicating
	   that the system cannot be solved using this method.

		\param x vector to store the solution of the system, modified in place
		\param b right-hand side vector of the system
		\return true if the system was solved successfully, false if a zero pivot was encountered
	*/
	bool					TriDiagonal_Solve( idVecX& x, const idVecX& b ) const;

	/*!
		\brief Computes the inverse of a tridiagonal matrix and stores the result in the provided matrix

		This function calculates the inverse of a tridiagonal matrix by solving a system of linear equations for each column of the identity matrix. It uses the TriDiagonal_Solve method to compute the
	   solution for each column, effectively building the inverse matrix column by column. The function requires the matrix to be square and asserts this condition before proceeding with the
	   computation.

		\param inv The matrix that will store the computed inverse of the tridiagonal matrix
		\throws assertion failure if the matrix is not square
	*/
	void					TriDiagonal_Inverse( idMatX& inv ) const;

	/*!
		\brief Solves the eigenvalue problem for a symmetric tridiagonal matrix and returns the eigenvalues.

		This function computes the eigenvalues of a symmetric tridiagonal matrix by first extracting the diagonal and subdiagonal elements, then using the QL algorithm to solve the eigenvalue problem.
	   The matrix must be square and symmetric for this method to be applicable. The function modifies the matrix in-place to form the tridiagonal form and returns the computed eigenvalues in the
	   provided vector.

		\param eigenValues output vector to store the computed eigenvalues
		\return true if the eigenvalue computation was successful, false otherwise
		\throws assertion failure if the matrix is not square
	*/
	bool					Eigen_SolveSymmetricTriDiagonal( idVecX& eigenValues );

	/*!
		\brief Computes the eigenvalues of a symmetric matrix and returns whether the computation was successful

		This function computes the eigenvalues of a symmetric matrix using the Householder reduction method followed by the QL algorithm. It requires the matrix to be square and symmetric. The
	   function modifies the eigenValues parameter to contain the computed eigenvalues and returns true if the computation was successful, false otherwise.

		\param eigenValues Reference to a vector that will be filled with the computed eigenvalues
		\return True if the eigenvalue computation was successful, false otherwise
		\throws Assertion error if the matrix is not square
	*/
	bool					Eigen_SolveSymmetric( idVecX& eigenValues );

	/*!
		\brief Computes the eigenvalues of a square matrix and returns whether the computation was successful

		This function calculates the eigenvalues of a square matrix by first reducing it to Hessenberg form and then converting it to real Schur form. The real and imaginary parts of the eigenvalues
	   are stored in the provided vectors. The function returns true if the computation was successful, false otherwise.

		\param realEigenValues Vector to store the real parts of the computed eigenvalues
		\param imaginaryEigenValues Vector to store the imaginary parts of the computed eigenvalues
		\return True if the eigenvalue computation was successful, false otherwise
	*/
	bool					Eigen_Solve( idVecX& realEigenValues, idVecX& imaginaryEigenValues );

	/*!
		\brief Sorts the eigenvalues in ascending order and rearranges the corresponding eigenvectors.

		This function performs an ascending sort on the eigenvalues provided in the eigenValues vector. During the sorting process, it also rearranges the columns of the matrix to maintain the
	   correspondence between eigenvalues and their respective eigenvectors. The sorting is implemented using a selection sort algorithm.

		\param eigenValues Vector containing the eigenvalues to be sorted in ascending order
	*/
	void					Eigen_SortIncreasing( idVecX& eigenValues );

	/*!
		\brief Sorts eigenvalues in decreasing order and updates corresponding matrix columns.

		This function performs a selection sort on the eigenvalues to arrange them in decreasing order. For each eigenvalue, it finds the maximum value in the remaining unsorted portion of the array
	   and swaps it with the current element. When a swap occurs, it also swaps the corresponding columns in the matrix to maintain consistency between eigenvalues and their associated eigenvectors.

		\param eigenValues The vector containing the eigenvalues to be sorted in decreasing order
	*/
	void					Eigen_SortDecreasing( idVecX& eigenValues );

	/*!
		\brief Performs comprehensive tests on various matrix operations and their inverses.

		This function conducts extensive testing of matrix inversion and decomposition operations including lower triangular inverse, upper triangular inverse, Gauss-Jordan elimination, and updates to
	   matrix inverses using rank-one, row-column, increment, and decrement operations. It also tests LU factorization and its updates. The function validates the correctness of these operations by
	   comparing results with expected outcomes and reports failures via warnings.

	*/
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
	/*!
		\brief Sets the size of a temporary matrix allocation for the specified number of rows and columns.

		This function configures the dimensions of a temporary matrix that will be used for calculations. It computes the required memory size based on the provided rows and columns, ensuring proper
	   alignment by rounding up to the nearest multiple of 4. The function also handles the allocation of memory from a pre-allocated temporary buffer, resetting the buffer index if necessary to
	   prevent overflow. The matrix data pointer is updated to point to the allocated memory, and the matrix dimensions and allocation status are set accordingly.

		\param rows The number of rows for the temporary matrix
		\param columns The number of columns for the temporary matrix
		\throws assertion failure if the computed size exceeds the maximum allowed temporary matrix size
	*/
	void  SetTempSize( int rows, int columns );

	/*!
		\brief Computes the determinant of the matrix using a generic algorithm.

		This function calculates the determinant of the matrix by first performing an LU decomposition with partial pivoting. The LU decomposition is performed in-place on a temporary copy of the
	   matrix, and the determinant is computed from the diagonal elements of the decomposed matrix. If the matrix is singular or cannot be decomposed, the function returns zero.

		\return The determinant of the matrix as a floating-point value.
	*/
	float DeterminantGeneric() const;

	/*!
		\brief Computes the inverse of the matrix in-place using a generic algorithm.

		This function calculates the inverse of the matrix by first performing an LU decomposition of the matrix. It then solves multiple linear systems to compute the inverse matrix. The algorithm
	   uses a temporary matrix and vector storage allocated on the stack. If the LU decomposition fails, the function returns false indicating that the matrix is not invertible. Otherwise, it returns
	   true after successfully computing the inverse.

		\return true if the matrix was successfully inverted, false if the matrix is not invertible
	*/
	bool  InverseSelfGeneric();

	/*!
		\brief Performs a Givens rotation on the matrix R and the current matrix using the given cosine and sine values

		This function applies a Givens rotation to two rows of the matrix, specifically rows i and i+1. It computes the cosine and sine values based on the input parameters a and b to ensure numerical
	   stability. The rotation is applied to both the matrix R and the current matrix instance. The function handles special cases where one of the input values is zero to avoid division by zero and
	   ensures proper sign handling for the computed trigonometric values

		\param R Reference to the matrix on which the rotation is applied
		\param i Index of the first row to be rotated
		\param a First parameter used to compute the rotation angles
		\param b Second parameter used to compute the rotation angles
	*/
	void  QR_Rotate( idMatX& R, int i, float a, float b );

	/*!
		\brief Computes the square root of the sum of squares of two floating-point numbers using a numerically stable approach to avoid overflow.

		This function calculates the Euclidean norm of two numbers, commonly used in mathematical computations to avoid numerical overflow that could occur when directly computing sqrt(a*a + b*b). The
	   implementation uses a stable algorithm that divides the larger absolute value by the smaller one before performing the square root calculation. If both values are zero, it returns zero.

		\param a First floating-point number
		\param b Second floating-point number
		\return The square root of the sum of squares of the two input values
	*/
	float Pythag( float a, float b ) const;

	/*!
		\brief Performs bidiagonalization of the matrix using SVD decomposition methods and initializes working vectors for singular value decomposition.

		This function implements the bidiagonalization step of the singular value decomposition (SVD) algorithm. It modifies the matrix in place to produce a bidiagonal form, while computing the
	   singular values and auxiliary vectors needed for the full SVD process. The function also computes the maximum absolute value of the matrix elements, which is stored in the anorm parameter and
	   used for numerical stability checks during subsequent SVD iterations.

		\param w Output vector containing the diagonal elements of the bidiagonal matrix
		\param rv1 Auxiliary vector used during the bidiagonalization process
		\param anorm Output parameter storing the maximum absolute value of all matrix elements
	*/
	void  SVD_BiDiag( idVecX& w, idVecX& rv1, float& anorm );

	/*!
		\brief Initializes the singular value decomposition working vectors for the current matrix by performing Householder reduction.

		This function sets up the necessary working vectors for computing the singular value decomposition of the matrix. It performs Householder reduction on the matrix, storing diagonal and
	   subdiagonal elements in the provided vectors w and rv1. The matrix V is updated to contain the householder vectors. The function modifies the current matrix in-place to store intermediate
	   results during the SVD computation process.

		\param w Vector to store diagonal elements of the reduced matrix
		\param V Matrix to store householder vectors
		\param rv1 Vector to store subdiagonal elements of the reduced matrix
	*/
	void  SVD_InitialWV( idVecX& w, idMatX& V, idVecX& rv1 );

	/*!
		\brief Performs Householder reduction on the matrix and stores the diagonal and subdiagonal elements in the provided vectors.

		This function applies the Householder reduction algorithm to transform the current matrix into a tridiagonal form. The diagonal elements are stored in the diag vector and the subdiagonal
	   elements are stored in the subd vector. The algorithm uses Householder reflections to zero out elements below the subdiagonal, effectively reducing the matrix to tridiagonal form. The function
	   modifies the current matrix in-place during the reduction process. The final result allows for subsequent QL decomposition to find eigenvalues.

		\param diag Vector to store the diagonal elements of the tridiagonal matrix
		\param subd Vector to store the subdiagonal elements of the tridiagonal matrix
		\throws assertion failure if the matrix is not square
	*/
	void  HouseholderReduction( idVecX& diag, idVecX& subd );

	/*!
		\brief Performs QL decomposition on the matrix and returns true if successful

		This function implements the QL algorithm for computing the eigenvalues and eigenvectors of a symmetric matrix. It modifies the matrix in place and requires the matrix to be square. The
	   algorithm uses a series of rotations to reduce the matrix to tridiagonal form, then iteratively computes the eigenvalues. The function takes two parameters: diag and subd, which are updated
	   with the diagonal and subdiagonal elements of the tridiagonal matrix after the decomposition. The function returns false if the maximum number of iterations is exceeded, indicating that the
	   algorithm did not converge.

		\param diag diagonal elements of the matrix
		\param subd subdiagonal elements of the matrix
		\return true if the QL decomposition was successful, false if the maximum number of iterations was exceeded
	*/
	bool  QL( idVecX& diag, idVecX& subd );

	/*!
		\brief Performs Hessenberg reduction on the input matrix H in place

		This function reduces a square matrix to Hessenberg form using Householder transformations. The reduction process involves applying a sequence of orthogonal transformations to eliminate
	   entries below the first subdiagonal. The algorithm processes the matrix from left to right, applying Householder reflections to zero out appropriate elements. The transformation matrix is
	   accumulated during the process to maintain the similarity relationship. The function modifies the input matrix H directly and does not return a separate result. The Hessenberg form has non-zero
	   entries only on the main diagonal, the first diagonal above it, and potentially some additional entries in the first subdiagonal.

		\param H Input square matrix to be reduced to Hessenberg form, modified in place
	*/
	void  HessenbergReduction( idMatX& H );

	/*!
		\brief Performs complex division of two complex numbers and stores the result in the provided output variables.

		This function computes the division of two complex numbers represented as (xr + xi * i) / (yr + yi * i). The calculation is performed in a numerically stable way by choosing the appropriate
	   computation path based on the relative magnitudes of the real and imaginary parts of the denominator. The results are stored in the provided output variables cdivr and cdivi which represent the
	   real and imaginary parts of the quotient respectively.

		\param xr real part of the numerator
		\param xi imaginary part of the numerator
		\param yr real part of the denominator
		\param yi imaginary part of the denominator
		\param cdivr output variable to store the real part of the quotient
		\param cdivi output variable to store the imaginary part of the quotient
	*/
	void  ComplexDivision( float xr, float xi, float yr, float yi, float& cdivr, float& cdivi );

	/*!
		\brief Computes the real and imaginary parts of eigenvalues for a Hessenberg matrix using the QR algorithm

		This function implements the QR algorithm to compute the eigenvalues of a Hessenberg matrix. It transforms the matrix into real Schur form and extracts the eigenvalues, storing the real parts
	   in realEigenValues and imaginary parts in imaginaryEigenValues. The function handles both real and complex eigenvalue pairs and uses Wilkinson's shift technique for improved convergence.

		\param H The Hessenberg matrix to compute eigenvalues for
		\param realEigenValues Output vector to store the real parts of the eigenvalues
		\param imaginaryEigenValues Output vector to store the imaginary parts of the eigenvalues
		\return True if the computation was successful, false otherwise
	*/
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

/*
========================
idMatX::operator[]
========================
*/
ID_INLINE const float* idMatX::operator[]( int index ) const
{
	assert( ( index >= 0 ) && ( index < numRows ) );
	return mat + index * numColumns;
}

/*
========================
idMatX::operator[]
========================
*/
ID_INLINE float* idMatX::operator[]( int index )
{
	assert( ( index >= 0 ) && ( index < numRows ) );
	return mat + index * numColumns;
}

/*
========================
idMatX::operator=
========================
*/
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

/*
========================
idMatX::operator*
========================
*/
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

/*
========================
idMatX::operator*
========================
*/
ID_INLINE idVecX idMatX::operator*( const idVecX& vec ) const
{
	assert( numColumns == vec.GetSize() );

	idVecX dst;
	dst.SetTempSize( numRows );
	Multiply( dst, vec );
	return dst;
}

/*
========================
idMatX::operator*
========================
*/
ID_INLINE idMatX idMatX::operator*( const idMatX& a ) const
{
	assert( numColumns == a.numRows );

	idMatX dst;
	dst.SetTempSize( numRows, a.numColumns );
	Multiply( dst, a );
	return dst;
}

/*
========================
idMatX::operator+
========================
*/
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

/*
========================
idMatX::operator-
========================
*/
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

/*
========================
idMatX::operator*=
========================
*/
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

/*
========================
idMatX::operator*=
========================
*/
ID_INLINE idMatX& idMatX::operator*=( const idMatX& a )
{
	*this			  = *this * a;
	idMatX::tempIndex = 0;
	return *this;
}

/*
========================
idMatX::operator+=
========================
*/
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

/*
========================
idMatX::operator-=
========================
*/
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

/*
========================
operator*
========================
*/
ID_INLINE idMatX operator*( const float a, idMatX const& m )
{
	return m * a;
}

/*
========================
operator*
========================
*/
ID_INLINE idVecX operator*( const idVecX& vec, const idMatX& m )
{
	return m * vec;
}

/*
========================
operator*=
========================
*/
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

/*
========================
idMatX::operator==
========================
*/
ID_INLINE bool idMatX::operator==( const idMatX& a ) const
{
	return Compare( a );
}

/*
========================
idMatX::operator!=
========================
*/
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

/*
========================
idMatX::Zero
========================
*/
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

/*
========================
idMatX::Identity
========================
*/
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
