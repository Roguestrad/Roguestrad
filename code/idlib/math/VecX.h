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

#ifndef __MATH_VECX_H__
#define __MATH_VECX_H__

/*
===============================================================================

idVecX - arbitrary sized vector

The vector lives on 16 byte aligned and 16 byte padded memory.

NOTE: due to the temporary memory pool idVecX cannot be used by multiple threads

===============================================================================
*/

#define VECX_MAX_TEMP  1024
#define VECX_QUAD( x ) ( ( ( ( x ) + 3 ) & ~3 ) * sizeof( float ) )
#define VECX_CLEAREND()               \
	int s = size;                     \
	while( s < ( ( s + 3 ) & ~3 ) ) { \
		p[s++] = 0.0f;                \
	}
#define VECX_ALLOCA( n ) ( ( float* )_alloca16( VECX_QUAD( n ) ) )

#if defined( USE_INTRINSICS_SSE )
	#define VECX_SIMD
#endif

/*!
	\class idVecX
	\brief A dynamic vector class that supports flexible memory management and mathematical operations on float vectors of arbitrary dimension.

	The idVecX class provides a flexible container for vectors of arbitrary dimension, supporting dynamic resizing, memory management, and a comprehensive set of mathematical operations. It is
   designed to handle both regular and temporary memory allocations, with special handling for temporary buffers to optimize performance in graphics and physics calculations. The class offers direct
   element access through indexing operators, element-wise operations, and vector arithmetic including normalization, clamping, and component extraction. Memory management is handled through custom
   allocation functions that support both standard and temporary memory pools. The class supports various vector operations such as addition, subtraction, scalar multiplication, dot product, and
   length calculations. It also provides utility methods for random number generation, zero initialization, and element-wise comparisons with epsilon tolerance. The implementation utilizes SIMD
   optimizations where available for performance-critical operations like vector normalization and zeroing. The class is intended for use throughout the engine for representing geometric data, physics
   vectors, and other mathematical constructs that require dynamic dimensionality and efficient memory usage.

*/
class idVecX
{
	friend class idMatX;

public:
	/*!
		\brief Initializes an empty idVecX object with no allocated memory.

		Constructs an idVecX object with no allocated memory and sets the size and allocated count to zero.

		\return ID_INLINE
	*/
	ID_INLINE idVecX();

	/*!
		\brief Constructs an idVecX object with the specified length

		The constructor initializes the vector by setting the size and allocated memory to zero, and the data pointer to NULL. It then calls SetSize to properly configure the object with the specified
	   length

		\param length The number of elements in the vector
		\return ID_INLINE
	*/
	ID_INLINE explicit idVecX( int length );

	/*!
		\brief Constructs an idVecX object with the specified length and data pointer

		This constructor initializes an idVecX object by setting the size and allocated memory to zero and the data pointer to NULL. It then calls SetData to properly configure the object with the
	   provided length and data pointer

		\param length The number of elements in the vector
		\param data Pointer to the float array containing the vector data
		\return ID_INLINE
	*/
	ID_INLINE explicit idVecX( int length, float* data );

	/*!
		\brief Destructor for idVecX that frees allocated memory if it's not temporary memory.

		The destructor checks if the vector's memory pointer is not part of the temporary memory pool. If the memory was allocated and is not temporary, it frees the memory using Mem_Free16. This
	   ensures that temporary memory is not freed, preventing potential issues with memory management.

		\return ID_INLINE
	*/
	ID_INLINE ~idVecX();

	/*!
		\brief Returns the element at the specified index in the vector

		This function retrieves the element located at the given index in the vector. It performs an assertion to ensure the index is within valid bounds, specifically checking that the index is
	   greater than or equal to zero and less than the size of the vector. The function returns a float value representing the element at the specified index

		\param index The zero-based index of the element to retrieve
		\return The element at the specified index in the vector
		\throws assertion failure if the index is out of bounds
	*/
	ID_INLINE float			Get( int index ) const;

	/*!
		\brief Returns a reference to the element at the specified index in the vector.

		This function provides direct access to an element of the vector by index. It performs an assertion check to ensure the index is within valid bounds before returning a reference to the
	   element. The returned reference allows both reading and modifying the vector element directly.

		\param index The zero-based index of the element to access
		\return A reference to the element at the specified index in the vector
		\throws assertion failure if the index is out of bounds
	*/
	ID_INLINE float&		Get( int index );

	ID_INLINE float			operator[]( const int index ) const;
	ID_INLINE float&		operator[]( const int index );
	ID_INLINE idVecX		operator-() const;
	ID_INLINE idVecX&		operator=( const idVecX& a );
	ID_INLINE idVecX		operator*( const float a ) const;
	ID_INLINE idVecX		operator/( const float a ) const;
	ID_INLINE float			operator*( const idVecX& a ) const;
	ID_INLINE idVecX		operator-( const idVecX& a ) const;
	ID_INLINE idVecX		operator+( const idVecX& a ) const;
	ID_INLINE idVecX&		operator*=( const float a );
	ID_INLINE idVecX&		operator/=( const float a );
	ID_INLINE idVecX&		operator+=( const idVecX& a );
	ID_INLINE idVecX&		operator-=( const idVecX& a );

	friend ID_INLINE idVecX operator*( const float a, const idVecX& b );

	/*!
		\brief Compares this vector with another vector for exact equality.

		This function performs an exact comparison between this vector and another vector of the same size. It first asserts that both vectors have the same size, then iterates through each element to
	   check for equality. The function returns true if all elements are equal, and false otherwise.

		\param a The vector to compare with this vector
		\return True if all elements of this vector are equal to the corresponding elements of the other vector, false otherwise
		\throws Assertion failure if the vectors have different sizes
	*/
	ID_INLINE bool			Compare( const idVecX& a ) const;

	/*!
		\brief Compares this vector with another vector using the specified epsilon tolerance

		This function performs an element-wise comparison between this vector and another vector of the same size. It uses the provided epsilon value to determine if the absolute difference between
	   corresponding elements is within the tolerance. If any pair of elements differs by more than epsilon, the function returns false. Otherwise, it returns true, indicating the vectors are equal
	   within the given tolerance.

		\param a The vector to compare against
		\param epsilon The tolerance value for comparison
		\return True if all corresponding elements of the vectors are within the specified epsilon tolerance, false otherwise
		\throws assertion failure if the sizes of the vectors do not match
	*/
	ID_INLINE bool			Compare( const idVecX& a, const float epsilon ) const;
	ID_INLINE bool			operator==( const idVecX& a ) const; // exact compare, no epsilon
	ID_INLINE bool			operator!=( const idVecX& a ) const; // exact compare, no epsilon

	/*!
		\brief Sets the size of the vector, reallocating memory if necessary.

		This function adjusts the size of the vector to the specified value. It will reallocate memory if the new size requires more space than currently allocated. The function ensures that the
	   vector can accommodate the new size, and if the vector is being resized to a larger size, the new elements are initialized to zero. If the vector is being resized to a smaller size, the excess
	   elements are discarded.

		\param size The new size for the vector
	*/
	ID_INLINE void			SetSize( int size );

	/*!
		\brief Changes the size of the vector, optionally zeroing new elements.

		This function adjusts the size of the vector to the specified new size. If the new size is larger than the current size, it allocates additional memory and optionally zeros the new elements.
	   If the new size is smaller, it simply reduces the size. The function ensures proper memory management by using aligned allocation and copying existing elements to the new memory location. The
	   makeZero parameter controls whether newly added elements are initialized to zero.

		\param size The new size for the vector
		\param makeZero If true, newly added elements are initialized to zero
	*/
	ID_INLINE void			ChangeSize( int size, bool makeZero = false );

	//! Returns the size of the vector.
	ID_INLINE int			GetSize() const { return size; }

	/*!
		\brief Sets the data pointer and length for the vector, taking ownership of the provided data buffer.

		This function assigns the provided data buffer to the vector and updates the size accordingly. It handles memory management by freeing previously allocated memory if necessary. The function
	   assumes the data pointer is 16-byte aligned and sets all elements of the vector to zero after assignment.

		\param length The length of the vector
		\param data Pointer to the data buffer to be used
		\throws assert_16_byte_aligned if data is not 16-byte aligned
	*/
	ID_INLINE void			SetData( int length, float* data );

	/*!
		\brief Sets all elements of the vector to zero.

		This function initializes all elements of the vector to zero. It uses SIMD instructions when available to optimize the zeroing process, otherwise falling back to a standard memory set
	   operation. The function operates on the internal data array of the vector object.

	*/
	ID_INLINE void			Zero();

	/*!
		\brief Sets all elements of the vector to zero.

		The function initializes all elements of the vector to zero. It first resizes the vector to the specified length and then fills the elements with zeros. The implementation uses SIMD
	   instructions for optimization when available, otherwise falling back to a standard memory copy operation.

		\param length the number of elements to set to zero
	*/
	ID_INLINE void			Zero( int length );

	/*!
		\brief Fills the vector with random values between l and u using the provided seed for the random number generator.

		The function fills each element of the vector with a random floating-point value in the range [l, u]. The random number generator is initialized with the given seed to ensure reproducible
	   results. Each element is calculated using the formula l + random_float * (u - l), where random_float is a value between 0 and 1.

		\param seed The seed value for the random number generator
		\param l The lower bound of the random value range
		\param u The upper bound of the random value range
	*/
	ID_INLINE void			Random( int seed, float l = 0.0f, float u = 1.0f );

	/*!
		\brief Fills the vector with random values in the specified range.

		The function initializes the vector with the specified length and fills it with random floating-point values between the lower bound l and upper bound u. The random number generator is seeded
	   with the provided seed value to ensure reproducible results. Each element in the vector is set to a random value using the formula l + random_float * (u - l) where random_float is a value
	   between 0 and 1.

		\param length The number of elements in the vector
		\param seed The seed value for the random number generator
		\param l The lower bound of the random value range
		\param u The upper bound of the random value range
	*/
	ID_INLINE void			Random( int length, int seed, float l = 0.0f, float u = 1.0f );

	/*!
		\brief Negates all elements of the vector in place.

		This function negates all elements of the vector by flipping the sign bit of each element. It uses SIMD instructions when available for better performance, otherwise falling back to a simple
	   loop that multiplies each element by -1. The operation is performed in place, modifying the original vector.

	*/
	ID_INLINE void			Negate();

	/*!
		\brief Clamps all elements of the vector to the specified range.

		This function iterates through all elements of the vector and ensures that each element is within the specified minimum and maximum bounds. If an element is less than the minimum value, it is
	   set to the minimum value. If an element is greater than the maximum value, it is set to the maximum value.

		\param min The minimum value to clamp elements to
		\param max The maximum value to clamp elements to
	*/
	ID_INLINE void			Clamp( float min, float max );

	/*!
		\brief Swaps the elements at the specified indices in the vector and returns a reference to the vector.

		This function performs an in-place swap of the elements located at indices e1 and e2 within the vector. The operation is done by temporarily storing the value at index e1, then assigning the
	   value at index e2 to index e1, and finally assigning the stored value to index e2. The function returns a reference to the modified vector to allow for method chaining.

		\param e1 index of the first element to swap
		\param e2 index of the second element to swap
		\return A reference to the vector after the elements at indices e1 and e2 have been swapped
	*/
	ID_INLINE idVecX&		SwapElements( int e1, int e2 );

	/*!
		\brief Returns the Euclidean length of this vector.

		This function calculates the Euclidean norm of the vector by summing the squares of all components and taking the square root of the result. The calculation iterates through all elements of
	   the vector as defined by the size member variable.

		\return The Euclidean length of the vector as a floating-point value.
	*/
	ID_INLINE float			Length() const;

	/*!
		\brief Returns the squared length of the vector.

		This function computes the squared length of the vector by summing the squares of all its components. It is useful when comparing vector lengths without the computational cost of a square root
	   operation. The function iterates through all components of the vector and accumulates their squares to compute the result.

		\return The squared length of the vector as a float value
	*/
	ID_INLINE float			LengthSqr() const;

	/*!
		\brief Returns a normalized copy of this vector.

		This function calculates the normalized (unit) vector of the current vector by dividing each component by the vector's magnitude. It computes the inverse square root of the sum of squares of
	   all components and multiplies each component by this value. The original vector remains unchanged as a new vector is returned.

		\return A new vector that is the normalized version of this vector
	*/
	ID_INLINE idVecX		Normalize() const;

	/*!
		\brief Normalizes the vector in place and returns the inverse square root of its length.

		This function normalizes the vector by dividing each component by the vector's length. It computes the sum of squares of all components, calculates the inverse square root of this sum, and
	   then scales each component of the vector by this value. The function returns the product of the inverse square root and the sum of squares, which represents the original length of the vector.

		\return The inverse square root of the vector's length.
	*/
	ID_INLINE float			NormalizeSelf();

	//! Returns the dimension of the vector.
	ID_INLINE int			GetDimension() const;

	/*!
		\brief Adds the scaled sum of two vectors to this vector.

		This function performs an in-place addition operation where the result of scaling the sum of two input vectors is added to the current vector. The operation is performed element-wise across
	   all components of the vectors. The function asserts that all vectors have the same size to ensure memory safety and correct operation.

		\param scale The scalar value used to scale the sum of v0 and v1 before adding to this vector
		\param v0 The first vector to be added and scaled
		\param v1 The second vector to be added and scaled
		\throws assertion failure if the size of this vector does not match the sizes of v0 or v1
	*/
	ID_INLINE void			AddScaleAdd( const float scale, const idVecX& v0, const idVecX& v1 );

	/*!
		\brief Returns a constant reference to a 3D vector component of this vector at the specified index.

		This function provides access to a 3D vector component stored within a larger vector structure. It calculates the starting position of the requested 3D vector component based on the index and
	   returns a constant reference to it. The function assumes that the vector has enough elements to accommodate the requested component, as verified by an assertion.

		\param index The index of the 3D vector component to return, where each component is 3 consecutive elements.
		\return A constant reference to the 3D vector component at the specified index.
		\throws assertion failure if the index is negative or if the requested component would exceed the vector's bounds
	*/
	ID_INLINE const idVec3& SubVec3( int index ) const;

	/*!
		\brief Returns a reference to a 3D vector component of this vector object at the specified index.

		This function provides access to a 3D vector component stored within a larger vector object. It calculates the position of the component based on the provided index and returns a reference to
	   it. The function performs an assertion to ensure the index is valid within the bounds of the vector. The index is used to determine the starting position of the 3D vector component, assuming
	   each component occupies 3 consecutive elements.

		\param index The index of the 3D vector component to return a reference to.
		\return A reference to a 3D vector component of this vector object at the specified index.
		\throws assertion failure if the index is out of bounds
	*/
	ID_INLINE idVec3&		SubVec3( int index );

	/*!
		\brief Returns a constant reference to a sub-vector of type idVec6 starting at the specified index

		This function provides access to a 6-dimensional vector component within a larger vector array. It validates that the requested sub-vector fits within the bounds of the current vector and
	   returns a reference to the specified 6D vector. The function is marked as inline for performance optimization and is intended for read-only access to sub-vectors

		\param index starting index of the 6D vector within the vector array
		\return constant reference to a 6-dimensional vector at the specified index in the vector array
		\throws assertion failure if the index is negative or if the requested sub-vector extends beyond the bounds of the vector array
	*/
	ID_INLINE const idVec6& SubVec6( int index = 0 ) const;

	/*!
		\brief Returns a reference to a 6-dimensional vector at the specified index in the vector array.

		The function provides access to a 6-dimensional vector component within the vector array. It performs bounds checking to ensure the requested index is valid and that there are enough elements
	   remaining in the array to form a complete 6-dimensional vector. The function returns a reference to an idVec6 object which allows for both reading and modification of the vector data.

		\param index The zero-based index of the 6-dimensional vector to access within the array
		\return A reference to a 6-dimensional vector at the specified index
		\throws assertion failure if the index is negative or if there are insufficient elements in the array to form a complete 6-dimensional vector
	*/
	ID_INLINE idVec6&		SubVec6( int index = 0 );

	//! Returns a pointer to the internal float array representing the vector data.
	ID_INLINE const float*	ToFloatPtr() const;

	//! Returns a pointer to the float array data of the vector
	ID_INLINE float*		ToFloatPtr();

	//! Returns a string representation of the vector with the specified decimal precision
	const char*				ToString( int precision = 2 ) const;

private:
	int			   size;	// size of the vector
	int			   alloced; // if -1 p points to data set with SetData
	float*		   p;		// memory the vector is stored

	static float   temp[VECX_MAX_TEMP + 4]; // used to store intermediate results
	static float*  tempPtr;					// pointer to 16 byte aligned temporary memory
	static int	   tempIndex;				// index into memory pool, wraps around

	/*!
		\brief Sets the temporary size for the vector, allocating memory from the temporary buffer.

		This function configures the size of a temporary vector by updating the internal size and allocation parameters. It calculates the allocated size by rounding up to the nearest multiple of four
	   and ensures the allocation does not exceed the maximum temporary buffer size. If the allocation would exceed the buffer limit, the temporary index is reset to zero. The function then updates
	   the vector's pointer to point to the appropriate location in the temporary buffer and advances the temporary index.

		\param size The new size for the temporary vector
		\throws assertion failure if alloced >= VECX_MAX_TEMP
	*/
	ID_INLINE void SetTempSize( int size );
};

ID_INLINE idVecX::idVecX()
{
	size = alloced = 0;
	p			   = NULL;
}

ID_INLINE idVecX::idVecX( int length )
{
	size = alloced = 0;
	p			   = NULL;
	SetSize( length );
}

ID_INLINE idVecX::idVecX( int length, float* data )
{
	size = alloced = 0;
	p			   = NULL;
	SetData( length, data );
}

ID_INLINE idVecX::~idVecX()
{
	// if not temp memory
	if( p && ( p < idVecX::tempPtr || p >= idVecX::tempPtr + VECX_MAX_TEMP ) && alloced != -1 ) { Mem_Free16( p ); }
}

ID_INLINE float idVecX::Get( int index ) const
{
	assert( index >= 0 && index < size );
	return p[index];
}

ID_INLINE float& idVecX::Get( int index )
{
	assert( index >= 0 && index < size );
	return p[index];
}

/*
========================
idVecX::operator[]
========================
*/
ID_INLINE float idVecX::operator[]( int index ) const
{
	return Get( index );
}

/*
========================
idVecX::operator[]
========================
*/
ID_INLINE float& idVecX::operator[]( int index )
{
	return Get( index );
}

/*
========================
idVecX::operator-
========================
*/
ID_INLINE idVecX idVecX::operator-() const
{
	idVecX m;

	m.SetTempSize( size );
#ifdef VECX_SIMD
	ALIGN16( unsigned int signBit[4] ) = { IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK };
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( m.p + i, _mm_xor_ps( _mm_load_ps( p + i ), ( __m128& )signBit[0] ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		m.p[i] = -p[i];
	}
#endif
	return m;
}

/*
========================
idVecX::operator=
========================
*/
ID_INLINE idVecX& idVecX::operator=( const idVecX& a )
{
	SetSize( a.size );
#ifdef VECX_SIMD
	for( int i = 0; i < a.size; i += 4 ) {
		_mm_store_ps( p + i, _mm_load_ps( a.p + i ) );
	}
#else
	memcpy( p, a.p, a.size * sizeof( float ) );
#endif
	idVecX::tempIndex = 0;
	return *this;
}

/*
========================
idVecX::operator+
========================
*/
ID_INLINE idVecX idVecX::operator+( const idVecX& a ) const
{
	idVecX m;

	assert( size == a.size );
	m.SetTempSize( size );
#ifdef VECX_SIMD
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( m.p + i, _mm_add_ps( _mm_load_ps( p + i ), _mm_load_ps( a.p + i ) ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		m.p[i] = p[i] + a.p[i];
	}
#endif
	return m;
}

/*
========================
idVecX::operator-
========================
*/
ID_INLINE idVecX idVecX::operator-( const idVecX& a ) const
{
	idVecX m;

	assert( size == a.size );
	m.SetTempSize( size );
#ifdef VECX_SIMD
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( m.p + i, _mm_sub_ps( _mm_load_ps( p + i ), _mm_load_ps( a.p + i ) ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		m.p[i] = p[i] - a.p[i];
	}
#endif
	return m;
}

/*
========================
idVecX::operator+=
========================
*/
ID_INLINE idVecX& idVecX::operator+=( const idVecX& a )
{
	assert( size == a.size );
#ifdef VECX_SIMD
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( p + i, _mm_add_ps( _mm_load_ps( p + i ), _mm_load_ps( a.p + i ) ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		p[i] += a.p[i];
	}
#endif
	idVecX::tempIndex = 0;
	return *this;
}

/*
========================
idVecX::operator-=
========================
*/
ID_INLINE idVecX& idVecX::operator-=( const idVecX& a )
{
	assert( size == a.size );
#ifdef VECX_SIMD
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( p + i, _mm_sub_ps( _mm_load_ps( p + i ), _mm_load_ps( a.p + i ) ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		p[i] -= a.p[i];
	}
#endif
	idVecX::tempIndex = 0;
	return *this;
}

/*
========================
idVecX::operator*
========================
*/
ID_INLINE idVecX idVecX::operator*( const float a ) const
{
	idVecX m;

	m.SetTempSize( size );
#ifdef VECX_SIMD
	__m128 va = _mm_load1_ps( &a );
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( m.p + i, _mm_mul_ps( _mm_load_ps( p + i ), va ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		m.p[i] = p[i] * a;
	}
#endif
	return m;
}

/*
========================
idVecX::operator*=
========================
*/
ID_INLINE idVecX& idVecX::operator*=( const float a )
{
#ifdef VECX_SIMD
	__m128 va = _mm_load1_ps( &a );
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( p + i, _mm_mul_ps( _mm_load_ps( p + i ), va ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		p[i] *= a;
	}
#endif
	return *this;
}

/*
========================
idVecX::operator/
========================
*/
ID_INLINE idVecX idVecX::operator/( const float a ) const
{
	assert( fabs( a ) > idMath::FLT_SMALLEST_NON_DENORMAL );
	return ( *this ) * ( 1.0f / a );
}

/*
========================
idVecX::operator/=
========================
*/
ID_INLINE idVecX& idVecX::operator/=( const float a )
{
	assert( fabs( a ) > idMath::FLT_SMALLEST_NON_DENORMAL );
	( *this ) *= ( 1.0f / a );
	return *this;
}

/*
========================
operator*
========================
*/
ID_INLINE idVecX operator*( const float a, const idVecX& b )
{
	return b * a;
}

/*
========================
idVecX::operator*
========================
*/
ID_INLINE float idVecX::operator*( const idVecX& a ) const
{
	assert( size == a.size );
	float sum = 0.0f;
	for( int i = 0; i < size; i++ ) {
		sum += p[i] * a.p[i];
	}
	return sum;
}

ID_INLINE bool idVecX::Compare( const idVecX& a ) const
{
	assert( size == a.size );
	for( int i = 0; i < size; i++ ) {
		if( p[i] != a.p[i] ) { return false; }
	}
	return true;
}

ID_INLINE bool idVecX::Compare( const idVecX& a, const float epsilon ) const
{
	assert( size == a.size );
	for( int i = 0; i < size; i++ ) {
		if( idMath::Fabs( p[i] - a.p[i] ) > epsilon ) { return false; }
	}
	return true;
}

/*
========================
idVecX::operator==
========================
*/
ID_INLINE bool idVecX::operator==( const idVecX& a ) const
{
	return Compare( a );
}

/*
========================
idVecX::operator!=
========================
*/
ID_INLINE bool idVecX::operator!=( const idVecX& a ) const
{
	return !Compare( a );
}

ID_INLINE void idVecX::SetSize( int newSize )
{
	// assert( p < idVecX::tempPtr || p > idVecX::tempPtr + VECX_MAX_TEMP );
	if( newSize != size || p == NULL ) {
		int alloc = ( newSize + 3 ) & ~3;
		if( alloc > alloced && alloced != -1 ) {
			if( p ) { Mem_Free16( p ); }
			p		= ( float* )Mem_Alloc16( alloc * sizeof( float ), TAG_MATH );
			alloced = alloc;
		}
		size = newSize;
		VECX_CLEAREND();
	}
}

ID_INLINE void idVecX::ChangeSize( int newSize, bool makeZero )
{
	if( newSize != size ) {
		int alloc = ( newSize + 3 ) & ~3;
		if( alloc > alloced && alloced != -1 ) {
			float* oldVec = p;
			p			  = ( float* )Mem_Alloc16( alloc * sizeof( float ), TAG_MATH );
			alloced		  = alloc;
			if( oldVec ) {
				for( int i = 0; i < size; i++ ) {
					p[i] = oldVec[i];
				}
				Mem_Free16( oldVec );
			}
			if( makeZero ) {
				// zero any new elements
				for( int i = size; i < newSize; i++ ) {
					p[i] = 0.0f;
				}
			}
		}
		size = newSize;
		VECX_CLEAREND();
	}
}

ID_INLINE void idVecX::SetTempSize( int newSize )
{
	size	= newSize;
	alloced = ( newSize + 3 ) & ~3;
	assert( alloced < VECX_MAX_TEMP );
	if( idVecX::tempIndex + alloced > VECX_MAX_TEMP ) { idVecX::tempIndex = 0; }
	p = idVecX::tempPtr + idVecX::tempIndex;
	idVecX::tempIndex += alloced;
	VECX_CLEAREND();
}

ID_INLINE void idVecX::SetData( int length, float* data )
{
	if( p != NULL && ( p < idVecX::tempPtr || p >= idVecX::tempPtr + VECX_MAX_TEMP ) && alloced != -1 ) { Mem_Free16( p ); }
	assert_16_byte_aligned( data ); // data must be 16 byte aligned
	p		= data;
	size	= length;
	alloced = -1;
	VECX_CLEAREND();
}

ID_INLINE void idVecX::Zero()
{
#ifdef VECX_SIMD
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( p + i, _mm_setzero_ps() );
	}
#else
	memset( p, 0, size * sizeof( float ) );
#endif
}

ID_INLINE void idVecX::Zero( int length )
{
	SetSize( length );
#ifdef VECX_SIMD
	for( int i = 0; i < length; i += 4 ) {
		_mm_store_ps( p + i, _mm_setzero_ps() );
	}
#else
	memset( p, 0, length * sizeof( float ) );
#endif
}

ID_INLINE void idVecX::Random( int seed, float l, float u )
{
	idRandom rnd( seed );

	float	 c = u - l;
	for( int i = 0; i < size; i++ ) {
		p[i] = l + rnd.RandomFloat() * c;
	}
}

ID_INLINE void idVecX::Random( int length, int seed, float l, float u )
{
	idRandom rnd( seed );

	SetSize( length );
	float c = u - l;
	for( int i = 0; i < size; i++ ) {
		p[i] = l + rnd.RandomFloat() * c;
	}
}

ID_INLINE void idVecX::Negate()
{
#ifdef VECX_SIMD
	ALIGN16( const unsigned int signBit[4] ) = { IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK };
	for( int i = 0; i < size; i += 4 ) {
		_mm_store_ps( p + i, _mm_xor_ps( _mm_load_ps( p + i ), ( __m128& )signBit[0] ) );
	}
#else
	for( int i = 0; i < size; i++ ) {
		p[i] = -p[i];
	}
#endif
}

ID_INLINE void idVecX::Clamp( float min, float max )
{
	for( int i = 0; i < size; i++ ) {
		if( p[i] < min ) {
			p[i] = min;
		} else if( p[i] > max ) {
			p[i] = max;
		}
	}
}

ID_INLINE idVecX& idVecX::SwapElements( int e1, int e2 )
{
	float tmp;
	tmp	  = p[e1];
	p[e1] = p[e2];
	p[e2] = tmp;
	return *this;
}

ID_INLINE float idVecX::Length() const
{
	float sum = 0.0f;
	for( int i = 0; i < size; i++ ) {
		sum += p[i] * p[i];
	}
	return idMath::Sqrt( sum );
}

ID_INLINE float idVecX::LengthSqr() const
{
	float sum = 0.0f;
	for( int i = 0; i < size; i++ ) {
		sum += p[i] * p[i];
	}
	return sum;
}

ID_INLINE idVecX idVecX::Normalize() const
{
	idVecX m;

	m.SetTempSize( size );
	float sum = 0.0f;
	for( int i = 0; i < size; i++ ) {
		sum += p[i] * p[i];
	}
	float invSqrt = idMath::InvSqrt( sum );
	for( int i = 0; i < size; i++ ) {
		m.p[i] = p[i] * invSqrt;
	}
	return m;
}

ID_INLINE float idVecX::NormalizeSelf()
{
	float sum = 0.0f;
	for( int i = 0; i < size; i++ ) {
		sum += p[i] * p[i];
	}
	float invSqrt = idMath::InvSqrt( sum );
	for( int i = 0; i < size; i++ ) {
		p[i] *= invSqrt;
	}
	return invSqrt * sum;
}

ID_INLINE int idVecX::GetDimension() const
{
	return size;
}

ID_INLINE idVec3& idVecX::SubVec3( int index )
{
	assert( index >= 0 && index * 3 + 3 <= size );
	return *reinterpret_cast<idVec3*>( p + index * 3 );
}

ID_INLINE const idVec3& idVecX::SubVec3( int index ) const
{
	assert( index >= 0 && index * 3 + 3 <= size );
	return *reinterpret_cast<const idVec3*>( p + index * 3 );
}

ID_INLINE idVec6& idVecX::SubVec6( int index )
{
	assert( index >= 0 && index * 6 + 6 <= size );
	return *reinterpret_cast<idVec6*>( p + index * 6 );
}

ID_INLINE const idVec6& idVecX::SubVec6( int index ) const
{
	assert( index >= 0 && index * 6 + 6 <= size );
	return *reinterpret_cast<const idVec6*>( p + index * 6 );
}

ID_INLINE const float* idVecX::ToFloatPtr() const
{
	return p;
}

ID_INLINE float* idVecX::ToFloatPtr()
{
	return p;
}

ID_INLINE void idVecX::AddScaleAdd( const float scale, const idVecX& v0, const idVecX& v1 )
{
	assert( GetSize() == v0.GetSize() );
	assert( GetSize() == v1.GetSize() );

	const float* v0Ptr	= v0.ToFloatPtr();
	const float* v1Ptr	= v1.ToFloatPtr();
	float*		 dstPtr = ToFloatPtr();

	for( int i = 0; i < size; i++ ) {
		dstPtr[i] += scale * ( v0Ptr[i] + v1Ptr[i] );
	}
}

#endif // !__MATH_VECTORX_H__
