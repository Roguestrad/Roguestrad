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

#ifndef __GAME_IK_H__
#define __GAME_IK_H__

/*
===============================================================================

  IK base class with a simple fast two bone solver.

===============================================================================
*/

#define IK_ANIM "ik_pose"

/*!
	\class idIK
	\brief Inverse kinematics system for entity animation control.

	Provides functionality for solving inverse kinematics problems to control entity joint positions during animation. The system can be initialized with an entity, animation, and model offset to
   establish the IK parameters. It supports saving and restoring the IK state for persistence across sessions. The class offers methods to evaluate the IK solution for specified joints and to clear
   joint modifications. It includes helper functions for solving two-bone IK systems and calculating bone axis orientations. The IK system is designed to work with entity animations and can be enabled
   or disabled through initialization and clearing operations.

*/
class idIK
{
public:
	//! Initializes an idIK object with default values.
	idIK();
	virtual ~idIK();

	//! Saves the inverse kinematics state to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the inverse kinematics state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Returns true if the IK system is initialized and enabled.
	bool		 IsInitialized() const;

	//! Initializes the inverse kinematics system for an entity with the specified animation and model offset.
	virtual bool Init( idEntity* self, const char* anim, const idVec3& modelOffset );

	//! Evaluates the inverse kinematics solution for the specified joints.
	virtual void Evaluate();

	//! Clears all joint modifications by deactivating the IK system.
	virtual void ClearJointMods();

	/*!
		\brief Solves for the joint position of a two-boneIK system given start and end positions, direction, and bone lengths.

		This function computes the position of a joint in a two-bone inverse kinematics system. It determines whether a valid solution exists based on the provided bone lengths and the distance
	   between start and end positions. If a solution is found, the joint position is calculated and returned. Otherwise, it returns false and sets the joint position to the midpoint between start and
	   end.

		\param startPos The starting position of the first bone
		\param endPos The target ending position for the second bone
		\param dir Direction vector used to determine the orientation of the joint
		\param len0 Length of the first bone
		\param len1 Length of the second bone
		\param jointPos Output parameter that will contain the computed joint position if a solution exists
		\return True if a valid joint position was calculated, false otherwise
	*/
	bool		 SolveTwoBones( const idVec3& startPos, const idVec3& endPos, const idVec3& dir, float len0, float len1, idVec3& jointPos );

	/*!
		\brief Calculates a bone axis system from start and end positions and a direction vector, returning the bone length.

		This function computes a 3x3 matrix representing the orientation of a bone segment defined by the start and end positions. The matrix is constructed using the direction vector to ensure proper
	   alignment. The first column of the matrix represents the normalized bone vector from startPos to endPos. The second column is derived by projecting the direction vector onto the bone axis and
	   subtracting it, then normalizing the result. The third column is the cross product of the second and first columns to complete the orthonormal basis. The function returns the length of the bone
	   vector.

		\param startPos The starting position of the bone segment
		\param endPos The ending position of the bone segment
		\param dir A direction vector used to orient the bone axis
		\param axis Output parameter that will contain the calculated 3x3 orientation matrix
		\return The length of the bone vector from startPos to endPos
	*/
	float		 GetBoneAxis( const idVec3& startPos, const idVec3& endPos, const idVec3& dir, idMat3& axis );

protected:
	bool		initialized;
	bool		ik_activate;
	idEntity*	self;		  // entity using the animated model
	idAnimator* animator;	  // animator on entity
	int			modifiedAnim; // animation modified by the IK
	idVec3		modelOffset;
};

/*!
	\class idIK_Walk
	\brief A class that manages inverse kinematics for walking animations.

	This class implements inverse kinematics specifically designed for walking animations, handling foot placement and body height adjustment. It inherits from idIK and provides functionality to
   initialize the IK system with an entity and animation, evaluate foot positions during animation, and manage the enabling/disabling of individual legs. The class supports serialization for saving
   and restoring IK states, and can clear joint modifications when needed. The implementation handles the complex calculations required to make characters walk naturally with proper foot contact and
   body positioning.

*/
class idIK_Walk : public idIK
{
public:
	//! Initializes an idIK_Walk instance with default values.
	idIK_Walk();

	//! Destructor for the idIK_Walk class that cleans up the foot model resource.
	virtual ~idIK_Walk();

	//! Serializes the inverse kinematics walk data to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the inverse kinematics walk state from a save file
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the inverse kinematics for walking with the specified entity, animation, and model offset.
	virtual bool Init( idEntity* self, const char* anim, const idVec3& modelOffset );

	//! Evaluates the inverse kinematics for walking animation by calculating foot positions and adjusting the character's body height.
	virtual void Evaluate();

	//! Clears all joint modifications for the IK walk system
	virtual void ClearJointMods();

	//! Enables all legs for inverse kinematics walking.
	void		 EnableAll();

	//! Disables all legs in the inverse kinematics walk solver.
	void		 DisableAll();

	//! Enables a specific leg for the IK walk mechanism.
	void		 EnableLeg( int num );

	//! Disables a specific leg by clearing its bit in the enabledLegs mask.
	void		 DisableLeg( int num );

private:
	static const int MAX_LEGS = 8;

	idClipModel*	 footModel;

	int				 numLegs;
	int				 enabledLegs;
	jointHandle_t	 footJoints[MAX_LEGS];
	jointHandle_t	 ankleJoints[MAX_LEGS];
	jointHandle_t	 kneeJoints[MAX_LEGS];
	jointHandle_t	 hipJoints[MAX_LEGS];
	jointHandle_t	 dirJoints[MAX_LEGS];
	jointHandle_t	 waistJoint;

	idVec3			 hipForward[MAX_LEGS];
	idVec3			 kneeForward[MAX_LEGS];

	float			 upperLegLength[MAX_LEGS];
	float			 lowerLegLength[MAX_LEGS];

	idMat3			 upperLegToHipJoint[MAX_LEGS];
	idMat3			 lowerLegToKneeJoint[MAX_LEGS];

	float			 smoothing;
	float			 waistSmoothing;
	float			 footShift;
	float			 waistShift;
	float			 minWaistFloorDist;
	float			 minWaistAnkleDist;
	float			 footUpTrace;
	float			 footDownTrace;
	bool			 tiltWaist;
	bool			 usePivot;

	// state
	int				 pivotFoot;
	float			 pivotYaw;
	idVec3			 pivotPos;
	bool			 oldHeightsValid;
	float			 oldWaistHeight;
	float			 oldAnkleHeights[MAX_LEGS];
	idVec3			 waistOffset;
};

/*!
	\class idIK_Reach
	\brief Inverse kinematics reach solver for entity animation.

	Handles inverse kinematics calculations to enable an entity to reach specified target positions within animation constraints. Provides functionality to initialize the solver with entity data,
   evaluate reach solutions, and manage joint modifications. Supports serialization through save and restore operations for persistent state management.

*/
class idIK_Reach : public idIK
{
public:
	//! Initializes an idIK_Reach object with default values.
	idIK_Reach();
	virtual ~idIK_Reach();

	//! Saves the ik reach data to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the state of the IK reach controller from a save file
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the IK reach solver for the given entity with specified animation and model offset.
	virtual bool Init( idEntity* self, const char* anim, const idVec3& modelOffset );

	//! Solves inverse kinematics for reachability constraints and updates joint axes accordingly
	virtual void Evaluate();

	//! Clears joint modification settings for reach animation.
	virtual void ClearJointMods();

private:
	static const int MAX_ARMS = 2;

	int				 numArms;
	int				 enabledArms;
	jointHandle_t	 handJoints[MAX_ARMS];
	jointHandle_t	 elbowJoints[MAX_ARMS];
	jointHandle_t	 shoulderJoints[MAX_ARMS];
	jointHandle_t	 dirJoints[MAX_ARMS];

	idVec3			 shoulderForward[MAX_ARMS];
	idVec3			 elbowForward[MAX_ARMS];

	float			 upperArmLength[MAX_ARMS];
	float			 lowerArmLength[MAX_ARMS];

	idMat3			 upperArmToShoulderJoint[MAX_ARMS];
	idMat3			 lowerArmToElbowJoint[MAX_ARMS];
};

#endif /* !__GAME_IK_H__ */
