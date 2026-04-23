/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2016-2017 Dustin Land
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
#ifndef __BUFFEROBJECT_H__
#define __BUFFEROBJECT_H__

#include <nvrhi/nvrhi.h>

#if defined( USE_AMD_ALLOCATOR )
	#include <nvrhi/vulkan.h>
	#include "vk_mem_alloc.h"
#endif

enum bufferMapType_t {
	BM_READ, // map for reading
	BM_WRITE // map for writing
};

enum bufferUsageType_t {
	BU_STATIC,	// GPU R
	BU_DYNAMIC, // GPU R, CPU R/W
};

//! Unbinds buffer objects from OpenGL targets to switch to virtual memory usage.
void UnbindBufferObjects();
bool IsWriteCombined( void* base );
void CopyBuffer( byte* dst, const byte* src, int numBytes );

/*!
	\class idBufferObject
	\brief Manages GPU buffer objects with memory mapping and ownership tracking.

	The idBufferObject class provides a wrapper for GPU buffer resources, maintaining tracking of buffer size, allocation details, usage type, and memory mapping state. It supports ownership tracking
   to determine if the object is responsible for managing the underlying buffer resource. The class facilitates access to the underlying API buffer handle and provides utilities for setting debug
   names and managing the buffer's mapped state. Memory management is handled through allocation size tracking rounded to 16-byte boundaries and offset tracking with ownership flag stripping.

*/
class idBufferObject
{
public:
	//! Initializes a new instance of the idBufferObject class.
	idBufferObject();

	//! Returns the size of the buffer object, excluding the mapped flag bit.
	int				  GetSize() const { return ( size & ~MAPPED_FLAG ); }

	//! Returns the allocated size of the buffer object, rounded up to the nearest 16-byte boundary.
	int				  GetAllocedSize() const { return ( ( size & ~MAPPED_FLAG ) + 15 ) & ~15; }

	//! Returns the usage type of the buffer object.
	bufferUsageType_t GetUsage() const { return usage; }

	//! Returns the NVRHI buffer handle associated with this buffer object.
	nvrhi::IBuffer*	  GetAPIObject() const { return bufferHandle; }

	//! Returns the offset value with the buffer ownership flag stripped
	int				  GetOffset() const { return ( offsetInOtherBuffer & ~OWNS_BUFFER_FLAG ); }

	//! Returns true if the buffer object is currently mapped in memory.
	bool			  IsMapped() const { return ( size & MAPPED_FLAG ) != 0; }

	//! Sets the debug name for the buffer object.
	void			  SetDebugName( idStr str ) { debugName = str; }

protected:
	//! Marks this buffer object as mapped.
	void SetMapped() const { const_cast<int&>( size ) |= MAPPED_FLAG; }

	//! Sets the unmapped state of the buffer object.
	void SetUnmapped() const { const_cast<int&>( size ) &= ~MAPPED_FLAG; }

	//! Returns true if this buffer object owns its underlying buffer
	bool OwnsBuffer() const { return ( ( offsetInOtherBuffer & OWNS_BUFFER_FLAG ) != 0 ); }

protected:
	int						 size;				  // size in bytes
	int						 offsetInOtherBuffer; // offset in bytes
	bufferUsageType_t		 usage;

	nvrhi::InputLayoutHandle inputLayout;
	nvrhi::BufferHandle		 bufferHandle;
	void*					 buffer;
	idStr					 debugName;

#if defined( USE_AMD_ALLOCATOR )
	VkBuffer		  vkBuffer;
	VmaAllocation	  allocation;
	VmaAllocationInfo allocationInfo;
#endif

	// sizeof() confuses typeinfo...
	static const int MAPPED_FLAG	  = 1 << ( 4 /* sizeof( int ) */ * 8 - 1 );
	static const int OWNS_BUFFER_FLAG = 1 << ( 4 /* sizeof( int ) */ * 8 - 1 );
};

/*!
	\class idVertexBuffer
	\brief Manages GPU vertex buffer objects for rendering operations.

	The idVertexBuffer class encapsulates GPU vertex buffer functionality, providing methods for allocation, updating, and managing vertex data on the graphics device. It supports both static and
   dynamic buffer usage patterns, with dynamic buffers allowing CPU access for updates. The class handles buffer allocation using either a custom allocator or the NV-RHI path, and provides mapping
   capabilities for CPU access to buffer contents. Buffer updates can be performed with or without command lists depending on whether the buffer is dynamic or static. The class supports referencing
   other buffer objects for data sharing and provides mechanisms for clearing buffer contents without releasing the underlying memory. The implementation includes proper alignment handling and
   validation to ensure correct GPU buffer operations.

*/
class idVertexBuffer : public idBufferObject
{
public:
	//! Initializes a new instance of the idVertexBuffer class.
	idVertexBuffer();

	//! Destructor for idVertexBuffer that releases the buffer object.
	~idVertexBuffer();

	/*!
		\brief Allocates a GPU buffer object for vertex data with specified size, usage, and optional initial data.

		This function initializes a vertex buffer object on the GPU, allocating memory based on the provided size and alignment requirements. It supports both static and dynamic buffer usage types,
	   with dynamic buffers being accessible from the CPU for updates. The function handles memory allocation using either a custom AMD allocator or the default NV-RHI buffer creation path. If initial
	   data is provided, it will be copied into the allocated buffer. The function asserts that the buffer is not already allocated and that the input data is 16-byte aligned.

		\param data Optional pointer to the initial vertex data to be copied into the buffer, must be 16-byte aligned if provided
		\param allocSize Size in bytes of the buffer to allocate, must be positive
		\param usage Buffer usage type indicating whether the buffer is static or dynamic
		\param commandList Optional command list for submission of buffer update operations
		\return True if the buffer allocation and optional data copy succeeded, false otherwise
		\throws idLib::Error if allocSize is zero or negative
	*/
	bool		AllocBufferObject( const void* data, int allocSize, bufferUsageType_t usage, nvrhi::ICommandList* commandList );

	//! Releases the GPU buffer resources held by the vertex buffer object.
	void		FreeBufferObject();

	//! Makes this buffer a reference to another buffer.
	void		Reference( const idVertexBuffer& other );

	//! Sets this vertex buffer to reference another vertex buffer's data with specified offset and size.
	void		Reference( const idVertexBuffer& other, int refOffset, int refSize );

	/*!
		\brief Updates the vertex buffer with new data starting at the specified offset

		This function copies the provided data into the vertex buffer at the specified offset. It handles both dynamic and static buffer update scenarios. For dynamic buffers, it directly copies the
	   data into the mapped memory region. For static buffers, it uses the command list to perform the buffer update operation. The function performs several assertions to ensure the buffer is valid
	   and properly aligned before performing the update. The update size is rounded up to the nearest 16-byte boundary for proper alignment.

		\param data Pointer to the source data to copy into the buffer
		\param size Size of the data to update in bytes
		\param offset Offset in bytes within the buffer where the update should start
		\param initialUpdate Flag indicating whether this is the initial update operation
		\param commandList Command list to use for GPU commands when updating static buffers
		\throws FatalError when the update would exceed the buffer size
	*/
	void		Update( const void* data, int size, int offset, bool initialUpdate, nvrhi::ICommandList* commandList ) const;

	//! Maps the vertex buffer for CPU access based on the specified map type
	void*		MapBuffer( bufferMapType_t mapType );

	//! Maps the vertex buffer for the specified map type and returns a pointer to the mapped vertex data.
	idDrawVert* MapVertexBuffer( bufferMapType_t mapType ) { return static_cast<idDrawVert*>( MapBuffer( mapType ) ); }

	//! Unmaps the vertex buffer from the GPU memory.
	void		UnmapBuffer();

private:
	//! Clears the vertex buffer contents without freeing the underlying buffer.
	void ClearWithoutFreeing();

	DISALLOW_COPY_AND_ASSIGN( idVertexBuffer );
};

/*!
	\class idIndexBuffer
	\brief A GPU buffer object for managing index data used in rendering operations.

	This class provides a mechanism for allocating, updating, and managing GPU buffer resources specifically for index data used in graphics rendering. It supports both static and dynamic buffer usage
   patterns, with the ability to initialize buffers with initial data or update them later. The class handles memory mapping for CPU access and integrates with command lists for GPU buffer updates. It
   supports reference semantics allowing one buffer to share memory with another, and provides utilities for clearing buffer contents without releasing memory. The implementation is designed to work
   with GPU resource management systems, supporting efficient access patterns for rendering pipelines.

*/
class idIndexBuffer : public idBufferObject
{
public:
	//! Initializes a new instance of the idIndexBuffer class.
	idIndexBuffer();

	//! Destructor for idIndexBuffer that releases the associated buffer object.
	~idIndexBuffer();

	/*!
		\brief Allocates a buffer object for index data with specified usage and optional initial data.

		This function initializes an index buffer object with the provided data size and usage type. It sets up the buffer descriptor with appropriate parameters based on whether the buffer is static
	   or dynamic. The function handles allocation using either a custom allocator or the default device allocator depending on configuration. If initial data is provided, it updates the buffer with
	   that data.

		\param data Pointer to the initial data to be copied into the buffer, or nullptr if no initial data is provided
		\param allocSize Size in bytes of the buffer to be allocated
		\param usage Usage type of the buffer, determining allocation strategy and access patterns
		\param commandList Command list to be used for buffer update operations if initial data is provided
		\return True if buffer allocation and initialization succeed, false otherwise
		\throws An error is thrown if allocSize is less than or equal to zero
	*/
	bool		AllocBufferObject( const void* data, int allocSize, bufferUsageType_t usage, nvrhi::ICommandList* commandList );

	//! Frees the GPU buffer resources associated with this index buffer object.
	void		FreeBufferObject();

	//! Makes this index buffer a reference to another index buffer.
	void		Reference( const idIndexBuffer& other );

	//! Initializes this index buffer to reference a portion of another index buffer
	void		Reference( const idIndexBuffer& other, int refOffset, int refSize );

	/*!
		\brief Updates the index buffer data with new content at a specified offset.

		This function copies the provided data into the index buffer at the given offset. It supports both dynamic and static buffer usage modes. For dynamic buffers, it directly copies the data into
	   the mapped buffer. For static buffers, it uses the provided command list to write the data to the GPU buffer. The function validates input parameters including alignment and size constraints.
	   The update size is rounded up to the nearest 16-byte boundary to ensure proper alignment.

		\param data Pointer to the source data to copy into the buffer
		\param size Size of the data to update in bytes
		\param offset Offset in bytes within the buffer where the update should start
		\param initialUpdate Flag indicating if this is the first update to the buffer, which affects command list handling
		\param commandList Command list used for GPU buffer updates in static buffer mode
		\throws Fatal error when the update size would exceed the buffer bounds
	*/
	void		Update( const void* data, int size, int offset, bool initialUpdate, nvrhi::ICommandList* commandList ) const;

	//! Maps the index buffer for CPU access based on the specified map type
	void*		MapBuffer( bufferMapType_t mapType );

	//! Maps the index buffer for reading or writing based on the specified map type and returns a pointer to the mapped memory.
	triIndex_t* MapIndexBuffer( bufferMapType_t mapType ) { return static_cast<triIndex_t*>( MapBuffer( mapType ) ); }

	//! Unmaps the index buffer from the CPU address space.
	void		UnmapBuffer();

private:
	//! Clears the index buffer without freeing the underlying memory.
	void ClearWithoutFreeing();

	DISALLOW_COPY_AND_ASSIGN( idIndexBuffer );
};

/*!
	\class idUniformBuffer
	\brief Manages GPU uniform buffer objects for rendering commands.

	This class provides a wrapper for GPU uniform buffer objects, handling allocation, updates, and memory management for uniform data used in rendering. It supports both dynamic and static buffer
   usage patterns, with appropriate handling of buffer updates through command lists for static buffers and direct mapping for dynamic buffers. The class ensures proper alignment and size validation
   during buffer operations, and supports referencing other buffer objects for efficient memory usage. It integrates with GPU command submission for static buffer updates and provides CPU mapping
   capabilities for direct buffer access when needed.

*/
class idUniformBuffer : public idBufferObject
{
public:
	//! Initializes a new uniform buffer object.
	idUniformBuffer();

	//! Destroys the uniform buffer object and releases its resources.
	~idUniformBuffer();

	/*!
		\brief Allocates a buffer object for uniform data with the specified size, usage, and optional initial data.

		This function initializes a uniform buffer object using the provided data and allocation size. It performs alignment checks and validates the allocation size. The buffer is created with
	   appropriate usage flags and memory properties based on whether the buffer is dynamic or static. If initial data is provided, it is copied into the buffer. The function returns true on
	   successful allocation, false otherwise.

		\param data Optional initial data to copy into the buffer
		\param allocSize Size in bytes of the buffer to allocate
		\param usage Usage type flag indicating if the buffer is dynamic or static
		\param commandList Command list to use for the buffer update operation
		\return True if the buffer was successfully allocated and initialized, false otherwise
		\throws idLib::Error if the allocation size is invalid
	*/
	bool  AllocBufferObject( const void* data, int allocSize, bufferUsageType_t usage, nvrhi::ICommandList* commandList );

	//! Frees the GPU buffer resources associated with this uniform buffer object.
	void  FreeBufferObject();

	//! Makes this buffer a reference to another buffer.
	void  Reference( const idUniformBuffer& other );

	//! Sets up this uniform buffer to reference a portion of another uniform buffer.
	void  Reference( const idUniformBuffer& other, int refOffset, int refSize );

	/*!
		\brief Updates the uniform buffer with the provided data, handling both dynamic and static buffer cases.

		This function copies the specified data into the uniform buffer at the given offset. For dynamic buffers, it directly updates the mapped buffer memory. For static buffers, it uses the command
	   list to write the buffer data. The function performs several assertions to ensure alignment and size constraints are met. It also handles initial updates by tracking buffer states properly.

		\param data Pointer to the source data to be copied into the buffer
		\param size Size of the data to be copied, rounded up to 16-byte alignment
		\param offset Offset in the buffer where the data should be written
		\param initialUpdate Flag indicating if this is the initial update to the buffer
		\param commandList Command list used to record buffer write operations for static buffers
		\throws FatalError if the update would overflow the buffer size
	*/
	void  Update( const void* data, int size, int offset, bool initialUpdate, nvrhi::ICommandList* commandList ) const;

	//! Maps the uniform buffer for CPU access based on the specified map type
	void* MapBuffer( bufferMapType_t mapType );

	//! Unmaps the uniform buffer from the CPU address space.
	void  UnmapBuffer();

private:
	//! Clears the uniform buffer data without freeing the underlying buffer.
	void ClearWithoutFreeing();

	DISALLOW_COPY_AND_ASSIGN( idUniformBuffer );
};

#endif // !__BUFFEROBJECT_H__
