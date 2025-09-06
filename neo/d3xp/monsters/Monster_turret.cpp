
#include "precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

#define TURRET_FIRE_RATE		0.1
#define TURRET_MIN_ATTACK_TIME	1.5
#define TURRET_SHUTDOWN_TIME	4

CLASS_DECLARATION( idAI, iceMonster_Turret )
END_CLASS

void iceMonster_Turret::Init( )
{
	float team = 0.0f;
	fire = false;
	attack_monsters = false;
	light_is_on = false;
	attackTime = 0.0f;
	currentBarrel = 1;

	// don't take damage
	Event_IgnoreDamage( );

	// can't move, so only fire on sight
	ambush = true;

	team = GetFloatKey( "team" );
	if( team != 1 )
	{
		attack_monsters = true;
	}

	if( GetIntKey( "light" ) )
	{
		spawn_light( );
	}

	spawnArgs.GetInt( "barrels", "1", barrelCount );

	flashJointWorldHandles.SetGranularity( 1 );
	flashJointWorldHandles.Alloc() = flashJointWorld;
	if( barrelCount > 1 )
	{
		flashJointWorldHandles.SetGranularity( 1 );
		idStr currentFlashStr;
		for( int i = 1 ; i < barrelCount; i++ )
		{
			currentFlashStr = "flash";
			char buf[128];
			buf[0] = '.';
			sprintf( &buf[1], "%03d", i );
			currentFlashStr += buf;
			flashJointWorldHandles.Alloc() = animator.GetJointHandle( currentFlashStr );
		}
	}
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 0 );
}

stateResult_t iceMonster_Turret::state_WakeUp( stateParms_t* parms )
{
	Event_SetState( "state_Begin" );
	return SRESULT_DONE;
}

stateResult_t iceMonster_Turret::state_Begin( stateParms_t* parms )
{
	Event_SetMoveType( MOVETYPE_STATIC );

	if( GetIntKey( "trigger" ) )
	{
		Event_SetState( "state_Disabled" );
	}
	else
	{
		if( parms->stage == 0 )
		{
			wait_for_enemy( parms );
			return SRESULT_WAIT;
		}
		else if( wait_for_enemy( parms ) == SRESULT_DONE )
		{
			Event_SetState( "state_Combat" );
			return SRESULT_DONE;
		}
	}
	return SRESULT_WAIT;
}

void iceMonster_Turret::AI_Begin( )
{
	Event_SetState( "state_Begin" );
}

stateResult_t	iceMonster_Turret::combat_attack( stateParms_t* parms )
{
	if( parms->stage == 1 )
	{
		Event_FaceEnemy( );
		fire = true;
		parms->stage = 2;
	}

	if( parms->stage == 2 )
	{
		if( AI_ENEMY_VISIBLE && canHit() )
		{
			Event_LookAtEnemy( 1 );
			//waitframe();

			if( gameLocal.InfluenceActive() )
			{
				fire = false;
				return SRESULT_DONE;
			}

			if( AI_ACTIVATED )
			{
				Event_SetState( "state_Disabled" );

				return SRESULT_DONE;
			}

			return SRESULT_WAIT;
		}
		else
		{
			fire = false;
			return SRESULT_DONE;
		}
	}

	//should never hit, satisfying compiler.
	//assert( 0, "DANGER DANGER, MR ROBINSON" );
	return SRESULT_DONE;
}

bool iceMonster_Turret::canHit()
{
	currentBarrelStr = "barrel";
	if( barrelCount > 1  && currentBarrel > 1 )
	{
		char buf[128];
		buf[0] = '.';
		sprintf( &buf[1], "%03d", currentBarrel - 1 );
		currentBarrelStr += buf;

	}

	return CanHitEnemyFromJoint( currentBarrelStr );
}

stateResult_t iceMonster_Turret::Torso_Death( stateParms_t* parms )
{
	Event_FinishAction( "dead" );
	return SRESULT_WAIT;
}

stateResult_t iceMonster_Turret::Torso_Idle( stateParms_t* parms )
{
	Event_PlayCycle( ANIMCHANNEL_TORSO, "idle" );

	if( fire )
	{
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Attack", 1 );
	}
	return SRESULT_WAIT;
}

stateResult_t iceMonster_Turret::Torso_Attack( stateParms_t* parms )
{
	enum
	{
		STAGE_WINDUP = 1,
		STAGE_FIRE,
		STAGE_WINDDOWN
	};

	if( parms->stage == 0 )
	{
		int time = 0;
		StartSound( "snd_windup", ( s_channelType ) SND_CHANNEL_BODY, 0, false, &time );
		parms->param1 = gameLocal.SysScriptTime( ) + MS2SEC( time ) ;
		parms->stage = STAGE_WINDUP;
	}

	//param1 float soundLen;
	if( parms->stage == STAGE_WINDUP )
	{
		if( gameLocal.SysScriptTime( ) < parms->param1 )
		{
			return SRESULT_WAIT;
		}

		parms->stage = STAGE_FIRE;
		attackTime = gameLocal.SysScriptTime( ) + TURRET_MIN_ATTACK_TIME;
	}

	if( parms->stage == STAGE_FIRE )
	{
		if( fire || ( gameLocal.SysScriptTime( ) < attackTime ) )
		{
			if( gameLocal.InfluenceActive( ) )
			{
				parms->stage = STAGE_WINDDOWN;
			}
			else
			{
				int time;
				StartSound( "snd_fire", ( s_channelType ) SND_CHANNEL_WEAPON, 0, false, &time );
				flashJointWorld = flashJointWorldHandles[currentBarrel - 1];
				Event_AttackMissile( currentBarrelStr );
				if( ++currentBarrel > barrelCount )
				{
					currentBarrel = 1;
				}
				parms->Wait( TURRET_FIRE_RATE );
			}
		}
		else
		{
			parms->stage = STAGE_WINDDOWN;
		}

		if( parms->stage == STAGE_WINDDOWN )
		{
			int time;
			StartSound( "snd_winddown", ( s_channelType ) SND_CHANNEL_BODY2, 0, false, &time );
			parms->param1 = gameLocal.SysScriptTime( ) + MS2SEC( time );
			parms->stage = STAGE_WINDDOWN;
		}
	}

	if( parms->stage == STAGE_WINDDOWN )
	{
		if( gameLocal.SysScriptTime( ) < parms->param1 )
		{
			return SRESULT_WAIT;
		}
	}

	Event_FinishAction( "attack" );
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 1 );
	return SRESULT_DONE;
}

stateResult_t iceMonster_Turret::Torso_CustomCycle( stateParms_t* parms )
{
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 1 );
	return SRESULT_DONE;
}

bool iceMonster_Turret::checkForEnemy( float use_fov )
{
	idActor* enemy = NULL;

	enemy = FindEnemy( false );
	if( !enemy )
	{
		if( !attack_monsters )
		{
			return false;
		}

		enemy = FindEnemyAI( false );
		if( !enemy )
		{
			return false;
		}
	}

	SetEnemy( enemy );
	return true;
}

stateResult_t iceMonster_Turret::state_Combat( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_StartSound( "snd_wakeup", SND_CHANNEL_VOICE, false );
		light_on( );
		parms->stage = 1;
		parms->param1 = gameLocal.SysScriptTime() + TURRET_SHUTDOWN_TIME;
	}

	Event_LookAtEnemy( 1 );

	if( AI_ENEMY_DEAD )
	{
		enemy_dead( );
		return SRESULT_WAIT;
	}

	if( AI_ACTIVATED )
	{
		Event_SetState( "state_Disabled" );
		return SRESULT_DONE;
	}

	if( AI_ENEMY_VISIBLE )
	{
		if( canHit() )
		{
			if( combat_attack( parms ) != SRESULT_DONE )
			{
				return SRESULT_WAIT;
			}
		}
		else
		{
			checkForEnemy( false );
		}
	}
	else if( !checkForEnemy( false ) )
	{
		if( parms->param1 < gameLocal.SysScriptTime( ) )
		{
			ClearEnemy( );
			Event_SetState( "state_Idle" );
			fire = false;
			return SRESULT_DONE;
		}
	}

	return SRESULT_WAIT;
}

stateResult_t iceMonster_Turret::state_Disabled( stateParms_t* parms )
{
	idActor* enemy;

	enum
	{
		STAGE_SHUTDOWN = 0,
		STAGE_WAIT_STOP_FIRE,
		STAGE_WAIT_ACTIVATION
	};

	switch( parms->stage )
	{
		case STAGE_SHUTDOWN:
			Event_StartSound( "snd_shutdown", SND_CHANNEL_VOICE, false );

			// wait till we stop firing and bullets are out of the air
			attackTime = 0;
			fire = false;
			parms->stage = STAGE_WAIT_STOP_FIRE;
			return SRESULT_WAIT;
		case STAGE_WAIT_STOP_FIRE:
			if( InAnimState( ANIMCHANNEL_TORSO, "Torso_Attack" ) )
			{
				return SRESULT_WAIT;
			}

			parms->Wait( 0.2f );

			// tell all enemies to forget about us
			enemy = NextEnemy( NULL );
			while( enemy )
			{
				idAI* enm = enemy->Cast<idAI>();
				if( enm )
				{
					enm->ClearEnemy();
				}
				enemy = NextEnemy( NULL );
			}

			//
			// clear our enemy
			ClearEnemy( );
			AI_ACTIVATED = false;
			parms->stage = STAGE_WAIT_ACTIVATION;
			return SRESULT_WAIT;
		case STAGE_WAIT_ACTIVATION:
			if( !AI_ACTIVATED )
			{
				return SRESULT_WAIT;
			}
			break;
		default:
			//should never hit!
			//assert( 0, "DANGER DANGER MR ROBINSON" );
			break;
	}

	AI_ACTIVATED = false;
	Event_SetState( "state_Combat" );
	return SRESULT_DONE;
}

stateResult_t iceMonster_Turret::state_Idle( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_StartSound( "snd_shutdown", SND_CHANNEL_VOICE, false );
		light_off( );
		wait_for_enemy( parms );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( wait_for_enemy( parms ) == SRESULT_DONE )
		{
			Event_SetState( "state_Combat" );
			return SRESULT_DONE;
		}
		return SRESULT_WAIT;
	}

	//should never hit, satisfying compiler.
	//assert( 0, "DANGER DANGER MR ROBINSON" );
	return SRESULT_DONE;
}

stateResult_t iceMonster_Turret::state_Killed( stateParms_t* parms )
{
	Event_StopMove( );

	light_off( );

	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Death", 1 );

	Event_WaitAction( "dead" );

	Event_StopThinking( );

	return SRESULT_DONE;
}
