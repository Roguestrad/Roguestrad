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
#pragma hdrstop
#include "precompiled.h"

/*!
	\brief Manages and retrieves the linked list of console commands for the command system.

	This function serves as a singleton accessor for the global list of console commands. It maintains a static pointer to the head of the command link list and allows setting a new head when a
   command link is provided. The function is used during command system initialization to iterate through all registered commands and add them to the system. It supports both retrieving the current
   command list and setting a new command list head, making it a key part of the command registration and enumeration process.

	\param cl The command link to set as the head of the command list, or NULL to retrieve the current list
	\return A pointer to the head of the command link list
*/
idCommandLink* CommandLinks( idCommandLink* cl )
{
	static idCommandLink* commandLinks = NULL;
	if( cl != NULL ) {
		commandLinks = cl;
	}
	return commandLinks;
}

idCommandLink* commandLinks = NULL;

idCommandLink::idCommandLink( const char* cmdName, cmdFunction_t function, const char* description, argCompletion_t argCompletion )
{
	next = CommandLinks();
	CommandLinks( this );
	cmdName_	   = cmdName;
	function_	   = function;
	description_   = description;
	argCompletion_ = argCompletion;
}
