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

static bool IsMatrixAnimated( const idList<swfMatrix_t>& frames )
{
	if( frames.Num() <= 1 ) {
		return false;
	}

	const swfMatrix_t& first = frames[0];
	const float		   eps	 = 0.0001f; // tolerance for rounding errors

	for( int i = 1; i < frames.Num(); i++ ) {
		const swfMatrix_t& current = frames[i];
		if( idMath::Fabs( current.xx - first.xx ) > eps || idMath::Fabs( current.yy - first.yy ) > eps || idMath::Fabs( current.xy - first.xy ) > eps || idMath::Fabs( current.yx - first.yx ) > eps ||
			idMath::Fabs( current.tx - first.tx ) > eps || idMath::Fabs( current.ty - first.ty ) > eps ) {
			return true;
		}
	}
	return false;
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
		if( lexer.CheckTokenString( "," ) ) {
		}
		yx = lexer.ParseFloat();
		if( lexer.CheckTokenString( "," ) ) {
		}
		xy = lexer.ParseFloat();
		if( lexer.CheckTokenString( "," ) ) {
		}
		yy = lexer.ParseFloat();
		if( lexer.CheckTokenString( "," ) ) {
		}
		tx = lexer.ParseFloat();
		if( lexer.CheckTokenString( "," ) ) {
		}
		ty = lexer.ParseFloat();
		lexer.ExpectTokenString( ")" );
	} else if( transform.Find( "translate", false ) != -1 ) {
		lexer.ExpectTokenString( "translate" );
		lexer.ExpectTokenString( "(" );
		tx = lexer.ParseFloat();
		if( lexer.CheckTokenString( "," ) ) {
		}
		ty = lexer.ParseFloat();
		lexer.ExpectTokenString( ")" );
	} else if( transform.Find( "scale", false ) != -1 ) {
		lexer.ExpectTokenString( "scale" );
		lexer.ExpectTokenString( "(" );
		xx = lexer.ParseFloat();
		if( lexer.CheckTokenString( "," ) ) {
			yy = lexer.ParseFloat();
		} else {
			yy = xx;
		}
		lexer.ExpectTokenString( ")" );
	} else if( transform.Find( "rotate", false ) != -1 ) {
		lexer.ExpectTokenString( "rotate" );
		lexer.ExpectTokenString( "(" );
		float angle = lexer.ParseFloat();
		float s, c;
		idMath::SinCos( angle * idMath::PI / 180.0f, s, c );
		xx = c;
		yx = s;
		xy = -s;
		yy = c;
		if( lexer.CheckTokenString( "," ) ) {
			// rotation around a point (cx, cy)
			float cx = lexer.ParseFloat();
			if( lexer.CheckTokenString( "," ) ) {
			}
			float cy = lexer.ParseFloat();
			tx		 = cx - c * cx + s * cy;
			ty		 = cy - s * cx - c * cy;
		}
		lexer.ExpectTokenString( ")" );
	}

	lexer.FreeSource();
}

static swfColorXform_t ParseColorXformFromFilter( const idHashTableT<idStr, swfColorXform_t>& svgFilterColorXforms, const char* filterStr )
{
	swfColorXform_t cxf;
	cxf.mul = vec4_one;
	cxf.add = vec4_zero;

	if( filterStr == NULL || filterStr[0] == '\0' ) {
		return cxf;
	}

	// expected: url(#ID)
	idStr s		  = filterStr;
	int	  hashPos = s.Find( "#", false );
	if( hashPos == -1 ) {
		return cxf;
	}
	int endPos = s.Find( ")", false );
	if( endPos == -1 || endPos <= hashPos + 1 ) {
		endPos = s.Length();
	}

	idStr				   filterId = s.Mid( hashPos + 1, endPos - ( hashPos + 1 ) );
	// filterId.StripWhitespace();

	const swfColorXform_t* found = NULL;
	if( svgFilterColorXforms.Get( filterId, &found ) && found != NULL ) {
		cxf = *found;
	}

	return cxf;
}

static bool ParseSVG_ColorMatrixValues( const char* valuesStr, float out[20] )
{
	if( valuesStr == NULL || valuesStr[0] == '\0' ) {
		return false;
	}

	return true;
}

static bool ParseColorXformFromFilterNode( const pugi::xml_node& filterNode, swfColorXform_t& outCxf )
{
	outCxf.mul = vec4_one;
	outCxf.add = vec4_zero;

	pugi::xml_node fe = filterNode.child( "feColorMatrix" );
	if( !fe ) {
		return false;
	}

	const char* typeStr = fe.attribute( "type" ).value();
	if( typeStr == NULL || idStr::Icmp( typeStr, "matrix" ) != 0 ) {
		return false;
	}

	const char* valuesStr = fe.attribute( "values" ).value();
	if( valuesStr == NULL || valuesStr[0] == '\0' ) {
		return false;
	}

	float	m[20];
	idLexer lexer;
	lexer.LoadMemory( valuesStr, idStr::Length( valuesStr ), "feColorMatrix values" );
	lexer.SetFlags( LEXFL_NOFATALERRORS | LEXFL_NOSTRINGCONCAT );

	for( int i = 0; i < 20; i++ ) {
		m[i] = lexer.ParseFloat();

		if( lexer.PeekTokenString( "," ) ) {
			lexer.ExpectTokenString( "," );
		}
	}

	lexer.FreeSource();

	// only diagonal + translate allowed (no channel-mixing)
	const float eps	   = 0.00001f;
	auto		isZero = [&]( float v ) { return idMath::Fabs( v ) <= eps; };

	if( !isZero( m[1] ) || !isZero( m[2] ) || !isZero( m[3] ) || !isZero( m[5] ) || !isZero( m[7] ) || !isZero( m[8] ) || !isZero( m[10] ) || !isZero( m[11] ) || !isZero( m[13] ) ||
		!isZero( m[15] ) || !isZero( m[16] ) || !isZero( m[17] ) ) {
		idLib::Warning( "SVG Import: feColorMatrix has cross-channel terms (unsupported). Ignoring filter '%s'.", filterNode.attribute( "id" ).value() );
		return false;
	}

	outCxf.mul.x = m[0];
	outCxf.mul.y = m[6];
	outCxf.mul.z = m[12];
	outCxf.mul.w = m[18];

	outCxf.add.x = m[4];
	outCxf.add.y = m[9];
	outCxf.add.z = m[14];
	outCxf.add.w = m[19];

	return true;
}

// -----------------------------------------------------------------
//  CSS standard colors (140 entries)
//  Source: https://www.w3.org/TR/css-color-3/#svg-color
// -----------------------------------------------------------------
struct cssColor_t {
	const char* name;
	uint8_t		r, g, b, a; // a == 255 (fully opaque)
};

static const cssColor_t cssColors[] = {
	/*  Pink colors  */
	{ "pink", 255, 192, 203, 255 },
	{ "lightpink", 255, 182, 193, 255 },
	{ "hotpink", 255, 105, 180, 255 },
	{ "deeppink", 255, 20, 147, 255 },
	{ "palevioletred", 219, 112, 147, 255 },
	{ "mediumvioletred", 199, 21, 133, 255 },

	/*  Red colors  */
	{ "lightsalmon", 255, 160, 122, 255 },
	{ "salmon", 250, 128, 114, 255 },
	{ "darksalmon", 233, 150, 122, 255 },
	{ "lightcoral", 240, 128, 128, 255 },
	{ "indianred", 205, 92, 92, 255 },
	{ "crimson", 220, 20, 60, 255 },
	{ "firebrick", 178, 34, 34, 255 },
	{ "darkred", 139, 0, 0, 255 },
	{ "red", 255, 0, 0, 255 },

	/*  Orange colors  */
	{ "orangered", 255, 69, 0, 255 },
	{ "tomato", 255, 99, 71, 255 },
	{ "coral", 255, 127, 80, 255 },
	{ "darkorange", 255, 140, 0, 255 },
	{ "orange", 255, 165, 0, 255 },

	/*  Yellow colors  */
	{ "yellow", 255, 255, 0, 255 },
	{ "lightyellow", 255, 255, 224, 255 },
	{ "lemonchiffon", 255, 250, 205, 255 },
	{ "lightgoldenrodyellow", 250, 250, 210, 255 },
	{ "papayawhip", 255, 239, 213, 255 },
	{ "moccasin", 255, 228, 181, 255 },
	{ "peachpuff", 255, 218, 185, 255 },
	{ "palegoldenrod", 238, 232, 170, 255 },
	{ "khaki", 240, 230, 140, 255 },
	{ "darkkhaki", 189, 183, 107, 255 },
	{ "gold", 255, 215, 0, 255 },

	/*  Brown colors  */
	{ "cornsilk", 255, 248, 220, 255 },
	{ "blanchedalmond", 255, 235, 205, 255 },
	{ "bisque", 255, 228, 196, 255 },
	{ "navajowhite", 255, 222, 173, 255 },
	{ "wheat", 245, 222, 179, 255 },
	{ "burlywood", 222, 184, 135, 255 },
	{ "tan", 210, 180, 140, 255 },
	{ "rosybrown", 188, 143, 143, 255 },
	{ "sandybrown", 244, 164, 96, 255 },
	{ "goldenrod", 218, 165, 32, 255 },
	{ "darkgoldenrod", 184, 134, 11, 255 },
	{ "peru", 205, 133, 63, 255 },
	{ "chocolate", 210, 105, 30, 255 },
	{ "saddlebrown", 139, 69, 19, 255 },
	{ "sienna", 160, 82, 45, 255 },
	{ "brown", 165, 42, 42, 255 },
	{ "maroon", 128, 0, 0, 255 },

	/*  Green colors  */
	{ "darkolivegreen", 85, 107, 47, 255 },
	{ "olive", 128, 128, 0, 255 },
	{ "olivedrab", 107, 142, 35, 255 },
	{ "yellowgreen", 154, 205, 50, 255 },
	{ "limegreen", 50, 205, 50, 255 },
	{ "lime", 0, 255, 0, 255 },
	{ "lawngreen", 124, 252, 0, 255 },
	{ "chartreuse", 127, 255, 0, 255 },
	{ "greenyellow", 173, 255, 47, 255 },
	{ "springgreen", 0, 255, 127, 255 },
	{ "mediumspringgreen", 0, 250, 154, 255 },
	{ "lightgreen", 144, 238, 144, 255 },
	{ "palegreen", 152, 251, 152, 255 },
	{ "darkseagreen", 143, 188, 143, 255 },
	{ "mediumseagreen", 60, 179, 113, 255 },
	{ "seagreen", 46, 139, 87, 255 },
	{ "forestgreen", 34, 139, 34, 255 },
	{ "green", 0, 128, 0, 255 },
	{ "darkgreen", 0, 100, 0, 255 },

	/*  Cyan colors  */
	{ "mediumaquamarine", 102, 205, 170, 255 },
	{ "aqua", 0, 255, 255, 255 },
	{ "cyan", 0, 255, 255, 255 },
	{ "lightcyan", 224, 255, 255, 255 },
	{ "paleturquoise", 175, 238, 238, 255 },
	{ "aquamarine", 127, 255, 212, 255 },
	{ "turquoise", 64, 224, 208, 255 },
	{ "mediumturquoise", 72, 209, 204, 255 },
	{ "darkturquoise", 0, 206, 209, 255 },
	{ "lightseagreen", 32, 178, 170, 255 },
	{ "cadetblue", 95, 158, 160, 255 },
	{ "darkcyan", 0, 139, 139, 255 },
	{ "teal", 0, 128, 128, 255 },

	/*  Blue colors  */
	{ "lightsteelblue", 176, 196, 222, 255 },
	{ "powderblue", 176, 224, 230, 255 },
	{ "lightblue", 173, 216, 230, 255 },
	{ "skyblue", 135, 206, 235, 255 },
	{ "lightskyblue", 135, 206, 250, 255 },
	{ "deepskyblue", 0, 191, 255, 255 },
	{ "dodgerblue", 30, 144, 255, 255 },
	{ "cornflowerblue", 100, 149, 237, 255 },
	{ "steelblue", 70, 130, 180, 255 },
	{ "royalblue", 65, 105, 225, 255 },
	{ "blue", 0, 0, 255, 255 },
	{ "mediumblue", 0, 0, 205, 255 },
	{ "darkblue", 0, 0, 139, 255 },
	{ "navy", 0, 0, 128, 255 },
	{ "midnightblue", 25, 25, 112, 255 },

	/*  Violet colors  */
	{ "lavender", 230, 230, 250, 255 },
	{ "thistle", 216, 191, 216, 255 },
	{ "plum", 221, 160, 221, 255 },
	{ "violet", 238, 130, 238, 255 },
	{ "orchid", 218, 112, 214, 255 },
	{ "fuchsia", 255, 0, 255, 255 },
	{ "magenta", 255, 0, 255, 255 },
	{ "mediumorchid", 186, 85, 211, 255 },
	{ "mediumpurple", 147, 112, 219, 255 },
	{ "blueviolet", 138, 43, 226, 255 },
	{ "darkviolet", 148, 0, 211, 255 },
	{ "darkorchid", 153, 50, 204, 255 },
	{ "darkmagenta", 139, 0, 139, 255 },
	{ "purple", 128, 0, 128, 255 },
	{ "indigo", 75, 0, 130, 255 },
	{ "darkslateblue", 72, 61, 139, 255 },
	{ "slateblue", 106, 90, 205, 255 },
	{ "mediumslateblue", 123, 104, 238, 255 },
	{ "rebeccapurple", 102, 51, 153, 255 },

	/*  White colors  */
	{ "white", 255, 255, 255, 255 },
	{ "snow", 255, 250, 250, 255 },
	{ "honeydew", 240, 255, 240, 255 },
	{ "mintcream", 245, 255, 250, 255 },
	{ "azure", 240, 255, 255, 255 },
	{ "aliceblue", 240, 248, 255, 255 },
	{ "ghostwhite", 248, 248, 255, 255 },
	{ "whitesmoke", 245, 245, 245, 255 },
	{ "seashell", 255, 245, 238, 255 },
	{ "beige", 245, 245, 220, 255 },
	{ "oldlace", 253, 245, 230, 255 },
	{ "floralwhite", 255, 250, 240, 255 },
	{ "ivory", 255, 255, 240, 255 },
	{ "antiquewhite", 250, 235, 215, 255 },
	{ "linen", 250, 240, 230, 255 },
	{ "lavenderblush", 255, 240, 245, 255 },
	{ "mistyrose", 255, 228, 225, 255 },

	/*  Gray colors  */
	{ "gainsboro", 220, 220, 220, 255 },
	{ "lightgray", 211, 211, 211, 255 },
	{ "silver", 192, 192, 192, 255 },
	{ "darkgray", 169, 169, 169, 255 },
	{ "gray", 128, 128, 128, 255 },
	{ "dimgray", 105, 105, 105, 255 },
	{ "lightslategray", 119, 136, 153, 255 },
	{ "slategray", 112, 128, 144, 255 },
	{ "darkslategray", 47, 79, 79, 255 },
	{ "black", 0, 0, 0, 255 },

	/*  Sentinel (checked in parser)  */
	{ nullptr, 0, 0, 0, 0 }
};

const char* cssNameFromRGBA( const swfColorRGBA_t& col )
{
	for( const cssColor_t* c = cssColors; c->name != nullptr; ++c ) {
		if( c->r == col.r && c->g == col.g && c->b == col.b && c->a == col.a ) {
			return c->name;
		}
	}

	// fallback: rgba(...)
	static char tmp[32];
	snprintf( tmp, sizeof( tmp ), "rgba(%d, %d, %d, %f)", col.r, col.g, col.b, col.a / 255.0f );
	return tmp;
}

void swfColorRGBA_t::ParseSVGColorFromString( const char* str )
{
	if( !str || !str[0] ) {
		return; // nothing to do
	}

	idStr colorStr( str );
	// colorStr.Trim();                  // remove whitespace

	// 1) Hex notation #RRGGBB or #RGB
	if( colorStr[0] == '#' ) {
		uint32_t hex = 0;
		if( sscanf( colorStr.c_str() + 1, "%x", &hex ) == 1 ) {
			if( colorStr.Length() == 7 ) { // #RRGGBB
				r = ( hex >> 16 ) & 0xFF;
				g = ( hex >> 8 ) & 0xFF;
				b = ( hex ) & 0xFF;
				a = 255;
			} else if( colorStr.Length() == 4 ) { // #RGB
				r = ( ( hex >> 8 ) & 0xF ) * 17;
				g = ( ( hex >> 4 ) & 0xF ) * 17;
				b = ( ( hex ) & 0xF ) * 17;
				a = 255;
			}
			return;
		}
	}

	// 2) rgba(r,g,b,a)
	if( colorStr.Find( "rgba", false ) != -1 ) {
		idLexer lexer;
		lexer.LoadMemory( str, idStr::Length( str ), "color" );
		lexer.ExpectTokenString( "rgba" );
		lexer.ExpectTokenString( "(" );
		int rr = lexer.ParseInt();
		lexer.ExpectTokenString( "," );
		int gg = lexer.ParseInt();
		lexer.ExpectTokenString( "," );
		int bb = lexer.ParseInt();
		lexer.ExpectTokenString( "," );
		float aa = lexer.ParseFloat();
		lexer.ExpectTokenString( ")" );
		r = ( uint8_t )rr;
		g = ( uint8_t )gg;
		b = ( uint8_t )bb;
		a = ( uint8_t )( aa * 255.0f );
		lexer.FreeSource();
		return;
	}

	// 3) rgb(r,g,b)
	if( colorStr.Find( "rgb", false ) != -1 ) {
		idLexer lexer;
		lexer.LoadMemory( str, idStr::Length( str ), "color" );
		lexer.ExpectTokenString( "rgb" );
		lexer.ExpectTokenString( "(" );
		int rr = lexer.ParseInt();
		lexer.ExpectTokenString( "," );
		int gg = lexer.ParseInt();
		lexer.ExpectTokenString( "," );
		int bb = lexer.ParseInt();
		lexer.ExpectTokenString( ")" );
		r = ( uint8_t )rr;
		g = ( uint8_t )gg;
		b = ( uint8_t )bb;
		a = 255;
		lexer.FreeSource();
		return;
	}

	// 4) CSS name
	// (case-insensitive comparison)
	for( const cssColor_t* c = cssColors; c->name != nullptr; ++c ) {
		if( colorStr.Icmp( c->name ) == 0 ) {
			r = c->r;
			g = c->g;
			b = c->b;
			a = c->a;
			return;
		}
	}

	// 5) unknown color – do not change
}

static bool HasDirectShapeChildren( const pugi::xml_node& g )
{
	const char* linkType = g.attribute( "link-type" ).value();
	return ( idStr::Icmp( linkType, "BITMAP" ) == 0 ) || g.child( "polygon" ) || g.child( "polyline" );
}

static bool HasDirectTextChild( const pugi::xml_node& g )
{
	return g.child( "text" );
}

static bool HasDirectImageChild( const pugi::xml_node& g )
{
	return g.child( "image" );
}

void idSWFSprite::LoadSVGNode_r( const pugi::xml_node& node, idList<idSWFDictionaryEntry>& dict, bool isUnfolded )
{
	frameCount = 1;
	frameOffsets.Clear();
	frameOffsets.Append( 0 );

	int						 depthCounter = 1;
	idHashTableT<idStr, int> idToDepth;
	idList<pugi::xml_node>	 animations;

	for( pugi::xml_node s = node.first_child(); s; s = s.next_sibling() ) {
		std::string childName	   = s.name();
		const char* dataType	   = s.attribute( "data-type" ).value();
		const bool	isWrapperPlace = ( dataType != nullptr && idStr::Icmp( dataType, "Tag_PlaceObject2" ) == 0 );

		if( childName == "use" ) {
			swfSpriteCommand_t& cmd = commands.Alloc();
			cmd.tag					= Tag_PlaceObject2;

			idFile_SWF memFile( new idFile_Memory() );

			uint8	   flags = PlaceFlagHasCharacter;
			if( s.attribute( "transform" ) ) {
				flags |= PlaceFlagHasMatrix;
			}
			if( s.attribute( "filter" ) ) {
				flags |= PlaceFlagHasColorTransform;
			}
			if( s.attribute( "id" ) ) {
				flags |= PlaceFlagHasName;
			}

			memFile.WriteU8( flags );
			memFile.WriteU16( depthCounter++ );

			std::string href   = s.attribute( "xlink:href" ).value();
			int			charID = atoi( href.c_str() + 1 );
			memFile.WriteU16( charID );

			if( flags & PlaceFlagHasMatrix ) {
				swfMatrix_t m;
				m.ParseSVGTransformFromString( s.attribute( "transform" ).value() );
				memFile.WriteMatrix( m );
			}

			if( flags & PlaceFlagHasColorTransform ) {
				swfColorXform_t cxf = ParseColorXformFromFilter( swf->svgFilterColorXforms, s.attribute( "filter" ).value() );
				memFile.WriteColorXFormRGBA( cxf );
			}

			if( flags & PlaceFlagHasName ) {
				idStr name = s.attribute( "id" ).value();
				memFile.WriteString( name );
				idToDepth.Set( name, depthCounter - 1 );
			} else {
				// Fallback: track depth by character ID if no name is present
				idToDepth.Set( va( "%i", charID ), depthCounter - 1 );
			}

			cmd.stream.Load( ( byte* )static_cast<idFile_Memory*>( ( idFile* )memFile )->GetDataPtr(), memFile->Length(), true );

		} else if( childName == "filter" ) {
			const char* filterIdStr = s.attribute( "id" ).value();
			if( filterIdStr != NULL && filterIdStr[0] != '\0' ) {
				swfColorXform_t cxf;
				if( ParseColorXformFromFilterNode( s, cxf ) ) {
					swf->svgFilterColorXforms.Set( filterIdStr, cxf );
				} else {
					// Unknown/unsupported filter -> store identity so lookups are stable
					swfColorXform_t ident;
					ident.mul = vec4_one;
					ident.add = vec4_zero;
					swf->svgFilterColorXforms.Set( filterIdStr, ident );
				}
			}

		} else if( childName == "g" ) {
			int					  newCharID = dict.Num();
			idSWFDictionaryEntry& newEntry	= dict.Alloc();

			if( HasDirectShapeChildren( s ) ) {
				newEntry.type  = SWF_DICT_SHAPE;
				newEntry.shape = new( TAG_SWF ) idSWFShape;
				swf->ParseSVG_Shape( s, newEntry.shape );
			} else if( HasDirectTextChild( s ) ) {
				newEntry.type	  = SWF_DICT_EDITTEXT;
				newEntry.edittext = new( TAG_SWF ) idSWFEditText;
				swf->ParseSVG_Text( s, newEntry.edittext );
			} else if( HasDirectImageChild( s ) ) {
				swf->ParseSVG_Image( s, newCharID, newEntry );
			} else {
				newEntry.type	= SWF_DICT_SPRITE;
				newEntry.sprite = new idSWFSprite( swf );
				newEntry.sprite->LoadSVGNode_r( s, dict, isUnfolded );
			}

			if( isWrapperPlace ) {
				swfSpriteCommand_t& cmd = commands.Alloc();
				cmd.tag					= Tag_PlaceObject2;

				idFile_SWF memFile( new idFile_Memory() );

				uint8	   flags = PlaceFlagHasCharacter;
				if( s.attribute( "transform" ) ) {
					flags |= PlaceFlagHasMatrix;
				}
				if( s.attribute( "filter" ) ) {
					flags |= PlaceFlagHasColorTransform;
				}
				if( s.attribute( "id" ) && !idStr::IsNumeric( s.attribute( "id" ).value() ) ) {
					flags |= PlaceFlagHasName;
				}

				memFile.WriteU8( flags );
				memFile.WriteU16( depthCounter++ );
				memFile.WriteU16( newCharID );

				if( flags & PlaceFlagHasMatrix ) {
					swfMatrix_t m;
					m.ParseSVGTransformFromString( s.attribute( "transform" ).value() );
					memFile.WriteMatrix( m );
				}

				if( flags & PlaceFlagHasColorTransform ) {
					swfColorXform_t cxf = ParseColorXformFromFilter( swf->svgFilterColorXforms, s.attribute( "filter" ).value() );
					memFile.WriteColorXFormRGBA( cxf );
				}

				if( flags & PlaceFlagHasName ) {
					idStr name = s.attribute( "id" ).value();
					if( !name.IsEmpty() ) {
						memFile.WriteString( name );
						idToDepth.Set( name, depthCounter - 1 );
					}
				} else {
					idToDepth.Set( va( "%i", newCharID ), depthCounter - 1 );
				}

				cmd.stream.Load( ( byte* )static_cast<idFile_Memory*>( ( idFile* )memFile )->GetDataPtr(), memFile->Length(), true );
			} else {
				// place the newly created character into this sprite
				swfSpriteCommand_t& cmd = commands.Alloc();
				cmd.tag					= Tag_PlaceObject2;
				idFile_SWF memFile( new idFile_Memory() );
				uint8	   flags = PlaceFlagHasCharacter;

				idStr	   idAttr = s.attribute( "id" ).value();
				if( !idAttr.IsEmpty() ) {
					flags |= PlaceFlagHasName;
				}

				memFile.WriteU8( flags );
				memFile.WriteU16( depthCounter++ );
				memFile.WriteU16( newCharID );

				if( flags & PlaceFlagHasName ) {
					memFile.WriteString( idAttr );
					idToDepth.Set( idAttr, depthCounter - 1 );
				} else {
					idToDepth.Set( va( "%i", newCharID ), depthCounter - 1 );
				}

				cmd.stream.Load( ( byte* )static_cast<idFile_Memory*>( ( idFile* )memFile )->GetDataPtr(), memFile->Length(), true );
			}

		} else if( childName == "animate" || childName == "animateTransform" ) {
			animations.Append( s );
		}
	}

	// Process SVG SMIL animations
	struct parsedAnim_t {
		int			  depth;
		idList<idStr> valueList;
		idStr		  attributeName;
		bool		  isTransform;
		idStr		  transformType;
		bool		  isAdditive;
	};
	idList<parsedAnim_t> parsedAnimations;

	for( int a = 0; a < animations.Num(); a++ ) {
		const pugi::xml_node& animNode = animations[a];
		idStr				  href	   = animNode.attribute( "xlink:href" ).value();
		if( href.IsEmpty() || href[0] != '#' ) {
			continue;
		}

		idStr targetID = href.c_str() + 1;
		int*  depthPtr;
		if( !idToDepth.Get( targetID, &depthPtr ) ) {
			continue;
		}

		idStr values = animNode.attribute( "values" ).value();
		if( values.IsEmpty() ) {
			continue;
		}

		parsedAnim_t& pa = parsedAnimations.Alloc();
		pa.depth		 = *depthPtr;
		idStr::Split( values.c_str(), pa.valueList, ';' );
		pa.attributeName = animNode.attribute( "attributeName" ).value();
		pa.isTransform	 = ( idStr::Icmp( animNode.name(), "animateTransform" ) == 0 );
		if( pa.isTransform ) {
			pa.transformType = animNode.attribute( "type" ).value();
			pa.isAdditive	 = ( idStr( animNode.attribute( "additive" ).value() ) == "sum" );
		}

		if( pa.valueList.Num() > frameCount ) {
			frameCount = pa.valueList.Num();
		}
	}

	// Group commands frame by frame
	for( int i = 1; i < frameCount; i++ ) {
		idList<int>						   depthsInFrame;
		idHashTableT<int, swfMatrix_t>	   depthMatrix;
		idHashTableT<int, swfColorXform_t> depthColor;

		for( int a = 0; a < parsedAnimations.Num(); a++ ) {
			const parsedAnim_t& pa = parsedAnimations[a];

			if( i >= pa.valueList.Num() ) {
				continue;
			}

			if( pa.isTransform ) {
				swfMatrix_t m;
				idLexer		lexer;
				lexer.LoadMemory( pa.valueList[i].c_str(), pa.valueList[i].Length(), "value" );
				if( pa.transformType == "translate" ) {
					m.tx = lexer.ParseFloat();
					if( !lexer.EndOfFile() ) {
						m.ty = lexer.ParseFloat();
					}
				} else if( pa.transformType == "scale" ) {
					m.xx = lexer.ParseFloat();
					if( !lexer.EndOfFile() ) {
						m.yy = lexer.ParseFloat();
					} else {
						m.yy = m.xx;
					}
				} else if( pa.transformType == "rotate" ) {
					float angle = lexer.ParseFloat();
					float s, c;
					idMath::SinCos( angle * idMath::PI / 180.0f, s, c );
					m.xx = c;
					m.yx = s;
					m.xy = -s;
					m.yy = c;
					if( !lexer.EndOfFile() ) {
						float cx = lexer.ParseFloat();
						float cy = 0.0f;
						if( !lexer.EndOfFile() ) {
							cy = lexer.ParseFloat();
						}
						m.tx = cx - c * cx + s * cy;
						m.ty = cy - s * cx - c * cy;
					}
				}
				lexer.FreeSource();

				swfMatrix_t* existingM;
				if( depthMatrix.Get( pa.depth, &existingM ) ) {
					if( pa.isAdditive ) {
						*existingM = existingM->Multiply( m );
					} else {
						*existingM = m;
					}
				} else {
					depthMatrix.Set( pa.depth, m );
					depthsInFrame.Append( pa.depth );
				}
			} else if( pa.attributeName == "opacity" ) {
				swfColorXform_t cxf;
				cxf.mul	  = vec4_one;
				cxf.mul.w = ( float )atof( pa.valueList[i].c_str() );
				cxf.add	  = vec4_zero;

				swfColorXform_t* existingCxf;
				if( depthColor.Get( pa.depth, &existingCxf ) ) {
					existingCxf->mul.w *= cxf.mul.w;
				} else {
					depthColor.Set( pa.depth, cxf );
					bool alreadyAdded = false;
					for( int d = 0; d < depthsInFrame.Num(); d++ ) {
						if( depthsInFrame[d] == pa.depth ) {
							alreadyAdded = true;
							break;
						}
					}
					if( !alreadyAdded ) {
						depthsInFrame.Append( pa.depth );
					}
				}
			}
		}

		for( int d = 0; d < depthsInFrame.Num(); d++ ) {
			int					depth = depthsInFrame[d];
			swfMatrix_t*		m;
			bool				hasMatrix = depthMatrix.Get( depth, &m );
			swfColorXform_t*	cxf;
			bool				hasColor = depthColor.Get( depth, &cxf );

			swfSpriteCommand_t& cmd = commands.Alloc();
			cmd.tag					= Tag_PlaceObject2;

			idFile_SWF memFile( new idFile_Memory() );
			uint8	   flags = PlaceFlagMove;
			if( hasMatrix ) {
				flags |= PlaceFlagHasMatrix;
			}
			if( hasColor ) {
				flags |= PlaceFlagHasColorTransform;
			}

			memFile.WriteU8( flags );
			memFile.WriteU16( depth );

			if( hasMatrix ) {
				memFile.WriteMatrix( *m );
			}

			if( hasColor ) {
				memFile.WriteColorXFormRGBA( *cxf );
			}

			cmd.stream.Load( ( byte* )static_cast<idFile_Memory*>( ( idFile* )memFile )->GetDataPtr(), memFile->Length(), true );
		}
		frameOffsets.Append( commands.Num() );
	}

	while( frameOffsets.Num() <= frameCount ) {
		frameOffsets.Append( commands.Num() );
	}
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

void idSWFSprite::WriteSVGUnfolded_r(
	idFile* file, int characterID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict, idHashTableT<int, svgDisplayEntry_t>& characterMap, float frameDur, const idStr& prefix, int indent )
{
	idHashTableT<int, svgDisplayEntry_t*> localDepthMap;

	idStr								  tabs;
	tabs.Fill( '\t', indent );

	// PHASE 1: Timeline Simulation & Snapshot Collection

	for( int frame = 0; frame < frameCount; frame++ ) {
		int frameStart = frameOffsets[frame];
		int frameEnd   = ( frame < frameCount - 1 ) ? frameOffsets[frame + 1] : commands.Num();

		// A) apply all changes to this frame
		for( int c = frameStart; c < frameEnd; c++ ) {
			idSWFSprite::swfSpriteCommand_t& command = commands[c];
			command.stream.Rewind();

			switch( command.tag ) {
				case Tag_PlaceObject2:
					PreRun_PlaceObject2( command.stream,
						characterID,
						prefix,
						c, // commandID
						dict,
						characterMap,
						localDepthMap,
						frame // currentFrame
					);
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

		// B) SNAPSHOT: Now store a value for each active object for this frame
		// for( int i = 0; i < localDepthMap.Num(); i++ ) {
		// 	svgDisplayEntry_t* e = *localDepthMap.GetIndex( i );
		// 	if( e != NULL ) {
		// 		e->matrixFrames.Append( e->matrix );
		// 		e->opacityFrames.Append( e->cxf.mul.w );
		// 	}
		// }
	}

	// PHASE 2: SVG Export

	// idStr uniqueID;
	// uniqueID.Format( "%s.%i", prefix.c_str(),characterID );

	file->WriteFloatString( "%s<g id=\"%s\" data-type=\"SPRITE\" >\n", tabs.c_str(), prefix.c_str() );

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
						prefix,
						c, // commandID
						dict,
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

		int	  depth = i;

		idStr targetID;
		if( !e->name.IsEmpty() ) {
			targetID = e->name;
		} else {
			// targetID.Format( "inst_%i_d%i", e->characterID, depth );
			targetID.Format( "%i", e->characterID );
		}

#if 1
		// animate opacity track
		if( e->opacityFrames.Num() > 1 ) {
			file->WriteFloatString( "\t%s<animate xlink:href=\"#%s\" attributeName=\"opacity\" values=\"", tabs.c_str(), targetID.c_str() );

			for( int f = 0; f < e->opacityFrames.Num(); f++ ) {
				file->WriteFloatString( "%f", e->opacityFrames[f] );
				if( f < e->opacityFrames.Num() - 1 )
					file->WriteFloatString( ";" );
			}
			file->WriteFloatString( "\" dur=\"%fs\" repeatCount=\"indefinite\" />\n", e->opacityFrames.Num() * frameDur );
			// file->WriteFloatString( "\" dur=\"%fs\" repeatCount=\"1\" restart=\"whenNotActive\" begin=\"%s.mouseover\" />\n", e->opacityFrames.Num() * frameDur, targetID.c_str() );
		}
#endif

#if 0
		// animate transform track
		if (e->matrixFrames.Num() > 1) {
			//file->WriteFloatString("%s\t<animateTransform xlink:href=\"#%s\" attributeName=\"transform\" type=\"matrix\" values=\"", tabs.c_str(), targetID.c_str() );
			file->WriteFloatString("%s\t<animateTransform xlink:href=\"#%s\" attributeName=\"transform\" type=\"translate\" values=\"", tabs.c_str(), targetID.c_str() );

			for (int f = 0; f < e->matrixFrames.Num(); f++) {
				const swfMatrix_t& m = e->matrixFrames[f];
				//file->WriteFloatString("%f %f %f %f %f %f", m.xx, m.yx, m.xy, m.yy, m.tx, m.ty);
				file->WriteFloatString("%f %f", m.tx, m.ty);
				if (f < e->matrixFrames.Num() - 1)
					file->WriteFloatString(";");
			}
			file->WriteFloatString("\" dur=\"%fs\" repeatCount=\"indefinite\" />\n", e->matrixFrames.Num() * frameDur );
			//file->WriteFloatString("\" dur=\"%fs\" repeatCount=\"1\" restart=\"whenNotActive\" begin=\"%s.mouseover\" />\n", e->matrixFrames.Num() * frameDur, targetID.c_str() );
		}
#elif 1
		// animate transform track
		if( e->matrixFrames.Num() > 1 ) {
			file->WriteFloatString( "%s\t<animateTransform xlink:href=\"#%s\" attributeName=\"transform\" type=\"translate\" values=\"", tabs.c_str(), targetID.c_str() );
			for( int f = 0; f < e->matrixFrames.Num(); f++ ) {
				const swfMatrix_t& m = e->matrixFrames[f];
				file->WriteFloatString( "%f %f", m.tx, m.ty );
				if( f < e->matrixFrames.Num() - 1 )
					file->WriteFloatString( ";" );
			}
			file->WriteFloatString( "\" dur=\"%fs\" repeatCount=\"indefinite\" />\n", e->matrixFrames.Num() * frameDur );

			file->WriteFloatString( "%s\t<animateTransform xlink:href=\"#%s\" attributeName=\"transform\" type=\"scale\" additive=\"sum\" values=\"", tabs.c_str(), targetID.c_str() );
			for( int f = 0; f < e->matrixFrames.Num(); f++ ) {
				const swfMatrix_t& m = e->matrixFrames[f];
				file->WriteFloatString( "%f %f", m.xx, m.yy );
				if( f < e->matrixFrames.Num() - 1 )
					file->WriteFloatString( ";" );
			}
			file->WriteFloatString( "\" dur=\"%fs\" repeatCount=\"indefinite\" />\n", e->matrixFrames.Num() * frameDur );
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

void idSWFSprite::PreRun_PlaceObject2( idSWFBitStream& bitstream,
	int												   sourceCharacterID,
	const idStr&									   sourcePrefix,
	int												   commandID,
	const idList<idSWFDictionaryEntry, TAG_SWF>&	   dict,
	idHashTableT<int, svgDisplayEntry_t>&			   characterMap,
	idHashTableT<int, svgDisplayEntry_t*>&			   localDepthMap,
	int												   currentFrame )
{
	uint8 flags1 = bitstream.ReadU8();
	int	  depth	 = bitstream.ReadU16();

	int	  characterID = -1;
	if( ( flags1 & PlaceFlagHasCharacter ) != 0 ) {
		characterID = bitstream.ReadU16();
	}

	idStr			filterID;
	idStr			transform;
	idStr			name;

	swfMatrix_t		localMatrix;
	swfColorXform_t localColor;

	if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
		swfMatrix_t m;
		bitstream.ReadMatrix( m );
		localMatrix = m;
	}

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
			}
		}
	}

	if( ( flags1 & PlaceFlagHasRatio ) != 0 ) {
		uint16 unused = bitstream.ReadU16();
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 ) {
		name = bitstream.ReadString();
	}

	idStr uniqueID;
	if( !name.IsEmpty() ) {
		uniqueID.Format( "%s.%s", sourcePrefix.c_str(), name.c_str() );
	} else {
		uniqueID.Format( "%s.%i", sourcePrefix.c_str(), characterID );
	}

	if( flags1 & PlaceFlagMove ) {
		// update existing object
		svgDisplayEntry_t** entryPtr;
		if( localDepthMap.Get( depth, &entryPtr ) ) {
			svgDisplayEntry_t* entry = *entryPtr;

			// CORRECTION: Fill in the gaps!
			// If we are at the current frame but the list is shorter,
			// copy the last known matrix for the missing frames.
			while( entry->matrixFrames.Num() < currentFrame ) {
				entry->matrixFrames.Append( entry->matrix );
			}

			while( entry->opacityFrames.Num() < currentFrame ) {
				entry->opacityFrames.Append( entry->cxf.mul.w );
			}

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
		entry.name		  = uniqueID;

		entry.matrixFrames.Append( localMatrix );
		entry.opacityFrames.Append( localColor.mul.w );

		characterMap.Set( characterID, entry );

		svgDisplayEntry_t* entryPtr;
		characterMap.Get( characterID, &entryPtr );

		localDepthMap.Set( depth, entryPtr );
	}
}

void idSWFSprite::WriteSVGUnfolded_PlaceObject2( idFile* file,
	idSWFBitStream&										 bitstream,
	int													 sourceCharacterID,
	const idStr&										 sourcePrefix,
	int													 commandID,
	const idList<idSWFDictionaryEntry, TAG_SWF>&		 dict,
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

	if( ( flags1 & PlaceFlagHasRatio ) != 0 ) {
		uint16 unused = bitstream.ReadU16();
	}

	if( ( flags1 & PlaceFlagHasName ) != 0 ) {
		name = bitstream.ReadString();
	}

	// ===========================================================
	// Write into SVG (only for Create!)
	// ===========================================================
	if( characterID == -1 ) {
		return;
	}

	idStr uniqueID;
	if( !name.IsEmpty() ) {
		uniqueID.Format( "%s.%s", sourcePrefix.c_str(), name.c_str() );
	} else {
		uniqueID.Format( "%s.%i", sourcePrefix.c_str(), characterID );
	}

	bool				isAnimated = false;
	svgDisplayEntry_t** entryPtr;
	if( localDepthMap.Get( depth, &entryPtr ) ) {
		svgDisplayEntry_t* entry = *entryPtr;

		if( IsMatrixAnimated( entry->matrixFrames ) ) {
			isAnimated = true;
		}
	}

	const idSWFDictionaryEntry& dictEntry = dict[characterID];
	switch( dictEntry.type ) {
		case SWF_DICT_MORPH:
		case SWF_DICT_SHAPE:
		case SWF_DICT_TEXT:
		case SWF_DICT_EDITTEXT: {
			file->WriteFloatString( "%s<use xlink:href=\"#%i\" link-type=\"%s\" ", tabs.c_str(), characterID, idSWF::GetDictTypeName( dictEntry.type ) );

			if( ( flags1 & PlaceFlagHasMatrix ) != 0 ) {
				file->WriteFloatString( "%s", transform.c_str() );
			}

			if( ( flags1 & PlaceFlagHasColorTransform ) != 0 ) {
				if( !filterID.IsEmpty() ) {
					file->WriteFloatString( "filter=\"url(#%s)\" ", filterID.c_str() );
				}
			}

			if( !name.IsEmpty() ) {
				file->WriteFloatString( "id=\"%s\" ", uniqueID.c_str() );
			}

			file->WriteFloatString( " />\n" );
			break;
		}

		case SWF_DICT_SPRITE: {
			idSWFSprite* sprite = dictEntry.sprite;

			if( !name.IsEmpty() ) {
				file->WriteFloatString( "%s<g id=\"%s\" data-type=\"Tag_PlaceObject2\" ", tabs.c_str(), uniqueID.c_str() );
			} else {
				file->WriteFloatString( "%s<g data-type=\"Tag_PlaceObject2\" ", tabs.c_str() );
			}

			if( ( flags1 & PlaceFlagHasMatrix ) != 0 && !isAnimated ) {
				file->WriteFloatString( "%s", transform.c_str() );
			}

			if( !filterID.IsEmpty() ) {
				file->WriteFloatString( "filter=\"url(#%s)\" ", filterID.c_str() );
			}

			file->WriteFloatString( ">\n" );

			sprite->WriteSVGUnfolded_r( file, characterID, dict, characterMap, frameDur, uniqueID, indent + 1 );

			file->WriteFloatString( "%s</g>\n", tabs.c_str() );
			break;
		}
	}
}
