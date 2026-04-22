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

#ifndef __VECTORSET_H__
#define __VECTORSET_H__

/*!
	\class idVectorSet
	\brief A hash-based set implementation for vectors that efficiently manages collections of vectors within a bounded space.

	This class provides a specialized container for storing vectors in a bounded space using a hash table for efficient lookup and insertion operations. It is designed to handle vector data with a
   specified dimensionality and maintains an internal hash structure to organize vectors within a defined bounding volume. The implementation supports initialization with custom minimum and maximum
   bounds, hash table size, and initial capacity. The class inherits from idList, indicating it maintains a list-based structure for vector storage, while extending it with hash-based lookup
   capabilities. Memory management is handled through the inherited list structure, and the container supports operations to find existing vectors or add new ones based on a tolerance epsilon value.
   The hash table size and bounding volume parameters influence the performance characteristics of the container, particularly for large vector sets. The class is templated to support different vector
   types and dimensions, making it flexible for various spatial data management scenarios.

*/
template<class type, int dimension>
class idVectorSet : public idList<type>
{
public:
	//! Constructs an empty idVectorSet with default parameters.
	idVectorSet();

	/*!
		\brief Constructs an idVectorSet object with the specified minimums, maximums, hash size, and initial size

		The constructor initializes a vector set with the given bounding box dimensions specified by mins and maxs. It also sets up the hash table size for spatial partitioning and the initial number
	   of elements the set can hold

		\param mins Minimum coordinates of the bounding box
		\param maxs Maximum coordinates of the bounding box
		\param boxHashSize Size of the hash table used for spatial partitioning
		\param initialSize Initial number of elements the set can hold
	*/
	idVectorSet( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Returns the total size of allocated memory for the vector set.
	size_t Allocated() const { return idList<type>::Allocated() + hash.Allocated(); }

	//! Returns the total size of allocated memory for this vector including the size of the type
	size_t Size() const { return sizeof( *this ) + Allocated(); }

	/*!
		\brief Initializes the vector set with specified bounding box parameters, hash table size, and initial capacity.

		This function sets up the internal data structures of the vector set by initializing the hash table with the specified hash size and allocating initial memory. It also calculates inverse and
	   half box sizes for spatial indexing based on the provided minimum and maximum bounds.

		\param mins The minimum bounds of the spatial domain
		\param maxs The maximum bounds of the spatial domain
		\param boxHashSize The size of the hash table for spatial partitioning, determines the granularity of the grid
		\param initialSize The initial number of elements the vector set can hold before reallocation
	*/
	void   Init( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Resizes the vector set to the specified new size.
	void   ResizeIndex( const int newSize );

	//! Clears all elements from the vector set.
	void   Clear();

	//! Finds a vector in the set or adds it if not found
	int	   FindVector( const type& v, const float epsilon );

private:
	idHashIndex hash;
	type		mins;
	type		maxs;
	int			boxHashSize;
	float		boxInvSize[dimension];
	float		boxHalfSize[dimension];
};

template<class type, int dimension>
ID_INLINE idVectorSet<type, dimension>::idVectorSet()
{
	hash.Clear( idMath::IPow( boxHashSize, dimension ), 128 );
	boxHashSize = 16;
	memset( boxInvSize, 0, dimension * sizeof( boxInvSize[0] ) );
	memset( boxHalfSize, 0, dimension * sizeof( boxHalfSize[0] ) );
}

template<class type, int dimension>
ID_INLINE idVectorSet<type, dimension>::idVectorSet( const type& mins, const type& maxs, const int boxHashSize, const int initialSize )
{
	Init( mins, maxs, boxHashSize, initialSize );
}

template<class type, int dimension>
ID_INLINE void idVectorSet<type, dimension>::Init( const type& mins, const type& maxs, const int boxHashSize, const int initialSize )
{
	int	  i;
	float boxSize;

	idList<type>::AssureSize( initialSize );
	idList<type>::SetNum( 0 );

	hash.Clear( idMath::IPow( boxHashSize, dimension ), initialSize );

	this->mins		  = mins;
	this->maxs		  = maxs;
	this->boxHashSize = boxHashSize;

	for( i = 0; i < dimension; i++ ) {
		boxSize		   = ( maxs[i] - mins[i] ) / ( float )boxHashSize;
		boxInvSize[i]  = 1.0f / boxSize;
		boxHalfSize[i] = boxSize * 0.5f;
	}
}

template<class type, int dimension>
ID_INLINE void idVectorSet<type, dimension>::ResizeIndex( const int newSize )
{
	idList<type>::Resize( newSize );
	hash.ResizeIndex( newSize );
}

template<class type, int dimension>
ID_INLINE void idVectorSet<type, dimension>::Clear()
{
	idList<type>::Clear();
	hash.Clear();
}

template<class type, int dimension>
ID_INLINE int idVectorSet<type, dimension>::FindVector( const type& v, const float epsilon )
{
	int i, j, k, hashKey, partialHashKey[dimension];

	for( i = 0; i < dimension; i++ ) {
		assert( epsilon <= boxHalfSize[i] );
		partialHashKey[i] = ( int )( ( v[i] - mins[i] - boxHalfSize[i] ) * boxInvSize[i] );
	}

	for( i = 0; i < ( 1 << dimension ); i++ ) {
		hashKey = 0;
		for( j = 0; j < dimension; j++ ) {
			hashKey *= boxHashSize;
			hashKey += partialHashKey[j] + ( ( i >> j ) & 1 );
		}

		for( j = hash.First( hashKey ); j >= 0; j = hash.Next( j ) ) {
			const type& lv = ( *this )[j];
			for( k = 0; k < dimension; k++ ) {
				if( idMath::Fabs( lv[k] - v[k] ) > epsilon ) { break; }
			}
			if( k >= dimension ) { return j; }
		}
	}

	hashKey = 0;
	for( i = 0; i < dimension; i++ ) {
		hashKey *= boxHashSize;
		hashKey += ( int )( ( v[i] - mins[i] ) * boxInvSize[i] );
	}

	hash.Add( hashKey, idList<type>::Num() );

	// RB: compile fix for Fedora 17
	this->Append( v );
	// RB end

	return idList<type>::Num() - 1;
}

/*!
	\class idVectorSubset
	\brief A template class for managing subsets of vectors with bounding box hashing and epsilon-based vector matching.

	This class provides functionality for storing and managing vector subsets within a defined bounding box using a hash table for efficient lookup. It supports initialization with specific bounds and
   hash table size, memory tracking through allocated and size methods, and finding vectors within a given epsilon tolerance. The template parameters allow for flexibility in vector types and
   dimensions. The class is designed for scenarios where vector data needs to be organized and retrieved efficiently based on spatial proximity and exact matches.

*/
template<class type, int dimension>
class idVectorSubset
{
public:
	//! Constructs a new idVectorSubset object with default values.
	idVectorSubset();

	/*!
		\brief Initializes a vector subset with specified bounding box and hash table size

		This constructor initializes a vector subset using the provided minimum and maximum coordinates to define the bounding box of the subset. It also sets up the hash table size for efficient
	   spatial indexing and specifies an initial size for the subset

		\param mins The minimum coordinates of the bounding box
		\param maxs The maximum coordinates of the bounding box
		\param boxHashSize The size of the hash table used for spatial indexing
		\param initialSize The initial number of elements the subset can hold
	*/
	idVectorSubset( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Returns the total amount of memory allocated by the vector subset.
	size_t Allocated() const { return idList<type>::Allocated() + hash.Allocated(); }

	//! Returns the total size of allocated memory for the vector subset including the size of the type itself.
	size_t Size() const { return sizeof( *this ) + Allocated(); }

	/*!
		\brief Initializes the vector subset with minimum and maximum bounds, hash size, and initial capacity

		Sets up the internal data structures for the vector subset using the provided bounding box coordinates, hash table size, and initial allocation size. Calculates inverse and half box sizes for
	   spatial indexing operations

		\param mins Minimum bounds of the vector subset
		\param maxs Maximum bounds of the vector subset
		\param boxHashSize Size of the hash grid for spatial partitioning
		\param initialSize Initial capacity for the hash table
	*/
	void   Init( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Clears all elements from the vector subset and its hash table.
	void   Clear();

	//! Returns the index of a previously found vector that matches the given vector within the specified epsilon tolerance, or the input vector number if no match is found.
	int	   FindVector( const type* vectorList, const int vectorNum, const float epsilon );

private:
	idHashIndex hash;
	type		mins;
	type		maxs;
	int			boxHashSize;
	float		boxInvSize[dimension];
	float		boxHalfSize[dimension];
};

template<class type, int dimension>
ID_INLINE idVectorSubset<type, dimension>::idVectorSubset()
{
	hash.Clear( idMath::IPow( boxHashSize, dimension ), 128 );
	boxHashSize = 16;
	memset( boxInvSize, 0, dimension * sizeof( boxInvSize[0] ) );
	memset( boxHalfSize, 0, dimension * sizeof( boxHalfSize[0] ) );
}

template<class type, int dimension>
ID_INLINE idVectorSubset<type, dimension>::idVectorSubset( const type& mins, const type& maxs, const int boxHashSize, const int initialSize )
{
	Init( mins, maxs, boxHashSize, initialSize );
}

template<class type, int dimension>
ID_INLINE void idVectorSubset<type, dimension>::Init( const type& mins, const type& maxs, const int boxHashSize, const int initialSize )
{
	int	  i;
	float boxSize;

	hash.Clear( idMath::IPow( boxHashSize, dimension ), initialSize );

	this->mins		  = mins;
	this->maxs		  = maxs;
	this->boxHashSize = boxHashSize;

	for( i = 0; i < dimension; i++ ) {
		boxSize		   = ( maxs[i] - mins[i] ) / ( float )boxHashSize;
		boxInvSize[i]  = 1.0f / boxSize;
		boxHalfSize[i] = boxSize * 0.5f;
	}
}

template<class type, int dimension>
ID_INLINE void idVectorSubset<type, dimension>::Clear()
{
	idList<type>::Clear();
	hash.Clear();
}

template<class type, int dimension>
ID_INLINE int idVectorSubset<type, dimension>::FindVector( const type* vectorList, const int vectorNum, const float epsilon )
{
	int			i, j, k, hashKey, partialHashKey[dimension];
	const type& v = vectorList[vectorNum];

	for( i = 0; i < dimension; i++ ) {
		assert( epsilon <= boxHalfSize[i] );
		partialHashKey[i] = ( int )( ( v[i] - mins[i] - boxHalfSize[i] ) * boxInvSize[i] );
	}

	for( i = 0; i < ( 1 << dimension ); i++ ) {
		hashKey = 0;
		for( j = 0; j < dimension; j++ ) {
			hashKey *= boxHashSize;
			hashKey += partialHashKey[j] + ( ( i >> j ) & 1 );
		}

		for( j = hash.First( hashKey ); j >= 0; j = hash.Next( j ) ) {
			const type& lv = vectorList[j];
			for( k = 0; k < dimension; k++ ) {
				if( idMath::Fabs( lv[k] - v[k] ) > epsilon ) { break; }
			}
			if( k >= dimension ) { return j; }
		}
	}

	hashKey = 0;
	for( i = 0; i < dimension; i++ ) {
		hashKey *= boxHashSize;
		hashKey += ( int )( ( v[i] - mins[i] ) * boxInvSize[i] );
	}

	hash.Add( hashKey, vectorNum );
	return vectorNum;
}

#endif /* !__VECTORSET_H__ */
