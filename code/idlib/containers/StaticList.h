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

#ifndef __STATICLIST_H__
#define __STATICLIST_H__

#include "List.h"

//! Clears all elements from the static list, resetting its size to zero.
template<class type, int size>
class idStaticList
{
public:
	//! Constructs an empty static list.
	idStaticList();

	//! Creates a new list as a copy of an existing list.
	idStaticList( const idStaticList<type, size>& other );

	//! Initializes the list with elements from the provided initializer list.
	idStaticList( std::initializer_list<type> initializerList );
	~idStaticList<type, size>();

	//! Clears all elements from the static list, resetting its size to zero.
	void		Clear();

	//! Returns the number of elements in the list.
	int			Num() const;

	//! Constructs an empty idStaticList instance.

	int			Max() const;

	//! Initializes a static list with elements from an initializer list.

	//! Sets the number of elements in the static list to the specified value
	void		SetNum( int newnum );

	//! Sets the number of elements in the list and initializes any newly allocated elements to the given value
	void		SetNum( int newNum, const type& initValue );

	//! Returns the total size of allocated memory for the list.
	size_t		Allocated() const;

	//! Returns the total size of allocated memory for the list including the list type size
	size_t		Size() const;

	//! Returns the total memory used by the elements in the list
	size_t		MemoryUsed() const;

	//! Returns a constant reference to the element at the specified index in the static list.
	const type& operator[]( int index ) const;

	//! Returns a reference to the element at the specified index in the static list.
	type&		operator[]( int index );

	//! Returns a pointer to the first element in the list.
	type*		Ptr();

	//! Returns a pointer to the first element in the list
	const type* Ptr() const;

	//! Returns a pointer to a new element at the end of the list, or NULL if the list is full.
	type*		Alloc();

	//! Appends an element to the list and returns its index, or -1 if the list is full
	int			Append( const type& obj );

	//! Appends all elements from another list to this list.
	int			Append( const idStaticList<type, size>& other );

	//! Adds an element to the list only if it is not already present, returning its index.
	int			AddUnique( const type& obj );

	//! Inserts an element into the list at the specified index and returns the index where it was inserted.
	int			Insert( const type& obj, int index = 0 );

	//! Finds the index of the given element in the list
	int			FindIndex( const type& obj ) const;

	//! Finds and returns a pointer to the given element in the list
	type*		Find( type const& obj ) const;

	//! Finds the index of the first NULL pointer in the list.
	int			FindNull() const;

	//! Returns the index of the given object pointer within the list.
	int			IndexOf( const type* obj ) const;

	//! Removes the element at the specified index from the list.
	bool		RemoveIndex( int index );

	//! Removes the element at the specified index from the list.
	bool		RemoveIndexFast( int index );

	//! Removes the specified element from the list if it exists
	bool		Remove( const type& obj );

	//! Swaps the contents of this list with another list.
	void		Swap( idStaticList<type, size>& other );

	//! Deletes the contents of the list by calling delete on each element
	void		DeleteContents( bool clear );

	//! Sorts the elements in the list using the provided sorting algorithm.
	void		Sort( const idSort<type>& sort = idSort_QuickDefault<type>() );

private:
	int	 num;
	type list[size];

private:
	//! Resizes the list to the given number of elements.
	void Resize( int newsize );
};

template<class type, int size>
ID_INLINE idStaticList<type, size>::idStaticList()
{
	num = 0;
}

template<class type, int size>
ID_INLINE idStaticList<type, size>::idStaticList( const idStaticList<type, size>& other )
{
	*this = other;
}

template<class type, int size>
ID_INLINE idStaticList<type, size>::idStaticList( std::initializer_list<type> initializerList )
{
	SetNum( initializerList.size() );
	std::copy( initializerList.begin(), initializerList.end(), list );
}

/*
================
idStaticList<type,size>::~idStaticList<type,size>
================
*/
template<class type, int size>
ID_INLINE idStaticList<type, size>::~idStaticList()
{
}

template<class type, int size>
ID_INLINE void idStaticList<type, size>::Clear()
{
	num = 0;
}

template<class type, int size>
ID_INLINE void idStaticList<type, size>::Sort( const idSort<type>& sort )
{
	/*  if( list == NULL )  */
	if( Num() <= 0 ) // SRS - Instead of checking this->list for NULL, check this->Num() for empty list
	{
		return;
	}
	sort.Sort( Ptr(), Num() );
}

template<class type, int size>
ID_INLINE void idStaticList<type, size>::DeleteContents( bool clear )
{
	int i;

	for( i = 0; i < num; i++ ) {
		delete list[i];
		list[i] = NULL;
	}

	if( clear ) {
		Clear();
	} else {
		memset( list, 0, sizeof( list ) );
	}
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::Num() const
{
	return num;
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::Max() const
{
	return size;
}

template<class type, int size>
ID_INLINE size_t idStaticList<type, size>::Allocated() const
{
	return size * sizeof( type );
}

template<class type, int size>
ID_INLINE size_t idStaticList<type, size>::Size() const
{
	return sizeof( idStaticList<type, size> ) + Allocated();
}

template<class type, int size>
ID_INLINE size_t idStaticList<type, size>::MemoryUsed() const
{
	return num * sizeof( list[0] );
}

template<class type, int size>
ID_INLINE void idStaticList<type, size>::SetNum( int newnum )
{
	assert( newnum >= 0 );
	assert( newnum <= size );
	num = newnum;
}

template<class type, int size>
ID_INLINE void idStaticList<type, size>::SetNum( int newNum, const type& initValue )
{
	assert( newNum >= 0 );
	newNum = Min( newNum, size );
	assert( newNum <= size );
	for( int i = num; i < newNum; i++ ) {
		list[i] = initValue;
	}
	num = newNum;
}

template<class type, int size>
ID_INLINE const type& idStaticList<type, size>::operator[]( int index ) const
{
	assert( index >= 0 );
	assert( index < num );

	return list[index];
}

template<class type, int size>
ID_INLINE type& idStaticList<type, size>::operator[]( int index )
{
	assert( index >= 0 );
	assert( index < num );

	return list[index];
}

template<class type, int size>
ID_INLINE type* idStaticList<type, size>::Ptr()
{
	return &list[0];
}

template<class type, int size>
ID_INLINE const type* idStaticList<type, size>::Ptr() const
{
	return &list[0];
}

template<class type, int size>
ID_INLINE type* idStaticList<type, size>::Alloc()
{
	if( num >= size ) { return NULL; }

	return &list[num++];
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::Append( type const& obj )
{
	assert( num < size );
	if( num < size ) {
		list[num] = obj;
		num++;
		return num - 1;
	}

	return -1;
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::Insert( type const& obj, int index )
{
	assert( num < size );
	if( num >= size ) { return -1; }

	assert( index >= 0 );
	if( index < 0 ) {
		index = 0;
	} else if( index > num ) {
		index = num;
	}

	for( int i = num; i > index; --i ) {
		list[i] = list[i - 1];
	}

	num++;
	list[index] = obj;
	return index;
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::Append( const idStaticList<type, size>& other )
{
	int n = other.Num();

	if( num + n > size ) { n = size - num; }
	for( int i = 0; i < n; i++ ) {
		list[i + num] = other.list[i];
	}
	num += n;
	return Num();
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::AddUnique( type const& obj )
{
	int index = FindIndex( obj );
	if( index < 0 ) { index = Append( obj ); }

	return index;
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::FindIndex( type const& obj ) const
{
	for( int i = 0; i < num; i++ ) {
		if( list[i] == obj ) { return i; }
	}

	// Not found
	return -1;
}

template<class type, int size>
ID_INLINE type* idStaticList<type, size>::Find( type const& obj ) const
{
	int i = FindIndex( obj );
	if( i >= 0 ) { return ( type* )&list[i]; }

	return NULL;
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::FindNull() const
{
	for( int i = 0; i < num; i++ ) {
		if( list[i] == NULL ) { return i; }
	}

	// Not found
	return -1;
}

template<class type, int size>
ID_INLINE int idStaticList<type, size>::IndexOf( type const* objptr ) const
{
	int index;

	index = objptr - list;

	assert( index >= 0 );
	assert( index < num );

	return index;
}

template<class type, int size>
ID_INLINE bool idStaticList<type, size>::RemoveIndex( int index )
{
	int i;

	assert( index >= 0 );
	assert( index < num );

	if( ( index < 0 ) || ( index >= num ) ) { return false; }

	num--;
	for( i = index; i < num; i++ ) {
		list[i] = std::move( list[i + 1] );
	}

	return true;
}

template<typename _type_, int size>
ID_INLINE bool idStaticList<_type_, size>::RemoveIndexFast( int index )
{
	if( ( index < 0 ) || ( index >= num ) ) { return false; }

	num--;
	if( index != num ) { list[index] = list[num]; }

	return true;
}

template<class type, int size>
ID_INLINE bool idStaticList<type, size>::Remove( type const& obj )
{
	int index;

	index = FindIndex( obj );
	if( index >= 0 ) { return RemoveIndex( index ); }

	return false;
}

template<class type, int size>
ID_INLINE void idStaticList<type, size>::Swap( idStaticList<type, size>& other )
{
	idStaticList<type, size> temp = *this;
	*this						  = other;
	other						  = temp;
}

//! Debug tool to identify dynamic growth of idList instances.
void BreakOnListGrowth();

//! Placeholder function for break-on-list default behavior.
void BreakOnListDefault();

template<class type, int size>
ID_INLINE void idStaticList<type, size>::Resize( int newsize )
{
	assert( newsize >= 0 );

	// free up the list if no data is being reserved
	if( newsize <= 0 ) {
		Clear();
		return;
	}

	if( newsize == size ) {
		// not changing the size, so just exit
		return;
	}

	assert( newsize < size );
	return;
}
#endif /* !__STATICLIST_H__ */
