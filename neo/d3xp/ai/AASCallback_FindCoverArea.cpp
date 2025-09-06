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

#include "AAS_local.h"
#include "../Game_local.h"		// for print and error

#include "AASCallback_FindCoverArea.h"

/*
============
idAASCallback_FindCoverArea::idAASCallback_FindCoverArea
============
*/
idAASCallback_FindCoverArea::idAASCallback_FindCoverArea( const idVec3& hideFromPos )
{
	int			numPVSAreas;
	idBounds	bounds( hideFromPos - idVec3( 16, 16, 0 ), hideFromPos + idVec3( 16, 16, 64 ) );

	// setup PVS
	numPVSAreas = gameLocal.pvs.GetPVSAreas( bounds, PVSAreas, MAX_PVS_AREAS );
	hidePVS		= gameLocal.pvs.SetupCurrentPVS( PVSAreas, numPVSAreas );
}

/*
============
idAASCallback_FindCoverArea::~idAASCallback_FindCoverArea
============
*/
idAASCallback_FindCoverArea::~idAASCallback_FindCoverArea()
{
	gameLocal.pvs.FreeCurrentPVS( hidePVS );
}

/*
============
idAASCallback_FindCoverArea::AreaIsGoal
============
*/
bool idAASCallback_FindCoverArea::AreaIsGoal( const idAAS* aas, int areaNum )
{
	idVec3	areaCenter;
	int		numPVSAreas;
	int		PVSAreas[ MAX_PVS_AREAS ];

	areaCenter = aas->AreaCenter( areaNum );
	areaCenter[ 2 ] += 1.0f;

	numPVSAreas = gameLocal.pvs.GetPVSAreas( idBounds( areaCenter ).Expand( 16.0f ), PVSAreas, MAX_PVS_AREAS );
	if( !gameLocal.pvs.InCurrentPVS( hidePVS, PVSAreas, numPVSAreas ) )
	{
		return true;
	}

	return false;
}
