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
	\class iceWeaponDoubleShotgun
	\brief A specialized weapon class that implements the behavior and states for a double shotgun weapon.

	This class extends the base weapon object to provide specific functionality for a double shotgun weapon. It manages the weapon's operational states including raising, lowering, idling, firing, and
   reloading. The class initializes with a provided weapon instance and handles animation states through defined callbacks. It also includes functionality for ejecting brass casings after firing. The
   weapon follows a state machine pattern where each primary action is handled by a dedicated state handler function.

*/
class iceWeaponDoubleShotgun : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponDoubleShotgun );

	//! Initializes the ice weapon double shotgun with the provided weapon instance.
	virtual void  Init( idWeapon* weapon );

	//! Handles the raising animation state for the ice weapon double shotgun.
	stateResult_t Raise( stateParms_t* parms );

	//! Sets the weapon to the lowering state and plays the putaway animation.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state logic for the ice weapon double shotgun.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires the double shotgun weapon, handling ammo management and animation states.
	stateResult_t Fire( stateParms_t* parms );

	//! Reloads the double shotgun weapon by playing a reload animation and adding ammunition to the clip
	stateResult_t Reload( stateParms_t* parms );

	//! Ejects the brass casing from the double shotgun weapon.
	void		  EjectBrass();

private:
};
