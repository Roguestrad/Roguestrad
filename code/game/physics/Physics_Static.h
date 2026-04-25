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

#ifndef __PHYSICS_STATIC_H__
#define __PHYSICS_STATIC_H__

/*
===============================================================================

	Physics for a non moving object using at most one collision model.

===============================================================================
*/

class idBitMsg;

typedef struct staticPState_s {
	idVec3 origin;
	idMat3 axis;
	idVec3 localOrigin;
	idMat3 localAxis;
} staticPState_t;

// Storing the state used for interpolation with quaternions
// means I don't have to do a bunch of conversions between
// idMat3s and idQuats every frame.
struct staticInterpolatePState_t {
	idVec3 origin;
	idQuat axis;
	idVec3 localOrigin;
	idQuat localAxis;
};

//! Reads and returns a static interpolate physics state from a bit message snapshot.
staticInterpolatePState_t ReadStaticInterpolatePStateFromSnapshot( const idBitMsg& msg );

//! Converts a static interpolate physics state to a static physics state.
staticPState_s			  ConvertInterpolateStateToPState( const staticInterpolatePState_t& interpolateState );

//! Converts a static particle state to an interpolate particle state.
staticInterpolatePState_t ConvertPStateToInterpolateState( const staticPState_t& state );

/*!
	\class idPhysics_Static
	\brief Represents a static physics object that maintains fixed position and orientation in the game world.

	This class implements physics behavior for static objects that do not move or change shape during gameplay. It provides methods for collision detection, state management, and integration with the
   game's physics simulation. The object maintains a link to a collision model and can be associated with an entity. It supports saving and restoring of state, and provides mechanisms for handling
   forces, impulses, and contact detection. The class is designed to work with a master entity for orientation and positioning, and provides interpolation support for smooth rendering during network
   play.

*/
class idPhysics_Static : public idPhysics
{
public:
	CLASS_PROTOTYPE( idPhysics_Static );

	//! Initializes a new instance of the idPhysics_Static class.
	idPhysics_Static();

	//! Destructor for the idPhysics_Static class that cleans up physics resources and clears references.
	~idPhysics_Static();

	//! Saves the static physics state to a save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the static physics state from a save file.
	void Restore( idRestoreGame* savefile );

public: // common physics interface
		//! Sets the entity that this physics object is associated with.
	void				 SetSelf( idEntity* e );

	/*!
		\brief Sets the collision model for the static physics object with optional density and cleanup settings.

		This function assigns a new collision model to the static physics object. If a previous collision model exists and differs from the new one, it can be deleted based on the freeOld parameter.
	   The function also links the new collision model to the game world using the current origin and axis of the object.

		\param model The new collision model to assign to this physics object
		\param density The density value to use for the collision model (not used in this implementation)
		\param id An identifier for the collision model (not used in this implementation)
		\param freeOld If true, the old collision model is deleted when replaced
		\throws assertion failure if self is null
	*/
	void				 SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model associated with this static physics object, or a default clip model if none is set.
	idClipModel*		 GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models associated with this static physics object.
	int					 GetNumClipModels() const;

	//! Sets the mass of the static physics object.
	void				 SetMass( float mass, int id = -1 );

	//! Returns the mass of the static physics object, which is always zero.
	float				 GetMass( int id = -1 ) const;

	//! Sets the contents of the physics static object's collision model.
	void				 SetContents( int contents, int id = -1 );

	//! Returns the contents bit mask of the collision model associated with this static physics object
	int					 GetContents( int id = -1 ) const;

	//! Sets the clip mask for the static physics object.
	void				 SetClipMask( int mask, int id = -1 );

	//! Returns the clip mask for the static physics object.
	int					 GetClipMask( int id = -1 ) const;

	//! Returns the bounding box of the static physics object or an empty bounds if no clip model is present
	const idBounds&		 GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics object.
	const idBounds&		 GetAbsBounds( int id = -1 ) const;

	//! Evaluates the physics state of a static object, updating its position and orientation based on its master object.
	bool				 Evaluate( int timeStepMSec, int endTimeMSec );

	//! Updates the physics state by interpolating between previous and next snapshots using the given fraction.
	bool				 Interpolate( const float fraction );

	//! Resets the interpolation state of the static physics object to the specified origin and axis.
	void				 ResetInterpolationState( const idVec3& origin, const idMat3& axis ) { }

	//! Updates the physics simulation time for the static physics object.
	void				 UpdateTime( int endTimeMSec );

	//! Returns the current time value used for physics simulation.
	int					 GetTime() const;

	//! Initializes the impact information structure for a given point and impact id.
	void				 GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to the static physics object at a specified point.
	void				 ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Adds a force to the static physics object at a specified point.
	void				 AddForce( const int id, const idVec3& point, const idVec3& force );

	//! Activates the static physics object.
	void				 Activate();

	//! Puts the static physics object to rest.
	void				 PutToRest();

	//! Returns true if the static physics object is at rest.
	bool				 IsAtRest() const;

	//! Returns the start time for the rest state of the static physics object.
	int					 GetRestStartTime() const;

	//! Returns false indicating that static physics objects are not pushable.
	bool				 IsPushable() const;

	//! Saves the current state of the static physics object.
	void				 SaveState();

	//! Restores the physics state for a static entity.
	void				 RestoreState();

	//! Sets the origin of the static physics object, optionally relative to a master object.
	void				 SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the static physics object, optionally using a master object for orientation.
	void				 SetAxis( const idMat3& newAxis, int id = -1 );

	//! Moves the static physics object by the specified translation vector.
	void				 Translate( const idVec3& translation, int id = -1 );

	//! Applies a rotation to the static physics object, updating its orientation and position.
	void				 Rotate( const idRotation& rotation, int id = -1 );

	//! Returns the origin position of the static physics object.
	const idVec3&		 GetOrigin( int id = 0 ) const;

	//! Returns the axis of the physics static object.
	const idMat3&		 GetAxis( int id = 0 ) const;

	//! Sets the linear velocity of the static physics object.
	void				 SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Sets the angular velocity of the static physics object.
	void				 SetAngularVelocity( const idVec3& newAngularVelocity, int id = 0 );

	//! Returns the linear velocity of the static physics object, which is always zero.
	const idVec3&		 GetLinearVelocity( int id = 0 ) const;

	//! Returns the angular velocity of the static physics object.
	const idVec3&		 GetAngularVelocity( int id = 0 ) const;

	//! Sets the gravity vector for the static physics object.
	void				 SetGravity( const idVec3& newGravity );

	//! Returns the gravity vector used by the static physics object.
	const idVec3&		 GetGravity() const;

	//! Returns the gravity normal vector for static physics.
	const idVec3&		 GetGravityNormal() const;

	//! Performs translation clipping for a static physics object using the provided clip model or default collision detection.
	void				 ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;

	//! Performs a rotational collision test using the provided rotation and clip model.
	void				 ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;

	//! Returns the contents of the static physics object at the specified clip model's position.
	int					 ClipContents( const idClipModel* model ) const;

	//! Disables the collision clipping for the static physics object.
	void				 DisableClip();

	//! Enables the collision model for the static physics object.
	void				 EnableClip();

	//! Unlinks the collision model from the physics system.
	void				 UnlinkClip();

	//! Links the physics static clip model to the game clip system.
	void				 LinkClip();

	//! Returns false indicating that contacts are not evaluated for static physics objects.
	bool				 EvaluateContacts();

	//! Returns the number of contacts for the static physics object.
	int					 GetNumContacts() const;

	//! Returns a contact information structure for the specified contact index.
	const contactInfo_t& GetContact( int num ) const;

	//! Clears all contacts associated with this static physics object.
	void				 ClearContacts();

	//! Adds a contact entity to the static physics object.
	void				 AddContactEntity( idEntity* e );

	//! Removes the specified entity from the list of contact entities.
	void				 RemoveContactEntity( idEntity* e );

	//! Returns false indicating that the static physics object has no ground contacts.
	bool				 HasGroundContacts() const;

	//! Returns false indicating no entity is considered a ground entity.
	bool				 IsGroundEntity( int entityNum ) const;

	//! Returns false indicating that the specified entity and id do not represent a ground clip model.
	bool				 IsGroundClipModel( int entityNum, int id ) const;

	//! Sets the pushed state of the static physics object with the specified delta time.
	void				 SetPushed( int deltaTime );

	//! Returns the linear velocity of the static physics object.
	const idVec3&		 GetPushedLinearVelocity( const int id = 0 ) const;

	//! Returns the angular velocity of the physics object when pushed.
	const idVec3&		 GetPushedAngularVelocity( const int id = 0 ) const;

	//! Sets the master entity for this static physics object, optionally orienting it relative to the master.
	void				 SetMaster( idEntity* master, const bool orientated = true );

	//! Returns a pointer to the trace information for the blocking event.
	const trace_t*		 GetBlockingInfo() const;

	//! Returns the entity that is blocking this static physics object.
	idEntity*			 GetBlockingEntity() const;

	//! Returns the end time for linear motion.
	int					 GetLinearEndTime() const;

	//! Returns the angular end time for the static physics object.
	int					 GetAngularEndTime() const;

	//! Writes the static physics state to a snapshot message.
	void				 WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the physics state from a snapshot message for static physics.
	void				 ReadFromSnapshot( const idBitMsg& msg );

protected:
	idEntity*				  self;		 // entity using this physics object
	staticPState_t			  current;	 // physics state
	idClipModel*			  clipModel; // collision model

	// Used for client-side interpolation
	staticInterpolatePState_t previous;
	staticInterpolatePState_t next;

	// master
	bool					  hasMaster;
	bool					  isOrientated;
};

//! Computes an interpolated static physics state between two given states using spherical linear interpolation for rotation.
staticPState_t InterpolateStaticPState( const staticInterpolatePState_t& previous, const staticInterpolatePState_t& next, float fraction );

#endif /* !__PHYSICS_STATIC_H__ */
