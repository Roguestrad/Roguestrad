<!-- doxygenix: xxh128=8faa682d028ed9ee8a0b43e085a52d78 -->

# idlib/bv/Bounds.cpp

## File Purpose
- Declares global static idBounds constants for degenerate and unit cubes.
- Implements the core geometric operations for the idBounds axis-aligned bounding box (AABB) class.

## Core Responsibilities
- Execute intersection and distance tests between AABBs and planes, line segments, and rays.
- Construct and transform bounding boxes under translation, affine rotation, and sweep operations.
- Balance computational precision and performance by switching to conservative spherical bounds for large rotation angles.
- Convert between compact min/max vertex storage and expanded corner point arrays.

## Key Types and Functions
- `bounds_zero`, `bounds_zeroOneCube`, `bounds_unitCube` - Global constant AABB instances representing degenerate, standard unit, and symmetric unit cubes.
- `idBounds::GetRadius() const` - Computes the Euclidean distance from the world origin to the bounds' furthest corner by projecting max extents onto each axis.
- `idBounds::GetRadius(const idVec3& center) const` - Computes the maximum distance from an arbitrary center point to any corner of the bounds.
- `idBounds::PlaneDistance(const idPlane& plane) const` - Calculates the signed distance to the closest point on the box, returning zero if the plane intersects the volume.
- `idBounds::PlaneSide(const idPlane& plane, const float epsilon) const` - Classifies the box as entirely front, entirely back, or crossing a plane using projected extents and a floating-point tolerance.
- `idBounds::LineIntersection(const idVec3& start, const idVec3& end) const` - Tests segment intersection against the box using axis-aligned separation checks and cross-product magnitude bounds.
- `idBounds::RayIntersection(const idVec3& start, const idVec3& dir, float& scale) const` - Determines if a ray strikes the box, outputting the hit parameter and handling interior start points via axis traversal logic.
- `idBounds::FromTransformedBounds(const idBounds& bounds, const idVec3& origin, const idMat3& axis)` - Computes the tightest AABB enclosing a box transformed by a rotation matrix and translation vector.
- `idBounds::FromPoints(const idVec3* points, const int numPoints)` - Constructs an AABB by computing component-wise minima and maxima across a point cloud via hardware-accelerated SIMD.
- `idBounds::FromPointTranslation(const idVec3& point, const idVec3& translation)` - Builds a volumetric bounds from a single point and its displacement vector along each axis.
- `BoundsForPointRotation(const idVec3& start, const idRotation& rotation)` - Free function calculating the tight AABB swept out by a point rotating around a specified axis over a given angle.
- `idBounds::FromBoundsRotation(const idBounds& bounds, const idVec3& origin, const idMat3& axis, const idRotation& rotation)` - Constructs bounds for a rotating AABB by sweeping its corners or using a conservative radius for large angles.
- `idBounds::ToPoints(idVec3 points[8]) const` - Expands the compact AABB representation into its eight constituent corner vertices using bit-shifting axis indexing.

## Important Control Flow
- Plane and ray intersection routines first compute center distances and project box extents onto normals/directions, utilizing early-exit conditions to determine overlap or side classification before performing heavier geometric calculations.
- Transformation constructors evaluate rotation matrix properties and angles to branch between precise corner-expansion algorithms for small rotations and fast spherical radius fallbacks for large angles, preventing excessive computational overhead.
- Min/max construction delegates component-wise extrema finding to SIMDProcessor->MinMax, leveraging hardware vectorization to populate the b[0] and b[1] members efficiently.
- Data flows from raw point arrays, transform parameters, or query rays into compact low/high coordinate storage, and can be expanded back to vertex arrays or evaluated for spatial tests during rendering, physics, or navigation updates.

## External Dependencies
- idlib/precompiled.h - Primary build configuration and macro definitions.
- idlib/bv/BV.h - Declares idBounds, idVec3, idPlane, idMat3, idRotation, and plane side enums.
- idlib/math/idMath.h - Provides idMath::Fabs, idMath::Sqrt, and fundamental floating-point utilities.
- SIMDProcessor abstraction - Exposes MinMax for vectorized bounding box construction.

## How It Fits
- Serves as the foundational AABB implementation within the idlib::bv (bounding volume) subsystem, supplying the primary spatial pruning primitive for the Doom 3 BFG engine fork.
- Integral to frustum culling, collision detection broad-phase queries, navigation mesh generation, and physics partitioning by providing highly optimized overlap and containment tests.
- Bridges low-level SIMD math capabilities with high-level spatial algorithms, ensuring that geometric queries remain performant across both static world geometry and dynamic entity updates.
