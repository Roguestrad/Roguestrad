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
#ifndef __SYS_ACHIEVEMENTS_H__
#define __SYS_ACHIEVEMENTS_H__

class idLocalUser;

// data structure for online achievement entry descriptions
// this is used for testing purposes to make sure that the consoles
// achievement settings match the game's decls
struct achievementDescription_t {
	void Clear()
	{
		name[0]		   = '\0';
		description[0] = '\0';
		hidden		   = false;
	};
	char name[500];
	char description[1000];
	bool hidden;
};

/*!
	\class idAchievementSystem
	\brief Provides platform-specific achievement tracking and management functionality.

	This class serves as an abstract interface for handling achievements within the application. It provides methods for initialization, user registration, achievement unlocking and locking, state
   management, and synchronization with platform-specific services. The system supports multiple local users and maintains separate achievement states for each user. The interface includes methods for
   retrieving achievement descriptions, managing achievement state caches, and performing system-wide operations such as reset and cancellation of pending actions. The class is designed to be
   implemented by platform-specific subclasses that handle the actual achievement system integration.

*/
class idAchievementSystem
{
public:
	static const int MAX_ACHIEVEMENTS = 128; // This matches the max number of achievements bits in the profile

	virtual ~idAchievementSystem() { }

	//! Initializes the achievement system for the current platform
	virtual void Init() { }

	//! Starts the achievement system.
	virtual void Start() { }

	//! Performs necessary cleanup operations for the achievement system.
	virtual void Shutdown() { }

	//! Checks if the achievement system is ready for requests.
	virtual bool IsInitialized() { return false; }

	//! Registers a local user with the achievement system
	virtual void RegisterLocalUser( idLocalUser* user ) { }

	//! Removes a local user from the achievement system and cancels any pending actions associated with that user.
	virtual void RemoveLocalUser( idLocalUser* user ) { }

	// Unlocks the achievement, all platforms silently fail if the achievement has already been unlocked
	virtual void AchievementUnlock( idLocalUser* user, const int achievementID ) = 0;

	//! Unlocks an achievement for the specified user.
	virtual void AchievementLock( idLocalUser* user, const int achievementID ) { }

	//! Resets all achievements to their original state for the specified user.
	virtual void AchievementLockAll( idLocalUser* user, const int maxId ) { }

	// Should be done every frame
	virtual void Pump() = 0;

	//! Resets the achievement system state, optionally for a specific user
	virtual void Reset( idLocalUser* user = NULL ) { }

	//! Cancels all in-flight achievements for the specified user
	virtual void Cancel( idLocalUser* user ) { }

	//! Retrieves textual information about a given achievement and returns false if there was an error.
	virtual bool GetAchievementDescription( idLocalUser* user, const int id, achievementDescription_t& data ) const { return false; }

	//! Returns the storage size required for trophies and returns true on success.
	virtual bool GetRequiredStorage( uint64& requiredSizeTrophiesBytes )
	{
		requiredSizeTrophiesBytes = 0;
		return true;
	}

	//! Retrieves the cached local state of all achievements for a given user.
	virtual bool GetAchievementState( idLocalUser* user, idArray<bool, idAchievementSystem::MAX_ACHIEVEMENTS>& achievements ) const { return false; }

	//! Sets the state of all achievements in the provided list for debug purposes and returns false if an error occurs.
	virtual bool SetAchievementState( idLocalUser* user, idArray<bool, idAchievementSystem::MAX_ACHIEVEMENTS>& achievements ) { return false; }

	//! Synchronizes achievement bits from the server cache to the user's profile
	void		 SyncAchievementBits( idLocalUser* user );

protected:
	//! Retrieves the index of the specified local user in the user list.
	int											  GetLocalUserIndex( idLocalUser* user ) const { return users.FindIndex( user ); }

	idStaticList<idLocalUser*, MAX_LOCAL_PLAYERS> users;
};

#endif // __SYS_ACHIEVEMENTS_H__
