<!-- archgen: sha256=62d897ac2fb3af18791399040c9ae6ffb73fff0ecdf9ad64c15f6e9eac056e7d -->

# idlib/bv/Box.h

## File Purpose
- Defines the `idBox` class, representing an Oriented B/ounding Box (OBB).
- Provides mathematical primitives for volumetric 3D spatial calculations involving oriented volumes.

## Core Responsibilities
- Representing a 3D volume defined by a center point, extents, and an orientation basis (axis).
- Performing intersection tests between OBBs, rays, lines, and planes.
- Executing geometric transformations including translation, rotation, and uniform expansion.
- Calculating tight-fitting bounding volumes from point clouds using principal component analysis (PCA) logic.
- Generating silhouette vertices for projection-based visibility or rendering logic.

## Key Types and Functions
- idBox — An oriented bounding box class defined by a center, extents, and an orientation matrix.
- idBox::AddBox(const idBox& a) — Computes the minimal volume bounding box that encompasses both this box and another.
- idBox::ContainsPoint(const idVec3& p) — Validates if a point resides within the box boundaries via local-space projection.
- idBox::IntersectsBox(const idBox& a) — Implements the Separating Axis Theorem (SAT) to detect overlap between two oriented boxes.
- idBox::RayIntersection(const idVec3& start, const idVec3& dir, float& scale1, float& scale2) — Calculates the entry and exit distances of a ray through the OBB.
- idBox::FromPoints(const idVec3* points, const int numPoints) — Derives a tight-fitting OBB from a set of points using covariance and eigenvectors.
- idBox::PlaneSide(const idPlane& plane, const float epsilon) — Determines the spatial relationship (front, back, or intersecting) of the box relative to a plane.
- idBox::AxisProjection(const idVec3& dir, float& min, float& max) — Computes the interval of the box's projection onto a specific directional vector.
- idBox::GetProjectionSilhouetteVerts(const idVec3& projectionOrigin, idVec3 silVerts[6]) — Calculates the silhouette edges of the box relative to a projection origin.

## Important Control Flow
- Transformation Flow: Operations like `RotateSelf` or `TranslateSelf` mutate the internal `center` and `axis` members, while `Rotate` or `Translate` return new `idBox` instances.
- Collision Detection Flow: Intersection tests (e.g., `IntersectsBox`) utilize the Separating Axis Theorem by iterating through potential separating axes derived from the box basis vectors and the vector connecting box centers.
- Raycasting Flow: For `RayIntersection`, the ray is transformed into the box's local coordinate space to simplify the intersection check against the six axis-aligned faces.
- Expansion Flow: Functions like `AddPoint` or `AddBox` implement logic to check if the incoming geometry is already contained; if not, they recalculate the center and extents to encapsulate the new data.

## External Dependencies
- idVec3 (math primitives)
- idMat3 (rotation and orientation basis)
- idBounds (axis-aligned bounding box structures)
- idPlane (plane-based collision primitives)
- idSphere (bounding sphere primitives)
- idMath (mathematical constants and utilities like INFINITUM)

## How It Fits
- Acts as a high-precision bounding volume primitive within the engine's Bounding Volume (BV) hierarchy.
- Provides the mathematical foundation for advanced collision detection between oriented game objects and static geometry.
- Supports visibility and culling systems through silhouette generation and axis projection logic.
- Functions as a more complex alternative to `idBounds` (AABB) when object rotation must be tightly encapsulated.
