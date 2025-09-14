/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Robert Beckebans

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

#include "AAS_local.h"
#include "../Game_local.h"		// for print and error

#include "AASCallback_FindAttackPosition.h"

/*
============
idAASCallback_FindAttackPosition::idAASCallback_FindAttackPosition
============
*/
idAASCallback_FindAttackPosition::idAASCallback_FindAttackPosition( const idAI* self, const idMat3& gravityAxis, idEntity* target, const idVec3& targetPos, const idVec3& fireOffset )
{
	int	numPVSAreas;

	this->target		= target;
	this->targetPos		= targetPos;
	this->fireOffset	= fireOffset;
	this->self			= self;
	this->gravityAxis	= gravityAxis;

	excludeBounds		= idBounds( idVec3( -64.0, -64.0f, -8.0f ), idVec3( 64.0, 64.0f, 64.0f ) );
	excludeBounds.TranslateSelf( self->GetPhysics()->GetOrigin() );

	// setup PVS
	idBounds bounds( targetPos - idVec3( 16, 16, 0 ), targetPos + idVec3( 16, 16, 64 ) );
	numPVSAreas = gameLocal.pvs.GetPVSAreas( bounds, PVSAreas, idEntity::MAX_PVS_AREAS );
	targetPVS	= gameLocal.pvs.SetupCurrentPVS( PVSAreas, numPVSAreas );
}

/*
============
idAASCallback_FindAttackPosition::~idAASCallback_FindAttackPosition
============
*/
idAASCallback_FindAttackPosition::~idAASCallback_FindAttackPosition()
{
	gameLocal.pvs.FreeCurrentPVS( targetPVS );
}

/*
============
idAASCallback_FindAttackPosition::TestArea
============
*/
bool idAASCallback_FindAttackPosition::AreaIsGoal( const idAAS* aas, int areaNum )
{
	idVec3	dir;
	idVec3	local_dir;
	idVec3	fromPos;
	idMat3	axis;
	idVec3	areaCenter;
	int		numPVSAreas;
	int		PVSAreas[ idEntity::MAX_PVS_AREAS ];

	areaCenter = aas->AreaCenter( areaNum );
	areaCenter[ 2 ] += 1.0f;

	if( excludeBounds.ContainsPoint( areaCenter ) )
	{
		// too close to where we already are
		return false;
	}

	numPVSAreas = gameLocal.pvs.GetPVSAreas( idBounds( areaCenter ).Expand( 16.0f ), PVSAreas, idEntity::MAX_PVS_AREAS );
	if( !gameLocal.pvs.InCurrentPVS( targetPVS, PVSAreas, numPVSAreas ) )
	{
		return false;
	}

	// calculate the world transform of the launch position
	dir = targetPos - areaCenter;
	gravityAxis.ProjectVector( dir, local_dir );
	local_dir.z = 0.0f;
	local_dir.ToVec2().Normalize();
	axis = local_dir.ToMat3();
	fromPos = areaCenter + fireOffset * axis;

	return self->GetAimDir( fromPos, target, self, dir );
}
