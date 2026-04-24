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

#ifndef __LISTGUILOCAL_H__
#define __LISTGUILOCAL_H__

/*!
	\class idListGUILocal
	\brief A GUI list container that manages string elements with associated IDs and integrates with UI interfaces.

	This class provides a specialized list implementation designed for GUI applications, extending functionality for managing string elements with unique identifiers. It supports configuration with UI
   interfaces, addition and removal of elements, selection handling, and state management. The class maintains an internal list of strings and their associated IDs, allowing for indexed access and
   manipulation. It inherits from idList for core list operations and idListGUI for GUI-specific features. The GUI integration enables synchronization between list contents and UI state, including
   selection tracking and state change notifications. Elements are managed through ID-based lookups, with methods supporting both explicit ID assignment and automatic ID generation. The class supports
   clearing all elements, checking configuration status, and controlling state change notifications. Memory management is handled through the base list class, with no explicit ownership semantics
   described in the provided information.

*/
class idListGUILocal : protected idList<idStr, TAG_OLD_UI>, public idListGUI
{
public:
	//! Initializes a new instance of the idListGUILocal class with default values.
	idListGUILocal()
	{
		m_pGUI		   = NULL;
		m_water		   = 0;
		m_stateUpdates = true;
	}

	//! Configures the list GUI with the provided GUI interface and name.
	void Config( idUserInterface* pGUI, const char* name )
	{
		m_pGUI = pGUI;
		m_name = name;
	}

	//! Adds a string value associated with an ID to the list, updating it if the ID already exists
	void Add( int id, const idStr& s );

	//! Adds a new string element to the list and assigns it an ID based on the current element count.
	void Push( const idStr& s );

	//! Removes an element with the specified ID from the list and returns true if successful.
	bool Del( int id );

	//! Clears all elements from the list and resets associated GUI state if a GUI is present
	void Clear();

	//! Returns the number of elements in the list
	int	 Num() { return idList<idStr, TAG_OLD_UI>::Num(); }

	//! Retrieves the ID of the selected item from a GUI list, returning -1 if no valid selection exists.
	int	 GetSelection( char* s, int size, int sel = 0 ) const;

	//! Sets the selection index for the GUI list element.
	void SetSelection( int sel );

	//! Returns the number of selected items in the list GUI.
	int	 GetNumSelections();

	//! Returns true if the GUI has been configured.
	bool IsConfigured() const;

	//! Enables or disables state changes for the list GUI.
	void SetStateChanges( bool enable );

	//! Clears all internal data and resets the GUI local list state
	void Shutdown();

private:
	idUserInterface*		m_pGUI;
	idStr					m_name;
	int						m_water;
	idList<int, TAG_OLD_UI> m_ids;
	bool					m_stateUpdates;

	//! Updates the GUI state variables based on the current list contents and triggers a state change notification.
	void					StateChanged();
};

#endif /* !__LISTGUILOCAL_H__ */
