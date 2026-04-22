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

#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
===============================================================================

	idToken is a token read from a file or memory with idLexer or idParser

===============================================================================
*/

// token types
#define TT_STRING			  1 // string
#define TT_LITERAL			  2 // literal
#define TT_NUMBER			  3 // number
#define TT_NAME				  4 // name
#define TT_PUNCTUATION		  5 // punctuation

// number sub types
#define TT_INTEGER			  0x00001 // integer
#define TT_DECIMAL			  0x00002 // decimal number
#define TT_HEX				  0x00004 // hexadecimal number
#define TT_OCTAL			  0x00008 // octal number
#define TT_BINARY			  0x00010 // binary number
#define TT_LONG				  0x00020 // long int
#define TT_UNSIGNED			  0x00040 // unsigned int
#define TT_FLOAT			  0x00080 // floating point number
#define TT_SINGLE_PRECISION	  0x00100 // float
#define TT_DOUBLE_PRECISION	  0x00200 // double
#define TT_EXTENDED_PRECISION 0x00400 // long double
#define TT_INFINITE			  0x00800 // infinite 1.#INF
#define TT_INDEFINITE		  0x01000 // indefinite 1.#IND
#define TT_NAN				  0x02000 // NaN
#define TT_IPADDRESS		  0x04000 // ip address
#define TT_IPPORT			  0x08000 // ip port
#define TT_VALUESVALID		  0x10000 // set if intvalue and floatvalue are valid

/*!
	\class idToken
	\brief The idToken class represents a tokenized element with associated metadata such as type, value, and whitespace information.

	The idToken class extends idStr to provide token-specific functionality including type classification, value extraction, and whitespace tracking. It is designed to handle various token types such
   as numbers, strings, punctuation, and names, while maintaining compatibility with string operations. The class supports direct assignment from string types and provides methods to retrieve numeric
   values in different formats. It also maintains information about whitespace preceding the token and allows for manipulation of the underlying string data. The class is intended to be used in
   parsing scenarios where tokens need to be examined for their type and associated values.

*/
class idToken : public idStr
{
	friend class idParser;
	friend class idLexer;

public:
	int type;		  // token type
	int subtype;	  // token sub type
	int line;		  // line in script the token was on
	int linesCrossed; // number of lines crossed in white space before token
	int flags;		  // token flags, used for recursive defines

public:
	//! Initializes a new idToken instance with default values for all members.
	idToken();

	//! Creates a copy of the provided token object.
	idToken( const idToken* token );
	~idToken();

	//! Assigns the contents of an idStr object to this token.
	void		  operator=( const idStr& text );

	//! Assigns a character string to this token
	void		  operator=( const char* text );

	//! Returns the double value of a TT_NUMBER token.
	double		  GetDoubleValue();

	//! Returns the float value of a token that represents a number.
	float		  GetFloatValue();

	//! Returns the unsigned long value of a token of type TT_NUMBER
	unsigned long GetUnsignedLongValue();

	//! Returns the integer value of a token that represents a number
	int			  GetIntValue();

	//! Returns the length of whitespace before the token.
	int			  WhiteSpaceBeforeToken() const;

	//! Clears the whitespace information stored in the token.
	void		  ClearTokenWhiteSpace();

	//! Calculates and stores the floating-point and integer values for a token of type TT_NUMBER
	void		  NumberValue();

private:
	unsigned int intvalue;			// DG: use int instead of long for 64bit compatibility
	double		 floatvalue;		// floating point value
	const char*	 whiteSpaceStart_p; // start of white space before token, only used by idLexer
	const char*	 whiteSpaceEnd_p;	// end of white space before token, only used by idLexer
	idToken*	 next;				// next token in chain, only used by idParser

	//! Appends a character to the token data without adding a trailing zero.
	void		 AppendDirty( const char a );
};

ID_INLINE idToken::idToken() :
	type(),
	subtype(),
	line(),
	linesCrossed(),
	flags()
{
}

ID_INLINE idToken::idToken( const idToken* token )
{
	*this = *token;
}

ID_INLINE idToken::~idToken()
{
}

ID_INLINE void idToken::operator=( const char* text )
{
	*static_cast<idStr*>( this ) = text;
}

ID_INLINE void idToken::operator=( const idStr& text )
{
	*static_cast<idStr*>( this ) = text;
}

ID_INLINE double idToken::GetDoubleValue()
{
	if( type != TT_NUMBER ) { return 0.0; }
	if( !( subtype & TT_VALUESVALID ) ) { NumberValue(); }
	return floatvalue;
}

ID_INLINE float idToken::GetFloatValue()
{
	return ( float )GetDoubleValue();
}

ID_INLINE unsigned long idToken::GetUnsignedLongValue()
{
	if( type != TT_NUMBER ) { return 0; }
	if( !( subtype & TT_VALUESVALID ) ) { NumberValue(); }
	return intvalue;
}

ID_INLINE int idToken::GetIntValue()
{
	return ( int )GetUnsignedLongValue();
}

ID_INLINE int idToken::WhiteSpaceBeforeToken() const
{
	return ( whiteSpaceEnd_p > whiteSpaceStart_p );
}

ID_INLINE void idToken::AppendDirty( const char a )
{
	EnsureAlloced( len + 2, true );
	data[len++] = a;
}

#endif /* !__TOKEN_H__ */
