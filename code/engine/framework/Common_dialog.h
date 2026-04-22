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

#ifndef __COMMON_DIALOG_H__
#define __COMMON_DIALOG_H__

static const int MAX_DIALOGS	  = 4; // maximum dialogs that can be open at one time
static const int PC_KEYBOARD_WAIT = 20000;

/*
================================================
Dialog box message types
================================================
*/
enum gameDialogMessages_t {
	GDM_INVALID,
	GDM_SWAP_DISKS_TO1,
	GDM_SWAP_DISKS_TO2,
	GDM_SWAP_DISKS_TO3,
	GDM_NO_GAMER_PROFILE,
	GDM_PLAY_ONLINE_NO_PROFILE,
	GDM_LEADERBOARD_ONLINE_NO_PROFILE,
	GDM_NO_STORAGE_SELECTED,
	GDM_ONLINE_INCORRECT_PERMISSIONS,
	GDM_SP_QUIT_SAVE,
	GDM_SP_RESTART_SAVE,
	GDM_SP_SIGNIN_CHANGE,
	GDM_SERVER_NOT_AVAILABLE,
	GDM_CONNECTION_LOST_HOST,
	GDM_CONNECTION_LOST,
	GDM_OPPONENT_CONNECTION_LOST,
	GDM_HOST_CONNECTION_LOST,
	GDM_HOST_CONNECTION_LOST_STATS,
	GDM_FAILED_TO_LOAD_RANKINGS,
	GDM_HOST_QUIT,
	GDM_BECAME_HOST_PARTY,				// Became host of party
	GDM_NEW_HOST_PARTY,					// Someone else became host of party
	GDM_LOBBY_BECAME_HOST_GAME,			// In lobby, you became game host
	GDM_LOBBY_NEW_HOST_GAME,			// In lobby, new game host was chosen (not you)
	GDM_NEW_HOST_GAME,					// Host left/DC'd, someone else is new host, unranked game
	GDM_NEW_HOST_GAME_STATS_DROPPED,	// Host left/DC'd, someone else is new host, ranked game so stats were dropped
	GDM_BECAME_HOST_GAME,				// Host left/DC'd, you became host, unranked game
	GDM_BECAME_HOST_GAME_STATS_DROPPED, // Host left/DC'd, you became host, ranked game so stats were dropped
	GDM_LOBBY_DISBANDED,
	GDM_LEAVE_WITH_PARTY,
	GDM_LEAVE_LOBBY_RET_MAIN,
	GDM_LEAVE_LOBBY_RET_NEW_PARTY,
	GDM_MIGRATING,
	GDM_OPPONENT_LEFT,
	GDM_NO_MATCHES_FOUND,
	GDM_INVALID_INVITE,
	GDM_KICKED,
	GDM_BANNED,
	GDM_SAVING,
	GDM_OVERWRITE_SAVE,
	GDM_LOAD_REQUEST,
	GDM_AUTOSAVE_DISABLED_STORAGE_REMOVED,
	GDM_STORAGE_INVALID,
	GDM_STORAGE_REMOVED,
	GDM_CONNECTING,
	GDM_REFRESHING,
	GDM_DELETE_SAVE,
	GDM_DELETING,
	GDM_BINDING_ALREDY_SET,
	GDM_CANNOT_BIND,
	GDM_OVERLAY_DISABLED,
	GDM_DIRECT_MAP_CHANGE,
	GDM_DELETE_AUTOSAVE,
	GDM_QUICK_SAVE,
	GDM_MULTI_RETRY,
	GDM_MULTI_SELF_DESTRUCT,
	GDM_MULTI_VDM_QUIT,
	GDM_MULTI_COOP_QUIT,
	GDM_LOADING_PROFILE,
	GDM_STORAGE_REQUIRED,
	GDM_INSUFFICENT_STORAGE_SPACE,
	GDM_PARTNER_LEFT,
	GDM_RESTORE_CORRUPT_SAVEGAME,
	GDM_UNRECOVERABLE_SAVEGAME,
	GDM_PROFILE_SAVE_ERROR,
	GDM_LOBBY_FULL,
	GDM_QUIT_GAME,
	GDM_CONNECTION_PROBLEMS,
	GDM_VOICE_RESTRICTED,
	GDM_LOAD_DAMAGED_FILE,
	GDM_MUST_SIGNIN,
	GDM_CONNECTION_LOST_NO_LEADERBOARD,
	GDM_SP_SIGNIN_CHANGE_POST,
	GDM_MIGRATING_WAITING,
	GDM_MIGRATING_RELAUNCHING,
	GDM_MIGRATING_FAILED_CONNECTION,
	GDM_MIGRATING_FAILED_CONNECTION_STATS,
	GDM_MIGRATING_FAILED_DISBANDED,
	GDM_MIGRATING_FAILED_DISBANDED_STATS,
	GDM_MIGRATING_FAILED_PARTNER_LEFT,
	GDM_HOST_RETURNED_TO_LOBBY,
	GDM_HOST_RETURNED_TO_LOBBY_STATS_DROPPED,
	GDM_FAILED_JOIN_LOCAL_SESSION,
	GDM_DELETE_CORRUPT_SAVEGAME,
	GDM_LEAVE_INCOMPLETE_INSTANCE,
	GDM_UNBIND_CONFIRM,
	GDM_BINDINGS_RESTORE,
	GDM_NEW_HOST,
	GDM_CONFIRM_VIDEO_CHANGES,
	GDM_UNABLE_TO_USE_SELECTED_STORAGE_DEVICE,
	GDM_ERROR_LOADING_SAVEGAME,
	GDM_ERROR_SAVING_SAVEGAME,
	GDM_DISCARD_CHANGES,
	GDM_LEAVE_LOBBY,
	GDM_LEAVE_LOBBY_AND_TEAM,
	GDM_CONTROLLER_DISCONNECTED_0,
	GDM_CONTROLLER_DISCONNECTED_1,
	GDM_CONTROLLER_DISCONNECTED_2,
	GDM_CONTROLLER_DISCONNECTED_3,
	GDM_CONTROLLER_DISCONNECTED_4,
	GDM_CONTROLLER_DISCONNECTED_5,
	GDM_CONTROLLER_DISCONNECTED_6,
	GDM_DLC_ERROR_REMOVED,
	GDM_DLC_ERROR_CORRUPT,
	GDM_DLC_ERROR_MISSING,
	GDM_DLC_ERROR_MISSING_GENERIC,
	GDM_DISC_SWAP,
	GDM_NEEDS_INSTALL,
	GDM_NO_SAVEGAMES_AVAILABLE,
	GDM_ERROR_JOIN_TWO_PROFILES_ONE_BOX,
	GDM_WARNING_PLAYING_COOP_SOLO,
	GDM_MULTI_COOP_QUIT_LOSE_LEADERBOARDS,
	GDM_CORRUPT_CONTINUE,
	GDM_MULTI_VDM_QUIT_LOSE_LEADERBOARDS,
	GDM_WARNING_PLAYING_VDM_SOLO,
	GDM_NO_GUEST_SUPPORT,
	GDM_DISC_SWAP_CONFIRMATION,
	GDM_ERROR_LOADING_PROFILE,
	GDM_CANNOT_INVITE_LOBBY_FULL,
	GDM_WARNING_FOR_NEW_DEVICE_ABOUT_TO_LOSE_PROGRESS,
	GDM_DISCONNECTED,
	GDM_INCOMPATIBLE_NEWER_SAVE,
	GDM_ACHIEVEMENTS_DISABLED_DUE_TO_CHEATING,
	GDM_INCOMPATIBLE_POINTER_SIZE,
	GDM_TEXTUREDETAIL_RESTARTREQUIRED,
	GDM_TEXTUREDETAIL_INSUFFICIENT_CPU,
	GDM_CHECKPOINT_SAVE,
	GDM_CALCULATING_BENCHMARK,
	GDM_DISPLAY_BENCHMARK,
	GDM_DISPLAY_CHANGE_FAILED,
	GDM_GPU_TRANSCODE_FAILED,
	GDM_OUT_OF_MEMORY,
	GDM_CORRUPT_PROFILE,
	GDM_PROFILE_TOO_OUT_OF_DATE_DEVELOPMENT_ONLY,
	GDM_SP_LOAD_SAVE,
	GDM_INSTALLING_TROPHIES,
	GDM_XBOX_DEPLOYMENT_TYPE_FAIL,
	GDM_SAVEGAME_WRONG_LANGUAGE,
	GDM_GAME_RESTART_REQUIRED,
	GDM_MAX
};

/*
================================================
Dialog box types
================================================
*/
enum dialogType_t {
	DIALOG_INVALID = -1,
	DIALOG_ACCEPT,
	DIALOG_CONTINUE,
	DIALOG_ACCEPT_CANCEL,
	DIALOG_YES_NO,
	DIALOG_CANCEL,
	DIALOG_WAIT,
	DIALOG_WAIT_BLACKOUT,
	DIALOG_WAIT_CANCEL,
	DIALOG_DYNAMIC,
	DIALOG_QUICK_SAVE,
	DIALOG_TIMER_ACCEPT_REVERT,
	DIALOG_CRAWL_SAVE,
	DIALOG_CONTINUE_LARGE,
	DIALOG_BENCHMARK,
};

/*!
	\class idDialogInfo
	\brief Manages dialog information for in-game conversations.
*/
class idDialogInfo
{
public:
	//! Initializes a new instance of the idDialogInfo class with default values.
	idDialogInfo()
	{
		msg				 = GDM_INVALID;
		type			 = DIALOG_ACCEPT;
		acceptCB		 = NULL;
		cancelCB		 = NULL;
		altCBOne		 = NULL;
		altCBTwo		 = NULL;
		showing			 = false;
		clear			 = false;
		waitClear		 = false;
		pause			 = false;
		startTime		 = 0;
		killTime		 = 0;
		leaveOnClear	 = false;
		renderDuringLoad = false;
	}
	gameDialogMessages_t msg;
	dialogType_t		 type;
	idSWFScriptFunction* acceptCB;
	idSWFScriptFunction* cancelCB;
	idSWFScriptFunction* altCBOne;
	idSWFScriptFunction* altCBTwo;
	bool				 showing;
	bool				 clear;
	bool				 waitClear;
	bool				 pause;
	bool				 forcePause;
	bool				 leaveOnClear;
	bool				 renderDuringLoad;
	int					 startTime;
	int					 killTime;
	idStrStatic<256>	 overrideMsg;

	idStrId				 txt1;
	idStrId				 txt2;
	idStrId				 txt3;
	idStrId				 txt4;
};

/*!
	\class idLoadScreenInfo
	\brief Manages information displayed during load screens.
*/
class idLoadScreenInfo
{
public:
	idStr varName;
	idStr value;
};

/*!
	\class idCommonDialog
	\brief Manages game dialog interfaces including display, rendering, and event handling.

	The idCommonDialog class serves as the central system for managing all dialog interfaces within the application. It handles the initialization, rendering, and shutdown of dialog systems while
   providing methods to add, clear, and control various dialog messages. The class supports both static and dynamic dialogs with customizable callbacks and display options, allowing for flexible UI
   interactions. It maintains an internal list of dialog entries and manages their lifecycle including activation, deactivation, and rendering during different application states such as loading. The
   system also handles dialog events and can pause the game when dialogs are active, providing hooks for script callbacks and localization support for dialog messages and titles. The class ensures
   proper resource management through initialization and shutdown routines, and offers utilities to clear specific or all dialogs as needed for different game states and conditions.

*/
class idCommonDialog
{
public:
	//! Initializes the common dialog system by setting up SWF dialogs and binding dialog constants.
	void  Init();

	//! Renders the common dialog interface, handling message display and UI updates.
	void  Render( bool loading );

	//! Shuts down the common dialog system by clearing dialogs and deleting associated resources.
	void  Shutdown();

	//! Restarts the common dialog system by shutting down and reinitializing it.
	void  Restart();

	//! Returns true if dialog is currently pausing the game, false otherwise.
	bool  IsDialogPausing() { return dialogPause; }

	//! Clears dialog messages from the list, optionally forcing clear all messages.
	void  ClearDialogs( bool forceClear = false );

	//! Checks if a dialog message is present in the dialog system and optionally returns whether it is currently active
	bool  HasDialogMsg( gameDialogMessages_t msg, bool* isNowActive );

	/*!
		\brief Adds a dialog to be displayed with specified message, type, and callback functions.

		This function creates and adds a dialog entry to the dialog system. It initializes the dialog with the provided parameters including message type, accept and cancel callbacks, pause behavior,
	   and various flags for controlling the dialog's lifecycle and rendering. The dialog is added to an internal dialog list managed by the system.

		\param msg The game dialog message to display
		\param type The type of dialog to create
		\param acceptCallback Callback function to invoke when the dialog is accepted
		\param cancelCallback Callback function to invoke when the dialog is cancelled
		\param pause Whether to pause the game when the dialog is shown
		\param location Source file location where this function was called from
		\param lineNumber Line number in the source file where this function was called
		\param leaveOnMapHeapReset Whether to preserve the dialog when the map heap is reset
		\param waitOnAtlas Whether to wait for texture atlas loading before displaying
		\param renderDuringLoad Whether to render the dialog during asset loading
	*/
	void  AddDialog( gameDialogMessages_t msg,
		 dialogType_t					  type,
		 idSWFScriptFunction*			  acceptCallback,
		 idSWFScriptFunction*			  cancelCallback,
		 bool							  pause,
		 const char*					  location			  = NULL,
		 int							  lineNumber		  = 0,
		 bool							  leaveOnMapHeapReset = false,
		 bool							  waitOnAtlas		  = false,
		 bool							  renderDuringLoad	  = false );

	/*!
		\brief Adds a dynamic dialog to the dialog system with specified message, callbacks, and options.

		This function creates and adds a dynamic dialog to the dialog system. It initializes a dialog info structure with the provided parameters including message, callbacks for different dialog
	   actions, option text for dialog buttons, and various flags controlling dialog behavior. The dialog is added to the internal dialog list for rendering and processing. The function handles up to
	   four callbacks and four option text entries for different dialog buttons.

		\param msg The game dialog message identifier to display
		\param callbacks List of script function callbacks for dialog actions (accept, cancel, alt1, alt2)
		\param optionText List of string identifiers for dialog button texts
		\param pause Whether to pause the game when the dialog is displayed
		\param overrideMsg Override message text to display instead of the message identifier
		\param leaveOnMapHeapReset Whether to keep the dialog when the map heap is reset
		\param waitOnAtlas Whether to wait for atlas loading before displaying
		\param renderDuringLoad Whether to render the dialog during asset loading
	*/
	void  AddDynamicDialog( gameDialogMessages_t	  msg,
		 const idStaticList<idSWFScriptFunction*, 4>& callbacks,
		 const idStaticList<idStrId, 4>&			  optionText,
		 bool										  pause,
		 idStrStatic<256>							  overrideMsg,
		 bool										  leaveOnMapHeapReset = false,
		 bool										  waitOnAtlas		  = false,
		 bool										  renderDuringLoad	  = false );

	//! Sets an integer value for a specified dialog name if the dialog is valid.
	void  AddDialogIntVal( const char* name, int val );

	//! Returns true if a dialog is currently active
	bool  IsDialogActive();

	//! Clears a specific dialog message from the dialog system
	void  ClearDialog( gameDialogMessages_t msg, const char* location = NULL, int lineNumber = 0 );

	//! Displays or hides the save indicator dialog based on the show parameter.
	void  ShowSaveIndicator( bool show );

	//! Returns true if there is at least one active dialog in the message list.
	bool  HasAnyActiveDialog() const { return ( messageList.Num() > 0 ) && ( !messageList[0].clear ); }

	//! Clears all dialog entries in the message list by setting their clear flags.
	void  ClearAllDialogHack();

	//! Returns a localized dialog message and title based on the specified game dialog message type
	idStr GetDialogMsg( gameDialogMessages_t msg, idStr& message, idStr& title );

	//! Handles a system event for the dialog, returning true if the event was processed.
	bool  HandleDialogEvent( const sysEvent_t* sev );

protected:
	//! Removes wait dialogs from the message list.
	void RemoveWaitDialogs();

	//! Displays a dialog box with the specified message and options.
	void ShowDialog( const idDialogInfo& info );

	//! Displays the next pending dialog message from the message list.
	void ShowNextDialog();

	//! Activates or deactivates the common dialog.
	void ActivateDialog( bool activate );

	//! Adds a dialog to the internal list, avoiding duplicates and managing dialog display.
	void AddDialogInternal( idDialogInfo& info );

	//! Releases all callback objects associated with the message at the specified index
	void ReleaseCallBacks( int index );

private:
	bool									dialogPause;
	idSWF*									dialog;
	idSWF*									saveIndicator;
	bool									dialogShowingSaveIndicatorRequested;
	int										dialogShowingSaveIndicatorTimeRemaining;

	idStaticList<idDialogInfo, MAX_DIALOGS> messageList;
	idStaticList<idLoadScreenInfo, 16>		loadScreenInfo;

	int										startSaveTime; // with stopSaveTime, useful to pass 360 TCR# 047.  Need to keep the dialog on the screen for a minimum amount of time
	int										stopSaveTime;
	bool									dialogInUse; // this is to prevent an active msg getting lost during a map heap reset
};

#endif
