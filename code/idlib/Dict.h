/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans

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

#ifndef __DICT_H__
#define __DICT_H__

class idSerializer;

/*
===============================================================================

Key/value dictionary

This is a dictionary class that tracks an arbitrary number of key / value
pair combinations. It is used for map entity spawning, GUI state management,
and other things.

Keys are compared case-insensitive.

Does not allocate memory until the first key/value pair is added.

===============================================================================
*/

class idKeyValue
{
	friend class idDict;

public:
	//! Returns the key string stored in this key-value pair
	const idStr& GetKey() const { return *key; }

	//! Returns the value stored in this key-value pair
	const idStr& GetValue() const { return *value; }

	//! Returns the total allocated memory used by the key and value of this key-value pair.
	size_t		 Allocated() const { return key->Allocated() + value->Allocated(); }

	//! Returns the total memory size occupied by this key-value pair instance and its associated key and value.
	size_t		 Size() const { return sizeof( *this ) + key->Size() + value->Size(); }

	bool		 operator==( const idKeyValue& kv ) const { return ( key == kv.key && value == kv.value ); }

private:
	const idPoolStr* key;
	const idPoolStr* value;
};

/*
================================================
idSort_KeyValue
================================================
*/
class idSort_KeyValue : public idSort_Quick<idKeyValue, idSort_KeyValue>
{
public:
	/*!
		\brief Compares two idKeyValue objects based on their key strings.

		This function performs a lexicographic comparison of the key strings contained in two idKeyValue objects. It returns a negative value if the key of the first object is lexicographically
	   smaller than the key of the second object, zero if they are equal, and a positive value if the key of the first object is lexicographically greater than the key of the second object.

		\param a The first idKeyValue object to compare
		\param b The second idKeyValue object to compare
		\return A negative integer if a's key is lexicographically smaller than b's key, zero if they are equal, or a positive integer if a's key is lexicographically greater than b's key
	*/
	int Compare( const idKeyValue& a, const idKeyValue& b ) const { return a.GetKey().Icmp( b.GetKey() ); }
};

class idDict
{
public:
	/*!
		\brief Initializes a new instance of the idDict class with default settings for argument storage and hashing.

		The constructor initializes the internal data structures used to store arguments and their hash table. It sets the granularity for the argument list and hash table to 16, and clears the hash
	   table with an initial size of 128 and a granularity of 16.

	*/
	idDict();

	/*!
		\brief Constructs a new idDict object as a copy of an existing idDict object.

		This constructor creates a new idDict instance by copying all key-value pairs from another idDict object. The implementation uses the assignment operator to perform the copy operation,
	   ensuring that the new object has identical content to the source object. The constructor is marked as ID_INLINE, indicating it should be inlined for performance reasons.

		\param other The idDict object to copy key-value pairs from
	*/
	idDict( const idDict& other );

	/*!
		\brief Destructor for the idDict class that clears all contained data.

		This inline destructor cleans up all resources associated with the idDict instance by calling the Clear method. It ensures that any dynamically allocated memory or references held by the
	   dictionary are properly released when the object goes out of scope.

	*/
	~idDict();

	/*!
		\brief Sets the memory allocation granularity for the dictionary's internal data structures

		This function configures the memory allocation granularity for both the argument list and hash table used internally by the dictionary. The granularity determines how memory is allocated in
	   chunks, which can impact performance and memory usage patterns. Setting a higher granularity can reduce memory fragmentation but may increase memory overhead. The parameter controls the
	   allocation chunk size for internal memory management.

		\param granularity The granularity value for memory allocation in internal data structures
	*/
	void			  SetGranularity( int granularity );

	/*!
		\brief Sets the hash size for the dictionary's argument hash table.

		This function configures the hash size of the dictionary's internal hash table used for storing arguments. It only takes effect if the dictionary is currently empty. The hash table is cleared
	   and reinitialized with the specified size and a bucket count of 16.

		\param hashSize The new hash size to set for the dictionary's internal hash table
	*/
	void			  SetHashSize( int hashSize );

	// clear existing key/value pairs and copy all key/value pairs from other
	idDict&			  operator=( const idDict& other );

	/*!
		\brief Copies key-value pairs from another idDict instance into this instance

		This function copies all key-value pairs from the provided idDict object into the current instance. It handles the case where the current instance is the same as the source by performing no
	   operation. When copying, it first checks if keys already exist in the current instance and updates their values if they do. If a key doesn't exist, it adds a new key-value pair. The function
	   ensures proper memory management by using global string pools for key and value storage, and it handles self-copying scenarios correctly by setting new values before freeing old ones.

		\param other The source idDict instance from which to copy key-value pairs
	*/
	void			  Copy( const idDict& other );

	/*!
		\brief Transfers all key-value pairs from another dictionary to this dictionary, clearing the source dictionary.

		This function moves all key-value pairs from the provided dictionary to this dictionary. It first checks if the source and destination dictionaries are the same, and if so, returns early. It
	   also verifies that the key pool of the source dictionary is compatible with the global key pool to prevent issues across DLL boundaries. After validation, it clears the current dictionary and
	   copies all key-value pairs from the source dictionary. Finally, it clears the source dictionary to ensure no duplicate entries remain.

		\param other The source dictionary from which key-value pairs will be transferred
		\throws FatalError if attempting to transfer key-value pairs across a DLL boundary
	*/
	void			  TransferKeyValues( idDict& other );

	/*!
		\brief Parses a dictionary from the provided parser and returns true if no errors occurred during parsing.

		This function reads a dictionary from the given parser by expecting an opening brace, then parsing key-value pairs until a closing brace is encountered. Each key-value pair is validated to
	   ensure the key is a quoted string and the value is properly read. If a key is encountered that already exists in the dictionary, a warning is issued. The function returns false if any errors or
	   warnings occurred during parsing, otherwise it returns true.

		\param parser The parser object from which the dictionary is parsed.
		\return True if no errors occurred during parsing, false otherwise.
	*/
	bool			  Parse( idParser& parser );

	/*!
		\brief Sets default key-value pairs from another dictionary that are not already present in this dictionary.

		This function iterates through all key-value pairs in the provided dictionary and adds any pairs that do not already exist in this dictionary. It ensures that default values are populated only
	   when necessary, avoiding overwrites of existing values. The function uses global string copying mechanisms to manage memory for the key-value pairs.

		\param dict Pointer to the source dictionary containing default key-value pairs to be set.
	*/
	void			  SetDefaults( const idDict* dict );

	//! Clears all entries from the dictionary and frees associated memory.
	void			  Clear();

	/*!
		\brief Prints all key-value pairs stored in the dictionary to the common output stream

		This function iterates through all entries in the dictionary and prints each key-value pair in the format "key = value" followed by a newline character. The output is sent to the common
	   logging system using Printf

	*/
	void			  Print() const;

	//! Returns the total amount of memory allocated by the dictionary.
	size_t			  Allocated() const;

	//! Returns the total memory size occupied by this dictionary instance.
	size_t			  Size() const { return sizeof( *this ) + Allocated(); }

	/*!
		\brief Sets a key-value pair in the dictionary, updating the value if the key already exists or adding a new entry if it does not.

		This function updates the dictionary with the provided key-value pair. If the key already exists, the function replaces the existing value with the new one. If the key does not exist, it adds
	   a new key-value pair to the dictionary. The function handles memory management for the strings using global string pools.

		\param key The key to set or update in the dictionary
		\param value The value to associate with the key
	*/
	void			  Set( const char* key, const char* value );

	/*!
		\brief Sets a float value for the specified key in the dictionary

		This function stores a float value associated with a given key in the dictionary. The float value is converted to a string representation using the va function before being stored. This is
	   typically used to manage configuration values or game state data that needs to be persisted or retrieved by key.

		\param key The key to associate with the float value
		\param val The float value to store
	*/
	void			  SetFloat( const char* key, float val );

	/*!
		\brief Sets an integer value for a specified key in the dictionary.

		This function stores an integer value associated with a given key in the dictionary. The integer value is converted to a string representation before being stored. It is typically used in UI
	   systems where window variables need to be updated with integer data.

		\param key The key string used to identify the value in the dictionary
		\param val The integer value to be stored in the dictionary
	*/
	void			  SetInt( const char* key, int val );

	/*!
		\brief Sets a boolean value for the specified key in the dictionary

		This function stores a boolean value in the dictionary by converting it to a string representation. The boolean value is converted to an integer (0 or 1) and then formatted as a string before
	   being stored with the given key. This allows boolean values to be persisted and retrieved through the dictionary's string-based storage system

		\param key The key under which the boolean value will be stored
		\param val The boolean value to be stored
	*/
	void			  SetBool( const char* key, bool val );

	/*!
		\brief Sets a vector value in the dictionary using a string representation of the vector

		This function stores a vector value in the dictionary by converting the vector to its string representation. The vector is converted to a string using the ToString() method of idVec3, which
	   formats the vector components as "x y z" space-separated values. This allows the vector data to be stored and retrieved as a string key-value pair in the dictionary

		\param key The dictionary key used to store the vector value
		\param val The vector value to be stored in the dictionary
	*/
	void			  SetVector( const char* key, const idVec3& val );

	/*!
		\brief Sets a vector2 value in the dictionary using the specified key

		This function stores a vector2 value in the dictionary by converting the vector to a string representation and associating it with the provided key. The vector2 value is converted to a string
	   using its ToString() method, which typically formats the vector as "x y" or "x,y" depending on the implementation. This allows the vector2 data to be stored and retrieved as a string in the
	   dictionary, making it suitable for configuration or data persistence scenarios.

		\param key The key under which the vector2 value will be stored
		\param val The vector2 value to be stored in the dictionary
	*/
	void			  SetVec2( const char* key, const idVec2& val );

	/*!
		\brief Sets a vector value in the dictionary using a string representation of the vector.

		This function stores a vector value in the dictionary by converting the idVec4 to its string representation. The key is used to identify the entry in the dictionary, and the vector value is
	   converted to a string before being stored.

		\param key The key under which the vector value will be stored in the dictionary
		\param val The vector value to be stored in the dictionary
	*/
	void			  SetVec4( const char* key, const idVec4& val );

	/*!
		\brief Sets an angles value in the dictionary using the specified key

		This function stores an angles value in the dictionary by converting the idAngles object to a string representation and associating it with the provided key. The function is marked as inline
	   for performance optimization.

		\param key The key to associate with the angles value
		\param val The idAngles object containing the angles to store
	*/
	void			  SetAngles( const char* key, const idAngles& val );

	/*!
		\brief Sets a matrix value in the dictionary using a string representation of the matrix

		This function stores a matrix value in the dictionary by converting the matrix to its string representation. The matrix is converted using the ToString() method of idMat3, which provides a
	   standardized format for the matrix data. The key is used to identify the matrix value in the dictionary, and the resulting string representation is stored as the value associated with that key.

		\param key The key used to identify the matrix value in the dictionary
		\param val The matrix value to be stored in the dictionary
	*/
	void			  SetMatrix( const char* key, const idMat3& val );

	/*!
		\brief Returns the string value for a given key from the dictionary, or a default string if the key is not found

		This function retrieves the string value associated with the specified key in the dictionary. If the key is not found, it returns the provided default string value. The function is designed to
	   be efficient and inline for performance-critical operations.

		\param key The key to search for in the dictionary
		\param defaultString The default string to return if the key is not found
		\return The string value associated with the key, or the default string if the key is not found
	*/
	const char*		  GetString( const char* key, const char* defaultString = "" ) const;

	//! Retrieves a float value from the dictionary using the specified key, returning a default value if the key is not found.
	float			  GetFloat( const char* key, const char* defaultString ) const;

	//! Retrieves an integer value from the dictionary using the specified key, returning a default value if the key is not found.
	int				  GetInt( const char* key, const char* defaultString ) const;

	/*!
		\brief Retrieves a boolean value from the dictionary using the specified key, returning the default value if the key is not found.

		This function fetches a string value associated with the given key from the dictionary. It then converts this string to an integer using atoi, and returns true if the integer is non-zero,
	   false otherwise. The defaultString parameter is used when the key is not present in the dictionary.

		\param key The key to search for in the dictionary
		\param defaultString The default string value to return if the key is not found
		\return True if the value associated with the key (or the default value) is non-zero, false otherwise
	*/
	bool			  GetBool( const char* key, const char* defaultString ) const;

	/*!
		\brief Retrieves a float value associated with the given key from the dictionary, returning a default value if the key is not found.

		This function searches for a key-value pair in the dictionary using the provided key. If the key is found, it converts the associated string value to a float using atof and returns it. If the
	   key is not found, it returns the specified default float value. This function is commonly used to extract numeric parameters from configuration dictionaries, with error handling typically
	   implemented by checking the return value against a boolean condition.

		\param key The key to search for in the dictionary.
		\param defaultFloat The float value to return if the key is not found.
		\return The float value associated with the key if found, otherwise the default float value.
	*/
	float			  GetFloat( const char* key, const float defaultFloat = 0.0f ) const;

	/*!
		\brief Retrieves an integer value from the dictionary for the given key, returning a default value if the key is not found.

		This function searches for a key in the dictionary and attempts to convert the associated value to an integer using atoi. If the key is not found, it returns the specified default integer
	   value. This is a utility function commonly used to extract integer properties from configuration or entity definitions.

		\param key The key to search for in the dictionary.
		\param defaultInt The default integer value to return if the key is not found.
		\return The integer value associated with the key if found, otherwise returns the default integer value.
	*/
	int				  GetInt( const char* key, const int defaultInt = 0 ) const;

	/*!
		\brief Retrieves a boolean value from the dictionary using the specified key, returning a default value if the key is not found.

		This function searches for a key in the dictionary and attempts to convert its associated value to a boolean. If the key is not found, it returns the provided default boolean value. The
	   conversion is performed by interpreting the value as a string and using atoi to convert it to an integer, then checking if that integer is non-zero.

		\param key The key to look for in the dictionary
		\param defaultBool The default boolean value to return if the key is not found
		\return The boolean value associated with the key, or the default value if the key is not found
	*/
	bool			  GetBool( const char* key, const bool defaultBool = false ) const;

	/*!
		\brief Retrieves a vector value from the dictionary using the specified key, returning a default value if the key is not found

		This function attempts to retrieve a vector value associated with the given key from the dictionary. If the key is not found, it uses the provided default string to construct a default vector
	   value. The function is designed to be used with a default string that represents a vector in the format "x y z". The retrieved vector is stored in a local variable which is then returned to the
	   caller. This inline function delegates the actual work to the non-inline GetVector method that takes an output parameter.

		\param key The key used to look up the vector value in the dictionary
		\param defaultString The default string to use if the key is not found, expected to contain three space-separated floating point values representing x, y, and z components
		\return A vector value retrieved from the dictionary or constructed from the default string if the key is not found
	*/
	idVec3			  GetVector( const char* key, const char* defaultString = NULL ) const;

	/*!
		\brief Retrieves a 2D vector value from the dictionary using the specified key, returning a default value if the key is not found.

		This function retrieves a 2D vector value associated with the given key from the dictionary. If the key does not exist in the dictionary, it uses the provided default string to construct a
	   fallback vector. The function delegates to the non-inline version of GetVec2 that takes an output parameter. This function is typically used in UI systems where vector values are stored as
	   string representations and need to be parsed into idVec2 objects for further processing.

		\param key The key used to look up the vector value in the dictionary
		\param defaultString The default string to use if the key is not found, which will be parsed to create a fallback vector
		\return The 2D vector value associated with the key, or a default vector constructed from the defaultString if the key is not found
	*/
	idVec2			  GetVec2( const char* key, const char* defaultString = NULL ) const;

	/*!
		\brief Returns a vec4 value from the dictionary with an optional default value

		This function retrieves a vec4 value associated with the specified key from the dictionary. If the key is not found, it uses the provided default string to construct a default vec4 value. The
	   function internally calls GetVec4 with a reference to an output vec4 variable to perform the actual retrieval and conversion

		\param key The key to look up in the dictionary
		\param defaultString The default string to use if the key is not found, can be NULL
		\return The vec4 value associated with the key, or a default vec4 constructed from the default string if the key is not found
	*/
	idVec4			  GetVec4( const char* key, const char* defaultString = NULL ) const;

	/*!
		\brief Retrieves an idAngles object from the dictionary using the specified key, with an optional default value.

		This function fetches an idAngles value associated with the given key from the dictionary. If the key is not found, it uses the provided default string to construct a default idAngles object.
	   The function internally calls GetAngles with a reference to an idAngles object to populate the result.

		\param key The key to look up in the dictionary.
		\param defaultString The default string to use if the key is not found.
		\return The idAngles object retrieved from the dictionary or constructed from the default string if the key is not found.
	*/
	idAngles		  GetAngles( const char* key, const char* defaultString = NULL ) const;

	/*!
		\brief Retrieves a 3x3 matrix from the dictionary using the specified key, returning a default matrix if the key is not found.

		This function fetches a 3x3 matrix from the dictionary by looking up the provided key. If the key does not exist, it uses the provided default string to construct a default matrix. The
	   function is designed to handle matrix data that can be specified in various formats, including as a list of nine floating point values. It delegates the actual retrieval and parsing to the
	   GetMatrix method that takes a reference to an idMat3 object.

		\param key The key to look up in the dictionary for the matrix data.
		\param defaultString The default string used to construct a default matrix if the key is not found.
		\return A 3x3 matrix retrieved from the dictionary or a default matrix constructed from the default string.
	*/
	idMat3			  GetMatrix( const char* key, const char* defaultString = NULL ) const;

	/*!
		\brief Retrieves a string value from the dictionary by key, returning a default value if the key is not found.

		This function searches for a key in the dictionary and if found, assigns the corresponding string value to the output pointer and returns true. If the key is not found, it assigns the default
	   string value to the output pointer and returns false. The function is designed to be inline for performance reasons and is typically used to safely retrieve configuration values or other string
	   data from a dictionary structure.

		\param key The key to search for in the dictionary
		\param defaultString The default string value to return if the key is not found
		\param out Pointer to a pointer where the retrieved string value will be stored
		\return True if the key was found and a value was retrieved, false otherwise.
	*/
	bool			  GetString( const char* key, const char* defaultString, const char** out ) const;

	/*!
		\brief Retrieves a string value from the dictionary using the specified key, returning false if the key is not found.

		The function attempts to find a string value associated with the provided key in the dictionary. If the key is found, it copies the string value to the output parameter and returns true. If
	   the key is not found, it copies the default string value to the output parameter and returns false. This is commonly used to read configuration values or entity properties from a dictionary.

		\param key The key to search for in the dictionary
		\param defaultString The default string value to use if the key is not found
		\param out The string value retrieved from the dictionary or copied from the default string
		\return True if the key was found in the dictionary, false otherwise
	*/
	bool			  GetString( const char* key, const char* defaultString, idStr& out ) const;

	/*!
		\brief Retrieves a float value from the dictionary using the specified key, returning false if the key is not found.

		The function attempts to find a string value associated with the provided key in the dictionary. If the key is found, it converts the string value to a float and stores it in the output
	   parameter. If the key is not found, it uses the default string value provided and converts that to a float. The function returns true if the key was found, and false otherwise. This is commonly
	   used to read configuration values or entity properties from a dictionary.

		\param key The key to search for in the dictionary
		\param defaultString The default string value to use if the key is not found
		\param out The float value retrieved from the dictionary or converted from the default string
		\return True if the key was found in the dictionary, false otherwise
	*/
	bool			  GetFloat( const char* key, const char* defaultString, float& out ) const;

	/*!
		\brief Retrieves an integer value from the dictionary using the specified key, returning false if the key is not found.

		The function attempts to retrieve a string value associated with the given key from the dictionary. If the key is found, the string value is converted to an integer and stored in the output
	   parameter. If the key is not found, the default string value is used instead. The function returns true if the key was found, and false otherwise.

		\param key The key used to look up the value in the dictionary
		\param defaultString The default string value to use if the key is not found
		\param out Reference to an integer where the retrieved value will be stored
		\return True if the key was found in the dictionary, false otherwise.
	*/
	bool			  GetInt( const char* key, const char* defaultString, int& out ) const;

	/*!
		\brief Retrieves a boolean value from the dictionary based on the specified key, using a default string if the key is not found.

		This function attempts to retrieve a string value associated with the given key from the dictionary. If the key is found, it converts the string to a boolean value using atoi, where any
	   non-zero value is treated as true. If the key is not found, it uses the default string to determine the boolean value and returns false to indicate the key was not present. The function
	   modifies the out parameter to contain the retrieved boolean value.

		\param key The key to look up in the dictionary
		\param defaultString The default string to use if the key is not found
		\param out Reference to store the resulting boolean value
		\return True if the key was found in the dictionary, false otherwise.
	*/
	bool			  GetBool( const char* key, const char* defaultString, bool& out ) const;

	/*!
		\brief Retrieves a float value from the dictionary by key, returning a default value if the key is not found.

		This function searches for a key in the dictionary and attempts to convert its associated value to a float. If the key is found, the float value is stored in the output parameter and the
	   function returns true. If the key is not found, the default float value is stored in the output parameter and the function returns false. The function is typically used to safely extract
	   configuration values or properties from a dictionary, with error handling through the return value and a provided default.

		\param key The key to search for in the dictionary
		\param defaultFloat The default float value to use if the key is not found
		\param out Reference to a float variable where the result will be stored
		\return True if the key was found and the value was successfully converted to a float, false otherwise
	*/
	bool			  GetFloat( const char* key, const float defaultFloat, float& out ) const;

	/*!
		\brief Retrieves an integer value from the dictionary by key, returning true if found or false if the key is missing.

		The function attempts to find a key-value pair in the dictionary using the provided key. If the key exists, the value is converted from a string to an integer using atoi and stored in the
	   output parameter. If the key does not exist, the default integer value is assigned to the output parameter and the function returns false. This function is commonly used to safely extract
	   integer values from configuration or entity definitions, with error handling typically implemented by checking the return value.

		\param key The key to search for in the dictionary
		\param defaultInt The default integer value to use if the key is not found
		\param out Reference to an integer where the retrieved value will be stored
		\return True if the key was found and the value was successfully converted to an integer, false otherwise.
	*/
	bool			  GetInt( const char* key, const int defaultInt, int& out ) const;

	/*!
		\brief Retrieves a boolean value from the dictionary based on the provided key, using a default value if the key is not found.

		This function searches for a key in the dictionary and attempts to convert its associated value to a boolean. If the key is found, the boolean representation of the value is stored in the
	   output parameter and the function returns true. If the key is not found, the default boolean value is stored in the output parameter and the function returns false. The conversion is performed
	   using atoi to parse the value string, treating any non-zero result as true.

		\param key The key to search for in the dictionary
		\param defaultBool The default boolean value to use if the key is not found
		\param out Output parameter to store the retrieved boolean value
		\return True if the key was found and the value was successfully converted to a boolean, false otherwise.
	*/
	bool			  GetBool( const char* key, const bool defaultBool, bool& out ) const;

	/*!
		\brief Parses a vector from the dictionary using the specified key and default string, storing the result in the output parameter

		This function retrieves a vector value from the dictionary by parsing a string representation of three floating-point numbers separated by spaces. It first checks if the key exists in the
	   dictionary, using the provided default string if the key is missing. The parsed vector components are stored in the output idVec3 parameter. The function returns true if the key was found in
	   the dictionary, or false if the default value was used

		\param key The key to look up in the dictionary
		\param defaultString The default string to use if the key is not found, defaults to '0 0 0' if null
		\param out The output parameter where the parsed vector will be stored
		\return True if the key was found in the dictionary, false if the default value was used
	*/
	bool			  GetVector( const char* key, const char* defaultString, idVec3& out ) const;

	/*!
		\brief Retrieves a 2D vector value from the dictionary using the specified key, returning true if found.

		This function looks up a key in the dictionary and parses its value as a 2D vector. If the key is not found, it uses the provided default string to initialize the vector. The default string is
	   expected to be in the format "x y" where x and y are floating-point numbers. The function returns true if the key was found in the dictionary, and false otherwise.

		\param key The key to look up in the dictionary
		\param defaultString The default string to use if the key is not found, defaults to "0 0" if null
		\param out Reference to the idVec2 object where the parsed vector will be stored
		\return True if the key was found in the dictionary, false otherwise
	*/
	bool			  GetVec2( const char* key, const char* defaultString, idVec2& out ) const;

	/*!
		\brief Retrieves a vec4 value from the dictionary by key, using a default string if the key is not found

		This function attempts to retrieve a vec4 value associated with the specified key from the dictionary. If the key is not found, it uses the provided default string to parse the vec4 value. The
	   parsed vec4 value is stored in the output parameter. The function returns true if the key was found, false otherwise.

		\param key The key to look up in the dictionary
		\param defaultString The default string to use if the key is not found, or null to use a default of "0 0 0 0"
		\param out The vec4 value parsed from the dictionary or default string
		\return true if the key was found in the dictionary, false otherwise
	*/
	bool			  GetVec4( const char* key, const char* defaultString, idVec4& out ) const;

	/*!
		\brief Retrieves angle values from the dictionary using a key, parses them into an idAngles object, and returns a boolean indicating success.

		This function looks up a string value associated with the specified key in the dictionary. If the key is not found, it uses the provided default string to initialize the angles. The string is
	   expected to contain three space-separated floating-point values representing pitch, yaw, and roll angles. These values are parsed and stored in the provided idAngles object. The function
	   returns true if the key was found in the dictionary, and false otherwise.

		\param key The key used to look up the string value in the dictionary
		\param defaultString The default string to use if the key is not found, must not be null
		\param out The idAngles object where the parsed angle values will be stored
		\return True if the key was found in the dictionary, false otherwise
	*/
	bool			  GetAngles( const char* key, const char* defaultString, idAngles& out ) const;

	/*!
		\brief Retrieves a 3x3 matrix from the dictionary using the specified key, or uses a default string if the key is not found.

		This function attempts to find a matrix value associated with the given key in the dictionary. If the key is not found, it falls back to using the provided default string. The matrix is parsed
	   from the string and stored in the output parameter. The function returns true if the key was found, false otherwise. The default string is expected to contain nine floating-point values in
	   row-major order representing the matrix elements.

		\param key The key to search for in the dictionary
		\param defaultString The default string to use if the key is not found, or null to use a default identity matrix
		\param out The output matrix to store the parsed values
		\return True if the key was found and the matrix was successfully parsed, false otherwise.
	*/
	bool			  GetMatrix( const char* key, const char* defaultString, idMat3& out ) const;

	//! Returns the number of key-value pairs stored in the dictionary.
	int				  GetNumKeyVals() const;

	/*!
		\brief Returns a pointer to the key-value pair at the specified index in the dictionary

		This function retrieves a key-value pair from the dictionary by its index position. It performs a bounds check to ensure the index is valid before returning a pointer to the key-value pair. If
	   the index is out of bounds, it returns NULL instead.

		\param index The zero-based index of the key-value pair to retrieve
		\return A pointer to the key-value pair at the specified index, or NULL if the index is invalid
	*/
	const idKeyValue* GetKeyVal( int index ) const;

	/*!
		\brief Returns the key/value pair with the given key from the dictionary

		This function searches for a key/value pair in the dictionary that matches the provided key. It performs a hash-based lookup for efficient searching. If the key is not found or if the key
	   parameter is NULL or empty, it returns NULL. The function also issues a warning if an empty key is provided. The search is case-insensitive as indicated by the use of Icmp method.

		\param key The key to search for in the dictionary
		\return A pointer to the key/value pair if found, or NULL if the key does not exist or if the input key is invalid
	*/
	const idKeyValue* FindKey( const char* key ) const;

	/*!
		\brief Returns the index of the key/value pair with the specified key, or -1 if not found.

		The function searches for a key in the dictionary and returns its index in the internal array. If the key is not found, it returns -1. The function handles null or empty keys by issuing a
	   warning and returning 0. The search uses a hash table for efficient lookup.

		\param key The key to search for in the dictionary
		\return The index of the key/value pair if found, otherwise -1
	*/
	int				  FindKeyIndex( const char* key ) const;

	/*!
		\brief Removes the key/value pair with the specified key from the dictionary

		This function searches for a key/value pair in the dictionary that matches the provided key and removes it from both the array of arguments and the hash index. The function iterates through
	   the hash table entries for the given key and removes the first matching entry. If no matching key is found, the function performs no operation. The implementation uses a hash table for
	   efficient lookup and removal of key/value pairs

		\param key The key of the key/value pair to be removed from the dictionary
	*/
	void			  Delete( const char* key );

	/*!
		\brief Removes all key/value pairs from the dictionary where the value is an empty string.

		This function iterates through all key/value pairs in the dictionary and deletes those entries where the value is an empty string. It creates a copy of the original list of key/value pairs to
	   avoid issues with iteration while modification. The function is useful for cleaning up dictionary entries that have no meaningful data associated with them.

	*/
	void			  DeleteEmptyKeys();

	/*!
		\brief Finds the next key/value pair in the dictionary with a key that starts with the given prefix.

		This function searches through the dictionary entries to find the next key/value pair whose key starts with the specified prefix. It can be used to iterate through all matching entries by
	   passing the previously found entry as the lastMatch parameter. The search begins after the lastMatch entry or from the start of the dictionary if lastMatch is NULL. The function returns a
	   pointer to the found key/value pair or NULL if no match is found.

		\param prefix The prefix to match at the beginning of key names
		\param lastMatch Optional pointer to a previously found match to continue searching from
		\return Pointer to the found key/value pair, or NULL if no matching entry is found
		\throws assertion error if prefix is NULL
	*/
	const idKeyValue* MatchPrefix( const char* prefix, const idKeyValue* lastMatch = NULL ) const;

	/*!
		\brief Returns a random value from the key/value pairs that match the given prefix.

		This function iterates through all key/value pairs in the dictionary that start with the specified prefix and randomly selects one of the values to return. It uses the provided random number
	   generator to determine which matching value to return.

		\param prefix The prefix to match keys against
		\param random The random number generator used to select which matching value to return
		\return A pointer to the randomly selected value string from the matching key/value pairs
	*/
	const char*		  RandomPrefix( const char* prefix, idRandom& random ) const;

	/*!
		\brief Writes the dictionary contents to a file handle

		This function serializes the dictionary entries to the provided file handle. It first writes the total number of entries in the dictionary, then iterates through each entry and writes both the
	   key and value strings to the file. The count is written in little-endian format to ensure compatibility across different platforms.

		\param f File handle to write the dictionary contents to
	*/
	void			  WriteToFileHandle( idFile* f ) const;

	/*!
		\brief Reads key-value pairs from a file handle into the dictionary

		This function reads a collection of key-value pairs from the provided file handle and populates the dictionary with them. It first reads the count of pairs, then iterates that many times to
	   read each key and value string, and finally sets them in the dictionary. The file handle is assumed to be positioned at the start of the data to be read

		\param f The file handle to read the key-value pairs from
	*/
	void			  ReadFromFileHandle( idFile* f );

	/*!
		\brief Writes the dictionary contents as a JSON formatted string to the provided file

		This function outputs the key-value pairs stored in the dictionary in JSON format to the specified file. It iterates through all arguments in the dictionary and writes each key-value pair as a
	   JSON entry. The prefix parameter allows for indentation or additional text at the beginning of each line. Tab characters in keys are replaced with spaces to ensure valid JSON output. Trailing
	   commas are omitted from the last entry to maintain correct JSON syntax

		\param f the file object to write the JSON data to
		\param prefix optional prefix string for indentation or additional text at the start of each line
	*/
	void			  WriteJSON( idFile* f, const char* prefix = "" ) const;

	/*!
		\brief Writes the dictionary contents to an INI file format with sorted keys and prefixed sections

		This function writes the key-value pairs stored in the dictionary to a file using an INI file format. The keys are sorted alphabetically, and keys containing forward slashes are grouped under
	   section headers. Keys without forward slashes are written at the beginning of the file. Keys with forward slashes are written under their respective section headers, where the part before the
	   last slash represents the section name. The function ensures proper formatting and uses C-style quoting for values to handle special characters.

		\param f File pointer to write the INI formatted data to
	*/
	void			  WriteToIniFile( idFile* f ) const;

	/*!
		\brief Populates the dictionary with key-value pairs from an INI file format.

		This function reads an entire INI-formatted file into memory and parses it using a lexer. It supports sections denoted by square brackets and key-value assignments using the equals sign. The
	   function handles nested keys by prefixing them with the current section name. The parsing process will return false if the file cannot be read completely or if there are syntax errors. Each
	   key-value pair is added to the dictionary, with duplicate keys generating a warning but not preventing parsing from continuing.

		\param f Pointer to the file object containing the INI data to be parsed.
		\return True if the file was successfully parsed and all key-value pairs were added to the dictionary; returns false if the file could not be read or if parsing encountered an error.
	*/
	bool			  ReadFromIniFile( idFile* f );

	/*!
		\brief Serializes the dictionary's key-value pairs using the provided serializer

		This function handles the serialization of an idDict object, which contains key-value pairs. When reading from the serializer, it clears the current dictionary contents and reads the key-value
	   pairs from the serializer. When writing to the serializer, it serializes the existing key-value pairs. The function manages the number of entries and properly handles both reading and writing
	   operations through the serializer interface

		\param ser Serializer object used for reading from or writing to the data stream
	*/
	void			  Serialize( idSerializer& ser );

	/*!
		\brief Returns a unique checksum for the dictionary's content

		This function calculates a checksum for the dictionary by first sorting all key-value pairs, then applying the CRC32 algorithm to the concatenated keys and values. The sorting ensures that the
	   checksum is deterministic regardless of the order in which the key-value pairs were added to the dictionary. The function uses a standard CRC32 implementation with initialization, updating, and
	   finalization steps.

		\return An integer representing the computed checksum for the dictionary's content
	*/
	int				  Checksum() const;

	/*!
		\brief Initializes the global key and value containers for the dictionary

		Initializes the globalKeys and globalValues containers used by the dictionary. The globalKeys container is set to be case insensitive while the globalValues container is set to be case
	   sensitive. This method is typically called during the initialization phase of the idDict class to prepare the internal data structures for use.

	*/
	static void		  Init();

	/*!
		\brief Clears all global keys and values stored in the idDict instance

		This function is responsible for cleaning up the internal storage of global keys and values maintained by the idDict class. It clears both the globalKeys and globalValues collections,
	   effectively resetting the dictionary state. The function is typically called during system shutdown to ensure proper cleanup of resources associated with the dictionary. Based on the call
	   examples, this function appears to be part of the broader shutdown sequence in the engine framework, where various subsystems are properly cleaned up before the application terminates.

	*/
	static void		  Shutdown();

	/*!
		\brief Prints memory usage statistics for keys and values stored in the global dictionary.

		This function outputs the amount of memory used by the global keys and values in kilobytes, along with the respective counts of keys and values. It uses the globalKeys and globalValues
	   collections to calculate and display this information.

		\param args Command line arguments, though not used in this implementation.
	*/
	static void		  ShowMemoryUsage_f( const idCmdArgs& args );

	/*!
		\brief Prints a message indicating that the function is not implemented due to sorting implementation issues.

		This function is intended to list all keys from a global set of keys, sorted alphabetically, and print them to the console. However, it is currently not implemented due to complications with
	   the sorting implementation. The function outputs a message indicating the reason for the lack of implementation and includes commented-out code that shows the intended approach for sorting and
	   printing the keys.

		\param args Command line arguments passed to the function.
	*/
	static void		  ListKeys_f( const idCmdArgs& args );

	/*!
		\brief Lists values from the dictionary and prints them to the console

		This function is intended to list all values stored in the globalValues collection and print them to the console. However, the implementation is currently not functional due to sorting
	   implementation issues. The code contains commented-out logic that would gather values, sort them, and then print the sorted list along with a count of total values. The function currently
	   outputs a message indicating that the functionality is not implemented.

		\param args Command line arguments containing the command being executed
	*/
	static void		  ListValues_f( const idCmdArgs& args );

private:
	idList<idKeyValue> args;
	idHashIndex		   argHash;

	static idStrPool   globalKeys;
	static idStrPool   globalValues;
};

ID_INLINE idDict::idDict()
{
	args.SetGranularity( 16 );
	argHash.SetGranularity( 16 );
	argHash.Clear( 128, 16 );
}

ID_INLINE idDict::idDict( const idDict& other )
{
	*this = other;
}

ID_INLINE idDict::~idDict()
{
	Clear();
}

ID_INLINE void idDict::SetGranularity( int granularity )
{
	args.SetGranularity( granularity );
	argHash.SetGranularity( granularity );
}

ID_INLINE void idDict::SetHashSize( int hashSize )
{
	if( args.Num() == 0 ) { argHash.Clear( hashSize, 16 ); }
}

ID_INLINE void idDict::SetFloat( const char* key, float val )
{
	Set( key, va( "%f", val ) );
}

ID_INLINE void idDict::SetInt( const char* key, int val )
{
	Set( key, va( "%i", val ) );
}

ID_INLINE void idDict::SetBool( const char* key, bool val )
{
	Set( key, va( "%i", val ) );
}

ID_INLINE void idDict::SetVector( const char* key, const idVec3& val )
{
	Set( key, val.ToString() );
}

ID_INLINE void idDict::SetVec4( const char* key, const idVec4& val )
{
	Set( key, val.ToString() );
}

ID_INLINE void idDict::SetVec2( const char* key, const idVec2& val )
{
	Set( key, val.ToString() );
}

ID_INLINE void idDict::SetAngles( const char* key, const idAngles& val )
{
	Set( key, val.ToString() );
}

ID_INLINE void idDict::SetMatrix( const char* key, const idMat3& val )
{
	Set( key, val.ToString() );
}

ID_INLINE bool idDict::GetString( const char* key, const char* defaultString, const char** out ) const
{
	const idKeyValue* kv = FindKey( key );
	if( kv ) {
		*out = kv->GetValue();
		return true;
	}
	*out = defaultString;
	return false;
}

ID_INLINE bool idDict::GetString( const char* key, const char* defaultString, idStr& out ) const
{
	const idKeyValue* kv = FindKey( key );
	if( kv ) {
		out = kv->GetValue();
		return true;
	}
	out = defaultString;
	return false;
}

ID_INLINE const char* idDict::GetString( const char* key, const char* defaultString ) const
{
	const idKeyValue* kv = FindKey( key );
	if( kv ) { return kv->GetValue(); }
	return defaultString;
}

ID_INLINE float idDict::GetFloat( const char* key, const char* defaultString ) const
{
	return atof( GetString( key, defaultString ) );
}

ID_INLINE int idDict::GetInt( const char* key, const char* defaultString ) const
{
	return atoi( GetString( key, defaultString ) );
}

ID_INLINE bool idDict::GetBool( const char* key, const char* defaultString ) const
{
	return ( atoi( GetString( key, defaultString ) ) != 0 );
}

ID_INLINE float idDict::GetFloat( const char* key, const float defaultFloat ) const
{
	const idKeyValue* kv = FindKey( key );
	if( kv ) { return atof( kv->GetValue() ); }
	return defaultFloat;
}

ID_INLINE int idDict::GetInt( const char* key, int defaultInt ) const
{
	const idKeyValue* kv = FindKey( key );
	if( kv ) { return atoi( kv->GetValue() ); }
	return defaultInt;
}

ID_INLINE bool idDict::GetBool( const char* key, const bool defaultBool ) const
{
	const idKeyValue* kv = FindKey( key );
	if( kv ) { return atoi( kv->GetValue() ) != 0; }
	return defaultBool;
}

ID_INLINE idVec3 idDict::GetVector( const char* key, const char* defaultString ) const
{
	idVec3 out;
	GetVector( key, defaultString, out );
	return out;
}

ID_INLINE idVec2 idDict::GetVec2( const char* key, const char* defaultString ) const
{
	idVec2 out;
	GetVec2( key, defaultString, out );
	return out;
}

ID_INLINE idVec4 idDict::GetVec4( const char* key, const char* defaultString ) const
{
	idVec4 out;
	GetVec4( key, defaultString, out );
	return out;
}

ID_INLINE idAngles idDict::GetAngles( const char* key, const char* defaultString ) const
{
	idAngles out;
	GetAngles( key, defaultString, out );
	return out;
}

ID_INLINE idMat3 idDict::GetMatrix( const char* key, const char* defaultString ) const
{
	idMat3 out;
	GetMatrix( key, defaultString, out );
	return out;
}

ID_INLINE int idDict::GetNumKeyVals() const
{
	return args.Num();
}

ID_INLINE const idKeyValue* idDict::GetKeyVal( int index ) const
{
	if( index >= 0 && index < args.Num() ) { return &args[index]; }
	return NULL;
}

#endif /* !__DICT_H__ */
