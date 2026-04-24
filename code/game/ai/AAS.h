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

#ifndef __AAS_H__
#define __AAS_H__

/*
===============================================================================

	Area Awareness System

===============================================================================
*/

enum { PATHTYPE_WALK, PATHTYPE_WALKOFFLEDGE, PATHTYPE_BARRIERJUMP, PATHTYPE_JUMP };

typedef struct aasPath_s {
	int					  type;			 // path type
	idVec3				  moveGoal;		 // point the AI should move towards
	int					  moveAreaNum;	 // number of the area the AI should move towards
	idVec3				  secondaryGoal; // secondary move goal for complex navigation
	const idReachability* reachability;	 // reachability used for navigation
} aasPath_t;

typedef struct aasGoal_s {
	int	   areaNum; // area the goal is in
	idVec3 origin;	// position of goal
} aasGoal_t;

typedef struct aasObstacle_s {
	idBounds absBounds;	   // absolute bounds of obstacle
	idBounds expAbsBounds; // expanded absolute bounds of obstacle
} aasObstacle_t;

/*!
	\class idAASCallback
	\brief Abstract base class for AAS callback functionality.
*/
class idAASCallback
{
public:
	virtual ~idAASCallback() {};
	virtual bool AreaIsGoal( const class idAAS* aas, int areaNum ) = 0;
};

typedef int aasHandle_t;

/*!
	\class idAAS
	\brief Base class for AAS (Area Awareness System) functionality.

	This abstract class defines the interface for an Area Awareness System that provides navigation and pathfinding capabilities within a game level. It offers methods for querying area information,
   performing collision detection and tracing, managing dynamic obstacles, and calculating travel paths between points. The system is designed to work with navigation data structures that represent
   the walkable areas of a map and their connectivity.

*/
class idAAS
{
public:
	//! Allocates and returns a new instance of idAASLocal.
	static idAAS* Alloc();
	virtual ~idAAS() = 0;

	//! Initializes the AAS local instance with the specified map name and CRC.
	virtual bool				 Init( const idStr& mapName, unsigned int mapFileCRC ) = 0;

	//! Outputs statistics for the AAS file
	virtual void				 Stats() const = 0;

	//! Tests AAS functionality at the specified origin position.
	virtual void				 Test( const idVec3& origin ) = 0;

	//! Returns the AAS settings for this local AAS file.
	virtual const idAASSettings* GetSettings() const = 0;

	//! Returns the area number of the given point in the AAS file.
	virtual int					 PointAreaNum( const idVec3& origin ) const = 0;

	//! Returns the area number of the first reachable area from a given point within specified bounds and travel flags
	virtual int					 PointReachableAreaNum( const idVec3& origin, const idBounds& searchBounds, const int areaFlags ) const = 0;

	//! Returns the number of the first reachable area found within the specified bounds, considering given area flags and travel flags.
	virtual int					 BoundsReachableAreaNum( const idBounds& bounds, const int areaFlags ) const = 0;

	//! Moves a point into the specified area if it is not already within that area.
	virtual void				 PushPointIntoAreaNum( int areaNum, idVec3& origin ) const = 0;

	//! Returns the center point of the specified AAS area
	virtual idVec3				 AreaCenter( int areaNum ) const = 0;

	//! Returns the flags for a specified area in the AAS file.
	virtual int					 AreaFlags( int areaNum ) const = 0;

	//! Returns the travel flags for a specified area in the AAS file.
	virtual int					 AreaTravelFlags( int areaNum ) const = 0;

	//! Performs a trace operation through the AAS file from start to end positions and returns true if the trace was successful.
	virtual bool				 Trace( aasTrace_t& trace, const idVec3& start, const idVec3& end ) const = 0;

	//! Returns a const reference to the plane at the specified index from the AAS file.
	virtual const idPlane&		 GetPlane( int planeNum ) const = 0;

	/*!
		\brief  Retrieves the edges of floor faces in an AAS area that are not shared with other floor faces and not used by reachabilities.

		This function processes the AAS file to find edges of floor faces in the specified area that meet certain criteria. It starts from a given area and traverses connected areas based on travel
	   flags and bounds intersection. The function identifies edges that are not shared with other floor faces within the same area and are not used by any reachabilities. It populates the provided
	   edges array with these edge numbers, up to the maximum number of edges specified. The function returns the total count of edges found.

		\param areaNum The number of the starting area to process
		\param bounds The bounds to check for intersection with areas during traversal
		\param travelFlags Flags that specify which travel types to consider when traversing areas
		\param edges Pointer to an array where the found edge numbers will be stored
		\param maxEdges The maximum number of edges to store in the edges array
		\return The number of edges found and stored in the edges array
	*/
	virtual int					 GetWallEdges( int areaNum, const idBounds& bounds, int travelFlags, int* edges, int maxEdges ) const = 0;

	//! Sorts wall edges into sequences based on connectivity
	virtual void				 SortWallEdges( int* edges, int numEdges ) const = 0;

	//! Retrieves the vertex numbers for a given edge in the AAS file.
	virtual void				 GetEdgeVertexNumbers( int edgeNum, int verts[2] ) const = 0;

	//! Retrieves the start and end vertices of a specified edge from the AAS file
	virtual void				 GetEdge( int edgeNum, idVec3& start, idVec3& end ) const = 0;

	//! Sets the routing state of areas within the specified bounds based on their contents.
	virtual bool				 SetAreaState( const idBounds& bounds, const int areaContents, bool disabled ) = 0;

	//! Adds a routing obstacle to the AAS local instance and returns its handle.
	virtual aasHandle_t			 AddObstacle( const idBounds& bounds ) = 0;

	//! Removes an obstacle from the AAS local navigation system using the provided handle
	virtual void				 RemoveObstacle( const aasHandle_t handle ) = 0;

	//! Removes all obstacles from the AAS routing system
	virtual void				 RemoveAllObstacles() = 0;

	/*!
		\brief Calculates the travel time from a given area and origin point to a goal area using AAS navigation data.

		This function determines the time required to travel from a specified area and origin position to a goal area. It uses the AAS (Area Awareness System) data to find a valid route and returns
	   the travel time. If no route is found or the AAS file is not available, it returns zero. The function considers travel flags to determine acceptable travel methods.

		\param areaNum The number of the starting area
		\param origin The 3D position within the starting area
		\param goalAreaNum The number of the target area
		\param travelFlags Flags specifying travel constraints and methods
		\return The travel time in milliseconds required to reach the goal area from the specified origin, or zero if no route is found or the AAS file is unavailable
	*/
	virtual int					 TravelTimeToGoalArea( int areaNum, const idVec3& origin, int goalAreaNum, int travelFlags ) const = 0;

	/*!
		\brief Calculates the optimal route from a given area to a goal area using AAS navigation data.

		This function determines the best path from the specified starting area to the goal area, considering travel flags and avoiding unnecessary routes. It handles cases where the areas are in the
	   same cluster or different clusters, including cases where the goal area is a portal. The function also manages caching of routing data to improve performance and ensures memory usage stays
	   within limits by deleting oldest cache entries when necessary. The optimal reachability and travel time are returned through the output parameters.

		\param areaNum The number of the starting area
		\param origin The origin position in the world space
		\param goalAreaNum The number of the goal area
		\param travelFlags Flags indicating travel restrictions or preferences
		\param travelTime Reference to an integer that will store the calculated travel time
		\param reach Reference to a pointer that will store the best reachability found
		\return True if a route to the goal area was successfully found, false otherwise
	*/
	virtual bool				 RouteToGoalArea( int areaNum, const idVec3 origin, int goalAreaNum, int travelFlags, int& travelTime, idReachability** reach ) const = 0;

	/*!
		\brief Calculates a walking path from a starting position to a goal position within the AAS navigation system.

		This function determines a walkable path from the specified start area and origin to the goal area and origin. It uses the AAS file to find valid reachabilities and checks for path validity.
	   The function handles cases where the start and goal are in the same area, and it includes logic to prevent infinite loops by tracking previously visited areas. It also supports path
	   optimization and can return different path types such as walk off ledge, barrier jump, or regular jump depending on the travel type of the final reachability. The function returns true if a
	   path is successfully calculated, false otherwise.

		\param path The path structure to be filled with the calculated route information
		\param areaNum The number of the starting area
		\param origin The starting position within the starting area
		\param goalAreaNum The number of the goal area
		\param goalOrigin The goal position within the goal area
		\param travelFlags Flags specifying the travel types allowed for this path calculation
		\return True if a valid path is calculated, false if the path calculation fails or no valid path exists
	*/
	virtual bool				 WalkPathToGoal( aasPath_t& path, int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags ) const = 0;

	/*!
		\brief Determines if a valid walking path exists between two points in the AAS navigation system.

		This function evaluates whether it's possible to walk in a straight line from the origin to the goal origin, considering the specified travel flags and area constraints. It performs path
	   validation by checking reachability between areas, ensuring that the movement remains within allowed travel types and does not require undesirable travel flags. The function also accounts for
	   step height limitations and ledge avoidance. If a valid path is found, it updates the end position and area number with the final location of the walk.

		\param areaNum The starting area number
		\param origin The starting position in 3D space
		\param goalAreaNum The target area number
		\param goalOrigin The target position in 3D space
		\param travelFlags Flags specifying allowed travel types
		\param endPos Output parameter for the ending position in 3D space
		\param endAreaNum Output parameter for the ending area number
		\return True if a valid walking path exists between origin and goalOrigin, false otherwise
	*/
	virtual bool				 WalkPathValid( int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags, idVec3& endPos, int& endAreaNum ) const = 0;

	/*!
		\brief Computes a flying path from a starting position to a goal position within the AAS navigation system.

		This function calculates a path for flying movement from a specified start area and position to a goal area and position. It uses the AAS file to determine valid movement routes and handles
	   special cases such as reaching the goal area or encountering invalid paths. The function returns true if a valid path is found, and false otherwise. The path is stored in the provided aasPath_t
	   structure. The implementation includes logic to prevent infinite loops by tracking recently visited areas.

		\param path Structure to store the computed path information including type, goal positions, and area numbers
		\param areaNum The starting area number in the AAS
		\param origin The starting position in world coordinates
		\param goalAreaNum The target area number in the AAS
		\param goalOrigin The target position in world coordinates
		\param travelFlags Flags that specify the travel types allowed for the path
		\return True if a valid path is found, false otherwise
	*/
	virtual bool				 FlyPathToGoal( aasPath_t& path, int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags ) const = 0;

	/*!
		\brief Checks if a straight-line flight path between two points is valid in the AAS system.

		This function determines whether it is possible to fly in a straight line from the origin point to the goal origin point without colliding with any obstacles. It uses the AAS file's trace
	   functionality to check for collisions. If the AAS file is not available, it assumes the path is valid and returns true. The function populates the end position and end area number with the
	   results of the trace operation.

		\param areaNum Starting area number for the flight path
		\param origin Starting position for the flight path
		\param goalAreaNum Target area number for the flight path
		\param goalOrigin Target position for the flight path
		\param travelFlags Flags indicating travel restrictions or preferences
		\param endPos Output parameter for the end position reached
		\param endAreaNum Output parameter for the end area number reached
		\return True if the straight-line flight path is valid and unobstructed, false otherwise.
	*/
	virtual bool				 FlyPathValid( int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags, idVec3& endPos, int& endAreaNum ) const = 0;

	//! Displays the walk path from an origin point to a goal area using debug arrows.
	virtual void				 ShowWalkPath( const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin ) const = 0;

	//! Visualizes a flying path from an origin point to a goal area in the AAS.
	virtual void				 ShowFlyPath( const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin ) const = 0;

	/*!
		\brief Finds the nearest goal area for pathfinding based on the given parameters.

		This function performs a pathfinding search to locate the nearest goal area starting from a specified area. It uses a routing algorithm that considers travel flags, obstacles, and a callback
	   function to determine valid goal areas. The function evaluates reachabilities between areas, calculates travel times, and accounts for obstacles in the path. It returns true if a valid goal
	   area is found, otherwise false.

		\param goal Output parameter that will hold the resulting goal information including area number and origin point
		\param areaNum Starting area number for the search
		\param origin Starting origin point for the search
		\param target Target point used to determine closeness to the goal
		\param travelFlags Travel flags that define allowed travel types
		\param obstacles Array of obstacles that must be avoided during pathfinding
		\param numObstacles Number of obstacles in the obstacles array
		\param callback Callback function used to determine if an area is a valid goal
		\return true if a valid goal area is found and the goal parameter is updated, false otherwise
	*/
	virtual bool				 FindNearestGoal(
						aasGoal_t& goal, int areaNum, const idVec3 origin, const idVec3& target, int travelFlags, aasObstacle_t* obstacles, int numObstacles, idAASCallback& callback ) const = 0;

	// jmarshall

	//! Returns the default search bounds for the local AAS file.
	virtual const idBounds& DefaultSearchBounds() const = 0;

	//! Returns the AAS area number for the given origin position after adjusting it to be within the area.
	virtual int				AdjustPositionAndGetArea( idVec3& origin ) = 0;

	//! Displays information about the AAS area containing the given origin point
	virtual void			ShowArea( const idVec3& origin ) const = 0;

	//! Draws the navigation areas and their reachabilities for the local AAS system.
	virtual void			DrawAreas() const = 0;

	//! Returns the AAS file associated with this local AAS instance.
	virtual idAASFile*		GetAASFile() = 0;

	//! Draws the edges of the specified AAS area.
	virtual void			DrawArea( int areaNum ) const = 0;

	// jmarshall end
};

#endif /* !__AAS_H__ */
