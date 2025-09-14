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

#ifndef __AASCALLBACK_AVOIDLOCATION_H__
#define __AASCALLBACK_AVOIDLOCATION_H__

/*
===============================================================================

	idAASCallback_AvoidLocation

===============================================================================
*/

struct idAASObstacle
{
	idBounds				absBounds;		// absolute bounds of obstacle
	mutable idBounds		expAbsBounds;	// expanded absolute bounds of obstacle
};

class idAASCallback_AvoidLocation : public idAASCallback
{
public:
	idAASCallback_AvoidLocation();
	~idAASCallback_AvoidLocation();

	void					SetAvoidLocation( const idVec3& start, const idVec3& avoidLocation );
	void					SetObstacles( const idAAS* aas, const idAASObstacle* obstacles, int numObstacles );

	virtual bool			PathValid( const idAAS* aas, const idVec3& start, const idVec3& end );
	virtual int				AdditionalTravelTimeForPath( const idAAS* aas, const idVec3& start, const idVec3& end );
	virtual bool			AreaIsGoal( const idAAS* aas, int areaNum ) = 0;

private:
	idVec3					avoidLocation;
	float					avoidDist;
	const idAASObstacle* 	obstacles;
	int						numObstacles;
};

#endif /* !__AASCALLBACK_AVOIDLOCATION_H__ */
