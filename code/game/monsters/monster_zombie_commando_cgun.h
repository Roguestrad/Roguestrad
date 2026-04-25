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
	\class iceMonsterZombieCommandoChaingun
	\brief Represents anmonster zombie commando chaingun AI entity with specific combat behaviors and state management.

	This class implements a specialized AI entity for anmonster zombie commando chaingun, inheriting from idAI. It manages the entity's behavior through various combat and idle states, including
   dodging maneuvers, crouched and standing attacks, and enemy detection. The class handles initialization, state transitions, and attack execution. The AI system is designed to respond to enemy
   presence with appropriate combat actions, utilizing dodge mechanics to avoid incoming attacks while maintaining offensive capabilities. The implementation includes state management for both idle
   and combat scenarios, with specific behaviors for different attack types and movement patterns.

*/
class iceMonsterZombieCommandoChaingun : public idAI
{
	CLASS_PROTOTYPE( iceMonsterZombieCommandoChaingun );

public:
	//! Initializes the AI system for the monster zombie commando chaingun.
	virtual void Init() override;

	//! Initializes the monster zombie commando chaingun AI state.
	virtual void AI_Begin() override;

	//! Checks and returns available attack flags for the monster zombie commando chaingun
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the beginning state for the monster zombie commando chaingun.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster zombie commando chaingun, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Executes a right dodge maneuver during combat for the monster zombie commando chaingun.
	stateResult_t combat_dodge_right( stateParms_t* parms );

	//! Executes a left dodge maneuver during combat for the monster zombie commando chaingun entity.
	stateResult_t combat_dodge_left( stateParms_t* parms );

	//! Handles the crouched attack state for the monster zombie commando chaingun enemy.
	stateResult_t crouch_attack( stateParms_t* parms );

	//! Handles the standing attack state for the ice zombie commando chaingun monster.
	stateResult_t stand_attack( stateParms_t* parms );

private:
	idScriptBool  fire;
	idScriptBool  crouch_fire;
	idScriptBool  step_left;
	idScriptBool  step_right;
	idScriptFloat nextDodge;
	idScriptFloat nextAttack;
	idScriptFloat nextNoFOVAttack;

private:
	idEntity* combat_node;
	float	  attackTime;
};
