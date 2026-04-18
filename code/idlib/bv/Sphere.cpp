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
	\file idlib/bv/Sphere.cpp
	\brief Provides the implementation for the `idSphere` class, a fundamental geometric bounding volume.
	\note archgen: sha256=33decc717868dc39e74f4b88b9a53b8b3913d6b347c2fac341851ec98f771ad2

	\par File Purpose
	- Provides the implementation for the `idSphere` class, a fundamental geometric bounding volume.
	- Implements various intersection tests and spatial relationship queries involving spheres, planes, lines, and rays.

	\par Core Responsibilities
	- Calculating signed distances from planes to sphere boundaries.
	- Determining the spatial orientation (front, back, or crossing) of a sphere relative to a plane.
	- Detecting intersections between line segments and the sphere volume.
	- Calculating intersection parameters for ray-sphere intersection tests.
	- Generating a bounding sphere from a collection of 3D points using an AABB-based center approach.

	\par Key Types and Functions
	- idSphere::PlaneDistance — Calculates the signed distance from a plane to the sphere's outer edge, accounting for the radius.
	- idSphere::PlaneSide — Evaluates if a sphere is entirely in front of, entirely behind, or intersecting a plane, using an epsilon for floating-point stability.
	- idSphere::LineIntersection — Determines if a finite line segment intersects the volume of the sphere.
	- idSphere::RayIntersection — Solves the quadratic equation for ray-sphere intersection, returning the entry and exit distances (scales) along the ray.
	- idSphere::FromPoints — Constructs a bounding sphere by finding the center of the points' AABB and calculating the maximum distance to any point in the set.

	\par Control Flow
	- For plane queries, the logic computes the distance from the sphere's origin to the plane and then offsets that distance by the radius to find the gap or penetration depth.
	- The line intersection algorithm projects the sphere's origin onto the line segment, checking the closest point (start, end, or internal) against the squared radius.
	- The ray intersection logic uses the quadratic formula (discriminant-based) to find intersection points, returning false if the ray misses the sphere.
	- The `FromPoints` routine utilizes `SIMDProcessor` for an efficient axis-aligned bounding box (AABB) calculation, followed by a single-pass radial distance check.

	\par Dependencies
	- idVec3 — 3D vector math primitives.
	- idPlane — Plane primitive for spatial queries.
	- idMath — Mathematical utility functions (e.g., Sqrt).
	- SIMDProcessor — Accelerated SIMD instructions for min/max point operations.
	- precompiled.h — Engine-wide precompiled header for core types.

	\par How It Fits
	- Acts as a low-level geometric primitive within the `idlib/bv` (Bounding Volume) system.
	- Serves as a building block for more complex collision detection algorithms and spatial partitioning structures within the engine.
	- Provides the mathematical foundation for visibility and collision queries involving spherical volumes.
*/

#include "precompiled.h"
#pragma hdrstop

idSphere sphere_zero( vec3_zero, 0.0f );

float	 idSphere::PlaneDistance( const idPlane& plane ) const
{
	float d;

	d = plane.Distance( origin );
	if( d > radius ) {
		return d - radius;
	}
	if( d < -radius ) {
		return d + radius;
	}
	return 0.0f;
}

int idSphere::PlaneSide( const idPlane& plane, const float epsilon ) const
{
	float d;

	d = plane.Distance( origin );
	if( d > radius + epsilon ) {
		return PLANESIDE_FRONT;
	}
	if( d < -radius - epsilon ) {
		return PLANESIDE_BACK;
	}
	return PLANESIDE_CROSS;
}

bool idSphere::LineIntersection( const idVec3& start, const idVec3& end ) const
{
	idVec3 r, s, e;
	float  a;

	s = start - origin;
	e = end - origin;
	r = e - s;
	a = -s * r;
	if( a <= 0 ) {
		return ( s * s < radius * radius );
	} else if( a >= r * r ) {
		return ( e * e < radius * radius );
	} else {
		r = s + ( a / ( r * r ) ) * r;
		return ( r * r < radius * radius );
	}
}

bool idSphere::RayIntersection( const idVec3& start, const idVec3& dir, float& scale1, float& scale2 ) const
{
	double a, b, c, d, sqrtd;
	idVec3 p;

	p = start - origin;
	a = dir * dir;
	b = dir * p;
	c = p * p - radius * radius;
	d = b * b - c * a;

	if( d < 0.0f ) {
		return false;
	}

	sqrtd = idMath::Sqrt( d );
	a	  = 1.0f / a;

	scale1 = ( -b + sqrtd ) * a;
	scale2 = ( -b - sqrtd ) * a;

	return true;
}

void idSphere::FromPoints( const idVec3* points, const int numPoints )
{
	int	   i;
	float  radiusSqr, dist;
	idVec3 mins, maxs;

	SIMDProcessor->MinMax( mins, maxs, points, numPoints );

	origin = ( mins + maxs ) * 0.5f;

	radiusSqr = 0.0f;
	for( i = 0; i < numPoints; i++ ) {
		dist = ( points[i] - origin ).LengthSqr();
		if( dist > radiusSqr ) {
			radiusSqr = dist;
		}
	}
	radius = idMath::Sqrt( radiusSqr );
}
