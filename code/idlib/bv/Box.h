/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/*!
	\file idlib/bv/Box.h
	\brief Defines the `idBox` class, representing an Oriented B/ounding Box (OBB).
	\note archgen: sha256=62d897ac2fb3af18791399040c9ae6ffb73fff0ecdf9ad64c15f6e9eac056e7d

	\par File Purpose
	- Defines the `idBox` class, representing an Oriented B/ounding Box (OBB).
	- Provides mathematical primitives for volumetric 3D spatial calculations involving oriented volumes.

	\par Core Responsibilities
	- Representing a 3D volume defined by a center point, extents, and an orientation basis (axis).
	- Performing intersection tests between OBBs, rays, lines, and planes.
	- Executing geometric transformations including translation, rotation, and uniform expansion.
	- Calculating tight-fitting bounding volumes from point clouds using principal component analysis (PCA) logic.
	- Generating silhouette vertices for projection-based visibility or rendering logic.

	\par Key Types and Functions
	- idBox — An oriented bounding box class defined by a center, extents, and an orientation matrix.
	- idBox::AddBox(const idBox& a) — Computes the minimal volume bounding box that encompasses both this box and another.
	- idBox::ContainsPoint(const idVec3& p) — Validates if a point resides within the box boundaries via local-space projection.
	- idBox::IntersectsBox(const idBox& a) — Implements the Separating Axis Theorem (SAT) to detect overlap between two oriented boxes.
	- idBox::RayIntersection(const idVec3& start, const idVec3& dir, float& scale1, float& scale2) — Calculates the entry and exit distances of a ray through the OBB.
	- idBox::FromPoints(const idVec3* points, const int numPoints) — Derives a tight-fitting OBB from a set of points using covariance and eigenvectors.
	- idBox::PlaneSide(const idPlane& plane, const float epsilon) — Determines the spatial relationship (front, back, or intersecting) of the box relative to a plane.
	- idBox::AxisProjection(const idVec3& dir, float& min, float& max) — Computes the interval of the box's projection onto a specific directional vector.
	- idBox::GetProjectionSilhouetteVerts(const idVec3& projectionOrigin, idVec3 silVerts[6]) — Calculates the silhouette edges of the box relative to a projection origin.

	\par Control Flow
	- Transformation Flow: Operations like `RotateSelf` or `TranslateSelf` mutate the internal `center` and `axis` members, while `Rotate` or `Translate` return new `idBox` instances.
	- Collision Detection Flow: Intersection tests (e.g., `IntersectsBox`) utilize the Separating Axis Theorem by iterating through potential separating axes derived from the box basis vectors and the
   vector connecting box centers.
	- Raycasting Flow: For `RayIntersection`, the ray is transformed into the box's local coordinate space to simplify the intersection check against the six axis-aligned faces.
	- Expansion Flow: Functions like `AddPoint` or `AddBox` implement logic to check if the incoming geometry is already contained; if not, they recalculate the center and extents to encapsulate the
   new data.

	\par Dependencies
	- idVec3 (math primitives)
	- idMat3 (rotation and orientation basis)
	- idBounds (axis-aligned bounding box structures)
	- idPlane (plane-based collision primitives)
	- idSphere (bounding sphere primitives)
	- idMath (mathematical constants and utilities like INFINITUM)

	\par How It Fits
	- Acts as a high-precision bounding volume primitive within the engine's Bounding Volume (BV) hierarchy.
	- Provides the mathematical foundation for advanced collision detection between oriented game objects and static geometry.
	- Supports visibility and culling systems through silhouette generation and axis projection logic.
	- Functions as a more complex alternative to `idBounds` (AABB) when object rotation must be tightly encapsulated.
*/

#ifndef __BV_BOX_H__
#define __BV_BOX_H__

/*!
	\class idBox
	\brief A class representing an oriented bounding box with methods for geometric operations and transformations.

	The idBox class provides a comprehensive interface for representing and manipulating oriented bounding boxes in 3D space. It supports various construction methods including initialization from
   center point, extents, and axis; bounds; or single points. The class offers fundamental geometric operations such as adding points or boxes, translating, rotating, and expanding the box. It
   provides methods for intersection testing with other boxes, planes, lines, and rays, making it suitable for collision detection and spatial queries. The class also includes utilities for converting
   between different geometric representations, such as computing bounding spheres, projecting boxes onto axes, and calculating silhouette vertices for rendering purposes. The implementation handles
   both axis-aligned and oriented boxes, with methods supporting in-place transformations and mathematical operations that maintain the integrity of the bounding volume representation throughout
   various geometric computations within the engine.

*/
class idBox
{
public:
	//! Constructs an empty box with default initialization.
	idBox();

	/*!
		\brief Constructs a box with the specified center, extents, and axis.

		The constructor initializes a box object using the provided center point, extent values along each axis, and the orientation axis.

		\param center The center point of the box
		\param extents The extent values along each axis defining the size of the box
		\param axis The orientation axis defining the box's rotation
	*/
	explicit idBox( const idVec3& center, const idVec3& extents, const idMat3& axis );

	/*!
		\brief Initializes a box with the given point as its center and zero extents.

		Constructs a box object using the provided point as the center of the box. The extents of the box are initialized to zero, meaning the box has no size in any dimension. The axis of the box is
	   initialized to the identity matrix, indicating no rotation.

		\param point The point to use as the center of the box
	*/
	explicit idBox( const idVec3& point );

	/*!
		\brief Constructs a box from the given bounds.

		Initializes the box center and extents based on the provided bounds. The axis is initialized to the identity matrix.

		\param bounds The bounds used to initialize the box.
	*/
	explicit idBox( const idBounds& bounds );

	/*!
		\brief Creates a box object from bounds, origin, and axis.

		Initializes a box by computing its center and extents from the given bounds, then transforms the center using the provided origin and axis. The axis is directly assigned to the box's axis
	   member.

		\param bounds The bounds defining the box dimensions
		\param origin The origin to translate the box center by
		\param axis The axis to transform the box center and define the box orientation
	*/
	explicit idBox( const idBounds& bounds, const idVec3& origin, const idMat3& axis );

	idBox		  operator+( const idVec3& t ) const; // returns translated box
	idBox&		  operator+=( const idVec3& t );	  // translate the box
	idBox		  operator*( const idMat3& r ) const; // returns rotated box
	idBox&		  operator*=( const idMat3& r );	  // rotate the box
	idBox		  operator+( const idBox& a ) const;
	idBox&		  operator+=( const idBox& a );
	idBox		  operator-( const idBox& a ) const;
	idBox&		  operator-=( const idBox& a );

	/*!
		\brief Compares this box with another box for exact equality.

		This function performs an exact comparison between this box and another box, checking if their center points, extents, and axis vectors are exactly equal. It is used to determine if two boxes
	   represent the same geometric object without any floating-point tolerance. The comparison uses the Compare method of the center, extents, and axis members, which likely performs component-wise
	   exact comparisons.

		\param a The other box to compare with this box
		\return True if all components of the boxes (center, extents, and axis) are exactly equal, false otherwise
	*/
	bool		  Compare( const idBox& a ) const;

	/*!
		\brief Compares this box with another box using the specified epsilon tolerance for floating-point comparisons

		This function performs a component-wise comparison of the center, extents, and axis of this box with those of another box using the provided epsilon value for floating-point comparisons. It
	   returns true if all components are considered equal within the specified tolerance, and false otherwise. The comparison uses the Compare method of idVec3 and idMat3 classes, which internally
	   performs epsilon-based floating-point comparisons for their respective data types. This method is typically used in collision detection and geometric comparisons where exact floating-point
	   equality is not practical due to floating-point precision limitations.

		\param a The box to compare this box against
		\param epsilon The epsilon tolerance value for floating-point comparisons
		\return True if all components (center, extents, and axis) of this box are equal to the corresponding components of the other box within the specified epsilon tolerance, false otherwise
	*/
	bool		  Compare( const idBox& a, const float epsilon ) const;
	bool		  operator==( const idBox& a ) const; // exact compare, no epsilon
	bool		  operator!=( const idBox& a ) const; // exact compare, no epsilon

	//! Initializes the box components to zero values, creating a single point at the origin.
	void		  Clear();

	//! Sets all components of the box to zero.
	void		  Zero();

	//! Returns the center point of the box.
	const idVec3& GetCenter() const;

	//! Returns the extents of the box.
	const idVec3& GetExtents() const;

	//! Returns the axis of the box.
	const idMat3& GetAxis() const;

	//! Returns the volume of the box
	float		  GetVolume() const;

	//! Returns true if the box extents are invalid or have been cleared.
	bool		  IsCleared() const;

	/*!
		\brief Adds a point to the box and returns true if the box expanded.

		This function updates the bounding box to include the specified point. If the point is already contained within the current box, the function returns false without modifying the box.
	   Otherwise, it recalculates the box dimensions to encompass the new point, potentially expanding the box. The function uses an optimized approach to determine the best axis orientation for the
	   new box, comparing volumes of potential bounding solutions.

		\param v The point to be added to the box
		\return true if the box was expanded to include the point, false if the point was already contained within the box
	*/
	bool		  AddPoint( const idVec3& v );

	/*!
		\brief Adds the given box to this box and returns true if this box was expanded.

		This function computes the minimal axis-aligned bounding box that contains both this box and the provided box 'a'. It performs the addition by testing multiple potential axis orientations and
	   selecting the one that results in the smallest volume bounding box. If the provided box 'a' is entirely contained within this box, no expansion occurs and the function returns false. If this
	   box is entirely contained within box 'a', this box is replaced by 'a' and the function returns true. The function returns false if the resulting box is expanded, but the expansion is not
	   significant enough to warrant a change in the bounding box.

		\param a The box to be added to this box.
		\return True if this box was expanded by the addition of box 'a', false otherwise.
	*/
	bool		  AddBox( const idBox& a );

	//! Returns a box expanded in all directions by the given value.
	idBox		  Expand( const float d ) const;

	/*!
		\brief Expands the box extents equally in all directions by the specified distance.

		This function modifies the box by increasing each of its extent values by the given distance amount. The expansion is uniform across all three dimensions, effectively growing the box
	   symmetrically from its center. The function returns a reference to the modified box, allowing for method chaining.

		\param d The distance by which to expand the box extents in all directions
		\return A reference to the modified box object after expansion
	*/
	idBox&		  ExpandSelf( const float d );

	//! Returns a new box translated by the specified vector.
	idBox		  Translate( const idVec3& translation ) const;

	/*!
		\brief Shifts the box by the provided translation vector.

		Updates the center of the box by adding the translation vector to it.

		\param translation The displacement vector applied to the box center.
		\return A reference to the modified box.
	*/
	idBox&		  TranslateSelf( const idVec3& translation );

	//! Returns a box rotated by the given rotation matrix.
	idBox		  Rotate( const idMat3& rotation ) const;

	/*!
		\brief Rotates the box by the given rotation matrix and returns a reference to itself.

		This function applies a rotation transformation to the box by multiplying its center and axis components with the provided rotation matrix. The rotation is applied in-place, modifying the
	   box's orientation and position. The function returns a reference to the modified box, allowing for method chaining.

		\param rotation The rotation matrix to apply to the box
		\return A reference to the rotated box
	*/
	idBox&		  RotateSelf( const idMat3& rotation );

	/*!
		\brief Computes the minimum distance from the box to a plane, considering the box's orientation and size

		This function calculates the signed distance from the box to the given plane. It determines how far the box extends in the direction of the plane normal, taking into account the box's center
	   and extents. The result is positive if the box is in front of the plane, negative if it's behind, and zero if it intersects the plane. The function uses an optimized approach that considers the
	   box's bounding extents along each axis to compute the distance efficiently

		\param plane The plane to compute the distance to
		\return The signed distance from the box to the plane, where positive values indicate the box is in front of the plane, negative values indicate it's behind, and zero indicates intersection
	*/
	float		  PlaneDistance( const idPlane& plane ) const;

	/*!
		\brief Determines which side of a plane the box resides on based on the distance from the box center to the plane and the box extents.

		The function evaluates the position of the box relative to the given plane by calculating the distance from the box center to the plane and adding the contribution of the box extents along
	   each axis. This approach accounts for the box's size when determining whether the box is in front of, behind, or crossing the plane. The epsilon parameter allows for a tolerance in the
	   comparison to handle floating-point precision issues.

		\param plane The plane to test against
		\param epsilon The tolerance for determining if the box is close to the plane
		\return The side of the plane the box is on, which can be PLANESIDE_FRONT, PLANESIDE_BACK, or PLANESIDE_CROSS
	*/
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	/*!
		\brief Checks if a given point is contained within the box, including points on the boundary

		This function determines whether a specified point lies within the boundaries of the box. The box is defined by its center, axes, and extents. The function uses a separating axis theorem
	   approach to test if the point is inside the box. It first calculates the local position of the point relative to the box center, then checks if the point's projection onto each axis falls
	   within the corresponding extent. If the point is outside any of the three axes' extents, the function returns false. Otherwise, it returns true, indicating the point is inside or on the
	   boundary of the box. The function includes touching boundary points as part of the box's interior.

		\param p The point to check for containment within the box
		\return True if the point is inside or on the boundary of the box, false otherwise
	*/
	bool		  ContainsPoint( const idVec3& p ) const;

	/*!
		\brief Determines whether this box intersects with another box.

		This function implements the Separating Axis Theorem (SAT) to check for intersection between two oriented boxes. It tests all possible separating axes derived from the cross products of the
	   box axes and the relative position vector between the box centers. The function returns true if the boxes intersect, including when they touch at a point or edge.

		\param a The other box to test intersection against
		\return True if the boxes intersect or touch, false otherwise
	*/
	bool		  IntersectsBox( const idBox& a ) const;

	/*!
		\brief Checks if a line segment intersects with the box.

		This function determines whether a line segment defined by two points, start and end, intersects with the box represented by the calling idBox object. It uses the separating axis theorem to
	   test for intersection. The algorithm first checks for separation along the box axes, then tests for separation using the cross product of the line direction and the box axes.

		\param start The starting point of the line segment
		\param end The ending point of the line segment
		\return True if the line segment intersects with the box, false otherwise
	*/
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	/*!
		\brief Checks if a ray intersects with the box and calculates the intersection distances along the ray.

		This function determines whether a ray defined by a starting point and direction intersects with the box. It transforms the ray into the box's local coordinate system and performs intersection
	   tests against the box's six faces. The function returns true if an intersection occurs, and updates the scale parameters with the entry and exit distances along the ray. The scale1 parameter
	   contains the smaller distance value, while scale2 contains the larger one.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale1 The first intersection distance along the ray, updated on success
		\param scale2 The second intersection distance along the ray, updated on success
		\return True if the ray intersects the box, false otherwise.
	*/
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale1, float& scale2 ) const;

	/*!
		\brief Computes the tight-fitting oriented bounding box for a collection of 3D points.

		This function calculates the oriented bounding box that tightly encloses a set of 3D points. It computes the mean of the points, constructs the covariance matrix, and finds its eigenvectors
	   and eigenvalues to determine the principal axes of the point cloud. The box is then refined by projecting the points onto the computed axes to determine the final center and extents.

		\param points Array of 3D points to compute the bounding box for
		\param numPoints Number of points in the points array
	*/
	void		  FromPoints( const idVec3* points, const int numPoints );

	/*!
		\brief Computes the tightest bounding box that encompasses both a point and its translation.

		This function calculates a bounding box that encloses both the given point and the point translated by the provided translation vector. The resulting box is the minimal volume box that can
	   contain both the original point and the translated point. The implementation currently has a placeholder and needs to be fully implemented.

		\param point The initial point to be included in the bounding box
		\param translation The translation vector to be applied to the point for computing the bounding box
	*/
	void		  FromPointTranslation( const idVec3& point, const idVec3& translation );

	/*!
		\brief Computes the tightest bounding box that contains a point after applying a rotation transformation

		This function calculates a box that tightly encloses a given point once it has been transformed by the specified rotation. The implementation currently contains a placeholder and requires a
	   proper implementation to accurately compute the bounding box. The function takes an input box and a translation vector to determine the resulting bounding volume.

		\param box The input box to be transformed
		\param translation The translation to be applied to the box before computing the bounding box
	*/
	void		  FromBoxTranslation( const idBox& box, const idVec3& translation );

	/*!
		\brief Computes the tightest bounding box that contains a point after applying a rotation transformation.

		This function calculates a box that tightly encloses a given point once it has been transformed by the specified rotation. The implementation currently contains a placeholder and requires a
	   proper implementation to accurately compute the bounding box.

		\param point The point to be transformed and enclosed by the box
		\param rotation The rotation to be applied to the point before computing the bounding box
	*/
	void		  FromPointRotation( const idVec3& point, const idRotation& rotation );

	/*!
		\brief Computes a new box from an existing box and a rotation.

		This function takes an existing box and applies a rotation transformation to compute a new box that encompasses the rotated box. The implementation is currently marked as incomplete and
	   requires further development to properly handle the rotation logic.

		\param box The input box to be transformed
		\param rotation The rotation to be applied to the box
	*/
	void		  FromBoxRotation( const idBox& box, const idRotation& rotation );

	/*!
		\brief Converts a box representation into its 8 corner points.

		This function takes a box defined by a center point, axis vectors, and extents, and computes the 8 corner points of the box. The box is axis-aligned in its local space, and the points are
	   calculated in world space using the box's transformation properties. The resulting points are stored in the provided array of 8 vectors.

		\param points Output array to store the 8 corner points of the box
	*/
	void		  ToPoints( idVec3 points[8] ) const;

	//! Returns the bounding sphere that contains this box.
	idSphere	  ToSphere() const;

	/*!
		\brief Calculates the projection of this box onto the given axis.

		This function computes the minimum and maximum values of the projection of the box onto the specified direction vector. It uses the box's center and axis-aligned extents to calculate the
	   projection range along the given axis. The calculation takes into account the orientation of the box through its axis matrix and the magnitude of the extents along each axis.

		\param dir The direction vector onto which the box projection is calculated
		\param min Output parameter for the minimum projection value
		\param max Output parameter for the maximum projection value
	*/
	void		  AxisProjection( const idVec3& dir, float& min, float& max ) const;

	/*!
		\brief Computes the projection of the box along the given axis and stores the resulting bounds.

		This function calculates the minimum and maximum values of the projection of the box onto each component of the given axis matrix. It uses the center and extents of the box along with the axis
	   vectors to compute these projections. The resulting bounds represent the range of the box's projection along each axis component.

		\param ax The axis matrix used for projecting the box
		\param bounds The resulting bounds of the projection
	*/
	void		  AxisProjection( const idMat3& ax, idBounds& bounds ) const;

	/*!
		\brief Calculates and returns the silhouette vertices of the box when projected from a given origin.

		This function determines the silhouette of a box as seen from a specified projection origin. It computes the intersection of the box's edges with the projection plane and stores the resulting
	   vertices in the provided array. The function uses bit manipulation to efficiently determine which vertices form the silhouette based on the projection direction. The results are stored in the
	   silVerts array, and the function returns the number of silhouette vertices calculated.

		\param projectionOrigin The origin point from which the projection is calculated
		\param silVerts Output array to store the silhouette vertices
		\return The number of silhouette vertices calculated and stored in the silVerts array
	*/
	int			  GetProjectionSilhouetteVerts( const idVec3& projectionOrigin, idVec3 silVerts[6] ) const;

	/*!
		\brief Computes the silhouette vertices of a box when projected along a given direction

		This function calculates the vertices that form the silhouette of the box when projected using parallel projection along the specified direction. It determines which faces of the box are
	   visible from the projection direction and extracts the corresponding vertices. The function uses a precomputed lookup table to determine which vertices make up the silhouette based on the
	   projection direction. The projection direction is used to determine the visible faces by checking the dot product with the box's axis vectors.

		\param projectionDir The direction vector along which the parallel projection is performed
		\param silVerts Output array that will contain the silhouette vertices
		\return The number of silhouette vertices computed and stored in the silVerts array
	*/
	int			  GetParallelProjectionSilhouetteVerts( const idVec3& projectionDir, idVec3 silVerts[6] ) const;

private:
	idVec3 center;
	idVec3 extents;
	idMat3 axis;
};

extern idBox box_zero;

ID_INLINE	 idBox::idBox()
{
}

ID_INLINE idBox::idBox( const idVec3& center, const idVec3& extents, const idMat3& axis )
{
	this->center  = center;
	this->extents = extents;
	this->axis	  = axis;
}

ID_INLINE idBox::idBox( const idVec3& point )
{
	this->center = point;
	this->extents.Zero();
	this->axis.Identity();
}

ID_INLINE idBox::idBox( const idBounds& bounds )
{
	this->center  = ( bounds[0] + bounds[1] ) * 0.5f;
	this->extents = bounds[1] - this->center;
	this->axis.Identity();
}

ID_INLINE idBox::idBox( const idBounds& bounds, const idVec3& origin, const idMat3& axis )
{
	this->center  = ( bounds[0] + bounds[1] ) * 0.5f;
	this->extents = bounds[1] - this->center;
	this->center  = origin + this->center * axis;
	this->axis	  = axis;
}

ID_INLINE idBox idBox::operator+( const idVec3& t ) const
{
	return idBox( center + t, extents, axis );
}

ID_INLINE idBox& idBox::operator+=( const idVec3& t )
{
	center += t;
	return *this;
}

ID_INLINE idBox idBox::operator*( const idMat3& r ) const
{
	return idBox( center * r, extents, axis * r );
}

ID_INLINE idBox& idBox::operator*=( const idMat3& r )
{
	center *= r;
	axis *= r;
	return *this;
}

ID_INLINE idBox idBox::operator+( const idBox& a ) const
{
	idBox newBox;
	newBox = *this;
	newBox.AddBox( a );
	return newBox;
}

ID_INLINE idBox& idBox::operator+=( const idBox& a )
{
	idBox::AddBox( a );
	return *this;
}

ID_INLINE idBox idBox::operator-( const idBox& a ) const
{
	return idBox( center, extents - a.extents, axis );
}

ID_INLINE idBox& idBox::operator-=( const idBox& a )
{
	extents -= a.extents;
	return *this;
}

ID_INLINE bool idBox::Compare( const idBox& a ) const
{
	return ( center.Compare( a.center ) && extents.Compare( a.extents ) && axis.Compare( a.axis ) );
}

ID_INLINE bool idBox::Compare( const idBox& a, const float epsilon ) const
{
	return ( center.Compare( a.center, epsilon ) && extents.Compare( a.extents, epsilon ) && axis.Compare( a.axis, epsilon ) );
}

ID_INLINE bool idBox::operator==( const idBox& a ) const
{
	return Compare( a );
}

ID_INLINE bool idBox::operator!=( const idBox& a ) const
{
	return !Compare( a );
}

ID_INLINE void idBox::Clear()
{
	center.Zero();
	extents[0] = extents[1] = extents[2] = -idMath::INFINITUM;
	axis.Identity();
}

ID_INLINE void idBox::Zero()
{
	center.Zero();
	extents.Zero();
	axis.Identity();
}

ID_INLINE const idVec3& idBox::GetCenter() const
{
	return center;
}

ID_INLINE const idVec3& idBox::GetExtents() const
{
	return extents;
}

ID_INLINE const idMat3& idBox::GetAxis() const
{
	return axis;
}

ID_INLINE float idBox::GetVolume() const
{
	return ( extents * 2.0f ).LengthSqr();
}

ID_INLINE bool idBox::IsCleared() const
{
	return extents[0] < 0.0f;
}

ID_INLINE idBox idBox::Expand( const float d ) const
{
	return idBox( center, extents + idVec3( d, d, d ), axis );
}

ID_INLINE idBox& idBox::ExpandSelf( const float d )
{
	extents[0] += d;
	extents[1] += d;
	extents[2] += d;
	return *this;
}

ID_INLINE idBox idBox::Translate( const idVec3& translation ) const
{
	return idBox( center + translation, extents, axis );
}

ID_INLINE idBox& idBox::TranslateSelf( const idVec3& translation )
{
	center += translation;
	return *this;
}

ID_INLINE idBox idBox::Rotate( const idMat3& rotation ) const
{
	return idBox( center * rotation, extents, axis * rotation );
}

ID_INLINE idBox& idBox::RotateSelf( const idMat3& rotation )
{
	center *= rotation;
	axis *= rotation;
	return *this;
}

ID_INLINE bool idBox::ContainsPoint( const idVec3& p ) const
{
	idVec3 lp = p - center;
	if( idMath::Fabs( lp * axis[0] ) > extents[0] || idMath::Fabs( lp * axis[1] ) > extents[1] || idMath::Fabs( lp * axis[2] ) > extents[2] ) { return false; }
	return true;
}

ID_INLINE idSphere idBox::ToSphere() const
{
	return idSphere( center, extents.Length() );
}

ID_INLINE void idBox::AxisProjection( const idVec3& dir, float& min, float& max ) const
{
	float d1 = dir * center;
	float d2 = idMath::Fabs( extents[0] * ( dir * axis[0] ) ) + idMath::Fabs( extents[1] * ( dir * axis[1] ) ) + idMath::Fabs( extents[2] * ( dir * axis[2] ) );
	min		 = d1 - d2;
	max		 = d1 + d2;
}

ID_INLINE void idBox::AxisProjection( const idMat3& ax, idBounds& bounds ) const
{
	for( int i = 0; i < 3; i++ ) {
		float d1	 = ax[i] * center;
		float d2	 = idMath::Fabs( extents[0] * ( ax[i] * axis[0] ) ) + idMath::Fabs( extents[1] * ( ax[i] * axis[1] ) ) + idMath::Fabs( extents[2] * ( ax[i] * axis[2] ) );
		bounds[0][i] = d1 - d2;
		bounds[1][i] = d1 + d2;
	}
}

#endif /* !__BV_BOX_H__ */
