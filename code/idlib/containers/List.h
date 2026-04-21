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

/*
===============================================================================

	List template
	Does not allocate memory until the first item is added.

===============================================================================
*/

//! Allocates and constructs an array of objects with optional zero initialization
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

//! Calls destructors on all elements in the array and frees the memory
template<typename _type_>
ID_INLINE void idListArrayDelete( void* ptr, int num )
{
	// Call the destructors on all the elements
	for( int i = 0; i < num; i++ ) {
		( ( _type_* )ptr )[i].~_type_();
	}
	Mem_Free( ptr );
}

//! Resizes an array while preserving existing elements and handling memory management.
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

//! Creates and returns a new element of the specified type.
template<class type>
ID_INLINE type* idListNewElement()
{
	return new type;
}

/*!
	\class idList
	\brief A dynamic array template class for managing collections of elements with configurable memory allocation.

	The idList template class provides a dynamic array implementation that can grow and shrink as needed. It supports various memory management strategies through its template parameters, including
   allocation granularity and memory tags. The class offers efficient element access, insertion, and removal operations while maintaining control over memory usage. It includes methods for sorting,
   searching, and managing the internal storage efficiently. The list can be configured with a specific memory tag for tracking allocation purposes and supports move semantics for efficient resource
   transfer. The implementation handles memory allocation in chunks based on the specified granularity to optimize performance and reduce fragmentation. The class provides both indexed access and
   iterator support for traversing elements.

*/
template<typename _type_, memTag_t _tag_ = TAG_IDLIB_LIST>
class idList
{
public:
	typedef int	   cmp_t( const _type_*, const _type_* );
	typedef _type_ new_t();

	//! Constructs an idList with the specified granularity.
	idList( int newgranularity = 16 );

	//! Moves the contents of another list into this list.
	idList( idList&& other );

	//! Creates a new list as a copy of an existing list.
	idList( const idList& other );

	//! Constructs a list and initializes it with elements from the given initializer list.
	idList( std::initializer_list<_type_> initializerList );

	//! Destructor for the idList template class that clears all elements.
	~idList();

	//! Clears all elements from the list and resets its size to zero.
	void				   Clear();

	//! Returns the number of elements in the list
	int					   Num() const;

	//! Returns the number of elements that are allocated for this list.
	int					   NumAllocated() const;

	//! Sets the granularity for the list allocation size.
	void				   SetGranularity( int newgranularity );

	//! Returns the current memory allocation granularity of the list.
	int					   GetGranularity() const;

	//! Returns the total size of allocated memory for the list elements
	size_t				   Allocated() const;

	//! Returns the total size of allocated memory for the list including the size of the list type itself
	size_t				   Size() const;

	//! Returns the total memory used by the elements in the list
	size_t				   MemoryUsed() const;

	//! Moves the contents of another list to this list
	idList<_type_, _tag_>& operator=( idList<_type_, _tag_>&& other );

	//! Assigns the contents of another list to this list.
	idList<_type_, _tag_>& operator=( const idList<_type_, _tag_>& other );

	//! Returns a const reference to the element at the specified index in the list
	const _type_&		   operator[]( int index ) const;

	//! Provides read and write access to an element at the specified index in the list
	_type_&				   operator[]( int index );

	//! Resizes the list to exactly match the number of elements it contains
	void				   Condense();

	//! Resizes the list to contain the specified number of elements.
	void				   Resize( int newsize );

	//! Resizes the list to the specified size and updates the granularity.
	void				   Resize( int newsize, int newgranularity );

	//! Sets the number of elements in the list to the specified value, resizing the list if necessary.
	void				   SetNum( int newnum );

	//! Ensures the list has at least the specified number of elements, leaving new elements uninitialized
	void				   AssureSize( int newSize );

	//! Ensures the list has the specified number of elements, initializing any new elements with the provided value
	void				   AssureSize( int newSize, const _type_& initValue );

	//! Ensures the list has the specified number of elements by allocating new ones if necessary.
	void				   AssureSizeAlloc( int newSize, new_t* allocator );

	//! Returns a pointer to the internal array of the list
	_type_*				   Ptr();

	//! Returns a pointer to the internal list data.
	const _type_*		   Ptr() const;

	//! Returns a reference to a new data element at the end of the list
	_type_&				   Alloc();

	//! Appends an element to the list and returns its index
	int					   Append( const _type_& obj );

	//! Appends all elements from another list to this list
	int					   Append( const idList& other );

	//! Appends an object to the list with exponential growth and returns its index.
	int					   AddGrow( const _type_& obj );

	//! Reverses the order of elements in the list
	void				   Reverse();

	//! Adds an element to the list only if it is not already present, returning its index.
	int					   AddUnique( const _type_& obj );

	//! Inserts an element at the specified index in the list
	int					   Insert( const _type_& obj, int index = 0 );

	//! Find the index of the given element in the list, returning -1 if not found.
	int					   FindIndex( const _type_& obj ) const;

	//! Finds and returns a pointer to the given element in the list.
	_type_*				   Find( _type_ const& obj ) const;

	//! Finds the index of the first NULL pointer in the list.
	int					   FindNull() const;

	//! Returns the index of the pointer to an element in the list.
	int					   IndexOf( const _type_* obj ) const;

	//! Removes the element at the specified index from the list
	bool				   RemoveIndex( int index );

	//! Removes the element at the specified index using a fast swap method that does not preserve list order.
	bool				   RemoveIndexFast( int index );

	//! Removes the first occurrence of the specified element from the list.
	bool				   Remove( const _type_& obj );

	//! Sorts the list elements using the provided sorting template.
	void				   SortWithTemplate( const idSort<_type_>& sort = idSort_QuickDefault<_type_>() );

	//! Swaps the contents of this list with another list.
	void				   Swap( idList& other );

	//! Deletes all dynamically allocated objects in the list and optionally clears the list.
	void				   DeleteContents( bool clear = true );

	//! Provides an automatic cast to another idList type with a different memory tag.
	template<memTag_t _t_>
	operator idList<_type_, _t_>&()
	{
		return *reinterpret_cast<idList<_type_, _t_>*>( this );
	}

	//! Converts the list to a const reference of the same type.
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

		//! Returns a reference to the element pointed to by the iterator.
		T&	 operator*() { return *p; }

		//! Checks if the current iterator is not equal to another iterator.
		bool operator!=( const Iterator& rhs ) { return p != rhs.p; }

		//! Increments the iterator to point to the next element in the list.
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

template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::idList( int newgranularity )
{
	assert( newgranularity > 0 );

	list		= NULL;
	granularity = newgranularity;
	memTag		= _tag_;
	Clear();
}

template<typename _type_, memTag_t _tag_>
ID_INLINE idList<_type_, _tag_>::idList( idList&& other )
{
	list  = NULL;
	*this = std::move( other );
}

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

template<typename _type_, memTag_t _tag_>
ID_INLINE const _type_& idList<_type_, _tag_>::operator[]( int index ) const
{
	assert( index >= 0 );
	assert( index < num );

	return list[index];
}

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

template<typename _type_, memTag_t _tag_>
const ID_INLINE _type_* idList<_type_, _tag_>::Ptr() const
{
	return list;
}

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

//! Swaps the values of two variables of the same type.
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

//! Finds an item in a list that matches the specified value based on equality comparison.
template<typename _type_, memTag_t _tag_, typename _compare_type_>
_type_* FindFromGeneric( idList<_type_, _tag_>& list, const _compare_type_& other )
{
	for( int i = 0; i < list.Num(); i++ ) {
		if( list[i] == other ) { return &list[i]; }
	}
	return NULL;
}

//! Searches for an element in a list that matches the given value using pointer comparison.
template<typename _type_, memTag_t _tag_, typename _compare_type_>
_type_* FindFromGenericPtr( idList<_type_, _tag_>& list, const _compare_type_& other )
{
	for( int i = 0; i < list.Num(); i++ ) {
		if( *list[i] == other ) { return &list[i]; }
	}
	return NULL;
}

#endif /* !__LIST_H__ */
