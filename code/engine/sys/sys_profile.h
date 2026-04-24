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
#ifndef __SYS_PROFILE_H__
#define __SYS_PROFILE_H__

#include "sys_savegame.h"
#include "sys_session_savegames.h"

class idSaveGameProcessorSaveProfile;
class idSaveGameProcessorLoadProfile;
class idLocalUser;
class idPlayerProfile;

/*!
	\class idProfileMgr
	\brief Manages player profiles and asynchronous loading/saving of profile settings.

	The idProfileMgr class serves as a central manager for player profiles, handling initialization with a local user, processing asynchronous profile operations, and maintaining the current player
   profile. It supports both loading and saving profile settings asynchronously while managing the lifecycle of profile data. The class ensures that profile data is properly validated upon loading and
   provides notifications upon saving completion. The manager is designed to integrate with a local user context and handle profile operations without blocking the main execution thread.

*/
class idProfileMgr
{
public:
	//! Initializes a new instance of the idProfileMgr class.
	idProfileMgr();
	~idProfileMgr();

	// Not copyable because we use unique_ptrs.
	idProfileMgr&	 operator=( const idProfileMgr& ) = delete;

	//! Initializes the profile manager with the specified local user.
	void			 Init( idLocalUser* user );

	//! Processes profile saving and loading operations.
	void			 Pump();

	//! Returns the player profile for the current user, creating it if necessary.
	idPlayerProfile* GetProfile();

private:
	//! Loads profile settings asynchronously if profile and save game are enabled.
	void LoadSettingsAsync();

	//! Saves the current profile settings asynchronously if enabled and a profile exists.
	void SaveSettingsAsync();

	//! Handles completion of loading game profile settings by validating checksum and deserializing profile data
	void OnLoadSettingsCompleted( idSaveLoadParms* parms );

	//! Handles completion of saving game settings and displays appropriate UI notifications.
	void OnSaveSettingsCompleted( idSaveLoadParms* parms );

private:
	std::unique_ptr<idSaveGameProcessorSaveProfile> profileSaveProcessor;
	std::unique_ptr<idSaveGameProcessorLoadProfile> profileLoadProcessor;

	idLocalUser*									user; // reference passed in
	idPlayerProfile*								profile;
	saveGameHandle_t								handle;
};

/*!
	\class idSaveGameProcessorSaveProfile
	\brief Manages save game profile processing for player data.
*/
class idSaveGameProcessorSaveProfile : public idSaveGameProcessorSaveFiles
{
public:
	DEFINE_CLASS( idSaveGameProcessorSaveProfile );

	//! Initializes a new instance of the idSaveGameProcessorSaveProfile class.
	idSaveGameProcessorSaveProfile();

	//! Initializes a save profile for the player profile and folder
	bool		 InitSaveProfile( idPlayerProfile* profile, const char* folder );

	//! Executes the save profile processing by delegating to the save files processor.
	virtual bool Process();

private:
	idFile_SaveGame* profileFile;
	idPlayerProfile* profile;
};

/*!
	\class idSaveGameProcessorLoadProfile
	\brief Provides functionality for loading and processing player profile save game data.
*/
class idSaveGameProcessorLoadProfile : public idSaveGameProcessorLoadFiles
{
public:
	DEFINE_CLASS( idSaveGameProcessorLoadProfile );

	//! Initializes a new instance of the idSaveGameProcessorLoadProfile class.
	idSaveGameProcessorLoadProfile();
	~idSaveGameProcessorLoadProfile();

	//! Initializes a load profile for player settings with the specified profile and folder.
	bool		 InitLoadProfile( idPlayerProfile* profile, const char* folder );

	//! Processes the load profile save game data by delegating to the base class implementation.
	virtual bool Process();

private:
	idFile_SaveGame* profileFile;
	idPlayerProfile* profile;
};

//! Checks if a save game profile exists in the savegame location
bool Sys_SaveGameProfileCheck();

#endif
