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

#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

/*
===============================================================================

	Game entity base class.

===============================================================================
*/

static const int		DELAY_DORMANT_TIME = 3000;

extern const idEventDef EV_PostSpawn;
extern const idEventDef EV_FindTargets;
extern const idEventDef EV_Touch;
extern const idEventDef EV_Use;
extern const idEventDef EV_Activate;
extern const idEventDef EV_ActivateTargets;
extern const idEventDef EV_Hide;
extern const idEventDef EV_Show;
extern const idEventDef EV_GetShaderParm;
extern const idEventDef EV_SetShaderParm;
extern const idEventDef EV_SetOwner;
extern const idEventDef EV_GetAngles;
extern const idEventDef EV_SetAngles;
extern const idEventDef EV_SetLinearVelocity;
extern const idEventDef EV_SetAngularVelocity;
extern const idEventDef EV_SetSkin;
extern const idEventDef EV_StartSoundShader;
extern const idEventDef EV_StopSound;
extern const idEventDef EV_CacheSoundShader;

// Think flags
enum {
	TH_ALL			   = -1,
	TH_THINK		   = 1, // run think function each frame
	TH_PHYSICS		   = 2, // run physics each frame
	TH_ANIMATE		   = 4, // update animation each frame
	TH_UPDATEVISUALS   = 8, // update renderEntity
	TH_UPDATEPARTICLES = 16
};

//
// Signals
// make sure to change script/doom_defs.script if you add any, or change their order
//
typedef enum {
	SIG_TOUCH,	 // object was touched
	SIG_USE,	 // object was used
	SIG_TRIGGER, // object was activated
	SIG_REMOVED, // object was removed from the game
	SIG_DAMAGE,	 // object was damaged
	SIG_BLOCKED, // object was blocked

	SIG_MOVER_POS1, // mover at position 1 (door closed)
	SIG_MOVER_POS2, // mover at position 2 (door open)
	SIG_MOVER_1TO2, // mover changing from position 1 to 2
	SIG_MOVER_2TO1, // mover changing from position 2 to 1

	NUM_SIGNALS
} signalNum_t;

// FIXME: At some point we may want to just limit it to one thread per signal, but
// for now, I'm allowing multiple threads.  We should reevaluate this later in the project
#define MAX_SIGNAL_THREADS 16 // probably overkill, but idList uses a granularity of 16

struct signal_t {
	int				  threadnum;
	const function_t* function;
};

/*!
	\class signalList_t
	\brief A class representing a collection of signals.
*/
class signalList_t
{
public:
	idList<signal_t, TAG_ENTITY> signal[NUM_SIGNALS];
};

/*
================================================
idNetEvent

Utility for detecting a bool state change:
-server calls ::Set
-client ::Get will return true (once only)

Useful because:
-Hides client from having to manually declare "last" state and manually checking against it
-using int counter prevents problems w/ dropped snapshots

(ie if we just serialized a bool to true for a single ss, if that ss is dropped,skipped,whatever
the client would never handle it. By incrementing a wrapped counter, we are guaranteed to detect
the state change no matter what happens at the net layer).
================================================
*/
template<int max>
struct idNetEvent {
	//! Initializes a new instance of the idNetEvent class with default values for count and lastCount.
	idNetEvent() :
		count( 0 ),
		lastCount( 0 )
	{
	}

	//! Sets the count to the next value in a cyclic manner up to the maximum
	void Set() { count = ( ( count + 1 ) % max ); }

	//! Returns true if the event count has changed since the last check.
	bool Get()
	{
		if( count != lastCount ) {
			lastCount = count;
			return true;
		}
		return false;
	}

	//! Serializes the net event data using the provided serializer
	void Serialize( idSerializer& ser )
	{
		if( count >= max ) { idLib::Warning( "idNetEvent. count %d > max %d", count, max ); }
		ser.SerializeUMax( count, max );
	}

public:
	static const int Maximum = max;
	int				 count;
	int				 lastCount;
};

typedef idNetEvent<7> netBoolEvent_t;

//! Writes a network boolean event to a bit message
inline void			  WriteToBitMsg( const netBoolEvent_t& netEvent, idBitMsg& msg )
{
	msg.WriteBits( netEvent.count, idMath::BitsForInteger( netBoolEvent_t::Maximum ) );

	assert( netEvent.count <= netBoolEvent_t::Maximum );
}

//! Reads a boolean event count from a bit message.
inline void ReadFromBitMsg( netBoolEvent_t& netEvent, const idBitMsg& msg )
{
	netEvent.count = msg.ReadBits( idMath::BitsForInteger( netBoolEvent_t::Maximum ) );

	assert( netEvent.count <= netBoolEvent_t::Maximum );
}

/*!
	\class idEntity
	\brief Base entity type for the game world managing entity state, rendering, physics, scripting, and network synchronization.

	Entities are the fundamental building blocks of the game world, representing all interactive objects, actors, and environmental elements. This class provides the core functionality for entities to
   exist, be managed, and interact within the game world. It handles entity lifecycle management, from initialization and spawning to destruction, as well as core systems like rendering, physics
   simulation, sound, and networking. The class also provides mechanisms for scripting integration, team management, binding to other entities, and various game-specific behaviors through its
   extensive method set. The class supports both single-player and multiplayer environments, with methods for network replication, snapshot management, and event handling. It provides mechanisms for
   entities to be bound to other entities for coordinated movement and behavior, as well as team-based interactions. The class encapsulates rendering functionality for visual representation, physics
   simulation for movement and collisions, and sound playback capabilities. Additionally, it includes methods for managing entity properties, spawn arguments, target entities, and various game logic
   operations through both direct methods and script event handlers.

*/
class idEntity : public idClass
{
public:
	static const int						  MAX_PVS_AREAS			 = 4;
	static const uint32						  INVALID_PREDICTION_KEY = 0xFFFFFFFF;

	int										  entityNumber;	   // index into the entity list
	int										  entityDefNumber; // index into the entity def list

	idLinkList<idEntity>					  spawnNode;	 // for being linked into spawnedEntities list
	idLinkList<idEntity>					  activeNode;	 // for being linked into activeEntities list
	idLinkList<idEntity>					  aimAssistNode; // linked into gameLocal.aimAssistEntities

	idLinkList<idEntity>					  snapshotNode;	   // for being linked into snapshotEntities list
	int										  snapshotChanged; // used to detect snapshot state changes
	int										  snapshotBits;	   // number of bits this entity occupied in the last snapshot
	bool									  snapshotStale;   // Set to true if this entity is considered stale in the snapshot

	idStr									  name;			// name of entity
	idDict									  spawnArgs;	// key/value pairs used to spawn and initialize entity
	idScriptObject							  scriptObject; // contains all script defined data for this entity

	int										  thinkFlags;	// TH_? flags
	int										  dormantStart; // time that the entity was first closed off from player
	bool									  cinematic;	// during cinematics, entity will only think if cinematic is set

	renderView_t*							  renderView;	// for camera views from this entity
	idEntity*								  cameraTarget; // any remoteRenderMap shaders will use this

	idList<idEntityPtr<idEntity>, TAG_ENTITY> targets; // when this entity is activated these entities entity are activated

	int										  health; // FIXME: do all objects really need health?

	struct entityFlags_s {
		bool notarget			: 1; // if true never attack or target this entity
		bool noknockback		: 1; // if true no knockback from hits
		bool takedamage			: 1; // if true this entity can be damaged
		bool hidden				: 1; // if true this entity is not visible
		bool bindOrientated		: 1; // if true both the master orientation is used for binding
		bool solidForTeam		: 1; // if true this entity is considered solid when a physics team mate pushes entities
		bool forcePhysicsUpdate : 1; // if true always update from the physics whether the object moved or not
		bool selected			: 1; // if true the entity is selected for editing
		bool neverDormant		: 1; // if true the entity never goes dormant
		bool isDormant			: 1; // if true the entity is dormant
		bool hasAwakened		: 1; // before a monster has been awakened the first time, use full PVS for dormant instead of area-connected
		bool networkSync		: 1; // if true the entity is synchronized over the network
		bool grabbed			: 1; // if true object is currently being grabbed
		bool skipReplication	: 1; // don't replicate this entity over the network.
	} fl;

	int				  timeGroup;

	bool			  noGrab;

	renderEntity_t	  xrayEntity;
	qhandle_t		  xrayEntityHandle;
	const idDeclSkin* xraySkin;

	//! Sets the time group for the entity based on whether slow motion is active or if the game is in multiplayer mode.
	void			  DetermineTimeGroup( bool slowmo );

	//! Sets the grabbed state of the entity.
	void			  SetGrabbedState( bool grabbed );

	//! Returns true if the entity is currently grabbed, false otherwise.
	bool			  IsGrabbed();

public:
	ABSTRACT_PROTOTYPE( idEntity );

	//! Initializes a new idEntity instance with default values.
	idEntity();

	//! Destructor for idEntity that cleans up all resources and removes the entity from the game world.
	~idEntity();

	//! Initializes an entity during game startup by parsing spawn arguments and setting up its properties.
	void					Spawn();

	//! Saves the entity state to a save game file
	void					Save( idSaveGame* savefile ) const;

	//! Restores the entity state from a save file
	void					Restore( idRestoreGame* savefile );

	//! Returns the name of the entity definition for this entity.
	const char*				GetEntityDefName() const;

	//! Sets the name of the entity and updates the game local hash table accordingly.
	void					SetName( const char* name );

	//! Returns the name of the entity as a null-terminated string.
	const char*				GetName() const;

	//! Updates changeable spawn arguments and handles camera target and GUI parameter updates.
	virtual void			UpdateChangeableSpawnArgs( const idDict* source );

	//! Returns the entity number of this entity.
	int						GetEntityNumber() const { return entityNumber; }

	//! Returns the render view for this entity, initializing it if necessary.
	virtual renderView_t*	GetRenderView();

	//! Executes the entity's thinking logic by running physics and presenting the entity.
	virtual void			Think();

	//! Checks if the entity should be marked as dormant based on its current state and triggers appropriate begin/end dormant callbacks.
	bool					CheckDormant();

	//! Called when the entity becomes dormant.
	virtual void			DormantBegin();

	//! Called when an entity wakes from being dormant.
	virtual void			DormantEnd();

	//! Returns true if the entity is active in the game world.
	bool					IsActive() const;

	//! Activates the entity for the specified think flags, potentially enabling physics or updating active entity tracking.
	void					BecomeActive( int flags );

	//! Deactivates the entity from the specified think flags.
	void					BecomeInactive( int flags );

	//! Updates the PVS areas for the entity based on the provided position.
	void					UpdatePVSAreas( const idVec3& pos );

	//! Enables replication for this entity and resets its physics interpolation state.
	void					BecomeReplicated();

	//! Retrieves a floating-point value associated with the specified key from the entity's spawn arguments.
	float					GetFloat( const char* key );

	//! Retrieves the string value associated with a specified key from the entity's spawn arguments.
	const char*				GetKey( const char* key );

	//! Retrieves an integer value associated with the specified key from the entity's spawn arguments
	int						GetInt( const char* key );

	//! Retrieves a boolean value from the entity's spawn arguments using the specified key
	bool					GetBool( const char* key );

	//! Handles the event when damage is inflicted by this entity on a target.
	virtual void			InflictedDamageEvent( idEntity* target ) { }

	//! Submits the entity's visual representation to the renderer for display.
	virtual void			Present();

	//! Returns the render entity associated with this entity
	virtual renderEntity_t* GetRenderEntity();

	//! Returns the model definition handle associated with this entity.
	virtual int				GetModelDefHandle();

	//! Sets the model for the entity using the provided model name.
	virtual void			SetModel( const char* modelname );

	//! Sets the skin for the entity to be used in rendering.
	void					SetSkin( const idDeclSkin* skin );

	//! Returns the custom skin associated with the entity.
	const idDeclSkin*		GetSkin() const;

	//! Sets a shader parameter value for the entity
	void					SetShaderParm( int parmnum, float value );

	//! Sets the color values for the entity's render entity.
	virtual void			SetColor( float red, float green, float blue );

	//! Sets the color of the entity using an idVec3 representing RGB values
	virtual void			SetColor( const idVec3& color );

	//! Retrieves the RGB color components of the entity and stores them in the provided vector
	virtual void			GetColor( idVec3& out ) const;

	//! Sets the color for the entity's rendering
	virtual void			SetColor( const idVec4& color );

	//! Returns the color of the entity as an idVec4 value
	virtual void			GetColor( idVec4& out ) const;

	//! Releases the model definition handle if it is valid.
	virtual void			FreeModelDef();

	//! Frees the light definition associated with this entity.
	virtual void			FreeLightDef();

	//! Hides the entity by marking it as hidden and updating its visual state.
	virtual void			Hide();

	//! Displays the entity if it is currently hidden.
	virtual void			Show();

	//! Returns true if the entity is currently hidden.
	bool					IsHidden() const;

	//! Updates the visual representation of the entity by refreshing its model and sound.
	void					UpdateVisuals();

	//! Updates the entity's model and associated render information.
	void					UpdateModel();

	//! Updates the model transform based on physics and visual transform data.
	void					UpdateModelTransform();

	/*!
		\brief Projects a texture overlay onto the entity's dynamic model at the specified position and orientation

		This function calculates a texture coordinate system based on the provided origin and direction, then projects a texture overlay onto the entity's dynamic model. The overlay is positioned
	   relative to the entity's coordinate system and uses the specified material. The function first validates that the entity has a valid model handle and that the model is a dynamic MD5 model
	   before proceeding with the projection. The overlay is rendered using the renderer's ProjectOverlay method and will cause the entity to update its visuals to reflect the new overlay.

		\param origin The world position where the overlay should be placed
		\param dir The direction vector that determines the orientation of the overlay
		\param size Controls the scaling of the overlay texture coordinates
		\param material The material name to be used for the overlay texture
	*/
	virtual void			ProjectOverlay( const idVec3& origin, const idVec3& dir, float size, const char* material );

	//! Returns the number of PVS areas that the entity is currently in.
	int						GetNumPVSAreas();

	//! Returns the PVS areas for this entity.
	const int*				GetPVSAreas();

	//! Clears the PVS areas associated with this entity.
	void					ClearPVSAreas();

	//! Checks if any entity in the physics team is visible in the specified PVS
	bool					PhysicsTeamInPVS( pvsHandle_t pvsHandle );

	// jmarshall
	virtual void			CallNativeEvent( idStr& name ) {};

	//! Updates animation controllers for the entity.
	virtual bool			UpdateAnimationControllers();

	//! Updates the render entity data for the given render view
	bool					UpdateRenderEntity( renderEntity_t* renderEntity, const renderView_t* renderView );

	//! Handles model callback for rendering entities by updating their render entity based on the current view.
	static bool				ModelCallback( renderEntity_t* renderEntity, const renderView_t* renderView );

	//! Returns the animator object used by this entity.
	virtual idAnimator*		GetAnimator();

	//! Indicates whether the entity can play chatter sounds.
	virtual bool			CanPlayChatterSounds() const;

	/*!
		\brief Starts playing a sound from a sound definition on the specified channel with optional flags and broadcast behavior

		This function initiates playback of a sound defined by a sound definition name. It validates the sound definition name format, retrieves the actual sound name from the entity's spawn
	   arguments, and then starts the sound using the sound system. The function handles cases where the sound definition is invalid or the sound system is not ready to process new sounds. It supports
	   additional sound shader flags and optional broadcast behavior for network synchronization. The length of the sound can optionally be returned through the length parameter.

		\param soundName Name of the sound definition to play
		\param channel Logical channel to play the sound on
		\param soundShaderFlags Additional flags to modify the sound behavior
		\param broadcast Whether to broadcast the sound to other clients in multiplayer
		\param length Optional pointer to store the length of the sound in milliseconds
		\return True if the sound was successfully started, false otherwise
		\throws Asserts if the sound name does not start with 'snd_'
	*/
	bool					StartSound( const char* soundName, const s_channelType channel, int soundShaderFlags, bool broadcast, int* length );

	/*!
		\brief Starts playing a sound shader on the entity with the specified channel and flags

		The function initiates playback of a sound shader associated with the entity. It handles both local and networked sound playback, sending events to clients when running on the server and
	   broadcasting. The function manages sound diversity, allocates sound emitters if needed, and updates the entity's sound state. It returns the length of the sound if a length parameter is
	   provided

		\param shader The sound shader to play
		\param channel The audio channel to play the sound on
		\param soundShaderFlags Flags controlling sound shader behavior
		\param broadcast Whether to broadcast the sound to all clients
		\param length Optional pointer to store the sound length
		\return True if the sound was successfully started, false otherwise
	*/
	bool					StartSoundShader( const idSoundShader* shader, const s_channelType channel, int soundShaderFlags, bool broadcast, int* length );

	//! Stops sound on the specified channel, with optional network broadcast capability.
	void					StopSound( const s_channelType channel, bool broadcast );

	//! Sets the volume for the entity's sound.
	void					SetSoundVolume( float volume );

	//! Updates the sound emitter position and parameters based on the entity's physics state.
	void					UpdateSound();

	//! Returns the listener ID associated with this entity.
	int						GetListenerId() const;

	//! Returns the sound emitter associated with this entity.
	idSoundEmitter*			GetSoundEmitter() const;

	//! Releases the sound emitter associated with this entity, optionally immediately.
	void					FreeSoundEmitter( bool immediate );

	//! Performs entity binding operations before the entity is bound.
	virtual void			PreBind();

	//! Performs post-binding operations for the entity.
	virtual void			PostBind();

	//! Performs pre-unbind operations for the entity.
	virtual void			PreUnbind();

	//! Performs cleanup operations after the entity has been unbound from the game world.
	virtual void			PostUnbind();

	//! Joins the specified entity to the caller's team.
	void					JoinTeam( idEntity* teammember );

	//! Binds this entity to a master entity, optionally orienting it relative to the master's visual position.
	void					Bind( idEntity* master, bool orientated );

	//! Binds the entity to a joint of the master entity's skeletal model
	void					BindToJoint( idEntity* master, const char* jointname, bool orientated );

	//! Binds this entity to a joint of the specified master entity.
	void					BindToJoint( idEntity* master, jointHandle_t jointnum, bool orientated );

	//! Binds the entity to a specific body of a master entity for relative positioning and orientation.
	void					BindToBody( idEntity* master, int bodyId, bool orientated );

	//! Removes all binding constraints and team connections from the entity.
	void					Unbind();

	//! Returns true if the entity is bound to a master entity.
	bool					IsBound() const;

	//! Checks if the entity is bound to the specified master entity.
	bool					IsBoundTo( idEntity* master ) const;

	//! Returns the bind master entity of this entity.
	idEntity*				GetBindMaster() const;

	//! Returns the joint handle that the entity is bound to.
	jointHandle_t			GetBindJoint() const;

	//! Returns the index of the body this entity is bound to.
	int						GetBindBody() const;

	//! Returns the team master entity of this entity.
	idEntity*				GetTeamMaster() const;

	//! Returns the next entity in the team chain.
	idEntity*				GetNextTeamEntity() const;

	//! Converts a local offset and axis to their world space equivalents.
	void					ConvertLocalToWorldTransform( idVec3& offset, idMat3& axis );

	//! Converts a vector from world space to local space relative to the entity's master.
	idVec3					GetLocalVector( const idVec3& vec ) const;

	//! Transforms a vector from world coordinates to the entity's local coordinates using its parent object's transformation
	idVec3					GetLocalCoordinates( const idVec3& vec ) const;

	//! Converts a vector from local space to world space using the entity's master transform
	idVec3					GetWorldVector( const idVec3& vec ) const;

	//! Transforms a vector from the entity's local coordinates to world coordinates using its master object's transformation.
	idVec3					GetWorldCoordinates( const idVec3& vec ) const;

	//! Returns the position and axis of the entity's master object if bound, otherwise returns false.
	bool					GetMasterPosition( idVec3& masterOrigin, idMat3& masterAxis ) const;

	//! Retrieves the world-space linear and angular velocities of the entity, accounting for master binding if applicable.
	void					GetWorldVelocities( idVec3& linearVelocity, idVec3& angularVelocity ) const;

	//! Sets a new physics object for the entity, clearing contacts from the previous physics object.
	void					SetPhysics( idPhysics* phys );

	//! Retrieves the physics object associated with this entity.
	idPhysics*				GetPhysics() const;

	//! Restores the physics pointer for save games.
	void					RestorePhysics( idPhysics* phys );

	//! Runs the physics simulation for the entity and its team members
	bool					RunPhysics();

	//! Interpolates the physics state of the entity using the provided fraction for smooth client-side updates.
	void					InterpolatePhysics( const float fraction );

	//! Updates entity physics state by interpolating to the specified fraction with optional team synchronization.
	void					InterpolatePhysicsOnly( const float fraction, bool updateTeam = false );

	//! Sets the origin of the entity's physics object and updates its visual representation.
	void					SetOrigin( const idVec3& org );

	//! Sets the axis of the entity's physics object, updating the visuals afterward.
	void					SetAxis( const idMat3& axis );

	//! Sets the axis of the entity using the provided angles.
	void					SetAngles( const idAngles& ang );

	//! Retrieves the floor position underneath the entity within the specified distance.
	bool					GetFloorPos( float max_dist, idVec3& floorpos ) const;

	//! Retrieves the transformation from physics to visual origin and axis.
	virtual bool			GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );

	//! Returns the transformation from the physics origin/axis to the sound origin/axis for the entity's first clip model.
	virtual bool			GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );

	//! Returns false to indicate that the physics simulation should not stop upon collision.
	virtual bool			Collide( const trace_t& collision, const idVec3& velocity );

	/*!
		\brief Retrieves impact information for a given entity at a specific point

		The function fetches impact information from the physics system for the specified entity at the provided point. The impact information includes details about the impact such as force,
	   direction, and other relevant physical properties. The entity parameter is used to identify which entity is retrieving the information, but the actual physics computation is delegated to the
	   physics system.

		\param ent The entity retrieving the impact information
		\param id Identifier for the impact event
		\param point The point in world space where the impact occurred
		\param info Output parameter that will contain the impact information
	*/
	virtual void			GetImpactInfo( idEntity* ent, int id, const idVec3& point, impactInfo_t* info );

	/*!
		\brief Applies an impulse to the physics object of the entity

		This function applies an impulse to the physics object of the entity using the provided point of application and impulse vector. The impulse is applied to the physics object associated with
	   the entity, which is retrieved through the GetPhysics() method. The entity performing the impulse application is passed as the first parameter, and the impulse is applied at the specified point
	   relative to the entity's origin.

		\param ent The entity applying the impulse
		\param id The identifier for the physics object
		\param point The point of application for the impulse
		\param impulse The impulse vector to apply
	*/
	virtual void			ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );

	/*!
		\brief Adds a force to the physics object of this entity.

		This function applies a force vector at a specified point on the entity's physics object. The force is added using the physics simulation system. The entity parameter is not directly used in
	   the implementation, but is part of the function signature for consistency with other force-related functions.

		\param ent entity adding the force
		\param id identifier for the force
		\param point point in global coordinates where the force is applied
		\param force force vector to be applied
	*/
	virtual void			AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );

	//! Activates the physics object of the specified entity.
	virtual void			ActivatePhysics( idEntity* ent );

	//! Returns true if the physics object of this entity is at rest.
	virtual bool			IsAtRest() const;

	//! Returns the time the physics object came to rest.
	virtual int				GetRestStartTime() const;

	//! Adds a contact entity to the physics system.
	virtual void			AddContactEntity( idEntity* ent );

	//! Removes a touching entity from the contact list.
	virtual void			RemoveContactEntity( idEntity* ent );

	//! Determines whether the entity can be damaged from a specified origin point.
	virtual bool			CanDamage( const idVec3& origin, idVec3& damagePoint ) const;

	/*!
		\brief Applies damage to this entity using the specified damage definition and parameters.

		This function handles the application of damage to an entity, taking into account whether the entity can take damage, the source of the damage, the attacking entity, the damage direction, and
	   the damage scale. It retrieves the damage definition from the game's entity dictionary and calculates the damage amount. If the damage is sufficient to reduce the entity's health below zero,
	   the entity is killed. Otherwise, the entity's pain reaction is triggered. The function also informs the attacker of the damage dealt.

		\param inflictor entity that caused the damage
		\param attacker entity that initiated the attack
		\param dir direction vector of the damage
		\param damageDefName name of the damage definition to use
		\param damageScale scale factor to apply to the base damage
		\param location location identifier where the damage was applied
		\throws Error when the damage definition is not found
	*/
	virtual void			Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );

	//! Adds a damage effect such as overlays, blood, sparks, or debris based on the collision and damage definition.
	virtual void			AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );

	//! Handles damage feedback when another entity receives damage from this entity, allowing damage adjustment.
	virtual void			DamageFeedback( idEntity* victim, idEntity* inflictor, int& damage );

	/*!
		\brief Notifies this entity that it is in pain

		This function is called when the entity experiences damage or pain. It serves as a callback mechanism to allow entities to respond to being hurt. The function currently returns false,
	   indicating that it does not handle the pain event, but it can be overridden by derived classes to implement custom pain behavior.

		\param inflictor Entity that caused the damage
		\param attacker Entity that initiated the attack
		\param damage Amount of damage inflicted
		\param dir Direction from which the damage came
		\param location Location on the entity where the damage occurred
		\return False indicating that the pain event was not handled
	*/
	virtual bool			Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	/*!
		\brief Notifies this entity that it has been killed

		This function is called when the entity receives damage that results in its death. It provides information about the source of the damage, the attacker, the amount of damage dealt, the
	   direction of the attack, and the location of the hit. The function serves as a callback for entities to handle death-related logic and notifications.

		\param inflictor The entity that caused the damage
		\param attacker The entity that initiated the attack
		\param damage The amount of damage that killed the entity
		\param dir The direction from which the damage came
		\param location The location of the hit on the entity
	*/
	virtual void			Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Returns true to indicate that the entity should construct its script object at spawn.
	virtual bool			ShouldConstructScriptObjectAtSpawn() const;

	//! Constructs and returns a script object thread for the entity.
	virtual idThread*		ConstructScriptObject();

	//! Calls the destructor on the script object associated with this entity.
	virtual void			DeconstructScriptObject();

	//! Sets a signal handler for the specified signal number with the given thread and function.
	void					SetSignal( signalNum_t signalnum, idThread* thread, const function_t* function );

	//! Clears a signal for the entity.
	void					ClearSignal( idThread* thread, signalNum_t signalnum );

	//! Clears a specific signal for a given thread in the entity's signal system.
	void					ClearSignalThread( signalNum_t signalnum, idThread* thread );

	//! Checks if the entity has any signal handlers registered for the specified signal number.
	bool					HasSignal( signalNum_t signalnum ) const;

	//! Sends a signal to all threads that are waiting for the specified signal number.
	void					Signal( signalNum_t signalnum );

	//! Sends a signal event to the entity, triggering the corresponding signal handler.
	void					SignalEvent( idThread* thread, signalNum_t signalnum );

	//! Triggers all GUIs associated with the entity's render data.
	void					TriggerGuis();

	//! Processes GUI commands for an entity and its targets, returning true if the GUI should be closed.
	bool					HandleGuiCommands( idEntity* entityGui, const char* cmds );

	//! Returns false indicating that the GUI command handling is not implemented or supported.
	virtual bool			HandleSingleGuiCommand( idEntity* entityGui, idLexer* src );

	//! Finds and validates the target entities for this entity.
	void					FindTargets();

	//! Removes any null targets from the entity's target list.
	void					RemoveNullTargets();

	//! Activates all target entities linked to this entity by sending them trigger signals and activating associated GUIs.
	void					ActivateTargets( idEntity* activator ) const;

	//! Teleports the entity to a specified location with given orientation.
	virtual void			Teleport( const idVec3& origin, const idAngles& angles, idEntity* destination );

	//! Activates all trigger entities that are touched at the current position.
	bool					TouchTriggers() const;

	//! Returns a spline curve object based on entity spawn arguments.
	idCurve_Spline<idVec3>* GetSpline() const;

	//! Displays the editing dialog for the entity.
	virtual void			ShowEditingDialog();

	enum { EVENT_STARTSOUNDSHADER, EVENT_STOPSOUNDSHADER, EVENT_MAXEVENTS };

	//! Updates the entity's state by interpolating physics and presenting the entity to the client.
	virtual void ClientThink( const int curTime, const float fraction, const bool predict );

	//! Executes physics simulation and presentation for client-side entity prediction.
	virtual void ClientPredictionThink();

	//! Serializes the entity's data to a snapshot message.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Increments the snapshot counter for the entity and reads entity data from a snapshot message.
	void		 ReadFromSnapshot_Ex( const idBitMsg& msg );

	//! Reads entity state from a snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

	//! Handles server-side events received for this entity.
	virtual bool ServerReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Handles client-side reception of sound-related events from the server.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Writes bind information to a snapshot message
	void		 WriteBindToSnapshot( idBitMsg& msg ) const;

	//! Reads bind information from a snapshot message and applies the appropriate binding to the entity.
	void		 ReadBindFromSnapshot( const idBitMsg& msg );

	//! Writes the entity's color data to a snapshot message.
	void		 WriteColorToSnapshot( idBitMsg& msg ) const;

	//! Reads a color value from a snapshot message and updates the entity's shader parameters.
	void		 ReadColorFromSnapshot( const idBitMsg& msg );

	//! Writes the GUI state for the first GUI of the entity to a snapshot message.
	void		 WriteGUIToSnapshot( idBitMsg& msg ) const;

	//! Updates the GUI state from a network snapshot message
	void		 ReadGUIFromSnapshot( const idBitMsg& msg );

	/*!
		\brief Sends a network event to clients in a lobby, with optional saving and exclusion of a specific client.

		This function serializes and sends an event to all connected clients in the lobby, excluding a specified client if provided. It initializes a message buffer, writes the entity's spawn ID,
	   event ID, and current game time, followed by optional message data. The event is sent reliably to all peers in the lobby, unless a specific peer is excluded. If the saveEvent flag is true, the
	   event is also saved for late-joining clients.

		\param eventId The identifier of the event to send
		\param msg Optional message data to include with the event
		\param saveEvent Flag indicating whether the event should be saved for late-joining clients
		\param excluding Optional lobby user ID to exclude from receiving the event
	*/
	void		 ServerSendEvent( int eventId, const idBitMsg* msg, bool saveEvent, lobbyUserID_t excluding = lobbyUserID_t() ) const;

	//! Sends a client event to the host with the specified event ID and optional message data.
	void		 ClientSendEvent( int eventId, const idBitMsg* msg ) const;

	//! Sets whether client interpolation should be used for this entity.
	void		 SetUseClientInterpolation( bool use ) { useClientInterpolation = use; }

	//! Sets whether the entity should be skipped during replication.
	void		 SetSkipReplication( const bool skip ) { fl.skipReplication = skip; }

	//! Returns whether the entity should be skipped during network replication.
	bool		 GetSkipReplication() const { return fl.skipReplication; }

	//! Determines whether the entity is replicated across the network.
	bool		 IsReplicated() const { return GetEntityNumber() < ENTITYNUM_FIRST_NON_REPLICATED; }

	//! Calculates origin and axis deltas for smooth transition from old to new position and orientation.
	void		 CreateDeltasFromOldOriginAndAxis( const idVec3& oldOrigin, const idMat3& oldAxis );

	//! Gradually reduces the origin and axis delta values towards zero using smoothing decay.
	void		 DecayOriginAndAxisDelta();

	//! Returns the prediction key used for entity prediction.
	uint32		 GetPredictedKey() { return predictionKey; }

	//! Sets the prediction key for the entity to the specified value.
	void		 SetPredictedKey( uint32 key_ ) { predictionKey = key_; }

	//! Updates the interpolation behavior to indicate a new snapshot is available for this entity.
	void		 FlagNewSnapshot();

	//! Returns the entity that is chained to this entity's team.
	idEntity*	 GetTeamChain() { return teamChain; }

	// It is only safe to interpolate if this entity has received two snapshots.
	enum interpolationBehavior_t { USE_NO_INTERPOLATION, USE_LATEST_SNAP_ONLY, USE_INTERPOLATION };

	//! Returns the interpolation behavior of the entity.
	interpolationBehavior_t GetInterpolationBehavior() const { return interpolationBehavior; }

	//! Returns the number of snapshots received for this entity.
	unsigned int			GetNumSnapshotsReceived() const { return snapshotsReceived; }

protected:
	renderEntity_t renderEntity;   // used to present a model to the renderer
	int			   modelDefHandle; // handle to static renderer model
	refSound_t	   refSound;	   // used to present sound to the audio engine

	//! Returns the origin delta of the entity.
	idVec3		   GetOriginDelta() const { return originDelta; }

	//! Returns the axis delta of the entity as a 3x3 matrix.
	idMat3		   GetAxisDelta() const { return axisDelta; }

public:
	//! Returns the integer value of a key from the entity's spawn arguments.
	int GetIntKey( const char* key )
	{
		int value;
		spawnArgs.GetInt( key, "0", value );
		return value;
	}

	//! Returns the float value of a key from the entity's spawn arguments.
	float GetFloatKey( const char* key )
	{
		float value;
		spawnArgs.GetFloat( key, "0", value );
		return value;
	}

private:
	idPhysics_Static		defaultPhysicsObj;		 // default physics object
	idPhysics*				physics;				 // physics used for this entity
	idEntity*				bindMaster;				 // entity bound to if unequal NULL
	jointHandle_t			bindJoint;				 // joint bound to if unequal INVALID_JOINT
	int						bindBody;				 // body bound to if unequal -1
	idEntity*				teamMaster;				 // master of the physics team
	idEntity*				teamChain;				 // next entity in physics team
	bool					useClientInterpolation;	 // disables interpolation for some objects (handy for weapon world models)
	int						numPVSAreas;			 // number of renderer areas the entity covers
	int						PVSAreas[MAX_PVS_AREAS]; // numbers of the renderer areas the entity covers

	signalList_t*			signals;

	int						mpGUIState; // local cache to avoid systematic SetStateInt

	uint32					predictionKey; // Unique key used to sync predicted ents (projectiles) in MP.

	// Delta values that are set when the server or client disagree on where the render model should be. If this happens,
	// they resolve it through DecayOriginAndAxisDelta()
	idVec3					originDelta;
	idMat3					axisDelta;

	interpolationBehavior_t interpolationBehavior;
	unsigned int			snapshotsReceived;

private:
	//! Updates localized string values in the entity's spawn arguments.
	void		FixupLocalizedStrings();

	//! Determines whether the entity should transition to a dormant state based on player connectivity and PVS checks.
	bool		DoDormantTests();

	//! Initializes the default physics properties for an entity using provided origin, axis, and entity definition.
	void		InitDefaultPhysics( const idVec3& origin, const idMat3& axis, const idDeclEntityDef* def );

	//! Updates the entity's visual position from physics simulation and adjusts view angles for actors with a bind master.
	void		UpdateFromPhysics( bool moveBack );

	//! Returns the time step used for physics simulation.
	virtual int GetPhysicsTimeStep() const;

	//! Initializes binding of this entity to a master entity.
	bool		InitBind( idEntity* master );

	//! Finish the entity binding process by setting up physics, team membership, and cinematic state.
	void		FinishBind();

	//! Removes all entities bound to this object
	void		RemoveBinds();

	//! Removes the entity from its current team and updates team chain references accordingly.
	void		QuitTeam();

	//! Updates the PVS areas for the entity based on its current bounds and render information.
	void		UpdatePVSAreas();

	// events
public:
	//! Returns the global origin of this entity in world coordinates
	idVec3		   GetOrigin() const;

	//! Calculates the distance from this entity to another entity.
	float		   DistanceTo( idEntity* ent );

	//! Returns the distance from this entity's origin to the specified position.
	float		   DistanceTo( const idVec3& pos ) const;

	//! Returns the next key from the entity's spawn arguments that matches the given prefix, starting after the last matched key.
	idStr		   GetNextKey( const char* prefix, const char* lastMatch );

	//! Returns the brush origin offset vector for the entity.
	idVec3		   GetOriginBrushOffset() const;

	//! Returns the origin of the entity for editing purposes.
	virtual idVec3 GetEditOrigin() const;

	//! Returns the name of the entity as a string.
	void		   Event_GetName();

	//! Sets the name of the entity to the provided string.
	void		   Event_SetName( const char* name );

	//! Finds and sets up target entities for this entity.
	void		   Event_FindTargets();

	//! Activates any entities targeted by this entity, primarily used as an event to delay activating targets
	void		   Event_ActivateTargets( idEntity* activator );

	//! Returns the number of targets associated with this entity.
	void		   Event_NumTargets();

	//! Returns the entity at the specified index from the targets array
	void		   Event_GetTarget( float index );

	//! Returns a random target entity from the entity's target list, optionally ignoring a specified target.
	void		   Event_RandomTarget( const char* ignore );

	//! Binds this entity to a master entity.
	void		   Event_Bind( idEntity* master );

	//! Binds this entity's position to the specified master entity.
	void		   Event_BindPosition( idEntity* master );

	//! Binds this entity to a joint of the specified master entity with optional orientation.
	void		   Event_BindToJoint( idEntity* master, const char* jointname, float orientated );

	//! Removes the binding associated with the entity.
	void		   Event_Unbind();

	//! Removes all input bindings associated with the entity.
	void		   Event_RemoveBinds();

	//! Binds the entity to a parent entity, joint, or body based on spawn arguments.
	void		   Event_SpawnBind();

	//! Sets the owner for all clip models associated with the entity's physics.
	void		   Event_SetOwner( idEntity* owner );

	//! Sets the model of the entity to the specified model name.
	void		   Event_SetModel( const char* modelname );

	//! Sets the skin of the entity using the provided skin name.
	void		   Event_SetSkin( const char* skinname );

	//! Returns the value of a specified shader parameter for this entity.
	void		   Event_GetShaderParm( int parmnum );

	//! Sets a shader parameter for the entity.
	void		   Event_SetShaderParm( int parmnum, float value );

	/*!
		\brief Sets the shader parameters for the entity's renderable object with the provided color values.

		This function updates the red, green, blue, and alpha shader parameters of the entity's renderable object using the specified float values. After updating the parameters, it calls
	   UpdateVisuals to refresh the entity's appearance.

		\param parm0 The red color component value for the shader
		\param parm1 The green color component value for the shader
		\param parm2 The blue color component value for the shader
		\param parm3 The alpha color component value for the shader
	*/
	void		   Event_SetShaderParms( float parm0, float parm1, float parm2, float parm3 );

	//! Sets the color of the entity using the provided red, green, and blue values.
	void		   Event_SetColor( float red, float green, float blue );

	//! Returns the color of the entity as a vector.
	void		   Event_GetColor();

	//! Returns the hidden state of the entity.
	void		   Event_IsHidden();

	//! Hides the entity by calling the Hide method.
	void		   Event_Hide();

	//! Shows the entity.
	void		   Event_Show();

	//! Caches a sound shader by its name.
	void		   Event_CacheSoundShader( const char* soundName );

	//! Starts playing a sound shader from a sound name on the specified channel.
	void		   Event_StartSoundShader( const char* soundName, int channel );

	//! Stops sound on the specified channel with optional network synchronization.
	void		   Event_StopSound( int channel, int netSync );

	//! Starts playing a sound event on the entity with the specified parameters.
	void		   Event_StartSound( const char* soundName, int channel, int netSync );

	//! Fades the sound for the specified channel to a target volume over a given time period.
	void		   Event_FadeSound( int channel, float to, float over );

	//! Returns the world origin of the entity.
	void		   Event_GetWorldOrigin();

	//! Sets the world origin of the entity.
	void		   Event_SetWorldOrigin( idVec3 const& org );

	//! Returns the origin of the entity in local coordinates.
	void		   Event_GetOrigin();

	//! Sets the origin position of the entity to the specified vector.
	void		   Event_SetOrigin( const idVec3& org );

	//! Returns the angles of the entity's physics axis as a vector.
	void		   Event_GetAngles();

	//! Sets the angles of the entity to the specified angles.
	void		   Event_SetAngles( const idAngles& ang );

	//! Sets the linear velocity of the entity's physics object to the specified velocity vector.
	void		   Event_SetLinearVelocity( const idVec3& velocity );

	//! Returns the linear velocity of the entity's physics object.
	void		   Event_GetLinearVelocity();

	//! Sets the angular velocity of the entity's physics object to the specified velocity vector.
	void		   Event_SetAngularVelocity( const idVec3& velocity );

	//! Returns the angular velocity of the entity's physics object.
	void		   Event_GetAngularVelocity();

	//! Sets the clip box size for the entitys physics using the provided minimum and maximum bounds.
	void		   Event_SetSize( const idVec3& mins, const idVec3& maxs );

	//! Returns the size of the entity as a vector representing the difference between the bounds corners.
	void		   Event_GetSize();

	//! Returns the size of the entity as a vector representing the difference between the bounds of its physics collision volume.
	idVec3		   GetSize();

	//! Returns the minimum bounds of the entity's physics collision volume.
	void		   Event_GetMins();

	//! Returns the maximum bounds of the entity's physics collision volume.
	void		   Event_GetMaxs();

	//! Checks if this entity touches another entity by intersecting their bounding boxes.
	bool		   Touches( idEntity* ent );

	//! Returns whether this entity touches the specified entity.
	void		   Event_Touches( idEntity* ent );

	//! Sets a GUI parameter for the entity's render entity GUIs.
	void		   Event_SetGuiParm( const char* key, const char* val );

	//! Sets a float value in the GUI state for all valid GUIs associated with the entity.
	void		   Event_SetGuiFloat( const char* key, float f );

	//! Retrieves the next key from the entity's spawn arguments that matches the given prefix, starting after the last matched key.
	void		   Event_GetNextKey( const char* prefix, const char* lastMatch );

	//! Sets a key-value pair in the entity's spawn arguments.
	void		   Event_SetKey( const char* key, const char* value );

	//! Returns the string value of a spawn argument key from the entity's spawn arguments.
	void		   Event_GetKey( const char* key );

	//! Retrieves an integer value associated with a specified key from the entity's spawn arguments and returns it as a float.
	void		   Event_GetIntKey( const char* key );

	//! Retrieves a float value associated with the specified key from the entity's spawn arguments and returns it to the calling thread.
	void		   Event_GetFloatKey( const char* key );

	//! Retrieves a vector value associated with the specified key from the entity's spawn arguments and returns it to the script thread.
	void		   Event_GetVectorKey( const char* key );

	//! Returns the value of the specified entity key
	void		   Event_GetEntityKey( const char* key );

	//! Returns the entity specified by the given key in the spawn arguments.
	idEntity*	   GetEntityKey( const char* key );

	//! Restores the entity position and handles associated team members teleportation.
	void		   Event_RestorePosition();

	//! Updates the camera target for the entity.
	void		   Event_UpdateCameraTarget();

	//! Returns the distance between this entity and the specified entity.
	void		   Event_DistanceTo( idEntity* ent );

	//! Returns the distance from the entity's origin to the specified point.
	void		   Event_DistanceToPoint( const idVec3& point );

	//! Starts a particle effect specified by the given name.
	void		   Event_StartFx( const char* fx );

	//! Puts the current thread to wait for one frame.
	void		   Event_WaitFrame();

	//! Pauses the execution of the current thread for the specified time interval.
	void		   Event_Wait( float time );

	//! Checks if the entity has a script function with the specified name and returns a boolean result.
	void		   Event_HasFunction( const char* name );

	//! Calls a script function by name on the entity.
	void		   Event_CallFunction( const char* name );

	//! Sets whether the entity should never dormant.
	void		   Event_SetNeverDormant( int enable );

	//! Sets a GUI for the entity at the specified GUI number using the provided GUI name.
	void		   Event_SetGui( int guiNum, const char* guiName );

	//! Preloads a GUI by its name for use in the entity.
	void		   Event_PrecacheGui( const char* guiName );

	//! Retrieves a GUI parameter value from a specified GUI number and key.
	void		   Event_GetGuiParm( int guiNum, const char* key );

	//! Returns the float value of a GUI parameter from a specified GUI index.
	void		   Event_GetGuiParmFloat( int guiNum, const char* key );

	//! Handles a named GUI event for a specific GUI number on the entity.
	void		   Event_GuiNamedEvent( int guiNum, const char* event );
};

ID_INLINE float idEntity::DistanceTo( idEntity* ent )
{
	return DistanceTo( ent->GetPhysics()->GetOrigin() );
}

ID_INLINE float idEntity::DistanceTo( const idVec3& pos ) const
{
	return ( pos - GetPhysics()->GetOrigin() ).LengthFast();
}

/*
===============================================================================

	Animated entity base class.

===============================================================================
*/

typedef struct damageEffect_s {
	jointHandle_t		   jointNum;
	idVec3				   localOrigin;
	idVec3				   localNormal;
	int					   time;
	const idDeclParticle*  type;
	struct damageEffect_s* next;
} damageEffect_t;

/*!
	\class idAnimatedEntity
	\brief A class representing animated entities that can handle physics, animation, and damage effects.

	This class extends entity functionality to support animated models with physics simulation, joint manipulation, and visual damage effects. It provides methods for saving and restoring entity
   state, updating animation frames, retrieving joint transformations, and managing damage effects. The class supports both client-side prediction and full simulation environments, allowing for
   accurate rendering and interaction within the game world. It maintains an animator object to control the animation state and provides interfaces for setting and manipulating joint positions and
   angles. The class handles client events for damage effects and manages the visualization of damage through particle systems and material-specific effects.

*/
class idAnimatedEntity : public idEntity
{
public:
	CLASS_PROTOTYPE( idAnimatedEntity );

	//! Initializes a new instance of the idAnimatedEntity class.
	idAnimatedEntity();

	//! Destructor for idAnimatedEntity that cleans up damage effects.
	~idAnimatedEntity();

	//! Serializes the animated entity state to a save game file
	void				Save( idSaveGame* savefile ) const;

	//! Restores the animated entity state from a save game file.
	void				Restore( idRestoreGame* savefile );

	//! Executes client-side prediction logic by running physics, updating animation, and presenting the entity.
	virtual void		ClientPredictionThink();

	//! Updates the entity's animation and presentation based on the current time and prediction state.
	virtual void		ClientThink( const int curTime, const float fraction, const bool predict );

	//! Executes the animation entity's think logic by running physics, updating animation, presenting the entity, and updating damage effects.
	virtual void		Think();

	//! Updates the animation state of the entity based on the current time and animation settings.
	void				UpdateAnimation();

	//! Returns the animator associated with this animated entity.
	virtual idAnimator* GetAnimator();

	//! Sets the model for the animated entity and updates its visual representation.
	virtual void		SetModel( const char* modelname );

	/*!
		\brief Returns the world-space transformation of a specified joint at a given time

		This function retrieves the local transformation of a joint at the specified time and converts it to world-space coordinates. It uses the animator to get the joint transform and then applies
	   the local to world conversion. The function returns false if the animator cannot provide the joint transform for the given time.

		\param jointHandle The handle identifying the joint to retrieve the transformation for
		\param currentTime The time index to retrieve the joint transformation at
		\param offset Output parameter for the world-space position of the joint
		\param axis Output parameter for the world-space orientation of the joint
		\return True if the joint transformation was successfully retrieved and converted, false otherwise
	*/
	bool				GetJointWorldTransform( jointHandle_t jointHandle, int currentTime, idVec3& offset, idMat3& axis );

	/*!
		\brief Retrieves the transformation matrix and offset for a specific joint during a given animation frame.

		This function calculates the position offset and orientation axis for a specified joint at a given animation time. It first validates the animation and joint handle to ensure they are within
	   bounds. If the inputs are valid, it creates an animation frame using the game edit system and extracts the joint transformation data. The function returns false if the animation or joint handle
	   is invalid, and true upon successful extraction.

		\param jointHandle The handle identifying the joint for which the transformation is calculated
		\param animNum The animation number from which the joint frame is retrieved
		\param currentTime The time in milliseconds for which the joint frame is calculated
		\param offset The output offset vector representing the position of the joint
		\param axis The output axis matrix representing the orientation of the joint
		\return True if the joint transformation was successfully calculated, false otherwise
		\throws Assertion failure if the animation or joint handle is invalid
	*/
	bool				GetJointTransformForAnim( jointHandle_t jointHandle, int animNum, int currentTime, idVec3& offset, idMat3& axis ) const;

	//! Returns the default surface type for the animated entity.
	virtual int			GetDefaultSurfaceType() const;

	//! Adds a damage effect at the collision point with the specified velocity and damage definition.
	virtual void		AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );

	/*!
		\brief Adds a local damage effect to the animated entity at the specified joint with associated visual and audio feedback.

		This function processes a damage effect at a specific joint of the animated entity. It computes the world position and direction based on the joint and local coordinates, determines the
	   material type from the collision material, and plays appropriate sounds, blood splats, and wound overlays. It also creates a bleeding particle effect if defined for the material type.

		\param jointNum The joint number where the damage effect should be applied
		\param localPoint The local origin of the damage effect relative to the joint
		\param localNormal The local normal vector at the point of impact
		\param localDir The local direction of the impact
		\param def The entity definition containing fallback properties for the damage effect
		\param collisionMaterial The material that was hit, used to determine the type of damage effect
	*/
	void AddLocalDamageEffect( jointHandle_t jointNum, const idVec3& localPoint, const idVec3& localNormal, const idVec3& localDir, const idDeclEntityDef* def, const idMaterial* collisionMaterial );

	//! Updates and manages damage effect particles for the animated entity.
	void UpdateDamageEffects();

	//! Handles client-side events for animated entities, specifically processing damage effect additions.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	enum { EVENT_ADD_DAMAGE_EFFECT = idEntity::EVENT_MAXEVENTS, EVENT_MAXEVENTS };

protected:
	idAnimator		animator;
	damageEffect_t* damageEffects;

public:
	//! Retrieves the handle for a specified joint by its name.
	void Event_GetJointHandle( const char* jointname );

	//! Clears all custom joint transforms from the animator.
	void Event_ClearAllJoints();

	//! Clears custom transforms on the specified joint.
	void Event_ClearJoint( jointHandle_t jointnum );

	//! Sets the position of a specified joint using the given transform type.
	void Event_SetJointPos( jointHandle_t jointnum, jointModTransform_t transform_type, const idVec3& pos );

	//! Sets the joint angle for a specified joint using the given transform type and angles.
	void Event_SetJointAngle( jointHandle_t jointnum, jointModTransform_t transform_type, const idAngles& angles );

	//! Returns the position of the specified joint in worldspace
	void Event_GetJointPos( jointHandle_t jointnum );

	//! Returns the orientation of the specified joint in world space as a vector of angles.
	void Event_GetJointAngle( jointHandle_t jointnum );
};

/*!
	\class SetTimeState
	\brief Manages time group states for temporal effects in the engine.
*/
class SetTimeState
{
private:
	bool activated;
	bool previousFast;
	bool fast;

public:
	//! Initializes a new instance of the SetTimeState class with the activated flag set to false.
	SetTimeState();

	//! Constructs a SetTimeState object and initializes it with the specified time group.
	SetTimeState( int timeGroup );

	//! Destructor for the SetTimeState class that restores the previous time group if activation occurred and not in multiplayer mode.
	~SetTimeState();

	//! Pushes a time state onto the stack for the specified time group.
	void PushState( int timeGroup );
};

ID_INLINE SetTimeState::SetTimeState()
{
	activated = false;
}

ID_INLINE SetTimeState::SetTimeState( int timeGroup )
{
	activated = false;
	PushState( timeGroup );
}

ID_INLINE void SetTimeState::PushState( int timeGroup )
{
	// Don't mess with time in Multiplayer
	if( !common->IsMultiplayer() ) {
		activated = true;

		// determine previous fast setting
		if( gameLocal.time == gameLocal.slow.time ) {
			previousFast = false;
		} else {
			previousFast = true;
		}

		// determine new fast setting
		if( timeGroup ) {
			fast = true;
		} else {
			fast = false;
		}

		// set correct time
		gameLocal.SelectTimeGroup( timeGroup );
	}
}

ID_INLINE SetTimeState::~SetTimeState()
{
	if( activated && !common->IsMultiplayer() ) {
		// set previous correct time
		gameLocal.SelectTimeGroup( previousFast );
	}
}

#endif /* !__GAME_ENTITY_H__ */
