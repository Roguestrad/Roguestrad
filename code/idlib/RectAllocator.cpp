/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Robert Beckebans

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
#include "precompiled.h"
#pragma hdrstop

#include "TileMap.h"
#include "../libs/binpack2d/binpack2d.h"

float RectPackingFraction( const idList<idVec2i>& inputSizes, const idVec2i totalSize )
{
	int totalArea = totalSize.Area();
	if( totalArea == 0 ) {
		return 0;
	}
	int inputArea = 0;
	for( int i = 0; i < inputSizes.Num(); i++ ) {
		inputArea += inputSizes[i].Area();
	}
	return ( float )inputArea / totalArea;
}

class idSortrects : public idSort_Quick<int, idSortrects>
{
public:
	int SizeMetric( idVec2i v ) const
	{
		// skinny rects will sort earlier than square ones, because
		// they are more likely to grow the entire region
		return v.x * v.x + v.y * v.y;
	}
	int Compare( const int& a, const int& b ) const
	{
		return SizeMetric( ( *inputSizes )[b] ) - SizeMetric( ( *inputSizes )[a] );
	}
	const idList<idVec2i>* inputSizes;
};

/*!
	\brief Allocates rectangular regions within a larger rectangle using a greedy corner-packing algorithm.

	This function takes a list of input rectangle sizes and determines optimal positions for each rectangle within a larger container. It uses a greedy approach that attempts to place each rectangle
   at a corner of previously placed rectangles, minimizing the total area required. The rectangles are sorted by size beforehand to improve packing efficiency. The algorithm ensures that the resulting
   total size aligns with GPU texture requirements by rounding widths to multiples of 32 DXT blocks. The function will fail if a rectangle cannot be placed within the prescribed limits.

	\param inputSizes List of input rectangle sizes to be allocated
	\param outputPositions Output list of positions where each input rectangle is placed
	\param totalSize Total size of the resulting rectangle container
	\param START_MAX Initial maximum size used for allocation checks
	\param imageMax Maximum allowed size for any dimension, or 0 to disable limit
	\throws FatalError if a rectangle cannot be fitted within the limits
*/
void RectAllocator( const idList<idVec2i>& inputSizes, idList<idVec2i>& outputPositions, idVec2i& totalSize, const int START_MAX, const int imageMax )
{
	outputPositions.SetNum( inputSizes.Num() );
	if( inputSizes.Num() == 0 ) {
		totalSize.Set( 0, 0 );
		return;
	}
	idList<int> sizeRemap;
	sizeRemap.SetNum( inputSizes.Num() );
	for( int i = 0; i < inputSizes.Num(); i++ ) {
		sizeRemap[i] = i;
	}

	// Sort the rects from largest to smallest (it makes allocating them in the image better)
	idSortrects sortrectsBySize;
	sortrectsBySize.inputSizes = &inputSizes;
	sizeRemap.SortWithTemplate( sortrectsBySize );

	// the largest rect goes to the top-left corner
	outputPositions[sizeRemap[0]].Set( 0, 0 );

	totalSize = inputSizes[sizeRemap[0]];

	// For each image try to fit it at a corner of one of the already fitted images while
	// minimizing the total area.
	// Somewhat better allocation could be had by checking all the combinations of x and y edges
	// in the allocated rectangles, rather than just the corners of each rectangle, but it
	// still does a pretty good job.
	// static const int START_MAX = 1 << 14;
	for( int i = 1; i < inputSizes.Num(); i++ ) {
		idVec2i best( 0, 0 );
		idVec2i bestMax( START_MAX, START_MAX );
		idVec2i size = inputSizes[sizeRemap[i]];
		for( int j = 0; j < i; j++ ) {
			for( int k = 1; k < 4; k++ ) {
				idVec2i test;
				for( int n = 0; n < 2; n++ ) {
					test[n] = outputPositions[sizeRemap[j]][n] + ( ( k >> n ) & 1 ) * inputSizes[sizeRemap[j]][n];
				}

				idVec2i newMax;
				for( int n = 0; n < 2; n++ ) {
					newMax[n] = Max( totalSize[n], test[n] + size[n] );
				}
				// widths must be multiples of 128 pixels / 32 DXT blocks to
				// allow it to be used directly as a GPU texture without re-packing
				// FIXME: make this a parameter
				newMax[0] = ( newMax[0] + 31 ) & ~31;

				// don't let an image get larger than 1024 DXT block, or PS3 crashes
				// FIXME: pass maxSize in as a parameter
				// if( newMax[0] > 1024 || newMax[1] > 1024 )

				if( imageMax > 0 && ( newMax[0] > imageMax || newMax[1] > imageMax ) ) {
					continue;
				}

				// if we have already found a spot that keeps the image smaller, don't bother checking here
				// This calculation biases the rect towards more square shapes instead of
				// allowing it to extend in one dimension for a long time.
				int newSize	 = newMax.x * newMax.x + newMax.y * newMax.y;
				int bestSize = bestMax.x * bestMax.x + bestMax.y * bestMax.y;
				if( newSize > bestSize ) {
					continue;
				}

				// if the image isn't required to grow, favor the location closest to the origin
				if( newSize == bestSize && best.x + best.y < test.x + test.y ) {
					continue;
				}

				// see if this spot overlaps any already allocated rect
				int n = 0;
				for( ; n < i; n++ ) {
					const idVec2i& check	 = outputPositions[sizeRemap[n]];
					const idVec2i& checkSize = inputSizes[sizeRemap[n]];
					if( test.x + size.x > check.x && test.y + size.y > check.y && test.x < check.x + checkSize.x && test.y < check.y + checkSize.y ) {
						break;
					}
				}
				if( n < i ) {
					// overlapped, can't use
					continue;
				}
				best	= test;
				bestMax = newMax;
			}
		}
		if( bestMax[0] == START_MAX ) // FIXME: return an error code
		{
			idLib::FatalError( "RectAllocator: couldn't fit everything" );
		}
		outputPositions[sizeRemap[i]] = best;
		totalSize					  = bestMax;
	}
}

/*!
	\brief Allocates rectangular tiles for tiled shadow mapping using a quad-tree data structure.

	This function takes a list of input rectangle sizes and assigns them positions within a tiled shadow map layout. It uses a quad-tree data structure to efficiently manage tile allocation, sorting
   rectangles by size from largest to smallest to improve packing efficiency. The function returns the total size required for the tile map and the position of each rectangle within that map. The
   output positions are specified in UV coordinates normalized to the range [0,1], which are then scaled to the actual resolution of the tiled shadow map.

	\param inputSizes List of rectangle sizes to be allocated
	\param outputPositions Output list of positions for each rectangle
	\param totalSize Total size of the allocated tile map
	\param TILED_SM_RES Resolution of the tiled shadow map
	\param MAX_TILE_RES Maximum tile size allowed
	\param NUM_QUAD_TREE_LEVELS Number of levels in the quad-tree structure
*/
void RectAllocatorQuadTree(
	const idList<idVec2i>& inputSizes, idList<idVec2i>& outputPositions, idVec2i& totalSize, const int TILED_SM_RES, const int MAX_TILE_RES = 512, const int NUM_QUAD_TREE_LEVELS = 8 )
{
	outputPositions.SetNum( inputSizes.Num() );
	if( inputSizes.Num() == 0 ) {
		totalSize.Set( 0, 0 );
		return;
	}

	idList<int> sizeRemap;
	sizeRemap.SetNum( inputSizes.Num() );
	for( int i = 0; i < inputSizes.Num(); i++ ) {
		sizeRemap[i] = i;
	}

	// Sort the rects from largest to smallest (it makes allocating them in the image better)
	idSortrects sortrectsBySize;
	sortrectsBySize.inputSizes = &inputSizes;
	sizeRemap.SortWithTemplate( sortrectsBySize );

	// quad-tree for managing tiles within tiled shadow map
	TileMap tileMap;

	totalSize.x = 0;
	totalSize.y = 0;

	// initialize tile-map that will manage tiles within tiled shadow map
	if( !tileMap.Init( TILED_SM_RES, MAX_TILE_RES, NUM_QUAD_TREE_LEVELS ) ) {
		return;
	}

	for( int i = 0; i < inputSizes.Num(); i++ ) {
		idVec2i size = inputSizes[sizeRemap[i]];

		int		area = Max( size.x, size.y );

		Tile	tile;
		bool	result = tileMap.GetTile( area, tile );

		if( !result ) {
			outputPositions[sizeRemap[i]].Set( -1, -1 );
		} else {
			// convert from [-1..-1] -> [0..1] and flip y
			idVec2 uvPos;
			uvPos.x = tile.position.x * 0.5f + 0.5f;
			uvPos.y = 1.0f - ( tile.position.y * 0.5f + 0.5f );

			outputPositions[sizeRemap[i]].x = uvPos.x * TILED_SM_RES;
			outputPositions[sizeRemap[i]].y = uvPos.y * TILED_SM_RES;

			if( ( tile.position.x + tile.size ) > totalSize.x ) {
				totalSize.x = tile.position.x + tile.size;
			}

			if( ( tile.position.y + tile.size ) > totalSize.y ) {
				totalSize.y = tile.position.y + tile.size;
			}
		}
	}
}

/*!
	\class MyContent
	\brief A class for managing content with string-based data.
*/
class MyContent
{
public:
	int	  itemIndex;
	idStr str;

	//! Initializes a MyContent object with a default string value.
	MyContent() :
		str( "default string" )
	{
	}

	//! Initializes a MyContent object with the provided string.
	MyContent( const idStr& str ) :
		str( str )
	{
	}
};

/*!
	\brief Packs 2D rectangles into a bins using a bin packing algorithm while tracking their positions and total required size.

	This function takes a list of rectangle sizes and names, and attempts to pack them into a set of bins using the BinPack2D algorithm. The positions of each rectangle within the packed bins are
   returned, along with the total size required to accommodate all rectangles. The packing process sorts the rectangles by size for better results. Rectangles that cannot be packed are skipped, and
   the function tracks the maximum dimensions needed to fit all packed rectangles. The START_MAX parameter controls the initial bin size used for packing.

	\param inputSizes List of rectangle sizes (width and height) to be packed
	\param inputNames List of names associated with each rectangle
	\param outputPositions Output list of positions where each rectangle was placed
	\param totalSize Total size required to fit all packed rectangles
	\param START_MAX Initial maximum size for the bins used in packing
*/
void RectAllocatorBinPack2D( const idList<idVec2i>& inputSizes, const idStrList& inputNames, idList<idVec2i>& outputPositions, idVec2i& totalSize, const int START_MAX )
{
	outputPositions.SetNum( inputSizes.Num() );

	if( inputSizes.Num() == 0 ) {
		totalSize.Set( 0, 0 );
		return;
	}

	// Create some 'content' to work on.
	BinPack2D::ContentAccumulator<MyContent> inputContent;

	for( int i = 0; i < inputSizes.Num(); i++ ) {
		// random size for this content
		int		  width	 = inputSizes[i].x;
		int		  height = inputSizes[i].y;

		// whatever data you want to associate with this content
		MyContent mycontent( inputNames[i] );
		mycontent.itemIndex = i;

		// Add it
		inputContent += BinPack2D::Content<MyContent>( mycontent, BinPack2D::Coord(), BinPack2D::Size( width, height ), false );
	}

	// Sort the input content by size... usually packs better.
	inputContent.Sort();

	// Create some bins! ( 2 bins, 128x128 in this example )
	BinPack2D::CanvasArray<MyContent>		 canvasArray = BinPack2D::UniformCanvasArrayBuilder<MyContent>( START_MAX, START_MAX, 2 ).Build();

	// A place to store content that didnt fit into the canvas array.
	BinPack2D::ContentAccumulator<MyContent> remainder;

	// try to pack content into the bins.
	bool									 success = canvasArray.Place( inputContent, remainder );

	// A place to store packed content.
	BinPack2D::ContentAccumulator<MyContent> outputContent;

	// Read all placed content.
	canvasArray.CollectContent( outputContent );

	// parse output.
	typedef BinPack2D::Content<MyContent>::Vector::iterator binpack2d_iterator;
	// idLib::Printf( "PLACED:\n" );

	totalSize.x = 0;
	totalSize.y = 0;

	int i = 0;
	for( binpack2d_iterator itor = outputContent.Get().begin(); itor != outputContent.Get().end(); itor++, i++ ) {
		const BinPack2D::Content<MyContent>& content = *itor;

		// retreive your data.
		const MyContent&					 myContent = content.content;

		int									 index = myContent.itemIndex;
		outputPositions[index].x				   = content.coord.x;
		outputPositions[index].y				   = content.coord.y;

		if( ( content.coord.x + content.size.w ) > totalSize.x ) {
			totalSize.x = content.coord.x + content.size.w;
		}

		if( ( content.coord.y + content.size.h ) > totalSize.y ) {
			totalSize.y = content.coord.y + content.size.h;
		}

#if 0
		idLib::Printf( "\t%9s of size %3dx%3d at position %3d,%3d,%2d rotated=%s\n",
					   myContent.str.c_str(),
					   content.size.w,
					   content.size.h,
					   content.coord.x,
					   content.coord.y,
					   content.coord.z,
					   ( content.rotated ? "yes" : " no" ) );
#endif
	}

#if 0
	for( binpack2d_iterator itor = remainder.Get().begin(); itor != remainder.Get().end(); itor++ )
	{
		const BinPack2D::Content<MyContent>& content = *itor;

		const MyContent& myContent = content.content;

		int index = myContent.itemIndex;
		outputPositions[ index ].x = -1;
		outputPositions[ index ].y = -1;

		idLib::Printf( "\tFailed to place %9s of size %3dx%3d\n",
					   myContent.str.c_str(),
					   content.size.w,
					   content.size.h );
	}
#endif
}
