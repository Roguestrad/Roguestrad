/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2026 Robert Beckebans

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
#include "../tr_local.h"
#include "engine/renderer/Font.h"

#ifndef BUILD_FREETYPE

	// stb_truetype — single-header TrueType rasterizer (replaces FreeType)
	// We define STBTT_STATIC so that all stb symbols are file-local and won't
	// conflict with any other translation units that also include stb_truetype
	// (e.g. Dear ImGui).
	#define STBTT_STATIC
	#define STB_TRUETYPE_IMPLEMENTATION
	#define STBTT_assert( x ) assert( x )
	#include "libs/stb/stb_truetype.h"

// =========================================================================
// stb_truetype-based TrueType font generation
// =========================================================================

static const int FONT_SIZE	   = 1024;
static const int GLYPH_PADDING = 2;

	// FreeType 26.6 fixed-point helpers -- replicate the rounding that
	// FreeType applies to glyph metrics so our width / height / top / left
	// values match the reference output as closely as possible.
	#define FT26_FLOOR( x ) ( ( x ) & -64 )
	#define FT26_CEIL( x )	( ( ( x ) + 63 ) & -64 )
	#define FT26_TRUNC( x ) ( ( x ) >> 6 )
	#define FT26_ROUND( x ) FT26_FLOOR( ( x ) + 32 )

//! Converts a float value to 26.6 fixed point format.
static inline int FloatTo26_6( float v )
{
	return ( int )( v * 64.0f + ( v >= 0.0f ? 0.5f : -0.5f ) );
}

//! Fills a list with codepoints that are present in the given font.
static void STB_EnumerateCodepoints( const stbtt_fontinfo* font, idList<uint32>& outChars )
{
	outChars.Clear();

	const int count = ( int )( sizeof( BFG_CHARSET ) / sizeof( BFG_CHARSET[0] ) );
	for( int i = 0; i < count; i++ ) {
		uint32 cp = BFG_CHARSET[i];
		if( stbtt_FindGlyphIndex( font, ( int )cp ) != 0 ) {
			outChars.Append( cp );
		}
	}
}

bool idFont::LoadFromTrueTypeFont()
{
	idStr ttfPath = va( "newfonts/%s", GetName() );
	ttfPath.SetFileExtension( ".ttf" );

	void*	  faceData;
	ID_TIME_T ftime;

	int		  len = fileSystem->ReadFile( ttfPath, &faceData, &ftime );
	if( len <= 0 ) {
		common->Printf( "LoadFromTrueTypeFont: Unable to read font file: '%s'\n", ttfPath.c_str() );
		return false;
	}

	// ---------------------------------------------------------------
	// Initialize stb_truetype
	// ---------------------------------------------------------------
	stbtt_fontinfo stbFont;
	memset( &stbFont, 0, sizeof( stbFont ) );

	int fontOffset = stbtt_GetFontOffsetForIndex( ( const unsigned char* )faceData, 0 );
	if( fontOffset < 0 ) {
		common->Printf( "LoadFromTrueTypeFont: Not a valid font file: '%s'\n", ttfPath.c_str() );
		fileSystem->FreeFile( faceData );
		return false;
	}

	if( !stbtt_InitFont( &stbFont, ( const unsigned char* )faceData, fontOffset ) ) {
		common->Printf( "LoadFromTrueTypeFont: stbtt_InitFont failed for '%s'\n", ttfPath.c_str() );
		fileSystem->FreeFile( faceData );
		return false;
	}

	// We want a 48-point font. FreeType's FT_Set_Char_Size with 72 DPI maps
	// points directly to the EM square (48pt at 72dpi = 48 pixels per EM).
	// stbtt_ScaleForMappingEmToPixels matches this behavior, whereas
	// stbtt_ScaleForPixelHeight would scale based on ascent-descent which
	// produces smaller glyphs.
	const float	   pixelHeight = 48.0f;
	float		   scale	   = stbtt_ScaleForMappingEmToPixels( &stbFont, pixelHeight );

	// ---------------------------------------------------------------
	// Step 1: Enumerate which Unicode codepoints actually exist in the
	// font by probing stbtt_FindGlyphIndex across the Unicode range.
	// ---------------------------------------------------------------
	idList<uint32> presentChars;
	STB_EnumerateCodepoints( &stbFont, presentChars );

	int numGlyphs = presentChars.Num();
	if( numGlyphs == 0 ) {
		common->Warning( "LoadFromTrueTypeFont: Font '%s' contains no glyphs", GetName() );
		fileSystem->FreeFile( faceData );
		return false;
	}

	common->Printf( "LoadFromTrueTypeFont: Font '%s' has %d glyphs\n", GetName(), numGlyphs );

	// ---------------------------------------------------------------
	// Step 2: Retrieve font vertical metrics (ascender / descender)
	// ---------------------------------------------------------------
	int stbAscent, stbDescent, stbLineGap;
	stbtt_GetFontVMetrics( &stbFont, &stbAscent, &stbDescent, &stbLineGap );

	// ---------------------------------------------------------------
	// Step 3: Allocate atlas and glyph storage
	// ---------------------------------------------------------------
	byte* out = ( byte* )Mem_Alloc( FONT_SIZE * FONT_SIZE, TAG_FONT );
	if( out == NULL ) {
		common->Printf( "LoadFromTrueTypeFont: Mem_Alloc failure during output image creation.\n" );
		fileSystem->FreeFile( faceData );
		return false;
	}
	memset( out, 0, FONT_SIZE * FONT_SIZE );

	fontInfo = new( TAG_FONT ) fontInfo_t;
	memset( fontInfo, 0, sizeof( fontInfo_t ) );

	// FreeType computes ascender/descender from face->size->metrics which are
	// rounded to whole pixels via ceil/floor respectively in 26.6 fixed point.
	// We replicate that by rounding ascender up and descender down.
	const int asc26		  = FloatTo26_6( ( float )stbAscent * scale );
	const int desc26	  = FloatTo26_6( ( float )stbDescent * scale );
	const int asc26Ceil	  = FT26_CEIL( asc26 );
	const int desc26Floor = FT26_FLOOR( desc26 );

	fontInfo->ascender	= ( short )FT26_TRUNC( asc26Ceil );
	fontInfo->descender = ( short )FT26_TRUNC( desc26Floor );

	fontInfo->numGlyphs = ( short )numGlyphs;
	fontInfo->glyphData = ( glyphInfo_t* )Mem_Alloc( sizeof( glyphInfo_t ) * numGlyphs, TAG_FONT );
	fontInfo->charIndex = ( uint32* )Mem_Alloc( sizeof( uint32 ) * numGlyphs, TAG_FONT );
	memset( fontInfo->glyphData, 0, sizeof( glyphInfo_t ) * numGlyphs );
	memset( fontInfo->charIndex, 0, sizeof( uint32 ) * numGlyphs );
	memset( fontInfo->ascii, -1, sizeof( fontInfo->ascii ) );

	// ---------------------------------------------------------------
	// Step 4: First pass -- calculate glyph metrics using FreeType-
	// compatible 26.6 fixed-point rounding and find maxHeight for
	// row-packing.
	//
	// FreeType's R_GetGlyphInfo computes:
	//   left   = FLOOR(horiBearingX)
	//   right  = CEIL(horiBearingX + width)
	//   width  = TRUNC(right - left)
	//   top    = CEIL(horiBearingY)
	//   bottom = FLOOR(horiBearingY - height)
	//   height = TRUNC(top - bottom)
	//   pitch  = (width + 3) & ~3
	//
	// And R_RenderGlyph sets:
	//   glyphOut->top = (horiBearingY >> 6) + 1
	//
	// We replicate this by converting the scaled float metrics to
	// 26.6 fixed-point and applying the same FLOOR/CEIL/TRUNC ops.
	//
	// Without hinting the outline bounds come from the glyf table
	// (stbtt_GetGlyphBox).  FreeType's unhinted "glyph metrics" are
	// derived from these plus the hmtx bearings:
	//   horiBearingX  = leftSideBearing  (font units, scaled)
	//   horiBearingY  = yMax             (font units, scaled)
	//   metric width  = xMax - xMin      (font units, scaled)
	//   metric height = yMax - yMin      (font units, scaled)
	//   horiAdvance   = advanceWidth     (font units, scaled)
	// ---------------------------------------------------------------

	// Temporary struct for pre-computed per-glyph metrics
	struct glyphMetrics_t {
		int	 ftTop;	  // vertical position: best FreeType-compatible approximation
		byte ftXSkip; // horizontal advance: (horiAdvance >> 6)
		int	 bmpW;	  // actual bitmap width for rendering (from STB bitmap box)
		int	 bmpH;	  // actual bitmap height for rendering
		int	 bmpIx0;  // STB bitmap box ix0 (left bearing for atlas & gi.left)
		int	 bmpIy0;  // STB bitmap box iy0 (for rendering offset)
	};

	glyphMetrics_t* gm = ( glyphMetrics_t* )Mem_Alloc( sizeof( glyphMetrics_t ) * numGlyphs, TAG_FONT );
	memset( gm, 0, sizeof( glyphMetrics_t ) * numGlyphs );

	int maxHeight = 0;
	for( int i = 0; i < numGlyphs; i++ ) {
		uint32 charCode = presentChars[i];

		// Get horizontal metrics in font units
		int	   advanceWidth, leftSideBearing;
		stbtt_GetCodepointHMetrics( &stbFont, ( int )charCode, &advanceWidth, &leftSideBearing );

		// Get glyph outline bounding box in font units
		int gx0 = 0, gy0 = 0, gx1 = 0, gy1 = 0;
		int glyphIdx = stbtt_FindGlyphIndex( &stbFont, ( int )charCode );
		stbtt_GetGlyphBox( &stbFont, glyphIdx, &gx0, &gy0, &gx1, &gy1 );

		// Scale to pixel coordinates and convert to 26.6 fixed-point.
		//
		// FreeType's glyph->metrics for unhinted TrueType outlines:
		//   horiBearingX = xMin of the outline (from glyf table, NOT lsb from hmtx)
		//   horiBearingY = yMax of the outline
		//   width        = xMax - xMin
		//   height       = yMax - yMin
		//   horiAdvance  = advanceWidth (from hmtx)
		//
		// We use gx0 (xMin) for horiBearingX rather than leftSideBearing
		// because FreeType derives horiBearingX from the actual outline
		// bounds after scaling, and for some glyphs xMin != lsb.
		int horiBearingX_26_6 = FloatTo26_6( ( float )gx0 * scale );
		int metricWidth_26_6  = FloatTo26_6( ( float )( gx1 - gx0 ) * scale );
		int horiBearingY_26_6 = FloatTo26_6( ( float )gy1 * scale );
		int metricHeight_26_6 = FloatTo26_6( ( float )( gy1 - gy0 ) * scale );
		int horiAdvance_26_6  = FloatTo26_6( ( float )advanceWidth * scale );

		// Apply FreeType's R_GetGlyphInfo rounding
		int ftLeft26  = FT26_FLOOR( horiBearingX_26_6 );
		int ftRight26 = FT26_CEIL( horiBearingX_26_6 + metricWidth_26_6 );
		int ftWidth	  = FT26_TRUNC( ftRight26 - ftLeft26 );

		int ftTop26	   = FT26_CEIL( horiBearingY_26_6 );
		int ftBottom26 = FT26_FLOOR( horiBearingY_26_6 - metricHeight_26_6 );
		int ftHeight   = FT26_TRUNC( ftTop26 - ftBottom26 );

		int ftPitch = ( ftWidth + 3 ) & ~3;

		// FreeType's R_RenderGlyph used: top = (horiBearingY >> 6) + 1
		// However, FreeType's horiBearingY was grid-fitted by hinting.
		// Without hinting the best approximation is TRUNC(CEIL(horiBearingY_26_6))
		// which rounds the fractional 26.6 value up to the next pixel boundary
		// before truncating.  Testing against the FreeType reference shows this
		// produces the highest match rate (34/55 vs 33/55 for TRUNC+1 and
		// 21/55 for the old ceil(float)+1 method).
		//
		// Special case: for empty glyphs (e.g. space, NBSP) that have no
		// outline, horiBearingY is 0 and the formula yields 0.  FreeType's
		// R_RenderGlyph would return (0 >> 6) + 1 = 1, so we match that.
		int ftTop;
		if( gx0 == 0 && gy0 == 0 && gx1 == 0 && gy1 == 0 ) {
			ftTop = 1; // empty glyph: replicate FreeType's (0 >> 6) + 1
		} else {
			ftTop = FT26_TRUNC( FT26_CEIL( horiBearingY_26_6 ) );
		}

		// RE_ConstructGlyphInfo: xSkip = (horiAdvance >> 6) + 1
		byte ftXSkip = ( byte )( FT26_TRUNC( horiAdvance_26_6 ) );

		// left = FLOOR(horiBearingX) in pixels
		int	 ftLeftPx = FT26_TRUNC( ftLeft26 );

		// Also get STB's actual bitmap box for rendering purposes.
		// We render using STB, so we need its exact pixel dimensions.
		int	 bix0, biy0, bix1, biy1;
		stbtt_GetCodepointBitmapBox( &stbFont, ( int )charCode, scale, scale, &bix0, &biy0, &bix1, &biy1 );
		int bmpW = bix1 - bix0;
		int bmpH = biy1 - biy0;

		gm[i].ftTop	  = ftTop;
		gm[i].ftXSkip = ftXSkip;
		gm[i].bmpW	  = bmpW;
		gm[i].bmpH	  = bmpH;
		gm[i].bmpIx0  = bix0;
		gm[i].bmpIy0  = biy0;

		int paddedHeight = ( bmpH > 0 ) ? ( bmpH + ( GLYPH_PADDING * 2 ) ) : 0;
		if( paddedHeight > maxHeight ) {
			maxHeight = paddedHeight;
		}
	}

	// ---------------------------------------------------------------
	// Step 5: Second pass -- render glyphs into the atlas
	// ---------------------------------------------------------------
	int xOut = 0;
	int yOut = 0;

	for( int i = 0; i < numGlyphs; i++ ) {
		uint32 charCode = presentChars[i];

		int	   bmpW = gm[i].bmpW;
		int	   bmpH = gm[i].bmpH;

		int	   paddedW = ( bmpW > 0 ) ? ( bmpW + ( GLYPH_PADDING * 2 ) ) : 0;
		int	   paddedH = ( bmpH > 0 ) ? ( bmpH + ( GLYPH_PADDING * 2 ) ) : 0;

		// Check if the glyph fits in the atlas; advance row if needed
		if( bmpW > 0 && bmpH > 0 ) {
			if( xOut + paddedW + 1 >= ( FONT_SIZE - 1 ) ) {
				if( yOut + ( maxHeight + 1 ) * 2 >= ( FONT_SIZE - 1 ) ) {
					// Atlas overflow
					common->Warning( "LoadFromTrueTypeFont: Font atlas overflow for '%s' at glyph %d (char %u)", GetName(), i, charCode );

					// Fill remaining glyphs with empty data
					for( int j = i; j < numGlyphs; j++ ) {
						memset( &fontInfo->glyphData[j], 0, sizeof( glyphInfo_t ) );
						fontInfo->charIndex[j] = presentChars[j];
					}
					break;
				} else {
					xOut = 0;
					yOut += maxHeight + 1;
				}
			} else if( yOut + maxHeight + 1 >= ( FONT_SIZE - 1 ) ) {
				// Atlas overflow
				common->Warning( "LoadFromTrueTypeFont: Font atlas overflow for '%s' at glyph %d (char %u)", GetName(), i, charCode );

				for( int j = i; j < numGlyphs; j++ ) {
					memset( &fontInfo->glyphData[j], 0, sizeof( glyphInfo_t ) );
					fontInfo->charIndex[j] = presentChars[j];
				}
				break;
			}

			const int renderX = xOut + GLYPH_PADDING;
			const int renderY = yOut + GLYPH_PADDING;

			// Render the glyph bitmap into the atlas using STB.
			// We render at the exact STB bitmap dimensions (bmpW x bmpH)
			// and store those same dimensions in gi.width / gi.height so
			// that the renderer's texture quad matches the actual pixel
			// data — no oversized quads that would cause smearing.
			stbtt_MakeCodepointBitmap( &stbFont,
				out + ( renderY * FONT_SIZE ) + renderX,
				bmpW,
				bmpH,
				FONT_SIZE, // stride = atlas width
				scale,
				scale,
				( int )charCode );

	#if 0
			// Edge extrusion into the padding to prevent bilinear bleed
			if( GLYPH_PADDING > 0 ) {
				const int baseX = renderX;
				const int baseY = renderY;

				for( int p = 1; p <= GLYPH_PADDING; p++ ) {
					// top and bottom rows
					byte* srcTop = out + ( baseY * FONT_SIZE ) + baseX;
					byte* dstTop = out + ( ( baseY - p ) * FONT_SIZE ) + baseX;
					memcpy( dstTop, srcTop, bmpW );

					byte* srcBottom = out + ( ( baseY + bmpH - 1 ) * FONT_SIZE ) + baseX;
					byte* dstBottom = out + ( ( baseY + bmpH - 1 + p ) * FONT_SIZE ) + baseX;
					memcpy( dstBottom, srcBottom, bmpW );

					// left and right columns
					for( int y = 0; y < bmpH; y++ ) {
						byte* row		  = out + ( ( baseY + y ) * FONT_SIZE ) + baseX;
						row[-p]			  = row[0];
						row[bmpW - 1 + p] = row[bmpW - 1];
					}
				}
			}
	#endif
		}

		// Fill in glyph metrics.
		// width/height/left come directly from the STB bitmap box so
		// the renderer's texture quad exactly covers the rendered pixels.
		// top and xSkip use the FreeType-compatible 26.6 values for
		// correct baseline positioning and horizontal advance.
		glyphInfo_t& gi = fontInfo->glyphData[i];
		gi.width		= ( byte )bmpW;
		gi.height		= ( byte )bmpH;
		gi.top			= ( signed char )gm[i].ftTop;
		gi.left			= ( signed char )gm[i].bmpIx0;
		gi.xSkip		= gm[i].ftXSkip;
		if( bmpW > 0 && bmpH > 0 ) {
			gi.s = ( uint16 )( xOut + GLYPH_PADDING );
			gi.t = ( uint16 )( yOut + GLYPH_PADDING );
		} else {
			gi.s = ( uint16 )xOut;
			gi.t = ( uint16 )yOut;
		}

		fontInfo->charIndex[i] = charCode;

		// Build the ASCII fast-lookup table: ascii[codepoint] = glyph index
		if( charCode < 128 ) {
			fontInfo->ascii[charCode] = ( char )i;
		}

		if( bmpW > 0 && bmpH > 0 ) {
			xOut += paddedW + 1;
		}
	}

	Mem_Free( gm );

	// ---------------------------------------------------------------
	// Step 6: Convert grayscale atlas to RGBA and write TGA
	// ---------------------------------------------------------------
	int	  scaledSize = FONT_SIZE * FONT_SIZE;
	int	  newSize	 = scaledSize * 4;
	byte* imageBuff	 = ( byte* )Mem_Alloc( newSize, TAG_FONT );

	float max = 0;
	for( int k = 0; k < scaledSize; k++ ) {
		if( max < out[k] ) {
			max = out[k];
		}
	}
	if( max > 0 ) {
		max = 255.0f / max;
	}

	for( int k = 0; k < scaledSize; k++ ) {
		imageBuff[k * 4 + 0] = 255;
		imageBuff[k * 4 + 1] = 255;
		imageBuff[k * 4 + 2] = 255;
		imageBuff[k * 4 + 3] = ( byte )( ( float )out[k] * max );
	}

	idStr fontTextureName = va( "newfonts/%s/48.tga", GetName() );
	R_WriteTGA( fontTextureName.c_str(), imageBuff, FONT_SIZE, FONT_SIZE, false, "fs_basepath" );

	Mem_Free( imageBuff );
	Mem_Free( out );

	// ---------------------------------------------------------------
	// Step 7: Register material and compute oldInfo metrics
	// ---------------------------------------------------------------
	fontInfo->material = declManager->FindMaterial( fontTextureName );
	fontInfo->material->SetSort( SS_GUI );

	int mh = 0;
	int mw = 0;
	for( int g = 0; g < fontInfo->numGlyphs; g++ ) {
		if( mh < fontInfo->glyphData[g].height ) {
			mh = fontInfo->glyphData[g].height;
		}
		if( mw < fontInfo->glyphData[g].xSkip ) {
			mw = fontInfo->glyphData[g].xSkip;
		}
	}
	fontInfo->oldInfo[0].maxWidth  = mw;
	fontInfo->oldInfo[0].maxHeight = mh;
	fontInfo->oldInfo[1].maxWidth  = mw;
	fontInfo->oldInfo[1].maxHeight = mh;
	fontInfo->oldInfo[2].maxWidth  = mw;
	fontInfo->oldInfo[2].maxHeight = mh;

	fileSystem->FreeFile( faceData );

	common->Printf( "LoadFromTrueTypeFont: Successfully generated font '%s' from TTF (%d glyphs)\n", GetName(), numGlyphs );

	DumpFontToJSON();

	return true;
}

#endif // !BUILD_FREETYPE
