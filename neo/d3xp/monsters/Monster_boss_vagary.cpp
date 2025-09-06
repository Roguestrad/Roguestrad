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

#define VAGARY_ATTACK_RATE			1.5
#define VAGARY_DODGE_RATE			4
#define	VAGARY_PAIN_DELAY			0.25
#define	VAGARY_TURRET_TO_IDLE		4
#define	VAGARY_THROW_SPEED			1200
#define VAGARY_THROW_SPEED2			( VAGARY_THROW_SPEED + 300 )
#define	VAGARY_THROW_DIST			200
#define VAGARY_THROW_MIN			idVec3(-512,-512,-512)
#define	VAGARY_THROW_MAX			idVec3(512,512,512)
#define VAGARY_THROW_OFFSET			64
#define VAGARY_NOFOVTIME			4

// anim blend times
#define	VAGARY_PAIN_TO_IDLE			2
#define VAGARY_PAIN_TO_PAIN			0
#define VAGARY_SIGHT_TO_IDLE		4
#define	VAGARY_MELEE_TO_IDLE		4
#define VAGARY_RANGE_TO_IDLE		4
#define VAGARY_DODGE_LEFT_TO_IDLE	4
#define	VAGARY_DODGE_RIGHT_TO_IDLE	4
#define VAGARY_WAIT_TO_OUT			4
#define	VAGARY_IN_TO_WAIT			4
#define	VAGARY_WALK_TO_WAIT			4
#define VAGARY_WALK_TO_ACTION		4
#define VAGARY_ACTION_TO_IDLE		4
#define VAGARY_WALK_TO_DODGE_LEFT	4
#define VAGARY_WALK_TO_DODGE_RIGHT	4
#define VAGARY_IDLE_TO_PAIN			0
#define VAGARY_IDLE_TO_WALK			4
#define VAGARY_IDLE_TO_SIGHT		4
#define VAGARY_WALK_TO_IDLE			4
#define VAGARY_WALK_TO_RANGEATTACK	4
#define VAGARY_IDLE_TO_RANGEATTACK	4
#define VAGARY_WALK_TO_MELEE		4
#define VAGARY_IDLE_TO_TURRETATTACK	4
#define VAGARY_TURRETATTACK_TO_IDLE	4

CLASS_DECLARATION( idAI, iceMonsterBossVagary )
END_CLASS

/*
=================
iceMonsterDemonHellknight::Init
=================
*/
void iceMonsterBossVagary::Init()
{
}

/*
=================
iceMonsterDemonHellknight::AI_Begin
=================
*/
void iceMonsterBossVagary::AI_Begin()
{
	Event_SetState( "state_Begin" );
}

/*
=====================
iceMonsterBossVagary::state_Begin
=====================
*/
stateResult_t iceMonsterBossVagary::state_Begin( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
	Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Idle", 0 );

	Event_SetMoveType( MOVETYPE_ANIM );
	Event_SetState( "state_Idle" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterBossVagary::state_Idle
=====================
*/
stateResult_t iceMonsterBossVagary::state_Idle( stateParms_t* parms )
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
	nextDodge = gameLocal.RandomTime( VAGARY_DODGE_RATE );

	Event_SetState( "state_Combat" );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterBossVagary::do_attack
=====================
*/
void iceMonsterBossVagary::do_attack( int attack_flags )
{
	nextNoFOVAttack = gameLocal.SysScriptTime() + VAGARY_NOFOVTIME;
	if( attack_flags & ATTACK_DODGE_LEFT )
	{
		stateThread.SetState( "combat_dodge_left" );
	}
	else if( attack_flags & ATTACK_DODGE_RIGHT )
	{
		//combat_dodge_right();
		stateThread.SetState( "combat_dodge_right" );
	}
	else if( attack_flags & ATTACK_COMBAT_NODE )
	{
		//combat_ainode(combat_node);
		gameLocal.Error( "iceMonsterBossVagary::CombatAINode\n" );
	}
	else if( attack_flags & ATTACK_MELEE )
	{
		//crouch_attack();
		stateThread.SetState( "combat_melee" );
	}
	else if( attack_flags & ATTACK_MISSILE )
	{
		//stand_attack();
		stateThread.SetState( "combat_range" );
	}
}

/*
=====================
iceMonsterBossVagary::combat_range
=====================
*/
stateResult_t iceMonsterBossVagary::combat_range( stateParms_t* parms )
{
	if( parms->stage == 10 )
	{
		pos = throwEntity->GetOrigin();
		pos.z += VAGARY_THROW_OFFSET;

		Event_StartSound( "snd_pickup", SND_CHANNEL_WEAPON, false );

		throwEntity->Event_SetAngularVelocity( idVec3( 30, 30, 0 ) );
		waitTime = gameLocal.SysScriptTime() + 0.5;
		start_offset = gameLocal.SysScriptTime() + gameLocal.Random( 360 );

		parms->stage = 11;

		return SRESULT_WAIT;
	}

	if( parms->stage == 11 )
	{
		if( gameLocal.SysScriptTime() < waitTime )
		{
			t = gameLocal.SysScriptTime() - start_offset * 360 * 2;
			offset.z = sin( t ) * 8;
			vel = ( pos - throwEntity->GetOrigin() + offset ) * 5;
			throwEntity->Event_SetLinearVelocity( vel );
			return SRESULT_WAIT;
		}
		ThrowObjectAtEnemy( throwEntity, VAGARY_THROW_SPEED2 );
		parms->stage = parms->param1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 0 )
	{
		Event_FaceEnemy();
		Event_LookAtEntity( throwEntity, 3 );
		parms->Wait( 0.3 );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( throwEntity == NULL )
		{
			parms->stage = 3;
			parms->Wait( 1 );
			return SRESULT_WAIT;
		}

		parms->stage = 10;
		parms->param1 = 2; // Return Stage
		parms->param2 = 1; // Start Entity
		parms->subparam1 = gameLocal.Random( 4 );
		return SRESULT_WAIT;
	}

	if( parms->stage == 2 )
	{
		int num = parms->subparam1;

		if( parms->param2 < num )
		{
			throwEntity = ChooseObjectToThrow( VAGARY_THROW_MIN, VAGARY_THROW_MAX, VAGARY_THROW_SPEED, VAGARY_THROW_DIST, VAGARY_THROW_OFFSET );
			parms->stage = 10;
			parms->param1 = 2; // Return Stage
			parms->param2++; // Start Entity
			return SRESULT_WAIT;
		}
		else
		{
			parms->stage = 3;
			parms->Wait( 1 );
			return SRESULT_WAIT;
		}
	}

	Event_LookAtEnemy( 1 );

	// don't attack for a bit
	nextAttack = gameLocal.DelayTime( VAGARY_ATTACK_RATE );
	nextNoFOVAttack = gameLocal.SysScriptTime() + VAGARY_NOFOVTIME;
	return SRESULT_WAIT;
}

/*
=====================
iceMonsterBossVagary::combat_melee
=====================
*/
stateResult_t iceMonsterBossVagary::combat_melee( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_LookAtEnemy( 100 );
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_LEGS, "Torso_MeleeAttack", VAGARY_WALK_TO_MELEE );
		SetWaitState( "melee_attack" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( !AnimDone( ANIMCHANNEL_LEGS, 0 ) )
	{
		return SRESULT_WAIT;
	}

	parms->stage = 2;

	Event_LookAtEnemy( 1 );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterBossVagary::combat_dodge_left
=====================
*/
stateResult_t iceMonsterBossVagary::combat_dodge_left( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_StopMove();
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_LEGS, "Legs_DodgeLeft", VAGARY_WALK_TO_DODGE_LEFT );
		SetWaitState( "strafe" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( !AnimDone( ANIMCHANNEL_LEGS, 0 ) )
	{
		return SRESULT_WAIT;
	}

	parms->stage = 2;

	nextDodge = gameLocal.DelayTime( VAGARY_DODGE_RATE );
	return SRESULT_DONE;
}

/*
=====================
iceMonsterBossVagary::combat_dodge_right
=====================
*/
stateResult_t iceMonsterBossVagary::combat_dodge_right( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_StopMove();
		Event_FaceEnemy();
		Event_AnimState( ANIMCHANNEL_LEGS, "Legs_DodgeRight", VAGARY_WALK_TO_DODGE_RIGHT );
		SetWaitState( "strafe" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( !AnimDone( ANIMCHANNEL_LEGS, 0 ) )
	{
		return SRESULT_WAIT;
	}

	parms->stage = 2;

	nextDodge = gameLocal.DelayTime( VAGARY_DODGE_RATE );
	return SRESULT_DONE;
}


/*
=====================
iceMonsterBossVagary::check_attacks
=====================
*/
int iceMonsterBossVagary::check_attacks()
{
	float canMelee;
	float currentTime;
	int attack_flags;

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
// jmarshall - fix throwing?
	//if (((gameLocal.SysScriptTime() > nextNoFOVAttack) && AI_ENEMY_VISIBLE) || AI_ENEMY_IN_FOV) {
	//	if (!CanReachEnemy() || (currentTime >= nextAttack)) {
	//		if (CanHitEnemy()) {
	//			throwEntity = ChooseObjectToThrow(VAGARY_THROW_MIN, VAGARY_THROW_MAX, VAGARY_THROW_SPEED, VAGARY_THROW_DIST, VAGARY_THROW_OFFSET);
	//			if (throwEntity) {
	//				attack_flags |= ATTACK_MISSILE;
	//			}
	//		}
	//	}
	//}

	return attack_flags;
}

/*
================
iceMonsterBossVagary::ChooseObjectToThrow
================
*/
idEntity* iceMonsterBossVagary::ChooseObjectToThrow( const idVec3& mins, const idVec3& maxs, float speed, float minDist, float offset )
{
	idEntity* 	ent;
	idEntity* 	entityList[ MAX_GENTITIES ];
	int			numListedEntities;
	int			i, index;
	float		dist;
	idVec3		vel;
	idVec3		offsetVec( 0, 0, offset );
	idEntity*	enemyEnt = enemy.GetEntity();

	if( !enemyEnt )
	{
		return ( NULL );
	}

	idVec3 enemyEyePos = lastVisibleEnemyPos + lastVisibleEnemyEyeOffset;
	const idBounds& myBounds = physicsObj.GetAbsBounds();
	idBounds checkBounds( mins, maxs );
	checkBounds.TranslateSelf( physicsObj.GetOrigin() );
	numListedEntities = gameLocal.clip.EntitiesTouchingBounds( checkBounds, -1, entityList, MAX_GENTITIES );

	index = gameLocal.random.RandomInt( numListedEntities );
	for( i = 0; i < numListedEntities; i++, index++ )
	{
		if( index >= numListedEntities )
		{
			index = 0;
		}
		ent = entityList[ index ];
		if( !ent->IsType( idMoveable::Type ) )
		{
			continue;
		}

		if( ent->fl.hidden )
		{
			// don't throw hidden objects
			continue;
		}

		idPhysics* entPhys = ent->GetPhysics();
		const idVec3& entOrg = entPhys->GetOrigin();
		dist = ( entOrg - enemyEyePos ).LengthFast();
		if( dist < minDist )
		{
			continue;
		}

		idBounds expandedBounds = myBounds.Expand( entPhys->GetBounds().GetRadius() );
		if( expandedBounds.LineIntersection( entOrg, enemyEyePos ) )
		{
			// ignore objects that are behind us
			continue;
		}

		if( PredictTrajectory( entPhys->GetOrigin() + offsetVec, enemyEyePos, speed, entPhys->GetGravity(),
							   entPhys->GetClipModel(), entPhys->GetClipMask(), MAX_WORLD_SIZE, NULL, enemyEnt, ai_debugTrajectory.GetBool() ? 4000 : 0, vel ) )
		{
			return  ent ;
		}
	}

	return NULL;
}

/*
================
iceMonsterBossVagary::ThrowObjectAtEnemy
================
*/
void iceMonsterBossVagary::ThrowObjectAtEnemy( idEntity* ent, float speed )
{
	idVec3		vel;
	idEntity*	enemyEnt;
	idPhysics*	entPhys;

	entPhys	= ent->GetPhysics();
	enemyEnt = enemy.GetEntity();
	if( !enemyEnt )
	{
		vel = ( viewAxis[ 0 ] * physicsObj.GetGravityAxis() ) * speed;
	}
	else
	{
		PredictTrajectory( entPhys->GetOrigin(), lastVisibleEnemyPos + lastVisibleEnemyEyeOffset, speed, entPhys->GetGravity(),
						   entPhys->GetClipModel(), entPhys->GetClipMask(), MAX_WORLD_SIZE, NULL, enemyEnt, ai_debugTrajectory.GetBool() ? 4000 : 0, vel );
		vel *= speed;
	}

	entPhys->SetLinearVelocity( vel );

	if( ent->IsType( idMoveable::Type ) )
	{
		idMoveable* ment = static_cast<idMoveable*>( ent );
		ment->EnableDamage( true, 2.5f );
	}
}
