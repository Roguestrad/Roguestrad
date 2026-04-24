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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __AASCALLBACK_AVOIDLOCATION_H__
#define __AASCALLBACK_AVOIDLOCATION_H__

struct idAASObstacle {
	idBounds		 absBounds;	   // absolute bounds of obstacle
	mutable idBounds expAbsBounds; // expanded absolute bounds of obstacle
};

/*!
	\class idAASCallback_AvoidLocation
	\brief A callback class for avoiding specific locations during pathfinding in an AAS system.

	This class extends the base AAS callback functionality to provide pathfinding logic that avoids specified locations. It is designed to be used in navigation systems where certain areas or points
   should be avoided when calculating paths. The callback maintains information about a location to avoid and any obstacles that might affect the path calculation. The class is intended to be
   subclassed, with the AreaIsGoal method requiring implementation to define goal area logic. The primary use case is in environments where dynamic obstacle avoidance is required during path searches.

*/
class idAASCallback_AvoidLocation : public idAASCallback
{
public:
	//! Initializes the avoid location callback with default values.
	idAASCallback_AvoidLocation();
	~idAASCallback_AvoidLocation();

	//! Sets the location to avoid and calculates the distance from the start position.
	void		 SetAvoidLocation( const idVec3& start, const idVec3& avoidLocation );

	//! Sets the obstacles for the AAS callback avoiding location.
	void		 SetObstacles( const idAAS* aas, const idAASObstacle* obstacles, int numObstacles );

	//! Checks if a path between two points is valid by ensuring it does not intersect any obstacles.
	virtual bool PathValid( const idAAS* aas, const idVec3& start, const idVec3& end );

	//! Returns additional travel time for a path to avoid a specified location.
	virtual int	 AdditionalTravelTimeForPath( const idAAS* aas, const idVec3& start, const idVec3& end );
	virtual bool AreaIsGoal( const idAAS* aas, int areaNum ) = 0;

private:
	idVec3				 avoidLocation;
	float				 avoidDist;
	const idAASObstacle* obstacles;
	int					 numObstacles;
};

#endif /* !__AASCALLBACK_AVOIDLOCATION_H__ */
