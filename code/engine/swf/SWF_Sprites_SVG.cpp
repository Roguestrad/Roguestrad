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

swfMatrix_t CombineMatrix( const swfMatrix_t& a, const swfMatrix_t& b )
{
	swfMatrix_t out;
	out.xx = a.xx * b.xx + a.xy * b.yx;
	out.xy = a.xx * b.xy + a.xy * b.yy;
	out.yx = a.yx * b.xx + a.yy * b.yx;
	out.yy = a.yx * b.xy + a.yy * b.yy;
	out.tx = a.xx * b.tx + a.xy * b.ty + a.tx;
	out.ty = a.yx * b.tx + a.yy * b.ty + a.ty;
	return out;
}

swfColorXform_t CombineColorXform( const swfColorXform_t& a, const swfColorXform_t& b )
{
	swfColorXform_t out;
	out.mul.x = a.mul.x * b.mul.x;
	out.mul.y = a.mul.y * b.mul.y;
	out.mul.z = a.mul.z * b.mul.z;
	out.mul.w = a.mul.w * b.mul.w;
	out.add.x = a.add.x + b.add.x;
	out.add.y = a.add.y + b.add.y;
	out.add.z = a.add.z + b.add.z;
	out.add.w = a.add.w + b.add.w;
	return out;
}

void idSWFSprite::WriteSVG( idFile* f, int characterID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict )
{
	f->WriteFloatString( "\t\t<g id=\"%i\" >\n", characterID );

	// Select frame 0 for static export; could be extended to use frameLabels (e.g., "rollOn")
	int frameStart = frameOffsets[0];
	int frameEnd   = ( frameCount > 1 ) ? frameOffsets[1] : commands.Num();

	// for (int i = frameStart; i < frameEnd; i++)
	for( int i = 0; i < commands.Num(); i++ ) {
		idSWFSprite::swfSpriteCommand_t& command = commands[i];

		command.stream.Rewind();
		switch( command.tag ) {
#define HANDLE_SWF_TAG( x )                                      \
	case Tag_##x:                                                \
		WriteSVG_##x( f, command.stream, characterID, i, dict ); \
		break;
			HANDLE_SWF_TAG( PlaceObject2 );
			// HANDLE_SWF_TAG( PlaceObject3 );
			// HANDLE_SWF_TAG( RemoveObject2 );
			// HANDLE_SWF_TAG( StartSound );
			// HANDLE_SWF_TAG( DoAction );
			// HANDLE_SWF_TAG( DoLua );
#undef HANDLE_SWF_TAG
			default:
				break;
				// idLib::Printf( "Export Sprite: Unhandled tag %s\n", idSWF::GetTagName( command.tag ) );
		}
	}

	f->WriteFloatString( "\t\t</g>\n" );
}

void idSWFSprite::WriteSVGUnfolded_r( idFile* f, int characterID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict, const swfMatrix_t& parentMatrix, const swfColorXform_t& parentColor, int indent )
{
	idStr tabs;
	tabs.Fill( '\t', indent );

	f->WriteFloatString( "%s<g id=\"%i\" >\n", tabs.c_str(), characterID );

	// Select frame 0 for static export; could be extended to use frameLabels (e.g., "rollOn")
	// int frameStart = frameOffsets[0];
	// int frameEnd = ( frameCount > 1 ) ? frameOffsets[1] : commands.Num();

	// for (int i = frameStart; i < frameEnd; i++)
	for( int i = 0; i < commands.Num(); i++ ) {
		idSWFSprite::swfSpriteCommand_t& command = commands[i];

		command.stream.Rewind();
		switch( command.tag ) {
#define HANDLE_SWF_TAG( x )                                                                                     \
	case Tag_##x:                                                                                               \
		WriteSVGUnfolded_##x( f, command.stream, characterID, i, dict, parentMatrix, parentColor, indent + 1 ); \
		break;
			HANDLE_SWF_TAG( PlaceObject2 );
			// HANDLE_SWF_TAG( PlaceObject3 );
			// HANDLE_SWF_TAG( RemoveObject2 );
			// HANDLE_SWF_TAG( StartSound );
			// HANDLE_SWF_TAG( DoAction );
			// HANDLE_SWF_TAG( DoLua );
#undef HANDLE_SWF_TAG
			default:
				break;
				// idLib::Printf( "Export Sprite: Unhandled tag %s\n", idSWF::GetTagName( command.tag ) );
		}
	}

	f->WriteFloatString( "%s</g>\n", tabs.c_str() );
}

void idSWFSprite::WriteSVG_PlaceObject2( idFile* file, idSWFBitStream& bitstream, int sourceCharacterID, int commandID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict )
{
	uint8 flags1 = bitstream.ReadU8();
	int	  depth	 = bitstream.ReadU16();

	//<use xlink:href="#candle" transform="translate(100,0)" />

	int	  characterID = -1;
	if( ( flags1 & PlaceFlagHasCharacter ) != 0 ) {
		characterID = bitstream.ReadU16();
	}

	if( characterID == -1 ) {
		return;
	}

	idStr filterID;
	idStr transform;

	if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
		swfMatrix_t m;
		bitstream.ReadMatrix( m );

		if( m.xx != 1.0f || m.yy != 1.0f || m.xy != 0.0f || m.yx != 0.0f ) {
			transform.Format( "transform=\"matrix(%f, %f, %f, %f, %f, %f)\" ",
				m.xx, // a
				m.yx, // b (instead of m.yy)
				m.xy, // c
				m.yy, // d (instead of m.yx)
				m.tx, // e
				m.ty  // f
			);
		} else if( m.tx != 0.0f || m.ty != 0.0f ) {
			transform.Format( "transform=\"translate(%f, %f)\" ", m.tx, m.ty );
		}
	}

	// color transformations are emulated by SVG filters and need be defined before use
	if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
		swfColorXform_t cxf;
		bitstream.ReadColorXFormRGBA( cxf );

		// RB: this adds a lot bloat
		if( cxf.mul != vec4_one || cxf.add != vec4_zero ) {
			filterID.Format( "cf_%i_%i", characterID, commandID );

			idVec4 colorMul = colorWhite;
			if( cxf.mul != vec4_one ) {
				colorMul = cxf.mul;
			}
			colorMul.w = 1.0f; // for debugging only, without most elements are invisible

			idVec4 colorAdd = vec4_zero; // colorBlack
			if( cxf.add != vec4_zero ) {
				colorAdd = cxf.add;
			}

			file->WriteFloatString( "\t\t\t<filter id=\"%s\">\n"
									"\t\t\t\t<feColorMatrix type=\"matrix\" values=\""
									"%f 0 0 0 %f "
									"0 %f 0 0 %f "
									"0 0 %f 0 %f "
									"0 0 0 %f %f\" />\n"
									"\t\t\t</filter>\n",
				filterID.c_str(),
				colorMul.x,
				colorAdd.x,
				colorMul.y,
				colorAdd.y,
				colorMul.z,
				colorAdd.z,
				colorMul.w,
				colorAdd.w );
		}
	}

	const idSWFDictionaryEntry& entry = dict[characterID];
	switch( entry.type ) {
		case SWF_DICT_MORPH:
		case SWF_DICT_SHAPE:
		case SWF_DICT_TEXT:
		case SWF_DICT_EDITTEXT:
		case SWF_DICT_SPRITE: {
			file->WriteFloatString( "\t\t\t<use xlink:href=\"#%i\" ", characterID );
			break;
		}
	}

	if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
		file->WriteFloatString( "%s", transform.c_str() );
	}

	if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
		if( !filterID.IsEmpty() ) {
			file->WriteFloatString( "filter=\"url(#%s)\" />\n", filterID.c_str() );
		}
	}

	if( ( flags1 & PlaceFlagHasRatio ) != 0 ) {
		uint16 ratio = bitstream.ReadU16();
		// file->WriteFloatString( ",\n\t\t\t\t\t\"ratio\": %i", ratio );
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 ) {
		idStr name = bitstream.ReadString();

		file->WriteFloatString( "id=\"%s\" ", name.c_str() );
	}

	if( ( flags1 & PlaceFlagHasClipDepth ) != 0 ) {
		uint16 clipDepth = bitstream.ReadU16();
		// file->WriteFloatString( ",\n\t\t\t\t\t\"clipDepth\": %i", clipDepth );
	}

	if( ( flags1 & PlaceFlagHasClipActions ) != 0 ) {
		// FIXME: clip actions
	}

	file->WriteFloatString( " />\n" );
}

void idSWFSprite::WriteSVGUnfolded_PlaceObject2( idFile* file,
	idSWFBitStream&										 bitstream,
	int													 sourceCharacterID,
	int													 commandID,
	const idList<idSWFDictionaryEntry, TAG_SWF>&		 dict,
	const swfMatrix_t&									 parentMatrix,
	const swfColorXform_t&								 parentColor,
	int													 indent )
{
	uint8 flags1 = bitstream.ReadU8();
	int	  depth	 = bitstream.ReadU16();

	int	  characterID = -1;
	if( ( flags1 & PlaceFlagHasCharacter ) != 0 ) {
		characterID = bitstream.ReadU16();
	}

	if( characterID == -1 ) {
		return;
	}

	idStr tabs;
	tabs.Fill( '\t', indent );

	idStr			filterID;
	idStr			transform;
	idStr			name;

	swfMatrix_t		localMatrix;
	swfColorXform_t localColor;

	if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
		swfMatrix_t m;
		bitstream.ReadMatrix( m );
		localMatrix = m;

		if( m.xx != 1.0f || m.yy != 1.0f || m.xy != 0.0f || m.yx != 0.0f ) {
			transform.Format( "transform=\"matrix(%f, %f, %f, %f, %f, %f)\" ",
				m.xx, // a
				m.yx, // b (instead of m.yy)
				m.xy, // c
				m.yy, // d (instead of m.yx)
				m.tx, // e
				m.ty  // f
			);
		} else if( m.tx != 0.0f || m.ty != 0.0f ) {
			transform.Format( "transform=\"translate(%f, %f)\" ", m.tx, m.ty );
		}
	}
	swfMatrix_t combinedMatrix = CombineMatrix( parentMatrix, localMatrix );

	// color transformations are emulated by SVG filters and need be defined before use
	if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
		swfColorXform_t cxf;
		bitstream.ReadColorXFormRGBA( cxf );
		localColor = cxf;

		// RB: this adds a lot bloat
		if( cxf.mul != vec4_one || cxf.add != vec4_zero ) {
			filterID.Format( "cf_%i_%i", characterID, commandID );

			idVec4 colorMul = colorWhite;
			if( cxf.mul != vec4_one ) {
				colorMul = cxf.mul;
			}
			colorMul.w = 1.0f; // for debugging only, without most elements are invisible

			idVec4 colorAdd = vec4_zero; // colorBlack
			if( cxf.add != vec4_zero ) {
				colorAdd = cxf.add;
			}

			file->WriteFloatString( "%s<filter id=\"%s\">\n"
									"%s\t<feColorMatrix type=\"matrix\" values=\""
									"%f 0 0 0 %f "
									"0 %f 0 0 %f "
									"0 0 %f 0 %f "
									"0 0 0 %f %f\" />\n"
									"%s</filter>\n",
				tabs.c_str(),
				filterID.c_str(),
				tabs.c_str(),
				colorMul.x,
				colorAdd.x,
				colorMul.y,
				colorAdd.y,
				colorMul.z,
				colorAdd.z,
				colorMul.w,
				colorAdd.w,
				tabs.c_str() );
		}
	}
	swfColorXform_t combinedColor = CombineColorXform( parentColor, localColor );

	if( ( flags1 & PlaceFlagHasRatio ) != 0 ) {
		uint16 unused = bitstream.ReadU16();
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 ) {
		name = bitstream.ReadString();
	}

	const idSWFDictionaryEntry& entry = dict[characterID];
	switch( entry.type ) {
		case SWF_DICT_MORPH:
		case SWF_DICT_SHAPE:
		case SWF_DICT_TEXT:
		case SWF_DICT_EDITTEXT: {
			file->WriteFloatString( "%s<use xlink:href=\"#%i\" ", tabs.c_str(), characterID );

			if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
				file->WriteFloatString( "%s", transform.c_str() );
			}

			if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
				if( !filterID.IsEmpty() ) {
					file->WriteFloatString( "filter=\"url(#%s)\" />\n", filterID.c_str() );
				}
			}

			file->WriteFloatString( " />\n" );
			break;
		}

		case SWF_DICT_SPRITE: {
			idSWFSprite* sprite = entry.sprite;

			if( !name.IsEmpty() ) {
				file->WriteFloatString( "%s<g id=\"%s\" ", tabs.c_str(), name.c_str() );
			} else {
				file->WriteFloatString( "%s<g id=\"%i\" ", tabs.c_str(), characterID );
			}

			if( flags1 & PlaceFlagHasMatrix ) {
				file->WriteFloatString( "%s", transform.c_str() );
			}

			if( !filterID.IsEmpty() ) {
				file->WriteFloatString( "filter=\"url(#%s)\"", filterID.c_str() );
			}

			file->WriteFloatString( ">\n" );

			sprite->WriteSVGUnfolded_r( file, characterID, dict, combinedMatrix, combinedColor, indent + 1 );

			file->WriteFloatString( "%s</g>\n", tabs.c_str() );
			break;
		}
	}
}
