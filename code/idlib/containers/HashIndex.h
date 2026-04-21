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
	\brief A hash index implementation that manages key-value mappings with dynamic resizing capabilities.

	The idHashIndex class provides a dynamic hash table structure for mapping integer keys to integer indices. It supports operations to add, remove, and query entries while maintaining internal
   chains for handling hash collisions. The class can be initialized with specific hash and index sizes, and offers methods to resize the hash table and index structure dynamically. It includes
   functionality for generating hash keys from various data types including strings, vectors, and integers. The implementation tracks memory usage through allocated size reporting methods. The class
   handles sparse hash entries and provides methods to iterate through hash chains, making it suitable for efficient lookups and modifications in performance-critical applications.

*/
class idHashIndex
{
public:
	static const int NULL_INDEX = -1;

	//! Initializes a new idHashIndex object with default hash size parameters.
	idHashIndex();

	//! Constructs an idHashIndex object with specified initial hash and index sizes.
	idHashIndex( const int initialHashSize, const int initialIndexSize );

	//! Destructor for the idHashIndex class that frees all allocated memory.
	~idHashIndex();

	//! Returns the total size of allocated memory for the hash index
	size_t		 Allocated() const;

	//! Returns the total size of allocated memory for the hash index including the hash index type itself.
	size_t		 Size() const;

	//! Assigns the contents of another hash index instance to this instance.
	idHashIndex& operator=( const idHashIndex& other );

	//! Adds an index to the hash table using the specified key.
	void		 Add( const int key, const int index );

	//! Removes an index from the hash table based on the provided key and index.
	void		 Remove( const int key, const int index );

	//! Returns the first index from the hash for the given key, or -1 if the hash entry is empty
	int			 First( const int key ) const;

	//! Returns the next index in the hash chain or -1 if at the end of the chain
	int			 Next( const int index ) const;

	//! Returns the first index in the hash table for the given key.
	int			 GetFirst( const int key ) const { return First( key ); }

	//! Returns the next index in the hash chain for the given index
	int			 GetNext( const int index ) const { return Next( index ); }

	//! Inserts a new entry into the hash index at the specified position and updates all subsequent indexes.
	void		 InsertIndex( const int key, const int index );

	//! Removes an entry from the index and hash, decreasing all indexes greater than or equal to the specified index.
	void		 RemoveIndex( const int key, const int index );

	//! Clears the hash table entries.
	void		 Clear();

	//! Clears and resizes the hash index with the specified hash and index sizes.
	void		 Clear( const int newHashSize, const int newIndexSize );

	//! Frees the memory allocated by the hash index.
	void		 Free();

	//! Returns the size of the hash table used by the hash index.
	int			 GetHashSize() const;

	//! Returns the size of the hash index.
	int			 GetIndexSize() const;

	//! Sets the granularity for the hash index.
	void		 SetGranularity( const int newGranularity );

	//! Resizes the hash index to the specified new size while preserving existing data.
	void		 ResizeIndex( const int newIndexSize );

	//! Returns a value between 0 and 100 representing the spread of items across the hash table buckets.
	int			 GetSpread() const;

	//! Generates a hash key for a string using either case-sensitive or case-insensitive hashing.
	int			 GenerateKey( const char* string, bool caseSensitive = true ) const;

	//! Returns a hash key for a vector value
	int			 GenerateKey( const idVec3& v ) const;

	//! Generates a hash key for two integers using bitwise AND with the hash mask.
	int			 GenerateKey( const int n1, const int n2 ) const;

	//! Returns a hash key for a given integer using the internal hash mask
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

	//! Initializes the hash index with specified hash and index sizes.
	void	   Init( const int initialHashSize, const int initialIndexSize );

	//! Allocates hash and index chains for the specified hash and index sizes.
	void	   Allocate( const int newHashSize, const int newIndexSize );
};

ID_INLINE idHashIndex::idHashIndex()
{
	Init( DEFAULT_HASH_SIZE, DEFAULT_HASH_SIZE );
}

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
