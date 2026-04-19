<!-- archgen: sha256=45f663ff2a3adafc48186fae4dec4a226117e3cbd4b84508db4412516bd4ee63 -->

# idlib/bv/BoxOctree.h

## File Purpose
- Defines a spatial partitioning structure (an Octree) designed for efficient intersection queries of axis-aligned bounding boxes (AABB).
- Provides a mechanism for tracking objects within the tree via handles to allow efficient updates and removals.

## Core Responsibilities
- Spatial indexing of arbitrary pointers (payloads) associated with Bounding Boxes.
- Hierarchical subdivision of 3D space into octants based on object density.
- Efficiently managing memory for spatial data using chunked arrays and block allocation.
- Performing spatial queries including static box intersections and swept-volume (moving box) intersections.
- Optimizing object updates by differentiating between 'fast-path' (bounds change only) and 'slow-path' (re-insertion) logic.

## Key Types and Functions
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

## Important Control Flow
- Insertion Flow: `Add` is called $\rightarrow$ `Add_r` traverses the tree $\rightarrow$ If the object's size is relatively small and the current node's `numSmall` exceeds `SPLIT_SMALL_NUM`, the node splits into 8 children $\rightarrow$ Objects are sifted down to the appropriate level.
- Update Flow: `Update` checks if the new `idBounds` still fit within the existing node assignments via the `idBoxOctreeHandle` $\rightarrow$ If valid, it updates the bounds in place $\rightarrow$ If the object has moved out of its current nodes, it triggers `Remove` followed by `Add`.
- Query Flow: `QueryInBox` or `QueryInMovingBox` initiates `Query_r` $\rightarrow$ The algorithm recursively traverses nodes, pruning branches where the node's `cellBox` does not intersect the query volume $\rightarrow$ Intersecting chunks are appended to the `QueryResult`.

## External Dependencies
- ../math/Line.h — Provides the `idBounds` and `idVec3` structures used for geometric calculations.
- ../containers/FlexList.h — Provides the `idFlexList` container used for handles and query results.
- idBlockAlloc (implied) — Used for high-performance, block-based allocation of `Chunk` structures.

## How It Fits
- Serves as the primary spatial acceleration structure for the engine's collision and visibility systems.
- Specifically utilized by `idClip` to store and query clipmodels for all entities in the game world.
- Provides the underlying geometric query interface used by higher-level physics and rendering subsystems to prune unnecessary intersection tests.
