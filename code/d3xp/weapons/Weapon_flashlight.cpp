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

CLASS_DECLARATION( iceWeaponObject, iceWeaponFlashlight )
END_CLASS

// blend times
static const int FLASHLIGHT_IDLE_TO_LOWER = 4;
static const int FLASHLIGHT_IDLE_TO_FIRE = 2;
static const int FLASHLIGHT_IDLE_TO_RELOAD = 4;
static const int FLASHLIGHT_RAISE_TO_IDLE = 4;
static const int FLASHLIGHT_FIRE_TO_IDLE = 4;
static const int FLASHLIGHT_RELOAD_TO_IDLE = 4;

static const float FLASHLIGHT_MIN_SKIN_INTENSITY = 0.2f;

/*
================
iceWeaponFlashlight::Init
================
*/
void iceWeaponFlashlight::Init( idWeapon* weapon )
{
	iceWeaponObject::Init( weapon );

	skin_on = owner->GetKey( "skin_on" );
	skin_on_invis = owner->GetKey( "skin_on_invis" );
	skin_off = owner->GetKey( "skin_off" );
	skin_off_invis = owner->GetKey( "skin_off_invis" );

	intensity = 1.0;

	owner->Event_SetLightParm( 3, 1.0 );
	owner->Event_SetShaderParm( 3, 1.0 );

	on = true;

	UpdateSkin();
}

/*
================
iceWeaponFlashlight::UpdateLightIntensity
================
*/
void iceWeaponFlashlight::UpdateLightIntensity()
{
	// TODO this has to interact with scripts somehow
}

/*
================
iceWeaponFlashlight::UpdateSkin
================
*/
void iceWeaponFlashlight::UpdateSkin()
{
	if( on && ( intensity > FLASHLIGHT_MIN_SKIN_INTENSITY ) )
	{
		if( !owner->Event_IsInvisible() )
		{
			owner->Event_SetSkin( skin_on );
		}
		else
		{
			owner->Event_SetSkin( skin_on_invis );
		}
	}
	else
	{
		if( !owner->Event_IsInvisible() )
		{
			owner->Event_SetSkin( skin_off );
		}
		else
		{
			owner->Event_SetSkin( skin_off_invis );
		}
	}
}

/*
================
iceWeaponFlashlight::Raise
================
*/
stateResult_t iceWeaponFlashlight::Raise( stateParms_t* parms )
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
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, FLASHLIGHT_RAISE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
================
iceWeaponFlashlight::Lower
================
*/
stateResult_t iceWeaponFlashlight::Lower( stateParms_t* parms )
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
================
iceWeaponFlashlight::Idle
================
*/
stateResult_t iceWeaponFlashlight::Idle( stateParms_t* parms )
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
================
iceWeaponFlashlight::Fire
================
*/
stateResult_t iceWeaponFlashlight::Fire( stateParms_t* parms )
{
	enum FIRE_State
	{
		FIRE_NOTSET = 0,
		FIRE_MELEE,
		FIRE_WAIT
	};

	switch( parms->stage )
	{
		case FIRE_NOTSET:
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "fire", false );
			parms->stage = FIRE_MELEE;
			parms->Wait( 0.1f );
			return SRESULT_WAIT;

		case FIRE_MELEE:
			owner->Event_Melee();
			parms->stage = FIRE_WAIT;
			return SRESULT_WAIT;

		case FIRE_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, FLASHLIGHT_FIRE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_DONE;
}

/*
================
iceWeaponFlashlight::Reload
================
*/
stateResult_t iceWeaponFlashlight::Reload( stateParms_t* parms )
{
	enum RELOAD_State
	{
		RELOAD_NOTSET = 0,
		RELOAD_TOGGLEFLASHLIGHT,
		RELOAD_WAIT
	};

	switch( parms->stage )
	{
		case RELOAD_NOTSET:
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "reload", false );
			parms->stage = RELOAD_TOGGLEFLASHLIGHT;
			parms->Wait( 0.2f );
			return SRESULT_WAIT;

		case RELOAD_TOGGLEFLASHLIGHT:
			on = !on;
			UpdateSkin();
			owner->Event_Flashlight( on );
			parms->stage = RELOAD_WAIT;
			return SRESULT_WAIT;

		case RELOAD_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, FLASHLIGHT_RELOAD_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}
