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

#ifndef __FORCE_CONSTANT_H__
#define __FORCE_CONSTANT_H__

/*!
	\class idForce_Constant
	\brief A force application that applies a constant force vector to physics objects.

	This class implements a constant force that can be applied to physics objects within a simulation. It inherits from idForce and provides functionality to set the force vector, position, and
   associated physics object. The force is applied consistently over time through the Evaluate method, which is typically called during physics simulation updates. The class supports persistence
   through Save and Restore methods, allowing the force state to be maintained across game sessions. It manages the association with physics objects and can remove that association when needed.

*/
class idForce_Constant : public idForce
{
public:
	CLASS_PROTOTYPE( idForce_Constant );

	//! Initializes a new instance of the idForce_Constant class with default values.
	idForce_Constant();
	virtual ~idForce_Constant();

	//! Saves the constant force properties to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the constant force state from a saved game file.
	void Restore( idRestoreGame* savefile );

	//! Sets the constant force vector for this force application.
	void SetForce( const idVec3& force );

	//! Sets the position of the constant force.
	void SetPosition( idPhysics* physics, int id, const idVec3& point );

	//! Sets the physics simulation for the constant force.
	void SetPhysics( idPhysics* physics );

public: // common force interface
		//! Applies a constant force to a physics object at the specified time.
	virtual void Evaluate( int time );

	//! Removes the association with the specified physics object if it matches the current physics object.
	virtual void RemovePhysics( const idPhysics* phys );

private:
	// force properties
	idVec3	   force;
	idPhysics* physics;
	int		   id;
	idVec3	   point;
};

#endif /* !__FORCE_CONSTANT_H__ */
