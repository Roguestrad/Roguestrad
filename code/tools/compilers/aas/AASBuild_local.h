/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Harrie van Ginneken
Copyright (C) 2022 Robert Beckebans

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

#ifndef __AASBUILD_LOCAL_H__
#define __AASBUILD_LOCAL_H__

#include "../../../engine/aas/AASFile.h"
#include "../../../engine/aas/AASFile_local.h"

#include "Brush.h"
#include "BrushBSP.h"
#include "AASReach.h"
#include "AASCluster.h"

//===============================================================
//
//	idAASBuild
//
//===============================================================

typedef struct aasProcNode_s {
	idPlane plane;
	int		children[2]; // negative numbers are (-1 - areaNumber), 0 = solid
} aasProcNode_t;

/*!
	\class idLedge
	\brief Represents a navigable ledge structure defined by geometric boundaries and gravity orientation.

	The idLedge class encapsulates the geometric properties of a ledge in a navigation system, defined by two endpoint vertices and a gravity direction. It maintains a set of planes that form the
   boundaries of the ledge, which are used for clipping and collision detection. The class supports operations to modify the ledge geometry, such as adding points, creating bevels, and expanding
   boundaries based on spatial constraints. The constructor initializes the ledge with four defining planes, and methods like ChopWinding and PointBetweenBounds provide utilities for spatial queries
   and clipping operations.

*/
class idLedge
{
public:
	idVec3			start;
	idVec3			end;
	idBrushBSPNode* node;
	int				numExpandedPlanes;
	int				numSplitPlanes;
	int				numPlanes;
	idPlane			planes[8];

public:
	//! Default constructor for the idLedge class.
	idLedge();

	/*!
		\brief Constructs an idLedge object representing a ledge defined by two points and a gravity direction within a BSP node

		Initializes a ledge structure using two vertices v1 and v2 that define the ledge's endpoints, a gravity direction to orient the ledge planes, and a BSP node that contains the ledge. The
	   constructor computes four planes that define the boundaries of the ledge, with the first two planes derived from cross products with the gravity direction to establish proper orientation, and
	   the last two planes representing the sides of the ledge. The numPlanes is set to 4 to indicate the number of defining planes for this ledge.

		\param v1 First vertex defining the ledge endpoint
		\param v2 Second vertex defining the ledge endpoint
		\param gravityDir Direction vector representing the gravitational pull for ledge orientation
		\param n BSP node that contains this ledge
	*/
	idLedge( const idVec3& v1, const idVec3& v2, const idVec3& gravityDir, idBrushBSPNode* n );

	//! Adds a point to the ledge, updating the start or end position if the point lies beyond the current plane.
	void	   AddPoint( const idVec3& v );

	//! Creates bevel planes for a ledge based on the provided gravity direction.
	void	   CreateBevels( const idVec3& gravityDir );

	//! Expands the ledge boundaries based on the provided bounds and maximum step height.
	void	   Expand( const idBounds& bounds, float maxStepHeight );

	//! Returns a copy of the input winding clipped by all planes of the ledge.
	idWinding* ChopWinding( const idWinding* winding ) const;

	//! Checks if a point lies within the bounds defined by the ledge's planes
	bool	   PointBetweenBounds( const idVec3& v ) const;
};

/*!
	\class idAASBuild
	\brief Manages the construction and processing of navigation data structures for game environments.

	Handles the conversion of map data into navigational meshes, including brush processing, BSP tree manipulation, and area awareness system generation. Supports procedural BSP operations, ledge and
   gravity subdivision, and vertex/edge hashing for efficient data storage. Provides methods for building AAS files, managing entity contents, and processing map geometry into AAS-compatible formats.
   The class orchestrates the entire AAS building pipeline from raw map data through to final navigation mesh output.

*/
class idAASBuild
{
public:
	//! Initializes a new instance of the idAASBuild class with default values.
	idAASBuild();

	//! Destroys the idAASBuild object and performs cleanup.
	~idAASBuild();

	//! Builds an AAS file from a map file using the specified settings and returns true if successful.
	bool Build( const idStr& fileName, const idAASSettings* settings );

	//! Builds AAS reachability data for the specified map file using the provided settings.
	bool BuildReachability( const idStr& fileName, const idAASSettings* settings );

	//! Shuts down the AAS build system and releases all allocated resources.
	void Shutdown();

private:
	const idAASSettings* aasSettings;
	idAASFileLocal*		 file;
	aasProcNode_t*		 procNodes;
	int					 numProcNodes;
	int					 numGravitationalSubdivisions;
	int					 numMergedLeafNodes;
	int					 numLedgeSubdivisions;
	idList<idLedge>		 ledgeList;
	idBrushMap*			 ledgeMap;

private:
	// map loading ----------------------------

	//! Parses procedural nodes from a lexer input stream.
	void		ParseProcNodes( idLexer* src );

	//! Loads a procedural BSP file for AAS building
	bool		LoadProcBSP( const char* name, ID_TIME_T minFileTime );

	//! Deletes the procedural BSP data structures.
	void		DeleteProcBSP();

	/*!
		\brief Determines if a winding is completely removed by the procedural BSP tree

		This function checks whether a given winding is entirely clipped away by the procedural BSP tree structure. It traverses the tree starting from the specified node, evaluating the winding's
	   relationship with each node's splitting plane. The function recursively processes child nodes based on the winding's position relative to the current node's plane. If the winding is entirely on
	   one side of a plane or is split, the function continues traversal accordingly. The process stops when a leaf node is reached or when the winding is determined to be fully removed by the tree.
	   The function considers the winding's normal and origin to determine traversal direction when the winding lies on the splitting plane.

		\param nodeNum Index of the starting node in the procedural BSP tree
		\param w Pointer to the winding to be evaluated
		\param normal Normal vector of the winding's plane
		\param origin Origin point of the winding's plane
		\param radius Radius used for distance comparisons with the BSP tree nodes
		\return True if the winding is not completely removed by the BSP tree, false otherwise
	*/
	bool		ChoppedAwayByProcBSP( int nodeNum, idFixedWinding* w, const idVec3& normal, const idVec3& origin, const float radius );

	//! Clips brush sides using the procedural BSP tree data.
	void		ClipBrushSidesWithProcBSP( idBrushList& brushList );

	//! Converts engine content flags to AAS area content flags
	int			ContentsForAAS( int contents );

	/*!
		\brief Converts a map brush into AAS-compatible brushes, applying transformation and filtering out invalid geometry.

		This function processes a map brush by extracting its sides, converting them into brush sides with appropriate content flags, and creating one or more idBrush objects. The brush geometry is
	   transformed using the provided origin and axis, and invalid or degenerate brushes are discarded. The resulting brushes are added to the provided brush list for further AAS processing.

		\param mapBrush The map brush to be converted into AAS brushes
		\param origin The origin offset to apply to the brush geometry
		\param axis The transformation matrix to apply to the brush geometry
		\param entityNum The entity number associated with the brush
		\param primitiveNum The primitive number within the entity for identification
		\param brushList The list to which newly created brushes are appended
		\return The updated brush list with new brushes added
	*/
	idBrushList AddBrushesForMapBrush( const idMapBrush* mapBrush, const idVec3& origin, const idMat3& axis, int entityNum, int primitiveNum, idBrushList brushList );

	/*!
		\brief Converts a map patch into a list of brushes for AAS building, applying transformations and content flags.

		This function takes a map patch and converts it into a set of brushes that can be used for AAS (Area Awareness System) building. It handles subdivision of the patch, creates triangular or
	   quadrilateral brush faces from the patch vertices, and applies the appropriate content flags based on the material. The resulting brushes are transformed by the provided origin and axis, and
	   added to the input brush list. The function checks for degenerate cases and logs warnings if necessary.

		\param mapPatch The map patch to convert into brushes
		\param origin The origin transformation to apply to the brushes
		\param axis The axis transformation to apply to the brushes
		\param entityNum The entity number to assign to the brushes
		\param primitiveNum The primitive number to assign to the brushes
		\param brushList The list to which the generated brushes will be added
		\return The updated brush list with the newly created brushes added to it
	*/
	idBrushList AddBrushesForMapPatch( const idMapPatch* mapPatch, const idVec3& origin, const idMat3& axis, int entityNum, int primitiveNum, idBrushList brushList );

	/*!
		\brief Creates brushes from a polygon mesh for AAS build, transforming and adding them to the brush list

		This function processes a map polygon mesh and converts each polygon face into a brush for AAS (Area Awareness System) building. It transforms the mesh vertices using the provided origin and
	   axis, creates triangular brushes from the polygon faces, and adds valid brushes to the brush list. The function checks for valid material contents and skips polygons that don't contribute to
	   the AAS. Each brush is assigned to the specified entity and primitive number, and is marked as a patch type brush.

		\param mapMesh Pointer to the map polygon mesh containing the faces to convert into brushes
		\param origin The origin transformation to apply to the mesh vertices
		\param axis The axis transformation matrix to apply to the mesh vertices
		\param entityNum The entity number to assign to the created brushes
		\param primitiveNum The primitive number to assign to the created brushes
		\param brushList The list to add the created brushes to
		\return The updated brush list containing the newly created brushes
	*/
	idBrushList AddBrushesForMapPolygonMesh( const MapPolygonMesh* mapMesh, const idVec3& origin, const idMat3& axis, int entityNum, int primitiveNum, idBrushList brushList );

	//! Adds brushes and patches from a map entity to a brush list, applying origin and rotation transformations
	idBrushList AddBrushesForMapEntity( const idMapEntity* mapEnt, int entityNum, idBrushList brushList );

	//! Adds brushes for the map file, specifically for AAS obstacle entities.
	idBrushList AddBrushesForMapFile( const idMapFile* mapFile, idBrushList brushList );

	//! Checks if the map file contains any entities that are valid for AAS building and returns true if found
	bool		CheckForEntities( const idMapFile* mapFile, idStrList& entityClassNames ) const;

	//! Recursively changes the contents of bounding boxes in a BSP node tree based on a mask.
	void		ChangeMultipleBoundingBoxContents_r( idBrushBSPNode* node, int mask );

private:
	// gravitational subdivision  ----------------------------

	//! Recursively sets portal flags on BSP nodes based on portal normal and gravity direction.
	void SetPortalFlags_r( idBrushBSPNode* node );

	//! Determines whether a portal represents a gap in the context of AAS building.
	bool PortalIsGap( idBrushBSPPortal* portal, int side );

	//! Classifies a leaf BSP node as either floor or gap based on portal analysis and subdivides it if necessary.
	void GravSubdivLeafNode( idBrushBSPNode* node );

	//! Recursively subdivides a BSP node for gravity-based area computation.
	void GravSubdiv_r( idBrushBSPNode* node );

	//! Performs gravitational subdivision on the given BSP tree to optimize navigation mesh generation.
	void GravitationalSubdivision( idBrushBSP& bsp );

private:
	// ledge subdivision ----------------------------

	//! Performs a flood fill operation to subdivide AAS nodes based on ledge information
	void LedgeSubdivFlood_r( idBrushBSPNode* node, const idLedge* ledge );

	//! Recursively processes leaf nodes of a BSP tree to handle ledge subdivisions.
	void LedgeSubdivLeafNodes_r( idBrushBSPNode* node, const idLedge* ledge );

	//! Creates bevels and expands ledges, then subdivides areas with the ledge.
	void LedgeSubdiv( idBrushBSPNode* root );

	/*!
		\brief Checks if a given winding represents a ledge side by traversing the BSP tree and evaluating the winding against the plane and normal.

		This function recursively traverses a BSP tree starting from a given node to determine if a winding represents a ledge side. It evaluates the winding against a plane and normal, considering
	   the origin and radius to assess whether the winding intersects with the ledge. The function returns true if the winding is determined to be a ledge side, and false otherwise. The traversal
	   considers the winding splitting and the contents of the BSP nodes to make decisions.

		\param node The current BSP node being evaluated
		\param w The winding to be checked for ledge side
		\param plane The plane used for comparison
		\param normal The normal vector for the winding
		\param origin The origin point for the evaluation
		\param radius The radius used for distance comparisons
		\return True if the winding represents a ledge side, false otherwise.
	*/
	bool IsLedgeSide_r( idBrushBSPNode* node, idFixedWinding* w, const idPlane& plane, const idVec3& normal, const idVec3& origin, const float radius );

	//! Adds a ledge defined by two points to the AAS build structure, merging it with existing ledges if possible.
	void AddLedge( const idVec3& v1, const idVec3& v2, idBrushBSPNode* node );

	//! Finds ledge boundaries for leaf nodes in the AAS build process.
	void FindLeafNodeLedges( idBrushBSPNode* root, idBrushBSPNode* node );

	//! Recursively finds ledge information for AAS (Area Awareness System) by traversing the BSP node tree.
	void FindLedges_r( idBrushBSPNode* root, idBrushBSPNode* node );

	//! Performs ledge subdivision on the given brush BSP to identify and process ledge boundaries.
	void LedgeSubdivision( idBrushBSP& bsp );

	//! Initializes a new ledge map with the specified file name and extension.
	void WriteLedgeMap( const idStr& fileName, const idStr& ext );

private:
	// merging ----------------------------

	//! Checks if all gaps in the first node lead to the second node.
	bool AllGapsLeadToOtherNode( idBrushBSPNode* nodeWithGaps, idBrushBSPNode* otherNode );

	//! Merges adjacent leaf nodes in the BSP if they are compatible based on contents and flags.
	bool MergeWithAdjacentLeafNodes( idBrushBSP& bsp, idBrushBSPNode* node );

	//! Recursively merges leaf nodes in a brush BSP structure.
	void MergeLeafNodes_r( idBrushBSP& bsp, idBrushBSPNode* node );

	//! Merges leaf nodes in the AAS build process for the given brush BSP.
	void MergeLeafNodes( idBrushBSP& bsp );

private:
	// storing file ----------------------------

	//! Initializes the hash tables used for AAS vertex and edge storage.
	void SetupHash();

	//! Cleans up and deallocates the vertex and edge hash tables used during AAS building.
	void ShutdownHash();

	//! Clears the hash tables and initializes vertex bounds and shift values based on the provided bounds.
	void ClearHash( const idBounds& bounds );

	//! Computes a hash value for a 3D vector using grid-based indexing.
	int	 HashVec( const idVec3& vec );

	//! Returns the index of a vertex in the AAS file, adding it if it doesn't exist
	bool GetVertex( const idVec3& v, int* vertexNum );

	/*!
		\brief Retrieves or creates an edge between two vertices in the AAS build data structure.

		This function finds an existing edge between two vertices or creates a new one if it doesn't exist. The edge is identified by the vertex numbers v1num and v2num. If v1num is -1, the function
	   first looks up the vertex number for v1. The edge number is returned through the edgeNum parameter. A negative edge number indicates the edge is reversed compared to the vertex order. The
	   function returns true if the edge already existed in the hash table, or false if a new edge was added.

		\param v1 The first vertex position.
		\param v2 The second vertex position.
		\param edgeNum Pointer to store the resulting edge number; negative if reversed.
		\param v1num The vertex number for the first vertex, or -1 to look it up.
		\return True if the edge was already present in the hash table, false if a new edge was added.
	*/
	bool GetEdge( const idVec3& v1, const idVec3& v2, int* edgeNum, int v1num );

	//! Retrieves or generates a face number for a given portal based on its side
	bool GetFaceForPortal( idBrushBSPPortal* portal, int side, int* faceNum );

	//! Determines the area number for a given leaf node in the BSP structure
	bool GetAreaForLeafNode( idBrushBSPNode* node, int* areaNum );

	//! Recursively stores the AAS tree nodes from a brush BSP node structure
	int	 StoreTree_r( idBrushBSPNode* node );

	//! Recursively estimates the size of AAS data structures by traversing the BSP tree and counting areas and nodes.
	void GetSizeEstimate_r( idBrushBSPNode* parent, idBrushBSPNode* node, struct sizeEstimate_s& size );

	//! Initializes AAS file memory allocations based on estimated sizes from the brush BSP.
	void SetSizeEstimate( const idBrushBSP& bsp, idAASFileLocal* file );

	//! Stores the AAS data for the given brush BSP into a local AAS file
	bool StoreFile( const idBrushBSP& bsp );
};

#endif /* !__AASBUILD_LOCAL_H__ */
