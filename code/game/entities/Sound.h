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

#ifndef __GAME_SOUND_H__
#define __GAME_SOUND_H__

/*!
	\class idSound
	\brief Manages sound playback and state for audio entities in the game.

	The idSound class extends idEntity to provide functionality for sound management including playback control, state persistence, and integration with game timing systems. It handles the
   initialization of sound emitters, updates from spawn arguments, and event-based triggering of sounds. The class supports saving and restoring sound states during game sessions, and manages the
   timing of sound events through dedicated timer logic. Sound playback can be controlled through direct method calls or through game events that trigger on/off states.

*/
class idSound : public idEntity
{
public:
	CLASS_PROTOTYPE( idSound );

	//! Initializes all member variables of the idSound class to their default values.
	idSound();

	//! Saves the sound state to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the sound state from a save file
	void		 Restore( idRestoreGame* savefile );

	//! Updates the sound emitter with changeable spawn arguments from the provided dictionary.
	virtual void UpdateChangeableSpawnArgs( const idDict* source );

	//! Initializes the sound object with spawn arguments and sets up timing and shaking parameters.
	void		 Spawn();

	void		 ToggleOnOff( idEntity* other, idEntity* activator );

	//! Executes the sound's think logic, including physics updates and visual state management.
	void		 Think();

	//! Sets the sound shader for this sound emitter and starts playing it if not already playing.
	void		 SetSound( const char* sound, int channel = SND_CHANNEL_ANY );

	//! Shows the sound editing dialog.
	virtual void ShowEditingDialog();

private:
	float	 lastSoundVol;
	float	 soundVol;
	float	 random;
	float	 wait;
	bool	 timerOn;
	idVec3	 shakeTranslate;
	idAngles shakeRotate;
	int		 playingUntilTime;

	//! Toggles the sound effect on and off based on timing and playback conditions.
	void	 Event_Trigger( idEntity* activator );

	//! Handles the sound timer event by playing the sound and scheduling the next timer event.
	void	 Event_Timer();

	//! Enables the sound event and schedules a timer if a wait time is specified.
	void	 Event_On();

	//! Stops the sound event and cancels any pending timer events.
	void	 Event_Off();

	//! Executes sound playback or stopping based on the play parameter.
	void	 DoSound( bool play );
};

#endif /* !__GAME_SOUND_H__ */
