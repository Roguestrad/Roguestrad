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

#ifndef __GAME_AF_H__
#define __GAME_AF_H__

/*
===============================================================================

  Articulated figure controller.

===============================================================================
*/

typedef struct jointConversion_s {
	int				 bodyId;		  // id of the body
	jointHandle_t	 jointHandle;	  // handle of joint this body modifies
	AFJointModType_t jointMod;		  // modify joint axis, origin or both
	idVec3			 jointBodyOrigin; // origin of body relative to joint
	idMat3			 jointBodyAxis;	  // axis of body relative to joint
} jointConversion_t;

typedef struct afTouch_s {
	idEntity*	 touchedEnt;
	idClipModel* touchedClipModel;
	idAFBody*	 touchedByBody;
} afTouch_t;

/*!
	\class idAF
	\brief Manages articulated figure physics simulation for animated entities.

	This class handles the integration of physics simulation with animated entities, allowing for realistic movement and interaction of articulated figures. It provides functionality to load
   articulated figure definitions, manage physics states, and synchronize animation with physics behavior. The class supports saving and restoring articulated figure states, setting up poses from
   animation data, and applying forces and impulses to specific body parts. It also handles constraint management for binding entities together and provides methods to query the current state of the
   articulated figure, including bounds, physics object access, and contact information with other entities. The class maintains the synchronization between animation and physics through pose setup
   and update methods, enabling complex interactions like applying external forces or retrieving impact information from specific body parts.

*/
class idAF
{
public:
	//! Initializes all member variables to their default values.
	idAF();
	~idAF();

	//! Saves the articulated figure state to a save file.
	void				Save( idSaveGame* savefile ) const;

	//! Restores the articulated figure state from a save file
	void				Restore( idRestoreGame* savefile );

	//! Sets the animator for this object.
	void				SetAnimator( idAnimator* a ) { animator = a; }

	//! Loads an articulated figure file for the specified entity.
	bool				Load( idEntity* ent, const char* fileName );

	//! Returns true if the collision model is loaded and valid.
	bool				IsLoaded() const { return isLoaded && self != NULL; }

	//! Returns a pointer to the null-terminated string containing the name of the collision model.
	const char*			GetName() const { return name.c_str(); }

	//! Sets up the pose for an articulated figure to match the animation pose of the given entity.
	void				SetupPose( idEntity* ent, int time );

	//! Changes the articulated figure pose to match the current animation pose of the given entity and sets the velocity relative to the previous pose
	void				ChangePose( idEntity* ent, int time );

	//! Returns the number of entities touching the AF's physics bodies.
	int					EntitiesTouchingAF( afTouch_t touchList[MAX_GENTITIES] ) const;

	//! Initializes and starts the articulated figure physics simulation
	void				Start();

	//! Initializes the articulated figure from the current animation pose, optionally inheriting velocity.
	void				StartFromCurrentPose( int inheritVelocityTime );

	//! Stops the articulated figure by disabling collision detection and marking it as inactive.
	void				Stop();

	//! Puts the physics object into a resting state.
	void				Rest();

	//! Returns true if the AF object is active.
	bool				IsActive() const { return isActive; }

	//! Moves the specified constraint to the given position if it binds to another entity.
	void				SetConstraintPosition( const char* name, const idVec3& pos );

	//! Returns a pointer to the physics object associated with the articulated figure.
	idPhysics_AF*		GetPhysics() { return &physicsObj; }

	//! Returns a pointer to the physics object associated with this articulated figure.
	const idPhysics_AF* GetPhysics() const { return &physicsObj; }

	//! Returns the world-space bounding box for the current pose of the articulated figure.
	idBounds			GetBounds() const;

	//! Updates the animation pose of the articulated figure based on the current physics state.
	bool				UpdateAnimation();

	//! Returns the transformation from physics to visual space for an articulated figure.
	void				GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis ) const;

	/*!
		\brief Retrieves impact information for a specific body part of the articulated figure at a given point.

		This function sets up the pose for the articulated figure and then retrieves impact information from the physics object using the specified body part and point. The body part is identified by
	   its clip model ID, and the impact information is stored in the provided structure.

		\param ent The entity associated with the articulated figure
		\param id The ID of the clip model representing the body part
		\param point The point in world space where the impact occurs
		\param info Pointer to the structure that will store the impact information
	*/
	void				GetImpactInfo( idEntity* ent, int id, const idVec3& point, impactInfo_t* info );

	/*!
		\brief Applies an impulse to a specific body of the physics object based on the entity and clip model ID.

		This function sets up the pose for the given entity and then applies the specified impulse at a given point to a specific body identified by the clip model ID within the physics object. It is
	   typically used to simulate external forces or impacts on articulated figures.

		\param ent The entity to which the impulse is applied
		\param id The clip model ID that identifies which body of the physics object is affected
		\param point The point in world space where the impulse is applied
		\param impulse The impulse vector to be applied to the body
	*/
	void				ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );

	/*!
		\brief Adds a force to a specified body part of the articulated figure.

		This function applies a force to a specific body part of the articulated figure identified by the given entity and body ID. It first ensures the pose is properly set up and then adds the force
	   to the physics object using the specified point of application and force vector.

		\param ent The entity associated with the articulated figure
		\param id The identifier for the specific body part
		\param point The point in world space where the force is applied
		\param force The force vector to be applied to the body part
	*/
	void				AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );

	//! Returns the body index for a given clip model ID, handling both direct and joint-based clip model identifiers.
	int					BodyForClipModelId( int id ) const;

	//! Saves the current state of the physics objects into the provided dictionary.
	void				SaveState( idDict& args ) const;

	//! Loads the state of the articulated figure from the provided arguments
	void				LoadState( const idDict& args );

	//! Adds bind constraints to the physics object based on spawn arguments.
	void				AddBindConstraints();

	//! Removes all bind constraints from the physics object.
	void				RemoveBindConstraints();

protected:
	idStr							  name;				  // name of the loaded .af file
	idPhysics_AF					  physicsObj;		  // articulated figure physics
	idEntity*						  self;				  // entity using the animated model
	idAnimator*						  animator;			  // animator on entity
	int								  modifiedAnim;		  // anim to modify
	idVec3							  baseOrigin;		  // offset of base body relative to skeletal model origin
	idMat3							  baseAxis;			  // axis of base body relative to skeletal model origin
	idList<jointConversion_t, TAG_AF> jointMods;		  // list with transforms from skeletal model joints to articulated figure bodies
	idList<int, TAG_AF>				  jointBody;		  // table to find the nearest articulated figure body for a joint of the skeletal model
	int								  poseTime;			  // last time the articulated figure was transformed to reflect the current animation pose
	int								  restStartTime;	  // time the articulated figure came to rest
	bool							  isLoaded;			  // true when the articulated figure is properly loaded
	bool							  isActive;			  // true if the articulated figure physics is active
	bool							  hasBindConstraints; // true if the bind constraints have been added

protected:
	//! Sets the base body for the physics object.
	void SetBase( idAFBody* body, const idJointMat* joints );

	/*!
		\brief Adds a body to the physics object with specified joint modifications.

		This function integrates a body into the physics simulation by associating it with a joint in the animation system. It retrieves the joint handle using the provided joint name, validates the
	   joint existence, and calculates the body's offset from the joint's position and orientation. The function then stores this information in a joint modification array for later use in the physics
	   simulation.

		\param body Pointer to the body to be added
		\param joints Array of joint matrices containing joint transformations
		\param jointName Name of the joint to which the body is attached
		\param mod Type of modification to apply to the joint
		\throws Error if the joint name is invalid or if the joint handle is out of bounds
	*/
	void AddBody( idAFBody* body, const idJointMat* joints, const char* jointName, const AFJointModType_t mod );

	//! Loads a body definition from a declaration and sets up the corresponding physics body with collision properties and joint attachments
	bool LoadBody( const idDeclAF_Body* fb, const idJointMat* joints );

	//! Loads and initializes a physics constraint from a constraint definition
	bool LoadConstraint( const idDeclAF_Constraint* fc );

	//! Tests if any body of the articulated figure is stuck in a solid.
	bool TestSolid() const;
};

#endif /* !__GAME_AF_H__ */
