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
	\class iceMonsterZombieSawyer
	\brief A specialized zombie monster class implementing sawyer attack behavior.

	This class extends the base zombie monster functionality to implement a specific attack pattern known as the sawyer behavior. The implementation includes initialization of AI states, handling of
   idle and combat states, and execution of melee attacks with specific animation and effect handling. The class manages its own state transitions and attack execution based on combat conditions and
   available attack flags.

*/
class iceMonsterZombieSawyer : public iceMonsterZombie
{
	CLASS_PROTOTYPE( iceMonsterZombieSawyer );

public:
	//! Initializes the monster zombie sawyer AI state.
	virtual void AI_Begin() override;

	//! Returns attack flags indicating which melee attacks are available
	virtual int	 check_attacks() override;

	//! Performs an attack action with the specified attack flags.
	virtual void do_attack( int attack_flags ) override;

private:
	//! Initializes the zombie sawyer monster state by setting up idle animations and move type.
	stateResult_t state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster zombie sawyer, transitioning to combat when an enemy is detected.
	stateResult_t state_Idle( stateParms_t* parms );

	//! Performs a melee attack action for the monster zombie sawyer, handling animation states and hit effects.
	stateResult_t combat_melee( stateParms_t* parms );

	float		  next_hit_time;
	int			  smoke_frames;
};
