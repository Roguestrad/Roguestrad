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

#ifndef __MATH_LINE_H__
#define __MATH_LINE_H__

//! Returns the inverse of the movement velocity vector between two points.
idVec3 GetInverseMovementVelocity( const idVec3& start, const idVec3& end );

/*!
	\brief Determines if a moving bounding box intersects with a static bounding box and calculates the intersection parameter range

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
bool   MovingBoundsIntersectBounds(
	  // moving bounds: center for t = 0, velocity for t = [0..1], extent
	  const idVec3&	  startPosition,
	  const idVec3&	  invVelocity,
	  const idVec3&	  extent,
	  // other bounds (standing still)
	  const idBounds& objBounds,
	  // in  [L,R]: time may vary as L <= t <= R    (usually L=0, R=1)
	  // out [L,R]: common points exist during L <= t <= R
	  float			  paramsRange[2] );

#endif
