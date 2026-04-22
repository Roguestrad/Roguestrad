/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#ifndef __AASFILELOCAL_H__
#define __AASFILELOCAL_H__

/*!
	\class idAASFileLocal
	\brief A class for managing local AAS file data with methods for geometry queries, reachability analysis, and file I/O operations.

	This class provides functionality for loading, parsing, and querying AAS (Area Awareness System) data, primarily for navigation and pathfinding purposes. It extends the base idAASFile class and
   offers methods to compute geometric properties of edges, faces, and areas, as well as determine reachability between different points in the navigation space. The class supports loading and saving
   AAS data from/to files, and includes optimization routines for improving data structure efficiency. It also provides methods for tracing through the AAS structure, pushing points into areas, and
   reporting routing efficiency.

*/
class idAASFileLocal : public idAASFile
{
	friend class idAASBuild;
	friend class idAASReach;
	friend class idAASCluster;

public:
	//! Initializes the idAASFileLocal object and sets up granularity for various internal data structures.
	idAASFileLocal();

	//! Destructor for idAASFileLocal that cleans up memory allocated for area reachability data.
	virtual ~idAASFileLocal();

public:
	//! Returns the center point of an edge in the AAS file
	virtual idVec3	 EdgeCenter( int edgeNum ) const;

	//! Calculates and returns the center point of a specified face in the AAS file.
	virtual idVec3	 FaceCenter( int faceNum ) const;

	//! Calculates and returns the center point of a specified area by averaging the centers of its constituent faces.
	virtual idVec3	 AreaCenter( int areaNum ) const;

	//! Returns the bounding box of an edge defined by its index.
	virtual idBounds EdgeBounds( int edgeNum ) const;

	//! Returns the bounding box of a specific face in the AAS file.
	virtual idBounds FaceBounds( int faceNum ) const;

	//! Returns the bounding box of an AAS area by combining the bounds of all faces that make up the area.
	virtual idBounds AreaBounds( int areaNum ) const;

	//! Returns the area number containing the specified origin point in the AAS file
	virtual int		 PointAreaNum( const idVec3& origin ) const;

	/*!
		\brief Finds the number of the reachable AAS area for a given point, considering area and travel flags.

		This function determines the AAS area number that is reachable from a specified point. It first checks if the point is directly within an area that matches the given flags. If not, it traces
	   up and down to find a reachable area. If no direct or traced area is found, it expands search bounds and checks areas within those bounds. The function returns 0 if no suitable area is found.

		\param origin The 3D point to check for reachability.
		\param searchBounds The bounds to expand when searching for a reachable area.
		\param areaFlags The flags that the target area must have to be considered.
		\param excludeTravelFlags The travel flags that must not be set in the target area.
		\return The number of the reachable AAS area, or 0 if no suitable area is found.
	*/
	virtual int		 PointReachableAreaNum( const idVec3& origin, const idBounds& searchBounds, const int areaFlags, const int excludeTravelFlags ) const;

	//! Returns the number of the first reachable area found within the specified bounds, considering area and travel flags.
	virtual int		 BoundsReachableAreaNum( const idBounds& bounds, const int areaFlags, const int excludeTravelFlags ) const;

	//! Pushes a point into the specified area by projecting it onto the area's face planes
	virtual void	 PushPointIntoAreaNum( int areaNum, idVec3& point ) const;

	//! Performs a trace operation through the AAS file from a start point to an end point, returning true if the trace hits something.
	virtual bool	 Trace( aasTrace_t& trace, const idVec3& start, const idVec3& end ) const;

	//! Prints information about the AAS file including size, number of areas, and maximum tree depth
	virtual void	 PrintInfo() const;

	//! Returns whether the AAS file has new features enabled
	virtual bool	 HasNewFeatures() const { return hasNewFeatures; }

	/*!
		\brief Calculates the distance from a point to the floor of an AAS area, considering edge distances and bounding box height.

		This function computes the vertical distance from a given point to the floor plane of a specified AAS area. It takes into account the height of the bounding box and a maximum edge distance to
	   determine the closest floor distance. If the bounding box height is less than or equal to the computed floor distance, the function also evaluates the distances to the edges of the area to find
	   the minimum valid distance. The result is a float representing the final computed floor distance.

		\param areaNum Index of the AAS area to evaluate
		\param floorPlane The plane representing the floor of the area
		\param point The 3D point from which the distance is calculated
		\param bboxHeight The height of the bounding box to consider
		\param maxEdgeDist Maximum distance to consider for edge calculations
		\return The calculated floor distance, which may be adjusted based on edge distances and bounding box height
	*/
	float			 GetFloorDistance( int areaNum, const idPlane& floorPlane, const idVec3& point, const float bboxHeight, const float maxEdgeDist );
	// jmarshall end
public:
	//! Loads an AAS file with the specified file name and map file CRC, returning true if successful.
	bool Load( const idStr& fileName, unsigned int mapFileCRC );

	//! Writes the AAS data to a file with the specified name and map file CRC.
	bool Write( const idStr& fileName, unsigned int mapFileCRC );

	//! Returns the total memory size of all data structures in the local AAS file.
	int	 MemorySize() const;

	//! Reports the routing efficiency of the AAS file by calculating and displaying reachable areas and memory usage.
	void ReportRoutingEfficiency() const;

	//! Optimizes the AAS file data by remapping vertices, edges, and faces to reduce redundancy and improve efficiency
	void Optimize();

	//! Links reversed reachabilities for all areas in the AAS file.
	void LinkReversedReachability();

	//! Calculates and sets the center and bounds for each area in the AAS file.
	void FinishAreas();

	//! Clears all internal data structures of the idAASFileLocal object.
	void Clear();

	//! Deletes all reachability links from the AAS file.
	void DeleteReachabilities();

	//! Initializes the AAS file local by clearing existing data and setting up dummy portal and cluster entries.
	void DeleteClusters();

private:
	//! Parses AAS index data from a lexer into a list of index values
	bool ParseIndex( idLexer& src, idList<aasIndex_t>& indexes );

	//! Parses plane data from the lexer input stream.
	bool ParsePlanes( idLexer& src );

	//! Parses vertex data from a lexer input stream and stores it in the vertices array.
	bool ParseVertices( idLexer& src );

	//! Parses edge data from a lexer input stream and stores it in the local AAS file.
	bool ParseEdges( idLexer& src );

	//! Parses face data from the provided lexer input stream
	bool ParseFaces( idLexer& src );

	//! Parses reachability data for a specified area from the given lexer input
	bool ParseReachabilities( idLexer& src, int areaNum );

	//! Parses area data from a lexer input stream and populates the areas array
	bool ParseAreas( idLexer& src );

	//! Parses AAS node data from a lexer stream and stores it in the local AAS file.
	bool ParseNodes( idLexer& src );

	//! Parses portal data from a lexer input stream and stores it in the local AAS file.
	bool ParsePortals( idLexer& src );

	//! Parses cluster data from a lexer input stream and populates the internal clusters array.
	bool ParseClusters( idLexer& src );

private:
	/*!
		\brief Recursively finds a reachable area number within the specified bounds considering area and travel flags

		This function performs a recursive traversal of the AAS tree to locate an area that is reachable within the given bounds and satisfies the specified area and travel flags. It navigates through
	   the AAS nodes using the bounding box to determine which child nodes to explore. The search continues until a matching area is found or all possibilities are exhausted. The function supports
	   exclusion of certain travel flags when determining area eligibility

		\param nodeNum Index of the current AAS node in the tree
		\param bounds Bounding volume used to test node traversal and area eligibility
		\param areaFlags Flags that must be set for an area to be considered valid
		\param excludeTravelFlags Travel flags that must not be set for an area to be considered valid
		\return Area number if a valid reachable area is found within the bounds, zero otherwise
	*/
	int	   BoundsReachableAreaNum_r( int nodeNum, const idBounds& bounds, const int areaFlags, const int excludeTravelFlags ) const;

	//! Computes the maximum depth of a tree structure starting from a given node.
	void   MaxTreeDepth_r( int nodeNum, int& depth, int& maxDepth ) const;

	//! Returns the maximum depth of the AAS tree.
	int	   MaxTreeDepth() const;

	//! Returns the travel flags for the specified AAS area based on its contents.
	int	   AreaContentsTravelFlags( int areaNum ) const;

	//! Returns the reachable goal position for a specified area in the AAS file.
	idVec3 AreaReachableGoal( int areaNum ) const;

	//! Returns the total number of reachabilities in all AAS areas.
	int	   NumReachabilities() const;

private:
	bool hasNewFeatures;
};

#endif /* !__AASFILELOCAL_H__ */
