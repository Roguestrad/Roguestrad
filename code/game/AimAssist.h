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

#ifndef __AIMASSIST_H__
#define __AIMASSIST_H__

/*
================================================================================================
Contains the AimAssist declaration.
================================================================================================
*/

class idEntity;
class idPlayer;

/*!
	\class idAimAssist
	\brief The idAimAssist class modifies weapon firing angles to help players hit targets.

	This class implements aim assistance functionality that adjusts weapon firing angles based on target positions and player view direction. It manages the calculation and application of angle
   corrections to improve targeting accuracy. The system initializes with a player reference and updates its corrections each frame. It finds suitable target entities within view, computes targeting
   scores, and applies both adhesion and friction corrections. The class maintains state for the last target entity and provides access to the current angle correction values. The implementation
   considers distance-based friction effects and supports different target position calculations for various entity types.

*/
class idAimAssist
{
public:
	//! Initializes an idAimAssist object with default values.
	idAimAssist() :
		player( NULL ),
		angleCorrection( ang_zero ),
		frictionScalar( 1.0f ),
		lastTargetPos( vec3_zero )
	{
	}

	//! Initializes the aim assist system with the specified player
	void	  Init( idPlayer* player );

	//! Updates the aim assist angle correction by resetting it to zero and performing a new aim assist update
	void	  Update();

	//! Returns the angle correction stored in the aim assist system.
	void	  GetAngleCorrection( idAngles& correction ) const { correction = angleCorrection; }

	//! Returns the friction scalar value used for aim assist calculations.
	float	  GetFrictionScalar() const { return frictionScalar; }

	//! Returns the last targeted entity.
	idEntity* GetLastTarget() { return targetEntity; }

	//! Finds the best aim assist target entity within the specified distance and field of view
	idEntity* FindAimAssistTarget( idVec3& targetPos );

private:
	//! Updates the aim assist targeting and corrections for the current frame.
	void				  UpdateNewAimAssist();

	//! Computes a score for aiming assistance towards a target entity based on view direction and distance.
	float				  ComputeEntityAimAssistScore( const idVec3& targetPos, const idVec3& cameraPos, const idMat3& cameraAxis );

	//! Computes the primary and secondary target positions for aim assist based on the entity type.
	bool				  ComputeTargetPos( idEntity* pTarget, idVec3& primaryTargetPos, idVec3& secondaryTargetPos );

	//! Computes the friction radius based on the distance to the target for aim assist functionality.
	float				  ComputeFrictionRadius( float distanceToTarget );

	//! Updates the adhesion aiming correction toward the target entity
	void				  UpdateAdhesion( idEntity* pTarget, const idVec3& targetPos );

	//! Updates the friction scalar based on the target's position and the player's view direction.
	void				  UpdateFriction( idEntity* pTarget, const idVec3& targetPos );

	idPlayer*			  player;		   // player associated with this object
	idAngles			  angleCorrection; // the angle delta to apply for aim assistance
	float				  frictionScalar;  // friction scalar
	idEntityPtr<idEntity> targetEntity;	   // the last target we had (updated every frame)
	idVec3				  lastTargetPos;   // the last target position ( updated every frame );
};

#endif // !__AIMASSIST_H__
