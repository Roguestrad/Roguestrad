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

#ifndef __FORCE_DRAG_H__
#define __FORCE_DRAG_H__

/*!
	\class idForce_Drag
	\brief A force class that applies drag to physics objects, pulling them toward a specified position.

	This class implements a drag force that influences physics objects by pulling them toward a target position. It maintains a reference to a physics object and a drag position, and updates the
   object's velocity based on the damping coefficient. The force is evaluated each frame to apply the appropriate drag effect. The class inherits from idForce and is designed to be used in conjunction
   with physics simulation systems. It supports initialization with damping parameters, setting of physics objects and drag positions, and evaluation of the drag force during simulation updates.

*/
class idForce_Drag : public idForce
{
public:
	CLASS_PROTOTYPE( idForce_Drag );

	//! Initializes a new instance of the idForce_Drag class with default values.
	idForce_Drag();
	virtual ~idForce_Drag();

	//! Initializes the drag force with the specified damping coefficient.
	void		  Init( float damping );

	//! Sets the physics object being dragged.
	void		  SetPhysics( idPhysics* physics, int id, const idVec3& p );

	//! Sets the position to drag towards.
	void		  SetDragPosition( const idVec3& pos );

	//! Returns the position that the drag force is pulling towards.
	const idVec3& GetDragPosition() const;

	//! Returns the position on the dragged physics object.
	const idVec3  GetDraggedPosition() const;

public: // common force interface
		//! Updates the physics properties of a dragged object based on the drag force.
	virtual void Evaluate( int time );

	//! Removes the specified physics object from the drag force if it matches the current physics object.
	virtual void RemovePhysics( const idPhysics* phys );

private:
	// properties
	float	   damping;

	// positioning
	idPhysics* physics;		 // physics object
	int		   id;			 // clip model id of physics object
	idVec3	   p;			 // position on clip model
	idVec3	   dragPosition; // drag towards this position
};

#endif /* !__FORCE_DRAG_H__ */
