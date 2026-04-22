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
#ifndef __DEBUGGRAPH_H__
#define __DEBUGGRAPH_H__

/*!
	\class idDebugGraph
	\brief Provides functionality for rendering debug graphs with configurable bars, labels, and visual properties.

	The idDebugGraph class is designed to support debugging tools by rendering visual graph representations with configurable properties. It maintains a collection of bars that can be individually
   configured with values, labels, and colors. The class supports various rendering options including fill modes, orientation, borders, and background colors. It allows for dynamic updates to bar
   values and labels, as well as the addition of grid lines for reference. The graph can be enabled or disabled, and its rendering is controlled through a provided render system interface. The class
   is intended to be used in debugging scenarios where visual representation of numerical data is beneficial for analysis.

*/
class idDebugGraph
{
public:
	//! Initializes a debug graph with the specified number of items
	idDebugGraph( int numItems = 0 );

	//! Enables or disables the debug graph based on the provided boolean value.
	void  Enable( bool b ) { enable = b; }

	//! Initializes the debug graph with the specified number of bars
	void  Init( int numBars );

	//! Adds a grid line to the debug graph with the specified value and color.
	void  AddGridLine( float value, const idVec4& color );

	//! Sets a bar value in the debug graph, appending a new element if index -1 is passed.
	void  SetValue( int b, float value, const idVec4& color );

	//! Returns the value of a specified bar in the debug graph.
	float GetValue( int b ) { return bars[b].value; }

	//! Sets the label text for a specific bar in the debug graph.
	void  SetLabel( int b, const char* text );

	enum fillMode_t {
		GRAPH_LINE,			// only draw a single top line for each bar
		GRAPH_FILL,			// fill the entire bar from the bottom (or left)
		GRAPH_FILL_REVERSE, // fill the entire bar from the top (or right)
	};

	//! Sets the fill mode for the debug graph.
	void SetFillMode( fillMode_t m ) { mode = m; }

	//! Sets whether the debug graph should be rendered in a sideways orientation.
	void SetSideways( bool s ) { sideways = s; }

	//! Sets the background color for the debug graph.
	void SetBackgroundColor( const idVec4& color ) { bgColor = color; }

	//! Sets the color used for the debug graph labels.
	void SetLabelColor( const idVec4& color ) { fontColor = color; }

	//! Sets the border value for the debug graph, controlling spacing between bars and around the entire graph.
	void SetBorder( float b ) { border = b; }

	/*!
		\brief Sets the screen position and dimensions of the debug graph.

		This function configures the rectangular area on screen where the debug graph will be displayed. The parameters define the top-left corner coordinates (x, y) and the width (w) and height (h)
	   of the graph's bounding box.

		\param x The x-coordinate of the top-left corner of the graph's bounding box
		\param y The y-coordinate of the top-left corner of the graph's bounding box
		\param w The width of the graph's bounding box
		\param h The height of the graph's bounding box
	*/
	void SetPosition( float x, float y, float w, float h ) { position.Set( x, y, w, h ); }

	//! Renders a debug graph using the provided render system interface.
	void Render( idRenderSystem* gui );

private:
	const class idMaterial* white;
	const class idMaterial* font;

	idVec4					bgColor;
	idVec4					fontColor;
	fillMode_t				mode;
	bool					sideways;
	float					border;
	idVec4					position;
	bool					enable;

	struct graphPlot_t {
		float  value;
		idVec4 color;
	};
	idList<graphPlot_t> bars;
	idList<graphPlot_t> grid;
	idList<idStr>		labels;
};

#endif
