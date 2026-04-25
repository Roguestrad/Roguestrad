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
	\class iceMonsterFlyingCacodemon
	\brief A flying cacodemon AI class with ice-based abilities and state management.

	The iceMonsterFlyingCacodemon class implements a flying cacodemon AI that inherits from idAI and manages its behavior through a state machine. It handles initialization, idle states, combat
   actions including melee and ranged attacks, and attack flag management. The class uses a state machine approach to control the entity's actions, transitioning between different states based on game
   conditions and enemy detection. The AI is designed to fly and perform combat maneuvers, with specific handling for melee and ranged combat scenarios.

*/
class iceMonsterFlyingCacodemon : public idAI
{
	CLASS_PROTOTYPE( iceMonsterFlyingCacodemon );

public:
	//! Initializes the AI system for the monster flying cacodemon.
	virtual void Init() override;

	//! Initializes the monster flying cacodemon AI state to idle.
	virtual void AI_Begin() override;

	//! Returns an integer flag indicating which attacks are available for the monster flying cacodemon.
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the flying cacodemon state by setting movement type, animation state, and transitioning to idle state.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster flying cacodemon, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Handles the melee combat action for the monster flying cacodemon.
	stateResult_t combat_melee( stateParms_t* parms );

	//! Executes the combat range state for the monster flying cacodemon, handling animation and attack timing.
	stateResult_t combat_range( stateParms_t* parms );

private:
	float	  nextAttack;
	float	  nextNoFOVAttack;
	idEntity* combat_node;
};
