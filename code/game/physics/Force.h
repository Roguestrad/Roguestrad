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

#ifndef __FORCE_H__
#define __FORCE_H__

/*
===============================================================================

	Force base class

	A force object applies a force to a physics object.

===============================================================================
*/

class idEntity;
class idPhysics;

/*!
	\class idForce
	\brief Base class for implementing force fields that can be applied to physics objects.

	This class serves as an abstract base for force implementations that can be applied to physics objects within the simulation. It maintains a list of active forces and provides mechanisms for
   managing these forces, including adding, removing, and evaluating them over time. The class is designed to be inherited by concrete force implementations that define specific force behaviors. The
   force list management allows for efficient cleanup when physics objects are removed or deleted, ensuring proper resource management. The Evaluate method provides the interface for force computation
   at specific time steps, enabling integration with the physics simulation.

*/
class idForce : public idClass
{
public:
	CLASS_PROTOTYPE( idForce );

	//! Initializes an idForce object and appends it to the force list.
	idForce();

	//! Destructor for the idForce class that removes the force from the force list.
	virtual ~idForce();

	//! Removes a physics object from all force list entries.
	static void DeletePhysics( const idPhysics* phys );

	//! Clears all forces from the force list.
	static void ClearForceList();

public:
	// common force interface ---------------------

	//! Evaluates the force up to the given time.
	virtual void Evaluate( int time );

	//! Removes any pointers to the specified physics object.
	virtual void RemovePhysics( const idPhysics* phys );

private:
	static idList<idForce*, TAG_IDLIB_LIST_PHYSICS> forceList;
};

#endif /* !__FORCE_H__ */
