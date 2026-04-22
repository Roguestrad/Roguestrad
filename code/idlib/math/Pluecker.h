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

#ifndef __MATH_PLUECKER_H__
#define __MATH_PLUECKER_H__

/*!
	\class idPluecker
	\brief A class representing Pluecker coordinates for line geometry in 3D space.

	This class provides a complete implementation for handling Pluecker coordinates, which are used to represent lines in 3D space using a six-dimensional vector. The class supports construction from
   various geometric primitives like line segments and rays, arithmetic operations, normalization, and conversion between different line representations. It includes methods for computing geometric
   properties such as distance between lines, dot products, and inner products. The implementation is optimized for inline operations and provides both const and non-const access to components, making
   it suitable for performance-critical graphics and geometric computations.

*/
class idPluecker
{
public:
	//! Constructs a new idPluecker object.
	idPluecker();

	//! Constructs a pluecker coordinate from a float array.
	explicit idPluecker( const float* a );

	//! Constructs a Pluecker coordinate from a line defined by start and end points.
	explicit idPluecker( const idVec3& start, const idVec3& end );

	//! Initializes a new Pluecker coordinate with the specified six float values.
	explicit idPluecker( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );

	//! Returns the Pluecker coordinate value at the specified index
	float		 operator[]( const int index ) const;

	//! Returns a reference to the Pluecker coordinate component at the specified index.
	float&		 operator[]( const int index );

	//! Returns a new pluecker coordinate with all components negated.
	idPluecker	 operator-() const;

	//! Returns a new pluecker vector scaled by the given float value
	idPluecker	 operator*( const float a ) const;

	//! Divides the Pluecker coordinate by a scalar value and returns the resulting Pluecker coordinate.
	idPluecker	 operator/( const float a ) const;

	//! Computes the permuted inner product of this Pluecker coordinate with another Pluecker coordinate.
	float		 operator*( const idPluecker& a ) const;

	//! Returns the component-wise difference between this pluecker coordinate and another pluecker coordinate.
	idPluecker	 operator-( const idPluecker& a ) const;

	//! Returns a new pluecker coordinate that is the component-wise sum of this pluecker coordinate and the given pluecker coordinate.
	idPluecker	 operator+( const idPluecker& a ) const;

	//! Multiplies all components of the Pluecker coordinate by the given scalar value and returns a reference to the modified object.
	idPluecker&	 operator*=( const float a );

	//! Divides each component of the Pluecker coordinate by the given scalar value
	idPluecker&	 operator/=( const float a );

	//! Adds the components of another pluecker vector to this pluecker vector and returns a reference to this pluecker vector.
	idPluecker&	 operator+=( const idPluecker& a );

	//! Subtracts the components of another pluecker vector from this pluecker vector and returns a reference to this vector.
	idPluecker&	 operator-=( const idPluecker& a );

	//! Compares this pluecker with another for exact equality.
	bool		 Compare( const idPluecker& a ) const;

	//! Compares this pluecker coordinate with another using the specified epsilon tolerance.
	bool		 Compare( const idPluecker& a, const float epsilon ) const;

	//! Compares two idPluecker objects for exact equality.
	bool		 operator==( const idPluecker& a ) const;

	//! Compares two idPluecker objects for inequality.
	bool		 operator!=( const idPluecker& a ) const;

	//! Sets the six components of the Pluecker coordinate vector.
	void		 Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );

	//! Sets all components of the Pluecker coordinate to zero.
	void		 Zero();

	//! Computes the Pluecker coordinates from a line defined by two points.
	void		 FromLine( const idVec3& start, const idVec3& end );

	//! Constructs a Pluecker coordinate from a ray defined by a start point and direction vector.
	void		 FromRay( const idVec3& start, const idVec3& dir );

	//! Computes a Pluecker coordinate from the intersection of two planes.
	bool		 FromPlanes( const idPlane& p1, const idPlane& p2 );

	//! Converts a Pluecker coordinate to a line segment representation.
	bool		 ToLine( idVec3& start, idVec3& end ) const;

	//! Converts a Pluecker coordinate into a ray representation with start point and direction.
	bool		 ToRay( idVec3& start, idVec3& dir ) const;

	//! Converts the Pluecker coordinate to a direction vector.
	void		 ToDir( idVec3& dir ) const;

	//! Computes the permuted inner product of this Pluecker coordinate with another Pluecker coordinate.
	float		 PermutedInnerProduct( const idPluecker& a ) const;

	//! Computes the square of the 3D distance between this Pluecker line and another Pluecker line.
	float		 Distance3DSqr( const idPluecker& a ) const;

	//! Returns the length of the Pluecker coordinate vector.
	float		 Length() const;

	//! Returns the squared length of the Pluecker coordinate.
	float		 LengthSqr() const;

	//! Returns a normalized copy of this pluecker coordinate.
	idPluecker	 Normalize() const;

	//! Normalizes the Pluecker coordinate in place and returns the scaling factor used.
	float		 NormalizeSelf();

	//! Returns the dimension of the Pluecker coordinate, which is always 6.
	int			 GetDimension() const;

	//! Returns a pointer to the internal float array representation of the Pluecker coordinate.
	const float* ToFloatPtr() const;

	//! Returns a pointer to the internal float array representing the Pluecker coordinate.
	float*		 ToFloatPtr();

	//! Returns a string representation of the Pluecker coordinate with the specified precision
	const char*	 ToString( int precision = 2 ) const;

private:
	float p[6];
};

extern idPluecker pluecker_origin;
#define pluecker_zero pluecker_origin

ID_INLINE idPluecker::idPluecker()
{
}

ID_INLINE idPluecker::idPluecker( const float* a )
{
	memcpy( p, a, 6 * sizeof( float ) );
}

ID_INLINE idPluecker::idPluecker( const idVec3& start, const idVec3& end )
{
	FromLine( start, end );
}

ID_INLINE idPluecker::idPluecker( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE idPluecker idPluecker::operator-() const
{
	return idPluecker( -p[0], -p[1], -p[2], -p[3], -p[4], -p[5] );
}

ID_INLINE float idPluecker::operator[]( const int index ) const
{
	return p[index];
}

ID_INLINE float& idPluecker::operator[]( const int index )
{
	return p[index];
}

ID_INLINE idPluecker idPluecker::operator*( const float a ) const
{
	return idPluecker( p[0] * a, p[1] * a, p[2] * a, p[3] * a, p[4] * a, p[5] * a );
}

ID_INLINE float idPluecker::operator*( const idPluecker& a ) const
{
	return p[0] * a.p[4] + p[1] * a.p[5] + p[2] * a.p[3] + p[4] * a.p[0] + p[5] * a.p[1] + p[3] * a.p[2];
}

ID_INLINE idPluecker idPluecker::operator/( const float a ) const
{
	float inva;

	assert( a != 0.0f );
	inva = 1.0f / a;
	return idPluecker( p[0] * inva, p[1] * inva, p[2] * inva, p[3] * inva, p[4] * inva, p[5] * inva );
}

ID_INLINE idPluecker idPluecker::operator+( const idPluecker& a ) const
{
	return idPluecker( p[0] + a[0], p[1] + a[1], p[2] + a[2], p[3] + a[3], p[4] + a[4], p[5] + a[5] );
}

ID_INLINE idPluecker idPluecker::operator-( const idPluecker& a ) const
{
	return idPluecker( p[0] - a[0], p[1] - a[1], p[2] - a[2], p[3] - a[3], p[4] - a[4], p[5] - a[5] );
}

ID_INLINE idPluecker& idPluecker::operator*=( const float a )
{
	p[0] *= a;
	p[1] *= a;
	p[2] *= a;
	p[3] *= a;
	p[4] *= a;
	p[5] *= a;
	return *this;
}

ID_INLINE idPluecker& idPluecker::operator/=( const float a )
{
	float inva;

	assert( a != 0.0f );
	inva = 1.0f / a;
	p[0] *= inva;
	p[1] *= inva;
	p[2] *= inva;
	p[3] *= inva;
	p[4] *= inva;
	p[5] *= inva;
	return *this;
}

ID_INLINE idPluecker& idPluecker::operator+=( const idPluecker& a )
{
	p[0] += a[0];
	p[1] += a[1];
	p[2] += a[2];
	p[3] += a[3];
	p[4] += a[4];
	p[5] += a[5];
	return *this;
}

ID_INLINE idPluecker& idPluecker::operator-=( const idPluecker& a )
{
	p[0] -= a[0];
	p[1] -= a[1];
	p[2] -= a[2];
	p[3] -= a[3];
	p[4] -= a[4];
	p[5] -= a[5];
	return *this;
}

ID_INLINE bool idPluecker::Compare( const idPluecker& a ) const
{
	return ( ( p[0] == a[0] ) && ( p[1] == a[1] ) && ( p[2] == a[2] ) && ( p[3] == a[3] ) && ( p[4] == a[4] ) && ( p[5] == a[5] ) );
}

ID_INLINE bool idPluecker::Compare( const idPluecker& a, const float epsilon ) const
{
	if( idMath::Fabs( p[0] - a[0] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[1] - a[1] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[2] - a[2] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[3] - a[3] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[4] - a[4] ) > epsilon ) { return false; }

	if( idMath::Fabs( p[5] - a[5] ) > epsilon ) { return false; }

	return true;
}

ID_INLINE bool idPluecker::operator==( const idPluecker& a ) const
{
	return Compare( a );
}

ID_INLINE bool idPluecker::operator!=( const idPluecker& a ) const
{
	return !Compare( a );
}

ID_INLINE void idPluecker::Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE void idPluecker::Zero()
{
	p[0] = p[1] = p[2] = p[3] = p[4] = p[5] = 0.0f;
}

ID_INLINE void idPluecker::FromLine( const idVec3& start, const idVec3& end )
{
	p[0] = start[0] * end[1] - end[0] * start[1];
	p[1] = start[0] * end[2] - end[0] * start[2];
	p[2] = start[0] - end[0];
	p[3] = start[1] * end[2] - end[1] * start[2];
	p[4] = start[2] - end[2];
	p[5] = end[1] - start[1];
}

ID_INLINE void idPluecker::FromRay( const idVec3& start, const idVec3& dir )
{
	p[0] = start[0] * dir[1] - dir[0] * start[1];
	p[1] = start[0] * dir[2] - dir[0] * start[2];
	p[2] = -dir[0];
	p[3] = start[1] * dir[2] - dir[1] * start[2];
	p[4] = -dir[2];
	p[5] = dir[1];
}

ID_INLINE bool idPluecker::ToLine( idVec3& start, idVec3& end ) const
{
	idVec3 dir1, dir2;
	float  d;

	dir1[0] = p[3];
	dir1[1] = -p[1];
	dir1[2] = p[0];

	dir2[0] = -p[2];
	dir2[1] = p[5];
	dir2[2] = -p[4];

	d = dir2 * dir2;
	if( d == 0.0f ) {
		return false; // pluecker coordinate does not represent a line
	}

	start = dir2.Cross( dir1 ) * ( 1.0f / d );
	end	  = start + dir2;
	return true;
}

ID_INLINE bool idPluecker::ToRay( idVec3& start, idVec3& dir ) const
{
	idVec3 dir1;
	float  d;

	dir1[0] = p[3];
	dir1[1] = -p[1];
	dir1[2] = p[0];

	dir[0] = -p[2];
	dir[1] = p[5];
	dir[2] = -p[4];

	d = dir * dir;
	if( d == 0.0f ) {
		return false; // pluecker coordinate does not represent a line
	}

	start = dir.Cross( dir1 ) * ( 1.0f / d );
	return true;
}

ID_INLINE void idPluecker::ToDir( idVec3& dir ) const
{
	dir[0] = -p[2];
	dir[1] = p[5];
	dir[2] = -p[4];
}

ID_INLINE float idPluecker::PermutedInnerProduct( const idPluecker& a ) const
{
	return p[0] * a.p[4] + p[1] * a.p[5] + p[2] * a.p[3] + p[4] * a.p[0] + p[5] * a.p[1] + p[3] * a.p[2];
}

ID_INLINE float idPluecker::Length() const
{
	return ( float )idMath::Sqrt( p[5] * p[5] + p[4] * p[4] + p[2] * p[2] );
}

ID_INLINE float idPluecker::LengthSqr() const
{
	return ( p[5] * p[5] + p[4] * p[4] + p[2] * p[2] );
}

ID_INLINE float idPluecker::NormalizeSelf()
{
	float l, d;

	l = LengthSqr();
	if( l == 0.0f ) {
		return l; // pluecker coordinate does not represent a line
	}
	d = idMath::InvSqrt( l );
	p[0] *= d;
	p[1] *= d;
	p[2] *= d;
	p[3] *= d;
	p[4] *= d;
	p[5] *= d;
	return d * l;
}

ID_INLINE idPluecker idPluecker::Normalize() const
{
	float d;

	d = LengthSqr();
	if( d == 0.0f ) {
		return *this; // pluecker coordinate does not represent a line
	}
	d = idMath::InvSqrt( d );
	return idPluecker( p[0] * d, p[1] * d, p[2] * d, p[3] * d, p[4] * d, p[5] * d );
}

ID_INLINE int idPluecker::GetDimension() const
{
	return 6;
}

ID_INLINE const float* idPluecker::ToFloatPtr() const
{
	return p;
}

ID_INLINE float* idPluecker::ToFloatPtr()
{
	return p;
}

#endif /* !__MATH_PLUECKER_H__ */
