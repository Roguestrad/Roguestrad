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
	\class iceWeaponFlashlight
	\brief Represents a flashlight weapon object with animation and state management capabilities.

	This class implements a specialized weapon object for a flashlight, extending base weapon functionality with specific animations and states for raising, lowering, idle, firing, and reloading
   actions. The class manages the weapon's visual appearance through skin updates and light intensity adjustments. It integrates with a weapon system that handles state transitions and animation
   playback. The implementation supports both the conceptual and visual aspects of a flashlight weapon within a game engine environment.

*/
class iceWeaponFlashlight : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponFlashlight );

	//! Initializes the flashlight weapon object with the specified weapon
	virtual void  Init( idWeapon* weapon );

	//! Raises the flashlight weapon animation and manages the animation state transitions.
	stateResult_t Raise( stateParms_t* parms );

	//! Handles the lowering animation state for the flashlight weapon, transitioning from active to holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the flashlight weapon.
	stateResult_t Idle( stateParms_t* parms );

	//! Executes the fire animation and melee attack sequence for the flashlight weapon.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reload state for the flashlight weapon, toggling the flashlight state and managing animation playback.
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Updates the skin of the flashlight based on its state and invisibility.
	void  UpdateSkin();

	//! Updates the light intensity for the ice weapon flashlight.
	void  UpdateLightIntensity();

	bool  on;
	float intensity;
	idStr skin_on;
	idStr skin_on_invis;
	idStr skin_off;
	idStr skin_off_invis;
};
