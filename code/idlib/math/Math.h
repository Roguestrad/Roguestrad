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

#ifndef __MATH_MATH_H__
#define __MATH_MATH_H__

class idVec2;
class idVec3;
class idVec4;

/*
===============================================================================

  Math

===============================================================================
*/

#ifdef INFINITUM
	#undef INFINITUM
#endif

#ifdef FLOAT_EPSILON
	#undef FLOAT_EPSILON
#endif

#define DEG2RAD( a )		( ( a ) * idMath::M_DEG2RAD )
#define RAD2DEG( a )		( ( a ) * idMath::M_RAD2DEG )

#define SEC2MS( t )			( idMath::Ftoi( ( t ) * idMath::M_SEC2MS ) )
#define MS2SEC( t )			( ( t ) * idMath::M_MS2SEC )

#define ANGLE2SHORT( x )	( idMath::Ftoi( ( x ) * 65536.0f / 360.0f ) & 65535 )
#define SHORT2ANGLE( x )	( ( x ) * ( 360.0f / 65536.0f ) )

#define ANGLE2BYTE( x )		( idMath::Ftoi( ( x ) * 256.0f / 360.0f ) & 255 )
#define BYTE2ANGLE( x )		( ( x ) * ( 360.0f / 256.0f ) )

#define C_FLOAT_TO_INT( x ) ( int )( x )

/*
================================================================================================

	two-complements integer bit layouts

================================================================================================
*/

#define INT8_SIGN_BIT		7
#define INT16_SIGN_BIT		15
#define INT32_SIGN_BIT		31
#define INT64_SIGN_BIT		63

#define INT8_SIGN_MASK		( 1 << INT8_SIGN_BIT )
#define INT16_SIGN_MASK		( 1 << INT16_SIGN_BIT )
#define INT32_SIGN_MASK		( 1UL << INT32_SIGN_BIT )
#define INT64_SIGN_MASK		( 1ULL << INT64_SIGN_BIT )

/*
================================================================================================

	integer sign bit tests

================================================================================================
*/

// If this was ever compiled on a system that had 64 bit unsigned ints,
// it would fail.
compile_time_assert( sizeof( unsigned int ) == 4 );

#define OLD_INT32_SIGNBITSET( i )	 ( static_cast<const unsigned int>( i ) >> INT32_SIGN_BIT )
#define OLD_INT32_SIGNBITNOTSET( i ) ( ( ~static_cast<const unsigned int>( i ) ) >> INT32_SIGN_BIT )

/*!
	\brief Returns the sign bit of a 32-bit integer as either 0 or 1

	This function extracts the sign bit from a 32-bit integer and returns it as either 0 or 1. The implementation uses a helper macro OLD_INT32_SIGNBITSET to perform the actual bit manipulation,
   followed by an assertion to ensure the result is either 0 or 1. The function is marked as inline extern, indicating it's intended to be inlined for performance.

	\param i The 32-bit integer from which to extract the sign bit
	\return 0 if the input integer is non-negative, 1 if the input integer is negative
	\throws assertion failure if the result is not 0 or 1
*/
ID_INLINE_EXTERN int INT32_SIGNBITSET( int i )
{
	int r = OLD_INT32_SIGNBITSET( i );
	assert( r == 0 || r == 1 );
	return r;
}

/*!
	\brief Returns 1 if the sign bit of the input integer is not set, otherwise returns 0

	This function checks the sign bit of a 32-bit integer and returns 1 if the sign bit is not set (indicating a non-negative number), or 0 if the sign bit is set (indicating a negative number). The
   function is inlined and includes an assertion to ensure the return value is either 0 or 1. It is commonly used in collision detection code where the sign bit is used to determine vertex ordering
   for polygon edges.

	\param i The 32-bit integer to check the sign bit of
	\return 1 if the sign bit of the input integer is not set (non-negative), 0 if the sign bit is set (negative)
	\throws assertion failure if the return value is not 0 or 1
*/
ID_INLINE_EXTERN int INT32_SIGNBITNOTSET( int i )
{
	int r = OLD_INT32_SIGNBITNOTSET( i );
	assert( r == 0 || r == 1 );
	return r;
}

/*
================================================================================================

	floating point bit layouts according to the IEEE 754-1985 and 754-2008 standard

================================================================================================
*/

#define IEEE_FLT16_MANTISSA_BITS	10
#define IEEE_FLT16_EXPONENT_BITS	5
#define IEEE_FLT16_EXPONENT_BIAS	15
#define IEEE_FLT16_SIGN_BIT			15
#define IEEE_FLT16_SIGN_MASK		( 1U << IEEE_FLT16_SIGN_BIT )

#define IEEE_FLT_MANTISSA_BITS		23
#define IEEE_FLT_EXPONENT_BITS		8
#define IEEE_FLT_EXPONENT_BIAS		127
#define IEEE_FLT_SIGN_BIT			31
#define IEEE_FLT_SIGN_MASK			( 1UL << IEEE_FLT_SIGN_BIT )

#define IEEE_DBL_MANTISSA_BITS		52
#define IEEE_DBL_EXPONENT_BITS		11
#define IEEE_DBL_EXPONENT_BIAS		1023
#define IEEE_DBL_SIGN_BIT			63
#define IEEE_DBL_SIGN_MASK			( 1ULL << IEEE_DBL_SIGN_BIT )

#define IEEE_DBLE_MANTISSA_BITS		63
#define IEEE_DBLE_EXPONENT_BITS		15
#define IEEE_DBLE_EXPONENT_BIAS		0
#define IEEE_DBLE_SIGN_BIT			79

/*
================================================================================================

	floating point sign bit tests

================================================================================================
*/

#define IEEE_FLT_SIGNBITSET( a )	( reinterpret_cast<const unsigned int&>( a ) >> IEEE_FLT_SIGN_BIT )
#define IEEE_FLT_SIGNBITNOTSET( a ) ( ( ~reinterpret_cast<const unsigned int&>( a ) ) >> IEEE_FLT_SIGN_BIT )
#define IEEE_FLT_ISNOTZERO( a )		( reinterpret_cast<const unsigned int&>( a ) & ~( 1u << IEEE_FLT_SIGN_BIT ) )

/*!
	\brief Checks if a floating-point value is not a number (NaN).

	This function determines whether a given floating-point number is Not a Number (NaN) by exploiting the property that NaN is the only value that is not equal to itself. When a floating-point number
   is NaN, the comparison x != x will return true, whereas for any valid number, x == x will be true.

	\param x The floating-point number to check for NaN
	\return true if the input value is NaN, false otherwise
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_NAN( float x )
{
	return x != x;
}

/*!
	\brief Checks if a floating-point value is infinite.

	This function determines whether a given floating-point number is infinite by using a specific bitwise comparison technique. It returns true if the input value is positive or negative infinity,
   and false otherwise. The implementation relies on the property that infinity comparisons behave differently than normal numbers.

	\param x The floating-point value to check for infinity
	\return True if the input value is infinite, false otherwise
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_INF( float x )
{
	return x == x && x * 0 != x * 0;
}

/*!
	\brief Checks if a floating-point value is infinite or not a number.

	This function determines whether the given floating-point number is either positive infinity, negative infinity, or not a number (NaN). It does so by leveraging the property that any comparison
   involving NaN will return false, and infinity operations maintain their identity when compared to themselves.

	\param x The floating-point value to check
	\return True if the value is infinite or NaN, false otherwise
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_INF_NAN( float x )
{
	return x * 0 != x * 0;
}

/*!
	\brief Checks if a floating-point value is an indentation error according to IEEE 754 standards

	This function determines whether a given floating-point number represents an invalid or infinite value according to the IEEE 754 floating-point standard. It does so by examining the bit
   representation of the float and comparing it against the specific bit pattern that denotes an indefinite value.

	\param x The floating-point value to check
	\return True if the floating-point value is an indefinite value, false otherwise
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_IND( float x )
{
	return ( reinterpret_cast<const unsigned int&>( x ) == 0xffc00000 );
}

/*!
	\brief Checks if a floating-point value is a denormalized number.

	This function determines whether a given floating-point number is denormalized, meaning it has a zero exponent field but a non-zero mantissa, which results in a very small number close to zero.
   The function performs a bitwise check on the IEEE 754 representation of the float to identify this special case.

	\param x The floating-point value to check for denormalization
	\return True if the input value is denormalized, false otherwise
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_DENORMAL( float x )
{
	return ( ( reinterpret_cast<const unsigned int&>( x ) & 0x7f800000 ) == 0x00000000 && ( reinterpret_cast<const unsigned int&>( x ) & 0x007fffff ) != 0x00000000 );
}

/*!
	\brief Checks if any component of the input vector contains NaN, infinity, or indeterminate values

	This function iterates through all components of the input vector and checks each floating-point value for invalid states including NaN, infinity, and indeterminate values. It returns true as soon
   as any invalid value is found, making it useful for validation purposes in graphics and rendering operations where data corruption needs to be detected

	\param v input vector to check for invalid floating-point values
	\return true if any component of the vector contains NaN, infinity, or indeterminate values, false otherwise
*/
template<class type>
ID_INLINE_EXTERN bool IsNAN( const type& v )
{
	for( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) ) { return true; }
	}
	return false;
}

/*!
	\brief Checks if all components of a vector are valid floating-point values

	This function validates each component of a vector by checking whether it is a valid floating-point number. It examines each element in the vector and returns false if any element is NaN,
   infinity, indeterminate, or denormal. The function iterates through all dimensions of the vector and performs IEEE floating-point validity checks on each value.

	\param v A constant reference to a vector object with a GetDimension() method and ToFloatPtr() method
	\return True if all components of the vector are valid floating-point values, false otherwise
*/
template<class type>
ID_INLINE_EXTERN bool IsValid( const type& v )
{
	for( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) || IEEE_FLT_IS_DENORMAL( f ) ) { return false; }
	}
	return true;
}

/*!
	\brief Checks if a floating-point value is valid, meaning it is not NaN, infinity, indeterminate, or denormalized

	This function determines whether a given floating-point value is valid by checking against several invalid states. It returns false if the value is NaN, infinity, indeterminate, or denormalized,
   and true otherwise. This is useful for validating floating-point inputs in mathematical operations or data processing where such invalid values could cause errors or unexpected behavior.

	\param f The floating-point value to check for validity
	\return True if the floating-point value is valid, false otherwise
*/
template<>
ID_INLINE bool IsValid( const float& f )
{
	return !( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) || IEEE_FLT_IS_DENORMAL( f ) );
}

/*!
	\brief Checks if a floating-point value is NaN, infinity, or indeterminate.

	This function determines whether a given floating-point value is not a number (NaN), infinity, or indeterminate. It is used to validate floating-point data and ensure that invalid values are
   detected and handled appropriately. The function returns true if the value is any of these invalid states, and false otherwise.

	\param f The floating-point value to check for invalid states
	\return True if the floating-point value is NaN, infinity, or indeterminate; false otherwise
*/
template<>
ID_INLINE bool IsNAN( const float& f )
{
	if( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) ) { return true; }
	return false;
}

/*!
	\brief Checks if all scalar components of the input vector are within the specified range

	Returns true if all scalar components of the input vector are within the specified range, false otherwise

	\param v input vector to check
	\param range maximum absolute value allowed for each component
	\return true if all scalar components of the input vector are within the specified range, false otherwise
*/
template<class type>
ID_INLINE bool IsInRange( const type& v, const float range )
{
	for( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if( f > range || f < -range ) { return false; }
	}
	return true;
}

/*!
	\brief Returns 0 if x is greater than y, otherwise returns 1

	This function compares two values of the same type and returns an index based on their relationship. It is typically used to determine which of two values is larger, returning 0 for the first
   value when it is greater, and 1 when the second value is greater or equal.

	\param x First value to compare
	\param y Second value to compare
	\return 0 if x is greater than y, otherwise 1
*/
template<class T>
ID_INLINE int MaxIndex( T x, T y )
{
	return ( x > y ) ? 0 : 1;
}

/*!
	\brief Returns the index of the smaller of two values.

	This function takes two values of type T and returns 0 if the first value is less than the second, otherwise it returns 1. It is useful for determining which of two values is smaller and returning
   a corresponding index.

	\param x First value for comparison
	\param y Second value for comparison
	\return 0 if x is less than y, 1 otherwise
*/
template<class T>
ID_INLINE int MinIndex( T x, T y )
{
	return ( x < y ) ? 0 : 1;
}

/*!
	\brief Returns the maximum value among three input values of type T.

	This function takes three template parameters of the same type T and returns the largest of the three values. It uses a ternary operator to perform the comparison in a nested fashion, first
   comparing x and y, then comparing the result with z to determine the maximum value.

	\param x First value to compare
	\param y Second value to compare
	\param z Third value to compare
	\return The maximum value among the three input parameters
*/
template<class T>
ID_INLINE T Max3( T x, T y, T z )
{
	return ( x > y ) ? ( ( x > z ) ? x : z ) : ( ( y > z ) ? y : z );
}

/*!
	\brief Returns the smallest of three values.

	This function takes three values of the same type and returns the smallest one using a simple comparison logic. It uses a ternary operator to perform the comparisons and determine the minimum
   value among the three inputs.

	\param x First value to compare
	\param y Second value to compare
	\param z Third value to compare
	\return The smallest of the three input values
*/
template<class T>
ID_INLINE T Min3( T x, T y, T z )
{
	return ( x < y ) ? ( ( x < z ) ? x : z ) : ( ( y < z ) ? y : z );
}

/*!
	\brief Returns the index of the maximum value among three input values.

	This function takes three values of the same type and determines which one is the largest. It returns 0 if the first value is the maximum, 1 if the second value is the maximum, and 2 if the third
   value is the maximum. The comparison is performed using the greater than operator.

	\param x First value to compare
	\param y Second value to compare
	\param z Third value to compare
	\return Index of the maximum value among the three inputs, where 0 corresponds to x, 1 to y, and 2 to z
*/
template<class T>
ID_INLINE int Max3Index( T x, T y, T z )
{
	return ( x > y ) ? ( ( x > z ) ? 0 : 2 ) : ( ( y > z ) ? 1 : 2 );
}

/*!
	\brief Returns the index of the smallest of three values.

	This function determines the index of the smallest value among three input parameters. It uses a ternary operator to compare the values and return 0, 1, or 2 based on which value is the smallest.
   The function is implemented as an inline function for performance optimization.

	\param x First value to compare
	\param y Second value to compare
	\param z Third value to compare
	\return Index 0, 1, or 2 corresponding to the smallest of the three input values x, y, or z respectively
*/
template<class T>
ID_INLINE int Min3Index( T x, T y, T z )
{
	return ( x < y ) ? ( ( x < z ) ? 0 : 2 ) : ( ( y < z ) ? 1 : 2 );
}

/*!
	\brief Returns the sign of the input value as -1, 0, or 1.

	This function determines the sign of a given numeric value and returns -1 if the value is negative, 1 if the value is positive, and 0 if the value is exactly zero.

	\param f The input numeric value to evaluate
	\return -1 if the input is negative, 1 if the input is positive, or 0 if the input is zero
*/
template<class T>
ID_INLINE T Sign( T f )
{
	return ( f > 0 ) ? 1 : ( ( f < 0 ) ? -1 : 0 );
}

/*!
	\brief Returns the square of the input value.

	This inline function computes the square of a given input value by multiplying it with itself. It is a generic template function that can work with any numeric type T.

	\param x The input value to be squared
	\return The square of the input value x
*/
template<class T>
ID_INLINE T Square( T x )
{
	return x * x;
}

/*!
	\brief Computes the cube of the given value.

	This function takes a single input value and returns its cube by multiplying the value by itself three times. It is a template function that works with any numeric type T.

	\param x The input value to be cubed
	\return The cube of the input value x
*/
template<class T>
ID_INLINE T Cube( T x )
{
	return x * x * x;
}

class idMath
{
public:
	/*!
		\brief Initializes the math module's lookup tables and state

		This function initializes the math module by computing lookup tables for fast square root calculations. It populates the iSqrt array with precomputed values used for efficient floating-point
	   square root operations. The initialization process sets up a lookup table based on bit manipulation and floating-point arithmetic to achieve fast computation of inverse square roots. The
	   function also marks the module as initialized once complete.

	*/
	static void			  Init();

	/*!
		\brief Computes the angle modulo 360 degrees.

		This function takes an angle value and wraps it to the range [0, 360) degrees. It uses a bit-wise AND operation with 65535 to achieve this wrapping, which is a common optimization technique in
	   game engines for angle normalization. The implementation approximates the reciprocal square root calculation but is primarily used for angle modulation.

		\param a The input angle in degrees to be normalized.
		\return The normalized angle value in the range [0, 360) degrees.
	*/
	static float		  AngleMod( float a );

	/*!
		\brief Computes an approximation of the reciprocal square root of the given float value.

		This function implements the classic fast reciprocal square root algorithm, commonly known as the "Quake III algorithm". It uses bit-level manipulation and Newton-Raphson iteration to quickly
	   approximate 1/sqrt(x). The algorithm works by first converting the float to its bit representation, performing a bit shift and subtraction, then converting back to float. A single
	   Newton-Raphson iteration is applied to refine the approximation.

		\param x the input float value for which to compute the reciprocal square root
		\return the approximated reciprocal square root of the input value x
	*/
	static float		  RSqrt( float x );

	//! Computes the cross product of two 3D vectors.
	static idVec3		  CrossProduct( const idVec3& a, const idVec3& b );

	/*!
		\brief Computes the squared Euclidean distance between two 3D vectors.

		This function calculates the squared distance between two 3D points represented as idVec3 vectors. It is useful when comparing distances without needing to perform a square root operation,
	   which can improve performance. The result is the square of the Euclidean distance between the two points.

		\param p1 The first 3D vector point
		\param p2 The second 3D vector point
		\return The squared Euclidean distance between the two input vectors
	*/
	static float		  DistanceSquared( idVec3 p1, idVec3 p2 );

	/*!
		\brief Calculates the Euclidean distance between two 3D points.

		This function computes the distance between two points in three-dimensional space by subtracting the first point from the second and then calculating the length of the resulting vector. The
	   calculation uses the standard Euclidean distance formula.

		\param p1 The first 3D point
		\param p2 The second 3D point
		\return The Euclidean distance between the two input points
	*/
	static float		  Distance( idVec3 p1, idVec3 p2 );

	/*!
		\brief Returns the reflection vector of the given vector off a surface with the specified normal.

		This function calculates the reflection vector using the standard reflection formula. It takes the input vector and reflects it across the provided normal vector, which represents the surface
	   normal at the point of reflection. The calculation uses the dot product to determine the projection of the vector onto the normal, then scales the normal by twice that value and subtracts it
	   from the original vector.

		\param vector The incident vector to be reflected
		\param normal The normal vector of the surface at the point of reflection
		\return The resulting reflection vector after applying the reflection formula
	*/
	static idVec3		  ReflectVector( idVec3 vector, idVec3 normal );

	//! Creates and returns a new idVec4 vector with the specified x, y, z, and w components.
	static idVec4		  CreateVector( float x, float y, float z, float w );

	//! Creates and returns a new idVec3 object with the specified x, y, and z coordinates.
	static idVec3		  CreateVector( float x, float y, float z );

	/*!
		\brief Returns a pseudo-random integer.

		The function generates and returns a pseudo-random integer using the standard library rand() function. The generated integer is typically within the range of 0 to RAND_MAX, though the exact
	   range depends on the implementation of the standard library.

		\return A pseudo-random integer value.
	*/
	static int			  Rand() { return rand(); }

	/*!
		\brief Returns a random floating-point number in the range [0, 1).

		This function generates a random float value between 0 and 1 by dividing the result of Rand() by the maximum value of RAND_MAX. The result is a floating-point number in the inclusive range [0,
	   1).

		\return A random floating-point number in the range [0, 1)
	*/
	static float		  FRand() { return Rand() / ( float )RAND_MAX; }

	/*!
		\brief Returns a random floating-point number in the range [min, max).

		The function generates a random floating-point number within the specified range using the FRand() function. It computes the result as min plus the product of the range (max - min) and a
	   random value between 0 and 1.

		\param min The minimum value of the range (inclusive)
		\param max The maximum value of the range (exclusive)
		\return A random floating-point number in the range [min, max)
	*/
	static float		  FRandRange( float min, float max ) { return min + ( max - min ) * FRand(); }

	/*!
		\brief Computes the inverse square root of the given floating-point value.

		The function calculates the inverse square root of the input value x. For values greater than the smallest non-denormal floating-point number, it returns the square root of the reciprocal of
	   x. For x less than or equal to the smallest non-denormal number, it returns infinity to handle the case where the input is effectively zero or too small to compute a meaningful inverse square
	   root.

		\param x The floating-point value to compute the inverse square root of
		\return The inverse square root of x, or infinity if x is less than or equal to the smallest non-denormal floating-point number
	*/
	static float		  InvSqrt( float x );

	/*!
		\brief Computes the inverse square root of a floating-point number with 16 bits precision, returning a large value when the input is zero.

		This function calculates the inverse square root of the input value x. When x is greater than the smallest non-denormal floating-point number, it computes the square root of 1.0 divided by x.
	   Otherwise, it returns the INFINITUM value, which represents a very large number.

		\param x The floating-point number to compute the inverse square root of
		\return The inverse square root of x, or INFINITUM if x is less than or equal to FLT_SMALLEST_NON_DENORMAL
	*/
	static float		  InvSqrt16( float x );

	/*!
		\brief Computes the square root of a floating-point value with 32-bit precision.

		This function calculates the square root of the input value x. It uses the inverse square root method for computation and returns zero for negative inputs. The implementation is optimized for
	   performance and is designed to work with 32-bit floating-point numbers.

		\param x The floating-point value to compute the square root of
		\return The square root of x if x is non-negative, otherwise zero
	*/
	static float		  Sqrt( float x );

	/*!
		\brief Computes the square root of a floating-point number with 16-bit precision.

		This function calculates the square root of the input value x using the InvSqrt16 function for optimization. If the input is negative, the function returns zero. The implementation is designed
	   for 16-bit precision accuracy.

		\param x The floating-point number to compute the square root of
		\return The square root of the input value x, or zero if x is negative
	*/
	static float		  Sqrt16( float x );

	//! Computes the sine of the given angle in radians with 32-bit precision.
	static float		  Sin( float a );

	/*!
		\brief Computes the sine of a float value using a 16-bit precision approximation

		This function calculates the sine of the input angle using a polynomial approximation optimized for 16-bit precision. The input angle is first normalized to the range [0, 2π) to ensure correct
	   results. The implementation uses a piecewise approach to handle different quadrants of the unit circle, followed by a polynomial approximation for the sine calculation. The function is designed
	   for performance in graphics and game engine applications where high precision is not required but speed is important.

		\param a The input angle in radians
		\return The sine of the input angle as a floating-point value
	*/
	static float		  Sin16( float a );

	//! Computes the cosine of the given angle using 32-bit precision.
	static float		  Cos( float a );

	/*!
		\brief Computes the cosine of a floating-point value with 16 bits precision

		This function calculates the cosine of the given angle using a polynomial approximation. It first normalizes the input angle to the range [0, 2π) and then uses symmetry properties to reduce
	   the computation to a smaller range. The result is computed using a Taylor series expansion for cosine with coefficients optimized for 16-bit precision.

		\param a the angle in radians
		\return the cosine value of the input angle
	*/
	static float		  Cos16( float a );

	/*!
		\brief Computes the sine and cosine of the given angle with 32-bit precision.

		This function calculates both the sine and cosine of a specified angle using either inline assembly for Microsoft Visual C++ on x86 platforms or the standard libm functions for other
	   compilers. The results are stored in the provided output parameters.

		\param a The angle in radians
		\param s Output parameter for the sine value
		\param c Output parameter for the cosine value
	*/
	static void			  SinCos( float a, float& s, float& c );

	/*!
		\brief Computes sine and cosine values with 16-bit precision for a given angle.

		This function calculates sine and cosine values for the provided angle using a polynomial approximation method optimized for 16-bit precision. The input angle is normalized to the range [0,
	   2π) before computation. The function uses a series of conditional checks to map the angle into the first quadrant for calculation, and then adjusts the results based on the original quadrant to
	   maintain correct signs. The sine and cosine values are computed using precomputed polynomial coefficients to achieve high precision with minimal computational overhead.

		\param a The input angle in radians
		\param s Output parameter for the sine value
		\param c Output parameter for the cosine value
	*/
	static void			  SinCos16( float a, float& s, float& c );

	//! Computes the tangent of the given angle in radians with 32-bit precision.
	static float		  Tan( float a );

	/*!
		\brief Computes the tangent of a value with 16 bits precision using a polynomial approximation.

		This function calculates the tangent of the input angle a using a polynomial approximation with 16 bits of precision. The input angle is first normalized to the range [0, PI) to handle angles
	   outside this range. The function uses a series of conditional checks to determine whether to compute the tangent directly or its reciprocal, depending on the normalized angle value. The
	   computation uses a polynomial expansion to approximate the tangent value, which is then returned as either the tangent or its reciprocal based on the computed flag.

		\param a The input angle in radians
		\return The tangent of the input angle a with 16 bits precision
	*/
	static float		  Tan16( float a );

	/*!
		\brief Computes the arc sine of the input value with clamping to the range [-1, 1].

		This function calculates the arc sine (inverse sine) of the given floating-point value. If the input value is less than or equal to -1.0, it returns -HALF_PI. If the input value is greater
	   than or equal to 1.0, it returns HALF_PI. For values within the range (-1, 1), it computes the arc sine using the standard math library function.

		\param a The input value for which to compute the arc sine, typically expected to be in the range [-1, 1]
		\return The arc sine of the input value in radians, clamped to the range [-HALF_PI, HALF_PI]
	*/
	static float		  ASin( float a );

	/*!
		\brief Computes the arc sine of a floating-point value with 16 bits precision

		This function calculates the arc sine of the input value using a polynomial approximation with specific coefficients for improved accuracy. The input value is clamped to the range [-1, 1] to
	   ensure valid results. For negative inputs, the function applies a transformation using the square root of (1 - a) and adjusts the result by subtracting HALF_PI. For positive inputs, it computes
	   the result by subtracting the transformed value from HALF_PI. The approximation uses a cubic polynomial for the core calculation with carefully chosen coefficients to maintain precision at the
	   cost of computational efficiency

		\param a input floating-point value to compute the arc sine for, clamped to range [-1, 1]
		\return the arc sine of the input value in radians
	*/
	static float		  ASin16( float a );

	/*!
		\brief Computes the arc cosine of the given value with input clamped to the range [-1, 1].

		The function calculates the arc cosine of the input value, ensuring the input is clamped between -1 and 1. When the input is less than or equal to -1, it returns PI. When the input is greater
	   than or equal to 1, it returns 0. For values in between, it uses the standard acosf function to compute the result.

		\param a The input value for which to compute the arc cosine, clamped to the range [-1, 1]
		\return The arc cosine of the input value, returned as a float in the range [0, PI].
	*/
	static float		  ACos( float a );

	/*!
		\brief Computes the arc cosine of a value with 16 bits precision, returning a float in the range [0, PI].

		This function calculates the arc cosine of the input value using a polynomial approximation for improved performance. The implementation handles both positive and negative input values, with
	   special cases for values outside the valid range [-1, 1]. For values less than 0, the function uses a modified approximation that incorporates the absolute value and adjusts the result
	   accordingly. For values greater than or equal to 0, it uses a direct approximation. The function utilizes the idMath::Sqrt function for computing the square root component of the approximation.

		\param a The input value for which to compute the arc cosine, should be in the range [-1, 1]
		\return The arc cosine of the input value in radians, within the range [0, PI]
	*/
	static float		  ACos16( float a );

	//! Computes the arc tangent of the given value with 32-bit precision.
	static float		  ATan( float a );

	/*!
		\brief Computes the arc tangent of the input value with 16 bits precision using a polynomial approximation.

		This function calculates the arc tangent of the input value using a rational polynomial approximation optimized for 16-bit precision. It handles both cases where the absolute value of the
	   input is greater than 1 and less than or equal to 1. For inputs with absolute values greater than 1, it applies a transformation to improve numerical stability and accuracy. The implementation
	   uses a series of polynomial coefficients to compute the result efficiently.

		\param a The input value for which to compute the arc tangent
		\return The arc tangent of the input value in radians
	*/
	static float		  ATan16( float a );

	//! Computes the arc tangent of y/x using the standard atan2f function.
	static float		  ATan( float y, float x );

	/*!
		\brief Computes the arc tangent with 16 bits precision, returning the angle in radians

		This function calculates the arc tangent of y/x with 16 bits precision. It uses a polynomial approximation method to compute the result. The function handles both cases where the absolute
	   value of y is greater than or less than the absolute value of x. It asserts that at least one of the inputs is not a denormalized floating-point number to avoid numerical issues

		\param y The y coordinate for the arc tangent calculation
		\param x The x coordinate for the arc tangent calculation
		\return The arc tangent of y/x in radians with 16 bits precision
		\throws Assertion failure if both y and x are denormalized floating-point numbers
	*/
	static float		  ATan16( float y, float x );

	//! Computes x raised to the power y with 32-bit precision.
	static float		  Pow( float x, float y );

	//! Computes x raised to the power y with 16 bits precision.
	static float		  Pow16( float x, float y );

	//! Computes e raised to the power of the given floating-point value with 32-bit precision.
	static float		  Exp( float f );

	/*!
		\brief Calculates e raised to the power of the given float value with 16 bits precision

		This function computes the exponential function e^f using a fast approximation method. It uses bit manipulation to quickly extract the exponent from the floating-point representation and then
	   applies polynomial approximation to refine the result. The implementation uses a combination of IEEE floating-point bit manipulation and rational polynomial approximation to achieve good
	   accuracy with 16 bits precision. The function handles both positive and negative input values correctly, with special handling for the square root of 2 adjustment when the fractional part is
	   greater than or equal to 0.5

		\param f the exponent value to compute e^f
		\return the exponential value e raised to the power of f
	*/
	static float		  Exp16( float f );

	//! Computes the natural logarithm of the given floating-point value.
	static float		  Log( float f );

	/*!
		\brief Computes the natural logarithm of a floating-point value with 16 bits precision

		This function calculates the natural logarithm of the input floating-point number using a fast approximation method. It employs bit manipulation to extract the exponent and mantissa from the
	   IEEE 754 floating-point representation. The algorithm then applies a polynomial approximation to compute the logarithm with reduced precision but high performance. The method is optimized for
	   speed rather than maximum accuracy, making it suitable for applications where computational efficiency is more important than absolute precision.

		\param f the floating-point value to compute the natural logarithm of
		\return the natural logarithm of the input floating-point value
	*/
	static float		  Log16( float f );

	/*!
		\brief Computes the integral power of x raised to the power y.

		This function calculates the result of raising an integer base x to an integer exponent y. It uses iterative multiplication to compute the result, starting with r initialized to x and then
	   multiplying by x for (y-1) times. The function assumes that y is a positive integer greater than 0.

		\param x the base integer value
		\param y the exponent integer value
		\return the result of x raised to the power of y as an integer
	*/
	static int			  IPow( int x, int y );

	/*!
		\brief Returns the integral base-2 logarithm of the given floating point value by extracting the exponent bits from its IEEE 754 representation.

		This function computes the base-2 logarithm of a floating-point number by directly accessing the bit representation of the float according to the IEEE 754 standard. It extracts the exponent
	   field, adjusts for the bias, and returns the resulting integer value. The function is designed to work with normalized floating-point numbers and should be used with care when dealing with
	   special cases such as zero, infinity, or denormal numbers.

		\param f The floating point value for which to compute the base-2 logarithm
		\return The integral base-2 logarithm of the input floating point value
	*/
	static int			  ILog2( float f );

	//! Computes the integral base-2 logarithm of an integer value.
	static int			  ILog2( int i );

	/*!
		\brief Returns the minimum number of bits required to represent the ceiling of the given floating-point value.

		This function calculates the number of bits needed to represent the ceiling of a floating-point number. It uses the ILog2 function to determine the logarithm base 2 of the floating-point value
	   and adds 1 to account for the ceiling operation. The result indicates the minimum bit width required for representation.

		\param f The floating-point value for which to calculate the minimum number of bits
		\return The minimum number of bits required to represent the ceiling of the given floating-point value
	*/
	static int			  BitsForFloat( float f );

	/*!
		\brief Returns the minimum number of bits required to represent the given integer value.
		\param i The integer value for which to calculate the number of bits required.
		\return The minimum number of bits required to represent the given integer value.
	*/
	static int			  BitsForInteger( int i );

	/*!
		\brief Returns a mask indicating the sign of the given floating-point number.

		The function extracts the sign bit from the IEEE 754 representation of the floating-point number and shifts it to the least significant bit position. If the input number is negative, the
	   result will be all ones, otherwise all zeros.

		\param f the floating-point number to evaluate
		\return an integer mask with all bits set to zero if the input is positive, or all bits set to one if the input is negative
	*/
	static int			  MaskForFloatSign( float f );

	/*!
		\brief Returns a mask of all zeros or all ones based on the sign of the input integer.

		The function calculates a mask where all bits are set to zero for positive integers and all bits are set to one for negative integers. This is achieved by right-shifting the input integer by
	   the number of bits in a 32-bit signed integer minus one. For zero, the result will be zero.

		\param i The input integer whose sign will determine the mask
		\return An integer mask where all bits are zero for positive numbers and all bits are one for negative numbers
	*/
	static int			  MaskForIntegerSign( int i );

	/*!
		\brief Computes the largest power of two that is less than or equal to the given integer.

		This function calculates the floor power of two for the input integer using a bit manipulation technique. It first propagates the highest set bit to all lower bits, then increments the value
	   and shifts it right by one position to obtain the largest power of two that does not exceed the input.

		\param x The input integer for which the floor power of two is computed
		\return The largest power of two that is less than or equal to the input integer
	*/
	static int			  FloorPowerOfTwo( int x );

	/*!
		\brief Rounds the input integer up to the nearest power of two.

		This function takes an integer input and computes the smallest power of two that is greater than or equal to the input value. The algorithm works by first decrementing the input, then
	   performing a series of bitwise OR operations with right-shifted versions of the value to set all bits to the left of the most significant bit. Finally, it increments the result to get the next
	   power of two.

		\param x the input integer to be rounded up to the nearest power of two
		\return the smallest power of two that is greater than or equal to the input integer
	*/
	static int			  CeilPowerOfTwo( int x );

	/*!
		\brief Checks if the given integer is a power of two.

		This function determines whether the provided integer is a power of two by using a bitwise operation. It returns true if the integer is a positive power of two, and false otherwise. The
	   implementation uses the bit manipulation technique where x & (x-1) equals zero only for powers of two.

		\param x The integer to check for being a power of two
		\return True if the input integer is a positive power of two, false otherwise
	*/
	static bool			  IsPowerOfTwo( int x );

	/*!
		\brief Returns the number of 1 bits in the input integer.

		This function calculates the Hamming weight of the input integer, which is the count of set bits (1s) in its binary representation. The implementation uses a bit manipulation algorithm that
	   efficiently counts the bits by combining parallel operations. The algorithm works by first subtracting pairs of bits, then summing pairs of 2-bit counts, then 4-bit counts, and so on, until the
	   final count is obtained.

		\param x The integer value for which to count the number of set bits
		\return The number of 1 bits in the input integer
	*/
	static int			  BitCount( int x );

	/*!
		\brief Returns the bit reverse of the given integer.

		This function performs a bit reversal operation on the input integer by sequentially swapping bits in groups of increasing size. It first swaps adjacent bits, then pairs of bits, then nibbles,
	   then bytes, and finally swaps the two 16-bit halves of the integer. The result is the bit-reversed version of the input.

		\param x The integer value to reverse bits for
		\return The bit-reversed integer value
	*/
	static int			  BitReverse( int x );

	/*!
		\brief Returns the absolute value of the given integer.

		This function computes the absolute value of an integer, returning the non-negative value of the input. It handles both positive and negative integers, with special handling for the minimum
	   integer value to avoid overflow issues. The implementation uses the standard library abs function when enabled, otherwise employs a bitwise operation to calculate the absolute value.

		\param x The integer value for which to compute the absolute value
		\return The absolute value of the input integer x
	*/
	static int			  Abs( int x );

	/*!
		\brief Returns the absolute value of the given floating point number

		This function calculates and returns the absolute value of a floating point number. It provides two implementation paths: a simple call to the standard library function fabsf when the
	   preprocessor flag is set, or a bit-level manipulation approach that directly masks the sign bit of the floating point representation. The function is marked as ID_INLINE, indicating it should
	   be inlined for performance reasons.

		\param f the floating point number to calculate the absolute value for
		\return the absolute value of the input floating point number
	*/
	static float		  Fabs( float f );

	//! Returns the largest integer that is less than or equal to the given floating-point value.
	static float		  Floor( float f );

	//! Returns the smallest integer that is greater than or equal to the given floating-point value.
	static float		  Ceil( float f );

	//! Returns the nearest integer to the given floating-point value.
	static float		  Rint( float f );

	/*!
		\brief Returns the fractional part of a floating-point number.

		This function computes the fractional part of the input floating-point number by subtracting its floor value from the original number. It is commonly used to extract the decimal portion of a
	   floating-point value for various computational purposes, such as interpolation or shadow mapping calculations.

		\param f The input floating-point number from which to extract the fractional part
		\return The fractional part of the input floating-point number, which is always in the range [0, 1).
	*/
	static float		  Frac( float f );

	/*!
		\brief Converts a floating-point value to an integer using fast conversion methods

		This function provides a fast conversion from floating-point to integer representation. It uses SSE intrinsics when available for optimized performance, or falls back to standard C
	   floating-point to integer conversion. The function handles the conversion with potential floating-point exception behavior when the result exceeds the maximum signed doubleword integer.

		\param f The floating-point value to convert to integer
		\return The integer representation of the floating-point input value
	*/
	static int			  Ftoi( float f );

	/*!
		\brief Converts a float value to a char with clamping to the range [-128, 127].

		This function takes a floating-point value and converts it to a char. The conversion process clamps the result to the valid range of char values, which is [-128, 127]. If the input float value
	   is less than -128, it is clamped to -128. If it exceeds 127, it is clamped to 127. This ensures that the output is always within the valid range for a char type. The function uses SSE
	   intrinsics when available for optimized performance, otherwise it falls back to a standard implementation with explicit clamping.

		\param f The floating-point value to be converted to a char
		\return A char representing the clamped conversion of the input float value
	*/
	static char			  Ftoi8( float f );

	/*!
		\brief Converts a float value to a short integer with clamping to the range [-32768, 32767].

		This function takes a floating-point value and converts it to a short integer. The conversion process clamps the result to the valid range of short values, which is [-32768, 32767]. If the
	   input float value is less than -32768, it is clamped to -32768. If it exceeds 32767, it is clamped to 32767. This ensures that the output is always within the valid range for a short type. The
	   function uses SSE intrinsics when available for optimized performance, otherwise it falls back to a standard implementation with explicit clamping.

		\param f The floating-point value to be converted to a short integer
		\return A short integer representing the clamped conversion of the input float value
	*/
	static short		  Ftoi16( float f );

	/*!
		\brief Converts a float value to an unsigned short with clamping to the range [0, 65535].

		This function takes a floating-point value and converts it to an unsigned short integer. The conversion process clamps the result to the valid range of unsigned short values, which is [0,
	   65535]. If the input float value is less than zero, it is clamped to 0. If it exceeds 65535, it is clamped to 65535. This ensures that the output is always within the valid range for an
	   unsigned short type.

		\param f The floating-point value to be converted to an unsigned short
		\return An unsigned short integer representing the clamped conversion of the input float value
	*/
	static unsigned short Ftoui16( float f );

	/*!
		\brief Converts a floating-point value to a byte with clamping to the range [0, 255].

		This function takes a floating-point number and converts it to an 8-bit byte value. The conversion includes clamping the input value to the valid range [0, 255] to ensure the result fits
	   within a byte. If the input is less than 0, it returns 0; if greater than 255, it returns 255. The function uses SSE instructions when available for optimized performance, otherwise falling
	   back to a standard conditional approach.

		\param f The floating-point value to convert to a byte
		\return The byte representation of the clamped floating-point value
	*/
	static byte			  Ftob( float f );

	/*!
		\brief Returns the smaller of two floating-point values.

		This function computes the minimum of two floating-point numbers. It uses SSE instructions if available, otherwise falls back to a standard conditional comparison. The implementation is
	   optimized for performance by using inline assembly when possible.

		\param a First floating-point value to compare
		\param b Second floating-point value to compare
		\return The smaller of the two input floating-point values
	*/
	static float		  Fmin( float a, float b );

	/*!
		\brief Returns the minimum value among three floating-point numbers.

		This function computes the minimum of three floating-point values by recursively applying the Fmin function to two values at a time. It leverages the existing two-argument Fmin implementation
	   to determine the smallest of the three inputs.

		\param a First floating-point number
		\param b Second floating-point number
		\param c Third floating-point number
		\return The smallest of the three input floating-point numbers
	*/
	static float		  Fmin( float a, float b, float c ) { return Fmin( a, Fmin( b, c ) ); }

	/*!
		\brief Returns the maximum of two floating-point values.

		This function computes the maximum of two floating-point numbers. It uses SSE instructions if available, otherwise falls back to a standard conditional expression. The implementation is
	   optimized for performance using compiler-specific intrinsics when possible.

		\param a First floating-point value to compare
		\param b Second floating-point value to compare
		\return The larger of the two input floating-point values
	*/
	static float		  Fmax( float a, float b );

	/*!
		\brief Returns the smaller of two integer values.

		This function takes two integer parameters and returns the smaller of the two. It uses a ternary operator to compare the values and return the minimum.

		\param a First integer value to compare
		\param b Second integer value to compare
		\return The smaller of the two input integers
	*/
	static int			  Imin( int a, int b );

	/*!
		\brief Returns the maximum of two integers.

		This function takes two integer values and returns the larger of the two. It is implemented as a simple inline comparison using the ternary operator. The function is marked as ID_INLINE,
	   indicating it's intended to be inlined by the compiler for performance reasons. The implementation is straightforward and efficient, making it suitable for use in performance-critical code
	   paths.

		\param a First integer value to compare
		\param b Second integer value to compare
		\return The larger of the two input integers
	*/
	static int			  Imax( int a, int b );

	/*!
		\brief Clamps an integer value to the range of a signed char.

		This function takes an integer input and clamps it to the valid range of a signed char, which is from -128 to 127. If the input integer is less than -128, it returns -128. If the input integer
	   is greater than 127, it returns 127. Otherwise, it casts the integer to a signed char and returns it. This function is commonly used in game development to ensure that movement values stay
	   within the bounds expected by the game engine's command handling system.

		\param i The integer value to clamp to the range of a signed char
		\return A signed char value that is clamped to the range [-128, 127]
	*/
	static signed char	  ClampChar( int i );

	/*!
		\brief Clamps an integer value to the range of a signed short integer.

		This function takes an integer input and clamps it to the valid range of a signed short integer, which is from -32768 to 32767. If the input value is less than -32768, it returns -32768. If
	   the input value is greater than 32767, it returns 32767. Otherwise, it safely casts the integer to a signed short and returns the result.

		\param i The integer value to be clamped to the range of a signed short
		\return A signed short integer value clamped to the range -32768 to 32767
	*/
	static signed short	  ClampShort( int i );

	/*!
		\brief Clamps an integer value to be within a specified minimum and maximum range.

		This function takes three integer parameters: min, max, and value. It returns the value if it is within the range [min, max]. If the value is less than min, it returns min. If the value is
	   greater than max, it returns max. This is useful for ensuring that integer values stay within valid bounds.

		\param min The minimum allowed value
		\param max The maximum allowed value
		\param value The integer value to be clamped
		\return The clamped integer value within the range [min, max]
	*/
	static int			  ClampInt( int min, int max, int value );

	//! Clamps a float value between a minimum and maximum bound.
	static float		  ClampFloat( float min, float max, float value );

	/*!
		\brief Normalizes an angle to the range [0, 360)

		This function takes an angle in degrees and normalizes it to the range [0, 360). It handles angles that may be outside this range by subtracting the appropriate multiple of 360 degrees to
	   bring the angle within the desired range. The function is inlined for performance reasons.

		\param angle The angle in degrees to be normalized
		\return The normalized angle in the range [0, 360)
	*/
	static float		  AngleNormalize360( float angle );

	/*!
		\brief Normalizes an angle to the range [-180, 180) degrees.

		This function takes an angle in degrees and normalizes it to the range [-180, 180). It first normalizes the angle to the range [0, 360) using the AngleNormalize360 function, and then adjusts
	   the result to fall within the desired range by subtracting 360 degrees if the angle exceeds 180 degrees.

		\param angle The angle in degrees to be normalized
		\return The normalized angle in the range [-180, 180)
	*/
	static float		  AngleNormalize180( float angle );

	//! Computes the normalized angular difference between two angles.
	static float		  AngleDelta( float angle1, float angle2 );

	/*!
		\brief Converts a floating-point value to a bit representation with specified exponent and mantissa bits

		This function transforms a floating-point number into a bit representation using a custom format defined by the specified number of exponent and mantissa bits. The function handles both
	   positive and negative values, clipping extreme values to the maximum or minimum representable values. It uses IEEE 754 floating-point format as the base for conversion, but packs the result
	   into a custom bit layout. The function includes assertions to validate that the exponent and mantissa bit counts are within acceptable ranges.

		\param f The floating-point value to convert to bit representation
		\param exponentBits The number of bits to use for the exponent field
		\param mantissaBits The number of bits to use for the mantissa field
		\return The bit representation of the floating-point value using the specified exponent and mantissa bit counts
		\throws assertion failure if exponentBits is less than 2 or greater than 8, or if mantissaBits is less than 2 or greater than 23
	*/
	static int			  FloatToBits( float f, int exponentBits, int mantissaBits );

	/*!
		\brief Converts an integer representation with specified exponent and mantissa bits into a floating-point number.

		This function takes an integer value and interprets it as a floating-point number with a custom bit layout. The integer is split into sign, exponent, and mantissa components based on the
	   specified bit counts. The exponent is signed and uses the most significant bit of the exponent field to determine its sign. The mantissa is normalized to the standard IEEE floating-point
	   format. The resulting float value is computed and returned.

		\param i The integer representation of the floating-point number
		\param exponentBits The number of bits allocated for the exponent field
		\param mantissaBits The number of bits allocated for the mantissa field
		\return The floating-point number represented by the integer with the specified bit layout
		\throws assertion failure if exponentBits is not between 2 and 8, or if mantissaBits is not between 2 and 23
	*/
	static float		  BitsToFloat( int i, int exponentBits, int mantissaBits );

	/*!
		\brief Computes a hash value for an array of floating-point numbers by XORing the integer representations of the floats.

		This function takes an array of floating-point numbers and converts them to their integer bit representations. It then computes a hash value by performing an XOR operation on all the integer
	   values. The hash value is returned as an integer. The function assumes that the array contains valid floating-point numbers and that numFloats is a positive integer.

		\param array Pointer to the array of floating-point numbers to hash
		\param numFloats Number of floating-point numbers in the array
		\return The computed hash value as an integer
	*/
	static int			  FloatHash( const float* array, const int numFloats );

	/*!
		\brief Linearly interpolates from the current value towards the destination value using the specified scale.

		This function performs linear interpolation between the current value and the destination value based on the provided scale factor. It first checks if the difference between the current and
	   destination values is negligible (within a small epsilon threshold). If so, it returns the destination value directly to avoid unnecessary computation. Otherwise, it computes the interpolated
	   value using the formula: cur + (dest - cur) * scale.

		\param cur The current value to interpolate from
		\param dest The destination value to interpolate towards
		\param scale The interpolation scale factor, typically between 0 and 1
		\return The interpolated value between cur and dest based on the scale factor
	*/
	static float		  LerpToWithScale( const float cur, const float dest, const float scale );

	/*!
		\brief Checks if the given integer is a prime number.

		This function determines whether a given integer is a prime number. It uses an optimized algorithm that first handles small cases and then checks divisibility using the 6k±1 optimization. The
	   function returns false for numbers less than 2, true for 2 and 3, and then checks for divisibility by 2 and 3 before testing potential divisors of the form 6k±1 up to the square root of n.

		\param n The integer to check for primality
		\return true if the integer is a prime number, false otherwise
	*/
	static bool			  IsPrime( int n );

	/*!
		\brief Returns the next prime number greater than the given integer.

		This function takes an integer input and finds the smallest prime number that is greater than the input value. It increments the input value until it finds a prime number using the IsPrime
	   helper function. The function is typically used in contexts where prime numbers are needed for hash table sizing or other algorithms that benefit from prime number properties.

		\param current The integer value to find the next prime number for
		\return The next prime number that is greater than the input value
	*/
	static int			  NextPrime( int current );

	static const float	  PI;						 // pi
	static const float	  TWO_PI;					 // pi * 2
	static const float	  HALF_PI;					 // pi / 2
	static const float	  ONEFOURTH_PI;				 // pi / 4
	static const float	  ONEOVER_PI;				 // 1 / pi
	static const float	  ONEOVER_TWOPI;			 // 1 / pi * 2
	static const float	  E;						 // e
	static const float	  SQRT_TWO;					 // sqrt( 2 )
	static const float	  SQRT_THREE;				 // sqrt( 3 )
	static const float	  SQRT_1OVER2;				 // sqrt( 1 / 2 )
	static const float	  SQRT_1OVER3;				 // sqrt( 1 / 3 )
	static const float	  M_DEG2RAD;				 // degrees to radians multiplier
	static const float	  M_RAD2DEG;				 // radians to degrees multiplier
	static const float	  M_SEC2MS;					 // seconds to milliseconds multiplier
	static const float	  M_MS2SEC;					 // milliseconds to seconds multiplier
	static const float	  INFINITUM;				 // huge number which should be larger than any valid number used
	static const float	  FLOAT_EPSILON;			 // smallest positive number such that 1.0+FLOAT_EPSILON != 1.0
	static const float	  FLT_SMALLEST_NON_DENORMAL; // smallest non-denormal 32-bit floating point value

#if defined( USE_INTRINSICS_SSE )
	static const __m128 SIMD_SP_zero;
	static const __m128 SIMD_SP_255;
	static const __m128 SIMD_SP_min_char;
	static const __m128 SIMD_SP_max_char;
	static const __m128 SIMD_SP_min_short;
	static const __m128 SIMD_SP_max_short;
	static const __m128 SIMD_SP_smallestNonDenorm;
	static const __m128 SIMD_SP_tiny;
	static const __m128 SIMD_SP_rsqrt_c0;
	static const __m128 SIMD_SP_rsqrt_c1;
#endif

private:
	enum {
		LOOKUP_BITS		= 8,
		EXP_POS			= 23,
		EXP_BIAS		= 127,
		LOOKUP_POS		= ( EXP_POS - LOOKUP_BITS ),
		SEED_POS		= ( EXP_POS - 8 ),
		SQRT_TABLE_SIZE = ( 2 << LOOKUP_BITS ),
		LOOKUP_MASK		= ( SQRT_TABLE_SIZE - 1 )
	};

	union _flint {
		dword i;
		float f;
	};

	static dword iSqrt[SQRT_TABLE_SIZE];
	static bool	 initialized;
};

/*!
	\brief Clamps an integer value to the range [0, 255] and returns it as a byte.

	This function takes an integer value and ensures it falls within the valid range for a byte datatype, which is from 0 to 255. Values below 0 are set to 0, and values above 255 are set to 255. This
   is commonly used in color space conversions and other operations where byte-level precision is required.

	\param x The integer value to be clamped to the range [0, 255]
	\return A byte value that is the clamped result of the input integer
*/
ID_INLINE byte CLAMP_BYTE( int x )
{
	return ( ( x ) < 0 ? ( 0 ) : ( ( x ) > 255 ? 255 : ( byte )( x ) ) );
}

ID_INLINE float idMath::RSqrt( float x )
{
	int	  i;
	float y, r;

	y = x * 0.5f;
	i = *reinterpret_cast<int*>( &x );
	i = 0x5f3759df - ( i >> 1 );
	r = *reinterpret_cast<float*>( &i );
	r = r * ( 1.5f - r * r * y );
	return r;
}

ID_INLINE float idMath::InvSqrt( float x )
{
	return ( x > FLT_SMALLEST_NON_DENORMAL ) ? sqrtf( 1.0f / x ) : INFINITUM;
}

ID_INLINE float idMath::InvSqrt16( float x )
{
	return ( x > FLT_SMALLEST_NON_DENORMAL ) ? sqrtf( 1.0f / x ) : INFINITUM;
}

ID_INLINE float idMath::Sqrt( float x )
{
	return ( x >= 0.0f ) ? x * InvSqrt( x ) : 0.0f;
}

ID_INLINE float idMath::Sqrt16( float x )
{
	return ( x >= 0.0f ) ? x * InvSqrt16( x ) : 0.0f;
}

ID_INLINE float idMath::Frac( float f )
{
	return f - floorf( f );
}

ID_INLINE float idMath::Sin( float a )
{
	return sinf( a );
}

ID_INLINE float idMath::Sin16( float a )
{
	float s;

	if( ( a < 0.0f ) || ( a >= TWO_PI ) ) { a -= floorf( a * ONEOVER_TWOPI ) * TWO_PI; }
#if 1
	if( a < PI ) {
		if( a > HALF_PI ) { a = PI - a; }
	} else {
		if( a > PI + HALF_PI ) {
			a = a - TWO_PI;
		} else {
			a = PI - a;
		}
	}
#else
	a = PI - a;
	if( fabsf( a ) >= HALF_PI ) { a = ( ( a < 0.0f ) ? -PI : PI ) - a; }
#endif
	s = a * a;
	return a * ( ( ( ( ( -2.39e-08f * s + 2.7526e-06f ) * s - 1.98409e-04f ) * s + 8.3333315e-03f ) * s - 1.666666664e-01f ) * s + 1.0f );
}

ID_INLINE float idMath::Cos( float a )
{
	return cosf( a );
}

ID_INLINE float idMath::Cos16( float a )
{
	float s, d;

	if( ( a < 0.0f ) || ( a >= TWO_PI ) ) { a -= floorf( a * ONEOVER_TWOPI ) * TWO_PI; }
#if 1
	if( a < PI ) {
		if( a > HALF_PI ) {
			a = PI - a;
			d = -1.0f;
		} else {
			d = 1.0f;
		}
	} else {
		if( a > PI + HALF_PI ) {
			a = a - TWO_PI;
			d = 1.0f;
		} else {
			a = PI - a;
			d = -1.0f;
		}
	}
#else
	a = PI - a;
	if( fabsf( a ) >= HALF_PI ) {
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
		d = 1.0f;
	} else {
		d = -1.0f;
	}
#endif
	s = a * a;
	return d * ( ( ( ( ( -2.605e-07f * s + 2.47609e-05f ) * s - 1.3888397e-03f ) * s + 4.16666418e-02f ) * s - 4.999999963e-01f ) * s + 1.0f );
}

ID_INLINE void idMath::SinCos( float a, float& s, float& c )
{
#if defined( _MSC_VER ) && defined( _M_IX86 )
	_asm
		{
		fld		a
		fsincos
		mov		ecx, c
		mov		edx, s
		fstp	dword ptr [ecx]
		fstp	dword ptr [edx]
		}
#else
	// DG: non-MSVC version
	s = sinf( a );
	c = cosf( a );
		// DG end
#endif
}

ID_INLINE void idMath::SinCos16( float a, float& s, float& c )
{
	float t, d;

	if( ( a < 0.0f ) || ( a >= TWO_PI ) ) { a -= floorf( a * ONEOVER_TWOPI ) * TWO_PI; }
#if 1
	if( a < PI ) {
		if( a > HALF_PI ) {
			a = PI - a;
			d = -1.0f;
		} else {
			d = 1.0f;
		}
	} else {
		if( a > PI + HALF_PI ) {
			a = a - TWO_PI;
			d = 1.0f;
		} else {
			a = PI - a;
			d = -1.0f;
		}
	}
#else
	a = PI - a;
	if( fabsf( a ) >= HALF_PI ) {
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
		d = 1.0f;
	} else {
		d = -1.0f;
	}
#endif
	t = a * a;
	s = a * ( ( ( ( ( -2.39e-08f * t + 2.7526e-06f ) * t - 1.98409e-04f ) * t + 8.3333315e-03f ) * t - 1.666666664e-01f ) * t + 1.0f );
	c = d * ( ( ( ( ( -2.605e-07f * t + 2.47609e-05f ) * t - 1.3888397e-03f ) * t + 4.16666418e-02f ) * t - 4.999999963e-01f ) * t + 1.0f );
}

ID_INLINE float idMath::Tan( float a )
{
	return tanf( a );
}

ID_INLINE float idMath::Tan16( float a )
{
	float s;
	bool  reciprocal;

	if( ( a < 0.0f ) || ( a >= PI ) ) { a -= floorf( a * ONEOVER_PI ) * PI; }
#if 1
	if( a < HALF_PI ) {
		if( a > ONEFOURTH_PI ) {
			a		   = HALF_PI - a;
			reciprocal = true;
		} else {
			reciprocal = false;
		}
	} else {
		if( a > HALF_PI + ONEFOURTH_PI ) {
			a		   = a - PI;
			reciprocal = false;
		} else {
			a		   = HALF_PI - a;
			reciprocal = true;
		}
	}
#else
	a = HALF_PI - a;
	if( fabsf( a ) >= ONEFOURTH_PI ) {
		a		   = ( ( a < 0.0f ) ? -HALF_PI : HALF_PI ) - a;
		reciprocal = false;
	} else {
		reciprocal = true;
	}
#endif
	s = a * a;
	s = a * ( ( ( ( ( ( 9.5168091e-03f * s + 2.900525e-03f ) * s + 2.45650893e-02f ) * s + 5.33740603e-02f ) * s + 1.333923995e-01f ) * s + 3.333314036e-01f ) * s + 1.0f );
	if( reciprocal ) {
		return 1.0f / s;
	} else {
		return s;
	}
}

ID_INLINE float idMath::ASin( float a )
{
	if( a <= -1.0f ) { return -HALF_PI; }
	if( a >= 1.0f ) { return HALF_PI; }
	return asinf( a );
}

ID_INLINE float idMath::ASin16( float a )
{
	if( a < 0.0f ) {
		if( a <= -1.0f ) { return -HALF_PI; }
		a = fabsf( a );
		return ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * idMath::Sqrt( 1.0f - a ) - HALF_PI;
	} else {
		if( a >= 1.0f ) { return HALF_PI; }
		return HALF_PI - ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * idMath::Sqrt( 1.0f - a );
	}
}

ID_INLINE float idMath::ACos( float a )
{
	if( a <= -1.0f ) { return PI; }
	if( a >= 1.0f ) { return 0.0f; }
	return acosf( a );
}

ID_INLINE float idMath::ACos16( float a )
{
	if( a < 0.0f ) {
		if( a <= -1.0f ) { return PI; }
		a = fabsf( a );
		return PI - ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * idMath::Sqrt( 1.0f - a );
	} else {
		if( a >= 1.0f ) { return 0.0f; }
		return ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * idMath::Sqrt( 1.0f - a );
	}
}

ID_INLINE float idMath::ATan( float a )
{
	return atanf( a );
}

ID_INLINE float idMath::ATan16( float a )
{
	float s;
	if( fabsf( a ) > 1.0f ) {
		a = 1.0f / a;
		s = a * a;
		s = -( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f ) * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) *
				   s ) +
				1.0f ) *
			a;
		if( a < 0.0f ) {
			return s - HALF_PI;
		} else {
			return s + HALF_PI;
		}
	} else {
		s = a * a;
		return ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f ) * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s -
					   0.3333314528f ) *
					 s ) +
				   1.0f ) *
			   a;
	}
}

ID_INLINE float idMath::ATan( float y, float x )
{
	// SRS - Don't need this assertion since atan2f(y,x) handles x=0, y=0 and x=0, y>0 or y<0 cases properly
	// SRS - This assertion can cause game to stop prematurely when _DEBUG is defined and asserts are enabled
	// assert( fabs( y ) > idMath::FLT_SMALLEST_NON_DENORMAL || fabs( x ) > idMath::FLT_SMALLEST_NON_DENORMAL );
	return atan2f( y, x );
}

ID_INLINE float idMath::ATan16( float y, float x )
{
	assert( fabs( y ) > idMath::FLT_SMALLEST_NON_DENORMAL || fabs( x ) > idMath::FLT_SMALLEST_NON_DENORMAL );

	float a, s;
	if( fabsf( y ) > fabsf( x ) ) {
		a = x / y;
		s = a * a;
		s = -( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f ) * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) *
				   s ) +
				1.0f ) *
			a;
		if( a < 0.0f ) {
			return s - HALF_PI;
		} else {
			return s + HALF_PI;
		}
	} else {
		a = y / x;
		s = a * a;
		return ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f ) * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s -
					   0.3333314528f ) *
					 s ) +
				   1.0f ) *
			   a;
	}
}

ID_INLINE float idMath::Pow( float x, float y )
{
	return powf( x, y );
}

ID_INLINE float idMath::Pow16( float x, float y )
{
	return Exp16( y * Log16( x ) );
}

ID_INLINE float idMath::Exp( float f )
{
	return expf( f );
}

ID_INLINE float idMath::Exp16( float f )
{
	float x = f * 1.44269504088896340f; // multiply with ( 1 / log( 2 ) )
#if 1
	int i = *reinterpret_cast<int*>( &x );
	int s = ( i >> IEEE_FLT_SIGN_BIT );
	int e = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	int m = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	i	  = ( ( m >> ( IEEE_FLT_MANTISSA_BITS - e ) ) & ~( e >> INT32_SIGN_BIT ) ) ^ s;
#else
	int i = ( int )x;
	if( x < 0.0f ) { i--; }
#endif
	int	  exponent = ( i + IEEE_FLT_EXPONENT_BIAS ) << IEEE_FLT_MANTISSA_BITS;
	float y		   = *reinterpret_cast<float*>( &exponent );
	x -= ( float )i;
	if( x >= 0.5f ) {
		x -= 0.5f;
		y *= 1.4142135623730950488f; // multiply with sqrt( 2 )
	}
	float x2 = x * x;
	float p	 = x * ( 7.2152891511493f + x2 * 0.0576900723731f );
	float q	 = 20.8189237930062f + x2;
	x		 = y * ( q + p ) / ( q - p );
	return x;
}

ID_INLINE float idMath::Log( float f )
{
	return logf( f );
}

ID_INLINE float idMath::Log16( float f )
{
	int i		 = *reinterpret_cast<int*>( &f );
	int exponent = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	i -= ( exponent + 1 ) << IEEE_FLT_MANTISSA_BITS; // get value in the range [.5, 1>
	float y = *reinterpret_cast<float*>( &i );
	y *= 1.4142135623730950488f; // multiply with sqrt( 2 )
	y		 = ( y - 1.0f ) / ( y + 1.0f );
	float y2 = y * y;
	y		 = y * ( 2.000000000046727f + y2 * ( 0.666666635059382f + y2 * ( 0.4000059794795f + y2 * ( 0.28525381498f + y2 * 0.2376245609f ) ) ) );
	y += 0.693147180559945f * ( ( float )exponent + 0.5f );
	return y;
}

ID_INLINE int idMath::IPow( int x, int y )
{
	int r;
	for( r = x; y > 1; y-- ) {
		r *= x;
	}
	return r;
}

ID_INLINE int idMath::ILog2( float f )
{
	return ( ( ( *reinterpret_cast<int*>( &f ) ) >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
}

ID_INLINE int idMath::ILog2( int i )
{
	return ILog2( ( float )i );
}

ID_INLINE int idMath::BitsForFloat( float f )
{
	return ILog2( f ) + 1;
}

ID_INLINE int idMath::BitsForInteger( int i )
{
	return ILog2( ( float )i ) + 1;
}

ID_INLINE int idMath::MaskForFloatSign( float f )
{
	return ( ( *reinterpret_cast<int*>( &f ) ) >> IEEE_FLT_SIGN_BIT );
}

ID_INLINE int idMath::MaskForIntegerSign( int i )
{
	return ( i >> INT32_SIGN_BIT );
}

ID_INLINE int idMath::FloorPowerOfTwo( int x )
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x >> 1;
}

ID_INLINE int idMath::CeilPowerOfTwo( int x )
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x;
}

ID_INLINE bool idMath::IsPowerOfTwo( int x )
{
	return ( x & ( x - 1 ) ) == 0 && x > 0;
}

ID_INLINE int idMath::BitCount( int x )
{
	x -= ( ( x >> 1 ) & 0x55555555 );
	x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
	x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
	x += ( x >> 8 );
	return ( ( x + ( x >> 16 ) ) & 0x0000003f );
}

ID_INLINE int idMath::BitReverse( int x )
{
	x = ( ( ( x >> 1 ) & 0x55555555 ) | ( ( x & 0x55555555 ) << 1 ) );
	x = ( ( ( x >> 2 ) & 0x33333333 ) | ( ( x & 0x33333333 ) << 2 ) );
	x = ( ( ( x >> 4 ) & 0x0f0f0f0f ) | ( ( x & 0x0f0f0f0f ) << 4 ) );
	x = ( ( ( x >> 8 ) & 0x00ff00ff ) | ( ( x & 0x00ff00ff ) << 8 ) );
	return ( ( x >> 16 ) | ( x << 16 ) );
}

ID_INLINE int idMath::Abs( int x )
{
#if 1
	return abs( x );
#else
	int y = x >> INT32_SIGN_BIT;
	return ( ( x ^ y ) - y );
#endif
}

ID_INLINE float idMath::Fabs( float f )
{
#if 1
	return fabsf( f );
#else
	int tmp = *reinterpret_cast<int*>( &f );
	tmp &= 0x7FFFFFFF;
	return *reinterpret_cast<float*>( &tmp );
#endif
}

ID_INLINE float idMath::Floor( float f )
{
	return floorf( f );
}

ID_INLINE float idMath::Ceil( float f )
{
	return ceilf( f );
}

ID_INLINE float idMath::Rint( float f )
{
	return floorf( f + 0.5f );
}

ID_INLINE int idMath::Ftoi( float f )
{
	// If a converted result is larger than the maximum signed doubleword integer,
	// the floating-point invalid exception is raised, and if this exception is masked,
	// the indefinite integer value (80000000H) is returned.
#if defined( USE_INTRINSICS_SSE )
	__m128 x = _mm_load_ss( &f );
	return _mm_cvttss_si32( x );
#elif 0 // round chop (C/C++ standard)
	int i, s, e, m, shift;
	i	  = *reinterpret_cast<int*>( &f );
	s	  = i >> IEEE_FLT_SIGN_BIT;
	e	  = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	m	  = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	shift = e - IEEE_FLT_MANTISSA_BITS;
	return ( ( ( ( m >> -shift ) | ( m << shift ) ) & ~( e >> INT32_SIGN_BIT ) ) ^ s ) - s;
#else
	// If a converted result is larger than the maximum signed doubleword integer the result is undefined.
	return C_FLOAT_TO_INT( f );
#endif
}

ID_INLINE char idMath::Ftoi8( float f )
{
#if defined( USE_INTRINSICS_SSE )
	__m128 x = _mm_load_ss( &f );
	x		 = _mm_max_ss( x, SIMD_SP_min_char );
	x		 = _mm_min_ss( x, SIMD_SP_max_char );
	return static_cast<char>( _mm_cvttss_si32( x ) );
#else
	// The converted result is clamped to the range [-128,127].
	int i = C_FLOAT_TO_INT( f );
	if( i < -128 ) {
		return -128;
	} else if( i > 127 ) {
		return 127;
	}
	return static_cast<char>( i );
#endif
}

ID_INLINE short idMath::Ftoi16( float f )
{
#if defined( USE_INTRINSICS_SSE )
	__m128 x = _mm_load_ss( &f );
	x		 = _mm_max_ss( x, SIMD_SP_min_short );
	x		 = _mm_min_ss( x, SIMD_SP_max_short );
	return static_cast<short>( _mm_cvttss_si32( x ) );
#else
	// The converted result is clamped to the range [-32768,32767].
	int i = C_FLOAT_TO_INT( f );
	if( i < -32768 ) {
		return -32768;
	} else if( i > 32767 ) {
		return 32767;
	}
	return static_cast<short>( i );
#endif
}

ID_INLINE unsigned short idMath::Ftoui16( float f )
{
	// TO DO - SSE ??

	// The converted result is clamped to the range [-32768,32767].
	int i = C_FLOAT_TO_INT( f );
	if( i < 0 ) {
		return 0;
	} else if( i > 65535 ) {
		return 65535;
	}
	return static_cast<unsigned short>( i );
}

ID_INLINE byte idMath::Ftob( float f )
{
	// If a converted result is negative the value (0) is returned and if the
	// converted result is larger than the maximum byte the value (255) is returned.

#if defined( USE_INTRINSICS_SSE )
	__m128 x = _mm_load_ss( &f );
	x		 = _mm_max_ss( x, SIMD_SP_zero );
	x		 = _mm_min_ss( x, SIMD_SP_255 );
	return static_cast<byte>( _mm_cvttss_si32( x ) );
#else
	// The converted result is clamped to the range [0,255].
	int i = C_FLOAT_TO_INT( f );
	if( i < 0 ) {
		return 0;
	} else if( i > 255 ) {
		return 255;
	}
	return static_cast<byte>( i );
#endif
}

ID_FORCE_INLINE float idMath::Fmin( float a, float b )
{
#ifdef __SSE__
	return _mm_cvtss_f32( _mm_min_ss( _mm_set_ss( a ), _mm_set_ss( b ) ) );
#else
	return a < b ? a : b;
#endif
}

ID_FORCE_INLINE float idMath::Fmax( float a, float b )
{
#ifdef __SSE__
	return _mm_cvtss_f32( _mm_max_ss( _mm_set_ss( a ), _mm_set_ss( b ) ) );
#else
	return a > b ? a : b;
#endif
}

ID_INLINE int idMath::Imin( int a, int b )
{
	return ( a < b ? a : b );
}

ID_INLINE int idMath::Imax( int a, int b )
{
	return ( a > b ? a : b );
}

ID_INLINE signed char idMath::ClampChar( int i )
{
	if( i < -128 ) { return -128; }
	if( i > 127 ) { return 127; }
	return static_cast<signed char>( i );
}

ID_INLINE signed short idMath::ClampShort( int i )
{
	if( i < -32768 ) { return -32768; }
	if( i > 32767 ) { return 32767; }
	return static_cast<signed short>( i );
}

ID_INLINE int idMath::ClampInt( int min, int max, int value )
{
	if( value < min ) { return min; }
	if( value > max ) { return max; }
	return value;
}

ID_INLINE float idMath::ClampFloat( float min, float max, float value )
{
	return Max( min, Min( max, value ) );
}

ID_INLINE float idMath::AngleNormalize360( float angle )
{
	if( ( angle >= 360.0f ) || ( angle < 0.0f ) ) { angle -= floorf( angle * ( 1.0f / 360.0f ) ) * 360.0f; }
	return angle;
}

ID_INLINE float idMath::AngleNormalize180( float angle )
{
	angle = AngleNormalize360( angle );
	if( angle > 180.0f ) { angle -= 360.0f; }
	return angle;
}

ID_INLINE float idMath::AngleDelta( float angle1, float angle2 )
{
	return AngleNormalize180( angle1 - angle2 );
}

ID_INLINE int idMath::FloatHash( const float* array, const int numFloats )
{
	int		   i, hash = 0;
	const int* ptr;

	ptr = reinterpret_cast<const int*>( array );
	for( i = 0; i < numFloats; i++ ) {
		hash ^= ptr[i];
	}
	return hash;
}

/*!
	\brief Computes a linear interpolation between two values based on an interpolation factor.

	This function performs a standard linear interpolation between two values from and to using the interpolation factor f. The factor f is typically between 0 and 1, where 0 returns the 'from' value
   and 1 returns the 'to' value. Values outside this range will extrapolate the result beyond the specified range.

	\param from The starting value of the interpolation.
	\param to The ending value of the interpolation.
	\param f The interpolation factor, typically between 0 and 1.
	\return The interpolated value between from and to based on the factor f.
*/
template<typename T>
ID_INLINE T Lerp( const T from, const T to, float f )
{
	return from + ( ( to - from ) * f );
}

//! Performs linear interpolation between two integer values using a floating-point factor.
template<>
ID_INLINE int Lerp( const int from, const int to, float f )
{
	return idMath::Ftoi( ( float )from + ( ( ( float )to - ( float )from ) * f ) );
}

ID_INLINE float idMath::LerpToWithScale( const float cur, const float dest, const float scale )
{
	float delta = dest - cur;
	if( delta > -1.0e-6f && delta < 1.0e-6f ) { return dest; }
	return cur + ( dest - cur ) * scale;
}

ID_INLINE float idMath::AngleMod( float a )
{
	a = ( 360.0 / 65536 ) * ( ( int )( a * ( 65536 / 360.0 ) ) & 65535 );
	return a;
}

// TODO replace with GPL code

// RAVEN BEGIN
// jscott: fast and reliable random routines

// This is the VC libc version of rand() without multiple seeds per thread or 12 levels
// of subroutine calls.
// Both calls have been designed to minimise the inherent number of float <--> int
// conversions and the additional math required to get the desired value.
// eg the typical tint = (rand() * 255) / 32768
// becomes tint = rvRandom::irand( 0, 255 )

class rvRandom
{
private:
	static unsigned long mSeed;

public:
	/*!
		\brief Initializes the random number generator with a fixed seed value.

		The constructor initializes the random number generator with a fixed seed value of 0x89abcdef. This ensures reproducible random sequences for testing and debugging purposes.

	*/
	rvRandom() { mSeed = 0x89abcdef; }

	/*!
		\brief Initializes the random number generator with a seed based on system milliseconds

		This function initializes the random number generator by multiplying the current seed value with the number of milliseconds elapsed since the system started. The result is returned as the new
	   seed value for the random number generator

		\return The initialized seed value for the random number generator
	*/
	static int	 Init();

	/*!
		\brief Initializes the random seed for the random number generator.

		This function sets the seed value used by the random number generator. The seed is used to initialize the state of the random number generator, ensuring reproducible sequences of random
	   numbers when the same seed is used. This is typically called at the start of a program or when a new random sequence is required.

		\param seed The seed value to initialize the random number generator with
	*/
	static void	 Init( unsigned long seed ) { mSeed = seed; }

	/*!
		\brief Returns a random floating-point number in the range [min, max).

		The function generates a random float value using a linear congruential generator algorithm. It uses the current seed value to produce the next random number in the sequence. The result is
	   scaled and shifted to fit within the specified range [min, max). The implementation uses bit shifting and multiplication to distribute the random values uniformly across the desired range.

		\param min The minimum value of the range (inclusive)
		\param max The maximum value of the range (exclusive)
		\return A random floating-point number in the range [min, max)
	*/
	static float flrand( float min, float max );

	//! Returns a random float value in the range [0.0, 1.0).
	static float flrand();

	//! Returns a random floating-point number within the range specified by the components of the input vector.
	static float flrand( const idVec2& v );

	/*!
		\brief Returns a random integer in the inclusive range [min, max].

		This function generates a pseudo-random integer within the specified inclusive range. It uses a linear congruential generator algorithm with a seed value stored in mSeed. The algorithm
	   produces a sequence of numbers that approximates randomness. The function first increments the max value by one to make the range exclusive of max, then applies bit shifting and multiplication
	   to distribute the results uniformly across the range. Finally, it adds the minimum value to shift the range back to the desired inclusive interval.

		\param min The minimum value of the inclusive range
		\param max The maximum value of the inclusive range
		\return A random integer in the inclusive range [min, max]
	*/
	static int	 irand( int min, int max );
};

// RAVEN END

#endif /* !__MATH_MATH_H__ */
