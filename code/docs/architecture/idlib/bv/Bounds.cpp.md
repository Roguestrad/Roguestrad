<!-- archgen: sha256=d584aa88578173c4716764a5dfa20d9cfe8844abd75dfa064bbfdccb0ac4ae31 -->

# idlib/bv/Bounds.cpp

## File Purpose
- Implements the `idBounds` class, representing Axis-Aligned Bounding Boxes (AABB).
- Provides geometric primitives and algorithms for intersection testing, spatial queries, and bounding volume transformations.

## Core Responsibilities
- Calculating bounding volumes for points, segments, and rotational paths.
- Performing intersection tests between AABBs and rays, line segments, and planes.
- Computing new AABBs after applying transformations (rotation, translation, and scaling).
- Classifying spatial relationships between AABBs and planes (front, back, or intersecting).
- Providing optimized SIMD-based construction of AABBs from vertex arrays.

## Key Types and Functions
- idBounds — The core class representing an Axis-Aligned Bounding Box (AABB) defined by its minimum and maximum extents.
- bounds_zero — A global constant representing a zero-volume bounding box at the origin.
- bounds_unitCube — A global constant representing an AABB with extents from -1 to 1 on all axes.
- idBounds::GetRadius() — Calculates the radius of the AABB relative to its own center.
- idBounds::GetRadius(const idVec3& center) — Calculates the maximum distance from an arbitrary point to the AABB's boundaries.
- idBounds::PlaneDistance(const idPlane& plane) — Computes the signed minimum distance from the AABB to a specified plane.
- idBounds::PlaneSide(const idPlane& plane, const float epsilon) — Determines if the AABB is in front of, behind, or spanning a plane.
- idBounds::LineIntersection(const idVec3& start, const idVec3& end) — Performs a Separating Axis Theorem (SAT) based intersection test with a line segment.
- idBounds::RayIntersection(const idVec3& start, const idVec3& dir, float& scale) — Executes a ray-AABB intersection test, returning the distance to the entry point.
- idBounds::FromPoints(const idVec3* points, const int numPoints) — Constructs an AABB enclosing a point cloud using SIMD-optimized min/max operations.
- idBounds::FromTransformedBounds(const idBounds& bounds, const idVec3& origin, const idMat3& axis) — Recomputes an AABB that encapsulates a transformed AABB.
- BoundsForPointRotation(const idVec3& start, const idRotation& rotation) — Computes the AABB that encloses the arc swept by a point during rotation.
- idBounds::ToPoints(idVec3 points[8]) — Extracts the eight vertices of the AABB into a provided array.

## Important Control Flow
- Intersection tests (Ray/Line) utilize conditional logic to evaluate axis-aligned extents and separation planes.
- Transformation logic branches based on whether a rotation matrix is identity or contains rotation (e.g., `IsRotated()`) to choose between direct translation or complex projection.
- Point-set construction (`FromPoints`) delegates heavy computation to a `SIMDProcessor` to process multiple axes in parallel.
- Rotational bounding logic (`FromPointRotation`) branches based on the rotation angle (threshold of 180 degrees) to switch between exact arc calculation and a conservative spherical approximation.

## External Dependencies
- "precompiled.h" — Provides foundational engine types including `idVec3`, `idMat3`, `idPlane`, `idRotation`, and `idMath`.
- SIMDProcessor — Used for high-performance extremum searches in point arrays.
- idMath — Essential for absolute values, square roots, and floating-point comparisons.

## How It Fits
- Functions as a fundamental primitive within the `idlib/bv` (Bounding Volume) system.
- Serves as the primary spatial primitive for visibility culling, collision detection, and physics queries.
- Provides the mathematical building blocks for more complex hierarchical bounding volumes (e.g., BVH or Octrees) throughout the engine.
