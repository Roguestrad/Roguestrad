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

#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

/*!
	\class idHashNodeT
	\brief Template class for hash table nodes used in hash table implementations.

	The idHashNodeT class serves as a generic hash table node template that can store key-value pairs and maintain linked list chains for collision resolution. It is designed to be used as a building
   block for hash table data structures, providing functionality for node creation, hash computation, and key comparison. The class is templated on key and value types, allowing it to work with any
   data types that support the required operations. Each node maintains a pointer to the next node in its bucket's collision chain, enabling efficient storage and retrieval of key-value pairs in hash
   tables. The static methods provide essential hash table operations including hash index calculation based on a mask and key comparison functions, making this class suitable for integration into
   various hash table implementations within the engine's data structures.

*/
template<typename _key_, class _value_>
class idHashNodeT
{
public:
	/*!
		\brief Default constructor for the idHashNodeT hash table node class.

		Initializes a new hash table node with no next node in the chain. The node is created in an empty state and can be used as the head of a hash bucket chain.

	*/
	idHashNodeT() :
		next( NULL )
	{
	}

	/*!
		\brief Constructs a hash node with the specified key, value, and next node pointer.

		This constructor initializes a hash node used in hash tables. It takes a key and value pair along with a pointer to the next node in the hash chain. The node is initialized with the provided
	   key and value, and the next pointer is set to the given node. This is typically used when inserting elements into a hash table data structure.

		\param key The key to store in the hash node
		\param value The value to store in the hash node
		\param next Pointer to the next node in the hash chain
	*/
	idHashNodeT( const _key_& key, const _value_& value, idHashNodeT* next ) :
		key( key ),
		value( value ),
		next( next )
	{
	}

	/*!
		\brief Computes the hash index for a given key based on the table mask

		This function calculates the hash index by performing a bitwise AND operation between the key value and the table mask. This is a common technique in hash table implementations to map keys to
	   array indices. The table mask is typically a value one less than a power of two, which allows for efficient bit masking operations.

		\param key The key value to hash
		\param tableMask The mask value used to compute the hash index
		\return The computed hash index used for locating the position in the hash table
	*/
	static int GetHash( const _key_& key, const int tableMask ) { return key & tableMask; }

	/*!
		\brief Compares two keys and returns -1 if the first is less than the second, 1 if greater, and 0 if equal

		This function performs a lexicographical comparison between two keys of the same type. It is designed to be used as a comparison function for sorting or searching operations. The comparison is
	   based on the less-than and greater-than operators defined for the key type. The function returns negative one if the first key is less than the second, positive one if the first key is greater
	   than the second, and zero if they are equal

		\param key1 First key to compare
		\param key2 Second key to compare
		\return -1 if key1 is less than key2, 1 if key1 is greater than key2, 0 if they are equal
	*/
	static int Compare( const _key_& key1, const _key_& key2 )
	{
		if( key1 < key2 ) {
			return -1;
		} else if( key1 > key2 ) {
			return 1;
		}
		return 0;
	}

public:
	_key_						 key;
	_value_						 value;
	idHashNodeT<_key_, _value_>* next;
};

/*!
	\class idHashNodeT< idStr, _value_ >
	\brief Hash node implementation for a hash table with string keys and arbitrary value types.

	This template class implements a hash node used in hash table data structures, where keys are of type idStr and values are of type _value_. The class provides the fundamental building block for
   hash table operations, including node construction, hash computation, and key comparison. It supports linking nodes in chains within hash buckets and is designed to work with hash table
   implementations that manage the overall table structure and resizing. The hash computation uses a bitwise AND operation with a table mask to ensure proper indexing within the hash table bounds. The
   key comparison is case-insensitive, following the conventions of idStr::Icmp for consistent string ordering. This node type is intended to be used internally by hash table implementations and does
   not manage memory allocation or deallocation of the node objects themselves.

*/
template<class _value_>
class idHashNodeT<idStr, _value_>
{
public:
	/*!
		\brief Constructs a new hash node with the specified key, value, and next node pointer.

		This constructor initializes a hash node used in a hash table implementation. It stores the provided key and value pair, and sets the next pointer to link to the next node in the hash bucket
	   chain. The key and value are copied from the input parameters, and the next pointer is set directly to the provided node.

		\param key The key to store in the hash node
		\param value The value to store in the hash node
		\param next Pointer to the next node in the hash bucket chain
	*/
	idHashNodeT( const idStr& key, const _value_& value, idHashNodeT* next ) :
		key( key ),
		value( value ),
		next( next )
	{
	}

	/*!
		\brief Computes the hash value for a given string key using the table mask to determine the hash table index.

		This function calculates a hash value for the provided string key and applies a bitwise AND operation with the table mask to ensure the result fits within the bounds of the hash table size.
	   The hash table size is determined by the table mask, which is typically a power of two minus one.

		\param key The string key for which to compute the hash value
		\param tableMask The mask used to constrain the hash value to the table size
		\return The computed hash table index for the given key
	*/
	static int GetHash( const idStr& key, const int tableMask ) { return ( idStr::Hash( key ) & tableMask ); }

	/*!
		\brief Compares two idStr objects in a case-insensitive manner and returns the comparison result

		This function performs a case-insensitive comparison between two idStr objects using the idStr::Icmp method. It is typically used as a comparison function for hash table operations or other
	   data structures that require ordering of string keys. The return value follows standard comparison conventions where negative values indicate the first string is less than the second, zero
	   indicates equality, and positive values indicate the first string is greater than the second.

		\param key1 First string to compare
		\param key2 Second string to compare
		\return Negative value if key1 is lexicographically less than key2, zero if they are equal, positive value if key1 is greater than key2
	*/
	static int Compare( const idStr& key1, const idStr& key2 ) { return idStr::Icmp( key1, key2 ); }

public:
	idStr						 key;
	_value_						 value;
	idHashNodeT<idStr, _value_>* next;
};

/*!
	\class idHashNodeT< const char *, _value_ >
	\brief A hash node template class for storing key-value pairs with string keys in a hash table implementation.

	This template class provides a hash node implementation specifically designed for use with string keys stored as const char pointers. It serves as a fundamental building block for hash table data
   structures within the engine, supporting efficient key-value storage and retrieval. The class includes static methods for hash computation and key comparison that are optimized for string handling.
   The node maintains links to other nodes in the same hash bucket through the next pointer, enabling chaining for collision resolution. The template parameter allows for flexible value types while
   maintaining the string key constraint. This design supports the engine's need for fast associative storage with case-insensitive key lookups.

*/
template<class _value_>
class idHashNodeT<const char*, _value_>
{
public:
	/*!
		\brief Constructs a hash node with the specified key, value, and next node pointer

		This constructor initializes a hash node used in a hash table implementation. It sets the key and value members to the provided parameters and links the node to the next node in the chain. The
	   key is stored as a const char pointer, and the value is copied from the provided value parameter. The next pointer is set to point to the subsequent node in the hash bucket

		\param key The key for the hash node
		\param value The value associated with the key
		\param next Pointer to the next node in the hash bucket chain
	*/
	idHashNodeT( const char* const& key, const _value_& value, idHashNodeT* next ) :
		key( key ),
		value( value ),
		next( next )
	{
	}

	/*!
		\brief Computes a hash value for a string key based on the provided table mask

		This function calculates a hash value for the given string key using the idStr::Hash function and then applies a bitwise AND operation with the tableMask to ensure the result fits within the
	   hash table size. The tableMask is typically a value one less than a power of two, which allows for efficient masking operations.

		\param key The string key to hash
		\param tableMask The mask value to apply to the hash result
		\return The computed hash value that can be used as an index into a hash table
	*/
	static int GetHash( const char* const& key, const int tableMask ) { return ( idStr::Hash( key ) & tableMask ); }

	/*!
		\brief Compares two string keys for hashing purposes using case-insensitive string comparison

		This function performs a case-insensitive comparison of two string keys used in a hash table implementation. It is designed to be used as a comparison function for hash nodes that store string
	   keys. The function leverages the existing idStr::Icmp method which provides platform-independent case-insensitive string comparison.

		\param key1 First string key to compare
		\param key2 Second string key to compare
		\return Negative value if key1 is lexicographically less than key2, zero if they are equal, positive value if key1 is lexicographically greater than key2
	*/
	static int Compare( const char* const& key1, const char* const& key2 ) { return idStr::Icmp( key1, key2 ); }

public:
	idStr							   key; // char * keys must still get stored in an idStr
	_value_							   value;
	idHashNodeT<const char*, _value_>* next;
};

/*!
	\class idHashTableT
	\brief A hash table implementation template for storing key-value pairs with efficient lookup and insertion operations.

	The idHashTableT class provides a template-based hash table implementation designed for efficient storage and retrieval of key-value pairs. It uses a linked list approach within each hash bucket
   to handle collisions, ensuring good performance for typical use cases. The table is initialized with a power-of-two size to enable fast bitwise operations during hashing. The class supports various
   operations including setting values, retrieving values, removing entries, and clearing the entire table. It also provides utility functions for memory tracking, entry enumeration, and checking
   distribution quality. The hash table maintains sorted order within buckets to optimize lookups, and handles memory management through standard allocation and deallocation patterns. The
   implementation is intended for use in engine systems that require fast associative storage with minimal overhead, such as resource management, caching, or configuration systems.

*/
template<typename _key_, class _value_>
class idHashTableT
{
public:
	/*!
		\brief Constructs an inline hash table with the specified table size

		Initializes a hash table with the given table size, which must be a power of two. The constructor allocates memory for the hash buckets and initializes all pointers to NULL. It also sets up
	   the internal mask used for hashing operations

		\param tableSize The size of the hash table, must be a power of two
		\throws Assertion failure if tableSize is not a power of two
	*/
	idHashTableT( const int tableSize = 256 );

	/*!
		\brief Creates a copy of the given hash table.

		This constructor initializes a new hash table by copying all elements from another hash table. It uses the Copy method to perform the actual duplication of the table's contents.

		\param other The hash table to copy from
	*/
	idHashTableT( const idHashTableT& other );

	/*!
		\brief Destructor for the idHashTableT template class that cleans up all allocated resources.

		This destructor releases all memory allocated for the hash table by calling Clear to remove all entries and then deleting the underlying array of hash table heads. It also resets all internal
	   state variables to their default values to ensure the object is properly cleaned up.

	*/
	~idHashTableT();

	//! Returns the total amount of memory allocated by the hash table in bytes
	size_t		  Allocated() const;

	//! Returns the total size in bytes of the hash table including its internal structures and stored elements.
	size_t		  Size() const;

	/*!
		\brief Sets the value for a given key in the hash table, returning a reference to the stored value

		This function inserts or updates a key-value pair in the hash table. If the key already exists, it updates the value and returns a reference to the existing value. If the key does not exist,
	   it creates a new entry and returns a reference to the newly created value. The function maintains sorted order within hash buckets for efficient lookups.

		\param key The key to set or update in the hash table
		\param value The value to associate with the key
		\return A reference to the value stored in the hash table for the specified key
	*/
	_value_&	  Set( const _key_& key, const _value_& value );

	/*!
		\brief Retrieves a value from the hash table by key, returning true if found.

		This function searches for a key in the hash table and retrieves the corresponding value if it exists. The function uses hash-based lookup for efficient retrieval. If the key is found, the
	   value is returned through the output parameter. If the key is not found, the output parameter is set to NULL and the function returns false. The hash table is expected to be properly
	   initialized and the key type must be compatible with the hash function and comparison operator.

		\param key The key to search for in the hash table
		\param value Pointer to a pointer where the found value will be stored, or NULL to only check for existence
		\return True if the key was found in the hash table, false otherwise
	*/
	bool		  Get( const _key_& key, _value_** value = NULL );

	/*!
		\brief Retrieves a value from the hash table using the specified key, returning true if the key exists.

		This function searches for a given key in the hash table and, if found, stores a pointer to the corresponding value in the output parameter. The function returns true if the key exists in the
	   table, and false otherwise. If the key is not found and the output parameter is provided, it will be set to NULL.

		\param key The key to search for in the hash table.
		\param value Pointer to a pointer where the retrieved value will be stored, or NULL if the key is not found.
		\return True if the key was found in the hash table, false otherwise.
	*/
	bool		  Get( const _key_& key, const _value_** value = NULL ) const;

	/*!
		\brief Removes a key-value pair from the hash table if the key exists

		This function removes a key-value pair from the hash table by searching for the specified key and unlinking the corresponding node from the linked list at the hash table's bucket. If the key
	   is found, the node is deleted and the function returns true, otherwise it returns false. The function maintains the hash table's entry count by decrementing it when a node is removed

		\param key The key to be removed from the hash table
		\return true if the key was found and removed, false otherwise
	*/
	bool		  Remove( const _key_& key );

	//! Clears all entries from the hash table by deleting all nodes and resetting the table state.
	void		  Clear();

	/*!
		\brief Deletes all contents stored in the hash table

		This function iterates through all buckets in the hash table and deletes all nodes and their associated values. It resets the table to an empty state by clearing all entries and setting the
	   entry count to zero

	*/
	void		  DeleteContents();

	//! Returns the number of entries in the hash table.
	int			  Num() const;

	/*!
		\brief Retrieves a pointer to the value at the specified index in the hash table.

		This function iterates through the hash table's internal structure to find the element at the given index. It performs bounds checking and returns NULL if the index is invalid. The function is
	   typically used to access elements in a hash table by their position rather than by key.

		\param index The zero-based index of the element to retrieve
		\return A pointer to the value at the specified index, or NULL if the index is out of bounds
		\throws Asserts and returns NULL when the index is out of valid range
	*/
	_value_*	  GetIndex( const int index ) const;

	/*!
		\brief Retrieves the key at the specified index in the hash table.

		This function iterates through the hash table buckets to find the entry at the given index and retrieves the corresponding key. It performs bounds checking to ensure the index is valid. If the
	   index is out of bounds, the function returns false and does not modify the key parameter. The function is typically used to enumerate all keys in the hash table in a deterministic order.

		\param index The zero-based index of the key to retrieve
		\param key Reference to store the retrieved key
		\return True if the key was successfully retrieved, false if the index is out of bounds
	*/
	bool		  GetIndexKey( const int index, _key_& key ) const;

	//! Returns the spread percentage of the hash table, indicating how evenly entries are distributed across buckets.
	int			  GetSpread() const;

	idHashTableT& operator=( const idHashTableT& other );

protected:
	/*!
		\brief Copies the contents of another hash table into this hash table

		This function performs a deep copy of all entries from the source hash table to the current hash table. It first checks if the source table is the same as the current table to avoid
	   self-copying. The function allocates new memory for the hash table structure and duplicates all key-value pairs from the source table. The hash table properties such as table size, number of
	   entries, and mask are also copied from the source table. Each node in the source table is duplicated to create a new node in the destination table.

		\param other The source hash table to copy from
	*/
	void Copy( const idHashTableT& other );

private:
	typedef idHashNodeT<_key_, _value_> hashnode_t;

	hashnode_t**						heads;

	int									tableSize;
	int									numEntries;
	int									tableSizeMask;
};

template<typename _key_, class _value_>
ID_INLINE idHashTableT<_key_, _value_>::idHashTableT( const int tableSize )
{
	assert( idMath::IsPowerOfTwo( tableSize ) );

	this->tableSize = tableSize;

	heads = new( TAG_IDLIB_HASH ) hashnode_t*[tableSize];
	memset( heads, 0, sizeof( hashnode_t* ) * tableSize );

	numEntries	  = 0;
	tableSizeMask = tableSize - 1;
}

template<typename _key_, class _value_>
ID_INLINE idHashTableT<_key_, _value_>::idHashTableT( const idHashTableT& other )
{
	Copy( other );
}

template<typename _key_, class _value_>
ID_INLINE idHashTableT<_key_, _value_>::~idHashTableT()
{
	Clear();
	delete[] heads;
	heads		  = NULL;
	tableSize	  = 0;
	tableSizeMask = 0;
	numEntries	  = 0;
}

template<typename _key_, class _value_>
ID_INLINE size_t idHashTableT<_key_, _value_>::Allocated() const
{
	return sizeof( heads ) * tableSize + sizeof( hashnode_t* ) * numEntries;
}

template<typename _key_, class _value_>
ID_INLINE size_t idHashTableT<_key_, _value_>::Size() const
{
	return sizeof( idHashTableT ) + sizeof( heads ) * tableSize + sizeof( hashnode_t* ) * numEntries;
}

template<typename _key_, class _value_>
ID_INLINE _value_& idHashTableT<_key_, _value_>::Set( const _key_& key, const _value_& value )
{
	// insert sorted
	int			 hash	 = hashnode_t::GetHash( key, tableSizeMask );
	hashnode_t** nextPtr = &( heads[hash] );
	hashnode_t*	 node	 = *nextPtr;
	for( ; node != NULL; nextPtr = &( node->next ), node = *nextPtr ) {
		int s = node->Compare( node->key, key );
		if( s == 0 ) {
			// return existing hashed item
			node->value = value;
			return node->value;
		}
		if( s > 0 ) { break; }
	}

	numEntries++;

	*nextPtr		   = new( TAG_IDLIB_HASH ) hashnode_t( key, value, heads[hash] );
	( *nextPtr )->next = node;
	return ( *nextPtr )->value;
}

template<typename _key_, class _value_>
ID_INLINE bool idHashTableT<_key_, _value_>::Get( const _key_& key, _value_** value )
{
	int			hash = hashnode_t::GetHash( key, tableSizeMask );
	hashnode_t* node = heads[hash];
	for( ; node != NULL; node = node->next ) {
		int s = node->Compare( node->key, key );
		if( s == 0 ) {
			if( value ) { *value = &node->value; }
			return true;
		}
		if( s > 0 ) { break; }
	}
	if( value ) { *value = NULL; }
	return false;
}

template<typename _key_, class _value_>
ID_INLINE bool idHashTableT<_key_, _value_>::Get( const _key_& key, const _value_** value ) const
{
	int			hash = hashnode_t::GetHash( key, tableSizeMask );
	hashnode_t* node = heads[hash];
	for( ; node != NULL; node = node->next ) {
		int s = node->Compare( node->key, key );
		if( s == 0 ) {
			if( value ) { *value = &node->value; }
			return true;
		}
		if( s > 0 ) { break; }
	}
	if( value ) { *value = NULL; }
	return false;
}

template<typename _key_, class _value_>
ID_INLINE _value_* idHashTableT<_key_, _value_>::GetIndex( const int index ) const
{
	if( index < 0 || index > numEntries ) {
		assert( 0 );
		return NULL;
	}

	int count = 0;
	for( int i = 0; i < tableSize; i++ ) {
		for( hashnode_t* node = heads[i]; node != NULL; node = node->next ) {
			if( count == index ) { return &node->value; }
			count++;
		}
	}
	return NULL;
}

template<typename _key_, class _value_>
ID_INLINE bool idHashTableT<_key_, _value_>::GetIndexKey( const int index, _key_& key ) const
{
	if( index < 0 || index > numEntries ) {
		assert( 0 );
		return false;
	}

	int count = 0;
	for( int i = 0; i < tableSize; i++ ) {
		for( hashnode_t* node = heads[i]; node != NULL; node = node->next ) {
			if( count == index ) {
				key = node->key;
				return true;
			}
			count++;
		}
	}
	return false;
}

template<typename _key_, class _value_>
ID_INLINE bool idHashTableT<_key_, _value_>::Remove( const _key_& key )
{
	int			 hash = hashnode_t::GetHash( key, tableSizeMask );
	hashnode_t** head = &heads[hash];
	if( *head ) {
		hashnode_t* prev = NULL;
		hashnode_t* node = *head;
		for( ; node != NULL; prev = node, node = node->next ) {
			if( node->key == key ) {
				if( prev ) {
					prev->next = node->next;
				} else {
					*head = node->next;
				}

				delete node;
				numEntries--;
				return true;
			}
		}
	}
	return false;
}

template<typename _key_, class _value_>
ID_INLINE void idHashTableT<_key_, _value_>::Clear()
{
	for( int i = 0; i < tableSize; i++ ) {
		hashnode_t* next = heads[i];
		while( next != NULL ) {
			hashnode_t* node = next;
			next			 = next->next;
			delete node;
		}
		heads[i] = NULL;
	}
	numEntries = 0;
}

template<typename _key_, class _value_>
ID_INLINE void idHashTableT<_key_, _value_>::DeleteContents()
{
	for( int i = 0; i < tableSize; i++ ) {
		hashnode_t* next = heads[i];
		while( next != NULL ) {
			hashnode_t* node = next;
			next			 = next->next;
			delete node->value;
			delete node;
		}
		heads[i] = NULL;
	}
	numEntries = 0;
}

template<typename _key_, class _value_>
ID_INLINE int idHashTableT<_key_, _value_>::Num() const
{
	return numEntries;
}

template<typename _key_, class _value_>
ID_INLINE int idHashTableT<_key_, _value_>::GetSpread() const
{
	if( !numEntries ) { return 100; }

	int average = numEntries / tableSize;
	int error	= 0;
	for( int i = 0; i < tableSize; i++ ) {
		int numItems = 0;
		for( hashnode_t* node = heads[i]; node != NULL; node = node->next ) {
			numItems++;
		}
		int e = abs( numItems - average );
		if( e > 1 ) { error += e - 1; }
	}
	return 100 - ( error * 100 / numEntries );
}

/*
========================
idHashTableT<_key_,_value_>::operator=
========================
*/
template<typename _key_, class _value_>
ID_INLINE idHashTableT<_key_, _value_>& idHashTableT<_key_, _value_>::operator=( const idHashTableT& other )
{
	Copy( other );
	return *this;
}

template<typename _key_, class _value_>
ID_INLINE void idHashTableT<_key_, _value_>::Copy( const idHashTableT& other )
{
	if( &other == this ) { return; }
	assert( other.tableSize > 0 );

	tableSize	  = other.tableSize;
	heads		  = new( TAG_IDLIB_HASH ) hashnode_t*[tableSize];
	numEntries	  = other.numEntries;
	tableSizeMask = other.tableSizeMask;

	for( int i = 0; i < tableSize; i++ ) {
		if( !other.heads[i] ) {
			heads[i] = NULL;
			continue;
		}
		hashnode_t** prev = &heads[i];
		for( hashnode_t* node = other.heads[i]; node != NULL; node = node->next ) {
			*prev = new( TAG_IDLIB_HASH ) hashnode_t( node->key, node->value, NULL );
			prev  = &( *prev )->next;
		}
	}
}

/*! \class
	\brief General hash table. Slower than idHashIndex but it can also be used for linked lists and other data structures than just indexes or arrays.

	\note The entire contents can be iterated over, but that the exact index for a given element may change when new elements are added.

*/

template<class Type>
class idHashTable
{
public:
	/*!
		\brief Initializes a new hash table with the specified table size

		Creates a new hash table instance with the given table size. The table size must be a power of two as enforced by an assertion. The hash table is initialized with all head pointers set to NULL
	   and the entry count set to zero. The table size mask is calculated as tablesize - 1 for efficient bitwise operations during hashing.

		\param newtablesize The size of the hash table, must be a power of two
		\throws assertion failure if newtablesize is not a power of two or is not greater than zero
	*/
	idHashTable( int newtablesize = 256 );

	/*!
		\brief Creates a new hash table as a copy of an existing hash table.

		This constructor initializes a new hash table by copying all entries from another hash table. It allocates memory for the new table structure and duplicates all key-value pairs from the source
	   table. The new table will have the same size and content as the original, but will be an independent copy with its own memory allocation.

		\param map The hash table to copy from
		\throws assertion failure if the source table is invalid
	*/
	idHashTable( const idHashTable<Type>& map );

	/*!
		\brief Destructor for the idHashTable class that clears the hash table and frees the memory allocated for the hash table heads array.

		This destructor cleans up the resources used by the hash table. It first clears all the elements in the hash table by calling the Clear method, which removes all key-value pairs. Then it
	   deallocates the memory that was allocated for the heads array, which stores the start of each hash bucket.

	*/
	~idHashTable();

	//! Returns the total amount of memory allocated by the hash table in bytes
	size_t Allocated() const;

	//! Returns the total size in bytes of the memory allocated for the hash table including size of hash table type
	size_t Size() const;

	/*!
		\brief Sets the value for a given key in the hash table

		This function stores a value associated with a key in the hash table. If the key already exists, it updates the existing value. Otherwise, it creates a new entry for the key-value pair. The
	   function uses a hash table implementation that maintains entries in sorted order within each hash bucket for efficient lookup and insertion operations. The hash table automatically manages
	   memory allocation for new entries using a tagged memory allocator.

		\param key The key to associate with the value
		\param value The value to store for the given key
	*/
	void   Set( const char* key, Type& value );

	/*!
		\brief Retrieves a value from the hash table using the provided key.

		This function searches for a key in the hash table and if found, returns true and sets the value parameter to point to the corresponding value. If the key is not found, it returns false and
	   sets the value parameter to NULL. The function is const and does not modify the hash table structure.

		\param key The key to search for in the hash table
		\param value Pointer to a pointer where the found value will be stored, or NULL if the key is not found
		\return True if the key was found in the hash table, false otherwise
	*/
	bool   Get( const char* key, Type** value = NULL ) const;

	/*!
		\brief Removes a key-value pair from the hash table using the specified key.

		This function searches for a node in the hash table with the given key and removes it if found. It updates the links in the linked list to maintain the integrity of the hash table structure.
	   The function returns true if the key was found and removed, and false if the key was not found in the table.

		\param key The key used to locate and remove the corresponding entry from the hash table
		\return True if the key was found and removed from the hash table, false otherwise
	*/
	bool   Remove( const char* key );

	//! Clears all entries from the hash table by deleting all nodes and resetting the table state.
	void   Clear();

	/*!
		\brief Deletes all contents stored in the hash table.

		This function iterates through all buckets in the hash table and deletes all nodes and their associated values. It frees the memory allocated for each node and the value it points to, then
	   resets the table to an empty state.

	*/
	void   DeleteContents();

	//! Returns the number of entries in the hash table.
	int	   Num() const;

	/*!
		\brief Returns a pointer to the Type object at the specified index in the hash table

		This function retrieves a pointer to the Type object located at the given index within the hash table. It performs bounds checking to ensure the index is valid, returning NULL and asserting if
	   the index is out of range. The function traverses the hash table buckets in order to find the element at the specified index, making it useful for iterating through all elements in the hash
	   table.

		\param index The zero-based index of the element to retrieve
		\return A pointer to the Type object at the specified index, or NULL if the index is invalid
		\throws This function asserts and returns NULL if the index is out of bounds
	*/
	Type*  GetIndex( int index ) const;

	//! Returns the spread percentage of the hash table.
	int	   GetSpread() const;

private:
	/*!
		\struct idHashTable::hashnode_s
		\brief Hash table node structure for storing key-value pairs in the hash table implementation.

		This structure represents a node within the hash table data structure used for efficient key-value pair storage and retrieval. The node contains a key-value pair along with a pointer to the
	   next node in the chain, supporting collision resolution through chaining. It is designed to work with the idHashTable template class and is not intended for direct instantiation outside of the
	   hash table's internal mechanisms.

	*/
	struct hashnode_s {
		idStr		key;
		Type		value;
		hashnode_s* next;

		hashnode_s( const idStr& k, Type v, hashnode_s* n ) :
			key( k ),
			value( v ),
			next( n ) {};
		hashnode_s( const char* k, Type v, hashnode_s* n ) :
			key( k ),
			value( v ),
			next( n ) {};
	};

	hashnode_s** heads;

	int			 tablesize;
	int			 numentries;
	int			 tablesizemask;

	/*!
		\brief Computes the hash value for a given string key using the table size mask

		The function calculates a hash value for the provided string key by utilizing the idStr::Hash function and then applies a bitwise AND operation with the tablesizemask to ensure the result fits
	   within the hash table's size constraints

		\param key The string key for which to compute the hash value
		\return The computed hash value as an integer that represents the index where the key should be stored in the hash table
	*/
	int			 GetHash( const char* key ) const;
};

template<class Type>
ID_INLINE idHashTable<Type>::idHashTable( int newtablesize )
{
	assert( idMath::IsPowerOfTwo( newtablesize ) );

	tablesize = newtablesize;
	assert( tablesize > 0 );

	heads = new( TAG_IDLIB_HASH ) hashnode_s*[tablesize];
	memset( heads, 0, sizeof( *heads ) * tablesize );

	numentries = 0;

	tablesizemask = tablesize - 1;
}

template<class Type>
ID_INLINE idHashTable<Type>::idHashTable( const idHashTable<Type>& map )
{
	int			 i;
	hashnode_s*	 node;
	hashnode_s** prev;

	assert( map.tablesize > 0 );

	tablesize	  = map.tablesize;
	heads		  = new( TAG_IDLIB_HASH ) hashnode_s*[tablesize];
	numentries	  = map.numentries;
	tablesizemask = map.tablesizemask;

	for( i = 0; i < tablesize; i++ ) {
		if( !map.heads[i] ) {
			heads[i] = NULL;
			continue;
		}

		prev = &heads[i];
		for( node = map.heads[i]; node != NULL; node = node->next ) {
			*prev = new( TAG_IDLIB_HASH ) hashnode_s( node->key, node->value, NULL );
			prev  = &( *prev )->next;
		}
	}
}

template<class Type>
ID_INLINE idHashTable<Type>::~idHashTable()
{
	Clear();
	delete[] heads;
}

template<class Type>
ID_INLINE size_t idHashTable<Type>::Allocated() const
{
	return sizeof( heads ) * tablesize + sizeof( *heads ) * numentries;
}

template<class Type>
ID_INLINE size_t idHashTable<Type>::Size() const
{
	return sizeof( idHashTable<Type> ) + sizeof( heads ) * tablesize + sizeof( *heads ) * numentries;
}

template<class Type>
ID_INLINE int idHashTable<Type>::GetHash( const char* key ) const
{
	return ( idStr::Hash( key ) & tablesizemask );
}

template<class Type>
ID_INLINE void idHashTable<Type>::Set( const char* key, Type& value )
{
	hashnode_s *node, **nextPtr;
	int			hash, s;

	hash = GetHash( key );
	for( nextPtr = &( heads[hash] ), node = *nextPtr; node != NULL; nextPtr = &( node->next ), node = *nextPtr ) {
		s = node->key.Cmp( key );
		if( s == 0 ) {
			node->value = value;
			return;
		}
		if( s > 0 ) { break; }
	}

	numentries++;

	*nextPtr		   = new( TAG_IDLIB_HASH ) hashnode_s( key, value, heads[hash] );
	( *nextPtr )->next = node;
}

template<class Type>
ID_INLINE bool idHashTable<Type>::Get( const char* key, Type** value ) const
{
	hashnode_s* node;
	int			hash, s;

	hash = GetHash( key );
	for( node = heads[hash]; node != NULL; node = node->next ) {
		s = node->key.Cmp( key );
		if( s == 0 ) {
			if( value ) { *value = &node->value; }
			return true;
		}
		if( s > 0 ) { break; }
	}

	if( value ) { *value = NULL; }

	return false;
}

template<class Type>
ID_INLINE Type* idHashTable<Type>::GetIndex( int index ) const
{
	hashnode_s* node;
	int			count;
	int			i;

	if( ( index < 0 ) || ( index > numentries ) ) {
		assert( 0 );
		return NULL;
	}

	count = 0;
	for( i = 0; i < tablesize; i++ ) {
		for( node = heads[i]; node != NULL; node = node->next ) {
			if( count == index ) { return &node->value; }
			count++;
		}
	}

	return NULL;
}

template<class Type>
ID_INLINE bool idHashTable<Type>::Remove( const char* key )
{
	hashnode_s** head;
	hashnode_s*	 node;
	hashnode_s*	 prev;
	int			 hash;

	hash = GetHash( key );
	head = &heads[hash];
	if( *head ) {
		for( prev = NULL, node = *head; node != NULL; prev = node, node = node->next ) {
			if( node->key == key ) {
				if( prev ) {
					prev->next = node->next;
				} else {
					*head = node->next;
				}

				delete node;
				numentries--;
				return true;
			}
		}
	}

	return false;
}

template<class Type>
ID_INLINE void idHashTable<Type>::Clear()
{
	int			i;
	hashnode_s* node;
	hashnode_s* next;

	for( i = 0; i < tablesize; i++ ) {
		next = heads[i];
		while( next != NULL ) {
			node = next;
			next = next->next;
			delete node;
		}

		heads[i] = NULL;
	}

	numentries = 0;
}

template<class Type>
ID_INLINE void idHashTable<Type>::DeleteContents()
{
	int			i;
	hashnode_s* node;
	hashnode_s* next;

	for( i = 0; i < tablesize; i++ ) {
		next = heads[i];
		while( next != NULL ) {
			node = next;
			next = next->next;
			delete node->value;
			delete node;
		}

		heads[i] = NULL;
	}

	numentries = 0;
}

template<class Type>
ID_INLINE int idHashTable<Type>::Num() const
{
	return numentries;
}

#if defined( ID_TYPEINFO )
	#define __GNUC__ 99
#endif

#if !defined( __GNUC__ ) || __GNUC__ < 4
template<class Type>
int idHashTable<Type>::GetSpread() const
{
	int			i, average, error, e;
	hashnode_s* node;

	// if no items in hash
	if( !numentries ) { return 100; }
	average = numentries / tablesize;
	error	= 0;
	for( i = 0; i < tablesize; i++ ) {
		numItems = 0;
		for( node = heads[i]; node != NULL; node = node->next ) {
			numItems++;
		}
		e = abs( numItems - average );
		if( e > 1 ) { error += e - 1; }
	}
	return 100 - ( error * 100 / numentries );
}
#endif

#if defined( ID_TYPEINFO )
	#undef __GNUC__
#endif

#endif /* !__HASHTABLE_H__ */
