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
iceBot::state_Retreat
=====================
*/
stateResult_t iceBot::state_Retreat( stateParms_t* parms )
{
	bot_goal_t goal;
	idPlayer* entinfo;
	iceBot* owner;
	idVec3 target, dir;
	float attack_skill, range;

	// respawn if dead.
	if( BotIsDead( &bs ) )
	{
		stateThread.SetState( "state_Respawn" );
		return SRESULT_DONE_FRAME;
	}

	// if no enemy.
	if( bs.enemy < 0 )
	{
		stateThread.SetState( "state_SeekLTG" );
		return SRESULT_DONE_FRAME;
	}

	// Ensure the target is a player.
	entinfo = gameLocal.entities[bs.enemy]->Cast<idPlayer>();
	if( !entinfo )
	{
		stateThread.SetState( "state_SeekLTG" );
		return SRESULT_DONE_FRAME;
	}

	owner = gameLocal.entities[bs.entitynum]->Cast<iceBot>();

	// If our enemy is dead, search for another LTG.
	if( EntityIsDead( entinfo ) )
	{
		stateThread.SetState( "state_SeekLTG" );
		return SRESULT_DONE_FRAME;
	}

	//if there is another better enemy
	if( BotFindEnemy( &bs, bs.enemy ) )
	{
		common->DPrintf( "found new better enemy\n" );
	}

	//update the attack inventory values
	BotUpdateBattleInventory( &bs, bs.enemy );

	//if the bot doesn't want to retreat anymore... probably picked up some nice items
	if( BotWantsToChase( &bs ) )
	{
		//empty the goal stack, when chasing, only the enemy is the goal
		botGoalManager.BotEmptyGoalStack( bs.gs );

		//go chase the enemy
		//AIEnter_Battle_Chase(bs, "battle retreat: wants to chase");
		stateThread.SetState( "state_Chase" );
		return SRESULT_DONE_FRAME;
	}

	//update the last time the enemy was visible
	if( BotEntityVisible( bs.entitynum, bs.eye, bs.viewangles, 360, bs.enemy ) )
	{
		bs.enemyvisible_time = Bot_Time();
		target = entinfo->GetOrigin();
		bs.lastenemyorigin = target;
	}

	//if the enemy is NOT visible for 4 seconds
	if( bs.enemyvisible_time < Bot_Time() - 4 )
	{
		stateThread.SetState( "state_SeekLTG" );
		return SRESULT_DONE_FRAME;
	}
	//else if the enemy is NOT visible
	else if( bs.enemyvisible_time < Bot_Time() )
	{
		//if there is another enemy
		if( BotFindEnemy( &bs, -1 ) )
		{
			//AIEnter_Battle_Fight(bs, "battle retreat: another enemy");
			stateThread.SetState( "state_BattleFight" );
			return SRESULT_DONE_FRAME;
		}
	}

	//use holdable items
	BotBattleUseItems( &bs );

	//get the current long term goal while retreating
	if( !BotGetItemLongTermGoal( &bs, 0, &bs.currentGoal ) )
	{
		//AIEnter_Battle_SuicidalFight(bs, "battle retreat: no way out");
		stateThread.SetState( "state_BattleFight" );
		bs.flags |= BFL_FIGHTSUICIDAL;
		return SRESULT_DONE_FRAME;
	}

	//check for nearby goals periodicly
	if( bs.check_time < Bot_Time() )
	{
		bs.check_time = Bot_Time() + 1;
		range = 150;

		//
		if( BotNearbyGoal( &bs, 0, &goal, range ) )
		{
			//trap_BotResetLastAvoidReach(bs.ms);
			//time the bot gets to pick up the nearby goal item
			bs.nbg_time = Bot_Time() + range / 100 + 1;
			//AIEnter_Battle_NBG(bs, "battle retreat: nbg");
			stateThread.SetState( "state_BattleNBG" );
			return SRESULT_DONE_FRAME;
		}
	}

	MoveToCoverPoint();

	if( bot_skill.GetInteger() > 1 )
	{
		bs.firethrottlewait_time = 0;
	}

	BotChooseWeapon( &bs );

	BotAimAtEnemy( &bs );

	//attack the enemy if possible
	BotCheckAttack( &bs );

	return SRESULT_WAIT;
}