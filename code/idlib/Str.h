/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

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

#ifndef __STR_H__
#define __STR_H__

/*
===============================================================================

	Character string

===============================================================================
*/

#define ASSERT_ENUM_STRING( string, index ) ( 1 / ( int )!( string - index ) ) ? #string : ""

enum utf8Encoding_t {
	UTF8_PURE_ASCII,	 // no characters with values > 127
	UTF8_ENCODED_BOM,	 // characters > 128 encoded with UTF8, but no byte-order-marker at the beginning
	UTF8_ENCODED_NO_BOM, // characters > 128 encoded with UTF8, with a byte-order-marker at the beginning
	UTF8_INVALID,		 // has values > 127 but isn't valid UTF8
	UTF8_INVALID_BOM	 // has a byte-order-marker at the beginning, but isn't valuid UTF8 -- it's messed up
};

// these library functions should not be used for cross platform compatibility
#define strcmp	idStr::Cmp // use_idStr_Cmp
#define strncmp use_idStr_Cmpn

#if defined( StrCmpN )
	#undef StrCmpN
#endif
#define StrCmpN use_idStr_Cmpn

#if defined( strcmpi )
	#undef strcmpi
#endif
#define strcmpi use_idStr_Icmp

#if defined( StrCmpI )
	#undef StrCmpI
#endif
#define StrCmpI use_idStr_Icmp

#if defined( StrCmpNI )
	#undef StrCmpNI
#endif
#define StrCmpNI use_idStr_Icmpn

#define stricmp	 idStr::Icmp // use_idStr_Icmp

#undef strcasecmp // DG: redefining this without undefining it causes tons of compiler warnings

#define _stricmp   use_idStr_Icmp
#define strcasecmp use_idStr_Icmp
#define strnicmp   use_idStr_Icmpn
#define _strnicmp  use_idStr_Icmpn
#define _memicmp   use_idStr_Icmpn

class idVec4;

#ifndef FILE_HASH_SIZE
	#define FILE_HASH_SIZE 1024
#endif

// color escape character
const int C_COLOR_ESCAPE  = '^';
const int C_COLOR_DEFAULT = '0';
const int C_COLOR_RED	  = '1';
const int C_COLOR_GREEN	  = '2';
const int C_COLOR_YELLOW  = '3';
const int C_COLOR_BLUE	  = '4';
const int C_COLOR_CYAN	  = '5';
const int C_COLOR_ORANGE  = '6';
const int C_COLOR_WHITE	  = '7';
const int C_COLOR_GRAY	  = '8';
const int C_COLOR_BLACK	  = '9';

// color escape string
#define S_COLOR_DEFAULT "^0"
#define S_COLOR_RED		"^1"
#define S_COLOR_GREEN	"^2"
#define S_COLOR_YELLOW	"^3"
#define S_COLOR_BLUE	"^4"
#define S_COLOR_CYAN	"^5"
#define S_COLOR_ORANGE	"^6"
#define S_COLOR_WHITE	"^7"
#define S_COLOR_GRAY	"^8"
#define S_COLOR_BLACK	"^9"

// make idStr a multiple of 16 bytes long
// don't make too large to keep memory requirements to a minimum
const int STR_ALLOC_BASE = 20;
const int STR_ALLOC_GRAN = 32;

typedef enum { MEASURE_SIZE = 0, MEASURE_BANDWIDTH } Measure_t;

/*!
	\class idStr
	\brief A robust string class designed for efficient memory management and comprehensive text manipulation capabilities.

	This class provides a comprehensive set of methods for string construction, manipulation, and comparison. It supports various constructors for different input types including character arrays,
   integers, floats, and booleans, making it versatile for different use cases. The class includes methods for appending, inserting, and replacing content, as well as utilities for case conversion,
   color code handling, and path manipulation. Memory management is handled internally with features for ensuring allocation, setting static buffers, and tracking allocated sizes. The class also
   offers advanced string operations such as UTF-8 support, formatting, filtering, and splitting. It provides comparison functions for both case-sensitive and case-insensitive operations, including
   specialized functions for path and color-aware comparisons. The implementation includes utility methods for validating UTF-8 encoding, converting between different string formats, and handling
   common string operations with efficient in-place modifications when possible.

*/
class idStr
{
public:
	//! Constructs an empty idStr object.
	idStr();

	//! Move constructor for idStr that initializes the object by moving resources from another idStr instance.
	idStr( idStr&& text ) noexcept;

	//! Constructs a new idStr object as a copy of the provided idStr object.
	idStr( const idStr& text );

	//! Constructs a new string by copying a substring from the given string.
	idStr( const idStr& text, int start, int end );

	//! Constructs a new string object by copying the contents of the provided C-string.
	idStr( const char* text );

	//! Constructs a new string by extracting a substring from the given text between the specified start and end indices.
	idStr( const char* text, int start, int end );

	//! Constructs a string representation of a boolean value
	explicit idStr( const bool b );

	//! Constructs a string from a single character
	explicit idStr( const char c );

	//! Constructs a string representation of the given integer.
	explicit idStr( const int i );

	//! Constructs a string representation of the given unsigned integer
	explicit idStr( const unsigned u );

	//! Constructs a string representation of the given float value.
	explicit idStr( const float f );

	//! Destructor for the idStr class that frees allocated memory.
	~idStr();

	//! Returns the total memory size occupied by this string instance.
	size_t		Size() const;

	//! Returns a pointer to the null-terminated string data.
	const char* c_str() const;

	//! Converts the string to a const char pointer.
	operator const char*() const;

	//! Converts the string to a const char pointer.
	operator const char*();

	//! Returns the character at the specified index in the string.
	char					operator[]( int index ) const;

	//! Provides access to a character at the specified index in the string.
	char&					operator[]( int index );

	//! Moves the contents of another idStr object to this object
	void					operator=( idStr&& text ) noexcept;

	//! Assigns the contents of another idStr object to this idStr object.
	void					operator=( const idStr& text );

	//! Assigns a C-string to this string object
	void					operator=( const char* text );

	friend idStr			operator+( const idStr& a, const idStr& b );
	friend idStr			operator+( const idStr& a, const char* b );
	friend idStr			operator+( const char* a, const idStr& b );

	friend idStr			operator+( const idStr& a, const float b );
	friend idStr			operator+( const idStr& a, const int b );
	friend idStr			operator+( const idStr& a, const unsigned b );
	friend idStr			operator+( const idStr& a, const bool b );
	friend idStr			operator+( const idStr& a, const char b );

	//! Appends the contents of another string to this string and returns a reference to this string.
	idStr&					operator+=( const idStr& a );

	//! Appends the given C-string to this string and returns a reference to this string.
	idStr&					operator+=( const char* a );

	//! Appends the string representation of a float value to this string
	idStr&					operator+=( const float a );

	//! Appends a character to the string and returns a reference to itself.
	idStr&					operator+=( const char a );

	//! Appends the string representation of an integer value to this string
	idStr&					operator+=( const int a );

	//! Appends the string representation of an unsigned integer to this string and returns a reference to this string.
	idStr&					operator+=( const unsigned a );

	//! Appends the string representation of a boolean value to the string.
	idStr&					operator+=( const bool a );

	// case sensitive compare
	friend bool				operator==( const idStr& a, const idStr& b );
	friend bool				operator==( const idStr& a, const char* b );
	friend bool				operator==( const char* a, const idStr& b );

	// case sensitive compare
	friend bool				operator!=( const idStr& a, const idStr& b );
	friend bool				operator!=( const idStr& a, const char* b );
	friend bool				operator!=( const char* a, const idStr& b );

	//! Compares this string with the provided text for equality.
	bool					Equals( const char* text ) const;

	//! Compares this string with the provided text and returns an integer indicating their lexicographical relationship.
	int						Cmp( const char* text ) const;

	//! Compares the first n characters of this string with the given text for equality.
	int						Cmpn( const char* text, int n ) const;

	//! Compares the beginning of the string with the given text up to the length of the text.
	int						CmpPrefix( const char* text ) const;

	//! Compares the string with the given text in a case-insensitive manner.
	bool					EqualsIgnoreCase( const char* text ) const;

	//! Performs a case-sensitive comparison of this string with the provided text.
	int						Icmp( const char* text ) const;

	//! Performs a case-insensitive comparison of the first n characters of this string with the provided text.
	int						Icmpn( const char* text, int n ) const;

	//! Compares the beginning of the string with the given text, ignoring case.
	int						IcmpPrefix( const char* text ) const;

	//! Checks if the string starts with the specified prefix, ignoring case.
	bool					StartsWithIgnoreCase( const char* prefix ) const;

	//! Checks if the string ends with the specified suffix, ignoring case differences.
	bool					EndsWithIgnoreCase( const char* suffix ) const;

	//! Performs a case insensitive comparison of the string with the provided text, ignoring color codes.
	int						IcmpNoColor( const char* text ) const;

	//! Compares this string with the provided text as paths, ensuring folders come first.
	int						IcmpPath( const char* text ) const;

	//! Performs a case-insensitive comparison of the first n characters of this string with the provided text.
	int						IcmpnPath( const char* text, int n ) const;

	//! Compares the string with the given text up to the length of the text, ignoring case differences, for path comparisons.
	int						IcmpPrefixPath( const char* text ) const;

	//! Returns the length of the string.
	int						Length() const;

	//! Returns the amount of memory allocated for the string data.
	int						Allocated() const;

	//! Clears the string content and ensures it is empty.
	void					Empty();

	//! Checks if the string is empty.
	bool					IsEmpty() const;

	//! Clears the string content
	void					Clear();

	//! Appends a single character to the string.
	void					Append( const char a );

	//! Appends the contents of another idStr to this string
	void					Append( const idStr& text );

	//! Appends the specified text to the string.
	void					Append( const char* text );

	//! Appends a specified number of characters from a text string to this string.
	void					Append( const char* text, int len );

	//! Inserts a character at the specified index in the string.
	void					Insert( const char a, int index );

	//! Inserts the specified text into the string at the given index.
	void					Insert( const char* text, int index );

	//! Converts all characters in the string to lowercase.
	void					ToLower();

	//! Converts all characters in the string to uppercase.
	void					ToUpper();

	//! Checks if the string contains only numeric characters.
	bool					IsNumeric() const;

	//! Checks if the string contains a color code.
	bool					IsColor() const;

	//! Returns true if the string contains at least one lowercase character.
	bool					HasLower() const;

	//! Returns true if the string contains at least one uppercase character.
	bool					HasUpper() const;

	//! Returns the length of the string excluding color formatting codes.
	int						LengthWithoutColors() const;

	//! Removes color codes from the string and returns a reference to itself.
	idStr&					RemoveColors();

	//! Limits the string length to the specified maximum length.
	void					CapLength( int );

	//! Fills the string buffer with a repeated character up to a given length, resizing the buffer if necessary.
	void					Fill( const char ch, int newlen );

	//! Returns the length of the UTF-8 encoded string.
	ID_INLINE int			UTF8Length();

	//! Returns the UTF-8 character at the specified index in the string and advances the index.
	ID_INLINE uint32		UTF8Char( int& idx );

	//! Returns the number of UTF-8 characters in a null-terminated byte string.
	static int				UTF8Length( const byte* s );

	//! Extracts a UTF-8 character from a string at the specified index and advances the index.
	static ID_INLINE uint32 UTF8Char( const char* s, int& idx );

	//! Returns the UTF-8 character at the specified index in the byte array and updates the index to point to the next character.
	static uint32			UTF8Char( const byte* s, int& idx );

	//! Appends a UTF-8 encoded character to the string
	void					AppendUTF8Char( uint32 c );

	//! Converts the string to UTF-8 format.
	ID_INLINE void			ConvertToUTF8();

	//! Validates if a byte sequence represents valid UTF-8 encoding and determines the encoding type.
	static bool				IsValidUTF8( const uint8* s, const int maxLen, utf8Encoding_t& encoding );

	//! Checks if a string is valid UTF-8 encoded.
	static ID_INLINE bool	IsValidUTF8( const char* s, const int maxLen, utf8Encoding_t& encoding ) { return IsValidUTF8( ( const uint8* )s, maxLen, encoding ); }

	//! Checks if a byte sequence is valid UTF-8.
	static ID_INLINE bool	IsValidUTF8( const uint8* s, const int maxLen );

	//! Checks if a string is valid UTF-8 encoded.
	static ID_INLINE bool	IsValidUTF8( const char* s, const int maxLen ) { return IsValidUTF8( ( const uint8* )s, maxLen ); }

	//! Finds the first occurrence of a character in the string within the specified range.
	int						Find( const char c, int start = 0, int end = -1 ) const;

	/*!
		\brief Searches for the first occurrence of a substring within the string and returns its starting index or -1 if not found.

		This function searches for the first occurrence of the specified substring within the string. It supports case-sensitive and case-insensitive search modes, and allows specifying a start and
	   end position for the search. The function returns the index of the first character of the found substring, or -1 if the substring is not found. If the end parameter is not specified, the search
	   extends to the end of the string.

		\param text The substring to search for
		\param casesensitive Flag indicating whether the search should be case-sensitive
		\param start The starting index for the search
		\param end The ending index for the search, or -1 to search until the end of the string
		\return The index of the first occurrence of the substring, or -1 if the substring is not found
	*/
	int						Find( const char* text, bool casesensitive = true, int start = 0, int end = -1 ) const;

	//! Checks if the string matches the given filter pattern.
	bool					Filter( const char* filter, bool casesensitive ) const;

	//! Returns the index of the last occurrence of the specified character in the string, or -1 if not found.
	int						Last( const char c ) const;

	//! Returns the leftmost 'len' characters from the string and stores them in the result
	const char*				Left( int len, idStr& result ) const;

	//! Returns the rightmost 'len' characters of the string, storing the result in the provided idStr reference.
	const char*				Right( int len, idStr& result ) const;

	//! Extracts a substring of specified length starting at the given index and stores it in the result parameter
	const char*				Mid( int start, int len, idStr& result ) const;

	//! Returns the leftmost 'len' characters from the string
	idStr					Left( int len ) const;

	//! Returns the rightmost 'len' characters from the string
	idStr					Right( int len ) const;

	//! Returns a substring starting at the specified index for the specified length.
	idStr					Mid( int start, int len ) const;

	//! Formats the string using a printf-style format string and variable arguments.
	void					Format( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Formats an integer with commas as thousands separators and optionally prefixes it with a dollar sign.
	static idStr			FormatInt( const int num, bool isCash = false );

	//! Formats an integer as a cash amount string with thousands separators.
	static idStr			FormatCash( const int num ) { return FormatInt( num, true ); }

	//! Removes all leading occurrences of a specified character from the string.
	void					StripLeading( const char c );

	//! Strips the leading occurrences of the specified string from the current string.
	void					StripLeading( const char* string );

	//! Strips the leading string from the current string if it occurs at the front.
	bool					StripLeadingOnce( const char* string );

	//! Removes all trailing occurrences of a specified character from the string.
	void					StripTrailing( const char c );

	//! Removes trailing occurrences of the specified string from the current string data.
	void					StripTrailing( const char* string );

	//! Removes a trailing string from the end of this string if it exists.
	bool					StripTrailingOnce( const char* string );

	//! Removes a trailing string from the end of this string in a case-insensitive manner, but only if it occurs at the end.
	bool					IStripTrailingOnce( const char* string );

	//! Removes all leading and trailing occurrences of a specified character from the string.
	void					Strip( const char c );

	//! Removes occurrences of the specified string from both the beginning and end of this string.
	void					Strip( const char* string );

	//! Removes trailing whitespace characters from the string.
	void					StripTrailingWhitespace();

	//! Removes surrounding quotes from the string.
	idStr&					StripQuotes();

	//! Replaces all occurrences of a substring with another substring and returns true if any replacements were made.
	bool					Replace( const char* old, const char* nw );

	//! Replaces all occurrences of a character with another character in the string and returns true if any replacements were made.
	bool					ReplaceChar( const char old, const char nw );

	//! Copies a range of characters from the input text into the string, starting at start and ending at end.
	ID_INLINE void			CopyRange( const char* text, int start, int end );

	//! Computes and returns a hash value for the filename, excluding the file extension.
	int						FileNameHash() const;

	//! Converts backslashes to forward slashes in the string.
	idStr&					BackSlashesToSlashes();

	//! Converts forward slashes to backslashes in the string.
	idStr&					SlashesToBackSlashes();

	//! Sets the file extension for the string, ensuring it starts with a period.
	idStr&					SetFileExtension( const char* extension );

	//! Removes the file extension from the string
	idStr&					StripFileExtension();

	//! Removes the file extension from the string by truncating at the first period.
	idStr&					StripAbsoluteFileExtension();

	//! Appends a default file extension to the string if no extension is present.
	idStr&					DefaultFileExtension( const char* extension );

	//! Combines a base path with the current string if the current string does not represent an absolute path.
	idStr&					DefaultPath( const char* basepath );

	//! Appends a partial path to the string, ensuring proper path separator handling.
	void					AppendPath( const char* text );

	//! Removes the filename from a path string
	idStr&					StripFilename();

	//! Removes the path from the filename, keeping only the file name part.
	idStr&					StripPath();

	//! Copies the file path portion of this string to the provided destination string.
	void					ExtractFilePath( idStr& dest ) const;

	//! Extracts the filename from this string and copies it to the destination string.
	void					ExtractFileName( idStr& dest ) const;

	//! Extracts the filename without the extension from this string and copies it to the destination string.
	void					ExtractFileBase( idStr& dest ) const;

	//! Extracts the file extension from the string and copies it to the destination string.
	void					ExtractFileExtension( idStr& dest ) const;

	//! Checks if the string ends with the specified file extension
	bool					CheckExtension( const char* ext );

	//! Removes surrounding double quotes from the string if they exist.
	void					StripDoubleQuotes();

	//! Splits the string into a list using the specified delimiters
	bool					Split( idList<idStr>& list, const char delimiter = ',', const char groupDelimiter = '\'' ) const;

	//! Returns the length of a null-terminated string.
	static int				Length( const char* s );

	//! Converts the input string to lowercase in place and returns a pointer to the modified string.
	static char*			ToLower( char* s );

	//! Converts all lowercase characters in the input string to uppercase in place and returns the modified string.
	static char*			ToUpper( char* s );

	//! Checks if a string contains only numerical values.
	static bool				IsNumeric( const char* s );

	//! Checks if a string represents a color code
	static bool				IsColor( const char* s );

	//! Checks if a string has any lowercase characters.
	static bool				HasLower( const char* s );

	//! Checks if a string has any uppercase characters.
	static bool				HasUpper( const char* s );

	//! Returns the length of a string excluding color escape sequences.
	static int				LengthWithoutColors( const char* s );

	//! Removes color codes from the input string and returns a pointer to the modified string.
	static char*			RemoveColors( char* s );

	//! Compares two null-terminated strings and returns an integer indicating their lexicographical relationship.
	static int				Cmp( const char* s1, const char* s2 );

	//! Compares up to n characters of two null-terminated strings, ignoring case differences.
	static int				Cmpn( const char* s1, const char* s2, int n );

	//! Performs case-insensitive comparison of two null-terminated strings and returns an integer indicating their lexicographical relationship.
	static int				Icmp( const char* s1, const char* s2 );

	//! Performs a case-insensitive comparison of two strings up to n characters.
	static int				Icmpn( const char* s1, const char* s2, int n );

	//! Performs a case-insensitive comparison of two strings while ignoring color codes.
	static int				IcmpNoColor( const char* s1, const char* s2 );

	//! Compares two paths case-insensitively while ensuring folders appear before files
	static int				IcmpPath( const char* s1, const char* s2 );

	//! Compares two paths case-insensitively and ensures folders appear before files
	static int				IcmpnPath( const char* s1, const char* s2, int n );

	//! Appends a source string to a destination buffer ensuring it does not exceed the specified size limit.
	static void				Append( char* dest, int size, const char* src );

	//! Copies a string with a size limit and ensures null termination.
	static void				Copynz( char* dest, const char* src, int destsize );

	/*!
		\brief Formats a string into a destination buffer with size limiting and returns the number of characters written.

		This function performs formatted string output into a destination buffer, ensuring that the output does not exceed the specified size limit. It uses a variable argument list to accept format
	   parameters and internally calls vsnPrintf for the actual formatting. The function handles overflow conditions by issuing a warning and adjusting the length value. It is designed to guarantee
	   null termination of the output string.

		\param dest Destination buffer where the formatted string will be stored
		\param size Size of the destination buffer in bytes
		\param fmt Format string specifying how to format the output
		\param  Variable arguments for the format string
		\return The number of characters written to the destination buffer, or a negative value if an overflow occurs
		\throws Warning is issued when buffer overflow occurs
	*/
	static int				snPrintf( char* dest, int size, VERIFY_FORMAT_STRING const char* fmt, ... );

	/*!
		\brief Formats a string using a va_list and writes it to a destination buffer with platform-specific handling.

		This function provides a cross-platform implementation for formatting strings using a va_list and writing them to a destination buffer. It handles Windows and Unix-like systems differently to
	   ensure compatibility. The function ensures that the destination buffer is null-terminated and returns the number of characters written, or -1 if an error occurs. The function also checks for
	   buffer overflow conditions and handles them appropriately.

		\param dest Destination buffer to write the formatted string
		\param size Size of the destination buffer
		\param fmt Format string specifying how to format the output
		\param argptr Pointer to the list of arguments to be formatted
		\return Number of characters written to the destination buffer, or -1 if an error occurs due to buffer overflow or other issues
	*/
	static int				vsnPrintf( char* dest, int size, const char* fmt, va_list argptr );

	/*!
		\brief Finds the first occurrence of a character in a string within a specified range

		This function searches for the first occurrence of a specified character within a given string, starting from a specified index and ending at another specified index. If the end index is not
	   specified, it defaults to the length of the string minus one. If the character is found, the function returns the index of its first occurrence. If the character is not found within the
	   specified range, the function returns -1. The function handles negative end values by automatically calculating the string length

		\param str The string to search in
		\param c The character to find
		\param start The starting index for the search
		\param end The ending index for the search, defaults to the string length minus one if not specified
		\return The index of the first occurrence of the character in the string, or -1 if not found
	*/
	static int				FindChar( const char* str, const char c, int start = 0, int end = -1 );

	/*!
		\brief Searches for a substring within a string and returns the index of the first occurrence or -1 if not found

		This function searches for a specified text substring within a given string. It supports case-sensitive and case-insensitive search modes. The search can be limited to a specific range within
	   the string using start and end parameters. The function returns the index of the first occurrence of the substring, or -1 if the substring is not found. The function handles edge cases such as
	   when the end parameter is -1, in which case it uses the full length of the string. It also properly calculates the maximum valid search position to avoid buffer overruns when comparing the
	   substring.

		\param str The string to search within
		\param text The substring to search for
		\param casesensitive Flag to indicate if the search should be case sensitive
		\param start The starting index for the search
		\param end The ending index for the search, or -1 to search until the end of the string
		\return The index of the first occurrence of the text substring within the string, or -1 if not found
	*/
	static int				FindText( const char* str, const char* text, bool casesensitive = true, int start = 0, int end = -1 );

	//! Checks if a string matches a filter pattern that may contain wildcards.
	static bool				Filter( const char* filter, const char* name, bool casesensitive );

	//! Strips the media name by converting to lowercase, replacing backslashes with forward slashes, and removing the file extension.
	static void				StripMediaName( const char* name, idStr& mediaName );

	//! Checks if a file name has the specified extension, case-insensitively.
	static bool				CheckExtension( const char* name, const char* ext );

	//! Converts a float array to a formatted string with specified precision
	static const char*		FloatArrayToString( const float* array, const int length, const int precision );

	//! Returns a quoted version of the input string formatted for C-style output
	static const char*		CStyleQuote( const char* str );

	//! Removes C-style quotes and escape sequences from a string
	static const char*		CStyleUnQuote( const char* str );

	//! Computes a hash value for the given string using a simple hashing algorithm.
	static int				Hash( const char* string );

	//! Calculates a hash value for a given string of specified length.
	static int				Hash( const char* string, int length );

	//! Calculates a case insensitive hash value for the given string.
	static int				IHash( const char* string );

	//! Computes a case-insensitive hash value for the given string of specified length.
	static int				IHash( const char* string, int length );

	//! Converts an uppercase ASCII character to lowercase.
	static char				ToLower( char c );

	//! Converts a lowercase character to uppercase.
	static char				ToUpper( char c );

	//! Returns true if the character is a printable ASCII character.
	static bool				CharIsPrintable( int c );

	//! Returns true if the given character is a lowercase letter.
	static bool				CharIsLower( int c );

	//! Checks if a character is an uppercase letter.
	static bool				CharIsUpper( int c );

	//! Checks if a character is an alphabetic character.
	static bool				CharIsAlpha( int c );

	//! Checks if the given character is a numeric digit.
	static bool				CharIsNumeric( int c );

	//! Checks if the given character is a newline character.
	static bool				CharIsNewLine( char c );

	//! Checks if the given character is a tab character.
	static bool				CharIsTab( char c );

	//! Returns the color index from a given color value by performing a bitwise AND operation with 15.
	static int				ColorIndex( int c );

	//! Returns the color vector for the given index from the global color table.
	static idVec4&			ColorForIndex( int i );

	friend int				sprintf( idStr& dest, const char* fmt, ... );
	friend int				vsprintf( idStr& dest, const char* fmt, va_list ap );

	//! Reallocates the string data buffer to a new size while optionally preserving existing data.
	void					ReAllocate( int amount, bool keepold );

	//! Frees the allocated memory for the string data
	void					FreeData();

	//! Formats a value using the best unit for the given measurement.
	int						BestUnit( const char* format, float value, Measure_t measure );

	//! Sets the string to the formatted value in the specified unit and measurement.
	void					SetUnit( const char* format, float value, int unit, Measure_t measure );

	//! Initializes the memory allocator used by the idStr class.
	static void				InitMemory();

	//! Shuts down the memory allocator used by idStr strings.
	static void				ShutdownMemory();

	//! Releases unused memory from the string data allocator.
	static void				PurgeMemory();

	//! Displays the current string memory usage statistics.
	static void				ShowMemoryUsage_f( const idCmdArgs& args );

	//! Returns the dynamically allocated memory used by the string
	int						DynamicMemoryUsed() const;

	//! Formats an integer number into a formatted string representation
	static idStr			FormatNumber( int number );

	/*!
		\brief Splits a source string into a list of substrings using the specified delimiter and group delimiter, returning true if successful.

		This function splits a source string into multiple substrings based on a delimiter character. It also supports a group delimiter which allows for quoted sections to be treated as single
	   elements. The function returns false if there is a mismatched group delimiter, otherwise it returns true. The resulting substrings are appended to the provided list.

		\param source The string to be split
		\param list The list to which the resulting substrings will be appended
		\param delimiter The character used to separate substrings
		\param groupDelimiter The character used to enclose groups of characters that should not be split
		\return True if the split operation completed successfully, false if there was a mismatched group delimiter.
	*/
	static bool				Split( const char* source, idList<idStr>& list, const char delimiter = ',', const char groupDelimiter = '\'' );

protected:
	int			   len;
	char*		   data;
	int			   allocedAndFlag; // top bit is used to store a flag that indicates if the string data is static or not
	char		   baseBuffer[STR_ALLOC_BASE];

	//! Ensures the string buffer has sufficient capacity for the specified amount of data.
	void		   EnsureAlloced( int amount, bool keepold = true );

	//! Sets the string buffer to a specified static buffer and marks it as static.
	ID_INLINE void SetStaticBuffer( char* buffer, const int bufferLength );

private:
	//! Initializes the string to use the base buffer and sets up its initial state.
	ID_INLINE void		Construct();

	static const uint32 STATIC_BIT	 = 31;
	static const uint32 STATIC_MASK	 = 1u << STATIC_BIT;
	static const uint32 ALLOCED_MASK = STATIC_MASK - 1;

	//! Returns the amount of memory allocated for the string.
	ID_INLINE int		GetAlloced() const { return allocedAndFlag & ALLOCED_MASK; }

	//! Sets the allocated size of the string while preserving static allocation flags.
	ID_INLINE void		SetAlloced( const int a ) { allocedAndFlag = ( allocedAndFlag & STATIC_MASK ) | ( a & ALLOCED_MASK ); }

	//! Checks if the string is using static memory allocation.
	ID_INLINE bool		IsStatic() const { return ( allocedAndFlag & STATIC_MASK ) != 0; }

	//! Sets the static flag for the string buffer.
	ID_INLINE void		SetStatic( const bool isStatic ) { allocedAndFlag = ( allocedAndFlag & ALLOCED_MASK ) | ( isStatic << STATIC_BIT ); }

public:
	static const int INVALID_POSITION = -1;
};

char* va( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

/*!
	\class idSort_Str
	\brief A sorting implementation for idStr objects that performs lexicographical comparison.
*/
class idSort_Str : public idSort_Quick<idStr, idSort_Str>
{
public:
	//! Compares two idStr objects and returns an integer indicating their lexicographical relationship.
	int Compare( const idStr& a, const idStr& b ) const { return a.Icmp( b ); }
};

/*!
	\class idSort_PathStr
	\brief A sorting comparator for path strings that handles file system paths appropriately.
*/
class idSort_PathStr : public idSort_Quick<idStr, idSort_PathStr>
{
public:
	//! Compares two path strings for sorting purposes
	int Compare( const idStr& a, const idStr& b ) const { return a.IcmpPath( b ); }
};

ID_INLINE void idStr::Construct()
{
	SetStatic( false );
	SetAlloced( STR_ALLOC_BASE );
	data	= baseBuffer;
	len		= 0;
	data[0] = '\0';
#ifdef ID_DEBUG_UNINITIALIZED_MEMORY
	memset( baseBuffer, 0, sizeof( baseBuffer ) );
#endif
}

ID_INLINE void idStr::EnsureAlloced( int amount, bool keepold )
{
	// static string's can't reallocate
	if( IsStatic() ) {
		release_assert( amount <= GetAlloced() );
		return;
	}
	if( amount > GetAlloced() ) { ReAllocate( amount, keepold ); }
}

ID_INLINE void idStr::SetStaticBuffer( char* buffer, const int bufferLength )
{
	// this should only be called on a freshly constructed idStr
	assert( data == baseBuffer );
	data = buffer;
	len	 = 0;
	SetAlloced( bufferLength );
	SetStatic( true );
}

ID_INLINE idStr::idStr()
{
	Construct();
}

ID_INLINE idStr::idStr( idStr&& text ) noexcept
{
	Construct();
	*this = std::move( text );
}

ID_INLINE idStr::idStr( const idStr& text )
{
	Construct();
	int l;

	l = text.Length();
	EnsureAlloced( l + 1 );
	strcpy( data, text.data );
	len = l;
}

ID_INLINE idStr::idStr( const idStr& text, int start, int end )
{
	Construct();
	int i;
	int l;

	if( end > text.Length() ) { end = text.Length(); }
	if( start > text.Length() ) {
		start = text.Length();
	} else if( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if( l < 0 ) { l = 0; }

	EnsureAlloced( l + 1 );

	for( i = 0; i < l; i++ ) {
		data[i] = text[start + i];
	}

	data[l] = '\0';
	len		= l;
}

ID_INLINE idStr::idStr( const char* text )
{
	Construct();
	int l;

	if( text ) {
		// RB: 64 bit fixes,  conversion from 'size_t' to 'int', possible loss of data
		l = ( int )strlen( text );
		// RB end
		EnsureAlloced( l + 1 );
		strcpy( data, text );
		len = l;
	}
}

ID_INLINE idStr::idStr( const char* text, int start, int end )
{
	Construct();
	int i;
	// RB: 64 bit fixes,  conversion from 'size_t' to 'int', possible loss of data
	int l = ( int )strlen( text );
	// RB end

	if( end > l ) { end = l; }
	if( start > l ) {
		start = l;
	} else if( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if( l < 0 ) { l = 0; }

	EnsureAlloced( l + 1 );

	for( i = 0; i < l; i++ ) {
		data[i] = text[start + i];
	}

	data[l] = '\0';
	len		= l;
}

ID_INLINE idStr::idStr( const bool b )
{
	Construct();
	EnsureAlloced( 2 );
	data[0] = b ? '1' : '0';
	data[1] = '\0';
	len		= 1;
}

ID_INLINE idStr::idStr( const char c )
{
	Construct();
	EnsureAlloced( 2 );
	data[0] = c;
	data[1] = '\0';
	len		= 1;
}

ID_INLINE idStr::idStr( const int i )
{
	Construct();
	char text[64];
	int	 l;

	l = idStr::snPrintf( text, sizeof( text ), "%d", i );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE idStr::idStr( const unsigned u )
{
	Construct();
	char text[64];
	int	 l;

	l = idStr::snPrintf( text, sizeof( text ), "%u", u );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE idStr::idStr( const float f )
{
	Construct();
	char text[64];
	int	 l;

	l = idStr::snPrintf( text, sizeof( text ), "%f", f );
	while( l > 0 && text[l - 1] == '0' ) {
		text[--l] = '\0';
	}
	while( l > 0 && text[l - 1] == '.' ) {
		text[--l] = '\0';
	}
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE idStr::~idStr()
{
	FreeData();
}

ID_INLINE size_t idStr::Size() const
{
	return sizeof( *this ) + Allocated();
}

ID_INLINE const char* idStr::c_str() const
{
	return data;
}

ID_INLINE idStr::operator const char*()
{
	return c_str();
}

ID_INLINE idStr::operator const char*() const
{
	return c_str();
}

ID_INLINE char idStr::operator[]( int index ) const
{
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[index];
}

ID_INLINE char& idStr::operator[]( int index )
{
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[index];
}

ID_INLINE void idStr::operator=( idStr&& text ) noexcept
{
	Clear();

	len			   = text.len;
	allocedAndFlag = text.allocedAndFlag;
	memcpy( baseBuffer, text.baseBuffer, sizeof( baseBuffer ) );

	if( text.data == text.baseBuffer ) {
		data = baseBuffer;
	} else {
		data = text.data;
	}

	text.len			= 0;
	text.allocedAndFlag = 0;
	text.data			= nullptr;
}

ID_INLINE void idStr::operator=( const idStr& text )
{
	int l;

	l = text.Length();
	EnsureAlloced( l + 1, false );
	memcpy( data, text.data, l );
	data[l] = '\0';
	len		= l;
}

//! Concatenates two idStr objects and returns the resulting string.
ID_INLINE idStr operator+( const idStr& a, const idStr& b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

//! Concatenates a string object with a C-string and returns the result as a new string object.
ID_INLINE idStr operator+( const idStr& a, const char* b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

//! Concatenates a C-string and an idStr object into a new idStr.
ID_INLINE idStr operator+( const char* a, const idStr& b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

//! Concatenates a string with the string representation of a boolean value.
ID_INLINE idStr operator+( const idStr& a, const bool b )
{
	idStr result( a );
	result.Append( b ? "true" : "false" );
	return result;
}

//! Creates a new string by appending a character to the end of another string.
ID_INLINE idStr operator+( const idStr& a, const char b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

//! Concatenates a string with the string representation of a float value.
ID_INLINE idStr operator+( const idStr& a, const float b )
{
	char  text[64];
	idStr result( a );

	idStr::snPrintf( text, sizeof( text ), "%f", b );
	result.Append( text );

	return result;
}

//! Appends an integer value to a string and returns the resulting string.
ID_INLINE idStr operator+( const idStr& a, const int b )
{
	char  text[64];
	idStr result( a );

	idStr::snPrintf( text, sizeof( text ), "%d", b );
	result.Append( text );

	return result;
}

//! Concatenates a string with the string representation of an unsigned integer.
ID_INLINE idStr operator+( const idStr& a, const unsigned b )
{
	char  text[64];
	idStr result( a );

	idStr::snPrintf( text, sizeof( text ), "%u", b );
	result.Append( text );

	return result;
}

ID_INLINE idStr& idStr::operator+=( const float a )
{
	char text[64];

	idStr::snPrintf( text, sizeof( text ), "%f", a );
	Append( text );

	return *this;
}

ID_INLINE idStr& idStr::operator+=( const int a )
{
	char text[64];

	idStr::snPrintf( text, sizeof( text ), "%d", a );
	Append( text );

	return *this;
}

ID_INLINE idStr& idStr::operator+=( const unsigned a )
{
	char text[64];

	idStr::snPrintf( text, sizeof( text ), "%u", a );
	Append( text );

	return *this;
}

ID_INLINE idStr& idStr::operator+=( const idStr& a )
{
	Append( a );
	return *this;
}

ID_INLINE idStr& idStr::operator+=( const char* a )
{
	Append( a );
	return *this;
}

ID_INLINE idStr& idStr::operator+=( const char a )
{
	Append( a );
	return *this;
}

ID_INLINE idStr& idStr::operator+=( const bool a )
{
	Append( a ? "true" : "false" );
	return *this;
}

//! Compares two idStr objects for equality.
ID_INLINE bool operator==( const idStr& a, const idStr& b )
{
	return ( !idStr::Cmp( a.data, b.data ) );
}

//! Compares an idStr object with a C-style string for equality.
ID_INLINE bool operator==( const idStr& a, const char* b )
{
	assert( b );
	return ( !idStr::Cmp( a.data, b ) );
}

//! Compares a C-string with an idStr object for equality.
ID_INLINE bool operator==( const char* a, const idStr& b )
{
	assert( a );
	return ( !idStr::Cmp( a, b.data ) );
}

//! Compares two idStr objects for inequality.
ID_INLINE bool operator!=( const idStr& a, const idStr& b )
{
	return !( a == b );
}

//! Returns true if two string objects are not equal.
ID_INLINE bool operator!=( const idStr& a, const char* b )
{
	return !( a == b );
}

//! Compares a C-string with an idStr object for inequality.
ID_INLINE bool operator!=( const char* a, const idStr& b )
{
	return !( a == b );
}

ID_INLINE bool idStr::Equals( const char* text ) const
{
	assert( text );
	return idStr::Cmp( data, text ) == 0;
}

ID_INLINE bool idStr::EqualsIgnoreCase( const char* text ) const
{
	assert( text );
	return idStr::Icmp( data, text ) == 0;
}

ID_INLINE int idStr::Cmp( const char* text ) const
{
	assert( text );
	return idStr::Cmp( data, text );
}

ID_INLINE int idStr::Cmpn( const char* text, int n ) const
{
	assert( text );
	return idStr::Cmpn( data, text, n );
}

ID_INLINE int idStr::CmpPrefix( const char* text ) const
{
	assert( text );
	// RB: 64 bit fixes,  conversion from 'size_t' to 'int', possible loss of data
	return idStr::Cmpn( data, text, ( int )strlen( text ) );
	// RB end
}

ID_INLINE int idStr::Icmp( const char* text ) const
{
	assert( text );
	return idStr::Icmp( data, text );
}

ID_INLINE int idStr::Icmpn( const char* text, int n ) const
{
	assert( text );
	return idStr::Icmpn( data, text, n );
}

ID_INLINE int idStr::IcmpPrefix( const char* text ) const
{
	assert( text );
	return idStr::Icmpn( data, text, ( int )strlen( text ) );
}

ID_INLINE bool idStr::StartsWithIgnoreCase( const char* prefix ) const
{
	assert( prefix );
	return idStr::Icmpn( data, prefix, ( int )strlen( prefix ) ) == 0;
}

ID_INLINE bool idStr::EndsWithIgnoreCase( const char* suffix ) const
{
	int len = idStr::Length( suffix );
	if( len > Length() ) { return false; }
	return idStr::Icmp( data + Length() - len, suffix ) == 0;
}

ID_INLINE int idStr::IcmpNoColor( const char* text ) const
{
	assert( text );
	return idStr::IcmpNoColor( data, text );
}

ID_INLINE int idStr::IcmpPath( const char* text ) const
{
	assert( text );
	return idStr::IcmpPath( data, text );
}

ID_INLINE int idStr::IcmpnPath( const char* text, int n ) const
{
	assert( text );
	return idStr::IcmpnPath( data, text, n );
}

ID_INLINE int idStr::IcmpPrefixPath( const char* text ) const
{
	assert( text );
	// RB: 64 bit fixes,  conversion from 'size_t' to 'int', possible loss of data
	return idStr::IcmpnPath( data, text, ( int )strlen( text ) );
	// RB end
}

ID_INLINE int idStr::Length() const
{
	return len;
}

ID_INLINE int idStr::Allocated() const
{
	if( data != baseBuffer ) {
		return GetAlloced();
	} else {
		return 0;
	}
}

ID_INLINE void idStr::Empty()
{
	EnsureAlloced( 1 );
	data[0] = '\0';
	len		= 0;
}

ID_INLINE bool idStr::IsEmpty() const
{
	return ( idStr::Cmp( data, "" ) == 0 );
}

ID_INLINE void idStr::Clear()
{
	if( IsStatic() ) {
		len		= 0;
		data[0] = '\0';
		return;
	}
	FreeData();
	Construct();
}

ID_INLINE void idStr::Append( const char a )
{
	EnsureAlloced( len + 2 );
	data[len] = a;
	len++;
	data[len] = '\0';
}

ID_INLINE void idStr::Append( const idStr& text )
{
	int newLen;
	int i;

	newLen = len + text.Length();
	EnsureAlloced( newLen + 1 );
	for( i = 0; i < text.len; i++ ) {
		data[len + i] = text[i];
	}
	len		  = newLen;
	data[len] = '\0';
}

ID_INLINE void idStr::Append( const char* text )
{
	int newLen;
	int i;

	if( text ) {
		// RB: 64 bit fixes,  conversion from 'size_t' to 'int', possible loss of data
		newLen = len + ( int )strlen( text );
		// RB end
		EnsureAlloced( newLen + 1 );
		for( i = 0; text[i]; i++ ) {
			data[len + i] = text[i];
		}
		len		  = newLen;
		data[len] = '\0';
	}
}

ID_INLINE void idStr::Append( const char* text, int l )
{
	int newLen;
	int i;

	if( text && l ) {
		newLen = len + l;
		EnsureAlloced( newLen + 1 );
		for( i = 0; text[i] && i < l; i++ ) {
			data[len + i] = text[i];
		}
		len		  = newLen;
		data[len] = '\0';
	}
}

ID_INLINE void idStr::Insert( const char a, int index )
{
	int i, l;

	if( index < 0 ) {
		index = 0;
	} else if( index > len ) {
		index = len;
	}

	l = 1;
	EnsureAlloced( len + l + 1 );
	for( i = len; i >= index; i-- ) {
		data[i + l] = data[i];
	}
	data[index] = a;
	len++;
}

ID_INLINE void idStr::Insert( const char* text, int index )
{
	int i, l;

	if( index < 0 ) {
		index = 0;
	} else if( index > len ) {
		index = len;
	}

	// RB: 64 bit fixes,  conversion from 'size_t' to 'int', possible loss of data
	l = ( int )strlen( text );
	// RB end
	EnsureAlloced( len + l + 1 );
	for( i = len; i >= index; i-- ) {
		data[i + l] = data[i];
	}
	for( i = 0; i < l; i++ ) {
		data[index + i] = text[i];
	}
	len += l;
}

ID_INLINE void idStr::ToLower()
{
	for( int i = 0; data[i]; i++ ) {
		if( CharIsUpper( data[i] ) ) { data[i] += ( 'a' - 'A' ); }
	}
}

ID_INLINE void idStr::ToUpper()
{
	for( int i = 0; data[i]; i++ ) {
		if( CharIsLower( data[i] ) ) { data[i] -= ( 'a' - 'A' ); }
	}
}

ID_INLINE bool idStr::IsNumeric() const
{
	return idStr::IsNumeric( data );
}

ID_INLINE bool idStr::IsColor() const
{
	return idStr::IsColor( data );
}

ID_INLINE bool idStr::HasLower() const
{
	return idStr::HasLower( data );
}

ID_INLINE bool idStr::HasUpper() const
{
	return idStr::HasUpper( data );
}

ID_INLINE idStr& idStr::RemoveColors()
{
	idStr::RemoveColors( data );
	len = Length( data );
	return *this;
}

ID_INLINE int idStr::LengthWithoutColors() const
{
	return idStr::LengthWithoutColors( data );
}

ID_INLINE void idStr::CapLength( int newlen )
{
	if( len <= newlen ) { return; }
	data[newlen] = 0;
	len			 = newlen;
}

ID_INLINE void idStr::Fill( const char ch, int newlen )
{
	EnsureAlloced( newlen + 1 );
	len = newlen;
	memset( data, ch, len );
	data[len] = 0;
}

ID_INLINE int idStr::UTF8Length()
{
	return UTF8Length( ( byte* )data );
}

ID_INLINE uint32 idStr::UTF8Char( int& idx )
{
	return UTF8Char( ( byte* )data, idx );
}

ID_INLINE void idStr::ConvertToUTF8()
{
	idStr temp( *this );
	Clear();
	for( int index = 0; index < temp.Length(); ++index ) {
		AppendUTF8Char( temp[index] );
	}
}

ID_INLINE uint32 idStr::UTF8Char( const char* s, int& idx )
{
	return UTF8Char( ( byte* )s, idx );
}

ID_INLINE bool idStr::IsValidUTF8( const uint8* s, const int maxLen )
{
	utf8Encoding_t encoding;
	return IsValidUTF8( s, maxLen, encoding );
}

ID_INLINE int idStr::Find( const char c, int start, int end ) const
{
	if( end == -1 ) { end = len; }
	return idStr::FindChar( data, c, start, end );
}

ID_INLINE int idStr::Find( const char* text, bool casesensitive, int start, int end ) const
{
	if( end == -1 ) { end = len; }
	return idStr::FindText( data, text, casesensitive, start, end );
}

ID_INLINE bool idStr::Filter( const char* filter, bool casesensitive ) const
{
	return idStr::Filter( filter, data, casesensitive );
}

ID_INLINE const char* idStr::Left( int len, idStr& result ) const
{
	return Mid( 0, len, result );
}

ID_INLINE const char* idStr::Right( int len, idStr& result ) const
{
	if( len >= Length() ) {
		result = *this;
		return result;
	}
	return Mid( Length() - len, len, result );
}

ID_INLINE idStr idStr::Left( int len ) const
{
	return Mid( 0, len );
}

ID_INLINE idStr idStr::Right( int len ) const
{
	if( len >= Length() ) { return *this; }
	return Mid( Length() - len, len );
}

ID_INLINE void idStr::Strip( const char c )
{
	StripLeading( c );
	StripTrailing( c );
}

ID_INLINE void idStr::Strip( const char* string )
{
	StripLeading( string );
	StripTrailing( string );
}

ID_INLINE bool idStr::CheckExtension( const char* ext )
{
	return idStr::CheckExtension( data, ext );
}

ID_INLINE int idStr::Length( const char* s )
{
	int i;
	for( i = 0; s[i]; i++ ) { }
	return i;
}

ID_INLINE char* idStr::ToLower( char* s )
{
	for( int i = 0; s[i]; i++ ) {
		if( CharIsUpper( s[i] ) ) { s[i] += ( 'a' - 'A' ); }
	}
	return s;
}

ID_INLINE char* idStr::ToUpper( char* s )
{
	for( int i = 0; s[i]; i++ ) {
		if( CharIsLower( s[i] ) ) { s[i] -= ( 'a' - 'A' ); }
	}
	return s;
}

ID_INLINE int idStr::Hash( const char* string )
{
	int i, hash = 0;
	for( i = 0; *string != '\0'; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int idStr::Hash( const char* string, int length )
{
	int i, hash = 0;
	for( i = 0; i < length; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int idStr::IHash( const char* string )
{
	int i, hash = 0;
	for( i = 0; *string != '\0'; i++ ) {
		hash += ToLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int idStr::IHash( const char* string, int length )
{
	int i, hash = 0;
	for( i = 0; i < length; i++ ) {
		hash += ToLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE bool idStr::IsColor( const char* s )
{
	return ( s[0] == C_COLOR_ESCAPE && s[1] != '\0' && s[1] != ' ' );
}

ID_INLINE char idStr::ToLower( char c )
{
	if( c <= 'Z' && c >= 'A' ) { return ( c + ( 'a' - 'A' ) ); }
	return c;
}

ID_INLINE char idStr::ToUpper( char c )
{
	if( c >= 'a' && c <= 'z' ) { return ( c - ( 'a' - 'A' ) ); }
	return c;
}

ID_INLINE bool idStr::CharIsPrintable( int c )
{
	// test for regular ascii and western European high-ascii chars
	return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

ID_INLINE bool idStr::CharIsLower( int c )
{
	// test for regular ascii and western European high-ascii chars
	return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

ID_INLINE bool idStr::CharIsUpper( int c )
{
	// test for regular ascii and western European high-ascii chars
	return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

ID_INLINE bool idStr::CharIsAlpha( int c )
{
	// test for regular ascii and western European high-ascii chars
	return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= 0xC0 && c <= 0xFF ) );
}

ID_INLINE bool idStr::CharIsNumeric( int c )
{
	return ( c <= '9' && c >= '0' );
}

ID_INLINE bool idStr::CharIsNewLine( char c )
{
	return ( c == '\n' || c == '\r' || c == '\v' );
}

ID_INLINE bool idStr::CharIsTab( char c )
{
	return ( c == '\t' );
}

ID_INLINE int idStr::ColorIndex( int c )
{
	return ( c & 15 );
}

ID_INLINE int idStr::DynamicMemoryUsed() const
{
	return ( data == baseBuffer ) ? 0 : GetAlloced();
}

ID_INLINE void idStr::CopyRange( const char* text, int start, int end )
{
	int l = end - start;
	if( l < 0 ) { l = 0; }

	EnsureAlloced( l + 1 );

	for( int i = 0; i < l; i++ ) {
		data[i] = text[start + i];
	}

	data[l] = '\0';
	len		= l;
}

#endif /* !__STR_H__ */
