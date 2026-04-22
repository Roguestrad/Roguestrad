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

#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

/*
===============================================================================

	3D plane with equation: a * x + b * y + c * z + d = 0

===============================================================================
*/

class idVec3;
class idMat3;

#define ON_EPSILON				0.1f
#define DEGENERATE_DIST_EPSILON 1e-4f

#define SIDE_FRONT				0
#define SIDE_BACK				1
#define SIDE_ON					2
#define SIDE_CROSS				3

// plane sides
#define PLANESIDE_FRONT			0
#define PLANESIDE_BACK			1
#define PLANESIDE_ON			2
#define PLANESIDE_CROSS			3

// plane types
#define PLANETYPE_X				0
#define PLANETYPE_Y				1
#define PLANETYPE_Z				2
#define PLANETYPE_NEGX			3
#define PLANETYPE_NEGY			4
#define PLANETYPE_NEGZ			5
#define PLANETYPE_TRUEAXIAL		6 // all types < 6 are true axial planes
#define PLANETYPE_ZEROX			6
#define PLANETYPE_ZEROY			7
#define PLANETYPE_ZEROZ			8
#define PLANETYPE_NONAXIAL		9

/*!
	\class idPlane
	\brief Represents a mathematical plane in 3D space with support for various geometric operations.

	The idPlane class encapsulates a plane equation in 3D space defined by the formula ax + by + cz + d = 0, where a, b, c represent the normal vector components and d represents the distance from the
   origin. The class provides constructors for initializing planes from various geometric inputs such as normal vectors and distances, three points, or direction vectors. It supports standard
   arithmetic operations including addition, subtraction, and scalar multiplication, as well as transformation operations like translation and rotation. The class includes methods for normalizing
   planes, checking for degenerate cases, calculating distances and sides of points relative to the plane, and performing intersection tests with lines and rays. Additionally, it offers utilities for
   fitting planes through points or vectors, converting between plane and vector representations, and generating string representations of the plane equation for debugging purposes. The design
   emphasizes both performance through inline implementations and functionality for geometric computations commonly required in 3D graphics and mathematical operations.

*/
class idPlane
{
public:
	//! Default constructor for the idPlane class.
	idPlane();

	//! Constructs an idPlane object with the specified coefficients a, b, c, and d.
	explicit idPlane( float a, float b, float c, float d );

	//! Initializes a plane with a normal vector and distance from origin.
	explicit idPlane( const idVec3& normal, const float dist );

	//! Constructs a plane from three points and optionally fixes degenerate cases.
	explicit idPlane( const idVec3& v0, const idVec3& v1, const idVec3& v2, bool fixDegenerate = false );

	//! Returns the plane coefficient at the specified index
	float		  operator[]( int index ) const;

	//! Provides indexed access to the components of a plane equation
	float&		  operator[]( int index );

	//! Returns the negated version of this plane.
	idPlane		  operator-() const;

	//! Assigns the components of a vector to the normal components of the plane and sets the distance component to zero
	idPlane&	  operator=( const idVec3& v );

	//! Adds two plane equations component-wise and returns the result.
	idPlane		  operator+( const idPlane& p ) const;

	//! Subtracts another plane equation from this plane equation and returns the result.
	idPlane		  operator-( const idPlane& p ) const;

	//! Scales the plane by the given scalar value.
	idPlane		  operator*( const float s ) const;

	//! Multiplies the plane's normal vector by the given matrix and returns a reference to the plane.
	idPlane&	  operator*=( const idMat3& m );

	//! Compares this plane with another plane for exact equality.
	bool		  Compare( const idPlane& p ) const;

	//! Compares this plane with another plane using the specified epsilon tolerance.
	bool		  Compare( const idPlane& p, const float epsilon ) const;

	//! Compares this plane with another plane using the specified epsilon values for normal and distance.
	bool		  Compare( const idPlane& p, const float normalEps, const float distEps ) const;

	//! Compares two planes for exact equality without any epsilon tolerance.
	bool		  operator==( const idPlane& p ) const;

	//! Returns true if the plane is not equal to the given plane.
	bool		  operator!=( const idPlane& p ) const;

	//! Sets all components of the plane to zero.
	void		  Zero();

	//! Sets the normal vector components of the plane from the given vector
	void		  SetNormal( const idVec3& normal );

	//! Returns a reference to the normal vector of the plane.
	const idVec3& Normal() const;

	//! Returns a reference to the normal vector of the plane.
	idVec3&		  Normal();

	//! Normalizes the plane normal and returns its length
	float		  Normalize( bool fixDegenerate = true );

	//! Fixes a degenerate normal vector.
	bool		  FixDegenerateNormal();

	//! Fixes degenerate plane normal and distance values.
	bool		  FixDegeneracies( float distEpsilon );

	//! Returns the distance component of the plane equation
	float		  Dist() const;

	//! Sets the distance component of the plane to the negative of the provided distance value
	void		  SetDist( const float dist );

	//! Returns the type of the plane based on its normal vector.
	int			  Type() const;

	//! Sets the plane from three points and returns true if the plane is valid.
	bool		  FromPoints( const idVec3& p1, const idVec3& p2, const idVec3& p3, bool fixDegenerate = true );

	//! Calculates a plane from two direction vectors and a point, returning false if the vectors are degenerate.
	bool		  FromVecs( const idVec3& dir1, const idVec3& dir2, const idVec3& p, bool fixDegenerate = true );

	//! Adjusts the plane constant d so that the plane passes through the given point p
	void		  FitThroughPoint( const idVec3& p );

	//! Fits a plane to a set of 3D points and returns true if successful.
	bool		  HeightFit( const idVec3* points, const int numPoints );

	//! Returns a new plane that is translated by the specified vector along the plane normal.
	idPlane		  Translate( const idVec3& translation ) const;

	//! Translates the plane by the specified translation vector and returns a reference to itself.
	idPlane&	  TranslateSelf( const idVec3& translation );

	//! Returns a new plane that is the result of rotating this plane by the given axis around the specified origin.
	idPlane		  Rotate( const idVec3& origin, const idMat3& axis ) const;

	//! Rotates the plane around a given origin using the provided rotation matrix and returns a reference to itself.
	idPlane&	  RotateSelf( const idVec3& origin, const idMat3& axis );

	//! Calculates the signed distance from a point to the plane.
	float		  Distance( const idVec3& v ) const;

	//! Determines which side of the plane a given point resides on, with an optional epsilon tolerance for on-plane detection.
	int			  Side( const idVec3& v, const float epsilon = 0.0f ) const;

	//! Determines if a line segment intersects with the plane.
	bool		  LineIntersection( const idVec3& start, const idVec3& end ) const;

	//! Computes the intersection of a ray with the plane and returns the scale factor for the ray equation.
	bool		  RayIntersection( const idVec3& start, const idVec3& dir, float& scale ) const;

	//! Computes the intersection line between this plane and another plane, returning true if successful.
	bool		  PlaneIntersection( const idPlane& plane, idVec3& start, idVec3& dir ) const;

	//! Returns the dimension of the plane, which is always 4.
	int			  GetDimension() const;

	//! Returns a const reference to the plane data as an idVec4.
	const idVec4& ToVec4() const;

	//! Returns a reference to the plane data reinterpreted as an idVec4.
	idVec4&		  ToVec4();

	//! Returns a pointer to the float representation of the plane data.
	const float*  ToFloatPtr() const;

	//! Returns a float pointer to the plane data.
	float*		  ToFloatPtr();

	//! Returns a string representation of the plane with the specified decimal precision
	const char*	  ToString( int precision = 2 ) const;

private:
	float a;
	float b;
	float c;
	float d;
};

extern idPlane plane_origin;
#define plane_zero plane_origin

ID_INLINE idPlane::idPlane()
{
}

ID_INLINE idPlane::idPlane( float a, float b, float c, float d )
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

ID_INLINE idPlane::idPlane( const idVec3& normal, const float dist )
{
	this->a = normal.x;
	this->b = normal.y;
	this->c = normal.z;
	this->d = -dist;
}

ID_INLINE idPlane::idPlane( const idVec3& v0, const idVec3& v1, const idVec3& v2, bool fixDegenerate )
{
	FromPoints( v0, v1, v2, fixDegenerate );
}

ID_INLINE float idPlane::operator[]( int index ) const
{
	return ( &a )[index];
}

ID_INLINE float& idPlane::operator[]( int index )
{
	return ( &a )[index];
}

ID_INLINE idPlane idPlane::operator-() const
{
	return idPlane( -a, -b, -c, -d );
}

ID_INLINE idPlane& idPlane::operator=( const idVec3& v )
{
	a = v.x;
	b = v.y;
	c = v.z;
	d = 0;
	return *this;
}

ID_INLINE idPlane idPlane::operator+( const idPlane& p ) const
{
	return idPlane( a + p.a, b + p.b, c + p.c, d + p.d );
}

ID_INLINE idPlane idPlane::operator-( const idPlane& p ) const
{
	return idPlane( a - p.a, b - p.b, c - p.c, d - p.d );
}

ID_INLINE idPlane idPlane::operator*( const float s ) const
{
	return idPlane( a * s, b * s, c * s, d * s );
}

ID_INLINE idPlane& idPlane::operator*=( const idMat3& m )
{
	Normal() *= m;
	return *this;
}

ID_INLINE bool idPlane::Compare( const idPlane& p ) const
{
	return ( a == p.a && b == p.b && c == p.c && d == p.d );
}

ID_INLINE bool idPlane::Compare( const idPlane& p, const float epsilon ) const
{
	if( idMath::Fabs( a - p.a ) > epsilon ) { return false; }

	if( idMath::Fabs( b - p.b ) > epsilon ) { return false; }

	if( idMath::Fabs( c - p.c ) > epsilon ) { return false; }

	if( idMath::Fabs( d - p.d ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idPlane::Compare( const idPlane& p, const float normalEps, const float distEps ) const
{
	if( idMath::Fabs( d - p.d ) > distEps ) { return false; }
	if( !Normal().Compare( p.Normal(), normalEps ) ) { return false; }
	return true;
}

ID_INLINE bool idPlane::operator==( const idPlane& p ) const
{
	return Compare( p );
}

ID_INLINE bool idPlane::operator!=( const idPlane& p ) const
{
	return !Compare( p );
}

ID_INLINE void idPlane::Zero()
{
	a = b = c = d = 0.0f;
}

ID_INLINE void idPlane::SetNormal( const idVec3& normal )
{
	a = normal.x;
	b = normal.y;
	c = normal.z;
}

ID_INLINE const idVec3& idPlane::Normal() const
{
	return *reinterpret_cast<const idVec3*>( &a );
}

ID_INLINE idVec3& idPlane::Normal()
{
	return *reinterpret_cast<idVec3*>( &a );
}

ID_INLINE float idPlane::Normalize( bool fixDegenerate )
{
	float length = reinterpret_cast<idVec3*>( &a )->Normalize();

	if( fixDegenerate ) { FixDegenerateNormal(); }
	return length;
}

ID_INLINE bool idPlane::FixDegenerateNormal()
{
	return Normal().FixDegenerateNormal();
}

ID_INLINE bool idPlane::FixDegeneracies( float distEpsilon )
{
	bool fixedNormal = FixDegenerateNormal();
	// only fix dist if the normal was degenerate
	if( fixedNormal ) {
		if( idMath::Fabs( d - idMath::Rint( d ) ) < distEpsilon ) { d = idMath::Rint( d ); }
	}
	return fixedNormal;
}

ID_INLINE float idPlane::Dist() const
{
	return -d;
}

ID_INLINE void idPlane::SetDist( const float dist )
{
	d = -dist;
}

ID_INLINE bool idPlane::FromPoints( const idVec3& p1, const idVec3& p2, const idVec3& p3, bool fixDegenerate )
{
	Normal() = ( p1 - p2 ).Cross( p3 - p2 );
	if( Normalize( fixDegenerate ) == 0.0f ) { return false; }
	d = -( Normal() * p2 );
	return true;
}

ID_INLINE bool idPlane::FromVecs( const idVec3& dir1, const idVec3& dir2, const idVec3& p, bool fixDegenerate )
{
	Normal() = dir1.Cross( dir2 );
	if( Normalize( fixDegenerate ) == 0.0f ) { return false; }
	d = -( Normal() * p );
	return true;
}

ID_INLINE void idPlane::FitThroughPoint( const idVec3& p )
{
	d = -( Normal() * p );
}

ID_INLINE idPlane idPlane::Translate( const idVec3& translation ) const
{
	return idPlane( a, b, c, d - translation * Normal() );
}

ID_INLINE idPlane& idPlane::TranslateSelf( const idVec3& translation )
{
	d -= translation * Normal();
	return *this;
}

ID_INLINE idPlane idPlane::Rotate( const idVec3& origin, const idMat3& axis ) const
{
	idPlane p;
	p.Normal() = Normal() * axis;
	p.d		   = d + origin * Normal() - origin * p.Normal();
	return p;
}

ID_INLINE idPlane& idPlane::RotateSelf( const idVec3& origin, const idMat3& axis )
{
	d += origin * Normal();
	Normal() *= axis;
	d -= origin * Normal();
	return *this;
}

ID_INLINE float idPlane::Distance( const idVec3& v ) const
{
	return a * v.x + b * v.y + c * v.z + d;
}

ID_INLINE int idPlane::Side( const idVec3& v, const float epsilon ) const
{
	float dist = Distance( v );
	if( dist > epsilon ) {
		return PLANESIDE_FRONT;
	} else if( dist < -epsilon ) {
		return PLANESIDE_BACK;
	} else {
		return PLANESIDE_ON;
	}
}

ID_INLINE bool idPlane::LineIntersection( const idVec3& start, const idVec3& end ) const
{
	float d1, d2, fraction;

	d1 = Normal() * start + d;
	d2 = Normal() * end + d;
	if( d1 == d2 ) { return false; }
	if( d1 > 0.0f && d2 > 0.0f ) { return false; }
	if( d1 < 0.0f && d2 < 0.0f ) { return false; }
	fraction = ( d1 / ( d1 - d2 ) );
	return ( fraction >= 0.0f && fraction <= 1.0f );
}

ID_INLINE bool idPlane::RayIntersection( const idVec3& start, const idVec3& dir, float& scale ) const
{
	float d1, d2;

	d1 = Normal() * start + d;
	d2 = Normal() * dir;
	if( d2 == 0.0f ) { return false; }
	scale = -( d1 / d2 );
	return true;
}

ID_INLINE int idPlane::GetDimension() const
{
	return 4;
}

ID_INLINE const idVec4& idPlane::ToVec4() const
{
	return *reinterpret_cast<const idVec4*>( &a );
}

ID_INLINE idVec4& idPlane::ToVec4()
{
	return *reinterpret_cast<idVec4*>( &a );
}

ID_INLINE const float* idPlane::ToFloatPtr() const
{
	return reinterpret_cast<const float*>( &a );
}

ID_INLINE float* idPlane::ToFloatPtr()
{
	return reinterpret_cast<float*>( &a );
}

#endif /* !__MATH_PLANE_H__ */
