/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2025-2026 Robert Beckebans

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
#include "../renderer/Font.h"
#include "../renderer/frontend/tr_local.h" //  for R_LoadImage

#include "libs/pugixml/pugixml.hpp"

static void ParsePointsFromString( const char* pointsStr, idList<idVec2>& verts )
{
	verts.Clear();
	if( !pointsStr || !pointsStr[0] ) {
		return;
	}

	idLexer lexer;
	lexer.LoadMemory( pointsStr, idStr::Length( pointsStr ), "points" );
	lexer.SetFlags( LEXFL_NOERRORS | LEXFL_NOFATALERRORS );

	while( !lexer.EndOfFile() ) {
		idVec2 v;
		v.x = lexer.ParseFloat();
		if( !lexer.ExpectTokenString( "," ) ) {
			break;
		}
		v.y = lexer.ParseFloat();
		verts.Append( v );

		if( lexer.CheckTokenString( " " ) ) {
			continue;
		}
		if( lexer.EndOfFile() ) {
			break;
		}
	}

	lexer.FreeSource();
}

int idSWF::ResolveSVGHref( const pugi::xml_node& node ) const
{
	idStr href = node.attribute( "xlink:href" ).value();
	if( href.IsEmpty() ) {
		href = node.attribute( "href" ).value();
	}
	if( href.IsEmpty() ) {
		return -1;
	}

	const char* name	 = ( href[0] == '#' ) ? href.c_str() + 1 : href.c_str();
	const int*	mappedID = NULL;
	svgNameToCharID.Get( idStr( name ), &mappedID );
	return mappedID ? *mappedID : atoi( name );
}

void idSWF::ParseSVG_Image( const pugi::xml_node& node, int characterID, idSWFDictionaryEntry& entry )
{
	// entry.type = SWF_DICT_IMAGE;

	idStr imagePath = node.attribute( "xlink:href" ).value();
	if( imagePath.IsEmpty() ) {
		imagePath = node.attribute( "href" ).value();
	}
	if( imagePath[0] == '.' ) {
		// internal image in the atlas
		entry.material = NULL;
	} else {
		imagePath	   = "swf/" + imagePath;
		entry.material = declManager->FindMaterial( imagePath );
	}

	// Store the SVG display size from the <image> attributes.
	// These may differ from the actual pixel dimensions of the image file
	// (e.g. a 640x480 PNG displayed at 1280x960 in SVG).
	entry.svgDisplaySize.x = node.attribute( "width" ).as_float();
	entry.svgDisplaySize.y = node.attribute( "height" ).as_float();

	byte*	  imageData = NULL;
	int		  width, height;
	ID_TIME_T timestamp;
	R_LoadImage( imagePath.c_str(), &imageData, &width, &height, &timestamp, false, NULL );
	if( imageData != NULL ) {
		PackImage( characterID, imageData, width, height );

		Mem_Free( imageData );
	}
}

// Parse Shape from <g> Node
void idSWF::ParseSVG_Shape( const pugi::xml_node& node, idSWFShape* shape )
{
	if( !shape )
		return;

	pugi::xml_node useNode = node.child( "use" );
	if( useNode ) {
		const char* linkType = useNode.attribute( "link-type" ).value();

		if( idStr::Icmp( linkType, "BITMAP" ) == 0 ) {
			// this is a bitmap helper fill shape, not necessary for SVG but needed for SWF
			// this is generated for each image

			idSWFShapeDrawFill& fill = shape->fillDraws.Alloc();
			fill.style.type			 = 4; // Bitmap fill
			fill.style.subType		 = 0; // near clamp (optional)

			// --- 1. Parse href → bitmapID ---
			fill.style.bitmapID = ResolveSVGHref( useNode );

			// --- 2. Parse transform → startMatrix ---
			fill.style.startMatrix.xx = 20.0f; // SVG units to SWF twips
			fill.style.startMatrix.yy = 20.0f;
			// fill.style.startMatrix.ParseSVGTransformFromString( useNode.attribute( "transform" ).value() );
			fill.style.endMatrix = fill.style.startMatrix;

			// --- 3. Fetch image size from packImages ---
			int			   refID	 = fill.style.bitmapID;
			imageToPack_t* packImage = NULL;
			for( imageToPack_t& img : packImages ) {
				if( img.characterID == refID ) {
					packImage = &img;
					break;
				}
			}

			if( !packImage ) {
				idLib::Warning( "SVG Load: Could not find packed image for characterID %d", refID );
				// Fallback: 128x64
				shape->startBounds = swfRect_t( 0, 0, 128, 64 );
				shape->endBounds   = shape->startBounds;
			} else {
				shape->startBounds = swfRect_t( 0, 0, packImage->trueSize.x, packImage->trueSize.y );
				shape->endBounds   = shape->startBounds;
			}

			// --- 4. Create 4 vertices (rectangle) ---
			float w = shape->startBounds.br.x - shape->startBounds.tl.x;
			float h = shape->startBounds.br.y - shape->startBounds.tl.y;

			fill.startVerts.SetNum( 4 );
			fill.startVerts[0].x = w;
			fill.startVerts[0].y = h; // bottom-right
			fill.startVerts[1].x = 0;
			fill.startVerts[1].y = h; // bottom-left
			fill.startVerts[2].x = 0;
			fill.startVerts[2].y = 0; // top-left
			fill.startVerts[3].x = w;
			fill.startVerts[3].y = 0; // top-right

			// fill.endVerts = fill.startVerts;

			// --- 5. Create quad (2 triangles) ---
			fill.indices.SetNum( 6 );
			fill.indices[0] = 0;
			fill.indices[1] = 1;
			fill.indices[2] = 2;
			fill.indices[3] = 0;
			fill.indices[4] = 2;
			fill.indices[5] = 3;
		}
	} else {
		// this is a regular shape

		for( pugi::xml_node poly = node.child( "polygon" ); poly; poly = poly.next_sibling( "polygon" ) ) {
			idSWFShapeDrawFill fill;
			fill.style.type = 0;
			fill.style.startColor.ParseSVGColorFromString( poly.attribute( "fill" ).value() );
			fill.style.endColor = fill.style.startColor;

			ParsePointsFromString( poly.attribute( "points" ).value(), fill.startVerts );
			fill.endVerts = fill.startVerts;

			for( int idx = 0; idx < fill.startVerts.Num(); idx++ ) {
				fill.indices.Append( idx );
			}

			shape->fillDraws.Append( fill );
		}

		for( pugi::xml_node line = node.child( "polyline" ); line; line = line.next_sibling( "polyline" ) ) {
			idSWFShapeDrawLine ldraw;
			ldraw.style.startColor.ParseSVGColorFromString( line.attribute( "stroke" ).value() );
			ldraw.style.endColor   = ldraw.style.startColor;
			ldraw.style.startWidth = line.attribute( "stroke-width" ).as_float();
			ldraw.style.endWidth   = ldraw.style.startWidth;

			ParsePointsFromString( line.attribute( "points" ).value(), ldraw.startVerts );
			ldraw.endVerts = ldraw.startVerts;

			// degenerate triangle format: v0,v0,v1 per segment (matches Flash ShapeParser / AllocTris GL_TRIANGLES convention)
			for( int idx = 0; idx < ldraw.startVerts.Num() - 1; idx++ ) {
				ldraw.indices.Append( idx );
				ldraw.indices.Append( idx );
				ldraw.indices.Append( idx + 1 );
			}

			shape->lineDraws.Append( ldraw );
		}

		// <line x1 y1 x2 y2> – two-point line segment
		for( pugi::xml_node lineNode = node.child( "line" ); lineNode; lineNode = lineNode.next_sibling( "line" ) ) {
			idSWFShapeDrawLine ldraw;
			ldraw.style.startColor.ParseSVGColorFromString( lineNode.attribute( "stroke" ).value() );
			ldraw.style.endColor   = ldraw.style.startColor;
			ldraw.style.startWidth = lineNode.attribute( "stroke-width" ).as_float();
			ldraw.style.endWidth   = ldraw.style.startWidth;

			ldraw.startVerts.Append( idVec2( lineNode.attribute( "x1" ).as_float(), lineNode.attribute( "y1" ).as_float() ) );
			ldraw.startVerts.Append( idVec2( lineNode.attribute( "x2" ).as_float(), lineNode.attribute( "y2" ).as_float() ) );
			ldraw.endVerts = ldraw.startVerts;

			// degenerate triangle format: v0,v0,v1 per segment (matches Flash ShapeParser / AllocTris GL_TRIANGLES convention)
			ldraw.indices.Append( 0 );
			ldraw.indices.Append( 0 );
			ldraw.indices.Append( 1 );

			shape->lineDraws.Append( ldraw );
		}

		// <rect x y width height [rx]> – solid filled rectangle (rx ignored for simplicity)
		for( pugi::xml_node rectNode = node.child( "rect" ); rectNode; rectNode = rectNode.next_sibling( "rect" ) ) {
			const char* fillStr = rectNode.attribute( "fill" ).value();
			if( fillStr && fillStr[0] && idStr::Icmp( fillStr, "none" ) != 0 ) {
				float			   x = rectNode.attribute( "x" ).as_float();
				float			   y = rectNode.attribute( "y" ).as_float();
				float			   w = rectNode.attribute( "width" ).as_float();
				float			   h = rectNode.attribute( "height" ).as_float();

				idSWFShapeDrawFill fill;
				fill.style.type = 0;
				fill.style.startColor.ParseSVGColorFromString( fillStr );
				fill.style.endColor = fill.style.startColor;

				fill.startVerts.Append( idVec2( x, y ) );		  // 0: top-left
				fill.startVerts.Append( idVec2( x + w, y ) );	  // 1: top-right
				fill.startVerts.Append( idVec2( x + w, y + h ) ); // 2: bottom-right
				fill.startVerts.Append( idVec2( x, y + h ) );	  // 3: bottom-left
				fill.endVerts = fill.startVerts;

				fill.indices.Append( 0 );
				fill.indices.Append( 1 );
				fill.indices.Append( 2 );
				fill.indices.Append( 0 );
				fill.indices.Append( 2 );
				fill.indices.Append( 3 );

				shape->fillDraws.Append( fill );
			}

			const char* strokeStr = rectNode.attribute( "stroke" ).value();
			if( strokeStr && strokeStr[0] && idStr::Icmp( strokeStr, "none" ) != 0 ) {
				float			   x = rectNode.attribute( "x" ).as_float();
				float			   y = rectNode.attribute( "y" ).as_float();
				float			   w = rectNode.attribute( "width" ).as_float();
				float			   h = rectNode.attribute( "height" ).as_float();

				idSWFShapeDrawLine ldraw;
				ldraw.style.startColor.ParseSVGColorFromString( strokeStr );
				ldraw.style.endColor   = ldraw.style.startColor;
				ldraw.style.startWidth = rectNode.attribute( "stroke-width" ).as_float();
				ldraw.style.endWidth   = ldraw.style.startWidth;

				// closed loop: TL -> TR -> BR -> BL -> TL
				ldraw.startVerts.Append( idVec2( x, y ) );
				ldraw.startVerts.Append( idVec2( x + w, y ) );
				ldraw.startVerts.Append( idVec2( x + w, y + h ) );
				ldraw.startVerts.Append( idVec2( x, y + h ) );
				ldraw.startVerts.Append( idVec2( x, y ) );
				ldraw.endVerts = ldraw.startVerts;

				// degenerate triangle format: v0,v0,v1 per segment (matches Flash ShapeParser / AllocTris GL_TRIANGLES convention)
				for( int idx = 0; idx < ldraw.startVerts.Num() - 1; idx++ ) {
					ldraw.indices.Append( idx );
					ldraw.indices.Append( idx );
					ldraw.indices.Append( idx + 1 );
				}

				shape->lineDraws.Append( ldraw );
			}
		}

		// <circle cx cy r> – approximated as N-gon polygon
		for( pugi::xml_node circleNode = node.child( "circle" ); circleNode; circleNode = circleNode.next_sibling( "circle" ) ) {
			float		cx = circleNode.attribute( "cx" ).as_float();
			float		cy = circleNode.attribute( "cy" ).as_float();
			float		r  = circleNode.attribute( "r" ).as_float();

			// choose segment count based on radius: small circles need fewer segments
			const int	numSegments = ( r <= 3.0f ) ? 12 : ( r <= 10.0f ) ? 16 : 24;

			const char* fillStr = circleNode.attribute( "fill" ).value();
			if( fillStr && fillStr[0] && idStr::Icmp( fillStr, "none" ) != 0 ) {
				idSWFShapeDrawFill fill;
				fill.style.type = 0;
				fill.style.startColor.ParseSVGColorFromString( fillStr );
				fill.style.endColor = fill.style.startColor;

				for( int i = 0; i < numSegments; i++ ) {
					float ang = idMath::TWO_PI * i / numSegments;
					fill.startVerts.Append( idVec2( cx + r * idMath::Cos( ang ), cy + r * idMath::Sin( ang ) ) );
				}
				fill.endVerts = fill.startVerts;

				// triangle fan from vertex 0
				for( int i = 1; i < numSegments - 1; i++ ) {
					fill.indices.Append( 0 );
					fill.indices.Append( i );
					fill.indices.Append( i + 1 );
				}

				shape->fillDraws.Append( fill );
			}

			const char* strokeStr = circleNode.attribute( "stroke" ).value();
			if( strokeStr && strokeStr[0] && idStr::Icmp( strokeStr, "none" ) != 0 ) {
				idSWFShapeDrawLine ldraw;
				ldraw.style.startColor.ParseSVGColorFromString( strokeStr );
				ldraw.style.endColor   = ldraw.style.startColor;
				ldraw.style.startWidth = circleNode.attribute( "stroke-width" ).as_float();
				ldraw.style.endWidth   = ldraw.style.startWidth;

				// numSegments+1 verts so the last vert closes back to the first position
				for( int i = 0; i <= numSegments; i++ ) {
					float ang = idMath::TWO_PI * i / numSegments;
					ldraw.startVerts.Append( idVec2( cx + r * idMath::Cos( ang ), cy + r * idMath::Sin( ang ) ) );
				}
				ldraw.endVerts = ldraw.startVerts;

				// degenerate triangle format: v0,v0,v1 per segment (matches Flash ShapeParser / AllocTris GL_TRIANGLES convention)
				for( int idx = 0; idx < ldraw.startVerts.Num() - 1; idx++ ) {
					ldraw.indices.Append( idx );
					ldraw.indices.Append( idx );
					ldraw.indices.Append( idx + 1 );
				}

				shape->lineDraws.Append( ldraw );
			}
		}

		shape->startBounds.tl = vec2_zero;
		shape->startBounds.br = idVec2( 100, 100 );
		shape->endBounds	  = shape->startBounds;
	}
}

void idSWF::ParseSVG_Text( const pugi::xml_node& node, idSWFEditText* et )
{
	const pugi::xml_node& textNode = node;
	if( textNode ) {
		idVec2 anchorPos;
		anchorPos.x	   = textNode.attribute( "x" ).as_float();
		anchorPos.y	   = textNode.attribute( "y" ).as_float();
		et->fontHeight = FLOAT2SWFTWIP( textNode.attribute( "font-size" ).as_float() );
		et->color.ParseSVGColorFromString( textNode.attribute( "fill" ).value() );

		idStr alignStr = textNode.attribute( "text-anchor" ).value();
		if( alignStr.Icmp( "middle" ) == 0 ) {
			et->align		= SWF_ET_ALIGN_CENTER;
			et->bounds.tl.x = anchorPos.x - 200;
			et->bounds.tl.y = anchorPos.y - SWFTWIP( et->fontHeight );
			et->bounds.br.x = anchorPos.x + 200;
			et->bounds.br.y = anchorPos.y;
		} else if( alignStr.Icmp( "end" ) == 0 ) {
			et->align		= SWF_ET_ALIGN_RIGHT;
			et->bounds.tl.x = anchorPos.x - 400;
			et->bounds.tl.y = anchorPos.y - SWFTWIP( et->fontHeight );
			et->bounds.br.x = anchorPos.x;
			et->bounds.br.y = anchorPos.y;
		} else {
			et->align		= SWF_ET_ALIGN_LEFT;
			et->bounds.tl.x = anchorPos.x;
			et->bounds.tl.y = anchorPos.y - SWFTWIP( et->fontHeight );
			et->bounds.br.x = anchorPos.x + 400; // default width
			et->bounds.br.y = anchorPos.y;
		}

		// Some SVG exporters emit multiline text as a series of <tspan> children.
		// Example:
		//   <text ...>
		//     <tspan ...>LINE1</tspan>
		//     <tspan ...>LINE2</tspan>
		//   </text>
		// In that case, concatenate tspans with '\n'. If there are no tspans, fall back to
		// the text node's direct content.
		idStr composedText;
		int	  numLines = 0;
		float totalDy  = 0.0f;
		for( pugi::xml_node tspan = textNode.child( "tspan" ); tspan; tspan = tspan.next_sibling( "tspan" ) ) {
			const char* line = tspan.text().as_string();
			if( numLines > 0 ) {
				composedText += "\n";
				// SVG dy is relative; add it to extend bounds for multiline blocks.
				// If dy is not present, assume roughly one font-size line advance.
				totalDy += tspan.attribute( "dy" ).as_float( textNode.attribute( "font-size" ).as_float() );
			}
			composedText += line;
			numLines++;
		}
		if( numLines > 0 ) {
			et->initialText = composedText;
			et->flags |= SWF_ET_MULTILINE;

			// Expand bounds vertically for additional lines.
			// NOTE: bounds are in the same coordinate space as the SVG x/y.
			et->bounds.br.y = anchorPos.y + totalDy;
		} else {
			et->initialText = textNode.text().as_string();
		}

		idStr fontName = textNode.attribute( "font-family" ).value();
		if( fontName.IsEmpty() ) {
			fontName = "Arial";
		}

		// find font in dictionary
		for( int i = 0; i < dictionary.Num(); i++ ) {
			idSWFDictionaryEntry& dictEntry = dictionary[i];
			if( dictEntry.type == SWF_DICT_FONT ) {
				if( fontName.Icmp( dictEntry.font->fontID->GetName(), fontName ) == 0 ) {
					et->fontID = i;
					break;
				}
			}
		}

#if 0
		// Flash attributes that are not required by SVG but by this SWF implementation
		idStr boundsStr = textNode.attribute( "data-bounds" ).value();
		if( !boundsStr.IsEmpty() ) {
			idLexer lexer( boundsStr, boundsStr.Length(), "bounds" );
			lexer.ExpectTokenString( "[" );
			et->bounds.tl.x = lexer.ParseFloat();
			lexer.ExpectTokenString( "," );
			et->bounds.tl.y = lexer.ParseFloat();
			lexer.ExpectTokenString( "," );
			et->bounds.br.x = lexer.ParseFloat();
			lexer.ExpectTokenString( "," );
			et->bounds.br.y = lexer.ParseFloat();
			lexer.ExpectTokenString( "]" );
		}

		et->flags	  = textNode.attribute( "data-flags" ).as_int( 0 );
		et->leading	  = textNode.attribute( "data-leading" ).as_int( 0 );
		et->maxLength = textNode.attribute( "data-maxlength" ).as_int( 65535 );
		et->variable  = textNode.attribute( "data-variable" ).value();
#endif
	}
}

void idSWF::ParseSVG_Font( const pugi::xml_node& node, idSWFFont* font )
{
	idStr fontName = node.attribute( "data-name" ).value();
	if( fontName.IsEmpty() ) {
		fontName = "Arial";
	}

	idFont* renderFont = renderSystem->RegisterFont( fontName );
	if( !renderFont->IsValid() ) {
		idLib::Warning( "Font '%s' not found, using default", fontName.c_str() );
		renderFont = renderSystem->RegisterFont( "Arial" ); // fallback
	}

	font->fontID = renderFont;
}

bool idSWF::LoadSVG( const char* filename )
{
	// Store base directory for sub-file resolution (e.g. "swf/" from "swf/shell.svg")
	svgBaseDir = filename;
	svgBaseDir.StripFilename();
	if( svgBaseDir.Length() > 0 && svgBaseDir[svgBaseDir.Length() - 1] != '/' ) {
		svgBaseDir += "/";
	}

	// Clean up any previous sub-documents
	for( int i = 0; i < svgSubDocuments.Num(); i++ ) {
		delete svgSubDocuments[i];
	}
	svgSubDocuments.Clear();

	idFile* f = fileSystem->OpenFileReadMemory( filename );
	if( f == NULL || f->Length() <= 0 ) {
		idLib::Warning( "SVG Load failed: Could not open file %s", filename );
		delete f;
		return false;
	}

	int			fileLength = f->Length();
	const char* fileData   = ( const char* )Mem_Alloc( fileLength, TAG_SWF );
	size_t		fileSize   = f->Read( ( byte* )fileData, fileLength );
	delete f;
	f = NULL;

	int					   timeStart = Sys_Milliseconds();

	pugi::xml_document	   doc;
	pugi::xml_parse_result result = doc.load_buffer( fileData, fileSize );
	if( !result ) {
		idLib::Warning( "SVG Load failed: %s", result.description() );
		return false;
	}

	pugi::xml_node svgNode = doc.child( "svg" );
	if( !svgNode ) {
		return false;
	}

	// Detect whether this SVG was exported by our engine.
	// Engine-exported SVGs contain a "data-exported-from" attribute on the root <svg> node
	// and already have proper SHAPE entries wrapping each IMAGE via <use link-type="BITMAP">.
	// External SVGs (e.g. hand-authored or from other tools) only have bare <image> nodes
	// and need auto-generated bitmap shapes so the SWF render pipeline can display them.
	bool exportedFromEngine = false;
	{
		const char* exportedFrom = svgNode.attribute( "data-exported-from" ).value();
		if( exportedFrom && exportedFrom[0] != '\0' ) {
			exportedFromEngine = true;
		}
	}

	frameWidth	= svgNode.attribute( "width" ).as_float();
	frameHeight = svgNode.attribute( "height" ).as_float();
	frameRate	= 60 * 256;

	bool											  isUnfolded = true;
	idHashTableT<idStr, idSWFSprite::svgAnimTarget_t> svgTargetMap;
	idList<pugi::xml_node>							  svgAnimations;

	// Clear the name-to-ID map; it will be populated below for non-engine SVGs.
	svgNameToCharID.Clear();

	pugi::xml_node defs = svgNode.child( "defs" );
	if( defs ) {
		// ---------- Pass 1: Determine numeric IDs for every <defs> child ----------
		// Engine-exported SVGs use purely numeric IDs (e.g. id="3", id="4").
		// Externally authored SVGs may use string IDs (e.g. id="background").
		// We assign numeric dictionary indices to every entry in this first pass
		// and build a name→index mapping so that <use xlink:href="#name"> can be
		// resolved later.

		// Collect all nodes that have an id attribute and figure out the max
		// pre-existing numeric ID so we can append string-named entries after it.
		struct defsNodeInfo_t {
			pugi::xml_node node;
			idStr		   name;	  // the raw id string
			int			   numericID; // -1 if the id is not purely numeric
		};
		idList<defsNodeInfo_t> defsNodes;
		int					   maxNumericID = -1;

		for( pugi::xml_node child = defs.first_child(); child; child = child.next_sibling() ) {
			pugi::xml_attribute idAttr = child.attribute( "id" );
			if( !idAttr ) {
				continue;
			}

			defsNodeInfo_t info;
			info.node = child;
			info.name = idAttr.value();

			// Check if the id string is a pure integer (e.g. "3", "25").
			if( idStr::IsNumeric( info.name ) ) {
				info.numericID = atoi( info.name.c_str() );
				maxNumericID   = idMath::Imax( maxNumericID, info.numericID );
			} else {
				info.numericID = -1;
			}

			defsNodes.Append( info );
		}

		// Assign numeric indices to string-named entries starting after the highest
		// numeric ID we found.  This guarantees no collisions.
		int nextFreeID = maxNumericID + 1;
		for( int i = 0; i < defsNodes.Num(); i++ ) {
			if( defsNodes[i].numericID == -1 ) {
				defsNodes[i].numericID = nextFreeID++;
			}
		}

		// Ensure the dictionary is large enough for all entries.
		int requiredSize = nextFreeID;
		if( requiredSize < maxNumericID + 1 ) {
			requiredSize = maxNumericID + 1;
		}
		dictionary.SetNum( requiredSize );

		// Populate the name→charID map for every entry (numeric and string alike).
		// For engine-exported SVGs with purely numeric IDs this is harmless but
		// still useful for consistency.
		for( int i = 0; i < defsNodes.Num(); i++ ) {
			svgNameToCharID.Set( defsNodes[i].name, defsNodes[i].numericID );
		}

		// ---------- Pass 2: Parse each <defs> child into the dictionary ----------
		for( int i = 0; i < defsNodes.Num(); i++ ) {
			pugi::xml_node		  n		  = defsNodes[i].node;
			int					  id	  = defsNodes[i].numericID;
			const char*			  tagName = n.name();

			idSWFDictionaryEntry& entry = dictionary[id];

			if( entry.type != SWF_DICT_NULL ) {
				idLib::Warning( "%s: Duplicate character %d (name '%s')", filename, id, defsNodes[i].name.c_str() );
				continue;
			}

			if( idStr::Icmp( tagName, "image" ) == 0 ) {
				ParseSVG_Image( n, id, entry );
			} else if( idStr::Icmp( tagName, "text" ) == 0 ) {
				entry.type	   = SWF_DICT_EDITTEXT;
				entry.edittext = new( TAG_SWF ) idSWFEditText;
				ParseSVG_Text( n, entry.edittext );
			} else if( idStr::Icmp( tagName, "rect" ) == 0 || idStr::Icmp( tagName, "line" ) == 0 || idStr::Icmp( tagName, "circle" ) == 0 || idStr::Icmp( tagName, "polygon" ) == 0 ||
					   idStr::Icmp( tagName, "polyline" ) == 0 ) {
				// Bare primitive element used directly as a shape (no wrapping <g>).
				// Wrap it in a synthetic parent so ParseSVG_Shape can iterate children normally.
				entry.type	= SWF_DICT_SHAPE;
				entry.shape = new( TAG_SWF ) idSWFShape;
				// ParseSVG_Shape iterates children of the node it receives, so we pass
				// a temporary wrapper node that has the primitive as its only child.
				// The easiest way: pass the node's *parent* — but that would drag in
				// siblings.  Instead we synthesise a one-child context by wrapping.
				// Since pugi doesn't allow creating nodes directly we create a tiny
				// in-memory document fragment.
				pugi::xml_document wrapper;
				pugi::xml_node	   wrapG = wrapper.append_child( "g" );
				wrapG.append_copy( n );
				ParseSVG_Shape( wrapG, entry.shape );
			} else if( idStr::Icmp( tagName, "g" ) == 0 ) {
				pugi::xml_node& g		 = n;
				const char*		dataType = g.attribute( "data-type" ).value();

				if( dataType && idStr::Icmp( dataType, "SHAPE" ) == 0 ) {
					entry.type	= SWF_DICT_SHAPE;
					entry.shape = new( TAG_SWF ) idSWFShape;
					ParseSVG_Shape( g, entry.shape );
				} else if( dataType && idStr::Icmp( dataType, "FONT" ) == 0 ) {
					entry.type = SWF_DICT_FONT;
					entry.font = new( TAG_SWF ) idSWFFont;
					ParseSVG_Font( g, entry.font );
				} else {
					entry.type	 = SWF_DICT_SPRITE;
					entry.sprite = new idSWFSprite( this );
					entry.sprite->LoadSVGNode_r( g, dictionary, isUnfolded, svgTargetMap, svgAnimations );
				}
			}
		}
	}

	// ---------- Auto-generate bitmap shapes for orphan images ----------
	// For non-engine SVGs: images that have no corresponding SHAPE with a bitmap
	// fill referencing them need a synthesized shape so the SWF render pipeline
	// can display them.  The svgNameToCharID map is updated so that the original
	// image name (e.g. "intro_placeholder") resolves to the *shape* ID, which is
	// what <use xlink:href="#intro_placeholder"> needs at runtime.
	if( !exportedFromEngine ) {
		// 1. Collect image IDs that are already referenced by a bitmap shape.
		idList<int> referencedImageIDs;
		for( int i = 0; i < dictionary.Num(); i++ ) {
			if( dictionary[i].type != SWF_DICT_SHAPE || dictionary[i].shape == NULL ) {
				continue;
			}
			for( int d = 0; d < dictionary[i].shape->fillDraws.Num(); d++ ) {
				if( dictionary[i].shape->fillDraws[d].style.type == 4 ) {
					referencedImageIDs.AddUnique( dictionary[i].shape->fillDraws[d].style.bitmapID );
				}
			}
		}

		// 2. Find orphan images and remember their string names so we can remap them.
		struct orphanImage_t {
			int	  imageID;
			idStr name; // original SVG id string of the <image>
		};
		idList<orphanImage_t> orphans;
		for( int i = 0; i < svgNameToCharID.Num(); i++ ) {
			idStr key;
			if( !svgNameToCharID.GetIndexKey( i, key ) ) {
				continue;
			}
			int* val = svgNameToCharID.GetIndex( i );
			if( val == NULL ) {
				continue;
			}
			int charID = *val;
			if( charID < 0 || charID >= dictionary.Num() ) {
				continue;
			}
			if( dictionary[charID].type == SWF_DICT_IMAGE && referencedImageIDs.FindIndex( charID ) == -1 ) {
				orphanImage_t& o = orphans.Alloc();
				o.imageID		 = charID;
				o.name			 = key;
			}
		}

		// 3. Synthesize a bitmap shape for each orphan and update the name map.
		if( orphans.Num() > 0 ) {
			int nextID = dictionary.Num();
			dictionary.SetNum( nextID + orphans.Num() );

			for( int j = 0; j < orphans.Num(); j++ ) {
				int					  imageID = orphans[j].imageID;
				int					  shapeID = nextID + j;

				idSWFDictionaryEntry& shapeEntry = dictionary[shapeID];
				shapeEntry.type					 = SWF_DICT_SHAPE;
				shapeEntry.shape				 = new( TAG_SWF ) idSWFShape;

				idSWFShapeDrawFill& fill = shapeEntry.shape->fillDraws.Alloc();
				fill.style.type			 = 4; // Bitmap fill
				fill.style.subType		 = 0;
				fill.style.bitmapID		 = imageID;

				// Fetch image pixel size from packImages
				imageToPack_t* packImage = NULL;
				for( imageToPack_t& img : packImages ) {
					if( img.characterID == imageID ) {
						packImage = &img;
						break;
					}
				}

				float pixelW, pixelH;
				if( !packImage ) {
					idLib::Warning( "SVG Load: Auto-shape for image %d ('%s') - could not find packed image, using fallback size", imageID, orphans[j].name.c_str() );
					pixelW = 128;
					pixelH = 64;
				} else {
					pixelW = packImage->trueSize.x;
					pixelH = packImage->trueSize.y;
				}

				// Use the SVG display size (width/height attributes on the <image> node)
				// for the shape bounds.  If none were specified, fall back to pixel size.
				const idSWFDictionaryEntry& imageEntry = dictionary[imageID];
				float						displayW   = ( imageEntry.svgDisplaySize.x > 0 ) ? imageEntry.svgDisplaySize.x : pixelW;
				float						displayH   = ( imageEntry.svgDisplaySize.y > 0 ) ? imageEntry.svgDisplaySize.y : pixelH;

				// The bitmap fill matrix must account for the scale from pixel size
				// to display size.  The base factor 20 converts SVG units to SWF twips.
				// When display size != pixel size we need to additionally scale the UVs
				// so the full texture maps onto the larger/smaller quad.
				fill.style.startMatrix.xx = 20.0f * ( displayW / pixelW );
				fill.style.startMatrix.yy = 20.0f * ( displayH / pixelH );
				fill.style.endMatrix	  = fill.style.startMatrix;

				shapeEntry.shape->startBounds = swfRect_t( 0, 0, displayW, displayH );
				shapeEntry.shape->endBounds	  = shapeEntry.shape->startBounds;

				float w = displayW;
				float h = displayH;

				fill.startVerts.SetNum( 4 );
				fill.startVerts[0].Set( w, h ); // bottom-right
				fill.startVerts[1].Set( 0, h ); // bottom-left
				fill.startVerts[2].Set( 0, 0 ); // top-left
				fill.startVerts[3].Set( w, 0 ); // top-right

				fill.indices.SetNum( 6 );
				fill.indices[0] = 0;
				fill.indices[1] = 1;
				fill.indices[2] = 2;
				fill.indices[3] = 0;
				fill.indices[4] = 2;
				fill.indices[5] = 3;

				// Remap: the original image name now resolves to the shape ID,
				// so <use xlink:href="#intro_placeholder"> finds the shape, not the image.
				svgNameToCharID.Set( orphans[j].name, shapeID );

				common->Printf( "SVG Load: Auto-generated bitmap shape %d for orphan image %d ('%s', %gx%g)\n", shapeID, imageID, orphans[j].name.c_str(), w, h );
			}
		}
	}

	// Mainsprite (after defs, or the last <g>)
	pugi::xml_node mainNode = defs.next_sibling( "g" );
	if( !mainNode ) {
		mainNode = svgNode.last_child(); // Fallback for unfolded
	}

	mainsprite = new idSWFSprite( this );
	mainsprite->LoadSVGNode_r( mainNode, dictionary, isUnfolded, svgTargetMap, svgAnimations );

	idList<idSWFSprite*> animatedSprites;
	if( svgAnimations.Num() > 0 ) {
		// Parse all collected animation nodes into parsedAnim_t entries stored on each target.
		idSWFSprite::ParseSVGAnimations( svgTargetMap, svgAnimations );

		// Group parsed animations by owner sprite, then apply each group once.
		// Using parallel lists to avoid pointer-keyed hash tables.
		idList<idSWFSprite*>					  uniqueOwners;
		idList<idList<idSWFSprite::parsedAnim_t>> ownerAnims;

		for( int i = 0; i < svgTargetMap.Num(); i++ ) {
			idSWFSprite::svgAnimTarget_t* target = svgTargetMap.GetIndex( i );
			if( target == NULL || target->owner == NULL || target->parsedAnims.Num() == 0 ) {
				continue;
			}

			int ownerIdx = uniqueOwners.FindIndex( target->owner );
			if( ownerIdx == -1 ) {
				ownerIdx = uniqueOwners.Num();
				uniqueOwners.Append( target->owner );
				ownerAnims.Alloc();
			}
			ownerAnims[ownerIdx].Append( target->parsedAnims );
		}

		for( int i = 0; i < uniqueOwners.Num(); i++ ) {
			uniqueOwners[i]->ApplySVGAnimationTargets( ownerAnims[i] );
			animatedSprites.AddUnique( uniqueOwners[i] );
		}
	}

	// Ensure deferred commands are materialized even when there are no animations.
	{
		idList<idSWFSprite*> allSprites;
		if( mainsprite != NULL ) {
			allSprites.Append( mainsprite );
		}
		for( int i = 0; i < dictionary.Num(); i++ ) {
			if( dictionary[i].type == SWF_DICT_SPRITE && dictionary[i].sprite != NULL ) {
				if( allSprites.FindIndex( dictionary[i].sprite ) == -1 ) {
					allSprites.Append( dictionary[i].sprite );
				}
			}
		}
		idList<idSWFSprite::parsedAnim_t> emptyAnims;
		for( int i = 0; i < allSprites.Num(); i++ ) {
			if( animatedSprites.FindIndex( allSprites[i] ) != -1 ) {
				continue;
			}
			allSprites[i]->ApplySVGAnimationTargets( emptyAnims );
		}
	}

	int timeEnd = Sys_Milliseconds();

	common->Printf( "Loading '%s' took %5.1f seconds\n\n", filename, ( timeEnd - timeStart ) * 0.001f );

	// Clean up sub-documents now that all animations have been processed
	for( int i = 0; i < svgSubDocuments.Num(); i++ ) {
		delete svgSubDocuments[i];
	}
	svgSubDocuments.Clear();

	// now that all images have been loaded, write out the combined image
	idStr atlasFileName = "generated/";
	atlasFileName += filename;
	atlasFileName.SetFileExtension( ".png" );

	WriteSwfImageAtlas( atlasFileName );

	return true;
}

/*
========================
idSWF::WriteSVGDefs

Writes the <defs>...</defs> section containing all dictionary entries
(IMAGE, SHAPE, MORPH, FONT, EDITTEXT) to the given file.
Sprites are skipped in unfolded mode.
========================
*/
void idSWF::WriteSVGDefs( idFile* file, const char* filenameWithoutExt, bool exportUnfolded, const char* imageHrefPrefix )
{
	const bool	exportBitmapShapesOnly = false;

	// For the main SVG the image href prefix is the same as filenameWithoutExt
	// (e.g. "shell" -> "shell/image_characterid_2.png").
	// For sub-SVGs that already live inside the "shell/" directory the prefix
	// should be empty so hrefs become just "image_characterid_2.png".
	const char* imgPrefix = ( imageHrefPrefix != NULL ) ? imageHrefPrefix : filenameWithoutExt;

	file->WriteFloatString( "<defs>\n" );
	for( int i = 0; i < dictionary.Num(); i++ ) {
		const idSWFDictionaryEntry& entry = dictionary[i];

		switch( dictionary[i].type ) {
			case SWF_DICT_IMAGE: {
				file->WriteFloatString( "\t<image id=\"%i\" ", i );
				if( imgPrefix[0] != '\0' ) {
					file->WriteFloatString( "href=\"%s/image_characterid_%i.png\"", imgPrefix, i );
				} else {
					file->WriteFloatString( "href=\"image_characterid_%i.png\"", i );
				}
				file->WriteFloatString( " width=\"%i\" height=\"%i\" />\n", entry.imageSize[0], entry.imageSize[1] );
				break;
			}

			case SWF_DICT_MORPH:
			case SWF_DICT_SHAPE: {
				idSWFShape* shape = dictionary[i].shape;

				file->WriteFloatString( "\t<g id=\"%i\" data-type=\"%s\">\n", i, idSWF::GetDictTypeName( dictionary[i].type ) );

				// export fill draws
				for( int d = 0; d < shape->fillDraws.Num(); d++ ) {
					idSWFShapeDrawFill& fillDraw = shape->fillDraws[d];

					if( fillDraw.style.type == 4 ) {
						int bitmapID = fillDraw.style.bitmapID;
						if( bitmapID == 65535 ) {
							continue;
						}

						const idSWFDictionaryEntry& bitmapEntry = dictionary[bitmapID];
						file->WriteFloatString( "\t\t<use href=\"#%i\" link-type=\"BITMAP\" ", bitmapID );

						idStr transform = "";

						file->WriteFloatString( "%s/>\n", transform.c_str() );
						continue;

					} else if( exportBitmapShapesOnly ) {
						continue;
					}

					idStr fillColor = "";
					if( fillDraw.style.type == 0 ) {
						// solid fill draw
						const swfColorRGBA_t& color = fillDraw.style.startColor;
						const char*			  fill	= cssNameFromRGBA( color );
						fillColor.Format( "fill=\"%s\"", fill );
					}

					for( int k = 0; k < fillDraw.indices.Num(); k += 3 ) {
						const uint16& i1 = fillDraw.indices[k + 0];
						const uint16& i2 = fillDraw.indices[k + 1];
						const uint16& i3 = fillDraw.indices[k + 2];

						const idVec2& v1 = fillDraw.startVerts[i1];
						const idVec2& v2 = fillDraw.startVerts[i2];
						const idVec2& v3 = fillDraw.startVerts[i3];

						file->WriteFloatString( "\t\t<polygon %s points=\"%f,%f %f,%f %f,%f\" />\n", fillColor.c_str(), v1.x, v1.y, v2.x, v2.y, v3.x, v3.y );
					}
				}

				// export line draws
				for( int d = 0; d < shape->lineDraws.Num(); d++ ) {
					const idSWFShapeDrawLine& lineDraw = shape->lineDraws[d];

					const swfColorRGBA_t&	  color = lineDraw.style.startColor;
					const char*				  fill	= cssNameFromRGBA( color );
					file->WriteFloatString( "\t\t<polyline fill=\"none\" stroke=\"%s\" stroke-width=\"%f\" points=\"", fill, lineDraw.style.startWidth );

					for( int v = 0; v < lineDraw.startVerts.Num(); v++ ) {
						const idVec2& vert = lineDraw.startVerts[v];
						file->WriteFloatString( "%f,%f ", vert.x, vert.y );
					}
					file->WriteFloatString( "\"/>\n" );
				}

				file->WriteFloatString( "\t</g>\n" );
				break;
			}

			case SWF_DICT_SPRITE: {
				if( !exportUnfolded ) {
					dictionary[i].sprite->WriteSVG( file, i, dictionary );
				}
				break;
			}

			case SWF_DICT_FONT: {
				const idSWFFont* font = dictionary[i].font;
				file->WriteFloatString( "\t<g id=\"%i\" data-type=\"FONT\" data-name=\"%s\">\n", i, font->fontID->GetName() );

				file->WriteFloatString( "\t\t<style>@font-face { font-family: '%s'; src: local('%s'), url('fonts/%s.ttf') format('truetype'); }</style>\n",
					font->fontID->GetName(),
					font->fontID->GetName(),
					font->fontID->GetName() );
				file->WriteFloatString( "\t</g>\n" );
				break;
			}

			case SWF_DICT_TEXT: {
				// RB: not used in BFG files
				break;
			}

			case SWF_DICT_EDITTEXT: {
				const idSWFEditText* et = dictionary[i].edittext;

				idStr				 initialText = et->initialText;

				// RB: ugly hack but necessary for exporting pda.json
				if( idStr::FindText( initialText, "bay-bee" ) > -1 ) {
					initialText = "\"It is DONE bay-bee!\"";
				} else if( idStr::FindText( initialText, "Email text goes in" ) > -1 ) {
					initialText = "\"Email text goes in here\"";
				}

				// notice ALIGN_JUSTIFY is not supported in SVG
				idStr  alignStr;
				idVec2 anchorPos;

				if( et->align == SWF_ET_ALIGN_CENTER ) {
					alignStr	= "middle";
					anchorPos.x = et->bounds.tl.x + et->bounds.br.x * 0.5f;
					anchorPos.y = et->bounds.tl.y + SWFTWIP( et->fontHeight );
				} else if( et->align == SWF_ET_ALIGN_RIGHT ) {
					alignStr	= "end";
					anchorPos.x = et->bounds.br.x;
					anchorPos.y = et->bounds.tl.y + SWFTWIP( et->fontHeight );
				} else {
					alignStr	= "start";
					anchorPos.x = et->bounds.tl.x;
					anchorPos.y = et->bounds.tl.y + SWFTWIP( et->fontHeight );
				}

				const swfColorRGBA_t& color	   = et->color;
				float				  fontSize = SWFTWIP( et->fontHeight ); // SWF font height is in twips

				// data-attribs for full Flash data
				idStr				  dataBounds;
				dataBounds.Format( "data-bounds=\"[%f,%f,%f,%f]\"", et->bounds.tl.x, et->bounds.tl.y, et->bounds.br.x, et->bounds.br.y );

				idStr dataFlags		= va( "data-flags=\"%i\"", et->flags );
				idStr dataLeading	= va( "data-leading=\"%i\"", et->leading );
				idStr dataMaxLength = va( "data-maxlength=\"%i\"", et->maxLength );
				idStr dataVariable	= va( "data-variable=\"%s\"", et->variable.c_str() );
				idStr dataMargins	= va( "data-margins=\"[%i,%i,%i]\"", et->leftMargin, et->rightMargin, et->indent );

				if( et->flags & SWF_ET_MULTILINE ) {
					// split initialText into multiple lines
					idStrList lines;
					if( !initialText.Split( lines, '\n', '\'' ) ) {
						initialText.Split( lines, '\n', '\0' );
					}

					if( lines.Num() > 1 ) {
						idStr multiLineText = "\n";
						for( int l = 0; l < lines.Num(); l++ ) {
							multiLineText += "\t\t<tspan x=\"";
							multiLineText += anchorPos.x;
							multiLineText += "\" dy=\"";
							multiLineText += fontSize * 1.2f; // line height
							multiLineText += "\">";

							idStr line = lines[l];
							multiLineText += line;
							multiLineText += "</tspan>\n";
						}
						initialText = multiLineText;
					}
				}

				file->WriteFloatString( "\t<text id=\"%i\" x=\"%f\" y=\"%f\" font-family=\"%s\" font-size=\"%f\" fill=\"rgba(%d, %d, %d, %f)\" text-anchor=\"%s\" %s %s %s %s %s>%s</text>\n",
					i,
					anchorPos.x,
					anchorPos.y,
					GetFontName( et->fontID ),
					fontSize,
					( int )( color.r ),
					( int )( color.g ),
					( int )( color.b ),
					color.a * ( 1.0f / 255.0f ),
					alignStr.c_str(),
					dataBounds.c_str(),
					dataFlags.c_str(),
					dataLeading.c_str(),
					dataMaxLength.c_str(),
					dataMargins.c_str(),
					initialText.c_str() );
				break;
			}
		}
	}

	file->WriteFloatString( "</defs>\n" );
}

void idSWF::WriteSVG( const char* filename, bool noAnims, bool splitSVG )
{
	idFileLocal file( fileSystem->OpenFileWrite( filename, "fs_basepath" ) );
	if( file == NULL ) {
		return;
	}

	idStr filenameWithoutExt = filename;
	filenameWithoutExt.StripFileExtension();
	filenameWithoutExt.StripLeadingOnce( "exported/swf/" );

	// missing timestamp, frameRate
	file->WriteFloatString( "<svg\n"
							"\txmlns=\"http://www.w3.org/2000/svg\"\n"
							"\twidth=\"%i\"\n"
							"\theight=\"%i\"\n"
							"\tdata-exported-from=\"%s\"\n>\n",
		( int )frameWidth,
		( int )frameHeight,
		ENGINE_VERSION );

	const bool exportUnfolded = true;

	WriteSVGDefs( file, filenameWithoutExt.c_str(), exportUnfolded );

	if( exportUnfolded ) {
		int									 characterID = dictionary.Num();
		idHashTableT<int, svgDisplayEntry_t> characterMap;
		float								 frameRate = ( ( float )this->frameRate / 256.0f ); // most likely 60 fps
		float								 frameDur  = 1.0f / frameRate;

		// Build split context
		svgSplitContext_t					 splitCtx;
		splitCtx.enabled  = splitSVG;
		splitCtx.basePath = filename;					  // e.g. "exported/swf/shell.svg"
		splitCtx.basePath.StripFileExtension();			  // -> "exported/swf/shell"
		splitCtx.filenameWithoutExt = filenameWithoutExt; // e.g. "shell"
		splitCtx.splitDepth			= 1;
		splitCtx.swf				= this;
		splitCtx.noAnims			= noAnims;

		mainsprite->WriteSVGUnfolded_r( file, characterID, dictionary, characterMap, frameDur, "", 0, true, noAnims, &splitCtx );
	} else {
		mainsprite->WriteSVG( file, dictionary.Num(), dictionary );
	}

	file->WriteFloatString( "</svg>\n" );
}

/*
========================
idSWF::LoadSVGSub

Loads a sub-SVG file exported by the split exporter.
Parses its <defs>, skipping entries that already exist in the dictionary.
Parses its body <g> as a new sprite and adds it to the dictionary.
Returns the character ID of the new sprite, or -1 on failure.
========================
*/
int idSWF::LoadSVGSub(
	const char* subFilename, idList<idSWFDictionaryEntry>& dict, bool isUnfolded, idHashTableT<idStr, idSWFSprite::svgAnimTarget_t>& svgTargetMap, idList<pugi::xml_node>& svgAnimations )
{
	// Resolve the path relative to the main SVG's location
	idStr	fullPath = svgBaseDir + subFilename;

	idFile* f = fileSystem->OpenFileReadMemory( fullPath );
	if( f == NULL || f->Length() <= 0 ) {
		idLib::Warning( "SVG Split Import: Could not open sub-file '%s'", fullPath.c_str() );
		delete f;
		return -1;
	}

	int	   fileLength = f->Length();
	char*  fileData	  = ( char* )Mem_Alloc( fileLength, TAG_SWF );
	size_t fileSize	  = f->Read( ( byte* )fileData, fileLength );
	delete f;
	f = NULL;

	// Allocate a new pugi::xml_document on the heap and keep it alive
	// until the main LoadSVG finishes (animation nodes reference it).
	pugi::xml_document*	   subDoc = new pugi::xml_document();
	pugi::xml_parse_result result = subDoc->load_buffer( fileData, fileSize );

	// load_buffer copies the data internally, so we can free our buffer now.
	Mem_Free( fileData );
	fileData = NULL;

	if( !result ) {
		idLib::Warning( "SVG Split Import: Parse error in '%s': %s", fullPath.c_str(), result.description() );
		delete subDoc;
		return -1;
	}

	// Store the document so it stays alive until the main LoadSVG finishes;
	// animation nodes collected during LoadSVGNode_r reference this XML tree.
	svgSubDocuments.Append( subDoc );

	pugi::xml_node svgNode = subDoc->child( "svg" );
	if( !svgNode ) {
		idLib::Warning( "SVG Split Import: No <svg> root in '%s'", fullPath.c_str() );
		return -1;
	}

	// Parse <defs>, skipping entries whose numeric ID already exists in the dictionary
	pugi::xml_node defs = svgNode.child( "defs" );
	if( defs ) {
		for( pugi::xml_node child = defs.first_child(); child; child = child.next_sibling() ) {
			pugi::xml_attribute idAttr = child.attribute( "id" );
			if( !idAttr ) {
				continue;
			}

			idStr idStr_val = idAttr.value();
			if( !idStr::IsNumeric( idStr_val ) ) {
				continue; // sub-SVGs from engine export always have numeric IDs
			}

			int id = atoi( idStr_val.c_str() );

			// Grow dictionary if needed (defensive)
			if( id >= dict.Num() ) {
				dict.SetNum( id + 1 );
			}

			// Skip if already loaded from the main SVG
			if( dict[id].type != SWF_DICT_NULL ) {
				continue;
			}

			// Parse this defs entry (same logic as LoadSVG Pass 2)
			const char* tagName = child.name();

			if( idStr::Icmp( tagName, "image" ) == 0 ) {
				ParseSVG_Image( child, id, dict[id] );
			} else if( idStr::Icmp( tagName, "text" ) == 0 ) {
				dict[id].type	  = SWF_DICT_EDITTEXT;
				dict[id].edittext = new( TAG_SWF ) idSWFEditText;
				ParseSVG_Text( child, dict[id].edittext );
			} else if( idStr::Icmp( tagName, "g" ) == 0 ) {
				const char* dataType = child.attribute( "data-type" ).value();

				if( dataType && idStr::Icmp( dataType, "SHAPE" ) == 0 ) {
					dict[id].type  = SWF_DICT_SHAPE;
					dict[id].shape = new( TAG_SWF ) idSWFShape;
					ParseSVG_Shape( child, dict[id].shape );
				} else if( dataType && idStr::Icmp( dataType, "FONT" ) == 0 ) {
					dict[id].type = SWF_DICT_FONT;
					dict[id].font = new( TAG_SWF ) idSWFFont;
					ParseSVG_Font( child, dict[id].font );
				}
				// Sprites in defs are skipped in unfolded mode (they're inlined)
			}
		}
	}

	// Find the body <g> (the sprite content) – it's the first <g> after <defs>
	pugi::xml_node bodyNode;
	if( defs ) {
		bodyNode = defs.next_sibling( "g" );
	}
	if( !bodyNode ) {
		bodyNode = svgNode.last_child();
	}
	if( !bodyNode ) {
		idLib::Warning( "SVG Split Import: No body <g> found in '%s'", fullPath.c_str() );
		return -1;
	}

	// Create a new sprite in the dictionary
	int					  newCharID = dict.Num();
	idSWFDictionaryEntry& newEntry	= dict.Alloc();
	newEntry.type					= SWF_DICT_SPRITE;
	newEntry.sprite					= new idSWFSprite( this );
	newEntry.sprite->LoadSVGNode_r( bodyNode, dict, isUnfolded, svgTargetMap, svgAnimations );

	common->Printf( "SVG Split Import: loaded sub-sprite '%s' as character %d\n", subFilename, newCharID );

	return newCharID;
}
