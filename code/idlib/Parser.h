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

// macro definitions
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

// indents used for conditional compilation directives:
// #if, #else, #elif, #ifdef, #ifndef
typedef struct indent_s {
	int				 type;	 // indent type
	int				 skip;	 // true if skipping current indent
	idLexer*		 script; // script the indent was in
	struct indent_s* next;	 // next indent on the indent stack
} indent_t;

/*!
	\class idParser
	\brief A parser class for processing structured text data with support for preprocessor directives and token-based parsing.

	The idParser class provides a comprehensive framework for parsing structured text inputs, such as configuration files or script languages. It supports loading data from files or memory buffers,
   tokenizing input streams, and handling preprocessor directives like #define, #include, and conditional compilation. The parser maintains internal state for tracking source files, line numbers, and
   indentation levels during parsing. It includes functionality for reading and validating specific token types, parsing numerical data, and managing define macros. The class also supports error and
   warning reporting, and offers methods for navigating through source text, including skipping sections and setting markers. Memory management is handled internally, with the parser freeing its
   source data when destroyed or when explicitly requested.

*/
class idParser
{
public:
	//! Initializes a new instance of the idParser class.
	idParser();

	//! Initializes an idParser instance with the specified flags.
	idParser( int flags );

	//! Constructs an idParser object and initializes its internal state, loading the specified file.
	idParser( const char* filename, int flags = 0, bool OSPath = false );

	//! Constructs an idParser object with the specified memory buffer, length, and name.
	idParser( const char* ptr, int length, const char* name, int flags = 0 );

	//! Destructor for the idParser class that frees the source.
	~idParser();

	//! Loads a source file for parsing
	int				LoadFile( const char* filename, bool OSPath = false );

	//! Loads a source from memory with the specified length and name.
	int				LoadMemory( const char* ptr, int length, const char* name );

	//! Frees the current source data, optionally preserving define definitions.
	void			FreeSource( bool keepDefines = false );

	//! Parses a 1D matrix of floats from the input stream.
	int				Parse1DMatrixLegacy( int x, float* m );

	//! Returns true if a source is loaded
	int				IsLoaded() const { return idParser::loaded; }

	//! Reads the next token from the source and returns true if successful
	int				ReadToken( idToken* token );

	//! Expects and reads a specific token string from the parser input
	int				ExpectTokenString( const char* string );

	//! Expects and validates a token of a specific type and subtype from the parser.
	int				ExpectTokenType( int type, int subtype, idToken* token );

	//! Expects and reads any token from the parser input
	int				ExpectAnyToken( idToken* token );

	//! Checks if the next token in the parser matches the given string and removes it from the source if it does.
	int				CheckTokenString( const char* string );

	//! Checks if the next token matches the specified type and subtype, and if so, removes and returns the token.
	int				CheckTokenType( int type, int subtype, idToken* token );

	//! Checks if the next token in the parser matches the given string without consuming it.
	int				PeekTokenString( const char* string );

	//! Checks if the next token matches the specified type and subtype without removing it from the source.
	int				PeekTokenType( int type, int subtype, idToken* token );

	//! Skips tokens in the parser until the specified string is found.
	int				SkipUntilString( const char* string );

	//! Skips the rest of the current line in the parser.
	int				SkipRestOfLine();

	//! Skips a braced section in the parser input.
	int				SkipBracedSection( bool parseFirstBrace = true );

	//! Parses a braced section from the input stream into the provided string.
	const char*		ParseBracedSection( idStr& out, int tabs, bool parseFirstBrace, char intro, char outro );

	//! Parses a braced section from the script into the provided string while maintaining indents and newlines.
	const char*		ParseBracedSectionExact( idStr& out, int tabs = -1 );

	//! Parses the rest of the current line into the provided string output.
	const char*		ParseRestOfLine( idStr& out );

	//! Unreads the given token by returning it to the parser's input stream.
	void			UnreadToken( idToken* token );

	//! Reads a token from the current line only if it does not cross line boundaries.
	int				ReadTokenOnLine( idToken* token );

	//! Parses and returns a signed integer value from the token stream
	int				ParseInt();

	//! Parses and returns a boolean value from the token stream
	bool			ParseBool();

	//! Parses and returns a floating point number from the token stream
	float			ParseFloat();

	//! Parses a 1D matrix of floats from the input stream.
	int				Parse1DMatrix( int x, float* m );

	//! Parses a 2D matrix of specified dimensions from the input stream.
	int				Parse2DMatrix( int y, int x, float* m );

	//! Parses a 3D matrix from the input stream, filling the provided float array with the parsed values.
	int				Parse3DMatrix( int z, int y, int x, float* m );

	//! Returns the length of the whitespace preceding the last read token.
	int				GetLastWhiteSpace( idStr& whiteSpace ) const;

	//! Sets a marker in the source file at the current position.
	void			SetMarker();

	//! Retrieves a string from a marker to the current position in the script.
	void			GetStringFromMarker( idStr& out, bool clean = false );

	//! Adds a define to the parser's source.
	int				AddDefine( const char* string );

	//! Adds built-in preprocessor defines to the parser.
	void			AddBuiltinDefines();

	//! Sets the source include path for the parser.
	void			SetIncludePath( const char* path );

	//! Sets the punctuation set for the parser.
	void			SetPunctuations( const punctuation_t* p );

	//! Returns a pointer to the punctuation string with the given id.
	const char*		GetPunctuationFromId( int id );

	//! Returns the id for the given punctuation string.
	int				GetPunctuationId( const char* p );

	//! Sets the lexer flags for the parser and all scripts in the script stack.
	void			SetFlags( int flags );

	//! Returns the lexer flags of the parser instance.
	int				GetFlags() const;

	//! Returns the name of the current script file being parsed.
	const char*		GetFileName() const;

	//! Returns the current offset in the current script.
	const int		GetFileOffset() const;

	//! Retrieves the file time for the currently parsed script.
	const ID_TIME_T GetFileTime() const;

	//! Returns the current line number from the parser's script stack or zero if no script is active
	const int		GetLineNum() const;

	//! Prints an error message using a format string and variable arguments
	void			Error( VERIFY_FORMAT_STRING const char* str, ... ) const;

	//! Prints a warning message using a format string and variable arguments.
	void			Warning( VERIFY_FORMAT_STRING const char* str, ... ) const;

	//! Returns true if the parser has reached the end of the file
	bool			EndOfFile();

	//! Adds a global define that will be applied to all opened sources.
	static int		AddGlobalDefine( const char* string );

	//! Removes a global define with the specified name.
	static int		RemoveGlobalDefine( const char* name );

	//! Removes all global defines from the parser.
	static void		RemoveAllGlobalDefines();

	//! Sets the base folder path from which files will be loaded.
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

	//! Pushes a new indentation level onto the parser's indentation stack.
	void				 PushIndent( int type, int skip );

	//! Pops the top indentation level from the parser's indentation stack and returns its type and skip values.
	void				 PopIndent( int* type, int* skip );

	//! Pushes a script onto the parser's script stack after checking for recursive inclusion.
	void				 PushScript( idLexer* script );

	//! Reads the next token from the source script and returns true if successful.
	int					 ReadSourceToken( idToken* token );

	//! Reads a token from the current line, continuing to the next line only if a backslash is found.
	int					 ReadLine( idToken* token );

	//! Adds a token back to the front of the parser's token stream
	int					 UnreadSourceToken( idToken* token );

	//! Reads and processes parameters for a preprocessor define directive.
	int					 ReadDefineParms( define_t* define, idToken** parms, int maxparms );

	//! Combines an array of tokens into a single string token.
	int					 StringizeTokens( idToken* tokens, idToken* token );

	//! Merges tokens of compatible types into the first token.
	int					 MergeTokens( idToken* t1, idToken* t2 );

	//! Expands builtin preprocessor defines such as line number, file name, date, and time into token sequences
	int					 ExpandBuiltinDefine( idToken* deftoken, define_t* define, idToken** firsttoken, idToken** lasttoken );

	//! Expands a define token into a list of tokens, handling parameters, stringizing, and merging operators
	int					 ExpandDefine( idToken* deftoken, define_t* define, idToken** firsttoken, idToken** lasttoken );

	//! Expands a define into the parser source and updates the token list
	int					 ExpandDefineIntoSource( idToken* deftoken, define_t* define );

	//! Adds global defines to the source parser
	void				 AddGlobalDefinesToSource();

	//! Creates a copy of the given define_t structure with all its tokens and parameters duplicated
	define_t*			 CopyDefine( define_t* define );

	//! Finds a define in the hash table by its name.
	define_t*			 FindHashedDefine( define_t** definehash, const char* name );

	//! Finds the index of a parameter with the specified name in a define structure.
	int					 FindDefineParm( define_t* define, const char* name );

	//! Adds a define to the hash table for fast lookup.
	void				 AddDefineToHash( define_t* define, define_t** definehash );

	//! Prints the details of a given define structure to the common output.
	static void			 PrintDefine( define_t* define );

	//! Frees all memory associated with a define_t structure
	static void			 FreeDefine( define_t* define );

	//! Finds and returns a define with the specified name in the given define list, or NULL if not found.
	static define_t*	 FindDefine( define_t* defines, const char* name );

	//! Creates a define_t object from a string representation.
	static define_t*	 DefineFromString( const char* string );

	//! Returns a copy of the first define in the parser's hash table.
	define_t*			 CopyFirstDefine();

	//! Processes a #include directive to read and parse another source file.
	virtual int			 Directive_include( idToken* token, bool supressWarning = false );

	//! Removes a previously defined macro or symbol from the parser's definition table.
	int					 Directive_undef();

	//! Processes a #ifdef directive by checking if a define is present and managing conditional compilation indentation.
	int					 Directive_if_def( int type );

	//! Processes an ifdef directive and returns the result of the conditional compilation check.
	int					 Directive_ifdef();

	//! Parses an #ifndef directive and returns the result of the conditional compilation check.
	int					 Directive_ifndef();

	//! Handles the #else directive in the parser.
	int					 Directive_else();

	//! Parses and processes the #endif directive in the parser.
	int					 Directive_endif();

	//! Evaluates tokens for conditional compilation expressions and returns the result
	int					 EvaluateTokens( idToken* tokens, signed int* intvalue, double* floatvalue, int integer );

	//! Evaluates a mathematical expression from parsed tokens and stores the result in either an integer or floating-point variable
	int					 Evaluate( signed int* intvalue, double* floatvalue, int integer );

	//! Evaluates a mathematical expression and returns the result as either an integer or floating-point value
	int					 DollarEvaluate( signed int* intvalue, double* floatvalue, int integer );

	//! Processes a #define directive to create or update a macro definition.
	int					 Directive_define();

	//! Parses and processes the #elif preprocessor directive.
	int					 Directive_elif();

	//! Evaluates a conditional expression and sets up indentation for conditional parsing.
	int					 Directive_if();

	//! Parses and handles the #line directive, which is not supported in this implementation.
	int					 Directive_line();

	//! Processes a #error directive and reports an error message.
	int					 Directive_error();

	//! Parses a #warning directive and issues a warning message.
	int					 Directive_warning();

	//! Parses a #pragma directive and issues a warning that it is not supported.
	int					 Directive_pragma();

	//! Undoes reading of a minus sign token by pushing it back onto the token stream.
	void				 UnreadSignToken();

	//! Evaluates a parser directive and returns the result as an integer.
	int					 Directive_eval();

	//! Evaluates a float value from the parser input and prepares it for further processing.
	int					 Directive_evalfloat();

	//! Reads a preprocessor directive from the source and processes it accordingly.
	int					 ReadDirective();

	//! Evaluates an integer expression and prepares it for parsing.
	int					 DollarDirective_evalint();

	//! Evaluates a float value from a dollar directive and prepares it for tokenization.
	int					 DollarDirective_evalfloat();

	//! Parses and processes a dollar directive from the source token stream.
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
