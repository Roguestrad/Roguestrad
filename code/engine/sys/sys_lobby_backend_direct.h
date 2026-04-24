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
#ifndef __SYS_LOBBY_BACKEND_DIRECT_H__
#define __SYS_LOBBY_BACKEND_DIRECT_H__

/*!
	\class idLobbyBackendDirect
	\brief Provides direct lobby backend functionality for hosting and joining game sessions.

	Handles the core operations for managing game sessions in a direct connection lobby environment. This class manages the state transitions and operations required to host or join games, including
   processing search results, managing user registration, and updating lobby parameters. It supports both hosting and searching modes and maintains the connection information necessary for direct
   network communication. The class is designed to work with match parameters and skill levels to facilitate proper matchmaking. It also handles state management through pumping operations and can
   transition between different lobby states as needed.

*/
class idLobbyBackendDirect : public idLobbyBackend
{
public:
	//! Constructs a new instance of idLobbyBackendDirect and initializes its state to STATE_INVALID
	idLobbyBackendDirect();

	//! Initializes the lobby backend to start hosting a game session.
	virtual void				StartHosting( const idMatchParameters& p, float skillLevel, lobbyBackendType_t type );

	//! Initializes the lobby backend to start searching for matches based on the provided parameters.
	virtual void				StartFinding( const idMatchParameters& p, int numPartyUsers, float skillLevel );

	//! Joins a lobby using the provided connection information, setting up the network address and lobby state.
	virtual void				JoinFromConnectInfo( const lobbyConnectInfo_t& connectInfo );

	//! Retrieves search results for lobby connections.
	virtual void				GetSearchResults( idList<lobbyConnectInfo_t>& searchResults );

	//! This function is a virtual placeholder that does nothing in the base class implementation.
	virtual void				FillMsgWithPostConnectInfo( idBitMsg& msg ) { }

	//! Placeholder implementation for handling post-connection messages.
	virtual void				PostConnectFromMsg( idBitMsg& msg ) { }

	//! Sets the shutdown state for the lobby backend direct instance.
	virtual void				Shutdown();

	//! Retrieves the network address of the lobby owner and updates the state to ready.
	virtual void				GetOwnerAddress( lobbyAddress_t& outAddr );

	//! Sets whether the lobby is joinable or not.
	virtual void				SetIsJoinable( bool joinable );

	//! Retrieves connection information for the lobby, including the network address.
	virtual lobbyConnectInfo_t	GetConnectInfo();

	//! Checks if the given connection info belongs to the owner of this lobby backend.
	virtual bool				IsOwnerOfConnectInfo( const lobbyConnectInfo_t& connectInfo ) const;

	//! Processes pending lobby backend operations and updates the lobby state.
	virtual void				Pump();

	//! Updates the match parameters for the current lobby session.
	virtual void				UpdateMatchParms( const idMatchParameters& p );

	//! Updates the lobby skill value for the direct lobby backend.
	virtual void				UpdateLobbySkill( float lobbySkill );

	//! Sets the in-game state of the lobby backend.
	virtual void				SetInGame( bool value );

	//! Returns the current state of the lobby backend.
	virtual lobbyBackendState_t GetState() { return state; }

	//! Makes the lobby backend the host with the specified number of invites.
	virtual void				BecomeHost( int numInvites );

	//! Sets the lobby backend to indicate that the local user has become the host.
	virtual void				FinishBecomeHost();

	//! Registers a user with the lobby backend, marking them as local or remote.
	virtual void				RegisterUser( lobbyUser_t* user, bool isLocal );

	//! Unregisters a user from the lobby backend.
	virtual void				UnregisterUser( lobbyUser_t* user, bool isLocal );

private:
	lobbyBackendState_t state;
	netadr_t			address;
};

#endif // __SYS_LOBBY_BACKEND_DIRECT_H__
