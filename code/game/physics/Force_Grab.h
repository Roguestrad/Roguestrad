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

#ifndef __FORCE_GRAB_H__
#define __FORCE_GRAB_H__

/*!
	\class idForce_Grab
	\brief Manages a grab force that attracts a physics object toward a specified goal position.

	The idForce_Grab class implements a force that pulls a physics object toward a target position, commonly used for grabbing or pulling entities within the simulation. It maintains a reference to a
   physics object and updates its force application based on the object's current state and the specified goal position. The force includes damping to control the acceleration and smoothness of the
   movement. The class supports initialization with damping parameters, setting the target physics object and goal position, and updating the force evaluation over time. It also provides methods to
   save and restore the grab force state, as well as to remove the physics object when needed. The class inherits from idForce, indicating it is part of a force system that can be applied to physics
   objects in the simulation.

*/
class idForce_Grab : public idForce
{
public:
	CLASS_PROTOTYPE( idForce_Grab );

	//! Saves the grab force parameters to the specified save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the grab force state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes a new instance of the idForce_Grab class.
	idForce_Grab();
	virtual ~idForce_Grab();

	//! Initializes the drag force with the specified damping value
	void Init( float damping );

	//! Sets the physics object being dragged along with its goal position.
	void SetPhysics( idPhysics* physics, int id, const idVec3& goal );

	//! Updates the goal position for the grab force effect.
	void SetGoalPosition( const idVec3& goal );

public: // common force interface
		//! Updates the grab force evaluation based on the current physics state and goal position.
	virtual void Evaluate( int time );

	//! Removes the specified physics object from the grab force if it matches the current physics object.
	virtual void RemovePhysics( const idPhysics* phys );

	//! Returns the distance from the object to the goal position.
	float		 GetDistanceToGoal();

private:
	// properties
	float	   damping;
	idVec3	   goalPosition;

	float	   distanceToGoal;

	// positioning
	idPhysics* physics; // physics object
	int		   id;		// clip model id of physics object
};

#endif /* !__FORCE_GRAB_H__ */
