/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

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

#ifndef __GAME_WEAPON_H__
#define __GAME_WEAPON_H__

#include "PredictedValue.h"

/*
===============================================================================

	Player Weapon

===============================================================================
*/

extern const idEventDef EV_Weapon_State;

typedef int				ammo_t;
static const int		AMMO_NUMTYPES = 16;

class idPlayer;

static const int LIGHTID_WORLD_MUZZLE_FLASH = 1;
static const int LIGHTID_VIEW_MUZZLE_FLASH	= 100;

class idMoveableItem;

typedef struct {
	char				  name[64];
	char				  particlename[128];
	bool				  active;
	int					  startTime;
	jointHandle_t		  joint;	// The joint on which to attach the particle
	bool				  smoke;	// Is this a smoke particle
	const idDeclParticle* particle; // Used for smoke particles
	idFuncEmitter*		  emitter;	// Used for non-smoke particles
} WeaponParticle_t;

typedef struct {
	char		  name[64];
	bool		  active;
	int			  startTime;
	jointHandle_t joint;
	int			  lightHandle;
	renderLight_t light;
} WeaponLight_t;

/*!
	\class iceWeaponObject
	\brief A class that manages the behavior and state of an weapon object within the game engine.

	The iceWeaponObject class extends idClass and provides functionality for initializing, managing, and executing the state machine of a weapon. It supports setting and appending states,
   executing state threads, and checking the current state of the weapon. The class also handles weapon-specific behaviors such as firing, reloading, and holstering states. The implementation includes
   virtual methods for handling events like the weapon's owner dying, and provides utility functions for managing sounds associated with the weapon. The class is designed to interact with a weapon
   object and manage its lifecycle and behavior within the game's state management system.

*/
class iceWeaponObject : public idClass
{
public:
	CLASS_PROTOTYPE( iceWeaponObject );

	//! Initializes the weapon object with the specified weapon.
	virtual void  Init( idWeapon* weapon );

	//! Sets the state of the weapon object using the provided state string
	void		  SetState( const char* state ) { stateThread.SetState( state ); }

	//! Appends a state to the weapon object's state thread.
	void		  AppendState( const char* state ) { stateThread.PostState( state ); }

	//! Executes the state thread associated with the ice weapon object.
	void		  Execute() { stateThread.Execute(); }

	//! Returns true if the weapon object's state thread is currently executing.
	bool		  IsRunning() { return stateThread.IsExecuting(); }

	//! Returns true if the specified state is currently active in the state thread.
	bool		  IsStateRunning( const char* name ) { return stateThread.CurrentStateIs( name ); }

	//! This function is a virtual placeholder that does nothing when the owner of the ice weapon object dies.
	virtual void  OwnerDied() { }

	//! Returns true if the weapon is currently firing or about to fire.
	bool		  IsFiring();

	//! Returns true if the weapon is currently in the reload state.
	bool		  IsReloading();

	//! Puts the weapon in a holstered state and waits.
	stateResult_t Holstered( stateParms_t* parms ) { return SRESULT_WAIT; }

	//! Returns true if the weapon is in the holstered state.
	virtual bool  IsHolstered() { return IsStateRunning( "Holstered" ); }

protected:
	idWeapon*			 owner;

	//! Returns a sound shader for the specified sound name, or NULL if not found
	const idSoundShader* FindSound( const char* name );

protected:
	rvStateThread stateThread;
	float		  next_attack;
};

/*!
	\class idWeapon
	\brief Manages weapon entities including firing, reloading, and visual effects.

	Handles the complete lifecycle and behavior of weapon entities within the game, from initialization and ownership management to firing mechanics, reloading, and visual feedback such as muzzle
   flashes and particle effects. The class supports both player-owned weapons and world models, providing methods for managing ammo, clip states, and weapon states like holstered or ready. It includes
   functionality for networking, scripting events, and integration with game UI elements such as crosshairs and PDA icons. The weapon can be raised, lowered, dropped, or stolen, and supports various
   firing modes including projectile launches, melee attacks, and power-up effects. Visual elements like flashlights, smoke, and particle effects are managed through dedicated methods, and the class
   integrates with the engine's animation and physics systems for realistic weapon behavior.

*/
class idWeapon : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idWeapon );

	//! Constructs a new idWeapon object with default values.
	idWeapon();

	//! Destroys the weapon object and cleans up its resources.
	virtual ~idWeapon();

	//! Initializes the weapon entity and sets up its world model and grabber.
	void			   Spawn();

	//! Sets the owner of the weapon to the specified player.
	void			   SetOwner( idPlayer* owner );

	//! Returns a pointer to the player who owns this weapon.
	idPlayer*		   GetOwner();

	//! Returns false to indicate that the weapon should not construct a script object at spawn.
	virtual bool	   ShouldConstructScriptObjectAtSpawn() const;

	//! Sets the flashlight owner for this weapon instance
	void			   SetFlashlightOwner( idPlayer* owner );

	//! Returns the current weapon object as an idClass instance.
	virtual idClass*   InvokeChild() override { return currentWeaponObject; }

	//! Pre-caches weapon assets including brass effects and GUI components.
	static void		   CacheWeapon( const char* weaponName );

	//! Saves the weapon state to a save game file.
	void			   Save( idSaveGame* savefile ) const;

	//! Restores the weapon state from a saved game file
	void			   Restore( idRestoreGame* savefile );

	//! Clears all weapon state and resets internal data structures to their default values.
	void			   Clear();

	//! Initializes weapon definition data from a given object name and ammo in clip value.
	void			   GetWeaponDef( const char* objectname, int ammoinclip );

	//! Returns true if the world model entity for this weapon is ready and valid.
	bool			   IsWorldModelReady();

	//! Returns the icon string associated with the weapon.
	const char*		   Icon() const;

	//! Updates the GUI elements to reflect the current weapon state and ammo information.
	void			   UpdateGUI();

	//! Returns the PDA icon string associated with this weapon.
	const char*		   PdaIcon() const;

	//! Returns the localized display name of the weapon.
	const char*		   DisplayName() const;

	//! Returns the localized description string for the weapon.
	const char*		   Description() const;

	//! Sets the model for the weapon to the specified model name.
	virtual void	   SetModel( const char* modelname );

	/*!
		\brief Retrieves the global transformation of a specified joint handle for either the view model or world model, returning true if successful.

		This function calculates the position and orientation of a joint in world space, which is useful for attaching additional models or lights to the weapon. When the view model flag is true, it
	   uses the view weapon's axis and origin for transformation. When false, it uses the world model's entity data, including its physics properties for accurate positioning. If the joint cannot be
	   found in either model, the function defaults to the view weapon origin and axis, returning false.

		\param viewModel Indicates whether to retrieve the joint transform for the view model (true) or the world model (false).
		\param jointHandle The handle of the joint for which the global transform is being retrieved.
		\param offset Output parameter that receives the translation component of the joint's transformation.
		\param axis Output parameter that receives the rotation component of the joint's transformation.
		\return True if the joint transform is successfully retrieved and calculated, false otherwise.
	*/
	bool			   GetGlobalJointTransform( bool viewModel, const jointHandle_t jointHandle, idVec3& offset, idMat3& axis );

	//! Sets the push velocity for the weapon.
	void			   SetPushVelocity( const idVec3& pushVelocity );

	//! Updates the skin of the weapon.
	bool			   UpdateSkin();

	//! Returns true if the weapon is currently firing.
	bool			   IsFiring() { return isFiring; }

	//! Executes the weapon's current object logic if it is not a flashlight.
	void			   Think();

	//! Sets the weapon object to the Raise state and transitions to Idle state afterward.
	void			   Raise();

	//! Hides the weapon from the view.
	void			   PutAway();

	//! Reloads the weapon and transitions to the idle state.
	void			   Reload();

	//! Lower the weapon by setting hide flags and timing parameters.
	void			   LowerWeapon();

	//! Raises the weapon by showing it and adjusting hide state.
	void			   RaiseWeapon();

	//! Hides the weapon and its associated world model
	void			   HideWeapon();

	//! Displays the weapon and its associated world model and muzzle flash light.
	void			   ShowWeapon();

	//! Hides the weapon's world model entity if it exists.
	void			   HideWorldModel();

	//! Shows the world model entity if it exists.
	void			   ShowWorldModel();

	//! Handles the weapon's behavior when its owner dies.
	void			   OwnerDied();

	//! Initializes the weapon attack sequence when the weapon is ready to fire.
	void			   BeginAttack();

	//! Marks the weapon as no longer firing.
	void			   EndAttack();

	//! Returns true if the weapon is ready to be used.
	bool			   IsReady() const;

	//! Checks if the weapon is currently in the reloading state.
	bool			   IsReloading() const;

	//! Checks if the weapon is currently in a holstered state.
	bool			   IsHolstered() const;

	//! Determines whether the crosshair should be displayed based on the weapon's current state and animation.
	bool			   ShowCrosshair() const;

	/*!
		\brief Creates and drops an item entity based on the weapon's definition with specified velocity and timing parameters.

		This function attempts to drop an item entity that represents the weapon being dropped. It first checks if the weapon has a valid definition and if the world model entity exists. It also
	   verifies that the weapon is allowed to be dropped. If all checks pass, it retrieves the classname for the drop item from the weapon's dictionary and proceeds to drop the item using the moveable
	   item drop functionality. The item is dropped with the provided velocity and delay settings for activation and removal.

		\param velocity The velocity vector to apply to the dropped item
		\param activateDelay The delay in milliseconds before the dropped item becomes active
		\param removeDelay The delay in milliseconds before the dropped item is removed
		\param died Indicates whether the weapon was dropped because the owner died
		\return A pointer to the dropped item entity, or NULL if the drop operation failed due to invalid state or missing definition.
		\throws NULL is returned if the weapon definition is invalid, the world model is missing, or if dropping is not allowed.
	*/
	idEntity*		   DropItem( const idVec3& velocity, int activateDelay, int removeDelay, bool died );

	//! Returns true if the weapon can be dropped, otherwise false.
	bool			   CanDrop() const;

	//! Handles the weapon being stolen by removing the projectile entity and hiding the weapon
	void			   WeaponStolen();

	//! Sets the ammo in the weapon's clip to the maximum clip size.
	void			   ForceAmmoInClip();

	//! Displays or hides the weapon model based on the showViewModel flag.
	void			   PresentWeapon( bool showViewModel );

	//! Returns the field of view used for zooming.
	int				   GetZoomFov();

	//! Returns the weapon angle offset parameters.
	void			   GetWeaponAngleOffsets( int* average, float* scale, float* max );

	//! Returns the weapon time offset and scale values.
	void			   GetWeaponTimeOffsets( float* time, float* scale );

	//! Applies a blood splat effect to the weapon model at the muzzle.
	bool			   BloodSplat( float size );

	//! Sets whether the weapon is a player flashlight.
	void			   SetIsPlayerFlashlight( bool bl ) { isPlayerFlashlight = bl; }

	//! Enables the flashlight effect for the weapon.
	void			   FlashlightOn();

	//! Turns off the weapon's flashlight.
	void			   FlashlightOff();

	//! Returns the ammo type identifier for the specified ammo name.
	static ammo_t	   GetAmmoNumForName( const char* ammoname );

	//! Returns the name of the ammo type corresponding to the given ammo number.
	static const char* GetAmmoNameForNum( ammo_t ammonum );

	//! Returns the pickup name for a given ammo number.
	static const char* GetAmmoPickupNameForNum( ammo_t ammonum );

	//! Returns the ammo type used by this weapon.
	ammo_t			   GetAmmoType() const;

	//! Returns the amount of ammo available for this weapon
	int				   AmmoAvailable() const;

	//! Returns the amount of ammunition currently in the weapon's clip.
	int				   AmmoInClip() const;

	//! Resets the weapon's ammo clip to its initial state.
	void			   ResetAmmoClip();

	//! Returns the clip size of the weapon.
	int				   ClipSize() const;

	//! Returns the low ammo threshold value for the weapon.
	int				   LowAmmo() const;

	//! Returns the amount of ammunition required for the weapon.
	int				   AmmoRequired() const;

	//! Returns the total number of rounds of the specified ammo type available to the weapon's owner.
	int				   AmmoCount() const;

	//! Returns the current grabber state of the weapon.
	int				   GetGrabberState() const;

	//! Returns the world model entity associated with this weapon.
	idAnimatedEntity*  GetWorldModel() { return worldModel.GetEntity(); }

	//! Writes weapon state information to a network snapshot message.
	virtual void	   WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads weapon state from a network snapshot message
	virtual void	   ReadFromSnapshot( const idBitMsg& msg );

	enum { EVENT_RELOAD = idEntity::EVENT_MAXEVENTS, EVENT_ENDRELOAD, EVENT_CHANGESKIN, EVENT_MAXEVENTS };

	//! Processes client-side weapon events such as reloads, end reloads, and skin changes.
	virtual bool		   ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Updates the weapon animation during client prediction.
	virtual void		   ClientPredictionThink();

	//! Updates the weapon animation based on the current time and prediction state.
	virtual void		   ClientThink( const int curTime, const float fraction, const bool predict );

	//! Updates and displays the muzzle flash light effect for the weapon.
	void				   MuzzleFlashLight();

	//! Removes the muzzle flash light effect from the weapon.
	void				   RemoveMuzzleFlashlight();

	//! Returns the global origin and axis for weapon muzzle position with special handling for certain weapons
	bool				   GetMuzzlePositionWithHacks( idVec3& origin, idMat3& axis );

	//! Retrieves the muzzle position and orientation of the weapon.
	bool				   GetMuzzlePosition( idVec3& origin, idMat3& axis );

	//! Retrieves the inverse handle transformation for a weapon's joint.
	bool				   GetInverseHandle( idVec3& origin, idMat3& axis );

	//! Returns the entity definition associated with this weapon.
	const idDeclEntityDef* GetDeclEntityDef() { return weaponDef; }

	friend class idPlayer;

private:
	int							  animBlendFrames;
	int							  animDoneTime;
	bool						  isLinked;
	bool						  isPlayerFlashlight;

	// precreated projectile
	idEntity*					  projectileEnt;

	idPlayer*					  owner;
	idEntityPtr<idAnimatedEntity> worldModel;

	// hiding (for GUIs and NPCs)
	int							  hideTime;
	float						  hideDistance;
	int							  hideStartTime;
	float						  hideStart;
	float						  hideEnd;
	float						  hideOffset;
	bool						  hide;
	bool						  disabled;

	bool						  isFlashLight; // jmarshall

	// berserk
	int							  berserk;

	// these are the player render view parms, which include bobbing
	idVec3						  playerViewOrigin;
	idMat3						  playerViewAxis;

	// the view weapon render entity parms
	idVec3						  viewWeaponOrigin;
	idMat3						  viewWeaponAxis;

	// the muzzle bone's position, used for launching projectiles and trailing smoke
	bool						  hasMuzzle; // Leyland VR
	idVec3						  muzzleOrigin;
	idMat3						  muzzleAxis;

	idVec3						  pushVelocity;

	// weapon definition
	// we maintain local copies of the projectile and brass dictionaries so they
	// do not have to be copied across the DLL boundary when entities are spawned
	const idDeclEntityDef*		  weaponDef;
	const idDeclEntityDef*		  meleeDef;
	idDict						  projectileDict;
	float						  meleeDistance;
	idStr						  meleeDefName;
	idDict						  brassDict;
	int							  brassDelay;
	idStr						  icon;
	idStr						  pdaIcon;
	idStr						  displayName;
	idStr						  itemDesc;

	// view weapon gui light
	renderLight_t				  guiLight;
	int							  guiLightHandle;

	// muzzle flash
	renderLight_t				  muzzleFlash; // positioned on view weapon bone
	int							  muzzleFlashHandle;

	renderLight_t				  worldMuzzleFlash; // positioned on world weapon bone
	int							  worldMuzzleFlashHandle;

	float						  fraccos;
	float						  fraccos2;

	idVec3						  flashColor;
	int							  muzzleFlashEnd;
	int							  flashTime;
	bool						  lightOn;
	bool						  silent_fire;
	bool						  allowDrop;

	// effects
	bool						  hasBloodSplat;

	// weapon kick
	int							  kick_endtime;
	int							  muzzle_kick_time;
	int							  muzzle_kick_maxtime;
	idAngles					  muzzle_kick_angles;
	idVec3						  muzzle_kick_offset;

	// ammo management
	ammo_t						  ammoType;
	int							  ammoRequired; // amount of ammo to use each shot.  0 means weapon doesn't need ammo.
	int							  clipSize;		// 0 means no reload
	idPredictedValue<int>		  ammoClip;
	int							  lowAmmo;	 // if ammo in clip hits this threshold, snd_
	bool						  powerAmmo; // true if the clip reduction is a factor of the power setting when
	// a projectile is launched
	// mp client
	bool						  isFiring;

	// zoom
	int							  zoomFov; // variable zoom fov per weapon

	// joints from models
	jointHandle_t				  barrelJointView;
	jointHandle_t				  flashJointView;
	jointHandle_t				  ejectJointView;
	jointHandle_t				  guiLightJointView;
	jointHandle_t				  ventLightJointView;

	jointHandle_t				  flashJointWorld;
	jointHandle_t				  barrelJointWorld;
	jointHandle_t				  ejectJointWorld;

	jointHandle_t				  smokeJointView;

	idHashTable<WeaponParticle_t> weaponParticles;
	idHashTable<WeaponLight_t>	  weaponLights;

	idDeclSkinWrapper			  vrWrapperSkin; // Leyland VR

	// sound
	const idSoundShader*		  sndHum;

	// new style muzzle smokes
	const idDeclParticle*		  weaponSmoke;			// null if it doesn't smoke
	int							  weaponSmokeStartTime; // set to gameLocal.time every weapon fire
	bool						  continuousSmoke;		// if smoke is continuous ( chainsaw )
	const idDeclParticle*		  strikeSmoke;			// striking something in melee
	int							  strikeSmokeStartTime; // timing
	idVec3						  strikePos;			// position of last melee strike
	idMat3						  strikeAxis;			// axis of last melee strike
	int							  nextStrikeFx;			// used for sound and decal ( may use for strike smoke too )

	// nozzle effects
	bool						  nozzleFx; // does this use nozzle effects ( parm5 at rest, parm6 firing )
	// this also assumes a nozzle light atm
	int							  nozzleFxFade;		// time it takes to fade between the effects
	int							  lastAttack;		// last time an attack occured
	renderLight_t				  nozzleGlow;		// nozzle light
	int							  nozzleGlowHandle; // handle for nozzle light

	idVec3						  nozzleGlowColor;	// color of the nozzle glow
	const idMaterial*			  nozzleGlowShader; // shader for glow light
	float						  nozzleGlowRadius; // radius of glow light

	// weighting for viewmodel angles
	int							  weaponAngleOffsetAverages;
	float						  weaponAngleOffsetScale;
	float						  weaponAngleOffsetMax;
	float						  weaponOffsetTime;
	float						  weaponOffsetScale;

	//! Alerts monsters and triggers when the weapon's flashlight hits an entity.
	void						  AlertMonsters();

	//! Initializes the world model for the weapon using the provided entity definition
	void						  InitWorldModel( const idDeclEntityDef* def );

	//! Applies muzzle rise effect to weapon origin and axis based on firing time.
	void						  MuzzleRise( idVec3& origin, idMat3& axis );

	//! Updates the nozzle effects for the weapon.
	void						  UpdateNozzleFx();

	//! Updates the position and orientation of the muzzle flash effect based on the weapon's joint and player view.
	void						  UpdateFlashPosition();

public:
	//! Calls a native event handler based on the provided event name.
	virtual void CallNativeEvent( idStr& name ) override;

	//! Sets a shader parameter for muzzle flash effects.
	void		 Event_SetLightParm( int parmnum, float value );

	/*!
		\brief Sets the light parameters for both muzzle flash effects

		Configures the red, green, blue, and alpha shader parameters for both the local muzzle flash and the world muzzle flash effects. This function updates the visual appearance of the weapon's
	   muzzle flash by setting the color and transparency values. After updating the parameters, it calls UpdateVisuals to refresh the display.

		\param parm0 Red color component value
		\param parm1 Green color component value
		\param parm2 Blue color component value
		\param parm3 Alpha transparency value
	*/
	void		 Event_SetLightParms( float parm0, float parm1, float parm2, float parm3 );

	//! Clears the weapon's state and resets its internal data.
	void		 Event_Clear();

	//! Returns the owner entity of the weapon.
	void		 Event_GetOwner();
	void		 Event_SetWeaponStatus( float newStatus );

	//! Sets the weapon to ready state with idle animation and optional hum sound.
	void		 Event_WeaponReady();

	//! Sets the weapon state to out of ammo.
	void		 Event_WeaponOutOfAmmo();

	//! Handles the weapon reloading event by calling the Reload function and printing a debug message.
	void		 Event_WeaponReloading();

	//! Sets the weapon status to holstered and prints a debug message if weapon debugging is enabled.
	void		 Event_WeaponHolstered();

	//! Handles the weapon rising event by raising the weapon and notifying the owner.
	void		 Event_WeaponRising();

	//! Handles the weapon lowering animation event.
	void		 Event_WeaponLowering();

	//! Uses the specified amount of ammo from the weapon's owner inventory and updates the weapon's clip.
	void		 Event_UseAmmo( int amount );

	//! Adds the specified amount of ammo to the weapon's clip.
	void		 Event_AddToClip( int amount );

	//! Returns the amount of ammo currently in the weapon's clip.
	void		 Event_AmmoInClip();

	//! Returns the total available ammunition for the weapon, including both inventory and clip ammo.
	int			 AmmoAvailable();

	//! Returns the amount of ammunition available for the weapon.
	void		 Event_AmmoAvailable();

	//! Returns the total amount of ammo available for the weapon's ammo type.
	void		 Event_TotalAmmoCount();

	//! Returns the clip size of the weapon.
	void		 Event_ClipSize();

	//! Plays a specified animation on the weapon entity.
	void		 Event_PlayAnim( int channel, const char* animname, bool loop );

	//! Plays a cycle animation on the weapon entity.
	void		 Event_PlayCycle( int channel, const char* animname );

	//! Checks if an animation is done based on the channel and blend frames.
	bool		 Event_AnimDone( int channel, int blendFrames );

	//! Sets the blend frames for the specified animation channel.
	void		 Event_SetBlendFrames( int channel, int blendFrames );

	//! Returns the animation blend frames value for the specified channel.
	void		 Event_GetBlendFrames( int channel );

	//! Changes to another weapon if possible.
	void		 Event_Next();

	//! Sets the skin of the weapon to the specified skin name.
	void		 Event_SetSkin( const char* skinname );

	//! Toggles the weapon's flashlight on or off based on the enable parameter.
	void		 Event_Flashlight( int enable );

	//! Returns the value of a specific shader parameter from the muzzle flash effect.
	void		 Event_GetLightParm( int parmnum );

	/*!
		\brief Launches a specified number of projectiles from the weapon with given spread and power parameters.

		This function handles the firing mechanics of a weapon, including ammo management, projectile creation, and prediction for multiplayer clients. It calculates the direction and spread for each
	   projectile based on the weapon's settings and the provided parameters. The function also manages the weapon's visual effects like muzzle flashes and kickback, and ensures proper networking for
	   client-server synchronization in multiplayer environments. It handles cases where the weapon is hidden, has no defined projectile, or when ammo is insufficient. The function supports both
	   regular and power-based ammo systems, and accounts for different firing modes such as hitscan weapons. When running on a client that's not the server, it only spawns local projectiles for
	   visual feedback unless the weapon is set to instant hit.

		\param num_projectiles The number of projectiles to launch
		\param spread The angular spread of the projectiles in degrees
		\param fuseOffset Time offset for projectile fuse detonation
		\param launchPower The initial velocity or power of the projectile launch
		\param dmgPower The damage power or energy level of the projectile
	*/
	void		 Event_LaunchProjectiles( int num_projectiles, float spread, float fuseOffset, float launchPower, float dmgPower );

	//! Creates and returns a projectile entity for the weapon
	idEntity*	 CreateProjectile();

	//! Creates and returns a new projectile entity.
	void		 Event_CreateProjectile();

	//! Ejects a brass shell from the weapon's ejection port if all conditions and resources are properly configured.
	void		 Event_EjectBrass();

	//! Performs a melee attack action, calculating hit detection and applying damage or effects to entities in range.
	void		 Event_Melee();

	//! Returns the world model entity associated with the weapon.
	void		 Event_GetWorldModel();

	//! Sets whether the weapon can be dropped by the player.
	void		 Event_AllowDrop( int allow );

	//! Returns the auto-reload setting for the weapon owned by the entity.
	void		 Event_AutoReload();

	//! Sends a reload event from the server to the client.
	void		 Event_NetReload();

	//! Returns true if the weapon's owner has the invisibility power-up active.
	bool		 Event_IsInvisible();

	//! Handles the network end of weapon reload event on the client side.
	void		 Event_NetEndReload();

	//! Prepares the weapon for cinematic mode by stopping sounds, disabling the weapon, and lowering it.
	void		 EnterCinematic();

	//! Exits the cinematic state and raises the weapon.
	void		 ExitCinematic();

	//! Performs network catchup operations for the weapon.
	void		 NetCatchup();

	//! Returns true if the weapon is currently linked to a weapon object.
	bool		 IsLinked() { return currentWeaponObject != NULL; }

private:
	idGrabber grabber;
	int		  grabberState;

public:
	//! Sets the grabber state based on the enable flag.
	void Event_Grabber( int enable );

	//! Returns the current grabber state of the weapon.
	int	 Event_GrabberHasTarget();

	//! Sets the grabber's drag distance to the specified value.
	void Event_GrabberSetGrabDistance( float dist );

	/*!
		\brief Launches a specified number of projectiles in an elliptical pattern with given spread parameters

		This function handles the launching of multiple projectiles in an elliptical spread pattern. It takes into account various weapon parameters including clip size, ammo consumption, and firing
	   effects. The function calculates projectile directions based on elliptical spread parameters and launches them from the weapon's muzzle position. It also handles client-side vs server-side
	   logic, manages ammo usage, and applies appropriate visual effects such as muzzle flashes and lighting. The function ensures projectiles start inside the owner's bounding box and handles proper
	   collision detection for the initial projectile positioning.

		\param num_projectiles Number of projectiles to launch
		\param spreada First spread parameter defining the elliptical spread along one axis
		\param spreadb Second spread parameter defining the elliptical spread along the perpendicular axis
		\param fuseOffset Offset for the projectile fuse timing
		\param power Power multiplier for the projectile launch
	*/
	void Event_LaunchProjectilesEllipse( int num_projectiles, float spreada, float spreadb, float fuseOffset, float power );

	//! Gives the player a powerup as if it were a weapon shot, using specified ammo if required.
	void Event_LaunchPowerup( const char* powerup, float duration, int useAmmo );

	//! Resets the muzzle smoke start time to the current game time.
	void Event_StartWeaponSmoke();

	//! Stops the weapon smoke effect by resetting the smoke start time.
	void Event_StopWeaponSmoke();

	//! Activates a weapon particle effect specified by the given name.
	void Event_StartWeaponParticle( const char* name );

	//! Stops a specified weapon particle effect by deactivating its emitter.
	void Event_StopWeaponParticle( const char* name );

	//! Activates a weapon light with the specified name.
	void Event_StartWeaponLight( const char* name );

	//! Stops a weapon light with the specified name
	void Event_StopWeaponLight( const char* name );

private:
	// jmarshall
	iceWeaponObject* currentWeaponObject;
	bool			 OutOfAmmo;
};

ID_INLINE bool idWeapon::IsWorldModelReady()
{
	return ( worldModel.GetEntity() != NULL );
}

ID_INLINE idPlayer* idWeapon::GetOwner()
{
	return owner;
}

#endif /* !__GAME_WEAPON_H__ */
