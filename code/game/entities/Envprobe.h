/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans

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

#ifndef __GAME_ENVIRONMENTPROBE_H__
#define __GAME_ENVIRONMENTPROBE_H__

/*!
	\class EnvironmentProbe
	\brief EnvironmentProbe represents a dynamic lighting and reflection source that can be configured and controlled through various methods.

	This class implements an environment probe entity that can be used to simulate reflective surfaces and dynamic lighting effects within a 3D scene. The probe maintains state information including
   color properties, shader parameters, and visibility settings. It provides functionality for spawning, saving, and restoring probe states, as well as controlling its visual appearance through fading
   effects and color manipulation. The class supports both client and server-side operations, with methods for network synchronization and client prediction. It also handles parent-child relationships
   with other entities and manages its own rendering presentation.

*/
class EnvironmentProbe : public idEntity
{
public:
	CLASS_PROTOTYPE( EnvironmentProbe );

	//! Initializes a new instance of the EnvironmentProbe class with default values.
	EnvironmentProbe();

	//! Destructor for the EnvironmentProbe class that releases the environment probe definition handle if it was allocated.
	~EnvironmentProbe();

	//! Initializes the environment probe entity after it has been spawned.
	void		  Spawn();

	//! Serializes the environment probe data to a save game file.
	void		  Save( idSaveGame* savefile ) const;

	//! Restores the environment probe state from a save game file.
	void		  Restore( idRestoreGame* savefile );

	//! Updates the environment probe's spawn arguments and visuals based on the provided source.
	virtual void  UpdateChangeableSpawnArgs( const idDict* source );

	//! Updates the environment probe's color fade effect and performs physics and presentation updates.
	virtual void  Think();

	//! Updates the environment probe's state and visual representation based on the current time and prediction settings.
	virtual void  ClientThink( const int curTime, const float fraction, const bool predict );

	//! Releases the environment probe definition handle if it is valid.
	virtual void  FreeEnvprobeDef();

	//! Updates the environment probe visualization for rendering.
	void		  Present();

	//! Saves the environment probe state by copying relevant key-value pairs from the spawn arguments to the provided dictionary.
	void		  SaveState( idDict* args );

	//! Sets the color value for the environment probe using individual red, green, and blue components
	virtual void  SetColor( float red, float green, float blue );

	//! Sets the color for the environment probe including RGB components and alpha value
	virtual void  SetColor( const idVec4& color );

	//! Sets the base color for the environment probe and updates the light level
	void		  SetColor( const idVec3& color );

	//! Retrieves the color components of the environment probe and stores them in the provided vector.
	virtual void  GetColor( idVec3& out ) const;

	//! Retrieves the color values of the environment probe and stores them in the provided output vector.
	virtual void  GetColor( idVec4& out ) const;

	//! Returns the base color of the environment probe.
	const idVec3& GetBaseColor() const { return baseColor; }

	//! Sets the value of a shader parameter for the environment probe.
	void		  SetEnvprobeParm( int parmnum, float value );

	/*!
		\brief Sets the shader parameters for an environment probe using the provided float values.

		This function assigns the input float parameters to specific shader parameter indices within the environment probe's render data. The parameters correspond to red, green, blue, and alpha color
	   components respectively. After updating the shader parameters, it notifies the system of the change by calling PresentEnvprobeDefChange.

		\param parm0 Red color component value for the environment probe shader
		\param parm1 Green color component value for the environment probe shader
		\param parm2 Blue color component value for the environment probe shader
		\param parm3 Alpha transparency value for the environment probe shader
	*/
	void		  SetEnvprobeParms( float parm0, float parm1, float parm2, float parm3 );

	//! Enables the environment probe and synchronizes its shader animation with game time.
	void		  On();

	//! Turns off the environment probe.
	void		  Off();

	//! Fades the environment probe color to the specified target color over a given time period.
	void		  Fade( const idVec4& to, float fadeTime );

	//! Fades out the environment probe to black over the specified time period.
	void		  FadeOut( float time );

	//! Starts a fade-in animation for the environment probe over the specified time period.
	void		  FadeIn( float time );

	//! Returns the handle to the environment probe definition.
	qhandle_t	  GetEnvprobeDefHandle() const { return envprobeDefHandle; }

	//! Sets the parent entity for the environment probe.
	void		  SetEnvprobeParent( idEntity* lparent ) { lightParent = lparent; }

	//! Sets the light level for the environment probe by calculating and applying a color intensity.
	void		  SetLightLevel();

	//! Displays the editing dialog for the environment probe.
	virtual void  ShowEditingDialog();

	enum { EVENT_BECOMEBROKEN = idEntity::EVENT_MAXEVENTS, EVENT_MAXEVENTS };

	//! Performs client-side prediction thinking for the environment probe.
	virtual void ClientPredictionThink();

	//! Writes environment probe data to a snapshot message for network synchronization
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads environment probe state from a snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );
	//	virtual bool	ClientReceiveEvent( int event, int time, const idBitMsg& msg );

private:
	renderEnvironmentProbe_t renderEnvprobe;	  // envprobe presented to the renderer
	idVec3					 localEnvprobeOrigin; // light origin relative to the physics origin
	idMat3					 localEnvprobeAxis;	  // light axis relative to physics axis
	qhandle_t				 envprobeDefHandle;	  // handle to renderer light def
	int						 levels;
	int						 currentLevel;
	idVec3					 baseColor;

	// Colors used for client-side interpolation.
	idVec3					 previousBaseColor;
	idVec3					 nextBaseColor;

	int						 count;
	int						 triggercount;
	idEntity*				 lightParent;
	idVec4					 fadeFrom;
	idVec4					 fadeTo;
	int						 fadeStart;
	int						 fadeEnd;

private:
	//! Updates or adds an environment probe definition in the render world.
	void PresentEnvprobeDefChange();

	//! Returns the value of a specified environment probe shader parameter.
	void Event_GetEnvprobeParm( int parmnum );

	//! Sets an environment probe parameter value by its index.
	void Event_SetEnvprobeParm( int parmnum, float value );

	/*!
		\brief Sets environment probe parameters for the environment probe.

		This function is an event handler that forwards the provided parameter values to the internal SetEnvprobeParms method. It is used to configure the environment probe settings such as
	   reflection, refraction, ambient, and specular parameters.

		\param parm0 First environment probe parameter
		\param parm1 Second environment probe parameter
		\param parm2 Third environment probe parameter
		\param parm3 Fourth environment probe parameter
	*/
	void Event_SetEnvprobeParms( float parm0, float parm1, float parm2, float parm3 );
	void Event_SetRadiusXYZ( float x, float y, float z );
	void Event_SetRadius( float radius );

	//! Hides the environment probe and turns it off.
	void Event_Hide();

	//! Shows the environment probe and enables it.
	void Event_Show();

	//! Enables the environment probe functionality.
	void Event_On();

	//! Turns off the environment probe.
	void Event_Off();

	//! Toggles the environment probe on or off based on trigger count and current level.
	void Event_ToggleOnOff( idEntity* activator );

	//! Initiates a fade-out effect for the environment probe over the specified time period.
	void Event_FadeOut( float time );

	//! Initiates a fade-in animation for the environment probe over the specified time period.
	void Event_FadeIn( float time );
};

#endif /* !__GAME_ENVIRONMENTPROBE_H__ */
