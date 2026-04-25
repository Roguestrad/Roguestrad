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
	\class iceWeaponShotgun
	\brief A specialized weapon class implementing the behavior and state management for an ice weapon shotgun.

	This class extends the base weapon object functionality to provide specific implementation for an ice weapon shotgun. It handles the complete weapon state machine including raising, lowering,
   idling, firing, and reloading behaviors. The class initializes its weapon properties through the provided weapon object and manages the transition between different animation states. Each state
   handler returns a result indicating whether the state transition should continue, stop, or change to another state. The implementation supports the weapon's firing mechanics and reload procedures
   while maintaining consistent behavior with the base weapon object.

*/
class iceWeaponShotgun : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponShotgun );

	//! Initializes the ice weapon shotgun with the specified weapon object.
	virtual void  Init( idWeapon* weapon );

	//! Returns the state result for raising the ice weapon shotgun.
	stateResult_t Raise( stateParms_t* parms );

	//! Handles the lowering animation state for the ice weapon shotgun, transitioning from lowering to holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon shotgun, transitioning through ready and idle cycle states.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires a shotgun projectile and handles the weapon state during the firing sequence.
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reloading state for the ice weapon shotgun, managing animation and ammo updates during the reload process.
	stateResult_t Reload( stateParms_t* parms );

private:
	float				 spread;

	const idSoundShader* snd_lowammo;
};
