
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

#define HELLKNIGHT_ATTACK_RATE			3
#define	HELLKNIGHT_PAIN_DELAY			0.25
#define HELLKNIGHT_NOFOVTIME			6

#define HELLKNIGHT_IDLE_TO_RANGEATTACK	4
#define HELLKNIGHT_WALK_TO_MELEE		4
#define HELLKNIGHT_WALK_TO_RANGEATTACK	4

CLASS_DECLARATION( idAI, iceMonsterDemonHellknight )
END_CLASS

/*
=================
iceMonsterDemonHellknight::Init
=================
*/
void iceMonsterDemonHellknight::Init()
{
	range_attack_anim.LinkTo( scriptObject, "range_attack_anim" );
}

/*
=================
iceMonsterDemonHellknight::AI_Begin
=================
*/
void iceMonsterDemonHellknight::AI_Begin()
{
	Event_SetState( "state_Begin" );
}

/*
=====================
iceMonsterDemonHellknight::state_Begin
=====================
*/
stateResult_t iceMonsterDemonHellknight::state_Begin( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 0 );

	Event_SetMoveType( MOVETYPE_ANIM );
	Event_SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterDemonHellknight::state_Idle
=====================
*/
stateResult_t iceMonsterDemonHellknight::state_Idle( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		if( wait_for_enemy( parms ) == SRESULT_DONE )
		{
			parms->stage = 1;
		}

		return SRESULT_WAIT;
	}

	nextAttack = 0;
	nextNoFOVAttack = 0;

	Event_SetState( "state_Combat" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterDemonHellknight::do_attack
=====================
*/
void iceMonsterDemonHellknight::do_attack( int attack_flags )
{
	nextNoFOVAttack = gameLocal.SysScriptTime() + HELLKNIGHT_NOFOVTIME;
	if( attack_flags & ATTACK_COMBAT_NODE )
	{
		//combat_ainode(combat_node);
		gameLocal.Error( "Combat_Node fix me\n" );
	}
	else if( attack_flags & ATTACK_MELEE )
	{
		SetState( "combat_melee" );
	}
	else if( attack_flags & ATTACK_MISSILE )
	{
		SetState( "combat_range" );
	}
}

/*
=====================
iceMonsterDemonHellknight::check_attacks
=====================
*/
int iceMonsterDemonHellknight::check_attacks()
{
	float	currentTime;
	float	canMelee;
	int	attack_flags;
	idVec3	vel;
	float	t;
	idVec3	jumpTarget;
	idStr	anim;

	attack_flags = 0;

	canMelee = TestMelee();
	currentTime = gameLocal.SysScriptTime();
	if( !canMelee )
	{
		combat_node = GetCombatNode();
		if( combat_node )
		{
			attack_flags |= ATTACK_COMBAT_NODE;
		}
	}

	if( canMelee )
	{
		attack_flags |= ATTACK_MELEE;
	}

	if( ( ( gameLocal.SysScriptTime() > nextNoFOVAttack ) && AI_ENEMY_VISIBLE ) || AI_ENEMY_IN_FOV )
	{
		if( !CanReachEnemy() || ( currentTime >= nextAttack ) )
		{
// jmarshall: I've temp disabled range_attack_anim. I can't seem to pass in idStr to doomscript?
			range_attack_anim = ChooseAnim( ANIMCHANNEL_LEGS, "turret_attack" );
			if( CanHitEnemyFromAnim( range_attack_anim ) )
			{
				attack_flags |= ATTACK_MISSILE;
			}

			anim = ChooseAnim( ANIMCHANNEL_LEGS, "range_attack" );
			if( TestAnimMoveTowardEnemy( anim ) )
			{
				if( CanHitEnemyFromAnim( anim ) )
				{
					range_attack_anim = anim;
					attack_flags |= ATTACK_MISSILE;
				}
			}
		}
	}

	return attack_flags;
}


/*
=====================
monster_zombie::combat_range
=====================
*/
stateResult_t iceMonsterDemonHellknight::combat_range( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_FaceEnemy();
		if( !AI_ENEMY_IN_FOV )
		{
			parms->Wait( 0.4 );
		}

		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_RangeAttack", HELLKNIGHT_WALK_TO_RANGEATTACK );
		SetWaitState( "range_attack" );
		parms->stage = 2;
		return SRESULT_WAIT;
	}

	if( parms->stage == 2 )
	{
		if( AnimDone( ANIMCHANNEL_TORSO, HELLKNIGHT_WALK_TO_RANGEATTACK ) )
		{
			parms->stage = 3;
		}
		return SRESULT_WAIT;
	}

	// don't attack for a bit
	nextAttack = gameLocal.DelayTime( HELLKNIGHT_ATTACK_RATE );
	nextNoFOVAttack = gameLocal.SysScriptTime() + HELLKNIGHT_NOFOVTIME;

	return SRESULT_DONE;
}

/*
=====================
monster_zombie::combat_melee
=====================
*/
stateResult_t iceMonsterDemonHellknight::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_LookAtEnemy( 100 );
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_MeleeAttack", HELLKNIGHT_WALK_TO_MELEE );
		SetWaitState( "melee_attack" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( waitState == "" )
		{
			parms->stage = 2;
		}
		return SRESULT_WAIT;
	}

	Event_LookAtEnemy( 1 );

	return SRESULT_DONE;
}

