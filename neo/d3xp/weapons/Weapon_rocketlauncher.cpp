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

#include "precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

CLASS_DECLARATION( iceWeaponObject, iceWeaponRocketLauncher )
END_CLASS

#define ROCKETLAUNCHER_LOWAMMO			1
#define ROCKETLAUNCHER_NUMPROJECTILES	1
#define ROCKETLAUNCHER_FIREDELAY		1		// was .75 changed again by Tim to 1

// blend times
#define ROCKETLAUNCHER_IDLE_TO_LOWER	4
#define ROCKETLAUNCHER_IDLE_TO_FIRE		0
#define	ROCKETLAUNCHER_IDLE_TO_RELOAD	4
#define ROCKETLAUNCHER_RAISE_TO_IDLE	4
#define ROCKETLAUNCHER_FIRE_TO_IDLE		0
#define ROCKETLAUNCHER_RELOAD_TO_IDLE	4
#define ROCKETLAUNCHER_RELOAD_FRAME		34		// how many frames from the end of "reload" to fill the clip

/*
===============
iceWeaponRocketLauncher::Init
===============
*/
void iceWeaponRocketLauncher::Init( idWeapon* weapon )
{
	iceWeaponObject::Init( weapon );

	next_attack = 0;
	spread = weapon->GetFloat( "spread" );
	skin_invisible = weapon->GetKey( "skin_invisible" );
}

/*
===============
iceWeaponRocketLauncher::UpdateSkin
===============
*/
void iceWeaponRocketLauncher::UpdateSkin()
{
	idStr skinname;
	int ammoClip;

	if( owner->Event_IsInvisible() )
	{
		skinname = skin_invisible;
	}
	else
	{
		ammoClip = owner->AmmoInClip();
		if( ammoClip > 5 )
		{
			// can happen in MP - weapon Raising with slightly out-of-sync ammoClip
			ammoClip = 5;
		}
		if( ammoClip < 0 )
		{
			ammoClip = 0;
		}
		skinname = va( "skins/models/weapons/%drox.skin", ammoClip );
	}
	owner->Event_SetSkin( skinname );
}

/*
===============
iceWeaponRocketLauncher::Raise
===============
*/
stateResult_t iceWeaponRocketLauncher::Raise( stateParms_t* parms )
{
	enum RisingState
	{
		RISING_NOTSET = 0,
		RISING_WAIT
	};

	switch( parms->stage )
	{
		case RISING_NOTSET:
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "raise", false );
			parms->stage = RISING_WAIT;
			return SRESULT_WAIT;

		case RISING_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, ROCKETLAUNCHER_RAISE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}


/*
===============
iceWeaponRocketLauncher::Lower
===============
*/
stateResult_t iceWeaponRocketLauncher::Lower( stateParms_t* parms )
{
	enum LoweringState
	{
		LOWERING_NOTSET = 0,
		LOWERING_WAIT
	};

	switch( parms->stage )
	{
		case LOWERING_NOTSET:
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "putaway", false );
			parms->stage = LOWERING_WAIT;
			return SRESULT_WAIT;

		case LOWERING_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, 0 ) )
			{
				SetState( "Holstered" );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}
/*
===============
iceWeaponRocketLauncher::Idle
===============
*/
stateResult_t iceWeaponRocketLauncher::Idle( stateParms_t* parms )
{
	enum IdleState
	{
		IDLE_NOTSET = 0,
		IDLE_WAIT
	};

	switch( parms->stage )
	{
		case IDLE_NOTSET:
			owner->Event_WeaponReady();
			owner->Event_PlayCycle( ANIMCHANNEL_ALL, "idle" );
			parms->stage = IDLE_WAIT;
			return SRESULT_WAIT;

		case IDLE_WAIT:
			// Do nothing.
			return SRESULT_DONE;
	}

	return SRESULT_ERROR;
}

/*
===============
iceWeaponRocketLauncher::Fire
===============
*/
stateResult_t iceWeaponRocketLauncher::Fire( stateParms_t* parms )
{
	int ammoClip = owner->AmmoInClip();

	enum FIRE_State
	{
		FIRE_NOTSET = 0,
		FIRE_WAIT
	};

	if( ammoClip == 0 && owner->AmmoAvailable() && parms->stage == 0 )
	{
		//owner->WeaponState( WP_RELOAD, PISTOL_IDLE_TO_RELOAD );
		owner->Reload();
		return SRESULT_DONE;
	}

	switch( parms->stage )
	{
		case FIRE_NOTSET:
			next_attack = gameLocal.realClientTime + SEC2MS( ROCKETLAUNCHER_FIREDELAY );
			owner->Event_LaunchProjectiles( ROCKETLAUNCHER_NUMPROJECTILES, spread, 0, 1, 1 );

			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "fire", false );
			parms->stage = FIRE_WAIT;
			return SRESULT_WAIT;

		case FIRE_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, ROCKETLAUNCHER_FIRE_TO_IDLE ) )
			{
				UpdateSkin();
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
===============
iceWeaponRocketLauncher::Reload
===============
*/
stateResult_t iceWeaponRocketLauncher::Reload( stateParms_t* parms )
{
	enum RELOAD_State
	{
		RELOAD_NOTSET = 0,
		RELOAD_WAIT
	};

	switch( parms->stage )
	{
		case RELOAD_NOTSET:
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "reload", false );
			parms->stage = RELOAD_WAIT;
			return SRESULT_WAIT;

		case RELOAD_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, 0 ) )
			{
				owner->Event_AddToClip( owner->ClipSize() );
				UpdateSkin();
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}