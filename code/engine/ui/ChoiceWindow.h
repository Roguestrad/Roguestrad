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
#ifndef __CHOICEWINDOW_H
#define __CHOICEWINDOW_H

#include "Window.h"

class idUserInterfaceLocal;

/*!
	\class idChoiceWindow
	\brief A window class that manages user choices with associated scripts and cvar integration.

	The idChoiceWindow class extends window functionality to support user selection from a list of options, where each option can trigger scripts and synchronize with console variables. It handles
   event processing, rendering, activation, and state management for choice-based UI elements. The class integrates with the console variable system to maintain persistent state and supports both
   direct cvar binding and GUI dictionary updates. Memory allocation tracking is provided through a virtual method for monitoring used resources. The window can be initialized with default values,
   parse internal variables for configuration, and update its state based on cvar changes or user interaction.

*/
class idChoiceWindow : public idWindow
{
public:
	//! Constructs an idChoiceWindow object with the specified user interface.
	idChoiceWindow( idUserInterfaceLocal* gui );
	virtual ~idChoiceWindow();

	//! Handles input events for a choice window, updating the selected option and executing associated scripts.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Processes post-parsing initialization and updates for the choice window
	virtual void		PostParse();

	//! Renders the choice window at the specified position with the given time
	virtual void		Draw( int time, float x, float y );

	//! Activates the choice window and updates its GUI state based on the current choice.
	virtual void		Activate( bool activate, idStr& act );
	virtual size_t		Allocated() { return idWindow::Allocated(); };

	//! Retrieves a window variable by name from the choice window, returning predefined variables for specific names or delegating to the base window class for others.
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Executes a named event for the choice window, specifically handling cvar read and write operations.
	void				RunNamedEvent( const char* eventName );

private:
	//! Parses internal variables for the choice window, handling 'choicetype' and 'currentchoice' parameters.
	virtual bool  ParseInternalVar( const char* name, idTokenParser* src );

	//! Initializes the choice window with default values.
	void		  CommonInit();

	//! Updates the choice window's current selection based on the associated cvar or GUI state.
	void		  UpdateChoice();

	//! Validates the current choice index and ensures there is at least one choice available.
	void		  ValidateChoice();

	//! Initializes variables for the choice window by processing associated cvar and gui strings.
	void		  InitVars();

	//! Updates variables by reading from or writing to the console variable system and GUI dictionary.
	void		  UpdateVars( bool read, bool force = false );

	//! Updates the choice lists and their corresponding values when the window variables change
	void		  UpdateChoicesAndVals();

	int			  currentChoice;
	int			  choiceType;
	idStr		  latchedChoices;
	idWinStr	  choicesStr;
	idStr		  latchedVals;
	idWinStr	  choiceVals;
	idStrList	  choices;
	idStrList	  values;

	idWinStr	  guiStr;
	idWinStr	  cvarStr;
	idCVar*		  cvar;
	idMultiWinVar updateStr;

	idWinBool	  liveUpdate;
	idWinStr	  updateGroup;
};

#endif // __CHOICEWINDOW_H
