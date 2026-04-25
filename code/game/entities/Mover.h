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

#ifndef __GAME_MOVER_H__
#define __GAME_MOVER_H__

extern const idEventDef EV_TeamBlocked;
extern const idEventDef EV_PartBlocked;
extern const idEventDef EV_ReachedPos;
extern const idEventDef EV_ReachedAng;

/*!
	\class idMover
	\brief A movable entity that supports various movement and rotation behaviors with physics integration.

	The idMover class represents a movable entity that can perform various types of motion including translation, rotation, and spline-based movement. It inherits from idEntity and provides
   comprehensive support for physics-based movement with configurable speeds, timing, and sounds. The class manages both client-side and server-side state synchronization, supports portal operations,
   and handles GUI interactions. Movement operations can be initiated through events or direct method calls, with support for acceleration, deceleration, and custom spline paths. The class integrates
   with the engine's save and restore systems, allowing persistent state management for moving entities. It also supports team-based blocking detection and callback mechanisms for complex movement
   sequences.

*/
class idMover : public idEntity
{
public:
	CLASS_PROTOTYPE( idMover );

	//! Initializes a new instance of the idMover class with default values for all movement and rotation parameters.
	idMover();

	//! Initializes the mover component and sets up its physics properties.
	void		 Spawn();

	//! Saves the mover's state to a save game file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the mover from a saved game file
	void		 Restore( idRestoreGame* savefile );

	/*!
		\brief Handles the event when the mover entity is killed by inflictor with damage from attacker in the specified direction and location

		This function is invoked when the mover entity receives damage that results in its death. It disables further damage to the entity and triggers activation of any targets associated with this
	   mover. The function does not use the damage amount, direction, or hit location parameters for its core logic, but they are provided for potential future extensions or debugging purposes.

		\param inflictor The entity that caused the damage
		\param attacker The entity that initiated the attack
		\param damage The amount of damage that killed the entity
		\param dir The direction from which the damage came
		\param location The location of the hit on the entity
	*/
	virtual void Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Updates the mover's client-side state based on the current time, interpolation fraction, and prediction flag.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Writes the mover's state to a snapshot message for network synchronization.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the mover's state from a snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

	//! Hides the mover entity and clears its physics contents.
	virtual void Hide();

	//! Initializes the mover entity and sets up its physics properties.
	virtual void Show();

	//! Sets the portal state to open or closed based on the boolean parameter.
	void		 SetPortalState( bool open );

protected:
	typedef enum { ACCELERATION_STAGE, LINEAR_STAGE, DECELERATION_STAGE, FINISHED_STAGE } moveStage_t;

	typedef enum { MOVER_NONE, MOVER_ROTATING, MOVER_MOVING, MOVER_SPLINE } moverCommand_t;

	//
	// mover directions.  make sure to change script/doom_defs.script if you add any, or change their order
	//
	typedef enum {
		DIR_UP			= -1,
		DIR_DOWN		= -2,
		DIR_LEFT		= -3,
		DIR_RIGHT		= -4,
		DIR_FORWARD		= -5,
		DIR_BACK		= -6,
		DIR_REL_UP		= -7,
		DIR_REL_DOWN	= -8,
		DIR_REL_LEFT	= -9,
		DIR_REL_RIGHT	= -10,
		DIR_REL_FORWARD = -11,
		DIR_REL_BACK	= -12
	} moverDir_t;

	typedef struct {
		moveStage_t stage;
		int			acceleration;
		int			movetime;
		int			deceleration;
		idVec3		dir;
	} moveState_t;

	typedef struct {
		moveStage_t stage;
		int			acceleration;
		int			movetime;
		int			deceleration;
		idAngles	rot;
	} rotationState_t;

	idPhysics_Parametric physicsObj;

	//! Opens the portal associated with this mover.
	void				 Event_OpenPortal();

	//! Closes the portal associated with this mover if one exists.
	void				 Event_ClosePortal();

	//! Handles the event when a mover part is blocked by another entity.
	void				 Event_PartBlocked( idEntity* blockingEntity );

	//! Moves the mover to the specified position in local coordinates.
	void				 MoveToPos( const idVec3& pos );

	//! Updates the movement sound based on the current stage of the move.
	void				 UpdateMoveSound( moveStage_t stage );

	//! Updates the rotation sound based on the current movement stage.
	void				 UpdateRotationSound( moveStage_t stage );

	//! Sets the GUI state string for mover entities and their associated GUIs.
	void				 SetGuiStates( const char* state );

	//! Finds and retrieves GUI target entities based on spawn arguments.
	void				 FindGuiTargets();

	//! Sets a state key-value pair on GUIs associated with the mover.
	void				 SetGuiState( const char* key, const char* val ) const;

	//! Completes the movement operation and cleans up related resources.
	virtual void		 DoneMoving();

	//! Completes the rotation animation and cleans up related resources.
	virtual void		 DoneRotating();

	//! Initializes the movement of a mover object to a destination position.
	virtual void		 BeginMove( idThread* thread = NULL );

	//! Starts a rotation movement for the mover.
	virtual void		 BeginRotation( idThread* thread, bool stopwhendone );
	moveState_t			 move;

private:
	rotationState_t							 rot;

	int										 move_thread;
	int										 rotate_thread;
	idAngles								 dest_angles;
	idAngles								 angle_delta;
	idVec3									 dest_position;
	idVec3									 move_delta;
	float									 move_speed;
	int										 move_time;
	int										 deceltime;
	int										 acceltime;
	bool									 stopRotation;
	bool									 useSplineAngles;
	idEntityPtr<idEntity>					 splineEnt;
	moverCommand_t							 lastCommand;
	float									 damage;

	qhandle_t								 areaPortal; // 0 = no portal

	idList<idEntityPtr<idEntity>, TAG_MOVER> guiTargets;

	//! Converts a direction code into a corresponding world-space vector.
	void									 VectorForDir( float dir, idVec3& vec );
	idCurve_Spline<idVec3>*					 GetSpline( idEntity* splineEntity ) const;

	//! Sets a callback for the mover's movement or rotation.
	void									 Event_SetCallback();

	//! Handles the event when a mover is blocked by a team member.
	void									 Event_TeamBlocked( idEntity* blockedPart, idEntity* blockingEntity );

	//! Stops the mover's movement and updates its final position
	void									 Event_StopMoving();

	//! Stops the rotation of the mover and sets the angular extrapolation to none.
	void									 Event_StopRotating();

	//! Updates the movement state of the mover based on its current stage and physics properties.
	void									 Event_UpdateMove();

	//! Updates the rotation of the mover based on the current rotation stage and physics parameters.
	void									 Event_UpdateRotation();

	//! Sets the movement speed for the mover object.
	void									 Event_SetMoveSpeed( float speed );

	//! Sets the movement time for the mover after validating that it is greater than zero.
	void									 Event_SetMoveTime( float time );

	//! Sets the deceleration time for the mover after the initial acceleration period.
	void									 Event_SetDecelerationTime( float time );

	//! Sets the acceleration time for the mover to the specified value.
	void									 Event_SetAccellerationTime( float time );

	//! Moves the entity to the position of the specified entity.
	void									 Event_MoveTo( idEntity* ent );

	//! Moves the actor to the specified position.
	void									 Event_MoveToPos( idVec3& pos );

	//! Moves the mover in the specified direction by the given distance.
	void									 Event_MoveDir( float angle, float distance );

	//! Sets the mover to accelerate to a specified speed over a given time period.
	void									 Event_MoveAccelerateTo( float speed, float time );

	//! Stops the mover by decelerating to a specified speed over a given time period.
	void									 Event_MoveDecelerateTo( float speed, float time );

	//! Rotates the mover down to a specified angle on the given axis.
	void									 Event_RotateDownTo( int axis, float angle );

	//! Sets the rotation angle for a specified axis and begins the rotation animation.
	void									 Event_RotateUpTo( int axis, float angle );

	//! Sets the destination angles for rotation and begins the rotation movement.
	void									 Event_RotateTo( idAngles& angles );

	//! Rotates the mover by the specified angles over the move time.
	void									 Event_Rotate( idAngles& angles );

	//! Rotates the mover by the specified angles once and asynchronously.
	void									 Event_RotateOnce( idAngles& angles );

	//! Sets up bobbing motion for the mover with specified speed, phase, and depth.
	void									 Event_Bob( float speed, float phase, idVec3& depth );

	//! Sets up angular extrapolation for a sway motion with specified speed, phase, and depth angles.
	void									 Event_Sway( float speed, float phase, idAngles& depth );

	//! Sets the acceleration sound for the mover.
	void									 Event_SetAccelSound( const char* sound );

	//! Sets the deceleration sound for the mover.
	void									 Event_SetDecelSound( const char* sound );

	//! Sets the sound to play during movement.
	void									 Event_SetMoveSound( const char* sound );

	//! Finds GUI targets for the mover.
	void									 Event_FindGuiTargets();

	//! Initializes GUI targets for the mover.
	void									 Event_InitGuiTargets();

	//! Enables spline angle interpolation for the mover.
	void									 Event_EnableSplineAngles();

	//! Disables the use of spline angles for the mover.
	void									 Event_DisableSplineAngles();

	//! Removes the initial spline angles from the mover's physics object.
	void									 Event_RemoveInitialSplineAngles();

	//! Initializes and starts spline movement using the provided spline entity.
	void									 Event_StartSpline( idEntity* splineEntity );

	//! Stops the spline movement by clearing the spline object and entity reference.
	void									 Event_StopSpline();

	//! Shows the mover and starts its spline animation.
	void									 Event_Activate( idEntity* activator );

	/*!
		\brief Handles post-restore setup for a mover entity using spline data and timing parameters.

		This function is called after an entity is restored from a saved state. It retrieves the spline data from the associated spline entity, adjusts the spline's time alignment, and configures the
	   physics object with the spline and timing parameters. The function ensures the spline is uniform and properly aligned before setting up the mover's physics behavior.

		\param start The start time for the spline
		\param total The total duration for the spline
		\param accel The acceleration time for the movement
		\param decel The deceleration time for the movement
		\param useSplineAng Flag indicating whether to use spline angles for orientation
		\throws Warning message if the spline entity is invalid
	*/
	void									 Event_PostRestore( int start, int total, int accel, int decel, int useSplineAng );

	//! Returns whether the mover is currently moving.
	void									 Event_IsMoving();

	//! Returns whether the mover is currently rotating.
	void									 Event_IsRotating();
};

/*!
	\class idSplinePath
	\brief Represents a spline-based path entity that can be used for object movement or positioning.
*/
class idSplinePath : public idEntity
{
public:
	CLASS_PROTOTYPE( idSplinePath );

	//! Initializes a new instance of the idSplinePath class.
	idSplinePath();

	//! Initializes the spline path object.
	void Spawn();
};

struct floorInfo_s {
	idVec3 pos;
	idStr  door;
	int	   floor;
};

/*!
	\class idElevator
	\brief Manages elevator movement and door control within a game environment.

	The idElevator class handles the behavior of elevators, including movement between floors, door operations, and interaction with GUI systems. It inherits from idMover and provides functionality
   for spawning elevator entities, managing floor information, and controlling door states during movement. The class supports save and restore operations, GUI command handling, and various events
   such as touching, activation, and team blocking. It also manages the elevator's internal state during movement and ensures proper door operations at each floor.

*/
class idElevator : public idMover
{
public:
	CLASS_PROTOTYPE( idElevator );

	//! Initializes a new instance of the idElevator class with default values.
	idElevator();

	//! Initializes the elevator state and configuration.
	void		 Spawn();

	//! Saves the elevator state and associated floor information to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the elevator's state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Handles a single GUI command for the elevator entity.
	virtual bool HandleSingleGuiCommand( idEntity* entityGui, idLexer* src );

	//! Moves the elevator to the specified floor if the floor information is valid.
	void		 Event_GotoFloor( int floor );

	//! Retrieves the floor information structure for a specified floor number.
	floorInfo_s* GetFloorInfo( int floor );

protected:
	//! Completes the elevator movement by enabling doors and updating floor status GUIs.
	virtual void DoneMoving();

	//! Starts the elevator movement sequence
	virtual void BeginMove( idThread* thread = NULL );
	void		 SpawnTrigger( const idVec3& pos );

	//! Handles touch events for the elevator, triggering floor changes when appropriate.
	void		 Event_Touch( idEntity* other, trace_t* trace );

private:
	typedef enum { INIT, IDLE, WAITING_ON_DOORS } elevatorState_t;

	elevatorState_t				   state;
	idList<floorInfo_s, TAG_MOVER> floorInfo;
	int							   currentFloor;
	int							   pendingFloor;
	int							   lastFloor;
	bool						   controlsDisabled;
	float						   returnTime;
	int							   returnFloor;
	int							   lastTouchTime;

	//! Returns the door entity with the specified name, resolving the move master if necessary.
	class idDoor*				   GetDoor( const char* name );

	//! Updates the elevator's state and position based on its current floor and target floor.
	void						   Think();

	//! Opens the inner door of the elevator.
	void						   OpenInnerDoor();

	//! Opens the door associated with the specified floor in the elevator.
	void						   OpenFloorDoor( int floor );

	//! Closes all doors associated with the elevator.
	void						   CloseAllDoors();

	//! Disables all doors associated with the elevator.
	void						   DisableAllDoors();

	//! Enables the appropriate doors for the elevator based on the current floor.
	void						   EnableProperDoors();

	//! Handles the event when the elevator is blocked by a team entity.
	void						   Event_TeamBlocked( idEntity* blockedEntity, idEntity* blockingEntity );

	//! Handles the activation event for an elevator, triggering a floor change if certain conditions are met.
	void						   Event_Activate( idEntity* activator );

	//! Handles post-floor arrival events for the elevator by opening doors, updating GUI states, and scheduling return to a specified floor.
	void						   Event_PostFloorArrival();

	//! Sets the GUI states based on the current floor position.
	void						   Event_SetGuiStates();
};

/*
===============================================================================

  Binary movers.

===============================================================================
*/

typedef enum { MOVER_POS1, MOVER_POS2, MOVER_1TO2, MOVER_2TO1 } moverState_t;

/*!
	\class idMover_Binary
	\brief A binary mover entity that transitions between two defined positions with configurable movement parameters.

	The idMover_Binary class implements a movable entity that can transition between two specific positions, supporting both speed-based and time-based movement initialization. It manages physics
   state, team binding, and GUI interactions for coordinated movement behaviors. The class supports enabling/disabling movement, setting blocked states, and handling activation events that trigger
   position transitions. The mover can be initialized with either a target speed or total movement time, and includes acceleration and deceleration timing controls. Movement is controlled through
   various event handlers and state management functions that coordinate with team entities and visual feedback systems.

*/
class idMover_Binary : public idEntity
{
public:
	CLASS_PROTOTYPE( idMover_Binary );

	//! Initializes a new instance of the idMover_Binary class with default values.
	idMover_Binary();

	//! Destructor for the idMover_Binary class that manages removal from the move master chain.
	~idMover_Binary();

	//! Initializes the binary mover entity and sets up its physics and team connections.
	void			Spawn();

	//! Saves the binary mover state to the provided save file.
	void			Save( idSaveGame* savefile ) const;

	//! Restores the state of the binary mover from a save file.
	void			Restore( idRestoreGame* savefile );

	//! Converts local position coordinates to world coordinates for both pos1 and pos2.
	virtual void	PreBind();

	//! Updates the local coordinates of pos1 and pos2 after binding.
	virtual void	PostBind();

	//! Enables or disables the binary mover based on the provided boolean value.
	void			Enable( bool b );

	/*!
		\brief Initializes the movement parameters for a binary mover between two positions with specified speed and acceleration/deceleration times

		Configures the mover to travel between two positions using the provided speed and timing parameters. The function calculates the travel duration based on distance and speed, and sets up the
	   physics object for linear movement. If no speed is provided, a default speed of 100 is used. The acceleration and deceleration times are snapped to physics frame boundaries for consistent
	   timing. The mover is initialized in the first position and ready for movement

		\param mpos1 Starting position of the mover
		\param mpos2 Ending position of the mover
		\param mspeed Speed of movement in units per second
		\param maccelTime Time to reach full speed during acceleration phase in seconds
		\param mdecelTime Time to reduce speed to zero during deceleration phase in seconds
	*/
	void			InitSpeed( idVec3& mpos1, idVec3& mpos2, float mspeed, float maccelTime, float mdecelTime );

	/*!
		\brief Initializes the binary mover with start and end positions and timing parameters for movement

		Sets up the mover to transition between two positions over a specified time period with acceleration and deceleration phases. The function calculates the appropriate physics frame durations
	   for the movement and initializes the mover state. It also configures the physics object for linear extrapolation and interpolation, and sets the initial position of the mover to the first
	   position.

		\param mpos1 Starting position of the mover
		\param mpos2 Ending position of the mover
		\param mtime Total time for the movement between positions
		\param maccelTime Time duration for the acceleration phase
		\param mdecelTime Time duration for the deceleration phase
	*/
	void			InitTime( idVec3& mpos1, idVec3& mpos2, float mtime, float maccelTime, float mdecelTime );

	//! Moves the binary mover to position 1.
	void			GotoPosition1();

	//! Moves the binary mover to position 2.
	void			GotoPosition2();

	//! Toggles the binary mover state between positions 1 and 2 in response to an activation event.
	void			Use_BinaryMover( idEntity* activator );

	//! Sets the GUI state for the mover and its activation chain.
	void			SetGuiStates( const char* state );

	//! Updates the shader parameter and visual state of associated entities.
	void			UpdateBuddies( int val );

	//! Returns the activate chain associated with this mover.
	idMover_Binary* GetActivateChain() const { return activateChain; }

	//! Returns the master mover that controls this mover.
	idMover_Binary* GetMoveMaster() const { return moveMaster; }

	//! Binds all entities in a mover team to the specified entity.
	void			BindTeam( idEntity* bindTo );

	//! Sets the blocked state for a binary mover and its slaves, optionally triggering events.
	void			SetBlocked( bool b );

	//! Returns true if the mover is currently blocked.
	bool			IsBlocked();

	//! Returns the entity that activated this binary mover.
	idEntity*		GetActivator() const;

	//! Writes the binary mover's state and binding information to a snapshot message.
	virtual void	WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the mover's state and physics information from a snapshot message.
	virtual void	ReadFromSnapshot( const idBitMsg& msg );

	//! Sets the portal state to open or closed.
	void			SetPortalState( bool open );

protected:
	idVec3									 pos1;
	idVec3									 pos2;
	moverState_t							 moverState;
	idMover_Binary*							 moveMaster;
	idMover_Binary*							 activateChain;
	int										 soundPos1;
	int										 sound1to2;
	int										 sound2to1;
	int										 soundPos2;
	int										 soundLoop;
	float									 wait;
	float									 damage;
	int										 duration;
	int										 accelTime;
	int										 decelTime;
	idEntityPtr<idEntity>					 activatedBy;
	int										 stateStartTime;
	idStr									 team;
	bool									 enabled;
	int										 move_thread;
	int										 updateStatus; // 1 = lock behaviour, 2 = open close status
	idStrList								 buddies;
	idPhysics_Parametric					 physicsObj;
	qhandle_t								 areaPortal; // 0 = no portal
	bool									 blocked;
	bool									 playerOnly;
	idList<idEntityPtr<idEntity>, TAG_MOVER> guiTargets;

	//! Sets the mover state for all entities in a mover team with the specified time.
	void									 MatchActivateTeam( moverState_t newstate, int time );

	//! Links this mover to the activation chain of the specified master mover.
	void									 JoinActivateTeam( idMover_Binary* master );

	//! Updates the sound state of the mover based on the provided mover state.
	void									 UpdateMoverSound( moverState_t state );

	//! Sets the mover state and updates the mover sound and physics properties.
	void									 SetMoverState( moverState_t newstate, int time );

	//! Returns the current state of the binary mover.
	moverState_t							 GetMoverState() const { return moverState; }

	//! Finds GUI targets for the binary mover.
	void									 FindGuiTargets();

	//! Sets the GUI state string for all GUIs associated with the mover.
	void									 SetGuiState( const char* key, const char* val ) const;

	//! Sets a callback for the binary mover.
	void									 Event_SetCallback();

	//! Initiates the movement of the binary mover back to its initial position.
	void									 Event_ReturnToPos1();

	//! Handles the use event for a binary mover, activating its movement functionality.
	void									 Event_Use_BinaryMover( idEntity* activator );

	//! Handles the event when a binary mover reaches its target position.
	void									 Event_Reached_BinaryMover();

	//! Handles the event to match and activate a team with the specified state and time.
	void									 Event_MatchActivateTeam( moverState_t newstate, int time );

	//! Enables all entities in a mover team
	void									 Event_Enable();

	//! Disables all entities in a mover team.
	void									 Event_Disable();

	//! Opens the portal associated with this mover
	void									 Event_OpenPortal();

	//! Closes the portal associated with this mover and its slave entities.
	void									 Event_ClosePortal();

	//! Finds GUI targets for the binary mover.
	void									 Event_FindGuiTargets();

	//! Initializes GUI targets for the binary mover based on the number of GUI targets.
	void									 Event_InitGuiTargets();

	//! Sets the movedir vector based on the given angle, handling special cases for up and down directions.
	static void								 GetMovedir( float dir, idVec3& movedir );
};

/*!
	\class idDoor
	\brief Represents a door entity with movement, locking, and trigger behaviors.

	The idDoor class implements a door entity that can open and close with associated movement logic, locking mechanisms, and trigger interactions. It inherits from idMover_Binary and extends
   functionality to support complex door behaviors including team-based operations, touch detection, and portal control. The class manages door state through various methods for opening, closing,
   locking, and interacting with triggers and sound effects. It supports both client and server-side updates, and provides methods for handling events such as reaching positions, being blocked, or
   being activated. The door can be configured to respond only to players, and supports companion doors for synchronized operations. The class also handles AAS area state management for navigation
   purposes.

*/
class idDoor : public idMover_Binary
{
public:
	CLASS_PROTOTYPE( idDoor );

	//! Initializes a new instance of the idDoor class with default values.
	idDoor();

	//! Destructor for the idDoor class that cleans up trigger and sound trigger resources.
	~idDoor();

	//! Initializes the door entity with its properties and movement parameters.
	void		 Spawn();

	//! Saves the door's state to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the door's state from a saved game file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the door's client-side state and trigger positions based on the current time and physics.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Updates the door's trigger positions during the think cycle.
	virtual void Think();

	//! Initializes the door's bind state before binding occurs.
	virtual void PreBind();

	//! Initializes the door's trigger position after binding.
	virtual void PostBind();

	//! Hides the door and its associated slave entities, disabling triggers and physics.
	virtual void Hide();

	//! Displays the door and all associated slave components in the game world.
	virtual void Show();

	//! Returns true if the door is currently open, false otherwise.
	bool		 IsOpen();

	//! Returns whether the door is configured to not respond to touch events.
	bool		 IsNoTouch();

	//! Determines whether a given entity is allowed to interact with the door when it is set to player-only mode.
	bool		 AllowPlayerOnly( idEntity* ent );

	//! Returns the locked state of the door.
	int			 IsLocked();

	//! Locks or unlocks the door and its team members based on the specified flag.
	void		 Lock( int f );

	//! Activates the door entity and triggers associated targets if requirements are met.
	void		 Use( idEntity* other, idEntity* activator );

	//! Closes the door by moving to position 1.
	void		 Close();

	//! Opens the door by moving it to its open position.
	void		 Open();

	//! Sets the companion door for this door.
	void		 SetCompanion( idDoor* door );

private:
	float		 triggersize;
	bool		 crusher;
	bool		 noTouch;
	bool		 aas_area_closed;
	idStr		 buddyStr;
	idClipModel* trigger;
	idClipModel* sndTrigger;
	int			 nextSndTriggerTime;
	idVec3		 localTriggerOrigin;
	idMat3		 localTriggerAxis;
	idStr
		requires;
	int		removeItem;
	idStr	syncLock;
	int		normalAxisIndex; // door faces X or Y for spectator teleports
	idDoor* companionDoor;

	//! Sets the AAS area state to closed or open.
	void	SetAASAreaState( bool closed );

	//! Sets the local trigger position relative to the door's master position.
	void	GetLocalTriggerPosition( const idClipModel* trigger );

	//! Calculates trigger bounds for a door based on size and existing bounds.
	void	CalcTriggerBounds( float size, idBounds& bounds );

	//! Handles the event when a door reaches its binary mover state.
	void	Event_Reached_BinaryMover();

	//! Handles the event when the door is blocked by a team entity.
	void	Event_TeamBlocked( idEntity* blockedEntity, idEntity* blockingEntity );

	//! Handles the event when a door part is blocked during operation.
	void	Event_PartBlocked( idEntity* blockingEntity );

	//! Handles touch events for a door, triggering interactions based on the entity that touched it.
	void	Event_Touch( idEntity* other, trace_t* trace );

	//! Handles the activation event for a door entity, managing locking, syncing, and triggering behaviors.
	void	Event_Activate( idEntity* activator );

	//! Reverses the door's start and end positions and initializes its movement based on specified speed or time.
	void	Event_StartOpen();

	//! Creates a trigger that encloses all spawned door parts.
	void	Event_SpawnDoorTrigger();

	//! Spawns a sound trigger for the door to activate locked sounds.
	void	Event_SpawnSoundTrigger();

	//! Closes the door.
	void	Event_Close();

	//! Opens the door.
	void	Event_Open();

	//! Locks the door with the specified lock level.
	void	Event_Lock( int f );

	//! Returns the open state of the door.
	void	Event_IsOpen();

	//! Returns the locked state of the door as a float.
	void	Event_Locked();

	//! Handles spectator touch events for door entities
	void	Event_SpectatorTouch( idEntity* other, trace_t* trace );

	//! Opens the portal associated with this door.
	void	Event_OpenPortal();

	//! Closes the portal associated with this door.
	void	Event_ClosePortal();
};

/*!
	\class idPlat
	\brief A platform entity that manages movement and interaction with triggers in a game environment.

	The idPlat class represents a movable platform entity that handles its own physics, trigger interactions, and client-side rendering updates. It inherits from idMover_Binary and extends
   platform-specific functionality including spawn initialization, physics evaluation, and event handling for blocking and touch interactions. The class manages a trigger clip model for detecting when
   entities interact with the platform, and supports both server and client-side operations including prediction and interpolation. The platform's behavior is controlled through spawn arguments and
   can be saved and restored during game sessions.

*/
class idPlat : public idMover_Binary
{
public:
	CLASS_PROTOTYPE( idPlat );

	//! Constructs a new idPlat object with default values.
	idPlat();

	//! Destructor for the idPlat class that cleans up the trigger object if it exists.
	~idPlat();

	//! Initializes the platform's movement and trigger based on spawn arguments.
	void		 Spawn();

	//! Saves the platform's trigger data to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the platform's state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Executes physics evaluation for the platform entity without blocking operations.
	void		 RunPhysics_NoBlocking();

	//! Updates the platform's client-side state including physics interpolation and rendering.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Updates the platform's physics and trigger position.
	virtual void Think();

	//! Initializes platform-specific bindings before other setup steps.
	virtual void PreBind();

	//! Performs post-binding operations for the platform, including updating the local trigger position.
	virtual void PostBind();

private:
	idClipModel* trigger;
	idVec3		 localTriggerOrigin;
	idMat3		 localTriggerAxis;

	//! Calculates and stores the local position and axis of a trigger clip model relative to the platform's master position.
	void		 GetLocalTriggerPosition( const idClipModel* trigger );

	//! Creates and links a trigger clip model for a platform at the specified position.
	void		 SpawnPlatTrigger( idVec3& pos );

	//! Handles the event when a platform entity is blocked by another entity.
	void		 Event_TeamBlocked( idEntity* blockedEntity, idEntity* blockingEntity );

	//! Handles the event when a platform part is blocked during movement.
	void		 Event_PartBlocked( idEntity* blockingEntity );

	//! Handles touch events for platform entities, triggering movement when a player interacts with the platform.
	void		 Event_Touch( idEntity* other, trace_t* trace );
};

/*!
	\class idMover_Periodic
	\brief A periodic mover entity that handles repetitive motion behavior with team and part blocking events.

	This class implements a mover entity that follows a periodic motion pattern, managing its state through various lifecycle events including spawning, thinking, and snapshot serialization. It
   supports team-based collision handling and individual part blocking events. The implementation inherits from idEntity, integrating with the engine's entity system for physics simulation and
   rendering. The mover maintains its state across save and restore operations, and handles network synchronization through snapshot messages. The periodic motion is executed through the Think method,
   which manages dormant states, physics updates, and visual presentation.

*/
class idMover_Periodic : public idEntity
{
public:
	CLASS_PROTOTYPE( idMover_Periodic );

	//! Constructs an idMover_Periodic object with default values.
	idMover_Periodic();

	//! Initializes the periodic mover component.
	void		 Spawn();

	//! Saves the periodic mover's state to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the periodic mover from a saved game file.
	void		 Restore( idRestoreGame* savefile );

	//! Executes the periodic mover's think logic, including dormant checks, physics updates, and presentation.
	virtual void Think();

	//! Writes the mover periodic's state to a snapshot message.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the mover's state from a snapshot message and updates visuals if changed.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

protected:
	idPhysics_Parametric physicsObj;
	float				 damage;

	//! Handles the event when a team member is blocked by another entity.
	void				 Event_TeamBlocked( idEntity* blockedEntity, idEntity* blockingEntity );

	//! Handles the event when a periodic mover part is blocked by another entity.
	void				 Event_PartBlocked( idEntity* blockingEntity );
};

/*!
	\class idRotater
	\brief A rotating entity that can be activated to start or stop its rotational motion.
*/
class idRotater : public idMover_Periodic
{
public:
	CLASS_PROTOTYPE( idRotater );

	//! Initializes an idRotater object with its activation source set to itself.
	idRotater();

	//! Initializes the rotater physics object and sets its properties.
	void Spawn();

	//! Saves the activatedBy member of the idRotater class to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the rotater from a save file
	void Restore( idRestoreGame* savefile );

private:
	idEntityPtr<idEntity> activatedBy;

	//! Activates or deactivates the rotater entity, starting or stopping its rotational motion.
	void				  Event_Activate( idEntity* activator );
};

/*!
	\class idBobber
	\brief A class that handles periodic bobbing motion for entities.
*/
class idBobber : public idMover_Periodic
{
public:
	CLASS_PROTOTYPE( idBobber );

	//! Initializes a new instance of the idBobber class.
	idBobber();

	//! Initializes the bobbing physics for the entity based on spawn arguments.
	void Spawn();

private:
};

/*!
	\class idPendulum
	\brief A class that implements pendulum movement behavior for game entities.
*/
class idPendulum : public idMover_Periodic
{
public:
	CLASS_PROTOTYPE( idPendulum );

	//! Initializes a new instance of the idPendulum class.
	idPendulum();

	//! Initializes the pendulum physics and parameters based on spawn arguments.
	void Spawn();

private:
};

/*!
	\class idRiser
	\brief A mover entity that performs linear vertical oscillation with optional visibility toggling.
*/
class idRiser : public idMover_Periodic
{
public:
	CLASS_PROTOTYPE( idRiser );

	//! Constructs a new instance of the idRiser class.
	idRiser();

	//! Initializes the physics object and sets up the mover's properties.
	void Spawn();

private:
	//! Toggles the visibility of the riser and initiates a linear movement animation when activated.
	void Event_Activate( idEntity* activator );
};

#endif /* !__GAME_MOVER_H__ */
