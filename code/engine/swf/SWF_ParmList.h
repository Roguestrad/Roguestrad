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
#ifndef __SWF_PARMLIST_H__
#define __SWF_PARMLIST_H__

// static list for script parameters
static const int SWF_MAX_PARMS = 16;

/*!
	\class idSWFParmList
	\brief A static list implementation for managing script parameters with reduced memory allocations.

	This class provides a specialized static list for handling script parameters in a scripting system. It inherits from idStaticList to leverage pre-allocated memory management and reduce the
   frequency of dynamic allocations during script execution. The class supports appending various types of script variables including objects, functions, strings, floats, integers, and booleans,
   making it flexible for different scripting scenarios. The design intent is to optimize performance by minimizing memory overhead associated with frequent allocation and deallocation of parameter
   lists during script execution.

*/
class idSWFParmList : public idStaticList<idSWFScriptVar, SWF_MAX_PARMS>
{
public:
	//! Initializes an empty script parameter list.
	idSWFParmList() { }

	//! Initializes the parameter list with a specified number of elements.
	explicit idSWFParmList( const int num_ ) { SetNum( num_ ); }

	//! Appends a copy of the provided script variable to this parameter list
	void Append( const idSWFScriptVar& other );

	//! Appends a script object to the parameter list
	void Append( idSWFScriptObject* o );

	//! Appends a script function to the parameter list
	void Append( idSWFScriptFunction* f );

	//! Appends a string to the parameter list
	void Append( const char* s );

	//! Appends a string value to the script parameter list
	void Append( const idStr& s );

	//! Appends a string to the parameter list
	void Append( idSWFScriptString* s );

	//! Appends a float value to the parameter list
	void Append( const float f );

	//! Appends an integer value to the script variable parameter list
	void Append( const int32 i );

	//! Appends a boolean value to the script variable list
	void Append( const bool b );
};

#endif // __SWF_PARMLIST_H__
