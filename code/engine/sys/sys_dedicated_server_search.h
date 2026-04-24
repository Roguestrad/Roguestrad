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

#ifndef __DEDICATEDSERVERSEARCH_H__
#define __DEDICATEDSERVERSEARCH_H__

/*!
	\class idDedicatedServerSearch
	\brief Manages searching and tracking dedicated game servers.

	Provides functionality to search for dedicated game servers, maintain a list of discovered servers, and handle server query responses. The class supports starting searches with a callback
   mechanism to receive updates, clearing search results, and retrieving server information by index. It handles network address retrieval and server description data access. The search state can be
   reset through the clear operation, and server data is managed internally with index-based access for server information and player lists.

*/
class idDedicatedServerSearch
{
public:
	//! Initializes a new instance of the idDedicatedServerSearch class with a null callback.
	idDedicatedServerSearch();

	//! Destructor for the idDedicatedServerSearch class that cleans up the callback object.
	~idDedicatedServerSearch();

	//! Initializes a dedicated server search with the provided callback.
	void				 StartSearch( const idCallback& cb );

	//! Clears the dedicated server search data and resets its state.
	void				 Clear();

	//! Handles a query acknowledgment from a dedicated server by updating its information or adding it to the list.
	void				 HandleQueryAck( lobbyAddress_t& addr, idBitMsg& msg );

	//! Retrieves the network address at the specified index if the index is valid.
	bool				 GetAddrAtIndex( netadr_t& addr, int i );

	//! Returns a pointer to the server information at the specified index, or NULL if the index is out of bounds.
	const serverInfo_t*	 DescribeServerAtIndex( int i ) const;

	//! Returns the list of players connected to the server at the specified index.
	const idList<idStr>* GetServerPlayersAtIndex( int i ) const;

	//! Returns the number of servers in the dedicated server search list.
	int					 NumServers() const;

private:
	struct serverInfoDedicated_t {
		lobbyAddress_t addr;
		serverInfo_t   serverInfo;
		idList<idStr>  connectedPlayers;
	};

	idList<serverInfoDedicated_t> list;
	idCallback*					  callback;
};

#endif // __DEDICATEDSERVERSEARCH_H__
