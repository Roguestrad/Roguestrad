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

#ifndef __AASFILE_H__
#define __AASFILE_H__

/*
===============================================================================

	AAS File

===============================================================================
*/

#define AAS_FILEID				   "DewmAAS"
#define AAS_FILEVERSION			   "1.07"
// jmarshall
#define AAS_ICE_FILEVERSION		   "1.07-ICE"
// jmarshall end

// travel flags
#define TFL_INVALID				   BIT( 0 )	 // not valid
#define TFL_WALK				   BIT( 1 )	 // walking
#define TFL_CROUCH				   BIT( 2 )	 // crouching
#define TFL_WALKOFFLEDGE		   BIT( 3 )	 // walking of a ledge
#define TFL_BARRIERJUMP			   BIT( 4 )	 // jumping onto a barrier
#define TFL_JUMP				   BIT( 5 )	 // jumping
#define TFL_LADDER				   BIT( 6 )	 // climbing a ladder
#define TFL_SWIM				   BIT( 7 )	 // swimming
#define TFL_WATERJUMP			   BIT( 8 )	 // jump out of the water
#define TFL_TELEPORT			   BIT( 9 )	 // teleportation
#define TFL_ELEVATOR			   BIT( 10 ) // travel by elevator
#define TFL_FLY					   BIT( 11 ) // fly
#define TFL_SPECIAL				   BIT( 12 ) // special
#define TFL_WATER				   BIT( 21 ) // travel through water
#define TFL_AIR					   BIT( 22 ) // travel through air

// face flags
#define FACE_SOLID				   BIT( 0 ) // solid at the other side
#define FACE_LADDER				   BIT( 1 ) // ladder surface
#define FACE_FLOOR				   BIT( 2 ) // standing on floor when on this face
#define FACE_LIQUID				   BIT( 3 ) // face seperating two areas with liquid
#define FACE_LIQUIDSURFACE		   BIT( 4 ) // face seperating liquid and air

// area flags
#define AREA_FLOOR				   BIT( 0 ) // AI can stand on the floor in this area
#define AREA_GAP				   BIT( 1 ) // area has a gap
#define AREA_LEDGE				   BIT( 2 ) // if entered the AI bbox partly floats above a ledge
#define AREA_LADDER				   BIT( 3 ) // area contains one or more ladder faces
#define AREA_LIQUID				   BIT( 4 ) // area contains a liquid
#define AREA_CROUCH				   BIT( 5 ) // AI cannot walk but can only crouch in this area
#define AREA_REACHABLE_WALK		   BIT( 6 ) // area is reachable by walking or swimming
#define AREA_REACHABLE_FLY		   BIT( 7 ) // area is reachable by flying

// area contents flags
#define AREACONTENTS_SOLID		   BIT( 0 ) // solid, not a valid area
#define AREACONTENTS_WATER		   BIT( 1 ) // area contains water
#define AREACONTENTS_CLUSTERPORTAL BIT( 2 ) // area is a cluster portal
#define AREACONTENTS_OBSTACLE	   BIT( 3 ) // area contains (part of) a dynamic obstacle
#define AREACONTENTS_TELEPORTER	   BIT( 4 ) // area contains (part of) a teleporter trigger

// bits for different bboxes
#define AREACONTENTS_BBOX_BIT	   24

#define MAX_REACH_PER_AREA		   256
#define MAX_AAS_TREE_DEPTH		   128

#define MAX_AAS_BOUNDING_BOXES	   4

/*!
	\class idReachability
	\brief The class represents reachability information between different areas in a navigation graph.
*/
class idReachability
{
public:
	int				travelType;		 // type of travel required to get to the area
	short			toAreaNum;		 // number of the reachable area
	short			fromAreaNum;	 // number of area the reachability starts
	idVec3			start;			 // start point of inter area movement
	idVec3			end;			 // end point of inter area movement
	int				edgeNum;		 // edge crossed by this reachability
	unsigned short	travelTime;		 // travel time of the inter area movement
	byte			number;			 // reachability number within the fromAreaNum (must be < 256)
	byte			disableCount;	 // number of times this reachability has been disabled
	idReachability* next;			 // next reachability in list
	idReachability* rev_next;		 // next reachability in reversed list
	unsigned short* areaTravelTimes; // travel times within the fromAreaNum from reachabilities that lead towards this area
public:
	//! Copies the base properties from another reachability object.
	void CopyBase( idReachability& reach );
};

/*!
	\class idReachability_Walk
	\brief A class representing walkable reachability information for navigation.
*/
class idReachability_Walk : public idReachability
{
};

/*!
	\class idReachability_BarrierJump
	\brief A reachability class for barrier jump movements.
*/
class idReachability_BarrierJump : public idReachability
{
};

/*!
	\class idReachability_WaterJump
	\brief Represents a water jump reachability for navigation mesh pathfinding.
*/
class idReachability_WaterJump : public idReachability
{
};

/*!
	\class idReachability_WalkOffLedge
	\brief A reachability type representing movement off a ledge by walking.
*/
class idReachability_WalkOffLedge : public idReachability
{
};

/*!
	\class idReachability_Swim
	\brief A specialized reachability class for swimming movement in a game environment.
*/
class idReachability_Swim : public idReachability
{
};

/*!
	\class idReachability_Fly
	\brief A specialized reachability implementation for flying movement navigation.
*/
class idReachability_Fly : public idReachability
{
};

/*!
	\class idReachability_Special
	\brief Specialized reachability implementation for advanced navigation.
*/
class idReachability_Special : public idReachability
{
public:
	idDict dict;
};

// index
typedef int	   aasIndex_t;

// vertex
typedef idVec3 aasVertex_t;

// edge
typedef struct aasEdge_s {
	int vertexNum[2]; // numbers of the vertexes of this edge
} aasEdge_t;

// area boundary face
typedef struct aasFace_s {
	unsigned short planeNum;  // number of the plane this face is on
	unsigned short flags;	  // face flags
	int			   numEdges;  // number of edges in the boundary of the face
	int			   firstEdge; // first edge in the edge index
	short		   areas[2];  // area at the front and back of this face
} aasFace_t;

// area with a boundary of faces
typedef struct aasArea_s {
	int				numFaces;		// number of faces used for the boundary of the area
	int				firstFace;		// first face in the face index used for the boundary of the area
	idBounds		bounds;			// bounds of the area
	idVec3			center;			// center of the area an AI can move towards
	unsigned short	flags;			// several area flags
	unsigned short	contents;		// contents of the area
	short			cluster;		// cluster the area belongs to, if negative it's a portal
	short			clusterAreaNum; // number of the area in the cluster
	int				travelFlags;	// travel flags for traveling through this area
	idReachability* reach;			// reachabilities that start from this area
	idReachability* rev_reach;		// reachabilities that lead to this area
									// jmarshall
	int				firstEdge;
	int				numEdges;
	// jmarshall end
} aasArea_t;

// nodes of the bsp tree
typedef struct aasNode_s {
	unsigned short planeNum;	// number of the plane that splits the subspace at this node
	int			   children[2]; // child nodes, zero is solid, negative is -(area number)
} aasNode_t;

// cluster portal
typedef struct aasPortal_s {
	short		   areaNum;			  // number of the area that is the actual portal
	short		   clusters[2];		  // number of cluster at the front and back of the portal
	short		   clusterAreaNum[2]; // number of this portal area in the front and back cluster
	unsigned short maxAreaTravelTime; // maximum travel time through the portal area
} aasPortal_t;

// cluster
typedef struct aasCluster_s {
	int numAreas;		   // number of areas in the cluster
	int numReachableAreas; // number of areas with reachabilities
	int numPortals;		   // number of cluster portals
	int firstPortal;	   // first cluster portal in the index
} aasCluster_t;

// trace through the world
typedef struct aasTrace_s {
	// parameters
	int		flags;		   // areas with these flags block the trace
	int		travelFlags;   // areas with these travel flags block the trace
	int		maxAreas;	   // size of the 'areas' array
	int		getOutOfSolid; // trace out of solid if the trace starts in solid
	// output
	float	fraction;		 // fraction of trace completed
	idVec3	endpos;			 // end position of trace
	int		planeNum;		 // plane hit
	int		lastAreaNum;	 // number of last area the trace went through
	int		blockingAreaNum; // area that could not be entered
	int		numAreas;		 // number of areas the trace went through
	int*	areas;			 // array to store areas the trace went through
	idVec3* points;			 // points where the trace entered each new area

	//! Initializes a new instance of the aasTrace_s structure with default values.
	aasTrace_s()
	{
		areas		  = NULL;
		points		  = NULL;
		getOutOfSolid = false;
		flags = travelFlags = maxAreas = 0;
	}
} aasTrace_t;

/*!
	\class idAASSettings
	\brief Manages configuration settings for AAS navigation mesh generation and validation.

	Provides a comprehensive interface for loading, parsing, and validating AAS settings from various sources including files, lexer streams, and dictionaries. The class supports initialization with
   default values and offers methods to check the validity of entity bounds and classnames for AAS processing. It also handles the serialization of settings to files and includes utility functions for
   parsing basic data types and bounding box definitions from token streams.

*/
class idAASSettings
{
public:
	// collision settings
	int		 numBoundingBoxes;
	idBounds boundingBoxes[MAX_AAS_BOUNDING_BOXES];
	bool	 usePatches;
	bool	 writeBrushMap;
	bool	 playerFlood;
	bool	 noOptimize;
	bool	 allowSwimReachabilities;
	bool	 allowFlyReachabilities;
	idStr	 fileExtension;
	// physics settings
	idVec3	 gravity;
	idVec3	 gravityDir;
	idVec3	 invGravityDir;
	float	 gravityValue;
	float	 maxStepHeight;
	float	 maxBarrierHeight;
	float	 maxWaterJumpHeight;
	float	 maxFallHeight;
	float	 minFloorCos;
	// fixed travel times
	int		 tt_barrierJump;
	int		 tt_startCrouching;
	int		 tt_waterJump;
	int		 tt_startWalkOffLedge;

public:
	//! Initializes default settings for AAS navigation mesh generation.
	idAASSettings();

	//! Loads AAS settings from a specified file.
	bool FromFile( const idStr& fileName );

	//! Parses AAS settings from a lexer token stream
	bool FromParser( idLexer& src );

	//! Initializes AAS settings from a dictionary
	bool FromDict( const char* name, const idDict* dict );

	//! Writes the AAS settings to the specified file.
	bool WriteToFile( idFile* fp ) const;

	//! Checks if the given bounds are valid for the AAS settings.
	bool ValidForBounds( const idBounds& bounds ) const;

	//! Checks if an entity class is valid for use with the AAS system based on its definition and bounding box constraints.
	bool ValidEntity( const char* classname ) const;

private:
	//! Parses a boolean value from the lexer input and stores it in the provided boolean reference.
	bool ParseBool( idLexer& src, bool& b );

	//! Parses an integer value from a lexer token stream and stores it in the provided variable.
	bool ParseInt( idLexer& src, int& i );

	//! Parses a float value from the lexer and stores it in the provided variable
	bool ParseFloat( idLexer& src, float& f );

	//! Parses a 3D vector from the lexer input stream and stores it in the provided vector variable.
	bool ParseVector( idLexer& src, idVec3& vec );

	//! Parses bounding box data from a lexer input stream
	bool ParseBBoxes( idLexer& src );
};

/*

-	when a node child is a solid leaf the node child number is zero
-	two adjacent areas (sharing a plane at opposite sides) share a face
	this face is a portal between the areas
-	when an area uses a face from the faceindex with a positive index
	then the face plane normal points into the area
-	the face edges are stored counter clockwise using the edgeindex
-	two adjacent convex areas (sharing a face) only share One face
	this is a simple result of the areas being convex
-	the areas can't have a mixture of ground and gap faces
	other mixtures of faces in one area are allowed
-	areas with the AREACONTENTS_CLUSTERPORTAL in the settings have
	the cluster number set to the negative portal number
-	edge zero is a dummy
-	face zero is a dummy
-	area zero is a dummy
-	node zero is a dummy
-	portal zero is a dummy
-	cluster zero is a dummy

*/

/*!
	\class idAASFile
	\brief Provides access to and management of AAS file data structures.

	This class serves as an abstract interface for accessing geometric and navigational data stored in AAS files. It provides methods to retrieve various components such as planes, vertices, edges,
   faces, areas, nodes, portals, and clusters that define the layout and navigation properties of a level. The interface includes functions for getting basic file information like name and CRC
   checksum, as well as functionality for querying reachability and performing spatial queries. It also exposes settings and travel flag manipulation methods. The class is designed to be extended by
   concrete implementations that provide actual data loading and access.

*/
class idAASFile
{
public:
	virtual ~idAASFile() { }

	//! Returns the name of the AAS file.
	const char*			 GetName() const { return name.c_str(); }

	//! Returns the CRC checksum of the AAS file.
	unsigned int		 GetCRC() const { return crc; }

	//! Returns the number of planes in the AAS file.
	int					 GetNumPlanes() const { return planeList.Num(); }

	//! Returns a reference to the plane at the specified index in the AAS file.
	const idPlane&		 GetPlane( int index ) const { return planeList[index]; }

	//! Returns the number of vertices in the AAS file.
	int					 GetNumVertices() const { return vertices.Num(); }

	//! Returns a const reference to the vertex at the specified index in the AAS file
	const aasVertex_t&	 GetVertex( int index ) const { return vertices[index]; }

	//! Returns the number of edges in the AAS file.
	int					 GetNumEdges() const { return edges.Num(); }

	//! Returns a constant reference to the AAS edge at the specified index.
	const aasEdge_t&	 GetEdge( int index ) const { return edges[index]; }

	//! Returns the number of edge indexes stored in the AAS file.
	int					 GetNumEdgeIndexes() const { return edgeIndex.Num(); }

	//! Returns the edge index at the specified index.
	const aasIndex_t&	 GetEdgeIndex( int index ) const { return edgeIndex[index]; }

	//! Returns the number of faces in the AAS file.
	int					 GetNumFaces() const { return faces.Num(); }

	//! Returns a constant reference to the face at the specified index.
	const aasFace_t&	 GetFace( int index ) const { return faces[index]; }

	//! Returns the number of face indexes in the AAS file.
	int					 GetNumFaceIndexes() const { return faceIndex.Num(); }

	//! Returns a reference to the face index at the specified index in the AAS file.
	const aasIndex_t&	 GetFaceIndex( int index ) const { return faceIndex[index]; }

	//! Returns the number of areas in the AAS file.
	int					 GetNumAreas() const { return areas.Num(); }

	//! Returns a reference to the AAS area at the specified index.
	const aasArea_t&	 GetArea( int index ) { return areas[index]; }

	//! Returns the number of nodes in the AAS file.
	int					 GetNumNodes() const { return nodes.Num(); }

	//! Returns a const reference to the AAS node at the specified index.
	const aasNode_t&	 GetNode( int index ) const { return nodes[index]; }

	//! Returns the number of portals in the AAS file.
	int					 GetNumPortals() const { return portals.Num(); }

	//! Returns a reference to the portal at the specified index.
	const aasPortal_t&	 GetPortal( int index ) { return portals[index]; }

	//! Returns the number of portal indexes in the AAS file.
	int					 GetNumPortalIndexes() const { return portalIndex.Num(); }

	//! Returns a constant reference to the portal index at the specified array index.
	const aasIndex_t&	 GetPortalIndex( int index ) const { return portalIndex[index]; }

	//! Returns the number of clusters in the AAS file.
	int					 GetNumClusters() const { return clusters.Num(); }

	//! Returns a reference to the AAS cluster at the specified index.
	const aasCluster_t&	 GetCluster( int index ) const { return clusters[index]; }

	//! Returns a const reference to the AAS settings stored in the file.
	const idAASSettings& GetSettings() const { return settings; }

	//! Sets the maximum travel time for a portal at the specified index.
	void				 SetPortalMaxTravelTime( int index, int time ) { portals[index].maxAreaTravelTime = time; }

	//! Sets a travel flag for the specified area in the AAS file.
	void				 SetAreaTravelFlag( int index, int flag ) { areas[index].travelFlags |= flag; }

	//! Removes a travel flag from an area in the AAS file.
	void				 RemoveAreaTravelFlag( int index, int flag ) { areas[index].travelFlags &= ~flag; }

	virtual idVec3		 EdgeCenter( int edgeNum ) const = 0;
	virtual idVec3		 FaceCenter( int faceNum ) const = 0;
	virtual idVec3		 AreaCenter( int areaNum ) const = 0;

	virtual idBounds	 EdgeBounds( int edgeNum ) const = 0;
	virtual idBounds	 FaceBounds( int faceNum ) const = 0;
	virtual idBounds	 AreaBounds( int areaNum ) const = 0;
	// jmarshall
	virtual bool		 HasNewFeatures() const = 0;
	// jmarshall end

	virtual int			 PointAreaNum( const idVec3& origin ) const																							  = 0;
	virtual int			 PointReachableAreaNum( const idVec3& origin, const idBounds& searchBounds, const int areaFlags, const int excludeTravelFlags ) const = 0;
	virtual int			 BoundsReachableAreaNum( const idBounds& bounds, const int areaFlags, const int excludeTravelFlags ) const							  = 0;
	virtual void		 PushPointIntoAreaNum( int areaNum, idVec3& point ) const																			  = 0;
	virtual bool		 Trace( aasTrace_t& trace, const idVec3& start, const idVec3& end ) const															  = 0;
	virtual void		 PrintInfo() const																													  = 0;

protected:
	idStr						  name;
	unsigned int				  crc;

	idPlaneSet					  planeList;
	idList<aasVertex_t, TAG_AAS>  vertices;
	idList<aasEdge_t, TAG_AAS>	  edges;
	idList<aasIndex_t, TAG_AAS>	  edgeIndex;
	idList<aasFace_t, TAG_AAS>	  faces;
	idList<aasIndex_t, TAG_AAS>	  faceIndex;
	idList<aasArea_t, TAG_AAS>	  areas;
	idList<aasNode_t, TAG_AAS>	  nodes;
	idList<aasPortal_t, TAG_AAS>  portals;
	idList<aasIndex_t, TAG_AAS>	  portalIndex;
	idList<aasCluster_t, TAG_AAS> clusters;
	idAASSettings				  settings;
};

#endif /* !__AASFILE_H__ */
