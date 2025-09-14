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

#define ZCT_RUNDISTANCE			128
#define ZCT_WALKTURN			65
#define ZCT_ATTACK_RATE			5
#define ZCT_TENTACLE_RANGE_MAX	200
#define ZCT_NOFOVTIME			4

#define ATTACK_TENTACLE			ATTACK_SPECIAL1

CLASS_DECLARATION( iceMonsterZombie, iceMonsterZombieCommandoTentacle )
END_CLASS

/*
=================
iceMonsterZombieCommandoTentacle::Init
=================
*/
void iceMonsterZombieCommandoTentacle::Init()
{

}

/*
=================
iceMonsterZombieCommandoTentacle::AI_Begin
=================
*/
void iceMonsterZombieCommandoTentacle::AI_Begin()
{
	run_distance = ZCT_RUNDISTANCE;
	walk_turn = ZCT_WALKTURN;

	Event_SetState( "state_Begin" );
}

/*
=====================
iceMonsterZombieCommandoTentacle::state_Begin
=====================
*/
stateResult_t iceMonsterZombieCommandoTentacle::state_Begin( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 8 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 8 );

	Event_SetMoveType( MOVETYPE_ANIM );
	Event_SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterZombieCommandoTentacle::state_Idle
=====================
*/
stateResult_t iceMonsterZombieCommandoTentacle::state_Idle( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		if( wait_for_enemy( parms ) == SRESULT_DONE )
		{
			parms->stage = 1;
		}

		return SRESULT_WAIT;
	}

	tentacleDamage = false;
	nextAttack = 0;
	nextNoFOVAttack = 0;

	Event_SetState( "state_Combat" );
	return SRESULT_DONE;
}

/*
=====================
monster_zombie::combat_melee
=====================
*/
stateResult_t iceMonsterZombieCommandoTentacle::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		tentacleDamage = false;
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
			Event_LookAtEnemy( 1 );
			if( tentacleDamage )
			{
				if( MeleeAttackToJoint( "joint13", "melee_commandoTentacle" ) )
				{
					tentacleDamage = false;
				}
			}
		}
		else
		{
			parms->stage = 2;
		}
		return SRESULT_WAIT;
	}

	tentacleDamage = false;

	AI_ATTACKING = false;
	return SRESULT_DONE;
}


/*
=====================
monster_zombie::combat_tentacle
=====================
*/
stateResult_t iceMonsterZombieCommandoTentacle::combat_tentacle( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		tentacleDamage = false;
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_TentacleAttack", 4 );
		//SetWaitState("melee_attack");
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( InAnimState( ANIMCHANNEL_TORSO, "Torso_TentacleAttack" ) )
		{
			if( tentacleDamage )
			{
				if( MeleeAttackToJoint( "joint13", "melee_commandoTentacle" ) )
				{
					tentacleDamage = false;
				}
			}
		}
		else
		{
			parms->stage = 2;
		}
		return SRESULT_WAIT;
	}

	tentacleDamage = false;

	AI_ATTACKING = false;
	return SRESULT_DONE;
}

/*
=====================
iceMonsterZombieCommandoTentacle::check_attacks
=====================
*/
int iceMonsterZombieCommandoTentacle::check_attacks()
{
	int attack_flags;
	float range;
	float currentTime;

	attack_flags = 0;
	if( TestMelee() )
	{
		attack_flags |= ATTACK_MELEE;
	}

	if( ( ( gameLocal.SysScriptTime() > nextNoFOVAttack ) && AI_ENEMY_VISIBLE ) || AI_ENEMY_IN_FOV )
	{
		range = EnemyRange();
		currentTime = gameLocal.SysScriptTime();
		if( ( range < ZCT_TENTACLE_RANGE_MAX ) && ( !CanReachEnemy() || ( currentTime >= nextAttack ) ) )
		{
			if( CanHitEnemy() )
			{
				attack_flags |= ATTACK_TENTACLE;
			}
		}
	}

	return attack_flags;
}

/*
=====================
iceMonsterZombieCommandoTentacle::do_attack
=====================
*/
void iceMonsterZombieCommandoTentacle::do_attack( int attack_flags )
{
	if( attack_flags & ATTACK_MELEE )
	{
		AI_ATTACKING = true;
		SetState( "combat_melee" );
	}
	else if( attack_flags & ATTACK_TENTACLE )
	{
		SetState( "combat_tentacle" );
	}
}

/*
=====================
iceMonsterZombieCommandoTentacle::tentacle_attack_start

Called from md5Anim frame via TypeInfoGen invoke
=====================
*/
void iceMonsterZombieCommandoTentacle::tentacle_attack_start()
{
	tentacleDamage = true;
}

/*
=====================
iceMonsterZombieCommandoTentacle::tentacle_attack_end

Called from md5Anim frame via TypeInfoGen invoke
=====================
*/
void iceMonsterZombieCommandoTentacle::tentacle_attack_end()
{
	tentacleDamage = false;
}
