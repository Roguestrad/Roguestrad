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
	/*!
		\brief Destroys the idFlexList object and frees any dynamically allocated memory.

		The destructor checks if the list pointer refers to dynamically allocated memory by comparing it to the autoStore pointer. If they are different, it means the list was allocated with new[] and
	   must be freed using delete[]. If they are the same, the list was stored inline and no deallocation is necessary.

	*/
	~idFlexList()
	{
		if( list != autoStore ) { delete[] list; }
	}

	/*!
		\brief Initializes a new instance of the idFlexList class with default values.

		The constructor initializes the list with the autoStore allocator, sets the current number of elements to zero, and assigns the maximum size of the list based on the template parameter N.

	*/
	idFlexList()
	{
		list = autoStore;
		num	 = 0;
		size = N;
	}

	//! Clears the flex list and resets it to its initial state.
	void Clear() { num = 0; }

	/*!
		\brief Clears the flex list and frees dynamically allocated memory if it was not using the automatic storage.

		This function resets the flex list to its initial state. If the list was using dynamically allocated memory (i.e., the list pointer is not equal to autoStore), it frees that memory. It then
	   resets the list to use the automatic storage, and initializes the number of elements and size counters to zero and N respectively.

	*/
	void ClearFree()
	{
		if( list != autoStore ) { delete[] list; }
		list = autoStore;
		num	 = 0;
		size = N;
	}

	/*!
		\brief Sets the number of elements in the flexible list to the specified value

		This function adjusts the number of elements in the list to the given value. If the requested number exceeds the current capacity, it grows the list exponentially to accommodate the new size.
	   The function ensures that the list can hold at least the specified number of elements while maintaining efficient memory usage. When the number is reduced, it simply updates the count without
	   deallocating memory.

		\param newNum The desired number of elements in the list
	*/
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

	//! Returns the number of elements currently contained in the list.
	ID_FORCE_INLINE int			Num() const { return num; }

	/*!
		\brief Returns the number of elements that have been allocated in the flexible list.

		This function provides the count of elements that have been allocated within the flexible list, regardless of whether they are currently in use or not. It gives insight into the memory
	   allocation status of the list.

		\return The total number of elements that have been allocated in the list
	*/
	int							NumAllocated() const { return size; }

	/*!
		\brief Returns a pointer to the internal array of elements in the flex list

		This function provides direct access to the underlying array used to store elements in the flex list. It is marked as inline for performance reasons and returns a pointer to the internal data.
	   The returned pointer is valid only as long as the flex list exists and is not modified.

		\return A pointer to the first element in the internal array of the flex list
	*/
	ID_FORCE_INLINE type*		Ptr() { return list; }

	/*!
		\brief Returns a pointer to the internal array of elements in the flex list

		This function provides direct access to the underlying array used to store elements in the flex list. It is marked as inline for performance reasons and returns a constant pointer to prevent
	   modification of the internal data through this interface. The returned pointer is valid only as long as the flex list exists and is not modified.

		\return A constant pointer to the first element in the internal array of the flex list
	*/
	ID_FORCE_INLINE const type* Ptr() const { return list; }

	ID_FORCE_INLINE const type& operator[]( int index ) const
	{
		assert( unsigned( index ) < unsigned( num ) );
		return list[index];
	}
	ID_FORCE_INLINE type& operator[]( int index )
	{
		assert( unsigned( index ) < unsigned( num ) );
		return list[index];
	}

	/*!
		\brief Returns a reference to the last element in the list.

		This function provides access to the last element of the flex list. It asserts that the list is not empty before returning the element. The returned reference allows for both reading and
	   modifying the last element.

		\return A constant reference to the last element in the list
		\throws assertion failure if the list is empty
	*/
	ID_FORCE_INLINE const type& Last() const
	{
		assert( num > 0 );
		return list[num - 1];
	}

	/*!
		\brief Returns a reference to the last element in the list.

		This function provides access to the last element of the flex list. It asserts that the list is not empty before returning a reference to the final element. The returned reference allows both
	   reading and modification of the last element.

		\return A reference to the last element in the list
		\throws assertion failure if the list is empty
	*/
	ID_FORCE_INLINE type& Last()
	{
		assert( num > 0 );
		return list[num - 1];
	}

	/*!
		\brief Adds a new element to the list, growing the list if necessary, and returns the index of the new element

		This function appends a new element to the flexible list. If the current capacity of the list is insufficient to accommodate the new element, the list is resized by calling the Grow function
	   with a size parameter of twice the current size. The function then assigns the provided object to the next available slot in the list and increments the element count. The index of the newly
	   added element is returned for use in subsequent operations.

		\param obj The element to be added to the list
		\return The index position where the new element was added
	*/
	int AddGrow( type obj )
	{
		if( num == size ) { Grow( 2 * size ); }
		int idx	  = num++;
		list[idx] = obj;
		return idx;
	}

	/*!
		\brief Removes and returns the last element from the list.

		This function retrieves the last element in the list and decrements the internal counter.
		It is expected that the list is not empty when this function is called, as it will access the element at index num - 1.

		\return The last element of the list before removal.
	*/
	ID_FORCE_INLINE type Pop() { return list[--num]; }

	/*!
		\brief Appends k elements from array arr to the list

		This function adds k elements from the provided array arr to the end of the list. It first calculates the base index where the new elements will be inserted, then resizes the list to
	   accommodate the new elements, and finally copies the elements from arr to the appropriate positions in the list

		\param k Number of elements to append from the array
		\param arr Pointer to the array of elements to append
	*/
	void				 Append( int k, const type* arr )
	{
		int base = num;
		SetNum( base + k );
		for( int i = 0; i < k; i++ ) {
			list[base + i] = arr[i];
		}
	}

private:
	/*!
		\brief Grows the internal storage of the flex list to accommodate the specified new size

		This function expands the internal array storage of the flex list to the specified size. It allocates a new array of the requested size, copies all existing elements from the current array to
	   the new array, and then replaces the old array with the new one. If the current list was not using the auto storage, the old array is properly deallocated. The function updates the size
	   tracking variable to reflect the new capacity.

		\param newSize the new size to grow the internal storage to
	*/
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
