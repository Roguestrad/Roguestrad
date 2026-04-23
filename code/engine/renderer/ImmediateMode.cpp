/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2016 Johannes Ohlemacher (http://github.com/eXistence/fhDOOM)
Copyright (C) 2022 Robert Beckebans (BFG integration)

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

#include "RenderCommon.h"
#include "ImmediateMode.h"

#include <engine/sys/DeviceManager.h>
extern DeviceManager* deviceManager;

namespace
{
const int  c_drawVertsCapacity	 = ( 40000 * 4 );
const int  c_drawIndexesCapacity = ( c_drawVertsCapacity * 2 );

idDrawVert drawVerts[c_drawVertsCapacity];
triIndex_t lineIndices[c_drawIndexesCapacity];
triIndex_t sphereIndices[c_drawIndexesCapacity];

bool	   active = false;
}

int			   fhImmediateMode::drawCallCount	   = 0;
int			   fhImmediateMode::drawCallVertexSize = 0;

idVertexBuffer fhImmediateMode::vertexBuffer;
idIndexBuffer  fhImmediateMode::indexBuffer;

void		   fhImmediateMode::Init( nvrhi::ICommandList* commandList )
{
	for( int i = 0; i < c_drawVertsCapacity * 2; ++i ) {
		lineIndices[i] = i;
	}

	ResetStats();
	InitBuffers( commandList );
}

void fhImmediateMode::Shutdown()
{
	vertexBuffer.FreeBufferObject();
	indexBuffer.FreeBufferObject();
}

void fhImmediateMode::ResetStats()
{
	drawCallCount	   = 0;
	drawCallVertexSize = 0;
}

int fhImmediateMode::DrawCallCount()
{
	return drawCallCount;
}

int fhImmediateMode::DrawCallVertexSize()
{
	return drawCallVertexSize;
}

void fhImmediateMode::InitBuffers( nvrhi::ICommandList* commandList )
{
	commandList->open();
	vertexBuffer.AllocBufferObject( nullptr, c_drawVertsCapacity * sizeof( idDrawVert ), bufferUsageType_t::BU_STATIC, commandList );
	indexBuffer.AllocBufferObject( nullptr, c_drawIndexesCapacity * sizeof( triIndex_t ), bufferUsageType_t::BU_STATIC, commandList );
	commandList->close();
	deviceManager->GetDevice()->executeCommandList( commandList );
}

fhImmediateMode::fhImmediateMode( nvrhi::ICommandList* _commandList, bool geometryOnly ) :
	commandList( _commandList ),
	drawVertsUsed( 0 ),
	currentTexture( nullptr ),
	geometryOnly( geometryOnly ),
	currentMode( GFX_INVALID_ENUM )
{
}

fhImmediateMode::~fhImmediateMode()
{
	End();
}

void fhImmediateMode::SetTexture( idImage* texture )
{
	currentTexture = texture;
}

void fhImmediateMode::Begin( GFXenum mode )
{
	End();
	assert( !active );
	active = true;

	currentMode	  = mode;
	drawVertsUsed = 0;
}

void fhImmediateMode::End()
{
	active = false;
	if( !drawVertsUsed ) {
		return;
	}

	vertexBuffer.Update( drawVerts, drawVertsUsed * sizeof( idDrawVert ), 0, false, commandList );
	indexBuffer.Update( lineIndices, drawVertsUsed * sizeof( triIndex_t ), 0, false, commandList );

	renderProgManager.CommitConstantBuffer( commandList, true );

	int																	  bindingLayoutType = renderProgManager.BindingLayoutType();

	idStaticList<nvrhi::BindingLayoutHandle, nvrhi::c_MaxBindingLayouts>* layouts = renderProgManager.GetBindingLayout( bindingLayoutType );

	for( int i = 0; i < layouts->Num(); i++ ) {
		if( !backEnd.currentBindingSets[i] || *backEnd.currentBindingSets[i]->getDesc() != backEnd.pendingBindingSetDescs[bindingLayoutType][i] ||
			bindingLayoutType != backEnd.prevBindingLayoutType ) {
			backEnd.currentBindingSets[i] = backEnd.bindingCache.GetOrCreateBindingSet( backEnd.pendingBindingSetDescs[bindingLayoutType][i], ( *layouts )[i] );
		}
	}

	uint64		stateBits = backEnd.glStateBits;

	int			program = renderProgManager.CurrentProgram();
	PipelineKey key { stateBits, program, static_cast<int>( backEnd.depthBias ), backEnd.slopeScaleBias, backEnd.currentFrameBuffer };
	auto		pipeline = backEnd.pipelineCache.GetOrCreatePipeline( key );

	{
		nvrhi::GraphicsState state;

		for( int i = 0; i < layouts->Num(); i++ ) {
			state.bindings.push_back( backEnd.currentBindingSets[i] );
		}

		state.indexBuffer	= { indexBuffer.GetAPIObject(), nvrhi::Format::R16_UINT, 0 };
		state.vertexBuffers = { { vertexBuffer.GetAPIObject(), 0, 0 } };
		state.pipeline		= pipeline;
		state.framebuffer	= backEnd.currentFrameBuffer->GetApiObject();

		nvrhi::Viewport viewport { ( float )backEnd.currentViewport.x1, ( float )backEnd.currentViewport.x2, ( float )backEnd.currentViewport.y1, ( float )backEnd.currentViewport.y2, 0.0f, 1.0f };
		state.viewport.addViewport( viewport );
		state.viewport.addScissorRect( nvrhi::Rect( viewport ) );

		commandList->setGraphicsState( state );
	}

	nvrhi::DrawArguments args;
	args.vertexCount = drawVertsUsed;
	commandList->drawIndexed( args );

	// RB: added stats
	backEnd.pc.c_drawElements++;
	backEnd.pc.c_drawIndexes += drawVertsUsed;

	// reset
	drawVertsUsed = 0;
	currentMode	  = GFX_INVALID_ENUM;
}

void fhImmediateMode::TexCoord2f( float s, float t )
{
	currentTexCoord[0] = s;
	currentTexCoord[1] = t;
}

void fhImmediateMode::TexCoord2fv( const float* v )
{
	TexCoord2f( v[0], v[1] );
}

void fhImmediateMode::Color4f( float r, float g, float b, float a )
{
	currentColor[0] = static_cast<byte>( r * 255.0f );
	currentColor[1] = static_cast<byte>( g * 255.0f );
	currentColor[2] = static_cast<byte>( b * 255.0f );
	currentColor[3] = static_cast<byte>( a * 255.0f );
}

void fhImmediateMode::Color3f( float r, float g, float b )
{
	Color4f( r, g, b, 1.0f );
}

void fhImmediateMode::Color3fv( const float* c )
{
	Color4f( c[0], c[1], c[2], 1.0f );
}

void fhImmediateMode::Color4fv( const float* c )
{
	Color4f( c[0], c[1], c[2], c[3] );
}

void fhImmediateMode::Color4ubv( const byte* bytes )
{
	currentColor[0] = bytes[0];
	currentColor[1] = bytes[1];
	currentColor[2] = bytes[2];
	currentColor[3] = bytes[3];
}

void fhImmediateMode::Vertex3fv( const float* c )
{
	Vertex3f( c[0], c[1], c[2] );
}

void fhImmediateMode::Vertex3f( float x, float y, float z )
{
	if( drawVertsUsed + 1 >= c_drawVertsCapacity ) {
		return;
	}

	// we don't want to draw deprecated quads/polygons... correct them by re-adding
	//  previous vertices, so we render triangles instead of quads/polygons
	//  NOTE: this only works for convex polygons (just as GL_POLYGON)
	if( ( currentMode == GFX_POLYGON || currentMode == GFX_QUADS ) && drawVertsUsed >= 3 && drawVertsUsed + 3 < c_drawVertsCapacity ) {
		drawVerts[drawVertsUsed]	 = drawVerts[0];
		drawVerts[drawVertsUsed + 1] = drawVerts[drawVertsUsed - 1];
		drawVertsUsed += 2;
	}

	if( currentMode == GFX_QUAD_STRIP && drawVertsUsed >= 3 && drawVertsUsed + 3 < c_drawVertsCapacity ) {
		if( drawVertsUsed % 6 == 0 ) {
			drawVerts[drawVertsUsed]	 = drawVerts[drawVertsUsed - 3];
			drawVerts[drawVertsUsed + 1] = drawVerts[drawVertsUsed - 1];
		} else if( drawVertsUsed % 3 == 0 ) {
			drawVerts[drawVertsUsed]	 = drawVerts[drawVertsUsed - 1];
			drawVerts[drawVertsUsed + 1] = drawVerts[drawVertsUsed - 2];
		}
		drawVertsUsed += 2;
	}

	/*
	if( ( currentMode == GFX_LINES ) &&
			drawVertsUsed >= 2 &&
			drawVertsUsed + 1 < c_drawVertsCapacity )
	{
		// duplicate the last one if new line starts
		if( drawVertsUsed % 2 == 0 )
		{
			drawVerts[drawVertsUsed] = drawVerts[drawVertsUsed -1];
			drawVertsUsed += 1;
		}
	}
	*/

	idDrawVert& vertex = drawVerts[drawVertsUsed++];
	vertex.xyz.Set( x, y, z );
	vertex.SetTexCoord( currentTexCoord[0], currentTexCoord[1] );
	vertex.color[0] = currentColor[0];
	vertex.color[1] = currentColor[1];
	vertex.color[2] = currentColor[2];
	vertex.color[3] = currentColor[3];
}

void fhImmediateMode::Vertex2f( float x, float y )
{
	Vertex3f( x, y, 0.0f );
}

void fhImmediateMode::AddTrianglesFromPolygon( fhImmediateMode& im, const idVec3* xyz, int num )
{
	assert( im.getCurrentMode() == GFX_TRIANGLES );

	if( num < 3 ) {
		return;
	}

	for( int i = 0; i < num; ++i ) {
		if( i > 0 && i % 3 == 0 ) {
			im.Vertex3fv( xyz[0].ToFloatPtr() );
			im.Vertex3fv( xyz[i - 1].ToFloatPtr() );
		}
		im.Vertex3fv( xyz[i].ToFloatPtr() );
	}
}
