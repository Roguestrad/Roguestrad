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
#ifndef __ARRAY_H__
#define __ARRAY_H__

/*
================================================
idArray is a replacement for a normal C array.

int		myArray[ARRAY_SIZE];

becomes:

idArray<int,ARRAY_SIZE>	myArray;

Has no performance overhead in release builds, but
does index range checking in debug builds.

Unlike idTempArray, the memory is allocated inline with the
object, rather than on the heap.

Unlike idStaticList, there are no fields other than the
actual raw data, and the size is fixed.
================================================
*/
template<class T_, int numElements>
class idArray
{
public:
	//! Returns the number of elements in the array
	int		  Num() const { return numElements; }

	//! Returns the size in bytes of the memory allocated for the array storage.
	int		  ByteSize() const { return sizeof( ptr ); }

	//! Clears the entire array memory to zero.
	void	  Zero() { memset( ptr, 0, sizeof( ptr ) ); }

	/*!
		\brief Clear the entire array memory to a specific byte value

		This function sets all elements of the array to the specified byte value using memset. It operates on the entire memory block of the array, treating it as a contiguous sequence of bytes. The
	   fill value is applied to each byte of the array elements, not just the first byte of each element.

		\param fill The byte value to set each byte of the array to
	*/
	void	  Memset( const char fill ) { memset( ptr, fill, numElements * sizeof( *ptr ) ); }

	// array operators
	const T_& operator[]( int index ) const
	{
		assert( ( unsigned )index < ( unsigned )numElements );
		return ptr[index];
	}
	T_& operator[]( int index )
	{
		assert( ( unsigned )index < ( unsigned )numElements );
		return ptr[index];
	}

	/*!
		\brief Returns a pointer to the internal array data

		This function provides access to the underlying memory buffer of the array. It returns a constant pointer to the first element of the array, allowing read-only access to the stored elements.
	   The returned pointer is valid as long as the array object exists and is not modified.

		\return A constant pointer to the first element of the internal array
	*/
	const T_* Ptr() const { return ptr; }

	/*!
		\brief Returns a pointer to the internal data array managed by the idArray object.

		This method provides direct access to the underlying data buffer of the idArray. It is typically used when interfacing with functions that expect a raw pointer to an array of elements. The
	   returned pointer is valid as long as the idArray object itself remains alive and unmodified.

		\return A pointer to the first element of the internal data array.
	*/
	T_*		  Ptr() { return ptr; }

private:
	T_ ptr[numElements];
};

#define ARRAY_COUNT( arrayName ) ( sizeof( arrayName ) / sizeof( arrayName[0] ) )
#define ARRAY_DEF( arrayName )	 arrayName, ARRAY_COUNT( arrayName )

/*
================================================
id2DArray is essentially a typedef (as close as we can
get for templates before C++11 anyway) to make
declaring two-dimensional idArrays easier.

Usage:
	id2DArray< int, 5, 10 >::type someArray;

================================================
*/
template<class _type_, int _dim1_, int _dim2_>
struct id2DArray {
	typedef idArray<idArray<_type_, _dim2_>, _dim1_> type;
};

/*
================================================
idTupleSize
Generic way to get the size of a tuple-like type.
Add specializations as needed.
This is modeled after std::tuple_size from C++11,
which works for std::arrays also.
================================================
*/
template<class _type_>
struct idTupleSize;

template<class _type_, int _num_>
struct idTupleSize<idArray<_type_, _num_>> {
	enum { value = _num_ };
};

#endif // !__ARRAY_H__
