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

#ifndef __LEXER_H__
#define __LEXER_H__

/*
===============================================================================

	Lexicographical parser

	Does not use memory allocation during parsing. The lexer uses no
	memory allocation if a source is loaded with LoadMemory().
	However, idToken may still allocate memory for large strings.

	A number directly following the escape character '\' in a string is
	assumed to be in decimal format instead of octal. Binary numbers of
	the form 0b.. or 0B.. can also be used.

===============================================================================
*/

// lexer flags
typedef enum {
	LEXFL_NOERRORS					 = BIT( 0 ),  // don't print any errors
	LEXFL_NOWARNINGS				 = BIT( 1 ),  // don't print any warnings
	LEXFL_NOFATALERRORS				 = BIT( 2 ),  // errors aren't fatal
	LEXFL_NOSTRINGCONCAT			 = BIT( 3 ),  // multiple strings separated by whitespaces are not concatenated
	LEXFL_NOSTRINGESCAPECHARS		 = BIT( 4 ),  // no escape characters inside strings
	LEXFL_NODOLLARPRECOMPILE		 = BIT( 5 ),  // don't use the $ sign for precompilation
	LEXFL_NOBASEINCLUDES			 = BIT( 6 ),  // don't include files embraced with < >
	LEXFL_ALLOWPATHNAMES			 = BIT( 7 ),  // allow path seperators in names
	LEXFL_ALLOWNUMBERNAMES			 = BIT( 8 ),  // allow names to start with a number
	LEXFL_ALLOWIPADDRESSES			 = BIT( 9 ),  // allow ip addresses to be parsed as numbers
	LEXFL_ALLOWFLOATEXCEPTIONS		 = BIT( 10 ), // allow float exceptions like 1.#INF or 1.#IND to be parsed
	LEXFL_ALLOWMULTICHARLITERALS	 = BIT( 11 ), // allow multi character literals
	LEXFL_ALLOWBACKSLASHSTRINGCONCAT = BIT( 12 ), // allow multiple strings separated by '\' to be concatenated
	LEXFL_ONLYSTRINGS				 = BIT( 13 )  // parse as whitespace deliminated strings (quoted strings keep quotes)
} lexerFlags_t;

typedef enum { BRSKIP_BRACES, BRSKIP_BRACKET } braceSkipMode_t;

// punctuation ids
#define P_RSHIFT_ASSIGN	   1
#define P_LSHIFT_ASSIGN	   2
#define P_PARMS			   3
#define P_PRECOMPMERGE	   4

#define P_LOGIC_AND		   5
#define P_LOGIC_OR		   6
#define P_LOGIC_GEQ		   7
#define P_LOGIC_LEQ		   8
#define P_LOGIC_EQ		   9
#define P_LOGIC_UNEQ	   10

#define P_MUL_ASSIGN	   11
#define P_DIV_ASSIGN	   12
#define P_MOD_ASSIGN	   13
#define P_ADD_ASSIGN	   14
#define P_SUB_ASSIGN	   15
#define P_INC			   16
#define P_DEC			   17

#define P_BIN_AND_ASSIGN   18
#define P_BIN_OR_ASSIGN	   19
#define P_BIN_XOR_ASSIGN   20
#define P_RSHIFT		   21
#define P_LSHIFT		   22

#define P_POINTERREF	   23
#define P_CPP1			   24
#define P_CPP2			   25
#define P_MUL			   26
#define P_DIV			   27
#define P_MOD			   28
#define P_ADD			   29
#define P_SUB			   30
#define P_ASSIGN		   31

#define P_BIN_AND		   32
#define P_BIN_OR		   33
#define P_BIN_XOR		   34
#define P_BIN_NOT		   35

#define P_LOGIC_NOT		   36
#define P_LOGIC_GREATER	   37
#define P_LOGIC_LESS	   38

#define P_REF			   39
#define P_COMMA			   40
#define P_SEMICOLON		   41
#define P_COLON			   42
#define P_QUESTIONMARK	   43

#define P_PARENTHESESOPEN  44
#define P_PARENTHESESCLOSE 45
#define P_BRACEOPEN		   46
#define P_BRACECLOSE	   47
#define P_SQBRACKETOPEN	   48
#define P_SQBRACKETCLOSE   49
#define P_BACKSLASH		   50

#define P_PRECOMP		   51
#define P_DOLLAR		   52

// punctuation
typedef struct punctuation_s {
	const char* p; // punctuation character(s)
	int			n; // punctuation id
} punctuation_t;

/*!
	\class idLexer
	\brief A lexer class for parsing text scripts into tokens.

	The idLexer class provides functionality for loading and parsing script files or memory buffers into structured tokens. It supports various token types including strings, numbers, punctuation, and
   identifiers, with methods for reading, peeking, and skipping tokens. The lexer maintains state such as line numbers, file offsets, and error conditions, and supports custom punctuation tables and
   file loading from different sources. It includes methods for parsing data types like integers, floats, and matrices, as well as handling escape sequences and complex parsing operations. The class
   is designed to be used in environments where structured parsing of text configuration or script files is required.

*/
class idLexer
{
	friend class idParser;

public:
	//! Constructs a new idLexer object with default settings.
	idLexer();

	//! Constructs an idLexer object with the specified flags.
	idLexer( int flags );

	//! Constructs an idLexer object to parse a file with the specified filename and flags.
	idLexer( const char* filename, int flags = 0, bool OSPath = false );

	//! Constructs an idLexer object to parse text from a memory buffer.
	idLexer( const char* ptr, int length, const char* name, int flags = 0 );

	//! Destroys the lexer and frees its associated resources.
	~idLexer();

	//! Loads a script file into the lexer for token parsing
	int				LoadFile( const char* filename, bool OSPath = false );

	//! Loads a script from memory with the specified length and line offset for parsing.
	int				LoadMemory( const char* ptr, int length, const char* name, int startLine = 1 );

	//! Frees the memory allocated for the lexer's source buffer and punctuation tables.
	void			FreeSource();
	// returns true if a script is loaded
	int				IsLoaded() { return idLexer::loaded; };

	//! Reads the next token from the lexer stream into the provided token object.
	int				ReadToken( idToken* token );

	//! Expects and reads a specific token string from the lexer
	int				ExpectTokenString( const char* string );

	//! Expects and validates a token of a specific type and subtype from the lexer.
	int				ExpectTokenType( int type, int subtype, idToken* token );

	//! Expects and reads any token from the lexer, returning 1 on success and 0 on failure.
	int				ExpectAnyToken( idToken* token );

	//! Checks if the next token in the lexer matches the given string and advances the lexer if it does.
	int				CheckTokenString( const char* string );

	//! Checks if the next token matches the specified type and subtype, and reads it if it does.
	int				CheckTokenType( int type, int subtype, idToken* token );

	//! Returns true if the next token equals the given string but does not remove the token from the source.
	int				PeekTokenString( const char* string );

	//! Checks if the next token matches the specified type and subtype without removing it from the source.
	int				PeekTokenType( int type, int subtype, idToken* token );

	//! Skips tokens in the lexer until the specified string is found and returns whether it was found.
	int				SkipUntilString( const char* string );

	//! Skips the rest of the current line in the lexer.
	int				SkipRestOfLine();

	//! Skips a braced section in the lexer input.
	int				SkipBracedSection( bool parseFirstBrace = true, braceSkipMode_t skipMode = BRSKIP_BRACES, int* skipped = nullptr );

	//! Skips whitespace and comments in the lexer's input stream, returning true if successful.
	bool			SkipWhiteSpace( bool currentLine );

	//! Unreads the provided token, making it available for subsequent lexical analysis.
	void			UnreadToken( const idToken* token );

	//! Reads a token from the lexer only if it appears on the same line
	int				ReadTokenOnLine( idToken* token );

	//! Returns the rest of the current line from the lexer's script.
	const char*		ReadRestOfLine( idStr& out );

	//! Parses and returns a signed integer value from the lexer input
	int				ParseInt();

	//! Parses a boolean value from the token stream
	bool			ParseBool();

	//! Parses a floating point number from the lexer input, with optional error flag handling
	float			ParseFloat( bool* errorFlag = NULL );

	//! Parses a 1D matrix of floats from the lexer input
	int				Parse1DMatrix( int x, float* m );

	//! Parses a 1D matrix in JSON format from the lexer input.
	int				Parse1DMatrixJSON( int x, float* m );

	//! Parses a 2D matrix of specified dimensions from the lexer input.
	int				Parse2DMatrix( int y, int x, float* m );

	//! Parses a 3D matrix from the lexer input into a float array.
	int				Parse3DMatrix( int z, int y, int x, float* m );

	//! Parses a braced section from the lexer into the provided string.
	const char*		ParseBracedSection( idStr& out );

	//! Parses a braced section from the lexer input into a string while maintaining indentation and newlines.
	const char*		ParseBracedSectionExact( idStr& out, int tabs = -1 );

	//! Parses a bracketed section from the lexer script into the provided string with exact tab handling.
	const char*		ParseBracketSectionExact( idStr& out, int tabs = -1 );

	//! Parses the rest of the current line into the provided string output.
	const char*		ParseRestOfLine( idStr& out );

	//! Parses and returns the complete line including the newline character from the lexer's current position.
	const char*		ParseCompleteLine( idStr& out );

	//! Retrieves the white space characters before the last read token.
	int				GetLastWhiteSpace( idStr& whiteSpace ) const;

	//! Returns the start index into the text buffer of the last white space.
	int				GetLastWhiteSpaceStart() const;

	//! Returns the end index into the text buffer of the last white space.
	int				GetLastWhiteSpaceEnd() const;

	//! Sets the punctuation table for the lexer, restoring the default C/C++ set if NULL is provided
	void			SetPunctuations( const punctuation_t* p );

	//! Returns a pointer to the punctuation string with the given id
	const char*		GetPunctuationFromId( int id );

	//! Returns the id for the given punctuation string.
	int				GetPunctuationId( const char* p );

	//! Sets the lexer flags to the specified value.
	void			SetFlags( int flags );

	//! Returns the lexer flags.
	int				GetFlags();

	//! Resets the lexer state to its initial conditions.
	void			Reset();

	//! Returns true if the lexer has reached the end of the input file.
	bool			EndOfFile();

	//! Returns the current filename being processed by the lexer.
	const char*		GetFileName();

	//! Returns the current offset within the script buffer
	const int		GetFileOffset();

	//! Returns the file time associated with the lexer's source file.
	const ID_TIME_T GetFileTime();

	//! Returns the current line number from the lexer.
	const int		GetLineNum();

	//! Prints an error message with file and line number context.
	void			Error( VERIFY_FORMAT_STRING const char* str, ... );

	//! Prints a warning message with file and line number context
	void			Warning( VERIFY_FORMAT_STRING const char* str, ... );

	//! Returns true if a lexical error occurred during parsing.
	bool			HadError() const;

	//! Sets the base folder path used for loading files.
	static void		SetBaseFolder( const char* path );

private:
	int					 loaded;			// set when a script file is loaded from file or memory
	idStr				 filename;			// file name of the script
	int					 allocated;			// true if buffer memory was allocated
	const char*			 buffer;			// buffer containing the script
	const char*			 script_p;			// current pointer in the script
	const char*			 end_p;				// pointer to the end of the script
	const char*			 lastScript_p;		// script pointer before reading token
	const char*			 whiteSpaceStart_p; // start of last white space
	const char*			 whiteSpaceEnd_p;	// end of last white space
	ID_TIME_T			 fileTime;			// file time
	int					 length;			// length of the script in bytes
	int					 line;				// current line in script
	int					 lastline;			// line before reading token
	int					 intialLine;		// line that was set on load as starting line
	int					 tokenavailable;	// set by unreadToken
	int					 flags;				// several script flags
	const punctuation_t* punctuations;		// the punctuations used in the script
	int*				 punctuationtable;	// ASCII table with punctuations
	int*				 nextpunctuation;	// next punctuation in chain
	idToken				 token;				// available token
	idLexer*			 next;				// next script in a chain
	bool				 hadError;			// set by idLexer::Error, even if the error is supressed

	static char			 baseFolder[256]; // base folder to load files from

private:
	//! Initializes or updates the punctuation table used by the lexer based on the provided punctuation list.
	void CreatePunctuationTable( const punctuation_t* punctuations );

	//! Reads whitespace and comments from the script, updating the line counter when newlines are encountered.
	int	 ReadWhiteSpace();

	//! Reads an escape character from the lexer stream and stores the resulting character in the provided pointer.
	int	 ReadEscapeCharacter( char* ch );

	//! Reads a string or literal token from the lexer, handling escape sequences and concatenation.
	int	 ReadString( idToken* token, int quote );

	//! Reads a name token from the lexer's input stream and stores it in the provided token object
	int	 ReadName( idToken* token );

	//! Reads a number token from the lexer script, handling various number formats including decimal, hexadecimal, binary, octal, floating-point, and IP addresses.
	int	 ReadNumber( idToken* token );

	//! Reads punctuation from the current script position and stores it in the provided token
	int	 ReadPunctuation( idToken* token );
	int	 ReadPrimitive( idToken* token );

	//! Checks if the given string matches the current position in the lexer script.
	int	 CheckString( const char* str ) const;

	//! Returns the number of lines crossed since the last line count reset.
	int	 NumLinesCrossed();
};

ID_INLINE const char* idLexer::GetFileName()
{
	return idLexer::filename;
}

ID_INLINE const int idLexer::GetFileOffset()
{
	return idLexer::script_p - idLexer::buffer;
}

ID_INLINE const ID_TIME_T idLexer::GetFileTime()
{
	return idLexer::fileTime;
}

ID_INLINE const int idLexer::GetLineNum()
{
	return idLexer::line;
}

ID_INLINE void idLexer::SetFlags( int flags )
{
	idLexer::flags = flags;
}

ID_INLINE int idLexer::GetFlags()
{
	return idLexer::flags;
}

/*!
	\class iceScopedLexerBaseFolder
	\brief A class for managing scoped lexer base folder functionality.
*/
class iceScopedLexerBaseFolder
{
public:
	//! Initializes the scoped lexer base folder with the specified base folder path.
	iceScopedLexerBaseFolder( const char* baseFolder ) { idLexer::SetBaseFolder( baseFolder ); }

	~iceScopedLexerBaseFolder() { idLexer::SetBaseFolder( "" ); }
};

#endif /* !__LEXER_H__ */
