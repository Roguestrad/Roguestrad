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
	\class iceWeaponChainsaw
	\brief A specialized chainsaw weapon implementation that extends basic weapon object functionality with specific animation and attack behaviors.

	This class implements the behavior and state management for a chainsaw weapon, inheriting from a base weapon object class. It provides specific implementations for weapon states including raising,
   lowering, idling, firing, and reloading. The class initializes with a weapon instance and manages the animation and action sequences through defined state handler methods. The design supports a
   state machine pattern for weapon operations, allowing for distinct behaviors during different phases of weapon usage. The implementation focuses on the logical flow and transitions between weapon
   states rather than memory management or object ownership details.

*/
class iceWeaponChainsaw : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponChainsaw );

	//! Initializes the chainsaw weapon object with the provided weapon instance.
	virtual void  Init( idWeapon* weapon );

	//! Handles the raising animation state for the chainsaw weapon.
	stateResult_t Raise( stateParms_t* parms );

	//! Sets up the lowering animation for the chainsaw weapon.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the chainsaw weapon by managing the weapon's ready state and idle animation cycle.
	stateResult_t Idle( stateParms_t* parms );

	//! Executes the chainsaw attack state machine for the ice weapon.
	stateResult_t Fire( stateParms_t* parms );

	//! Completes the reload state transition for the chainsaw weapon.
	stateResult_t Reload( stateParms_t* parms );

private:
	bool side;
};
