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
#ifndef __PLAYERPROFILE_H__
#define __PLAYERPROFILE_H__

#define MAX_PROFILE_SIZE ( 1024 * 1000 ) // High number for the key bindings

/*
================================================
profileStatValue_t
================================================
*/
union profileStatValue_t {
	int	  i;
	float f;
};

/*!
	\class idPlayerProfile
	\brief Manages player profile data including settings, achievements, and statistics.

	This class encapsulates the data and behavior associated with a player profile, providing mechanisms for serialization, state management, and configuration handling. It maintains player-specific
   settings such as achievements, statistics, and configuration states while supporting operations to load, save, and modify these settings. The class supports different states for profile management
   and provides methods to handle device associations and loading states. It also includes functionality for managing DLC versions, player levels, and lefty flip settings, along with methods to
   execute configuration commands.

*/
class idPlayerProfile
{
	friend class idLocalUser;
	friend class idProfileMgr;

public:
	// Only have room to squeeze ~450 in doom3 right now
	static const int MAX_PLAYER_PROFILE_STATS = 200;

	enum state_t { IDLE = 0, SAVING, LOADING, SAVE_REQUESTED, LOAD_REQUESTED, ERR };

protected:
	//! Initializes a new instance of the player profile with default values and initial state.
	idPlayerProfile();

public:
	virtual ~idPlayerProfile();

	//! Creates and returns a player profile for the specified input device index.
	static idPlayerProfile* CreatePlayerProfile( int deviceIndex );

	//! Initializes all player profile data members to their default values.
	void					SetDefaults();

	//! Serializes player profile data using the provided serializer.
	bool					Serialize( idSerializer& ser );

	//! Returns the device number associated with the player profile.
	const int				GetDeviceNumForProfile() const { return deviceNum; }

	//! Sets the device number for the player profile.
	void					SetDeviceNumForProfile( int num ) { deviceNum = num; }

	//! Saves the player profile settings if they are marked as dirty or if forced to save.
	void					SaveSettings( bool forceDirty );

	//! Loads player profile settings asynchronously when the profile is not already in a loading state.
	void					LoadSettings();

	//! Returns the current state of the player profile.
	state_t					GetState() const { return state; }

	//! Returns the requested state of the player profile.
	state_t					GetRequestedState() const { return requestedState; }

	//! Returns true if the player profile has been modified since the last save.
	bool					IsDirty() { return dirty; }

	//! Retrieves the state of a specific achievement identified by the given ID.
	bool					GetAchievement( const int id ) const;

	//! Sets a player achievement by ID.
	void					SetAchievement( const int id );

	//! Clears the achievement flag for the specified achievement ID.
	void					ClearAchievement( const int id );

	//! Returns the DLC release version associated with the player profile.
	int						GetDlcReleaseVersion() const { return dlcReleaseVersion; }

	//! Sets the DLC release version for the player profile.
	void					SetDlcReleaseVersion( int version ) { dlcReleaseVersion = version; }

	//! Returns the level of the player profile.
	int						GetLevel() const { return 0; }

	//! Returns the current configuration set identifier.
	int						GetConfig() const { return configSet; }

	//! Sets the player configuration and optionally saves it.
	void					SetConfig( int config, bool save );

	//! Restores the default configuration for the player profile.
	void					RestoreDefault();

	//! Sets the lefty flip boolean flag and executes the configuration.
	void					SetLeftyFlip( bool lf );

	//! Returns the lefty flip setting of the player profile.
	bool					GetLeftyFlip() const { return leftyFlip; }

private:
	//! Sets an integer statistic value at the specified index and marks the profile as dirty
	void  StatSetInt( int s, int v );

	//! Sets a float statistic value at the specified index and marks the profile as dirty
	void  StatSetFloat( int s, float v );

	//! Returns the integer value of a specified statistics field from the player profile.
	int	  StatGetInt( int s ) const;

	//! Returns the float value of a specified statistic from the player profile
	float StatGetFloat( int s ) const;

	//! Sets the state of the player profile to the specified value.
	void  SetState( state_t value ) { state = value; }

	//! Sets the requested state of the player profile to the specified value.
	void  SetRequestedState( state_t value ) { requestedState = value; }

	//! Sets the dirty state of the player profile.
	void  MarkDirty( bool isDirty ) { dirty = isDirty; }

	//! Executes configuration commands based on profile settings and optional save flags.
	void  ExecConfig( bool save = false, bool forceDefault = false );

protected:
	// Do not save:
	state_t													   state;
	state_t													   requestedState;
	int														   deviceNum;

	// Save:
	uint64													   achievementBits;
	uint64													   achievementBits2;
	int														   dlcReleaseVersion;
	int														   configSet;
	bool													   customConfig;
	bool													   leftyFlip;

	bool													   dirty; // dirty bit to indicate whether or not we need to save

	idStaticList<profileStatValue_t, MAX_PLAYER_PROFILE_STATS> stats;
};

#endif
