/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014 Robert Beckebans

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

#ifndef __PARSER_H__
#define __PARSER_H__

/*
===============================================================================

	C/C++ compatible pre-compiler

===============================================================================
*/

#define DEFINE_FIXED  0x0001

#define BUILTIN_LINE  1
#define BUILTIN_FILE  2
#define BUILTIN_DATE  3
#define BUILTIN_TIME  4
#define BUILTIN_STDC  5

#define INDENT_IF	  0x0001
#define INDENT_ELSE	  0x0002
#define INDENT_ELIF	  0x0004
#define INDENT_IFDEF  0x0008
#define INDENT_IFNDEF 0x0010

typedef struct define_s {
	char*			 name;	   // define name
	int				 flags;	   // define flags
	int				 builtin;  // > 0 if builtin define
	int				 numparms; // number of define parameters
	idToken*		 parms;	   // define parameters
	idToken*		 tokens;   // macro tokens (possibly containing parm tokens)
	struct define_s* next;	   // next defined macro in a list
	struct define_s* hashnext; // next define in the hash chain
} define_t;

typedef struct indent_s {
	int				 type;	 // indent type
	int				 skip;	 // true if skipping current indent
	idLexer*		 script; // script the indent was in
	struct indent_s* next;	 // next indent on the indent stack
} indent_t;

/*!
	\class idParser
	\brief A robust script parser and preprocessor for handling source code files and token-based parsing operations.

	The idParser class provides comprehensive parsing capabilities for source files and memory buffers, supporting preprocessor directives, macro expansion, and structured token reading. It manages
   multiple script sources, defines, and indentation levels while handling conditional compilation and error reporting. The parser supports loading files from disk or memory, parsing matrices, reading
   tokens with various validation options, and processing preprocessor directives like #define, #include, #ifdef, and #ifndef. It includes facilities for managing global defines, handling include
   paths, setting punctuation rules, and tracking parsing state through markers and indentation stacks. The class handles both basic token parsing and complex preprocessor evaluation including
   arithmetic expressions and conditional compilation.

*/
class idParser
{
public:
	/*!
		\brief Initializes a new instance of the idParser class.

		The constructor initializes all member variables to their default states. The loaded flag is set to false, indicating that no script has been loaded yet. The OSPath flag is set to false,
	   suggesting that the parser is not operating in OS path mode. The punctuations counter is initialized to zero. The flags variable is set to zero, meaning no special parsing flags are active. The
	   script stack, indent stack, define hash, defines list, tokens list, and marker pointer are all initialized to NULL, indicating that these structures have not been allocated or populated yet.

	*/
	idParser();

	/*!
		\brief Initializes a new instance of the idParser class with the specified flags.

		The constructor initializes all member variables of the idParser class. It sets the loaded and OSPath flags to false, punctuations to zero, and initializes all pointer members to NULL. The
	   provided flags are stored for later use in parsing operations.

		\param flags Specifies the parsing flags that control the behavior of the parser
	*/
	idParser( int flags );

	/*!
		\brief Constructs an idParser object and loads the specified file.

		The constructor initializes the parser state and loads the content of the specified file into memory. It sets up internal data structures for parsing and handles the file loading process based
	   on the provided flags and OSPath parameter.

		\param filename The path to the file to be parsed
		\param flags Flags that control the parsing behavior
		\param OSPath Indicates whether the filename uses OS-specific path separators
	*/
	idParser( const char* filename, int flags = 0, bool OSPath = false );

	/*!
		\brief Constructs an idParser object to parse text from memory

		Initializes a new parser instance that can parse text data provided as a memory buffer. The parser is set up with the given pointer to memory, its length, and a name for identification.
	   Various internal state variables are initialized to their default values, and the memory is loaded into the parser for parsing operations.

		\param ptr Pointer to the memory buffer containing text to parse
		\param length Length of the memory buffer in bytes
		\param name Name identifier for the parser instance
		\param flags Parser flags that control parsing behavior
	*/
	idParser( const char* ptr, int length, const char* name, int flags = 0 );

	/*!
		\brief Destroys the idParser object and frees associated source memory.

		The destructor cleans up the resources used by the idParser instance. It calls FreeSource with false to release the memory allocated for the source data without resetting the parser state.

	*/
	~idParser();

	/*!
		\brief Loads a source file for parsing

		This function loads a source file for parsing by creating a new lexer instance and setting up the parser state. It checks if another source is already loaded and will cause a fatal error if
	   so. The function initializes various parser structures including defines hash, tokens, and indentation stack. It returns true on successful load, false otherwise.

		\param filename Name of the file to load
		\param OSPath Flag indicating if the path should be treated as an OS path
		\return True if the file was loaded successfully, false otherwise
		\throws FatalError if another source is already loaded
	*/
	int				LoadFile( const char* filename, bool OSPath = false );

	/*!
		\brief Loads a source file from memory with the specified pointer, length, and name.

		This function initializes a parser to read from a memory buffer that contains the source content. It ensures that no other source is currently loaded and sets up the lexer with appropriate
	   flags and punctuation settings. The function returns true on successful loading, or false if the loading fails or if another source is already loaded.

		\param ptr Pointer to the memory buffer containing the source content
		\param length Length of the memory buffer in bytes
		\param name Name of the source being loaded
		\return True if the source was successfully loaded, false otherwise.
		\throws FatalError if another source is already loaded.
	*/
	int				LoadMemory( const char* ptr, int length, const char* name );

	/*!
		\brief Frees all memory associated with the current source file, optionally preserving preprocessor defines.

		This function releases all resources allocated for parsing the current source file. It cleans up the lexer scripts, tokens, and indentation stack. If the keepDefines parameter is false, it
	   also frees the preprocessor defines and their hash table. The loaded flag is set to false after cleanup.

		\param keepDefines If true, preserves preprocessor defines; if false, frees all defines and their hash table
	*/
	void			FreeSource( bool keepDefines = false );

	/*!
		\brief Parses a one-dimensional matrix of floating-point values from input.

		This function reads a one-dimensional matrix of floating-point values from the input stream, expecting a specific format with braces and comma-separated values. It parses x values into the
	   provided array m. The function expects an opening brace '{', followed by x floating-point numbers separated by commas, and finally a closing brace '}'. Returns true if parsing is successful,
	   false otherwise.

		\param x The number of elements to parse into the matrix
		\param m Pointer to the array where the parsed values will be stored
		\return True if the parsing is successful, false otherwise
	*/
	int				Parse1DMatrixLegacy( int x, float* m );

	//! Returns true if a source is loaded
	int				IsLoaded() const { return idParser::loaded; }

	/*!
		\brief Reads the next token from the source, handling preprocessor directives, macro expansion, and string concatenation

		This function reads tokens from the source stream, processing precompiler directives and dollar directives when encountered. It handles macro expansion by looking up defined tokens in the hash
	   table and replacing them with their definitions. The function also concatenates adjacent string tokens and skips source content when conditional compilation is active. The function returns true
	   when a valid token is found, false if the end of source is reached or an error occurs

		\param token pointer to the token structure where the parsed token will be stored
		\return non-zero value indicating success when a token is successfully read, zero if end of source or error occurs
	*/
	int				ReadToken( idToken* token );

	/*!
		\brief Expects a specific token string from the parser and returns true if found, false otherwise

		This function attempts to read the next token from the parser and checks if it matches the expected string. If the token cannot be read or does not match the expected string, an error is
	   generated and the function returns false. Otherwise, it returns true indicating successful matching of the expected token string

		\param string The expected token string to match against the next token in the parser
		\return True if the next token matches the expected string, false otherwise
	*/
	int				ExpectTokenString( const char* string );

	/*!
		\brief Expects and validates a token of a specific type and subtype, returning 1 if successful or 0 if not.

		This function reads a token from the parser and verifies that its type matches the expected type. For number tokens, it also checks that the subtype matches the expected subtype. If the token
	   does not match the expected type or subtype, an error message is generated and the function returns 0. The function returns 1 upon successful validation of the token.

		\param type The expected token type.
		\param subtype The expected token subtype, used primarily for number tokens.
		\param token Pointer to the token structure to be filled with the parsed token.
		\return Returns 1 if the token is successfully read and matches the expected type and subtype, or 0 if the token does not match or if an error occurs during parsing.
		\throws The function may throw an error through idParser::Error if the token type or subtype does not match the expected values.
	*/
	int				ExpectTokenType( int type, int subtype, idToken* token );

	/*!
		\brief Reads and expects any token from the parser, returning true if successful or false if not.

		This function attempts to read a token using the parser's ReadToken method. If reading fails, it reports an error and returns false. Otherwise, it returns true to indicate successful token
	   reading. The function is commonly used when parsing structured data where specific tokens are expected in a particular order.

		\param token Pointer to an idToken object where the parsed token will be stored
		\return Integer value indicating success (true) or failure (false) of reading the expected token.
	*/
	int				ExpectAnyToken( idToken* token );

	/*!
		\brief Checks if the next token in the source matches the given string and removes it if it does.

		This function attempts to read the next token from the source and compares it with the provided string. If the token matches, it is consumed and the function returns true. Otherwise, the token
	   is returned to the source and the function returns false. This is commonly used for parsing structured text files where specific tokens denote the start of different sections or elements.

		\param string The string to compare the next token against
		\return True if the next token matches the provided string, false otherwise.
	*/
	int				CheckTokenString( const char* string );

	/*!
		\brief Checks if the next token matches the specified type and subtype, and if so, stores it in the provided token pointer.

		This function attempts to read the next token from the source. If the token matches the specified type and subtype, it stores the token in the provided token pointer and returns true.
	   Otherwise, it unreads the token and returns false. The subtype is used as a bitmask to match against the token's subtype. This function is commonly used for parsing structured input where
	   specific token types are expected in a sequence.

		\param type The expected token type to match against
		\param subtype The expected token subtype to match against, used as a bitmask
		\param token Pointer to store the matched token if the function returns true
		\return True if the next token matches the specified type and subtype, false otherwise.
	*/
	int				CheckTokenType( int type, int subtype, idToken* token );

	/*!
		\brief Checks if the next token in the parser matches the specified string without consuming it.

		This function attempts to read the next token from the parser and compares it to the provided string.
		If the token matches, the function returns true and leaves the token in the input stream.
		If the token does not match or if there is no token available, the function returns false.
		The function is useful for lookahead operations where you need to verify the presence of a specific token
		before deciding how to proceed with parsing.

		\param string The string to compare against the next token in the parser
		\return True if the next token matches the provided string, false otherwise
	*/
	int				PeekTokenString( const char* string );

	/*!
		\brief Checks if the next token matches the specified type and subtype without removing it from the source.

		This function reads the next token from the parser without consuming it, then compares its type and subtype against the provided parameters. If there is a match, the token is copied to the
	   output parameter and the function returns true. Otherwise, it returns false and the original token is restored to the source stream.

		\param type Expected token type to match
		\param subtype Expected token subtype to match (bitmask operation)
		\param token Output parameter to store the matching token if found
		\return True if a matching token is found and copied to the output parameter, false otherwise
	*/
	int				PeekTokenType( int type, int subtype, idToken* token );

	/*!
		\brief Skips tokens in the parser until the specified string is found and returns whether the string was found.

		This function advances through tokens in the parser, reading each token until it encounters the specified string. It returns true if the string is found and false if the end of the token
	   stream is reached without finding the string. The function is typically used to skip over tokens in a parser until a particular delimiter or keyword is encountered. The string parameter is
	   expected to be a null-terminated C-string that identifies the target token to stop at.

		\param string The null-terminated C-string to search for in the token stream
		\return An integer value of 1 if the string was found in the token stream, or 0 if the end of the stream was reached without finding the string
	*/
	int				SkipUntilString( const char* string );

	/*!
		\brief Skips the remainder of the current line in the parser input

		Reads tokens from the parser input until it reaches the end of the current line or the end of the input. When a token is encountered that spans across multiple lines, the function unreads that
	   token and returns true. If the end of the input is reached without encountering a multi-line token, the function returns false.

		\return The function returns true if a token that crosses multiple lines was encountered and unread, false otherwise
		\throws This function may throw an exception if the parser encounters an error while reading tokens
	*/
	int				SkipRestOfLine();

	/*!
		\brief Skips over a braced section in the parser input, handling nested braces correctly.

		This function advances through the parser's input stream, skipping all tokens until the matching closing brace is found. It correctly handles nested braces by maintaining a depth counter. If
	   parseFirstBrace is true, it assumes the opening brace has already been consumed and starts with depth 0. Otherwise, it starts with depth 1 to account for the initial opening brace.

		\param parseFirstBrace If true, assumes the first brace is already parsed and starts counting from depth 0; if false, starts counting from depth 1
		\return Returns 1 if the braced section was successfully skipped, 0 if the end of input was reached before the closing brace
	*/
	int				SkipBracedSection( bool parseFirstBrace = true );

	/*!
		\brief Parses a braced section from the input and stores it in the provided string.

		This function reads tokens from the input stream until a matching closing brace is found, handling nested braces by tracking depth. It supports optional tab indentation and can parse an
	   initial opening brace if requested. The parsed content is appended to the provided string with proper formatting including newlines and tabs. If the closing brace is not found, an error is
	   reported.

		\param out The string to store the parsed braced section into
		\param tabs The number of tabs to use for indentation, or -1 to disable tab handling
		\param parseFirstBrace Whether to parse the initial opening brace
		\param intro The character that starts a braced section
		\param outro The character that ends a braced section
		\return A pointer to the parsed string.
		\throws Error is thrown if a matching closing brace is not found.
	*/
	const char*		ParseBracedSection( idStr& out, int tabs, bool parseFirstBrace, char intro, char outro );

	//! Parses a braced section from the script into the provided string while maintaining indentation and newlines.
	const char*		ParseBracedSectionExact( idStr& out, int tabs = -1 );

	/*!
		\brief Parses the rest of the current line into the provided string output.

		This function reads tokens from the parser's input until it reaches the end of the current line or encounters a token that crosses multiple lines. It concatenates all the tokens into the
	   output string, separating them with spaces. The function returns a pointer to the internal character array of the output string.

		\param out The string to store the parsed tokens from the rest of the line
		\return A pointer to the character array of the output string containing the parsed tokens.
	*/
	const char*		ParseRestOfLine( idStr& out );

	/*!
		\brief Undoes the reading of the specified token, making it available for subsequent reading.

		This function reverses the effect of reading a token by returning it to the parser's input stream. It is typically used when a token has been read but needs to be re-read or processed
	   differently. The function simply delegates to the internal UnreadSourceToken method to perform the actual operation.

		\param token The token to be unread, which must have been previously read from the parser.
	*/
	void			UnreadToken( idToken* token );

	/*!
		\brief Reads a token from the current line only if it is on the same line as the current position.

		This function attempts to read a token from the input source. If a token is successfully read and it does not cross any lines, the token is copied to the provided token pointer and the
	   function returns true. If the token crosses lines, it is unread and the function returns false. This ensures that only tokens that are entirely contained on the current line are accepted.

		\param token Pointer to an idToken where the read token will be stored if successful
		\return True if a token was read and it did not cross lines, false otherwise
	*/
	int				ReadTokenOnLine( idToken* token );

	/*!
		\brief Parses and returns a signed integer value from the token stream

		This function reads a token from the parser's input stream and attempts to interpret it as a signed integer. If the token is a minus sign followed by a number, it will parse the following
	   number as a negative integer. If the token is not a valid integer, it will generate an error message and return zero. The function handles both positive and negative integers and will report an
	   error for floating point numbers or invalid tokens

		\return The parsed signed integer value from the token stream
	*/
	int				ParseInt();

	/*!
		\brief Parses a boolean value from the token stream.

		This function reads a token from the parser and expects it to be a number. It then converts the number to a boolean value, returning true if the number is non-zero and false if it is zero. If
	   the expected number token is not found, an error is issued and false is returned.

		\return true if the parsed number is non-zero, false otherwise
	*/
	bool			ParseBool();

	/*!
		\brief Parses and returns a floating point number from the input stream

		This function reads a token from the input stream and attempts to interpret it as a floating point number. If the token is a punctuation mark representing a negative sign, it will read the
	   following number and return its negative value. If the token is not a number, it will generate an error message. The function returns 0.0f if no token can be read or if an error occurs during
	   parsing.

		\return The parsed floating point number from the input stream
		\throws An error is thrown if the expected floating point number is not found or if the token is not of a numeric type
	*/
	float			ParseFloat();

	/*!
		\brief Parses a 1D matrix of floats from the input stream

		This function parses a 1D matrix of floating point values from the input stream. It expects an opening parenthesis, followed by x float values, and then a closing parenthesis. The parsed
	   values are stored in the provided float array m. The function returns true if parsing is successful, false otherwise.

		\param x Number of float values to parse
		\param m Pointer to the array where parsed float values will be stored
		\return true if parsing is successful, false otherwise
	*/
	int				Parse1DMatrix( int x, float* m );

	/*!
		\brief Parses a 2D matrix of specified dimensions from the input stream

		This function reads a 2D matrix of y rows and x columns from the input stream and stores the parsed values in the provided float array m. The matrix is expected to be enclosed in parentheses.
	   The function first validates the opening parenthesis, then parses each row using Parse1DMatrix, and finally validates the closing parenthesis. It returns true if the entire matrix is
	   successfully parsed, false otherwise.

		\param y number of rows in the matrix
		\param x number of columns in the matrix
		\param m pointer to the float array where the matrix data will be stored
		\return true if the 2D matrix was successfully parsed and stored, false otherwise
	*/
	int				Parse2DMatrix( int y, int x, float* m );

	/*!
		\brief Parses a 3D matrix of specified dimensions from the input stream.

		This function reads a 3D matrix from the input stream, where the matrix is composed of multiple 2D matrices. The dimensions of the 3D matrix are defined by the z, y, and x parameters. It
	   expects an opening parenthesis before parsing and a closing parenthesis after parsing. Each 2D matrix is parsed using the Parse2DMatrix function. The function returns false if any parsing step
	   fails, otherwise it returns true.

		\param z the number of 2D matrices along the z-axis
		\param y the number of rows in each 2D matrix
		\param x the number of columns in each 2D matrix
		\param m pointer to the memory where the parsed 3D matrix will be stored
		\return true if the 3D matrix was parsed successfully, false otherwise
	*/
	int				Parse3DMatrix( int z, int y, int x, float* m );

	/*!
		\brief Retrieves the whitespace characters preceding the last read token from the script stack.

		This function obtains the whitespace that appears before the last token read by the parser. It populates the provided idStr object with the whitespace characters. If there is no active script
	   stack, the whitespace string is cleared. The function returns the length of the whitespace string.

		\param whiteSpace Output parameter that will be filled with the whitespace characters preceding the last read token
		\return The length of the whitespace string retrieved from the script stack or cleared if no script stack is available.
	*/
	int				GetLastWhiteSpace( idStr& whiteSpace ) const;

	/*!
		\brief Sets a marker in the source file at the current position.

		This function establishes a marker at the current parsing position in the source file. The marker can be used later to return to this specific location. Only one marker is supported at a time.

	*/
	void			SetMarker();

	/*!
		\brief Extracts a string from a marker position to the current position in the script buffer

		This function retrieves a substring from the marker position to the current parsing position in the script buffer. It temporarily null-terminates the buffer to isolate the desired segment,
	   then restores the original character. When the clean flag is true, it re-parses the extracted text to remove extraneous whitespace and formatting. Otherwise, it returns the raw text segment
	   as-is.

		\param out Output string to store the extracted text
		\param clean Flag indicating whether to reparse and clean the extracted text
	*/
	void			GetStringFromMarker( idStr& out, bool clean = false );

	/*!
		\brief Adds a define to the parser's source

		This function takes a string representation of a define and adds it to the parser's internal hash table for tracking. It first converts the string into a define_t structure using
	   DefineFromString, and if successful, adds the define to the hash table. The function returns true if the define was successfully added, and false otherwise.

		\param string The string representation of the define to be added
		\return true if the define was successfully added to the parser's source, false otherwise
	*/
	int				AddDefine( const char* string );

	/*!
		\brief Adds predefined builtin defines to the parser's hash table

		This function initializes and adds several predefined builtin defines to the parser's hash table. It creates define_t structures for built-in identifiers such as __LINE__, __FILE__, __DATE__,
	   __TIME__, and __STDC__. Each define is allocated memory, its properties are set including name, flags, builtin id, and parameters, then added to the parser's define hash table. The function
	   handles memory allocation using Mem_Alloc and ensures proper string copying for the define names.

	*/
	void			AddBuiltinDefines();

	/*!
		\brief Sets the include path for the parser and ensures it ends with a path separator.

		This function assigns the provided path to the internal include path variable of the parser. It then checks if the last character of the path is neither a backslash nor a forward slash, and if
	   so, appends the appropriate path separator to ensure consistent path handling.

		\param path The path to be set as the include path for the parser.
	*/
	void			SetIncludePath( const char* path );

	/*!
		\brief Sets the punctuation set for the parser.

		This function assigns the provided punctuation set to the parser's internal punctuation table. The punctuation set defines the characters that are treated as punctuation during parsing
	   operations.

		\param p Pointer to an array of punctuation_t structures that define the punctuation set to be used by the parser
	*/
	void			SetPunctuations( const punctuation_t* p );

	/*!
		\brief Returns a pointer to the punctuation string with the given id

		This function retrieves the punctuation string associated with a given integer identifier. If the punctuations array is not initialized, it falls back to using an idLexer instance to obtain
	   the punctuation. The function iterates through the initialized punctuations array to find a match for the provided id. If no match is found, it returns a default string "unknown punctuation".

		\param id integer identifier for the punctuation to retrieve
		\return pointer to the punctuation string corresponding to the given id, or a default string if not found
	*/
	const char*		GetPunctuationFromId( int id );

	/*!
		\brief Returns the identifier for the given punctuation string.

		This function looks up a punctuation string in the parser's punctuation table and returns its associated identifier. If the punctuation table is not initialized, it falls back to using a
	   temporary lexer instance. The function iterates through the punctuation table until it finds a match for the given string, returning the corresponding identifier. If no match is found, it
	   returns zero.

		\param p The punctuation string to look up
		\return The identifier associated with the punctuation string, or zero if not found
	*/
	int				GetPunctuationId( const char* p );

	/*!
		\brief Sets the lexer flags for the parser and all scripts in the script stack

		This function updates the global flags used by the parser and propagates the same flags to all lexer instances in the script stack. It ensures consistent lexer behavior across all active
	   scripts in the parsing context.

		\param flags The flags to set for the lexer
	*/
	void			SetFlags( int flags );

	//! Returns the lexer flags of the parser.
	int				GetFlags() const;

	//! Returns the current filename being parsed.
	const char*		GetFileName() const;

	//! Returns the current offset in the current script.
	const int		GetFileOffset() const;

	//! Returns the file time for the current script
	const ID_TIME_T GetFileTime() const;

	//! Returns the current line number from the parser's script stack or zero if none is available.
	const int		GetLineNum() const;

	/*!
		\brief Reports an error message with formatted arguments

		This function formats an error message using a printf-style format string and variable arguments, then passes it to the current script parser's error handler if one exists. It is typically
	   used for reporting syntax errors or other issues during parsing of script files.

		\param str Format string for the error message, followed by variable arguments
	*/
	void			Error( VERIFY_FORMAT_STRING const char* str, ... ) const;

	/*!
		\brief Prints a warning message using a format string and variable arguments.

		This function takes a format string and a variable number of arguments, formats them into a warning message, and prints the message using the current script stack's warning mechanism. The
	   formatted message is limited to MAX_STRING_CHARS characters. The function is typically used for reporting non-fatal issues or deprecated functionality.

		\param str Format string for the warning message, which may contain format specifiers.
	*/
	void			Warning( VERIFY_FORMAT_STRING const char* str, ... ) const;

	/*!
		\brief Returns true if the parser has reached the end of the file

		This function checks whether the current script stack has reached the end of the file. If there is an active script stack, it delegates the check to the script stack's EndOfFile method. If
	   there is no script stack, it returns true, indicating the end of the file has been reached.

		\return true if the parser has reached the end of the file, false otherwise
	*/
	bool			EndOfFile();

	/*!
		\brief Adds a global define to all opened sources

		This function takes a string representation of a define and adds it to the list of global defines that will be automatically included in all opened source files. The function parses the string
	   to create a define_t structure, then inserts it at the beginning of the globaldefines linked list. If the parsing fails, the function returns false, otherwise it returns true after successfully
	   adding the define.

		\param string String representation of the define to be added
		\return Integer value indicating success (true) or failure (false) of the operation
	*/
	static int		AddGlobalDefine( const char* string );

	/*!
		\brief Removes a global define with the specified name from the parser's global defines list

		This function searches for a global define with the given name in the parser's global defines list and removes it if found. The function traverses the linked list of defines, updates the links
	   to remove the node, and then frees the memory associated with the define. If the define is not found, the function returns false. The function returns true if the define was successfully
	   removed

		\param name name of the global define to remove
		\return 1 if the global define was successfully removed, 0 if the define was not found
	*/
	static int		RemoveGlobalDefine( const char* name );

	/*!
		\brief Removes all global defines from the parser

		This function iterates through all global defines stored in the globaldefines list and frees each one. It clears the entire global defines list by resetting the globaldefines pointer to NULL
	   after freeing all entries.

	*/
	static void		RemoveAllGlobalDefines();

	/*!
		\brief Sets the base folder path from which files will be loaded.

		This function configures the base directory path that the parser will use when loading files. It delegates the actual implementation to the underlying idLexer::SetBaseFolder method.

		\param path The path to the base folder from which files will be loaded
	*/
	static void		SetBaseFolder( const char* path );

	// RB: made protected to have custom #include behaviours for embedded resources
protected:
	// RB end
	int					 loaded;	   // set when a source file is loaded from file or memory
	idStr				 filename;	   // file name of the script
	idStr				 includepath;  // path to include files
	bool				 OSPath;	   // true if the file was loaded from an OS path
	const punctuation_t* punctuations; // punctuations to use
	int					 flags;		   // flags used for script parsing
	idLexer*			 scriptstack;  // stack with scripts of the source
	idToken*			 tokens;	   // tokens to read first
	define_t*			 defines;	   // list with macro definitions
	define_t**			 definehash;   // hash chain with defines
	indent_t*			 indentstack;  // stack with indents
	int					 skip;		   // > 0 if skipping conditional code
	const char*			 marker_p;

	static define_t*	 globaldefines; // list with global defines added to every source loaded

	/*!
		\brief Pushes a new indentation level onto the parser's indentation stack with the specified type and skip values.

		This function allocates memory for a new indentation entry and initializes it with the provided type and skip values. The skip value is converted to a boolean before being stored. The function
	   then updates the global skip counter and adds the new entry to the top of the indentation stack. The type parameter determines the kind of indentation being pushed, while the skip parameter
	   indicates whether to skip certain parsing operations at this indentation level.

		\param type Type of the indentation level to be pushed
		\param skip Value indicating whether to skip parsing operations at this indentation level
	*/
	void				 PushIndent( int type, int skip );

	/*!
		\brief Pops the top indentation level from the parser's indentation stack and returns its type and skip values.

		This function removes the top entry from the indentation stack used by the parser to track indentation levels. It retrieves the type and skip values from the popped indentation entry and
	   stores them in the provided output parameters. The function also updates the global skip counter and frees the memory allocated for the indentation entry.

		\param type Output parameter that will receive the type of the popped indentation level
		\param skip Output parameter that will receive the skip value of the popped indentation level
	*/
	void				 PopIndent( int* type, int* skip );

	/*!
		\brief Pushes a script onto the parser's script stack after checking for recursive inclusion.

		This function adds a given lexer script to the top of the parser's script stack. It first checks if the script is already present in the stack to prevent recursive inclusion. If a recursion is
	   detected, a warning is issued and the function returns without adding the script. The script is then inserted at the beginning of the stack, making it the current script to be parsed.

		\param script The lexer script to be pushed onto the script stack
		\throws Warning is issued if recursive inclusion is detected
	*/
	void				 PushScript( idLexer* script );

	/*!
		\brief Reads the next token from the source script and returns it in the provided token parameter

		This function retrieves the next available token from the parser's token stream. If no tokens are currently available, it attempts to read from the current script. The function handles script
	   stack management, including switching to previous scripts when the current one ends. It manages script indentation and warnings for missing #endif directives. The function returns true if a
	   token was successfully read or false if the end of all scripts has been reached.

		\param token Pointer to the token structure where the parsed token will be stored
		\return Integer value indicating success (true) or failure (false) of the token reading operation
		\throws FatalError exception when the parser is not properly initialized
	*/
	int					 ReadSourceToken( idToken* token );

	/*!
		\brief Reads a line of tokens from the source, handling continuation lines.

		This function reads tokens from the source until it encounters a line that does not have a continuation character. It tracks the number of lines crossed and returns false if a token crosses
	   more lines than expected. The function handles line continuation by checking for the backslash character at the end of tokens.

		\param token Pointer to the token structure to store the read token.
		\return Returns true if the line was read successfully, false otherwise.
	*/
	int					 ReadLine( idToken* token );

	/*!
		\brief Unreads a token back onto the parser's token stream

		This function takes a token and pushes it back onto the front of the parser's token list, effectively making it available for the next token read operation. The token is duplicated using the
	   TAG_IDLIB_PARSER memory tag to ensure proper memory management within the parser's context.

		\param token The token to be unread back onto the parser's token stream
		\return Always returns true to indicate the operation was successful
	*/
	int					 UnreadSourceToken( idToken* token );

	/*!
		\brief Reads and parses the parameters for a preprocessor define statement

		This function reads the parameters for a preprocessor define statement from the source code, handling nested parentheses and expanding any nested defines. It validates that the number of
	   parameters matches the expected count and properly stores the parsed parameters in the provided array. The function manages the parser state and handles error conditions such as missing
	   parentheses, too many or too few parameters, and incomplete define statements.

		\param define The define statement being processed
		\param parms Array to store the parsed parameter tokens
		\param maxparms Maximum number of parameters allowed for this define
		\return Returns true if the parameters were successfully read and parsed, false otherwise
	*/
	int					 ReadDefineParms( define_t* define, idToken** parms, int maxparms );

	/*!
		\brief Converts a list of tokens into a single string token

		This function takes a linked list of tokens and concatenates their string representations into a single token. The input token is modified in place to contain the concatenated result. The
	   function sets the type of the output token to TT_STRING and clears any whitespace information. It iterates through the input token list and appends each token's string content to the output
	   token.

		\param tokens Pointer to the first token in a linked list of tokens to be stringized
		\param token Pointer to the output token that will contain the concatenated result
		\return The function returns 1 (true) to indicate successful completion of the stringization process
	*/
	int					 StringizeTokens( idToken* tokens, idToken* token );

	/*!
		\brief Merges the content of two tokens into the first token if their types are compatible.

		This function attempts to merge the content of the second token into the first token under specific conditions. The merge is allowed when the first token is a name and the second token is
	   either a name or an integer number, when both tokens are strings, or when both tokens are numbers with compatible types. The function modifies the first token in place and returns true if the
	   merge was successful, otherwise false.

		\param t1 Pointer to the first token whose content will be extended
		\param t2 Pointer to the second token whose content will be appended to the first
		\return True if the tokens were successfully merged, false otherwise
	*/
	int					 MergeTokens( idToken* t1, idToken* t2 );

	/*!
		\brief Expands a builtin define token into a sequence of tokens representing the define's value.

		This function processes a builtin define token and generates corresponding tokens based on the type of builtin. It supports builtins for line number, file name, date, and time. The function
	   allocates a new token and sets its properties according to the builtin type. For the __STDC__ builtin, it issues a warning and returns null tokens. The function returns true to indicate
	   successful processing.

		\param deftoken The token representing the define being expanded
		\param define The define structure containing the builtin type information
		\param firsttoken Pointer to store the first token of the expanded sequence
		\param lasttoken Pointer to store the last token of the expanded sequence
		\return Returns 1 to indicate successful processing of the builtin define expansion.
	*/
	int					 ExpandBuiltinDefine( idToken* deftoken, define_t* define, idToken** firsttoken, idToken** lasttoken );

	/*!
		\brief Expands a define token into a list of tokens, handling parameters, stringizing, and merging operators.

		This function processes a define token and expands it into a list of tokens based on the define's definition. It handles both parameterized and non-parameterized defines, processes stringizing
	   operators (#) and merging operators (##), and manages memory for temporary tokens during expansion.

		\param deftoken The token representing the define being expanded
		\param define The define structure containing the definition to be expanded
		\param firsttoken Pointer to store the first token of the expanded result
		\param lasttoken Pointer to store the last token of the expanded result
		\return Returns 1 if the expansion was successful, 0 otherwise
	*/
	int					 ExpandDefine( idToken* deftoken, define_t* define, idToken** firsttoken, idToken** lasttoken );

	/*!
		\brief Expands a define token into the parser source by replacing it with tokens from the define

		This function takes a define token and expands it into the parser's source by replacing the token with the tokens associated with the define. It first attempts to expand the define and if
	   successful, it adjusts the line counts of the expanded tokens and links them into the parser's token stream. The function returns true if the expansion was successful, and false otherwise.

		\param deftoken The token representing the define to be expanded
		\param define The define structure containing the expansion tokens
		\return True if the define was successfully expanded and inserted into the source, false otherwise
	*/
	int					 ExpandDefineIntoSource( idToken* deftoken, define_t* define );

	/*!
		\brief Copies global defines from the global defines list to the parser's define hash table

		This function iterates through all global defines stored in the globaldefines list and creates copies of each define. Each copied define is then added to the parser's define hash table. This
	   allows the parser to have access to all global preprocessor definitions during parsing operations.

	*/
	void				 AddGlobalDefinesToSource();

	/*!
		\brief Creates a deep copy of a define_t structure including its name, flags, tokens, and parameters

		This function allocates memory for a new define_t structure and copies all fields from the input define_t structure. It duplicates the define name, flags, builtin flag, and number of
	   parameters. The function also performs a deep copy of the token lists for both the define tokens and parameters by creating new idToken objects for each token in the original lists. The new
	   structure is not linked to any hash table or chain, and all copied tokens maintain their original order and content

		\param define Pointer to the define_t structure to be copied
		\return Pointer to the newly allocated and copied define_t structure, or NULL if allocation fails
	*/
	define_t*			 CopyDefine( define_t* define );

	/*!
		\brief Finds a hashed define by its name in the provided hash table

		This function searches for a define_t structure in the hash table using the provided name. It calculates the hash value of the name and traverses the linked list at that hash index to find a
	   match. The function returns a pointer to the matching define_t structure if found, or NULL if no match is found.

		\param definehash Pointer to the hash table of define_t structures
		\param name Name of the define to search for
		\return Pointer to the found define_t structure, or NULL if no matching define is found
	*/
	define_t*			 FindHashedDefine( define_t** definehash, const char* name );

	/*!
		\brief Finds the index of a parameter with the specified name in a define structure

		This function searches through the parameter list of a define structure to locate a parameter with a matching name. It iterates through the linked list of parameters and compares each
	   parameter name with the provided name. The function returns the zero-based index of the first matching parameter, or -1 if no match is found.

		\param define Pointer to the define structure containing the parameter list to search
		\param name The name of the parameter to find
		\return The zero-based index of the parameter if found, or -1 if no matching parameter is found
	*/
	int					 FindDefineParm( define_t* define, const char* name );

	/*!
		\brief Adds a define to the hash table for fast lookup

		This function inserts a define entry into a hash table structure to enable efficient retrieval of preprocessor definitions. It calculates the hash value of the define's name and places the
	   define at the head of the corresponding linked list in the hash table

		\param define Pointer to the define entry to be added to the hash table
		\param definehash Pointer to the hash table array where defines are stored
	*/
	void				 AddDefineToHash( define_t* define, define_t** definehash );

	/*!
		\brief Prints the details of a define_t structure to the common output

		This function outputs the name, flags, builtin status, and number of parameters for a given define_t structure to the common output stream

		\param define Pointer to the define_t structure to be printed
	*/
	static void			 PrintDefine( define_t* define );

	/*!
		\brief Frees all memory associated with a define_t structure by deallocating its parameters, tokens, and the structure itself

		This function cleans up a define_t structure by first freeing all the parameter tokens in the parms linked list, then freeing all the token tokens in the tokens linked list, and finally
	   freeing the define_t structure itself using Mem_Free

		\param define Pointer to the define_t structure to be freed
	*/
	static void			 FreeDefine( define_t* define );

	/*!
		\brief Finds a define with the specified name in the given list of defines

		This function searches through a linked list of define_t structures to find one that matches the provided name. It performs a string comparison between the name field of each define_t
	   structure and the input name parameter. The function returns a pointer to the matching define_t structure if found, or NULL if no match is found.

		\param defines Pointer to the first define_t structure in the linked list to search through
		\param name The name of the define to search for
		\return Pointer to the define_t structure that matches the specified name, or NULL if no match is found
	*/
	static define_t*	 FindDefine( define_t* defines, const char* name );

	/*!
		\brief Creates a define_t object from a string representation

		This function parses a string to create a define_t object. It uses an idParser to load the string as source material and processes it to create a define directive. If the parsing fails at any
	   point, it returns NULL. The function takes ownership of the parsing process and cleans up resources after creating the define object.

		\param string The string representation of the define to create
		\return A pointer to the newly created define_t object, or NULL if creation failed
		\throws NULL is returned if the string cannot be loaded or parsed correctly
	*/
	static define_t*	 DefineFromString( const char* string );

	/*!
		\brief Returns a copy of the first define from the parser's hash table

		This function iterates through the parser's define hash table to find the first non-null entry. When it finds the first define, it creates and returns a copy of that define using the
	   CopyDefine function. If no defines are found in the hash table, it returns NULL.

		\return A pointer to a copy of the first define in the hash table, or NULL if no defines exist
	*/
	define_t*			 CopyFirstDefine();

	/*!
		\brief Processes an include directive to load and parse another source file

		This function handles the #include directive by reading the file name from the token and attempting to load the specified file using multiple search paths. It first tries to load the file
	   relative to the current file, then as a full OS path, and finally from the include path. For angle bracket includes, it reads the entire path between the brackets and attempts to load it from
	   the include path. The function returns false if the file cannot be found or loaded, and true upon successful loading. The script is then pushed onto the script stack for parsing

		\param token Pointer to the token containing the include directive and file name
		\param supressWarning Flag to suppress warnings when the file cannot be found
		\return Returns 1 if the include file was successfully loaded and parsed, 0 if it failed to load
	*/
	virtual int			 Directive_include( idToken* token, bool supressWarning = false );

	/*!
		\brief Removes a previously defined macro or constant from the parser's define hash table

		This function processes an undef directive by reading the name of the macro or constant to be undefined from the input stream. It then searches through the hash table to find and remove the
	   corresponding definition. If the definition is marked as fixed, a warning is issued and the definition is not removed. The function handles proper linking of hash chain nodes during removal to
	   maintain the integrity of the hash table structure.

		\return Returns true if the undef directive was processed successfully, false otherwise
	*/
	int					 Directive_undef();

	/*!
		\brief Processes a #ifdef directive by checking if a define exists and managing conditional compilation indentation.

		This function handles the #ifdef preprocessor directive by reading the next token to identify the define name, checking if the define exists in the hash table, and then determining whether to
	   skip the following code block based on the existence of the define. It manages the indentation stack to control conditional compilation behavior.

		\param type Type of directive being processed, typically INDENT_IFDEF or related constants.
		\return Returns true if the directive was processed successfully, false otherwise.
		\throws Throws an error if the directive is malformed or if the required name is not found.
	*/
	int					 Directive_if_def( int type );

	//! Processes an ifdef directive and returns the result of the if_def directive processing.
	int					 Directive_ifdef();

	//! Processes a preprocessor directive to conditionally include code blocks if a given identifier is not defined.
	int					 Directive_ifndef();

	/*!
		\brief Processes the #else directive in the parser, handling conditional compilation logic.

		This function handles the preprocessor #else directive by checking the current indentation state and ensuring proper nesting of conditional compilation blocks. It pops the current indentation
	   state and validates that the #else is not misplaced or duplicated. If the validation passes, it pushes a new indentation state for the #else block.

		\return The function returns an integer value indicating success (true) or failure (false) of the operation.
		\throws The function may throw an error if the #else directive is misplaced or if there are issues with indentation handling.
	*/
	int					 Directive_else();

	/*!
		\brief Processes a preprocessor conditional compilation #endif directive

		This function handles the preprocessor endif directive by popping the current indentation level and checking if it matches a corresponding start directive. It ensures proper nesting of
	   conditional compilation blocks and reports an error if the endif is misplaced.

		\return Returns true if the endif directive is properly matched with a corresponding start directive, false otherwise
		\throws Throws an error if the endif directive is misplaced
	*/
	int					 Directive_endif();

	/*!
		\brief Evaluates a sequence of tokens for preprocessor conditional compilation expressions

		This function processes a linked list of tokens representing a preprocessor conditional expression and evaluates the result. It handles various token types including numbers, names (like
	   defined macros), and punctuation. The function supports arithmetic operations, logical operations, and bitwise operations. It maintains separate stacks for values and operators, resolving
	   expressions according to operator precedence and parentheses nesting. The result is stored in either integer or floating-point format depending on the evaluation context.

		\param tokens Linked list of tokens representing the expression to evaluate
		\param intvalue Pointer to store the integer result of the evaluation
		\param floatvalue Pointer to store the floating-point result of the evaluation
		\param integer Flag indicating whether to evaluate in integer context (non-zero) or floating-point context (zero)
		\return Returns the evaluation result code, typically 0 for success or non-zero for errors during parsing or evaluation
	*/
	int					 EvaluateTokens( idToken* tokens, signed int* intvalue, double* floatvalue, int integer );

	/*!
		\brief Evaluates a mathematical expression from parsed tokens and stores the result in either an integer or floating-point variable

		This function processes a series of tokens read from a parser to evaluate a mathematical expression. It handles both integer and floating-point values, as well as defined constants and the
	   'defined' keyword for preprocessor-like evaluations. The tokens are parsed and expanded from defines, then evaluated into the provided integer or floating-point output variables. The function
	   supports both integer and floating-point evaluation based on the integer parameter, with the result stored in the appropriate output variable.

		\param intvalue Pointer to store the integer result of the evaluation
		\param floatvalue Pointer to store the floating-point result of the evaluation
		\param integer Flag indicating whether to evaluate as integer (non-zero) or floating-point (zero)
		\return Returns true if the expression was successfully parsed and evaluated, false otherwise
		\throws Throws an error if the expression cannot be evaluated or if required tokens are missing
	*/
	int					 Evaluate( signed int* intvalue, double* floatvalue, int integer );

	/*!
		\brief Evaluates a mathematical expression from tokens and stores the result as either an integer or floating-point value.

		This function processes tokens from the parser to evaluate a mathematical expression. It handles named tokens by looking them up in the define hash table and expanding them into the source.
	   The function also processes parentheses to handle operator precedence and supports both integer and floating-point evaluation based on the integer parameter. The result is stored in either
	   intvalue or floatvalue depending on the evaluation mode.

		\param intvalue Pointer to store the integer result of the evaluation
		\param floatvalue Pointer to store the floating-point result of the evaluation
		\param integer Flag indicating whether to evaluate as integer (non-zero) or floating-point (zero)
		\return Returns true if the evaluation was successful, false otherwise.
		\throws This function may throw an error if the expression is malformed or if required tokens are missing, and it may issue warnings for undefined names.
	*/
	int					 DollarEvaluate( signed int* intvalue, double* floatvalue, int integer );

	/*!
		\brief Parses and processes a #define directive from the source code

		This function handles the parsing of preprocessor #define directives. It reads the define name, checks for existing definitions, and processes either parameterized or simple defines. The
	   function manages memory allocation for the define structure and handles recursive define detection. It reads tokens until the end of the define definition and properly sets up the define's
	   token list with appropriate flags for recursive detection.

		\return Returns 1 if the #define directive was processed successfully, 0 if there was an error during processing
	*/
	int					 Directive_define();

	/*!
		\brief Processes a #elif directive by evaluating its condition and setting up the appropriate indentation for conditional compilation.

		This function handles the preprocessor directive #elif which is used in conditional compilation. It pops the current indentation state to verify the directive's placement, evaluates the
	   condition specified in the directive, and then sets up the new indentation state for the elif branch. If the condition evaluates to zero, the code following this directive will be skipped
	   during compilation. The function ensures proper nesting and placement of conditional compilation directives.

		\return Returns 1 (true) if the #elif directive was processed successfully, 0 (false) otherwise.
	*/
	int					 Directive_elif();

	/*!
		\brief Evaluates a conditional expression and sets up indentation for conditional compilation.

		This function evaluates a conditional expression and determines whether to skip subsequent code blocks based on the evaluation result. If the expression evaluates to zero, it sets a skip flag
	   which will cause subsequent code blocks to be ignored during parsing. The function also pushes an indentation level to track the conditional block.

		\return Returns true if the conditional expression was evaluated successfully, false otherwise.
	*/
	int					 Directive_if();

	/*!
		\brief Parses and handles the #line directive, reporting an error and skipping the line.

		This function is responsible for processing the #line preprocessor directive. It reports an error indicating that the #line directive is not supported, then reads and discards the rest of the
	   line. The function is typically used in the context of parsing C-style code where preprocessor directives are encountered.

		\return The function always returns true, indicating that the parsing of the #line directive was completed, even though the directive itself is not supported.
		\throws The function may throw an error if the #line directive is encountered during parsing.
	*/
	int					 Directive_line();

	/*!
		\brief Parses and reports an error directive from the input stream.

		This function reads a line from the parser input, expecting it to contain a string token. If the token is not found or is not of string type, it reports an error indicating that the #error
	   directive is missing a string. Otherwise, it reports the error with the provided string message.

		\return The function returns true if the error directive was successfully parsed and reported, false otherwise.
		\throws This function may throw an error if the input does not conform to the expected format for an error directive.
	*/
	int					 Directive_error();

	/*!
		\brief Handles the #warning directive by issuing a warning message and consuming the remainder of the line.

		This function processes the #warning directive encountered by the parser. It reads the next line and expects a string token to be present. If no string token is found, it issues a warning
	   indicating that the #warning directive is missing a string. If a string token is found, it issues a warning with the string content. The function consumes all tokens on the current line until
	   the end of the line is reached. The function always returns true to indicate successful completion of the directive handling.

		\return The function always returns true to indicate successful completion of the directive processing.
		\throws This function does not explicitly throw any exceptions, but may emit a warning through the parser's warning system.
	*/
	int					 Directive_warning();

	/*!
		\brief Handles the #pragma directive by issuing a warning and consuming the remainder of the line.

		This function processes the #pragma directive encountered by the parser. It issues a warning indicating that the #pragma directive is not supported, then consumes all tokens on the current
	   line until the end of the line is reached. The function always returns true to indicate completion of the directive handling.

		\return The function always returns true to indicate successful completion of the directive processing.
		\throws This function does not explicitly throw any exceptions, but may emit a warning through the parser's warning system.
	*/
	int					 Directive_pragma();

	/*!
		\brief Undoes the reading of a sign token by pushing a punctuation token with a minus sign back onto the token stream.

		This function is used to revert the effect of reading a sign token. It creates a temporary token representing a minus sign with the appropriate type and subtype, and then pushes this token
	   back onto the input stream so that subsequent reads will encounter it again. This is typically used in parsing scenarios where a token needs to be "un-read" to allow for alternative parsing
	   paths.

	*/
	void				 UnreadSignToken();

	/*!
		\brief Evaluates a numeric expression and adds the result as a token to the parser's input stream

		This function evaluates a numeric expression using the parser's evaluation routine. It retrieves the evaluated integer value, formats it as a string, and converts it into a token that is
	   pushed back onto the parser's input stream. If the original value was negative, a sign token is also pushed back to maintain correct expression semantics. The function handles both positive and
	   negative values appropriately.

		\return The function returns true if the evaluation was successful, false otherwise
	*/
	int					 Directive_eval();

	/*!
		\brief Evaluates a floating-point expression and adds the result as a token to the parser's input stream.

		This function processes a floating-point expression by calling the parser's evaluation routine. It retrieves the evaluated value and formats it as a string with two decimal places. The
	   formatted string is then converted into a token and pushed back onto the parser's input stream. If the original value was negative, a sign token is also pushed back to maintain the correct
	   expression semantics.

		\return Returns true if the evaluation was successful, false otherwise.
	*/
	int					 Directive_evalfloat();

	/*!
		\brief Parses and handles preprocessor directives from the source code

		Reads a preprocessor directive token and dispatches execution to the appropriate directive handler function based on the directive name. Handles conditional compilation directives like if,
	   ifdef, ifndef, elif, else, and endif, as well as control directives like include, define, undef, line, error, warning, pragma, eval, and evalfloat. If the directive is not recognized, it
	   reports an error. If skipping is active, it consumes the rest of the line and returns true.

		\return The return value from the respective directive handler function, typically true for success and false for failure
	*/
	int					 ReadDirective();

	/*!
		\brief Evaluates a dollar directive and returns the parsed integer value

		This function processes a dollar directive by evaluating the expression and converting it to an integer value. It handles the parsing of the directive and manages tokenization of the result.
	   The function correctly handles negative values by storing the sign separately and reading it back later when needed. It returns true on successful evaluation, false otherwise.

		\return The parsed integer value from the dollar directive evaluation
	*/
	int					 DollarDirective_evalint();

	/*!
		\brief Evaluates a float value from a dollar directive and prepares it for tokenization

		This function processes a dollar directive by evaluating a floating-point value from the current script context. It handles the evaluation, formats the result into a string with two decimal
	   places, and sets up a token with the appropriate type and value information. If the evaluated value is negative, it also handles the sign token appropriately. The function is used in the
	   parsing of special directives that involve floating-point numbers.

		\return Returns 1 if the evaluation and token preparation was successful, 0 otherwise
		\throws This function does not explicitly throw exceptions but may fail if the evaluation process encounters issues
	*/
	int					 DollarDirective_evalfloat();

	/*!
		\brief Parses and executes a $ directive from the source code.

		This function reads a $ directive from the source code, validates that it is on a single line, and then processes it based on its name. It supports directives such as evalint and evalfloat. If
	   the directive is not recognized or if there are syntax errors, it returns false and issues an error message.

		\return The return value indicates the result of processing the directive, typically true for success or false for failure.
	*/
	int					 ReadDollarDirective();
};

ID_INLINE const char* idParser::GetFileName() const
{
	if( idParser::scriptstack ) {
		return idParser::scriptstack->GetFileName();
	} else {
		return "";
	}
}

ID_INLINE const int idParser::GetFileOffset() const
{
	if( idParser::scriptstack ) {
		return idParser::scriptstack->GetFileOffset();
	} else {
		return 0;
	}
}

ID_INLINE const ID_TIME_T idParser::GetFileTime() const
{
	if( idParser::scriptstack ) {
		return idParser::scriptstack->GetFileTime();
	} else {
		return 0;
	}
}

ID_INLINE const int idParser::GetLineNum() const
{
	if( idParser::scriptstack ) {
		return idParser::scriptstack->GetLineNum();
	} else {
		return 0;
	}
}

#endif /* !__PARSER_H__ */
