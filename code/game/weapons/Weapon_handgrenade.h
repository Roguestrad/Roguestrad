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
	\class iceWeaponHandgrenade
	\brief Represents a hand grenade weapon implementation with animation states and explosion behavior.

	This class implements a hand grenade weapon that inherits from iceWeaponObject. It manages the weapon's animation states including raising, lowering, and idle behaviors, as well as the firing
   mechanics with timed fuse activation. The class handles grenade visibility through show and hide methods, and includes logic for when the weapon owner dies. The grenade can be fired with varying
   power based on hold time, and includes an explosion effect when detonated in the player's hand. The class orchestrates the transition between different weapon states and maintains the visual
   representation of the grenade through skin updates.

*/
class iceWeaponHandgrenade : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponHandgrenade );

	//! Initializes the hand grenade weapon with the specified weapon object
	virtual void  Init( idWeapon* weapon );

	//! Handles the cleanup and activation of the grenade when the owner dies.
	virtual void  OwnerDied() override;

	//! Raises the hand grenade weapon through a series of animation states.
	stateResult_t Raise( stateParms_t* parms );

	//! Handles the lowering animation state for the handgrenade weapon, transitioning from ready to holstered state.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the handgrenade weapon, transitioning and managing the weapon's idle animation and readiness.
	stateResult_t Idle( stateParms_t* parms );

	//! Fires a handgrenade with a timed fuse that can be thrown with varying power based on hold time
	stateResult_t Fire( stateParms_t* parms );

	//! Completes the reload state execution and returns a done result.
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Enables the display of the grenade and updates its skin.
	void GrenadeNade();

	//! Hides the grenade and updates the skin to reflect the change.
	void GrenadeNoNade();

	//! Updates the skin of the grenade based on visibility and show_grenade flag
	void UpdateSkin();

	//! Causes the handgrenade to explode in the player's hand, removing the projectile and spawning an explosion effect.
	void ExplodeInHand();

private:
	float		  spread;
	float		  fuse_start;
	idStr		  skin_nade;
	idStr		  skin_nade_invis;
	idStr		  skin_nonade;
	idStr		  skin_nonade_invis;
	idProjectile* projectile;

	bool		  show_grenade;

private:
	float				 fuse_end;
	float				 current_time;
	float				 time_held;
	float				 power;
	bool				 exploded;

	const idSoundShader* snd_lowammo;
};
