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

#ifndef __BV_BOX_H__
#define __BV_BOX_H__

/*!
	\class idBox
	\brief A class representing an oriented bounding box with methods for construction, transformation, and geometric queries.

	This class encapsulates an oriented bounding box defined by a center point, extents, and an orientation axis. It supports various operations including construction from different geometric
   primitives, transformation through translation and rotation, and geometric queries such as containment, intersection, and projection. The class provides both const and non-const versions of
   transformation methods, allowing for in-place modification or creation of new instances. It includes specialized methods for computing tight bounding boxes from points, translations, and rotations,
   as well as methods for determining spatial relationships with planes, rays, and other boxes. The design emphasizes flexibility in handling oriented bounding boxes while maintaining performance
   through inline operations where appropriate.

*/
class idBox
{
public:
	//! Constructs an empty box with default initialization.
	idBox();

	//! Initializes a bounding box with a center point, extents, and orientation axis.
	explicit idBox( const idVec3& center, const idVec3& extents, const idMat3& axis );

	//! Constructs a box at the given point with zero extents and identity axis.
	explicit idBox( const idVec3& point );

	//! Creates a box from the given bounds.
	explicit idBox( const idBounds& bounds );

	//! Initializes a box with the given bounds, origin, and axis.
	explicit idBox( const idBounds& bounds, const idVec3& origin, const idMat3& axis );

	//! Returns a copy of this box translated by the specified vector.
	idBox		  operator+( const idVec3& t ) const;

	//! Translates the box by the specified vector.
	idBox&		  operator+=( const idVec3& t );

	//! Returns a box rotated by the provided matrix.
	idBox		  operator*( const idMat3& r ) const;

	//! Rotates the box by the given matrix.
	idBox&		  operator*=( const idMat3& r );

	//! Returns a new box that is the result of adding the current box and the provided box together.
	idBox		  operator+( const idBox& a ) const;

	//! Adds the coordinates of another box to this box and returns a reference to this box.
	idBox&		  operator+=( const idBox& a );

	//! Returns a new box that is the difference between this box and another box.
	idBox		  operator-( const idBox& a ) const;

	//! Subtracts the extents of another box from this box and returns a reference to this box.
	idBox&		  operator-=( const idBox& a );

	//! Compares this box with another box for equality.
	bool		  Compare( const idBox& a ) const;

	//! Compares this box with another box using the specified epsilon value for floating-point comparison
	bool		  Compare( const idBox& a, const float epsilon ) const;

	//! Checks if this box is exactly equal to another box.
	bool		  operator==( const idBox& a ) const;

	//! Returns true if this box is not equal to the given box using exact comparison.
	bool		  operator!=( const idBox& a ) const;

	//! Initializes the box to an inside-out state with zero center and negative infinite extents.
	void		  Clear();

	//! Sets all components of the box to zero, effectively creating a single point at the origin.
	void		  Zero();

	//! Returns the center point of the box.
	const idVec3& GetCenter() const;

	//! Returns the extents of the box.
	const idVec3& GetExtents() const;

	//! Returns the axis of the box.
	const idMat3& GetAxis() const;

	//! Returns the volume of the box.
	float		  GetVolume() const;

	//! Returns true if the box has invalid or cleared extents
	bool		  IsCleared() const;

	//! Adds a point to the box and returns true if the box expanded.
	bool		  AddPoint( const idVec3& v );

	//! Adds the given box to this box and returns true if the box expanded.
	bool		  AddBox( const idBox& a );

	//! Returns a copy of this box expanded in all directions by the given distance
	idBox		  Expand( const float d ) const;

	//! Expands the box in all directions by the specified delta value.
	idBox&		  ExpandSelf( const float d );

	//! Returns a new box translated by the specified vector.
	idBox		  Translate( const idVec3& translation ) const;

	//! Translates this box by the specified translation vector and returns a reference to itself.
	idBox&		  TranslateSelf( const idVec3& translation );

	//! Returns a new box rotated by the specified rotation matrix.
	idBox		  Rotate( const idMat3& rotation ) const;

	//! Rotates the box by the provided rotation matrix and returns a reference to itself.
	idBox&		  RotateSelf( const idMat3& rotation );

	//! Computes the signed distance from the box to a plane, accounting for the box's orientation and size.
	float		  PlaneDistance( const idPlane& plane ) const;

	//! Determines which side of a plane the box resides on using the given epsilon tolerance
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	//! Checks if a point is contained within the box, including points on the surface.
	bool		  ContainsPoint( const idVec3& p ) const;

	//! Determines if this box intersects with another box, including cases where they touch.
	bool		  IntersectsBox( const idBox& a ) const;

	//! Tests if a line segment intersects with the box.
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	/*!
		\brief Tests if a ray intersects with the box and calculates the intersection scalars.

		This function determines whether a ray defined by a starting point and direction intersects with the box. It computes two scalar values that represent the intersection points along the ray.
	   The function transforms the ray into the local space of the box before performing the intersection test. The scalar values are updated only if the ray intersects the box.

		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale1 Output scalar representing the first intersection point along the ray
		\param scale2 Output scalar representing the second intersection point along the ray
		\return True if the ray intersects the box, false otherwise.
	*/
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale1, float& scale2 ) const;

	//! Computes a tight-fitting axis-aligned bounding box for a collection of points.
	void		  FromPoints( const idVec3* points, const int numPoints );

	//! Computes the tightest bounding box for a point translated by a given vector.
	void		  FromPointTranslation( const idVec3& point, const idVec3& translation );

	//! Initializes the box from another box and a translation vector.
	void		  FromBoxTranslation( const idBox& box, const idVec3& translation );

	//! Creates the tightest bounding box for a given point and rotation
	void		  FromPointRotation( const idVec3& point, const idRotation& rotation );

	//! Computes a new box from an existing box and a rotation.
	void		  FromBoxRotation( const idBox& box, const idRotation& rotation );

	//! Computes the 8 corner points of the box and stores them in the provided array.
	void		  ToPoints( idVec3 points[8] ) const;

	//! Returns the bounding sphere of this box.
	idSphere	  ToSphere() const;

	//! Calculates the projection of this box onto the given axis.
	void		  AxisProjection( const idVec3& dir, float& min, float& max ) const;

	//! Computes the projection of this box along the given axis and stores the result in the provided bounds.
	void		  AxisProjection( const idMat3& ax, idBounds& bounds ) const;

	//! Calculates the silhouette vertices of the box projected from a given origin.
	int			  GetProjectionSilhouetteVerts( const idVec3& projectionOrigin, idVec3 silVerts[6] ) const;

	//! Calculates the silhouette vertices of a box projected along a given direction.
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
