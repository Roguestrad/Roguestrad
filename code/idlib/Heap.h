/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2026 Robert Beckebans

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

#ifndef __HEAP_H__
#define __HEAP_H__

/*
===============================================================================

	Memory Management

===============================================================================
*/

// memory tag names are used to sort allocations for sys_dumpMemory and other reporting functions
enum memTag_t {
#define MEM_TAG( x ) TAG_##x,
#include "sys/sys_alloc_tags.h"
	TAG_NUM_TAGS,
};

static const int MAX_TAGS = 256;

/*!
	\brief Allocates aligned memory with 16-byte alignment for the specified size and tag

	This function allocates memory with 16-byte alignment, which is required for certain operations that benefit from or require aligned memory access. The allocation is performed using
   platform-specific functions; on Windows it uses _aligned_malloc, while on POSIX systems it uses posix_memalign. The size is padded to the next 16-byte boundary to ensure proper alignment. If the
   requested size is zero, the function returns NULL. The memory allocated should be freed using Mem_Free16.

	\param size The size in bytes of the memory block to allocate
	\param tag A tag used for memory tracking and debugging purposes
	\return A pointer to the allocated memory block, properly aligned to a 16-byte boundary, or NULL if the allocation fails
*/
void*			 Mem_Alloc16( const size_t size, const memTag_t tag );

/*!
	\brief Frees a block of memory that was allocated with Mem_Alloc16.

	This function frees memory that was previously allocated using the Mem_Alloc16 function. It handles platform-specific memory deallocation, using _aligned_free on Windows and free on other
   platforms. The function safely handles null pointers by returning immediately if the input pointer is null.

	\param ptr Pointer to the memory block to be freed
*/
void			 Mem_Free16( void* ptr );

//! Allocates memory of the specified size with the given tag.
ID_INLINE void*	 Mem_Alloc( const size_t size, const memTag_t tag )
{
	return Mem_Alloc16( size, tag );
}

/*!
	\brief Frees memory that was previously allocated by the memory manager.

	This function is an inline wrapper that delegates to Mem_Free16 to release memory. It is typically used to free memory allocated by functions like Sys_GetClipboardData which expects the caller to
   free the returned memory. The function does not validate if the pointer is null before freeing, so callers should ensure the pointer is valid.

	\param ptr Pointer to the memory block to be freed. Can be null, but should be validated by callers.
*/
ID_INLINE void Mem_Free( void* ptr )
{
	Mem_Free16( ptr );
}

/*!
	\brief Allocates memory and initializes it to zero

	This function allocates a block of memory of the specified size and initializes all bytes in the block to zero. It first calls Mem_Alloc to obtain the memory and then uses SIMDProcessor's Memset
   function to clear the memory. The allocated memory is suitable for use with collision model data structures where zero-initialization is required.

	\param size The size in bytes of the memory block to allocate
	\param tag Memory allocation tag for tracking and debugging purposes
	\return A pointer to the newly allocated and zero-initialized memory block
*/
void* Mem_ClearedAlloc( const size_t size, const memTag_t tag );

/*!
	\brief Allocates memory and copies a string into it

	This function allocates memory for a new string that is a copy of the input string. It uses the Mem_Alloc function to allocate the memory with the TAG_STRING tag, and then copies the input string
   into the newly allocated memory using strcpy. The caller is responsible for freeing the returned memory.

	\param in The input string to be copied
	\return A pointer to the newly allocated and copied string
*/
char* Mem_CopyString( const char* in );

/*!
	\brief Allocates memory with a specified alignment, using platform-specific alignment functions.

	This function allocates a block of memory with the specified size and alignment. For alignments less than or equal to 16 bytes, it delegates to the standard Mem_Alloc function. For larger
   alignments, it uses platform-specific alignment functions: _aligned_malloc on Windows and posix_memalign on other platforms. The allocated memory is tagged with the provided memory tag for tracking
   and debugging purposes.

	\param size The size of the memory block to allocate
	\param alignment The alignment requirement for the memory block
	\param tag The memory tag to associate with the allocation
	\return A pointer to the newly allocated memory block, or nullptr if allocation fails
*/
void* Mem_AllocAligned( const size_t size, const size_t alignment, const memTag_t tag );

/*!
	\brief Frees memory that was allocated with Mem_AllocAligned

	This function frees memory that was previously allocated using Mem_AllocAligned. It handles different alignment requirements by checking the alignment value. If the alignment is 16 bytes or less,
   it uses the standard Mem_Free function. For larger alignments, it uses platform-specific deallocation functions: _aligned_free on Windows or free on other platforms. The function is marked as
   noexcept, indicating it will not throw any exceptions.

	\param ptr Pointer to the memory block to be freed
	\param alignment The alignment value used during allocation
*/
void  Mem_FreeAligned( void* ptr, const size_t alignment ) noexcept;

// ---- unsized new/delete ----
void* operator new( size_t s );
void* operator new[]( size_t s );
void  operator delete( void* p ) noexcept;
void  operator delete[]( void* p ) noexcept;

// ---- sized delete (C++14) ----
void  operator delete( void* p, size_t ) noexcept;
void  operator delete[]( void* p, size_t ) noexcept;

// ---- aligned new/delete (C++17) ----
void* operator new( size_t s, std::align_val_t al );
void* operator new[]( size_t s, std::align_val_t al );
void  operator delete( void* p, std::align_val_t al ) noexcept;
void  operator delete[]( void* p, std::align_val_t al ) noexcept;

// ---- sized + aligned delete (C++17) ----
void  operator delete( void* p, size_t, std::align_val_t al ) noexcept;
void  operator delete[]( void* p, size_t, std::align_val_t al ) noexcept;

// ---- tagged new/delete ----
void* operator new( size_t s, memTag_t tag );
void* operator new[]( size_t s, memTag_t tag );

// This overload is only used if a constructor throws after `new(tag)`.
void  operator delete( void* p, memTag_t ) noexcept;
void  operator delete[]( void* p, memTag_t ) noexcept;

/*
================================================
idTempArray is an array that is automatically free'd when it goes out of scope.
There is no "cast" operator because these are very unsafe.

The template parameter MUST BE POD!

Compile time asserting POD-ness of the template parameter is complicated due
to our vector classes that need a default constructor but are otherwise
considered POD.
================================================
*/
template<class T>
class idTempArray
{
public:
	/*!
		\brief Constructs a new idTempArray by moving resources from another idTempArray.

		This constructor performs a move operation, transferring the buffer and element count from the source idTempArray to the newly constructed one. After the move, the source idTempArray is left
	   in a valid but empty state, with its num set to 0 and buffer set to NULL.

		\param other The source idTempArray whose resources will be moved to this new instance
	*/
	idTempArray( idTempArray<T>& other );

	/*!
		\brief Initializes the idTempArray with a specified number of elements and allocates memory for them.

		The constructor sets up the array with a given number of elements and allocates memory using the Mem_Alloc function. The allocated memory is initialized with the TAG_TEMP tag for memory
	   management purposes. The array is ready to be used after this initialization.

		\param num The number of elements to allocate space for in the array
	*/
	idTempArray( unsigned int num );

	/*!
		\brief Destroys the idTempArray and frees the allocated memory buffer.
	*/
	~idTempArray();

	T& operator[]( unsigned int i )
	{
		assert( i < num );
		return buffer[i];
	}
	const T& operator[]( unsigned int i ) const
	{
		assert( i < num );
		return buffer[i];
	}

	/*!
		\brief Returns a pointer to the internal buffer of the temporary array

		This function provides direct access to the internal memory buffer of the idTempArray container. It is typically used when low-level manipulation of the array's memory is required, such as
	   when interfacing with C-style APIs or when performing bulk operations on the array data. The returned pointer is valid as long as the idTempArray instance exists and is not modified in a way
	   that would cause reallocation.

		\return A pointer to the first element of the internal buffer
	*/
	T*			 Ptr() { return buffer; }

	/*!
		\brief Returns a pointer to the internal buffer of the temporary array.

		This method provides direct access to the underlying data buffer of the idTempArray. It is typically used when interfacing with functions that expect a raw pointer to array data. The returned
	   pointer is valid as long as the idTempArray object itself remains alive and unchanged.

		\return A constant pointer to the first element of the internal data buffer.
	*/
	const T*	 Ptr() const { return buffer; }

	//! Returns the size in bytes of the array elements.
	size_t		 Size() const { return num * sizeof( T ); }

	//! Returns the number of elements in the temporary array.
	unsigned int Num() const { return num; }

	//! Initializes all elements of the array to zero.
	void		 Zero() { memset( Ptr(), 0, Size() ); }

private:
	T*			 buffer; // Ensure this buffer comes first, so this == &this->buffer
	unsigned int num;
};

template<class T>
ID_INLINE idTempArray<T>::idTempArray( idTempArray<T>& other )
{
	this->num	 = other.num;
	this->buffer = other.buffer;
	other.num	 = 0;
	other.buffer = NULL;
}

template<class T>
ID_INLINE idTempArray<T>::idTempArray( unsigned int num )
{
	this->num = num;
	buffer	  = ( T* )Mem_Alloc( num * sizeof( T ), TAG_TEMP );
}

template<class T>
ID_INLINE idTempArray<T>::~idTempArray()
{
	Mem_Free( buffer );
}

/*
===============================================================================

	Block based allocator for fixed size objects.

	All objects of the 'type' are properly constructed and destructed when reused.

===============================================================================
*/

#define BLOCK_ALLOC_ALIGNMENT 16

// Define this to force all block allocators to act like normal new/delete allocation
// for tool checking.
// #define	FORCE_DISCRETE_BLOCK_ALLOCS

/*
================================================
idBlockAlloc is a block-based allocator for fixed-size objects.

All objects are properly constructed and destructed.
================================================
*/
template<class _type_, int _blockSize_, memTag_t memTag = TAG_BLOCKALLOC>
class idBlockAlloc
{
public:
	/*!
		\brief Initializes a new instance of the idBlockAlloc template class with optional memory clearing.

		This constructor initializes the internal state of the block allocator. It sets up the blocks list, free list, and various tracking variables. The clear parameter determines whether allocated
	   memory should be cleared upon allocation.

		\param clear If true, allocated memory will be cleared (zeroed out) upon allocation.
	*/
	ID_INLINE idBlockAlloc( bool clear = false );

	/*!
		\brief Destructor for the idBlockAlloc class that cleans up allocated memory blocks.

		This inline destructor releases all memory blocks that were allocated by the idBlockAlloc instance. It calls the Shutdown method to ensure proper cleanup of internal data structures and memory
	   management.

	*/
	ID_INLINE ~idBlockAlloc();

	//! Returns the total size of allocated memory.
	size_t			  Allocated() const { return total * sizeof( _type_ ); }

	//! Returns the total size of allocated memory including the size of the object itself.
	size_t			  Size() const { return sizeof( *this ) + Allocated(); }

	/*!
		\brief Shuts down the block allocator by freeing all allocated memory blocks and resetting internal counters

		This function cleans up all memory blocks that have been allocated by the block allocator. It iterates through the linked list of blocks, freeing each one using Mem_Free, and then resets the
	   internal state of the allocator including the blocks pointer, free pointer, and counters for total and active allocations

	*/
	ID_INLINE void	  Shutdown();

	/*!
		\brief Initializes a fixed number of memory blocks for allocation

		This function ensures that a specified number of memory blocks are allocated and ready for use. It first counts the currently existing blocks, then allocates new blocks until the desired
	   number is reached. After this initialization, further allocations are disabled to enforce the fixed block count

		\param numBlocks The total number of memory blocks to initialize
	*/
	ID_INLINE void	  SetFixedBlocks( int numBlocks );

	/*!
		\brief Frees memory blocks that are completely empty

		This function iterates through all allocated memory blocks to count how many free elements are present in each block. It then identifies blocks where all elements are free and deallocates
	   those blocks entirely. After freeing the empty blocks, it rebuilds the global free list by iterating through the remaining blocks and linking their free elements together

		\return void
		\throws assertion failure if an element cannot be found in any block
	*/
	ID_INLINE void	  FreeEmptyBlocks();

	/*!
		\brief Allocates a new element from the block allocator, potentially creating a new block if necessary.

		This function retrieves an available element from the internal block allocator. If no elements are currently free, it attempts to allocate a new block. The allocated element is initialized
	   using placement new. If clearAllocs is enabled, the memory is zeroed before initialization. The function returns a pointer to the newly allocated element.

		\return A pointer to the newly allocated element of type _type_
	*/
	ID_INLINE _type_* Alloc();

	/*!
		\brief Frees a previously allocated element from the block allocator

		This function deallocates memory that was previously allocated by the block allocator. It performs different actions depending on whether the FORCE_DISCRETE_BLOCK_ALLOCS macro is defined. When
	   the macro is not defined, it checks for null pointer, calls the destructor for the element, and then returns the element to the free list for reuse. The active count is decremented to reflect
	   the freed element

		\param element Pointer to the element to be freed
	*/
	ID_INLINE void	  Free( _type_* element );

	//! Returns the total count of allocated blocks.
	int				  GetTotalCount() const { return total; }

	//! Returns the current count of allocated items from the block allocator.
	int				  GetAllocCount() const { return active; }

	//! Returns the number of free elements in the block allocator.
	int				  GetFreeCount() const { return total - active; }

private:
	union element_t {
		_type_*	   data; // this is a hack to make sure the save game system marks _type_ as saveable
		element_t* next;
		byte	   buffer[( CONST_MAX( sizeof( _type_ ), sizeof( element_t* ) ) + ( BLOCK_ALLOC_ALIGNMENT - 1 ) ) & ~( BLOCK_ALLOC_ALIGNMENT - 1 )];
	};

	class idBlock
	{
	public:
		element_t  elements[_blockSize_];
		idBlock*   next;
		element_t* free;	  // list with free elements in this block (temp used only by FreeEmptyBlocks)
		int		   freeCount; // number of free elements in this block (temp used only by FreeEmptyBlocks)
	};

	idBlock*	   blocks;
	element_t*	   free;
	int			   total;
	int			   active;
	bool		   allowAllocs;
	bool		   clearAllocs;

	/*!
		\brief Allocates a new memory block for the block allocator and initializes its elements.

		This function creates a new block of memory with the specified block size and initializes all elements within the block to be available for allocation. It links the new block to the list of
	   existing blocks and sets up the free list pointer to point to the first element of the newly allocated block.

		\throws assertion failure if the alignment of the free pointer does not match the required block allocation alignment
	*/
	ID_INLINE void AllocNewBlock();
};

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE idBlockAlloc<_type_, _blockSize_, memTag>::idBlockAlloc( bool clear ) :
	blocks( NULL ),
	free( NULL ),
	total( 0 ),
	active( 0 ),
	allowAllocs( true ),
	clearAllocs( clear )
{
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE idBlockAlloc<_type_, _blockSize_, memTag>::~idBlockAlloc()
{
	Shutdown();
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE _type_* idBlockAlloc<_type_, _blockSize_, memTag>::Alloc()
{
#ifdef FORCE_DISCRETE_BLOCK_ALLOCS
	// for debugging tools
	return new _type_;
#else
	if( free == NULL ) {
		if( !allowAllocs ) { return NULL; }
		AllocNewBlock();
	}

	active++;
	element_t* element = free;
	free			   = free->next;
	element->next	   = NULL;

	_type_* t = ( _type_* )element->buffer;
	if( clearAllocs ) {
		memset( ( void* )t, 0, sizeof( _type_ ) ); // SRS - Added (void*) cast to silence build-time warning
	}
	new( t ) _type_;
	return t;
#endif
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE void idBlockAlloc<_type_, _blockSize_, memTag>::Free( _type_* t )
{
#ifdef FORCE_DISCRETE_BLOCK_ALLOCS
	// for debugging tools
	delete t;
#else
	if( t == NULL ) { return; }

	t->~_type_();

	element_t* element = ( element_t* )( t );
	element->next	   = free;
	free			   = element;
	active--;
#endif
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE void idBlockAlloc<_type_, _blockSize_, memTag>::Shutdown()
{
	while( blocks != NULL ) {
		idBlock* block = blocks;
		blocks		   = blocks->next;
		Mem_Free( block );
	}
	blocks = NULL;
	free   = NULL;
	total = active = 0;
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE void idBlockAlloc<_type_, _blockSize_, memTag>::SetFixedBlocks( int numBlocks )
{
	int currentNumBlocks = 0;
	for( idBlock* block = blocks; block != NULL; block = block->next ) {
		currentNumBlocks++;
	}
	for( int i = currentNumBlocks; i < numBlocks; i++ ) {
		AllocNewBlock();
	}
	allowAllocs = false;
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE void idBlockAlloc<_type_, _blockSize_, memTag>::AllocNewBlock()
{
	idBlock* block = ( idBlock* )Mem_Alloc( sizeof( idBlock ), memTag );
	block->next	   = blocks;
	blocks		   = block;
	for( int i = 0; i < _blockSize_; i++ ) {
		block->elements[i].next = free;
		free					= &block->elements[i];

		// RB: changed UINT_PTR to uintptr_t
		assert( ( ( ( uintptr_t )free ) & ( BLOCK_ALLOC_ALIGNMENT - 1 ) ) == 0 );
		// RB end
	}
	total += _blockSize_;
}

template<class _type_, int _blockSize_, memTag_t memTag>
ID_INLINE void idBlockAlloc<_type_, _blockSize_, memTag>::FreeEmptyBlocks()
{
	// first count how many free elements are in each block
	// and build up a free chain per block
	for( idBlock* block = blocks; block != NULL; block = block->next ) {
		block->free		 = NULL;
		block->freeCount = 0;
	}
	for( element_t* element = free; element != NULL; ) {
		element_t* next = element->next;
		for( idBlock* block = blocks; block != NULL; block = block->next ) {
			if( element >= block->elements && element < block->elements + _blockSize_ ) {
				element->next = block->free;
				block->free	  = element;
				block->freeCount++;
				break;
			}
		}
		// if this assert fires, we couldn't find the element in any block
		assert( element->next != next );
		element = next;
	}
	// now free all blocks whose free count == _blockSize_
	idBlock* prevBlock = NULL;
	for( idBlock* block = blocks; block != NULL; ) {
		idBlock* next = block->next;
		if( block->freeCount == _blockSize_ ) {
			if( prevBlock == NULL ) {
				assert( blocks == block );
				blocks = block->next;
			} else {
				assert( prevBlock->next == block );
				prevBlock->next = block->next;
			}
			Mem_Free( block );
			total -= _blockSize_;
		} else {
			prevBlock = block;
		}
		block = next;
	}
	// now rebuild the free chain
	free = NULL;
	for( idBlock* block = blocks; block != NULL; block = block->next ) {
		for( element_t* element = block->free; element != NULL; ) {
			element_t* next = element->next;
			element->next	= free;
			free			= element;
			element			= next;
		}
	}
}

/*
==============================================================================

	Dynamic allocator, simple wrapper for normal allocations which can
	be interchanged with idDynamicBlockAlloc.

	No constructor is called for the 'type'.
	Allocated blocks are always 16 byte aligned.

==============================================================================
*/

template<class type, int baseBlockSize, int minBlockSize>
class idDynamicAlloc
{
public:
	/*!
		\brief Initializes a new instance of the idDynamicAlloc class.

		The constructor initializes the idDynamicAlloc object by calling the Clear method to ensure all internal state is properly set up. This ensures that the allocator starts with a clean state,
	   ready for use.

	*/
	idDynamicAlloc();

	/*!
		\brief Destroys the dynamic allocator and cleans up its resources.

		This destructor finalizes the dynamic allocator by calling the Shutdown method, which ensures that all allocated memory blocks are properly released and internal structures are cleaned up.

	*/
	~idDynamicAlloc();

	/*!
		\brief Initializes the dynamic allocation system

		This function initializes the dynamic allocation system for the given type with specified block sizes. It sets up internal data structures needed for dynamic memory management.

	*/
	void		Init();

	/*!
		\brief Clears all allocated memory blocks in the dynamic allocator

		This function terminates the dynamic memory allocation system by clearing all previously allocated memory blocks. It is typically called during system shutdown to ensure proper cleanup of
	   dynamically allocated resources. The function delegates to the Clear method which handles the actual deallocation of memory blocks.

	*/
	void		Shutdown();

	/*!
		\brief Sets the number of fixed blocks for the dynamic allocator

		This function is used to configure the dynamic allocator by setting the number of fixed blocks it should use. The fixed blocks are pre-allocated memory blocks that are used for efficient
	   memory management. The allocator uses these blocks to avoid frequent memory allocation and deallocation overhead.

		\param numBlocks The number of fixed blocks to configure for the allocator
	*/
	void		SetFixedBlocks( int numBlocks ) { }

	/*!
		\brief Sets whether memory should be locked.

		This function configures the memory allocation behavior to lock or unlock memory pages. When locking is enabled, the allocated memory pages will be locked in physical memory and won't be
	   swapped out. This can be useful for performance-critical applications where memory swapping should be avoided.

		\param lock True to lock memory, false to unlock
	*/
	void		SetLockMemory( bool lock ) { }

	/*!
		\brief Frees empty base blocks in the dynamic allocation system.

		This function is responsible for cleaning up base blocks that are currently empty within the dynamic allocation system. It is typically called during memory management operations to ensure
	   that unused memory blocks are properly deallocated and returned to the system.

	*/
	void		FreeEmptyBaseBlocks() { }

	/*!
		\brief Allocates a block of memory for the specified number of objects of type T

		This function is used to dynamically allocate memory for a specified number of objects of type T. It increments the allocation counter and tracks the memory usage. The allocated memory is
	   aligned to 16-byte boundaries. The function returns a pointer to the allocated memory block or NULL if the requested number of elements is zero or less.

		\param num The number of objects to allocate memory for
		\return A pointer to the allocated memory block, or NULL if the allocation fails or num is zero or less
	*/
	type*		Alloc( const int num );

	/*!
		\brief Resizes a dynamically allocated memory block to the specified number of elements

		This function attempts to resize an existing memory block allocated by the dynamic allocator. If the pointer is NULL, it allocates a new block of the specified size. If the new size is zero or
	   negative, it frees the existing block and returns NULL. The function currently contains an assertion that will always fail, indicating that the resize functionality is not yet implemented or is
	   intentionally disabled.

		\param ptr Pointer to the memory block to resize, or NULL to allocate a new block
		\param num The new number of elements to allocate, must be positive
		\return Pointer to the resized memory block, or NULL if the operation fails or the new size is zero or negative
	*/
	type*		Resize( type* ptr, const int num );

	/*!
		\brief Frees a previously allocated memory block

		This function decrements the free counter and calls the memory manager to free the specified memory block. It handles the case where a NULL pointer is passed by simply returning without
	   performing any action. The function is part of a dynamic memory allocation system that manages memory blocks of specific sizes.

		\param ptr Pointer to the memory block to be freed
	*/
	void		Free( type* ptr );

	/*!
		\brief Checks the memory for the given pointer and returns an error message if corruption is detected.

		This function performs a memory integrity check on the specified pointer. It verifies that the memory region pointed to by ptr is valid and has not been corrupted. The function is typically
	   used for debugging and validation purposes. It returns NULL if no issues are found, otherwise it returns a string describing the memory error.

		\param ptr Pointer to the memory block to be checked for corruption
		\return Null pointer if memory is valid, otherwise returns a string describing the memory error
		\throws NULL (no exceptions thrown)
	*/
	const char* CheckMemory( const type* ptr ) const;

	//! Returns the number of base blocks allocated.
	int			GetNumBaseBlocks() const { return 0; }

	//! Returns the base block memory size for the dynamic allocator.
	int			GetBaseBlockMemory() const { return 0; }

	//! Returns the number of used blocks in the dynamic allocator.
	int			GetNumUsedBlocks() const { return numUsedBlocks; }

	//! Returns the total amount of memory currently used by allocated blocks.
	int			GetUsedBlockMemory() const { return usedBlockMemory; }

	//! Returns the number of free blocks available for allocation.
	int			GetNumFreeBlocks() const { return 0; }

	//! Returns the amount of free memory available in the dynamic allocator.
	int			GetFreeBlockMemory() const { return 0; }

	//! Returns the number of empty base blocks in the dynamic allocator.
	int			GetNumEmptyBaseBlocks() const { return 0; }

private:
	int	 numUsedBlocks;	  // number of used blocks
	int	 usedBlockMemory; // total memory in used blocks

	int	 numAllocs;
	int	 numResizes;
	int	 numFrees;

	//! Clears all tracking statistics and resets the dynamic allocator state
	void Clear();
};

template<class type, int baseBlockSize, int minBlockSize>
idDynamicAlloc<type, baseBlockSize, minBlockSize>::idDynamicAlloc()
{
	Clear();
}

template<class type, int baseBlockSize, int minBlockSize>
idDynamicAlloc<type, baseBlockSize, minBlockSize>::~idDynamicAlloc()
{
	Shutdown();
}

template<class type, int baseBlockSize, int minBlockSize>
void idDynamicAlloc<type, baseBlockSize, minBlockSize>::Init()
{
}

template<class type, int baseBlockSize, int minBlockSize>
void idDynamicAlloc<type, baseBlockSize, minBlockSize>::Shutdown()
{
	Clear();
}

template<class type, int baseBlockSize, int minBlockSize>
type* idDynamicAlloc<type, baseBlockSize, minBlockSize>::Alloc( const int num )
{
	numAllocs++;
	if( num <= 0 ) { return NULL; }
	numUsedBlocks++;
	usedBlockMemory += num * sizeof( type );
	return Mem_Alloc16( num * sizeof( type ), TAG_BLOCKALLOC );
}

template<class type, int baseBlockSize, int minBlockSize>
type* idDynamicAlloc<type, baseBlockSize, minBlockSize>::Resize( type* ptr, const int num )
{
	numResizes++;

	if( ptr == NULL ) { return Alloc( num ); }

	if( num <= 0 ) {
		Free( ptr );
		return NULL;
	}

	assert( 0 );
	return ptr;
}

template<class type, int baseBlockSize, int minBlockSize>
void idDynamicAlloc<type, baseBlockSize, minBlockSize>::Free( type* ptr )
{
	numFrees++;
	if( ptr == NULL ) { return; }
	Mem_Free16( ptr );
}

template<class type, int baseBlockSize, int minBlockSize>
const char* idDynamicAlloc<type, baseBlockSize, minBlockSize>::CheckMemory( const type* ptr ) const
{
	return NULL;
}

template<class type, int baseBlockSize, int minBlockSize>
void idDynamicAlloc<type, baseBlockSize, minBlockSize>::Clear()
{
	numUsedBlocks	= 0;
	usedBlockMemory = 0;
	numAllocs		= 0;
	numResizes		= 0;
	numFrees		= 0;
}

/*
==============================================================================

	Fast dynamic block allocator.

	No constructor is called for the 'type'.
	Allocated blocks are always 16 byte aligned.

==============================================================================
*/

#include "containers/BTree.h"

// #define DYNAMIC_BLOCK_ALLOC_CHECK

/*!
	\brief Sets the size of the dynamic block, with optional base block handling

	This function updates the size of a dynamic block. If isBaseBlock is true, the size is stored as a negative value, otherwise it's stored as a positive value. This is typically used in
serialization contexts where the size needs to be tracked differently based on whether the block represents a base structure.

	\param s The size to set for the dynamic block

//! Returns a pointer to the memory block following the idDynamicBlock header.
	\param isBaseBlock Flag indicating if this is a base block, which affects how the size is stored

//! Returns the absolute size of the dynamic block.
*/
template<class type>
class idDynamicBlock
{
public:
	//! Returns a pointer to the memory block following the idDynamicBlock header.
	type* GetMemory() const { return ( type* )( ( ( byte* )this ) + sizeof( idDynamicBlock<type> ) ); }

	//! Returns the absolute size of the dynamic block.
	int	  GetSize() const { return abs( size ); }

	/*!
		\brief Sets the size of the dynamic block, with an option to mark it as a base block

		This function configures the size of a dynamic block, which is typically used in serialization operations. When the isBaseBlock flag is true, the size is stored as a negative value, indicating
	   that this block is considered a base block. Otherwise, the size is stored as a positive value. This function is commonly used during deserialization to properly initialize the dimensions of
	   dynamic data structures such as matrices and vectors.

		\param s The size to set for the dynamic block
		\param isBaseBlock Flag indicating whether this block is a base block, which affects how the size is stored
	*/
	void  SetSize( int s, bool isBaseBlock ) { size = isBaseBlock ? -s : s; }

	//! Returns true if this is a base block.
	bool  IsBaseBlock() const { return ( size < 0 ); }

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	int	  id[3];
	void* allocator;
#endif

	int										size; // size in bytes of the block
	idDynamicBlock<type>*					prev; // previous memory block
	idDynamicBlock<type>*					next; // next memory block
	idBTreeNode<idDynamicBlock<type>, int>* node; // node in the B-Tree with free blocks
};

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_ = TAG_BLOCKALLOC>
class idDynamicBlockAlloc
{
public:
	/*!
		\brief Initializes a new instance of the idDynamicBlockAlloc class with the specified memory allocation tag and clears internal state.

		The constructor initializes the memory allocation tag using the template parameter _tag_ and then calls the Clear method to reset the internal state of the allocator. This ensures that the
	   allocator starts with a clean state and is ready to manage dynamic memory blocks according to the specified template parameters.

	*/
	idDynamicBlockAlloc();

	/*!
		\brief Destructor for the idDynamicBlockAlloc class that shuts down the dynamic memory allocation system.

		This destructor cleans up the dynamic block allocation system by calling the Shutdown method. It is responsible for releasing any resources held by the idDynamicBlockAlloc instance, ensuring
	   proper cleanup of the free memory block tree structure that was initialized during construction.

	*/
	~idDynamicBlockAlloc();

	/*!
		\brief Initializes the free block tree for dynamic memory allocation

		This function initializes the freeTree member of the idDynamicBlockAlloc class, which is used to manage free memory blocks for dynamic allocation. The initialization sets up the tree structure
	   that will be used to track available memory blocks of various sizes.

	*/
	void		Init();

	/*!
		\brief Shuts down the dynamic block allocator by freeing all allocated memory blocks and clearing internal structures

		This function cleans up all memory blocks managed by the dynamic block allocator. It first frees any internal blocks that are not part of the base block chain, then iterates through all blocks
	   in the chain to free them. During this process, it ensures proper memory locking/unlocking if enabled. Finally, it shuts down the free tree and clears the allocator's state. This function is
	   typically called during system shutdown to properly release all allocated memory.

	*/
	void		Shutdown();

	/*!
		\brief Initializes a specified number of fixed blocks for memory allocation

		This function sets up a fixed number of memory blocks that can be used for allocation. It allocates memory for each block using Mem_Alloc16 and initializes the block structure. The blocks are
	   linked together in a doubly-linked list and added to the free list. The function also disables further allocations once the fixed blocks are set up

		\param numBlocks The total number of blocks to initialize
	*/
	void		SetFixedBlocks( int numBlocks );

	/*!
		\brief Sets whether memory should be locked.
		\param lock true to lock memory, false to unlock
	*/
	void		SetLockMemory( bool lock );

	/*!
		\brief Frees empty base blocks from the dynamic block allocator.

		This function iterates through all blocks in the allocator and frees those that are base blocks and have no pending allocations. It removes the block from the linked list of blocks and adjusts
	   the pointers of adjacent blocks. Memory locking is released if enabled, and statistics are updated to reflect the freed memory.

	*/
	void		FreeEmptyBaseBlocks();

	/*!
		\brief Allocates a block of memory for a specified number of elements of type T

		This function is used to dynamically allocate memory for objects of type T. It handles the allocation and resizing of memory blocks internally. The function increments the allocation counter
	   and tracks memory usage. It returns a pointer to the allocated memory block or NULL if allocation fails. The function also performs memory checking if the DYNAMIC_BLOCK_ALLOC_CHECK flag is
	   enabled.

		\param num Number of elements to allocate memory for
		\return Pointer to the allocated memory block, or NULL if allocation fails
	*/
	type*		Alloc( const int num );

	/*!
		\brief Resizes a previously allocated memory block to a new size

		This function adjusts the size of a memory block that was previously allocated using the Alloc method. If the pointer is null, it allocates a new block of the specified size. If the new size
	   is less than or equal to zero, it frees the existing block and returns null. The function updates internal memory tracking statistics and performs the actual resize operation through internal
	   methods. Memory checking can be performed if the DYNAMIC_BLOCK_ALLOC_CHECK flag is defined.

		\param ptr Pointer to the memory block to be resized
		\param num New size for the memory block
		\return Pointer to the resized memory block, or null if the resize operation fails
	*/
	type*		Resize( type* ptr, const int num );

	/*!
		\brief Frees a previously allocated memory block from the dynamic block allocator

		This function releases a memory block that was previously allocated using the corresponding Alloc function. It updates internal statistics tracking the number of frees and used blocks, and
	   performs the actual deallocation through FreeInternal. The function includes optional memory checking when the DYNAMIC_BLOCK_ALLOC_CHECK flag is defined. The function handles null pointer
	   inputs gracefully by returning immediately without performing any operations.

		\param ptr Pointer to the memory block to be freed
	*/
	void		Free( type* ptr );

	/*!
		\brief Checks the memory at the specified pointer for integrity and returns an error message if corruption is detected

		This function verifies the integrity of a memory block allocated by the dynamic block allocator. It checks if the pointer is valid, if the block has not been freed, and if the memory contains
	   valid allocation identifiers. The function performs various integrity checks including verifying the block's identifier values and ensuring the allocator reference matches the current instance.
	   It returns NULL if the memory is valid, or a descriptive error message if corruption is detected.

		\param ptr Pointer to the memory block to check for integrity
		\return NULL if the memory is valid, or a descriptive error message if corruption is detected
		\throws This function does not explicitly throw exceptions but may assert if memory integrity checks fail when DYNAMIC_BLOCK_ALLOC_CHECK is defined
	*/
	const char* CheckMemory( const type* ptr ) const;

	//! Returns the number of base blocks allocated by this dynamic block allocator.
	int			GetNumBaseBlocks() const { return numBaseBlocks; }

	//! Returns the memory size of the base blocks used by the dynamic block allocator.
	int			GetBaseBlockMemory() const { return baseBlockMemory; }

	//! Returns the number of used blocks in the dynamic block allocator.
	int			GetNumUsedBlocks() const { return numUsedBlocks; }

	//! Returns the total amount of memory currently used by allocated blocks.
	int			GetUsedBlockMemory() const { return usedBlockMemory; }

	//! Returns the number of free blocks available in the dynamic block allocator.
	int			GetNumFreeBlocks() const { return numFreeBlocks; }

	//! Returns the amount of free memory available in the dynamic block allocator.
	int			GetFreeBlockMemory() const { return freeBlockMemory; }

	//! Returns the number of empty base blocks in the dynamic block allocator.
	int			GetNumEmptyBaseBlocks() const;

private:
	idDynamicBlock<type>*				  firstBlock;  // first block in list in order of increasing address
	idDynamicBlock<type>*				  lastBlock;   // last block in list in order of increasing address
	idBTree<idDynamicBlock<type>, int, 4> freeTree;	   // B-Tree with free memory blocks
	bool								  allowAllocs; // allow base block allocations
	bool								  lockMemory;  // lock memory so it cannot get swapped out

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	int blockId[3];
#endif

	int					  numBaseBlocks;   // number of base blocks
	int					  baseBlockMemory; // total memory in base blocks
	int					  numUsedBlocks;   // number of used blocks
	int					  usedBlockMemory; // total memory in used blocks
	int					  numFreeBlocks;   // number of free blocks
	int					  freeBlockMemory; // total memory in free blocks

	int					  numAllocs;
	int					  numResizes;
	int					  numFrees;

	memTag_t			  tag;

	//! Resets all internal state and statistics of the dynamic block allocator to their initial values.
	void				  Clear();

	/*!
		\brief Allocates a dynamic memory block of specified size from either the free list or by allocating new memory.

		This function attempts to find a suitable free block that can accommodate the requested number of elements of type 'type'. If no such block is available and allocation is allowed, it allocates
	   a new block with sufficient memory. The block is linked into the list of allocated blocks and the internal statistics are updated.

		\param num The number of elements of type 'type' to allocate memory for
		\return A pointer to the allocated dynamic block, or NULL if allocation fails and allowAllocs is false
	*/
	idDynamicBlock<type>* AllocInternal( const int num );

	/*!
		\brief Resizes a dynamic memory block to accommodate the specified number of elements, potentially merging with adjacent free blocks or allocating a new block if necessary.

		This function adjusts the size of a given dynamic block to fit the requested number of elements. If the current block is too small, it attempts to merge with the next free block if possible.
	   If merging is not feasible, it allocates a new block, copies the data, and frees the old block. After resizing, if there's sufficient unused space at the end of the block to form a new block
	   with at least one element, that space is carved out and freed. The function ensures proper linking and sizing of blocks within the allocator's free list structure.

		\param block Pointer to the dynamic block to be resized
		\param num The desired number of elements the block should accommodate
		\return Pointer to the resized block, or NULL if allocation fails
	*/
	idDynamicBlock<type>* ResizeInternal( idDynamicBlock<type>* block, const int num );

	/*!
		\brief Frees a dynamic memory block and attempts to merge it with adjacent free blocks.

		This function releases a dynamic memory block back to the allocator. It first checks if the block can be merged with the next free block in the list, and if so, it combines their sizes and
	   updates the links. Then it checks if the block can be merged with the previous free block, and if so, it performs a similar merge operation. If no merges occur, the block is simply added back
	   to the free list. The function includes assertions to validate the block's state and integrity during the process.

		\param block Pointer to the dynamic block to be freed
		\throws assertion failure if block node is not null or if dynamic block allocation check fails
	*/
	void				  FreeInternal( idDynamicBlock<type>* block );

	/*!
		\brief Links a free block into the internal free tree structure

		This function adds a free block to the internal free tree structure for memory management. It associates the block with its size in the free tree and updates the statistics tracking the number
	   of free blocks and total free memory.

		\param block Pointer to the dynamic block to be linked into the free tree
	*/
	void				  LinkFreeInternal( idDynamicBlock<type>* block );

	/*!
		\brief Removes a free block from the internal free list and updates memory tracking counts.

		This function performs the necessary cleanup to remove a block from the free list structure. It removes the block's node from the free tree, resets the node pointer to NULL, and decrements the
	   free block count. Additionally, it adjusts the total free memory counter by subtracting the size of the block being removed.

		\param block Pointer to the dynamic block to be unlinked from the free list
	*/
	void				  UnlinkFreeInternal( idDynamicBlock<type>* block );

	/*!
		\brief Validates the internal linking structure of dynamic memory blocks to ensure consistency.

		This function performs consistency checks on the linked list structure of memory blocks managed by the dynamic block allocator. It verifies that each block is properly linked to its
	   predecessor and successor blocks, ensuring that the first and last block pointers are correctly maintained. The function uses assertions to detect any inconsistencies in the linking structure,
	   which could indicate memory corruption or allocator errors.

		\throws assertion failure if block linking is inconsistent
	*/
	void				  CheckMemory() const;
};

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::idDynamicBlockAlloc()
{
	tag = _tag_;
	Clear();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::~idDynamicBlockAlloc()
{
	Shutdown();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::Init()
{
	freeTree.Init();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::Shutdown()
{
	idDynamicBlock<type>* block;

	for( block = firstBlock; block != NULL; block = block->next ) {
		if( block->node == NULL ) { FreeInternal( block ); }
	}

	for( block = firstBlock; block != NULL; block = firstBlock ) {
		firstBlock = block->next;
		assert( block->IsBaseBlock() );
		if( lockMemory ) {
			// idLib::sys->UnlockMemory( block, block->GetSize() + (int)sizeof( idDynamicBlock<type> ) );
		}
		Mem_Free16( block );
	}

	freeTree.Shutdown();

	Clear();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::SetFixedBlocks( int numBlocks )
{
	idDynamicBlock<type>* block;

	for( int i = numBaseBlocks; i < numBlocks; i++ ) {
		block = ( idDynamicBlock<type>* )Mem_Alloc16( baseBlockSize, _tag_ );
		if( lockMemory ) {
			// idLib::sys->LockMemory( block, baseBlockSize );
		}
#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
		memcpy( block->id, blockId, sizeof( block->id ) );
		block->allocator = ( void* )this;
#endif
		block->SetSize( baseBlockSize - ( int )sizeof( idDynamicBlock<type> ), true );
		block->next = NULL;
		block->prev = lastBlock;
		if( lastBlock ) {
			lastBlock->next = block;
		} else {
			firstBlock = block;
		}
		lastBlock	= block;
		block->node = NULL;

		FreeInternal( block );

		numBaseBlocks++;
		baseBlockMemory += baseBlockSize;
	}

	allowAllocs = false;
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::SetLockMemory( bool lock )
{
	lockMemory = lock;
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::FreeEmptyBaseBlocks()
{
	idDynamicBlock<type>*block, *next;

	for( block = firstBlock; block != NULL; block = next ) {
		next = block->next;

		if( block->IsBaseBlock() && block->node != NULL && ( next == NULL || next->IsBaseBlock() ) ) {
			UnlinkFreeInternal( block );
			if( block->prev ) {
				block->prev->next = block->next;
			} else {
				firstBlock = block->next;
			}
			if( block->next ) {
				block->next->prev = block->prev;
			} else {
				lastBlock = block->prev;
			}
			if( lockMemory ) {
				// idLib::sys->UnlockMemory( block, block->GetSize() + (int)sizeof( idDynamicBlock<type> ) );
			}
			numBaseBlocks--;
			baseBlockMemory -= block->GetSize() + ( int )sizeof( idDynamicBlock<type> );
			Mem_Free16( block );
		}
	}

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	CheckMemory();
#endif
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
int idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::GetNumEmptyBaseBlocks() const
{
	int					  numEmptyBaseBlocks;
	idDynamicBlock<type>* block;

	numEmptyBaseBlocks = 0;
	for( block = firstBlock; block != NULL; block = block->next ) {
		if( block->IsBaseBlock() && block->node != NULL && ( block->next == NULL || block->next->IsBaseBlock() ) ) { numEmptyBaseBlocks++; }
	}
	return numEmptyBaseBlocks;
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
type* idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::Alloc( const int num )
{
	idDynamicBlock<type>* block;

	numAllocs++;

	if( num <= 0 ) { return NULL; }

	block = AllocInternal( num );
	if( block == NULL ) { return NULL; }
	block = ResizeInternal( block, num );
	if( block == NULL ) { return NULL; }

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	CheckMemory();
#endif

	numUsedBlocks++;
	usedBlockMemory += block->GetSize();

	return block->GetMemory();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
type* idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::Resize( type* ptr, const int num )
{
	numResizes++;

	if( ptr == NULL ) { return Alloc( num ); }

	if( num <= 0 ) {
		Free( ptr );
		return NULL;
	}

	idDynamicBlock<type>* block = ( idDynamicBlock<type>* )( ( ( byte* )ptr ) - ( int )sizeof( idDynamicBlock<type> ) );

	usedBlockMemory -= block->GetSize();

	block = ResizeInternal( block, num );
	if( block == NULL ) { return NULL; }

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	CheckMemory();
#endif

	usedBlockMemory += block->GetSize();

	return block->GetMemory();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::Free( type* ptr )
{
	numFrees++;

	if( ptr == NULL ) { return; }

	idDynamicBlock<type>* block = ( idDynamicBlock<type>* )( ( ( byte* )ptr ) - ( int )sizeof( idDynamicBlock<type> ) );

	numUsedBlocks--;
	usedBlockMemory -= block->GetSize();

	FreeInternal( block );

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	CheckMemory();
#endif
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
const char* idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::CheckMemory( const type* ptr ) const
{
	idDynamicBlock<type>* block;

	if( ptr == NULL ) { return NULL; }

	block = ( idDynamicBlock<type>* )( ( ( byte* )ptr ) - ( int )sizeof( idDynamicBlock<type> ) );

	if( block->node != NULL ) { return "memory has been freed"; }

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	if( block->id[0] != 0x11111111 || block->id[1] != 0x22222222 || block->id[2] != 0x33333333 ) { return "memory has invalid id"; }
	if( block->allocator != ( void* )this ) { return "memory was allocated with different allocator"; }
#endif

	/* base blocks can be larger than baseBlockSize which can cause this code to fail
	idDynamicBlock<type> *base;
	for ( base = firstBlock; base != NULL; base = base->next ) {
		if ( base->IsBaseBlock() ) {
			if ( ((int)block) >= ((int)base) && ((int)block) < ((int)base) + baseBlockSize ) {
				break;
			}
		}
	}
	if ( base == NULL ) {
		return "no base block found for memory";
	}
	*/

	return NULL;
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::Clear()
{
	firstBlock = lastBlock = NULL;
	allowAllocs			   = true;
	lockMemory			   = false;
	numBaseBlocks		   = 0;
	baseBlockMemory		   = 0;
	numUsedBlocks		   = 0;
	usedBlockMemory		   = 0;
	numFreeBlocks		   = 0;
	freeBlockMemory		   = 0;
	numAllocs			   = 0;
	numResizes			   = 0;
	numFrees			   = 0;

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	blockId[0] = 0x11111111;
	blockId[1] = 0x22222222;
	blockId[2] = 0x33333333;
#endif
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
idDynamicBlock<type>* idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::AllocInternal( const int num )
{
	idDynamicBlock<type>* block;
	int					  alignedBytes = ( num * sizeof( type ) + 15 ) & ~15;

	block = freeTree.FindSmallestLargerEqual( alignedBytes );
	if( block != NULL ) {
		UnlinkFreeInternal( block );
	} else if( allowAllocs ) {
		int allocSize = Max( baseBlockSize, alignedBytes + ( int )sizeof( idDynamicBlock<type> ) );
		block		  = ( idDynamicBlock<type>* )Mem_Alloc16( allocSize, _tag_ );
		if( lockMemory ) {
			// idLib::sys->LockMemory( block, baseBlockSize );
		}
#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
		memcpy( block->id, blockId, sizeof( block->id ) );
		block->allocator = ( void* )this;
#endif
		block->SetSize( allocSize - ( int )sizeof( idDynamicBlock<type> ), true );
		block->next = NULL;
		block->prev = lastBlock;
		if( lastBlock ) {
			lastBlock->next = block;
		} else {
			firstBlock = block;
		}
		lastBlock	= block;
		block->node = NULL;

		numBaseBlocks++;
		baseBlockMemory += allocSize;
	}

	return block;
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
idDynamicBlock<type>* idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::ResizeInternal( idDynamicBlock<type>* block, const int num )
{
	int alignedBytes = ( num * sizeof( type ) + 15 ) & ~15;

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	assert( block->id[0] == 0x11111111 && block->id[1] == 0x22222222 && block->id[2] == 0x33333333 && block->allocator == ( void* )this );
#endif

	// if the new size is larger
	if( alignedBytes > block->GetSize() ) {
		idDynamicBlock<type>* nextBlock = block->next;

		// try to annexate the next block if it's free
		if( nextBlock && !nextBlock->IsBaseBlock() && nextBlock->node != NULL && block->GetSize() + ( int )sizeof( idDynamicBlock<type> ) + nextBlock->GetSize() >= alignedBytes ) {
			UnlinkFreeInternal( nextBlock );
			block->SetSize( block->GetSize() + ( int )sizeof( idDynamicBlock<type> ) + nextBlock->GetSize(), block->IsBaseBlock() );
			block->next = nextBlock->next;
			if( nextBlock->next ) {
				nextBlock->next->prev = block;
			} else {
				lastBlock = block;
			}
		} else {
			// allocate a new block and copy
			idDynamicBlock<type>* oldBlock = block;
			block						   = AllocInternal( num );
			if( block == NULL ) { return NULL; }
			memcpy( block->GetMemory(), oldBlock->GetMemory(), oldBlock->GetSize() );
			FreeInternal( oldBlock );
		}
	}

	// if the unused space at the end of this block is large enough to hold a block with at least one element
	if( block->GetSize() - alignedBytes - ( int )sizeof( idDynamicBlock<type> ) < Max( minBlockSize, ( int )sizeof( type ) ) ) { return block; }

	idDynamicBlock<type>* newBlock;

	newBlock = ( idDynamicBlock<type>* )( ( ( byte* )block ) + ( int )sizeof( idDynamicBlock<type> ) + alignedBytes );
#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	memcpy( newBlock->id, blockId, sizeof( newBlock->id ) );
	newBlock->allocator = ( void* )this;
#endif
	newBlock->SetSize( block->GetSize() - alignedBytes - ( int )sizeof( idDynamicBlock<type> ), false );
	newBlock->next = block->next;
	newBlock->prev = block;
	if( newBlock->next ) {
		newBlock->next->prev = newBlock;
	} else {
		lastBlock = newBlock;
	}
	newBlock->node = NULL;
	block->next	   = newBlock;
	block->SetSize( alignedBytes, block->IsBaseBlock() );

	FreeInternal( newBlock );

	return block;
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::FreeInternal( idDynamicBlock<type>* block )
{
	assert( block->node == NULL );

#ifdef DYNAMIC_BLOCK_ALLOC_CHECK
	assert( block->id[0] == 0x11111111 && block->id[1] == 0x22222222 && block->id[2] == 0x33333333 && block->allocator == ( void* )this );
#endif

	// try to merge with a next free block
	idDynamicBlock<type>* nextBlock = block->next;
	if( nextBlock && !nextBlock->IsBaseBlock() && nextBlock->node != NULL ) {
		UnlinkFreeInternal( nextBlock );
		block->SetSize( block->GetSize() + ( int )sizeof( idDynamicBlock<type> ) + nextBlock->GetSize(), block->IsBaseBlock() );
		block->next = nextBlock->next;
		if( nextBlock->next ) {
			nextBlock->next->prev = block;
		} else {
			lastBlock = block;
		}
	}

	// try to merge with a previous free block
	idDynamicBlock<type>* prevBlock = block->prev;
	if( prevBlock && !block->IsBaseBlock() && prevBlock->node != NULL ) {
		UnlinkFreeInternal( prevBlock );
		prevBlock->SetSize( prevBlock->GetSize() + ( int )sizeof( idDynamicBlock<type> ) + block->GetSize(), prevBlock->IsBaseBlock() );
		prevBlock->next = block->next;
		if( block->next ) {
			block->next->prev = prevBlock;
		} else {
			lastBlock = prevBlock;
		}
		LinkFreeInternal( prevBlock );
	} else {
		LinkFreeInternal( block );
	}
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
ID_INLINE void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::LinkFreeInternal( idDynamicBlock<type>* block )
{
	block->node = freeTree.Add( block, block->GetSize() );
	numFreeBlocks++;
	freeBlockMemory += block->GetSize();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
ID_INLINE void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::UnlinkFreeInternal( idDynamicBlock<type>* block )
{
	freeTree.Remove( block->node );
	block->node = NULL;
	numFreeBlocks--;
	freeBlockMemory -= block->GetSize();
}

template<class type, int baseBlockSize, int minBlockSize, memTag_t _tag_>
void idDynamicBlockAlloc<type, baseBlockSize, minBlockSize, _tag_>::CheckMemory() const
{
	idDynamicBlock<type>* block;

	for( block = firstBlock; block != NULL; block = block->next ) {
		// make sure the block is properly linked
		if( block->prev == NULL ) {
			assert( firstBlock == block );
		} else {
			assert( block->prev->next == block );
		}
		if( block->next == NULL ) {
			assert( lastBlock == block );
		} else {
			assert( block->next->prev == block );
		}
	}
}

#endif /* !__HEAP_H__ */
