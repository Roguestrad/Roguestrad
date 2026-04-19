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

#ifndef __SURFACE_SWEPTSPLINE_H__
#define __SURFACE_SWEPTSPLINE_H__

/*
===============================================================================

	Swept Spline surface.

===============================================================================
*/

class idSurface_SweptSpline : public idSurface
{
public:
	//! Initializes an idSurface_SweptSpline object with null pointers for spline and sweptSpline.
	idSurface_SweptSpline();

	/*!
		\brief Destroys the idSurface_SweptSpline object and cleans up its associated spline data.

		The destructor releases the memory allocated for the spline and sweptSpline members of the idSurface_SweptSpline class. It ensures that no memory leaks occur by properly deleting the
	   dynamically allocated objects.

	*/
	~idSurface_SweptSpline();

	/*!
		\brief Sets the spline used for swept surface calculations.

		This function replaces the current spline with the provided one. If a spline was already set, it is deleted to prevent memory leaks. The function takes ownership of the provided spline
	   pointer.

		\param spline Pointer to the new spline to be used for swept surface calculations
	*/
	void SetSpline( idCurve_Spline<idVec4>* spline );

	/*!
		\brief Sets the swept spline for this surface.

		This function assigns a new swept spline to the surface, replacing any previously set spline. If a spline was already set, it is deleted before the new one is assigned.

		\param sweptSpline Pointer to the new swept spline to be set
	*/
	void SetSweptSpline( idCurve_Spline<idVec4>* sweptSpline );

	/*!
		\brief Initializes a NURBS curve to represent a swept circle with the specified radius.

		This function sets up a NURBS curve that defines a circle shape for sweeping operations. It creates a new NURBS curve with four control points arranged to form a circle, initializes the curve
	   with specific boundary conditions, and replaces any existing swept spline with the newly created one. The curve is configured to be closed and has a specific time parameterization for the sweep
	   operation.

		\param radius The radius of the circle to be created for the swept spline
	*/
	void SetSweptCircle( const float radius );

	/*!
		\brief Computes a tessellated surface by sweeping a spline along another spline with specified subdivision counts

		This function generates a surface by sweeping one spline (defined by sweptSpline) along another spline (defined by spline). The tessellation is controlled by two parameters that define the
	   number of subdivisions for each spline. The resulting surface is stored in the verts and indexes members of the class. The function handles both open and closed boundary conditions for the
	   splines. It calculates positions, tangents, and normals for each vertex in the tessellated surface.

		\param splineSubdivisions Number of subdivisions for the sweeping spline
		\param sweptSplineSubdivisions Number of subdivisions for the swept spline
	*/
	void Tessellate( const int splineSubdivisions, const int sweptSplineSubdivisions );

	//! Clears the swept spline surface by resetting its state and deleting internal spline data.
	void Clear();

protected:
	idCurve_Spline<idVec4>* spline;
	idCurve_Spline<idVec4>* sweptSpline;

	/*!
		\brief Computes a new coordinate frame based on a previous frame and a direction vector.

		This function calculates a new orthonormal basis (coordinate frame) from the given previous frame and direction. It uses quaternion mathematics to rotate the previous frame towards the new
	   direction while ensuring the resulting frame remains orthonormal. The operation involves computing a rotation quaternion from the angle between the previous frame's Z axis and the new
	   direction, converting it to a rotation matrix, and applying it to the previous frame. The resulting frame is updated to have the new direction as its Z axis, with the X and Y axes recomputed to
	   maintain orthogonality.

		\param previousFrame The input coordinate frame to be used as the starting point for the calculation
		\param dir The target direction vector that defines the new Z axis of the resulting frame
		\param newFrame The output coordinate frame computed by this function
	*/
	void					GetFrame( const idMat3& previousFrame, const idVec3 dir, idMat3& newFrame );
};

ID_INLINE idSurface_SweptSpline::idSurface_SweptSpline()
{
	spline		= NULL;
	sweptSpline = NULL;
}

ID_INLINE idSurface_SweptSpline::~idSurface_SweptSpline()
{
	delete spline;
	delete sweptSpline;
}

ID_INLINE void idSurface_SweptSpline::Clear()
{
	idSurface::Clear();
	delete spline;
	spline = NULL;
	delete sweptSpline;
	sweptSpline = NULL;
}

#endif /* !__SURFACE_SWEPTSPLINE_H__ */
