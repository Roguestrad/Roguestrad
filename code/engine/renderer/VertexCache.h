/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2016-2017 Dustin Land
Copyright (C) 2022 Stephen Pridham
Copyright (C) 2024 Robert Beckebans

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
#ifndef __VERTEXCACHE_H__
#define __VERTEXCACHE_H__

#if 1

// RB: quadruppled static memory limits for custom content

const int	   VERTCACHE_INDEX_MEMORY_PER_FRAME	 = 31 * 1024 * 1024;
const int	   VERTCACHE_VERTEX_MEMORY_PER_FRAME = 31 * 1024 * 1024;
const int	   VERTCACHE_JOINT_MEMORY_PER_FRAME	 = 256 * 1024;

// there are a lot more static indexes than vertexes, because interactions are just new
// index lists that reference existing vertexes
const int	   STATIC_INDEX_MEMORY	= 4 * 31 * 1024 * 1024;
const int	   STATIC_VERTEX_MEMORY = 4 * 31 * 1024 * 1024; // make sure it fits in VERTCACHE_OFFSET_MASK!

// vertCacheHandle_t packs size, offset, and frame number into 64 bits
typedef uint64 vertCacheHandle_t;
const int	   VERTCACHE_STATIC = 1; // in the static set, not the per-frame set

const int	   VERTCACHE_SIZE_SHIFT = 1;
const int	   VERTCACHE_SIZE_MASK	= 0x7fffff; // 23 bits = 8 megs

const int	   VERTCACHE_OFFSET_SHIFT = 24;
const int	   VERTCACHE_OFFSET_MASK  = 0x7ffffff; // 27 bits = 128 megs

const int	   VERTCACHE_FRAME_SHIFT = 51;
const int	   VERTCACHE_FRAME_MASK	 = 0x1fff; // 13 bits = 8191 frames to wrap around

#else

// RB: original values which are still good for low spec hardware and performance

const int	   VERTCACHE_INDEX_MEMORY_PER_FRAME	 = 31 * 1024 * 1024;
const int	   VERTCACHE_VERTEX_MEMORY_PER_FRAME = 31 * 1024 * 1024;
const int	   VERTCACHE_JOINT_MEMORY_PER_FRAME	 = 256 * 1024;

// there are a lot more static indexes than vertexes, because interactions are just new
// index lists that reference existing vertexes
const int	   STATIC_INDEX_MEMORY	= 31 * 1024 * 1024;
const int	   STATIC_VERTEX_MEMORY = 31 * 1024 * 1024; // make sure it fits in VERTCACHE_OFFSET_MASK!

// vertCacheHandle_t packs size, offset, and frame number into 64 bits
typedef uint64 vertCacheHandle_t;
const int	   VERTCACHE_STATIC		  = 1; // in the static set, not the per-frame set
const int	   VERTCACHE_SIZE_SHIFT	  = 1;
const int	   VERTCACHE_SIZE_MASK	  = 0x7fffff; // 8 megs
const int	   VERTCACHE_OFFSET_SHIFT = 24;
const int	   VERTCACHE_OFFSET_MASK  = 0x1ffffff; // 32 megs
const int	   VERTCACHE_FRAME_SHIFT  = 49;
const int	   VERTCACHE_FRAME_MASK	  = 0x7fff; // 15 bits = 32k frames to wrap around, python hex( ( 1 << 15 ) - 1 )

#endif

const int VERTEX_CACHE_ALIGN = 32;
const int INDEX_CACHE_ALIGN	 = 16;
const int JOINT_CACHE_ALIGN	 = 16;

enum cacheType_t { CACHE_VERTEX, CACHE_INDEX, CACHE_JOINT };

struct geoBufferSet_t {
	idIndexBuffer			indexBuffer;
	idVertexBuffer			vertexBuffer;
	idUniformBuffer			jointBuffer;
	byte*					mappedVertexBase;
	byte*					mappedIndexBase;
	byte*					mappedJointBase;
	idSysInterlockedInteger indexMemUsed;
	idSysInterlockedInteger vertexMemUsed;
	idSysInterlockedInteger jointMemUsed;
	int						allocations; // number of index and vertex allocations combined
};

/*!
	\class idVertexCache
	\brief Manages temporary and static vertex, index, and joint data for GPU rendering.

	Provides a caching system for geometry data used in rendering operations, supporting both temporary frame-specific allocations and static data that persists across frames. The cache handles
   vertex, index, and joint data separately with appropriate memory management and alignment. Memory is allocated in chunks and can be mapped for CPU writing before being uploaded to the GPU. The
   system supports both dynamic and static allocation patterns, with automatic cleanup of temporary data at the end of each frame. The cache can be initialized with a specific alignment requirement
   and operates through a command list interface for GPU operations. Static data allocation is separate from the regular frame-based allocation and is used for data that does not change between
   frames.

*/
class idVertexCache
{
public:
	//! Initializes the vertex cache with specified alignment and command list
	void			  Init( int uniformBufferOffsetAlignment, nvrhi::ICommandList* commandList );

	//! Shuts down the vertex cache by freeing all buffer objects.
	void			  Shutdown();

	//! Clears all vertex cache data and reinitializes it with the specified command list.
	void			  PurgeAll( nvrhi::ICommandList* commandList );

	//! Frees static data used by the vertex cache.
	void			  FreeStaticData();

	/*!
		\brief Allocates vertex cache memory for rendering data that is valid for one frame.

		This function allocates a block of vertex cache memory for storing vertex data that will be used during rendering. The allocated memory is only valid for one frame and will be automatically
	   disposed of at the end of the frame. The function supports allocating memory with or without initial data, and can optionally specify a command list for GPU operations. The returned handle can
	   be used to map the allocated memory for writing vertex data.

		\param data Pointer to the initial vertex data to copy into the cache, or NULL if no initial data is provided
		\param num Number of vertices to allocate space for
		\param size Size of each vertex structure, defaults to sizeof(idDrawVert)
		\param commandList Optional command list for GPU operations, can be NULL
		\return A handle to the allocated vertex cache block that can be used to map the memory for writing
	*/
	vertCacheHandle_t AllocVertex( const void* data, int num, size_t size = sizeof( idDrawVert ), nvrhi::ICommandList* commandList = nullptr );

	/*!
		\brief Allocates index buffer memory for vertex caching in the rendering pipeline

		This function allocates memory for index data within the vertex cache system, which is used to store and manage vertex and index buffers for rendering operations. The allocated memory is
	   specific to index data and can be used to store triangle indices for mesh rendering. The function handles the actual allocation by delegating to the internal ActuallyAlloc function, which
	   manages the cache allocation and potentially binds the buffer to a command list for GPU operations. The size parameter defaults to the size of a triangle index type, and the commandList
	   parameter allows for specifying a command list for GPU command recording.

		\param data Pointer to the index data to be stored, or NULL to allocate memory without initial data
		\param num Number of indices to allocate
		\param size Size of each index element, defaults to size of triIndex_t
		\param commandList Optional command list for GPU command recording, can be NULL
		\return Handle to the allocated index buffer in the vertex cache
	*/
	vertCacheHandle_t AllocIndex( const void* data, int num, size_t size = sizeof( triIndex_t ), nvrhi::ICommandList* commandList = nullptr );

	/*!
		\brief Allocates joint animation data in the vertex cache for rendering

		This function allocates joint animation data in the vertex cache, typically used for GPU skinning operations. It takes joint data, number of joints, and optional size parameter, then stores
	   the data in the cache for use during rendering. The function is commonly called during model setup where joint matrices need to be uploaded to the GPU for skinning calculations.

		\param data Pointer to the joint data to be cached
		\param num Number of joints to cache
		\param size Size of each joint element, defaults to size of idJointMat
		\param commandList Command list for GPU operations, can be null
		\return Handle to the cached joint data that can be used for rendering
	*/
	vertCacheHandle_t AllocJoint( const void* data, int num, size_t size = sizeof( idJointMat ), nvrhi::ICommandList* commandList = nullptr );

	//! Allocates static vertex data into the vertex cache
	vertCacheHandle_t AllocStaticVertex( const void* data, int bytes, nvrhi::ICommandList* commandList );

	//! Allocates static index data for vertex caching
	vertCacheHandle_t AllocStaticIndex( const void* data, int bytes, nvrhi::ICommandList* commandList );

	//! Returns a pointer to the mapped vertex buffer for the specified cache handle.
	byte*			  MappedVertexBuffer( vertCacheHandle_t handle );

	//! Returns a pointer to the mapped index buffer for the specified vertex cache handle.
	byte*			  MappedIndexBuffer( vertCacheHandle_t handle );

	//! Checks if the vertex cache handle is still valid and has not been purged.
	bool			  CacheIsCurrent( const vertCacheHandle_t handle );

	//! Checks if the given vertex cache handle represents a static cache entry.
	static bool		  CacheIsStatic( const vertCacheHandle_t handle ) { return ( handle & VERTCACHE_STATIC ) != 0; }

	//! Retrieves a vertex buffer reference from the cache using the provided handle.
	bool			  GetVertexBuffer( vertCacheHandle_t handle, idVertexBuffer* vb );

	//! Retrieves the index buffer for a given vertex cache handle.
	bool			  GetIndexBuffer( vertCacheHandle_t handle, idIndexBuffer* ib );

	//! Retrieves the joint buffer for a given vertex cache handle and references it into the provided uniform buffer.
	bool			  GetJointBuffer( vertCacheHandle_t handle, idUniformBuffer* jb );

	//! Initializes the vertex cache for the next frame by unmapping the current frame and preparing the next frame for CPU writing.
	void			  BeginBackEnd();

public:
	int				  currentFrame; // for determining the active buffers
	int				  listNum;		// currentFrame % NUM_FRAME_DATA
	int				  drawListNum;	// (currentFrame-1) % NUM_FRAME_DATA

	geoBufferSet_t	  staticData;
	geoBufferSet_t	  frameData[NUM_FRAME_DATA];

	int				  uniformBufferOffsetAlignment;

	// High water marks for the per-frame buffers
	int				  mostUsedVertex;
	int				  mostUsedIndex;
	int				  mostUsedJoint;

	/*!
		\brief Allocates vertex cache memory for geometry data with specified alignment and updates the buffer.

		This function allocates memory in the vertex cache for geometry data based on the specified cache type. It handles three cache types: index, vertex, and joint, each with their own alignment
	   requirements. The function ensures proper alignment of the data and updates the corresponding buffer with the provided data if it is not null. It also tracks allocation statistics and
	   constructs a handle to reference the allocated memory.

		\param vcs Reference to the geometry buffer set to allocate from
		\param data Pointer to the data to be copied into the cache, or NULL if no initial data is provided
		\param bytes Size in bytes of the data to be allocated
		\param type Type of cache to allocate memory in
		\param commandList Command list to use for buffer updates
		\return A handle to the allocated cache memory that can be used to reference the data later
		\throws Error if there is insufficient space in the cache for the requested allocation
	*/
	vertCacheHandle_t ActuallyAlloc( geoBufferSet_t& vcs, const void* data, int bytes, cacheType_t type, nvrhi::ICommandList* commandList );
};

// platform specific code to memcpy into vertex buffers efficiently
// 16 byte alignment is guaranteed
void				 CopyBuffer( byte* dst, const byte* src, int numBytes );

extern idVertexCache vertexCache;

#endif // __VERTEXCACHE_H__
