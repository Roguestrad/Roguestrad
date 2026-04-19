<!-- archgen: sha256=b38fd030b0eccae2488612621de9a621afcdc7329928461f073f5ad3a1940082 -->

# idlib/bv/Box.cpp

## File Purpose
- Implements the logic for the `idBox` class, representing an Oriented Bounding Box (OBB).
- Provides optimized algorithms for OBB construction, intersection testing, and silhouette extraction.

## Core Responsibilities
- Calculating minimal volume OBBs from point clouds using Eigen decomposition.
- Performing geometric intersection tests (Box-Box via SAT, Box-Line, Box-Ray).
- Computing spatial relationships between boxes and planes.
- Extracting silhouette vertices for shadow/projection calculations using precomputed lookup tables.
- Managing box expansion and union operations while minimizing volume.

## Key Types and Functions
- idBox::AddPoint(const idVec3& v) — Updates the box to include a point, potentially reorienting the box axes to minimize volume.
- idBox::AddBox(const idBox& a) — Computes the minimal volume OBB that encloses both the current box and another box by testing multiple optimal orientations.
- idBox::PlaneDistance(const idPlane& plane) — Calculates the signed distance from the box to a plane.
- idBox::PlaneSide(const idPlane& plane, const float epsilon) — Determines if the box is in front of, behind, or intersecting a plane.
- idBox::IntersectsBox(const idBox& a) — Implements the Separating Axis Theorem (SAT) to detect intersection between two OBBs.
- idBox::LineIntersection(const idVec3& start, const idVec3& end) — Determines if a line segment intersects the OBB using SAT.
- idBox::RayIntersection(const idVec3& start, const idVec3& dir, float& scale1, float& scale2) — Calculates the entry and exit distances of a ray through the box.
- idBox::FromPoints(const idVec3* points, const int numPoints) — Generates an OBB from a point cloud by computing a covariance matrix and extracting eigenvectors/eigenvalues.
- idBox::ToPoints(idVec3 points[8]) — Computes the world-space coordinates of the eight corner vertices of the box.
- idBox::GetProjectionSilhouetteVerts(const idVec3& projectionOrigin, idVec3 silVerts[6]) — Identifies vertices forming the silhouette from a perspective viewpoint using a bitmask-based lookup table.
- idBox::GetParallelProjectionSilhouetteVerts(const idVec3& projectionDir, idVec3 silVerts[6]) — Identifies vertices forming the silhouette under parallel projection using a bitmask-based lookup table.

## Important Control Flow
- Construction (FromPoints): Point Cloud -> Mean Calculation -> Covariance Matrix -> Eigenvalue/Eigenvector Solver -> Axis/Extent Assignment.
- Intersection (SAT): Project centers onto separating axes (box axes and cross-product axes) -> Compare projected extents -> Return false if any axis proves separation.
- Raycasting: Transform ray to local OBB space -> Perform slab-based clipping against the six planes of the AABB in local space.
- Silhouette Extraction: Project vertices -> Generate a bitmask based on the sign of the projection direction relative to box axes -> Retrieve vertex indices from the `boxPlaneBitsSilVerts` lookup table.

## External Dependencies
- idlib/math/idMath.h (for absolute values and infinity)
- idlib/math/idVec3.h (for vector operations)
- idlib/math/idMat3.h (for rotation/basis transformation)
- idlib/math/idMatX.h (for Eigen decomposition)
- idlib/bv/idBounds.h (used during box expansion/union)
- idlib/bv/idPlane.h (used for distance/side queries)

## How It Fits
- Acts as a fundamental primitive within the Bounding Volume (BV) hierarchy for collision detection and culling.
- Provides the geometric foundation for shadow volume generation (via silhouette extraction) and visibility tests.
- Serves as a high-performance utility for spatial partitioning and object bounds management in the engine.
