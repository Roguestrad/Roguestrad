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
#ifndef __LIGHTWEIGHT_COMPRESSION_H__
#define __LIGHTWEIGHT_COMPRESSION_H__

struct lzwCompressionData_t {
	static const int LZW_DICT_BITS = 12;
	static const int LZW_DICT_SIZE = 1 << LZW_DICT_BITS;

	uint8			 dictionaryK[LZW_DICT_SIZE];
	uint16			 dictionaryW[LZW_DICT_SIZE];

	int				 nextCode;
	int				 codeBits;

	int				 codeWord;

	uint64			 tempValue;
	int				 tempBits;
	int				 bytesWritten;
};

/*!
	\class idLZWCompressor
	\brief Provides LZW compression and decompression functionality for binary data.

	Implements a complete LZW compression and decompression system that can encode and decode binary data streams. The class maintains internal state for both compression and decompression operations,
   supporting bit-level operations and dictionary management. It handles buffer management for both input and output streams, with methods for reading and writing data in various formats including raw
   bytes, bits, and typed values. The compressor manages its own internal buffers and provides methods for saving and restoring compression state. The implementation supports overflow detection and
   handles compression dictionary growth with automatic bit width adjustment. The class is designed for efficient binary data processing and can be used in scenarios requiring lossless data
   compression.

*/
class idLZWCompressor
{
public:
	//! Initializes the LZW compressor with the provided compression data.
	idLZWCompressor( lzwCompressionData_t* lzwData_ ) :
		lzwData( lzwData_ )
	{
	}

	static const int LZW_BLOCK_SIZE = ( 1 << 15 );
	static const int LZW_START_BITS = 9;
	static const int LZW_FIRST_CODE = ( 1 << ( LZW_START_BITS - 1 ) );

	//! Initializes the LZW compressor with the provided data buffer and maximum size, optionally appending to existing data.
	void			 Start( uint8* data_, int maxSize, bool append = false );

	//! Reads a specified number of bits from the compressed data stream and returns the value.
	int				 ReadBits( int bits );

	//! Writes a chain of characters from the LZW dictionary to the output block in reverse order and returns the first character of the chain
	int				 WriteChain( int code );

	//! Decompresses a block of data using LZW decompression algorithm
	void			 DecompressBlock();

	//! Writes a specified number of bits from a value into the compressed data stream.
	void			 WriteBits( uint32 value, int bits );

	//! Reads a single byte from the decompressed data block, handling overflow conditions.
	int				 ReadByte( bool ignoreOverflow = false );

	//! Writes a single byte to the LZW compression stream.
	void			 WriteByte( uint8 value );

	//! Looks up a dictionary entry for the given w and k values in the LZW compressor.
	int				 Lookup( int w, int k );

	//! Adds an entry to the LZW compression dictionary and returns the code for the new entry.
	int				 AddToDict( int w, int k );

	//! Increments the code bit length when the maximum code for the current bit length is reached, and clears the hash table if the maximum bit length is exceeded.
	bool			 BumpBits();

	//! Finalizes the LZW compression stream and returns the number of bytes written
	int				 End();

	//! Returns the number of bytes written by the LZW compressor.
	int				 Length() const { return lzwData->bytesWritten; }

	//! Returns the number of bytes read by the decompressor.
	int				 GetReadCount() const { return bytesRead; }

	//! Saves the current state of the LZW compression data.
	void			 Save();

	//! Restores the LZW compression state from saved values.
	void			 Restore();

	//! Checks if the LZW compressor has overflowed
	bool			 IsOverflowed() { return overflowed; }

	//! Writes data to the compressor buffer.
	int				 Write( const void* data, int length )
	{
		uint8* src = ( uint8* )data;

		for( int i = 0; i < length && !IsOverflowed(); i++ ) {
			WriteByte( src[i] );
		}

		return length;
	}

	//! Reads up to length bytes of data into the provided buffer, returning the actual number of bytes read.
	int Read( void* data, int length, bool ignoreOverflow = false )
	{
		uint8* src = ( uint8* )data;

		for( int i = 0; i < length; i++ ) {
			int byte = ReadByte( ignoreOverflow );

			if( byte == -1 ) { return i; }

			src[i] = ( uint8 )byte;
		}

		return length;
	}

	//! Writes data to the compressor in reverse order
	int WriteR( const void* data, int length )
	{
		uint8* src = ( uint8* )data;

		for( int i = 0; i < length && !IsOverflowed(); i++ ) {
			WriteByte( src[length - i - 1] );
		}

		return length;
	}

	//! Reads a specified number of bytes from the compressed data stream and stores them in the provided buffer, returning the actual number of bytes read.
	int ReadR( void* data, int length, bool ignoreOverflow = false )
	{
		uint8* src = ( uint8* )data;

		for( int i = 0; i < length; i++ ) {
			int byte = ReadByte( ignoreOverflow );

			if( byte == -1 ) { return i; }

			src[length - i - 1] = ( uint8 )byte;
		}

		return length;
	}

	//! Writes a value of type type to the compressor buffer.
	template<class type>
	ID_INLINE size_t WriteAgnostic( const type& c )
	{
		return Write( &c, sizeof( c ) );
	}

	//! Reads a value of type from the compressor stream
	template<class type>
	ID_INLINE size_t ReadAgnostic( type& c, bool ignoreOverflow = false )
	{
		size_t r = Read( &c, sizeof( c ), ignoreOverflow );
		return r;
	}

	static const int DICTIONARY_HASH_BITS = 10;
	static const int MAX_DICTIONARY_HASH  = 1 << DICTIONARY_HASH_BITS;
	static const int HASH_MASK			  = MAX_DICTIONARY_HASH - 1;

private:
	//! Initializes the hash table used by the LZW compressor to all 0xFF values.
	void				  ClearHash();

	lzwCompressionData_t* lzwData;
	uint16				  hash[MAX_DICTIONARY_HASH];
	uint16				  nextHash[lzwCompressionData_t::LZW_DICT_SIZE];

	// Used by DecompressBlock
	int					  oldCode;

	uint8*				  data; // Read/write
	int					  maxSize;
	bool				  overflowed;

	// For reading
	int					  bytesRead;
	uint8				  block[LZW_BLOCK_SIZE];
	int					  blockSize;
	int					  blockIndex;

	// saving/restoring when overflow (when writing).
	// Must call End directly after restoring (dictionary is bad so can't keep writing)
	int					  savedBytesWritten;
	int					  savedCodeWord;
	int					  saveCodeBits;
	uint64				  savedTempValue;
	int					  savedTempBits;
};

/*!
	\class idZeroRunLengthCompressor
	\brief A compressor that encodes and decodes data using zero-run length compression.

	This class provides functionality for compressing data by encoding sequences of zero bytes as run-length encoded entries. It supports both compression and decompression operations, allowing data
   to be written to and read from a compressed stream. The compression process is managed through a destination buffer, a compression context, and a maximum size limit. Decompression reads from the
   compressed data stream and handles decoding of zero-run length entries. The class maintains internal state for tracking current position and size of compressed data.

*/
class idZeroRunLengthCompressor
{
public:
	//! Initializes a new instance of the idZeroRunLengthCompressor class.
	idZeroRunLengthCompressor() :
		zeroCount( 0 ),
		destStart( NULL )
	{
	}

	//! Initializes the zero run length compressor with destination buffer, compression context, and maximum size.
	void Start( uint8* dest_, idLZWCompressor* comp_, int maxSize_ );

	//! Writes a run of zero bytes to the compressed data
	bool WriteRun();

	//! Writes a byte value to the compressed data stream, handling zero run-length compression
	bool WriteByte( uint8 value );

	//! Reads a byte value from the compressed data stream, handling zero-run length decoding.
	byte ReadByte();

	//! Reads a specified number of bytes from the compressor into a destination buffer.
	void ReadBytes( byte* dest, int count );

	//! Writes a specified number of bytes from a source buffer to the compressor output.
	void WriteBytes( uint8* src, int count );

	//! Finalizes the compression process and returns the size of the compressed data, or -1 if overflow occurred.
	int	 End();

	//! Returns the size of the compressed data.
	int	 CompressedSize() const { return compressed; }

private:
	//! Reads a single byte from the compressed data stream.
	int				 ReadInternal();

	int				 zeroCount; // Number of pending zeroes
	idLZWCompressor* comp;
	uint8*			 destStart;
	uint8*			 dest;
	int				 compressed; // Compressed size
	int				 maxSize;
};

#endif // __LIGHTWEIGHT_COMPRESSION_H__
