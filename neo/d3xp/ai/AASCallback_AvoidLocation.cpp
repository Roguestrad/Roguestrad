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
#include "AASCallback_AvoidLocation.h"

/*
============
idAASCallback_AvoidLocation::idAASCallback_AvoidLocation
============
*/
idAASCallback_AvoidLocation::idAASCallback_AvoidLocation()
{
	avoidLocation.Zero();
	avoidDist = 0.0f;
	obstacles = NULL;
	numObstacles = 0;
}

/*
============
idAASCallback_AvoidLocation::~idAASCallback_AvoidLocation
============
*/
idAASCallback_AvoidLocation::~idAASCallback_AvoidLocation()
{
}

/*
============
idAASCallback_AvoidLocation::SetAvoidLocation
============
*/
void idAASCallback_AvoidLocation::SetAvoidLocation( const idVec3& start, const idVec3& avoidLocation )
{
	this->avoidLocation = avoidLocation;
	this->avoidDist = ( avoidLocation - start ).Length();
}

/*
============
idAASCallback_AvoidLocation::SetObstacles
============
*/
void idAASCallback_AvoidLocation::SetObstacles( const idAAS* aas, const idAASObstacle* obstacles, int numObstacles )
{
	this->obstacles = obstacles;
	this->numObstacles = numObstacles;

	for( int i = 0; i < numObstacles; i++ )
	{
		obstacles[i].expAbsBounds[0] = obstacles[i].absBounds[0] - aas->GetSettings()->boundingBoxes[0][1];
		obstacles[i].expAbsBounds[1] = obstacles[i].absBounds[1] - aas->GetSettings()->boundingBoxes[0][0];
	}
}

/*
============
idAASCallback_AvoidLocation::PathValid
============
*/
bool idAASCallback_AvoidLocation::PathValid( const idAAS* aas, const idVec3& start, const idVec3& end )
{
	// path may not go through any obstacles
	for( int i = 0; i < numObstacles; i++ )
	{
		// if the movement vector intersects the expanded obstacle bounds
		if( obstacles[i].expAbsBounds.LineIntersection( start, end ) )
		{
			return false;
		}
	}
	return true;
}

/*
============
idAASCallback_AvoidLocation::AdditionalTravelTimeForPath
============
*/
int idAASCallback_AvoidLocation::AdditionalTravelTimeForPath( const idAAS* aas, const idVec3& start, const idVec3& end )
{
	if( avoidDist <= 0.0f )
	{
		return 0;
	}

	// project avoidLocation origin onto movement vector
	idVec3 v1 = end - start;
	v1.Normalize();
	idVec3 v2 = avoidLocation - start;
	idVec3 p = start + ( v2 * v1 ) * v1;

	// get the point on the path closest to the avoidLocation
	int i;
	for( i = 0; i < 3; i++ )
	{
		if( ( p[i] > start[i] + 0.1f && p[i] > end[i] + 0.1f ) ||
				( p[i] < start[i] - 0.1f && p[i] < end[i] - 0.1f ) )
		{
			break;
		}
	}

	float dist;
	if( i >= 3 )
	{
		dist = ( avoidLocation - p ).Length();
	}
	else
	{
		dist = ( avoidLocation - end ).Length();
	}

	// avoid moving closer to the avoidLocation
	if( dist < avoidDist )
	{
		return idMath::Ftoi( ( avoidDist - dist ) * 10.0f );
	}

	return 0;
}
