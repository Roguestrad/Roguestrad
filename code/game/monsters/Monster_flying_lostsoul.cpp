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

#define LOSTSOUL_CHARGE_SPEED		440
#define LOSTSOUL_NORMAL_SPEED		140
#define LOSTSOUL_ATTACK_RATE		2
#define LOSTSOUL_CHARGE_RANGE_MAX	768
#define LOSTSOUL_CHARGE_RANGE_MIN	64
#define LOSTSOUL_NOFOVTIME			4

#define ATTACK_LOSTSOUL_CHARGE	ATTACK_SPECIAL1
#define ATTACK_LOSTSOUL_RETREAT	ATTACK_SPECIAL2

CLASS_DECLARATION( idAI, iceMonsterLostSoul )
END_CLASS

/*
====================
iceMonsterLostSoul::Init
====================
*/
void iceMonsterLostSoul::Init()
{

}

/*
=================
iceMonsterLostSoul::AI_Begin
=================
*/
void iceMonsterLostSoul::AI_Begin()
{
	fly_offset = GetFloatKey( "fly_offset" );
	Event_SetState( "state_Begin" );
}

/*
=====================
iceMonsterLostSoul::state_Begin
=====================
*/
stateResult_t iceMonsterLostSoul::state_Begin( stateParms_t* parms )
{
	Event_SetMoveType( MOVETYPE_FLY );
	noMeleeTime = 0;
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
	Event_SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterLostSoul::state_Idle
=====================
*/
stateResult_t iceMonsterLostSoul::state_Idle( stateParms_t* parms )
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
iceMonsterLostSoul::do_attack
=====================
*/
void iceMonsterLostSoul::do_attack( int attack_flags )
{
	nextNoFOVAttack = gameLocal.SysScriptTime() + LOSTSOUL_NOFOVTIME;
	if( attack_flags & ATTACK_LOSTSOUL_RETREAT )
	{
		stateThread.SetState( "combat_retreat" );
	}
	else if( attack_flags & ATTACK_MELEE )
	{
		stateThread.SetState( "combat_melee" );
	}
	else if( attack_flags & ATTACK_LOSTSOUL_CHARGE )
	{
		stateThread.SetState( "combat_charge" );
	}
}

/*
=====================
iceMonsterLostSoul::check_attacks
=====================
*/
int iceMonsterLostSoul::check_attacks()
{
	float range;
	float currentTime;
	int attack_flags;

	attack_flags = 0;
	currentTime = gameLocal.SysScriptTime();

	if( currentTime >= noMeleeTime )
	{
		if( TestMelee() )
		{
			attack_flags |= ATTACK_MELEE;
		}
	}

	if( ( ( gameLocal.SysScriptTime() > nextNoFOVAttack ) && AI_ENEMY_VISIBLE ) || AI_ENEMY_IN_FOV )
	{
		range = EnemyRange2D();
		if( range < 40 )
		{
			attack_flags |= ATTACK_LOSTSOUL_RETREAT;
		}

		if( ( range < LOSTSOUL_CHARGE_RANGE_MAX && range > 200 ) && ( !CanReachEnemy() || ( currentTime >= nextAttack ) ) )
		{
			if( TestChargeAttack() )
			{
				attack_flags |= ATTACK_LOSTSOUL_CHARGE;
			}
		}
	}

	return attack_flags;
}

/*
=====================
iceMonsterLostSoul::combat_charge
=====================
*/
stateResult_t iceMonsterLostSoul::combat_charge( stateParms_t* parms )
{
	// rise up a bit
	if( parms->stage == 0 )
	{
		Event_SetFlyOffset( fly_offset + 200 );
		parms->Wait( 0.5f );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		Event_FaceEnemy();

		// determin direction to enemy
		idVec3 pos = GetEnemyEyePos();
		idVec3 org = GetOrigin();
		idVec3 forward = pos - org;
		forward.z = 0;
		forward.Normalize();

		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Charge", 4 );

		// set speed and height
		vel = forward * LOSTSOUL_CHARGE_SPEED;
		Event_SetFlyOffset( fly_offset );
		Event_SetFlySpeed( LOSTSOUL_CHARGE_SPEED );

		parms->stage = 2;
		return SRESULT_WAIT;
	}

	if( parms->stage == 2 )
	{
		if( InAnimState( ANIMCHANNEL_TORSO, "Torso_Charge" ) )
		{
			// do a dive bomb run
			idVec3 org = GetOrigin();
// jmarshall - need to fix this, causing "huge translation" issue.
			//vel.z = -(org.z - pos.z + 16) * 3;
// jmarshall end
			Event_SetLinearVelocity( vel );
			return SRESULT_WAIT;
		}
	}

	// restore the fly speed
	Event_SetFlySpeed( LOSTSOUL_NORMAL_SPEED );

	// don't attack for a bit
	nextAttack = gameLocal.DelayTime( LOSTSOUL_ATTACK_RATE );
	nextNoFOVAttack = gameLocal.SysScriptTime() + LOSTSOUL_NOFOVTIME;

	// don't allow melee for a bit so that charge attacks don't cause double damage
	noMeleeTime = gameLocal.SysScriptTime() + 1;

	return SRESULT_DONE;
}
/*
=====================
iceMonsterLostSoul::combat_melee
=====================
*/
stateResult_t iceMonsterLostSoul::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_MeleeAttack", 4 );
		SetWaitState( "melee_attack" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( waitState != "" )
	{
		return SRESULT_WAIT;
	}

	return SRESULT_DONE;
}

/*
=====================
iceMonsterLostSoul::combat_retreat
=====================
*/
stateResult_t iceMonsterLostSoul::combat_retreat( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_MoveOutOfRange( GetEnemy(), EnemyRange2D() + 256 );
		endtime = gameLocal.SysScriptTime() + 1;
		parms->stage = 1;
		return SRESULT_WAIT;
	}


	if( endtime < gameLocal.SysScriptTime() )
	{
		if( AI_MOVE_DONE )
		{
			return SRESULT_DONE;
		}
	}

	return SRESULT_DONE;
}