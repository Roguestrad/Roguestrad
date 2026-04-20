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

class idStr
{
public:
	/*!
		\brief Constructs an empty idStr object.

		This constructor initializes an empty idStr object by calling the Construct method. It is marked as ID_INLINE, indicating that it should be inlined for performance reasons. The constructor is
	   typically used when a new empty string object needs to be created without any initial content.

	*/
	idStr();

	/*!
		\brief Move constructor for idStr that transfers ownership of resources from another idStr object

		This move constructor initializes a new idStr object by transferring the resources from an existing idStr object. It first calls Construct to initialize the object and then uses std::move to
	   transfer the contents of the source object. This is an optimized way to construct a new string object when the source object is no longer needed, avoiding unnecessary copying of string data.

		\param text An rvalue reference to another idStr object whose resources will be moved to this object
	*/
	idStr( idStr&& text ) noexcept;

	/*!
		\brief Constructs a new idStr object as a copy of an existing idStr object

		This constructor creates a new idStr object by copying the contents of an existing idStr object. It first initializes the object using the Construct() method, then calculates the length of the
	   source string, allocates sufficient memory, and copies the string data. The length field is updated to reflect the copied string length.

		\param text The idStr object to copy from
	*/
	idStr( const idStr& text );

	/*!
		\brief Constructs a new string by extracting a substring from the given string between the specified start and end indices.

		This constructor creates a new string by copying a portion of the input string text. The substring is defined by the start and end indices. If the start index is negative, it is clamped to
	   zero. If the end index exceeds the length of the input string, it is clamped to the string length. If the start index is greater than the end index, the resulting string will be empty. The
	   constructor ensures that the new string is properly null-terminated and has sufficient allocated space for the substring plus the null terminator.

		\param text The input string from which the substring is extracted
		\param start The starting index of the substring (inclusive)
		\param end The ending index of the substring (exclusive)
	*/
	idStr( const idStr& text, int start, int end );

	/*!
		\brief Initializes a new string instance by copying the contents of the provided C-string.

		This constructor creates a new string object by copying the contents of the provided C-string. It allocates memory sufficient to hold the string plus a null terminator, then copies the source
	   string into the allocated buffer. The constructor handles null input gracefully by initializing an empty string. The implementation includes 64-bit compatibility fixes for size_t to int
	   conversion.

		\param text Pointer to the null-terminated C-string to copy
	*/
	idStr( const char* text );

	/*!
		\brief Constructs a new string by extracting a substring from the given text using specified start and end indices.

		This constructor initializes a new string by copying a portion of the input text. It takes a start and end index to define the range of characters to extract. The start index is clamped to the
	   beginning of the string if it's negative, and the end index is clamped to the end of the string if it exceeds the length. If the calculated length is negative, it is set to zero. The
	   constructor ensures that the allocated buffer is large enough to hold the extracted substring plus a null terminator.

		\param text The input character array from which to extract the substring.
		\param start The starting index of the substring to extract.
		\param end The ending index of the substring to extract.
	*/
	idStr( const char* text, int start, int end );

	/*!
		\brief Constructs a string representation of a boolean value

		This constructor initializes a string object with the character representation of a boolean value. The boolean value is converted to either '1' or '0' and stored in the string. The constructor
	   ensures that the string has sufficient allocation for the boolean character and the null terminator

		\param b The boolean value to convert to a string
	*/
	explicit idStr( const bool b );

	/*!
		\brief Constructs a string object with a single character.

		This constructor initializes a string object with a single character provided as the argument. It allocates memory for the character and a null terminator, then sets the string length to one.

		\param c The character to initialize the string with
	*/
	explicit idStr( const char c );

	/*!
		\brief Constructs a string representation of the given integer value.

		This constructor initializes a string object by converting the provided integer value into its string representation. It uses a temporary buffer to format the integer using snprintf, then
	   allocates enough memory to hold the resulting string and copies it into the internal data buffer. The length of the resulting string is stored in the len member.

		\param i The integer value to convert to a string
	*/
	explicit idStr( const int i );

	/*!
		\brief Constructs a string representation of the given unsigned integer.

		This constructor initializes a string object with the decimal string representation of the provided unsigned integer. It uses a temporary buffer to format the integer using snprintf, then
	   allocates and copies the formatted string into the internal storage. The length of the resulting string is tracked and stored.

		\param u The unsigned integer value to convert to a string
	*/
	explicit idStr( const unsigned u );

	/*!
		\brief Constructs a string representation of the given float value, with trailing zeros and decimal points removed.

		This constructor converts a floating-point number into its string representation. The implementation first formats the float into a temporary buffer using snprintf. It then removes any
	   trailing zeros after the decimal point and eliminates a trailing decimal point if present. The resulting string is then copied into the idStr object's internal buffer.

		\param f The float value to convert to a string
	*/
	explicit idStr( const float f );

	//! Destructor for the idStr class that releases any allocated memory.
	~idStr();

	//! Returns the total size in bytes of this string instance including allocated memory
	size_t		Size() const;

	//! Returns a pointer to the null-terminated character array representing the string's content.
	const char* c_str() const;
	operator const char*() const;
	operator const char*();

	char					operator[]( int index ) const;
	char&					operator[]( int index );

	void					operator=( idStr&& text ) noexcept; // Admer: added move operator
	void					operator=( const idStr& text );
	void					operator=( const char* text );

	friend idStr			operator+( const idStr& a, const idStr& b );
	friend idStr			operator+( const idStr& a, const char* b );
	friend idStr			operator+( const char* a, const idStr& b );

	friend idStr			operator+( const idStr& a, const float b );
	friend idStr			operator+( const idStr& a, const int b );
	friend idStr			operator+( const idStr& a, const unsigned b );
	friend idStr			operator+( const idStr& a, const bool b );
	friend idStr			operator+( const idStr& a, const char b );

	idStr&					operator+=( const idStr& a );
	idStr&					operator+=( const char* a );
	idStr&					operator+=( const float a );
	idStr&					operator+=( const char a );
	idStr&					operator+=( const int a );
	idStr&					operator+=( const unsigned a );
	idStr&					operator+=( const bool a );

	// case sensitive compare
	friend bool				operator==( const idStr& a, const idStr& b );
	friend bool				operator==( const idStr& a, const char* b );
	friend bool				operator==( const char* a, const idStr& b );

	// case sensitive compare
	friend bool				operator!=( const idStr& a, const idStr& b );
	friend bool				operator!=( const idStr& a, const char* b );
	friend bool				operator!=( const char* a, const idStr& b );

	/*!
		\brief Compares this string with the provided text for equality in a case-sensitive manner

		This function performs a case-sensitive comparison between the current string instance and the provided character array. It returns true if the strings are identical, and false otherwise. The
	   function includes an assertion to ensure the input text pointer is not null before performing the comparison. The comparison is done using the internal Cmp method which handles the actual
	   string comparison logic.

		\param text The null-terminated character array to compare with this string
		\return true if the strings are identical, false otherwise
		\throws assertion failure if the input text pointer is null
	*/
	bool					Equals( const char* text ) const;

	/*!
		\brief Compares this string with the provided text and returns an integer indicating their lexicographical relationship

		This function performs a lexicographical comparison between the string data stored in this idStr object and the provided null-terminated character string. It is a constant member function that
	   does not modify the object's state. The comparison is performed by delegating to the static idStr::Cmp function which handles the actual string comparison logic. The function includes an
	   assertion to ensure that the input text pointer is not null.

		\param text The null-terminated character string to compare with this object's string data
		\return A negative integer if this string is lexicographically less than the provided text, zero if they are equal, or a positive integer if this string is lexicographically greater than the
	   provided text \throws assertion failure if the input text pointer is null
	*/
	int						Cmp( const char* text ) const;

	/*!
		\brief Compares the first n characters of this string with the given text

		This function performs a case-sensitive comparison of the first n characters of the current string with the provided text. It is useful for partial string matching where only a portion of the
	   string needs to be compared. The function asserts that the provided text pointer is valid before performing the comparison. It delegates the actual comparison to the static idStr::Cmpn
	   function.

		\param text The null-terminated string to compare against
		\param n The maximum number of characters to compare
		\return Negative value if this string is lexicographically less than text, zero if they are equal, positive value if this string is lexicographically greater than text
		\throws assertion failure if text is null
	*/
	int						Cmpn( const char* text, int n ) const;

	/*!
		\brief Compares the prefix of this string with the given text.

		This function compares the beginning of the string with the provided text up to the length of the text. It returns an integer less than, equal to, or greater than zero if the prefix of this
	   string is found, respectively, to be less than, to match, or to be greater than the text.

		\param text The null-terminated string to compare the prefix against
		\return An integer less than, equal to, or greater than zero if the prefix of this string is found, respectively, to be less than, to match, or to be greater than the text
		\throws assertion failure if text is null
	*/
	int						CmpPrefix( const char* text ) const;

	/*!
		\brief Compares the string with the provided text in a case-insensitive manner.

		This function performs a case-insensitive comparison between the current string and the provided text. It returns true if the strings are equal ignoring case differences, and false otherwise.
	   The function uses the idStr::Icmp function internally to perform the comparison.

		\param text The null-terminated string to compare against
		\return True if the string is equal to the provided text ignoring case, false otherwise
		\throws asserts if the provided text pointer is null
	*/
	bool					EqualsIgnoreCase( const char* text ) const;

	/*!
		\brief Performs a case-insensitive comparison between the string and the provided text.

		This function compares the current string object with the provided text in a case-insensitive manner. It returns zero if the strings are equal, a negative value if the current string is
	   lexicographically less than the provided text, and a positive value if the current string is lexicographically greater than the provided text. The function asserts that the provided text
	   pointer is not null.

		\param text The null-terminated string to compare against.
		\return Zero if the strings are equal, negative if this string is less than the provided text, positive if this string is greater than the provided text.
		\throws assertion failure if text is null
	*/
	int						Icmp( const char* text ) const;

	/*!
		\brief Performs a case-insensitive comparison of the first n characters of this string with the given text

		This function compares the first n characters of the current string with the provided text in a case-insensitive manner. It internally calls the static Icmpn function with the current string
	   data and the provided parameters. The comparison stops after n characters or when a null terminator is encountered, whichever comes first. The function is typically used for prefix matching
	   where the exact length of the prefix is known.

		\param text The null-terminated string to compare against
		\param n The maximum number of characters to compare
		\return Zero if the first n characters match (case-insensitive), a negative value if this string is lexicographically less than text, or a positive value if this string is lexicographically
	   greater than text \throws asserts if the text parameter is null
	*/
	int						Icmpn( const char* text, int n ) const;

	/*!
		\brief Performs a case-insensitive comparison of the string with the prefix of the given text.

		This function compares the current string with the provided text, checking if the string is a prefix of the text in a case-insensitive manner. It uses the idStr::Icmpn function to perform the
	   comparison up to the length of the provided text.

		\param text The text to compare the string prefix against.
		\return An integer value indicating the result of the comparison. Returns zero if the string is a prefix of the text, a negative value if the string is lexicographically less than the text
	   prefix, and a positive value otherwise. \throws assertion failure if the input text pointer is null
	*/
	int						IcmpPrefix( const char* text ) const;

	/*!
		\brief Checks if the string starts with the specified prefix, ignoring case differences.

		This function performs a case-insensitive comparison of the beginning of the string with the provided prefix. It returns true if the string starts with the prefix, and false otherwise. The
	   comparison is done using the Icmpn function which is a case-insensitive string comparison function.

		\param prefix The character array that serves as the prefix to check for at the beginning of the string
		\return True if the string starts with the given prefix ignoring case, false otherwise
		\throws asserts if prefix is null
	*/
	bool					StartsWithIgnoreCase( const char* prefix ) const;

	/*!
		\brief Checks if the string ends with the specified suffix, ignoring case differences.

		This function determines whether the current string ends with the given suffix string, performing a case-insensitive comparison. It first checks if the suffix length exceeds the string length,
	   returning false in that case. Otherwise, it compares the end portion of the string with the suffix using case-insensitive comparison.

		\param suffix The suffix string to check for at the end of this string
		\return True if the string ends with the suffix (case-insensitive), false otherwise
	*/
	bool					EndsWithIgnoreCase( const char* suffix ) const;

	/*!
		\brief Performs a case-insensitive comparison of the string with the provided text while ignoring color codes.

		This function compares the current string instance with the provided text in a case-insensitive manner, but it ignores any color codes that might be present in the strings. It is useful for
	   comparing strings where color formatting should not affect the comparison result. The function internally delegates to the static IcmpNoColor function which handles the actual comparison logic.

		\param text The null-terminated character array to compare with the current string.
		\return An integer less than, equal to, or greater than zero if the current string is found, respectively, to be less than, to match, or be greater than the provided text.
		\throws asserts if the provided text pointer is null.
	*/
	int						IcmpNoColor( const char* text ) const;

	/*!
		\brief Compares two paths and ensures folders come first

		This function performs a path comparison between the current string and the provided text parameter. It ensures that directory paths are properly ordered by placing folders before files in the
	   comparison result. The function internally delegates to the static IcmpPath method which handles the actual comparison logic.

		\param text The null-terminated string containing the path to compare against
		\return Negative value if this path is less than the provided text, zero if they are equal, positive value if this path is greater than the provided text
		\throws assertion failure if text is null
	*/
	int						IcmpPath( const char* text ) const;

	/*!
		\brief Performs a case-insensitive comparison of the first n characters of this string with the provided text, treating paths as case-insensitive.

		This function compares the first n characters of the current string with the provided text in a case-insensitive manner. It is designed to handle path comparisons where the case should be
	   ignored. The function relies on the static IcmpnPath function to perform the actual comparison. The input text must not be null.

		\param text The character array to compare against the current string.
		\param n The number of characters to compare.
		\return The result of the comparison, similar to strcmp, where 0 indicates equality, negative values indicate the current string is lexicographically less than the text, and positive values
	   indicate the current string is lexicographically greater than the text. \throws assertion error if the input text is null.
	*/
	int						IcmpnPath( const char* text, int n ) const;

	/*!
		\brief Compares the string with a given text prefix, treating both as paths and returning zero if they match up to the length of the text.

		This function performs a case-insensitive comparison of the string with the provided text prefix. It treats both strings as paths, which means it compares the characters up to the length of
	   the text parameter. The function uses the IcmpnPath helper to perform the actual comparison, which ensures that the comparison is done correctly for path-like strings. The result is zero if the
	   prefix matches, otherwise it returns a non-zero value indicating the difference.

		\param text The text prefix to compare against the string
		\return Zero if the string matches the text prefix up to the length of the text, non-zero otherwise
		\throws Assertion failure if text is null
	*/
	int						IcmpPrefixPath( const char* text ) const;

	//! Returns the length of the string
	int						Length() const;

	//! Returns the amount of memory allocated for the string data.
	int						Allocated() const;

	/*!
		\brief Clears the string content by ensuring allocation for one character and setting it to null terminator.

		This function ensures that the string has allocated memory for at least one character, then sets the first character to the null terminator and resets the string length to zero. It effectively
	   empties the string content while maintaining proper memory allocation.

	*/
	void					Empty();

	//! Returns true if the string is empty.
	bool					IsEmpty() const;

	//! Clears the string content, resetting it to an empty state.
	void					Clear();

	/*!
		\brief Appends a single character to the string, ensuring sufficient buffer allocation

		This function adds a character to the end of the string by first ensuring that the internal buffer has enough space to accommodate the new character and the null terminator. It updates the
	   string length and properly null-terminates the string after appending the character.

		\param a The character to append to the string
	*/
	void					Append( const char a );

	/*!
		\brief Appends the contents of another idStr object to this string

		This function appends the characters from the provided idStr object to the end of the current string. It first calculates the new length required for the combined string, ensures that the
	   internal buffer has sufficient capacity, and then copies the characters from the source string. The function updates the length of the current string and ensures null termination of the
	   resulting string.

		\param text The idStr object whose contents will be appended to this string
	*/
	void					Append( const idStr& text );

	/*!
		\brief Appends the given text to the end of the string

		This function appends the contents of the provided text parameter to the end of the current string. It first calculates the new length required for the combined string, ensures that the
	   internal buffer has sufficient capacity, and then copies the characters from the input text into the buffer. The function handles null input by doing nothing, and ensures the resulting string
	   remains null-terminated.

		\param text The null-terminated string to append to this string
	*/
	void					Append( const char* text );

	/*!
		\brief Appends a specified number of characters from a text string to this string instance

		This function appends a portion of the provided text string to the current string instance. It takes care of memory allocation to accommodate the new content and ensures proper null
	   termination. The function only appends characters up to the specified length or until a null terminator is encountered in the source text, whichever comes first. If either the text pointer is
	   null or the length is zero, the function performs no operation.

		\param text The character array to append from
		\param len The maximum number of characters to append
	*/
	void					Append( const char* text, int len );

	/*!
		\brief Inserts a character at the specified index in the string, shifting existing characters to the right.

		This function inserts a single character at the given index within the string. If the index is negative, it is clamped to zero. If the index exceeds the string length, it is clamped to the end
	   of the string. The function ensures sufficient memory is allocated before performing the insertion. All characters at and after the insertion point are shifted one position to the right to make
	   room for the new character. The string length is increased by one after the insertion.

		\param a The character to insert into the string
		\param index The position at which to insert the character
	*/
	void					Insert( const char a, int index );

	/*!
		\brief Inserts the specified text into the string at the given index position.

		This function inserts the provided text into the string at the specified index. If the index is negative, it is set to 0. If the index is greater than the string length, it is set to the
	   string length. The function ensures there is enough allocated memory for the new text and shifts existing characters to make room for the inserted text. The function handles memory allocation
	   internally and updates the string length accordingly.

		\param text The null-terminated string to be inserted.
		\param index The position in the string where the text should be inserted.
	*/
	void					Insert( const char* text, int index );

	/*!
		\brief Converts all uppercase characters in the string to lowercase in place.

		This function modifies the string by converting all uppercase letters to their corresponding lowercase letters. It iterates through each character of the string until it reaches the null
	   terminator. For each character, it checks if it is an uppercase letter using the CharIsUpper helper function, and if so, converts it to lowercase by adding the difference between 'a' and 'A'.
	   The conversion is done in place, meaning the original string buffer is modified directly.

	*/
	void					ToLower();

	/*!
		\brief Converts all characters in the string to uppercase.

		This function iterates through each character of the string and converts lowercase letters to their corresponding uppercase letters. Non-alphabetic characters remain unchanged. The conversion
	   is done in-place, modifying the original string object.

	*/
	void					ToUpper();

	//! Checks if the string contains only numeric characters.
	bool					IsNumeric() const;

	//! Checks if the string represents a color code.
	bool					IsColor() const;

	//! Checks if the string contains any lowercase characters.
	bool					HasLower() const;

	//! Checks if the string contains any uppercase characters.
	bool					HasUpper() const;

	//! Returns the length of the string excluding color escape sequences.
	int						LengthWithoutColors() const;

	/*!
		\brief Removes color codes from the string and returns a reference to the modified string.

		This function strips all color formatting codes from the string data. It modifies the string in-place by calling the static RemoveColors function and then updates the length of the string to
	   reflect the new content. The function returns a reference to the modified string to allow for method chaining.

		\return A reference to the modified string with color codes removed
	*/
	idStr&					RemoveColors();

	/*!
		\brief Sets the string length to the specified value, truncating the string if necessary.

		This function ensures that the string does not exceed the specified length. If the current length of the string is greater than the new length, it truncates the string by setting the character
	   at the new length position to null terminator and updates the internal length counter. If the current length is less than or equal to the new length, the function returns without making any
	   changes.

		\param newlen The maximum allowed length of the string
	*/
	void					CapLength( int );

	/*!
		\brief Fills the string buffer with a repeated character up to a given length, resizing the buffer if necessary.

		The function first ensures that the internal storage can hold the required number of characters plus a terminating null.
		It then sets the string length, copies the repeated character into the buffer using memset, and finally writes a null terminator to mark the end of the string.

		\param ch The character used to fill the string.
		\param newlen The desired length of the string before the null terminator.
	*/
	void					Fill( const char ch, int newlen );

	//! Returns the length of the string in UTF-8 encoded characters.
	ID_INLINE int			UTF8Length();

	//! Returns the UTF-8 character at the specified index in the string.
	ID_INLINE uint32		UTF8Char( int& idx );

	/*!
		\brief Calculates the number of UTF-8 characters in a null-terminated byte string

		This function takes a null-terminated byte string encoded in UTF-8 and returns the count of actual Unicode characters contained within it. It properly handles multi-byte UTF-8 sequences by
	   analyzing the leading byte to determine how many trailing bytes follow. The function processes each character sequence, counting the UTF-8 characters rather than just the raw byte count. The
	   trailingBytes lookup table is used to efficiently determine the number of continuation bytes for each UTF-8 leading byte. This implementation correctly supports UTF-8 sequences of 1 to 4 bytes.

		\param s Pointer to a null-terminated UTF-8 encoded byte string
		\return The number of UTF-8 characters in the input string
	*/
	static int				UTF8Length( const byte* s );

	//! Extracts a UTF-8 character from a string at the specified index and advances the index.
	static ID_INLINE uint32 UTF8Char( const char* s, int& idx );

	/*!
		\brief Returns the next UTF-8 character from a byte array starting at the given index, advancing the index to the next character.

		This function extracts a single UTF-8 encoded character from the provided byte array starting at the specified index. It handles UTF-8 decoding, including multi-byte sequences, and advances
	   the index to point to the next character. The function correctly processes ASCII characters (0-127) as single bytes and multi-byte UTF-8 sequences (2-4 bytes) by reading the appropriate number
	   of trailing bytes. When the end of the string is reached, the index is incremented and a null terminator (0) is returned. This function is commonly used for iterating through UTF-8 encoded
	   strings to process individual characters.

		\param s Pointer to the UTF-8 encoded byte array to read from
		\param idx Reference to the index variable indicating current position in the array, which gets updated to point to the next character
		\return The decoded UTF-8 character as a 32-bit unsigned integer, or 0 if the end of the string is reached.
	*/
	static uint32			UTF8Char( const byte* s, int& idx );

	/*!
		\brief Appends a UTF-8 encoded character to the string

		This function takes a Unicode code point and appends its UTF-8 encoded representation to the string. It handles characters from different Unicode ranges by using the appropriate number of
	   bytes for UTF-8 encoding. Characters outside the valid Unicode range are replaced with a question mark

		\param c Unicode code point to append as UTF-8
	*/
	void					AppendUTF8Char( uint32 c );

	/*!
		\brief Converts the string to UTF-8 encoding.

		This function converts the current string to UTF-8 encoding by iterating through each character of the original string and appending it as a UTF-8 character. It uses a temporary string to
	   preserve the original data during conversion.

	*/
	ID_INLINE void			ConvertToUTF8();

	/*!
		\brief Checks if a byte sequence represents valid UTF-8 encoding and determines the encoding type

		This function validates whether a given byte sequence conforms to UTF-8 encoding rules. It examines each byte in the sequence to determine the number of bytes used for a UTF-8 character,
	   ensuring proper continuation byte formatting. The function handles various UTF-8 encodings including ASCII, 2-byte, 3-byte, and 4-byte sequences, and identifies different encoding types such as
	   those with or without byte order marks. It returns false if any invalid UTF-8 sequence is encountered. The function also updates the encoding parameter with the detected encoding type.

		\param s Pointer to the byte sequence to validate
		\param maxLen Maximum length of the byte sequence to check
		\param encoding Reference to store the detected UTF-8 encoding type
		\return True if the byte sequence represents valid UTF-8 encoding, false otherwise
	*/
	static bool				IsValidUTF8( const uint8* s, const int maxLen, utf8Encoding_t& encoding );

	/*!
		\brief Checks if a string is valid UTF-8 encoded and determines its encoding.

		This function validates whether the given character sequence represents a valid UTF-8 encoded string. It also detects the specific UTF-8 encoding used in the string. The function takes a
	   pointer to the string data, the maximum length to check, and a reference to store the detected encoding. It returns true if the string is valid UTF-8, and false otherwise.

		\param s Pointer to the null-terminated string to validate
		\param maxLen Maximum number of bytes to check in the string
		\param encoding Reference to store the detected UTF-8 encoding type
		\return True if the string is valid UTF-8, false otherwise
	*/
	static ID_INLINE bool	IsValidUTF8( const char* s, const int maxLen, utf8Encoding_t& encoding ) { return IsValidUTF8( ( const uint8* )s, maxLen, encoding ); }

	/*!
		\brief Checks if a given byte sequence is valid UTF-8 encoding up to a specified maximum length.

		This function verifies whether the provided byte sequence represents valid UTF-8 encoded text. It uses an internal helper function to perform the validation and returns true if the sequence is
	   valid UTF-8, or false otherwise. The validation stops after examining up to maxLen bytes.

		\param s Pointer to the byte sequence to validate as UTF-8.
		\param maxLen Maximum number of bytes to check for valid UTF-8 encoding.
		\return True if the byte sequence pointed to by s is valid UTF-8 encoding within the first maxLen bytes, false otherwise.
	*/
	static ID_INLINE bool	IsValidUTF8( const uint8* s, const int maxLen );

	/*!
		\brief Checks if a string is valid UTF-8 encoded up to a specified length

		This function validates whether the given character sequence represents valid UTF-8 encoding up to the specified maximum length. It is commonly used to ensure proper encoding of text data
	   before processing or storage. The function internally converts the input string to unsigned 8-bit integers and delegates validation to another implementation

		\param s Pointer to the null-terminated character string to validate
		\param maxLen Maximum number of bytes to check for valid UTF-8 encoding
		\return True if the string contains valid UTF-8 encoding up to maxLen bytes, false otherwise
	*/
	static ID_INLINE bool	IsValidUTF8( const char* s, const int maxLen ) { return IsValidUTF8( ( const uint8* )s, maxLen ); }

	/*!
		\brief Searches for the first occurrence of a character in the string within a specified range.

		This function searches for the first occurrence of the specified character 'c' in the string, starting from the 'start' index up to but not including the 'end' index. If 'end' is not specified
	   or is -1, the search extends to the end of the string. The function returns the index of the found character, or -1 if the character is not found.

		\param c The character to search for
		\param start The starting index for the search (inclusive)
		\param end The ending index for the search (exclusive), or -1 to search to the end of the string
		\return The index of the first occurrence of the character 'c' in the string, or -1 if the character is not found
	*/
	int						Find( const char c, int start = 0, int end = -1 ) const;

	/*!
		\brief Searches for the first occurrence of a substring within the string, with optional case sensitivity and range parameters

		This function performs a substring search within the string instance. It allows specifying a range of indices to search within, as well as controlling whether the search is case sensitive.
	   When the end parameter is set to -1, the search extends to the end of the string. The function returns the index of the first occurrence of the substring, or -1 if the substring is not found.

		\param text The substring to search for
		\param casesensitive Flag indicating if the search should be case sensitive
		\param start The starting index for the search
		\param end The ending index for the search, or -1 to search to the end of the string
		\return The index of the first occurrence of the substring, or -1 if not found
	*/
	int						Find( const char* text, bool casesensitive = true, int start = 0, int end = -1 ) const;

	//! Checks if the string matches the given filter pattern.
	bool					Filter( const char* filter, bool casesensitive ) const;

	/*!
		\brief Returns the index of the last occurrence of the specified character in the string, or -1 if the character is not found

		This function searches backwards through the string to find the last occurrence of the given character. It starts from the end of the string and moves towards the beginning, comparing each
	   character with the target character. If a match is found, it returns the index of that character. If no match is found after checking all characters, it returns -1 to indicate the character is
	   not present in the string

		\param c the character to search for
		\return the index of the last occurrence of the character, or -1 if not found
	*/
	int						Last( const char c ) const;

	//! Returns the leftmost 'len' characters of the string and stores them in the result.
	const char*				Left( int len, idStr& result ) const;

	/*!
		\brief Returns the rightmost 'len' characters of the string, storing the result in the provided idStr reference

		This function extracts the rightmost 'len' characters from the current string and stores the result in the provided idStr reference. If the specified length is greater than or equal to the
	   current string length, the entire string is copied to the result. Otherwise, it uses the Mid function to extract the appropriate substring starting from the position calculated as (Length() -
	   len)

		\param len number of characters to extract from the right side
		\param result reference to idStr where the extracted substring will be stored
		\return Pointer to the resulting string stored in the result parameter
	*/
	const char*				Right( int len, idStr& result ) const;

	/*!
		\brief Extracts a substring of specified length starting at a given index and stores it in the result parameter

		This function extracts a portion of the string starting at the specified index and continuing for the specified number of characters. The extracted substring is stored in the provided result
	   parameter. If the start index is out of bounds, or if the length is zero or negative, the function returns NULL. The function ensures that the extracted substring does not exceed the bounds of
	   the original string.

		\param start The starting index of the substring to extract
		\param len The number of characters to extract
		\param result The string object where the extracted substring will be stored
		\return A pointer to the result string containing the extracted substring, or NULL if the extraction parameters are invalid
	*/
	const char*				Mid( int start, int len, idStr& result ) const;

	//! Returns the leftmost 'len' characters from the string.
	idStr					Left( int len ) const;

	/*!
		\brief Returns the rightmost 'len' characters from the string

		This function extracts the last 'len' characters from the current string. If the requested length is greater than or equal to the total string length, the entire string is returned. The
	   function uses the Mid method internally to perform the extraction, calculating the starting position as (Length() - len)

		\param len number of characters to extract from the right side
		\return A new idStr object containing the rightmost 'len' characters
	*/
	idStr					Right( int len ) const;

	/*!
		\brief Returns a substring starting at the specified index with the specified length

		This function extracts a portion of the string starting at the specified index and continuing for the specified number of characters. If the start index is beyond the string length, or if the
	   length is zero or negative, an empty string is returned. If the requested substring extends beyond the end of the string, the function automatically adjusts the length to fit within the string
	   boundaries. The function is safe to use with out-of-bounds parameters and will never cause a buffer overflow.

		\param start The starting index of the substring to extract
		\param len The number of characters to extract
		\return A new idStr object containing the extracted substring
	*/
	idStr					Mid( int start, int len ) const;

	/*!
		\brief Formats the string using a printf-style format string and variable arguments

		This function performs a thread-safe sprintf operation on the string object, allowing for formatted string construction. It uses a va_list to handle variable arguments and employs
	   idStr::vsnPrintf for safe formatting. The function will cause a fatal error if the formatted string exceeds the maximum allowed buffer size. The result is assigned to the current string object.

		\param fmt printf-style format string specifying how subsequent arguments are converted for output
		\throws FatalError if the formatted string exceeds the maximum allowed buffer size
	*/
	void					Format( VERIFY_FORMAT_STRING const char* fmt, ... );

	/*!
		\brief Formats an integer with comma separators for thousands places

		This function takes an integer value and formats it as a string with comma separators inserted every three digits from the right. If the isCash parameter is set to true, a dollar sign is
	   prepended to the formatted string. The function handles negative numbers correctly by placing the comma separators after the negative sign. The formatting process calculates the appropriate
	   positions for comma insertion based on the length of the string representation of the number.

		\param num The integer value to be formatted
		\param isCash If true, prepends a dollar sign to the formatted result
		\return A formatted string representation of the integer with comma separators and optional dollar sign
	*/
	static idStr			FormatInt( const int num, bool isCash = false );

	/*!
		\brief Formats an integer as a cash amount string with commas as thousands separators.

		This function takes an integer value and formats it as a cash amount string, inserting commas as thousands separators for improved readability. The formatting is performed by delegating to the
	   FormatInt function with the comma separator flag enabled.

		\param num The integer value to format as cash
		\return A formatted string representation of the integer with comma separators suitable for displaying monetary amounts
	*/
	static idStr			FormatCash( const int num ) { return FormatInt( num, true ); }

	/*!
		\brief Removes all leading occurrences of a specified character from the string.

		This function iteratively removes the specified character from the beginning of the string until no more of that character exist at the front. The function modifies the string in place by
	   shifting the remaining characters left and adjusting the length accordingly. It continues removing the character as long as it appears at the start of the string.

		\param c the character to remove from the front of the string
	*/
	void					StripLeading( const char c );

	/*!
		\brief Strips the specified string from the front of this string as many times as it occurs.

		This function removes occurrences of the given string from the beginning of the current string. It continues removing the string from the front until the string no longer appears at the
	   beginning. The function modifies the current string in place.

		\param string The string to strip from the front of this string
	*/
	void					StripLeading( const char* string );

	/*!
		\brief Strips the leading occurrence of the specified string from the current string if it exists.

		This function attempts to remove the leading portion of the current string that matches the provided string parameter. It checks if the current string starts with the given string and removes
	   it if found. The function returns true if the string was stripped, false otherwise.

		\param string The string to strip from the front of the current string
		\return True if the leading string was found and removed, false otherwise
	*/
	bool					StripLeadingOnce( const char* string );

	/*!
		\brief Removes all trailing occurrences of a specified character from the string

		This function iterates from the end of the string backwards and removes all consecutive occurrences of the specified character that appear at the end of the string. The function modifies the
	   string in place by null-terminating the string at the correct position and updating the length counter. The function handles the case where the character to strip is not present at the end of
	   the string by doing nothing.

		\param c the character to strip from the end of the string
	*/
	void					StripTrailing( const char c );

	/*!
		\brief Strips occurrences of the specified string from the end of this string.

		This function removes all trailing occurrences of the provided string from the end of the current string. It repeatedly checks for matches at the end of the string and removes them until no
	   more matches are found. The operation modifies the current string in place.

		\param string The string to strip from the end of this string
	*/
	void					StripTrailing( const char* string );

	/*!
		\brief Removes a specified trailing string from the end of this string if it exists.

		This function checks if the provided string matches the trailing portion of this string and removes it if found. It performs a single removal operation, ensuring that only one instance of the
	   trailing string is stripped. The function returns true if the trailing string was found and removed, otherwise it returns false. This is useful for processing file names or other strings where
	   specific suffixes need to be conditionally removed.

		\param string The string to check for and remove from the end of this string
		\return True if the specified trailing string was found and removed from the end of this string; false otherwise.
	*/
	bool					StripTrailingOnce( const char* string );

	/*!
		\brief Case insensitive function to strip a suffix from the end of the string if it matches the given string.

		This function performs a case insensitive comparison between the provided string and the end of the current string. If they match, it removes the matching suffix from the current string and
	   returns true. Otherwise, it returns false. The function only strips the suffix once, even if it occurs multiple times at the end of the string. It is designed to work with strings that are
	   managed by the idStr class, using the 'data' and 'len' member variables to track the string content and length.

		\param string The suffix string to check for and potentially strip from the end of the current string.
		\return True if the suffix was found and stripped from the end of the string, false otherwise.
	*/
	bool					IStripTrailingOnce( const char* string );

	/*!
		\brief Removes all leading and trailing occurrences of a specified character from the string.

		This function removes all instances of the specified character from both the beginning and end of the string. It first removes leading occurrences of the character, then removes trailing
	   occurrences. The function modifies the string in place.

		\param c the character to strip from both the beginning and end of the string
	*/
	void					Strip( const char c );

	/*!
		\brief Strips leading and trailing whitespace from the provided string in-place.

		This function removes all leading and trailing whitespace characters from the input string. It processes the string by first stripping leading whitespace and then trailing whitespace. The
	   modification is performed directly on the internal data buffer of the string object. The function handles various whitespace characters including spaces, tabs, and control characters with
	   values less than or equal to a space character.

		\param string The input string to be stripped of leading and trailing whitespace
	*/
	void					Strip( const char* string );

	/*!
		\brief Removes trailing whitespace characters from the string

		This function strips all trailing whitespace characters from the string, including spaces, tabs, and other control characters that have a value less than or equal to a space character. The
	   function works by iterating from the end of the string backwards and removing characters until a non-whitespace character is found. The modification is done in-place on the internal data
	   buffer.

	*/
	void					StripTrailingWhitespace();

	/*!
		\brief Removes leading and trailing quotation marks from the string and returns a reference to the modified string.

		This function checks if the string starts with a quotation mark. If not, it returns immediately without modification. If the string starts with a quotation mark, it first removes any trailing
	   quotation mark, then shifts the remaining characters to the left to remove the leading quotation mark. The function modifies the string in place and returns a reference to itself.

		\return Reference to the modified string with leading and trailing quotation marks removed
	*/
	idStr&					StripQuotes();

	/*!
		\brief Replaces all occurrences of a specified substring with another substring and returns true if any replacements were made

		This function searches for all occurrences of the old substring within the string and replaces them with the new substring. It calculates the required buffer size based on the number of
	   replacements needed, allocates memory accordingly, and performs the actual replacement. The function returns true if at least one replacement was made, false otherwise. The implementation
	   handles cases where the new string is longer or shorter than the old string by adjusting the buffer allocation accordingly

		\param old The substring to be replaced
		\param nw The replacement substring
		\return True if at least one occurrence of the old substring was replaced, false otherwise
	*/
	bool					Replace( const char* old, const char* nw );

	/*!
		\brief Replaces all occurrences of a specified character with another character in the string.

		This function iterates through the string and replaces every instance of the specified old character with the new character. It returns true if any replacements were made, false otherwise. The
	   function modifies the string in place.

		\param old The character to be replaced
		\param nw The character to replace with
		\return True if at least one character was replaced, false if no replacements were made
	*/
	bool					ReplaceChar( const char old, const char nw );

	/*!
		\brief Copies a range of characters from the input text into the string, starting at start and ending at end.

		The function copies characters from the input text starting at the specified start index up to (but not including) the end index. It ensures that the string has enough allocated space to hold
	   the copied characters plus a null terminator. The resulting string is null terminated and the length is updated accordingly. If the end index is less than the start index, no characters are
	   copied.

		\param text The input character array from which to copy characters.
		\param start The starting index in the input text to begin copying from.
		\param end The ending index in the input text to stop copying at.
		\return No return value.
	*/
	ID_INLINE void			CopyRange( const char* text, int start, int end );

	/*!
		\brief Computes and returns a hash value for the file name, excluding the file extension.

		This function calculates a hash based on the characters of the file name stored in the idStr object. It converts all characters to lowercase and treats backslashes as forward slashes. The hash
	   computation stops when a period (.) is encountered, indicating the start of the file extension. The resulting hash value is masked to fit within the bounds of FILE_HASH_SIZE, which is a power
	   of two. This ensures that the hash can be used effectively as an index in a hash table of size FILE_HASH_SIZE.

		\return An integer hash value suitable for use as an index in a hash table of size FILE_HASH_SIZE
	*/
	int						FileNameHash() const;

	/*!
		\brief Converts backslashes to forward slashes in the string.

		This function iterates through each character of the string and replaces any backslash characters with forward slashes. The modification is done in-place on the current string object. The
	   function returns a reference to the modified string to allow for method chaining.

		\return A reference to the current string object after converting backslashes to forward slashes
	*/
	idStr&					BackSlashesToSlashes();

	/*!
		\brief Converts forward slashes to backslashes in the string.

		This function iterates through each character of the string and replaces any forward slash character with a backslash. The modification is done in place and the function returns a reference to
	   the modified string.

		\return A reference to the modified string with forward slashes converted to backslashes
	*/
	idStr&					SlashesToBackSlashes();

	/*!
		\brief Sets the file extension for the string, ensuring it begins with a dot.

		This function first removes any existing file extension from the string, then appends the provided extension. If the extension does not start with a dot, one is automatically added before
	   appending the extension. The function returns a reference to the modified string to allow for method chaining.

		\param extension The file extension to set, which may or may not start with a dot
		\return A reference to the modified string with the new file extension.
	*/
	idStr&					SetFileExtension( const char* extension );

	/*!
		\brief Removes the file extension from the string in place

		This function strips the file extension from the current string by finding the last occurrence of the '.' character and truncating the string at that position. It modifies the string in place
	   and returns a reference to the modified string. The function iterates backward through the string to find the last dot, which is assumed to indicate the start of the file extension. If no dot
	   is found, the string remains unchanged.

		\return A reference to the modified string with the file extension removed
	*/
	idStr&					StripFileExtension();

	/*!
		\brief Removes the file extension from the string by truncating at the first period character.

		This function modifies the string in-place by removing any file extension. It searches for the first period character in the string and truncates the string at that point. The function is
	   useful when dealing with file paths where multiple periods might be present in the filename. The operation is performed in-place and the modified string is returned.

		\return A reference to the modified string with the file extension removed
	*/
	idStr&					StripAbsoluteFileExtension();

	/*!
		\brief Appends a default file extension to the string if it does not already have an extension.

		This function checks if the current string already contains a file extension by looking for a period character from the end of the string. If no extension is found, it appends the provided
	   extension. If the extension does not start with a period, one is added before appending. The function modifies the string in place and returns a reference to itself.

		\param extension The file extension to append, which may or may not start with a period
		\return A reference to the modified string object.
	*/
	idStr&					DefaultFileExtension( const char* extension );

	/*!
		\brief Sets the path to the default location if no path is specified

		This function checks if the current string represents an absolute path by examining the first character. If the path is absolute (starting with '/' or '\''), it returns the string unchanged.
	   Otherwise, it prepends the provided basepath to the current string and updates the string with the result

		\param basepath the base path to prepend to the current string if it's not absolute
		\return a reference to the updated string with the default path set
	*/
	idStr&					DefaultPath( const char* basepath );

	/*!
		\brief Appends a partial path to the current string, ensuring proper path separator handling.

		This function appends the provided text to the current string, treating it as a partial path. It handles path separators by normalizing backslashes to forward slashes and ensures that path
	   segments are properly separated. The function automatically allocates additional memory as needed to accommodate the new content. If the current string is not empty, it adds a forward slash
	   separator before appending the new path segment. If the input text starts with a forward slash, it skips that initial separator to prevent duplicate separators in the result.

		\param text The partial path to append to the current string
	*/
	void					AppendPath( const char* text );

	/*!
		\brief Removes the filename from a path string, keeping only the directory portion.

		This function modifies the current string by removing the filename portion, leaving only the directory path. It searches for the last occurrence of either a forward slash or backslash
	   character and truncates the string at that position. If no path separators are found, the entire string is cleared. The function returns a reference to the modified string for chaining
	   operations.

		\return A reference to the modified string with the filename portion removed
	*/
	idStr&					StripFilename();

	/*!
		\brief Removes the path portion from the string, keeping only the filename.

		This function modifies the string in place to remove any directory path components, leaving only the filename. It searches for the last occurrence of either a forward slash or backslash
	   character and removes all characters up to and including that delimiter. If no path separators are found, the entire string remains unchanged.

		\return A reference to the modified string with the path removed
	*/
	idStr&					StripPath();

	/*!
		\brief Extracts the file path portion from this string and stores it in the destination string.

		This function identifies the file path by locating the last occurrence of a forward slash or backslash character. It then copies all characters up to but not including this delimiter into the
	   destination string. If no delimiter is found, the entire string is copied to the destination. The function operates on a const instance of idStr, meaning it does not modify the original string.

		\param dest The destination string where the extracted file path will be stored.
	*/
	void					ExtractFilePath( idStr& dest ) const;

	/*!
		\brief Copies the filename portion of this string to the provided destination string.

		This function extracts the filename from the current string by finding the last occurrence of a path separator character, either forward slash '/' or backslash ''. It then copies the substring
	   starting from that position to the end of the string into the destination string. The function handles cases where the path separator is not found by returning the entire string as the
	   filename.

		\param dest The destination string object that will receive the extracted filename.
	*/
	void					ExtractFileName( idStr& dest ) const;

	/*!
		\brief Extracts the filename without the extension from this string and stores it in the destination string.

		This function processes the current string to find the base filename by locating the last path separator and then finding the extension delimiter. It copies the substring between these
	   positions to the destination string. The function handles both forward slashes and backslashes as path separators. If no extension is found, it copies the entire filename portion.

		\param dest The destination string where the extracted filename will be stored
	*/
	void					ExtractFileBase( idStr& dest ) const;

	/*!
		\brief Extracts the file extension from the string and copies it to the destination string

		This function locates the last occurrence of a period character in the string and extracts the substring that follows it, which represents the file extension. If no period is found, the
	   destination string is set to empty. The function searches backwards from the end of the string to find the last period, ensuring that it correctly identifies the file extension even when the
	   filename contains multiple periods

		\param dest destination string where the extracted file extension will be copied
	*/
	void					ExtractFileExtension( idStr& dest ) const;

	//! Checks if the string ends with the specified file extension.
	bool					CheckExtension( const char* ext );

	/*!
		\brief Removes surrounding double quotes from the string if they exist.

		This function checks if the string starts with a double quote character and removes it if present. It also checks if the last character of the string is a double quote and removes it if
	   present. The function modifies the current string object in place.

	*/
	void					StripDoubleQuotes();

	//! Splits the string into a list using the specified delimiters
	bool					Split( idList<idStr>& list, const char delimiter = ',', const char groupDelimiter = '\'' ) const;

	/*!
		\brief Calculates the length of a null-terminated string by counting characters until the null terminator is encountered.

		This function computes the length of a C-style string by iterating through each character until it reaches the null terminator. It is a utility method that replaces the standard library strlen
	   function. The implementation uses a simple loop that increments a counter for each character in the string. The function handles empty strings correctly, returning 0 for null pointers or
	   strings consisting only of the null terminator.

		\param s Pointer to the null-terminated string whose length is to be calculated
		\return The number of characters in the string, excluding the null terminator
	*/
	static int				Length( const char* s );

	/*!
		\brief Converts all uppercase characters in the input string to lowercase in place and returns the modified string.

		This function modifies the input string by converting any uppercase letters to their corresponding lowercase letters. It processes the string character by character until it reaches the null
	   terminator. The conversion is done in place, meaning the original string buffer is modified. The function returns a pointer to the same string after modification.

		\param s Pointer to the null-terminated character string to convert to lowercase
		\return Pointer to the modified input string with all uppercase characters converted to lowercase
	*/
	static char*			ToLower( char* s );

	/*!
		\brief Converts all lowercase characters in the input string to uppercase in place and returns a pointer to the modified string.

		This function iterates through each character of the input string and checks if it is a lowercase letter. If so, it converts the character to its uppercase equivalent by subtracting the
	   difference between 'a' and 'A'. The modification is done in place, meaning the original string buffer is altered. The function returns a pointer to the same string after the conversion is
	   complete.

		\param s Pointer to the null-terminated input string to be converted to uppercase
		\return Pointer to the modified input string with all lowercase characters converted to uppercase
	*/
	static char*			ToUpper( char* s );

	/*!
		\brief Checks if a string represents a valid numeric value, including integers and floating-point numbers.

		This function determines whether the provided string contains a valid numeric representation. It accepts integers and floating-point numbers, including negative values. The function returns
	   true if the entire string consists of valid numeric characters, false otherwise. It handles both integer and decimal point separators but does not allow multiple decimal points within the same
	   number.

		\param s The null-terminated string to check for numeric validity.
		\return True if the string represents a valid numeric value, false otherwise.
	*/
	static bool				IsNumeric( const char* s );

	/*!
		\brief Checks if a string represents a color code

		This function determines whether the given string starts with a color escape character followed by a non-null, non-space character. It is used to identify color codes in text processing,
	   particularly in console output and text editing functionality. The function is designed to work with the Doom 3 BFG color coding system where color codes are represented by a special escape
	   character followed by a color identifier.

		\param s Pointer to the string to check for color code
		\return True if the string represents a valid color code, false otherwise
	*/
	static bool				IsColor( const char* s );

	/*!
		\brief Checks if the input string contains any lowercase characters.

		This function iterates through each character of the input string and returns true as soon as it finds a lowercase character. If the string is null or contains no lowercase characters, it
	   returns false. The function uses the CharIsLower helper function to determine if a character is lowercase.

		\param s The input string to check for lowercase characters
		\return true if the string contains at least one lowercase character, false otherwise
	*/
	static bool				HasLower( const char* s );

	/*!
		\brief Checks if the input string contains any uppercase characters.

		This function iterates through each character of the input string and returns true as soon as it finds an uppercase character. If the string is null or contains no uppercase characters, it
	   returns false. The function uses the CharIsUpper helper function to determine if a character is uppercase.

		\param s The input string to check for uppercase characters
		\return true if the string contains at least one uppercase character, false otherwise
	*/
	static bool				HasUpper( const char* s );

	/*!
		\brief Returns the length of a string excluding color escape sequences.

		This function calculates the length of the input string while ignoring any color escape sequences. Color escape sequences in this context are typically two-character sequences starting with a
	   backslash followed by a character. The function properly handles null input by returning zero.

		\param s The input string to measure, can be null
		\return The length of the string excluding color escape sequences
	*/
	static int				LengthWithoutColors( const char* s );

	/*!
		\brief Removes color escape sequences from the input string and returns a pointer to the modified string.

		This function processes the input string to eliminate color escape sequences, which are typically used in terminal or console applications to format text with colors. It iterates through the
	   string, identifying and skipping over color escape sequences while copying regular characters to the output. The function modifies the string in place and returns a pointer to the beginning of
	   the modified string. Color escape sequences are identified by the IsColor function, which checks for specific patterns indicating color formatting.

		\param s Pointer to the input string from which color sequences will be removed
		\return Pointer to the modified input string with color sequences removed
	*/
	static char*			RemoveColors( char* s );

	/*!
		\brief Compares two null-terminated strings and returns an integer indicating their lexicographical relationship.

		This function performs a case-sensitive lexicographical comparison of two C-style strings. It returns zero if the strings are identical, a negative value if the first string is
	   lexicographically less than the second, and a positive value if the first string is lexicographically greater than the second. The comparison is done character by character until a difference
	   is found or until the end of one of the strings is reached. The implementation uses a bitwise operation to efficiently determine the sign of the difference between characters.

		\param s1 First null-terminated string to compare
		\param s2 Second null-terminated string to compare
		\return Zero if strings are equal, negative value if s1 < s2, positive value if s1 > s2
	*/
	static int				Cmp( const char* s1, const char* s2 );

	/*!
		\brief Performs a case-sensitive comparison of the first n characters of two strings and returns an integer indicating their relationship.

		This function compares up to n characters of two null-terminated strings s1 and s2. It returns zero if the strings are equal up to the specified number of characters, a negative value if s1 is
	   lexicographically less than s2, or a positive value if s1 is lexicographically greater than s2. The comparison is performed character by character using their ASCII values. The function uses a
	   loop to iterate through the strings, decrementing the character count n with each comparison. If n reaches zero before the end of the strings, it returns zero, indicating equality up to that
	   point. If a difference is found, it returns the sign of the difference between the character values.

		\param s1 First null-terminated string to compare
		\param s2 Second null-terminated string to compare
		\param n Maximum number of characters to compare
		\return Zero if the first n characters of s1 and s2 are equal, negative if s1 is lexicographically less than s2, positive if s1 is lexicographically greater than s2
		\throws Assertion failure if n is negative
	*/
	static int				Cmpn( const char* s1, const char* s2, int n );

	/*!
		\brief Performs a case-insensitive comparison of two null-terminated strings and returns an integer indicating their relationship.

		This function compares two strings character by character in a case-insensitive manner. It converts uppercase letters to lowercase during comparison, ensuring that the comparison treats 'A'
	   and 'a' as equivalent. The function continues comparing until it finds a difference between characters or until it reaches the end of one of the strings. It returns a negative value if the
	   first string is lexicographically less than the second string, zero if they are equal, and a positive value if the first string is greater than the second string.

		\param s1 First null-terminated string to compare
		\param s2 Second null-terminated string to compare
		\return Negative value if s1 is lexicographically less than s2, zero if they are equal, positive value if s1 is greater than s2
	*/
	static int				Icmp( const char* s1, const char* s2 );

	/*!
		\brief Performs a case-insensitive comparison of the first n characters of two strings and returns an integer indicating their relationship.

		This function compares two null-terminated strings character by character up to n characters or until a null terminator is reached. The comparison is case-insensitive, meaning uppercase and
	   lowercase letters are treated as equal. The function returns zero if the strings are equal up to n characters, a negative value if the first string is lexicographically less than the second,
	   and a positive value if the first string is greater. The comparison is performed by converting uppercase letters to lowercase for comparison purposes, while preserving the original characters
	   in the comparison.

		\param s1 First null-terminated string to compare
		\param s2 Second null-terminated string to compare
		\param n Maximum number of characters to compare
		\return Zero if the strings are equal up to n characters, negative if s1 is lexicographically less than s2, positive if s1 is greater than s2
		\throws Assertion failure if n is negative
	*/
	static int				Icmpn( const char* s1, const char* s2, int n );

	/*!
		\brief Compares two strings ignoring color codes and case differences

		This function performs a case-insensitive comparison of two strings while skipping over color codes that are typically used in console output. Color codes in this context are assumed to be
	   two-character sequences starting with the escape character. The comparison continues until either the end of the strings is reached or a difference is found. The function handles uppercase and
	   lowercase letters by converting uppercase letters to lowercase during comparison. It returns zero if the strings are equal, a negative value if the first string is lexicographically less than
	   the second, and a positive value if the first string is greater.

		\param s1 First string to compare
		\param s2 Second string to compare
		\return Zero if the strings are equal, negative if s1 is less than s2, positive if s1 is greater than s2
	*/
	static int				IcmpNoColor( const char* s1, const char* s2 );

	/*!
		\brief Compares two path strings case-insensitively while ensuring folders appear before files

		This function performs a case-insensitive comparison of two null-terminated strings representing file paths. It treats forward slashes and backslashes as equivalent directory separators. The
	   comparison logic ensures that directory paths are considered less than file paths when they have the same prefix, effectively placing folders before files in the ordering. The function stops
	   comparing when a difference is found or when the end of either string is reached. It handles case conversion for letters and directory separator conversion, making it suitable for filesystem
	   operations where case insensitivity is required.

		\param s1 First path string to compare
		\param s2 Second path string to compare
		\return Zero if strings are equal, negative if s1 is lexicographically less than s2, positive if s1 is lexicographically greater than s2
	*/
	static int				IcmpPath( const char* s1, const char* s2 );

	/*!
		\brief Compares two path strings case-insensitively and ensures folders appear before files.

		This function performs a case-insensitive comparison of two null-terminated strings representing file paths. It treats forward slashes and backslashes as equivalent directory separators. The
	   comparison logic ensures that directory paths are considered less than file paths when they have the same prefix, effectively placing folders before files in the ordering. The function stops
	   comparing after n characters or when a difference is found. If the strings are equal up to the specified length, it returns zero. The function handles case conversion for letters and directory
	   separator conversion.

		\param s1 First path string to compare
		\param s2 Second path string to compare
		\param n Maximum number of characters to compare
		\return Zero if strings are equal up to n characters, negative if s1 is lexicographically less than s2, positive if s1 is lexicographically greater than s2
		\throws assertion failure if n is negative
	*/
	static int				IcmpnPath( const char* s1, const char* s2, int n );

	/*!
		\brief Appends the source string to the destination string, ensuring the total length does not exceed the specified size.

		This function appends the contents of the source string to the end of the destination string while ensuring that the total length does not exceed the specified size limit. It first calculates
	   the current length of the destination string and checks if appending the source string would cause an overflow. If an overflow is detected, it triggers an error. Otherwise, it uses the Copynz
	   function to safely append the source string to the destination.

		\param dest Destination character array to which the source string will be appended
		\param size Maximum size of the destination buffer including the null terminator
		\param src Source string to be appended to the destination
		\throws Error if the destination buffer would overflow when appending the source string
	*/
	static void				Append( char* dest, int size, const char* src );

	/*!
		\brief Copies a string from src to dest with a specified maximum size, ensuring null termination.

		This function performs a safe string copy operation that prevents buffer overflows by limiting the number of characters copied from the source string. It first checks if the source pointer is
	   valid and if the destination size is at least 1. If either check fails, it issues a warning and returns early. Otherwise, it copies up to destsize - 1 characters from src to dest, then
	   explicitly null-terminates the destination string at the final position to ensure it is properly terminated.

		\param dest Destination buffer where the string will be copied
		\param src Source string to be copied
		\param destsize Size of the destination buffer in characters
	*/
	static void				Copynz( char* dest, const char* src, int destsize );

	/*!
		\brief Formats a string into a destination buffer with size limitation and returns the number of characters written

		This function performs formatted string output into a destination buffer with specified size limit. It uses variadic arguments to format the string according to the provided format specifier.
	   The function guarantees that the output will be null-terminated and will not exceed the specified buffer size. If the formatting would overflow the buffer, a warning is issued and the return
	   value indicates the overflow. This is a safe alternative to standard snprintf that integrates with the engine's error reporting system.

		\param dest destination buffer to write the formatted string
		\param size size of the destination buffer
		\param fmt format string specifying how to format the output
		\param  variadic arguments for the format string
		\return the number of characters written to the buffer, or a negative value if overflow occurred
		\throws Warning message is issued if buffer overflow occurs during formatting
	*/
	static int				snPrintf( char* dest, int size, VERIFY_FORMAT_STRING const char* fmt, ... );

	/*!
		\brief Formats a string using a variable argument list and writes it to a destination buffer

		This function provides a cross-platform implementation of vsnprintf that handles string formatting with variable arguments. It uses platform-specific functions (_vsnprintf on Windows,
	   vsnprintf on other platforms) and ensures proper null termination of the result string. The function handles buffer overflow conditions by checking the return value and the size parameter,
	   returning -1 if the output was truncated or if an error occurred.

		\param dest destination buffer to write the formatted string
		\param size size of the destination buffer
		\param fmt format string specifying how to format the output
		\param argptr variable argument list containing the values to format
		\return the number of characters written to the destination buffer, or -1 if an error occurred or the output was truncated
	*/
	static int				vsnPrintf( char* dest, int size, const char* fmt, va_list argptr );

	/*!
		\brief Finds the first occurrence of a character in a string within a specified range

		This function searches for the first occurrence of a specified character within a given string, starting from a specified index and ending at another specified index. If the character is
	   found, the function returns the index of its first occurrence. If the character is not found or if the end index is not specified, the function returns -1. The function handles the case where
	   the end index is set to -1 by calculating the length of the string and using that as the end index.

		\param str The string to search in
		\param c The character to search for
		\param start The starting index for the search
		\param end The ending index for the search, or -1 to search to the end of the string
		\return The index of the first occurrence of the character in the string, or -1 if not found
	*/
	static int				FindChar( const char* str, const char c, int start = 0, int end = -1 );

	/*!
		\brief Finds the first occurrence of text within a string, with optional case sensitivity and position range.

		This function searches for the first occurrence of the specified text within the given string. It supports case-sensitive and case-insensitive searches and allows specifying a starting
	   position and an ending position for the search. The search is performed within the substring defined by the start and end positions. If the text is found, the function returns the index of the
	   first character of the match; otherwise, it returns -1.

		\param str The string to search within
		\param text The text to search for
		\param casesensitive Whether the search should be case sensitive
		\param start The starting index for the search
		\param end The ending index for the search, or -1 to search until the end of the string
		\return The index of the first occurrence of the text within the string, or -1 if the text is not found.
	*/
	static int				FindText( const char* str, const char* text, bool casesensitive = true, int start = 0, int end = -1 );

	/*!
		\brief Checks if a name matches a filter pattern that may contain wildcards.

		This function performs a pattern matching operation on a name string using a filter string that may contain wildcard characters. The filter supports three types of wildcards: asterisk (*)
	   matches any sequence of characters, question mark (?) matches any single character, and bracket expressions [abc] or [a-z] match any character within the brackets. The matching can be either
	   case-sensitive or case-insensitive depending on the casesensitive parameter. The function processes the filter character by character and handles nested structures such as bracket expressions
	   and escaped brackets. It returns true if the entire name matches the filter pattern, and false otherwise.

		\param filter The filter pattern string that may contain wildcards
		\param name The name string to match against the filter
		\param casesensitive If true, matching is case-sensitive; if false, matching is case-insensitive
		\return True if the name matches the filter pattern, false otherwise
	*/
	static bool				Filter( const char* filter, const char* name, bool casesensitive );

	/*!
		\brief Strips the media name from a file path by converting backslashes to forward slashes and truncating at the first extension.

		This function processes a file path string to extract just the media name portion. It converts all backslashes to forward slashes and stops processing at the first period character,
	   effectively removing file extensions. The resulting string is stored in the provided idStr reference.

		\param name Input file path to process
		\param mediaName Output string to store the processed media name
	*/
	static void				StripMediaName( const char* name, idStr& mediaName );

	/*!
		\brief Checks if a file name has a specific extension, ignoring case differences.

		This function compares the extension of a given file name with a specified extension. It performs a case-insensitive comparison, converting uppercase letters to lowercase during the comparison
	   process. The function returns true if the file name ends with the specified extension, and false otherwise.

		\param name The file name to check for the extension
		\param ext The extension to compare against the file name
		\return True if the file name ends with the specified extension (case-insensitive), false otherwise
	*/
	static bool				CheckExtension( const char* name, const char* ext );

	/*!
		\brief Converts an array of floating-point numbers into a formatted string with specified precision.

		This function takes an array of floating-point values and converts them into a string representation with a specified number of decimal places. The function uses a static buffer to avoid
	   memory allocation and handles precision by stripping trailing zeros and decimal points from the formatted output. It supports nested function calls by rotating through a fixed-size array of
	   buffers.

		\param array Pointer to the array of floating-point numbers to convert
		\param length Number of elements in the array
		\param precision Number of decimal places to include in the output
		\return A pointer to a static string containing the formatted floating-point values separated by spaces
	*/
	static const char*		FloatArrayToString( const float* array, const int length, const int precision );

	/*!
		\brief Returns a C-style quoted version of the input string with special characters escaped.

		This function takes a null-terminated string and returns a C-style quoted string where special characters such as backslash, newline, carriage return, tab, and quotes are escaped with
	   backslashes. The function uses a static buffer to avoid dynamic memory allocation and supports up to four nested calls. The returned string is null-terminated and includes the opening and
	   closing double quotes.

		\param str Input null-terminated string to be quoted and escaped.
		\return A pointer to a static buffer containing the C-style quoted and escaped string.
	*/
	static const char*		CStyleQuote( const char* str );

	/*!
		\brief Removes C-style quotation marks and escape sequences from a string

		This function takes a string that is expected to start with a quotation mark and processes it to remove the surrounding quotes and handle escape sequences. It supports common C-style escape
	   sequences like newline, tab, backslash, etc. The function uses a static buffer to avoid memory allocation and handles nested function calls by cycling through four buffers. The input string
	   must start with a quotation mark, or it will be returned unchanged. The function modifies the string in place within a static buffer and returns a pointer to that buffer.

		\param str The input string that is expected to start with a quotation mark, potentially containing C-style escape sequences
		\return A pointer to a static buffer containing the unquoted and unescaped string
		\throws assertion failure if the processed string does not end with a quotation mark
	*/
	static const char*		CStyleUnQuote( const char* str );

	/*!
		\brief Calculates a hash value for the provided string using a specific hashing algorithm.

		This function implements a custom string hashing algorithm that iterates through each character of the input string and computes a hash value. The hash is calculated by multiplying each
	   character's ASCII value with a factor that depends on its position in the string, and then summing these products. The algorithm is designed to provide a reasonable distribution of hash values
	   for different input strings.

		\param string The null-terminated character string to be hashed
		\return An integer hash value computed from the input string
	*/
	static int				Hash( const char* string );

	/*!
		\brief Computes a hash value for a given string of specified length using a simple polynomial rolling hash algorithm.

		This function implements a hash calculation for a string using a straightforward polynomial rolling hash approach. It iterates through each character in the string, multiplying the character
	   value by a factor based on its position and a fixed offset. The length parameter determines how many characters are processed. This hash function is used primarily for identifying user profiles
	   and session data in the game.

		\param string The character array to hash
		\param length The number of characters in the string to process
		\return The computed hash value as an integer
	*/
	static int				Hash( const char* string, int length );

	/*!
		\brief Computes a case-insensitive hash value for the provided string.

		This function calculates a hash value for the input string using a specific algorithm that treats uppercase and lowercase letters as equivalent. The hash is computed by iterating through each
	   character of the string, converting it to lowercase, and multiplying it by a factor that depends on its position in the string. The result is accumulated to produce the final hash value.

		\param string The input string to hash.
		\return The computed hash value as an integer.
	*/
	static int				IHash( const char* string );

	/*!
		\brief Computes a case-insensitive hash value for the given string of specified length.

		The function calculates a hash by iterating through each character of the input string, converting it to lowercase, and multiplying it by a factor that increases with the character's position.
	   This approach ensures that the hash value is case-insensitive and takes into account the position of characters in the string.

		\param string The input string to hash
		\param length The number of characters in the string to hash
		\return The computed hash value as an integer
	*/
	static int				IHash( const char* string, int length );

	/*!
		\brief Converts an uppercase ASCII character to lowercase

		This function takes a single character as input and converts it to lowercase if it is an uppercase ASCII letter. The conversion is performed by adding the difference between 'a' and 'A' to the
	   character value. If the input character is not an uppercase letter, it is returned unchanged. This function is typically used to normalize string case for comparison or processing.

		\param c Input character to convert to lowercase
		\return The lowercase version of the input character if it was an uppercase letter, otherwise the original character
	*/
	static char				ToLower( char c );

	/*!
		\brief Converts a lowercase ASCII character to uppercase.

		This function takes a single ASCII character as input and converts it to uppercase if it is a lowercase letter. The conversion is done by checking if the character falls within the range of
	   'a' to 'z'. If so, it adjusts the character value by subtracting the difference between 'a' and 'A' to map it to the uppercase equivalent. If the character is not a lowercase letter, it is
	   returned unchanged.

		\param c The ASCII character to convert to uppercase
		\return The uppercase version of the input character if it was a lowercase letter; otherwise, the original character.
	*/
	static char				ToUpper( char c );

	/*!
		\brief Determines if a character is printable based on ASCII and Western European high-ASCII ranges.

		This function checks whether a given character code falls within the range of printable ASCII characters (space through tilde) or Western European high-ASCII characters (from 0xA1 to 0xFF). It
	   is used to filter out non-printable characters in user input handling.

		\param c The character code to check for printable status
		\return True if the character is printable, false otherwise
	*/
	static bool				CharIsPrintable( int c );

	/*!
		\brief Checks if a character is a lowercase letter.

		This function determines whether the provided character is a lowercase letter. It accepts both regular ASCII lowercase letters (a-z) and Western European high-ASCII lowercase letters
	   (0xE0-0xFF). The function returns true if the character falls within any of these ranges, and false otherwise.

		\param c The character to check, provided as an integer.
		\return True if the character is a lowercase letter, false otherwise.
	*/
	static bool				CharIsLower( int c );

	/*!
		\brief Checks if a character is an uppercase letter, including standard ASCII and Western European high-ASCII characters.

		This function determines whether the provided character is an uppercase alphabetic character. It supports standard ASCII uppercase letters (A-Z) and extends support to Western European
	   high-ASCII characters in the range 0xC0 to 0xDF. The function returns true if the character falls within these ranges, indicating it is an uppercase letter.

		\param c The character to test for being an uppercase letter
		\return True if the character is an uppercase letter, false otherwise
	*/
	static bool				CharIsUpper( int c );

	/*!
		\brief Checks if a character is alphabetic, including standard ASCII letters and Western European high-ASCII characters.

		This function determines whether the provided character is an alphabetic character. It supports standard ASCII letters (a-z, A-Z) and extends support to Western European high-ASCII characters
	   in the range 0xC0 to 0xFF. This is useful for text processing that needs to handle international character sets beyond basic ASCII.

		\param c The character to test for being alphabetic
		\return True if the character is alphabetic, false otherwise
	*/
	static bool				CharIsAlpha( int c );

	/*!
		\brief Checks if a character is a numeric digit.

		This function determines whether the provided character is a numeric digit between '0' and '9'. It performs a simple range check to validate if the character falls within the numeric ASCII
	   range.

		\param c The character to check for numeric validity
		\return True if the character is a numeric digit, false otherwise
	*/
	static bool				CharIsNumeric( int c );

	/*!
		\brief Checks if a character is a newline character.

		This function determines whether the provided character is one of the common newline characters including line feed, carriage return, or vertical tab.

		\param c The character to check
		\return True if the character is a newline character, false otherwise.
	*/
	static bool				CharIsNewLine( char c );

	/*!
		\brief Checks if the given character is a tab character.
		\param c the character to check
		\return true if the character is a tab character, false otherwise
	*/
	static bool				CharIsTab( char c );

	/*!
		\brief Returns the color index from a given color value by masking the lower 4 bits

		This function extracts the color index from a color value by performing a bitwise AND operation with 15. This is commonly used in console rendering to determine the color attribute of text
	   characters. The color value is typically encoded with the color in the upper bits and the character in the lower bits, so this function isolates just the color portion.

		\param c The color value to extract the index from
		\return The color index extracted from the input color value, which is the lower 4 bits of the input
	*/
	static int				ColorIndex( int c );

	/*!
		\brief Returns the color vector corresponding to the given color index from the global color table

		This function accesses a global color table using a modulo operation on the input index to ensure it falls within the valid range of 0 to 15. It is used to retrieve predefined color values for
	   colored text rendering in the engine. The function is called from various parts of the rendering system where colored text output is needed, such as in console text rendering, SWF text
	   rendering, and general string drawing functions

		\param i The color index to retrieve, typically derived from a color escape sequence in text
		\return A reference to the idVec4 color value from the global color table corresponding to the masked index
	*/
	static idVec4&			ColorForIndex( int i );

	friend int				sprintf( idStr& dest, const char* fmt, ... );
	friend int				vsprintf( idStr& dest, const char* fmt, va_list ap );

	/*!
		\brief Reallocates the string data buffer to a new size, optionally preserving existing data.

		This function adjusts the allocated size of the string buffer to accommodate the specified amount of data. It ensures the new size is aligned to the allocation granularity defined by
	   STR_ALLOC_GRAN. If the keepold parameter is true and the current data exists, the existing content is copied to the new buffer before the old buffer is freed. The function handles memory
	   allocation using either a custom allocator or standard new/delete operators based on the USE_STRING_DATA_ALLOCATOR macro.

		\param amount The desired size of the new buffer, which will be adjusted to the nearest multiple of the allocation granularity
		\param keepold If true, copies the existing string data to the new buffer; if false, the new buffer is left uninitialized
	*/
	void					ReAllocate( int amount, bool keepold );

	/*!
		\brief Frees the dynamically allocated memory used by the string, unless it's using a static buffer.

		This function releases any dynamically allocated memory that the string object might be using. If the string is using a static buffer (indicated by the IsStatic() check), the function returns
	   immediately without freeing anything. Otherwise, it frees the memory pointed to by data using either the custom stringDataAllocator or the standard delete[] operator, and resets the data
	   pointer to point to the baseBuffer.

	*/
	void					FreeData();

	/*!
		\brief Formats a value using the most appropriate unit for the given measurement type.

		This function determines the best unit to represent a given value in a specified measurement type. It calculates the appropriate unit by comparing the value against powers of 1024 (1024^unit)
	   and selects the largest unit that keeps the value greater than or equal to 1. The formatted string is then appended to the current string object, followed by the unit suffix. The function
	   returns the index of the selected unit.

		\param format Formatting string to be used for the value
		\param value The numeric value to be formatted
		\param measure Type of measurement to determine the appropriate units
		\return The index of the best unit selected from the units array for the given measurement type
	*/
	int						BestUnit( const char* format, float value, Measure_t measure );

	/*!
		\brief Sets the string to the formatted value in the specified unit and measurement.

		This function takes a format string and a floating-point value, converts the value based on the specified unit and measurement, and formats it into the string. The value is scaled by a factor
	   of 2^(unit * 10) before formatting. The formatted value is then appended with a unit suffix from the units array based on the measurement type and unit.

		\param format The format string used to format the value
		\param value The floating-point value to be formatted
		\param unit The unit index used to scale the value
		\param measure The measurement type used to determine the unit suffix
	*/
	void					SetUnit( const char* format, float value, int unit, Measure_t measure );

	/*!
		\brief Initializes the memory allocator used for string data management.

		This function initializes the memory allocator that is used for managing string data when the USE_STRING_DATA_ALLOCATOR compile-time flag is enabled. It sets up the allocator to handle memory
	   allocation and deallocation for string objects, which can improve performance and reduce memory fragmentation in applications that frequently create and destroy strings.

	*/
	static void				InitMemory();

	/*!
		\brief Frees empty memory blocks from the string data allocator if the USE_STRING_DATA_ALLOCATOR macro is defined

		This function releases unused memory blocks that have been allocated by the string data allocator. It is only active when the USE_STRING_DATA_ALLOCATOR macro is defined. The function calls
	   FreeEmptyBaseBlocks on the global stringDataAllocator instance to perform the memory cleanup

	*/
	static void				ShutdownMemory();

	/*!
		\brief Frees empty memory blocks from the string data allocator if the USE_STRING_DATA_ALLOCATOR macro is defined.

		This function is used to release any unused memory blocks that have been allocated by the string data allocator. It is only active when the USE_STRING_DATA_ALLOCATOR macro is defined. The
	   function calls FreeEmptyBaseBlocks on the global stringDataAllocator instance to perform the memory cleanup.

	*/
	static void				PurgeMemory();

	/*!
		\brief Displays memory usage information for string data allocation

		This function outputs detailed memory statistics about the string data allocator when the USE_STRING_DATA_ALLOCATOR compile-time flag is enabled. It prints the total memory used by string
	   data, the amount of free memory available in blocks, and the number of empty base blocks currently in use

		\param args Command line arguments containing the command to execute
	*/
	static void				ShowMemoryUsage_f( const idCmdArgs& args );

	/*!
		\brief Returns the dynamically allocated memory used by this string instance

		This function determines the amount of memory that has been dynamically allocated for this string instance. If the string is using the base buffer (static buffer), it returns 0. Otherwise, it
	   returns the allocated memory size using the GetAlloced() method.

		\return The amount of dynamically allocated memory in bytes, or 0 if no dynamic memory is being used
	*/
	int						DynamicMemoryUsed() const;

	/*!
		\brief Formats an integer number into a string with a specific formatting pattern for memory display

		This function takes an integer value and formats it into a string representation that is suitable for displaying memory usage information. The function uses a predefined list of format units
	   (like kilobytes, megabytes, etc.) to break down the number into a more human-readable format. It processes the number by iterating through the format list and subtracting the largest applicable
	   unit value until the number is fully decomposed. The resulting string is then padded to a fixed width for consistent output formatting.

		\param number The integer number to be formatted
		\return A formatted string representation of the input number, suitable for displaying memory usage information
	*/
	static idStr			FormatNumber( int number );

	/*!
		\brief Splits a source string into a list of strings using the specified delimiter and optional group delimiter

		This function parses a source string and splits it into multiple substrings based on a delimiter character. It supports an optional group delimiter which allows for quoted segments to be
	   treated as single elements. The function returns true if the parsing was successful, and false if there was a mismatched group delimiter. When a group delimiter is encountered, the function
	   looks for the matching closing delimiter and treats everything between them as a single element. Regular delimiters separate elements at their positions. The parsed elements are appended to the
	   provided list, clearing any existing elements first.

		\param source The input string to be split
		\param list The list to store the resulting substrings
		\param delimiter The character used to separate elements in the source string
		\param groupDelimiter The character used to mark quoted segments that should be treated as single elements
		\return true if the string was successfully split, false if there was a mismatched group delimiter
	*/
	static bool				Split( const char* source, idList<idStr>& list, const char delimiter = ',', const char groupDelimiter = '\'' );

protected:
	int			   len;
	char*		   data;
	int			   allocedAndFlag; // top bit is used to store a flag that indicates if the string data is static or not
	char		   baseBuffer[STR_ALLOC_BASE];

	/*!
		\brief Ensures the string buffer has sufficient capacity to hold the specified amount of data.

		This function checks if the current buffer allocation is sufficient for the requested amount of data. If the buffer is too small, it reallocates the buffer to accommodate the new size. Static
	   strings cannot be reallocated and will assert if the requested size exceeds the current allocation. The optional keepold parameter determines whether to preserve the existing content during
	   reallocation.

		\param amount The minimum required buffer size
		\param keepold Whether to preserve existing content during reallocation
		\throws assertion if attempting to reallocate a static string with insufficient buffer size
	*/
	void		   EnsureAlloced( int amount, bool keepold = true );

	/*!
		\brief Sets the string data pointer to the specified buffer and initializes it as static.

		This function should only be called on a freshly constructed idStr object. It replaces the internal buffer with the provided buffer, marks the string as static, and initializes the length to
	   zero. The function asserts that the current data pointer matches the base buffer to ensure it's being called on a newly constructed object. This method is intended for use primarily in derived
	   class constructors.

		\param buffer The buffer to use as the string data pointer
		\param bufferLength The length of the provided buffer
		\throws assertion failure if the current data pointer does not match the base buffer
	*/
	ID_INLINE void SetStaticBuffer( char* buffer, const int bufferLength );

private:
	/*!
		\brief Initializes a string using the base buffer, intended for internal constructor use only.

		This function sets up the string object to use a base buffer for storage. It initializes the string as not static, allocates a base amount of memory, sets the data pointer to the base buffer,
	   and ensures the string is empty with a null terminator. Debug memory initialization is also handled when enabled.

	*/
	ID_INLINE void		Construct();

	static const uint32 STATIC_BIT	 = 31;
	static const uint32 STATIC_MASK	 = 1u << STATIC_BIT;
	static const uint32 ALLOCED_MASK = STATIC_MASK - 1;

	//! Returns the allocated size of the string buffer.
	ID_INLINE int		GetAlloced() const { return allocedAndFlag & ALLOCED_MASK; }

	/*!
		\brief Sets the allocated size of the string while preserving the static flag.

		This function modifies the internal allocedAndFlag member to update the allocated size of the string. It masks out the existing allocated size bits and replaces them with the new value
	   provided in parameter a, while preserving the static flag bits.

		\param a The new allocated size value to set
	*/
	ID_INLINE void		SetAlloced( const int a ) { allocedAndFlag = ( allocedAndFlag & STATIC_MASK ) | ( a & ALLOCED_MASK ); }

	//! Checks if the string is using static memory allocation.
	ID_INLINE bool		IsStatic() const { return ( allocedAndFlag & STATIC_MASK ) != 0; }

	/*!
		\brief Sets the static flag for the string object.

		This function modifies the internal flag state of the string object to indicate whether it should be treated as static. The static flag is stored in the allocedAndFlag member variable
	   alongside allocation information.

		\param isStatic The boolean value to set the static flag to
	*/
	ID_INLINE void		SetStatic( const bool isStatic ) { allocedAndFlag = ( allocedAndFlag & ALLOCED_MASK ) | ( isStatic << STATIC_BIT ); }

public:
	static const int INVALID_POSITION = -1;
};

char* va( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

/*
================================================================================================

	Sort routines for sorting idList<idStr>

================================================================================================
*/

class idSort_Str : public idSort_Quick<idStr, idSort_Str>
{
public:
	/*!
		\brief Compares two idStr objects case-insensitively and returns an integer indicating their relative order.

		This function performs a case-insensitive comparison between two idStr objects. It returns a negative value if the first string is lexicographically less than the second, zero if they are
	   equal, and a positive value if the first string is lexicographically greater than the second. The comparison is performed using the Icmp method of the idStr class.

		\param a First string to compare
		\param b Second string to compare
		\return Negative value if a is lexicographically less than b, zero if they are equal, positive value if a is lexicographically greater than b
	*/
	int Compare( const idStr& a, const idStr& b ) const { return a.Icmp( b ); }
};

class idSort_PathStr : public idSort_Quick<idStr, idSort_PathStr>
{
public:
	/*!
		\brief Compares two path strings using case-insensitive path comparison.

		This function performs a case-insensitive comparison of two path strings, taking into account path-specific formatting and conventions. It is used for sorting and comparing path strings in a
	   way that is consistent with how paths are typically handled in the engine.

		\param a First path string to compare
		\param b Second path string to compare
		\return Negative value if a is less than b, zero if a equals b, positive value if a is greater than b
	*/
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

ID_INLINE idStr operator+( const idStr& a, const idStr& b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE idStr operator+( const idStr& a, const char* b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE idStr operator+( const char* a, const idStr& b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE idStr operator+( const idStr& a, const bool b )
{
	idStr result( a );
	result.Append( b ? "true" : "false" );
	return result;
}

ID_INLINE idStr operator+( const idStr& a, const char b )
{
	idStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE idStr operator+( const idStr& a, const float b )
{
	char  text[64];
	idStr result( a );

	idStr::snPrintf( text, sizeof( text ), "%f", b );
	result.Append( text );

	return result;
}

ID_INLINE idStr operator+( const idStr& a, const int b )
{
	char  text[64];
	idStr result( a );

	idStr::snPrintf( text, sizeof( text ), "%d", b );
	result.Append( text );

	return result;
}

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

ID_INLINE bool operator==( const idStr& a, const idStr& b )
{
	return ( !idStr::Cmp( a.data, b.data ) );
}

ID_INLINE bool operator==( const idStr& a, const char* b )
{
	assert( b );
	return ( !idStr::Cmp( a.data, b ) );
}

ID_INLINE bool operator==( const char* a, const idStr& b )
{
	assert( a );
	return ( !idStr::Cmp( a, b.data ) );
}

ID_INLINE bool operator!=( const idStr& a, const idStr& b )
{
	return !( a == b );
}

ID_INLINE bool operator!=( const idStr& a, const char* b )
{
	return !( a == b );
}

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
