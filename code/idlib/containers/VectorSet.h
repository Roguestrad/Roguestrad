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

/*
===============================================================================

	Vector Set

	Creates a set of vectors without duplicates.

===============================================================================
*/

template<class type, int dimension>
class idVectorSet : public idList<type>
{
public:
	/*!
		\brief Initializes an empty idVectorSet with default hash parameters and zeroed box size values.

		The constructor initializes the hash table with a default size based on the box hash size and a starting capacity of 128. It also sets the box hash size to 16 and clears the box inverse size
	   and half size arrays, initializing them to zero for all dimensions.

	*/
	idVectorSet();

	/*!
		\brief Initializes a vector set with the specified minimums, maximums, hash size, and initial size.

		This constructor sets up the internal data structures of the vector set using the provided bounding box coordinates, hash table size, and initial capacity. It delegates the actual
	   initialization logic to the Init method.

		\param mins The minimum coordinates of the bounding box
		\param maxs The maximum coordinates of the bounding box
		\param boxHashSize The size of the hash table used for spatial partitioning
		\param initialSize The initial number of elements the vector set can hold
	*/
	idVectorSet( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Returns the total size in bytes of all memory allocated by this vector set.
	size_t Allocated() const { return idList<type>::Allocated() + hash.Allocated(); }

	//! Returns the total size in bytes of the allocated memory for this vector instance.
	size_t Size() const { return sizeof( *this ) + Allocated(); }

	/*!
		\brief Initializes the vector set with minimum and maximum bounds, hash table size, and initial capacity

		This function sets up the internal state of the vector set by initializing the list size, clearing the hash table, and computing inverse and half box sizes for spatial partitioning. It
	   prepares the data structure for storing and hashing vectors within the specified bounds.

		\param mins The minimum bounds of the vector set
		\param maxs The maximum bounds of the vector set
		\param boxHashSize The size of the hash grid in each dimension
		\param initialSize The initial number of elements the list can hold
	*/
	void   Init( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	/*!
		\brief Resizes the vector set and its associated hash table to the specified new size

		This function adjusts the internal storage of the vector set to accommodate the specified number of elements. It updates both the base list storage and the hash table to match the new size,
	   ensuring consistent state between the two data structures. The resize operation affects both the capacity and the index mapping used for hash table lookups.

		\param newSize The new size to set for the vector set and its hash table
	*/
	void   ResizeIndex( const int newSize );

	//! Clears the vector set and its hash table.
	void   Clear();

	/*!
		\brief Finds an existing vector in the set or adds a new one if not found, using a hash-based lookup with epsilon tolerance for floating-point comparison

		This function implements a hash-based lookup to find a vector that is approximately equal to the input vector within the specified epsilon tolerance. It uses a multi-dimensional hash table to
	   efficiently search for existing vectors. If no matching vector is found, it adds the new vector to the set and returns its index. The function handles n-dimensional vectors and uses a bounding
	   box to determine hash keys. The epsilon parameter controls the tolerance for floating-point comparisons, and the function asserts that epsilon does not exceed the box half size for each
	   dimension.

		\param v The vector to find or add to the set
		\param epsilon The epsilon tolerance for floating-point comparisons between vectors
		\return The index of the found or newly added vector in the set
	*/
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

/*
===============================================================================

	Vector Subset

	Creates a subset without duplicates from an existing list with vectors.

===============================================================================
*/

template<class type, int dimension>
class idVectorSubset
{
public:
	/*!
		\brief Initializes a new instance of the idVectorSubset class with default values.

		The constructor initializes the internal hash table, sets the box hash size to 16, and clears the box inverse size and half size arrays. This ensures the object is in a valid state for
	   subsequent operations.

	*/
	idVectorSubset();

	/*!
		\brief Initializes a new instance of the idVectorSubset class with specified bounding box parameters and hash table size.

		This constructor initializes the idVectorSubset object with minimum and maximum coordinates that define the bounding box, a hash table size for spatial partitioning, and an initial capacity
	   for the internal data structure. The initialization is delegated to the Init method which sets up the internal state for efficient spatial queries and storage of vector data within the defined
	   bounds.

		\param mins The minimum coordinates of the bounding box
		\param maxs The maximum coordinates of the bounding box
		\param boxHashSize The size of the hash table used for spatial partitioning
		\param initialSize The initial number of elements the internal storage can hold
	*/
	idVectorSubset( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Returns the total amount of memory allocated by this vector subset instance in bytes.
	size_t Allocated() const { return idList<type>::Allocated() + hash.Allocated(); }

	//! Returns the total size in bytes of memory allocated for this vector subset
	size_t Size() const { return sizeof( *this ) + Allocated(); }

	/*!
		\brief Initializes the vector subset with minimum and maximum bounds, hash size, and initial capacity

		This function sets up the internal data structures for a vector subset by clearing the hash table and initializing the bounding box parameters. It calculates inverse and half sizes for each
	   dimension based on the provided minimum and maximum values and the hash size. The function is intended to be inlined for performance reasons.

		\param mins The minimum bounds of the vector subset
		\param maxs The maximum bounds of the vector subset
		\param boxHashSize The size of the hash grid for spatial partitioning
		\param initialSize The initial capacity of the hash table
	*/
	void   Init( const type& mins, const type& maxs, const int boxHashSize, const int initialSize );

	//! Clears the vector subset list and its hash table
	void   Clear();

	/*!
		\brief Finds a vector in a list that matches the given vector within the specified epsilon tolerance, returning either the index of the matching vector or the index of the new vector if no
	   match is found.

		This function performs a hash-based lookup to find a vector in a list that is within a specified epsilon tolerance of the given vector. It uses a subset of the vector space to optimize the
	   search. If a matching vector is found, it returns the index of that vector. Otherwise, it adds the new vector to the hash table and returns its index. The function is designed to efficiently
	   deduplicate vectors in large datasets, commonly used for vertex and texture coordinate remapping in 3D rendering.

		\param vectorList Pointer to the list of vectors to search through
		\param vectorNum Index of the vector in the list to find
		\param epsilon Tolerance value for comparing vector components
		\return Index of the matching vector in the list, or the index of the new vector if no match is found
	*/
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
