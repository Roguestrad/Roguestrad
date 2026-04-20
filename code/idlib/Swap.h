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
#ifndef __SWAP_H__
#define __SWAP_H__

//! Checks if the given type is a pointer type.
template<typename type>
bool IsPointer( type )
{
	return false;
}

//! Returns true indicating that the input is a pointer.
template<typename type>
bool IsPointer( type* )
{
	return true;
}

/*
================================================
The *Swap* static template class, idSwap, is used by the SwapClass template class for
performing EndianSwapping.
================================================
*/
class idSwap
{
public:
	// #define SwapBytes( x, y )		(x) ^= (y) ^= (x) ^= (y)
#define SwapBytes( x, y ) \
	{                     \
		byte t = ( x );   \
		( x )  = ( y );   \
		( y )  = t;       \
	}

	/*!
		\brief Swaps the byte order of the given value if it is not a pointer.

		This function performs byte swapping on the provided value to ensure proper endianness. It asserts that the value is not a pointer since pointer values should never be stored to disk. The
	   function is intended to be used for types that require byte order conversion.

		\param c Reference to the value that needs byte order swapping
		\throws Assertion failure if the input value is a pointer
	*/
	template<class type>
	static void Little( type& c )
	{
		// byte swapping pointers is pointless because we should never store pointers on disk
		assert( !IsPointer( c ) );
	}

	/*!
		\brief Converts the byte order of a value from little-endian to big-endian format

		This function performs byte swapping on a value to convert its endianness from little-endian to big-endian. It handles different data sizes (1, 2, 4, and 8 bytes) by swapping the appropriate
	   bytes. The function asserts that the input is not a pointer type since pointer byte swapping is not meaningful in this context. For types larger than 8 bytes or smaller than 1 byte, the
	   function will assert and terminate.

		\param c Reference to the value whose byte order will be swapped
		\throws Assertion failure if the input is a pointer type or if the size of the type is not 1, 2, 4, or 8 bytes
	*/
	template<class type>
	static void Big( type& c )
	{
		// byte swapping pointers is pointless because we should never store pointers on disk
		assert( !IsPointer( c ) );

		if( sizeof( type ) == 1 ) {
		} else if( sizeof( type ) == 2 ) {
			byte* b = ( byte* )&c;
			SwapBytes( b[0], b[1] );
		} else if( sizeof( type ) == 4 ) {
			byte* b = ( byte* )&c;
			SwapBytes( b[0], b[3] );
			SwapBytes( b[1], b[2] );
		} else if( sizeof( type ) == 8 ) {
			byte* b = ( byte* )&c;
			SwapBytes( b[0], b[7] );
			SwapBytes( b[1], b[6] );
			SwapBytes( b[2], b[5] );
			SwapBytes( b[3], b[4] );
		} else {
			assert( false );
		}
	}

	/*!
		\brief Converts the byte order of an array of values from big-endian to little-endian.

		This function performs in-place byte order conversion for an array of values. It is typically used to ensure proper byte ordering when reading data from files or network streams that may have
	   been formatted in big-endian byte order. The function operates on the array in-place, modifying the original data.

		\param c Pointer to the array of values to be converted
		\param count Number of elements in the array to convert
	*/
	template<class type>
	static void LittleArray( type* c, int count )
	{
	}

	/*!
		\brief Swaps the byte order of elements in the given array from big-endian to little-endian representation.

		This function iterates through the specified number of elements in the array and applies the Big function to each element, effectively converting their byte order from big-endian to
	   little-endian format. It is commonly used when dealing with data that needs to be byte-swapped for cross-platform compatibility or specific memory layout requirements.

		\param c Pointer to the array of elements to be byte-swapped
		\param count Number of elements in the array to process
	*/
	template<class type>
	static void BigArray( type* c, int count )
	{
		for( int i = 0; i < count; i++ ) {
			Big( c[i] );
		}
	}

	/*!
		\brief Converts an integer into four six-bit values stored in a byte array

		This function takes a 32-bit integer and breaks it down into four 6-bit values, which are stored in the provided output byte array. Each six-bit value represents a portion of the original
	   integer's bytes. The function extracts these values by performing bitwise operations on the bytes of the integer. This is typically used for encoding purposes where data needs to be represented
	   using only 6-bit values, such as in base64-like encoding schemes.

		\param out Output array of four bytes to store the six-bit values
		\param src Input 32-bit integer to be converted into six-bit values
	*/
	static void SixtetsForInt( byte* out, int src )
	{
		byte* b = ( byte* )&src;
		out[0]	= ( b[0] & 0xfc ) >> 2;
		out[1]	= ( ( b[0] & 0x3 ) << 4 ) + ( ( b[1] & 0xf0 ) >> 4 );
		out[2]	= ( ( b[1] & 0xf ) << 2 ) + ( ( b[2] & 0xc0 ) >> 6 );
		out[3]	= b[2] & 0x3f;
	}

	/*!
		\brief Converts four 6-bit values from a byte array into a 32-bit integer.

		This function takes an array of four bytes where each byte represents a 6-bit value and combines them into a single 32-bit integer. The function performs bit manipulation to properly extract
	   and reposition the bits from the input array into the correct positions in the output integer. Each byte in the input array contributes to the final result through specific bit shifts and masks
	   to ensure correct bit placement.

		\param in Pointer to an array of four bytes containing 6-bit values to be combined into a 32-bit integer
		\return A 32-bit integer containing the combined bits from the input byte array
	*/
	static int IntForSixtets( byte* in )
	{
		int	  ret = 0;
		byte* b	  = ( byte* )&ret;
		b[0] |= in[0] << 2;
		b[0] |= ( in[1] & 0x30 ) >> 4;
		b[1] |= ( in[1] & 0xf ) << 4;
		b[1] |= ( in[2] & 0x3c ) >> 2;
		b[2] |= ( in[2] & 0x3 ) << 6;
		b[2] |= in[3];
		return ret;
	}

public:				 // specializations
#ifndef ID_SWAP_LITE // avoid dependency avalanche for SPU code
	#define SWAP_VECTOR( x )                                 \
		static void Little( x& c )                           \
		{                                                    \
			LittleArray( c.ToFloatPtr(), c.GetDimension() ); \
		}                                                    \
		static void Big( x& c )                              \
		{                                                    \
			BigArray( c.ToFloatPtr(), c.GetDimension() );    \
		}

	SWAP_VECTOR( idVec2 );
	SWAP_VECTOR( idVec3 );
	SWAP_VECTOR( idVec4 );
	SWAP_VECTOR( idVec5 );
	SWAP_VECTOR( idVec6 );
	SWAP_VECTOR( idMat2 );
	SWAP_VECTOR( idMat3 );
	SWAP_VECTOR( idMat4 );
	SWAP_VECTOR( idMat5 );
	SWAP_VECTOR( idMat6 );
	SWAP_VECTOR( idPlane );
	SWAP_VECTOR( idQuat );
	SWAP_VECTOR( idCQuat );
	SWAP_VECTOR( idAngles );
	SWAP_VECTOR( idBounds );

	/*!
		\brief Converts the byte order of the vertex data in the given draw vertex to little-endian format

		This function performs byte order conversion on all components of the idDrawVert structure to ensure that the data is in little-endian format. It processes the position, texture coordinates,
	   normal, tangent, and color components of the vertex. The conversion affects floating-point and integer values within each component array, making the vertex data compatible with systems that
	   use little-endian byte ordering.

		\param v Reference to the draw vertex whose byte order needs to be converted to little-endian
	*/
	static void Little( idDrawVert& v )
	{
		Little( v.xyz );
		LittleArray( v.st, 2 );
		LittleArray( v.normal, 4 );
		LittleArray( v.tangent, 4 );
		LittleArray( v.color, 4 );
	}

	/*!
		\brief Converts the byte order of the idDrawVert structure's members to big-endian format

		This function takes an idDrawVert reference and converts the byte order of its components to big-endian format. It processes the vertex position, texture coordinates, normal, tangent, and
	   color data by calling appropriate byte-order conversion functions for each component

		\param v Reference to the idDrawVert structure to be converted to big-endian byte order
	*/
	static void Big( idDrawVert& v )
	{
		Big( v.xyz );
		BigArray( v.st, 2 );
		BigArray( v.normal, 4 );
		BigArray( v.tangent, 4 );
		BigArray( v.color, 4 );
	}
#endif
};

/*
================================================
idSwapClass is a template class for performing EndianSwapping.
================================================
*/
template<class classType>
class idSwapClass
{
public:
	/*!
		\brief Default constructs an idSwapClass object.

		This is the default constructor for the idSwapClass template. It initializes the object with default values. In debug builds, it also initializes the size member to zero.

	*/
	idSwapClass()
	{
#ifdef _DEBUG
		size = 0;
#endif
	}

	/*!
		\brief Destructor for the idSwapClass template that performs a debug assertion to verify the size of the class type matches the expected size.

		The destructor for idSwapClass performs a debug assertion to ensure that the size of the templated class type matches the expected size. This is typically used in debugging to catch potential
	   issues with class size changes that might affect memory layout or swapping operations. The assertion is only active in debug builds.

	*/
	~idSwapClass()
	{
#ifdef _DEBUG
		assert( size == sizeof( classType ) );
#endif
	}

	/*!
		\brief Swaps the byte order of the input value to little-endian format

		This function performs a byte order swap on the provided value to convert it to little-endian format. It internally calls idSwap::Little to perform the actual swapping operation. In debug
	   builds, it also increments a size counter by the size of the type being swapped

		\param c Reference to the value to be converted to little-endian byte order
	*/
	template<class type>
	void Little( type& c )
	{
		idSwap::Little( c );
#ifdef _DEBUG
		size += sizeof( type );
#endif
	}

	/*!
		\brief Swaps the byte order of the given value to big-endian format

		This function performs a byte-order swap on the provided value to convert it to big-endian format. It internally calls idSwap::Big to do the actual swapping. In debug builds, it also
	   increments a size counter by the size of the type being swapped

		\param c Reference to the value to be swapped to big-endian format
	*/
	template<class type>
	void Big( type& c )
	{
		idSwap::Big( c );
#ifdef _DEBUG
		size += sizeof( type );
#endif
	}

	/*!
		\brief Converts an array of elements from big-endian to little-endian byte order

		This function converts an array of elements from big-endian to little-endian byte order. It utilizes the idSwap::LittleArray function to perform the actual conversion. In debug builds, it also
	   tracks the total size of converted data for profiling purposes.

		\param c pointer to the array of elements to convert
		\param count number of elements in the array to convert
	*/
	template<class type>
	void LittleArray( type* c, int count )
	{
		idSwap::LittleArray( c, count );
#ifdef _DEBUG
		size += count * sizeof( type );
#endif
	}

	/*!
		\brief Converts an array of elements from little-endian to big-endian byte order in place

		This function performs byte-order conversion on an array of elements, changing their byte representation from little-endian to big-endian format. It operates in-place on the provided array and
	   uses the idSwap::BigArray function for the actual conversion. The function also tracks memory usage in debug builds by incrementing the size counter with the total bytes processed.

		\param c pointer to the array of elements to convert
		\param count number of elements in the array to convert
	*/
	template<class type>
	void BigArray( type* c, int count )
	{
		idSwap::BigArray( c, count );
#ifdef _DEBUG
		size += count * sizeof( type );
#endif
	}

#ifdef _DEBUG
private:
	int size;
#endif
};

#define BIG32( v ) ( ( ( ( uint32 )( v ) ) >> 24 ) | ( ( ( uint32 )( v ) & 0x00FF0000 ) >> 8 ) | ( ( ( uint32 )( v ) & 0x0000FF00 ) << 8 ) | ( ( uint32 )( v ) << 24 ) )
#define BIG16( v ) ( ( ( ( uint16 )( v ) ) >> 8 ) | ( ( uint16 )( v ) << 8 ) )

#endif // !__SWAP_H__
