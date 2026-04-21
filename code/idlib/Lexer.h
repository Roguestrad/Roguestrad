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

typedef struct punctuation_s {
	const char* p; // punctuation character(s)
	int			n; // punctuation id
} punctuation_t;

/*!
	\class idLexer
	\brief A lexical analyzer class for parsing text files and memory buffers into tokens.

	The idLexer class provides a comprehensive lexical analysis capability for parsing text-based content in the engine. It supports loading content from both file sources and memory buffers, with
   flexible configuration through various flags that control parsing behavior such as string concatenation and precompilation options. The class maintains internal state for tracking position within
   input streams, line numbers, and token availability. It handles multiple token types including strings, numbers, names, and punctuation, with specialized methods for parsing structured data such as
   matrices and braced sections. The lexer supports error handling with detailed reporting including file names and line numbers, and provides utility functions for peeking, skipping, and unreading
   tokens. It includes support for different punctuation configurations and can process both standard C/C++ style comments and whitespace, making it suitable for parsing configuration files, asset
   definitions, and other structured text data within the engine's content pipeline.

*/
class idLexer
{
	friend class idParser;

public:
	/*!
		\brief Initializes a new instance of the idLexer class with default values.

		This constructor initializes all internal state variables of the idLexer class to their default values. It sets the loaded flag to false, initializes the filename to an empty string, resets
	   the flags, and initializes various internal tracking variables such as line numbers, token availability, and file time. The punctuations are set to null, and memory allocation tracking is
	   initialized to false. This constructor is typically used when creating a lexer instance that will later be loaded with a file or data source.

	*/
	idLexer();

	/*!
		\brief Initializes a new instance of the idLexer class with the specified flags.

		The idLexer constructor initializes all internal state variables to their default values. It sets the loaded flag to false, clears the filename, assigns the provided flags, and initializes
	   various other internal tracking variables such as line numbers, token availability, and error states. The punctuations are set to NULL and the lexer is marked as not allocated.

		\param flags The flags to configure the lexer behavior
	*/
	idLexer( int flags );

	/*!
		\brief Initializes a new lexer instance to parse the specified file with given flags.

		Constructs a lexer object that will load and tokenize the contents of the provided file. The lexer is configured based on the specified flags and can handle different file path conventions
	   depending on the OSPath parameter. It sets up default values for internal state variables and attempts to load the specified file.

		\param filename The path to the file to be lexed and parsed
		\param flags Flags that control lexer behavior such as string concatenation and precompilation options
		\param OSPath Indicates whether the filename uses OS-specific path conventions
	*/
	idLexer( const char* filename, int flags = 0, bool OSPath = false );

	/*!
		\brief Initializes a new lexer instance to parse tokenized data from a memory buffer.

		This constructor sets up a lexer to read from a provided memory buffer specified by ptr and length. It initializes internal state including flags, punctuation settings, and error tracking. The
	   lexer is configured to load the provided memory segment as a source for tokenization, using the name parameter for identification. It is commonly used to parse configuration or data files
	   loaded into memory.

		\param ptr Pointer to the memory buffer containing the data to be lexed.
		\param length Number of bytes in the memory buffer to be lexed.
		\param name Name of the source file or buffer used for identification and error reporting.
		\param flags Optional flags to configure lexer behavior, such as string concatenation or precompilation handling.
	*/
	idLexer( const char* ptr, int length, const char* name, int flags = 0 );

	/*!
		\brief Destructor for the idLexer class that frees the allocated source memory.

		This destructor is responsible for cleaning up the resources associated with the idLexer object. It calls the FreeSource() method to release any memory that was allocated for the lexical
	   source.

	*/
	~idLexer();

	/*!
		\brief Loads a script file into memory for token parsing.

		This function opens and reads the specified file into a memory buffer, setting up the lexer to parse the script. It handles path resolution based on the OSPath flag and tracks the file's
	   timestamp and full path. The function ensures only one script is loaded at a time, reporting an error if another script is already loaded. It allocates memory for the buffer and initializes all
	   lexer state variables including line numbers and pointers to the script buffer.

		\param filename The name of the file to load as a script
		\param OSPath If true, treats the filename as an absolute OS path; if false, uses the baseFolder for path resolution
		\return Returns 1 if the file is successfully loaded, 0 otherwise
		\throws An error is thrown if another script is already loaded
	*/
	int				LoadFile( const char* filename, bool OSPath = false );

	/*!
		\brief Loads a script from memory with the specified length and line offset for parsing

		This function initializes the lexer to parse a script that is already loaded in memory. It sets up the internal state of the lexer including buffer pointers, line tracking, and file
	   information. The function expects the provided memory to contain a valid null-terminated C string. It returns true if the memory was successfully loaded, false if another script is already
	   loaded.

		\param ptr Pointer to the memory buffer containing the script to load
		\param length Number of bytes to read from the memory buffer
		\param name Name of the script being loaded, used for error reporting
		\param startLine Starting line number for the script, used for proper line number tracking in errors
		\return True if the script was successfully loaded, false if another script is already loaded
		\throws Error is thrown if another script is already loaded
	*/
	int				LoadMemory( const char* ptr, int length, const char* name, int startLine = 1 );

	/*!
		\brief Frees the memory allocated for the lexer's source buffer and punctuation tables

		This function releases all memory that was previously allocated for holding the lexer's source buffer and punctuation tables. It checks if the punctuation tables have been dynamically
	   allocated and frees them if they are not pointing to the default tables. It also frees the main buffer if it was allocated and resets all internal state variables to their default values.

	*/
	void			FreeSource();
	// returns true if a script is loaded
	int				IsLoaded() { return idLexer::loaded; };

	/*!
		\brief Reads the next token from the input stream and stores it in the provided token object.

		This function reads the next token from the lexer's input stream. It handles various token types including strings, numbers, names, and punctuation. The function manages whitespace, line
	   counting, and token flags. If a token is already available from a previous unread operation, it returns that token. The function returns 1 on success and 0 if no token could be read or an error
	   occurred.

		\param token Pointer to the token object where the parsed token will be stored
		\return Integer value indicating success (1) or failure (0) of token reading operation
		\throws Error thrown when no file is loaded or when encountering unknown punctuation
	*/
	int				ReadToken( idToken* token );

	/*!
		\brief Expects a specific token string from the lexer input and returns 1 if found, 0 otherwise

		This function attempts to read the next token from the lexer input and checks if it matches the expected string. If the token cannot be read or does not match the expected string, an error
	   message is generated and the function returns 0. If the token matches, the function returns 1. The function is commonly used in parsing operations where specific tokens are required to proceed
	   with parsing.

		\param string The expected token string to match against the next token in the input
		\return 1 if the next token matches the expected string, 0 otherwise
	*/
	int				ExpectTokenString( const char* string );

	/*!
		\brief Expects and validates a token of a specific type and subtype from the lexer input.

		This function reads a token from the lexer and verifies that its type matches the expected type. If the token type matches, it further checks the subtype for number and punctuation tokens to
	   ensure they meet the specified requirements. If the token does not match the expected type or subtype, an error message is generated and the function returns zero. Otherwise, it returns one to
	   indicate success. The function is commonly used when parsing structured input where specific token types and subtypes are expected at certain points.

		\param type The expected token type, such as TT_STRING, TT_NUMBER, etc.
		\param subtype The expected subtype for tokens, particularly used for numbers and punctuation to specify formats like decimal, hex, etc.
		\param token Pointer to the token structure where the read token will be stored.
		\return Returns 1 if the token matches the expected type and subtype, otherwise returns 0.
		\throws Throws an error via idLexer::Error if the token type or subtype does not match the expected values.
	*/
	int				ExpectTokenType( int type, int subtype, idToken* token );

	/*!
		\brief Reads and expects a token from the lexer, returning a success status.

		This function attempts to read a token from the lexer and stores it in the provided token pointer. If the token cannot be read, an error message is issued and the function returns zero.
	   Otherwise, it returns one to indicate successful reading of the token.

		\param token Pointer to the token structure where the parsed token will be stored
		\return Integer value indicating success (1) or failure (0) of reading the token.
	*/
	int				ExpectAnyToken( idToken* token );

	/*!
		\brief Checks if the next token in the lexer matches the specified string and returns 1 if it does, otherwise returns 0.

		This function attempts to read the next token from the lexer and compares it to the provided string.
		If the token matches the string, it returns 1.
		If the token does not match or no token is available, it unreads the token and returns 0.
		This function is commonly used for parsing structured input where specific tokens are expected, such as in collision model file parsing.

		\param string The string to compare against the next token in the lexer.
		\return 1 if the next token matches the specified string, 0 otherwise.
	*/
	int				CheckTokenString( const char* string );

	/*!
		\brief Checks if the next token in the lexer matches the specified type and subtype, and reads it into the provided token if it matches.

		This function attempts to read the next token from the lexer and checks if its type matches the specified type parameter and if its subtype matches the specified subtype parameter. If the
	   token matches, it is copied into the provided token pointer and the function returns true. If the token does not match, the lexer's position is reset and the function returns false. The subtype
	   parameter is used with a bitwise AND operation to check for specific subtype flags.

		\param type The expected token type to match against.
		\param subtype The expected token subtype to match against, used with bitwise AND operation.
		\param token Pointer to an idToken where the matched token will be stored if found.
		\return Returns 1 if a matching token was found and read, 0 otherwise.
	*/
	int				CheckTokenType( int type, int subtype, idToken* token );

	/*!
		\brief Checks if the next token in the lexer matches the given string without consuming it

		This function attempts to read the next token from the lexer and compares it to the provided string.
		If they match, it returns 1, otherwise 0. The token is not consumed from the input stream regardless of the match result.
		This allows for peeking at the next token to make parsing decisions without advancing the lexer position.

		\param string The string to compare against the next token in the lexer
		\return 1 if the next token matches the given string, 0 otherwise
	*/
	int				PeekTokenString( const char* string );

	/*!
		\brief Checks if the next token matches the specified type and subtype without removing it from the input stream

		This function examines the next token in the input stream to determine if it matches the provided type and subtype criteria. If a match is found, the token is copied to the output parameter
	   and the function returns true. Otherwise, the token is not consumed and the function returns false. The function is useful for lookahead operations in parsing.

		\param type The expected token type to match against
		\param subtype The expected token subtype to match against
		\param token Pointer to the token structure to store the matched token if found
		\return 1 if the next token matches the specified type and subtype, 0 otherwise
	*/
	int				PeekTokenType( int type, int subtype, idToken* token );

	/*!
		\brief Skips tokens in the lexer until the specified string is found and returns whether the string was found.

		This function iterates through tokens read from the lexer and skips them until the specified string is encountered. It returns 1 if the string is found and 0 if the end of the token stream is
	   reached without finding the string. The function is commonly used to skip over irrelevant tokens in a parser to find a specific marker or delimiter.

		\param string The string token to search for in the token stream.
		\return 1 if the specified string is found in the token stream, 0 if the end of the stream is reached without finding the string.
	*/
	int				SkipUntilString( const char* string );

	/*!
		\brief Skips the rest of the current line in the lexer's input stream.

		The function reads tokens from the input stream until it encounters a token that crosses a line boundary. When such a token is found, the lexer's position is reset to the start of the current
	   line, and the function returns 1. If the end of the input stream is reached without finding a line-crossing token, the function returns 0.

		\return 1 if a line-crossing token was found and the position was reset, 0 if the end of input was reached
	*/
	int				SkipRestOfLine();

	/*!
		\brief Skips a braced section in the lexer input, handling nested braces and returning the number of skipped sections.

		This function processes the input stream to skip over a section enclosed in braces or brackets as determined by the skipMode parameter. It maintains a depth counter to track nested structures
	   and returns true when the matching closing brace or bracket is found. The function can optionally track the number of sections skipped.

		\param parseFirstBrace If true, the first opening brace is parsed and counted towards the depth
		\param skipMode Determines whether to skip braces {} or brackets []
		\param skipped Optional pointer to store the count of skipped sections
		\return Returns 1 if the braced section is successfully skipped, 0 if the end of input is reached before finding the closing brace.
	*/
	int				SkipBracedSection( bool parseFirstBrace = true, braceSkipMode_t skipMode = BRSKIP_BRACES, int* skipped = nullptr );

	/*!
		\brief Skips whitespace and comments in the input stream, optionally stopping at the end of the current line

		This function advances the lexer's position through the input stream by skipping over whitespace characters and comments. It handles both single-line comments starting with // and multi-line
	comments enclosed in block comments. When the currentLine parameter		   is true, the function will stop skipping at the end of the current line and return true,
		allowing the caller to process the end - of -
			line condition.The function properly maintains the line number count when encountering newline characters.If the end of the input stream is reached before finding a valid token,
		the function returns false.

	\param currentLine If true, stops skipping at the end of the current line and returns true
	\return True if the function successfully skipped the white space and comments, false if there is no token left to read */
	bool			SkipWhiteSpace( bool currentLine );

	/*!
		\brief Unreads the provided token, making it available for subsequent reads.

		This function places the given token back into the lexer's buffer, making it available for the next token read operation. It asserts that no other token is currently waiting to be read,
	   ensuring that the unread operation is performed correctly. The token is copied into the lexer's internal token storage and marked as available for reading.

		\param token The token to be unread from the lexer
		\throws FatalError if a token is already available for reading
	*/
	void			UnreadToken( const idToken* token );

	/*!
		\brief Reads a token from the lexer only if it appears on the same line as the current position.

		This function attempts to read a token from the lexer. If the token is found on the same line as the current position, it copies the token to the provided memory location and returns true. If
	   the token appears on a different line, the lexer position is restored and the function returns false. This is useful for parsing parameters or values that must remain on a single line.

		\param token Output parameter that will receive the parsed token if it is found on the same line
		\return True if a token was successfully read and is on the same line, false otherwise.
	*/
	int				ReadTokenOnLine( idToken* token );

	/*!
		\brief Reads the remaining content of the current line into the provided string buffer.

		This function processes the remaining characters on the current line from the lexer's script position until it encounters a newline character or reaches the end of the script. It appends
	   characters to the provided string buffer, replacing control characters with spaces. The function strips trailing whitespace from the final result before returning a pointer to the string's
	   content.

		\param out Buffer to store the remaining line content
		\return Pointer to the string buffer containing the remaining line content after processing
	*/
	const char*		ReadRestOfLine( idStr& out );

	/*!
		\brief Parses and returns a signed integer value from the lexer input

		This function reads a token from the lexer and attempts to parse it as a signed integer value. If the token is a negative sign followed by a number, it will parse the negative value. The
	   function will generate an error and return 0 if the token is not a valid integer. The function is commonly used when parsing structured data files, such as collision model files, where integer
	   values are expected in specific positions.

		\return The parsed signed integer value from the lexer input
		\throws An error is thrown if the expected integer value is not found
	*/
	int				ParseInt();

	/*!
		\brief Parses a boolean value from the token stream

		This function reads a token from the lexer and attempts to parse it as a boolean value. It expects a numeric token and returns true if the value is non-zero, false otherwise. If the expected
	   token is not found, it reports an error and returns false.

		\return true if a valid boolean value (non-zero number) was parsed, false otherwise
	*/
	bool			ParseBool();

	/*!
		\brief Parses a floating point number from the lexer input, with optional error flag handling

		This function reads a token from the lexer and attempts to convert it to a floating point number. If the token cannot be parsed as a number, it will either issue an error or warning based on
	   the errorFlag parameter. The function handles negative numbers by checking for a minus punctuation token followed by a number. It returns 0 if parsing fails and the errorFlag is provided,
	   otherwise it throws an error directly. This function is commonly used when parsing numeric values from configuration files or other text-based data sources, such as in collision model parsing
	   or UI element definitions.

		\param errorFlag Optional pointer to a boolean that will be set to true if parsing fails, otherwise will issue an error directly
		\return The parsed floating point number, or 0 if parsing fails and an errorFlag is provided
	*/
	float			ParseFloat( bool* errorFlag = NULL );

	/*!
		\brief Parses a 1D matrix of floats from the lexer input.

		This function reads a sequence of floating-point values from the lexer input, expecting an opening parenthesis, followed by x float values, and a closing parenthesis. The parsed values are
	   stored in the provided float array m. It returns true if parsing is successful, false otherwise.

		\param x The number of float values to parse
		\param m Pointer to the array where the parsed float values will be stored
		\return True if parsing is successful, false otherwise
	*/
	int				Parse1DMatrix( int x, float* m );

	/*!
		\brief Parses a 1D matrix from JSON format into a float array

		This function reads a 1D matrix from JSON input, expecting an array format with square brackets. It parses x floating-point values from the input stream and stores them in the provided array
	   m. The function expects comma-separated values within the brackets and validates the closing bracket. It returns true upon successful parsing, false if the input is malformed or does not match
	   the expected format.

		\param x Number of elements to parse
		\param m Pointer to the destination array where parsed values will be stored
		\return True if the matrix was successfully parsed, false otherwise
	*/
	int				Parse1DMatrixJSON( int x, float* m );

	/*!
		\brief Parses a 2D matrix from the lexer input

		This function parses a 2D matrix of dimensions y by x from the lexer input. It first expects an opening parenthesis token, then parses each row of the matrix using Parse1DMatrix, and finally
	   expects a closing parenthesis token. The matrix data is stored in the provided float array m.

		\param y number of rows in the matrix
		\param x number of columns in the matrix
		\param m pointer to the float array where the matrix data will be stored
		\return true if the matrix was successfully parsed, false otherwise
	*/
	int				Parse2DMatrix( int y, int x, float* m );

	/*!
		\brief Parses a 3D matrix of dimensions z by y by x from the lexer input into the provided float array.

		This function reads a 3D matrix from the lexer input, where the matrix is structured as a series of 2D matrices. It first expects an opening parenthesis, then parses each 2D matrix using
	   Parse2DMatrix, and finally expects a closing parenthesis. The parsed values are stored in the provided float array m, with each 2D matrix stored sequentially in memory.

		\param z The depth dimension of the 3D matrix
		\param y The height dimension of the 3D matrix
		\param x The width dimension of the 3D matrix
		\param m Pointer to the float array where the matrix data will be stored
		\return Returns true if the 3D matrix was successfully parsed, false otherwise.
	*/
	int				Parse3DMatrix( int z, int y, int x, float* m );

	/*!
		\brief Parses a braced section from the lexer input and returns it as a string

		This function reads tokens from the lexer until a matching closing brace is found, handling nested braces by tracking the depth. The parsed content is stored in the provided output string,
	   including proper spacing and string literals. If the opening brace is missing, it returns the empty string. If a closing brace is missing, it reports an error and returns the partially parsed
	   string

		\param out output string to store the parsed braced section
		\return const char * pointer to the parsed braced section stored in the output string
	*/
	const char*		ParseBracedSection( idStr& out );

	/*!
		\brief Parses a braced section from the lexer input into the provided string, maintaining indentation and newlines.

		This function extracts a braced section from the lexer's input stream and stores it in the provided string. It handles nested braces by tracking the depth of nesting. The function preserves
	   tabs and newlines in the output, adjusting indentation based on the nesting level. If a tab count is specified, it will maintain that level of indentation for the parsed content. The function
	   returns a pointer to the internal string buffer containing the parsed content.

		\param out The string to store the parsed braced section
		\param tabs The initial tab depth to maintain, or -1 to use automatic tab handling
		\return A pointer to the internal string buffer containing the parsed braced section
	*/
	const char*		ParseBracedSectionExact( idStr& out, int tabs = -1 );

	/*!
		\brief Parses a bracket section from the lexer input and stores it in the provided string, handling nested brackets and optional tab indentation.

		This function reads a bracketed section from the lexer's input stream, starting with an opening bracket '[', and continues until it finds the matching closing bracket ']'. It correctly handles
	   nested brackets by tracking the depth of nesting. The function also processes tab indentation based on the tabs parameter. If tabs is set to a negative value, tab processing is disabled. The
	   result is stored in the provided out string parameter.

		\param out The string to store the parsed bracket section
		\param tabs The number of tabs to use for indentation, or -1 to disable tab processing
		\return A pointer to the parsed string stored in the out parameter
	*/
	const char*		ParseBracketSectionExact( idStr& out, int tabs = -1 );

	/*!
		\brief Parses the remainder of the current line into the provided string, returning a pointer to the string's contents.

		This function reads tokens from the current line until it encounters a newline or end of file. It accumulates the tokens into the provided string, separating them with spaces. If a token
	   crosses a line, the function stops parsing and restores the lexer state to the beginning of the line. The function returns a pointer to the internal string buffer.

		\param out The string to be filled with the parsed tokens from the remainder of the line
		\return A pointer to the character array containing the parsed tokens from the remainder of the line
	*/
	const char*		ParseRestOfLine( idStr& out );

	/*!
		\brief Parses a complete line from the script including the newline character and returns a pointer to the result.

		This function reads from the current script position until it encounters a newline character or reaches the end of the buffer. It captures the entire line including the newline character and
	   stores it in the provided output string. The function advances the script pointer to the position after the newline character. The returned pointer points to the internal buffer of the output
	   string.

		\param out The string to store the parsed line including the newline character
		\return A pointer to the internal buffer of the output string containing the complete line
	*/
	const char*		ParseCompleteLine( idStr& out );

	/*!
		\brief Retrieves the whitespace characters that appear before the last read token and stores them in the provided string.

		This function copies the whitespace characters found between the start and end positions of whitespace into the provided string parameter. It clears the string first, then appends each
	   character from the internal whitespace storage. The function returns the length of the whitespace string after copying.

		\param whiteSpace Output string that will contain the whitespace characters preceding the last read token
		\return The length of the whitespace string that was copied into the provided parameter
	*/
	int				GetLastWhiteSpace( idStr& whiteSpace ) const;

	//! Returns the start index into the text buffer of the last white space.
	int				GetLastWhiteSpaceStart() const;

	//! Returns the end index into the text buffer of the last white space.
	int				GetLastWhiteSpaceEnd() const;

	/*!
		\brief Sets the punctuation table used by the lexer, restoring the default if NULL is provided

		This function configures the punctuation table that the lexer uses to identify and tokenize punctuation characters. If a custom punctuation array is provided, it will be used; otherwise, the
	   default punctuation set is restored. The function ensures that both the internal punctuation table and the reference pointer are updated accordingly.

		\param p Pointer to an array of punctuation definitions, or NULL to restore the default C/C++ punctuation set
	*/
	void			SetPunctuations( const punctuation_t* p );

	/*!
		\brief Returns a pointer to the punctuation string with the given id

		This function searches through the global punctuations array to find the punctuation entry that matches the specified id. It iterates through the array until it finds a match or reaches the
	   end of the array. If a match is found, it returns the punctuation string associated with that id. If no match is found, it returns a default string "unknown punctuation".

		\param id the id of the punctuation to search for
		\return a pointer to the punctuation string if found, otherwise a default string 'unknown punctuation'
	*/
	const char*		GetPunctuationFromId( int id );

	/*!
		\brief Returns the ID for the given punctuation string

		This function searches through the predefined punctuation table to find a match for the provided punctuation string. It iterates through the table until it finds a matching punctuation or
	   reaches the end of the table. If a match is found, it returns the associated ID; otherwise, it returns zero to indicate no match was found.

		\param p The punctuation string to look up
		\return The ID associated with the punctuation string, or zero if not found
	*/
	int				GetPunctuationId( const char* p );

	/*!
		\brief Sets the lexer flags to the specified value.

		This function configures the behavior of the lexer by setting its internal flags. The flags control various parsing options and modes that affect how the lexer processes input.

		\param flags The new value for the lexer flags
	*/
	void			SetFlags( int flags );

	//! Returns the lexer flags
	int				GetFlags();

	/*!
		\brief Resets the lexer state to its initial conditions

		This function resets the lexer's internal state by reinitializing pointers to the script buffer, clearing the current token, and resetting line number tracking. It prepares the lexer for
	   processing a new script or resuming processing from the beginning of the current script buffer.

	*/
	void			Reset();

	/*!
		\brief Checks if the lexer has reached the end of the input data

		This function determines whether the lexer has processed all available input data by comparing the current position pointer with the end pointer of the input buffer

		\return true if the lexer has reached the end of the input data, false otherwise
	*/
	bool			EndOfFile();

	//! Returns the current filename associated with the lexer.
	const char*		GetFileName();

	//! Returns the current offset within the script buffer.
	const int		GetFileOffset();

	//! Returns the file time associated with the lexer's source file.
	const ID_TIME_T GetFileTime();

	//! Returns the current line number of the lexer.
	const int		GetLineNum();

	/*!
		\brief Reports an error message with the specified format string and arguments, including file and line number information.

		This function is used to report errors during lexical analysis. It formats the error message using a printf-style format string and variable arguments, then outputs the message along with the
	   current file name and line number. If the LEXFL_NOERRORS flag is set, the function returns without reporting anything. If the LEXFL_NOFATALERRORS flag is set, it reports a warning instead of an
	   error. Otherwise, it reports a fatal error and terminates execution.

		\param str Format string for the error message
	*/
	void			Error( VERIFY_FORMAT_STRING const char* str, ... );

	/*!
		\brief Prints a warning message including the file name and line number where the warning occurred.

		This function formats a warning message using a printf-style format string and variable arguments, then outputs it to the common warning system. The warning includes the current filename and
	   line number from the lexer context. The function checks if warnings are disabled via a global flag before proceeding. It is typically used to report non-fatal issues during lexical analysis or
	   parsing.

		\param str A printf-style format string for the warning message
	*/
	void			Warning( VERIFY_FORMAT_STRING const char* str, ... );

	//! Returns true if a lexer error occurred during parsing.
	bool			HadError() const;

	/*!
		\brief Sets the base folder path used for loading files.

		This function configures the base folder path that the lexer will use when loading files. The path is copied into an internal buffer, ensuring it does not exceed the allocated size. This is
	   typically used to specify the directory from which asset files should be loaded.

		\param path The path to the base folder to set for file loading
	*/
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
	/*!
		\brief Initializes or updates the punctuation table used by the lexer for tokenizing input based on the provided punctuation list.

		This function sets up the punctuation table that the lexer uses to identify and categorize punctuation characters during tokenization. It handles both default and custom punctuation lists.
	   When the default punctuation list is provided, it reuses existing tables and marks the default setup as complete. For custom lists, it allocates memory for new tables and populates them with
	   the provided punctuation entries, ensuring that longer punctuation strings are prioritized. The function sorts the punctuations in the table by length, placing longer strings before shorter
	   ones to ensure correct parsing.

		\param punctuations Pointer to an array of punctuation_t structures defining the punctuation to be handled by the lexer.
	*/
	void CreatePunctuationTable( const punctuation_t* punctuations );

	/*!
		\brief Reads and skips whitespace characters and comments from the input stream

		This function processes the input stream by skipping over whitespace characters and comments. It handles both single-line comments starting with // and multi-line comments enclosed in block
	comments .The function updates the line counter whenever a newline character is encountered.It returns zero when the end of the input stream is reached, and one when whitespace and comments are
	successfully skipped.The function advances the input pointer through the stream and manages various edge cases such as nested comments and end - of - stream conditions.

	\return 1 if whitespace and comments were successfully skipped, 0 if end of input stream was reached
	*/
	int	 ReadWhiteSpace();

	/*!
		\brief Reads and processes an escape character sequence from the lexer's input stream

		This function handles the parsing of escape character sequences that begin with a backslash. It supports various standard escape sequences like newline, tab, and backspace, as well as
	   hexadecimal and decimal numeric escape sequences. The function advances the input pointer through the sequence and stores the resulting character in the provided output parameter. It handles
	   error conditions such as invalid escape sequences and overly large numeric values by issuing warnings and truncating values to 8-bit range.

		\param ch pointer to store the parsed escape character
		\return 1 if an escape character was successfully read, 0 otherwise
	*/
	int	 ReadEscapeCharacter( char* ch );

	/*!
		\brief Reads a string or literal token from the lexer input, handling escape sequences and concatenation according to the lexer flags.

		This function processes a string or literal token from the lexer's input stream. It handles both quoted strings and single-character literals, with support for escape sequences and optional
	   concatenation of consecutive strings. The function manages the lexer's internal state, including line numbers and script position, and updates the token's type and subtype based on the parsed
	   content. It also handles error conditions such as missing quotes or newlines within strings.

		\param token Pointer to the token structure where the parsed string or literal will be stored
		\param quote The character used as the quote delimiter, either '"' for strings or '\' for literals
		\return Returns 1 if the string or literal was successfully parsed, or 0 if an error occurred during parsing.
	*/
	int	 ReadString( idToken* token, int quote );

	/*!
		\brief Reads a name token from the lexer's input stream and stores it in the provided token object.

		This function reads a sequence of characters that form a name token, which can include letters, digits, underscores, and in certain modes, dashes, forward slashes, backslashes, colons, and
	   periods. The function continues reading until it encounters a character that is not part of a name token. The length of the parsed name is stored as the token's subtype. The function returns 1
	   upon successful parsing.

		\param token Pointer to the token object where the parsed name will be stored.
		\return Returns 1 to indicate successful parsing of the name token.
	*/
	int	 ReadName( idToken* token );

	/*!
		\brief Reads a number from the lexer input and stores it in the provided token, determining its type and subtype.

		This function parses a number from the input stream starting at the current position of the lexer. It handles various number formats including hexadecimal, binary, octal, decimal integers,
	   floating-point numbers, and IP addresses. The function sets the appropriate type and subtype flags on the token based on the parsed number format. For floating-point numbers, it also checks for
	   special cases like infinity, indefinite, and NaN values. For integers, it determines the precision and signedness based on suffixes like 'u', 'l', 'L', etc.

		\param token Pointer to the token structure where the parsed number will be stored
		\return Returns 1 if a number was successfully parsed, 0 otherwise
		\throws May throw an error if the number format is invalid or if floating-point exceptions are not allowed and special values like INF, IND, or NAN are encountered.
	*/
	int	 ReadNumber( idToken* token );

	/*!
		\brief Reads punctuation from the current script position and stores it in the provided token.

		This function attempts to match punctuation characters from the current position in the script against a predefined list of punctuation tokens. If a match is found, the punctuation is stored
	   in the provided token structure and the script pointer is advanced. The function returns 1 if punctuation is successfully read, 0 otherwise.

		\param token Pointer to the token structure where the read punctuation will be stored
		\return 1 if punctuation was successfully read and stored in the token, 0 if no matching punctuation was found
	*/
	int	 ReadPunctuation( idToken* token );
	int	 ReadPrimitive( idToken* token );

	/*!
		\brief Checks if the current script position matches the given string.

		This function compares the characters at the current script position with the provided string. It returns true if all characters match, and false otherwise. The comparison is case-sensitive
	   and stops at the first mismatch or end of the string.

		\param str The string to compare against the current script position
		\return True if the string matches the current script position, false otherwise
	*/
	int	 CheckString( const char* str ) const;

	/*!
		\brief Returns the number of lines crossed since the last line count reset.

		This function calculates the difference between the current line number and the previously recorded line number. It is typically used to determine how many lines have been processed or skipped
	   since a certain point in the lexical analysis.

		\return The difference between the current line number and the last recorded line number, representing the number of lines crossed.
	*/
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
	\brief Manages the base folder configuration for a lexer instance.

	The iceScopedLexerBaseFolder class is designed to configure and manage the base folder path for a lexer object. It provides a constructor that initializes the lexer's base folder using the
   idLexer::SetBaseFolder method, allowing the lexer to search for files within a specified directory. This class serves as a scoped configuration helper, ensuring that the lexer is properly
   initialized with the correct base folder path. The destructor is implemented but its specific purpose requires clarification. The class is intended for use in engine components that require file
   parsing capabilities with a defined search path.

*/
class iceScopedLexerBaseFolder
{
public:
	/*!
		\brief Sets the base folder for the lexer.

		This constructor initializes the lexer base folder by calling idLexer::SetBaseFolder with the provided base folder path. It's used to configure the directory from which lexer will search for
	   files.

		\param baseFolder The path to the base folder to be set for the lexer
	*/
	iceScopedLexerBaseFolder( const char* baseFolder ) { idLexer::SetBaseFolder( baseFolder ); }

	~iceScopedLexerBaseFolder() { idLexer::SetBaseFolder( "" ); }
};

#endif /* !__LEXER_H__ */
