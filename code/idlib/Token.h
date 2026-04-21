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
	\brief idToken represents a parsed token with numeric value conversion capabilities within the Doom 3 BFG engine's lexer.

	The idToken class extends idStr to provide specialized functionality for handling parsed tokens from the engine's lexical analyzer. It maintains token type information, whitespace data, and
   numeric representations of number tokens. The class is designed for efficient inline operations during parsing, with constructors for copying and assignment operators for string-based
   initialization. Key features include methods for converting tokens to various numeric types (integer, float, double, unsigned long) with appropriate validation. The class supports whitespace
   tracking and manipulation, allowing it to accurately represent tokens as they appear in source files, including any leading whitespace. The NumberValue method handles complex numeric conversions
   for different number formats, while ClearTokenWhiteSpace resets whitespace tracking information for tokens that may be reused. This design enables robust parsing of source data, particularly useful
   in the engine's collision model loading and other text-based data processing systems.

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
	/*!
		\brief Initializes a new instance of the idToken class with default values for all its member fields.

		This constructor initializes all member variables of the idToken class to their default values. It uses an initializer list to set type, subtype, line, linesCrossed, and flags to their default
	   states. The constructor is marked as ID_INLINE, indicating it should be inlined for performance reasons.

	*/
	idToken();

	/*!
		\brief Initializes a new idToken by copying the contents of an existing idToken.

		This constructor creates a new idToken instance by performing a direct copy of the provided token.
		The implementation simply assigns the value of the input token to this token using the assignment operator.
		This is a inline constructor, typically used for efficient copying of token objects within the Doom 3 BFG engine's tokenization system.

		\param token Pointer to the idToken object to be copied
	*/
	idToken( const idToken* token );
	~idToken();

	void		  operator=( const idStr& text );
	void		  operator=( const char* text );

	/*!
		\brief Returns the double precision floating point value of a token that must be of type TT_NUMBER.

		This function retrieves the double precision floating point value from a token that is expected to be of type TT_NUMBER. If the token is not of type TT_NUMBER, the function returns 0.0. If the
	   token's subtype does not indicate that the values are valid, the function calls NumberValue() to compute the value. The function then returns the computed floatvalue member of the token.

		\return The double precision floating point value of the token if it is of type TT_NUMBER, otherwise 0.0.
	*/
	double		  GetDoubleValue();

	/*!
		\brief Returns the float value of a token that contains a numeric value

		This function retrieves the floating point value from a token that was previously parsed from input text. It is designed to convert the internal double precision value stored in the token to a
	   single precision float. The function assumes that the token has already been validated as containing a numeric value and is typically used in parsing contexts where numeric literals are
	   expected. The implementation simply casts the result of GetDoubleValue to a float.

		\return The floating point representation of the numeric value stored in the token
	*/
	float		  GetFloatValue();

	/*!
		\brief Returns the unsigned long value of a token that must be of type TT_NUMBER

		This function retrieves the unsigned long value from a token that has been identified as a number token. It first checks if the token type is TT_NUMBER, and if not, it returns 0. If the
	   token's subtype does not have valid values, it calls NumberValue() to compute them. The function then returns the intvalue member of the token, which holds the unsigned long value

		\return The unsigned long value stored in the token if it is of type TT_NUMBER, otherwise 0
	*/
	unsigned long GetUnsignedLongValue();

	/*!
		\brief Returns the integer value of a token that represents a number

		This function retrieves the integer value from a token that was parsed as a numeric type. It is used primarily when parsing numerical values from source files, such as when reading polygon and
	   brush data during collision model loading. The function internally calls GetUnsignedLongValue() to obtain the numeric value and then casts it to an integer.

		\return The integer value represented by the token
	*/
	int			  GetIntValue();

	/*!
		\brief Returns the length of whitespace before the token.

		The function calculates the difference between the end and start of the whitespace region stored in the token. If there is whitespace before the token, this difference will be positive,
	   otherwise it will be zero.

		\return The length of whitespace before the token
	*/
	int			  WhiteSpaceBeforeToken() const;

	/*!
		\brief Clears the whitespace information stored in the token.

		This function resets the whitespace tracking variables of the token, effectively forgetting any whitespace that was previously associated with it. It sets the start and end pointers for
	   whitespace to NULL and resets the line crossing count to zero.

	*/
	void		  ClearTokenWhiteSpace();

	/*!
		\brief Calculates the numeric values for a token of type TT_NUMBER

		This function processes a token that has been identified as a number and computes both its integer and floating-point representations. It handles various number formats including
	   floating-point, decimal, IP addresses, octal, hexadecimal, and binary numbers. The function first validates that the token type is TT_NUMBER, then parses the token's string representation
	   according to its subtype to determine the appropriate numeric value. The computed values are stored in the floatvalue and intvalue member variables of the token.

		\throws assertion failure if the token type is not TT_NUMBER
	*/
	void		  NumberValue();

private:
	// DG: use int instead of long for 64bit compatibility
	unsigned int intvalue; // integer value
	// DG end
	double		 floatvalue;		// floating point value
	const char*	 whiteSpaceStart_p; // start of white space before token, only used by idLexer
	const char*	 whiteSpaceEnd_p;	// end of white space before token, only used by idLexer
	idToken*	 next;				// next token in chain, only used by idParser

	/*!
		\brief Appends a character to the token data without adding a trailing zero.

		This function appends the specified character to the token's internal data buffer. It ensures that the buffer has enough space for the new character and the existing length, then adds the
	   character to the buffer and increments the length counter. The function does not add a trailing zero byte after appending the character.

		\param a The character to append to the token data
	*/
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
