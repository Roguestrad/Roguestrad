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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop
#include "../Game_local.h"

/*
=====================
iceBot::state_SeekLTG
=====================
*/
stateResult_t iceBot::state_SeekLTG( stateParms_t* parms )
{
	if( BotIsDead( &bs ) )
	{
		stateThread.SetState( "state_Respawn" );
		return SRESULT_DONE_FRAME;
	}

	BotGetItemLongTermGoal( &bs, 0, &bs.currentGoal );

	// No Enemy.
	bs.enemy = -1;

	//if there is an enemy
	if( BotFindEnemy( &bs, -1 ) )
	{
		if( BotWantsToRetreat( &bs ) )
		{
			//keep the current long term goal and retreat
			//AIEnter_Battle_Retreat(bs, "seek ltg: found enemy");
			stateThread.SetState( "state_Retreat" );
			return SRESULT_DONE_FRAME;
		}
		else
		{
			//trap_BotResetLastAvoidReach(bs.ms);
			//empty the goal stack
			botGoalManager.BotEmptyGoalStack( bs.gs );

			//go fight
			stateThread.SetState( "state_BattleFight" );
			return SRESULT_DONE_FRAME;
		}
	}

	return SRESULT_WAIT;
}
