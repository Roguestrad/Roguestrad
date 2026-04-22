/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

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

static const int MAX_USERCMD_BACKUP = 256;
static const int NUM_USERCMD_RELAY	= 10;
static const int NUM_USERCMD_SEND	= 8;

static const int initialHz				= 60;
static const int initialBaseTicks		= 1000 / initialHz;
static const int initialBaseTicksPerSec = initialHz * initialBaseTicks;

static const int LOAD_TIP_CHANGE_INTERVAL = 12000;
static const int LOAD_TIP_COUNT			  = 26;

/*!
	\class idGameThread
	\brief Manages threaded execution of game logic and rendering with time tracking capabilities.

	The idGameThread class extends idSysThread to provide specialized functionality for executing game logic and rendering operations in a threaded environment. It supports both threaded and main
   thread execution based on SMP settings, allowing for parallel processing of game frames while maintaining proper synchronization. The class handles user command preparation and execution, tracking
   game, render, and total thread times for performance monitoring and optimization. It provides methods for initiating game frame execution cycles and retrieving timing information for the current
   thread's execution.

*/
class idGameThread : public idSysThread
{
public:
	//! Initializes a new instance of the idGameThread class.
	idGameThread() :
		gameTime(),
		drawTime(),
		threadTime(),
		threadGameTime(),
		threadRenderTime(),
		userCmdMgr( NULL ),
		ret(),
		numGameFrames(),
		isClient()
	{
	}

	/*!
		\brief Executes game logic and drawing for a specified number of frames in a threaded manner, returning the result from the previous frame.

		This function prepares user commands for background execution and starts either a threaded or main thread execution based on SMP settings. It returns the game return value from the previous
	   frame while the new frame runs in parallel. The function ensures that the user commands are properly saved and the thread is signaled to begin execution, or runs the game directly in the main
	   thread for non-SMP configurations or editor modes.

		\param numGameFrames Number of game frames to execute
		\param userCmdMgr_ Reference to the user command manager containing commands for the game frames
		\param isClient_ Flag indicating if this is a client thread
		\param startGameFrame Starting game frame number for the execution
		\return The game return value from the previous frame execution.
	*/
	gameReturn_t RunGameAndDraw( int numGameFrames, idUserCmdMgr& userCmdMgr_, bool isClient_, int startGameFrame );

	//! Sets the total thread time value.
	void		 SetThreadTotalTime( const int inTime ) { threadTime = inTime; }

	//! Returns the total time spent by the game thread.
	int			 GetThreadTotalTime() const { return threadTime; }

	//! Sets the game time value for the current thread.
	void		 SetThreadGameTime( const int time ) { threadGameTime = time; }

	//! Returns the game time associated with the current thread.
	int			 GetThreadGameTime() const { return threadGameTime; }

	//! Sets the render time for the game thread.
	void		 SetThreadRenderTime( const int time ) { threadRenderTime = time; }

	//! Returns the render time of the game thread.
	int			 GetThreadRenderTime() const { return threadRenderTime; }

private:
	//! Executes the main game thread routine including game logic and rendering.
	virtual int	  Run();

	int			  gameTime;
	int			  drawTime;
	int			  threadTime;		// total time : game time + foreground render time
	int			  threadGameTime;	// game time only
	int			  threadRenderTime; // render fg time only
	idUserCmdMgr* userCmdMgr;
	gameReturn_t  ret;
	int			  numGameFrames;
	bool		  isClient;
};

enum errorParm_t {
	ERP_NONE,
	ERP_FATAL,	   // exit the entire game with a popup window
	ERP_DROP,	   // print to console and disconnect from game
	ERP_DISCONNECT // don't kill server
};

enum gameLaunch_t {
	LAUNCH_TITLE_DOOM = 0,
	LAUNCH_TITLE_DOOM2,
};

struct netTimes_t {
	int localTime;
	int serverTime;
};

struct frameTiming_t {
	uint64 startSyncTime;
	uint64 finishSyncTime;
	uint64 startGameTime;
	uint64 finishGameTime;
	uint64 finishDrawTime;
	uint64 startRenderTime;
	uint64 finishRenderTime;
	uint64 finishSyncTime_EndFrame;
};

#define MAX_PRINT_MSG_SIZE			  4096
#define MAX_WARNING_LIST			  256

#define SAVEGAME_CHECKPOINT_FILENAME  "gamedata.save"
#define SAVEGAME_DESCRIPTION_FILENAME "gamedata.txt"
#define SAVEGAME_STRINGS_FILENAME	  "gamedata.strings"

/*!
	\class idCommonLocal
	\brief Provides core engine initialization, management, and runtime services for the application.

	This class serves as the primary interface for engine initialization, configuration management, and runtime operation. It handles system initialization with command line arguments, manages game
   state, processes input events, and coordinates rendering and networking activities. The class maintains the main execution loop through its Frame method and provides facilities for saving and
   loading game states, managing console output, and handling user interactions. It also includes functionality for multiplayer networking, map loading, and resource management. The class implements
   various utility methods for debugging, logging, and system diagnostics, making it the central hub for engine-wide operations and coordination between different subsystems.

*/
class idCommonLocal : public idCommon
{
	friend class idConsoleLocal;

public:
	//! Initializes all members of the idCommonLocal class to their default values.
	idCommonLocal();

	//! Initializes the common engine components with the specified command line arguments.
	virtual void				   Init( int argc, const char* const* argv, const char* cmdline );

	//! Shuts down the common engine systems and releases all allocated resources.
	virtual void				   Shutdown();

	//! Creates the main menu for the game.
	virtual void				   CreateMainMenu();

	//! Terminates the application by shutting down the common system and exiting the process.
	virtual void				   Quit();

	//! Returns true if the common system has been fully initialized.
	virtual bool				   IsInitialized() const;

	//! Main frame processing function that updates game state, handles input, and manages rendering
	virtual void				   Frame();

	//! Updates the screen by rendering the current frame and handling mouse capture.
	virtual void				   UpdateScreen( bool captureToImage, bool releaseMouse = true );

	//! Updates the level load pacifier during map changes and background swaps.
	virtual void				   UpdateLevelLoadPacifier();

	//! Processes command line set commands for console variables, optionally filtering by a specific variable name.
	virtual void				   StartupVariable( const char* match );

	//! Initializes a tool with the specified flags, dictionary, and entity.
	virtual void				   InitTool( const toolFlag_t tool, const idDict* dict, idEntity* entity );

	//! Writes the current configuration to the specified file.
	virtual void				   WriteConfigToFile( const char* filename );

	//! Begins redirection of console output to the specified buffer.
	virtual void				   BeginRedirect( char* buffer, int buffersize, void ( *flush )( const char* ) );

	//! Stops redirection of console output.
	virtual void				   EndRedirect();

	//! Sets whether the screen should be refreshed after printing messages.
	virtual void				   SetRefreshOnPrint( bool set );
	virtual void				   Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );

	//! Prints a formatted message to the console and log file, handling varargs input.
	virtual void				   VPrintf( const char* fmt, va_list arg );
	virtual void				   DPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void				   VerbosePrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void				   Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void				   DWarning( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );

	//! Prints all queued warning messages.
	virtual void				   PrintWarnings();

	//! Clears all queued warnings and sets the warning caption to the provided reason.
	virtual void				   ClearWarnings( const char* reason );
	virtual void				   Error( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void				   FatalError( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );

	//! Returns true if the common initialization is complete.
	virtual bool				   IsShuttingDown() const { return com_shuttingDown; }

	//! Returns the key bound to the specified command.
	virtual const char*			   KeysFromBinding( const char* bind );

	//! Returns the command bound to the specified key
	virtual const char*			   BindingFromKey( const char* key );

	//! Returns true if the game is currently in multiplayer mode.
	virtual bool				   IsMultiplayer();

	//! Checks if the current game instance is running as a server.
	virtual bool				   IsServer();

	//! Returns true if the current game session is running as a multiplayer client.
	virtual bool				   IsClient();

	//! Returns true if the player has ever enabled the console
	virtual bool				   GetConsoleUsed() { return consoleUsed; }

	//! Returns the current network snapshot rate setting
	virtual int					   GetSnapRate();

	//! Handles receiving a reliable network message from a peer
	virtual void				   NetReceiveReliable( int peer, int type, idBitMsg& msg );

	//! Processes and stores an incoming network snapshot while managing the snapshot buffer.
	virtual void				   NetReceiveSnapshot( class idSnapShot& ss );

	//! Handles incoming user commands from a specific network peer
	virtual void				   NetReceiveUsercmds( int peer, idBitMsg& msg );

	//! Reads user commands from a network message for a specified client
	void						   NetReadUsercmds( int clientNum, idBitMsg& msg );

	//! Processes a system event, handling dialog, console, menu, and game input events.
	virtual bool				   ProcessEvent( const sysEvent_t* event );

	//! Loads a saved game from the specified save name
	virtual bool				   LoadGame( const char* saveName );

	//! Saves the current game state to a specified save file.
	virtual bool				   SaveGame( const char* saveName );

	//! Returns the state of the specified button
	virtual int					   ButtonState( int key );

	//! Returns the state of the specified key
	virtual int					   KeyState( int key );

	//! Returns a pointer to the active game instance.
	virtual idGame*				   Game() { return game; }

	//! Returns the render world interface.
	virtual idRenderWorld*		   RW() { return renderWorld; }

	//! Returns the sound world instance used by the common interface.
	virtual idSoundWorld*		   SW() { return soundWorld; }

	//! Returns the sound world used for menu audio processing.
	virtual idSoundWorld*		   MenuSW() { return menuSoundWorld; }

	//! Returns a pointer to the current session object.
	virtual idSession*			   Session() { return session; }

	//! Returns a reference to the common dialog interface used for displaying user notifications and prompts.
	virtual idCommonDialog&		   Dialog() { return commonDialog; }

	//! Handles the completion of a save game operation, including cleanup and error reporting
	virtual void				   OnSaveCompleted( idSaveLoadParms& parms );

	//! Handles the completion of a savegame load operation and displays an error dialog if loading failed.
	virtual void				   OnLoadCompleted( idSaveLoadParms& parms );

	//! Handles the completion of loading save game files and initiates the game load process.
	virtual void				   OnLoadFilesCompleted( idSaveLoadParms& parms );

	//! Handles the completion of save game enumeration, updating the saved games list on success.
	virtual void				   OnEnumerationCompleted( idSaveLoadParms& parms );

	//! Handles the completion of a save game deletion operation
	virtual void				   OnDeleteCompleted( idSaveLoadParms& parms );

	//! Initializes and completes a screen wipe effect using the specified material and hold setting.
	virtual void				   TriggerScreenWipe( const char* _wipeMaterial, bool hold );

	//! Handles the setup of match parameters when starting to host a game session.
	virtual void				   OnStartHosting( idMatchParameters& parms );

	//! Returns the current game frame count.
	virtual int					   GetGameFrame() { return gameFrame; }

	//! Initializes the list of multiplayer maps and game modes by loading data from the game and map definitions.
	virtual void				   InitializeMPMapsModes();

	//! Returns a constant reference to the list of available game modes.
	virtual const idStrList&	   GetModeList() const { return mpGameModes; }

	//! Returns the list of multiplayer mode display names.
	virtual const idStrList&	   GetModeDisplayList() const { return mpDisplayGameModes; }

	//! Returns the list of multiplayer maps available in the game.
	virtual const idList<mpMap_t>& GetMapList() const { return mpGameMaps; }

	//! Resets the input state for the specified player index
	virtual void				   ResetPlayerInput( int playerIndex );

	//! Returns true if Japanese censorship is enabled, otherwise false.
	virtual bool				   JapaneseCensorship() const;

	//! Sets a flag to request the shell to be shown on the next frame.
	virtual void				   QueueShowShell() { showShellRequested = true; }

public:
	//! Draws the game frame and handles various rendering states including loading screens, game views, and console overlays.
	void   Draw();

	//! Waits for the game thread to complete its execution.
	void   WaitGameThread() { gameThread.WaitForThread(); }

	//! Returns the total elapsed time in milliseconds for the game thread.
	int	   GetGameThreadTotalTime() const { return gameThread.GetThreadTotalTime(); }

	//! Returns the game thread's elapsed game time in milliseconds.
	int	   GetGameThreadGameTime() const { return gameThread.GetThreadGameTime(); }

	//! Returns the render time of the game thread in milliseconds.
	int	   GetGameThreadRenderTime() const { return gameThread.GetThreadRenderTime(); }

	//! Returns the backend rendering time in microseconds.
	uint64 GetRendererBackEndMicroseconds() const { return time_backend; }

	//! Returns the time spent on masked occlusion rasterization in microseconds.
	uint64 GetRendererMaskedOcclusionRasterizationMicroseconds() const { return time_moc; }

	//! Returns the number of microseconds the renderer was idle between the start of rendering and the completion of synchronization.
	uint64 GetRendererIdleMicroseconds() const { return mainFrameTiming.startRenderTime - mainFrameTiming.finishSyncTime; }

	//! Returns the GPU time in microseconds for the renderer.
	uint64 GetRendererGPUMicroseconds() const { return time_gpu; }

	//! Returns the GPU begin drawing microseconds timestamp from the backend statistics.
	uint64 GetRendererGpuBeginDrawingMicroseconds() const { return stats_backend.gpuBeginDrawingMicroSec; }

	//! Returns the GPU early Z microseconds spent in the renderer.
	uint64 GetRendererGpuEarlyZMicroseconds() const { return stats_backend.gpuDepthMicroSec; }

	//! Returns the GPU geometry processing time in microseconds.
	uint64 GetRendererGpuGeometryMicroseconds() const { return stats_backend.gpuGeometryMicroSec; }

	//! Returns the GPU time spent on screen space ambient occlusion in microseconds.
	uint64 GetRendererGpuSSAOMicroseconds() const { return stats_backend.gpuScreenSpaceAmbientOcclusionMicroSec; }

	//! Returns the GPU screen space reflections time in microseconds.
	uint64 GetRendererGpuSSRMicroseconds() const { return stats_backend.gpuScreenSpaceReflectionsMicroSec; }

	//! Returns the GPU ambient pass execution time in microseconds.
	uint64 GetRendererGpuAmbientPassMicroseconds() const { return stats_backend.gpuAmbientPassMicroSec; }

	//! Returns the GPU shadow atlas pass execution time in microseconds.
	uint64 GetRendererGpuShadowAtlasPassMicroseconds() const { return stats_backend.gpuShadowAtlasPassMicroSec; }

	//! Returns the GPU interaction time in microseconds.
	uint64 GetRendererGpuInteractionsMicroseconds() const { return stats_backend.gpuInteractionsMicroSec; }

	//! Returns the GPU shader pass execution time in microseconds.
	uint64 GetRendererGpuShaderPassMicroseconds() const { return stats_backend.gpuShaderPassMicroSec; }

	//! Returns the GPU fog all lights microseconds statistic.
	uint64 GetRendererGpuFogAllLightsMicroseconds() const { return stats_backend.gpuFogAllLightsMicroSec; }

	//! Returns the GPU bloom time in microseconds.
	uint64 GetRendererGpuBloomMicroseconds() const { return stats_backend.gpuBloomMicroSec; }

	//! Returns the GPU shader pass post microseconds spent in the renderer.
	uint64 GetRendererGpuShaderPassPostMicroseconds() const { return stats_backend.gpuShaderPassPostMicroSec; }

	//! Returns the GPU motion vectors rendering time in microseconds.
	uint64 GetRendererGpuMotionVectorsMicroseconds() const { return stats_backend.gpuMotionVectorsMicroSec; }

	//! Returns the GPU temporal anti-aliasing time in microseconds.
	uint64 GetRendererGpuTAAMicroseconds() const { return stats_backend.gpuTemporalAntiAliasingMicroSec; }

	//! Returns the GPU tone mapping pass execution time in microseconds.
	uint64 GetRendererGpuToneMapPassMicroseconds() const { return stats_backend.gpuToneMapPassMicroSec; }

	//! Returns the GPU post-processing time in microseconds.
	uint64 GetRendererGpuPostProcessingMicroseconds() const { return stats_backend.gpuPostProcessingMicroSec; }

	//! Returns the GPU draw GUI microseconds statistic from the renderer backend.
	uint64 GetRendererGpuDrawGuiMicroseconds() const { return stats_backend.gpuDrawGuiMicroSec; }

	//! Returns the GPU CRT post-processing time in microseconds.
	uint64 GetRendererGpuCrtPostProcessingMicroseconds() const { return stats_backend.gpuCrtPostProcessingMicroSec; }

	//! Sets the Metal encoder microseconds value for the renderer.
	void   SetRendererMvkEncodeMicroseconds( uint64 mvkEncodeMicroSeconds )
	{
		metal_encode = mvkEncodeMicroSeconds;
		return;
	}

	//! Returns the accumulated microseconds spent on Metal encoder operations.
	uint64 GetRendererMvkEncodeMicroseconds() const { return metal_encode; }

	//! Sets the GPU memory value for the renderer.
	void   SetRendererGpuMemoryMB( uint64 gpuMemoryMB )
	{
		gpu_memory = gpuMemoryMB;
		return;
	}

	//! Returns the amount of GPU memory used by the renderer in megabytes.
	uint64		  GetRendererGpuMemoryMB() const { return gpu_memory; }

	//! Sets the load pacifier status message using a format string and variable arguments.
	virtual void  LoadPacifierInfo( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Initializes the pacifier progress tracking with the specified total count.
	virtual void  LoadPacifierProgressTotal( int total );

	//! Increments the load pacifier progress by the specified step amount
	virtual void  LoadPacifierProgressIncrement( int step );

	//! Returns true if a pacifier progress indicator is currently active.
	virtual bool  LoadPacifierRunning();

	//! Initializes the pacifier display for a binarization operation with the specified filename and reason.
	virtual void  LoadPacifierBinarizeFilename( const char* filename, const char* reason );

	//! Sets the pacifier binarize information string
	virtual void  LoadPacifierBinarizeInfo( const char* info );

	//! Sets the current and total miplevel values for the pacifier binarization process.
	virtual void  LoadPacifierBinarizeMiplevel( int level, int maxLevel );

	//! Updates the binarization progress pacifier with the given progress value
	virtual void  LoadPacifierBinarizeProgress( float progress );

	//! Ends the binarize operation pacifier display
	virtual void  LoadPacifierBinarizeEnd();

	//! Sets the total progress value for the pacifier binarization process
	virtual void  LoadPacifierBinarizeProgressTotal( int total );

	//! Increments the binarize progress counter by the specified step value.
	virtual void  LoadPacifierBinarizeProgressIncrement( int step );

	virtual void  RogmapPacifierFilename( const char* filename, const char* reason ) {};
	virtual void  RogmapPacifierInfo( VERIFY_FORMAT_STRING const char* fmt, ... ) {};
	virtual void  RogmapPacifierCompileProgressTotal( int total ) {};
	virtual void  RogmapPacifierCompileProgressIncrement( int step ) {};

	frameTiming_t frameTiming;
	frameTiming_t mainFrameTiming;

public: // These are public because they are called directly by static functions in this file
		//! Returns the name of the currently loaded map.
	const char*	  GetCurrentMapName() { return currentMapName.c_str(); }

	//! Loads a map and starts a new game on it
	void		  StartNewGame( const char* mapName, bool devmap, int gameMode );

	//! Exits the current game session and returns to the main menu.
	void		  LeaveGame();

	//! Initializes the language dictionary by loading relevant language files
	void		  InitLanguageDict();

	//! Processes a GUI file to localize its text strings using the provided language dictionary.
	void		  LocalizeGui( const char* fileName, idLangDict& langDict );

	//! Processes map data localization using the provided language dictionary and file name.
	void		  LocalizeMapData( const char* fileName, idLangDict& langDict );

	//! Localizes specific map data by replacing strings in entity key-value pairs with localized versions from a language dictionary.
	void		  LocalizeSpecificMapData( const char* fileName, idLangDict& langDict, const idLangDict& replaceArgs );

	//! Returns a reference to the user command manager.
	idUserCmdMgr& GetUCmdMgr() { return userCmdMgr; }

private:
	bool		com_fullyInitialized;
	bool		com_refreshOnPrint; // update the screen every print for dmap
	errorParm_t com_errorEntered;
	bool		com_shuttingDown;
	bool		com_isJapaneseSKU;

	idFile*		logFile;

	char		errorMessage[MAX_PRINT_MSG_SIZE];

	char*		rd_buffer;
	int			rd_buffersize;
	void ( *rd_flush )( const char* buffer );

	idStr					  warningCaption;
	idStrList				  warningList;
	idStrList				  errorList;

	int						  gameDLL;

	idCommonDialog			  commonDialog;

	idFile_SaveGame			  saveFile;
	idFile_SaveGame			  stringsFile;
	idFile_SaveGamePipelined* pipelineFile;

	// The main render world and sound world
	idRenderWorld*			  renderWorld;
	idSoundWorld*			  soundWorld;

	bool					  menuActive;
	idSoundWorld*			  menuSoundWorld; // so the game soundWorld can be muted

	bool					  insideExecuteMapChange; // Enable Pacifier Updates

	// This is set if the player enables the console, which disables achievements
	bool					  consoleUsed;

	// This additional information is required for ExecuteMapChange for SP games ONLY
	// This data is cleared after ExecuteMapChange
	struct mapSpawnData_t {
		idFile_SaveGame*		  savegameFile;	   // Used for loading a save game
		idFile_SaveGame*		  stringTableFile; // String table read from save game loaded
		idFile_SaveGamePipelined* pipelineFile;
		int						  savegameVersion;		// Version of the save game we're loading
		idDict					  persistentPlayerInfo; // Used for transitioning from map to map
	};
	mapSpawnData_t mapSpawnData;
	idStr		   currentMapName; // for checking reload on same level
	bool		   mapSpawned;	   // cleared on Stop()

	bool		   insideUpdateScreen; // true while inside ::UpdateScreen()

	idUserCmdMgr   userCmdMgr;

	int			   nextUsercmdSendTime;	 // Next time to send usercmds
	int			   nextSnapshotSendTime; // Next time to send a snapshot

	idSnapShot	   lastSnapShot; // last snapshot we received from the server
	struct reliableMsg_t {
		int	  client;
		int	  type;
		int	  dataSize;
		byte* data;
	};
	idList<reliableMsg_t>							  reliableQueue;

	// Snapshot interpolation
	idSnapShot										  oldss; // last local snapshot
	// (ie on server this is the last "master" snapshot  we created)
	// (on clients this is the last received snapshot)
	// used for comparisons with the new snapshot for com_drawSnapshot

	// This is ultimately controlled by net_maxBufferedSnapshots by running double speed, but this is the hard max before seeing visual popping
	static const int								  RECEIVE_SNAPSHOT_BUFFER_SIZE = 16;

	int												  readSnapshotIndex;
	int												  writeSnapshotIndex;
	idArray<idSnapShot, RECEIVE_SNAPSHOT_BUFFER_SIZE> receivedSnaps;

	float											  optimalPCTBuffer;
	float											  optimalTimeBuffered;
	float											  optimalTimeBufferedWindow;

	uint64											  snapRate;
	uint64											  actualRate;

	uint64											  snapTime;		 // time we got the most recent snapshot
	uint64											  snapTimeDelta; // time interval that current ss was sent in

	uint64											  snapTimeWrite;
	uint64											  snapCurrentTime; // realtime playback time
	netTimes_t										  snapCurrent;	   // current snapshot
	netTimes_t										  snapPrevious;	   // previous snapshot
	float											  snapCurrentResidual;

	float											  snapTimeBuffered;
	float											  effectiveSnapRate;
	int												  totalBufferedTime;
	int												  totalRecvTime;

	int												  clientPrediction;

	int												  gameFrame;		// Frame number of the local game
	double											  gameTimeResidual; // left over msec from the last game frame
	bool											  syncNextGameFrame;

	enum timeDemo_t { TD_NO, TD_YES, TD_YES_THEN_QUIT };
	timeDemo_t						  timeDemo;
	int								  timeDemoStartTime;
	int								  numDemoFrames; // for timeDemo and demoShot
	int								  numShotFrames; // SRS - for demoShot playback timeout
	int								  demoTimeOffset;
	renderView_t					  currentDemoRenderView;

	idStrList						  mpGameModes;
	idStrList						  mpDisplayGameModes;
	idList<mpMap_t>					  mpGameMaps;

	idSWF*							  loadGUI;
	int								  nextLoadTip;
	bool							  isHellMap;
	bool							  defaultLoadscreen;
	idStaticList<int, LOAD_TIP_COUNT> loadTipList;

	const idMaterial*				  splashScreen;

	const idMaterial*				  whiteMaterial;

	const idMaterial*				  wipeMaterial;
	int								  wipeStartTime;
	int								  wipeStopTime;
	bool							  wipeHold;
	bool							  wipeForced; // used for the PS3 to start an early wipe while we are accessing saved game data

	idGameThread					  gameThread; // the game and draw code can be run in parallel

	// com_speeds times
	int								  count_numGameFrames; // total number of game frames that were run
	int								  time_gameFrame;	   // game logic time
	int								  time_maxGameFrame;   // maximum single frame game logic time
	int								  time_gameDraw;	   // game present time
	uint64							  time_frontend;	   // renderer frontend time
	uint64							  time_backend;		   // renderer backend time
	uint64							  time_moc;			   // renderer frontend masked software rasterization time
	uint64							  time_gpu;			   // total gpu time, at least for PC

	// RB: r_speeds counters
	backEndCounters_t				  stats_backend;
	performanceCounters_t			  stats_frontend;

	// SRS - MoltenVK's Vulkan to Metal command buffer encoding time, set default to 0 for non-macOS platforms (Windows and Linux)
	uint64							  metal_encode = 0;
	// SRS - Cross-platform GPU Memory usage counter, set default to 0 in case platform or graphics API does not support queries
	uint64							  gpu_memory = 0;

	// Used during loading screens
	int								  lastPacifierSessionTime;
	int								  lastPacifierGuiTime;
	bool							  lastPacifierDialogState;

	// RB begin
	idStrStatic<256>				  loadPacifierStatus		= "-";
	int								  loadPacifierCount			= 0;
	int								  loadPacifierExpectedCount = 0;
	size_t							  loadPacifierTics			= 0;
	size_t							  loadPacifierNextTicCount	= 0;
	// RB end

	// foresthale 2014-05-30: a special binarize pacifier has to be shown in some cases, which includes filename and ETA information
	bool							  loadPacifierBinarizeActive	= false;
	int								  loadPacifierBinarizeStartTime = 0;
	float							  loadPacifierBinarizeProgress	= 0.0f;
	float							  loadPacifierBinarizeTimeLeft	= 0.0f;
	idStr							  loadPacifierBinarizeFilename;
	idStr							  loadPacifierBinarizeInfo;
	int								  loadPacifierBinarizeMiplevel		  = 0;
	int								  loadPacifierBinarizeMiplevelTotal	  = 0;
	int								  loadPacifierBinarizeProgressTotal	  = 0;
	int								  loadPacifierBinarizeProgressCurrent = 0;

	bool							  showShellRequested;

private:
	//! Initializes and registers command-line commands for map compilation and AAS file processing tools.
	void InitCommands();

	//! Initializes the SIMD processor for the doom application.
	void InitSIMD();

	//! Adds startup commands to the command system by buffering tokenized console lines.
	void AddStartupCommands();

	//! Parses command line arguments and processes console commands for the common local functionality.
	void ParseCommandLine( int argc, const char* const* argv );

	//! Checks if the command line contains safe or cvar_restart arguments to skip loading the config file.
	bool SafeMode();

	//! Closes the current log file if one is open.
	void CloseLogFile();

	//! Writes archived console variables and key bindings to the configuration file if they have been modified
	void WriteConfiguration();

	//! Writes accumulated warnings and errors to a file and optionally opens it in Notepad on Windows.
	void DumpWarnings();

	//! Loads the game dynamic library and initializes the game object.
	void LoadGameDLL();

	//! Unloads the game DLL and shuts down the game object if loaded.
	void UnloadGameDLL();

	//! Cleans up the game shell by calling the game's cleanup function if the game pointer is valid.
	void CleanupShell();

	//! Renders a Bink video file at the specified path
	void RenderBink( const char* path );

	//! Renders the splash screen with proper aspect ratio handling and black bars if needed.
	void RenderSplash();

	//! Filters a list of strings to retain only those that match the specified language prefix.
	void FilterLangList( idStrList* list, idStr lang );

	//! Checks if there is sufficient storage space available for save games and profiles during startup
	void CheckStartupStorageRequirements();

	//! Exits the menu by hiding the game shell.
	void ExitMenu();

	//! Processes input events for menu and GUI handling
	bool MenuEvent( const sysEvent_t* event );

	//! Initializes and displays the main menu interface.
	void StartMenu( bool playIntro = false );

	//! Synchronizes the game session with the shell for GUI frame events.
	void GuiFrameEvents();

	//! Processes game return values including vibration feedback and session commands
	void ProcessGameReturn( const gameReturn_t& ret );

	//! Processes network snapshot frames and handles game timing for client interpolation.
	void RunNetworkSnapshotFrame();

	//! Processes all reliable messages that have been received and clears the queue afterwards.
	void ExecuteReliableMessages();

	//! Processes a snapshot received from the server for synchronization and gameplay state updates.
	void ProcessSnapshot( idSnapShot& ss );

	//! Calculates and returns the amount of game time left in buffered snapshots after accounting for interpolation.
	int	 CalcSnapTimeBuffered( int& totalBufferedTime, int& totalRecvTime );

	//! Processes the next available snapshot in the receive buffer.
	void ProcessNextSnapshot();

	/*!
		\brief Interpolates network snapshot data between two time points based on a fraction value

		This function performs interpolation of network time data between two snapshots represented by prev and next parameters. It calculates an interpolated server time value using the provided
	   fraction and updates the global server time accordingly. The function also sets up interpolation parameters for the game engine to maintain smooth networked gameplay. The predict parameter may
	   influence how the interpolation is applied in relation to prediction logic

		\param prev Previous network snapshot time data
		\param next Next network snapshot time data
		\param fraction Interpolation fraction between 0 and 1
		\param predict Flag indicating if prediction logic should be applied
	*/
	void InterpolateSnapshot( netTimes_t& prev, netTimes_t& next, float fraction, bool predict );

	//! Resets all networking state variables and clears snapshot buffers.
	void ResetNetworkingState();

	int	 NetworkFrame();

	//! Sends game state snapshots to connected peers if conditions are met.
	void SendSnapshots();

	//! Sends user commands for the specified local client to the game session.
	void SendUsercmds( int localClientNum );

	//! Loads and displays the appropriate loading GUI for the specified map, handling demo mode andHellMap states.
	void LoadLoadingGui( const char* mapName, bool& hellMap );

	//! Provides a busy-wait loop implementation that processes events and updates the screen during waiting periods.
	void BusyWait();

	//! Waits for the session to reach the specified desired state
	bool WaitForSessionState( idSession::sessionState_t desiredState );

	//! Executes a map change using session match parameters for game initialization
	void ExecuteMapChange();

	//! Unloads the current map and performs necessary cleanup.
	void UnloadMap();

	//! Stops the current session and cleans up resources, optionally resetting the session state.
	void Stop( bool resetSession = true );

	//! Draws the wipe model during scene transitions.
	void DrawWipeModel();

	//! Draws a progress bar for the loading pacifier.
	void DrawLoadPacifierProgressbar();

	//! Starts a screen wipe effect using the specified material.
	void StartWipe( const char* materialName, bool hold = false );

	//! Waits until the wipe effect is complete and ensures it is fully faded out.
	void CompleteWipe();

	//! Resets all wipe-related state flags and timers to their default values.
	void ClearWipe();

	//! Transitions to a new map in single player mode.
	void MoveToNewMap( const char* mapName, bool devmap );

	//! Replaces invalid characters in a save game file name with underscores.
	void ScrubSaveGameFileName( idStr& saveFileName ) const;
};

extern idCommonLocal commonLocal;
