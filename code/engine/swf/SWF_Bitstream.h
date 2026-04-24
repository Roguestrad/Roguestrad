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
#ifndef __SWF_BITSTREAM_H__
#define __SWF_BITSTREAM_H__

/*!
	\class idSWFBitStream
	\brief A bit stream class for reading SWF data.

	The idSWFBitStream class provides functionality for reading SWF (Small Web Format) data from byte buffers. It supports various data types including integers, booleans, strings, and complex
   structures like rectangles, matrices, and color transformations. The class can be initialized with either copy or reference semantics, allowing for flexible memory management. It maintains an
   internal read pointer that can be manipulated through seek and rewind operations. The class includes optimized inline methods for reading primitive types and specialized methods for reading
   SWF-specific structures. Bit-level reading operations are supported for signed and unsigned integers with configurable bit counts. The implementation handles endianness conversion for numeric
   values and provides methods for reading variable-length encoded integers. The class also supports reading of color data in various formats including RGB, RGBA, and color transformation data.

*/
class idSWFBitStream
{
public:
	//! Constructs an empty idSWFBitStream instance.
	idSWFBitStream();

	//! Constructs an idSWFBitStream object by loading data from a byte array.
	idSWFBitStream( const byte* data, uint32 len, bool copy )
	{
		free = false;
		Load( data, len, copy );
	}
	~idSWFBitStream() { Free(); }

	//! Assigns the contents of another idSWFBitStream instance to this instance
	idSWFBitStream& operator=( idSWFBitStream& other );

	//! Moves the contents of another SWF bit stream instance to this instance.
	idSWFBitStream& operator=( idSWFBitStream&& other );

	//! Initializes the bit stream with data from a byte buffer, optionally copying the data.
	void			Load( const byte* data, uint32 len, bool copy );

	//! Frees the memory allocated for the SWF bitstream data
	void			Free();

	//! Returns a pointer to the current position in the bit stream.
	const byte*		Ptr() { return startp; }

	//! Returns a pointer to the current position in the bit stream.
	const byte*		Ptr() const { return startp; }

	//! Returns the length of the bit stream in bytes.
	uint32			Length() const { return ( uint32 )( endp - startp ); }

	//! Returns the current read position in the bit stream.
	uint32			Tell() const { return ( uint32 )( readp - startp ); }

	//! Moves the read position by the specified offset bytes.
	void			Seek( int32 offset ) { readp += offset; }

	//! Resets the read pointer of the SWF bit stream to the beginning.
	void			Rewind() { readp = startp; }

	//! Resets the bit stream to its initial state.
	void			ResetBits();

	//! Reads a signed integer value from the bit stream with the specified number of bits
	int				ReadS( unsigned int numBits );

	//! Reads an unsigned integer value from the bit stream with the specified number of bits.
	unsigned int	ReadU( unsigned int numBits );

	//! Reads a single boolean value from the bit stream
	bool			ReadBool();

	//! Reads a block of data from the bit stream and returns a pointer to the beginning of that data.
	const byte*		ReadData( int size );

	//! Reads a little-endian value from the bit stream and converts it to the native endianness.
	template<typename T>
	void		ReadLittle( T& val );

	//! Reads and returns a single unsigned 8-bit integer from the bit stream.
	uint8		ReadU8();

	//! Reads a 16-bit unsigned integer from the bit stream
	uint16		ReadU16();

	//! Reads a 32-bit unsigned integer from the bit stream.
	uint32		ReadU32();

	//! Reads a signed 16-bit integer from the bit stream.
	int16		ReadS16();

	//! Reads a 32-bit signed integer from the bit stream.
	int32		ReadS32();

	//! Reads a 32-bit unsigned integer from the bit stream using variable-length encoding.
	uint32		ReadEncodedU32();

	//! Reads a fixed 8 value from the bit stream
	float		ReadFixed8();

	//! Reads a 16-bit fixed point value from the bit stream and returns it as a float.
	float		ReadFixed16();

	//! Reads a floating-point value from the bit stream.
	float		ReadFloat();

	//! Reads and returns a double value from the bit stream
	double		ReadDouble();

	//! Reads a null-terminated string from the bit stream.
	const char* ReadString();

	//! Reads a rectangle from the bit stream and stores it in the provided rect structure.
	void		ReadRect( swfRect_t& rect );

	//! Reads a SWF matrix transformation from the bit stream and stores it in the provided matrix structure.
	void		ReadMatrix( swfMatrix_t& matrix );

	//! Reads color transformation data including multiplication and addition terms from the bit stream and stores it in the provided color transform structure.
	void		ReadColorXFormRGBA( swfColorXform_t& cxf );

	//! Reads RGB color values from the bit stream into the provided color structure.
	void		ReadColorRGB( swfColorRGB_t& color );

	//! Reads RGBA color values from the bit stream into the provided color structure
	void		ReadColorRGBA( swfColorRGBA_t& color );

	//! Reads gradient data from the bit stream into the provided gradient structure.
	void		ReadGradient( swfGradient_t& grad, bool rgba );

	//! Reads morph gradient data from the bit stream into the provided gradient structure
	void		ReadMorphGradient( swfGradient_t& grad );

private:
	bool		 free;

	const byte*	 startp;
	const byte*	 endp;
	const byte*	 readp;

	uint64		 currentBit;
	uint64		 currentByte;

	//! Reads a signed integer from a bit stream with specified number of bits.
	int			 ReadInternalS( uint64& regCurrentBit, uint64& regCurrentByte, unsigned int numBits );

	//! Reads a specified number of unsigned bits from the bit stream
	unsigned int ReadInternalU( uint64& regCurrentBit, uint64& regCurrentByte, unsigned int numBits );
};

ID_INLINE void idSWFBitStream::ResetBits()
{
	currentBit	= 0;
	currentByte = 0;
}

template<typename T>
void idSWFBitStream::ReadLittle( T& val )
{
	val = *( T* )ReadData( sizeof( val ) );
	idSwap::Little( val );
}

ID_INLINE bool idSWFBitStream::ReadBool()
{
	return ( ReadU( 1 ) != 0 );
}
ID_INLINE uint8 idSWFBitStream::ReadU8()
{
	ResetBits();
	return *readp++;
}
ID_INLINE uint16 idSWFBitStream::ReadU16()
{
	ResetBits();
	readp += 2;
	return ( readp[-2] | ( readp[-1] << 8 ) );
}
ID_INLINE uint32 idSWFBitStream::ReadU32()
{
	ResetBits();
	readp += 4;
	return ( readp[-4] | ( readp[-3] << 8 ) | ( readp[-2] << 16 ) | ( readp[-1] << 24 ) );
}
ID_INLINE int16 idSWFBitStream::ReadS16()
{
	ResetBits();
	readp += 2;
	return ( readp[-2] | ( readp[-1] << 8 ) );
}
ID_INLINE int32 idSWFBitStream::ReadS32()
{
	ResetBits();
	readp += 4;
	return ( readp[-4] | ( readp[-3] << 8 ) | ( readp[-2] << 16 ) | ( readp[-1] << 24 ) );
}
ID_INLINE float idSWFBitStream::ReadFixed8()
{
	ResetBits();
	readp += 2;
	return SWFFIXED8( ( readp[-2] | ( readp[-1] << 8 ) ) );
}
ID_INLINE float idSWFBitStream::ReadFixed16()
{
	ResetBits();
	readp += 4;
	return SWFFIXED16( ( readp[-4] | ( readp[-3] << 8 ) | ( readp[-2] << 16 ) | ( readp[-1] << 24 ) ) );
}
ID_INLINE float idSWFBitStream::ReadFloat()
{
	ResetBits();
	readp += 4;
	uint32 i = ( readp[-4] | ( readp[-3] << 8 ) | ( readp[-2] << 16 ) | ( readp[-1] << 24 ) );
	return ( float& )i;
}

ID_INLINE double idSWFBitStream::ReadDouble()
{
	const byte* swfIsRetarded = ReadData( 8 );
	byte		buffer[8];
	buffer[0] = swfIsRetarded[4];
	buffer[1] = swfIsRetarded[5];
	buffer[2] = swfIsRetarded[6];
	buffer[3] = swfIsRetarded[7];
	buffer[4] = swfIsRetarded[0];
	buffer[5] = swfIsRetarded[1];
	buffer[6] = swfIsRetarded[2];
	buffer[7] = swfIsRetarded[3];
	double d  = *( double* )buffer;
	idSwap::Little( d );
	return d;
}

#endif // !__SWF_BITSTREAM_H__
