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

#ifndef __EDITWINDOW_H__
#define __EDITWINDOW_H__

#include "Window.h"

const int MAX_EDITFIELD = 4096;

class idUserInterfaceLocal;
class idSliderWindow;

/*!
	\class idEditWindow
	\brief Manages editable text input with keyboard handling and visual feedback.

	The idEditWindow class provides a visual interface element for text input, handling keyboard events, cursor positioning, and text editing operations. It inherits from idWindow and integrates with
   user interface systems to manage focus, drawing, and event handling. The class supports various text editing features including activation states, buddy window updates, and synchronization with
   console variables. It maintains internal state for text content, cursor position, and visual appearance, ensuring proper rendering and user interaction. Memory management is handled through
   explicit allocation tracking via the Allocated() method.

*/
class idEditWindow : public idWindow
{
public:
	//! Constructs an edit window with the specified user interface.
	idEditWindow( idUserInterfaceLocal* gui );
	virtual ~idEditWindow();

	//! Draws the edit window content at the specified position and time.
	virtual void		Draw( int time, float x, float y );

	//! Handles UI events for an edit window, processing keyboard and character input while managing text editing operations.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Initializes the edit window after parsing
	virtual void		PostParse();

	//! Sets the cursor to the end of the text and ensures it is visible when the window gains focus.
	virtual void		GainFocus();
	virtual size_t		Allocated() { return idWindow::Allocated(); };

	//! Returns a window variable by name, with special handling for specific variable names
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Handles updates to buddy windows in the edit window.
	virtual void		HandleBuddyUpdate( idWindow* buddy );

	//! Activates or deactivates the edit window and updates its state accordingly.
	virtual void		Activate( bool activate, idStr& act );

	//! Executes a named event based on the provided event name string.
	void				RunNamedEvent( const char* eventName );

private:
	//! Parses internal variables for the edit window, handling various properties like max characters, numeric input, and read-only settings.
	virtual bool	ParseInternalVar( const char* name, idTokenParser* src );

	//! Initializes the cvar reference for the edit window by looking up the cvar string.
	void			InitCvar();

	//! Updates a cvar value by either reading from or writing to the cvar system based on the read parameter.
	void			UpdateCvar( bool read, bool force = false );

	//! Initializes the common properties and members of the edit window.
	void			CommonInit();

	//! Ensures the cursor position is visible in the edit window
	void			EnsureCursorVisible();

	//! Initializes a scroller widget for the edit window, configuring it based on whether it's horizontal or vertical.
	void			InitScroller( bool horizontal );

	int				maxChars;
	int				paintOffset;
	int				cursorPos;
	int				cursorLine;
	int				cvarMax;
	bool			wrap;
	bool			readonly;
	bool			numeric;
	idStr			sourceFile;
	idSliderWindow* scroller;
	idList<int>		breaks;
	float			sizeBias;
	int				textIndex;
	int				lastTextLength;
	bool			forceScroll;
	idWinBool		password;

	idWinStr		cvarStr;
	idCVar*			cvar;

	idWinBool		liveUpdate;
	idWinStr		cvarGroup;
};

#endif /* !__EDITWINDOW_H__ */
