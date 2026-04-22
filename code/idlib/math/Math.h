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

//! Returns the sign bit of a 32-bit integer as either 0 or 1.
ID_INLINE_EXTERN int INT32_SIGNBITSET( int i )
{
	int r = OLD_INT32_SIGNBITSET( i );
	assert( r == 0 || r == 1 );
	return r;
}

//! Returns 1 if the sign bit of the input integer is not set, otherwise returns 0.
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

//! Returns true if the given float value is not a number (NaN).
ID_INLINE_EXTERN bool IEEE_FLT_IS_NAN( float x )
{
	return x != x;
}

//! Checks if a floating-point value is positive or negative infinity.
ID_INLINE_EXTERN bool IEEE_FLT_IS_INF( float x )
{
	return x == x && x * 0 != x * 0;
}

//! Checks if a floating-point value is infinity or NaN.
ID_INLINE_EXTERN bool IEEE_FLT_IS_INF_NAN( float x )
{
	return x * 0 != x * 0;
}

//! Checks if a floating-point value is an indefinite (NaN) according to IEEE 754 standard.
ID_INLINE_EXTERN bool IEEE_FLT_IS_IND( float x )
{
	return ( reinterpret_cast<const unsigned int&>( x ) == 0xffc00000 );
}

//! Checks if a floating-point value is a denormalized number according to IEEE 754 standards.
ID_INLINE_EXTERN bool IEEE_FLT_IS_DENORMAL( float x )
{
	return ( ( reinterpret_cast<const unsigned int&>( x ) & 0x7f800000 ) == 0x00000000 && ( reinterpret_cast<const unsigned int&>( x ) & 0x007fffff ) != 0x00000000 );
}

//! Checks if any component of the input vector is NaN, infinity, or indeterminate.
template<class type>
ID_INLINE_EXTERN bool IsNAN( const type& v )
{
	for( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) ) { return true; }
	}
	return false;
}

//! Checks if all components of a given value are valid floating-point numbers
template<class type>
ID_INLINE_EXTERN bool IsValid( const type& v )
{
	for( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) || IEEE_FLT_IS_DENORMAL( f ) ) { return false; }
	}
	return true;
}

//! Checks if a floating-point value is valid, meaning it is not NaN, infinity, indeterminate, or denormalized.
template<>
ID_INLINE bool IsValid( const float& f )
{
	return !( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) || IEEE_FLT_IS_DENORMAL( f ) );
}

//! Checks if a floating-point value is NaN, infinity, or indeterminate.
template<>
ID_INLINE bool IsNAN( const float& f )
{
	if( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) ) { return true; }
	return false;
}

//! Checks if all components of the vector are within the specified range
template<class type>
ID_INLINE bool IsInRange( const type& v, const float range )
{
	for( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if( f > range || f < -range ) { return false; }
	}
	return true;
}

//! Returns the index of the maximum of two values, with 0 for x and 1 for y.
template<class T>
ID_INLINE int MaxIndex( T x, T y )
{
	return ( x > y ) ? 0 : 1;
}

//! Returns 0 if x is less than y, otherwise returns 1.
template<class T>
ID_INLINE int MinIndex( T x, T y )
{
	return ( x < y ) ? 0 : 1;
}

//! Returns the maximum value among three input values of type T.
template<class T>
ID_INLINE T Max3( T x, T y, T z )
{
	return ( x > y ) ? ( ( x > z ) ? x : z ) : ( ( y > z ) ? y : z );
}

//! Returns the smallest of three values of type T.
template<class T>
ID_INLINE T Min3( T x, T y, T z )
{
	return ( x < y ) ? ( ( x < z ) ? x : z ) : ( ( y < z ) ? y : z );
}

//! Returns the index of the maximum value among three input values.
template<class T>
ID_INLINE int Max3Index( T x, T y, T z )
{
	return ( x > y ) ? ( ( x > z ) ? 0 : 2 ) : ( ( y > z ) ? 1 : 2 );
}

//! Returns the index of the smallest of three values.
template<class T>
ID_INLINE int Min3Index( T x, T y, T z )
{
	return ( x < y ) ? ( ( x < z ) ? 0 : 2 ) : ( ( y < z ) ? 1 : 2 );
}

//! Returns the sign of the input value as -1, 0, or 1.
template<class T>
ID_INLINE T Sign( T f )
{
	return ( f > 0 ) ? 1 : ( ( f < 0 ) ? -1 : 0 );
}

//! Computes the square of the input value.
template<class T>
ID_INLINE T Square( T x )
{
	return x * x;
}

//! Computes the cube of the given value.
template<class T>
ID_INLINE T Cube( T x )
{
	return x * x * x;
}

/*!
	\class idMath
	\brief A comprehensive mathematical utility class providing fast and precise floating-point operations, vector mathematics, and random number generation.

	This class serves as a central repository for mathematical operations and utilities used throughout the engine. It provides both basic arithmetic functions and specialized mathematical
   computations including vector operations, trigonometric functions, and bit manipulation utilities. The class supports both high-precision and lower-precision variants of mathematical functions to
   balance accuracy and performance. It includes functionality for random number generation, angle normalization, and various clamping and conversion utilities. The implementation emphasizes
   efficiency through inline functions and lookup table optimizations, particularly for operations like square root and trigonometric functions.

*/
class idMath
{
public:
	//! Initializes the math library by setting up lookup tables for fast square root calculations.
	static void			  Init();

	//! Normalizes an angle to the range [0, 360).
	static float		  AngleMod( float a );

	//! Computes an approximation of the reciprocal square root of the given float value.
	static float		  RSqrt( float x );

	//! Computes the cross product of two 3D vectors.
	static idVec3		  CrossProduct( const idVec3& a, const idVec3& b );

	//! Calculates the squared distance between two 3D points.
	static float		  DistanceSquared( idVec3 p1, idVec3 p2 );

	//! Computes the Euclidean distance between two 3D points.
	static float		  Distance( idVec3 p1, idVec3 p2 );

	//! Returns the reflection vector of the given vector and normal
	static idVec3		  ReflectVector( idVec3 vector, idVec3 normal );

	/*!
		\brief Creates and returns a new idVec4 vector with the specified x, y, z, and w components
		\return A new idVec4 vector initialized with the provided x, y, z, and w values
	*/
	static idVec4		  CreateVector( float x, float y, float z, float w );

	//! Creates and returns a new idVec3 object with the specified x, y, and z coordinates.
	static idVec3		  CreateVector( float x, float y, float z );

	//! Returns a random integer
	static int			  Rand() { return rand(); }

	//! Generates a random floating-point number between 0 and 1.
	static float		  FRand() { return Rand() / ( float )RAND_MAX; }

	//! Generates a random floating-point number within the specified range.
	static float		  FRandRange( float min, float max ) { return min + ( max - min ) * FRand(); }

	//! Computes the inverse square root of a floating-point value with 32-bit precision, returning a large number when the input is zero.
	static float		  InvSqrt( float x );

	//! Computes the inverse square root of the given float with 16 bits precision, returning a huge number when x is zero.
	static float		  InvSqrt16( float x );

	//! Computes the square root of a floating-point number with 32 bits precision.
	static float		  Sqrt( float x );

	//! Computes the square root of the given float value with 16 bits precision.
	static float		  Sqrt16( float x );

	//! Computes the sine of the given angle in radians with 32-bit precision.
	static float		  Sin( float a );

	//! Computes the sine of a float value with 16-bit precision.
	static float		  Sin16( float a );

	//! Computes the cosine of the given angle using 32-bit floating-point precision.
	static float		  Cos( float a );

	//! Computes the cosine of a float value with 16 bits precision
	static float		  Cos16( float a );

	//! Computes the sine and cosine of the given angle with 32-bit precision.
	static void			  SinCos( float a, float& s, float& c );

	//! Computes sine and cosine values with 16 bits precision.
	static void			  SinCos16( float a, float& s, float& c );

	//! Computes the tangent of the input angle using 32-bit precision.
	static float		  Tan( float a );

	//! Computes the tangent of a value with 16 bits precision.
	static float		  Tan16( float a );

	//! Returns the arc sine of the given value with input clamped to the range [-1, 1].
	static float		  ASin( float a );

	//! Computes the arc sine of a value with 16 bits precision.
	static float		  ASin16( float a );

	//! Computes the arc cosine of the input value with clamping to the range [-1, 1].
	static float		  ACos( float a );

	//! Computes the arc cosine with 16 bits precision, returning a float value.
	static float		  ACos16( float a );

	//! Computes the arc tangent of the given float value with 32-bit precision.
	static float		  ATan( float a );

	//! Computes the arc tangent of the input value with 16 bits precision.
	static float		  ATan16( float a );

	//! Computes the arc tangent of y and x with 32-bit precision.
	static float		  ATan( float y, float x );

	//! Computes the arc tangent with 16 bits precision, returning the angle in radians.
	static float		  ATan16( float y, float x );

	//! Computes x raised to the power y with 32-bit precision
	static float		  Pow( float x, float y );

	//! Computes x raised to the power y with 16 bits precision.
	static float		  Pow16( float x, float y );

	//! Computes e raised to the power of the given float value with 32-bit precision.
	static float		  Exp( float f );

	//! Computes the exponential function e raised to the power of f with 16 bits precision.
	static float		  Exp16( float f );

	//! Computes the natural logarithm of the given floating-point value with 32-bit precision.
	static float		  Log( float f );

	//! Computes the natural logarithm of a floating-point value with 16 bits precision.
	static float		  Log16( float f );

	//! Computes the integral value of x raised to the power y.
	static int			  IPow( int x, int y );

	//! Computes the integral base-2 logarithm of a floating point value.
	static int			  ILog2( float f );

	//! Computes the integral base-2 logarithm of an integer value.
	static int			  ILog2( int i );

	//! Returns the minimum number of bits required to represent the ceiling of the given floating-point value.
	static int			  BitsForFloat( float f );

	//! Returns the minimum number of bits required to represent the given integer value
	static int			  BitsForInteger( int i );

	//! Returns a mask indicating the sign of the input float value.
	static int			  MaskForFloatSign( float f );

	//! Returns a mask of all zeros or all ones based on the sign of the input integer.
	static int			  MaskForIntegerSign( int i );

	//! Rounds the input integer down to the nearest power of two.
	static int			  FloorPowerOfTwo( int x );

	//! Rounds the input integer up to the nearest power of two.
	static int			  CeilPowerOfTwo( int x );

	//! Returns true if the input integer is a power of two.
	static bool			  IsPowerOfTwo( int x );

	//! Returns the number of 1 bits in the input integer.
	static int			  BitCount( int x );

	//! Returns the bit reverse of the input integer.
	static int			  BitReverse( int x );

	//! Returns the absolute value of an integer.
	static int			  Abs( int x );

	//! Returns the absolute value of the given floating point number
	static float		  Fabs( float f );

	//! Returns the largest integer that is less than or equal to the given floating-point value
	static float		  Floor( float f );

	//! Returns the smallest integer that is greater than or equal to the given floating-point value.
	static float		  Ceil( float f );

	//! Returns the nearest integer to the given floating-point value
	static float		  Rint( float f );

	//! Returns the fractional part of a floating-point number.
	static float		  Frac( float f );

	//! Converts a floating-point value to an integer.
	static int			  Ftoi( float f );

	//! Converts a float value to a char, clamped to the range [-128, 127].
	static char			  Ftoi8( float f );

	//! Converts a float value to a short with clamping to the range [-32768, 32767].
	static short		  Ftoi16( float f );

	//! Converts a float value to an unsigned short, clamped to the range [0, 65535].
	static unsigned short Ftoui16( float f );

	//! Converts a float value to a byte, clamping the result to the range [0, 255]
	static byte			  Ftob( float f );

	//! Returns the smaller of two floating-point values.
	static float		  Fmin( float a, float b );

	//! Returns the minimum of three floating-point values.
	static float		  Fmin( float a, float b, float c ) { return Fmin( a, Fmin( b, c ) ); }

	//! Returns the maximum of two floating-point values.
	static float		  Fmax( float a, float b );

	//! Returns the smaller of two integer values.
	static int			  Imin( int a, int b );

	//! Returns the larger of two integer values.
	static int			  Imax( int a, int b );

	//! Clamps an integer value to the range of a signed char.
	static signed char	  ClampChar( int i );

	//! Clamps an integer value to the range of a signed short integer.
	static signed short	  ClampShort( int i );

	//! Clamps an integer value between a specified minimum and maximum range.
	static int			  ClampInt( int min, int max, int value );

	//! Clamps a float value between a specified minimum and maximum.
	static float		  ClampFloat( float min, float max, float value );

	//! Normalizes an angle to the range [0, 360).
	static float		  AngleNormalize360( float angle );

	//! Normalizes an angle to the range [-180, 180).
	static float		  AngleNormalize180( float angle );

	//! Calculates the angular difference between two angles normalized to the range [-180, 180).
	static float		  AngleDelta( float angle1, float angle2 );

	//! Converts a floating-point number to its bit representation with specified exponent and mantissa bits
	static int			  FloatToBits( float f, int exponentBits, int mantissaBits );

	//! Converts an integer representation with specified exponent and mantissa bits into a floating-point number.
	static float		  BitsToFloat( int i, int exponentBits, int mantissaBits );

	//! Computes a hash value for an array of floating-point numbers.
	static int			  FloatHash( const float* array, const int numFloats );

	//! Performs linear interpolation from cur to dest, scaled by the given factor, with early termination for small deltas.
	static float		  LerpToWithScale( const float cur, const float dest, const float scale );

	//! Checks if the given integer is a prime number.
	static bool			  IsPrime( int n );

	//! Returns the next prime number greater than the given integer.
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

//! Clamps an integer value to the byte range 0-255
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

//! Computes a linear interpolation between two values using a blending factor.
template<typename T>
ID_INLINE T Lerp( const T from, const T to, float f )
{
	return from + ( ( to - from ) * f );
}

//! Computes a linear interpolation between two integers using a floating-point factor
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

/*!
	\class rvRandom
	\brief A random number generator class implementing a custom rand() replacement with optimized float and integer operations.

	This class provides a deterministic random number generation implementation designed to minimize float to integer conversions and associated mathematical operations. It offers methods for
   generating both floating-point and integer random values within specified ranges. The class supports initialization with fixed or system-based seeds, enabling reproducible random sequences when
   needed. The implementation focuses on performance optimization while maintaining the standard behavior of a random number generator.

*/
class rvRandom
{
private:
	static unsigned long mSeed;

public:
	//! Initializes the random number generator with a fixed seed value.
	rvRandom() { mSeed = 0x89abcdef; }

	//! Initializes the random number generator with a seed based on system milliseconds.
	static int	 Init();

	//! Initializes the random number generator seed
	static void	 Init( unsigned long seed ) { mSeed = seed; }

	//! Returns a random floating-point number in the range [min, max).
	static float flrand( float min, float max );

	//! Returns a random floating-point number in the range [0.0, 1.0).
	static float flrand();

	//! Returns a random float value between the components of the given idVec2.
	static float flrand( const idVec2& v );

	//! Returns a random integer in the inclusive range [min, max].
	static int	 irand( int min, int max );
};

#endif /* !__MATH_MATH_H__ */
