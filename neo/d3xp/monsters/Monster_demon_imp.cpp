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

#define IMP_RUNDISTANCE		192
#define IMP_WALKTURN		65
#define IMP_ATTACK_RATE		1
#define IMP_LEAP_RATE		7
#define IMP_LEAP_SPEED		650
#define IMP_LEAP_MAXHEIGHT	48
#define IMP_DODGE_RATE		8
#define IMP_LEAP_RANGE_MIN	200
#define IMP_LEAP_RANGE_MAX	480
#define IMP_NOFOVTIME		4

CLASS_DECLARATION( idAI, iceMonsterDemonImp )
END_CLASS

/*
=================
iceMonsterDemonImp::Init
=================
*/
void iceMonsterDemonImp::Init()
{
	jumpVelocity.LinkTo( scriptObject, "jumpVelocity" );
	range_attack_anim.LinkTo( scriptObject, "range_attack_anim" );
}

/*
=================
iceMonsterDemonImp::AI_Begin
=================
*/
void iceMonsterDemonImp::AI_Begin()
{
	run_distance = IMP_RUNDISTANCE;
	walk_turn = IMP_WALKTURN;

	Event_SetState( "state_Begin" );
}

/*
=====================
iceMonsterDemonImp::state_Begin
=====================
*/
stateResult_t iceMonsterDemonImp::state_Begin( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 0 );

	Event_SetMoveType( MOVETYPE_ANIM );
	Event_SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterDemonImp::state_Idle
=====================
*/
stateResult_t iceMonsterDemonImp::state_Idle( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		if( wait_for_enemy( parms ) == SRESULT_DONE )
		{
			parms->stage = 1;
		}

		return SRESULT_WAIT;
	}

	nextLeap = gameLocal.RandomTime( IMP_LEAP_RATE );
	nextAttack = 0;
	nextDodge = gameLocal.RandomTime( IMP_DODGE_RATE );
	nextNoFOVAttack = 0;


	Event_SetState( "state_Combat" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterDemonImp::do_attack
=====================
*/
void iceMonsterDemonImp::do_attack( int attack_flags )
{
	nextNoFOVAttack = gameLocal.SysScriptTime() + IMP_NOFOVTIME;
	if( attack_flags & ATTACK_DODGE_LEFT )
	{
		SetState( "combat_dodge_left" );
	}
	else if( attack_flags & ATTACK_DODGE_RIGHT )
	{
		SetState( "combat_dodge_right" );
	}
	else if( attack_flags & ATTACK_COMBAT_NODE )
	{
		//combat_ainode(combat_node);
		gameLocal.Error( "combat_ainode" );
	}
	else if( attack_flags & ATTACK_MELEE )
	{
		SetState( "combat_melee" );
	}
	else if( attack_flags & ATTACK_LEAP )
	{
		SetState( "combat_leap" );
	}
	else if( attack_flags & ATTACK_MISSILE )
	{
		SetState( "combat_range" );
	}
}

/*
=====================
iceMonsterDemonImp::check_attacks
=====================
*/
int iceMonsterDemonImp::check_attacks()
{
	float range;
	float currentTime;
	float canMelee;
	int attack_flags;
	float checkLeap;
	idVec3 vel;
	float t;
	idStr anim;
	idVec3 jumpTarget;

	attack_flags = 0;

	canMelee = TestMelee();
	currentTime = gameLocal.SysScriptTime();
	if( !canMelee )
	{
		if( AI_PAIN && ( currentTime >= nextDodge ) )
		{
			if( TestAnimMove( "evade_left" ) )
			{
				attack_flags |= ATTACK_DODGE_LEFT;
			}
			if( TestAnimMove( "evade_right" ) )
			{
				attack_flags |= ATTACK_DODGE_RIGHT;

				// if we can dodge either direction, pick one
				if( attack_flags & ATTACK_DODGE_LEFT )
				{
					if( gameLocal.Random( 100 ) < 50 )
					{
						attack_flags &= ~ATTACK_DODGE_RIGHT;
					}
					else
					{
						attack_flags &= ~ATTACK_DODGE_LEFT;
					}
				}
			}
		}

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
		range = EnemyRange();
		if( ( range >= IMP_LEAP_RANGE_MIN ) && ( range < IMP_LEAP_RANGE_MAX ) && ( currentTime >= nextLeap ) )
		{
			if( CanHitEnemy() )
			{
				t = AnimLength( ANIMCHANNEL_TORSO, "jump_start" );
				jumpTarget = PredictEnemyPos( t );
				idVec3 _jumpVelocity = GetJumpVelocity( jumpTarget, IMP_LEAP_SPEED, IMP_LEAP_MAXHEIGHT );
				if( _jumpVelocity != vec3_origin )
				{
					attack_flags |= ATTACK_LEAP;
				}
				else
				{
					// check if we can leap again in 2 seconds
					nextLeap = gameLocal.DelayTime( 2 );
				}

				jumpVelocity = _jumpVelocity;
			}
		}

		if( !CanReachEnemy() || ( currentTime >= nextAttack ) )
		{
// jmarshall: can't pass idStr to doomscript for some reason, currently hardcoded in script.
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
iceMonsterDemonImp::combat_range
=====================
*/
stateResult_t iceMonsterDemonImp::combat_range( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_RangeAttack", 4 );
		SetWaitState( "range_attack" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( waitState != "" )
		{
			return SRESULT_WAIT;
		}
	}

	// don't attack for a bit
	nextAttack = gameLocal.DelayTime( IMP_ATTACK_RATE );
	nextNoFOVAttack = gameLocal.SysScriptTime() + IMP_NOFOVTIME;
	return SRESULT_DONE;
}

/*
=====================
iceMonsterDemonImp::combat_leap
=====================
*/
stateResult_t iceMonsterDemonImp::combat_leap( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_StopMove();
		Event_TurnToPos( GetOrigin() + jumpVelocity );
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_LeapAttack", 4 );
		SetWaitState( "leap_attack" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( waitState != "" )
		{
			return SRESULT_WAIT;
		}
	}

	nextLeap = gameLocal.DelayTime( IMP_LEAP_RATE );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterDemonImp::combat_melee
=====================
*/
stateResult_t iceMonsterDemonImp::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_LookAtEnemy( 100 );
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_MeleeAttack", 5 );
		SetWaitState( "melee_attack" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( waitState != "" )
		{
			return SRESULT_WAIT;
		}
	}

	Event_LookAtEnemy( 1 );
	Event_StopMove();
	return SRESULT_DONE;
}

/*
=====================
monster_demon_imp::combat_dodge_left
=====================
*/
stateResult_t iceMonsterDemonImp::combat_dodge_left( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_FaceEnemy();
		Event_StopMove();
		Event_AnimState( ANIMCHANNEL_LEGS, "Legs_DodgeLeft", 2 );
		SetWaitState( "strafe" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( waitState != "" )
		{
			return SRESULT_WAIT;
		}
	}

	nextDodge = gameLocal.DelayTime( IMP_DODGE_RATE );
}

/*
=====================
monster_demon_imp::combat_dodge_right
=====================
*/
stateResult_t iceMonsterDemonImp::combat_dodge_right( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_FaceEnemy();
		Event_StopMove();
		Event_AnimState( ANIMCHANNEL_LEGS, "Legs_DodgeRight", 2 );
		SetWaitState( "strafe" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( waitState != "" )
		{
			return SRESULT_WAIT;
		}
	}

	nextDodge = gameLocal.DelayTime( IMP_DODGE_RATE );
}