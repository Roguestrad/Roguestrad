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
#ifndef __SYS_SIGNIN_H__
#define __SYS_SIGNIN_H__

/*!
	\class idSignInManagerBase
	\brief Base class for managing sign-in and local user authentication.

	The idSignInManagerBase class provides a foundation for managing local user authentication and sign-in operations. It serves as an abstract interface for handling multiple local users, their
   registration, profiles, and authentication states. The class supports operations such as registering new users, retrieving user information by index or input device, removing users, and validating
   user states. It also manages default profiles, master user identification, and input device mapping. The class is designed to be inherited by concrete implementations that provide specific sign-in
   functionality for different platforms or configurations. The interface includes methods for processing input events, saving user profiles, and debugging user information. Memory management is
   handled through the use of handles and pointers, with methods for removing users either by index, handle, or input device.

*/
class idSignInManagerBase
{
public:
	//! Initializes a new instance of the idSignInManagerBase class.
	idSignInManagerBase() :
		minDesiredLocalUsers( 0 ),
		maxDesiredLocalUsers( 0 ),
		defaultProfile( NULL )
	{
	}
	virtual ~idSignInManagerBase() { }

	virtual void			   Pump()								  = 0;
	virtual int				   GetNumLocalUsers() const				  = 0;
	virtual idLocalUser*	   GetLocalUserByIndex( int index )		  = 0;
	virtual const idLocalUser* GetLocalUserByIndex( int index ) const = 0;
	virtual void			   RemoveLocalUserByIndex( int index )	  = 0;
	virtual void			   RegisterLocalUser( int inputDevice )	  = 0; // Register a local controller user to the passed in input device

	//! Returns the user that has started the registration process but is not yet a local user.
	virtual idLocalUser*	   GetRegisteringUser()
	{
		return NULL; // This is a user that has started the registration process but is not yet a local user.
	}

	//! Returns a pointer to the local user registering by the specified input device, or null if none.
	virtual idLocalUser* GetRegisteringUserByInputDevice( int inputDevice ) { return NULL; }

	//! Placeholder function for signing in.
	virtual void		 SignIn() { }

	//! Returns whether the specified input device is currently being registered.
	virtual bool		 IsDeviceBeingRegistered( int intputDevice ) { return false; }

	//! Checks if any device is currently being registered.
	virtual bool		 IsAnyDeviceBeingRegistered() { return false; }

	//! Shuts down the sign-in manager and releases associated resources
	virtual void		 Shutdown() { }

	//! Outputs all the local users and other debugging information from the sign in manager
	virtual void		 DebugOutputLocalUserInfo() { }

	//! Sets the minimum and maximum desired local users for sign-in management.
	void				 SetDesiredLocalUsers( int minDesiredLocalUsers, int maxDesiredLocalUsers )
	{
		this->minDesiredLocalUsers = minDesiredLocalUsers;
		this->maxDesiredLocalUsers = maxDesiredLocalUsers;
	}

	//! Processes input events for sign-in management and user registration.
	bool			   ProcessInputEvent( const sysEvent_t* ev );

	//! Returns the default player profile, creating it if necessary
	idPlayerProfile*   GetDefaultProfile();

	//! Returns the master local user, which is always the user at index 0.
	idLocalUser*	   GetMasterLocalUser() { return ( GetNumLocalUsers() > 0 ) ? GetLocalUserByIndex( 0 ) : NULL; }

	//! Returns the master local user from the sign-in manager
	const idLocalUser* GetMasterLocalUser() const { return ( GetNumLocalUsers() > 0 ) ? GetLocalUserByIndex( 0 ) : NULL; }

	//! Determines whether the master local user is persistent.
	bool			   IsMasterLocalUserPersistent() const { return ( GetMasterLocalUser() != NULL ) ? GetMasterLocalUser()->IsPersistent() : false; }

	//! Checks if the master local user is online.
	bool			   IsMasterLocalUserOnline() const { return ( GetMasterLocalUser() != NULL ) ? GetMasterLocalUser()->IsOnline() : false; }

	//! Returns the input device ID of the master user's input device, or -1 if no master user is available.
	int				   GetMasterInputDevice() const { return ( GetMasterLocalUser() != NULL ) ? GetMasterLocalUser()->GetInputDevice() : -1; }

	//! Returns the local user handle of the master local user, or an invalid handle if no master user is set.
	localUserHandle_t  GetMasterLocalUserHandle() const { return ( GetMasterLocalUser() != NULL ) ? GetMasterLocalUser()->GetLocalUserHandle() : localUserHandle_t(); }

	//! Returns the local user associated with the specified input device index
	idLocalUser*	   GetLocalUserByInputDevice( int index );

	//! Returns the local user associated with the given handle, or NULL if not found
	idLocalUser*	   GetLocalUserByHandle( localUserHandle_t handle );

	//! Returns the player profile associated with the specified input device index.
	idPlayerProfile*   GetPlayerProfileByInputDevice( int index );

	//! Removes a local user associated with the specified input device index.
	bool			   RemoveLocalUserByInputDevice( int index );

	//! Removes a local user from the sign-in manager by their handle
	bool			   RemoveLocalUserByHandle( localUserHandle_t handle );

	//! Removes all local users from the sign-in manager.
	void			   RemoveAllLocalUsers();

	//! Saves the settings for all local user profiles managed by the sign-in manager.
	void			   SaveUserProfiles();

	//! Validates local users and removes those without profiles or online capabilities when required.
	void			   ValidateLocalUsers( bool requireOnline );

	//! Returns whether a persistent master user is required for the current build configuration
	bool			   RequirePersistentMaster();

	//! Generates a unique local user handle based on a name and current clock ticks.
	localUserHandle_t  GetUniqueLocalUserHandle( const char* name );

protected:
	int				 minDesiredLocalUsers;
	int				 maxDesiredLocalUsers;
	idPlayerProfile* defaultProfile;
};

#endif // __SYS_SIGNIN_H__
