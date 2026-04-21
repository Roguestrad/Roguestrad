/*****************************************************************************
The Dark Mod GPL Source Code

This file is part of the The Dark Mod Source Code, originally based
on the Doom 3 GPL Source Code as published in 2011.

The Dark Mod Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Project: The Dark Mod (http://www.thedarkmod.com/)

******************************************************************************/

#ifndef __FLEXLIST_H__
#define __FLEXLIST_H__

/**
 * Array with hybrid storage.
 * Contains automatic storage, which is used as long as count <= N.
 * Allocates dynamic memory from heap when count exceeds N.
 *
 * As in all other idlib containers, all elements are always in constructed state.
 * This includes the N elements in automatic storage.
 * BEWARE: do not use it for elements with nontrivial constructor,
 *         or you will get serious performance problem!
 *
 * Originally implemented to substitute numerous local arrays of size MAX_GENTITIES.
 * That's why its interface is a bit limited compared to idList.
 *
 * Contains self-reference, hence is not trivially relocatable.
 */

template<class type, int N>
class idFlexList
{
public:
	//! Destroys the flex list and frees the allocated memory if it was dynamically allocated.
	~idFlexList()
	{
		if( list != autoStore ) { delete[] list; }
	}

	//! Initializes a new instance of the idFlexList class with default values.
	idFlexList()
	{
		list = autoStore;
		num	 = 0;
		size = N;
	}

	//! Clears the contents of the flex list, resetting its size to zero.
	void Clear() { num = 0; }

	//! Clears the flex list and frees dynamically allocated memory if it's not using the auto storage.
	void ClearFree()
	{
		if( list != autoStore ) { delete[] list; }
		list = autoStore;
		num	 = 0;
		size = N;
	}

	//! Sets the number of elements in the list to the specified value.
	void SetNum( int newNum )
	{
		if( newNum > size ) {
			int newSize = newNum;
			if( newSize < 2 * size ) {
				newSize = 2 * size; // ensure exponential growth
			}
			Grow( newSize );
		}
		num = newNum;
	}

	//! Returns the number of elements in the flex list.
	ID_FORCE_INLINE int			Num() const { return num; }

	//! Returns the number of allocated elements in the flex list.
	int							NumAllocated() const { return size; }

	//! Returns a pointer to the internal array of the flex list
	ID_FORCE_INLINE type*		Ptr() { return list; }

	//! Returns a pointer to the internal array of the flex list
	ID_FORCE_INLINE const type* Ptr() const { return list; }

	//! Provides read-only access to an element at the specified index in the flex list.
	ID_FORCE_INLINE const type& operator[]( int index ) const
	{
		assert( unsigned( index ) < unsigned( num ) );
		return list[index];
	}

	//! Returns a reference to the element at the specified index in the flexible list.
	ID_FORCE_INLINE type& operator[]( int index )
	{
		assert( unsigned( index ) < unsigned( num ) );
		return list[index];
	}

	//! Returns a reference to the last element in the list.
	ID_FORCE_INLINE const type& Last() const
	{
		assert( num > 0 );
		return list[num - 1];
	}

	//! Returns a reference to the last element in the list.
	ID_FORCE_INLINE type& Last()
	{
		assert( num > 0 );
		return list[num - 1];
	}

	//! Adds a new element to the list and returns its index.
	int AddGrow( type obj )
	{
		if( num == size ) { Grow( 2 * size ); }
		int idx	  = num++;
		list[idx] = obj;
		return idx;
	}

	//! Removes and returns the last element from the list.
	ID_FORCE_INLINE type Pop() { return list[--num]; }

	//! Appends k elements from array arr to the flex list
	void				 Append( int k, const type* arr )
	{
		int base = num;
		SetNum( base + k );
		for( int i = 0; i < k; i++ ) {
			list[base + i] = arr[i];
		}
	}

private:
	//! Resizes the internal array to accommodate the specified new size.
	void Grow( int newSize )
	{
		type* newList = new type[newSize];
		for( int i = 0; i < num; i++ ) {
			newList[i] = list[i];
		}
		if( list != autoStore ) { delete[] list; }
		list = newList;
		size = newSize;
	}

	// noncopyable!
	// moving automatic storage around is not worth it
	idFlexList( const idFlexList& )			   = delete;
	idFlexList& operator=( const idFlexList& ) = delete;

	int			num;
	int			size;
	type*		list;
	type		autoStore[N];
};

#endif
