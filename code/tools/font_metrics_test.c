/*
 * font_metrics_test.c
 *
 * Standalone test tool that loads a TTF via stb_truetype and compares
 * two metric-calculation approaches:
 *
 *   1) "Old STB" method  – uses stbtt_GetCodepointBitmapBox directly,
 *      then pitch = (gw+3)&~3, top = -iy0 + 1, left = ix0,
 *      xSkip = (advance*scale + 1.5f).
 *
 *   2) "FT26.6" method   – derives metrics from the raw font-unit
 *      glyph box (stbtt_GetGlyphBox) and horizontal metrics
 *      (stbtt_GetCodepointHMetrics), converts to 26.6 fixed-point,
 *      then applies the same FLOOR / CEIL / TRUNC rounding that
 *      FreeType's R_GetGlyphInfo and R_RenderGlyph use.
 *
 * The output is a side-by-side comparison for every glyph found in
 * the font, plus a summary of the differences.
 *
 * Build (MSVC, from repo root):
 *   cl /nologo /W3 /O2 code\tools\font_metrics_test.c /Fe:font_metrics_test.exe
 *
 * Build (GCC / Clang):
 *   gcc -O2 -o font_metrics_test code/tools/font_metrics_test.c -lm
 *
 * Usage:
 *   font_metrics_test path/to/font.ttf [pointSize]
 *
 * If pointSize is omitted it defaults to 48 (matching BFG's pipeline).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* stb_truetype – single-file include                                 */
/* ------------------------------------------------------------------ */
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "../libs/stb/stb_truetype.h"

/* ------------------------------------------------------------------ */
/* FreeType 26.6 fixed-point helpers                                  */
/* ------------------------------------------------------------------ */
#define FT26_FLOOR( x ) ( ( x ) & -64 )
#define FT26_CEIL( x )	( ( ( x ) + 63 ) & -64 )
#define FT26_TRUNC( x ) ( ( x ) >> 6 )
#define FT26_ROUND( x ) FT26_FLOOR( ( x ) + 32 )

static int FloatTo26_6( float v )
{
	return ( int )( v * 64.0f + ( v >= 0.0f ? 0.5f : -0.5f ) );
}

/* ------------------------------------------------------------------ */
/* Per-glyph metric structs                                           */
/* ------------------------------------------------------------------ */
typedef struct {
	int charCode;
	int width;
	int height;
	int top;
	int left;
	int xSkip;
	int pitch;
} GlyphMetrics;

/* ------------------------------------------------------------------ */
/* Compute metrics the "old STB" way (matches the previous Font.cpp)  */
/* ------------------------------------------------------------------ */
static GlyphMetrics CalcOldSTB( const stbtt_fontinfo* font, int cp, float scale )
{
	GlyphMetrics m;
	memset( &m, 0, sizeof( m ) );
	m.charCode = cp;

	int ix0, iy0, ix1, iy1;
	stbtt_GetCodepointBitmapBox( font, cp, scale, scale, &ix0, &iy0, &ix1, &iy1 );
	int gw	  = ix1 - ix0;
	int gh	  = iy1 - iy0;
	int pitch = ( gw + 3 ) & ~3;

	int advanceWidth, leftSideBearing;
	stbtt_GetCodepointHMetrics( font, cp, &advanceWidth, &leftSideBearing );

	m.width	 = pitch;
	m.height = gh;
	m.top	 = -iy0 + 1;
	m.left	 = ix0;
	m.xSkip	 = ( int )( advanceWidth * scale + 1.5f );
	m.pitch	 = pitch;
	return m;
}

/* ------------------------------------------------------------------ */
/* Top-calculation strategy enum                                       */
/* ------------------------------------------------------------------ */
typedef enum {
	TOP_CEIL_PLUS1,	   /* ceil(gy1*scale) + 1      -- old STB method */
	TOP_TRUNC26_PLUS1, /* TRUNC(26.6(gy1*scale)) + 1 -- pure 26.6   */
	TOP_ROUND_PLUS1,   /* round(gy1*scale) + 1                       */
	TOP_CEIL26_PLUS1,  /* TRUNC(CEIL(26.6(gy1*scale))) + 1           */
	TOP_CEIL26_NOADD,  /* TRUNC(CEIL(26.6(gy1*scale)))  -- no +1     */
	TOP_ROUND_NOADD,   /* round(gy1*scale)              -- no +1     */
	TOP_STRATEGY_COUNT
} TopStrategy;

static const char* topStrategyNames[TOP_STRATEGY_COUNT] = {
	"ceil+1",
	"trunc26+1",
	"round+1",
	"ceil26+1",
	"ceil26",
	"round",
};

/* ------------------------------------------------------------------ */
/* Compute metrics the "FT 26.6" way with configurable top strategy    */
/* ------------------------------------------------------------------ */
static GlyphMetrics CalcFT26Ex( const stbtt_fontinfo* font, int cp, float scale, TopStrategy topStrat )
{
	GlyphMetrics m;
	memset( &m, 0, sizeof( m ) );
	m.charCode = cp;

	int advanceWidth, leftSideBearing;
	stbtt_GetCodepointHMetrics( font, cp, &advanceWidth, &leftSideBearing );

	int gx0 = 0, gy0 = 0, gx1 = 0, gy1 = 0;
	int glyphIdx = stbtt_FindGlyphIndex( font, cp );
	stbtt_GetGlyphBox( font, glyphIdx, &gx0, &gy0, &gx1, &gy1 );

	/* Convert to 26.6 fixed-point */
	int	  horiBearingX_26_6 = FloatTo26_6( ( float )gx0 * scale );
	int	  metricWidth_26_6	= FloatTo26_6( ( float )( gx1 - gx0 ) * scale );
	int	  horiBearingY_26_6 = FloatTo26_6( ( float )gy1 * scale );
	int	  metricHeight_26_6 = FloatTo26_6( ( float )( gy1 - gy0 ) * scale );
	int	  horiAdvance_26_6	= FloatTo26_6( ( float )advanceWidth * scale );

	/* R_GetGlyphInfo rounding for width/height */
	int	  ftLeft26	= FT26_FLOOR( horiBearingX_26_6 );
	int	  ftRight26 = FT26_CEIL( horiBearingX_26_6 + metricWidth_26_6 );
	int	  ftWidth	= FT26_TRUNC( ftRight26 - ftLeft26 );

	int	  ftTop26	 = FT26_CEIL( horiBearingY_26_6 );
	int	  ftBottom26 = FT26_FLOOR( horiBearingY_26_6 - metricHeight_26_6 );
	int	  ftHeight	 = FT26_TRUNC( ftTop26 - ftBottom26 );

	int	  ftPitch = ( ftWidth + 3 ) & ~3;

	/* Top calculation -- various strategies */
	float scaledY = ( float )gy1 * scale;
	int	  ftTop	  = 0;
	switch( topStrat ) {
		case TOP_CEIL_PLUS1:
			ftTop = ( int )ceilf( scaledY ) + 1;
			break;
		case TOP_TRUNC26_PLUS1:
			ftTop = FT26_TRUNC( horiBearingY_26_6 ) + 1;
			break;
		case TOP_ROUND_PLUS1:
			ftTop = ( int )roundf( scaledY ) + 1;
			break;
		case TOP_CEIL26_PLUS1:
			ftTop = FT26_TRUNC( FT26_CEIL( horiBearingY_26_6 ) ) + 1;
			break;
		case TOP_CEIL26_NOADD:
			ftTop = FT26_TRUNC( FT26_CEIL( horiBearingY_26_6 ) );
			break;
		case TOP_ROUND_NOADD:
			ftTop = ( int )roundf( scaledY );
			break;
		default:
			ftTop = FT26_TRUNC( horiBearingY_26_6 ) + 1;
			break;
	}

	/* RE_ConstructGlyphInfo: xSkip = (horiAdvance >> 6) + 1 */
	int ftXSkip = FT26_TRUNC( horiAdvance_26_6 ) + 1;

	/* left = FLOOR(horiBearingX) in pixels */
	int ftLeftPx = FT26_TRUNC( ftLeft26 );

	m.width	 = ftPitch;
	m.height = ftHeight;
	m.top	 = ftTop;
	m.left	 = ftLeftPx;
	m.xSkip	 = ftXSkip;
	m.pitch	 = ftPitch;
	return m;
}

static GlyphMetrics CalcFT26( const stbtt_fontinfo* font, int cp, float scale )
{
	return CalcFT26Ex( font, cp, scale, TOP_TRUNC26_PLUS1 );
}

/* ------------------------------------------------------------------ */
/* Also dump the intermediate 26.6 values for a single glyph          */
/* ------------------------------------------------------------------ */
static void DumpFT26Detail( const stbtt_fontinfo* font, int cp, float scale )
{
	int advanceWidth, leftSideBearing;
	stbtt_GetCodepointHMetrics( font, cp, &advanceWidth, &leftSideBearing );

	int gx0 = 0, gy0 = 0, gx1 = 0, gy1 = 0;
	int glyphIdx = stbtt_FindGlyphIndex( font, cp );
	stbtt_GetGlyphBox( font, glyphIdx, &gx0, &gy0, &gx1, &gy1 );

	printf( "      font-units: gx0=%d gy0=%d gx1=%d gy1=%d  lsb=%d adv=%d\n", gx0, gy0, gx1, gy1, leftSideBearing, advanceWidth );

	float fHBX = ( float )gx0 * scale;
	float fW   = ( float )( gx1 - gx0 ) * scale;
	float fHBY = ( float )gy1 * scale;
	float fH   = ( float )( gy1 - gy0 ) * scale;
	float fAdv = ( float )advanceWidth * scale;

	printf( "      scaled-float: hbx=%.4f w=%.4f hby=%.4f h=%.4f adv=%.4f\n", fHBX, fW, fHBY, fH, fAdv );

	int hbx26 = FloatTo26_6( fHBX );
	int w26	  = FloatTo26_6( fW );
	int hby26 = FloatTo26_6( fHBY );
	int h26	  = FloatTo26_6( fH );
	int adv26 = FloatTo26_6( fAdv );

	printf( "      26.6 fixed: hbx=%d(%.2f) w=%d(%.2f) hby=%d(%.2f) h=%d(%.2f) adv=%d(%.2f)\n", hbx26, hbx26 / 64.0f, w26, w26 / 64.0f, hby26, hby26 / 64.0f, h26, h26 / 64.0f, adv26, adv26 / 64.0f );

	int ftLeft26  = FT26_FLOOR( hbx26 );
	int ftRight26 = FT26_CEIL( hbx26 + w26 );
	int ftTop26	  = FT26_CEIL( hby26 );
	int ftBot26	  = FT26_FLOOR( hby26 - h26 );

	printf( "      26.6 rounded: L=%d(%.2f) R=%d(%.2f) T=%d(%.2f) B=%d(%.2f)\n", ftLeft26, ftLeft26 / 64.0f, ftRight26, ftRight26 / 64.0f, ftTop26, ftTop26 / 64.0f, ftBot26, ftBot26 / 64.0f );

	printf( "      final: width=%d height=%d pitch=%d top=%d left=%d xSkip=%d\n",
		FT26_TRUNC( ftRight26 - ftLeft26 ),
		FT26_TRUNC( ftTop26 - ftBot26 ),
		( FT26_TRUNC( ftRight26 - ftLeft26 ) + 3 ) & ~3,
		FT26_TRUNC( hby26 ) + 1,
		FT26_TRUNC( ftLeft26 ),
		FT26_TRUNC( adv26 ) + 1 );
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
int main( int argc, char** argv )
{
	if( argc < 2 ) {
		fprintf( stderr, "Usage: %s <font.ttf> [pointSize]\n", argv[0] );
		return 1;
	}

	const char* ttfPath	  = argv[1];
	float		pointSize = 48.0f;
	if( argc >= 3 ) {
		pointSize = ( float )atof( argv[2] );
		if( pointSize <= 0.0f )
			pointSize = 48.0f;
	}

	/* Load TTF file */
	FILE* f = fopen( ttfPath, "rb" );
	if( !f ) {
		fprintf( stderr, "ERROR: Cannot open '%s'\n", ttfPath );
		return 1;
	}
	fseek( f, 0, SEEK_END );
	long fsize = ftell( f );
	fseek( f, 0, SEEK_SET );
	unsigned char* data = ( unsigned char* )malloc( fsize );
	if( !data ) {
		fclose( f );
		fprintf( stderr, "ERROR: malloc failed\n" );
		return 1;
	}
	fread( data, 1, fsize, f );
	fclose( f );

	/* Init stb_truetype */
	stbtt_fontinfo font;
	int			   offset = stbtt_GetFontOffsetForIndex( data, 0 );
	if( offset < 0 || !stbtt_InitFont( &font, data, offset ) ) {
		fprintf( stderr, "ERROR: stbtt_InitFont failed for '%s'\n", ttfPath );
		free( data );
		return 1;
	}

	float scale			 = stbtt_ScaleForMappingEmToPixels( &font, pointSize );
	float scalePixHeight = stbtt_ScaleForPixelHeight( &font, pointSize );

	int	  ascent, descent, lineGap;
	stbtt_GetFontVMetrics( &font, &ascent, &descent, &lineGap );

	printf( "Font: %s\n", ttfPath );
	printf( "Point size: %.1f\n", pointSize );
	printf( "Scale (EmToPixels): %.10f\n", scale );
	printf( "Scale (PixelHeight): %.10f\n", scalePixHeight );
	printf( "Ascent (font units): %d  -> scaled: %.2f -> ceil: %d\n", ascent, ascent * scale, ( int )ceilf( ascent * scale ) );
	printf( "Descent (font units): %d -> scaled: %.2f -> floor: %d\n", descent, descent * scale, ( int )floorf( descent * scale ) );
	printf( "LineGap (font units): %d\n\n", lineGap );

	/* FreeType reference values from the user's JSON (E1234 font) */
	/* We'll print these if the font name matches */
	typedef struct {
		int cp;
		int width, height, top, left, xSkip;
	} RefGlyph;

	static const RefGlyph ftRef[] = {
		{ 32, 0, 0, 1, 0, 39 },
		{ 33, 8, 35, 40, 7, 39 },
		{ 34, 24, 16, 40, 7, 39 },
		{ 39, 8, 16, 40, 7, 39 },
		{ 40, 20, 43, 44, 7, 39 },
		{ 41, 20, 43, 44, 11, 39 },
		{ 42, 16, 5, 44, 12, 39 },
		{ 43, 20, 35, 40, 7, 39 },
		{ 44, 16, 40, 44, 12, 39 },
		{ 45, 16, 5, 25, 12, 39 },
		{ 46, 8, 7, 10, 15, 39 },
		{ 47, 24, 35, 40, 7, 39 },
		{ 48, 24, 43, 44, 7, 39 },
		{ 49, 8, 35, 40, 26, 39 },
		{ 50, 24, 43, 44, 7, 39 },
		{ 51, 20, 43, 44, 11, 39 },
		{ 52, 24, 35, 40, 7, 39 },
		{ 53, 24, 43, 44, 7, 39 },
		{ 54, 24, 43, 44, 7, 39 },
		{ 55, 20, 39, 44, 11, 39 },
		{ 56, 24, 43, 44, 7, 39 },
		{ 57, 24, 43, 44, 7, 39 },
		{ 58, 8, 26, 35, 15, 39 },
		{ 59, 8, 26, 35, 15, 39 },
		{ 60, 20, 24, 25, 7, 39 },
		{ 61, 16, 24, 25, 12, 39 },
		{ 62, 20, 24, 25, 11, 39 },
		{ 63, 24, 39, 44, 7, 39 },
		{ 64, 24, 43, 44, 7, 39 },
		{ 65, 24, 39, 44, 7, 39 },
		{ 66, 24, 39, 40, 7, 39 },
		{ 67, 20, 43, 44, 7, 39 },
		{ 68, 24, 39, 40, 7, 39 },
		{ 69, 20, 43, 44, 7, 39 },
		{ 70, 20, 39, 44, 7, 39 },
		{ 71, 24, 43, 44, 7, 39 },
		{ 72, 24, 35, 40, 7, 39 },
		{ 73, 8, 35, 40, 26, 39 },
		{ 74, 24, 39, 40, 7, 39 },
		{ 75, 20, 35, 40, 11, 39 },
		{ 76, 20, 39, 40, 7, 39 },
		{ 77, 24, 39, 44, 7, 39 },
		{ 78, 24, 39, 44, 7, 39 },
		{ 79, 24, 43, 44, 7, 39 },
		{ 80, 24, 39, 44, 7, 39 },
		{ 81, 24, 39, 44, 7, 39 },
		{ 82, 20, 39, 44, 7, 39 },
		{ 83, 24, 43, 44, 7, 39 },
		{ 84, 20, 39, 40, 7, 39 },
		{ 85, 24, 39, 40, 7, 39 },
		{ 86, 24, 35, 40, 7, 39 },
		{ 87, 24, 39, 40, 7, 39 },
		{ 88, 24, 35, 40, 7, 39 },
		{ 89, 24, 39, 40, 7, 39 },
		{ 90, 24, 43, 44, 7, 39 },
		{ 0, 0, 0, 0, 0, 0 } /* sentinel */
	};

	/* Test codepoints that appear in the reference */
	int testCodepoints[] = {
		32,
		33,
		34,
		39,
		40,
		41,
		42,
		43,
		44,
		45,
		46,
		47,
		48,
		49,
		50,
		51,
		52,
		53,
		54,
		55,
		56,
		57,
		58,
		59,
		60,
		61,
		62,
		63,
		64,
		65,
		66,
		67,
		68,
		69,
		70,
		71,
		72,
		73,
		74,
		75,
		76,
		77,
		78,
		79,
		80,
		81,
		82,
		83,
		84,
		85,
		86,
		87,
		88,
		89,
		90,
		91,
		92,
		93,
		94,
		95,
		97,
		98,
		99,
		100,
		101,
		102,
		103,
		104,
		105,
		106,
		107,
		108,
		109,
		110,
		111,
		112,
		113,
		114,
		115,
		116,
		117,
		118,
		119,
		120,
		121,
		122,
		123,
		124,
		125,
		126,
		-1 /* sentinel */
	};

	/* ============================================================== */
	/* Phase 1: Evaluate all top-calculation strategies                */
	/* ============================================================== */
	printf( "\n=== Top-Strategy Comparison (only 'top' field, vs FreeType ref) ===\n\n" );

	int stratExact[TOP_STRATEGY_COUNT];
	int stratTotalDiff[TOP_STRATEGY_COUNT];
	int stratMaxDiff[TOP_STRATEGY_COUNT];
	memset( stratExact, 0, sizeof( stratExact ) );
	memset( stratTotalDiff, 0, sizeof( stratTotalDiff ) );
	memset( stratMaxDiff, 0, sizeof( stratMaxDiff ) );

	/* Count reference glyphs */
	int totalRefGlyphs = 0;
	for( int ti = 0; testCodepoints[ti] >= 0; ti++ ) {
		int cp = testCodepoints[ti];
		int gi = stbtt_FindGlyphIndex( &font, cp );
		if( gi == 0 && cp != 0 )
			continue;
		int refIdx = -1;
		for( int r = 0; ftRef[r].cp != 0 || r == 0; r++ ) {
			if( ftRef[r].cp == cp ) {
				refIdx = r;
				break;
			}
		}
		if( refIdx >= 0 )
			totalRefGlyphs++;
	}

	/* Print per-glyph top values for each strategy */
	printf( "%-6s | FT:top", "CP" );
	for( int s = 0; s < TOP_STRATEGY_COUNT; s++ ) {
		printf( " | %-10s", topStrategyNames[s] );
	}
	printf( "\n" );
	printf( "-------+-------" );
	for( int s = 0; s < TOP_STRATEGY_COUNT; s++ ) {
		printf( "-+-----------" );
	}
	printf( "\n" );

	for( int ti = 0; testCodepoints[ti] >= 0; ti++ ) {
		int cp = testCodepoints[ti];
		int gi = stbtt_FindGlyphIndex( &font, cp );
		if( gi == 0 && cp != 0 )
			continue;

		int refIdx = -1;
		for( int r = 0; ftRef[r].cp != 0 || r == 0; r++ ) {
			if( ftRef[r].cp == cp ) {
				refIdx = r;
				break;
			}
		}
		if( refIdx < 0 )
			continue;

		const RefGlyph* ref = &ftRef[refIdx];

		char			cpStr[16];
		if( cp >= 33 && cp < 127 )
			sprintf( cpStr, "'%c'%3d", ( char )cp, cp );
		else
			sprintf( cpStr, "   %3d", cp );

		printf( "%-6s | %5d ", cpStr, ref->top );

		for( int s = 0; s < TOP_STRATEGY_COUNT; s++ ) {
			GlyphMetrics gm	   = CalcFT26Ex( &font, cp, scale, ( TopStrategy )s );
			int			 diff  = gm.top - ref->top;
			int			 adiff = abs( diff );

			if( adiff == 0 ) {
				stratExact[s]++;
				printf( " | %3d  ok   ", gm.top );
			} else {
				printf( " | %3d %+2d   ", gm.top, diff );
			}
			stratTotalDiff[s] += adiff;
			if( adiff > stratMaxDiff[s] )
				stratMaxDiff[s] = adiff;
		}
		printf( "\n" );
	}

	printf( "\n--- Top-Strategy Summary (%d reference glyphs) ---\n", totalRefGlyphs );
	printf( "%-14s | exact | totalDiff | maxDiff\n", "Strategy" );
	printf( "---------------+-------+-----------+--------\n" );
	for( int s = 0; s < TOP_STRATEGY_COUNT; s++ ) {
		printf( "%-14s | %3d/%d |    %4d   |   %2d\n", topStrategyNames[s], stratExact[s], totalRefGlyphs, stratTotalDiff[s], stratMaxDiff[s] );
	}

	/* ============================================================== */
	/* Phase 2: Full comparison using the best default (trunc26+1)     */
	/* ============================================================== */
	printf( "\n\n=== Full Metric Comparison (26.6 method with trunc26+1 for top) ===\n\n" );

	printf( "%-6s | %-6s %-6s %-5s %-5s %-6s | %-6s %-6s %-5s %-5s %-6s | %-6s %-6s %-5s %-5s %-6s | dW  dH  dT  dL  dX\n",
		"CP",
		"FT:w",
		"h",
		"top",
		"left",
		"xSkip",
		"STB:w",
		"h",
		"top",
		"left",
		"xSkip",
		"26.6:w",
		"h",
		"top",
		"left",
		"xSkip" );
	printf( "-------+--------------------------------------+--------------------------------------+--------------------------------------+-------------------\n" );

	int totalGlyphs	 = 0;
	int matchOldSTB	 = 0;
	int matchFT26	 = 0;
	int improvedFT26 = 0;

	for( int ti = 0; testCodepoints[ti] >= 0; ti++ ) {
		int cp = testCodepoints[ti];
		int gi = stbtt_FindGlyphIndex( &font, cp );
		if( gi == 0 && cp != 0 )
			continue;

		GlyphMetrics old  = CalcOldSTB( &font, cp, scale );
		GlyphMetrics ft26 = CalcFT26( &font, cp, scale );

		/* Find FreeType reference */
		int			 refIdx = -1;
		for( int r = 0; ftRef[r].cp != 0 || r == 0; r++ ) {
			if( ftRef[r].cp == cp ) {
				refIdx = r;
				break;
			}
		}

		int			dW_old = 0, dH_old = 0, dT_old = 0, dL_old = 0, dX_old = 0;
		int			dW_26 = 0, dH_26 = 0, dT_26 = 0, dL_26 = 0, dX_26 = 0;
		const char *ftW = "-", *ftH = "-", *ftT = "-", *ftL = "-", *ftXS = "-";
		char		ftWb[16], ftHb[16], ftTb[16], ftLb[16], ftXSb[16];

		if( refIdx >= 0 ) {
			const RefGlyph* ref = &ftRef[refIdx];
			sprintf( ftWb, "%d", ref->width );
			ftW = ftWb;
			sprintf( ftHb, "%d", ref->height );
			ftH = ftHb;
			sprintf( ftTb, "%d", ref->top );
			ftT = ftTb;
			sprintf( ftLb, "%d", ref->left );
			ftL = ftLb;
			sprintf( ftXSb, "%d", ref->xSkip );
			ftXS = ftXSb;

			dW_old = abs( old.width - ref->width );
			dH_old = abs( old.height - ref->height );
			dT_old = abs( old.top - ref->top );
			dL_old = abs( old.left - ref->left );
			dX_old = abs( old.xSkip - ref->xSkip );

			dW_26 = abs( ft26.width - ref->width );
			dH_26 = abs( ft26.height - ref->height );
			dT_26 = abs( ft26.top - ref->top );
			dL_26 = abs( ft26.left - ref->left );
			dX_26 = abs( ft26.xSkip - ref->xSkip );

			if( dW_old + dH_old + dT_old + dL_old + dX_old == 0 )
				matchOldSTB++;
			if( dW_26 + dH_26 + dT_26 + dL_26 + dX_26 == 0 )
				matchFT26++;
			if( ( dW_26 + dH_26 + dT_26 + dL_26 + dX_26 ) < ( dW_old + dH_old + dT_old + dL_old + dX_old ) )
				improvedFT26++;
			totalGlyphs++;
		}

		char cpStr[16];
		if( cp >= 33 && cp < 127 )
			sprintf( cpStr, "'%c'%3d", ( char )cp, cp );
		else
			sprintf( cpStr, "   %3d", cp );

		printf( "%-6s | %-6s %-6s %-5s %-5s %-6s | %-6d %-6d %-5d %-5d %-6d | %-6d %-6d %-5d %-5d %-6d | %+d%s %+d%s %+d%s %+d%s %+d%s\n",
			cpStr,
			ftW,
			ftH,
			ftT,
			ftL,
			ftXS,
			old.width,
			old.height,
			old.top,
			old.left,
			old.xSkip,
			ft26.width,
			ft26.height,
			ft26.top,
			ft26.left,
			ft26.xSkip,
			ft26.width - old.width,
			( refIdx >= 0 && dW_26 < dW_old ) ? "*" : "",
			ft26.height - old.height,
			( refIdx >= 0 && dH_26 < dH_old ) ? "*" : "",
			ft26.top - old.top,
			( refIdx >= 0 && dT_26 < dT_old ) ? "*" : "",
			ft26.left - old.left,
			( refIdx >= 0 && dL_26 < dL_old ) ? "*" : "",
			ft26.xSkip - old.xSkip,
			( refIdx >= 0 && dX_26 < dX_old ) ? "*" : "" );
	}

	printf( "\n" );
	printf( "=== Summary (vs FreeType reference, %d glyphs with ref data) ===\n", totalGlyphs );
	printf( "  Old STB exact matches: %d / %d\n", matchOldSTB, totalGlyphs );
	printf( "  FT 26.6 exact matches: %d / %d\n", matchFT26, totalGlyphs );
	printf( "  FT 26.6 improved (closer to ref): %d / %d\n", improvedFT26, totalGlyphs );

	/* Print detailed 26.6 breakdown for a few key glyphs */
	printf( "\n=== Detailed 26.6 breakdown for selected glyphs ===\n" );
	int detailCPs[] = { 33, 34, 48, 65, 73, 45, 46, 40, -1 };
	for( int di = 0; detailCPs[di] >= 0; di++ ) {
		int cp = detailCPs[di];
		int gi = stbtt_FindGlyphIndex( &font, cp );
		if( gi == 0 )
			continue;
		if( cp >= 33 && cp < 127 )
			printf( "  Glyph '%c' (U+%04X):\n", ( char )cp, cp );
		else
			printf( "  Glyph U+%04X:\n", cp );
		DumpFT26Detail( &font, cp, scale );
		printf( "\n" );
	}

	free( data );
	return 0;
}