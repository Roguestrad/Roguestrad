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

#ifndef __STRPOOL_H__
#define __STRPOOL_H__

/*
===============================================================================

	idStrPool

===============================================================================
*/

class idStrPool;

/*!
	\class idPoolStr
	\brief A string pool implementation that tracks user references and manages string memory allocation.

	The idPoolStr class extends idStr to provide string pooling functionality with user reference counting. It is designed to manage memory for strings that are frequently allocated and deallocated,
   reducing memory fragmentation and improving performance. The class maintains a count of users to ensure proper resource cleanup, with the destructor asserting that no users remain when the object
   is destroyed. The string pool stores strings in allocated memory that can be tracked through the Allocated() and Size() methods. The GetPool() method provides access to the underlying string pool
   from which the string was allocated, allowing for proper management of shared string resources.

*/
class idPoolStr : public idStr
{
	friend class idStrPool;

public:
	/*!
		\brief Initializes an idPoolStr object with zero users.

		This constructor initializes the idPoolStr object by setting the numUsers member variable to zero. It is designed to create a new instance of the idPoolStr class in a consistent initial state.

	*/
	idPoolStr() { numUsers = 0; }

	/*!
		\brief Destructor for idPoolStr that asserts no users remain.

		The destructor for idPoolStr performs a sanity check using an assertion to ensure that the number of users is zero. This indicates that all references to the resource managed by this pool have
	   been properly released before the object is destroyed.

	*/
	~idPoolStr() { assert( numUsers == 0 ); }

	//! Returns the total size in bytes of memory that has been allocated for the string
	size_t			 Allocated() const { return idStr::Allocated(); }

	//! Returns the total size in bytes of the allocated memory for this string pool.
	size_t			 Size() const { return sizeof( *this ) + Allocated(); }

	//! Returns a pointer to the pool this string was allocated from.
	const idStrPool* GetPool() const { return pool; }

private:
	idStrPool*	pool;
	mutable int numUsers;
};

/*!
	\class idStrPool
	\brief A string pool implementation for efficient storage and management of shared strings.

	The idStrPool class provides a mechanism for storing and managing strings in a memory-efficient manner by reusing identical strings. It maintains a hash table for fast lookups and supports both
   case-sensitive and case-insensitive string comparisons. The pool tracks reference counts for strings to determine when they can be safely freed. This design enables efficient string handling in
   engines where many identical strings may exist, reducing memory footprint and improving performance through string reuse. The class supports allocation of new strings, freeing of strings when no
   longer referenced, and copying of strings between different pools.

*/
class idStrPool
{
public:
	/*!
		\brief Initializes a new instance of the idStrPool class with case sensitivity enabled.

		The constructor initializes the idStrPool object and sets the caseSensitive flag to true, indicating that string comparisons will be case-sensitive by default.

	*/
	idStrPool() { caseSensitive = true; }

	/*!
		\brief Sets whether the string pool performs case-sensitive comparisons.

		This function configures the string pool to either perform case-sensitive or case-insensitive string comparisons. When set to true, string comparisons will consider the case of characters.
	   When set to false, comparisons will treat uppercase and lowercase characters as equivalent.

		\param caseSensitive True to enable case-sensitive comparisons, false otherwise
	*/
	void			 SetCaseSensitive( bool caseSensitive );

	//! Returns the total number of strings contained in the string pool.
	int				 Num() const { return pool.Num(); }

	//! Returns the total number of bytes allocated by the string pool, including internal structures and all stored strings.
	size_t			 Allocated() const;

	//! Returns the total number of bytes occupied by all strings and internal structures within the string pool
	size_t			 Size() const;

	const idPoolStr* operator[]( int index ) const { return pool[index]; }

	/*!
		\brief Allocates and returns a pooled string with the specified value, reusing existing strings when possible.

		This function attempts to find an existing string in the pool that matches the provided string. If a match is found, the reference count of the existing string is incremented and it is
	   returned. If no match is found, a new string is created, initialized with the provided string value, added to the pool, and returned. The function checks for case sensitivity when comparing
	   strings.

		\param string The string value to allocate in the pool
		\return A pointer to the pooled string, either existing or newly created
	*/
	const idPoolStr* AllocString( const char* string );

	/*!
		\brief Frees a string from the string pool if its reference count drops to zero.

		This function decrements the reference count of a string in the pool. If the reference count reaches zero, the string is removed from the pool and its memory is deallocated. The function first
	   checks if the pool is valid and then verifies that the string belongs to this pool. It performs a hash lookup to find the string in the pool and removes it if the reference count has reached
	   zero.

		\param poolStr Pointer to the idPoolStr object to be freed from the pool
		\throws assertion failure if the string does not belong to this pool or if the reference count is invalid
	*/
	void			 FreeString( const idPoolStr* poolStr );

	/*!
		\brief Returns a reference to a string from this pool, increasing the user count if it's already in this pool, or allocating a new copy if it's from another pool.

		This function is used to manage string references within a string pool. If the input string is already part of this pool, it simply increments the user count and returns the same reference. If
	   the string belongs to a different pool, it allocates a new copy of the string within this pool and returns the reference to the new copy. This ensures that references to strings are managed
	   efficiently across different pools.

		\param poolStr Pointer to the string to be copied or referenced from this pool
		\return Pointer to the string in this pool, either the original if it was already here, or a newly allocated copy if it came from another pool
		\throws assertion failure if the input string has less than one user
	*/
	const idPoolStr* CopyString( const idPoolStr* poolStr );

	//! Clears all strings from the string pool and resets the hash table.
	void			 Clear();

private:
	bool			   caseSensitive;
	idList<idPoolStr*> pool;
	idHashIndex		   poolHash;
};

ID_INLINE void idStrPool::SetCaseSensitive( bool caseSensitive )
{
	this->caseSensitive = caseSensitive;
}

ID_INLINE const idPoolStr* idStrPool::AllocString( const char* string )
{
	int		   i, hash;
	idPoolStr* poolStr;

	hash = poolHash.GenerateKey( string, caseSensitive );
	if( caseSensitive ) {
		for( i = poolHash.First( hash ); i != -1; i = poolHash.Next( i ) ) {
			if( pool[i]->Cmp( string ) == 0 ) {
				pool[i]->numUsers++;
				return pool[i];
			}
		}
	} else {
		for( i = poolHash.First( hash ); i != -1; i = poolHash.Next( i ) ) {
			if( pool[i]->Icmp( string ) == 0 ) {
				pool[i]->numUsers++;
				return pool[i];
			}
		}
	}

	poolStr							= new( TAG_IDLIB_STRING ) idPoolStr;
	*static_cast<idStr*>( poolStr ) = string;
	poolStr->pool					= this;
	poolStr->numUsers				= 1;
	poolHash.Add( hash, pool.Append( poolStr ) );
	return poolStr;
}

ID_INLINE void idStrPool::FreeString( const idPoolStr* poolStr )
{
	int i, hash;

	// SRS - check for empty idStrPool and return to prevent segfaulting on shutdown
	if( pool.Num() <= 0 ) { return; }

	assert( poolStr->pool == this );
	assert( poolStr->numUsers >= 1 ); // SRS - Reestablish assertion

	poolStr->numUsers--;
	if( poolStr->numUsers <= 0 ) {
		hash = poolHash.GenerateKey( poolStr->c_str(), caseSensitive );
		if( caseSensitive ) {
			for( i = poolHash.First( hash ); i != -1; i = poolHash.Next( i ) ) {
				if( pool[i]->Cmp( poolStr->c_str() ) == 0 ) { break; }
			}
		} else {
			for( i = poolHash.First( hash ); i != -1; i = poolHash.Next( i ) ) {
				if( pool[i]->Icmp( poolStr->c_str() ) == 0 ) { break; }
			}
		}
		assert( i != -1 );
		assert( pool[i] == poolStr );
		delete pool[i];
		pool.RemoveIndex( i );
		poolHash.RemoveIndex( hash, i );
	}
}

ID_INLINE const idPoolStr* idStrPool::CopyString( const idPoolStr* poolStr )
{
	assert( poolStr->numUsers >= 1 );

	if( poolStr->pool == this ) {
		// the string is from this pool so just increase the user count
		poolStr->numUsers++;
		return poolStr;
	} else {
		// the string is from another pool so it needs to be re-allocated from this pool.
		return AllocString( poolStr->c_str() );
	}
}

ID_INLINE void idStrPool::Clear()
{
	int i;

	for( i = 0; i < pool.Num(); i++ ) {
		pool[i]->numUsers = 0;
	}
	pool.DeleteContents( true );
	poolHash.Free();
}

ID_INLINE size_t idStrPool::Allocated() const
{
	int	   i;
	size_t size;

	size = pool.Allocated() + poolHash.Allocated();
	for( i = 0; i < pool.Num(); i++ ) {
		size += pool[i]->Allocated();
	}
	return size;
}

ID_INLINE size_t idStrPool::Size() const
{
	int	   i;
	size_t size;

	size = pool.Size() + poolHash.Size();
	for( i = 0; i < pool.Num(); i++ ) {
		size += pool[i]->Size();
	}
	return size;
}

#endif /* !__STRPOOL_H__ */
