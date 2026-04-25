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

#ifndef __PHYSICS_PLAYER_H__
#define __PHYSICS_PLAYER_H__

/*
===================================================================================

	Player physics

	Simulates the motion of a player through the environment. Input from the
	player is used to allow a certain degree of control over the motion.

===================================================================================
*/

// movementType
typedef enum {
	PM_NORMAL,	  // normal physics
	PM_DEAD,	  // no acceleration or turning, but free falling
	PM_SPECTATOR, // flying without gravity but with collision detection
	PM_FREEZE,	  // stuck in place without control
	PM_NOCLIP	  // flying without collision detection nor gravity
} pmtype_t;

typedef enum { WATERLEVEL_NONE, WATERLEVEL_FEET, WATERLEVEL_WAIST, WATERLEVEL_HEAD } waterLevel_t;

#define MAXTOUCH 32

typedef struct playerPState_s {
	idVec3 origin;
	idVec3 velocity;
	idVec3 localOrigin;
	idVec3 pushVelocity;
	float  stepUp;
	int	   movementType;
	int	   movementFlags;
	int	   movementTime;

	//! Initializes a new instance of playerPState_s with default values.
	playerPState_s() :
		origin( vec3_zero ),
		velocity( vec3_zero ),
		localOrigin( vec3_zero ),
		pushVelocity( vec3_zero ),
		stepUp( 0.0f ),
		movementType( 0 ),
		movementFlags( 0 ),
		movementTime( 0 )
	{
	}
} playerPState_t;

/*!
	\class idPhysics_Player
	\brief The idPhysics_Player class encapsulates the physics behavior and movement mechanics for player characters in the game.

	This class manages player-specific physics including movement types, collision detection, stepping behavior, and various game state interactions such as jumping, ducking, and ladder climbing. It
   handles the evaluation of physics updates, state saving and restoration, and network synchronization. The class supports different movement modes like walking, flying, swimming, and noclip, and
   provides methods for applying forces, handling impacts, and managing player input. It also tracks player-specific states such as water level, jump history, and whether the player is crouching or on
   a ladder.

*/
class idPhysics_Player : public idPhysics_Actor
{
public:
	CLASS_PROTOTYPE( idPhysics_Player );

	//! Initializes a new instance of the idPhysics_Player class with default values.
	idPhysics_Player();

	//! Saves the player physics state to a save file
	void		  Save( idSaveGame* savefile ) const;

	//! Restores the player physics state from a saved game file.
	void		  Restore( idRestoreGame* savefile );

	//! Sets the walk and crouch speeds for the player physics.
	void		  SetSpeed( const float newWalkSpeed, const float newCrouchSpeed );

	//! Sets the maximum height that the player can step over.
	void		  SetMaxStepHeight( const float newMaxStepHeight );

	//! Returns the maximum height the player can step over.
	float		  GetMaxStepHeight() const;

	//! Sets the maximum height that can be jumped.
	void		  SetMaxJumpHeight( const float newMaxJumpHeight );

	//! Sets the movement type for the player physics.
	void		  SetMovementType( const pmtype_t type );

	//! Sets the player input consisting of command, forward vector, and face forward matrix.
	void		  SetPlayerInput( const usercmd_t& cmd, const idVec3& forwardVector, const idMat3& faceForward );

	//! Sets the knockback time for the player physics.
	void		  SetKnockBack( const int knockBackTime );

	//! Sets the debug level for the player physics.
	void		  SetDebugLevel( bool set );

	//! Returns the water level detected in the last physics frame.
	waterLevel_t  GetWaterLevel() const;

	//! Returns the water type identifier for the player physics.
	int			  GetWaterType() const;

	//! Returns true if the player has jumped during the current physics update.
	bool		  HasJumped() const;

	//! Returns true if the player has stepped up or down during the current physics update.
	bool		  HasSteppedUp() const;

	//! Returns the current step-up height value used by the player physics.
	float		  GetStepUp() const;

	//! Returns true if the player is currently crouching.
	bool		  IsCrouching() const;

	//! Returns true if the player is currently on a ladder.
	bool		  OnLadder() const;

	//! Returns the player's current origin position.
	const idVec3& PlayerGetOrigin() const;

public: // common physics interface
		//! Evaluates the player physics state for a given time step and end time, updating the player's position and velocity.
	bool		  Evaluate( int timeStepMSec, int endTimeMSec );

	//! Interpolates the player physics state based on the given fraction.
	bool		  Interpolate( const float fraction );

	//! Updates the physics simulation time for the player physics object.
	void		  UpdateTime( int endTimeMSec );

	//! Returns the current game time value
	int			  GetTime() const;

	//! Retrieves impact information for a given object id and point.
	void		  GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to the player physics object at a specific point.
	void		  ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Returns false indicating the player is not at rest.
	bool		  IsAtRest() const;

	//! Returns the start time for when the player physics simulation entered a resting state.
	int			  GetRestStartTime() const;

	//! Saves the current physics state to the saved state.
	void		  SaveState();

	//! Restores the physics state from a previously saved state.
	void		  RestoreState();

	//! Updates the physics origin of the player, optionally considering a master entity for the position.
	void		  SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the physics player object and updates its link in the game clip.
	void		  SetAxis( const idMat3& newAxis, int id = -1 );

	//! Updates the player's position by translating it according to the provided vector.
	void		  Translate( const idVec3& translation, int id = -1 );

	//! Applies a rotation to the player physics object and updates its position and clip model accordingly.
	void		  Rotate( const idRotation& rotation, int id = -1 );

	//! Sets the linear velocity of the player physics object to the specified value.
	void		  SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Returns the linear velocity of the player physics object.
	const idVec3& GetLinearVelocity( int id = 0 ) const;

	//! Determines if the client is currently locked by a pusher and updates the unlock status.
	bool		  ClientPusherLocked( bool& justBecameUnlocked );

	//! Updates the player's push velocity based on the delta time and movement since the last update.
	void		  SetPushed( int deltaTime );

	//! Sets the pushed velocity for the player, with a hack to ignore abnormal velocity changes.
	void		  SetPushedWithAbnormalVelocityHack( int deltaTime );

	//! Returns the pushed linear velocity of the player physics object.
	const idVec3& GetPushedLinearVelocity( const int id = 0 ) const;

	//! Clears the pushed velocity of the player physics.
	void		  ClearPushedVelocity();

	//! Sets the master entity for this player physics object, optionally orienting it relative to the master.
	void		  SetMaster( idEntity* master, const bool orientated = true );

	//! Writes player physics state to a snapshot message
	void		  WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads player physics state from a network snapshot message.
	void		  ReadFromSnapshot( const idBitMsg& msg );

	//! Transitions the player physics state to the next state.
	void		  SnapToNextState()
	{
		current	 = next;
		previous = current;
	}

	//! Returns whether the player should blink.
	bool		  ShouldBlink() const { return blink; }

	//! Returns the head offset vector for the player physics object.
	const idVec3& GetHeadOffset() const { return headOrigin; }
	// Leyland end

private:
	// player physics state
	playerPState_t	  current;
	playerPState_t	  saved;

	// physics state for client interpolation
	playerPState_t	  previous;
	playerPState_t	  next;

	// Leyland VR
	idMat3			  vrFaceForward;
	bool			  vrHadHeadOrigin;
	idVec3			  vrLastHeadOrigin;
	idVec3			  vrDelta;
	idVec3			  headOrigin;
	bool			  blink;
	bool			  headBumped;
	// Leyland end

	// properties
	float			  walkSpeed;
	float			  crouchSpeed;
	float			  maxStepHeight;
	float			  maxJumpHeight;
	int				  debugLevel; // if set, diagnostic output will be printed

	// player input
	usercmd_t		  command;
	idVec3			  commandForward; // can't use cmd.angles cause of the delta_angles and head tracking

	// run-time variables
	int				  framemsec;
	float			  frametime;
	float			  playerSpeed;
	idVec3			  viewForward;
	idVec3			  viewRight;

	// walk movement
	bool			  walking;
	bool			  groundPlane;
	trace_t			  groundTrace;
	const idMaterial* groundMaterial;

	// ladder movement
	bool			  ladder;
	idVec3			  ladderNormal;

	// results of last evaluate
	waterLevel_t	  waterLevel;
	int				  waterType;

	bool			  clientPusherLocked;

private:
	//! Returns the scale factor to apply to command movements to normalize speed distortion
	float CmdScale( const usercmd_t& cmd ) const;

	//! Applies acceleration to the player's velocity in the direction of the desired movement.
	void  Accelerate( const idVec3& wishdir, const float wishspeed, const float accel );

	/*!
		\brief Performs a slide movement for the player physics, handling collisions, stepping, and pushing.

		This function executes a physics-based movement for a player character, taking into account gravity, stepping up and down, and pushing other entities. It calculates the player's new position
	   based on velocity and collision detection, adjusting for various physical constraints such as ground planes, clip planes, and step heights. The function handles multiple collision responses,
	   including sliding along surfaces, stepping over obstacles, and applying forces to movable objects. It also manages the player's movement flags and updates the character's velocity and position
	   accordingly during the movement process.

		\param gravity Whether to apply gravity to the player's velocity during the movement
		\param stepUp Whether to allow the player to step up over obstacles
		\param stepDown Whether to allow the player to step down when moving near edges
		\param push Whether to push other entities that the player collides with
		\return True if the movement completed successfully without getting stuck, false otherwise
	*/
	bool  SlideMove( bool gravity, bool stepUp, bool stepDown, bool push );

	//! Applies friction to the player's velocity based on movement type and surface conditions.
	void  Friction();

	//! Performs a water jump move where the player exits the water with gravity applied.
	void  WaterJumpMove();

	//! Handles the player movement logic when in water.
	void  WaterMove();

	//! Applies flying movement physics to the player character.
	void  FlyMove();

	//! Applies air movement physics to the player character.
	void  AirMove();

	//! Handles player movement when walking on the ground.
	void  WalkMove();

	//! Applies friction to the player's velocity when the player is not walking.
	void  DeadMove();

	//! Performs noclip movement for the player physics object.
	void  NoclipMove();

	//! Handles the movement logic for spectator mode in the player physics simulation.
	void  SpectatorMove();

	//! Handles the player's movement while clinging to a ladder, including upward, downward, and sideward motion.
	void  LadderMove();

	//! Corrects a collision where the player is stuck inside a solid object by resetting the trace data.
	void  CorrectAllSolid( trace_t& trace, int contents );

	//! Checks if the player is touching the ground and updates relevant physics state
	void  CheckGround();

	//! Checks if the player should duck or stand up based on collision detection and updates the clip model size accordingly.
	void  CheckDuck();

	//! Checks if the player is near a ladder surface and sets ladder state accordingly.
	void  CheckLadder();

	//! Checks if a jump command is valid and executes it.
	bool  CheckJump();

	//! Checks if the player can perform a water jump and initiates the jump if possible.
	bool  CheckWaterJump();

	//! Sets the water level state based on the player's position and bounding box.
	void  SetWaterLevel();

	//! Drops the player movement timers by decrementing or resetting them based on the frame time.
	void  DropTimers();

	//! Moves the player entity based on input commands and current physics state.
	void  MovePlayer( int msec );
};

#endif /* !__PHYSICS_PLAYER_H__ */
