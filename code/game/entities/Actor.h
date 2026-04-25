/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

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

#ifndef __GAME_ACTOR_H__
#define __GAME_ACTOR_H__

/*
===============================================================================

	idActor

===============================================================================
*/

extern const idEventDef AI_EnableEyeFocus;
extern const idEventDef AI_DisableEyeFocus;
extern const idEventDef EV_Footstep;
extern const idEventDef EV_FootstepLeft;
extern const idEventDef EV_FootstepRight;
extern const idEventDef EV_EnableWalkIK;
extern const idEventDef EV_DisableWalkIK;
extern const idEventDef EV_EnableLegIK;
extern const idEventDef EV_DisableLegIK;
extern const idEventDef AI_SetAnimPrefix;
extern const idEventDef AI_PlayAnim;
extern const idEventDef AI_PlayCycle;
extern const idEventDef AI_AnimDone;
extern const idEventDef AI_SetBlendFrames;
extern const idEventDef AI_GetBlendFrames;

extern const idEventDef AI_SetState;

class idDeclParticle;

/*!
	\class idAnimState
	\brief Manages animation states and transitions for animated entities.

	The idAnimState class provides functionality for controlling animation sequences on entities, handling state management, blending, and synchronization with animator components. It maintains a
   thread-based animation system that can be initialized with a specific owner actor and animation channel. The class supports various animation operations including playing, stopping, cycling, and
   transitioning between states while maintaining proper state flags and blend durations. It handles both enabling and disabling of animation states, and provides methods for checking animation
   completion and idle status. The class integrates with save and restore mechanisms to persist animation state information. Memory management is handled through initialization and shutdown methods
   that properly set up and tear down the underlying animation thread structure.

*/
class idAnimState
{
public:
	bool  idleAnim;
	idStr state;
	int	  animBlendFrames;
	int	  lastAnimBlendFrames; // allows override anims to blend based on the last transition time

public:
	//! Initializes a new instance of the idAnimState class with default values.
	idAnimState();

	//! Destructor for the idAnimState class that cleans up the animation thread.
	~idAnimState();

	//! Saves the animation state to a save file.
	void		Save( idSaveGame* savefile ) const;

	//! Restores the animation state from a save file.
	void		Restore( idRestoreGame* savefile );

	//! Initializes the animation state with the specified owner actor, animator, and animation channel.
	void		Init( idActor* owner, idAnimator* _animator, int animchannel );

	//! Destroys the animation state by deleting its thread and setting the thread pointer to NULL.
	void		Shutdown();

	//! Sets the animation state of an entity to the specified state with a given blend duration.
	void		SetState( const char* name, int blendFrames );

	//! Stops the animation on the specified channel after the given number of frames.
	void		StopAnim( int frames );

	//! Plays the specified animation on the animation channel.
	void		PlayAnim( int anim );

	//! Cycles the animation to the specified animation index.
	void		CycleAnim( int anim );

	//! Sets the animation state to idle.
	void		BecomeIdle();

	//! Updates the animation state and returns true if successful.
	bool		UpdateState();

	//! Returns true if the animation state is disabled.
	bool		Disabled() const;

	//! Enables the animation state if it was previously disabled.
	void		Enable( int blendFrames );

	//! Disables the animation state and clears the idle animation flag.
	void		Disable();

	//! Determines if an animation has finished playing based on the specified blend frames.
	bool		AnimDone( int blendFrames ) const;

	//! Returns true if the animation state is idle.
	bool		IsIdle() const;

	//! Returns the animation flags for the current animation state.
	animFlags_t GetAnimFlags() const;

private:
	idActor*	  self;
	idAnimator*	  animator;
	idThread*	  thread;
	rvStateThread stateThread;

	int			  channel;
	bool		  disabled;
};

/*!
	\class idAttachInfo
	\brief Class for managing attachment information.
*/
class idAttachInfo
{
public:
	idEntityPtr<idEntity> ent;
	int					  channel;
};

typedef struct {
	jointModTransform_t mod;
	jointHandle_t		from;
	jointHandle_t		to;
} copyJoints_t;

/*!
	\class idActor
	\brief Base class for actor entities with animation, combat, and AI behavior capabilities.

	Provides core functionality for entity characters including animation management, combat systems, AI behavior, and physics interaction. The class supports articulated figure simulation, damage
   handling, enemy detection, and various animation states. It integrates with the game's scripting system and handles entity attachment and detachment. The actor maintains visual and physical
   representations with support for ragdoll physics and inverse kinematics. Memory management is handled through base class inheritance and standard C++ practices. The class is designed to be extended
   by specific actor types while providing a common interface for actor behavior and state management.

*/
class idActor : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idActor );

	int					team;
	int					rank;	  // monsters don't fight back if the attacker's rank is higher
	idMat3				viewAxis; // view axis of the actor

	idLinkList<idActor> enemyNode; // node linked into an entity's enemy list for quick lookups of who is attacking him
	idLinkList<idActor> enemyList; // list of characters that have targeted the player as their enemy

public:
	//! Initializes a new instance of the idActor class.
	idActor();

	//! Destructor for the idActor class that cleans up actor resources and removes attached entities.
	virtual ~idActor();

	//! Initializes the actor entity with properties from spawn arguments and sets up animations and attachments.
	void				  Spawn();

	//! Resets the actor state by setting up the head and finishing the setup process.
	virtual void		  Restart();

	//! Saves the actor's state to a save game file.
	void				  Save( idSaveGame* savefile ) const;

	//! Restores the actor's state from a saved game file
	void				  Restore( idRestoreGame* savefile );

	//! Hides the actor and its associated entities and lights.
	virtual void		  Hide();

	//! Makes the actor and its associated entities visible and active.
	virtual void		  Show();

	//! Returns the default surface type for the actor.
	virtual int			  GetDefaultSurfaceType() const;

	/*!
		\brief Projects an overlay onto the actor and its team entities that are bound to this actor

		This function projects an overlay onto the actor itself and then iterates through all team entities bound to this actor. For each team entity that is bound to this actor and has takedamage
	   enabled along with a "bleed" spawn argument, it projects the same overlay onto that entity. The overlay is positioned using the provided origin, direction, size and material parameters.

		\param origin The world space origin point from which the overlay is projected
		\param dir The direction vector for the overlay projection
		\param size The size of the overlay to be projected
		\param material The material to be used for the overlay
	*/
	virtual void		  ProjectOverlay( const idVec3& origin, const idVec3& dir, float size, const char* material );

	//! Loads the articulated figure for the actor.
	virtual bool		  LoadAF();

	//! Initializes the actor's body components including eye joints and animations
	void				  SetupBody();

	//! Plays a blink animation on the actor's eyes if conditions are met.
	void				  CheckBlink();

	//! Returns the transformation from physics to visual space for the actor.
	virtual bool		  GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );

	//! Retrieves the transformation from physics to sound coordinates for the actor.
	virtual bool		  GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );

	//! Shuts down the actor's animation threads and removes the script thread.
	void				  ShutdownThreads();

	//! Returns false to indicate that the script object should not be constructed at spawn time.
	virtual bool		  ShouldConstructScriptObjectAtSpawn() const;

	//! Constructs and initializes the script object for the actor by calling its constructor.
	virtual idThread*	  ConstructScriptObject();

	//! Updates the actor's script execution state.
	void				  UpdateScript();

	//! Returns the script function with the specified name from the actor's script object.
	const function_t*	  GetScriptFunction( const char* funcname );

	//! Sets the actor's current state to the specified state function and executes it.
	void				  SetState( const function_t* newState );

	//! Sets the actor's state to the specified state name, handling both native and script functions.
	void				  SetState( const char* statename );

	//! Sets the vertical position of the actor's eyes relative to the actor's origin.
	void				  SetEyeHeight( float height );

	//! Returns the z-coordinate of the actors eye offset.
	float				  EyeHeight() const;

	//! Returns the eye offset vector for the actor based on the gravity normal and eye offset z-coordinate.
	idVec3				  EyeOffset() const;

	//! Returns the eye position of the actor.
	idVec3				  GetEyePosition() const;

	//! Retrieves the view position and axis of the actor.
	virtual void		  GetViewPos( idVec3& origin, idMat3& axis ) const;

	//! Sets the field of view for the actor.
	void				  SetFOV( float fov );

	//! Checks if the given position is within the actor's field of view
	bool				  CheckFOV( const idVec3& pos ) const;

	//! Checks if the actor can see the specified entity, optionally considering the field of view.
	bool				  CanSee( idEntity* ent, bool useFOV ) const;

	//! Checks if a given point is visible from the actor's eye position.
	bool				  PointVisible( const idVec3& point ) const;

	//! Returns the head and chest positions for AI aiming based on the last sight position.
	virtual void		  GetAIAimTargets( const idVec3& lastSightPos, idVec3& headPos, idVec3& chestPos );

	//! Initializes damage groups and scales for the actor's joints based on spawn arguments.
	void				  SetupDamageGroups();

	/*!
		\brief Applies damage to the actor based on the given inflictor, attacker, and damage definition.

		This function handles the logic for applying damage to an actor, including checking for damage immunity conditions such as when the actor is not taking damage or when it's a final boss that
	   should ignore certain types of attacks. It also manages achievement tracking for kills and performs damage scaling based on location and damage definitions. The function uses the provided
	   damage definition to compute the actual damage amount, which may be randomized between minimum and maximum values. It updates the actor's health and triggers appropriate feedback to the
	   attacker. The function also includes special logic for bosses and various game achievements.

		\param inflictor Entity that caused the damage
		\param attacker Entity that initiated the damage
		\param dir Direction of the damage vector
		\param damageDefName Name of the damage definition to use
		\param damageScale Scale factor for the damage amount
		\param location Location index where the damage was applied
	*/
	virtual void		  Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );

	//! Returns the damage amount scaled for a specific location.
	int					  GetDamageForLocation( int damage, int location );

	//! Returns the damage group string for the specified location index.
	const char*			  GetDamageGroup( int location );

	//! Clears the pain debounce time for the actor.
	void				  ClearPain();

	/*!
		\brief Handles the actor's pain response to damage, including sound playback and animation triggering.

		This function processes the actor's reaction to being hurt, determining whether to play pain sounds and animations based on the amount of damage and the actor's current health. It manages
	   debouncing to prevent excessive pain events, checks if pain animations are allowed, and selects an appropriate pain animation based on the damage location and actor prefix. The function also
	   handles debug printing of damage information when enabled.

		\param inflictor Entity that caused the damage
		\param attacker Entity that inflicted the damage
		\param damage Amount of damage taken
		\param dir Direction from which the damage came
		\param location Location of the damage hit
		\return True if a pain animation was triggered, false otherwise
	*/
	virtual bool		  Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Sets the combat model for the actor using the assigned model definition.
	void				  SetCombatModel();

	//! Returns the combat model associated with the actor.
	idClipModel*		  GetCombatModel() const;

	//! Links the actor's combat model to the game's collision detection system and recursively links the head entity's combat model if it exists.
	virtual void		  LinkCombat();

	//! Unlinks the actor's combat model and its head entity from the physics simulation.
	virtual void		  UnlinkCombat();

	//! Starts the ragdoll physics for the actor if an articulated figure is loaded and not already active.
	bool				  StartRagdoll();

	//! Stops the ragdoll physics simulation for the actor if it is currently active.
	void				  StopRagdoll();

	//! Updates the animation controllers for the actor based on active states andIK evaluation.
	virtual bool		  UpdateAnimationControllers();

	//! Returns the delta view angles used to allow movers to rotate the view of the actor.
	const idAngles&		  GetDeltaViewAngles() const;

	//! Sets the delta view angles for the actor.
	void				  SetDeltaViewAngles( const idAngles& delta );

	//! Determines whether the actor has any visible enemies in its enemy list.
	bool				  HasEnemies() const;

	//! Returns the closest visible enemy to the specified position.
	idActor*			  ClosestEnemyToPoint( const idVec3& pos );

	//! Returns the enemy actor with the highest health value among visible enemies.
	idActor*			  EnemyWithMostHealth();

	//! Returns false indicating the actor is not currently on a ladder.
	virtual bool		  OnLadder() const;

	//! Retrieves the AAS area number and position for the actor.
	virtual void		  GetAASLocation( idAAS* aas, idVec3& pos, int& areaNum ) const;

	//! Attaches an entity to a joint on the actor.
	void				  Attach( idEntity* ent );

	//! Teleports the actor to a specified location with given angles, optionally killing entities at the destination.
	virtual void		  Teleport( const idVec3& origin, const idAngles& angles, idEntity* destination );

	//! Returns the render view for the actor, initialized with the actor's view axis and eye position.
	virtual renderView_t* GetRenderView();

	//! Returns the animation index for the specified channel and animation name.
	int					  GetAnim( int channel, const char* name );

	//! Updates the animation states for the head, torso, and legs of the actor.
	void				  UpdateAnimState();

	//! Sets the animation state for the specified channel with the given statename and blend frames.
	void				  SetAnimState( int channel, const char* name, int blendFrames );

	//! Returns the animation state for the specified channel.
	const char*			  GetAnimState( int channel ) const;

	//! Returns a reference to the animation state variable for the specified channel.
	idAnimState&		  GetAnimStateVar( int channel );

	//! Checks if the actor is in a specific animation state for the given channel.
	bool				  InAnimState( int channel, const char* name ) const;

	//! Returns the current wait state of the actor as a string, or NULL if no wait state is set.
	const char*			  WaitState() const;

	//! Sets the wait state of the actor to the specified state string.
	void				  SetWaitState( const char* _waitstate );

	//! Checks if the animation sequence for a specified channel is complete, considering the blend frames.
	bool				  AnimDone( int channel, int blendFrames ) const;

	//! Spawns gib entities for the actor using the provided direction and damage definition name, then removes actor attachments.
	virtual void		  SpawnGibs( const idVec3& dir, const char* damageDefName );

	idEntity*			  GetHeadEntity() { return head.GetEntity(); };

	//! Returns the next visible enemy in the actor's enemy list starting from the specified entity.
	idActor*			  NextEnemy( idEntity* ent );

protected:
	friend class idAnimState;

	float							fovDot;		 // cos( fovDegrees )
	idVec3							eyeOffset;	 // offset of eye relative to physics origin
	idVec3							modelOffset; // offset of visual model relative to the physics origin

	idAngles						deltaViewAngles; // delta angles relative to view input angles

	int								pain_debounce_time; // next time the actor can show pain
	int								pain_delay;			// time between playing pain sound
	int								pain_threshold;		// how much damage monster can take at any one time before playing pain animation

	idStrList						damageGroups; // body damage groups
	idList<float, TAG_ACTOR>		damageScale;  // damage scale per damage gruop

	bool							use_combat_bbox; // whether to use the bounding box for combat collision
	idEntityPtr<idAFAttachment>		head;
	idList<copyJoints_t, TAG_ACTOR> copyJoints; // copied from the body animation to the head model

	// state variables
	const function_t*				state;
	const function_t*				idealState;

	// joint handles
	jointHandle_t					leftEyeJoint;
	jointHandle_t					rightEyeJoint;
	jointHandle_t					soundJoint;

	idIK_Walk						walkIK;

	idStr							animPrefix;
	idStr							painAnim;

	// blinking
	int								blink_anim;
	int								blink_time;
	int								blink_min;
	int								blink_max;

	// script variables
	idThread*						scriptThread;
	idStr							waitState;
	idAnimState						headAnim;
	idAnimState						torsoAnim;
	idAnimState						legsAnim;

	rvStateThread					stateThread;

	bool							allowPain;
	bool							allowEyeFocus;
	bool							finalBoss;

	int								painTime;
	bool							damageNotByFists;

	idList<idAttachInfo, TAG_ACTOR> attachments;

	int								damageCap;

	//! Causes the actor to gib by applying the specified damage definition and direction.
	virtual void					Gib( const idVec3& dir, const char* damageDefName );

	//! Removes all attached entities from the actor that have the 'remove' spawn argument set.
	void							RemoveAttachments();

	//! Copies animation data from body joints to head joints for proper coordination.
	void							CopyJointsFromBodyToHead();

	//! Checks if an animation on the specified channel is complete, considering the blending frames.
	bool							AnimDone( int channel, int blendFrames );

protected:
	//! Synchronizes animation channels for the actor, blending animations between specified channels.
	void  SyncAnimChannels( int channel, int syncToChannel, int blendFrames );

	//! Completes the actor setup by initializing the script object and configuring the body.
	void  FinishSetup();

	//! Initializes and sets up the head attachment for the actor if a head model is specified.
	void  SetupHead();

	//! Plays a footstep sound based on the material type of the surface beneath the actor.
	void  PlayFootStepSound();

	//! Enables eye focus for the actor and sets the next blink time.
	void  Event_EnableEyeFocus();

	//! Disables eye focus for the actor and clears eyelid animations.
	void  Event_DisableEyeFocus();

	//! Plays a footstep sound for the actor.
	void  Event_Footstep();

	//! Enables the walk inverse kinematics for the actor.
	void  Event_EnableWalkIK();

	//! Disables all walk IK for the actor.
	void  Event_DisableWalkIK();

	//! Enables leg inverse kinematics for the specified leg number.
	void  Event_EnableLegIK( int num );

	//! Disables the inverse kinematics for a specified leg on the actor.
	void  Event_DisableLegIK( int num );

	//! Sets the animation prefix for the actor.
	void  Event_SetAnimPrefix( const char* name );
	void  Event_LookAtEntity( idEntity* ent, float duration );

	//! Sets the pain time to prevent the actor from experiencing pain for a specified duration.
	void  Event_PreventPain( float duration );

	//! Disables the actor's pain response.
	void  Event_DisablePain();

	//! Enables pain reactions for the actor.
	void  Event_EnablePain();

	//! Returns the pain animation name for the actor.
	void  Event_GetPainAnim();

	//! Stops animation on the specified channel with the given number of frames.
	void  Event_StopAnim( int channel, int frames );

	//! Plays an animation on the specified channel for the actor.
	void  Event_PlayAnim( int channel, const char* name );

	//! Plays a cycle animation on the specified channel for the actor
	void  Event_PlayCycle( int channel, const char* name );

	//! Sets the idle animation for a specified channel and animation name.
	void  Event_IdleAnim( int channel, const char* name );

	//! Sets the weight of a synchronized animation for the specified channel and animation index.
	void  Event_SetSyncedAnimWeight( int channel, int anim, float weight );

	//! Disables the animation for the specified channel and synchronizes it with other channels.
	void  Event_OverrideAnim( int channel );

	//! Enables animation for a specified channel with optional blending.
	void  Event_EnableAnim( int channel, int blendFrames );

	//! Sets the blend frames for a specified animation channel on the actor.
	void  Event_SetBlendFrames( int channel, int blendFrames );

	//! Returns the number of blend frames for the specified animation channel.
	void  Event_GetBlendFrames( int channel );

	//! Sets the animation state for the specified channel with the given state name and blend frames.
	void  Event_AnimState( int channel, const char* name, int blendFrames );

	//! Returns the animation state of the specified channel as a string
	void  Event_GetAnimState( int channel );

	//! Checks if the actor is in a specific animation state on the given channel.
	void  Event_InAnimState( int channel, const char* name );

	//! Completes a pending action if it matches the provided name.
	void  Event_FinishAction( const char* name );

	//! Handles the animation done event for an actor, returning whether the animation has completed.
	void  Event_AnimDone( int channel, int blendFrames );

	//! Checks if the actor has an animation with the specified name on the given channel and returns a float indicating the result.
	void  Event_HasAnim( int channel, const char* name );

	//! Checks if an animation exists for the actor and reports an error if it does not.
	void  Event_CheckAnim( int channel, const char* animname );

	//! Returns the full animation name for a given channel and animation name.
	idStr ChooseAnim( int channel, const char* animname );

	//! Selects and returns an animation for the actor based on the specified channel and animation name.
	void  Event_ChooseAnim( int channel, const char* animname );

	//! Returns the length of an animation for the specified channel and animation name.
	float AnimLength( int channel, const char* animname );

	//! Returns the length of an animation for the specified channel and animation name.
	void  Event_AnimLength( int channel, const char* animname );

	//! Returns the total movement distance of a specified animation channel.
	void  Event_AnimDistance( int channel, const char* animname );

	//! Returns whether the actor has any enemies.
	void  Event_HasEnemies();

	//! Sets the next enemy for the actor based on the provided entity.
	void  Event_NextEnemy( idEntity* ent );

	//! Returns the closest enemy entity to the specified position.
	void  Event_ClosestEnemyToPoint( const idVec3& pos );

	//! Stops sound on the actor for the specified channel and networking sync settings.
	void  Event_StopSound( int channel, int netsync );

	//! Sets the next state for the actor to transition to when the current state completes.
	void  Event_SetNextState( const char* name );

	//! Sets the actor's state to the specified script function name.
	void  Event_SetState( const char* name );

	//! Returns the name of the current state of the actor.
	void  Event_GetState();

	//! Returns the head entity of the actor.
	void  Event_GetHead();

	//! Sets the damage scale for a specified damage group on the actor.
	void  Event_SetDamageGroupScale( const char* groupName, float scale );

	//! Sets the damage scale for all damage groups to the specified value.
	void  Event_SetDamageGroupScaleAll( float scale );

	//! Returns the damage scale factor for a specified damage group name.
	void  Event_GetDamageGroupScale( const char* groupName );

	//! Sets the damage cap for the actor to the specified value.
	void  Event_SetDamageCap( float _damageCap );

	//! Sets the wait state of the actor to the specified state string.
	void  Event_SetWaitState( const char* waitState );

	//! Returns the current wait state of the actor as a string.
	void  Event_GetWaitState();
};

#endif /* !__GAME_ACTOR_H__ */
