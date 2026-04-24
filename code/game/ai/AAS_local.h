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

#ifndef __AAS_LOCAL_H__
#define __AAS_LOCAL_H__

#include "AAS.h"
#include "../Pvs.h"

/*!
	\class idRoutingCache
	\brief A caching mechanism for storing and managing routing information.
*/
class idRoutingCache
{
	friend class idAASLocal;

public:
	//! Constructs an idRoutingCache object with the specified size.
	idRoutingCache( int size );

	//! Destructor for idRoutingCache that frees allocated memory for reachabilities and travel times.
	~idRoutingCache();

	//! Returns the total memory size of the routing cache object including its dynamic arrays.
	int Size() const;

private:
	int				type;			 // portal or area cache
	int				size;			 // size of cache
	int				cluster;		 // cluster of the cache
	int				areaNum;		 // area of the cache
	int				travelFlags;	 // combinations of the travel flags
	idRoutingCache* next;			 // next in list
	idRoutingCache* prev;			 // previous in list
	idRoutingCache* time_next;		 // next in time based list
	idRoutingCache* time_prev;		 // previous in time based list
	unsigned short	startTravelTime; // travel time to start with
	unsigned char*	reachabilities;	 // reachabilities used for routing
	unsigned short* travelTimes;	 // travel time for every area
};

/*!
	\class idRoutingUpdate
	\brief Handles routing updates in a navigation system.
*/
class idRoutingUpdate
{
	friend class idAASLocal;

private:
	int				 cluster;		  // cluster number of this update
	int				 areaNum;		  // area number of this update
	unsigned short	 tmpTravelTime;	  // temporary travel time
	unsigned short*	 areaTravelTimes; // travel times within the area
	idVec3			 start;			  // start point into area
	idRoutingUpdate* next;			  // next in list
	idRoutingUpdate* prev;			  // prev in list
	bool			 isInList;		  // true if the update is in the list
};

/*!
	\class idRoutingObstacle
	\brief Represents an obstacle in a routing system.
*/
class idRoutingObstacle
{
	friend class idAASLocal;

	//! Initializes a new instance of the idRoutingObstacle class.
	idRoutingObstacle() { }

private:
	idBounds			 bounds; // obstacle bounds
	idList<int, TAG_AAS> areas;	 // areas the bounds are in
};

/*!
	\class idAASLocal
	\brief Provides local navigation data and routing services for a specific map.

	This class implements local navigation data handling and routing services for a specific map file, extending the functionality of the base idAAS class. It manages AAS file data, area information,
   and pathfinding capabilities for navigation within a single map. The class supports initialization, shutdown, and various navigation operations including area queries, pathfinding, obstacle
   management, and routing cache handling. It provides methods for calculating travel times, finding reachable areas, performing trace operations, and managing routing states. The class maintains
   internal data structures for efficient pathfinding and includes functionality for debugging and visualization of navigation data.

*/
class idAASLocal : public idAAS
{
public:
	//! Initializes an idAASLocal object with no associated file.
	idAASLocal();

	//! Destroys the idAASLocal object and shuts down associated resources.
	virtual ~idAASLocal();

	//! Initializes the AAS local instance with the specified map name and CRC.
	virtual bool				 Init( const idStr& mapName, unsigned int mapFileCRC );

	//! Shuts down the AAS local instance and releases all associated resources.
	virtual void				 Shutdown();

	//! Outputs statistics for the AAS file
	virtual void				 Stats() const;

	//! Tests AAS functionality at the specified origin position.
	virtual void				 Test( const idVec3& origin );

	//! Returns the AAS settings for this local AAS file.
	virtual const idAASSettings* GetSettings() const;

	//! Returns the area number of the given point in the AAS file.
	virtual int					 PointAreaNum( const idVec3& origin ) const;

	//! Returns the area number of the first reachable area from a given point within specified bounds and travel flags
	virtual int					 PointReachableAreaNum( const idVec3& origin, const idBounds& searchBounds, const int areaFlags ) const;

	//! Returns the number of the first reachable area found within the specified bounds, considering given area flags and travel flags.
	virtual int					 BoundsReachableAreaNum( const idBounds& bounds, const int areaFlags ) const;

	//! Moves a point into the specified area if it is not already within that area.
	virtual void				 PushPointIntoAreaNum( int areaNum, idVec3& origin ) const;

	//! Returns the center point of the specified AAS area
	virtual idVec3				 AreaCenter( int areaNum ) const;

	//! Returns the flags for a specified area in the AAS file.
	virtual int					 AreaFlags( int areaNum ) const;

	//! Returns the travel flags for a specified area in the AAS file.
	virtual int					 AreaTravelFlags( int areaNum ) const;

	//! Performs a trace operation through the AAS file from start to end positions and returns true if the trace was successful.
	virtual bool				 Trace( aasTrace_t& trace, const idVec3& start, const idVec3& end ) const;

	//! Returns a const reference to the plane at the specified index from the AAS file.
	virtual const idPlane&		 GetPlane( int planeNum ) const;

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
	virtual int					 GetWallEdges( int areaNum, const idBounds& bounds, int travelFlags, int* edges, int maxEdges ) const;

	//! Sorts wall edges into sequences based on connectivity
	virtual void				 SortWallEdges( int* edges, int numEdges ) const;

	//! Retrieves the vertex numbers for a given edge in the AAS file.
	virtual void				 GetEdgeVertexNumbers( int edgeNum, int verts[2] ) const;

	//! Retrieves the start and end vertices of a specified edge from the AAS file
	virtual void				 GetEdge( int edgeNum, idVec3& start, idVec3& end ) const;

	//! Sets the routing state of areas within the specified bounds based on their contents.
	virtual bool				 SetAreaState( const idBounds& bounds, const int areaContents, bool disabled );

	//! Adds a routing obstacle to the AAS local instance and returns its handle.
	virtual aasHandle_t			 AddObstacle( const idBounds& bounds );

	//! Removes an obstacle from the AAS local navigation system using the provided handle
	virtual void				 RemoveObstacle( const aasHandle_t handle );

	//! Removes all obstacles from the local AAS file.
	virtual void				 RemoveAllObstacles();

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
	virtual int					 TravelTimeToGoalArea( int areaNum, const idVec3& origin, int goalAreaNum, int travelFlags ) const;

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
	virtual bool				 RouteToGoalArea( int areaNum, const idVec3 origin, int goalAreaNum, int travelFlags, int& travelTime, idReachability** reach ) const;

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
	virtual bool				 WalkPathToGoal( aasPath_t& path, int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags ) const;

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
	virtual bool				 WalkPathValid( int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags, idVec3& endPos, int& endAreaNum ) const;

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
	virtual bool				 FlyPathToGoal( aasPath_t& path, int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags ) const;

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
	virtual bool				 FlyPathValid( int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin, int travelFlags, idVec3& endPos, int& endAreaNum ) const;

	//! Displays the walk path from an origin point to a goal area using debug arrows.
	virtual void				 ShowWalkPath( const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin ) const;

	//! Visualizes a flying path from an origin point to a goal area in the AAS.
	virtual void				 ShowFlyPath( const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin ) const;

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
	virtual bool FindNearestGoal( aasGoal_t& goal, int areaNum, const idVec3 origin, const idVec3& target, int travelFlags, aasObstacle_t* obstacles, int numObstacles, idAASCallback& callback ) const;

	//! Returns the default search bounds for the local AAS file.
	virtual const idBounds& DefaultSearchBounds() const;

	//! Returns the AAS area number for the given origin position after adjusting it to be within the area.
	virtual int				AdjustPositionAndGetArea( idVec3& origin );

	//! Displays information about the AAS area containing the given origin point
	virtual void			ShowArea( const idVec3& origin ) const;

	//! Draws the navigation areas and their reachabilities for the local AAS system.
	virtual void			DrawAreas() const;

	//! Returns the AAS file associated with this local AAS instance.
	virtual idAASFile*		GetAASFile() { return file; }

	//! Draws the edges of the specified AAS area.
	virtual void			DrawArea( int areaNum ) const;
	// jmarshall end
private:
	idAASFile* file;
	idStr	   name;

private:													  // routing data
	idRoutingCache***					areaCacheIndex;		  // for each area in each cluster the travel times to all other areas in the cluster
	int									areaCacheIndexSize;	  // number of area cache entries
	idRoutingCache**					portalCacheIndex;	  // for each area in the world the travel times from each portal
	int									portalCacheIndexSize; // number of portal cache entries
	idRoutingUpdate*					areaUpdate;			  // memory used to update the area routing cache
	idRoutingUpdate*					portalUpdate;		  // memory used to update the portal routing cache
	unsigned short*						goalAreaTravelTimes;  // travel times to goal areas
	unsigned short*						areaTravelTimes;	  // travel times through the areas
	int									numAreaTravelTimes;	  // number of area travel times
	mutable idRoutingCache*				cacheListStart;		  // start of list with cache sorted from oldest to newest
	mutable idRoutingCache*				cacheListEnd;		  // end of list with cache sorted from oldest to newest
	mutable int							totalCacheMemory;	  // total cache memory used
	idList<idRoutingObstacle*, TAG_AAS> obstacleList;		  // list with obstacles

private:
	// ROUTING ------------------------------------

	//! Initializes the routing system for the local AAS.
	bool			SetupRouting();

	//! Cleans up routing-related resources by deleting area travel times and shutting down the routing cache.
	void			ShutdownRouting();

	//! Returns the estimated travel time between two points within a specified AAS area.
	unsigned short	AreaTravelTime( int areaNum, const idVec3& start, const idVec3& end ) const;

	//! Computes travel times between reachabilities for all areas in the AAS file
	void			CalculateAreaTravelTimes();

	//! Frees the memory allocated for area travel times and resets the associated counters and pointers.
	void			DeleteAreaTravelTimes();

	//! Initializes the routing cache structures for the local AAS instance.
	void			SetupRoutingCache();

	//! Deletes the cache for a specified cluster in the local AAS.
	void			DeleteClusterCache( int clusterNum );

	//! Deletes all portal cache entries for the local AAS instance.
	void			DeletePortalCache();

	//! Clears and frees all memory associated with the routing cache.
	void			ShutdownRoutingCache();

	//! Prints routing cache statistics for area and portal caches.
	void			RoutingStats() const;

	//! Links a routing cache into the cache list in order from oldest to newest.
	void			LinkCache( idRoutingCache* cache ) const;

	//! Removes a routing cache from the linked list of cached entries.
	void			UnlinkCache( idRoutingCache* cache ) const;

	//! Removes the oldest cache entry from the routing cache list and deallocates its memory.
	void			DeleteOldestCache() const;

	//! Returns the reachability for a given area and reachability number.
	idReachability* GetAreaReachability( int areaNum, int reachabilityNum ) const;

	//! Returns the cluster area number for the given cluster and area numbers.
	int				ClusterAreaNum( int clusterNum, int areaNum ) const;

	//! Updates the routing cache for a specific area in the AAS local navigation system.
	void			UpdateAreaRoutingCache( idRoutingCache* areaCache ) const;

	//! Retrieves or creates a routing cache for a specific area within a cluster, considering travel flags.
	idRoutingCache* GetAreaRoutingCache( int clusterNum, int areaNum, int travelFlags ) const;

	//! Updates the portal routing cache for the specified portal cache
	void			UpdatePortalRoutingCache( idRoutingCache* portalCache ) const;

	//! Retrieves or creates a portal routing cache for the specified cluster, area, and travel flags.
	idRoutingCache* GetPortalRoutingCache( int clusterNum, int areaNum, int travelFlags ) const;

	//! Removes routing cache associated with a specified area number.
	void			RemoveRoutingCacheUsingArea( int areaNum );

	//! Disables a specific area in the AAS file by marking it as invalid and removing associated routing cache.
	void			DisableArea( int areaNum );

	//! Enables a specified area by removing the invalid travel flag and clearing related routing cache.
	void			EnableArea( int areaNum );

	/*!
		\brief Recursively sets the state of AAS areas based on the specified bounds and content flags.

		This function traverses the AAS tree starting from the given node number and determines whether the specified bounds intersect with areas that match the given content flags. If a match is
	   found, it enables or disables the area accordingly. The function handles cluster portals by checking if the area contents match the specified flags and then calling the appropriate enable or
	   disable function.

		\param nodeNum The starting node number in the AAS tree
		\param bounds The bounding volume used to test area intersections
		\param areaContents The content flags used to filter areas
		\param disabled Flag indicating whether to enable or disable matching areas
		\return True if a cluster portal was found and processed, false otherwise.
	*/
	bool			SetAreaState_r( int nodeNum, const idBounds& bounds, const int areaContents, bool disabled );

	//! Recursively finds all AAS areas that intersect with the given bounding box.
	void			GetBoundsAreas_r( int nodeNum, const idBounds& bounds, idList<int>& areas ) const;

	//! Sets the routing obstacle state for a given obstacle by enabling or disabling reachabilities.
	void			SetObstacleState( const idRoutingObstacle* obstacle, bool enable );

private: // pathing
		 //! Calculates the split point of an edge with a plane and returns true if the point lies between the edge's vertices.
	bool   EdgeSplitPoint( idVec3& split, int edgeNum, const idPlane& plane ) const;

	/*!
		\brief Calculates either the closest or furthest point on the floor of an area that lies on a specified plane and is on the front side of another plane

		This function finds a split point on the floor faces of a specified area. It iterates through all floor faces and their edges to calculate potential split points. The point must satisfy two
	   conditions: it must lie on the pathPlane and be on the front side of the frontPlane. The function can find either the closest point (when closest is true) or the furthest point (when closest is
	   false) based on the distance to the frontPlane. The distance is measured from the frontPlane to the calculated split point.

		\param split Output parameter that receives the calculated split point
		\param areaNum The number of the area to process
		\param splitPlane The plane used to determine valid split points
		\param frontPlane The plane used to validate that the split point is on the front side
		\param closest Flag indicating whether to find the closest (true) or furthest (false) point
		\return True if a valid split point was found and meets the criteria, false otherwise
	*/
	bool   FloorEdgeSplitPoint( idVec3& split, int areaNum, const idPlane& splitPlane, const idPlane& frontPlane, bool closest ) const;

	/*!
		\brief Computes a subdivided walk path between two points in an AAS area, returning the furthest valid position.

		This function calculates intermediate points between a start and end position based on a sampling distance. It checks each sample point for walkability using the WalkPathValid method. If a
	   point is not valid or exceeds the maximum walk path distance, the function returns the last valid position. The end area number is updated to reflect the area of the last valid point.

		\param areaNum The number of the AAS area to perform the walk path sampling in
		\param origin The origin point used for path validation calculations
		\param start The starting position of the walk path
		\param end The ending position of the walk path
		\param travelFlags Flags indicating travel capabilities or restrictions for the path
		\param endAreaNum Reference to an integer that will be updated with the area number of the last valid point
		\return The last valid position along the sampled path, or the start position if no valid samples are found
	*/
	idVec3 SubSampleWalkPath( int areaNum, const idVec3& origin, const idVec3& start, const idVec3& end, int travelFlags, int& endAreaNum ) const;

	/*!
		\brief Samples a flight path between two points in an AAS system to find a valid endpoint.

		This function performs sub-sampling along a flight path from a start point to an end point, checking for valid flight conditions at each sample point. It returns the last valid point if the
	   path is valid or the point at which the path becomes invalid. The function uses a specified travel flags mask to determine valid travel conditions. The end area number is updated to reflect the
	   area containing the returned point.

		\param areaNum The area number to start from for the flight path
		\param origin The origin point for the flight path calculation
		\param start The starting point of the flight path
		\param end The ending point of the flight path
		\param travelFlags Travel flags to determine valid flight conditions
		\param endAreaNum Reference to an integer that will be set to the area number of the valid endpoint
		\return The last valid point along the flight path, or the point where the path became invalid
	*/
	idVec3 SubSampleFlyPath( int areaNum, const idVec3& origin, const idVec3& start, const idVec3& end, int travelFlags, int& endAreaNum ) const;

private:
	// DEBUG -------------------------------------------

	/*!
		\brief Draws a cone shape using debug lines in the render world

		This function renders a cone shape in the debug visualization system using the provided origin, direction, radius, and color parameters. The cone is drawn by creating a circular base at the
	   origin point and extending it along the direction vector. The function uses a series of line segments to approximate the conical surface and the base circle. The direction vector determines the
	   orientation of the cone axis, while the radius controls the size of the base and the overall shape.

		\param origin The starting point of the cone axis
		\param dir The direction vector along which the cone extends
		\param radius The radius of the cone base
		\param color The color to use for drawing the cone lines
	*/
	void DrawCone( const idVec3& origin, const idVec3& dir, float radius, const idVec4& color ) const;

	//! Draws a face of the AAS data structure with an optional side indicator.
	void DrawFace( int faceNum, bool side ) const;

	//! Draws an edge of the AAS graph with optional arrow indicator
	void DrawEdge( int edgeNum, bool arrow ) const;

	//! Draws a visual representation of a reachability link between two points in the game world.
	void DrawReachability( const idReachability* reach ) const;

	//! Displays the wall edges of the area containing the specified origin.
	void ShowWallEdges( const idVec3& origin ) const;

	//! Displays or hides a navigation area based on the target area number and origin position.
	void ShowHideArea( const idVec3& origin, int targerAreaNum ) const;

	//! Moves the player towards a specified area using physics-based velocity adjustment.
	bool PullPlayer( const idVec3& origin, int toAreaNum ) const;

	//! Sets a random area for player pulling if the current pull player setting is not valid.
	void RandomPullPlayer( const idVec3& origin ) const;

	//! Visualizes the result of pushing a point into the nearest reachable area by drawing a debug arrow from the origin to the pushed target position.
	void ShowPushIntoArea( const idVec3& origin ) const;
};

#endif /* !__AAS_LOCAL_H__ */
