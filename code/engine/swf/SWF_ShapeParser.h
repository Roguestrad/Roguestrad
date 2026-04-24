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
#ifndef __SWF_SHAPEPARSER_H__
#define __SWF_SHAPEPARSER_H__

/*!
	\class idSWFShapeParser
	\brief Parser for SWF shape data that handles triangulation of geometric primitives.

	This class provides functionality for parsing SWF shape data from bitstreams and converting it into triangulated geometric representations. It supports parsing regular shapes, morph shapes, and
   font glyphs, handling both straight and curved edges through Bézier curve computation. The parser maintains state for pen position during edge parsing and manages vertex data in a shared buffer. It
   includes methods for converting edge soup into vertex loops and performing triangulation using an ear clipping algorithm for both regular shapes and font glyphs. The class is designed to process
   multiple bitstreams for morph animations and handles fill style definitions for different shape regions. The SWF coordinate system is converted to internal coordinates during parsing.

*/
class idSWFShapeParser
{
public:
	//! Constructs a new instance of the SWF shape parser.
	idSWFShapeParser() { }

	//! Parses SWF shape data from a bitstream and constructs a shape object with triangulated geometry.
	void Parse( idSWFBitStream& bitstream, idSWFShape& shape, int recordType );

	//! Parses morph shape data from a bitstream and initializes the corresponding shape object.
	void ParseMorph( idSWFBitStream& bitstream, idSWFShape& shape );

	//! Parses font shape data from a bitstream and triangulates it into a font glyph structure.
	void ParseFont( idSWFBitStream& bitstream, idSWFFontGlyph& shape );

private:
	bool extendedCount;
	bool rgba;
	bool morph;
	bool lineStyle2;

	struct swfSPEdge_t {
		uint16 v0;
		uint16 v1;
		uint16 cp; // control point if this is a curve, 0xFFFF otherwise
	};
	struct swfSPMorphEdge_t {
		swfSPEdge_t start;
		swfSPEdge_t end;
	};
	struct swfSPLineLoop_t {
		bool					hole;
		idList<uint16, TAG_SWF> vindex1;
		idList<uint16, TAG_SWF> vindex2;
	};
	struct swfSPDrawFill_t {
		swfFillStyle_t					  style;
		idList<swfSPMorphEdge_t, TAG_SWF> edges;
		idList<swfSPLineLoop_t, TAG_SWF>  loops;
	};
	struct swfSPDrawLine_t {
		swfLineStyle_t					  style;
		idList<swfSPMorphEdge_t, TAG_SWF> edges;
	};
	idList<idVec2, TAG_SWF>			 verts;
	idList<swfSPDrawFill_t, TAG_SWF> fillDraws;
	idList<swfSPDrawLine_t, TAG_SWF> lineDraws;

private:
	//! Parses shape data from two SWF bitstreams, handling morph streams and edge definitions.
	void ParseShapes( idSWFBitStream& bitstream1, idSWFBitStream* bitstream2, bool swap );

	//! Reads fill style data from a bitstream and populates style information for shapes.
	void ReadFillStyle( idSWFBitStream& bitstream );

	/*!
		\brief Parses a shape edge from a SWF bitstream, updating pen position and edge data

		This function processes a single edge definition in a SWF shape, handling both straight and curved edges. For straight edges, it reads direction and distance values to update the pen position
	   and defines the edge with a start and end point. For curved edges, it reads control point and end position data to define a cubic Bézier curve. The function maintains pen position state across
	   multiple calls and stores vertex data in a shared vertex buffer. The SWF coordinate system is converted to internal coordinates using the SWFTWIP macro.

		\param bitstream Input bitstream containing the edge data encoded in SWF format
		\param penX Reference to current pen X coordinate, updated with final edge position
		\param penY Reference to current pen Y coordinate, updated with final edge position
		\param edge Output structure containing the parsed edge data including vertex indices and control point
	*/
	void ParseEdge( idSWFBitStream& bitstream, int32& penX, int32& penY, swfSPEdge_t& edge );

	//! Converts edge soup into connected vertex loops for each fill style
	void MakeLoops();

	//! Converts shape fill loops into triangle draws using ear clipping algorithm
	void TriangulateSoup( idSWFShape& shape );

	//! Triangulates a font glyph shape into triangles using ear clipping algorithm
	void TriangulateSoup( idSWFFontGlyph& shape );

	//! Finds a valid ear vertex in a polygonal loop for triangulation.
	int	 FindEarVert( const swfSPLineLoop_t& loop );

	//! Adds a unique vertex to the shape draw fill, using either morph or non-morph logic based on the morph flag.
	void AddUniqueVert( idSWFShapeDrawFill& drawFill, const idVec2& start, const idVec2& end );
};

#endif // !__SWF_SHAPEPARSER_H__
