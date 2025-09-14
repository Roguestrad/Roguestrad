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

#include "AASCallback_FindAreaOutOfRange.h"

/*
============
idAASCallback_FindAreaOutOfRange::idAASCallback_FindAreaOutOfRange
============
*/
idAASCallback_FindAreaOutOfRange::idAASCallback_FindAreaOutOfRange( const idVec3& targetPos, float maxDist )
{
	this->targetPos		= targetPos;
	this->maxDistSqr	= maxDist * maxDist;
}

/*
============
idAASCallback_FindAreaOutOfRange::AreaIsGoal
============
*/
bool idAASCallback_FindAreaOutOfRange::AreaIsGoal( const idAAS* aas, int areaNum )
{
	const idVec3& areaCenter = aas->AreaCenter( areaNum );

	if( maxDistSqr > 0.0f )
	{
		float dist = ( targetPos.ToVec2() - areaCenter.ToVec2() ).LengthSqr();
		if( dist < maxDistSqr )
		{
			return false;
		}
	}

	trace_t	trace;
	gameLocal.clip.TracePoint( trace, targetPos, areaCenter + idVec3( 0.0f, 0.0f, 1.0f ), MASK_OPAQUE, NULL );
	if( trace.fraction < 1.0f )
	{
		return false;
	}

	return true;
}
