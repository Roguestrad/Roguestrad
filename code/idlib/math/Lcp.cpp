/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

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
#include "precompiled.h"
#pragma hdrstop

// this file is full of intentional case fall throughs
// lint -e616

// the code is correct, it can't be a NULL pointer
// lint -e613

static idCVar lcp_showFailures( "lcp_showFailures", "0", CVAR_BOOL, "show LCP solver failures" );

const float	  LCP_BOUND_EPSILON		  = 1e-5f;
const float	  LCP_ACCEL_EPSILON		  = 1e-5f;
const float	  LCP_DELTA_ACCEL_EPSILON = 1e-9f;
const float	  LCP_DELTA_FORCE_EPSILON = 1e-9f;

#define IGNORE_UNSATISFIABLE_VARIABLES

#if defined( USE_INTRINSICS_SSE )
	#define LCP_SIMD
#endif

#if defined( LCP_SIMD )
ALIGN16( const __m128 SIMD_SP_zero )					= { 0.0f, 0.0f, 0.0f, 0.0f };
ALIGN16( const __m128 SIMD_SP_one )						= { 1.0f, 1.0f, 1.0f, 1.0f };
ALIGN16( const __m128 SIMD_SP_two )						= { 2.0f, 2.0f, 2.0f, 2.0f };
ALIGN16( const __m128 SIMD_SP_tiny )					= { 1e-10f, 1e-10f, 1e-10f, 1e-10f };
ALIGN16( const __m128 SIMD_SP_infinity )				= { idMath::INFINITUM, idMath::INFINITUM, idMath::INFINITUM, idMath::INFINITUM };
ALIGN16( const __m128 SIMD_SP_LCP_DELTA_ACCEL_EPSILON ) = { LCP_DELTA_ACCEL_EPSILON, LCP_DELTA_ACCEL_EPSILON, LCP_DELTA_ACCEL_EPSILON, LCP_DELTA_ACCEL_EPSILON };
ALIGN16( const __m128 SIMD_SP_LCP_DELTA_FORCE_EPSILON ) = { LCP_DELTA_FORCE_EPSILON, LCP_DELTA_FORCE_EPSILON, LCP_DELTA_FORCE_EPSILON, LCP_DELTA_FORCE_EPSILON };
ALIGN16( const __m128 SIMD_SP_LCP_BOUND_EPSILON )		= { LCP_BOUND_EPSILON, LCP_BOUND_EPSILON, LCP_BOUND_EPSILON, LCP_BOUND_EPSILON };
ALIGN16( const __m128 SIMD_SP_neg_LCP_BOUND_EPSILON )	= { -LCP_BOUND_EPSILON, -LCP_BOUND_EPSILON, -LCP_BOUND_EPSILON, -LCP_BOUND_EPSILON };
ALIGN16( const unsigned int SIMD_SP_signBit[4] )		= { IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK, IEEE_FLT_SIGN_MASK };
	// DG: cast IEEE_FLT_SIGN_MASK to uint before interting it, as uint and not long values are expected here
	#define INV_IEEE_FLT_SIGN_MASK ( ~( ( unsigned int )IEEE_FLT_SIGN_MASK ) )
ALIGN16( const unsigned int SIMD_SP_absMask[4] ) = { INV_IEEE_FLT_SIGN_MASK, INV_IEEE_FLT_SIGN_MASK, INV_IEEE_FLT_SIGN_MASK, INV_IEEE_FLT_SIGN_MASK };
// DG end
ALIGN16( const unsigned int SIMD_SP_indexedStartMask[4][4] ) = {
	{ 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF }, { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF }, { 0, 0, 0xFFFFFFFF, 0xFFFFFFFF }, { 0, 0, 0, 0xFFFFFFFF }
};
ALIGN16( const unsigned int SIMD_SP_indexedEndMask[4][4] ) = { { 0, 0, 0, 0 }, { 0xFFFFFFFF, 0, 0, 0 }, { 0xFFFFFFFF, 0xFFFFFFFF, 0, 0 }, { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0 } };
ALIGN16( const unsigned int SIMD_SP_clearLast1[4] )		   = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0 };
ALIGN16( const unsigned int SIMD_SP_clearLast2[4] )		   = { 0xFFFFFFFF, 0xFFFFFFFF, 0, 0 };
ALIGN16( const unsigned int SIMD_SP_clearLast3[4] )		   = { 0xFFFFFFFF, 0, 0, 0 };
ALIGN16( const unsigned int SIMD_DW_zero[4] )			   = { 0, 0, 0, 0 };
ALIGN16( const unsigned int SIMD_DW_one[4] )			   = { 1, 1, 1, 1 };
ALIGN16( const unsigned int SIMD_DW_four[4] )			   = { 4, 4, 4, 4 };
ALIGN16( const unsigned int SIMD_DW_index[4] )			   = { 0, 1, 2, 3 };
ALIGN16( const int SIMD_DW_not3[4] )					   = { ~3, ~3, ~3, ~3 };
#endif // #if defined(LCP_SIMD)

/*!
	\brief Multiplies two arrays of floats element-wise using SIMD optimization

	This function performs element-wise multiplication of two float arrays, optimizing performance by using SIMD instructions when available. It handles alignment requirements for SIMD operations and
   falls back to scalar multiplication for cases where SIMD cannot be used or when alignment is not met. The function processes elements in chunks of four when SIMD is enabled, and processes remaining
   elements individually after the SIMD loop.

	\param dst destination array where the results are stored
	\param src0 first source array of floats
	\param src1 second source array of floats
	\param count number of elements to process in the arrays
*/
static void Multiply_SIMD( float* dst, const float* src0, const float* src1, const int count )
{
	int i = 0;

	// RB: changed unsigned int to uintptr_t
	for( ; ( ( uintptr_t )dst & 0xF ) != 0 && i < count; i++ )
	// RB end
	{
		dst[i] = src0[i] * src1[i];
	}

#if defined( LCP_SIMD )

	for( ; i + 4 <= count; i += 4 ) {
		assert_16_byte_aligned( &dst[i] );
		assert_16_byte_aligned( &src0[i] );
		assert_16_byte_aligned( &src1[i] );

		__m128 s0 = _mm_load_ps( src0 + i );
		__m128 s1 = _mm_load_ps( src1 + i );
		s0		  = _mm_mul_ps( s0, s1 );
		_mm_store_ps( dst + i, s0 );
	}

#else

	for( ; i + 4 <= count; i += 4 ) {
		assert_16_byte_aligned( &dst[i] );
		assert_16_byte_aligned( &src0[i] );
		assert_16_byte_aligned( &src1[i] );

		dst[i + 0] = src0[i + 0] * src1[i + 0];
		dst[i + 1] = src0[i + 1] * src1[i + 1];
		dst[i + 2] = src0[i + 2] * src1[i + 2];
		dst[i + 3] = src0[i + 3] * src1[i + 3];
	}

#endif

	for( ; i < count; i++ ) {
		dst[i] = src0[i] * src1[i];
	}
}

/*!
	\brief Performs SIMD-optimized multiply-add operations on arrays of floating-point numbers

	This function applies a multiply-add operation to two arrays of floating-point numbers, where each element of the destination array is updated by adding the product of the corresponding source
   element and a constant value. The function is optimized using SIMD instructions when available, with special handling for memory alignment. It processes elements in chunks of four when possible,
   using either SSE instructions or standard scalar operations depending on SIMD support.

	\param dst Destination array where results are stored
	\param constant Scalar value multiplied with source elements
	\param src Source array of floating-point values
	\param count Number of elements to process in both arrays
	\throws assertion failure if memory alignment requirements are not met
*/
static void MultiplyAdd_SIMD( float* dst, const float constant, const float* src, const int count )
{
	int i = 0;

	// RB: changed unsigned int to uintptr_t
	for( ; ( ( uintptr_t )dst & 0xF ) != 0 && i < count; i++ )
	// RB end
	{
		dst[i] += constant * src[i];
	}

#if defined( LCP_SIMD )

	__m128 c = _mm_load1_ps( &constant );
	for( ; i + 4 <= count; i += 4 ) {
		assert_16_byte_aligned( &dst[i] );
		assert_16_byte_aligned( &src[i] );

		__m128 s = _mm_load_ps( src + i );
		__m128 d = _mm_load_ps( dst + i );
		s		 = _mm_add_ps( _mm_mul_ps( s, c ), d );
		_mm_store_ps( dst + i, s );
	}

#else

	for( ; i + 4 <= count; i += 4 ) {
		assert_16_byte_aligned( &src[i] );
		assert_16_byte_aligned( &dst[i] );

		dst[i + 0] += constant * src[i + 0];
		dst[i + 1] += constant * src[i + 1];
		dst[i + 2] += constant * src[i + 2];
		dst[i + 3] += constant * src[i + 3];
	}

#endif

	for( ; i < count; i++ ) {
		dst[i] += constant * src[i];
	}
}

/*!
	\brief Computes the dot product of two float arrays using SIMD optimization when available

	This function calculates the dot product of two float arrays of a specified count using SIMD instructions for optimized performance. When SIMD is not available, it falls back to a standard loop
   implementation. The function assumes that both input arrays are 16-byte aligned for optimal SIMD performance.

	\param src0 Pointer to the first float array
	\param src1 Pointer to the second float array
	\param count Number of elements to process in both arrays
	\return The computed dot product of the two input arrays
	\throws Assertion failure if the input arrays are not 16-byte aligned
*/
static float DotProduct_SIMD( const float* src0, const float* src1, const int count )
{
	assert_16_byte_aligned( src0 );
	assert_16_byte_aligned( src1 );

#if defined( LCP_SIMD )

	__m128 sum = ( __m128& )SIMD_SP_zero;
	int	   i   = 0;
	for( ; i < count - 3; i += 4 ) {
		__m128 s0 = _mm_load_ps( src0 + i );
		__m128 s1 = _mm_load_ps( src1 + i );
		sum		  = _mm_add_ps( _mm_mul_ps( s0, s1 ), sum );
	}
	__m128 mask = _mm_load_ps( ( float* )&SIMD_SP_indexedEndMask[count & 3] );
	__m128 s0	= _mm_and_ps( _mm_load_ps( src0 + i ), mask );
	__m128 s1	= _mm_and_ps( _mm_load_ps( src1 + i ), mask );
	sum			= _mm_add_ps( _mm_mul_ps( s0, s1 ), sum );
	sum			= _mm_add_ps( sum, _mm_shuffle_ps( sum, sum, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
	sum			= _mm_add_ps( sum, _mm_shuffle_ps( sum, sum, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
	float dot;
	_mm_store_ss( &dot, sum );
	return dot;

#else

	// RB: the old loop caused completely broken rigid body physics and NaN errors
	#if 1
	float s0 = 0.0f;
	float s1 = 0.0f;
	float s2 = 0.0f;
	float s3 = 0.0f;

	int	  i = 0;
	for( ; i + 4 <= count; i += 4 ) {
		s0 += src0[i + 0] * src1[i + 0];
		s1 += src0[i + 1] * src1[i + 1];
		s2 += src0[i + 2] * src1[i + 2];
		s3 += src0[i + 3] * src1[i + 3];
	}

	switch( count - i ) {
		NODEFAULT;

		case 4:
			s3 += src0[i + 3] * src1[i + 3];
		case 3:
			s2 += src0[i + 2] * src1[i + 2];
		case 2:
			s1 += src0[i + 1] * src1[i + 1];
		case 1:
			s0 += src0[i + 0] * src1[i + 0];
		case 0:
			break;
	}
	return s0 + s1 + s2 + s3;
	#else

	float s = 0;
	for( int i = 0; i < count; i++ ) {
		s += src0[i] * src1[i];
	}

	return s;
	#endif
	// RB end

#endif
}

/*!
	\brief Solves a lower triangular system of equations using SIMD optimization

	This function performs forward substitution to solve a lower triangular matrix equation of the form L * x = b, where L is a lower triangular matrix, x is the unknown vector, and b is the known
   right-hand side vector. The implementation uses SIMD instructions for performance optimization. The function handles special cases for small matrix sizes by using switch statements to avoid loop
   overhead, and for larger matrices, it processes rows in batches of four using SSE instructions. The skip parameter allows for partial solving of the system starting from a specified row index. The
   matrix L is assumed to be stored in row-major order with a column count that is a multiple of 4.

	\param L Lower triangular matrix stored in row-major order
	\param x Solution vector to be filled
	\param b Right-hand side vector
	\param n Number of rows to solve
	\param skip Row index to start solving from
	\throws assertion failure if the number of columns in L is not a multiple of 4
*/
static void LowerTriangularSolve_SIMD( const idMatX& L, float* x, const float* b, const int n, int skip )
{
	if( skip >= n ) {
		return;
	}

	const float* lptr = L.ToFloatPtr();
	int			 nc	  = L.GetNumColumns();

	assert( ( nc & 3 ) == 0 );

	// unrolled cases for n < 8
	if( n < 8 ) {
#define NSKIP( n, s ) ( ( n << 3 ) | ( s & 7 ) )
		switch( NSKIP( n, skip ) ) {
			case NSKIP( 1, 0 ):
				x[0] = b[0];
				return;
			case NSKIP( 2, 0 ):
				x[0] = b[0];
			case NSKIP( 2, 1 ):
				x[1] = b[1] - lptr[1 * nc + 0] * x[0];
				return;
			case NSKIP( 3, 0 ):
				x[0] = b[0];
			case NSKIP( 3, 1 ):
				x[1] = b[1] - lptr[1 * nc + 0] * x[0];
			case NSKIP( 3, 2 ):
				x[2] = b[2] - lptr[2 * nc + 0] * x[0] - lptr[2 * nc + 1] * x[1];
				return;
			case NSKIP( 4, 0 ):
				x[0] = b[0];
			case NSKIP( 4, 1 ):
				x[1] = b[1] - lptr[1 * nc + 0] * x[0];
			case NSKIP( 4, 2 ):
				x[2] = b[2] - lptr[2 * nc + 0] * x[0] - lptr[2 * nc + 1] * x[1];
			case NSKIP( 4, 3 ):
				x[3] = b[3] - lptr[3 * nc + 0] * x[0] - lptr[3 * nc + 1] * x[1] - lptr[3 * nc + 2] * x[2];
				return;
			case NSKIP( 5, 0 ):
				x[0] = b[0];
			case NSKIP( 5, 1 ):
				x[1] = b[1] - lptr[1 * nc + 0] * x[0];
			case NSKIP( 5, 2 ):
				x[2] = b[2] - lptr[2 * nc + 0] * x[0] - lptr[2 * nc + 1] * x[1];
			case NSKIP( 5, 3 ):
				x[3] = b[3] - lptr[3 * nc + 0] * x[0] - lptr[3 * nc + 1] * x[1] - lptr[3 * nc + 2] * x[2];
			case NSKIP( 5, 4 ):
				x[4] = b[4] - lptr[4 * nc + 0] * x[0] - lptr[4 * nc + 1] * x[1] - lptr[4 * nc + 2] * x[2] - lptr[4 * nc + 3] * x[3];
				return;
			case NSKIP( 6, 0 ):
				x[0] = b[0];
			case NSKIP( 6, 1 ):
				x[1] = b[1] - lptr[1 * nc + 0] * x[0];
			case NSKIP( 6, 2 ):
				x[2] = b[2] - lptr[2 * nc + 0] * x[0] - lptr[2 * nc + 1] * x[1];
			case NSKIP( 6, 3 ):
				x[3] = b[3] - lptr[3 * nc + 0] * x[0] - lptr[3 * nc + 1] * x[1] - lptr[3 * nc + 2] * x[2];
			case NSKIP( 6, 4 ):
				x[4] = b[4] - lptr[4 * nc + 0] * x[0] - lptr[4 * nc + 1] * x[1] - lptr[4 * nc + 2] * x[2] - lptr[4 * nc + 3] * x[3];
			case NSKIP( 6, 5 ):
				x[5] = b[5] - lptr[5 * nc + 0] * x[0] - lptr[5 * nc + 1] * x[1] - lptr[5 * nc + 2] * x[2] - lptr[5 * nc + 3] * x[3] - lptr[5 * nc + 4] * x[4];
				return;
			case NSKIP( 7, 0 ):
				x[0] = b[0];
			case NSKIP( 7, 1 ):
				x[1] = b[1] - lptr[1 * nc + 0] * x[0];
			case NSKIP( 7, 2 ):
				x[2] = b[2] - lptr[2 * nc + 0] * x[0] - lptr[2 * nc + 1] * x[1];
			case NSKIP( 7, 3 ):
				x[3] = b[3] - lptr[3 * nc + 0] * x[0] - lptr[3 * nc + 1] * x[1] - lptr[3 * nc + 2] * x[2];
			case NSKIP( 7, 4 ):
				x[4] = b[4] - lptr[4 * nc + 0] * x[0] - lptr[4 * nc + 1] * x[1] - lptr[4 * nc + 2] * x[2] - lptr[4 * nc + 3] * x[3];
			case NSKIP( 7, 5 ):
				x[5] = b[5] - lptr[5 * nc + 0] * x[0] - lptr[5 * nc + 1] * x[1] - lptr[5 * nc + 2] * x[2] - lptr[5 * nc + 3] * x[3] - lptr[5 * nc + 4] * x[4];
			case NSKIP( 7, 6 ):
				x[6] = b[6] - lptr[6 * nc + 0] * x[0] - lptr[6 * nc + 1] * x[1] - lptr[6 * nc + 2] * x[2] - lptr[6 * nc + 3] * x[3] - lptr[6 * nc + 4] * x[4] - lptr[6 * nc + 5] * x[5];
				return;
		}
#undef NSKIP
		return;
	}

	// process first 4 rows
	switch( skip ) {
		case 0:
			x[0] = b[0];
		case 1:
			x[1] = b[1] - lptr[1 * nc + 0] * x[0];
		case 2:
			x[2] = b[2] - lptr[2 * nc + 0] * x[0] - lptr[2 * nc + 1] * x[1];
		case 3:
			x[3] = b[3] - lptr[3 * nc + 0] * x[0] - lptr[3 * nc + 1] * x[1] - lptr[3 * nc + 2] * x[2];
			skip = 4;
	}

	lptr = L[skip];

	int i = skip;

#if defined( LCP_SIMD )

	// work up to a multiple of 4 rows
	for( ; ( i & 3 ) != 0 && i < n; i++ ) {
		__m128 sum = _mm_load_ss( &b[i] );
		int	   j   = 0;
		for( ; j < i - 3; j += 4 ) {
			__m128 s0 = _mm_load_ps( lptr + j );
			__m128 s1 = _mm_load_ps( x + j );
			sum		  = _mm_sub_ps( sum, _mm_mul_ps( s0, s1 ) );
		}
		__m128 mask = _mm_load_ps( ( float* )&SIMD_SP_indexedEndMask[i & 3] );
		__m128 s0	= _mm_and_ps( _mm_load_ps( lptr + j ), mask );
		__m128 s1	= _mm_and_ps( _mm_load_ps( x + j ), mask );
		sum			= _mm_sub_ps( sum, _mm_mul_ps( s0, s1 ) );
		sum			= _mm_add_ps( sum, _mm_shuffle_ps( sum, sum, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
		sum			= _mm_add_ps( sum, _mm_shuffle_ps( sum, sum, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
		_mm_store_ss( &x[i], sum );
		lptr += nc;
	}

	for( ; i + 3 < n; i += 4 ) {
		const float* lptr0 = &lptr[0 * nc];
		const float* lptr1 = &lptr[1 * nc];
		const float* lptr2 = &lptr[2 * nc];
		const float* lptr3 = &lptr[3 * nc];

		assert_16_byte_aligned( lptr0 );
		assert_16_byte_aligned( lptr1 );
		assert_16_byte_aligned( lptr2 );
		assert_16_byte_aligned( lptr3 );

		__m128 va = _mm_load_ss( &b[i + 0] );
		__m128 vb = _mm_load_ss( &b[i + 1] );
		__m128 vc = _mm_load_ss( &b[i + 2] );
		__m128 vd = _mm_load_ss( &b[i + 3] );

		__m128 x0 = _mm_load_ps( &x[0] );

		va = _mm_sub_ps( va, _mm_mul_ps( x0, _mm_load_ps( lptr0 + 0 ) ) );
		vb = _mm_sub_ps( vb, _mm_mul_ps( x0, _mm_load_ps( lptr1 + 0 ) ) );
		vc = _mm_sub_ps( vc, _mm_mul_ps( x0, _mm_load_ps( lptr2 + 0 ) ) );
		vd = _mm_sub_ps( vd, _mm_mul_ps( x0, _mm_load_ps( lptr3 + 0 ) ) );

		for( int j = 4; j < i; j += 4 ) {
			__m128 xj = _mm_load_ps( &x[j] );

			va = _mm_sub_ps( va, _mm_mul_ps( xj, _mm_load_ps( lptr0 + j ) ) );
			vb = _mm_sub_ps( vb, _mm_mul_ps( xj, _mm_load_ps( lptr1 + j ) ) );
			vc = _mm_sub_ps( vc, _mm_mul_ps( xj, _mm_load_ps( lptr2 + j ) ) );
			vd = _mm_sub_ps( vd, _mm_mul_ps( xj, _mm_load_ps( lptr3 + j ) ) );
		}

		vb = _mm_sub_ps( vb, _mm_mul_ps( va, _mm_load1_ps( lptr1 + i + 0 ) ) );
		vc = _mm_sub_ps( vc, _mm_mul_ps( va, _mm_load1_ps( lptr2 + i + 0 ) ) );
		vc = _mm_sub_ps( vc, _mm_mul_ps( vb, _mm_load1_ps( lptr2 + i + 1 ) ) );
		vd = _mm_sub_ps( vd, _mm_mul_ps( va, _mm_load1_ps( lptr3 + i + 0 ) ) );
		vd = _mm_sub_ps( vd, _mm_mul_ps( vb, _mm_load1_ps( lptr3 + i + 1 ) ) );
		vd = _mm_sub_ps( vd, _mm_mul_ps( vc, _mm_load1_ps( lptr3 + i + 2 ) ) );

		__m128 ta = _mm_unpacklo_ps( va, vc ); // x0, z0, x1, z1
		__m128 tb = _mm_unpackhi_ps( va, vc ); // x2, z2, x3, z3
		__m128 tc = _mm_unpacklo_ps( vb, vd ); // y0, w0, y1, w1
		__m128 td = _mm_unpackhi_ps( vb, vd ); // y2, w2, y3, w3

		va = _mm_unpacklo_ps( ta, tc ); // x0, y0, z0, w0
		vb = _mm_unpackhi_ps( ta, tc ); // x1, y1, z1, w1
		vc = _mm_unpacklo_ps( tb, td ); // x2, y2, z2, w2
		vd = _mm_unpackhi_ps( tb, td ); // x3, y3, z3, w3

		va = _mm_add_ps( va, vb );
		vc = _mm_add_ps( vc, vd );
		va = _mm_add_ps( va, vc );

		_mm_store_ps( &x[i], va );

		lptr += 4 * nc;
	}

	// go through any remaining rows
	for( ; i < n; i++ ) {
		__m128 sum = _mm_load_ss( &b[i] );
		int	   j   = 0;
		for( ; j < i - 3; j += 4 ) {
			__m128 s0 = _mm_load_ps( lptr + j );
			__m128 s1 = _mm_load_ps( x + j );
			sum		  = _mm_sub_ps( sum, _mm_mul_ps( s0, s1 ) );
		}
		__m128 mask = _mm_load_ps( ( float* )&SIMD_SP_indexedEndMask[i & 3] );
		__m128 s0	= _mm_and_ps( _mm_load_ps( lptr + j ), mask );
		__m128 s1	= _mm_and_ps( _mm_load_ps( x + j ), mask );
		sum			= _mm_sub_ps( sum, _mm_mul_ps( s0, s1 ) );
		sum			= _mm_add_ps( sum, _mm_shuffle_ps( sum, sum, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
		sum			= _mm_add_ps( sum, _mm_shuffle_ps( sum, sum, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
		_mm_store_ss( &x[i], sum );
		lptr += nc;
	}

#else

	// work up to a multiple of 4 rows
	for( ; ( i & 3 ) != 0 && i < n; i++ ) {
		float sum = b[i];
		for( int j = 0; j < i; j++ ) {
			sum -= lptr[j] * x[j];
		}
		x[i] = sum;
		lptr += nc;
	}

	assert_16_byte_aligned( x );

	for( ; i + 3 < n; i += 4 ) {
		const float* lptr0 = &lptr[0 * nc];
		const float* lptr1 = &lptr[1 * nc];
		const float* lptr2 = &lptr[2 * nc];
		const float* lptr3 = &lptr[3 * nc];

		assert_16_byte_aligned( lptr0 );
		assert_16_byte_aligned( lptr1 );
		assert_16_byte_aligned( lptr2 );
		assert_16_byte_aligned( lptr3 );

		float a0 = -lptr0[0] * x[0] + b[i + 0];
		float a1 = -lptr0[1] * x[1];
		float a2 = -lptr0[2] * x[2];
		float a3 = -lptr0[3] * x[3];

		float b0 = -lptr1[0] * x[0] + b[i + 1];
		float b1 = -lptr1[1] * x[1];
		float b2 = -lptr1[2] * x[2];
		float b3 = -lptr1[3] * x[3];

		float c0 = -lptr2[0] * x[0] + b[i + 2];
		float c1 = -lptr2[1] * x[1];
		float c2 = -lptr2[2] * x[2];
		float c3 = -lptr2[3] * x[3];

		float d0 = -lptr3[0] * x[0] + b[i + 3];
		float d1 = -lptr3[1] * x[1];
		float d2 = -lptr3[2] * x[2];
		float d3 = -lptr3[3] * x[3];

		for( int j = 4; j < i; j += 4 ) {
			a0 -= lptr0[j + 0] * x[j + 0];
			a1 -= lptr0[j + 1] * x[j + 1];
			a2 -= lptr0[j + 2] * x[j + 2];
			a3 -= lptr0[j + 3] * x[j + 3];

			b0 -= lptr1[j + 0] * x[j + 0];
			b1 -= lptr1[j + 1] * x[j + 1];
			b2 -= lptr1[j + 2] * x[j + 2];
			b3 -= lptr1[j + 3] * x[j + 3];

			c0 -= lptr2[j + 0] * x[j + 0];
			c1 -= lptr2[j + 1] * x[j + 1];
			c2 -= lptr2[j + 2] * x[j + 2];
			c3 -= lptr2[j + 3] * x[j + 3];

			d0 -= lptr3[j + 0] * x[j + 0];
			d1 -= lptr3[j + 1] * x[j + 1];
			d2 -= lptr3[j + 2] * x[j + 2];
			d3 -= lptr3[j + 3] * x[j + 3];
		}

		b0 -= lptr1[i + 0] * a0;
		b1 -= lptr1[i + 0] * a1;
		b2 -= lptr1[i + 0] * a2;
		b3 -= lptr1[i + 0] * a3;

		c0 -= lptr2[i + 0] * a0;
		c1 -= lptr2[i + 0] * a1;
		c2 -= lptr2[i + 0] * a2;
		c3 -= lptr2[i + 0] * a3;

		c0 -= lptr2[i + 1] * b0;
		c1 -= lptr2[i + 1] * b1;
		c2 -= lptr2[i + 1] * b2;
		c3 -= lptr2[i + 1] * b3;

		d0 -= lptr3[i + 0] * a0;
		d1 -= lptr3[i + 0] * a1;
		d2 -= lptr3[i + 0] * a2;
		d3 -= lptr3[i + 0] * a3;

		d0 -= lptr3[i + 1] * b0;
		d1 -= lptr3[i + 1] * b1;
		d2 -= lptr3[i + 1] * b2;
		d3 -= lptr3[i + 1] * b3;

		d0 -= lptr3[i + 2] * c0;
		d1 -= lptr3[i + 2] * c1;
		d2 -= lptr3[i + 2] * c2;
		d3 -= lptr3[i + 2] * c3;

		x[i + 0] = a0 + a1 + a2 + a3;
		x[i + 1] = b0 + b1 + b2 + b3;
		x[i + 2] = c0 + c1 + c2 + c3;
		x[i + 3] = d0 + d1 + d2 + d3;

		lptr += 4 * nc;
	}

	// go through any remaining rows
	for( ; i < n; i++ ) {
		float sum = b[i];
		for( int j = 0; j < i; j++ ) {
			sum -= lptr[j] * x[j];
		}
		x[i] = sum;
		lptr += nc;
	}

#endif
}

/*!
	\brief Solves the linear system L'x = b for a lower triangular matrix L using SIMD optimization

	This function performs backward substitution to solve a linear system where L is a lower triangular matrix and the operation is performed on the transpose of L. The implementation uses SIMD
   instructions for optimized performance when processing four rows at a time. It handles both aligned and unaligned memory access patterns and includes special handling for cases where the number of
   rows is not a multiple of four.

	\param L Lower triangular matrix used in the linear system
	\param x Solution vector to be computed
	\param b Right-hand side vector of the linear system
	\param n Number of rows/columns in the matrix
	\throws assertion failure if the number of columns in matrix L is not a multiple of 4
*/
static void LowerTriangularSolveTranspose_SIMD( const idMatX& L, float* x, const float* b, const int n )
{
	int nc = L.GetNumColumns();

	assert( ( nc & 3 ) == 0 );

	int m = n;
	int r = n & 3;

	if( ( m & 3 ) != 0 ) {
		const float* lptr = L.ToFloatPtr() + m * nc + m;
		if( ( m & 3 ) == 1 ) {
			x[m - 1] = b[m - 1];
			m -= 1;
		} else if( ( m & 3 ) == 2 ) {
			x[m - 1] = b[m - 1];
			x[m - 2] = b[m - 2] - lptr[-1 * nc - 2] * x[m - 1];
			m -= 2;
		} else {
			x[m - 1] = b[m - 1];
			x[m - 2] = b[m - 2] - lptr[-1 * nc - 2] * x[m - 1];
			x[m - 3] = b[m - 3] - lptr[-1 * nc - 3] * x[m - 1] - lptr[-2 * nc - 3] * x[m - 2];
			m -= 3;
		}
	}

	const float* lptr = L.ToFloatPtr() + m * nc + m - 4;
	float*		 xptr = x + m;

#if defined( LCP_SIMD )

	// process 4 rows at a time
	for( int i = m; i >= 4; i -= 4 ) {
		assert_16_byte_aligned( b );
		assert_16_byte_aligned( xptr );
		assert_16_byte_aligned( lptr );

		__m128		 s0 = _mm_load_ps( &b[i - 4] );
		__m128		 s1 = ( __m128& )SIMD_SP_zero;
		__m128		 s2 = ( __m128& )SIMD_SP_zero;
		__m128		 s3 = ( __m128& )SIMD_SP_zero;

		// process 4x4 blocks
		const float* xptr2 = xptr; // x + i;
		const float* lptr2 = lptr; // ptr = L[i] + i - 4;
		for( int j = i; j < m; j += 4 ) {
			__m128 xj = _mm_load_ps( xptr2 );

			s0 = _mm_sub_ps( s0, _mm_mul_ps( _mm_splat_ps( xj, 0 ), _mm_load_ps( lptr2 + 0 * nc ) ) );
			s1 = _mm_sub_ps( s1, _mm_mul_ps( _mm_splat_ps( xj, 1 ), _mm_load_ps( lptr2 + 1 * nc ) ) );
			s2 = _mm_sub_ps( s2, _mm_mul_ps( _mm_splat_ps( xj, 2 ), _mm_load_ps( lptr2 + 2 * nc ) ) );
			s3 = _mm_sub_ps( s3, _mm_mul_ps( _mm_splat_ps( xj, 3 ), _mm_load_ps( lptr2 + 3 * nc ) ) );

			lptr2 += 4 * nc;
			xptr2 += 4;
		}
		for( int j = 0; j < r; j++ ) {
			s0 = _mm_sub_ps( s0, _mm_mul_ps( _mm_load_ps( lptr2 ), _mm_load1_ps( &xptr2[j] ) ) );
			lptr2 += nc;
		}
		s0 = _mm_add_ps( s0, s1 );
		s2 = _mm_add_ps( s2, s3 );
		s0 = _mm_add_ps( s0, s2 );
		// process left over of the 4 rows
		lptr -= 4 * nc;
		__m128 t0 = _mm_and_ps( _mm_load_ps( lptr + 3 * nc ), ( __m128& )SIMD_SP_clearLast1 );
		__m128 t1 = _mm_and_ps( _mm_load_ps( lptr + 2 * nc ), ( __m128& )SIMD_SP_clearLast2 );
		__m128 t2 = _mm_load_ss( lptr + 1 * nc );
		s0		  = _mm_sub_ps( s0, _mm_mul_ps( t0, _mm_splat_ps( s0, 3 ) ) );
		s0		  = _mm_sub_ps( s0, _mm_mul_ps( t1, _mm_splat_ps( s0, 2 ) ) );
		s0		  = _mm_sub_ps( s0, _mm_mul_ps( t2, _mm_splat_ps( s0, 1 ) ) );
		// store result
		_mm_store_ps( &xptr[-4], s0 );
		// update pointers for next four rows
		lptr -= 4;
		xptr -= 4;
	}

#else

	// process 4 rows at a time
	for( int i = m; i >= 4; i -= 4 ) {
		assert_16_byte_aligned( b );
		assert_16_byte_aligned( xptr );
		assert_16_byte_aligned( lptr );

		float		 s0 = b[i - 4];
		float		 s1 = b[i - 3];
		float		 s2 = b[i - 2];
		float		 s3 = b[i - 1];
		// process 4x4 blocks
		const float* xptr2 = xptr; // x + i;
		const float* lptr2 = lptr; // ptr = L[i] + i - 4;
		for( int j = i; j < m; j += 4 ) {
			float t0 = xptr2[0];
			s0 -= lptr2[0] * t0;
			s1 -= lptr2[1] * t0;
			s2 -= lptr2[2] * t0;
			s3 -= lptr2[3] * t0;
			lptr2 += nc;
			float t1 = xptr2[1];
			s0 -= lptr2[0] * t1;
			s1 -= lptr2[1] * t1;
			s2 -= lptr2[2] * t1;
			s3 -= lptr2[3] * t1;
			lptr2 += nc;
			float t2 = xptr2[2];
			s0 -= lptr2[0] * t2;
			s1 -= lptr2[1] * t2;
			s2 -= lptr2[2] * t2;
			s3 -= lptr2[3] * t2;
			lptr2 += nc;
			float t3 = xptr2[3];
			s0 -= lptr2[0] * t3;
			s1 -= lptr2[1] * t3;
			s2 -= lptr2[2] * t3;
			s3 -= lptr2[3] * t3;
			lptr2 += nc;
			xptr2 += 4;
		}
		for( int j = 0; j < r; j++ ) {
			float t = xptr2[j];
			s0 -= lptr2[0] * t;
			s1 -= lptr2[1] * t;
			s2 -= lptr2[2] * t;
			s3 -= lptr2[3] * t;
			lptr2 += nc;
		}
		// process left over of the 4 rows
		lptr -= nc;
		s0 -= lptr[0] * s3;
		s1 -= lptr[1] * s3;
		s2 -= lptr[2] * s3;
		lptr -= nc;
		s0 -= lptr[0] * s2;
		s1 -= lptr[1] * s2;
		lptr -= nc;
		s0 -= lptr[0] * s1;
		lptr -= nc;
		// store result
		xptr[-4] = s0;
		xptr[-3] = s1;
		xptr[-2] = s2;
		xptr[-1] = s3;
		// update pointers for next four rows
		lptr -= 4;
		xptr -= 4;
	}

#endif
}

/*!
	\brief Solves an upper triangular system of linear equations using SIMD optimization

	This function performs backward substitution to solve an upper triangular system of linear equations represented by the matrix U. It uses the inverse diagonal elements for optimization and
   processes the system in reverse order from the last row to the first. The function implements SIMD optimization for improved performance.

	\param U The upper triangular matrix containing the system coefficients
	\param invDiag Array of inverse diagonal elements of matrix U
	\param x Output array to store the solution vector
	\param b Input array containing the right-hand side values
	\param n The dimension of the square matrix U and vectors x, b
*/
static void UpperTriangularSolve_SIMD( const idMatX& U, const float* invDiag, float* x, const float* b, const int n )
{
	for( int i = n - 1; i >= 0; i-- ) {
		float		 sum  = b[i];
		const float* uptr = U[i];
		for( int j = i + 1; j < n; j++ ) {
			sum -= uptr[j] * x[j];
		}
		x[i] = sum * invDiag[i];
	}
}

/*!
	\brief Performs in-place LU factorization on a sub-matrix using SIMD optimization and returns true if successful.

	This function implements LU factorization for a matrix sub-matrix of size n by n. It modifies the input matrix in-place to store the LU decomposition. The diagonal elements are inverted and stored
   in the invDiag vector. The implementation uses SIMD optimizations to improve performance during the factorization process. The function checks for numerical stability by verifying that diagonal
   elements are not too close to zero.

	\param mat The matrix to be factorized, modified in-place to contain the LU decomposition
	\param invDiag Vector to store the inverse of diagonal elements from the factorized matrix
	\param n The size of the square sub-matrix to factorize
	\return True if the factorization was successful, false if a diagonal element was too close to zero to be invertible.
*/
static bool LU_Factor_SIMD( idMatX& mat, idVecX& invDiag, const int n )
{
	for( int i = 0; i < n; i++ ) {
		float d1 = mat[i][i];

		if( fabs( d1 ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
			return false;
		}

		invDiag[i] = d1 = 1.0f / d1;

		float* ptr1 = mat[i];

		for( int j = i + 1; j < n; j++ ) {
			float* ptr2 = mat[j];
			float  d2	= ptr2[i] * d1;
			ptr2[i]		= d2;

			int k;
			for( k = n - 1; k > i + 15; k -= 16 ) {
				ptr2[k - 0] -= d2 * ptr1[k - 0];
				ptr2[k - 1] -= d2 * ptr1[k - 1];
				ptr2[k - 2] -= d2 * ptr1[k - 2];
				ptr2[k - 3] -= d2 * ptr1[k - 3];
				ptr2[k - 4] -= d2 * ptr1[k - 4];
				ptr2[k - 5] -= d2 * ptr1[k - 5];
				ptr2[k - 6] -= d2 * ptr1[k - 6];
				ptr2[k - 7] -= d2 * ptr1[k - 7];
				ptr2[k - 8] -= d2 * ptr1[k - 8];
				ptr2[k - 9] -= d2 * ptr1[k - 9];
				ptr2[k - 10] -= d2 * ptr1[k - 10];
				ptr2[k - 11] -= d2 * ptr1[k - 11];
				ptr2[k - 12] -= d2 * ptr1[k - 12];
				ptr2[k - 13] -= d2 * ptr1[k - 13];
				ptr2[k - 14] -= d2 * ptr1[k - 14];
				ptr2[k - 15] -= d2 * ptr1[k - 15];
			}
			switch( k - i ) {
				NODEFAULT;
				case 15:
					ptr2[k - 14] -= d2 * ptr1[k - 14];
				case 14:
					ptr2[k - 13] -= d2 * ptr1[k - 13];
				case 13:
					ptr2[k - 12] -= d2 * ptr1[k - 12];
				case 12:
					ptr2[k - 11] -= d2 * ptr1[k - 11];
				case 11:
					ptr2[k - 10] -= d2 * ptr1[k - 10];
				case 10:
					ptr2[k - 9] -= d2 * ptr1[k - 9];
				case 9:
					ptr2[k - 8] -= d2 * ptr1[k - 8];
				case 8:
					ptr2[k - 7] -= d2 * ptr1[k - 7];
				case 7:
					ptr2[k - 6] -= d2 * ptr1[k - 6];
				case 6:
					ptr2[k - 5] -= d2 * ptr1[k - 5];
				case 5:
					ptr2[k - 4] -= d2 * ptr1[k - 4];
				case 4:
					ptr2[k - 3] -= d2 * ptr1[k - 3];
				case 3:
					ptr2[k - 2] -= d2 * ptr1[k - 2];
				case 2:
					ptr2[k - 1] -= d2 * ptr1[k - 1];
				case 1:
					ptr2[k - 0] -= d2 * ptr1[k - 0];
				case 0:
					break;
			}
		}
	}

	return true;
}

/*!
	\brief Performs LDLT factorization on a matrix using SIMD optimization

	This function computes the LDLT factorization of a symmetric matrix, where L is a lower triangular matrix with ones on the diagonal, D is a diagonal matrix, and the factorization is represented as
   A = L * D * L^T. The operation is optimized using SIMD instructions for performance. The matrix is modified in-place during the computation. The function also computes the inverse of the diagonal
   elements and stores them in the invDiag vector.

	\param mat The matrix to be factorized, modified in-place during computation
	\param invDiag Output vector containing the inverse of the diagonal elements of the D matrix
	\param n The size of the matrix to factorize
	\return true if the factorization was successful, false if a diagonal element was too small to be used as a divisor
*/
static bool LDLT_Factor_SIMD( idMatX& mat, idVecX& invDiag, const int n )
{
	float  s0, s1, s2, d;

	float* v		  = ( float* )_alloca16( ( ( n + 3 ) & ~3 ) * sizeof( float ) );
	float* diag		  = ( float* )_alloca16( ( ( n + 3 ) & ~3 ) * sizeof( float ) );
	float* invDiagPtr = invDiag.ToFloatPtr();

	int	   nc = mat.GetNumColumns();

	assert( ( nc & 3 ) == 0 );

	if( n <= 0 ) {
		return true;
	}

	float* mptr = mat[0];

	float  sum = mptr[0];

	if( fabs( sum ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		return false;
	}

	diag[0]		  = sum;
	invDiagPtr[0] = d = 1.0f / sum;

	if( n <= 1 ) {
		return true;
	}

	mptr = mat[0];
	for( int j = 1; j < n; j++ ) {
		mptr[j * nc + 0] = ( mptr[j * nc + 0] ) * d;
	}

	mptr = mat[1];

	v[0] = diag[0] * mptr[0];
	s0	 = v[0] * mptr[0];
	sum	 = mptr[1] - s0;

	if( fabs( sum ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		return false;
	}

	mat[1][1]	  = sum;
	diag[1]		  = sum;
	invDiagPtr[1] = d = 1.0f / sum;

	if( n <= 2 ) {
		return true;
	}

	mptr = mat[0];
	for( int j = 2; j < n; j++ ) {
		mptr[j * nc + 1] = ( mptr[j * nc + 1] - v[0] * mptr[j * nc + 0] ) * d;
	}

	mptr = mat[2];

	v[0] = diag[0] * mptr[0];
	s0	 = v[0] * mptr[0];
	v[1] = diag[1] * mptr[1];
	s1	 = v[1] * mptr[1];
	sum	 = mptr[2] - s0 - s1;

	if( fabs( sum ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		return false;
	}

	mat[2][2]	  = sum;
	diag[2]		  = sum;
	invDiagPtr[2] = d = 1.0f / sum;

	if( n <= 3 ) {
		return true;
	}

	mptr = mat[0];
	for( int j = 3; j < n; j++ ) {
		mptr[j * nc + 2] = ( mptr[j * nc + 2] - v[0] * mptr[j * nc + 0] - v[1] * mptr[j * nc + 1] ) * d;
	}

	mptr = mat[3];

	v[0] = diag[0] * mptr[0];
	s0	 = v[0] * mptr[0];
	v[1] = diag[1] * mptr[1];
	s1	 = v[1] * mptr[1];
	v[2] = diag[2] * mptr[2];
	s2	 = v[2] * mptr[2];
	sum	 = mptr[3] - s0 - s1 - s2;

	if( fabs( sum ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		return false;
	}

	mat[3][3]	  = sum;
	diag[3]		  = sum;
	invDiagPtr[3] = d = 1.0f / sum;

	if( n <= 4 ) {
		return true;
	}

	mptr = mat[0];
	for( int j = 4; j < n; j++ ) {
		mptr[j * nc + 3] = ( mptr[j * nc + 3] - v[0] * mptr[j * nc + 0] - v[1] * mptr[j * nc + 1] - v[2] * mptr[j * nc + 2] ) * d;
	}

#if defined( LCP_SIMD )

	__m128 vzero = _mm_setzero_ps();
	for( int i = 4; i < n; i += 4 ) {
		_mm_store_ps( diag + i, vzero );
	}

	for( int i = 4; i < n; i++ ) {
		mptr = mat[i];

		assert_16_byte_aligned( v );
		assert_16_byte_aligned( mptr );
		assert_16_byte_aligned( diag );

		__m128 m0 = _mm_load_ps( mptr + 0 );
		__m128 d0 = _mm_load_ps( diag + 0 );
		__m128 v0 = _mm_mul_ps( d0, m0 );
		__m128 t0 = _mm_load_ss( mptr + i );
		t0		  = _mm_sub_ps( t0, _mm_mul_ps( m0, v0 ) );
		_mm_store_ps( v + 0, v0 );

		int k = 4;
		for( ; k < i - 3; k += 4 ) {
			m0 = _mm_load_ps( mptr + k );
			d0 = _mm_load_ps( diag + k );
			v0 = _mm_mul_ps( d0, m0 );
			t0 = _mm_sub_ps( t0, _mm_mul_ps( m0, v0 ) );
			_mm_store_ps( v + k, v0 );
		}

		__m128 mask = ( __m128& )SIMD_SP_indexedEndMask[i & 3];

		m0 = _mm_and_ps( _mm_load_ps( mptr + k ), mask );
		d0 = _mm_load_ps( diag + k );
		v0 = _mm_mul_ps( d0, m0 );
		t0 = _mm_sub_ps( t0, _mm_mul_ps( m0, v0 ) );
		_mm_store_ps( v + k, v0 );

		t0 = _mm_add_ps( t0, _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
		t0 = _mm_add_ps( t0, _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );

		__m128 tiny = _mm_and_ps( _mm_cmpeq_ps( t0, SIMD_SP_zero ), SIMD_SP_tiny );
		t0			= _mm_or_ps( t0, tiny );

		_mm_store_ss( mptr + i, t0 );
		_mm_store_ss( diag + i, t0 );

		__m128 d = _mm_rcp32_ps( t0 );
		_mm_store_ss( invDiagPtr + i, d );

		if( i + 1 >= n ) {
			return true;
		}

		int j = i + 1;
		for( ; j < n - 3; j += 4 ) {
			float* ra = mat[j + 0];
			float* rb = mat[j + 1];
			float* rc = mat[j + 2];
			float* rd = mat[j + 3];

			assert_16_byte_aligned( v );
			assert_16_byte_aligned( ra );
			assert_16_byte_aligned( rb );
			assert_16_byte_aligned( rc );
			assert_16_byte_aligned( rd );

			__m128 va = _mm_load_ss( ra + i );
			__m128 vb = _mm_load_ss( rb + i );
			__m128 vc = _mm_load_ss( rc + i );
			__m128 vd = _mm_load_ss( rd + i );

			__m128 v0 = _mm_load_ps( v + 0 );

			va = _mm_sub_ps( va, _mm_mul_ps( _mm_load_ps( ra + 0 ), v0 ) );
			vb = _mm_sub_ps( vb, _mm_mul_ps( _mm_load_ps( rb + 0 ), v0 ) );
			vc = _mm_sub_ps( vc, _mm_mul_ps( _mm_load_ps( rc + 0 ), v0 ) );
			vd = _mm_sub_ps( vd, _mm_mul_ps( _mm_load_ps( rd + 0 ), v0 ) );

			int k = 4;
			for( ; k < i - 3; k += 4 ) {
				v0 = _mm_load_ps( v + k );

				va = _mm_sub_ps( va, _mm_mul_ps( _mm_load_ps( ra + k ), v0 ) );
				vb = _mm_sub_ps( vb, _mm_mul_ps( _mm_load_ps( rb + k ), v0 ) );
				vc = _mm_sub_ps( vc, _mm_mul_ps( _mm_load_ps( rc + k ), v0 ) );
				vd = _mm_sub_ps( vd, _mm_mul_ps( _mm_load_ps( rd + k ), v0 ) );
			}

			v0 = _mm_load_ps( v + k );

			va = _mm_sub_ps( va, _mm_mul_ps( _mm_and_ps( _mm_load_ps( ra + k ), mask ), v0 ) );
			vb = _mm_sub_ps( vb, _mm_mul_ps( _mm_and_ps( _mm_load_ps( rb + k ), mask ), v0 ) );
			vc = _mm_sub_ps( vc, _mm_mul_ps( _mm_and_ps( _mm_load_ps( rc + k ), mask ), v0 ) );
			vd = _mm_sub_ps( vd, _mm_mul_ps( _mm_and_ps( _mm_load_ps( rd + k ), mask ), v0 ) );

			__m128 ta = _mm_unpacklo_ps( va, vc ); // x0, z0, x1, z1
			__m128 tb = _mm_unpackhi_ps( va, vc ); // x2, z2, x3, z3
			__m128 tc = _mm_unpacklo_ps( vb, vd ); // y0, w0, y1, w1
			__m128 td = _mm_unpackhi_ps( vb, vd ); // y2, w2, y3, w3

			va = _mm_unpacklo_ps( ta, tc ); // x0, y0, z0, w0
			vb = _mm_unpackhi_ps( ta, tc ); // x1, y1, z1, w1
			vc = _mm_unpacklo_ps( tb, td ); // x2, y2, z2, w2
			vd = _mm_unpackhi_ps( tb, td ); // x3, y3, z3, w3

			va = _mm_add_ps( va, vb );
			vc = _mm_add_ps( vc, vd );
			va = _mm_add_ps( va, vc );
			va = _mm_mul_ps( va, d );

			_mm_store_ss( ra + i, _mm_splat_ps( va, 0 ) );
			_mm_store_ss( rb + i, _mm_splat_ps( va, 1 ) );
			_mm_store_ss( rc + i, _mm_splat_ps( va, 2 ) );
			_mm_store_ss( rd + i, _mm_splat_ps( va, 3 ) );
		}
		for( ; j < n; j++ ) {
			float* mptr = mat[j];

			assert_16_byte_aligned( v );
			assert_16_byte_aligned( mptr );

			__m128 va = _mm_load_ss( mptr + i );
			__m128 v0 = _mm_load_ps( v + 0 );

			va = _mm_sub_ps( va, _mm_mul_ps( _mm_load_ps( mptr + 0 ), v0 ) );

			int k = 4;
			for( ; k < i - 3; k += 4 ) {
				v0 = _mm_load_ps( v + k );
				va = _mm_sub_ps( va, _mm_mul_ps( _mm_load_ps( mptr + k ), v0 ) );
			}

			v0 = _mm_load_ps( v + k );
			va = _mm_sub_ps( va, _mm_mul_ps( _mm_and_ps( _mm_load_ps( mptr + k ), mask ), v0 ) );

			va = _mm_add_ps( va, _mm_shuffle_ps( va, va, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
			va = _mm_add_ps( va, _mm_shuffle_ps( va, va, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
			va = _mm_mul_ps( va, d );

			_mm_store_ss( mptr + i, va );
		}
	}
	return true;

#else

	for( int i = 4; i < n; i += 4 ) {
		diag[i + 0] = 0.0f;
		diag[i + 1] = 0.0f;
		diag[i + 2] = 0.0f;
		diag[i + 3] = 0.0f;
	}

	for( int i = 4; i < n; i++ ) {
		mptr = mat[i];

		assert_16_byte_aligned( v );
		assert_16_byte_aligned( mptr );
		assert_16_byte_aligned( diag );

		v[0] = diag[0] * mptr[0];
		v[1] = diag[1] * mptr[1];
		v[2] = diag[2] * mptr[2];
		v[3] = diag[3] * mptr[3];

		float t0 = -mptr[0] * v[0] + mptr[i];
		float t1 = -mptr[1] * v[1];
		float t2 = -mptr[2] * v[2];
		float t3 = -mptr[3] * v[3];

		int	  k = 4;
		for( ; k < i - 3; k += 4 ) {
			v[k + 0] = diag[k + 0] * mptr[k + 0];
			v[k + 1] = diag[k + 1] * mptr[k + 1];
			v[k + 2] = diag[k + 2] * mptr[k + 2];
			v[k + 3] = diag[k + 3] * mptr[k + 3];

			t0 -= mptr[k + 0] * v[k + 0];
			t1 -= mptr[k + 1] * v[k + 1];
			t2 -= mptr[k + 2] * v[k + 2];
			t3 -= mptr[k + 3] * v[k + 3];
		}

		float m0 = ( i - k > 0 ) ? mptr[k + 0] : 0.0f;
		float m1 = ( i - k > 1 ) ? mptr[k + 1] : 0.0f;
		float m2 = ( i - k > 2 ) ? mptr[k + 2] : 0.0f;
		float m3 = ( i - k > 3 ) ? mptr[k + 3] : 0.0f;

		v[k + 0] = diag[k + 0] * m0;
		v[k + 1] = diag[k + 1] * m1;
		v[k + 2] = diag[k + 2] * m2;
		v[k + 3] = diag[k + 3] * m3;

		t0 -= m0 * v[k + 0];
		t1 -= m1 * v[k + 1];
		t2 -= m2 * v[k + 2];
		t3 -= m3 * v[k + 3];

		sum = t0 + t1 + t2 + t3;

		if( fabs( sum ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
			return false;
		}

		mat[i][i]	  = sum;
		diag[i]		  = sum;
		invDiagPtr[i] = d = 1.0f / sum;

		if( i + 1 >= n ) {
			return true;
		}

		int j = i + 1;
		for( ; j < n - 3; j += 4 ) {
			float* ra = mat[j + 0];
			float* rb = mat[j + 1];
			float* rc = mat[j + 2];
			float* rd = mat[j + 3];

			assert_16_byte_aligned( v );
			assert_16_byte_aligned( ra );
			assert_16_byte_aligned( rb );
			assert_16_byte_aligned( rc );
			assert_16_byte_aligned( rd );

			float a0 = -ra[0] * v[0] + ra[i];
			float a1 = -ra[1] * v[1];
			float a2 = -ra[2] * v[2];
			float a3 = -ra[3] * v[3];

			float b0 = -rb[0] * v[0] + rb[i];
			float b1 = -rb[1] * v[1];
			float b2 = -rb[2] * v[2];
			float b3 = -rb[3] * v[3];

			float c0 = -rc[0] * v[0] + rc[i];
			float c1 = -rc[1] * v[1];
			float c2 = -rc[2] * v[2];
			float c3 = -rc[3] * v[3];

			float d0 = -rd[0] * v[0] + rd[i];
			float d1 = -rd[1] * v[1];
			float d2 = -rd[2] * v[2];
			float d3 = -rd[3] * v[3];

			int	  k = 4;
			for( ; k < i - 3; k += 4 ) {
				a0 -= ra[k + 0] * v[k + 0];
				a1 -= ra[k + 1] * v[k + 1];
				a2 -= ra[k + 2] * v[k + 2];
				a3 -= ra[k + 3] * v[k + 3];

				b0 -= rb[k + 0] * v[k + 0];
				b1 -= rb[k + 1] * v[k + 1];
				b2 -= rb[k + 2] * v[k + 2];
				b3 -= rb[k + 3] * v[k + 3];

				c0 -= rc[k + 0] * v[k + 0];
				c1 -= rc[k + 1] * v[k + 1];
				c2 -= rc[k + 2] * v[k + 2];
				c3 -= rc[k + 3] * v[k + 3];

				d0 -= rd[k + 0] * v[k + 0];
				d1 -= rd[k + 1] * v[k + 1];
				d2 -= rd[k + 2] * v[k + 2];
				d3 -= rd[k + 3] * v[k + 3];
			}

			float ra0 = ( i - k > 0 ) ? ra[k + 0] : 0.0f;
			float ra1 = ( i - k > 1 ) ? ra[k + 1] : 0.0f;
			float ra2 = ( i - k > 2 ) ? ra[k + 2] : 0.0f;
			float ra3 = ( i - k > 3 ) ? ra[k + 3] : 0.0f;

			float rb0 = ( i - k > 0 ) ? rb[k + 0] : 0.0f;
			float rb1 = ( i - k > 1 ) ? rb[k + 1] : 0.0f;
			float rb2 = ( i - k > 2 ) ? rb[k + 2] : 0.0f;
			float rb3 = ( i - k > 3 ) ? rb[k + 3] : 0.0f;

			float rc0 = ( i - k > 0 ) ? rc[k + 0] : 0.0f;
			float rc1 = ( i - k > 1 ) ? rc[k + 1] : 0.0f;
			float rc2 = ( i - k > 2 ) ? rc[k + 2] : 0.0f;
			float rc3 = ( i - k > 3 ) ? rc[k + 3] : 0.0f;

			float rd0 = ( i - k > 0 ) ? rd[k + 0] : 0.0f;
			float rd1 = ( i - k > 1 ) ? rd[k + 1] : 0.0f;
			float rd2 = ( i - k > 2 ) ? rd[k + 2] : 0.0f;
			float rd3 = ( i - k > 3 ) ? rd[k + 3] : 0.0f;

			a0 -= ra0 * v[k + 0];
			a1 -= ra1 * v[k + 1];
			a2 -= ra2 * v[k + 2];
			a3 -= ra3 * v[k + 3];

			b0 -= rb0 * v[k + 0];
			b1 -= rb1 * v[k + 1];
			b2 -= rb2 * v[k + 2];
			b3 -= rb3 * v[k + 3];

			c0 -= rc0 * v[k + 0];
			c1 -= rc1 * v[k + 1];
			c2 -= rc2 * v[k + 2];
			c3 -= rc3 * v[k + 3];

			d0 -= rd0 * v[k + 0];
			d1 -= rd1 * v[k + 1];
			d2 -= rd2 * v[k + 2];
			d3 -= rd3 * v[k + 3];

			ra[i] = ( a0 + a1 + a2 + a3 ) * d;
			rb[i] = ( b0 + b1 + b2 + b3 ) * d;
			rc[i] = ( c0 + c1 + c2 + c3 ) * d;
			rd[i] = ( d0 + d1 + d2 + d3 ) * d;
		}
		for( ; j < n; j++ ) {
			mptr = mat[j];

			assert_16_byte_aligned( v );
			assert_16_byte_aligned( mptr );

			float a0 = -mptr[0] * v[0] + mptr[i];
			float a1 = -mptr[1] * v[1];
			float a2 = -mptr[2] * v[2];
			float a3 = -mptr[3] * v[3];

			int	  k = 4;
			for( ; k < i - 3; k += 4 ) {
				a0 -= mptr[k + 0] * v[k + 0];
				a1 -= mptr[k + 1] * v[k + 1];
				a2 -= mptr[k + 2] * v[k + 2];
				a3 -= mptr[k + 3] * v[k + 3];
			}

			float m0 = ( i - k > 0 ) ? mptr[k + 0] : 0.0f;
			float m1 = ( i - k > 1 ) ? mptr[k + 1] : 0.0f;
			float m2 = ( i - k > 2 ) ? mptr[k + 2] : 0.0f;
			float m3 = ( i - k > 3 ) ? mptr[k + 3] : 0.0f;

			a0 -= m0 * v[k + 0];
			a1 -= m1 * v[k + 1];
			a2 -= m2 * v[k + 2];
			a3 -= m3 * v[k + 3];

			mptr[i] = ( a0 + a1 + a2 + a3 ) * d;
		}
	}
	return true;

#endif
}

/*!
	\brief Computes the maximum step size for LCP solving using SIMD instructions

	This function calculates the maximum step size for solving Linear Complementarity Problems using SIMD instructions. It evaluates constraints based on force and acceleration deltas, considering
   both bounded and unbounded variables. The function determines the limiting constraint and side for the step calculation, handling various cases including clamped variables and bounded constraints.

	\param f Current force values
	\param a Current acceleration values
	\param delta_f Force deltas
	\param delta_a Acceleration deltas
	\param lo Lower bounds
	\param hi Upper bounds
	\param side Constraint sides
	\param numUnbounded Number of unbounded variables
	\param numClamped Number of clamped variables
	\param d Dimension count
	\param dir Direction of movement
	\param maxStep Output maximum step size
	\param limit Output limiting variable index
	\param limitSide Output limiting side
*/
static void GetMaxStep_SIMD( const float* f,
	const float*						  a,
	const float*						  delta_f,
	const float*						  delta_a,
	const float*						  lo,
	const float*						  hi,
	const int*							  side,
	int									  numUnbounded,
	int									  numClamped,
	int									  d,
	float								  dir,
	float&								  maxStep,
	int&								  limit,
	int&								  limitSide )
{
#if defined( LCP_SIMD )
	__m128	vMaxStep;
	__m128i vLimit;
	__m128i vLimitSide;

	// default to a full step for the current variable
	{
		__m128 vNegAccel   = _mm_xor_ps( _mm_load1_ps( a + d ), ( __m128& )SIMD_SP_signBit );
		__m128 vDeltaAccel = _mm_load1_ps( delta_a + d );
		__m128 vM0		   = _mm_cmpgt_ps( _mm_and_ps( vDeltaAccel, ( __m128& )SIMD_SP_absMask ), SIMD_SP_LCP_DELTA_ACCEL_EPSILON );
		__m128 vStep	   = _mm_div32_ps( vNegAccel, _mm_sel_ps( SIMD_SP_one, vDeltaAccel, vM0 ) );
		vMaxStep		   = _mm_sel_ps( SIMD_SP_zero, vStep, vM0 );
		vLimit			   = _mm_shuffle_epi32( _mm_cvtsi32_si128( d ), 0 );
		vLimitSide		   = ( __m128i& )SIMD_DW_zero;
	}

	// test the current variable
	{
		__m128	vDeltaForce = _mm_load1_ps( &dir );
		__m128	vSign		= _mm_cmplt_ps( vDeltaForce, SIMD_SP_zero );
		__m128	vForceLimit = _mm_sel_ps( _mm_load1_ps( hi + d ), _mm_load1_ps( lo + d ), vSign );
		__m128	vStep		= _mm_div32_ps( _mm_sub_ps( vForceLimit, _mm_load1_ps( f + d ) ), vDeltaForce );
		__m128i vSetSide	= _mm_or_si128( __m128c( vSign ), ( __m128i& )SIMD_DW_one );
		__m128	vM0			= _mm_cmpgt_ps( _mm_and_ps( vDeltaForce, ( __m128& )SIMD_SP_absMask ), SIMD_SP_LCP_DELTA_FORCE_EPSILON );
		__m128	vM1			= _mm_cmpneq_ps( _mm_and_ps( vForceLimit, ( __m128& )SIMD_SP_absMask ), SIMD_SP_infinity );
		__m128	vM2			= _mm_cmplt_ps( vStep, vMaxStep );
		__m128	vM3			= _mm_and_ps( _mm_and_ps( vM0, vM1 ), vM2 );
		vMaxStep			= _mm_sel_ps( vMaxStep, vStep, vM3 );
		vLimitSide			= _mm_sel_si128( vLimitSide, vSetSide, __m128c( vM3 ) );
	}

	// test the clamped bounded variables
	{
		__m128	mask  = ( __m128& )SIMD_SP_indexedStartMask[numUnbounded & 3];
		__m128i index = _mm_add_epi32( _mm_and_si128( _mm_shuffle_epi32( _mm_cvtsi32_si128( numUnbounded ), 0 ), ( __m128i& )SIMD_DW_not3 ), ( __m128i& )SIMD_DW_index );
		int		i	  = numUnbounded & ~3;
		for( ; i < numClamped - 3; i += 4 ) {
			__m128	vDeltaForce = _mm_and_ps( _mm_load_ps( delta_f + i ), mask );
			__m128	vSign		= _mm_cmplt_ps( vDeltaForce, SIMD_SP_zero );
			__m128	vForceLimit = _mm_sel_ps( _mm_load_ps( hi + i ), _mm_load_ps( lo + i ), vSign );
			__m128	vM0			= _mm_cmpgt_ps( _mm_and_ps( vDeltaForce, ( __m128& )SIMD_SP_absMask ), SIMD_SP_LCP_DELTA_FORCE_EPSILON );
			__m128	vStep		= _mm_div32_ps( _mm_sub_ps( vForceLimit, _mm_load_ps( f + i ) ), _mm_sel_ps( SIMD_SP_one, vDeltaForce, vM0 ) );
			__m128i vSetSide	= _mm_or_si128( __m128c( vSign ), ( __m128i& )SIMD_DW_one );
			__m128	vM1			= _mm_cmpneq_ps( _mm_and_ps( vForceLimit, ( __m128& )SIMD_SP_absMask ), SIMD_SP_infinity );
			__m128	vM2			= _mm_cmplt_ps( vStep, vMaxStep );
			__m128	vM3			= _mm_and_ps( _mm_and_ps( vM0, vM1 ), vM2 );
			vMaxStep			= _mm_sel_ps( vMaxStep, vStep, vM3 );
			vLimit				= _mm_sel_si128( vLimit, index, vM3 );
			vLimitSide			= _mm_sel_si128( vLimitSide, vSetSide, __m128c( vM3 ) );
			mask				= ( __m128& )SIMD_SP_indexedStartMask[0];
			index				= _mm_add_epi32( index, ( __m128i& )SIMD_DW_four );
		}
		__m128	vDeltaForce = _mm_and_ps( _mm_load_ps( delta_f + i ), _mm_and_ps( mask, ( __m128& )SIMD_SP_indexedEndMask[numClamped & 3] ) );
		__m128	vSign		= _mm_cmplt_ps( vDeltaForce, SIMD_SP_zero );
		__m128	vForceLimit = _mm_sel_ps( _mm_load_ps( hi + i ), _mm_load_ps( lo + i ), vSign );
		__m128	vM0			= _mm_cmpgt_ps( _mm_and_ps( vDeltaForce, ( __m128& )SIMD_SP_absMask ), SIMD_SP_LCP_DELTA_FORCE_EPSILON );
		__m128	vStep		= _mm_div32_ps( _mm_sub_ps( vForceLimit, _mm_load_ps( f + i ) ), _mm_sel_ps( SIMD_SP_one, vDeltaForce, vM0 ) );
		__m128i vSetSide	= _mm_or_si128( __m128c( vSign ), ( __m128i& )SIMD_DW_one );
		__m128	vM1			= _mm_cmpneq_ps( _mm_and_ps( vForceLimit, ( __m128& )SIMD_SP_absMask ), SIMD_SP_infinity );
		__m128	vM2			= _mm_cmplt_ps( vStep, vMaxStep );
		__m128	vM3			= _mm_and_ps( _mm_and_ps( vM0, vM1 ), vM2 );
		vMaxStep			= _mm_sel_ps( vMaxStep, vStep, vM3 );
		vLimit				= _mm_sel_si128( vLimit, index, vM3 );
		vLimitSide			= _mm_sel_si128( vLimitSide, vSetSide, __m128c( vM3 ) );
	}

	// test the not clamped bounded variables
	{
		__m128	mask  = ( __m128& )SIMD_SP_indexedStartMask[numClamped & 3];
		__m128i index = _mm_add_epi32( _mm_and_si128( _mm_shuffle_epi32( _mm_cvtsi32_si128( numClamped ), 0 ), ( __m128i& )SIMD_DW_not3 ), ( __m128i& )SIMD_DW_index );
		int		i	  = numClamped & ~3;
		for( ; i < d - 3; i += 4 ) {
			__m128 vNegAccel   = _mm_xor_ps( _mm_load_ps( a + i ), ( __m128& )SIMD_SP_signBit );
			__m128 vDeltaAccel = _mm_and_ps( _mm_load_ps( delta_a + i ), mask );
			__m128 vSide	   = _mm_cvtepi32_ps( _mm_load_si128( ( __m128i* )( side + i ) ) );
			__m128 vM0		   = _mm_cmpgt_ps( _mm_mul_ps( vSide, vDeltaAccel ), SIMD_SP_LCP_DELTA_ACCEL_EPSILON );
			__m128 vStep	   = _mm_div32_ps( vNegAccel, _mm_sel_ps( SIMD_SP_one, vDeltaAccel, vM0 ) );
			__m128 vM1		   = _mm_or_ps( _mm_cmplt_ps( _mm_load_ps( lo + i ), SIMD_SP_neg_LCP_BOUND_EPSILON ), _mm_cmpgt_ps( _mm_load_ps( hi + i ), SIMD_SP_LCP_BOUND_EPSILON ) );
			__m128 vM2		   = _mm_cmplt_ps( vStep, vMaxStep );
			__m128 vM3		   = _mm_and_ps( _mm_and_ps( vM0, vM1 ), vM2 );
			vMaxStep		   = _mm_sel_ps( vMaxStep, vStep, vM3 );
			vLimit			   = _mm_sel_si128( vLimit, index, vM3 );
			vLimitSide		   = _mm_sel_si128( vLimitSide, ( __m128i& )SIMD_DW_zero, __m128c( vM3 ) );
			mask			   = ( __m128& )SIMD_SP_indexedStartMask[0];
			index			   = _mm_add_epi32( index, ( __m128i& )SIMD_DW_four );
		}
		__m128 vNegAccel   = _mm_xor_ps( _mm_load_ps( a + i ), ( __m128& )SIMD_SP_signBit );
		__m128 vDeltaAccel = _mm_and_ps( _mm_load_ps( delta_a + i ), _mm_and_ps( mask, ( __m128& )SIMD_SP_indexedEndMask[d & 3] ) );
		__m128 vSide	   = _mm_cvtepi32_ps( _mm_load_si128( ( __m128i* )( side + i ) ) );
		__m128 vM0		   = _mm_cmpgt_ps( _mm_mul_ps( vSide, vDeltaAccel ), SIMD_SP_LCP_DELTA_ACCEL_EPSILON );
		__m128 vStep	   = _mm_div32_ps( vNegAccel, _mm_sel_ps( SIMD_SP_one, vDeltaAccel, vM0 ) );
		__m128 vM1		   = _mm_or_ps( _mm_cmplt_ps( _mm_load_ps( lo + i ), SIMD_SP_neg_LCP_BOUND_EPSILON ), _mm_cmpgt_ps( _mm_load_ps( hi + i ), SIMD_SP_LCP_BOUND_EPSILON ) );
		__m128 vM2		   = _mm_cmplt_ps( vStep, vMaxStep );
		__m128 vM3		   = _mm_and_ps( _mm_and_ps( vM0, vM1 ), vM2 );
		vMaxStep		   = _mm_sel_ps( vMaxStep, vStep, vM3 );
		vLimit			   = _mm_sel_si128( vLimit, index, vM3 );
		vLimitSide		   = _mm_sel_si128( vLimitSide, ( __m128i& )SIMD_DW_zero, __m128c( vM3 ) );
	}

	{
		__m128	tMaxStep   = _mm_shuffle_ps( vMaxStep, vMaxStep, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		__m128i tLimit	   = _mm_shuffle_epi32( vLimit, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		__m128i tLimitSide = _mm_shuffle_epi32( vLimitSide, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		__m128c mask	   = _mm_cmplt_ps( tMaxStep, vMaxStep );
		vMaxStep		   = _mm_min_ps( vMaxStep, tMaxStep );
		vLimit			   = _mm_sel_si128( vLimit, tLimit, mask );
		vLimitSide		   = _mm_sel_si128( vLimitSide, tLimitSide, mask );
	}

	{
		__m128	tMaxStep   = _mm_shuffle_ps( vMaxStep, vMaxStep, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		__m128i tLimit	   = _mm_shuffle_epi32( vLimit, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		__m128i tLimitSide = _mm_shuffle_epi32( vLimitSide, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		__m128c mask	   = _mm_cmplt_ps( tMaxStep, vMaxStep );
		vMaxStep		   = _mm_min_ps( vMaxStep, tMaxStep );
		vLimit			   = _mm_sel_si128( vLimit, tLimit, mask );
		vLimitSide		   = _mm_sel_si128( vLimitSide, tLimitSide, mask );
	}

	_mm_store_ss( &maxStep, vMaxStep );
	limit	  = _mm_cvtsi128_si32( vLimit );
	limitSide = _mm_cvtsi128_si32( vLimitSide );

#else

	// default to a full step for the current variable
	{
		float negAccel	 = -a[d];
		float deltaAccel = delta_a[d];
		int	  m0		 = ( fabs( deltaAccel ) > LCP_DELTA_ACCEL_EPSILON );
		float step		 = negAccel / ( m0 ? deltaAccel : 1.0f );
		maxStep			 = m0 ? step : 0.0f;
		limit			 = d;
		limitSide		 = 0;
	}

	// test the current variable
	{
		float deltaForce = dir;
		float forceLimit = ( deltaForce < 0.0f ) ? lo[d] : hi[d];
		float step		 = ( forceLimit - f[d] ) / deltaForce;
		int	  setSide	 = ( deltaForce < 0.0f ) ? -1 : 1;
		int	  m0		 = ( fabs( deltaForce ) > LCP_DELTA_FORCE_EPSILON );
		int	  m1		 = ( fabs( forceLimit ) != idMath::INFINITUM );
		int	  m2		 = ( step < maxStep );
		int	  m3		 = ( m0 & m1 & m2 );
		maxStep			 = m3 ? step : maxStep;
		limit			 = m3 ? d : limit;
		limitSide		 = m3 ? setSide : limitSide;
	}

	// test the clamped bounded variables
	for( int i = numUnbounded; i < numClamped; i++ ) {
		float deltaForce = delta_f[i];
		float forceLimit = ( deltaForce < 0.0f ) ? lo[i] : hi[i];
		int	  m0		 = ( fabs( deltaForce ) > LCP_DELTA_FORCE_EPSILON );
		float step		 = ( forceLimit - f[i] ) / ( m0 ? deltaForce : 1.0f );
		int	  setSide	 = ( deltaForce < 0.0f ) ? -1 : 1;
		int	  m1		 = ( fabs( forceLimit ) != idMath::INFINITUM );
		int	  m2		 = ( step < maxStep );
		int	  m3		 = ( m0 & m1 & m2 );
		maxStep			 = m3 ? step : maxStep;
		limit			 = m3 ? i : limit;
		limitSide		 = m3 ? setSide : limitSide;
	}

	// test the not clamped bounded variables
	for( int i = numClamped; i < d; i++ ) {
		float negAccel	 = -a[i];
		float deltaAccel = delta_a[i];
		int	  m0		 = ( side[i] * deltaAccel > LCP_DELTA_ACCEL_EPSILON );
		float step		 = negAccel / ( m0 ? deltaAccel : 1.0f );
		int	  m1		 = ( lo[i] < -LCP_BOUND_EPSILON || hi[i] > LCP_BOUND_EPSILON );
		int	  m2		 = ( step < maxStep );
		int	  m3		 = ( m0 & m1 & m2 );
		maxStep			 = m3 ? step : maxStep;
		limit			 = m3 ? i : limit;
		limitSide		 = m3 ? 0 : limitSide;
	}

#endif
}

/*
================================================================================================

	SIMD test code

================================================================================================
*/

// #define ENABLE_TEST_CODE

#ifdef ENABLE_TEST_CODE

	#define TEST_TRIANGULAR_SOLVE_SIMD_EPSILON 0.1f
	#define TEST_TRIANGULAR_SOLVE_SIZE		   50
	#define TEST_FACTOR_SIMD_EPSILON		   0.1f
	#define TEST_FACTOR_SOLVE_SIZE			   50
	#define NUM_TESTS						   50

/*
========================
PrintClocks
========================
*/
static void PrintClocks( const char* string, int dataCount, int64 clocks, int64 otherClocks = 0 )
{
	idLib::Printf( string );
	for( int i = idStr::LengthWithoutColors( string ); i < 48; i++ ) {
		idLib::Printf( " " );
	}
	if( clocks && otherClocks ) {
		int p = 0;
		if( clocks <= otherClocks ) {
			p = idMath::Ftoi( ( float )( otherClocks - clocks ) * 100.0f / ( float )otherClocks );
		} else {
			p = -idMath::Ftoi( ( float )( clocks - otherClocks ) * 100.0f / ( float )clocks );
		}
		idLib::Printf( "c = %4d, clcks = %5lld, %d%%\n", dataCount, clocks, p );
	} else {
		idLib::Printf( "c = %4d, clcks = %5lld\n", dataCount, clocks );
	}
}

/*
========================
DotProduct_Test
========================
*/
static void DotProduct_Test()
{
	ALIGN16( float fsrc0[TEST_TRIANGULAR_SOLVE_SIZE + 1]; )
	ALIGN16( float fsrc1[TEST_TRIANGULAR_SOLVE_SIZE + 1]; )

	idRandom srnd( 13 );

	for( int i = 0; i < TEST_TRIANGULAR_SOLVE_SIZE; i++ ) {
		fsrc0[i] = srnd.CRandomFloat() * 10.0f;
		fsrc1[i] = srnd.CRandomFloat() * 10.0f;
	}

	idTimer timer;

	for( int i = 0; i < TEST_TRIANGULAR_SOLVE_SIZE; i++ ) {
		float dot1			= DotProduct_Generic( fsrc0, fsrc1, i );
		int64 clocksGeneric = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			fsrc1[TEST_TRIANGULAR_SOLVE_SIZE] = j;
			timer.Clear();
			timer.Start();
			dot1 = DotProduct_Generic( fsrc0, fsrc1, i );
			timer.Stop();
			clocksGeneric = Min( clocksGeneric, timer.ClockTicks() );
		}

		PrintClocks( va( "DotProduct_Generic %d", i ), 1, clocksGeneric );

		float dot2		 = DotProduct_SIMD( fsrc0, fsrc1, i );
		int64 clocksSIMD = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			fsrc1[TEST_TRIANGULAR_SOLVE_SIZE] = j;
			timer.Clear();
			timer.Start();
			dot2 = DotProduct_SIMD( fsrc0, fsrc1, i );
			timer.Stop();
			clocksSIMD = Min( clocksSIMD, timer.ClockTicks() );
		}

		const char* result = idMath::Fabs( dot1 - dot2 ) < 1e-4f ? "ok" : S_COLOR_RED "X";
		PrintClocks( va( "DotProduct_SIMD    %d %s", i, result ), 1, clocksSIMD, clocksGeneric );
	}
}

/*
========================
LowerTriangularSolve_Test
========================
*/
static void LowerTriangularSolve_Test()
{
	idMatX L;
	idVecX x, b, tst;

	int	   paddedSize = ( TEST_TRIANGULAR_SOLVE_SIZE + 3 ) & ~3;

	L.Random( paddedSize, paddedSize, 0, -1.0f, 1.0f );
	x.SetSize( paddedSize );
	b.Random( paddedSize, 0, -1.0f, 1.0f );

	idTimer	  timer;

	const int skip = 0;

	for( int i = 1; i < TEST_TRIANGULAR_SOLVE_SIZE; i++ ) {
		x.Zero( i );

		LowerTriangularSolve_Generic( L, x.ToFloatPtr(), b.ToFloatPtr(), i, skip );
		int64 clocksGeneric = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			timer.Clear();
			timer.Start();
			LowerTriangularSolve_Generic( L, x.ToFloatPtr(), b.ToFloatPtr(), i, skip );
			timer.Stop();
			clocksGeneric = Min( clocksGeneric, timer.ClockTicks() );
		}

		tst = x;
		x.Zero();

		PrintClocks( va( "LowerTriangularSolve_Generic %dx%d", i, i ), 1, clocksGeneric );

		LowerTriangularSolve_SIMD( L, x.ToFloatPtr(), b.ToFloatPtr(), i, skip );
		int64 clocksSIMD = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			timer.Clear();
			timer.Start();
			LowerTriangularSolve_SIMD( L, x.ToFloatPtr(), b.ToFloatPtr(), i, skip );
			timer.Stop();
			clocksSIMD = Min( clocksSIMD, timer.ClockTicks() );
		}

		const char* result = x.Compare( tst, TEST_TRIANGULAR_SOLVE_SIMD_EPSILON ) ? "ok" : S_COLOR_RED "X";
		PrintClocks( va( "LowerTriangularSolve_SIMD    %dx%d %s", i, i, result ), 1, clocksSIMD, clocksGeneric );
	}
}

/*
========================
LowerTriangularSolveTranspose_Test
========================
*/
static void LowerTriangularSolveTranspose_Test()
{
	idMatX L;
	idVecX x, b, tst;

	int	   paddedSize = ( TEST_TRIANGULAR_SOLVE_SIZE + 3 ) & ~3;

	L.Random( paddedSize, paddedSize, 0, -1.0f, 1.0f );
	x.SetSize( paddedSize );
	b.Random( paddedSize, 0, -1.0f, 1.0f );

	idTimer timer;

	for( int i = 1; i < TEST_TRIANGULAR_SOLVE_SIZE; i++ ) {
		x.Zero( i );

		LowerTriangularSolveTranspose_Generic( L, x.ToFloatPtr(), b.ToFloatPtr(), i );
		int64 clocksGeneric = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			timer.Clear();
			timer.Start();
			LowerTriangularSolveTranspose_Generic( L, x.ToFloatPtr(), b.ToFloatPtr(), i );
			timer.Stop();
			clocksGeneric = Min( clocksGeneric, timer.ClockTicks() );
		}

		tst = x;
		x.Zero();

		PrintClocks( va( "LowerTriangularSolveTranspose_Generic %dx%d", i, i ), 1, clocksGeneric );

		LowerTriangularSolveTranspose_SIMD( L, x.ToFloatPtr(), b.ToFloatPtr(), i );
		int64 clocksSIMD = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			timer.Clear();
			timer.Start();
			LowerTriangularSolveTranspose_SIMD( L, x.ToFloatPtr(), b.ToFloatPtr(), i );
			timer.Stop();
			clocksSIMD = Min( clocksSIMD, timer.ClockTicks() );
		}

		const char* result = x.Compare( tst, TEST_TRIANGULAR_SOLVE_SIMD_EPSILON ) ? "ok" : S_COLOR_RED "X";
		PrintClocks( va( "LowerTriangularSolveTranspose_SIMD    %dx%d %s", i, i, result ), 1, clocksSIMD, clocksGeneric );
	}
}

/*
========================
LDLT_Factor_Test
========================
*/
static void LDLT_Factor_Test()
{
	idMatX src, original, mat1, mat2;
	idVecX invDiag1, invDiag2;

	int	   paddedSize = ( TEST_FACTOR_SOLVE_SIZE + 3 ) & ~3;

	original.SetSize( paddedSize, paddedSize );
	src.Random( paddedSize, paddedSize, 0, -1.0f, 1.0f );
	src.TransposeMultiply( original, src );

	idTimer timer;

	for( int i = 1; i < TEST_FACTOR_SOLVE_SIZE; i++ ) {
		int64 clocksGeneric = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			mat1 = original;
			invDiag1.Zero( TEST_FACTOR_SOLVE_SIZE );
			timer.Clear();
			timer.Start();
			LDLT_Factor_Generic( mat1, invDiag1, i );
			timer.Stop();
			clocksGeneric = Min( clocksGeneric, timer.ClockTicks() );
		}

		PrintClocks( va( "LDLT_Factor_Generic %dx%d", i, i ), 1, clocksGeneric );

		int64 clocksSIMD = 0xFFFFFFFFFFFF;
		for( int j = 0; j < NUM_TESTS; j++ ) {
			mat2 = original;
			invDiag2.Zero( TEST_FACTOR_SOLVE_SIZE );
			timer.Clear();
			timer.Start();
			LDLT_Factor_SIMD( mat2, invDiag2, i );
			timer.Stop();
			clocksSIMD = Min( clocksSIMD, timer.ClockTicks() );
		}

		const char* result = mat1.Compare( mat2, TEST_FACTOR_SIMD_EPSILON ) && invDiag1.Compare( invDiag2, TEST_FACTOR_SIMD_EPSILON ) ? "ok" : S_COLOR_RED "X";
		PrintClocks( va( "LDLT_Factor_SIMD    %dx%d %s", i, i, result ), 1, clocksSIMD, clocksGeneric );
	}
}
#endif

#define Multiply					  Multiply_SIMD
#define MultiplyAdd					  MultiplyAdd_SIMD
#define BigDotProduct				  DotProduct_SIMD
#define LowerTriangularSolve		  LowerTriangularSolve_SIMD
#define LowerTriangularSolveTranspose LowerTriangularSolveTranspose_SIMD
#define UpperTriangularSolve		  UpperTriangularSolve_SIMD
#define LU_Factor					  LU_Factor_SIMD
#define LDLT_Factor					  LDLT_Factor_SIMD
#define GetMaxStep					  GetMaxStep_SIMD

/*!
	\class idLCP_Square
	\brief A solver for linear complementarity problems using square matrix factorization techniques.

	This class implements a specialized solver for linear complementarity problems (LCP) that operates on square matrices. It provides functionality for solving LCP systems with both bounded and
   unbounded variables, including handling clamped variables through LU factorization and triangular solves. The class manages variable permutations to separate unbounded from bounded variables and
   supports dynamic addition and removal of clamped variables during the solving process. It also includes methods for calculating force and acceleration deltas, which are essential for physics
   simulations and constraint solving in game engines. The solver is designed to work with the broader idLCP framework while providing specific optimizations for square matrix operations.

*/
class idLCP_Square : public idLCP
{
public:
	/*!
		\brief Solves a linear complementarity problem with bounds using a square matrix.

		This function solves a linear complementarity problem (LCP) where the system is defined by a square matrix and vectors representing bounds. It handles both unbounded and bounded variables,
	   using a combination of factorization and iterative methods to find a solution that satisfies the complementarity conditions. The solver permutes variables to separate unbounded from bounded
	   ones and processes them accordingly, with special handling for boundary conditions.

		\param o_m The square matrix defining the LCP system
		\param o_x On input, the initial guess for the solution; on output, the computed solution
		\param o_b The right-hand side vector of the system
		\param o_lo Lower bounds for each variable
		\param o_hi Upper bounds for each variable
		\param o_boxIndex Optional array mapping variables to box constraints, or NULL if not used
		\return True if the LCP was solved successfully, false if factorization failed or an invalid step was encountered
	*/
	virtual bool Solve( const idMatX& o_m, idVecX& o_x, const idVecX& o_b, const idVecX& o_lo, const idVecX& o_hi, const int* o_boxIndex );

private:
	idMatX	m;				  // original matrix
	idVecX	b;				  // right hand side
	idVecX	lo, hi;			  // low and high bounds
	idVecX	f, a;			  // force and acceleration
	idVecX	delta_f, delta_a; // delta force and delta acceleration
	idMatX	clamped;		  // LU factored sub matrix for clamped variables
	idVecX	diagonal;		  // reciprocal of diagonal of U of the LU factored sub matrix for clamped variables
	int		numUnbounded;	  // number of unbounded variables
	int		numClamped;		  // number of clamped variables
	float** rowPtrs;		  // pointers to the rows of m
	int*	boxIndex;		  // box index
	int*	side;			  // tells if a variable is at the low boundary = -1, high boundary = 1 or inbetween = 0
	int*	permuted;		  // index to keep track of the permutation
	bool	padded;			  // set to true if the rows of the initial matrix are 16 byte padded

	/*!
		\brief Factors a clamped linear complementarity problem matrix and solves the linear complementarity problem with clamped variables using lower and upper triangular solves.

		This function performs the factorization of a clamped linear complementarity problem matrix. It copies the clamped rows into a temporary matrix and then applies LU factorization to solve the
	   linear complementarity problem with clamped variables. The function is designed to handle the specific case where some variables are clamped, meaning they are constrained to lie within certain
	   bounds.

		\return true if the LU factorization succeeds, false otherwise
	*/
	bool	FactorClamped();

	/*!
		\brief Solves the linear complementarity problem with clamped variables using lower and upper triangular solves.

		This function solves the linear complementarity problem by performing two triangular solves. First, it solves the lower triangular system using the clamped matrix and the input vector b. Then,
	   it solves the upper triangular system using the clamped matrix, diagonal vector, and the intermediate result from the first solve to compute the final solution vector x.

		\param x the solution vector to be computed
		\param b the input vector for the linear system
	*/
	void	SolveClamped( idVecX& x, const float* b );

	/*!
		\brief Swaps the elements at indices i and j in the LCP square data structure.

		This function performs a swap operation between two elements at specified indices within the LCP square data structure. It handles multiple internal arrays and matrices including row pointers,
	   matrix m, vectors b, lo, hi, a, and f, as well as auxiliary arrays boxIndex, side, and permuted. The function ensures that swapping is only performed when the indices are different to avoid
	   unnecessary operations. Special handling is included for the boxIndex array which may be null.

		\param i first index to swap
		\param j second index to swap
	*/
	void	Swap( int i, int j );

	/*!
		\brief Adds a clamped variable to the LCP Square matrix by updating the factorization

		This function updates the LCP Square matrix factorization by adding a new clamped variable. It handles the mathematical operations required to maintain the factorization when a new variable is
	   clamped. The function swaps the clamped variable index with the current number of clamped variables, then updates the L matrix by computing new row values and the U matrix by computing new
	   column values. Finally, it updates the diagonal element and increments the count of clamped variables.

		\param r The index of the clamped variable to add
		\throws assertion failure if r is less than numClamped
	*/
	void	AddClamped( int r );

	/*!
		\brief Removes a clamped row and column from the LCP square factorization and updates the factorization accordingly.

		This function removes the specified row and column from the clamped system and adjusts the factorization to maintain numerical stability. It handles the swapping of the removed row/column with
	   the last row/column and performs necessary updates to the factorization matrix. The function includes error checking and handles exceptional cases where floating point values might cause
	   failures during the factorization update process.

		\param r The index of the row and column to be removed from the clamped system
	*/
	void	RemoveClamped( int r );

	/*!
		\brief Calculates the force delta for a specified direction and dimension, adjusting the acceleration change for the given variable index.

		This function updates the delta force for a specific dimension and solves for the clamped force delta. If the direction is positive, the resulting delta force values are negated. The function
	   uses temporary memory allocation for matrix column extraction and performs a clamped force solving operation.

		\param d The dimension index for which to calculate the force delta
		\param dir The direction of the force, used to determine sign adjustment for the delta force
	*/
	void	CalcForceDelta( int d, float dir );

	/*!
		\brief Calculates the change in acceleration for a specified variable index by updating the force value with a step change.

		This function computes the acceleration delta for variables starting from the first clamped variable up to and including the specified index. It uses the dot product of row pointers with the
	   force delta and adds the contribution from the diagonal element multiplied by the current variable's force delta. The calculation affects variables that are not clamped and includes the current
	   variable itself.

		\param d the index of the variable for which to calculate the acceleration change
	*/
	void	CalcAccelDelta( int d );

	/*!
		\brief Updates the force value for a specific variable by applying a step change and modifies the acceleration for a degree of freedom in the LCP solver.

		This function modifies the force vector by applying a step change to the specified degree of freedom. It updates the force value for the given variable index by adding the product of the step
	   size and the delta force for that variable. The function also updates the acceleration for the specified degree of freedom in the LCP solver.

		\param d The degree of freedom index to modify
		\param step The step size to apply to the force change
	*/
	void	ChangeForce( int d, float step );

	/*!
		\brief Updates the acceleration for a specific degree of freedom in the LCP solver.

		This function modifies the acceleration value for a given degree of freedom by applying a step size to the delta acceleration vector. It operates only on variables that are not clamped,
	   including the current variable, allowing their acceleration to be adjusted. The function uses a multiply-add operation to update the acceleration values starting from the first unclamped
	   variable up to the specified degree of freedom.

		\param d the degree of freedom index to update acceleration for
		\param step the step size to apply to the acceleration update
	*/
	void	ChangeAccel( int d, float step );
};

bool idLCP_Square::FactorClamped()
{
	for( int i = 0; i < numClamped; i++ ) {
		memcpy( clamped[i], rowPtrs[i], numClamped * sizeof( float ) );
	}
	return LU_Factor( clamped, diagonal, numClamped );
}

void idLCP_Square::SolveClamped( idVecX& x, const float* b )
{
	// solve L
	LowerTriangularSolve( clamped, x.ToFloatPtr(), b, numClamped, 0 );

	// solve U
	UpperTriangularSolve( clamped, diagonal.ToFloatPtr(), x.ToFloatPtr(), x.ToFloatPtr(), numClamped );
}

void idLCP_Square::Swap( int i, int j )
{
	if( i == j ) {
		return;
	}

	SwapValues( rowPtrs[i], rowPtrs[j] );
	m.SwapColumns( i, j );
	b.SwapElements( i, j );
	lo.SwapElements( i, j );
	hi.SwapElements( i, j );
	a.SwapElements( i, j );
	f.SwapElements( i, j );
	if( boxIndex != NULL ) {
		SwapValues( boxIndex[i], boxIndex[j] );
	}
	SwapValues( side[i], side[j] );
	SwapValues( permuted[i], permuted[j] );
}

void idLCP_Square::AddClamped( int r )
{
	assert( r >= numClamped );

	// add a row at the bottom and a column at the right of the factored
	// matrix for the clamped variables

	Swap( numClamped, r );

	// add row to L
	for( int i = 0; i < numClamped; i++ ) {
		float sum = rowPtrs[numClamped][i];
		for( int j = 0; j < i; j++ ) {
			sum -= clamped[numClamped][j] * clamped[j][i];
		}
		clamped[numClamped][i] = sum * diagonal[i];
	}

	// add column to U
	for( int i = 0; i <= numClamped; i++ ) {
		float sum = rowPtrs[i][numClamped];
		for( int j = 0; j < i; j++ ) {
			sum -= clamped[i][j] * clamped[j][numClamped];
		}
		clamped[i][numClamped] = sum;
	}

	diagonal[numClamped] = 1.0f / clamped[numClamped][numClamped];

	numClamped++;
}

void idLCP_Square::RemoveClamped( int r )
{
	if( !verify( r < numClamped ) ) {
		// complete fail, most likely due to exceptional floating point values
		return;
	}

	numClamped--;

	// no need to swap and update the factored matrix when the last row and column are removed
	if( r == numClamped ) {
		return;
	}

	float* y0 = ( float* )_alloca16( numClamped * sizeof( float ) );
	float* z0 = ( float* )_alloca16( numClamped * sizeof( float ) );
	float* y1 = ( float* )_alloca16( numClamped * sizeof( float ) );
	float* z1 = ( float* )_alloca16( numClamped * sizeof( float ) );

	// the row/column need to be subtracted from the factorization
	for( int i = 0; i < numClamped; i++ ) {
		y0[i] = -rowPtrs[i][r];
	}

	memset( y1, 0, numClamped * sizeof( float ) );
	y1[r] = 1.0f;

	memset( z0, 0, numClamped * sizeof( float ) );
	z0[r] = 1.0f;

	for( int i = 0; i < numClamped; i++ ) {
		z1[i] = -rowPtrs[r][i];
	}

	// swap the to be removed row/column with the last row/column
	Swap( r, numClamped );

	// the swapped last row/column need to be added to the factorization
	for( int i = 0; i < numClamped; i++ ) {
		y0[i] += rowPtrs[i][r];
	}

	for( int i = 0; i < numClamped; i++ ) {
		z1[i] += rowPtrs[r][i];
	}
	z1[r] = 0.0f;

	// update the beginning of the to be updated row and column
	for( int i = 0; i < r; i++ ) {
		float p0	= y0[i];
		float beta1 = z1[i] * diagonal[i];

		clamped[i][r] += p0;
		for( int j = i + 1; j < numClamped; j++ ) {
			z1[j] -= beta1 * clamped[i][j];
		}
		for( int j = i + 1; j < numClamped; j++ ) {
			y0[j] -= p0 * clamped[j][i];
		}
		clamped[r][i] += beta1;
	}

	// update the lower right corner starting at r,r
	for( int i = r; i < numClamped; i++ ) {
		float diag = clamped[i][i];

		float p0 = y0[i];
		float p1 = z0[i];
		diag += p0 * p1;

		if( fabs( diag ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
			idLib::Printf( "idLCP_Square::RemoveClamped: updating factorization failed\n" );
			diag = idMath::FLT_SMALLEST_NON_DENORMAL;
		}

		float beta0 = p1 / diag;

		float q0 = y1[i];
		float q1 = z1[i];
		diag += q0 * q1;

		if( fabs( diag ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
			idLib::Printf( "idLCP_Square::RemoveClamped: updating factorization failed\n" );
			diag = idMath::FLT_SMALLEST_NON_DENORMAL;
		}

		float d		= 1.0f / diag;
		float beta1 = q1 * d;

		clamped[i][i] = diag;
		diagonal[i]	  = d;

		for( int j = i + 1; j < numClamped; j++ ) {
			d = clamped[i][j];

			d += p0 * z0[j];
			z0[j] -= beta0 * d;

			d += q0 * z1[j];
			z1[j] -= beta1 * d;

			clamped[i][j] = d;
		}

		for( int j = i + 1; j < numClamped; j++ ) {
			d = clamped[j][i];

			y0[j] -= p0 * d;
			d += beta0 * y0[j];

			y1[j] -= q0 * d;
			d += beta1 * y1[j];

			clamped[j][i] = d;
		}
	}
	return;
}

void idLCP_Square::CalcForceDelta( int d, float dir )
{
	delta_f[d] = dir;

	if( numClamped <= 0 ) {
		return;
	}

	// get column d of matrix
	float* ptr = ( float* )_alloca16( numClamped * sizeof( float ) );
	for( int i = 0; i < numClamped; i++ ) {
		ptr[i] = rowPtrs[i][d];
	}

	// solve force delta
	SolveClamped( delta_f, ptr );

	// flip force delta based on direction
	if( dir > 0.0f ) {
		ptr = delta_f.ToFloatPtr();
		for( int i = 0; i < numClamped; i++ ) {
			ptr[i] = -ptr[i];
		}
	}
}

ID_INLINE void idLCP_Square::CalcAccelDelta( int d )
{
	// only the not clamped variables, including the current variable, can have a change in acceleration
	for( int j = numClamped; j <= d; j++ ) {
		// only the clamped variables and the current variable have a force delta unequal zero
		float dot  = BigDotProduct( rowPtrs[j], delta_f.ToFloatPtr(), numClamped );
		delta_a[j] = dot + rowPtrs[j][d] * delta_f[d];
	}
}

ID_INLINE void idLCP_Square::ChangeForce( int d, float step )
{
	// only the clamped variables and current variable have a force delta unequal zero
	MultiplyAdd( f.ToFloatPtr(), step, delta_f.ToFloatPtr(), numClamped );
	f[d] += step * delta_f[d];
}

ID_INLINE void idLCP_Square::ChangeAccel( int d, float step )
{
	// only the not clamped variables, including the current variable, can have an acceleration unequal zero
	MultiplyAdd( a.ToFloatPtr() + numClamped, step, delta_a.ToFloatPtr() + numClamped, d - numClamped + 1 );
}

bool idLCP_Square::Solve( const idMatX& o_m, idVecX& o_x, const idVecX& o_b, const idVecX& o_lo, const idVecX& o_hi, const int* o_boxIndex )
{
	// true when the matrix rows are 16 byte padded
	padded = ( ( o_m.GetNumRows() + 3 ) & ~3 ) == o_m.GetNumColumns();

	assert( padded || o_m.GetNumRows() == o_m.GetNumColumns() );
	assert( o_x.GetSize() == o_m.GetNumRows() );
	assert( o_b.GetSize() == o_m.GetNumRows() );
	assert( o_lo.GetSize() == o_m.GetNumRows() );
	assert( o_hi.GetSize() == o_m.GetNumRows() );

	// allocate memory for permuted input
	f.SetData( o_m.GetNumRows(), VECX_ALLOCA( o_m.GetNumRows() ) );
	a.SetData( o_b.GetSize(), VECX_ALLOCA( o_b.GetSize() ) );
	b.SetData( o_b.GetSize(), VECX_ALLOCA( o_b.GetSize() ) );
	lo.SetData( o_lo.GetSize(), VECX_ALLOCA( o_lo.GetSize() ) );
	hi.SetData( o_hi.GetSize(), VECX_ALLOCA( o_hi.GetSize() ) );
	if( o_boxIndex != NULL ) {
		boxIndex = ( int* )_alloca16( o_x.GetSize() * sizeof( int ) );
		memcpy( boxIndex, o_boxIndex, o_x.GetSize() * sizeof( int ) );
	} else {
		boxIndex = NULL;
	}

	// we override the const on o_m here but on exit the matrix is unchanged
	m.SetData( o_m.GetNumRows(), o_m.GetNumColumns(), const_cast<float*>( o_m[0] ) );
	f.Zero();
	a.Zero();
	b  = o_b;
	lo = o_lo;
	hi = o_hi;

	// pointers to the rows of m
	rowPtrs = ( float** )_alloca16( m.GetNumRows() * sizeof( float* ) );
	for( int i = 0; i < m.GetNumRows(); i++ ) {
		rowPtrs[i] = m[i];
	}

	// tells if a variable is at the low boundary, high boundary or inbetween
	side = ( int* )_alloca16( m.GetNumRows() * sizeof( int ) );

	// index to keep track of the permutation
	permuted = ( int* )_alloca16( m.GetNumRows() * sizeof( int ) );
	for( int i = 0; i < m.GetNumRows(); i++ ) {
		permuted[i] = i;
	}

	// permute input so all unbounded variables come first
	numUnbounded = 0;
	for( int i = 0; i < m.GetNumRows(); i++ ) {
		if( lo[i] == -idMath::INFINITUM && hi[i] == idMath::INFINITUM ) {
			if( numUnbounded != i ) {
				Swap( numUnbounded, i );
			}
			numUnbounded++;
		}
	}

	// permute input so all variables using the boxIndex come last
	int boxStartIndex = m.GetNumRows();
	if( boxIndex ) {
		for( int i = m.GetNumRows() - 1; i >= numUnbounded; i-- ) {
			if( boxIndex[i] >= 0 ) {
				boxStartIndex--;
				if( boxStartIndex != i ) {
					Swap( boxStartIndex, i );
				}
			}
		}
	}

	// sub matrix for factorization
	clamped.SetData( m.GetNumRows(), m.GetNumColumns(), MATX_ALLOCA( m.GetNumRows() * m.GetNumColumns() ) );
	diagonal.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );

	// all unbounded variables are clamped
	numClamped = numUnbounded;

	// if there are unbounded variables
	if( numUnbounded ) {
		// factor and solve for unbounded variables
		if( !FactorClamped() ) {
			idLib::Printf( "idLCP_Square::Solve: unbounded factorization failed\n" );
			return false;
		}
		SolveClamped( f, b.ToFloatPtr() );

		// if there are no bounded variables we are done
		if( numUnbounded == m.GetNumRows() ) {
			o_x = f; // the vector is not permuted
			return true;
		}
	}

	int numIgnored = 0;

	// allocate for delta force and delta acceleration
	delta_f.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );
	delta_a.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );

	// solve for bounded variables
	idStr failed;
	for( int i = numUnbounded; i < m.GetNumRows(); i++ ) {
		// once we hit the box start index we can initialize the low and high boundaries of the variables using the box index
		if( i == boxStartIndex ) {
			for( int j = 0; j < boxStartIndex; j++ ) {
				o_x[permuted[j]] = f[j];
			}
			for( int j = boxStartIndex; j < m.GetNumRows(); j++ ) {
				float s = o_x[boxIndex[j]];
				if( lo[j] != -idMath::INFINITUM ) {
					lo[j] = -idMath::Fabs( lo[j] * s );
				}
				if( hi[j] != idMath::INFINITUM ) {
					hi[j] = idMath::Fabs( hi[j] * s );
				}
			}
		}

		// calculate acceleration for current variable
		float dot = BigDotProduct( rowPtrs[i], f.ToFloatPtr(), i );
		a[i]	  = dot - b[i];

		// if already at the low boundary
		if( lo[i] >= -LCP_BOUND_EPSILON && a[i] >= -LCP_ACCEL_EPSILON ) {
			side[i] = -1;
			continue;
		}

		// if already at the high boundary
		if( hi[i] <= LCP_BOUND_EPSILON && a[i] <= LCP_ACCEL_EPSILON ) {
			side[i] = 1;
			continue;
		}

		// if inside the clamped region
		if( idMath::Fabs( a[i] ) <= LCP_ACCEL_EPSILON ) {
			side[i] = 0;
			AddClamped( i );
			continue;
		}

		// drive the current variable into a valid region
		int n = 0;
		for( ; n < maxIterations; n++ ) {
			// direction to move
			float dir = ( a[i] <= 0.0f ) ? 1.0f : -1.0f;

			// calculate force delta
			CalcForceDelta( i, dir );

			// calculate acceleration delta: delta_a = m * delta_f;
			CalcAccelDelta( i );

			float maxStep;
			int	  limit;
			int	  limitSide;

			// maximum step we can take
			GetMaxStep(
				f.ToFloatPtr(), a.ToFloatPtr(), delta_f.ToFloatPtr(), delta_a.ToFloatPtr(), lo.ToFloatPtr(), hi.ToFloatPtr(), side, numUnbounded, numClamped, i, dir, maxStep, limit, limitSide );

			if( maxStep <= 0.0f ) {
#ifdef IGNORE_UNSATISFIABLE_VARIABLES
				// ignore the current variable completely
				lo[i] = hi[i] = 0.0f;
				f[i]		  = 0.0f;
				side[i]		  = -1;
				numIgnored++;
#else
				failed.Format( "invalid step size %.4f", maxStep );
				for( int j = i; j < m.GetNumRows(); j++ ) {
					f[j] = 0.0f;
				}
				numIgnored = m.GetNumRows() - i;
#endif
				break;
			}

			// change force
			ChangeForce( i, maxStep );

			// change acceleration
			ChangeAccel( i, maxStep );

			// clamp/unclamp the variable that limited this step
			side[limit] = limitSide;
			if( limitSide == 0 ) {
				a[limit] = 0.0f;
				AddClamped( limit );
			} else if( limitSide == -1 ) {
				f[limit] = lo[limit];
				if( limit != i ) {
					RemoveClamped( limit );
				}
			} else if( limitSide == 1 ) {
				f[limit] = hi[limit];
				if( limit != i ) {
					RemoveClamped( limit );
				}
			}

			// if the current variable limited the step we can continue with the next variable
			if( limit == i ) {
				break;
			}
		}

		if( n >= maxIterations ) {
			failed.Format( "max iterations %d", maxIterations );
			break;
		}

		if( failed.Length() ) {
			break;
		}
	}

#ifdef IGNORE_UNSATISFIABLE_VARIABLES
	if( numIgnored ) {
		if( lcp_showFailures.GetBool() ) {
			idLib::Printf( "idLCP_Square::Solve: %d of %d bounded variables ignored\n", numIgnored, m.GetNumRows() - numUnbounded );
		}
	}
#endif

	// if failed clear remaining forces
	if( failed.Length() ) {
		if( lcp_showFailures.GetBool() ) {
			idLib::Printf( "idLCP_Square::Solve: %s (%d of %d bounded variables ignored)\n", failed.c_str(), numIgnored, m.GetNumRows() - numUnbounded );
		}
	}

#if defined( _DEBUG ) && 0
	if( failed.Length() ) {
		// test whether or not the solution satisfies the complementarity conditions
		for( int i = 0; i < m.GetNumRows(); i++ ) {
			a[i] = -b[i];
			for( int j = 0; j < m.GetNumRows(); j++ ) {
				a[i] += rowPtrs[i][j] * f[j];
			}

			if( f[i] == lo[i] ) {
				if( lo[i] != hi[i] && a[i] < -LCP_ACCEL_EPSILON ) {
					int bah1 = 1;
				}
			} else if( f[i] == hi[i] ) {
				if( lo[i] != hi[i] && a[i] > LCP_ACCEL_EPSILON ) {
					int bah2 = 1;
				}
			} else if( f[i] < lo[i] || f[i] > hi[i] || idMath::Fabs( a[i] ) > 1.0f ) {
				int bah3 = 1;
			}
		}
	}
#endif

	// unpermute result
	for( int i = 0; i < f.GetSize(); i++ ) {
		o_x[permuted[i]] = f[i];
	}

	return true;
}

/*!
	\class idLCP_Symmetric
	\brief A symmetric linear complementarity problem solver with support for box constraints and clamped variables.

	This class provides methods for solving symmetric linear complementarity problems (LCPs) that arise in physics simulation, particularly in rigid body dynamics. It supports both unbounded and
   bounded variables through box constraints, and handles clamped variables that are frozen in certain directions during the solution process. The implementation uses LDLT factorization for numerical
   stability and includes specialized methods for adding and removing clamped variables, which is essential for dynamic constraint handling. The class also provides functionality for computing force
   and acceleration deltas, which are important for iterative solvers that update constraint forces based on velocity changes. The design supports both direct solution methods and incremental updates
   to the factorization, enabling efficient handling of time-varying constraints.

*/
class idLCP_Symmetric : public idLCP
{
public:
	/*!
		\brief Solves a symmetric linear complementarity problem with optional box constraints.

		This function solves a symmetric linear complementarity problem of the form M*x = b + f, where f is the force vector and x is the solution vector. The problem is subject to box constraints
	   defined by lower and upper bounds. The function handles both unbounded and bounded variables, and uses a factorization-based approach to find a solution. It supports permutation of variables to
	   optimize the solution process, with special handling for unbounded variables and box constraints. The matrix M is assumed to be symmetric and positive semi-definite. The solution vector x is
	   updated in place.

		\param o_m The symmetric matrix M of the linear complementarity problem
		\param o_x The solution vector x, updated in place with the result
		\param o_b The vector b in the linear complementarity problem
		\param o_lo The lower bounds for variables, with -INFINITUM indicating no lower bound
		\param o_hi The upper bounds for variables, with INFINITUM indicating no upper bound
		\param o_boxIndex Optional array of indices for box constraints, or NULL if not used
		\return True if the solution converged successfully, false if the factorization failed or an unsatisfiable constraint was encountered
	*/
	virtual bool Solve( const idMatX& o_m, idVecX& o_x, const idVecX& o_b, const idVecX& o_lo, const idVecX& o_hi, const int* o_boxIndex );

private:
	idMatX	m;					// original matrix
	idVecX	b;					// right hand side
	idVecX	lo, hi;				// low and high bounds
	idVecX	f, a;				// force and acceleration
	idVecX	delta_f, delta_a;	// delta force and delta acceleration
	idMatX	clamped;			// LDLt factored sub matrix for clamped variables
	idVecX	diagonal;			// reciprocal of diagonal of LDLt factored sub matrix for clamped variables
	idVecX	solveCache1;		// intermediate result cached in SolveClamped
	idVecX	solveCache2;		// "
	int		numUnbounded;		// number of unbounded variables
	int		numClamped;			// number of clamped variables
	int		clampedChangeStart; // lowest row/column changed in the clamped matrix during an iteration
	float** rowPtrs;			// pointers to the rows of m
	int*	boxIndex;			// box index
	int*	side;				// tells if a variable is at the low boundary = -1, high boundary = 1 or inbetween = 0
	int*	permuted;			// index to keep track of the permutation
	bool	padded;				// set to true if the rows of the initial matrix are 16 byte padded

	/*!
		\brief Performs LDLT factorization on a clamped system to solve a linear complementarity problem.

		This function prepares the clamped system by copying the row pointers into the clamped matrix and then applies LDLT factorization to solve the linear complementarity problem. The factorization
	   is performed in-place on the clamped matrix and diagonal vector.

		\return True if the factorization was successful, false otherwise.
	*/
	bool	FactorClamped();

	/*!
		\brief Solves a clamped linear complementarity problem using LU decomposition and triangular solves.

		This function performs the solution of a clamped linear complementarity problem by first solving the lower triangular system, then scaling with the diagonal, and finally solving the upper
	   triangular system. It uses precomputed LU decomposition data stored in clamped. The function modifies the x vector in place with the solution and updates the clampedChangeStart to reflect the
	   current clamped region size.

		\param x The vector to store the solution, modified in place
		\param b The right-hand side vector for the linear system
	*/
	void	SolveClamped( idVecX& x, const float* b );

	/*!
		\brief Swaps elements at indices i and j in the symmetric LCP data structures.

		This function performs a swap operation between two elements identified by indices i and j within various data structures of the symmetric LCP class. The swap affects row pointers, matrix
	   columns, vector elements, and other associated arrays. The function includes a check to avoid unnecessary operations when i equals j. Additionally, if boxIndex is not NULL, it swaps the
	   corresponding elements in that array as well.

		\param i index of the first element to swap
		\param j index of the second element to swap
	*/
	void	Swap( int i, int j );

	/*!
		\brief Adds a clamped row to the symmetric LCP matrix by updating the factorization.

		This function inserts a new clamped variable into the symmetric LCP system by updating the matrix factorization. It handles the necessary computations for adding a new row and column to the
	   factored matrix, performing either a cached or uncached lower triangular solve depending on the useSolveCache flag. The function ensures numerical stability by checking for near-zero diagonal
	   values and clamping them to a minimum non-denormal value.

		\param r The row index to be added as a clamped variable
		\param useSolveCache Flag indicating whether to use cached solve results for performance
	*/
	void	AddClamped( int r, bool useSolveCache );

	/*!
		\brief Removes a clamped row and column from the LCP symmetric matrix

		This function removes a specified clamped row and column from the LCP symmetric matrix and updates the factorization accordingly. It handles the case where the removed row/column is the last
	   one, in which case no swapping is needed. For other cases, it swaps the row/column to be removed with the last row/column and then performs updates to the factorized matrix. The function also
	   handles special cases for the first row and the last row/column updates. The factorization is updated using a simultaneous update/downdate algorithm that maintains the numerical stability of
	   the matrix decomposition.

		\param r the index of the clamped row/column to be removed
	*/
	void	RemoveClamped( int r );

	/*!
		\brief Sets the force delta for a specified direction and solves the clamped constraints.

		Computes the change in acceleration for variables up to and including the specified index. The function first sets the delta force for the given direction, then solves the clamped constraints
	   if any are present. If the direction is positive, the resulting force delta is flipped in sign for the clamped variables.

		\param d the index of the variable to set the force delta for
		\param dir the direction of the force delta
	*/
	void	CalcForceDelta( int d, float dir );

	/*!
		\brief Computes the change in acceleration for variables up to and including the specified index.

		This function calculates the acceleration delta for variables in the LCP (Linear Complementarity Problem) system. It processes variables starting from the first clamped variable up to and
	   including the specified index. The calculation involves a dot product between the row pointer and the force delta vector, plus a contribution from the diagonal element and the force delta of
	   the current variable. The function modifies the acceleration delta array for the affected variables.

		\param d The index up to which the acceleration change is calculated, inclusive
	*/
	void	CalcAccelDelta( int d );

	/*!
		\brief Updates the force value for a specific variable by applying a step change.

		This function modifies the force value for a specified variable in the LCP solver by applying a step change. It updates the force vector by adding a scaled delta force for the specified
	   variable, while also updating the force vector for the clamped variables.

		\param d the index of the variable to update
		\param step the step size to apply to the force change
	*/
	void	ChangeForce( int d, float step );

	/*!
		\brief Updates the acceleration for a specific variable in the LCP solver by modifying the delta_a array.

		This function modifies the acceleration values for variables in the LCP solver. It operates on a subset of the delta_a array starting from the index numClamped, updating elements from
	   numClamped to d-1. The modification is performed using the MultiplyAdd operation which adds the product of step and the delta_a values to the corresponding a values.

		\param d The index of the variable to update, inclusive
		\param step The step size to apply to the acceleration update
	*/
	void	ChangeAccel( int d, float step );
};

bool idLCP_Symmetric::FactorClamped()
{
	clampedChangeStart = 0;

	for( int i = 0; i < numClamped; i++ ) {
		memcpy( clamped[i], rowPtrs[i], numClamped * sizeof( float ) );
	}
	return LDLT_Factor( clamped, diagonal, numClamped );
}

void idLCP_Symmetric::SolveClamped( idVecX& x, const float* b )
{
	// solve L
	LowerTriangularSolve( clamped, solveCache1.ToFloatPtr(), b, numClamped, clampedChangeStart );

	// scale with D
	Multiply( solveCache2.ToFloatPtr(), solveCache1.ToFloatPtr(), diagonal.ToFloatPtr(), numClamped );

	// solve Lt
	LowerTriangularSolveTranspose( clamped, x.ToFloatPtr(), solveCache2.ToFloatPtr(), numClamped );

	clampedChangeStart = numClamped;
}

void idLCP_Symmetric::Swap( int i, int j )
{
	if( i == j ) {
		return;
	}

	SwapValues( rowPtrs[i], rowPtrs[j] );
	m.SwapColumns( i, j );
	b.SwapElements( i, j );
	lo.SwapElements( i, j );
	hi.SwapElements( i, j );
	a.SwapElements( i, j );
	f.SwapElements( i, j );
	if( boxIndex != NULL ) {
		SwapValues( boxIndex[i], boxIndex[j] );
	}
	SwapValues( side[i], side[j] );
	SwapValues( permuted[i], permuted[j] );
}

void idLCP_Symmetric::AddClamped( int r, bool useSolveCache )
{
	assert( r >= numClamped );

	if( numClamped < clampedChangeStart ) {
		clampedChangeStart = numClamped;
	}

	// add a row at the bottom and a column at the right of the factored
	// matrix for the clamped variables

	Swap( numClamped, r );

	// solve for v in L * v = rowPtr[numClamped]
	float dot;
	if( useSolveCache ) {
		// the lower triangular solve was cached in SolveClamped called by CalcForceDelta
		memcpy( clamped[numClamped], solveCache2.ToFloatPtr(), numClamped * sizeof( float ) );
		// calculate row dot product
		dot = BigDotProduct( solveCache2.ToFloatPtr(), solveCache1.ToFloatPtr(), numClamped );

	} else {
		float* v = ( float* )_alloca16( numClamped * sizeof( float ) );

		LowerTriangularSolve( clamped, v, rowPtrs[numClamped], numClamped, 0 );
		// add bottom row to L
		Multiply( clamped[numClamped], v, diagonal.ToFloatPtr(), numClamped );
		// calculate row dot product
		dot = BigDotProduct( clamped[numClamped], v, numClamped );
	}

	// update diagonal[numClamped]
	float d = rowPtrs[numClamped][numClamped] - dot;

	if( fabs( d ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
		idLib::Printf( "idLCP_Symmetric::AddClamped: updating factorization failed\n" );
		d = idMath::FLT_SMALLEST_NON_DENORMAL;
	}

	clamped[numClamped][numClamped] = d;
	diagonal[numClamped]			= 1.0f / d;

	numClamped++;
}

void idLCP_Symmetric::RemoveClamped( int r )
{
	if( !verify( r < numClamped ) ) {
		// complete fail, most likely due to exceptional floating point values
		return;
	}

	if( r < clampedChangeStart ) {
		clampedChangeStart = r;
	}

	numClamped--;

	// no need to swap and update the factored matrix when the last row and column are removed
	if( r == numClamped ) {
		return;
	}

	// swap the to be removed row/column with the last row/column
	Swap( r, numClamped );

	// update the factored matrix
	float* addSub = ( float* )_alloca16( numClamped * sizeof( float ) );

	if( r == 0 ) {
		if( numClamped == 1 ) {
			float diag = rowPtrs[0][0];
			if( fabs( diag ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
				idLib::Printf( "idLCP_Symmetric::RemoveClamped: updating factorization failed\n" );
				diag = idMath::FLT_SMALLEST_NON_DENORMAL;
			}
			clamped[0][0] = diag;
			diagonal[0]	  = 1.0f / diag;
			return;
		}

		// calculate the row/column to be added to the lower right sub matrix starting at (r, r)
		float* original = rowPtrs[numClamped];
		float* ptr		= rowPtrs[r];
		addSub[0]		= ptr[0] - original[numClamped];
		for( int i = 1; i < numClamped; i++ ) {
			addSub[i] = ptr[i] - original[i];
		}

	} else {
		float* v = ( float* )_alloca16( numClamped * sizeof( float ) );

		// solve for v in L * v = rowPtr[r]
		LowerTriangularSolve( clamped, v, rowPtrs[r], r, 0 );

		// update removed row
		Multiply( clamped[r], v, diagonal.ToFloatPtr(), r );

		// if the last row/column of the matrix is updated
		if( r == numClamped - 1 ) {
			// only calculate new diagonal
			float dot  = BigDotProduct( clamped[r], v, r );
			float diag = rowPtrs[r][r] - dot;
			if( fabs( diag ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
				idLib::Printf( "idLCP_Symmetric::RemoveClamped: updating factorization failed\n" );
				diag = idMath::FLT_SMALLEST_NON_DENORMAL;
			}
			clamped[r][r] = diag;
			diagonal[r]	  = 1.0f / diag;
			return;
		}

		// calculate the row/column to be added to the lower right sub matrix starting at (r, r)
		for( int i = 0; i < r; i++ ) {
			v[i] = clamped[r][i] * clamped[i][i];
		}
		for( int i = r; i < numClamped; i++ ) {
			float sum;
			if( i == r ) {
				sum = clamped[r][r];
			} else {
				sum = clamped[r][r] * clamped[i][r];
			}
			float* ptr = clamped[i];
			for( int j = 0; j < r; j++ ) {
				sum += ptr[j] * v[j];
			}
			addSub[i] = rowPtrs[r][i] - sum;
		}
	}

	// add row/column to the lower right sub matrix starting at (r, r)

	float* v1 = ( float* )_alloca16( numClamped * sizeof( float ) );
	float* v2 = ( float* )_alloca16( numClamped * sizeof( float ) );

	float  diag = idMath::SQRT_1OVER2;
	v1[r]		= ( 0.5f * addSub[r] + 1.0f ) * diag;
	v2[r]		= ( 0.5f * addSub[r] - 1.0f ) * diag;
	for( int i = r + 1; i < numClamped; i++ ) {
		v1[i] = v2[i] = addSub[i] * diag;
	}

	float alpha1 = 1.0f;
	float alpha2 = -1.0f;

	// simultaneous update/downdate of the sub matrix starting at (r, r)
	int	  n = clamped.GetNumColumns();
	for( int i = r; i < numClamped; i++ ) {
		diag		  = clamped[i][i];
		float p1	  = v1[i];
		float newDiag = diag + alpha1 * p1 * p1;

		if( fabs( newDiag ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
			idLib::Printf( "idLCP_Symmetric::RemoveClamped: updating factorization failed\n" );
			newDiag = idMath::FLT_SMALLEST_NON_DENORMAL;
		}

		alpha1 /= newDiag;
		float beta1 = p1 * alpha1;
		alpha1 *= diag;

		diag	 = newDiag;
		float p2 = v2[i];
		newDiag	 = diag + alpha2 * p2 * p2;

		if( fabs( newDiag ) < idMath::FLT_SMALLEST_NON_DENORMAL ) {
			idLib::Printf( "idLCP_Symmetric::RemoveClamped: updating factorization failed\n" );
			newDiag = idMath::FLT_SMALLEST_NON_DENORMAL;
		}

		clamped[i][i]	 = newDiag;
		float invNewDiag = 1.0f / newDiag;
		diagonal[i]		 = invNewDiag;

		alpha2 *= invNewDiag;
		float beta2 = p2 * alpha2;
		alpha2 *= diag;

		// update column below diagonal (i,i)
		float* ptr = clamped.ToFloatPtr() + i;

		int	   j;
		for( j = i + 1; j < numClamped - 1; j += 2 ) {
			float sum0 = ptr[( j + 0 ) * n];
			float sum1 = ptr[( j + 1 ) * n];

			v1[j + 0] -= p1 * sum0;
			v1[j + 1] -= p1 * sum1;

			sum0 += beta1 * v1[j + 0];
			sum1 += beta1 * v1[j + 1];

			v2[j + 0] -= p2 * sum0;
			v2[j + 1] -= p2 * sum1;

			sum0 += beta2 * v2[j + 0];
			sum1 += beta2 * v2[j + 1];

			ptr[( j + 0 ) * n] = sum0;
			ptr[( j + 1 ) * n] = sum1;
		}

		for( ; j < numClamped; j++ ) {
			float sum = ptr[j * n];

			v1[j] -= p1 * sum;
			sum += beta1 * v1[j];

			v2[j] -= p2 * sum;
			sum += beta2 * v2[j];

			ptr[j * n] = sum;
		}
	}
}

ID_INLINE void idLCP_Symmetric::CalcForceDelta( int d, float dir )
{
	delta_f[d] = dir;

	if( numClamped == 0 ) {
		return;
	}

	// solve force delta
	SolveClamped( delta_f, rowPtrs[d] );

	// flip force delta based on direction
	if( dir > 0.0f ) {
		float* ptr = delta_f.ToFloatPtr();
		for( int i = 0; i < numClamped; i++ ) {
			ptr[i] = -ptr[i];
		}
	}
}

ID_INLINE void idLCP_Symmetric::CalcAccelDelta( int d )
{
	// only the not clamped variables, including the current variable, can have a change in acceleration
	for( int j = numClamped; j <= d; j++ ) {
		// only the clamped variables and the current variable have a force delta unequal zero
		float dot  = BigDotProduct( rowPtrs[j], delta_f.ToFloatPtr(), numClamped );
		delta_a[j] = dot + rowPtrs[j][d] * delta_f[d];
	}
}

ID_INLINE void idLCP_Symmetric::ChangeForce( int d, float step )
{
	// only the clamped variables and current variable have a force delta unequal zero
	MultiplyAdd( f.ToFloatPtr(), step, delta_f.ToFloatPtr(), numClamped );
	f[d] += step * delta_f[d];
}

ID_INLINE void idLCP_Symmetric::ChangeAccel( int d, float step )
{
	// only the not clamped variables, including the current variable, can have an acceleration unequal zero
	MultiplyAdd( a.ToFloatPtr() + numClamped, step, delta_a.ToFloatPtr() + numClamped, d - numClamped + 1 );
}

bool idLCP_Symmetric::Solve( const idMatX& o_m, idVecX& o_x, const idVecX& o_b, const idVecX& o_lo, const idVecX& o_hi, const int* o_boxIndex )
{
	// true when the matrix rows are 16 byte padded
	padded = ( ( o_m.GetNumRows() + 3 ) & ~3 ) == o_m.GetNumColumns();

	assert( padded || o_m.GetNumRows() == o_m.GetNumColumns() );
	assert( o_x.GetSize() == o_m.GetNumRows() );
	assert( o_b.GetSize() == o_m.GetNumRows() );
	assert( o_lo.GetSize() == o_m.GetNumRows() );
	assert( o_hi.GetSize() == o_m.GetNumRows() );

	// allocate memory for permuted input
	f.SetData( o_m.GetNumRows(), VECX_ALLOCA( o_m.GetNumRows() ) );
	a.SetData( o_b.GetSize(), VECX_ALLOCA( o_b.GetSize() ) );
	b.SetData( o_b.GetSize(), VECX_ALLOCA( o_b.GetSize() ) );
	lo.SetData( o_lo.GetSize(), VECX_ALLOCA( o_lo.GetSize() ) );
	hi.SetData( o_hi.GetSize(), VECX_ALLOCA( o_hi.GetSize() ) );
	if( o_boxIndex != NULL ) {
		boxIndex = ( int* )_alloca16( o_x.GetSize() * sizeof( int ) );
		memcpy( boxIndex, o_boxIndex, o_x.GetSize() * sizeof( int ) );
	} else {
		boxIndex = NULL;
	}

	// we override the const on o_m here but on exit the matrix is unchanged
	m.SetData( o_m.GetNumRows(), o_m.GetNumColumns(), const_cast<float*>( o_m[0] ) );
	f.Zero();
	a.Zero();
	b  = o_b;
	lo = o_lo;
	hi = o_hi;

	// pointers to the rows of m
	rowPtrs = ( float** )_alloca16( m.GetNumRows() * sizeof( float* ) );
	for( int i = 0; i < m.GetNumRows(); i++ ) {
		rowPtrs[i] = m[i];
	}

	// tells if a variable is at the low boundary, high boundary or inbetween
	side = ( int* )_alloca16( m.GetNumRows() * sizeof( int ) );

	// index to keep track of the permutation
	permuted = ( int* )_alloca16( m.GetNumRows() * sizeof( int ) );
	for( int i = 0; i < m.GetNumRows(); i++ ) {
		permuted[i] = i;
	}

	// permute input so all unbounded variables come first
	numUnbounded = 0;
	for( int i = 0; i < m.GetNumRows(); i++ ) {
		if( lo[i] == -idMath::INFINITUM && hi[i] == idMath::INFINITUM ) {
			if( numUnbounded != i ) {
				Swap( numUnbounded, i );
			}
			numUnbounded++;
		}
	}

	// permute input so all variables using the boxIndex come last
	int boxStartIndex = m.GetNumRows();
	if( boxIndex != NULL ) {
		for( int i = m.GetNumRows() - 1; i >= numUnbounded; i-- ) {
			if( boxIndex[i] >= 0 ) {
				boxStartIndex--;
				if( boxStartIndex != i ) {
					Swap( boxStartIndex, i );
				}
			}
		}
	}

	// sub matrix for factorization
	clamped.SetDataCacheLines( m.GetNumRows(), m.GetNumColumns(), MATX_ALLOCA_CACHE_LINES( m.GetNumRows() * m.GetNumColumns() ), true );
	diagonal.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );
	solveCache1.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );
	solveCache2.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );

	// all unbounded variables are clamped
	numClamped = numUnbounded;

	// if there are unbounded variables
	if( numUnbounded ) {
		// factor and solve for unbounded variables
		if( !FactorClamped() ) {
			idLib::Printf( "idLCP_Symmetric::Solve: unbounded factorization failed\n" );
			return false;
		}
		SolveClamped( f, b.ToFloatPtr() );

		// if there are no bounded variables we are done
		if( numUnbounded == m.GetNumRows() ) {
			o_x = f; // the vector is not permuted
			return true;
		}
	}

	int numIgnored = 0;

	// allocate for delta force and delta acceleration
	delta_f.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );
	delta_a.SetData( m.GetNumRows(), VECX_ALLOCA( m.GetNumRows() ) );

	// solve for bounded variables
	idStr failed;
	for( int i = numUnbounded; i < m.GetNumRows(); i++ ) {
		clampedChangeStart = 0;

		// once we hit the box start index we can initialize the low and high boundaries of the variables using the box index
		if( i == boxStartIndex ) {
			for( int j = 0; j < boxStartIndex; j++ ) {
				o_x[permuted[j]] = f[j];
			}
			for( int j = boxStartIndex; j < m.GetNumRows(); j++ ) {
				float s = o_x[boxIndex[j]];
				if( lo[j] != -idMath::INFINITUM ) {
					lo[j] = -idMath::Fabs( lo[j] * s );
				}
				if( hi[j] != idMath::INFINITUM ) {
					hi[j] = idMath::Fabs( hi[j] * s );
				}
			}
		}

		// calculate acceleration for current variable
		float dot = BigDotProduct( rowPtrs[i], f.ToFloatPtr(), i );
		a[i]	  = dot - b[i];

		// if already at the low boundary
		if( lo[i] >= -LCP_BOUND_EPSILON && a[i] >= -LCP_ACCEL_EPSILON ) {
			side[i] = -1;
			continue;
		}

		// if already at the high boundary
		if( hi[i] <= LCP_BOUND_EPSILON && a[i] <= LCP_ACCEL_EPSILON ) {
			side[i] = 1;
			continue;
		}

		// if inside the clamped region
		if( idMath::Fabs( a[i] ) <= LCP_ACCEL_EPSILON ) {
			side[i] = 0;
			AddClamped( i, false );
			continue;
		}

		// drive the current variable into a valid region
		int n = 0;
		for( ; n < maxIterations; n++ ) {
			// direction to move
			float dir = ( a[i] <= 0.0f ) ? 1.0f : -1.0f;

			// calculate force delta
			CalcForceDelta( i, dir );

			// calculate acceleration delta: delta_a = m * delta_f;
			CalcAccelDelta( i );

			float maxStep;
			int	  limit;
			int	  limitSide;

			// maximum step we can take
			GetMaxStep(
				f.ToFloatPtr(), a.ToFloatPtr(), delta_f.ToFloatPtr(), delta_a.ToFloatPtr(), lo.ToFloatPtr(), hi.ToFloatPtr(), side, numUnbounded, numClamped, i, dir, maxStep, limit, limitSide );

			if( maxStep <= 0.0f ) {
#ifdef IGNORE_UNSATISFIABLE_VARIABLES
				// ignore the current variable completely
				lo[i] = hi[i] = 0.0f;
				f[i]		  = 0.0f;
				side[i]		  = -1;
				numIgnored++;
#else
				failed.Format( "invalid step size %.4f", maxStep );
				for( int j = i; j < m.GetNumRows(); j++ ) {
					f[j] = 0.0f;
				}
				numIgnored = m.GetNumRows() - i;
#endif
				break;
			}

			// change force
			ChangeForce( i, maxStep );

			// change acceleration
			ChangeAccel( i, maxStep );

			// clamp/unclamp the variable that limited this step
			side[limit] = limitSide;
			if( limitSide == 0 ) {
				a[limit] = 0.0f;
				AddClamped( limit, ( limit == i ) );
			} else if( limitSide == -1 ) {
				f[limit] = lo[limit];
				if( limit != i ) {
					RemoveClamped( limit );
				}
			} else if( limitSide == 1 ) {
				f[limit] = hi[limit];
				if( limit != i ) {
					RemoveClamped( limit );
				}
			}

			// if the current variable limited the step we can continue with the next variable
			if( limit == i ) {
				break;
			}
		}

		if( n >= maxIterations ) {
			failed.Format( "max iterations %d", maxIterations );
			break;
		}

		if( failed.Length() ) {
			break;
		}
	}

#ifdef IGNORE_UNSATISFIABLE_VARIABLES
	if( numIgnored ) {
		if( lcp_showFailures.GetBool() ) {
			idLib::Printf( "idLCP_Symmetric::Solve: %d of %d bounded variables ignored\n", numIgnored, m.GetNumRows() - numUnbounded );
		}
	}
#endif

	// if failed clear remaining forces
	if( failed.Length() ) {
		if( lcp_showFailures.GetBool() ) {
			idLib::Printf( "idLCP_Symmetric::Solve: %s (%d of %d bounded variables ignored)\n", failed.c_str(), numIgnored, m.GetNumRows() - numUnbounded );
		}
	}

#if defined( _DEBUG ) && 0
	if( failed.Length() ) {
		// test whether or not the solution satisfies the complementarity conditions
		for( int i = 0; i < m.GetNumRows(); i++ ) {
			a[i] = -b[i];
			for( j = 0; j < m.GetNumRows(); j++ ) {
				a[i] += rowPtrs[i][j] * f[j];
			}

			if( f[i] == lo[i] ) {
				if( lo[i] != hi[i] && a[i] < -LCP_ACCEL_EPSILON ) {
					int bah1 = 1;
				}
			} else if( f[i] == hi[i] ) {
				if( lo[i] != hi[i] && a[i] > LCP_ACCEL_EPSILON ) {
					int bah2 = 1;
				}
			} else if( f[i] < lo[i] || f[i] > hi[i] || idMath::Fabs( a[i] ) > 1.0f ) {
				int bah3 = 1;
			}
		}
	}
#endif

	// unpermute result
	for( int i = 0; i < f.GetSize(); i++ ) {
		o_x[permuted[i]] = f[i];
	}

	return true;
}

idLCP* idLCP::AllocSquare()
{
	idLCP* lcp = new idLCP_Square;
	lcp->SetMaxIterations( 32 );
	return lcp;
}

idLCP* idLCP::AllocSymmetric()
{
	idLCP* lcp = new idLCP_Symmetric;
	lcp->SetMaxIterations( 32 );
	return lcp;
}

idLCP::~idLCP()
{
}

void idLCP::SetMaxIterations( int max )
{
	maxIterations = max;
}

int idLCP::GetMaxIterations()
{
	return maxIterations;
}

void idLCP::Test_f( const idCmdArgs& args )
{
#ifdef ENABLE_TEST_CODE
	DotProduct_Test();
	LowerTriangularSolve_Test();
	LowerTriangularSolveTranspose_Test();
	LDLT_Factor_Test();
#endif
}
