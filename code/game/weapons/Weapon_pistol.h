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
	\class iceWeaponPistol
	\brief A specialized pistol weapon implementation for ice-based weaponry.

	This class represents a pistol weapon that inherits from iceWeaponObject, specifically designed for ice-based ammunition and mechanics. It manages the complete state machine for the pistol
   including raising, lowering, idle, firing, and reloading behaviors. The implementation handles weapon initialization with provided weapon data and orchestrates the animation states through distinct
   handler methods. Each state handler processes specific weapon behaviors and returns state transition results to control the flow of weapon operations. The class is intended to be used within a
   broader weapon system where different weapon types can be instantiated and controlled through their respective state machines.

*/
class iceWeaponPistol : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponPistol );

	//! Initializes the pistol weapon object with the provided weapon data.
	virtual void  Init( idWeapon* weapon );

	//! Handles the raising animation state for the ice weapon pistol.
	stateResult_t Raise( stateParms_t* parms );

	//! Moves the pistol to a lower state and waits for the animation to complete.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon pistol, transitioning between idle animations and weapon readiness events.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires the pistol weapon, handling ammo management and animation states.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reloading state for the ice weapon pistol, playing the reload animation and updating the clip count.
	stateResult_t Reload( stateParms_t* parms );

private:
	float				 spread;

	const idSoundShader* snd_lowammo;
};
