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

#undef private
#undef protected

// DG: achievements and signin is the same on windows, linux => put them in common dir
#include "common/achievements.h"
#include "common/signin.h"
// DG end

#include "sys_lobby_backend.h"
#include "sys_lobby.h"

class idSaveGameProcessorNextMap;
class idSaveGameProcessorSaveGame;
class idSaveGameProcessorLoadGame;
class idSaveGameProcessorDelete;
class idSaveGameProcessorEnumerateGames;

/*!
	\class idLobbyStub
	\brief Provides a stub implementation for lobby management functionality.

	This class serves as a base implementation for lobby management, offering a set of virtual methods that define the interface for interacting with lobby users, managing peer connections, and
   handling reliable message transmission. It extends idLobbyBase and provides default behaviors for lobby operations. The class is designed to be inherited by more specific lobby implementations that
   may override certain methods to provide customized functionality. It supports operations such as user connection handling, team management, message sending to specific users or the host, and
   snapshot management for synchronization purposes. The stub implementation handles basic lobby state management and user tracking, but delegates more complex operations to derived classes.

*/
class idLobbyStub : public idLobbyBase
{
public:
	//! Returns whether the current session is hosted by this client
	virtual bool					 IsHost() const { return false; }

	//! Returns false indicating this lobby stub is not a peer.
	virtual bool					 IsPeer() const { return false; }

	//! Checks if the lobby has any active peers.
	virtual bool					 HasActivePeers() const { return false; }

	//! Returns the number of users in the lobby
	virtual int						 GetNumLobbyUsers() const { return 0; }

	//! Returns the number of active users in the lobby.
	virtual int						 GetNumActiveLobbyUsers() const { return 0; }

	//! Checks if a lobby user at the specified index is connected.
	virtual bool					 IsLobbyUserConnected( int index ) const { return false; }

	//! Returns the lobby user ID for a given user index from the lobby
	virtual lobbyUserID_t			 GetLobbyUserIdByOrdinal( int userIndex ) const { return lobbyUserID_t(); }

	//! Returns the lobby user index corresponding to the given lobby user ID, or -1 if not found.
	virtual int						 GetLobbyUserIndexFromLobbyUserID( lobbyUserID_t lobbyUserID ) const { return -1; }

	/*!
		\brief Sends a reliable message to peers in the lobby with optional callback and user mask.

		This function transmits a reliable message to connected peers in the lobby. It supports optional callback execution on receipt and allows specifying which users in the session should receive
	   the message via a bitmask. The function is virtual and may be overridden by derived classes to provide specific implementations for sending messages.

		\param type Type identifier for the message being sent
		\param msg The message content to be sent
		\param callReceiveReliable Flag indicating whether the receive callback should be invoked when the message is received
		\param sessionUserMask Bitmask specifying which users in the session should receive this message
	*/
	virtual void					 SendReliable( int type, idBitMsg& msg, bool callReceiveReliable = true, peerMask_t sessionUserMask = MAX_UNSIGNED_TYPE( peerMask_t ) ) { }

	//! Sends a reliable message to a specific lobby user.
	virtual void					 SendReliableToLobbyUser( lobbyUserID_t lobbyUserID, int type, idBitMsg& msg ) { }

	//! Sends a reliable message to the host in the lobby
	virtual void					 SendReliableToHost( int type, idBitMsg& msg ) { }

	//! Returns the user name for a given lobby user ID, or 'INVALID' if the ID is not valid.
	virtual const char*				 GetLobbyUserName( lobbyUserID_t lobbyUserID ) const { return "INVALID"; }

	//! Kicks a lobby user identified by the given lobby user ID.
	virtual void					 KickLobbyUser( lobbyUserID_t lobbyUserID ) { }

	//! Checks if a lobby user ID refers to a valid lobby user.
	virtual bool					 IsLobbyUserValid( lobbyUserID_t lobbyUserID ) const { return false; }

	//! Checks if a lobby user is loaded based on the provided lobby user ID.
	virtual bool					 IsLobbyUserLoaded( lobbyUserID_t lobbyUserID ) const { return false; }

	//! Returns whether the lobby user has loaded the first full snapshot
	virtual bool					 LobbyUserHasFirstFullSnap( lobbyUserID_t lobbyUserID ) const { return false; }

	//! Enables snapshots for the specified lobby user.
	virtual void					 EnableSnapshotsForLobbyUser( lobbyUserID_t lobbyUserID ) { }

	//! Returns the skin index for a specified lobby user.
	virtual int						 GetLobbyUserSkinIndex( lobbyUserID_t lobbyUserID ) const { return 0; }

	//! Returns the weapon auto-reload setting for a specified lobby user
	virtual bool					 GetLobbyUserWeaponAutoReload( lobbyUserID_t lobbyUserID ) const { return false; }

	//! Returns the weapon auto-switch setting for a specific lobby user
	virtual bool					 GetLobbyUserWeaponAutoSwitch( lobbyUserID_t lobbyUserID ) const { return false; }

	//! Returns the user level of the specified lobby user
	virtual int						 GetLobbyUserLevel( lobbyUserID_t lobbyUserID ) const { return 0; }

	//! Retrieves the quality of service value for a specified lobby user.
	virtual int						 GetLobbyUserQoS( lobbyUserID_t lobbyUserID ) const { return 0; }

	//! Retrieves the team number associated with a specific lobby user.
	virtual int						 GetLobbyUserTeam( lobbyUserID_t lobbyUserID ) const { return 0; }

	//! Sets the team number for a specified lobby user and returns true if successful
	virtual bool					 SetLobbyUserTeam( lobbyUserID_t lobbyUserID, int teamNumber ) { return false; }

	//! Returns the party token associated with the specified lobby user ID.
	virtual int						 GetLobbyUserPartyToken( lobbyUserID_t lobbyUserID ) const { return 0; }

	//! Retrieves the player profile associated with a specific lobby user ID.
	virtual idPlayerProfile*		 GetProfileFromLobbyUser( lobbyUserID_t lobbyUserID ) { return NULL; }

	//! Returns the local user associated with the specified lobby user ID
	virtual idLocalUser*			 GetLocalUserFromLobbyUser( lobbyUserID_t lobbyUserID ) { return NULL; }

	//! Returns the number of lobby users on the specified team
	virtual int						 GetNumLobbyUsersOnTeam( int teamNumber ) const { return 0; }

	//! Converts a lobby user ID to a peer index.
	virtual int						 PeerIndexFromLobbyUser( lobbyUserID_t lobbyUserID ) const { return -1; }

	//! Returns the time since the last packet was received from the specified peer.
	virtual int						 GetPeerTimeSinceLastPacket( int peerIndex ) const { return 0; }

	//! Returns the peer index corresponding to the host in the lobby
	virtual int						 PeerIndexForHost() const { return -1; }

	//! Allocates a lobby user slot for a bot with the specified name and returns the corresponding user ID.
	virtual lobbyUserID_t			 AllocLobbyUserSlotForBot( const char* botName ) { return lobbyUserID_t(); }

	//! Removes a bot from the lobby user list using the specified lobby user ID.
	virtual void					 RemoveBotFromLobbyUserList( lobbyUserID_t lobbyUserID ) { }

	//! Checks whether the specified lobby user is a bot.
	virtual bool					 GetLobbyUserIsBot( lobbyUserID_t lobbyUserID ) const { return false; }

	//! Returns the user name of the host.
	virtual const char*				 GetHostUserName() const { return "INVALID"; }

	//! Returns the match parameters for the lobby stub.
	virtual const idMatchParameters& GetMatchParms() const { return fakeParms; }

	//! Returns true if the lobby has reached its maximum capacity.
	virtual bool					 IsLobbyFull() const { return false; }

	//! Ensures all peers in the lobby have the base game state synchronized
	virtual bool					 EnsureAllPeersHaveBaseState() { return false; }

	//! Returns true if all peers in the lobby are in the game.
	virtual bool					 AllPeersInGame() const { return false; }

	//! Returns the number of connected peers in the lobby.
	virtual int						 GetNumConnectedPeers() const { return 0; }

	//! Returns the number of connected peers that are currently in the game
	virtual int						 GetNumConnectedPeersInGame() const { return 0; }

	//! Returns the peer index of the local player relative to the host in a lobby system
	virtual int						 PeerIndexOnHost() const { return -1; }

	//! Checks if a peer at the specified index is disconnected
	virtual bool					 IsPeerDisconnected( int peerIndex ) const { return false; }

	//! Checks if all peers in the lobby have a stale snapshot object with the specified ID.
	virtual bool					 AllPeersHaveStaleSnapObj( int objId ) { return false; }

	//! Checks if all peers in the lobby have the expected snapshot object with the specified ID.
	virtual bool					 AllPeersHaveExpectedSnapObj( int objId ) { return false; }

	//! Refreshes the snapshot object with the specified ID.
	virtual void					 RefreshSnapObj( int objId ) { }

	//! Marks a snapshot object as deleted using the provided object ID.
	virtual void					 MarkSnapObjDeleted( int objId ) { }

	//! Adds a snapshot object template to the lobby system
	virtual void					 AddSnapObjTemplate( int objID, idBitMsg& msg ) { }

	//! Draws debug network information on the heads-up display.
	virtual void					 DrawDebugNetworkHUD() const { }

	//! Draws debug network HUD2 information
	virtual void					 DrawDebugNetworkHUD2() const { }

	//! Draws server snapshot metrics debugging information when called with draw set to true
	virtual void					 DrawDebugNetworkHUD_ServerSnapshotMetrics( bool draw ) { }

private:
	idMatchParameters fakeParms;
};

/*!
	\class idSessionLocal
	\brief Manages local session state and network communication for multiplayer gaming.

	Handles all aspects of local multiplayer session management including lobby creation, state transitions, network communication, player coordination, and game flow control. Provides interfaces for
   matching, party management, game state handling, and integration with platform-specific features like voice chat, save games, and leaderboard systems. The class coordinates between different lobby
   types (party, game, game state) and manages the complex state machine that governs multiplayer sessions from initial connection through gameplay and post-match transitions. It also handles
   platform-specific functionality such as user authentication, content management, and system UI integration while maintaining network synchronization across all connected peers.

*/
class idSessionLocal : public idSession
{
	friend class idLeaderboards;
	friend class idStatsSession;
	friend class idLobbyBackend360;
	friend class idLobbyBackendPS3;
	friend class idSessionLocalCallbacks;
	friend class idPsnAsyncSubmissionLookupPS3_TitleStorage;
	friend class idNetSessionPort;
	friend class lobbyAddress_t;

protected:
	//=====================================================================================================
	//	Mixed Common/Platform enums/structs
	//=====================================================================================================

	// Overall state of the session
	enum state_t {
		STATE_IDLE,								   // We are at the main menu
		STATE_PARTY_LOBBY_HOST,					   // We are in the party lobby menu as host
		STATE_PARTY_LOBBY_PEER,					   // We are in the party lobby menu as a peer
		STATE_GAME_LOBBY_HOST,					   // We are in the game lobby as a host
		STATE_GAME_LOBBY_PEER,					   // We are in the game lobby as a peer
		STATE_GAME_STATE_LOBBY_HOST,			   // We are in the game state lobby as a host
		STATE_GAME_STATE_LOBBY_PEER,			   // We are in the game state lobby as a peer
		STATE_CREATE_AND_MOVE_TO_PARTY_LOBBY,	   // We are creating a party lobby, and will move to that state when done
		STATE_CREATE_AND_MOVE_TO_GAME_LOBBY,	   // We are creating a game lobby, and will move to that state when done
		STATE_CREATE_AND_MOVE_TO_GAME_STATE_LOBBY, // We are creating a game state lobby, and will move to that state when done
		STATE_FIND_OR_CREATE_MATCH,
		STATE_CONNECT_AND_MOVE_TO_PARTY,
		STATE_CONNECT_AND_MOVE_TO_GAME,
		STATE_CONNECT_AND_MOVE_TO_GAME_STATE,
		STATE_BUSY, // Doing something internally like a QoS/bandwidth challenge

		// These are last, so >= STATE_LOADING tests work
		STATE_LOADING, // We are loading the map, preparing to go into a match
		STATE_INGAME,  // We are currently in a match
		NUM_STATES
	};

	enum connectType_t {
		CONNECT_NONE		   = 0,
		CONNECT_DIRECT		   = 1,
		CONNECT_FIND_OR_CREATE = 2,
	};

	enum pendingInviteMode_t {
		PENDING_INVITE_NONE			= 0, // No invite waiting
		PENDING_INVITE_WAITING		= 1, // Invite is waiting
		PENDING_SELF_INVITE_WAITING = 2, // We invited ourselves to a match
	};

	struct contentData_t {
		bool					isMounted;
		idStrStatic<128>		displayName;
		idStrStatic<MAX_OSPATH> packageFileName;
		idStrStatic<MAX_OSPATH> rootPath;
		int						dlcID;
	};

public:
	//! Initializes a new instance of the idSessionLocal class.
	idSessionLocal();

	//! Destroys the session local object and cleans up associated resources.
	virtual ~idSessionLocal();

	//! Initializes the base state of the session local object.
	void						 InitBaseState();

	//! Checks if the platform party is currently in a lobby.
	virtual bool				 IsPlatformPartyInLobby();

	//! Returns the number of downloadable content packages available.
	virtual int					 GetNumContentPackages() const;

	//! Returns the content package ID for the specified content index.
	virtual int					 GetContentPackageID( int contentIndex ) const;

	//! Returns the file system path for a mounted downloadable content package at the specified index.
	virtual const char*			 GetContentPackagePath( int contentIndex ) const;

	//! Returns the index of a content package given its ID
	virtual int					 GetContentPackageIndexForID( int contentID ) const;

	//! Returns whether the system marketplace has new content available.
	virtual bool				 GetSystemMarketplaceHasNewContent() const { return marketplaceHasNewContent; }

	//! Sets the flag indicating whether the system marketplace has new content.
	virtual void				 SetSystemMarketplaceHasNewContent( bool hasNewContent ) { marketplaceHasNewContent = hasNewContent; }

	//! Creates a new party lobby with the specified match parameters.
	virtual void				 CreatePartyLobby( const idMatchParameters& parms_ );

	//! Initiates searching for or creating a game match based on the provided parameters.
	virtual void				 FindOrCreateMatch( const idMatchParameters& parms );

	//! Creates a new match with the specified parameters.
	virtual void				 CreateMatch( const idMatchParameters& parms_ );

	//! Creates a new game lobby state using the provided match parameters.
	virtual void				 CreateGameStateLobby( const idMatchParameters& parms_ );

	//! Updates party parameters in the lobby or game lobby to keep them synchronized.
	virtual void				 UpdatePartyParms( const idMatchParameters& parms_ );

	//! Updates match parameters for the current lobby if the session is in the lobby state and the local user is the host.
	virtual void				 UpdateMatchParms( const idMatchParameters& parms_ );

	//! Starts a game match when hosting a lobby.
	virtual void				 StartMatch();

	//! Sets a session option by ORing it with the existing session options.
	virtual void				 SetSessionOption( sessionOption_t option ) { sessionOptions |= option; }

	//! Clears a specific session option by removing it from the session options bitmask.
	virtual void				 ClearSessionOption( sessionOption_t option ) { sessionOptions &= ~option; }

	//! Returns the appropriate session state to transition to when navigating back from the current state.
	virtual sessionState_t		 GetBackState();

	//! Cancels the current session and returns to the previous state
	virtual void				 Cancel();

	//! Finalizes the disconnection process by closing the port and clearing send and receive queues.
	virtual void				 FinishDisconnect();

	//! Determines whether the migrating dialog should be displayed based on the active lobby's migration status.
	virtual bool				 ShouldShowMigratingDialog() const;

	//! Checks if the current lobby is in the process of migrating.
	virtual bool				 IsCurrentLobbyMigrating() const;

	//! Checks if the session is losing connection to the host.
	virtual bool				 IsLosingConnectionToHost() const;

	//! Returns true if the current game session was migrated from another game instance.
	virtual bool				 WasMigrationGame() const;

	//! Returns true if the current lobby is hosting a migrated game with valid migration data and is not currently migrating.
	virtual bool				 ShouldRelaunchMigrationGame() const;

	//! Retrieves or sets migration game data from the current lobby session.
	virtual bool				 GetMigrationGameData( idBitMsg& msg, bool reading );

	//! Returns false if the current session host, otherwise delegates to the game lobby to handle migration game data for a specific user.
	virtual bool				 GetMigrationGameDataUser( lobbyUserID_t lobbyUserID, idBitMsg& msg, bool reading );

	//! Returns true if the game lobby was coalesced.
	virtual bool				 WasGameLobbyCoalesced() const { return gameLobbyWasCoalesced; }

	//! Returns the loading identifier for the session.
	virtual int					 GetLoadingID() { return loadingID; }

	//! Returns true if the session is about to load.
	virtual bool				 IsAboutToLoad() const { return GetGameLobby().IsLobbyActive() && GetGameLobby().startLoadingFromHost; }

	//! Retrieves the next match parameter update from the session local storage.
	virtual bool				 GetMatchParamUpdate( int& peer, int& msg );

	//! Sets up inputRouting to be a mapping from inputDevice index to session user index and returns the number of local users.
	virtual int					 GetInputRouting( int inputRouting[MAX_INPUT_DEVICES] );

	//! Ends a match gracefully and returns to the game lobby
	virtual void				 EndMatch( bool premature = false );

	//! Finalizes match-related operations when the game ends before returning to the lobby, particularly handling host migration scenarios.
	virtual void				 MatchFinished();

	//! Quits the current match, handling different behaviors for hosts and regular players.
	virtual void				 QuitMatch();

	//! Forcefully quits the current match and returns to the title screen.
	virtual void				 QuitMatchToTitle();

	//! Marks the loading process as complete and transitions the session state accordingly.
	virtual void				 LoadingFinished();

	//! Processes session updates and maintains lobby synchronization.
	virtual void				 Pump();

	//! Processes the snapshot acknowledgment queue when a lobby is active.
	virtual void				 ProcessSnapAckQueue();

	//! Returns the current session state converted from the internal state to an external state enumeration.
	virtual sessionState_t		 GetState() const;

	//! Returns a string representation of the current session state.
	virtual const char*			 GetStateString() const;

	//! Sends user commands to the host peer in a lobby game session
	virtual void				 SendUsercmds( idBitMsg& msg );

	//! Sends a snapshot to all connected and loaded peers in the acting game state lobby.
	virtual void				 SendSnapshot( idSnapShot& ss );

	//! Returns the name of the peer at the specified index
	virtual const char*			 GetPeerName( int peerNum );

	//! Returns the gamer tag of the local user at the specified index.
	virtual const char*			 GetLocalUserName( int i ) const { return signInManager->GetLocalUserByIndex( i )->GetGamerTag(); }

	//! Updates the sign-in manager to handle local user authentication and lobby synchronization.
	virtual void				 UpdateSignInManager();

	//! Returns the player profile associated with the master local user, or a default profile if none is available.
	virtual idPlayerProfile*	 GetProfileFromMasterLocalUser();

	//! Prepares for the process of picking a new host for a lobby, handling different migration scenarios based on lobby state and match flags.
	virtual void				 PrePickNewHost( idLobby& lobby, bool forceMe, bool inviteOldHost );

	//! Checks if a lobby invite can be processed during a migration event
	virtual bool				 PreMigrateInvite( idLobby& lobby );

	//! Retrieves a floating-point value from title storage variables, returning a default if the key is not found.
	virtual float				 GetTitleStorageFloat( const char* name, float defaultFloat ) const { return titleStorageVars.GetFloat( name, defaultFloat ); }

	//! Retrieves an integer value from title storage, returning a default if the key is not found.
	virtual int					 GetTitleStorageInt( const char* name, int defaultInt ) const { return titleStorageVars.GetInt( name, defaultInt ); }

	//! Retrieves a boolean value from title storage, returning a default if the key is not found.
	virtual bool				 GetTitleStorageBool( const char* name, bool defaultBool ) const { return titleStorageVars.GetBool( name, defaultBool ); }

	//! Returns a string value for a given title storage variable name, or a default string if the variable is not found.
	virtual const char*			 GetTitleStorageString( const char* name, const char* defaultString ) const { return titleStorageVars.GetString( name, defaultString ); }

	//! Retrieves a float value from title storage by name, returning a default if not found.
	virtual bool				 GetTitleStorageFloat( const char* name, float defaultFloat, float& out ) const { return titleStorageVars.GetFloat( name, defaultFloat, out ); }

	//! Retrieves an integer value from title storage, returning a default if the key is not found.
	virtual bool				 GetTitleStorageInt( const char* name, int defaultInt, int& out ) const { return titleStorageVars.GetInt( name, defaultInt, out ); }

	//! Retrieves a boolean title storage variable by name, returning a default value if not found
	virtual bool				 GetTitleStorageBool( const char* name, bool defaultBool, bool& out ) const { return titleStorageVars.GetBool( name, defaultBool, out ); }

	//! Retrieves a string value from title storage, returning a default if the key is not found.
	virtual bool				 GetTitleStorageString( const char* name, const char* defaultString, const char** out ) const { return titleStorageVars.GetString( name, defaultString, out ); }

	//! Returns true if the title storage has been loaded.
	virtual bool				 IsTitleStorageLoaded() { return titleStorageLoaded; }

	//! Returns the voice chat state for a specific user in the active lobby
	virtual voiceState_t		 GetLobbyUserVoiceState( lobbyUserID_t lobbyUserID );

	//! Converts a voice chat state to its corresponding display state.
	virtual voiceStateDisplay_t	 GetDisplayStateFromVoiceState( voiceState_t voiceState ) const;

	//! Toggles the voice mute state for a specified lobby user.
	virtual void				 ToggleLobbyUserVoiceMute( lobbyUserID_t lobbyUserID );

	//! Sets the active chat group for voice communication.
	virtual void				 SetActiveChatGroup( int groupIndex );

	//! Updates the headset state for the master user in the current session lobby
	virtual void				 UpdateMasterUserHeadsetState();

	//! Starts or continues a bandwidth challenge using the active platform lobby.
	virtual bool				 StartOrContinueBandwidthChallenge( bool forceStart );

	//! Sets the snapshot rate for a specified peer in the active lobby.
	virtual void				 DebugSetPeerSnaprate( int peerIndex, int snapRateMS );

	//! Returns the total incoming byte rate from all connected peers in the active lobby
	virtual float				 GetIncomingByteRate();

	//=====================================================================================================
	// Invites
	//=====================================================================================================
	virtual void				 HandleBootableInvite( int64 lobbyId = 0 ) = 0;
	virtual void				 ClearBootableInvite()					   = 0;
	virtual void				 ClearPendingInvite()					   = 0;
	virtual bool				 HasPendingBootableInvite()				   = 0;
	virtual void				 SetDiscSwapMPInvite( void* parm )		   = 0; // call to request a discSwap multiplayer invite
	virtual void*				 GetDiscSwapMPInviteParms()				   = 0;

	//! Returns true if a disc swap multiplayer invite has been requested.
	virtual bool				 IsDiscSwapMPInviteRequested() const { return inviteInfoRequested; }

	//! Returns the flushed statistics status of the local session.
	bool						 GetFlushedStats() { return flushedStats; }

	//! Sets the flushed statistics flag to the specified boolean value.
	void						 SetFlushedStats( bool _flushedStats ) { flushedStats = _flushedStats; }

	//! Handles the event when a local user signs in to the session.
	virtual void				 OnLocalUserSignin( idLocalUser* user );

	//! Handles the signout event for a local user, removing them from the achievement system and potentially triggering master user signout.
	virtual void				 OnLocalUserSignout( idLocalUser* user );

	//! Handles the cleanup when the master local user signs out
	virtual void				 OnMasterLocalUserSignout();

	//! Handles the event when the master local user signs in by initiating asynchronous save game enumeration.
	virtual void				 OnMasterLocalUserSignin();

	//! Notifies when a local user's profile has been loaded after signing in.
	virtual void				 OnLocalUserProfileLoaded( idLocalUser* user );

	//=====================================================================================================
	//	Platform specific (different platforms implement these differently)
	//=====================================================================================================

	virtual void				 Initialize() = 0;
	virtual void				 Shutdown()	  = 0;

	virtual void				 InitializeSoundRelatedSystems() = 0;
	virtual void				 ShutdownSoundRelatedSystems()	 = 0;

	virtual void				 PlatformPump() = 0;

	virtual void				 InviteFriends()	 = 0;
	virtual void				 InviteParty()		 = 0;
	virtual void				 ShowPartySessions() = 0;

	virtual bool				 ProcessInputEvent( const sysEvent_t* ev ) = 0;

	// Play with Friends server listing
	virtual int					 NumServers() const						   = 0;
	virtual void				 ListServers( const idCallback& callback ) = 0;

	//! Sends a broadcast query to discover servers on the network.
	virtual void				 ListServersCommon();
	virtual void				 CancelListServers()	   = 0;
	virtual void				 ConnectToServer( int i )  = 0;
	virtual const serverInfo_t*	 ServerInfo( int i ) const = 0;

	//! Returns a pointer to a list of player names for the specified server index.
	virtual const idList<idStr>* ServerPlayerList( int i );
	virtual void				 ShowServerGamerCardUI( int i ) = 0;

	virtual void				 HandleServerQueryRequest( lobbyAddress_t& remoteAddr, idBitMsg& msg, int msgType ) = 0;
	virtual void				 HandleServerQueryAck( lobbyAddress_t& remoteAddr, idBitMsg& msg )					= 0;

	// System UI
	virtual bool				 IsSystemUIShowing() const		 = 0;
	virtual void				 SetSystemUIShowing( bool show ) = 0;

	virtual void				 ShowSystemMarketplaceUI() const					   = 0;
	virtual void				 ShowLobbyUserGamerCardUI( lobbyUserID_t lobbyUserID ) = 0;

	// Leaderboards
	virtual void				 LeaderboardUpload( lobbyUserID_t lobbyUserID, const leaderboardDefinition_t* leaderboard, const column_t* stats, const idFile_Memory* attachment = NULL )	   = 0;
	virtual void				 LeaderboardDownload( int sessionUserIndex, const leaderboardDefinition_t* leaderboard, int startingRank, int numRows, const idLeaderboardCallback& callback ) = 0;
	virtual void				 LeaderboardDownloadAttachment( int sessionUserIndex, const leaderboardDefinition_t* leaderboard, int64 attachmentID )										   = 0;

	// Scoring (currently just for TrueSkill)
	virtual void				 SetLobbyUserRelativeScore( lobbyUserID_t lobbyUserID, int relativeScore, int team ) = 0;

	virtual void				 LeaderboardFlush() = 0;

	//! Saves game data synchronously with the specified name, file list, and description.
	virtual saveGameHandle_t	 SaveGameSync( const char* name, const saveFileEntryList_t& files, const idSaveGameDetails& description );

	//! Initiates an asynchronous save game operation with the specified name, files, and description
	virtual saveGameHandle_t	 SaveGameAsync( const char* name, const saveFileEntryList_t& files, const idSaveGameDetails& description );

	//! Loads a save game synchronously using the provided name and file list
	virtual saveGameHandle_t	 LoadGameSync( const char* name, saveFileEntryList_t& files );

	//! Enumerates save games synchronously and returns a handle to the operation.
	virtual saveGameHandle_t	 EnumerateSaveGamesSync();

	//! Initiates an asynchronous enumeration of save games and returns a handle to track the operation.
	virtual saveGameHandle_t	 EnumerateSaveGamesAsync();

	//! Deletes a save game file synchronously and returns a handle to track the operation
	virtual saveGameHandle_t	 DeleteSaveGameSync( const char* name );

	//! Deletes a save game asynchronously and returns a handle to track the operation
	virtual saveGameHandle_t	 DeleteSaveGameAsync( const char* name );

	//! Checks if a save game operation identified by the given handle has completed.
	virtual bool				 IsSaveGameCompletedFromHandle( const saveGameHandle_t& handle ) const { return saveGameManager->IsSaveGameCompletedFromHandle( handle ); }

	//! Cancels a save game operation identified by the provided handle.
	virtual void				 CancelSaveGameWithHandle( const saveGameHandle_t& handle ) { GetSaveGameManager().CancelWithHandle( handle ); }

	//! Returns the list of enumerated savegames
	virtual const saveGameDetailsList_t& GetEnumeratedSavegames() const { return saveGameManager->GetEnumeratedSavegames(); }

	//! Checks if the session is currently enumerating save games.
	virtual bool						 IsEnumerating() const;

	//! Returns the handle for save game enumeration.
	virtual saveGameHandle_t			 GetEnumerationHandle() const;

	//! Sets the current save slot to the specified slot name.
	virtual void						 SetCurrentSaveSlot( const char* slotName ) { currentSaveSlot = slotName; }

	//! Returns the name of the current save slot used by the session.
	virtual const char*					 GetCurrentSaveSlot() const { return currentSaveSlot.c_str(); }

	//! Handles the completion of a save operation, updating game state and UI indicators
	void								 OnSaveCompleted( idSaveLoadParms* parms );

	//! Handles the completion of a save game load operation.
	void								 OnLoadCompleted( idSaveLoadParms* parms );

	//! Handles completion of a save game deletion operation.
	void								 OnDeleteCompleted( idSaveLoadParms* parms );

	//! Handles the completion of save game enumeration by sorting and processing the enumerated save game details
	void								 OnEnumerationCompleted( idSaveLoadParms* parms );

	//! Checks if the DLC content for a specified map is available.
	virtual bool						 IsDLCAvailable( const char* mapName );

	//! Checks if the correct disc is inserted for loading a save game.
	virtual bool						 LoadGameCheckDiscNumber( idSaveLoadParms& parms );

	//! Checks if the savegame description file is valid and compatible with the current build
	bool								 LoadGameCheckDescriptionFile( idSaveLoadParms& parms );

	// Downloadable Content
	virtual void						 EnumerateDownloadableContent() = 0;

	//! Drops a client from a specified session and lobby type.
	void								 DropClient( int peerNum, int session );

protected:
	//! Returns the upstream packet drop rate.
	float				 GetUpstreamDropRate() { return upstreamDropRate; }

	//! Returns the rate at which data is being sent upstream in the queue.
	float				 GetUpstreamQueueRate() { return upstreamQueueRate; }

	//! Returns the number of queued bytes in the session local.
	int					 GetQueuedBytes() { return queuedBytes; }

	//=====================================================================================================
	// Common functions (sys_session_local.cpp)
	//=====================================================================================================
	void				 HandleLobbyControllerState( int lobbyType );

	//! Processes a pending invite by validating the master user and connecting to the lobby.
	virtual void		 UpdatePendingInvite();

	//! Handles the current session state by pumping lobbies and executing state-specific logic
	bool				 HandleState();

	//! Returns a reference to the party lobby instance.
	idLobby&			 GetPartyLobby() { return partyLobby; }

	//! Returns a constant reference to the local session's party lobby.
	const idLobby&		 GetPartyLobby() const { return partyLobby; }

	//! Returns a reference to the game lobby associated with this session.
	idLobby&			 GetGameLobby() { return gameLobby; }

	//! Returns a constant reference to the game lobby associated with this local session.
	const idLobby&		 GetGameLobby() const { return gameLobby; }

	//! Returns a reference to the game state lobby used during in-game sessions
	idLobby&			 GetGameStateLobby() { return gameStateLobby; }

	//! Returns a constant reference to the game state lobby object
	const idLobby&		 GetGameStateLobby() const { return gameStateLobby; }

	//! Returns a reference to the lobby that is currently acting as the game state lobby, either the game lobby or the state lobby based on a configuration flag
	idLobby&			 GetActingGameStateLobby();

	//! Returns a reference to the lobby that is currently being used for the game state
	const idLobby&		 GetActingGameStateLobby() const;

	//! Returns the active platform lobby based on the current session state
	idLobby*			 GetActivePlatformLobby();

	//! Returns a pointer to the active platform lobby based on the current session state.
	const idLobby*		 GetActivePlatformLobby() const;

	//! Returns a pointer to a lobby object based on the specified lobby type.
	idLobby*			 GetLobbyFromType( idLobby::lobbyType_t lobbyType );

	//! Returns a reference to the party lobby base object.
	virtual idLobbyBase& GetPartyLobbyBase() { return partyLobby; }

	//! Returns a reference to the game lobby base object.
	virtual idLobbyBase& GetGameLobbyBase() { return gameLobby; }

	//! Returns a reference to the lobby base object representing the current acting game state.
	virtual idLobbyBase& GetActingGameStateLobbyBase() { return GetActingGameStateLobby(); }

	//! Returns the base version of the active platform lobby for the current session
	virtual idLobbyBase& GetActivePlatformLobbyBase();

	//! Returns a lobby base reference from a lobby user ID.
	virtual idLobbyBase& GetLobbyFromLobbyUserID( lobbyUserID_t lobbyUserID );

	//! Sets the local session state to the specified new state.
	void				 SetState( state_t newState );

	//! Processes incoming network packets and routes them to the appropriate lobby handlers based on session ID.
	bool				 HandlePackets();

	//! Handles displaying a voice restriction dialog when needed
	void				 HandleVoiceRestrictionDialog();

	//! Sets whether the session was dropped by the host.
	void				 SetDroppedByHost( bool dropped ) { droppedByHost = dropped; }

	//! Returns whether the session was dropped by the host.
	bool				 GetDroppedByHost() { return droppedByHost; }

public:
	int storedPeer;
	int storedMsgType;

protected:
	static const char*								  stateToString[NUM_STATES];

	state_t											  localState;
	uint32											  sessionOptions;

	connectType_t									  connectType;
	int												  connectTime;

	idLobby											  partyLobby;
	idLobby											  gameLobby;
	idLobby											  gameStateLobby;
	idLobbyStub										  stubLobby; // We use this when we request the active lobby when we are not in a lobby (i.e at press start)

	int												  currentID; // The host used this to send out a unique id to all users so we can identify them

	class idVoiceChatMgr*							  voiceChat;
	int												  lastVoiceSendtime;
	bool											  hasShownVoiceRestrictionDialog;

	pendingInviteMode_t								  pendingInviteMode;
	int												  pendingInviteDevice;
	lobbyConnectInfo_t								  pendingInviteConnectInfo;

	bool											  isSysUIShowing;

	idDict											  titleStorageVars;
	bool											  titleStorageLoaded;

	int												  showMigratingInfoStartTime;

	int												  nextGameCoalesceTime;
	bool											  gameLobbyWasCoalesced;
	int												  numFullSnapsReceived;

	bool											  flushedStats;

	int												  loadingID;

	bool											  inviteInfoRequested;

	idSaveGameProcessorSaveFiles*					  processorSaveFiles;
	idSaveGameProcessorLoadFiles*					  processorLoadFiles;
	idSaveGameProcessorDelete*						  processorDelete;
	idSaveGameProcessorEnumerateGames*				  processorEnumerate;

	idStr											  currentSaveSlot;
	saveGameHandle_t								  enumerationHandle;

	//! Handles the lobby state when the local session is the host of a party.
	bool											  State_Party_Lobby_Host();

	//! Returns true if the peer lobby state is active and packets are handled successfully.
	bool											  State_Party_Lobby_Peer();

	//! Handles the game lobby state when the local session is hosting.
	bool											  State_Game_Lobby_Host();

	//! Handles the game lobby peer state logic for session management.
	bool											  State_Game_Lobby_Peer();

	//! Handles the game state lobby host logic, managing transitions and loading states.
	bool											  State_Game_State_Lobby_Host();

	//! Checks if all lobby members have joined and starts the game if the timeout has expired
	bool											  State_Game_State_Lobby_Peer();

	//! Checks if the game loading state is complete and transitions to the in-game state when all peers have loaded.
	bool											  State_Loading();

	//! Returns true if the session is currently in the in-game state.
	bool											  State_InGame();

	//! Attempts to find or create a match for the game session.
	bool											  State_Find_Or_Create_Match();

	//! Creates a party lobby and transitions to the party lobby host state.
	bool											  State_Create_And_Move_To_Party_Lobby();

	//! Creates a game lobby and transitions to the game lobby host state.
	bool											  State_Create_And_Move_To_Game_Lobby();

	//! Creates and transitions to the game state lobby.
	bool											  State_Create_And_Move_To_Game_State_Lobby();

	//! Connects and moves to a party lobby state
	bool											  State_Connect_And_Move_To_Party();

	//! Connects and moves to the game state lobby.
	bool											  State_Connect_And_Move_To_Game();

	//! Attempts to connect and move to the game state lobby
	bool											  State_Connect_And_Move_To_Game_State();
	bool											  State_Finalize_Connect();

	//! Checks if the session is in a busy state, handling bandwidth challenges and packet processing.
	bool											  State_Busy();

	// -----------------------
	// Downloadable Content
	// -----------------------
	static const int								  MAX_CONTENT_PACKAGES = 16;

	idStaticList<contentData_t, MAX_CONTENT_PACKAGES> downloadedContent;
	bool											  marketplaceHasNewContent;

	/*!
		\class idSessionLocal::idQueuePacket
		\brief A packet queue management class for handling network communication.
	*/
	class idQueuePacket
	{
	public:
		byte					   data[idPacketProcessor::MAX_FINAL_PACKET_SIZE];
		lobbyAddress_t			   address;
		int						   size;
		int						   time;
		bool					   dedicated;
		idQueueNode<idQueuePacket> queueNode;
	};

	idBlockAlloc<idQueuePacket, 64, TAG_NETWORKING>	  packetAllocator;
	idQueue<idQueuePacket, &idQueuePacket::queueNode> sendQueue;
	idQueue<idQueuePacket, &idQueuePacket::queueNode> recvQueue;

	float											  upstreamDropRate; // instant rate in B/s at which we are dropping packets due to simulated upstream saturation
	int												  upstreamDropRateTime;

	float											  upstreamQueueRate; // instant rate in B/s at which queued packets are coming out after local buffering due to upstream saturation
	int												  upstreamQueueRateTime;

	int												  queuedBytes;

	int												  waitingOnGameStateMembersToLeaveTime;
	int												  waitingOnGameStateMembersToJoinTime;

	//! Processes and sends queued network packets that are ready to be transmitted.
	void											  TickSendQueue();

	/*!
		\brief Queues a packet for sending with the specified parameters

		This function allocates a packet from the packet allocator, initializes its fields with the provided parameters, copies the data into the packet, and adds it to the specified queue for later
	   processing

		\param queue The queue to which the packet will be added
		\param time The time when the packet should be sent
		\param to The destination address for the packet
		\param data Pointer to the data to be sent in the packet
		\param size The size of the data to be sent
		\param dedicated Flag indicating if the packet is for a dedicated server
	*/
	void						   QueuePacket( idQueue<idQueuePacket, &idQueuePacket::queueNode>& queue, int time, const lobbyAddress_t& to, const void* data, int size, bool dedicated );

	/*!
		\brief Reads a raw network packet from the receive queue if one is available for the specified time.

		This function attempts to retrieve a packet from the receive queue that is ready to be processed at the given time. It populates the provided buffer with packet data and returns true if a
	   packet was successfully read. The function checks if the packet is ready based on the provided time and ensures the packet size does not exceed the maximum allowed size. If no packet is
	   available or the packet is not ready, the function returns false.

		\param time The time at which the packet should be considered ready
		\param from Reference to store the address of the packet's sender
		\param data Pointer to the buffer where packet data will be copied
		\param size Reference to store the actual size of the received packet
		\param outDedicated Reference to store whether the packet came from a dedicated server
		\param maxSize Maximum allowed size of the packet data buffer
		\return True if a packet was successfully read from the queue, false otherwise
	*/
	bool						   ReadRawPacketFromQueue( int time, lobbyAddress_t& from, void* data, int& size, bool& outDedicated, int maxSize );

	/*!
		\brief Sends a raw network packet to the specified address with optional queuing and bandwidth limiting.

		The function sends a raw packet to the given address using either direct transmission or queuing based on network simulation parameters. It supports forced upstream bandwidth limiting and
	   queuing, as well as simulated network latency. If network simulation is enabled through console variables, the packet may be dropped or queued depending on current bandwidth usage and queue
	   limits. The function handles both dedicated and non-dedicated packet transmission paths.

		\param to The destination address for the packet
		\param data Pointer to the raw packet data to send
		\param size Size of the packet data in bytes
		\param dedicated Flag indicating whether to use dedicated port for transmission
	*/
	void						   SendRawPacket( const lobbyAddress_t& to, const void* data, int size, bool dedicated );

	/*!
		\brief Reads a raw network packet from the session, handling both dedicated and non-dedicated ports with optional latency simulation.

		This function attempts to read a raw packet from the session's network ports. It processes packets from both dedicated and non-dedicated ports in a round-robin fashion to ensure fair
	   distribution. The function supports latency simulation via a console variable and can queue packets for delayed processing if latency is configured. It returns true if a packet was successfully
	   read or retrieved from the queue, and false otherwise. The function updates the 'from' parameter with the source address of the packet, 'data' with the packet data, 'size' with the packet size,
	   and 'outDedicated' to indicate if the packet came from a dedicated port.

		\param from The lobby address of the packet's sender
		\param data Buffer to store the packet data
		\param size Reference to the packet size, updated upon return
		\param outDedicated Reference to a boolean indicating if the packet came from a dedicated port
		\param maxSize Maximum size of the data buffer
		\return True if a packet was successfully read or retrieved from the queue, false otherwise
	*/
	bool						   ReadRawPacket( lobbyAddress_t& from, void* data, int& size, bool& outDedicated, int maxSize );

	//! Connects to a specified lobby and transitions to the appropriate state based on the lobby type.
	void						   ConnectAndMoveToLobby( idLobby& lobby, const lobbyConnectInfo_t& connectInfo, bool fromInvite );

	/*!
		\brief Handles disconnection messages from a host, determining whether the disconnection was due to a connection attempt denial or a general disconnect.

		This function processes goodbye messages from a host, distinguishing between cases where a connection attempt was denied and cases where a connected peer was disconnected. When a connection
	   attempt is denied, it handles the failure by attempting to connect to the next available peer or creating a match. When a peer is disconnected, it evaluates whether the disconnection should
	   trigger a switch to a party lobby or simply designate a new host.

		\param lobby The lobby from which the goodbye message originated
		\param peerNum The index of the peer in the lobby that sent the message
		\param remoteAddress The network address of the host sending the message
		\param msgType The type of goodbye message, indicating the reason for disconnection
	*/
	void						   GoodbyeFromHost( idLobby& lobby, int peerNum, const lobbyAddress_t& remoteAddress, int msgType );

	//! Writes leaderboard data to a bit message
	void						   WriteLeaderboardToMsg( idBitMsg& msg, const leaderboardDefinition_t* leaderboard, const column_t* stats );

	//! Sends leaderboard statistics for a specific player to the lobby
	void						   SendLeaderboardStatsToPlayer( lobbyUserID_t lobbyUserID, const leaderboardDefinition_t* leaderboard, const column_t* stats );

	//! Receives and processes leaderboard statistics for a specific player from a message.
	void						   RecvLeaderboardStatsForPlayer( idBitMsg& msg );

	//! Reads leaderboard data from a network message and returns the corresponding leaderboard definition.
	const leaderboardDefinition_t* ReadLeaderboardFromMsg( idBitMsg& msg, column_t* stats );

	//! Returns true if the session requires a persistent master user profile
	bool						   RequirePersistentMaster();

	virtual idNetSessionPort&	   GetPort( bool dedicated = false )																								 = 0;
	virtual idLobbyBackend*		   CreateLobbyBackend( const idMatchParameters& p, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType )					 = 0;
	virtual idLobbyBackend*		   FindLobbyBackend( const idMatchParameters& p, int numPartyUsers, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType ) = 0;
	virtual idLobbyBackend*		   JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo, idLobbyBackend::lobbyBackendType_t lobbyType )						 = 0;
	virtual void				   DestroyLobbyBackend( idLobbyBackend* lobby )																						 = 0;
	virtual void				   PumpLobbies()																													 = 0;
	virtual bool				   GetLobbyAddressFromNetAddress( const netadr_t& netAddr, lobbyAddress_t& outAddr ) const											 = 0;
	virtual bool				   GetNetAddressFromLobbyAddress( const lobbyAddress_t& lobbyAddress, netadr_t& outNetAddr ) const									 = 0;

	//! Handles a dedicated server query request from a remote address and responds with session information if joining is allowed.
	void						   HandleDedicatedServerQueryRequest( lobbyAddress_t& remoteAddr, idBitMsg& msg, int msgType );

	//! Handles a dedicated server query acknowledgment message from a remote address.
	void						   HandleDedicatedServerQueryAck( lobbyAddress_t& remoteAddr, idBitMsg& msg );

	//! Clears the migration state for both party and game lobbies.
	void						   ClearMigrationState();

	//! Finalizes a match and returns the lobby to its appropriate state.
	void						   EndMatchInternal( bool premature = false );

	//! Handles cleanup and communication when a match has finished.
	void						   MatchFinishedInternal();

	//! Ends the current match and prepares for migration.
	void						   EndMatchForMigration();

	//! Sends voice chat audio data to connected players in the active lobby.
	void						   SendVoiceAudio();

	//! Processes out-of-band voice audio data received from a remote lobby member.
	void						   HandleOobVoiceAudio( const lobbyAddress_t& from, const idBitMsg& msg );

	//! Configures voice chat groups to match the teams in the current lobby
	void						   SetVoiceGroupsToTeams();

	//! Clears all voice chat groups in the session.
	void						   ClearVoiceGroups();

	//! Initializes and starts the party and game lobby sessions.
	void						   StartSessions();

	//! Ends the current party and game sessions if they exist.
	void						   EndSessions();

	//! Sets the joinable state of party and game lobbies.
	void						   SetLobbiesAreJoinable( bool joinable );

	//! Ends all session activities and transitions to the idle state.
	void						   MoveToMainMenu();

	//! Waits for a lobby creation operation to complete and handles failure cases.
	bool						   WaitOnLobbyCreate( idLobby& lobby );

	//! Checks if the connection to the service has been lost based on a timeout period
	bool						   DetectDisconnectFromService( bool cancelAndShowMsg );

	//! Handles a failed connection attempt by managing lobby state and displaying appropriate dialogs based on connection type and error conditions.
	void						   HandleConnectionFailed( idLobby& lobby, bool wasFull );
	void						   ConnectToNextSearchResultFailed( idLobby& lobby );

	//! Handles the connection process and transition to a lobby based on the current session state.
	bool						   HandleConnectAndMoveToLobby( idLobby& lobby );

	//! Verifies that the snapshot initial state is correctly reset for all connected peers
	void						   VerifySnapshotInitialState();

	//! Computes the next game coalesce time based on session settings and random factors.
	void						   ComputeNextGameCoalesceTime();

	//! Initializes the loading process for the game session.
	void						   StartLoading();

	//! Determines whether the session should have an active party lobby based on the current lobby state and match flags.
	bool						   ShouldHavePartyLobby();

	//! Validates active lobbies based on the current session state and requirements.
	void						   ValidateLobbies();

	//! Validates the state of a lobby and handles failure cases by moving to the main menu and showing appropriate dialogs.
	void						   ValidateLobby( idLobby& lobby );

	//! Reads title storage data from a buffer and parses it into session variables.
	void						   ReadTitleStorage( void* buffer, int bufferLen );

	//! Parses DLC information from a memory buffer into an idDict object.
	bool						   ReadDLCInfo( idDict& dlcInfo, void* buffer, int bufferLen );

	idSessionCallbacks*			   sessionCallbacks;

	int							   offlineTransitionTimerStart;

	bool						   droppedByHost;
};

/*!
	\class idSessionLocalCallbacks
	\brief Manages local session callbacks and lobby interactions for networked game sessions.

	Provides a callback interface for managing lobby states, session transitions, and network communications within a local game session. Handles operations such as lobby creation, migration, peer
   management, and voice chat integration. The class serves as a bridge between lobby-specific functionality and the broader session management system, enabling coordinated behavior across different
   lobby types including party and game lobbies. It supports session state transitions, match parameter handling, and network message processing for lobby communications. The implementation delegates
   many operations to underlying session local objects while maintaining the interface contract defined by the base class.

*/
class idSessionLocalCallbacks : public idSessionCallbacks
{
public:
	//! Initializes the session local callbacks with the provided session local object.
	idSessionLocalCallbacks( idSessionLocal* sessionLocal_ ) { sessionLocal = sessionLocal_; }

	//! Returns a reference to the local session's party lobby
	virtual idLobby&				  GetPartyLobby() { return sessionLocal->GetPartyLobby(); }

	//! Returns a reference to the game lobby associated with this session.
	virtual idLobby&				  GetGameLobby() { return sessionLocal->GetGameLobby(); }

	//! Returns a reference to the lobby currently managing the game state.
	virtual idLobby&				  GetActingGameStateLobby() { return sessionLocal->GetActingGameStateLobby(); }

	//! Returns a lobby instance corresponding to the specified lobby type
	virtual idLobby*				  GetLobbyFromType( idLobby::lobbyType_t lobbyType ) { return sessionLocal->GetLobbyFromType( lobbyType ); }

	//! Returns a unique player ID incrementing from the current session ID.
	virtual int						  GetUniquePlayerId() const { return sessionLocal->currentID++; }

	//! Returns a reference to the sign-in manager associated with the local session.
	virtual idSignInManagerBase&	  GetSignInManager() { return *sessionLocal->signInManager; }

	/*!
		\brief Sends a raw packet to the specified address using the session local implementation

		This function forwards the raw packet sending request to the underlying session local implementation. It takes a destination address, packet data, size information, and a flag indicating
	   whether to use a direct port for transmission. The function is part of the session callbacks interface and is used to handle low-level network communications within the lobby system.

		\param to The destination address for the packet
		\param data Pointer to the packet data to send
		\param size Size of the packet data in bytes
		\param useDirectPort Flag indicating whether to use a direct port for transmission
	*/
	virtual void					  SendRawPacket( const lobbyAddress_t& to, const void* data, int size, bool useDirectPort ) { sessionLocal->SendRawPacket( to, data, size, useDirectPort ); }

	//! Determines whether the session can transition to host state for the specified lobby.
	virtual bool					  BecomingHost( idLobby& lobby );

	//! Updates the local session state when the lobby becomes the host.
	virtual void					  BecameHost( idLobby& lobby );

	//! Returns true if the lobby is transitioning to a peer state during game migration, false otherwise.
	virtual bool					  BecomingPeer( idLobby& lobby );

	//! Handles the transition of a lobby to a peer state when the lobby type is a game lobby.
	virtual void					  BecamePeer( idLobby& lobby );

	//! Handles the failure of a game migration by resetting lobby state and updating session state
	virtual void					  FailedGameMigration( idLobby& lobby );

	//! Handles the completion of a lobby migration process and displays appropriate dialogs to the user.
	virtual void					  MigrationEnded( idLobby& lobby );

	/*!
		\brief Handles the goodbye message from a host in a lobby session

		This function is called when a peer receives a goodbye message from a host, indicating that a peer is leaving the lobby. It forwards the message to the session local implementation for further
	   processing.

		\param lobby Reference to the lobby the peer is leaving
		\param peerNum The peer number of the peer leaving
		\param remoteAddress The network address of the peer leaving
		\param msgType The type of goodbye message being sent
	*/
	virtual void					  GoodbyeFromHost( idLobby& lobby, int peerNum, const lobbyAddress_t& remoteAddress, int msgType );

	//! Retrieves the session options bitmask from the local session.
	virtual uint32					  GetSessionOptions() { return sessionLocal->sessionOptions; }

	//! Checks if any peer in the party or game lobby has the specified remote address.
	virtual bool					  AnyPeerHasAddress( const lobbyAddress_t& remoteAddress ) const;

	//! Retrieves the current state of the session.
	virtual idSession::sessionState_t GetState() const { return sessionLocal->GetState(); }

	//! Clears all migration state for party and game lobbies.
	virtual void					  ClearMigrationState()
	{
		GetPartyLobby().ResetAllMigrationState();
		GetGameLobby().ResetAllMigrationState();
	}

	//! Ends the current match, optionally marking it as premature.
	virtual void			EndMatchInternal( bool premature = false ) { sessionLocal->EndMatchInternal( premature ); }

	//! Handles incoming leaderboard statistics from a network message for the local session.
	virtual void			RecvLeaderboardStats( idBitMsg& msg );

	//! Handles the reception of a full network snapshot to transition the session into the INGAME state.
	virtual void			ReceivedFullSnap();

	//! Leaves the game lobby and transitions to the party lobby state.
	virtual void			LeaveGameLobby();

	//! Prepares for the process of picking a new host in the lobby.
	virtual void			PrePickNewHost( idLobby& lobby, bool forceMe, bool inviteOldHost );

	//! PreMigrateInvite is called before accepting an invite to a migrated session and returns false to ignore the invite
	virtual bool			PreMigrateInvite( idLobby& lobby );

	//! Handles out-of-band voice audio data received from a specific lobby address.
	virtual void			HandleOobVoiceAudio( const lobbyAddress_t& from, const idBitMsg& msg ) { sessionLocal->HandleOobVoiceAudio( from, msg ); }

	//! Connects to and moves to a specified lobby type using the provided connection information.
	virtual void			ConnectAndMoveToLobby( idLobby::lobbyType_t destLobbyType, const lobbyConnectInfo_t& connectInfo, bool waitForPartyOk );

	//! Returns the voice chat manager instance associated with the local session.
	virtual idVoiceChatMgr* GetVoiceChat() { return sessionLocal->voiceChat; }

	//! Handles a server query request from a remote address with the provided message and message type.
	virtual void			HandleServerQueryRequest( lobbyAddress_t& remoteAddr, idBitMsg& msg, int msgType );

	//! Handles a server query acknowledgment message from a remote address
	virtual void			HandleServerQueryAck( lobbyAddress_t& remoteAddr, idBitMsg& msg );

	//! Handles updates to peer match parameters in a lobby session.
	virtual void			HandlePeerMatchParamUpdate( int peer, int msg );

	//! Creates a lobby backend instance for the specified match parameters, skill level, and lobby type.
	virtual idLobbyBackend* CreateLobbyBackend( const idMatchParameters& p, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType );

	/*!
		\brief Finds or creates a lobby backend for matching based on the provided parameters

		This function serves as a helper method to locate an appropriate lobby backend for matchmaking operations. It delegates the actual search or creation to the session local object, passing along
	   the match parameters, number of party users, skill level, and desired lobby type. The function is used during lobby search operations and helps in managing lobby connections and matchmaking
	   processes.

		\param p Match parameters defining the search criteria
		\param numPartyUsers Number of users in the party
		\param skillLevel Average skill level of the party members
		\param lobbyType Type of lobby backend to find or create
		\return Pointer to the found or created lobby backend instance, or NULL if unsuccessful
	*/
	virtual idLobbyBackend* FindLobbyBackend( const idMatchParameters& p, int numPartyUsers, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType );

	//! Joins a lobby backend using the provided connection information and lobby type
	virtual idLobbyBackend* JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo, idLobbyBackend::lobbyBackendType_t lobbyType );

	//! Destroys a lobby backend instance
	virtual void			DestroyLobbyBackend( idLobbyBackend* lobby );

	idSessionLocal*			sessionLocal;
};
