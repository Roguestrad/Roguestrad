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

#ifndef __GAME_MOVEABLE_H__
#define __GAME_MOVEABLE_H__

/*
===============================================================================

  Entity using rigid body physics.

===============================================================================
*/

extern const idEventDef EV_BecomeNonSolid;
extern const idEventDef EV_IsAtRest;

/*!
	\class idMoveable
	\brief Represents a moveable entity that can follow spline paths, handle collisions, and manage physics states.

	This class implements a moveable entity that can follow spline paths, handle collisions, and manage various physics states. It inherits from idEntity and provides functionality for spawning with
   physics properties, saving and restoring state, and handling destruction events. The class supports spline-based movement initialization and follows an initial spline path. It includes methods for
   handling damage, collision events, visibility management, and interaction with other entities. The class also supports client-side prediction, snapshot serialization, and various activation events
   that control the entity's behavior and state transitions. The moveable object can become non-solid while maintaining bullet collision detection and can handle breaking animations and explosion
   effects upon destruction.

*/
class idMoveable : public idEntity
{
public:
	CLASS_PROTOTYPE( idMoveable );

	//! Initializes a new instance of the idMoveable class.
	idMoveable();

	//! Destructor for the idMoveable class that cleans up the initialSpline member.
	~idMoveable();

	//! Initializes the moveable object's physics and collision properties based on spawn arguments.
	void			Spawn();

	//! Saves the moveable object's state to a save game file.
	void			Save( idSaveGame* savefile ) const;

	//! Restores the moveable object's state from a save file.
	void			Restore( idRestoreGame* savefile );

	//! Executes the moveable entity's think logic, including spline path following and activity management.
	virtual void	Think();

	//! Updates the moveable object's state for client rendering and prediction.
	virtual void	ClientThink( const int curTime, const float fraction, const bool predict );

	//! Hides the moveable entity and clears its physics contents.
	virtual void	Hide();

	//! Makes the moveable entity visible and sets its physics contents to solid if it is not non-solid.
	virtual void	Show();

	//! Returns whether stepping is allowed for this moveable object.
	bool			AllowStep() const;

	//! Enables or disables damage for the moveable object with an optional duration.
	void			EnableDamage( bool enable, float duration );

	//! Handles collision events for moveable entities, including sound, damage, and visual effects
	virtual bool	Collide( const trace_t& collision, const idVec3& velocity );

	/*!
		\brief Handles the destruction of a moveable entity when it is killed by an attacker.

		This function manages the actions that occur when a moveable entity is destroyed. It first checks if the entity should unbind from its parent when killed, and performs the unbinding if
	   necessary. If a broken model is specified, it replaces the current model with the broken one. If the entity is configured to explode, it schedules removal after a delay if no broken model is
	   set. It also clears any GUI associated with the entity, activates any targets linked to this entity, and disables further damage intake.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage inflicted
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
	*/
	virtual void	Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Writes the moveable object's physics state to a snapshot message.
	virtual void	WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the moveable object's state from a snapshot message and updates visuals if changes are detected.
	virtual void	ReadFromSnapshot( const idBitMsg& msg );

	//! Sets the attacker entity for this moveable object.
	void			SetAttacker( idEntity* ent );

	//! Returns the entity that attacked this moveable object.
	const idEntity* GetAttacker() { return attacker; }

protected:
	idPhysics_RigidBody		physicsObj;	 // physics object
	idStr					brokenModel; // model set when health drops down to or below zero
	idStr					damage;		 // if > 0 apply damage to hit entities
	idStr					monsterDamage;
	idEntity*				attacker;
	idStr					fxCollide;		   // fx system to start when collides with something
	int						nextCollideFxTime; // next time it is ok to spawn collision fx
	float					minDamageVelocity; // minimum velocity before moveable applies damage
	float					maxDamageVelocity; // velocity at which the maximum damage is applied
	idCurve_Spline<idVec3>* initialSpline;	   // initial spline path the moveable follows
	idVec3					initialSplineDir;  // initial relative direction along the spline path
	bool					explode;		   // entity explodes when health drops down to or below zero
	bool					unbindOnDeath;	   // unbind from master when health drops down to or below zero
	bool					allowStep;		   // allow monsters to step on the object
	bool					canDamage;		   // only apply damage when this is set
	int						nextDamageTime;	   // next time the movable can hurt the player
	int						nextSoundTime;	   // next time the moveable can make a sound

	//! Returns the material used by the render model for this moveable object.
	const idMaterial*		GetRenderModelMaterial() const;

	//! Makes the moveable object non-solid while maintaining bullet collision detection.
	void					BecomeNonSolid();

	//! Initializes the initial spline for the moveable object with the specified start time.
	void					InitInitialSpline( int startTime );

	//! Updates the moveable's physics based on the initial spline path.
	bool					FollowInitialSplinePath();

	//! Activates the moveable entity, initializing its physics state and applying initial velocities if specified.
	void					Event_Activate( idEntity* activator );

	//! Makes the moveable object non-solid.
	void					Event_BecomeNonSolid();

	//! Sets the owner of the moveable object from spawn arguments.
	void					Event_SetOwnerFromSpawnArgs();

	//! Returns whether the moveable object is at rest.
	void					Event_IsAtRest();

	//! Enables or disables damage for the moveable object based on the enable flag.
	void					Event_EnableDamage( float enable );
};

/*!
	\class idBarrel
	\brief The idBarrel class represents a movable object that can be spawned, saved, restored, and updated during gameplay.

	The idBarrel class extends idMoveable to provide functionality for a barrel entity within the game world. It handles the initialization of the barrel's physical properties and state through the
   Spawn method, and supports serialization through Save and Restore methods for game persistence. The class updates its state through BarrelThink and Think methods, with the latter incorporating
   spline path following and other barrel-specific logic. Client-side updates are handled by ClientThink, which uses interpolation and presentation for visual accuracy. The GetPhysicsToVisualTransform
   method enables accurate positioning of the visual representation relative to the physics simulation. The class follows standard idTech game object patterns with prototype and state management
   capabilities.

*/
class idBarrel : public idMoveable
{
public:
	CLASS_PROTOTYPE( idBarrel );

	//! Constructs a new idBarrel object with default values.
	idBarrel();

	//! Initializes the barrel's physical properties and state.
	void		 Spawn();

	//! Saves the barrel's state to the given save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the barrel's state from a saved game file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the barrel's visual state based on its physics simulation
	void		 BarrelThink();

	//! Executes the barrel's think logic, including spline path following and barrel-specific updates.
	virtual void Think();

	//! Returns the transformation from physics to visual space for the barrel.
	virtual bool GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );

	//! Updates the barrel's state on the client side using interpolation and presentation.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

private:
	float  radius;			   // radius of barrel
	int	   barrelAxis;		   // one of the coordinate axes the barrel cylinder is parallel to
	idVec3 lastOrigin;		   // origin of the barrel the last think frame
	idMat3 lastAxis;		   // axis of the barrel the last think frame
	float  additionalRotation; // additional rotation of the barrel about it's axis
	idMat3 additionalAxis;	   // additional rotation axis
};

/*!
	\class idExplodingBarrel
	\brief Manages exploding barrels with burn and explosion effects.

	This class implements the behavior of exploding barrels, including stability management, burning states, and destruction effects. It handles damage application, explosion logic, particle and light
   effects, and client-server synchronization. The class inherits from idBarrel and extends its functionality to support realistic destruction sequences with visual and audio feedback. It provides
   methods for controlling the barrel's state, managing its interaction with damage sources, and coordinating multiplayer gameplay aspects such as snapshot synchronization and client events. The class
   supports both immediate explosion and delayed burning behaviors, and can trigger associated targets upon destruction.

*/
class idExplodingBarrel : public idBarrel
{
public:
	CLASS_PROTOTYPE( idExplodingBarrel );

	//! Initializes a new instance of the idExplodingBarrel class with default values.
	idExplodingBarrel();

	//! Destructor for the idExplodingBarrel class that cleans up allocated resources.
	~idExplodingBarrel();

	//! Initializes the exploding barrel's properties and physics state.
	void		 Spawn();

	//! Saves the exploding barrel state to the given save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the exploding barrel state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Returns the stability state of the exploding barrel.
	bool		 IsStable();

	//! Sets the stability state of the exploding barrel.
	void		 SetStability( bool stability );

	//! Initializes the exploding barrel to start burning.
	void		 StartBurning();

	//! Stops the burning effect of the exploding barrel
	void		 StopBurning();

	//! Updates the exploding barrel's light, interpolates its physics, and presents it during client gameplay.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Executes the thinking logic for an exploding barrel, handling its state and visual effects.
	virtual void Think();

	/*!
		\brief Applies damage to the exploding barrel entity using the specified damage definition and parameters, potentially triggering an explosion.

		This function processes damage applied to an exploding barrel. It retrieves the damage definition from the game's entity dictionary and checks if the damage has a radius attribute. If the
	   barrel has contents and is not bound to another entity, and the damage has a radius, it schedules an explosion event 400 milliseconds in the future. Otherwise, it delegates the damage
	   application to the base entity damage handling. The function will cause a game error if the specified damage definition is not found.

		\param inflictor entity that caused the damage
		\param attacker entity that initiated the attack
		\param dir direction vector of the damage
		\param damageDefName name of the damage definition to use
		\param damageScale scale factor to apply to the base damage
		\param location location identifier where the damage was applied
		\throws gameLocal.Error when the damage definition is not found
	*/
	virtual void Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );

	/*!
		\brief Handles the destruction of an exploding barrel, triggering explosions, damage, and debris effects.

		This function processes the death of an exploding barrel entity. It checks if the barrel is already in a destroyed or burning state, and if not, it determines whether to burn the barrel or
	   explode it immediately. If burning is specified, it sets the state to burning and schedules an explosion event. If not burning, it sets the state to exploding, hides the barrel, and applies
	   radius damage to nearby entities. It then generates explosion effects, creates debris particles, and schedules the removal of the barrel. If the barrel is configured to trigger targets, it
	   activates those targets. Additionally, it awards the 'Boomtastic' achievement to the local player if the game is not in multiplayer mode.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage Amount of damage inflicted
		\param dir Direction of the damage
		\param location Location of the damage
	*/
	virtual void Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Writes the exploding barrel's state to a snapshot message.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the exploding barrel's state from a snapshot message, including visibility.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

	//! Handles client-side events for the exploding barrel, specifically processing explosion events.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	enum { EVENT_EXPLODE = idEntity::EVENT_MAXEVENTS, EVENT_MAXEVENTS };

private:
	typedef enum { NORMAL = 0, BURNING, BURNEXPIRED, EXPLODING } explode_state_t;
	explode_state_t state;

	idVec3			spawnOrigin;
	idMat3			spawnAxis;
	qhandle_t		particleModelDefHandle;
	qhandle_t		lightDefHandle;
	renderEntity_t	particleRenderEntity;
	renderLight_t	light;
	int				particleTime;
	int				lightTime;
	float			time;
	bool			isStable;

	//! Adds particles to the exploding barrel effect using the specified particle model name and burn flag.
	void			AddParticles( const char* name, bool burn );

	//! Adds a light to the exploding barrel with the specified name and burn state.
	void			AddLight( const char* name, bool burn );

	//! Plays explosion sounds and displays visual effects for the exploding barrel.
	void			ExplodingEffects();

	//! Updates the light effect for a burning exploding barrel.
	void			UpdateLight();

	//! Activates the exploding barrel, causing it to explode.
	void			Event_Activate( idEntity* activator );

	//! Respawns the exploding barrel after checking the minimum respawn distance from players.
	void			Event_Respawn();

	//! Triggers the explosion event for the exploding barrel, changing its state and calling the killed function.
	void			Event_Explode();

	//! Triggers targets associated with the exploding barrel.
	void			Event_TriggerTargets();
};

#endif /* !__GAME_MOVEABLE_H__ */
