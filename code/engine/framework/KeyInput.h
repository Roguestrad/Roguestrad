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

#ifndef __KEYINPUT_H__
#define __KEYINPUT_H__

struct keyBindings_t {
	idStr keyboard;
	idStr mouse;
	idStr gamepad;
};

class idSerializer;

//! Converts a USB HID code to a K_ code
int Key_CovertHIDCode( int hid );

/*!
	\class idKeyInput
	\brief Manages keyboard input handling, key state tracking, and command binding for user actions.

	The idKeyInput class provides comprehensive keyboard input management, including tracking key states, managing command bindings, and handling input event processing. It supports initialization and
   shutdown procedures for the input system, maintains key state information for both system and user commands, and provides facilities for binding commands to keys. The class handles various
   key-related operations such as converting between key names and numbers, retrieving localized key names for display, and executing bound commands. It also offers functionality for managing multiple
   types of bindings including keyboard, mouse, and gamepad inputs. The implementation supports console command completion for key names and provides methods for clearing input states and writing
   bindings to files. The system tracks global key up/down states and maintains an overstrike mode setting for keyboard input.

*/
class idKeyInput
{
public:
	//! Initializes the key input system by allocating memory for key states and registering console commands for key binding operations.
	static void			 Init();

	//! Shuts down the key input system by deallocating the keys array.
	static void			 Shutdown();

	//! Completes command arguments with key names by invoking a callback for each key name.
	static void			 ArgCompletion_KeyName( const idCmdArgs& args, void ( *callback )( const char* s ) );

	//! Tracks global key up/down state for system key events
	static void			 PreliminaryKeyEvent( int keyNum, bool down );

	//! Checks if a specific key is currently pressed down.
	static bool			 IsDown( int keyNum );

	//! Returns the user command action associated with the specified key number.
	static int			 GetUsercmdAction( int keyNum );

	//! Returns the current overstrike mode state for keyboard input.
	static bool			 GetOverstrikeMode();

	//! Sets the overstrike mode state for keyboard input.
	static void			 SetOverstrikeMode( bool state );

	//! Clears the state of all input keys and user command generation.
	static void			 ClearStates();

	//! Converts a string representation of a key to its corresponding key number
	static keyNum_t		 StringToKeyNum( const char* str );

	//! Converts a key number to its string representation
	static const char*	 KeyNumToString( keyNum_t keyNum );

	//! Returns a localized string representation of a key number suitable for display on screen.
	static const char*	 LocalizedKeyName( keyNum_t keyNum );

	//! Sets the binding for a specified key number to the given command string.
	static void			 SetBinding( int keyNum, const char* binding );

	//! Returns the binding string for a specified key number
	static const char*	 GetBinding( int keyNum );

	//! Removes the binding associated with the specified command string
	static bool			 UnbindBinding( const char* bind );

	//! Returns the number of key bindings associated with the specified command name.
	static int			 NumBinds( const char* binding );

	//! Executes the command bound to a given key number.
	static bool			 ExecKeyBinding( int keyNum );

	//! Returns the localized name of the key for the given binding.
	static const char*	 KeysFromBinding( const char* bind );

	//! Returns the binding for the localized name of the key.
	static const char*	 BindingFromKey( const char* key );

	//! Checks if a specific key is bound to the given command binding
	static bool			 KeyIsBoundTo( int keyNum, const char* binding );

	//! Writes key binding commands to the specified file
	static void			 WriteBindings( idFile* f );

	//! Converts a key binding string into separate keyboard, mouse, and gamepad bindings.
	static keyBindings_t KeyBindingsFromBinding( const char* bind, bool firstOnly = false, bool localized = false );
};

#endif /* !__KEYINPUT_H__ */
