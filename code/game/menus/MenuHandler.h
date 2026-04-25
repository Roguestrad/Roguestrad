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
#ifndef __MENUDATA_H__
#define __MENUDATA_H__

enum shellAreas_t {
	SHELL_AREA_INVALID = -1,
	SHELL_AREA_START,
	SHELL_AREA_ROOT,
	SHELL_AREA_DEV,
	SHELL_AREA_CAMPAIGN,
	SHELL_AREA_LOAD,
	SHELL_AREA_SAVE,
	SHELL_AREA_NEW_GAME,
	SHELL_AREA_GAME_OPTIONS,
	SHELL_AREA_SYSTEM_OPTIONS,
	SHELL_AREA_MULTIPLAYER,
	SHELL_AREA_GAME_LOBBY,
#if VR_OPTIONS
	SHELL_AREA_STEREOSCOPICS,
#endif
	SHELL_AREA_PARTY_LOBBY,
	SHELL_AREA_SETTINGS,
	SHELL_AREA_AUDIO,
	SHELL_AREA_VIDEO,
	SHELL_AREA_KEYBOARD,
	SHELL_AREA_CONTROLS,
	SHELL_AREA_CONTROLLER_LAYOUT,
	SHELL_AREA_GAMEPAD,
	SHELL_AREA_PAUSE,
	SHELL_AREA_LEADERBOARDS,
	SHELL_AREA_PLAYSTATION,
	SHELL_AREA_DIFFICULTY,
	SHELL_AREA_RESOLUTION,
	SHELL_AREA_MATCH_SETTINGS,
	SHELL_AREA_MODE_SELECT,
	SHELL_AREA_BROWSER,
	SHELL_AREA_CREDITS,
	SHELL_NUM_AREAS
};

enum shellState_t {
	SHELL_STATE_INVALID = -1,
	SHELL_STATE_PRESS_START,
	SHELL_STATE_IDLE,
	SHELL_STATE_PARTY_LOBBY,
	SHELL_STATE_GAME_LOBBY,
	SHELL_STATE_PAUSED,
	SHELL_STATE_CONNECTING,
	SHELL_STATE_SEARCHING,
	SHELL_STATE_LOADING,
	SHELL_STATE_BUSY,
	SHELL_STATE_IN_GAME
};

enum pdaAreas_t { PDA_AREA_INVALID = -1, PDA_AREA_USER_DATA, PDA_AREA_USER_EMAIL, PDA_AREA_VIDEO_DISKS, PDA_AREA_INVENTORY, PDA_NUM_AREAS };

enum hudArea_t { HUD_AREA_INVALID = -1, HUD_AREA_PLAYING, HUD_NUM_AREAS };

enum scoreboardArea_t { SCOREBOARD_AREA_INVALID = -1, SCOREBOARD_AREA_DEFAULT, SCOREBOARD_AREA_TEAM, SCOREBOARD_AREA_CTF, SCOREBOARD_NUM_AREAS };

enum pdaHandlerWidgets_t { PDA_WIDGET_NAV_BAR, PDA_WIDGET_PDA_LIST, PDA_WIDGET_PDA_LIST_SCROLLBAR, PDA_WIDGET_CMD_BAR };

enum scoreboardHandlerWidgets_t {
	SCOREBOARD_WIDGET_CMD_BAR,
};

enum menuSounds_t {
	GUI_SOUND_MUSIC,
	GUI_SOUND_SCROLL,
	GUI_SOUND_ADVANCE,
	GUI_SOUND_BACK,
	GUI_SOUND_BUILD_ON,
	GUI_SOUND_BUILD_OFF,
	GUI_SOUND_FOCUS,
	GUI_SOUND_ROLL_OVER,
	GUI_SOUND_ROLL_OUT,
	NUM_GUI_SOUNDS,
};

static const int MAX_SCREEN_AREAS	   = 32;
static const int DEFAULT_REPEAT_TIME   = 150;
static const int WAIT_START_TIME_LONG  = 30000;
static const int WAIT_START_TIME_SHORT = 5000;

struct actionRepeater_t {
	//! Initializes a new instance of the actionRepeater_t class with default values.
	actionRepeater_t() :
		widget( NULL ),
		numRepetitions( 0 ),
		nextRepeatTime( 0 ),
		repeatDelay( DEFAULT_REPEAT_TIME ),
		screenIndex( -1 ),
		isActive( false )
	{
	}

	idMenuWidget*  widget;
	idWidgetEvent  event;
	idWidgetAction action;
	int			   numRepetitions;
	int			   nextRepeatTime;
	int			   repeatDelay;
	int			   screenIndex;
	bool		   isActive;
};

/*!
	\class mpScoreboardInfo
	\brief Provides scoreboard information for multiplayer game sessions.
*/
class mpScoreboardInfo
{
public:
	//! Initializes a new instance of mpScoreboardInfo with default values.
	mpScoreboardInfo() :
		voiceState( VOICECHAT_DISPLAY_NONE ),
		score( 0 ),
		wins( 0 ),
		ping( 0 ),
		team( -1 ),
		playerNum( 0 )
	{
	}

	//! Copies the contents of another mpScoreboardInfo instance.
	mpScoreboardInfo( const mpScoreboardInfo& src )
	{
		voiceState	 = src.voiceState;
		score		 = src.score;
		wins		 = src.wins;
		ping		 = src.ping;
		spectateData = src.spectateData;
		name		 = src.name;
		team		 = src.team;
		playerNum	 = src.playerNum;
	}

	//! Assigns the contents of another mpScoreboardInfo instance to this instance
	void operator=( const mpScoreboardInfo& src )
	{
		voiceState	 = src.voiceState;
		score		 = src.score;
		wins		 = src.wins;
		ping		 = src.ping;
		spectateData = src.spectateData;
		name		 = src.name;
		team		 = src.team;
		playerNum	 = src.playerNum;
	}

	//! Compares this mpScoreboardInfo object with another for inequality.
	bool operator!=( const mpScoreboardInfo& otherInfo ) const
	{
		if( otherInfo.score != score || otherInfo.wins != wins || otherInfo.ping != ping || otherInfo.spectateData != spectateData || otherInfo.name != name || otherInfo.team != team ||
			otherInfo.playerNum != playerNum || otherInfo.voiceState != voiceState ) {
			return true;
		}

		return false;
	}

	//! Compares this mpScoreboardInfo object with another for equality based on all member variables.
	bool operator==( const mpScoreboardInfo& otherInfo ) const
	{
		if( otherInfo.score != score || otherInfo.wins != wins || otherInfo.ping != ping || otherInfo.spectateData != spectateData || otherInfo.name != name || otherInfo.team != team ||
			otherInfo.playerNum != playerNum || otherInfo.voiceState != voiceState ) {
			return false;
		}

		return true;
	}

	voiceStateDisplay_t voiceState;
	int					score;
	int					wins;
	int					ping;
	int					team;
	int					playerNum;
	idStr				spectateData;
	idStr				name;
};

/*!
	\class idMenuHandler
	\brief Manages a collection of menu screens and handles GUI events for a user interface system.

	The idMenuHandler class serves as the central controller for a menu system, maintaining a collection of menu screens and coordinating their display and interaction. It handles GUI events, manages
   transitions between screens, and provides mechanisms for widget actions and sound playback. The class initializes with a SWF file and sound world, and can activate or deactivate the entire menu
   system. It supports adding child widgets, managing repeaters for repeated actions, and retrieving specific menu screens by index. The handler delegates action processing to child menu handlers when
   appropriate, and maintains state for active and next screens. It integrates with a SWF-based GUI system and provides access to a command bar widget for common interface elements.

*/
class idMenuHandler
{
public:
	//! Initializes a new instance of the idMenuHandler class.
	idMenuHandler();

	//! Destructor for the idMenuHandler class that cleans up resources.
	virtual ~idMenuHandler();

	//! Initializes the menu handler with a SWF file and sound world.
	virtual void		  Initialize( const char* swfFile, idSoundWorld* sw );

	//! Releases all menu handler resources and clears associated data structures
	virtual void		  Cleanup();

	//! Updates the menu handler state and renders the active GUI
	virtual void		  Update();

	//! Updates all child menu handlers in the collection.
	virtual void		  UpdateChildren();

	//! Updates the display of the specified menu screen and its children.
	virtual void		  UpdateMenuDisplay( int menu );

	//! Handles a GUI event by passing it to the active screen's event handler.
	virtual bool		  HandleGuiEvent( const sysEvent_t* sev );

	//! Checks if the menu handler's associated GUI is active.
	virtual bool		  IsActive();

	//! Activates or deactivates the menu system controlled by this handler.
	virtual void		  ActivateMenu( bool show );

	//! Triggers the menu handler to process menu events.
	virtual void		  TriggerMenu();

	/*!
		\brief Handles widget actions by processing different action types and delegating to child handlers when necessary.

		This function processes various widget actions based on their type. It handles adjusting fields, calling script functions, press events, and starting or stopping repeaters. For some actions,
	   it directly modifies widget data or triggers events. For others, it delegates the handling to child menu handlers if the current handler is not set to be the parent. The function returns true
	   if the action was handled, or false if it was not.

		\param action The widget action to be processed
		\param event The widget event associated with the action
		\param widget The widget that the action is associated with
		\param forceHandled Flag indicating whether the action should be forced to be handled
		\return True if the action was handled by this handler or a child handler, false otherwise.
	*/
	virtual bool		  HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Returns the index of the currently active screen.
	virtual int			  ActiveScreen() { return activeScreen; }

	//! Returns the index of the next screen in the menu handler.
	virtual int			  NextScreen() { return nextScreen; }

	//! Returns the current menu transition value.
	virtual int			  MenuTransition() { return transition; }

	//! Returns a pointer to the menu screen at the specified index, or null if the index is out of bounds.
	virtual idMenuScreen* GetMenuScreen( int index ) { return NULL; }

	//! Sets the next screen to display and the transition effect to use.
	virtual void		  SetNextScreen( int screen, int trans )
	{
		nextScreen = screen;
		transition = trans;
	}

	//! Starts a widget action repeater for the specified widget and action.
	virtual void			 StartWidgetActionRepeater( idMenuWidget* widget, const idWidgetAction& action, const idWidgetEvent& event );

	//! Processes repeated actions for widgets in the menu system.
	virtual void			 PumpWidgetActionRepeater();

	//! Clears the widget action repeater state.
	virtual void			 ClearWidgetActionRepeater();

	//! Returns the SWF GUI associated with this menu handler.
	virtual idSWF*			 GetGUI() { return gui; }

	//! Adds a child widget to the menu handler.
	virtual void			 AddChild( idMenuWidget* widget );

	//! Returns the menu widget child at the specified index from the menu handler's children list.
	virtual idMenuWidget*	 GetChildFromIndex( int index );

	//! Returns the platform identifier, with special handling for VR and joystick input.
	virtual int				 GetPlatform( bool realPlatform = false );

	//! Plays a menu sound of the specified type on the given channel.
	virtual void			 PlaySound( menuSounds_t type, int channel = -1 );

	//! Stops sound playback on the specified audio channel
	virtual void			 StopSound( int channel = SCHANNEL_ANY );

	//! Returns the command bar widget managed by this menu handler.
	idMenuWidget_CommandBar* GetCmdBar() { return cmdBar; }

protected:
	bool									   scrollingMenu;
	int										   scrollCounter;
	int										   activeScreen;
	int										   nextScreen;
	int										   transition;
	int										   platform;
	idSWF*									   gui;
	actionRepeater_t						   actionRepeater;
	idMenuScreen*							   menuScreens[MAX_SCREEN_AREAS];
	idList<idMenuWidget*, TAG_IDLIB_LIST_MENU> children;

	idStaticList<idStr, NUM_GUI_SOUNDS>		   sounds;

	idMenuWidget_CommandBar*				   cmdBar;
};

/*
================================================
lobbyPlayerInfo_t
================================================
*/
struct lobbyPlayerInfo_t {
	//! Constructs a default lobby player info object with default values for party token and voice state.
	lobbyPlayerInfo_t() :
		partyToken( 0 ),
		voiceState( VOICECHAT_DISPLAY_NONE )
	{
	}

	idStr				name;
	int					partyToken;
	voiceStateDisplay_t voiceState;
};

/*!
	\class idMenuHandler_Shell
	\brief Manages the shell menu system for interface navigation and game state handling.

	The idMenuHandler_Shell class serves as the primary controller for managing the shell menu system, handling user interactions, screen transitions, and game state synchronization. It inherits from
   idMenuHandler and provides specialized functionality for initializing, updating, and cleaning up menu resources. The class coordinates between various menu screens, processes widget actions, and
   manages platform-specific features like lobby lists, leaderboard updates, and intro videos. It maintains state information such as in-game status, time remaining, and new game type while providing
   methods to control UI elements like pacifiers, frame visibility, and logo display. The handler supports both single-player and multiplayer game modes through dedicated initialization and update
   functions, and handles platform-specific configurations like PC options setup.

*/
class idMenuHandler_Shell : public idMenuHandler
{
public:
	//! Initializes a new instance of the idMenuHandler_Shell class with default values for all member variables.
	idMenuHandler_Shell() :
		state( SHELL_STATE_INVALID ),
		nextState( SHELL_STATE_INVALID ),
		smallFrameShowing( false ),
		largeFrameShowing( false ),
		bgShowing( true ),
		waitForBinding( false ),
		waitBind( NULL ),
		menuBar( NULL ),
		pacifier( NULL ),
		timeRemaining( 0 ),
		nextPeerUpdateMs( 0 ),
		newGameType( 0 ),
		inGame( false ),
		showingIntro( false ),
		continueWaitForEnumerate( false ),
		gameComplete( false ),
		introGui( NULL ),
		typeSoundShader( NULL ),
		doom3Intro( NULL ),
		roeIntro( NULL ),
		lmIntro( NULL ),
		marsRotation( NULL )
	{
	}

	//! Updates the shell menu handler state and transitions between different screens.
	virtual void		  Update();

	//! Activates or deactivates the shell menu system based on the show parameter.
	virtual void		  ActivateMenu( bool show );

	//! Initializes the shell menu handler with the specified SWF file and sound world.
	virtual void		  Initialize( const char* swfFile, idSoundWorld* sw );

	//! Cleans up resources used by the shell menu handler.
	virtual void		  Cleanup();

	/*!
		\brief Handles menu actions for the shell interface, processing commands and managing screen transitions.

		This function processes widget actions within the shell menu system. It checks if an action should be handled based on the current active screen and forwards the action to the appropriate menu
	   screen if necessary. The function also manages various game commands such as starting campaigns, multiplayer sessions, settings access, and quitting the game. It handles screen transitions and
	   updates menu states accordingly. The function returns true if the action was handled, otherwise it delegates to the parent menu handler.

		\param action The widget action to be processed
		\param event The widget event associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Flag indicating whether to force handling of the action
		\return True if the action was handled by this function, false otherwise
	*/
	virtual bool		  HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Retrieves a menu screen instance at the specified index.
	virtual idMenuScreen* GetMenuScreen( int index );

	//! Handles GUI events for the shell menu system
	virtual bool		  HandleGuiEvent( const sysEvent_t* sev );

	//! Updates the saved game enumerations for the load or save menu screen based on the currently active screen.
	void				  UpdateSavedGames();

	//! Displays or hides the small frame UI element based on the show parameter.
	void				  ShowSmallFrame( bool show );

	//! Shows or hides the MP frame in the shell menu.
	void				  ShowMPFrame( bool show );

	//! Shows or hides the logo and background elements in the shell menu interface.
	void				  ShowLogo( bool show );

	//! Sets the shell state to the specified value.
	void				  SetShellState( shellState_t s ) { nextState = s; }

	//! Returns true if the small frame is currently showing.
	bool				  IsSmallFrameShowing() { return smallFrameShowing; }

	//! Updates the background state based on the current screen and frame visibility settings.
	void				  UpdateBGState();
	void				  GetMapName( int index, idStr& name );
	void				  GetModeName( int index, idStr& name );

	//! Returns the pacifier widget managed by this menu handler.
	idMenuWidget*		  GetPacifier() { return pacifier; }

	//! Returns the menu bar associated with the shell menu handler.
	idMenuWidget_MenuBar* GetMenuBar() { return menuBar; }

	//! Returns true if the pacifier sprite is visible, otherwise false.
	bool				  IsPacifierVisible() const { return ( pacifier != NULL && pacifier->GetSprite() != NULL ) ? pacifier->GetSprite()->IsVisible() : false; }

	//! Displays a pacifier with the specified message.
	void				  ShowPacifier( const idStr& msg );

	//! Hides the pacifier widget if it is currently visible.
	void				  HidePacifier();

	//! Sets the remaining time value.
	void				  SetTimeRemaining( int time ) { timeRemaining = time; }

	//! Returns the remaining time value.
	int					  GetTimeRemaining() { return timeRemaining; }

	//! Sets the new game type to the specified value.
	void				  SetNewGameType( int type ) { newGameType = type; }

	//! Returns the selected new game type.
	int					  GetNewGameType() { return newGameType; }

	//! Sets the in-game status flag for the shell menu handler.
	void				  SetInGame( bool val ) { inGame = val; }

	//! Returns the current in-game state of the shell menu handler.
	bool				  GetInGame() { return inGame; }

	//! Handles the exit game button click event by displaying a quit dialog.
	void				  HandleExitGameBtn();

	//! Sets up the PC options menu with appropriate buttons and commands based on the platform and demo mode.
	void				  SetupPCOptions();

	//! Sets the wait for binding flag and stores the binding string to wait for.
	void				  SetWaitForBinding( const char* bind )
	{
		waitForBinding = true;
		waitBind	   = bind;
	}

	//! Clears the flag indicating that the menu is waiting for a key binding.
	void ClearWaitForBinding() { waitForBinding = false; }

	//! Updates the leaderboard display with the provided callback data.
	void UpdateLeaderboard( const idLeaderboardCallback* callback );

	//! Updates the lobby list widget with player information from the active platform lobby.
	void UpdateLobby( idMenuWidget_LobbyList* lobbyList );

	//! Displays the Doom intro video and handles its associated text and sound playback.
	void ShowDoomIntro();

	//! Displays the ROE intro screen with animated text and cinematic material.
	void ShowROEIntro();

	//! Displays the LE intro video and handles text rendering and fading effects.
	void ShowLEIntro();

	//! Starts a game by executing a command to load a test map.
	void StartGame( int index );

	//! Sets whether the continue operation should wait for enumeration to complete.
	void SetContinueWaitForEnumerate( bool wait ) { continueWaitForEnumerate = wait; }

	//! Sets whether the continue option is valid in the singleplayer campaign screen.
	void SetCanContinue( bool valid );

	//! Sets the game completion status to true.
	void SetGameComplete() { gameComplete = true; }

	//! Returns the game completion status.
	bool GetGameComplete() { return gameComplete; }

private:
	shellState_t							 state;
	shellState_t							 nextState;
	bool									 smallFrameShowing;
	bool									 largeFrameShowing;
	bool									 bgShowing;
	bool									 waitForBinding;
	const char*								 waitBind;
	// idSysSignal				deviceRequestedSignal;

	idList<const char*, TAG_IDLIB_LIST_MENU> mpGameModes;
	idList<mpMap_t, TAG_IDLIB_LIST_MENU>	 mpGameMaps;
	idMenuWidget_MenuBar*					 menuBar;
	idMenuWidget*							 pacifier;
	int										 timeRemaining;
	int										 nextPeerUpdateMs;
	int										 newGameType;
	bool									 inGame;
	bool									 showingIntro;
	bool									 continueWaitForEnumerate;
	bool									 gameComplete;
	idSWF*									 introGui;
	const idSoundShader*					 typeSoundShader;
	const idMaterial*						 doom3Intro;
	const idMaterial*						 roeIntro;
	const idMaterial*						 lmIntro;
	const idMaterial*						 marsRotation;
	idList<idStr, TAG_IDLIB_LIST_MENU>		 navOptions;
};

/*!
	\class idMenuHandler_PDA
	\brief Manages the PDA menu system including audio/video playback and screen transitions.

	Handles the complete PDA menu interface with support for multiple screens and interactive elements. The class maintains audio and video playback states, manages screen transitions, and processes
   user interactions through widget actions. It initializes with a specified SWF file and sound world, and provides methods to activate the menu, trigger specific menu areas, and update playback
   states. The implementation supports automatic and forced action handling, navigation between PDA areas, and proper cleanup of UI resources when the menu is deactivated. The class delegates action
   processing to specific menu screens based on the current active screen, ensuring organized handling of user input across different PDA sections.

*/
class idMenuHandler_PDA : public idMenuHandler
{
public:
	//! Initializes a new instance of the idMenuHandler_PDA class with default values for audio and video playback states.
	idMenuHandler_PDA() :
		audioLogPlaying( false ),
		videoPlaying( false ),
		audioFile( NULL )
	{
	}

	//! Destructor for the idMenuHandler_PDA class that cleans up all associated UI elements.
	virtual ~idMenuHandler_PDA();

	//! Updates the PDA menu system and handles screen transitions and audio state changes.
	virtual void		  Update();

	//! Activates or deactivates the PDA menu, initializing its content when shown.
	virtual void		  ActivateMenu( bool show );

	//! Triggers the menu to transition to the user data area.
	virtual void		  TriggerMenu();

	//! Initializes the PDA menu handler with the specified SWF file and sound world.
	virtual void		  Initialize( const char* swfFile, idSoundWorld* sw );

	/*!
		\brief Handles user interface actions for the PDA menu system, processing commands and navigation events.

		This function processes widget actions for the PDA menu system, handling various user interactions such as navigation between different PDA areas, selecting items in lists, and managing
	   audio/video playback. It checks the current active screen and delegates action handling to the appropriate menu screen. The function also manages transitions between different PDA areas and
	   handles sound feedback for user interactions. It supports both automatic and forced handling of actions, with special handling for back navigation and tab scrolling.

		\param action The widget action that occurred
		\param event The event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force the action as handled, overriding normal behavior
		\return True if the action was handled by this function, false otherwise
	*/
	virtual bool		  HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Returns the menu screen at the specified index from the PDA menu handler.
	virtual idMenuScreen* GetMenuScreen( int index );

	//! Updates the audio log playing state and triggers a menu screen update when necessary.
	void				  UpdateAudioLogPlaying( bool playing );

	//! Updates the video playing state and handles associated UI and player actions.
	void				  UdpateVideoPlaying( bool playing );

	//! Resets the video playing state to false.
	void				  ClearVideoPlaying() { videoPlaying = false; }

	//! Plays or stops an audio log from a PDA entry.
	bool				  PlayPDAAudioLog( int pdaIndex, int audioIndex );

	//! Cleans up the PDA menu handler by deleting all menu screens and resetting their pointers.
	virtual void		  Cleanup();

protected:
	bool															audioLogPlaying;
	bool															videoPlaying;
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> pdaNames;
	idList<idStr, TAG_IDLIB_LIST_MENU>								navOptions;
	const idDeclAudio*												audioFile;
	idMenuWidget_ScrollBar											pdaScrollBar;
	idMenuWidget_DynamicList										pdaList;
	idMenuWidget_NavBar												navBar;
	idMenuWidget_CommandBar											commandBarWidget;
};

/*!
	\class idMenuHandler_HUD
	\brief Manages the graphical user interface elements and interactions for the heads-up display.

	The idMenuHandler_HUD class is responsible for managing all visual and interactive components of the heads-up display within the application. It handles initialization of the display with SWF
   files, updates the state of the HUD during runtime, and controls the activation and deactivation of the user interface. The class provides methods to retrieve specific screens and manage tip
   messages that appear on the display. It serves as a central controller for the HUD's behavior and appearance.

*/
class idMenuHandler_HUD : public idMenuHandler
{
public:
	//! Initializes a new instance of the idMenuHandler_HUD class with default values for its member variables.
	idMenuHandler_HUD() :
		autoHideTip( true ),
		tipStartTime( 0 ),
		hiding( false ),
		radioMessage( false )
	{
	}

	//! Updates the HUD menu handler state and transitions between screens.
	virtual void		  Update();

	//! Activates or deactivates the HUD menu system based on the show parameter.
	virtual void		  ActivateMenu( bool show );

	//! Initializes the HUD menu handler with the specified SWF file and sound world.
	virtual void		  Initialize( const char* swfFile, idSoundWorld* sw );

	//! Returns the menu screen at the specified index from the HUD menu handler.
	virtual idMenuScreen* GetMenuScreen( int index );

	//! Returns a pointer to the HUD menu screen.
	idMenuScreen_HUD*	  GetHud();

	//! Displays a tip message in the HUD with an optional auto-hide feature.
	void				  ShowTip( const char* title, const char* tip, bool autoHide );

	//! Hides the tip displayed on the HUD.
	void				  HideTip();

	//! Sets the radio message display state.
	void				  SetRadioMessage( bool show ) { radioMessage = show; }

protected:
	bool autoHideTip;
	int	 tipStartTime;
	bool hiding;
	bool radioMessage;
};

/*!
	\class idMenuHandler_Scoreboard
	\brief Handles display and management of scoreboard information during gameplay.

	The idMenuHandler_Scoreboard class provides functionality for managing and displaying scoreboard data during multiplayer gameplay. It maintains player information, team scores, and voice states
   while supporting navigation between different scoreboard screens. The class handles initialization with SWF files and sound worlds, updates scoreboard content with new player data and game
   information, and manages player-specific actions such as muting and viewing profiles. It supports team-based organization of player data and provides methods for updating spectating states and
   voice indicators. The class delegates widget action handling to active screens while falling back to parent implementation when necessary.

*/
class idMenuHandler_Scoreboard : public idMenuHandler
{
public:
	//! Initializes a new instance of the idMenuHandler_Scoreboard class with default values for scores and activation screen.
	idMenuHandler_Scoreboard() :
		redScore( 0 ),
		blueScore( 0 ),
		activationScreen( SCOREBOARD_AREA_INVALID )
	{
	}

	//! Updates the scoreboard menu handler state and transitions between different scoreboard screens.
	virtual void		  Update();

	//! Activates the scoreboard menu by setting the next screen to the activation screen.
	virtual void		  TriggerMenu();

	//! Activates or deactivates the scoreboard menu based on the show parameter.
	virtual void		  ActivateMenu( bool show );

	//! Initializes the scoreboard menu handler with the specified SWF file and sound world.
	virtual void		  Initialize( const char* swfFile, idSoundWorld* sw );

	//! Returns the menu screen at the specified index from the scoreboard menu handler.
	virtual idMenuScreen* GetMenuScreen( int index );

	/*!
		\brief Handles widget actions for the scoreboard menu, delegating to the active screen or parent handler when appropriate.

		This function processes widget actions for the scoreboard menu system. It first checks if there is an active screen set, and if not, it returns true immediately. It examines the event type to
	   determine if it's a command event, and if so, it delegates the action to the currently active menu screen if one exists. If the active screen handles the action, it plays an appropriate sound
	   based on whether the action is to go back or advance, and returns true. If the action is not handled by the active screen, it falls back to the parent menu handler's implementation for further
	   processing.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the event
		\param forceHandled Flag indicating whether the action should be forced as handled
		\return True if the action was handled, false otherwise
	*/
	virtual bool		  HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	/*!
		\brief Adds player information to the scoreboard for display

		This function processes player data and appends it to the appropriate team list in the scoreboard. The player information includes name, score, wins, ping, and spectate data. The function
	   handles special cases for spectate data and game state when deciding what to display for score. Team information determines whether the player data is added to the blue or red team list.

		\param index The index of the player in the scoreboard
		\param voiceState The voice state of the player for display
		\param team The team number (1 for blue, otherwise red)
		\param name The player's name
		\param score The player's current score
		\param wins The number of wins for the player
		\param ping The player's ping value
		\param spectateData Data related to spectating, may be empty
	*/
	void				  AddPlayerInfo( int index, voiceStateDisplay_t voiceState, int team, idStr name, int score, int wins, int ping, idStr spectateData );

	//! Updates the scoreboard with new player data and game information
	void				  UpdateScoreboard( idList<mpScoreboardInfo>& data, idStr gameInfo );
	void				  UpdateVoiceStates();

	//! Updates the scoreboard to reflect the current spectating state for the specified spectate and follow targets.
	void				  UpdateSpectating( idStr spectate, idStr follow );

	//! Sets the scores for the red and blue teams.
	void				  SetTeamScores( int r, int b );

	//! Returns the number of players on the specified team.
	int					  GetNumPlayers( int team );

	//! Sets the activation screen and transition values for the scoreboard menu handler.
	void				  SetActivationScreen( int screen, int trans );

	//! Displays the gamer card UI for the player at the specified slot
	void				  ViewPlayerProfile( int slot );

	//! Mutes the player at the specified slot
	void				  MutePlayer( int slot );

	//! Retrieves the user ID for a specified slot in the scoreboard.
	void				  GetUserID( int slot, lobbyUserID_t& luid );

	//! Updates the scoreboard selection highlight based on the current screen state.
	void				  UpdateScoreboardSelection();

protected:
	int											  redScore;
	int											  blueScore;
	int											  activationScreen;

	idList<mpScoreboardInfo>					  scoreboardInfo;
	idList<scoreboardInfo_t, TAG_IDLIB_LIST_MENU> redInfo;
	idList<scoreboardInfo_t, TAG_IDLIB_LIST_MENU> blueInfo;
};

#endif //__MENUDATA_H__
