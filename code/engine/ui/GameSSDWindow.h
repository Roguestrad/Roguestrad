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
#ifndef __GAME_SSD_WINDOW_H__
#define __GAME_SSD_WINDOW_H__

class idGameSSDWindow;

/*!
	\class SSDCrossHair
	\brief Manages crosshair rendering and state persistence for a user interface element.
*/
class SSDCrossHair
{
public:
	enum { CROSSHAIR_STANDARD = 0, CROSSHAIR_SUPER, CROSSHAIR_COUNT };
	const idMaterial* crosshairMaterial[CROSSHAIR_COUNT];
	int				  currentCrosshair;
	float			  crosshairWidth, crosshairHeight;

public:
	//! Constructor for the SSDCrossHair class.
	SSDCrossHair();
	~SSDCrossHair();

	//! Writes the crosshair state to a save file
	virtual void WriteToSaveGame( idFile* savefile );

	//! Restores the crosshair state from a save game file
	virtual void ReadFromSaveGame( idFile* savefile );

	//! Initializes the crosshairs for the SSD crosshair system.
	void		 InitCrosshairs();

	//! Draws the crosshair at the specified cursor position using the current crosshair material.
	void		 Draw( const idVec2& cursor );
};

enum { SSD_ENTITY_BASE = 0, SSD_ENTITY_ASTEROID, SSD_ENTITY_ASTRONAUT, SSD_ENTITY_EXPLOSION, SSD_ENTITY_POINTS, SSD_ENTITY_PROJECTILE, SSD_ENTITY_POWERUP };

/*!
	\class SSDEntity
	\brief Handles entity management and rendering for a 2D sound system.

	The SSDEntity class represents a visual and interactive entity within a 2D sound system environment. It manages the entity's properties such as position, size, rotation, and material, and provides
   functionality for initialization, updates, rendering, and interaction handling. The class supports serialization through save game functionality and includes methods for coordinate transformations
   between world and screen spaces. The entity can be configured with specific properties like radius and hit factor for collision detection, and it maintains a reference to its associated game
   instance. The class is designed to be extended with virtual methods for custom behavior on hit and strike events.

*/
class SSDEntity
{
public:
	// SSDEntity Information
	int				  type;
	int				  id;
	idStr			  materialName;
	const idMaterial* material;
	idVec3			  position;
	idVec2			  size;
	float			  radius;
	float			  hitRadius;
	float			  rotation;

	idVec4			  matColor;

	idStr			  text;
	float			  textScale;
	idVec4			  foreColor;

	idGameSSDWindow*  game;
	int				  currentTime;
	int				  lastUpdate;
	int				  elapsed;

	bool			  destroyed;
	bool			  noHit;
	bool			  noPlayerDamage;

	bool			  inUse;

public:
	//! Initializes a new instance of the SSDEntity class.
	SSDEntity();
	virtual ~SSDEntity();

	//! Writes the entity's state to a save game file
	virtual void WriteToSaveGame( idFile* savefile );

	//! Restores the entity's state from a save game file.
	virtual void ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	//! Initializes the SSD entity properties to their default values.
	void		 EntityInit();

	//! Sets the game instance for the SSDEntity.
	void		 SetGame( idGameSSDWindow* _game );

	//! Sets the material for the SDDS entity using the provided material name.
	void		 SetMaterial( const char* _name );

	//! Sets the position of the sound voice to the specified 3D point.
	void		 SetPosition( const idVec3& _position );

	//! Sets the size of the entity to the specified idVec2 value
	void		 SetSize( const idVec2& _size );

	//! Sets the radius and hit radius of the entity based on the provided radius and hit factor.
	void		 SetRadius( float _radius, float _hitFactor = 1.0f );

	//! Sets the rotation of the SSG entity to the specified float value.
	void		 SetRotation( float _rotation );

	//! Updates the entity's state based on elapsed time
	void		 Update();

	//! Tests if a given point intersects with the entity's hit area.
	bool		 HitTest( const idVec2& pt );

	virtual void EntityUpdate() {};

	//! Draws the entity on screen using the specified material and text settings.
	virtual void Draw();

	//! Marks the entity as no longer in use.
	virtual void DestroyEntity();

	virtual void OnHit( int key ) {};
	virtual void OnStrikePlayer() {};

	//! Transforms a world-space bounding box into screen-space coordinates
	idBounds	 WorldToScreen( const idBounds worldBounds );

	//! Transforms a world space position to screen space coordinates
	idVec3		 WorldToScreen( const idVec3& worldPos );

	//! Converts a screen-space position to world-space coordinates
	idVec3		 ScreenToWorld( const idVec3& screenPos );
};

/*!
	\class SSDMover
	\brief Manages the movement and state of entities with configurable speed and rotation parameters.
*/
class SSDMover : public SSDEntity
{
public:
	idVec3 speed;
	float  rotationSpeed;

public:
	//! Initializes a new instance of the SSDMover class.
	SSDMover();
	virtual ~SSDMover();

	//! Writes the SSDMover's state to a save game file
	virtual void WriteToSaveGame( idFile* savefile );

	//! Loads the SSD mover's state from a save game file
	virtual void ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	//! Initializes the mover with the specified speed and rotation speed.
	void		 MoverInit( const idVec3& _speed, float _rotationSpeed );

	//! Updates the entity position and rotation based on elapsed time and speed parameters.
	virtual void EntityUpdate();
};

/*
*****************************************************************************
* SSDAsteroid
****************************************************************************
*/

#define MAX_ASTEROIDS 64

/*!
	\class SSDAsteroid
	\brief Manages asteroid entities in a game, handling their initialization, update, and persistence.

	This class implements the behavior and management of asteroid entities within a game environment. It inherits from SSDMover, indicating it participates in the game's movable object system. The
   class supports initialization with various parameters such as position, size, speed, rotation, and health, allowing for flexible asteroid creation. It provides functionality for updating asteroid
   states and managing a pool of asteroids to optimize resource usage. The class also handles serialization of asteroid data for save game functionality, enabling persistence of asteroid states across
   game sessions. The implementation includes methods for retrieving asteroids from a pool, both generically and by specific ID, supporting efficient reuse of asteroid objects. The design emphasizes
   encapsulation of asteroid-specific behavior while leveraging the base class for movement and positioning logic.

*/
class SSDAsteroid : public SSDMover
{
public:
	int health;

public:
	//! Constructs a new SSDAsteroid object.
	SSDAsteroid();
	~SSDAsteroid();

	//! Writes the asteroid's health data to the specified save game file
	virtual void		WriteToSaveGame( idFile* savefile );

	//! Reads asteroid data from a save game file
	virtual void		ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	/*!
		\brief Initializes an asteroid entity with specified properties and game context

		This function sets up the asteroid entity by initializing its base properties, configuring its movement trajectory, associating it with a game window, and setting its visual and physical
	   characteristics. The asteroid is initialized with a starting position, size, speed, rotation, and health points. It also assigns a material and calculates the asteroid's radius based on its
	   dimensions.

		\param _game Pointer to the game window instance this asteroid belongs to
		\param startPosition Initial 3D position of the asteroid
		\param _size 2D dimensions of the asteroid surface
		\param _speed Vertical speed of the asteroid's movement
		\param rotate Rotation angle for the asteroid
		\param _health Initial health points of the asteroid
	*/
	void				Init( idGameSSDWindow* _game, const idVec3& startPosition, const idVec2& _size, float _speed, float rotate, int _health );

	//! Updates the asteroid entity state.
	virtual void		EntityUpdate();

	/*!
		\brief Returns a new asteroid instance from the asteroid pool with specified properties

		This function retrieves an available asteroid from a pre-allocated pool of asteroids. It iterates through the pool to find an unused asteroid, initializes it with the provided parameters,
	   marks it as in use, and returns a pointer to the initialized asteroid. If no unused asteroids are available, it returns NULL.

		\param _game Pointer to the game window instance
		\param startPosition Initial 3D position of the asteroid
		\param _size 2D size of the asteroid
		\param _speed Speed of the asteroid movement
		\param rotate Rotation angle of the asteroid
		\param _health Health points of the asteroid
		\return Pointer to the newly initialized asteroid instance, or NULL if no asteroids are available in the pool
	*/
	static SSDAsteroid* GetNewAsteroid( idGameSSDWindow* _game, const idVec3& startPosition, const idVec2& _size, float _speed, float rotate, int _health );

	//! Returns a pointer to a specific asteroid from the asteroid pool using the given ID.
	static SSDAsteroid* GetSpecificAsteroid( int id );

	//! Writes all active asteroids to the specified save file
	static void			WriteAsteroids( idFile* savefile );

	//! Reads asteroid data from a save file into the game.
	static void			ReadAsteroids( idFile* savefile, idGameSSDWindow* _game );

protected:
	static SSDAsteroid asteroidPool[MAX_ASTEROIDS];
};

/*
*****************************************************************************
* SSDAstronaut
****************************************************************************
*/
#define MAX_ASTRONAUT 8

/*!
	\class SSDAstronaut
	\brief Manages astronaut entities in a game, handling initialization, pooling, and save game operations.

	The SSDAstronaut class represents astronaut entities within the game, inheriting from SSDMover to handle basic movement and rendering. It provides functionality for initializing astronauts with
   specific parameters, managing a fixed pool of entities to avoid dynamic allocation during gameplay, and handling save and load operations for astronaut data. The class supports both generic
   initialization through the Init method and pooled retrieval via GetNewAstronaut, which ensures efficient resource usage by reusing pre-allocated entities. It also handles serialization of astronaut
   states to and from save files, allowing the game state to be preserved and restored. The class maintains a collection of astronauts that can be accessed by index through GetSpecificAstronaut.

*/
class SSDAstronaut : public SSDMover
{
public:
	int health;

public:
	//! Initializes a new instance of the SSDAstronaut class.
	SSDAstronaut();
	~SSDAstronaut();

	//! Writes the astronaut's health data to the specified save file
	virtual void		 WriteToSaveGame( idFile* savefile );

	//! Reads the astronaut's health data from a save game file
	virtual void		 ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	/*!
		\brief Initializes an astronaut entity with specified game, position, speed, rotation, and health parameters

		This function initializes an astronaut entity by calling base entity and mover initialization functions, setting the game reference, entity type, material, size, radius, and rotation. It also
	   assigns the starting position and health values to the entity

		\param _game Pointer to the game window instance
		\param startPosition Initial 3D position of the astronaut
		\param _speed Speed value for movement initialization
		\param rotate Rotation angle for the entity
		\param _health Initial health value for the astronaut
	*/
	void				 Init( idGameSSDWindow* _game, const idVec3& startPosition, float _speed, float rotate, int _health );

	/*!
		\brief Retrieves or creates a new astronaut entity from a pool for use in the game.

		This function searches for an unused astronaut in the pre-allocated astronaut pool. If an unused astronaut is found, it initializes the astronaut with the provided game window, starting
	   position, speed, rotation, and health values. The astronaut is then marked as in use and returned. If no unused astronaut is available, the function returns NULL. The function is used to
	   efficiently manage a fixed number of astronaut entities, avoiding dynamic allocation during gameplay.

		\param _game Pointer to the game window instance that will manage the astronaut.
		\param startPosition The initial 3D position where the astronaut will be spawned.
		\param _speed The speed at which the astronaut moves.
		\param rotate The initial rotation angle of the astronaut.
		\param _health The initial health value of the astronaut.
		\return A pointer to the initialized astronaut object if successful, or NULL if no astronaut is available in the pool.
	*/
	static SSDAstronaut* GetNewAstronaut( idGameSSDWindow* _game, const idVec3& startPosition, float _speed, float rotate, int _health );

	//! Returns a pointer to the astronaut object at the specified index in the astronaut pool
	static SSDAstronaut* GetSpecificAstronaut( int id );

	//! Writes astronaut data to a save file.
	static void			 WriteAstronauts( idFile* savefile );

	//! Reads astronaut data from a save file and initializes corresponding astronaut entities.
	static void			 ReadAstronauts( idFile* savefile, idGameSSDWindow* _game );

protected:
	static SSDAstronaut astronautPool[MAX_ASTRONAUT];
};

/*
*****************************************************************************
* SSDExplosion
****************************************************************************
*/
#define MAX_EXPLOSIONS 64

/*!
	\class SSDExplosion
	\brief Manages explosion effects with pooled allocation and save game support.

	The SSDExplosion class handles explosion visual effects with a fixed-size object pool for efficient memory usage. It supports initialization with various parameters including position, size,
   duration, and type, and can optionally link to a buddy entity for tracking or destruction. The class provides methods for updating explosion state, writing and reading explosion data to/from save
   files, and retrieving specific explosions from the pool. It inherits from SSDEntity, indicating its role as a specialized entity within a larger system. The implementation uses a pool allocation
   pattern to optimize performance by reusing explosion objects rather than frequent allocation and deallocation.

*/
class SSDExplosion : public SSDEntity
{
public:
	idVec2	   finalSize;
	int		   length;
	int		   beginTime;
	int		   endTime;
	int		   explosionType;

	// The entity that is exploding
	SSDEntity* buddy;
	bool	   killBuddy;
	bool	   followBuddy;

	enum { EXPLOSION_NORMAL = 0, EXPLOSION_TELEPORT = 1 };

public:
	//! Initializes an SSDExplosion object with the explosion type.
	SSDExplosion();
	~SSDExplosion();

	//! Writes the explosion data to a save game file
	virtual void		 WriteToSaveGame( idFile* savefile );

	//! Reads explosion data from a save game file and initializes the explosion entity.
	virtual void		 ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	/*!
		\brief Initializes an explosion entity with specified properties and settings.

		Sets up the explosion entity with the provided game window, position, size, and duration. Configures the explosion type, material, and initial state. The entity will start with zero size and
	   gradually expand to its final size over the specified duration. Optionally links the explosion to a buddy entity and controls whether the buddy should be killed or followed.

		\param _game Pointer to the game window instance
		\param _position Initial 3D position of the explosion
		\param _size Final 2D size of the explosion
		\param _length Duration in milliseconds for the explosion animation
		\param _type Type identifier for the explosion effect
		\param _buddy Pointer to an optional buddy entity to link with the explosion
		\param _killBuddy Flag indicating whether the buddy entity should be destroyed
		\param _followBuddy Flag indicating whether the explosion should follow the buddy entity
	*/
	void				 Init( idGameSSDWindow* _game, const idVec3& _position, const idVec2& _size, int _length, int _type, SSDEntity* _buddy, bool _killBuddy = true, bool _followBuddy = true );

	//! Updates the explosion entity position and size, and handles destruction when the explosion animation completes.
	virtual void		 EntityUpdate();

	/*!
		\brief Returns a new explosion instance from a pool, initializing it with the provided parameters

		This function implements a fixed-size pool allocation pattern for explosion objects. It searches for the first unused explosion in the pool and initializes it with the specified parameters. If
	   no unused explosion is available, it returns NULL. The function is designed to be called by entities that need to create visual effects like explosions, using shared resources from a
	   pre-allocated pool to optimize memory usage and performance.

		\param _game Pointer to the game window instance that manages the explosion
		\param _position The 3D position where the explosion will be rendered
		\param _size The 2D size of the explosion effect
		\param _length The duration or lifetime of the explosion effect in milliseconds
		\param _type The type of explosion to create, used to determine visual properties
		\param _buddy Optional entity to associate with the explosion for tracking or effects
		\param _killBuddy Flag indicating whether the buddy entity should be destroyed when the explosion occurs
		\param _followBuddy Flag indicating whether the explosion should follow the buddy entity if it moves
		\return A pointer to the newly initialized explosion object if available, or NULL if the pool is exhausted
	*/
	static SSDExplosion* GetNewExplosion(
		idGameSSDWindow* _game, const idVec3& _position, const idVec2& _size, int _length, int _type, SSDEntity* _buddy, bool _killBuddy = true, bool _followBuddy = true );

	//! Returns a pointer to a specific explosion object from the explosion pool using the given ID
	static SSDExplosion* GetSpecificExplosion( int id );

	//! Writes explosion data to a save file.
	static void			 WriteExplosions( idFile* savefile );

	//! Reads explosion data from a save file for the SSD explosion system.
	static void			 ReadExplosions( idFile* savefile, idGameSSDWindow* _game );

protected:
	static SSDExplosion explosionPool[MAX_EXPLOSIONS];
};

#define MAX_POINTS 16

/*!
	\class SSDPoints
	\brief Manages the display and rendering of point values in a game context.

	The SSDPoints class handles the visual representation of score or damage values that appear above entities in the game. It manages a pool of reusable point display objects to avoid frequent memory
   allocation. Each point display has properties such as point value, duration, vertical movement distance, and color. The class supports initialization with entity context, updating of display
   positions and fading effects, and serialization for save game support. Points are rendered as moving text elements that fade out over time, starting from an entity's location and moving upward. The
   class maintains a collection of active point displays and provides mechanisms to retrieve, update, and manage these displays within a game window context. It integrates with a broader system for
   managing special effect entities and supports both creation of new point displays and retrieval of existing ones from a pre-allocated pool.

*/
class SSDPoints : public SSDEntity
{
	int	   length;
	int	   distance;
	int	   beginTime;
	int	   endTime;

	idVec3 beginPosition;
	idVec3 endPosition;

	idVec4 beginColor;
	idVec4 endColor;

public:
	//! Initializes an SSDPoints object with the entity points type.
	SSDPoints();
	~SSDPoints();

	//! Writes the SSD points data to a save game file
	virtual void	  WriteToSaveGame( idFile* savefile );

	//! Reads saved game data from a file into the SSDPoints object
	virtual void	  ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	/*!
		\brief Initializes the SSD points display with game context, entity reference, point value, and visual properties.

		This function sets up the visual representation of point values in the SSD window, including positioning, timing, and color properties. It calculates the screen position based on the provided
	   entity's world position and adjusts the text alignment. The points are rendered as a moving text element that fades out over time, starting from the entity's location and moving upward by a
	   specified distance.

		\param _game Pointer to the game window instance that will display the points
		\param _ent Pointer to the entity that the points are associated with
		\param _points The numeric value of points to display
		\param _length Duration in game time for which the points display remains visible
		\param _distance Vertical distance in screen space the text moves upward during display
		\param color Initial color of the text display with alpha channel for fade effect
	*/
	void			  Init( idGameSSDWindow* _game, SSDEntity* _ent, int _points, int _length, int _distance, const idVec4& color );

	//! Updates the entity position and color based on the current time and animation parameters.
	virtual void	  EntityUpdate();

	/*!
		\brief Retrieves a new SSDPoints object from the pool for displaying score or damage values.

		This function attempts to find an unused SSDPoints object in the pre-allocated pool. If an unused object is found, it initializes the object with the provided parameters and marks it as in
	   use. If no unused objects are available, it returns NULL. The returned object is intended to be added to the game entities list for rendering.

		\param _game Pointer to the game window context where the points will be displayed.
		\param _ent Pointer to the entity associated with the points, used for positioning and linking.
		\param _points The numeric value to be displayed as points or damage.
		\param _length The length of the text display area for the points.
		\param _distance The vertical distance from the entity position where the points will be rendered.
		\param color The color in which the points text will be displayed, represented as RGBA values.
		\return A pointer to the initialized SSDPoints object if successful, or NULL if no unused object is available in the pool.
	*/
	static SSDPoints* GetNewPoints( idGameSSDWindow* _game, SSDEntity* _ent, int _points, int _length, int _distance, const idVec4& color );

	//! Returns a specific points entity from the points pool by its ID
	static SSDPoints* GetSpecificPoints( int id );

	//! Writes all active points from the points pool to the provided save file
	static void		  WritePoints( idFile* savefile );

	//! Reads point data from a save file into SSDPoints entities.
	static void		  ReadPoints( idFile* savefile, idGameSSDWindow* _game );

protected:
	static SSDPoints pointsPool[MAX_POINTS];
};

#define MAX_PROJECTILES 64

/*!
	\class SSDProjectile
	\brief Manages projectile entities with object pooling and save game functionality.

	The SSDProjectile class handles projectile entities within the game, supporting initialization with specific parameters, position updates, and integration with save game systems. It utilizes an
   object pooling mechanism to efficiently manage projectile instances, reducing memory allocation overhead during gameplay. The class inherits from SSDEntity and provides methods for serializing
   projectile state to and from save files, ensuring consistent game progression. Projectile behavior is defined by start and end positions, speed, and size, with the entity configured to not register
   hits during movement. The class supports both general projectile creation through GetNewProjectile and specific projectile retrieval by ID via GetSpecificProjectile.

*/
class SSDProjectile : public SSDEntity
{
	idVec3 dir;
	idVec3 speed;
	int	   beginTime;
	int	   endTime;

	idVec3 endPosition;

public:
	//! Initializes a new instance of the SSDProjectile class.
	SSDProjectile();
	~SSDProjectile();

	//! Writes the SSD projectile's state to a save game file.
	virtual void		  WriteToSaveGame( idFile* savefile );

	//! Loads projectile-specific data from a save game file
	virtual void		  ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	/*!
		\brief Initializes a projectile with the specified game, start and end positions, speed, and size

		This function sets up the projectile entity by initializing its state, associating it with the provided game window, configuring its visual properties such as material and size, and setting
	   its initial position and movement direction. The projectile is configured to not register hits during its movement.

		\param _game Pointer to the game window instance this projectile belongs to
		\param _beginPosition Starting position of the projectile in world coordinates
		\param _endPosition Target position of the projectile in world coordinates
		\param _speed Speed at which the projectile travels
		\param _size Size of the projectile visual representation
	*/
	void				  Init( idGameSSDWindow* _game, const idVec3& _beginPosition, const idVec3& _endPosition, float _speed, float _size );

	//! Updates the projectile entity position based on direction, speed, and elapsed time.
	virtual void		  EntityUpdate();

	/*!
		\brief Returns a new SSDProjectile instance from a pool, initializing it with the specified parameters.

		This function implements a simple object pooling mechanism for SSDProjectile objects. It searches through a predefined pool of projectiles to find an unused one. If an unused projectile is
	   found, it initializes the projectile with the provided game context, start and end positions, speed, and size, marks it as in use, and returns a pointer to it. If no unused projectiles are
	   available, it returns NULL. The function is typically used to manage projectile resources efficiently, preventing frequent memory allocation and deallocation during gameplay.

		\param _game Pointer to the game window context
		\param _beginPosition Starting position of the projectile
		\param _endPosition Ending position of the projectile
		\param _speed Speed of the projectile
		\param _size Size of the projectile
		\return Pointer to a newly initialized SSDProjectile instance, or NULL if no projectiles are available in the pool.
	*/
	static SSDProjectile* GetNewProjectile( idGameSSDWindow* _game, const idVec3& _beginPosition, const idVec3& _endPosition, float _speed, float _size );

	//! Returns a specific projectile from the projectile pool by its ID
	static SSDProjectile* GetSpecificProjectile( int id );

	//! Writes all active projectiles to the specified save file.
	static void			  WriteProjectiles( idFile* savefile );

	//! Reads projectile data from a save file into the game.
	static void			  ReadProjectiles( idFile* savefile, idGameSSDWindow* _game );

protected:
	static SSDProjectile projectilePool[MAX_PROJECTILES];
};

#define MAX_POWERUPS 64

/**
 * Powerups work in two phases:
 *	1.) Closed container hurls at you
 *		If you shoot the container it open
 *	3.) If an opened powerup hits the player he aquires the powerup
 * Powerup Types:
 *	Health - Give a specific amount of health
 *	Super Blaster - Increases the power of the blaster (lasts a specific amount of time)
 *	Asteroid Nuke - Destroys all asteroids on screen as soon as it is aquired
 *	Rescue Powerup - Rescues all astronauts as soon as it is acquited
 *	Bonus Points - Gives some bonus points when acquired
 */
class SSDPowerup : public SSDMover
{
	enum { POWERUP_STATE_CLOSED = 0, POWERUP_STATE_OPEN };

	enum { POWERUP_TYPE_HEALTH = 0, POWERUP_TYPE_SUPER_BLASTER, POWERUP_TYPE_ASTEROID_NUKE, POWERUP_TYPE_RESCUE_ALL, POWERUP_TYPE_BONUS_POINTS, POWERUP_TYPE_DAMAGE, POWERUP_TYPE_MAX };

	int powerupState;
	int powerupType;

public:
public:
	//! Initializes an SSDPowerup object with default values.
	SSDPowerup();
	virtual ~SSDPowerup();

	//! Writes the SSD powerup state and type to a save game file
	virtual void	   WriteToSaveGame( idFile* savefile );

	//! Deserializes the power-up state and type from a save file
	virtual void	   ReadFromSaveGame( idFile* savefile, idGameSSDWindow* _game );

	//! Handles the event when the powerup is hit, either opening it with a small explosion or destroying it with a large explosion.
	virtual void	   OnHit( int key );

	//! Handles the event when the SSD powerup strikes a player by activating it if open and then marking it for destruction.
	virtual void	   OnStrikePlayer();

	//! Handles the event when a power-up object is opened.
	void			   OnOpenPowerup();

	//! Activates the powerup based on its type and performs the corresponding game action.
	void			   OnActivatePowerup();

	//! Initializes the SSD powerup entity with game reference, speed, and rotation parameters.
	void			   Init( idGameSSDWindow* _game, float _speed, float _rotation );

	//! Retrieves or creates a new powerup instance from a pool with specified speed and rotation parameters.
	static SSDPowerup* GetNewPowerup( idGameSSDWindow* _game, float _speed, float _rotation );

	//! Returns a pointer to a specific powerup from the powerup pool using the given ID.
	static SSDPowerup* GetSpecificPowerup( int id );

	//! Writes all active powerups to the specified save file.
	static void		   WritePowerups( idFile* savefile );

	//! Reads powerup data from a save file into the game.
	static void		   ReadPowerups( idFile* savefile, idGameSSDWindow* _game );

protected:
	static SSDPowerup powerupPool[MAX_POWERUPS];
};

typedef struct {
	float spawnBuffer;
	int	  needToWin;
} SSDLevelData_t;

typedef struct {
	float speedMin, speedMax;
	float sizeMin, sizeMax;
	float rotateMin, rotateMax;
	int	  spawnMin, spawnMax;
	int	  asteroidHealth;
	int	  asteroidPoints;
	int	  asteroidDamage;
} SSDAsteroidData_t;

typedef struct {
	float speedMin, speedMax;
	float rotateMin, rotateMax;
	int	  spawnMin, spawnMax;
	int	  health;
	int	  points;
	int	  penalty;
} SSDAstronautData_t;

typedef struct {
	float speedMin, speedMax;
	float rotateMin, rotateMax;
	int	  spawnMin, spawnMax;
} SSDPowerupData_t;

typedef struct {
	float speed;
	int	  damage;
	int	  size;
} SSDWeaponData_t;

/**
 * SSDLevelStats_t
 *	Data that is used for each level. This data is reset
 *	each new level.
 */
typedef struct {
	int		   shotCount;
	int		   hitCount;
	int		   destroyedAsteroids;
	int		   nextAsteroidSpawnTime;

	int		   killedAstronauts;
	int		   savedAstronauts;

	// Astronaut Level Data
	int		   nextAstronautSpawnTime;

	// Powerup Level Data
	int		   nextPowerupSpawnTime;

	SSDEntity* targetEnt;
} SSDLevelStats_t;

/**
 * SSDGameStats_t
 *	Data that is used for the game that is currently running. Memset this
 *	to completely reset the game
 */
typedef struct {
	bool			gameRunning;

	int				score;
	int				prebonusscore;

	int				health;

	int				currentWeapon;
	int				currentLevel;
	int				nextLevel;

	SSDLevelStats_t levelStats;
} SSDGameStats_t;

/*!
	\class idGameSSDWindow
	\brief A window class that manages the game UI and game state for a space shooter defense game.

	This class implements a window-based UI system for a space shooter defense game, handling both the visual presentation and game logic. It manages game statistics, entity spawning and rendering,
   input handling, and game state transitions. The class integrates with a GUI system to display game information and responds to system events. It includes functionality for managing player health,
   scoring, level progression, and various game abilities like super blaster. The class also handles serialization of game state to and from save files, asset precaching, and entity management
   including collision detection and rendering. The window maintains a list of game entities and updates their positions and states based on game logic and user input.

*/
class idGameSSDWindow : public idWindow
{
public:
	//! Constructor for the idGameSSDWindow class that initializes the window with the provided GUI interface
	idGameSSDWindow( idUserInterfaceLocal* gui );

	//! Destroys the game SSD window and resets game statistics.
	~idGameSSDWindow();

	//! Serializes the SSD window state and associated game data to a save file.
	virtual void		WriteToSaveGame( idFile* savefile );

	//! Restores the state of the SSD window from a save game file.
	virtual void		ReadFromSaveGame( idFile* savefile );

	//! Processes a system event for the SSD window and returns a pointer to a string describing the event handling result.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Retrieves a window variable by name, checking for specific predefined variables first before falling back to the base implementation.
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Draws the game window content including entities and crosshair at the specified position and time.
	virtual void		Draw( int time, float x, float y );

	//! Adds the specified health amount to the current game statistics, clamped to a maximum of 100.
	void				AddHealth( int health );

	//! Adds points to the score for the specified entity and displays a visual feedback.
	void				AddScore( SSDEntity* ent, int points );

	//! Adds damage to the player's health and updates the GUI, potentially triggering game over if health reaches zero.
	void				AddDamage( int damage );

	//! Handles the nuke event by destroying all asteroids and triggering explosions.
	void				OnNuke();

	//! Handles the rescue all astronauts event and processes each astronaut entity.
	void				OnRescueAll();

	//! Initializes the super blaster functionality.
	void				OnSuperBlaster();

	//! Returns a specific SSDEntity instance based on the given type and ID.
	SSDEntity*			GetSpecificEntity( int type, int id );

	//! Plays a sound shader directly using the sound system.
	void				PlaySound( const char* sound );

	static idRandom		random;
	int					ssdTime;

private:
	//! Parses internal variables for the SSD window configuration.
	virtual bool ParseInternalVar( const char* name, idTokenParser* src );

	//! Parses level data from a string and stores the parsed values in the level data array.
	void		 ParseLevelData( int level, const idStr& levelDataString );

	//! Parses asteroid data for a specified level from a string.
	void		 ParseAsteroidData( int level, const idStr& asteroidDataString );

	//! Parses weapon data from a string and stores it in the weapon data array.
	void		 ParseWeaponData( int weapon, const idStr& weaponDataString );

	//! Parses astronaut data for a given level from a string.
	void		 ParseAstronautData( int level, const idStr& astronautDataString );

	//! Parses powerup data for a specific level from a string.
	void		 ParsePowerupData( int level, const idStr& powerupDataString );

	//! Initializes the SSD window common data and precaches required assets
	void		 CommonInit();

	//! Resets the game statistics and entity states to their initial values.
	void		 ResetGameStats();

	//! Resets the level statistics and entities.
	void		 ResetLevelStats();

	//! Destroys all entities in the window and clears the entity list.
	void		 ResetEntities();

	//! Starts the game by setting the game running flag to true.
	void		 StartGame();

	//! Stops the currently running game by setting the game running flag to false.
	void		 StopGame();

	//! Ends the current game and triggers the game over event.
	void		 GameOver();

	//! Initializes the game state for a new level.
	void		 BeginLevel( int level );

	//! Continue game resets the player's health to 100 and starts the game.
	void		 ContinueGame();

	//! Handles the completion of a game level by calculating bonuses and advancing to the next level or game completion.
	void		 LevelComplete();

	//! Indicates that the game has completed by stopping the game and triggering a gameComplete event.
	void		 GameComplete();

	//! Updates the game state and handles GUI interactions
	void		 UpdateGame();

	//! Checks for collisions between game entities and the player within the defined screen bounds.
	void		 CheckForHits();

	//! Sorts entities by their Z position in ascending order.
	void		 ZOrderEntities();

	//! Spawns a new asteroid at a random position with randomized properties.
	void		 SpawnAsteroid();

	//! Handles firing a weapon based on the input key, typically for mouse clicks in the SSD window.
	void		 FireWeapon( int key );

	//! Returns the first SSD entity that contains the specified point, or NULL if no entity is found.
	SSDEntity*	 EntityHitTest( const idVec2& pt );

	//! Processes the destruction or damage of an asteroid based on the hit key.
	void		 HitAsteroid( SSDAsteroid* asteroid, int key );

	//! Handles the event when an asteroid strikes the player by applying damage and creating an explosion.
	void		 AsteroidStruckPlayer( SSDAsteroid* asteroid );

	//! Updates the GUI with the latest game statistics and player information.
	void		 RefreshGuiData();

	//! Returns the current cursor position in world coordinates.
	idVec2		 GetCursorWorld();

	//! Spawns a new astronaut entity at a random position with randomized speed, rotation, and health values.
	void		 SpawnAstronaut();

	//! Processes a hit on an astronaut, reducing health and handling destruction or damage effects.
	void		 HitAstronaut( SSDAstronaut* astronaut, int key );

	//! Handles the event when an astronaut is struck by a player, updating game statistics and triggering visual and audio effects.
	void		 AstronautStruckPlayer( SSDAstronaut* astronaut );

	//! Spawns a powerup entity with randomized speed and rotation properties.
	void		 SpawnPowerup();

	//! Starts the super blaster effect and updates the game statistics.
	void		 StartSuperBlaster();

	//! Stops the super blaster effect and resets the associated game statistics.
	void		 StopSuperBlaster();

	// void				FreeSoundEmitter( bool immediate );

public:
	// WinVars used to call functions from the guis
	idWinBool				   beginLevel;
	idWinBool				   resetGame;
	idWinBool				   continueGame;
	idWinBool				   refreshGuiData;

	SSDCrossHair			   crosshair;
	idBounds				   screenBounds;

	// Level Data
	int						   levelCount;
	idList<SSDLevelData_t>	   levelData;
	idList<SSDAsteroidData_t>  asteroidData;
	idList<SSDAstronautData_t> astronautData;
	idList<SSDPowerupData_t>   powerupData;

	// Weapon Data
	int						   weaponCount;
	idList<SSDWeaponData_t>	   weaponData;

	int						   superBlasterTimeout;

	// All current game data is stored in this structure (except the entity list)
	SSDGameStats_t			   gameStats;
	idList<SSDEntity*>		   entities;

	int						   currentSound;
};

#endif //__GAME_SSD_WINDOW_H__
