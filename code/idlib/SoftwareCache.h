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
#ifndef __SOFTWARECACHE_H__
#define __SOFTWARECACHE_H__

#ifdef _MSC_VER						  // DG: #pragma warning is MSVC specific
	#pragma warning( disable : 4324 ) // structure was padded due to __declspec(align())
#endif
/*
================================================================================================

On-Demand Streamed Objects and Arrays

idODSObject					// DMA in a single object
idODSCachedObject			// DMA in a single object through a software cache
idODSArray					// DMA in an array with objects
idODSIndexedArray			// DMA gather from an array with objects
idODSStreamedArray			// overlapped DMA streaming of an array with objects
idODSStreamedIndexedArray	// overlapped DMA gather from an array with objects

On the SPU the 'idODSObject' streams the data into temporary memory using the DMA controller
and the object constructor immediately waits for the DMA transfer to complete. In other words
there is no caching and every random memory access incurs full memory latency. This should be
used to stream in objects that are only used once at unpredictable times.

The 'idODSCachedObject' uses an object based software cache on the SPU which is useful for
streaming in objects that may be used repeatedly or which usage can be predicted allowing
the objects to be prefetched.

	class idMyType {};
	class idMyCache : public idSoftwareCache< idMyType, 8, 4 > {};
	idMyCache myCache;
	idMyType * myPtr;
	idODSCachedObject< idMyType, idMyCache > myODS( myPtr, myCache );

The 'idSoftwareCache' implements a Prefetch() function that can be used to prefetch whole
objects into the cache well before they are needed. However, any idODSObject, idODSArray,
idODSIndexedArray etc. after calling the Prefetch() function will have to wait for the
prefetch to complete. In other words, make sure there is enough "work" done in between
a Prefetch() call and the first next idODS* object.

The 'idODSArray' streams in a block of objects that are tightly packed in memory.

The 'idODSIndexedArray' is used to gather a number of objects that are not necessarily
contiguous in memory. On the SPU a DMA-list is used in the 'idODSIndexedArray' constructor
to efficiently gather all the objects.

The 'idODSStreamedArray' is used for sequentially reading a large input array. Overlapped
streaming is used where one batch of array elements can be accessed while the next batch
is being streamed in.

The 'idODSStreamedIndexedArray' is used for gathering elements from an array using a
sequentially read index. Overlapped streaming is used for both the index and the array
elements where one batch of array elements can be accessed while the next batch of
indices/array elements is being streamed in.

Outside the SPU, data is never copied to temporary memory because this would cause
significant load-hit-store penalties. Instead, the object constructor issues prefetch
instructions where appropriate and only maintains pointers to the actual data. In the
case of 'idODSObject' or 'idODSCachedObject' the class is no more than a simple wrapper
of a pointer and the class should completely compile away with zero overhead.

COMMON MISTAKES:

1. When using ODS objects do not forget to set the "globalDmaTag" that is used to issue
   and wait for DMAs.

   void cellSpursJobMain2( CellSpursJobContext2 * stInfo, CellSpursJob256 * job ) {
	  globalDmaTag = stInfo->dmaTag;	// for ODS objects
   }

2. ODS objects can consume quite a bit of stack space. You may have to increase the SPU job
   stack size. For instance:

   job->header.sizeStack = SPURS_QUADWORDS( 16 * 1024 );   // the ODS objects get pretty large

   Make sure you measure the size of each ODS object and if there are recursive functions
   using ODS objects make sure the recursion is bounded. When the stack overflows the scratch
   and output memory may get overwritten and the results will be undefined. Finding stack
   overflows is painful.

3. While you can setup a regular DMA list entry to use a NULL pointer with zero size, do not use
   a NULL pointer for a cache DMA list entry. This confuses SPURS and can cause your SPU binary
   to get corrupted.

================================================================================================
*/

extern uint32 globalDmaTag;

#define MAX_DMA_SIZE	 ( 1 << 14 )
#define ODS_ROUND16( x ) ( ( x + 15 ) & ~15 )

enum streamBufferType_t { SBT_DOUBLE = 2, SBT_QUAD = 4 };

/*
================================================================================================

	non-SPU code

================================================================================================
*/

/*
================================================
idSoftwareCache
================================================
*/
template<typename _type_, int _entries_ = 8, int _associativity_ = 4, bool aligned = false>
class ALIGNTYPE128 idSoftwareCache
{
public:
	/*!
		\brief Prefetches the specified object into cache

		This function performs a prefetch operation on the given object to load it into cache, potentially improving performance by reducing cache misses. The prefetch is done with a hint of 0, which
	   typically indicates a read operation without specific cache level preference.

		\param obj Pointer to the object to prefetch into cache
	*/
	void Prefetch( const _type_* obj ) { ::Prefetch( obj, 0 ); }
};

/*!
	\brief Returns a const reference to the internal object pointed to by objectPtr.

	This function provides access to the object that the idODSObject wrapper is managing. It simply dereferences the internal pointer and returns a const reference to the object. The function is
   marked as const, indicating it does not modify the wrapper object's state.

	\return A const reference to the object that objectPtr points to.
*/
template<typename _type_>
class idODSObject

/*!
	\brief Constructs an idODSObject instance by storing a pointer to the given object.

	The constructor initializes the idODSObject by taking a pointer to an object of type _type_ and storing it internally. This allows the idODSObject to reference the original object without owning
   it, meaning the object pointed to by obj must remain valid for the lifetime of this idODSObject instance.

	\param obj Pointer to the object to be referenced by this idODSObject instance.
*/
{
public:
	/*!
		\brief Initializes an idODSObject with a pointer to a _type_ object.

		Constructs an idODSObject instance by storing the provided pointer to a _type_ object. The pointer is initialized to the value passed in the constructor parameter.

		\param obj Pointer to the _type_ object to be stored
	*/
	idODSObject( const _type_* obj ) :
		objectPtr( obj )
	{
	}
	operator const _type_&() const { return *objectPtr; }
	const _type_* operator->() const { return objectPtr; }

	/*!
		\brief Returns a constant reference to the internal object managed by this idODSObject instance

		This function provides access to the underlying object that is being managed by the idODSObject template class. It returns a const reference, ensuring that the object cannot be modified
	   through this interface. The function is marked as const, indicating it does not modify the state of the idODSObject instance itself. This is a common pattern in smart pointer or wrapper classes
	   where you need to access the wrapped object while maintaining const-correctness.

		\return A constant reference to the internal object of type _type_ that this idODSObject instance manages
	*/
	const _type_& Get() const { return *objectPtr; }

	/*!
		\brief Returns a constant pointer to the internal object managed by this idODSObject instance

		This function provides access to the underlying object that is being managed by the idODSObject template class. It returns a const pointer, ensuring that the object cannot be modified through
	   this interface. The function is marked as const, indicating it does not modify the state of the idODSObject instance itself. This is a common pattern in smart pointer or wrapper classes where
	   you need to access the wrapped object while maintaining const-correctness.

		\return A constant pointer to the internal object of type _type_ that this idODSObject instance manages
	*/
	const _type_* Ptr() const { return objectPtr; }

	/*!
		\brief Returns a pointer to the original object stored in the idODSObject.

		This method provides access to the underlying object pointer that was stored within the idODSObject instance. It is a const method, meaning it does not modify the object's state and returns a
	   const pointer to the original object type.

		\return A const pointer to the original object of type _type_ that is managed by this idODSObject instance
	*/
	const _type_* OriginalPtr() const { return objectPtr; }

private:
	const _type_* objectPtr;
};

/*!
	\brief Returns a constant reference to the cached object managed by this instance.

	This function provides access to the actual object being cached. It returns a constant reference to avoid modification of the cached object through this interface. The object is stored internally
   in a pointer and dereferenced here.

	\return A constant reference to the cached object of type _type_
*/
template<typename _type_, typename _cache_>
class idODSCachedObject

/*!
	\brief Constructs an idODSCachedObject with the specified object pointer and cache reference.

	The constructor initializes the object pointer member with the provided object and stores the cache reference for later use. This is a simple constructor that sets up the internal state of the
   cached object wrapper.

	\param obj Pointer to the object to be cached
	\param cache Reference to the cache that will manage this object
*/
{
public:
	/*!
		\brief Constructs an idODSCachedObject instance with the given object pointer and cache reference.

		The constructor initializes the objectPtr member with the provided object pointer and stores a reference to the cache.

		\param obj Pointer to the object to be cached
		\param cache Reference to the cache that will manage the object
	*/
	idODSCachedObject( const _type_* obj, _cache_& cache ) :
		objectPtr( obj )
	{
	}
	operator const _type_&() const { return *objectPtr; }
	const _type_* operator->() const { return objectPtr; }

	/*!
		\brief Returns a const reference to the cached object managed by this instance

		This method provides read-only access to the underlying object stored in the cache. It is a simple getter that returns the internal object reference without modifying the cache state. The
	   returned reference is guaranteed to be valid as long as this cached object instance exists

		\return A const reference to the cached object of type _type_
	*/
	const _type_& Get() const { return *objectPtr; }

	/*!
		\brief Returns a const pointer to the cached object managed by this instance.

		This method provides read-only access to the underlying object stored in the cache. It is a simple getter that returns the internal object pointer without modifying the cache state.

		\return A const pointer to the cached object of type _type_.
	*/
	const _type_* Ptr() const { return objectPtr; }

	/*!
		\brief Returns a pointer to the original object stored in the cache.

		This function provides access to the underlying original object that is being cached. It is typically used to retrieve the actual object data when working with cached objects.

		\return A constant pointer to the original object of type _type_ stored in the cache
	*/
	const _type_* OriginalPtr() const { return objectPtr; }

private:
	const _type_* objectPtr;
};

/*
================================================
idODSArray
================================================
*/
template<typename _type_, int max>
class idODSArray
{
public:
	/*!
		\brief Constructs an idODSArray object by initializing it with a pointer to an array and its size.

		The constructor initializes the idODSArray object with the provided array pointer and number of elements. It asserts that the number of elements does not exceed the maximum allowed size and
	   prefetches the array data for better performance.

		\param array Pointer to the array of elements to initialize with
		\param num Number of elements in the array
		\throws Throws an assertion error if the number of elements exceeds the maximum allowed size.
	*/
	idODSArray( const _type_* array, int num ) :
		arrayPtr( array ),
		arrayNum( num )
	{
		assert( num <= max );
		Prefetch( array, 0 );
	}
	const _type_& operator[]( int index ) const
	{
		assert( index >= 0 && index < arrayNum );
		return arrayPtr[index];
	}

	/*!
		\brief Returns a pointer to the internal array managed by the idODSArray container

		This method provides access to the underlying array storage of the idODSArray container. It returns a const pointer to the array data, allowing read-only access to the elements stored in the
	   container. The returned pointer is valid as long as the idODSArray instance exists and is not modified. This is a utility method for interfacing with APIs that expect a raw array pointer, such
	   as graphics functions or memory operations.

		\return A constant pointer to the first element of the internal array
	*/
	const _type_* Ptr() const { return arrayPtr; }

	//! Returns the number of elements in the array
	const int	  Num() const { return arrayNum; }

private:
	const _type_* arrayPtr;
	int			  arrayNum;
};

/*
================================================
idODSIndexedArray
================================================
*/
template<typename _elemType_, typename _indexType_, int max>
class idODSIndexedArray
{
public:
	/*!
		\brief Initializes an indexed array with the given array and index data

		Constructs an idODSIndexedArray object by copying elements from the provided array based on the index mappings. The function pre-fetches data to improve performance and stores pointers to the
	   indexed elements. The number of elements must not exceed the maximum capacity of the array.

		\param array Pointer to the source array of elements
		\param index Pointer to the index array that maps to elements in the source array
		\param num Number of elements to initialize
		\throws assertion failure if num exceeds the maximum capacity 'max'
	*/
	idODSIndexedArray( const _elemType_* array, const _indexType_* index, int num ) :
		arrayNum( num )
	{
		assert( num <= max );
		for( int i = 0; i < num; i++ ) {
			Prefetch( arrayPtr, abs( index[i] ) * sizeof( _elemType_ ) );
			arrayPtr[i] = array + abs( index[i] );
		}
	}
	const _elemType_& operator[]( int index ) const
	{
		assert( index >= 0 && index < arrayNum );
		return *arrayPtr[index];
	}

	/*!
		\brief Ensures the array size is a multiple of four by replicating the first element.

		This function adjusts the size of the array to be a multiple of four by repeatedly copying the first element to the end of the array. It is designed to work only with arrays whose maximum size
	   is a multiple of four, as enforced by an assertion. The function continues until the current array size is a multiple of four.

		\throws Assertion failure if max is not a multiple of four.
	*/
	void ReplicateUpToMultipleOfFour()
	{
		assert( ( max & 3 ) == 0 );
		while( ( arrayNum & 3 ) != 0 ) {
			arrayPtr[arrayNum++] = arrayPtr[0];
		}
	}

private:
	const _elemType_* arrayPtr[max];
	int				  arrayNum;
};

/*
================================================
idODSStreamedOutputArray
================================================
*/
template<typename _type_, int _bufferSize_>
class ALIGNTYPE16 idODSStreamedOutputArray
{
public:
	/*!
		\brief Constructor for idODSStreamedOutputArray that initializes the output array and its parameters

		Initializes the idODSStreamedOutputArray with the provided array, number of elements pointer, and maximum elements.
		The constructor sets up internal variables for tracking the local number of elements, output array, number of elements, and maximum elements.
		It also performs compile-time assertions to ensure that the buffer size is a power of two, that the buffer size multiplied by the size of the type is divisible by 16, and that the buffer size
	   multiplied by the size of the type is less than MAX_DMA_SIZE. Additionally, it asserts that the array is 16-byte aligned.

		\param array Pointer to the array to be used for output
		\param numElements Pointer to the number of elements in the output array
		\param maxElements Maximum number of elements allowed in the output array
	*/
	idODSStreamedOutputArray( _type_* array, int* numElements, int maxElements ) :
		localNum( 0 ),
		outArray( array ),
		outNum( numElements ),
		outMax( maxElements )
	{
		compile_time_assert( CONST_ISPOWEROFTWO( _bufferSize_ ) );
		compile_time_assert( ( ( _bufferSize_ * sizeof( _type_ ) ) & 15 ) == 0 );
		compile_time_assert( _bufferSize_ * sizeof( _type_ ) < MAX_DMA_SIZE );
		assert_16_byte_aligned( array );
	}
	~idODSStreamedOutputArray() { *outNum = localNum; }

	//! Returns the number of elements in the array.
	int	 Num() const { return localNum; }

	/*!
		\brief Appends an element to the internal array, growing it if necessary

		This function adds a new element to the internal array of the streamed output array. It checks that there is available space in the array before appending the element. The function uses assert
	   to ensure that the current number of elements is less than the maximum allowed. The array will grow as needed to accommodate new elements up to the specified buffer size.

		\param element The element to be added to the array
		\throws assertion failure if the array is already at maximum capacity
	*/
	void Append( _type_ element )
	{
		assert( localNum < outMax );
		outArray[localNum++] = element;
	}

	/*!
		\brief Returns a reference to the next available element in the output array, incrementing the internal counter

		This function provides access to the next available slot in a pre-allocated output array. It is designed to be used in scenarios where a fixed-size buffer is maintained and elements are added
	   sequentially. The function assumes that the internal counter localNum is properly managed and that the buffer size outMax is sufficient to prevent overflow. Each call to Alloc() returns a
	   reference to the next element in the array and increments the internal counter, allowing for sequential allocation of elements.

		\return A reference to the next available element in the output array
		\throws assertion failure if localNum reaches outMax
	*/
	_type_& Alloc()
	{
		assert( localNum < outMax );
		return outArray[localNum++];
	}

private:
	int		localNum;
	_type_* outArray;
	int*	outNum;
	int		outMax;
};

/*
================================================
idODSStreamedArray
================================================
*/
template<typename _type_, int _bufferSize_, streamBufferType_t _sbt_ = SBT_DOUBLE, int _roundUpToMultiple_ = 1>
class ALIGNTYPE16 idODSStreamedArray
{
public:
	/*!
		\brief Initializes a streamed array wrapper for efficient memory access with buffered retrieval

		Constructs an idODSStreamedArray object that provides buffered access to an input array. The constructor sets up internal state tracking for the cached array range and stream position, and
	   fetches the first batch of elements. It performs compile-time assertions to verify buffer size constraints and alignment requirements, and runtime assertions to validate input array alignment
	   and address validity. The number of elements is rounded up to ensure proper alignment with the specified multiple for efficient memory access patterns.

		\param array Input array to wrap for streaming access
		\param numElements Number of elements in the input array
	*/
	idODSStreamedArray( const _type_* array, const int numElements ) :
		cachedArrayStart( 0 ),
		cachedArrayEnd( 0 ),
		streamArrayEnd( 0 ),
		inArray( array ),
		inArrayNum( numElements ),
		inArrayNumRoundedUp( numElements )
	{
		compile_time_assert( CONST_ISPOWEROFTWO( _bufferSize_ ) );
		compile_time_assert( ( ( _bufferSize_ * sizeof( _type_ ) ) & 15 ) == 0 );
		compile_time_assert( _bufferSize_ * sizeof( _type_ ) < MAX_DMA_SIZE );
		compile_time_assert( _roundUpToMultiple_ >= 1 );
		assert_16_byte_aligned( array );
		assert( ( uintptr_t )array > _bufferSize_ * sizeof( _type_ ) );
		// Fetch the first batch of elements.
		FetchNextBatch();
		// Calculate the rounded up size here making the mod effectively for free because we have to wait
		// for memory access anyway while the above FetchNextBatch() does not need the rounded up size yet.
		inArrayNumRoundedUp += _roundUpToMultiple_ - 1;
		inArrayNumRoundedUp -= inArrayNumRoundedUp % ( ( _roundUpToMultiple_ > 1 ) ? _roundUpToMultiple_ : 1 );
	}

	/*!
		\brief Destructor for the idODSStreamedArray class that flushes the accessible portion of the array.

		This destructor is responsible for cleaning up the idODSStreamedArray object by ensuring that any data in the accessible portion of the array is properly flushed. It calculates the range of
	   the array to be flushed based on the cached array start and end positions, converting these positions to byte offsets using the size of the data type stored in the array.

	*/
	~idODSStreamedArray()
	{
		// Flush the accessible part of the array.
		FlushArray( inArray, cachedArrayStart * sizeof( _type_ ), cachedArrayEnd * sizeof( _type_ ) );
	}

	/*!
		\brief Fetches the next batch of array elements for streaming access and returns the index after the new batch.

		This function manages a streaming buffer for accessing array elements in a controlled manner. It updates the cached array bounds to reflect the current batch of elements that can be accessed.
	   When not all elements have been streamed, it calculates the new cached array range based on the buffer size and streaming behavior. It also handles flushing old elements that are no longer
	   accessible and prefetching the next batch for performance. The function supports quad-buffering semantics, allowing access to elements from two previous batches. The return value indicates the
	   position after the current batch, which can be used to determine the range of accessible elements through the indexing operator.

		\return The index immediately following the newly fetched batch of array elements, or the rounded-up array size if all elements have been streamed
	*/
	int FetchNextBatch()
	{
		// If not everything has been streamed already.
		if( cachedArrayEnd < inArrayNum ) {
			cachedArrayEnd	 = streamArrayEnd;
			cachedArrayStart = Max( cachedArrayEnd - _bufferSize_ * ( _sbt_ - 1 ), 0 );

			// Flush the last batch of elements that is no longer accessible.
			FlushArray( inArray, ( cachedArrayStart - _bufferSize_ ) * sizeof( _type_ ), cachedArrayStart * sizeof( _type_ ) );

			// Prefetch the next batch of elements.
			if( streamArrayEnd < inArrayNum ) {
				streamArrayEnd = Min( streamArrayEnd + _bufferSize_, inArrayNum );
				for( unsigned int offset = cachedArrayEnd * sizeof( _type_ ); offset < streamArrayEnd * sizeof( _type_ ); offset += CACHE_LINE_SIZE ) {
					Prefetch( inArray, offset );
				}
			}
		}
		return ( cachedArrayEnd == inArrayNum ) ? inArrayNumRoundedUp : cachedArrayEnd;
	}

	// Provides access to the elements starting at the index returned by the next-to-last call
	// to FetchNextBach() (or zero if only called once so far) up to (excluding) the index
	// returned by the last call to FetchNextBatch(). When quad-buffering, the elements starting
	// at the index returned by the second-from-last call to FetchNextBatch() can still be accessed.
	// This is useful when the algorithm needs to successively access an odd number of elements
	// at the same time that may cross a single buffer boundary.
	const _type_& operator[]( int index ) const
	{
		assert( ( index >= cachedArrayStart && index < cachedArrayEnd ) || ( cachedArrayEnd == inArrayNum && index >= inArrayNum && index < inArrayNumRoundedUp ) );
		if( _roundUpToMultiple_ > 1 ) { index &= ( index - inArrayNum ) >> 31; }
		return inArray[index];
	}

private:
	int			  cachedArrayStart;
	int			  cachedArrayEnd;
	int			  streamArrayEnd;
	const _type_* inArray;
	int			  inArrayNum;
	int			  inArrayNumRoundedUp;

	/*!
		\brief Flushes cache lines for a specified range within a given array

		This function flushes cache lines for a specified range within a given array. It calculates the appropriate cache line boundaries based on the input parameters and ensures that only complete
	   cache lines within the specified range are flushed. The function uses alignment constants to ensure proper cache line granularity and avoids flushing data that may still be partially accessible
	   or extends beyond the array boundaries.

		\param flushArray Pointer to the base address of the array to flush
		\param flushStart Starting offset within the array from which to begin flushing
		\param flushEnd Ending offset within the array at which to stop flushing
	*/
	static void	  FlushArray( const void* flushArray, int flushStart, int flushEnd )
	{
#if 0
		// arrayFlushBase is rounded up so we do not flush anything before the array.
		// arrayFlushStart is rounded down so we start right after the last cache line that was previously flushed.
		// arrayFlushEnd is rounded down so we do not flush a cache line that holds data that may still be partially
		// accessible or a cache line that stretches beyond the end of the array.
		const uintptr_t arrayAddress = ( uintptr_t )flushArray;
		const uintptr_t arrayFlushBase = ( arrayAddress + CACHE_LINE_SIZE - 1 ) & ~( CACHE_LINE_SIZE - 1 );
		const uintptr_t arrayFlushStart = ( arrayAddress + flushStart ) & ~( CACHE_LINE_SIZE - 1 );
		const uintptr_t arrayFlushEnd = ( arrayAddress + flushEnd ) & ~( CACHE_LINE_SIZE - 1 );
		for( uintptr_t offset = Max( arrayFlushBase, arrayFlushStart ); offset < arrayFlushEnd; offset += CACHE_LINE_SIZE )
		{
			FlushCacheLine( flushArray, offset - arrayAddress );
		}
#endif
	}
};

/*
================================================
idODSStreamedIndexedArray

For gathering elements from an array using a sequentially read index.
This uses overlapped streaming for both the index and the array elements
where one batch of indices and/or array elements can be accessed while
the next batch is being streamed in.

NOTE: currently the size of array elements must be a multiple of 16 bytes.
An index with offsets and more complex logic is needed to support other sizes.
================================================
*/
template<typename _elemType_, typename _indexType_, int _bufferSize_, streamBufferType_t _sbt_ = SBT_DOUBLE, int _roundUpToMultiple_ = 1>
class ALIGNTYPE16 idODSStreamedIndexedArray
{
public:
	/*!
		\brief Constructor for idODSStreamedIndexedArray that initializes the streaming array and index data

		Initializes the streaming array and index data for efficient GPU memory access. Sets up internal state variables for tracking cached and streamed data ranges, and performs compile-time
	   assertions to ensure buffer sizes and alignment requirements are met. The constructor fetches the initial batches of indices and elements to prepare for streaming operations.

		\param array Pointer to the input array of elements
		\param numElements Number of elements in the input array
		\param index Pointer to the input index array
		\param numIndices Number of indices in the input index array
		\throws Compile-time assertions and runtime asserts for buffer size, alignment, and memory address requirements
	*/
	idODSStreamedIndexedArray( const _elemType_* array, const int numElements, const _indexType_* index, const int numIndices ) :
		cachedArrayStart( 0 ),
		cachedArrayEnd( 0 ),
		streamArrayEnd( 0 ),
		cachedIndexStart( 0 ),
		cachedIndexEnd( 0 ),
		streamIndexEnd( 0 ),
		inArray( array ),
		inArrayNum( numElements ),
		inIndex( index ),
		inIndexNum( numIndices ),
		inIndexNumRoundedUp( numIndices )
	{
		compile_time_assert( CONST_ISPOWEROFTWO( _bufferSize_ ) );
		compile_time_assert( ( ( _bufferSize_ * sizeof( _indexType_ ) ) & 15 ) == 0 );
		compile_time_assert( _bufferSize_ * sizeof( _indexType_ ) < MAX_DMA_SIZE );
		compile_time_assert( _bufferSize_ * sizeof( _elemType_ ) < MAX_DMA_SIZE );
		compile_time_assert( ( sizeof( _elemType_ ) & 15 ) == 0 ); // to avoid complexity due to cellDmaListGet
		compile_time_assert( _roundUpToMultiple_ >= 1 );
		assert_16_byte_aligned( index );
		assert_16_byte_aligned( array );
		assert( ( uintptr_t )index > _bufferSize_ * sizeof( _indexType_ ) );
		assert( ( uintptr_t )array > _bufferSize_ * sizeof( _elemType_ ) );
		// Fetch the first batch of indices.
		FetchNextBatch();
		// Fetch the first batch of elements and the next batch of indices.
		FetchNextBatch();
		// Calculate the rounded up size here making the mod effectively for free because we have to wait
		// for memory access anyway while the above FetchNextBatch() do not need the rounded up size yet.
		inIndexNumRoundedUp += _roundUpToMultiple_ - 1;
		inIndexNumRoundedUp -= inIndexNumRoundedUp % ( ( _roundUpToMultiple_ > 1 ) ? _roundUpToMultiple_ : 1 );
	}

	/*!
		\brief Destructor for the idODSStreamedIndexedArray class that flushes the accessible parts of the index and array.

		This destructor ensures that any cached data in the index and array portions of the stream are properly flushed to their respective destinations. It handles the cleanup of the streamed indexed
	   array by writing out the portions of the index and array that are currently cached, based on the start and end offsets stored in cachedIndexStart, cachedIndexEnd, cachedArrayStart, and
	   cachedArrayEnd.

	*/
	~idODSStreamedIndexedArray()
	{
		// Flush the accessible part of the index.
		FlushArray( inIndex, cachedIndexStart * sizeof( _indexType_ ), cachedIndexEnd * sizeof( _indexType_ ) );
		// Flush the accessible part of the array.
		FlushArray( inArray, cachedArrayStart * sizeof( _elemType_ ), cachedArrayEnd * sizeof( _elemType_ ) );
	}

	/*!
		\brief Fetches the next batch of array elements for streaming access and returns the index after the batch.

		This function manages the streaming of array elements in a buffer-based system. It determines whether a new batch of elements needs to be fetched from the input array based on the current
	   streaming state. The function updates internal indices to track which elements are cached and accessible. It also handles prefetching of both indices and array elements to improve performance.
	   When quad-buffering is used, it ensures that elements from the second-to-last batch remain accessible. The function returns the index that marks the end of the current batch, allowing the
	   caller to know which elements are available for access.

		\return The index of the first element after the newly fetched batch, or inIndexNumRoundedUp if all elements have been streamed.
		\throws This function may throw an assertion failure if indices are out of bounds.
	*/
	int FetchNextBatch()
	{
		// If not everything has been streamed already.
		if( cachedArrayEnd < inIndexNum ) {
			if( streamIndexEnd > 0 ) {
				cachedArrayEnd	 = streamArrayEnd;
				cachedArrayStart = Max( cachedArrayEnd - _bufferSize_ * ( _sbt_ - 1 ), 0 );
				cachedIndexEnd	 = streamIndexEnd;
				cachedIndexStart = Max( cachedIndexEnd - _bufferSize_ * ( _sbt_ - 1 ), 0 );

				// Flush the last batch of indices that are no longer accessible.
				FlushArray( inIndex, ( cachedIndexStart - _bufferSize_ ) * sizeof( _indexType_ ), cachedIndexStart * sizeof( _indexType_ ) );
				// Flush the last batch of elements that is no longer accessible.
				FlushArray( inArray, ( cachedArrayStart - _bufferSize_ ) * sizeof( _elemType_ ), cachedArrayStart * sizeof( _elemType_ ) );

				// Prefetch the next batch of elements.
				if( streamArrayEnd < inIndexNum ) {
					streamArrayEnd = cachedIndexEnd;
					for( int i = cachedArrayEnd; i < streamArrayEnd; i++ ) {
						assert( i >= cachedIndexStart && i < cachedIndexEnd );
						assert( inIndex[i] >= 0 && inIndex[i] < inArrayNum );

						Prefetch( inArray, inIndex[i] * sizeof( _elemType_ ) );
					}
				}
			}

			// Prefetch the next batch of indices.
			if( streamIndexEnd < inIndexNum ) {
				streamIndexEnd = Min( streamIndexEnd + _bufferSize_, inIndexNum );
				for( unsigned int offset = cachedIndexEnd * sizeof( _indexType_ ); offset < streamIndexEnd * sizeof( _indexType_ ); offset += CACHE_LINE_SIZE ) {
					Prefetch( inIndex, offset );
				}
			}
		}
		return ( cachedArrayEnd == inIndexNum ) ? inIndexNumRoundedUp : cachedArrayEnd;
	}

	// Provides access to the elements starting at the index returned by the next-to-last call
	// to FetchNextBach() (or zero if only called once so far) up to (excluding) the index
	// returned by the last call to FetchNextBatch(). When quad-buffering, the elements starting
	// at the index returned by the second-from-last call to FetchNextBatch() can still be accessed.
	// This is useful when the algorithm needs to successively access an odd number of elements
	// at the same time that may cross a single buffer boundary.
	const _elemType_& operator[]( int index ) const
	{
		assert( ( index >= cachedArrayStart && index < cachedArrayEnd ) || ( cachedArrayEnd == inIndexNum && index >= inIndexNum && index < inIndexNumRoundedUp ) );
		if( _roundUpToMultiple_ > 1 ) { index &= ( index - inIndexNum ) >> 31; }
		return inArray[inIndex[index]];
	}

private:
	int				   cachedArrayStart;
	int				   cachedArrayEnd;
	int				   streamArrayEnd;
	int				   cachedIndexStart;
	int				   cachedIndexEnd;
	int				   streamIndexEnd;
	const _elemType_*  inArray;
	int				   inArrayNum;
	const _indexType_* inIndex;
	int				   inIndexNum;
	int				   inIndexNumRoundedUp;

	/*!
		\brief Flushes cache lines for a specified range of an array

		This function flushes cache lines for a given range of an array, ensuring that any modified data is written back to memory. The function calculates the appropriate cache line boundaries based
	   on the input parameters and flushes the relevant cache lines. The implementation uses cache line alignment to ensure proper cache behavior. The function is currently disabled by a #if 0
	   preprocessor directive but contains the logic for cache line flushing.

		\param flushArray Pointer to the array whose cache lines need to be flushed
		\param flushStart Starting index for the flush operation
		\param flushEnd Ending index for the flush operation
	*/
	static void		   FlushArray( const void* flushArray, int flushStart, int flushEnd )
	{
#if 0
		// arrayFlushBase is rounded up so we do not flush anything before the array.
		// arrayFlushStart is rounded down so we start right after the last cache line that was previously flushed.
		// arrayFlushEnd is rounded down so we do not flush a cache line that holds data that may still be partially
		// accessible or a cache line that stretches beyond the end of the array.
		const uintptr_t arrayAddress = ( uintptr_t )flushArray;
		const uintptr_t arrayFlushBase = ( arrayAddress + CACHE_LINE_SIZE - 1 ) & ~( CACHE_LINE_SIZE - 1 );
		const uintptr_t arrayFlushStart = ( arrayAddress + flushStart ) & ~( CACHE_LINE_SIZE - 1 );
		const uintptr_t arrayFlushEnd = ( arrayAddress + flushEnd ) & ~( CACHE_LINE_SIZE - 1 );
		for( uintptr_t offset = Max( arrayFlushBase, arrayFlushStart ); offset < arrayFlushEnd; offset += CACHE_LINE_SIZE )
		{
			FlushCacheLine( flushArray, offset - arrayAddress );
		}
#endif
	}
};

#endif // !__SOFTWARECACHE_H__
