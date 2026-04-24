/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#ifndef __DEVICECONTEXT_H__
#define __DEVICECONTEXT_H__

// device context support for gui stuff
//

#include "Rectangle.h"
#include "../renderer/Font.h"

const int VIRTUAL_WIDTH	 = 640;
const int VIRTUAL_HEIGHT = 480;
const int BLINK_DIVISOR	 = 200;

/*!
	\class idDeviceContext
	\brief A device context for rendering graphics and text with support for transformations, clipping, and material-based drawing.

	This class provides a comprehensive interface for rendering 2D graphics and text within a device context. It supports various drawing operations including materials, rectangles, text, and cursor
   rendering. The context manages transformation information, clipping rectangles, and font settings to enable flexible and efficient rendering. It handles coordinate transformations, texture
   coordinate adjustments for scaling and rotation, and clipping checks to ensure proper rendering boundaries. The class also provides functionality for managing cursor states, overstrike mode, and
   text formatting with support for color codes and Unicode text. Initialization and shutdown routines manage the context's lifecycle, while utility functions provide metrics for text sizing and
   character dimensions.

*/
class idDeviceContext
{
public:
	//! Initializes a new instance of the idDeviceContext class.
	idDeviceContext();
	~idDeviceContext() { }

	//! Initializes the device context with default values and resources.
	void Init();

	//! Shuts down the device context by clearing clip rectangles and performing cleanup.
	void Shutdown();

	//! Returns true if the device context has been successfully initialized.
	bool Initialized() { return initialized; }
	void EnableLocalization();

	//! Retrieves the transformation information including origin and matrix from the device context.
	void GetTransformInfo( idVec3& origin, idMat3& mat );

	//! Sets the transformation information including origin and orientation matrix.
	void SetTransformInfo( const idVec3& origin, const idMat3& mat );

	/*!
		\brief Draws a material stretched to fit a specified rectangle with optional scaling and clipping

		This function draws a material stretched to fit the specified rectangle defined by position (x, y) and dimensions (w, h). The material is drawn with the specified color and can be scaled using
	   scalex and scaley parameters. Negative scale values are handled by flipping the texture coordinates and adjusting the dimensions accordingly. The function performs clipping checks and returns
	   early if the rectangle is completely clipped. The material is rendered using the internal DrawStretchPic function.

		\param x The x-coordinate of the top-left corner of the rectangle
		\param y The y-coordinate of the top-left corner of the rectangle
		\param w The width of the rectangle
		\param h The height of the rectangle
		\param mat Pointer to the material to be drawn
		\param color The color to be applied to the material
		\param scalex Horizontal scaling factor for the material texture coordinates
		\param scaley Vertical scaling factor for the material texture coordinates
	*/
	void DrawMaterial( float x, float y, float w, float h, const idMaterial* mat, const idVec4& color, float scalex = 1.0, float scaley = 1.0 );

	/*!
		\brief Draws a rectangular outline with specified dimensions and color

		This function renders a rectangular border using four separate draw calls for the sides of the rectangle. The rectangle is drawn using the provided color and line thickness. If the alpha
	   component of the color is zero, the function returns immediately without drawing. The coordinates are clipped to the view area before rendering. The rectangle is drawn using a white image
	   texture for all four edges

		\param x The x-coordinate of the rectangle's top-left corner
		\param y The y-coordinate of the rectangle's top-left corner
		\param width The width of the rectangle
		\param height The height of the rectangle
		\param size The thickness of the rectangle outline
		\param color The color of the rectangle outline, where alpha controls visibility
	*/
	void DrawRect( float x, float y, float width, float height, float size, const idVec4& color );

	/*!
		\brief Draws a filled rectangle on the screen with the specified position, dimensions, and color

		This function renders a solid colored rectangle at the given coordinates with the specified width and height. The rectangle is drawn using the device context's rendering system. The function
	   first checks if the alpha component of the color is zero, and if so, returns immediately without drawing. It then sets the current color in the render system and performs coordinate clipping to
	   ensure the rectangle is within the viewable area. Finally, it draws the rectangle using a white image texture, effectively rendering a solid color rectangle

		\param x The x-coordinate of the rectangle's top-left corner
		\param y The y-coordinate of the rectangle's top-left corner
		\param width The width of the rectangle
		\param height The height of the rectangle
		\param color The color of the rectangle expressed as RGBA values
	*/
	void DrawFilledRect( float x, float y, float width, float height, const idVec4& color );

	/*!
		\brief Draws text within a specified rectangle, handling line breaks, text alignment, and optional cursor positioning

		This function renders text using the active font and handles various text formatting options. It supports wrapping text to fit within a given rectangle, aligning text (left, center, right),
	   and drawing a cursor at a specific position. The function can also calculate text metrics without rendering, and handles special cases like color escape sequences in the text. It supports both
	   single-line and multi-line text rendering, with automatic line breaking when text exceeds the rectangle width. The cursor parameter controls where a cursor is drawn, and the limit parameter can
	   constrain the number of characters rendered.

		\param text The text string to render
		\param textScale The scaling factor for text size
		\param textAlign The alignment of text (ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT)
		\param color The color to use for rendering text
		\param rectDraw The rectangle bounds within which text is drawn
		\param wrap Whether to wrap text to new lines when it exceeds rectangle width
		\param cursor The position of the cursor within the text, or -1 to disable cursor drawing
		\param calcOnly If true, only calculate text metrics without rendering
		\param breaks Optional list to store positions where text was broken into new lines
		\param limit Maximum number of characters to render, or 0 to render all
		\return The number of characters that would fit within the rectangle width, or the actual number of characters drawn if calcOnly is false
	*/
	int	 DrawText( const char* text, float textScale, int textAlign, idVec4 color, idRectangle rectDraw, bool wrap, int cursor = -1, bool calcOnly = false, idList<int>* breaks = NULL, int limit = 0 );

	/*!
		\brief Draws a rectangular outline using a specified material and color

		This function renders a rectangular border by drawing four separate material segments: top, bottom, left, and right edges. The function first checks if the alpha component of the color is
	   zero, and if so, it returns early without rendering. The rectangle is drawn using the provided material and color, with the specified size determining the thickness of the border edges. The
	   function utilizes the RenderSystem to set the color before drawing each segment.

		\param x The x-coordinate of the top-left corner of the rectangle
		\param y The y-coordinate of the top-left corner of the rectangle
		\param w The width of the rectangle
		\param h The height of the rectangle
		\param size The thickness of the border edges
		\param mat Pointer to the material to be used for rendering the rectangle
		\param color The color to be used for rendering, including alpha channel
	*/
	void DrawMaterialRect( float x, float y, float w, float h, float size, const idMaterial* mat, const idVec4& color );

	/*!
		\brief Draws a textured quad on screen with specified coordinates, texture coordinates, and material.

		This function renders a textured rectangle on the screen using the provided material and texture coordinates. The rectangle is defined by the x, y, w, and h parameters representing screen
	   position and dimensions. Texture coordinates are defined by s0, t0, s1, and t1. If a transformation matrix is applied, the function applies the transformation to the quad's vertices before
	   rendering. The function delegates to the render system for actual drawing when no transformation is needed.

		\param x Screen X coordinate of the top-left corner of the quad
		\param y Screen Y coordinate of the top-left corner of the quad
		\param w Width of the quad in screen coordinates
		\param h Height of the quad in screen coordinates
		\param s0 Texture coordinate S for the top-left corner
		\param t0 Texture coordinate T for the top-left corner
		\param s1 Texture coordinate S for the bottom-right corner
		\param t1 Texture coordinate T for the bottom-right corner
		\param mat Material to use for the textured quad rendering
	*/
	void DrawStretchPic( float x, float y, float w, float h, float s0, float t0, float s1, float t1, const idMaterial* mat );

	/*!
		\brief Renders a material stretched to a specified rectangle with optional scaling and rotation

		This function draws a material stretched to a rectangular area defined by position (x, y) and dimensions (w, h). The material is rendered with the specified color and can be scaled and
	   rotated. Negative scale values are handled by adjusting the texture coordinates and dimensions accordingly. The function clips the coordinates if necessary and calls the appropriate rendering
	   function for the final display

		\param x X coordinate of the top-left corner of the rectangle
		\param y Y coordinate of the top-left corner of the rectangle
		\param w Width of the rectangle
		\param h Height of the rectangle
		\param mat Pointer to the material to be rendered
		\param color Color to be applied to the material
		\param scalex Horizontal scaling factor
		\param scaley Vertical scaling factor
		\param angle Rotation angle in degrees
	*/
	void DrawMaterialRotated( float x, float y, float w, float h, const idMaterial* mat, const idVec4& color, float scalex = 1.0, float scaley = 1.0, float angle = 0.0f );

	/*!
		\brief Draws a rotated textured quad using the specified material and texture coordinates.

		This function renders a textured rectangle that can be rotated around its center point. The quad is defined by its position, dimensions, and texture coordinate mapping. The rotation is applied
	   around the Z-axis, with the center of rotation being the center of the quad. The function internally constructs a winding from the provided coordinates, applies translation and rotation
	   transformations, and then draws the resulting winding using the specified material.

		\param x The x-coordinate of the top-left corner of the quad
		\param y The y-coordinate of the top-left corner of the quad
		\param w The width of the quad
		\param h The height of the quad
		\param s0 The initial texture x-coordinate for the quad
		\param t0 The initial texture y-coordinate for the quad
		\param s1 The final texture x-coordinate for the quad
		\param t1 The final texture y-coordinate for the quad
		\param mat The material to use for rendering the quad
		\param angle The rotation angle in radians around the Z-axis
	*/
	void DrawStretchPicRotated( float x, float y, float w, float h, float s0, float t0, float s1, float t1, const idMaterial* mat, float angle = 0.0f );

	//! Draws a winding using the specified material and applies screen-space transformations.
	void DrawWinding( idWinding& w, const idMaterial* mat );

	//! Returns the pixel width of a character at a given scale
	int	 CharWidth( const char c, float scale );

	//! Calculates the pixel width of a text string using the active font and specified scale.
	int	 TextWidth( const char* text, float scale, int limit );

	//! Returns the height of a text line using the active font and specified scale.
	int	 TextHeight( const char* text, float scale, int limit );

	//! Returns the maximum character height for the active font at the given scale
	int	 MaxCharHeight( float scale );

	//! Returns the maximum character width for the active font at the given scale.
	int	 MaxCharWidth( float scale );

	/*!
		\brief Returns a text region for rendering text within a specified rectangle using the provided scaling and starting position.

		This function computes and returns an idRegion that represents the area where text will be rendered. It takes into account the provided text string, scaling factor, drawing rectangle, and
	   starting coordinates for text placement. The text is only supported for left alignment as indicated by the original comment. This function currently returns NULL and is likely a stub or
	   placeholder implementation in the current codebase.

		\param text The null-terminated character array containing the text to be rendered
		\param textScale The scaling factor for the text size
		\param rectDraw The rectangle defining the drawing area for the text
		\param xStart The starting x-coordinate for text placement
		\param yStart The starting y-coordinate for text placement
		\return A pointer to an idRegion representing the text area, or NULL if not implemented
	*/
	idRegion*		  GetTextRegion( const char* text, float textScale, idRectangle rectDraw, float xStart, float yStart );

	//! Sets the width and height for the device context, adjusting scaling factors.
	void			  SetSize( float width, float height );

	//! Sets the x and y offset values for the device context.
	void			  SetOffset( float x, float y );

	//! Returns the scrollbar image material for the specified index, with bounds checking.
	const idMaterial* GetScrollBarImage( int index );

	//! Updates cursor position within virtual screen bounds and renders the cursor at the specified location.
	void			  DrawCursor( float* x, float* y, float size );

	//! Sets the cursor to the specified value if it is within valid bounds, otherwise defaults to the arrow cursor.
	void			  SetCursor( int n );

	/*!
		\brief Clips rectangle coordinates and texture coordinates against a list of clipping rectangles.

		This function modifies the provided rectangle coordinates and texture coordinates to fit within the defined clipping rectangles. It adjusts the position, size, and texture coordinate ranges of
	   the rectangle based on intersection with each clipping rectangle in the list. The function returns true if the final rectangle has zero width or height, indicating it was completely clipped
	   out, and false otherwise. The function processes the clipping rectangles in reverse order and updates the input parameters in place.

		\param x Pointer to the x-coordinate of the rectangle to be clipped
		\param y Pointer to the y-coordinate of the rectangle to be clipped
		\param w Pointer to the width of the rectangle to be clipped
		\param h Pointer to the height of the rectangle to be clipped
		\param s1 Pointer to the start s texture coordinate of the rectangle to be clipped
		\param t1 Pointer to the start t texture coordinate of the rectangle to be clipped
		\param s2 Pointer to the end s texture coordinate of the rectangle to be clipped
		\param t2 Pointer to the end t texture coordinate of the rectangle to be clipped
		\return True if the rectangle was completely clipped out (zero width or height), false otherwise.
	*/
	virtual bool	  ClippedCoords( float* x, float* y, float* w, float* h, float* s1, float* t1, float* s2, float* t2 );

	//! Adds a clipping rectangle to the clip rectangle stack.
	virtual void	  PushClipRect( idRectangle r );

	//! Removes the most recently added clip rectangle from the stack.
	virtual void	  PopClipRect();
	virtual void	  EnableClipping( bool b );

	//! Sets the active font for the device context.
	void			  SetFont( idFont* font ) { activeFont = font; }

	//! Sets the overwrite mode state for the device context.
	void			  SetOverStrike( bool b ) { overStrikeMode = b; }

	//! Returns the current overstrike mode setting.
	bool			  GetOverStrike() { return overStrikeMode; }

	//! Draws an edit cursor at the specified position with the given scale.
	void			  DrawEditCursor( float x, float y, float scale );

	enum { CURSOR_ARROW, CURSOR_HAND, CURSOR_HAND_JOY1, CURSOR_HAND_JOY2, CURSOR_HAND_JOY3, CURSOR_HAND_JOY4, CURSOR_COUNT };

	enum { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

	enum { SCROLLBAR_HBACK, SCROLLBAR_VBACK, SCROLLBAR_THUMB, SCROLLBAR_RIGHT, SCROLLBAR_LEFT, SCROLLBAR_UP, SCROLLBAR_DOWN, SCROLLBAR_COUNT };

	static idVec4 colorPurple;
	static idVec4 colorOrange;
	static idVec4 colorYellow;
	static idVec4 colorGreen;
	static idVec4 colorBlue;
	static idVec4 colorRed;
	static idVec4 colorWhite;
	static idVec4 colorBlack;
	static idVec4 colorNone;

protected:
	/*!
		\brief Draws formatted text using the device context with support for color codes, cursor positioning, and text limits

		This function renders text on the device context at the specified coordinates with the given scale and color. It supports Unicode text through UTF-8 encoding and handles color codes embedded
	   within the text string. The function can limit the number of characters drawn and manage cursor positioning for editing contexts. It processes color changes within the text by interpreting
	   color escape sequences and updates the rendering color accordingly. The function also handles special cases like cursor display and character skipping for proper text layout.

		\param x Horizontal coordinate for the text start position
		\param y Vertical coordinate for the text start position
		\param scale Scaling factor for text size
		\param color Color to draw the text with
		\param text Text string to render
		\param adjust Additional spacing adjustment between characters
		\param limit Maximum number of characters to draw, or 0 for no limit
		\param style Text style flags
		\param cursor Position of cursor for editing, or -1 to disable
		\return The total length of the input text string in characters
	*/
	virtual int			DrawText( float x, float y, float scale, idVec4 color, const char* text, float adjust, int limit, int style, int cursor = -1 );

	//! Paints a character glyph at the specified screen coordinates using the provided glyph information.
	void				PaintChar( float x, float y, const scaledGlyphInfo_t& glyphInfo );

	//! Clears the device context state by resetting the initialized flag.
	void				Clear();

	const idMaterial*	cursorImages[CURSOR_COUNT];
	const idMaterial*	scrollBarImages[SCROLLBAR_COUNT];
	const idMaterial*	whiteImage;
	idFont*				activeFont;

	float				xScale;
	float				yScale;
	float				xOffset;
	float				yOffset;

	int					cursor;

	idList<idRectangle> clipRects;

	bool				enableClipping;

	bool				overStrikeMode;

	idMat3				mat;
	bool				matIsIdentity;
	idVec3				origin;
	bool				initialized;
};

/*!
	\class idDeviceContextOptimized
	\brief Optimized device context for rendering with improved clipping and text drawing capabilities.
*/
class idDeviceContextOptimized : public idDeviceContext
{
	/*!
		\brief Clips rectangle coordinates and texture coordinates against a clipping region.

		This function modifies the provided rectangle coordinates and texture coordinates to reflect the portion of the rectangle that intersects with the clipping region defined by clipX1, clipY1,
	   clipX2, and clipY2. It returns true if the rectangle is completely outside the clipping region or becomes degenerate after clipping, indicating it should not be drawn. The texture coordinates
	   are adjusted to correspond to the clipped portion of the rectangle. The function operates in-place on the provided coordinate and texture coordinate pointers.

		\param x Pointer to the x-coordinate of the rectangle
		\param y Pointer to the y-coordinate of the rectangle
		\param w Pointer to the width of the rectangle
		\param h Pointer to the height of the rectangle
		\param s1 Pointer to the first s texture coordinate
		\param t1 Pointer to the first t texture coordinate
		\param s2 Pointer to the second s texture coordinate
		\param t2 Pointer to the second t texture coordinate
		\return True if the rectangle is completely outside the clipping region or becomes degenerate after clipping, false otherwise.
	*/
	virtual bool ClippedCoords( float* x, float* y, float* w, float* h, float* s1, float* t1, float* s2, float* t2 );

	//! Pushes a clipping rectangle onto the clip stack and updates the current clipping boundaries.
	virtual void PushClipRect( idRectangle r );

	//! Removes the top clip rectangle from the stack and updates the current clip boundaries.
	virtual void PopClipRect();
	virtual void EnableClipping( bool b );

	/*!
		\brief Draws text using optimized rendering when possible, falling back to legacy code for special cases.

		This function renders text in the 3D world by converting each character into graphical vertices. It handles color codes within the text, manages scaling and positioning, and supports clipping
	   to prevent rendering outside of view. The function uses a fast path for standard cases and falls back to the base class implementation when special conditions like color changes or cursor
	   positioning are needed. It supports UTF-8 text and uses a material-based approach for rendering glyphs.

		\param x X coordinate of the starting position for the text
		\param y Y coordinate of the starting position for the text
		\param scale Scale factor for the size of the text characters
		\param color Color of the text as RGBA values
		\param text The text string to be rendered
		\param adjust Adjustment value for spacing between characters
		\param limit Maximum number of characters to render, or 0 for no limit
		\param style Text style or alignment option
		\param cursor Cursor position within the text, or -1 if not used
		\return The total number of characters processed and rendered.
	*/
	virtual int	 DrawText( float x, float y, float scale, idVec4 color, const char* text, float adjust, int limit, int style, int cursor = -1 );

	float		 clipX1;
	float		 clipX2;
	float		 clipY1;
	float		 clipY2;
};

#endif /* !__DEVICECONTEXT_H__ */
