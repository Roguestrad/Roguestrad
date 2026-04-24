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
#ifndef __POSIX_SIGNIN_H__
#define __POSIX_SIGNIN_H__

#include "localuser.h"

/*!
	\class idSignInManagerWin
	\brief Manages local user sign-in and state management for Windows platform.

	This class provides platform-specific implementation for handling local user authentication and state tracking on Windows systems. It extends the base sign-in manager functionality to support
   Windows-specific user management operations. The class maintains a collection of local users and provides methods to enumerate, register, and remove users. It also handles processing user state
   updates and manages the shutdown sequence for the Windows sign-in subsystem. The implementation is designed to integrate with Windows user authentication mechanisms while providing a consistent
   interface for user management across different platforms.

*/
class idSignInManagerWin : public idSignInManagerBase
{
public:
	//! Constructs an idSignInManagerWin instance and initializes the DLC version check flag to false.
	idSignInManagerWin() :
		dlcVersionChecked( false )
	{
	}
	virtual ~idSignInManagerWin() { }

	//! Processes local user management and updates user states.
	virtual void			   Pump();

	//! Shuts down the Windows sign-in manager
	virtual void			   Shutdown();

	//! Returns the number of local users managed by the sign-in manager.
	virtual int				   GetNumLocalUsers() const { return localUsers.Num(); }

	//! Returns a pointer to the local user at the specified index.
	virtual idLocalUser*	   GetLocalUserByIndex( int index ) { return &localUsers[index]; }

	//! Returns a pointer to the local user at the specified index.
	virtual const idLocalUser* GetLocalUserByIndex( int index ) const { return &localUsers[index]; }

	//! Removes a local user from the collection at the specified index.
	virtual void			   RemoveLocalUserByIndex( int index );

	//! Registers a local user for the specified input device
	virtual void			   RegisterLocalUser( int inputDevice );

	//! Creates a new user with the specified input device state and registers it locally.
	bool					   CreateNewUser( winUserState_t& state );

private:
	idStaticList<idLocalUserWin, MAX_INPUT_DEVICES> localUsers;
	bool											dlcVersionChecked;
};

#endif
