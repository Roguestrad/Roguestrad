/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

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

#pragma once

/*!
	\class iceWeaponBFG
	\brief The iceWeaponBFG class implements the behavior and states for an ice-based BFG weapon.

	This class extends iceWeaponObject to provide specific functionality for an ice weapon version of the BFG. It manages the complete weapon state machine including raising, lowering, idle, firing,
   and reloading behaviors. The class initializes with a weapon object and maintains internal state variables for attack handling. The firing mechanism uses a fuse-based approach with multiple stages,
   and includes an overcharge attack capability. Animation states are handled through dedicated methods for each phase of weapon operation, ensuring proper timing and transitions between states.

*/
class iceWeaponBFG : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponBFG );

	//! Initializes the ice weapon BFG with the provided weapon object and resets all attack-related state variables.
	virtual void  Init( idWeapon* weapon );

	//! Executes the raising animation state for the ice weapon BFG.
	stateResult_t Raise( stateParms_t* parms );

	//! Handles the lowering animation state for the BFG weapon.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the BFG weapon, playing appropriate animations and managing ammo status.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires the BFG weapon with a fuse-based firing mechanism that handles different stages of the firing animation and cooldown.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reloading state for the BFG weapon, playing the reload animation and updating the clip count.
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Executes the overcharge attack for the ice weapon BFG.
	void  OverCharge();

	float spread;

	float fuse_start;
	float fuse_end;
	float powerLevel;
	float fire_time;
};
