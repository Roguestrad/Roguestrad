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
#ifndef __SYS_VOICECHATMGR_H__
#define __SYS_VOICECHATMGR_H__

#include "sys_lobby_backend.h"

/*!
	\class idVoiceChatMgr
	\brief Manages voice chat functionality including talker registration, data routing, and system state management.

	The idVoiceChatMgr class serves as the central manager for voice chat operations within the system. It handles the registration and tracking of talkers across different lobby types, manages the
   routing of voice data between local and remote participants, and maintains the overall state of the voice chat system. The class supports multiple lobby types and allows for grouping of talkers to
   control communication patterns. It provides methods for initializing and shutting down the voice chat system, registering and unregistering talkers, retrieving active local talkers, and determining
   communication relationships between talkers. The manager also handles headset state tracking, mute toggling, and system-level voice chat restrictions. The interface includes virtual methods for
   platform-specific implementations of core voice chat operations such as data retrieval, submission, and talker registration, allowing for customization based on the underlying audio subsystem. The
   class maintains internal data structures to track machines and talkers, manages reference counting for remote machines, and ensures proper cleanup during shutdown.

*/
class idVoiceChatMgr
{
public:
	//! Initializes a new instance of the idVoiceChatMgr class with default values.
	idVoiceChatMgr() :
		activeLobbyType( -1 ),
		activeGroupIndex( 0 ),
		sendFrame( 0 ),
		disableVoiceReasons( 0 ),
		sendGlobal( false )
	{
	}

	virtual ~idVoiceChatMgr() { } // SRS - Added virtual destructor

	//! Initializes the voice chat manager with the specified XAudio2 pointer.
	virtual void Init( void* pXAudio2 );

	//! Shuts down the voice chat manager and performs cleanup assertions
	virtual void Shutdown();

	//! Registers a talker for voice chat in the specified lobby type.
	void		 RegisterTalker( lobbyUser_t* user, int lobbyType, bool isLocal );

	//! Removes a talker from the voice chat system based on the provided user, lobby type, and local flag.
	void		 UnregisterTalker( lobbyUser_t* user, int lobbyType, bool isLocal );

	//! Retrieves the list of active local talkers by filtering through all talkers and appending valid local ones to the provided list.
	void		 GetActiveLocalTalkers( idStaticList<int, MAX_PLAYERS>& localTalkers );

	//! Populates a list of remote talker addresses that should receive voice data from the specified local talker.
	void		 GetRecipientsForTalker( int talkerIndex, idStaticList<const lobbyAddress_t*, MAX_PLAYERS>& recipients );

	//! Sets the talker group index for a specified user in a lobby
	void		 SetTalkerGroup( const lobbyUser_t* user, int lobbyType, int groupIndex );

	//! Sets the active lobby type and updates registered talkers if the lobby type changes.
	void		 SetActiveLobby( int lobbyType );

	//! Sets the active chat group index and updates registered talkers accordingly.
	void		 SetActiveChatGroup( int groupIndex );

	//! Finds the index of a talker by its user ID and lobby type.
	int			 FindTalkerByUserId( lobbyUserID_t lobbyUserID, int lobbyType );

	//! Retrieves voice chat data for a local talker.
	bool		 GetLocalChatData( int talkerIndex, byte* data, int& dataSize );

	//! Processes incoming voice chat data for a specific talker in the voice chat manager.
	void		 SubmitIncomingChatData( const byte* data, int dataSize );

	//! Retrieves the voice chat state for a specified lobby user.
	voiceState_t GetVoiceState( const lobbyUser_t* user );

	//! Determines whether voice chat can be sent from one talker to another.
	bool		 CanSendVoiceTo( int talkerFromIndex, int talkerToIndex );

	//! Returns true if voice chat is restricted due to account privileges.
	bool		 IsRestrictedByPrivleges();

	//! Sets the headset state for a specified talker index.
	void		 SetHeadsetState( int talkerIndex, bool state );

	//! Returns the headset state for a specified talker index.
	bool		 GetHeadsetState( int talkerIndex ) const { return talkers[talkerIndex].hasHeadset; }

	//! Checks if the headset state has changed for a specified talker and resets the changed flag.
	bool		 HasHeadsetStateChanged( int talkerIndex );

	enum disableVoiceReason_t {
		REASON_GENERIC	  = BIT( 0 ),
		REASON_PRIVILEGES = BIT( 1 ),
	};

	//! Sets the reason why voice chat is disabled.
	void		 SetDisableVoiceReason( disableVoiceReason_t reason );

	//! Removes a specific voice chat disable reason and updates registered talkers.
	void		 ClearDisableVoiceReason( disableVoiceReason_t reason );

	virtual bool GetLocalChatDataInternal( int talkerIndex, byte* data, int& dataSize )			   = 0;
	virtual void SubmitIncomingChatDataInternal( int talkerIndex, const byte* data, int dataSize ) = 0;
	virtual bool TalkerHasData( int talkerIndex )												   = 0;

	//! Pumps the voice chat manager to process audio input and output.
	virtual void Pump() { }

	//! Flushes all voice chat buffers.
	virtual void FlushBuffers() { }

	//! Toggles the mute state for a target user based on the source user's request.
	virtual void ToggleMuteLocal( const lobbyUser_t* src, const lobbyUser_t* target );

protected:
	struct remoteMachine_t {
		int			   lobbyType;
		lobbyAddress_t address;
		int			   refCount;
		int			   sendFrame;
	};

	struct talker_t {
		//! Initializes a new talker_t instance with default values.
		talker_t() :
			user( NULL ),
			isLocal( false ),
			lobbyType( -1 ),
			groupIndex( -1 ),
			registered( false ),
			registeredSuccess( false ),
			machineIndex( -1 ),
			isMuted( false ),
			hasHeadset( true ),
			hasHeadsetChanged( false ),
			talking( false ),
			talkingGlobal( false ),
			talkingTime( 0 )
		{
		}

		lobbyUser_t* user;
		bool		 isLocal;
		int			 lobbyType;
		int			 groupIndex;
		bool		 registered;		// True if this user is currently registered with the XHV engine
		bool		 registeredSuccess; // True if this user is currently successfully registered with the XHV engine
		int			 machineIndex;		// Index into remote machines array (-1 if this is a local talker)
		bool		 isMuted;			// This machine is not allowed to hear or talk to this player
		bool		 hasHeadset;		// This user has a headset connected
		bool		 hasHeadsetChanged; // This user's headset state has changed
		bool		 talking;
		bool		 talkingGlobal;
		int			 talkingTime;

		//! Returns true if the talker is local, false otherwise.
		bool		 IsLocal() const { return isLocal; }
	};

	virtual bool								   RegisterTalkerInternal( int index )	 = 0;
	virtual void								   UnregisterTalkerInternal( int index ) = 0;

	//! Finds the index of a talker in the voice chat manager based on user and lobby type.
	int											   FindTalkerIndex( const lobbyUser_t* user, int lobbyType );

	//! Finds a remote machine in the voice chat manager by its address and lobby type
	int											   FindMachine( const lobbyAddress_t& address, int lobbyType );

	//! Adds a machine to the voice chat manager or increments its reference count if it already exists
	int											   AddMachine( const lobbyAddress_t& address, int lobbyType );

	//! Decrements the reference count of a remote machine entry to potentially free it.
	void										   RemoveMachine( int machineIndex, int lobbyType );

	//! Updates the registration status of talkers based on their lobby type and system state.
	void										   UpdateRegisteredTalkers();

	idStaticList<talker_t, MAX_PLAYERS * 2>		   talkers;		   // * 2 to account for handling both session types
	idStaticList<remoteMachine_t, MAX_PLAYERS * 2> remoteMachines; // * 2 to account for handling both session types

	int											   activeLobbyType;
	int											   activeGroupIndex;
	int											   sendFrame;
	uint32										   disableVoiceReasons;
	bool										   sendGlobal;
};

#endif // __SYS_VOICECHATMGR_H__
