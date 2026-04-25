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

#ifndef __PUSH_H__
#define __PUSH_H__

/*
===============================================================================

  Allows physics objects to be pushed geometrically.

===============================================================================
*/

#define PUSHFL_ONLYMOVEABLE		1  // only push moveable entities
#define PUSHFL_NOGROUNDENTITIES 2  // don't push entities the clip model rests upon
#define PUSHFL_CLIP				4  // also clip against all non-moveable entities
#define PUSHFL_CRUSH			8  // kill blocking entities
#define PUSHFL_APPLYIMPULSE		16 // apply impulse to pushed entities

/*!
	\class idPush
	\brief Handles entity pushing operations including translation, rotation, and collision detection.

	This class implements comprehensive entity pushing functionality within a game world, supporting both translational and rotational movement of entities. It provides methods for clipping push
   operations against geometry, managing physics states of pushed entities, and handling various edge cases such as projectiles, crushed entities, and gibbable entities. The class maintains a registry
   of entities that have been pushed during an operation, allowing for position restoration and tracking of push results. It supports different push behaviors through flags, including clipping,
   impulse application, and crushing effects. The implementation handles complex scenarios like rotating entities to axial orientations, managing skip clip models during operations, and filtering
   entities based on pushability criteria. The system is designed to integrate with the physics engine to ensure realistic interaction between pusher and pushed entities.

*/
class idPush
{
public:
	/*!
		\brief Attempts to push other entities by moving the given entity and returns the total mass of all pushed entities.

		This function performs translational pushing of entities within the game world. It calculates the movement bounds and identifies entities that may be affected by the push. If the PUSHFL_CLIP
	   flag is set, it performs a collision trace to determine how far the pusher can move before hitting something. For each entity that can be pushed, it attempts to translate the entity and applies
	   an impulse if the PUSHFL_APPLYIMPULSE flag is set. The function handles special cases like projectiles, crushed entities, and gibbable entities, applying appropriate damage or events. It
	   properly manages the clip model state during the process.

		\param results Structure containing the results of the trace, including fraction of movement completed and the entity that blocked the movement
		\param pusher Pointer to the entity that is performing the pushing action
		\param flags Flags controlling the pushing behavior including clipping, impulse application, and crushing
		\param newOrigin The intended end position of the pusher after movement
		\param move The translation vector representing the movement direction and distance
		\return Returns the total mass of all entities that were successfully pushed
	*/
	float	  ClipTranslationalPush( trace_t& results, idEntity* pusher, const int flags, const idVec3& newOrigin, const idVec3& move );

	/*!
		\brief Attempts to push other entities by rotating the given entity within a specified bounds and rotation

		This function performs rotational pushing of entities within the game world by calculating the movement bounds based on the provided rotation and axis. It checks for entities that can be
	   pushed, and attempts to move them accordingly. The function handles clipping if the PUSHFL_CLIP flag is set, and manages the physics and collision states of both the pusher and the entities
	   being pushed. It returns the total mass of entities that were successfully pushed or 0.0f if no rotation is specified.

		\param results Output structure containing the results of the push operation including fraction, end position, end axis, and contact information
		\param pusher Entity that is initiating the push operation
		\param flags Flags that control the behavior of the push operation, such as clipping or crushing behavior
		\param newAxis New axis orientation of the pusher after the push operation
		\param rotation Rotation to apply to the pusher during the push operation
		\return Total mass of entities that were successfully pushed, or 0.0f if no rotation is specified
	*/
	float	  ClipRotationalPush( trace_t& results, idEntity* pusher, const int flags, const idMat3& newAxis, const idRotation& rotation );

	/*!
		\brief Attempts to push other entities by moving the given entity and returns the total mass involved in the push operation.

		This function handles both translational and rotational pushing of entities. It first calculates the translation and rotational components of the movement, then performs clipping operations to
	   determine if the pusher collides with other entities. If a collision occurs, the function updates the results with the collision information and returns the mass of the pusher. The function
	   modifies the newOrigin and newAxis parameters to reflect the final position and orientation after the push operation, taking into account any collisions that occurred.

		\param results Output structure containing the result of the trace, including collision fraction and end position/axis
		\param pusher The entity that is performing the push operation
		\param flags Flags that control the behavior of the push operation
		\param oldOrigin The starting position of the pusher before the movement
		\param oldAxis The starting orientation of the pusher before the movement
		\param newOrigin The desired end position of the pusher after the movement
		\param newAxis The desired end orientation of the pusher after the movement
		\return The total mass involved in the push operation, which is accumulated from both translational and rotational push calculations.
	*/
	float	  ClipPush( trace_t& results, idEntity* pusher, const int flags, const idVec3& oldOrigin, const idMat3& oldAxis, idVec3& newOrigin, idMat3& newAxis );

	//! Initializes the tracking of pushed entity positions.
	void	  InitSavingPushedEntityPositions();

	//! Restores the positions and physics states of all pushed entities to their previous values.
	void	  RestorePushedEntityPositions();

	//! Returns the number of entities that have been pushed.
	int		  GetNumPushedEntities() const { return numPushed; }

	//! Returns the ith pushed entity in the push list.
	idEntity* GetPushedEntity( int i ) const
	{
		assert( i >= 0 && i < numPushed );
		return pushed[i].ent;
	}

private:
	struct pushed_s {
		idEntity* ent;			   // pushed entity
		idAngles  deltaViewAngles; // actor delta view angles
	} pushed[MAX_GENTITIES];	   // pushed entities
	int numPushed;				   // number of pushed entities

	struct pushedGroup_s {
		idEntity* ent;
		float	  fraction;
		bool	  groundContact;
		bool	  test;
	} pushedGroup[MAX_GENTITIES];
	int pushedGroupSize;

private:
	//! Saves the physics state and delta view angles for an entity if it hasn't been saved already
	void SaveEntityPosition( idEntity* ent );

	//! Rotates an entity to an axial orientation using up to four attempts.
	bool RotateEntityToAxial( idEntity* ent, idVec3 rotationPoint );
#ifdef NEW_PUSH
	bool CanPushEntity( idEntity* ent, idEntity* pusher, idEntity* initialPusher, const int flags );
	void AddEntityToPushedGroup( idEntity* ent, float fraction, bool groundContact );
	bool IsFullyPushed( idEntity* ent );
	bool ClipTranslationAgainstPusher( trace_t& results, idEntity* ent, idEntity* pusher, const idVec3& translation );
	int	 GetPushableEntitiesForTranslation( idEntity* pusher, idEntity* initialPusher, const int flags, const idVec3& translation, idEntity* entityList[], int maxEntities );
	bool ClipRotationAgainstPusher( trace_t& results, idEntity* ent, idEntity* pusher, const idRotation& rotation );
	int	 GetPushableEntitiesForRotation( idEntity* pusher, idEntity* initialPusher, const int flags, const idRotation& rotation, idEntity* entityList[], int maxEntities );
#else

	/*!
		\brief Clips the rotation of an entity using the specified trace and clip model, with optional skipping of a specific clip model.

		The function disables a skip clip model if provided, performs the rotation clipping operation using the entity's physics, and then re-enables the skip clip model. This ensures that the
	   rotation clipping is performed without interference from the skip model.

		\param trace The trace structure to store the results of the clipping operation
		\param ent The entity whose rotation is to be clipped
		\param clipModel The clip model to use for the clipping operation
		\param skip Optional clip model to temporarily disable during the operation
		\param rotation The rotation to apply and clip
	*/
	void ClipEntityRotation( trace_t& trace, const idEntity* ent, const idClipModel* clipModel, idClipModel* skip, const idRotation& rotation );

	/*!
		\brief Performs a translation clipping operation for an entity using a clip model while optionally skipping a specific clip model.

		This function handles the clipping of an entity's translation movement by using the entity's physics system. It temporarily disables a skip clip model if provided, performs the translation
	   clipping, and then re-enables the skip clip model. This ensures that the entity can be properly clipped against the environment without colliding with the skip model.

		\param trace Output trace structure that will contain the results of the clipping operation
		\param ent Pointer to the entity whose translation is being clipped
		\param clipModel Pointer to the clip model used for the clipping operation
		\param skip Optional pointer to a clip model that should be temporarily disabled during the clipping
		\param translation The translation vector to apply to the entity
	*/
	void ClipEntityTranslation( trace_t& trace, const idEntity* ent, const idClipModel* clipModel, idClipModel* skip, const idVec3& translation );

	/*!
		\brief Attempts to translate a pusher entity based on collision detection with other entities.

		This function handles the translation of an entity when it is pushed by another entity. It determines whether the entity can be moved in the direction of the push, considering collisions with
	   other entities. If a collision occurs and the entity cannot be moved further, it may return a blocked status. The function also manages the physics of the entity during movement and updates its
	   position accordingly.

		\param results Output structure containing the results of the trace operation.
		\param check Entity that is being pushed.
		\param clipModel The clip model of the entity being pushed.
		\param flags Flags that control the behavior of the push operation.
		\param newOrigin The target origin position for the entity.
		\param move The movement vector to apply to the entity.
		\return An integer value indicating the result of the push operation, with possible values including PUSH_OK, PUSH_NO, or PUSH_BLOCKED.
	*/
	int	 TryTranslatePushEntity( trace_t& results, idEntity* check, idClipModel* clipModel, const int flags, const idVec3& newOrigin, const idVec3& move );

	/*!
		\brief Attempts to rotate a pusher entity and handles collision detection and response.

		This function tries to rotate a pusher entity and checks for collisions with other entities. It handles different cases based on whether the entity is standing on the pusher or not. The
	   function updates the entity's position and rotation, and returns a value indicating the outcome of the operation.

		\param results Output structure containing the results of the trace
		\param check The entity being pushed
		\param clipModel The clip model of the entity being pushed
		\param flags Flags for the push operation
		\param newAxis The new axis after rotation
		\param rotation The rotation to be applied to the entity
		\return An integer indicating the result of the push operation: PUSH_OK if successful, PUSH_NO if not pushed, PUSH_BLOCKED if blocked by another entity
	*/
	int	 TryRotatePushEntity( trace_t& results, idEntity* check, idClipModel* clipModel, const int flags, const idMat3& newAxis, const idRotation& rotation );

	/*!
		\brief Filters and discards entities from a list based on pushability and various flags.

		This function processes a list of entities to determine which ones can be pushed by a specified pusher entity. It evaluates each entity against multiple criteria including whether the entity
	   is pushable, whether it collides with the pusher, whether it is a player in noclip mode, whether it is a moveable entity when required, and whether it is a ground entity that should not be
	   pushed. Entities that fail any of these checks are removed from the list, and the function returns the count of entities that remain.

		\param entityList Array of entity pointers to be processed
		\param numEntities Number of entities in the entity list
		\param flags Flags that control how entities are filtered
		\param pusher The entity that is attempting to push the entities
		\return The number of entities that passed all filtering criteria and remain in the list.
	*/
	int	 DiscardEntities( idEntity* entityList[], int numEntities, int flags, idEntity* pusher );
#endif
};

#endif /* !__PUSH_H__ */
