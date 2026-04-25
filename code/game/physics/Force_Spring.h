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

#ifndef __FORCE_SPRING_H__
#define __FORCE_SPRING_H__

/*!
	\class idForce_Spring
	\brief A spring force implementation that applies forces between two physics entities based on their relative positions and velocities.

	This class implements a spring force that connects two physics entities and applies forces based on the distance between their attachment points. The spring force is calculated using stretch and
   compression spring constants, a damping factor, and a rest length. It inherits from idForce and provides methods to initialize the spring properties, set the attachment points on physics entities,
   evaluate the force at a given time, and remove physics objects from the spring. The spring force is computed by considering the relative positions and velocities of the two attachment points to
   simulate spring behavior in a physics simulation.

*/
class idForce_Spring : public idForce
{
public:
	CLASS_PROTOTYPE( idForce_Spring );

	//! Initializes a new instance of the idForce_Spring class with default values.
	idForce_Spring();
	virtual ~idForce_Spring();

	/*!
		\brief Initializes the spring properties with the specified stretch, compression, damping, and rest length parameters.

		This function sets up the spring force parameters for the spring force calculation. It configures the spring constants for stretching and compressing forces, the damping factor to control
	   oscillation, and the rest length at which the spring force is zero.

		\param Kstretch Spring constant for stretching forces
		\param Kcompress Spring constant for compression forces
		\param damping Damping factor to reduce oscillation
		\param restLength Rest length of the spring where no force is applied
	*/
	void InitSpring( float Kstretch, float Kcompress, float damping, float restLength );

	/*!
		\brief Sets the entities and positions where the spring force is attached.

		This function configures the spring force by specifying two physics entities and their respective attachment points in world space. The spring will attempt to maintain a connection between
	   these two points as the physics simulation progresses.

		\param physics1 First physics entity the spring is attached to
		\param id1 Attachment point identifier on the first physics entity
		\param p1 World space position of the first attachment point
		\param physics2 Second physics entity the spring is attached to
		\param id2 Attachment point identifier on the second physics entity
		\param p2 World space position of the second attachment point
	*/
	void SetPosition( idPhysics* physics1, int id1, const idVec3& p1, idPhysics* physics2, int id2, const idVec3& p2 );

public:
	// common force interface ---------------------

	//! Evaluates the spring force between two points based on their positions and velocities at the given time.
	virtual void Evaluate( int time );

	//! Removes a physics object from the spring force.
	virtual void RemovePhysics( const idPhysics* phys );

private:
	// spring properties
	float	   Kstretch;
	float	   Kcompress;
	float	   damping;
	float	   restLength;

	// positioning
	idPhysics* physics1; // first physics object
	int		   id1;		 // clip model id of first physics object
	idVec3	   p1;		 // position on clip model
	idPhysics* physics2; // second physics object
	int		   id2;		 // clip model id of second physics object
	idVec3	   p2;		 // position on clip model
};

#endif /* !__FORCE_SPRING_H__ */
