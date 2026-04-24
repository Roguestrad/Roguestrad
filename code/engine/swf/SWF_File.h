/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2025 Robert Beckebans

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
#ifndef __SWF_FILE_H__
#define __SWF_FILE_H__

/*!
	\class idFile_SWF
	\brief A wrapper class for writing SWF file data with bit-level and type-specific write operations.

	This class provides a specialized interface for writing data to SWF files, offering both low-level byte manipulation and high-level type-specific write functions. It wraps an existing file pointer
   and exposes it through standard operator overloading for seamless integration with code expecting raw file pointers. The class supports writing various SWF-specific data types including rectangles,
   matrices, colors, and strings, along with utility functions for calculating bit requirements. It also includes functionality for managing bit-level alignment and writing SWF tag headers, making it
   suitable for generating or modifying SWF file content programmatically.

*/
class idFile_SWF // : public idFile
{
public:
	//! Constructor that accepts and stores the file pointer.
	idFile_SWF( idFile* _file ) :
		file( _file )
	{
		bitPos = 0;
		NBits  = 0;
	}

	//! Destructor that will destroy (close) the file when this wrapper class goes out of scope.
	~idFile_SWF();

	//! Converts the idFile_SWF object to a raw file pointer.
	operator idFile*() const { return file; }

	//! Returns the wrapped file pointer for member access.
	idFile* operator->() const { return file; }

	//! Aligns the bit position to the next byte boundary in the SWF file.
	void	ByteAlign()
	{
		if( bitPos > 0 ) {
			WriteByte( NBits );

			bitPos = 0;
			NBits  = 0;
		}
	}

	//! Calculates the number of bits required to represent a signed or unsigned 64-bit integer value.
	static int	 BitCountS( const int64 value, bool isSigned );

	//! Calculates the number of bits required to represent the given unsigned integer value.
	static int	 BitCountU( const int value );

	//! Returns the number of bits required to represent the integer part of a float value.
	static int	 BitCountFloat( const float value );

	//! Determines the minimum number of bits required to represent a signed integer value, considering a given number of bits.
	static int	 EnlargeBitCountS( const int value, int numBits );

	//! Returns the number of bits needed to represent the given value, ensuring it is at least the specified number of bits.
	static int	 EnlargeBitCountU( const int value, int numBits );

	//! Writes data to the file and ensures byte alignment.
	virtual int	 Write( const void* buffer, int len );

	//! Writes a specified number of bits from an integer value to a SWF file
	void		 WriteUBits( int value, int numBits );

	//! Writes a signed integer value using a specified number of bits to the SWF file.
	void		 WriteSBits( int value, int numBits );

	//! Writes an 8-bit unsigned integer value to the SWF file.
	void		 WriteU8( uint8 value );

	//! Writes a 16-bit unsigned integer value to the file in little-endian byte order
	void		 WriteU16( uint16 value );

	//! Writes a 32-bit unsigned integer value to the SWF file in little-endian byte order
	void		 WriteU32( uint32 value );

	//! Writes a rectangle structure to the SWF file using bit manipulation.
	void		 WriteRect( const swfRect_t& rect );

	//! Writes a SWF matrix to the file with optimized bit packing.
	void		 WriteMatrix( const swfMatrix_t& matrix );

	//! Writes the red, green, and blue components of a color to the file.
	void		 WriteColorRGB( const swfColorRGB_t& color );

	//! Writes a color value including alpha component to the SWF file.
	void		 WriteColorRGBA( const swfColorRGBA_t& color );

	//! Writes a color transformation RGBA to the SWF file
	void		 WriteColorXFormRGBA( const swfColorXform_t& xcf );

	//! Writes a null-terminated string to the SWF file, handling null input by writing a null terminator.
	void		 WriteString( const char* str );

	//! Calculates the size of a SWF tag header based on the tag type and length.
	static int32 GetTagHeaderSize( swfTag_t tag, int32 tagLength );

	//! Writes a SWF tag header with the specified tag and length to the file.
	void		 WriteTagHeader( swfTag_t tag, int32 tagLength );

private:
	//! Writes a single byte to the SWF file.
	void	WriteByte( byte bits );

	idFile* file; // The managed file pointer.

	int		bitPos;
	int		NBits;
};

#endif // !__SWF_FILE_H__
