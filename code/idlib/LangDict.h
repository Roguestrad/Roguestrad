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
#ifndef __LANGDICT_H__
#define __LANGDICT_H__

/*!
	\class idLangKeyValue
	\brief A simple key-value pair container for language data.
*/
class idLangKeyValue
{
public:
	/*!
		\brief Initializes a new instance of the idLangKeyValue class with null key and value pointers.

		The constructor initializes the key and value member variables to NULL, setting up the object in a valid but empty state. This is typically used when creating instances of the idLangKeyValue
	   class that will later be populated with actual key-value pairs.

	*/
	idLangKeyValue() :
		key( NULL ),
		value( NULL )
	{
	}

	/*!
		\brief Constructs an idLangKeyValue object with the specified key and value strings.

		The constructor initializes the key and value members of the idLangKeyValue object using the provided character pointers. It performs a direct assignment of the input pointers to the member
	   variables, assuming they point to valid null-terminated strings. The constructor does not perform any memory management or copying of the input strings, so the caller must ensure the input
	   strings remain valid for the lifetime of the object.

		\param k Pointer to the key string for the key-value pair
		\param v Pointer to the value string for the key-value pair
	*/
	idLangKeyValue( char* k, char* v ) :
		key( k ),
		value( v )
	{
	}
	char* key;
	char* value;
};

class idStrId;

/*!
	\class idLangDict
	\brief A dictionary class for managing language string lookups and localization within the Doom 3 BFG engine.

	The idLangDict class provides a comprehensive system for storing and retrieving localized strings used in the engine's internationalization framework. It supports loading dictionary data from
   buffers, saving to files, and performing key-based lookups with various methods for finding, adding, and updating string entries. The class uses a hash table implementation for efficient string
   lookup operations and handles UTF-8 encoding for international characters. It supports both direct string lookup and recursive resolution of string identifiers to handle complex localization
   scenarios. The class maintains an internal key-value storage system where each string is associated with a unique key for fast retrieval and management. Memory management is handled through
   internal allocators, and the class provides methods for clearing all entries, deleting specific entries, and updating existing entries. The implementation includes safeguards against infinite
   recursion during string resolution and proper handling of string identifiers that begin with a specific prefix used by the engine.

*/
class idLangDict
{
public:
	static const char* KEY_PREFIX;
	static const int   KEY_PREFIX_LEN;
	static const int   MAX_REDIRECTION_DEPTH = 2;

	/*!
		\brief Initializes a new instance of the idLangDict class with a key index capacity of 4096.

		The constructor initializes the idLangDict object by setting up the keyIndex member with a capacity of 4096 entries. This dictionary is used for language string lookups within the Doom 3 BFG
	   engine.

	*/
	idLangDict();

	/*!
		\brief Destructor for idLangDict that clears all dictionary entries.

		This destructor cleans up all resources associated with the idLangDict instance by calling the Clear method. It ensures that any dynamically allocated memory or resources used by the
	   dictionary are properly released when the object goes out of scope.

	*/
	~idLangDict();

	//! Clears all key-value pairs in the language dictionary.
	void				  Clear();

	/*!
		\brief Loads language dictionary data from a buffer, parsing key-value pairs and handling UTF-8 encoding.

		This function reads language data from a provided buffer, parsing key-value pairs formatted as quoted strings. It handles both UTF-8 and ASCII encoding, checking for proper byte-order marks in
	   debug builds and skipping them in release builds. Comments in the file are skipped, and escape sequences in values are processed. The function tracks line numbers and reports the number of
	   strings read. It returns true on successful parsing, or false if the buffer is invalid or parsing fails.

		\param buffer Pointer to the buffer containing the language data to parse
		\param bufferLen Length of the buffer in bytes
		\param name Name of the file or resource being loaded, used for logging and error reporting
		\return True if the buffer was successfully parsed and loaded into the language dictionary, false otherwise
		\throws FatalError if the file ends unexpectedly or UTF-8 encoding is invalid in debug builds
	*/
	bool				  Load( const byte* buffer, const int bufferLen, const char* name );

	/*!
		\brief Saves the language dictionary to a file with UTF-8 BOM and formatted key-value pairs

		Writes the language dictionary contents to the specified file with a UTF-8 byte order mark, followed by a formatted key-value list enclosed in curly braces. The function handles special
	   character escaping for tabs, newlines, quotes, and backslashes in the values. Returns false if the file cannot be opened for writing, and true upon successful completion.

		\param fileName The path to the file where the language dictionary will be saved
		\return True if the file was successfully written, false otherwise
	*/
	bool				  Save( const char* fileName );

	/*!
		\brief Returns the localized string for the given string key, or the original string if no localization is found

		This function attempts to find a localized version of the provided string key within the language dictionary. If a localized string is found, it is returned; otherwise, the original string is
	   returned as a fallback. This behavior ensures that untranslated strings are still displayed properly while allowing for full internationalization support.

		\param str The string key to look up in the language dictionary
		\return The localized string if found, otherwise the original input string
	*/
	const char*			  GetString( const char* str ) const;

	/*!
		\brief Returns a pointer to the localized string for the given input string, or NULL if not found.

		This function searches for a localized version of the input string within the language dictionary. It uses a recursive helper function to perform the actual search. The function returns NULL
	   when the input string is not found in the dictionary.

		\param str The input string to search for in the language dictionary
		\return A pointer to the localized string if found, or NULL if the string is not present in the dictionary
		\throws NULL is returned when the string is not found, but no exception is thrown
	*/
	const char*			  FindString( const char* str ) const;

	/*!
		\brief Adds a string to the dictionary with a generated key and returns the key.

		The function generates a unique key for the given string using a timestamp-based approach to ensure uniqueness. It handles potential key collisions by incrementing the timestamp until a unique
	   key is found. The string is then added to the dictionary with this generated key, and the key is returned for later use.

		\param str The string to be added to the dictionary
		\return The generated key associated with the added string in the dictionary
	*/
	const char*			  AddString( const char* str );

	//! Deletes a string from the language dictionary by key and returns true if successful.
	bool				  DeleteString( const char* key );

	/*!
		\brief Renames a string key in the language dictionary from oldKey to newKey

		This function searches for the specified oldKey in the language dictionary and replaces it with the newKey. The function returns false if the oldKey is not found, and true if the renaming
	   operation is successful. The function handles memory allocation and deallocation for the new key string, and updates the hash index if the hash values of the old and new keys differ

		\param oldKey The current key string to be renamed
		\param newKey The new key string to replace the old key
		\return true if the key was successfully renamed, false if the oldKey was not found in the dictionary
	*/
	bool				  RenameStringKey( const char* oldKey, const char* newKey );

	/*!
		\brief Sets the value for a given key in the language dictionary and returns true on success

		This function updates the value associated with the specified key in the language dictionary. It first searches for the key using FindStringIndex to get the index. If the key is not found, it
	   returns false. Otherwise, it frees the existing memory allocated for the value and allocates new memory to store the updated value. The function returns true upon successful update of the
	   key-value pair

		\param key The key whose value needs to be updated
		\param val The new value to be associated with the key
		\return True if the key was found and the value was successfully updated, false otherwise
	*/
	bool				  SetString( const char* key, const char* val );

	/*!
		\brief Adds a key-value pair to the dictionary if it does not already exist, otherwise updates the existing entry.

		This function attempts to set a key-value pair in the dictionary. If the key already exists, it updates the value. If the key does not exist, it adds the new key-value pair to the dictionary.
	   The function uses a block allocator to manage memory for the key and value strings.

		\param key The key to add or update in the dictionary
		\param val The value to associate with the key
	*/
	void				  AddKeyVal( const char* key, const char* val );

	//! Returns the number of key-value pairs stored in the language dictionary.
	int					  GetNumKeyVals() const;

	/*!
		\brief Retrieves a key-value pair from the dictionary by index.

		This function returns a pointer to the key-value pair at the specified index in the dictionary. The index must be within the valid range of the dictionary's key-value pairs, otherwise the
	   behavior is undefined. The returned pointer points to an internal structure and should not be modified directly.

		\param i The zero-based index of the key-value pair to retrieve.
		\return A pointer to the key-value pair at the specified index, or nullptr if the index is out of bounds.
	*/
	const idLangKeyValue* GetKeyVal( int i ) const;

	/*!
		\brief Deletes a string from the language dictionary at the specified index

		This function removes a string value from the language dictionary by freeing the memory allocated for it and setting the value pointer to NULL. It performs bounds checking to ensure the index
	   is valid before attempting deletion. The function uses a block allocator to free the memory associated with the string value.

		\param idx The index of the string to delete from the language dictionary
		\return True if the string was successfully deleted, false if the index was out of bounds
	*/
	bool				  DeleteString( const int idx );

	/*!
		\brief Returns the localized string value for the given string ID, or the ID itself if no localization is found.

		This function retrieves the localized string value associated with the provided string ID. It first checks if the ID's index is valid within the internal key-value storage. If the index is
	   valid and the value is set, it returns the localized string. If the value is not set, it returns the key (ID) itself. If the index is out of bounds, it returns an empty string.

		\param strId The string ID for which to retrieve the localized string.
		\return A pointer to the localized string, the original string ID if no localization is found, or an empty string if the ID is invalid.
	*/
	const char*			  GetLocalizedString( const idStrId& strId ) const;

	/*!
		\brief Checks if a string starts with the key prefix used for string identifiers.

		This function determines whether the provided string begins with the predefined key prefix that is used to identify string identifiers within the language dictionary system. It performs a
	   case-insensitive comparison of the string prefix to verify this condition.

		\param str The string to check for the key prefix
		\return True if the string starts with the key prefix, false otherwise
	*/
	static bool			  IsStringId( const char* str );

private:
	idDynamicBlockAlloc<char, 100 * 1024, 16> blockAlloc;
	idList<idLangKeyValue>					  keyVals;
	idHashIndex								  keyIndex;

private:
	/*!
		\brief Finds the index of a string in the dictionary

		This function searches for a given string in the dictionary and returns its index if found. It uses a hash table approach to efficiently locate the string. If the input string is NULL, the
	   function returns -1. The function performs a case-insensitive comparison when checking for string equality.

		\param str The string to search for in the dictionary
		\return The index of the string in the dictionary if found, otherwise -1
	*/
	int			FindStringIndex( const char* str ) const;

	/*!
		\brief Recursively finds and returns the resolved string value for a given string key, tracking redirection depth to prevent infinite loops.

		This function performs a recursive lookup of a string key in the language dictionary. It tracks the depth of redirections to prevent infinite loops. If the maximum redirection depth is
	   exceeded, it issues a warning and returns NULL. The function handles cases where the input string is NULL or empty, returning NULL in such cases. If a string key is found but has no associated
	   value, it also returns NULL. If the value is itself a string identifier, the function recursively resolves it until a final string value is obtained.

		\param str The string key to look up in the dictionary
		\param depth Reference to an integer that tracks the current recursion depth to prevent infinite loops
		\return The resolved string value corresponding to the input key, or NULL if the key is not found, the value is NULL, or the maximum redirection depth is exceeded.
		\throws Warning is issued via idLib::Warning if the maximum redirection depth is exceeded.
	*/
	const char* FindString_r( const char* str, int& depth ) const;

	friend class idStrId;
};

/*!
	\class idLocalization
	\brief Manages localization dictionaries and string translation for the engine.

	The idLocalization class provides functionality for loading, managing, and retrieving localized strings within the engine. It maintains a global language dictionary that stores key-value pairs of
   original and localized strings. The class supports loading dictionary data from buffers, clearing the dictionary, and verifying UTF-8 encoding of localization files. Game code should typically use
   GetString() to retrieve localized strings, while tools and development environments can access the global dictionary through GetDictionary(). The class is designed to support multiple languages
   through its dictionary-based approach and includes validation mechanisms to ensure proper UTF-8 encoding of localization files.

*/
class idLocalization
{
public:
	//! Returns the localized string for the given input string, or the input string itself if no localization is found.
	static const char*	  GetString( const char* inString );

	//! Returns the localized string for the given input string, or NULL if not found.
	static const char*	  FindString( const char* inString );

	/*!
		\brief Clears all entries from the language dictionary.

		This function removes all key-value pairs from the language dictionary, effectively resetting it to an empty state. It is typically used when preparing to load a new language file or when
	   cleaning up localization data.

	*/
	static void			  ClearDictionary();

	//! Loads a dictionary from the provided buffer data.
	static bool			  LoadDictionary( const byte* buffer, const int bufferLen, const char* name );

	/*!
		\brief Returns a reference to the global language dictionary used for localization.

		This function provides access to the static language dictionary that stores all localized strings. It is primarily intended for use in tools and development environments, as normal game code
	   should use GetString() instead to retrieve localized text. The returned reference points to a static member variable that is initialized during program startup.

		\return A reference to the global idLangDict object containing all localized strings
	*/
	static idLangDict&	  GetDictionary() { return languageDict; }

	/*!
		\brief Verifies that a buffer contains valid UTF-8 encoded text and returns the detected encoding type.

		This function checks if the provided buffer contains valid UTF-8 encoded text or plain ASCII. It uses the IsValidUTF8 function to determine the encoding type and performs various checks to
	   ensure the file is properly encoded. If the buffer fails validation, the function will cause a fatal error with a descriptive message indicating the specific encoding problem.

		\param buffer pointer to the buffer containing the text to verify
		\param bufferLen length of the buffer in bytes
		\param name name of the file being validated for logging purposes
		\return The detected UTF-8 encoding type (UTF8_ENCODED_BOM, UTF8_PURE_ASCII, etc.) if validation passes
		\throws Fatal error if the buffer contains invalid UTF-8 encoding, invalid BOM, no BOM, or unknown encoding type
	*/
	static utf8Encoding_t VerifyUTF8( const uint8* buffer, const int bufferLen, const char* name );

private:
	static idLangDict languageDict;
	friend class idStrId;
};

/*!
	\class idStrId
	\brief A string identifier wrapper that manages localized string lookups using integer indices.

	The idStrId class serves as a wrapper for string identifiers used in localized text systems within the engine. It maintains an internal index that maps to a string in a language dictionary,
   allowing efficient retrieval of localized strings without repeatedly parsing or hashing string keys. The class supports construction from various identifier types including raw integers, string
   keys, and other idStrId objects. It provides mechanisms to set and retrieve string keys, localized strings, and internal indices, with special handling for invalid states where the index is set to
   -1. The class is designed for use in dialog systems and other localized content areas where consistent string lookup is required. The implementation includes validation to prevent unauthorized
   additions to the string table, ensuring only predefined strings can be referenced.

*/
class idStrId
{
public:
	/*!
		\brief Constructs a default idStrId object with an invalid index.

		This constructor initializes an idStrId object with an index set to -1, indicating that the object does not reference a valid string identifier. This is typically used as a default or invalid
	   state for idStrId objects, which are commonly used in the Doom 3 BFG engine for managing localized string identifiers. The object can later be assigned a valid identifier through assignment or
	   other initialization methods.

	*/
	idStrId() :
		index( -1 )
	{
	}

	/*!
		\brief Copy constructs a new idStrId object from another idStrId object

		This is a copy constructor for the idStrId class that initializes a new instance with the same index value as the provided source object. The class appears to be a string identifier wrapper
	   that maintains an index for lookup in a string table or similar data structure. This constructor is commonly used when creating copies of string identifier objects, such as when passing them by
	   value or storing them in containers.

		\param other The idStrId object to copy the index value from
	*/
	idStrId( const idStrId& other ) :
		index( other.index )
	{
	}

	/*!
		\brief Constructs an idStrId object with the specified integer index.

		This is an explicit constructor for the idStrId class that initializes the index member variable with the provided integer value. The constructor is marked as explicit to prevent implicit
	   conversions from int to idStrId.

		\param i The integer index to initialize the idStrId object with
	*/
	explicit idStrId( int i ) :
		index( i )
	{
	}

	/*!
		\brief Constructs an idStrId object by setting its internal key to the provided string identifier

		This constructor initializes an idStrId object using a string key. The key is typically a localized string identifier that starts with a hash symbol, such as "#STR_SWF_ACCEPT". The constructor
	   internally calls the Set method to assign the key value to the object's internal storage. This is commonly used when working with dialog systems and localized strings in the game framework

		\param key String identifier that typically starts with a '#' symbol, used as a key for localized strings
	*/
	explicit idStrId( const char* key ) { Set( key ); }

	/*!
		\brief Constructs an idStrId object by setting it to the provided key string

		This constructor initializes an idStrId object using the provided idStr key. It internally calls the Set method to assign the key value. The idStrId class appears to be a string identifier
	   that supports localized string retrieval, as evidenced by usage in dialog systems where localized strings are fetched using the GetLocalizedString method. The constructor is marked as explicit
	   to prevent unintended implicit conversions.

		\param key The idStr key used to initialize the idStrId object
	*/
	explicit idStrId( const idStr& key ) { Set( key ); }

	void		operator=( const char* key ) { Set( key ); }
	void		operator=( const idStr& key ) { Set( key ); }
	void		operator=( const idStrId& other ) { index = other.index; }

	bool		operator==( const idStrId& other ) const { return index == other.index; }
	bool		operator!=( const idStrId& other ) const { return index != other.index; }

	/*!
		\brief Sets the string ID index based on the provided key string, or marks it as invalid if the key is null or empty.

		This function assigns an index to the string ID by looking up the provided key in the language dictionary. If the key is null or empty, the index is set to -1 to indicate an invalid state. If
	   the key is not found in the dictionary, a warning is issued to prevent unauthorized additions to the string table. This ensures that only known string IDs can be set, maintaining consistency
	   with the defined language resources.

		\param key The key string used to look up the index in the language dictionary.
	*/
	void		Set( const char* key );

	/*!
		\brief Initializes the idStrId object by setting its index to -1.

		This function resets the internal state of an idStrId object by setting its index member variable to -1. This effectively clears any previous string identifier that might have been stored in
	   the object. The idStrId class is typically used for efficient string handling in the engine, and this function serves to reset its state to an uninitialized condition.

	*/
	void		Empty() { index = -1; }

	//! Returns true if the interaction is empty
	bool		IsEmpty() const { return index < 0; }

	//! Returns the key string associated with this idStrId object
	const char* GetKey() const;

	//! Returns the localized string representation of this idStrId object
	const char* GetLocalizedString() const;

	//! Returns the index value stored in the idStrId object.
	int			GetIndex() const { return index; }

	/*!
		\brief Sets the index value for this idStrId object

		This function assigns the provided integer value to the internal index member of the idStrId class. It is a simple setter function that updates the index state of the object. Based on the
	   context from BoundsTrack.h, this appears to be part of a bounds tracking system where indices are used to manage and identify different bounding volumes. The index value is typically used in
	   conjunction with other functions like FindIntersections to determine spatial relationships between bounding volumes.

		\param i The integer index value to assign to this object
	*/
	void		SetIndex( int i ) { index = i; }

private:
	int index; // Index into the language dictionary
};

#endif // !__LANGDICT_H__
