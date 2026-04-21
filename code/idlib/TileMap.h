/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2014 Hawar Doghramachi
Copyright (C) 2022 Robert Beckebans (id Tech 4x integration)

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

#ifndef TILE_MAP_H
#define TILE_MAP_H

// Tile specifies the position and size within a texture atlas.
struct Tile {
	/*!
		\brief Initializes a new Tile object with a size of zero.

		This is the default constructor for the Tile class. It initializes the size member variable to zero, which represents the dimensions of the tile in the game world.

	*/
	Tile() :
		size( 0.0f )
	{
	}

	idVec2 position;
	float  size;
};

// TileNode of a quad-tree that efficiently packs all tiles in a limited area.
struct TileNode {
	/*!
		\brief Constructs a new TileNode instance with default values.

		Initializes a TileNode object with level and minLevel set to zero. The childIndices array is initialized to -1 for all four elements, indicating no child nodes are present. This constructor is
	   typically used to create tile nodes for spatial partitioning structures.

	*/
	TileNode() :
		level( 0 ),
		minLevel( 0 )
	{
		for( unsigned int i = 0; i < 4; i++ ) {
			childIndices[i] = -1;
		}
	}

	idVec2		 position;
	int			 childIndices[4];
	unsigned int level;
	unsigned int minLevel;
};

/*!
	\class TileMap
	\brief A hierarchical tile map system for resource management in the engine.

	Quad-tree that manages tiles in a power of two/ squared texture atlas. At initialization the quad-tree is build so that
	all nodes already have the information of the position for the corresponding tile. All nodes are kept in a cache-friendly
	manner in one linear list, which makes clearing the quad-tree very fast. Therefore instead of pointer indirections, indices
	into the underlying list are used.
	At runtime each relevant light will request per frame a tile with a size that corresponds to the screen-space light-area of
	the light. Thereby the size is clamped between a min/ max resolution. To determine the level of the requested tile first the
	next power of two size is determined which is larger than the requested size. However, instead of using the power of two size
	of the determined tile, the actual incoming dynamically changing size is used. In this way unpleasant popping of shadows can
	be avoided, which would occur otherwise when discrete power of two steps would be used.
	Since this operation is working with a O(n) complexity, the quad-tree is held on software-side, which is faster than keeping
	the quad-tree on the GPU.

*/
class TileMap
{
public:
	/*!
		\brief Initializes a new instance of the TileMap class with default values.

		The constructor initializes all member variables to their default states. The mapSize, minAbsTileSize, and maxAbsTileSize are set to zero float values. The log2MapSize, numLevels, numNodes,
	   and nodeIndex are initialized to zero integers. The nodeIndex is set to point to NULL, indicating no node has been found yet.

	*/
	TileMap() :
		mapSize( 0.0f ),
		log2MapSize( 0 ),
		minAbsTileSize( 0.0f ),
		maxAbsTileSize( 0.0f ),
		numLevels( 0 ),
		numNodes( 0 ),
		nodeIndex( 0 ),
		foundNode( NULL )
	{
	}

	~TileMap() { Release(); }

	/*!
		\brief Releases any resources held by the TileMap instance.

		This function is designed to release any resources or references held by the TileMap instance. Based on the implementation, it appears to be a placeholder or stub function as the body is
	   currently empty. The function name and signature suggest it follows a common pattern used in COM-like interfaces where objects must release their resources. The function may be intended to
	   clean up internal data structures or perform other cleanup tasks in the future.

	*/
	void Release();

	/*!
		\brief Initializes a tile map with specified size, maximum tile size, and number of levels.

		This function sets up the internal data structures for a tile map used in resource management. It validates the input parameters to ensure they are within acceptable bounds and initializes the
	   tile node list. The function also builds the hierarchical tile tree structure based on the specified parameters. The map size must be a power of two, and the maximum tile size must be less than
	   or equal to the map size. The minimum tile size is calculated based on the number of levels.

		\param mapSize The size of the map, which must be a power of two
		\param maxAbsTileSize The maximum absolute tile size allowed
		\param numLevels The number of levels in the tile hierarchy, must be at least 1
		\return True if initialization succeeds, false otherwise. Initialization fails if parameters are invalid or if the calculated minimum tile size is outside acceptable bounds.
	*/
	bool Init( unsigned int mapSize, unsigned int maxAbsTileSize, unsigned int numLevels );

	//! Clears the minimum level data for all tile nodes in the tile map.
	void Clear();

	/*!
		\brief Retrieves a tile of the specified size from the tile map, returning true if successful.

		This function attempts to find a suitable tile in the tile map based on the requested size. It clamps the size to a valid range and calculates the required level for the tile based on the map
	   size. The function searches for a node in the tile node list that matches the required level. If a suitable node is found, the tile's position and size are set accordingly. The size is
	   normalized relative to the map size. The function returns false if no suitable tile can be found.

		\param size The requested size of the tile to retrieve
		\param tile The tile structure to be filled with the position and size of the retrieved tile
		\return True if a tile of the specified size was successfully found and retrieved, false otherwise.
	*/
	bool GetTile( float size, Tile& tile );

private:
	/*!
		\brief Recursively builds a tree structure for tile map nodes starting from a parent node and specified level

		This function constructs a hierarchical tree of tile map nodes by recursively subdividing the space. It takes a parent node and a current level, increments the level, and creates four child
	   nodes for each parent. The function calculates positions for child nodes based on the current level, ensuring that the node indices remain within valid bounds. The recursion stops when the
	   maximum number of levels is reached

		\param parentNode The parent node from which to build child nodes
		\param level The current level in the tree hierarchy
		\throws assertion failure if node index exceeds the maximum number of nodes
	*/
	void			 BuildTree( TileNode& parentNode, unsigned int level );

	/*!
		\brief Recursively searches for a tile node at the specified level starting from the parent node.

		This function traverses the tile map structure to locate a node at the given level. It starts from the provided parent node and explores its children recursively. The search stops when a node
	   at the target level is found or when all possible paths have been exhausted. The function updates the parent node's minimum level and marks the found node for subsequent use.

		\param parentNode The parent tile node to start the search from
		\param level The target level to find the node at
	*/
	void			 FindNode( TileNode& parentNode, unsigned int level );

	float			 mapSize;
	unsigned int	 log2MapSize;
	float			 minAbsTileSize;
	float			 maxAbsTileSize;
	unsigned int	 numLevels;
	idList<TileNode> tileNodeList;
	unsigned int	 numNodes;
	unsigned int	 nodeIndex;
	TileNode*		 foundNode;
};

#endif
