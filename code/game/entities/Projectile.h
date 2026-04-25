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

#ifndef __GAME_PROJECTILE_H__
#define __GAME_PROJECTILE_H__

/*
===============================================================================

  idProjectile

===============================================================================
*/

extern const idEventDef EV_Explode;

/*!
	\class idProjectile
	\brief Projectile entity class that manages physics, collisions, and behaviors for thrown objects.

	The idProjectile class represents a throwable object with physical properties and behaviors such as launching, colliding, exploding, or fizzling. It inherits from idEntity and handles its own
   physics simulation, client-server synchronization, and visual effects. The class supports various projectile types with different damage, gravity, and behavior characteristics based on spawn
   arguments. It manages its lifecycle through states like launched, exploded, or fizzled, and provides methods for creating, launching, and handling collisions. The class also includes client-side
   prediction and network synchronization capabilities for multiplayer environments.

*/
class idProjectile : public idEntity
{
public:
	CLASS_PROTOTYPE( idProjectile );

	//! Initializes a new instance of the idProjectile class.
	idProjectile();

	//! Destructor for the idProjectile class that stops sound and frees the light definition.
	virtual ~idProjectile();

	//! Initializes the projectile's physics and properties.
	void		  Spawn();

	//! Saves the projectile state to a save game file.
	void		  Save( idSaveGame* savefile ) const;

	//! Restores the projectile's state from a save file.
	void		  Restore( idRestoreGame* savefile );

	//! Initializes a projectile with the specified owner, start position, and direction.
	void		  Create( idEntity* owner, const idVec3& start, const idVec3& dir );

	/*!
		\brief Initializes and launches a projectile with specified start position, direction, and various physical properties.

		Configures the projectile's physical properties including mass, friction, gravity, and bounciness based on spawn arguments. Sets up the projectile's initial velocity and angular velocity,
	   binds it to the owner if applicable, and determines whether it should detonate on world impact, actor impact, or after a fuse time. Handles special cases for different projectile types such as
	   the helltime killer. Sets up tracer models and sound effects based on spawn arguments. The function also manages the projectile's lifecycle including removal timing and event posting for
	   detonation or fizzling. It updates the visual representation and sets the projectile state to launched.

		\param start The starting position of the projectile
		\param dir The direction the projectile is launched
		\param pushVelocity Additional velocity to be applied to the projectile
		\param timeSinceFire Time elapsed since the projectile was fired, used for fuse calculations
		\param launchPower Multiplier for the launch speed
		\param dmgPower Multiplier for the damage power
	*/
	virtual void  Launch( const idVec3& start, const idVec3& dir, const idVec3& pushVelocity, const float timeSinceFire = 0.0f, const float launchPower = 1.0f, const float dmgPower = 1.0f );

	//! Frees the light definition handle for the projectile if it is valid
	virtual void  FreeLightDef();

	//! Returns the owner entity of this projectile.
	idEntity*	  GetOwner() const;

	//! Sets the owner of the projectile and updates its physics object and damage definition when caught by a new owner.
	void		  CatchProjectile( idEntity* o, const char* reflectName );

	//! Returns the current state of the projectile as an integer.
	int			  GetProjectileState();

	//! Creates a projectile with the specified owner, start position, and direction.
	void		  Event_CreateProjectile( idEntity* owner, const idVec3& start, const idVec3& dir );

	//! Launches the projectile with the specified start position, direction, and push velocity.
	void		  Event_LaunchProjectile( const idVec3& start, const idVec3& dir, const idVec3& pushVelocity );

	//! Sets the gravity for the projectile based on the provided gravity value.
	void		  Event_SetGravity( float gravity );

	//! Executes the projectile's thinking logic, including physics updates and trigger interactions.
	virtual void  Think();

	/*!
		\brief Handles the projectile's behavior when it is killed, either detonating or fizzling based on spawn arguments.

		This function is called when the projectile entity is killed. It checks if the projectile should detonate upon death based on the spawn argument 'detonate_on_death'. If true, it performs a
	   trace to determine the collision point and normal, then triggers an explosion at that location. It also clears contacts and puts the physics object to rest after detonation. If the projectile
	   is not set to detonate on death, it fizzles instead.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage inflicted
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
	*/
	virtual void  Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Handles collision events for a projectile, determining its behavior upon impact with surfaces or entities.
	virtual bool  Collide( const trace_t& collision, const idVec3& velocity );

	//! Handles the explosion effect of a projectile upon collision, including visual effects, sound, and damage calculation.
	virtual void  Explode( const trace_t& collision, idEntity* ignore );

	//! Causes the projectile to fizzle and removes it from the game
	void		  Fizzle();

	//! Returns the velocity vector of a projectile specified by the given dictionary.
	static idVec3 GetVelocity( const idDict* projectile );

	//! Returns the gravity vector for a projectile based on the gravity value specified in the projectile dictionary.
	static idVec3 GetGravity( const idDict* projectile );

	enum { EVENT_DAMAGE_EFFECT = idEntity::EVENT_MAXEVENTS, EVENT_MAXEVENTS };

	//! Sets whether the projectile was launched from a grabber.
	void		 SetLaunchedFromGrabber( bool bl ) { launchedFromGrabber = bl; }

	//! Returns whether the projectile was launched from a grabber.
	bool		 GetLaunchedFromGrabber() { return launchedFromGrabber; }

	/*!
		\brief Handles the default damage effect for a projectile upon collision, including playing sounds and projecting decals based on the material type.

		This function processes the impact of a projectile by determining the material type of the surface it collided with. It then retrieves appropriate sound and decal definitions from the
	   projectile definition based on the material type. If specific sounds or decals are not defined for the material, it falls back to default options. The function plays the impact sound through
	   the specified sound entity and projects a decal at the collision point.

		\param soundEnt Entity to play the impact sound on
		\param projectileDef Definition dictionary containing projectile parameters including sound and decal settings
		\param collision Collision information including the material and point of impact
		\param velocity Velocity vector of the projectile at the time of impact
	*/
	static void	 DefaultDamageEffect( idEntity* soundEnt, const idDict& projectileDef, const trace_t& collision, const idVec3& velocity );

	/*!
		\brief Determines whether a projectile should collide with an entity based on surface flags, entity type, and projectile definition settings.

		This function evaluates if a projectile should trigger a collision event when hitting an entity. It first checks if the surface has the noimpact flag and returns false if present. It then
	   verifies the entity exists and is not a noclip player. The function further checks if the entity is an actor or attachment to an actor and evaluates the 'detonate_on_actor' property of the
	   projectile definition. If the entity is not an actor, it checks the 'detonate_on_world' property. If all conditions are met and the projectile should cause a damage effect, it predicts the
	   damage effect on the entity.

		\param soundEnt Entity that plays the sound effect
		\param projectileDef Definition of the projectile including detonation properties
		\param collision Collision data including material and entity number
		\param velocity Velocity vector of the projectile
		\param addDamageEffect Flag indicating if damage effect should be added
		\return True if the projectile should collide and detonate, false otherwise
	*/
	static bool	 ClientPredictionCollide( idEntity* soundEnt, const idDict& projectileDef, const trace_t& collision, const idVec3& velocity, bool addDamageEffect );

	//! Performs client-side prediction thinking for the projectile.
	virtual void ClientPredictionThink();

	//! Updates the projectile's client-side state including physics interpolation, rendering, and effects.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Writes the projectile's state and physics information to a bit message for network synchronization.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Restores the projectile's state from a network snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

	//! Handles client-side events for the projectile, specifically damage effects.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Queues this projectile to be simulated on the server at the specified start time.
	void		 QueueToSimulate( int startTime );

	//! Advances the projectile simulation by the specified time interval.
	virtual void SimulateProjectileFrame( int msec, int endTime );

	//! Updates projectile state after simulation based on explosion or fizzled conditions.
	virtual void PostSimulate( int endTime );

	struct simulatedProjectile_t {
		//! Initializes a simulated projectile with null projectile pointer and zero start time.
		simulatedProjectile_t() :
			projectile( NULL ),
			startTime( 0 )
		{
		}
		idProjectile* projectile;
		int			  startTime;
	};

	static const int												 MAX_SIMULATED_PROJECTILES = 64;

	// This list is used to "catch up" client projectiles to the current time on the server
	static idArray<simulatedProjectile_t, MAX_SIMULATED_PROJECTILES> projectilesToSimulate;

protected:
	idEntityPtr<idEntity> owner;

	struct projectileFlags_s {
		bool detonate_on_world : 1;
		bool detonate_on_actor : 1;
		bool randomShaderSpin  : 1;
		bool isTracer		   : 1;
		bool noSplashDamage	   : 1;
	} projectileFlags;

	bool				  launchedFromGrabber;

	float				  thrust;
	int					  thrust_end;
	float				  damagePower;

	renderLight_t		  renderLight;
	qhandle_t			  lightDefHandle; // handle to renderer light def
	idVec3				  lightOffset;
	int					  lightStartTime;
	int					  lightEndTime;
	idVec3				  lightColor;

	idForce_Constant	  thruster;
	idPhysics_RigidBody	  physicsObj;

	const idDeclParticle* smokeFly;
	int					  smokeFlyTime;
	bool				  mNoExplodeDisappear;
	bool				  mTouchTriggers;

	int					  originalTimeGroup;

	typedef enum {
		// must update these in script/doom_defs.script if changed
		SPAWNED	 = 0,
		CREATED	 = 1,
		LAUNCHED = 2,
		FIZZLED	 = 3,
		EXPLODED = 4
	} projectileState_t;

	projectileState_t state;

private:
	idVec3 launchOrigin;
	idMat3 launchAxis;

	//! Adds the default damage effect for a projectile impact.
	void   AddDefaultDamageEffect( const trace_t& collision, const idVec3& velocity );

	//! Adds particles and light effects to the projectile.
	void   AddParticlesAndLight();

	//! Handles the explosion event for a projectile by calculating collision data and triggering the explosion effect.
	void   Event_Explode();

	//! Handles the projectile fizzle event.
	void   Event_Fizzle();

	//! Executes radius damage using the projectile's splash damage definition while ignoring the specified entity.
	void   Event_RadiusDamage( idEntity* ignore );

	//! Handles the projectile touch event, triggering explosion when colliding with entities other than the owner.
	void   Event_Touch( idEntity* other, trace_t* trace );

	//! Returns the current state of the projectile.
	void   Event_GetProjectileState();
};

/*!
	\class idGuidedProjectile
	\brief A guided projectile class that tracks and pursues enemy targets with adjustable parameters.

	The idGuidedProjectile class extends the basic projectile functionality to include guided movement toward a specified enemy target. It handles projectile initialization, trajectory calculation,
   and targeting logic based on the owner entity type. The class supports different targeting strategies for AI and player entities, with fallback behavior when no valid target is found. The
   projectile's movement is updated each frame through the Think method, which adjusts orientation and velocity to pursue the target. Launch parameters can be configured with additional velocity,
   timing, and power multipliers to adjust behavior. The class implements save and restore functionality to maintain state during game sessions. Memory management is handled through the base class and
   standard C++ practices.

*/
class idGuidedProjectile : public idProjectile
{
public:
	CLASS_PROTOTYPE( idGuidedProjectile );

	//! Initializes a new instance of the idGuidedProjectile class with default values.
	idGuidedProjectile();
	~idGuidedProjectile();

	//! Saves the guided projectile state to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the guided projectile's state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the guided projectile behavior.
	void		 Spawn();

	//! Updates the guided projectile's orientation and velocity based on the target position.
	virtual void Think();

	/*!
		\brief Initializes the guided projectile's trajectory and target based on the owner's entity type and environment.

		The function sets up the projectile's launch parameters by calling the base class implementation and then determining the target enemy based on the owner entity. If the owner is an AI, it
	   assigns the AI's enemy. If the owner is a player, it traces a line from the player's eye position to find an enemy, or defaults to the enemy with the most health if no valid target is found.
	   The function also initializes various projectile properties such as angles, speed, turn rate, and other configuration values.

		\param start The starting position of the projectile
		\param dir The initial direction of the projectile
		\param pushVelocity Additional velocity to apply to the projectile at launch
		\param timeSinceFire Time elapsed since the weapon was fired, used for timing calculations
		\param launchPower Multiplies the launch power of the projectile
		\param dmgPower Multiplies the damage power of the projectile
	*/
	virtual void Launch( const idVec3& start, const idVec3& dir, const idVec3& pushVelocity, const float timeSinceFire = 0.0f, const float launchPower = 1.0f, const float dmgPower = 1.0f );

	//! Sets the enemy entity that the guided projectile will target.
	void		 SetEnemy( idEntity* ent );

	//! Sets the enemy target for the guided projectile.
	void		 Event_SetEnemy( idEntity* ent );

protected:
	float				  speed;
	idEntityPtr<idEntity> enemy;

	//! Calculates and returns the seek position for the guided projectile based on the target enemy entity.
	virtual void		  GetSeekPos( idVec3& out );

private:
	idAngles rndScale;
	idAngles rndAng;
	idAngles angles;
	int		 rndUpdateTime;
	float	 turn_max;
	float	 clamp_dist;
	bool	 burstMode;
	bool	 unGuided;
	float	 burstDist;
	float	 burstVelocity;
};

/*!
	\class idSoulCubeMissile
	\brief A specialized guided projectile implementing soul cube missile behavior with return and detonation logic.

	This class implements a specialized guided projectile that follows a predefined path and can return to its owner. The missile is initialized with specific launch parameters and follows a
   trajectory determined by spawn arguments. During its flight, it updates its state and behavior through the Think method, and can be configured to target and damage specific entities. The missile
   supports serialization for save/load functionality and can register itself with its owner for tracking purposes. The ReturnToOwner method initiates a return phase to the missile's owner, while
   KillTarget handles the detonation and damage logic. The missile is designed to not collide with objects and relies on a separate think routine for detonation timing, making it suitable for specific
   weapon behaviors.

*/
class idSoulCubeMissile : public idGuidedProjectile
{
public:
	CLASS_PROTOTYPE( idSoulCubeMissile );
	~idSoulCubeMissile();

	//! Serializes the soul cube missile's state to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the soul cube missile from a save file
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the soul cube missile properties to their default values.
	void		 Spawn();

	//! Updates the soul cube missile's state and behavior during the game loop.
	virtual void Think();

	/*!
		\brief Initializes and launches a soul cube missile with specified start position, direction, and velocity parameters

		Configures the missile's initial trajectory and physics properties based on input parameters and spawn arguments. The missile is launched with an offset from the start position and follows a
	   predetermined path determined by spawn parameters. The function sets up the missile's velocity acceleration over time and updates its visual representation. It also handles special behavior
	   when targeting an enemy actor, setting a destination point. The missile is configured to not collide with objects and relies on a separate think routine for detonation timing. Additionally, if
	   the missile's owner is a player, it registers itself with the player for tracking purposes.

		\param start The starting position of the missile
		\param dir The direction the missile should travel
		\param pushVelocity Additional velocity to apply to the missile upon launch
		\param timeSinceFire Time elapsed since the weapon was fired
		\param power Launch power multiplier affecting the missile's trajectory
		\param dmgPower Damage power multiplier affecting the missile's damage
	*/
	virtual void Launch( const idVec3& start, const idVec3& dir, const idVec3& pushVelocity, const float timeSinceFire = 0.0f, const float power = 1.0f, const float dmgPower = 1.0f );

protected:
	//! Sets the output vector to the seek position for the soul cube missile, prioritizing the owner's eye position during return phase or a destination organization.
	virtual void GetSeekPos( idVec3& out );

	//! Initiates the return phase of the soul cube missile towards its owner.
	void		 ReturnToOwner();

	//! Kills the target entity and applies damage and effects based on the missile's configuration.
	void		 KillTarget( const idVec3& dir );

private:
	idVec3				  startingVelocity;
	idVec3				  endingVelocity;
	float				  accelTime;
	int					  launchTime;
	bool				  killPhase;
	bool				  returnPhase;
	idVec3				  destOrg;
	idVec3				  orbitOrg;
	int					  orbitTime;
	int					  smokeKillTime;
	const idDeclParticle* smokeKill;
};

struct beamTarget_t {
	idEntityPtr<idEntity> target;
	renderEntity_t		  renderEntity;
	qhandle_t			  modelDefHandle;
};

/*!
	\class idBFGProjectile
	\brief Represents a specialized projectile implementation for a BFG-style weapon with explosion and beam effects.

	This class extends the base projectile functionality to implement a BFG-style weapon projectile that features beam visualization, damage calculation, and explosion effects. The projectile tracks
   entities within its damage radius and creates visual beam targets for them. It handles the launch process by setting up initial state, determining targets, and configuring visual effects. The
   explosion event applies damage to tracked entities and manages beam effects. The class supports serialization for save game compatibility and includes cleanup functionality for beam resources. The
   projectile's behavior includes special handling for end boss targets to ensure proper visualization of the beam effects.

*/
class idBFGProjectile : public idProjectile
{
public:
	CLASS_PROTOTYPE( idBFGProjectile );

	//! Initializes a new instance of the idBFGProjectile class.
	idBFGProjectile();

	//! Destroys an idBFGProjectile instance and releases associated resources.
	~idBFGProjectile();

	//! Serializes the BFG projectile state to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the BFG projectile from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the BFG projectile properties and sets up the secondary model if specified.
	void		 Spawn();

	//! Updates beam targets and handles damage logic during the projectile's active state
	virtual void Think();

	/*!
		\brief Initializes and launches a BFG projectile with specified start position, direction, and velocity parameters.

		This function sets up the projectile's initial state by calling the base class Launch method with the provided parameters. It then determines which entities are within the projectile's damage
	   radius and creates visual beam targets for them. The function also handles special cases for the end boss, ensuring the beam is properly visualized. The damage is not applied until the
	   projectile explodes. The function configures rendering properties for the projectile's second model if one is specified and sets up beam targets for visual effects, ensuring they are properly
	   attached to entities that can take damage.

		\param start starting position of the projectile
		\param dir direction in which the projectile is launched
		\param pushVelocity additional velocity to be applied to the projectile
		\param timeSinceFire time elapsed since the projectile was fired
		\param launchPower power multiplier for the projectile's launch force
		\param dmgPower power multiplier for the projectile's damage
	*/
	virtual void Launch( const idVec3& start, const idVec3& dir, const idVec3& pushVelocity, const float timeSinceFire = 0.0f, const float launchPower = 1.0f, const float dmgPower = 1.0f );

	//! Handles the explosion event of the BFG projectile, damaging targets and managing beam effects.
	virtual void Explode( const trace_t& collision, idEntity* ignore );

private:
	idList<beamTarget_t, TAG_PROJECTILE> beamTargets;
	renderEntity_t						 secondModel;
	qhandle_t							 secondModelDefHandle;
	int									 nextDamageTime;
	idStr								 damageFreq;

	//! Releases all beam targets and disables BFG vision for the local player.
	void								 FreeBeams();

	//! Removes the beams associated with the BFG projectile.
	void								 Event_RemoveBeams();
};

/*!
	\class idHomingProjectile
	\brief A homing projectile entity that tracks and seeks a target.

	The idHomingProjectile class extends the base projectile functionality to provide homing behavior, allowing the projectile to track and seek a target entity or position. The class manages the
   projectile's physics, target acquisition, and movement updates during its flight. It supports initialization through a launch function that sets the projectile's starting position, direction, and
   velocity, while also determining the initial target based on the owner entity. The projectile's behavior includes speed control, turn rate limiting, and orientation updates as it seeks its target.
   The class provides persistence mechanisms through save and restore functions to maintain the projectile's state during game loading and saving operations. Target tracking can be overridden manually
   using set functions, allowing for custom behavior or specific targeting scenarios. The Think method handles the real-time update of the projectile's path and orientation as it homes in on its
   target, performing necessary calculations to adjust its trajectory and maintain tracking.

*/
class idHomingProjectile : public idProjectile
{
public:
	CLASS_PROTOTYPE( idHomingProjectile );

	//! Initializes a new instance of the idHomingProjectile class with default values.
	idHomingProjectile();
	~idHomingProjectile();

	//! Saves the homing projectile state to a save game file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the homing projectile state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the homing projectile entity.
	void		 Spawn();

	//! Updates the homing projectile's position and orientation based on the target seek position
	virtual void Think();

	/*!
		\brief Initializes and launches a homing projectile with specified start position, direction, and velocity parameters.

		The Launch function sets up a homing projectile by initializing its physics properties and determining the initial target. It first calls the base class Launch method to set up basic
	   projectile behavior. Then, it determines the enemy target based on the owner entity type. If the owner is an AI, it uses the AI's current enemy. If the owner is a player, it performs a trace to
	   find the closest visible enemy in the direction the player is facing, and if none is found or the found entity is not a valid enemy, it defaults to the enemy with the most health. The function
	   also sets up homing parameters including speed, angles, turn rate, and other physics properties for the projectile's behavior.

		\param start The initial position of the projectile
		\param dir The initial direction vector for the projectile
		\param pushVelocity Additional velocity to apply to the projectile at launch
		\param timeSinceFire Time elapsed since the weapon was fired, used for timing adjustments
		\param launchPower Multiplier for the initial launch force
		\param dmgPower Multiplier for the damage calculation
	*/
	virtual void Launch( const idVec3& start, const idVec3& dir, const idVec3& pushVelocity, const float timeSinceFire = 0.0f, const float launchPower = 1.0f, const float dmgPower = 1.0f );

	//! Sets the enemy entity that the homing projectile will track.
	void		 SetEnemy( idEntity* ent );

	//! Sets the target position for the homing projectile to seek towards.
	void		 SetSeekPos( idVec3 pos );

	//! Sets the enemy entity for the homing projectile.
	void		 Event_SetEnemy( idEntity* ent );

protected:
	float				  speed;
	idEntityPtr<idEntity> enemy;
	idVec3				  seekPos;

private:
	idAngles rndScale;
	idAngles rndAng;
	idAngles angles;
	float	 turn_max;
	float	 clamp_dist;
	bool	 burstMode;
	bool	 unGuided;
	float	 burstDist;
	float	 burstVelocity;
};

/*!
	\class idDebris
	\brief Manages debris entities that can be created, launched, and destroyed with explosion or fizzle effects.

	The idDebris class represents entities that are generated as a result of destruction events, typically used for visual effects such as flying debris. It inherits from idEntity and provides
   functionality for initialization, launching, collision handling, and destruction through either explosion or fizzle effects. The class supports serialization through Save and Restore methods, and
   can be spawned with default settings or initialized with a specific owner, position, and orientation. The Launch method sets up the physics properties for the debris, while Think handles the
   ongoing behavior. Destruction is managed by Killed, which determines whether to explode or fizzle based on spawn arguments. The Collide method provides basic collision response by playing a bounce
   sound and returning false to indicate no further collision processing is needed. Event methods are provided to explicitly trigger explosion or fizzle behavior.

*/
class idDebris : public idEntity
{
public:
	CLASS_PROTOTYPE( idDebris );

	//! Constructs a new idDebris object with default initialization.
	idDebris();
	~idDebris();

	//! Saves the debris object to a save game file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the debris object from a saved game file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the debris object with default values.
	void		 Spawn();

	//! Initializes the debris object with the specified owner, position, and orientation.
	void		 Create( idEntity* owner, const idVec3& start, const idMat3& axis );

	//! Initializes and launches debris with specified physics properties and behavior.
	void		 Launch();

	//! Executes the debris think logic, updating its physics and presentation state.
	void		 Think();

	/*!
		\brief Handles the destruction of a debris entity, either exploding or fizzling based on spawn arguments.

		This function is called when the debris entity is killed. It checks the spawn arguments to determine whether the debris should detonate upon death or simply fizzle. If the "detonate_on_death"
	   spawn argument is true, the debris will explode using the Explode() method. Otherwise, it will fizz using the Fizzle() method.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage inflicted
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
	*/
	void		 Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Explosion event for debris object
	void		 Explode();

	//! Makes the debris disappear with a fizzle effect
	void		 Fizzle();

	//! Handles collision for debris by playing a bounce sound and returning false.
	virtual bool Collide( const trace_t& collision, const idVec3& velocity );

private:
	idEntityPtr<idEntity> owner;
	idPhysics_RigidBody	  physicsObj;
	const idDeclParticle* smokeFly;
	int					  smokeFlyTime;
	const idSoundShader*  sndBounce;

	//! Triggers the debris to explode.
	void				  Event_Explode();

	//! Causes the debris to fizzle out, typically removing it from the game world.
	void				  Event_Fizzle();
};

#endif /* !__GAME_PROJECTILE_H__ */
