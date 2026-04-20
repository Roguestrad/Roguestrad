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

/*!
	\brief Returns the inverse movement velocity vector from start to end positions.

	This function calculates the inverse of the velocity vector derived from the difference between the end and start positions. It ensures that axis-aligned directions with magnitudes below a minimum
   threshold are adjusted to prevent division by zero. The function asserts that the squared length of the velocity vector is greater than a small epsilon value to avoid invalid operations.

	\param start The starting position vector
	\param end The ending position vector
	\return The inverse velocity vector calculated from the difference between end and start positions
	\throws Assertion failure if the squared length of the velocity vector is not greater than 1e-10f
*/
idVec3 GetInverseMovementVelocity( const idVec3& start, const idVec3& end );

/*!
	\brief Determines if a moving bounding box intersects with a static bounding box and calculates the time range of intersection.

	This function performs an intersection test between a moving bounding box defined by its start position, extent, and inverse velocity, and a static bounding box. It computes the range of time
   parameters where the intersection occurs. The function modifies the paramsRange array to store the minimum and maximum time values of intersection.

	\param startPosition The starting position of the moving bounding box
	\param invVelocity The inverse velocity of the moving bounding box
	\param extent The extent of the moving bounding box
	\param objBounds The static bounding box to test against
	\param paramsRange An array containing the minimum and maximum time parameters of intersection
	\return True if the moving bounding box intersects with the static bounding box, false otherwise.
	\throws Assertion failure if the objBounds is backwards.
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
