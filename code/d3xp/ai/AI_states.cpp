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
idAI::state_TeleportTriggered
=====================
*/
stateResult_t idAI::state_TeleportTriggered( stateParms_t* parms )
{
	int teleportType = 0;

	enum
	{
		STAGE_TRIGGER_TELEPORT_INIT = 0,
		STAGE_TRIGGER_TELEPORT_WAIT_FOR_ACTIVATION,
		STAGE_TRIGGER_TELEPORT_SPAWN_VFX,
		STAGE_TRIGGER_TELEPORT_CUSTOM_ANIMATION,
		STAGE_TRIGGER_TELEPORT_START_ACTIVE,
	};

	//
	// teleport in when triggered
	//
	switch( parms->stage )
	{
		case STAGE_TRIGGER_TELEPORT_INIT:
			Hide();
			parms->stage = STAGE_TRIGGER_TELEPORT_WAIT_FOR_ACTIVATION;
			return SRESULT_WAIT;

		case STAGE_TRIGGER_TELEPORT_WAIT_FOR_ACTIVATION:
			if( AI_ACTIVATED )
			{
				parms->stage = STAGE_TRIGGER_TELEPORT_SPAWN_VFX;
			}
			return SRESULT_WAIT;

		case STAGE_TRIGGER_TELEPORT_SPAWN_VFX:
			if( CanBecomeSolid() )
			{
				Event_BecomeSolid();
				parms->param1 = move.moveType;

				// don't go dormant during teleport anims since they
				// may end up floating in air during no gravity anims.
				Event_SetNeverDormant( true );

				Event_SetMoveType( MOVETYPE_STATIC );

				teleportType = GetIntKey( "teleport" );

				if( teleportType == 1 )
				{
					Event_StartFx( "fx_teleport1" );
					parms->Wait( 1.6f );
					parms->stage = STAGE_TRIGGER_TELEPORT_CUSTOM_ANIMATION;
				}
				else if( teleportType == 2 )
				{
					Event_StartFx( "fx_teleport2" );
					parms->Wait( 2.6f );
					parms->stage = STAGE_TRIGGER_TELEPORT_CUSTOM_ANIMATION;
				}
				else if( teleportType == 3 )
				{
					Event_StartFx( "fx_teleport3" );
					parms->Wait( 3.6 );
					parms->stage = STAGE_TRIGGER_TELEPORT_CUSTOM_ANIMATION;
				}
				else
				{
					Event_StartFx( "fx_teleport" );
					parms->Wait( 0.6f );
					parms->stage = STAGE_TRIGGER_TELEPORT_CUSTOM_ANIMATION;
				}
			}
			return SRESULT_WAIT;

		case STAGE_TRIGGER_TELEPORT_CUSTOM_ANIMATION:
			Show();
			PlayCustomAnim( "teleport", 0, 4 );
			SetWaitState( "customAnim" );
			parms->stage = STAGE_TRIGGER_TELEPORT_START_ACTIVE;
			return SRESULT_WAIT;

		case STAGE_TRIGGER_TELEPORT_START_ACTIVE:
			if( waitState == "" )
			{
				Event_SetNeverDormant( GetFloatKey( "neverdormant" ) );
				Event_LocateEnemy();
				Event_SetMoveType( parms->param1 );
				stateThread.SetState( "state_WakeUp" );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
=====================
idAI::state_TriggerAnim
=====================
*/
stateResult_t idAI::state_TriggerAnim( stateParms_t* parms )
{
	idStr triggerAnim;

	enum
	{
		STAGE_TRIGGER_ANIM_INIT = 0,
		STAGE_TRIGGER_ANIM_WAIT_FOR_ACTIVATION,
		STAGE_TRIGGER_ANIM_SHOW,
		STAGE_TRIGGER_WAIT_FOR_ANIMATION_COMPLETE,
	};

	//
	// Hide until Triggered State
	//
	switch( parms->stage )
	{
		case STAGE_TRIGGER_ANIM_INIT:
			//
			// hide until triggered and then play a special animation
			//
			triggerAnim = GetKey( "trigger_anim" );
			Event_CheckAnim( ANIMCHANNEL_TORSO, triggerAnim );
			Hide();
			parms->stage = STAGE_TRIGGER_ANIM_WAIT_FOR_ACTIVATION;
			return SRESULT_WAIT;

		case STAGE_TRIGGER_ANIM_WAIT_FOR_ACTIVATION:
			if( AI_ACTIVATED )
			{
				parms->stage = STAGE_TRIGGER_ANIM_SHOW;
			}
			return SRESULT_WAIT;

		case STAGE_TRIGGER_ANIM_SHOW:
			if( CanBecomeSolid() )
			{
				triggerAnim = GetKey( "trigger_anim" );

				// don't go dormant during trigger_anim anims since they
				// may end up floating in air during no gravity anims.
				Event_SetNeverDormant( true );
				Show();
				trigger_wakeup_targets();
				PlayCustomAnim( triggerAnim, 0, 4 );
				parms->stage = STAGE_TRIGGER_WAIT_FOR_ANIMATION_COMPLETE;
				SetWaitState( "customAnim" );
			}
			return SRESULT_WAIT;

		case STAGE_TRIGGER_WAIT_FOR_ANIMATION_COMPLETE:
			if( waitState == "" )
			{
				Event_SetNeverDormant( GetFloatKey( "neverdormant" ) );
				Event_LocateEnemy();
				stateThread.SetState( "state_WakeUp" );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
=====================
idAI::state_TriggerHidden
=====================
*/
stateResult_t idAI::state_TriggerHidden( stateParms_t* parms )
{
	enum
	{
		STAGE_TRIGGER_HIDE_INIT = 0,
		STAGE_TRIGGER_HIDE_WAIT_FOR_ACTIVATION,
		STAGE_TRIGGER_HIDE_SHOW,
	};


	//
	// Hide until triggered
	//
	switch( parms->stage )
	{
		case STAGE_TRIGGER_HIDE_INIT:
			Hide();
			parms->stage = STAGE_TRIGGER_HIDE_WAIT_FOR_ACTIVATION;
			return SRESULT_WAIT;

		case STAGE_TRIGGER_HIDE_WAIT_FOR_ACTIVATION:
			if( AI_ACTIVATED )
			{
				if( ( GetIntKey( "hide" ) == 1 ) || ambush )
				{
					AI_ACTIVATED = false;
					Event_ClearEnemy();
				}
				parms->stage = STAGE_TRIGGER_HIDE_SHOW;
			}
			return SRESULT_WAIT;

		case STAGE_TRIGGER_HIDE_SHOW:
			if( CanBecomeSolid() )
			{
				Show();
				stateThread.SetState( "state_WakeUp" );
			}
			return SRESULT_DONE;
	}

	return SRESULT_ERROR;
}

/*
=====================
idAI::state_WakeUp
=====================
*/
stateResult_t idAI::state_WakeUp( stateParms_t* parms )
{
	float	waittime;
	idEntity* enemy;

	enum
	{
		STAGE_TRIGGER_NORMAL_FLOW_INIT = 0,
		STAGE_TRIGGER_NORMAL_FLOW_FINISH
	};

	AI_RUN = false;
	ignore_sight = GetIntKey( "no_sight" );

	bool start_active = true;

	if( parms->stage == STAGE_TRIGGER_NORMAL_FLOW_INIT )
	{
		parms->stage = STAGE_TRIGGER_NORMAL_FLOW_FINISH;
		waittime = GetFloatKey( "wait" );
		if( waittime > 0 )
		{
			parms->Wait( waittime );
		}
		return SRESULT_WAIT;
	}

	enemy = GetEntityKey( "enemy" );
	if( enemy )
	{
		Event_SetEnemy( enemy );
	}

	if( GetIntKey( "wake_on_attackcone" ) )
	{
		stateThread.SetState( "wake_on_attackcone" );
	}
	else if( GetIntKey( "walk_on_trigger" ) )
	{
		stateThread.SetState( "walk_on_trigger" );
	}
	else if( GetIntKey( "trigger" ) )
	{
		stateThread.SetState( "wake_on_trigger" );
	}
	else
	{
		stateThread.SetState( "wake_on_enemy" );
	}

	return SRESULT_DONE;
}

/*
=====================
idAI::state_Spawner
=====================
*/
stateResult_t idAI::state_Spawner( stateParms_t* parms )
{
	idEntity* ent;
	float	maxSpawn;
	int	i;
	idStr	name;

	if( parms->stage == 0 )
	{
		Hide();
		AI_ACTIVATED = false;
		parms->stage++;
	}

	if( AI_ACTIVATED )
	{
		parms->stage++;
		AI_ACTIVATED = false;
	}

	int triggerCount = parms->stage - 1;
	if( triggerCount > 0 )
	{
		maxSpawn = GetIntKey( "spawner" );
		name = GetName();

		if( CanBecomeSolid() )
		{
			for( i = 0; i < maxSpawn; i++ )
			{
				ent = gameLocal.GetEntity( name + i );
				if( !ent )
				{
					break;
				}
			}

			if( i < maxSpawn )
			{
				triggerCount--;
				idDict dict = spawnArgs;
				dict.Set( "spawner", "0" );
				dict.Set( "name", ( name + i ).c_str() );
				if( GetKey( "spawn_target" ) != "" )
				{
					dict.Set( "target", GetKey( "spawn_target" ) );
				}

				dict.Set( "classname", GetKey( "classname" ) );
				gameLocal.SpawnEntityDef( dict, &ent );
				scriptThread->Event_Trigger( ent );
			}
			parms->Wait( GetFloatKey( "wait" ) );
		}
	}

	return SRESULT_WAIT;
}

/*
=====================
monster_base::wait_for_enemy
=====================
*/
stateResult_t idAI::wait_for_enemy( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		// prevent an infinite loop when in notarget
		AI_PAIN = false;

		Event_StopMove();

		parms->stage = 1;

		return SRESULT_WAIT;
	}

	if( !AI_PAIN && !GetEnemy() )
	{
		if( checkForEnemy( idle_sight_fov ) )
		{
			return SRESULT_DONE;
		}
		else
		{
			return SRESULT_WAIT;
		}
	}

	return SRESULT_DONE;
}

/*
=====================
idAI::wake_on_trigger
=====================
*/
stateResult_t idAI::wake_on_trigger( stateParms_t* parms )
{
	idStr	animname;
	idEntity* path;

	enum
	{
		STAGE_INIT = 0,
		STAGE_WAIT_FOR_ACTIVATION,
		STAGE_FINISH
	};

	if( parms->stage == STAGE_INIT )
	{
		if( !GetIntKey( "attack_path" ) )
		{
			path = idPathCorner::RandomPath( this, NULL );
			if( path )
			{
				idle_followPathEntities( path );
			}
		}

		if( !GetEnemy() && !AI_ACTIVATED && !AI_PAIN )
		{
			// sit in our idle anim till we're activated
			Event_AllowMovement( false );

			animname = GetKey( "anim" );
			PlayCustomCycle( animname, 4 );

			parms->stage = STAGE_WAIT_FOR_ACTIVATION;
		}

		return SRESULT_WAIT;
	}


	if( parms->stage == STAGE_WAIT_FOR_ACTIVATION )
	{
		if( !( AI_ACTIVATED || AI_PAIN ) )
		{
			return SRESULT_WAIT;
		}

		Event_AllowMovement( true );

		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 4 );

		parms->stage = STAGE_FINISH;

		return SRESULT_WAIT;
	}

	trigger_wakeup_targets();
	if( GetIntKey( "attack_path" ) )
	{
		// follow a path and fight player at end
		path = idPathCorner::RandomPath( this, NULL );
		if( path )
		{
			ignoreEnemies = true;
			AI_RUN = true;
			idle_followPathEntities( path );
			ignoreEnemies = false;
		}
	}
	else
	{
		sight_enemy();
	}

	stateThread.SetState( "wake_call_constructor" );

	isAwake = true;

	// allow him to see after he's woken up
	ignore_sight = false;

	// ignore the flashlight from now on
	Event_WakeOnFlashlight( false );

	return SRESULT_DONE;
}

/*
=====================
idAI::walk_on_trigger
=====================
*/
stateResult_t idAI::walk_on_trigger( stateParms_t* parms )
{
	idEntity* path;

	enum
	{
		STAGE_INIT = 0,
		STAGE_WAIT_FOR_ACTIVATION,
		STAGE_AFTER_ACTIVATION,
		STAGE_CHECK_ENEMY,
		STAGE_WAKEUP
	};

	if( parms->stage == STAGE_INIT )
	{
		idStr animname;

		Event_AllowMovement( false );

		// sit in our idle anim till we're activated
		animname = GetKey( "anim" );
		PlayCustomCycle( animname, 4 );

		parms->stage = STAGE_WAIT_FOR_ACTIVATION;
		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_WAIT_FOR_ACTIVATION )
	{
		if( AI_ACTIVATED || AI_PAIN )
		{
			if( AI_ACTIVATED )
			{
				Event_ClearEnemy();
				AI_ACTIVATED = false;
			}

			parms->stage = STAGE_AFTER_ACTIVATION;
		}

		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_AFTER_ACTIVATION )
	{
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 4 );
		Event_AllowMovement( true );

		// follow a path
		path = idPathCorner::RandomPath( this, NULL );
		if( path )
		{
			idle_followPathEntities( path );
		}

		if( !GetEnemy() && !AI_ACTIVATED && !AI_PAIN )
		{
			idStr animname = GetKey( "anim" );

			// sit in our idle anim till we're activated
			Event_AllowMovement( false );
			PlayCustomCycle( animname, 4 );
			parms->stage = STAGE_CHECK_ENEMY;
		}
		else
		{
			parms->stage = STAGE_WAKEUP;
		}

		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_CHECK_ENEMY )
	{
		if( !AI_PAIN && !AI_ACTIVATED )
		{
			checkForEnemy( true );
		}
		else
		{
			Event_AllowMovement( true );
			Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 4 );
			parms->stage = STAGE_WAKEUP;
		}
		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_WAKEUP )
	{
		trigger_wakeup_targets();
		sight_enemy();

		stateThread.SetState( "wake_call_constructor" );

		isAwake = true;

		// allow him to see after he's woken up
		ignore_sight = false;

		// ignore the flashlight from now on
		Event_WakeOnFlashlight( false );

		return SRESULT_DONE;
	}

	return SRESULT_ERROR;
}


/*
================
idAI::State_WakeUp
================
*/
stateResult_t idAI::wake_on_enemy( stateParms_t* parms )
{
	idStr animname;
	idEntity* path;

	if( parms->stage == 0 )
	{
		if( !GetIntKey( "attack_path" ) )
		{
			path = idPathCorner::RandomPath( this, NULL );
			if( path )
			{
				idle_followPathEntities( path );
			}
		}

		if( !GetEnemy() && !AI_ACTIVATED && !AI_PAIN )
		{
			// sit in our idle anim till we're activated
			Event_AllowMovement( false );
			animname = GetKey( "anim" );
			PlayCustomCycle( animname, 4 );

			parms->stage = 1;
		}
		else
		{
			parms->stage = 2;
		}

		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( !AI_PAIN && !AI_ACTIVATED )
		{
			if( checkForEnemy( true ) )
			{
				parms->stage = 2;
			}
		}
		else
		{
			parms->stage = 2;
		}

		return SRESULT_WAIT;
	}

	Event_AllowMovement( true );
	Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 4 );

	trigger_wakeup_targets();

	if( GetIntKey( "attack_path" ) )
	{
		// follow a path and fight player at end
		path = idPathCorner::RandomPath( this, NULL );
		if( path )
		{
			AI_RUN = true;
			ignoreEnemies = true;
			idle_followPathEntities( path );
			ignoreEnemies = false;
		}
	}
	else
	{
		sight_enemy();
	}

	stateThread.SetState( "wake_call_constructor" );

	isAwake = true;

	// allow him to see after he's woken up
	ignore_sight = false;

	// ignore the flashlight from now on
	Event_WakeOnFlashlight( false );

	return SRESULT_DONE;
}
/*
================
idAI::wake_call_constructor
================
*/
stateResult_t idAI::wake_call_constructor( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		//if (AnimDone(ANIMCHANNEL_TORSO, 0))
		{
			parms->stage = 1;
		}
		return SRESULT_WAIT;
	}

	CallConstructor();
	return SRESULT_DONE;
}

/*
================
idAI::wake_on_attackcone
================
*/
stateResult_t idAI::wake_on_attackcone( stateParms_t* parms )
{
	idStr animname;
	idEntity* path;
	idEntity* enemy;

	enum
	{
		STAGE_INIT = 0,
		STAGE_FIND_ENEMY_COMBAT_NODES,
		STAGE_CHECK_ENEMY,
		STAGE_WAIT_ENEMY,
		STAGE_WAKEUP
	};

	if( parms->stage == STAGE_INIT )
	{
		if( !GetIntKey( "attack_path" ) )
		{
			path = idPathCorner::RandomPath( this, NULL );
			if( path )
			{
				idle_followPathEntities( path );
				AI_RUN = path->GetIntKey( "run" );
				parms->stage = STAGE_FIND_ENEMY_COMBAT_NODES;
			}
		}
		else
		{
			parms->stage = STAGE_CHECK_ENEMY;
		}
		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_FIND_ENEMY_COMBAT_NODES )
	{
		if( !AI_MOVE_DONE && !AI_ACTIVATED && !AI_PAIN )
		{
			enemy = FindEnemyInCombatNodes();
			if( enemy )
			{
				Event_SetEnemy( enemy );
				parms->stage = STAGE_CHECK_ENEMY;
			}
		}
		else
		{
			parms->stage = STAGE_CHECK_ENEMY;
		}
		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_CHECK_ENEMY )
	{
		if( !GetEnemy() && !AI_ACTIVATED && !AI_PAIN )
		{
			// sit in our idle anim till we're activated
			Event_AllowMovement( false );

			animname = GetKey( "anim" );
			PlayCustomCycle( animname, 4 );

			parms->stage = STAGE_WAIT_ENEMY;
		}
		else
		{
			parms->stage = STAGE_WAKEUP;
		}
		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_WAIT_ENEMY )
	{
		if( !AI_ACTIVATED && !AI_PAIN )
		{
			enemy = FindEnemyInCombatNodes();
			if( enemy )
			{
				Event_SetEnemy( enemy );
				parms->stage = STAGE_WAKEUP;
				Event_AllowMovement( true );
				Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Idle", 4 );
				parms->stage = STAGE_WAKEUP;
			}
		}
		else
		{
			parms->stage = STAGE_WAKEUP;
		}
		return SRESULT_WAIT;
	}

	if( parms->stage == STAGE_WAKEUP )
	{
		trigger_wakeup_targets();

		if( GetIntKey( "attack_path" ) )
		{
			// follow a path and fight player at end
			path = idPathCorner::RandomPath( this, NULL );
			if( path )
			{
				AI_RUN = true;
				ignoreEnemies = true;
				idle_followPathEntities( path );
				ignoreEnemies = false;
			}
		}
		else
		{
			sight_enemy();
		}

		stateThread.SetState( "wake_call_constructor" );

		isAwake = true;

		// allow him to see after he's woken up
		ignore_sight = false;

		// ignore the flashlight from now on
		Event_WakeOnFlashlight( false );

		return SRESULT_DONE;
	}

	return SRESULT_ERROR;
}

/*
==================
idAI::state_Killed
==================
*/
stateResult_t idAI::state_Killed( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		Event_StopMove();

		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Death", 0 );
		Event_AnimState( ANIMCHANNEL_LEGS, "Legs_Death", 0 );

		SetWaitState( "dead" );
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( waitState != "" )
	{
		return SRESULT_WAIT;
	}

	SetState( "state_Dead" );
	return SRESULT_DONE;
}

/*
=====================
idAI::state_Dead
=====================
*/
stateResult_t idAI::state_Dead( stateParms_t* parms )
{
	//if (parms->stage == 0)
	//{
	//	float burnDelay = GetFloatKey("burnaway");
	//	if (burnDelay != 0) {
	//		Event_PreBurn();
	//		parms->stage = 1;
	//		parms->Wait(burnDelay);
	//		return SRESULT_WAIT;
	//	}
	//	parms->stage = 2;
	//	return SRESULT_WAIT;
	//}
	//
	//if (parms->stage == 1)
	//{
	//	Event_Burn();
	//	Event_StartSound("snd_burn", SND_CHANNEL_BODY, false);
	//	parms->stage = 2;
	//	return SRESULT_WAIT;
	//}
	//
	//if (parms->stage == 2)
	//{
	//	parms->Wait(30);
	//	parms->stage = 3;
	//	return SRESULT_WAIT;
	//}

	//if (resurrect) {
	//	hide();
	//	stopRagdoll();
	//	restorePosition();
	//
	//	// wait until we're resurrected
	//	waitUntil(0);
	//}
	//gameLocal.DelayRemoveEntity(this, 0);
	return SRESULT_DONE;
}
