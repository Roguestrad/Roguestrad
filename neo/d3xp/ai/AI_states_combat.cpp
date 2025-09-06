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
idAI::state_Combat

This is the main state for all enemies.
=====================
*/
stateResult_t idAI::state_Combat( stateParms_t* parms )
{
	if( parms->stage == 1 || parms->stage == 2 )
	{
		bool result;
		if( combat_chase( parms, result ) == SRESULT_DONE )
		{
			parms->substage = 0;
			if( result )
			{
				parms->stage = 0;
				return SRESULT_WAIT;
			}

			if( parms->stage == 1 )
			{
				Event_LocateEnemy();
				parms->stage = 2;
				return SRESULT_WAIT;
			}

			if( parms->stage == 2 )
			{
				combat_lost();
				parms->stage = 0;
				return SRESULT_WAIT;
			}
		}

		return SRESULT_WAIT;
	}

	FaceEnemy();
	if( AI_ENEMY_IN_FOV )
	{
		Event_LookAtEnemy( 1 );
	}

	if( gameLocal.InfluenceActive() )
	{
		return SRESULT_WAIT;
	}

	if( AI_ENEMY_DEAD )
	{
		enemy_dead();
	}

	int attack_flags = check_attacks();
	if( attack_flags )
	{
		stateThread.Clear();
		do_attack( attack_flags );
		stateThread.PostState( "state_Combat" );
		return SRESULT_DONE_FRAME;
	}

	parms->stage = 1;

	return SRESULT_WAIT;
}


/*
=====================
idAI::state_LostCombat
=====================
*/
stateResult_t idAI::state_LostCombat( stateParms_t* parms )
{
	idVec3 ang;
	float dist;
	float yaw;
	float attack_flags;
	idEntity* possibleEnemy;

	lost_time = gameLocal.SysScriptTime() + gameLocal.SysScriptFrameTime();
	allow_attack = gameLocal.SysScriptTime() + 4;

	lost_combat_node = GetClosestHiddenTarget( "ai_lostcombat" );
	if( lost_combat_node )
	{
		stateThread.SetState( "state_LostCombat_Node" );
		return SRESULT_DONE;
	}
	else
	{
		stateThread.SetState( "state_LostCombat_No_Node" );
		return SRESULT_DONE;
	}

	return SRESULT_ERROR;
}

/*
=====================
idAI::state_LostCombat_Node
=====================
*/
stateResult_t idAI::state_LostCombat_Node( stateParms_t* parms )
{
	idVec3 ang;
	float dist;
	float yaw;
	float attack_flags;
	idEntity* possibleEnemy;
	idEntity* node = lost_combat_node;

	if( parms->stage == 0 )
	{
		dist = DistanceTo( node );
		if( dist < 40 )
		{
			// fixes infinite loops when close to lost combat node
			return SRESULT_DONE;
		}

		parms->stage = 1;

		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		AI_RUN = true;
		Event_MoveToEntity( node );

		parms->stage = 2;

		return SRESULT_WAIT;
	}

	if( parms->stage == 2 )
	{
		if( !AI_MOVE_DONE )
		{
			if( gameLocal.InfluenceActive() )
			{
				parms->stage = 3;
				return SRESULT_WAIT;
			}

			possibleEnemy = HeardSound( true );
			if( possibleEnemy )
			{
				if( CanReachEntity( possibleEnemy ) )
				{
					Event_SetEnemy( possibleEnemy );
					parms->stage = 3;
					return SRESULT_WAIT;
				}
			}

			parms->stage = 4;

			return SRESULT_WAIT;
		}

		parms->stage = 3;
		return SRESULT_WAIT;
	}

	if( parms->stage == 4 )
	{
		bool result;
		if( check_blocked( parms, result ) == SRESULT_DONE )
		{
			parms->substage = 0;
			if( result )
			{
				parms->stage = 3;
				return SRESULT_WAIT;
			}

			// allow attacks when enemy is outside of fov
			if( gameLocal.SysScriptTime() > allow_attack )
			{
				AI_ENEMY_IN_FOV = AI_ENEMY_VISIBLE;
			}

			attack_flags = check_attacks();
			if( attack_flags )
			{
				do_attack( attack_flags );
				stateThread.PostState( "state_Combat" );
				return SRESULT_DONE;
			}

			parms->stage = 2; // Go back and do this over
		}

		return SRESULT_WAIT;
	}

	if( parms->stage == 3 )
	{
		ang = idVec3( 0.0f, current_yaw, 0.0f );
		Event_TurnTo( ang.y );

		parms->stage = 5;
		return SRESULT_WAIT;
	}

	if( parms->stage == 5 )
	{
		if( !AI_MOVE_DONE )
		{
			if( CanReachEnemy() )
			{
				if( HeardSound( true ) )
				{
					parms->stage = 6;
					return SRESULT_WAIT;
				}

				if( AI_ENEMY_IN_FOV || AI_PAIN )
				{
					parms->stage = 6;
					return SRESULT_WAIT;
				}
			}
		}

		return SRESULT_WAIT;
	}

	stateThread.SetState( "state_LostCombat_Finish" );
	return SRESULT_DONE;
}

/*
=====================
idAI::state_LostCombat_No_Node
=====================
*/
stateResult_t idAI::state_LostCombat_No_Node( stateParms_t* parms )
{
	if( parms->stage == 7 )
	{
		bool result;
		if( check_blocked( parms, result ) == SRESULT_DONE )
		{
			parms->substage = 0;
			if( result )
			{
				parms->stage = 5;
			}
			else
			{
				parms->stage = 4;
			}
		}

		return SRESULT_WAIT;
	}

	if( parms->stage == 0 )
	{
		AI_RUN = true;
		Event_MoveToCover();

		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( AI_DEST_UNREACHABLE )
		{
			parms->stage = 2;
		}

		parms->stage = 3;

		return SRESULT_WAIT;
	}

	if( parms->stage == 2 )
	{
		if( combat_wander( parms ) != SRESULT_DONE )
		{
			return SRESULT_WAIT;
		}

		parms->stage = 3;
		return SRESULT_WAIT;
	}

	// if we're not already in cover
	if( parms->stage == 3 )
	{
		if( !AI_MOVE_DONE )
		{
			parms->stage = 4;
		}
		else
		{
			goto done;
		}

		return SRESULT_DONE;
	}

	if( parms->stage == 4 )
	{
		if( !AI_MOVE_DONE )
		{
			if( gameLocal.InfluenceActive() )
			{
				parms->stage = 5;
				return SRESULT_WAIT;
			}
			// allow attacks when enemy is outside of fov
			if( gameLocal.SysScriptTime() > allow_attack )
			{
				AI_ENEMY_IN_FOV = AI_ENEMY_VISIBLE;
			}
			attack_flags = check_attacks();
			if( attack_flags )
			{
				do_attack( attack_flags );
				stateThread.PostState( "state_Combat" );
				return SRESULT_DONE;
			}

			idEntity* possibleEnemy = HeardSound( true );
			if( possibleEnemy )
			{
				if( CanReachEntity( possibleEnemy ) )
				{
					Event_SetEnemy( possibleEnemy );
					parms->stage = 5;
					return SRESULT_WAIT;
				}
			}
			if( CanReachEnemy() )
			{
				if( AI_ENEMY_IN_FOV || AI_PAIN )
				{
					parms->stage = 5;
					return SRESULT_WAIT;
				}
			}

			parms->stage = 7; // check_blocked
		}
		else
		{
			parms->stage = 6;
			return SRESULT_WAIT;
		}

		parms->stage = 5;
		return SRESULT_WAIT;
	}

	if( parms->stage == 5 )
	{
		if( !gameLocal.InfluenceActive() )
		{
			if( AI_ENEMY_VISIBLE )
			{
				Event_FaceEnemy();
			}
			else if( AI_MOVE_DONE )
			{
				// turn around to face the way we came
				float yaw = current_yaw;
				Event_TurnTo( yaw + 180 );
			}
		}

		parms->stage = 6;
		return SRESULT_WAIT;
	}

done:
	stateThread.SetState( "state_LostCombat_Finish" );
	return SRESULT_DONE;
}

/*
=====================
idAI::state_LostCombat_Finish
=====================
*/
stateResult_t idAI::state_LostCombat_Finish( stateParms_t* parms )
{
	if( lost_time >= gameLocal.SysScriptTime() )
	{
		if( combat_wander( parms ) != SRESULT_DONE )
		{
			return SRESULT_WAIT;
		}
	}

	if( AI_ENEMY_VISIBLE || gameLocal.InfluenceActive() )
	{
		stateThread.SetState( "state_Combat" );
	}
	else
	{
		Event_ClearEnemy();
		stateThread.SetState( "state_Idle" );
	}

	return SRESULT_DONE;
}

/*
=====================
idAI::combat_wander
=====================
*/
stateResult_t idAI::combat_wander( stateParms_t* parms )
{
	float mintime;
	float endtime;

	if( parms->stage == 0 )
	{
		parms->param1 = gameLocal.SysScriptTime() + 0.2;
		parms->param2 = gameLocal.SysScriptTime() + 3;

		Event_Wander();
		parms->stage = 1;

		return SRESULT_WAIT;
	}

	mintime = parms->param1;
	endtime = parms->param2;

	if( parms->stage == 1 )
	{
		if( gameLocal.SysScriptTime() < endtime )
		{
			if( gameLocal.InfluenceActive() )
			{
				parms->stage = 2;
				return SRESULT_WAIT;
			}
			// jmarshall - todo
			//if (check_attacks()) {
			//	break;
			//}
			// jmarshall end

			if( gameLocal.SysScriptTime() > mintime )
			{
				if( HeardSound( true ) )
				{
					parms->stage = 2;
					return SRESULT_WAIT;
				}

				if( AI_ENEMY_IN_FOV || AI_PAIN )
				{
					parms->stage = 2;
					return SRESULT_WAIT;
				}
			}
		}
		else
		{
			parms->stage = 2;
		}

		return SRESULT_WAIT;
	}

	SetState( "state_Combat" );
	return SRESULT_DONE;
}