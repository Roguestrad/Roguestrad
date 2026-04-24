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
#ifndef __SYS_LOBBY_BACKEND_H__
#define __SYS_LOBBY_BACKEND_H__

extern idCVar net_verboseResource;
#define NET_VERBOSERESOURCE_PRINT \
	if( net_verboseResource.GetBool() ) idLib::Printf

extern idCVar net_verbose;
#define NET_VERBOSE_PRINT \
	if( net_verbose.GetBool() ) idLib::Printf

/*!
	\class lobbyAddress_t
	\brief A class for representing and managing lobby network addresses.

	This class encapsulates the address information for lobby systems, providing functionality to initialize from different source types including network addresses and IP/port combinations. It
   supports conversion to string representation and network message serialization for communication purposes. The class is designed to handle address comparisons and maintains state information about
   relay usage. The implementation focuses on providing a consistent interface for lobby address management within networked environments.

*/
class lobbyAddress_t
{
public:
	//! Initializes a lobbyAddress_t object with default values.
	lobbyAddress_t();

	//! Initializes the lobby address from a network address.
	void		InitFromNetadr( const netadr_t& netadr );

	//! Initializes the lobby address from an IP string and port number.
	void		InitFromIPandPort( const char* ip, int port );

	//! Converts the lobby address to a string representation.
	const char* ToString() const;

	//! Returns false indicating that the lobby address is not using a relay
	bool		UsingRelay() const;

	//! Compares this lobby address with another address for equality.
	bool		Compare( const lobbyAddress_t& addr, bool ignoreSessionCheck = false ) const;

	//! Writes the lobby address data to a bit message for network transmission.
	void		WriteToMsg( idBitMsg& msg ) const;

	//! Reads network address data from a bit message.
	void		ReadFromMsg( idBitMsg& msg );

	// IP address
	netadr_t	netAddr;
};

struct lobbyConnectInfo_t {
public:
	//! Writes the network address information to a bit message for transmission.
	void WriteToMsg( idBitMsg& msg ) const { msg.WriteNetadr( netAddr ); }

	//! Reads network address information from a bit message
	void ReadFromMsg( idBitMsg& msg ) { msg.ReadNetadr( &netAddr ); }

	//! Constructs a new lobby connection information object with a default network address.
	lobbyConnectInfo_t() :
		netAddr()
	{
	}

	netadr_t netAddr;
};

/*!
	\class idNetSessionPort
	\brief Manages network communication for session-based networking through UDP.
*/
class idNetSessionPort
{
public:
	//! Initializes a new instance of the idNetSessionPort class.
	idNetSessionPort();

	//! Initializes the network port for the session
	bool InitPort( int portNumber, bool useBackend );

	/*!
		\brief Reads a raw network packet from the session port into the provided buffer

		This function retrieves a raw packet from the underlying UDP socket and optionally drops packets based on configured network force drop settings. The function updates the from parameter with
	   the sender's address and modifies the size parameter to reflect the actual packet size. It returns true if a packet was successfully read, false otherwise. The function supports packet latency
	   simulation through net_forceLatency and packet dropping through net_forceDrop cvars.

		\param from Reference to lobbyAddress_t that will be filled with the sender's address
		\param data Pointer to buffer where packet data will be stored
		\param size Reference to integer that will be updated with the actual packet size
		\param maxSize Maximum size of the data buffer in bytes
		\return true if a packet was successfully read from the UDP socket, false if no packet was available or if packet dropping was triggered by net_forceDrop
	*/
	bool ReadRawPacket( lobbyAddress_t& from, void* data, int& size, int maxSize );

	//! Sends a raw packet to the specified address using the UDP network layer
	void SendRawPacket( const lobbyAddress_t& to, const void* data, int size );

	//! Checks if the network session port is currently open and ready for use
	bool IsOpen();

	//! Closes the network session port by closing the underlying UDP socket.
	void Close();

private:
	float forcePacketDropCurr; // Used with net_forceDrop and net_forceDropCorrelation
	float forcePacketDropPrev;

	idUDP UDP;
};

struct lobbyUser_t {
	static const int INVALID_PING = 9999;
	// gamertags can be up to 16 4-byte characters + \0
	static const int MAX_GAMERTAG = 64 + 1;

	//! Initializes a new instance of the lobbyUser_t class with default values.
	lobbyUser_t()
	{
		isBot			 = false;
		peerIndex		 = -1;
		disconnecting	 = false;
		level			 = 1;
		pingMs			 = INVALID_PING;
		teamNumber		 = 0;
		arbitrationAcked = false;
		partyToken		 = 0;

		selectedSkin	 = 0;
		weaponAutoSwitch = true;
		weaponAutoReload = true;

		migrationGameData = -1;
	}

	// Common variables
	bool		   isBot;		// true if lobbyUser is a bot.
	int			   peerIndex;	// peer number on host
	lobbyUserID_t  lobbyUserID; // Locally generated to be unique, and internally keeps the local user handle
	char		   gamertag[MAX_GAMERTAG];
	int			   pingMs; // round trip time in milliseconds

	bool		   disconnecting; // true if we've sent a msg to disconnect this user from the session
	int			   level;
	int			   teamNumber;
	uint32		   partyToken; // set by the server when people join as a party

	int			   selectedSkin;
	bool		   weaponAutoSwitch;
	bool		   weaponAutoReload;

	bool		   arbitrationAcked; // if the user is verified for arbitration

	lobbyAddress_t address;

	int			   migrationGameData; // index into the local migration gamedata array that is associated with this user. -1=no migration game data available

	//! Returns true if the lobby user is disconnected, false otherwise.
	bool		   IsDisconnected() const { return lobbyUserID.IsValid() ? false : true; }

	//! Writes lobby user data to a message buffer.
	void		   WriteToMsg( idBitMsg& msg )
	{
		address.WriteToMsg( msg );
		lobbyUserID.WriteToMsg( msg );
		msg.WriteLong( peerIndex );
		msg.WriteShort( pingMs );
		msg.WriteLong( partyToken );
		msg.WriteString( gamertag, MAX_GAMERTAG, false );
		WriteClientMutableData( msg );
	}

	//! Reads user data from a bit message
	void ReadFromMsg( idBitMsg& msg )
	{
		address.ReadFromMsg( msg );
		lobbyUserID.ReadFromMsg( msg );
		peerIndex  = msg.ReadLong();
		pingMs	   = msg.ReadShort();
		partyToken = msg.ReadLong();
		msg.ReadString( gamertag, MAX_GAMERTAG );
		ReadClientMutableData( msg );
	}

	//! Updates the mutable client data for a lobby user with values from the local user profile.
	bool UpdateClientMutableData( const idLocalUser* localUser );

	//! Writes client-side mutable data to a bit message.
	void WriteClientMutableData( idBitMsg& msg )
	{
		msg.WriteBits( selectedSkin, 4 );
		msg.WriteBits( teamNumber, 2 ); // We need two bits since we use team value of 2 for spectating
		msg.WriteBool( weaponAutoSwitch );
		msg.WriteBool( weaponAutoReload );
		release_assert( msg.GetWriteBit() == 0 );
	}

	//! Reads client mutable data from a bit message.
	void ReadClientMutableData( idBitMsg& msg )
	{
		selectedSkin	 = msg.ReadBits( 4 );
		teamNumber		 = msg.ReadBits( 2 ); // We need two bits since we use team value of 2 for spectating
		weaponAutoSwitch = msg.ReadBool();
		weaponAutoReload = msg.ReadBool();
	}
};

/*!
	\class idLobbyBackend
	\brief Abstract base class for lobby backend implementations that interface with different platform backends.

	This class defines the interface for lobby management operations across different platform backends. It provides methods for hosting, searching, joining, and managing lobby sessions, as well as
   handling user registration and arbitration. The class is designed to be inherited by platform-specific implementations that provide concrete functionality for each backend type. The lobby backend
   handles both local and online sessions, and supports various lobby states and operations including matchmaking, session management, and user synchronization. The interface includes methods for
   setting lobby properties such as joinability, in-game status, and skill levels, as well as for arbitration and session lifecycle management.

*/
class idLobbyBackend
{
public:
	enum lobbyBackendState_t {
		STATE_INVALID			= 0,
		STATE_READY				= 1,
		STATE_CREATING			= 2, // In the process of creating the lobby as a host
		STATE_SEARCHING			= 3, // In the process of searching for a lobby to join
		STATE_OBTAINING_ADDRESS = 4, // In the process of obtaining the address of the lobby owner
		STATE_ARBITRATING		= 5, // Arbitrating
		STATE_SHUTTING_DOWN		= 6, // In the process of shutting down
		STATE_SHUTDOWN			= 7, // Was a host or peer at one point, now ready to be deleted
		STATE_FAILED			= 8, // Failure occurred
		NUM_STATES
	};

	//! Returns a string representation of the given lobby backend state.
	static const char* GetStateString( lobbyBackendState_t state_ )
	{
		static const char* stateToString[NUM_STATES] = {
			"STATE_INVALID", "STATE_READY", "STATE_CREATING", "STATE_SEARCHING", "STATE_OBTAINING_ADDRESS", "STATE_ARBITRATING", "STATE_SHUTTING_DOWN", "STATE_SHUTDOWN", "STATE_FAILED"
		};

		return stateToString[state_];
	}

	enum lobbyBackendType_t {
		TYPE_PARTY		= 0,
		TYPE_GAME		= 1,
		TYPE_GAME_STATE = 2,
		TYPE_INVALID	= 0xff,
	};

	//! Initializes a new instance of the idLobbyBackend class with default values.
	idLobbyBackend() :
		type( TYPE_INVALID ),
		isLocal( false ),
		isHost( false )
	{
	}

	//! Constructs an idLobbyBackend object with the specified lobby backend type.
	idLobbyBackend( lobbyBackendType_t lobbyType ) :
		type( lobbyType ),
		isLocal( false ),
		isHost( false )
	{
	}

	virtual ~idLobbyBackend() { } // SRS - Added virtual destructor

	virtual void				StartHosting( const idMatchParameters& p, float skillLevel, lobbyBackendType_t type ) = 0;
	virtual void				StartFinding( const idMatchParameters& p, int numPartyUsers, float skillLevel )		  = 0;
	virtual void				JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo )						  = 0;
	virtual void				GetSearchResults( idList<lobbyConnectInfo_t>& searchResults )						  = 0;
	virtual lobbyConnectInfo_t	GetConnectInfo()																	  = 0;
	virtual void				FillMsgWithPostConnectInfo( idBitMsg& msg )											  = 0; // Passed itno PostConnectFromMsg
	virtual void				PostConnectFromMsg( idBitMsg& msg )													  = 0; // Uses results from FillMsgWithPostConnectInfo

	//! Checks if the local backend owns the provided connection information
	virtual bool				IsOwnerOfConnectInfo( const lobbyConnectInfo_t& connectInfo ) const { return false; }
	virtual void				Shutdown()								   = 0;
	virtual void				GetOwnerAddress( lobbyAddress_t& outAddr ) = 0;

	//! Returns whether the current lobby instance is the host
	virtual bool				IsHost() { return isHost; }

	//! Sets whether the lobby is joinable or not.
	virtual void				SetIsJoinable( bool joinable ) { }
	virtual void				Pump()										   = 0;
	virtual void				UpdateMatchParms( const idMatchParameters& p ) = 0;
	virtual void				UpdateLobbySkill( float lobbySkill )		   = 0;

	//! Sets the in-game status of the lobby backend.
	virtual void				SetInGame( bool value ) { }

	virtual lobbyBackendState_t GetState() = 0;

	//! Returns true if the lobby backend represents a local session.
	virtual bool				IsLocal() const { return isLocal; }

	//! Checks if the lobby backend is currently online.
	virtual bool				IsOnline() const { return !isLocal; }

	//! Indicates whether the arbitration process has started successfully.
	virtual bool				StartArbitration() { return false; }

	//! Placeholder function for arbitrating lobby-related operations.
	virtual void				Arbitrate() { }

	//! Performs arbitration verification.
	virtual void				VerifyArbitration() { }

	//! Returns false indicating that the user is not arbitrated.
	virtual bool				UserArbitrated( lobbyUser_t* user ) { return false; }

	//! Registers a user with the lobby backend.
	virtual void				RegisterUser( lobbyUser_t* user, bool isLocal ) { }

	//! Unregisters a user from the lobby backend.
	virtual void				UnregisterUser( lobbyUser_t* user, bool isLocal ) { }

	//! Starts a new session.
	virtual void				StartSession() { }

	//! Ends the current session.
	virtual void				EndSession() { }

	//! Returns whether the lobby session has been started.
	virtual bool				IsSessionStarted() { return false; }

	//! Flushes statistics data.
	virtual void				FlushStats() { }

	//! Makes this lobby instance the host with the specified number of invites.
	virtual void				BecomeHost( int numInvites ) { }

	//! Registers an address for sending invites after becoming a new host.
	virtual void				RegisterAddress( lobbyAddress_t& address ) { }

	//! Completes the process of becoming the host in a lobby.
	virtual void				FinishBecomeHost() { }

	//! Sets the lobby type for the backend.
	void						SetLobbyType( lobbyBackendType_t lobbyType ) { type = lobbyType; }

	//! Returns the lobby type of this lobby backend instance.
	lobbyBackendType_t			GetLobbyType() const { return type; }

	//! Returns a string representation of the lobby type, either 'Party' or 'Game'.
	const char*					GetLobbyTypeString() const { return ( GetLobbyType() == TYPE_PARTY ) ? "Party" : "Game"; }

	//! Returns true if the current match type is ranked.
	bool						IsRanked() { return MatchTypeIsRanked( parms.matchFlags ); }

	//! Returns true if the lobby match is set to private.
	bool						IsPrivate() { return MatchTypeIsPrivate( parms.matchFlags ); }

protected:
	lobbyBackendType_t type;
	idMatchParameters  parms;
	bool			   isLocal; // True if this lobby is restricted to local play only (won't need and can't connect to online lobbies)
	bool			   isHost;	// True if we created this lobby
};

/*!
	\class idLobbyToSessionCB
	\brief Abstract interface for lobby to session conversion callback handling.
*/
class idLobbyToSessionCB
{
public:
	virtual class idLobbyBackend* GetLobbyBackend( idLobbyBackend::lobbyBackendType_t type ) const = 0;
	virtual bool				  CanJoinLocalHost() const										   = 0;

	// Ugh, hate having to ifdef these, but we're doing some fairly platform specific callbacks
};

#endif // __SYS_LOBBY_BACKEND_H__
