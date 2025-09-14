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
#include "AASCallback_FindAreaOutOfRange.h"
#include "AASCallback_FindCoverArea.h"
#include "AASCallback_FindAttackPosition.h"


static const char* moveCommandString[NUM_MOVE_COMMANDS] =
{
	"MOVE_NONE",
	"MOVE_FACE_ENEMY",
	"MOVE_FACE_ENTITY",
	"MOVE_TO_ENEMY",
	"MOVE_TO_ENEMYHEIGHT",
	"MOVE_TO_ENTITY",
	"MOVE_OUT_OF_RANGE",
	"MOVE_TO_ATTACK_POSITION",
	"MOVE_TO_COVER",
	"MOVE_TO_POSITION",
	"MOVE_TO_POSITION_DIRECT",
	"MOVE_SLIDE_TO_POSITION",
	"MOVE_WANDER"
};

/*
=====================
idMoveState::idMoveState
=====================
*/
idMoveState::idMoveState()
{
	moveType = MOVETYPE_ANIM;
	moveCommand = MOVE_NONE;
	moveStatus = MOVE_STATUS_DONE;
	moveDest.Zero();
	moveDir.Set( 1.0f, 0.0f, 0.0f );
	goalEntity = NULL;
	goalEntityOrigin.Zero();
	toAreaNum = 0;
	startTime = 0;
	duration = 0;
	speed = 0.0f;
	range = 0.0f;
	wanderYaw = 0;
	nextWanderTime = 0;
	blockTime = 0;
	obstacle = NULL;
	lastMoveOrigin = vec3_origin;
	lastMoveTime = 0;
	anim = 0;
}

/*
=====================
idMoveState::Save
=====================
*/
void idMoveState::Save( idSaveGame* savefile ) const
{
	savefile->WriteInt( ( int )moveType );
	savefile->WriteInt( ( int )moveCommand );
	savefile->WriteInt( ( int )moveStatus );
	savefile->WriteVec3( moveDest );
	savefile->WriteVec3( moveDir );
	goalEntity.Save( savefile );
	savefile->WriteVec3( goalEntityOrigin );
	savefile->WriteInt( toAreaNum );
	savefile->WriteInt( startTime );
	savefile->WriteInt( duration );
	savefile->WriteFloat( speed );
	savefile->WriteFloat( range );
	savefile->WriteFloat( wanderYaw );
	savefile->WriteInt( nextWanderTime );
	savefile->WriteInt( blockTime );
	obstacle.Save( savefile );
	savefile->WriteVec3( lastMoveOrigin );
	savefile->WriteInt( lastMoveTime );
	savefile->WriteInt( anim );
}

/*
=====================
idMoveState::Restore
=====================
*/
void idMoveState::Restore( idRestoreGame* savefile )
{
	savefile->ReadInt( ( int& )moveType );
	savefile->ReadInt( ( int& )moveCommand );
	savefile->ReadInt( ( int& )moveStatus );
	savefile->ReadVec3( moveDest );
	savefile->ReadVec3( moveDir );
	goalEntity.Restore( savefile );
	savefile->ReadVec3( goalEntityOrigin );
	savefile->ReadInt( toAreaNum );
	savefile->ReadInt( startTime );
	savefile->ReadInt( duration );
	savefile->ReadFloat( speed );
	savefile->ReadFloat( range );
	savefile->ReadFloat( wanderYaw );
	savefile->ReadInt( nextWanderTime );
	savefile->ReadInt( blockTime );
	obstacle.Restore( savefile );
	savefile->ReadVec3( lastMoveOrigin );
	savefile->ReadInt( lastMoveTime );
	savefile->ReadInt( anim );
}


/*
=====================
idAI::Event_GetMoveType
=====================
*/
void idAI::Event_GetMoveType()
{
	idThread::ReturnInt( move.moveType );
}

/*
=====================
idAI::Event_SetMoveTypes
=====================
*/
void idAI::Event_SetMoveType( int moveType )
{
	if( ( moveType < 0 ) || ( moveType >= NUM_MOVETYPES ) )
	{
		gameLocal.Error( "Invalid movetype %d", moveType );
	}

	move.moveType = static_cast<moveType_t>( moveType );
	if( move.moveType == MOVETYPE_FLY )
	{
		travelFlags = TFL_WALK | TFL_AIR | TFL_FLY;
	}
	else
	{
		travelFlags = TFL_WALK | TFL_AIR;
	}
}


/*
=====================
idAI::Event_SaveMove
=====================
*/
void idAI::Event_SaveMove()
{
	savedMove = move;
}

/*
=====================
idAI::Event_RestoreMove
=====================
*/
void idAI::Event_RestoreMove()
{
	idVec3 goalPos;
	idVec3 dest;

	switch( savedMove.moveCommand )
	{
		case MOVE_NONE:
			StopMove( savedMove.moveStatus );
			break;

		case MOVE_FACE_ENEMY:
			FaceEnemy();
			break;

		case MOVE_FACE_ENTITY:
			FaceEntity( savedMove.goalEntity.GetEntity() );
			break;

		case MOVE_TO_ENEMY:
			MoveToEnemy();
			break;

		case MOVE_TO_ENEMYHEIGHT:
			MoveToEnemyHeight();
			break;

		case MOVE_TO_ENTITY:
			MoveToEntity( savedMove.goalEntity.GetEntity() );
			break;

		case MOVE_OUT_OF_RANGE:
			MoveOutOfRange( savedMove.goalEntity.GetEntity(), savedMove.range );
			break;

		case MOVE_TO_ATTACK_POSITION:
			MoveToAttackPosition( savedMove.goalEntity.GetEntity(), savedMove.anim );
			break;

		case MOVE_TO_COVER:
			MoveToCover( savedMove.goalEntity.GetEntity(), lastVisibleEnemyPos );
			break;

		case MOVE_TO_POSITION:
			MoveToPosition( savedMove.moveDest );
			break;

		case MOVE_TO_POSITION_DIRECT:
			DirectMoveToPosition( savedMove.moveDest );
			break;

		case MOVE_SLIDE_TO_POSITION:
			SlideToPosition( savedMove.moveDest, savedMove.duration );
			break;

		case MOVE_WANDER:
			WanderAround();
			break;
	}

	if( GetMovePos( goalPos ) )
	{
		CheckObstacleAvoidance( goalPos, dest );
	}
}

/*
=====================
idAI::Event_AllowMovement
=====================
*/
void idAI::Event_AllowMovement( float flag )
{
	allowMove = ( flag != 0.0f );
}


/***********************************************************************

	navigation

***********************************************************************/

/*
============
idAI::KickObstacles
============
*/
void idAI::KickObstacles( const idVec3& dir, float force, idEntity* alwaysKick )
{
	int i, numListedClipModels;
	idBounds clipBounds;
	idEntity* obEnt;
	idClipModel* clipModel;
	idClipModel* clipModelList[MAX_GENTITIES];
	int clipmask;
	idVec3 org;
	idVec3 forceVec;
	idVec3 delta;
	idVec2 perpendicular;

	org = physicsObj.GetOrigin();

	// find all possible obstacles
	clipBounds = physicsObj.GetAbsBounds();
	clipBounds.TranslateSelf( dir * 32.0f );
	clipBounds.ExpandSelf( 8.0f );
	clipBounds.AddPoint( org );
	clipmask = physicsObj.GetClipMask();
	numListedClipModels = gameLocal.clip.ClipModelsTouchingBounds( clipBounds, clipmask, clipModelList, MAX_GENTITIES );
	for( i = 0; i < numListedClipModels; i++ )
	{
		clipModel = clipModelList[i];
		obEnt = clipModel->GetEntity();
		if( obEnt == alwaysKick )
		{
			// we'll kick this one outside the loop
			continue;
		}

		if( !clipModel->IsTraceModel() )
		{
			continue;
		}

		if( obEnt->IsType( idMoveable::Type ) && obEnt->GetPhysics()->IsPushable() )
		{
			delta = obEnt->GetPhysics()->GetOrigin() - org;
			delta.NormalizeFast();
			perpendicular.x = -delta.y;
			perpendicular.y = delta.x;
			delta.z += 0.5f;
			delta.ToVec2() += perpendicular * gameLocal.random.CRandomFloat() * 0.5f;
			forceVec = delta * force * obEnt->GetPhysics()->GetMass();
			obEnt->ApplyImpulse( this, 0, obEnt->GetPhysics()->GetOrigin(), forceVec );
		}
	}

	if( alwaysKick )
	{
		delta = alwaysKick->GetPhysics()->GetOrigin() - org;
		delta.NormalizeFast();
		perpendicular.x = -delta.y;
		perpendicular.y = delta.x;
		delta.z += 0.5f;
		delta.ToVec2() += perpendicular * gameLocal.random.CRandomFloat() * 0.5f;
		forceVec = delta * force * alwaysKick->GetPhysics()->GetMass();
		alwaysKick->ApplyImpulse( this, 0, alwaysKick->GetPhysics()->GetOrigin(), forceVec );
	}
}

/*
============
ValidForBounds
============
*/
bool ValidForBounds( const idAASSettings* settings, const idBounds& bounds )
{
	int i;

	for( i = 0; i < 3; i++ )
	{
		if( bounds[0][i] < settings->boundingBoxes[0][0][i] )
		{
			return false;
		}
		if( bounds[1][i] > settings->boundingBoxes[0][1][i] )
		{
			return false;
		}
	}
	return true;
}

/*
=====================
idAI::SetAAS
=====================
*/
void idAI::SetAAS()
{
	idStr use_aas;

	spawnArgs.GetString( "use_aas", NULL, use_aas );
	aas = gameLocal.GetAAS( use_aas );
	if( aas )
	{
		const idAASSettings* settings = aas->GetSettings();
		if( settings )
		{
			if( !ValidForBounds( settings, physicsObj.GetBounds() ) )
			{
				gameLocal.Error( "%s cannot use use_aas %s\n", name.c_str(), use_aas.c_str() );
			}
			float height = settings->maxStepHeight;
			physicsObj.SetMaxStepHeight( height );
			return;
		}
		else
		{
			aas = NULL;
		}
	}
	gameLocal.Printf( "WARNING: %s has no AAS file\n", name.c_str() );
}

/*
=====================
idAI::DrawRoute
=====================
*/
void idAI::DrawRoute() const
{
	if( aas && move.toAreaNum && move.moveCommand != MOVE_NONE && move.moveCommand != MOVE_WANDER && move.moveCommand != MOVE_FACE_ENEMY && move.moveCommand != MOVE_FACE_ENTITY && move.moveCommand != MOVE_TO_POSITION_DIRECT )
	{
		if( move.moveType == MOVETYPE_FLY )
		{
			aas->ShowFlyPath( physicsObj.GetOrigin(), move.toAreaNum, move.moveDest );
		}
		else
		{
			aas->ShowWalkPath( physicsObj.GetOrigin(), move.toAreaNum, move.moveDest );
		}
	}
}

/*
=====================
idAI::ReachedPos
=====================
*/
bool idAI::ReachedPos( const idVec3& pos, const moveCommand_t moveCommand ) const
{
	if( move.moveType == MOVETYPE_SLIDE )
	{
		idBounds bnds( idVec3( -4, -4.0f, -8.0f ), idVec3( 4.0f, 4.0f, 64.0f ) );
		bnds.TranslateSelf( physicsObj.GetOrigin() );
		if( bnds.ContainsPoint( pos ) )
		{
			return true;
		}
	}
	else
	{
		if( ( moveCommand == MOVE_TO_ENEMY ) || ( moveCommand == MOVE_TO_ENTITY ) )
		{
			if( physicsObj.GetAbsBounds().IntersectsBounds( idBounds( pos ).Expand( 8.0f ) ) )
			{
				return true;
			}
		}
		else
		{
			idBounds bnds( idVec3( -16.0, -16.0f, -8.0f ), idVec3( 16.0, 16.0f, 64.0f ) );
			bnds.TranslateSelf( physicsObj.GetOrigin() );
			if( bnds.ContainsPoint( pos ) )
			{
				return true;
			}
		}
	}
	return false;
}

/*
=====================
idAI::PointReachableAreaNum
=====================
*/
int idAI::PointReachableAreaNum( const idVec3& pos, const float boundsScale ) const
{
	int areaNum;
	idVec3 size;
	idBounds bounds;

	if( !aas )
	{
		return 0;
	}

	size = aas->GetSettings()->boundingBoxes[0][1] * boundsScale;
	bounds[0] = -size;
	size.z = 32.0f;
	bounds[1] = size;

	if( move.moveType == MOVETYPE_FLY )
	{
		areaNum = aas->PointReachableAreaNum( pos, bounds, AREA_REACHABLE_WALK | AREA_REACHABLE_FLY );
	}
	else
	{
		areaNum = aas->PointReachableAreaNum( pos, bounds, AREA_REACHABLE_WALK );
	}

	return areaNum;
}

/*
=====================
idAI::PathToGoal
=====================
*/
bool idAI::PathToGoal( aasPath_t& path, int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin ) const
{
	idVec3 org;
	idVec3 goal;

	if( !aas )
	{
		return false;
	}

	org = origin;
	aas->PushPointIntoAreaNum( areaNum, org );
	if( !areaNum )
	{
		return false;
	}

	goal = goalOrigin;
	aas->PushPointIntoAreaNum( goalAreaNum, goal );
	if( !goalAreaNum )
	{
		return false;
	}

	if( move.moveType == MOVETYPE_FLY )
	{
		return aas->FlyPathToGoal( path, areaNum, org, goalAreaNum, goal, travelFlags );
	}
	else
	{
		return aas->WalkPathToGoal( path, areaNum, org, goalAreaNum, goal, travelFlags );
	}
}

/*
=====================
idAI::TravelDistance

Returns the approximate travel distance from one position to the goal, or if no AAS, the straight line distance.

This is feakin' slow, so it's not good to do it too many times per frame.  It also is slower the further you
are from the goal, so try to break the goals up into shorter distances.
=====================
*/
float idAI::TravelDistance( const idVec3& start, const idVec3& end ) const
{
	int			fromArea;
	int			toArea;
	float		dist;
	idVec2		delta;
	aasPath_t	path;

	if( !aas )
	{
		// no aas, so just take the straight line distance
		delta = end.ToVec2() - start.ToVec2();
		dist = delta.LengthFast();

		if( ai_debugMove.GetBool() )
		{
			gameRenderWorld->DebugLine( colorBlue, start, end, 1, false );
			gameRenderWorld->DrawText( va( "%d", ( int )dist ), ( start + end ) * 0.5f, 0.1f, colorWhite, gameLocal.GetLocalPlayer()->viewAngles.ToMat3() );
		}

		return dist;
	}

	fromArea = PointReachableAreaNum( start );
	toArea = PointReachableAreaNum( end );

	if( !fromArea || !toArea )
	{
		// can't seem to get there
		return -1;
	}

	if( fromArea == toArea )
	{
		// same area, so just take the straight line distance
		delta = end.ToVec2() - start.ToVec2();
		dist = delta.LengthFast();

		if( ai_debugMove.GetBool() )
		{
			gameRenderWorld->DebugLine( colorBlue, start, end, 1, false );
			gameRenderWorld->DrawText( va( "%d", ( int )dist ), ( start + end ) * 0.5f, 0.1f, colorWhite, gameLocal.GetLocalPlayer()->viewAngles.ToMat3() );
		}

		return dist;
	}

	idReachability* reach;
	int travelTime;
	if( !aas->RouteToGoalArea( fromArea, start, toArea, travelFlags, travelTime, &reach ) )
	{
		return -1;
	}

	if( ai_debugMove.GetBool() )
	{
		if( move.moveType == MOVETYPE_FLY )
		{
			aas->ShowFlyPath( start, toArea, end );
		}
		else
		{
			aas->ShowWalkPath( start, toArea, end );
		}
	}

	return travelTime;
}

/*
=====================
idAI::StopMove
=====================
*/
void idAI::StopMove( moveStatus_t status )
{
	AI_MOVE_DONE = true;
	AI_FORWARD = false;
	move.moveCommand = MOVE_NONE;
	move.moveStatus = status;
	move.toAreaNum = 0;
	move.goalEntity = NULL;
	move.moveDest = physicsObj.GetOrigin();
	AI_DEST_UNREACHABLE = false;
	AI_OBSTACLE_IN_PATH = false;
	AI_BLOCKED = false;
	move.startTime = gameLocal.time;
	move.duration = 0;
	move.range = 0.0f;
	move.speed = 0.0f;
	move.anim = 0;
	move.moveDir.Zero();
	move.lastMoveOrigin.Zero();
	move.lastMoveTime = gameLocal.time;
}


/*
=====================
idAI::FaceEnemy

Continually face the enemy's last known position.  MoveDone is always true in this case.
=====================
*/
bool idAI::FaceEnemy()
{
	idActor* enemyEnt = enemy.GetEntity();
	if( !enemyEnt )
	{
		StopMove( MOVE_STATUS_DEST_NOT_FOUND );
		return false;
	}

	TurnToward( lastVisibleEnemyPos );
	move.goalEntity = enemyEnt;
	move.moveDest = physicsObj.GetOrigin();
	move.moveCommand = MOVE_FACE_ENEMY;
	move.moveStatus = MOVE_STATUS_WAITING;
	move.startTime = gameLocal.time;
	move.speed = 0.0f;
	AI_MOVE_DONE = true;
	AI_FORWARD = false;
	AI_DEST_UNREACHABLE = false;

	return true;
}

/*
=====================
idAI::FaceEntity

Continually face the entity position.  MoveDone is always true in this case.
=====================
*/
bool idAI::FaceEntity( idEntity* ent )
{
	if( !ent )
	{
		StopMove( MOVE_STATUS_DEST_NOT_FOUND );
		return false;
	}

	idVec3 entityOrg = ent->GetPhysics()->GetOrigin();
	TurnToward( entityOrg );
	move.goalEntity = ent;
	move.moveDest = physicsObj.GetOrigin();
	move.moveCommand = MOVE_FACE_ENTITY;
	move.moveStatus = MOVE_STATUS_WAITING;
	move.startTime = gameLocal.time;
	move.speed = 0.0f;
	AI_MOVE_DONE = true;
	AI_FORWARD = false;
	AI_DEST_UNREACHABLE = false;

	return true;
}

/*
=====================
idAI::DirectMoveToPosition
=====================
*/
bool idAI::DirectMoveToPosition( const idVec3& pos )
{
	if( ReachedPos( pos, move.moveCommand ) )
	{
		StopMove( MOVE_STATUS_DONE );
		return true;
	}

	move.moveDest = pos;
	move.goalEntity = NULL;
	move.moveCommand = MOVE_TO_POSITION_DIRECT;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.startTime = gameLocal.time;
	move.speed = fly_speed;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = true;

	if( move.moveType == MOVETYPE_FLY )
	{
		idVec3 dir = pos - physicsObj.GetOrigin();
		dir.Normalize();
		dir *= fly_speed;
		physicsObj.SetLinearVelocity( dir );
	}

	return true;
}

/*
=====================
idAI::MoveToEnemyHeight
=====================
*/
bool idAI::MoveToEnemyHeight()
{
	idActor* enemyEnt = enemy.GetEntity();

	if( !enemyEnt || ( move.moveType != MOVETYPE_FLY ) )
	{
		StopMove( MOVE_STATUS_DEST_NOT_FOUND );
		return false;
	}

	move.moveDest.z = lastVisibleEnemyPos.z + enemyEnt->EyeOffset().z + fly_offset;
	move.goalEntity = enemyEnt;
	move.moveCommand = MOVE_TO_ENEMYHEIGHT;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.startTime = gameLocal.time;
	move.speed = 0.0f;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = false;

	return true;
}

/*
=====================
idAI::MoveToEnemy
=====================
*/
bool idAI::MoveToEnemy()
{
	int			areaNum;
	aasPath_t	path;
	idActor* enemyEnt = enemy.GetEntity();

	if( !enemyEnt )
	{
		StopMove( MOVE_STATUS_DEST_NOT_FOUND );
		return false;
	}

	if( ReachedPos( lastVisibleReachableEnemyPos, MOVE_TO_ENEMY ) )
	{
		if( !ReachedPos( lastVisibleEnemyPos, MOVE_TO_ENEMY ) || !AI_ENEMY_VISIBLE )
		{
			StopMove( MOVE_STATUS_DEST_UNREACHABLE );
			AI_DEST_UNREACHABLE = true;
			return false;
		}
		StopMove( MOVE_STATUS_DONE );
		return true;
	}

	idVec3 pos = lastVisibleReachableEnemyPos;

	move.toAreaNum = 0;
	if( aas )
	{
		move.toAreaNum = PointReachableAreaNum( pos );
		aas->PushPointIntoAreaNum( move.toAreaNum, pos );

		areaNum = PointReachableAreaNum( physicsObj.GetOrigin() );
		if( !PathToGoal( path, areaNum, physicsObj.GetOrigin(), move.toAreaNum, pos ) )
		{
			AI_DEST_UNREACHABLE = true;
			return false;
		}
	}

	if( !move.toAreaNum )
	{
		// if only trying to update the enemy position
		if( move.moveCommand == MOVE_TO_ENEMY )
		{
			if( !aas )
			{
				// keep the move destination up to date for wandering
				move.moveDest = pos;
			}
			return false;
		}

		if( !NewWanderDir( pos ) )
		{
			StopMove( MOVE_STATUS_DEST_UNREACHABLE );
			AI_DEST_UNREACHABLE = true;
			return false;
		}
	}

	if( move.moveCommand != MOVE_TO_ENEMY )
	{
		move.moveCommand = MOVE_TO_ENEMY;
		move.startTime = gameLocal.time;
	}

	move.moveDest = pos;
	move.goalEntity = enemyEnt;
	move.speed = fly_speed;
	move.moveStatus = MOVE_STATUS_MOVING;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = true;

	return true;
}

/*
=====================
idAI::MoveToEntity
=====================
*/
bool idAI::MoveToEntity( idEntity* ent )
{
	int			areaNum;
	aasPath_t	path;
	idVec3		pos;

	if( !ent )
	{
		StopMove( MOVE_STATUS_DEST_NOT_FOUND );
		return false;
	}

	pos = ent->GetPhysics()->GetOrigin();
	if( ( move.moveType != MOVETYPE_FLY ) && ( ( move.moveCommand != MOVE_TO_ENTITY ) || ( move.goalEntityOrigin != pos ) ) )
	{
		ent->GetFloorPos( 64.0f, pos );
	}

	if( ReachedPos( pos, MOVE_TO_ENTITY ) )
	{
		StopMove( MOVE_STATUS_DONE );
		return true;
	}

	move.toAreaNum = 0;
	if( aas )
	{
		move.toAreaNum = PointReachableAreaNum( pos );
		aas->PushPointIntoAreaNum( move.toAreaNum, pos );

		areaNum = PointReachableAreaNum( physicsObj.GetOrigin() );
		if( !PathToGoal( path, areaNum, physicsObj.GetOrigin(), move.toAreaNum, pos ) )
		{
			AI_DEST_UNREACHABLE = true;
			return false;
		}
	}

	if( !move.toAreaNum )
	{
		// if only trying to update the entity position
		if( move.moveCommand == MOVE_TO_ENTITY )
		{
			if( !aas )
			{
				// keep the move destination up to date for wandering
				move.moveDest = pos;
			}
			return false;
		}

		if( !NewWanderDir( pos ) )
		{
			StopMove( MOVE_STATUS_DEST_UNREACHABLE );
			AI_DEST_UNREACHABLE = true;
			return false;
		}
	}

	if( ( move.moveCommand != MOVE_TO_ENTITY ) || ( move.goalEntity.GetEntity() != ent ) )
	{
		move.startTime = gameLocal.time;
		move.goalEntity = ent;
		move.moveCommand = MOVE_TO_ENTITY;
	}

	move.moveDest = pos;
	move.goalEntityOrigin = ent->GetPhysics()->GetOrigin();
	move.moveStatus = MOVE_STATUS_MOVING;
	move.speed = fly_speed;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = true;

	return true;
}

/*
=====================
idAI::MoveOutOfRange
=====================
*/
bool idAI::MoveOutOfRange( idEntity* ent, float range )
{
	int				areaNum;
	aasObstacle_t	obstacle;
	aasGoal_t		goal;
	idBounds		bounds;
	idVec3			pos;

	if( !aas || !ent )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	const idVec3& org = physicsObj.GetOrigin();
	areaNum = PointReachableAreaNum( org );

	// consider the entity the monster is getting close to as an obstacle
	obstacle.absBounds = ent->GetPhysics()->GetAbsBounds();

	if( ent == enemy.GetEntity() )
	{
		pos = lastVisibleEnemyPos;
	}
	else
	{
		pos = ent->GetPhysics()->GetOrigin();
	}

	idAASCallback_FindAreaOutOfRange findGoal( pos, range );
	if( !aas->FindNearestGoal( goal, areaNum, org, pos, travelFlags, &obstacle, 1, findGoal ) )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	if( ReachedPos( goal.origin, move.moveCommand ) )
	{
		StopMove( MOVE_STATUS_DONE );
		return true;
	}

	move.moveDest = goal.origin;
	move.toAreaNum = goal.areaNum;
	move.goalEntity = ent;
	move.moveCommand = MOVE_OUT_OF_RANGE;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.range = range;
	move.speed = fly_speed;
	move.startTime = gameLocal.time;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = true;

	return true;
}

/*
=====================
idAI::MoveToAttackPosition
=====================
*/
bool idAI::MoveToAttackPosition( idEntity* ent, int attack_anim )
{
	int				areaNum;
	aasObstacle_t	obstacle;
	aasGoal_t		goal;
	idBounds		bounds;
	idVec3			pos;

	if( !aas || !ent )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	const idVec3& org = physicsObj.GetOrigin();
	areaNum	= PointReachableAreaNum( org );

	// consider the entity the monster is getting close to as an obstacle
	obstacle.absBounds = ent->GetPhysics()->GetAbsBounds();

	if( ent == enemy.GetEntity() )
	{
		pos = lastVisibleEnemyPos;
	}
	else
	{
		pos = ent->GetPhysics()->GetOrigin();
	}

	idAASCallback_FindAttackPosition findGoal( this, physicsObj.GetGravityAxis(), ent, pos, missileLaunchOffset[ attack_anim ] );
	if( !aas->FindNearestGoal( goal, areaNum, org, pos, travelFlags, &obstacle, 1, findGoal ) )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	move.moveDest		= goal.origin;
	move.toAreaNum		= goal.areaNum;
	move.goalEntity		= ent;
	move.moveCommand	= MOVE_TO_ATTACK_POSITION;
	move.moveStatus		= MOVE_STATUS_MOVING;
	move.speed			= fly_speed;
	move.startTime		= gameLocal.time;
	move.anim			= attack_anim;
	AI_MOVE_DONE		= false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD			= true;

	return true;
}

/*
=====================
idAI::MoveToPosition
=====================
*/
bool idAI::MoveToPosition( const idVec3& pos )
{
	idVec3		org;
	int			areaNum;
	aasPath_t	path;

	if( ReachedPos( pos, move.moveCommand ) )
	{
		StopMove( MOVE_STATUS_DONE );
		return true;
	}

	org = pos;
	move.toAreaNum = 0;
	if( aas )
	{
		move.toAreaNum = PointReachableAreaNum( org );
		aas->PushPointIntoAreaNum( move.toAreaNum, org );

		areaNum = PointReachableAreaNum( physicsObj.GetOrigin() );
		if( !PathToGoal( path, areaNum, physicsObj.GetOrigin(), move.toAreaNum, org ) )
		{
			StopMove( MOVE_STATUS_DEST_UNREACHABLE );
			AI_DEST_UNREACHABLE = true;
			return false;
		}
	}

	if( !move.toAreaNum && !NewWanderDir( org ) )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	move.moveDest = org;
	move.goalEntity = NULL;
	move.moveCommand = MOVE_TO_POSITION;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.startTime = gameLocal.time;
	move.speed = fly_speed;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = true;

	return true;
}

/*
=====================
idAI::MoveToCover
=====================
*/
bool idAI::MoveToCover( idEntity* entity, const idVec3& hideFromPos )
{
	int				areaNum;
	aasObstacle_t	obstacle;
	aasGoal_t		hideGoal;
	idBounds		bounds;

	if( !aas || !entity )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	const idVec3& org = physicsObj.GetOrigin();
	areaNum = PointReachableAreaNum( org );

	// consider the entity the monster tries to hide from as an obstacle
	obstacle.absBounds = entity->GetPhysics()->GetAbsBounds();

	idAASCallback_FindCoverArea findCover( hideFromPos );
	if( !aas->FindNearestGoal( hideGoal, areaNum, org, hideFromPos, travelFlags, &obstacle, 1, findCover ) )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	if( ReachedPos( hideGoal.origin, move.moveCommand ) )
	{
		StopMove( MOVE_STATUS_DONE );
		return true;
	}

	move.moveDest = hideGoal.origin;
	move.toAreaNum = hideGoal.areaNum;
	move.goalEntity = entity;
	move.moveCommand = MOVE_TO_COVER;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.startTime = gameLocal.time;
	move.speed = fly_speed;
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = true;

	return true;
}

/*
=====================
idAI::SlideToPosition
=====================
*/
bool idAI::SlideToPosition( const idVec3& pos, float time )
{
	StopMove( MOVE_STATUS_DONE );

	move.moveDest = pos;
	move.goalEntity = NULL;
	move.moveCommand = MOVE_SLIDE_TO_POSITION;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.startTime = gameLocal.time;
	move.duration = idPhysics::SnapTimeToPhysicsFrame( SEC2MS( time ) );
	AI_MOVE_DONE = false;
	AI_DEST_UNREACHABLE = false;
	AI_FORWARD = false;

	if( move.duration > 0 )
	{
		move.moveDir = ( pos - physicsObj.GetOrigin() ) / MS2SEC( move.duration );
		if( move.moveType != MOVETYPE_FLY )
		{
			move.moveDir.z = 0.0f;
		}
		move.speed = move.moveDir.LengthFast();
	}

	return true;
}

/*
=====================
idAI::WanderAround
=====================
*/
bool idAI::WanderAround()
{
	StopMove( MOVE_STATUS_DONE );

	move.moveDest = physicsObj.GetOrigin() + viewAxis[0] * physicsObj.GetGravityAxis() * 256.0f;
	if( !NewWanderDir( move.moveDest ) )
	{
		StopMove( MOVE_STATUS_DEST_UNREACHABLE );
		AI_DEST_UNREACHABLE = true;
		return false;
	}

	move.moveCommand = MOVE_WANDER;
	move.moveStatus = MOVE_STATUS_MOVING;
	move.startTime = gameLocal.time;
	move.speed = fly_speed;
	AI_MOVE_DONE = false;
	AI_FORWARD = true;

	return true;
}

// jmarshall begin
/*
================
idAI::CanReachEntity
================
*/
bool idAI::CanReachEntity( idEntity* ent )
{
	aasPath_t	path;
	int			toAreaNum;
	int			areaNum;
	idVec3		pos;

	if( !ent )
	{
		return false;
	}

	if( move.moveType != MOVETYPE_FLY )
	{
		if( !ent->GetFloorPos( 64.0f, pos ) )
		{
			return false;
		}
		if( ent->IsType( idActor::Type ) && static_cast<idActor*>( ent )->OnLadder() )
		{
			return false;
		}
	}
	else
	{
		pos = ent->GetPhysics()->GetOrigin();
	}

	toAreaNum = PointReachableAreaNum( pos );
	if( !toAreaNum )
	{
		return false;
	}

	const idVec3& org = physicsObj.GetOrigin();
	areaNum = PointReachableAreaNum( org );
	if( !toAreaNum || !PathToGoal( path, areaNum, org, toAreaNum, pos ) )
	{
		return false;
	}

	return true;
}

/*
================
idAI::CanReachEnemy
================
*/
bool idAI::CanReachEnemy()
{
	aasPath_t	path;
	int			toAreaNum;
	int			areaNum;
	idVec3		pos;
	idActor* enemyEnt;

	enemyEnt = enemy.GetEntity();
	if( !enemyEnt )
	{
		return false;
	}

	if( move.moveType != MOVETYPE_FLY )
	{
		if( enemyEnt->OnLadder() )
		{
			return false;
		}
		enemyEnt->GetAASLocation( aas, pos, toAreaNum );
	}
	else
	{
		pos = enemyEnt->GetPhysics()->GetOrigin();
		toAreaNum = PointReachableAreaNum( pos );
	}

	if( !toAreaNum )
	{
		return false;
	}

	const idVec3& org = physicsObj.GetOrigin();
	areaNum = PointReachableAreaNum( org );
	if( !PathToGoal( path, areaNum, org, toAreaNum, pos ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}
// jmarshall end

/*
=====================
idAI::MoveDone
=====================
*/
bool idAI::MoveDone() const
{
	return ( move.moveCommand == MOVE_NONE );
}

/*
================
idAI::StepDirection
================
*/
bool idAI::StepDirection( float dir )
{
	predictedPath_t path;
	idVec3 org;

	move.wanderYaw = dir;
	move.moveDir = idAngles( 0, move.wanderYaw, 0 ).ToForward();

	org = physicsObj.GetOrigin();

	idAI::PredictPath( this, aas, org, move.moveDir * 48.0f, 1000, 1000, ( move.moveType == MOVETYPE_FLY ) ? SE_BLOCKED : ( SE_ENTER_OBSTACLE | SE_BLOCKED | SE_ENTER_LEDGE_AREA ), path );

	if( path.blockingEntity && ( ( move.moveCommand == MOVE_TO_ENEMY ) || ( move.moveCommand == MOVE_TO_ENTITY ) ) && ( path.blockingEntity == move.goalEntity.GetEntity() ) )
	{
		// don't report being blocked if we ran into our goal entity
		return true;
	}

	if( ( move.moveType == MOVETYPE_FLY ) && ( path.endEvent == SE_BLOCKED ) )
	{
		float z;

		move.moveDir = path.endVelocity * 1.0f / 48.0f;

		// trace down to the floor and see if we can go forward
		idAI::PredictPath( this, aas, org, idVec3( 0.0f, 0.0f, -1024.0f ), 1000, 1000, SE_BLOCKED, path );

		idVec3 floorPos = path.endPos;
		idAI::PredictPath( this, aas, floorPos, move.moveDir * 48.0f, 1000, 1000, SE_BLOCKED, path );
		if( !path.endEvent )
		{
			move.moveDir.z = -1.0f;
			return true;
		}

		// trace up to see if we can go over something and go forward
		idAI::PredictPath( this, aas, org, idVec3( 0.0f, 0.0f, 256.0f ), 1000, 1000, SE_BLOCKED, path );

		idVec3 ceilingPos = path.endPos;

		for( z = org.z; z <= ceilingPos.z + 64.0f; z += 64.0f )
		{
			idVec3 start;
			if( z <= ceilingPos.z )
			{
				start.x = org.x;
				start.y = org.y;
				start.z = z;
			}
			else
			{
				start = ceilingPos;
			}
			idAI::PredictPath( this, aas, start, move.moveDir * 48.0f, 1000, 1000, SE_BLOCKED, path );
			if( !path.endEvent )
			{
				move.moveDir.z = 1.0f;
				return true;
			}
		}
		return false;
	}

	return ( path.endEvent == 0 );
}

/*
================
idAI::NewWanderDir
================
*/
bool idAI::NewWanderDir( const idVec3& dest )
{
	float	deltax, deltay;
	float	d[3];
	float	tdir, olddir, turnaround;

	move.nextWanderTime = gameLocal.time + ( gameLocal.random.RandomFloat() * 500 + 500 );

	olddir = idMath::AngleNormalize360( ( int )( current_yaw / 45 ) * 45 );
	turnaround = idMath::AngleNormalize360( olddir - 180 );

	idVec3 org = physicsObj.GetOrigin();
	deltax = dest.x - org.x;
	deltay = dest.y - org.y;
	if( deltax > 10 )
	{
		d[1] = 0;
	}
	else if( deltax < -10 )
	{
		d[1] = 180;
	}
	else
	{
		d[1] = DI_NODIR;
	}

	if( deltay < -10 )
	{
		d[2] = 270;
	}
	else if( deltay > 10 )
	{
		d[2] = 90;
	}
	else
	{
		d[2] = DI_NODIR;
	}

	// try direct route
	if( d[1] != DI_NODIR && d[2] != DI_NODIR )
	{
		if( d[1] == 0 )
		{
			tdir = d[2] == 90 ? 45 : 315;
		}
		else
		{
			tdir = d[2] == 90 ? 135 : 215;
		}

		if( tdir != turnaround && StepDirection( tdir ) )
		{
			return true;
		}
	}

	// try other directions
	if( ( gameLocal.random.RandomInt() & 1 ) || idMath::Fabs( deltay ) > idMath::Fabs( deltax ) )
	{
		tdir = d[1];
		d[1] = d[2];
		d[2] = tdir;
	}

	if( d[1] != DI_NODIR && d[1] != turnaround && StepDirection( d[1] ) )
	{
		return true;
	}

	if( d[2] != DI_NODIR && d[2] != turnaround && StepDirection( d[2] ) )
	{
		return true;
	}

	// there is no direct path to the player, so pick another direction
	if( olddir != DI_NODIR && StepDirection( olddir ) )
	{
		return true;
	}

	// randomly determine direction of search
	if( gameLocal.random.RandomInt() & 1 )
	{
		for( tdir = 0; tdir <= 315; tdir += 45 )
		{
			if( tdir != turnaround && StepDirection( tdir ) )
			{
				return true;
			}
		}
	}
	else
	{
		for( tdir = 315; tdir >= 0; tdir -= 45 )
		{
			if( tdir != turnaround && StepDirection( tdir ) )
			{
				return true;
			}
		}
	}

	if( turnaround != DI_NODIR && StepDirection( turnaround ) )
	{
		return true;
	}

	// can't move
	StopMove( MOVE_STATUS_DEST_UNREACHABLE );
	return false;
}

/*
=====================
idAI::GetMovePos
=====================
*/
bool idAI::GetMovePos( idVec3& seekPos )
{
	int			areaNum;
	aasPath_t	path;
	bool		result;
	idVec3		org;

	org = physicsObj.GetOrigin();
	seekPos = org;

	switch( move.moveCommand )
	{
		case MOVE_NONE:
			seekPos = move.moveDest;
			return false;
			break;

		case MOVE_FACE_ENEMY:
		case MOVE_FACE_ENTITY:
			seekPos = move.moveDest;
			return false;
			break;

		case MOVE_TO_POSITION_DIRECT:
			seekPos = move.moveDest;
			if( ReachedPos( move.moveDest, move.moveCommand ) )
			{
				StopMove( MOVE_STATUS_DONE );
			}
			return false;
			break;

		case MOVE_SLIDE_TO_POSITION:
			seekPos = org;
			return false;
			break;
	}

	if( move.moveCommand == MOVE_TO_ENTITY )
	{
		MoveToEntity( move.goalEntity.GetEntity() );
	}

	move.moveStatus = MOVE_STATUS_MOVING;
	result = false;
	if( gameLocal.time > move.blockTime )
	{
		if( move.moveCommand == MOVE_WANDER )
		{
			move.moveDest = org + viewAxis[0] * physicsObj.GetGravityAxis() * 256.0f;
		}
		else
		{
			if( ReachedPos( move.moveDest, move.moveCommand ) )
			{
				StopMove( MOVE_STATUS_DONE );
				seekPos = org;
				return false;
			}
		}

		if( aas && move.toAreaNum )
		{
			areaNum = PointReachableAreaNum( org );
			if( PathToGoal( path, areaNum, org, move.toAreaNum, move.moveDest ) )
			{
				seekPos = path.moveGoal;
				result = true;
				move.nextWanderTime = 0;
			}
			else
			{
				AI_DEST_UNREACHABLE = true;
			}
		}
	}

	if( !result )
	{
		// wander around
		if( ( gameLocal.time > move.nextWanderTime ) || !StepDirection( move.wanderYaw ) )
		{
			result = NewWanderDir( move.moveDest );
			if( !result )
			{
				StopMove( MOVE_STATUS_DEST_UNREACHABLE );
				AI_DEST_UNREACHABLE = true;
				seekPos = org;
				return false;
			}
		}
		else
		{
			result = true;
		}

		seekPos = org + move.moveDir * 2048.0f;
		if( ai_debugMove.GetBool() )
		{
			gameRenderWorld->DebugLine( colorYellow, org, seekPos, 1, true );
		}
	}
	else
	{
		AI_DEST_UNREACHABLE = false;
	}

	if( result && ( ai_debugMove.GetBool() ) )
	{
		gameRenderWorld->DebugLine( colorCyan, physicsObj.GetOrigin(), seekPos );
	}

	return result;
}


/***********************************************************************

	turning

***********************************************************************/

/*
=====================
idAI::Turn
=====================
*/
void idAI::Turn()
{
	float diff;
	float diff2;
	float turnAmount;
	animFlags_t animflags;

	if( !turnRate )
	{
		return;
	}

	// check if the animator has marker this anim as non-turning
	if( !legsAnim.Disabled() && !legsAnim.AnimDone( 0 ) )
	{
		animflags = legsAnim.GetAnimFlags();
	}
	else
	{
		animflags = torsoAnim.GetAnimFlags();
	}
	if( animflags.ai_no_turn )
	{
		return;
	}

	if( anim_turn_angles && animflags.anim_turn )
	{
		idMat3 rotateAxis;

		// set the blend between no turn and full turn
		float frac = anim_turn_amount / anim_turn_angles;
		animator.CurrentAnim( ANIMCHANNEL_LEGS )->SetSyncedAnimWeight( 0, 1.0f - frac );
		animator.CurrentAnim( ANIMCHANNEL_LEGS )->SetSyncedAnimWeight( 1, frac );
		animator.CurrentAnim( ANIMCHANNEL_TORSO )->SetSyncedAnimWeight( 0, 1.0f - frac );
		animator.CurrentAnim( ANIMCHANNEL_TORSO )->SetSyncedAnimWeight( 1, frac );

		// get the total rotation from the start of the anim
		animator.GetDeltaRotation( 0, gameLocal.time, rotateAxis );
		current_yaw = idMath::AngleNormalize180( anim_turn_yaw + rotateAxis[0].ToYaw() );
	}
	else
	{
		diff = idMath::AngleNormalize180( ideal_yaw - current_yaw );
		turnVel += AI_TURN_SCALE * diff * MS2SEC( gameLocal.time - gameLocal.previousTime );
		if( turnVel > turnRate )
		{
			turnVel = turnRate;
		}
		else if( turnVel < -turnRate )
		{
			turnVel = -turnRate;
		}
		turnAmount = turnVel * MS2SEC( gameLocal.time - gameLocal.previousTime );
		if( ( diff >= 0.0f ) && ( turnAmount >= diff ) )
		{
			turnVel = diff / MS2SEC( gameLocal.time - gameLocal.previousTime );
			turnAmount = diff;
		}
		else if( ( diff <= 0.0f ) && ( turnAmount <= diff ) )
		{
			turnVel = diff / MS2SEC( gameLocal.time - gameLocal.previousTime );
			turnAmount = diff;
		}
		current_yaw += turnAmount;
		current_yaw = idMath::AngleNormalize180( current_yaw );
		diff2 = idMath::AngleNormalize180( ideal_yaw - current_yaw );
		if( idMath::Fabs( diff2 ) < 0.1f )
		{
			current_yaw = ideal_yaw;
		}
	}

	viewAxis = idAngles( 0, current_yaw, 0 ).ToMat3();

	if( ai_debugMove.GetBool() )
	{
		const idVec3& org = physicsObj.GetOrigin();
		gameRenderWorld->DebugLine( colorRed, org, org + idAngles( 0, ideal_yaw, 0 ).ToForward() * 64, 1 );
		gameRenderWorld->DebugLine( colorGreen, org, org + idAngles( 0, current_yaw, 0 ).ToForward() * 48, 1 );
		gameRenderWorld->DebugLine( colorYellow, org, org + idAngles( 0, current_yaw + turnVel, 0 ).ToForward() * 32, 1 );
	}
}

/*
=====================
idAI::FacingIdeal
=====================
*/
bool idAI::FacingIdeal()
{
	float diff;

	if( !turnRate )
	{
		return true;
	}

	diff = idMath::AngleNormalize180( current_yaw - ideal_yaw );
	if( idMath::Fabs( diff ) < 0.01f )
	{
		// force it to be exact
		current_yaw = ideal_yaw;
		return true;
	}

	return false;
}

/*
=====================
idAI::TurnToward
=====================
*/
bool idAI::TurnToward( float yaw )
{
	ideal_yaw = idMath::AngleNormalize180( yaw );
	bool result = FacingIdeal();
	return result;
}

/*
=====================
idAI::TurnToward
=====================
*/
bool idAI::TurnToward( const idVec3& pos )
{
	idVec3 dir;
	idVec3 local_dir;
	float lengthSqr;

	dir = pos - physicsObj.GetOrigin();
	physicsObj.GetGravityAxis().ProjectVector( dir, local_dir );
	local_dir.z = 0.0f;
	lengthSqr = local_dir.LengthSqr();
	if( lengthSqr > Square( 2.0f ) || ( lengthSqr > Square( 0.1f ) && enemy.GetEntity() == NULL ) )
	{
		ideal_yaw = idMath::AngleNormalize180( local_dir.ToYaw() );
	}

	bool result = FacingIdeal();
	return result;
}


/***********************************************************************

	Movement

***********************************************************************/

/*
================
idAI::ApplyImpulse
================
*/
void idAI::ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse )
{
	// FIXME: Jim take a look at this and see if this is a reasonable thing to do
	// instead of a spawnArg flag.. Sabaoth is the only slide monster ( and should be the only one for D3 )
	// and we don't want him taking physics impulses as it can knock him off the path
	if( move.moveType != MOVETYPE_STATIC && move.moveType != MOVETYPE_SLIDE )
	{
		idActor::ApplyImpulse( ent, id, point, impulse );
	}
}

/*
=====================
idAI::GetMoveDelta
=====================
*/
void idAI::GetMoveDelta( const idMat3& oldaxis, const idMat3& axis, idVec3& delta )
{
	idVec3 oldModelOrigin;
	idVec3 modelOrigin;

	animator.GetDelta( gameLocal.previousTime, gameLocal.time, delta );
	delta = axis * delta;

	if( modelOffset != vec3_zero )
	{
		// the pivot of the monster's model is around its origin, and not around the bounding
		// box's origin, so we have to compensate for this when the model is offset so that
		// the monster still appears to rotate around it's origin.
		oldModelOrigin = modelOffset * oldaxis;
		modelOrigin = modelOffset * axis;
		delta += oldModelOrigin - modelOrigin;
	}

	delta *= physicsObj.GetGravityAxis();
}

/*
=====================
idAI::CheckObstacleAvoidance
=====================
*/
void idAI::CheckObstacleAvoidance( const idVec3& goalPos, idVec3& newPos )
{
	idEntity* obstacle;
	obstaclePath_t	path;
	idVec3			dir;
	float			dist;
	bool			foundPath;

	if( ignore_obstacles )
	{
		newPos = goalPos;
		move.obstacle = NULL;
		return;
	}

	const idVec3& origin = physicsObj.GetOrigin();

	obstacle = NULL;
	AI_OBSTACLE_IN_PATH = false;
	foundPath = FindPathAroundObstacles( &physicsObj, aas, enemy.GetEntity(), origin, goalPos, path );
	if( ai_showObstacleAvoidance.GetBool() )
	{
		gameRenderWorld->DebugLine( colorBlue, goalPos + idVec3( 1.0f, 1.0f, 0.0f ), goalPos + idVec3( 1.0f, 1.0f, 64.0f ), 1 );
		gameRenderWorld->DebugLine( foundPath ? colorYellow : colorRed, path.seekPos, path.seekPos + idVec3( 0.0f, 0.0f, 64.0f ), 1 );
	}

	if( !foundPath )
	{
		// couldn't get around obstacles
		if( path.firstObstacle )
		{
			AI_OBSTACLE_IN_PATH = true;
			if( physicsObj.GetAbsBounds().Expand( 2.0f ).IntersectsBounds( path.firstObstacle->GetPhysics()->GetAbsBounds() ) )
			{
				obstacle = path.firstObstacle;
			}
		}
		else if( path.startPosObstacle )
		{
			AI_OBSTACLE_IN_PATH = true;
			if( physicsObj.GetAbsBounds().Expand( 2.0f ).IntersectsBounds( path.startPosObstacle->GetPhysics()->GetAbsBounds() ) )
			{
				obstacle = path.startPosObstacle;
			}
		}
		else
		{
			// Blocked by wall
			move.moveStatus = MOVE_STATUS_BLOCKED_BY_WALL;
		}
#if 0
	}
	else if( path.startPosObstacle )
	{
		// check if we're past where the our origin was pushed out of the obstacle
		dir = goalPos - origin;
		dir.Normalize();
		dist = ( path.seekPos - origin ) * dir;
		if( dist < 1.0f )
		{
			AI_OBSTACLE_IN_PATH = true;
			obstacle = path.startPosObstacle;
		}
#endif
	}
	else if( path.seekPosObstacle )
	{
		// if the AI is very close to the path.seekPos already and path.seekPosObstacle != NULL
		// then we want to push the path.seekPosObstacle entity out of the way
		AI_OBSTACLE_IN_PATH = true;

		// check if we're past where the goalPos was pushed out of the obstacle
		dir = goalPos - origin;
		dir.Normalize();
		dist = ( path.seekPos - origin ) * dir;
		if( dist < 1.0f )
		{
			obstacle = path.seekPosObstacle;
		}
	}

	// if we had an obstacle, set our move status based on the type, and kick it out of the way if it's a moveable
	if( obstacle )
	{
		if( obstacle->IsType( idActor::Type ) )
		{
			// monsters aren't kickable
			if( obstacle == enemy.GetEntity() )
			{
				move.moveStatus = MOVE_STATUS_BLOCKED_BY_ENEMY;
			}
			else
			{
				move.moveStatus = MOVE_STATUS_BLOCKED_BY_MONSTER;
			}
		}
		else
		{
			// try kicking the object out of the way
			move.moveStatus = MOVE_STATUS_BLOCKED_BY_OBJECT;
		}
		newPos = obstacle->GetPhysics()->GetOrigin();
		//newPos = path.seekPos;
		move.obstacle = obstacle;
	}
	else
	{
		newPos = path.seekPos;
		move.obstacle = NULL;
	}
}

/*
=====================
idAI::DeadMove
=====================
*/
void idAI::DeadMove()
{
	idVec3				delta;
	monsterMoveResult_t	moveResult;

	idVec3 org = physicsObj.GetOrigin();

	GetMoveDelta( viewAxis, viewAxis, delta );
	physicsObj.SetDelta( delta );

	RunPhysics();

	moveResult = physicsObj.GetMoveResult();
	AI_ONGROUND = physicsObj.OnGround();
}

/*
=====================
idAI::AnimMove
=====================
*/
void idAI::AnimMove()
{
	idVec3				goalPos;
	idVec3				delta;
	idVec3				goalDelta;
	float				goalDist;
	monsterMoveResult_t	moveResult;
	idVec3				newDest;

	idVec3 oldorigin = physicsObj.GetOrigin();
	idMat3 oldaxis = viewAxis;

	AI_BLOCKED = false;

	if( move.moveCommand < NUM_NONMOVING_COMMANDS )
	{
		move.lastMoveOrigin.Zero();
		move.lastMoveTime = gameLocal.time;
	}

	move.obstacle = NULL;
	if( ( move.moveCommand == MOVE_FACE_ENEMY ) && enemy.GetEntity() )
	{
		TurnToward( lastVisibleEnemyPos );
		goalPos = oldorigin;
	}
	else if( ( move.moveCommand == MOVE_FACE_ENTITY ) && move.goalEntity.GetEntity() )
	{
		TurnToward( move.goalEntity.GetEntity()->GetPhysics()->GetOrigin() );
		goalPos = oldorigin;
	}
	else if( GetMovePos( goalPos ) )
	{
		if( move.moveCommand != MOVE_WANDER )
		{
			CheckObstacleAvoidance( goalPos, newDest );
			TurnToward( newDest );
		}
		else
		{
			TurnToward( goalPos );
		}
	}

	Turn();

	if( move.moveCommand == MOVE_SLIDE_TO_POSITION )
	{
		if( gameLocal.time < move.startTime + move.duration )
		{
			goalPos = move.moveDest - move.moveDir * MS2SEC( move.startTime + move.duration - gameLocal.time );
			delta = goalPos - oldorigin;
			delta.z = 0.0f;
		}
		else
		{
			delta = move.moveDest - oldorigin;
			delta.z = 0.0f;
			StopMove( MOVE_STATUS_DONE );
		}
	}
	else if( allowMove )
	{
		GetMoveDelta( oldaxis, viewAxis, delta );
	}
	else
	{
		delta.Zero();
	}

	if( move.moveCommand == MOVE_TO_POSITION )
	{
		goalDelta = move.moveDest - oldorigin;
		goalDist = goalDelta.LengthFast();
		if( goalDist < delta.LengthFast() )
		{
			delta = goalDelta;
		}
	}

	physicsObj.UseFlyMove( false );
	physicsObj.SetDelta( delta );
	physicsObj.ForceDeltaMove( disableGravity );

	RunPhysics();

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugLine( colorCyan, oldorigin, physicsObj.GetOrigin(), 5000 );
	}

	moveResult = physicsObj.GetMoveResult();
	if( !af_push_moveables && attack.Length() && TestMelee() )
	{
		DirectDamage( attack, enemy.GetEntity() );
	}
	else
	{
		idEntity* blockEnt = physicsObj.GetSlideMoveEntity();
		if( blockEnt != NULL && blockEnt->IsType( idMoveable::Type ) && blockEnt->GetPhysics()->IsPushable() )
		{
			KickObstacles( viewAxis[0], kickForce, blockEnt );
		}
	}

	BlockedFailSafe();

	AI_ONGROUND = physicsObj.OnGround();

	idVec3 org = physicsObj.GetOrigin();
	if( oldorigin != org )
	{
		TouchTriggers();
	}

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugBounds( colorMagenta, physicsObj.GetBounds(), org, 1 );
		gameRenderWorld->DebugBounds( colorMagenta, physicsObj.GetBounds(), move.moveDest, 1 );
		gameRenderWorld->DebugLine( colorYellow, org + EyeOffset(), org + EyeOffset() + viewAxis[0] * physicsObj.GetGravityAxis() * 16.0f, 1, true );
		DrawRoute();
	}
}

/*
=====================
Seek
=====================
*/
idVec3 Seek( idVec3& vel, const idVec3& org, const idVec3& goal, float prediction )
{
	idVec3 predictedPos;
	idVec3 goalDelta;
	idVec3 seekVel;

	// predict our position
	predictedPos = org + vel * prediction;
	goalDelta = goal - predictedPos;
	seekVel = goalDelta * MS2SEC( gameLocal.time - gameLocal.previousTime );

	return seekVel;
}

/*
=====================
idAI::SlideMove
=====================
*/
void idAI::SlideMove()
{
	idVec3				goalPos;
	idVec3				delta;
	idVec3				goalDelta;
	float				goalDist;
	monsterMoveResult_t	moveResult;
	idVec3				newDest;

	idVec3 oldorigin = physicsObj.GetOrigin();
	idMat3 oldaxis = viewAxis;

	AI_BLOCKED = false;

	if( move.moveCommand < NUM_NONMOVING_COMMANDS )
	{
		move.lastMoveOrigin.Zero();
		move.lastMoveTime = gameLocal.time;
	}

	move.obstacle = NULL;
	if( ( move.moveCommand == MOVE_FACE_ENEMY ) && enemy.GetEntity() )
	{
		TurnToward( lastVisibleEnemyPos );
		goalPos = move.moveDest;
	}
	else if( ( move.moveCommand == MOVE_FACE_ENTITY ) && move.goalEntity.GetEntity() )
	{
		TurnToward( move.goalEntity.GetEntity()->GetPhysics()->GetOrigin() );
		goalPos = move.moveDest;
	}
	else if( GetMovePos( goalPos ) )
	{
		CheckObstacleAvoidance( goalPos, newDest );
		TurnToward( newDest );
		goalPos = newDest;
	}

	if( move.moveCommand == MOVE_SLIDE_TO_POSITION )
	{
		if( gameLocal.time < move.startTime + move.duration )
		{
			goalPos = move.moveDest - move.moveDir * MS2SEC( move.startTime + move.duration - gameLocal.time );
		}
		else
		{
			goalPos = move.moveDest;
			StopMove( MOVE_STATUS_DONE );
		}
	}

	if( move.moveCommand == MOVE_TO_POSITION )
	{
		goalDelta = move.moveDest - oldorigin;
		goalDist = goalDelta.LengthFast();
		if( goalDist < delta.LengthFast() )
		{
			delta = goalDelta;
		}
	}

	idVec3 vel = physicsObj.GetLinearVelocity();
	float z = vel.z;
	idVec3  predictedPos = oldorigin + vel * AI_SEEK_PREDICTION;

	// seek the goal position
	goalDelta = goalPos - predictedPos;
	vel -= vel * AI_FLY_DAMPENING * MS2SEC( gameLocal.time - gameLocal.previousTime );
	vel += goalDelta * MS2SEC( gameLocal.time - gameLocal.previousTime );

	// cap our speed
	vel = vel.Truncate( fly_speed );
	vel.z = z;
	physicsObj.SetLinearVelocity( vel );
	physicsObj.UseVelocityMove( true );
	RunPhysics();

	if( ( move.moveCommand == MOVE_FACE_ENEMY ) && enemy.GetEntity() )
	{
		TurnToward( lastVisibleEnemyPos );
	}
	else if( ( move.moveCommand == MOVE_FACE_ENTITY ) && move.goalEntity.GetEntity() )
	{
		TurnToward( move.goalEntity.GetEntity()->GetPhysics()->GetOrigin() );
	}
	else if( move.moveCommand != MOVE_NONE )
	{
		if( vel.ToVec2().LengthSqr() > 0.1f )
		{
			TurnToward( vel.ToYaw() );
		}
	}
	Turn();

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugLine( colorCyan, oldorigin, physicsObj.GetOrigin(), 5000 );
	}

	moveResult = physicsObj.GetMoveResult();
	if( !af_push_moveables && attack.Length() && TestMelee() )
	{
		DirectDamage( attack, enemy.GetEntity() );
	}
	else
	{
		idEntity* blockEnt = physicsObj.GetSlideMoveEntity();
		if( blockEnt != NULL && blockEnt->IsType( idMoveable::Type ) && blockEnt->GetPhysics()->IsPushable() )
		{
			KickObstacles( viewAxis[0], kickForce, blockEnt );
		}
	}

	BlockedFailSafe();

	AI_ONGROUND = physicsObj.OnGround();

	idVec3 org = physicsObj.GetOrigin();
	if( oldorigin != org )
	{
		TouchTriggers();
	}

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugBounds( colorMagenta, physicsObj.GetBounds(), org, 1 );
		gameRenderWorld->DebugBounds( colorMagenta, physicsObj.GetBounds(), move.moveDest, 1 );
		gameRenderWorld->DebugLine( colorYellow, org + EyeOffset(), org + EyeOffset() + viewAxis[0] * physicsObj.GetGravityAxis() * 16.0f, 1, true );
		DrawRoute();
	}
}

/*
=====================
idAI::AdjustFlyingAngles
=====================
*/
void idAI::AdjustFlyingAngles()
{
	idVec3	vel;
	float 	speed;
	float 	roll;
	float 	pitch;

	vel = physicsObj.GetLinearVelocity();

	speed = vel.Length();
	if( speed < 5.0f )
	{
		roll = 0.0f;
		pitch = 0.0f;
	}
	else
	{
		roll = vel * viewAxis[1] * -fly_roll_scale / fly_speed;
		if( roll > fly_roll_max )
		{
			roll = fly_roll_max;
		}
		else if( roll < -fly_roll_max )
		{
			roll = -fly_roll_max;
		}

		pitch = vel * viewAxis[2] * -fly_pitch_scale / fly_speed;
		if( pitch > fly_pitch_max )
		{
			pitch = fly_pitch_max;
		}
		else if( pitch < -fly_pitch_max )
		{
			pitch = -fly_pitch_max;
		}
	}

	fly_roll = fly_roll * 0.95f + roll * 0.05f;
	fly_pitch = fly_pitch * 0.95f + pitch * 0.05f;

	if( flyTiltJoint != INVALID_JOINT )
	{
		animator.SetJointAxis( flyTiltJoint, JOINTMOD_WORLD, idAngles( fly_pitch, 0.0f, fly_roll ).ToMat3() );
	}
	else
	{
		viewAxis = idAngles( fly_pitch, current_yaw, fly_roll ).ToMat3();
	}
}

/*
=====================
idAI::AddFlyBob
=====================
*/
void idAI::AddFlyBob( idVec3& vel )
{
	idVec3	fly_bob_add;
	float	t;

	if( fly_bob_strength )
	{
		t = MS2SEC( gameLocal.time + entityNumber * 497 );
		fly_bob_add = ( viewAxis[1] * idMath::Sin16( t * fly_bob_horz ) + viewAxis[2] * idMath::Sin16( t * fly_bob_vert ) ) * fly_bob_strength;
		vel += fly_bob_add * MS2SEC( gameLocal.time - gameLocal.previousTime );
		if( ai_debugMove.GetBool() )
		{
			const idVec3& origin = physicsObj.GetOrigin();
			gameRenderWorld->DebugArrow( colorOrange, origin, origin + fly_bob_add, 0 );
		}
	}
}

/*
=====================
idAI::AdjustFlyHeight
=====================
*/
void idAI::AdjustFlyHeight( idVec3& vel, const idVec3& goalPos )
{
	const idVec3& origin = physicsObj.GetOrigin();
	predictedPath_t path;
	idVec3			end;
	idVec3			dest;
	trace_t			trace;
	idActor* enemyEnt;
	bool			goLower;

	// make sure we're not flying too high to get through doors
	goLower = false;
	if( origin.z > goalPos.z )
	{
		dest = goalPos;
		dest.z = origin.z + 128.0f;
		idAI::PredictPath( this, aas, goalPos, dest - origin, 1000, 1000, SE_BLOCKED, path );
		if( path.endPos.z < origin.z )
		{
			idVec3 addVel = Seek( vel, origin, path.endPos, AI_SEEK_PREDICTION );
			vel.z += addVel.z;
			goLower = true;
		}

		if( ai_debugMove.GetBool() )
		{
			gameRenderWorld->DebugBounds( goLower ? colorRed : colorGreen, physicsObj.GetBounds(), path.endPos, 1 );
		}
	}

	if( !goLower )
	{
		// make sure we don't fly too low
		end = origin;

		enemyEnt = enemy.GetEntity();
		if( enemyEnt )
		{
			end.z = lastVisibleEnemyPos.z + lastVisibleEnemyEyeOffset.z + fly_offset;
		}
		else
		{
			// just use the default eye height for the player
			end.z = goalPos.z + DEFAULT_FLY_OFFSET + fly_offset;
		}

		gameLocal.clip.Translation( trace, origin, end, physicsObj.GetClipModel(), mat3_identity, MASK_MONSTERSOLID, this );
		vel += Seek( vel, origin, trace.endpos, AI_SEEK_PREDICTION );
	}
}

/*
=====================
idAI::FlySeekGoal
=====================
*/
void idAI::FlySeekGoal( idVec3& vel, idVec3& goalPos )
{
	idVec3 seekVel;

	// seek the goal position
	seekVel = Seek( vel, physicsObj.GetOrigin(), goalPos, AI_SEEK_PREDICTION );
	seekVel *= fly_seek_scale;
	vel += seekVel;
}

/*
=====================
idAI::AdjustFlySpeed
=====================
*/
void idAI::AdjustFlySpeed( idVec3& vel )
{
	float speed;

	// apply dampening
	vel -= vel * AI_FLY_DAMPENING * MS2SEC( gameLocal.time - gameLocal.previousTime );

	// gradually speed up/slow down to desired speed
	speed = vel.Normalize();
	speed += ( move.speed - speed ) * MS2SEC( gameLocal.time - gameLocal.previousTime );
	if( speed < 0.0f )
	{
		speed = 0.0f;
	}
	else if( move.speed && ( speed > move.speed ) )
	{
		speed = move.speed;
	}

	vel *= speed;
}

/*
=====================
idAI::FlyTurn
=====================
*/
void idAI::FlyTurn()
{
	if( move.moveCommand == MOVE_FACE_ENEMY )
	{
		TurnToward( lastVisibleEnemyPos );
	}
	else if( ( move.moveCommand == MOVE_FACE_ENTITY ) && move.goalEntity.GetEntity() )
	{
		TurnToward( move.goalEntity.GetEntity()->GetPhysics()->GetOrigin() );
	}
	else if( move.speed > 0.0f )
	{
		const idVec3& vel = physicsObj.GetLinearVelocity();
		if( vel.ToVec2().LengthSqr() > 0.1f )
		{
			TurnToward( vel.ToYaw() );
		}
	}
	Turn();
}

/*
=====================
idAI::FlyMove
=====================
*/
void idAI::FlyMove()
{
	idVec3	goalPos;
	idVec3	oldorigin;
	idVec3	newDest;

	AI_BLOCKED = false;
	if( ( move.moveCommand != MOVE_NONE ) && ReachedPos( move.moveDest, move.moveCommand ) )
	{
		StopMove( MOVE_STATUS_DONE );
	}

	if( ai_debugMove.GetBool() )
	{
		gameLocal.Printf( "%d: %s: %s, vel = %.2f, sp = %.2f, maxsp = %.2f\n", gameLocal.time, name.c_str(), moveCommandString[move.moveCommand], physicsObj.GetLinearVelocity().Length(), move.speed, fly_speed );
	}

	if( move.moveCommand != MOVE_TO_POSITION_DIRECT )
	{
		idVec3 vel = physicsObj.GetLinearVelocity();

		if( GetMovePos( goalPos ) )
		{
			CheckObstacleAvoidance( goalPos, newDest );
			goalPos = newDest;
		}

		if( move.speed )
		{
			FlySeekGoal( vel, goalPos );
		}

		// add in bobbing
		AddFlyBob( vel );

		if( enemy.GetEntity() && ( move.moveCommand != MOVE_TO_POSITION ) )
		{
			AdjustFlyHeight( vel, goalPos );
		}

		AdjustFlySpeed( vel );

		physicsObj.SetLinearVelocity( vel );
	}

	// turn
	FlyTurn();

	// run the physics for this frame
	oldorigin = physicsObj.GetOrigin();
	physicsObj.UseFlyMove( true );
	physicsObj.UseVelocityMove( false );
	physicsObj.SetDelta( vec3_zero );
	physicsObj.ForceDeltaMove( disableGravity );
	RunPhysics();

	monsterMoveResult_t	moveResult = physicsObj.GetMoveResult();
	if( !af_push_moveables && attack.Length() && TestMelee() )
	{
		DirectDamage( attack, enemy.GetEntity() );
	}
	else
	{
		idEntity* blockEnt = physicsObj.GetSlideMoveEntity();
		if( blockEnt != NULL && blockEnt->IsType( idMoveable::Type ) && blockEnt->GetPhysics()->IsPushable() )
		{
			KickObstacles( viewAxis[0], kickForce, blockEnt );
		}
		else if( moveResult == MM_BLOCKED )
		{
			move.blockTime = gameLocal.time + 500;
			AI_BLOCKED = true;
		}
	}

	idVec3 org = physicsObj.GetOrigin();
	if( oldorigin != org )
	{
		TouchTriggers();
	}

	if( ai_debugMove.GetBool() )
	{
		gameRenderWorld->DebugLine( colorCyan, oldorigin, physicsObj.GetOrigin(), 4000 );
		gameRenderWorld->DebugBounds( colorOrange, physicsObj.GetBounds(), org, 1 );
		gameRenderWorld->DebugBounds( colorMagenta, physicsObj.GetBounds(), move.moveDest, 1 );
		gameRenderWorld->DebugLine( colorRed, org, org + physicsObj.GetLinearVelocity(), 1, true );
		gameRenderWorld->DebugLine( colorBlue, org, goalPos, 1, true );
		gameRenderWorld->DebugLine( colorYellow, org + EyeOffset(), org + EyeOffset() + viewAxis[0] * physicsObj.GetGravityAxis() * 16.0f, 1, true );
		DrawRoute();
	}
}

/*
=====================
idAI::StaticMove
=====================
*/
void idAI::StaticMove()
{
	idActor* enemyEnt = enemy.GetEntity();

	if( AI_DEAD )
	{
		return;
	}

	if( ( move.moveCommand == MOVE_FACE_ENEMY ) && enemyEnt )
	{
		TurnToward( lastVisibleEnemyPos );
	}
	else if( ( move.moveCommand == MOVE_FACE_ENTITY ) && move.goalEntity.GetEntity() )
	{
		TurnToward( move.goalEntity.GetEntity()->GetPhysics()->GetOrigin() );
	}
	else if( move.moveCommand != MOVE_NONE )
	{
		TurnToward( move.moveDest );
	}
	Turn();

	physicsObj.ForceDeltaMove( true ); // disable gravity
	RunPhysics();

	AI_ONGROUND = false;

	if( !af_push_moveables && attack.Length() && TestMelee() )
	{
		DirectDamage( attack, enemyEnt );
	}

	if( ai_debugMove.GetBool() )
	{
		const idVec3& org = physicsObj.GetOrigin();
		gameRenderWorld->DebugBounds( colorMagenta, physicsObj.GetBounds(), org, 1 );
		gameRenderWorld->DebugLine( colorBlue, org, move.moveDest, 1, true );
		gameRenderWorld->DebugLine( colorYellow, org + EyeOffset(), org + EyeOffset() + viewAxis[0] * physicsObj.GetGravityAxis() * 16.0f, 1, true );
	}
}