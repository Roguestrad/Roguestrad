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
	\brief A bit-level message buffer for serialization and deserialization of data

	The idBitMsg class provides a flexible buffer for writing and reading binary data at the bit level. It supports both reading and writing operations with explicit initialization for either mode.
   The class is designed for efficient serialization of game data including primitive types, vectors, strings, and complex structures like network addresses and dictionaries. It includes support for
   delta compression, quantized values, and bit-aligned operations to optimize network transmission and file storage. The implementation handles buffer management, overflow detection, and maintains
   internal read/write positions for seamless data flow. The class supports various data types with specialized methods for different precision requirements such as normalized floats and angle
   representations. The design allows for checkpointing of read/write states for complex serialization scenarios and provides utilities for encoding direction vectors efficiently.

*/
class idBitMsg
{
public:
	/*!
		\brief Initializes a bit message object for writing data to a buffer

		Constructs an idBitMsg object by initializing it for writing operations using a null buffer and zero length

	*/
	idBitMsg() { InitWrite( NULL, 0 ); }

	/*!
		\brief Initializes a bit message object for writing data to a buffer

		Constructs an idBitMsg object by initializing it for writing operations using the provided data buffer and its length

		\param data Pointer to the byte buffer to use for message writing
		\param length Size of the data buffer in bytes
	*/
	idBitMsg( byte* data, int length ) { InitWrite( data, length ); }

	/*!
		\brief Initializes a bit message for reading from a byte array

		Constructs a bit message object by initializing it for reading operations using the provided byte data and its length

		\param data Pointer to the byte array containing the message data
		\param length Number of bytes in the data array
	*/
	idBitMsg( const byte* data, int length ) { InitRead( data, length ); }

	/*!
		\brief Initializes the message buffer for writing data to a byte array

		Sets up the internal state of the message buffer to prepare for writing operations. The function configures the buffer to write to the provided data pointer with the specified length,
	   initializes all write-related counters and flags, and ensures the buffer is ready for bit-level writing operations. The read-related fields are reset to indicate that this buffer is configured
	   for write-only operations.

		\param data Pointer to the byte array where data will be written
		\param length Number of bytes available for writing
	*/
	void		InitWrite( byte* data, int length );

	/*!
		\brief Initializes the message buffer for reading data from a byte array

		Sets up the internal state of the message buffer to prepare for reading operations. The function configures the buffer to read from the provided data pointer with the specified length,
	   initializes all read-related counters and flags, and ensures the buffer is ready for bit-level reading operations. The write-related fields are reset to indicate that this buffer is configured
	   for read-only operations.

		\param data Pointer to the byte array containing the data to read
		\param length Number of bytes available for reading
	*/
	void		InitRead( const byte* data, int length );

	//! Returns a pointer to the data buffer for writing.
	byte*		GetWriteData();

	//! Returns the data pointer for reading from the message buffer
	const byte* GetReadData() const;

	//! Returns the maximum size of the message.
	int			GetMaxSize() const;

	/*!
		\brief Sets whether the message allows overflow.

		This function configures the message object to either allow or prevent overflow behavior. When overflow is not allowed, an error will be generated if the message exceeds its allocated size.

		\param set Flag indicating whether overflow should be allowed
	*/
	void		SetAllowOverflow( bool set );

	//! Returns true if the message was overflowed
	bool		IsOverflowed() const;

	//! Returns the size of the message in bytes.
	int			GetSize() const;

	/*!
		\brief Sets the size of the bit message to the specified value, clamping it to the maximum size if necessary

		This function configures the current size of the bit message buffer. It asserts that no bits have been written yet, ensuring the operation can only occur at the beginning of a message. The
	   size is clamped to the maximum allowed size if the provided size exceeds it

		\param size The desired size for the message buffer
		\throws Assertion failure if writeBit is not zero, indicating bits have already been written to the message
	*/
	void		SetSize( int size );

	//! Returns the current write bit position in the bit message.
	int			GetWriteBit() const;

	/*!
		\brief Sets the current write bit position within the message buffer

		This function configures the bit position where the next write operation will occur within the message buffer. It calculates the bit offset within the current byte and ensures that any bits
	   beyond the specified position are cleared. The function asserts that the operation is valid and updates the internal write bit tracking.

		\param bit the bit position to set as the current write bit
		\throws assertion failure if the implementation is not valid
	*/
	void		SetWriteBit( int bit );

	//! Returns the total number of bits written to the bit message.
	int			GetNumBitsWritten() const;

	//! Returns the amount of space remaining in the message buffer for writing.
	int			GetRemainingSpace() const;

	//! Returns the number of bits remaining for writing in the message.
	int			GetRemainingWriteBits() const;

	/*!
		\brief Saves the current write state of the bit message.

		This function preserves the current state of the bit message writer, including the current size, the current bit position, and the temporary value. It is useful for checkpointing the write
	   state so it can be restored later.

		\param s Reference to store the current size of the message
		\param b Reference to store the current bit position
		\param t Reference to store the temporary value used during writing
	*/
	void		SaveWriteState( int& s, int& b, uint64& t ) const;

	/*!
		\brief Restores the write state of the bit message stream

		This function restores the internal state of the bit message writer to a previously saved state. It sets the current size, resets the write bit position, and applies a mask to clear any
	   remaining bits in the current byte. The temporary value is also restored to maintain consistency in the message handling.

		\param s The current size to restore
		\param b The bit position to restore
		\param t The temporary value to restore
	*/
	void		RestoreWriteState( int s, int b, uint64 t );

	//! Returns the number of bytes read from the message buffer.
	int			GetReadCount() const;

	/*!
		\brief Sets the number of bytes read from the message.

		This function updates the internal read counter to the specified number of bytes, effectively resetting or adjusting the position from which subsequent reads will occur.

		\param bytes The number of bytes to set the read position to
	*/
	void		SetReadCount( int bytes );

	//! Returns the current read bit position in the message.
	int			GetReadBit() const;

	/*!
		\brief Sets the current read bit position within the message.

		This function configures the bit offset from which the next read operation will begin. The bit position is constrained to the range 0-7 using a bitwise AND operation with 7.

		\param bit The bit position to set for the next read operation
	*/
	void		SetReadBit( int bit );

	//! Returns the number of bits that have been read from the message.
	int			GetNumBitsRead() const;

	//! Returns the number of bytes remaining to be read from the message.
	int			GetRemainingData() const;

	//! Returns the number of bits remaining to be read from the message.
	int			GetRemainingReadBits() const;

	/*!
		\brief Saves the current read position state of the bit message

		This function saves the current read position within the bit message by storing the current read count and bit offset. It is typically used before reading data that might need to be restored
	   later, such as when processing unreliable messages or handling error cases. The function asserts that the write bit is zero, ensuring the message is in a consistent state for reading.

		\param c Reference to store the current read count
		\param b Reference to store the current read bit offset
		\throws _assertion failure if writeBit is not zero_
	*/
	void		SaveReadState( int& c, int& b ) const;

	/*!
		\brief Restores the read state of the message to the saved count and bit positions.

		This function restores the read position of the message to a previously saved state using the provided count and bit values. It asserts that the write bit is zero before restoring the state.
	   The read count is set to the provided count value, and the read bit is set to the provided bit value modulo 8.

		\param c The saved read count to restore
		\param b The saved read bit position to restore
		\throws assertion failure if writeBit is not zero
	*/
	void		RestoreReadState( int c, int b );

	/*!
		\brief Initializes the message object for writing data.

		Prepares the message object to begin writing new data by resetting internal state variables. The current size is set to zero, the overflow flag is cleared, the write bit position is reset, and
	   the temporary value is initialized to zero. This function should be called before starting to write data into the message buffer.

	*/
	void		BeginWriting();

	/*!
		\brief Aligns the message stream to the next byte boundary by ensuring proper bit alignment.

		This function ensures that the current message stream is properly aligned to the next byte boundary. It is important to avoid uninitialized data in the message stream because the engine uses
	   memcmp to determine if entities have changed and need transmission. The function adds a check to ensure that any bits written are properly initialized to zero to prevent false positives in
	   change detection. The function updates the current size and resets the bit counter to zero after alignment.

		\throws Assertion failure if there are uninitialized bits in the message stream that would cause false positives in change detection.
	*/
	void		WriteByteAlign();

	/*!
		\brief Writes a specified number of bits from an integer value to the message buffer

		This function writes a specified number of bits from the provided integer value to the message buffer. It performs validation to ensure the number of bits is valid and that the value fits
	   within the specified bit width. The function handles both positive and negative bit counts, and manages bit overflow by checking against the message buffer capacity. It efficiently packs bits
	   into bytes and flushes full bytes to the buffer while keeping any remaining bits for the next write operation

		\param value The integer value to write bits from
		\param numBits The number of bits to write, can be negative to indicate signed bit fields
		\throws Fatal error if the number of bits is invalid, if the value overflows the specified bit width, or if writing to the message fails
	*/
	void		WriteBits( int value, int numBits );

	/*!
		\brief Writes a boolean value to the message stream using one bit.
		\param c The boolean value to write to the message stream
	*/
	void		WriteBool( bool c );

	/*!
		\brief Writes a signed 8-bit integer value to the message stream.

		This function writes a character value to the message bit stream. The value is written using the WriteBits method with a bit count of -8, which indicates a signed 8-bit integer. The negative
	   bit count specifies that the value should be treated as signed.

		\param c The signed 8-bit integer value to write to the message stream
	*/
	void		WriteChar( int8 c );

	/*!
		\brief Writes a single byte value to the message buffer

		This function writes a byte value to the message buffer by calling WriteBits with 8 bits. It is a simple inline function that provides a convenient way to serialize byte-sized data. The
	   function is typically used in serialization contexts where data needs to be written to a message buffer for network transmission or file storage. The byte is written using the underlying bit
	   manipulation functions of the idBitMsg class, which handles the actual buffering and bit packing operations. The function is commonly used in conjunction with other serialization functions like
	   SerializePacked and SerializeSPacked which handle more complex data types with variable bit encoding. The implementation assumes that the byte value is already properly formatted and does not
	   perform any range checking or validation beyond what WriteBits does.

		\param c the byte value to write to the message buffer
	*/
	void		WriteByte( uint8 c );

	/*!
		\brief Writes a 16-bit signed integer to the message stream.

		This function writes a 16-bit signed integer value to the message stream using the WriteBits method. The integer is written with a bit count of -16, which indicates that the full 16-bit
	   representation should be written.

		\param c The 16-bit signed integer value to write to the message stream
	*/
	void		WriteShort( int16 c );

	/*!
		\brief Writes a 16-bit unsigned integer to the message buffer.

		This function writes a 16-bit unsigned integer value to the message buffer by calling WriteBits with a bit count of 16. It is typically used when serializing data for network transmission or
	   file storage.

		\param c The 16-bit unsigned integer value to write to the message buffer
	*/
	void		WriteUShort( uint16 c );

	/*!
		\brief Writes a 32-bit signed integer to the message buffer.

		This function writes a 32-bit signed integer value to the message buffer by calling WriteBits with a bit count of 32. It is typically used when serializing data for network transmission or
	   file storage.

		\param c The 32-bit signed integer value to write to the message buffer
	*/
	void		WriteLong( int32 c );

	/*!
		\brief Writes a 64-bit integer value to the message by splitting it into two 32-bit components

		This function takes a 64-bit integer value and writes it to the message stream by splitting it into two 32-bit parts. The lower 32 bits are written first, followed by the upper 32 bits. This
	   approach ensures that the 64-bit value is properly serialized in a consistent manner regardless of the platform's endianness.

		\param c The 64-bit integer value to be written to the message
	*/
	void		WriteLongLong( int64 c );

	/*!
		\brief Writes a 32-bit floating-point value to the message using bit-level representation

		This function takes a floating-point value and writes its binary representation to the message stream. The implementation uses reinterpret_cast to treat the float as an integer before writing
	   it as 32 bits. This approach preserves the exact binary representation of the float, which is useful for network synchronization or saving/loading binary data where precise bit representation
	   is required.

		\param f The floating-point value to write to the message
	*/
	void		WriteFloat( float f );

	/*!
		\brief Writes a floating-point value using a specified number of exponent and mantissa bits to the message.

		This function converts a floating-point value into its bit representation using the specified number of exponent and mantissa bits, then writes those bits to the message. The total number of
	   bits written is determined by the sum of the exponent bits, mantissa bits, and one sign bit.

		\param f The floating-point value to write
		\param exponentBits The number of bits to use for the exponent
		\param mantissaBits The number of bits to use for the mantissa
	*/
	void		WriteFloat( float f, int exponentBits, int mantissaBits );

	/*!
		\brief Writes a byte representation of the given angle value to the message.

		This function converts the provided angle value into a byte representation and writes it to the message buffer. The conversion uses the ANGLE2BYTE macro which maps the floating-point angle to
	   a byte integer, likely for compression or storage efficiency. The function is marked as ID_INLINE, indicating it should be inlined for performance.

		\param f The angle value to be written as a byte representation
	*/
	void		WriteAngle8( float f );

	/*!
		\brief Writes a 16-bit representation of the given angle value to the message.

		This function converts the provided angle value into a 16-bit short integer representation and writes it to the message buffer. The conversion uses the ANGLE2SHORT macro which maps the
	   floating-point angle to a short integer, likely for compression or storage efficiency. The function is marked as ID_INLINE, indicating it should be inlined for performance.

		\param f The angle value to be written as a 16-bit representation
	*/
	void		WriteAngle16( float f );

	/*!
		\brief Writes a direction vector to the message using a specified number of bits

		This function encodes a 3D direction vector into a bit stream using the DirToBits helper function. The direction is quantized to the specified number of bits, which determines the precision of
	   the encoded direction. The function is inlined for performance reasons.

		\param dir The 3D direction vector to be written
		\param numBits The number of bits to use for encoding the direction
	*/
	void		WriteDir( const idVec3& dir, int numBits );

	/*!
		\brief Writes a string to the message buffer, optionally converting it to 7-bit ASCII format.

		This function writes a null-terminated string to the message buffer. If the input string is null, it writes an empty string. The function supports limiting the string length and converting
	   characters outside the 7-bit ASCII range to periods. The string is written with a null terminator. The maximum length parameter can be used to truncate strings to prevent buffer overflows.

		\param s The null-terminated string to write, or null to write an empty string
		\param maxLength The maximum number of characters to write, or -1 for no limit
		\param make7Bit If true, characters outside 7-bit ASCII range are converted to periods
	*/
	void		WriteString( const char* s, int maxLength = -1, bool make7Bit = true );

	/*!
		\brief Writes a specified number of bytes from a data buffer into the message.

		This function copies a given number of bytes from the provided data buffer into the internal message buffer. The destination space in the message buffer is allocated dynamically based on the
	   required length. It is typically used to serialize arbitrary binary data into the message stream.

		\param data Pointer to the source data to be written into the message
		\param length Number of bytes to copy from the data buffer
	*/
	void		WriteData( const void* data, int length );

	/*!
		\brief Writes a network address to the message buffer

		This function serializes a network address structure into the message buffer by writing the IP address, port, and address type in sequence. The IP address is written as 4 bytes, the port as an
	   unsigned short, and the type as a byte.

		\param adr The network address to write to the message buffer
	*/
	void		WriteNetadr( const netadr_t adr );

	/*!
		\brief Writes an 8-bit unsigned normalized floating-point value to the message.

		This function converts a floating-point value in the range [0, 1] to an 8-bit unsigned integer by scaling it to the range [0, 255] and then writes the result as a byte to the message. The
	   conversion is performed using the idMath::Ftob function which truncates the floating-point value to an integer.

		\param f The floating-point value to be written, expected to be in the range [0, 1]
	*/
	void		WriteUNorm8( float f ) { WriteByte( idMath::Ftob( f * 255.0f ) ); }

	/*!
		\brief Writes a 16-bit unsigned normalized floating-point value to the message.

		This function takes a floating-point value in the range [0, 1] and converts it to a 16-bit unsigned integer by scaling it to the range [0, 65535]. The resulting integer is then written to the
	   message using the WriteUShort method. This is commonly used for packing color or texture coordinate data into a compact format.

		\param f The floating-point value to be written, expected to be in the range [0, 1]
	*/
	void		WriteUNorm16( float f ) { WriteUShort( idMath::Ftoi( f * 65535.0f ) ); }

	/*!
		\brief Writes a normalized 16-bit integer representation of the given floating-point value.

		The function takes a floating-point value in the range [-1.0, 1.0] and scales it to fit within the range of a 16-bit signed integer. The scaling factor used is 32767.0, which maps the input
	   range to approximately [-32767, 32767]. The result is then written as a short integer to the message buffer.

		\param f The normalized floating-point value to be written as a 16-bit integer
	*/
	void		WriteNorm16( float f ) { WriteShort( idMath::Ftoi( f * 32767.0f ) ); }

	/*!
		\brief Writes the difference between two char values as a byte to the message.

		This function calculates the difference between the old and new char values and writes the result as a single byte to the message. It is typically used for delta compression to efficiently
	   transmit changes in character values.

		\param oldValue The previous value of the character
		\param newValue The new value of the character
	*/
	void		WriteDeltaChar( int8 oldValue, int8 newValue ) { WriteByte( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two byte values as a single byte to the message.

		This function calculates the difference between the old and new byte values and writes the result as a single byte to the message buffer. It is typically used in network synchronization to
	   efficiently transmit changes between states.

		\param oldValue The initial byte value
		\param newValue The new byte value to compare against
	*/
	void		WriteDeltaByte( uint8 oldValue, uint8 newValue ) { WriteByte( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two 16-bit values as an unsigned short to the message.

		This function calculates the difference between the new and old 16-bit values and writes the result as an unsigned short to the message buffer. It is typically used in network communication to
	   efficiently transmit changes in values by sending only the delta instead of the full value.

		\param oldValue The previous 16-bit value
		\param newValue The new 16-bit value
	*/
	void		WriteDeltaShort( int16 oldValue, int16 newValue ) { WriteUShort( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two unsigned short values to the message buffer.

		This function calculates the difference between the new value and the old value, then writes the result as an unsigned short to the message buffer. It is typically used for delta compression
	   to efficiently transmit changes in values.

		\param oldValue The previous value to subtract from the new value
		\param newValue The current value to be written as a delta
	*/
	void		WriteDeltaUShort( uint16 oldValue, uint16 newValue ) { WriteUShort( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two 32-bit integers to the message.

		This function calculates the delta between the old and new 32-bit values and writes the result to the message stream. It is typically used for efficiently transmitting changes in integer
	   values by sending only the difference rather than the full value.

		\param oldValue The previous 32-bit integer value
		\param newValue The new 32-bit integer value
	*/
	void		WriteDeltaLong( int32 oldValue, int32 newValue ) { WriteLong( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two floating-point values to the message.

		This function calculates the difference between the old and new floating-point values and writes the result to the message buffer. It is typically used in network communication to efficiently
	   transmit changes in floating-point values by sending only the delta rather than the full value.

		\param oldValue The initial floating-point value
		\param newValue The new floating-point value
	*/
	void		WriteDeltaFloat( float oldValue, float newValue ) { WriteFloat( newValue - oldValue ); }

	/*!
		\brief Writes the difference between two floating-point values using a specified number of exponent and mantissa bits

		This function calculates the delta between the old and new floating-point values and writes the result using the WriteFloat method with the provided exponent and mantissa bit specifications.
	   It is typically used for efficiently transmitting changes in floating-point values over a network or saving bandwidth

		\param oldValue The previous floating-point value
		\param newValue The new floating-point value
		\param exponentBits The number of bits to use for the exponent component
		\param mantissaBits The number of bits to use for the mantissa component
	*/
	void		WriteDeltaFloat( float oldValue, float newValue, int exponentBits, int mantissaBits ) { WriteFloat( newValue - oldValue, exponentBits, mantissaBits ); }

	/*!
		\brief Writes the difference between a dictionary and a base dictionary to the message stream.

		This function compares the provided dictionary with a base dictionary and writes only the differing key-value pairs to the message stream. If the base dictionary is not null, it writes both
	   new or changed key-value pairs from the provided dictionary and removed keys from the base dictionary. If the base dictionary is null, it writes all key-value pairs from the provided
	   dictionary. The function returns true if any changes were written, otherwise false.

		\param dict The dictionary to write differences for
		\param base The base dictionary to compare against, can be null
		\return True if any changes were written to the message stream, false otherwise.
	*/
	bool		WriteDeltaDict( const idDict& dict, const idDict* base );

	template<int _max_, int _numBits_>
	void WriteQuantizedFloat( float value );

	/*!
		\brief Writes a quantized unsigned float value using a variable number of bits

		This function quantizes a floating-point value into a variable bit representation suitable for unsigned values. The quantization process maps the input float to a discrete set of values based
	   on the number of bits available. The function assumes the input represents an unsigned value and applies simple quantization logic to store the value efficiently. The exact number of bits used
	   for quantization depends on the implementation details of the bit message system.

		\param value The floating-point value to quantize and write to the message
	*/
	template<int _max_, int _numBits_>
	void WriteQuantizedUFloat( float value );

	/*!
		\brief Writes each component of the vector v as a float to the message

		This function iterates through all dimensions of the input vector v and writes each component as a float value to the message stream. The vector's dimension is determined by calling
	   GetDimension() method, and each component is accessed using the [] operator

		\param v the vector whose components are to be written as floats
	*/
	template<typename T>
	void WriteVectorFloat( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteFloat( v[i] );
		}
	}

	/*!
		\brief Writes each component of a vector as an unsigned normalized 8-bit value

		This function takes a vector object and writes each of its components as unsigned normalized 8-bit values to the message buffer. It iterates through each dimension of the vector and calls
	   WriteUNorm8 for each component. The vector type T must have a GetDimension() method and support indexing operator [].

		\param v The vector whose components will be written as unsigned normalized 8-bit values
	*/
	template<typename T>
	void WriteVectorUNorm8( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteUNorm8( v[i] );
		}
	}

	/*!
		\brief Writes each component of a vector as an unsigned normalized 16-bit value

		This function takes a vector with a specified dimension and writes each of its components as unsigned normalized 16-bit values to the message stream. It iterates through each component of the
	   vector and calls WriteUNorm16 to serialize the data

		\param v The vector whose components will be written as unsigned normalized 16-bit values
	*/
	template<typename T>
	void WriteVectorUNorm16( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteUNorm16( v[i] );
		}
	}

	/*!
		\brief Writes a normalized 16-bit representation of each component in the input vector

		This function takes a vector of type T and writes each of its components as 16-bit normalized values. It iterates through all dimensions of the vector and calls WriteNorm16 for each component
	   to serialize the data. The vector type T must support GetDimension() and indexing operations.

		\param v Input vector whose components will be written as normalized 16-bit values
	*/
	template<typename T>
	void WriteVectorNorm16( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteNorm16( v[i] );
		}
	}

	/*!
		\brief Writes a quantized vector using a variable number of bits for each component

		This function compresses a vector by quantizing each component using a specified maximum value and number of bits. It iterates through each dimension of the vector and writes the quantized
	   float value for each component. The quantization process assumes signed values and uses simple quantization techniques to reduce the bit requirement for each component

		\param v The vector to be quantized and written, where T is a vector type with GetDimension() method and bracket operator
	*/
	template<typename T, int _max_, int _numBits_>
	void WriteQuantizedVector( const T& v )
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			WriteQuantizedFloat<_max_, _numBits_>( v[i] );
		}
	}

	/*!
		\brief Initializes the message reading state to prepare for reading bits from the message.

		The function resets the internal reading counters and temporary values to their initial state, allowing the message to be read from the beginning. This is typically called before starting to
	   read data from a message buffer that was previously written to.

	*/
	void   BeginReading() const;

	/*!
		\brief Aligns the read position to the next byte boundary.

		This function resets the bit read position to the beginning of the next byte, effectively skipping any remaining bits in the current byte. It is typically used to ensure proper alignment when
	   reading data that requires byte-aligned access.

	*/
	void   ReadByteAlign() const;

	/*!
		\brief Reads a specified number of bits from the message buffer and returns the value.

		This function reads a specified number of bits from the message buffer. It handles both signed and unsigned bit reading, with the number of bits specified by the numBits parameter. The
	   function checks for valid bit counts and ensures there are enough remaining bits to read. If the number of bits is negative, it treats the value as signed and performs sign extension. The
	   function returns -1 if there aren't enough bits available to read, and otherwise returns the read value.

		\param numBits The number of bits to read from the message buffer. If negative, the value is treated as signed.
		\return The value read from the message buffer as an integer. Returns -1 if there aren't enough bits available to read.
		\throws idLib::FatalError if attempting to read from a message with no read data or with invalid bit count.
	*/
	int	   ReadBits( int numBits ) const;

	/*!
		\brief Reads a single boolean value from the bit message stream.

		This function extracts one bit from the bit message stream and interprets it as a boolean value. It calls ReadBits with a count of 1 to retrieve the bit, then compares the result to 1 to
	   determine the boolean state. The function is marked as const, indicating it does not modify the message stream's internal state during reading.

		\return A boolean value read from the bit message stream, either true or false.
	*/
	bool   ReadBool() const;

	/*!
		\brief Reads a signed 8-bit integer value from the message stream

		This function reads a character value from the bit stream by reading 8 bits and interpreting them as a signed 8-bit integer. It is used for serialization operations where signed character
	   values need to be retrieved from a message buffer. The function calls ReadBits with a parameter of -8 to specify the number of bits to read and interpret as a signed value.

		\return The signed 8-bit integer value read from the message stream
	*/
	int	   ReadChar() const;

	/*!
		\brief Reads a single byte value from the message buffer

		This function reads 8 bits from the message buffer and returns them as an unsigned char cast to an int. It is typically used as part of serialization operations where binary data needs to be
	   extracted from a message stream. The function is marked as inline for performance reasons and is const, indicating it does not modify the message object state.

		\return The byte value read from the message buffer, cast to an int
	*/
	int	   ReadByte() const;

	/*!
		\brief Reads a 16-bit signed integer from the message

		This function reads a 16-bit signed integer from the bit message stream. It internally calls ReadBits with a parameter of -16 to extract the bits and then casts the result to a short integer.
	   The function is typically used when serializing or deserializing data that requires 16-bit signed integer values, such as ping values in networking code.

		\return A 16-bit signed integer value that was read from the message
	*/
	int	   ReadShort() const;

	/*!
		\brief Reads and returns a 16-bit unsigned integer value from the message buffer

		This function extracts a 16-bit unsigned short value from the bit stream by reading 16 bits and casting the result to an integer. It is typically used for serializing and deserializing
	   unsigned short values in network communications or file I/O operations. The function is marked as inline for performance reasons and is const since it only reads from the message buffer without
	   modifying it.

		\return The 16-bit unsigned integer value read from the message buffer
	*/
	int	   ReadUShort() const;

	//! Reads a 32-bit signed integer from the bit message.
	int	   ReadLong() const;

	/*!
		\brief Reads a 64-bit signed integer from the message buffer

		This function reads a 64-bit signed integer from the message buffer by reading two 32-bit chunks and combining them into a single 64-bit value. It is used for serializing and deserializing
	   64-bit integer values in the network protocol or file serialization system.

		\return The 64-bit signed integer value read from the message buffer
	*/
	int64  ReadLongLong() const;

	/*!
		\brief Reads a 32-bit floating point value from the message bit stream

		This function extracts a 32-bit floating point value from the bit stream by reading 32 bits and interpreting them as a float. It is typically used during serialization operations to
	   reconstruct floating point values that were previously written to a message buffer. The function is marked as inline for performance reasons and is const, indicating it doesn't modify the
	   message object state.

		\return The floating point value that was read from the bit stream
	*/
	float  ReadFloat() const;

	/*!
		\brief Reads a floating-point value from the message using specified exponent and mantissa bits

		This function reads a bit-encoded floating-point value from the message stream. It first reads the specified number of bits combining the sign, exponent, and mantissa fields, then converts the
	   raw bits into a float using the idMath::BitsToFloat function. The function is typically used for serialization to read back floating-point values that were written using a corresponding
	   WriteFloat function.

		\param exponentBits number of bits to use for the exponent field
		\param mantissaBits number of bits to use for the mantissa field
		\return the decoded floating-point value read from the message
	*/
	float  ReadFloat( int exponentBits, int mantissaBits ) const;

	//! Reads an 8-bit angle value and converts it to a float angle.
	float  ReadAngle8() const;

	//! Reads a 16-bit signed integer from the message and converts it to an angle in degrees.
	float  ReadAngle16() const;

	//! Reads a direction vector from the message using the specified number of bits.
	idVec3 ReadDir( int numBits ) const;

	/*!
		\brief Reads a null-terminated string from the message stream into the provided buffer

		This function reads bytes from the message stream until it encounters a null byte or a byte with value 255 or greater. It translates any '%' characters to '.' to prevent potential issues with
	   format specifiers in string routines. The function ensures that the buffer is null-terminated and will truncate the string if it exceeds the buffer size. The function returns the length of the
	   string read, excluding the null terminator

		\param buffer Output buffer to store the read string
		\param bufferSize Size of the output buffer in bytes
		\return Length of the string read, excluding the null terminator
	*/
	int	   ReadString( char* buffer, int bufferSize ) const;

	/*!
		\brief Reads a null-terminated string from the message buffer and stores it in the provided idStr object

		This function reads a string from the message buffer starting at the current read position. It first aligns the read position to a byte boundary, then scans for a null terminator to determine
	   the string length. The string is then copied into the provided idStr object and the read position is advanced by the string length plus one to account for the null terminator. The function
	   returns the length of the read string.

		\param str The idStr object to store the read string in
		\return The length of the string that was read from the message buffer
	*/
	int	   ReadString( idStr& str ) const;

	/*!
		\brief Reads a specified number of bytes from the message data into the provided buffer

		This function reads data from an internal message buffer starting at the current read position. It ensures proper byte alignment before reading and handles cases where the requested read
	   length would exceed the remaining data in the message. The function updates the internal read position and returns the actual number of bytes read

		\param data Pointer to the buffer where the read data will be stored
		\param length Number of bytes to read from the message
		\return Actual number of bytes read from the message, which may be less than the requested length if end-of-message is reached
	*/
	int	   ReadData( void* data, int length ) const;

	/*!
		\brief Reads network address data from the message buffer into the provided address structure

		This function reads network address information from the message buffer and populates the provided netadr_t structure. It reads the IP address as a 4-byte value, the port as an unsigned 16-bit
	   integer, and the address type as an unsigned 8-bit integer. The function assumes the message buffer is positioned at the correct location for reading the network address data.

		\param adr Pointer to the network address structure to be populated with data from the message
	*/
	void   ReadNetadr( netadr_t* adr ) const;

	/*!
		\brief Reads an unsigned 8-bit integer from the message and returns it as a normalized floating-point value in the range [0, 1].

		This function retrieves a byte value from the message buffer and scales it to a floating-point value between 0.0 and 1.0. The scaling is performed by dividing the byte value by 255.0, which
	   maps the 8-bit range [0, 255] to the floating-point range [0, 1]. This is commonly used for normalizing color components or other data that is stored in a compact 8-bit format.

		\return A floating-point value in the range [0, 1] representing the normalized unsigned 8-bit integer read from the message
	*/
	float  ReadUNorm8() const { return ReadByte() / 255.0f; }

	/*!
		\brief Reads an unsigned 16-bit integer from the message and converts it to a normalized floating-point value in the range [0, 1].

		This function retrieves an unsigned 16-bit integer from the message stream and scales it to a floating-point value between 0 and 1. The scaling is achieved by dividing the unsigned 16-bit
	   integer by 65535, which is the maximum value that can be represented by a 16-bit unsigned integer. This is commonly used for encoding normalized color or texture coordinate data.

		\return A floating-point value in the range [0, 1] representing the normalized unsigned 16-bit integer read from the message.
	*/
	float  ReadUNorm16() const { return ReadUShort() / 65535.0f; }

	/*!
		\brief Reads a normalized 16-bit signed integer from the message and returns it as a floating-point value in the range [-1, 1].

		The function reads a 16-bit signed integer from the message buffer using ReadShort and normalizes it by dividing by 32767.0f. This normalization maps the integer range [-32768, 32767] to the
	   floating-point range [-1, 1]. The function is marked as const, indicating it does not modify the message object's state.

		\return A floating-point value in the range [-1, 1] representing the normalized 16-bit signed integer read from the message.
	*/
	float  ReadNorm16() const { return ReadShort() / 32767.0f; }

	/*!
		\brief Reads a delta-encoded character value by adding a byte delta to the provided old value

		This function implements delta compression for character values by reading a single byte delta from the message stream and adding it to the provided old value. It is used in conjunction with
	   WriteDeltaChar to efficiently serialize small changes in character values. The function assumes the message stream is properly initialized and contains valid data.

		\param oldValue The base character value to which the delta will be applied
		\return The new character value calculated as oldValue plus the delta byte read from the message stream
	*/
	int8   ReadDeltaChar( int8 oldValue ) const { return oldValue + ReadByte(); }

	/*!
		\brief Reads a delta-encoded byte value by adding the serialized byte to the old value

		This function implements delta compression for byte values. It reads a serialized byte from the message stream and adds it to the provided old value to reconstruct the current value. This is
	   typically used in network serialization to send only the difference between consecutive values rather than the full values, reducing bandwidth usage. The function is part of a serialization
	   system that supports delta compression for various data types.

		\param oldValue The previous or base value to which the delta will be added
		\return The reconstructed byte value after adding the delta to the old value
	*/
	uint8  ReadDeltaByte( uint8 oldValue ) const { return oldValue + ReadByte(); }

	/*!
		\brief Reads a delta-encoded short integer value by adding the decoded unsigned short to the provided old value

		This function is used to decode a delta-compressed short integer value. It reads an unsigned short integer from the message stream and adds it to the provided old value to reconstruct the
	   original short integer. This is commonly used in network serialization to reduce bandwidth usage by only transmitting the difference between consecutive values. The function is typically called
	   as part of a serialization process where delta encoding is employed for efficiency.

		\param oldValue The base value to which the delta will be added
		\return The decoded short integer value obtained by adding the read unsigned short to the old value
	*/
	int16  ReadDeltaShort( int16 oldValue ) const { return oldValue + ReadUShort(); }

	/*!
		\brief Reads a delta value for a 16-bit unsigned integer from the message stream

		This function reads a delta-encoded 16-bit unsigned integer from the message stream. It is used in conjunction with WriteDeltaUShort to compress network messages by only sending the difference
	   between the current value and a base value. The function takes an old value as a parameter and adds the delta value read from the stream to it to reconstruct the original value. This approach
	   reduces bandwidth usage when sending small changes to values that are mostly similar to previous values.

		\param oldValue The base value to which the delta will be added to reconstruct the original value
		\return The reconstructed 16-bit unsigned integer value after adding the delta to the old value
	*/
	uint16 ReadDeltaUShort( uint16 oldValue ) const { return oldValue + ReadUShort(); }

	/*!
		\brief Reads a delta-encoded long integer value by adding the encoded delta to the provided old value

		This function is used to decode a delta-compressed integer value. It reads a raw long integer from the message stream and adds it to the provided base value to reconstruct the original
	   integer. This technique is commonly used in network communication to reduce bandwidth usage by sending only the differences between consecutive values rather than the full values.

		\param oldValue The base value to which the delta will be added to reconstruct the original value
		\return The reconstructed integer value obtained by adding the delta to the provided old value
	*/
	int32  ReadDeltaLong( int32 oldValue ) const { return oldValue + ReadLong(); }

	/*!
		\brief Reads a delta-encoded float value by adding a delta to the old value

		This function is used to deserialize a delta-encoded float value from a message stream. It reads a float delta value using the ReadFloat method and adds it to the provided old value to
	   reconstruct the original float value. This is commonly used in network synchronization and serialization where only the difference between values is transmitted to save bandwidth.

		\param oldValue The previous or base float value to which the delta will be added
		\return The reconstructed float value obtained by adding the read delta to the old value
	*/
	float  ReadDeltaFloat( float oldValue ) const { return oldValue + ReadFloat(); }

	/*!
		\brief Reads a delta-encoded float value by adding a delta to the old value

		This function is used to decode a delta-encoded floating-point value. It takes an old value and adds a newly read float delta to it, where the delta is constructed from the specified number of
	   exponent and mantissa bits. This is commonly used in network serialization to efficiently transmit small changes in values

		\param oldValue The previous or base value to which the delta will be added
		\param exponentBits The number of bits to use for the exponent component of the delta
		\param mantissaBits The number of bits to use for the mantissa component of the delta
		\return The reconstructed float value obtained by adding the delta to the old value
	*/
	float  ReadDeltaFloat( float oldValue, int exponentBits, int mantissaBits ) const { return oldValue + ReadFloat( exponentBits, mantissaBits ); }

	/*!
		\brief Reads a delta-encoded dictionary from the message, applying changes to the target dictionary based on a base dictionary.

		This function reads a sequence of key-value pairs from the message and applies them to the target dictionary. If a base dictionary is provided, it is used as the initial state for the target
	   dictionary. The function first reads all the key-value pairs to be added or modified, then reads a sequence of keys to be deleted from the dictionary. The return value indicates whether any
	   changes were applied to the dictionary.

		\param dict The dictionary to be updated with the delta changes.
		\param base The base dictionary to use as the initial state, or null to start with an empty dictionary.
		\return True if the dictionary was modified by the operation, false otherwise.
	*/
	bool   ReadDeltaDict( idDict& dict, const idDict* base ) const;

	template<int _max_, int _numBits_>
	float ReadQuantizedFloat() const;

	/*!
		\brief Reads a quantized unsigned float value from the message stream

		This function reads a quantized unsigned float value from the message stream by first determining whether to scale up or down based on the maximum value. When scaling down, it calculates an
	   inverse scale factor and multiplies the read bits by this factor. When scaling up, it ensures the scale is a whole number and calculates an inverse scale factor to maintain precision. This
	   approach preserves whole numbers when possible while handling values that exceed the storage capacity.

		\return The decoded float value that was quantized and stored in the message stream
	*/
	template<int _max_, int _numBits_>
	float ReadQuantizedUFloat() const;

	/*!
		\brief Reads floating-point values from the message stream into the components of a vector

		This function reads a sequence of floating-point values from the message stream and assigns them to the components of the provided vector. The number of values read corresponds to the
	   dimensionality of the vector. Each value is read using the ReadFloat method, which extracts a single floating-point number from the message stream.

		\param v The vector whose components will be populated with floating-point values from the message stream
	*/
	template<typename T>
	void ReadVectorFloat( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadFloat();
		}
	}

	/*!
		\brief Reads normalized 8-bit unsigned integer values into a vector

		This function reads a sequence of normalized 8-bit unsigned integer values from the message and assigns them to the components of the provided vector. The number of components read is
	   determined by the dimension of the vector. Each component is read using the ReadUNorm8 method, which interprets the byte as an unsigned normalized fixed-point number in the range [0, 1].

		\param v The vector to be populated with normalized 8-bit unsigned integer values
	*/
	template<typename T>
	void ReadVectorUNorm8( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadUNorm8();
		}
	}

	/*!
		\brief Reads normalized 16-bit unsigned integer values into a vector

		This function reads a sequence of normalized 16-bit unsigned integer values from the message stream and assigns them to the components of the provided vector. The number of components read is
	   determined by the dimension of the vector. Each component is read using the ReadUNorm16 method which interprets the raw 16-bit value as an unsigned normalized fixed-point number in the range
	   [0, 1].

		\param v The vector to be populated with normalized 16-bit unsigned integer values
	*/
	template<typename T>
	void ReadVectorUNorm16( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadUNorm16();
		}
	}

	/*!
		\brief Reads normalized 16-bit values into a vector from the message

		This function reads a series of normalized 16-bit integer values from the message stream and assigns them to the corresponding elements of the provided vector. The number of elements read is
	   determined by the dimension of the vector. Each component is read using the ReadNorm16 method, which decodes the normalized 16-bit value.

		\param v The vector to read normalized 16-bit values into
	*/
	template<typename T>
	void ReadVectorNorm16( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadNorm16();
		}
	}

	/*!
		\brief Reads a quantized vector from the message

		This function reads a vector of floating-point values that have been quantized using a fixed-point representation. It iterates through each component of the vector and reads the quantized
	   value using the ReadQuantizedFloat template function.

		\param v The vector to read the quantized values into
	*/
	template<typename T, int _max_, int _numBits_>
	void ReadQuantizedVector( T& v ) const
	{
		for( int i = 0; i < v.GetDimension(); i++ ) {
			v[i] = ReadQuantizedFloat<_max_, _numBits_>();
		}
	}

	/*!
		\brief Converts a normalized 3D direction vector into a compact bit representation using the specified number of bits.

		This function encodes a normalized 3D direction vector into an integer bit pattern. The encoding uses a quantized approach where each component of the vector is mapped to a fixed number of
	   bits. The function first validates that the number of bits is between 6 and 32, and that the input vector is normalized. It then divides the total number of bits by 3 to allocate bits to each
	   component. The sign of each component is stored in the most significant bit position for that component, and the magnitude is quantized and scaled to fit within the remaining bits. The final
	   bit pattern is constructed by combining all three components into a single integer.

		\param dir The normalized 3D direction vector to encode
		\param numBits Total number of bits to use for encoding (must be between 6 and 32)
		\return An integer containing the compact bit representation of the direction vector
		\throws assertion failure if numBits is less than 6 or greater than 32, or if the input vector is not normalized
	*/
	static int	  DirToBits( const idVec3& dir, int numBits );

	/*!
		\brief Converts a bit field into a normalized 3D direction vector

		This function takes an integer bit field and converts it into a 3D direction vector by interpreting the bits as coordinates in a normalized space. The number of bits allocated to each
	   coordinate component is determined by dividing the total number of bits by 3. The function handles signed components by using the most significant bit of each component as the sign bit. The
	   resulting vector is normalized for unit length.

		\param bits The integer bit field containing the directional data
		\param numBits The total number of bits used for encoding the direction, must be between 6 and 32
		\return A normalized 3D vector representing the direction encoded in the bit field
		\throws Assertion error if numBits is less than 6 or greater than 32
	*/
	static idVec3 BitsToDir( int bits, int numBits );

	/*!
		\brief Sets the hasChanged flag to the specified boolean value.
		\param b The boolean value to set the hasChanged flag to
	*/
	void		  SetHasChanged( bool b ) { hasChanged = b; }

	//! Returns true if the message has changed since the last reset.
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
	/*!
		\brief Checks if writing the specified number of bits would cause an overflow in the message buffer.

		Returns false if there are sufficient bits remaining to write the specified number of bits. Returns true and sets the overflow flag if the write would exceed the buffer size. If overflow is
	   not allowed and an overflow occurs, the function will call a fatal error. If the number of bits exceeds the maximum message size, a fatal error is also called. In overflow cases, the message
	   buffer is reset to allow further writing.

		\param numBits The number of bits to check for potential overflow
		\return True if an overflow would occur, false otherwise.
		\throws Fatal error if overflow occurs and allowOverflow is not set, or if the number of bits exceeds the maximum message size.
	*/
	bool  CheckOverflow( int numBits );

	/*!
		\brief Returns a pointer to a byte space of the specified length within the message buffer

		This function allocates space within the message buffer for writing data. It first checks if writing is enabled, and if not, it causes a fatal error. It then ensures proper byte alignment
	   before checking for buffer overflow. The function returns a pointer to the allocated space, and updates the current size of the message buffer accordingly

		\param length The number of bytes to allocate space for in the message buffer
		\return A pointer to the allocated byte space in the message buffer
		\throws Fatal error if writing to the message is not enabled
	*/
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

/*!
	\brief Writes a quantized floating-point value to the message using bit-level compression.

	This function quantizes a floating-point value by scaling it based on the maximum allowed value and the number of bits available. It handles both cases where the scaling factor is less than or
   greater than one to preserve precision. The scaled value is then clamped and written as a signed integer using the WriteBits method.

	\param value The floating-point value to be quantized and written to the message
*/
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

/*!
	\brief Writes a quantized unsigned float value to the message using a specified number of bits.

	This function compresses a floating-point value into a fixed number of bits for efficient storage or transmission. It handles scaling of the input value based on the maximum representable value
   and the number of bits allocated. The function uses clamping to ensure the final value stays within valid bounds before writing the bits to the message buffer. The scaling factor adjusts depending
   on whether the maximum representable value is larger or smaller than the actual storage capacity.

	\param value The floating-point value to be quantized and written to the message
*/
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

/*!
	\brief Reads a quantized floating-point value from the bit stream

	This function reads a quantized floating-point value from the bit stream by first determining the appropriate scaling factor based on the maximum value and the number of bits available. If the
   maximum value is greater than the maximum representable value with the given number of bits, it scales down the result. Otherwise, it scales up the result to preserve whole numbers when possible.
   The scaling factor is calculated to ensure that the quantized values can represent the full range of the original data.

	\return The reconstructed floating-point value from the bit stream
*/
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

/*!
	\brief Writes all values from the input array to the bit message

	This function iterates through all elements of the input array and writes each element as a float to the provided bit message. It uses the array size determined by the idTupleSize template
   specialization for the array type

	\param message The bit message to write the array values to
	\param sourceArray The array containing float values to be written to the message
*/
template<class _arrayType_>
void WriteFloatArray( idBitMsg& message, const _arrayType_& sourceArray )
{
	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		message.WriteFloat( sourceArray[i] );
	}
}

/*!
	\brief Writes the delta of two float arrays to a bit message

	This function writes the differences between two arrays of floating point values to a bit message. It iterates through each element of the arrays and uses the WriteDeltaFloat method to efficiently
   encode the differences. The function assumes both arrays have the same size determined by the template parameter _arrayType_.

	\param message The bit message to write the delta data to
	\param oldArray The original array of floating point values
	\param newArray The new array of floating point values to compare against
*/
template<class _arrayType_>
void WriteDeltaFloatArray( idBitMsg& message, const _arrayType_& oldArray, const _arrayType_& newArray )
{
	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		message.WriteDeltaFloat( oldArray[i], newArray[i] );
	}
}

/*!
	\brief Reads a fixed-size array of floating-point values from a bit message

	This function reads a sequence of floating-point values from the provided bit message and stores them in a fixed-size array. The number of values read is determined by the template parameter
   _arrayType_ which defines the size of the array. Each value is read using the message.ReadFloat() method, which extracts a single floating-point number from the bit stream.

	\param message The bit message containing the serialized floating-point values
	\return An array of floating-point values read from the message
*/
template<class _arrayType_>
_arrayType_ ReadFloatArray( const idBitMsg& message )
{
	_arrayType_ result;

	for( int i = 0; i < idTupleSize<_arrayType_>::value; ++i ) {
		result[i] = message.ReadFloat();
	}

	return result;
}

/*!
	\brief Reads a delta-encoded float array from a bit message using the provided old array values as references.

	This function reads a sequence of delta-encoded float values from a bit message. Each value is read using the ReadDeltaFloat method which decodes the difference between the current and previous
   values. The function iterates through each element of the array, reads the delta-encoded value, and constructs a new array with the decoded values. The old array provides reference values for the
   delta decoding process.

	\param message The bit message containing the delta-encoded float values
	\param oldArray The reference array used for delta decoding of the float values
	\return A new array with the decoded float values read from the bit message
*/
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
