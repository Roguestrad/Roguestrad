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

#ifndef __GAME_AFENTITY_H__
#define __GAME_AFENTITY_H__

/*
===============================================================================

idMultiModelAF

Entity using multiple separate visual models animated with a single
articulated figure. Only used for debugging!

===============================================================================
*/
const int GIB_DELAY = 200; // only gib this often to keep performace hits when blowing up several mobs

/*!
	\class idMultiModelAF
	\brief Manages a multi-model articulated figure with physics simulation and rendering capabilities.
*/
class idMultiModelAF : public idEntity
{
public:
	CLASS_PROTOTYPE( idMultiModelAF );

	//! Initializes the physics object for the multi-model articulated figure.
	void Spawn();

	//! Destructor for idMultiModelAF that cleans up entity definitions.
	~idMultiModelAF();

	//! Executes physics simulation and presents the animated multi-model articulation.
	virtual void Think();

	//! Updates and presents the visual representation of the multimodel articulated figure to the renderer.
	virtual void Present();

protected:
	idPhysics_AF physicsObj;

	//! Sets the model for a specified ID using the provided model name.
	void		 SetModelForId( int id, const idStr& modelName );

private:
	idList<idRenderModel*, TAG_AF> modelHandles;
	idList<int, TAG_AF>			   modelDefHandles;
};

/*!
	\class idChain
	\brief A physics-based chain implementation that simulates a series of linked rigid bodies.
*/
class idChain : public idMultiModelAF
{
public:
	CLASS_PROTOTYPE( idChain );

	//! Initializes the chain physics and builds the chain structure using spawn parameters.
	void Spawn();

protected:
	/*!
		\brief Builds a chain of linked bodies with specified parameters.

		Constructs a chain of physical bodies connected by constraints. Each body in the chain is represented by a trace model and linked to the next body via either a universal joint or a
	   ball-and-socket joint depending on the bindToWorld parameter. The chain hangs downward from the specified origin point.

		\param name Base name for the chain bodies and constraints
		\param origin Starting position for the chain
		\param linkLength Length of each link in the chain
		\param linkWidth Width of each link in the chain
		\param density Density of the chain links
		\param numLinks Number of links to create in the chain
		\param bindToWorld If true, the first link is fixed to the world; otherwise, links are connected via ball-and-socket joints
	*/
	void BuildChain( const idStr& name, const idVec3& origin, float linkLength, float linkWidth, float density, int numLinks, bool bindToWorld = true );
};

/*!
	\class idAFAttachment
	\brief Manages attachment entities bound to physics bodies with animation and combat functionality.

	The idAFAttachment class represents an entity that is attached to a physics body and can be animated, rendered, and participate in combat interactions. It inherits from idAnimatedEntity and
   provides functionality for managing the attachment's relationship with a body entity, handling animations, combat model linking, and forwarding physics interactions like damage, forces, and
   impulses to the attached body. The class supports saving and restoring state, hiding and showing the entity, and updating visual transforms for VR rendering when applicable. It is designed for use
   in scenarios where entities need to be dynamically bound to physics bodies while maintaining separate visual and animation properties.

*/
class idAFAttachment : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idAFAttachment );

	//! Constructs a new idAFAttachment object with default values.
	idAFAttachment();

	//! Destructor for the idAFAttachment class that cleans up sound and combat model resources.
	virtual ~idAFAttachment();

	//! Initializes the idle animation for the attachment.
	void		 Spawn();

	//! Saves the attachment object to a save game file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the attachment state from a save game file.
	void		 Restore( idRestoreGame* savefile );

	//! Sets the body entity, attach joint, and model for the attachment.
	void		 SetBody( idEntity* bodyEnt, const char* headModel, jointHandle_t attachJoint );

	//! Clears the body reference and attachment joint for this attachment.
	void		 ClearBody();

	//! Returns the body entity associated with this attachment.
	idEntity*	 GetBody() const;

	//! Executes the attachment's think logic, updating particle effects if necessary.
	virtual void Think();

	//! Hides the attachment entity and unlinks it from combat.
	virtual void Hide();

	//! Displays the attachment entity and links it to combat.
	virtual void Show();

	//! Plays the idle animation for the attachment with the specified blend time.
	void		 PlayIdleAnim( int blendTime );

	/*!
		\brief Retrieves impact information for the attachment's body or falls back to the entity's impact info if no body is present.

		This function determines the impact information at a specified point on the attachment. If the attachment has an associated physics body, it delegates the call to that body's GetImpactInfo
	   method using the joint handle converted to a clip model ID. Otherwise, it falls back to the base entity's GetImpactInfo method.

		\param ent The entity involved in the impact
		\param id The identifier for the impact
		\param point The point in world space where the impact occurs
		\param info Output structure containing the impact information
	*/
	virtual void GetImpactInfo( idEntity* ent, int id, const idVec3& point, impactInfo_t* info );

	/*!
		\brief Applies an impulse to the attachment's physics body or entity.

		This function applies a given impulse at a specified point to either the physics body associated with the attachment or to the entity directly if no physics body is present. The impulse is
	   applied using the joint handle converted to a clipmodel ID for the physics body.

		\param ent The entity that applies the impulse
		\param id The ID of the entity applying the impulse
		\param point The point in world space where the impulse is applied
		\param impulse The impulse vector to be applied
	*/
	virtual void ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );

	/*!
		\brief Adds a force to the attachment body or delegates to the entity's AddForce method if no body is present.

		The function applies a force to an attachment body when it exists, using the joint handle converted to a clip model ID for the force application. If no body is present, it falls back to the
	   standard entity AddForce method to apply the force.

		\param ent The entity applying the force
		\param id The ID of the force application
		\param point The point in world space where the force is applied
		\param force The force vector to be applied
	*/
	virtual void AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );

	/*!
		\brief Passes damage to the attached body at the bind joint.

		This function forwards damage information to the associated physics body at the joint where the attachment is bound. It checks if the body exists before applying the damage, using the provided
	   inflictor, attacker, direction, damage definition name, damage scale, and location parameters.

		\param inflictor Entity that caused the damage
		\param attacker Entity responsible for the attack
		\param dir Direction of the damage force
		\param damageDefName Name of the damage definition to apply
		\param damageScale Scale factor for the damage amount
		\param location Location index for the damage application
	*/
	virtual void Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );

	//! Adds a damage effect to the attachment body based on collision and velocity data.
	virtual void AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );

	//! Updates the combat model for the attachment.
	void		 SetCombatModel();

	//! Returns the combat model associated with this attachment.
	idClipModel* GetCombatModel() const;

	//! Links the combat model for the attachment to the game world.
	virtual void LinkCombat();

	//! Unlinks the combat model from the physics simulation.
	virtual void UnlinkCombat();

	//! Returns the transformation from physics to visual space for VR rendering when VR is active and a non-unit scale is specified.
	virtual bool GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );

protected:
	idEntity*	  body;
	idClipModel*  combatModel; // render model for hit detection of head
	int			  idleAnim;
	jointHandle_t attachJoint;
};

/*!
	\class idAFEntity_Base
	\brief Base class for entities using articulated figure physics simulation.

	Provides fundamental functionality for entities that utilize articulated figure physics, including physics initialization, animation control, collision handling, and save/load operations. The
   class serves as a foundation for entities that require complex physics-based movement and interaction with the game world. It manages the link between visual representation and physics simulation,
   handles damage effects and impulses, and supports serialization of physics states. The class supports both active physics simulation and binding constraints for static positioning, with methods to
   manage these states throughout the entity's lifecycle.

*/
class idAFEntity_Base : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idAFEntity_Base );

	//! Initializes a new instance of the idAFEntity_Base class.
	idAFEntity_Base();

	//! Destructor for the idAFEntity_Base class that cleans up the combat model.
	virtual ~idAFEntity_Base();

	//! Initializes the physics state and sound timing for the AF entity.
	void		  Spawn();

	//! Saves the entity's combat model contents, clip model, spawn origin, spawn axis, next sound time, and associated physics data to the save file.
	void		  Save( idSaveGame* savefile ) const;

	//! Restores the state of the AF entity from a save file
	void		  Restore( idRestoreGame* savefile );

	//! Executes the thinking logic for the articulated figure entity, including physics, animation, and visual updates.
	virtual void  Think();

	//! Adds a damage effect to the entity based on a collision and velocity.
	virtual void  AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );

	/*!
		\brief Retrieves impact information for a given entity at a specific point by delegating to either the active physics system or the base entity implementation

		The function determines whether the physics system is active and delegates the retrieval of impact information accordingly. If the physics system is active, it uses the physics system's
	   GetImpactInfo method; otherwise, it falls back to the base entity's GetImpactInfo method. This allows for flexible handling of impact information retrieval based on the current state of the
	   physics simulation.

		\param ent The entity retrieving the impact information
		\param id Identifier for the impact event
		\param point The point in world space where the impact occurred
		\param info Output parameter that will contain the impact information
	*/
	virtual void  GetImpactInfo( idEntity* ent, int id, const idVec3& point, impactInfo_t* info );

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
	virtual void  ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );

	/*!
		\brief Adds a force to the physics simulation for the specified entity.

		The function applies a force to the physics simulation when the physics system is loaded. If the physics system is not active, it delegates the force application to the base entity class. This
	   allows for conditional application of forces based on the state of the physics simulation.

		\param ent pointer to the entity that is applying the force
		\param id identifier for the force
		\param point the point in world space where the force is applied
		\param force the force vector to be applied
	*/
	virtual void  AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );

	//! Handles collision detection and plays a bounce sound if the velocity exceeds a minimum threshold.
	virtual bool  Collide( const trace_t& collision, const idVec3& velocity );

	//! Returns the transformation from physics to visual space for the articulated figure entity.
	virtual bool  GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );

	//! Updates the animation controllers for the physics entity and returns true if successful.
	virtual bool  UpdateAnimationControllers();

	//! Frees the model definition for the AF entity base.
	virtual void  FreeModelDef();

	//! Loads the articulated figure for the entity and initializes its physics and animation.
	virtual bool  LoadAF();

	//! Returns true if the attached physics object is active.
	bool		  IsActiveAF() const { return af.IsActive(); }

	//! Returns the name of the physics object associated with this entity.
	const char*	  GetAFName() const { return af.GetName(); }

	//! Returns the physics object associated with the articulated figure.
	idPhysics_AF* GetAFPhysics() { return af.GetPhysics(); }

	//! Sets the combat model for the entity.
	void		  SetCombatModel();

	//! Returns the combat model associated with this entity.
	idClipModel*  GetCombatModel() const;

	//! Sets the combat contents of the entity to either enabled or disabled state.
	void		  SetCombatContents( bool enable );

	//! Links the combat model for the AF entity into the game's clip system.
	virtual void  LinkCombat();

	//! Unlinks the combat model from the simulation.
	virtual void  UnlinkCombat();

	//! Returns the body index for the given clip model ID.
	int			  BodyForClipModelId( int id ) const;

	//! Saves the ragdoll pose and binding constraints state to the provided dictionary.
	void		  SaveState( idDict& args ) const;

	//! Loads the physical simulation state from the provided dictionary arguments.
	void		  LoadState( const idDict& args );

	//! Adds bind constraints to the physics simulation.
	void		  AddBindConstraints();

	//! Removes all bind constraints from the physics simulation.
	void		  RemoveBindConstraints();

	//! Displays the editing dialog for the physics entity.
	virtual void  ShowEditingDialog();

	//! Drops articulated figures from the specified entity using the given type and optional list to store results
	static void	  DropAFs( idEntity* ent, const char* type, idList<idEntity*>* list );

protected:
	idAF		 af;		  // articulated figure
	idClipModel* combatModel; // render model for hit detection
	int			 combatModelContents;
	idVec3		 spawnOrigin;	// spawn origin
	idMat3		 spawnAxis;		// rotation axis used when spawned
	int			 nextSoundTime; // next time this can make a sound

	//! Sets the position of a constraint by name.
	void		 Event_SetConstraintPosition( const char* name, const idVec3& pos );
};

/*
===============================================================================

idAFEntity_Gibbable

===============================================================================
*/

extern const idEventDef EV_Gib;
extern const idEventDef EV_Gibbed;

/*!
	\class idAFEntity_Gibbable
	\brief A class representing articulated figure entities that can be gibbed under specific conditions.

	This class extends the base articulated figure entity functionality to support gibbon behavior, allowing entities to be destroyed and replaced with smaller gib fragments when taking sufficient
   damage. The class manages the state transitions between normal operation and gibbed state, handles collision detection that can trigger gibbing, and controls the physics and visual presentation of
   both the main entity and its gibs. It integrates with the engine's save and restore systems to maintain entity state across game sessions. The entity maintains a skeleton model that is initialized
   upon creation and properly released during destruction. The class supports setting a thrown state that affects how physics bodies are configured for the entity.

*/
class idAFEntity_Gibbable : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_Gibbable );

	//! Constructs a new idAFEntity_Gibbable object with default values.
	idAFEntity_Gibbable();

	//! Destructor for the idAFEntity_Gibbable class that releases the skeleton model definition handle if it was allocated.
	~idAFEntity_Gibbable();

	//! Initializes the gibbable entity and resets its gibbed and thrown state flags.
	void		 Spawn();

	//! Saves the gibbable entity state to the provided save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the gibbon entity from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates and presents the giber entity's visual representation to the renderer.
	virtual void Present();

	/*!
		\brief Applies damage to the entity and triggers gibbing if the health drops below a threshold.

		This function processes damage applied to the entity, checking if the entity can take damage before proceeding. If the health drops below -20 and the entity is configured to gib, it triggers
	   the Gib method to destroy the entity.

		\param inflictor The entity causing the damage
		\param attacker The entity that is responsible for the damage
		\param dir The direction vector of the damage
		\param damageDefName The name of the damage definition to use
		\param damageScale A scaling factor for the damage amount
		\param location The location on the entity where the damage is applied
	*/
	virtual void Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );

	//! Sets the thrown state of the entity and configures the physics bodies accordingly.
	void		 SetThrown( bool isThrown );

	//! Handles collision events for gibbable articulated figures, triggering gibbing behavior when certain conditions are met.
	virtual bool Collide( const trace_t& collision, const idVec3& velocity );

	//! Spawns gib entities from the entity based on damage definition and direction
	virtual void SpawnGibs( const idVec3& dir, const char* damageDefName );

	bool		 IsGibbed() { return gibbed; };

protected:
	idRenderModel* skeletonModel;
	int			   skeletonModelDefHandle;
	bool		   gibbed;

	bool		   wasThrown;

	//! Causes the entity to be gibbed, applying appropriate physics and visual effects based on the damage definition.
	virtual void   Gib( const idVec3& dir, const char* damageDefName );

	//! Initializes the skeleton model for gibbable entity
	void		   InitSkeletonModel();

	//! Causes the entity to gib using the specified damage definition.
	void		   Event_Gib( const char* damageDefName );
};

/*!
	\class idAFEntity_Generic
	\brief Represents a generic articulated figure entity with configurable physics behavior.

	This class implements a generic articulated figure entity that can have physics processing enabled or disabled. The entity is initialized with physics disabled by default and can be activated to
   start physics simulation. The class manages the entity's state through spawn, think, and event handling methods. It provides functionality to save and restore the entity's state, including a flag
   that controls whether physics continues running. The entity supports activation events that initialize its physics state and velocities, making it suitable for interactive or dynamic physics-based
   objects within the simulation.

*/
class idAFEntity_Generic : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idAFEntity_Generic );

	//! Constructs an idAFEntity_Generic object with physics running disabled.
	idAFEntity_Generic();
	~idAFEntity_Generic();

	//! Initializes the physics and combat model for the entity.
	void		 Spawn();

	//! Saves the keepRunningPhysics boolean state to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the generic articulated figure entity from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the entity's state and potentially activates physics processing.
	virtual void Think();

	//! Sets the keepRunningPhysics flag to true.
	void		 KeepRunningPhysics() { keepRunningPhysics = true; }

private:
	//! Handles the activation event for a generic articulated figure entity, initializing its physics state and velocities.
	void Event_Activate( idEntity* activator );

	bool keepRunningPhysics;
};

/*!
	\class idAFEntity_WithAttachedHead
	\brief A class representing an animated entity with an attachable head that supports physics and combat interactions.

	This class extends the functionality of a basic animated entity by adding support for a detachable head that can be attached, detached, and managed independently. It handles the initialization,
   saving, and restoration of the head state, as well as integrating the head's physics and combat properties with the main entity. The class supports visual effects through overlay projection and
   provides methods for activation, gibbing, and combat model linking. It is designed to work within a physics-based simulation environment where both entity and head can be influenced by forces and
   interactions.

*/
class idAFEntity_WithAttachedHead : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idAFEntity_WithAttachedHead );

	//! Constructs a new idAFEntity_WithAttachedHead object with no attached head.
	idAFEntity_WithAttachedHead();

	//! Destructor for idAFEntity_WithAttachedHead that cleans up the attached head entity.
	~idAFEntity_WithAttachedHead();

	//! Initializes the entity with attached head by setting up the head, loading the physics asset, and configuring the combat model.
	void		 Spawn();

	//! Saves the attached head state to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the attached head from the save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes and configures the head attachment for the entity.
	void		 SetupHead();

	//! Executes the thinking logic for the entity with attached head.
	virtual void Think();

	//! Hides the entity and its attached head.
	virtual void Hide();

	//! Shows the entity and its attached head if present
	virtual void Show();

	/*!
		\brief Projects an overlay onto the entity and its attached head if present

		This function projects an overlay onto the entity using the provided origin, direction, size, and material. If the entity has an attached head, it also projects the overlay onto the head
	   entity. The overlay is typically used for visual effects like blood, damage marks, or other decals that deform with animated models.

		\param origin The origin point of the projection
		\param dir The direction of the projection
		\param size The size of the overlay
		\param material The material to use for the overlay
	*/
	virtual void ProjectOverlay( const idVec3& origin, const idVec3& dir, float size, const char* material );

	//! Links the combat model for the entity and its attached head.
	virtual void LinkCombat();

	//! Unlinks the combat model and the head attachment from the physics simulation.
	virtual void UnlinkCombat();

protected:
	//! Causes the entity to gib and hides the attached head.
	virtual void Gib( const idVec3& dir, const char* damageDefName );

public:
	idEntityPtr<idAFAttachment> head;

	//! Causes the entity to gib using the specified damage definition.
	void						Event_Gib( const char* damageDefName );

	//! Activates the entity and initializes its physics properties.
	void						Event_Activate( idEntity* activator );
};

/*!
	\class idAFEntity_Vehicle
	\brief A vehicle entity that manages player interaction, physics, and steering mechanics.
*/
class idAFEntity_Vehicle : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_Vehicle );

	//! Initializes a new instance of the idAFEntity_Vehicle class.
	idAFEntity_Vehicle();

	//! Initializes the vehicle entity by loading its physics, setting up joints, and configuring damage and smoke effects.
	void Spawn();

	//! Toggles player binding to the vehicle, either unbinding an existing player or binding a new one.
	void Use( idPlayer* player );

protected:
	idPlayer*			  player;
	jointHandle_t		  eyesJoint;
	jointHandle_t		  steeringWheelJoint;
	float				  wheelRadius;
	float				  steerAngle;
	float				  steerSpeed;
	const idDeclParticle* dustSmoke;

	//! Returns the current steering angle of the vehicle, smoothly interpolating towards the ideal steering angle based on player input.
	float				  GetSteerAngle();
};

/*!
	\class idAFEntity_VehicleSimple
	\brief Manages a simplified vehicle physics and animation system with wheel constraints.
*/
class idAFEntity_VehicleSimple : public idAFEntity_Vehicle
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleSimple );

	//! Initializes a new instance of the idAFEntity_VehicleSimple class.
	idAFEntity_VehicleSimple();

	//! Destructor for the idAFEntity_VehicleSimple class that cleans up the wheel model resources.
	~idAFEntity_VehicleSimple();

	//! Initializes the vehicle simple physics and wheel constraints.
	void		 Spawn();

	//! Updates the vehicle's physics and visual state during each game frame.
	virtual void Think();

protected:
	idClipModel*			   wheelModel;
	idAFConstraint_Suspension* suspension[4];
	jointHandle_t			   wheelJoints[4];
	float					   wheelAngles[4];
};

/*!
	\class idAFEntity_VehicleFourWheels
	\brief A four-wheel vehicle entity that manages physics and visual updates for vehicle simulation.
*/
class idAFEntity_VehicleFourWheels : public idAFEntity_Vehicle
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleFourWheels );

	//! Initializes a new instance of the idAFEntity_VehicleFourWheels class.
	idAFEntity_VehicleFourWheels();

	//! Initializes the four-wheel vehicle entity by setting up wheel bodies, joints, and steering constraints.
	void		 Spawn();

	//! Updates the vehicle's physics and visuals based on player input and wheel dynamics.
	virtual void Think();

protected:
	idAFBody*			  wheels[4];
	idAFConstraint_Hinge* steering[2];
	jointHandle_t		  wheelJoints[4];
	float				  wheelAngles[4];
};

/*!
	\class idAFEntity_VehicleSixWheels
	\brief Manages a six-wheel vehicle entity with physics and animation.
*/
class idAFEntity_VehicleSixWheels : public idAFEntity_Vehicle
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleSixWheels );

	//! Initializes a new instance of the idAFEntity_VehicleSixWheels class.
	idAFEntity_VehicleSixWheels();

	//! Initializes the six-wheel vehicle entity by loading wheel bodies, joints, and steering hinges from spawn arguments.
	void		 Spawn();

	//! Updates the vehicle's physics and animation based on player input and wheel dynamics.
	virtual void Think();

	float		 force;
	float		 velocity;
	float		 steerAngle;

private:
	idAFBody*			  wheels[6];
	idAFConstraint_Hinge* steering[4];
	jointHandle_t		  wheelJoints[6];
	float				  wheelAngles[6];
};

/*!
	\class idAFEntity_VehicleAutomated
	\brief Automated vehicle entity that follows waypoints and handles vehicle dynamics.

	This class extends the six-wheeled vehicle physics to provide automated behavior for vehicle entities. It manages waypoint-based navigation, velocity, torque, and steering speed controls. The
   entity is designed to be spawned and initialized with a target waypoint, then update its behavior through the think method to follow the designated path. The vehicle's physical properties such as
   velocity, torque, and steering speed can be adjusted through event handlers.

*/
class idAFEntity_VehicleAutomated : public idAFEntity_VehicleSixWheels
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleAutomated );

	//! Initializes the vehicle automated entity's properties and spawns it.
	void		 Spawn();

	//! Initializes the vehicle's waypoint target after spawning.
	void		 PostSpawn();

	//! Updates vehicle behavior based on waypoint navigation and steering adjustments.
	virtual void Think();

private:
	idEntity* waypoint;
	float	  steeringSpeed;
	float	  currentSteering;
	float	  idealSteering;
	float	  originHeight;

	//! Sets the velocity of the automated vehicle entity to the specified value.
	void	  Event_SetVelocity( float _velocity );

	//! Sets the torque force for the vehicle automated entity.
	void	  Event_SetTorque( float _torque );

	//! Sets the steering speed of the vehicle automated entity.
	void	  Event_SetSteeringSpeed( float _steeringSpeed );

	//! Sets the waypoint entity for the automated vehicle.
	void	  Event_SetWayPoint( idEntity* _waypoint );
};

/*!
	\class idAFEntity_SteamPipe
	\brief Represents a steam pipe entity with physics and rendering capabilities.

	This class implements a steam pipe entity that integrates physics simulation with visual rendering. It inherits from idAFEntity_Base to leverage physics and animation features. The entity is
   initialized through the Spawn method which sets up its physical properties and collision detection. The Think method handles frame-based updates for both physics and rendering. Memory management is
   handled through standard constructors and destructors, with the destructor ensuring cleanup of associated render world definitions. The class supports save and restore functionality for persistent
   state management.

*/
class idAFEntity_SteamPipe : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_SteamPipe );

	//! Constructs a new idAFEntity_SteamPipe object with default values.
	idAFEntity_SteamPipe();

	//! Destroys the steam pipe entity and frees its associated render world definition.
	~idAFEntity_SteamPipe();

	//! Initializes the steam pipe entity by loading its physics, setting up collision, and configuring force dynamics.
	void		 Spawn();

	//! Saves the steam pipe entity data to the provided save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the steam pipe entity state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Updates the steam pipe's physics and render entity each frame.
	virtual void Think();

private:
	int				 steamBody;
	float			 steamForce;
	float			 steamUpForce;
	idForce_Constant force;
	renderEntity_t	 steamRenderEntity;
	qhandle_t		 steamModelDefHandle;

	//! Initializes the steam render entity for the steam pipe.
	void			 InitSteamRenderEntity();
};

/*!
	\class idAFEntity_ClawFourFingers
	\brief A physics-based claw entity with four fingers that can be controlled through constraints and angles.

	This class represents a claw entity with four fingers that is derived from a base articulated frame entity. It manages the physics simulation of the claw and its individual finger constraints. The
   entity is initialized through the Spawn method which sets up the physics and constraints for the fingers. The Save and Restore methods handle persistence of the entity's state. Finger angles can be
   controlled via the SetFingerAngle event, and fingers can be stopped using the StopFingers event. The class uses standard initialization patterns with pointers to finger entities that are set to
   NULL by default.

*/
class idAFEntity_ClawFourFingers : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_ClawFourFingers );

	//! Initializes a new instance of the idAFEntity_ClawFourFingers class with all finger pointers set to NULL.
	idAFEntity_ClawFourFingers();

	//! Initializes the claw entity by loading its physics and setting up finger constraints.
	void Spawn();

	//! Saves the state of the claw four fingers entity to the provided save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the claw fingers from a save file
	void Restore( idRestoreGame* savefile );

private:
	idAFConstraint_Hinge* fingers[4];

	//! Sets the steer angle for all four fingers of the claw entity.
	void				  Event_SetFingerAngle( float angle );

	//! Stops the fingers of the claw by setting their steer angle to their current angle.
	void				  Event_StopFingers();
};

/**
 * idHarvestable contains all of the code required to turn an entity into a harvestable
 * entity. The entity must create an instance of this class and call the appropriate
 * interface methods at the correct time.
 */
class idHarvestable : public idEntity
{
public:
	CLASS_PROTOTYPE( idHarvestable );

	//! Initializes a new instance of the idHarvestable class.
	idHarvestable();

	//! Destructor for the idHarvestable class that cleans up the trigger object.
	~idHarvestable();

	//! Initializes the harvestable object's properties and spawn arguments.
	void Spawn();

	//! Initializes the harvestable entity with a parent entity and sets up associated properties and sounds.
	void Init( idEntity* parent );

	//! Saves the harvestable object state to a save game file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the harvestable object state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Sets the parent entity of this harvestable object.
	void SetParent( idEntity* parent );

	//! Updates the harvestable entity's state and handles giving items to the player.
	void Think();

	//! Stops any looping sound that was playing on the parent entity when the harvestable is gibbed.
	void Gib();

protected:
	idEntityPtr<idEntity>	parentEnt;
	float					triggersize;
	idClipModel*			trigger;
	float					giveDelay;
	float					removeDelay;
	bool					given;

	idEntityPtr<idPlayer>	player;
	int						startTime;

	bool					fxFollowPlayer;
	idEntityPtr<idEntityFx> fx;
	idStr					fxOrient;

protected:
	//! Initializes the burning effect for a harvestable entity and its associated head entity if present.
	void BeginBurn();

	//! Starts the FX effect for the harvestable entity.
	void BeginFX();

	//! Calculates trigger bounds for the harvestable entity based on its parent's physics bounds and a specified size.
	void CalcTriggerBounds( float size, idBounds& bounds );

	//! Sets the provided matrix to the orientation axis based on the harvestable's FX orientation settings.
	bool GetFxOrientationAxis( idMat3& mat );

	//! Creates and links a trigger clip model for the harvestable entity.
	void Event_SpawnHarvestTrigger();

	//! Handles the touch event for a harvestable entity, managing player interaction and harvesting logic.
	void Event_Touch( idEntity* other, trace_t* trace );
};

/*!
	\class idAFEntity_Harvest
	\brief Manages a harvest entity with attached head physics.

	This class extends idAFEntity_WithAttachedHead to provide functionality for handling harvest entities that are attached to a head structure. It manages the lifecycle of the harvest entity,
   including spawning, saving, restoring, and destruction. The class delegates most of its core logic to the base class while adding specific behavior for harvest entity operations such as gibbing.
   The harvest entity is initialized during spawning and can be persisted across save/load operations. The class ensures proper cleanup of the harvest entity when destroyed.

*/
class idAFEntity_Harvest : public idAFEntity_WithAttachedHead
{
public:
	CLASS_PROTOTYPE( idAFEntity_Harvest );

	//! Constructs a new idAFEntity_Harvest object with harvestEnt initialized to NULL.
	idAFEntity_Harvest();

	//! Destroys the idAFEntity_Harvest object and removes the associated harvest entity if it exists.
	~idAFEntity_Harvest();

	//! Initializes the harvest entity by posting a spawn event.
	void		 Spawn();

	//! Saves the harvest entity data to the specified save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the harvest entity state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Executes the think logic for the harvest entity, delegating to the base class implementation.
	virtual void Think();

	//! Causes the harvest entity to gib and performs the standard gib behavior for the attached head entity.
	virtual void Gib( const idVec3& dir, const char* damageDefName );

protected:
	idEntityPtr<idHarvestable> harvestEnt;

protected:
	//! Spawns a harvest entity based on the defined type and initializes it.
	void Event_SpawnHarvestEntity();
};

#endif /* !__GAME_AFENTITY_H__ */
