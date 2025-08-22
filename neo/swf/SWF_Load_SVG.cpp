/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2013-2024 Robert Beckebans

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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#pragma hdrstop


/*
===================
idSWF::WriteSVG
===================
*/
void idSWF::WriteSVG( const char* filename )
{
	const bool exportBitmapShapesOnly = false;

	idFileLocal file( fileSystem->OpenFileWrite( filename, "fs_basepath" ) );
	if( file == NULL )
	{
		return;
	}

	idStr filenameWithoutExt = filename;
	filenameWithoutExt.StripFileExtension();
	filenameWithoutExt.StripLeadingOnce( "exported/swf/" );

	// missing timestamp, frameRate
	// \tviewBox=\"0 0 600 300\"\n
	file->WriteFloatString(
		"<svg\n"
		"\txmlns=\"http://www.w3.org/2000/svg\"\n"
		"\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
		"\twidth=\"%i\"\n"
		"\theight=\"%i\"\n >\n", ( int ) frameWidth, ( int ) frameHeight );

	file->WriteFloatString( "\t<defs>\n" );
	for( int i = 0; i < dictionary.Num(); i++ )
	{
		const idSWFDictionaryEntry& entry = dictionary[i];

		//file->WriteFloatString( "\t<DictionaryEntry type=\"%s\">\n", idSWF::GetDictTypeName( dictionary[i].type ) );
		switch( dictionary[i].type )
		{
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
			case SWF_DICT_SHAPE:
			{
				idSWFShape* shape = dictionary[i].shape;

				//file->WriteFloatString( "\t\t<g id=\"%i\" visibility=\"hidden\">\n", i );
				file->WriteFloatString( "\t\t<g id=\"%i\" >\n", i );

				float x = shape->startBounds.tl.y;
				float y = shape->startBounds.tl.x;
				float width = fabs( shape->startBounds.br.y - shape->startBounds.tl.y );
				float height = fabs( shape->startBounds.br.x - shape->startBounds.tl.x );

				//file->WriteFloatString( "\t\t\t<StartBounds x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />\n", x, y, width, height );

				x = shape->endBounds.tl.y;
				y = shape->endBounds.tl.x;
				width = fabs( shape->endBounds.br.y - shape->endBounds.tl.y );
				height = fabs( shape->endBounds.br.x - shape->endBounds.tl.x );

				//file->WriteFloatString( "\t\t\t<EndBounds x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />\n", x, y, width, height );

				// export fill draws
				for( int d = 0; d < shape->fillDraws.Num(); d++ )
				{
					idSWFShapeDrawFill& fillDraw = shape->fillDraws[d];

					if( fillDraw.style.type == 4 )
					{
						int bitmapID = fillDraw.style.bitmapID;
						if( bitmapID == 65535 )
						{
							continue;
						}

						const idSWFDictionaryEntry& bitmapEntry = dictionary[bitmapID];

						file->WriteFloatString( "\t\t\t<image id=\"%i\" ", bitmapID );
						file->WriteFloatString( "xlink:href=\"%s/image_characterid_%i.png\"", filenameWithoutExt.c_str(), bitmapID );
						file->WriteFloatString( " width=\"%i\" height=\"%i\" />\n", bitmapEntry.imageSize[0], bitmapEntry.imageSize[1] );

						continue;
					}
					else if( exportBitmapShapesOnly )
					{
						continue;
					}

					//<!-- Example of the same polygon shape with stroke and no fill -->
					// <polygon points="100,100 150,25 150,75 200,0" fill="none" stroke="black" />
					file->WriteFloatString( "\t\t\t<polygon " );


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

					if( fillDraw.style.type == 0 )
					{
						// solid fill draw
						const swfColorRGBA_t& color = fillDraw.style.startColor;
						file->WriteFloatString( " fill=\"rgba(%i,%i,%i,%i)\" ", ( int )( color.r ), ( int )( color.g ), ( int )( color.b ), ( int )( color.a ) );
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

					file->WriteFloatString( "points=\"" );
					for( int k = 0; k < fillDraw.indices.Num(); k += 3 )
					{
						const uint16& i1 = fillDraw.indices[k + 0];
						const uint16& i2 = fillDraw.indices[k + 1];
						const uint16& i3 = fillDraw.indices[k + 2];

						const idVec2& v1 = fillDraw.startVerts[i1];
						const idVec2& v2 = fillDraw.startVerts[i2];
						const idVec2& v3 = fillDraw.startVerts[i3];

						file->WriteFloatString( "%f,%f ", v1.x, v1.y );
						file->WriteFloatString( "%f,%f ", v2.x, v2.y );
						file->WriteFloatString( "%f,%f ", v3.x, v3.y );

						// close the triangle
						file->WriteFloatString( "%f,%f ", v1.x, v1.y );
					}
					file->WriteFloatString( "\"" );

					file->WriteFloatString( "/>\n" );
				}

				// export line draws
				for( int d = 0; d < shape->lineDraws.Num(); d++ )
				{
					const idSWFShapeDrawLine& lineDraw = shape->lineDraws[d];

					const swfColorRGBA_t& color = lineDraw.style.startColor;
					file->WriteFloatString(
						"\t\t\t<polyline fill=\"none\" stroke=\"rgba(%d, %d, %d, %f)\" stroke-width=\"%f\" points=\"",
						( int )( color.r ), ( int )( color.g ), ( int )( color.b ), color.a, lineDraw.style.startWidth
					);

					for( int v = 0; v < lineDraw.startVerts.Num(); v++ )
					{
						const idVec2& vert = lineDraw.startVerts[v];
						file->WriteFloatString( "%f,%f ", vert.x, vert.y );
					}
					file->WriteFloatString( "\"/>\n" );
				}

				file->WriteFloatString( "\t\t</g>\n" );
				break;
			}

			case SWF_DICT_SPRITE:
			{
				dictionary[i].sprite->WriteSVG( file, i, dictionary );
				break;
			}

			case SWF_DICT_FONT:
			{
				// TODO

				//const idSWFFont* font = dictionary[i].font;

				//file->WriteFloatString( "\t\t<Font characterID=\"%i\" name=\"%s\" ascent=\"%i\" descent=\"%i\" leading=\"%i\" glyphsNum=\"%i\">\n",
				//						i, font->fontID->GetName(), font->ascent, font->descent, font->leading, font->glyphs.Num() );

#if 0
				for( int g = 0; g < font->glyphs.Num(); g++ )
				{
					file->WriteFloatString( "\t\t\t<Glyph code=\"%i\" advance=\"%i\"/>\n", font->glyphs[g].code, font->glyphs[g].advance );

#if 0
					for( int v = 0; v < font->glyphs[g].verts.Num(); v++ )
					{
						const idVec2& vert = font->glyphs[g].verts[v];

						file->WriteFloatString( "\t\t\t\t<Vertex x=\"%f\" y=\"%f\"/>\n", vert.x, vert.y );
					}

					file->WriteFloatString( "\t\t\t\t<Indices num=\"%i\">", font->glyphs[g].indices.Num() );
					for( int v = 0; v < font->glyphs[g].indices.Num(); v++ )
					{
						const uint16& vert = font->glyphs[g].indices[v];

						file->WriteFloatString( "%i ", vert );
					}
					file->WriteFloatString( "</Indices>\n" );

					file->WriteFloatString( "\t\t\t</Glyph>\n" );
#endif
				}
#endif
				//file->WriteFloatString( "\t\t</Font>\n" );
				break;
			}

			case SWF_DICT_TEXT:
			{
				// RB: not used in BFG files

				const idSWFText* text = dictionary[i].text;

				file->WriteFloatString( "\t\t<Text characterID=\"%i\">\n", i );

				float x = text->bounds.tl.y;
				float y = text->bounds.tl.x;
				float width = fabs( text->bounds.br.y - text->bounds.tl.y );
				float height = fabs( text->bounds.br.x - text->bounds.tl.x );

				file->WriteFloatString( "\t\t\t<Bounds x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />\n", x, y, width, height );

				//file->WriteBig( text->bounds.tl );
				//file->WriteBig( text->bounds.br );

				//file->WriteBigArray( ( float* )&text->matrix, 6 );

				swfMatrix_t m = text->matrix;
				file->WriteFloatString( "\t\t\t<Matrix>%f %f %f %f %f %f</Matrix>\n",
										m.xx, m.yy, m.xy, m.yx, m.tx, m.ty );

				//file->WriteBig( text->textRecords.Num() );
				for( int t = 0; t < text->textRecords.Num(); t++ )
				{
					const idSWFTextRecord& textRecord = text->textRecords[t];

					file->WriteFloatString( "\t\t\t\t<Record fontID=\"%i\" xOffet=\"%i\" yOffset=\"%i\" textHeight=\"%f\" firstGlyph=\"%i\" numGlyphs=\"%i\">\n",
											textRecord.fontID, textRecord.xOffset, textRecord.yOffset, textRecord.textHeight, textRecord.firstGlyph, textRecord.numGlyphs );

					idVec4 color = textRecord.color.ToVec4();
					file->WriteFloatString( "\t\t\t\t\t<Color r=\"%f\" g=\"%f\" b=\"%f\" a=\"%f\"/>\n",
											color.x, color.y, color.z, color.w );

					file->WriteFloatString( "\t\t\t\t</Record>\n" );

					/*file->WriteBig( textRecord.fontID );
					file->Write( &textRecord.color, 4 );
					file->WriteBig( textRecord.xOffset );
					file->WriteBig( textRecord.yOffset );
					file->WriteBig( textRecord.textHeight );
					file->WriteBig( textRecord.firstGlyph );
					file->WriteBig( textRecord.numGlyphs );*/
				}

				for( int g = 0; g < text->glyphs.Num(); g++ )
				{
					file->WriteFloatString( "\t\t\t\t<Glyph index=\"%i\" advance=\"%i\">\n", text->glyphs[g].index, text->glyphs[g].advance );
				}

				/*
				file->WriteBig( text->glyphs.Num() );
				for( int g = 0; g < text->glyphs.Num(); g++ )
				{
					file->WriteBig( text->glyphs[g].index );
					file->WriteBig( text->glyphs[g].advance );
				}
				*/

				file->WriteFloatString( "\t\t</Text>\n" );
				break;
			}

			case SWF_DICT_EDITTEXT:
			{
#if 0
				const idSWFEditText* et = dictionary[i].edittext;

				idStr initialText = idStr::CStyleQuote( et->initialText.c_str() );

				// RB: ugly hack but necessary for exporting pda.json
				//if( initialText.Cmp( "\"It\\'s DONE bay-bee!\"") == 0 )
				if( idStr::FindText( initialText, "bay-bee" ) > -1 )
				{
					initialText = "\"It is DONE bay-bee!\"";
				}
				else if( idStr::FindText( initialText, "Email text goes in" ) > -1 )
				{
					initialText = "\"Email text goes in here\"";
				}

				file->WriteFloatString( "\t\t<EditText characterID=\"%i\" flags=\"%i\" fontID=\"%i\" fontHeight=\"%i\" maxLength=\"%i\" align=\"%s\" leftMargin=\"%i\" rightMargin=\"%i\" indent=\"%i\" leading=\"%i\" variable=\"%s\" initialText=\"%s\">\n",
										i,
										et->flags, et->fontID, et->fontHeight, et->maxLength, idSWF::GetEditTextAlignName( et->align ),
										et->leftMargin, et->rightMargin, et->indent, et->leading,
										et->variable.c_str(), et->initialText.c_str() );

				float x = et->bounds.tl.y;
				float y = et->bounds.tl.x;
				float width = fabs( et->bounds.br.y - et->bounds.tl.y );
				float height = fabs( et->bounds.br.x - et->bounds.tl.x );

				file->WriteFloatString( "\t\t\t<Bounds x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />\n", x, y, width, height );

				idVec4 color = et->color.ToVec4();
				file->WriteFloatString( "\t\t\t<Color r=\"%f\" g=\"%f\" b=\"%f\" a=\"%f\"/>\n",
										color.x, color.y, color.z, color.w );

				file->WriteFloatString( "\t\t</EditText>\n" );

				//file->WriteBig( et->bounds.tl );
				//file->WriteBig( et->bounds.br );
				//file->WriteBig( et->flags );
				//file->WriteBig( et->fontID );
				//file->WriteBig( et->fontHeight );
				//file->Write( &et->color, 4 );
				//file->WriteBig( et->maxLength );
				//file->WriteBig( et->align );
				//file->WriteBig( et->leftMargin );
				//file->WriteBig( et->rightMargin );
				//file->WriteBig( et->indent );
				//file->WriteBig( et->leading );
				//file->WriteString( et->variable );
				//file->WriteString( et->initialText );
#endif
				break;
			}
		}

		//file->WriteFloatString( "\t</DictionaryEntry>\n" );
	}

	file->WriteFloatString( "\t</defs>\n" );

	mainsprite->WriteSVG( file, dictionary.Num(), dictionary );

	file->WriteFloatString( "</svg>\n" );
}
