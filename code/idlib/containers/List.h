/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Stephen Pridham

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

#ifndef __LIST_H__
#define __LIST_H__

#include <new>
#include <initializer_list>
#include <algorithm> // SRS - Needed for clang 14 so std::copy() is defined

/*!
	\brief Creates and initializes a new array of _type_ objects with optional zero initialization

	This function allocates memory for an array of _type_ objects and constructs each object using its default constructor. If zeroBuffer is true, the memory is cleared before allocation. The function
   returns a pointer to the allocated and initialized array

	\param num number of _type_ objects to allocate and initialize
	\param zeroBuffer if true, initializes the allocated memory to zero before constructing objects
	\return pointer to the newly allocated and initialized array of _type_ objects
	\throws Memory allocation failure if the underlying Mem_Alloc or Mem_ClearedAlloc calls fail
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE void* idListArrayNew( int num, bool zeroBuffer )
{
	_type_* ptr = NULL;
	if( zeroBuffer ) {
		ptr = ( _type_* )Mem_ClearedAlloc( sizeof( _type_ ) * num, _tag_ );
	} else {
		ptr = ( _type_* )Mem_Alloc( sizeof( _type_ ) * num, _tag_ );
	}
	for( int i = 0; i < num; i++ ) {
		new( &ptr[i] ) _type_;
	}
	return ptr;
}

/*!
	\brief Calls destructors on all elements in the array and frees the memory

	This function iterates through the specified number of elements in the array and calls their destructors before freeing the allocated memory. It is typically used to clean up dynamically allocated
   arrays of objects that require explicit destructor calls

	\param ptr Pointer to the array of elements to be destructed and freed
	\param num Number of elements in the array
*/
template<typename _type_>
ID_INLINE void idListArrayDelete( void* ptr, int num )
{
	// Call the destructors on all the elements
	for( int i = 0; i < num; i++ ) {
		( ( _type_* )ptr )[i].~_type_();
	}
	Mem_Free( ptr );
}

/*!
	\brief Resizes an array list, moving existing elements to a new memory block

	This function resizes an array list by allocating a new memory block of the specified size, copying existing elements from the old block to the new block, and then deleting the old memory block.
   The function uses move semantics to efficiently transfer elements when possible.

	\param voldptr Pointer to the old array memory block
	\param oldNum Number of elements in the old array
	\param newNum Number of elements in the new array
	\param zeroBuffer Flag indicating whether to zero the new buffer
	\return Pointer to the new array memory block
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE void* idListArrayResize( void* voldptr, int oldNum, int newNum, bool zeroBuffer )
{
	_type_* oldptr = ( _type_* )voldptr;
	_type_* newptr = NULL;
	if( newNum > 0 ) {
		newptr		= ( _type_* )idListArrayNew<_type_, _tag_>( newNum, zeroBuffer );
		int overlap = Min( oldNum, newNum );
		for( int i = 0; i < overlap; i++ ) {
			// newptr[i] = oldptr[i];
			newptr[i] = std::move( oldptr[i] );
		}
	}
	idListArrayDelete<_type_>( voldptr, oldNum );
	return newptr;
}

/*!
	\brief Creates and returns a new element of type T using dynamic allocation

	This function allocates memory for a new object of type T using the new operator and returns a pointer to the newly created object. It is typically used in conjunction with idList to dynamically
   add elements to the list. The function is marked as inline to optimize performance by avoiding function call overhead.

	\return A pointer to the newly created object of type T
*/
template<class type>
ID_INLINE type* idListNewElement()
{
	return new type;
}

template<typename _type_, memTag_t _tag_ = TAG_IDLIB_LIST>
class idList
{
public:
	typedef int	   cmp_t( const _type_*, const _type_* );
	typedef _type_ new_t();

	/*!
		\brief Constructs an idList with the specified granularity for memory allocation.

		The constructor initializes the idList with a given granularity which determines how many elements are allocated at a time. The list starts empty and the memory tag is set to the template
	   parameter _tag_. The assert statement ensures that the granularity is positive.

		\param newgranularity The number of elements to allocate at a time for memory management
		\throws assertion failure if newgranularity is not greater than zero
	*/
	idList( int newgranularity = 16 );

	/*!
		\brief Moves the contents of another list into this list

		This constructor performs a move operation, transferring all elements from the source list to this list. The source list is left in a valid but unspecified state after the move. This is useful
	   for efficient transfer of list contents without unnecessary copying.

		\param other The source list to move elements from
	*/
	idList( idList&& other );

	/*!
		\brief Creates a new list as a copy of an existing list.

		The constructor initializes the list by copying all elements from the provided other list. It first sets the internal list pointer to NULL and then assigns the contents of the other list to
	   this instance.

		\param other The list to copy elements from
	*/
	idList( const idList& other );

	/*!
		\brief Constructs a new list and initializes it with elements from the provided initializer list.

		The constructor creates a new list with an initial capacity of 16 elements. It then sets the actual number of elements to match the size of the initializer list and copies all elements from
	   the initializer list into the internal storage.

		\param initializerList The initializer list containing the elements to initialize the list with
	*/
	idList( std::initializer_list<_type_> initializerList );

	/*!
		\brief Destructor for the idList template class that clears all elements.

		This destructor is responsible for cleaning up the resources used by the idList template class. It calls the Clear method to ensure all dynamically allocated elements are properly deallocated
	   before the list object is destroyed. The ID_INLINE keyword indicates that this destructor is intended to be inlined for performance reasons.

	*/
	~idList();

	//! Clears all elements from the list and resets its size to zero.
	void				   Clear();

	//! Returns the number of elements in the list
	int					   Num() const;

	//! Returns the number of elements that are allocated for.
	int					   NumAllocated() const;

	/*!
		\brief Sets the granularity for the list allocation size

		Configures the granularity used for allocating memory for the list. When the list is resized, it will align to the specified granularity. The granularity must be greater than zero.

		\param newgranularity The new granularity value to use for list allocations
		\throws assertion failure if newgranularity is less than or equal to zero
	*/
	void				   SetGranularity( int newgranularity );

	//! Returns the current granularity of the list.
	int					   GetGranularity() const;

	//! Returns the total size in bytes of memory allocated for the list elements
	size_t				   Allocated() const;

	//! Returns the total size in bytes of all memory allocated for this list, including both the list structure and its allocated elements
	size_t				   Size() const;

	//! Returns the total memory size in bytes used by all the elements in the list.
	size_t				   MemoryUsed() const;

	idList<_type_, _tag_>& operator=( idList<_type_, _tag_>&& other );
	idList<_type_, _tag_>& operator=( const idList<_type_, _tag_>& other );
	const _type_&		   operator[]( int index ) const;
	_type_&				   operator[]( int index );

	/*!
		\brief Resizes the list to exactly match the number of elements it contains

		This function adjusts the internal storage of the list to match its current element count. If the list has elements, it resizes the memory allocation to fit exactly the number of elements
	   present. If the list is empty, it clears the list entirely. The function is typically used to free up unused memory when the list size has changed significantly.

	*/
	void				   Condense();

	/*!
		\brief Resizes the list to contain the specified number of elements.

		This function adjusts the size of the list to match the given number of elements. If the new size is zero or negative, the list is cleared. If the new size is the same as the current size, no
	   changes are made. Otherwise, the internal array is resized using a helper function, and the size and num members are updated accordingly. If the new size is smaller than the current number of
	   elements, the number of elements is also adjusted to match the new size.

		\param newsize The desired number of elements in the list
		\throws assertion failure if newsize is negative
	*/
	void				   Resize( int newsize );

	/*!
		\brief Resizes the list to the specified size and updates the granularity.

		This function resizes the internal list to accommodate the specified number of elements. It also updates the granularity used for memory allocation. If the new size is zero or negative, the
	   list is cleared. The function ensures that the number of elements does not exceed the new size after resizing.

		\param newsize The desired size of the list after resizing
		\param newgranularity The new granularity to be used for memory allocation
		\throws assertion failure if newsize is negative or newgranularity is not positive
	*/
	void				   Resize( int newsize, int newgranularity );

	/*!
		\brief Sets the number of elements in the list to the specified value, resizing the list if necessary

		This function adjusts the number of elements in the list to match the provided value. If the new number exceeds the current capacity, the list is resized to accommodate the change. The
	   function ensures the new number is non-negative through an assertion. This is typically used to set the list size to zero or to prepare the list for a specific number of elements without
	   necessarily initializing them.

		\param newnum The desired number of elements in the list
		\throws assertion failure if newnum is negative
	*/
	void				   SetNum( int newnum );

	/*!
		\brief Ensures the list has at least the specified number of elements, leaving existing elements unchanged and new elements uninitialized

		This function guarantees that the list contains at least the specified number of elements. If the requested size is larger than the current size, it expands the list to accommodate the new
	   elements. The existing elements remain untouched while any newly added elements are left in an uninitialized state. The function uses a granularity-based allocation strategy to optimize memory
	   usage.

		\param newSize The minimum number of elements the list should contain
	*/
	void				   AssureSize( int newSize );

	/*!
		\brief Ensures the list has at least the specified number of elements, initializing any new elements with the provided value

		This function guarantees that the list contains at least the specified number of elements. If the current size is smaller than the requested size, it expands the list by allocating additional
	   memory and initializes the new elements with the provided initialization value. The function also ensures that the number of elements in the list is set to the new size after the operation.

		\param newSize The minimum number of elements the list should contain
		\param initValue The value to initialize any newly added elements with
	*/
	void				   AssureSize( int newSize, const _type_& initValue );

	/*!
		\brief Ensures the list has at least the specified number of elements, allocating new elements if necessary.

		This function guarantees that the list contains at least the specified number of elements. If the current size is smaller than the requested size, it will resize the list and initialize any
	   new elements using the provided allocator function. The function handles memory allocation and initialization of new elements, ensuring that the list can accommodate the requested number of
	   elements.

		\param newSize The minimum number of elements the list should contain
		\param allocator Function used to allocate and initialize new elements
	*/
	void				   AssureSizeAlloc( int newSize, new_t* allocator );

	//! Returns a pointer to the internal array of the list.
	_type_*				   Ptr();

	//! Returns a pointer to the internal array of the list
	const _type_*		   Ptr() const;

	/*!
		\brief Returns a reference to a new data element at the end of the list

		This function provides access to a new element in the list, allocating space if necessary. It ensures the list has sufficient capacity by resizing if needed, and then returns a reference to
	   the next available slot in the list. The function is typically used to add new elements to the list without explicitly calling Add or Insert, as it automatically handles list expansion.

		\return A reference to the newly allocated element at the end of the list
	*/
	_type_&				   Alloc();

	/*!
		\brief Appends an element to the list and returns the index of the new element

		This function adds a new element to the end of the list. If the list is empty, it initializes the list with the specified granularity. If the list is full, it resizes the list by adding the
	   granularity amount of elements. The function then assigns the new element to the last position in the list and increments the element counter. The return value is the index of the newly added
	   element.

		\param obj The element to append to the list
		\return The index of the newly appended element in the list
	*/
	int					   Append( const _type_& obj );

	/*!
		\brief Appends all elements from another list to this list.

		This function appends all elements from the provided list to the current list. It first checks if the current list is empty and initializes it with a default granularity if needed. Then it
	   iterates through each element of the provided list and appends them one by one using the Append method for individual elements. The function returns the total number of elements in the list
	   after the append operation.

		\param other The list whose elements are to be appended to this list.
		\return The total number of elements in the list after appending the elements from the other list.
	*/
	int					   Append( const idList& other );

	/*!
		\brief Appends an object to the list with exponential growth in capacity

		This function adds an object to the list and automatically grows the list capacity when necessary. The growth strategy uses exponential expansion, increasing the size by 50% plus a granularity
	   amount, rounded up to the nearest multiple of the granularity. This is similar to the behavior of std::vector::push_back. The function returns the index of the newly added element.

		\param obj The object to be appended to the list
		\return The index of the appended object in the list
	*/
	int					   AddGrow( const _type_& obj );

	/*!
		\brief Reverses the order of elements in the list.

		This function reverses the order of elements stored in the list by swapping elements from the beginning with elements from the end. The operation is performed in-place and runs in linear time
	   relative to the number of elements in the list.

	*/
	void				   Reverse();

	/*!
		\brief Adds an element to the list only if it is not already present, returning its index.

		This function checks if the given object already exists in the list using FindIndex. If the object is not found, it appends the object to the list and returns the index of the newly added
	   element. If the object already exists, it returns the index of the existing element without adding a duplicate. This ensures that the list contains only unique elements.

		\param obj The object to add to the list if it does not already exist
		\return The index of the object in the list, either existing or newly added
	*/
	int					   AddUnique( const _type_& obj );

	/*!
		\brief Inserts an element into the list at the specified index and returns the index where the element was inserted

		This function inserts a new element into the list at the given index position. If the list is empty, it will be resized to accommodate the new element. If the list is full, it will be resized
	   according to the granularity setting. The function handles edge cases where the index is negative or exceeds the list size by clamping it to valid bounds. All existing elements at or after the
	   insertion point are shifted to higher indices. The function returns the actual index where the element was inserted.

		\param obj The element to be inserted into the list
		\param index The position at which to insert the element
		\return The index where the element was inserted
	*/
	int					   Insert( const _type_& obj, int index = 0 );

	/*!
		\brief Finds the index of the given element in the list

		This function searches for the specified element in the list and returns its index if found. The search is performed by comparing each element in the list with the provided object using the
	   equality operator. If the element is not found, the function returns -1

		\param obj The element to search for in the list
		\return The index of the element if found, or -1 if the element is not present in the list
	*/
	int					   FindIndex( const _type_& obj ) const;

	/*!
		\brief Finds and returns a pointer to the first occurrence of the specified object in the list

		This function searches for the first occurrence of the given object in the list and returns a pointer to that object. It internally uses the FindIndex method to locate the object and then
	   returns a pointer to the element at that index. If the object is not found, it returns NULL

		\param obj the object to search for in the list
		\return a pointer to the first occurrence of the object in the list, or NULL if the object is not found
	*/
	_type_*				   Find( _type_ const& obj ) const;

	/*!
		\brief Find the index of the first NULL pointer in the list.

		This function searches through the list to find the index of the first element that is NULL. It iterates through all elements starting from index 0 until it finds a NULL value or reaches the
	   end of the list. If a NULL pointer is found, its index is returned. If no NULL pointer is found, the function returns -1 to indicate that no NULL pointer exists in the list.

		\return The index of the first NULL pointer in the list, or -1 if no NULL pointer is found.
	*/
	int					   FindNull() const;

	/*!
		\brief Returns the index of the element pointed to by obj in the list.

		This function calculates the index of a given object pointer within the list by computing the difference between the pointer and the beginning of the list array. It performs assertions to
	   ensure the calculated index is within valid bounds.

		\param obj pointer to the element whose index is to be returned
		\return The index of the element pointed to by obj in the list
		\throws assertion failure if the calculated index is out of bounds
	*/
	int					   IndexOf( const _type_* obj ) const;

	/*!
		\brief Removes the element at the specified index from the list

		This function removes an element from the list at the given index by shifting all subsequent elements one position to the left. The function performs bounds checking to ensure the index is
	   valid before proceeding with the removal. If the index is out of bounds, the function returns false without modifying the list. Otherwise, it returns true to indicate successful removal

		\param index The index of the element to remove from the list
		\return true if the element was successfully removed, false if the index was out of bounds
		\throws assertion failure if the list is null or the index is invalid
	*/
	bool				   RemoveIndex( int index );

	/*!
		\brief Removes the element at the specified index and replaces it with the last element in the list, without preserving order.

		This function removes the element at the given index from the list by replacing it with the last element in the list. This approach is more efficient than shifting all subsequent elements, but
	   it does not preserve the original order of elements in the list. The function returns false if the index is out of bounds, and true if the removal was successful.

		\param index The index of the element to remove from the list
		\return true if the element was successfully removed, false if the index was out of bounds
	*/
	bool				   RemoveIndexFast( int index );

	/*!
		\brief Removes the first occurrence of the specified object from the list

		Returns true if the object was found and removed, false if the object was not found in the list

		\param obj the object to remove from the list
		\return true if the object was found and removed, false if the object was not found
	*/
	bool				   Remove( const _type_& obj );

	/*!
		\brief Sorts the list using the provided sorting template

		This function sorts the elements in the list using the specified sorting template. It first checks if the list is valid (not NULL) and then performs the sorting operation on the pointer to the
	   list data with the number of elements. The sorting is done in-place.

		\param sort The sorting template used to define the sorting criteria
	*/
	void				   SortWithTemplate( const idSort<_type_>& sort = idSort_QuickDefault<_type_>() );

	/*!
		\brief Swaps the contents of this list with another list.

		This function exchanges all elements between the current list and the provided other list. After the swap, the current list will contain the elements that were originally in the other list,
	   and vice versa. The operation is performed in constant time by swapping the internal pointers and metadata of both lists.

		\param other The other list whose contents will be swapped with this list
	*/
	void				   Swap( idList& other );

	/*!
		\brief Deletes all dynamically allocated objects contained in the list

		This function iterates through all elements in the list and deletes any non-null pointers, setting them to NULL afterward. If the clear parameter is true, the list is cleared entirely after
	   deleting the contents. Otherwise, the list structure remains but all pointer entries are set to NULL

		\param clear If true, clears the entire list after deleting contents; if false, only sets pointers to NULL while preserving list structure
	*/
	void				   DeleteContents( bool clear = true );

	//------------------------
	// auto-cast to other idList types with a different memory tag
	//------------------------

	template<memTag_t _t_>
	operator idList<_type_, _t_>&()
	{
		return *reinterpret_cast<idList<_type_, _t_>*>( this );
	}

	template<memTag_t _t_>
	operator const idList<_type_, _t_>&() const
	{
		return *reinterpret_cast<const idList<_type_, _t_>*>( this );
	}

	//------------------------
	// memTag
	//
	// Changing the memTag when the list has an allocated buffer will
	// result in corruption of the memory statistics.
	//------------------------
	memTag_t GetMemTag() const { return ( memTag_t )memTag; };
	void	 SetMemTag( memTag_t tag_ ) { memTag = ( byte )tag_; };

	template<typename T>
	struct Iterator {
		T*	 p;
		T&	 operator*() { return *p; }
		bool operator!=( const Iterator& rhs ) { return p != rhs.p; }
		void operator++() { ++p; }
	};

	auto begin() const // const version
	{
		return Iterator<_type_> { list };
	};
	auto end() const // const version
	{
		return Iterator<_type_> { list + Num() };
	};

	/*
	// Begin/End methods for range-based for loops.
	_type_* begin()
	{
		if( num > 0 )
		{
			return &list[0];
		}
		else
		{
			return nullptr;
		}
	}
	_type_* end()
	{
		if( num > 0 )
		{
			return &list[num - 1];
		}
		else
		{
			return nullptr;
		}
	}

	const _type_* begin() const
	{
		if( num > 0 )
		{
			return &list[0];
		}
		else
		{
			return nullptr;
		}
	}
	const _type_* end() const
	{
		if( num > 0 )
		{
			return &list[num - 1];
		}
		else
		{
			return nullptr;
		}
	}
	*/
private:
	int		num;
	int		size;
	int		granularity;
	_type_* list;
	byte	memTag;
};

/*
================
idList<_type_,_tag_>::idList( int )
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::idList( int newgranularity )
{
	assert( newgranularity > 0 );

	list		= NULL;
	granularity = newgranularity;
	memTag		= _tag_;
	Clear();
}

/*
================
idList<_type_,_tag_>::idList( idList< _type_, _tag_ >&& other )
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::idList( idList&& other )
{
	list  = NULL;
	*this = std::move( other );
}

/*
================
idList<_type_,_tag_>::idList( const idList< _type_, _tag_ > &other )
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::idList( const idList& other )
{
	list  = NULL;
	*this = other;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::idList( std::initializer_list<_type_> initializerList ) :
	idList( 16 )
{
	SetNum( initializerList.size() );
	std::copy( initializerList.begin(), initializerList.end(), list );
}

template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::~idList()
{
	Clear();
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::Clear()
{
	if( list ) { idListArrayDelete<_type_>( list, size ); }

	list = NULL;
	num	 = 0;
	size = 0;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::DeleteContents( bool clear )
{
	int i;

	for( i = 0; i < num; i++ ) {
		if( list[i] ) { delete list[i]; }
		list[i] = NULL;
	}

	if( clear ) {
		Clear();
	} else {
		memset( list, 0, size * sizeof( _type_ ) );
	}
}

template<typename _type_, memTag_t _tag_>
ID_INLINE size_t idList<_type_, _tag_>::Allocated() const
{
	return size * sizeof( _type_ );
}

template<typename _type_, memTag_t _tag_>
ID_INLINE size_t idList<_type_, _tag_>::Size() const
{
	return sizeof( idList<_type_, _tag_> ) + Allocated();
}

template<typename _type_, memTag_t _tag_>
ID_INLINE size_t idList<_type_, _tag_>::MemoryUsed() const
{
	return num * sizeof( *list );
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::Num() const
{
	return num;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::NumAllocated() const
{
	return size;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::SetNum( int newnum )
{
	assert( newnum >= 0 );
	if( newnum > size ) { Resize( newnum ); }
	num = newnum;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::SetGranularity( int newgranularity )
{
	int newsize;

	assert( newgranularity > 0 );
	granularity = newgranularity;

	if( list ) {
		// resize it to the closest level of granularity
		newsize = num + granularity - 1;
		newsize -= newsize % granularity;
		if( newsize != size ) { Resize( newsize ); }
	}
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::GetGranularity() const
{
	return granularity;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::Condense()
{
	if( list ) {
		if( num ) {
			Resize( num );
		} else {
			Clear();
		}
	}
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::Resize( int newsize )
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

	list = ( _type_* )idListArrayResize<_type_, _tag_>( list, size, newsize, false );
	size = newsize;
	if( size < num ) { num = size; }
}

/*
================
idList<_type_,_tag_>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::Resize( int newsize, int newgranularity )
{
	assert( newsize >= 0 );

	assert( newgranularity > 0 );
	granularity = newgranularity;

	// free up the list if no data is being reserved
	if( newsize <= 0 ) {
		Clear();
		return;
	}

	list = ( _type_* )idListArrayResize<_type_, _tag_>( list, size, newsize, false );
	size = newsize;
	if( size < num ) { num = size; }
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::AssureSize( int newSize )
{
	int newNum = newSize;

	if( newSize > size ) {
		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		Resize( newSize );

		num = newNum;
	}
}

/*
================
idList<_type_,_tag_>::AssureSize

Makes sure the list has at least the given number of elements and initialize any elements not yet initialized.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::AssureSize( int newSize, const _type_& initValue )
{
	int newNum = newSize;

	if( newSize > size ) {
		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );

		for( int i = num; i < newSize; i++ ) {
			list[i] = initValue;
		}
	}

	num = newNum;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::AssureSizeAlloc( int newSize, new_t* allocator )
{
	int newNum = newSize;

	if( newSize > size ) {
		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );

		for( int i = num; i < newSize; i++ ) {
			list[i] = ( *allocator )();
		}
	}

	num = newNum;
}

/*
================
idList<_type_,_tag_>::operator=

Moves the contents and size attributes of another list, effectively emptying the other list.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>& idList<_type_, _tag_>::operator=( idList<_type_, _tag_>&& other )
{
	Clear();

	num			= other.num;
	size		= other.size;
	granularity = other.granularity;
	memTag		= other.memTag;
	list		= other.list;

	other.list = nullptr;
	other.Clear();

	return *this;
}

/*
================
idList<_type_,_tag_>::operator=

Copies the contents and size attributes of another list.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>& idList<_type_, _tag_>::operator=( const idList<_type_, _tag_>& other )
{
	int i;

	Clear();

	num			= other.num;
	size		= other.size;
	granularity = other.granularity;
	memTag		= other.memTag;

	if( size ) {
		list = ( _type_* )idListArrayNew<_type_, _tag_>( size, false );
		for( i = 0; i < num; i++ ) {
			list[i] = other.list[i];
		}
	}

	return *this;
}

/*
================
idList<_type_,_tag_>::operator[] const

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE const _type_& idList<_type_, _tag_>::operator[]( int index ) const
{
	assert( index >= 0 );
	assert( index < num );

	return list[index];
}

/*
================
idList<_type_,_tag_>::operator[]

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE _type_& idList<_type_, _tag_>::operator[]( int index )
{
	assert( index >= 0 );
	assert( index < num );

	return list[index];
}

template<typename _type_, memTag_t _tag_>
ID_INLINE _type_* idList<_type_, _tag_>::Ptr()
{
	return list;
}

/*
================
idList<_type_,_tag_>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template<typename _type_, memTag_t _tag_>
const ID_INLINE _type_* idList<_type_, _tag_>::Ptr() const
{
	return list;
}

/*
================
idList<_type_,_tag_>::Alloc

Returns a reference to a new data element at the end of the list.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE _type_& idList<_type_, _tag_>::Alloc()
{
	if( !list ) { Resize( granularity ); }

	if( num == size ) { Resize( size + granularity ); }

	return list[num++];
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::Append( _type_ const& obj )
{
	if( !list ) { Resize( granularity ); }

	if( num == size ) {
		int newsize;

		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}

	list[num] = obj;
	num++;

	return num - 1;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::AddGrow( _type_ const& obj )
{
	if( num == size ) {
		int newsize;

		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		newsize = ( size * 3 ) >> 1;	  // + 50% size
		newsize += granularity;			  // round up to granularity
		newsize -= newsize % granularity; //
		Resize( newsize );
	}

	list[num] = obj;
	num++;

	return num - 1;
}

/*!
	\brief Swaps the values of two variables of the same type.

	This function performs an in-place swap of two variables of the same type. It uses a temporary variable to hold the value of the first variable, then assigns the value of the second variable to
   the first, and finally assigns the temporary value to the second variable. This is a common idiom for exchanging values between two variables.

	\param a Reference to the first variable to be swapped
	\param b Reference to the second variable to be swapped
*/
template<typename _type_, memTag_t _tag_ = TAG_IDLIB_LIST>
ID_INLINE void idSwap( _type_& a, _type_& b )
{
	_type_ c = a;
	a		 = b;
	b		 = c;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::Reverse()
{
	int k = ( num >> 1 );
	for( int i = 0; i < k; i++ ) {
		idSwap<_type_>( list[i], list[num - 1 - i] );
	}
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::Insert( _type_ const& obj, int index )
{
	if( !list ) { Resize( granularity ); }

	if( num == size ) {
		int newsize;

		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}

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

/*
================
idList<_type_,_tag_>::Append

adds the other list to this one

Returns the size of the new combined list
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::Append( const idList<_type_, _tag_>& other )
{
	if( !list ) {
		if( granularity == 0 ) // this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		Resize( granularity );
	}

	int n = other.Num();
	for( int i = 0; i < n; i++ ) {
		Append( other[i] );
	}

	return Num();
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::AddUnique( _type_ const& obj )
{
	int index;

	index = FindIndex( obj );
	if( index < 0 ) { index = Append( obj ); }

	return index;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::FindIndex( _type_ const& obj ) const
{
	int i;

	for( i = 0; i < num; i++ ) {
		if( list[i] == obj ) { return i; }
	}

	// Not found
	return -1;
}

/*
================
idList<_type_,_tag_>::Find

Searches for the specified data in the list and returns it's address. Returns NULL if the data is not found.
================
*/
template<typename _type_, memTag_t _tag_>
ID_INLINE _type_* idList<_type_, _tag_>::Find( _type_ const& obj ) const
{
	int i;

	i = FindIndex( obj );
	if( i >= 0 ) { return &list[i]; }

	return NULL;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::FindNull() const
{
	int i;

	for( i = 0; i < num; i++ ) {
		if( list[i] == NULL ) { return i; }
	}

	// Not found
	return -1;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE int idList<_type_, _tag_>::IndexOf( _type_ const* objptr ) const
{
	int index;

	index = objptr - list;

	assert( index >= 0 );
	assert( index < num );

	return index;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE bool idList<_type_, _tag_>::RemoveIndex( int index )
{
	int i;

	assert( list != NULL );
	assert( index >= 0 );
	assert( index < num );

	if( ( index < 0 ) || ( index >= num ) ) { return false; }

	num--;
	for( i = index; i < num; i++ ) {
		list[i] = list[i + 1];
	}

	return true;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE bool idList<_type_, _tag_>::RemoveIndexFast( int index )
{
	if( ( index < 0 ) || ( index >= num ) ) { return false; }

	num--;
	if( index != num ) { list[index] = list[num]; }

	return true;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE bool idList<_type_, _tag_>::Remove( _type_ const& obj )
{
	int index;

	index = FindIndex( obj );
	if( index >= 0 ) { return RemoveIndex( index ); }

	return false;
}

template<typename _type_, memTag_t _tag_>
ID_INLINE void idList<_type_, _tag_>::SortWithTemplate( const idSort<_type_>& sort )
{
	if( list == NULL ) { return; }
	sort.Sort( Ptr(), Num() );
}

/*!
	\brief Finds an item in a list that matches the given value based on equality comparison

	This function searches through a list of items to find the first item that is equal to the provided value. It uses the equality operator == to compare items in the list with the given value. The
   function returns a pointer to the found item if a match is found, or NULL if no match is found. The _type_ parameter must have an overloaded operator== for this function to work correctly. If the
   list contains pointers, use the FindFromGenericPtr function instead.

	\param list The list of items to search through
	\param other The value to compare against items in the list
	\return Pointer to the first matching item in the list, or NULL if no match is found
*/
template<typename _type_, memTag_t _tag_, typename _compare_type_>
_type_* FindFromGeneric( idList<_type_, _tag_>& list, const _compare_type_& other )
{
	for( int i = 0; i < list.Num(); i++ ) {
		if( list[i] == other ) { return &list[i]; }
	}
	return NULL;
}

/*!
	\brief Finds an element in a list by comparing against a given value using pointer dereferencing

	This function searches through a list of pointers to objects of type _type_ and checks if the dereferenced pointer value matches the provided comparison value. It returns a pointer to the first
   matching element in the list, or NULL if no match is found. The comparison is performed using the equality operator == between the dereferenced list element and the comparison value.

	\param list The list of pointers to search through
	\param other The value to compare against the dereferenced pointers in the list
	\return A pointer to the first matching element in the list, or NULL if no match is found
*/
template<typename _type_, memTag_t _tag_, typename _compare_type_>
_type_* FindFromGenericPtr( idList<_type_, _tag_>& list, const _compare_type_& other )
{
	for( int i = 0; i < list.Num(); i++ ) {
		if( *list[i] == other ) { return &list[i]; }
	}
	return NULL;
}

#endif /* !__LIST_H__ */
