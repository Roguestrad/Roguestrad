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

CLASS_DECLARATION( idAI, iceAI_Follower )
END_CLASS

#define FOLLOW_MAXDIST	120
#define FOLLOW_MINDIST	80
#define FOLLOW_RUNDIST	180

/*
================
iceAI_Follower::Init
================
*/
void iceAI_Follower::Init()
{
	inCustomAnim = false;
	AI_RUN = false;

	int mod;
	spawnArgs.GetInt( "head_look", 0, mod );
	Event_SetJointMod( mod );

	leader = NULL;

	Event_SetTalkState( TALK_OK );

	Event_SetKey( "conversationNext", "" );

	Event_SetKey( "wander", "1" );

	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 0 );
	stateThread.SetState( "state_idle" );
}

/*
================
iceAI_Follower::state_idle
================
*/
stateResult_t iceAI_Follower::state_idle( stateParms_t* parms )
{
	Event_StopMove();
	Event_SetTalkTarget( NULL );
	stateThread.SetState( "state_idle_frame" );
	return SRESULT_DONE;
}

/*
================
iceAI_Follower::state_idle_frame
================
*/
stateResult_t iceAI_Follower::state_idle_frame( stateParms_t* parms )
{
	if( AI_TALK )
	{
		PlayCustomAnim( "melee_attack1", 0, 0 );
		stateThread.SetState( "state_talk_anim" );
		return SRESULT_DONE;
	}

	return SRESULT_WAIT;
}


/*
================
iceAI_Follower::state_idle_frame
================
*/
stateResult_t iceAI_Follower::state_talk_anim( stateParms_t* parms )
{
	if( AnimDone( ANIMCHANNEL_TORSO, 0 ) )
	{
		stateThread.SetState( "state_follow" );
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
		Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 0 );
		return SRESULT_DONE;
	}
	return SRESULT_WAIT;
}

/*
================
iceAI_Follower::state_follow
================
*/
stateResult_t iceAI_Follower::state_follow( stateParms_t* parms )
{
	leader = talkTarget.GetEntity();
	if( !leader )
	{
		leader = gameLocal.GetLocalPlayer();
	}

	Event_SetTalkTarget( NULL );

	stateThread.SetState( "state_follow_frame" );
	return SRESULT_DONE;
}

/*
================
iceAI_Follower::state_follow_frame
================
*/
stateResult_t iceAI_Follower::state_follow_frame( stateParms_t* parms )
{
	Event_LookAtEntity( leader, 0.1f );

	if( AI_TALK )
	{
		stateThread.SetState( "state_idle" );
		return SRESULT_DONE;
	}

	if( DistanceTo( leader ) > FOLLOW_MAXDIST )
	{
		leader = talkTarget.GetEntity();
		if( !leader )
		{
			leader = gameLocal.GetLocalPlayer();
		}
		Event_SetTalkTarget( NULL );
		AI_RUN = false;
		Event_MoveToEntity( leader );
		stateThread.SetState( "state_get_closer" );
	}
	else
	{
		Event_FaceEntity( leader );
	}
	return SRESULT_WAIT;
}

/*
================
iceAI_Follower::state_get_closer
================
*/
stateResult_t iceAI_Follower::state_get_closer( stateParms_t* parms )
{
	bool switchState = !( !AI_DEST_UNREACHABLE && !AI_MOVE_DONE && ( DistanceTo( leader ) > FOLLOW_MINDIST ) );
	if( switchState )
	{
		stateThread.SetState( "state_follow_frame" );
		Event_StopMove();
		return SRESULT_DONE;
	}

	Event_LookAtEntity( leader, 0.1f );
	if( DistanceTo( leader ) > FOLLOW_RUNDIST )
	{
		AI_RUN = true;
	}
	if( DistanceTo( leader ) < FOLLOW_MAXDIST )
	{
		AI_RUN = false;
	}
	if( AI_TALK )
	{
		stateThread.SetState( "state_Idle" );
	}
	return SRESULT_WAIT;
}

/*
================
iceAI_Follower::state_killed
================
*/
stateResult_t iceAI_Follower::state_killed( stateParms_t* parms )
{
	Event_StopMove();
	return SRESULT_DONE;
}

