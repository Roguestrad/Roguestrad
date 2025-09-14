/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2024 Robert Beckebans

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


void idSWFSprite::WriteSVG( idFile* f, int characterID, const idList< idSWFDictionaryEntry, TAG_SWF >& dict )
{
	f->WriteFloatString( "\t\t<g id=\"%i\" >\n", characterID );

	for( int i = 0; i < commands.Num(); i++ )
	{
		idSWFSprite::swfSpriteCommand_t& command = commands[i];

		command.stream.Rewind();
		switch( command.tag )
		{

#define HANDLE_SWF_TAG( x ) case Tag_##x: WriteSVG_##x( f, command.stream, characterID, i, dict ); break;
				HANDLE_SWF_TAG( PlaceObject2 );
				//HANDLE_SWF_TAG( PlaceObject3 );
				//HANDLE_SWF_TAG( RemoveObject2 );
				//HANDLE_SWF_TAG( StartSound );
				//HANDLE_SWF_TAG( DoAction );
				//HANDLE_SWF_TAG( DoLua );
#undef HANDLE_SWF_TAG
			default:
				break;
				//idLib::Printf( "Export Sprite: Unhandled tag %s\n", idSWF::GetTagName( command.tag ) );
		}
	}

	f->WriteFloatString( "\t\t</g>\n" );
}
// RB end

void idSWFSprite::WriteSVG_PlaceObject2( idFile* file, idSWFBitStream& bitstream, int sourceCharacterID, int commandID, const idList< idSWFDictionaryEntry, TAG_SWF >& dict )
{
	uint8 flags1 = bitstream.ReadU8();
	int depth = bitstream.ReadU16();

	//<use xlink:href="#candle" transform="translate(100,0)" />

	int characterID = -1;
	if( ( flags1 & PlaceFlagHasCharacter ) != 0 )
	{
		characterID = bitstream.ReadU16();

		const idSWFDictionaryEntry& entry = dict[ characterID ];

		switch( entry.type )
		{
			case SWF_DICT_MORPH:
			case SWF_DICT_SHAPE:
			case SWF_DICT_TEXT:
			case SWF_DICT_EDITTEXT:
			case SWF_DICT_SPRITE:
			{
				file->WriteFloatString( "\t\t\t<use xlink:href=\"#%i\" ", characterID );
				break;
			}

				//case SWF_DICT_SPRITE:
				//{
				//	//dictionary[i].sprite->WriteSVG( file, i, dictionary );
				//	break;
				//}
		}
	}

	if( characterID == -1 )
	{
		return;
	}

	if( ( flags1 & PlaceFlagHasMatrix ) != 0 )
	{
		swfMatrix_t m;

		bitstream.ReadMatrix( m );

		//file->WriteFloatString( ",\n\t\t\t\t\t\"startMatrix\": [ %f, %f, %f, %f, %f, %f ]", m.xx, m.yy, m.xy, m.yx, m.tx, m.ty );

		// TODO scaling

		file->WriteFloatString( "transform=\"translate(%f,%f)\" ", m.tx, m.ty );
	}

	if( ( flags1 & PlaceFlagHasColorTransform ) != 0 )
	{
		swfColorXform_t cxf;
		bitstream.ReadColorXFormRGBA( cxf );

		idVec4 color = cxf.mul;
		//file->WriteFloatString( ",\n\t\t\t\t\t\"mulColor\": [ %f, %f, %f, %f ]", color.x, color.y, color.z, color.w );

		color = cxf.add;
		if( color != vec4_origin )
		{
			//file->WriteFloatString( ",\n\t\t\t\t\t\"addColor\": [ %f, %f, %f, %f ]", color.x, color.y, color.z, color.w );
		}
	}

	if( ( flags1 & PlaceFlagHasRatio ) != 0 )
	{
		uint16 ratio = bitstream.ReadU16();
		//file->WriteFloatString( ",\n\t\t\t\t\t\"ratio\": %i", ratio );
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 )
	{
		idStr name = bitstream.ReadString();

		file->WriteFloatString( "id=\"%s\" ", name.c_str() );
	}

	if( ( flags1 & PlaceFlagHasClipDepth ) != 0 )
	{
		uint16 clipDepth = bitstream.ReadU16();
		//file->WriteFloatString( ",\n\t\t\t\t\t\"clipDepth\": %i", clipDepth );
	}

	if( ( flags1 & PlaceFlagHasClipActions ) != 0 )
	{
		// FIXME: clip actions
	}

	file->WriteFloatString( " />\n" );
}



