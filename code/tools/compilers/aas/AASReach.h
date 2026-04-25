/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU
General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __AASREACH_H__
#define __AASREACH_H__

/*!
	\class idAASReach
	\brief The idAASReach class handles the generation and management of reachability data for areas within a navigation mesh.

	This class is responsible for building and maintaining reachability information between different areas in a navigation mesh, which is essential for pathfinding and AI movement. It provides
   methods to construct reachability data based on map geometry and area properties, and includes functionality for different types of movement such as walking, swimming, and flying. The class
   supports checking specific reachability conditions and adding various types of reachability connections between areas. It interfaces with map and AAS file data structures to determine valid
   movement paths and transitions.

*/
class idAASReach
{
public:
	//! Builds AAS reachability data for the given map file and AAS file
	bool Build( const idMapFile* mapFile, idAASFileLocal* file );

private:
	const idMapFile* mapFile;
	idAASFileLocal*	 file;
	int				 numReachabilities;
	bool			 allowSwimReachabilities;
	bool			 allowFlyReachabilities;

private:
	//! Flags areas in the AAS file as reachable based on their properties and settings.
	void FlagReachableAreas( idAASFileLocal* file );

	//! Checks if reachability exists between two AAS areas.
	bool ReachabilityExists( int fromAreaNum, int toAreaNum );

	//! Checks if a given area number allows swimming by testing if the area's contents include water.
	bool CanSwimInArea( int areaNum );

	//! Checks if the specified area has a floor flag set.
	bool AreaHasFloor( int areaNum );

	//! Checks if the specified area number corresponds to a cluster portal area.
	bool AreaIsClusterPortal( int areaNum );

	//! Adds a reachability to a specified area in the AAS file.
	void AddReachabilityToArea( idReachability* reach, int areaNum );

	//! Calculates and adds fly reachability connections between areas through shared faces.
	void Reachability_Fly( int areaNum );

	//! Creates swim reachability connections between areas in the AAS file
	void Reachability_Swim( int areaNum );

	//! Creates walk reachability between areas with equal floor height
	void Reachability_EqualFloorHeight( int areaNum );

	//! Checks if a reachability exists between two AAS areas using step, barrier, water jump, or walk off ledge movements
	bool Reachability_Step_Barrier_WaterJump_WalkOffLedge( int fromAreaNum, int toAreaNum );

	//! Computes walk-off-ledge reachability from a given area to adjacent areas
	void Reachability_WalkOffLedge( int areaNum );
};

#endif /* !__AASREACH_H__ */
