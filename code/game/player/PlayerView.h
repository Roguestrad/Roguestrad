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

#ifndef __GAME_PLAYERVIEW_H__
#define __GAME_PLAYERVIEW_H__

class idMenuHandler_HUD;

/*
===============================================================================

  Player view.

===============================================================================
*/

// screenBlob_t are for the on-screen damage claw marks, etc
typedef struct {
	const idMaterial* material;
	float			  x, y, w, h;
	float			  s1, t1, s2, t2;
	int				  finishTime;
	int				  startFadeTime;
	float			  driftAmount;
} screenBlob_t;

#define MAX_SCREEN_BLOBS 8

/*!
	\class WarpPolygon_t
	\brief A structure for representing warping polygons used in visual effects.
*/
class WarpPolygon_t
{
public:
	idVec4 outer1;
	idVec4 outer2;
	idVec4 center;
};

/*!
	\class Warp_t
	\brief A structure for managing warping effects.
*/
class Warp_t
{
public:
	int											 id;
	bool										 active;

	int											 startTime;
	float										 initialRadius;

	idVec3										 worldOrigin;
	idVec2										 screenOrigin;

	int											 durationMsec;

	idList<WarpPolygon_t, TAG_IDLIB_LIST_PLAYER> polys;
};

class idPlayerView;
class FullscreenFXManager;

/*!
	\class FxFader
	\brief FxFader manages fade effects with configurable timing and trigger states.

	The FxFader class is designed to handle fade effects, typically used for visual transitions or audio fades. It maintains a trigger state that can be activated or deactivated, with ramping behavior
   during state transitions. The class supports serialization through save and restore operations, allowing fade states to persist across game sessions. Timing parameters control the duration of fade
   transitions, and the current alpha value represents the interpolation factor for the fade effect. The class is intended to be used in scenarios requiring smooth transitions between states, such as
   screen fades or audio volume adjustments.

*/
class FxFader
{
private:
	enum { FX_STATE_OFF, FX_STATE_RAMPUP, FX_STATE_RAMPDOWN, FX_STATE_ON };

	int	  time;
	int	  state;
	float alpha;
	int	  msec;

public:
	//! Initializes a new instance of the FxFader class with default values.
	FxFader();

	//! Sets the trigger state for the fader, transitioning between on and off states with ramping behavior.
	bool		 SetTriggerState( bool active );

	//! Saves the FxFader state to the provided save file
	virtual void Save( idSaveGame* savefile );

	//! Restores the fader state from a save file.
	virtual void Restore( idRestoreGame* savefile );

	// fader functions
	void		 SetFadeTime( int t ) { msec = t; };
	int			 GetFadeTime() { return msec; };

	// misc functions
	float		 GetAlpha() { return alpha; };
};

/*!
	\class FullscreenFX
	\brief Base class for fullscreen effects in the rendering system.

	This class serves as an abstract base for implementing various fullscreen visual effects within the rendering pipeline. It provides a standardized interface for managing effect initialization,
   activation states, quality settings, and rendering passes. The class includes functionality for handling fade transitions, effect naming, and integration with a manager class that oversees multiple
   fullscreen effects. It supports both high and low quality rendering modes, and provides mechanisms for accumulating rendering passes. The interface is designed to be extended by concrete fullscreen
   effect implementations, each providing specific rendering behavior while adhering to a common contract.

*/
class FullscreenFX
{
protected:
	idStr				 name;
	FxFader				 fader;
	FullscreenFXManager* fxman;

public:
	FullscreenFX() { fxman = NULL; };
	virtual ~FullscreenFX() {};

	virtual void Initialize()  = 0;
	virtual bool Active()	   = 0;
	virtual void HighQuality() = 0;
	virtual void LowQuality() {};
	virtual void AccumPass( const renderView_t* view ) {};
	virtual bool HasAccum() { return false; };

	void		 SetName( idStr n ) { name = n; };
	idStr		 GetName() { return name; };

	void		 SetFXManager( FullscreenFXManager* fx ) { fxman = fx; };

	bool		 SetTriggerState( bool state ) { return fader.SetTriggerState( state ); };
	void		 SetFadeSpeed( int msec ) { fader.SetFadeTime( msec ); };
	float		 GetFadeAlpha() { return fader.GetAlpha(); };

	//! Saves the fullscreen effect fader state to the provided save file.
	virtual void Save( idSaveGame* savefile );

	//! Restores the fullscreen effect fader state from a save file.
	virtual void Restore( idRestoreGame* savefile );
};

/*!
	\class FullscreenFX_Helltime
	\brief Handles the fullscreen Helltime effect implementation.

	This class implements the fullscreen Helltime effect, which modifies the visual appearance of the screen based on player power-ups and game states. It inherits from FullscreenFX and provides
   specific functionality for managing the Helltime effect's activation, rendering passes, and state restoration. The class initializes required materials, determines the effect level, and handles
   both high-quality rendering and accumulation passes for the effect.

*/
class FullscreenFX_Helltime : public FullscreenFX
{
private:
	const idMaterial* initMaterial;
	const idMaterial* captureMaterials[3];
	const idMaterial* drawMaterial;
	bool			  clearAccumBuffer;

	//! Returns the helltime effect level based on player power-ups and test settings.
	int				  DetermineLevel();

public:
	//! Initializes the Helltime fullscreen effect by loading required materials and setting initial state.
	virtual void Initialize();

	//! Determines if the helltime fullscreen effect is currently active.
	virtual bool Active();

	//! Applies a high-quality fullscreen effect using a specified material.
	virtual void HighQuality();

	//! Performs the accumulation pass for the Helltime fullscreen effect.
	virtual void AccumPass( const renderView_t* view );
	virtual bool HasAccum() { return true; };

	//! Restores the fullscreen effect fader state from a save file and ensures the accumulation buffer is cleared.
	virtual void Restore( idRestoreGame* savefile );
};

/*!
	\class FullscreenFX_Multiplayer
	\brief Handles fullscreen visual effects specifically for multiplayer gameplay scenarios.

	This class extends base fullscreen effect functionality to support multiplayer-specific rendering requirements. It manages the initialization and activation of visual effects tailored for
   multiplayer modes, including quality settings and accumulation passes. The class integrates with multiplayer test settings and player power-ups to determine appropriate effect levels. It provides
   methods for setting up high-quality rendering, performing accumulation passes, and restoring effect states from save files. The class maintains its own initialization flags and accumulation buffer
   management while building upon inherited fullscreen effect capabilities.

*/
class FullscreenFX_Multiplayer : public FullscreenFX
{
private:
	const idMaterial* initMaterial;
	const idMaterial* captureMaterial;
	const idMaterial* drawMaterial;
	bool			  clearAccumBuffer;

	//! Determines the fullscreen effect level based on multiplayer test settings and player power-ups
	int				  DetermineLevel();

public:
	//! Initializes the multiplayer fullscreen effects by loading required materials and setting initialization flags.
	virtual void Initialize();

	//! Determines whether fullscreen effects are active for multiplayer mode.
	virtual bool Active();

	//! Sets up the fullscreen effect for high quality multiplayer rendering.
	virtual void HighQuality();

	//! Performs the accumulation pass for fullscreen effects in multiplayer mode.
	virtual void AccumPass( const renderView_t* view );
	virtual bool HasAccum() { return true; };

	//! Restores the multiplayer fullscreen FX state from a save file and initializes the accumulation buffer clear flag.
	virtual void Restore( idRestoreGame* savefile );
};

/*!
	\class FullscreenFX_Warp
	\brief A fullscreen effect that renders warped polygon graphics with interpolation and transition capabilities.

	This class implements a fullscreen warp effect that can render polygon graphics with a warping distortion applied. It inherits from FullscreenFX and provides functionality for initializing the
   effect, drawing warped polygons with interpolation, and managing its active state. The effect supports both standard and high-quality rendering modes, and can be enabled or disabled. It also
   handles saving and restoring its state for game persistence.

*/
class FullscreenFX_Warp : public FullscreenFX
{
private:
	const idMaterial* material;
	bool			  grabberEnabled;
	int				  startWarpTime;

	//! Draws a warped polygon effect using the provided warp polygon and interpolation factor.
	void			  DrawWarp( WarpPolygon_t wp, float interp );

public:
	//! Initializes the fullscreen warp effect by loading its material and resetting its state.
	virtual void Initialize();

	//! Returns true if the warp effect is currently active.
	virtual bool Active();

	//! Renders a high-quality warp effect during the warp transition
	virtual void HighQuality();

	void		 EnableGrabber( bool active )
	{
		grabberEnabled = active;
		startWarpTime  = gameLocal.slow.time;
	};

	//! Saves the fullscreen warp effect state to the provided save file.
	virtual void Save( idSaveGame* savefile );

	//! Restores the state of the fullscreen warp effect from a save file.
	virtual void Restore( idRestoreGame* savefile );
};

/*!
	\class FullscreenFX_EnviroSuit
	\brief Manages the environmental suit fullscreen effect display.
*/
class FullscreenFX_EnviroSuit : public FullscreenFX
{
private:
	const idMaterial* material;

public:
	//! Initializes the environment suit fullscreen effect by loading its associated material.
	virtual void Initialize();

	//! Returns true if the player is currently wearing the environmental suit power-up.
	virtual bool Active();

	//! Draws a high quality fullscreen effect using the enviro suit material.
	virtual void HighQuality();
};

/*!
	\class FullscreenFX_DoubleVision
	\brief Manages the double vision fullscreen visual effect.
*/
class FullscreenFX_DoubleVision : public FullscreenFX
{
private:
	const idMaterial* material;

public:
	//! Initializes the double vision fullscreen effect by loading its material
	virtual void Initialize();

	//! Checks if the double vision effect is currently active.
	virtual bool Active();

	//! Applies a high-quality double vision effect to the fullscreen rendering.
	virtual void HighQuality();
};

/*!
	\class FullscreenFX_InfluenceVision
	\brief Provides fullscreen visual effects for influence vision gameplay mechanics.
*/
class FullscreenFX_InfluenceVision : public FullscreenFX
{
public:
	//! Initializes the FullscreenFX_InfluenceVision system.
	virtual void Initialize();

	//! Returns true if the influence vision fullscreen effect is currently active.
	virtual bool Active();

	//! Applies a high-quality influence vision effect based on the player's proximity to an influence entity.
	virtual void HighQuality();
};

/*!
	\class FullscreenFX_Bloom
	\brief Manages bloom fullscreen effects with initialization, rendering, and state persistence.
*/
class FullscreenFX_Bloom : public FullscreenFX
{
private:
	const idMaterial* drawMaterial;
	const idMaterial* initMaterial;

	float			  currentIntensity;
	float			  targetIntensity;

public:
	//! Initializes the bloom fullscreen effect by loading required materials and resetting intensity values.
	virtual void Initialize();

	//! Returns true if bloom effects are enabled for the current player.
	virtual bool Active();

	//! Performs high quality bloom effect rendering.
	virtual void HighQuality();

	//! Saves the bloom effect parameters to the specified save file.
	virtual void Save( idSaveGame* savefile );

	//! Restores the bloom effect state from a save file.
	virtual void Restore( idRestoreGame* savefile );
};

/*!
	\class FullscreenFXManager
	\brief Manages fullscreen visual effects for the rendering system.

	The FullscreenFXManager class is responsible for creating, initializing, and processing fullscreen visual effects within the rendering pipeline. It maintains a collection of fullscreen effects
   that can be created by type and name, and supports saving and restoring their state. The manager integrates with a player view to apply effects during rendering and provides functionality to blend
   effects back into the scene. It is designed to handle the lifecycle of fullscreen effects including their initialization, processing, and cleanup.

*/
class FullscreenFXManager
{
private:
	idList<FullscreenFX*, TAG_FX> fx;

	idPlayerView*				  playerView;
	const idMaterial*			  blendBackMaterial;

	//! Creates a fullscreen effect of the specified type and adds it to the manager.
	void						  CreateFX( idStr name, idStr fxtype, int fade );

public:
	//! Initializes a new instance of the FullscreenFXManager class.
	FullscreenFXManager();

	//! Destructor for the FullscreenFXManager class that cleans up allocated resources.
	virtual ~FullscreenFXManager();

	//! Initializes the fullscreen FX manager with the specified player view.
	void		  Initialize( idPlayerView* pv );

	//! Processes fullscreen effects for the given render view
	void		  Process( const renderView_t* view );

	//! Performs a fullscreen alpha blend back operation with the specified alpha value.
	void		  Blendback( float alpha );

	idPlayerView* GetPlayerView() { return playerView; };
	idPlayer*	  GetPlayer() { return gameLocal.GetLocalPlayer(); };

	int			  GetNum() { return fx.Num(); };
	FullscreenFX* GetFX( int index ) { return fx[index]; };

	//! Finds and returns a fullscreen FX by its name
	FullscreenFX* FindFX( idStr name );

	//! Saves all fullscreen effects managed by the FullscreenFXManager to the provided save file.
	void		  Save( idSaveGame* savefile );

	//! Restores the state of all fullscreen effects managed by this manager from the provided save file.
	void		  Restore( idRestoreGame* savefile );
};

/*!
	\class idPlayerView
	\brief Manages the player's visual perspective and rendering effects including damage feedback, weapon recoil, and special visual effects.

	Handles the visualization of the player's viewpoint, including management of various visual effects such as damage impulse responses, weapon fire feedback, screen fading, and warping effects. The
   class integrates with player entity state and supports rendering in both stereo and single view modes with optional VR support. It maintains internal state for kick angles, shake effects, and timed
   visual effects like fades and flashes. The class is responsible for coordinating with HUD managers and render views to present the appropriate visual feedback to the player during gameplay.

*/
class idPlayerView
{
public:
	//! Initializes a new instance of the idPlayerView class.
	idPlayerView();

	//! Destructor for the idPlayerView class that cleans up the effect manager.
	~idPlayerView();

	//! Saves the player view state to a save file
	void	 Save( idSaveGame* savefile ) const;

	//! Restores the player view state from a save file.
	void	 Restore( idRestoreGame* savefile );

	//! Sets the player entity for the player view.
	void	 SetPlayerEntity( class idPlayer* playerEnt );

	//! Clears all visual effects and timed states of the player view.
	void	 ClearEffects();

	//! Applies damage impulse effects to the player's view based on local kick direction and damage definition
	void	 DamageImpulse( idVec3 localKickDir, const idDict* damageDef );

	//! Applies weapon fire feedback such as head twitches and recoil angles when a weapon is fired.
	void	 WeaponFireFeedback( const idDict* weaponDef );

	//! Returns the current kick angle applied to the player view.
	idAngles AngleOffset() const;

	//! Returns the current shake angle as a 3x3 matrix.
	idMat3	 ShakeAxis() const;

	//! Calculates the shake angle for the player view based on the current shake amplitude.
	void	 CalculateShake();

	//! Renders the player's view, handling both stereo and single view modes with optional VR cinematic effects and fading.
	void	 RenderPlayerView( idMenuHandler_HUD* hudManager );

	//! Emits a stereo eye view for the specified eye with optional VR support.
	void	 EmitStereoEyeView( const int eye, idMenuHandler_HUD* hudManager );

	//! Sets a fade effect with the specified color and duration for the player view.
	void	 Fade( idVec4 color, int time );

	//! Flashes the player view with the specified color for the given duration.
	void	 Flash( idVec4 color, int time );

	// temp for view testing
	void	 EnableBFGVision( bool b ) { bfgVision = b; };

private:
	//! Renders the player's view with optional HUD elements and special effects
	void		  SingleView( const renderView_t* view, idMenuHandler_HUD* hudManager );

	//! Applies a screen fade effect based on the current fade parameters.
	void		  ScreenFade();

	//! Returns a pointer to the oldest screen blob in the player view.
	screenBlob_t* GetScreenBlob();

	screenBlob_t  screenBlobs[MAX_SCREEN_BLOBS];

public:
	int					 dvFinishTime; // double vision will be stopped at this time

	int					 kickFinishTime; // view kick will be stopped at this time
	idAngles			 kickAngles;

	bool				 bfgVision; //

	const idMaterial*	 tunnelMaterial;	 // health tunnel vision
	const idMaterial*	 armorMaterial;		 // armor damage view effect
	const idMaterial*	 berserkMaterial;	 // berserk effect
	const idMaterial*	 irGogglesMaterial;	 // ir effect
	const idMaterial*	 bloodSprayMaterial; // blood spray
	const idMaterial*	 bfgMaterial;		 // when targeted with BFG
	float				 lastDamageTime;	 // accentuate the tunnel effect for a while

	idVec4				 fadeColor;		// fade color
	idVec4				 fadeToColor;	// color to fade to
	idVec4				 fadeFromColor; // color to fade from
	float				 fadeRate;		// fade rate
	int					 fadeTime;		// fade time

	idAngles			 shakeAng; // from the sound sources

	idPlayer*			 player;
	renderView_t		 view;

	FullscreenFXManager* fxManager;

public:
	/*!
		\brief Adds a warp effect to the player's vision at the specified world origin

		This function creates and enables a warp effect in the player's view at the given world coordinates. The effect is centered at the specified coordinates with an initial radius and duration.
	   The function returns a value indicating the success or state of the operation.

		\param worldOrigin The world coordinates where the warp effect should be centered
		\param centerx The x coordinate of the warp effect center
		\param centery The y coordinate of the warp effect center
		\param initialRadius The initial radius of the warp effect
		\param durationMsec The duration of the warp effect in milliseconds
		\return Returns 1 to indicate that the warp effect was successfully added or enabled
	*/
	int	 AddWarp( idVec3 worldOrigin, float centerx, float centery, float initialRadius, float durationMsec );

	//! Disables the warp effect grabber for the specified effect ID.
	void FreeWarp( int id );
};

//! Returns true if stereo rendering is active for the game.
bool IsGameStereoRendered();

#endif /* !__GAME_PLAYERVIEW_H__ */
