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

#ifndef __GAME_TARGET_H__
#define __GAME_TARGET_H__

/*!
	\class idTarget
	\brief Represents a target entity that can be referenced by other entities in the game.
*/
class idTarget : public idEntity
{
public:
	CLASS_PROTOTYPE( idTarget );
};

/*!
	\class idTarget_Remove
	\brief A target entity that removes all targeted entities and then destroys itself.
*/
class idTarget_Remove : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Remove );

private:
	//! Removes all target entities and then removes itself.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_Show
	\brief A target entity that shows all target entities and removes itself from the game when activated.
*/
class idTarget_Show : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Show );

private:
	//! Shows all target entities and removes itself from the game.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_Damage
	\brief A target entity that damages other entities when activated.
*/
class idTarget_Damage : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Damage );

private:
	//! Handles the activation event to damage target entities.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SessionCommand
	\brief A target entity that executes a session command upon activation.
*/
class idTarget_SessionCommand : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SessionCommand );

private:
	//! Sets the session command to be executed when the target is activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_EndLevel
	\brief Handles the activation event for ending the current level, managing game flow and achievements.
*/
class idTarget_EndLevel : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_EndLevel );

private:
	//! Handles the activation event for ending the current level, managing game flow and achievements based on the map and player state.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_WaitForButton
	\brief A target that waits for a button press event before activating other targets.
*/
class idTarget_WaitForButton : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_WaitForButton );

	//! Processes the waiting for a button press event and activates targets when the attack button is detected.
	void Think();

private:
	//! Toggles the active state of the wait-for-button target based on its current state and cinematic settings.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SetGlobalShaderTime
	\brief A target entity that sets a global shader time parameter upon activation.
*/
class idTarget_SetGlobalShaderTime : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetGlobalShaderTime );

private:
	//! Sets a global shader parameter to the negative time value when activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SetShaderParm
	\brief A target entity that sets shader parameters and color on other entities when activated.
*/
class idTarget_SetShaderParm : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetShaderParm );

private:
	//! Sets shader parameters and color on target entities when activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SetShaderTime
	\brief A target entity that sets shader time offset on activated.
*/
class idTarget_SetShaderTime : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetShaderTime );

private:
	//! Sets shader time offset for target entities when activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_FadeEntity
	\brief Manages fade animations for target entities.
*/
class idTarget_FadeEntity : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_FadeEntity );

	//! Initializes a new instance of the idTarget_FadeEntity class with default fade values.
	idTarget_FadeEntity();

	//! Saves the fade entity state to the provided save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the fade entity state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Updates the fade animation for target entities.
	void Think();

private:
	idVec4 fadeFrom;
	int	   fadeStart;
	int	   fadeEnd;

	//! Activates the fade entity effect on targeted entities.
	void   Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_LightFadeIn
	\brief A target entity that triggers a light fade-in effect on targeted lights.
*/
class idTarget_LightFadeIn : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_LightFadeIn );

private:
	//! Activates the light fade-in effect on targeted light entities.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_LightFadeOut
	\brief Manages activation of light fade out effects on targeted lights.
*/
class idTarget_LightFadeOut : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_LightFadeOut );

private:
	//! Activates the light fade out effect on targeted lights.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_Give
	\brief Handles giving items to players when activated.
*/
class idTarget_Give : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Give );

	//! Activates the target immediately if the 'onSpawn' spawn flag is set.
	void Spawn();

private:
	//! Activates the target to give items to the player.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_GiveEmail
	\brief Handles the activation event to give an email to the player if a PDA is available.
*/
class idTarget_GiveEmail : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_GiveEmail );

private:
	//! Handles the activation event to give an email to the player if a PDA is available.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SetModel
	\brief Manages the dynamic model replacement of target entities.
*/
class idTarget_SetModel : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetModel );

	//! Initializes the target set model by loading the specified model if it doesn't already exist.
	void Spawn();

private:
	//! Sets the model of target entities to a new model specified in the spawn arguments.
	void Event_Activate( idEntity* activator );
};

/*
===============================================================================

idTarget_SetInfluence

===============================================================================
*/

typedef struct SavedGui_s {
	SavedGui_s() { memset( gui, 0, sizeof( idUserInterface* ) * MAX_RENDERENTITY_GUI ); };
	idUserInterface* gui[MAX_RENDERENTITY_GUI];
} SavedGui_t;

/*!
	\class idTarget_SetInfluence
	\brief Manages influence settings and entity interactions for targeted influence effects.

	This class implements a target entity that controls influence behaviors, including setting influence levels, gathering nearby entities, and managing visual and audio effects. It handles activation
   events to modify player influence, restore previous settings, and update the influence field of view. The class supports save and restore functionality for persistent influence states and includes
   methods for collecting entities based on type and properties. It integrates with the game's entity system to trigger effects and manage player interactions within influence fields.

*/
class idTarget_SetInfluence : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetInfluence );

	//! Constructs an idTarget_SetInfluence object with default values.
	idTarget_SetInfluence();

	//! Saves the influence target state to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the influence settings from a saved game file
	void Restore( idRestoreGame* savefile );

	//! Initializes the influence target entity and queues entity gathering.
	void Spawn();

private:
	//! Handles the activation event for a target that sets influence, including triggering targets, fading sounds, updating visuals, and modifying player influence level.
	void						   Event_Activate( idEntity* activator );

	//! Restores influence settings and entity states after a demonic influence event.
	void						   Event_RestoreInfluence();

	//! Collects and categorizes entities within a specified radius based on their types and specific properties.
	void						   Event_GatherEntities();

	//! Sets the flash effect for the player's view.
	void						   Event_Flash( float flash, int out );

	//! Clears the flash effect on the player's view with the specified duration.
	void						   Event_ClearFlash( float flash );

	//! Updates the player's influence field of view based on the current settings and deactivates when done.
	void						   Think();

	idList<int, TAG_TARGET>		   lightList;
	idList<int, TAG_TARGET>		   guiList;
	idList<int, TAG_TARGET>		   soundList;
	idList<int, TAG_TARGET>		   genericList;
	float						   flashIn;
	float						   flashOut;
	float						   delay;
	idStr						   flashInSound;
	idStr						   flashOutSound;
	idEntity*					   switchToCamera;
	idInterpolate<float>		   fovSetting;
	bool						   soundFaded;
	bool						   restoreOnTrigger;

	idList<SavedGui_t, TAG_TARGET> savedGuiList;
};

/*!
	\class idTarget_SetKeyVal
	\brief A target entity that sets key-value pairs on other entities when activated.
*/
class idTarget_SetKeyVal : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetKeyVal );

private:
	//! Sets key-value pairs on target entities when activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SetFov
	\brief Manages field of view settings for target entities.
*/
class idTarget_SetFov : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetFov );

	//! Saves the field of view setting data to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the field of view settings from a save file.
	void Restore( idRestoreGame* savefile );

	//! Updates the player's field of influence based on the current FOV setting.
	void Think();

private:
	idInterpolate<float> fovSetting;

	//! Activates the field of view setting event for the target entity.
	void				 Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_SetPrimaryObjective
	\brief A target entity that sets the primary objective for the local player upon activation.
*/
class idTarget_SetPrimaryObjective : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_SetPrimaryObjective );

private:
	//! Sets the primary objective for the local player when activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_LockDoor
	\brief A target entity that toggles the locked state of targeted doors.
*/
class idTarget_LockDoor : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_LockDoor );

private:
	//! Toggles the locked state of doors targeted by this lock door entity.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_CallObjectFunction
	\brief A target entity that activates object functions on its targets when triggered.
*/
class idTarget_CallObjectFunction : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_CallObjectFunction );

private:
	//! Activates the target object function by calling it on all valid targets with the specified function name.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_EnableLevelWeapons
	\brief A target entity that enables or disables level weapons based on spawn arguments and client activation.
*/
class idTarget_EnableLevelWeapons : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_EnableLevelWeapons );

private:
	//! Enables or disables level weapons based on the spawn arguments and activates the appropriate weapon for clients.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_Tip
	\brief Manages tip messages displayed to players in response to entity activation.

	The idTarget_Tip class handles the display and management of tip messages for players. It inherits from idTarget and provides functionality to show tip messages when activated, hide them based on
   player distance, and manage player position tracking. The class supports save and restore operations for player position data, allowing the tip system to persist across save states. The tip display
   logic includes scheduling follow-up events to manage visibility based on player proximity, ensuring tips are only shown when appropriate. The class is designed to work within an entity-based system
   where activation events trigger tip display behavior.

*/
class idTarget_Tip : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Tip );

	//! Initializes a new instance of the idTarget_Tip class with default values.
	idTarget_Tip();

	//! Initializes the idTarget_Tip entity.
	void Spawn();

	//! Saves the player position to the specified save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the player position from the save file.
	void Restore( idRestoreGame* savefile );

private:
	idVec3 playerPos;

	//! Displays a tip message to the player when the target is activated.
	void   Event_Activate( idEntity* activator );

	//! Hides the tip display for the local player if the player is far away, otherwise schedules another tip off event.
	void   Event_TipOff();

	//! Retrieves the player's position and schedules a follow-up event.
	void   Event_GetPlayerPos();
};

/*!
	\class idTarget_GiveSecurity
	\brief A target that gives security to the player upon activation.
*/
class idTarget_GiveSecurity : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_GiveSecurity );

private:
	//! Handles the activation event to give security to the player.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_RemoveWeapons
	\brief Removes all non-essential weapons from players and selects the fists weapon.
*/
class idTarget_RemoveWeapons : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_RemoveWeapons );

private:
	//! Removes all non-essential weapons from all players and selects the fists weapon.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_LevelTrigger
	\brief A target entity that triggers level-specific events for all players.
*/
class idTarget_LevelTrigger : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_LevelTrigger );

private:
	//! Sets the level trigger for all players when activated.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_Checkpoint
	\brief A checkpoint target that saves the game state when activated.
*/
class idTarget_Checkpoint : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Checkpoint );

private:
	//! Saves the game to an autosave slot when the checkpoint target is activated and conditions are met.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_EnableStamina
	\brief A target entity that controls the player's stamina system.
*/
class idTarget_EnableStamina : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_EnableStamina );

private:
	//! Enables or disables player stamina based on the spawn argument setting.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_FadeSoundClass
	\brief Manages fading sound effects for specific sound classes.
*/
class idTarget_FadeSoundClass : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_FadeSoundClass );

private:
	//! Activates the sound fading effect for a specified sound class with optional fade duration.
	void Event_Activate( idEntity* activator );

	//! Restores the volume of sound classes by fading them back in.
	void Event_RestoreVolume();
};

/*!
	\class idTarget_RumbleJoystick
	\brief Class for controlling joystick rumble effects on local player.
*/
class idTarget_RumbleJoystick : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_RumbleJoystick );

private:
	//! Activates joystick rumble effect on the local player based on spawn arguments.
	void Event_Activate( idEntity* activator );
};

/*!
	\class idTarget_Achievement
	\brief A target class for handling achievement activation events.
*/
class idTarget_Achievement : public idTarget
{
public:
	CLASS_PROTOTYPE( idTarget_Achievement );

private:
	//! Handles the activation event for an achievement target.
	void Event_Activate( idEntity* activator );
};

#endif /* !__GAME_TARGET_H__ */
