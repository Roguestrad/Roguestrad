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

#ifndef __PHYSICS_RIGIDBODY_H__
#define __PHYSICS_RIGIDBODY_H__

/*
===================================================================================

	Rigid body physics

	Employs an impulse based dynamic simulation which is not very accurate but
	relatively fast and still reliable due to the continuous collision detection.

===================================================================================
*/

extern const float RB_VELOCITY_MAX;
extern const int   RB_VELOCITY_TOTAL_BITS;
extern const int   RB_VELOCITY_EXPONENT_BITS;
extern const int   RB_VELOCITY_MANTISSA_BITS;

typedef struct rididBodyIState_s {
	idVec3 position;		// position of trace model
	idMat3 orientation;		// orientation of trace model
	idVec3 linearMomentum;	// translational momentum relative to center of mass
	idVec3 angularMomentum; // rotational momentum relative to center of mass

	//! Initializes a rigid body state with zero position, identity orientation, and zero momentum vectors.
	rididBodyIState_s() :
		position( vec3_zero ),
		orientation( mat3_identity ),
		linearMomentum( vec3_zero ),
		angularMomentum( vec3_zero )
	{
	}
} rigidBodyIState_t;

typedef struct rigidBodyPState_s {
	int				  atRest;		  // set when simulation is suspended
	float			  lastTimeStep;	  // length of last time step
	idVec3			  localOrigin;	  // origin relative to master
	idMat3			  localAxis;	  // axis relative to master
	idVec6			  pushVelocity;	  // push velocity
	idVec3			  externalForce;  // external force relative to center of mass
	idVec3			  externalTorque; // external torque relative to center of mass
	rigidBodyIState_t i;			  // state used for integration

	//! Initializes a rigidBodyPState_s object with default values.
	rigidBodyPState_s() :
		atRest( true ),
		lastTimeStep( 0 ),
		localOrigin( vec3_zero ),
		localAxis( mat3_identity ),
		pushVelocity( vec6_zero ),
		externalForce( vec3_zero ),
		externalTorque( vec3_zero )
	{
	}
} rigidBodyPState_t;

/*!
	\class idPhysics_RigidBody
	\brief Rigid body physics class that manages physics simulation, collision detection, and state management for dynamic objects.

	This class implements physics behavior for rigid body objects, handling simulation, collision detection, and state management. It supports various physics properties such as mass, friction,
   bounciness, and contact handling. The class provides methods for setting and retrieving physical properties, applying forces and impulses, managing collision models, and integrating physics state
   over time. It also supports network state synchronization, interpolation for multiplayer clients, and debug visualization capabilities. The implementation handles both active and resting states,
   supports master/entity relationships for hierarchical movement, and provides mechanisms for saving and restoring physics states. The class manages collision detection through clip models and
   ensures proper integration of physics calculations including translation, rotation, and contact forces.

*/
class idPhysics_RigidBody : public idPhysics_Base
{
public:
	CLASS_PROTOTYPE( idPhysics_RigidBody );

	//! Initializes a new instance of the rigid body physics class with default properties.
	idPhysics_RigidBody();

	//! Destroys the rigid body physics object and cleans up associated resources.
	~idPhysics_RigidBody();

	//! Saves the rigid body physics state and related properties to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the rigid body physics state from a save file
	void Restore( idRestoreGame* savefile );

	//! Sets the linear, angular, and contact friction values for the rigid body physics.
	void SetFriction( const float linear, const float angular, const float contact );

	//! Sets the bouncyness property of the rigid body physics object to the specified value.
	void SetBouncyness( const float b );

	//! Sets flags to drop the rigid body to the floor and test for solid collision.
	void DropToFloor();

	//! Sets the physics rigid body to no contact mode.
	void NoContact();

	//! Enables activation of the rigid body by impact events.
	void EnableImpact();

	//! Disables the impact detection for the rigid body physics object.
	void DisableImpact();

public: // common physics interface
		/*!
			\brief Sets the collision model for a rigid body physics object with specified density and optional parameters.
	
			Configures the collision model for a rigid body physics object, calculating mass properties from the provided trace model and initializing the physics state. The function handles memory
		   management for the previous collision model if specified. It validates the mass properties and warns about unbalanced inertia tensors. The mass properties are used to compute inverse mass and
		   inverse inertia tensor for physics calculations.
	
			\param model Pointer to the collision model to be used for this rigid body
			\param density Density value used to calculate mass properties from the trace model
			\param id Identifier for the collision model, default is 0
			\param freeOld Boolean flag indicating whether to free the previous collision model, default is true
			\throws Assertion failures if self or model are null, if model is not a trace model, or if density is not positive
		*/
	void			SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model associated with this rigid body physics object.
	idClipModel*	GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models associated with this rigid body physics object.
	int				GetNumClipModels() const;

	//! Sets the mass of the rigid body, adjusting the inertia tensor accordingly.
	void			SetMass( float mass, int id = -1 );

	//! Returns the mass of the rigid body or a specific part if an id is provided.
	float			GetMass( int id = -1 ) const;

	//! Sets the contents of the physics rigid body's collision model.
	void			SetContents( int contents, int id = -1 );

	//! Returns the contents of the collision model associated with this rigid body physics object
	int				GetContents( int id = -1 ) const;

	//! Returns the bounding box of the physics rigid body
	const idBounds& GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics rigid body
	const idBounds& GetAbsBounds( int id = -1 ) const;

	//! Evaluates rigid body physics for a specified time interval, handling collisions and updating the body's state.
	bool			Evaluate( int timeStepMSec, int endTimeMSec );

	//! Interpolates between snapshots of the rigid body state for MP clients.
	bool			Interpolate( const float fraction );

	//! Resets the interpolation state by setting both previous and next states to the current state.
	void			ResetInterpolationState( const idVec3& origin, const idMat3& axis );

	//! Updates the physics simulation time for the rigid body up to the specified end time in milliseconds.
	void			UpdateTime( int endTimeMSec );

	//! Returns the current game time value.
	int				GetTime() const;

	//! Retrieves impact information for a specified id and point.
	void			GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to the rigid body at a specified point.
	void			ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Adds an external force and torque to the rigid body at a specified point.
	void			AddForce( const int id, const idVec3& point, const idVec3& force );

	//! Activates the rigid body physics object.
	void			Activate();

	//! Puts the rigid body physics object to rest until a collision occurs.
	void			PutToRest();

	//! Determines whether the rigid body physics object is at rest.
	bool			IsAtRest() const;

	//! Returns the time when the rigid body started resting.
	int				GetRestStartTime() const;

	//! Returns true if the rigid body can be pushed by impacts and forces.
	bool			IsPushable() const;

	//! Saves the current physics state to the saved state.
	void			SaveState();

	//! Restores the rigid body physics state to the previously saved configuration.
	void			RestoreState();

	//! Sets the origin of the rigid body, optionally updating its position relative to a master object.
	void			SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the rigid body physics object, optionally relative to a master object.
	void			SetAxis( const idMat3& newAxis, int id = -1 );

	//! Translates the rigid body by the specified translation vector.
	void			Translate( const idVec3& translation, int id = -1 );

	//! Applies a rotation to the rigid body's orientation and position, updating its clip model and activating it.
	void			Rotate( const idRotation& rotation, int id = -1 );

	//! Returns the origin position of the rigid body physics object
	const idVec3&	GetOrigin( int id = 0 ) const;

	//! Returns the orientation axis of the rigid body physics object.
	const idMat3&	GetAxis( int id = 0 ) const;

	//! Sets the linear velocity of the rigid body
	void			SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Sets the angular velocity of the rigid body.
	void			SetAngularVelocity( const idVec3& newAngularVelocity, int id = 0 );

	//! Returns the linear velocity of the rigid body at the specified index.
	const idVec3&	GetLinearVelocity( int id = 0 ) const;

	//! Returns the angular velocity of the rigid body.
	const idVec3&	GetAngularVelocity( int id = 0 ) const;

	//! Performs translation collision detection for a rigid body, using either a specified clip model or the body's own model.
	void			ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;

	//! Performs rotation clipping using either a specified collision model or the physics object's own collision model.
	void			ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;

	//! Returns the content bits for the rigid body's collision model, optionally clipped against a specified model.
	int				ClipContents( const idClipModel* model ) const;

	//! Disables the clip model associated with this rigid body physics object.
	void			DisableClip();

	//! Enables the clip model associated with this rigid body physics object.
	void			EnableClip();

	//! Unlinks the collision model from the clip system.
	void			UnlinkClip();

	//! Links the rigid body's clip model into the game's collision system.
	void			LinkClip();

	//! Evaluates contacts for the rigid body physics object and returns whether any contacts were found.
	bool			EvaluateContacts();

	//! Updates the pushed velocity of the rigid body based on the change in position and orientation over the specified time delta.
	void			SetPushed( int deltaTime );

	//! Returns the linear velocity of the rigid body at the specified index.
	const idVec3&	GetPushedLinearVelocity( const int id = 0 ) const;

	//! Returns the pushed angular velocity of the rigid body
	const idVec3&	GetPushedAngularVelocity( const int id = 0 ) const;

	//! Sets the master entity for this rigid body physics object, establishing a hierarchical relationship.
	void			SetMaster( idEntity* master, const bool orientated );

	//! Writes the rigid body physics state to a network snapshot message.
	void			WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads rigid body physics state from a snapshot message
	void			ReadFromSnapshot( const idBitMsg& msg );

private:
	// state of the rigid body
	rigidBodyPState_t current;
	rigidBodyPState_t saved;

	// states for client interpolation
	rigidBodyPState_t previous;
	rigidBodyPState_t next;

	// rigid body properties
	float			  linearFriction;  // translational friction
	float			  angularFriction; // rotational friction
	float			  contactFriction; // friction with contact surfaces
	float			  bouncyness;	   // bouncyness
	idClipModel*	  clipModel;	   // clip model used for collision detection

	// derived properties
	float			  mass;					// mass of body
	float			  inverseMass;			// 1 / mass
	idVec3			  centerOfMass;			// center of mass of trace model
	idMat3			  inertiaTensor;		// mass distribution
	idMat3			  inverseInertiaTensor; // inverse inertia tensor

	idODE*			  integrator;  // integrator
	bool			  dropToFloor; // true if dropping to the floor and putting to rest
	bool			  testSolid;   // true if testing for solid when dropping to the floor
	bool			  noImpact;	   // if true do not activate when another object collides
	bool			  noContact;   // if true do not determine contacts and no contact friction

	// master
	bool			  hasMaster;
	bool			  isOrientated;

private:
	friend void RigidBodyDerivatives( const float t, const void* clientData, const float* state, float* derivatives );

	//! Integrates the rigid body physics state forward in time using the provided time step and integrator.
	void		Integrate( const float deltaTime, rigidBodyPState_t& next );

	//! Checks for collisions between the current and next rigid body states and updates the next state to the collision impact position if a collision occurs.
	bool		CheckForCollisions( const float deltaTime, rigidBodyPState_t& next, trace_t& collision );

	//! Calculates the collision impulse for a rigid body based on the collision trace and updates the body's momentum
	bool		CollisionImpulse( const trace_t& collision, idVec3& impulse );

	//! Applies contact friction to the rigid body based on the given time delta.
	void		ContactFriction( float deltaTime );

	//! Drops the rigid body object straight down to the floor and verifies if the object is at rest on the floor.
	void		DropToFloorAndRest();

	//! Returns true if the rigid body is considered to be at rest based on velocity and contact constraints.
	bool		TestIfAtRest() const;

	//! Sets the rigid body physics object to a resting state.
	void		Rest();

	//! Draws debug information for the rigid body physics object.
	void		DebugDraw();
};

#endif /* !__PHYSICS_RIGIDBODY_H__ */
