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

#ifndef __PHYSICS_ACTOR_H__
#define __PHYSICS_ACTOR_H__

/*!
	\class idPhysics_Actor
	\brief Manages the physics simulation for actor entities with collision detection and gravity handling.

	The idPhysics_Actor class extends base physics functionality to support actor-specific behaviors including collision detection, gravity simulation, and clip model management. It maintains a single
   collision model for the actor and handles operations such as setting mass, contents, and gravity. The class supports saving and restoring physics states, and provides methods for clipping
   translation and rotation to handle collisions with other objects. The clip model can be linked or unlinked from the physics simulation, and can be disabled or enabled as needed. The class also
   provides access to the actor's origin, axis, and bounds for integration with the game world.

*/
class idPhysics_Actor : public idPhysics_Base
{
public:
	CLASS_PROTOTYPE( idPhysics_Actor );

	//! Initializes a new instance of the idPhysics_Actor class.
	idPhysics_Actor();

	//! Destroys the idPhysics_Actor instance and cleans up its associated clip model.
	~idPhysics_Actor();

	//! Saves the actor physics state to a save file
	void	  Save( idSaveGame* savefile ) const;

	//! Restores the actor physics state from a save file.
	void	  Restore( idRestoreGame* savefile );

	//! Returns the delta yaw of the master entity.
	float	  GetMasterDeltaYaw() const;

	//! Returns the ground entity that the actor is standing on.
	idEntity* GetGroundEntity() const;

	//! Aligns the clip model with the gravity direction.
	void	  SetClipModelAxis();

public:
	// common physics interface ----------------------

	/*!
		\brief Sets the collision model for the actor with the specified density and linking parameters.

		This function assigns a clip model to the actor physics object. It ensures that the provided clip model is valid and is a trace model. If there is an existing clip model that is different from
	   the new one and the freeOld parameter is true, the old model is deleted. The new clip model is then linked to the game world with the actor's origin and orientation.

		\param model The clip model to be used for collision detection
		\param density The density value used for physics calculations
		\param id An identifier for the clip model, defaults to 0
		\param freeOld Flag indicating whether to delete the old clip model if different
		\throws assertion failures if self, model, or density are invalid
	*/
	void			SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model associated with this physics actor.
	idClipModel*	GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models associated with this physics actor, which is always 1.
	int				GetNumClipModels() const;

	//! Sets the mass of the actor physics object, updating the inverse mass accordingly.
	void			SetMass( float mass, int id = -1 );

	//! Returns the mass of the physics actor.
	float			GetMass( int id = -1 ) const;

	//! Sets the contents of the physics actor's clip model.
	void			SetContents( int contents, int id = -1 );

	//! Returns the contents of the clip model associated with this physics actor.
	int				GetContents( int id = -1 ) const;

	//! Returns the bounding box of the physics actor's clip model.
	const idBounds& GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics actor or a specific clip model within it.
	const idBounds& GetAbsBounds( int id = -1 ) const;

	//! Returns true if the actor is pushable, false otherwise.
	bool			IsPushable() const;

	//! Returns the origin position of the physics actor's collision model
	const idVec3&	GetOrigin( int id = 0 ) const;

	//! Returns the axis of the physics actor.
	const idMat3&	GetAxis( int id = 0 ) const;

	//! Sets the gravity vector for the actor physics object.
	void			SetGravity( const idVec3& newGravity );

	//! Returns the gravity axis of the physics actor as a constant reference to an idMat3.
	const idMat3&	GetGravityAxis() const;

	//! Performs translation clipping for the actor physics object, checking for collisions with other objects.
	void			ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;

	//! Performs rotation clipping using either a specified collision model or the actor's own collision model.
	void			ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;

	//! Returns the contents of the physics actor at its current position, optionally clipped against a specified model.
	int				ClipContents( const idClipModel* model ) const;

	//! Disables the collision model for the actor physics object.
	void			DisableClip();

	//! Enables the collision model for the actor physics object.
	void			EnableClip();

	//! Unlinks the clip model from the physics simulation.
	void			UnlinkClip();

	//! Links the actor's clip model into the game's collision system.
	void			LinkClip();

	//! Evaluates and returns true if there are contacts available
	bool			EvaluateContacts();

protected:
	idClipModel*		  clipModel;	 // clip model used for collision detection
	idMat3				  clipModelAxis; // axis of clip model aligned with gravity direction

	// derived properties
	float				  mass;
	float				  invMass;

	// master
	idEntity*			  masterEntity;
	float				  masterYaw;
	float				  masterDeltaYaw;

	// results of last evaluate
	idEntityPtr<idEntity> groundEntityPtr;
};

#endif /* !__PHYSICS_ACTOR_H__ */
