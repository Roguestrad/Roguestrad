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

/*!
	\class idArray
	\brief Fixed-size array template class for storing elements of a specified type.

	This class provides a fixed-size array implementation with compile-time known size. It stores elements in a contiguous block of memory and provides direct access to elements via indexing
   operators. The array size is determined at compile time through the template parameter numElements. Unlike dynamic containers, this implementation has no additional fields beyond the raw data,
   making it memory efficient for fixed-size collections. The class offers methods for retrieving array size, memory usage, and direct memory access, as well as initialization and manipulation
   functions. Memory management is handled through direct memory operations, with zeroing and filling capabilities.

*/
template<class T_, int numElements>
class idArray
{
public:
	//! Returns the number of elements in the array
	int		  Num() const { return numElements; }

	//! Returns the number of bytes the array takes up.
	int		  ByteSize() const { return sizeof( ptr ); }

	//! Sets all elements of the array to zero using memset.
	void	  Zero() { memset( ptr, 0, sizeof( ptr ) ); }

	// memset the entire array to a specific value
	void	  Memset( const char fill ) { memset( ptr, fill, numElements * sizeof( *ptr ) ); }

	//! Provides read-only access to an element at the specified index in the array
	const T_& operator[]( int index ) const
	{
		assert( ( unsigned )index < ( unsigned )numElements );
		return ptr[index];
	}

	//! Returns a reference to the element at the specified index in the array
	T_& operator[]( int index )
	{
		assert( ( unsigned )index < ( unsigned )numElements );
		return ptr[index];
	}

	//! Returns a pointer to the internal data array.
	const T_* Ptr() const { return ptr; }

	//! Returns a pointer to the internal data array
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
