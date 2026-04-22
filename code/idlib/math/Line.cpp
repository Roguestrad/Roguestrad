/*****************************************************************************
The Dark Mod GPL Source Code

This file is part of the The Dark Mod Source Code, originally based
on the Doom 3 GPL Source Code as published in 2011.

The Dark Mod Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Project: The Dark Mod (http://www.thedarkmod.com/)

******************************************************************************/

#include "precompiled.h"
#pragma hdrstop

#include "Line.h"

//! Returns the inverse of the movement velocity vector between two points.
idVec3 GetInverseMovementVelocity( const idVec3& start, const idVec3& end )
{
	auto inverse = []( float x ) -> float {
		// note: we change axis-aligned directions by at most 1e-10
		static const float MinDelta = 1e-10f;
		if( idMath::Fabs( x ) < MinDelta ) {
			x = x < 0.0f ? -MinDelta : MinDelta;
		}
		return 1.0f / x;
	};

	idVec3 vel = end - start;
	assert( vel.LengthSqr() > 1e-10f );
	idVec3 invVel( inverse( vel.x ), inverse( vel.y ), inverse( vel.z ) );
	return invVel;
}

/*!
	\brief Determines if a moving bounding box intersects with a static bounding box and calculates the intersection parameter range.

	This function performs intersection testing between a moving bounding box defined by its start position, inverse velocity, and extent, and a static bounding box. It calculates the range of
   parameter values where the intersection occurs and stores these values in the paramsRange array. The function uses an optimized approach based on separating axis theorem principles, computing
   intersection times along each axis and then combining these results. The function assumes that the object bounds are not backwards and will assert in debug builds if this condition is not met.

	\param startPosition The starting position of the moving bounding box
	\param invVelocity The inverse velocity vector of the moving bounding box
	\param extent The extent (half-size) of the moving bounding box
	\param objBounds The static bounding box to test intersection with
	\param paramsRange An array of two floats that will be filled with the minimum and maximum intersection parameters
	\return true if the moving bounding box intersects with the static bounding box, false otherwise
	\throws assertion in debug builds if objBounds.IsBackwards() returns true
*/
bool MovingBoundsIntersectBounds( const idVec3& startPosition, const idVec3& invVelocity, const idVec3& extent, const idBounds& objBounds, float paramsRange[2] )
{
	assert( !objBounds.IsBackwards() );
	idVec3 pmin = objBounds[0] - extent;
	idVec3 pmax = objBounds[1] + extent;
	idVec3 tmin = ( pmin - startPosition );
	idVec3 tmax = ( pmax - startPosition );
	tmin.MulCW( invVelocity );
	tmax.MulCW( invVelocity );
	for( int d = 0; d < 3; d++ ) {
		float a = tmin[d], b = tmax[d];
		tmin[d] = idMath::Fmin( a, b );
		tmax[d] = idMath::Fmax( a, b );
	}
	float scaleMin = idMath::Fmax( idMath::Fmax( tmin.x, tmin.y ), idMath::Fmax( tmin.z, paramsRange[0] ) );
	float scaleMax = idMath::Fmin( idMath::Fmin( tmax.x, tmax.y ), idMath::Fmin( tmax.z, paramsRange[1] ) );
	paramsRange[0] = scaleMin;
	paramsRange[1] = scaleMax;
	return scaleMin <= scaleMax;
}
