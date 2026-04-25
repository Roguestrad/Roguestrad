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

#ifndef __PHYSICS_BASE_H__
#define __PHYSICS_BASE_H__

/*
===============================================================================

	Physics base for a moving object using one or more collision models.

===============================================================================
*/

#define contactEntity_t idEntityPtr<idEntity>

/*!
	\class idPhysics_Base
	\brief Base class for physics objects providing core physics simulation functionality.

	This class serves as the foundation for physics objects within the engine, offering essential methods for managing collision models, physical properties, and simulation state. It handles core
   physics operations such as setting and retrieving mass, contents, gravity, and velocity, while providing interfaces for collision detection and contact management. The class supports multiple clip
   models and offers methods for activating, deactivating, and interpolating physics states. It also manages entity relationships through master entities and provides mechanisms for saving and
   restoring physics state during game sessions. The implementation is designed to be inherited by specific physics types, with the base class handling common functionality and state management.

*/
class idPhysics_Base : public idPhysics
{
public:
	CLASS_PROTOTYPE( idPhysics_Base );

	//! Initializes a new instance of the idPhysics_Base class.
	idPhysics_Base();

	//! Destroys the physics object and cleans up associated resources.
	~idPhysics_Base();

	//! Saves the physics base state to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the physics state from a saved game file.
	void Restore( idRestoreGame* savefile );

public:
	// common physics interface ----------------------

	//! Sets the entity that this physics object is associated with.
	void				 SetSelf( idEntity* e );

	/*!
		\brief Sets the collision model for this physics object with specified density, ID, and cleanup behavior.

		This function assigns a clip model to the physics object, allowing it to participate in collision detection. The density parameter determines the physical properties of the object, while the
	   ID provides a way to identify the model. The freeOld parameter controls whether the previous clip model should be freed if one exists.

		\param model The clip model to assign to this physics object
		\param density The density value used to calculate physical properties of the object
		\param id An identifier for the clip model
		\param freeOld Specifies whether to free the existing clip model if one exists
	*/
	void				 SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model associated with the specified ID, or NULL if not found.
	idClipModel*		 GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models associated with this physics base object.
	int					 GetNumClipModels() const;

	//! Sets the mass of the physics object or a specific part of it.
	void				 SetMass( float mass, int id = -1 );

	//! Returns the mass of the physics object or a specified part.
	float				 GetMass( int id = -1 ) const;

	//! Sets the contents of the physics base object.
	void				 SetContents( int contents, int id = -1 );

	//! Returns the contents of a physics object, optionally identified by an index.
	int					 GetContents( int id = -1 ) const;

	//! Sets the collision mask used for clipping.
	void				 SetClipMask( int mask, int id = -1 );

	//! Returns the collision clip mask for the physics base object.
	int					 GetClipMask( int id = -1 ) const;

	//! Returns the bounding box of the physics object.
	const idBounds&		 GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics object
	const idBounds&		 GetAbsBounds( int id = -1 ) const;

	//! Evaluates the physics state for a given time step and end time.
	bool				 Evaluate( int timeStepMSec, int endTimeMSec );

	//! Returns false, indicating that interpolation is not supported or implemented.
	bool				 Interpolate( const float fraction );

	//! Resets the interpolation state with the given origin and axis.
	void				 ResetInterpolationState( const idVec3& origin, const idMat3& axis );

	//! Updates the physics simulation time to the specified end time in milliseconds.
	void				 UpdateTime( int endTimeMSec );

	//! Returns the current physics simulation time.
	int					 GetTime() const;

	//! Initializes the impact info structure for a given object id and point.
	void				 GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to the physics object at a specified point.
	void				 ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Adds a force to the physics object at a specified point.
	void				 AddForce( const int id, const idVec3& point, const idVec3& force );

	//! Activates the physics base object.
	void				 Activate();

	//! Puts the physics object to rest.
	void				 PutToRest();

	//! Returns true if the physics object is at rest.
	bool				 IsAtRest() const;

	//! Returns the start time for the rest state.
	int					 GetRestStartTime() const;

	//! Returns true if the physics object is pushable.
	bool				 IsPushable() const;

	//! Saves the current state of the physics base.
	void				 SaveState();

	//! Restores the physics state to its previous condition.
	void				 RestoreState();

	//! Sets the origin of the physics object to the specified position.
	void				 SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the physics base object to the specified matrix.
	void				 SetAxis( const idMat3& newAxis, int id = -1 );

	//! Translates the physics object by the specified vector.
	void				 Translate( const idVec3& translation, int id = -1 );

	//! Rotates the physics object by the specified rotation.
	void				 Rotate( const idRotation& rotation, int id = -1 );

	//! Returns the origin point of the physics object
	const idVec3&		 GetOrigin( int id = 0 ) const;

	//! Returns the identity matrix as the axis for the physics base object.
	const idMat3&		 GetAxis( int id = 0 ) const;

	//! Sets the linear velocity of the physics object.
	void				 SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Sets the angular velocity of the physics object.
	void				 SetAngularVelocity( const idVec3& newAngularVelocity, int id = 0 );

	//! Returns the linear velocity of the physics object at the specified index.
	const idVec3&		 GetLinearVelocity( int id = 0 ) const;

	//! Returns the angular velocity of the physics object.
	const idVec3&		 GetAngularVelocity( int id = 0 ) const;

	//! Sets the gravity vector and normalizes it for use in physics calculations.
	void				 SetGravity( const idVec3& newGravity );

	//! Returns the gravity vector used by the physics system.
	const idVec3&		 GetGravity() const;

	//! Returns the normalized gravity vector for this physics object.
	const idVec3&		 GetGravityNormal() const;

	//! Initializes the trace results structure for translation clipping operations.
	void				 ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;

	//! Initializes the trace results for a rotation clipping operation.
	void				 ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;

	//! Returns the contents mask for the given clip model.
	int					 ClipContents( const idClipModel* model ) const;

	//! Disables clipping for the physics base object.
	void				 DisableClip();

	//! Enables clipping for the physics base object.
	void				 EnableClip();

	//! Unlinks the physics object from the clip model.
	void				 UnlinkClip();

	//! Links the physics base to the clip world.
	void				 LinkClip();

	//! Returns false indicating no contacts were evaluated.
	bool				 EvaluateContacts();

	//! Returns the number of contacts associated with this physics base object.
	int					 GetNumContacts() const;

	//! Returns a reference to the contact information at the specified index.
	const contactInfo_t& GetContact( int num ) const;

	//! Clears all contact entities associated with this physics object.
	void				 ClearContacts();

	//! Adds an entity to the list of contact entities.
	void				 AddContactEntity( idEntity* e );

	//! Removes a contact entity from the physics system.
	void				 RemoveContactEntity( idEntity* e );

	//! Determines if the physics base object has any ground contacts.
	bool				 HasGroundContacts() const;

	//! Determines if the specified entity is currently a ground entity for this physics object.
	bool				 IsGroundEntity( int entityNum ) const;

	//! Checks if a specified clip model is colliding with the ground.
	bool				 IsGroundClipModel( int entityNum, int id ) const;

	//! Sets the pushed state of the physics base with the specified delta time.
	void				 SetPushed( int deltaTime );

	//! Returns the pushed linear velocity of the physics object.
	const idVec3&		 GetPushedLinearVelocity( const int id = 0 ) const;

	//! Returns the angular velocity of the physics object.
	const idVec3&		 GetPushedAngularVelocity( const int id = 0 ) const;

	//! Sets the master entity for this physics object, with an optional orientation flag.
	void				 SetMaster( idEntity* master, const bool orientated = true );

	//! Returns information about the last trace that blocked this physics object.
	const trace_t*		 GetBlockingInfo() const;

	//! Returns the entity that is currently blocking this physics object.
	idEntity*			 GetBlockingEntity() const;

	//! Returns the end time for linear motion.
	int					 GetLinearEndTime() const;

	//! Returns the end time of the angular motion.
	int					 GetAngularEndTime() const;

	//! Writes physics state to a snapshot message.
	void				 WriteToSnapshot( idBitMsg& msg ) const;

	//! Deserializes physics state from a snapshot message.
	void				 ReadFromSnapshot( const idBitMsg& msg );

protected:
	idEntity*										self;			 // entity using this physics object
	int												clipMask;		 // contents the physics object collides with
	idVec3											gravityVector;	 // direction and magnitude of gravity
	idVec3											gravityNormal;	 // normalized direction of gravity
	idList<contactInfo_t, TAG_IDLIB_LIST_PHYSICS>	contacts;		 // contacts with other physics objects
	idList<contactEntity_t, TAG_IDLIB_LIST_PHYSICS> contactEntities; // entities touching this physics object

protected:
	//! Adds ground contacts for the specified clip model.
	void AddGroundContacts( const idClipModel* clipModel );

	//! Adds contact entity links to entities that are in contact with this physics object.
	void AddContactEntitiesForContacts();

	//! Activates all entities that are in contact with this physics object.
	void ActivateContactEntities();

	//! Returns true if the physics object is completely outside the world bounds.
	bool IsOutsideWorld() const;

	//! Draws the linear and angular velocity of a physics object with specified scales
	void DrawVelocity( int id, float linearScale, float angularScale ) const;
};

/*
===============================================================================
Physics interpolation state
===============================================================================
*/
struct physicsInterpolationState_t {
	//! Initializes a physics interpolation state with zero origin and identity axis.
	physicsInterpolationState_t() :
		origin( 0.0f, 0.0f, 0.0f ),
		axis( 0.0f, 0.0f, 0.0f, 1.0f )
	{
	}

	idVec3 origin;
	idQuat axis;
};

/*!
	\brief Updates the origin and axis of stateToUpdate by interpolating between previous and next states based on the given fraction

	The function performs linear interpolation on the origin vector and spherical linear interpolation on the rotation axis to compute the new state. It returns true if either the origin or axis has
   changed, indicating that the state was modified, and false if no changes occurred

	\param stateToUpdate Reference to the physics state that will be updated with interpolated values
	\param previous Physics state representing the starting point for interpolation
	\param next Physics state representing the ending point for interpolation
	\param fraction Interpolation factor between 0 and 1, where 0 returns previous state and 1 returns next state
	\return True if the origin or axis of stateToUpdate was modified, false if no changes occurred
*/
template<class _stateType_>
bool InterpolatePhysicsState( _stateType_& stateToUpdate, const physicsInterpolationState_t& previous, const physicsInterpolationState_t& next, const float fraction )
{
	const idVec3 oldOrigin = stateToUpdate.origin;
	const idMat3 oldAxis   = stateToUpdate.axis;

	stateToUpdate.origin = Lerp( previous.origin, next.origin, fraction );

	const idQuat currentQuat = Slerp( previous.axis, next.axis, fraction );
	stateToUpdate.axis		 = currentQuat.ToMat3();

	return ( stateToUpdate.origin != oldOrigin || stateToUpdate.axis != oldAxis );
}

#endif /* !__PHYSICS_BASE_H__ */
