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
#ifndef __WIN_ACHIEVEMENTS_H__
#define __WIN_ACHIEVEMENTS_H__

/*!
	\class idAchievementSystemWin
	\brief Windows-specific implementation of the achievement system interface.

	This class is stub

*/
class idAchievementSystemWin : public idAchievementSystem
{
public:
	//! Constructs a new instance of the Windows achievement system.
	idAchievementSystemWin();

	//! Returns false indicating the achievement system is not initialized.
	bool IsInitialized();

	//! Unlocks the specified achievement for the given local user.
	void AchievementUnlock( idLocalUser* user, const int achievementID );

	//! Locks an achievement for the specified user.
	void AchievementLock( idLocalUser* user, const int achievementID );

	//! Locks all achievements up to the specified ID for the given user.
	void AchievementLockAll( idLocalUser* user, const int maxId );

	//! Processes achievement system updates each frame
	void Pump();

	//! Retrieves textual information about a given achievement and returns false indicating no implementation is provided.
	bool GetAchievementDescription( idLocalUser* user, const int id, achievementDescription_t& data ) const;

	//! Returns the achievement state for a given user
	bool GetAchievementState( idLocalUser* user, idArray<bool, idAchievementSystem::MAX_ACHIEVEMENTS>& achievements ) const;
};

#endif // __WIN_ACHIEVEMENTS_H__
