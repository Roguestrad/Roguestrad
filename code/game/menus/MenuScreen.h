/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014 Robert Beckebans

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
#ifndef __MENUSCREEN_H__
#define __MENUSCREEN_H__

#ifdef __TYPEINFOGEN__
	#define VR_OPTIONS 0
#endif

enum mainMenuTransition_t { MENU_TRANSITION_INVALID = -1, MENU_TRANSITION_SIMPLE, MENU_TRANSITION_ADVANCE, MENU_TRANSITION_BACK, MENU_TRANSITION_FORCE };

enum cursorState_t {
	CURSOR_NONE,
	CURSOR_IN_COMBAT,
	CURSOR_TALK,
	CURSOR_GRABBER,
	CURSOR_ITEM,
};

/*!
	\class idLBRowBlock
	\brief A block used for managing row data in a linked list structure.
*/
class idLBRowBlock
{
public:
	//! Initializes a new instance of the idLBRowBlock class with default values.
	idLBRowBlock() :
		lastTime( 0 ),
		startIndex( 0 )
	{
	}

	int									 lastTime;
	int									 startIndex;
	idList<idLeaderboardCallback::row_t> rows;
};

enum leaderboardFilterMode_t { LEADERBOARD_FILTER_OVERALL = 0, LEADERBOARD_FILTER_MYSCORE = 1, LEADERBOARD_FILTER_FRIENDS = 2 };

/*!
	\class idLBCache
	\brief Manages cached leaderboard data with filtering and scrolling capabilities.

	The idLBCache class provides a mechanism for caching leaderboard data, supporting filtering, scrolling, and updating operations. It maintains leaderboard definitions, row blocks, and handles
   asynchronous updates from leaderboard callbacks. The class supports cycling through different filter modes and can display gamer card UI for specific leaderboard entries. It tracks the current view
   state including row offset, entry index, and local index to manage navigation through leaderboard data. The cache can handle loading new leaderboards and reports errors that occur during
   leaderboard operations.

*/
class idLBCache
{
public:
	static const int					 NUM_ROW_BLOCKS				= 5;
	static const leaderboardFilterMode_t DEFAULT_LEADERBOARD_FILTER = LEADERBOARD_FILTER_OVERALL;

	//! Initializes a new instance of the idLBCache class with default values.
	idLBCache() :
		def( NULL ),
		filter( DEFAULT_LEADERBOARD_FILTER ),
		pendingDef( NULL ),
		pendingFilter( DEFAULT_LEADERBOARD_FILTER ),
		requestingRows( false ),
		loadingNewLeaderboard( false ),
		numRowsInLeaderboard( 0 ),
		entryIndex( 0 ),
		rowOffset( 0 ),
		localIndex( -1 ),
		errorCode( LEADERBOARD_DISPLAY_ERROR_NONE )
	{
	}

	//! Processes pending leaderboard updates and requests.
	void								Pump();

	//! Resets the leaderboard cache state to its initial empty configuration.
	void								Reset();

	//! Sets the leaderboard definition and filter for caching and starts loading the initial leaderboard data.
	void								SetLeaderboard( const leaderboardDefinition_t* def_, leaderboardFilterMode_t filter_ = DEFAULT_LEADERBOARD_FILTER );

	//! Cycles the leaderboard filter through different options.
	void								CycleFilter();

	//! Returns the current leaderboard filter mode.
	leaderboardFilterMode_t				GetFilter() const { return filter; }

	//! Returns a localized string representing the filter type for leaderboards.
	idStr								GetFilterStrType();

	//! Updates the leaderboard cache cursor index and row offset by the specified amount, returning true if any change occurred.
	bool								Scroll( int amount );

	//! Updates the scroll offset in the leaderboard cache by the specified amount and returns whether the offset changed.
	bool								ScrollOffset( int amount );

	//! Returns a pointer to a free row block from the cache, preferring empty blocks or the oldest used block if none are available.
	idLBRowBlock*						FindFreeRowBlock();

	//! Updates the leaderboard cache with data from the provided callback
	void								Update( const idLeaderboardCallback* callback );

	//! Returns a pointer to the leaderboard row data at the specified index, or NULL if the row is not available.
	const idLeaderboardCallback::row_t* GetLeaderboardRow( int row );

	//! Returns a pointer to the leaderboard definition stored in the cache.
	const leaderboardDefinition_t*		GetLeaderboard() const { return def; }

	//! Returns the number of rows in the leaderboard.
	int									GetNumRowsInLeaderboard() const { return numRowsInLeaderboard; }

	//! Returns the index of the entry in the cache.
	int									GetEntryIndex() const { return entryIndex; }

	//! Returns the row offset value stored in the idLBCache instance.
	int									GetRowOffset() const { return rowOffset; }

	//! Returns the local index of the LBCache entry.
	int									GetLocalIndex() const { return localIndex; }

	//! Returns the error code from the leaderboard callback.
	leaderboardDisplayError_t			GetErrorCode() const { return errorCode; }

	//! Checks if the cache is currently requesting rows.
	bool								IsRequestingRows() const { return requestingRows; }

	//! Returns true if a new leaderboard is currently being loaded.
	bool								IsLoadingNewLeaderboard() const { return loadingNewLeaderboard; }

	//! Sets the entry index of the LBCache to the specified value.
	void								SetEntryIndex( int value ) { entryIndex = value; }

	//! Sets the row offset value for the LBCache.
	void								SetRowOffset( int value ) { rowOffset = value; }

	//! Displays the gamer card UI for the specified leaderboard row.
	void								DisplayGamerCardUI( const idLeaderboardCallback::row_t* row );

private:
	//! Converts a leaderboard error code to a display error code.
	leaderboardDisplayError_t	   CallbackErrorToDisplayError( leaderboardError_t errorCode );

	idLBRowBlock				   rowBlocks[NUM_ROW_BLOCKS];

	const leaderboardDefinition_t* def;
	leaderboardFilterMode_t		   filter;

	// Pending def and filter are simply used to queue up SetLeaderboard calls when the system is currently
	// busy waiting on results from a previous SetLeaderboard/GetLeaderboardRow call.
	// This is so we only have ONE request in-flight at any given time.
	const leaderboardDefinition_t* pendingDef;
	leaderboardFilterMode_t		   pendingFilter;

	bool						   requestingRows;		  // True while requested rows are in flight
	bool						   loadingNewLeaderboard; // True when changing to a new leaderboard (or filter type)

	int							   numRowsInLeaderboard; // Total rows in this leaderboard (they won't all be cached though)
	int							   entryIndex;			 // Relative row offset (from top of viewing window)
	int							   rowOffset;			 // Absolute row offset
	int							   localIndex;			 // Row for the master local user (-1 means not on leaderboard)

	leaderboardDisplayError_t	   errorCode; // Error state of the leaderboard
};

/*!
	\class idMenuScreen
	\brief Base class for menu screens that manages UI state and navigation.

	The idMenuScreen class serves as the foundational element for menu screens, inheriting from idMenuWidget to provide standard UI widget functionality. It manages the lifecycle and visual
   presentation of menu screens including showing, hiding, and transitioning between different menu states. The class handles user interactions through event observation and updates the menu's visual
   state. It also manages shortcut key command bindings for UI navigation and supports integration with SWF-based GUIs. The class is designed to be extended by specific menu screen implementations
   that provide the actual menu content and behavior.

*/
class idMenuScreen : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuScreen class with default values.
	idMenuScreen();
	virtual ~idMenuScreen();

	//! Updates the menu screen and its child elements
	virtual void Update();

	//! Initializes and updates the shortcut key command bindings for UI navigation in the menu screen.
	virtual void UpdateCmds();

	//! Handles menu transitions by sending appropriate events based on the transition type.
	virtual void HandleMenu( const mainMenuTransition_t type );

	//! Displays the menu screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the menu screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	//! Processes widget events for the menu screen, handling command bar button presses.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Sets the SWF GUI for the menu screen.
	virtual void SetScreenGui( idSWF* gui ) { menuGUI = gui; }

protected:
	idSWF*				 menuGUI;
	mainMenuTransition_t transition;
};

/*!
	\class idMenuScreen_PDA_UserData
	\brief Manages the presentation and interaction of user data within a PDA menu system.

	This class implements a screen for displaying and managing user data within a PDA menu interface. It inherits from idMenuScreen and provides specific functionality for initializing, updating, and
   handling user interactions with PDA user data elements. The class maintains references to various PDA widgets including user data, objectives, and audio files. It handles navigation actions to
   transition between different menu screens and manages audio playback for user data related content. The implementation supports different transition types for showing and hiding the screen, and
   processes widget actions including repeater events for continuous interactions. The class is designed to integrate within a larger menu system where it serves as a dedicated screen for user data
   presentation and interaction.

*/
class idMenuScreen_PDA_UserData : public idMenuScreen
{
public:
	//! Constructs a new instance of the idMenuScreen_PDA_UserData class.
	idMenuScreen_PDA_UserData() { }

	virtual ~idMenuScreen_PDA_UserData() { }

	//! Initializes the PDA user data screen with the provided menu handler.
	virtual void				 Initialize( idMenuHandler* data );

	//! Updates the user data screen for the PDA menu
	virtual void				 Update();

	//! Displays the PDA user data screen with the specified transition type.
	virtual void				 ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the menu screen with the specified transition type.
	virtual void				 HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user interactions and actions within the PDA user data screen

		This function processes various widget actions for the PDA user data screen, including handling audio playback, navigation, and repeater events. It checks if the current screen is the user
	   data area and performs different actions based on the widget action type. For audio playback, it stops existing audio logs and starts new ones, while for navigation it handles going back to
	   previous screens. The function also manages widget action repeaters for continuous actions.

		\param action The widget action to be processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag indicating whether the action should be forcefully handled
		\return True if the action was handled, false otherwise
		\throws Assertion error if the parameter count is not equal to 2 in START_REPEATER action
	*/
	virtual bool				 HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Returns a pointer to the PDA user data widget.
	idMenuWidget_PDA_UserData*	 GetUserData() { return &pdaUserData; }

	//! Returns a pointer to the PDA objective widget.
	idMenuWidget_PDA_Objective*	 GetObjective() { return &pdaObjectiveSimple; }

	//! Returns a pointer to the audio files widget for the PDA user data screen.
	idMenuWidget_PDA_AudioFiles* GetAudioFiles() { return &pdaAudioFiles; }

private:
	idMenuWidget_PDA_UserData	pdaUserData;
	idMenuWidget_PDA_Objective	pdaObjectiveSimple;
	idMenuWidget_PDA_AudioFiles pdaAudioFiles;
};

/*!
	\class idMenuScreen_PDA_UserEmails
	\brief Manages the presentation and interaction with user emails within a PDA interface.

	This class implements the user emails screen for a PDA interface, handling the display and navigation of email content. It inherits from idMenuScreen and provides functionality for initializing
   the screen layout, updating email information, handling user actions such as selection and navigation, and managing screen transitions. The class is designed to work within a larger menu system and
   interfaces with email data through widget components. It supports operations like showing or hiding the email interface, updating email displays based on selection, and handling scrolling events
   for email lists. The implementation manages the visual representation of email data and provides methods for integrating with the broader PDA user interface.

*/
class idMenuScreen_PDA_UserEmails : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_PDA_UserEmails class.
	idMenuScreen_PDA_UserEmails() :
		readingEmails( false ),
		scrollEmailInfo( false )
	{
	}

	virtual ~idMenuScreen_PDA_UserEmails() { }

	//! Updates the PDA user emails screen by configuring command bar buttons and handling email selection.
	virtual void				 Update();

	//! Initializes the user emails screen by setting up the GUI and widget hierarchy.
	virtual void				 Initialize( idMenuHandler* data );

	//! Displays the user emails screen with the specified transition effect.
	virtual void				 ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the user emails screen and handles transition logic for the PDA interface.
	virtual void				 HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the PDA user emails screen, managing navigation, email selection, and screen transitions.

		This function processes various widget actions such as closing the screen, navigating back, refreshing email data, selecting emails, and handling hover events. It returns true if the action
	   was handled, false otherwise. The function also manages the display of individual emails and updates the email list when needed.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Flag indicating whether the action should be forced as handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool				 HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles widget events for the user emails screen in the PDA interface.
	virtual void				 ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Returns a reference to the email inbox widget.
	idMenuWidget_PDA_EmailInbox& GetInbox() { return pdaInbox; }

	//! Handles scrolling and action events for email lists in the PDA user emails screen
	bool						 ScrollCorrectList( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget );

	//! Shows or hides the email interface based on the show parameter.
	void						 ShowEmail( bool show );

	//! Updates the email information display based on the currently selected PDA and email index.
	void						 UpdateEmail();

private:
	idMenuWidget_PDA_EmailInbox pdaInbox;
	idMenuWidget_InfoBox		emailInfo;
	idMenuWidget_ScrollBar		emailScrollbar;
	bool						readingEmails;
	bool						scrollEmailInfo;
};

/*!
	\class idMenuScreen_PDA_VideoDisks
	\brief A menu screen implementation for managing video disks within a PDA interface.

	This class provides the user interface for browsing and playing video disks in a PDA menu system. It manages the display of video content, handles user interactions such as selecting videos and
   toggling playback, and coordinates with a data handler for menu state. The screen supports transitions for showing and hiding, and can process various widget actions including navigation and
   selection events. It maintains references to the currently active video and can clear or retrieve this reference as needed. The class is designed to integrate with a larger menu system and supports
   specific behaviors for video disk playback within the PDA context.

*/
class idMenuScreen_PDA_VideoDisks : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_PDA_VideoDisks class.
	idMenuScreen_PDA_VideoDisks() :
		activeVideo( NULL )
	{
	}

	virtual ~idMenuScreen_PDA_VideoDisks() { }

	//! Initializes the video disks menu screen with the provided data handler.
	virtual void	   Initialize( idMenuHandler* data );

	//! Updates the video disks menu screen, handling command bar buttons and video display.
	virtual void	   Update();

	//! Displays the video disks screen for the PDA menu.
	virtual void	   ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the video disks screen with the specified transition type.
	virtual void	   HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the PDA video disks screen, managing navigation and widget interactions.

		This function processes widget actions such as going back, starting or stopping repeaters, and pressing the focused widget. It checks if the current screen is the video disks area before
	   handling actions. When the press focused action is detected, it toggles video disk playback and updates the screen. The function delegates unhandled actions to the base class implementation.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget triggering the action
		\param forceHandled Optional parameter to force handling of the action, defaults to false
		\return True if the action was handled, false otherwise
	*/
	virtual bool	   HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Toggles the playback of a video disk for the local player.
	void			   ToggleVideoDiskPlay();
	void			   UpdateVideoDetails();

	//! Handles the selection of a video to play from the PDA video disks screen.
	void			   SelectedVideoToPlay( int index );

	//! Clears the reference to the currently active video.
	void			   ClearActiveVideo() { activeVideo = NULL; }

	//! Returns a pointer to the currently active video declaration.
	const idDeclVideo* GetActiveVideo() { return activeVideo; }

private:
	idMenuWidget_ScrollBar											scrollbar;
	idMenuWidget_DynamicList										pdaVideoList;
	idMenuWidget_PDA_VideoInfo										videoDetails;
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> videoItems;
	const idDeclVideo*												activeVideo;
};

/*!
	\class idMenuScreen_PDA_Inventory
	\brief Manages the presentation and interaction of weapon inventory within a PDA interface.

	This class implements the user interface screen for displaying and managing weapon inventory in a PDA system. It handles the visual representation of available weapons, user input processing for
   navigation and selection, and the mechanics of equipping weapons. The screen integrates with a menu handler to manage its display state and transitions, and processes widget actions to enable user
   interaction with the inventory items. The class maintains awareness of which weapons are visible in the inventory list and provides functionality to retrieve weapon information by index. It
   supports standard menu screen operations like showing, hiding, and updating the display, while coordinating with the broader menu system to handle navigation commands and action processing.

*/
class idMenuScreen_PDA_Inventory : public idMenuScreen
{
public:
	//! Constructs a new instance of the PDA inventory menu screen.
	idMenuScreen_PDA_Inventory() { }

	//! Initializes the PDA inventory screen with the provided menu handler data
	virtual void Initialize( idMenuHandler* data );

	//! Updates the PDA inventory screen display and handles user input for weapon selection.
	virtual void Update();

	//! Initializes and displays the PDA inventory screen for the player
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the inventory screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the PDA inventory screen, processing widget interactions and navigation commands.

		This function processes various widget actions such as joystick input, navigation commands, and item selection for the PDA inventory screen. It checks if the current screen is the inventory
	   screen and handles specific actions like equipping weapons, navigating back, repeating actions, and scrolling through items. The function also manages the updating of the inventory list when
	   items are selected or moved.

		\param action The widget action being processed
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Equips the selected weapon from the PDA inventory screen.
	void		 EquipWeapon();

	//! Returns the name of a weapon at the specified index from the player's inventory.
	const char*	 GetWeaponName( int index );

	//! Determines if a weapon at the specified index is visible in the PDA inventory.
	bool		 IsVisibleWeapon( int index );

private:
	idMenuWidget_Carousel itemList;
	idMenuWidget_InfoBox  infoBox;
};

/*!
	\class idMenuScreen_Shell_Root
	\brief Provides the main root menu screen interface for shell-based navigation.

	This class implements the primary menu screen for shell navigation, serving as the entry point for user interaction within the menu system. It handles initialization, display, and updates of the
   root menu screen while managing platform-specific behavior and transition effects. The class integrates with a menu handler to process user actions and route commands appropriately. It supports
   different game modes and platform configurations, including demo mode and online gameplay restrictions. The menu screen includes functionality for navigating to various game states such as
   settings, multiplayer sessions, and quitting the application. It also manages the focus index for menu options and provides access to help widgets. The implementation includes specialized handling
   for different platform types, ensuring appropriate command bar configurations and user experience.

*/
class idMenuScreen_Shell_Root : public idMenuScreen
{
public:
	//! Constructs a new root shell menu screen instance.
	idMenuScreen_Shell_Root() :
		options( NULL ),
		helpWidget( NULL )
	{
	}

	//! Initializes the root shell menu screen with the provided menu handler data.
	virtual void	   Initialize( idMenuHandler* data );

	//! Updates the root menu screen by configuring command bar buttons based on platform and demo mode settings.
	virtual void	   Update();

	//! Displays the root menu screen with appropriate options based on the game mode and platform.
	virtual void	   ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the root menu screen with the specified transition type.
	virtual void	   HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles menu actions for the root shell screen, processing user interactions and routing commands to appropriate game states.

		This function processes widget actions for the root menu screen, handling navigation, menu bar scrolling, and command execution. It checks if the current screen is the root screen and returns
	   false if not. For specific actions like pressing focused items or scrolling horizontally, it performs specialized logic based on the platform. Command actions are handled by executing specific
	   game commands such as starting demos, navigating to settings, quitting, or initiating multiplayer sessions. The function also manages online gameplay permissions and displays appropriate
	   dialogs when necessary.

		\param action The widget action to handle
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force handling of the action even if it would normally be ignored
		\return True if the action was handled, false otherwise
	*/
	virtual bool	   HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles the exit game button click by showing a quit dialog.
	void			   HandleExitGameBtn();

	//! Returns the focus index of the options menu or zero if options is null.
	int				   GetRootIndex();

	//! Sets the focus index of the options menu.
	void			   SetRootIndex( int index );

	//! Returns the help widget associated with the root shell menu screen.
	idMenuWidget_Help* GetHelpWidget() { return helpWidget; }

private:
	idMenuWidget_DynamicList* options;
	idMenuWidget_Help*		  helpWidget;
};

/*!
	\class idMenuScreen_Shell_Pause
	\brief Manages the game pause menu screen interface and user interactions.

	Provides the user interface and handling logic for the game pause menu screen. The class initializes and updates the pause menu based on game state, manages visibility transitions, and processes
   user actions through widget event handling. It supports showing and hiding the pause screen with different transition types and handles specific commands like exit game and restart game by
   displaying confirmation dialogs. The class integrates with a menu handler system to manage menu data and screen transitions.

*/
class idMenuScreen_Shell_Pause : public idMenuScreen
{
public:
	//! Constructs a new pause menu screen with default options and non-MP pause state.
	idMenuScreen_Shell_Pause() :
		options( NULL ),
		isMpPause( false )
	{
	}

	//! Initializes the pause menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the pause menu screen by configuring command bar buttons based on game state.
	virtual void Update();

	//! Displays the pause screen with appropriate menu options based on game state and mode.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the pause screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the pause menu screen by processing widget commands and dispatching to appropriate handler functions.

		This function processes widget actions specific to the pause menu screen. It first checks if the menu data is valid and if the current screen is the root menu. If not, it returns false to
	   indicate the action was not handled. The function examines the command type and parameters, and based on the command, it either calls specific handler functions like HandleRestartBtn or
	   HandleExitGameBtn, or transitions to different menu screens. For commands that are not explicitly handled, it delegates to the parent class's HandleAction method. The function returns true if
	   the action was handled, and false otherwise.

		\param action The widget action that was triggered
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Indicates whether the action should be marked as handled regardless of previous handling
		\return True if the action was handled by this function, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles the exit game button press by showing a confirmation dialog.
	void		 HandleExitGameBtn();

	//! Handles the restart button press event by showing a confirmation dialog.
	void		 HandleRestartBtn();

private:
	idMenuWidget_DynamicList* options;
	bool					  isMpPause;
};

/*!
	\class idMenuScreen_Shell_Singleplayer
	\brief A menu screen class for managing singleplayer game options and transitions.

	This class implements a menu screen for singleplayer game scenarios, handling user interactions and game state management. It provides functionality to initialize menu data, update screen
   elements, display or hide the menu with transitions, and process user actions such as navigation and game start options. The screen supports different menu states based on save game availability,
   allowing players to continue existing games, start new ones, or load saved games. The implementation includes handling of confirmation dialogs for new game starts and management of menu options
   based on available save data.

*/
class idMenuScreen_Shell_Singleplayer : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_Singleplayer class.
	idMenuScreen_Shell_Singleplayer() :
		canContinue( false ),
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the singleplayer menu screen with the provided data handler.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the singleplayer menu screen by configuring command bar buttons and binding sprites.
	virtual void Update();

	//! Displays the singleplayer menu screen with appropriate options based on save game availability.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the singleplayer menu screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the singleplayer menu screen, processing navigation and game start options.

		This function processes widget actions for the singleplayer menu screen. It checks if the current screen is the campaign screen and handles navigation back to the root menu. For the press
	   focused action, it determines the selected option and either continues an existing game, starts a new game with a confirmation dialog, or loads a saved game. The function also handles cases
	   where no saved games exist by presenting different menu options.

		\param action The widget action that occurred
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Sets whether the player can continue the game.
	void		 SetCanContinue( bool valid ) { canContinue = valid; }

	//! Continues the game by loading the most recent save game or showing a damage warning dialog.
	void		 ContinueGame();

private:
	bool					  canContinue;
	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

/*!
	\class idMenuScreen_Shell_Settings
	\brief Manages the shell settings menu screen including initialization, updates, and user action handling.
*/
class idMenuScreen_Shell_Settings : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_Settings class.
	idMenuScreen_Shell_Settings() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the settings menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the settings menu screen by configuring command bar buttons and binding UI elements.
	virtual void Update();

	//! Displays the shell settings screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the settings screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the settings menu screen, routing commands to appropriate sub-menus and managing navigation.

		This function processes widget actions within the settings menu screen. It first checks if the current screen is the settings menu, and if not, it returns false to allow other screens to
	   handle the action. If the screen is active, it handles the WIDGET_ACTION_GO_BACK action by navigating back to the root menu, and WIDGET_ACTION_COMMAND actions by routing them to the appropriate
	   sub-menu based on the command parameter. The function also manages focus and view indices for the options list when a command is processed. It returns true if the action was handled, and false
	   otherwise.

		\param action The widget action to handle
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

struct creditInfo_t {
	//! Initializes a creditInfo_t object with default values.
	creditInfo_t()
	{
		type  = -1;
		entry = "";
	}

	//! Constructs a creditInfo_t object with the specified type and entry value.
	creditInfo_t( int t, const char* val )
	{
		type  = t;
		entry = val;
	}

	int	  type;
	idStr entry;
};

/*!
	\class idMenuScreen_Shell_Credits
	\brief Handles the display and navigation of game credits within a menu system.

	This class manages the credits screen UI, including initialization of UI elements, display logic, and user interaction handling. It integrates with the menu system to provide navigation between
   the credits screen and other menu states. The class supports transitioning between different game states based on whether the game has been completed, controlling whether a player can disconnect
   from a session. It manages the credit list display and updates the information shown to the user during gameplay.

*/
class idMenuScreen_Shell_Credits : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_Credits class with default values for the back button pointer and credit index.
	idMenuScreen_Shell_Credits() :
		btnBack( NULL ),
		creditIndex( 0 )
	{
	}

	//! Initializes the credits screen UI elements and sets up the back button functionality.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the credits screen UI elements and command bar buttons based on game completion status
	virtual void Update();

	//! Initializes and displays the credits screen, playing music if the game is complete.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the credits screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the credits screen, managing navigation and game state transitions.

		This function processes widget actions specific to the credits screen. When the user triggers a 'go back' action, it checks if the game is complete. If so, it disconnects the player;
	   otherwise, it returns to the root screen. The function returns true if the action was handled, and false otherwise.

		\param action The widget action that was triggered
		\param event The event associated with the widget action
		\param widget The widget that triggered the action
		\param forceHandled Optional parameter to force the action to be marked as handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Initializes the credit list for the shell credits screen.
	void		 SetupCreditList();

	//! Updates the credit display information for the shell credits screen.
	void		 UpdateCredits();

private:
	idMenuWidget_Button* btnBack;
	idList<creditInfo_t> creditList;
	int					 creditIndex;
};

/*!
	\class idMenuScreen_Shell_Resolution
	\brief Manages the display and interaction with video resolution settings in a menu system.
*/
class idMenuScreen_Shell_Resolution : public idMenuScreen
{
public:
	//! Initializes a new instance of the resolution menu screen.
	idMenuScreen_Shell_Resolution() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the resolution selection screen by setting up UI elements and widget bindings.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the resolution menu screen by configuring command bar buttons and binding UI elements.
	virtual void Update();

	//! Displays the resolution selection screen with available video modes and sets the current selection.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the resolution menu screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the resolution settings menu screen

		Processes widget actions such as navigating back or selecting a resolution option. When a resolution is selected, it applies the changes and may prompt the user to confirm the video settings
	   change. The function manages transitions between different menu screens and handles the vid_restart command to apply new video settings

		\param action The widget action that was performed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force the action to be handled regardless of normal processing
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	struct optionData_t {
		//! Initializes a new instance of the optionData_t struct with default values.
		optionData_t()
		{
			fullscreen = -1;
			vidmode	   = -1;
		}

		//! Constructs an optionData_t object with specified fullscreen and video mode values.
		optionData_t( int f, int v )
		{
			fullscreen = f;
			vidmode	   = v;
		}

		//! Constructs a new optionData_t object as a copy of another optionData_t object.
		optionData_t( const optionData_t& other )
		{
			fullscreen = other.fullscreen;
			vidmode	   = other.vidmode;
		}

		//! Assigns the contents of another optionData_t instance to this instance and returns a reference to this instance.
		const optionData_t& operator=( const optionData_t& other )
		{
			fullscreen = other.fullscreen;
			vidmode	   = other.vidmode;
			return *this;
		}

		//! Compares this resolution option with another for equality based on fullscreen mode and video mode
		bool operator==( const optionData_t& other ) const { return ( fullscreen == other.fullscreen ) && ( ( vidmode == other.vidmode ) || ( fullscreen <= 0 ) ); }
		int	 fullscreen;
		int	 vidmode;
	};
	idList<optionData_t>	  optionData;

	optionData_t			  originalOption;

	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

/*!
	\class idMenuScreen_Shell_Difficulty
	\brief Manages the difficulty selection menu screen for the game.
*/
class idMenuScreen_Shell_Difficulty : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_Difficulty class.
	idMenuScreen_Shell_Difficulty() :
		nightmareUnlocked( false ),
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the difficulty selection menu screen with options for Easy, Medium, Hard, and Nightmare difficulty levels.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the difficulty selection screen by configuring command bar buttons and binding UI elements.
	virtual void Update();

	//! Displays the difficulty selection screen with appropriate skill level options based on game type and unlock status.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the difficulty selection screen with the specified transition effect.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the difficulty selection menu screen.

		This function processes widget actions such as scrolling through difficulty options, navigating back to the previous menu, and selecting a difficulty level. It ensures proper handling of the
	   nightmare difficulty option which is unlocked conditionally. The function also updates the game skill level and triggers appropriate intro sequences based on the selected game type.

		\param action The widget action to be processed
		\param event The widget event associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Flag indicating whether the action should be forced as handled
		\return Boolean value indicating whether the action was handled successfully
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	bool					  nightmareUnlocked;
	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

/*!
	\class idMenuScreen_Shell_Playstation
	\brief Provides the PlayStation-specific shell menu screen implementation.
*/
class idMenuScreen_Shell_Playstation : public idMenuScreen
{
public:
	//! Constructs a new instance of the PlayStation shell menu screen.
	idMenuScreen_Shell_Playstation() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the PlayStation shell menu screen with the provided menu handler data
	virtual void Initialize( idMenuHandler* data );

	//! Updates the PlayStation-specific menu screen by configuring command bar buttons and binding sprites.
	virtual void Update();

	//! Shows the PlayStation shell menu screen with the specified transition type.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the PlayStation shell screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the PlayStation shell menu screen

		This function processes user interactions within the PlayStation-specific menu screen. It first validates that menu data is available and that the current screen is the PlayStation area. It
	   then handles different action types such as navigating back to the root screen or processing focus selections. The function manages selection indices and updates the menu view accordingly. If
	   the action type is not handled specifically, it delegates to the base class implementation.

		\param action The widget action that occurred
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag indicating if the action should be forced as handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

/*!
	\class idMenuScreen_Shell_ModeSelect
	\brief A menu screen implementation for selecting game modes in a shell interface.
*/
class idMenuScreen_Shell_ModeSelect : public idMenuScreen
{
public:
	//! Initializes a new instance of the mode selection menu screen.
	idMenuScreen_Shell_ModeSelect() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the mode selection menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the mode selection screen display and command bar buttons.
	virtual void Update();

	//! Displays the mode selection screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the mode selection screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for selecting a game mode in the shell interface.

		This function processes widget actions for the mode selection screen. It handles the back action to return to the party lobby and the press focused action to select a game mode. When a mode is
	   selected, it updates the match parameters with the chosen game mode and creates a new match. The function ensures the correct screen transition and updates the party lobby settings accordingly.

		\param action The widget action that was performed
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

/*!
	\class idMenuScreen_Shell_GameBrowser
	\brief Provides a user interface for browsing and connecting to game servers.

	This class implements a menu screen for displaying and managing game server listings. It handles user interactions such as navigating back to previous screens, refreshing server information, and
   connecting to selected servers. The screen supports showing loading indicators during server list updates and displays server information in a list format. The implementation manages the transition
   between different visual states of the browser UI and coordinates with a menu handler for consistent user experience.

*/
class idMenuScreen_Shell_GameBrowser : public idMenuScreen
{
public:
	//! Constructs a new instance of the game browser menu screen.
	idMenuScreen_Shell_GameBrowser() :
		listWidget( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the game browser menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Displays the game browser screen with the specified transition type.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the game browser screen and cancels server listings
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the game browser menu screen

		Processes different widget actions such as navigating back, refreshing server lists, showing gamer tags, and connecting to servers. The function checks if the current screen is the browser
	   screen and validates the menu handler before processing actions. It handles going back to the party lobby, refreshing server information, displaying gamer card UI, and connecting to selected
	   servers.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandle Flag to force handling of the action
		\return True if the action was handled successfully, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandle = false );

	//! Updates the server list display by hiding all children and showing a loading indicator before fetching server data.
	void		 UpdateServerList();

	//! Handles the completion of server list retrieval by updating the game browser UI.
	void		 OnServerListReady();

	//! Populates game server information into a list widget for display
	void		 DescribeServer( const serverInfo_t& server, const int index );

private:
	idMenuWidget_GameBrowserList* listWidget;
	idMenuWidget_Button*		  btnBack;
};

/*!
	\class idMenuScreen_Shell_Leaderboards
	\brief Manages the display and interaction of leaderboard data within a menu system.

	This class implements a menu screen for showing leaderboard information, handling user interactions, and managing the display of leaderboard data. It inherits from idMenuScreen and provides
   functionality to initialize the screen, update UI elements, handle user actions like navigation and selection, and manage leaderboard data caching and refreshing. The class supports displaying
   messages and handling online status checks when updating leaderboard information. It is designed to work within a larger menu system and provides methods for transitioning between states and
   managing the leaderboard data flow.

*/
class idMenuScreen_Shell_Leaderboards : public idMenuScreen
{
public:
	//! Initializes a new instance of the leaderboards menu screen with default values for all member variables.
	idMenuScreen_Shell_Leaderboards() :
		options( NULL ),
		btnBack( NULL ),
		btnPrev( NULL ),
		btnNext( NULL ),
		btnPageDwn( NULL ),
		btnPageUp( NULL ),
		lbCache( NULL ),
		lbHeading( NULL ),
		lbIndex( 0 ),
		refreshLeaderboard( false ),
		refreshWhenMasterIsOnline( false )
	{
	}

	//! Destructor for the leaderboards menu screen that cleans up the leaderboard cache.
	virtual ~idMenuScreen_Shell_Leaderboards();

	//! Initializes the leaderboards screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the leaderboard screen UI elements and command bar buttons based on current state and platform.
	virtual void Update();

	//! Displays the leaderboards screen with the specified transition type.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the leaderboards screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles UI actions for the leaderboards screen, managing navigation, scrolling, and selection interactions.

		This function processes various widget actions for the leaderboards screen, including navigation back to the party lobby, cycling filters, selecting entries, and handling scrolling. It updates
	   the leaderboard cache and triggers refreshes as needed. The function returns true if the action was handled, false otherwise.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Indicates if the action should be forced to be handled
		\return True if the action was handled by this function, false otherwise.
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Updates the leaderboard with the provided callback data and handles online status checks.
	void		 UpdateLeaderboard( const idLeaderboardCallback* callback );

	//! Advances the leaderboard cache processing.
	void		 PumpLBCache();

	//! Refreshes the leaderboard display with updated data from the leaderboard cache.
	void		 RefreshLeaderboard();

	//! Displays or hides a message with an optional spinner in the leaderboards menu.
	void		 ShowMessage( bool show, idStr message, bool spinner );

	//! Clears the leaderboard cache data.
	void		 ClearLeaderboard();

	//! Sets the current leaderboard index, wrapping around boundaries if necessary.
	void		 SetLeaderboardIndex();

protected:
	struct doomLeaderboard_t {
		//! Initializes a new instance of the doomLeaderboard_t struct with a null leaderboard pointer.
		doomLeaderboard_t() :
			lb( NULL )
		{
		}

		//! Constructs a doomLeaderboard_t object with the provided leaderboard definition and name.
		doomLeaderboard_t( const leaderboardDefinition_t* _lb, idStr _name )
		{
			lb	 = _lb;
			name = _name;
		}
		const leaderboardDefinition_t* lb;
		idStr						   name;
	};

	idList<doomLeaderboard_t> leaderboards;

	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
	idMenuWidget_Button*	  btnPrev;
	idMenuWidget_Button*	  btnNext;
	idMenuWidget_Button*	  btnPageDwn;
	idMenuWidget_Button*	  btnPageUp;
	idLBCache*				  lbCache;
	idSWFTextInstance*		  lbHeading;
	int						  lbIndex;
	bool					  refreshLeaderboard;
	bool					  refreshWhenMasterIsOnline;
};

/*!
	\class idMenuScreen_Shell_Bindings
	\brief Manages the user interface for configuring control bindings including keyboard, controller, and mouse inputs.

	This class implements a menu screen for users to customize their control bindings. It provides functionality for displaying binding options, handling user interactions such as selecting bindings
   or restoring defaults, and updating the visual representation of current bindings. The class manages the transition between different UI states including when a user is in the process of assigning
   a new key binding. It integrates with a menu handler to set up UI elements and process widget actions. The screen supports displaying different types of bindings and handles proper validation and
   updating of binding states.

*/
class idMenuScreen_Shell_Bindings : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_Bindings class.
	idMenuScreen_Shell_Bindings() :
		options( NULL ),
		restoreDefault( NULL ),
		blinder( NULL ),
		txtBlinder( NULL ),
		btnBack( NULL ),
		bindingsChanged( false )
	{
	}

	//! Initializes the bindings menu screen with the provided menu handler and sets up UI elements.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the bindings menu screen by configuring command bar buttons and binding sprites.
	virtual void Update();

	//! Displays the bindings menu screen with specified transition type.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the bindings screen and updates modified flags if changes were made
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the keyboard bindings menu screen

		This function processes widget actions for the keyboard bindings menu, including navigation, restoring defaults, and binding key assignments. It checks if the current screen is the keyboard
	   bindings screen and handles various action types such as going back, restoring defaults, pressing focused items, and scrolling through the bindings list. When a binding is selected, it either
	   focuses on the binding or enters binding mode to wait for a new key press. The function also handles scrolling through the list of bindings with proper index validation.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Sets the key binding for the specified key number
	void		 SetBinding( int keyNum );

	//! Updates the binding display for keyboard, controller, and mouse inputs
	void		 UpdateBindingDisplay();

	//! Toggles the visibility of UI elements based on the wait state.
	void		 ToggleWait( bool wait );

	//! Sets the binding changed flag to the specified value.
	void		 SetBindingChanged( bool changed ) { bindingsChanged = changed; }

protected:
	//! Displays a dialog to confirm restoring default control bindings.
	void					  HandleRestoreDefaults();

	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  restoreDefault;
	idSWFSpriteInstance*	  blinder;
	idSWFSpriteInstance*	  txtBlinder;
	idMenuWidget_Button*	  btnBack;
	bool					  bindingsChanged;
};

/*!
	\class idMenuScreen_Shell_Dev
	\brief Manages the developer menu screen interface and interactions.

	This class implements the developer menu screen functionality, handling the display and user interactions within the development environment. It inherits from the base menu screen class and
   provides specific implementations for initializing, updating, showing, and hiding the developer menu. The class manages the development options list and processes user actions such as navigation
   and map selection. When a map is selected, it executes the corresponding devmap command. The screen supports different transition types for showing and hiding, and validates that actions are
   processed within the correct menu context.

*/
class idMenuScreen_Shell_Dev : public idMenuScreen
{
public:
	struct devOption_t {
		devOption_t()
		{
			map	 = "";
			name = "";
		};

		//! Constructs a devOption_t object with the specified map and name strings.
		devOption_t( const char* m, const char* n )
		{
			map	 = m;
			name = n;
		}

		const char* map;
		const char* name;
	};

	//! Constructs a new instance of the development shell menu screen.
	idMenuScreen_Shell_Dev() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the development menu screen with the provided menu handler data
	virtual void Initialize( idMenuHandler* data );

	//! Updates the development menu screen by configuring command bar buttons and binding UI elements.
	virtual void Update();

	//! Shows the developer menu screen with the specified transition type.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the developer menu screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles widget actions for the development menu screen, processing navigation and map selection events.

		This function processes user interactions within the development menu screen. It handles back navigation and map selection actions. When the user selects a map from the development options, it
	   executes the corresponding devmap command. The function returns true if the action was handled, false otherwise. It validates that the current screen is the development area before processing
	   actions.

		\param action The widget action being performed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled If true, forces the action to be marked as handled regardless of processing outcome
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Initializes the development options list with various game map names and labels.
	void		 SetupDevOptions();

private:
	idMenuWidget_DynamicList*				 options;
	idMenuWidget_Button*					 btnBack;
	idList<devOption_t, TAG_IDLIB_LIST_MENU> devOptions;
};

/*!
	\class idMenuScreen_Shell_NewGame
	\brief Manages the new game selection screen in the shell menu.
*/
class idMenuScreen_Shell_NewGame : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_NewGame class.
	idMenuScreen_Shell_NewGame() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the new game menu screen with options for different game modes and a back button.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the new game menu screen by configuring command bar buttons and binding sprites.
	virtual void Update();

	//! Displays the new game menu screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the new game screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the new game screen in the shell menu, managing navigation and game type selection.

		This function processes widget actions for the new game screen, determining whether to navigate back to the campaign screen or proceed to the difficulty selection screen based on user input.
	   It also handles setting the new game type and updating the menu state accordingly. The function checks if the current screen is active before processing actions, and falls back to the base
	   class implementation for unhandled actions.

		\param action The widget action that was triggered
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force the action to be handled regardless of normal processing
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList* options;
	idMenuWidget_Button*	  btnBack;
};

/*!
	\class idMenuScreen_Shell_Load
	\brief Manages the load game screen interface and functionality.

	Provides the user interface and control logic for browsing, loading, and deleting saved game files. The class handles navigation through save game options, processes user actions for game loading
   and deletion, and manages the display of save game information including sorting by date. It integrates with a menu handler to coordinate screen transitions and maintains references to UI elements
   for interaction. The implementation supports displaying dialogs for confirming game deletion and handling corrupted save files.

*/
class idMenuScreen_Shell_Load : public idMenuScreen
{
public:
	//! Initializes a new instance of the load menu screen with default null values for options, save information, and button references.
	idMenuScreen_Shell_Load() :
		options( NULL ),
		saveInfo( NULL ),
		btnBack( NULL ),
		btnDelete( NULL )
	{
	}

	//! Initializes the load menu screen with the provided menu handler data.
	virtual void		  Initialize( idMenuHandler* data );

	//! Updates the load game screen elements and bindings.
	virtual void		  Update();

	//! Displays the load screen with the specified transition type.
	virtual void		  ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the load screen with the specified transition type.
	virtual void		  HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the load game screen, including navigation and game loading.

		This function processes various widget actions for the load game screen, such as pressing joystick buttons, navigating options, and loading or deleting saved games. It checks if the current
	   screen is the load screen and returns false if not. The function handles actions like pressing joystick buttons to exit, deleting saved games, going back to previous screens, and loading games.
	   It also manages the selection index and updates the view when necessary. The function returns true for handled actions or calls the parent class's HandleAction method for unhandled actions.

		\param action The widget action that was triggered
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag to force the action to be handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool		  HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Updates the save game enumerations and sorts them by date.
	void				  UpdateSaveEnumerations();

	//! Handles loading a damaged game save by displaying a dialog to delete or cancel
	void				  LoadDamagedGame( int index );

	//! Loads a game save at the specified index.
	void				  LoadGame( int index );

	//! Deletes a saved game at the specified index after confirming with the user.
	void				  DeleteGame( int index );

	//! Returns the list of sorted save game details.
	saveGameDetailsList_t GetSortedSaves() const { return sortedSaves; }

private:
	idMenuWidget_DynamicList*	 options;
	idMenuWidget_Shell_SaveInfo* saveInfo;
	idMenuWidget_Button*		 btnBack;
	idMenuWidget_Button*		 btnDelete;
	saveGameDetailsList_t		 sortedSaves;
};

/*!
	\class idMenuScreen_Shell_Save
	\brief Manages the save game screen interface and functionality.

	This class handles the user interface and interaction logic for saving game progress. It inherits from idMenuScreen and provides specific implementations for initializing, updating, showing, and
   hiding the save menu screen. The class manages save game details, sorts them, and provides functionality for saving, deleting, and updating save enumerations. It also processes widget actions by
   delegating to parent or menu data handlers. The save game operations include creating new saves or overwriting existing ones, with appropriate UI handling for save slots.

*/
class idMenuScreen_Shell_Save : public idMenuScreen
{
public:
	//! Initializes a new instance of the idMenuScreen_Shell_Save class.
	idMenuScreen_Shell_Save() :
		btnBack( NULL ),
		options( NULL ),
		saveInfo( NULL ),
		btnDelete( NULL )
	{
	}

	//! Initializes the save menu screen with the provided menu handler data.
	virtual void		  Initialize( idMenuHandler* data );

	//! Updates the save game screen UI elements and bindings
	virtual void		  Update();

	//! Displays the save screen with the specified transition effect.
	virtual void		  ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the save screen with the specified transition type.
	virtual void		  HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles widget actions for the save screen, processing user input and triggering save or delete operations.

		This function processes various widget actions such as joystick inputs, back navigation, and selection events. It handles saving games, deleting saved games, and navigating back to the root
	   menu. The function checks for valid menu data and active screen state before processing actions. It returns true for actions that are handled by this function, or delegates to the parent class
	   for unhandled actions.

		\param action Reference to the widget action to be handled
		\param event Reference to the widget event associated with the action
		\param widget Pointer to the widget that triggered the action
		\param forceHandled Boolean flag indicating whether to force handling regardless of parent
		\return Boolean value indicating whether the action was handled by this function
	*/
	virtual bool		  HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Returns the list of sorted save game details.
	saveGameDetailsList_t GetSortedSaves() const { return sortedSaves; }

	//! Updates the save game enumerations and handles UI display logic for save slots.
	void				  UpdateSaveEnumerations();

	//! Saves the game to a specified slot, creating a new save or overwriting an existing one.
	void				  SaveGame( int index );

	//! Deletes a save game at the specified index after confirming with the user
	void				  DeleteGame( int index );

private:
	idMenuWidget_Button*		 btnBack;
	idMenuWidget_DynamicList*	 options;
	idMenuWidget_Shell_SaveInfo* saveInfo;
	idMenuWidget_Button*		 btnDelete;
	saveGameDetailsList_t		 sortedSaves;
};

/*!
	\class idMenuScreen_Shell_GameOptions
	\brief Provides a user interface for configuring game options within a shell menu system.
*/
class idMenuScreen_Shell_GameOptions : public idMenuScreen
{
public:
	/*!
		\class idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings
		\brief Data source for managing game settings in a menu system.
	*/
	class idMenuDataSource_GameSettings : public idMenuDataSource
	{
	public:
		enum gameSettingFields_t {
			GAME_FIELD_FOV,
			GAME_FIELD_CHECKPOINTS,
			GAME_FIELD_AUTO_SWITCH,
			GAME_FIELD_AUTO_RELOAD,
			GAME_FIELD_AIM_ASSIST,
			GAME_FIELD_ALWAYS_SPRINT,
			GAME_FIELD_CLASSIC_FLASHLIGHT,
			GAME_FIELD_MUZZLE_FLASHES,
			MAX_GAME_FIELDS
		};

		//! Constructs a new instance of the game settings data source.
		idMenuDataSource_GameSettings();

		//! Loads game settings data into the data source fields.
		virtual void		   LoadData();

		//! Commits the game settings data to the corresponding console variables.
		virtual void		   CommitData();

		//! Indicates whether any game settings data has changed compared to the original values.
		virtual bool		   IsDataChanged() const;

		//! Retrieves a specific field value by its index for reading or updating.
		virtual idSWFScriptVar GetField( const int fieldIndex ) const { return fields[fieldIndex]; }

		//! Adjusts a game settings field value based on the field index and adjustment amount.
		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

	private:
		idStaticList<idSWFScriptVar, MAX_GAME_FIELDS> fields;
		idStaticList<idSWFScriptVar, MAX_GAME_FIELDS> originalFields;
	};

	//! Initializes a new instance of the game options menu screen.
	idMenuScreen_Shell_GameOptions() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the game options menu screen with the provided data handler.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the game options screen UI elements and command bar buttons.
	virtual void Update();

	//! Initializes system data and displays the game options screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the game options screen and commits any changed system data before hiding.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the game options menu screen, processing navigation and selection events.

		This function processes widget actions for the game options screen, including returning to the previous menu, handling selection changes, and managing repeater events. It validates the current
	   screen context and updates the menu state accordingly. The function returns true to indicate that the action was handled, and false if the action should be passed to the parent class for
	   further processing.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList*	  options;
	idMenuDataSource_GameSettings systemData;
	idMenuWidget_Button*		  btnBack;
};

/*!
	\class idMenuScreen_Shell_MatchSettings
	\brief Provides a user interface for configuring match settings within a game lobby environment.
*/
class idMenuScreen_Shell_MatchSettings : public idMenuScreen
{
public:
	/*!
		\class idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings
		\brief Data source for match settings management with load, commit, and adjustment capabilities.

		This class serves as a data source for match settings, providing functionality to load settings data, commit changes, and adjust field values. It maintains current and backup states to track
	   modifications and supports retrieving field values, game mode names, and map names. The class enables wrapping behavior when adjusting fields and provides mechanisms to check for data changes,
	   including map changes. The interface allows for retrieval of specific field values based on index and supports adjusting numeric fields while maintaining valid ranges.

	*/
	class idMenuDataSource_MatchSettings : public idMenuDataSource
	{
	public:
		enum matchSettingFields_t { MATCH_FIELD_MODE, MATCH_FIELD_MAP, MATCH_FIELD_TIME, MATCH_FIELD_SCORE, MAX_MATCH_FIELDS };

		//! Initializes the match settings data source with default values.
		idMenuDataSource_MatchSettings();

		//! Loads match settings data into the data source
		virtual void		   LoadData();

		//! Commits the current data by marking modified flags and copying fields to backup fields.
		virtual void		   CommitData();

		//! Indicates whether any match setting data has changed compared to the original values.
		virtual bool		   IsDataChanged() const;

		//! Retrieves a specific field value from the match settings data source based on the provided index.
		virtual idSWFScriptVar GetField( const int fieldIndex ) const { return fields[fieldIndex]; }

		//! Adjusts a match setting field by a specified amount with wrapping behavior
		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

		//! Returns whether the map has changed.
		bool				   MapChanged() { return updateMap; }

		//! Clears the map changed flag.
		void				   ClearMapChanged() { updateMap = false; }

	private:
		//! Retrieves the display name for a game mode based on the provided index and sets the output string.
		void										   GetModeName( int index, idStr& name );

		//! Retrieves the name of a map at the specified index and stores it in the provided string.
		void										   GetMapName( int index, idStr& name );

		idStaticList<idSWFScriptVar, MAX_MATCH_FIELDS> fields;
		idStaticList<idSWFScriptVar, MAX_MATCH_FIELDS> originalFields;
		bool										   updateMap;
	};

	//! Constructs a new instance of the match settings menu screen.
	idMenuScreen_Shell_MatchSettings() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the match settings menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the match settings screen interface and command bar buttons.
	virtual void Update();

	//! Prepares and displays the match settings screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the match settings screen and commits any changed data.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles widget actions for the match settings menu screen, processing user interactions and updating game match data accordingly.

		This function processes various widget actions such as adjusting fields, navigating back, and handling press events within the match settings menu. It checks if the current screen is active
	   and handles different action types accordingly. For field adjustment actions, it updates the corresponding data source and potentially refreshes related UI elements. For navigation actions, it
	   transitions to the game lobby screen. The function also manages repeater actions for focus index adjustments and delegates unhandled actions to the parent class.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force handling of the action even if it's already been handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList*	   options;
	idMenuDataSource_MatchSettings matchData;
	idMenuWidget_Button*		   btnBack;
};

/*!
	\class idMenuScreen_Shell_Controls
	\brief Manages the controls menu screen for user interface configuration.
*/
class idMenuScreen_Shell_Controls : public idMenuScreen
{
public:
	/*!
		\class idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings
		\brief Data source for managing control settings in a menu system.
	*/
	class idMenuDataSource_ControlSettings : public idMenuDataSource
	{
	public:
		enum controlSettingFields_t { CONTROLS_FIELD_INVERT_MOUSE, CONTROLS_FIELD_GAMEPAD_ENABLED, CONTROLS_FIELD_MOUSE_SENS, MAX_CONTROL_FIELDS };

		//! Constructs a new instance of the control settings data source.
		idMenuDataSource_ControlSettings();

		//! Loads control settings data into the data source
		virtual void		   LoadData();

		//! Commits the control settings data to the system.
		virtual void		   CommitData();

		//! Indicates whether any control settings data has changed.
		virtual bool		   IsDataChanged() const;

		//! Retrieves a specific field value from the control settings data source based on the provided index.
		virtual idSWFScriptVar GetField( const int fieldIndex ) const { return fields[fieldIndex]; }

		//! Adjusts a control settings field by the specified amount or toggles boolean fields.
		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

	private:
		idStaticList<idSWFScriptVar, MAX_CONTROL_FIELDS> fields;
		idStaticList<idSWFScriptVar, MAX_CONTROL_FIELDS> originalFields;
	};

	//! Initializes a new instance of the idMenuScreen_Shell_Controls class.
	idMenuScreen_Shell_Controls() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the controls menu screen with UI elements and event handlers.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the controls menu screen by configuring command bar buttons and binding UI elements.
	virtual void Update();

	//! Loads control data and displays the shell controls screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the controls screen and commits any changed control data.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the controls menu screen, managing navigation and control setting adjustments.

		This function processes widget actions within the controls menu screen. It handles returning to the previous menu, executing commands related to control settings such as bindings, gamepad
	   configuration, mouse inversion, mouse sensitivity, and gamepad enablement, and managing the repeater action for menu navigation. The function checks if the current screen is the controls screen
	   and returns false if not. It updates control data and menu options accordingly.

		\param action The widget action being handled
		\param event The widget event associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Flag indicating whether the action should be forced as handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList*		 options;
	idMenuDataSource_ControlSettings controlData;
	idMenuWidget_Button*			 btnBack;
};

/*!
	\class idMenuScreen_Shell_Gamepad
	\brief A menu screen class for managing gamepad control settings and UI interactions.
*/
class idMenuScreen_Shell_Gamepad : public idMenuScreen
{
public:
	/*!
		\class idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings
		\brief Data source for managing gamepad settings in a menu system.
	*/
	class idMenuDataSource_GamepadSettings : public idMenuDataSource
	{
	public:
		enum controlSettingFields_t {
			GAMEPAD_FIELD_LEFTY,
			GAMEPAD_FIELD_INVERT,
			GAMEPAD_FIELD_VIBRATE,
			GAMEPAD_FIELD_HOR_SENS,
			GAMEPAD_FIELD_VERT_SENS,
			GAMEPAD_FIELD_ACCELERATION,
			GAMEPAD_FIELD_THRESHOLD,
			MAX_GAMEPAD_FIELDS
		};

		//! Initializes a new instance of the GamepadSettings data source.
		idMenuDataSource_GamepadSettings();

		//! Loads gamepad settings data into the data source.
		virtual void		   LoadData();

		//! Commits the gamepad settings data to the system
		virtual void		   CommitData();

		//! Checks whether any gamepad settings have been modified compared to the original values.
		virtual bool		   IsDataChanged() const;

		//! Retrieves a specific field from the gamepad settings data source by its index.
		virtual idSWFScriptVar GetField( const int fieldIndex ) const { return fields[fieldIndex]; }

		//! Adjusts a gamepad setting field by either toggling a boolean value or clamping a float value within a range.
		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

	private:
		idStaticList<idSWFScriptVar, MAX_GAMEPAD_FIELDS> fields;
		idStaticList<idSWFScriptVar, MAX_GAMEPAD_FIELDS> originalFields;
	};

	//! Initializes a new instance of the idMenuScreen_Shell_Gamepad class.
	idMenuScreen_Shell_Gamepad() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the gamepad settings menu screen with control options and UI elements.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the gamepad control screen elements and UI bindings
	virtual void Update();

	//! Loads gamepad data and displays the gamepad menu screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the gamepad screen and commits any pending data changes.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles widget actions for the gamepad settings menu screen by processing specific commands and delegating other actions to the parent handler.

		This function processes widget actions for the gamepad settings screen, including navigation commands and configuration adjustments. It checks if the current screen is the gamepad settings
	   screen, then handles specific actions such as going back to the controls screen or adjusting gamepad settings. The function updates the gamepad data based on the selected parameter and
	   refreshes the options display. For actions not explicitly handled, it delegates to the parent class's HandleAction method.

		\param action Reference to the widget action to be handled
		\param event Reference to the widget event associated with the action
		\param widget Pointer to the widget that triggered the action
		\param forceHandled Boolean flag indicating whether to force handling regardless of parent
		\return Boolean value indicating whether the action was handled by this function or its parent/menu data
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList*		 options;
	idMenuDataSource_GamepadSettings gamepadData;
	idMenuWidget_Button*			 btnBack;
};

/*!
	\class idMenuScreen_Shell_ControllerLayout
	\brief Manages the controller layout configuration interface.

	Provides functionality for displaying and managing controller layout settings within a menu system. The class handles initialization of the layout screen, updates to binding information, and user
   interaction through action handling. It supports showing and hiding the screen with appropriate transitions, and ensures that layout data is properly committed when the screen is hidden. The
   implementation integrates with a menu handler to manage UI elements and maintains synchronization between the displayed binding information and the underlying controller configuration.

*/
class idMenuScreen_Shell_ControllerLayout : public idMenuScreen
{
public:
	/*!
		\class idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings
		\brief Data source for managing controller layout settings in a menu system.
	*/
	class idMenuDataSource_LayoutSettings : public idMenuDataSource
	{
	public:
		enum controlSettingFields_t {
			LAYOUT_FIELD_LAYOUT,
			MAX_LAYOUT_FIELDS,
		};

		//! Initializes the layout settings data source with default values.
		idMenuDataSource_LayoutSettings();

		//! Loads controller layout data from the player profile.
		virtual void		   LoadData();

		//! Submits data changes and updates the backup fields with the current values.
		virtual void		   CommitData();

		//! Returns true if the layout data has changed.
		virtual bool		   IsDataChanged() const;

		//! Returns the field at the specified index from the layout settings data source.
		virtual idSWFScriptVar GetField( const int fieldIndex ) const { return fields[fieldIndex]; }

		//! Adjusts a specific field in the controller layout settings by modifying its value based on the provided adjustment amount.
		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

	private:
		idStaticList<idSWFScriptVar, MAX_LAYOUT_FIELDS> fields;
		idStaticList<idSWFScriptVar, MAX_LAYOUT_FIELDS> originalFields;
	};

	//! Constructs a new controller layout menu screen.
	idMenuScreen_Shell_ControllerLayout() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the controller layout menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the controller layout screen by configuring command bar buttons and binding sprites.
	virtual void Update();

	//! Displays the controller layout screen with appropriate visual assets and updates binding information.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the controller layout screen and commits any pending layout data before transitioning.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the controller layout menu screen

		This function processes user input actions for the controller layout screen, including navigation and adjustments to controller bindings. It checks if the current screen is the controller
	   layout screen and handles different action types such as going back, selecting options, and adjusting field values. The function updates the menu state and binding information when appropriate.

		\param action The action performed by the user
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Updates the controller binding information displayed on the screen.
	void		 UpdateBindingInfo();

private:
	idMenuDataSource_LayoutSettings layoutData;
	idMenuWidget_DynamicList*		options;
	idMenuWidget_Button*			btnBack;
};

/*!
	\class idMenuScreen_Shell_SystemOptions
	\brief Manages the system options menu screen for configuring game settings.
*/
class idMenuScreen_Shell_SystemOptions : public idMenuScreen
{
public:
	/*!
		\class idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings
		\brief Data source for system settings in a menu interface.

		Provides access to system settings data for display and modification within a menu screen. Manages loading, committing, and tracking changes to system settings. Supports field-level access and
	   adjustment operations while indicating when a restart is required due to configuration changes.

	*/
	class idMenuDataSource_SystemSettings : public idMenuDataSource
	{
	public:
		enum systemSettingFields_t {
#ifdef _WIN32
			SYSTEM_FIELD_RENDERAPI, // RB: choose between DX12 and Vulkan on Windows
#endif
			SYSTEM_FIELD_FULLSCREEN,
			SYSTEM_FIELD_FRAMERATE,
			SYSTEM_FIELD_VSYNC,
			SYSTEM_FIELD_ANTIALIASING,
			// RB begin
			SYSTEM_FIELD_RENDERMODE,
			SYSTEM_FIELD_AMBIENT_BRIGHTNESS,
			SYSTEM_FIELD_SSAO,
			SYSTEM_FIELD_BLOOD_REFLECTIONS,
			SYSTEM_FIELD_FILMIC_POSTFX,
			SYSTEM_FIELD_CRT_POSTFX,
			// RB end
			SYSTEM_FIELD_BRIGHTNESS,
			SYSTEM_FIELD_VOLUME,
			MAX_SYSTEM_FIELDS
		};

		//! Constructs a new instance of the system settings data source.
		idMenuDataSource_SystemSettings();

		//! Initializes and loads the system settings data for the menu screen.
		virtual void		   LoadData();

		//! Submits the system settings data.
		virtual void		   CommitData();

		//! Determines whether any system settings have been modified compared to the original values.
		virtual bool		   IsDataChanged() const;

		//! Retrieves a specific system settings field value for display in the UI.
		virtual idSWFScriptVar GetField( const int fieldIndex ) const;

		//! Updates a specific system settings field value based on the provided adjustment amount.
		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

		//! Checks if a restart is required due to system setting changes
		bool				   IsRestartRequired() const;

	private:
		idStr			  originalRenderAPI;
		int				  originalFramerate;
		int				  originalAntialias;
		int				  originalVsync;
		float			  originalBrightness;
		float			  originalVolume;
		// RB begin
		// int originalShadowMapping; // TODO use for quality of shadowmaps?
		int				  originalRenderMode;
		float			  originalAmbientBrightness;
		int				  originalSSAO;
		int				  originalBloodReflections;
		int				  originalPostProcessing;
		int				  originalCRTPostFX;
		// RB end

		idList<vidMode_t> modeList;
	};

	//! Initializes a new instance of the system options menu screen.
	idMenuScreen_Shell_SystemOptions() :
		options( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the system options menu screen with the provided data handler
	virtual void Initialize( idMenuHandler* data );

	//! Updates the system options menu screen by configuring command bar buttons and binding UI elements.
	virtual void Update();

	//! Loads system data and displays the system options screen with the specified transition effect.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the system options screen and handles restart requirements and data changes.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions and events for the system options menu screen, managing navigation and field adjustments.

		This function processes widget actions such as going back to the previous screen, adjusting fields like fullscreen settings, and executing commands based on user input. It checks the current
	   active screen and ensures proper navigation between system options and resolution settings. The function also handles focus management for menu options and updates the system settings
	   accordingly.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Whether to force handling of the action even if it's already handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList*		options;
	idMenuDataSource_SystemSettings systemData;
	idMenuWidget_Button*			btnBack;
};

#if VR_OPTIONS

//*
//================================================
// idMenuScreen_Shell_Stereoscopics
//================================================
//*/
class idMenuScreen_Shell_Stereoscopics : public idMenuScreen
{
public:
	/*
	================================================
	idMenuDataSource_StereoSettings
	================================================
	*/
	class idMenuDataSource_StereoSettings : public idMenuDataSource
	{
	public:
		enum stereoSettingFields_t { STEREO_FIELD_ENABLE, STEREO_FIELD_SEPERATION, STEREO_FIELD_SWAP_EYES, MAX_STEREO_FIELDS };

		idMenuDataSource_StereoSettings();

		// loads data
		virtual void		   LoadData();

		// submits data
		virtual void		   CommitData();

		// says whether something changed with the data
		virtual bool		   IsDataChanged() const;

		// retrieves a particular field for reading or updating
		virtual idSWFScriptVar GetField( const int fieldIndex ) const;

		virtual void		   AdjustField( const int fieldIndex, const int adjustAmount );

		bool				   IsRestartRequired() const;

	private:
		idStaticList<idSWFScriptVar, MAX_STEREO_FIELDS> fields;
		idStaticList<idSWFScriptVar, MAX_STEREO_FIELDS> originalFields;
	};

	idMenuScreen_Shell_Stereoscopics() :
		options( NULL ),
		btnBack( NULL ),
		leftEyeMat( NULL ),
		rightEyeMat( NULL )
	{
	}
	virtual void Initialize( idMenuHandler* data );
	virtual void Update();
	virtual void ShowScreen( const mainMenuTransition_t transitionType );
	virtual void HideScreen( const mainMenuTransition_t transitionType );
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

private:
	idMenuWidget_DynamicList*		options;
	idMenuDataSource_StereoSettings stereoData;
	idMenuWidget_Button*			btnBack;
	const idMaterial*				leftEyeMat;
	const idMaterial*				rightEyeMat;
};

#endif

/*!
	\class idMenuScreen_Shell_PartyLobby
	\brief Manages the user interface and interactions for a party lobby screen within a multiplayer gaming environment.

	Provides the complete functionality for displaying and managing a party lobby user interface, including initialization, screen transitions, user input handling, and synchronization with session
   state. The class is responsible for updating the lobby display, managing user interactions such as kicking players or muting users, and coordinating with the game session to reflect current party
   status. It handles various UI elements and command bar configurations to support lobby operations like matchmaking, game creation, and exiting the lobby. The implementation ensures proper
   integration with the menu system and maintains consistency with the overall game flow and session management.

*/
class idMenuScreen_Shell_PartyLobby : public idMenuScreen
{
public:
	//! Constructs a new instance of the party lobby menu screen.
	idMenuScreen_Shell_PartyLobby() :
		isHost( false ),
		isPeer( false ),
		inParty( false ),
		options( NULL ),
		lobby( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the party lobby screen with the provided menu handler and sets up UI elements.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the party lobby screen by managing user list navigation, binding UI elements, and configuring command bar buttons.
	virtual void Update();

	//! Initializes and displays the party lobby screen
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the party lobby screen using the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions within the party lobby screen, processing commands like kicking players, leaving the lobby, muting players, and initiating game sessions.

		This function processes widget actions related to party lobby interactions. It handles various input events such as joystick presses, navigation commands, and user selections. The function
	   manages lobby operations including kicking players, muting users, starting matchmaking, creating or joining games, and handling lobby exit dialogs. It returns true when an action is
	   successfully handled, false otherwise.

		\param action The widget action that was triggered
		\param event The event associated with the action
		\param widget The widget that generated the action
		\param forceHandled Flag indicating whether the action should be treated as handled regardless of other conditions
		\return True if the action was successfully handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Updates the party lobby options based on the session state.
	void		 UpdateOptions();

	//! Updates the party lobby menu based on session state and lobby data.
	void		 UpdateLobby();

	//! Checks if the selected player can be kicked from the party lobby
	bool		 CanKickSelectedPlayer( lobbyUserID_t& luid );

	//! Displays the leaderboards screen if online play is available and valid menu data exists.
	void		 ShowLeaderboards();

private:
	bool															isHost;
	bool															isPeer;
	bool															inParty;
	idMenuWidget_DynamicList*										options;
	idMenuWidget_LobbyList*											lobby;
	idMenuWidget_Button*											btnBack;
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> menuOptions;
};

/*!
	\class idMenuScreen_Shell_GameLobby
	\brief Handles the user interface and interaction for game lobby sessions.

	Manages the display and functionality of the game lobby screen, including initialization, updating UI state, handling user actions, and integrating with session and lobby systems. The class
   supports operations like kicking players, muting users, starting games, and managing lobby settings. It provides methods to show and hide the lobby screen with transitions, and handles confirmation
   dialogs for critical actions such as leaving the lobby. The screen updates based on the current lobby state and session parameters, and ensures proper interaction with platform-specific features.

*/
class idMenuScreen_Shell_GameLobby : public idMenuScreen
{
public:
	//! Constructs a new instance of the game lobby menu screen.
	idMenuScreen_Shell_GameLobby() :
		longCountdown( 0 ),
		longCountRemaining( 0 ),
		shortCountdown( 0 ),
		isHost( false ),
		isPeer( false ),
		privateGameLobby( true ),
		options( NULL ),
		lobby( NULL ),
		btnBack( NULL )
	{
	}

	//! Initializes the game lobby menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the game lobby screen state and UI elements.
	virtual void Update();

	//! Initializes and displays the game lobby screen with appropriate options and settings based on the match parameters and lobby type.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the game lobby screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions within the game lobby screen, processing commands like kicking players, muting users, starting games, and managing lobby settings.

		This function processes widget actions specific to the game lobby interface. It handles various events such as kicking players, muting users, navigating back to previous screens, starting
	   games, and changing lobby privacy settings. It also manages dialogs for confirmation prompts when leaving the lobby and handles the display of gamer cards for lobby users. The function
	   integrates with the session and lobby systems to execute platform-specific operations like kicking users or updating match parameters.

		\param action The widget action to be processed
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Flag indicating whether the action should be marked as handled regardless of processing outcome
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Updates the game lobby screen based on session and lobby state.
	void		 UpdateLobby();

	//! Determines whether the selected player can be kicked from the lobby.
	bool		 CanKickSelectedPlayer( lobbyUserID_t& luid );

private:
	int																longCountdown;
	int																longCountRemaining;
	int																shortCountdown;

	bool															isHost;
	bool															isPeer;
	bool															privateGameLobby;

	idMenuWidget_DynamicList*										options;
	idMenuWidget_LobbyList*											lobby;
	idMenuWidget_Button*											btnBack;
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> menuOptions;
};

/*!
	\class idMenuScreen_HUD
	\brief The class manages the graphical user interface elements and visual feedback displayed during gameplay, including health, weapon, and communication indicators.

	The idMenuScreen_HUD class is responsible for rendering and updating all visual elements that constitute the game's heads-up display. It handles player status information such as health, armor,
   stamina, and weapon states, while also managing auxiliary UI components like tooltips, objective displays, pickup notifications, and multiplayer game information. The class integrates closely with
   player data and game state to ensure real-time updates of the HUD elements. It provides methods for showing and hiding various UI components with transition animations, managing communication
   indicators, displaying game messages and notifications, and handling special game events like respawning, objective completion, and multiplayer team scoring. The class maintains state for cursor
   visibility and text, manages video and PDA download displays, and supports various game-specific HUD elements like oxygen levels, soul cube status, and hit target indicators. All UI elements are
   updated based on player information and game conditions to provide immersive feedback during gameplay.

*/
class idMenuScreen_HUD : public idMenuScreen
{
public:
	//! Initializes all member variables to their default values for the HUD menu screen.
	idMenuScreen_HUD() :
		weaponInfo( NULL ),
		playerInfo( NULL ),
		stamina( NULL ),
		weaponName( NULL ),
		weaponPills( NULL ),
		downloadPda( NULL ),
		downloadVideo( NULL ),
		tipInfo( NULL ),
		mpChat( NULL ),
		mpWeapons( NULL ),
		healthBorder( NULL ),
		healthPulse( NULL ),
		armorFrame( NULL ),
		security( NULL ),
		newPDADownload( NULL ),
		newVideoDownload( NULL ),
		newPDA( NULL ),
		newVideo( NULL ),
		audioLog( NULL ),
		communication( NULL ),
		oxygen( NULL ),
		objective( NULL ),
		objectiveComplete( NULL ),
		ammoInfo( NULL ),
		weaponImg( NULL ),
		newWeapon( NULL ),
		pickupInfo( NULL ),
		talkCursor( NULL ),
		combatCursor( NULL ),
		grabberCursor( NULL ),
		bsInfo( NULL ),
		soulcubeInfo( NULL ),
		newItem( NULL ),
		respawnMessage( NULL ),
		flashlight( NULL ),
		mpChatObject( NULL ),
		mpConnection( NULL ),
		mpInfo( NULL ),
		mpHitInfo( NULL ),
		locationName( NULL ),
		securityText( NULL ),
		newPDAName( NULL ),
		newPDAHeading( NULL ),
		newVideoHeading( NULL ),
		mpMessage( NULL ),
		mpTime( NULL ),
		audioLogPrevTime( 0 ),
		commPrevTime( 0 ),
		oxygenComm( false ),
		inVaccuum( false ),
		objScreenshot( NULL ),
		cursorState( CURSOR_NONE ),
		cursorInCombat( 0 ),
		cursorTalking( 0 ),
		cursorItem( 0 ),
		cursorGrabber( 0 ),
		cursorNone( 0 ),
		showSoulCubeInfoOnLoad( false )
	{
	}

	//! Initializes the HUD menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the HUD screen state based on the local player's information.
	virtual void Update();

	//! Initializes and sets up all HUD UI elements when the screen is shown
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	//! Hides the HUD screen with the specified transition type.
	virtual void HideScreen( const mainMenuTransition_t transitionType );

	//! Updates the health and armor display values on the HUD based on the player's current stats.
	void		 UpdateHealthArmor( idPlayer* player );

	//! Updates the stamina display based on the player's current stamina level and game mode.
	void		 UpdateStamina( idPlayer* player );

	//! Updates the location text display based on the player's current position
	void		 UpdateLocation( idPlayer* player );

	//! Updates the weapon information displayed on the HUD for the specified player.
	void		 UpdateWeaponInfo( idPlayer* player );

	//! Updates the weapon state display for the player in the HUD
	void		 UpdateWeaponStates( idPlayer* player, bool weaponChanged );

	//! Displays a tooltip with the specified title and tip text on the HUD.
	void		 ShowTip( const char* title, const char* tip );

	//! Hides the tip display by animating its removal from the screen.
	void		 HideTip();

	//! Sets the video download screen visible and configures its heading text.
	void		 DownloadVideo();

	//! Displays PDA download information and updates security status if needed.
	void		 DownloadPDA( const idDeclPDA* pda, bool newSecurity );

	//! Updates the security display elements on the HUD.
	void		 UpdatedSecurity();

	//! Toggles the visibility and playback of the new video element based on the show flag.
	void		 ToggleNewVideo( bool show );

	//! Clears new PDA info by disabling video and PDA notifications and stopping associated frames.
	void		 ClearNewPDAInfo();

	//! Toggles the visibility of the new PDA screen based on the show parameter.
	void		 ToggleNewPDA( bool show );

	//! Updates the audio log visibility and animation based on the show flag.
	void		 UpdateAudioLog( bool show );

	//! Updates the communication HUD element based on whether communication should be shown and the player's vacuum state.
	void		 UpdateCommunication( bool show, idPlayer* player );

	//! Updates the oxygen display state and value based on the show flag and given value
	void		 UpdateOxygen( bool show, int val = 0 );

	//! Initializes the objective display with a title, description, and screenshot.
	void		 SetupObjective( const idStr& title, const idStr& desc, const idMaterial* screenshot );

	//! Sets the title for the objective completion screen.
	void		 SetupObjectiveComplete( const idStr& title );

	//! Displays either the objective or completion screen based on whether the objective is complete.
	void		 ShowObjective( bool complete );

	//! Hides the objective display with an optional complete animation
	void		 HideObjective( bool complete );

	//! Sets up and displays a weapon icon in the HUD for the specified player.
	void		 GiveWeapon( idPlayer* player, int weaponIndex );

	//! Updates the pickup information text for a given index with the specified name.
	void		 UpdatePickupInfo( int index, const idStr& name );

	//! Checks if the pickup list is ready for display.
	bool		 IsPickupListReady();

	//! Enables display of pickup information on the HUD.
	void		 ShowPickups();

	//! Sets the cursor state for the HUD based on the provided player, state, and set value.
	void		 SetCursorState( idPlayer* player, cursorState_t state, int set );

	//! Sets the cursor text for action and focus.
	void		 SetCursorText( const idStr& action, const idStr& focus );

	//! Updates the cursor state based on game conditions such as talking, combat, or item interaction.
	void		 UpdateCursorState();

	//! Plays a hit animation frame on the combat cursor when in combat state.
	void		 CombatCursorFlash();

	//! Updates the Soul Cube display based on the ready state.
	void		 UpdateSoulCube( bool ready );

	//! Shows or hides the respawn message UI element based on the show parameter.
	void		 ShowRespawnMessage( bool show );

	// MULTIPLAYER --------------------------------------

	//! Sets whether the Soul Cube information should be displayed when loading a save game.
	void		 SetShowSoulCubeOnLoad( bool show ) { showSoulCubeInfoOnLoad = show; }

	//! Shows or hides the multiplayer information display with optional team and flag visibility.
	void		 ToggleMPInfo( bool show, bool showTeams, bool isCTF = false );

	//! Sets the state of a flag for a specific team in the HUD.
	void		 SetFlagState( int team, int state );

	//! Sets the score for a specified team in the multiplayer HUD.
	void		 SetTeamScore( int team, int score );

	//! Sets the team for the multiplayer HUD screen.
	void		 SetTeam( int team );

	//! Shows or hides the hit target display with optional color coding.
	void		 TriggerHitTarget( bool show, const idStr& target, int color = 0 );

	//! Toggles the visibility of the lagged connection indicator based on the show parameter.
	void		 ToggleLagged( bool show );

	//! Updates the game time display with the provided time string.
	void		 UpdateGameTime( const char* time );

	//! Updates the HUD message display with the specified message text and visibility.
	void		 UpdateMessage( bool show, const idStr& message );

	//! Displays a new item notification on the HUD with the specified name and icon.
	void		 ShowNewItem( const char* name, const char* icon );

	//! Updates the flashlight HUD element based on the player's flashlight battery level.
	void		 UpdateFlashlight( idPlayer* player );

	//! Updates the chat HUD display based on the player's chatting state
	void		 UpdateChattingHud( idPlayer* player );

private:
	idSWFScriptObject*	 weaponInfo;
	idSWFScriptObject*	 playerInfo;
	idSWFScriptObject*	 stamina;
	idSWFScriptObject*	 weaponName;
	idSWFScriptObject*	 weaponPills;
	idSWFScriptObject*	 downloadPda;
	idSWFScriptObject*	 downloadVideo;
	idSWFScriptObject*	 tipInfo;
	idSWFScriptObject*	 mpChat;
	idSWFScriptObject*	 mpWeapons;

	idSWFSpriteInstance* healthBorder;
	idSWFSpriteInstance* healthPulse;
	idSWFSpriteInstance* armorFrame;
	idSWFSpriteInstance* security;
	idSWFSpriteInstance* newPDADownload;
	idSWFSpriteInstance* newVideoDownload;
	idSWFSpriteInstance* newPDA;
	idSWFSpriteInstance* newVideo;
	idSWFSpriteInstance* audioLog;
	idSWFSpriteInstance* communication;
	idSWFSpriteInstance* oxygen;
	idSWFSpriteInstance* objective;
	idSWFSpriteInstance* objectiveComplete;
	idSWFSpriteInstance* ammoInfo;
	idSWFSpriteInstance* weaponImg;
	idSWFSpriteInstance* newWeapon;
	idSWFSpriteInstance* pickupInfo;
	idSWFSpriteInstance* talkCursor;
	idSWFSpriteInstance* combatCursor;
	idSWFSpriteInstance* grabberCursor;
	idSWFSpriteInstance* bsInfo;
	idSWFSpriteInstance* soulcubeInfo;
	idSWFSpriteInstance* newItem;
	idSWFSpriteInstance* respawnMessage;
	idSWFSpriteInstance* flashlight;
	idSWFSpriteInstance* mpChatObject;
	idSWFSpriteInstance* mpConnection;

	idSWFSpriteInstance* mpInfo;
	idSWFSpriteInstance* mpHitInfo;

	idSWFTextInstance*	 locationName;
	idSWFTextInstance*	 securityText;
	idSWFTextInstance*	 newPDAName;
	idSWFTextInstance*	 newPDAHeading;
	idSWFTextInstance*	 newVideoHeading;

	idSWFTextInstance*	 mpMessage;
	idSWFTextInstance*	 mpTime;

	int					 audioLogPrevTime;
	int					 commPrevTime;

	bool				 oxygenComm;
	bool				 inVaccuum;

	idStr				 objTitle;
	idStr				 objDesc;
	const idMaterial*	 objScreenshot;
	idStr				 objCompleteTitle;

	cursorState_t		 cursorState;
	int					 cursorInCombat;
	int					 cursorTalking;
	int					 cursorItem;
	int					 cursorGrabber;
	int					 cursorNone;
	idStr				 cursorAction;
	idStr				 cursorFocus;

	bool				 showSoulCubeInfoOnLoad;
};

/*!
	\class idMenuScreen_Scoreboard
	\brief Manages and displays scoreboard information during gameplay.

	This class handles the presentation and interaction of scoreboard data within a game menu system. It initializes display elements, updates player information, manages team scores, and processes
   user actions such as navigation and player selection. The class integrates with a menu handler to synchronize display state and responds to various widget actions to provide an interactive
   scoreboard experience. It supports updating game information, spectating details, and maintaining the highlighted player selection based on team data and list positions.

*/
class idMenuScreen_Scoreboard : public idMenuScreen
{
public:
	//! Constructs a new scoreboard menu screen.
	idMenuScreen_Scoreboard() :
		playerList( NULL )
	{
	}

	//! Initializes the scoreboard menu screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the scoreboard screen display and command bar buttons.
	virtual void Update();

	//! Displays the scoreboard screen with localized text for game type and score information.
	virtual void ShowScreen( const mainMenuTransition_t transitionType );

	/*!
		\brief Handles user actions for the scoreboard menu screen, including navigation, player muting, and profile viewing.

		This function processes various widget actions within the scoreboard menu. It handles returning to the previous screen, muting players, selecting players to view profiles, and scrolling
	   through the player list. The function checks for valid menu data and handles different action types through a switch statement. It also manages the display logic for red and blue team players
	   based on their positions in the list.

		\param action The widget action being processed
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force the action as handled
		\return True if the action was handled successfully, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Sets the player data for the scoreboard screen.
	virtual void SetPlayerData( idList<scoreboardInfo_t, TAG_IDLIB_LIST_MENU> data );

	//! Updates the red and blue team scores in the scoreboard SWF interface.
	virtual void UpdateTeamScores( int r, int b );

	//! Updates the game information displayed on the scoreboard screen.
	virtual void UpdateGameInfo( idStr gameInfo );

	//! Updates the spectating information displayed on the scoreboard screen.
	virtual void UpdateSpectating( idStr spectating, idStr follow );

	//! Updates the highlight index in the scoreboard menu based on player list and team data.
	virtual void UpdateHighlight();

protected:
	idMenuWidget_ScoreboardList* playerList;
};

/*!
	\class idMenuScreen_Scoreboard_CTF
	\brief Provides a CTF-specific scoreboard screen implementation.
*/
class idMenuScreen_Scoreboard_CTF : public idMenuScreen_Scoreboard
{
public:
	//! Initializes the CTF scoreboard screen with the provided menu handler.
	virtual void Initialize( idMenuHandler* data );
};

/*!
	\class idMenuScreen_Scoreboard_Team
	\brief Manages the display and initialization of team-based scoreboard screens.
*/
class idMenuScreen_Scoreboard_Team : public idMenuScreen_Scoreboard
{
public:
	//! Initializes the scoreboard team screen with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );
};

//! Invites the master local user's party if in one, otherwise invites friends.
inline void InvitePartyOrFriends()
{
	const idLocalUser* const user = session->GetSignInManager().GetMasterLocalUser();
	if( user != NULL && user->IsInParty() && user->GetPartyCount() > 1 && !session->IsPlatformPartyInLobby() ) {
		session->InviteParty();
	} else {
		session->InviteFriends();
	}
}

#endif
