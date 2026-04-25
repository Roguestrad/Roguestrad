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
	\class iceMonsterZombieMorgue
	\brief Represents anmonster zombie entity with specialized AI behavior for morgue environments.

	This class implements a specialized zombie monster variant designed for morgue-themed environments, extending base zombie functionality with unique AI states and combat behaviors. The entity
   manages its state transitions through defined states including initialization, idle behavior, and melee combat. The AI system is initialized through the Init method, and the entity begins its AI
   cycle with AI_Begin, which sets the initial state. State management includes handling idle conditions and transitioning to combat when enemies are detected. The class overrides standard attack
   checking and execution methods to provide specific melee combat behavior tailored for the morgue environment. The class uses a state machine approach with specific state handlers for different AI
   behaviors.

*/
class iceMonsterZombieMorgue : public iceMonsterZombie
{
	CLASS_PROTOTYPE( iceMonsterZombieMorgue );

public:
	//! Initializes the AI system
	virtual void Init() override;

	//! Sets the AI state to "state_Begin" for the monster zombie morgue.
	virtual void AI_Begin() override;

	//! Returns attack flags indicating whether melee attacks are available.
	virtual int	 check_attacks() override;

	//! Performs a melee attack action when the ATTACK_MELEE flag is set.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the zombie morgue monster state by setting up idle animations and move type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster zombie morgue, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Handles the melee combat state for the monster zombie morgue entity.
	stateResult_t combat_melee( stateParms_t* parms );
};
