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
#ifndef __SYS_SAVEGAME_H__
#define __SYS_SAVEGAME_H__

#ifdef OUTPUT_FUNC
	#undef OUTPUT_FUNC
#endif
#ifdef OUTPUT_FUNC_EXIT
	#undef OUTPUT_FUNC_EXIT
#endif
#define OUTPUT_FUNC()	   idLib::PrintfIf( saveGame_verbose.GetBool(), "[%s] Enter\n", __FUNCTION__ )
#define OUTPUT_FUNC_EXIT() idLib::PrintfIf( saveGame_verbose.GetBool(), "[%s] Exit\n", __FUNCTION__ )

#define DEFINE_CLASS( x )            \
	virtual const char* Name() const \
	{                                \
		return #x;                   \
	}
#define MAX_SAVEGAMES					   16
#define MAX_FILES_WITHIN_SAVEGAME		   10

#define MIN_SAVEGAME_SIZE_BYTES			   ( 4 * 1024 * 1024 )

// RB: doubled this for DoomSpartan360 mods
#define MAX_SAVEGAME_STRING_TABLE_SIZE	   ( 400 * 1024 * 2 ) // 400 kB max string table size

#define MAX_FILENAME_LENGTH				   255
#define MAX_FILENAME_LENGTH_PATTERN		   8
#define MAX_FOLDER_NAME_LENGTH			   64
#define SAVEGAME_DETAILS_FILENAME		   "game.details"

// PS3 restrictions:  The only characters that can be used are 0-9 (numbers), A-Z (uppercase alphabet), "_" (underscore), and "-" (hyphen)
#define SAVEGAME_AUTOSAVE_FOLDER		   "AUTOSAVE" // auto save slot

// common descriptors for savegame description fields
#define SAVEGAME_DETAIL_FIELD_EXPANSION	   "expansion"
#define SAVEGAME_DETAIL_FIELD_MAP		   "mapName"
#define SAVEGAME_DETAIL_FIELD_MAP_LOCATE   "mapLocation"
#define SAVEGAME_DETAIL_FIELD_DIFFICULTY   "difficulty"
#define SAVEGAME_DETAIL_FIELD_PLAYTIME	   "playTime"
#define SAVEGAME_DETAIL_FIELD_LANGUAGE	   "language"
#define SAVEGAME_DETAIL_FIELD_SAVE_VERSION "saveVersion"
#define SAVEGAME_DETAIL_FIELD_CHECKSUM	   "checksum"

#define SAVEGAME_GAME_DIRECTORY_PREFIX	   "GAME-"
#define SAVEGAME_PROFILE_DIRECTORY_PREFIX  ""
#define SAVEGAME_RAW_DIRECTORY_PREFIX	   ""

extern idCVar saveGame_verbose;
extern idCVar saveGame_enable;

class idGameSpawnInfo;
class idSession;
class idSessionLocal;
class idSaveGameManager;

// Specific savegame sub-system errors
enum saveGameError_t {
	SAVEGAME_E_NONE							   = 0,
	SAVEGAME_E_CANCELLED					   = BIT( 0 ),
	SAVEGAME_E_INSUFFICIENT_ROOM			   = BIT( 1 ),
	SAVEGAME_E_CORRUPTED					   = BIT( 2 ),
	SAVEGAME_E_UNABLE_TO_SELECT_STORAGE_DEVICE = BIT( 3 ),
	SAVEGAME_E_UNKNOWN						   = BIT( 4 ),
	SAVEGAME_E_INVALID_FILENAME				   = BIT( 5 ),
	SAVEGAME_E_STEAM_ERROR					   = BIT( 6 ),
	SAVEGAME_E_FOLDER_NOT_FOUND				   = BIT( 7 ),
	SAVEGAME_E_FILE_NOT_FOUND				   = BIT( 8 ),
	SAVEGAME_E_DLC_NOT_FOUND				   = BIT( 9 ),
	SAVEGAME_E_INVALID_USER					   = BIT( 10 ),
	SAVEGAME_E_PROFILE_TOO_BIG				   = BIT( 11 ),
	SAVEGAME_E_DISC_SWAP					   = BIT( 12 ),
	SAVEGAME_E_INCOMPATIBLE_NEWER_VERSION	   = BIT( 13 ),

	SAVEGAME_E_BITS_USED = 14,
	SAVEGAME_E_NUM		 = SAVEGAME_E_BITS_USED + 1 // because we're counting "none"
};

// Modes to control behavior of savegame manager
enum saveGameModeBitfield_t {
	SAVEGAME_MBF_NONE				= 0,
	SAVEGAME_MBF_LOAD				= BIT( 0 ), // standard file load (can be individual/multiple files described in parms)
	SAVEGAME_MBF_SAVE				= BIT( 1 ), // standard file save (can be individual/multiple files described in parms)
	SAVEGAME_MBF_DELETE_FOLDER		= BIT( 2 ), // standard package delete
	SAVEGAME_MBF_DELETE_ALL_FOLDERS = BIT( 3 ), // deletes all of the savegame folders (should only be used in testing)
	SAVEGAME_MBF_ENUMERATE			= BIT( 4 ), // gets listing of all savegame folders, typically used with READ_DETAILS to read the description file
	SAVEGAME_MBF_NO_COMPRESS		= BIT( 5 ), // tells the system the files aren't compressed, usually only needed when reading the descriptors file internally
	SAVEGAME_MBF_ENUMERATE_FILES	= BIT( 6 ), // enumerates all the files within a particular savegame folder (can be individual/multiple files or pattern described in parms)
	SAVEGAME_MBF_DELETE_FILES		= BIT( 7 ), // deletes individual files within a particular savegame folder (can be individual/multiple files or pattern described in parms)
	SAVEGAME_MBF_READ_DETAILS		= BIT( 8 ), // reads the description file (if specified, parms.enumeratedEntry.name & parms.enumeratedEntry.type must be specified)
	SAVEGAME_MBF_KEEP_FOLDER		= BIT( 9 )	// don't delete the folder before saving
};

typedef interlockedInt_t saveGameHandle_t;

typedef int				 savegameUserId_t; // [internal] hash of gamer tag for steam

/*
================================================
saveGameCheck_t
================================================
*/
struct saveGameCheck_t {
	//! Constructs a saveGameCheck_t object with default values.
	saveGameCheck_t()
	{
		exists		   = false;
		autosaveExists = false;
		autosaveFolder = NULL;
	}
	bool		exists;
	bool		autosaveExists;
	const char* autosaveFolder;
};

/*!
	\class idSaveGameDetails
	\brief Provides details about save game data including map name, location, language, playtime, and other metadata.

	This class encapsulates information related to a save game, such as the map name, location descriptor, language, playtime, expansion, difficulty, and save version. It supports comparison
   operations for ordering and equality checking based on slot names and date. The class includes methods for clearing all data and retrieving individual metadata values. It is designed to store and
   manage save game details in a structured manner, allowing for easy access and comparison of save game information.

*/
class idSaveGameDetails
{
public:
	//! Initializes a new instance of the idSaveGameDetails class.
	idSaveGameDetails();
	~idSaveGameDetails() { Clear(); }

	//! Clears all members of the save game details object.
	void			   Clear();

	//! Compares this save game details object with another for equality based on their slot names
	bool			   operator==( const idSaveGameDetails& other ) const { return ( idStr::Icmp( slotName, other.slotName ) == 0 ); }

	//! Assigns the contents of another idSaveGameDetails object to this object
	idSaveGameDetails& operator=( const idSaveGameDetails& other )
	{
		descriptors.Clear();
		descriptors = other.descriptors;
		damaged		= other.damaged;
		date		= other.date;
		slotName	= other.slotName;
		return *this;
	}

	//! Compares this save game details object with another to determine ordering based on date.
	bool  operator<( const idSaveGameDetails& other ) const { return date > other.date; }

	//! Returns the map name associated with the save game details.
	idStr GetMapName() const { return descriptors.GetString( SAVEGAME_DETAIL_FIELD_MAP, "" ); }

	//! Returns the location descriptor string from the save game details.
	idStr GetLocation() const { return descriptors.GetString( SAVEGAME_DETAIL_FIELD_MAP_LOCATE, "" ); }

	//! Returns the language identifier stored in the save game details.
	idStr GetLanguage() const { return descriptors.GetString( SAVEGAME_DETAIL_FIELD_LANGUAGE, "" ); }

	//! Returns the playtime stored in the save game details.
	int	  GetPlaytime() const { return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_PLAYTIME, 0 ); }

	//! Returns the expansion identifier for the save game details.
	int	  GetExpansion() const { return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_EXPANSION, 0 ); }

	//! Returns the difficulty level stored in the save game details.
	int	  GetDifficulty() const { return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_DIFFICULTY, -1 ); }

	//! Returns the save version number stored in the save game details.
	int	  GetSaveVersion() const { return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_SAVE_VERSION, 0 ); }

public:
	idDict								descriptors; // [in] Descriptors available to be shown on the save/load screen.  Each game can define their own, e.g. Difficulty, level, map, score, time.
	bool								damaged;	 // [out]
	time_t								date;		 // [out] read from the filesystem, not set by client
	idStrStatic<MAX_FOLDER_NAME_LENGTH> slotName;	 // [out] folder/slot name, e.g. AUTOSAVE
};

typedef idStaticList<idSaveGameDetails, MAX_SAVEGAMES> saveGameDetailsList_t;

// Making a unique_ptr to handle lifetime issues better
typedef idList<idFile_SaveGame*, TAG_SAVEGAMES>		   saveFileEntryList_t;

/*!
	\class idSaveLoadParms
	\brief Manages parameters and state for save and load operations.

	Handles the configuration and state tracking for save and load operations, including error handling and file operation management. Supports initialization with default values based on input
   device, resetting cancellation flags, and managing pipelined save game file operations. The class encapsulates the necessary parameters and provides mechanisms to track errors and handle save game
   file operations.

*/
class idSaveLoadParms
{
public:
	//! Initializes a new instance of the idSaveLoadParms class.
	idSaveLoadParms();

	//! Destructor for idSaveLoadParms that cleans up auto-deleted save game files.
	~idSaveLoadParms();

	//! Resets the cancelled flag to false.
	void					ResetCancelled();

	//! Initializes the save load parameters to their default values.
	void					Init();

	//! Initializes the save load parameters with default values based on the specified input device or the master user.
	void					SetDefaults( int inputDevice = -1 );

	//! Cancels all pipelined save game file operations by notifying them of failure or end-of-file.
	void					CancelSaveGameFilePipelines();

	//! Aborts all pipelined save game files in the parameters list.
	void					AbortSaveGameFilePipeline();

	//! Returns the error code stored in the save load parameters.
	const int&				GetError() const { return errorCode; }

	//! Returns a constant reference to the collection of error codes that are handled by this save/load parameter object.
	const int&				GetHandledErrors() const { return handledErrorCodes; }

	//! Returns the handle associated with this save game parameter object
	const saveGameHandle_t& GetHandle() const { return handle; }

public:
	idStrStatic<MAX_FOLDER_NAME_LENGTH>		 directory;	  // [in] real directory of the savegame package
	idStrStatic<MAX_FILENAME_LENGTH_PATTERN> pattern;	  // [in] pattern to use while enumerating/deleting files within a savegame folder
	idStrStatic<MAX_FILENAME_LENGTH_PATTERN> postPattern; // [in] pattern at the end of the file to use while enumerating/deleting files within a savegame folder

	int										 mode; // [in] SAVE, LOAD, ENUM, DELETE, etc.
	idSaveGameDetails	  description; // [in/out] in: description used to serialize into game.details file, out: if SAVEGAME_MBF_READ_DETAILS used with certain modes, item 0 contains the read details
	saveFileEntryList_t	  files;	   // [in/out] in: files to be saved, out: objects loaded, for SAVEGAME_MBF_ENUMERATE_FILES, it contains a listing of the filenames only
	saveGameDetailsList_t detailList;  // [out] listing of the enumerated savegames used only with SAVEGAME_MBF_ENUMERATE

	int					  errorCode;			// [out] combination of saveGameError_t bits
	int					  handledErrorCodes;	// [out] combination of saveGameError_t bits
	int64				  requiredSpaceInBytes; // [out] when fails for insufficient space, this is populated with additional space required
	int					  skipErrorDialogMask;

	// ----------------------
	// Internal vars
	// ----------------------
	idSysSignal			  callbackSignal; // [internal] used to signal savegame manager that the Process() call is completed (we still might have more Process() calls to make though...)
	volatile bool	 cancelled; // [internal] while processor is running, this can be set outside of the normal operation of the processor.  Each implementation should check this during operation to
								// allow it to shutdown cleanly.
	savegameUserId_t userId;	// [internal] to get the proper user during every step
	int				 inputDeviceId; // [internal] consoles will use this to segregate each player's files
	saveGameHandle_t handle;

private:
	//! Constructs a new instance as a copy of an existing idSaveLoadParms instance.
	idSaveLoadParms( const idSaveLoadParms& s ) { }

	//! Assigns the contents of another idSaveLoadParms instance to this instance.
	void operator=( const idSaveLoadParms& s ) { }
};

// Using function pointers because:
// 1. CompletedCallback methods in processors weren't generic enough, we could use SaveFiles processors
//		for profiles/games, but there would be a single completed callback and we'd have to update
//		the callback to detect what type of call it was, store the type in the processor, etc.
// 2. Using a functor class would require us to define classes for each callback.  The definition of those
//		classes could be scattered and a little difficult to follow
// 3. With callback methods, we assign them when needed and know exactly where they are defined/declared.
// typedef void (*saveGameProcessorCallback_t)( idSaveLoadParms & parms );

/*
================================================
saveGameThreadArgs_t
================================================
*/
struct saveGameThreadArgs_t {
	//! Constructor for saveGameThreadArgs_t that initializes the saveLoadParms to NULL.
	saveGameThreadArgs_t() :
		saveLoadParms( NULL )
	{
	}

	idSaveLoadParms* saveLoadParms;
};

/*!
	\class idSaveGameThread
	\brief A thread class responsible for managing save game operations including saving, loading, and enumerating save files.

	This class extends idSysThread to provide a dedicated thread for handling save game operations. It supports asynchronous execution of save, load, and enumeration tasks while providing mechanisms
   to cancel pending operations. The thread manages various file operations including creating, deleting, and listing save game files and directories. The class is designed to handle the complexity of
   file I/O operations in a separate thread to prevent blocking the main application thread. It provides methods for both individual save game operations and bulk operations on multiple save files or
   directories.

*/
class idSaveGameThread : public idSysThread
{
public:
	//! Constructs a new idSaveGameThread object.
	idSaveGameThread() :
		cancel( false )
	{
	}

	//! Executes the thread's main routine and returns an integer result.
	int	 Run();

	//! Signals the save game thread to cancel all pending operations.
	void CancelOperations() { cancel = true; }

private:
	//! Saves game data to the specified location
	int Save();

	//! Loads save game data from files into memory.
	int Load();

	//! Enumerates save game files and populates the details list with information about each save game.
	int Enumerate();

	//! This function deletes a complete savegame directory.
	int Delete();

	//! Deletes all savegame directories and files
	int DeleteAll();

	//! Deletes specified save game files and optionally cleans up matching files in the save game folder.
	int DeleteFiles();

	//! Enumerates save game files in a specified directory that match a given pattern.
	int EnumerateFiles();

public:
	saveGameThreadArgs_t data;
	volatile bool		 cancel;
};

/*!
	\class idSaveGameProcessor
	\brief A base class for processing save game operations with error handling and callback support.

	The idSaveGameProcessor serves as a foundational class for managing save game operations, providing mechanisms for initialization, error validation, and completion callbacks. It supports
   asynchronous processing through a dedicated thread and offers methods to check the processor's status, retrieve error codes, and manage system error dialog masks. The class is designed to be
   inherited by specific save game processing implementations that handle the actual save or load logic.

*/
class idSaveGameProcessor
{
	friend class idSaveGameManager;

public:
	DEFINE_CLASS( idSaveGameProcessor );
	static const int MAX_COMPLETED_CALLBACKS = 5;

	//! Initializes a new instance of the idSaveGameProcessor class.
	idSaveGameProcessor();
	virtual ~idSaveGameProcessor() { }

	//! Initializes the save game processor for operations.
	virtual bool Init();

	//! Returns false to indicate no additional sub-states to manage
	virtual bool Process() { return false; }

	//! Validates an error from a previous process call and returns true if the error is acceptable.
	virtual bool ValidateLastError() { return false; }

	//! Returns true to indicate this processor is simple and will not reset the map.
	virtual bool IsSimpleProcessor() const { return true; }

	//! Returns false to indicate that the save game processor should not time out
	virtual bool ShouldTimeout() const { return false; }

	//! Cancels this processor and sets the error code to SAVEGAME_E_CANCELLED
	void		 Cancel()
	{
		parms.cancelled = true;
		parms.errorCode = SAVEGAME_E_CANCELLED;
	}

	//! Returns the callback signal used for error status reporting.
	idSysSignal&		   GetSignal() { return parms.callbackSignal; }

	//! Returns the error status of the save game processing operation.
	const int&			   GetError() const { return parms.errorCode; }

	//! Returns the processor's save/load parameters.
	const idSaveLoadParms& GetParms() const { return parms; }

	//! Returns a non-const reference to the processor's save/load parameters.
	idSaveLoadParms&	   GetParmsNonConst() { return parms; }

	//! Returns whether this processor is currently working.
	bool				   IsWorking() const { return working; }

	//! Sets the error mask that determines which system errors should be skipped during processing.
	void				   SetSkipSystemErrorDialogMask( const int errorMask ) { parms.skipErrorDialogMask = errorMask; }

	//! Retrieves the mask used to determine which system error dialogs should be skipped.
	int					   GetSkipSystemErrorDialogMask() const { return parms.skipErrorDialogMask; }

	//! Returns the handle associated with the save game processor.
	saveGameHandle_t	   GetHandle() const { return parms.GetHandle(); }

	//! Adds a callback to be invoked when the save game processor has completed its execution.
	void				   AddCompletedCallback( const idCallback& callback );

private:
	//! Returns whether or not the save game processing thread has finished operating.
	bool IsThreadFinished();

protected:
	idSaveLoadParms parms;
	int				savegameLogicTestIterator;

private:
	bool											   init;
	bool											   working;

	idStaticList<idCallback*, MAX_COMPLETED_CALLBACKS> completedCallbacks;
};

/*!
	\class idSaveGameManager
	\brief Manages asynchronous save game operations through a processor strategy pattern.

	Why all the object-oriented nonsense?

	- Savegames need to be processed asynchronously, saving/loading/deleting files should happen during the game frame
	so there is a common way to update the render device.

	- When executing commands, if no "strategy"s are used, the pump() method would need to have a switch statement,
	extending the manager for other commands would mean modifying the manager itself for various commands.
	By making it a strategy, we are able to create custom commands and define the behavior within game code and keep
	the manager code in the engine static.

*/
class idSaveGameManager
{
public:
	enum packageType_t { PACKAGE_PROFILE, PACKAGE_GAME, PACKAGE_RAW, PACKAGE_NUM };

	const static int MAX_SAVEGAME_DIRECTORY_DEPTH = 5;

	//! Initializes a new instance of the idSaveGameManager class.
	explicit idSaveGameManager();

	//! Destructor for the idSaveGameManager class that cleans up internal resources.
	~idSaveGameManager();

	//! Processes save game operations in the main game thread.
	void			  Pump();

	//! Checks if a storage device has been selected for saving.
	bool			  IsStorageAvailable() const { return storageAvailable; }

	//! Sets the storage availability status.
	void			  SetStorageAvailable( const bool available ) { storageAvailable = available; }

	//! Checks if the save game manager has an active processor.
	bool			  IsWorking() const;

	//! Assigns a processor to the manager for asynchronous execution and returns a handle to track its progress.
	saveGameHandle_t  ExecuteProcessor( idSaveGameProcessor* processor );

	//! Executes a save game processor synchronously and waits for completion
	saveGameHandle_t  ExecuteProcessorAndWait( idSaveGameProcessor* processor );

	//! Clears the processor queue while allowing currently processing items to finish
	void			  Clear();

	//! Waits for all save game processors to complete execution
	void			  WaitForAllProcessors( bool overrideSimpleProcessorCheck = false );

	//! Returns whether the save game operation has been cancelled.
	const bool		  IsCancelled() const { return cancel; }

	//! Cancels all save game processors and optionally forces cancellation of any in-flight processor.
	void			  CancelAllProcessors( const bool forceCancelInFlightProcessor );

	//! Cancels the save game processor and terminates the save thread.
	void			  CancelToTerminate();

	//! Returns a reference to the save game thread managed by this save game manager.
	idSaveGameThread& GetSaveGameThread() { return saveThread; }

	//! Checks if a save game operation identified by the handle has been completed.
	bool			  IsSaveGameCompletedFromHandle( const saveGameHandle_t& handle ) const
	{
		return handle <= lastExecutedProcessorHandle || handle == 0; // last case should never be reached since it would be also be true in first case, this is just to show intent
	}

	//! Sets the folder and byte count for retry save operations after a device is selected on Xbox 360.
	void						 Set360RetrySaveAfterDeviceSelected( const char* folder, const int64 bytes );

	//! Returns true if the save manager is waiting to retry a save operation to the game autosave folder.
	bool						 DeviceSelectorWaitingOnSaveRetry();

	//! Displays a dialog to the user when there is insufficient storage space to save the game.
	void						 ShowRetySaveDialog( const char* folder, const int64 bytes );

	//! Displays a dialog for retrying save operations.
	void						 ShowRetySaveDialog();

	//! Clears the retry information stored in the save game manager.
	void						 ClearRetryInfo();

	//! Attempts to retry a save game operation.
	void						 RetrySave();

	//! Cancels a save or load operation identified by the given handle if it is not yet completed
	void						 CancelWithHandle( const saveGameHandle_t& handle );

	//! Returns the list of enumerated savegames
	const saveGameDetailsList_t& GetEnumeratedSavegames() const { return enumeratedSaveGames; }

	//! Returns a non-const reference to the list of enumerated save games.
	saveGameDetailsList_t&		 GetEnumeratedSavegamesNonConst() { return enumeratedSaveGames; }

private:
	//! Initializes and starts the next simple processor in the queue for execution.
	void StartNextProcessor();

	//! Finalizes a save game processor and cleans up its auto-deletable files
	void FinishProcessor( idSaveGameProcessor* processor );

	// Calls start on the processor after it's been assigned
	void Start();

private:
	idSaveGameProcessor*				  processor;
	idStaticList<idSaveGameProcessor*, 4> processorQueue;
	bool								  cancel;
	idSaveGameThread					  saveThread;
	int									  startTime;
	bool								  continueProcessing;
	saveGameHandle_t					  submittedProcessorHandle;
	saveGameHandle_t					  executingProcessorHandle;
	saveGameHandle_t					  lastExecutedProcessorHandle;
	saveGameDetailsList_t				  enumeratedSaveGames;
	bool								  storageAvailable; // On 360, this is false by default, after the storage device is selected
	// it becomes true.  This allows us to start the game without a storage device
	// selected and pop the selector when necessary.
	const char*							  retryFolder;
	int64								  retryBytes;
	bool								  retrySave;
	idSysSignal							  deviceRequestedSignal;
};

//! Bridges session APIs to the savegame thread for asynchronous save operations.
void		 Sys_ExecuteSavegameCommandAsync( idSaveLoadParms* savegameParms );

//! Checks if any savegames exist and whether an autosave exists.
void		 Sys_SaveGameCheck( bool& exists, bool& autosaveExists );

//! Returns the save folder path prefix for a given package type.
const idStr& GetSaveFolder( idSaveGameManager::packageType_t type );

//! Constructs a full save game folder path by prefixing the input folder name with the appropriate base save folder path for the specified package type.
idStr		 AddSaveFolderPrefix( const char* folder, idSaveGameManager::packageType_t type );

//! Removes the save folder prefix from the given folder path based on the specified package type.
idStr		 RemoveSaveFolderPrefix( const char* folder, idSaveGameManager::packageType_t type );

//! Reads save game details from a file and validates the checksum, marking the save as damaged if validation fails.
bool		 SavegameReadDetailsFromFile( idFile* file, idSaveGameDetails& details );

//! Converts a save game error mask into a human-readable string representation
idStr		 GetSaveGameErrorString( int errorMask );

#endif // __SYS_SAVEGAME_H__
