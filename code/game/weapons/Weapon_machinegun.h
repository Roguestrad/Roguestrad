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
	\class iceWeaponMachineGun
	\brief A specialized machine gun weapon implementation that extends basic weapon object functionality with specific firing and animation behaviors.

	This class represents a machine gun weapon that inherits from a base weapon object, providing specialized behavior for raising, lowering, idling, firing, and reloading states. The implementation
   manages weapon-specific animation states and handles the interaction with a weapon instance through initialization and state transition methods. The class is designed to integrate with a larger
   weapon system while maintaining distinct machine gun characteristics.

*/
class iceWeaponMachineGun : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponMachineGun );

	//! Initializes the machine gun weapon object with the specified weapon instance.
	virtual void  Init( idWeapon* weapon );

	//! Sets up the raising animation for the machine gun weapon.
	stateResult_t Raise( stateParms_t* parms );

	//! Handles the lowering animation state for the ice weapon machine gun, transitioning from ready to holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon machine gun, transitioning through stages and returning appropriate state results.
	stateResult_t Idle( stateParms_t* parms );

	//! Fire function for the ice weapon machine gun that handles firing logic and animation states.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reloading animation and clip refilling for the machine gun weapon.
	stateResult_t Reload( stateParms_t* parms );

private:
	float				 spread;

	const idSoundShader* snd_lowammo;
};
