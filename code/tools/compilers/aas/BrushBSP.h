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

#ifndef __BRUSHBSP_H__
#define __BRUSHBSP_H__

/*
===============================================================================

	BrushBSP

===============================================================================
*/

class idBrushBSP;
class idBrushBSPNode;
class idBrushBSPPortal;

/*!
	\class idBrushBSPPortal
	\brief Represents a portal in a brush-based BSP tree used for visibility determination and spatial partitioning.

	The idBrushBSPPortal class encapsulates the geometric and topological data of a portal within a BSP (Binary Space Partitioning) tree structure. It maintains a winding that defines the portal's
   shape and a plane that represents its orientation in 3D space. The portal connects two BSP nodes and is used during rendering to determine visibility between different regions of the scene. The
   class supports operations such as splitting the portal by a plane, flipping its orientation, and managing its association with BSP nodes. It also provides methods to access and modify portal
   properties including face numbers, flags, and connections to adjacent portals and nodes. Memory management is handled through the destructor which releases the winding memory.

*/
class idBrushBSPPortal
{
	friend class idBrushBSP;
	friend class idBrushBSPNode;

public:
	//! Initializes a new instance of the idBrushBSPPortal class with default values.
	idBrushBSPPortal();

	//! Destructor for idBrushBSPPortal that releases the allocated winding memory.
	~idBrushBSPPortal();

	//! Adds this portal to the specified front and back BSP nodes.
	void			  AddToNodes( idBrushBSPNode* front, idBrushBSPNode* back );

	//! Removes the portal from the specified BSP node.
	void			  RemoveFromNode( idBrushBSPNode* l );

	//! Reverses the orientation of the BSP portal by flipping its plane and reversing its winding.
	void			  Flip();

	//! Splits the portal using the given plane and returns the side relationship to the plane.
	int				  Split( const idPlane& splitPlane, idBrushBSPPortal** front, idBrushBSPPortal** back );

	//! Returns the winding associated with this portal.
	idWinding*		  GetWinding() const { return winding; }

	//! Returns the plane data of this BSP portal.
	const idPlane&	  GetPlane() const { return plane; }

	//! Sets the face number for the brush BSP portal.
	void			  SetFaceNum( int num ) { faceNum = num; }

	//! Returns the face number associated with this BSP portal
	int				  GetFaceNum() const { return faceNum; }

	//! Returns the flags associated with this brush BSP portal
	int				  GetFlags() const { return flags; }

	//! Sets the specified flag in the portal's flags member.
	void			  SetFlag( int flag ) { flags |= flag; }

	//! Removes a specified flag from the portal's flags.
	void			  RemoveFlag( int flag ) { flags &= ~flag; }

	//! Returns the next portal in the specified side direction.
	idBrushBSPPortal* Next( int side ) const { return next[side]; }

	//! Returns the BSP node for the specified side of the portal.
	idBrushBSPNode*	  GetNode( int side ) const { return nodes[side]; }

private:
	idPlane			  plane;	// portal plane
	int				  planeNum; // number of plane this portal is on
	idWinding*		  winding;	// portal winding
	idBrushBSPNode*	  nodes[2]; // nodes this portal seperates
	idBrushBSPPortal* next[2];	// next portal in list for both nodes
	int				  flags;	// portal flags
	int				  faceNum;	// number of the face created for this portal
};

//===============================================================
//
//	idBrushBSPNode
//
//===============================================================

#define NODE_VISITED BIT( 30 )
#define NODE_DONE	 BIT( 31 )

/*!
	\class idBrushBSPNode
	\brief Represents a node in a brush-based BSP tree used for spatial partitioning and collision detection.

	The idBrushBSPNode class encapsulates the structure and behavior of nodes within a brush-based binary space partitioning tree. Each node maintains information about its spatial properties,
   including bounding volume, plane splitting the space, contents, area number, and associated portals. The class provides methods for managing the node's relationships with parent and child nodes,
   setting and retrieving various attributes such as contents and flags, and performing operations like splitting the node or testing if it represents a valid leaf node. The class supports flag
   management for various node states and includes functionality for flood-based flag propagation through the BSP tree structure, which is essential for operations like area connectivity tracking or
   visibility determination.

*/
class idBrushBSPNode
{
	friend class idBrushBSP;
	friend class idBrushBSPPortal;

public:
	//! Initializes a new instance of the idBrushBSPNode class.
	idBrushBSPNode();

	//! Destroys a brush BSP node and cleans up its associated brushes, volume brush, and portals.
	~idBrushBSPNode();

	//! Sets the contents of the BSP node based on the contents of all brushes in its brush list.
	void			  SetContentsFromBrushes();

	//! Returns the bounding box of all portals connected to this BSP node.
	idBounds		  GetPortalBounds();

	//! Returns the child node at the specified index
	idBrushBSPNode*	  GetChild( int index ) const { return children[index]; }

	//! Returns the parent node of this BSP node.
	idBrushBSPNode*	  GetParent() const { return parent; }

	//! Sets the contents of the brush BSP node to the specified integer value.
	void			  SetContents( int contents ) { this->contents = contents; }

	//! Returns the contents of the brush BSP node.
	int				  GetContents() const { return contents; }

	//! Returns the plane associated with this BSP node.
	const idPlane&	  GetPlane() const { return plane; }

	//! Returns the first portal in the linked list of portals for this BSP node.
	idBrushBSPPortal* GetPortals() const { return portals; }

	//! Sets the area number for this BSP node.
	void			  SetAreaNum( int num ) { areaNum = num; }

	//! Returns the area number associated with this BSP node.
	int				  GetAreaNum() const { return areaNum; }

	//! Returns the flags associated with this brush BSP node.
	int				  GetFlags() const { return flags; }

	//! Sets the specified flag on the brush BSP node.
	void			  SetFlag( int flag ) { flags |= flag; }

	//! Removes a specified flag from the node's flags.
	void			  RemoveFlag( int flag ) { flags &= ~flag; }

	//! Tests if the leaf node is valid by checking the distance from the center to the planes of the portals.
	bool			  TestLeafNode();

	//! Removes a flag from nodes by flooding through portals to nodes with the flag set.
	void			  RemoveFlagFlood( int flag );

	//! Removes a flag from the current node and recursively from all child nodes in the BSP tree.
	void			  RemoveFlagRecurse( int flag );

	//! Removes a flag from the current node and recursively floods the flag to child nodes if the current node is a leaf.
	void			  RemoveFlagRecurseFlood( int flag );

	//! Returns the side of the plane the node is on
	int				  PlaneSide( const idPlane& plane, float epsilon = ON_EPSILON ) const;

	//! Splits the BSP node using the provided plane and plane number.
	bool			  Split( const idPlane& splitPlane, int splitPlaneNum );

private:
	idPlane			  plane;	   // split plane if this is not a leaf node
	idBrush*		  volume;	   // node volume
	int				  contents;	   // node contents
	idBrushList		  brushList;   // list with brushes for this node
	idBrushBSPNode*	  parent;	   // parent of this node
	idBrushBSPNode*	  children[2]; // both are NULL if this is a leaf node
	idBrushBSPPortal* portals;	   // portals of this node
	int				  flags;	   // node flags
	int				  areaNum;	   // number of the area created for this node
	int				  occupied;	   // true when portal is occupied
};

/*!
	\class idBrushBSP
	\brief A class for constructing and manipulating binary space partitioning trees from brush data.

	The idBrushBSP class is designed to build and manage binary space partitioning trees for brush-based geometry, primarily used in collision detection and rendering. It provides functionality to
   construct trees from lists of brushes, apply constraints for splitting and merging, and perform operations such as pruning, portalization, and leak detection. The implementation uses a grid-based
   approach for efficient processing and includes methods for managing tree structure, portal creation, and flood filling operations to determine connectivity. Memory management is handled through
   recursive free operations and direct node manipulation. The class supports various filtering and optimization techniques including merge and melt operations, and provides utilities for checking
   splitter validity and computing statistics for brush splitting operations.

*/
class idBrushBSP
{
public:
	//! Constructs an idBrushBSP object and initializes its member variables.
	idBrushBSP();

	//! Destructor for idBrushBSP that cleans up the BSP tree and removes references.
	~idBrushBSP();

	/*!
		\brief Builds a BSP tree from a set of brushes using the provided chop and merge constraints.

		This function constructs a binary space partitioning tree from a list of brushes. It initializes the root node with the brush list and calculates the bounding volume for the tree. The function
	   uses a grid-based approach to process the brush list and applies the provided chop and merge constraints to determine how brushes can be split or merged. The tree building process is displayed
	   with progress information showing the number of grid cells and splits.

		\param brushList List of brushes to be used for building the BSP tree
		\param skipContents Contents mask to skip during the BSP building process
		\param ChopAllowed Function pointer to determine if two brushes can be chopped
		\param MergeAllowed Function pointer to determine if two brushes can be merged
	*/
	void			Build( idBrushList brushList, int skipContents, bool ( *ChopAllowed )( idBrush* b1, idBrush* b2 ), bool ( *MergeAllowed )( idBrush* b1, idBrush* b2 ) );

	//! Removes splits in subspaces with the given contents.
	void			PruneTree( int contents );

	//! Portalizes the BSP tree by creating portals for all nodes.
	void			Portalize();

	//! Removes BSP leaf nodes that are outside the map and not reachable by entities
	bool			RemoveOutside( const idMapFile* mapFile, int contents, const idStrList& classNames );

	//! Writes a file containing a trace through a leak in the brush BSP structure.
	void			LeakFile( const idStr& fileName );

	//! Merges portals in the brush BSP structure while skipping portals with the specified contents.
	void			MergePortals( int skipContents );

	//! Attempts to merge two leaf nodes separated by a portal if the resulting node would remain convex.
	bool			TryMergeLeafNodes( idBrushBSPPortal* portal, int side );

	//! Recursively prunes merged nodes from the BSP tree starting from the given node.
	void			PruneMergedTree_r( idBrushBSPNode* node );

	//! Melts portal windings by removing colinear points and inserting new vertices.
	void			MeltPortals( int skipContents );

	//! Writes a map file with a brush for every leaf node that has the given contents
	void			WriteBrushMap( const idStr& fileName, const idStr& ext, int contents );

	//! Returns the bounds of the entire BSP tree.
	const idBounds& GetTreeBounds() const { return treeBounds; }

	//! Returns the root node of the BSP tree.
	idBrushBSPNode* GetRootNode() const { return root; }

private:
	idBrushBSPNode* root;
	idBrushBSPNode* outside;
	idBounds		treeBounds;
	idPlaneSet		portalPlanes;
	int				numGridCells;
	int				numSplits;
	int				numGridCellSplits;
	int				numPrunedSplits;
	int				numPortals;
	int				solidLeafNodes;
	int				outsideLeafNodes;
	int				insideLeafNodes;
	int				numMergedPortals;
	int				numInsertedPoints;
	idVec3			leakOrigin;
	int				brushMapContents;
	idBrushMap*		brushMap;

	bool ( *BrushChopAllowed )( idBrush* b1, idBrush* b2 );
	bool ( *BrushMergeAllowed )( idBrush* b1, idBrush* b2 );

private:
	//! Recursively removes invalid leaf node references from a BSP tree node.
	void			RemoveMultipleLeafNodeReferences_r( idBrushBSPNode* node );

	//! Frees the memory allocated for a BSP node and its children recursively.
	void			Free_r( idBrushBSPNode* node );
	void			IncreaseNumSplits();

	//! Checks if a brush side can be used as a splitter by verifying it does not have the SFL_SPLIT or SFL_USED_SPLITTER flags set.
	bool			IsValidSplitter( const idBrushSide* side );

	/*!
		\brief Computes statistics for a brush splitter operation by analyzing the brush's relationship with a given plane and tracking split behavior

		This function evaluates how a brush interacts with a specific plane in the context of BSP tree construction. It determines whether the brush is entirely in front of, behind, or spans the
	   plane. The function also tracks whether the brush sides are split by the plane and counts epsilon brushes that barely penetrate the splitting plane. It updates the provided statistics structure
	   with information about splits, front/back placements, and facing sides.

		\param brush Pointer to the brush being analyzed
		\param planeNum Index of the plane in the plane list to test against
		\param planeList Set of planes used for testing
		\param testedPlanes Array tracking which planes have already been tested
		\param stats Structure to store statistics about the brush splitting operation
		\return Integer value indicating the brush's relationship to the plane, which can be FRONT, BACK, FACING, or BOTH, and also includes flags for split and epsilon brush counts
	*/
	int				BrushSplitterStats( const idBrush* brush, int planeNum, const idPlaneSet& planeList, bool* testedPlanes, struct splitterStats_s& stats );

	/*!
		\brief Finds the best splitting plane for a BSP node based on the provided plane list and brush geometry.

		This function iterates through all brushes in the node and their sides to identify valid splitters. It evaluates potential splitting planes by calculating a score based on the number of
	   brushes on each side, the number of splits, and other factors. The best scoring plane is returned along with statistics about the split.

		\param node The BSP node for which to find a splitter
		\param planeList The list of planes available for splitting
		\param testedPlanes An array tracking which planes have been tested
		\param bestStats Statistics about the best splitter found
		\return The index of the best splitting plane, or -1 if no valid splitter is found
	*/
	int				FindSplitter( idBrushBSPNode* node, const idPlaneSet& planeList, bool* testedPlanes, struct splitterStats_s& bestStats );

	//! Marks a splitter plane as used for the given BSP node and updates brush splitter flags accordingly
	void			SetSplitterUsed( idBrushBSPNode* node, int planeNum );

	/*!
		\brief Recursively builds a BSP tree for brush-based collision detection and rendering by splitting nodes based on plane choices.

		This function implements the recursive portion of BSP tree construction for brushes. It determines the best splitting plane for the current node, splits the node's volume and brush list into
	   two child nodes, and recursively processes the children. If no suitable split plane is found, it converts the current node into a leaf node and cleans up memory. The function tracks statistics
	   about splits and handles special cases where both children contain specific skip contents, which triggers a merge operation to simplify the tree structure.

		\param node The current node in the BSP tree being processed
		\param planeList Set of candidate planes to use for splitting the node
		\param testedPlanes Boolean array tracking which planes have been tested for splitting
		\param skipContents Contents mask that determines when to merge child nodes
		\return The root node of the BSP tree built for the current recursive call
	*/
	idBrushBSPNode* BuildBrushBSP_r( idBrushBSPNode* node, const idPlaneSet& planeList, bool* testedPlanes, int skipContents );

	//! Processes a grid cell by chopping brushes and building a BSP tree.
	idBrushBSPNode* ProcessGridCell( idBrushBSPNode* node, int skipContents );

	//! Recursively builds a grid from BSP node information.
	void			BuildGrid_r( idList<idBrushBSPNode*>& gridCells, idBrushBSPNode* node );

	//! Recursively prunes branches of the BSP tree that do not contribute to the specified contents.
	void			PruneTree_r( idBrushBSPNode* node, int contents );

	//! Creates outside portals for the BSP tree to define the boundaries of the world.
	void			MakeOutsidePortals();

	//! Creates a base winding for a given BSP node by clipping the node's plane with all parent nodes.
	idWinding*		BaseWindingForNode( idBrushBSPNode* node );

	//! Creates a portal for the given BSP node by clipping its base winding with parent portals.
	void			MakeNodePortal( idBrushBSPNode* node );

	//! Splits the portals of a BSP node along the node's separating plane
	void			SplitNodePortals( idBrushBSPNode* node );

	//! Recursively creates portals for the BSP tree nodes.
	void			MakeTreePortals_r( idBrushBSPNode* node );

	//! Performs a recursive flood fill operation through BSP portals to mark connected nodes.
	void			FloodThroughPortals_r( idBrushBSPNode* node, int contents, int depth );

	//! Flood fills the BSP tree from a given origin point to determine if a region can be occupied.
	bool			FloodFromOrigin( const idVec3& origin, int contents );

	//! Flood fills the BSP from entities matching specified class names and contents
	bool			FloodFromEntities( const idMapFile* mapFile, int contents, const idStrList& classNames );

	//! Removes outside nodes from the BSP tree by updating their contents based on the specified contents mask.
	void			RemoveOutside_r( idBrushBSPNode* node, int contents );

	//! Sets portal plane numbers for a given BSP node and its children recursively.
	void			SetPortalPlanes_r( idBrushBSPNode* node, idPlaneSet& planeList );

	//! Sets portal planes for the brush BSP.
	void			SetPortalPlanes();

	//! Recursively merges portals in the brush BSP tree while skipping nodes with specified contents.
	void			MergePortals_r( idBrushBSPNode* node, int skipContents );

	//! Merges portal windings for leaf node portals that separate the same leaf nodes or share the same plane with specified contents
	void			MergeLeafNodePortals( idBrushBSPNode* node, int skipContents );
	void			UpdateTreeAfterMerge_r( idBrushBSPNode* node, const idBounds& bounds, idBrushBSPNode* oldNode, idBrushBSPNode* newNode );

	//! Removes colinear points from the winding of portals in the specified leaf node.
	void			RemoveLeafNodeColinearPoints( idBrushBSPNode* node );

	//! Recursively removes colinear points from brush BSP nodes.
	void			RemoveColinearPoints_r( idBrushBSPNode* node, int skipContents );

	/*!
		\brief Flood fills through BSP portals to find vertices within the specified bounds for melting operations.

		This function recursively traverses the BSP tree starting from the given node, visiting all connected nodes through portals that intersect with the specified bounds. It collects all unique
	   vertices within the bounds that could potentially be affected by a melting operation.

		\param node The current BSP node to start flooding from
		\param skipContents Contents flag to skip when traversing nodes
		\param bounds The bounding volume to test against portal windings
		\param vertexList The set to store found vertices that may be inside the bounds
	*/
	void			MeltFlood_r( idBrushBSPNode* node, int skipContents, idBounds& bounds, idVectorSet<idVec3, 3>& vertexList );

	//! Melts portal vertices in leaf nodes together based on vertex proximity and content filtering.
	void			MeltLeafNodePortals( idBrushBSPNode* node, int skipContents, idVectorSet<idVec3, 3>& vertexList );

	//! Recursively melts portals in a BSP node structure while skipping nodes with specific contents.
	void			MeltPortals_r( idBrushBSPNode* node, int skipContents, idVectorSet<idVec3, 3>& vertexList );
};

#endif /* !__BRUSHBSP_H__ */
