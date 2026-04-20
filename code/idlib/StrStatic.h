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

/*
================================================
idStrStatic
================================================
*/
template<int _size_>
class idStrStatic : public idStr
{
public:
	ID_INLINE void operator=( const idStrStatic& text )
	{
		// we should only get here when the types, including the size, are identical
		len = text.Length();
		memcpy( data, text.data, len + 1 );
	}

	/*!
		\brief Initializes an idStrStatic object with a static buffer of specified size.

		This constructor initializes the static buffer of the idStrStatic object with a null terminator and sets the static buffer using the provided size. The constructor ensures that the static
	   buffer is properly initialized before any data is copied into it. This is done to avoid issues with uninitialized memory and to provide a consistent starting state for the string object.

	*/
	ID_INLINE idStrStatic()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
	}

	/*!
		\brief Constructs a new static string by copying the contents of another static string.

		This constructor initializes a new static string object by copying the contents of an existing static string. It sets up the internal buffer to use the static storage and performs the copy
	   operation using the base class assignment operator.

		\param text The existing static string to copy from
		\return A new static string object initialized with the contents of the input string
	*/
	ID_INLINE idStrStatic( const idStrStatic& text ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	/*!
		\brief Constructs a static string object by copying the content from another idStr object.

		This constructor initializes a static string object with a fixed-size buffer of size _size_. It sets the initial buffer to an empty string, configures the static buffer for the object, and
	   then copies the content from the provided idStr object.

		\param text The idStr object whose content will be copied to this static string.
		\return A newly constructed idStrStatic object with the content copied from the input idStr object.
	*/
	ID_INLINE idStrStatic( const idStr& text ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	/*!
		\brief Constructs a new static string by copying a range of characters from another static string.

		This constructor initializes a new static string object by copying a substring from the provided source string. The substring is defined by the start and end indices. The constructor uses a
	   static buffer of size _size_ for storage and initializes the buffer with a null terminator before copying the specified range of characters.

		\param text The source static string from which to copy characters
		\param start The starting index of the substring to copy
		\param end The ending index of the substring to copy
		\return ID_INLINE
	*/
	ID_INLINE idStrStatic( const idStrStatic& text, int start, int end ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		CopyRange( text.c_str(), start, end );
	}

	/*!
		\brief Constructs an idStrStatic object with the specified text, using a static buffer of fixed size.

		The constructor initializes the idStrStatic object by setting up a static buffer of size _size_ and then assigns the provided text to the string. The buffer is initialized to an empty string,
	   and the static buffer is configured before the text assignment occurs.

		\param text The null-terminated character string to initialize the idStrStatic object with
		\return ID_INLINE
	*/
	ID_INLINE idStrStatic( const char* text ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	/*!
		\brief Initializes a static string with a substring of the provided text.

		Constructs a static string by copying a range of characters from the input text. The range is defined by the start and end indices. The buffer used for the string is statically allocated with
	   a fixed size determined by the template parameter _size_.

		\param text The input character array from which to copy the substring.
		\param start The starting index of the substring in the input text.
		\param end The ending index of the substring in the input text.
		\return This constructor does not return a value.
	*/
	ID_INLINE idStrStatic( const char* text, int start, int end ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		CopyRange( text, start, end );
	}

	/*!
		\brief Constructs a static string from a boolean value.

		Initializes a static string buffer with the specified size and assigns the string representation of the given boolean value to it. The boolean value is converted to a string representation
	   using the base class operator= method.

		\param b The boolean value to convert to a string
	*/
	ID_INLINE explicit idStrStatic( const bool b ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( b ) );
	}

	/*!
		\brief Constructs a static string from a single character.

		This constructor initializes a static string object with a single character. It sets up the internal buffer for static strings and assigns the character to the string, ensuring proper memory
	   management for static string buffers.

		\param c The character to initialize the static string with
		\return TODO: clarify return value description
	*/
	ID_INLINE explicit idStrStatic( const char c ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( c ) );
	}

	/*!
		\brief Constructs a static string from an integer value.

		This constructor initializes a static string by converting the provided integer value to a string representation. It uses a fixed-size buffer to store the string data. The buffer is explicitly
	   set using the SetStaticBuffer method, and the string content is assigned using the base class assignment operator.

		\param i The integer value to convert to a string
		\return ID_INLINE
	*/
	ID_INLINE explicit idStrStatic( const int i ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( i ) );
	}

	/*!
		\brief Constructs an idStrStatic object initialized with the string representation of the given unsigned integer.

		The constructor initializes the object by first setting up a static buffer of size _size_ and then assigning the string representation of the provided unsigned integer to the object. The idStr
	   base class is initialized first, followed by setting the static buffer, and finally the assignment of the string representation of the unsigned integer.

		\param u The unsigned integer to convert to a string and initialize the object with
		\return ID_INLINE
	*/
	ID_INLINE explicit idStrStatic( const unsigned u ) :
		idStr()
	{
		buffer[0] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( u ) );
	}

	/*!
		\brief Constructs a static string representation of a floating-point number.

		This constructor initializes a static string buffer with the provided floating-point value. It sets up the internal buffer and assigns the string representation of the float to this buffer.
	   The buffer size is determined by the template parameter _size_.

		\param f The floating-point number to convert to a string
		\return ID_INLINE
	*/
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
