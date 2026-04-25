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

#ifndef __GAME_BRITTLEFRACTURE_H__
#define __GAME_BRITTLEFRACTURE_H__

/*
===============================================================================

B-rep Brittle Fracture - Static entity using the boundary representation
of the render model which can fracture.

===============================================================================
*/

typedef struct shard_s {
	idClipModel*								 clipModel;
	idFixedWinding								 winding;
	idList<idFixedWinding*, TAG_PHYSICS_BRITTLE> decals;
	idList<bool>								 edgeHasNeighbour;
	idList<struct shard_s*, TAG_PHYSICS_BRITTLE> neighbours;
	idPhysics_RigidBody							 physicsObj;
	int											 droppedTime;
	bool										 atEdge;
	int											 islandNum;
} shard_t;

/*!
	\class idBrittleFracture
	\brief Manages the destruction and rendering of brittle fracture effects in the game world.

	This class implements the behavior for objects that break into fragments when damaged or impacted. It handles the creation, management, and rendering of fracture shards, including physics
   simulation, rendering updates, and network synchronization. The class supports both server and client-side operations, with separate logic for handling damage effects, shattering events, and visual
   updates. It maintains a collection of shard data structures that represent individual fracture pieces, and provides mechanisms for applying forces, removing shards, and triggering breaking
   animations. The system supports various fracture patterns and can be activated through touch events or explicit activation commands. The class integrates with the engine's save/load system to
   maintain fracture state across game sessions.

*/
class idBrittleFracture : public idEntity
{
public:
	CLASS_PROTOTYPE( idBrittleFracture );

	//! Initializes a new instance of the idBrittleFracture class with default values.
	idBrittleFracture();

	//! Destroys the brittle fracture object and frees associated resources.
	virtual ~idBrittleFracture();

	//! Saves the brittle fracture state to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the brittle fracture state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the brittle fracture object by loading properties and setting up fracture geometry.
	void		 Spawn();

	//! Renders the brittle fracture entity to the game world.
	virtual void Present();

	//! Updates the brittle fracture effect by managing shard lifecycles and physics.
	virtual void Think();

	/*!
		\brief Applies an impulse to a specific shard of the brittle fracture system or triggers shattering if conditions are met.

		This function handles the application of a force impulse to a specific shard identified by its index. If the shard has already been dropped, it directly applies the impulse to the shard's
	   physics object. If the shard hasn't been dropped yet but the overall health is zero and fracturing is not disabled, it triggers the shattering of the fracture system. The function checks for
	   valid shard indices and ensures proper state management before applying the impulse or initiating shattering.

		\param ent Pointer to the entity associated with the fracture
		\param id Index of the shard to apply the impulse to
		\param point The point in world space where the impulse is applied
		\param impulse The impulse vector to be applied to the shard
	*/
	virtual void ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );

	/*!
		\brief Adds a force to a specific shard or triggers shattering based on the entity's health and fracture state.

		This function applies a force to a shard identified by the given index if the shard has already been dropped. If the shard hasn't been dropped yet but the entity's health is depleted and
	   fracturing is not disabled, it triggers the shattering process at the specified point with the given force.

		\param ent Pointer to the entity that the force is being applied to
		\param id Index of the shard to apply the force to
		\param point The position where the force is applied
		\param force The force vector to be applied
	*/
	virtual void AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );

	//! Adds a damage effect to the fracture system at the collision point.
	virtual void AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );

	/*!
		\brief Handles the destruction event when the brittle fracture object is killed.

		This function is called when the brittle fracture object receives a death event. It checks if the fracture is not disabled, and if so, activates any target entities and triggers the breaking
	   animation or effect. The function does not perform any action if the fracture is disabled.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage inflicted
		\param dir The direction from which the damage came
		\param location The location on the entity where the damage was applied
	*/
	virtual void Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	/*!
		\brief Projects a decal onto fracture shards using the specified point, direction, time, and damage definition.

		This function handles the projection of decals onto fracture shards when a damage event occurs. It first sends the decal event to the server if running as a network server, then stores the
	   event for potential use after loading a saved game. If the time is greater than or equal to the current game time, it attempts to play a sound based on the damage definition. The function
	   calculates texture coordinates for the decal by projecting the specified point and direction onto each shard's winding, and creates a decal for each shard that intersects with the projection.
	   Finally, it marks the fracture as active for visual updates.

		\param point The 3D point where the decal projection originates
		\param dir The 3D direction vector of the decal projection
		\param time The time when the decal effect should be applied
		\param damageDefName The name of the damage definition used to determine the sound to play
	*/
	void		 ProjectDecal( const idVec3& point, const idVec3& dir, const int time, const char* damageDefName );

	//! Returns true if the brittle fracture object is broken.
	bool		 IsBroken() const;

	enum { EVENT_PROJECT_DECAL = idEntity::EVENT_MAXEVENTS, EVENT_SHATTER, EVENT_MAXEVENTS };

	//! Updates the brittle fracture simulation on the client side.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Performs client-side prediction thinking for brittle fracture effects.
	virtual void ClientPredictionThink();

	//! Handles client-side events for brittle fracture effects such as decal projection and shattering.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

private:
	// setttings
	const idMaterial* material;
	const idMaterial* decalMaterial;
	float			  decalSize;
	float			  maxShardArea;
	float			  maxShatterRadius;
	float			  minShatterRadius;
	float			  linearVelocityScale;
	float			  angularVelocityScale;
	float			  shardMass;
	float			  density;
	float			  friction;
	float			  bouncyness;
	idStr			  fxFracture;

	struct fractureEvent_s {
		int	   eventType;
		idVec3 point;
		idVec3 vector;
	};
	idList<fractureEvent_s>				  storedEvents;
	bool								  processStoredEvents;
	idRenderModel*						  defaultRenderModel;
	bool								  isXraySurface;

	// state
	idPhysics_StaticMulti				  physicsObj;
	idList<shard_t*, TAG_PHYSICS_BRITTLE> shards;
	idBounds							  bounds;
	bool								  disableFracture;

	// for rendering
	mutable int							  lastRenderEntityUpdate;
	mutable bool						  changed;

	//! Updates the render entity for brittle fracture effects based on the current state and view.
	bool								  UpdateRenderEntity( renderEntity_s* renderEntity, const renderView_t* renderView ) const;

	//! Updates the render entity for a brittle fracture effect based on the current render view.
	static bool							  ModelCallback( renderEntity_s* renderEntity, const renderView_t* renderView );

	//! Adds a new shard to the brittle fracture system using the specified clip model and winding.
	void								  AddShard( idClipModel* clipModel, idFixedWinding& w );

	//! Removes a shard at the specified index from the brittle fracture system
	void								  RemoveShard( int index );

	/*!
		\brief Drops a shard from the brittle fracture system, applying physics and removing it from the static structure.

		This function handles the removal of a shard from the brittle fracture system. It removes decals, breaks neighbour links, clears neighbour pointers, and sets up physics properties for the
	   shard. The shard is then given an impulse and angular velocity based on the provided direction and impulse values, and is marked as dropped for fading purposes. The function also ensures the
	   shard is activated for physics simulation.

		\param shard Pointer to the shard to be dropped
		\param point The point of impact used to calculate the direction and distance for physics effects
		\param dir The direction of the impulse applied to the shard
		\param impulse The magnitude of the impulse applied to the shard
		\param time The current time used to set the dropped time for fading effects
	*/
	void								  DropShard( shard_t* shard, const idVec3& point, const idVec3& dir, const float impulse, const int time );

	//! Initiates the shattering effect of the brittle fracture object at the specified point with the given impulse and time.
	void								  Shatter( const idVec3& point, const idVec3& impulse, const int time );

	//! Drops floating islands of fractured shards that are not connected to the world edges
	void								  DropFloatingIslands( const idVec3& point, const idVec3& impulse, const int time );

	//! Destroys the brittle fracture object by disabling damage and updating its physics contents.
	void								  Break();

	//! Recursively fractures a winding into smaller shards using a random split plane selection strategy.
	void								  Fracture_r( idFixedWinding& w, idRandom2& random );

	//! Creates fractures for the specified render model.
	void								  CreateFractures( const idRenderModel* renderModel );

	//! Finds neighboring shards for each shard in the brittle fracture simulation
	void								  FindNeighbours();

	//! Enables fracture and triggers breaking if health is depleted.
	void								  Event_Activate( idEntity* activator );

	//! Handles touch events for brittle fracture objects, triggering shattering when certain conditions are met.
	void								  Event_Touch( idEntity* other, trace_t* trace );
};

#endif /* !__GAME_BRITTLEFRACTURE_H__ */
