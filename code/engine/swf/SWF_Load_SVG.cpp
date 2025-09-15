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
						file->WriteFloatString( "fill=\"rgba(%d, %d, %d, %f)\" ", ( int )( color.r ), ( int )( color.g ), ( int )( color.b ), color.a * ( 1.0f / 255.0f ) );
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
						( int )( color.r ), ( int )( color.g ), ( int )( color.b ), color.a * ( 1.0f / 255.0f ), lineDraw.style.startWidth
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

			case SWF_DICT_TEXT:
			{
				// RB: not used in BFG files
				break;
			}

			case SWF_DICT_EDITTEXT:
			{
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

				// notice ALIGN_JUSTIFY is not supported in SVG
				idStr alignStr = ( et->align == SWF_ET_ALIGN_LEFT ) ? "start" :
								 ( et->align == SWF_ET_ALIGN_CENTER ) ? "middle" :
								 ( et->align == SWF_ET_ALIGN_RIGHT ) ? "end" : "start";

				const swfColorRGBA_t& color = et->color;
				float fontSize = SWFTWIP( et->fontHeight ); // SWF font height is in twips

				file->WriteFloatString( "\t\t<g id=\"%i\" >\n", i );
				file->WriteFloatString(
					"\t\t\t<text x=\"%f\" y=\"%f\" font-family=\"%s\" font-size=\"%f\" fill=\"rgba(%d, %d, %d, %f)\" text-anchor=\"%s\">%s</text>\n",
					et->bounds.tl.x, et->bounds.br.y, GetFontName( et->fontID ), fontSize,
					( int )( color.r ), ( int )( color.g ), ( int )( color.b ), color.a * ( 1.0f / 255.0f ),
					alignStr.c_str(), et->initialText.c_str()
				);
				file->WriteFloatString( "\t\t</g>\n" );
				break;
			}
		}
	}

	file->WriteFloatString( "\t</defs>\n" );

	mainsprite->WriteSVG( file, dictionary.Num(), dictionary );

	file->WriteFloatString( "</svg>\n" );
}
