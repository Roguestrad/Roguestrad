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

#ifndef __GAME_WORLDSPAWN_H__
#define __GAME_WORLDSPAWN_H__

/*!
	\class idWorldspawn
	\brief The idWorldspawn class manages the main world entity and its initialization, saving, and restoration behavior.

	This class represents the primary world entity that is instantiated at the start of a map. It handles the setup of the game environment, including music track assignment and background music
   playback. The class is responsible for maintaining the world's state during save and load operations. It inherits from idEntity and is designed to be a singleton-like entity that governs core game
   world properties and behaviors. The class ensures proper cleanup of world references during destruction and provides mechanisms to handle the world's persistence.

*/
class idWorldspawn : public idEntity
{
public:
	CLASS_PROTOTYPE( idWorldspawn );

	//! Destructor for the idWorldspawn class that clears the world reference in gameLocal.
	~idWorldspawn();

	//! Initializes the worldspawn entity and sets up the game environment.
	void Spawn();

	//! Saves the worldspawn state to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the worldspawn state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Plays the background music track if one is set.
	void Event_PlayBackgroundMusic();

private:
	//! Sets the music track for the worldspawn based on spawn arguments or scans for available tracks.
	void  SetMusicTrack();

	idStr musicTrack;

	//! Error message when attempting to remove the world entity.
	void  Event_Remove();
};

#endif /* !__GAME_WORLDSPAWN_H__ */
