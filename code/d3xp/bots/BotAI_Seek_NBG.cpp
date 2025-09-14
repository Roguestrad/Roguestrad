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
iceBot::state_SeekNBG
=====================
*/
stateResult_t iceBot::state_SeekNBG( stateParms_t* parms )
{
	bot_goal_t goal;
	idVec3 target, dir;
	//bot_moveresult_t moveresult;

	//if (BotIsObserver(bs)) {
	//	AIEnter_Observer(bs, "seek nbg: observer");
	//	return qfalse;
	//}
	////if in the intermission
	//if (BotIntermission(bs)) {
	//	AIEnter_Intermission(bs, "seek nbg: intermision");
	//	return qfalse;
	//}

	// respawn if dead.
	if( BotIsDead( &bs ) )
	{
		stateThread.SetState( "state_Respawn" );
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
	//no enemy
	bs.enemy = -1;
	//if the bot has no goal
	if( !botGoalManager.BotGetTopGoal( bs.gs, &goal ) )
	{
		bs.nbg_time = 0;
	}
	//if the bot touches the current goal
	else if( BotReachedGoal( &bs, &goal ) )
	{
		BotChooseWeapon( &bs );
		bs.nbg_time = 0;
	}

	if( bs.nbg_time < Bot_Time() )
	{
		//pop the current goal from the stack
		botGoalManager.BotPopGoal( bs.gs );
		//check for new nearby items right away
		//NOTE: we canNOT reset the check_time to zero because it would create an endless loop of node switches
		bs.check_time = Bot_Time() + 0.05;
		//go back to seek ltg
//		AIEnter_Seek_LTG(bs, "seek nbg: time out");
		stateThread.SetState( "state_SeekLTG" );
		return SRESULT_DONE_FRAME;
	}

	//predict obstacles
	//if (BotAIPredictObstacles(bs, &goal))
	//	return qfalse;
	////initialize the movement state
	//BotSetupForMovement(bs);
	////move towards the goal
	//trap_BotMoveToGoal(&moveresult, bs.ms, &goal, bs.tfl);
	////if the movement failed
	//if (moveresult.failure) {
	//	//reset the avoid reach, otherwise bot is stuck in current area
	//	trap_BotResetAvoidReach(bs.ms);
	//	bs.nbg_time = 0;
	//}
	BotMoveToGoal( &bs, &goal );

	//check if the bot is blocked
	//BotAIBlocked(bs, &moveresult, qtrue);
	////
	//BotClearPath(bs, &moveresult);

// jmarshall - fix look at code.
	//if the viewangles are used for the movement
	//if (moveresult.flags & (MOVERESULT_MOVEMENTVIEWSET | MOVERESULT_MOVEMENTVIEW | MOVERESULT_SWIMVIEW)) {
	//	VectorCopy(moveresult.ideal_viewangles, bs.ideal_viewangles);
	//}
	////if waiting for something
	//else if (moveresult.flags & MOVERESULT_WAITING) {
	//	if (random() < bs.thinktime * 0.8) {
	//		BotRoamGoal(bs, target);
	//		VectorSubtract(target, bs.origin, dir);
	//		vectoangles(dir, bs.ideal_viewangles);
	//		bs.ideal_viewangles[2] *= 0.5;
	//	}
	//}
	//else if (!(bs.flags & BFL_IDEALVIEWSET)) {
	//	if (!trap_BotGetSecondGoal(bs.gs, &goal)) trap_BotGetTopGoal(bs.gs, &goal);
	//	if (trap_BotMovementViewTarget(bs.ms, &goal, bs.tfl, 300, target)) {
	//		VectorSubtract(target, bs.origin, dir);
	//		vectoangles(dir, bs.ideal_viewangles);
	//	}
	//	//FIXME: look at cluster portals?
	//	else vectoangles(moveresult.movedir, bs.ideal_viewangles);
	//	bs.ideal_viewangles[2] *= 0.5;
	//}
	////if the weapon is used for the bot movement
	//if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs.weaponnum = moveresult.weapon;
// jmarshall end
	//if there is an enemy
	if( BotFindEnemy( &bs, -1 ) )
	{
		if( BotWantsToRetreat( &bs ) )
		{
			//keep the current long term goal and retreat
			//AIEnter_Battle_NBG(bs, "seek nbg: found enemy");
			stateThread.SetState( "state_BattleNBG" );
			return SRESULT_DONE_FRAME;
		}
		else
		{
			//trap_BotResetLastAvoidReach(bs.ms);
			//empty the goal stack
			botGoalManager.BotEmptyGoalStack( bs.gs );
			//go fight
			//AIEnter_Battle_Fight(bs, "seek nbg: found enemy");
			stateThread.SetState( "state_BattleFight" );
			return SRESULT_DONE_FRAME;
		}
	}
	return SRESULT_WAIT;
}