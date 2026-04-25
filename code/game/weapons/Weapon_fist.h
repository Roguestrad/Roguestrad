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
	\class iceWeaponFist
	\brief The iceWeaponFist class implements the behavior and states for an ice weapon fist weapon.

	This class extends iceWeaponObject to define the specific behavior of an ice weapon fist, including its initialization, raising, lowering, idle, firing, and reloading states. The class manages the
   weapon's animation states and transitions between them based on game events. It supports different firing animations based on the weapon side and provides methods to handle each state of the
   weapon's operation. The implementation uses a state machine approach to manage the weapon's behavior during various actions such as raising, firing, and reloading.

*/
class iceWeaponFist : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponFist );

	//! Initializes the ice weapon fist with the provided weapon object.
	virtual void  Init( idWeapon* weapon );

	//! Raises the ice weapon fist to idle position using animation.
	stateResult_t Raise( stateParms_t* parms );

	//! Puts away the ice weapon fist by playing a putaway animation and transitioning to the holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon fist, transitioning through stages and returning appropriate state results.
	stateResult_t Idle( stateParms_t* parms );

	//! Executes the firing state of the ice weapon fist, handling animation playback and melee attack.
	stateResult_t Fire( stateParms_t* parms );

	//! Completes the reload state operation for the ice weapon fist
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Returns the firing animation name for the ice weapon fist based on the side.
	const char* GetFireAnim();
	bool		side;
};
