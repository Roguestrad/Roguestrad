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

#ifndef __AASCLUSTER_H__
#define __AASCLUSTER_H__

/*!
	\class idAASCluster
	\brief Manages AAS cluster construction and portal validation for navigation mesh processing.

	This class handles the construction of AAS clusters by processing portal areas and determining reachable clusters. It provides methods for building individual clusters, flooding connected areas to
   assign cluster numbers, and creating portal entries for cluster boundaries. The class supports operations for validating portal integrity, removing invalid portals, and reporting processing
   efficiency. It can build clusters from complete AAS file data or process individual clusters, making it suitable for navigation mesh construction and validation tasks in pathfinding systems.

*/
class idAASCluster
{
public:
	//! Builds AAS clusters by processing portal areas and finding reachable clusters.
	bool Build( idAASFileLocal* file );

	//! Builds a single cluster from the provided AAS file data
	bool BuildSingleCluster( idAASFileLocal* file );

private:
	idAASFileLocal* file;
	bool			noFaceFlood;

private:
	//! Updates the portal for the specified area and cluster number
	bool UpdatePortal( int areaNum, int clusterNum );

	//! Floods and assigns cluster numbers to connected areas starting from a given area
	bool FloodClusterAreas_r( int areaNum, int clusterNum );

	//! Clear the cluster numbers assigned to AAS areas.
	void RemoveAreaClusterNumbers();

	//! Numbers the areas and portals within a specified cluster that have reachabilities
	void NumberClusterAreas( int clusterNum );

	//! Finds and creates clusters from areas in the AAS file.
	bool FindClusters();

	//! Creates portal entries for areas marked as cluster portals in the AAS file.
	void CreatePortals();

	//! Tests the validity of AAS cluster portals and returns true if all portals are valid.
	bool TestPortals();
	void ReportEfficiency();

	//! Removes invalid cluster portals from the AAS file
	void RemoveInvalidPortals();
};

#endif /* !__AASCLUSTER_H__ */
