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

class idWindow;

/*!
	\class idUserInterfaceLocal
	\brief Manages the local user interface state, events, and rendering within a system.

	The idUserInterfaceLocal class provides a complete implementation for handling user interfaces with support for state management, event processing, rendering, and persistence. It maintains a
   desktop window structure and manages the interaction between system events and UI elements. The class supports initialization from files, state variable manipulation, and integration with system
   time and input handling. It also provides functionality for drawing cursors, managing references, and handling key binding names. The implementation includes methods for saving and loading UI state
   to/from save files, and supports both interactive and non-interactive UI modes. Memory management is handled through reference counting, and the class can be activated or deactivated as needed.

*/
class idUserInterfaceLocal : public idUserInterface
{
	friend class idUserInterfaceManagerLocal;

public:
	//! Initializes a new instance of the idUserInterfaceLocal class with default values.
	idUserInterfaceLocal();

	//! Destructor for the user interface local class that cleans up the desktop resource.
	virtual ~idUserInterfaceLocal();

	//! Returns the name of the user interface.
	virtual const char*	  Name() const;

	//! Returns a comment on the user interface.
	virtual const char*	  Comment() const;

	//! Returns true if the user interface is interactive.
	virtual bool		  IsInteractive() const;

	//! Initializes the user interface from a file, optionally rebuilding or caching the UI elements.
	virtual bool		  InitFromFile( const char* qpath, bool rebuild = true, bool cache = true );

	//! Handles a system event for the user interface, processing input and updating visual state as needed
	virtual const char*	  HandleEvent( const sysEvent_t* event, int time, bool* updateVisuals );

	//! Executes a named event by passing it to the desktop's RunNamedEvent method.
	virtual void		  HandleNamedEvent( const char* namedEvent );

	//! Redraws the user interface with the specified time and hud flag.
	virtual void		  Redraw( int time, bool hud );

	//! Draws the cursor using the appropriate size based on the desktop flags.
	virtual void		  DrawCursor();

	//! Returns the current state dictionary of the user interface
	virtual const idDict& State() const;

	//! Deletes a state variable from the UI state dictionary by its name
	virtual void		  DeleteStateVar( const char* varName );

	//! Sets a string state variable in the user interface.
	virtual void		  SetStateString( const char* varName, const char* value );

	//! Sets a boolean state variable in the user interface
	virtual void		  SetStateBool( const char* varName, const bool value );

	//! Sets an integer state variable in the user interface
	virtual void		  SetStateInt( const char* varName, const int value );

	//! Sets a float-typed state variable in the user interface
	virtual void		  SetStateFloat( const char* varName, const float value );

	//! Retrieves a string value from the GUI state variables using the specified variable name and default string
	virtual const char*	  GetStateString( const char* varName, const char* defaultString = "" ) const;

	//! Returns the boolean value of a specified GUI state variable, or a default value if the variable is not found
	virtual bool		  GetStateBool( const char* varName, const char* defaultString = "0" ) const;

	//! Returns an integer state variable value from the user interface
	virtual int			  GetStateInt( const char* varName, const char* defaultString = "0" ) const;

	//! Retrieves a floating-point value from the user interface state, using a default value if the variable is not found.
	virtual float		  GetStateFloat( const char* varName, const char* defaultString = "0" ) const;

	//! Updates the GUI state and optionally triggers a redraw when the state changes.
	virtual void		  StateChanged( int time, bool redraw );

	//! Activates or deactivates the user interface and returns a string identifier.
	virtual const char*	  Activate( bool activate, int time );

	//! Triggers the user interface to execute onTrigger scripts at the specified time
	virtual void		  Trigger( int time );

	//! Writes the user interface state to a save game file.
	virtual bool		  WriteToSaveGame( idFile* savefile ) const;

	//! Reads the user interface state from a save file.
	virtual bool		  ReadFromSaveGame( idFile* savefile );

	//! Sets the key binding names for the user interface.
	virtual void		  SetKeyBindingNames();
	virtual bool		  IsUniqued() const { return uniqued; };
	virtual void		  SetUniqued( bool b ) { uniqued = b; };

	//! Sets the cursor position to the specified x and y coordinates.
	virtual void		  SetCursor( float x, float y );

	//! Returns the x-coordinate of the cursor position.
	virtual float		  CursorX() { return cursorX; }

	//! Returns the current Y coordinate of the cursor.
	virtual float		  CursorY() { return cursorY; }

	//! Returns the total memory size occupied by this user interface instance.
	size_t				  Size();

	//! Returns a pointer to the state dictionary for the user interface.
	idDict*				  GetStateDict() { return &state; }

	//! Returns the source file path of the user interface
	const char*			  GetSourceFile() const { return source; }

	//! Returns the time stamp associated with the user interface.
	ID_TIME_T			  GetTimeStamp() const { return timeStamp; }

	//! Returns the desktop window associated with this user interface.
	idWindow*			  GetDesktop() const { return desktop; }

	//! Sets the bind handler window for the user interface.
	void				  SetBindHandler( idWindow* win ) { bindHandler = win; }

	//! Returns true if the user interface is currently active.
	bool				  Active() const { return active; }

	//! Returns the current time value stored in the user interface local instance.
	int					  GetTime() const { return time; }

	//! Sets the UI time value to the specified integer time.
	void				  SetTime( int _time ) { time = _time; }

	//! Clears all references stored in the UI local instance.
	void				  ClearRefs() { refs = 0; }

	//! Increments the reference count for this user interface object.
	void				  AddRef() { refs++; }

	//! Returns the reference count of the user interface instance.
	int					  GetRefs() { return refs; }

	//! Recursively updates key binding names for UI window and its children
	void				  RecurseSetKeyBindingNames( idWindow* window );
	idStr&				  GetPendingCmd() { return pendingCmd; };
	idStr&				  GetReturnCmd() { return returnCmd; };

private:
	bool	  active;
	bool	  loading;
	bool	  interactive;
	bool	  uniqued;

	idDict	  state;
	idWindow* desktop;
	idWindow* bindHandler;

	idStr	  source;
	idStr	  activateStr;
	idStr	  pendingCmd;
	idStr	  returnCmd;
	ID_TIME_T timeStamp;

	float	  cursorX;
	float	  cursorY;

	int		  time;

	int		  refs;
};

/*!
	\class idUserInterfaceManagerLocal
	\brief Manages user interface resources and provides functionality for loading, tracking, and accessing GUI elements within the system.

	This class serves as the primary manager for user interface resources, handling the lifecycle of GUI elements including their allocation, loading, and deallocation. It supports both automatic and
   manual loading of GUIs, with options for enforcing uniqueness and interactivity. The manager tracks active GUIs and provides mechanisms for preloading, reloading, and cleaning up resources during
   level transitions. It also offers utilities for checking GUI existence, listing loaded interfaces, and writing precache commands for GUI files. The class integrates with binary parsing for
   efficient GUI data loading and maintains a collection of allocated GUI objects that can be accessed by their file paths.

*/
class idUserInterfaceManagerLocal : public idUserInterfaceManager
{
	friend class idUserInterfaceLocal;

public:
	//! Initializes the user interface manager local.
	virtual void			 Init();

	//! Shuts down the user interface manager by cleaning up all GUI resources and related systems.
	virtual void			 Shutdown();

	//! Sets the drawing device context based on the g_useNewGuiCode console variable.
	virtual void			 SetDrawingDC();

	//! Initializes a user interface from a file and discards it immediately.
	virtual void			 Touch( const char* name );

	//! Writes touchGui commands for all referenced GUIs to the provided file.
	virtual void			 WritePrecacheCommands( idFile* f );

	//! Sets the width and height dimensions for the user interface
	virtual void			 SetSize( float width, float height );

	//! Clears references for all GUIs when beginning a level load operation.
	virtual void			 BeginLevelLoad();

	//! Completes level load operations by cleaning up unreferenced GUIs and saving generated GUI files.
	virtual void			 EndLevelLoad( const char* mapName );

	//! Preloads UI data for a specified map if binary loading is enabled.
	virtual void			 Preload( const char* mapName );

	//! Reloads user interface files, optionally reloading all files or only those that have changed.
	virtual void			 Reload( bool all );

	//! Lists all user interfaces managed by the system
	virtual void			 ListGuis() const;

	//! Checks if a GUI file exists by attempting to open it.
	virtual bool			 CheckGui( const char* qpath ) const;

	//! Allocates and returns a new user interface instance
	virtual idUserInterface* Alloc() const;

	//! Frees the memory of a user interface object from the manager's collection.
	virtual void			 DeAlloc( idUserInterface* gui );

	/*!
		\brief Finds or loads a user interface GUI by its file path with optional auto-loading and uniqueness constraints

		This function searches for an existing user interface GUI by comparing the provided file path with the source files of loaded GUIs. If a matching GUI is found and it has no references, it will
	   be reinitialized. The function supports auto-loading of GUIs when they are not already loaded, and handles uniqueness constraints based on flags. It returns a reference to the found or loaded
	   GUI, or NULL if the GUI cannot be found or loaded.

		\param qpath The file path of the GUI to find or load
		\param autoLoad If true, automatically loads the GUI if not already present
		\param needInteractive If true, requires the GUI to be interactive
		\param forceUnique If true, forces the GUI to be unique and not shared
		\return A pointer to the found or loaded user interface GUI, or NULL if not found or failed to load
	*/
	virtual idUserInterface* FindGui( const char* qpath, bool autoLoad = false, bool needInteractive = false, bool forceUnique = false );

	//! Returns the demo GUI with the specified file path, or NULL if not found.
	virtual idUserInterface* FindDemoGui( const char* qpath );

	//! Allocates and returns a new idListGUI object.
	virtual idListGUI*		 AllocListGUI() const;

	//! Frees the memory of the specified list GUI object
	virtual void			 FreeListGUI( idListGUI* listgui );

	//! Returns a reference to the binary parser used for UI parsing.
	idTokenParser&			 GetBinaryParser() { return mapParser; }

private:
	idRectangle					  screenRect;
	idDeviceContext				  dcOld;
	idDeviceContextOptimized	  dcOptimized;

	idList<idUserInterfaceLocal*> guis;
	idList<idUserInterfaceLocal*> demoGuis;

	idTokenParser				  mapParser;
};

// These used to be in every window, but they all pointed at the same one in idUserInterfaceManagerLocal.
// Made a global so it can be switched out dynamically to test optimized versions.
extern idDeviceContext* dc;
