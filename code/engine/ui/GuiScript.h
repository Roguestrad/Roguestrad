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
#ifndef __GUISCRIPT_H
#define __GUISCRIPT_H

#include "Window.h"
#include "Winvar.h"

struct idGSWinVar {
	//! Initializes a new instance of the idGSWinVar class with null variable and false ownership flag.
	idGSWinVar()
	{
		var = NULL;
		own = false;
	}
	idWinVar* var;
	bool	  own;
};

class idGuiScriptList;

/*!
	\class idGuiScript
	\brief A class for managing GUI script commands and their execution within a windowing system.

	This class represents a GUI script that can be parsed from a token stream, executed on a window, and managed through save/load operations. It handles the parsing of script commands, parameter
   resolution, and execution within the context of a window. The class maintains internal state for script parameters and conditional structures, and provides mechanisms for memory management,
   resource pre-caching, and persistence. The script execution can modify window properties or trigger other actions based on the defined commands. The class supports both parsing from source and
   serialization to save games.

*/
class idGuiScript
{
	friend class idGuiScriptList;
	friend class idWindow;

public:
	//! Initializes a new instance of the idGuiScript class with default values.
	idGuiScript();

	//! Destructor for the idGuiScript class that cleans up allocated memory for if/else lists and parameters.
	~idGuiScript();

	//! Parses a GUI script command from the provided token parser
	bool Parse( idTokenParser* src );

	//! Executes the GUI script handler associated with this script, passing the provided window and its parameters.
	void Execute( idWindow* win )
	{
		if( handler ) { handler( win, &parms ); }
	}

	//! Fixes up script parameters for a window by processing and resolving variable references and pre-caching resources
	void   FixupParms( idWindow* win );

	//! Returns the total memory size of the GUI script and its parameters.
	size_t Size()
	{
		int sz = sizeof( *this );
		for( int i = 0; i < parms.Num(); i++ ) {
			sz += parms[i].var->Size();
		}
		return sz;
	}

	//! Writes the GUI script state to a save game file
	void WriteToSaveGame( idFile* savefile );

	//! Loads the gui script state from a save game file
	void ReadFromSaveGame( idFile* savefile );

protected:
	int							   conditionReg;
	idGuiScriptList*			   ifList;
	idGuiScriptList*			   elseList;
	idList<idGSWinVar, TAG_OLD_UI> parms;
	void ( *handler )( idWindow* window, idList<idGSWinVar, TAG_OLD_UI>* src );
};

/*!
	\class idGuiScriptList
	\brief Manages a collection of GUI scripts for execution and persistence.

	This class maintains a list of GUI scripts that can be executed against window objects. It provides functionality to append new scripts, execute all scripts in the list, and manage script
   parameters. The class supports persistence operations for saving and loading the script list state. It also provides memory size calculation for resource monitoring purposes.

*/
class idGuiScriptList
{
	idList<idGuiScript*, TAG_OLD_UI> list;

public:
	idGuiScriptList() { list.SetGranularity( 4 ); };
	~idGuiScriptList() { list.DeleteContents( true ); };

	//! Executes all GUI scripts in the list for the given window.
	void   Execute( idWindow* win );

	//! Appends a GUI script to the list
	void   Append( idGuiScript* gs ) { list.Append( gs ); }

	//! Returns the total memory size occupied by the GUI script list and its elements
	size_t Size()
	{
		int sz = sizeof( *this );
		for( int i = 0; i < list.Num(); i++ ) {
			sz += list[i]->Size();
		}
		return sz;
	}

	//! Processes and updates script parameters for a given window.
	void FixupParms( idWindow* win );

	//! Writes all GUI script list entries to the specified save file
	void WriteToSaveGame( idFile* savefile );

	//! Reads all GUI script list items from a save game file.
	void ReadFromSaveGame( idFile* savefile );
};

#endif // __GUISCRIPT_H
