<!-- archgen: sha256=c2d29ff30541f6355cc3957bdee1e65a62ef3a9d913fc8d015d75c5db14ddca5 -->

# idlib/bv/BoxOctree.cpp

## File Purpose
- Implements a spatial partitioning octree specifically optimized for axis-aligned bounding boxes (AABB).
- Provides mechanisms for high-performance spatial queries, including static box intersections and swept-volume (moving box) intersections.
- Handles dynamic object management, including efficient insertion, removal, and localized updates of moving entities.

## Core Responsibilities
- Maintaining a hierarchical 3D grid of nodes (octree) to accelerate spatial lookups.
- Managing object lifecycle within the tree, ensuring that movement or deletion is reflected across all inhabited nodes.
- Automating tree subdivision (splitting) when the density of small objects in a specific node exceeds a predefined threshold.
- Optimizing memory layout through chunk-based storage and periodic condensation of fragmented chunks.
- Providing optimized 'fast-path' updates for objects whose movement does not cause them to exit their current spatial cells.

## Key Types and Functions
- idBoxOctree — The primary class managing the spatial octree structure and its associated nodes.
- idBoxOctree::Add(Pointer ptr, const idBounds& box) — Inserts an object into the hierarchy at the appropriate depth based on its size.
- idBoxOctree::Remove(Pointer ptr) — Removes an object from the tree by leveraging its handle to locate all occupied nodes.
- idBoxOctree::Update(Pointer ptr, const idBounds& box) — Updates an object's spatial presence, using a fast-path if the object remains within the same cell ranges.
- idBoxOctree::QueryInBox(const idBounds& box, QueryResult& res) — Performs a recursive search to find all object chunks intersecting a static volume.
- idBoxOctree::QueryInMovingBox(...) — Executes a swept-volume query to detect intersections between a moving AABB and the tree contents.
- id1BoxOctree::Condense() — Reorganizes the internal linked list of chunks into a more contiguous and compact format to improve cache locality.
- idBoxOctree::GetLevel(const idBounds& box) — Calculates the hierarchical depth an object should reside in based on its dimensions relative to the world bounds.
- idBoxOctree::Chunk — A memory-managed structure holding a fixed-size array of object links to reduce allocation overhead.
- idBoxOctree::OctreeNode — Represents a specific volume in the octree, potentially containing child nodes and links to object chunks.

## Important Control Flow
- Insertion Flow: `Add` initiates a recursive traversal (`Add_r`). If an object is too large or the node is a leaf, it is added to a `Chunk` via `AddToNode`. If a node's small-object count exceeds `SPLIT_SMALL_NUM`, the node is subdivided into eight children, and existing objects are redistributed.
- Query Flow: `QueryInBox` or `QueryInMovingBox` triggers a recursive traversal (`Query_r`). The algorithm uses bitmasks to determine which of the eight potential child sub-volumes intersect the query volume, traversing only intersecting branches.
- Update Flow: `Update` first checks if the object's new bounds are identical to its old bounds. If changed, it compares the `CellRanges` (the integer grid coordinates). If the ranges are identical, it performs an in-place update; otherwise, it performs a full `Remove` and `Add` sequence.
- Removal Flow: `Remove` uses the object's `idBoxOctreeHandle` to iterate directly through the list of `nodeIdx` values the object inhabits, searching the chunks in those nodes to extract the pointer.

## External Dependencies
- "BoxOctree.h" — The header containing the class definition and internal structures.
- "../math/Vector.h" — For `idVec3` operations and geometric math.
- "precompiled.h" — For standard engine-wide definitions and includes.
- Implication of usage of `idBounds`, `idList`, and `idMath` from the broader `idlib` framework.

## How It Fits
- Acts as the primary acceleration structure for collision detection, visibility culling, and proximity-based gameplay logic.
- Integrates with the engine's `handle` system to allow $O(1)$ access to an object's spatial metadata during removal and updates.
- Serves as a low-level primitive used by higher-level systems (like physics or AI) to prune the search space for potential interactions.
