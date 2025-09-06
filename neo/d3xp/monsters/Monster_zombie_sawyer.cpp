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

#define SAWYER_SMOKE_PARTICLES	0
#define SAWYER_BLOOD_PARTICLES	1

CLASS_DECLARATION( iceMonsterZombie, iceMonsterZombieSawyer )
END_CLASS

/*
====================
iceMonsterZombieSawyer::AI_Begin
====================
*/
void iceMonsterZombieSawyer::AI_Begin()
{
	Event_SetSmokeVisibility( ALL_PARTICLES, 0 );
	Event_SetSmokeVisibility( SAWYER_SMOKE_PARTICLES, 1 ); // This was called as a seperate event before; possibly a optimization?
	SetState( "state_Begin" );
}

/*
=====================
iceMonsterZombieSawyer::state_Begin
=====================
*/
stateResult_t iceMonsterZombieSawyer::state_Begin( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 0 );

	Event_SetMoveType( MOVETYPE_ANIM );
	SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterZombieSawyer::state_Idle
=====================
*/
stateResult_t iceMonsterZombieSawyer::state_Idle( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		if( wait_for_enemy( parms ) == SRESULT_DONE )
		{
			parms->stage = 1;
		}

		return SRESULT_WAIT;
	}

	Event_SetState( "state_Combat" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterZombieSawyer::check_attacks
=====================
*/
int iceMonsterZombieSawyer::check_attacks()
{
	int attack_flags;

	attack_flags = 0;
	if( TestMelee() )
	{
		attack_flags |= ATTACK_MELEE;
	}

	return attack_flags;
}

/*
=====================
iceMonsterZombieSawyer::do_attack
=====================
*/
void iceMonsterZombieSawyer::do_attack( int attack_flags )
{
	if( attack_flags & ATTACK_MELEE )
	{
		AI_ATTACKING = true;
		SetState( "combat_melee" );
	}
}

/*
=====================
iceMonsterZombieSawyer::combat_melee
=====================
*/
stateResult_t iceMonsterZombieSawyer::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		next_hit_time = 0;
		smoke_frames = 0;

		Event_LookAtEnemy( 100 );
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_MeleeAttack", 8 );
		//SetWaitState("melee_attack");
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( InAnimState( ANIMCHANNEL_TORSO, "Torso_MeleeAttack" ) )
		{
			if( gameLocal.SysScriptTime() > next_hit_time )
			{
				next_hit_time = gameLocal.SysScriptTime() + 0.025;
				Event_SetSmokeVisibility( SAWYER_BLOOD_PARTICLES, 1 );
				smoke_frames = 4;
			}

			if( smoke_frames > 0 )
			{
				smoke_frames--;
				if( !smoke_frames )
				{
					Event_SetSmokeVisibility( SAWYER_BLOOD_PARTICLES, 0 );
				}
			}
		}
		else
		{
			parms->stage = 2;
		}
		return SRESULT_WAIT;
	}

	//waitAction("melee_attack");
	Event_SetSmokeVisibility( SAWYER_BLOOD_PARTICLES, 0 );
	Event_LookAtEnemy( 1 );
	//AI_ATTACKING = false;
	return SRESULT_DONE;
}