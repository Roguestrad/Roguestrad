/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2021 Justin Marshall

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

#ifndef __GAME_LOCAL_H__
#define __GAME_LOCAL_H__

/*
===============================================================================

	Local implementation of the public game interface.

===============================================================================
*/

#ifdef ID_DEBUG_UNINITIALIZED_MEMORY
	// This is real evil but allows the code to inspect arbitrary class variables.
	#define private	  public
	#define protected public
#endif

extern idRenderWorld* gameRenderWorld;
extern idSoundWorld*  gameSoundWorld;

// the "gameversion" client command will print this plus compile date
#define GAME_VERSION   "baseDOOM-1"

#define GAME_FRAMETIME 0.016 // 16 milliseconds
#define USERCMD_HZ	   90

// classes used by idGameLocal
class idEntity;
class idActor;
class idPlayer;
class idCamera;
class idWorldspawn;
class idTestModel;
class idAAS;
class idAI;
class idSmokeParticles;
class idEntityFx;
class idTypeInfo;
class idProgram;
class idThread;
class idEditEntities;
class idLocationEntity;
class idMenuHandler_Shell;
class EnvironmentProbe; // RB
class iceBot;

extern idCVar		  r_singleLight;
extern idCVar		  r_showLights;

const int			  MAX_CLIENTS					 = MAX_PLAYERS;
const int			  MAX_CLIENTS_IN_PVS			 = MAX_CLIENTS >> 3;
const int			  GENTITYNUM_BITS				 = 12;
const int			  MAX_GENTITIES					 = 1 << GENTITYNUM_BITS;
const int			  ENTITYNUM_NONE				 = MAX_GENTITIES - 1;
const int			  ENTITYNUM_WORLD				 = MAX_GENTITIES - 2;
const int			  ENTITYNUM_MAX_NORMAL			 = MAX_GENTITIES - 2;
const int			  ENTITYNUM_FIRST_NON_REPLICATED = ENTITYNUM_MAX_NORMAL - 256;

// Admer: brush origin offsets for better TrenchBroom support
constexpr const char* BRUSH_ORIGIN_KEY = "__brushOrigin";

//! Reports a game error with formatted message
void				  gameError( const char* fmt, ... );

#include "gamesys/Event.h"
#include "gamesys/State.h"
#include "gamesys/Class.h"
#include "gamesys/SysCvar.h"
#include "gamesys/SysCmds.h"
#include "gamesys/SaveGame.h"

#include "script/Script_Program.h"

#include "anim/Anim.h"

#include "ai/AAS.h"

#include "physics/Clip.h"
#include "physics/Push.h"

#include "Pvs.h"
#include "Leaderboards.h"
#include "MultiplayerGame.h"

class idWeapon;

//============================================================================

const int MAX_GAME_MESSAGE_SIZE	 = 8192;
const int MAX_ENTITY_STATE_SIZE	 = 512;
const int ENTITY_PVS_SIZE		 = ( ( MAX_GENTITIES + 31 ) >> 5 );
const int NUM_RENDER_PORTAL_BITS = idMath::BitsForInteger( PS_BLOCK_ALL );

const int MAX_EVENT_PARAM_SIZE = 128;

typedef struct entityNetEvent_s {
	int						 spawnId;
	int						 event;
	int						 time;
	int						 paramsSize;
	byte					 paramsBuf[MAX_EVENT_PARAM_SIZE];
	struct entityNetEvent_s* next;
	struct entityNetEvent_s* prev;
} entityNetEvent_t;

enum {
	GAME_RELIABLE_MESSAGE_SYNCEDCVARS,
	GAME_RELIABLE_MESSAGE_SPAWN_PLAYER,
	GAME_RELIABLE_MESSAGE_CHAT,
	GAME_RELIABLE_MESSAGE_TCHAT,
	GAME_RELIABLE_MESSAGE_SOUND_EVENT,
	GAME_RELIABLE_MESSAGE_SOUND_INDEX,
	GAME_RELIABLE_MESSAGE_DB,
	GAME_RELIABLE_MESSAGE_DROPWEAPON,
	GAME_RELIABLE_MESSAGE_RESTART,
	GAME_RELIABLE_MESSAGE_TOURNEYLINE,
	GAME_RELIABLE_MESSAGE_VCHAT,
	GAME_RELIABLE_MESSAGE_STARTSTATE,
	GAME_RELIABLE_MESSAGE_WARMUPTIME,
	GAME_RELIABLE_MESSAGE_SPECTATE,
	GAME_RELIABLE_MESSAGE_EVENT,
	GAME_RELIABLE_MESSAGE_LOBBY_COUNTDOWN,
	GAME_RELIABLE_MESSAGE_RESPAWN_AVAILABLE, // Used just to show clients the respawn text on the hud.
	GAME_RELIABLE_MESSAGE_MATCH_STARTED_TIME,
	GAME_RELIABLE_MESSAGE_ACHIEVEMENT_UNLOCK,
	GAME_RELIABLE_MESSAGE_CLIENT_HITSCAN_HIT
};

typedef enum {
	GAMESTATE_UNINITIALIZED, // prior to Init being called
	GAMESTATE_NOMAP,		 // no map loaded
	GAMESTATE_STARTUP,		 // inside InitFromNewMap().  spawning map entities.
	GAMESTATE_ACTIVE,		 // normal gameplay
	GAMESTATE_SHUTDOWN		 // inside MapShutdown().  clearing memory.
} gameState_t;

typedef struct {
	idEntity* ent;
	int		  dist;
	int		  team;
} spawnSpot_t;

/*!
	\class idEventQueue
	\brief Manages a queue of entity network events for processing.

	The idEventQueue class provides a mechanism for storing, managing, and retrieving entity network events in a queue-based structure. It supports allocation and deallocation of events through an
   internal allocator, and offers methods to enqueue events with specific out-of-order behavior handling, as well as dequeue events from the front or back of the queue. The queue is initialized and
   shut down to manage its lifecycle, ensuring proper memory management and state reinitialization. This class is designed to handle event processing in a structured manner, supporting both ordered
   and out-of-order event handling.

*/
class idEventQueue
{
public:
	typedef enum { OUTOFORDER_IGNORE, OUTOFORDER_DROP, OUTOFORDER_SORT } outOfOrderBehaviour_t;

	//! Initializes an empty event queue with null start and end pointers.
	idEventQueue() :
		start( NULL ),
		end( NULL )
	{
	}

	//! Allocates and initializes a new entity network event from the event allocator.
	entityNetEvent_t* Alloc();

	//! Frees an unlinked entity network event by returning it to the event allocator.
	void			  Free( entityNetEvent_t* event );

	//! Shuts down the event queue by deallocating allocated memory and reinitializing the queue.
	void			  Shutdown();

	//! Initializes the event queue by setting the start and end pointers to NULL.
	void			  Init();

	//! Enqueues a network event into the event queue with specified out-of-order behavior handling.
	void			  Enqueue( entityNetEvent_t* event, outOfOrderBehaviour_t oooBehaviour );

	//! Removes and returns the first event from the queue.
	entityNetEvent_t* Dequeue();

	//! Removes and returns the last event from the event queue.
	entityNetEvent_t* RemoveLast();

	//! Returns the start pointer of the event queue.
	entityNetEvent_t* Start() { return start; }

private:
	entityNetEvent_t*				   start;
	entityNetEvent_t*				   end;
	idBlockAlloc<entityNetEvent_t, 32> eventAllocator;
};

/*!
	\class idEntityPtr
	\brief A template class that provides a safe way to reference entities by spawn ID.

	The idEntityPtr template class encapsulates a reference to an entity using its spawn ID rather than a direct pointer. This design allows the class to function as a proxy that can be safely saved
   and restored during game state serialization. The class maintains a spawn ID and can resolve it to an actual entity pointer when needed, checking validity before returning the reference. The
   UpdateSpawnId method appears to refresh the internal spawn ID, likely to handle cases where the entity has been respawned or moved. This approach prevents dangling pointer issues while maintaining
   the ability to access entity data through a consistent interface.

*/
template<class type>
class idEntityPtr
{
public:
	//! Initializes an idEntityPtr with a null spawn ID.
	idEntityPtr();

	//! Saves the entity pointer's spawn ID to the save game file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the entity pointer state from a save game file.
	void		 Restore( idRestoreGame* savefile );

	//! Assigns an entity pointer to this entity pointer wrapper.
	idEntityPtr& operator=( const type* ent );

	//! Assigns the spawn ID from another entity pointer to this entity pointer
	idEntityPtr& operator=( const idEntityPtr& ep );

	//! Compares two idEntityPtr objects for equality based on their spawn IDs.
	bool		 operator==( const idEntityPtr& ep ) { return spawnId == ep.spawnId; }

	//! Returns the managed entity pointer for member access.
	type*		 operator->() const { return GetEntity(); }

	//! Converts the entity pointer to a raw pointer of the specified type.
	operator type*() const { return GetEntity(); }

	//! Returns the spawn ID of the entity pointer.
	int	  GetSpawnId() const { return spawnId; }

	//! Sets the spawn ID for the entity pointer and returns true if successful.
	bool  SetSpawnId( int id );
	bool  UpdateSpawnId();

	//! Checks if the entity pointer is still valid and refers to a currently spawned entity
	bool  IsValid() const;

	//! Returns the entity that this pointer references, or NULL if the entity is no longer valid.
	type* GetEntity() const;

	//! Returns the entity number from the spawn ID.
	int	  GetEntityNum() const;

private:
	int spawnId;
};

struct timeState_t {
	int	 time;
	int	 previousTime;
	int	 realClientTime;

	void Set( int t, int pt, int rct )
	{
		time		   = t;
		previousTime   = pt;
		realClientTime = rct;
	};
	void Get( int& t, int& pt, int& rct )
	{
		t	= time;
		pt	= previousTime;
		rct = realClientTime;
	};

	//! Serializes the time state information into the provided save file.
	void Save( idSaveGame* savefile ) const
	{
		savefile->WriteInt( time );
		savefile->WriteInt( previousTime );
		savefile->WriteInt( realClientTime );
	}

	//! Restores the time state from a save file.
	void Restore( idRestoreGame* savefile )
	{
		savefile->ReadInt( time );
		savefile->ReadInt( previousTime );
		savefile->ReadInt( realClientTime );
	}
};

enum slowmoState_t { SLOWMO_STATE_OFF, SLOWMO_STATE_RAMPUP, SLOWMO_STATE_ON, SLOWMO_STATE_RAMPDOWN };

struct iceGameDelayRemoveEntry_t {
	int32_t	  removeTime;
	idEntity* entity;
};

/*!
	\class idGameLocal
	\brief Core game state manager handling map initialization, entity management, and game loop execution.

	Manages the primary game state including map loading, entity spawning, and game loop execution. Provides interfaces for player management, AI systems, and network synchronization. Handles both
   single-player and multiplayer game modes with support for save games, time group management, and various game system initialization tasks. The class serves as the central coordination point for all
   game-related operations while maintaining awareness of player states, entity relationships, and network communication.

*/
class idGameLocal : public idGame
{
public:
	int									previousServerTime; // time in msec of last frame on the server
	int									serverTime;			// in msec. ( on the client ) the server time. ( on the server ) the actual game time.
	idDict								serverInfo;			// all the tunable parameters, like numclients, etc
	int									numClients;			// pulled from serverInfo and verified
	idArray<lobbyUserID_t, MAX_CLIENTS> lobbyUserIDs;		// Maps from a client (player) number to a lobby user
	idDict								persistentPlayerInfo[MAX_CLIENTS];
	idEntity*							entities[MAX_GENTITIES]; // index to entities
	int									spawnIds[MAX_GENTITIES]; // for use in idEntityPtr
	idArray<int, 2>						firstFreeEntityIndex;	 // first free index in the entities array. [0] for replicated entities, [1] for non-replicated
	int									num_entities;			 // current number <= MAX_GENTITIES
	idHashIndex							entityHash;				 // hash table to quickly find entities by name
	idWorldspawn*						world;					 // world entity
	idLinkList<idEntity>				spawnedEntities;		 // all spawned entities
	idLinkList<idEntity>				activeEntities;			 // all thinking entities (idEntity::thinkFlags != 0)
	idLinkList<idEntity>				aimAssistEntities;		 // all aim Assist entities
	int									numEntitiesToDeactivate; // number of entities that became inactive in current frame
	bool								sortPushers;			 // true if active lists needs to be reordered to place pushers at the front
	bool								sortTeamMasters;		 // true if active lists needs to be reordered to place physics team masters before their slaves
	idDict								persistentLevelInfo;	 // contains args that are kept around between levels

	// can be used to automatically effect every material in the world that references globalParms
	float								globalShaderParms[MAX_GLOBAL_SHADER_PARMS];

	idRandom							random; // random number generator used throughout the game

	idProgram							program; // currently loaded script and data space
	idThread*							frameCommandThread;

	idClip								clip; // collision detection
	idPush								push; // geometric pushing
	idPVS								pvs;  // potential visible set

	idTestModel*						testmodel; // for development testing of models
	idEntityFx*							testFx;	   // for development testing of fx

	idStr								sessionCommand; // a target_sessionCommand can set this to return something to the session

	idMultiplayerGame					mpGame; // handles rules for standard dm

	idSmokeParticles*					smokeParticles; // global smoke trails
	idEditEntities*						editEntities;	// in game editing

	int									cinematicSkipTime;	  // don't allow skipping cinemetics until this time has passed so player doesn't skip out accidently from a firefight
	int									cinematicStopTime;	  // cinematics have several camera changes, so keep track of when we stop them so that we don't reset cinematicSkipTime unnecessarily
	int									cinematicMaxSkipTime; // time to end cinematic when skipping.  there's a possibility of an infinite loop if the map isn't set up right.
	bool								inCinematic;		  // game is playing cinematic (player controls frozen)
	bool								skipCinematic;

	int									framenum;
	int									time;		  // in msec
	int									previousTime; // time in msec of last frame

	int									vacuumAreaNum; // -1 if level doesn't have any outside areas

	gameType_t							gameType;
	idLinkList<idEntity>				snapshotEntities; // entities from the last snapshot
	int									realClientTime;	  // real client time
	bool								isNewFrame;		  // true if this is a new game frame, not a rerun due to prediction
	float								clientSmoothing;  // smoothing of other clients in the view
	int									entityDefBits;	  // bits required to store an entity def number

	static const char*					sufaceTypeNames[MAX_SURFACE_TYPES]; // text names for surface types

	idEntityPtr<idEntity>				lastGUIEnt; // last entity with a GUI, used by Cmd_NextGUI_f
	int									lastGUI;	// last GUI on the lastGUIEnt

	idEntityPtr<idPlayer>				playerActivateFragChamber; // The player that activated the frag chamber

	idEntityPtr<idEntity>				portalSkyEnt;
	bool								portalSkyActive;

	//! Sets the portal sky entity for the game local instance.
	void								SetPortalSkyEnt( idEntity* ent );

	//! Returns true if the portal sky is active.
	bool								IsPortalSkyAcive();

	timeState_t							fast;
	timeState_t							slow;
	int									selectedGroup;

	slowmoState_t						slowmoState;
	float								slowmoScale;

	bool								quickSlowmoReset;

	//! Selects a time group for frame timing synchronization.
	virtual void						SelectTimeGroup( int timeGroup );

	//! Returns the time value from either the fast or slow time group based on the provided time group identifier.
	virtual int							GetTimeGroupTime( int timeGroup );

	//! Computes the slow motion scale and updates the slow motion state based on player power-up status and game configuration.
	void								ComputeSlowScale();

	//! Executes the think function for all entities in the TIME_GROUP2 time group.
	void								RunTimeGroup2( idUserCmdMgr& userCmdMgr );

	//! Resets the slow motion time variables to their default states.
	void								ResetSlowTimeVars();

	//! Resets the quick slow motion state.
	void								QuickSlowmoReset();

	//! Splits a string into tokens separated by semicolons and converts them to lowercase.
	void								Tokenize( idStrList& out, const char* in );

	//! Initializes a new instance of the idGameLocal class.
	idGameLocal();

	//! Initializes the game object, typically called once at startup.
	virtual void		   Init();

	//! Shuts down the entire game engine and releases all allocated resources.
	virtual void		   Shutdown();

	//! Sets the server information dictionary and adjusts the frag limit for Last Man Standing game type if necessary.
	virtual void		   SetServerInfo( const idDict& serverInfo );

	//! Returns the server information dictionary.
	virtual const idDict&  GetServerInfo();

	//! Retrieves the persistent player information for the specified client number.
	virtual const idDict&  GetPersistentPlayerInfo( int clientNum );

	//! Stores the player information for a specific client number in the persistent player info map.
	virtual void		   SetPersistentPlayerInfo( int clientNum, const idDict& playerInfo );

	/*!
		\brief Initializes the game local state from a new map.

		Sets up the game state for a new map by initializing the game type, shutting down the previous map if necessary, loading the new map, and performing various initialization tasks such as script
	   initialization, map population, and environment probe population. It also handles multiplayer-specific settings and resets the multi-player game state.

		\param mapName Name of the map to initialize
		\param renderWorld Render world object for the new map
		\param soundWorld Sound world object for the new map
		\param gameType Type of game to initialize
		\param randSeed Random seed for map initialization
	*/
	virtual void		   InitFromNewMap( const char* mapName, idRenderWorld* renderWorld, idSoundWorld* soundWorld, int gameType, int randSeed );

	/*!
		\brief Initializes the game state from a saved game file, loading map, entities, and game data.

		This function restores the game state from a savegame file, including map data, entities, player information, and various game systems. It handles the loading of the map specified by mapName,
	   reads entity data from the savegame file, and reconstructs the game world. The function also restores scripting state, media precaching, and various game settings. It returns false if the
	   savegame format is incompatible or if the loading process fails, requiring a restart of the level with persistent player data. The function also handles connecting players with lobby users and
	   restores various game state parameters such as time, gravity, and AI information.

		\param mapName The name of the map to load for this savegame
		\param renderWorld Pointer to the render world to use for rendering
		\param soundWorld Pointer to the sound world to use for sound processing
		\param saveGameFile File pointer to the savegame file to read from
		\param stringTableFile File pointer to the string table file for localization
		\param saveGameVersion Version number of the savegame format to be loaded
		\return True if the savegame was successfully loaded and initialized, false if loading failed due to version incompatibility or other errors
		\throws May throw an error during savegame reading if there's a mismatch between map areas or if assertions fail
	*/
	virtual bool		   InitFromSaveGame( const char* mapName, idRenderWorld* renderWorld, idSoundWorld* soundWorld, idFile* saveGameFile, idFile* stringTableFile, int saveGameVersion );

	//! Saves the current game state to the specified files
	virtual void		   SaveGame( idFile* saveGameFile, idFile* stringTableFile );

	//! Populates the provided save game details structure with information about the current game state
	virtual void		   GetSaveGameDetails( idSaveGameDetails& gameDetails );

	//! Performs cleanup operations when shutting down the current game map.
	virtual void		   MapShutdown();

	//! Caches media resources referenced in a dictionary for use in the game.
	virtual void		   CacheDictionaryMedia( const idDict* dict );

	//! Preloads resources specified in the manifest for the game level.
	virtual void		   Preload( const idPreloadManifest& manifest );

	//! Executes a single game frame, processing entity thinks, events, and multiplayer logic.
	virtual void		   RunFrame( idUserCmdMgr& cmdMgr, gameReturn_t& gameReturn );

	//! Processes all user commands for a specified player, handling both local and remote command execution with appropriate timing and buffering.
	void				   RunAllUserCmdsForPlayer( idUserCmdMgr& cmdMgr, const int playerNumber );

	//! Executes a single user command for a player, handling think logic and updating the command with player state
	void				   RunSingleUserCmd( usercmd_t& cmd, idPlayer& player );

	//! Executes the think function for an entity, handling player entities differently based on multiplayer or singleplayer mode.
	void				   RunEntityThink( idEntity& ent, idUserCmdMgr& userCmdMgr );

	//! Draws the game view for the specified client
	virtual bool		   Draw( int clientNum );

	//! Handles GUI events for the player and multiplayer game if not already handled.
	virtual bool		   HandlePlayerGuiEvent( const sysEvent_t* ev );

	//! Writes a snapshot of the current game state to the provided snapshot object
	virtual void		   ServerWriteSnapshot( idSnapShot& ss );

	//! Processes a reliable message based on whether the game session is acting as a peer or server.
	virtual void		   ProcessReliableMessage( int clientNum, int type, const idBitMsg& msg );

	//! Processes a snapshot received from the server to update client-side game state and entities.
	virtual void		   ClientReadSnapshot( const idSnapShot& ss );

	//! Executes a single game frame for client-side prediction and updates game state.
	virtual void		   ClientRunFrame( idUserCmdMgr& cmdMgr, bool lastPredictFrame, gameReturn_t& ret );

	//! Fills out gameReturn_t with vibration values and session command data
	void				   BuildReturnValue( gameReturn_t& ret );

	//! Returns the available multiplayer game modes.
	virtual int			   GetMPGameModes( const char*** gameModes, const char*** gameModesDisplay );

	//! Retrieves client statistics for the specified client number and stores them in the provided data buffer.
	virtual void		   GetClientStats( int clientNum, char* data, const int len );

	//! Returns true if the game is currently active and in progress.
	virtual bool		   IsInGame() const { return GameState() == GAMESTATE_ACTIVE; }

	//! Maps a peer index to a client index.
	virtual int			   MapPeerToClient( int peer ) const;

	//! Returns the local client number for the current game session.
	virtual int			   GetLocalClientNum() const;

	//! Sets the provided angles to the aim assist correction values from the local player's aim assist system.
	virtual void		   GetAimAssistAngles( idAngles& angles );

	//! Retrieves the aim assist sensitivity scalar value for the local player.
	virtual float		   GetAimAssistSensitivity();

	//! Prints a formatted message to the console
	void				   Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Outputs debug information to the console when the developer console is enabled
	void				   DPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Reports a formatted warning message to the console or current thread
	void				   Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Prints a warning message that only appears when the developer cvar is set
	void				   DWarning( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Reports an error message to the common error handler or current thread's error handler
	void				   Error( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Loads a map file and initializes all game entities and systems for the specified map.
	void				   LoadMap( const char* mapName, int randseed );

	//! Resets the game map and reinitializes all entities and systems for a new game session.
	void				   LocalMapRestart();

	//! Restarts the current game map
	void				   MapRestart();

	//! Restarts the current game map.
	static void			   MapRestart_f( const idCmdArgs& args );

	//! Retrieves or loads the map file for the current level.
	idMapFile*			   GetLevelMap();

	//! Returns the name of the currently loaded map.
	const char*			   GetMapName() const;

	//! Returns the current system script time in seconds.
	float				   SysScriptTime() const { return MS2SEC( realClientTime ); }

	//! Returns the time elapsed since the previous script frame in seconds.
	float				   SysScriptFrameTime() const { return MS2SEC( time - previousTime ); }

	//! Schedules an entity to be removed after a specified delay.
	void				   DelayRemoveEntity( idEntity* entity, int delay );

	//! Checks if influence is active for the local player.
	bool				   InfluenceActive() const;

	//! Retrieves an entity by its name or number from the game world
	idEntity*			   GetEntity( const char* name );

	//! Returns a random floating-point value between 0 and the specified range.
	float				   Random( float range );

	//! Returns a future timestamp by adding a random delay to the current script time.
	float				   RandomDelay( float min, float max );

	//! Returns a future time value by adding a random fraction of the specified delay to the current script time.
	float				   RandomTime( float delay );

	//! Returns a delayed time value based on the current script time and a random offset.
	float				   DelayTime( float delay );

	//! Spawns an entity of the specified class name and returns a pointer to the created entity.
	idEntity*			   Spawn( const char* classname );

	//! Adds a bot to the multiplayer game session.
	void				   AddBot( const char* name );

	//! Calculates the travel time between two points using the AAS navigation system.
	int					   TravelTimeToGoal( const idVec3& origin, const idVec3& goal );

	//! Returns the bot item entry index for the specified item name.
	int					   GetBotItemEntry( const char* name );

	/*!
		\brief Performs a trace operation from start to end position, storing the results in the provided trace structure

		This function executes a collision trace between two 3D points using the game's clipping system. It supports tracing against solid content and can optionally ignore a specific entity during
	   the trace. The trace result includes information about what was hit, the point of impact, and the surface normal. The function uses an identity axis for the trace operation and is typically
	   used for raycasting scenarios where collision detection is needed.

		\param results Output structure containing the trace results including hit information
		\param start Starting 3D position for the trace
		\param end Ending 3D position for the trace
		\param contentMask Bitmask specifying which content types to trace against
		\param passEntity Entity index to ignore during the trace, or -1 to trace against all entities
	*/
	void				   Trace( trace_t& results, const idVec3& start, const idVec3& end, int contentMask, int passEntity );

	//! Alerts all bots in the game to a player's position.
	void				   AlertBots( idPlayer* player, idVec3 alert_position );

	//! Returns the number of AAS (Area Awareness System) instances in the game local context.
	int					   NumAAS() const;

	//! Returns the AAS instance for the specified index if it exists and has settings, otherwise returns NULL.
	idAAS*				   GetAAS( int num ) const;

	//! Retrieves the AAS instance matching the specified name, or null if not found or invalid.
	idAAS*				   GetAAS( const char* name ) const;

	//! Sets the state of AAS areas within the specified bounds.
	void				   SetAASAreaState( const idBounds& bounds, const int areaContents, bool closed );

	//! Adds an AAS obstacle to all AAS instances within the game local context.
	aasHandle_t			   AddAASObstacle( const idBounds& bounds );

	//! Removes an AAS obstacle with the specified handle from all AAS instances.
	void				   RemoveAASObstacle( const aasHandle_t handle );

	//! Removes all AAS obstacles from the game local instance.
	void				   RemoveAllAASObstacles();

	//! Checks if cheats are allowed based on game mode and player status
	bool				   CheatsOk( bool requirePlayer = true );

	//! Returns the current game state.
	gameState_t			   GameState() const;

	//! Spawns an entity of the specified type using the provided arguments
	idEntity*			   SpawnEntityType( const idTypeInfo& classdef, const idDict* args = NULL, bool bIsClientReadSnapshot = false );

	//! Spawns an entity from a dictionary of arguments and optionally returns a pointer to the created entity.
	bool				   SpawnEntityDef( const idDict& args, idEntity** ent = NULL, bool setDefaults = true );

	//! Returns the spawn ID for the given entity.
	int					   GetSpawnId( const idEntity* ent ) const;

	//! Finds and returns the entity definition for the given name, creating a default if specified.
	const idDeclEntityDef* FindEntityDef( const char* name, bool makeDefault = true ) const;

	//! Retrieves the dictionary associated with an entity definition by name, optionally creating a default definition if it doesn't exist.
	const idDict*		   FindEntityDefDict( const char* name, bool makeDefault = true ) const;

	//! Registers an entity with the game local system and assigns it a spawn number.
	void				   RegisterEntity( idEntity* ent, int forceSpawnId, const idDict& spawnArgsToCopy );

	//! Removes an entity from the game world by clearing its registration and making its entity number available for reuse.
	void				   UnregisterEntity( idEntity* ent );

	//! Returns the spawn arguments dictionary for the current game entity.
	const idDict&		   GetSpawnArgs() const { return spawnArgs; }

	//! Checks if the specified requirement is met by the activator entity and optionally removes the item.
	bool				   RequirementMet( idEntity* activator, const idStr& requires, int removeItem );

	//! Alerts AI entities to a given entity, with special handling for multiplayer bot alerts.
	void				   AlertAI( idEntity* ent );

	//! Returns the entity that triggered the last AI alert.
	idActor*			   GetAlertEntity();

	//! Checks if the given entity is in the player's PVS.
	bool				   InPlayerPVS( idEntity* ent ) const;

	//! Checks if the given entity is in a player connected area.
	bool				   InPlayerConnectedArea( idEntity* ent ) const;
	pvsHandle_t			   GetPlayerPVS() { return playerPVS; };

	//! Sets the camera for the game local instance, handling cinematic transitions and player model visibility.
	void				   SetCamera( idCamera* cam );

	//! Returns the camera object used by the game local instance.
	idCamera*			   GetCamera() const;

	//! Determines whether to skip the current cinematic sequence.
	bool				   SkipCinematic();

	//! Calculates the horizontal and vertical field of view based on a base field of view and the current screen aspect ratio
	void				   CalcFov( float base_fov, float& fov_x, float& fov_y ) const;

	//! Adds an entity to the hash table using the specified name.
	void				   AddEntityToHash( const char* name, idEntity* ent );

	//! Removes an entity from the hash table using its name and pointer.
	bool				   RemoveEntityFromHash( const char* name, idEntity* ent );

	//! Retrieves entities from the game world that match a specified reference prefix.
	int					   GetTargets( const idDict& args, idList<idEntityPtr<idEntity>>& list, const char* ref ) const;

	//! Returns the master entity of a trace, useful for getting the parent entity of a trace hit
	idEntity*			   GetTraceEntity( const trace_t& trace ) const;

	//! Provides argument completion for entity names by calling a callback with formatted entity name suggestions.
	static void			   ArgCompletion_EntityName( const idCmdArgs& args, void ( *callback )( const char* s ) );

	/*!
		\brief Finds the closest entity of a specified type that intersects a ray trace from start to end

		This function iterates through all spawned entities to locate the closest entity that intersects a ray defined by the start and end points. It handles two different entity types differently:
	   lights use their edit origin for bounding, while other entities use their physics bounds. The function skips a specified entity and returns the closest match based on the intersection scale.
	   The search considers only entities that are of the specified type.

		\param start Starting point of the ray trace
		\param end Ending point of the ray trace
		\param c Type info for the entity type to search for
		\param skip Entity to skip during the search
		\return Pointer to the closest entity that intersects the ray trace, or NULL if none found
	*/
	idEntity*			   FindTraceEntity( idVec3 start, idVec3 end, const idTypeInfo& c, const idEntity* skip ) const;

	//! Returns the entity whose name matches the specified string.
	idEntity*			   FindEntity( const char* name ) const;

	//! Finds the next entity using the specified entity definition name, starting after the given entity.
	idEntity*			   FindEntityUsingDef( idEntity* from, const char* match ) const;

	/*!
		\brief Counts and retrieves entities within a specified radius from a given origin point.

		This function searches through all spawned entities in the game world and determines which ones intersect with a bounding volume centered at the specified origin point with a given radius. The
	   function populates the provided entity list array with pointers to entities that are within the specified radius. The search is performed by checking the intersection of each entity's absolute
	   physics bounds with the expanded bounds centered at the origin.

		\param org The origin point around which to search for entities
		\param radius The radius within which to search for entities
		\param entityList Pointer to an array where entity pointers will be stored
		\param maxCount The maximum number of entities that can be stored in the entityList array
		\return The number of entities found within the specified radius, or zero if no entities are found.
	*/
	int					   EntitiesWithinRadius( const idVec3 org, float radius, idEntity** entityList, int maxCount ) const;

	//! Kills entities that touch the bounding box of the given entity, with special handling for teleporting players.
	void				   KillBox( idEntity* ent, bool catch_teleport = false );

	/*!
		\brief Applies damage to entities within a specified radius around a given origin point.

		This function calculates damage and push effects for entities within a defined radius from a given point. It considers the damage definition specified by damageDefName and applies appropriate
	   scaling based on distance from the origin. Entities that are ignored can be specified through inflictor, attacker, ignoreDamage, or ignorePush parameters. The function also handles special
	   cases for AFAttachment entities and player-specific logic including multiplayer death handling and local client shake effects.

		\param origin The center point of the radius damage effect
		\param inflictor The entity that caused the damage
		\param attacker The entity that is the source of the damage
		\param ignoreDamage Entity to exclude from taking damage
		\param ignorePush Entity to exclude from being pushed
		\param damageDefName Name of the damage definition to use for damage calculation
		\param dmgPower Scale factor for the overall damage amount
	*/
	void			  RadiusDamage( const idVec3& origin, idEntity* inflictor, idEntity* attacker, idEntity* ignoreDamage, idEntity* ignorePush, const char* damageDefName, float dmgPower = 1.0f );

	/*!
		\brief Applies a radial push force to entities within a specified radius, with options to ignore specific entities and scale the force.

		This function calculates and applies a radial push force to all entities within a given radius from an origin point. The force can be scaled based on the inflictor entity and can be applied
	   either as a direct impulse or through a radius push operation depending on the quake parameter. Entities such as render models, projectiles, and players are excluded from the force application.
	   The function supports ignoring specific entities and can handle special cases for attachment entities.

		\param origin The center point of the radial force
		\param radius The radius within which entities are affected
		\param push The magnitude of the push force to apply
		\param inflictor The entity causing the push, used for scaling
		\param ignore An entity to exclude from the push effect
		\param inflictorScale Scaling factor for the push force when applied by the inflictor
		\param quake If true, applies the force as an impulse; otherwise uses RadiusPushClipModel
	*/
	void			  RadiusPush( const idVec3& origin, const float radius, const float push, const idEntity* inflictor, const idEntity* ignore, float inflictorScale, const bool quake );

	//! Applies a radius-based push impulse to a clip model from a given origin point
	void			  RadiusPushClipModel( const idVec3& origin, const float push, const idClipModel* clipModel );

	/*!
		\brief Projects a decal onto world surfaces or static models using a specified winding, origin, direction, and material.

		This function creates a decal by defining a winding plane and projecting it onto world geometry or static models. The decal is rendered between the winding plane and a projection origin, with
	   depth fading applied. The decal's orientation is determined by the input direction vector, and it can be either parallel or perpendicular to that direction. The function uses a predefined decal
	   winding pattern and applies random rotation based on the angle parameter or a random value if not specified. The decal is only created if the g_decals cvar is enabled.

		\param origin The 3D position where the decal projection starts
		\param dir The direction vector that determines the orientation of the decal
		\param depth The distance from the origin to the winding plane where the decal is projected
		\param parallel Specifies whether the decal projection should be parallel to the winding plane
		\param size The scale factor for the decal winding dimensions
		\param material The material name to be used for the decal
		\param angle The rotation angle in radians for the decal winding, defaults to a random value
	*/
	void			  ProjectDecal( const idVec3& origin, const idVec3& dir, float depth, bool parallel, float size, const char* material, float angle = 0 );

	/*!
		\brief Creates a blood splat effect at the specified location with the given direction, size, and material.

		This function generates a blood splat effect by creating a polygonal decal at the specified origin position. The decal is oriented based on the provided direction and scaled according to the
	   specified size. The function performs a trace to ensure the splat is properly positioned and then projects a decal at the traced location. The effect is only created if the g_bloodEffects cvar
	   is enabled.

		\param origin The world position where the blood splat should be created
		\param dir The direction the splat should face, typically the normal of the surface hit
		\param size The base size of the splat, which is randomly adjusted
		\param material The material name to use for the decal effect
	*/
	void			  BloodSplat( const idVec3& origin, const idVec3& dir, float size, const char* material );

	//! Executes a frame command function on the specified entity using the game's thread system.
	void			  CallFrameCommand( idEntity* ent, const function_t* frameCommand );

	//! Executes a frame command on the specified entity using its script object or native function.
	void			  CallObjectFrameCommand( idEntity* ent, const char* frameCommand );

	//! Returns the gravity vector used by the game engine.
	const idVec3&	  GetGravity() const;

	// added the following to assist licensees with merge issues
	int				  GetFrameNum() const { return framenum; };
	int				  GetTime() const { return time; };

	//! Returns the index of the next client entity in the game.
	int				  GetNextClientNum( int current ) const;

	//! Returns a pointer to the player entity for the client at the specified index.
	idPlayer*		  GetClientByNum( int current ) const;

	//! Returns the local player object for the current client, or NULL if not available.
	idPlayer*		  GetLocalPlayer() const;

	//! Associates areas with location entities after all entities have been spawned.
	void			  SpreadLocations();

	//! Returns the location entity for the given point.
	idLocationEntity* LocationForPoint( const idVec3& point );

	//! Selects an appropriate spawn point for a player based on game state and player settings.
	idEntity*		  SelectInitialSpawnPoint( idPlayer* player );

	//! Sets the blocking state of a portal using specified bits.
	void			  SetPortalState( qhandle_t portal, int blockingBits );

	//! Saves an entity network event to the queue for later processing.
	void			  SaveEntityNetworkEvent( const idEntity* ent, int event, const idBitMsg* msg );

	//! Returns the index parameter unchanged for the specified client number and declaration type.
	int				  ServerRemapDecl( int clientNum, declType_t type, int index );

	//! Returns the remapped declaration index for a given declaration type and index.
	int				  ClientRemapDecl( declType_t type, int index );

	//! Synchronizes game players with lobby users, creating new players for lobby users that don't have one and validating existing player connections.
	void			  SyncPlayersWithLobbyUsers( bool initial );

	//! Sends reliable initialization messages to a client to synchronize game state.
	void			  ServerWriteInitialReliableMessages( int clientNum, lobbyUserID_t lobbyUserID );

	//! Sends network-synced console variables to clients.
	void			  ServerSendNetworkSyncCvars();

	/*!
		\brief Updates the interpolation state with the provided fraction and timing information for network synchronization.

		This function sets up interpolation parameters used for network synchronization between the client and server. It stores the previous server game time, current interpolation fraction, server
	   game time, and start and end times for smooth interpolation of networked game states. The function is typically used during network updates to maintain consistent game state rendering across
	   different client configurations.

		\param fraction The interpolation fraction used for calculating intermediate positions between network updates
		\param serverGameMS The current server game time in milliseconds
		\param ssStartTime The start time for the snapshot sequence in milliseconds
		\param ssEndTime The end time for the snapshot sequence in milliseconds
	*/
	virtual void	  SetInterpolation( const float fraction, const int serverGameMS, const int ssStartTime, const int ssEndTime );

	//! Processes reliable messages from clients on the server
	void			  ServerProcessReliableMessage( int clientNum, int type, const idBitMsg& msg );

	//! Processes reliable network messages received by the client based on the message type.
	void			  ClientProcessReliableMessage( int type, const idBitMsg& msg );

	//! Returns the snapshot end time used for interpolation.
	int				  GetSSEndTime() const { return netInterpolationInfo.ssEndTime; }

	//! Returns the start time of the server snapshot interpolation.
	int				  GetSSStartTime() const { return netInterpolationInfo.ssStartTime; }

	//! Sets the global server game time in milliseconds.
	virtual void	  SetServerGameTimeMs( const int time );

	//! Returns the current server game time in milliseconds.
	virtual int		  GetServerGameTimeMs() const;

	//! Finds a predicted entity with the specified key and type in the active entities list.
	idEntity*		  FindPredictedEntity( uint32 predictedKey, idTypeInfo* type );

	//! Generates a prediction key based on weapon, player attacker, and an optional override key for client-server prediction synchronization.
	uint32			  GeneratePredictionKey( idWeapon* weapon, idPlayer* playerAttacker, int overrideKey );

	//! Returns the number of milliseconds since the last user command for the specified player.
	int				  GetLastClientUsercmdMilliseconds( int playerIndex ) const { return usercmdLastClientMilliseconds[playerIndex]; }

	//! Sets the global material used by the game.
	void			  SetGlobalMaterial( const idMaterial* mat );

	//! Returns the global material used by the game local instance.
	const idMaterial* GetGlobalMaterial();

	void			  SetGibTime( int _time ) { nextGibTime = _time; };
	int				  GetGibTime() { return nextGibTime; };

	//! Returns the AAS instance used for bot navigation.
	idAAS*			  GetBotAAS() { return bot_aas; }

	//! Registers a bot with the game local instance.
	void			  RegisterBot( iceBot* bot ) { registeredBots.AddUnique( bot ); }

	//! Removes a bot from the list of registered bots.
	void			  UnRegisterBot( iceBot* bot ) { registeredBots.Remove( bot ); }

	//! Determines whether player controls are currently inhibited by checking various game states.
	virtual bool	  InhibitControls();

	//! Checks if the PDA is currently open.
	virtual bool	  IsPDAOpen() const;

	//! Returns true if the local player is currently chatting and not spectating.
	virtual bool	  IsPlayerChatting() const;

	//! Initializes leaderboards for each map or game mode.
	virtual void	  Leaderboards_Init();

	//! Shuts down the leaderboard system.
	virtual void	  Leaderboards_Shutdown();

	//! Initializes the shell handler with the specified filename and sound world.
	virtual void	  Shell_Init( const char* filename, idSoundWorld* sw );

	//! Cleans up the VR shell handler and scoreboard
	virtual void	  Shell_Cleanup();

	//! Shows or hides the shell menu.
	virtual void	  Shell_Show( bool show );

	//! Closes the pause shell interface if valid and game is not complete
	virtual void	  Shell_ClosePause();

	//! Creates a shell menu for either in-game or main menu contexts.
	virtual void	  Shell_CreateMenu( bool inGame );

	//! Checks if the shell interface is currently active.
	virtual bool	  Shell_IsActive() const;

	//! Handles GUI events for the shell interface
	virtual bool	  Shell_HandleGuiEvent( const sysEvent_t* sev );

	//! Renders the shell interface if the shell handler is initialized.
	virtual void	  Shell_Render();

	//! Resets the shell menu handler by deallocating the existing handler and creating a new one.
	virtual void	  Shell_ResetMenu();

	//! Synchronizes the shell state with the current session state.
	virtual void	  Shell_SyncWithSession();

	//! Sets whether the shell handler can continue based on the provided validity flag.
	virtual void	  Shell_SetCanContinue( bool valid );

	//! Updates the saved games through the shell handler if it exists.
	virtual void	  Shell_UpdateSavedGames();

	//! Updates the client countdown timer for the shell handler.
	virtual void	  Shell_UpdateClientCountdown( int countdown );

	//! Updates the leaderboard using the provided callback handler.
	virtual void	  Shell_UpdateLeaderboard( const idLeaderboardCallback* callback );

	//! Sets the game completion status and shows the shell interface.
	virtual void	  Shell_SetGameComplete();

	//! Returns whether the cinematic scene should be skipped.
	virtual bool	  SkipCinematicScene();

	//! Checks if the game is currently in a cinematic sequence.
	virtual bool	  CheckInCinematic();

	enum gameDemoCommand_t {
		GCMD_UNKNOWN,
		GCMD_GAMETIME,
	};

	//! Clears the widget action repeater in the shell handler if it exists.
	void		Shell_ClearRepeater();

	//! Returns the file name of the current map.
	const char* GetMapFileName() { return mapFileName.c_str(); }

	//! Returns whether the current map is an intro map.
	bool		IsMapIntro() { return mapIsIntro; }

	//! Returns the multiplayer player definition name based on the current game type.
	const char* GetMPPlayerDefName() const;

private:
	const static int						 INITIAL_SPAWN_COUNT = 1;

	bool									 mapIsIntro;  // Leyland VR
	idStr									 mapFileName; // name of the map, empty string if no map loaded
	idMapFile*								 mapFile;	  // will be NULL during the game unless in-game editing is used
	bool									 mapCycleLoaded;

	int										 spawnCount;
	int										 mapSpawnCount; // it's handy to know which entities are part of the map

	idLocationEntity**						 locationEntities; // for location names, etc

	idCamera*								 camera;
	const idMaterial*						 globalMaterial; // for overriding everything

	idList<idAAS*>							 aasList; // area system

	idMenuHandler_Shell*					 shellHandler;

	idStrList								 aasNames;

	idEntityPtr<idActor>					 lastAIAlertEntity;
	int										 lastAIAlertTime;

	idDict									 spawnArgs; // spawn args used during entity spawning  FIXME: shouldn't be necessary anymore

	pvsHandle_t								 playerPVS;			   // merged pvs of all players
	pvsHandle_t								 playerConnectedAreas; // all areas connected to any player area

	idVec3									 gravity;		  // global gravity vector
	gameState_t								 gamestate;		  // keeps track of whether we're spawning, shutting down, or normal gameplay
	bool									 influenceActive; // true when a phantasm is happening
	int										 nextGibTime;

	idEventQueue							 eventQueue;
	idEventQueue							 savedEventQueue;

	idStaticList<spawnSpot_t, MAX_GENTITIES> spawnSpots;
	idStaticList<idEntity*, MAX_GENTITIES>	 initialSpots;
	int										 currentInitialSpot;

	idStaticList<spawnSpot_t, MAX_GENTITIES> teamSpawnSpots[2];
	idStaticList<idEntity*, MAX_GENTITIES>	 teamInitialSpots[2];
	int										 teamCurrentInitialSpot[2];

	struct netInterpolationInfo_t // Was in GameTimeManager.h in id5, needed common place to put this.
	{
		//! Initializes a netInterpolationInfo_t object with default values.
		netInterpolationInfo_t() :
			pct( 0.0f ),
			serverGameMs( 0 ),
			previousServerGameMs( 0 ),
			ssStartTime( 0 ),
			ssEndTime( 0 )
		{
		}
		float pct;					// % of current interpolation
		int	  serverGameMs;			// Interpolated server game time
		int	  previousServerGameMs; // last frame's interpolated server game time
		int	  ssStartTime;			// Server time of old snapshot
		int	  ssEndTime;			// Server time of next snapshot
	};

	netInterpolationInfo_t	  netInterpolationInfo;

	idDict					  newInfo;

	idArray<int, MAX_PLAYERS> usercmdLastClientMilliseconds; // The latest client time the server has run.
	idArray<int, MAX_PLAYERS> lastCmdRunTimeOnClient;
	idArray<int, MAX_PLAYERS> lastCmdRunTimeOnServer;

	//! Clears all game local data and resets the game state to uninitialized.
	void					  Clear();

	//! Determines whether an entity should be inhibited from spawning based on game type and difficulty settings.
	bool					  InhibitEntitySpawn( idDict& spawnArgs );

	//! Spawns all entities from the map file, including the worldspawn and other map-defined entities.
	void					  SpawnMapEntities();

	//! Populates the game map by spawning entities and initializing game state.
	void					  MapPopulate();

	//! Clears all entities from the game map, optionally preserving client entities.
	void					  MapClear( bool clearClients );

	//! Populates the game world with environment probes if none are defined by the artist.
	void					  PopulateEnvironmentProbes();

	//! Retrieves the PVS handle for a specified player and PVS type.
	pvsHandle_t				  GetClientPVS( idPlayer* player, pvsType_t type );

	//! Initializes and merges the player visibility set for all connected clients.
	void					  SetupPlayerPVS();

	//! Frees the player PVS and connected areas memory.
	void					  FreePlayerPVS();

	//! Updates the gravity value and applies it to physics objects in the game.
	void					  UpdateGravity();

	//! Sorts the active entity list to prioritize pushers, actors, and physics team masters.
	void					  SortActiveEntityList();

	//! Displays target entities and their connections in the game world for debugging purposes.
	void					  ShowTargets();

	//! Displays debug information for entities and game objects.
	void					  RunDebugInfo();

	//! Executes the shared think function for all active entities in the game loop.
	void					  RunSharedThink();

	//! Initializes the script system for the current map by creating a frame command thread and executing the default game script function.
	void					  InitScriptForMap();

	//! Sets the game frame rate value used in scripts.
	void					  SetScriptFPS( const float com_engineHz );

	//! Executes a bot input frame for all registered bots using the provided command manager.
	void					  RunBotFrame( idUserCmdMgr& cmdMgr );

	//! Spawns a player entity for the given client number, optionally as a bot.
	void					  SpawnPlayer( int clientNum, bool isBot, const char* botName );

	//! Initializes console commands for the game.
	void					  InitConsoleCommands();

	//! Removes all game-specific console commands that have been flagged with CMD_FL_GAME.
	void					  ShutdownConsoleCommands();

	//! Initializes the asynchronous network subsystem for the game local instance.
	void					  InitAsyncNetwork();

	//! Shuts down the asynchronous network event queues.
	void					  ShutdownAsyncNetwork();

	//! Logs a network event warning with detailed information about the event and entity
	void					  NetworkEventWarning( const entityNetEvent_t* event, VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Processes queued network events for entities on the server.
	void					  ServerProcessEntityNetworkEventQueue();

	//! Processes queued network events for entities on the client side.
	void					  ClientProcessEntityNetworkEventQueue();
	// call after any change to serverInfo. Will update various quick-access flags
	void					  UpdateServerInfoFlags();

	//! Randomizes the order of initial spawn points for players in multiplayer games.
	void					  RandomizeInitialSpawns();

	//! Sorts spawn points based on their distance in descending order
	static int				  sortSpawnPoints( const void* ptr1, const void* ptr2 );

	//! Simulates a batch of projectiles for the current frame
	bool					  SimulateProjectiles();

	// jmarshall
	const idDeclEntityDef*	  botItemTable;
	;

	idList<iceBot*>					  registeredBots;
	idList<iceGameDelayRemoveEntry_t> delayRemoveEntities;

	idAAS*							  bot_aas;
	// jmarshall end
};

//============================================================================

extern idGameLocal	 gameLocal;
extern idAnimManager animationLib;

/*!
	\class idGameError
	\brief Exception class for handling game-related errors.
*/
class idGameError : public idException
{
public:
	//! Constructs an idGameError object with the specified text message.
	idGameError( const char* text ) :
		idException( text )
	{
	}
};

template<class type>
ID_INLINE idEntityPtr<type>::idEntityPtr()
{
	spawnId = 0;
}

template<class type>
ID_INLINE void idEntityPtr<type>::Save( idSaveGame* savefile ) const
{
	savefile->WriteInt( spawnId );
}

template<class type>
ID_INLINE void idEntityPtr<type>::Restore( idRestoreGame* savefile )
{
	savefile->ReadInt( spawnId );
}

template<class type>
ID_INLINE idEntityPtr<type>& idEntityPtr<type>::operator=( const type* ent )
{
	if( ent == NULL ) {
		spawnId = 0;
	} else {
		spawnId = ( gameLocal.spawnIds[ent->entityNumber] << GENTITYNUM_BITS ) | ent->entityNumber;
	}
	return *this;
}

template<class type>
ID_INLINE idEntityPtr<type>& idEntityPtr<type>::operator=( const idEntityPtr& ep )
{
	spawnId = ep.spawnId;
	return *this;
}

template<class type>
ID_INLINE bool idEntityPtr<type>::SetSpawnId( int id )
{
	// the reason for this first check is unclear:
	// the function returning false may mean the spawnId is already set right, or the entity is missing
	if( id == spawnId ) { return false; }
	if( ( id >> GENTITYNUM_BITS ) == gameLocal.spawnIds[id & ( ( 1 << GENTITYNUM_BITS ) - 1 )] ) {
		spawnId = id;
		return true;
	}
	return false;
}

template<class type>
ID_INLINE bool idEntityPtr<type>::IsValid() const
{
	return ( gameLocal.spawnIds[spawnId & ( ( 1 << GENTITYNUM_BITS ) - 1 )] == ( spawnId >> GENTITYNUM_BITS ) );
}

template<class type>
ID_INLINE type* idEntityPtr<type>::GetEntity() const
{
	int entityNum = spawnId & ( ( 1 << GENTITYNUM_BITS ) - 1 );
	// DG: removed extraneous parenthesis to shut up clang
	if( gameLocal.spawnIds[entityNum] == ( spawnId >> GENTITYNUM_BITS ) ) { return static_cast<type*>( gameLocal.entities[entityNum] ); }
	return NULL;
}

template<class type>
ID_INLINE int idEntityPtr<type>::GetEntityNum() const
{
	return ( spawnId & ( ( 1 << GENTITYNUM_BITS ) - 1 ) );
}

//  ===========================================================================

//
// these defines work for all startsounds from all entity types
// make sure to change script/doom_defs.script if you add any channels, or change their order
//
typedef enum {
	SND_CHANNEL_ANY	  = SCHANNEL_ANY,
	SND_CHANNEL_VOICE = SCHANNEL_ONE,
	SND_CHANNEL_VOICE2,
	SND_CHANNEL_BODY,
	SND_CHANNEL_BODY2,
	SND_CHANNEL_BODY3,
	SND_CHANNEL_WEAPON,
	SND_CHANNEL_ITEM,
	SND_CHANNEL_HEART,
	SND_CHANNEL_PDA_AUDIO,
	SND_CHANNEL_PDA_VIDEO,
	SND_CHANNEL_DEMONIC,
	SND_CHANNEL_RADIO,

	// internal use only.  not exposed to script or framecommands.
	SND_CHANNEL_AMBIENT,
	SND_CHANNEL_DAMAGE,
	SND_CHANNEL_MUSIC // RB
} gameSoundChannel_t;

// content masks
#define MASK_ALL			  ( -1 )
#define MASK_SOLID			  ( CONTENTS_SOLID )
#define MASK_MONSTERSOLID	  ( CONTENTS_SOLID | CONTENTS_MONSTERCLIP | CONTENTS_BODY )
#define MASK_PLAYERSOLID	  ( CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_BODY )
#define MASK_DEADSOLID		  ( CONTENTS_SOLID | CONTENTS_PLAYERCLIP )
#define MASK_WATER			  ( CONTENTS_WATER )
#define MASK_OPAQUE			  ( CONTENTS_OPAQUE )
#define MASK_SHOT_RENDERMODEL ( CONTENTS_SOLID | CONTENTS_RENDERMODEL )
#define MASK_SHOT_BOUNDINGBOX ( CONTENTS_SOLID | CONTENTS_BODY )
#define MASK_SHOT			  ( CONTENTS_SOLID | CONTENTS_BODY | CONTENTS_CORPSE ) // jmarshall

const float DEFAULT_GRAVITY = 1066.0f;
#define DEFAULT_GRAVITY_STRING "1066"
const idVec3 DEFAULT_GRAVITY_VEC3( 0, 0, -DEFAULT_GRAVITY );

const int	 CINEMATIC_SKIP_DELAY = SEC2MS( 2.0f );

//============================================================================

#include "physics/Force.h"
#include "physics/Force_Constant.h"
#include "physics/Force_Drag.h"
#include "physics/Force_Grab.h"
#include "physics/Force_Field.h"
#include "physics/Force_Spring.h"
#include "physics/Physics.h"
#include "physics/Physics_Static.h"
#include "physics/Physics_StaticMulti.h"
#include "physics/Physics_Base.h"
#include "physics/Physics_Actor.h"
#include "physics/Physics_Monster.h"
#include "physics/Physics_Player.h"
#include "physics/Physics_Parametric.h"
#include "physics/Physics_RigidBody.h"
#include "physics/Physics_AF.h"

#include "SmokeParticles.h"

#include "entities/Entity.h"
#include "GameEdit.h"
#include "entities/Grabber.h"
#include "entities/AF.h"
#include "IK.h"
#include "entities/AFEntity.h"
#include "entities/Misc.h"
#include "entities/Actor.h"
#include "entities/Projectile.h"
#include "Weapon.h"
#include "entities/Light.h"
#include "entities/Envprobe.h"
#include "entities/WorldSpawn.h"
#include "entities/Item.h"
#include "player/PlayerView.h"
#include "player/PlayerIcon.h"
#include "Achievements.h"
#include "AimAssist.h"
#include "player/Inventory.h"
#include "player/Player.h"
#include "entities/Mover.h"
#include "entities/Camera.h"
#include "entities/Moveable.h"
#include "entities/Target.h"
#include "entities/Trigger.h"
#include "entities/Sound.h"
#include "entities/Fx.h"
#include "entities/SecurityCamera.h"
#include "entities/BrittleFracture.h"

#include "ai/AI.h"
#include "anim/Anim_Testmodel.h"

// jmarshall
#include "weapons/Weapon_fist.h"
#include "weapons/Weapon_pistol.h"
#include "weapons/Weapon_flashlight.h"
#include "weapons/Weapon_pda.h"
#include "weapons/Weapon_shotgun.h"
#include "weapons/Weapon_double_shotgun.h"
#include "weapons/Weapon_machinegun.h"
#include "weapons/Weapon_plasmagun.h"
#include "weapons/Weapon_chaingun.h"
#include "weapons/Weapon_rocketlauncher.h"
#include "weapons/Weapon_bfg.h"
#include "weapons/Weapon_handgrenade.h"
#include "weapons/Weapon_chainsaw.h"
#include "weapons/Weapon_grabber.h"
// jmarshall end

// menus
#include "menus/MenuWidget.h"
#include "menus/MenuScreen.h"
#include "menus/MenuHandler.h"

#include "script/Script_Compiler.h"
#include "script/Script_Interpreter.h"
#include "script/Script_Thread.h"

#endif /* !__GAME_LOCAL_H__ */
