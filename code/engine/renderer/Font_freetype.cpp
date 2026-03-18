/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015-2026 Robert Beckebans

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
#include "Font.h"

#ifdef BUILD_FREETYPE

	#include <ft2build.h>
	#include FT_FREETYPE_H
	#include FT_OUTLINE_H
	#include FT_IMAGE_H
	#include FT_SYSTEM_H

static FT_Library ftLibrary		= NULL;
const int		  FONT_SIZE		= 512;
const int		  GLYPH_PADDING = 2;

/*
============
R_GetGlyphInfo
============
*/
void			  R_GetGlyphInfo( FT_GlyphSlot glyph, int* left, int* right, int* width, int* top, int* bottom, int* height, int* pitch )
{
	#define _FLOOR( x ) ( ( x ) & -64 )
	#define _CEIL( x )	( ( ( x ) + 63 ) & -64 )
	#define _TRUNC( x ) ( ( x ) >> 6 )

	*left  = _FLOOR( glyph->metrics.horiBearingX );
	*right = _CEIL( glyph->metrics.horiBearingX + glyph->metrics.width );
	*width = _TRUNC( *right - *left );

	*top	= _CEIL( glyph->metrics.horiBearingY );
	*bottom = _FLOOR( glyph->metrics.horiBearingY - glyph->metrics.height );
	*height = _TRUNC( *top - *bottom );

	//*pitch  = ( qtrue ? (*width+3) & -4 : (*width+7) >> 3 );
	*pitch = ( *width + 3 ) & -4;

	#undef _FLOOR
	#undef _CEIL
	#undef _TRUNC
}

/*
============
R_RenderGlyph
============
*/
FT_Bitmap* R_RenderGlyph( FT_GlyphSlot glyph, idFont::glyphInfo_t* glyphOut )
{
	FT_Bitmap* bit2;
	int		   left, right, width, top, bottom, height, pitch, size;

	R_GetGlyphInfo( glyph, &left, &right, &width, &top, &bottom, &height, &pitch );

	if( glyph->format == ft_glyph_format_outline ) {
		size = pitch * height;

		bit2 = ( FT_Bitmap* )Mem_Alloc( sizeof( FT_Bitmap ), TAG_FONT );

		bit2->width		 = width;
		bit2->rows		 = height;
		bit2->pitch		 = pitch;
		bit2->pixel_mode = ft_pixel_mode_grays;
		// bit2->pixel_mode = ft_pixel_mode_mono;
		bit2->num_grays = 256;
		bit2->buffer	= ( byte* )Mem_Alloc( pitch * height, TAG_FONT );
		memset( bit2->buffer, 0, size );

		FT_Outline_Translate( &glyph->outline, -left, -bottom );

		FT_Outline_Get_Bitmap( ftLibrary, &glyph->outline, bit2 );

		glyphOut->height = height;
		glyphOut->width	 = width;
		glyphOut->top	 = ( glyph->metrics.horiBearingY >> 6 );
		glyphOut->left	 = ( left >> 6 );

		return bit2;
	} else {
		common->Printf( "Non-outline fonts are not supported\n" );
	}
	return NULL;
}

/*
============
RE_ConstructGlyphInfo
============
*/
static idFont::glyphInfo_t* RE_ConstructGlyphInfo( unsigned char* imageOut, int* xOut, int* yOut, int* maxHeight, FT_Face face, const uint32 c, bool calcHeight )
{
	int						   i;
	static idFont::glyphInfo_t glyph;
	unsigned char *			   src, *dst;
	float					   scaled_width, scaled_height;
	FT_Bitmap*				   bitmap = NULL;

	memset( &glyph, 0, sizeof( idFont::glyphInfo_t ) );

	// make sure everything is here
	if( face != NULL ) {
		FT_Load_Glyph( face, FT_Get_Char_Index( face, c ), FT_LOAD_DEFAULT );
		bitmap = R_RenderGlyph( face->glyph, &glyph );
		if( bitmap ) {
			glyph.xSkip = ( face->glyph->metrics.horiAdvance >> 6 );
		} else {
			return &glyph;
		}

		int paddedHeight = ( glyph.height > 0 ) ? ( glyph.height + ( GLYPH_PADDING * 2 ) ) : 0;
		if( paddedHeight > *maxHeight ) {
			*maxHeight = paddedHeight;
		}

		if( calcHeight ) {
			Mem_Free( bitmap->buffer );
			Mem_Free( bitmap );
			return &glyph;
		}

		/*
				// need to convert to power of 2 sizes so we do not get
				// any scaling from the gl upload
				for (scaled_width = 1 ; scaled_width < glyph.pitch ; scaled_width<<=1)
					;
				for (scaled_height = 1 ; scaled_height < glyph.height ; scaled_height<<=1)
					;
		*/

		scaled_width  = ( glyph.width > 0 ) ? ( glyph.width + ( GLYPH_PADDING * 2 ) ) : 0;
		scaled_height = ( glyph.height > 0 ) ? ( glyph.height + ( GLYPH_PADDING * 2 ) ) : 0;

		// we need to make sure we fit
		// RB: changed constants to FONT_SIZE -1
		if( *xOut + scaled_width + 1 >= ( FONT_SIZE - 1 ) ) {
			if( *yOut + ( *maxHeight + 1 ) * 2 >= ( FONT_SIZE - 1 ) ) {
				*yOut = -1;
				*xOut = -1;
				Mem_Free( bitmap->buffer );
				Mem_Free( bitmap );
				return &glyph;
			} else {
				*xOut = 0;
				*yOut += *maxHeight + 1;
			}
		} else if( *yOut + *maxHeight + 1 >= ( FONT_SIZE - 1 ) ) {
			*yOut = -1;
			*xOut = -1;
			Mem_Free( bitmap->buffer );
			Mem_Free( bitmap );
			return &glyph;
		}

		src = bitmap->buffer;
		if( glyph.width > 0 && glyph.height > 0 ) {
			dst = imageOut + ( ( *yOut + GLYPH_PADDING ) * FONT_SIZE ) + ( *xOut + GLYPH_PADDING );
		} else {
			dst = imageOut + ( *yOut * FONT_SIZE ) + *xOut;
		}

		if( bitmap->pixel_mode == ft_pixel_mode_mono ) {
			for( i = 0; i < glyph.height; i++ ) {
				int			   j;
				unsigned char* _src = src;
				unsigned char* _dst = dst;
				unsigned char  mask = 0x80;
				unsigned char  val	= *_src;
				for( j = 0; j < glyph.width; j++ ) {
					if( mask == 0x80 ) {
						val = *_src++;
					}
					if( val & mask ) {
						*_dst = 0xff;
					}
					mask >>= 1;

					if( mask == 0 ) {
						mask = 0x80;
					}
					_dst++;
				}

				src += bitmap->pitch;
				dst += FONT_SIZE;
			}
		} else {
			for( i = 0; i < glyph.height; i++ ) {
				memcpy( dst, src, bitmap->width );
				src += bitmap->pitch;
				dst += FONT_SIZE;
			}
		}

		// Edge extrusion into the padding to prevent bilinear bleed
		if( glyph.width > 0 && glyph.height > 0 && GLYPH_PADDING > 0 ) {
			const int baseX = *xOut + GLYPH_PADDING;
			const int baseY = *yOut + GLYPH_PADDING;

			for( int p = 1; p <= GLYPH_PADDING; p++ ) {
				// top and bottom rows
				unsigned char* srcTop = imageOut + ( baseY * FONT_SIZE ) + baseX;
				unsigned char* dstTop = imageOut + ( ( baseY - p ) * FONT_SIZE ) + baseX;
				memcpy( dstTop, srcTop, glyph.width );

				unsigned char* srcBottom = imageOut + ( ( baseY + glyph.height - 1 ) * FONT_SIZE ) + baseX;
				unsigned char* dstBottom = imageOut + ( ( baseY + glyph.height - 1 + p ) * FONT_SIZE ) + baseX;
				memcpy( dstBottom, srcBottom, glyph.width );

				// left and right columns
				for( int y = 0; y < glyph.height; y++ ) {
					unsigned char* row		 = imageOut + ( ( baseY + y ) * FONT_SIZE ) + baseX;
					row[-p]					 = row[0];
					row[glyph.width - 1 + p] = row[glyph.width - 1];
				}
			}
		}

		// we now have an 8 bit per pixel grey scale bitmap
		// that is width wide and pf->ftSize->metrics.y_ppem tall

		//		glyph.imageHeight = scaled_height;
		//		glyph.imageWidth = scaled_width;

		// RB: BFG glyphInfo_t uses pixel coordinates (uint16) not float texture coords
		if( glyph.width > 0 && glyph.height > 0 ) {
			glyph.s = ( uint16 )( *xOut + GLYPH_PADDING );
			glyph.t = ( uint16 )( *yOut + GLYPH_PADDING );
		} else {
			glyph.s = ( uint16 )*xOut;
			glyph.t = ( uint16 )*yOut;
		}

		*xOut += scaled_width + 1;
	}

	Mem_Free( bitmap->buffer );
	Mem_Free( bitmap );

	return &glyph;
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

	if( ftLibrary == NULL ) {
		if( FT_Init_FreeType( &ftLibrary ) ) {
			common->Printf( "LoadFromTrueTypeFont: Unable to initialize FreeType.\n" );
			fileSystem->FreeFile( faceData );
			return false;
		}
	}

	FT_Face face;
	if( FT_New_Memory_Face( ftLibrary, ( const FT_Byte* )faceData, len, 0, &face ) ) {
		common->Printf( "LoadFromTrueTypeFont: FreeType2, unable to allocate new face.\n" );
		fileSystem->FreeFile( faceData );
		return false;
	}

	float dpi		= 72;
	short pointSize = 48;

	if( FT_Set_Char_Size( face, pointSize << 6, pointSize << 6, dpi, dpi ) ) {
		common->Printf( "LoadFromTrueTypeFont: FreeType2, Unable to set face char size.\n" );
		FT_Done_Face( face );
		fileSystem->FreeFile( faceData );
		return false;
	}

	// ---------------------------------------------------------------
	// Step 1: Enumerate which Unicode codepoints actually exist in the
	// font. We scan all of Unicode via FT_Get_First_Char / FT_Get_Next_Char,
	// collecting codepoints into a sorted list. This mirrors how the
	// original BFG tool builds its sparse charIndex array.
	// ---------------------------------------------------------------
	idList<uint32> presentChars;
	{
		FT_UInt	 glyphIdx;
		FT_ULong charCode = FT_Get_First_Char( face, &glyphIdx );
		while( glyphIdx != 0 ) {
			presentChars.Append( ( uint32 )charCode );
			charCode = FT_Get_Next_Char( face, charCode, &glyphIdx );
		}
	}

	int numGlyphs = presentChars.Num();
	if( numGlyphs == 0 ) {
		common->Warning( "LoadFromTrueTypeFont: Font '%s' contains no glyphs", GetName() );
		FT_Done_Face( face );
		fileSystem->FreeFile( faceData );
		return false;
	}

	common->Printf( "LoadFromTrueTypeFont: Font '%s' has %d glyphs\n", GetName(), numGlyphs );

	// ---------------------------------------------------------------
	// Step 2: Allocate atlas and glyph storage
	// ---------------------------------------------------------------
	byte* out = ( byte* )Mem_Alloc( FONT_SIZE * FONT_SIZE, TAG_FONT );
	if( out == NULL ) {
		common->Printf( "LoadFromTrueTypeFont: Mem_Alloc failure during output image creation.\n" );
		FT_Done_Face( face );
		fileSystem->FreeFile( faceData );
		return false;
	}
	memset( out, 0, FONT_SIZE * FONT_SIZE );

	fontInfo = new( TAG_FONT ) fontInfo_t;
	memset( fontInfo, 0, sizeof( fontInfo_t ) );

	const FT_Fixed yScale = face->size->metrics.y_scale;
	const int	   asc26  = FT_MulFix( face->ascender, yScale );
	const int	   desc26 = FT_MulFix( face->descender, yScale );

	const int	   asc26Ceil   = ( asc26 + 63 ) & -64;
	const int	   desc26Floor = desc26 & -64;

	fontInfo->ascender	= ( short )( asc26Ceil >> 6 );
	fontInfo->descender = ( short )( desc26Floor >> 6 );

	fontInfo->numGlyphs = ( short )numGlyphs;
	fontInfo->glyphData = ( glyphInfo_t* )Mem_Alloc( sizeof( glyphInfo_t ) * numGlyphs, TAG_FONT );
	fontInfo->charIndex = ( uint32* )Mem_Alloc( sizeof( uint32 ) * numGlyphs, TAG_FONT );
	memset( fontInfo->glyphData, 0, sizeof( glyphInfo_t ) * numGlyphs );
	memset( fontInfo->charIndex, 0, sizeof( uint32 ) * numGlyphs );
	memset( fontInfo->ascii, -1, sizeof( fontInfo->ascii ) );

	// ---------------------------------------------------------------
	// Step 3: First pass - calculate maxHeight for row-packing
	// Only iterate over characters that actually exist in the font.
	// ---------------------------------------------------------------
	int xOut	  = 0;
	int yOut	  = 0;
	int maxHeight = 0;
	for( int i = 0; i < numGlyphs; i++ ) {
		RE_ConstructGlyphInfo( out, &xOut, &yOut, &maxHeight, face, presentChars[i], true );
	}

	// ---------------------------------------------------------------
	// Step 4: Second pass - render glyphs into the atlas
	// ---------------------------------------------------------------
	xOut = 0;
	yOut = 0;

	for( int i = 0; i < numGlyphs; i++ ) {
		uint32		 charCode = presentChars[i];

		glyphInfo_t* glyph = RE_ConstructGlyphInfo( out, &xOut, &yOut, &maxHeight, face, charCode, false );

		if( xOut == -1 || yOut == -1 ) {
			common->Warning( "LoadFromTrueTypeFont: Font atlas overflow for '%s' at glyph %d (char %u)", GetName(), i, charCode );

			// Fill remaining glyphs with empty data
			for( int j = i; j < numGlyphs; j++ ) {
				memset( &fontInfo->glyphData[j], 0, sizeof( glyphInfo_t ) );
				fontInfo->charIndex[j] = presentChars[j];
			}
			break;
		}

		fontInfo->glyphData[i] = *glyph;
		fontInfo->charIndex[i] = charCode;

		// Build the ASCII fast-lookup table: ascii[codepoint] = glyph index
		if( charCode < 128 ) {
			fontInfo->ascii[charCode] = ( char )i;
		}
	}

	// ---------------------------------------------------------------
	// Step 5: Convert grayscale atlas to RGBA and write TGA
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
	// Step 6: Register material and compute oldInfo metrics
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

	FT_Done_Face( face );
	fileSystem->FreeFile( faceData );

	common->Printf( "LoadFromTrueTypeFont: Successfully generated font '%s' from TTF (%d glyphs)\n", GetName(), numGlyphs );

	DumpFontToJSON();

	return true;
}

#endif // BUILD_FREETYPE
