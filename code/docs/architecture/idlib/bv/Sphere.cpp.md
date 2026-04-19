<!-- archgen: sha256=33decc717868dc39e74f4b88b9a53b8b3913d6b347c2fac341851ec98f771ad2 -->

# idlib/bv/Sphere.cpp

## File Purpose
- Provides the implementation for the `idSphere` class, a fundamental geometric bounding volume.
- Implements various intersection tests and spatial relationship queries involving spheres, planes, lines, and rays.

## Core Responsibilities
- Calculating signed distances from planes to sphere boundaries.
- Determining the spatial orientation (front, back, or crossing) of a sphere relative to a plane.
- Detecting intersections between line segments and the sphere volume.
- Calculating intersection parameters for ray-sphere intersection tests.
- Generating a bounding sphere from a collection of 3D points using an AABB-based center approach.

## Key Types and Functions
- idSphere::PlaneDistance — Calculates the signed distance from a plane to the sphere's outer edge, accounting for the radius.
- idSphere::PlaneSide — Evaluates if a sphere is entirely in front of, entirely behind, or intersecting a plane, using an epsilon for floating-point stability.
- idSphere::LineIntersection — Determines if a finite line segment intersects the volume of the sphere.
- idSphere::RayIntersection — Solves the quadratic equation for ray-sphere intersection, returning the entry and exit distances (scales) along the ray.
- idSphere::FromPoints — Constructs a bounding sphere by finding the center of the points' AABB and calculating the maximum distance to any point in the set.

## Important Control Flow
- For plane queries, the logic computes the distance from the sphere's origin to the plane and then offsets that distance by the radius to find the gap or penetration depth.
- The line intersection algorithm projects the sphere's origin onto the line segment, checking the closest point (start, end, or internal) against the squared radius.
- The ray intersection logic uses the quadratic formula (discriminant-based) to find intersection points, returning false if the ray misses the sphere.
- The `FromPoints` routine utilizes `SIMDProcessor` for an efficient axis-aligned bounding box (AABB) calculation, followed by a single-pass radial distance check.

## External Dependencies
- idVec3 — 3D vector math primitives.
- idPlane — Plane primitive for spatial queries.
- idMath — Mathematical utility functions (e.g., Sqrt).
- SIMDProcessor — Accelerated SIMD instructions for min/max point operations.
- precompiled.h — Engine-wide precompiled header for core types.

## How It Fits
- Acts as a low-level geometric primitive within the `idlib/bv` (Bounding Volume) system.
- Serves as a building block for more complex collision detection algorithms and spatial partitioning structures within the engine.
- Provides the mathematical foundation for visibility and collision queries involving spherical volumes.
