<!-- archgen: sha256=59d7e385174605c7bca742be12533eec379f29beb025f22f9cb95e0bc91f9710 -->

# idlib/bv/Bounds.h

## File Purpose
- Declare and inline‑implement the idBounds class, the engine’s fundamental tool for spatial extents.
- Provide a header‑only implementation to avoid multiple definition issues and to enable inlining for performance-critical paths.

## Core Responsibilities
- Represent a 3‑D axis‑aligned bounding box with minimum and maximum corner vectors.
- Provide constructors for emptiness, minima/maxima, single point, and inline zero/clear init.
- Implement arithmetic operations: union, subtraction of a constant box, translation, and rotation.
- Offer geometric queries: center, volume, radius, size, containment, intersection, and sphere conversion.
- Expose low‑level helpers for transforming bounds, computing axis projections, and generating corner points.
- Handle epsilon‑based comparison and clear/backward state checks.

## Key Types and Functions
- idBounds — represents an axis‑aligned bounding box with min/max corners.
- idBounds::idBounds() — default constructor (empty box).
- idBounds::idBounds(const idVec3& mins, const idVec3& maxs) — initializes with explicit bounds.
- idBounds::idBounds(const idVec3& point) — creates a zero‑volume box at a point.
- idBounds::operator[](int) const/— — access min/max corners.
- idBounds::operator+(const idVec3&) const — translate the bounds.
- idBounds::operator+=(const idVec3&) — in‑place translation.
- idBounds::operator*(const idMat3&) const — return rotated bounds.
- idBounds::operator*=(const idMat3&) — in‑place rotation.
- idBounds::operator+(const idBounds&) const — union of two bounds.
- idBounds::operator+=(const idBounds&) — in‑place union.
- idBounds::operator-(const idBounds&) const — subtract a constant box.
- idBounds::operator-=(const idBounds&) — in‑place subtraction.
- idBounds::Compare(const idBounds&) const — strict equality test.
- idBounds::Compare(const idBounds&, float epsilon) const — epsilon‑based comparison.
- idBounds::operator==(const idBounds&) const — alias for Compare.
- idBounds::operator!=(const idBounds&) const — negated Compare.
- idBounds::Clear() — set to inside‑out state for incremental accumulation.
- idBounds::Zero() — set all extents to zero.
- idBounds::GetCenter() const — midpoint of min and max.
- idBounds::GetVolume() const — product of side lengths, zero if invalid.
- idBounds::IsCleared() const — true if inside‑out.
- idBounds::AddPoint(const idVec3&) — expand to include a point, return if expanded.
- idBounds::AddBounds(const idBounds&) — expand to also include another bounds, return if expanded.
- idBounds::Intersect(const idBounds&) const — return overlapping region.
- idBounds::IntersectSelf(const idBounds&) — modify this bounds to the overlap.
- idBounds::Expand(float d) const — return bounds expanded by distance.
- idBounds::ExpandSelf(float d) — in‑place expansion.
- idBounds::Translate(const idVec3&) const — return translated bounds.
- idBounds::TranslateSelf(const idVec3&) — in‑place translation.
- idBounds::Rotate(const idMat3&) const — return rotated bounds.
- idBounds::RotateSelf(const idMat3&) — in‑place rotation.
- idBounds::GetSize() const — vector of side lengths.
- idBounds::IsBackwards() const — detect negative extents.
- idBounds::ContainsPoint(const idVec3&) const — point containment test.
- idBounds::IntersectsBounds(const idBounds&) const — overlap test including touch.
- idBounds::LineIntersection(const idVec3&, const idVec3&) const — segment‑box intersection.
- idBounds::RayIntersection(const idVec3&, const idVec3&, float&) const — ray‑box intersection with distance.
- idBounds::FromTransformedBounds(const idBounds&, const idVec3&, const idMat3&) — compute AABB from transformed bounds.
- idBounds::FromPoints(const idVec3*, int) — build minimal box around an array of points.
- idBounds::FromPointTranslation(const idVec3&, const idVec3&) — box of a point’s swept volume.
- idBounds::FromBoundsTranslation(const idBounds&, const idVec3&, const idMat3&, const idVec3&) — apply translation and optional rotation.
- idBounds::FromPointRotation(const idVec3&, const idRotation&) — compute AABB of rotated point.
- idBounds::FromBoundsRotation(const idBounds&, const idVec3&, const idMat3&, const idRotation&) — transform and return bounding box.
- idBounds::ToPoints(idVec3[8]) const — write the 8 corner vertices.
- idBounds::ToSphere() const — return minimal sphere that fully encloses the box.
- idBounds::AxisProjection(const idVec3&, float&, float&) const — projection onto an axis.
- idBounds::AxisProjection(const idVec3&, const idMat3&, const idVec3&, float&, float&) const — projected extent after transformation.
- idBounds::GetDimension() const — always returns 6.
- idBounds::ToFloatPtr() const/— — pointer to underlying float array.

## Important Control Flow
- The idBounds object stores its extents in a fixed two‑element array of idVec3 (`b[2]`).
- Most operations perform per‑component comparisons or arithmetic on these two vectors to update the box.
- Methods that return a new bounds create a temporary idBounds, invoke a helper (e.g. `FromTransformedBounds`) or compute the result directly from the two corner vectors.
- Transform functions (`Rotate`, `RotateSelf`, `FromTransformedBounds`) delegate to a generic routine that applies a matrix and optional translation and then recomputes the min/max corners.
- Intersection and containment checks are implemented with straightforward per‑axis comparisons, producing boolean results or updated bounds.

## External Dependencies
- "idlib/vec3.h" (idVec3 definition)
- "idlib/mat3.h" (idMat3 definition)
- "idlib/plane.h" (idPlane definition)
- "idlib/rotation.h" (idRotation definition)
- "idlib/sphere.h" (idSphere definition)
- "idlib/math.h" (idMath constants and functions)
- "idlib/bv/Bounds.h" depends on the core numeric types and constants: `vec3_origin`, `ON_EPSILON`, `idMath::INFINITUM`.

## How It Fits
- Forms the backbone of bounding‑volume hierarchical structures (BVHs) used for collision detection and visibility.
- Supports frustum culling, ray‑casting, and light‑shadow computations by offering fast intersection tests.
- Enables transformation of objects in the world by integrating rotation and translation into the bounding box.
- Acts as a shared geometric primitive across many subsystems (geometry, physics, AI navigation, rendering).
