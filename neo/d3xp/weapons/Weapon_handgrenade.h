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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma once

class iceWeaponHandgrenade : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponHandgrenade );

	virtual void			Init( idWeapon* weapon );
	virtual void			OwnerDied() override;

	stateResult_t			Raise( stateParms_t* parms );
	stateResult_t			Lower( stateParms_t* parms );
	stateResult_t			Idle( stateParms_t* parms );
	stateResult_t			Fire( stateParms_t* parms );
	stateResult_t			Reload( stateParms_t* parms );
private:
	void					GrenadeNade();
	void					GrenadeNoNade();
	void					UpdateSkin();

	void					ExplodeInHand();
private:
	float		spread;
	float		fuse_start;
	idStr		skin_nade;
	idStr		skin_nade_invis;
	idStr		skin_nonade;
	idStr		skin_nonade_invis;
	idProjectile* projectile;

	bool		show_grenade;
private:
	float fuse_end;
	float current_time;
	float time_held;
	float power;
	bool exploded;


	const idSoundShader* snd_lowammo;
};