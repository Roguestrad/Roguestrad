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
#ifndef __SYS_LOCALUSER_H__
#define __SYS_LOCALUSER_H__

#include "sys_profile.h"

struct achievementDescription_t;
class idPlayerProfile;
class idProfileMgr;

enum onlineCaps_t {
	CAP_IS_ONLINE		   = BIT( 0 ),
	CAP_BLOCKED_PERMISSION = BIT( 1 ),
	CAP_CAN_PLAY_ONLINE	   = BIT( 2 ),
};

class idSerializer;

/*
================================================
localUserHandle_t
================================================
*/
struct localUserHandle_t {
public:
	typedef uint32 userHandleType_t;

	//! Constructs a default local user handle with a zero value.
	localUserHandle_t() :
		handle( 0 )
	{
	}

	//! Constructs a local user handle with the specified user handle value.
	explicit localUserHandle_t( userHandleType_t handle_ ) :
		handle( handle_ )
	{
	}

	//! Compares two localUserHandle_t objects for equality based on their handle values.
	bool operator==( const localUserHandle_t& other ) const { return handle == other.handle; }

	//! Compares this local user handle with another for ordering purposes.
	bool operator<( const localUserHandle_t& other ) const { return handle < other.handle; }

	//! Checks if the local user handle is valid by verifying that the handle value is greater than zero.
	bool IsValid() const { return handle > 0; }

	//! Writes the local user handle value to the provided message buffer.
	void WriteToMsg( idBitMsg& msg ) { msg.WriteLong( handle ); }

	//! Reads a long integer from the message and assigns it to the handle.
	void ReadFromMsg( const idBitMsg& msg ) { handle = msg.ReadLong(); }

	//! Serializes the handle member of the localUserHandle_t object using the provided serializer
	void Serialize( idSerializer& ser );

private:
	userHandleType_t handle;
};

/*!
	\class idLocalUser
	\brief Represents a local user controlling a game session through an input device.

	The idLocalUser class serves as a representation of a user who controls the game or menu using an input device such as a gamepad or keyboard. It maintains user-specific data including profile
   information, input device association, and online status. The class handles user input processing, achievement states, and profile settings management. It can be in different states such as joining
   a lobby or playing online, and supports operations to load/save profile settings, manage statistics, and query user properties like online capabilities and storage device availability.

*/
class idLocalUser
{
public:
	//! Initializes a new instance of the idLocalUser class.
	idLocalUser();
	virtual ~idLocalUser() { }

	//! Processes input and updates achievement states for the local user.
	void		 Pump();
	virtual void PumpPlatform() = 0;

	//! Returns true if the local user is signed in and can save stats and profile information.
	virtual bool IsPersistent() const
	{
		return IsProfileReady(); // True if this user is a persistent user, and can save stats, etc (signed in)
	}
	virtual bool   IsProfileReady() const = 0; // True if IsPersistent is true AND profile is signed into LIVE service
	virtual bool   IsOnline() const		  = 0; // True if this user has online capabilities
	virtual uint32 GetOnlineCaps() const  = 0; // Returns combination of onlineCaps_t flags

	//! Returns whether the original persistent owner has changed since it was first registered
	virtual bool   HasOwnerChanged() const
	{
		return false; // Whether or not the original persistent owner has changed since it was first registered
	}
	virtual int			GetInputDevice() const = 0; // Input device of controller
	virtual const char* GetGamerTag() const	   = 0; // Gamertag of user
	virtual bool		IsInParty() const	   = 0; // True if the user is in a party (do we support this on pc and ps3? )
	virtual int			GetPartyCount() const  = 0; // Gets the amount of users in the party

	// Storage related
	virtual bool

		//! Returns true if a storage device is available for save games, false if the player has chosen to play without one.
							 IsStorageDeviceAvailable() const;

	//! Resets the storage device associated with the local user.
	virtual void			 ResetStorageDevice();

	//! Sets an integer statistic value for the local user profile.
	virtual void			 SetStatInt( int stat, int value );

	//! Sets a float statistic value for the local user profile.
	virtual void			 SetStatFloat( int stat, float value );

	//! Returns the integer value of a specified statistic for the local user.
	virtual int				 GetStatInt( int stat );

	//! Returns the float value of a specified stat for the local user.
	virtual float			 GetStatFloat( int stat );

	//! Returns the player profile associated with this local user
	virtual idPlayerProfile* GetProfile() { return GetProfileMgr().GetProfile(); }

	//! Returns a pointer to the player profile associated with this local user.
	const idPlayerProfile*	 GetProfile() const { return const_cast<idLocalUser*>( this )->GetProfile(); }

	//! Returns a reference to the profile manager associated with this local user.
	idProfileMgr&			 GetProfileMgr() { return profileMgr; }

	//! Sets the joining lobby state for a specified lobby type
	void					 SetJoiningLobby( int lobbyType, bool value ) { joiningLobby[lobbyType] = value; }

	//! Checks if a local user is joining a specific type of lobby.
	bool					 IsJoiningLobby( int lobbyType ) const { return joiningLobby[lobbyType]; }

	//! Checks if the local user has permission to play online
	bool					 CanPlayOnline() const { return ( GetOnlineCaps() & CAP_CAN_PLAY_ONLINE ) > 0; }

	//! Returns the local user handle associated with this local user instance.
	localUserHandle_t		 GetLocalUserHandle() const { return localUserHandle; }

	//! Sets the local user handle to the specified value.
	void					 SetLocalUserHandle( localUserHandle_t newHandle ) { localUserHandle = newHandle; }

	//! Loads or creates profile settings for the local user.
	void					 LoadProfileSettings();

	//! Saves the profile settings for the local user.
	void					 SaveProfileSettings();

	//! Requests synchronization of achievement data between the server and local user.
	void					 RequestSyncAchievements() { syncAchievementsRequested = true; }

private:
	bool			  joiningLobby[2];
	localUserHandle_t localUserHandle;
	idProfileMgr	  profileMgr;

	bool			  syncAchievementsRequested;
};

#endif // __SYS_LOCALUSER_H__
