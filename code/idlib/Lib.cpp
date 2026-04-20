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

#include "precompiled.h"
#pragma hdrstop

/*
===============================================================================

	idLib

===============================================================================
*/

idSys*		  idLib::sys				   = NULL;
idCommon*	  idLib::common				   = NULL;
idCVarSystem* idLib::cvarSystem			   = NULL;
idFileSystem* idLib::fileSystem			   = NULL;
int			  idLib::frameNumber		   = 0;
bool		  idLib::mainThreadInitialized = 0;
ID_TLS		  idLib::isMainThread		   = 0;

char		  idException::error[2048];

void		  idLib::Init()
{
	assert( sizeof( bool ) == 1 );

	isMainThread		  = 1;
	mainThreadInitialized = 1; // note that the thread-local isMainThread is now valid

	// initialize little/big endian conversion
	Swap_Init();

	// init string memory allocator
	idStr::InitMemory();

	// initialize generic SIMD implementation
	idSIMD::Init();

	// initialize math
	idMath::Init();

	// test idMatX
	// idMatX::Test();

	// test idPolynomial
#ifdef _DEBUG
	idPolynomial::Test();
#endif

	// initialize the dictionary string pools
	idDict::Init();
}

void idLib::ShutDown()
{
	// shut down the dictionary string pools
	idDict::Shutdown();

	// shut down the string memory allocator
	idStr::ShutdownMemory();

	// shut down the SIMD engine
	idSIMD::Shutdown();
}

/*
===============================================================================

	Colors

===============================================================================
*/

// Color definitions (alphabetical order)
const idVec4 colorAqua			 = idVec4( 0.00f, 1.00f, 1.00f, 1.00f ); // #00FFFF
const idVec4 colorBlack			 = idVec4( 0.00f, 0.00f, 0.00f, 1.00f ); // #000000
const idVec4 colorBlue			 = idVec4( 0.00f, 0.00f, 1.00f, 1.00f ); // #0000FF
const idVec4 colorBrown			 = idVec4( 0.40f, 0.35f, 0.08f, 1.00f );
const idVec4 colorBurlyWood		 = idVec4( 0.87f, 0.72f, 0.53f, 1.00f ); // #DEB887
const idVec4 colorCoral			 = idVec4( 1.00f, 0.50f, 0.31f, 1.00f ); // #FF7F50
const idVec4 colorCrimson		 = idVec4( 0.86f, 0.08f, 0.24f, 1.00f ); // #DC143C
const idVec4 colorCyan			 = idVec4( 0.00f, 1.00f, 1.00f, 1.00f );
const idVec4 colorDarkBlue		 = idVec4( 0.00f, 0.00f, 0.55f, 1.00f ); // #00008B
const idVec4 colorDarkCyan		 = idVec4( 0.00f, 0.55f, 0.55f, 1.00f ); // #008B8B
const idVec4 colorDarkGoldenRod	 = idVec4( 0.72f, 0.53f, 0.04f, 1.00f ); // #B8860B
const idVec4 colorDarkKhaki		 = idVec4( 0.74f, 0.72f, 0.42f, 1.00f ); // #BDB76B
const idVec4 colorDarkSalmon	 = idVec4( 0.91f, 0.59f, 0.48f, 1.00f ); // #E9967A
const idVec4 colorDarkSlateGray	 = idVec4( 0.18f, 0.31f, 0.31f, 1.00f ); // #2F4F4F
const idVec4 colorDkGrey		 = idVec4( 0.25f, 0.25f, 0.25f, 1.00f );
const idVec4 colorDodgerBlue	 = idVec4( 0.12f, 0.56f, 1.00f, 1.00f ); // #1E90FF
const idVec4 colorFuchsia		 = idVec4( 1.00f, 0.00f, 1.00f, 1.00f ); // #FF00FF
const idVec4 colorGold			 = idVec4( 0.68f, 0.63f, 0.36f, 1.00f );
const idVec4 colorGray			 = idVec4( 0.50f, 0.50f, 0.50f, 1.00f ); // #808080
const idVec4 colorGreen			 = idVec4( 0.00f, 0.50f, 0.00f, 1.00f ); // #008000
const idVec4 colorLightSeaGreen	 = idVec4( 0.13f, 0.70f, 0.67f, 1.00f ); // #20B2AA
const idVec4 colorLightSteelBlue = idVec4( 0.69f, 0.77f, 0.87f, 1.00f ); // #B0C4DE
const idVec4 colorLtGrey		 = idVec4( 0.75f, 0.75f, 0.75f, 1.00f );
const idVec4 colorLime			 = idVec4( 0.00f, 1.00f, 0.00f, 1.00f ); // #00FF00
const idVec4 colorMagenta		 = idVec4( 1.00f, 0.00f, 1.00f, 1.00f );
const idVec4 colorMaroon		 = idVec4( 0.50f, 0.00f, 0.00f, 1.00f ); // #800000
const idVec4 colorMdGrey		 = idVec4( 0.50f, 0.50f, 0.50f, 1.00f );
const idVec4 colorNavy			 = idVec4( 0.00f, 0.00f, 0.50f, 1.00f ); // #000080
const idVec4 colorOlive			 = idVec4( 0.50f, 0.50f, 0.00f, 1.00f ); // #808000
const idVec4 colorOrange		 = idVec4( 1.00f, 0.50f, 0.00f, 1.00f );
const idVec4 colorPink			 = idVec4( 0.73f, 0.40f, 0.48f, 1.00f );
const idVec4 colorPurple		 = idVec4( 0.60f, 0.00f, 0.60f, 1.00f );
const idVec4 colorRed			 = idVec4( 1.00f, 0.00f, 0.00f, 1.00f );
const idVec4 colorSilver		 = idVec4( 0.75f, 0.75f, 0.75f, 1.00f ); // #C0C0C0
const idVec4 colorTeal			 = idVec4( 0.00f, 0.50f, 0.50f, 1.00f ); // #008080
const idVec4 colorWhite			 = idVec4( 1.00f, 1.00f, 1.00f, 1.00f );
const idVec4 colorYellow		 = idVec4( 1.00f, 1.00f, 0.00f, 1.00f );

dword		 PackColor( const idVec4& color )
{
	byte dx = idMath::Ftob( color.x * 255.0f );
	byte dy = idMath::Ftob( color.y * 255.0f );
	byte dz = idMath::Ftob( color.z * 255.0f );
	byte dw = idMath::Ftob( color.w * 255.0f );
	return ( dx << 0 ) | ( dy << 8 ) | ( dz << 16 ) | ( dw << 24 );
}

void UnpackColor( const dword color, idVec4& unpackedColor )
{
	unpackedColor.Set(
		( ( color >> 0 ) & 255 ) * ( 1.0f / 255.0f ), ( ( color >> 8 ) & 255 ) * ( 1.0f / 255.0f ), ( ( color >> 16 ) & 255 ) * ( 1.0f / 255.0f ), ( ( color >> 24 ) & 255 ) * ( 1.0f / 255.0f ) );
}

dword PackColor( const idVec3& color )
{
	byte dx = idMath::Ftob( color.x * 255.0f );
	byte dy = idMath::Ftob( color.y * 255.0f );
	byte dz = idMath::Ftob( color.z * 255.0f );
	return ( dx << 0 ) | ( dy << 8 ) | ( dz << 16 );
}

void UnpackColor( const dword color, idVec3& unpackedColor )
{
	unpackedColor.Set( ( ( color >> 0 ) & 255 ) * ( 1.0f / 255.0f ), ( ( color >> 8 ) & 255 ) * ( 1.0f / 255.0f ), ( ( color >> 16 ) & 255 ) * ( 1.0f / 255.0f ) );
}

void idLib::FatalError( const char* fmt, ... )
{
	va_list argptr;
	char	text[MAX_STRING_CHARS];

	va_start( argptr, fmt );
	idStr::vsnPrintf( text, sizeof( text ), fmt, argptr );
	va_end( argptr );

	common->FatalError( "%s", text );

#if !defined( _WIN32 )
	// SRS - Added exit to silence build warning since FatalError has attribute noreturn
	exit( EXIT_FAILURE );
#endif
}

void idLib::Error( const char* fmt, ... )
{
	va_list argptr;
	char	text[MAX_STRING_CHARS];

	va_start( argptr, fmt );
	idStr::vsnPrintf( text, sizeof( text ), fmt, argptr );
	va_end( argptr );

	common->Error( "%s", text );

#if !defined( _WIN32 )
	// SRS - Added exit to silence build warning since FatalError has attribute noreturn
	exit( EXIT_FAILURE );
#endif
}

void idLib::Warning( const char* fmt, ... )
{
	va_list argptr;
	char	text[MAX_STRING_CHARS];

	va_start( argptr, fmt );
	idStr::vsnPrintf( text, sizeof( text ), fmt, argptr );
	va_end( argptr );

	common->Warning( "%s", text );
}

void idLib::WarningIf( const bool test, const char* fmt, ... )
{
	if( !test ) {
		return;
	}

	va_list argptr;
	char	text[MAX_STRING_CHARS];

	va_start( argptr, fmt );
	idStr::vsnPrintf( text, sizeof( text ), fmt, argptr );
	va_end( argptr );

	common->Warning( "%s", text );
}

void idLib::Printf( const char* fmt, ... )
{
	va_list argptr;
	va_start( argptr, fmt );
	if( common ) {
		common->VPrintf( fmt, argptr );
	}
	va_end( argptr );
}

void idLib::PrintfIf( const bool test, const char* fmt, ... )
{
	if( !test ) {
		return;
	}

	va_list argptr;
	va_start( argptr, fmt );
	common->VPrintf( fmt, argptr );
	va_end( argptr );
}

/*
===============================================================================

	Byte order functions

===============================================================================
*/

// can't just use function pointers, or dll linkage can mess up
static short ( *_BigShort )( short l );
static short ( *_LittleShort )( short l );
static int ( *_BigLong )( int l );
static int ( *_LittleLong )( int l );
static float ( *_BigFloat )( float l );
static float ( *_LittleFloat )( float l );
static void ( *_BigRevBytes )( void* bp, int elsize, int elcount );
static void ( *_LittleRevBytes )( void* bp, int elsize, int elcount );
static void ( *_LittleBitField )( void* bp, int elsize );
static void ( *_SixtetsForInt )( byte* out, int src );
static int ( *_IntForSixtets )( byte* in );

short BigShort( short l )
{
	return _BigShort( l );
}
short LittleShort( short l )
{
	return _LittleShort( l );
}
int BigLong( int l )
{
	return _BigLong( l );
}
int LittleLong( int l )
{
	return _LittleLong( l );
}
float BigFloat( float l )
{
	return _BigFloat( l );
}
float LittleFloat( float l )
{
	return _LittleFloat( l );
}
void BigRevBytes( void* bp, int elsize, int elcount )
{
	_BigRevBytes( bp, elsize, elcount );
}
void LittleRevBytes( void* bp, int elsize, int elcount )
{
	_LittleRevBytes( bp, elsize, elcount );
}
void LittleBitField( void* bp, int elsize )
{
	_LittleBitField( bp, elsize );
}

void SixtetsForInt( byte* out, int src )
{
	_SixtetsForInt( out, src );
}
int IntForSixtets( byte* in )
{
	return _IntForSixtets( in );
}

/*!
	\brief Swaps the byte order of a 16-bit short integer.

	This function takes a 16-bit short integer and reverses the order of its two bytes. It is commonly used to convert between different byte order formats, such as when reading data from files or
   network streams that may have a different endianness than the system. The function extracts the least significant byte and the most significant byte, then recombines them in reverse order.

	\param l The 16-bit short integer to swap
	\return The 16-bit short integer with its bytes swapped
*/
short ShortSwap( short l )
{
	byte b1, b2;

	b1 = l & 255;
	b2 = ( l >> 8 ) & 255;

	return ( b1 << 8 ) + b2;
}

/*!
	\brief Returns the input short value without performing any byte swapping operation.

	This function takes a short integer as input and directly returns it without any modification or byte order conversion. It is typically used as a placeholder or identity function in scenarios
   where byte swapping might otherwise be applied.

	\param l Input short integer value to be returned as-is
	\return The input short integer value unchanged
*/
short ShortNoSwap( short l )
{
	return l;
}

/*!
	\brief Swaps the byte order of a 32-bit integer.

	This function takes a 32-bit integer and reverses the order of its bytes. It is commonly used for converting between different endianness formats, such as when reading data from a file or network
   stream that has a different byte order than the local system. The implementation extracts each byte of the input integer, then reassembles them in reverse order to produce the swapped result.

	\param l The 32-bit integer value to be byte-swapped
	\return The byte-swapped 32-bit integer value
*/
int LongSwap( int l )
{
	byte b1, b2, b3, b4;

	b1 = l & 255;
	b2 = ( l >> 8 ) & 255;
	b3 = ( l >> 16 ) & 255;
	b4 = ( l >> 24 ) & 255;

	return ( ( int )b1 << 24 ) + ( ( int )b2 << 16 ) + ( ( int )b3 << 8 ) + b4;
}

/*!
	\brief Returns the input integer value without performing any byte swapping operation.

	This function serves as a no-operation implementation for swapping bytes in an integer value. It is typically used in code that handles data serialization or network byte order conversions, where
   some paths may require byte swapping while others do not. This particular implementation simply returns the input value unchanged, effectively acting as an identity function for integer values.

	\param l The integer value to be returned without modification
	\return The input integer value passed to the function, unchanged
*/
int LongNoSwap( int l )
{
	return l;
}

/*!
	\brief Swaps the byte order of a floating-point number.

	This function performs a byte-order swap on a floating-point number by reinterpreting its bits as a sequence of bytes and then reversing their order. It is commonly used for handling endianness
   conversions when transferring binary data between systems with different byte order conventions.

	\param f The floating-point value whose byte order will be swapped
	\return The floating-point value with its byte order reversed
*/
float FloatSwap( float f )
{
	union {
		float f;
		byte  b[4];
	} dat1, dat2;

	dat1.f	  = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}

/*!
	\brief Returns the input float value without performing any swapping operation.

	This function serves as a placeholder or helper that simply returns the input float value unchanged. It is typically used in contexts where a function pointer or method signature requires a swap
   function, but no actual swapping is needed. The function is a no-op implementation that preserves the original value.

	\param f Input floating point value to be returned unchanged
	\return The same float value that was passed as input
*/
float FloatNoSwap( float f )
{
	return f;
}

/*!
	\brief Swaps the byte order in each element of an array of elements.

	This function reverses the byte order for each element in the provided array. For elements of size 2, it uses a specialized XOR-based swapping algorithm. For other sizes, it swaps bytes from both
   ends of each element towards the center. The function modifies the input buffer in place.

	\param bp pointer to the buffer containing the elements to swap
	\param elsize size of each element in bytes
	\param elcount number of elements in the buffer
*/
void RevBytesSwap( void* bp, int elsize, int elcount )
{
	unsigned char *p, *q;

	p = ( unsigned char* )bp;

	if( elsize == 2 ) {
		q = p + 1;
		while( elcount-- ) {
			*p ^= *q;
			*q ^= *p;
			*p ^= *q;
			p += 2;
			q += 2;
		}
		return;
	}

	while( elcount-- ) {
		q = p + elsize - 1;
		while( p < q ) {
			*p ^= *q;
			*q ^= *p;
			*p ^= *q;
			++p;
			--q;
		}
		p += elsize >> 1;
	}
}

/*!
	\brief Reverses the bitfield of the specified size in the provided buffer.

	This function performs two main operations on the provided buffer. First, it reverses the byte order of the data using LittleRevBytes. Then, it reverses the individual bits within each byte of the
   buffer. The elsize parameter specifies the number of bytes to process. The function modifies the buffer in-place.

	\param bp Pointer to the buffer containing the data to be bitfield reversed
	\param elsize Number of bytes in the buffer to process
*/
void RevBitFieldSwap( void* bp, int elsize )
{
	int			   i;
	unsigned char *p, t, v;

	LittleRevBytes( bp, elsize, 1 );

	p = ( unsigned char* )bp;
	while( elsize-- ) {
		v = *p;
		t = 0;
		for( i = 7; i >= 0; i-- ) {
			t <<= 1;
			v >>= 1;
			t |= v & 1;
		}
		*p++ = t;
	}
}

/*!
	\brief Does nothing, placeholder function.

	This function is a placeholder that performs no operations. It takes a pointer to memory, an element size, and a count of elements, but does not modify or process the data. The function is likely
   intended as a stub or placeholder for a more complex implementation that may be added later.

	\param bp Pointer to the memory block to process
	\param elsize Size of each element in bytes
	\param elcount Number of elements to process
*/
void RevBytesNoSwap( void* bp, int elsize, int elcount )
{
	return;
}

/*!
	\brief Reverses the bit field in place without swapping bytes

	This function is intended to reverse the bit field of data in place but currently does nothing. The implementation is empty and serves as a placeholder or stub. The function takes a pointer to
   data and the size of each element to be reversed.

	\param bp Pointer to the data to be reversed
	\param elsize Size of each element in bytes
*/
void RevBitFieldNoSwap( void* bp, int elsize )
{
	return;
}

/*!
	\brief Encodes an integer into four six-bit values stored in little-endian byte order

	This function takes a 32-bit integer and splits it into four 6-bit values, which are then stored in the output buffer in little-endian byte order. Each 6-bit value represents a portion of the
   original integer's bits, with bit manipulation used to extract and reorganize the bits accordingly. This encoding is commonly used for base64-like compression or transmission of integer values.

	\param out Buffer to store the four 6-bit encoded values
	\param src The 32-bit integer to encode
*/
void SixtetsForIntLittle( byte* out, int src )
{
	byte* b = ( byte* )&src;
	out[0]	= ( b[0] & 0xfc ) >> 2;
	out[1]	= ( ( b[0] & 0x3 ) << 4 ) + ( ( b[1] & 0xf0 ) >> 4 );
	out[2]	= ( ( b[1] & 0xf ) << 2 ) + ( ( b[2] & 0xc0 ) >> 6 );
	out[3]	= b[2] & 0x3f;
}

/*!
	\brief Converts a 32-bit integer into four 6-bit values stored in a byte array

	This function takes a 32-bit integer and decomposes it into four 6-bit components, storing each component in the output byte array. The decomposition is performed by extracting the least
   significant 6 bits in each iteration and right-shifting the source integer by 6 bits. This is typically used in base64 encoding operations where data needs to be split into 6-bit chunks.

	\param out Output array of 4 bytes to store the 6-bit components
	\param src Source 32-bit integer to decompose into 6-bit values
*/
void SixtetsForIntBig( byte* out, int src )
{
	for( int i = 0; i < 4; i++ ) {
		out[i] = src & 0x3f;
		src >>= 6;
	}
}

/*!
	\brief Converts a sequence of four bytes representing six-bit values into a 32-bit integer.

	This function takes an input byte buffer containing four bytes that represent six-bit values and combines them into a single 32-bit integer. The bit manipulation is performed to reconstruct the
   integer by properly shifting and combining the bits from each input byte. The input is interpreted as a sequence of six-bit values packed together, with the first byte contributing the lower 6 bits
   of the result, and so on.

	\param in Pointer to a byte buffer containing four bytes that represent packed six-bit values.
	\return A 32-bit integer constructed from the six-bit values in the input buffer.
*/
int IntForSixtetsLittle( byte* in )
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

/*!
	\brief Converts four bytes into a 24-bit integer using big-endian six-bit packing.

	This function takes a pointer to four bytes and combines them into a single 24-bit integer. Each byte contributes 6 bits to the result, with the first byte providing the least significant 6 bits
   and the fourth byte providing the most significant 6 bits. The function performs bit manipulation to properly align and combine the six-bit values into the final integer.

	\param in Pointer to an array of four bytes to be converted into a 24-bit integer
	\return A 24-bit integer value formed by combining the four input bytes, with each byte contributing 6 bits in big-endian order.
*/
int IntForSixtetsBig( byte* in )
{
	int ret = 0;
	ret |= in[0];
	ret |= in[1] << 6;
	ret |= in[2] << 2 * 6;
	ret |= in[3] << 3 * 6;
	return ret;
}

void Swap_Init()
{
	byte swaptest[2] = { 1, 0 };

	// set the byte swapping variables in a portable manner
	if( *( short* )swaptest == 1 ) {
		// little endian ex: x86
		_BigShort		= ShortSwap;
		_LittleShort	= ShortNoSwap;
		_BigLong		= LongSwap;
		_LittleLong		= LongNoSwap;
		_BigFloat		= FloatSwap;
		_LittleFloat	= FloatNoSwap;
		_BigRevBytes	= RevBytesSwap;
		_LittleRevBytes = RevBytesNoSwap;
		_LittleBitField = RevBitFieldNoSwap;
		_SixtetsForInt	= SixtetsForIntLittle;
		_IntForSixtets	= IntForSixtetsLittle;
	} else {
		// big endian ex: ppc
		_BigShort		= ShortNoSwap;
		_LittleShort	= ShortSwap;
		_BigLong		= LongNoSwap;
		_LittleLong		= LongSwap;
		_BigFloat		= FloatNoSwap;
		_LittleFloat	= FloatSwap;
		_BigRevBytes	= RevBytesNoSwap;
		_LittleRevBytes = RevBytesSwap;
		_LittleBitField = RevBitFieldSwap;
		_SixtetsForInt	= SixtetsForIntBig;
		_IntForSixtets	= IntForSixtetsBig;
	}
}

bool Swap_IsBigEndian()
{
	byte swaptest[2] = { 1, 0 };
	return *( short* )swaptest != 1;
}

void BreakOnListGrowth()
{
}

void BreakOnListDefault()
{
}
