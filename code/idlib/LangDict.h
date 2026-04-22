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
	\brief A key-value pair container for language data.
*/
class idLangKeyValue
{
public:
	//! Initializes a new instance with null key and value pointers.
	idLangKeyValue() :
		key( NULL ),
		value( NULL )
	{
	}

	//! Constructs an idLangKeyValue object with the specified key and value.
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
	\brief A dictionary class for managing localized string resources.

	The idLangDict class provides a key-value storage mechanism specifically designed for handling localized strings in a language dictionary. It supports loading and saving dictionary data from/to
   buffers and files, and offers methods for adding, retrieving, updating, and deleting string entries. The class maintains a collection of key-value pairs where keys are used to reference localized
   values, and includes functionality to handle string redirections with depth tracking to prevent infinite loops. It also supports string ID validation and provides mechanisms for efficiently
   accessing dictionary entries by index or key.

*/
class idLangDict
{
public:
	static const char* KEY_PREFIX;
	static const int   KEY_PREFIX_LEN;
	static const int   MAX_REDIRECTION_DEPTH = 2;

	//! Constructs a new idLangDict instance.
	idLangDict();

	//! Destroys the language dictionary and clears all its contents.
	~idLangDict();

	//! Clears all key-value pairs from the language dictionary and frees their allocated memory.
	void				  Clear();

	//! Loads language dictionary data from a buffer
	bool				  Load( const byte* buffer, const int bufferLen, const char* name );

	//! Saves the language dictionary to a file with UTF-8 BOM and formatted key-value pairs
	bool				  Save( const char* fileName );

	//! Returns the localized string for the given input string, or the input string itself if no localization is found
	const char*			  GetString( const char* str ) const;

	//! Returns the translated string for the given input string, or NULL if not found.
	const char*			  FindString( const char* str ) const;

	//! Returns a randomly generated key for the given string.
	const char*			  AddString( const char* str );

	//! Deletes a string from the dictionary using the provided key and returns false if the key doesn't exist.
	bool				  DeleteString( const char* key );

	//! Renames a string key in the language dictionary, returning true if successful.
	bool				  RenameStringKey( const char* oldKey, const char* newKey );

	//! Sets the value for a given key in the language dictionary and returns false if the key doesn't exist
	bool				  SetString( const char* key, const char* val );

	//! Adds a key-value pair to the dictionary if it does not already exist.
	void				  AddKeyVal( const char* key, const char* val );

	//! Returns the number of key-value entries in the language dictionary.
	int					  GetNumKeyVals() const;

	//! Returns a pointer to the key-value pair at the specified index in the language dictionary.
	const idLangKeyValue* GetKeyVal( int i ) const;

	//! Removes a string entry from the dictionary at the specified index.
	bool				  DeleteString( const int idx );

	//! Returns the localized string for the given string ID, or the ID itself if not found
	const char*			  GetLocalizedString( const idStrId& strId ) const;

	//! Returns true if the input string starts with the KEY_PREFIX string.
	static bool			  IsStringId( const char* str );

private:
	idDynamicBlockAlloc<char, 100 * 1024, 16> blockAlloc;
	idList<idLangKeyValue>					  keyVals;
	idHashIndex								  keyIndex;

private:
	//! Returns the index of the given string in the language dictionary, or -1 if not found.
	int			FindStringIndex( const char* str ) const;

	//! Recursively finds and returns the resolved string value for a given string identifier, tracking redirection depth to prevent infinite loops.
	const char* FindString_r( const char* str, int& depth ) const;

	friend class idStrId;
};

/*!
	\class idLocalization
	\brief Manages localization dictionaries and provides string translation services.
*/
class idLocalization
{
public:
	//! Returns the localized version of the input string, or the input string itself if no localization is found
	static const char*	  GetString( const char* inString );

	//! Returns the localized version of the input string or NULL if not found.
	static const char*	  FindString( const char* inString );

	//! Clears all entries from the language dictionary.
	static void			  ClearDictionary();

	//! Loads a localization dictionary from a byte buffer.
	static bool			  LoadDictionary( const byte* buffer, const int bufferLen, const char* name );

	//! Returns the language dictionary used for localization.
	static idLangDict&	  GetDictionary() { return languageDict; }

	//! Verifies that a buffer contains valid UTF-8 or ASCII text and returns the detected encoding type.
	static utf8Encoding_t VerifyUTF8( const uint8* buffer, const int bufferLen, const char* name );

private:
	static idLangDict languageDict;
	friend class idStrId;
};

/*!
	\class idStrId
	\brief Represents a localized string identifier with index-based lookup capabilities.

	The idStrId class serves as a wrapper for localized string identifiers, using an index to reference entries in a language dictionary. It provides constructors for initialization from various
   sources including integer indices, string keys, and other idStrId objects. The class supports assignment operations, equality comparisons, and methods to retrieve the associated key string or
   localized value. It can be explicitly set to an invalid state using the Empty method, and provides access to its internal index value. The design enables efficient lookup and management of
   localized strings within a system that maintains a dictionary of string identifiers.

*/
class idStrId
{
public:
	//! Constructs a default idStrId object with an invalid index.
	idStrId() :
		index( -1 )
	{
	}

	//! Copies the index value from another idStrId instance
	idStrId( const idStrId& other ) :
		index( other.index )
	{
	}

	//! Initializes an idStrId object with the given integer index.
	explicit idStrId( int i ) :
		index( i )
	{
	}

	//! Constructs an idStrId object by setting its key to the provided string.
	explicit idStrId( const char* key ) { Set( key ); }

	//! Constructs an idStrId object by setting it to the value of the provided idStr key.
	explicit idStrId( const idStr& key ) { Set( key ); }

	//! Assigns a string key to this idStrId object.
	void		operator=( const char* key ) { Set( key ); }

	//! Assigns the value of a string to this string identifier.
	void		operator=( const idStr& key ) { Set( key ); }

	//! Assigns the index value from another idStrId object to this object.
	void		operator=( const idStrId& other ) { index = other.index; }

	//! Compares two idStrId objects for equality based on their index values
	bool		operator==( const idStrId& other ) const { return index == other.index; }

	//! Compares this idStrId object with another for inequality.
	bool		operator!=( const idStrId& other ) const { return index != other.index; }

	//! Sets the string ID index from a key string, or resets to invalid if the key is null or empty.
	void		Set( const char* key );

	//! Sets the index member variable to -1.
	void		Empty() { index = -1; }

	//! Returns true if the idStrId object represents an empty string.
	bool		IsEmpty() const { return index < 0; }

	//! Returns the key string stored in the language dictionary at the index specified by this idStrId object
	const char* GetKey() const;

	//! Returns the localized string for this idStrId identifier.
	const char* GetLocalizedString() const;

	//! Returns the index value stored in the idStrId object.
	int			GetIndex() const { return index; }

	//! Sets the index value of this idStrId object to the specified integer.
	void		SetIndex( int i ) { index = i; }

private:
	int index; // Index into the language dictionary
};

#endif // !__LANGDICT_H__
