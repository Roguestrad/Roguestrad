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
iceBot::state_BattleNBG
=====================
*/
stateResult_t iceBot::state_BattleNBG( stateParms_t* parms )
{
	int areanum;
	bot_goal_t goal;
	//aas_entityinfo_t entinfo;
	idEntity* entinfo;
	//bot_moveresult_t moveresult;
	float attack_skill;
	idVec3 target, dir;

	//if (BotIsObserver(bs)) {
	//	AIEnter_Observer(bs, "battle nbg: observer");
	//	return qfalse;
	//}
	////if in the intermission
	//if (BotIntermission(bs)) {
	//	AIEnter_Intermission(bs, "battle nbg: intermission");
	//	return qfalse;
	//}

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

	//BotEntityInfo(bs.enemy, &entinfo);
	entinfo = gameLocal.entities[bs.enemy]->Cast<idPlayer>();
	if( entinfo->health <= 0 )
	{
		//AIEnter_Seek_NBG(bs, "battle nbg: enemy dead");
		stateThread.SetState( "state_SeekNBG" );
		return SRESULT_DONE_FRAME;
	}

	//bs.tfl = TFL_DEFAULT;
	//if (bot_grapple.integer) bs.tfl |= TFL_GRAPPLEHOOK;
	////if in lava or slime the bot should be able to get out
	//if (BotInLavaOrSlime(bs)) bs.tfl |= TFL_LAVA | TFL_SLIME;
	////
	//if (BotCanAndWantsToRocketJump(bs)) {
	//	bs.tfl |= TFL_ROCKETJUMP;
	//}
	////map specific code
	//BotMapScripts(bs);

	//update the last time the enemy was visible
	if( BotEntityVisible( bs.entitynum, bs.eye, bs.viewangles, 360, bs.enemy ) )
	{
		bs.enemyvisible_time = Bot_Time();
		//VectorCopy(entinfo->GetOrigin(), target);
		target = entinfo->GetOrigin();
		// if not a player enemy
		if( bs.enemy >= MAX_CLIENTS )
		{
#ifdef MISSIONPACK
			// if attacking an obelisk
			if( bs.enemy == redobelisk.entitynum ||
					bs.enemy == blueobelisk.entitynum )
			{
				target[2] += 16;
			}
#endif
		}
		//update the reachability area and origin if possible
		//areanum = BotPointAreaNum(target);
		//if (areanum && trap_AAS_AreaReachability(areanum)) {
		VectorCopy( target, bs.lastenemyorigin );
		//	bs.lastenemyareanum = areanum;
		//}
	}

	//if the bot has no goal or touches the current goal
	if( !botGoalManager.BotGetTopGoal( bs.gs, &goal ) )
	{
		bs.nbg_time = 0;
	}
	else if( BotReachedGoal( &bs, &goal ) )
	{
		bs.nbg_time = 0;
	}
	//
	if( bs.nbg_time < Bot_Time() )
	{
		//pop the current goal from the stack
		botGoalManager.BotPopGoal( bs.gs );
		//if the bot still has a goal
		if( botGoalManager.BotGetTopGoal( bs.gs, &goal ) )
		{
			//AIEnter_Battle_Retreat(bs, "battle nbg: time out");
			stateThread.SetState( "state_Retreat" );
		}
		else
		{
			//AIEnter_Battle_Fight(bs, "battle nbg: time out");
			stateThread.SetState( "state_BattleFight" );
		}

		return SRESULT_DONE_FRAME;
	}

	//move towards the goal
	BotMoveToGoal( &bs, &goal );

	//initialize the movement state
	//BotSetupForMovement(bs);
	////move towards the goal
	//trap_BotMoveToGoal(&moveresult, bs.ms, &goal, bs.tfl);
	////if the movement failed
	//if (moveresult.failure) {
	//	//reset the avoid reach, otherwise bot is stuck in current area
	//	trap_BotResetAvoidReach(bs.ms);
	//	//BotAI_Print(PRT_MESSAGE, "movement failure %d\n", moveresult.traveltype);
	//	bs.nbg_time = 0;
	//}
	////
	//BotAIBlocked(bs, &moveresult, qfalse);

	//update the attack inventory values
	BotUpdateBattleInventory( &bs, bs.enemy );

	//choose the best weapon to fight with
	BotChooseWeapon( &bs );

	//if the view is fixed for the movement
	//if (moveresult.flags & (MOVERESULT_MOVEMENTVIEW | MOVERESULT_SWIMVIEW)) {
	//	VectorCopy(moveresult.ideal_viewangles, bs.ideal_viewangles);
	//}
	//else if (!(moveresult.flags & MOVERESULT_MOVEMENTVIEWSET)
	//	&& !(bs.flags & BFL_IDEALVIEWSET)) {
	//	attack_skill = trap_Characteristic_BFloat(bs.character, CHARACTERISTIC_ATTACK_SKILL, 0, 1);
	//	//if the bot is skilled anough and the enemy is visible
	//	if (attack_skill > 0.3) {
	//		//&& BotEntityVisible(bs.entitynum, bs.eye, bs.viewangles, 360, bs.enemy)
	//		BotAimAtEnemy(bs);
	//	}
	//	else {
	//		if (trap_BotMovementViewTarget(bs.ms, &goal, bs.tfl, 300, target)) {
	//			VectorSubtract(target, bs.origin, dir);
	//			vectoangles(dir, bs.ideal_viewangles);
	//		}
	//		else {
	//			vectoangles(moveresult.movedir, bs.ideal_viewangles);
	//		}
	//		bs.ideal_viewangles[2] *= 0.5;
	//	}
	//}
	//if (attack_skill > 0.3) {
	//&& BotEntityVisible(bs.entitynum, bs.eye, bs.viewangles, 360, bs.enemy)
	BotAimAtEnemy( &bs );
	//}

	//if the weapon is used for the bot movement
	//if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs.weaponnum = moveresult.weapon;
	//attack the enemy if possible
	BotCheckAttack( &bs );
	return SRESULT_WAIT;
}