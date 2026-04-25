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
	\class iceWeaponPlasmaGun
	\brief A plasma gun weapon implementation that extends basic weapon object functionality with specialized firing, reloading, and animation states.

	This class implements a plasma gun weapon that inherits from a base weapon object. It provides specific behaviors for initializing the weapon, raising and lowering it with animations, handling
   idle states, firing projectiles with ammo management, and managing the reloading process. The implementation follows a state-driven approach for weapon operations, using animation states and
   parameters to control transitions and behavior. The class is designed to integrate with a larger weapon system while maintaining clear separation of plasma gun specific functionality.

*/
class iceWeaponPlasmaGun : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponPlasmaGun );

	//! Initializes the plasma gun weapon object with the specified weapon parameters.
	virtual void  Init( idWeapon* weapon );

	//! Raises the plasma gun weapon by playing the raise animation and waiting for it to complete.
	stateResult_t Raise( stateParms_t* parms );

	//! Puts the plasma gun away by playing the putaway animation and transitioning to the holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the plasma gun weapon.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires the plasma gun weapon, handling ammo checks, projectile launching, and animation states.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reloading state for the plasma gun weapon.
	stateResult_t Reload( stateParms_t* parms );

private:
	float				 spread;

	const idSoundShader* snd_lowammo;
};
