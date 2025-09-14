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
idAI::idAI
=====================
*/
idAI::idAI()
{
	aas					= NULL;
	travelFlags			= TFL_WALK | TFL_AIR;

	kickForce			= 2048.0f;
	ignore_obstacles	= false;
	blockedRadius		= 0.0f;
	blockedMoveTime		= 750;
	blockedAttackTime	= 750;
	turnRate			= 360.0f;
	turnVel				= 0.0f;
	anim_turn_yaw		= 0.0f;
	anim_turn_amount	= 0.0f;
	anim_turn_angles	= 0.0f;
	fly_offset			= 0;
	fly_seek_scale		= 1.0f;
	fly_roll_scale		= 0.0f;
	fly_roll_max		= 0.0f;
	fly_roll			= 0.0f;
	fly_pitch_scale		= 0.0f;
	fly_pitch_max		= 0.0f;
	fly_pitch			= 0.0f;
	allowMove			= false;
	allowHiddenMovement	= false;
	fly_speed			= 0.0f;
	fly_bob_strength	= 0.0f;
	fly_bob_vert		= 0.0f;
	fly_bob_horz		= 0.0f;
	lastHitCheckResult	= false;
	lastHitCheckTime	= 0;
	lastAttackTime		= 0;
	melee_range			= 0.0f;
	projectile_height_to_distance_ratio = 1.0f;
	projectileDef		= NULL;
	projectile			= NULL;
	projectileClipModel	= NULL;
	projectileRadius	= 0.0f;
	projectileVelocity	= vec3_origin;
	projectileGravity	= vec3_origin;
	projectileSpeed		= 0.0f;
	chat_snd			= NULL;
	chat_min			= 0;
	chat_max			= 0;
	chat_time			= 0;
	talk_state			= TALK_NEVER;
	talkTarget			= NULL;

	particles.Clear();
	restartParticles	= true;
	useBoneAxis			= false;

	wakeOnFlashlight	= false;
	memset( &worldMuzzleFlash, 0, sizeof( worldMuzzleFlash ) );
	worldMuzzleFlashHandle = -1;

	enemy				= NULL;
	lastVisibleEnemyPos.Zero();
	lastVisibleEnemyEyeOffset.Zero();
	lastVisibleReachableEnemyPos.Zero();
	lastReachableEnemyPos.Zero();
	fl.neverDormant		= false;		// AI's can go dormant
	current_yaw			= 0.0f;
	ideal_yaw			= 0.0f;

	spawnClearMoveables	= false;
	harvestEnt			= NULL;

	num_cinematics		= 0;
	current_cinematic	= 0;

	allowEyeFocus		= true;
	allowPain			= true;
	allowJointMod		= true;
	focusEntity			= NULL;
	focusTime			= 0;
	alignHeadTime		= 0;
	forceAlignHeadTime	= 0;

	currentFocusPos.Zero();
	eyeAng.Zero();
	lookAng.Zero();
	destLookAng.Zero();
	lookMin.Zero();
	lookMax.Zero();

	eyeMin.Zero();
	eyeMax.Zero();
	muzzleFlashEnd		= 0;
	flashTime			= 0;
	flashJointWorld		= INVALID_JOINT;

	focusJoint			= INVALID_JOINT;
	orientationJoint	= INVALID_JOINT;
	flyTiltJoint		= INVALID_JOINT;

	eyeVerticalOffset	= 0.0f;
	eyeHorizontalOffset = 0.0f;
	eyeFocusRate		= 0.0f;
	headFocusRate		= 0.0f;
	focusAlignTime		= 0;
}

/*
=====================
idAI::~idAI
=====================
*/
idAI::~idAI()
{
	delete projectileClipModel;
	DeconstructScriptObject();
	scriptObject.Free();
	if( worldMuzzleFlashHandle != -1 )
	{
		gameRenderWorld->FreeLightDef( worldMuzzleFlashHandle );
		worldMuzzleFlashHandle = -1;
	}

	if( harvestEnt.GetEntity() )
	{
		harvestEnt.GetEntity()->PostEventMS( &EV_Remove, 0 );
	}
}

/*
=====================
idAI::Save
=====================
*/
void idAI::Save( idSaveGame* savefile ) const
{
	savefile->WriteInt( travelFlags );
	move.Save( savefile );
	savedMove.Save( savefile );
	savefile->WriteFloat( kickForce );
	savefile->WriteBool( ignore_obstacles );
	savefile->WriteFloat( blockedRadius );
	savefile->WriteInt( blockedMoveTime );
	savefile->WriteInt( blockedAttackTime );

	savefile->WriteFloat( ideal_yaw );
	savefile->WriteFloat( current_yaw );
	savefile->WriteFloat( turnRate );
	savefile->WriteFloat( turnVel );
	savefile->WriteFloat( anim_turn_yaw );
	savefile->WriteFloat( anim_turn_amount );
	savefile->WriteFloat( anim_turn_angles );

	savefile->WriteStaticObject( physicsObj );

	savefile->WriteFloat( fly_speed );
	savefile->WriteFloat( fly_bob_strength );
	savefile->WriteFloat( fly_bob_vert );
	savefile->WriteFloat( fly_bob_horz );
	savefile->WriteInt( fly_offset );
	savefile->WriteFloat( fly_seek_scale );
	savefile->WriteFloat( fly_roll_scale );
	savefile->WriteFloat( fly_roll_max );
	savefile->WriteFloat( fly_roll );
	savefile->WriteFloat( fly_pitch_scale );
	savefile->WriteFloat( fly_pitch_max );
	savefile->WriteFloat( fly_pitch );

	savefile->WriteBool( allowMove );
	savefile->WriteBool( allowHiddenMovement );
	savefile->WriteBool( disableGravity );
	savefile->WriteBool( af_push_moveables );

	savefile->WriteBool( lastHitCheckResult );
	savefile->WriteInt( lastHitCheckTime );
	savefile->WriteInt( lastAttackTime );
	savefile->WriteFloat( melee_range );
	savefile->WriteFloat( projectile_height_to_distance_ratio );

	savefile->WriteInt( missileLaunchOffset.Num() );
	for( int i = 0; i < missileLaunchOffset.Num(); i++ )
	{
		savefile->WriteVec3( missileLaunchOffset[ i ] );
	}

	idStr projectileName;
	spawnArgs.GetString( "def_projectile", "", projectileName );
	savefile->WriteString( projectileName );
	savefile->WriteFloat( projectileRadius );
	savefile->WriteFloat( projectileSpeed );
	savefile->WriteVec3( projectileVelocity );
	savefile->WriteVec3( projectileGravity );
	projectile.Save( savefile );
	savefile->WriteString( attack );

	savefile->WriteSoundShader( chat_snd );
	savefile->WriteInt( chat_min );
	savefile->WriteInt( chat_max );
	savefile->WriteInt( chat_time );
	savefile->WriteInt( talk_state );
	talkTarget.Save( savefile );

	savefile->WriteInt( num_cinematics );
	savefile->WriteInt( current_cinematic );

	savefile->WriteBool( allowJointMod );
	focusEntity.Save( savefile );
	savefile->WriteVec3( currentFocusPos );
	savefile->WriteInt( focusTime );
	savefile->WriteInt( alignHeadTime );
	savefile->WriteInt( forceAlignHeadTime );
	savefile->WriteAngles( eyeAng );
	savefile->WriteAngles( lookAng );
	savefile->WriteAngles( destLookAng );
	savefile->WriteAngles( lookMin );
	savefile->WriteAngles( lookMax );

	savefile->WriteInt( lookJoints.Num() );
	for( int i = 0; i < lookJoints.Num(); i++ )
	{
		savefile->WriteJoint( lookJoints[ i ] );
		savefile->WriteAngles( lookJointAngles[ i ] );
	}

	savefile->WriteInt( particles.Num() );
	for( int i = 0; i < particles.Num(); i++ )
	{
		savefile->WriteParticle( particles[i].particle );
		savefile->WriteInt( particles[i].time );
		savefile->WriteJoint( particles[i].joint );
	}
	savefile->WriteBool( restartParticles );
	savefile->WriteBool( useBoneAxis );

	enemy.Save( savefile );
	savefile->WriteVec3( lastVisibleEnemyPos );
	savefile->WriteVec3( lastVisibleEnemyEyeOffset );
	savefile->WriteVec3( lastVisibleReachableEnemyPos );
	savefile->WriteVec3( lastReachableEnemyPos );
	savefile->WriteBool( wakeOnFlashlight );

	savefile->WriteAngles( eyeMin );
	savefile->WriteAngles( eyeMax );

	savefile->WriteFloat( eyeVerticalOffset );
	savefile->WriteFloat( eyeHorizontalOffset );
	savefile->WriteFloat( eyeFocusRate );
	savefile->WriteFloat( headFocusRate );
	savefile->WriteInt( focusAlignTime );

	savefile->WriteJoint( flashJointWorld );
	savefile->WriteInt( muzzleFlashEnd );

	savefile->WriteJoint( focusJoint );
	savefile->WriteJoint( orientationJoint );
	savefile->WriteJoint( flyTiltJoint );

	savefile->WriteBool( GetPhysics() == static_cast<const idPhysics*>( &physicsObj ) );

	savefile->WriteInt( funcEmitters.Num() );
	for( int i = 0; i < funcEmitters.Num(); i++ )
	{
		funcEmitter_t* emitter = funcEmitters.GetIndex( i );
		savefile->WriteString( emitter->name );
		savefile->WriteJoint( emitter->joint );
		savefile->WriteObject( emitter->particle );
	}

	harvestEnt.Save( savefile );
}

// jmarshall begin
/*
=====================
idAI::combat_lost
=====================
*/
void idAI::combat_lost()
{
	//if (!ignore_lostcombat) {
	SetState( "state_LostCombat" );
	//}
}


/*
=====================
idAI::GetCombatNode
=====================
*/
idEntity* idAI::GetCombatNode()
{
	int				i;
	float			dist;
	idEntity* targetEnt;
	idCombatNode* node;
	float			bestDist;
	idCombatNode* bestNode;
	idActor* enemyEnt = enemy.GetEntity();

	if( !targets.Num() )
	{
		// no combat nodes
		return ( NULL );
	}

	if( !enemyEnt || !EnemyPositionValid() )
	{
		// don't return a combat node if we don't have an enemy or
		// if we can see he's not in the last place we saw him

		if( team == 0 )
		{
			// find the closest attack node to the player
			bestNode = NULL;
			const idVec3& myPos = physicsObj.GetOrigin();
			const idVec3& playerPos = gameLocal.GetLocalPlayer()->GetPhysics()->GetOrigin();

			bestDist = ( myPos - playerPos ).LengthSqr();

			for( i = 0; i < targets.Num(); i++ )
			{
				targetEnt = targets[i].GetEntity();
				if( !targetEnt || !targetEnt->IsType( idCombatNode::Type ) )
				{
					continue;
				}

				node = static_cast<idCombatNode*>( targetEnt );
				if( !node->IsDisabled() )
				{
					idVec3 org = node->GetPhysics()->GetOrigin();
					dist = ( playerPos - org ).LengthSqr();
					if( dist < bestDist )
					{
						bestNode = node;
						bestDist = dist;
					}
				}
			}

			return bestNode;
		}

		return NULL;
	}

	// find the closest attack node that can see our enemy and is closer than our enemy
	bestNode = NULL;
	const idVec3& myPos = physicsObj.GetOrigin();
	bestDist = ( myPos - lastVisibleEnemyPos ).LengthSqr();
	for( i = 0; i < targets.Num(); i++ )
	{
		targetEnt = targets[i].GetEntity();
		if( !targetEnt || !targetEnt->IsType( idCombatNode::Type ) )
		{
			continue;
		}

		node = static_cast<idCombatNode*>( targetEnt );
		if( !node->IsDisabled() && node->EntityInView( enemyEnt, lastVisibleEnemyPos ) )
		{
			idVec3 org = node->GetPhysics()->GetOrigin();
			dist = ( myPos - org ).LengthSqr();
			if( dist < bestDist )
			{
				bestNode = node;
				bestDist = dist;
			}
		}
	}

	return bestNode;
}

/*
=====================
idAI::TestAnimMove
=====================
*/
bool idAI::TestAnimMove( const char* animname )
{
	int				anim;
	predictedPath_t path;
	idVec3			moveVec;

	anim = GetAnim( ANIMCHANNEL_LEGS, animname );
	if( !anim )
	{
		gameLocal.DWarning( "missing '%s' animation on '%s' (%s)", animname, name.c_str(), GetEntityDefName() );
		return false;
	}

	moveVec = animator.TotalMovementDelta( anim ) * idAngles( 0.0f, ideal_yaw, 0.0f ).ToMat3() * physicsObj.GetGravityAxis();
	idAI::PredictPath( this, aas, physicsObj.GetOrigin(), moveVec, 1000, 1000, ( move.moveType == MOVETYPE_FLY ) ? SE_BLOCKED : ( SE_ENTER_OBSTACLE | SE_BLOCKED | SE_ENTER_LEDGE_AREA ), path );

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugLine( colorGreen, physicsObj.GetOrigin(), physicsObj.GetOrigin() + moveVec, 1 );
		gameRenderWorld->DebugBounds( path.endEvent == 0 ? colorYellow : colorRed, physicsObj.GetBounds(), physicsObj.GetOrigin() + moveVec, 1 );
	}

	return path.endEvent == 0;
}
// jmarshall end

/*
=====================
idAI::Restore
=====================
*/
void idAI::Restore( idRestoreGame* savefile )
{
	bool		restorePhysics;
	int			num;
	idBounds	bounds;

	savefile->ReadInt( travelFlags );
	move.Restore( savefile );
	savedMove.Restore( savefile );
	savefile->ReadFloat( kickForce );
	savefile->ReadBool( ignore_obstacles );
	savefile->ReadFloat( blockedRadius );
	savefile->ReadInt( blockedMoveTime );
	savefile->ReadInt( blockedAttackTime );

	savefile->ReadFloat( ideal_yaw );
	savefile->ReadFloat( current_yaw );
	savefile->ReadFloat( turnRate );
	savefile->ReadFloat( turnVel );
	savefile->ReadFloat( anim_turn_yaw );
	savefile->ReadFloat( anim_turn_amount );
	savefile->ReadFloat( anim_turn_angles );

	savefile->ReadStaticObject( physicsObj );

	savefile->ReadFloat( fly_speed );
	savefile->ReadFloat( fly_bob_strength );
	savefile->ReadFloat( fly_bob_vert );
	savefile->ReadFloat( fly_bob_horz );
	savefile->ReadInt( fly_offset );
	savefile->ReadFloat( fly_seek_scale );
	savefile->ReadFloat( fly_roll_scale );
	savefile->ReadFloat( fly_roll_max );
	savefile->ReadFloat( fly_roll );
	savefile->ReadFloat( fly_pitch_scale );
	savefile->ReadFloat( fly_pitch_max );
	savefile->ReadFloat( fly_pitch );

	savefile->ReadBool( allowMove );
	savefile->ReadBool( allowHiddenMovement );
	savefile->ReadBool( disableGravity );
	savefile->ReadBool( af_push_moveables );

	savefile->ReadBool( lastHitCheckResult );
	savefile->ReadInt( lastHitCheckTime );
	savefile->ReadInt( lastAttackTime );
	savefile->ReadFloat( melee_range );
	savefile->ReadFloat( projectile_height_to_distance_ratio );

	savefile->ReadInt( num );
	missileLaunchOffset.SetGranularity( 1 );
	missileLaunchOffset.SetNum( num );
	for( int i = 0; i < num; i++ )
	{
		savefile->ReadVec3( missileLaunchOffset[ i ] );
	}

	idStr projectileName;
	savefile->ReadString( projectileName );
	if( projectileName.Length() )
	{
		projectileDef = gameLocal.FindEntityDefDict( projectileName );
	}
	else
	{
		projectileDef = NULL;
	}
	savefile->ReadFloat( projectileRadius );
	savefile->ReadFloat( projectileSpeed );
	savefile->ReadVec3( projectileVelocity );
	savefile->ReadVec3( projectileGravity );
	projectile.Restore( savefile );
	savefile->ReadString( attack );

	savefile->ReadSoundShader( chat_snd );
	savefile->ReadInt( chat_min );
	savefile->ReadInt( chat_max );
	savefile->ReadInt( chat_time );
	int tmp;
	savefile->ReadInt( tmp );
	talk_state = static_cast<talkState_t>( tmp );
	talkTarget.Restore( savefile );

	savefile->ReadInt( num_cinematics );
	savefile->ReadInt( current_cinematic );

	savefile->ReadBool( allowJointMod );
	focusEntity.Restore( savefile );
	savefile->ReadVec3( currentFocusPos );
	savefile->ReadInt( focusTime );
	savefile->ReadInt( alignHeadTime );
	savefile->ReadInt( forceAlignHeadTime );
	savefile->ReadAngles( eyeAng );
	savefile->ReadAngles( lookAng );
	savefile->ReadAngles( destLookAng );
	savefile->ReadAngles( lookMin );
	savefile->ReadAngles( lookMax );

	savefile->ReadInt( num );
	lookJoints.SetGranularity( 1 );
	lookJoints.SetNum( num );
	lookJointAngles.SetGranularity( 1 );
	lookJointAngles.SetNum( num );
	for( int i = 0; i < num; i++ )
	{
		savefile->ReadJoint( lookJoints[ i ] );
		savefile->ReadAngles( lookJointAngles[ i ] );
	}

	savefile->ReadInt( num );
	particles.SetNum( num );
	for( int i = 0; i < particles.Num(); i++ )
	{
		savefile->ReadParticle( particles[i].particle );
		savefile->ReadInt( particles[i].time );
		savefile->ReadJoint( particles[i].joint );
	}
	savefile->ReadBool( restartParticles );
	savefile->ReadBool( useBoneAxis );

	enemy.Restore( savefile );
	savefile->ReadVec3( lastVisibleEnemyPos );
	savefile->ReadVec3( lastVisibleEnemyEyeOffset );
	savefile->ReadVec3( lastVisibleReachableEnemyPos );
	savefile->ReadVec3( lastReachableEnemyPos );

	savefile->ReadBool( wakeOnFlashlight );

	savefile->ReadAngles( eyeMin );
	savefile->ReadAngles( eyeMax );

	savefile->ReadFloat( eyeVerticalOffset );
	savefile->ReadFloat( eyeHorizontalOffset );
	savefile->ReadFloat( eyeFocusRate );
	savefile->ReadFloat( headFocusRate );
	savefile->ReadInt( focusAlignTime );

	savefile->ReadJoint( flashJointWorld );
	savefile->ReadInt( muzzleFlashEnd );

	savefile->ReadJoint( focusJoint );
	savefile->ReadJoint( orientationJoint );
	savefile->ReadJoint( flyTiltJoint );

	savefile->ReadBool( restorePhysics );

	// Set the AAS if the character has the correct gravity vector
	idVec3 gravity = spawnArgs.GetVector( "gravityDir", "0 0 -1" );
	gravity *= g_gravity.GetFloat();
	if( gravity == gameLocal.GetGravity() )
	{
		SetAAS();
	}

	SetCombatModel();
	LinkCombat();

	InitMuzzleFlash();

	// Link the script variables back to the scriptobject
	LinkScriptVariables();

	if( restorePhysics )
	{
		RestorePhysics( &physicsObj );
	}


	//Clean up the emitters
	for( int i = 0; i < funcEmitters.Num(); i++ )
	{
		funcEmitter_t* emitter = funcEmitters.GetIndex( i );
		if( emitter->particle )
		{
			//Destroy the emitters
			emitter->particle->PostEventMS( &EV_Remove, 0 );
		}
	}
	funcEmitters.Clear();

	int emitterCount;
	savefile->ReadInt( emitterCount );
	for( int i = 0; i < emitterCount; i++ )
	{
		funcEmitter_t newEmitter;
		memset( &newEmitter, 0, sizeof( newEmitter ) );

		idStr name;
		savefile->ReadString( name );

		strcpy( newEmitter.name, name.c_str() );

		savefile->ReadJoint( newEmitter.joint );
		savefile->ReadObject( reinterpret_cast<idClass*&>( newEmitter.particle ) );

		funcEmitters.Set( newEmitter.name, newEmitter );
	}

	harvestEnt.Restore( savefile );
	//if(harvestEnt.GetEntity()) {
	//	harvestEnt.GetEntity()->SetParent(this);
	//}

}

// jmarshall begin
/*
=====================
idAI::FindEnemyInCombatNodes
=====================
*/
idEntity* idAI::FindEnemyInCombatNodes()
{
	int				i, j;
	idCombatNode* node;
	idEntity* ent;
	idEntity* targetEnt;
	idActor* actor;

	if( !gameLocal.InPlayerPVS( this ) )
	{
		// don't locate the player when we're not in his PVS
		return NULL;
	}

	for( i = 0; i < gameLocal.numClients; i++ )
	{
		ent = gameLocal.entities[i];

		if( !ent || !ent->IsType( idActor::Type ) )
		{
			continue;
		}

		actor = static_cast<idActor*>( ent );
		if( ( actor->health <= 0 ) || !( ReactionTo( actor ) & ATTACK_ON_SIGHT ) )
		{
			continue;
		}

		for( j = 0; j < targets.Num(); j++ )
		{
			targetEnt = targets[j].GetEntity();
			if( !targetEnt || !targetEnt->IsType( idCombatNode::Type ) )
			{
				continue;
			}

			node = static_cast<idCombatNode*>( targetEnt );
			if( !node->IsDisabled() && node->EntityInView( actor, actor->GetPhysics()->GetOrigin() ) )
			{
				return actor;
			}
		}
	}

	return NULL;
}
// jmarshall end

/*
=====================
idAI::Spawn
=====================
*/
void idAI::Spawn()
{
	const char*			jointname;
	const idKeyValue*	kv;
	idStr				jointName;
	idAngles			jointScale;
	jointHandle_t		joint;
	idVec3				local_dir;
	bool				talks;
	float				teleportType; // jmarshall
	idStr				triggerAnim; // jmarshall

	if( !g_monsters.GetBool() )
	{
		PostEventMS( &EV_Remove, 0 );
		return;
	}

	spawnArgs.GetInt(	"team",					"1",		team );
	spawnArgs.GetInt(	"rank",					"0",		rank );
	spawnArgs.GetInt(	"fly_offset",			"0",		fly_offset );
	spawnArgs.GetFloat( "fly_speed",			"100",		fly_speed );
	spawnArgs.GetFloat( "fly_bob_strength",		"50",		fly_bob_strength );
	spawnArgs.GetFloat( "fly_bob_vert",			"2",		fly_bob_horz );
	spawnArgs.GetFloat( "fly_bob_horz",			"2.7",		fly_bob_vert );
	spawnArgs.GetFloat( "fly_seek_scale",		"4",		fly_seek_scale );
	spawnArgs.GetFloat( "fly_roll_scale",		"90",		fly_roll_scale );
	spawnArgs.GetFloat( "fly_roll_max",			"60",		fly_roll_max );
	spawnArgs.GetFloat( "fly_pitch_scale",		"45",		fly_pitch_scale );
	spawnArgs.GetFloat( "fly_pitch_max",		"30",		fly_pitch_max );

	spawnArgs.GetFloat( "melee_range",			"64",		melee_range );
	spawnArgs.GetFloat( "projectile_height_to_distance_ratio",	"1", projectile_height_to_distance_ratio );

	spawnArgs.GetFloat( "turn_rate",			"360",		turnRate );

	spawnArgs.GetBool( "talks",					"0",		talks );
	if( spawnArgs.GetString( "npc_name", NULL ) != NULL )
	{
		if( talks )
		{
			talk_state = TALK_OK;
		}
		else
		{
			talk_state = TALK_BUSY;
		}
	}
	else
	{
		talk_state = TALK_NEVER;
	}

	spawnArgs.GetBool( "animate_z",				"0",		disableGravity );
	spawnArgs.GetBool( "af_push_moveables",		"0",		af_push_moveables );
	spawnArgs.GetFloat( "kick_force",			"4096",		kickForce );
	spawnArgs.GetBool( "ignore_obstacles",		"0",		ignore_obstacles );
	spawnArgs.GetFloat( "blockedRadius",		"-1",		blockedRadius );
	spawnArgs.GetInt( "blockedMoveTime",		"750",		blockedMoveTime );
	spawnArgs.GetInt( "blockedAttackTime",		"750",		blockedAttackTime );

	spawnArgs.GetInt(	"num_cinematics",		"0",		num_cinematics );
	current_cinematic = 0;

	LinkScriptVariables();

	fl.takedamage		= !spawnArgs.GetBool( "noDamage" );
	enemy				= NULL;
	allowMove			= true;
	allowHiddenMovement = false;

	animator.RemoveOriginOffset( true );

	// create combat collision hull for exact collision detection
	SetCombatModel();

	lookMin	= spawnArgs.GetAngles( "look_min", "-80 -75 0" );
	lookMax	= spawnArgs.GetAngles( "look_max", "80 75 0" );

	lookJoints.SetGranularity( 1 );
	lookJointAngles.SetGranularity( 1 );
	kv = spawnArgs.MatchPrefix( "look_joint", NULL );
	while( kv )
	{
		jointName = kv->GetKey();

		// RB: TrenchBroom interop use look_joint.<name> instead so we can build this up using the FGD files
		if( !jointName.StripLeadingOnce( "look_joint " ) )
		{
			jointName.StripLeadingOnce( "look_joint." );
		}
		// RB end

		joint = animator.GetJointHandle( jointName );
		if( joint == INVALID_JOINT )
		{
			gameLocal.Warning( "Unknown look_joint '%s' on entity %s", jointName.c_str(), name.c_str() );
		}
		else
		{
			jointScale = spawnArgs.GetAngles( kv->GetKey(), "0 0 0" );
			jointScale.roll = 0.0f;

			// if no scale on any component, then don't bother adding it.  this may be done to
			// zero out rotation from an inherited entitydef.
			if( jointScale != ang_zero )
			{
				lookJoints.Append( joint );
				lookJointAngles.Append( jointScale );
			}
		}
		kv = spawnArgs.MatchPrefix( "look_joint", kv );
	}

	// calculate joint positions on attack frames so we can do proper "can hit" tests
	CalculateAttackOffsets();

	eyeMin				= spawnArgs.GetAngles( "eye_turn_min", "-10 -30 0" );
	eyeMax				= spawnArgs.GetAngles( "eye_turn_max", "10 30 0" );
	eyeVerticalOffset	= spawnArgs.GetFloat( "eye_verticle_offset", "5" );
	eyeHorizontalOffset = spawnArgs.GetFloat( "eye_horizontal_offset", "-8" );
	eyeFocusRate		= spawnArgs.GetFloat( "eye_focus_rate", "0.5" );
	headFocusRate		= spawnArgs.GetFloat( "head_focus_rate", "0.1" );
	focusAlignTime		= SEC2MS( spawnArgs.GetFloat( "focus_align_time", "1" ) );

	flashJointWorld = animator.GetJointHandle( "flash" );

	if( head.GetEntity() )
	{
		idAnimator* headAnimator = head.GetEntity()->GetAnimator();

		jointname = spawnArgs.GetString( "bone_focus" );
		if( *jointname )
		{
			focusJoint = headAnimator->GetJointHandle( jointname );
			if( focusJoint == INVALID_JOINT )
			{
				gameLocal.Warning( "Joint '%s' not found on head on '%s'", jointname, name.c_str() );
			}
		}
	}
	else
	{
		jointname = spawnArgs.GetString( "bone_focus" );
		if( *jointname )
		{
			focusJoint = animator.GetJointHandle( jointname );
			if( focusJoint == INVALID_JOINT )
			{
				gameLocal.Warning( "Joint '%s' not found on '%s'", jointname, name.c_str() );
			}
		}
	}

	jointname = spawnArgs.GetString( "bone_orientation" );
	if( *jointname )
	{
		orientationJoint = animator.GetJointHandle( jointname );
		if( orientationJoint == INVALID_JOINT )
		{
			gameLocal.Warning( "Joint '%s' not found on '%s'", jointname, name.c_str() );
		}
	}

	jointname = spawnArgs.GetString( "bone_flytilt" );
	if( *jointname )
	{
		flyTiltJoint = animator.GetJointHandle( jointname );
		if( flyTiltJoint == INVALID_JOINT )
		{
			gameLocal.Warning( "Joint '%s' not found on '%s'", jointname, name.c_str() );
		}
	}

	InitMuzzleFlash();

	physicsObj.SetSelf( this );
	physicsObj.SetClipModel( new( TAG_PHYSICS_CLIP_ENTITY ) idClipModel( GetPhysics()->GetClipModel() ), 1.0f );
	physicsObj.SetMass( spawnArgs.GetFloat( "mass", "100" ) );

	if( spawnArgs.GetBool( "big_monster" ) )
	{
		physicsObj.SetContents( 0 );
		physicsObj.SetClipMask( MASK_MONSTERSOLID & ~CONTENTS_BODY );
	}
	else
	{
		if( use_combat_bbox )
		{
			physicsObj.SetContents( CONTENTS_BODY | CONTENTS_SOLID );
		}
		else
		{
			physicsObj.SetContents( CONTENTS_BODY );
		}
		physicsObj.SetClipMask( MASK_MONSTERSOLID );
	}

	// move up to make sure the monster is at least an epsilon above the floor
	physicsObj.SetOrigin( GetPhysics()->GetOrigin() + idVec3( 0, 0, CM_CLIP_EPSILON ) );

	if( num_cinematics )
	{
		physicsObj.SetGravity( vec3_origin );
	}
	else
	{
		idVec3 gravity = spawnArgs.GetVector( "gravityDir", "0 0 -1" );
		gravity *= g_gravity.GetFloat();
		physicsObj.SetGravity( gravity );
	}

	SetPhysics( &physicsObj );

	physicsObj.GetGravityAxis().ProjectVector( viewAxis[ 0 ], local_dir );
	current_yaw		= local_dir.ToYaw();
	ideal_yaw		= idMath::AngleNormalize180( current_yaw );

	move.blockTime = 0;

	SetAAS();

	projectile		= NULL;
	projectileDef	= NULL;
	projectileClipModel	= NULL;
	idStr projectileName;
	if( spawnArgs.GetString( "def_projectile", "", projectileName ) && projectileName.Length() )
	{
		projectileDef = gameLocal.FindEntityDefDict( projectileName );
		CreateProjectile( vec3_origin, viewAxis[ 0 ] );
		projectileRadius	= projectile.GetEntity()->GetPhysics()->GetClipModel()->GetBounds().GetRadius();
		projectileVelocity	= idProjectile::GetVelocity( projectileDef );
		projectileGravity	= idProjectile::GetGravity( projectileDef );
		projectileSpeed		= projectileVelocity.Length();
		delete projectile.GetEntity();
		projectile = NULL;
	}

	particles.Clear();
	restartParticles = true;
	useBoneAxis = spawnArgs.GetBool( "useBoneAxis" );
	SpawnParticles( "smokeParticleSystem" );

	if( num_cinematics || spawnArgs.GetBool( "hide" ) || spawnArgs.GetBool( "teleport" ) || spawnArgs.GetBool( "trigger_anim" ) )
	{
		fl.takedamage = false;
		physicsObj.SetContents( 0 );
		physicsObj.GetClipModel()->Unlink();
		Hide();
	}
	else
	{
		// play a looping ambient sound if we have one
		StartSound( "snd_ambient", SND_CHANNEL_AMBIENT, 0, false, NULL );
	}

	if( health <= 0 )
	{
		gameLocal.Warning( "entity '%s' doesn't have health set", name.c_str() );
		health = 1;
	}

	// set up monster chatter
	SetChatSound();

	BecomeActive( TH_THINK );

	if( af_push_moveables )
	{
		af.SetupPose( this, gameLocal.time );
		af.GetPhysics()->EnableClip();
	}

	// init the move variables
	StopMove( MOVE_STATUS_DONE );

// jmarshall begin

	// Only AI that derives off of ai_monster_base can support native AI.
	supportsNative = scriptObject.GetFunction( "supports_native" ) != NULL;

	if( supportsNative )
	{
		stateThread.SetOwner( this );

		Init();

		isAwake = false;

		teleportType = GetIntKey( "teleport" );
		triggerAnim = GetKey( "trigger_anim" );

		if( !GetIntKey( "ignore_flashlight" ) )
		{
			// allow waking up from the flashlight
			Event_WakeOnFlashlight( true );
		}

		// The original script does not set state but extends init over multiple frames.
		// to fix the spawner, state_Spawner is exclusive for now.
		if( GetIntKey( "spawner" ) )
		{
			stateThread.SetState( "state_Spawner" );
		}
		else if( triggerAnim != "" )
		{
			stateThread.SetState( "state_TriggerAnim" );
		}
		else if( teleportType > 0 )
		{
			stateThread.SetState( "state_TeleportTriggered" );
		}
		else if( GetIntKey( "hide" ) )
		{
			stateThread.SetState( "state_TriggerHidden" );
		}
		else
		{
			stateThread.SetState( "state_WakeUp" );
		}
	}
// jmarshall end

	spawnArgs.GetBool( "spawnClearMoveables", "0", spawnClearMoveables );
}


void idAI::Gib( const idVec3& dir, const char* damageDefName )
{
	if( harvestEnt.GetEntity() )
	{
		//Let the harvest ent know that we gibbed
		harvestEnt.GetEntity()->Gib();
	}
	idActor::Gib( dir, damageDefName );
}

/*
===================
idAI::InitMuzzleFlash
===================
*/
void idAI::InitMuzzleFlash()
{
	const char*			shader;
	idVec3				flashColor;

	spawnArgs.GetString( "mtr_flashShader", "muzzleflash", &shader );
	spawnArgs.GetVector( "flashColor", "0 0 0", flashColor );
	float flashRadius = spawnArgs.GetFloat( "flashRadius" );
	flashTime = SEC2MS( spawnArgs.GetFloat( "flashTime", "0.25" ) );

	memset( &worldMuzzleFlash, 0, sizeof( worldMuzzleFlash ) );

	worldMuzzleFlash.pointLight = true;
	worldMuzzleFlash.shader = declManager->FindMaterial( shader, false );
	worldMuzzleFlash.shaderParms[ SHADERPARM_RED ] = flashColor[0];
	worldMuzzleFlash.shaderParms[ SHADERPARM_GREEN ] = flashColor[1];
	worldMuzzleFlash.shaderParms[ SHADERPARM_BLUE ] = flashColor[2];
	worldMuzzleFlash.shaderParms[ SHADERPARM_ALPHA ] = 1.0f;
	worldMuzzleFlash.shaderParms[ SHADERPARM_TIMESCALE ] = 1.0f;
	worldMuzzleFlash.lightRadius[0] = flashRadius;
	worldMuzzleFlash.lightRadius[1]	= flashRadius;
	worldMuzzleFlash.lightRadius[2]	= flashRadius;

	worldMuzzleFlashHandle = -1;
}

/*
===================
idAI::List_f
===================
*/
void idAI::List_f( const idCmdArgs& args )
{
	int		e;
	idAI*	check;
	int		count;
	const char* statename;

	count = 0;

	gameLocal.Printf( "%-4s  %-20s %s\n", " Num", "EntityDef", "Name" );
	gameLocal.Printf( "------------------------------------------------\n" );
	for( e = 0; e < MAX_GENTITIES; e++ )
	{
		check = static_cast<idAI*>( gameLocal.entities[ e ] );
		if( !check || !check->IsType( idAI::Type ) )
		{
			continue;
		}

		if( check->state )
		{
			statename = check->state->Name();
		}
		else
		{
			statename = "NULL state";
		}

		gameLocal.Printf( "%4i: %-20s %-20s %s  move: %d\n", e, check->GetEntityDefName(), check->name.c_str(), statename, check->allowMove );
		count++;
	}

	gameLocal.Printf( "...%d monsters\n", count );
}

/*
================
idAI::DormantBegin

called when entity becomes dormant
================
*/
void idAI::DormantBegin()
{
	// since dormant happens on a timer, we wont get to update particles to
	// hidden through the think loop, but we need to hide them though.
	if( particles.Num() )
	{
		for( int i = 0; i < particles.Num(); i++ )
		{
			particles[i].time = 0;
		}
	}

	if( enemyNode.InList() )
	{
		// remove ourselves from the enemy's enemylist
		enemyNode.Remove();
	}
	idActor::DormantBegin();
}

/*
================
idAI::DormantEnd

called when entity wakes from being dormant
================
*/
void idAI::DormantEnd()
{
	if( enemy.GetEntity() && !enemyNode.InList() )
	{
		// let our enemy know we're back on the trail
		enemyNode.AddToEnd( enemy.GetEntity()->enemyList );
	}

	if( particles.Num() )
	{
		for( int i = 0; i < particles.Num(); i++ )
		{
			particles[i].time = gameLocal.time;
		}
	}

	idActor::DormantEnd();
}

// jmarshall begin
/*
======================
idAI::checkForEnemy
======================
*/
bool idAI::checkForEnemy( float use_fov )
{
	idEntity* enemy = NULL;
	idVec3 size;
	float dist;

	if( gameLocal.InfluenceActive() )
	{
		return false;
	}

	if( AI_PAIN )
	{
		// get out of ambush mode when shot
		ambush = false;
	}

	if( ignoreEnemies )
	{
		// while we're following paths, we only respond to enemies on pain, or when close enough to them
		if( stay_on_attackpath )
		{
			// don't exit attack_path when close to enemy
			return false;
		}

		enemy = this->enemy.GetEntity();
		if( !enemy )
		{
			enemy = FindEnemy( false );
		}

		if( !enemy )
		{
			return false;
		}

		size = GetSize();
		dist = ( size.x * 1.414 ) + 16; // diagonal distance plus 16 units
		if( EnemyRange() > dist )
		{
			return false;
		}
	}
	else
	{
		if( this->enemy.GetEntity() )
		{
			// we were probably triggered (which sets our enemy)
			return true;
		}

		if( !ignore_sight )
		{
			enemy = FindEnemy( use_fov );
		}

		if( !enemy )
		{
			if( ambush )
			{
				return false;
			}

			enemy = HeardSound( true );
			if( !enemy )
			{
				return false;
			}
		}
	}

	ignoreEnemies = false;

	// once we've woken up, get out of ambush mode
	ambush = false;

	// don't use the fov for sight anymore
	idle_sight_fov = false;

	Event_SetEnemy( enemy );
	return true;
}
// jmarshall

/*
=====================
idAI::Think
=====================
*/
idCVar ai_think( "ai_think", "1", CVAR_BOOL, "for testing.." );
void idAI::Think()
{
	// if we are completely closed off from the player, don't do anything at all
	if( CheckDormant() )
	{
		return;
	}

	if( !ai_think.GetBool() )
	{
		return;
	}

	if( thinkFlags & TH_THINK )
	{
		// jmarshall begin
		if( supportsNative )
		{
			stateThread.Execute();
		}
		// jmarshall end

		// clear out the enemy when he dies or is hidden
		if( enemy.IsValid() )
		{
			idActor* enemyEnt = enemy.GetEntity();
			if( enemyEnt )
			{
				if( enemyEnt->health <= 0 )
				{
					EnemyDead();
				}
			}
		}

		current_yaw += deltaViewAngles.yaw;
		ideal_yaw = idMath::AngleNormalize180( ideal_yaw + deltaViewAngles.yaw );
		deltaViewAngles.Zero();
		viewAxis = idAngles( 0, current_yaw, 0 ).ToMat3();

		if( num_cinematics )
		{
			if( !IsHidden() && torsoAnim.AnimDone( 0 ) )
			{
				PlayCinematic();
			}
			RunPhysics();
		}
		else if( !allowHiddenMovement && IsHidden() )
		{
			// hidden monsters
			UpdateAIScript();
		}
		else
		{
			// clear the ik before we do anything else so the skeleton doesn't get updated twice
			walkIK.ClearJointMods();

			switch( move.moveType )
			{
				case MOVETYPE_DEAD :
					// dead monsters
					UpdateAIScript();
					DeadMove();
					break;

				case MOVETYPE_FLY :
					// flying monsters
					UpdateEnemyPosition();
					UpdateAIScript();
					FlyMove();
					PlayChatter();
					CheckBlink();
					break;

				case MOVETYPE_STATIC :
					// static monsters
					UpdateEnemyPosition();
					UpdateAIScript();
					StaticMove();
					PlayChatter();
					CheckBlink();
					break;

				case MOVETYPE_ANIM :
					// animation based movement
					UpdateEnemyPosition();
					UpdateAIScript();
					AnimMove();
					PlayChatter();
					CheckBlink();
					break;

				case MOVETYPE_SLIDE :
					// velocity based movement
					UpdateEnemyPosition();
					UpdateAIScript();
					SlideMove();
					PlayChatter();
					CheckBlink();
					break;
			}
		}

		// clear pain flag so that we recieve any damage between now and the next time we run the script
		AI_PAIN = false;
		AI_SPECIAL_DAMAGE = 0;
		AI_PUSHED = false;
	}
	else if( thinkFlags & TH_PHYSICS )
	{
		RunPhysics();
	}

	if( af_push_moveables )
	{
		PushWithAF();
	}

	if( fl.hidden && allowHiddenMovement )
	{
		// UpdateAnimation won't call frame commands when hidden, so call them here when we allow hidden movement
		animator.ServiceAnims( gameLocal.previousTime, gameLocal.time );
	}
	/*	this still draws in retail builds.. not sure why.. don't care at this point.
		if ( !aas && developer.GetBool() && !fl.hidden && !num_cinematics ) {
			gameRenderWorld->DrawText( "No AAS", physicsObj.GetAbsBounds().GetCenter(), 0.1f, colorWhite, gameLocal.GetLocalPlayer()->viewAngles.ToMat3(), 1, 1 );
		}
	*/

	UpdateMuzzleFlash();
	UpdateAnimation();
	UpdateParticles();
	Present();
	UpdateDamageEffects();
	LinkCombat();

	if( ai_showHealth.GetBool() )
	{
		idVec3 aboveHead( 0, 0, 20 );
		gameRenderWorld->DrawText( va( "%d", ( int )health ), this->GetEyePosition() + aboveHead, 0.5f, colorWhite, gameLocal.GetLocalPlayer()->viewAngles.ToMat3() );
	}
}

/***********************************************************************

	AI script state management

***********************************************************************/

/*
=====================
idAI::LinkScriptVariables
=====================
*/
void idAI::LinkScriptVariables()
{
// jmarshall begin
	run_distance.LinkTo( scriptObject, "run_distance" );
	walk_turn.LinkTo( scriptObject, "walk_turn" );
	ambush.LinkTo( scriptObject, "ambush" );
	ignoreEnemies.LinkTo( scriptObject, "ignoreEnemies" );
	stay_on_attackpath.LinkTo( scriptObject, "stay_on_attackpath" );
	ignore_sight.LinkTo( scriptObject, "ignore_sight" );
	idle_sight_fov.LinkTo( scriptObject, "idle_sight_fov" );
// jmarshall end
	AI_TALK.LinkTo(	scriptObject, "AI_TALK" );
	AI_DAMAGE.LinkTo(	scriptObject, "AI_DAMAGE" );
	AI_PAIN.LinkTo(	scriptObject, "AI_PAIN" );
	AI_SPECIAL_DAMAGE.LinkTo(	scriptObject, "AI_SPECIAL_DAMAGE" );
	AI_DEAD.LinkTo(	scriptObject, "AI_DEAD" );
// jmarshall begin
	AI_RUN.LinkTo(	scriptObject, "run" );
	blocked.LinkTo( scriptObject, "blocked" );
	AI_ATTACKING.LinkTo( scriptObject, "AI_ATTACKING" );
// jmarshall end
	AI_ENEMY_VISIBLE.LinkTo(	scriptObject, "AI_ENEMY_VISIBLE" );
	AI_ENEMY_IN_FOV.LinkTo(	scriptObject, "AI_ENEMY_IN_FOV" );
	AI_ENEMY_DEAD.LinkTo(	scriptObject, "AI_ENEMY_DEAD" );
	AI_MOVE_DONE.LinkTo(	scriptObject, "AI_MOVE_DONE" );
	AI_ONGROUND.LinkTo(	scriptObject, "AI_ONGROUND" );
	AI_ACTIVATED.LinkTo(	scriptObject, "AI_ACTIVATED" );
	AI_FORWARD.LinkTo(	scriptObject, "AI_FORWARD" );
	AI_JUMP.LinkTo(	scriptObject, "AI_JUMP" );
	AI_BLOCKED.LinkTo(	scriptObject, "AI_BLOCKED" );
	AI_DEST_UNREACHABLE.LinkTo( scriptObject, "AI_DEST_UNREACHABLE" );
	AI_HIT_ENEMY.LinkTo(	scriptObject, "AI_HIT_ENEMY" );
	AI_OBSTACLE_IN_PATH.LinkTo(	scriptObject, "AI_OBSTACLE_IN_PATH" );
	AI_PUSHED.LinkTo(	scriptObject, "AI_PUSHED" );
}

/*
=====================
idAI::UpdateAIScript
=====================
*/
void idAI::UpdateAIScript()
{
	UpdateScript();

	// clear the hit enemy flag so we catch the next time we hit someone
	AI_HIT_ENEMY = false;

	if( allowHiddenMovement || !IsHidden() )
	{
		// update the animstate if we're not hidden
		UpdateAnimState();
	}
}

/*
=====================
idAI::GetClosestHiddenTarget
=====================
*/
idEntity* idAI::GetClosestHiddenTarget( const char* type )
{
	int	i;
	idEntity* ent;
	idEntity* bestEnt;
	float time;
	float bestTime;
	const idVec3& org = physicsObj.GetOrigin();
	idActor* enemyEnt = enemy.GetEntity();

	if( !enemyEnt )
	{
		// no enemy to hide from
		return NULL;
	}

	if( targets.Num() == 1 )
	{
		ent = targets[0].GetEntity();
		if( ent != NULL && idStr::Cmp( ent->GetEntityDefName(), type ) == 0 )
		{
			if( !EntityCanSeePos( enemyEnt, lastVisibleEnemyPos, ent->GetPhysics()->GetOrigin() ) )
			{
				return ent;
			}
		}
		return NULL;
	}

	bestEnt = NULL;
	bestTime = idMath::INFINITUM;
	for( i = 0; i < targets.Num(); i++ )
	{
		ent = targets[i].GetEntity();
		if( ent != NULL && idStr::Cmp( ent->GetEntityDefName(), type ) == 0 )
		{
			const idVec3& destOrg = ent->GetPhysics()->GetOrigin();
			time = TravelDistance( org, destOrg );
			if( ( time >= 0.0f ) && ( time < bestTime ) )
			{
				if( !EntityCanSeePos( enemyEnt, lastVisibleEnemyPos, destOrg ) )
				{
					bestEnt = ent;
					bestTime = time;
				}
			}
		}
	}
	return bestEnt;
}

/*
=====================
idAI::EntityCanSeePos
=====================
*/
bool idAI::EntityCanSeePos( idActor* actor, const idVec3& actorOrigin, const idVec3& pos )
{
	idVec3 eye, point;
	trace_t results;
	pvsHandle_t handle;

	handle = gameLocal.pvs.SetupCurrentPVS( actor->GetPVSAreas(), actor->GetNumPVSAreas() );

	if( !gameLocal.pvs.InCurrentPVS( handle, GetPVSAreas(), GetNumPVSAreas() ) )
	{
		gameLocal.pvs.FreeCurrentPVS( handle );
		return false;
	}

	gameLocal.pvs.FreeCurrentPVS( handle );

	eye = actorOrigin + actor->EyeOffset();

	point = pos;
	point[2] += 1.0f;

	physicsObj.DisableClip();

	gameLocal.clip.TracePoint( results, eye, point, MASK_SOLID, actor );
	if( results.fraction >= 1.0f || ( gameLocal.GetTraceEntity( results ) == this ) )
	{
		physicsObj.EnableClip();
		return true;
	}

	const idBounds& bounds = physicsObj.GetBounds();
	point[2] += bounds[1][2] - bounds[0][2];

	gameLocal.clip.TracePoint( results, eye, point, MASK_SOLID, actor );
	physicsObj.EnableClip();
	if( results.fraction >= 1.0f || ( gameLocal.GetTraceEntity( results ) == this ) )
	{
		return true;
	}
	return false;
}

/*
=====================
idAI::BlockedFailSafe
=====================
*/
void idAI::BlockedFailSafe()
{
	if( !ai_blockedFailSafe.GetBool() || blockedRadius < 0.0f )
	{
		return;
	}
	if( !physicsObj.OnGround() || enemy.GetEntity() == NULL ||
			( physicsObj.GetOrigin() - move.lastMoveOrigin ).LengthSqr() > Square( blockedRadius ) )
	{
		move.lastMoveOrigin = physicsObj.GetOrigin();
		move.lastMoveTime = gameLocal.time;
	}
	if( move.lastMoveTime < gameLocal.time - blockedMoveTime )
	{
		if( lastAttackTime < gameLocal.time - blockedAttackTime )
		{
			AI_BLOCKED = true;
			move.lastMoveTime = gameLocal.time;
		}
	}
}


/***********************************************************************

	Damage

***********************************************************************/

/*
=====================
idAI::ReactionTo
=====================
*/
int idAI::ReactionTo( const idEntity* ent )
{

	if( ent->fl.hidden )
	{
		// ignore hidden entities
		return ATTACK_IGNORE;
	}

	if( !ent->IsType( idActor::Type ) )
	{
		return ATTACK_IGNORE;
	}

	const idActor* actor = static_cast<const idActor*>( ent );
	if( actor->IsType( idPlayer::Type ) && static_cast<const idPlayer*>( actor )->noclip )
	{
		// ignore players in noclip mode
		return ATTACK_IGNORE;
	}

	// actors on different teams will always fight each other
	if( actor->team != team )
	{
		if( actor->fl.notarget )
		{
			// don't attack on sight when attacker is notargeted
			return ATTACK_ON_DAMAGE | ATTACK_ON_ACTIVATE;
		}
		return ATTACK_ON_SIGHT | ATTACK_ON_DAMAGE | ATTACK_ON_ACTIVATE;
	}

	// monsters will fight when attacked by lower ranked monsters.  rank 0 never fights back.
	if( rank && ( actor->rank < rank ) )
	{
		return ATTACK_ON_DAMAGE;
	}

	// don't fight back
	return ATTACK_IGNORE;
}


/*
=====================
idAI::Pain
=====================
*/
bool idAI::Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location )
{
	idActor*	actor;

	AI_PAIN = idActor::Pain( inflictor, attacker, damage, dir, location );
	AI_DAMAGE = true;

	// force a blink
	blink_time = 0;

	// ignore damage from self
	if( attacker != this )
	{
		if( inflictor )
		{
			AI_SPECIAL_DAMAGE = inflictor->spawnArgs.GetInt( "special_damage" );
		}
		else
		{
			AI_SPECIAL_DAMAGE = 0;
		}

		if( enemy.GetEntity() != attacker && attacker->IsType( idActor::Type ) )
		{
			actor = ( idActor* )attacker;
			if( ReactionTo( actor ) & ATTACK_ON_DAMAGE )
			{
				gameLocal.AlertAI( actor );
				SetEnemy( actor );
			}
		}
	}

	return ( AI_PAIN != 0 );
}


/*
=====================
idAI::SpawnParticles
=====================
*/
void idAI::SpawnParticles( const char* keyName )
{
	const idKeyValue* kv = spawnArgs.MatchPrefix( keyName, NULL );
	while( kv )
	{
		particleEmitter_t pe;

		idStr particleName = kv->GetValue();

		if( particleName.Length() )
		{

			idStr jointName = kv->GetValue();
			int dash = jointName.Find( '-' );
			if( dash > 0 )
			{
				particleName = particleName.Left( dash );
				jointName = jointName.Right( jointName.Length() - dash - 1 );
			}

			SpawnParticlesOnJoint( pe, particleName, jointName );
			particles.Append( pe );
		}

		kv = spawnArgs.MatchPrefix( keyName, kv );
	}
}

/*
=====================
idAI::SpawnParticlesOnJoint
=====================
*/
const idDeclParticle* idAI::SpawnParticlesOnJoint( particleEmitter_t& pe, const char* particleName, const char* jointName )
{
	idVec3 origin;
	idMat3 axis;

	if( *particleName == '\0' )
	{
		memset( &pe, 0, sizeof( pe ) );
		return pe.particle;
	}

	pe.joint = animator.GetJointHandle( jointName );
	if( pe.joint == INVALID_JOINT )
	{
		gameLocal.Warning( "Unknown particleJoint '%s' on '%s'", jointName, name.c_str() );
		pe.time = 0;
		pe.particle = NULL;
	}
	else
	{
		animator.GetJointTransform( pe.joint, gameLocal.time, origin, axis );
		origin = renderEntity.origin + origin * renderEntity.axis;

		BecomeActive( TH_UPDATEPARTICLES );
		if( !gameLocal.time )
		{
			// particles with time of 0 don't show, so set the time differently on the first frame
			pe.time = 1;
		}
		else
		{
			pe.time = gameLocal.time;
		}
		pe.particle = static_cast<const idDeclParticle*>( declManager->FindType( DECL_PARTICLE, particleName ) );
		gameLocal.smokeParticles->EmitSmoke( pe.particle, pe.time, gameLocal.random.CRandomFloat(), origin, axis, timeGroup /*_D3XP*/ );
	}

	return pe.particle;
}

/*
=====================
idAI::Killed
=====================
*/
void idAI::Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location )
{
	idAngles ang;
	const char* modelDeath;

	// Guardian died?  grats, you get an achievement
	if( idStr::Icmp( name, "guardian_spawn" ) == 0 )
	{
		idPlayer* player = gameLocal.GetLocalPlayer();
		if( player != NULL && player->GetExpansionType() == GAME_BASE )
		{
			player->GetAchievementManager().EventCompletesAchievement( ACHIEVEMENT_DEFEAT_GUARDIAN_BOSS );
		}
	}

	// make sure the monster is activated
	EndAttack();

	if( g_debugDamage.GetBool() )
	{
		gameLocal.Printf( "Damage: joint: '%s', zone '%s'\n", animator.GetJointName( ( jointHandle_t )location ),
						  GetDamageGroup( location ) );
	}

	if( inflictor )
	{
		AI_SPECIAL_DAMAGE = inflictor->spawnArgs.GetInt( "special_damage" );
	}
	else
	{
		AI_SPECIAL_DAMAGE = 0;
	}

	if( AI_DEAD )
	{
		AI_PAIN = true;
		AI_DAMAGE = true;
		return;
	}

	// stop all voice sounds
	StopSound( SND_CHANNEL_VOICE, false );
	if( head.GetEntity() )
	{
		head.GetEntity()->StopSound( SND_CHANNEL_VOICE, false );
		head.GetEntity()->GetAnimator()->ClearAllAnims( gameLocal.time, 100 );
	}

	disableGravity = false;
	move.moveType = MOVETYPE_DEAD;
	af_push_moveables = false;

	physicsObj.UseFlyMove( false );
	physicsObj.ForceDeltaMove( false );

	// end our looping ambient sound
	StopSound( SND_CHANNEL_AMBIENT, false );

	if( attacker && attacker->IsType( idActor::Type ) )
	{
		gameLocal.AlertAI( ( idActor* )attacker );
	}

	// activate targets
	ActivateTargets( attacker );

	RemoveAttachments();
	RemoveProjectile();
	StopMove( MOVE_STATUS_DONE );

	ClearEnemy();
	AI_DEAD	= true;

	// make monster nonsolid
	physicsObj.SetContents( 0 );
	physicsObj.GetClipModel()->Unlink();

	Unbind();

	if( StartRagdoll() )
	{
		StartSound( "snd_death", SND_CHANNEL_VOICE, 0, false, NULL );
	}

	if( spawnArgs.GetString( "model_death", "", &modelDeath ) )
	{
		// lost soul is only case that does not use a ragdoll and has a model_death so get the death sound in here
		StartSound( "snd_death", SND_CHANNEL_VOICE, 0, false, NULL );
		renderEntity.shaderParms[ SHADERPARM_TIMEOFFSET ] = -MS2SEC( gameLocal.time );
		SetModel( modelDeath );
		physicsObj.SetLinearVelocity( vec3_zero );
		physicsObj.PutToRest();
		physicsObj.DisableImpact();
		// No grabbing if "model_death"
		noGrab = true;
	}

	restartParticles = false;

	// jmarshall
	stateThread.SetState( "state_Killed" );
	SetWaitState( "" );

	const idKeyValue* kv = spawnArgs.MatchPrefix( "def_drops", NULL );
	while( kv )
	{
		idDict args;

		args.Set( "classname", kv->GetValue() );
		args.Set( "origin", physicsObj.GetOrigin().ToString() );
		gameLocal.SpawnEntityDef( args );
		kv = spawnArgs.MatchPrefix( "def_drops", kv );
	}

	if( ( attacker && attacker->IsType( idPlayer::Type ) ) && ( inflictor && !inflictor->IsType( idSoulCubeMissile::Type ) ) )
	{
		static_cast< idPlayer* >( attacker )->AddAIKill();
	}

	if( spawnArgs.GetBool( "harvest_on_death" ) )
	{
		const idDict* harvestDef = gameLocal.FindEntityDefDict( spawnArgs.GetString( "def_harvest_type" ), false );
		if( harvestDef )
		{
			idEntity* temp;
			gameLocal.SpawnEntityDef( *harvestDef, &temp, false );
			harvestEnt = static_cast<idHarvestable*>( temp );

		}

		if( harvestEnt.GetEntity() )
		{
			//Let the harvest entity set itself up
			harvestEnt.GetEntity()->Init( this );
			harvestEnt.GetEntity()->BecomeActive( TH_THINK );
		}
	}
}

/***********************************************************************

	Targeting/Combat

***********************************************************************/

/*
=====================
idAI::PlayCinematic
=====================
*/
void idAI::PlayCinematic()
{
	const char* animname;

	if( current_cinematic >= num_cinematics )
	{
		if( g_debugCinematic.GetBool() )
		{
			gameLocal.Printf( "%d: '%s' stop\n", gameLocal.framenum, GetName() );
		}
		if( !spawnArgs.GetBool( "cinematic_no_hide" ) )
		{
			Hide();
		}
		current_cinematic = 0;
		ActivateTargets( gameLocal.GetLocalPlayer() );
		fl.neverDormant = false;
		return;
	}

	Show();
	current_cinematic++;

	allowJointMod = false;
	allowEyeFocus = false;

	spawnArgs.GetString( va( "anim%d", current_cinematic ), NULL, &animname );
	if( !animname )
	{
		gameLocal.Warning( "missing 'anim%d' key on %s", current_cinematic, name.c_str() );
		return;
	}

	if( g_debugCinematic.GetBool() )
	{
		gameLocal.Printf( "%d: '%s' start '%s'\n", gameLocal.framenum, GetName(), animname );
	}

	headAnim.animBlendFrames = 0;
	headAnim.lastAnimBlendFrames = 0;
	headAnim.BecomeIdle();

	legsAnim.animBlendFrames = 0;
	legsAnim.lastAnimBlendFrames = 0;
	legsAnim.BecomeIdle();

	torsoAnim.animBlendFrames = 0;
	torsoAnim.lastAnimBlendFrames = 0;
	ProcessEvent( &AI_PlayAnim, ANIMCHANNEL_TORSO, animname );

	// make sure our model gets updated
	animator.ForceUpdate();

	// update the anim bounds
	UpdateAnimation();
	UpdateVisuals();
	Present();

	if( head.GetEntity() )
	{
		// since the body anim was updated, we need to run physics to update the position of the head
		RunPhysics();

		// make sure our model gets updated
		head.GetEntity()->GetAnimator()->ForceUpdate();

		// update the anim bounds
		head.GetEntity()->UpdateAnimation();
		head.GetEntity()->UpdateVisuals();
		head.GetEntity()->Present();
	}

	fl.neverDormant = true;
}

/*
=====================
idAI::Activate

Notifies the script that a monster has been activated by a trigger or flashlight
=====================
*/
void idAI::Activate( idEntity* activator )
{
	idPlayer* player;

	if( AI_DEAD )
	{
		// ignore it when they're dead
		return;
	}

	// make sure he's not dormant
	dormantStart = 0;

	if( num_cinematics )
	{
		PlayCinematic();
	}
	else
	{
		AI_ACTIVATED = true;
		if( !activator || !activator->IsType( idPlayer::Type ) )
		{
			player = gameLocal.GetLocalPlayer();
		}
		else
		{
			player = static_cast<idPlayer*>( activator );
		}

		if( ReactionTo( player ) & ATTACK_ON_ACTIVATE )
		{
			SetEnemy( player );
		}

		// update the script in cinematics so that entities don't start anims or show themselves a frame late.
		if( cinematic )
		{
			UpdateAIScript();

			// make sure our model gets updated
			animator.ForceUpdate();

			// update the anim bounds
			UpdateAnimation();
			UpdateVisuals();
			Present();

			if( head.GetEntity() )
			{
				// since the body anim was updated, we need to run physics to update the position of the head
				RunPhysics();

				// make sure our model gets updated
				head.GetEntity()->GetAnimator()->ForceUpdate();

				// update the anim bounds
				head.GetEntity()->UpdateAnimation();
				head.GetEntity()->UpdateVisuals();
				head.GetEntity()->Present();
			}
		}
	}
}

/*
=====================
idAI::EnemyDead
=====================
*/
void idAI::EnemyDead()
{
	ClearEnemy();
	AI_ENEMY_DEAD = true;
}

/*
=====================
idAI::TalkTo
=====================
*/
void idAI::TalkTo( idActor* actor )
{
	if( talk_state != TALK_OK )
	{
		return;
	}

	// Wake up monsters that are pretending to be NPC's
	if( team == 1 && actor && actor->team != team )
	{
		ProcessEvent( &EV_Activate, actor );
	}

	talkTarget = actor;
	if( actor )
	{
		AI_TALK = true;
	}
	else
	{
		AI_TALK = false;
	}
}

/*
=====================
idAI::GetEnemy
=====================
*/
idActor*	idAI::GetEnemy() const
{
	return enemy.GetEntity();
}

/*
=====================
idAI::GetTalkState
=====================
*/
talkState_t idAI::GetTalkState() const
{
	if( ( talk_state != TALK_NEVER ) && AI_DEAD )
	{
		return TALK_DEAD;
	}
	if( IsHidden() )
	{
		return TALK_NEVER;
	}
	return talk_state;
}

/*
=====================
idAI::TouchedByFlashlight
=====================
*/
void idAI::TouchedByFlashlight( idActor* flashlight_owner )
{
	if( wakeOnFlashlight )
	{
		Activate( flashlight_owner );
	}
}

/*
=====================
idAI::ClearEnemy
=====================
*/
void idAI::ClearEnemy()
{
	if( move.moveCommand == MOVE_TO_ENEMY )
	{
		StopMove( MOVE_STATUS_DEST_NOT_FOUND );
	}

	enemyNode.Remove();
	enemy				= NULL;
	AI_ENEMY_IN_FOV		= false;
	AI_ENEMY_VISIBLE	= false;
	AI_ENEMY_DEAD		= true;

	SetChatSound();
}

/*
=====================
idAI::EnemyPositionValid
=====================
*/
bool idAI::EnemyPositionValid() const
{
	trace_t	tr;
	idVec3	muzzle;
	idMat3	axis;

	if( !enemy.GetEntity() )
	{
		return false;
	}

	if( AI_ENEMY_VISIBLE )
	{
		return true;
	}

	gameLocal.clip.TracePoint( tr, GetEyePosition(), lastVisibleEnemyPos + lastVisibleEnemyEyeOffset, MASK_OPAQUE, this );
	if( tr.fraction < 1.0f )
	{
		// can't see the area yet, so don't know if he's there or not
		return true;
	}

	return false;
}

// jmarshall begin
/*
================
idAI::PlayCustomAnim
================
*/
void idAI::PlayCustomAnim( idStr animname, float blendIn, float blendOut )
{
	scriptThread->ClearStack();
	scriptThread->PushEntity( this );
	scriptThread->PushString( animname );
	scriptThread->PushFloat( blendIn );
	scriptThread->PushFloat( blendOut );
	scriptThread->CallFunction( scriptObject.GetFunction( "playCustomAnim" ), false );
}
/*
================
idAI::PlayCustomCycle
================
*/
void idAI::PlayCustomCycle( idStr animname, float blendTime )
{
	scriptThread->ClearStack();
	scriptThread->PushEntity( this );
	scriptThread->PushString( animname );
	scriptThread->PushFloat( blendTime );
	scriptThread->CallFunction( scriptObject.GetFunction( "playCustomCycle" ), false );
}

/*
======================
idAI::trigger_wakeup_targets
======================
*/
void idAI::trigger_wakeup_targets()
{
	idStr key;
	idStr name;
	idEntity* ent;

	key = GetNextKey( "wakeup_target", "" );
	while( key != "" )
	{
		name = GetKey( key );
		ent = gameLocal.FindEntity( name );
		if( !ent )
		{
			idLib::Warning( "Unknown wakeup_target '%s' on entity '%s'", name.c_str(), GetName() );
		}
		else
		{
			ent->Signal( SIG_TRIGGER );
			ent->ProcessEvent( &EV_Activate, gameLocal.GetLocalPlayer() );
			ent->TriggerGuis();
		}
		key = GetNextKey( "wakeup_target", key );
	}
}

/*
======================
idAI::checkForEnemy
======================
*/
void idAI::sight_enemy()
{
	idStr animname;

	Event_FaceEnemy();
	animname = GetKey( "on_activate" );
	if( animname != "" )
	{
		// don't go dormant during on_activate anims since they
		// may end up floating in air during no gravity anims.
		Event_SetNeverDormant( true );
		if( GetIntKey( "walk_on_sight" ) )
		{
			Event_MoveToEnemy();
		}
		Event_AnimState( ANIMCHANNEL_TORSO, "Torso_Sight", 4 );
		//waitAction("sight"); // jmarshall implement this?
		Event_SetNeverDormant( GetFloatKey( "neverdormant" ) );
	}
}
// jmarshall end

/*
=====================
idAI::SetEnemyPosition
=====================
*/
void idAI::SetEnemyPosition()
{
	idActor*		enemyEnt = enemy.GetEntity();
	int			enemyAreaNum;
	int			areaNum;
	int			lastVisibleReachableEnemyAreaNum = 0;
	idVec3		pos;
	bool		onGround;

	if( !enemyEnt )
	{
		return;
	}

	lastVisibleReachableEnemyPos = lastReachableEnemyPos;
	lastVisibleEnemyEyeOffset = enemyEnt->EyeOffset();
	lastVisibleEnemyPos = enemyEnt->GetPhysics()->GetOrigin();
	if( move.moveType == MOVETYPE_FLY )
	{
		pos = lastVisibleEnemyPos;
		onGround = true;
	}
	else
	{
		onGround = enemyEnt->GetFloorPos( 64.0f, pos );
		if( enemyEnt->OnLadder() )
		{
			onGround = false;
		}
	}

	if( !onGround )
	{
		if( move.moveCommand == MOVE_TO_ENEMY )
		{
			AI_DEST_UNREACHABLE = true;
		}
		return;
	}

	// when we don't have an AAS, we can't tell if an enemy is reachable or not,
	// so just assume that he is.
	if( !aas )
	{
		lastVisibleReachableEnemyPos = lastVisibleEnemyPos;
		if( move.moveCommand == MOVE_TO_ENEMY )
		{
			AI_DEST_UNREACHABLE = false;
		}
		enemyAreaNum = 0;
		areaNum = 0;
	}
	else
	{
		lastVisibleReachableEnemyAreaNum = move.toAreaNum;
		enemyAreaNum = PointReachableAreaNum( lastVisibleEnemyPos, 1.0f );
		if( !enemyAreaNum )
		{
			enemyAreaNum = PointReachableAreaNum( lastReachableEnemyPos, 1.0f );
			pos = lastReachableEnemyPos;
		}
		if( !enemyAreaNum )
		{
			if( move.moveCommand == MOVE_TO_ENEMY )
			{
				AI_DEST_UNREACHABLE = true;
			}
			areaNum = 0;
		}
		else
		{
			aasPath_t path;
			const idVec3& org = physicsObj.GetOrigin();
			areaNum = PointReachableAreaNum( org );
			if( PathToGoal( path, areaNum, org, enemyAreaNum, pos ) )
			{
				lastVisibleReachableEnemyPos = pos;
				lastVisibleReachableEnemyAreaNum = enemyAreaNum;
				if( move.moveCommand == MOVE_TO_ENEMY )
				{
					AI_DEST_UNREACHABLE = false;
				}
			}
			else if( move.moveCommand == MOVE_TO_ENEMY )
			{
				AI_DEST_UNREACHABLE = true;
			}
		}
	}

	if( move.moveCommand == MOVE_TO_ENEMY )
	{
		if( !aas )
		{
			// keep the move destination up to date for wandering
			move.moveDest = lastVisibleReachableEnemyPos;
		}
		else if( enemyAreaNum )
		{
			move.toAreaNum = lastVisibleReachableEnemyAreaNum;
			move.moveDest = lastVisibleReachableEnemyPos;
		}

		if( move.moveType == MOVETYPE_FLY )
		{
			predictedPath_t path;
			idVec3 end = move.moveDest;
			end.z += enemyEnt->EyeOffset().z + fly_offset;
			idAI::PredictPath( this, aas, move.moveDest, end - move.moveDest, 1000, 1000, SE_BLOCKED, path );
			move.moveDest = path.endPos;
			move.toAreaNum = PointReachableAreaNum( move.moveDest, 1.0f );
		}
	}
}

/*
=====================
idAI::UpdateEnemyPosition
=====================
*/
void idAI::UpdateEnemyPosition()
{
	idActor* enemyEnt = enemy.GetEntity();
	int				enemyAreaNum;
	int				areaNum;
	aasPath_t		path;
	predictedPath_t predictedPath;
	idVec3			enemyPos;
	bool			onGround;

	if( !enemyEnt )
	{
		return;
	}

	const idVec3& org = physicsObj.GetOrigin();

	if( move.moveType == MOVETYPE_FLY )
	{
		enemyPos = enemyEnt->GetPhysics()->GetOrigin();
		onGround = true;
	}
	else
	{
		onGround = enemyEnt->GetFloorPos( 64.0f, enemyPos );
		if( enemyEnt->OnLadder() )
		{
			onGround = false;
		}
	}

	if( onGround )
	{
		// when we don't have an AAS, we can't tell if an enemy is reachable or not,
		// so just assume that he is.
		if( !aas )
		{
			enemyAreaNum = 0;
			lastReachableEnemyPos = enemyPos;
		}
		else
		{
			enemyAreaNum = PointReachableAreaNum( enemyPos, 1.0f );
			if( enemyAreaNum )
			{
				areaNum = PointReachableAreaNum( org );
				if( PathToGoal( path, areaNum, org, enemyAreaNum, enemyPos ) )
				{
					lastReachableEnemyPos = enemyPos;
				}
			}
		}
	}

	AI_ENEMY_IN_FOV		= false;
	AI_ENEMY_VISIBLE	= false;

	if( CanSee( enemyEnt, false ) )
	{
		AI_ENEMY_VISIBLE = true;
		if( CheckFOV( enemyEnt->GetPhysics()->GetOrigin() ) )
		{
			AI_ENEMY_IN_FOV = true;
		}

		SetEnemyPosition();
	}
	else
	{
		// check if we heard any sounds in the last frame
		if( enemyEnt == gameLocal.GetAlertEntity() )
		{
			float dist = ( enemyEnt->GetPhysics()->GetOrigin() - org ).LengthSqr();
			if( dist < Square( AI_HEARING_RANGE ) )
			{
				SetEnemyPosition();
			}
		}
	}

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugBounds( colorLtGrey, enemyEnt->GetPhysics()->GetBounds(), lastReachableEnemyPos, 1 );
		gameRenderWorld->DebugBounds( colorWhite, enemyEnt->GetPhysics()->GetBounds(), lastVisibleReachableEnemyPos, 1 );
	}
}

/*
=====================
idAI::SetEnemy
=====================
*/
void idAI::SetEnemy( idActor* newEnemy )
{
	int enemyAreaNum;

	if( AI_DEAD )
	{
		ClearEnemy();
		return;
	}

	AI_ENEMY_DEAD = false;
	if( !newEnemy )
	{
		ClearEnemy();
	}
	else if( enemy.GetEntity() != newEnemy )
	{

		// Check to see if we should unlock the 'Turncloak' achievement
		const idActor* enemyEnt = enemy.GetEntity();
		if( enemyEnt != NULL && enemyEnt->IsType( idPlayer::Type ) && newEnemy->IsType( idAI::Type ) && newEnemy->team == this->team && ( idStr::Icmp( newEnemy->GetName(), "hazmat_dummy" ) != 0 ) )
		{
			idPlayer* player = gameLocal.GetLocalPlayer();
			if( player != NULL )
			{
				player->GetAchievementManager().EventCompletesAchievement( ACHIEVEMENT_TWO_DEMONS_FIGHT_EACH_OTHER );
			}
		}

		enemy = newEnemy;
		enemyNode.AddToEnd( newEnemy->enemyList );
		if( newEnemy->health <= 0 )
		{
			EnemyDead();
			return;
		}
		// let the monster know where the enemy is
		newEnemy->GetAASLocation( aas, lastReachableEnemyPos, enemyAreaNum );
		SetEnemyPosition();
		SetChatSound();

		lastReachableEnemyPos = lastVisibleEnemyPos;
		lastVisibleReachableEnemyPos = lastReachableEnemyPos;
		enemyAreaNum = PointReachableAreaNum( lastReachableEnemyPos, 1.0f );
		if( aas && enemyAreaNum )
		{
			aas->PushPointIntoAreaNum( enemyAreaNum, lastReachableEnemyPos );
			lastVisibleReachableEnemyPos = lastReachableEnemyPos;
		}
	}
}

/*
============
idAI::FirstVisiblePointOnPath
============
*/
idVec3 idAI::FirstVisiblePointOnPath( const idVec3 origin, const idVec3& target, int travelFlags ) const
{
	int i, areaNum, targetAreaNum, curAreaNum, travelTime;
	idVec3 curOrigin;
	idReachability* reach;

	if( !aas )
	{
		return origin;
	}

	areaNum = PointReachableAreaNum( origin );
	targetAreaNum = PointReachableAreaNum( target );

	if( !areaNum || !targetAreaNum )
	{
		return origin;
	}

	if( ( areaNum == targetAreaNum ) || PointVisible( origin ) )
	{
		return origin;
	}

	curAreaNum = areaNum;
	curOrigin = origin;

	for( i = 0; i < 10; i++ )
	{

		if( !aas->RouteToGoalArea( curAreaNum, curOrigin, targetAreaNum, travelFlags, travelTime, &reach ) )
		{
			break;
		}

		if( !reach )
		{
			return target;
		}

		curAreaNum = reach->toAreaNum;
		curOrigin = reach->end;

		if( PointVisible( curOrigin ) )
		{
			return curOrigin;
		}
	}

	return origin;
}

/*
===================
idAI::CalculateAttackOffsets

calculate joint positions on attack frames so we can do proper "can hit" tests
===================
*/
void idAI::CalculateAttackOffsets()
{
	const idDeclModelDef*	modelDef;
	int						num;
	int						i;
	int						frame;
	const frameCommand_t*	command;
	idMat3					axis;
	const idAnim*			anim;
	jointHandle_t			joint;

	modelDef = animator.ModelDef();
	if( !modelDef )
	{
		return;
	}
	num = modelDef->NumAnims();

	// needs to be off while getting the offsets so that we account for the distance the monster moves in the attack anim
	animator.RemoveOriginOffset( false );

	// anim number 0 is reserved for non-existant anims.  to avoid off by one issues, just allocate an extra spot for
	// launch offsets so that anim number can be used without subtracting 1.
	missileLaunchOffset.SetGranularity( 1 );
	missileLaunchOffset.SetNum( num + 1 );
	missileLaunchOffset[ 0 ].Zero();

	for( i = 1; i <= num; i++ )
	{
		missileLaunchOffset[ i ].Zero();
		anim = modelDef->GetAnim( i );
		if( anim )
		{
			frame = anim->FindFrameForFrameCommand( FC_LAUNCHMISSILE, &command );
			if( frame >= 0 )
			{
				joint = animator.GetJointHandle( command->string->c_str() );
				if( joint == INVALID_JOINT )
				{
					gameLocal.Error( "Invalid joint '%s' on 'launch_missile' frame command on frame %d of model '%s'", command->string->c_str(), frame, modelDef->GetName() );
				}
				GetJointTransformForAnim( joint, i, FRAME2MS( frame ), missileLaunchOffset[ i ], axis );
			}
		}
	}

	animator.RemoveOriginOffset( true );
}

/*
=====================
idAI::CreateProjectileClipModel
=====================
*/
void idAI::CreateProjectileClipModel() const
{
	if( projectileClipModel == NULL )
	{
		idBounds projectileBounds( vec3_origin );
		projectileBounds.ExpandSelf( projectileRadius );
		projectileClipModel	= new( TAG_MODEL ) idClipModel( idTraceModel( projectileBounds ) );
	}
}

/*
=====================
idAI::GetAimDir
=====================
*/
bool idAI::GetAimDir( const idVec3& firePos, idEntity* aimAtEnt, const idEntity* ignore, idVec3& aimDir ) const
{
	idVec3	targetPos1;
	idVec3	targetPos2;
	idVec3	delta;
	float	max_height;
	bool	result;

	// if no aimAtEnt or projectile set
	if( !aimAtEnt || !projectileDef )
	{
		aimDir = viewAxis[ 0 ] * physicsObj.GetGravityAxis();
		return false;
	}

	if( projectileClipModel == NULL )
	{
		CreateProjectileClipModel();
	}

	if( aimAtEnt == enemy.GetEntity() )
	{
		static_cast<idActor*>( aimAtEnt )->GetAIAimTargets( lastVisibleEnemyPos, targetPos1, targetPos2 );
	}
	else if( aimAtEnt->IsType( idActor::Type ) )
	{
		static_cast<idActor*>( aimAtEnt )->GetAIAimTargets( aimAtEnt->GetPhysics()->GetOrigin(), targetPos1, targetPos2 );
	}
	else
	{
		targetPos1 = aimAtEnt->GetPhysics()->GetAbsBounds().GetCenter();
		targetPos2 = targetPos1;
	}

	if( this->team == 0 && !idStr::Cmp( aimAtEnt->GetEntityDefName(), "monster_demon_vulgar" ) )
	{
		targetPos1.z -= 28.f;
		targetPos2.z -= 12.f;
	}

	// try aiming for chest
	delta = firePos - targetPos1;
	max_height = delta.LengthFast() * projectile_height_to_distance_ratio;
	result = PredictTrajectory( firePos, targetPos1, projectileSpeed, projectileGravity, projectileClipModel, MASK_SHOT_RENDERMODEL, max_height, ignore, aimAtEnt, ai_debugTrajectory.GetBool() ? 1000 : 0, aimDir );
	if( result || !aimAtEnt->IsType( idActor::Type ) )
	{
		return result;
	}

	// try aiming for head
	delta = firePos - targetPos2;
	max_height = delta.LengthFast() * projectile_height_to_distance_ratio;
	result = PredictTrajectory( firePos, targetPos2, projectileSpeed, projectileGravity, projectileClipModel, MASK_SHOT_RENDERMODEL, max_height, ignore, aimAtEnt, ai_debugTrajectory.GetBool() ? 1000 : 0, aimDir );

	return result;
}

/*
=====================
idAI::BeginAttack
=====================
*/
void idAI::BeginAttack( const char* name )
{
	attack = name;
	lastAttackTime = gameLocal.time;
}

/*
=====================
idAI::EndAttack
=====================
*/
void idAI::EndAttack()
{
	attack = "";
}

/*
=====================
idAI::CreateProjectile
=====================
*/
idProjectile* idAI::CreateProjectile( const idVec3& pos, const idVec3& dir )
{
	idEntity* ent;
	const char* clsname;

	if( !projectile.GetEntity() )
	{
		gameLocal.SpawnEntityDef( *projectileDef, &ent, false );
		if( ent == NULL )
		{
			clsname = projectileDef->GetString( "classname" );
			gameLocal.Error( "Could not spawn entityDef '%s'", clsname );
			return NULL;
		}

		if( !ent->IsType( idProjectile::Type ) )
		{
			clsname = ent->GetClassname();
			gameLocal.Error( "'%s' is not an idProjectile", clsname );
		}
		projectile = ( idProjectile* )ent;
	}

	projectile.GetEntity()->Create( this, pos, dir );

	return projectile.GetEntity();
}

/*
=====================
idAI::RemoveProjectile
=====================
*/
void idAI::RemoveProjectile()
{
	if( projectile.GetEntity() )
	{
		projectile.GetEntity()->PostEventMS( &EV_Remove, 0 );
		projectile = NULL;
	}
}

/*
=====================
idAI::LaunchProjectile
=====================
*/
idProjectile* idAI::LaunchProjectile( const char* jointname, idEntity* target, bool clampToAttackCone )
{
	idVec3				muzzle;
	idVec3				dir;
	idVec3				start;
	trace_t				tr;
	idBounds			projBounds;
	float				distance;
	const idClipModel*	projClip;
	float				attack_accuracy;
	float				attack_cone;
	float				projectile_spread;
	float				diff;
	float				angle;
	float				spin;
	idAngles			ang;
	int					num_projectiles;
	int					i;
	idMat3				axis;
	idMat3				proj_axis;
	bool				forceMuzzle;
	idVec3				tmp;
	idProjectile*		lastProjectile;

	if( !projectileDef )
	{
		gameLocal.Warning( "%s (%s) doesn't have a projectile specified", name.c_str(), GetEntityDefName() );
		return NULL;
	}

	attack_accuracy = spawnArgs.GetFloat( "attack_accuracy", "7" );
	attack_cone = spawnArgs.GetFloat( "attack_cone", "70" );
	projectile_spread = spawnArgs.GetFloat( "projectile_spread", "0" );
	num_projectiles = spawnArgs.GetInt( "num_projectiles", "1" );
	forceMuzzle = spawnArgs.GetBool( "forceMuzzle", "0" );

	GetMuzzle( jointname, muzzle, axis );

	if( !projectile.GetEntity() )
	{
		CreateProjectile( muzzle, axis[ 0 ] );
	}

	lastProjectile = projectile.GetEntity();

	if( target != NULL )
	{
		tmp = target->GetPhysics()->GetAbsBounds().GetCenter() - muzzle;
		tmp.Normalize();
		axis = tmp.ToMat3();
	}
	else
	{
		axis = viewAxis;
	}

	// rotate it because the cone points up by default
	tmp = axis[2];
	axis[2] = axis[0];
	axis[0] = -tmp;

	proj_axis = axis;

	if( !forceMuzzle )  	// _D3XP
	{
		// make sure the projectile starts inside the monster bounding box
		const idBounds& ownerBounds = physicsObj.GetAbsBounds();
		projClip = lastProjectile->GetPhysics()->GetClipModel();
		projBounds = projClip->GetBounds().Rotate( axis );

		// check if the owner bounds is bigger than the projectile bounds
		if( ( ( ownerBounds[1][0] - ownerBounds[0][0] ) > ( projBounds[1][0] - projBounds[0][0] ) ) &&
				( ( ownerBounds[1][1] - ownerBounds[0][1] ) > ( projBounds[1][1] - projBounds[0][1] ) ) &&
				( ( ownerBounds[1][2] - ownerBounds[0][2] ) > ( projBounds[1][2] - projBounds[0][2] ) ) )
		{
			if( ( ownerBounds - projBounds ).RayIntersection( muzzle, viewAxis[ 0 ], distance ) )
			{
				start = muzzle + distance * viewAxis[ 0 ];
			}
			else
			{
				start = ownerBounds.GetCenter();
			}
		}
		else
		{
			// projectile bounds bigger than the owner bounds, so just start it from the center
			start = ownerBounds.GetCenter();
		}

		gameLocal.clip.Translation( tr, start, muzzle, projClip, axis, MASK_SHOT_RENDERMODEL, this );
		muzzle = tr.endpos;
	}

	// set aiming direction
	GetAimDir( muzzle, target, this, dir );
	ang = dir.ToAngles();

	// adjust his aim so it's not perfect.  uses sine based movement so the tracers appear less random in their spread.
	float t = MS2SEC( gameLocal.time + entityNumber * 497 );
	ang.pitch += idMath::Sin16( t * 5.1 ) * attack_accuracy;
	ang.yaw	+= idMath::Sin16( t * 6.7 ) * attack_accuracy;

	if( clampToAttackCone )
	{
		// clamp the attack direction to be within monster's attack cone so he doesn't do
		// things like throw the missile backwards if you're behind him
		diff = idMath::AngleDelta( ang.yaw, current_yaw );
		if( diff > attack_cone )
		{
			ang.yaw = current_yaw + attack_cone;
		}
		else if( diff < -attack_cone )
		{
			ang.yaw = current_yaw - attack_cone;
		}
	}

	axis = ang.ToMat3();

	float spreadRad = DEG2RAD( projectile_spread );
	for( i = 0; i < num_projectiles; i++ )
	{
		// spread the projectiles out
		angle = idMath::Sin( spreadRad * gameLocal.random.RandomFloat() );
		spin = ( float )DEG2RAD( 360.0f ) * gameLocal.random.RandomFloat();
		dir = axis[ 0 ] + axis[ 2 ] * ( angle * idMath::Sin( spin ) ) - axis[ 1 ] * ( angle * idMath::Cos( spin ) );
		dir.Normalize();

		// launch the projectile
		if( !projectile.GetEntity() )
		{
			CreateProjectile( muzzle, dir );
		}
		lastProjectile = projectile.GetEntity();
		lastProjectile->Launch( muzzle, dir, vec3_origin );
		projectile = NULL;
	}

	TriggerWeaponEffects( muzzle );

	lastAttackTime = gameLocal.time;

	return lastProjectile;
}

/*
================
idAI::DamageFeedback

callback function for when another entity received damage from this entity.  damage can be adjusted and returned to the caller.

FIXME: This gets called when we call idPlayer::CalcDamagePoints from idAI::AttackMelee, which then checks for a saving throw,
possibly forcing a miss.  This is harmless behavior ATM, but is not intuitive.
================
*/
void idAI::DamageFeedback( idEntity* victim, idEntity* inflictor, int& damage )
{
	if( ( victim == this ) && inflictor->IsType( idProjectile::Type ) )
	{
		// monsters only get half damage from their own projectiles
		damage = ( damage + 1 ) / 2;  // round up so we don't do 0 damage

	}
	else if( victim == enemy.GetEntity() )
	{
		AI_HIT_ENEMY = true;
	}
}

/*
=====================
idAI::DirectDamage

Causes direct damage to an entity

kickDir is specified in the monster's coordinate system, and gives the direction
that the view kick and knockback should go
=====================
*/
void idAI::DirectDamage( const char* meleeDefName, idEntity* ent )
{
	const idDict* meleeDef;
	const char* p;
	const idSoundShader* shader;

	meleeDef = gameLocal.FindEntityDefDict( meleeDefName, false );
	if( meleeDef == NULL )
	{
		gameLocal.Error( "Unknown damage def '%s' on '%s'", meleeDefName, name.c_str() );
		return;
	}

	if( !ent->fl.takedamage )
	{
		const idSoundShader* shader = declManager->FindSound( meleeDef->GetString( "snd_miss" ) );
		StartSoundShader( shader, SND_CHANNEL_DAMAGE, 0, false, NULL );
		return;
	}

	//
	// do the damage
	//
	p = meleeDef->GetString( "snd_hit" );
	if( p != NULL && *p != '\0' )
	{
		shader = declManager->FindSound( p );
		StartSoundShader( shader, SND_CHANNEL_DAMAGE, 0, false, NULL );
	}

	idVec3	kickDir;
	meleeDef->GetVector( "kickDir", "0 0 0", kickDir );

	idVec3	globalKickDir;
	globalKickDir = ( viewAxis * physicsObj.GetGravityAxis() ) * kickDir;

	ent->Damage( this, this, globalKickDir, meleeDefName, 1.0f, INVALID_JOINT );

	// end the attack if we're a multiframe attack
	EndAttack();
}

// jmarshall begin
/*
=====================
idAI::enemy_dead
=====================
*/
void idAI::enemy_dead()
{
	AI_ENEMY_DEAD = false;
	checkForEnemy( false );
	if( !GetEnemy() )
	{
		SetState( "state_Idle" );
	}
	else
	{
		SetState( "state_Combat" );
	}
}
// jmarshall end

/*
=====================
idAI::TestMelee
=====================
*/
bool idAI::TestMelee() const
{
	trace_t trace;
	idActor* enemyEnt = enemy.GetEntity();

	if( !enemyEnt || !melee_range )
	{
		return false;
	}

	//FIXME: make work with gravity vector
	idVec3 org = physicsObj.GetOrigin();
	const idBounds& myBounds = physicsObj.GetBounds();
	idBounds bounds;

	// expand the bounds out by our melee range
	bounds[0][0] = -melee_range;
	bounds[0][1] = -melee_range;
	bounds[0][2] = myBounds[0][2] - 4.0f;
	bounds[1][0] = melee_range;
	bounds[1][1] = melee_range;
	bounds[1][2] = myBounds[1][2] + 4.0f;
	bounds.TranslateSelf( org );

	idVec3 enemyOrg = enemyEnt->GetPhysics()->GetOrigin();
	idBounds enemyBounds = enemyEnt->GetPhysics()->GetBounds();
	enemyBounds.TranslateSelf( enemyOrg );

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugBounds( colorYellow, bounds, vec3_zero, 1 );
	}

	if( !bounds.IntersectsBounds( enemyBounds ) )
	{
		return false;
	}

	idVec3 start = GetEyePosition();
	idVec3 end = enemyEnt->GetEyePosition();

	gameLocal.clip.TracePoint( trace, start, end, MASK_SHOT_BOUNDINGBOX, this );
	if( ( trace.fraction == 1.0f ) || ( gameLocal.GetTraceEntity( trace ) == enemyEnt ) )
	{
		return true;
	}

	return false;
}

/*
=====================
idAI::AttackMelee

jointname allows the endpoint to be exactly specified in the model,
as for the commando tentacle.  If not specified, it will be set to
the facing direction + melee_range.

kickDir is specified in the monster's coordinate system, and gives the direction
that the view kick and knockback should go
=====================
*/
bool idAI::AttackMelee( const char* meleeDefName )
{
	const idDict* meleeDef;
	idActor* enemyEnt = enemy.GetEntity();
	const char* p;
	const idSoundShader* shader;

	meleeDef = gameLocal.FindEntityDefDict( meleeDefName, false );
	if( meleeDef == NULL )
	{
		gameLocal.Error( "Unknown melee '%s'", meleeDefName );
		return false;
	}

	if( enemyEnt == NULL )
	{
		p = meleeDef->GetString( "snd_miss" );
		if( p != NULL && *p != '\0' )
		{
			shader = declManager->FindSound( p );
			StartSoundShader( shader, SND_CHANNEL_DAMAGE, 0, false, NULL );
		}
		return false;
	}

	// check for the "saving throw" automatic melee miss on lethal blow
	// stupid place for this.
	bool forceMiss = false;
	if( enemyEnt->IsType( idPlayer::Type ) && g_skill.GetInteger() < 2 )
	{
		int	damage, armor;
		idPlayer* player = static_cast<idPlayer*>( enemyEnt );
		player->CalcDamagePoints( this, this, meleeDef, 1.0f, INVALID_JOINT, &damage, &armor );

		if( enemyEnt->health <= damage )
		{
			int	t = gameLocal.time - player->lastSavingThrowTime;
			if( t > SAVING_THROW_TIME )
			{
				player->lastSavingThrowTime = gameLocal.time;
				t = 0;
			}
			if( t < 1000 )
			{
				gameLocal.Printf( "Saving throw.\n" );
				forceMiss = true;
			}
		}
	}

	// make sure the trace can actually hit the enemy
	if( forceMiss || !TestMelee() )
	{
		// missed
		p = meleeDef->GetString( "snd_miss" );
		if( p != NULL && *p != '\0' )
		{
			shader = declManager->FindSound( p );
			StartSoundShader( shader, SND_CHANNEL_DAMAGE, 0, false, NULL );
		}
		return false;
	}

	//
	// do the damage
	//
	p = meleeDef->GetString( "snd_hit" );
	if( p != NULL && *p != '\0' )
	{
		shader = declManager->FindSound( p );
		StartSoundShader( shader, SND_CHANNEL_DAMAGE, 0, false, NULL );
	}

	idVec3	kickDir;
	meleeDef->GetVector( "kickDir", "0 0 0", kickDir );

	idVec3	globalKickDir;
	globalKickDir = ( viewAxis * physicsObj.GetGravityAxis() ) * kickDir;

	enemyEnt->Damage( this, this, globalKickDir, meleeDefName, 1.0f, INVALID_JOINT );

	lastAttackTime = gameLocal.time;

	return true;
}

/*
================
idAI::PushWithAF
================
*/
void idAI::PushWithAF()
{
	int i, j;
	afTouch_t touchList[ MAX_GENTITIES ];
	idEntity* pushed_ents[ MAX_GENTITIES ];
	idEntity* ent;
	idVec3 vel;
	int num_pushed;

	num_pushed = 0;
	af.ChangePose( this, gameLocal.time );
	int num = af.EntitiesTouchingAF( touchList );
	for( i = 0; i < num; i++ )
	{
		if( touchList[ i ].touchedEnt->IsType( idProjectile::Type ) )
		{
			// skip projectiles
			continue;
		}

		// make sure we havent pushed this entity already.  this avoids causing double damage
		for( j = 0; j < num_pushed; j++ )
		{
			if( pushed_ents[ j ] == touchList[ i ].touchedEnt )
			{
				break;
			}
		}
		if( j >= num_pushed )
		{
			ent = touchList[ i ].touchedEnt;
			pushed_ents[num_pushed++] = ent;
			vel = ent->GetPhysics()->GetAbsBounds().GetCenter() - touchList[ i ].touchedByBody->GetWorldOrigin();
			vel.Normalize();
			if( attack.Length() && ent->IsType( idActor::Type ) )
			{
				ent->Damage( this, this, vel, attack, 1.0f, INVALID_JOINT );
			}
			else
			{
				ent->GetPhysics()->SetLinearVelocity( 100.0f * vel, touchList[ i ].touchedClipModel->GetId() );
			}
		}
	}
}

/***********************************************************************

	Misc

***********************************************************************/

/*
================
idAI::GetMuzzle
================
*/
void idAI::GetMuzzle( const char* jointname, idVec3& muzzle, idMat3& axis )
{
	jointHandle_t joint;

	if( !jointname || !jointname[ 0 ] )
	{
		muzzle = physicsObj.GetOrigin() + viewAxis[ 0 ] * physicsObj.GetGravityAxis() * 14;
		muzzle -= physicsObj.GetGravityNormal() * physicsObj.GetBounds()[ 1 ].z * 0.5f;
	}
	else
	{
		joint = animator.GetJointHandle( jointname );
		if( joint == INVALID_JOINT )
		{
			gameLocal.Error( "Unknown joint '%s' on %s", jointname, GetEntityDefName() );
		}
		GetJointWorldTransform( joint, gameLocal.time, muzzle, axis );
	}
}

/*
================
idAI::TriggerWeaponEffects
================
*/
void idAI::TriggerWeaponEffects( const idVec3& muzzle )
{
	idVec3 org;
	idMat3 axis;

	if( !g_muzzleFlash.GetBool() )
	{
		return;
	}

	// muzzle flash
	// offset the shader parms so muzzle flashes show up
	renderEntity.shaderParms[SHADERPARM_TIMEOFFSET] = -MS2SEC( gameLocal.time );
	renderEntity.shaderParms[ SHADERPARM_DIVERSITY ] = gameLocal.random.CRandomFloat();

	if( flashJointWorld != INVALID_JOINT )
	{
		GetJointWorldTransform( flashJointWorld, gameLocal.time, org, axis );

		if( worldMuzzleFlash.lightRadius.x > 0.0f )
		{
			worldMuzzleFlash.origin = org;
			worldMuzzleFlash.axis = axis;
			worldMuzzleFlash.shaderParms[SHADERPARM_TIMEOFFSET] = -MS2SEC( gameLocal.time );
			if( worldMuzzleFlashHandle != - 1 )
			{
				gameRenderWorld->UpdateLightDef( worldMuzzleFlashHandle, &worldMuzzleFlash );
			}
			else
			{
				worldMuzzleFlashHandle = gameRenderWorld->AddLightDef( &worldMuzzleFlash );
			}
			muzzleFlashEnd = gameLocal.time + flashTime;
			UpdateVisuals();
		}
	}
}

/*
================
idAI::UpdateMuzzleFlash
================
*/
void idAI::UpdateMuzzleFlash()
{
	if( worldMuzzleFlashHandle != -1 )
	{
		if( gameLocal.time >= muzzleFlashEnd )
		{
			gameRenderWorld->FreeLightDef( worldMuzzleFlashHandle );
			worldMuzzleFlashHandle = -1;
		}
		else
		{
			idVec3 muzzle;
			animator.GetJointTransform( flashJointWorld, gameLocal.time, muzzle, worldMuzzleFlash.axis );
			muzzle = physicsObj.GetOrigin() + ( muzzle + modelOffset ) * viewAxis * physicsObj.GetGravityAxis();
			worldMuzzleFlash.origin = muzzle;
			gameRenderWorld->UpdateLightDef( worldMuzzleFlashHandle, &worldMuzzleFlash );
		}
	}
}

/*
================
idAI::Hide
================
*/
void idAI::Hide()
{
	idActor::Hide();
	fl.takedamage = false;
	physicsObj.SetContents( 0 );
	physicsObj.GetClipModel()->Unlink();
	StopSound( SND_CHANNEL_AMBIENT, false );
	SetChatSound();

	AI_ENEMY_IN_FOV		= false;
	AI_ENEMY_VISIBLE	= false;
	StopMove( MOVE_STATUS_DONE );
}

/*
================
idAI::Show
================
*/
void idAI::Show()
{
	idActor::Show();
	if( spawnArgs.GetBool( "big_monster" ) )
	{
		physicsObj.SetContents( 0 );
	}
	else if( use_combat_bbox )
	{
		physicsObj.SetContents( CONTENTS_BODY | CONTENTS_SOLID );
	}
	else
	{
		physicsObj.SetContents( CONTENTS_BODY );
	}
	physicsObj.GetClipModel()->Link( gameLocal.clip );
	fl.takedamage = !spawnArgs.GetBool( "noDamage" );
	SetChatSound();
	StartSound( "snd_ambient", SND_CHANNEL_AMBIENT, 0, false, NULL );
}

/*
=====================
idAI::SetChatSound
=====================
*/
void idAI::SetChatSound()
{
	const char* snd;

	if( IsHidden() )
	{
		snd = NULL;
	}
	else if( enemy.GetEntity() )
	{
		snd = spawnArgs.GetString( "snd_chatter_combat", NULL );
		chat_min = SEC2MS( spawnArgs.GetFloat( "chatter_combat_min", "5" ) );
		chat_max = SEC2MS( spawnArgs.GetFloat( "chatter_combat_max", "10" ) );
	}
	else if( !spawnArgs.GetBool( "no_idle_chatter" ) )
	{
		snd = spawnArgs.GetString( "snd_chatter", NULL );
		chat_min = SEC2MS( spawnArgs.GetFloat( "chatter_min", "5" ) );
		chat_max = SEC2MS( spawnArgs.GetFloat( "chatter_max", "10" ) );
	}
	else
	{
		snd = NULL;
	}

	if( snd != NULL && *snd != '\0' )
	{
		chat_snd = declManager->FindSound( snd );

		// set the next chat time
		chat_time = gameLocal.time + chat_min + gameLocal.random.RandomFloat() * ( chat_max - chat_min );
	}
	else
	{
		chat_snd = NULL;
	}
}

/*
================
idAI::CanPlayChatterSounds

Used for playing chatter sounds on monsters.
================
*/
bool idAI::CanPlayChatterSounds() const
{
	if( AI_DEAD )
	{
		return false;
	}

	if( IsHidden() )
	{
		return false;
	}

	if( enemy.GetEntity() )
	{
		return true;
	}

	if( spawnArgs.GetBool( "no_idle_chatter" ) )
	{
		return false;
	}

	return true;
}

/*
=====================
idAI::PlayChatter
=====================
*/
void idAI::PlayChatter()
{
	// check if it's time to play a chat sound
	if( AI_DEAD || !chat_snd || ( chat_time > gameLocal.time ) )
	{
		return;
	}

	StartSoundShader( chat_snd, SND_CHANNEL_VOICE, 0, false, NULL );

	// set the next chat time
	chat_time = gameLocal.time + chat_min + gameLocal.random.RandomFloat() * ( chat_max - chat_min );
}

/*
=====================
idAI::UpdateParticles
=====================
*/
void idAI::UpdateParticles()
{
	if( ( thinkFlags & TH_UPDATEPARTICLES ) && !IsHidden() )
	{
		idVec3 realVector;
		idMat3 realAxis;

		int particlesAlive = 0;
		for( int i = 0; i < particles.Num(); i++ )
		{
			// Smoke particles on AI characters will always be "slow", even when held by grabber
			SetTimeState ts( TIME_GROUP1 );
			if( particles[i].particle && particles[i].time )
			{
				particlesAlive++;
				if( af.IsActive() )
				{
					realAxis = mat3_identity;
					realVector = GetPhysics()->GetOrigin();
				}
				else
				{
					animator.GetJointTransform( particles[i].joint, gameLocal.time, realVector, realAxis );
					realAxis *= renderEntity.axis;
					realVector = physicsObj.GetOrigin() + ( realVector + modelOffset ) * ( viewAxis * physicsObj.GetGravityAxis() );
				}

				if( !gameLocal.smokeParticles->EmitSmoke( particles[i].particle, particles[i].time, gameLocal.random.CRandomFloat(), realVector, realAxis, timeGroup /*_D3XP*/ ) )
				{
					if( restartParticles )
					{
						particles[i].time = gameLocal.time;
					}
					else
					{
						particles[i].time = 0;
						particlesAlive--;
					}
				}
			}
		}
		if( particlesAlive == 0 )
		{
			BecomeInactive( TH_UPDATEPARTICLES );
		}
	}
}

/*
=====================
idAI::TriggerParticles
=====================
*/
void idAI::TriggerParticles( const char* jointName )
{
	jointHandle_t jointNum;

	jointNum = animator.GetJointHandle( jointName );
	for( int i = 0; i < particles.Num(); i++ )
	{
		if( particles[i].joint == jointNum )
		{
			particles[i].time = gameLocal.time;
			BecomeActive( TH_UPDATEPARTICLES );
		}
	}
}

// jmarshall begin
/*
================
idActor::ConstructScriptObject

Called during idEntity::Spawn.  Calls the constructor on the script object.
Can be overridden by subclasses when a thread doesn't need to be allocated.
================
*/
idThread* idAI::ConstructScriptObject()
{
	// make sure we have a scriptObject
	if( !scriptObject.HasObject() )
	{
		gameLocal.Error( "No scriptobject set on '%s'.  Check the '%s' entityDef.", name.c_str(), GetEntityDefName() );
	}

	if( !scriptThread )
	{
		// create script thread
		scriptThread = new idThread();
		scriptThread->ManualDelete();
		scriptThread->ManualControl();
		scriptThread->SetThreadName( name.c_str() );
	}
	else
	{
		scriptThread->EndThread();
	}

	return scriptThread;
}

/*
================
CallConstructor
================
*/
void idAI::CallConstructor()
{
	const function_t* constructor;

	// call script object's constructor
	constructor = scriptObject.GetConstructor();
	if( constructor )
	{
		//gameLocal.Error( "Missing constructor on '%s' for entity '%s'", scriptObject.GetTypeName(), name.c_str() );
		// init the script object's data
		scriptObject.ClearObject();

		// just set the current function on the script.  we'll execute in the subclasses.
		scriptThread->CallFunction( this, constructor, true );
	}

	AI_Begin();
}
// jmarshall end

void idAI::TriggerFX( const char* joint, const char* fx )
{

	if( !strcmp( joint, "origin" ) )
	{
		idEntityFx::StartFx( fx, NULL, NULL, this, true );
	}
	else
	{
		idVec3	joint_origin;
		idMat3	joint_axis;
		jointHandle_t jointNum;
		jointNum = animator.GetJointHandle( joint );

		if( jointNum == INVALID_JOINT )
		{
			gameLocal.Warning( "Unknown fx joint '%s' on entity %s", joint, name.c_str() );
			return;
		}

		GetJointWorldTransform( jointNum, gameLocal.time, joint_origin, joint_axis );
		idEntityFx::StartFx( fx, &joint_origin, &joint_axis, this, true );
	}
}

idEntity* idAI::StartEmitter( const char* name, const char* joint, const char* particle )
{

	idEntity* existing = GetEmitter( name );
	if( existing )
	{
		return existing;
	}

	jointHandle_t jointNum;
	jointNum = animator.GetJointHandle( joint );

	idVec3 offset;
	idMat3 axis;

	GetJointWorldTransform( jointNum, gameLocal.time, offset, axis );

	/*animator.GetJointTransform( jointNum, gameLocal.time, offset, axis );
	offset = GetPhysics()->GetOrigin() + offset * GetPhysics()->GetAxis();
	axis = axis * GetPhysics()->GetAxis();*/



	idDict args;

	const idDeclEntityDef* emitterDef = gameLocal.FindEntityDef( "func_emitter", false );
	args = emitterDef->dict;
	args.Set( "model", particle );
	args.Set( "origin", offset.ToString() );
	args.SetBool( "start_off", true );

	idEntity* ent;
	gameLocal.SpawnEntityDef( args, &ent, false );

	ent->GetPhysics()->SetOrigin( offset );
	//ent->GetPhysics()->SetAxis(axis);

	// align z-axis of model with the direction
	/*idVec3		tmp;
	axis = (viewAxis[ 0 ] * physicsObj.GetGravityAxis()).ToMat3();
	tmp = axis[2];
	axis[2] = axis[0];
	axis[0] = -tmp;

	ent->GetPhysics()->SetAxis(axis);*/

	axis = physicsObj.GetGravityAxis();
	ent->GetPhysics()->SetAxis( axis );


	ent->GetPhysics()->GetClipModel()->SetOwner( this );


	//Keep a reference to the emitter so we can track it
	funcEmitter_t newEmitter;
	strcpy( newEmitter.name, name );
	newEmitter.particle = ( idFuncEmitter* )ent;
	newEmitter.joint = jointNum;
	funcEmitters.Set( newEmitter.name, newEmitter );

	//Bind it to the joint and make it active
	newEmitter.particle->BindToJoint( this, jointNum, true );
	newEmitter.particle->BecomeActive( TH_THINK );
	newEmitter.particle->Show();
	newEmitter.particle->PostEventMS( &EV_Activate, 0, this );
	return newEmitter.particle;
}

idEntity* idAI::GetEmitter( const char* name )
{
	funcEmitter_t* emitter;
	funcEmitters.Get( name, &emitter );
	if( emitter )
	{
		return emitter->particle;
	}
	return NULL;
}

void idAI::StopEmitter( const char* name )
{
	funcEmitter_t* emitter;
	funcEmitters.Get( name, &emitter );
	if( emitter )
	{
		emitter->particle->Unbind();
		emitter->particle->PostEventMS( &EV_Remove, 0 );
		funcEmitters.Remove( name );
	}
}



/***********************************************************************

	Head & torso aiming

***********************************************************************/

/*
================
idAI::UpdateAnimationControllers
================
*/
bool idAI::UpdateAnimationControllers()
{
	idVec3		local;
	idVec3		focusPos;
	idQuat		jawQuat;
	idVec3		left;
	idVec3 		dir;
	idVec3 		orientationJointPos;
	idVec3 		localDir;
	idAngles 	newLookAng;
	idAngles	diff;
	idMat3		mat;
	idMat3		axis;
	idMat3		orientationJointAxis;
	idAFAttachment*	headEnt = head.GetEntity();
	idVec3		eyepos;
	idVec3		pos;
	int			i;
	idAngles	jointAng;
	float		orientationJointYaw;

	if( AI_DEAD )
	{
		return idActor::UpdateAnimationControllers();
	}

	if( orientationJoint == INVALID_JOINT )
	{
		orientationJointAxis = viewAxis;
		orientationJointPos = physicsObj.GetOrigin();
		orientationJointYaw = current_yaw;
	}
	else
	{
		GetJointWorldTransform( orientationJoint, gameLocal.time, orientationJointPos, orientationJointAxis );
		orientationJointYaw = orientationJointAxis[ 2 ].ToYaw();
		orientationJointAxis = idAngles( 0.0f, orientationJointYaw, 0.0f ).ToMat3();
	}

	if( focusJoint != INVALID_JOINT )
	{
		if( headEnt )
		{
			headEnt->GetJointWorldTransform( focusJoint, gameLocal.time, eyepos, axis );
		}
		else
		{
			GetJointWorldTransform( focusJoint, gameLocal.time, eyepos, axis );
		}
		eyeOffset.z = eyepos.z - physicsObj.GetOrigin().z;
		if( ai_debugMove.GetBool() )
		{
			gameRenderWorld->DebugLine( colorRed, eyepos, eyepos + orientationJointAxis[ 0 ] * 32.0f, 1 );
		}
	}
	else
	{
		eyepos = GetEyePosition();
	}

	if( headEnt )
	{
		CopyJointsFromBodyToHead();
	}

	// Update the IK after we've gotten all the joint positions we need, but before we set any joint positions.
	// Getting the joint positions causes the joints to be updated.  The IK gets joint positions itself (which
	// are already up to date because of getting the joints in this function) and then sets their positions, which
	// forces the heirarchy to be updated again next time we get a joint or present the model.  If IK is enabled,
	// or if we have a separate head, we end up transforming the joints twice per frame.  Characters with no
	// head entity and no ik will only transform their joints once.  Set g_debuganim to the current entity number
	// in order to see how many times an entity transforms the joints per frame.
	idActor::UpdateAnimationControllers();

	idEntity* focusEnt = focusEntity.GetEntity();
	if( !allowJointMod || !allowEyeFocus || ( gameLocal.time >= focusTime ) )
	{
		focusPos = GetEyePosition() + orientationJointAxis[ 0 ] * 512.0f;
	}
	else if( focusEnt == NULL )
	{
		// keep looking at last position until focusTime is up
		focusPos = currentFocusPos;
	}
	else if( focusEnt == enemy.GetEntity() )
	{
		focusPos = lastVisibleEnemyPos + lastVisibleEnemyEyeOffset - eyeVerticalOffset * enemy.GetEntity()->GetPhysics()->GetGravityNormal();
	}
	else if( focusEnt->IsType( idActor::Type ) )
	{
		focusPos = static_cast<idActor*>( focusEnt )->GetEyePosition() - eyeVerticalOffset * focusEnt->GetPhysics()->GetGravityNormal();
	}
	else
	{
		focusPos = focusEnt->GetPhysics()->GetOrigin();
	}

	currentFocusPos = currentFocusPos + ( focusPos - currentFocusPos ) * eyeFocusRate;

	// determine yaw from origin instead of from focus joint since joint may be offset, which can cause us to bounce between two angles
	dir = focusPos - orientationJointPos;
	newLookAng.yaw = idMath::AngleNormalize180( dir.ToYaw() - orientationJointYaw );
	newLookAng.roll = 0.0f;
	newLookAng.pitch = 0.0f;

#if 0
	gameRenderWorld->DebugLine( colorRed, orientationJointPos, focusPos, 1 );
	gameRenderWorld->DebugLine( colorYellow, orientationJointPos, orientationJointPos + orientationJointAxis[ 0 ] * 32.0f, 1 );
	gameRenderWorld->DebugLine( colorGreen, orientationJointPos, orientationJointPos + newLookAng.ToForward() * 48.0f, 1 );
#endif

	// determine pitch from joint position
	dir = focusPos - eyepos;
	dir.NormalizeFast();
	orientationJointAxis.ProjectVector( dir, localDir );
	newLookAng.pitch = -idMath::AngleNormalize180( localDir.ToPitch() );
	newLookAng.roll	= 0.0f;

	diff = newLookAng - lookAng;

	if( eyeAng != diff )
	{
		eyeAng = diff;
		eyeAng.Clamp( eyeMin, eyeMax );
		idAngles angDelta = diff - eyeAng;
		if( !angDelta.Compare( ang_zero, 0.1f ) )
		{
			alignHeadTime = gameLocal.time;
		}
		else
		{
			alignHeadTime = gameLocal.time + ( 0.5f + 0.5f * gameLocal.random.RandomFloat() ) * focusAlignTime;
		}
	}

	if( idMath::Fabs( newLookAng.yaw ) < 0.1f )
	{
		alignHeadTime = gameLocal.time;
	}

	if( ( gameLocal.time >= alignHeadTime ) || ( gameLocal.time < forceAlignHeadTime ) )
	{
		alignHeadTime = gameLocal.time + ( 0.5f + 0.5f * gameLocal.random.RandomFloat() ) * focusAlignTime;
		destLookAng = newLookAng;
		destLookAng.Clamp( lookMin, lookMax );
	}

	diff = destLookAng - lookAng;
	if( ( lookMin.pitch == -180.0f ) && ( lookMax.pitch == 180.0f ) )
	{
		if( ( diff.pitch > 180.0f ) || ( diff.pitch <= -180.0f ) )
		{
			diff.pitch = 360.0f - diff.pitch;
		}
	}
	if( ( lookMin.yaw == -180.0f ) && ( lookMax.yaw == 180.0f ) )
	{
		if( diff.yaw > 180.0f )
		{
			diff.yaw -= 360.0f;
		}
		else if( diff.yaw <= -180.0f )
		{
			diff.yaw += 360.0f;
		}
	}
	lookAng = lookAng + diff * headFocusRate;
	lookAng.Normalize180();

	jointAng.roll = 0.0f;
	for( i = 0; i < lookJoints.Num(); i++ )
	{
		jointAng.pitch	= lookAng.pitch * lookJointAngles[ i ].pitch;
		jointAng.yaw	= lookAng.yaw * lookJointAngles[ i ].yaw;
		animator.SetJointAxis( lookJoints[ i ], JOINTMOD_WORLD, jointAng.ToMat3() );
	}

	if( move.moveType == MOVETYPE_FLY )
	{
		// lean into turns
		AdjustFlyingAngles();
	}

	if( headEnt )
	{
		idAnimator* headAnimator = headEnt->GetAnimator();

		if( allowEyeFocus )
		{
			idMat3 eyeAxis = ( lookAng + eyeAng ).ToMat3();
			idMat3 headTranspose = headEnt->GetPhysics()->GetAxis().Transpose();
			axis =  eyeAxis * orientationJointAxis;
			left = axis[ 1 ] * eyeHorizontalOffset;
			eyepos -= headEnt->GetPhysics()->GetOrigin();
			headAnimator->SetJointPos( leftEyeJoint, JOINTMOD_WORLD_OVERRIDE, eyepos + ( axis[ 0 ] * 64.0f + left ) * headTranspose );
			headAnimator->SetJointPos( rightEyeJoint, JOINTMOD_WORLD_OVERRIDE, eyepos + ( axis[ 0 ] * 64.0f - left ) * headTranspose );
		}
		else
		{
			headAnimator->ClearJoint( leftEyeJoint );
			headAnimator->ClearJoint( rightEyeJoint );
		}
	}
	else
	{
		if( allowEyeFocus )
		{
			idMat3 eyeAxis = ( lookAng + eyeAng ).ToMat3();
			axis =  eyeAxis * orientationJointAxis;
			left = axis[ 1 ] * eyeHorizontalOffset;
			eyepos += axis[ 0 ] * 64.0f - physicsObj.GetOrigin();
			animator.SetJointPos( leftEyeJoint, JOINTMOD_WORLD_OVERRIDE, eyepos + left );
			animator.SetJointPos( rightEyeJoint, JOINTMOD_WORLD_OVERRIDE, eyepos - left );
		}
		else
		{
			animator.ClearJoint( leftEyeJoint );
			animator.ClearJoint( rightEyeJoint );
		}
	}

	return true;
}

/***********************************************************************

idCombatNode

***********************************************************************/

const idEventDef EV_CombatNode_MarkUsed( "markUsed" );

CLASS_DECLARATION( idEntity, idCombatNode )
EVENT( EV_CombatNode_MarkUsed,				idCombatNode::Event_MarkUsed )
EVENT( EV_Activate,							idCombatNode::Event_Activate )
END_CLASS

/*
=====================
idCombatNode::idCombatNode
=====================
*/
idCombatNode::idCombatNode()
{
	min_dist = 0.0f;
	max_dist = 0.0f;
	cone_dist = 0.0f;
	min_height = 0.0f;
	max_height = 0.0f;
	cone_left.Zero();
	cone_right.Zero();
	offset.Zero();
	disabled = false;
}

/*
=====================
idCombatNode::Save
=====================
*/
void idCombatNode::Save( idSaveGame* savefile ) const
{
	savefile->WriteFloat( min_dist );
	savefile->WriteFloat( max_dist );
	savefile->WriteFloat( cone_dist );
	savefile->WriteFloat( min_height );
	savefile->WriteFloat( max_height );
	savefile->WriteVec3( cone_left );
	savefile->WriteVec3( cone_right );
	savefile->WriteVec3( offset );
	savefile->WriteBool( disabled );
}

/*
=====================
idCombatNode::Restore
=====================
*/
void idCombatNode::Restore( idRestoreGame* savefile )
{
	savefile->ReadFloat( min_dist );
	savefile->ReadFloat( max_dist );
	savefile->ReadFloat( cone_dist );
	savefile->ReadFloat( min_height );
	savefile->ReadFloat( max_height );
	savefile->ReadVec3( cone_left );
	savefile->ReadVec3( cone_right );
	savefile->ReadVec3( offset );
	savefile->ReadBool( disabled );
}

/*
=====================
idCombatNode::Spawn
=====================
*/
void idCombatNode::Spawn()
{
	float fov;
	float yaw;
	float height;

	min_dist = spawnArgs.GetFloat( "min" );
	max_dist = spawnArgs.GetFloat( "max" );
	height = spawnArgs.GetFloat( "height" );
	fov = spawnArgs.GetFloat( "fov", "60" );
	offset = spawnArgs.GetVector( "offset" );

	const idVec3& org = GetPhysics()->GetOrigin() + offset;
	min_height = org.z - height * 0.5f;
	max_height = min_height + height;

	const idMat3& axis = GetPhysics()->GetAxis();
	yaw = axis[ 0 ].ToYaw();

	idAngles leftang( 0.0f, yaw + fov * 0.5f - 90.0f, 0.0f );
	cone_left = leftang.ToForward();

	idAngles rightang( 0.0f, yaw - fov * 0.5f + 90.0f, 0.0f );
	cone_right = rightang.ToForward();

	disabled = spawnArgs.GetBool( "start_off" );
}

/*
=====================
idCombatNode::IsDisabled
=====================
*/
bool idCombatNode::IsDisabled() const
{
	return disabled;
}

/*
=====================
idCombatNode::DrawDebugInfo
=====================
*/
void idCombatNode::DrawDebugInfo()
{
	idEntity*		ent;
	idCombatNode*	node;
	idPlayer*		player = gameLocal.GetLocalPlayer();
	idVec4			color;
	idBounds		bounds( idVec3( -16, -16, 0 ), idVec3( 16, 16, 0 ) );

	for( ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next() )
	{
		if( !ent->IsType( idCombatNode::Type ) )
		{
			continue;
		}

		node = static_cast<idCombatNode*>( ent );
		if( node->disabled )
		{
			color = colorMdGrey;
		}
		else if( player != NULL && node->EntityInView( player, player->GetPhysics()->GetOrigin() ) )
		{
			color = colorYellow;
		}
		else
		{
			color = colorRed;
		}

		idVec3 leftDir( -node->cone_left.y, node->cone_left.x, 0.0f );
		idVec3 rightDir( node->cone_right.y, -node->cone_right.x, 0.0f );
		idVec3 org = node->GetPhysics()->GetOrigin() + node->offset;

		bounds[ 1 ].z = node->max_height;

		leftDir.NormalizeFast();
		rightDir.NormalizeFast();

		const idMat3& axis = node->GetPhysics()->GetAxis();
		float cone_dot = node->cone_right * axis[ 1 ];
		if( idMath::Fabs( cone_dot ) > 0.1 )
		{
			float cone_dist = node->max_dist / cone_dot;
			idVec3 pos1 = org + leftDir * node->min_dist;
			idVec3 pos2 = org + leftDir * cone_dist;
			idVec3 pos3 = org + rightDir * node->min_dist;
			idVec3 pos4 = org + rightDir * cone_dist;

			gameRenderWorld->DebugLine( color, node->GetPhysics()->GetOrigin(), ( pos1 + pos3 ) * 0.5f, 1 );
			gameRenderWorld->DebugLine( color, pos1, pos2, 1 );
			gameRenderWorld->DebugLine( color, pos1, pos3, 1 );
			gameRenderWorld->DebugLine( color, pos3, pos4, 1 );
			gameRenderWorld->DebugLine( color, pos2, pos4, 1 );
			gameRenderWorld->DebugBounds( color, bounds, org, 1 );
		}
	}
}

/*
=====================
idCombatNode::EntityInView
=====================
*/
bool idCombatNode::EntityInView( idActor* actor, const idVec3& pos )
{
	if( !actor || ( actor->health <= 0 ) )
	{
		return false;
	}

	const idBounds& bounds = actor->GetPhysics()->GetBounds();
	if( ( pos.z + bounds[ 1 ].z < min_height ) || ( pos.z + bounds[ 0 ].z >= max_height ) )
	{
		return false;
	}

	const idVec3& org = GetPhysics()->GetOrigin() + offset;
	const idMat3& axis = GetPhysics()->GetAxis();
	idVec3 dir = pos - org;
	float  dist = dir * axis[ 0 ];

	if( ( dist < min_dist ) || ( dist > max_dist ) )
	{
		return false;
	}

	float left_dot = dir * cone_left;
	if( left_dot < 0.0f )
	{
		return false;
	}

	float right_dot = dir * cone_right;
	if( right_dot < 0.0f )
	{
		return false;
	}

	return true;
}

/*
=====================
idCombatNode::Event_Activate
=====================
*/
void idCombatNode::Event_Activate( idEntity* activator )
{
	disabled = !disabled;
}

/*
=====================
idCombatNode::Event_MarkUsed
=====================
*/
void idCombatNode::Event_MarkUsed()
{
	if( spawnArgs.GetBool( "use_once" ) )
	{
		disabled = true;
	}
}
