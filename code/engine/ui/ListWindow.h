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
#ifndef __LISTWINDOW_H
#define __LISTWINDOW_H

class idSliderWindow;

enum { TAB_TYPE_TEXT = 0, TAB_TYPE_ICON = 1 };

struct idTabRect {
	int	   x;
	int	   w;
	int	   align;
	int	   valign;
	int	   type;
	idVec2 iconSize;
	float  iconVOffset;
};

/*!
	\class idListWindow
	\brief A list window implementation that manages selectable items with scrollable display and event handling.

	The idListWindow class provides a user interface element for displaying and managing lists of selectable items. It inherits from idWindow and extends functionality to support scrolling, selection
   management, and event processing within a graphical user interface. The class handles drawing, activation, and state changes while maintaining synchronization with associated buddy windows such as
   scrollers. It supports parsing of internal variables to configure display properties and list behavior, including layout orientation and material settings. Selection state management is handled
   through methods for setting, adding, checking, and clearing selections. The class integrates with a parent GUI system and provides memory allocation tracking. Initialization methods set up tab
   stops, scroller behavior, and common properties for consistent window behavior.

*/
class idListWindow : public idWindow
{
public:
	//! Constructs an idListWindow object with the specified user interface.
	idListWindow( idUserInterfaceLocal* gui );

	//! Handles window events for a list window, processing user input and updating selection state.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Initializes tab stop information for the list window after parsing
	virtual void		PostParse();

	//! Draws the list window at the specified position and time
	virtual void		Draw( int time, float x, float y );

	//! Activates the list window and updates the list when activation occurs
	virtual void		Activate( bool activate, idStr& act );

	//! Updates the top value of the list window based on the scroller's current value when a buddy window updates.
	virtual void		HandleBuddyUpdate( idWindow* buddy );

	//! Updates the list window state and optionally redraws the interface.
	virtual void		StateChanged( bool redraw = false );
	virtual size_t		Allocated() { return idWindow::Allocated(); };

	//! Retrieves a window variable by its name from the window hierarchy.
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Updates the list window by reading items from the GUI state and configuring the scroll range.
	void				UpdateList();

private:
	//! Parses internal variables for the list window, handling various properties like horizontal layout, list name, and material settings.
	virtual bool				   ParseInternalVar( const char* name, idTokenParser* src );

	//! Initializes the common properties and members of the idListWindow class.
	void						   CommonInit();

	//! Initializes a scroller for the list window, setting up vertical or horizontal orientation based on the horizontal parameter.
	void						   InitScroller( bool horizontal );

	//! Sets the current selection to the specified index.
	void						   SetCurrentSel( int sel );

	//! Adds the specified selection index to the list of current selections.
	void						   AddCurrentSel( int sel );

	//! Returns the currently selected item index from the list window.
	int							   GetCurrentSel();

	//! Checks if the item at the specified index is currently selected.
	bool						   IsSelected( int index );

	//! Removes a specific selection index from the list of selected items.
	void						   ClearSelection( int sel );

	idList<idTabRect, TAG_OLD_UI>  tabInfo;
	int							   top;
	float						   sizeBias;
	bool						   horizontal;
	idStr						   tabStopStr;
	idStr						   tabAlignStr;
	idStr						   tabVAlignStr;
	idStr						   tabTypeStr;
	idStr						   tabIconSizeStr;
	idStr						   tabIconVOffsetStr;
	idHashTable<const idMaterial*> iconMaterials;
	bool						   multipleSel;

	idStrList					   listItems;
	idSliderWindow*				   scroller;
	idList<int, TAG_OLD_UI>		   currentSel;
	idStr						   listName;

	int							   clickTime;

	int							   typedTime;
	idStr						   typed;
};

#endif // __LISTWINDOW_H
