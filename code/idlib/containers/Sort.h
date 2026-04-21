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
#ifndef __SORT_H__
#define __SORT_H__

/*
================================================================================================
Contains the generic templated sort algorithms for quick-sort, heap-sort and insertion-sort.

The sort algorithms do not use class operators or overloaded functions to compare
objects because it is often desireable to sort the same objects in different ways
based on different keys (not just ascending and descending but sometimes based on
name and other times based on say priority). So instead, for each different sort a
separate class is implemented with a Compare() function.

This class is derived from one of the classes that implements a sort algorithm.
The Compare() member function does not only define how objects are sorted, the class
can also store additional data that can be used by the Compare() function. This, for
instance, allows a list of indices to be sorted where the indices point to objects
in an array. The base pointer of the array with objects can be stored on the class
that implements the Compare() function such that the Compare() function can use keys
that are stored on the objects.

The Compare() function is not virtual because this would incur significant overhead.
Do NOT make the Compare() function virtual on the derived class!
The sort implementations also explicitely call the Compare() function of the derived
class. This is to avoid various compiler bugs with using overloaded compare functions
and the inability of various compilers to find the right overloaded compare function.

To sort an array, an idList or an idStaticList, a new sort class, typically derived from
idSort_Quick, is implemented as follows:

class idSort_MySort : public idSort_Quick< idMyObject, idSort_MySort > {
public:
	int Compare( const idMyObject & a, const idMyObject & b ) const {
		if ( a should come before b ) {
			return -1; // or any negative integer
		} if ( a should come after b ) {
			return 1;  // or any positive integer
		} else {
			return 0;
		}
	}
};

To sort an array:

idMyObject array[100];
idSort_MySort().Sort( array, 100 );

To sort an idList:

idList< idMyObject > list;
list.Sort( idSort_MySort() );

The sort implementations never create temporaries of the template type. Only the
'SwapValues' template is used to move data around. This 'SwapValues' template can be
specialized to implement fast swapping of data. For instance, when sorting a list with
objects of some string class it is important to implement a specialized 'SwapValues' for
this string class to avoid excessive re-allocation and copying of strings.

================================================================================================
*/

/*!
	\brief Swaps the values of two variables of the same type.

	This function takes two references to variables of the same type and exchanges their values. It uses a temporary variable to perform the swap operation, ensuring that the original values are
   correctly exchanged between the two parameters.

	\param a Reference to the first variable whose value will be swapped with the second variable.
	\param b Reference to the second variable whose value will be swapped with the first variable.
*/
template<typename _type_>
ID_INLINE void SwapValues( _type_& a, _type_& b )
{
	_type_ c = a;
	a		 = b;
	b		 = c;
}

/*!
	\class idSort
	\brief A generic sorting interface for organizing collections of elements.

	The idSort class serves as a polymorphic sorting interface that defines a contract for sorting algorithms to be implemented by derived classes. It is designed to provide a standardized way to sort
   arrays of elements of any type through a virtual interface. The Sort method takes a base pointer to an array and the number of elements to sort, allowing for flexible sorting implementations. The
   class is intended to be used within the engine's data processing pipeline where various sorting strategies may be needed, with derived implementations providing specific sorting algorithms such as
   quicksort, mergesort, or others. The pure virtual nature of the Sort method enforces that all concrete implementations must provide their own sorting logic without default behavior.

*/
template<typename _type_>
class idSort
{
public:
	virtual ~idSort() { }
	virtual void Sort( _type_* base, unsigned int num ) const = 0;
};

/*!
	\class idSort_Quick
	\brief A hybrid quicksort and insertion sort implementation for efficient array sorting.

	This class provides a hybrid sorting algorithm that combines the efficiency of quicksort for larger datasets with the simplicity and speed of insertion sort for small datasets. The implementation
   avoids traditional recursion by maintaining a stack of subarray indices, making it suitable for environments where stack overflow might be a concern. The algorithm includes optimizations for
   handling duplicate elements to prevent worst-case performance scenarios. It is designed to be used as a sorting mechanism within the engine's data processing pipelines where reliable and efficient
   sorting is required.

*/
template<typename _type_, typename _derived_>
class idSort_Quick : public idSort<_type_>
{
public:
	/*!
		\brief Sorts an array of elements using a hybrid quicksort and insertion sort algorithm.

		This function implements a hybrid sorting algorithm that combines quicksort with insertion sort for improved performance. It uses quicksort for larger subarrays and falls back to insertion
	   sort for smaller ones. The algorithm maintains a stack of subarray indices to avoid recursion, and includes optimizations for handling duplicate elements to prevent worst-case performance.

		\param base Pointer to the first element of the array to be sorted
		\param num Number of elements in the array to be sorted
	*/
	virtual void Sort( _type_* base, unsigned int num ) const
	{
		if( num <= 0 ) { return; }

		const int64 MAX_LEVELS = 128;
		int64		lo[MAX_LEVELS], hi[MAX_LEVELS];

		// 'lo' is the lower index, 'hi' is the upper index
		// of the region of the array that is being sorted.
		lo[0] = 0;
		hi[0] = num - 1;

		for( int64 level = 0; level >= 0; ) {
			int64 i = lo[level];
			int64 j = hi[level];

			// Only use quick-sort when there are 4 or more elements in this region and we are below MAX_LEVELS.
			// Otherwise fall back to an insertion-sort.
			if( ( ( j - i ) >= 4 ) && ( level < ( MAX_LEVELS - 1 ) ) ) {
				// Use the center element as the pivot.
				// The median of a multi point sample could be used
				// but simply taking the center works quite well.
				int64 pi = ( i + j ) / 2;

				// Move the pivot element to the end of the region.
				SwapValues( base[j], base[pi] );

				// Get a reference to the pivot element.
				_type_& pivot = base[j--];

				// Partition the region.
				do {
					while( static_cast<const _derived_*>( this )->Compare( base[i], pivot ) < 0 ) {
						if( ++i >= j ) { break; }
					}
					while( static_cast<const _derived_*>( this )->Compare( base[j], pivot ) > 0 ) {
						if( --j <= i ) { break; }
					}
					if( i >= j ) { break; }
					SwapValues( base[i], base[j] );
				} while( ++i < --j );

				// Without these iterations sorting of arrays with many duplicates may
				// become really slow because the partitioning can be very unbalanced.
				// However, these iterations are unnecessary if all elements are unique.
				while( static_cast<const _derived_*>( this )->Compare( base[i], pivot ) <= 0 && i < hi[level] ) {
					i++;
				}
				while( static_cast<const _derived_*>( this )->Compare( base[j], pivot ) >= 0 && lo[level] < j ) {
					j--;
				}

				// Move the pivot element in place.
				SwapValues( pivot, base[i] );

				assert( level < MAX_LEVELS - 1 );
				lo[level + 1] = i;
				hi[level + 1] = hi[level];
				hi[level]	  = j;
				level++;

			} else {
				// Insertion-sort of the remaining elements.
				for( ; i < j; j-- ) {
					int64 m = i;
					for( int64 k = i + 1; k <= j; k++ ) {
						if( static_cast<const _derived_*>( this )->Compare( base[k], base[m] ) > 0 ) { m = k; }
					}
					SwapValues( base[m], base[j] );
				}
				level--;
			}
		}
	}
};

/*!
	\class idSort_QuickDefault
	\brief Quick sorting implementation for default type comparison

	This class provides a quick sorting implementation that uses default comparison semantics for sorting elements of a specified type. It inherits from idSort_Quick and is designed to be used as a
   template specialization for sorting operations within the engine's sorting framework. The Compare method performs simple arithmetic subtraction between two values to determine their relative
   ordering, making it suitable for numeric types that support subtraction operations. This implementation is intended to be a lightweight, efficient sorting solution for basic data types where the
   natural ordering defined by subtraction is appropriate.

*/
template<typename _type_>
class idSort_QuickDefault : public idSort_Quick<_type_, idSort_QuickDefault<_type_>>
{
public:
	/*!
		\brief Compares two values and returns their difference

		This function performs a simple subtraction operation between two values of the same type. It is typically used as a comparison function for sorting algorithms, where the result indicates the
	   relative order of the two values. A negative result indicates that the first value is less than the second, zero indicates equality, and a positive result indicates that the first value is
	   greater than the second.

		\param a First value to compare
		\param b Second value to compare
		\return The difference between the two values (a - b)
	*/
	int Compare( const _type_& a, const _type_& b ) const { return a - b; }
};

/*!
	\class idSort_QuickDefault< float >
	\brief Provides a quick sorting implementation for floating-point values.

	This class implements a specialized quick sort algorithm tailored for sorting floating-point numbers. It inherits from idSort_Quick and provides a specific comparison function for floats. The
   implementation is designed to efficiently sort float values using the quick sort methodology, making it suitable for performance-critical sorting operations within the engine's data management
   systems. The class template specialization ensures optimal performance characteristics when sorting floating-point data types.

*/
template<>
class idSort_QuickDefault<float> : public idSort_Quick<float, idSort_QuickDefault<float>>
{
public:
	/*!
		\brief Compares two float values and returns an integer indicating their relative order.

		This function performs a standard comparison of two floating-point numbers. It returns -1 if the first number is less than the second, 1 if the first number is greater than the second, and 0
	   if they are equal. This implementation is used as a comparison function for sorting operations, specifically within the context of the idSort_QuickDefault class template which is typically
	   employed for quick sorting algorithms.

		\param a First float value to compare
		\param b Second float value to compare
		\return -1 if a is less than b, 1 if a is greater than b, 0 if a equals b
	*/
	int Compare( const float& a, const float& b ) const
	{
		if( a < b ) { return -1; }
		if( a > b ) { return 1; }
		return 0;
	}
};

/*!
	\class idSort_Heap
	\brief Heap sort implementation for sorting arrays of elements.

	The idSort_Heap class provides an implementation of the heap sort algorithm for sorting arrays of elements. It is designed as a template class that can work with any type of elements, making it
   flexible for different data types within the engine. The sorting is performed in-place, which means it does not require additional memory proportional to the input size, making it memory efficient.
   This implementation follows the standard heap sort algorithm where the array is first transformed into a max heap, and then elements are repeatedly extracted and re-heapified to produce a sorted
   sequence. The class inherits from idSort, indicating it is part of a sorting hierarchy within the engine's codebase, and the sorting behavior is determined by the comparison function defined in the
   derived class. The time complexity of this sorting algorithm is O(n log n), providing consistent performance regardless of the input data distribution.

*/
template<typename _type_, typename _derived_>
class idSort_Heap : public idSort<_type_>
{
public:
	/*!
		\brief Sorts an array of elements using the heap sort algorithm.

		This function performs an in-place heap sort on the provided array. It first builds a max heap from the input array, then repeatedly extracts the maximum element and re-heapifies the remaining
	   elements. The sorting is done in ascending order based on the comparison function defined by the derived class. The algorithm has O(n log n) time complexity and sorts the array in-place without
	   requiring additional memory proportional to the input size.

		\param base Pointer to the array of elements to be sorted
		\param num Number of elements in the array to be sorted
	*/
	virtual void Sort( _type_* base, unsigned int num ) const
	{
		// get all elements in heap order
#if 1
		// O( n )
		for( unsigned int i = num / 2; i > 0; i-- ) {
			// sift down
			unsigned int parent = i - 1;
			for( unsigned int child = parent * 2 + 1; child < num; child = parent * 2 + 1 ) {
				if( child + 1 < num && static_cast<const _derived_*>( this )->Compare( base[child + 1], base[child] ) > 0 ) { child++; }
				if( static_cast<const _derived_*>( this )->Compare( base[child], base[parent] ) <= 0 ) { break; }
				SwapValues( base[parent], base[child] );
				parent = child;
			}
		}
#else
		// O(n log n)
		for( unsigned int i = 1; i < num; i++ ) {
			// sift up
			for( unsigned int child = i; child > 0; ) {
				unsigned int parent = ( child - 1 ) / 2;
				if( static_cast<const _derived_*>( this )->Compare( base[parent], base[child] ) > 0 ) { break; }
				SwapValues( base[child], base[parent] );
				child = parent;
			}
		}
#endif
		// get sorted elements while maintaining heap order
		for( unsigned int i = num - 1; i > 0; i-- ) {
			SwapValues( base[0], base[i] );
			// sift down
			unsigned int parent = 0;
			for( unsigned int child = parent * 2 + 1; child < i; child = parent * 2 + 1 ) {
				if( child + 1 < i && static_cast<const _derived_*>( this )->Compare( base[child + 1], base[child] ) > 0 ) { child++; }
				if( static_cast<const _derived_*>( this )->Compare( base[child], base[parent] ) <= 0 ) { break; }
				SwapValues( base[parent], base[child] );
				parent = child;
			}
		}
	}
};

/*!
	\class idSort_HeapDefault
	\brief A heap-based sorting implementation that uses default numeric comparison for sorting elements.

	This class provides a heap-based sorting mechanism that inherits from idSort_Heap and uses a default numeric comparison function. It is designed to efficiently sort elements in a heap structure
   using the subtraction operator for ordering. The implementation is templated to work with any type that supports the subtraction operation and can be used for heap operations such as priority
   queues or sorting algorithms. The class leverages the heap data structure's properties to provide efficient sorting and retrieval of elements based on their numeric values. The comparison function
   performs simple numeric subtraction to determine element ordering, making it suitable for sorting numeric data types or objects that can be compared using numeric operations.

*/
template<typename _type_>
class idSort_HeapDefault : public idSort_Heap<_type_, idSort_HeapDefault<_type_>>
{
public:
	/*!
		\brief Compares two values of type _type_ and returns their difference

		This function performs a simple subtraction operation between two values of the template type _type_. It is typically used as a comparison function in sorting algorithms or heap data
	   structures where a numeric ordering is required. The function assumes that the type _type_ supports the subtraction operator and returns an integer result indicating the relative ordering of
	   the two operands.

		\param a First value to compare
		\param b Second value to compare
		\return The difference between the two input values a and b
	*/
	int Compare( const _type_& a, const _type_& b ) const { return a - b; }
};

/*!
	\class idSort_Insertion
	\brief Insertion sort implementation for sorting arrays of elements in descending order.

	This class provides a concrete implementation of the sorting algorithm using insertion sort to arrange elements in descending order. It inherits from the generic idSort template class and
   overrides the Sort method to perform the specific sorting operation. The implementation iterates through the array and applies the insertion sort technique to sort elements in descending order. The
   comparison functionality is delegated to a derived class through the Compare method, allowing for flexible sorting criteria. This class is designed to be used as a sorting mechanism within the
   engine's data processing pipelines where a stable, efficient sorting solution for small to medium-sized datasets is required.

*/
template<typename _type_, typename _derived_>
class idSort_Insertion : public idSort<_type_>
{
public:
	/*!
		\brief Sorts an array of elements in descending order using the insertion sort algorithm.

		This function implements the insertion sort algorithm to sort an array of elements in descending order. It iterates through the array, finding the maximum element in the unsorted portion and
	   swapping it with the last element of the unsorted portion. The process continues until the entire array is sorted. The comparison is performed using a derived class's Compare method.

		\param base Pointer to the beginning of the array to be sorted.
		\param num The number of elements in the array.
	*/
	virtual void Sort( _type_* base, unsigned int num ) const
	{
		_type_* lo = base;
		_type_* hi = base + ( num - 1 );
		while( hi > lo ) {
			_type_* max = lo;
			for( _type_* p = lo + 1; p <= hi; p++ ) {
				if( static_cast<const _derived_*>( this )->Compare( ( *p ), ( *max ) ) > 0 ) { max = p; }
			}
			SwapValues( *max, *hi );
			hi--;
		}
	}
};

/*!
	\class idSort_InsertionDefault
	\brief Insertion sort implementation using default comparison for sorting values.

	This class provides a specialized insertion sort implementation that uses default comparison operations for sorting values of a given type. It inherits from idSort_Insertion and implements the
   Compare method to perform direct subtraction-based comparisons between values. The class is designed to work with numeric types or other types that support subtraction and comparison operations. It
   serves as a sorting utility within the engine's sorting framework, specifically optimized for small datasets or nearly sorted data where insertion sort's simplicity and efficiency make it
   preferable over more complex algorithms.

*/
template<typename _type_>
class idSort_InsertionDefault : public idSort_Insertion<_type_, idSort_InsertionDefault<_type_>>
{
public:
	/*!
		\brief Compares two values of type _type_ and returns their difference

		This function performs a comparison between two values of the template type _type_ by subtracting the second value from the first. The implementation assumes that the type supports subtraction
	   and comparison operations. It is typically used as a comparison function for sorting algorithms, particularly in the context of insertion sort.

		\param a First value to compare
		\param b Second value to compare
		\return The difference between the two values, which is typically negative if a < b, zero if a == b, and positive if a > b
	*/
	int Compare( const _type_& a, const _type_& b ) const { return a - b; }
};

#endif // !__SORT_H__
