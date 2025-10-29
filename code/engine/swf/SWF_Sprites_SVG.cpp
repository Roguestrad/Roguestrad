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

static swfMatrix_t CombineMatrix( const swfMatrix_t& a, const swfMatrix_t& b )
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

static swfColorXform_t CombineColorXform( const swfColorXform_t& a, const swfColorXform_t& b )
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

static void WriteAnimateTransform( idFile* file, const idStr& tabs, const swfMatrix_t& from, const swfMatrix_t& to, int frame, float frameDur )
{
	float begin = frame * frameDur;
	file->WriteFloatString( "%s<animateTransform attributeName=\"transform\" type=\"matrix\" "
							"begin=\"%fs\" dur=\"%fs\" "
							"from=\"matrix(%f %f %f %f %f %f)\" "
							"to=\"matrix(%f %f %f %f %f %f)\" fill=\"freeze\" />\n",
		tabs.c_str(),
		begin,
		frameDur,
		from.xx,
		from.yx,
		from.xy,
		from.yy,
		from.tx,
		from.ty,
		to.xx,
		to.yx,
		to.xy,
		to.yy,
		to.tx,
		to.ty );
}

static void WriteAnimateOpacity( idFile* file, const idStr& tabs, const swfColorXform_t& from, const swfColorXform_t& to, int frame, float frameDur )
{
	float begin = frame * frameDur;
	file->WriteFloatString( "%s<animate attributeName=\"opacity\" "
							"begin=\"%fs\" dur=\"%fs\" "
							"from=\"%f\" to=\"%f\" fill=\"freeze\" />\n",
		tabs.c_str(),
		begin,
		frameDur,
		from.mul.w,
		to.mul.w );
}

void swfMatrix_t::ParseSVGTransformFromString( const char* str )
{
	xx = 1.0f;
	yy = 1.0f;
	xy = 0.0f;
	yx = 0.0f;
	tx = 0.0f;
	ty = 0.0f;
	if( !str || !str[0] ) {
		return;
	}

	idStr	transform( str );
	idLexer lexer;
	lexer.LoadMemory( str, idStr::Length( str ), "matrix" );

	if( transform.Find( "matrix", false ) != -1 ) {
		lexer.ExpectTokenString( "matrix" );
		lexer.ExpectTokenString( "(" );
		xx = lexer.ParseFloat();
		lexer.ExpectTokenString( "," );
		yx = lexer.ParseFloat();
		lexer.ExpectTokenString( "," );
		xy = lexer.ParseFloat();
		lexer.ExpectTokenString( "," );
		yy = lexer.ParseFloat();
		lexer.ExpectTokenString( "," );
		tx = lexer.ParseFloat();
		lexer.ExpectTokenString( "," );
		ty = lexer.ParseFloat();
		lexer.ExpectTokenString( ")" );
	} else if( transform.Find( "translate", false ) != -1 ) {
		lexer.ExpectTokenString( "translate" );
		lexer.ExpectTokenString( "(" );
		tx = lexer.ParseFloat();
		lexer.ExpectTokenString( "," );
		ty = lexer.ParseFloat();
		lexer.ExpectTokenString( ")" );
	}
	// TODO: handle other Transform-Types (scale, rotate etc.)

	lexer.FreeSource();
}

static swfColorXform_t ParseColorXformFromFilter( const char* filterStr )
{
	swfColorXform_t cxf;
	cxf.mul = vec4_one;
	cxf.add = vec4_zero;
	if( filterStr ) {
		// TODO: Parse <feColorMatrix> values
	}
	return cxf;
}

void swfColorRGBA_t::ParseSVGColorFromString( const char* str )
{
	if( !str || !str[0] ) {
		return;
	}

	idStr colorStr( str );
	if( colorStr.Find( "rgba", false ) == -1 ) {
		return;
	}

	idLexer lexer;
	lexer.LoadMemory( str, idStr::Length( str ), "color" );
	lexer.ExpectTokenString( "rgba" );
	lexer.ExpectTokenString( "(" );
	int r = lexer.ParseInt();
	lexer.ExpectTokenString( "," );
	int g = lexer.ParseInt();
	lexer.ExpectTokenString( "," );
	int b = lexer.ParseInt();
	lexer.ExpectTokenString( "," );
	float a = lexer.ParseFloat();
	lexer.ExpectTokenString( ")" );

	this->r = ( uint8 )( r );
	this->g = ( uint8 )( g );
	this->b = ( uint8 )( b );
	this->a = ( uint8 )( a * 255.0f );

	lexer.FreeSource();
}

void idSWFSprite::LoadSVGNode( const pugi::xml_node& node, idList<idSWFDictionaryEntry>& dict, bool isUnfolded )
{
	frameCount = 1;

	int depthCounter = 1;

	for( pugi::xml_node child = node.first_child(); child; child = child.next_sibling() ) {
		std::string childName = child.name();

		if( childName == "use" ) {
			swfSpriteCommand_t& cmd = commands.Alloc();
			cmd.tag					= Tag_PlaceObject2;

			idFile_SWF memFile( new idFile_Memory() );

			uint8	   flags = PlaceFlagHasCharacter;
			if( child.attribute( "transform" ) ) {
				flags |= PlaceFlagHasMatrix;
			}
			if( child.attribute( "filter" ) ) {
				flags |= PlaceFlagHasColorTransform;
			}
			if( child.attribute( "id" ) ) {
				flags |= PlaceFlagHasName;
			}

			memFile.WriteU8( flags );
			memFile.WriteU16( depthCounter++ );

			std::string href   = child.attribute( "xlink:href" ).value();
			int			charID = atoi( href.c_str() + 1 );
			memFile.WriteU16( charID );

			if( flags & PlaceFlagHasMatrix ) {
				swfMatrix_t m;
				m.ParseSVGTransformFromString( child.attribute( "transform" ).value() );
				memFile.WriteMatrix( m );
			}

			if( flags & PlaceFlagHasColorTransform ) {
				swfColorXform_t cxf = ParseColorXformFromFilter( child.attribute( "filter" ).value() );
				memFile.WriteColorXFormRGBA( cxf );
			}

			if( child.attribute( "id" ) ) {
				idStr name = child.attribute( "id" ).value();
				memFile.WriteString( name );
			}

			cmd.stream.Load( ( byte* )static_cast<idFile_Memory*>( ( idFile* )memFile )->GetDataPtr(), memFile->Length(), true );

		} else if( childName == "g" ) {
			int					  newCharID = dict.Num();
			idSWFDictionaryEntry& newEntry	= dict.Alloc();
			newEntry.type					= SWF_DICT_SPRITE;
			newEntry.sprite					= new idSWFSprite( NULL );

			newEntry.sprite->LoadSVGNode( child, dict, isUnfolded );

			swfSpriteCommand_t& cmd = commands.Alloc();
			cmd.tag					= Tag_PlaceObject2;
			idFile_SWF memFile( new idFile_Memory() );
			uint8	   flags = PlaceFlagHasCharacter;
			memFile.WriteU8( flags );
			memFile.WriteU16( depthCounter++ );
			memFile.WriteU16( newCharID );

			cmd.stream.Load( ( byte* )static_cast<idFile_Memory*>( ( idFile* )memFile )->GetDataPtr(), memFile->Length(), true );

		} else if( childName == "animate" || childName == "animateTransform" ) {
			idLib::Warning( "SVG Import: Animations not fully supported yet." );
		}
	}

	frameOffsets.Append( 0 );
	frameOffsets.Append( commands.Num() );
}

void idSWFSprite::WriteSVG( idFile* f, int characterID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict )
{
	f->WriteFloatString( "\t\t<g id=\"%i\" data-type=\"SPRITE\" >\n", characterID );

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

void idSWFSprite::WriteSVGUnfolded_r( idFile*	 file,
	int											 characterID,
	const idList<idSWFDictionaryEntry, TAG_SWF>& dict,
	const swfMatrix_t&							 parentMatrix,
	const swfColorXform_t&						 parentColor,
	idHashTableT<int, svgDisplayEntry_t>&		 characterMap,
	float										 frameDur,
	int											 indent )
{
	idHashTableT<int, svgDisplayEntry_t*> localDepthMap;

	idStr								  tabs;
	tabs.Fill( '\t', indent );

	file->WriteFloatString( "%s<g id=\"%i\" >\n", tabs.c_str(), characterID );

	// iterate frame by frame
	for( int frame = 0; frame < frameCount; frame++ ) {
		int frameStart = frameOffsets[frame];
		int frameEnd   = ( frame < frameCount - 1 ) ? frameOffsets[frame + 1] : commands.Num();

		for( int c = frameStart; c < frameEnd; c++ ) {
			idSWFSprite::swfSpriteCommand_t& command = commands[c];
			command.stream.Rewind();

			switch( command.tag ) {
				case Tag_PlaceObject2:
					WriteSVGUnfolded_PlaceObject2( file,
						command.stream,
						characterID,
						c, // commandID
						dict,
						parentMatrix,
						parentColor,
						characterMap,
						localDepthMap,
						frame, // currentFrame
						frameDur,
						indent + 1 );
					break;

				case Tag_PlaceObject3:
					// optional, maybe later
					break;

				case Tag_RemoveObject2:
					// TODO: possibly export <animate ... visibility="hidden">
					break;

				case Tag_StartSound:
				case Tag_DoAction:
				case Tag_DoLua:
					// ignore for now
					break;

				default:
					idLib::Printf( "Export Sprite: Unhandled tag %s\n", idSWF::GetTagName( command.tag ) );
					break;
			}
		}
	}

	// ----------------------------------------------------------
	// After all frames -> export collected animations
	// ----------------------------------------------------------
	for( int i = 0; i < localDepthMap.Num(); i++ ) {
		svgDisplayEntry_t* e = *localDepthMap.GetIndex( i );
		if( e == NULL ) {
			continue;
		}

		idStr targetID = e->name.IsEmpty() ? va( "%i", e->characterID ) : e->name;

#if 0
		// animate opacity track
		if( e->opacityFrames.Num() > 1 ) {
			file->WriteFloatString( "\t%s<animate xlink:href=\"#%s\" attributeName=\"opacity\" values=\"", tabs.c_str(), targetID.c_str() );

			for( int f = 0; f < e->opacityFrames.Num(); f++ ) {
				file->WriteFloatString( "%f", e->opacityFrames[f] );
				if( f < e->opacityFrames.Num() - 1 )
					file->WriteFloatString( ";" );
			}
			//file->WriteFloatString( "\" dur=\"%fs\" repeatCount=\"indefinite\" />\n", e->opacityFrames.Num() * frameDur );
			file->WriteFloatString( "\" dur=\"%fs\" repeatCount=\"1\" restart=\"whenNotActive\" begin=\"%s.mouseover\" />\n", e->opacityFrames.Num() * frameDur, targetID.c_str() );
		}
#endif

#if 0
		// animate transform track
		if (e->matrixFrames.Num() > 1) {
			file->WriteFloatString("%s\t<animateTransform xlink:href=\"#%s\" attributeName=\"transform\" type=\"matrix\" values=\"", tabs.c_str(), targetID.c_str() );

			for (int f = 0; f < e->matrixFrames.Num(); f++) {
				const swfMatrix_t& m = e->matrixFrames[f];
				file->WriteFloatString("matrix(%f %f %f %f %f %f)",
					m.xx, m.yx, m.xy, m.yy, m.tx, m.ty);
				if (f < e->matrixFrames.Num() - 1)
					file->WriteFloatString(";");
			}
			//file->WriteFloatString("\" dur=\"%fs\" repeatCount=\"indefinite\" />\n", e->matrixFrames.Num() * frameDur );
			file->WriteFloatString("\" dur=\"%fs\" repeatCount=\"1\" restart=\"whenNotActive\" begin=\"%s.mouseover\" />\n", e->matrixFrames.Num() * frameDur, targetID.c_str() );
		}
#endif
	}

	file->WriteFloatString( "%s</g>\n", tabs.c_str() );
}

void idSWFSprite::WriteSVG_PlaceObject2( idFile* file, idSWFBitStream& bitstream, int sourceCharacterID, int commandID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict )
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
			file->WriteFloatString( "\t\t\t<use xlink:href=\"#%i\" link-type=\"%s\" ", characterID, idSWF::GetDictTypeName( entry.type ) );
			break;
		}
	}

	if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
		file->WriteFloatString( "%s", transform.c_str() );
	}

	if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
		if( !filterID.IsEmpty() ) {
			file->WriteFloatString( "filter=\"url(#%s)\" ", filterID.c_str() );
		}
	}

	if( ( flags1 & PlaceFlagHasRatio ) != 0 ) {
		uint16 ratio = bitstream.ReadU16();
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 ) {
		idStr name = bitstream.ReadString();

		file->WriteFloatString( "id=\"%s\" ", name.c_str() );
	}

	if( ( flags1 & PlaceFlagHasClipDepth ) != 0 ) {
		uint16 clipDepth = bitstream.ReadU16();
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
	idHashTableT<int, svgDisplayEntry_t>&				 characterMap,
	idHashTableT<int, svgDisplayEntry_t*>&				 localDepthMap,
	int													 currentFrame,
	float												 frameDur,
	int													 indent )
{
	uint8 flags1 = bitstream.ReadU8();
	int	  depth	 = bitstream.ReadU16();

	int	  characterID = -1;
	if( ( flags1 & PlaceFlagHasCharacter ) != 0 ) {
		characterID = bitstream.ReadU16();
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

		if( characterID != -1 ) {
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
	}
	swfMatrix_t combinedMatrix = CombineMatrix( parentMatrix, localMatrix );

	// color transformations are emulated by SVG filters and need be defined before use
	if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
		swfColorXform_t cxf;
		bitstream.ReadColorXFormRGBA( cxf );
		localColor = cxf;

		// this adds a lot bloat, only do it for new objects
		if( characterID != -1 ) {
			if( cxf.mul != vec4_one || cxf.add != vec4_zero ) {
				filterID.Format( "cf_%i_%i", ( characterID != -1 ) ? characterID : depth, commandID );

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
	}
	swfColorXform_t combinedColor = CombineColorXform( parentColor, localColor );

	if( ( flags1 & PlaceFlagHasRatio ) != 0 ) {
		uint16 unused = bitstream.ReadU16();
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 ) {
		name = bitstream.ReadString();
	}

	// ===========================================================
	// DisplayList Handling with Animation
	// ===========================================================
	if( flags1 & PlaceFlagMove ) {
		// update existing object
		svgDisplayEntry_t** entryPtr;
		if( localDepthMap.Get( depth, &entryPtr ) ) {
			svgDisplayEntry_t* entry = *entryPtr;

			if( flags1 & PlaceFlagHasMatrix ) {
				entry->matrixFrames.Append( localMatrix );
				entry->matrix = localMatrix;
			}

			if( flags1 & PlaceFlagHasColorTransform ) {
				entry->opacityFrames.Append( localColor.mul.w ); // TODO should be full color animation
				entry->cxf = localColor;
			}
		}
	} else {
		// new entry (as in runtime, mandatory: characterID != -1)
		if( characterID == -1 ) {
			idLib::Warning( "SVG Export: PlaceObject2 without characterID at depth %i", depth );
			return;
		}

		svgDisplayEntry_t entry;
		entry.characterID = characterID;
		entry.matrix	  = localMatrix;
		entry.cxf		  = localColor;
		entry.name		  = name;

		entry.matrixFrames.Append( localMatrix );
		entry.opacityFrames.Append( localColor.mul.w );

		characterMap.Set( characterID, entry );

		svgDisplayEntry_t* entryPtr;
		characterMap.Get( characterID, &entryPtr );

		localDepthMap.Set( depth, entryPtr );
	}

	// ===========================================================
	// Write into SVG (only for Create!)
	// ===========================================================
	if( characterID == -1 ) {
		return;
	}

	const idSWFDictionaryEntry& dictEntry = dict[characterID];
	switch( dictEntry.type ) {
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
					file->WriteFloatString( "filter=\"url(#%s)\" ", filterID.c_str() );
				}
			}

			if( !name.IsEmpty() ) {
				file->WriteFloatString( "id=\"%s\" ", name.c_str() );
			}

			file->WriteFloatString( " />\n" );
			break;
		}

		case SWF_DICT_SPRITE: {
			idSWFSprite* sprite = dictEntry.sprite;

			if( !name.IsEmpty() ) {
				file->WriteFloatString( "%s<g id=\"%s\" ", tabs.c_str(), name.c_str() );
			} else {
				file->WriteFloatString( "%s<g id=\"%i\" ", tabs.c_str(), characterID );
			}

			if( flags1 & PlaceFlagHasMatrix ) {
				file->WriteFloatString( "%s", transform.c_str() );
			}

			if( !filterID.IsEmpty() ) {
				file->WriteFloatString( "filter=\"url(#%s)\" ", filterID.c_str() );
			}

			file->WriteFloatString( ">\n" );

			sprite->WriteSVGUnfolded_r( file, characterID, dict, combinedMatrix, combinedColor, characterMap, frameDur, indent + 1 );

			file->WriteFloatString( "%s</g>\n", tabs.c_str() );
			break;
		}
	}
}
