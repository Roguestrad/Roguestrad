/*****************************************************************************
The Dark Mod GPL Source Code

This file is part of the The Dark Mod Source Code, originally based
on the Doom 3 GPL Source Code as published in 2011.

The Dark Mod Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Project: The Dark Mod (http://www.thedarkmod.com/)

******************************************************************************/

#pragma once

#include "../math/Line.h"
#include "../containers/FlexList.h"

/*!
	\class idBoxOctreeHandle
	\brief A handle class used to associate objects with an octree structure for spatial indexing.
*/
class idBoxOctreeHandle
{
private:
	// indices of octree nodes which this object is attached to
	idFlexList<int, 2> ids;
	// bounds passed to idBoxOctree::Add (or idBoxOctree::Update)
	idBounds		   bounds;

	friend class idBoxOctree;

public:
	//! Returns true if the object with this handle is already included in the octree.
	ID_FORCE_INLINE bool IsLinked() const { return ids.Num() > 0; }
};

/*!
	\class idBoxOctree
	\brief Octree data structure for efficiently organizing and querying axis-aligned bounding boxes in 3D space.

	Provides spatial indexing for objects represented as axis-aligned bounding boxes, enabling efficient collision detection and spatial queries. The structure automatically manages tree depth and
   node splitting based on object density, ensuring large objects remain at higher levels while small objects can be deeply nested. Supports dynamic addition, removal, and updating of objects, along
   with various query operations including static box intersection and moving box intersection tests. The implementation uses chunked storage for efficient memory usage and provides methods for both
   immediate and recursive operations. Designed for applications requiring frequent spatial lookups, such as collision detection systems.

*/
class idBoxOctree
{
public:
	// pointer to object stored in octree
	// this is usually idClipModel*, but can be something else as well
	typedef void* Pointer;

	// function pointer for getting handle stored inside object
	// usually it returns ((idClipModel*)ptr)->octreeHandle
	typedef idBoxOctreeHandle& ( *HandleGetter )( Pointer );

	// maximum number of objects per chunk
	// if exceeded, then more chunks are chained in a linked list
	static const int CHUNK_SIZE = 127; // 4 KB chunk

	// critical number of objects which should ideally reside in smaller cells
	// when this number of "small" objects is gathered, octree node is split into 8 subnodes
	static const int SPLIT_SMALL_NUM = 90;

	// link to single object
	struct Link {
		Pointer	 object;
		idBounds bounds;
	};
	// a bunch of links to objects
	struct Chunk {
		int	   num;
		Chunk* next;
		Link   arr[CHUNK_SIZE];
	};

	// number of chunk pointers embedded in QueryResult object (same as CLIPARRAY_AUTOSIZE)
	static const int							RESULT_AUTOSIZE = 128;

	// returned by Query methods
	// called should walk through all objects mentioned in the returned chunks
	// note: ignore "next" member, iterate over arr[0..num) for every chunk
	typedef idFlexList<Chunk*, RESULT_AUTOSIZE> QueryResult;

	//! Initializes a new instance of the idBoxOctree class.
	idBoxOctree();

	//! Destructor for the idBoxOctree class that clears all contained data.
	~idBoxOctree();

	//! Initializes the octree with the specified world bounds and handle getter function.
	void Init( const idBounds& worldBounds, HandleGetter getHandle );

	//! Condenses chunks in the octree by merging items and removing empty chunks
	void Condense();

	//! Removes all elements from the octree.
	void Clear();

	//! Adds an object with the specified box to the octree
	void Add( Pointer ptr, const idBounds& box );

	//! Removes an object from the octree.
	void Remove( Pointer ptr );

	//! Updates the position and bounds of an object in the octree structure.
	void Update( Pointer ptr, const idBounds& box );

	//! Finds objects with bounding boxes intersecting the specified box and returns a list of chunks that may contain such objects.
	void QueryInBox( const idBounds& box, QueryResult& res ) const;

	//! Finds objects with bounding boxes intersecting a specified moving box and returns a list of chunks that may contain such objects
	void QueryInMovingBox( const idBounds& box, const idVec3& start, const idVec3& invDir, const idVec3& radius, QueryResult& res ) const;

private:
	struct QueryContext;
	struct AddContext;
	struct CellRanges;

	//! Returns the octree level for the given bounding box.
	int		   GetLevel( const idBounds& box ) const;

	//! Calculates the cell ranges for a given bounding box within the octree structure.
	CellRanges GetCellRanges( const idBounds& box, int maxDepth ) const;

	//! Recursively queries the octree for objects within a specified bounding box.
	void	   Query_r( QueryContext& ctx, int nodeIdx, const idBounds& cellBox, const idBounds& spaceBox ) const;

	//! Recursively adds an object to the octree nodes based on its bounding box containment.
	void	   Add_r( AddContext& ctx, int nodeIdx, const idBounds& cellBox );

	//! Adds an object to a specific node in the octree, potentially splitting the node if it becomes too crowded
	void	   AddToNode( AddContext& ctx, int nodeIdx, const idBounds& cellBox );

	struct OctreeNode {
		Chunk* links	   = nullptr; // linked list of chunks attached to node
		int	   firstSonIdx = -1;	  // sons have indices [firstSonIdx .. firstSonIdx + 8)
		short  depth	   = 0;
		short  numSmall	   = 0; // number of objects with GetLevel(obj) > depth
	};

	// this function gives access to handle within an object
	HandleGetter			  getHandle = nullptr;

	// space area at the root node, which is subdivided by octree
	// note: objects may go outside worldBounds, but too many outliers will harm performance
	idBounds				  worldBounds;

	// 1 / worldSize for each coordinate
	idVec3					  invWorldSize;

	// all nodes of octree
	idList<OctreeNode>		  nodes;

	// allocates ~4 MB blocks from where chunks are quickly allocated
	idBlockAlloc<Chunk, 1024> allocator;
};
