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

#ifndef __EDITFIELD_H__
#define __EDITFIELD_H__

/*
===============================================================================

	Edit field

===============================================================================
*/

const int MAX_EDIT_LINE = 256;

typedef struct autoComplete_s {
	bool valid;
	int	 length;
	char completionString[MAX_EDIT_LINE];
	char currentMatch[MAX_EDIT_LINE];
	int	 matchCount;
	int	 matchIndex;
	int	 findMatchIndex;
} autoComplete_t;

/*!
	\class idEditField
	\brief A class that manages an editable text field with cursor positioning, auto-completion, and drawing functionality.

	The idEditField class provides a comprehensive interface for handling text input in an editable field. It maintains an internal buffer for text content, manages cursor position, and supports
   operations such as character insertion, keyboard navigation, and auto-completion of commands and console variables. The class also handles visual rendering of the text field, including cursor
   blinking, color code processing, and scrolling. It supports setting and retrieving the buffer content, configuring the display width, and managing the state of auto-completion. The implementation
   handles various input events including character input, key down events, and paste operations, making it suitable for use in interactive console or text editing interfaces.

*/
class idEditField
{
public:
	//! Initializes a new instance of the idEditField class.
	idEditField();
	~idEditField();

	//! Clears the edit field buffer and resets all associated state variables.
	void  Clear();

	//! Sets the width of the edit field in characters.
	void  SetWidthInChars( int w );

	//! Sets the cursor position for the edit field
	void  SetCursor( int c );

	//! Returns the current cursor position in the edit field.
	int	  GetCursor() const;

	//! Clears the auto-complete state and resets the edit buffer
	void  ClearAutoComplete();

	//! Returns the length of the auto-completed portion of the edit field
	int	  GetAutoCompleteLength() const;

	//! Performs auto-completion of commands and cvars in the edit field
	void  AutoComplete();

	//! Handles character input events for the edit field, processing control commands and inserting characters.
	void  CharEvent( int c );

	//! Handles key down events for the edit field, processing keyboard input for text manipulation and navigation.
	void  KeyDownEvent( int key );

	//! Pastes clipboard content into the edit field
	void  Paste();

	//! Returns a pointer to the internal character buffer of the edit field.
	char* GetBuffer();

	/*!
		\brief Draws the edit field content and cursor on the screen at the specified position

		This function renders the contents of the edit field buffer onto the screen, taking into account scrolling and color codes. It calculates the visible portion of the text based on the current
	   scroll position and the available width. The function also handles cursor drawing, including blink animation and overstrike mode indicators. The cursor position is adjusted to account for color
	   codes in the displayed text.

		\param x The x coordinate of the top-left corner where the edit field should be drawn
		\param y The y coordinate of the top-left corner where the edit field should be drawn
		\param width The maximum number of characters to display in the edit field
		\param showCursor Flag indicating whether to display the cursor
	*/
	void  Draw( int x, int y, int width, bool showCursor );

	//! Sets the internal buffer of the edit field to the provided string.
	void  SetBuffer( const char* buffer );

private:
	int			   cursor;
	int			   scroll;
	int			   widthInChars;
	char		   buffer[MAX_EDIT_LINE];
	autoComplete_t autoComplete;
};

#endif /* !__EDITFIELD_H__ */
