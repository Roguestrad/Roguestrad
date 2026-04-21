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

#ifndef __BASE64_H__
#define __BASE64_H__

/*!
	\class idBase64
	\brief A class for base64 encoding and decoding of binary data.

	The idBase64 class provides functionality for encoding binary data into base64 format and decoding base64 strings back into binary data. It manages an internal buffer to store encoded or decoded
   results and offers methods for both string and file-based operations. The class supports initialization, encoding from byte arrays or idStr objects, and decoding into byte buffers, idStr objects,
   or idFile objects. It includes utility methods to calculate required buffer sizes for decoding and handles memory management internally through EnsureAlloced, Init, and Release methods. The class
   is designed for use within the engine where base64 encoding is needed for data serialization or transmission, particularly when dealing with binary data that needs to be represented as
   text.

*/
class idBase64
{
public:
	//! Initializes a new instance of the idBase64 class.
	idBase64();

	/*!
		\brief Initializes a new idBase64 object with the given idStr string.
		\param s The idStr string to initialize the idBase64 object with
	*/
	idBase64( const idStr& s );

	/*!
		\brief Destructor for the idBase64 class that releases any allocated resources.

		This function is the destructor for the idBase64 class. It is responsible for releasing any memory or resources that were allocated during the object's lifetime. The destructor calls the
	   Release method to ensure proper cleanup of any internal data structures or buffers.

	*/
	~idBase64();

	/*!
		\brief Encodes a byte array into base64 format

		This function takes a byte array and its size as input and encodes it into base64 format. The encoded result is stored in an internal buffer managed by the class. The function handles padding
	   with '=' characters when the input size is not a multiple of 3. The output is null-terminated.

		\param from Pointer to the input byte array to be encoded
		\param size Size of the input byte array in bytes
	*/
	void		Encode( const byte* from, int size );

	/*!
		\brief Encodes the contents of a string using base64 encoding.

		This function takes a source string and encodes its contents into base64 format. It internally converts the string to a byte array and passes it to the overloaded Encode function that handles
	   the actual encoding process. The result is stored in the object's internal buffer.

		\param src The source string to be encoded using base64 encoding
	*/
	void		Encode( const idStr& src );

	/*!
		\brief Returns the minimum size in bytes of the destination buffer required for decoding a base64-encoded string.

		This function calculates the minimum size in bytes needed for the destination buffer when decoding a base64-encoded string. The calculation is based on the length of the encoded string, using
	   the formula 3 times the length divided by 4. This accounts for the fact that base64 encoding increases the size of the data by approximately 33%, so when decoding, the original size can be
	   estimated by taking three quarters of the encoded length.

		\return The minimum size in bytes required for the destination buffer to hold the decoded data
	*/
	int			DecodeLength() const;

	/*!
		\brief Decodes a base64-encoded string into the provided byte buffer

		This function decodes a base64-encoded string stored in the instance into a provided byte buffer. It handles whitespace and newline characters in the input, and processes the data in chunks of
	   four base64 characters to reconstruct the original bytes. The function does not append a null terminator to the output buffer and requires a buffer size equal to the value returned by
	   DecodeLength(). The decoding process uses a static lookup table for base64 character to sixtet conversion that is initialized on first use. The function returns the number of bytes written to
	   the output buffer

		\param to Output buffer where the decoded bytes will be written
		\return The number of bytes written to the output buffer
	*/
	int			Decode( byte* to ) const;

	/*!
		\brief Decodes the base64 encoded content into the provided destination string

		This function decodes the base64 encoded data stored in the idBase64 object into the provided destination string. The decoded data may contain null bytes and other non-ASCII characters. The
	   function allocates a temporary buffer to hold the decoded bytes and appends a null terminator before assigning the result to the destination string. The implementation handles the conversion
	   and memory management internally.

		\param dest The destination string to store the decoded binary content
	*/
	void		Decode( idStr& dest ) const;

	/*!
		\brief Decodes the base64 data stored in this object and writes the decoded bytes to the provided destination file.

		This function decodes the base64 encoded string stored in the idBase64 object and writes the resulting decoded bytes to the provided destination file. The function allocates a buffer large
	   enough to hold the decoded data plus a null terminator, performs the decoding, and writes the decoded data to the destination file. The buffer is automatically deallocated after use. The
	   destination file must be opened and ready for writing before calling this function. Note that a null terminator is automatically appended to the decoded data, which is why the stream length is
	   adjusted by -1 when reading back from the file.

		\param dest The destination file to write the decoded bytes to
	*/
	void		Decode( idFile* dest ) const;

	/*!
		\brief Returns a constant character pointer to the internal data of the base64 encoding

		This function provides access to the raw character data stored in the idBase64 object. It is a constant function that returns a const char pointer, allowing read-only access to the encoded
	   data. The returned pointer points to the internal storage of the base64 encoded string.

		\return A constant character pointer to the internal base64 encoded data
	*/
	const char* c_str() const;

	void		operator=( const idStr& s );

private:
	byte* data;
	int	  len;
	int	  alloced;

	/*!
		\brief Initializes the base64 encoding/decoding buffer state

		This function initializes the internal buffer state for base64 operations by setting the length and allocation size to zero and the data pointer to null

	*/
	void  Init();

	/*!
		\brief Releases the allocated memory for the base64 data and initializes the object state.

		This function releases the memory allocated for the base64 data by deleting the data array if it exists. It then calls the Init function to reset the object's internal state, preparing it for
	   potential reuse.

	*/
	void  Release();

	/*!
		\brief Ensures that the base64 encoding buffer has sufficient allocated space for the specified size

		This function checks if the current allocated buffer size is less than the requested size. If so, it releases the existing buffer and allocates a new buffer of the specified size. The function
	   is marked as inline for performance optimization.

		\param size The minimum number of bytes to allocate for the base64 encoding buffer
	*/
	void  EnsureAlloced( int size );
};

ID_INLINE idBase64::idBase64()
{
	Init();
}

ID_INLINE idBase64::idBase64( const idStr& s )
{
	Init();
	*this = s;
}

ID_INLINE idBase64::~idBase64()
{
	Release();
}

ID_INLINE const char* idBase64::c_str() const
{
	return ( const char* )data;
}

ID_INLINE void idBase64::Init()
{
	len		= 0;
	alloced = 0;
	data	= NULL;
}

ID_INLINE void idBase64::Release()
{
	if( data ) { delete[] data; }
	Init();
}

ID_INLINE void idBase64::EnsureAlloced( int size )
{
	if( size > alloced ) { Release(); }
	data	= new( TAG_IDLIB ) byte[size];
	alloced = size;
}

ID_INLINE void idBase64::operator=( const idStr& s )
{
	EnsureAlloced( s.Length() + 1 ); // trailing \0 - beware, this does a Release
	strcpy( ( char* )data, s.c_str() );
	len = s.Length();
}

#endif /* !__BASE64_H__ */
