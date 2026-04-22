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
#ifndef __STRSTATIC_H__
#define __STRSTATIC_H__

/*!
	\class idStrStatic
	\brief A fixed-size string class template that provides compile-time memory allocation for string data.

	This class extends the idStr base class to provide a string implementation with a statically allocated buffer of fixed size determined by the template parameter. The design enables efficient
   memory usage for strings with known maximum lengths, eliminating dynamic allocation overhead. The class supports various constructors for initialization from different data types and ranges,
   ensuring flexibility while maintaining the fixed-size constraint. Assignment operators and copy constructors are provided to support standard string operations within the fixed-size limitation. All
   string operations are constrained by the predefined buffer size, making it suitable for performance-critical scenarios where heap allocation should be avoided.

*/
template<int _size_>
class idStrStatic : public idStr
{
public:
	//! Assigns the contents of another idStrStatic object to this object.
	ID_INLINE void operator=( const idStrStatic& text )
	{
		// we should only get here when the types, including the size, are identical
		len = text.Length();
		memcpy( data, text.data, len + 1 );
	}

	//! Constructs an empty static string with a fixed-size buffer.
	ID_INLINE idStrStatic()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
	}

	//! Creates a copy of the provided idStrStatic object.
	ID_INLINE idStrStatic( const idStrStatic& text ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	//! Constructs a static string from another string object.
	ID_INLINE idStrStatic( const idStr& text ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	//! Constructs a new static string by copying a range of characters from another static string.
	ID_INLINE idStrStatic( const idStrStatic& text, int start, int end ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		CopyRange( text.c_str(), start, end );
	}

	//! Constructs an idStrStatic object with the specified text.
	ID_INLINE idStrStatic( const char* text ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	//! Initializes a static string with a substring from the given text.
	ID_INLINE idStrStatic( const char* text, int start, int end ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		CopyRange( text, start, end );
	}

	//! Constructs an idStrStatic object initialized with the string representation of a boolean value.
	ID_INLINE explicit idStrStatic( const bool b ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( b ) );
	}

	//! Constructs an idStrStatic object from a single character.
	ID_INLINE explicit idStrStatic( const char c ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( c ) );
	}

	//! Constructs a static string object initialized with the value of an integer.
	ID_INLINE explicit idStrStatic( const int i ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( i ) );
	}

	//! Constructs an idStrStatic object initialized with the string representation of the given unsigned integer.
	ID_INLINE explicit idStrStatic( const unsigned u ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( u ) );
	}

	//! Constructs a static string from a float value.
	ID_INLINE explicit idStrStatic( const float f ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( f ) );
	}

private:
	char buffer[_size_];
};
#endif // __STRSTATIC_H__
