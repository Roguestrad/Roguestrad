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
#ifndef __BITMSG_H__
#define __BITMSG_H__

/*!
	\class idBitMsg
	\brief idBitMsg provides bit-level serialization and deserialization for binary data with support for various data types and compression techniques.

	idBitMsg operates on a sequence of individual bits and handles byte ordering while avoiding alignment errors. It allows concurrent writing and reading operations on the same message instance. The
   class supports both writing and reading data, with methods for various data types including integers, floats, vectors, and strings. It provides functionality for delta encoding, quantization, and
   normalized floating-point values to optimize data transmission. The data set with Init is never freed, meaning the caller is responsible for managing the lifetime of the buffer. Methods are
   available to check for buffer overflows, manage read/write positions, and save/restore state for both reading and writing operations. The class supports alignment to byte boundaries and can handle
   network address structures. It includes specialized methods for writing and reading direction vectors using compact bit representations. The hasChanged flag can be used to track modifications to
   the message since the last reset.

*/
class idBitMsg
{
public:
	//! Initializes a new instance of idBitMsg for writing with no initial buffer.
	idBitMsg() { InitWrite( NULL, 0 ); }

	//! Initializes a bit message for writing with the provided data buffer and length.
	idBitMsg( byte* data, int length ) { InitWrite( data, length ); }

	//! Initializes a bit message for reading from the provided data buffer and length.
	idBitMsg( const byte* data, int length ) { InitRead( data, length ); }

	//! Initializes the message buffer for writing data.
	void		InitWrite( byte* data, int length );

	//! Initializes the message object for reading data from a byte array.
	void		InitRead( const byte* data, int length );

	//! Returns the write data buffer for the message
	byte*		GetWriteData();

	//! Returns a pointer to the data used for reading in the bit message.
	const byte* GetReadData() const;

	//! Returns the maximum size of the message.
	int			GetMaxSize() const;

	//! Sets whether the message allows overflow.
	void		SetAllowOverflow( bool set );

	//! Returns true if the message was overflowed
	bool		IsOverflowed() const;

	//! Returns the size of the message in bytes.
	int			GetSize() const;

	//! Sets the size of the message
	void		SetSize( int size );

	//! Returns the current write bit position in the bit message.
	int			GetWriteBit() const;

	//! Sets the current write bit position for the bit message.
	void		SetWriteBit( int bit );

	//! Returns the total number of bits written to the bit message.
	int			GetNumBitsWritten() const;

	//! Returns the remaining space available for writing in the message buffer.
	int			GetRemainingSpace() const;

	//! Returns the number of bits remaining for writing in the message buffer.
	int			GetRemainingWriteBits() const;

	//------------------------
	// Write State
	//------------------------

	//! Saves the current write state of the bit message.
	void		SaveWriteState( int& s, int& b, uint64& t ) const;

	//! Restores the write state of the bit message to the specified size, bit position, and temporary value.
	void		RestoreWriteState( int s, int b, uint64 t );

	//------------------------
	// Reading
	//------------------------

	//! Returns the number of bytes read from the message so far.
	int			GetReadCount() const;

	//! Sets the number of bytes read from the message.
	void		SetReadCount( int bytes );

	//! Returns the current read bit position in the message
	int			GetReadBit() const;

	//! Sets the current read bit position for the bit message.
	void		SetReadBit( int bit );

	//! Returns the number of bits read from the message.
	int			GetNumBitsRead() const;

	//! Returns the number of bytes left to read from the message
	int			GetRemainingData() const;

	//! Returns the number of bits remaining to be read from the message.
	int			GetRemainingReadBits() const;

	//! Saves the current read state of the bit message.
	void		SaveReadState( int& c, int& b ) const;

	//! Restores the read state of the bit message using the saved count and bit values.
	void		RestoreReadState( int c, int b );

	//------------------------
	// Writing
	//------------------------

	//! Initializes the message for writing by resetting internal state variables.
	void		BeginWriting();

	//! Aligns the message stream to the next byte boundary.
	void		WriteByteAlign();

	//! Writes a specified number of bits from an integer value to the message
	void		WriteBits( int value, int numBits );

	//! Writes a boolean value to the message stream.
	void		WriteBool( bool c );

	//! Writes a signed 8-bit character value to the message buffer.
	void		WriteChar( int8 c );

	//! Writes a single byte value to the message stream.
	void		WriteByte( uint8 c );

	//! Writes a 16-bit signed integer to the message stream.
	void		WriteShort( int16 c );

	//! Writes a 16-bit unsigned integer to the message buffer.
	void		WriteUShort( uint16 c );

	//! Writes a 32-bit integer value to the message stream.
	void		WriteLong( int32 c );

	//! Writes a 64-bit integer to the message stream by splitting it into two 32-bit components.
	void		WriteLongLong( int64 c );

	//! Writes a floating-point value to the message using 32 bits.
	void		WriteFloat( float f );

	//! Writes a floating-point value using a specified number of exponent and mantissa bits to the message.
	void		WriteFloat( float f, int exponentBits, int mantissaBits );

	//! Writes an 8-bit representation of a floating-point angle value to the message.
	void		WriteAngle8( float f );

	//! Writes a 16-bit representation of the given angle value to the message.
	void		WriteAngle16( float f );

	//! Writes a direction vector as a bit representation to the message.
	void		WriteDir( const idVec3& dir, int numBits );

	//! Writes a string to the message buffer, optionally converting it to 7-bit ASCII.
	void		WriteString( const char* s, int maxLength = -1, bool make7Bit = true );

	//! Writes data to the message buffer.
	void		WriteData( const void* data, int length );

	//! Writes a network address structure to the message buffer.
	void		WriteNetadr( const netadr_t adr );

	//! Writes an 8-bit unsigned normalized floating-point value to the message.
	void		WriteUNorm8( float f ) { WriteByte( idMath::Ftob( f * 255.0f ) ); }

	//! Writes a 16-bit unsigned normalized floating-point value to the message.
	void		WriteUNorm16( float f ) { WriteUShort( idMath::Ftoi( f * 65535.0f ) ); }

	//! Writes a normalized 16-bit integer representation of the given floating-point value.
	void		WriteNorm16( float f ) { WriteShort( idMath::Ftoi( f * 32767.0f ) ); }

	//! Writes the difference between two signed 8-bit integer values as a byte.
	void		WriteDeltaChar( int8 oldValue, int8 newValue ) { WriteByte( newValue - oldValue ); }

	//! Writes the difference between two byte values to the message.
	void		WriteDeltaByte( uint8 oldValue, uint8 newValue ) { WriteByte( newValue - oldValue ); }

	//! Writes the difference between two 16-bit values as an unsigned short to the message.
	void		WriteDeltaShort( int16 oldValue, int16 newValue ) { WriteUShort( newValue - oldValue ); }

	//! Writes the difference between two unsigned short values as an unsigned short.
	void		WriteDeltaUShort( uint16 oldValue, uint16 newValue ) { WriteUShort( newValue - oldValue ); }

	//! Writes the difference between two 32-bit integers to the message buffer.
	void		WriteDeltaLong( int32 oldValue, int32 newValue ) { WriteLong( newValue - oldValue ); }

	//! Writes the difference between two float values as a delta.
	void		WriteDeltaFloat( float oldValue, float newValue ) { WriteFloat( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two floating-point values using a specified number of exponent and mantissa bits

		This function calculates the difference between the new and old floating-point values and writes the result using the WriteFloat method with the specified number of exponent and mantissa bits

		\param oldValue The original floating-point value
		\param newValue The new floating-point value
		\param exponentBits The number of bits to use for the exponent
		\param mantissaBits The number of bits to use for the mantissa
	*/
	void		WriteDeltaFloat( float oldValue, float newValue, int exponentBits, int mantissaBits ) { WriteFloat( newValue - oldValue, exponentBits, mantissaBits ); }

	//! Writes the differences between a dictionary and a base dictionary to the message buffer
	bool		WriteDeltaDict( const idDict& dict, const idDict* base );

	template<int _max_, int _numBits_>
	void WriteQuantizedFloat( float value );

	//! Writes a quantized unsigned float value using a variable number of bits.
	template<int _max_, int _numBits_>
	void WriteQuantizedUFloat( float value );

	//! Writes each component of the input vector as a float to the message
	template<typename T>
	void WriteVectorFloat( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteFloat( v[i] );
		}
	}

	//! Writes each component of the input vector as an unsigned normalized 8-bit value to the message.
	template<typename T>
	void WriteVectorUNorm8( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteUNorm8( v[i] );
		}
	}

	//! Writes each component of the input vector as an unsigned normalized 16-bit value to the message.
	template<typename T>
	void WriteVectorUNorm16( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteUNorm16( v[i] );
		}
	}

	//! Writes each component of the input vector as a normalized 16-bit value to the message.
	template<typename T>
	void WriteVectorNorm16( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteNorm16( v[i] );
		}
	}

	//! Writes a quantized vector using a variable number of bits for each component.
	template<typename T, int _max_, int _numBits_>
	void WriteQuantizedVector( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteQuantizedFloat<_max_, _numBits_>( v[i] );
		}
	}

	//! Initializes the message for reading by resetting the read position and temporary value.
	void   BeginReading() const;

	//! Aligns the read position to the next byte boundary.
	void   ReadByteAlign() const;

	//! Reads the specified number of bits from the message
	int	   ReadBits( int numBits ) const;

	//! Reads a single boolean value from the bit message.
	bool   ReadBool() const;

	//! Reads a signed 8-bit integer value from the message stream.
	int	   ReadChar() const;

	//! Reads a byte value from the message buffer.
	int	   ReadByte() const;

	//! Reads a 16-bit signed integer from the message buffer.
	int	   ReadShort() const;

	//! Reads a 16-bit unsigned integer value from the bit message.
	int	   ReadUShort() const;

	//! Reads a 32-bit signed integer from the message buffer
	int	   ReadLong() const;

	//! Reads a 64-bit integer value from the message buffer.
	int64  ReadLongLong() const;

	//! Reads a floating-point value from the message stream
	float  ReadFloat() const;

	//! Reads a floating-point value from the message stream using specified bit formats for exponent and mantissa.
	float  ReadFloat( int exponentBits, int mantissaBits ) const;

	//! Reads an 8-bit angle value and converts it to a float angle.
	float  ReadAngle8() const;

	//! Reads a 16-bit signed integer from the message and converts it to an angle in degrees.
	float  ReadAngle16() const;

	//! Reads a directory vector from the message using the specified number of bits.
	idVec3 ReadDir( int numBits ) const;

	//! Reads a null-terminated string from the message buffer into the provided character buffer
	int	   ReadString( char* buffer, int bufferSize ) const;

	//! Reads a null-terminated string from the message buffer into the provided string object
	int	   ReadString( idStr& str ) const;

	//! Reads a specified number of bytes from the message data into a buffer
	int	   ReadData( void* data, int length ) const;

	//! Reads a network address from the message buffer into the provided address structure.
	void   ReadNetadr( netadr_t* adr ) const;

	//! Reads an unsigned 8-bit integer from the message and converts it to a normalized float in the range [0, 1].
	float  ReadUNorm8() const { return ReadByte() / 255.0f; }

	//! Reads an unsigned 16-bit integer from the message and scales it to a floating-point value between 0 and 1.
	float  ReadUNorm16() const { return ReadUShort() / 65535.0f; }

	//! Reads a normalized 16-bit integer from the message and returns it as a floating-point value between -1.0 and 1.0.
	float  ReadNorm16() const { return ReadShort() / 32767.0f; }

	//! Reads a delta-encoded character value by adding the serialized byte to the old value.
	int8   ReadDeltaChar( int8 oldValue ) const { return oldValue + ReadByte(); }

	//! Reads a delta byte value by adding the serialized byte to the old value.
	uint8  ReadDeltaByte( uint8 oldValue ) const { return oldValue + ReadByte(); }

	//! Reads a delta-encoded short value from the message buffer.
	int16  ReadDeltaShort( int16 oldValue ) const { return oldValue + ReadUShort(); }

	//! Reads a delta-encoded unsigned short value from the message stream based on the provided old value.
	uint16 ReadDeltaUShort( uint16 oldValue ) const { return oldValue + ReadUShort(); }

	//! Reads a delta-encoded long value by adding the encoded delta to the old value.
	int32  ReadDeltaLong( int32 oldValue ) const { return oldValue + ReadLong(); }

	//! Reads a delta-encoded float value by adding the encoded delta to the old value.
	float  ReadDeltaFloat( float oldValue ) const { return oldValue + ReadFloat(); }

	//! Reads a delta float value by adding a float to the old value
	float  ReadDeltaFloat( float oldValue, int exponentBits, int mantissaBits ) const { return oldValue + ReadFloat( exponentBits, mantissaBits ); }

	//! Reads a delta dictionary from the message stream, applying changes to the provided dictionary and returning whether any changes were made.
	bool   ReadDeltaDict( idDict& dict, const idDict* base ) const;

	template<int _max_, int _numBits_>
	float ReadQuantizedFloat() const;

	//! Reads a quantized unsigned float value from the message buffer.
	template<int _max_, int _numBits_>
	float ReadQuantizedUFloat() const;

	//! Reads float values from the message and assigns them to the components of the given vector.
	template<typename T>
	void ReadVectorFloat( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadFloat();
		}
	}

	//! Reads normalized 8-bit unsigned integer values into a vector.
	template<typename T>
	void ReadVectorUNorm8( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadUNorm8();
		}
	}

	//! Reads normalized 16-bit unsigned integer values into a vector
	template<typename T>
	void ReadVectorUNorm16( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadUNorm16();
		}
	}

	//! Reads normalized 16-bit values into each component of the given vector.
	template<typename T>
	void ReadVectorNorm16( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadNorm16();
		}
	}

	//! Reads a quantized vector from the message stream into the provided vector object.
	template<typename T, int _max_, int _numBits_>
	void ReadQuantizedVector( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadQuantizedFloat<_max_, _numBits_>();
		}
	}

	//! Converts a normalized 3D direction vector into a compact bit representation.
	static int	  DirToBits( const idVec3& dir, int numBits );

	//! Converts a bit representation into a normalized 3D direction vector.
	static idVec3 BitsToDir( int bits, int numBits );

	//! Sets the hasChanged flag to the specified boolean value.
	void		  SetHasChanged( bool b ) { hasChanged = b; }

	//! Returns true if the bit message has changed since the last reset.
	bool		  HasChanged() const { return hasChanged; }

private:
	byte*		   writeData;	  // pointer to data for writing
	const byte*	   readData;	  // pointer to data for reading
	int			   maxSize;		  // maximum size of message in bytes
	int			   curSize;		  // current size of message in bytes
	mutable int	   writeBit;	  // number of bits written to the last written byte
	mutable int	   readCount;	  // number of bytes read so far
	mutable int	   readBit;		  // number of bits read from the last read byte
	bool		   allowOverflow; // if false, generate error when the message is overflowed
	bool		   overflowed;	  // set true if buffer size failed (with allowOverflow set)
	bool		   hasChanged;	  // Hack

	mutable uint64 tempValue;

private:
	//! Checks if writing the specified number of bits would cause an overflow in the message buffer.
	bool  CheckOverflow( int numBits );

	//! Returns a pointer to a byte space of the specified length within the message.
	byte* GetByteSpace( int length );
};

ID_INLINE void idBitMsg::InitWrite( byte* data, int length )
{
	writeData = data;
	readData  = data;
	maxSize	  = length;
	curSize	  = 0;

	writeBit	  = 0;
	readCount	  = 0;
	readBit		  = 0;
	allowOverflow = false;
	overflowed	  = false;

	tempValue = 0;
}

ID_INLINE void idBitMsg::InitRead( const byte* data, int length )
{
	writeData = NULL;
	readData  = data;
	maxSize	  = length;
	curSize	  = length;

	writeBit	  = 0;
	readCount	  = 0;
	readBit		  = 0;
	allowOverflow = false;
	overflowed	  = false;

	tempValue = 0;
}

ID_INLINE byte* idBitMsg::GetWriteData()
{
	return writeData;
}

ID_INLINE const byte* idBitMsg::GetReadData() const
{
	return readData;
}

ID_INLINE int idBitMsg::GetMaxSize() const
{
	return maxSize;
}

ID_INLINE void idBitMsg::SetAllowOverflow( bool set )
{
	allowOverflow = set;
}

ID_INLINE bool idBitMsg::IsOverflowed() const
{
	return overflowed;
}

ID_INLINE int idBitMsg::GetSize() const
{
	return curSize + ( writeBit != 0 );
}

ID_INLINE void idBitMsg::SetSize( int size )
{
	assert( writeBit == 0 );

	if( size > maxSize ) {
		curSize = maxSize;
	} else {
		curSize = size;
	}
}

ID_INLINE int idBitMsg::GetWriteBit() const
{
	return writeBit;
}

ID_INLINE void idBitMsg::SetWriteBit( int bit )
{
	// see idBitMsg::WriteByteAlign
	assert( false );
	writeBit = bit & 7;
	if( writeBit ) { writeData[curSize - 1] &= ( 1 << writeBit ) - 1; }
}

ID_INLINE int idBitMsg::GetNumBitsWritten() const
{
	return ( curSize << 3 ) + writeBit;
}

ID_INLINE int idBitMsg::GetRemainingSpace() const
{
	return maxSize - GetSize();
}

ID_INLINE int idBitMsg::GetRemainingWriteBits() const
{
	return ( maxSize << 3 ) - GetNumBitsWritten();
}

ID_INLINE void idBitMsg::SaveWriteState( int& s, int& b, uint64& t ) const
{
	s = curSize;
	b = writeBit;
	t = tempValue;
}

ID_INLINE void idBitMsg::RestoreWriteState( int s, int b, uint64 t )
{
	curSize	 = s;
	writeBit = b & 7;
	if( writeBit ) { writeData[curSize] &= ( 1 << writeBit ) - 1; }
	tempValue = t;
}

ID_INLINE int idBitMsg::GetReadCount() const
{
	return readCount;
}

ID_INLINE void idBitMsg::SetReadCount( int bytes )
{
	readCount = bytes;
}

ID_INLINE int idBitMsg::GetReadBit() const
{
	return readBit;
}

ID_INLINE void idBitMsg::SetReadBit( int bit )
{
	readBit = bit & 7;
}

ID_INLINE int idBitMsg::GetNumBitsRead() const
{
	return ( ( readCount << 3 ) - ( ( 8 - readBit ) & 7 ) );
}

ID_INLINE int idBitMsg::GetRemainingData() const
{
	assert( writeBit == 0 );
	return curSize - readCount;
}

ID_INLINE int idBitMsg::GetRemainingReadBits() const
{
	assert( writeBit == 0 );
	return ( curSize << 3 ) - GetNumBitsRead();
}

ID_INLINE void idBitMsg::SaveReadState( int& c, int& b ) const
{
	assert( writeBit == 0 );
	c = readCount;
	b = readBit;
}

ID_INLINE void idBitMsg::RestoreReadState( int c, int b )
{
	assert( writeBit == 0 );
	readCount = c;
	readBit	  = b & 7;
}

ID_INLINE void idBitMsg::BeginWriting()
{
	curSize	   = 0;
	overflowed = false;
	writeBit   = 0;
	tempValue  = 0;
}

ID_INLINE void idBitMsg::WriteByteAlign()
{
	// it is important that no uninitialized data slips in the msg stream,
	// because we use memcmp to decide if entities have changed and wether we should transmit them
	// this function has the potential to leave uninitialized bits into the stream,
	// however idBitMsg::WriteBits is properly initializing the byte to 0 so hopefully we are still safe
	// adding this extra check just in case
	curSize += writeBit != 0;
	assert( writeBit == 0 || ( ( writeData[curSize - 1] >> writeBit ) ==
								 0 ) ); // had to early out writeBit == 0 because when writeBit == 0 writeData[curSize - 1] may be the previous byte written and trigger false positives
	writeBit  = 0;
	tempValue = 0;
}

ID_INLINE void idBitMsg::WriteBool( bool c )
{
	WriteBits( c, 1 );
}

ID_INLINE void idBitMsg::WriteChar( int8 c )
{
	WriteBits( c, -8 );
}

ID_INLINE void idBitMsg::WriteByte( uint8 c )
{
	WriteBits( c, 8 );
}

ID_INLINE void idBitMsg::WriteShort( int16 c )
{
	WriteBits( c, -16 );
}

ID_INLINE void idBitMsg::WriteUShort( uint16 c )
{
	WriteBits( c, 16 );
}

ID_INLINE void idBitMsg::WriteLong( int32 c )
{
	WriteBits( c, 32 );
}

ID_INLINE void idBitMsg::WriteLongLong( int64 c )
{
	int a = c;
	int b = c >> 32;
	WriteBits( a, 32 );
	WriteBits( b, 32 );
}

ID_INLINE void idBitMsg::WriteFloat( float f )
{
	WriteBits( *reinterpret_cast<int*>( &f ), 32 );
}

ID_INLINE void idBitMsg::WriteFloat( float f, int exponentBits, int mantissaBits )
{
	int bits = idMath::FloatToBits( f, exponentBits, mantissaBits );
	WriteBits( bits, 1 + exponentBits + mantissaBits );
}

ID_INLINE void idBitMsg::WriteAngle8( float f )
{
	WriteByte( ANGLE2BYTE( f ) );
}

ID_INLINE void idBitMsg::WriteAngle16( float f )
{
	WriteShort( ANGLE2SHORT( f ) );
}

ID_INLINE void idBitMsg::WriteDir( const idVec3& dir, int numBits )
{
	WriteBits( DirToBits( dir, numBits ), numBits );
}

ID_INLINE void idBitMsg::BeginReading() const
{
	readCount = 0;
	readBit	  = 0;

	writeBit  = 0;
	tempValue = 0;
}

ID_INLINE void idBitMsg::ReadByteAlign() const
{
	readBit = 0;
}

ID_INLINE bool idBitMsg::ReadBool() const
{
	return ( ReadBits( 1 ) == 1 ) ? true : false;
}

ID_INLINE int idBitMsg::ReadChar() const
{
	return ( signed char )ReadBits( -8 );
}

ID_INLINE int idBitMsg::ReadByte() const
{
	return ( unsigned char )ReadBits( 8 );
}

ID_INLINE int idBitMsg::ReadShort() const
{
	return ( short )ReadBits( -16 );
}

ID_INLINE int idBitMsg::ReadUShort() const
{
	return ( unsigned short )ReadBits( 16 );
}

ID_INLINE int idBitMsg::ReadLong() const
{
	return ReadBits( 32 );
}

ID_INLINE int64 idBitMsg::ReadLongLong() const
{
	int64 a = ReadBits( 32 );
	int64 b = ReadBits( 32 );
	int64 c = ( 0x00000000ffffffff & a ) | ( b << 32 );
	return c;
}

ID_INLINE float idBitMsg::ReadFloat() const
{
	float value;
	*reinterpret_cast<int*>( &value ) = ReadBits( 32 );
	return value;
}

ID_INLINE float idBitMsg::ReadFloat( int exponentBits, int mantissaBits ) const
{
	int bits = ReadBits( 1 + exponentBits + mantissaBits );
	return idMath::BitsToFloat( bits, exponentBits, mantissaBits );
}

ID_INLINE float idBitMsg::ReadAngle8() const
{
	return BYTE2ANGLE( ReadByte() );
}

ID_INLINE float idBitMsg::ReadAngle16() const
{
	return SHORT2ANGLE( ReadShort() );
}

ID_INLINE idVec3 idBitMsg::ReadDir( int numBits ) const
{
	return BitsToDir( ReadBits( numBits ), numBits );
}

//! Writes a quantized floating-point value to the message buffer.
template<int _max_, int _numBits_>
ID_INLINE void idBitMsg::WriteQuantizedFloat( float value )
{
	enum { storeMax = ( 1 << ( _numBits_ - 1 ) ) - 1 };
	if( _max_ > storeMax ) {
		// Scaling down (scale should be < 1)
		const float scale = ( float )storeMax / ( float )_max_;
		WriteBits( idMath::ClampInt( -storeMax, storeMax, idMath::Ftoi( value * scale ) ), -_numBits_ );
	} else {
		// Scaling up (scale should be >= 1) (Preserve whole numbers when possible)
		enum { scale = storeMax / _max_ };
		WriteBits( idMath::ClampInt( -storeMax, storeMax, idMath::Ftoi( value * scale ) ), -_numBits_ );
	}
}

//! Writes a quantized unsigned float value to the bit message.
template<int _max_, int _numBits_>
ID_INLINE void idBitMsg::WriteQuantizedUFloat( float value )
{
	enum { storeMax = ( 1 << _numBits_ ) - 1 };
	if( _max_ > storeMax ) {
		// Scaling down (scale should be < 1)
		const float scale = ( float )storeMax / ( float )_max_;
		WriteBits( idMath::ClampInt( 0, storeMax, idMath::Ftoi( value * scale ) ), _numBits_ );
	} else {
		// Scaling up (scale should be >= 1) (Preserve whole numbers when possible)
		enum { scale = storeMax / _max_ };
		WriteBits( idMath::ClampInt( 0, storeMax, idMath::Ftoi( value * scale ) ), _numBits_ );
	}
}

//! Reads a quantized float value from the bit message.
template<int _max_, int _numBits_>
ID_INLINE float idBitMsg::ReadQuantizedFloat() const
{
	enum { storeMax = ( 1 << ( _numBits_ - 1 ) ) - 1 };
	if( _max_ > storeMax ) {
		// Scaling down (scale should be < 1)
		const float invScale = ( float )_max_ / ( float )storeMax;
		return ( float )ReadBits( -_numBits_ ) * invScale;
	} else {
		// Scaling up (scale should be >= 1) (Preserve whole numbers when possible)
		// Scale will be a whole number.
		// We use a float to get rid of (potential divide by zero) which is handled above, but the compiler is dumb
		const float scale	 = storeMax / _max_;
		const float invScale = 1.0f / scale;
		return ( float )ReadBits( -_numBits_ ) * invScale;
	}
}

template<int _max_, int _numBits_>
float idBitMsg::ReadQuantizedUFloat() const
{
	enum { storeMax = ( 1 << _numBits_ ) - 1 };
	if( _max_ > storeMax ) {
		// Scaling down (scale should be < 1)
		const float invScale = ( float )_max_ / ( float )storeMax;
		return ( float )ReadBits( _numBits_ ) * invScale;
	} else {
		// Scaling up (scale should be >= 1) (Preserve whole numbers when possible)
		// Scale will be a whole number.
		// We use a float to get rid of (potential divide by zero) which is handled above, but the compiler is dumb
		const float scale	 = storeMax / _max_;
		const float invScale = 1.0f / scale;
		return ( float )ReadBits( _numBits_ ) * invScale;
	}
}

//! Writes all the values from the array to the bit message
template<class _arrayType_>
void WriteFloatArray( idBitMsg& message, const _arrayType_& sourceArray )
{
	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		message.WriteFloat( sourceArray[i] );
	}
}

//! Writes a delta-encoded floating-point array to a bit message
template<class _arrayType_>
void WriteDeltaFloatArray( idBitMsg& message, const _arrayType_& oldArray, const _arrayType_& newArray )
{
	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		message.WriteDeltaFloat( oldArray[i], newArray[i] );
	}
}

//! Reads a fixed-size array of float values from a bit message
template<class _arrayType_>
_arrayType_ ReadFloatArray( const idBitMsg& message )
{
	_arrayType_ result;

	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		result[i] = message.ReadFloat();
	}

	return result;
}

//! Reads a delta-encoded float array from a bit message using the provided old array as reference.
template<class _arrayType_>
_arrayType_ ReadDeltaFloatArray( const idBitMsg& message, const _arrayType_& oldArray )
{
	_arrayType_ result;

	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		result[i] = message.ReadDeltaFloat( oldArray[i] );
	}

	return result;
}

#endif /* !__BITMSG_H__ */
