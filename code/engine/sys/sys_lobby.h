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

#include "sys_lobby_backend.h"

#define INVALID_LOBBY_USER_NAME " " // Used to be "INVALID" but Sony might not like that.

class idSessionCallbacks;
class idDebugGraph;

/*!
	\class idLobby
	\brief A class that manages multiplayer lobby functionality including user connections, messaging, and session state.

	The idLobby class provides comprehensive management of multiplayer game sessions, handling user connections, peer-to-peer communication, and session state transitions. It supports hosting and
   joining game lobbies with various network configurations and manages the lifecycle of connected users. The class tracks connection states, handles reliable and unreliable messaging between peers,
   and provides mechanisms for session management including migration and bandwidth optimization. It also includes functionality for managing user settings, team assignments, and synchronization
   between local and remote user states. The implementation supports different lobby types and handles the complexities of networked multiplayer session management including connection establishment,
   maintenance, and cleanup. The class maintains internal state tracking for connected peers and lobby users, providing methods for querying connection status, user information, and session
   parameters.

*/
class idLobby : public idLobbyBase
{
public:
	//! Constructs a new idLobby instance and initializes its internal state.
	idLobby();

	//! Destructor for the idLobby class that cleans up allocated memory for multiplayer networking support.
	~idLobby();

	enum lobbyType_t { TYPE_PARTY = 0, TYPE_GAME = 1, TYPE_GAME_STATE = 2, TYPE_INVALID = 0xff };

	enum lobbyState_t { STATE_IDLE, STATE_CREATE_LOBBY_BACKEND, STATE_SEARCHING, STATE_OBTAINING_ADDRESS, STATE_CONNECT_HELLO_WAIT, STATE_FINALIZE_CONNECT, STATE_FAILED, NUM_STATES };

	enum failedReason_t {
		FAILED_UNKNOWN,
		FAILED_CONNECT_FAILED,
		FAILED_MIGRATION_CONNECT_FAILED,
	};

	//! Initializes the lobby with the specified session type and callbacks.
	void		 Initialize( lobbyType_t sessionType_, class idSessionCallbacks* callbacks );

	//! Initializes the lobby system to host a new game session with the specified match parameters.
	void		 StartHosting( const idMatchParameters& parms );

	//! Initializes the lobby search process with the specified match parameters
	void		 StartFinding( const idMatchParameters& parms_ );

	//! Processes lobby state updates and handles peer connectivity checks.
	void		 Pump();

	//! Processes a limited number of snapshot acknowledgments from the queue.
	void		 ProcessSnapAckQueue();

	//! Shuts down the lobby session and cleans up associated resources.
	void		 Shutdown( bool retainMigrationInfo = false, bool skipGoodbye = false );

	//! Processes an incoming network packet from a remote address using the specified session ID and message data.
	void		 HandlePacket( lobbyAddress_t& remoteAddress, idBitMsg fragMsg, idPacketProcessor::sessionId_t sessionID );

	//! Returns the current state of the lobby.
	lobbyState_t GetState() { return state; }

	//! Checks if the lobby has any active peers.
	virtual bool HasActivePeers() const;

	//! Returns true if the lobby has no free slots available.
	virtual bool IsLobbyFull() const { return NumFreeSlots() == 0; }

	//! Returns the number of free slots available in the lobby.
	int			 NumFreeSlots() const;

public:
	enum reliablePlayerToPlayer_t {
		// RELIABLE_PLAYER_TO_PLAYER_VOICE_EVENT,
		RELIABLE_PLAYER_TO_PLAYER_GAME_DATA,
		// Game messages would be reserved here in the same way that RELIABLE_GAME_DATA is.
		// I'm worried about using up the 0xff values we have for reliable type, so I'm not
		// going to reserve anything here just yet.
		NUM_RELIABLE_PLAYER_TO_PLAYER,
	};

	enum reliableType_t {
		RELIABLE_HELLO,						// host to peer : connection established
		RELIABLE_USER_CONNECTED,			// host to peer : a new session user connected
		RELIABLE_USER_DISCONNECTED,			// host to peer : a session user disconnected
		RELIABLE_START_LOADING,				// host to peer : peer should begin loading the map
		RELIABLE_LOADING_DONE,				// peer to host : finished loading map
		RELIABLE_IN_GAME,					// peer to host : first full snap received, in game now
		RELIABLE_SNAPSHOT_ACK,				// peer to host : got a snapshot
		RELIABLE_RESOURCE_ACK,				// peer to host : got some new resources
		RELIABLE_CONNECT_AND_MOVE_TO_LOBBY, // host to peer : connect to this server
		RELIABLE_PARTY_CONNECT_OK,			// host to peer
		RELIABLE_PARTY_LEAVE_GAME_LOBBY,	// host to peer : leave game lobby
		RELIABLE_MATCH_PARMS,				// host to peer : update in match parms
		RELIABLE_UPDATE_MATCH_PARMS,		// peer to host : peer updating match parms

		// User join in progress msg's (join in progress for the party/game lobby, not inside a match)
		RELIABLE_USER_CONNECT_REQUEST, // peer to host: local user wants to join session in progress
		RELIABLE_USER_CONNECT_DENIED,  // host to peer: user join session in progress denied (not enough slots)

		// User leave in progress msg's (leave in progress for the party/game lobby, not inside a match)
		RELIABLE_USER_DISCONNECT_REQUEST, // peer to host: request host to remove user from session

		RELIABLE_KICK_PLAYER, // host to peer : kick a player

		RELIABLE_MATCHFINISHED,		 // host to peer - Match is in post looking at score board
		RELIABLE_ENDMATCH,			 // host to peer - End match, and go to game lobby
		RELIABLE_ENDMATCH_PREMATURE, // host to peer - End match prematurely, and go to game lobby (onl possible in unrated/custom games)

		RELIABLE_SESSION_USER_MODIFIED, // peer to host : user changed something (emblem, name, etc)
		RELIABLE_UPDATE_SESSION_USER,	// host to peers : inform all peers of the change

		RELIABLE_HEADSET_STATE, // * to * : headset state change for user
		RELIABLE_VOICE_STATE,	// * to * : voice state changed for user pair (mute, unmute, etc)
		RELIABLE_PING,			// * to * : send host->peer, then reflected
		RELIABLE_PING_VALUES,	// host to peers : ping data from lobbyUser_t for everyone

		RELIABLE_BANDWIDTH_VALUES, // peer to host: data back about bandwidth test

		RELIABLE_ARBITRATE,	   // host to peer : start arbitration
		RELIABLE_ARBITRATE_OK, // peer to host : ack arbitration request

		RELIABLE_POST_STATS, // host to peer : here, write these stats now (hacky)

		RELIABLE_MIGRATION_GAME_DATA, // host to peers: game data to use incase of a migration

		RELIABLE_START_MATCH_GAME_LOBBY_HOST, // game lobby host to game state lobby host: start the match, since all players are in

		RELIABLE_DUMMY_MSG, // used as a placeholder for old removed msg's

		RELIABLE_PLAYER_TO_PLAYER_BEGIN,
		// use reliablePlayerToPlayer_t
		RELIABLE_PLAYER_TO_PLAYER_END = RELIABLE_PLAYER_TO_PLAYER_BEGIN + NUM_RELIABLE_PLAYER_TO_PLAYER,

		// * to * : misc reliable game data above this
		RELIABLE_GAME_DATA = RELIABLE_PLAYER_TO_PLAYER_END
	};

	// JGM: Reliable type in packet is a byte and there are a lot of reliable game messages.
	// Feel free to bump this up since it's arbitrary anyway, but take a look at gameReliable_t.
	// At the moment, both Doom and Rage have around 32 gameReliable_t values.
	compile_time_assert( RELIABLE_GAME_DATA < 64 );

	static const char* stateToString[NUM_STATES];

	// Consts

	static const int   PEER_HEARTBEAT_IN_SECONDS = 5; // Make sure something was sent every 5 seconds, so we don't time out
	static const int   CONNECT_REQUEST_FREQUENCY_IN_SECONDS =
		5; // Frequency at which we resend a request to connect to a server (will increase in frequency over time down to MIN_CONNECT_FREQUENCY_IN_SECONDS)
	static const int MIN_CONNECT_FREQUENCY_IN_SECONDS = 1; // Min frequency of connection attempts
	static const int MAX_CONNECT_ATTEMPTS			  = 5;
	static const int BANDWIDTH_REPORTING_MAX		  = 10240; // make bps to report receiving (clamp if higher). For quantizing
	static const int BANDWIDTH_REPORTING_BITS		  = 16;	   // number of bits to use for bandwidth reporting
	static const int MAX_BPS_HISTORY				  = 32;	   // size of outgoing bps history to maintain for each client

	static const int MAX_SNAP_SIZE		= idPacketProcessor::MAX_MSG_SIZE;
	static const int MAX_SNAPSHOT_QUEUE = 64;

	static const int OOB_HELLO			 = 0;
	static const int OOB_GOODBYE		 = 1;
	static const int OOB_GOODBYE_W_PARTY = 2;
	static const int OOB_GOODBYE_FULL	 = 3;
	static const int OOB_RESOURCE_LIST	 = 4;
	static const int OOB_VOICE_AUDIO	 = 5;

	static const int OOB_MATCH_QUERY	 = 6;
	static const int OOB_MATCH_QUERY_ACK = 7;

	static const int OOB_SYSTEMLINK_QUERY = 8;

	static const int OOB_MIGRATE_INVITE = 9;

	static const int OOB_BANDWIDTH_TEST = 10;

	enum connectionState_t {
		CONNECTION_FREE		   = 0, // Free peer slot
		CONNECTION_CONNECTING  = 1, // Waiting for response from host for initial connection
		CONNECTION_ESTABLISHED = 2, // Connection is established and active
	};

	struct peer_t {
		peer_t()
		{
			loaded				  = false;
			inGame				  = false;
			networkChecksum		  = 0;
			lastSnapTime		  = 0;
			snapHz				  = 0.0f;
			numResources		  = 0;
			lastHeartBeat		  = 0;
			connectionState		  = CONNECTION_FREE;
			packetProc			  = NULL;
			snapProc			  = NULL;
			nextPing			  = 0; // do it asap
			lastPingRtt			  = 0;
			sessionID			  = idPacketProcessor::SESSION_ID_INVALID;
			startResourceLoadTime = 0;
			nextThrottleCheck	  = 0;
			maxSnapQueueSize	  = 0;
			throttledSnapRate	  = 0;
			pauseSnapshots		  = false;

			receivedBps			 = -1.0f;
			maxSnapBps			 = -1.0f;
			receivedThrottle	 = 0;
			receivedThrottleTime = 0;

			throttleSnapsForXSeconds = 0;
			recoverPing				 = 0;
			failedPingRecoveries	 = 0;
			rightBeforeSnapsPing	 = 0;

			bandwidthTestLastSendTime		= 0;
			bandwidthSequenceNum			= 0;
			bandwidthTestBytes				= 0;
			bandwidthChallengeStartSendTime = 0;
			bandwidthChallengeResults		= false;
			bandwidthChallengeSendComplete	= false;

			numSnapsSent = 0;

			ResetConnectState();
		};

		//! Resets the connection state variables to their default values.
		void ResetConnectState()
		{
			lastResourceTime = 0;
			lastSnapTime	 = 0;
			snapHz = lastProcTime	= 0;
			lastInBandProcTime		= 0;
			lastFragmentSendTime	= 0;
			needToSubmitPendingSnap = false;
			lastSnapJobTime			= true;
			startResourceLoadTime	= 0;

			receivedBps				 = -1.0;
			maxSnapBps				 = -1.0f;
			receivedThrottle		 = 0;
			receivedThrottleTime	 = 0;
			throttleSnapsForXSeconds = 0;
			recoverPing				 = 0;
			failedPingRecoveries	 = 0;
			rightBeforeSnapsPing	 = 0;

			bandwidthTestLastSendTime		= 0;
			bandwidthSequenceNum			= 0;
			bandwidthTestBytes				= 0;
			bandwidthChallengeStartSendTime = 0;
			bandwidthChallengeResults		= false;
			bandwidthChallengeSendComplete	= false;
			memset( sentBpsHistory, 0, sizeof( sentBpsHistory ) );
			receivedBpsIndex = 0;

			debugGraphs.Clear();
		}

		//! Resets all peer data by clearing the connection state and match data.
		void ResetAllData()
		{
			ResetConnectState();
			ResetMatchData();
		}

		//! Resets all match data fields and the snapshot processor for a peer in the lobby.
		void ResetMatchData()
		{
			loaded					= false;
			networkChecksum			= 0;
			inGame					= false;
			numResources			= 0;
			needToSubmitPendingSnap = false;
			throttledSnapRate		= 0;
			maxSnapQueueSize		= 0;
			receivedBpsIndex		= -1;
			numSnapsSent			= 0;
			pauseSnapshots			= false;

			// Reset the snapshot processor
			if( snapProc != NULL ) { snapProc->Reset( false ); }
		}

		//! Prints peer timing and state information to the console
		void Print()
		{
			idLib::Printf( "   lastResourceTime: %d\n", lastResourceTime );
			idLib::Printf( "   lastSnapTime: %d\n", lastSnapTime );
			idLib::Printf( "   lastProcTime: %d\n", lastProcTime );
			idLib::Printf( "   lastInBandProcTime: %d\n", lastInBandProcTime );
			idLib::Printf( "   lastFragmentSendTime: %d\n", lastFragmentSendTime );
			idLib::Printf( "   needToSubmitPendingSnap: %d\n", needToSubmitPendingSnap );
			idLib::Printf( "   lastSnapJobTime: %d\n", lastSnapJobTime );
		}

		//! Returns true if the peer is active and connected.
		bool						   IsActive() const { return connectionState != CONNECTION_FREE; }

		//! Returns true if the peer has an established connection.
		bool						   IsConnected() const { return connectionState == CONNECTION_ESTABLISHED; }

		//! Returns the connection state of the peer.
		connectionState_t			   GetConnectionState() const;

		connectionState_t			   connectionState;
		bool						   loaded;		 // true if this peer has finished loading the map
		bool						   inGame;		 // true if this peer received the first snapshot, and is in-game
		int							   lastSnapTime; // Last time a snapshot was sent on the network to this peer
		float						   snapHz;
		int							   lastProcTime;		 // Used to determine when a packet was processed for sending to this peer
		int							   lastInBandProcTime;	 // Last time a in-band packet was processed for sending
		int							   lastFragmentSendTime; // Last time a fragment was sent out (fragments are processed msg's, waiting to be fully sent)
		// DG: use int instead of long for 64bit compatibility
		unsigned int				   networkChecksum; // Checksum used to determine if a peer loaded the network resources the EXACT same as the server did
		// DG end
		int							   pauseSnapshots;

		lobbyAddress_t				   address;

		int							   numResources; // number of network resources we know the peer has

		idPacketProcessor*			   packetProc;	// Processes packets for this peer
		idSnapshotProcessor*		   snapProc;	// Processes snapshots for this peer
		idStaticList<idDebugGraph*, 4> debugGraphs; //

		int							   lastResourceTime; // Used to throttle the sending of resources

		int							   lastHeartBeat;
		int							   nextPing; // next Sys_Milliseconds when I'll send this peer a RELIABLE_PING
		int							   lastPingRtt;
		bool						   needToSubmitPendingSnap;
		int							   lastSnapJobTime; // Last time a snapshot was sent to the joblist for this peer

		int							   startResourceLoadTime; // Used to determine how long a peer has been loading resources

		int							   maxSnapQueueSize;  // how big has the snap queue gotten?
		int							   throttledSnapRate; // effective snap rate for this peer
		int							   nextThrottleCheck;

		int							   numSnapsSent;

		float						   sentBpsHistory[MAX_BPS_HISTORY];
		int							   receivedBpsIndex;

		float						   receivedBps; // peer's reported bps (they tell us their effective downstream)
		float						   maxSnapBps;
		float						   receivedThrottle;	 // amount of accumlated time this client has been lagging behind
		int							   receivedThrottleTime; // last time we did received based throttle calculations

		int							   throttleSnapsForXSeconds;
		int							   recoverPing;
		int							   failedPingRecoveries;
		int							   rightBeforeSnapsPing;

		int							   bandwidthChallengeStartSendTime; // time we sent first packet of bw challenge to this peer
		int							   bandwidthTestLastSendTime;		// last time in MS we sent them a bw challenge packet
		int							   bandwidthTestBytes;				// used to measure number of bytes we sent them
		int							   bandwidthSequenceNum;			// number of challenge sequences we sent them
		bool						   bandwidthChallengeResults;		// we got results back
		bool						   bandwidthChallengeSendComplete;	// we finished sending everything

		idPacketProcessor::sessionId_t sessionID;
	};

	//! Returns a string identifier for the lobby type.
	const char* GetLobbyName()
	{
		switch( lobbyType ) {
			case TYPE_PARTY:
				return "TYPE_PARTY";
			case TYPE_GAME:
				return "TYPE_GAME";
			case TYPE_GAME_STATE:
				return "TYPE_GAME_STATE";
		}

		return "LOBBY_INVALID";
	}

	//! Finds an open user slot for a bot and returns the user ID.
	virtual lobbyUserID_t AllocLobbyUserSlotForBot( const char* botName );

	//! Removes a bot user from the lobby user list and frees the associated user slot.
	virtual void		  RemoveBotFromLobbyUserList( lobbyUserID_t lobbyUserID );

	//! Checks whether the lobby user with the specified ID is a bot.
	virtual bool		  GetLobbyUserIsBot( lobbyUserID_t lobbyUserID ) const;

	//! Returns the number of users in the lobby
	virtual int			  GetNumLobbyUsers() const { return userList.Num(); }

	//! Returns the number of active lobby users by counting non-disconnected users
	virtual int			  GetNumActiveLobbyUsers() const;

	//! Returns true if all connected peers in the lobby are in the game.
	virtual bool		  AllPeersInGame() const;

	//! Returns a pointer to the lobby user at the specified index, or NULL if the index is out of bounds.
	lobbyUser_t*		  GetLobbyUser( int index ) { return ( index >= 0 && index < GetNumLobbyUsers() ) ? userList[index] : NULL; }

	//! Returns a pointer to the lobby user at the specified index, or NULL if the index is invalid
	const lobbyUser_t*	  GetLobbyUser( int index ) const { return ( index >= 0 && index < GetNumLobbyUsers() ) ? userList[index] : NULL; }

	//! Checks if a lobby user at the specified index is connected.
	virtual bool		  IsLobbyUserConnected( int index ) const { return !IsLobbyUserDisconnected( index ); }

	//! Returns the peer index associated with a given lobby user ID
	virtual int			  PeerIndexFromLobbyUser( lobbyUserID_t lobbyUserID ) const;

	//! Returns the time in milliseconds since the last packet was received from a peer
	virtual int			  GetPeerTimeSinceLastPacket( int peerIndex ) const;

	//! Returns the peer index corresponding to the host in the lobby.
	virtual int			  PeerIndexForHost() const { return host; }

	//! Returns the index of the peer on the host, or -1 if the current instance is the host.
	virtual int			  PeerIndexOnHost() const
	{
		return peerIndexOnHost; // Returns -1 if we are the host
	}

	//! Returns the match parameters for the lobby.
	virtual const idMatchParameters& GetMatchParms() const { return parms; }

	//! Returns the lobby type that should be used for the current game state
	lobbyType_t						 GetActingGameStateLobbyType() const;

	//! Returns true if the current session is hosted by this client
	bool							 IsHost() const { return isHost; }

	//! Returns true if the current lobby instance represents a peer connection to a host.
	bool							 IsPeer() const
	{
		if( host == -1 ) {
			return false; // Can't possibly be a peer if we haven't setup a host
		}
		assert( !IsHost() );
		return peers[host].IsConnected();
	}

	//! Checks if the connecting peer is present in the lobby.
	bool IsConnectingPeer() const
	{
		if( host == -1 ) {
			return false; // Can't possibly be a peer if we haven't setup a host
		}
		assert( !IsHost() );
		return peers[host].connectionState == CONNECTION_CONNECTING;
	}

	//! Returns true if the lobby is running as either a host or a peer.
	bool IsRunningAsHostOrPeer() const { return IsHost() || IsPeer(); }

	//! Checks if the lobby is currently active by verifying if it's running as host or peer.
	bool IsLobbyActive() const { return IsRunningAsHostOrPeer(); }

	struct reliablePlayerToPlayerHeader_t {
		int fromSessionUserIndex;
		int toSessionUserIndex;

		//! Constructs a new reliable player-to-player header with default session user indices set to -1.
		reliablePlayerToPlayerHeader_t();

		//! Reads reliable player-to-player header data from a message and translates user IDs to session indices.
		bool Read( idLobby* lobby, idBitMsg& msg );

		//! Writes lobby user IDs to a message for reliable player-to-player communication.
		bool Write( idLobby* lobby, idBitMsg& msg );
	};

	//! Returns the total instantaneous outgoing bandwidth in bytes per second.
	int GetTotalOutgoingRate();

	// private:
public: // Turning this on for now, for the sake of getting this up and running to see where things are
		//! Handles the idle state of the lobby system
	void		 State_Idle();

	//! Handles the backend creation of a lobby state
	void		 State_Create_Lobby_Backend();

	//! Handles the searching state for the lobby by validating the backend state and processing search results.
	void		 State_Searching();

	//! Handles the state when obtaining the address for a lobby connection
	void		 State_Obtaining_Address();

	//! Finalizes the lobby connection process and updates the lobby state based on the backend state and migration information.
	void		 State_Finalize_Connect();

	//! Waits for a lobby connection hello response while managing connection attempts and timeouts.
	void		 State_Connect_Hello_Wait();

	//! Sets the lobby state to the specified new state.
	void		 SetState( lobbyState_t newState );

	//! Initializes the lobby creation process.
	void		 StartCreating();

	//! Finds a peer in the lobby by remote address and session ID, with optional session ID ignoring.
	int			 FindPeer( const lobbyAddress_t& remoteAddress, idPacketProcessor::sessionId_t sessionID, bool ignoreSessionID = false );

	//! Finds a peer in the lobby by comparing the remote address with peer addresses.
	int			 FindAnyPeer( const lobbyAddress_t& remoteAddress ) const;

	//! Finds the index of the first inactive peer in the lobby, or -1 if all peers are active.
	int			 FindFreePeer() const;

	//! Adds a new peer to the lobby or returns the index of an existing peer with the same address and session ID.
	int			 AddPeer( const lobbyAddress_t& remoteAddress, idPacketProcessor::sessionId_t sessionID );

	//! Disconnects a peer from the session if the current player is the host.
	void		 DisconnectPeerFromSession( int p );

	//! Sets the connection state of a peer in the lobby
	void		 SetPeerConnectionState( int p, connectionState_t newState, bool skipGoodbye = false );

	//! Disconnects all peers from the lobby session.
	void		 DisconnectAllPeers();

	/*!
		\brief Sends a reliable message to lobby users while optionally processing it locally and filtering recipients based on a session user mask.

		This function sends a reliable message to all connected lobby users in the current session, with the ability to process the message locally and filter out specific users based on a session
	   user mask. The message is queued for transmission to peers only once per peer, even if multiple lobby users are associated with the same peer. The function ensures that the message has not been
	   read before, and asserts this condition. It performs checks to ensure the peer is connected and valid before queuing the message.

		\param type Message type identifier, must be less than 256
		\param msg The message to be sent, which must not have been read previously
		\param callReceiveReliable If true, the message is processed locally before being sent
		\param sessionUserMask Mask specifying which lobby users to exclude from receiving the message
		\throws assertion failures if message type exceeds 255 or if the message has been partially read before calling this function
	*/
	virtual void SendReliable( int type, idBitMsg& msg, bool callReceiveReliable = true, peerMask_t sessionUserMask = MAX_UNSIGNED_TYPE( peerMask_t ) );

	//! Sends a reliable message to a specific lobby user, handling local delivery if necessary.
	virtual void SendReliableToLobbyUser( lobbyUserID_t lobbyUserID, int type, idBitMsg& msg );

	//! Sends a reliable message to the lobby host, executing locally if called on the host.
	virtual void SendReliableToHost( int type, idBitMsg& msg );

	//! Sends a goodbye message to a remote address, with optional full lobby indication.
	void		 SendGoodbye( const lobbyAddress_t& remoteAddress, bool wasFull = false );

	//! Queues a reliable message for transmission to a specified peer.
	void		 QueueReliableMessage( int peerNum, byte type ) { QueueReliableMessage( peerNum, type, NULL, 0 ); }

	/*!
		\brief Queues a reliable message for transmission to a specific peer in the lobby

		This function attempts to queue a reliable message for transmission to a specified peer in the lobby. It first validates the peer index and checks if the peer has an established connection. If
	   the peer's reliable message queue is already overloaded, a warning is printed. If the message cannot be queued due to queue overflow, the peer is disconnected from the session or the entire
	   session is shut down depending on whether the local instance is the host

		\param p index of the peer to send the message to
		\param type type identifier for the reliable message
		\param data pointer to the message data to send
		\param dataLen length of the message data in bytes
	*/
	void		 QueueReliableMessage( int p, byte type, const byte* data, int dataLen );

	//! Returns the number of connected peers in the lobby.
	virtual int	 GetNumConnectedPeers() const;

	//! Returns the count of connected peers who are currently in the game
	virtual int	 GetNumConnectedPeersInGame() const;

	//! Sends match parameters to connected peers in a lobby if the current instance is the host.
	void		 SendMatchParmsToPeers();

	//! Checks if a given byte type falls within the range of reliable player-to-player message types.
	static bool	 IsReliablePlayerToPlayerType( byte type );

	//! Handles reliable player-to-player messages by processing or forwarding them based on the target player and connection role.
	void		 HandleReliablePlayerToPlayerMsg( int peerNum, idBitMsg& msg, int type );

	//! Handles reliable player-to-player messages in the lobby
	void		 HandleReliablePlayerToPlayerMsg( const reliablePlayerToPlayerHeader_t& info, idBitMsg& msg, int reliableType );

	//! Sends a connectionless packet to the specified remote address with the given type.
	void		 SendConnectionLess( const lobbyAddress_t& remoteAddress, byte type ) { SendConnectionLess( remoteAddress, type, NULL, 0 ); }

	/*!
		\brief Sends a connectionless message to a remote address using the lobby's packet processor

		This function prepares and sends a connectionless packet to a specified remote address. It takes raw message data, processes it through the lobby's packet processor to handle connectionless
	   communication, and then transmits the processed packet using the session callback. The function handles different packet routing based on the lobby type, specifically using a direct port for
	   game state communications.

		\param remoteAddress The network address of the remote system to send the message to
		\param type The type identifier for the connectionless message
		\param data Pointer to the raw message data to be sent
		\param dataLen Length of the message data in bytes
	*/
	void		 SendConnectionLess( const lobbyAddress_t& remoteAddress, byte type, const byte* data, int dataLen );

	//! Sends a connection request to the lobby host.
	void		 SendConnectionRequest();

	//! Initiates a connection to a lobby using the provided connection information and invite flag.
	void		 ConnectTo( const lobbyConnectInfo_t& connectInfo, bool fromInvite );

	//! Handles a goodbye message received from a peer in a lobby session.
	void		 HandleGoodbyeFromPeer( int peerNum, lobbyAddress_t& remoteAddress, int msgType );

	//! Handles a failed connection attempt by shutting down and setting the appropriate failure state.
	void		 HandleConnectionAttemptFailed();

	//! Attempts to connect to the next available search result in a game lobby.
	bool		 ConnectToNextSearchResult();

	//! Checks if the version checksum in the message matches the local checksum.
	bool		 CheckVersion( idBitMsg& msg, lobbyAddress_t peerAddress );

	//! Verifies that the number of users connecting through the message is valid and that there are enough free slots available in the lobby.
	bool		 VerifyNumConnectingUsers( idBitMsg& msg );

	//! Verifies that the lobby user IDs in the message do not conflict with existing user IDs.
	bool		 VerifyLobbyUserIDs( idBitMsg& msg );

	//! Handles an initial peer connection request from a remote client.
	int			 HandleInitialPeerConnection( idBitMsg& msg, const lobbyAddress_t& peerAddress, int peerNum );

	//! Initializes the lobby state when the local user becomes the host.
	void		 InitStateLobbyHost();

	//! Sends lobby member information to all connected peers if the current instance is the party host.
	void		 SendMembersToLobby( lobbyType_t destLobbyType, const lobbyConnectInfo_t& connectInfo, bool waitForOtherMembers );

	//! Sends lobby member information to another lobby using the specified connection details.
	void		 SendMembersToLobby( idLobby& destLobby, bool waitForOtherMembers );

	/*!
		\brief Sends lobby connection information to a specific peer to connect to a game lobby.

		This function is responsible for sending lobby connection information to a specific peer in the lobby. It only performs the operation if the current instance is the party host. The function
	   constructs a message containing the destination lobby type and connection information, then queues a reliable message to be sent to the specified peer. This allows the peer to connect to the
	   designated lobby server. The function ensures the peer is connected before attempting to send the information and includes validation checks for the peer index.

		\param peerIndex Index of the peer in the peers array to send the lobby information to
		\param destLobbyType Type of the destination lobby the peer should connect to
		\param connectInfo Connection information structure containing details needed to connect to the lobby
		\param waitForOtherMembers Flag indicating whether to wait for other members before proceeding with the connection
	*/
	void		 SendPeerMembersToLobby( int peerIndex, lobbyType_t destLobbyType, const lobbyConnectInfo_t& connectInfo, bool waitForOtherMembers );

	//! Sends peer members to a lobby for the specified peer index and lobby type
	void		 SendPeerMembersToLobby( int peerIndex, lobbyType_t destLobbyType, bool waitForOtherMembers );

	//! Notifies the party members to leave the current game lobby.
	void		 NotifyPartyOfLeavingGameLobby();

	//! Returns the party token for the host in a party lobby.
	uint32		 GetPartyTokenAsHost();

	//! Displays debug network information for lobby peers on the screen
	virtual void DrawDebugNetworkHUD() const;

	//! Displays debug network information for connected peers in a lobby
	virtual void DrawDebugNetworkHUD2() const;

	//! Draws server snapshot metrics debug HUD when draw parameter is true
	virtual void DrawDebugNetworkHUD_ServerSnapshotMetrics( bool draw );

	//! Checks for peer timeouts and disconnects inactive peers from the lobby session.
	void		 CheckHeartBeats();

	//! Checks if the connection to the host is about to be lost based on heartbeat and queue status.
	bool		 IsLosingConnectionToHost() const;

	//! Returns true if the current lobby session is a migrated stats game
	bool		 IsMigratedStatsGame() const;

	//! Returns true if the game should relaunch a migrated session.
	bool		 ShouldRelaunchMigrationGame() const;

	//! Returns true if the migrating dialog should be shown based on the current lobby state
	bool		 ShouldShowMigratingDialog() const;

	//! Checks if the lobby is currently in a migration state.
	bool		 IsMigrating() const;

	// Pings
	struct pktPing_t {
		int timestamp;
	};

	//! Sends ping messages to connected peers if it is the host.
	void						   PingPeers();

	//! Sends ping values to all connected peers for UI display
	void						   SendPingValues();

	//! Updates ping information for lobby members.
	void						   PumpPings();

	//! Handles a reliable ping message from a peer.
	void						   HandleReliablePing( int p, idBitMsg& msg );

	//! Handles a ping reply from a peer by calculating and storing the round-trip time.
	void						   HandlePingReply( int p, const pktPing_t& ping );

	//! Processes incoming ping values from network messages and updates ping information for lobby users and the host.
	void						   HandlePingValues( idBitMsg& msg );

	//! Processes bandwidth test results received from a peer in a lobby.
	void						   HandleBandwidhTestValue( int p, idBitMsg& msg );

	//! Handles incoming game migration data from the server by storing raw data and resetting user migration information.
	void						   HandleMigrationGameData( idBitMsg& msg );

	//! Handles headset state changes for lobby users.
	void						   HandleHeadsetStateChange( int fromPeer, idBitMsg& msg );

	//! Sends another fragment of a packet to a specified peer if conditions allow.
	bool						   SendAnotherFragment( int p );

	//! Determines whether more data can be sent to a specific peer in the lobby.
	bool						   CanSendMoreData( int p );

	/*!
		\brief Processes outgoing network messages for a specific peer in the lobby

		This function handles the processing of outgoing network messages for a peer in the lobby system. It first verifies that the peer has established a connection before proceeding. If the peer is
	   not fully connected, it logs a message and returns without sending anything. The function also checks that no packet fragments are pending to be sent, raising a fatal error if there are. It
	   tracks timing information to manage when messages are processed and updates internal state regarding the last processing time. For non-out-of-band messages, it also updates the last in-band
	   processing time. The function then initializes a bit message with the provided data and delegates the actual processing to the peer's packet processor.

		\param p Index of the peer in the peers array
		\param data Pointer to the message data to be processed
		\param size Size of the message data in bytes
		\param isOOB Flag indicating if the message is out-of-band
		\param userData Additional user data to be passed to the packet processor
		\throws Fatal error if packet fragments are still pending to be sent
	*/
	void						   ProcessOutgoingMsg( int p, const void* data, int size, bool isOOB, int userData );

	//! Resends reliable network messages to a specified peer if conditions allow.
	void						   ResendReliables( int p );

	//! Processes network packets and maintains peer connections for the lobby.
	void						   PumpPackets();

	//! Updates the match parameters for the lobby if the current instance is the host.
	void						   UpdateMatchParms( const idMatchParameters& p );

	//! Encodes a session ID from a key value and the lobby type.
	idPacketProcessor::sessionId_t EncodeSessionID( uint32 key ) const;

	//! Decodes a session ID to extract a key
	void						   DecodeSessionID( idPacketProcessor::sessionId_t sessionID, uint32& key ) const;

	//! Generates a unique session ID for the lobby.
	idPacketProcessor::sessionId_t GenerateSessionID() const;

	//! Returns whether the given session ID can be used for in-band communication
	bool						   SessionIDCanBeUsedForInBand( idPacketProcessor::sessionId_t sessionID ) const;

	//! Increments a session ID while ensuring the new ID can be used for in-band communication.
	idPacketProcessor::sessionId_t IncrementSessionID( idPacketProcessor::sessionId_t sessionID ) const;

	//! Handles the acknowledgment of a hello message from a peer in the lobby system
	void						   HandleHelloAck( int p, idBitMsg& msg );

	//! Retrieves the user name associated with a given lobby user ID.
	virtual const char*			   GetLobbyUserName( lobbyUserID_t lobbyUserID ) const;

	//! Returns the weapon auto-reload setting for a specified lobby user
	virtual bool				   GetLobbyUserWeaponAutoReload( lobbyUserID_t lobbyUserID ) const;

	//! Retrieves the weapon auto-switch setting for a specific lobby user.
	virtual bool				   GetLobbyUserWeaponAutoSwitch( lobbyUserID_t lobbyUserID ) const;

	//! Returns the skin index selected by the specified lobby user.
	virtual int					   GetLobbyUserSkinIndex( lobbyUserID_t lobbyUserID ) const;

	//! Returns the level of the lobby user with the specified ID
	virtual int					   GetLobbyUserLevel( lobbyUserID_t lobbyUserID ) const;

	//! Retrieves the Quality of Service metric, specifically the ping in milliseconds, for a given lobby user.
	virtual int					   GetLobbyUserQoS( lobbyUserID_t lobbyUserID ) const;

	//! Retrieves the team number assigned to a specific lobby user.
	virtual int					   GetLobbyUserTeam( lobbyUserID_t lobbyUserID ) const;

	//! Sets the team number for a specified lobby user and updates the lobby session if the host.
	virtual bool				   SetLobbyUserTeam( lobbyUserID_t lobbyUserID, int teamNumber );

	//! Returns the party token associated with a specified lobby user ID
	virtual int					   GetLobbyUserPartyToken( lobbyUserID_t lobbyUserID ) const;

	//! Returns the player profile associated with the specified lobby user ID
	virtual idPlayerProfile*	   GetProfileFromLobbyUser( lobbyUserID_t lobbyUserID );

	//! Retrieves the local user associated with a given lobby user ID.
	virtual idLocalUser*		   GetLocalUserFromLobbyUser( lobbyUserID_t lobbyUserID );

	//! Returns the number of lobby users assigned to the specified team
	virtual int					   GetNumLobbyUsersOnTeam( int teamNumber ) const;

	//! Returns the name of a peer given its peer number.
	const char*					   GetPeerName( int peerNum ) const;

	//! Returns the user name of the host in the lobby
	virtual const char*			   GetHostUserName() const;

	//! Handles reliable messages from peers in a lobby system
	void						   HandleReliableMsg( int p, idBitMsg& msg, const lobbyAddress_t* remoteAddress = NULL );

	//! Initiates a bandwidth test on the host.
	void						   BeginBandwidthTest();

	//! Returns true if a bandwidth test has been started, otherwise false.
	bool						   BandwidthTestStarted();

	//! Updates the server bandwidth test by sending challenge packets to connected peers and handling test completion or timeouts.
	void						   ServerUpdateBandwidthTest();

	//! Updates the bandwidth test status on the client
	void						   ClientUpdateBandwidthTest();

	//! Throttles the snapshot rate for a specified peer if the current instance is the host.
	void						   ThrottlePeerSnapRate( int peerNum );

	//! Allocates and initializes a new lobby user from the provided defaults
	lobbyUser_t*				   AllocUser( const lobbyUser_t& defaults );

	//! Frees a lobby user by removing it from the active user list and adding it to the free users list
	void						   FreeUser( lobbyUser_t* user );

	//! Checks if a lobby user is valid and exists in the user list.
	bool						   VerifyUser( const lobbyUser_t* lobbyUser ) const;

	//! Frees all users in the lobby by iterating through the user list and releasing each user.
	void						   FreeAllUsers();

	//! Registers a user with the lobby systems
	void						   RegisterUser( lobbyUser_t* lobbyUser );

	//! Unregisters a lobby user from the session.
	void						   UnregisterUser( lobbyUser_t* lobbyUser );

	//! Determines if a user in the lobby is local to the host session
	bool						   IsSessionUserLocal( const lobbyUser_t* lobbyUser ) const;

	//! Checks if a lobby user at the specified index is local to the session.
	bool						   IsSessionUserIndexLocal( int i ) const;

	//! Returns the index of a lobby user identified by their lobby user ID, or -1 if not found.
	int							   GetLobbyUserIndexByID( lobbyUserID_t lobbyUserId, bool ignoreLobbyType = false ) const;

	//! Returns a lobby user by its unique ID, or NULL if not found
	lobbyUser_t*				   GetLobbyUserByID( lobbyUserID_t lobbyUserId, bool ignoreLobbyType = false );

	//! Creates a lobby user entry from a local user, copying gamer tag and initializing lobby-specific data.
	lobbyUser_t					   CreateLobbyUserFromLocalUser( const idLocalUser* localUser );

	//! Initializes session users from local users based on the online match requirement
	void						   InitSessionUsersFromLocalUsers( bool onlineMatch );

	//! Converts a local user handle to a lobby user index, returning -1 if no matching user is found.
	int							   GetLobbyUserIndexByLocalUserHandle( const localUserHandle_t localUserHandle ) const;

	//! Retrieves a local user pointer from a lobby user index if the user is local.
	idLocalUser*				   GetLocalUserFromLobbyUserIndex( int lobbyUserIndex );

	//! Returns the session user corresponding to the given local user, or NULL if no session user exists for that local user
	lobbyUser_t*				   GetSessionUserFromLocalUser( const idLocalUser* controller );

	//! Removes lobby users whose peer connections are disconnected
	void						   RemoveUsersWithDisconnectedPeers();

	//! Removes users from a session by their IDs and handles cleanup and notification.
	void						   RemoveSessionUsersByIDList( idList<lobbyUserID_t>& usersToRemoveByID );

	//! Sends a range of lobby users to connected peers, skipping a specified peer and excluding peers not in established connection state.
	void						   SendNewUsersToPeers( int skipPeer, int userStart, int numUsers );

	//! Sends microphone status of existing users to a newly added peer
	void						   SendPeersMicStatusToNewUsers( int peerNumber );

	//! Adds users from a message to the lobby user list
	void						   AddUsersFromMsg( idBitMsg& msg, int fromPeer );

	//! Sends session user updates to all peers and handles the update locally.
	void						   UpdateSessionUserOnPeers( idBitMsg& msg );

	//! Handles updating session user data from a message.
	void						   HandleUpdateSessionUser( idBitMsg& msg );

	//! Creates a user update message for the specified user index and writes it to the provided bit message.
	void						   CreateUserUpdateMessage( int userIndex, idBitMsg& msg );

	//! Updates the local session users in the lobby.
	void						   UpdateLocalSessionUsers();

	//! Returns the peer index for a given session user index.
	int							   PeerIndexForSessionUserIndex( int sessionUserIndex ) const;

	//! Handles a user connection failure by sending a failure acknowledgment back to the peer.
	void						   HandleUserConnectFailure( int p, idBitMsg& inMsg, int reliableType );

	//! Processes a message to disconnect users from the lobby.
	void						   ProcessUserDisconnectMsg( idBitMsg& msg );

	//! Removes disconnected users from the lobby by compacting the user list.
	void						   CompactDisconnectedUsers();

	//! Sends a request to the host to join a local user to a session
	void						   RequestLocalUserJoin( idLocalUser* localUser );

	//! Sends a request to the host to remove a session user from the session
	void						   RequestSessionUserDisconnect( int sessionUserIndex );

	//! Synchronizes lobby users with local users based on join permissions and online match settings.
	void						   SyncLobbyUsersWithLocalUsers( bool allowJoin, bool onlineMatch );

	//! Validates if a connected user is properly initialized and connected.
	bool						   ValidateConnectedUser( const lobbyUser_t* user ) const;

	//! Checks if a lobby user at the specified index is disconnected.
	virtual bool				   IsLobbyUserDisconnected( int userIndex ) const;

	//! Checks if a lobby user ID refers to a valid user in the lobby
	virtual bool				   IsLobbyUserValid( lobbyUserID_t lobbyUserID ) const;

	//! Checks if a lobby user is loaded
	virtual bool				   IsLobbyUserLoaded( lobbyUserID_t lobbyUserID ) const;

	//! Checks if a lobby user has received their first full network snapshot.
	virtual bool				   LobbyUserHasFirstFullSnap( lobbyUserID_t lobbyUserID ) const;

	//! Returns the lobby user ID for a user at the specified ordinal index
	virtual lobbyUserID_t		   GetLobbyUserIdByOrdinal( int userIndex ) const;

	//! Retrieves the index of a lobby user from their unique lobby user ID.
	virtual int					   GetLobbyUserIndexFromLobbyUserID( lobbyUserID_t lobbyUserID ) const;

	//! Enables snapshots for a specified lobby user by identifying the user and updating the peer snapshot settings.
	virtual void				   EnableSnapshotsForLobbyUser( lobbyUserID_t lobbyUserID );

	//! Checks if a peer at the specified index is disconnected.
	virtual bool				   IsPeerDisconnected( int peerIndex ) const { return !peers[peerIndex].IsConnected(); }

	//! Calculates and returns the average level of active members in the lobby.
	float						   GetAverageSessionLevel();

	//! Returns the average level of local users, optionally filtering for those who can play online.
	float						   GetAverageLocalUserLevel( bool onlineOnly );

	/*!
		\brief Queues a reliable player-to-player message for transmission to another user in the lobby.

		This function prepares and queues a reliable message that is intended to be sent directly from one user to another within the lobby. It constructs a packet header containing the sender and
	   receiver session indices, and appends the provided data. The message is then queued for transmission using the lobby's reliable messaging system.

		\param fromSessionUserIndex Index of the sending user in the lobby session
		\param toSessionUserIndex Index of the receiving user in the lobby session
		\param type Type identifier for the reliable message
		\param data Pointer to the message data to be sent
		\param dataLen Length of the message data in bytes
		\throws Warning message is issued if the message cannot be queued due to invalid data or if the target user is not found.
	*/
	void						   QueueReliablePlayerToPlayerMessage( int fromSessionUserIndex, int toSessionUserIndex, reliablePlayerToPlayer_t type, const byte* data, int dataLen );

	//! Kicks a lobby user from the session if the current user is the host.
	virtual void				   KickLobbyUser( lobbyUserID_t lobbyUserID );

	//! Returns the count of lobby users who are currently connected and not disconnected
	int							   GetNumConnectedUsers() const;

	/*!
		\brief Determines if the first user is a better host than the second user based on ping and user ID.

		This function is used to compare two potential hosts in a lobby session. For party type lobbies, it only compares user IDs since ping is not a factor. For other lobby types, it first compares
	   ping values, where a lower ping is better. If pings are equal, it uses the user ID as a tiebreaker. The function returns true if the first user should be the host.

		\param ping1 Ping value of the first user
		\param userId1 User ID of the first user
		\param ping2 Ping value of the second user
		\param userId2 User ID of the second user
		\return True if the first user is a better host than the second user, false otherwise.
	*/
	bool						   IsBetterHost( int ping1, lobbyUserID_t userId1, int ping2, lobbyUserID_t userId2 );

	//! Finds the index of a migration invite for the specified address in the migration info.
	int							   FindMigrationInviteIndex( lobbyAddress_t& address );

	//! Updates the host migration process by handling host selection and sending migration invites.
	void						   UpdateHostMigration();

	//! Builds a list of migration invites for lobby users
	void						   BuildMigrationInviteList( bool inviteOldHost );

	//! Selects a new host for the lobby session.
	void						   PickNewHost( bool forceMe = false, bool inviteOldHost = false );

	//! Selects a new host for the lobby based on ping and other criteria.
	void						   PickNewHostInternal( bool forceMe, bool inviteOldHost );

	//! Makes the lobby transition to host state
	void						   BecomeHost();

	//! Ends the migration process and cleans up related information.
	void						   EndMigration();

	//! Resets all host migration state to ensure the next game is not treated as a migrated game.
	void						   ResetAllMigrationState();

	//! Sends migration game data to a peer client
	void						   SendMigrationGameData();

	//! Initializes a bit message for reading or writing migration game data based on the reading flag
	bool						   GetMigrationGameData( idBitMsg& msg, bool reading );

	//! Sets up an idBitMsg to read or write from a user's migration game data buffer
	bool						   GetMigrationGameDataUser( lobbyUserID_t lobbyUserID, idBitMsg& msg, bool reading );

	//! Updates snapshots for lobby peers.
	void						   UpdateSnaps();

	//! Sends completed snapshots to connected peers and returns whether all submitted snapshots were sent
	bool						   SendCompletedSnaps();

	//! Sends resources to a specified peer in the lobby
	bool						   SendResources( int p );

	//! Submits a pending snapshot for the specified peer if conditions are met
	bool						   SubmitPendingSnap( int p );

	//! Sends a completed pending snapshot to the specified peer if ready and valid
	void						   SendCompletedPendingSnap( int p );

	//! Checks if a peer should be throttled based on bandwidth usage
	void						   CheckPeerThrottle( int p );

	//! Applies a snapshot delta to the lobby state for a specific peer.
	void						   ApplySnapshotDelta( int p, int snapshotNumber );

	//! Applies a snapshot delta to a specific peer in the lobby
	bool						   ApplySnapshotDeltaInternal( int p, int snapshotNumber );

	//! Sends a snapshot to a specific peer in the lobby
	void						   SendSnapshotToPeer( idSnapShot& ss, int p );

	//! Returns true if all connected peers have received the initial base state snapshot.
	bool						   AllPeersHaveBaseState();

	//! Throttles snapshots for a specified peer for a given number of seconds.
	void						   ThrottleSnapsForXSeconds( int p, int seconds, bool recoverPing );

	//! Returns true if the specified peer has sent at least one snapshot and is ready to start the match.
	bool						   FirstSnapHasBeenSent( int p );

	//! Ensures all connected peers in the lobby have received the initial game state snapshot before gameplay begins.
	virtual bool				   EnsureAllPeersHaveBaseState();

	//! Checks if all connected peers in the lobby have a stale snapshot object with the specified ID.
	virtual bool				   AllPeersHaveStaleSnapObj( int objId );

	//! Checks if all connected peers have a valid expected snapshot object with the specified ID
	virtual bool				   AllPeersHaveExpectedSnapObj( int objId );

	//! Marks a snapshot object as deleted for all connected peers in the lobby
	virtual void				   MarkSnapObjDeleted( int objId );

	//! Refreshes a snapshot object by marking it for update in all connected peers
	virtual void				   RefreshSnapObj( int objId );

	//! Resets the bandwidth statistics for all peers in the lobby.
	void						   ResetBandwidthStats();

	//! Detects if a peer's ping has increased significantly, indicating potential connection saturation.
	void						   DetectSaturation( int p );

	//! Adds a snapshot object template to the lobby for future snapshot processing
	virtual void				   AddSnapObjTemplate( int objID, idBitMsg& msg );

	static const int			   MAX_PEERS = MAX_PLAYERS;

	//------------------------
	// Pings
	//------------------------
	struct pktPingValues_t {
		idArray<short, MAX_PEERS> pings;
	};

	static const int	 PING_INTERVAL_MS = 3000;

	int					 lastPingValuesRecvTime; // so clients can display something when server stops pinging
	int					 nextSendPingValuesTime; // the next time to send RELIABLE_PING_VALUES

	static const int	 MIGRATION_GAME_DATA_INTERVAL_MS = 1000;
	int					 nextSendMigrationGameTime; // when to send next migration game data
	int					 nextSendMigrationGamePeer; // who to send next migration game data to

	lobbyType_t			 lobbyType;
	lobbyState_t		 state; // State of this lobby
	failedReason_t		 failedReason;

	int					 host;			  // which peer is the host of this type of session (-1 if we are the host)
	int					 peerIndexOnHost; // -1 if we are the host
	lobbyAddress_t		 hostAddress;	  // address of the host for this type of session
	bool				 isHost;		  // true if we are the host
	idLobbyBackend*		 lobbyBackend;

	int					 helloStartTime;	 // Used to determine when the first hello was sent
	int					 lastConnectRequest; // Used to determine when the last hello was sent
	int					 connectionAttempts; // Number of connection attempts

	bool				 needToDisplayMigrateMsg; // If true, we migrated as host, so we need to display the msg as soon as the lobby is active
	gameDialogMessages_t migrationDlg;			  // current migration dialog we should be showing

	uint8				 migrateMsgFlags; // cached match flags from the old game we migrated from, so we know what type of msg to display

	bool				 joiningMigratedGame; // we are joining a migrated game and need to tell the session mgr if we succeed or fail

	// ------------------------
	//	Bandwidth challenge
	// ------------------------
	int					 bandwidthChallengeEndTime;	   // When the challenge will end/timeout
	int					 bandwidthChallengeStartTime;  // time in MS the challenge started
	bool				 bandwidthChallengeFinished;   // (HOST) test is finished and we received results back from all peers (or timed out)
	int					 bandwidthChallengeNumGoodSeq; // (PEER) num of good, in order packets we recevieved

	int					 lastSnapBspHistoryUpdateSequence;

	//! Saves the gamertag of a disconnected user for later retrieval.
	void				 SaveDisconnectedUser( const lobbyUser_t& user );

	idSessionCallbacks*	 sessionCB;

	enum migrationState_t {
		MIGRATE_NONE,
		MIGRATE_PICKING_HOST,
		MIGRATE_BECOMING_HOST,
	};

	struct migrationInvite_t {
		//! Initializes a migrationInvite_t object with default values.
		migrationInvite_t()
		{
			lastInviteTime	  = -1;
			pingMs			  = 0;
			migrationGameData = -1;
		}

		lobbyAddress_t address;
		int			   pingMs;
		lobbyUserID_t  userId;
		int			   lastInviteTime;
		int			   migrationGameData;
	};

	struct migrationInfo_t {
		//! Initializes a migrationInfo_t object with default values.
		migrationInfo_t()
		{
			state	  = MIGRATE_NONE;
			ourPingMs = 0;
			ourUserId = lobbyUserID_t();
		}

		migrationState_t						   state;
		idStaticList<migrationInvite_t, MAX_PEERS> invites;
		int										   migrationStartTime;
		int										   ourPingMs;
		lobbyUserID_t							   ourUserId;

		struct persistUntilGameEnds_t {
			//! Constructs a new persistUntilGameEnds_t object and initializes it by clearing any existing data.
			persistUntilGameEnds_t() { Clear(); }

			//! Clears all migration-related flags and resets game data fields to their default states.
			void Clear()
			{
				wasMigratedHost			  = false;
				wasMigratedJoin			  = false;
				wasMigratedGame			  = false;
				ourGameData				  = -1;
				hasGameData				  = false;
				hasRelaunchedMigratedGame = false;

				memset( gameData, 0, sizeof( gameData ) );
				memset( gameDataUser, 0, sizeof( gameDataUser ) );
			}

			int				 ourGameData;
			bool			 wasMigratedHost; // we are hosting a migrated session
			bool			 wasMigratedJoin; // we joined a migrated session
			bool			 wasMigratedGame; // If true, we migrated from a game
			bool			 hasRelaunchedMigratedGame;

			// A generic blob of data that the gamechallenge (or anything else) can read and write to for host migration
			static const int MIGRATION_GAME_DATA_SIZE = 32;
			byte			 gameData[MIGRATION_GAME_DATA_SIZE];

			static const int MIGRATION_GAME_DATA_USER_SIZE = 64;
			byte			 gameDataUser[MAX_PLAYERS][MIGRATION_GAME_DATA_USER_SIZE];

			bool			 hasGameData;
		} persistUntilGameEndsData;
	};

	struct disconnectedUser_t {
		lobbyUserID_t lobbyUserID; // Locally generated to be unique, and internally keeps the local user handle
		char		  gamertag[lobbyUser_t::MAX_GAMERTAG];
	};

	migrationInfo_t									migrationInfo;

	bool											showHostLeftTheSession;
	bool											connectIsFromInvite;

	idList<lobbyConnectInfo_t>						searchResults;

	typedef idStaticList<lobbyUser_t*, MAX_PLAYERS> idLobbyUserList;
	typedef idStaticList<lobbyUser_t, MAX_PLAYERS>	idLobbyUserPool;

	idLobbyUserList									userList;  // list of currently connected users to this lobby
	idLobbyUserList									freeUsers; // list of free users
	idLobbyUserPool									userPool;

	idList<disconnectedUser_t>						disconnectedUsers; // List of users which were connected, but aren't anymore, for printing their name on the hud

	idStaticList<peer_t, MAX_PEERS>					peers; // Unique machines connected to this lobby

	uint32											partyToken;

	idMatchParameters								parms;

	bool											loaded;				// Used for game sessions, whether this machine is loaded or not
	bool											respondToArbitrate; // true when the host has requested us to arbitrate our session (for TYPE_GAME only)
	bool											everyoneArbitrated;
	bool											waitForPartyOk;
	bool											startLoadingFromHost;

	//------------------------
	// Snapshot jobs
	//------------------------
	static const int								SNAP_OBJ_JOB_MEMORY = 1024 * 128; // 128k of obj memory

	lzwCompressionData_t*							lzwData;			// Shared across all snapshot jobs
	uint8*											objMemory;			// Shared across all snapshot jobs
	bool											haveSubmittedSnaps; // True if we previously submitted snaps to jobs
	idSnapShot*										localReadSS;

	struct snapDeltaAck_t {
		int p;
		int snapshotNumber;
	};

	idStaticList<snapDeltaAck_t, 16> snapDeltaAckQueue;
};

/*!
	\class idSessionCallbacks
	\brief Interface for session management and lobby handling callbacks.

	This class defines an interface for session management and lobby handling within a networked gaming environment. It provides methods for accessing lobby information, managing player states,
   handling network communication, and managing session transitions. The interface is designed to be implemented by concrete session managers that handle the actual logic for these operations. It
   includes methods for getting lobby references, managing session states, handling network packets, and managing voice chat. The interface is intended to abstract the session management logic from
   the core game logic, allowing for flexible implementation of different session management strategies.

*/
class idSessionCallbacks
{
public:
	virtual ~idSessionCallbacks() { } // SRS - Added virtual destructor

	virtual idLobby&				  GetPartyLobby()																			= 0;
	virtual idLobby&				  GetGameLobby()																			= 0;
	virtual idLobby&				  GetActingGameStateLobby()																	= 0;
	virtual idLobby*				  GetLobbyFromType( idLobby::lobbyType_t lobbyType )										= 0;
	virtual int						  GetUniquePlayerId() const																	= 0;
	virtual idSignInManagerBase&	  GetSignInManager()																		= 0;
	virtual void					  SendRawPacket( const lobbyAddress_t& to, const void* data, int size, bool useDirectPort ) = 0;

	virtual bool					  BecomingHost( idLobby& lobby )		= 0; // Called when a lobby is about to become host
	virtual void					  BecameHost( idLobby& lobby )			= 0; // Called when a lobby becomes a host
	virtual bool					  BecomingPeer( idLobby& lobby )		= 0; // Called when a lobby is about to become peer
	virtual void					  BecamePeer( idLobby& lobby )			= 0; // Called when a lobby becomes a peer
	virtual void					  FailedGameMigration( idLobby& lobby ) = 0;
	virtual void					  MigrationEnded( idLobby& lobby )		= 0;

	virtual void					  GoodbyeFromHost( idLobby& lobby, int peerNum, const lobbyAddress_t& remoteAddress, int msgType ) = 0;

	virtual uint32					  GetSessionOptions()											 = 0;
	virtual bool					  AnyPeerHasAddress( const lobbyAddress_t& remoteAddress ) const = 0;

	virtual idSession::sessionState_t GetState() const = 0;

	virtual void					  ClearMigrationState() = 0;
	// Called when the lobby receives a RELIABLE_ENDMATCH msg
	virtual void					  EndMatchInternal( bool premature = false ) = 0;

	// Called when the game lobby receives leaderboard stats
	virtual void					  RecvLeaderboardStats( idBitMsg& msg ) = 0;

	// Called once the lobby received its first full snap (used to advance from LOADING to INGAME state)
	virtual void					  ReceivedFullSnap() = 0;

	// Called when lobby received RELIABLE_PARTY_LEAVE_GAME_LOBBY msg
	virtual void					  LeaveGameLobby() = 0;

	virtual void					  PrePickNewHost( idLobby& lobby, bool forceMe, bool inviteOldHost ) = 0;
	virtual bool					  PreMigrateInvite( idLobby& lobby )								 = 0;

	virtual void					  HandleOobVoiceAudio( const lobbyAddress_t& from, const idBitMsg& msg ) = 0;

	// ConnectAndMoveToLobby is called when the lobby receives a RELIABLE_CONNECT_AND_MOVE_TO_LOBBY
	virtual void					  ConnectAndMoveToLobby( idLobby::lobbyType_t destLobbyType, const lobbyConnectInfo_t& connectInfo, bool waitForPartyOk ) = 0;

	virtual class idVoiceChatMgr*	  GetVoiceChat() = 0;

	virtual void					  HandleServerQueryRequest( lobbyAddress_t& remoteAddr, idBitMsg& msg, int msgType ) = 0;
	virtual void					  HandleServerQueryAck( lobbyAddress_t& remoteAddr, idBitMsg& msg )					 = 0;

	virtual void					  HandlePeerMatchParamUpdate( int peer, int msg ) = 0;

	virtual idLobbyBackend*			  CreateLobbyBackend( const idMatchParameters& p, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType )					= 0;
	virtual idLobbyBackend*			  FindLobbyBackend( const idMatchParameters& p, int numPartyUsers, float skillLevel, idLobbyBackend::lobbyBackendType_t lobbyType ) = 0;
	virtual idLobbyBackend*			  JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo, idLobbyBackend::lobbyBackendType_t lobbyType )						= 0;
	virtual void					  DestroyLobbyBackend( idLobbyBackend* lobbyBackend )																				= 0;
};
