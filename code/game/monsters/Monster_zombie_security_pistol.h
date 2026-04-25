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

#pragma once

/*!
	\class iceMonsterZombieSecurityPistol
	\brief A specialized zombie monster class implementing security pistol AI behavior with ranged attacks and dodge maneuvers.

	This class represents a zombie monster variant that utilizes a security pistol weapon for ranged combat. It inherits from the basemonster zombie class and implements specific AI states for
   beginning, idling, attacking, and dodging. The monster supports both standing and crouched attack animations, with the ability to perform right and left dodge maneuvers during combat. The AI system
   is initialized through the Init and AI_Begin methods, which set up the monster's behavior and state management. Attack availability and execution are handled through the check_attacks and do_attack
   methods, allowing the monster to switch between different combat behaviors based on the environment and target conditions.

*/
class iceMonsterZombieSecurityPistol : public iceMonsterZombie
{
	CLASS_PROTOTYPE( iceMonsterZombieSecurityPistol );

public:
	//! Initializes the AI system for the monster zombie security pistol.
	virtual void Init() override;

	//! Initializes the ai state for the monster zombie security pistol.
	virtual void AI_Begin() override;

	//! Returns flags indicating which attacks are available for the zombie security pistol to perform
	virtual int	 check_attacks() override;

	//! Performs an attack action based on the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the zombie security pistol monster's state and sets up idle animations and movement type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the zombie security pistol monster, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Handles the standing attack state for the zombie security pistol monster
	stateResult_t stand_attack( stateParms_t* parms );

	//! Performs a crouched ranged attack animation with possible dodge interrupts.
	stateResult_t crouch_attack( stateParms_t* parms );

	//! Performs a right dodge action during combat for the zombie security pistol monster.
	stateResult_t combat_dodge_right( stateParms_t* parms );

	//! Handles the left dodge maneuver during combat for the monster zombie security pistol AI.
	stateResult_t combat_dodge_left( stateParms_t* parms );

private:
	idEntity*	  combat_node;

	idScriptBool  fire;
	idScriptBool  crouch_fire;
	idScriptBool  run_attack;
	idScriptFloat nextDodge;
	idScriptFloat nextAttack;
	idScriptFloat nextNoFOVAttack;

	// start out with a 50/50 chance of stand vs. crouch attacks.
	float		  zsecp_num_stand_attacks;
	float		  zsecp_num_crouch_attacks;

private:
	float attackTime;
};
