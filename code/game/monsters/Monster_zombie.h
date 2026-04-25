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
	\class iceMonsterZombie
	\brief A specialized AI monster class representing anmonster zombie with melee combat capabilities.

	This class implements the behavior and state management for anmonster zombie entity. It extends the base idAI class to provide specific AI functionality including state transitions, combat
   behaviors, and animation control. The implementation handles initialization, idle behavior, and melee combat states while integrating with the engine's AI system. The class manages state
   transitions between beginning, idle, and combat states, with specific handling for enemy detection and melee attacks. The AI system initialization and beginning functions provide the framework for
   the monster's behavior, while attack-related methods define how the monster engages in combat. The class is designed to work within a larger game engine framework that manages entity states and AI
   behaviors.

*/
class iceMonsterZombie : public idAI
{
	CLASS_PROTOTYPE( iceMonsterZombie );

public:
	//! Initializes the AI system.
	virtual void Init() override;

	//! Initializes the zombie's AI state and parameters.
	virtual void AI_Begin() override;

	//! Returns attack flags indicating which melee attacks are available.
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the zombie monster's state by setting up animations and movement type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for anmonster zombie, transitioning to combat state when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Handles the melee combat state for anmonster zombie, including looking at the enemy, facing the enemy, and performing a melee attack animation.
	stateResult_t combat_melee( stateParms_t* parms );

private:
	idScriptBool can_run;
};
