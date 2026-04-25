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

#ifndef __PHYSICS_MONSTER_H__
#define __PHYSICS_MONSTER_H__

/*
===================================================================================

	Monster physics

	Simulates the motion of a monster through the environment. The monster motion
	is typically driven by animations.

===================================================================================
*/

typedef enum { MM_OK, MM_SLIDING, MM_BLOCKED, MM_STEPPED, MM_FALLING } monsterMoveResult_t;

typedef struct monsterPState_s {
	int	   atRest;
	bool   onGround;
	idVec3 origin;
	idVec3 velocity;
	idVec3 localOrigin;
	idVec3 pushVelocity;
} monsterPState_t;

/*!
	\class idPhysics_Monster
	\brief Handles the physics simulation for monster entities with specialized movement capabilities.

	This class extends actor physics to provide specialized behavior for monster entities, including step height management, movement modes, and impact handling. It supports various physics operations
   such as movement evaluation, state saving and restoring, and network synchronization. The class manages the monster's position, velocity, and interaction with the environment through methods for
   sliding, stepping, and ground checking. It also provides functionality for setting and retrieving physics state information and handling entity interactions through impact and impulse mechanisms.

*/
class idPhysics_Monster : public idPhysics_Actor
{
public:
	CLASS_PROTOTYPE( idPhysics_Monster );

	//! Initializes a new instance of the idPhysics_Monster class with default values.
	idPhysics_Monster();

	//! Saves the monster physics state to the provided save file
	void				Save( idSaveGame* savefile ) const;

	//! Restores the monster physics state from a saved game file.
	void				Restore( idRestoreGame* savefile );

	//! Sets the maximum height a monster can step up.
	void				SetMaxStepHeight( const float newMaxStepHeight );

	//! Returns the maximum height a monster can step over.
	float				GetMaxStepHeight() const;
	// minimum cosine of floor angle to be able to stand on the floor
	void				SetMinFloorCosine( const float newMinFloorCosine );

	//! Sets the delta displacement for the next move.
	void				SetDelta( const idVec3& d );

	//! Returns true if the monster is standing on the ground.
	bool				OnGround() const;

	//! Returns the movement result of the monster physics simulation.
	monsterMoveResult_t GetMoveResult() const;

	//! Sets whether to force delta movement by overriding velocity.
	void				ForceDeltaMove( bool force );

	//! Sets whether the monster should use fly movement mode.
	void				UseFlyMove( bool force );

	//! Sets whether to use velocity movement for the monster physics.
	void				UseVelocityMove( bool force );

	//! Returns the entity that is blocking the monster's movement.
	idEntity*			GetSlideMoveEntity() const;

	//! Enables impact activation for the monster physics object.
	void				EnableImpact();

	//! Disables the impact functionality for the monster physics object.
	void				DisableImpact();

public:
	// common physics interface ----------------------

	//! Evaluates the monster's physics state and updates its position and velocity over a given time step.
	bool		  Evaluate( int timeStepMSec, int endTimeMSec );

	//! Updates the physics simulation time to the specified end time in milliseconds.
	void		  UpdateTime( int endTimeMSec );

	//! Returns the current game time value.
	int			  GetTime() const;

	//! Retrieves impact information for a specified id and point
	void		  GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to the monster physics object at a specified point.
	void		  ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Activates the monster physics and marks it as not at rest.
	void		  Activate();

	//! Puts the monster physics object to rest.
	void		  PutToRest();

	//! Returns true if the monster physics object is at rest.
	bool		  IsAtRest() const;

	//! Returns the start time for the rest state.
	int			  GetRestStartTime() const;

	//! Saves the current physics state to the saved state.
	void		  SaveState();

	//! Restores the physics state of the monster from a previously saved state.
	void		  RestoreState();

	//! Sets the origin position of the monster physics object, optionally relative to a master entity.
	void		  SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the monster's clip model to the specified new axis.
	void		  SetAxis( const idMat3& newAxis, int id = -1 );

	//! Moves the monster by the specified translation vector.
	void		  Translate( const idVec3& translation, int id = -1 );

	//! Applies a rotation to the monster physics object and updates its position and clip model accordingly.
	void		  Rotate( const idRotation& rotation, int id = -1 );

	//! Sets the linear velocity of the monster physics object to the specified value.
	void		  SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Returns the linear velocity of the monster physics object.
	const idVec3& GetLinearVelocity( int id = 0 ) const;

	//! Updates the push velocity of the monster based on the change in origin over the specified time delta.
	void		  SetPushed( int deltaTime );

	//! Returns the linear velocity of the monster's push.
	const idVec3& GetPushedLinearVelocity( const int id = 0 ) const;

	//! Sets the master entity for this monster physics object, optionally orienting it relative to the master.
	void		  SetMaster( idEntity* master, const bool orientated = true );

	//! Writes the monster's physics state to a network snapshot message
	void		  WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads monster physics state from a network snapshot message.
	void		  ReadFromSnapshot( const idBitMsg& msg );

private:
	// monster physics state
	monsterPState_t		current;
	monsterPState_t		saved;

	// client interpolation state
	monsterPState_t		previous;
	monsterPState_t		next;

	// properties
	float				maxStepHeight;	// maximum step height
	float				minFloorCosine; // minimum cosine of floor angle
	idVec3				delta;			// delta for next move

	bool				forceDeltaMove;
	bool				fly;
	bool				useVelocityMove;
	bool				noImpact; // if true do not activate when another object collides

	// results of last evaluate
	monsterMoveResult_t moveResult;
	idEntity*			blockingEntity;

private:
	//! Checks if the monster is touching the ground based on physics calculations.
	void				CheckGround( monsterPState_t& state );

	//! Moves a monster along a sliding path while handling collisions and returns the result of the movement operation.
	monsterMoveResult_t SlideMove( idVec3& start, idVec3& velocity, const idVec3& delta );

	//! Moves the physics object along a delta direction while handling stepping over obstacles and collision detection.
	monsterMoveResult_t StepMove( idVec3& start, idVec3& velocity, const idVec3& delta );

	//! Sets the monster to a resting state with zero velocity and deactivates physics.
	void				Rest();
};

#endif /* !__PHYSICS_MONSTER_H__ */
