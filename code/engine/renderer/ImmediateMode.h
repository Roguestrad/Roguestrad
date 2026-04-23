/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 2016 Johannes Ohlemacher (http://github.com/eXistence/fhDOOM)

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

#pragma once

class idDrawVert;

enum GFXenum {
	GFX_INVALID_ENUM = 0x0500,
	GFX_LINES		 = 0x0001,
	GFX_LINE_LOOP	 = 0x0002,
	GFX_TRIANGLES	 = 0x0004,
	GFX_QUADS		 = 0x0007,
	GFX_QUAD_STRIP	 = 0x0008,
	GFX_POLYGON		 = 0x0009,
};

/*!
	\class fhImmediateMode
	\brief A class for managing immediate mode rendering operations with flexible vertex and texture coordinate handling.

	This class provides a structured interface for immediate mode rendering, allowing for the construction of graphical primitives through a sequence of vertex, color, and texture coordinate commands.
   It supports various primitive types and offers methods for setting up rendering states, adding geometry, and submitting draw operations to a command list. The class manages internal buffers for
   vertices and indices, and provides functionality for tracking rendering statistics. It is designed to be used in a graphics rendering pipeline where immediate mode drawing is needed, with support
   for both geometry-only and full rendering modes. The implementation handles color component scaling for alpha values and offers methods for both 2D and 3D vertex positioning. The class is expected
   to be initialized with a command list and can be reset or shut down to free resources.

*/
class fhImmediateMode
{
public:
	//! Initializes a new immediate mode rendering context with the specified command list and optional geometry-only flag.
	explicit fhImmediateMode( nvrhi::ICommandList* _commandList, bool geometryOnly = false );

	//! Destructor for fhImmediateMode that ends the immediate mode rendering.
	~fhImmediateMode();

	//! Sets the current texture for the immediate mode rendering context.
	void		SetTexture( idImage* texture );

	//! Initializes a new drawing operation with the specified primitive type.
	void		Begin( GFXenum mode );

	//! Sets the current texture coordinates for subsequent rendering operations.
	void		TexCoord2f( float s, float t );

	//! Sets the texture coordinates using a 2D float vector
	void		TexCoord2fv( const float* v );

	//! Sets the color using three float values for red, green, and blue components.
	void		Color3fv( const float* c );

	//! Sets the color for subsequent drawing operations using RGB values.
	void		Color3f( float r, float g, float b );

	/*!
		\brief Sets the current color using float values for red, green, blue, and alpha components

		This function takes four float values representing the red, green, blue, and alpha color components and converts them to byte values by scaling by 255.0f. The resulting byte values are stored
	   in the currentColor array which is used for subsequent rendering operations. Each color component should be in the range [0.0, 1.0] to produce the expected results

		\param r red color component, expected to be in range [0.0, 1.0]
		\param g green color component, expected to be in range [0.0, 1.0]
		\param b blue color component, expected to be in range [0.0, 1.0]
		\param a alpha color component, expected to be in range [0.0, 1.0]
	*/
	void		Color4f( float r, float g, float b, float a );

	//! Sets the current color using four floating-point values.
	void		Color4fv( const float* c );

	//! Sets the current color using four unsigned bytes.
	void		Color4ubv( const byte* bytes );

	//! Sets a 3D vertex position from a float array.
	void		Vertex3fv( const float* c );

	//! Adds a 3D vertex with the specified coordinates to the current drawing mode.
	void		Vertex3f( float x, float y, float z );

	//! Sets the current vertex position in 2D space.
	void		Vertex2f( float x, float y );

	//! Ends the immediate mode rendering and submits the draw commands to the command list.
	void		End();

	void		Sphere( float radius, int rings, int sectors, bool inverse = false );

	//! Returns the current graphics mode stored in the immediate mode renderer.
	GFXenum		getCurrentMode() const { return currentMode; }

	//! Adds triangles to the immediate mode renderer from a polygon defined by a list of vertices.
	static void AddTrianglesFromPolygon( fhImmediateMode& im, const idVec3* xyz, int num );

	//! Initializes the immediate mode rendering system with the provided command list.
	static void Init( nvrhi::ICommandList* commandList );

	//! Destroys the immediate mode rendering resources by freeing the vertex and index buffers.
	static void Shutdown();

	//! Resets the statistics counters for draw calls and vertex sizes.
	static void ResetStats();

	//! Returns the number of draw calls made in the immediate mode rendering context.
	static int	DrawCallCount();

	//! Returns the vertex size used for draw calls in immediate mode.
	static int	DrawCallVertexSize();

private:
	//! Initializes the vertex and index buffers for immediate mode rendering using the provided command list.
	static void				 InitBuffers( nvrhi::ICommandList* commandList );

	nvrhi::CommandListHandle commandList;
	static idVertexBuffer	 vertexBuffer;
	static idIndexBuffer	 indexBuffer;

	bool					 geometryOnly;
	float					 currentTexCoord[2] = {};
	GFXenum					 currentMode;
	byte					 currentColor[4];
	idImage*				 currentTexture;
	int						 drawVertsUsed;

	// idDrawVert*                     vertexPointer;
	// triIndex_t*                     indexPointer;
	// int							    numVerts;
	// int							    numIndexes;

	static int				 drawCallCount;
	static int				 drawCallVertexSize;
};

/*!
	\class fhLineBuffer
	\brief A utility class for buffering and rendering line segments in 3D space.
*/
class fhLineBuffer
{
public:
	fhLineBuffer();
	~fhLineBuffer();

	void Add( idVec3 from, idVec3 to, idVec4 color );
	void Add( idVec3 from, idVec3 to, idVec3 color );
	void Clear();
	void Commit();

private:
	int			verticesAllocated;
	int			verticesUsed;
	idDrawVert* vertices;
};
