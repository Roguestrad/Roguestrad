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

/*!
	\file idlib/bv/BoxOctree.h
	\brief Defines a spatial partitioning structure (an Octree) designed for efficient intersection queries of axis-aligned bounding boxes (AABB).
	\note archgen: sha256=45f663ff2a3adafc48186fae4dec4a226117e3cbd4b84508db4412516bd4ee63

	\par File Purpose
	- Defines a spatial partitioning structure (an Octree) designed for efficient intersection queries of axis-aligned bounding boxes (AABB).
	- Provides a mechanism for tracking objects within the tree via handles to allow efficient updates and removals.

	\par Core Responsibilities
	- Spatial indexing of arbitrary pointers (payloads) associated with Bounding Boxes.
	- Hierarchical subdivision of 3D space into octants based on object density.
	- Efficiently managing memory for spatial data using chunked arrays and block allocation.
	- Performing spatial queries including static box intersections and swept-volume (moving box) intersections.
	- Optimizing object updates by differentiating between 'fast-path' (bounds change only) and 'slow-path' (re-insertion) logic.

	\par Key Types and Functions
	- idBoxOctreeHandle — An object-side tracker that stores the indices of all octree nodes an object currently inhabits, enabling $O(1)$ node lookups.
	- idBoxOctree::Pointer — A type alias for the payload stored in the tree, typically `idClipModel*` or similar.
	- idBoxOctree::HandleGetter — A function pointer signature used to retrieve an `idBoxOctreeHandle` from a payload pointer.
	- idBoxOctree::Link — A structure pair containing the object pointer and its bounding box, representing a single entry in a chunk.
	- idBoxOctree::Chunk — A fixed-size array of `Link` objects, part of a linked list used to store objects within a node.
	- idBoxOctree::QueryResult — A fixed-capacity list of pointers to `Chunk` structures returned by spatial queries.
	- idBoxOctree::Add(Pointer, idBounds) — Recursively inserts an object into the hierarchy, potentially triggering a node split if a threshold is met.
	- idBoxOctree::Update(Pointer, idBounds) — Updates an object's bounds; performs a fast update if the object remains within its current nodes, otherwise re-inserts the object.
	- idBoxOctree::QueryInBox(idBounds, QueryResult&) — Traverses the tree to collect all chunks containing objects that intersect the specified volume.
	- idBoxOctree::QueryInMovingBox(idBounds, idVec3, idVec3, idVec3, QueryResult&) — Performs a swept-volume query to find objects intersecting a moving AABB.
	- idBoxOctree::Condense() — An optimization pass that merges sparse chunks and deallocates unused memory segments.

	\par Control Flow
	- Insertion Flow: `Add` is called $\rightarrow$ `Add_r` traverses the tree $\rightarrow$ If the object's size is relatively small and the current node's `numSmall` exceeds `SPLIT_SMALL_NUM`, the
   node splits into 8 children $\rightarrow$ Objects are sifted down to the appropriate level.
	- Update Flow: `Update` checks if the new `idBounds` still fit within the existing node assignments via the `idBoxOctreeHandle` $\rightarrow$ If valid, it updates the bounds in place $\rightarrow$
   If the object has moved out of its current nodes, it triggers `Remove` followed by `Add`.
	- Query Flow: `QueryInBox` or `QueryInMovingBox` initiates `Query_r` $\rightarrow$ The algorithm recursively traverses nodes, pruning branches where the node's `cellBox` does not intersect the
   query volume $\rightarrow$ Intersecting chunks are appended to the `QueryResult`.

	\par Dependencies
	- ../math/Line.h — Provides the `idBounds` and `idVec3` structures used for geometric calculations.
	- ../containers/FlexList.h — Provides the `idFlexList` container used for handles and query results.
	- idBlockAlloc (implied) — Used for high-performance, block-based allocation of `Chunk` structures.

	\par How It Fits
	- Serves as the primary spatial acceleration structure for the engine's collision and visibility systems.
	- Specifically utilized by `idClip` to store and query clipmodels for all entities in the game world.
	- Provides the underlying geometric query interface used by higher-level physics and rendering subsystems to prune unnecessary intersection tests.
*/

#pragma once

#include "../math/Line.h"
#include "../containers/FlexList.h"

/*!
	\class idBoxOctreeHandle
	\brief Handle for managing objects within a box octree structure.

	The idBoxOctreeHandle class provides a mechanism for tracking and managing objects within a box octree data structure. It serves as a lightweight reference that can indicate whether an object is
   currently linked to the octree. The IsLinked method allows for quick verification of an object's inclusion status, which is essential for maintaining consistency during dynamic updates to the
   octree. This handle is designed to integrate seamlessly with the octree's management systems, enabling efficient spatial queries and object tracking. The class is intended to be used primarily by
   the octree implementation itself rather than by external systems, as it focuses on internal object linkage management rather than providing access to the actual octree data.

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
	\brief idBoxOctree is a spatial data structure for efficient object management and query operations in 3D space.

	The idBoxOctree class implements a hierarchical spatial partitioning system that organizes objects within a 3D world bounding volume. It supports dynamic insertion, removal, and updating of
   objects with associated bounding boxes, making it suitable for collision detection, visibility determination, and other spatial queries. The octree structure maintains a tree of nodes where each
   node represents a cubic region of space, and objects are stored in the leaf nodes that best contain them. Objects can be queried by static bounding boxes or moving boxes with specified
   trajectories, enabling efficient intersection testing with minimal overhead. The implementation handles memory management through chunk-based storage for efficient object allocation and automatic
   tree restructuring during insertions that cause node overflow. The design optimizes for frequent queries with minimal updates, trading some insertion complexity for fast retrieval operations.

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

	struct Link {
		Pointer	 object;
		idBounds bounds;
	};

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

	//! Initializes an empty idBoxOctree instance.
	idBoxOctree();

	/*!
		\brief Destructor for idBoxOctree that clears all internal data.

		This function is the destructor for the idBoxOctree class. It is responsible for cleaning up all internal resources and data structures managed by the octree. The destructor calls the Clear
	   method to ensure that all nodes and elements within the octree are properly released before the object is destroyed.

	*/
	~idBoxOctree();

	/*!
		\brief Initializes the octree with the specified world bounds and handle getter function.

		This function sets up the octree data structure for spatial indexing. It clears any existing data, stores the provided world bounds and handle getter function, and initializes the node array.
	   The inverse world size is calculated for use in coordinate transformations.

		\param worldBounds The bounding box defining the world space covered by the octree
		\param getHandle Function used to retrieve handles for objects in the octree
	*/
	void Init( const idBounds& worldBounds, HandleGetter getHandle );

	/*!
		\brief Reorganizes and condenses chunks in the octree by merging items and freeing empty chunks.

		This function processes each node in the octree to condense chunk data. It collects all chunks associated with a node, reverses their order to maintain chronological sequence, and then merges
	   the items from multiple chunks into fewer chunks where possible. Empty chunks are identified and freed to reduce memory usage. The function ensures that items are properly consolidated while
	   maintaining the integrity of the octree structure.

	*/
	void Condense();

	//! Clears all nodes and their associated data from the octree structure.
	void Clear();

	/*!
		\brief Adds an object with the specified bounding box to the octree structure

		This function inserts a pointer to an object along with its bounding box into the octree data structure. It first verifies that the object is not already linked in the octree, then determines
	   the appropriate level in the octree hierarchy based on the bounding box dimensions. The actual insertion is performed recursively through the Add_r function. The function also updates the
	   handle's bounds member with the provided box value.

		\param ptr Pointer to the object being added
		\param box Bounding box of the object being added
		\throws _assertion failure if the pointer is already linked in the octree_
	*/
	void Add( Pointer ptr, const idBounds& box );

	/*!
		\brief Removes an object from the octree structure

		This function removes an object identified by a pointer from the octree data structure. It iterates through all the node indices stored in the object's handle and removes the object from the
	   corresponding octree nodes. The function updates the node's link list by swapping the element to be removed with the last element and reducing the count. It also adjusts the node's small object
	   counter if necessary. Finally, it clears the object's handle indices and bounds.

		\param ptr Pointer identifying the object to be removed from the octree
	*/
	void Remove( Pointer ptr );

	/*!
		\brief Updates the position and bounds of an object in the octree

		This function updates the location of an object within the octree structure. It first checks if the object's bounds have changed, and if so, determines whether a fast update is possible by
	   comparing cell ranges. If the object can remain in the same nodes, it performs a fast update by modifying the bounds in place. Otherwise, it removes the object and re-adds it to the octree. The
	   function is optimized to avoid unnecessary operations when the object's position hasn't significantly changed.

		\param ptr Pointer to the object being updated
		\param box New bounds of the object
	*/
	void Update( Pointer ptr, const idBounds& box );

	/*!
		\brief Finds objects with bounding boxes intersecting the specified box and returns a list of chunks that may contain such objects

		This function performs an octree query to identify all objects whose bounding boxes intersect with the provided box parameter. It clears the result list before populating it with the matching
	   chunks. The function uses a recursive helper method to traverse the octree structure and collect potential matches. The results are stored in the provided QueryResult object which is cleared at
	   the beginning of the operation.

		\param box The bounding box to test for intersections with object bounding boxes
		\param res Reference to the QueryResult object where matching chunks will be stored
	*/
	void QueryInBox( const idBounds& box, QueryResult& res ) const;

	/*!
		\brief Finds objects whose bounding boxes intersect with a specified moving box and returns a list of chunks that may contain such objects.

		This function performs a query operation on an octree structure to identify all objects that might intersect with a moving box defined by its starting position, velocity, and radius. It uses a
	   recursive approach to traverse the octree, checking for intersections with the moving box. The results are stored in the provided QueryResult object, which is cleared before the query begins.

		\param box The initial bounding box of the moving object
		\param start The starting position of the moving box
		\param invDir The inverse direction of movement for the box
		\param radius The radius of the moving box in each dimension
		\param res The result object to store the query results
	*/
	void QueryInMovingBox( const idBounds& box, const idVec3& start, const idVec3& invDir, const idVec3& radius, QueryResult& res ) const;

private:
	struct QueryContext;
	struct AddContext;
	struct CellRanges;

	/*!
		\brief Determines the octree level for a given bounding box based on its size relative to the world size

		This function calculates the appropriate octree level for a bounding box by computing the ratio of the box size to the world size, then determining the logarithm base 2 of the maximum ratio.
	   The result is adjusted to ensure a minimum level of 0, which is useful for spatial partitioning in octree data structures. The function is typically used to determine the appropriate level in
	   an octree hierarchy for spatial queries or object placement.

		\param box The bounding box for which to determine the octree level
		\return The calculated octree level, which represents the hierarchy level in the octree structure where the box should be placed
	*/
	int		   GetLevel( const idBounds& box ) const;

	/*!
		\brief Computes the cell ranges within the octree that intersect with the given bounding box at the specified maximum depth.

		The function calculates the minimum and maximum cell coordinates in each dimension that intersect with the provided bounding box. It first transforms the box coordinates into normalized octree
	   space, clamps them to the valid range [0, 1], and then scales them to the cell coordinate system based on the maximum depth. The resulting ranges are returned as a CellRanges structure.

		\param box The bounding box to compute cell ranges for
		\param maxDepth The maximum depth level to compute cell ranges at
		\return CellRanges structure containing the minimum and maximum cell coordinates in each dimension that intersect with the input box
	*/
	CellRanges GetCellRanges( const idBounds& box, int maxDepth ) const;

	/*!
		\brief Performs a recursive query on the octree to find all objects that intersect with a given bounding box.

		This function recursively traverses the octree structure to identify all objects that intersect with the provided bounding box. It handles both static and moving object queries by checking
	   intersection with the space box and considering the movement parameters. The function accumulates the results in the provided query context.

		\param ctx The query context containing the search box, movement parameters, and result container.
		\param nodeIdx The index of the current node in the octree to process.
		\param cellBox The bounding box representing the current cell in the octree.
		\param spaceBox The bounding box representing the current space in the octree.
		\throws Assertion failure if the space box does not intersect with the query box.
	*/
	void	   Query_r( QueryContext& ctx, int nodeIdx, const idBounds& cellBox, const idBounds& spaceBox ) const;

	/*!
		\brief Recursively adds an object to the octree structure by determining the appropriate node and splitting the tree when necessary.

		This function recursively inserts an object into the octree by first checking if the current node is a leaf or if the object is too large to go deeper. If not, it splits the cell into eight
	   subcells and processes each subcell recursively. The function uses masks to efficiently determine which subcells overlap with the object's bounding box. It handles chunk allocation and
	   potential tree splitting when the object count exceeds a threshold.

		\param ctx Context containing the object pointer and its bounding box
		\param nodeIdx Index of the node to add the object to
		\param cellBox Bounding box representing the cell size for octree subdivision
		\throws assertion failure if the node's firstSonIdx is not properly initialized
	*/
	void	   Add_r( AddContext& ctx, int nodeIdx, const idBounds& cellBox );

	/*!
		\brief Adds an object to a specified node in the octree, handling chunk allocation and potential tree splitting when too many small objects are present.

		This function manages the insertion of an object into a specific node of the octree structure. It first attempts to find an existing chunk within the node that has available space. If no such
	   chunk exists, it allocates a new one. The object is then added to the chunk, and the node's small object count is updated. If the number of small objects in the node exceeds a threshold, the
	   octree structure is split to accommodate the increasing complexity of object distribution. During splitting, existing objects are reinserted into the appropriate child nodes.

		\param ctx Context containing the object pointer and its bounding box
		\param nodeIdx Index of the node to add the object to
		\param cellBox Bounding box representing the cell size for octree subdivision
		\throws assertion failure if the node's firstSonIdx is not properly initialized
	*/
	void	   AddToNode( AddContext& ctx, int nodeIdx, const idBounds& cellBox );

	/*!
		\struct idBoxOctree::OctreeNode
		\brief Octree node structure for spatial partitioning in the Roguestrad engine.

		The OctreeNode structure serves as a fundamental component for building octree data structures used for efficient spatial partitioning and collision detection within the Roguestrad engine. It
	   provides the hierarchical organization necessary for managing spatial queries and optimizing rendering operations in 3D environments. This implementation is designed to support the engine's
	   need for fast spatial lookups and culling operations. The node structure is intended to be used internally by the octree system and does not expose ownership or memory management details
	   directly.

	*/
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
