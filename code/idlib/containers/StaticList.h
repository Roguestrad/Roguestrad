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

/*!
	\class idStaticList
	\brief A fixed-size templated list class for managing collections of elements with efficient access and modification operations.

	The idStaticList class provides a fixed-capacity container for managing collections of elements of any type, with compile-time size limits. It is designed for performance-critical applications
   within the engine, offering direct memory access through Ptr() methods and efficient in-place operations. The class supports standard list operations including insertion, removal, searching, and
   sorting, with specialized methods for handling dynamic allocation and memory management. It is intended for use in scenarios where the maximum number of elements is known at compile time or when
   fixed-size containers are preferred for performance reasons. The class includes safety checks through assertions to prevent out-of-bounds access and maintains internal state tracking for element
   count and capacity. Memory management is handled internally, with explicit cleanup mechanisms provided through Clear and DeleteContents methods when dealing with dynamically allocated objects.

*/
template<class type, int size>
class idStaticList
{
public:
	/*!
		\brief Constructs an empty static list with zero elements.

		This constructor initializes the static list object to an empty state, setting the element count to zero. It is marked as inline for performance reasons, indicating that the implementation
	   will be embedded directly into calling code during compilation.

	*/
	idStaticList();

	/*!
		\brief Constructs a new list as a copy of the provided list.

		This constructor initializes a new list by copying all elements from the provided list. It uses the assignment operator internally to perform the copy operation, ensuring that the new list
	   contains the same elements and size as the source list.

		\param other The list to be copied
	*/
	idStaticList( const idStaticList<type, size>& other );

	/*!
		\brief Initializes a static list with elements from an initializer list.

		Constructs a static list by copying elements from the provided initializer list. The size of the list is set to match the number of elements in the initializer list, and then all elements are
	   copied into the internal array.

		\param initializerList The initializer list containing the elements to populate the static list with
	*/
	idStaticList( std::initializer_list<type> initializerList );

	/*!
		\brief Destructor for the idStaticList template class that cleans up any dynamically allocated memory.

		The destructor for idStaticList is marked as ID_INLINE, indicating it is intended to be inlined for performance. It performs no explicit operations in its implementation, suggesting that the
	   list manages its own memory and cleanup is handled by the underlying implementation or compiler.

	*/
	~idStaticList<type, size>();

	//! Clears all elements from the static list, resetting its size to zero.
	void		Clear();

	//! Returns the number of elements in the list
	int			Num() const;

	//! Returns the maximum number of elements that can be stored in the static list
	int			Max() const;

	/*!
		\brief Sets the number of elements in the static list to the specified value

		This function updates the number of elements currently stored in the static list. It asserts that the new number is non-negative and does not exceed the maximum size of the list. The function
	   does not resize the underlying storage, only updates the logical count of elements.

		\param newnum The new number of elements in the list
		\throws Assertion failure if newnum is negative or exceeds the maximum size of the list
	*/
	void		SetNum( int newnum );

	/*!
		\brief Sets the number of elements in the list and initializes any newly allocated elements to the given value

		This function adjusts the number of elements in the static list to the specified count. If the new count is larger than the current size, the additional elements are initialized with the
	   provided value. If the new count is smaller, the list is truncated without changing the allocated memory. The function ensures that the new number of elements does not exceed the maximum size
	   of the static list.

		\param newNum The new number of elements in the list
		\param initValue The value to initialize newly allocated elements with
		\throws assertion failure if newNum is negative or exceeds the maximum size
	*/
	void		SetNum( int newNum, const type& initValue );

	//! Returns the total size in bytes of the allocated memory for the list
	size_t		Allocated() const;

	//! Returns the total size in bytes of the allocated memory for the list including the list type
	size_t		Size() const;

	//! Returns the total number of bytes used by the elements currently stored in the list
	size_t		MemoryUsed() const;

	const type& operator[]( int index ) const;
	type&		operator[]( int index );

	/*!
		\brief Returns a pointer to the first element of the internal array

		This function provides direct access to the underlying memory buffer of the static list by returning a pointer to the first element. It is typically used when direct memory access is needed
	   for performance reasons or when interfacing with functions that expect a pointer to an array of elements.

		\return Pointer to the first element in the internal array storage
	*/
	type*		Ptr();

	/*!
		\brief Returns a pointer to the first element in the list

		This function provides direct access to the underlying memory buffer of the static list by returning a pointer to the first element. It is marked as inline and const, indicating it does not
	   modify the list and can be inlined for performance. The returned pointer points to the beginning of the internal storage array managed by the idStaticList template class.

		\return A constant pointer to the first element in the list
	*/
	const type* Ptr() const;

	/*!
		\brief Returns a pointer to a new data element at the end of the list, or NULL if the list is full.

		This function allocates a new element in the static list and returns a pointer to it. The element is placed at the next available position in the list. If the list is already at its maximum
	   capacity, the function returns NULL. The function is typically used when building up data structures incrementally, such as when parsing declaration files where new body or constraint objects
	   are added to lists.

		\return A pointer to the newly allocated element in the list, or NULL if the list is full
	*/
	type*		Alloc();

	/*!
		\brief Appends an element to the list and returns its index, or -1 if the list is full

		This function adds a new element to the static list if there is available space. It performs an assertion check to ensure the list has not exceeded its maximum size. If the list is not full,
	   the element is copied to the next available position in the list, the count is incremented, and the index of the newly added element is returned. If the list is full, the function returns -1 to
	   indicate failure

		\param obj the element to append to the list
		\return the index of the newly appended element, or -1 if the list is full
		\throws assertion failure if the list is full when attempting to append
	*/
	int			Append( const type& obj );

	/*!
		\brief Appends all elements from another list to this list.

		This function appends all elements from the provided list 'other' to the current list. If the total number of elements would exceed the capacity of the list, only the remaining available slots
	   are filled. The function returns the new number of elements in the list after the append operation.

		\param other The list whose elements are to be appended to this list.
		\return The total number of elements in the list after appending the elements from the other list.
	*/
	int			Append( const idStaticList<type, size>& other );

	/*!
		\brief Adds an element to the list only if it is not already present, returning its index.

		This function checks if the specified element already exists in the list using FindIndex. If the element is not found, it appends the element to the list and returns the index of the newly
	   added element. If the element already exists, it simply returns the index of the existing element. This ensures that duplicate elements are not added to the list.

		\param obj The element to add to the list if it is not already present
		\return The index of the element in the list, either existing or newly added
	*/
	int			AddUnique( const type& obj );

	/*!
		\brief Inserts an element into the list at the specified index and returns the index where it was inserted

		This function inserts a new element into the static list at the given index position. If the index is out of bounds, it will be clamped to valid range. The function shifts all elements after
	   the insertion point to the right to make space for the new element. The function will return -1 if the list is already at maximum capacity. The insertion operation maintains the order of
	   elements in the list.

		\param obj The element to be inserted into the list
		\param index The index at which to insert the element, defaults to 0 if not specified
		\return The index where the element was inserted, or -1 if the insertion failed due to insufficient space
	*/
	int			Insert( const type& obj, int index = 0 );

	/*!
		\brief Finds the index of the given element in the list

		This function searches for the specified element in the list and returns its index if found. If the element is not found, it returns -1. The search is performed linearly through the list
	   elements up to the current number of elements stored in the list

		\param obj the element to search for in the list
		\return the index of the element if found, or -1 if the element is not present in the list
	*/
	int			FindIndex( const type& obj ) const;

	/*!
		\brief Finds and returns a pointer to the given element in the list

		This function searches for the specified element in the static list and returns a pointer to it if found. It uses the FindIndex method to locate the element and then returns a pointer to the
	   element at that index. If the element is not found, it returns NULL

		\param obj The element to search for in the list
		\return A pointer to the found element, or NULL if the element is not present in the list
	*/
	type*		Find( type const& obj ) const;

	/*!
		\brief Finds the index of the first NULL pointer in the list.

		This function iterates through the list and returns the index of the first element that is NULL. If no NULL pointer is found, it returns -1.

		\return The index of the first NULL pointer in the list, or -1 if no NULL pointer is found.
	*/
	int			FindNull() const;

	/*!
		\brief Returns the index of the pointer to an element in the list

		This function calculates the index of a given object pointer within the list by subtracting the base address of the list from the object pointer. It asserts that the calculated index is within
	   valid bounds.

		\param obj pointer to an element in the list
		\return the index of the pointer to an element in the list
		\throws assertion failure if the index is out of bounds
	*/
	int			IndexOf( const type* obj ) const;

	/*!
		\brief Removes the element at the specified index from the static list

		This function removes an element from the static list by shifting all subsequent elements one position to the left. The function first validates that the index is within the valid range [0,
	   num). If the index is invalid, the function returns false without making any changes. Otherwise, it decrements the list size and performs a move operation for each element from the specified
	   index to the end of the list. The function returns true if the removal was successful

		\param index The index of the element to remove from the list
		\return True if the element at the specified index was successfully removed, false if the index was out of bounds
	*/
	bool		RemoveIndex( int index );

	/*!
		\brief Removes the element at the specified index from the list.

		This function removes the element at the given index by replacing it with the last element in the list and then decrementing the size. This approach avoids shifting all subsequent elements,
	   making it more efficient than a standard removal. The function returns false if the index is out of bounds.

		\param index The index of the element to remove
		\return True if the element was successfully removed, false if the index is out of bounds.
	*/
	bool		RemoveIndexFast( int index );

	/*!
		\brief Removes the first occurrence of the specified object from the list

		This function searches for the first occurrence of the given object in the static list and removes it if found. The search is performed using the FindIndex method, which locates the object's
	   index in the list. If the object is found, the RemoveIndex method is called to remove it from the list. If the object is not found, the function returns false without making any changes to the
	   list.

		\param obj The object to be removed from the list
		\return True if the object was found and removed, false otherwise
	*/
	bool		Remove( const type& obj );

	/*!
		\brief Swaps the contents of this list with another list.

		This function exchanges all elements between this list and the provided other list. It uses a temporary list to perform the swap operation, ensuring that the contents of both lists are
	   correctly transferred. The function is marked as ID_INLINE, indicating it should be inlined for performance reasons.

		\param other The other list whose contents will be swapped with this list
	*/
	void		Swap( idStaticList<type, size>& other );

	/*!
		\brief Deletes all dynamically allocated objects in the list and optionally clears the list.

		This function iterates through all elements in the list and deletes each dynamically allocated object. After deletion, it sets the list element pointers to NULL. If the clear parameter is
	   true, the list is cleared entirely by calling the Clear method. Otherwise, the list memory is reset to zero using memset.

		\param clear If true, clears the entire list after deleting contents; if false, only sets pointers to NULL and resets memory to zero
	*/
	void		DeleteContents( bool clear );

	/*!
		\brief Sorts the elements in the list using the provided sorting function

		This function sorts the elements contained in the static list using the specified sorting function. It first checks if the list is empty or has only one element, and if so, returns immediately
	   without performing any sorting. Otherwise, it calls the Sort method of the provided sorting function, passing the pointer to the list data and the number of elements in the list

		\param sort The sorting function to use for sorting the list elements
	*/
	void		Sort( const idSort<type>& sort = idSort_QuickDefault<type>() );

private:
	int	 num;
	type list[size];

private:
	/*!
		\brief Resizes the list to the specified number of elements

		This function adjusts the size of the static list to the given number of elements. If the new size is zero or negative, the list is cleared. If the new size equals the current size, no changes
	   are made. The function asserts that the new size is less than the maximum size of the list

		\param newsize The desired number of elements in the list
		\throws assertion failure if newsize is negative or greater than or equal to the maximum size
	*/
	void Resize( int newsize );
};

template<class type, int size>
ID_INLINE idStaticList<type, size>::idStaticList()
{
	num = 0;
}

/*
================
idStaticList<type,size>::idStaticList( const idStaticList<type,size> &other )
================
*/
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

/*
================
idStaticList<type,size>::operator[] const

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template<class type, int size>
ID_INLINE const type& idStaticList<type, size>::operator[]( int index ) const
{
	assert( index >= 0 );
	assert( index < num );

	return list[index];
}

/*
================
idStaticList<type,size>::operator[]

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
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

/*!
	\brief Debug tool to identify uses of idList that dynamically grow.

	This function serves as a debug utility to detect instances where idList objects are growing dynamically instead of being pre-sized. The intention is to encourage explicit sizing of lists in
   shipping builds to avoid performance penalties associated with dynamic allocation during list growth.

*/
void BreakOnListGrowth();

/*!
	\brief Function that serves as a default break point for list operations.

	This function is a placeholder or default implementation for breaking on list operations. It is typically used in debugging scenarios to provide a breakpoint location when list resizing or other
   operations occur. The function does not perform any operations itself but is intended to be overridden or replaced with a more specific implementation when needed.

*/
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
