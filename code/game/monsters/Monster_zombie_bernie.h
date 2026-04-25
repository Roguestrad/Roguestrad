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
	\class iceMonsterZombieBernie
	\brief A specialized zombie monster implementation with ice-themed abilities and AI behavior.

	This class represents a zombie monster variant with ice-themed characteristics and specific AI behaviors. It inherits from iceMonsterZombie and implements custom states for initialization, idle
   behavior, and combat actions. The class manages the monster's animation states and combat responses through overridden methods. The AI system is initialized through the Init and AI_Begin methods,
   while combat functionality is handled via check_attacks and do_attack overrides. The state machine controls the monster's behavior flow from beginning states through idle periods to combat
   engagement.

*/
class iceMonsterZombieBernie : public iceMonsterZombie
{
	CLASS_PROTOTYPE( iceMonsterZombieBernie );

public:
	//! Initializes the AI system.
	virtual void Init() override;

	//! Initializes the AI behavior for the monster zombie bernie.
	virtual void AI_Begin() override;

	//! Returns attack flags indicating which attacks are available.
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the zombie bernie monster state by setting up idle animations and move type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster zombie bernie, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Handles the melee combat state for the zombie monster, managing the animation and attack sequence.
	stateResult_t combat_melee( stateParms_t* parms );
};
