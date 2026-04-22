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
	\brief A class for encoding and decoding base64 data with support for various output formats.

	The idBase64 class provides functionality for encoding binary data into base64 format and decoding base64 data back into binary form. It supports multiple output destinations including memory
   buffers, strings, and file objects. The class maintains an internal buffer that can be managed through explicit allocation functions or automatically through the encoding process. Construction can
   be done with or without initial data, and the class provides methods to initialize, release, and ensure buffer allocation. The design allows for flexible usage patterns where data can be encoded
   from byte arrays or strings, and decoded into various target formats. The implementation handles the base64 encoding and decoding algorithm internally, managing the conversion between binary and
   textual representations.

*/
class idBase64
{
public:
	//! Initializes a new instance of the idBase64 class.
	idBase64();

	//! Constructs an idBase64 object and initializes it with the provided idStr string.
	idBase64( const idStr& s );

	//! Destroys the idBase64 object and releases any allocated resources.
	~idBase64();

	//! Encodes a byte array into base64 format.
	void		Encode( const byte* from, int size );

	//! Encodes a string using Base64 encoding.
	void		Encode( const idStr& src );

	//! Returns the minimum size in bytes of the destination buffer needed for decoding a base64 encoded string.
	int			DecodeLength() const;

	//! Decodes Base64-encoded data into a provided buffer and returns the number of decoded bytes.
	int			Decode( byte* to ) const;

	//! Decodes base64-encoded data into the provided destination string.
	void		Decode( idStr& dest ) const;

	//! Decodes base64 data into the provided destination file.
	void		Decode( idFile* dest ) const;

	//! Returns a constant character pointer to the internal data of the base64 string.
	const char* c_str() const;

	//! Assigns the contents of a string to this base64 object.
	void		operator=( const idStr& s );

private:
	byte* data;
	int	  len;
	int	  alloced;

	//! Initializes the base64 encoder/decoder state
	void  Init();

	//! Releases any allocated resources and initializes the object to its default state
	void  Release();

	//! Ensures the base64 encoding buffer is allocated with at least the specified size.
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
