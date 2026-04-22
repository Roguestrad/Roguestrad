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

#ifndef __CMDARGS_H__
#define __CMDARGS_H__

/*!
	\class idCmdArgs
	\brief A class for managing command-line arguments and parsing text into tokenized argument lists.

	The idCmdArgs class provides functionality for handling command-line style argument parsing and management. It can tokenize input strings into arguments, store and manipulate argument lists, and
   reconstruct arguments into strings. The class supports both manual construction with tokenized input and dynamic argument appending. It is designed for use in command processing scenarios where
   text needs to be parsed into discrete arguments while preserving the ability to reconstruct the argument list into a formatted string. The class maintains a collection of string arguments that can
   be accessed by index or retrieved as a complete argument list.

*/
class idCmdArgs
{
public:
	//! Initializes an empty command arguments object.
	idCmdArgs() { argc = 0; }

	//! Constructs an idCmdArgs object by tokenizing the provided text string.
	idCmdArgs( const char* text, bool keepAsStrings ) { TokenizeString( text, keepAsStrings ); }

	//! Assigns the contents of another idCmdArgs object to this object
	void			   operator=( const idCmdArgs& args );

	//! Returns the argument count of the command line arguments.
	int				   Argc() const { return argc; }

	//! Returns the argument at the specified index, or an empty string if the index is out of bounds.
	const char*		   Argv( int arg ) const { return ( arg >= 0 && arg < argc ) ? argv[arg] : ""; }

	//! Returns a single string containing arguments from start to end, with optional escaping.
	const char*		   Args( int start = 1, int end = -1, bool escapeArgs = false ) const;

	//! Parses a null-terminated string into command-line arguments, separating tokens by whitespace and comments while optionally preserving string literals.
	void			   TokenizeString( const char* text, bool keepAsStrings );

	//! Appends a new argument to the command line arguments list.
	void			   AppendArg( const char* text );

	//! Clears all command arguments stored in the idCmdArgs object.
	void			   Clear() { argc = 0; }

	//! Returns the command line arguments as an array of string pointers.
	const char* const* GetArgs( int* argc );

private:
	static const int MAX_COMMAND_ARGS	= 64;
	static const int MAX_COMMAND_STRING = 2 * MAX_STRING_CHARS;

	int				 argc;							// number of arguments
	char*			 argv[MAX_COMMAND_ARGS];		// points into tokenized
	char			 tokenized[MAX_COMMAND_STRING]; // will have 0 bytes inserted
};

#endif /* !__CMDARGS_H__ */
