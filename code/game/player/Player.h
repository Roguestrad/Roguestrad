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

#ifndef __GAME_PLAYER_H__
#define __GAME_PLAYER_H__

#include "../PredictedValue.h"

/*
===============================================================================

	Player entity.

===============================================================================
*/

class idMenuHandler_PDA;
class idMenuHandler_HUD;
class idMenuScreen_HUD;
class idTarget_SetPrimaryObjective;

extern const idEventDef EV_Player_GetButtons;
extern const idEventDef EV_Player_GetMove;
extern const idEventDef EV_Player_GetViewAngles;
extern const idEventDef EV_Player_EnableWeapon;
extern const idEventDef EV_Player_DisableWeapon;
extern const idEventDef EV_Player_ExitTeleporter;
extern const idEventDef EV_Player_SelectWeapon;
extern const idEventDef EV_SpectatorTouch;

const float				THIRD_PERSON_FOCUS_DISTANCE = 512.0f;
const int				LAND_DEFLECT_TIME			= 150;
const int				LAND_RETURN_TIME			= 300;
const int				FOCUS_TIME					= 300;
const int				FOCUS_GUI_TIME				= 500;
const int				NUM_QUICK_SLOTS				= 4;

const int				DEAD_HEARTRATE			= 0;   // fall to as you die
const int				LOWHEALTH_HEARTRATE_ADJ = 20;  //
const int				DYING_HEARTRATE			= 30;  // used for volumen calc when dying/dead
const int				BASE_HEARTRATE			= 70;  // default
const int				ZEROSTAMINA_HEARTRATE	= 115; // no stamina
const int				MAX_HEARTRATE			= 130; // maximum
const int				ZERO_VOLUME				= -40; // volume at zero
const int				DMG_VOLUME				= 5;   // volume when taking damage
const int				DEATH_VOLUME			= 15;  // volume at death

const int				SAVING_THROW_TIME = 5000; // maximum one "saving throw" every five seconds

const int				ASYNC_PLAYER_INV_AMMO_BITS = idMath::BitsForInteger( 3000 );
const int				ASYNC_PLAYER_INV_CLIP_BITS = -7; // -7 bits to cover the range [-1, 60]

enum gameExpansionType_t { GAME_BASE, GAME_D3XP, GAME_D3LE, GAME_UNKNOWN };

// influence levels
enum {
	INFLUENCE_NONE = 0, // none
	INFLUENCE_LEVEL1,	// no gun or hud
	INFLUENCE_LEVEL2,	// no gun, hud, movement
	INFLUENCE_LEVEL3,	// slow player movement
};

typedef struct {
	char							   name[64];
	idList<int, TAG_IDLIB_LIST_PLAYER> toggleList;
	int								   lastUsed;
} WeaponToggle_t;

typedef struct {
	int	   time;
	idVec3 dir; // scaled larger for running
} loggedAccel_t;

typedef struct {
	int	   areaNum;
	idVec3 pos;
} aasLocation_t;

// Leyland VR
enum slotIndex_t { SLOT_NONE = -1, SLOT_LEFT_HIP, SLOT_RIGHT_HIP, SLOT_RIGHT_BACK_BOTTOM, SLOT_RIGHT_BACK_TOP, SLOT_COUNT };

struct slot_t {
	idVec3 origin;
	float  radiusSq;
};

/*!
	\class idPlayer
	\brief The idPlayer class manages player-specific game entities including state, input handling, inventory, weapons, and multiplayer functionality.

	This class represents the player character in the game, extending actor functionality to include player-specific behaviors such as input processing, weapon management, inventory handling, and
   multiplayer state coordination. The class maintains player state information including health, armor, power-ups, inventory items, and weapon selections. It handles player movement, physics
   interactions, and camera rendering for both first-person and third-person views. The class supports various game modes including spectator and cinematic states, and provides methods for saving and
   restoring player progress. Player actions such as shooting, reloading, weapon switching, and using items are managed through this class. It also handles multiplayer-specific features like
   networking, scoring, and team-based gameplay elements. The class implements game-specific mechanics such as power-ups, damage calculation, and visual effects including HUD rendering, particle
   systems, and audio logging.

*/
class idPlayer : public idActor
{
public:
	enum {
		EVENT_IMPULSE = idEntity::EVENT_MAXEVENTS,
		EVENT_EXIT_TELEPORTER,
		EVENT_ABORT_TELEPORTER,
		EVENT_POWERUP,
		EVENT_SPECTATE,
		EVENT_PICKUPNAME,
		EVENT_FORCE_ORIGIN,
		EVENT_KNOCKBACK,
		EVENT_MAXEVENTS
	};

	static const int			  MAX_PLAYER_PDA		   = 100;
	static const int			  MAX_PLAYER_VIDEO		   = 100;
	static const int			  MAX_PLAYER_AUDIO		   = 100;
	static const int			  MAX_PLAYER_AUDIO_ENTRIES = 2;

	usercmd_t					  oldCmd;
	usercmd_t					  usercmd;

	class idPlayerView			  playerView; // handles damage kicks and effects

	renderEntity_t				  laserSightRenderEntity; // replace crosshair for 3DTV
	qhandle_t					  laserSightHandle;

	// Leyland VR
	renderEntity_t				  pdaRenderEntity;	 // used to present a model to the renderer
	qhandle_t					  pdaModelDefHandle; // handle to static renderer model

	renderEntity_t				  holsterRenderEntity;	 // used to present a model to the renderer
	qhandle_t					  holsterModelDefHandle; // handle to static renderer model
	idMat3						  holsterAxis;
	int							  holsteredWeapon;
	// Leyland end

	bool						  noclip;
	bool						  godmode;

	bool						  spawnAnglesSet; // on first usercmd, we must set deltaAngles
	idAngles					  spawnAngles;
	idAngles					  viewAngles;				   // player view angles
	idAngles					  cmdAngles;				   // player cmd angles
	float						  independentWeaponPitchAngle; // viewAngles[PITCH} when head tracking is active

	// For interpolating angles between snapshots
	idQuat						  previousViewQuat;
	idQuat						  nextViewQuat;

	int							  buttonMask;
	int							  oldButtons;
	int							  oldImpulseSequence;

	int							  lastHitTime;		   // last time projectile fired by player hit target
	int							  lastSndHitTime;	   // MP hit sound - != lastHitTime because we throttle
	int							  lastSavingThrowTime; // for the "free miss" effect

	bool						  pdaHasBeenRead[MAX_PLAYER_PDA];
	bool						  videoHasBeenViewed[MAX_PLAYER_VIDEO];
	bool						  audioHasBeenHeard[MAX_PLAYER_AUDIO][MAX_PLAYER_AUDIO_ENTRIES];

	idScriptBool				  AI_FORWARD;
	idScriptBool				  AI_BACKWARD;
	idScriptBool				  AI_STRAFE_LEFT;
	idScriptBool				  AI_STRAFE_RIGHT;
	idScriptBool				  AI_ATTACK_HELD;
	idScriptBool				  AI_WEAPON_FIRED;
	idScriptBool				  AI_JUMP;
	idScriptBool				  AI_CROUCH;
	idScriptBool				  AI_ONGROUND;
	idScriptBool				  AI_ONLADDER;
	idScriptBool				  AI_DEAD;
	idScriptBool				  AI_RUN;
	idScriptBool				  AI_PAIN;
	idScriptBool				  AI_HARDLANDING;
	idScriptBool				  AI_SOFTLANDING;
	idScriptBool				  AI_RELOAD;
	idScriptBool				  AI_TELEPORT;
	idScriptBool				  AI_TURN_LEFT;
	idScriptBool				  AI_TURN_RIGHT;

	// inventory
	idInventory					  inventory;
	idTarget_SetPrimaryObjective* primaryObjective;

	// RB: hack to reinit flash light when switching between classic mode
	bool						  flashlightReset;
	int							  flashlightBattery;
	idEntityPtr<idWeapon>		  flashlight;

	idEntityPtr<idWeapon>		  weapon;
	idMenuHandler_HUD*			  hudManager;
	idMenuScreen_HUD*			  hud;
	idMenuHandler_PDA*			  pdaMenu;
	idSWF*						  mpMessages;
	bool						  objectiveSystemOpen;
	int							  quickSlot[NUM_QUICK_SLOTS];

	int							  weapon_soulcube;
	int							  weapon_pda;
	int							  weapon_fists;
	int							  weapon_flashlight;
	int							  weapon_chainsaw;
	int							  weapon_bloodstone;
	int							  weapon_bloodstone_active1;
	int							  weapon_bloodstone_active2;
	int							  weapon_bloodstone_active3;
	bool						  harvest_lock;

	int							  heartRate;
	idInterpolate<float>		  heartInfo;
	int							  lastHeartAdjust;
	int							  lastHeartBeat;
	int							  lastDmgTime;
	int							  deathClearContentsTime;
	bool						  doingDeathSkin;
	int							  lastArmorPulse; // lastDmgTime if we had armor at time of hit
	float						  stamina;
	float						  healthPool; // amount of health to give over time
	int							  nextHealthPulse;
	bool						  healthPulse;
	bool						  healthTake;
	int							  nextHealthTake;

	//-----------------------------------------------------------------
	// controller shake parms
	//-----------------------------------------------------------------

	const static int			  MAX_SHAKE_BUFFER = 3;
	float						  controllerShakeHighMag[MAX_SHAKE_BUFFER];	 // magnitude of the high frequency controller shake
	float						  controllerShakeLowMag[MAX_SHAKE_BUFFER];	 // magnitude of the low frequency controller shake
	int							  controllerShakeHighTime[MAX_SHAKE_BUFFER]; // time the controller shake ends for high frequency.
	int							  controllerShakeLowTime[MAX_SHAKE_BUFFER];	 // time the controller shake ends for low frequency.
	int							  controllerShakeTimeGroup;

	bool						  hiddenWeapon; // if the weapon is hidden ( in noWeapons maps )
	idEntityPtr<idProjectile>	  soulCubeProjectile;

	idAimAssist					  aimAssist;

	int							  spectator;
	bool						  forceScoreBoard;
	bool						  forceRespawn;
	bool						  spectating;
	int							  lastSpectateTeleport;
	bool						  lastHitToggle;
	bool						  wantSpectate;		// from userInfo
	bool						  weaponGone;		// force stop firing
	bool						  useInitialSpawns; // toggled by a map restart to be active for the first game spawn
	int							  tourneyRank;		// for tourney cycling - the higher, the more likely to play next - server
	int							  tourneyLine;		// client side - our spot in the wait line. 0 means no info.
	int							  spawnedTime;		// when client first enters the game
													// jmarshall
	idStr						  netname;
	// jmarshall end
	bool						  carryingFlag; // is the player carrying the flag?

	idEntityPtr<idEntity>		  teleportEntity;	// while being teleported, this is set to the entity we'll use for exit
	int							  teleportKiller;	// entity number of an entity killing us at teleporter exit
	bool						  lastManOver;		// can't respawn in last man anymore (srv only)
	bool						  lastManPlayAgain; // play again when end game delay is cancelled out before expiring (srv only)
	bool						  lastManPresent;	// true when player was in when game started (spectators can't join a running LMS)
	bool						  isLagged;			// replicated from server, true if packets haven't been received from client.
	int							  isChatting;		// replicated from server, true if the player is chatting.

	// timers
	int							  minRespawnTime; // can respawn when time > this, force after g_forcerespawn
	int							  maxRespawnTime; // force respawn after this time

	// the first person view values are always calculated, even
	// if a third person view is used
	idVec3						  firstPersonViewOrigin;
	idMat3						  firstPersonViewAxis;

	// Leyland VR
	idVec3						  flashlightOrigin;
	idMat3						  flashlightAxis;

	idVec3						  hmdOrigin;
	idMat3						  hmdAxis;

	idVec3						  leftHandOrigin;
	idMat3						  leftHandAxis;
	slotIndex_t					  leftHandSlot;

	idVec3						  rightHandOrigin;
	idMat3						  rightHandAxis;
	slotIndex_t					  rightHandSlot;

	idVec3						  waistOrigin;
	idMat3						  waistAxis;
	// Leyland end

	idDragEntity				  dragEntity;

	idFuncMountedObject*		  mountedObject;
	idEntityPtr<idLight>		  enviroSuitLight;

	bool						  healthRecharge;
	int							  lastHealthRechargeTime;
	int							  rechargeSpeed;

	float						  new_g_damageScale;

	bool						  bloomEnabled;
	float						  bloomSpeed;
	float						  bloomIntensity;

public:
	CLASS_PROTOTYPE( idPlayer );

	//! Initializes a new instance of the idPlayer class with default values for all member variables.
	idPlayer();

	//! Destructor for the idPlayer class that releases all resources used by the player.
	virtual ~idPlayer();

	//! Initializes player resources and sets up player state for spawning.
	void		 Spawn();

	//! Updates player state and handles input for a single game frame.
	void		 Think();

	//! Initializes the PDA slot render entity for the player.
	void		 SetupPDASlot();

	//! Frees the PDA slot model definition handle if it is valid.
	void		 FreePDASlot();

	//! Updates the PDA slot rendering for the player
	void		 UpdatePDASlot();

	//! Sets up the holster slot for the player's weapon
	void		 SetupHolsterSlot();

	//! Frees the holster slot model definition if it is currently allocated.
	void		 FreeHolsterSlot();

	//! Updates the holster slot state and rendering for the player.
	void		 UpdateHolsterSlot();

	//! Updates the laser sight visualization for the player's weapon.
	void		 UpdateLaserSight();

	//! Saves the player state to a save game file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the player state from a save game file
	void		 Restore( idRestoreGame* savefile );

	//! Hides the player entity and its associated weapon and flashlight models.
	virtual void Hide();

	//! Displays the player and their attached weapon and flashlight models.
	virtual void Show();

	//! Returns true if the player is currently holding down the attack button.
	bool		 IsShooting();

	//! Returns the current view height of the player based on their state and configuration.
	float		 GetViewHeight();

	//! Initializes the player state and variables.
	void		 Init();

	//! Prepares the player for a game restart by clearing power-ups, setting spectator mode, and resetting HUD states.
	void		 PrepareForRestart();

	//! Resets the player state and reinitializes the player object.
	virtual void Restart();

	//! Links script variables for player AI conditions.
	void		 LinkScriptVariables();

	//! Initializes or reinitializes the player's weapon entity.
	void		 SetupWeaponEntity();

	//! Selects an initial spawn point for the player, using a designated 'initial' spawn point if available.
	void		 SelectInitialSpawnPoint( idVec3& origin, idAngles& angles );

	//! Selects an initial spawn point and spawns the player at that location.
	void		 SpawnFromSpawnSpot();

	//! Spawns the player at the specified origin and angles, initializing player state and handling spectating behavior.
	virtual void SpawnToPoint( const idVec3& spawn_origin, const idAngles& spawn_angles );

	//! Sets the clip model for the player based on whether they are spectating or not.
	void		 SetClipModel();

	//! Saves inventory and player stats for persistent information when changing levels
	void		 SavePersistantInfo();

	//! Restores player inventory and stats from persistent data when changing levels
	void		 RestorePersistantInfo();

	//! Sets a level trigger for the player using the specified level and trigger names.
	void		 SetLevelTrigger( const char* levelName, const char* triggerName );

	//! Caches weapon assets for all acquired weapons in the player's inventory.
	void		 CacheWeapons();

	//! Prepares the player for cinematic sequence by stopping sounds, hiding, and disabling controls.
	void		 EnterCinematic();

	//! Exits cinematic mode and resets player state and weapon flashlight.
	void		 ExitCinematic();

	//! Updates the player's movement conditions based on velocity and user input
	void		 UpdateConditions();

	//! Sets the view angles for the player.
	void		 SetViewAngles( const idAngles& angles );

	//! Returns false indicating this player is not a bot.
	virtual bool IsBot() { return false; }

	//! Applies controller shake effect to the player based on damage received.
	void		 ControllerShakeFromDamage( int damage );

	//! Applies controller shake to the player based on damage received.
	void		 ControllerShakeFromDamage( int damage, const idVec3& dir );

	//! Sets the controller shake effect for the player with specified magnitude, duration, and direction.
	void		 SetControllerShake( float magnitude, int duration, const idVec3& direction );

	/*!
		\brief Sets controller shake parameters for high and low magnitude rumble effects with specified durations

		This function initializes or updates controller shake effects for VR gameplay. It manages a buffer system to handle multiple simultaneous shake events. The function first checks if both
	   duration parameters are zero, and if so, resets the shake time group. Otherwise, it attempts to find an existing buffer with similar magnitude values to average with, allowing multiple effects
	   to coexist. If no similar buffer exists, it either uses an inactive buffer or replaces the oldest one. The function tracks shake effects per time group and updates the timing for both high and
	   low magnitude shakes.

		\param highMagnitude The intensity of the high magnitude rumble effect
		\param highDuration The duration of the high magnitude rumble effect in milliseconds
		\param lowMagnitude The intensity of the low magnitude rumble effect
		\param lowDuration The duration of the low magnitude rumble effect in milliseconds
	*/
	void		 SetControllerShake( float highMagnitude, int highDuration, float lowMagnitude, int lowDuration );

	//! Resets all controller shake parameters to their default values.
	void		 ResetControllerShake();

	//! Retrieves the current controller shake magnitude values for high and low frequency vibrations.
	void		 GetControllerShake( int& highMagnitude, int& lowMagnitude ) const;

	//! Returns a pointer to the aim assist component associated with the player.
	idAimAssist* GetAimAssist() { return &aimAssist; }

	//! Updates the delta view angles based on the provided angles to allow movers to rotate the player's view.
	void		 UpdateDeltaViewAngles( const idAngles& angles );

	//! Handles collision events for the player entity.
	virtual bool Collide( const trace_t& collision, const idVec3& velocity );

	//! Retrieves the AAS location data for the specified AAS instance.
	virtual void GetAASLocation( idAAS* aas, idVec3& pos, int& areaNum ) const;

	//! Calculates and returns the head and chest aim positions for AI targeting based on the last sight position.
	virtual void GetAIAimTargets( const idVec3& lastSightPos, idVec3& headPos, idVec3& chestPos );

	//! Applies damage modification and handles feedback logic when the player entity inflicts damage on another entity.
	virtual void DamageFeedback( idEntity* victim, idEntity* inflictor, int& damage );

	/*!
		\brief Calculates the damage to health and armor based on the attack parameters.

		This function computes the actual damage points that will be applied to a player's health and armor. It takes into account the damage definition, scaling factors, skill settings, and various
	   game rules such as team damage, god mode, and invulnerability. The function also handles special cases like self-damage and armor protection. It does not actually apply the damage, but rather
	   calculates the values that would be used in damage application.

		\param inflictor Entity that caused the damage
		\param attacker Entity that inflicted the damage
		\param damageDef Dictionary containing damage definition parameters
		\param damageScale Scale factor to multiply the base damage
		\param location Damage location identifier used for damage calculation
		\param health Pointer to store the calculated health damage amount
		\param armor Pointer to store the calculated armor damage amount
	*/
	void		 CalcDamagePoints( idEntity* inflictor, idEntity* attacker, const idDict* damageDef, const float damageScale, const int location, int* health, int* armor );

	/*!
		\brief Applies damage to the player from a given inflictor and attacker, with optional direction and damage scaling.

		This function handles the application of damage to the player entity. It takes into account various game states such as noclip, spectating, and whether the player is taking damage. The
	   function calculates the damage based on the provided damage definition, scales it, and applies knockback if specified. It also handles multiplayer scenarios where damage is sent to the server
	   for processing. The function updates the player's health and inventory armor, and triggers damage-related sounds and effects. This function also sets up predictive damage feedback for local
	   players in multiplayer mode.

		\param inflictor The entity that caused the damage, can be NULL for environmental effects
		\param attacker The entity that is attacking the player, can be NULL for environmental effects
		\param dir The direction from which the damage is coming, can be NULL for environmental effects
		\param damageDefName Name of the damage definition to use for calculating damage
		\param damageScale A scaling factor for the damage amount
		\param location The location on the player that was hit
	*/
	virtual void Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );

	/*!
		\brief Applies damage to the player from an inflictor with directional information and damage definition

		This function handles the server-side logic for applying damage to a player entity. It validates the damage definition, calculates local damage direction, and updates player health. The
	   function also manages damage impulses for client feedback, handles death conditions, and tracks achievement events for specific game scenarios. It ensures proper damage application and handles
	   special cases like the frag chamber mechanic in certain maps.

		\param damage Amount of damage to apply
		\param inflictor Entity causing the damage
		\param attacker Entity responsible for the attack
		\param dir Direction vector of the damage
		\param damageDefName Name of the damage definition to use
		\param location Location index where damage was applied
		\throws assertion failure if called on client
	*/
	void		 ServerDealDamage(
				int damage, idEntity& inflictor, idEntity& attacker, const idVec3& dir, const char* damageDefName, const int location ); // Actually updates the player's health independent of feedback.

	//! Adjusts the input damage amount based on game settings and conditions.
	int							  AdjustDamageAmount( const int inputDamage );

	//! Teleports the player to a specified location with optional destination entity.
	virtual void				  Teleport( const idVec3& origin, const idAngles& angles, idEntity* destination );

	//! Kills the player, with options for delayed respawn and no damage effects.
	void						  Kill( bool delayRespawn, bool nodamage );

	/*!
		\brief Handles the player death event, including animation, sound, and game state updates.

		This function is called when a player is killed, and performs a variety of actions to handle the death. It stops knockback, sets the player to a dead state, initializes heart rate, and
	   triggers death animations. If the player enters a ragdoll state, it sets respawn timers; otherwise, it calculates a delay before respawn. The function also handles sound effects, weapon
	   dropping, and updates multiplayer game state. In multiplayer, it manages gibbing conditions and notifies the game about the death event.

		\param inflictor The entity that caused the death
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage that caused the death
		\param dir The direction from which the damage came
		\param location The location on the player that was hit
		\throws assertion failure if running on client
	*/
	virtual void				  Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Starts a particle effect at a specified bone location on the player.
	void						  StartFxOnBone( const char* fx, const char* bone );

	//! Returns the render view calculated for this tic
	renderView_t*				  GetRenderView();

	//! Calculates and sets up the rendering view for the player, handling camera perspectives and VR integration.
	void						  CalculateRenderView();

	//! Calculates the first person view origin and axis based on player state and VR configuration.
	void						  CalculateFirstPersonView();

	//! Returns whether the player should blink based on the physics object's blinking state.
	bool						  ShouldBlink();

	//! Calculates the waist origin and axis based on HMD data for VR rendering.
	void						  CalculateWaist();

	//! Calculates the left hand origin and axis based on VR controller input or fallback to HMD-based positioning.
	void						  CalculateLeftHand();

	//! Calculates the right hand origin and axis for VR user input.
	void						  CalculateRightHand();

	//! Calculates the VR view origin and axis for the player, with optional pitch override.
	bool						  CalculateVRView( idVec3& origin, idMat3& axis, bool overridePitch );

	//! Adds a chat message to the multiplayer chat interface at the specified index with the given alpha transparency.
	void						  AddChatMessage( int index, int alpha, const idStr& message );

	//! Updates the spectator text display for the player.
	void						  UpdateSpectatingText();

	//! Clears a chat message at the specified index in the multiplayer messages system
	void						  ClearChatMessage( int index );

	//! Draws the heads-up display for the player
	void						  DrawHUD( idMenuHandler_HUD* hudManager );

	//! Handles weapon fire feedback including blinking, animations, and controller shake effects
	void						  WeaponFireFeedback( const idDict* weaponDef );

	//! Returns the base field of view for the player.
	float						  DefaultFov() const;

	//! Calculates the field of view, taking into account zoom and other factors.
	float						  CalcFov( bool honorZoom );

	//! Calculates the position and orientation of the view weapon based on player state and animations.
	void						  CalculateViewWeaponPos( idVec3& origin, idMat3& axis );

	//! Returns the eye position of the player.
	idVec3						  GetEyePosition() const;

	//! Retrieves the player's view position and orientation, accounting for death state and VR settings.
	void						  GetViewPos( idVec3& origin, idMat3& axis ) const;

	/*!
		\brief Adjusts the third-person camera view based on specified angle, range, and height parameters, with optional collision clipping.

		This function modifies the player's third-person camera view by calculating a new viewpoint based on the provided angle, range, and height offsets. It adjusts the view angle to prevent
	   excessive overhead perspective and applies collision detection to ensure the view does not clip into solid objects. The function also recalculates the focus point and adjusts the pitch to
	   maintain proper orientation toward the player's focus target.

		\param angle The horizontal angle offset for the camera view, controlling the sideways shift of the camera
		\param range The distance from the player's origin to the camera position, determining how far back the camera is positioned
		\param height The vertical offset applied to the camera position, controlling the height of the camera above the player
		\param clip Whether to perform collision detection to prevent the camera from clipping into solid geometry
	*/
	void						  OffsetThirdPersonView( float angle, float range, float height, bool clip );

	//! Sets player statistics or inventory items based on the provided statname and value
	bool						  Give( const char* statname, const char* value, unsigned int giveFlags );

	//! Determines whether the player can pickup and receive the specified item
	bool						  GiveItem( idItem* item, unsigned int giveFlags );

	//! Gives the player an item by spawning an entity with the specified classname.
	void						  GiveItem( const char* name );

	//! Adds health to the player health pool
	void						  GiveHealthPool( float amt );

	//! Sets the primary objective for the player.
	void						  SetPrimaryObjective( idTarget_SetPrimaryObjective* target ) { primaryObjective = target; }

	//! Returns the primary objective associated with the player.
	idTarget_SetPrimaryObjective* GetPrimaryObjective() { return primaryObjective; }

	//! Returns a reference to the player's inventory.
	idInventory&				  GetInventory() { return inventory; }

	//! Adds an inventory item to the player's inventory based on the provided item data and flags.
	bool						  GiveInventoryItem( idDict* item, unsigned int giveFlags );

	//! Removes an inventory item from the player's inventory and updates the power cell count if necessary.
	void						  RemoveInventoryItem( idDict* item );

	//! Adds an inventory item to the player by spawning an entity with the specified classname.
	bool						  GiveInventoryItem( const char* name );

	//! Removes an inventory item from the player by its name
	void						  RemoveInventoryItem( const char* name );

	//! Returns the inventory item with the specified name, or NULL if not found.
	idDict*						  FindInventoryItem( const char* name );

	//! Returns the inventory item at the specified index if it exists, otherwise returns NULL.
	idDict*						  FindInventoryItem( int index );

	//! Returns the number of inventory items the player currently has.
	int							  GetNumInventoryItems();

	//! Plays an audio log using the specified sound shader if the player has a name.
	void						  PlayAudioLog( const idSoundShader* sound );

	//! Stops the audio log sound on the PDA channel.
	void						  EndAudioLog();

	//! Plays a video disk using the provided video declaration.
	void						  PlayVideoDisk( const idDeclVideo* decl );

	//! Ends the video disk playback and stops the associated sound.
	void						  EndVideoDisk();

	//! Returns the material used for displaying PDA video content.
	const idMaterial*			  GetVideoMaterial() { return pdaVideoMat; }

	//! Sets the value of a quick slot at the specified index.
	void						  SetQuickSlot( int index, int val );

	//! Returns the quick slot value at the specified index.
	int							  GetQuickSlot( int index );

	//! Gives the player a PDA and handles associated security items and UI updates.
	void						  GivePDA( const idDeclPDA* pda, const char* securityItem );

	//! Adds a video to the player's inventory and updates related achievements and HUD.
	void						  GiveVideo( const idDeclVideo* video, const char* itemName );

	//! Adds a new email to the player's inventory and PDA.
	void						  GiveEmail( const idDeclEmail* email );

	//! Sets the security level for the player's PDA and updates the HUD if available.
	void						  GiveSecurity( const char* security );

	//! Adds a new objective to the player's inventory and updates the HUD display.
	void						  GiveObjective( const char* title, const char* text, const idMaterial* screenshot );

	//! Completes an objective by removing it from the player's inventory and updating the HUD
	void						  CompleteObjective( const char* title );

	//! Gives the player a specified power-up with optional time and flags.
	bool						  GivePowerUp( int powerup, int time, unsigned int giveFlags );

	//! Clears all active power-ups and related resources for the player.
	void						  ClearPowerUps();

	//! Checks if a specified power-up is currently active for the player.
	bool						  PowerUpActive( int powerup ) const;

	//! Returns a modifier value for a specified power-up type while berserk is active
	float						  PowerUpModifier( int type );

	//! Checks if the left impulse slot action is triggered and performs corresponding player actions.
	bool						  LeftImpulseSlot();

	//! Returns true if the right impulse slot action was processed, false otherwise.
	bool						  RightImpulseSlot();

	//! Returns the slot index for a specified weapon name, or -1 if not found.
	int							  SlotForWeapon( const char* weaponName );

	//! Reloads the player's weapon if valid and not in a restricted state
	void						  Reload();

	//! Cycles to the next available weapon in the player's inventory.
	void						  NextWeapon();

	//! Sets the ideal weapon to the next best available weapon based on inventory and ammo status.
	void						  NextBestWeapon();

	//! Cycles to the previous available weapon in the player's inventory.
	void						  PrevWeapon();

	//! Sets the previous weapon index for the player.
	void						  SetPreviousWeapon( int num ) { previousWeapon = num; }

	//! Selects a weapon for the player either immediately or based on the force parameter.
	void						  SelectWeapon( int num, bool force );

	//! Drops the player's current weapon when they die.
	void						  DropWeapon( bool died );

	//! Steals the specified player's current weapon and ammo
	void						  StealWeapon( idPlayer* player );

	//! Increments the total count of projectiles fired by the player by the specified amount.
	void						  AddProjectilesFired( int count );

	//! Increments the projectile hits counter by the specified count.
	void						  AddProjectileHits( int count );

	//! Sets the last hit time for the player and updates related combat HUD effects and multiplayer aim states.
	void						  SetLastHitTime( int time );

	//! Lowers the player's current weapon if it is not hidden.
	TYPEINFO_IGNORE void		  LowerWeapon();

	//! Raises the player's weapon if it exists and is hidden.
	TYPEINFO_IGNORE void		  RaiseWeapon();

	//! Sets the player state to LowerWeapon and updates the script.
	void						  WeaponLoweringCallback();

	//! Sets the player state to RaiseWeapon and updates the script when the weapon is rising.
	void						  WeaponRisingCallback();

	//! Removes a weapon from the player's inventory by name.
	void						  RemoveWeapon( const char* weap );

	//! Removes all weapons from the player except for essential ones like fists, soul cube, PDA, and flashlights.
	void						  RemoveAllButEssentialWeapons();

	//! Returns true if the weapon viewmodel should be displayed.
	bool						  CanShowWeaponViewmodel() const;

	//! Increments the soul cube ammo count when an AI enemy is killed.
	void						  AddAIKill();

	//! Sets the soul cube projectile for the player.
	void						  SetSoulCubeProjectile( idProjectile* projectile );

	/*!
		\brief Adjusts the player's heart rate to a target value over a specified time with optional delay and force flag.

		This function modifies the player's heart rate by initializing a heart rate transition using the provided parameters. The heart rate will transition from the current value to the target value
	   over the specified time duration, with an optional delay before starting the transition. If the player is dead and the force flag is not set, the function will return without making changes.
	   The function also updates the last heart adjustment time to the current game time.

		\param target The target heart rate value to adjust to
		\param timeInSecs The duration in seconds over which the heart rate should transition to the target value
		\param delay The delay in seconds before the heart rate transition begins
		\param force If true, the heart rate adjustment will proceed even if the player is dead
	*/
	void						  AdjustHeartRate( int target, float timeInSecs, float delay, bool force );

	//! Updates the player's heart rate based on health and active power-ups.
	void						  SetCurrentHeartRate();

	//! Returns the base heart rate of the player.
	int							  GetBaseHeartRate();

	//! Updates the player's air status and manages oxygen depletion or regenration based on environmental conditions.
	void						  UpdateAir();

	//! Updates the powerup HUD state based on the current powerup and health.
	void						  UpdatePowerupHud();

	//! Handles a single GUI command from the provided lexer input.
	virtual bool				  HandleSingleGuiCommand( idEntity* entityGui, idLexer* src );

	//! Returns true if a GUI element is currently focused, false otherwise.
	bool						  GuiActive() { return focusGUIent != NULL; }

	//! Handles GUI events for the player by forwarding them to active HUD and PDA menu systems.
	bool						  HandleGuiEvents( const sysEvent_t* ev );

	//! Handles player impulse commands for weapon selection, flashlight control, PDA access, and other game actions.
	void						  PerformImpulse( int impulse );

	//! Sets the player to spectate mode or not.
	void						  Spectate( bool spectate, bool force = false );

	//! Toggles the PDA menu display for the player.
	void						  TogglePDA();

	//! Routes mouse input events to the specified GUI interface.
	void						  RouteGuiMouse( idUserInterface* gui );

	//! Updates the player's HUD display with item pickup information and multiplayer aim state
	void						  UpdateHud();

	//! Returns the player's PDA declaration.
	const idDeclPDA*			  GetPDA() const;

	//! Returns true if the player's PDA is currently open.
	bool						  GetPDAOpen() const { return objectiveSystemOpen; }

	//! Returns the video declaration at the specified index from the player's inventory.
	const idDeclVideo*			  GetVideo( int index );

	//! Sets the influence field of view angle for the player.
	void						  SetInfluenceFov( float fov );

	/*!
		\brief Sets the influence view parameters for the player, including material, skin, radius, and associated entity.

		This function configures the visual influence effect that the player experiences, typically used for special visual states or effects. It initializes the influence material and skin from the
	   provided names, updates the influence radius, and sets the associated entity. If a skin is specified, it also updates the head entity's shader parameters and calls UpdateVisuals to apply the
	   changes. The influence effect is only active when a positive radius is specified.

		\param mtr The name of the material to use for the influence effect, can be NULL or empty
		\param skinname The name of the skin to use for the influence effect, can be NULL or empty
		\param radius The radius of the influence effect, when greater than zero the effect is active
		\param ent The entity associated with the influence effect, can be NULL
	*/
	void						  SetInfluenceView( const char* mtr, const char* skinname, float radius, idEntity* ent );

	//! Sets the influence level of the player, affecting projectile behavior and weapon state.
	void						  SetInfluenceLevel( int level );
	int							  GetInfluenceLevel() { return influenceActive; };

	//! Sets the private camera view for the player, optionally hiding the player when the camera is active.
	void						  SetPrivateCameraView( idCamera* camView );

	//! Returns the private camera view associated with the player.
	idCamera*					  GetPrivateCameraView() const { return privateCameraView; }

	//! Enables a field of view effect and schedules its automatic termination after the specified duration.
	void						  StartFxFov( float duration );

	//! Updates the weapon state displayed on the HUD for the player.
	void						  UpdateHudWeapon( bool flashWeapon = true );

	//! Updates the chatting HUD display for the player.
	void						  UpdateChattingHud();

	//! Updates the HUD statistics display with player health, armor, stamina, weapon info, and flag status for flag-based game types.
	void						  UpdateHudStats( idMenuHandler_HUD* hudManager );

	//! Stops the audio log for the player.
	void						  Event_StopAudioLog();

	//! Checks if a sound is currently playing on the specified channel.
	bool						  IsSoundChannelPlaying( const s_channelType channel = SND_CHANNEL_ANY );

	//! Displays a tip message with the specified title and text on the HUD.
	void						  ShowTip( const char* title, const char* tip, bool autoHide );

	//! Hides the tip displayed on the HUD.
	void						  HideTip();
	bool						  IsTipVisible() { return tipUp; };

	//! Hides the objective indicator on the HUD
	void						  HideObjective();

	//! Updates the player's state and performs actions based on user input and game conditions.
	virtual void				  ClientThink( const int curTime, const float fraction, const bool predict );

	//! Writes player state information to a snapshot message for network synchronization
	virtual void				  WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads player state from a network snapshot message
	virtual void				  ReadFromSnapshot( const idBitMsg& msg );

	//! Writes the player state to a snapshot message.
	void						  WritePlayerStateToSnapshot( idBitMsg& msg ) const;

	//! Reads the player state from a snapshot message
	void						  ReadPlayerStateFromSnapshot( const idBitMsg& msg );

	//! Handles server-side events for the player entity.
	virtual bool				  ServerReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Returns the transformation from physics to visual space for the player
	virtual bool				  GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );

	//! Retrieves the sound transformation matrix and origin from the player's camera view.
	virtual bool				  GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );

	//! Handles client-side events received from the server for player-specific actions and state changes.
	virtual bool				  ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Returns true if the player is currently respawning.
	bool						  IsRespawning();

	//! Checks if the player is currently in a teleportation state.
	bool						  IsInTeleport();

	//! Returns the index of the skin currently used by the player.
	int							  GetSkinIndex() const { return skinIndex; }

	idEntity*					  GetInfluenceEntity() { return influenceEntity; };
	const idMaterial*			  GetInfluenceMaterial() { return influenceMaterial; };
	float						  GetInfluenceRadius() { return influenceRadius; };

	//! Sets the player to spectate mode or exits it
	void						  ServerSpectate( bool spectate );

	//! Returns a pointer to the player's physics object.
	idPhysics*					  GetPlayerPhysics();

	//! Sets the killer index for a teleportation-based death event.
	void						  TeleportDeath( int killer );

	//! Sets whether the player is currently the leader.
	void						  SetLeader( bool lead );

	//! Returns true if the player is the leader of the team.
	bool						  IsLeader();

	//! Updates the player's skin setup based on team or entity number in multiplayer mode.
	void						  UpdateSkinSetup();

	//! Returns true if the player is currently on a ladder.
	bool						  OnLadder() const;

	//! Updates the player icons based on network lag status.
	virtual void				  UpdatePlayerIcons();

	//! Draws the player icon for the current player if needed and not hidden
	virtual void				  DrawPlayerIcons();

	//! Hides the player icons by freeing the associated icon resource.
	virtual void				  HidePlayerIcons();

	//! Returns true if the player needs an icon displayed.
	bool						  NeedsIcon();

	//! Initializes the health recharge process for the player with the specified speed.
	void						  StartHealthRecharge( int speed );

	//! Stops the health recharge behavior for the player.
	void						  StopHealthRecharge();

	//! Returns the name of the currently equipped weapon.
	idStr						  GetCurrentWeapon();

	//! Returns the current weapon slot index.
	int							  GetCurrentWeaponSlot() { return currentWeapon; }

	//! Returns the ideal weapon index for the player.
	int							  GetIdealWeapon() { return idealWeapon.Get(); }

	//! Returns the weapon toggles associated with this player.
	idHashTable<WeaponToggle_t>	  GetWeaponToggles() const { return weaponToggles; }

	//! Checks if the player can give a specified stat value based on current state and inventory conditions.
	bool						  CanGive( const char* statname, const char* value );

	//! Stops the helltime powerup and resets game variables, with an option for quick reset.
	void						  StopHelltime( bool quick = true );

	//! Plays the helltime stop sound effect if available.
	void						  PlayHelltimeStopSound();

	//! Drops the CTF flag if the player is carrying it and meets the game conditions.
	void						  DropFlag();

	//! Returns the flag if the player is carrying it and the game is a flag-based multiplayer game.
	void						  ReturnFlag();

	//! Frees the model definition for the player
	virtual void				  FreeModelDef();

	//! Returns the self-smooth setting of the player.
	bool						  SelfSmooth();

	//! Sets the self-smooth flag for the player.
	void						  SetSelfSmooth( bool b );

	//! Returns the view bob angles for the player.
	const idAngles&				  GetViewBobAngles() { return viewBobAngles; }

	//! Returns the current view bob offset for the player.
	const idVec3&				  GetViewBob() { return viewBob; }

	//! Returns a reference to the achievement manager associated with the player.
	idAchievementManager&		  GetAchievementManager() { return achievementManager; }

	//! Returns a const reference to the achievement manager associated with the player.
	const idAchievementManager&	  GetAchievementManager() const { return achievementManager; }

	//! Returns the total played time of the player in seconds.
	int							  GetPlayedTime() const { return playedTimeSecs; }

	//! Processes and stores the incoming user command data.
	void						  HandleUserCmds( const usercmd_t& newcmd );

	//! Returns the client-side fire count for the player.
	int							  GetClientFireCount() const { return clientFireCount; }

	//! Increments the client-side fire count for the player.
	void						  IncrementFireCount() { ++clientFireCount; }

	//! Displays a respawn message in the HUD when the minimum respawn time has elapsed after death
	void						  ShowRespawnHudMessage();

	//! Hides the respawn message from the HUD for locally controlled players.
	void						  HideRespawnHudMessage();

	//! Returns true if the player is controlled by the local client.
	bool						  IsLocallyControlled() const { return entityNumber == gameLocal.GetLocalClientNum(); }

	//! Returns the game expansion type associated with the player.
	gameExpansionType_t			  GetExpansionType() const;

	//! Increments the projectile kills counter by one.
	void						  AddProjectileKills() { numProjectileKills++; }

	//! Returns the number of projectile kills accumulated by the player.
	int							  GetProjectileKills() const { return numProjectileKills; }

	//! Resets the projectile kill counter to zero.
	void						  ResetProjectileKills() { numProjectileKills = 0; }

	//! Returns the index of the previously selected weapon.
	int							  GetPrevWeapon() { return previousWeapon; }

	//! Returns the VR face forward matrix for the player.
	const idMat3&				  GetVRFaceForward() { return vrFaceForward; }
	// Leyland end
private:
	// Stats & achievements
	idAchievementManager						 achievementManager;

	int											 playedTimeSecs;
	int											 playedTimeResidual;

	jointHandle_t								 hipJoint;
	jointHandle_t								 chestJoint;
	jointHandle_t								 headJoint;

	idPhysics_Player							 physicsObj; // player physics

	idList<aasLocation_t, TAG_IDLIB_LIST_PLAYER> aasLocation; // for AI tracking the player

	int											 bobFoot;
	float										 bobFrac;
	float										 bobfracsin;
	int											 bobCycle; // for view bobbing and footstep generation
	float										 xyspeed;
	int											 stepUpTime;
	float										 stepUpDelta;
	float										 idealLegsYaw;
	float										 legsYaw;
	bool										 legsForward;
	float										 oldViewYaw;
	idAngles									 viewBobAngles;
	idVec3										 viewBob;
	int											 landChange;
	int											 landTime;

	// Leyland VR
	bool										 hasCameraFirstFrame;
	idVec3										 lastHeadOrigin;
	idMat3										 lastHeadAxisInv;

	bool										 hadBodyYaw;
	float										 oldBodyYaw;

	idMat3										 vrFaceForward;
	// Leyland end

	int											 currentWeapon;
	idPredictedValue<int>						 idealWeapon;
	int											 previousWeapon;
	int											 weaponSwitchTime;
	bool										 weaponEnabled;

	int											 skinIndex;
	const idDeclSkin*							 skin;
	const idDeclSkin*							 powerUpSkin;

	int											 numProjectilesFired; // number of projectiles fired
	int											 numProjectileHits;	  // number of hits on mobs
	int											 numProjectileKills;  // number of kills with a projectile.

	bool										 airless;
	int											 airMsec; // set to pm_airMsec at start, drops in vacuum
	int											 lastAirDamage;

	bool										 gibDeath;
	bool										 gibsLaunched;
	idVec3										 gibsDir;

	idInterpolate<float>						 zoomFov;
	idInterpolate<float>						 centerView;
	bool										 fxFov;

	float										 influenceFov;
	int											 influenceActive; // level of influence.. 1 == no gun or hud .. 2 == 1 + no movement
	idEntity*									 influenceEntity;
	const idMaterial*							 influenceMaterial;
	float										 influenceRadius;
	const idDeclSkin*							 influenceSkin;

	idCamera*									 privateCameraView;

	static const int							 NUM_LOGGED_VIEW_ANGLES = 64;			   // for weapon turning angle offsets
	idAngles									 loggedViewAngles[NUM_LOGGED_VIEW_ANGLES]; // [gameLocal.framenum&(LOGGED_VIEW_ANGLES-1)]
	static const int							 NUM_LOGGED_ACCELS = 16;				   // for weapon turning angle offsets
	loggedAccel_t								 loggedAccel[NUM_LOGGED_ACCELS];		   // [currentLoggedAccel & (NUM_LOGGED_ACCELS-1)]
	int											 currentLoggedAccel;

	// if there is a focusGUIent, the attack button will be changed into mouse clicks
	idEntity*									 focusGUIent;
	idUserInterface*							 focusUI; // focusGUIent->renderEntity.gui, gui2, or gui3
	idAI*										 focusCharacter;
	int											 talkCursor; // show the state of the focusCharacter (0 == can't talk/dead, 1 == ready to talk, 2 == busy talking)
	int											 focusTime;
	idAFEntity_Vehicle*							 focusVehicle;
	idUserInterface*							 cursor;

	// full screen guis track mouse movements directly
	int											 oldMouseX;
	int											 oldMouseY;

	const idMaterial*							 pdaVideoMat;

	bool										 tipUp;
	bool										 objectiveUp;

	int											 lastDamageDef;
	idVec3										 lastDamageDir;
	int											 lastDamageLocation;
	int											 smoothedFrame;
	bool										 smoothedOriginUpdated;
	idVec3										 smoothedOrigin;
	idAngles									 smoothedAngles;

	idHashTable<WeaponToggle_t>					 weaponToggles;

	int											 hudPowerup;
	int											 lastHudPowerup;
	int											 hudPowerupDuration;

	// mp
	bool										 respawning; // set to true while in SpawnToPoint for telefrag checks
	bool										 leader;	 // for sudden death situations
	int											 lastSpectateChange;
	int											 lastTeleFX;
	bool										 weaponCatchup; // raise up the weapon silently ( state catchups )
	int											 MPAim;			// player num in aim
	int											 lastMPAim;
	int											 lastMPAimTime; // last time the aim changed
	int											 MPAimFadeTime; // for GUI fade
	bool										 MPAimHighlight;
	bool										 isTelefragged; // proper obituaries
	int											 serverOverridePositionTime;
	int											 clientFireCount;

	idPlayerIcon								 playerIcon;

	bool										 selfSmooth;

	netBoolEvent_t								 respawn_netEvent;

	//! Sets the player's view angles to look at the entity that caused the damage.
	void										 LookAtKiller( idEntity* inflictor, idEntity* attacker );

	//! Stops the player's firing action and resets related flags and weapon state.
	void										 StopFiring();

	//! Fire the player's weapon if conditions are met.
	void										 FireWeapon();

	//! Handles the combat state and weapon management for the player
	void										 Weapon_Combat();

	//! Handles NPC weapon behavior for the player character.
	void										 Weapon_NPC();

	//! Handles GUI interactions for the player's weapon.
	void										 Weapon_GUI();

	//! Updates the player's weapon state and handles weapon interactions.
	void										 UpdateWeapon();

	//! Returns true if the player uses the classic flashlight mode.
	bool										 UsesClassicFlashlight();

	//! Updates the state and rendering of the player's flashlight
	void										 UpdateFlashlight();

	//! Turns on the player's flashlight if conditions are met.
	void										 FlashlightOn();

	//! Turns off the player's flashlight if it is currently on.
	void										 FlashlightOff();

	//! Updates the player's spectating state and handles input for changing spectator modes.
	void										 UpdateSpectating();

	//! Sets the player to spectate in free fly mode, optionally forcing an immediate change.
	void										 SpectateFreeFly( bool force );

	//! Cycles through spectators in the game.
	void										 SpectateCycle();

	//! Returns a rotational offset for the gun based on view angle history.
	idAngles									 GunTurningOffset();

	//! Computes and returns a positional offset for the gun based on movement history stored in loggedAccelerations.
	idVec3										 GunAcceleratingOffset();

	//! Handles crash landing physics and damage calculation for the player character based on fall velocity and surface properties.
	void										 CrashLand( const idVec3& oldOrigin, const idVec3& oldVelocity );

	//! Calculates and applies bobbing effects for player movement and view animation
	void										 BobCycle( const idVec3& pushVelocity );

	//! Updates the player's view angles based on input and game state.
	void										 UpdateViewAngles();

	//! Evaluates player controls and handles respawning, grabbing, and impulse commands.
	void										 EvaluateControls();

	//! Adjusts the player's speed based on various game states and conditions.
	void										 AdjustSpeed();

	//! Adjusts the player's body angles based on movement and view direction
	void										 AdjustBodyAngles();

	//! Initializes the AAS location data for the player.
	void										 InitAASLocation();

	//! Sets the AAS location for the player
	void										 SetAASLocation();

	//! Updates the player's physics state and movement based on input and game conditions
	void										 Move();

	//! Updates player physics state and view position using interpolated values based on the provided fraction.
	void										 Move_Interpolated( float fraction );

	//! Applies client position correction for remote clients in networked gameplay.
	void										 RunPhysics_RemoteClientCorrection();

	//! Updates the player's power-up states and associated visual effects
	void										 UpdatePowerUps();

	//! Updates the player's death skin visualization state based on health and hitch flag
	void										 UpdateDeathSkin( bool state_hitch );

	//! Clears a specified powerup from the player
	void										 ClearPowerup( int i );

	//! Sets the spectate origin for the player.
	void										 SetSpectateOrigin();

	//! Returns whether client-side physics authorization is allowed based on server time and user command availability.
	bool										 AllowClientAuthPhysics();

	//! Returns the physics time step based on user command or game time
	virtual int									 GetPhysicsTimeStep() const;

	//! Clears all focus-related data members of the player object.
	void										 ClearFocus();

	//! Updates the player's focus on interactive entities and GUIs within view.
	void										 UpdateFocus();

	//! Updates the player's location information in the HUD.
	void										 UpdateLocation();

	//! Returns the currently active user interface for the player.
	idUserInterface*							 ActiveGui();

	//! Handles shared respawn logic for the player.
	void										 Respawn_Shared();

	//! Checks if a specified weapon is available to the player.
	bool										 WeaponAvailable( const char* name );

	//! Uses a vehicle if the player is bound to one or is looking at one.
	void										 UseVehicle();

public:
	//! Returns the current button states for the player.
	void Event_GetButtons();

	//! Returns the player's movement vector including forward, right, and up components.
	void Event_GetMove();

	//! Returns the current view angles of the player as a vector.
	void Event_GetViewAngles();

	//! Stops the FOV effect on the player.
	void Event_StopFxFov();

	//! Enables the player's weapon and exits cinematic mode if the weapon entity exists.
	void Event_EnableWeapon();

	//! Disables the player's weapon by setting weaponEnabled to false and entering cinematic mode if a weapon entity exists.
	void Event_DisableWeapon();

	//! Returns the name of the currently equipped weapon.
	void Event_GetCurrentWeapon();

	//! Returns the name of the previous weapon.
	void Event_GetPreviousWeapon();

	//! Selects a weapon by name for the player.
	void Event_SelectWeapon( const char* weaponName );

	//! Returns the entity of the player's current weapon.
	void Event_GetWeaponEntity();

	//! Toggles the PDA state when not in multiplayer mode.
	void Event_OpenPDA();

	//! Returns whether the objective system is currently open.
	void Event_InPDA();

	//! Handles the player exiting a teleporter and performs cleanup and movement.
	void Event_ExitTeleporter();

	//! Hides the current tip displayed to the player.
	void Event_HideTip();

	//! Handles level trigger events by finding and activating corresponding entities based on the current map name.
	void Event_LevelTrigger();

	//! Handles the player gibbed event.
	void Event_Gibbed();

	//! Sets the player's origin and updates visuals, with a small epsilon offset.
	void Event_ForceOrigin( idVec3& origin, idAngles& angles );

	//! Gives the player an inventory item by name.
	void Event_GiveInventoryItem( const char* name );

	//! Removes an inventory item with the specified name from the player.
	void Event_RemoveInventoryItem( const char* name );

	//! Returns the ideal weapon for the player.
	void Event_GetIdealWeapon();

	//! Returns whether the specified weapon is available to the player.
	void Event_WeaponAvailable( const char* name );

	//! Sets or clears a powerup time for the player.
	void Event_SetPowerupTime( int powerup, int time );

	//! Checks if a specified powerup is currently active for the player.
	void Event_IsPowerupActive( int powerup );

	//! Initializes a warp effect in the player's view.
	void Event_StartWarp();

	//! Stops helltime mode based on the specified mode parameter.
	void Event_StopHelltime( int mode );

	//! Toggles the bloom effect on or off based on the provided integer flag.
	void Event_ToggleBloom( int on );

	//! Sets the bloom effect parameters for the player.
	void Event_SetBloomParms( float speed, float intensity );
};

ID_INLINE bool idPlayer::IsRespawning()
{
	return respawning;
}

ID_INLINE idPhysics* idPlayer::GetPlayerPhysics()
{
	return &physicsObj;
}

ID_INLINE bool idPlayer::IsInTeleport()
{
	return ( teleportEntity.GetEntity() != NULL );
}

ID_INLINE void idPlayer::SetLeader( bool lead )
{
	leader = lead;
}

ID_INLINE bool idPlayer::IsLeader()
{
	return leader;
}

ID_INLINE bool idPlayer::SelfSmooth()
{
	return selfSmooth;
}

ID_INLINE void idPlayer::SetSelfSmooth( bool b )
{
	selfSmooth = b;
}

extern idCVar g_infiniteAmmo;

#endif /* !__GAME_PLAYER_H__ */
