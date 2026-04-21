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

#ifndef __HASHINDEX_H__
#define __HASHINDEX_H__

/*
===============================================================================

	Fast hash table for indexes and arrays.
	Does not allocate memory until the first key/index pair is added.

===============================================================================
*/

#define DEFAULT_HASH_SIZE		 1024
#define DEFAULT_HASH_GRANULARITY 1024

/*!
	\class idHashIndex
	\brief A hash index implementation for efficient storage and retrieval of indexed data using hash tables.

	The idHashIndex class provides a hash table implementation designed for efficient storage and retrieval of indexed data, commonly used in collision detection and AAS (Area Awareness System)
   building within the engine. It supports dynamic resizing, memory management, and various hash key generation methods for different data types including strings, 3D vectors, and integers. The class
   maintains internal data structures including hash tables and index chains to manage indexed entries, with support for adding, removing, and iterating through hash chain entries. The implementation
   uses bitwise operations for efficient hash computations and includes mechanisms for controlling memory allocation granularity. The hash table size must be a power of two, and the class provides
   methods for initializing, allocating, clearing, and freeing hash index resources.

*/
class idHashIndex
{
public:
	static const int NULL_INDEX = -1;

	/*!
		\brief Initializes a hash index with default hash size parameters

		This constructor initializes a hash index using the default hash size for both the initial and maximum hash sizes. It is typically used during the setup of collision detection systems where
	   hash tables are needed to efficiently store and retrieve geometric data such as vertices and edges.

	*/
	idHashIndex();

	/*!
		\brief Initializes a new hash index with the specified initial hash size and index size.

		This constructor initializes a new hash index structure using the provided initial hash size and index size parameters. The hash index is typically used for efficiently storing and retrieving
	   elements based on their hash values. The constructor calls the Init method to perform the actual initialization work.

		\param initialHashSize The initial size of the hash table
		\param initialIndexSize The initial size of the index table
	*/
	idHashIndex( const int initialHashSize, const int initialIndexSize );

	/*!
		\brief Destructor for the idHashIndex class that frees all allocated resources.

		This inline destructor cleans up the idHashIndex object by calling the Free method, which releases any memory that was allocated for the hash index structure. It ensures proper resource
	   management when the object goes out of scope or is explicitly destroyed.

	*/
	~idHashIndex();

	//! Returns the total amount of memory allocated by the hash index structure in bytes
	size_t		 Allocated() const;

	//! Returns the total size in bytes of memory allocated for the hash index including the hash index type size
	size_t		 Size() const;

	idHashIndex& operator=( const idHashIndex& other );

	/*!
		\brief Adds an index to the hash table using the provided key

		This function inserts an index into the hash table structure. It first validates that the index is non-negative and then ensures the hash table has sufficient capacity. The function computes
	   the hash value using the key and the hash mask, then updates the linked list structure of the hash table to include the new index.

		\param key The key used to determine the hash bucket for the index
		\param index The index value to be added to the hash table
		\throws assertion failure if index is negative
	*/
	void		 Add( const int key, const int index );

	/*!
		\brief Removes a specific index from the hash table entry associated with the given key.

		This function removes an index from a hash table structure by updating the linked list of indices for the specified key. It handles two cases: when the index to remove is the first in the
	   chain, or when it is somewhere later in the chain. The function updates the appropriate links in the indexChain array and marks the removed index as invalid by setting its entry to -1. The hash
	   table uses a bitwise AND operation with hashMask to compute the initial hash index.

		\param key The hash key used to locate the entry in the hash table
		\param index The index to be removed from the hash table
	*/
	void		 Remove( const int key, const int index );

	/*!
		\brief Retrieves the first index from the hash for a given key, returning -1 if the hash entry is empty

		This function accesses a hash table structure to obtain the first entry associated with the specified key. It uses bitwise operations to compute the hash index and returns -1 when no entries
	   are found for the key. The function is commonly used in collision detection and AAS (Area Awareness System) building where vertices and edges are stored in hash tables for efficient lookup.

		\param key The hash key used to look up the first entry in the hash table
		\return The first index in the hash table for the given key, or -1 if the hash entry is empty
	*/
	int			 First( const int key ) const;

	/*!
		\brief Returns the next index in the hash chain for the given index

		This function retrieves the next element in a hash chain by using the index to look up the next entry in the indexChain array. The index is masked with lookupMask to ensure it stays within
	   valid bounds. The function is typically used to iterate through all elements that share the same hash key in a hash table implementation.

		\param index the current index in the hash chain
		\return the next index in the hash chain, or -1 if at the end of the chain
		\throws asserts that the index is within valid range [0, indexSize)
	*/
	int			 Next( const int index ) const;

	/*!
		\brief Returns the first index in the hash table for the given key

		This function retrieves the first entry index associated with the specified key in the hash table. It is commonly used in hash-based lookups to iterate through entries that map to the same
	   hash key. The function is typically called as part of a loop to traverse all entries for a given key, using GetNext to continue iteration.

		\param key The hash key to look up in the table
		\return The index of the first entry matching the key, or idHashIndex::NULL_INDEX if no entries are found
	*/
	int			 GetFirst( const int key ) const { return First( key ); }

	/*!
		\brief Returns the next index in the hash chain for a given index

		This function retrieves the next index in a hash table chain for a specified index. It is used as part of an iterator pattern to traverse all entries that hash to the same value. The function
	   is marked as const, indicating it does not modify the hash index object. The returned index can be used to continue iteration through the hash table entries or to check if there are more
	   entries in the chain.

		\param index The current index in the hash table to get the next index for
		\return The next index in the hash chain, or idHashIndex::NULL_INDEX if there are no more entries
	*/
	int			 GetNext( const int index ) const { return Next( index ); }

	/*!
		\brief Inserts a new entry into the hash index structure, adjusting existing indices to maintain consistency.

		This function inserts a new entry into the hash index by first updating the hash table and index chain to account for the new entry. It increases all existing indices that are greater than or
	   equal to the new index value to maintain data structure integrity. The function also handles dynamic resizing of the index chain when necessary to accommodate the new entry. After updating the
	   indices, it adds the new key-value pair to the hash table.

		\param key The key to be inserted into the hash table
		\param index The index position where the new entry should be placed in the index chain
	*/
	void		 InsertIndex( const int key, const int index );

	/*!
		\brief Removes an entry from the index and hash, adjusting all subsequent indexes accordingly.

		This function removes a specific entry identified by its key and index from both the hash table and the index chain. It decrements all index values that are greater than or equal to the
	   removed index to maintain consistency. The function handles the reorganization of the index chain by shifting elements and setting the last element to -1 to indicate its removal. This operation
	   ensures that the internal data structures remain consistent after an element is removed.

		\param key The key identifying the hash entry to remove
		\param index The index of the entry to remove from the hash table
	*/
	void		 RemoveIndex( const int key, const int index );

	//! Clears the hash index table
	void		 Clear();

	//! Clears the hash index and resets its size.
	void		 Clear( const int newHashSize, const int newIndexSize );

	/*!
		\brief Frees the memory allocated for the hash index and index chain arrays

		This function releases the memory that was previously allocated for the hash table and index chain structures. It checks if the hash and indexChain pointers are valid (not equal to
	   INVALID_INDEX) before attempting to delete the allocated memory. After freeing the memory, it sets both pointers to INVALID_INDEX to indicate that the memory has been released. The lookupMask
	   is also reset to zero.

	*/
	void		 Free();

	//! Returns the size of the hash table used by the hash index.
	int			 GetHashSize() const;

	//! Returns the size of the hash index.
	int			 GetIndexSize() const;

	/*!
		\brief Sets the granularity for the hash index to control memory allocation behavior.

		This function updates the granularity parameter that determines how memory is allocated in chunks for the hash index. The granularity must be a positive integer value. It is used internally by
	   the hash index implementation to optimize memory usage and allocation patterns.

		\param newGranularity The new granularity value to set, must be greater than zero
		\throws assertion failure if newGranularity is less than or equal to zero
	*/
	void		 SetGranularity( const int newGranularity );

	/*!
		\brief Resizes the hash index to the specified new size while maintaining existing hash table data.

		This function adjusts the internal index array to a new size that is a multiple of the granularity. It preserves the existing hash table data by copying it to the new array. The function only
	   performs resizing if the new size is larger than the current size. If the index chain is invalid, it simply updates the index size. The new array is initialized with invalid indices to maintain
	   data integrity.

		\param newIndexSize The desired new size for the hash index
	*/
	void		 ResizeIndex( const int newIndexSize );

	//! Returns a value in the range [0-100] representing the spread of items over the hash table.
	int			 GetSpread() const;

	/*!
		\brief Generates a hash key for a string using either case-sensitive or case-insensitive hashing based on the specified parameter.

		This function computes a hash value for the provided string using either the standard hash function or the case-insensitive hash function depending on the caseSensitive parameter. The
	   resulting hash is then masked with the internal hashMask to ensure it fits within the hash table's bounds. This is commonly used for indexing into hash tables where fast lookups are required.

		\param string The input string to generate a hash key for
		\param caseSensitive Determines whether the hash should be case-sensitive or case-insensitive
		\return An integer hash key that can be used for indexing into a hash table
	*/
	int			 GenerateKey( const char* string, bool caseSensitive = true ) const;

	/*!
		\brief Generates a hash key for a 3D vector using integer components and a mask

		This function computes a hash key for a 3D vector by summing the integer components of the vector and applying a bitwise AND with a hash mask. The hash mask is typically a power-of-two minus
	   one, which ensures the result fits within the hash table size. This approach is commonly used for simple and fast hash computations in collision detection and other systems that require quick
	   lookups based on vector coordinates.

		\param v The 3D vector for which to generate a hash key
		\return An integer hash key derived from the vector's components using a bitwise mask
	*/
	int			 GenerateKey( const idVec3& v ) const;

	/*!
		\brief Generates a hash key for two integers using bitwise AND with the hash mask

		This function takes two integer values and combines them using addition before applying a bitwise AND operation with the hash mask to produce a hash key. The result is used for indexing into a
	   hash table structure. The hash mask is typically derived from the size of the hash table and is used to ensure the key falls within valid bounds

		\param n1 First integer value for hash generation
		\param n2 Second integer value for hash generation
		\return The generated hash key as an integer value
	*/
	int			 GenerateKey( const int n1, const int n2 ) const;

	/*!
		\brief Generates a hash key for a given integer using the internal hash mask

		This function computes a hash key for a given integer by performing a bitwise AND operation with the internal hash mask. It is used to determine the bucket index in a hash table for collision
	   resolution. The hash mask is typically a power-of-two minus one, which allows for efficient hash computation through bitwise operations.

		\param n The integer value to generate a hash key for
		\return The computed hash key as an integer
	*/
	int			 GenerateKey( const int n ) const;

private:
	int		   hashSize;
	int*	   hash;
	int		   indexSize;
	int*	   indexChain;
	int		   granularity;
	int		   hashMask;
	int		   lookupMask;

	static int INVALID_INDEX[1];

	/*!
		\brief Initializes the hash index with the specified hash size and index size.

		This function sets up the internal hash table and index chain for the hash index structure. It asserts that the hash size is a power of two, which is required for the hash table to function
	   correctly. The function initializes various internal state variables including the hash size, index size, and mask values used for hash calculations. The granularity is set to a default value,
	   and the lookup mask is initialized to zero.

		\param initialHashSize The initial size of the hash table, must be a power of two
		\param initialIndexSize The initial size of the index chain
		\throws assertion failure if initialHashSize is not a power of two
	*/
	void	   Init( const int initialHashSize, const int initialIndexSize );

	/*!
		\brief Allocates memory for the hash table and index chain with specified sizes.

		This function initializes the hash index structure by allocating memory for both the hash table and the index chain. It ensures the hash size is a power of two and properly initializes all
	   elements to indicate empty slots. The hash table is filled with -1 values to mark empty buckets, and the index chain is also initialized with -1 values to indicate unused entries. The hash mask
	   is set to hashSize - 1 to enable fast bitwise operations for hash lookups.

		\param newHashSize The size of the hash table to allocate, must be a power of two
		\param newIndexSize The size of the index chain to allocate
		\throws assertion failure if newHashSize is not a power of two
	*/
	void	   Allocate( const int newHashSize, const int newIndexSize );
};

ID_INLINE idHashIndex::idHashIndex()
{
	Init( DEFAULT_HASH_SIZE, DEFAULT_HASH_SIZE );
}

/*
================
idHashIndex::idHashIndex
================
*/
ID_INLINE idHashIndex::idHashIndex( const int initialHashSize, const int initialIndexSize )
{
	Init( initialHashSize, initialIndexSize );
}

ID_INLINE idHashIndex::~idHashIndex()
{
	Free();
}

ID_INLINE size_t idHashIndex::Allocated() const
{
	return hashSize * sizeof( int ) + indexSize * sizeof( int );
}

ID_INLINE size_t idHashIndex::Size() const
{
	return sizeof( *this ) + Allocated();
}

/*
================
idHashIndex::operator=
================
*/
ID_INLINE idHashIndex& idHashIndex::operator=( const idHashIndex& other )
{
	granularity = other.granularity;
	hashMask	= other.hashMask;
	lookupMask	= other.lookupMask;

	if( other.lookupMask == 0 ) {
		hashSize  = other.hashSize;
		indexSize = other.indexSize;
		Free();
	} else {
		if( other.hashSize != hashSize || hash == INVALID_INDEX ) {
			if( hash != INVALID_INDEX ) { delete[] hash; }
			hashSize = other.hashSize;
			hash	 = new( TAG_IDLIB_HASH ) int[hashSize];
		}
		if( other.indexSize != indexSize || indexChain == INVALID_INDEX ) {
			if( indexChain != INVALID_INDEX ) { delete[] indexChain; }
			indexSize  = other.indexSize;
			indexChain = new( TAG_IDLIB_HASH ) int[indexSize];
		}
		memcpy( hash, other.hash, hashSize * sizeof( hash[0] ) );
		memcpy( indexChain, other.indexChain, indexSize * sizeof( indexChain[0] ) );
	}

	return *this;
}

ID_INLINE void idHashIndex::Add( const int key, const int index )
{
	int h;

	assert( index >= 0 );
	if( hash == INVALID_INDEX ) {
		Allocate( hashSize, index >= indexSize ? index + 1 : indexSize );
	} else if( index >= indexSize ) {
		ResizeIndex( index + 1 );
	}
	h				  = key & hashMask;
	indexChain[index] = hash[h];
	hash[h]			  = index;
}

ID_INLINE void idHashIndex::Remove( const int key, const int index )
{
	int k = key & hashMask;

	if( hash == INVALID_INDEX ) { return; }
	if( hash[k] == index ) {
		hash[k] = indexChain[index];
	} else {
		for( int i = hash[k]; i != -1; i = indexChain[i] ) {
			if( indexChain[i] == index ) {
				indexChain[i] = indexChain[index];
				break;
			}
		}
	}
	indexChain[index] = -1;
}

ID_INLINE int idHashIndex::First( const int key ) const
{
	return hash[key & hashMask & lookupMask];
}

ID_INLINE int idHashIndex::Next( const int index ) const
{
	assert( index >= 0 && index < indexSize );
	return indexChain[index & lookupMask];
}

ID_INLINE void idHashIndex::InsertIndex( const int key, const int index )
{
	int i, max;

	if( hash != INVALID_INDEX ) {
		max = index;
		for( i = 0; i < hashSize; i++ ) {
			if( hash[i] >= index ) {
				hash[i]++;
				if( hash[i] > max ) { max = hash[i]; }
			}
		}
		for( i = 0; i < indexSize; i++ ) {
			if( indexChain[i] >= index ) {
				indexChain[i]++;
				if( indexChain[i] > max ) { max = indexChain[i]; }
			}
		}
		if( max >= indexSize ) { ResizeIndex( max + 1 ); }
		for( i = max; i > index; i-- ) {
			indexChain[i] = indexChain[i - 1];
		}
		indexChain[index] = -1;
	}
	Add( key, index );
}

ID_INLINE void idHashIndex::RemoveIndex( const int key, const int index )
{
	int i, max;

	Remove( key, index );
	if( hash != INVALID_INDEX ) {
		max = index;
		for( i = 0; i < hashSize; i++ ) {
			if( hash[i] >= index ) {
				if( hash[i] > max ) { max = hash[i]; }
				hash[i]--;
			}
		}
		for( i = 0; i < indexSize; i++ ) {
			if( indexChain[i] >= index ) {
				if( indexChain[i] > max ) { max = indexChain[i]; }
				indexChain[i]--;
			}
		}
		for( i = index; i < max; i++ ) {
			indexChain[i] = indexChain[i + 1];
		}
		indexChain[max] = -1;
	}
}

ID_INLINE void idHashIndex::Clear()
{
	// only clear the hash table because clearing the indexChain is not really needed
	if( hash != INVALID_INDEX ) { memset( hash, 0xff, hashSize * sizeof( hash[0] ) ); }
}

ID_INLINE void idHashIndex::Clear( const int newHashSize, const int newIndexSize )
{
	Free();
	hashSize  = newHashSize;
	indexSize = newIndexSize;
}

ID_INLINE int idHashIndex::GetHashSize() const
{
	return hashSize;
}

ID_INLINE int idHashIndex::GetIndexSize() const
{
	return indexSize;
}

ID_INLINE void idHashIndex::SetGranularity( const int newGranularity )
{
	assert( newGranularity > 0 );
	granularity = newGranularity;
}

ID_INLINE int idHashIndex::GenerateKey( const char* string, bool caseSensitive ) const
{
	if( caseSensitive ) {
		return ( idStr::Hash( string ) & hashMask );
	} else {
		return ( idStr::IHash( string ) & hashMask );
	}
}

ID_INLINE int idHashIndex::GenerateKey( const idVec3& v ) const
{
	return ( ( ( ( int )v[0] ) + ( ( int )v[1] ) + ( ( int )v[2] ) ) & hashMask );
}

ID_INLINE int idHashIndex::GenerateKey( const int n1, const int n2 ) const
{
	return ( ( n1 + n2 ) & hashMask );
}

ID_INLINE int idHashIndex::GenerateKey( const int n ) const
{
	return n & hashMask;
}

#endif /* !__HASHINDEX_H__ */
