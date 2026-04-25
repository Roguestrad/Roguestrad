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
	\class iceMonsterZombieCommandoTentacle
	\brief Represents anmonster zombie commando tentacle AI entity with specialized attack behaviors.

	This class implements a specialized AI entity that extends the base zombie AI functionality to include tentacle-specific attack patterns. The entity manages multiple states including idle, combat,
   and attack execution, with distinct behaviors for tentacle attacks and melee combat. It handles initialization of the AI system, state management, and attack flag checking to determine available
   actions. The tentacle attack functionality includes start and end methods that control damage infliction during combat sequences.

*/
class iceMonsterZombieCommandoTentacle : public iceMonsterZombie
{
	CLASS_PROTOTYPE( iceMonsterZombieCommandoTentacle );

public:
	//! Initializes the AI system
	virtual void Init() override;

	//! Initializes the monster zombie commando tentacle AI state.
	virtual void AI_Begin() override;

	//! Returns attack flags indicating which attacks are available for the tentacle.
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the zombie commando tentacle state by setting up idle animations and move type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster zombie commando tentacle, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Executes a tentacle attack state for the monster zombie commando.
	stateResult_t combat_tentacle( stateParms_t* parms );

	//! Executes a melee attack animation and handles damage for the zombie commander tentacle monster.
	stateResult_t combat_melee( stateParms_t* parms );

	//! Ends the tentacle attack by disabling tentacle damage.
	void		  tentacle_attack_end();

	//! Starts the tentacle attack by enabling damage infliction.
	void		  tentacle_attack_start();

private:
	float nextAttack;
	float nextNoFOVAttack;
	bool  tentacleDamage;
};
