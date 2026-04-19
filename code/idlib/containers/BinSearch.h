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

#ifndef __BINSEARCH_H__
#define __BINSEARCH_H__

/*
===============================================================================

	Binary Search templates

	The array elements have to be ordered in increasing order.

===============================================================================
*/

/*!
	\brief Finds the index of the last element in the array that is smaller than the specified value using binary search.

	This function performs a binary search on a sorted array to locate the position of the largest element that is strictly less than the given value. It returns the index of that element. If all
   elements in the array are greater than or equal to the value, it returns -1. The function assumes the input array is sorted in ascending order.

	\param array Pointer to the sorted array of elements to search through
	\param arraySize Number of elements in the array
	\param value The value to compare against the array elements
	\return The index of the last element in the array that is smaller than the specified value, or -1 if no such element exists
*/
template<class type>
ID_INLINE int idBinSearch_Less( const type* array, const int arraySize, const type& value )
{
	int len	   = arraySize;
	int mid	   = len;
	int offset = 0;
	while( mid > 0 ) {
		mid = len >> 1;
		if( array[offset + mid] < value ) { offset += mid; }
		len -= mid;
	}
	return offset;
}

/*!
	\brief Finds the index of the last array element that is less than or equal to the specified value using binary search.

	This function performs a binary search on a sorted array to locate the last element that is less than or equal to the given value. It returns the index of that element. If all elements in the
   array are greater than the value, it returns -1. The function assumes the input array is sorted in ascending order.

	\param array Pointer to the sorted array to search
	\param arraySize Number of elements in the array
	\param value The value to compare against elements in the array
	\return The index of the last element in the array that is less than or equal to the specified value, or -1 if no such element exists
*/
template<class type>
ID_INLINE int idBinSearch_LessEqual( const type* array, const int arraySize, const type& value )
{
	int len	   = arraySize;
	int mid	   = len;
	int offset = 0;
	while( mid > 0 ) {
		mid = len >> 1;
		if( array[offset + mid] <= value ) { offset += mid; }
		len -= mid;
	}
	return offset;
}

/*!
	\brief Finds the first array element which is greater than the given value using binary search.

	This function performs a binary search on a sorted array to locate the first element that is greater than the specified value. It returns the index of that element. The function assumes the input
   array is sorted in ascending order. If all elements in the array are less than or equal to the value, it returns the size of the array. The implementation uses a while loop to iteratively narrow
   down the search space by halving the remaining elements until the correct position is found.

	\param array Pointer to the sorted array of elements to search through
	\param arraySize Number of elements in the array
	\param value The value to compare against elements in the array
	\return The index of the first array element that is greater than the given value, or the array size if no such element exists
*/
template<class type>
ID_INLINE int idBinSearch_Greater( const type* array, const int arraySize, const type& value )
{
	int len	   = arraySize;
	int mid	   = len;
	int offset = 0;
	int res	   = 0;
	while( mid > 0 ) {
		mid = len >> 1;
		if( array[offset + mid] > value ) {
			res = 0;
		} else {
			offset += mid;
			res = 1;
		}
		len -= mid;
	}
	return offset + res;
}

/*!
	\brief Finds the first array element which is greater than or equal to the given value using binary search.

	This function performs a binary search on a sorted array to locate the first element that is greater than or equal to the specified value. It returns the index of that element. The function
   assumes the input array is sorted in ascending order. If all elements in the array are less than the specified value, the function returns the size of the array.

	\param array Pointer to the sorted array of elements to search through
	\param arraySize The number of elements in the array
	\param value The value to find the first greater than or equal element for
	\return The index of the first array element that is greater than or equal to the given value
*/
template<class type>
ID_INLINE int idBinSearch_GreaterEqual( const type* array, const int arraySize, const type& value )
{
	int len	   = arraySize;
	int mid	   = len;
	int offset = 0;
	int res	   = 0;
	while( mid > 0 ) {
		mid = len >> 1;
		if( array[offset + mid] >= value ) {
			res = 0;
		} else {
			offset += mid;
			res = 1;
		}
		len -= mid;
	}
	return offset + res;
}

#endif /* !__BINSEARCH_H__ */
