/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Robert Beckebans

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

#ifndef __AASCALLBACK_FINDATTACKPOSITION_H__
#define __AASCALLBACK_FINDATTACKPOSITION_H__

/*!
	\class idAASCallback_FindAttackPosition
	\brief Callback class for finding valid attack positions using AAS navigation data.
*/
class idAASCallback_FindAttackPosition : public idAASCallback
{
public:
	/*!
		\brief Constructor for idAASCallback_FindAttackPosition that initializes callback parameters for attack position finding

		Initializes the callback object with AI entity, gravity axis, target entity, target position, and fire offset parameters. Sets up exclude bounds around the AI entity and configures the PVS
	   (Potentially Visible Set) for the target area to optimize AAS (Area Awareness System) queries during attack position calculation.

		\param self The AI entity that will perform the attack
		\param gravityAxis The gravity axis defining the orientation of the AI's world space
		\param target The target entity to which the attack position is being calculated
		\param targetPos The world position of the target entity
		\param fireOffset The offset from the AI's position where the fire/attack originates
	*/
	idAASCallback_FindAttackPosition( const idAI* self, const idMat3& gravityAxis, idEntity* target, const idVec3& targetPos, const idVec3& fireOffset );

	//! Destructor for the idAASCallback_FindAttackPosition class that frees the target PVS.
	~idAASCallback_FindAttackPosition();

	//! Checks if a given AAS area is a valid goal position for finding an attack position.
	virtual bool AreaIsGoal( const idAAS* aas, int areaNum ) override;

private:
	const idAI* self;
	idEntity*	target;
	idBounds	excludeBounds;
	idVec3		targetPos;
	idVec3		fireOffset;
	idMat3		gravityAxis;
	pvsHandle_t targetPVS;
	int			PVSAreas[idEntity::MAX_PVS_AREAS];
};

#endif /* !__AASCALLBACK_FINDCOVERAREA_H__ */
