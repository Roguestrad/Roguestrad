/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2025 Robert Beckebans

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

// Parse Shape from <g> Node
static void ParseShape( const pugi::xml_node& node, idSWFShape* shape )
{
	if( !shape )
		return;

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

		for( int idx = 0; idx < ldraw.startVerts.Num(); idx++ ) {
			ldraw.indices.Append( idx );
		}

		shape->lineDraws.Append( ldraw );
	}

	shape->startBounds.tl = vec2_zero;
	shape->startBounds.br = idVec2( 100, 100 );
	shape->endBounds	  = shape->startBounds;
}

static void ParseText( const pugi::xml_node& node, idSWFEditText* et )
{
	if( !et )
		return;

	pugi::xml_node textNode = node.child( "text" );
	if( textNode ) {
		et->bounds.tl.x = textNode.attribute( "x" ).as_float();
		et->bounds.br.y = textNode.attribute( "y" ).as_float();
		et->fontHeight	= FLOAT2SWFTWIP( textNode.attribute( "font-size" ).as_float() );
		et->color.ParseSVGColorFromString( textNode.attribute( "fill" ).value() );
		et->align		= ( textNode.attribute( "text-anchor" ).value() == "start" ) ? SWF_ET_ALIGN_LEFT : SWF_ET_ALIGN_CENTER;
		et->initialText = textNode.text().as_string();
		et->fontID		= 0;
	}
}

bool idSWF::LoadSVG( const char* filename )
{
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

	frameWidth	= svgNode.attribute( "width" ).as_float();
	frameHeight = svgNode.attribute( "height" ).as_float();
	frameRate	= 60 * 256;

	bool		   isUnfolded = true;

	pugi::xml_node defs = svgNode.child( "defs" );
	if( defs ) {
		int maxID = 0;
		for( pugi::xml_node g = defs.child( "g" ); g; g = g.next_sibling( "g" ) ) {
			int id = g.attribute( "id" ).as_int();
			maxID  = idMath::Imax( maxID, id );
		}
		dictionary.SetNum( maxID + 1 );

		for( pugi::xml_node g = defs.child( "g" ); g; g = g.next_sibling( "g" ) ) {
			int					  id	= g.attribute( "id" ).as_int();
			idSWFDictionaryEntry& entry = dictionary[id];

			if( g.child( "polygon" ) || g.child( "polyline" ) ) {
				entry.type	= SWF_DICT_SHAPE;
				entry.shape = new( TAG_SWF ) idSWFShape;
				ParseShape( g, entry.shape );
			} else if( g.child( "text" ) ) {
				entry.type	   = SWF_DICT_EDITTEXT;
				entry.edittext = new( TAG_SWF ) idSWFEditText;
				ParseText( g, entry.edittext );
			} else if( g.child( "image" ) ) {
				entry.type = SWF_DICT_IMAGE;
				// TODO: Load image from xlink:href and set imageSize etc.
				idStr imagePath = g.child( "image" ).attribute( "xlink:href" ).value();
				// call LoadImage(id, ...) auf, similar as in LoadJSON
			} else {
				entry.type	 = SWF_DICT_SPRITE;
				entry.sprite = new idSWFSprite( this );
				entry.sprite->LoadSVGNode( g, dictionary, isUnfolded );
			}
		}
	}

	// Mainsprite (after defs, or the last <g>)
	pugi::xml_node mainNode = defs.next_sibling( "g" );
	if( !mainNode ) {
		mainNode = svgNode.last_child(); // Fallback for unfolded
	}

	mainsprite = new idSWFSprite( this );
	mainsprite->LoadSVGNode( mainNode, dictionary, isUnfolded );

	return true;
}

void idSWF::WriteSVG( const char* filename )
{
	const bool	exportBitmapShapesOnly = false;

	idFileLocal file( fileSystem->OpenFileWrite( filename, "fs_basepath" ) );
	if( file == NULL ) {
		return;
	}

	idStr filenameWithoutExt = filename;
	filenameWithoutExt.StripFileExtension();
	filenameWithoutExt.StripLeadingOnce( "exported/swf/" );

	// missing timestamp, frameRate
	// \tviewBox=\"0 0 600 300\"\n
	file->WriteFloatString( "<svg\n"
							"\txmlns=\"http://www.w3.org/2000/svg\"\n"
							"\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
							"\twidth=\"%i\"\n"
							"\theight=\"%i\"\n >\n",
		( int )frameWidth,
		( int )frameHeight );

	const bool exportUnfolded = true;

	file->WriteFloatString( "\t<defs>\n" );
	for( int i = 0; i < dictionary.Num(); i++ ) {
		const idSWFDictionaryEntry& entry = dictionary[i];

		switch( dictionary[i].type ) {
				/*
				case SWF_DICT_IMAGE:
				{
					file->WriteFloatString( "\t\t<image id=\"%i\" ", i );
					file->WriteFloatString( "xlink:href=\"%s/image_characterid_%i.png\"", filenameWithoutExt.c_str(), i );
					file->WriteFloatString( " width=\"%i\" height=\"%i\" />\n", entry.imageSize[0], entry.imageSize[1] );
					break;
				}
				*/

			case SWF_DICT_MORPH:
			case SWF_DICT_SHAPE: {
				idSWFShape* shape = dictionary[i].shape;

				// file->WriteFloatString( "\t\t<g id=\"%i\" visibility=\"hidden\">\n", i );
				file->WriteFloatString( "\t\t<g id=\"%i\" >\n", i );

				// export fill draws
				for( int d = 0; d < shape->fillDraws.Num(); d++ ) {
					idSWFShapeDrawFill& fillDraw = shape->fillDraws[d];

					if( fillDraw.style.type == 4 ) {
						int bitmapID = fillDraw.style.bitmapID;
						if( bitmapID == 65535 ) {
							continue;
						}

						const idSWFDictionaryEntry& bitmapEntry = dictionary[bitmapID];

						file->WriteFloatString( "\t\t\t<image id=\"%i\" ", bitmapID );
						file->WriteFloatString( "xlink:href=\"%s/image_characterid_%i.png\"", filenameWithoutExt.c_str(), bitmapID );
						file->WriteFloatString( " width=\"%i\" height=\"%i\" />\n", bitmapEntry.imageSize[0], bitmapEntry.imageSize[1] );

						continue;
					} else if( exportBitmapShapesOnly ) {
						continue;
					}

					// TODO sub types
					// 0 = linear, 2 = radial, 3 = focal; 0 = repeat, 1 = clamp, 2 = near repeat, 3 = near clamp
					/*
					file->WriteFloatString( " subType=" );
					if( fillDraw.style.subType == 0 )
					{
						file->WriteFloatString( "\"linear\"" );
					}
					else if( fillDraw.style.subType == 1 )
					{
						file->WriteFloatString( "\"radial\"" );
					}
					else if( fillDraw.style.subType == 2 )
					{
						file->WriteFloatString( "\"focal\"" );
					}
					else if( fillDraw.style.subType == 3 )
					{
						file->WriteFloatString( "\"near clamp\"" );
					}
					else
					{
						file->WriteFloatString( "\"%i\"", fillDraw.style.subType );
					}
					*/

					/*
					unused in BFG
					if( fillDraw.style.type == 1 && fillDraw.style.subType == 3 )
					{
						file->WriteFloatString( " focalPoint=\"%f\"", fillDraw.style.focalPoint );
					}
					*/

					idStr fillColor = "";
					if( fillDraw.style.type == 0 ) {
						// solid fill draw
						const swfColorRGBA_t& color = fillDraw.style.startColor;
						fillColor.Format( "fill=\"rgba(%d, %d, %d, %f)\"", ( int )( color.r ), ( int )( color.g ), ( int )( color.b ), color.a * ( 1.0f / 255.0f ) );
					}

					/*
					if( fillDraw.style.type > 0 )
					{
						swfMatrix_t m = fillDraw.style.startMatrix;
						file->WriteFloatString( "\t\t\t\t\t<StartMatrix>%f %f %f %f %f %f</StartMatrix>\n",
												m.xx, m.yy, m.xy, m.yx, m.tx, m.ty );

						if( fillDraw.style.startMatrix != fillDraw.style.endMatrix )
						{
							m = fillDraw.style.endMatrix;
							file->WriteFloatString( "\t\t\t\t\t<EndMatrix>%f %f %f %f %f %f</EndMatrix>\n",
													m.xx, m.yy, m.xy, m.yx, m.tx, m.ty );
						}
					}
					*/

					for( int k = 0; k < fillDraw.indices.Num(); k += 3 ) {
						const uint16& i1 = fillDraw.indices[k + 0];
						const uint16& i2 = fillDraw.indices[k + 1];
						const uint16& i3 = fillDraw.indices[k + 2];

						const idVec2& v1 = fillDraw.startVerts[i1];
						const idVec2& v2 = fillDraw.startVerts[i2];
						const idVec2& v3 = fillDraw.startVerts[i3];

						file->WriteFloatString( "\t\t\t<polygon %s points=\"%f,%f %f,%f %f,%f\" />\n", fillColor.c_str(), v1.x, v1.y, v2.x, v2.y, v3.x, v3.y );
					}
				}

				// export line draws
				for( int d = 0; d < shape->lineDraws.Num(); d++ ) {
					const idSWFShapeDrawLine& lineDraw = shape->lineDraws[d];

					const swfColorRGBA_t&	  color = lineDraw.style.startColor;
					file->WriteFloatString( "\t\t\t<polyline fill=\"none\" stroke=\"rgba(%d, %d, %d, %f)\" stroke-width=\"%f\" points=\"",
						( int )( color.r ),
						( int )( color.g ),
						( int )( color.b ),
						color.a * ( 1.0f / 255.0f ),
						lineDraw.style.startWidth );

					for( int v = 0; v < lineDraw.startVerts.Num(); v++ ) {
						const idVec2& vert = lineDraw.startVerts[v];
						file->WriteFloatString( "%f,%f ", vert.x, vert.y );
					}
					file->WriteFloatString( "\"/>\n" );
				}

				file->WriteFloatString( "\t\t</g>\n" );
				break;
			}

			case SWF_DICT_SPRITE: {
				if( !exportUnfolded ) {
					dictionary[i].sprite->WriteSVG( file, i, dictionary );
				}
				break;
			}

			case SWF_DICT_FONT: {
#if 0
				file->WriteFloatString( "\t\t<g id=\"%i\" >\n", i );
				const idSWFFont* font = dictionary[i].font;
				file->WriteFloatString(
					"\t\t\t<style>@font-face { font-family: '%s'; src: local('%s'), url('fonts/%s.ttf') format('truetype'); }</style>\n",
					font->fontID->GetName(), font->fontID->GetName(), font->fontID->GetName()
				);
				file->WriteFloatString( "\t\t</g>\n" );
#endif
				break;
			}

			case SWF_DICT_TEXT: {
				// RB: not used in BFG files
				break;
			}

			case SWF_DICT_EDITTEXT: {
				const idSWFEditText* et = dictionary[i].edittext;

				idStr				 initialText = idStr::CStyleQuote( et->initialText.c_str() );

				// RB: ugly hack but necessary for exporting pda.json
				// if( initialText.Cmp( "\"It\\'s DONE bay-bee!\"") == 0 )
				if( idStr::FindText( initialText, "bay-bee" ) > -1 ) {
					initialText = "\"It is DONE bay-bee!\"";
				} else if( idStr::FindText( initialText, "Email text goes in" ) > -1 ) {
					initialText = "\"Email text goes in here\"";
				}

				// notice ALIGN_JUSTIFY is not supported in SVG
				idStr				  alignStr = ( et->align == SWF_ET_ALIGN_LEFT ) ? "start" : ( et->align == SWF_ET_ALIGN_CENTER ) ? "middle" : ( et->align == SWF_ET_ALIGN_RIGHT ) ? "end" : "start";

				const swfColorRGBA_t& color	   = et->color;
				float				  fontSize = SWFTWIP( et->fontHeight ); // SWF font height is in twips

				file->WriteFloatString( "\t\t<g id=\"%i\" >\n", i );
				file->WriteFloatString( "\t\t\t<text x=\"%f\" y=\"%f\" font-family=\"%s\" font-size=\"%f\" fill=\"rgba(%d, %d, %d, %f)\" text-anchor=\"%s\">%s</text>\n",
					et->bounds.tl.x,
					et->bounds.br.y,
					GetFontName( et->fontID ),
					fontSize,
					( int )( color.r ),
					( int )( color.g ),
					( int )( color.b ),
					color.a * ( 1.0f / 255.0f ),
					alignStr.c_str(),
					et->initialText.c_str() );
				file->WriteFloatString( "\t\t</g>\n" );
				break;
			}
		}
	}

	file->WriteFloatString( "\t</defs>\n" );

	if( exportUnfolded ) {
		int									 characterID = dictionary.Num();
		swfMatrix_t							 identityMatrix;
		swfColorXform_t						 identityColor;
		idHashTableT<int, svgDisplayEntry_t> characterMap;
		float								 frameRate = ( ( float )this->frameRate / 256.0f ); // most likely 60 fps
		float								 frameDur  = 1.0f / frameRate;
		mainsprite->WriteSVGUnfolded_r( file, characterID, dictionary, identityMatrix, identityColor, characterMap, frameDur, 2 );
	} else {
		mainsprite->WriteSVG( file, dictionary.Num(), dictionary );
	}

	file->WriteFloatString( "</svg>\n" );
}
