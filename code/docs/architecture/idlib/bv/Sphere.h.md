<!-- archgen: sha256=09254cdac7762dcfe99a7e8f43aefe23e7cf2ea052bd07d474c94e66b8604ecc -->

# idlib/bv/Sphere.h

## File Purpose
- Defines the `idSphere` class, a basic geometric primitive used for bounding volumes.
- Provides functionality for spherical collision detection, spatial queries, and bounding sphere construction.

## Core Responsibilities
- Representing a 3D sphere via an origin (`idVec3`) and a radius (`float`).
- Performing intersection tests between spheres, lines, and rays.
- Determining spatial relationships between spheres and planes.
- Managing sphere expansion logic when adding points or other spheres to a bounding volume.
- Constructing spheres from complex geometric transformations (rotation, translation, etc.).

## Key Types and Functions
- idSphere — Represents a 3D bounding sphere defined by a center and radius.
- idSphere::AddPoint(const idVec3& p) — Expands the sphere to encompass a new point, recalculating the center and radius if necessary.
- idSphere::AddSphere(const idSphere& s) — Expands the sphere to encompass another sphere.
- idSphere::IntersectsSphere(const idSphere& s) — Performs an efficient overlap test between two spheres using squared distance.
- idSphere::PlaneSide(const idPlane& plane, const float epsilon) — Returns whether a sphere is in front of, behind, or intersecting a plane.
- idSphere::RayIntersection(const idVec3& start, const idVec3& dir, float& scale1, float& scale2) — Calculates the entry and exit parameters of a ray passing through the sphere.
- idSphere::FromPoints(const idVec3* points, const int numPoints) — Computes the minimal bounding sphere for a provided array of 3D points.
- idSphere::Clear() — Sets the sphere to an 'inside out' or invalid state by setting radius to -1.0f.

## Important Control Flow
- The sphere maintains an 'invalid' state when `radius < 0.0f` (triggered by `Clear()`), which forces subsequent `AddPoint` or `AddSphere` calls to initialize the sphere's origin based on the input.
- Intersection logic prioritizes performance by utilizing squared distances to avoid expensive square root operations.
- The `AddPoint` and `AddSphere` algorithms use weighted averages and geometric midpoints to expand the bounding volume while attempting to keep the sphere as tight as possible.
- Transformation methods like `Translate` and `Expand` follow a functional pattern, returning new objects, while `TranslateSelf` and `ExpandSelf` provide in-place modification for efficiency.

## External Dependencies
- idVec3 — For 3D vector arithmetic and coordinate representation.
- idPlane — For sphere-plane intersection and distance queries.
- idRotation — For computing spheres resulting from rotational transformations.
- idMath — For mathematical utilities such as `Sqrt` and `Fabs`.
- ON_EPSILON — For floating-point tolerance in spatial comparisons.

## How It Fits
- Part of the `idlib/bv` (Bounding Volume) system.
- Acts as a primary primitive for the engine's collision detection pipeline.
- Used by spatial partitioning structures (e.g., Octrees) to perform rapid rejection tests for visibility and physics queries.
