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

#ifndef __PHYSICS_STATICMULTI_H__
#define __PHYSICS_STATICMULTI_H__

/*!
	\class idPhysics_StaticMulti
	\brief A physics system for managing multiple static collision models with unified simulation properties.

	This class implements a static multi-body physics system that aggregates multiple clip models under a single physics object. It provides methods to manage individual clip models, set and retrieve
   physics properties like mass, contents, and gravity, and handle simulation state including time, origin, axis, and velocity. The system supports saving and restoring physics states, activating and
   deactivating the physics object, and linking/unlinking clip models to/from the game world. It also handles contact detection, impulse and force application, and network synchronization. The
   implementation assumes a fixed relationship between the physics object and its associated entity, and provides specific methods for setting origin, axis, and applying transformations to individual
   or all clip models.

*/
class idPhysics_StaticMulti : public idPhysics
{
public:
	CLASS_PROTOTYPE( idPhysics_StaticMulti );

	//! Initializes a new instance of the idPhysics_StaticMulti class with default values.
	idPhysics_StaticMulti();

	//! Destructor for the idPhysics_StaticMulti class that cleans up associated physics data and clip models.
	~idPhysics_StaticMulti();

	//! Saves the static multi physics state to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the static multi physics object from a save file.
	void Restore( idRestoreGame* savefile );

	//! Removes a clip model at the specified index from the static multi physics system
	void RemoveIndex( int id = 0, bool freeClipModel = true );

public: // common physics interface
		//! Sets the entity that this physics object is associated with.
	void				 SetSelf( idEntity* e );

	/*!
		\brief Sets the clip model for a specific index in the static multi-physics object, managing memory and linking the model to the game world.

		This function updates the clip model at the specified index within the physics object. It handles memory management by deleting the existing clip model if it differs from the new one and the
	   freeOld flag is true. The function also ensures proper linking of the new clip model to the game world's clip system. It adjusts the internal arrays to maintain consistent sizing and
	   initializes interpolation states for the physics simulation.

		\param model The new clip model to be assigned at the specified index
		\param density The density value used for the clip model
		\param id The index in the clip models array to update
		\param freeOld Determines whether to delete the existing clip model if it differs from the new one
		\throws assertion failure if self pointer is null
	*/
	void				 SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model at the specified index or a default clip model if the index is invalid.
	idClipModel*		 GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models contained in the static multi-physics object.
	int					 GetNumClipModels() const;

	//! Sets the mass of a specified object in the static multi physics system.
	void				 SetMass( float mass, int id = -1 );

	//! Returns the mass of the physics object, or zero if the object has no mass.
	float				 GetMass( int id = -1 ) const;

	//! Sets the contents of a clip model or all clip models in the static multi physics object.
	void				 SetContents( int contents, int id = -1 );

	//! Returns the contents mask for a specific clip model or the combined contents mask for all clip models if the id is -1.
	int					 GetContents( int id = -1 ) const;

	//! Sets the clip mask for the static multi physics object.
	void				 SetClipMask( int mask, int id = -1 );

	//! Returns the clip mask for the physics static multi object.
	int					 GetClipMask( int id = -1 ) const;

	//! Returns the bounds of a specific clip model or the combined bounds of all clip models if the id is -1.
	const idBounds&		 GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics object or a specific clip model within it
	const idBounds&		 GetAbsBounds( int id = -1 ) const;

	//! Evaluates the physics state for the static multi-body physics object based on the time step and end time.
	bool				 Evaluate( int timeStepMSec, int endTimeMSec );

	//! Interpolates the physics state of static multi-body physics objects using the specified fraction.
	bool				 Interpolate( const float fraction );

	//! Resets the interpolation state with the given origin and axis.
	void				 ResetInterpolationState( const idVec3& origin, const idMat3& axis ) { }

	//! Updates the physics time for the static multi physics object.
	void				 UpdateTime( int endTimeMSec );

	//! Returns the current simulation time for the physics object
	int					 GetTime() const;

	//! Initializes the impact information structure for a specified id and point.
	void				 GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to a specific object within the static multi physics system
	void				 ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Adds a force to the static multi physics object at the specified point.
	void				 AddForce( const int id, const idVec3& point, const idVec3& force );

	//! Activates the static multi physics object.
	void				 Activate();

	//! Puts the static multi physics object to rest.
	void				 PutToRest();

	//! Returns true if the static multi physics object is at rest.
	bool				 IsAtRest() const;

	//! Returns the start time for the rest state of the physics object.
	int					 GetRestStartTime() const;

	//! Returns false indicating that the static multi physics object is not pushable.
	bool				 IsPushable() const;

	//! Saves the current state of the static multi physics object.
	void				 SaveState();

	//! Restores the physics state for the static multi-body physics object.
	void				 RestoreState();

	//! Sets the origin of a specific clip model or all clip models in the static multi physics object.
	void				 SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of a specific clip model or rotates all clip models if id is -1.
	void				 SetAxis( const idMat3& newAxis, int id = -1 );

	//! Translates the physics object or a specific clip model by the given translation vector.
	void				 Translate( const idVec3& translation, int id = -1 );

	//! Applies a rotation to a specific or all clip models in the static multi-physics system.
	void				 Rotate( const idRotation& rotation, int id = -1 );

	//! Returns the origin position of the physics object with the specified clip model ID
	const idVec3&		 GetOrigin( int id = 0 ) const;

	//! Returns the axis of the specified clip model or the first one if the index is out of bounds.
	const idMat3&		 GetAxis( int id = 0 ) const;

	//! Sets the linear velocity of a specified object in the static multi physics system.
	void				 SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Sets the angular velocity of the physics object.
	void				 SetAngularVelocity( const idVec3& newAngularVelocity, int id = 0 );

	//! Returns the linear velocity of the physics object.
	const idVec3&		 GetLinearVelocity( int id = 0 ) const;

	//! Returns the angular velocity of the physics object.
	const idVec3&		 GetAngularVelocity( int id = 0 ) const;

	//! Sets the gravity vector for the static multi physics object.
	void				 SetGravity( const idVec3& newGravity );

	//! Returns the gravity vector used by the static multi physics object.
	const idVec3&		 GetGravity() const;

	//! Returns the gravity normal vector for the static multi physics object.
	const idVec3&		 GetGravityNormal() const;

	//! This function is called when translation clipping is attempted on a static multi physics object and currently only logs a warning.
	void				 ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;

	//! This function is called when a rotation clipping operation is performed, but currently does nothing more than warning.
	void				 ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;

	//! Returns the contents of the collision models associated with this static multi physics object.
	int					 ClipContents( const idClipModel* model ) const;

	//! Disables clipping for all clip models in the static multi physics object.
	void				 DisableClip();

	//! Enables clipping for all clip models in the static multi physics object.
	void				 EnableClip();

	//! Unlinks all clip models in the static multi physics object.
	void				 UnlinkClip();

	//! Links all clip models in the static multi physics object to the game world.
	void				 LinkClip();

	//! Evaluates contacts for the static multi physics object and returns false.
	bool				 EvaluateContacts();

	//! Returns the number of contacts for the static multi physics object.
	int					 GetNumContacts() const;

	//! Returns a const reference to the contact information at the specified index.
	const contactInfo_t& GetContact( int num ) const;

	//! Clears all contacts associated with the static multi physics object.
	void				 ClearContacts();

	//! Adds a contact entity to the static multi physics object.
	void				 AddContactEntity( idEntity* e );

	//! Removes a contact entity from the static multi physics object.
	void				 RemoveContactEntity( idEntity* e );

	//! Returns false, indicating that the static multi physics object does not have ground contacts.
	bool				 HasGroundContacts() const;

	//! Returns false indicating no entity is considered a ground entity.
	bool				 IsGroundEntity( int entityNum ) const;

	//! Returns false indicating that the specified entity and id do not represent a ground clip model.
	bool				 IsGroundClipModel( int entityNum, int id ) const;

	//! Sets the pushed state for the static multi physics object with the specified delta time.
	void				 SetPushed( int deltaTime );

	//! Returns the linear velocity of the physics object
	const idVec3&		 GetPushedLinearVelocity( const int id = 0 ) const;

	//! Returns the angular velocity of the physics object.
	const idVec3&		 GetPushedAngularVelocity( const int id = 0 ) const;

	//! Sets the master entity for this physics object, adjusting local origin and axis relative to the master.
	void				 SetMaster( idEntity* master, const bool orientated = true );

	//! Returns the blocking information for the static multi physics object.
	const trace_t*		 GetBlockingInfo() const;

	//! Returns the entity that is blocking this static multi physics object.
	idEntity*			 GetBlockingEntity() const;

	//! Returns the linear end time for the static multi physics object.
	int					 GetLinearEndTime() const;

	//! Returns the angular end time for the static multi physics object.
	int					 GetAngularEndTime() const;

	//! Writes physics state information for static multi-body entities to a network snapshot message
	void				 WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads physics state data from a snapshot message for static multi-body physics.
	void				 ReadFromSnapshot( const idBitMsg& msg );

protected:
	idEntity*												  self;		  // entity using this physics object
	idList<staticPState_t, TAG_IDLIB_LIST_PHYSICS>			  current;	  // physics state
	idList<idClipModel*, TAG_IDLIB_LIST_PHYSICS>			  clipModels; // collision model

	// States used in client-side interpolation
	idList<staticInterpolatePState_t, TAG_IDLIB_LIST_PHYSICS> previous;
	idList<staticInterpolatePState_t, TAG_IDLIB_LIST_PHYSICS> next;

	// master
	bool													  hasMaster;
	bool													  isOrientated;
};

#endif /* !__PHYSICS_STATICMULTI_H__ */
