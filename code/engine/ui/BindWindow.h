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
#ifndef __BINDWINDOW_H
#define __BINDWINDOW_H

class idUserInterfaceLocal;

/*!
	\class idBindWindow
	\brief Manages window binding operations for user interface elements.

	The idBindWindow class handles the binding of user interface elements to system events, providing functionality for event handling, rendering, and management of bind states. It extends the basic
   window functionality to support interactive binding operations within a user interface context. The class is designed to be constructed with a specific user interface instance and manages its own
   lifecycle through standard constructor and destructor patterns. Event handling is performed through the HandleEvent method which processes system events and returns relevant command strings. The
   Draw method renders the window content at specified positions and times. Memory allocation tracking is available through the Allocated method, and the class supports retrieving window variables by
   name with special handling for bind variables.

*/
class idBindWindow : public idWindow
{
public:
	//! Constructs an idBindWindow object with the specified user interface.
	idBindWindow( idUserInterfaceLocal* gui );
	virtual ~idBindWindow();

	//! Handles system events for window binding operations and returns a command string.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Performs post-parsing setup for the bind window, including updating bind name state and setting window flags.
	virtual void		PostParse();

	//! Renders the window content at the specified position and time
	virtual void		Draw( int time, float x, float y );
	virtual size_t		Allocated() { return idWindow::Allocated(); };

	//! Returns a window variable by name, with special handling for the "bind" variable
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Activates the window and updates the bind name.
	virtual void		Activate( bool activate, idStr& act );

private:
	//! Initializes the bind window common properties.
	void	 CommonInit();
	idWinStr bindName;
	bool	 waitingOnKey;
};

#endif // __BINDWINDOW_H
