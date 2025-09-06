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

CLASS_DECLARATION( iceWeaponObject, iceWeaponDoubleShotgun )
END_CLASS


//#define SHOTGUN_DOUBLE_FIRERATE		1.333
#define SHOTGUN_DOUBLE_FIRERATE		2.2

#define SHOTGUN_DOUBLE_REQUIRED			2

// blend times
#define SHOTGUN_DOUBLE_IDLE_TO_IDLE	0
#define SHOTGUN_DOUBLE_IDLE_TO_LOWER	4
#define SHOTGUN_DOUBLE_IDLE_TO_FIRE	0
#define	SHOTGUN_DOUBLE_IDLE_TO_RELOAD	4
#define	SHOTGUN_DOUBLE_IDLE_TO_NOAMMO	4
#define SHOTGUN_DOUBLE_NOAMMO_TO_RELOAD	4
#define SHOTGUN_DOUBLE_NOAMMO_TO_IDLE	4
#define SHOTGUN_DOUBLE_RAISE_TO_IDLE	4
#define SHOTGUN_DOUBLE_FIRE_TO_IDLE	4
#define SHOTGUN_DOUBLE_RELOAD_TO_IDLE	4
#define	SHOTGUN_DOUBLE_RELOAD_TO_FIRE	4


//Shotgun Projectile Information
#define SHOTGUN_CENTER_PROJECTILES		8
//#define SHOTGUN_CENTER_PROJECTILES 7
#define SHOTGUN_BIG_PROJECTILES			12
//#define SHOTGUN_BIG_PROJECTILES 13

#define SHOTGUN_CENTER_WIDTH			5
#define SHOTGUN_CENTER_HEIGHT			10
//#define SHOTGUN_CENTER_HEIGHT 12
#define SHOTGUN_BIG_WIDTH				22
//#define SHOTGUN_BIG_WIDTH 25
#define SHOTGUN_BIG_HEIGHT				15

/*
===============
iceWeaponDoubleShotgun::Init
===============
*/
void iceWeaponDoubleShotgun::Init( idWeapon* weapon )
{
	iceWeaponObject::Init( weapon );

	next_attack = 0;
}

/*
===============
iceWeaponDoubleShotgun::Raise
===============
*/
stateResult_t iceWeaponDoubleShotgun::Raise( stateParms_t* parms )
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
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, SHOTGUN_DOUBLE_RAISE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}


/*
===============
iceWeaponDoubleShotgun::Lower
===============
*/
stateResult_t iceWeaponDoubleShotgun::Lower( stateParms_t* parms )
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
iceWeaponDoubleShotgun::Idle
===============
*/
stateResult_t iceWeaponDoubleShotgun::Idle( stateParms_t* parms )
{
	//float currentTime = 0;
	float clip_size;

	clip_size = owner->ClipSize();

	enum IdleState
	{
		IDLE_NOTSET = 0,
		IDLE_WAIT
	};

	switch( parms->stage )
	{
		case IDLE_NOTSET:
			owner->Event_WeaponReady();
			if( !owner->AmmoInClip() )
			{
				owner->Event_WeaponOutOfAmmo();
			}
			else
			{
				owner->Event_WeaponReady();
			}

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
iceWeaponDoubleShotgun::Fire
===============
*/
stateResult_t iceWeaponDoubleShotgun::Fire( stateParms_t* parms )
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
			//if (ammoClip == SHOTGUN_LOWAMMO) {
			//	int length;
			//	owner->StartSoundShader(snd_lowammo, SND_CHANNEL_ITEM, 0, false, &length);
			//}

			//owner->Event_LaunchProjectiles(SHOTGUN_NUMPROJECTILES, spread, 0, 1, 1);
			owner->Event_LaunchProjectilesEllipse( SHOTGUN_CENTER_PROJECTILES, SHOTGUN_CENTER_WIDTH, SHOTGUN_CENTER_HEIGHT, 0, 1.0 );
			owner->Event_LaunchProjectilesEllipse( SHOTGUN_BIG_PROJECTILES, SHOTGUN_BIG_WIDTH, SHOTGUN_BIG_HEIGHT, 0, 1.0 );

			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "fire", false );
			parms->stage = FIRE_WAIT;
			return SRESULT_WAIT;

		case FIRE_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, SHOTGUN_DOUBLE_FIRE_TO_IDLE ) )
			{
				owner->Event_WeaponReloading();
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
===============
iceWeaponDoubleShotgun::Reload
===============
*/
stateResult_t iceWeaponDoubleShotgun::Reload( stateParms_t* parms )
{
	enum RELOAD_State
	{
		RELOAD_NOTSET = 0,
		RELOAD_WAIT
	};

	switch( parms->stage )
	{
		case RELOAD_NOTSET:
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "reload_start", false );
			parms->stage = RELOAD_WAIT;
			return SRESULT_WAIT;

		case RELOAD_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, 0 ) )
			{
				owner->Event_AddToClip( owner->ClipSize() );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
===============
iceWeaponDoubleShotgun::EjectBrass
===============
*/
void iceWeaponDoubleShotgun::EjectBrass()
{

}