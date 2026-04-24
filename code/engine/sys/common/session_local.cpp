/*
================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION/NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright 2010 id Software LLC, a ZeniMax Media company. All Rights Reserved.
================================================================================================
*/

/*
================================================================================================

Contains the windows implementation of the network session

================================================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "../../framework/Common_local.h"
#include "../sys_session_local.h"
#include "../sys_stats.h"
#include "../sys_savegame.h"
#include "../sys_lobby_backend_direct.h"
#include "../sys_voicechat.h"
#include "achievements.h"
// #include "win_local.h"

/*
========================
Global variables
========================
*/

extern idCVar net_port;

class idLobbyToSessionCBLocal;

/*!
	\class idSessionLocalWin
	\brief Platform-specific session management implementation for Windows.

	Provides Windows-specific implementation of session management functionality including voice chat, lobby handling, server listing, and leaderboard operations. Implements platform-specific
   initialization and shutdown procedures, system UI management, and network session port handling. Supports multiplayer session operations such as inviting friends, joining parties, and managing
   online presence. Handles platform-specific input event processing and system UI state management. Contains methods for leaderboard upload and download operations with callback mechanisms. The class
   serves as a concrete implementation of the session local interface tailored for Windows platform requirements.

*/
class idSessionLocalWin : public idSessionLocal
{
	friend class idLobbyToSessionCBLocal;

public:
	//! Initializes a new instance of the idSessionLocalWin class for Windows platform.
	idSessionLocalWin();

	//! Destructor for the idSessionLocalWin class that cleans up voice chat and lobby to session callback resources.
	virtual ~idSessionLocalWin();

	//! Initializes the Windows session local implementation.
	virtual void				Initialize();

	//! Shuts down the Windows session local system
	virtual void				Shutdown();

	//! Initializes the voice chat system if it exists.
	virtual void				InitializeSoundRelatedSystems();

	//! Shuts down the voice chat system if it is initialized.
	virtual void				ShutdownSoundRelatedSystems();

	//! Handles platform-specific pumping operations for the session.
	virtual void				PlatformPump();

	//! Invites friends to join the current session.
	virtual void				InviteFriends();

	//! Invites the party to a lobby session.
	virtual void				InviteParty();

	//! Displays the party sessions interface.
	virtual void				ShowPartySessions();

	//! Displays the system marketplace user interface.
	virtual void				ShowSystemMarketplaceUI() const;

	//! Lists servers using the provided callback mechanism
	virtual void				ListServers( const idCallback& callback );

	//! Cancels the ongoing server listing process.
	virtual void				CancelListServers();

	//! Returns the number of servers available.
	virtual int					NumServers() const;

	//! Returns a pointer to the server information at the specified index, or NULL if the index is invalid.
	virtual const serverInfo_t* ServerInfo( int i ) const;

	//! Connects to a server at the specified index
	virtual void				ConnectToServer( int i );

	//! Displays the gamer card UI for a specific server
	virtual void				ShowServerGamerCardUI( int i );

	//! Displays the gamer card UI for a specified lobby user
	virtual void				ShowLobbyUserGamerCardUI( lobbyUserID_t lobbyUserID );

	//! Displays the online sign-in UI for the session.
	virtual void				ShowOnlineSignin()
	{
	}

	//! Updates the rich presence information for the session.
	virtual void UpdateRichPresence()
	{
	}

	//! Checks voice privileges for the local session.
	virtual void CheckVoicePrivileges()
	{
	}

	//! Processes a system input event for the local session window.
	virtual bool  ProcessInputEvent( const sysEvent_t* ev );

	//! Returns true if the system UI is currently showing
	virtual bool  IsSystemUIShowing() const;

	//! Sets the system UI showing state flag
	virtual void  SetSystemUIShowing( bool show );

	//! Handles a bootable invite with the specified lobby ID.
	virtual void  HandleBootableInvite( int64 lobbyId = 0 );

	//! Clears the bootable invite state.
	virtual void  ClearBootableInvite();

	//! Clears any pending invite state in the session.
	virtual void  ClearPendingInvite();

	//! Returns false indicating no pending bootable invite is available.
	virtual bool  HasPendingBootableInvite();

	//! Sets the parameters for a disc swap multiplayer invite.
	virtual void  SetDiscSwapMPInvite( void* parm );

	//! Returns null pointer for disc swap MP invite parameters.
	virtual void* GetDiscSwapMPInviteParms();

	//! Enumerates downloadable content for the session.
	virtual void  EnumerateDownloadableContent();

	//! Handles a server query request from a remote address with the provided message and message type.
	virtual void  HandleServerQueryRequest( lobbyAddress_t& remoteAddr, idBitMsg& msg, int msgType );

	//! Handles a server query acknowledgment message from a remote address.
	virtual void  HandleServerQueryAck( lobbyAddress_t& remoteAddr, idBitMsg& msg );

	/*!
		\brief Uploads leaderboard statistics for a specific user to the game's leaderboard system

		This function is responsible for uploading leaderboard data including user statistics and an optional attachment to the game's leaderboard system. It takes a lobby user ID, leaderboard
	   definition, array of statistics, and an optional memory file attachment. The function is part of the session management system and supports online leaderboard functionality.

		\param lobbyUserID Identifier for the lobby user whose statistics are being uploaded
		\param leaderboard Pointer to the leaderboard definition structure specifying which leaderboard to upload to
		\param stats Pointer to array of column statistics to be uploaded
		\param attachment Optional pointer to a memory file containing additional data to be attached to the leaderboard entry
	*/
	virtual void  LeaderboardUpload( lobbyUserID_t lobbyUserID, const leaderboardDefinition_t* leaderboard, const column_t* stats, const idFile_Memory* attachment = NULL );

	/*!
		\brief Initiates an asynchronous download of leaderboard data for a specific user

		This function starts the process of downloading leaderboard information from the game session for a specified user. It retrieves leaderboard entries starting from a given rank and continues
	   for a specified number of rows. The results are passed back through the provided callback mechanism.

		\param sessionUserIndex Index of the user in the session whose leaderboard data is being requested
		\param leaderboard Pointer to the leaderboard definition specifying which leaderboard to download
		\param startingRank The rank to start downloading entries from
		\param numRows The number of rows to download starting from the starting rank
		\param callback Callback object to be invoked when the download completes
	*/
	virtual void  LeaderboardDownload( int sessionUserIndex, const leaderboardDefinition_t* leaderboard, int startingRank, int numRows, const idLeaderboardCallback& callback );

	//! Initiates downloading a leaderboard attachment for a specific user and leaderboard.
	virtual void  LeaderboardDownloadAttachment( int sessionUserIndex, const leaderboardDefinition_t* leaderboard, int64 attachmentID );

	//! Sets the relative score for a lobby user within a specific team
	virtual void  SetLobbyUserRelativeScore( lobbyUserID_t lobbyUserID, int relativeScore, int team )
	{
	}

	//! Flushes the leaderboard data
	virtual void			  LeaderboardFlush();

	//! Returns a reference to the network session port, creating it if necessary.
	virtual idNetSessionPort& GetPort( bool dedicated = false );

	//! Creates a lobby backend for hosting a game session with the specified parameters.
	virtual idLobbyBackend*	  CreateLobbyBackend( const idMatchParameters& p, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType );

	/*!
		\brief Finds or creates a lobby backend for searching matches based on the provided parameters.

		This function is responsible for locating or creating a lobby backend instance that will be used to find matches based on the specified match parameters, party size, skill level, and lobby
	   type. It first creates a new lobby backend of the specified type, then initiates the finding process with the given parameters. The function returns the lobby backend instance that is ready to
	   search for matches.

		\param p The match parameters to use for finding a lobby
		\param numPartyUsers The number of users in the party
		\param skillLevel The average skill level of the party members
		\param lobbyType The type of lobby backend to create and use for finding matches
		\return A pointer to the lobby backend instance that has been initialized to start finding matches.
	*/
	virtual idLobbyBackend*	  FindLobbyBackend( const idMatchParameters& p, int numPartyUsers, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType );

	//! Creates and joins a lobby backend using the provided connection information and lobby type
	virtual idLobbyBackend*	  JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo, idLobbyBackend::lobbyBackendType_t lobbyType );

	//! Destroys a lobby backend instance and cleans up its resources.
	virtual void			  DestroyLobbyBackend( idLobbyBackend* lobbyBackend );

	//! Processes all active lobby backends to update their state.
	virtual void			  PumpLobbies();

	//! Placeholder function for joining after a swap operation.
	virtual void			  JoinAfterSwap( void* joinID );

	//! Returns false indicating that retrieving lobby address from network address is not supported.
	virtual bool			  GetLobbyAddressFromNetAddress( const netadr_t& netAddr, lobbyAddress_t& outAddr ) const;

	//! Converts a lobby address to a network address and returns true if successful.
	virtual bool			  GetNetAddressFromLobbyAddress( const lobbyAddress_t& lobbyAddress, netadr_t& outNetAddr ) const;

public:
	//! Connects to a game server specified by the IP address and optional port from command line arguments.
	void Connect_f( const idCmdArgs& args );

private:
	//! Initializes the network port for the session if it is not already open.
	void						EnsurePort();

	//! Creates a lobby backend instance for the specified lobby type
	idLobbyBackend*				CreateLobbyInternal( idLobbyBackend::lobbyBackendType_t lobbyType );

	idArray<idLobbyBackend*, 3> lobbyBackends;

	idNetSessionPort			port;
	bool						canJoinLocalHost;

	idLobbyToSessionCBLocal*	lobbyToSessionCB;
};

idSessionLocalWin sessionLocalWin;
idSession*		  session = &sessionLocalWin;

/*!
	\class idLobbyToSessionCBLocal
	\brief Provides local lobby backend functionality for session management.
*/
class idLobbyToSessionCBLocal : public idLobbyToSessionCB
{
public:
	//! Constructs an idLobbyToSessionCBLocal object with the specified session local window.
	idLobbyToSessionCBLocal( idSessionLocalWin* sessionLocalWin_ ) :
		sessionLocalWin( sessionLocalWin_ )
	{
	}

	//! Checks if local host can be joined
	virtual bool CanJoinLocalHost() const
	{
		sessionLocalWin->EnsurePort();
		return sessionLocalWin->canJoinLocalHost;
	}

	//! Returns the lobby backend of the specified type from the local session
	virtual class idLobbyBackend* GetLobbyBackend( idLobbyBackend::lobbyBackendType_t type ) const
	{
		return sessionLocalWin->lobbyBackends[type];
	}

private:
	idSessionLocalWin* sessionLocalWin;
};

idLobbyToSessionCBLocal lobbyToSessionCBLocal( &sessionLocalWin );
idLobbyToSessionCB*		lobbyToSessionCB = &lobbyToSessionCBLocal;

/*!
	\class idVoiceChatMgrWin
	\brief Manages voice chat functionality for Windows platform with specific talker registration and data handling.
*/
class idVoiceChatMgrWin : public idVoiceChatMgr
{
public:
	//! Returns false indicating no local chat data is available
	virtual bool GetLocalChatDataInternal( int talkerIndex, byte* data, int& dataSize )
	{
		return false;
	}

	//! Submits incoming chat data for a specified talker index.
	virtual void SubmitIncomingChatDataInternal( int talkerIndex, const byte* data, int dataSize )
	{
	}

	//! Returns false indicating no talker has data
	virtual bool TalkerHasData( int talkerIndex )
	{
		return false;
	}

	//! Registers a talker internally using the provided index
	virtual bool RegisterTalkerInternal( int index )
	{
		return true;
	}

	//! Unregisters a talker internal index
	virtual void UnregisterTalkerInternal( int index )
	{
	}
};

idSessionLocalWin::idSessionLocalWin()
{
	signInManager	 = new( TAG_SYSTEM ) idSignInManagerWin;
	saveGameManager	 = new( TAG_SAVEGAMES ) idSaveGameManager();
	voiceChat		 = new( TAG_SYSTEM ) idVoiceChatMgrWin();
	lobbyToSessionCB = new( TAG_SYSTEM ) idLobbyToSessionCBLocal( this );

	canJoinLocalHost = false;

	lobbyBackends.Zero();
}

idSessionLocalWin::~idSessionLocalWin()
{
	delete voiceChat;
	delete lobbyToSessionCB;
}

void idSessionLocalWin::Initialize()
{
	idSessionLocal::Initialize();

	// The shipping path doesn't load title storage
	// Instead, we inject values through code which is protected through steam DRM
	titleStorageVars.Set( "MAX_PLAYERS_ALLOWED", "8" );
	titleStorageLoaded = true;

	// First-time check for downloadable content once game is launched
	EnumerateDownloadableContent();

	GetPartyLobby().Initialize( idLobby::TYPE_PARTY, sessionCallbacks );
	GetGameLobby().Initialize( idLobby::TYPE_GAME, sessionCallbacks );
	GetGameStateLobby().Initialize( idLobby::TYPE_GAME_STATE, sessionCallbacks );

	achievementSystem = new( TAG_SYSTEM ) idAchievementSystemWin();
	achievementSystem->Init();
}

void idSessionLocalWin::Shutdown()
{
	NET_VERBOSE_PRINT( "NET: Shutdown\n" );
	idSessionLocal::Shutdown();

	MoveToMainMenu();

	// Wait until we fully shutdown
	while( localState != STATE_IDLE ) {
		Pump();
	}

	if( achievementSystem != NULL ) {
		achievementSystem->Shutdown();
		delete achievementSystem;
		achievementSystem = NULL;
	}
}

void idSessionLocalWin::InitializeSoundRelatedSystems()
{
	if( voiceChat != NULL ) {
		voiceChat->Init( NULL );
	}
}

void idSessionLocalWin::ShutdownSoundRelatedSystems()
{
	if( voiceChat != NULL ) {
		voiceChat->Shutdown();
	}
}

void idSessionLocalWin::PlatformPump()
{
}

void idSessionLocalWin::InviteFriends()
{
}

void idSessionLocalWin::InviteParty()
{
}

void idSessionLocalWin::ShowPartySessions()
{
}

void idSessionLocalWin::ShowSystemMarketplaceUI() const
{
}

void idSessionLocalWin::ListServers( const idCallback& callback )
{
	ListServersCommon();
}

void idSessionLocalWin::CancelListServers()
{
}

int idSessionLocalWin::NumServers() const
{
	return 0;
}

const serverInfo_t* idSessionLocalWin::ServerInfo( int i ) const
{
	return NULL;
}

void idSessionLocalWin::ConnectToServer( int i )
{
}

void idSessionLocalWin::Connect_f( const idCmdArgs& args )
{
	if( args.Argc() < 2 ) {
		idLib::Printf( "Usage: Connect to IP. Use IP:Port to specify port (e.g. 10.0.0.1:1234) \n" );
		return;
	}

	Cancel();

	if( signInManager->GetMasterLocalUser() == NULL ) {
		signInManager->RegisterLocalUser( 0 );
	}

	lobbyConnectInfo_t connectInfo;

	Sys_StringToNetAdr( args.Argv( 1 ), &connectInfo.netAddr, true );
	// DG: don't use net_port to select port to connect to
	//     the port can be specified in the command, else the default port is used
	if( connectInfo.netAddr.port == 0 ) {
		connectInfo.netAddr.port = 27015;
	}
	// DG end

	ConnectAndMoveToLobby( GetPartyLobby(), connectInfo, false );
}

//! Connects to the specified IP address.
CONSOLE_COMMAND( connect, "Connect to the specified IP", NULL )
{
	sessionLocalWin.Connect_f( args );
}

void idSessionLocalWin::ShowServerGamerCardUI( int i )
{
}

void idSessionLocalWin::ShowLobbyUserGamerCardUI( lobbyUserID_t lobbyUserID )
{
}

bool idSessionLocalWin::ProcessInputEvent( const sysEvent_t* ev )
{
	if( GetSignInManager().ProcessInputEvent( ev ) ) {
		return true;
	}
	return false;
}

bool idSessionLocalWin::IsSystemUIShowing() const
{
	// DG: pausing here when window is out of focus like originally done on windows is hacky
	// it's done with com_pause now.
	return isSysUIShowing;
}

void idSessionLocalWin::SetSystemUIShowing( bool show )
{
	isSysUIShowing = show;
}

void idSessionLocalWin::HandleServerQueryRequest( lobbyAddress_t& remoteAddr, idBitMsg& msg, int msgType )
{
	NET_VERBOSE_PRINT( "HandleServerQueryRequest from %s\n", remoteAddr.ToString() );
}

void idSessionLocalWin::HandleServerQueryAck( lobbyAddress_t& remoteAddr, idBitMsg& msg )
{
	NET_VERBOSE_PRINT( "HandleServerQueryAck from %s\n", remoteAddr.ToString() );
}

void idSessionLocalWin::ClearBootableInvite()
{
}

void idSessionLocalWin::ClearPendingInvite()
{
}

void idSessionLocalWin::HandleBootableInvite( int64 lobbyId )
{
}

bool idSessionLocalWin::HasPendingBootableInvite()
{
	return false;
}

void idSessionLocalWin::SetDiscSwapMPInvite( void* parm )
{
}

void* idSessionLocalWin::GetDiscSwapMPInviteParms()
{
	return NULL;
}

void idSessionLocalWin::EnumerateDownloadableContent()
{
}

void idSessionLocalWin::LeaderboardUpload( lobbyUserID_t lobbyUserID, const leaderboardDefinition_t* leaderboard, const column_t* stats, const idFile_Memory* attachment )
{
}

void idSessionLocalWin::LeaderboardFlush()
{
}

void idSessionLocalWin::LeaderboardDownload( int sessionUserIndex, const leaderboardDefinition_t* leaderboard, int startingRank, int numRows, const idLeaderboardCallback& callback )
{
}

void idSessionLocalWin::LeaderboardDownloadAttachment( int sessionUserIndex, const leaderboardDefinition_t* leaderboard, int64 attachmentID )
{
}

void idSessionLocalWin::EnsurePort()
{
	// Init the port using reqular sockets
	if( port.IsOpen() ) {
		return; // Already initialized
	}

	if( port.InitPort( net_port.GetInteger(), false ) ) {
		// TODO: what about canJoinLocalHost when running two instances with different net_port values?
		canJoinLocalHost = false;
	} else {
		// Assume this is another instantiation on the same machine, and just init using any available port
		port.InitPort( PORT_ANY, false );
		canJoinLocalHost = true;
	}
}

idNetSessionPort& idSessionLocalWin::GetPort( bool dedicated )
{
	EnsurePort();
	return port;
}

idLobbyBackend* idSessionLocalWin::CreateLobbyBackend( const idMatchParameters& p, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType )
{
	idLobbyBackend* lobbyBackend = CreateLobbyInternal( lobbyType );
	lobbyBackend->StartHosting( p, skillLevel, lobbyType );
	return lobbyBackend;
}

idLobbyBackend* idSessionLocalWin::FindLobbyBackend( const idMatchParameters& p, int numPartyUsers, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType )
{
	idLobbyBackend* lobbyBackend = CreateLobbyInternal( lobbyType );
	lobbyBackend->StartFinding( p, numPartyUsers, skillLevel );
	return lobbyBackend;
}

idLobbyBackend* idSessionLocalWin::JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo, idLobbyBackend::lobbyBackendType_t lobbyType )
{
	idLobbyBackend* lobbyBackend = CreateLobbyInternal( lobbyType );
	lobbyBackend->JoinFromConnectInfo( connectInfo );
	return lobbyBackend;
}

void idSessionLocalWin::DestroyLobbyBackend( idLobbyBackend* lobbyBackend )
{
	assert( lobbyBackend != NULL );
	assert( lobbyBackends[lobbyBackend->GetLobbyType()] == lobbyBackend );

	lobbyBackends[lobbyBackend->GetLobbyType()] = NULL;

	lobbyBackend->Shutdown();
	delete lobbyBackend;
}

void idSessionLocalWin::PumpLobbies()
{
	assert( lobbyBackends[idLobbyBackend::TYPE_PARTY] == NULL || lobbyBackends[idLobbyBackend::TYPE_PARTY]->GetLobbyType() == idLobbyBackend::TYPE_PARTY );
	assert( lobbyBackends[idLobbyBackend::TYPE_GAME] == NULL || lobbyBackends[idLobbyBackend::TYPE_GAME]->GetLobbyType() == idLobbyBackend::TYPE_GAME );
	assert( lobbyBackends[idLobbyBackend::TYPE_GAME_STATE] == NULL || lobbyBackends[idLobbyBackend::TYPE_GAME_STATE]->GetLobbyType() == idLobbyBackend::TYPE_GAME_STATE );

	// Pump lobbyBackends
	for( int i = 0; i < lobbyBackends.Num(); i++ ) {
		if( lobbyBackends[i] != NULL ) {
			lobbyBackends[i]->Pump();
		}
	}
}

idLobbyBackend* idSessionLocalWin::CreateLobbyInternal( idLobbyBackend::lobbyBackendType_t lobbyType )
{
	EnsurePort();
	idLobbyBackend* lobbyBackend = new( TAG_NETWORKING ) idLobbyBackendDirect();

	lobbyBackend->SetLobbyType( lobbyType );

	assert( lobbyBackends[lobbyType] == NULL );
	lobbyBackends[lobbyType] = lobbyBackend;

	return lobbyBackend;
}

void idSessionLocalWin::JoinAfterSwap( void* joinID )
{
}

bool idSessionLocalWin::GetLobbyAddressFromNetAddress( const netadr_t& netAddr, lobbyAddress_t& outAddr ) const
{
	return false;
}

bool idSessionLocalWin::GetNetAddressFromLobbyAddress( const lobbyAddress_t& lobbyAddress, netadr_t& outNetAddr ) const
{
	return false;
}
