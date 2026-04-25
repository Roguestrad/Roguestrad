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
	\class iceWeaponChainGun
	\brief A specialized weapon class implementing the chain gun weapon behavior with animation states and barrel rotation effects.

	This class extends the base weapon object to implement specific behavior for a chain gun weapon, including handling various animation states such as raising, lowering, idle, firing, and reloading.
   It manages the weapon's barrel rotation through spin-up and spin-down animations, and integrates with a weapon instance for initialization and operation. The class uses state machines to control
   the weapon's behavior during different actions, ensuring proper sequencing of animations and logical operations.

*/
class iceWeaponChainGun : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponChainGun );

	//! Initializes the ice weapon chain gun with the specified weapon instance.
	virtual void  Init( idWeapon* weapon );

	//! Sets up and manages the raising animation state for the chaingun weapon.
	stateResult_t Raise( stateParms_t* parms );

	//! Puts away the chain gun weapon by playing a putaway animation and transitioning to the holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon chain gun, managing ammo status and animation playback.
	stateResult_t Idle( stateParms_t* parms );

	//! Executes the firing state machine for the chain gun weapon
	stateResult_t Fire( stateParms_t* parms );

	//! Handles the reloading animation and logic for the chain gun weapon.
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Updates the barrel angle of the ice weapon chain gun based on the current rotation rate.
	void UpdateBarrel();

	//! Initializes the spinning up animation for the chain gun.
	void SpinUp();

	//! Stops the chain gun's spinning barrel animation by gradually decreasing its rotation rate.
	void SpinDown();

private:
	idAnimatedEntity*	 world_model;
	jointHandle_t		 world_barrel_joint;
	jointHandle_t		 barrel_joint;
	float				 barrel_angle;
	float				 current_rate;
	float				 start_rate;
	float				 end_rate;
	float				 spin_start;
	float				 spin_end;
	float				 spread;
	int					 numSkipFrames;

	const idSoundShader* snd_windup;
	const idSoundShader* snd_winddown;
};
