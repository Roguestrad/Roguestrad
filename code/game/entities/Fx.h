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

#ifndef __GAME_FX_H__
#define __GAME_FX_H__

/*
===============================================================================

  Special effects.

===============================================================================
*/

typedef struct {
	renderLight_t  renderLight;	   // light presented to the renderer
	qhandle_t	   lightDefHandle; // handle to renderer light def
	renderEntity_t renderEntity;   // used to present a model to the renderer
	int			   modelDefHandle; // handle to static renderer model
	float		   delay;
	int			   particleSystem;
	int			   start;
	bool		   soundStarted;
	bool		   shakeStarted;
	bool		   decalDropped;
	bool		   launched;
} idFXLocalAction;

/*!
	\class idEntityFx
	\brief Manages visual effects for entities with support for spawning, running, and cleaning up particle systems and other dynamic effects.

	The idEntityFx class extends entity functionality to handle visual effects such as particle systems, lights, and other dynamic effects. It provides methods to initialize effects, execute them over
   time, and manage their lifecycle including starting, stopping, and cleaning up resources. The class supports binding effects to entities, setting custom origins and orientations, and handling
   network synchronization for multi-player environments. It also includes functionality for triggering effects, clearing them, and applying fade effects to visual components. The class is designed to
   work with an effect system that manages multiple actions and can handle both client-side and server-side operations for visual effects.

*/
class idEntityFx : public idEntity
{
public:
	CLASS_PROTOTYPE( idEntityFx );

	//! Initializes a new instance of the idEntityFx class.
	idEntityFx();

	//! Destroys the idEntityFx object and cleans up associated resources.
	virtual ~idEntityFx();

	//! Initializes the entity effect by setting up the effect system and handling activation events.
	void			   Spawn();

	//! Saves the entity effect state to a save file.
	void			   Save( idSaveGame* savefile ) const;

	//! Restores the entity FX state from a save file.
	void			   Restore( idRestoreGame* savefile );

	//! Executes the entity's visual effects logic and updates its state.
	virtual void	   Think();

	//! Initializes the entity effect system with the specified effect name
	void			   Setup( const char* fx );

	//! Executes a frame of the effect system for the entity
	void			   Run( int time );

	//! Initializes the entity effects system with the specified start time.
	void			   Start( int time );

	//! Stops the entity effects and cleans up resources.
	void			   Stop();

	//! Returns the maximum duration of all actions in the entity's effect.
	const int		   Duration();

	//! Returns the name of the effect associated with this entity.
	const char*		   EffectName();

	//! Returns the name of the joint associated with the entity effect.
	const char*		   Joint();

	//! Returns true if the effect has finished based on its start time and duration.
	const bool		   Done();

	//! Writes the entity's effects data to a network snapshot message
	virtual void	   WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads entity effect data from a network snapshot message
	virtual void	   ReadFromSnapshot( const idBitMsg& msg );

	//! Executes client-side thinking logic for the entity effects, including running and presenting the effects.
	virtual void	   ClientThink( const int curTime, const float fraction, const bool predict );

	//! Executes client-side prediction logic for entity effects.
	virtual void	   ClientPredictionThink();

	/*!
		\brief Spawns and initializes a dynamic effect entity based on the provided parameters

		This function creates a new dynamic effect entity by spawning an entity of type idEntityFx using the specified effect name. The function handles binding the effect to an entity and setting its
	   initial position and orientation. It checks for various conditions such as whether effects are disabled, if the effect name is valid, and if the entity is not the world spawn before proceeding
	   with the creation.

		\param fx Name of the effect to spawn
		\param useOrigin Optional custom origin for the effect, or NULL to use entity origin
		\param useAxis Optional custom axis for the effect, or NULL to use entity axis
		\param ent Entity to bind the effect to, or NULL to not bind
		\param bind Flag indicating whether to bind the effect to the entity
		\return Pointer to the newly created idEntityFx instance, or NULL if creation failed or effects are disabled
	*/
	static idEntityFx* StartFx( const char* fx, const idVec3* useOrigin, const idMat3* useAxis, idEntity* ent, bool bind );

protected:
	//! Triggers the entitys effects and action with optional delay.
	void							Event_Trigger( idEntity* activator );

	//! Clears any visual effects that were started when the entity was spawned.
	void							Event_ClearFx();

	//! Cleans up the entity's FX effect by iterating through its events and cleaning up each action.
	void							CleanUp();

	//! Cleans up resources associated with a single effect action.
	void							CleanUpSingleAction( const idFXSingleAction& fxaction, idFXLocalAction& laction );

	/*!
		\brief Applies fade effect to visual and light components of a particle system action based on time and fade parameters.

		This function calculates a fade percentage based on the current time relative to the start time and the fade duration specified in the action. It then modifies the RGB shader parameters of
	   both the model and light components of the action to achieve a fade-in or fade-out effect. The fade effect is determined by the fadeInTime and fadeOutTime properties of the fxaction. If the
	   fade percentage exceeds 1.0, it is clamped to 1.0 to prevent over-fading. The function updates the rendering entities accordingly.

		\param fxaction Reference to the single action containing fade parameters and light color information
		\param laction Reference to the local action containing render entity and light definitions
		\param time Current time value used to calculate fade percentage
		\param actualStart Start time of the fade effect
	*/
	void							ApplyFade( const idFXSingleAction& fxaction, idFXLocalAction& laction, const int time, const int actualStart );

	int								started;
	int								nextTriggerTime;
	const idDeclFX*					fxEffect; // GetFX() should be called before using fxEffect as a pointer
	idList<idFXLocalAction, TAG_FX> actions;
	idStr							systemName;
};

/*!
	\class idTeleporter
	\brief A teleporter entity that moves specified entities to its location with a calculated angle.
*/
class idTeleporter : public idEntityFx
{
public:
	CLASS_PROTOTYPE( idTeleporter );

private:
	//! Teleports the specified entity to the teleporter's location with a calculated angle.
	void Event_DoAction( idEntity* activator );
};

#endif /* !__GAME_FX_H__ */
