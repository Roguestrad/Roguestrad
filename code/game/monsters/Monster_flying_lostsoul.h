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
	\class iceMonsterLostSoul
	\brief the monster lost soul AI class implements the behavior and state management for an ice-themed enemy creature.

	This class extends the base AI functionality to define the specific behaviors of anmonster lost soul enemy. It manages the creature's state machine including initialization, idle behavior,
   combat states such as charging, melee attacks, and retreating. The class handles AI initialization, begins the AI behavior, checks for applicable attacks based on positioning and timing, and
   executes the corresponding attack actions. The implementation supports fly movement and idle animation during the begin state, and transitions between idle and combat states based on enemy
   detection. The combat behavior includes charging, melee attacks, and retreat mechanics to provide varied and challenging enemy interactions.

*/
class iceMonsterLostSoul : public idAI
{
	CLASS_PROTOTYPE( iceMonsterLostSoul );

public:
	//! Initializes the AI system
	virtual void Init() override;

	//! Initializes the monster lost soul AI behavior.
	virtual void AI_Begin() override;

	//! Checks for and returns applicable attack flags for the monster lost soul based on enemy position and timing.
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the monster lost soul state with fly movement and idle animation.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster lost soul, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Executes a charging attack move for the monster lost soul.
	stateResult_t combat_charge( stateParms_t* parms );

	//! Executes a melee attack animation and waits for it to complete.
	stateResult_t combat_melee( stateParms_t* parms );

	//! Handles the retreat behavior of the monster lost soul during combat.
	stateResult_t combat_retreat( stateParms_t* parms );

private:
	float nextAttack;
	float nextNoFOVAttack;
	float noMeleeTime;
	float fly_offset;

private:
	idVec3 vel;
	idVec3 pos;
	float  endtime;
};
