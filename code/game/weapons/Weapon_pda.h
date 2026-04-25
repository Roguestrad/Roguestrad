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
	\class iceWeaponPDA
	\brief A class that manages the behavior and state transitions of an ice weapon PDA.

	This class extends iceWeaponObject to provide specific functionality for an ice weapon PDA, including initialization, state management, and animation control. It handles the raise, lower, idle,
   fire, and reload states, with each method controlling specific aspects of the PDA's operation. The class is designed to integrate with a state machine for coordinated weapon behavior.

*/
class iceWeaponPDA : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponPDA );

	//! Initializes the ice weapon PDA with the specified weapon
	virtual void  Init( idWeapon* weapon );

	//! Raises the ice weapon PDA with animation control
	stateResult_t Raise( stateParms_t* parms );

	//! Places the PDA weapon into the lowered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon PDA, managing PDA opening and weapon lowering transitions.
	stateResult_t Idle( stateParms_t* parms );

	//! Completes the fire state and returns a done result.
	stateResult_t Fire( stateParms_t* parms );

	//! Completes the reload state operation and returns a done result
	stateResult_t Reload( stateParms_t* parms );
};
