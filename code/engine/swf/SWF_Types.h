/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2015 Robert Beckebans

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
#ifndef __SWF_TYPES1_H__
#define __SWF_TYPES1_H__

//! Converts a TWIP value to a float value by scaling with 1/20.
ID_INLINE float SWFTWIP( int twip )
{
	return twip * ( 1.0f / 20.0f );
}

//! Converts a fixed-point integer value to its floating-point representation
ID_INLINE float SWFFIXED16( int fixed )
{
	return fixed * ( 1.0f / 65536.0f );
}

//! Converts a fixed-point 8.8 integer value to floating-point.
ID_INLINE float SWFFIXED8( int fixed )
{
	return fixed * ( 1.0f / 256.0f );
}

//! Converts a float value to an integer using a fixed multiplier of 20
ID_INLINE int FLOAT2SWFTWIP( float value )
{
	return ( int )( value * 20 );
}

//! Converts a floating-point value to a fixed-point integer representation using a 16.16 bit format.
ID_INLINE int FLOAT2SWFFIXED16( float value )
{
	return ( int )( value * 65536 );
}

//! Converts a float value to a fixed-point integer representation scaled by 256.
ID_INLINE int FLOAT2SWFFIXED8( float value )
{
	return ( int )( value * 256 );
}

struct swfHeader_t {
	byte   compression;
	byte   W;
	byte   S;
	byte   version;
	uint32 fileLength;
};

struct swfRect_t {
	//! Initializes a swfRect_t object with both top-left and bottom-right corners set to (0, 0).
	swfRect_t();
	idVec2 tl;
	idVec2 br;

	/*!
		\brief Initializes a swfRect_t object with the specified position and dimensions.

		This constructor sets up a rectangle structure using the given top-left coordinates and dimensions. The top-left corner is defined by (x, y) and the bottom-right corner is calculated as (x +
	   w, y + h). This is commonly used to define bounding boxes for graphical elements in SWF vector graphics.

		\param x The x-coordinate of the top-left corner
		\param y The y-coordinate of the top-left corner
		\param w The width of the rectangle
		\param h The height of the rectangle
	*/
	swfRect_t( float x, float y, float w, float h )
	{
		tl.x = x;
		tl.y = y;
		br.x = x + w;
		br.y = y + h;
	}

	//! Returns the x-coordinate of the top-left corner of the rectangle.
	float x() const { return tl.x; }

	//! Returns the y-coordinate of the top-left corner of the rectangle.
	float y() const { return tl.y; }

	//! Returns the width of the rectangle defined by the swfRect_t structure.
	float w() const { return fabs( br.x - tl.x ); }

	//! Returns the height of the rectangle as the absolute difference between the bottom and top y-coordinates.
	float h() const { return fabs( br.y - tl.y ); }

	//! Returns the bottom Y-coordinate of the rectangle
	float Bottom() const { return br.y; }
	// RB end
};

struct swfMatrix_t {
	//! Initializes a new swfMatrix_t object with default values.
	swfMatrix_t();
	float		 xx, yy;
	float		 xy, yx;
	float		 tx, ty;

	//! Scales the input vector using the matrix components xx, xy, yx, and yy.
	idVec2		 Scale( const idVec2& in ) const;

	//! Transforms a 2D vector using the matrix components xx, xy, tx, yy, yx, and ty.
	idVec2		 Transform( const idVec2& in ) const;

	//! Returns the matrix multiplication result of this matrix with another matrix.
	swfMatrix_t	 Multiply( const swfMatrix_t& a ) const;

	//! Returns the inverse of this transformation matrix.
	swfMatrix_t	 Inverse() const;

	//! Assigns the contents of another swfMatrix_t instance to this instance and returns a reference to this instance.
	swfMatrix_t& operator=( const swfMatrix_t& a )
	{
		xx = a.xx;
		yy = a.yy;
		xy = a.xy;
		yx = a.yx;
		tx = a.tx;
		ty = a.ty;
		return *this;
	}

	//! Parses an SVG transform string and updates the matrix components accordingly.
	void ParseSVGTransformFromString( const char* str );

	//! Compares two swfMatrix_t objects for equality.
	bool operator==( const swfMatrix_t& a ) { return ( xx == a.xx && yy == a.yy && xy == a.xy && yx == a.yx && tx == a.tx && ty == a.ty ); }

	//! Compares this swfMatrix_t object with another for inequality.
	bool operator!=( const swfMatrix_t& a ) { return !( xx == a.xx && yy == a.yy && xy == a.xy && yx == a.yx && tx == a.tx && ty == a.ty ); }
	// RB end
};

struct swfColorRGB_t {
	//! Constructs a swfColorRGB_t object with default RGB values set to 255, 255, 255.
	swfColorRGB_t();

	//! Converts the RGB color values to a normalized vec4 with alpha set to 1.0.
	idVec4 ToVec4() const;
	uint8  r, g, b;
};
struct swfColorRGBA_t : public swfColorRGB_t {
	//! Initializes a swfColorRGBA_t object with default alpha value of 255.
	swfColorRGBA_t();

	//! Converts the color components to a normalized idVec4 vector.
	idVec4 ToVec4() const;
	uint8  a;

	//! Parses a color string in SVG format and sets the RGBA color components.
	void   ParseSVGColorFromString( const char* str );
};
struct swfLineStyle_t {
	//! Initializes a swfLineStyle_t object with default start and end width values of 20.
	swfLineStyle_t();
	uint16		   startWidth;
	uint16		   endWidth;
	swfColorRGBA_t startColor;
	swfColorRGBA_t endColor;
};
struct swfGradientRecord_t {
	//! Initializes a new instance of the swfGradientRecord_t struct with default start and end ratios set to zero.
	swfGradientRecord_t();
	uint8		   startRatio;
	uint8		   endRatio;
	swfColorRGBA_t startColor;
	swfColorRGBA_t endColor;
};
struct swfGradient_t {
	//! Initializes a new swfGradient_t object with zero gradients.
	swfGradient_t();
	uint8				numGradients;
	swfGradientRecord_t gradientRecords[16];
};
struct swfFillStyle_t {
	//! Initializes a new instance of the swfFillStyle_t structure with default values.
	swfFillStyle_t();
	uint8		   type;		// 0 = solid, 1 = gradient, 4 = bitmap
	uint8		   subType;		// 0 = linear, 2 = radial, 3 = focal; 0 = repeat, 1 = clamp, 2 = near repeat, 3 = near clamp
	swfColorRGBA_t startColor;	// if type = 0
	swfColorRGBA_t endColor;	// if type = 0
	swfMatrix_t	   startMatrix; // if type > 0
	swfMatrix_t	   endMatrix;	// if type > 0
	swfGradient_t  gradient;	// if type = 1
	float		   focalPoint;	// if type = 1 and subType = 3
	uint16		   bitmapID;	// if type = 4
};

/*!
	\class idSWFShapeDrawFill
	\brief The class represents a fill style for drawing shapes in a SWF-based rendering system.
*/
class idSWFShapeDrawFill
{
public:
	swfFillStyle_t			style;
	idList<idVec2, TAG_SWF> startVerts;
	idList<idVec2, TAG_SWF> endVerts;
	idList<uint16, TAG_SWF> indices;
};

/*!
	\class idSWFShapeDrawLine
	\brief A class for drawing lines in a SWF shape context.
*/
class idSWFShapeDrawLine
{
public:
	swfLineStyle_t			style;
	idList<idVec2, TAG_SWF> startVerts;
	idList<idVec2, TAG_SWF> endVerts;
	idList<uint16, TAG_SWF> indices;
};

/*!
	\class idSWFShape
	\brief Manages SWF shape data for rendering and animation.
*/
class idSWFShape
{
public:
	//! Destroys an idSWFShape object and releases its associated resources.
	~idSWFShape()
	{
		fillDraws.Clear();
		lineDraws.Clear();
	}
	swfRect_t							startBounds;
	swfRect_t							endBounds;
	idList<idSWFShapeDrawFill, TAG_SWF> fillDraws;
	idList<idSWFShapeDrawLine, TAG_SWF> lineDraws;
};

/*!
	\class idSWFFontGlyph
	\brief The idSWFFontGlyph class represents a glyph in a font, storing character code and advance information.
*/
class idSWFFontGlyph
{
public:
	//! Initializes a new instance of the idSWFFontGlyph class with default values for code and advance.
	idSWFFontGlyph();
	uint16					code;
	int16					advance;
	// RB: verts and indices are not used by the renderer
	idList<idVec2, TAG_SWF> verts;
	idList<uint16, TAG_SWF> indices;
};

/*!
	\class idSWFFont
	\brief A class representing a SWF font used for text rendering.
*/
class idSWFFont
{
public:
	//! Initializes a new instance of the idSWFFont class with default values.
	idSWFFont();
	class idFont*					fontID;
	int16							ascent;	 // unused except for read/write
	int16							descent; // unused except for read/write
	int16							leading; // unused except for read/write
	idList<idSWFFontGlyph, TAG_SWF> glyphs;	 // not really used in BFG assets
};

/*!
	\class idSWFTextRecord
	\brief Represents a text record in a SWF file.
*/
class idSWFTextRecord
{
public:
	//! Initializes a new instance of the idSWFTextRecord class with default values.
	idSWFTextRecord();
	uint16		   fontID;
	swfColorRGBA_t color;
	int16		   xOffset;
	int16		   yOffset;
	uint16		   textHeight;
	uint16		   firstGlyph;
	uint8		   numGlyphs;
};
struct swfGlyphEntry_t {
	//! Initializes a new instance of the swfGlyphEntry_t structure with default values.
	swfGlyphEntry_t();
	uint32 index;
	int32  advance;
};

/*!
	\class idSWFText
	\brief A class for handling text rendering and manipulation within a SWF-based user interface system.
*/
class idSWFText
{
public:
	swfRect_t						 bounds;
	swfMatrix_t						 matrix;
	idList<idSWFTextRecord, TAG_SWF> textRecords;
	idList<swfGlyphEntry_t, TAG_SWF> glyphs;
};
enum swfEditTextFlags_t {
	SWF_ET_NONE		 = 0,
	SWF_ET_WORDWRAP	 = BIT( 0 ),
	SWF_ET_MULTILINE = BIT( 1 ),
	SWF_ET_PASSWORD	 = BIT( 2 ),
	SWF_ET_READONLY	 = BIT( 3 ),
	SWF_ET_AUTOSIZE	 = BIT( 4 ),
	SWF_ET_BORDER	 = BIT( 5 ),
};
enum swfEditTextAlign_t { SWF_ET_ALIGN_LEFT, SWF_ET_ALIGN_RIGHT, SWF_ET_ALIGN_CENTER, SWF_ET_ALIGN_JUSTIFY };
enum swfTextRenderMode_t {
	SWF_TEXT_RENDER_NORMAL = 0,
	SWF_TEXT_RENDER_RANDOM_APPEAR,
	SWF_TEXT_RENDER_RANDOM_APPEAR_CAPS,
	SWF_TEXT_RENDER_PARAGRAPH,
	SWF_TEXT_RENDER_AUTOSCROLL,
	SWF_TEXT_RENDER_MODE_COUNT,
};

/*!
	\class idSWFEditText
	\brief The idSWFEditText class represents an editable text element within a SWF-based user interface system.
*/
class idSWFEditText
{
public:
	//! Initializes a new instance of the idSWFEditText class with default values.
	idSWFEditText();
	swfRect_t		   bounds;
	uint32			   flags;
	uint16			   fontID;
	uint16			   fontHeight;
	swfColorRGBA_t	   color;
	uint16			   maxLength;
	swfEditTextAlign_t align;
	uint16			   leftMargin;
	uint16			   rightMargin;
	uint16			   indent;
	int16			   leading;
	idStr			   variable;
	idStr			   initialText;
};
struct swfColorXform_t {
	//! Initializes a new swfColorXform_t instance with default multiply and add color values.
	swfColorXform_t();
	idVec4			 mul;
	idVec4			 add;

	//! Multiplies this color transformation with another color transformation and returns the result.
	swfColorXform_t	 Multiply( const swfColorXform_t& a ) const;

	//! Assigns the contents of another swfColorXform_t instance to this instance
	swfColorXform_t& operator=( const swfColorXform_t& a )
	{
		mul = a.mul;
		add = a.add;
		return *this;
	}
};
struct swfDisplayEntry_t {
	//! Initializes a new instance of swfDisplayEntry_t with default values.
	swfDisplayEntry_t();
	uint16					   characterID;
	uint16					   depth;
	uint16					   clipDepth;
	uint16					   blendMode;
	swfMatrix_t				   matrix;
	swfColorXform_t			   cxf;
	float					   ratio;
	// if this entry is a sprite, then this will point to the specific instance of that sprite
	class idSWFSpriteInstance* spriteInstance;
	// if this entry is text, then this will point to the specific instance of the text
	class idSWFTextInstance*   textInstance;
};

struct svgDisplayEntry_t {
	//! Initializes a new instance of svgDisplayEntry_t with default values.
	svgDisplayEntry_t();
	uint16					characterID;
	uint16					depth;
	uint16					clipDepth;
	uint16					blendMode;
	swfMatrix_t				matrix;
	swfColorXform_t			cxf;
	float					ratio;
	idStr					name; // RB: instance name
	idList<float>			opacityFrames;
	idList<swfMatrix_t>		matrixFrames;
	idList<swfColorXform_t> colorFrames;
};

struct swfRenderState_t {
	//! Initializes a new instance of the swfRenderState_t structure with default values.
	swfRenderState_t();
	swfMatrix_t		  matrix;
	swfColorXform_t	  cxf;
	const idMaterial* material;
	int				  materialWidth;
	int				  materialHeight;
	int				  activeMasks;
	uint8			  blendMode;
	float			  ratio;
	stereoDepthType_t stereoDepth;
};

ID_INLINE swfRect_t::swfRect_t() :
	tl( 0.0f, 0.0f ),
	br( 0.0f, 0.0f )
{
}

ID_INLINE swfMatrix_t::swfMatrix_t() :
	xx( 1.0f ),
	yy( 1.0f ),
	xy( 0.0f ),
	yx( 0.0f ),
	tx( 0.0f ),
	ty( 0.0f )
{
}

ID_INLINE idVec2 swfMatrix_t::Scale( const idVec2& in ) const
{
	return idVec2( ( in.x * xx ) + ( in.y * xy ), ( in.y * yy ) + ( in.x * yx ) );
}

ID_INLINE idVec2 swfMatrix_t::Transform( const idVec2& in ) const
{
	return idVec2( ( in.x * xx ) + ( in.y * xy ) + tx, ( in.y * yy ) + ( in.x * yx ) + ty );
}

ID_INLINE swfMatrix_t swfMatrix_t::Inverse() const
{
	swfMatrix_t inverse;
	float		det = ( ( xx * yy ) - ( yx * xy ) );
	if( idMath::Fabs( det ) < idMath::FLT_SMALLEST_NON_DENORMAL ) { return *this; }
	float invDet = 1.0f / det;
	inverse.xx	 = invDet * yy;
	inverse.yx	 = invDet * -yx;
	inverse.xy	 = invDet * -xy;
	inverse.yy	 = invDet * xx;
	// inverse.tx = invDet * ( xy * ty ) - ( yy * tx );
	// inverse.ty = invDet * ( yx * tx ) - ( xx * ty );
	return inverse;
}

ID_INLINE swfMatrix_t swfMatrix_t::Multiply( const swfMatrix_t& a ) const
{
	swfMatrix_t result;
	result.xx = xx * a.xx + yx * a.xy;
	result.yx = xx * a.yx + yx * a.yy;
	result.xy = xy * a.xx + yy * a.xy;
	result.yy = xy * a.yx + yy * a.yy;
	result.tx = tx * a.xx + ty * a.xy + a.tx;
	result.ty = tx * a.yx + ty * a.yy + a.ty;
	return result;
}

ID_INLINE swfColorRGB_t::swfColorRGB_t() :
	r( 255 ),
	g( 255 ),
	b( 255 )
{
}

ID_INLINE idVec4 swfColorRGB_t::ToVec4() const
{
	return idVec4( r * ( 1.0f / 255.0f ), g * ( 1.0f / 255.0f ), b * ( 1.0f / 255.0f ), 1.0f );
}

ID_INLINE swfColorRGBA_t::swfColorRGBA_t() :
	a( 255 )
{
}

ID_INLINE idVec4 swfColorRGBA_t::ToVec4() const
{
	return idVec4( r * ( 1.0f / 255.0f ), g * ( 1.0f / 255.0f ), b * ( 1.0f / 255.0f ), a * ( 1.0f / 255.0f ) );
}

ID_INLINE swfLineStyle_t::swfLineStyle_t() :
	startWidth( 20 ),
	endWidth( 20 )
{
}

ID_INLINE swfGradientRecord_t::swfGradientRecord_t() :
	startRatio( 0 ),
	endRatio( 0 )
{
}

ID_INLINE swfGradient_t::swfGradient_t() :
	numGradients( 0 )
{
}

ID_INLINE swfFillStyle_t::swfFillStyle_t() :
	type( 0 ),
	subType( 0 ),
	focalPoint( 0.0f ),
	bitmapID( 0 )
{
}

ID_INLINE swfColorXform_t::swfColorXform_t() :
	mul( 1.0f, 1.0f, 1.0f, 1.0f ),
	add( 0.0f, 0.0f, 0.0f, 0.0f )
{
}

ID_INLINE swfColorXform_t swfColorXform_t::Multiply( const swfColorXform_t& a ) const
{
	swfColorXform_t result;
	result.mul = mul.Multiply( a.mul );
	result.add = ( add.Multiply( a.mul ) ) + a.add;
	return result;
}

ID_INLINE swfDisplayEntry_t::swfDisplayEntry_t() :
	characterID( 0 ),
	depth( 0 ),
	clipDepth( 0 ),
	blendMode( 0 ),
	ratio( 0.0f ),
	spriteInstance( NULL ),
	textInstance( NULL )
{
}

ID_INLINE svgDisplayEntry_t::svgDisplayEntry_t() :
	characterID( 0 ),
	depth( 0 ),
	clipDepth( 0 ),
	blendMode( 0 ),
	ratio( 0.0f )
{
}

ID_INLINE swfRenderState_t::swfRenderState_t() :
	material( NULL ),
	materialWidth( 0 ),
	materialHeight( 0 ),
	activeMasks( 0 ),
	blendMode( 0 ),
	ratio( 0.0f ),
	stereoDepth( STEREO_DEPTH_TYPE_NONE )
{
}

ID_INLINE idSWFFontGlyph::idSWFFontGlyph() :
	code( 0 ),
	advance( 0 )
{
}

ID_INLINE idSWFFont::idSWFFont() :
	fontID( 0 ),
	ascent( 0 ),
	descent( 0 ),
	leading( 0 )
{
}

ID_INLINE idSWFTextRecord::idSWFTextRecord() :
	fontID( 0 ),
	xOffset( 0 ),
	yOffset( 0 ),
	textHeight( 0 ),
	firstGlyph( 0 ),
	numGlyphs( 0 )
{
}

ID_INLINE idSWFEditText::idSWFEditText() :
	flags( SWF_ET_NONE ),
	fontID( 0 ),
	fontHeight( 24 ),
	maxLength( 0xFFFF ),
	align( SWF_ET_ALIGN_LEFT ),
	leftMargin( 0 ),
	rightMargin( 0 ),
	indent( 0 ),
	leading( 0 )
{
}

ID_INLINE swfGlyphEntry_t::swfGlyphEntry_t() :
	index( 0 ),
	advance( 0 )
{
}

#endif // !__SWF_TYPES1_H__
