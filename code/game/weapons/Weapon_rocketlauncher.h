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
	\class iceWeaponRocketLauncher
	\brief A weapon class that implements the behavior and states for a rocket launcher weapon.

	This class extends the base weapon object functionality to provide specific implementation for a rocket launcher weapon. It handles the complete weapon state machine including raising, lowering,
   idle, firing, and reloading behaviors. The class integrates with the weapon system to manage projectile firing, animation control, and visual feedback through skin updates. It initializes with a
   weapon instance and maintains the state transitions required for proper weapon operation within the game's combat system.

*/
class iceWeaponRocketLauncher : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponRocketLauncher );

	//! Initializes the rocket launcher weapon object with the specified weapon instance.
	virtual void  Init( idWeapon* weapon );

	//! Raises the rocket launcher weapon by playing the raise animation and waiting for it to complete.
	stateResult_t Raise( stateParms_t* parms );

	//! Puts away the rocket launcher weapon by playing a putaway animation and transitioning to a holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the rocket launcher weapon.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires a rocket launcher projectile and handles the weapon's firing state.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reload animation and clip refilling for the rocket launcher weapon.
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Updates the skin of the rocket launcher based on the remaining ammunition count.
	void  UpdateSkin();

	float spread;
	idStr skin_invisible;
};
