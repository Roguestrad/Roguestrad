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
#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#define SERIALIZE_BOOL( ser, x )			( ( x ) = ser.SerializeBoolNonRef( x ) )
#define SERIALIZE_ENUM( ser, x, type, max ) ( ( x ) = ( type )ser.SerializeUMaxNonRef( x, max ) )
#define SERIALIZE_CVAR_FLOAT( ser, cvar ) \
	{                                     \
		float a = cvar.GetFloat();        \
		ser.Serialize( a );               \
		cvar.SetFloat( a );               \
	}
#define SERIALIZE_CVAR_INT( ser, cvar ) \
	{                                   \
		int a = cvar.GetInteger();      \
		ser.Serialize( a );             \
		cvar.SetInteger( a );           \
	}
#define SERIALIZE_CVAR_BOOL( ser, cvar ) \
	{                                    \
		bool a = cvar.GetBool();         \
		SERIALIZE_BOOL( ser, a );        \
		cvar.SetBool( a );               \
	}

#define SERIALIZE_MATX( ser, var )                           \
	{                                                        \
		int rows = var.GetNumRows();                         \
		int cols = var.GetNumColumns();                      \
		ser.Serialize( rows );                               \
		ser.Serialize( cols );                               \
		if( ser.IsReading() ) { var.SetSize( rows, cols ); } \
		for( int y = 0; y < rows; y++ ) {                    \
			for( int x = 0; x < rows; x++ ) {                \
				ser.Serialize( var[x][y] );                  \
			}                                                \
		}                                                    \
	}

#define SERIALIZE_VECX( ser, var )                     \
	{                                                  \
		int size = var.GetSize();                      \
		ser.Serialize( size );                         \
		if( ser.IsReading() ) { var.SetSize( size ); } \
		for( int x = 0; x < size; x++ ) {              \
			ser.Serialize( var[x] );                   \
		}                                              \
	}

#define SERIALIZE_JOINT( ser, var )                                                    \
	{                                                                                  \
		uint16 jointIndex = ( var == NULL_JOINT_INDEX ) ? 65535 : var;                 \
		ser.Serialize( jointIndex );                                                   \
		var = ( jointIndex == 65535 ) ? NULL_JOINT_INDEX : ( jointIndex_t )jointIndex; \
	}

// #define ENABLE_SERIALIZE_CHECKPOINTS
// #define SERIALIZE_SANITYCHECK
// #define SERIALIZE_NO_QUANT

#define SERIALIZE_CHECKPOINT( ser ) ser.SerializeCheckpoint( __FILE__, __LINE__ );

/*!
	\class idSerializer
	\brief A serializer for handling data serialization and deserialization with various data type support and optimization techniques.

	The idSerializer provides a comprehensive interface for serializing and deserializing different data types to and from a message buffer. It supports both reading and writing modes, with
   specialized methods for handling integers, floating-point numbers, vectors, matrices, strings, and boolean values. The serializer includes functionality for quantization, delta encoding, and packed
   integer serialization to optimize data transmission. It also offers methods for handling lists and checkpoints to ensure data integrity during serialization processes. The class is designed to work
   with bit-level messaging systems and provides utilities for various optimization techniques such as compression and precision control.

*/
class idSerializer
{
public:
	//! Constructs an idSerializer object with the specified message buffer and writing mode.
	idSerializer( idBitMsg& msg_, bool writing_ ) :
		writing( writing_ ),
		msg( &msg_ )
#ifdef SERIALIZE_SANITYCHECK
		,
		magic( 0 )
#endif
	{
	}

	//! Returns true if the serializer is in reading mode, false if in writing mode.
	bool IsReading() { return !writing; }

	//! Returns true if the serializer is in write mode.
	bool IsWriting() { return writing; }

	//! Serializes an integer value within a specified range, supporting signed types.
	void SerializeRange( int& value, int minSize, int maxSize )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteBits( value - minSize, idMath::BitsForInteger( maxSize - minSize ) );
		} else {
			value = minSize + msg->ReadBits( idMath::BitsForInteger( maxSize - minSize ) );
		}
		assert( value >= minSize && value <= maxSize );
	}

	//! Serializes an unsigned integer value using a maximum size constraint.
	void SerializeUMax( int& value, int maxSize )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteBits( value, idMath::BitsForInteger( maxSize ) );
		} else {
			value = msg->ReadBits( idMath::BitsForInteger( maxSize ) );
		}
		assert( value <= maxSize );
	}

	//! Serializes an unsigned integer value within the specified maximum size range.
	int SerializeUMaxNonRef( int value, int maxSize )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteBits( value, idMath::BitsForInteger( maxSize ) );
		} else {
			value = msg->ReadBits( idMath::BitsForInteger( maxSize ) );
		}
		assert( value <= maxSize );
		return value;
	}

	// void SerializeBitMsg( idBitMsg & inOutMsg, int numBytes ) { SanityCheck(); if ( writing ) { msg->WriteBitMsg( inOutMsg, numBytes ); } else { msg->ReadBitMsg( inOutMsg, numBytes ); } }

	// this is still needed to compile Rage code
	void SerializeBytes( void* bytes, int numBytes )
	{
		SanityCheck();
		for( int i = 0; i < numBytes; i++ ) {
			Serialize( ( ( uint8* )bytes )[i] );
		}
	};

	//! Serializes a boolean value for reading or writing.
	bool SerializeBoolNonRef( bool value )
	{
		SanityCheck(); // We return a value so we can support bit fields (can't pass by reference)
		if( writing ) {
			msg->WriteBool( value );
		} else {
			value = msg->ReadBool();
		}
		return value;
	}

#ifdef SERIALIZE_NO_QUANT
	template<int _max_, int _numBits_>
	void SerializeQ( idVec3& value )
	{
		Serialize( value );
	}
	template<int _max_, int _numBits_>
	void SerializeQ( float& value )
	{
		Serialize( value );
	}
	template<int _max_, int _numBits_>
	void SerializeUQ( float& value )
	{
		Serialize( value );
	}
	void SerializeQ( idMat3& axis, int bits = 15 ) { Serialize( axis ); }
#else

	//! Serializes a vector using quantization with a specified maximum value and number of bits.
	template<int _max_, int _numBits_>
	void SerializeQ( idVec3& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteQuantizedVector<idVec3, _max_, _numBits_>( value );
		} else {
			msg->ReadQuantizedVector<idVec3, _max_, _numBits_>( value );
		}
	}

	//! Serializes a floating-point value using quantization.
	template<int _max_, int _numBits_>
	void SerializeQ( float& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteQuantizedFloat<_max_, _numBits_>( value );
		} else {
			value = msg->ReadQuantizedFloat<_max_, _numBits_>();
		}
	}

	//! Serializes a quantized unsigned float value for writing or reading.
	template<int _max_, int _numBits_>
	void SerializeUQ( float& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteQuantizedUFloat<_max_, _numBits_>( value );
		} else {
			value = msg->ReadQuantizedUFloat<_max_, _numBits_>();
		}
	}

	//! Serializes a 3x3 matrix as a quaternion using bit-level compression with configurable precision.
	void SerializeQ( idMat3& axis, int bits = 15 );
#endif

	//! Serializes a 3x3 matrix by serializing each of its row vectors.
	void Serialize( idMat3& axis );

	//! Serializes a 3x3 matrix using a compressed quaternion representation.
	void SerializeC( idMat3& axis );

	//! Serializes an integer using a packed format that is more efficient for small values.
	template<typename _type_>
	void SerializeListElement( const idList<_type_*>& list, const _type_*& element );

	void SerializePacked( int& original );

	//! Serializes a signed integer using a packed format that encodes the sign in the first byte
	void SerializeSPacked( int& original );

	//! Serializes a string to or from a message buffer
	void SerializeString( char* s, int bufferSize )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteString( s );
		} else {
			msg->ReadString( s, bufferSize );
		}
	}

	//! Serializes a string by writing it to or reading it from a message buffer depending on the serialization mode.
	void SerializeString( idStr& s )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteString( s );
		} else {
			msg->ReadString( s );
		}
	}

	//! Serializes a delta-encoded integer value relative to a base value.
	void SerializeDelta( int32& value, const int32& base )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteDeltaLong( base, value );
		} else {
			value = msg->ReadDeltaLong( base );
		}
	}

	//! Serializes a16-bit integer value as a delta from a base value
	void SerializeDelta( int16& value, const int16& base )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteDeltaShort( base, value );
		} else {
			value = msg->ReadDeltaShort( base );
		}
	}

	//! Serializes a signed 8-bit integer value as a delta relative to a base value
	void SerializeDelta( int8& value, const int8& base )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteDeltaChar( base, value );
		} else {
			value = msg->ReadDeltaChar( base );
		}
	}

	//! Serializes a delta-encoded 16-bit unsigned integer value relative to a base value.
	void SerializeDelta( uint16& value, const uint16& base )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteDeltaUShort( base, value );
		} else {
			value = msg->ReadDeltaUShort( base );
		}
	}

	//! Serializes a byte value as a delta from a base value.
	void SerializeDelta( uint8& value, const uint8& base )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteDeltaByte( base, value );
		} else {
			value = msg->ReadDeltaByte( base );
		}
	}

	//! Serializes a floating-point value as a delta from a base value.
	void SerializeDelta( float& value, const float& base )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteDeltaFloat( base, value );
		} else {
			value = msg->ReadDeltaFloat( base );
		}
	}

	//! Serializes a 64-bit integer value for reading or writing.
	void Serialize( int64& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteLongLong( value );
		} else {
			value = msg->ReadLongLong();
		}
	}

	//! Serializes a 64-bit unsigned integer value for reading from or writing to a message buffer
	void Serialize( uint64& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteLongLong( value );
		} else {
			value = msg->ReadLongLong();
		}
	}

	//! Serializes a 32-bit integer value by reading from or writing to a message buffer.
	void Serialize( int32& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteLong( value );
		} else {
			value = msg->ReadLong();
		}
	}

	//! Serializes a 32-bit unsigned integer value by reading from or writing to a message buffer.
	void Serialize( uint32& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteLong( value );
		} else {
			value = msg->ReadLong();
		}
	}

	//! Serializes a 16-bit signed integer value by reading from or writing to a message buffer.
	void Serialize( int16& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteShort( value );
		} else {
			value = msg->ReadShort();
		}
	}

	//! Serializes a 16-bit unsigned integer value by reading from or writing to a message buffer.
	void Serialize( uint16& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteUShort( value );
		} else {
			value = msg->ReadUShort();
		}
	}

	//! Serializes a single byte value to or from a message buffer.
	void Serialize( uint8& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteByte( value );
		} else {
			value = msg->ReadByte();
		}
	}

	//! Serializes a signed 8-bit integer value to or from a message buffer
	void Serialize( int8& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteChar( value );
		} else {
			value = msg->ReadChar();
		}
	}

	//! Serializes a boolean value to or from a message buffer
	void Serialize( bool& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteByte( value ? 1 : 0 );
		} else {
			value = msg->ReadByte() != 0;
		}
	}

	//! Serializes a float value by reading from or writing to a message buffer
	void Serialize( float& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteFloat( value );
		} else {
			value = msg->ReadFloat();
		}
	}

	//! Serializes a random number generator seed into or from a message stream.
	void Serialize( idRandom2& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteLong( value.GetSeed() );
		} else {
			value.SetSeed( msg->ReadLong() );
		}
	}

	//! Serializes a 3D vector value by reading from or writing to a message buffer
	void Serialize( idVec3& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteVectorFloat( value );
		} else {
			msg->ReadVectorFloat( value );
		}
	}

	//! Serializes a 2D vector value by reading from or writing to a message buffer
	void Serialize( idVec2& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteVectorFloat( value );
		} else {
			msg->ReadVectorFloat( value );
		}
	}

	//! Serializes a 6-dimensional vector value by writing or reading it from a message buffer.
	void Serialize( idVec6& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteVectorFloat( value );
		} else {
			msg->ReadVectorFloat( value );
		}
	}

	//! Serializes a 4-dimensional vector value by reading from or writing to a message buffer
	void Serialize( idVec4& value )
	{
		SanityCheck();
		if( writing ) {
			msg->WriteVectorFloat( value );
		} else {
			msg->ReadVectorFloat( value );
		}
	}

	//! Serializes an angle normalized to 0-360 degrees using 16-bit quantization
	void SerializeAngle( float& value )
	{
		SanityCheck();
		if( writing ) {
			float nAngle = idMath::AngleNormalize360( value );
			assert( nAngle >= 0.0f ); // should never get a negative angle
			uint16 sAngle = nAngle * ( 65536.0f / 360.0f );
			msg->WriteUShort( sAngle );
		} else {
			uint16 sAngle = msg->ReadUShort();
			value		  = sAngle * ( 360.0f / 65536.0f );
		}
	}

	//! Performs a sanity check during serialization by validating magic numbers.
	void SanityCheck()
	{
#ifdef SERIALIZE_SANITYCHECK
		if( writing ) {
			msg->WriteUShort( 0xCCCC );
			msg->WriteUShort( magic );
		} else {
			int cccc = msg->ReadUShort();
			int m	 = msg->ReadUShort();
			assert( cccc == 0xCCCC );
			assert( m == magic );
			if( cccc != 0xCCCC ) { idLib::Error( "idSerializer::SanityCheck - cccc != 0xCCCC" ); }
			if( m != magic ) { idLib::Error( "idSerializer::SanityCheck - m != magic" ); }
		}
		magic++;
#endif
	}

	//! Serializes a checkpoint tag to verify serialization integrity at the specified file and line.
	void SerializeCheckpoint( const char* file, int line )
	{
#ifdef ENABLE_SERIALIZE_CHECKPOINTS
		const uint32 tagValue = 0xABADF00D;
		uint32		 tag	  = tagValue;
		Serialize( tag );
		if( tag != tagValue ) { idLib::Error( "SERIALIZE_CHECKPOINT: tag != tagValue (file: %s - line: %i)", file, line ); }
#endif
	}

	//! Returns a reference to the internal idBitMsg object used for serialization
	idBitMsg& GetMsg() { return *msg; }

private:
	bool	  writing;
	idBitMsg* msg;
#ifdef SERIALIZE_SANITYCHECK
	int magic;
#endif
};

/*!
	\class idSerializerScopedBlock
	\brief A scoped block for managing serialization operations with a specified maximum size.
*/
class idSerializerScopedBlock
{
public:
	//! Initializes a scoped block for serialization with a specified maximum size.
	idSerializerScopedBlock( idSerializer& ser_, int maxSizeBytes_ )
	{
		ser			 = &ser_;
		maxSizeBytes = maxSizeBytes_;

		startByte	   = ser->IsReading() ? ser->GetMsg().GetReadCount() : ser->GetMsg().GetSize();
		startWriteBits = ser->GetMsg().GetWriteBit();
	}

	//! Destroys the scoped block and ensures proper serialization of remaining bits and bytes.
	~idSerializerScopedBlock()
	{
		// Serialize remaining bits
		while( ser->GetMsg().GetWriteBit() != startWriteBits ) {
			ser->SerializeBoolNonRef( false );
		}

		// Verify we didn't go over
		int endByte	  = ser->IsReading() ? ser->GetMsg().GetReadCount() : ser->GetMsg().GetSize();
		int sizeBytes = endByte - startByte;
		if( !verify( sizeBytes <= maxSizeBytes ) ) {
			idLib::Warning( "idSerializerScopedBlock went over maxSize (%d > %d)", sizeBytes, maxSizeBytes );
			return;
		}

		// Serialize remaining bytes
		uint8 b = 0;
		while( sizeBytes < maxSizeBytes ) {
			ser->Serialize( b );
			sizeBytes++;
		}

		int finalSize = ( ( ser->IsReading() ? ser->GetMsg().GetReadCount() : ser->GetMsg().GetSize() ) - startByte );
		verify( maxSizeBytes == finalSize );
	}

private:
	idSerializer* ser;
	int			  maxSizeBytes;

	int			  startByte;
	int			  startWriteBits;
};

/*
========================
idSerializer::SerializeQ
========================
*/
#ifndef SERIALIZE_NO_QUANT
ID_INLINE void idSerializer::SerializeQ( idMat3& axis, int bits )
{
	SanityCheck();

	const float scale = ( ( 1 << ( bits - 1 ) ) - 1 );
	if( IsWriting() ) {
		idQuat quat = axis.ToQuat();

		int	   maxIndex = 0;
		for( unsigned int i = 1; i < 4; i++ ) {
			if( idMath::Fabs( quat[i] ) > idMath::Fabs( quat[maxIndex] ) ) { maxIndex = i; }
		}

		msg->WriteBits( maxIndex, 2 );

		idVec3 out;

		if( quat[maxIndex] < 0.0f ) {
			out.x = -quat[( maxIndex + 1 ) & 3];
			out.y = -quat[( maxIndex + 2 ) & 3];
			out.z = -quat[( maxIndex + 3 ) & 3];
		} else {
			out.x = quat[( maxIndex + 1 ) & 3];
			out.y = quat[( maxIndex + 2 ) & 3];
			out.z = quat[( maxIndex + 3 ) & 3];
		}
		msg->WriteBits( idMath::Ftoi( out.x * scale ), -bits );
		msg->WriteBits( idMath::Ftoi( out.y * scale ), -bits );
		msg->WriteBits( idMath::Ftoi( out.z * scale ), -bits );

	} else if( IsReading() ) {
		idQuat quat;
		idVec3 in;

		int	   maxIndex = msg->ReadBits( 2 );

		in.x = ( float )msg->ReadBits( -bits ) / scale;
		in.y = ( float )msg->ReadBits( -bits ) / scale;
		in.z = ( float )msg->ReadBits( -bits ) / scale;

		quat[( maxIndex + 1 ) & 3] = in.x;
		quat[( maxIndex + 2 ) & 3] = in.y;
		quat[( maxIndex + 3 ) & 3] = in.z;

		quat[maxIndex] = idMath::Sqrt( idMath::Fabs( 1.0f - in.x * in.x - in.y * in.y - in.z * in.z ) );

		axis = quat.ToMat3();
	}
}
#endif

ID_INLINE void idSerializer::Serialize( idMat3& axis )
{
	SanityCheck();

	Serialize( axis[0] );
	Serialize( axis[1] );
	Serialize( axis[2] );
}

ID_INLINE void idSerializer::SerializeC( idMat3& axis )
{
	SanityCheck();

	if( IsWriting() ) {
		idCQuat cquat = axis.ToCQuat();

		Serialize( cquat.x );
		Serialize( cquat.y );
		Serialize( cquat.z );
	} else if( IsReading() ) {
		idCQuat cquat;

		Serialize( cquat.x );
		Serialize( cquat.y );
		Serialize( cquat.z );

		axis = cquat.ToMat3();
	}
}

//! Serializes a list element by writing its index during writing or reading it back during reading
template<typename _type_>
ID_INLINE void idSerializer::SerializeListElement( const idList<_type_*>& list, const _type_*& element )
{
	SanityCheck();

	if( IsWriting() ) {
		int index = list.FindIndex( const_cast<_type_*>( element ) );
		assert( index >= 0 );
		SerializePacked( index );
	} else if( IsReading() ) {
		int index = 0;
		SerializePacked( index );
		element = list[index];
	}
}

ID_INLINE void idSerializer::SerializePacked( int& original )
{
	SanityCheck();

	if( IsWriting() ) {
		uint32 value = original;

		while( true ) {
			uint8 byte = value & 0x7F;
			value >>= 7;
			byte |= value ? 0x80 : 0;
			msg->WriteByte( byte ); // Emit byte
			if( value == 0 ) { break; }
		}
	} else {
		uint8  byte	 = 0x80;
		uint32 value = 0;
		int32  shift = 0;

		while( byte & 0x80 && shift < 32 ) {
			byte = msg->ReadByte();
			value |= ( byte & 0x7F ) << shift;
			shift += 7;
		}

		original = value;
	}
}

ID_INLINE void idSerializer::SerializeSPacked( int& value )
{
	SanityCheck();

	if( IsWriting() ) {
		uint32 uvalue = idMath::Abs( value );

		// Write the first byte specifically to handle the sign bit
		uint8  byte = uvalue & 0x3f;
		byte |= value < 0 ? 0x40 : 0;
		uvalue >>= 6;
		byte |= uvalue > 0 ? 0x80 : 0;

		msg->WriteByte( byte );

		while( uvalue > 0 ) {
			uint8 byte2 = uvalue & 0x7F;
			uvalue >>= 7;
			byte2 |= uvalue ? 0x80 : 0;
			msg->WriteByte( byte2 ); // Emit byte
		}
	} else {
		// Load the first byte specifically to handle the sign bit
		uint8  byte	  = msg->ReadByte();
		uint32 uvalue = byte & 0x3f;
		bool   sgn	  = ( byte & 0x40 ) ? true : false;
		int32  shift  = 6;

		while( byte & 0x80 && shift < 32 ) {
			byte = msg->ReadByte(); // Read byte
			uvalue |= ( byte & 0x7F ) << shift;
			shift += 7;
		}

		value = sgn ? -( ( int )uvalue ) : uvalue;
	}
}

#endif
