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

CLASS_DECLARATION( iceMonsterZombie, iceMonsterZombieBernie )
END_CLASS

/*
=================
iceMonsterZombieBernie::Init
=================
*/
void iceMonsterZombieBernie::Init()
{

}

/*
=================
iceMonsterZombieBernie::AI_Begin
=================
*/
void iceMonsterZombieBernie::AI_Begin()
{
	Event_SetState( "state_Begin" );
}

/*
=====================
iceMonsterZombieBernie::state_Begin
=====================
*/
stateResult_t iceMonsterZombieBernie::state_Begin( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 8 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 8 );

	Event_SetMoveType( MOVETYPE_ANIM );
	Event_SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterZombieBernie::state_Idle
=====================
*/
stateResult_t iceMonsterZombieBernie::state_Idle( stateParms_t* parms )
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
monster_zombie::combat_melee
=====================
*/
stateResult_t iceMonsterZombieBernie::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_LookAtEnemy( 100 );
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_MeleeAttack", 8 );
		//SetWaitState("melee_attack");
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( AnimDone( ANIMCHANNEL_TORSO, 8 ) )
		{
			parms->stage = 2;
		}
		return SRESULT_WAIT;
	}

	//waitAction("melee_attack");
	Event_LookAtEnemy( 1 );
	AI_ATTACKING = false;
	return SRESULT_DONE;
}

/*
=====================
iceMonsterZombieBernie::check_attacks
=====================
*/
int iceMonsterZombieBernie::check_attacks()
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
iceMonsterZombieBernie::do_attack
=====================
*/
void iceMonsterZombieBernie::do_attack( int attack_flags )
{
	if( attack_flags & ATTACK_MELEE )
	{
		AI_ATTACKING = true;
		SetState( "combat_melee" );
	}
}