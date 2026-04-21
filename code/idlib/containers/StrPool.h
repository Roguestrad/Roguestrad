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
	\brief A string class that manages memory allocation within a shared string pool.
*/
class idPoolStr : public idStr
{
	friend class idStrPool;

public:
	//! Initializes an idPoolStr object with zero users.
	idPoolStr() { numUsers = 0; }
	~idPoolStr() { assert( numUsers == 0 ); }

	//! Returns the total size of allocated memory for the string.
	size_t			 Allocated() const { return idStr::Allocated(); }

	//! Returns the total size of allocated memory for the string pool including the size of the pool type itself
	size_t			 Size() const { return sizeof( *this ) + Allocated(); }

	//! Returns a pointer to the pool this string was allocated from.
	const idStrPool* GetPool() const { return pool; }

private:
	idStrPool*	pool;
	mutable int numUsers;
};

/*!
	\class idStrPool
	\brief A string pool implementation that manages and reuses string instances to optimize memory usage and comparison operations.

	The idStrPool class provides a mechanism for efficiently managing string objects by storing them in a pool and reusing existing instances when possible. This approach reduces memory fragmentation
   and improves performance by avoiding duplicate string allocations. The pool supports case-sensitive and case-insensitive comparisons based on configuration. Strings in the pool are
   reference-counted, allowing for safe sharing and automatic cleanup when no longer referenced. The class provides methods to allocate, copy, and free pooled strings, as well as to query pool
   statistics such as the number of strings and memory usage. The internal hash table structure enables fast lookups of existing strings, and the pool can be cleared to reset all stored strings and
   their associated data structures.

*/
class idStrPool
{
public:
	//! Initializes a new instance of the idStrPool class with case sensitivity enabled.
	idStrPool() { caseSensitive = true; }

	//! Sets whether the string pool performs case-sensitive comparisons.
	void			 SetCaseSensitive( bool caseSensitive );

	//! Returns the number of strings in the string pool
	int				 Num() const { return pool.Num(); }

	//! Returns the total amount of memory allocated by the string pool and its internal data structures.
	size_t			 Allocated() const;

	//! Returns the total memory size occupied by all strings in the string pool
	size_t			 Size() const;

	//! Returns a pointer to the idPoolStr object at the specified index in the pool.
	const idPoolStr* operator[]( int index ) const { return pool[index]; }

	//! Returns a pointer to a pooled string, reusing existing strings when possible.
	const idPoolStr* AllocString( const char* string );

	//! Frees a string from the pool if its reference count reaches zero.
	void			 FreeString( const idPoolStr* poolStr );

	//! Returns a reference to the given pool string, increasing its user count if it belongs to this pool, otherwise allocates a new copy from this pool.
	const idPoolStr* CopyString( const idPoolStr* poolStr );

	//! Clears all entries from the string pool and resets the hash table.
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
