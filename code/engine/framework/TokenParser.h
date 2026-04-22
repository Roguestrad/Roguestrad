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

#ifndef __TOKENPARSER_H__
#define __TOKENPARSER_H__

/*!
	\class idBinaryToken
	\brief A class representing a binary token with support for file serialization and comparison operations.
*/
class idBinaryToken
{
public:
	//! Initializes a binary token with default type and subtype values.
	idBinaryToken()
	{
		tokenType	 = 0;
		tokenSubType = 0;
	}

	//! Constructs an idBinaryToken object by copying data from a given idToken object.
	idBinaryToken( const idToken& tok )
	{
		token		 = tok.c_str();
		tokenType	 = tok.type;
		tokenSubType = tok.subtype;
	}

	//! Compares two idBinaryToken objects for equality based on their token type, subtype, and token string.
	bool operator==( const idBinaryToken& b ) const { return ( tokenType == b.tokenType && tokenSubType == b.tokenSubType && token.Cmp( b.token ) == 0 ); }

	//! Reads a binary token from the provided file.
	void Read( idFile* inFile )
	{
		inFile->ReadString( token );
		inFile->ReadBig( tokenType );
		inFile->ReadBig( tokenSubType );
	}

	//! Writes the token data to the specified file
	void Write( idFile* inFile )
	{
		inFile->WriteString( token );
		inFile->WriteBig( tokenType );
		inFile->WriteBig( tokenSubType );
	}
	idStr token;
	int8  tokenType;
	short tokenSubType;
};

/*!
	\class idTokenIndexes
	\brief A collection class for managing token indexes with file I/O capabilities.

	The idTokenIndexes class provides functionality for storing and manipulating a collection of short integer token indexes. It supports operations to append, access, and manage the size of the
   collection, as well as associating a name with the token indexes. The class includes methods for serializing the token index data to and from files, making it suitable for persisting token index
   information. The implementation allows direct access to individual elements through array indexing and provides mechanisms for clearing and resizing the collection.

*/
class idTokenIndexes
{
public:
	//! Constructs an empty idTokenIndexes object.
	idTokenIndexes() { }

	//! Clears all token indexes from the collection.
	void		Clear() { tokenIndexes.Clear(); }

	//! Appends a short index value to the token index list and returns the resulting list size
	int			Append( short sdx ) { return tokenIndexes.Append( sdx ); }

	//! Returns the number of token indexes in the collection.
	int			Num() { return tokenIndexes.Num(); }

	//! Sets the number of elements in the token indexes array to the specified count.
	void		SetNum( int num ) { tokenIndexes.SetNum( num ); }

	//! Returns a reference to the token index at the specified array index
	short&		operator[]( const int index ) { return tokenIndexes[index]; }

	//! Sets the name of the token indexes.
	void		SetName( const char* name ) { fileName = name; }

	//! Returns the name of the token index as a null-terminated string
	const char* GetName() { return fileName.c_str(); }

	//! Writes token index data to the specified output file.
	void		Write( idFile* outFile )
	{
		outFile->WriteString( fileName );
		outFile->WriteBig( ( int )tokenIndexes.Num() );
		outFile->WriteBigArray( tokenIndexes.Ptr(), tokenIndexes.Num() );
	}

	//! Reads token index data from the provided file input stream
	void Read( idFile* inFile )
	{
		inFile->ReadString( fileName );
		int num;
		inFile->ReadBig( num );
		tokenIndexes.SetNum( num );
		inFile->ReadBigArray( tokenIndexes.Ptr(), num );
	}

private:
	idList<short> tokenIndexes;
	idStr		  fileName;
};

/*!
	\class idTokenParser
	\brief A parser for handling tokenized data with file I/O and error reporting capabilities.

	The idTokenParser class provides functionality for loading, parsing, and managing tokenized data from files or other parsers. It supports reading tokens sequentially, setting markers for
   backtracking, and handling various token types including integers, floats, and booleans. The class offers error and warning reporting mechanisms with printf-style formatting support. It can track
   parsing state and maintain a list of tokens that can be cleared, loaded, or written to files. The parser is designed for use with GUI-related token files and includes methods for validating
   expected tokens and managing parsing sessions.

*/
class idTokenParser
{
public:
	//! Initializes a new instance of the idTokenParser class with default values.
	idTokenParser()
	{
		timeStamp		 = FILE_NOT_FOUND_TIMESTAMP;
		preloaded		 = false;
		currentToken	 = 0;
		currentTokenList = 0;
	}
	~idTokenParser() { Clear(); }

	//! Clears all internal token lists and resets parser state.
	void Clear()
	{
		tokens.Clear();
		guiTokenIndexes.Clear();
		currentToken	 = 0;
		currentTokenList = -1;
		preloaded		 = false;
	}

	//! Loads token data from a file into the parser
	void  LoadFromFile( const char* filename );

	//! Writes the token parser data to a file with the specified filename
	void  WriteToFile( const char* filename );

	//! Loads token data from a parser into the token parser, using the provided GUI name as a reference.
	void  LoadFromParser( idParser& parser, const char* guiName );

	//! Starts parsing the specified GUI token file and returns true if successful.
	bool  StartParsing( const char* fileName );

	//! Marks the parsing session as complete by invalidating the current token list.
	void  DoneParsing() { currentTokenList = -1; }

	//! Returns true if the token parser has loaded tokens.
	bool  IsLoaded() { return tokens.Num() > 0; }

	//! Reads the next token from the current token list into the provided token object
	bool  ReadToken( idToken* tok );

	//! Returns true if the next token in the parser matches the given string, otherwise reports an error and returns false.
	int	  ExpectTokenString( const char* string );

	//! Expects a token of a specific type and subtype, returning 1 if successful or 0 if not.
	int	  ExpectTokenType( int type, int subtype, idToken* token );

	//! Expects and reads the next token from the parser, returning 1 on success and 0 on failure
	int	  ExpectAnyToken( idToken* token );

	//! Sets a marker at the current position in the token parser.
	void  SetMarker() { }

	//! Unreads the last token from the token stream.
	void  UnreadToken( const idToken* token );

	//! Reports an error message using printf-style formatting
	void  Error( VERIFY_FORMAT_STRING const char* str, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );

	//! Issues a warning message using a format string and variable arguments.
	void  Warning( VERIFY_FORMAT_STRING const char* str, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );

	//! Parses and returns an integer value from the token stream
	int	  ParseInt();

	//! Parses and returns a boolean value from the token parser
	bool  ParseBool();

	//! Parses a floating point number from the token stream, with optional error flag handling.
	float ParseFloat( bool* errorFlag = NULL );

	//! Updates the time stamp value if the provided time is greater than the current time stamp.
	void  UpdateTimeStamp( ID_TIME_T& t )
	{
		if( t > timeStamp ) { timeStamp = t; }
	}

private:
	idList<idBinaryToken>  tokens;
	idList<idTokenIndexes> guiTokenIndexes;
	int					   currentToken;
	int					   currentTokenList;
	ID_TIME_T			   timeStamp;
	bool				   preloaded;
};

#endif /* !__TOKENPARSER_H__ */
