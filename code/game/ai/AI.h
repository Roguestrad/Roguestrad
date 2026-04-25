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

#ifndef __AI_H__
#define __AI_H__

/*
===============================================================================

	idAI

===============================================================================
*/

const float SQUARE_ROOT_OF_2   = 1.414213562f;
const float AI_TURN_PREDICTION = 0.2f;
const float AI_TURN_SCALE	   = 60.0f;
const float AI_SEEK_PREDICTION = 0.3f;
const float AI_FLY_DAMPENING   = 0.15f;
const float AI_HEARING_RANGE   = 2048.0f;
const int	DEFAULT_FLY_OFFSET = 68;

// jmarshall begin

//
// attack flags
//
#define ATTACK_DODGE_LEFT  1
#define ATTACK_DODGE_RIGHT 2
#define ATTACK_COMBAT_NODE 4
#define ATTACK_MELEE	   8
#define ATTACK_LEAP		   16
#define ATTACK_MISSILE	   32
#define ATTACK_SPECIAL1	   64
#define ATTACK_SPECIAL2	   128
#define ATTACK_SPECIAL3	   256
#define ATTACK_SPECIAL4	   512

#define AI_NOT_ACTIVATED   0
#define AI_CHASING_ENEMY   1
#define AI_LOST			   2
#define AI_PATH_FOLLOWING  3
#define AI_ATTACK_NODE	   4

#define ATTACK_IGNORE	   0
#define ATTACK_ON_DAMAGE   1
#define ATTACK_ON_ACTIVATE 2
#define ATTACK_ON_SIGHT	   4

#define ALL_PARTICLES	   -1 // used with setSmokeVisibility
// jmarshall end

typedef struct ballistics_s {
	float angle; // angle in degrees in the range [-180, 180]
	float time;	 // time it takes before the projectile arrives
} ballistics_t;

/*!
	\brief Computes ballistic trajectories for hitting a target with a projectile given start position, end position, speed, and gravity

	Calculates up to two possible ballistic trajectories from a starting position to an ending position based on projectile speed and gravity. Returns the number of valid solutions found. Each
   solution includes the required angle and time of flight to reach the target. The function handles cases where no valid trajectories exist, such as when the target is unreachable given the
   projectile speed.

	\param start The starting position of the projectile
	\param end The target position to hit
	\param speed The initial speed of the projectile
	\param gravity The gravity constant affecting the projectile
	\param bal Array to store the computed ballistic solutions
	\return The number of valid ballistic solutions found, which can be 0, 1, or 2
*/
extern int Ballistics( const idVec3& start, const idVec3& end, float speed, float gravity, ballistics_t bal[2] );

// defined in script/ai_base.script.  please keep them up to date.
typedef enum { MOVETYPE_DEAD, MOVETYPE_ANIM, MOVETYPE_SLIDE, MOVETYPE_FLY, MOVETYPE_STATIC, NUM_MOVETYPES } moveType_t;

typedef enum {
	MOVE_NONE,
	MOVE_FACE_ENEMY,
	MOVE_FACE_ENTITY,

	// commands < NUM_NONMOVING_COMMANDS don't cause a change in position
	NUM_NONMOVING_COMMANDS,

	MOVE_TO_ENEMY = NUM_NONMOVING_COMMANDS,
	MOVE_TO_ENEMYHEIGHT,
	MOVE_TO_ENTITY,
	MOVE_OUT_OF_RANGE,
	MOVE_TO_ATTACK_POSITION, // RB: only used by the sentry AI
	MOVE_TO_COVER,
	MOVE_TO_POSITION,
	MOVE_TO_POSITION_DIRECT,
	MOVE_SLIDE_TO_POSITION,
	MOVE_WANDER,
	NUM_MOVE_COMMANDS
} moveCommand_t;

typedef enum { TALK_NEVER, TALK_DEAD, TALK_OK, TALK_BUSY, NUM_TALK_STATES } talkState_t;

//
// status results from move commands
// make sure to change script/doom_defs.script if you add any, or change their order
//
typedef enum {
	MOVE_STATUS_DONE,
	MOVE_STATUS_MOVING,
	MOVE_STATUS_WAITING,
	MOVE_STATUS_DEST_NOT_FOUND,
	MOVE_STATUS_DEST_UNREACHABLE,
	MOVE_STATUS_BLOCKED_BY_WALL,
	MOVE_STATUS_BLOCKED_BY_OBJECT,
	MOVE_STATUS_BLOCKED_BY_ENEMY,
	MOVE_STATUS_BLOCKED_BY_MONSTER
} moveStatus_t;

#define DI_NODIR -1

// obstacle avoidance
typedef struct obstaclePath_s {
	idVec3	  seekPos;					// seek position avoiding obstacles
	idEntity* firstObstacle;			// if != NULL the first obstacle along the path
	idVec3	  startPosOutsideObstacles; // start position outside obstacles
	idEntity* startPosObstacle;			// if != NULL the obstacle containing the start position
	idVec3	  seekPosOutsideObstacles;	// seek position outside obstacles
	idEntity* seekPosObstacle;			// if != NULL the obstacle containing the seek position
} obstaclePath_t;

// path prediction
typedef enum { SE_BLOCKED = BIT( 0 ), SE_ENTER_LEDGE_AREA = BIT( 1 ), SE_ENTER_OBSTACLE = BIT( 2 ), SE_FALL = BIT( 3 ), SE_LAND = BIT( 4 ) } stopEvent_t;

typedef struct predictedPath_s {
	idVec3			endPos;			// final position
	idVec3			endVelocity;	// velocity at end position
	idVec3			endNormal;		// normal of blocking surface
	int				endTime;		// time predicted
	int				endEvent;		// event that stopped the prediction
	const idEntity* blockingEntity; // entity that blocks the movement
} predictedPath_t;

//
// events
//
extern const idEventDef AI_BeginAttack;
extern const idEventDef AI_EndAttack;
extern const idEventDef AI_MuzzleFlash;
extern const idEventDef AI_CreateMissile;
extern const idEventDef AI_AttackMissile;
extern const idEventDef AI_FireMissileAtTarget;
extern const idEventDef AI_LaunchProjectile;
extern const idEventDef AI_TriggerFX;
extern const idEventDef AI_StartEmitter;
extern const idEventDef AI_StopEmitter;
extern const idEventDef AI_AttackMelee;
extern const idEventDef AI_DirectDamage;
extern const idEventDef AI_JumpFrame;
extern const idEventDef AI_EnableClip;
extern const idEventDef AI_DisableClip;
extern const idEventDef AI_EnableGravity;
extern const idEventDef AI_DisableGravity;
extern const idEventDef AI_TriggerParticles;
extern const idEventDef AI_RandomPath;

class idPathCorner;

typedef struct particleEmitter_s {
	particleEmitter_s()
	{
		particle = NULL;
		time	 = 0;
		joint	 = INVALID_JOINT;
	};
	const idDeclParticle* particle;
	int					  time;
	jointHandle_t		  joint;
} particleEmitter_t;

typedef struct funcEmitter_s {
	char		   name[64];
	idFuncEmitter* particle;
	jointHandle_t  joint;
} funcEmitter_t;

/*!
	\class idMoveState
	\brief A class for managing and persisting movement state data.
*/
class idMoveState
{
public:
	//! Initializes a new instance of the idMoveState class with default values.
	idMoveState();

	//! Saves the move state data to a save file.
	void				  Save( idSaveGame* savefile ) const;

	//! Restores the movement state from a save file.
	void				  Restore( idRestoreGame* savefile );

	moveType_t			  moveType;
	moveCommand_t		  moveCommand;
	moveStatus_t		  moveStatus;
	idVec3				  moveDest;
	idVec3				  moveDir; // used for wandering and slide moves
	idEntityPtr<idEntity> goalEntity;
	idVec3				  goalEntityOrigin; // move to entity uses this to avoid checking the floor position every frame
	int					  toAreaNum;
	int					  startTime;
	int					  duration;
	float				  speed; // only used by flying creatures
	float				  range;
	float				  wanderYaw;
	int					  nextWanderTime;
	int					  blockTime;
	idEntityPtr<idEntity> obstacle;
	idVec3				  lastMoveOrigin;
	int					  lastMoveTime;
	int					  anim;
};

/*!
	\class idAI
	\brief Base class for artificial intelligence entities that handles behavior, movement, and combat interactions.

	This class serves as the foundation for all artificial intelligence entities in the engine, extending from idActor to inherit core character properties and behaviors. The class manages AI-specific
   functionality including enemy detection, pathfinding using AAS navigation, combat actions, animation control, and various movement patterns. It provides comprehensive support for AI state
   management through scripting interfaces, handles damage and death events with appropriate visual and audio feedback, and includes sophisticated projectile and melee attack systems. The
   implementation supports different movement types such as walking, flying, and sliding, with specialized behaviors for each. The class also includes functionality for obstacle avoidance, trajectory
   prediction, and animation controller updates to ensure realistic character movement and response. Memory management is handled through the base class inheritance, with no explicit ownership
   patterns indicated in the method signatures, though the system manages entity references and script objects through standard engine mechanisms. The AI system is designed to integrate with the
   game's scripting system for flexible behavior customization and state transitions.

*/
class idAI : public idActor
{
public:
	CLASS_PROTOTYPE( idAI );

	//! Constructs a new idAI object and initializes all its member variables to default values.
	idAI();

	//! Destructor for the idAI class that cleans up resources and removes associated entities.
	~idAI();

	//! Saves the AI state to a save game file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the AI state from a save file
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the AI entity and its various properties based on spawn arguments.
	void		 Spawn();
	void		 HeardSound( idEntity* ent, const char* action );

	//! Returns the entity that was last heard by the AI if it is within hearing range and meets the specified conditions.
	idEntity*	 HeardSound( int ignore_team );

	//! Returns the current enemy entity of the AI actor.
	idActor*	 GetEnemy() const;

	//! Sets the AI to talk to the specified actor.
	void		 TalkTo( idActor* actor );

	//! Returns the current talk state of the AI, considering dead status and hidden state.
	talkState_t	 GetTalkState() const;

	//! Returns the distance to the current enemy entity.
	float		 EnemyRange();

	/*!
		\brief Computes the aiming direction for projectile firing at a target entity considering trajectory prediction and weapon properties

		The function calculates the optimal aiming direction for an AI character to fire a projectile at a target entity. It handles different target types including actors and static entities, with
	   special logic for enemy targeting. The function uses trajectory prediction to account for projectile physics including gravity and height-to-distance ratios. It first attempts to aim at the
	   chest of the target, and if that fails (or the target is not an actor), attempts to aim at the head. When the target is an enemy, it retrieves specific aiming targets from the actor. For other
	   entity types, it uses the center of the entity's bounding box as the aiming point. The function also implements special handling for specific enemy types like vulgar demons, adjusting the
	   aiming position.

		\param firePos The position from which the projectile is fired
		\param aimAtEnt The entity to aim at
		\param ignore Entity to ignore during trajectory prediction
		\param aimDir Output parameter containing the calculated aim direction
		\return True if a valid aim direction could be computed, false otherwise
	*/
	bool		 GetAimDir( const idVec3& firePos, idEntity* aimAtEnt, const idEntity* ignore, idVec3& aimDir ) const;

	//! Activates the AI when touched by a flashlight if wakeOnFlashlight is enabled.
	void		 TouchedByFlashlight( idActor* flashlight_owner );

	//! Outputs a list of all monsters to the console.
	static void	 List_f( const idCmdArgs& args );

	/*!
		\brief Finds a path around dynamic obstacles for AI navigation

		This function calculates a path around dynamic obstacles for AI movement by analyzing the given physics and AAS data. It determines obstacle positions, finds valid start and goal positions
	   outside obstacles, builds a path tree, prunes it, and finds an optimal path to the destination. The function handles cases where the start and goal positions might be inside obstacles by
	   finding alternative positions outside them. It returns true if a valid path exists, false if the path cannot be found or if the start and destination are pushed to the same point.

		\param physics Physics object containing the AI's physical properties
		\param aas Area Awareness System data for navigation
		\param ignore Entity to ignore when checking for obstacles
		\param startPos Starting position for the path
		\param seekPos Target position to reach
		\param path Output structure containing the calculated obstacle path information
		\return True if a valid path around obstacles exists and was calculated successfully, false if path calculation fails or if start and destination are too close
	*/
	static bool	 FindPathAroundObstacles( const idPhysics* physics, const idAAS* aas, const idEntity* ignore, const idVec3& startPos, const idVec3& seekPos, obstaclePath_t& path );

	//! Frees any nodes used for dynamic obstacle avoidance.
	static void	 FreeObstacleAvoidanceNodes();

	/*!
		\brief Predicts the movement path of an entity using AAS navigation and returns true if a stop event was triggered.

		This function calculates the predicted path of an entity moving through a navigation area network (AAS) based on its initial position, velocity, and physics settings. It simulates the movement
	   frame by frame, accounting for gravity, collisions, and stepping over obstacles. The function handles sliding along surfaces and stepping up obstacles up to three times per frame. It returns
	   true if a stop event occurs during the simulation, such as hitting a wall, floor, or other blocking entity, and sets the path details accordingly. The simulation continues for the specified
	   total time, broken into frames determined by frameTime.

		\param ent Entity being predicted
		\param aas Area Awareness System for navigation data
		\param start Starting position of the entity
		\param velocity Initial velocity of the entity
		\param totalTime Total time to simulate movement
		\param frameTime Time interval for each simulation frame
		\param stopEvent Event types that will cause the prediction to stop
		\param path Output structure containing the predicted path details
		\return True if a stop event was encountered during path prediction, false otherwise
	*/
	static bool	 PredictPath( const idEntity* ent, const idAAS* aas, const idVec3& start, const idVec3& velocity, int totalTime, int frameTime, int stopEvent, predictedPath_t& path );

	/*!
		\brief Tests if a projectile trajectory from start to end position is collision-free and respects height constraints.

		This function evaluates a parabolic trajectory for a projectile moving from a start position to an end position, taking into account initial vertical velocity, gravity, and a maximum allowed
	   height. It checks if the trajectory intersects with any obstacles using collision detection. The function also supports visual debugging by drawing the trajectory and collision points. The
	   trajectory is divided into segments for collision testing, and the function returns true only if the entire trajectory is free of collisions and does not exceed the specified maximum height.

		\param start The starting position of the projectile trajectory
		\param end The ending position of the projectile trajectory
		\param zVel The initial vertical velocity of the projectile
		\param gravity The gravitational force acting on the projectile
		\param time The total time for the projectile to travel from start to end
		\param max_height The maximum height allowed for the projectile trajectory
		\param clip The clip model representing the projectile's physical bounds
		\param clipmask The collision mask used for determining which entities to collide with
		\param ignore Entity to ignore during collision testing
		\param targetEntity Entity to check for specific collision behavior
		\param drawtime Time in milliseconds to visually draw the trajectory and collision results
		\return True if the projectile trajectory is collision-free and within the specified height constraints, false otherwise
	*/
	static bool	 TestTrajectory( const idVec3& start,
		 const idVec3&						   end,
		 float								   zVel,
		 float								   gravity,
		 float								   time,
		 float								   max_height,
		 const idClipModel*					   clip,
		 int								   clipmask,
		 const idEntity*					   ignore,
		 const idEntity*					   targetEntity,
		 int								   drawtime );

	/*!
		\brief Computes and returns the best collision-free trajectory for a projectile from fire position to target position, considering gravity and collision detection.

		This function determines the optimal firing angle for a projectile to reach a target while avoiding collisions with obstacles. It uses ballistic calculations to compute possible trajectories
	   based on the projectile speed and gravity. If a valid trajectory is found, the function sets the aim direction vector to the computed trajectory and returns true. Otherwise, it returns false
	   and sets the aim direction to the straight-line vector from fire position to target. The function also supports debugging visualization of the trajectory if enabled.

		\param firePos Position from where the projectile is fired
		\param target Target position that the projectile should reach
		\param projectileSpeed Initial speed of the projectile
		\param projGravity Gravity vector affecting the projectile
		\param clip Clip model representing the projectile's physical bounds
		\param clipmask Clip mask used for collision detection
		\param max_height Maximum height above the fire position that the trajectory can reach
		\param ignore Entity to ignore during collision detection
		\param targetEntity Entity that is the target of the projectile
		\param drawtime Duration for which to draw debug visualization of the trajectory
		\param aimDir Output parameter that receives the computed trajectory direction vector
		\return True if a collision-free trajectory is found, false otherwise
	*/
	static bool	 PredictTrajectory( const idVec3& firePos,
		 const idVec3&							  target,
		 float									  projectileSpeed,
		 const idVec3&							  projGravity,
		 const idClipModel*						  clip,
		 int									  clipmask,
		 float									  max_height,
		 const idEntity*						  ignore,
		 const idEntity*						  targetEntity,
		 int									  drawtime,
		 idVec3&								  aimDir );

	//! Causes the AI entity to gib and notifies the harvest entity if it exists.
	virtual void Gib( const idVec3& dir, const char* damageDefName );

	// jmarshall begin
protected:
	//! Initializes the AI system
	virtual void Init() { }

	//! Checks for the presence of an enemy within the specified field of view.
	virtual bool checkForEnemy( float use_fov );

private:
	//! Executes actions and triggers associated with path entities in a sequence until no next path entity is found.
	void idle_followPathEntities( idEntity* pathnode );

protected:
	idScriptBool					  ambush;
	idScriptBool					  ignoreEnemies;	  // used to disable enemy checks during attack_path
	idScriptBool					  stay_on_attackpath; // used to disable enemy checks during attack_path
	idScriptBool					  ignore_sight;
	idScriptBool					  idle_sight_fov;
	// jmarshall end

	// navigation
	idAAS*							  aas;
	int								  travelFlags;

	idMoveState						  move;
	idMoveState						  savedMove;

	float							  kickForce;
	bool							  ignore_obstacles;
	float							  blockedRadius;
	int								  blockedMoveTime;
	int								  blockedAttackTime;

	// turning
	float							  ideal_yaw;
	float							  current_yaw;
	float							  turnRate;
	float							  turnVel;
	float							  anim_turn_yaw;
	float							  anim_turn_amount;
	float							  anim_turn_angles;

	// physics
	idPhysics_Monster				  physicsObj;

	// flying
	jointHandle_t					  flyTiltJoint;
	float							  fly_speed;
	float							  fly_bob_strength;
	float							  fly_bob_vert;
	float							  fly_bob_horz;
	int								  fly_offset; // prefered offset from player's view
	float							  fly_seek_scale;
	float							  fly_roll_scale;
	float							  fly_roll_max;
	float							  fly_roll;
	float							  fly_pitch_scale;
	float							  fly_pitch_max;
	float							  fly_pitch;

	bool							  allowMove;		   // disables any animation movement
	bool							  allowHiddenMovement; // allows character to still move around while hidden
	bool							  disableGravity;	   // disables gravity and allows vertical movement by the animation
	bool							  af_push_moveables;   // allow the articulated figure to push moveable objects

	// weapon/attack vars
	bool							  lastHitCheckResult;
	int								  lastHitCheckTime;
	int								  lastAttackTime;
	float							  melee_range;
	float							  projectile_height_to_distance_ratio; // calculates the maximum height a projectile can be thrown
	idList<idVec3, TAG_AI>			  missileLaunchOffset;

	const idDict*					  projectileDef;
	mutable idClipModel*			  projectileClipModel;
	float							  projectileRadius;
	float							  projectileSpeed;
	idVec3							  projectileVelocity;
	idVec3							  projectileGravity;
	idEntityPtr<idProjectile>		  projectile;
	idStr							  attack;
	idVec3							  homingMissileGoal;

	// chatter/talking
	const idSoundShader*			  chat_snd;
	int								  chat_min;
	int								  chat_max;
	int								  chat_time;
	talkState_t						  talk_state;
	idEntityPtr<idActor>			  talkTarget;

	// cinematics
	int								  num_cinematics;
	int								  current_cinematic;

	bool							  allowJointMod;
	idEntityPtr<idEntity>			  focusEntity;
	idVec3							  currentFocusPos;
	int								  focusTime;
	int								  alignHeadTime;
	int								  forceAlignHeadTime;
	idAngles						  eyeAng;
	idAngles						  lookAng;
	idAngles						  destLookAng;
	idAngles						  lookMin;
	idAngles						  lookMax;
	idList<jointHandle_t, TAG_AI>	  lookJoints;
	idList<idAngles, TAG_AI>		  lookJointAngles;
	float							  eyeVerticalOffset;
	float							  eyeHorizontalOffset;
	float							  eyeFocusRate;
	float							  headFocusRate;
	int								  focusAlignTime;

	// special fx
	bool							  restartParticles; // should smoke emissions restart
	bool							  useBoneAxis;		// use the bone vs the model axis
	idList<particleEmitter_t, TAG_AI> particles;		// particle data

	renderLight_t					  worldMuzzleFlash; // positioned on world weapon bone
	int								  worldMuzzleFlashHandle;
	jointHandle_t					  flashJointWorld;
	int								  muzzleFlashEnd;
	int								  flashTime;

	// joint controllers
	idAngles						  eyeMin;
	idAngles						  eyeMax;
	jointHandle_t					  focusJoint;
	jointHandle_t					  orientationJoint;

	// enemy variables
	idEntityPtr<idActor>			  enemy;
	idVec3							  lastVisibleEnemyPos;
	idVec3							  lastVisibleEnemyEyeOffset;
	idVec3							  lastVisibleReachableEnemyPos;
	idVec3							  lastReachableEnemyPos;
	bool							  wakeOnFlashlight;

	bool							  spawnClearMoveables;

	bool							  isAwake; // jmarshall

	idHashTable<funcEmitter_t>		  funcEmitters;

	idEntityPtr<idHarvestable>		  harvestEnt;

	// script variables
	idScriptBool					  AI_TALK;
	idScriptBool					  AI_DAMAGE;
	idScriptBool					  AI_PAIN;
	idScriptFloat					  AI_SPECIAL_DAMAGE;
	idScriptBool					  AI_DEAD;
	idScriptBool					  AI_RUN;		// jmarshall
	idScriptBool					  blocked;		// jmarshall: its stupid they had two block states.
	idScriptBool					  AI_ATTACKING; // jmarshall
	idScriptBool					  AI_ENEMY_VISIBLE;
	idScriptBool					  AI_ENEMY_IN_FOV;
	idScriptBool					  AI_ENEMY_DEAD;
	idScriptBool					  AI_MOVE_DONE;
	idScriptBool					  AI_ONGROUND;
	idScriptBool					  AI_ACTIVATED;
	idScriptBool					  AI_FORWARD;
	idScriptBool					  AI_JUMP;
	idScriptBool					  AI_ENEMY_REACHABLE;
	idScriptBool					  AI_BLOCKED;
	idScriptBool					  AI_OBSTACLE_IN_PATH;
	idScriptBool					  AI_DEST_UNREACHABLE;
	idScriptBool					  AI_HIT_ENEMY;
	idScriptBool					  AI_PUSHED;

	idScriptFloat					  run_distance; // jmarshall
	idScriptFloat					  walk_turn;	// jmarshall

	//! Sets the AAS for the AI entity based on the spawn argument.
	void							  SetAAS();

	//! Called when the AI entity becomes dormant.
	virtual void					  DormantBegin();

	//! Called when the AI entity wakes from being dormant.
	virtual void					  DormantEnd();

	//! Executes the AI behavior and updates the state of the entity.
	void							  Think();

	//! Activates the AI entity and triggers appropriate responses based on the activating entity.
	void							  Activate( idEntity* activator );

public:
	//! Returns the AI reaction type to a given entity based on team and rank.
	int				  ReactionTo( const idEntity* ent );

	//! Creates or reinitializes the script thread for the AI entity.
	virtual idThread* ConstructScriptObject();

	//! Clears the AI's enemy reference and updates related flags.
	void			  ClearEnemy();

protected:
	virtual void		  AI_Begin() {};

	//! Returns zero, indicating no attacks are checked.
	virtual int			  check_attacks() { return 0; }

	//! Performs an attack action with the specified attack flags.
	virtual void		  do_attack( int attack_flags ) { }

	//! Handles the event when an AI enemy dies.
	void				  enemy_dead();

	//! Plays a custom animation on the AI entity with specified blend times.
	void				  PlayCustomAnim( idStr animname, float blendIn, float blendOut );

	//! Executes a custom animation cycle on the AI entity using the specified animation name and blending time.
	void				  PlayCustomCycle( idStr animname, float blendTime );

	//! Triggers wakeup targets by finding and activating entities with the specified target names.
	void				  trigger_wakeup_targets();

	//! Makes the AI face and activate an enemy sight animation.
	void				  sight_enemy();

	//! Calls the constructor for the AI script object.
	void				  CallConstructor();

	//! Clears the enemy and sets the enemy dead flag.
	void				  EnemyDead();

	//! Determines whether the AI can play chatter sounds based on its state and conditions.
	virtual bool		  CanPlayChatterSounds() const;

	//! Sets the chat sound for the AI based on its state and spawn arguments.
	void				  SetChatSound();

	//! Plays a chat sound for the AI character.
	void				  PlayChatter();

	//! Hides the AI actor and disables damage.
	virtual void		  Hide();

	//! Shows the AI entity and initializes its physics contents based on spawn arguments.
	virtual void		  Show();

	//! Returns the first visible point on the path from the origin to the target.
	idVec3				  FirstVisiblePointOnPath( const idVec3 origin, const idVec3& target, int travelFlags ) const;

	//! Calculates joint positions for attack frames to enable proper hit detection tests.
	void				  CalculateAttackOffsets();

	//! Plays the next cinematic animation in the sequence for the AI entity.
	void				  PlayCinematic();

	/*!
		\brief Applies a physics impulse to the AI entity, skipping the operation for static or slide movement types.

		This function applies a physics impulse to the AI entity, but only if the entity's movement type is not MOVETYPE_STATIC or MOVETYPE_SLIDE. This prevents certain monsters, like Sabaoth, from
	   being knocked off their intended path by physics impulses. The function delegates the actual impulse application to the parent idActor::ApplyImpulse method for eligible movement types.

		\param ent The entity that applies the impulse
		\param id The identifier for the impulse application
		\param point The point in world space where the impulse is applied
		\param impulse The impulse vector to be applied to the entity
	*/
	virtual void		  ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );

	//! Calculates the movement delta for an AI entity based on axis transformations and model offset.
	void				  GetMoveDelta( const idMat3& oldaxis, const idMat3& axis, idVec3& delta );

	//! Checks for obstacles along the path to the goal position and adjusts the new position accordingly
	void				  CheckObstacleAvoidance( const idVec3& goalPos, idVec3& newPos );

	//! Executes the movement behavior for a dead AI entity.
	void				  DeadMove();

	//! Moves the AI entity according to its current movement command and physics state.
	void				  AnimMove();

	//! Moves the AI entity while sliding along surfaces and avoiding obstacles
	void				  SlideMove();

	//! Adjusts the flying angles of the AI based on its velocity and configured flight parameters.
	void				  AdjustFlyingAngles();

	//! Adds a fly bob motion to the provided velocity vector.
	void				  AddFlyBob( idVec3& vel );

	//! Adjusts the flying height of the AI to navigate through obstacles and maintain proper altitude.
	void				  AdjustFlyHeight( idVec3& vel, const idVec3& goalPos );

	//! Computes and applies a velocity adjustment to seek a goal position while flying.
	void				  FlySeekGoal( idVec3& vel, idVec3& goalPos );

	//! Adjusts the flying speed of the AI based on dampening and target speed.
	void				  AdjustFlySpeed( idVec3& vel );

	//! Performs turning actions for the AI during flight based on movement commands and velocity.
	void				  FlyTurn();

	//! Moves the AI entity through flight while handling obstacles and updating physics.
	void				  FlyMove();

	//! Performs static movement for the AI entity, handling turning and physics updates.
	void				  StaticMove();

	/*!
		\brief Handles pain and damage events for the AI entity, including updating damage state and potentially triggering enemy reactions.

		This function processes pain and damage events for the AI entity. It updates internal state variables such as AI_PAIN and AI_DAMAGE, and manages blinking behavior. The function evaluates
	   whether the damage came from an attacker different from itself, and if so, checks for special damage properties from the inflictor. It also assesses if the attacker is an actor and whether a
	   reaction to damage should be triggered, potentially alerting AI and setting the attacker as an enemy.

		\param inflictor Entity that caused the damage
		\param attacker Entity that inflicted the damage
		\param damage Amount of damage inflicted
		\param dir Direction of the damage source
		\param location Location of the damage on the entity
		\return True if the AI experienced pain, false otherwise
	*/
	virtual bool		  Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	/*!
		\brief Handles the death of an AI entity, managing animations, sounds, ragdoll effects, and triggering death-related events.

		This function is called when an AI entity is killed. It processes the death event by handling various game state changes, such as stopping sounds, disabling gravity, clearing enemy targets,
	   and setting the entity to a dead state. It also manages ragdoll physics, plays death sounds, and triggers death-related effects like particle systems and model changes. The function handles
	   special cases like guardian boss achievements, death sounds for specific entity types, and spawning loot or harvestable items upon death. It also updates player statistics when the attacker is
	   a player.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage dealt
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
	*/
	virtual void		  Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Kicks obstacles out of the AI's path using force applied to pushable entities.
	void				  KickObstacles( const idVec3& dir, float force, idEntity* alwaysKick );

	//! Checks if the AI has reached a specified position based on the movement command and current state.
	bool				  ReachedPos( const idVec3& pos, const moveCommand_t moveCommand ) const;

	//! Calculates the travel distance between two points using AAS navigation data
	float				  TravelDistance( const idVec3& start, const idVec3& end ) const;

	//! Returns the area number of the AAS area reachable from the given position.
	int					  PointReachableAreaNum( const idVec3& pos, const float boundsScale = 2.0f ) const;

	/*!
		\brief Computes a path from an origin position to a goal position within the AAS navigation system

		This function calculates a valid path between two points in the navigation mesh. It first ensures both the start and goal positions are properly positioned within their respective areas by
	   pushing them into the correct AAS areas. The function then chooses between flying or walking pathfinding based on the AI's movement type. The resulting path is stored in the provided path
	   structure and the function returns true if a valid path was found, false otherwise

		\param path Output parameter that will contain the computed path data
		\param areaNum The AAS area number where the origin position is located
		\param origin Starting position for pathfinding
		\param goalAreaNum The AAS area number where the goal position is located
		\param goalOrigin Target position for pathfinding
		\return True if a valid path was computed and stored in the path parameter, false otherwise
	*/
	bool				  PathToGoal( aasPath_t& path, int areaNum, const idVec3& origin, int goalAreaNum, const idVec3& goalOrigin ) const;

	//! Draws the AI's movement route using the AAS system
	void				  DrawRoute() const;

	//! Determines the next position for AI movement based on current move command and pathfinding.
	bool				  GetMovePos( idVec3& seekPos );

	//! Returns true if the AI has finished its current movement command.
	bool				  MoveDone() const;

	//! Determines if an actor can see a specific position by checking visibility through tracing.
	bool				  EntityCanSeePos( idActor* actor, const idVec3& actorOrigin, const idVec3& pos );

	//! Handles the AI's blocked fail-safe mechanism to prevent getting stuck.
	void				  BlockedFailSafe();

	//! Stops the AI's movement and updates its movement status.
	void				  StopMove( moveStatus_t status );

	//! Faces the enemy's last known position and returns true if successful.
	bool				  FaceEnemy();

	//! Faces the specified entity and returns true if successful.
	bool				  FaceEntity( idEntity* ent );

	//! Moves the AI directly to the specified position.
	bool				  DirectMoveToPosition( const idVec3& pos );

	//! Moves the AI entity to the height of the enemy.
	bool				  MoveToEnemyHeight();

	//! Moves the AI entity out of range of a specified entity
	bool				  MoveOutOfRange( idEntity* entity, float range );

	//! Moves the AI entity to an attack position relative to the specified entity using the given attack animation.
	bool				  MoveToAttackPosition( idEntity* ent, int attack_anim );

	//! Moves the AI entity toward its current enemy target.
	bool				  MoveToEnemy();

	//! Moves the AI entity towards a specified target entity.
	bool				  MoveToEntity( idEntity* ent );

	//! Moves the AI entity to the specified position.
	bool				  MoveToPosition( const idVec3& pos );

	//! Moves the AI entity to a cover position while avoiding the specified entity.
	bool				  MoveToCover( idEntity* entity, const idVec3& pos );

	//! Moves the AI to the specified position using a sliding motion over the given time.
	bool				  SlideToPosition( const idVec3& pos, float time );

	//! Moves the AI to a random destination within a wandering range.
	bool				  WanderAround();

	//! Updates the AI's movement direction and predicts the resulting path to determine if the movement is valid.
	bool				  StepDirection( float dir );

	//! Computes and sets a new wander direction for the AI toward the specified destination.
	bool				  NewWanderDir( const idVec3& dest );

	//! Spawns particles at a specified joint position on the AI entity.
	const idDeclParticle* SpawnParticlesOnJoint( particleEmitter_t& pe, const char* particleName, const char* jointName );

	//! Spawns particle effects on joints based on spawn arguments matching a given key prefix.
	void				  SpawnParticles( const char* keyName );

	//! Checks if the AI is facing the ideal yaw angle.
	bool				  FacingIdeal();

	//! Updates the AI's yaw angle based on the ideal yaw and turn rate
	void				  Turn();

	//! Updates the AI's ideal yaw angle and checks if it is already facing that direction.
	bool				  TurnToward( float yaw );

	//! Turns the AI entity toward the specified position.
	bool				  TurnToward( const idVec3& pos );

	//! Checks if the AI's enemy position is valid based on visibility and line of sight.
	bool				  EnemyPositionValid() const;

	//! Updates the AI's record of the enemy's position and determines if the enemy is reachable.
	void				  SetEnemyPosition();

	//! Updates the AI's recorded position of its enemy based on visibility and reachability checks.
	void				  UpdateEnemyPosition();

	//! Sets the enemy of the AI actor to the specified enemy entity.
	void				  SetEnemy( idActor* newEnemy );

	//! Checks if the AI can reach a specified entity using pathfinding.
	bool				  CanReachEntity( idEntity* ent );

	//! Checks if the AI can reach its enemy using AAS pathfinding.
	bool				  CanReachEnemy();

	//! Creates a projectile clip model for the AI if one does not already exist.
	void				  CreateProjectileClipModel() const;

	//! Creates and returns a projectile entity for the AI at the specified position and direction.
	idProjectile*		  CreateProjectile( const idVec3& pos, const idVec3& dir );

	//! Removes the AI's projectile entity if one exists.
	void				  RemoveProjectile();

	//! Launches a projectile from the AI entity toward a target, optionally clamped to an attack cone.
	idProjectile*		  LaunchProjectile( const char* jointname, idEntity* target, bool clampToAttackCone );

	//! Handles damage feedback for AI entities based on victim, inflictor, and damage values.
	virtual void		  DamageFeedback( idEntity* victim, idEntity* inflictor, int& damage );

	//! Applies direct melee damage to an entity using the specified damage definition.
	void				  DirectDamage( const char* meleeDefName, idEntity* ent );

	//! Tests if the AI can perform a melee attack on its enemy.
	bool				  TestMelee() const;

	//! Performs a melee attack using the specified melee definition against the AI's enemy
	bool				  AttackMelee( const char* meleeDefName );

	//! Initializes the attack state for the AI with the specified attack name and records the start time.
	void				  BeginAttack( const char* name );

	//! Clear the current attack string for the AI entity.
	void				  EndAttack();

	//! Applies force to entities touching the AI's articulated figure.
	void				  PushWithAF();

	//! Retrieves the muzzle position and axis for a specified joint, or calculates a default muzzle position if no joint is provided.
	void				  GetMuzzle( const char* jointname, idVec3& muzzle, idMat3& axis );

	//! Initializes the muzzle flash effect for the AI entity.
	void				  InitMuzzleFlash();

	//! Triggers weapon muzzle flash effects at the specified muzzle position
	void				  TriggerWeaponEffects( const idVec3& muzzle );

	//! Updates the muzzle flash effect for the AI entity.
	void				  UpdateMuzzleFlash();

	//! Updates the animation controllers for the AI character, handling look direction and joint positioning.
	virtual bool		  UpdateAnimationControllers();

	//! Updates the particles associated with the AI character.
	void				  UpdateParticles();

	//! Triggers particle effects attached to a specified joint on the AI
	void				  TriggerParticles( const char* jointName );

	//! Sets the AI state to LostCombat when combat is lost.
	void				  combat_lost();

	//! Returns the best combat node for the AI to use in combat based on enemy position and node visibility.
	idEntity*			  GetCombatNode();

	//! Tests if an animation move is valid for the AI character.
	bool				  TestAnimMove( const char* animname );

	//! Triggers a particle effect at a specified joint position on the AI entity.
	void				  TriggerFX( const char* joint, const char* fx );

	//! Creates or retrieves a particle emitter entity bound to a specified joint.
	idEntity*			  StartEmitter( const char* name, const char* joint, const char* particle );

	//! Returns the entity associated with the specified emitter name, or NULL if not found.
	idEntity*			  GetEmitter( const char* name );

	//! Stops a particle emitter by name and removes it from the active emitters list.
	void				  StopEmitter( const char* name );

	//! Finds an enemy actor that is visible within combat nodes.
	idEntity*			  FindEnemyInCombatNodes();

	//! Checks if the AI entity is blocked and handles the blocking state
	stateResult_t		  check_blocked( stateParms_t* parms, bool& result );

	//! Handles the combat chase state for an AI entity, managing movement towards and engagement with an enemy.
	stateResult_t		  combat_chase( stateParms_t* parms, bool& result );

	//! Links script variables to the AI's script object for state management
	void				  LinkScriptVariables();

	//! Updates the AI script and handles enemy hit detection and animation state.
	void				  UpdateAIScript();

	//! Performs a melee attack from the AI's eye position to a specified joint on the AI's model.
	bool				  MeleeAttackToJoint( const char* jointname, const char* meleeDefName );

	//! Returns the closest hidden target entity of the specified type that is not visible to the enemy.
	idEntity*			  GetClosestHiddenTarget( const char* type );

	//! Handles the spawner state for an AI entity, managing the spawning of other entities based on configuration.
	stateResult_t		  state_Spawner( stateParms_t* parms );

	//! Handles the AI state when waking up from sleep or idle state.
	stateResult_t		  state_WakeUp( stateParms_t* parms );

	//! Handles the AI state transition for waking up the entity when attacked within an attack cone
	stateResult_t		  wake_on_attackcone( stateParms_t* parms );

	//! Handles the AI state when the entity is activated by a trigger
	stateResult_t		  walk_on_trigger( stateParms_t* parms );

	//! Sets the AI state to awake and triggers wakeup targets when activated.
	stateResult_t		  wake_on_trigger( stateParms_t* parms );

	//! Handles the AI state transition when the entity wakes up and begins interacting with enemies.
	stateResult_t		  wake_on_enemy( stateParms_t* parms );

	//! Waits for an enemy to be acquired or for a timeout to occur.
	stateResult_t		  wait_for_enemy( stateParms_t* parms );

	//! Executes a state that hides the AI until triggered, then plays a custom animation and wakes up targets.
	stateResult_t		  state_TriggerAnim( stateParms_t* parms );

	//! Handles the teleportation state for an AI entity when triggered.
	stateResult_t		  state_TeleportTriggered( stateParms_t* parms );

	//! Handles the AI state where the entity remains hidden until triggered.
	stateResult_t		  state_TriggerHidden( stateParms_t* parms );

	//! Initializes the AI state and calls the constructor for the AI object.
	stateResult_t		  wake_call_constructor( stateParms_t* parms );

	//! Handles the killed state for an AI entity, managing the death animation and transition to dead state.
	stateResult_t		  state_Killed( stateParms_t* parms );

	//! Handles the dead state for an AI entity.
	stateResult_t		  state_Dead( stateParms_t* parms );

	//! Executes a combat wandering state for an AI entity, managing movement and transition to combat state.
	stateResult_t		  combat_wander( stateParms_t* parms );

	//! Handles the AI's behavior when it loses combat, determining whether to move to a hidden target or continue without one.
	stateResult_t		  state_LostCombat( stateParms_t* parms );

	//! Handles the AI state when the entity has lost combat and is not near a node.
	stateResult_t		  state_LostCombat_No_Node( stateParms_t* parms );

	//! Handles the AI's movement and state transitions when the AI has lost combat and is moving toward a designated lost combat node.
	stateResult_t		  state_LostCombat_Node( stateParms_t* parms );

	//! Handles the finish state of a lost combat situation for an AI entity.
	stateResult_t		  state_LostCombat_Finish( stateParms_t* parms );

	//! This function handles the main combat state for AI enemies, managing enemy chasing, combat actions, and state transitions.
	stateResult_t		  state_Combat( stateParms_t* parms );

	//! Checks if the AI can hit its enemy with a shot
	bool				  CanHitEnemy();

	//! Checks if the specified entity is within the AI's attack cone.
	bool				  EntityInAttackCone( idEntity* ent );

	//! Returns the 2D distance to the AI's enemy entity, or a large value if no enemy is present.
	float				  EnemyRange2D();

	//! Tests a charge attack against the enemy and returns the estimated time to reach the target position.
	float				  TestChargeAttack();

	//! Returns the world position of the enemy's eye based on the last visible enemy position and eye offset.
	idVec3				  GetEnemyEyePos();

	//! Calculates the velocity vector for a jump to reach a target position with specified speed and maximum height.
	idVec3				  GetJumpVelocity( const idVec3& pos, float speed, float max_height );

	// jmarshall end

	//! Activates the AI entity using the provided activator entity.
	void				  Event_Activate( idEntity* activator );

	//! Finds and returns the first visible enemy actor within the player's PVS that has attack-on-sight reaction.
	idActor*			  FindEnemy( int useFOV );

	//! Finds the closest visible enemy actor within the AI's field of view.
	idActor*			  FindEnemyAI( int useFOV );

	//! Handles touch events for the AI entity, activating when encountering eligible targets.
	void				  Event_Touch( idEntity* other, trace_t* trace );

	//! Finds and returns the first visible enemy actor within the player's PVS that the AI can see.
	void				  Event_FindEnemy( int useFOV );

	//! Finds and returns the nearest enemy AI entity within the specified field of view.
	void				  Event_FindEnemyAI( int useFOV );

	//! Checks for an enemy within the specified field of view and returns the result as a float.
	void				  Event_CheckForEnemy( float use_fov );

	//! Returns the enemy entity found in combat nodes.
	void				  Event_FindEnemyInCombatNodes();

	//! Predicts the position of the enemy at a given time in the future based on their current movement.
	idVec3				  PredictEnemyPos( float time );

	//! Finds the closest reachable enemy of a given team mate entity.
	void				  Event_ClosestReachableEnemyOfEntity( idEntity* team_mate );

	//! Returns the entity that was heard within hearing range, or NULL if no sound was detected.
	void				  Event_HeardSound( int ignore_team );

	//! Sets the enemy of the AI to the specified entity.
	void				  Event_SetEnemy( idEntity* ent );

	//! Clears the AI's current enemy target.
	void				  Event_ClearEnemy();

	//! Triggers muzzle flash effects at the specified joint position
	void				  Event_MuzzleFlash( const char* jointname );

	//! Creates a missile projectile from the AI entity at the specified joint.
	void				  Event_CreateMissile( const char* jointname );

	//! Launches a missile projectile from the specified joint and returns the projectile entity.
	void				  Event_AttackMissile( const char* jointname );

	//! Tests if an animation move will advance toward the enemy
	bool				  TestAnimMoveTowardEnemy( const char* animname );

	//! Fires a missile at the specified target entity from the given joint position
	void				  Event_FireMissileAtTarget( const char* jointname, const char* targetname );

	//! Launches a missile projectile from the AI entity
	void				  Event_LaunchMissile( const idVec3& muzzle, const idAngles& ang );

	//! Launches a homing missile projectile at the AI's current enemy.
	void				  Event_LaunchHomingMissile();

	//! Sets the homing missile goal to the current enemy's position.
	void				  Event_SetHomingMissileGoal();

	//! Launches a projectile using the specified entity definition and fires it from the AI's muzzle direction.
	void				  Event_LaunchProjectile( const char* entityDefName );

	//! Performs a melee attack using the specified attack definition and returns whether the attack hit.
	void				  Event_AttackMelee( const char* meleeDefName );

	//! Applies direct damage to a target entity using a specified damage definition.
	void				  Event_DirectDamage( idEntity* damageTarget, const char* damageDefName );

	//! Determines if the AI can hit an enemy using a specific animation.
	bool				  CanHitEnemyFromAnim( const char* animname );

	//! Applies radius damage from a specified joint or the entity's origin.
	void				  Event_RadiusDamageFromJoint( const char* jointname, const char* damageDefName );

	//! Starts the attack animation and behavior for the AI entity using the specified attack name.
	void				  Event_BeginAttack( const char* name );

	//! Ends the current attack action performed by the AI entity.
	void				  Event_EndAttack();

	//! Performs a melee attack at a specified joint using the given melee definition.
	void				  Event_MeleeAttackToJoint( const char* jointname, const char* meleeDefName );

	//! Returns a random path corner entity from the AI's pathfinding system.
	void				  Event_RandomPath();

	//! Returns whether the AI can become solid.
	void				  Event_CanBecomeSolid();

	//! Determines if the AI can transition to a solid state without colliding with any obstacles.
	bool				  CanBecomeSolid();

	//! Makes the AI entity solid and sets its collision properties based on spawn arguments.
	void				  Event_BecomeSolid();

	//! Makes the AI entity non-solid and disables damage.
	void				  Event_BecomeNonSolid();

	//! Makes the AI entity enter a ragdoll state.
	void				  Event_BecomeRagdoll();

	//! Stops the ragdoll physics simulation and restores the monster physics.
	void				  Event_StopRagdoll();

	//! Sets the health value of the AI entity and updates its dead state accordingly.
	void				  Event_SetHealth( float newHealth );

	//! Returns the health of the AI entity as a floating-point value.
	void				  Event_GetHealth();

	//! Enables damage taking for the AI entity.
	void				  Event_AllowDamage();

	//! Sets the AI to ignore damage by disabling take damage flag.
	void				  Event_IgnoreDamage();

	//! Returns the current yaw angle of the AI entity.
	void				  Event_GetCurrentYaw();

	//! Turns the AI entity toward the specified angle.
	void				  Event_TurnTo( float angle );

	//! Turns the AI entity to face the specified position.
	void				  Event_TurnToPos( const idVec3& pos );

	//! Turns the AI to face the given entity.
	void				  Event_TurnToEntity( idEntity* ent );

	//! Returns the current move status of the AI entity.
	void				  Event_MoveStatus();

	//! Stops the AI movement and sets the move status to done.
	void				  Event_StopMove();

	//! Moves the AI actor to cover position relative to the enemy.
	void				  Event_MoveToCover();

	//! Moves the AI entity towards its enemy.
	void				  Event_MoveToEnemy();

	//! Moves the AI entity to the height of its enemy.
	void				  Event_MoveToEnemyHeight();

	//! Stops current movement and initiates moving away from the specified entity until the desired range is achieved.
	void				  Event_MoveOutOfRange( idEntity* entity, float range );

	//! Moves the AI entity to an attack position using the specified attack animation.
	void				  Event_MoveToAttackPosition( idEntity* entity, const char* attack_anim );

	//! Moves the AI entity toward the specified target entity.
	void				  Event_MoveToEntity( idEntity* ent );

	//! Moves the AI entity to the specified position.
	void				  Event_MoveToPosition( const idVec3& pos );

	//! Initiates a sliding movement to the specified position over a given time period.
	void				  Event_SlideTo( const idVec3& pos, float time );

	//! Moves the AI to a random location within the wander range.
	void				  Event_Wander();

	//! Returns whether the AI is facing its ideal angle.
	void				  Event_FacingIdeal();

	//! Makes the AI face its enemy.
	void				  Event_FaceEnemy();

	//! Makes the AI entity face the given entity.
	void				  Event_FaceEntity( idEntity* ent );

	//! Handles waiting for a specific action state in the AI system.
	void				  Event_WaitAction( const char* waitForState );

	//! Returns the combat node entity associated with this AI.
	void				  Event_GetCombatNode();

	//! Checks if the enemy is within the combat cone of a specified combat node.
	void				  Event_EnemyInCombatCone( idEntity* ent, int use_current_enemy_location );

	//! Handles the event for waiting during movement in the AI system.
	void				  Event_WaitMove();

	//! Returns the jump velocity vector needed to reach a target position with specified speed and maximum height.
	void				  Event_GetJumpVelocity( const idVec3& pos, float speed, float max_height );

	//! Returns whether the specified entity is within the AI's attack cone.
	void				  Event_EntityInAttackCone( idEntity* ent );

	//! Checks if the AI can see the specified entity.
	void				  Event_CanSeeEntity( idEntity* ent );

	//! Sets the talk target for the AI character, ensuring it is a valid actor type.
	void				  Event_SetTalkTarget( idEntity* target );

	//! Returns the entity that the AI is currently talking to.
	void				  Event_GetTalkTarget();

	//! Sets the talk state of the AI character to the specified value.
	void				  Event_SetTalkState( int state );

	//! Returns the distance to the AI's enemy entity.
	void				  Event_EnemyRange();

	//! Returns the 2D distance to the current enemy.
	void				  Event_EnemyRange2D();

	//! Returns the awake state of the AI entity.
	void				  Event_IsAwake();

	//! Returns the current enemy entity of the AI.
	void				  Event_GetEnemy();

	//! Returns the last known position of the AI's enemy.
	void				  Event_GetEnemyPos();

	//! Returns the position of the AI's enemy's eyes.
	void				  Event_GetEnemyEyePos();

	//! Returns the predicted position of the enemy at a specified time.
	void				  Event_PredictEnemyPos( float time );

	//! Checks if the AI can hit its enemy with a direct line of sight.
	void				  Event_CanHitEnemy();

	//! Checks if the AI can hit its enemy from a specified animation.
	void				  Event_CanHitEnemyFromAnim( const char* animname );

	//! Checks if the AI can hit its enemy from a specified joint
	bool				  CanHitEnemyFromJoint( const char* jointname );

	//! Checks if the AI can hit its enemy from a specified joint.
	void				  Event_CanHitEnemyFromJoint( const char* jointname );

	//! Returns whether the current enemy position is valid.
	void				  Event_EnemyPositionValid();

	//! Initiates a charge attack motion towards the enemy entity using the specified damage definition.
	void				  Event_ChargeAttack( const char* damageDef );

	//! Tests if a charge attack can be performed by the AI entity.
	void				  Event_TestChargeAttack();

	//! Executes a test animation movement toward the enemy using the specified animation name.
	void				  Event_TestAnimMoveTowardEnemy( const char* animname );

	//! Tests if the AI can move using the specified animation name.
	void				  Event_TestAnimMove( const char* animname );

	//! Tests if an AI can move to a specified position without obstacles or events.
	void				  Event_TestMoveToPosition( const idVec3& position );

	//! Tests if a melee attack can be performed and returns the result.
	void				  Event_TestMeleeAttack();

	//! Tests if an animation attack will hit the enemy.
	void				  Event_TestAnimAttack( const char* animname );

	//! This function was intended to handle the shriveling effect animation for AI entities, but is currently disabled in the BFG edition.
	void				  Event_Shrivel( float shirvel_time );

	//! Burns the AI entity by updating its visual state and spawning burn particles.
	void				  Event_Burn();

	//! Prepares the AI entity for burning by disabling grabbing and turning off shadows.
	void				  Event_PreBurn();

	//! Clears the burn effect from the AI entity.
	void				  Event_ClearBurn();

	//! Sets the visibility state of smoke particles for the AI entity.
	void				  Event_SetSmokeVisibility( int num, int on );

	//! Returns the number of smoke emitters associated with this AI entity.
	void				  Event_NumSmokeEmitters();

	//! Stops the AI from thinking and marks the current thread as done processing.
	void				  Event_StopThinking();

	//! Returns the angular difference between the ideal and current yaw angles for turning.
	void				  Event_GetTurnDelta();

	//! Returns the movement type of the AI entity.
	void				  Event_GetMoveType();

	//! Sets the movement type for the AI entity and updates the travel flags accordingly.
	void				  Event_SetMoveType( int moveType );

	//! Saves the current move state of the AI character.
	void				  Event_SaveMove();

	//! Restores the AI's movement state after a saved move command has been processed.
	void				  Event_RestoreMove();

	//! Sets the allowMove flag based on the provided float value.
	void				  Event_AllowMovement( float flag );

	//! Sets the AI_JUMP flag to true.
	void				  Event_JumpFrame();

	//! Enables clipping and gravity for the AI
	void				  Event_EnableClip();

	//! Disables clipping and gravity for the AI entity.
	void				  Event_DisableClip();

	//! Enables gravity for the AI entity.
	void				  Event_EnableGravity();

	//! Disables gravity for the AI entity.
	void				  Event_DisableGravity();

	//! Enables AI push behavior for articulated figures.
	void				  Event_EnableAFPush();

	//! Disables the AF push moveables flag.
	void				  Event_DisableAFPush();

	//! Sets the flying speed of the AI entity.
	void				  Event_SetFlySpeed( float speed );

	//! Sets the fly offset value for the AI entity.
	void				  Event_SetFlyOffset( int offset );

	//! Clears the fly offset value for the AI entity.
	void				  Event_ClearFlyOffset();

	//! Returns the closest hidden target of the specified type.
	void				  Event_GetClosestHiddenTarget( const char* type );

	//! Returns a random entity of the specified type from the AI's target list.
	void				  Event_GetRandomTarget( const char* type );

	//! Returns the travel time between the AI's current position and the specified position.
	void				  Event_TravelDistanceToPoint( const idVec3& pos );

	//! Returns the travel distance from the AI's current position to the specified entity's position.
	void				  Event_TravelDistanceToEntity( idEntity* ent );

	//! Returns the travel distance between two points as a float.
	void				  Event_TravelDistanceBetweenPoints( const idVec3& source, const idVec3& dest );

	//! Calculates and returns the travel distance between two entities.
	void				  Event_TravelDistanceBetweenEntities( idEntity* source, idEntity* dest );

	//! Makes the AI actor look at a specified entity for a given duration.
	void				  Event_LookAtEntity( idEntity* ent, float duration );

	//! Sets the AI to look at its enemy for a specified duration.
	void				  Event_LookAtEnemy( float duration );

	//! Sets the joint modification flag for the AI entity.
	void				  Event_SetJointMod( int allowJointMod );

	//! Unbinds and sets the owner of a moveable entity bound to the AI.
	void				  Event_ThrowMoveable();

	//! Unbinds and sets the owner of an articulated figure entity bound to the AI.
	void				  Event_ThrowAF();

	//! Sets the AI's yaw angle and updates the view axis accordingly.
	void				  Event_SetAngles( idAngles const& ang );

	//! Returns the current yaw angle of the AI entity.
	void				  Event_GetAngles();

	//! Returns the trajectory vector toward the player for AI movement prediction
	void				  Event_GetTrajectoryToPlayer();

	//! Completely kills the AI entity by setting health to zero and triggering death events.
	void				  Event_RealKill();

	//! Schedules the AI entity for immediate deletion.
	void				  Event_Kill();

	//! Sets whether the AI should wake up when illuminated by a flashlight.
	void				  Event_WakeOnFlashlight( int enable );

	//! Locates the enemy entity and updates its position in the AI system.
	void				  Event_LocateEnemy();

	//! Kicks obstacles away from the AI entity using a specified force.
	void				  Event_KickObstacles( idEntity* kickEnt, float force );

	//! Returns the obstacle entity that the AI is currently encountering.
	void				  Event_GetObstacle();

	//! Pushes a point into the AAS (Area Awareness System) to find a valid reachable position.
	void				  Event_PushPointIntoAAS( const idVec3& pos );

	//! Returns the AI's current turn rate as a floating-point value.
	void				  Event_GetTurnRate();

	//! Sets the turn rate of the AI entity to the specified value.
	void				  Event_SetTurnRate( float rate );

	//! Sets the angle for animation-based turning.
	void				  Event_AnimTurn( float angles );

	//! Sets whether hidden movement is allowed for the AI.
	void				  Event_AllowHiddenMovement( int enable );

	//! Triggers particles at the specified joint name.
	void				  Event_TriggerParticles( const char* jointName );

	//! Finds the first actor entity within the specified bounding box, excluding the AI entity itself.
	void				  Event_FindActorsInBounds( const idVec3& mins, const idVec3& maxs );

	//! Checks if the AI can reach a specified position.
	void				  Event_CanReachPosition( const idVec3& pos );

	//! Returns whether the AI can reach the specified entity.
	void				  Event_CanReachEntity( idEntity* ent );

	//! Returns whether the AI can reach its enemy.
	void				  Event_CanReachEnemy();

	//! Returns the reachable position of the specified entity.
	void				  Event_GetReachableEntityPosition( idEntity* ent );

	//! Moves the AI directly to the specified position.
	void				  Event_MoveToPositionDirect( const idVec3& pos );

	//! Sets the obstacle avoidance behavior based on the ignore parameter.
	void				  Event_AvoidObstacles( int ignore );

	//! Triggers a effects animation on the specified joint of the AI entity.
	void				  Event_TriggerFX( const char* joint, const char* fx );

	//! Starts a particle emitter with the specified name, joint, and particle type.
	void				  Event_StartEmitter( const char* name, const char* joint, const char* particle );

	//! Returns the emitter entity with the specified name.
	void				  Event_GetEmitter( const char* name );

	//! Stops the sound emitter with the specified name.
	void				  Event_StopEmitter( const char* name );

	// jmarshall begin
private:
	// These are used by the lost combat state.
	float		 allow_attack;
	float		 lost_time;
	idEntity*	 lost_combat_node;

	float		 attack_flags;

	bool		 supportsNative;

	idStr		 lastStateName;
	stateParms_t storedState;
	// jmarshall end
};

/*!
	\class idCombatNode
	\brief A combat node that defines a strategic position for AI actors during combat.

	The idCombatNode class represents a strategic location in the game world that can be used by AI actors during combat scenarios. It maintains state information about whether the node is active or
   disabled, and provides functionality to determine if an actor is within its view cone. The class supports persistence through save and restore operations, and can be toggled between active and
   inactive states via events. Debug visualization capabilities are included to assist in development and testing. Combat nodes are typically spawned during level initialization and configured through
   spawn arguments to define their behavior and constraints.

*/
class idCombatNode : public idEntity
{
public:
	CLASS_PROTOTYPE( idCombatNode );

	//! Constructs a new idCombatNode instance with default values.
	idCombatNode();

	//! Saves the combat node data to a save file
	void		Save( idSaveGame* savefile ) const;

	//! Restores the combat node state from a save file.
	void		Restore( idRestoreGame* savefile );

	//! Initializes the combat node properties based on spawn arguments.
	void		Spawn();

	//! Returns true if the combat node is disabled.
	bool		IsDisabled() const;

	//! Checks if the specified actor is within the view cone and bounds of the combat node
	bool		EntityInView( idActor* actor, const idVec3& pos );

	//! Draws debug visualization for combat nodes in the game world
	static void DrawDebugInfo();

private:
	float  min_dist;
	float  max_dist;
	float  cone_dist;
	float  min_height;
	float  max_height;
	idVec3 cone_left;
	idVec3 cone_right;
	idVec3 offset;
	bool   disabled;

	//! Toggles the disabled state of the combat node.
	void   Event_Activate( idEntity* activator );

	//! Marks the combat node as used and potentially disables it if it's set to be used only once.
	void   Event_MarkUsed();
};

/*!
	\class iceAI_Follower
	\brief A follower AI class that manages an entity's behavior in relation to a leader.

	This class implements a follower AI that tracks a leader entity and manages its animations and states. The AI handles various states including idle, following, getting closer to the leader, and
   talking animations. It inherits from idAI and overrides initialization and state handling methods to provide specific follower behavior. The implementation manages transitions between states based
   on the leader's position and the AI's configuration.

*/
class iceAI_Follower : public idAI
{
public:
	CLASS_PROTOTYPE( iceAI_Follower );

	//! Initializes the follower AI by setting up animations, talk state, and idle states.
	virtual void Init() override;

private:
	bool	  inCustomAnim;
	idEntity* leader;

private:
	//! Moves the follower to the idle state and returns that the state is done.
	stateResult_t state_idle( stateParms_t* parms );

	//! Handles the idle frame logic for the follower AI, potentially transitioning to a talking animation if AI_TALK is enabled.
	stateResult_t state_idle_frame( stateParms_t* parms );

	//! Handles the follow state for the ice AI follower entity.
	stateResult_t state_follow( stateParms_t* parms );

	//! Updates the follower AI state based on the leader's position and talk status.
	stateResult_t state_follow_frame( stateParms_t* parms );

	//! Moves the entity closer to the leader within specified distance thresholds.
	stateResult_t state_get_closer( stateParms_t* parms );

	//! Handles the killed state for the ice AI follower.
	stateResult_t state_killed( stateParms_t* parms );

	//! Handles the talking animation state for the follower AI, transitioning to the follow state when animation completes.
	stateResult_t state_talk_anim( stateParms_t* parms );
};

//
// Bosses
//
#include "../monsters/Monster_boss_vagary.h"

//
// Demons
//
#include "../monsters/Monster_demon_hellknight.h"
#include "../monsters/Monster_demon_imp.h"

//
// Flying Monsters
//
#include "../monsters/Monster_flying_lostsoul.h"
#include "../monsters/Monster_flying_cacodemon.h"

//
// Zombie Monsters
//
#include "../monsters/Monster_zombie.h"
#include "../monsters/Monster_zombie_sawyer.h"
#include "../monsters/Monster_zombie_bernie.h"
#include "../monsters/Monster_zombie_morgue.h"
#include "../monsters/Monster_zombie_security_pistol.h"
#include "../monsters/Monster_zombie_commando_tentacle.h"
#include "../monsters/monster_zombie_commando_cgun.h"
#include "../monsters/Monster_turret.h"

#include "../bots/Bot.h"

#endif /* !__AI_H__ */
