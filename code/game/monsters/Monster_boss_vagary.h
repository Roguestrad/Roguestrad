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

// Monster_boss_vagary.h
//

#pragma once

/*!
	\class iceMonsterBossVagary
	\brief Represents a specialized AI boss entity with ranged and melee combat capabilities.

	This class implements a boss-level AI entity that utilizes both melee and ranged attack strategies. The entity manages its own state machine for combat behaviors including idle, movement, dodging,
   melee attacks, and ranged attacks using throwable objects. The AI system handles enemy detection, pathfinding, and tactical decision making. The class extends base AI functionality with specialized
   combat behavior for a boss enemy type.

*/
class iceMonsterBossVagary : public idAI
{
public:
	CLASS_PROTOTYPE( iceMonsterBossVagary );

	//! Initializes the AI system
	virtual void Init() override;

	//! Sets the AI state to "state_Begin" for the monster boss.
	virtual void AI_Begin() override;

	//! Returns attack flags indicating which attacks are available for the monster boss.
	virtual int	 check_attacks() override;

	//! Performs an attack action based on the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	/*!
		\brief Selects a suitable movable entity within specified bounds to throw at an enemy.

		This function identifies movable entities within a given bounding volume that are not hidden and sufficiently distant from the enemy. It then evaluates the trajectory of each candidate entity
	   to ensure a valid throw path can be calculated. The function returns a randomly selected entity from those that pass all checks, or NULL if no suitable entity is found.

		\param mins Minimum bounds of the volume to check for entities
		\param maxs Maximum bounds of the volume to check for entities
		\param speed The speed at which the object will be thrown
		\param minDist Minimum distance from the enemy for an object to be considered
		\param offset Vertical offset applied to the object's position when calculating trajectory
		\return A pointer to the chosen entity that can be thrown, or NULL if no suitable entity is found.
		\throws NULL is returned if no valid entity is found, or if enemy is not valid.
	*/
	idEntity* ChooseObjectToThrow( const idVec3& mins, const idVec3& maxs, float speed, float minDist, float offset );

	//! Throws an object at the enemy entity with the specified speed.
	void	  ThrowObjectAtEnemy( idEntity* ent, float speed );

private:
	//! Initializes the boss vagary state by setting up animation states and move type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state logic for the monster boss, transitioning to combat state when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Executes a right dodge combat move for the monster boss.
	stateResult_t combat_dodge_right( stateParms_t* parms );

	//! Executes a left dodge motion during combat for the monster boss vagary.
	stateResult_t combat_dodge_left( stateParms_t* parms );

	//! Executes a melee attack animation and state management for the monster boss vagary.
	stateResult_t combat_melee( stateParms_t* parms );

	//! Handles the combat range state logic for the monster boss vagary, managing throwing behavior and attack timing.
	stateResult_t combat_range( stateParms_t* parms );

private:
	float	  nextDodge;
	float	  nextAttack;
	float	  nextNoFOVAttack;
	idEntity* combat_node;
	idEntity* throwEntity;
	int		  num;
	int		  i;
	idVec3	  pos;
	float	  waitTime;
	float	  t;
	idVec3	  offset;
	idVec3	  vel;
	float	  start_offset;
};
