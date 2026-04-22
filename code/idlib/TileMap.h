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
	//! Default constructs a Tile object with zero size.
	Tile() :
		size( 0.0f )
	{
	}

	idVec2 position;
	float  size;
};

// TileNode of a quad-tree that efficiently packs all tiles in a limited area.
struct TileNode {
	//! Initializes a TileNode object with default values.
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
	\brief A quad-tree data structure for managing tiles in a texture atlas.

	The TileMap implements a quad-tree structure to organize tiles within a power-of-two sized texture atlas. It is designed for efficient tile retrieval and management during runtime, particularly
   for light-related operations. The tree structure is built during initialization and maintained in a cache-friendly linear list format to enable fast clearing operations. Tiles are retrieved based
   on requested sizes, with size clamping and power-of-two calculations to prevent visual artifacts like shadow popping. The implementation keeps the quad-tree on the software side for better
   performance compared to GPU-based solutions. The tree is constructed recursively using indices into the underlying list instead of pointer indirections.

*/
class TileMap
{
public:
	//! Initializes a new TileMap object with default values.
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

	//! Releases any resources held by the TileMap instance
	void Release();

	//! Initializes the tile map with specified size, maximum tile size, and number of levels.
	bool Init( unsigned int mapSize, unsigned int maxAbsTileSize, unsigned int numLevels );

	//! Clears the minimum level data for all tile nodes in the tile map.
	void Clear();

	//! Retrieves a tile from the tile map based on the specified size
	bool GetTile( float size, Tile& tile );

private:
	//! Recursively builds a tile map tree structure starting from a parent node.
	void			 BuildTree( TileNode& parentNode, unsigned int level );

	//! Recursively searches for a tile node at the specified level starting from the parent node.
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
