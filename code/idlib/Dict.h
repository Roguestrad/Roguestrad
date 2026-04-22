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

/*!
	\class idKeyValue
	\brief Manages a key-value pair with string data.
*/
class idKeyValue
{
	friend class idDict;

public:
	//! Returns the key string stored in this key-value pair
	const idStr& GetKey() const { return *key; }

	//! Returns the value string stored in this key-value pair.
	const idStr& GetValue() const { return *value; }

	//! Returns the total memory allocated for the key and value of this key-value pair.
	size_t		 Allocated() const { return key->Allocated() + value->Allocated(); }

	//! Returns the total memory size occupied by this key-value pair and its associated key and value objects.
	size_t		 Size() const { return sizeof( *this ) + key->Size() + value->Size(); }

	//! Compares two idKeyValue objects for equality based on their key and value members.
	bool		 operator==( const idKeyValue& kv ) const { return ( key == kv.key && value == kv.value ); }

private:
	const idPoolStr* key;
	const idPoolStr* value;
};

/*!
	\class idSort_KeyValue
	\brief Sorts idKeyValue objects by their keys.
*/
class idSort_KeyValue : public idSort_Quick<idKeyValue, idSort_KeyValue>
{
public:
	//! Compares two idKeyValue objects based on their keys.
	int Compare( const idKeyValue& a, const idKeyValue& b ) const { return a.GetKey().Icmp( b.GetKey() ); }
};

/*!
	\class idDict
	\brief A dictionary class for storing and managing key-value pairs of various data types.

	The idDict class provides a flexible storage mechanism for key-value pairs with support for multiple data types including strings, integers, floats, booleans, vectors, angles, and matrices. It
   offers both direct value setting and retrieval methods, as well as parsing capabilities from external sources like parsers and file handles. The class supports memory management through granular
   allocation settings and provides serialization and checksum functionality for data integrity. It also includes utility methods for copying, transferring, and manipulating dictionary contents, along
   with debugging features such as memory usage reporting. The implementation handles internal memory allocation and deallocation automatically, making it suitable for managing large collections of
   data with efficient access patterns.

*/
class idDict
{
public:
	//! Initializes a new instance of the idDict class with default settings.
	idDict();

	//! Constructs a new dictionary by copying the contents of another dictionary.
	idDict( const idDict& other );

	//! Destroys the idDict object and clears all contained data.
	~idDict();

	//! Sets the memory allocation granularity for the dictionary's internal data structures.
	void			  SetGranularity( int granularity );

	//! Sets the hash size for the dictionary.
	void			  SetHashSize( int hashSize );

	//! Assigns all key/value pairs from another idDict to this idDict
	idDict&			  operator=( const idDict& other );

	//! Copies key-value pairs from another idDict instance, preserving existing entries.
	void			  Copy( const idDict& other );

	//! Transfers all key/value pairs from this dictionary to another dictionary and clears the original dictionary.
	void			  TransferKeyValues( idDict& other );

	//! Parses a dictionary from the provided parser object
	bool			  Parse( idParser& parser );

	//! Copies key/value pairs from another dictionary that are not already present in this dictionary.
	void			  SetDefaults( const idDict* dict );

	//! Clears the dictionary and frees all associated memory
	void			  Clear();

	//! Prints all key-value pairs in the dictionary to the common output.
	void			  Print() const;

	//! Returns the total amount of memory allocated by the dictionary.
	size_t			  Allocated() const;

	//! Returns the total memory size occupied by this dictionary instance
	size_t			  Size() const { return sizeof( *this ) + Allocated(); }

	//! Sets a key-value pair in the dictionary, replacing any existing value for the key.
	void			  Set( const char* key, const char* value );

	//! Sets a float value in the dictionary with the specified key.
	void			  SetFloat( const char* key, float val );

	//! Sets an integer value for the specified key in the dictionary
	void			  SetInt( const char* key, int val );

	//! Sets a boolean value for the specified key in the dictionary.
	void			  SetBool( const char* key, bool val );

	//! Sets a vector value in the dictionary using a string representation
	void			  SetVector( const char* key, const idVec3& val );

	//! Sets a vector2 value in the dictionary using a string representation.
	void			  SetVec2( const char* key, const idVec2& val );

	//! Sets a vector value in the dictionary using a string representation.
	void			  SetVec4( const char* key, const idVec4& val );

	//! Sets an angles value in the dictionary using the specified key.
	void			  SetAngles( const char* key, const idAngles& val );

	//! Sets a matrix value in the dictionary using the specified key.
	void			  SetMatrix( const char* key, const idMat3& val );

	//! Returns the string value for the given key from the dictionary, or the default string if the key is not found
	const char*		  GetString( const char* key, const char* defaultString = "" ) const;

	//! Retrieves a floating-point value for the specified key from the dictionary, returning a default value if the key is not found.
	float			  GetFloat( const char* key, const char* defaultString ) const;

	//! Returns the integer value of a key from the dictionary, using a default string if the key is not found.
	int				  GetInt( const char* key, const char* defaultString ) const;

	//! Returns the boolean value of a key from the dictionary, using a default string if the key is not found.
	bool			  GetBool( const char* key, const char* defaultString ) const;

	//! Retrieves a float value from the dictionary by key, returning a default if the key is not found.
	float			  GetFloat( const char* key, const float defaultFloat = 0.0f ) const;

	//! Retrieves an integer value from the dictionary using the specified key, returning a default value if the key is not found.
	int				  GetInt( const char* key, const int defaultInt = 0 ) const;

	//! Retrieves a boolean value from the dictionary using the specified key, returning a default value if the key is not found.
	bool			  GetBool( const char* key, const bool defaultBool = false ) const;

	//! Retrieves a vector value from the dictionary using the specified key and optional default string
	idVec3			  GetVector( const char* key, const char* defaultString = NULL ) const;

	//! Retrieves a 2D vector value from the dictionary using the specified key, returning a default value if the key is not found.
	idVec2			  GetVec2( const char* key, const char* defaultString = NULL ) const;

	//! Retrieves a vector4 value from the dictionary using the specified key, returning a default value if the key is not found.
	idVec4			  GetVec4( const char* key, const char* defaultString = NULL ) const;

	//! Retrieves an idAngles object from the dictionary using the specified key and optional default string.
	idAngles		  GetAngles( const char* key, const char* defaultString = NULL ) const;

	//! Retrieves a 3x3 matrix from the dictionary by key, using a default value if the key is not found.
	idMat3			  GetMatrix( const char* key, const char* defaultString = NULL ) const;

	//! Retrieves a string value from the dictionary by key, returning true if found or false if the default value is used.
	bool			  GetString( const char* key, const char* defaultString, const char** out ) const;

	//! Retrieves a string value from the dictionary by key, returning true if found or false if the default value is used.
	bool			  GetString( const char* key, const char* defaultString, idStr& out ) const;

	//! Retrieves a floating-point value from the dictionary by key, returning false if the key is not found.
	bool			  GetFloat( const char* key, const char* defaultString, float& out ) const;

	//! Retrieves an integer value from the dictionary by key, using a default string if the key is not found.
	bool			  GetInt( const char* key, const char* defaultString, int& out ) const;

	//! Retrieves a boolean value from the dictionary using the specified key, returning false if the key is not found.
	bool			  GetBool( const char* key, const char* defaultString, bool& out ) const;

	//! Retrieves a float value from the dictionary by key, returning false if the key is not found.
	bool			  GetFloat( const char* key, const float defaultFloat, float& out ) const;

	//! Retrieves an integer value from the dictionary by key, returning false if the key is not found.
	bool			  GetInt( const char* key, const int defaultInt, int& out ) const;

	//! Retrieves a boolean value from the dictionary by key, returning true if the key exists and false otherwise.
	bool			  GetBool( const char* key, const bool defaultBool, bool& out ) const;

	//! Retrieves a vector value from the dictionary by key, using a default string if the key is not found.
	bool			  GetVector( const char* key, const char* defaultString, idVec3& out ) const;

	//! Retrieves a 2D vector from the dictionary using the specified key, returning false if the key is not found.
	bool			  GetVec2( const char* key, const char* defaultString, idVec2& out ) const;

	//! Retrieves a vec4 value from the dictionary by key, using a default string if the key is not found.
	bool			  GetVec4( const char* key, const char* defaultString, idVec4& out ) const;

	//! Retrieves angular values from the dictionary and parses them into an idAngles object.
	bool			  GetAngles( const char* key, const char* defaultString, idAngles& out ) const;

	//! Retrieves a 3x3 matrix from the dictionary by key, using a default string if the key is not found
	bool			  GetMatrix( const char* key, const char* defaultString, idMat3& out ) const;

	//! Returns the number of key-value pairs stored in the dictionary.
	int				  GetNumKeyVals() const;

	//! Retrieves a key-value pair from the dictionary by index.
	const idKeyValue* GetKeyVal( int index ) const;

	//! Returns the key/value pair with the given key or NULL if it does not exist
	const idKeyValue* FindKey( const char* key ) const;

	//! Returns the index of the key/value pair with the specified key, or -1 if not found.
	int				  FindKeyIndex( const char* key ) const;

	//! Deletes the key/value pair with the specified key from the dictionary.
	void			  Delete( const char* key );

	//! Removes all key-value pairs from the dictionary where the value is an empty string.
	void			  DeleteEmptyKeys();

	//! Finds the next key/value pair with the given key prefix, optionally starting after a previous match.
	const idKeyValue* MatchPrefix( const char* prefix, const idKeyValue* lastMatch = NULL ) const;

	//! Returns a random value from the key/value pairs that match the given prefix.
	const char*		  RandomPrefix( const char* prefix, idRandom& random ) const;

	//! Writes the dictionary contents to a file handle.
	void			  WriteToFileHandle( idFile* f ) const;

	//! Reads key-value pairs from a file handle into the dictionary.
	void			  ReadFromFileHandle( idFile* f );

	//! Writes the dictionary contents as JSON to the specified file with an optional prefix.
	void			  WriteJSON( idFile* f, const char* prefix = "" ) const;

	//! Writes the dictionary contents to an INI file format with prefixed key-value pairs.
	void			  WriteToIniFile( idFile* f ) const;

	//! Reads dictionary data from an INI file format.
	bool			  ReadFromIniFile( idFile* f );

	//! Serializes the dictionary data using the provided serializer
	void			  Serialize( idSerializer& ser );

	//! Returns a unique checksum for the dictionary's content
	int				  Checksum() const;

	//! Initializes the global key and value collections for the dictionary.
	static void		  Init();

	//! Clears all global keys and values stored in the idDict instance.
	static void		  Shutdown();

	//! Prints memory usage statistics for dictionary keys and values.
	static void		  ShowMemoryUsage_f( const idCmdArgs& args );

	//! Prints a message indicating that the function is not implemented due to sorting implementation issues.
	static void		  ListKeys_f( const idCmdArgs& args );

	//! Lists values from a dictionary, although this functionality is currently not implemented due to sorting implementation issues.
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
