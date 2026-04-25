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

#ifndef __GAME_MISC_H__
#define __GAME_MISC_H__

/*!
	\class idSpawnableEntity
	\brief A spawnable entity that represents a simple, static model without functional behavior.
*/
class idSpawnableEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idSpawnableEntity );

	//! Initializes the spawnable entity and prepares it to hold dictionary information.
	void Spawn();

private:
};

/*!
	\class idPlayerStart
	\brief Manages player teleportation start points with teleportation stages and effects.

	The idPlayerStart class represents entities that serve as teleportation sources for players, handling the teleportation process including stage management, visual effects, and audio feedback. It
   maintains a teleport stage state that tracks the progress of teleportation operations. The class supports both server and client side operations, with methods for saving and restoring teleportation
   state during game sessions. Teleportation involves coordinating with player entities to move them to new locations while managing associated visual and auditory effects. The class inherits from
   idEntity, integrating into the engine's entity system for proper registration and update handling during gameplay.

*/
class idPlayerStart : public idEntity
{
public:
	CLASS_PROTOTYPE( idPlayerStart );

	enum { EVENT_TELEPORTPLAYER = idEntity::EVENT_MAXEVENTS, EVENT_MAXEVENTS };

	//! Constructs an idPlayerStart object and initializes the teleport stage to zero.
	idPlayerStart();

	//! Initializes the player start entity by setting the teleport stage to zero.
	void		 Spawn();

	//! Saves the player start teleport stage to the given save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the teleportation stage state from the save file.
	void		 Restore( idRestoreGame* savefile );

	//! Handles client-side events for player start entities, specifically teleportation events.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

private:
	int	 teleportStage;

	//! Teleports the player to the location of the player start entity.
	void Event_TeleportPlayer( idEntity* activator );

	//! Handles the teleportation stage for a player entity, managing visual and audio effects during the teleport process.
	void Event_TeleportStage( idEntity* player );

	//! Teleports a player to the teleporter location with optional visual effects and push velocity.
	void TeleportPlayer( idPlayer* player );
};

/*!
	\class idActivator
	\brief Manages entity activation states and trigger interactions.
*/
class idActivator : public idEntity
{
public:
	CLASS_PROTOTYPE( idActivator );

	//! Initializes the activator entity based on spawn arguments and physics properties.
	void		 Spawn();

	//! Saves the activator state to the given save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the activation state from a saved game file.
	void		 Restore( idRestoreGame* savefile );

	//! Executes the activator's thinking logic, including physics simulation and trigger touching.
	virtual void Think();

private:
	bool stay_on;

	//! Toggles the activation state of the activator entity.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idPathCorner
	\brief A path corner entity that manages navigation waypoints and random path selection.
*/
class idPathCorner : public idEntity
{
public:
	CLASS_PROTOTYPE( idPathCorner );

	//! Initializes the path corner entity.
	void				 Spawn();

	//! Draws debug information for path corner entities in the game world.
	static void			 DrawDebugInfo();

	//! Returns a randomly selected path corner entity from the targets of the source entity, excluding any ignored entity.
	static idPathCorner* RandomPath( const idEntity* source, const idEntity* ignore );

private:
	//! Returns a random path entity from the available paths.
	void Event_RandomPath();
};

/*!
	\class idDamagable
	\brief A class representing entities that can sustain damage and transition to a broken state.

	The idDamagable class extends idEntity to provide functionality for entities that can take damage, be destroyed, and transition to a broken state. It manages the entity's health, visibility, and
   physics properties throughout its lifecycle. The class supports saving and restoring the entity's state, handling death events, and controlling the transition to broken states through explicit
   methods and events. It provides mechanisms to hide and show the entity, as well as to restore its damageable state. The class is designed to work within a game engine framework where entities can
   be interacted with through damage and destruction mechanics.

*/
class idDamagable : public idEntity
{
public:
	CLASS_PROTOTYPE( idDamagable );

	//! Initializes a new instance of the idDamagable class.
	idDamagable();

	//! Saves the damagable object's state to the provided save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the damageable object's state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the damageable entity's properties and physics setup.
	void		 Spawn();

	/*!
		\brief Handles the death of a damagable entity by another entity.

		This function processes the death of a damagable entity, checking if the entity can be killed at the current time. If the entity is killed, it triggers the broken state.

		\param inflictor The entity that caused the damage
		\param attacker The entity that attacked
		\param damage The amount of damage dealt
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
	*/
	void		 Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Hides the damagable entity and clears its physics contents.
	virtual void Hide();

	//! Makes the damagable entity visible and sets its physics contents to solid.
	virtual void Show();

private:
	int	 count;
	int	 nextTriggerTime;

	//! Sets the damagable object to a broken state and triggers associated behavior.
	void BecomeBroken( idEntity* activator );

	//! Makes the damagable entity become broken.
	void Event_BecomeBroken( idEntity* activator );

	//! Restores the damagable object's health and makes it visible.
	void Event_RestoreDamagable();
};

/*!
	\class idExplodable
	\brief A class representing entities that can be explodable and trigger explosion effects.
*/
class idExplodable : public idEntity
{
public:
	CLASS_PROTOTYPE( idExplodable );

	//! Initializes the explodable object by hiding it.
	void Spawn();

private:
	//! Causes the explodable entity to explode, applying damage and triggering visual effects.
	void Event_Explode( idEntity* activator );
};

/*!
	\class idSpring
	\brief A physics entity that simulates a spring connection between two entities.
*/
class idSpring : public idEntity
{
public:
	CLASS_PROTOTYPE( idSpring );

	//! Initializes the spring properties and sets up the spring connection between two entities.
	void		 Spawn();

	//! Updates the spring physics and renders the spring line.
	virtual void Think();

private:
	idEntity*	   ent1;
	idEntity*	   ent2;
	int			   id1;
	int			   id2;
	idVec3		   p1;
	idVec3		   p2;
	idForce_Spring spring;

	//! Links the spring to two entities specified by spawn arguments.
	void		   Event_LinkSpring();
};

/*!
	\class idForceField
	\brief A force field entity that can be activated, toggled, and has save/load functionality.

	The idForceField class represents an interactive force field entity that can be toggled on and off, activated with a delay, and managed through various events. It inherits from idEntity and
   includes methods for saving and restoring its state, initializing properties during spawning, and evaluating its behavior during gameplay. The class supports both server-side and client-side
   thinking, allowing for dynamic updates and presentation of the force field. The force field can be activated through events and has a toggle mechanism to switch its active state. It also handles
   target finding functionality for determining relevant entities.

*/
class idForceField : public idEntity
{
public:
	CLASS_PROTOTYPE( idForceField );

	//! Saves the force field data to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the force field state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the force field properties and collision settings based on spawn arguments.
	void		 Spawn();

	//! Evaluates the force field at the current time if thinking is enabled, and presents the force field.
	virtual void Think();

	//! Evaluates the force field and presents the results.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

private:
	idForce_Field forceField;

	//! Toggles the active state of the force field.
	void		  Toggle();

	//! Activates the force field and schedules a toggle event after a specified delay.
	void		  Event_Activate( idEntity* activator );

	//! Toggles the state of the force field.
	void		  Event_Toggle();

	//! Finds and processes targets for the force field.
	void		  Event_FindTargets();
};

/*!
	\class idAnimated
	\brief A class that manages animated entities with support for ragdoll physics, animation sequencing, and missile launching.

	This class extends idAFEntity_Gibbable to provide functionality for animated entities that can play sequences of animations, switch to ragdoll physics, launch projectiles, and handle various
   animation-related events. The class is designed to be spawned with specific animation and physics properties, and can be activated to trigger animations or ragdoll behavior. It supports saving and
   restoring of animation states, and includes methods for playing animations, handling footstep events, and launching missiles from specified joints. The class manages its own animation state and can
   integrate with physics simulation through the use of animation frameworks and ragdoll physics. The entity can be configured through spawn arguments and provides events for external triggering of
   various animation and physics behaviors.

*/
class idAnimated : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idAnimated );

	//! Initializes all member variables to their default values.
	idAnimated();

	//! Destructor for the idAnimated class that cleans up the combat model.
	~idAnimated();

	//! Saves the animated object's state to the provided save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the animated state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the animated entity's animation state and properties based on spawn arguments.
	void		 Spawn();

	//! Loads the ragdoll animation file for the animated object.
	virtual bool LoadAF();

	//! Starts the ragdoll physics for the animated object if an animation framework is loaded and active.
	bool		 StartRagdoll();

	//! Returns the transformation from physics to sound space for the animated entity.
	virtual bool GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );

private:
	int					  num_anims;
	int					  current_anim_index;
	int					  anim;
	int					  blendFrames;
	jointHandle_t		  soundJoint;
	idEntityPtr<idEntity> activator;
	bool				  activated;
	int					  achievement;

	//! Plays the next animation in the sequence
	void				  PlayNextAnim();

	//! Handles the activation event for an animated entity, triggering animations and achievements.
	void				  Event_Activate( idEntity* activator );

	//! Starts the animation playback for the animated entity.
	void				  Event_Start();

	//! Starts the ragdoll physics simulation for this animated object.
	void				  Event_StartRagdoll();

	//! Handles the completion of an animation event for the animated entity.
	void				  Event_AnimDone( int animIndex );

	//! Handles the footstep event by starting a footstep sound.
	void				  Event_Footstep();

	/*!
		\brief Launches missiles from the animated entity using specified projectile, sound, and joint parameters.

		This function initializes the missile launching process by validating the projectile definition and joint handles. It sets up the spawn arguments with the projectile name and sound, then
	   schedules the missile launching update event. The function will warn if the projectile definition is not found or if the launch joint is invalid, and will error if the target joint is invalid.
	   The missile launching is controlled by the number of shots and frame delay parameters.

		\param projectilename Name of the projectile entity definition to use for missile creation
		\param sound Sound to play when launching missiles
		\param launchjoint Name of the joint to launch missiles from
		\param targetjoint Name of the joint to target missiles at
		\param numshots Number of missiles to launch
		\param framedelay Delay in frames between missile launches
	*/
	void				  Event_LaunchMissiles( const char* projectilename, const char* sound, const char* launchjoint, const char* targetjoint, int numshots, int framedelay );

	/*!
		\brief Updates missile launching by creating and launching projectiles at a target position from a specified joint.

		This function handles the updating phase of a missile launching event. It retrieves the launch and target joint positions, calculates the direction vector, and spawns a projectile entity. The
	   projectile is then launched from the launch position toward the target. If multiple shots are specified, the function schedules itself to be called again after a specified delay to continue the
	   launching sequence.

		\param launchjoint The handle of the joint from which the missile is launched
		\param targetjoint The handle of the joint that serves as the target for the missile
		\param numshots The number of shots to be launched
		\param framedelay The delay in game frames between successive shots
	*/
	void				  Event_LaunchMissilesUpdate( int launchjoint, int targetjoint, int numshots, int framedelay );

	//! Sets the animation for the animated entity by name.
	void				  Event_SetAnimation( const char* animName );

	//! Returns the length of the current animation in seconds.
	void				  Event_GetAnimationLength();
};

/*!
	\class idStaticEntity
	\brief Represents a static entity with save/restore functionality and visual state management.

	This class implements a static entity that can be saved and restored, with capabilities to show, hide, fade, and update its visual properties. It supports network synchronization through snapshot
   messages and handles activation events to toggle its active state. The entity maintains its state across game sessions and can be managed through editing dialogs. The class inherits from idEntity,
   extending its functionality with specific behaviors for static objects.

*/
class idStaticEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idStaticEntity );

	//! Initializes a new instance of the idStaticEntity class.
	idStaticEntity();

	//! Saves the static entity's state to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the static entity's state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the static entity based on spawn arguments and sets its properties.
	void		 Spawn();

	//! Displays the editing dialog for the static entity.
	void		 ShowEditingDialog();

	//! Hides the static entity and clears its physics contents.
	virtual void Hide();

	//! Makes the static entity visible and sets its physics contents to solid if specified.
	virtual void Show();

	//! Fades the entity color from the current color to the specified target color over the given time period.
	void		 Fade( const idVec4& to, float fadeTime );

	//! Executes the thinking behavior for the static entity, handling GUI updates and fading effects.
	virtual void Think();

	//! Writes the entity's state to a snapshot message for network synchronization.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads entity state from a snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

private:
	//! Toggles the active state of the static entity and updates its visual properties.
	void   Event_Activate( idEntity* activator );

	int	   spawnTime;
	bool   active;
	idVec4 fadeFrom;
	idVec4 fadeTo;
	int	   fadeStart;
	int	   fadeEnd;
	bool   runGui;
};

/*!
	\class idFuncEmitter
	\brief Manages particle emission effects with activation and persistence capabilities.

	The idFuncEmitter class represents a particle emitter entity that can be activated and deactivated during gameplay. It inherits from idStaticEntity and manages the state of particle effects
   through spawn arguments and scripting events. The class supports saving and restoring its hidden state, which determines whether the emitter is active or inactive. It handles particle rendering
   through snapshot messaging for network synchronization. The emitter's initial state is determined by the 'start_off' spawn argument, controlling whether it begins active or inactive. Activation
   events control the emitter's behavior, toggling particle rendering on or off based on its configuration.

*/
class idFuncEmitter : public idStaticEntity
{
public:
	CLASS_PROTOTYPE( idFuncEmitter );

	//! Constructs a new idFuncEmitter object with hidden set to false.
	idFuncEmitter();

	//! Saves the hidden state of the function emitter to the specified save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the hidden state of the function emitter from the save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the particle emitter state based on the 'start_off' spawn argument.
	void		 Spawn();

	//! Activates the particle emitter effect, starting or stopping particle rendering based on spawn arguments.
	void		 Event_Activate( idEntity* activator );

	//! Writes the emitter's state to a snapshot message
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads particle system state from a snapshot message
	virtual void ReadFromSnapshot( const idBitMsg& msg );

private:
	bool hidden;
};

/*!
	\class idFuncShootProjectile
	\brief A class that manages the spawning and activation of projectiles from a static entity.

	This class extends idStaticEntity to provide functionality for shooting projectiles at specified intervals or upon activation. It handles the configuration of projectile parameters such as speed
   and direction, as well as respawn timing. The class supports serialization for saving and restoring its state during game sessions. It can be activated to launch projectiles and manages its own
   thinking logic to control when projectiles are spawned.

*/
class idFuncShootProjectile : public idStaticEntity
{
public:
	CLASS_PROTOTYPE( idFuncShootProjectile );

	//! Initializes an idFuncShootProjectile object with default values for respawn delay, respawn time, shoot speed, and shoot direction.
	idFuncShootProjectile();

	//! Saves the projectile shooting configuration to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the shoot projectile function from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the projectile shooting function entity.
	void		 Spawn();

	//! Activates or deactivates the projectile shooting function, triggering projectile launch or respawn timing.
	void		 Event_Activate( idEntity* activator );

	//! Executes the projectile spawning logic when the entity is triggered to think
	virtual void Think();

	//! Writes projectile shooting data to a network snapshot message
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads snapshot data from a bit message for projectile shooting functionality.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

private:
	int					  mRespawnDelay;
	int					  mRespawnTime;
	float				  mShootSpeed;
	idVec3				  mShootDir;
	idStr				  mEntityDefName;
	idEntityPtr<idEntity> mLastProjectile;
};

/*!
	\class idFuncSmoke
	\brief Represents a smoke effect entity that manages particle animations and persistence.

	This class implements a smoke effect entity that integrates with the game's entity system. It handles the initialization, rendering, and updating of smoke particle effects. The entity can be
   activated to start or stop smoke animation, and it supports saving and restoring its state during game sessions. The smoke effect is controlled through spawn arguments and can be triggered by
   activation events. The class inherits from idEntity, indicating its integration into the engine's entity framework.

*/
class idFuncSmoke : public idEntity
{
public:
	CLASS_PROTOTYPE( idFuncSmoke );

	//! Initializes a new instance of the idFuncSmoke class with default values.
	idFuncSmoke();

	//! Initializes the smoke effect entity based on spawn arguments and starts or stops the smoke animation.
	void		 Spawn();

	//! Saves the smoke effect data to a save game file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the smoke effect state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the smoke particle effects for the entity.
	virtual void Think();

	//! Handles the activation event for smoke functionality, controlling particle updates and timing.
	void		 Event_Activate( idEntity* activator );

private:
	int					  smokeTime;
	const idDeclParticle* smoke;
	bool				  restart;
};

/*!
	\class idFuncSplat
	\brief A class that manages splat effects and decals for surface interactions.
*/
class idFuncSplat : public idFuncEmitter
{
public:
	CLASS_PROTOTYPE( idFuncSplat );

	//! Constructs an instance of the idFuncSplat class.
	idFuncSplat();

	//! Initializes the idFuncSplat object for use.
	void Spawn();

private:
	//! Handles the activation event for a splat entity, triggering splat effects and sounds.
	void Event_Activate( idEntity* activator );

	//! Applies decals to surfaces based on spawn arguments.
	void Event_Splat();
};

/*!
	\class idTextEntity
	\brief A text entity class that manages display and behavior of text-based game elements.
*/
class idTextEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idTextEntity );

	//! Initializes the text entity and activates its thinking behavior if conditions are met.
	void		 Spawn();

	//! Saves the text entity data to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the text entity state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the text entity's display and debug arrows each frame.
	virtual void Think();

private:
	idStr text;
	bool  playerOriented;
};

/*!
	\class idLocationEntity
	\brief A location entity that stores and provides access to location names for use in the game world.
*/
class idLocationEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idLocationEntity );

	//! Initializes the location entity by setting its location name from the spawn arguments or entity name.
	void		Spawn();

	//! Returns the location string from the entity's spawn arguments.
	const char* GetLocation() const;

private:
};

/*!
	\class idLocationSeparatorEntity
	\brief A specialized entity that manages portal blocking and location separation in the game world.
*/
class idLocationSeparatorEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idLocationSeparatorEntity );

	//! Initializes the location separator entity by finding and blocking the associated portal.
	void Spawn();

private:
};

/*!
	\class idVacuumSeparatorEntity
	\brief Handles portal blocking and activation for vacuum separator entities.
*/
class idVacuumSeparatorEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idVacuumSeparatorEntity );

	//! Constructs a new idVacuumSeparatorEntity object.
	idVacuumSeparatorEntity();

	//! Initializes the vacuum separator entity by finding and blocking the associated portal.
	void Spawn();

	//! Saves the vacuum separator entity state to the provided save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the vacuum separator entity state from a saved game file.
	void Restore( idRestoreGame* savefile );

	//! Activates the vacuum separator entity by clearing the portal state.
	void Event_Activate( idEntity* activator );

private:
	qhandle_t portal;
};

/*!
	\class idVacuumEntity
	\brief Manages vacuum entities that control suction areas within the game world.
*/
class idVacuumEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idVacuumEntity );

	//! Initializes the vacuum entity and sets the vacuum area number based on the entity's spawn position.
	void Spawn();

private:
};

/*!
	\class idBeam
	\brief Represents a beam entity that can track and synchronize with target and master entities.

	The idBeam class implements a visual beam effect that can follow a master beam entity and target a specific location. It manages the beam's visibility, synchronization with other entities, and
   network state synchronization. The class supports setting a master beam to follow, defining a target endpoint, and handling activation events to show or hide the beam. It also provides
   serialization methods for saving and restoring the beam's state, as well as network messaging support for updating the beam's properties across the network.

*/
class idBeam : public idEntity
{
public:
	CLASS_PROTOTYPE( idBeam );

	//! Constructs an idBeam object with target and master set to NULL.
	idBeam();

	//! Initializes the beam entity's properties and prepares it for rendering.
	void		 Spawn();

	//! Saves the beam's target and master to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the beam's target and master from the save file
	void		 Restore( idRestoreGame* savefile );

	//! Updates the beam entity's state and synchronizes its target with the master entity.
	virtual void Think();

	//! Sets the master beam that this beam follows.
	void		 SetMaster( idBeam* masterbeam );

	//! Sets the target end point of the beam effect.
	void		 SetBeamTarget( const idVec3& origin );

	//! Displays the beam entity and updates its target position.
	virtual void Show();

	//! Writes beam entity data to a network snapshot message.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads beam entity state from a network snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

private:
	//! Finds a beam target entity and establishes a connection to it.
	void				Event_MatchTarget();

	//! Toggles the visibility of the beam entity based on its current hidden state.
	void				Event_Activate( idEntity* activator );

	idEntityPtr<idBeam> target;
	idEntityPtr<idBeam> master;
};

/*
===============================================================================

  idLiquid

===============================================================================
*/

class idRenderModelLiquid;

/*!
	\class idLiquid
	\brief Manages liquid entities with touch event handling and persistence.
*/
class idLiquid : public idEntity
{
public:
	CLASS_PROTOTYPE( idLiquid );

	//! Initializes the liquid entity.
	void Spawn();

	//! Saves the liquid declaration to a file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the liquid effect state from a saved game file.
	void Restore( idRestoreGame* savefile );

private:
	//! Handles touch events for liquid entities, with a FIXME comment indicating incomplete implementation for QuakeCon.
	void				 Event_Touch( idEntity* other, trace_t* trace );

	idRenderModelLiquid* model;
};

/*!
	\class idShaking
	\brief Manages and controls shaking effects for entities in the game.

	The idShaking class is designed to control visual shaking effects applied to game entities. It inherits from idEntity and provides functionality to initialize, activate, and persist shaking
   states. The class supports saving and restoring shaking states during game sessions. The shaking effect can be started manually through BeginShaking or triggered via the Event_Activate method.
   Initialization of the shaking effect occurs during Spawn, which also determines whether the effect starts automatically based on spawn arguments.

*/
class idShaking : public idEntity
{
public:
	CLASS_PROTOTYPE( idShaking );

	//! Initializes a new instance of the idShaking class with active set to false.
	idShaking();

	//! Initializes the shaking effect and starts it if not disabled by spawn arguments.
	void Spawn();

	//! Saves the shaking state to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the shaking state from a saved game file.
	void Restore( idRestoreGame* savefile );

private:
	idPhysics_Parametric physicsObj;
	bool				 active;

	//! Starts the shaking animation for the object.
	void				 BeginShaking();

	//! Activates or deactivates the shaking effect based on the current state.
	void				 Event_Activate( idEntity* activator );
};

/*!
	\class idEarthQuake
	\brief Manages earthquake effects within the game world.

	The idEarthQuake class represents a dynamic environmental effect that simulates seismic activity. It inherits from idEntity to integrate seamlessly with the game's entity system and provides
   functionality for activation, state management, and persistence. The class initializes its behavior through the Spawn method, which processes spawn arguments to configure the earthquake's
   properties. It supports saving and restoring its state during game sessions, ensuring consistent behavior across load operations. The Think method handles the ongoing update logic for the
   earthquake effect, including timing and removal when the shaking ceases. Activation is managed through the Event_Activate method, which triggers the earthquake's behavior and responds to game
   entity interactions.

*/
class idEarthQuake : public idEntity
{
public:
	CLASS_PROTOTYPE( idEarthQuake );

	//! Constructs an idEarthQuake object with default values.
	idEarthQuake();

	//! Initializes the earthquake entity with spawn arguments and sets up its initial state.
	void		 Spawn();

	//! Saves the earthquake state to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the earthquake state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the earthquake effect and handles removal when the shaking stops.
	virtual void Think();

private:
	int	  nextTriggerTime;
	int	  shakeStopTime;
	float wait;
	float random;
	bool  triggered;
	bool  playerOriented;
	bool  disabled;
	float shakeTime;

	//! Activates the earthquake effect and handles triggering behavior.
	void  Event_Activate( idEntity* activator );
};

/*!
	\class idFuncPortal
	\brief Manages portal functionality for entity-based traversal mechanics.
*/
class idFuncPortal : public idEntity
{
public:
	CLASS_PROTOTYPE( idFuncPortal );

	//! Initializes a new instance of the idFuncPortal class with default values.
	idFuncPortal();

	//! Initializes the portal functionality for the entity.
	void Spawn();

	//! Saves the portal state and configuration to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the portal state from a saved game file.
	void Restore( idRestoreGame* savefile );

private:
	qhandle_t portal;
	bool	  state;

	//! Toggles the portal state between blocked and unblocked when activated.
	void	  Event_Activate( idEntity* activator );
};

/*!
	\class idFuncAASPortal
	\brief Manages AAS portal state for navigation area updates.
*/
class idFuncAASPortal : public idEntity
{
public:
	CLASS_PROTOTYPE( idFuncAASPortal );

	//! Initializes a new instance of the idFuncAASPortal class with the state set to false.
	idFuncAASPortal();

	//! Initializes the AAS portal state based on spawn arguments.
	void Spawn();

	//! Saves the portal state to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the AAS portal from the save file.
	void Restore( idRestoreGame* savefile );

private:
	bool state;

	//! Toggles the AAS portal state and updates the navigation area accordingly.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idFuncAASObstacle
	\brief Manages AAS obstacle state for entity activation and persistence.
*/
class idFuncAASObstacle : public idEntity
{
public:
	CLASS_PROTOTYPE( idFuncAASObstacle );

	//! Constructs an idFuncAASObstacle object and initializes its state to false.
	idFuncAASObstacle();

	//! Initializes the AAS obstacle state based on spawn arguments.
	void Spawn();

	//! Saves the AAS obstacle state to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the AAS obstacle from the save file.
	void Restore( idRestoreGame* savefile );

private:
	bool state;

	//! Toggles the AAS obstacle state and updates the area state accordingly.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idFuncRadioChatter
	\brief Manages radio chatter functionality for entities in the game.

	This class implements the behavior for radio chatter entities that can be activated to display messages on the HUD and trigger associated game events. The class extends idEntity to integrate with
   the game's entity system and handles serialization of its state during save/load operations. The radio chatter functionality is typically used to provide in-game communication or narrative elements
   through audio and visual feedback. The class supports activation events that can display messages and reset HUD states, making it useful for implementing interactive radio communication systems
   within the game.

*/
class idFuncRadioChatter : public idEntity
{
public:
	CLASS_PROTOTYPE( idFuncRadioChatter );

	//! Constructs a new idFuncRadioChatter object with time initialized to 0.0.
	idFuncRadioChatter();

	//! Initializes the radio chatter time value from spawn arguments.
	void Spawn();

	//! Serializes the radio chatter time value to the specified save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the radio chatter state from a saved game file.
	void Restore( idRestoreGame* savefile );

private:
	float time;

	//! Activates the radio chatter event and displays a radio message on the HUD.
	void  Event_Activate( idEntity* activator );

	//! Resets the radio HUD message for the player and activates associated targets.
	void  Event_ResetRadioHud( idEntity* activator );
};

/*!
	\class idPhantomObjects
	\brief Manages phantom objects that can be activated and hidden during gameplay.

	The idPhantomObjects class extends idEntity to provide functionality for objects that can be activated and then hidden or moved to specific positions. These objects are typically used for visual
   effects or scripted events where an object appears, is activated, and then either disappears or moves to a new location. The class handles the spawning, saving, restoring, and ticking behavior of
   these phantom objects. It supports activation events that can initialize target positions and timing, allowing for complex scripted sequences. The phantom objects can be saved and restored during
   game sessions, maintaining their state. The class uses a prototype pattern for instantiation and integrates with the game's entity system for activation and interaction.

*/
class idPhantomObjects : public idEntity
{
public:
	CLASS_PROTOTYPE( idPhantomObjects );

	//! Initializes a new instance of the idPhantomObjects class.
	idPhantomObjects();

	//! Initializes the phantom objects spawn parameters and prepares the object for hiding.
	void		 Spawn();

	//! Saves the phantom objects state to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the phantom objects from a save file
	void		 Restore( idRestoreGame* savefile );

	//! Executes the phantom objects behavior during the game loop
	virtual void Think();

private:
	//! Handles the activation event for phantom objects, initializing their target positions and timing.
	void				 Event_Activate( idEntity* activator );
	void				 Event_ShakeObject( idEntity* object, int starttime );

	int					 end_time;
	float				 throw_time;
	float				 shake_time;
	idVec3				 shake_ang;
	float				 speed;
	int					 min_wait;
	int					 max_wait;
	idEntityPtr<idActor> target;
	idList<int>			 targetTime;
	idList<idVec3>		 lastTargetPos;
};

/*!
	\class idShockwave
	\brief Manages and simulates shockwave effects with expandable radius and damage application.

	The idShockwave class represents a spherical shockwave effect that expands over time and applies forces or damage to entities within its radius. It inherits from idEntity and implements standard
   game entity behaviors such as spawning, thinking, saving, and restoring. The shockwave can be activated to begin its expansion and effects. The class controls the shockwave's lifetime, radius
   growth, and interaction with other entities through its Think method. Save and restore functionality ensures the shockwave state can be persisted during game saves.

*/
class idShockwave : public idEntity
{
public:
	CLASS_PROTOTYPE( idShockwave );

	//! Initializes a new instance of the idShockwave class with default values.
	idShockwave();
	~idShockwave();

	//! Initializes the shockwave properties and activates it if specified.
	void Spawn();

	//! Updates the shockwave effect by expanding its size and applying forces or damage to affected entities.
	void Think();

	//! Saves the shockwave state to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the shockwave state from a save file.
	void Restore( idRestoreGame* savefile );

private:
	//! Activates the shockwave effect and initializes its start time and state.
	void  Event_Activate( idEntity* activator );

	bool  isActive;
	int	  startTime;
	int	  duration;

	float startSize;
	float endSize;
	float currentSize;

	float magnitude;

	float height;
	bool  playerDamaged;
	float playerDamageSize;
};

/*!
	\class idFuncMountedObject
	\brief Manages mounted objects that can be activated by players and restrict player movement through angular limitations.

	This class implements the behavior for entities that function as mounted objects, typically used for controlling player movement or interaction within a confined angular range. It inherits from
   idEntity and provides functionality to initialize spawn parameters, handle touch and activation events, and enforce angular restrictions on yaw and pitch. The Spawn method sets up the object's
   initial state and script function, while Think handles the object's ongoing behavior. The GetAngleRestrictions method allows retrieving the limits for rotational movement, and the event handlers
   manage interactions with other entities. The class is designed to work in conjunction with player entities to create controlled movement or interaction scenarios.

*/
class idFuncMountedObject : public idEntity
{
public:
	CLASS_PROTOTYPE( idFuncMountedObject );

	//! Constructs a new idFuncMountedObject instance with default values.
	idFuncMountedObject();
	~idFuncMountedObject();

	//! Initializes the mounted object's spawn parameters and script function.
	void Spawn();

	//! Executes the thinking logic for the mounted object entity.
	void Think();

	//! Retrieves the angular restrictions for yaw and pitch limits.
	void GetAngleRestrictions( int& yaw_min, int& yaw_max, int& pitch );

private:
	int	 harc;
	int	 varc;

	//! Handles touch events for mounted objects by activating them when not in client mode.
	void Event_Touch( idEntity* other, trace_t* trace );

	//! Mounts a player entity when the mounted object is activated.
	void Event_Activate( idEntity* activator );

public:
	bool		isMounted;
	function_t* scriptFunction;
	idPlayer*	mountedPlayer;
};

/*!
	\class idFuncMountedWeapon
	\brief A class representing a mounted weapon entity that handles turret orientation, firing logic, and projectile management.
*/
class idFuncMountedWeapon : public idFuncMountedObject
{
public:
	CLASS_PROTOTYPE( idFuncMountedWeapon );

	//! Constructs a new idFuncMountedWeapon object and initializes its member variables.
	idFuncMountedWeapon();
	~idFuncMountedWeapon();

	//! Initializes the mounted weapon entity by loading projectile and sound definitions and setting up firing parameters.
	void Spawn();

	//! Updates the mounted weapon's turret orientation and handles firing logic.
	void Think();

private:
	// The actual turret that moves with the player's view
	idEntity*			 turret;

	// the muzzle bone's position, used for launching projectiles and trailing smoke
	idVec3				 muzzleOrigin;
	idMat3				 muzzleAxis;

	float				 weaponLastFireTime;
	float				 weaponFireDelay;

	const idDict*		 projectile;

	const idSoundShader* soundFireWeapon;

	//! Sets the turret entity based on the targets list if available, otherwise warns about missing target.
	void				 Event_PostSpawn();
};

/*!
	\class idPortalSky
	\brief Manages portal sky entities that control sky rendering through portal systems.
*/
class idPortalSky : public idEntity
{
public:
	CLASS_PROTOTYPE( idPortalSky );

	//! Constructs a new idPortalSky object.
	idPortalSky();
	~idPortalSky();

	//! Initializes the portal sky object and schedules a post-spawn event if not triggered.
	void Spawn();

	//! Sets the portal sky entity for the game world after spawning.
	void Event_PostSpawn();

	//! Sets the portal sky entity to the current instance.
	void Event_Activate( idEntity* activator );
};

#endif /* !__GAME_MISC_H__ */
