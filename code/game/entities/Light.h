/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall
Copyright (C) 2021-2024 Robert Beckebans

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

#ifndef __GAME_LIGHT_H__
#define __GAME_LIGHT_H__

/*
===============================================================================

  Generic light.

===============================================================================
*/

extern const idEventDef EV_Light_GetLightParm;
extern const idEventDef EV_Light_SetLightParm;
extern const idEventDef EV_Light_SetLightParms;

// jmarshall
struct iceLightStyleState_t {
	//! Initializes a new instance of the iceLightStyleState_t class and resets its state.
	iceLightStyleState_t();

	int	  dl_frame;
	float dl_framef;
	int	  dl_oldframe;
	int	  dl_time;
	float dl_backlerp;

	//! Resets all light style state variables to their default values.
	void  Reset();
};

ID_INLINE iceLightStyleState_t::iceLightStyleState_t()
{
	Reset();
}

ID_INLINE void iceLightStyleState_t::Reset()
{
	dl_frame	= 0;
	dl_framef	= 0;
	dl_oldframe = 0;
	dl_time		= 0;
	dl_backlerp = 0;
}

/*!
	\class idLight
	\brief A light entity class that manages light properties, rendering, and behavior within a game engine.

	The idLight class represents a dynamic light source in the engine, handling its visual properties, spawning, saving, and restoration. It manages light definitions, color, radius, shader
   parameters, and interaction with the renderer and physics systems. The class supports various light behaviors including fading, activation, deactivation, and breaking states. It also handles
   networking synchronization through snapshot serialization and client-side prediction. The light can be associated with a parent entity and can target a model for visual representation. It
   interfaces with the engine's save system to persist light states and supports editor functionality for light configuration.

*/
class idLight : public idEntity
{
public:
	CLASS_PROTOTYPE( idLight );

	//! Constructs a new idLight object with default initializations.
	idLight();

	//! Destroys the light definition handle if it was allocated.
	~idLight();

	//! Initializes the light entity by parsing spawn arguments and setting up its properties.
	void				 Spawn();

	//! Saves the light object to a save game file.
	void				 Save( idSaveGame* savefile ) const;

	//! Restores the light object state from a save game file.
	void				 Restore( idRestoreGame* savefile );

	//! Updates the light's properties based on changeable spawn arguments.
	virtual void		 UpdateChangeableSpawnArgs( const idDict* source );

	//! Executes the light's think function, handling fading and updating its state.
	virtual void		 Think();

	//! Updates the light's state and visuals based on the current time and prediction.
	virtual void		 ClientThink( const int curTime, const float fraction, const bool predict );

	//! Frees the light definition handle if it is valid
	virtual void		 FreeLightDef();

	//! Returns the transformation from physics to sound coordinates for the light.
	virtual bool		 GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );

	//! Prepares the light entity for rendering by updating its visual properties and handling sound references.
	void				 Present();

	//! Performs shared thinking logic for the light class.
	virtual void		 SharedThink();

	//! Saves the light's state to the provided dictionary, excluding editor and parse keys.
	void				 SaveState( idDict* args );

	//! Sets the color of the light using red, green, and blue components
	virtual void		 SetColor( float red, float green, float blue );

	//! Sets the color value for the light including its alpha component
	virtual void		 SetColor( const idVec4& color );

	//! Sets the color of the light using the provided RGB vector.
	void				 SetColor( const idVec3& color );

	//! Returns the color of the light by filling the provided vector with red, green, and blue components
	virtual void		 GetColor( idVec3& out ) const;

	//! Retrieves the color of the light and stores it in the provided vector
	virtual void		 GetColor( idVec4& out ) const;

	//! Returns the base color of the light.
	const idVec3&		 GetBaseColor() const { return baseColor; }

	//! Returns the origin of the light in edit mode, calculated from the physics origin and local offset.
	virtual idVec3		 GetEditOrigin() const;

	//! Sets the shader for the light using the provided shader name.
	void				 SetShader( const char* shadername );

	//! Sets a light parameter value at the specified parameter index.
	void				 SetLightParm( int parmnum, float value );

	/*!
		\brief Sets the light parameters for red, green, blue, and alpha shader values.

		This function updates the shader parameters for a light's color and alpha values. It applies these values to both the light's render entity and, if a model target is set, to the target's
	   render entity as well. The changes are then propagated to the rendering system by calling PresentLightDefChange and PresentModelDefChange.

		\param parm0 Red color intensity value
		\param parm1 Green color intensity value
		\param parm2 Blue color intensity value
		\param parm3 Alpha (transparency) value
	*/
	void				 SetLightParms( float parm0, float parm1, float parm2, float parm3 );

	//! Sets the X, Y, and Z radius values for the light.
	void				 SetRadiusXYZ( float x, float y, float z );

	//! Sets the radius of the light in all three dimensions.
	void				 SetRadius( float radius );

	//! Activates the light and synchronizes its shader animation with the game time.
	void				 On();

	//! Turns off the light and stops any associated sound.
	void				 Off();

	//! Fades the light color from its current value to a specified color over a given time period.
	void				 Fade( const idVec4& to, float fadeTime );

	//! Fades the light out to black over the specified time period.
	void				 FadeOut( float time );

	//! Starts a fade-in animation for the light over the specified time period.
	void				 FadeIn( float time );

	/*!
		\brief Handles the death of the light entity by breaking it.

		This function is invoked when the light entity is killed, typically as a result of damage from an attacker. It triggers the light to break, which may involve visual effects or state changes.
	   The function does not use the inflictor, damage, direction, or location parameters, but passes the attacker to the BecomeBroken function.

		\param inflictor The entity that caused the damage
		\param attacker The entity that attacked the light
		\param damage Amount of damage dealt
		\param dir Direction from which the damage came
		\param location Location on the entity that was hit
	*/
	void				 Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Makes the light entity enter a broken state, updating its model, sound, and triggering associated events.
	void				 BecomeBroken( idEntity* activator );

	//! Returns the handle to the light definition.
	qhandle_t			 GetLightDefHandle() const { return lightDefHandle; }

	//! Sets the parent entity for this light.
	void				 SetLightParent( idEntity* lparent ) { lightParent = lparent; }

	//! Updates the light color based on the current light level.
	void				 SetLightLevel();

	//! Displays the editing dialog for the light entity.
	virtual void		 ShowEditingDialog();

	//! Returns the render light data structure for this light entity
	const renderLight_t& GetRenderLight() const { return renderLight; }

	enum { EVENT_BECOMEBROKEN = idEntity::EVENT_MAXEVENTS, EVENT_MAXEVENTS };

	//! Performs client-side prediction thinking for the light entity.
	virtual void ClientPredictionThink();

	//! Serializes the light's state to a bit message for network synchronization
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads light state from a snapshot message
	virtual void ReadFromSnapshot( const idBitMsg& msg );

	//! Handles client-side events for the light entity, specifically processing broken state changes and delegating other events to the parent entity class.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

private:
	renderLight_t				renderLight;	  // light presented to the renderer
	idVec3						localLightOrigin; // light origin relative to the physics origin
	idMat3						localLightAxis;	  // light axis relative to physics axis
	qhandle_t					lightDefHandle;	  // handle to renderer light def
	idStr						brokenModel;
	int							levels;
	int							currentLevel;
	idVec3						baseColor;

	// Colors used for client-side interpolation.
	idVec3						previousBaseColor;
	idVec3						nextBaseColor;

	bool						breakOnTrigger;
	int							count;
	// jmarshall
	int							lightStyle;
	int							lightStyleFrameTime;
	idVec3						lightStyleBase;
	// jmarshall end
	int							triggercount;
	idEntity*					lightParent;
	idVec4						fadeFrom;
	idVec4						fadeTo;
	int							fadeStart;
	int							fadeEnd;
	bool						soundWasPlaying;

	// RB: pointing to static model because this light entity was split into 2 entities by convertMapToValve220
	idEntityPtr<idStaticEntity> modelTarget;

private:
	//! Updates or adds the light definition to the renderer world.
	void				 PresentLightDefChange();

	//! Updates the light's visual representation in the render world.
	void				 PresentModelDefChange();

	//! Sets the shader for the light using the provided shader name.
	void				 Event_SetShader( const char* shadername );

	//! Returns the value of a specified light shader parameter.
	void				 Event_GetLightParm( int parmnum );

	//! Sets a light parameter value for the specified parameter number.
	void				 Event_SetLightParm( int parmnum, float value );

	/*!
		\brief Sets the light parameters for this light entity

		This function is an event handler that updates the light parameters of the light entity. It takes four floating-point values representing different light properties and passes them to the
	   internal SetLightParms function. The parameters typically control aspects such as light radius, color, intensity, and other lighting characteristics.

		\param parm0 First light parameter, typically representing light radius or intensity
		\param parm1 Second light parameter, typically representing light color or falloff
		\param parm2 Third light parameter, typically representing light color or shadow properties
		\param parm3 Fourth light parameter, typically representing light color or animation properties
	*/
	void				 Event_SetLightParms( float parm0, float parm1, float parm2, float parm3 );

	//! Sets the radius of the light in X, Y, and Z dimensions.
	void				 Event_SetRadiusXYZ( float x, float y, float z );

	//! Sets the radius of the light
	void				 Event_SetRadius( float radius );

	//! Hides the light's model target or the light itself, then presents the model definition change and turns the light off.
	void				 Event_Hide();

	//! Displays the light's model target or the light itself.
	void				 Event_Show();

	//! Enables the light source.
	void				 Event_On();

	//! Turns the light off.
	void				 Event_Off();

	//! Toggles the light state on or off based on trigger count and current level.
	void				 Event_ToggleOnOff( idEntity* activator );

	//! Sets the same sound definition handle on all targeted lights.
	void				 Event_SetSoundHandles();

	//! Initiates a fade-out effect for the light over the specified time period.
	void				 Event_FadeOut( float time );

	//! Initiates a fade-in animation for the light over the specified time period.
	void				 Event_FadeIn( float time );

	//! Updates the model target for the light entity.
	void				 Event_UpdateModelTarget();

	// jmarshall
	idList<idStr>		 light_styles;
	iceLightStyleState_t lightStyleState;
	// jmarshall end
};

#endif /* !__GAME_LIGHT_H__ */
