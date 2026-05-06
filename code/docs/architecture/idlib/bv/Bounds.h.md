<!-- doxygenix: xxh128=0533c2fe381e82544b77191efae1fd2f -->

# idlib/bv/Bounds.h

## File Purpose
- Declares the `idBounds` class representing a 3D axis-aligned bounding box (AABB) with inline math operations.
- Provides optimized constructors, arithmetic operators, spatial query methods, and bounding volume transformation utilities.
- Exposes global sentinel instances (`bounds_zero`, `bounds_zeroOneCube`, `bounds_unitCube`) for common initialization defaults.

## Core Responsibilities
- Encapsulate AABB min/max coordinates with high-performance inline operations for real-time spatial reasoning.
- Provide both mutation-free (`const`) and in-place variants for expansion, translation, rotation, and intersection calculations.
- Execute precise overlap and distance tests against points, rays, lines, planes, and other bounding volumes.
- Enable geometric property extraction (center, volume, radius, projections) and conversion to/from complementary primitives like spheres and point clouds.

## Key Types and Functions
- `idBounds` - Primary AABB representation storing two `idVec3` components (`b[0]` for mins, `b[1]` for maxs) with heavy inline optimization for cache locality.
- `idBounds::AddPoint(const idVec3&)` - Expands the current bounds to tightly enclose an additional vertex, returning a boolean flag indicating if expansion occurred.
- `idBounds::IntersectsBounds(const idBounds&)` - Performs a fast six-condition slab test to determine boolean overlap with another AABB.
- `idBounds::RayIntersection(const idVec3&, const idVec3&, float&)` - Executes an optimized ray-AABB intersection test, outputting the hit distance scale for early-out culling.
- `idBounds::FromTransformedBounds(...)` - Computes the tightest axis-aligned bounding box enclosing a transformed (rotated/translated) bounding volume.
- `idBounds::AxisProjection(...)` - Calculates the scalar projection interval of the bounds along an arbitrary direction vector, supporting swept collision and frustum tests.
- `bounds_zero`, `bounds_zeroOneCube`, `bounds_unitCube` - Predefined global constants representing degenerate, zero-to-one range, and unit cube defaults used throughout engine initialization.

## Important Control Flow
- Data flows primarily through the `b[0]` and `b[1]` member array, updated via in-place compound operators or returned as new instances through query methods to preserve functional safety in hot loops.
- Spatial queries read min/max components directly, executing early-exit slab tests and branch-predictable comparisons optimized for SIMD-friendly access patterns and cache coherence.
- Transformation methods reconstruct bounds by evaluating corner extremes or radius-based expansion, ensuring the result remains strictly axis-aligned after arbitrary rotational matrices are applied.
- Global constants are typically passed by reference or value during editor and runtime initialization to avoid heap allocation in performance-critical rendering and physics pipelines.

## External Dependencies
- `idlib/math/Vec.h` (`idVec3`) - Required for 3D coordinate storage and component-wise arithmetic.
- `idlib/math/Mat.h` (`idMat3`) - Required for rotation matrix multiplication and axis vector extraction.
- `idlib/math/Plane.h` (`idPlane`) - Required for plane-side evaluation and signed distance calculations.
- `idlib/math/Sphere.h` (`idSphere`) - Required for AABB-to-sphere conversion via `ToSphere()`.
- `idlib/math/Rotation.h` (`idRotation`) - Required for sweep bounding calculations in rotation methods.
- `idlib/math/Math.h` (`idMath`, `INFINITUM`) - Required for infinity sentinels, absolute value, and floating-point tolerance constants.
- TODO: clarify exact precompiled header paths providing `vec3_origin` and `ON_EPSILON` macros, likely defined in engine-wide math configuration.

## How It Fits
- Serves as the foundational bounding volume primitive for view frustum culling, portal rendering, and spatial partitioning algorithms in the id Tech derivative.
- Interfaces directly with physics and collision subsystems by providing O(1) overlap tests against rays and lines, acting as a primary broadphase filter before expensive triangle mesh intersection. 
- Integrates with `idLib`'s math namespace to enable generic algorithm support for spatial hierarchies (BVH, octrees, BSPs), dynamic shadow casting bounds, and LOD scheduling thresholds.
- Acts as the canonical configuration and query target for mesh collision hull construction, editor viewport management, and network entity culling synchronization.
