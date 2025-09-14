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

CLASS_DECLARATION( iceWeaponObject, iceWeaponFist )
END_CLASS


// blend times
static const int FISTS_IDLE_TO_LOWER = 4;
static const int FISTS_IDLE_TO_PUNCH = 0;
static const int FISTS_RAISE_TO_IDLE = 4;
static const int FISTS_PUNCH_TO_IDLE = 1;


/*
================
iceWeaponFist::Init
================
*/
void iceWeaponFist::Init( idWeapon* weapon )
{
	iceWeaponObject::Init( weapon );
}

/*
================
iceWeaponFist::Raise
================
*/
stateResult_t iceWeaponFist::Raise( stateParms_t* parms )
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
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, FISTS_RAISE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
================
iceWeaponFist::Lower
================
*/
stateResult_t iceWeaponFist::Lower( stateParms_t* parms )
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
iceWeaponFist::Idle
================
*/
stateResult_t iceWeaponFist::Idle( stateParms_t* parms )
{
	enum IdleState
	{
		IDLE_NOTSET = 0,
		IDLE_WAIT
	};

	switch( parms->stage )
	{
		case IDLE_NOTSET:
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
iceWeaponFist::Fire
================
*/
stateResult_t iceWeaponFist::Fire( stateParms_t* parms )
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
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, GetFireAnim(), false );
			parms->stage = FIRE_MELEE;
			parms->Wait( 0.1f );
			return SRESULT_WAIT;

		case FIRE_MELEE:
			owner->Event_Melee();
			parms->stage = FIRE_WAIT;
			return SRESULT_WAIT;

		case FIRE_WAIT:
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, 0 ) )
			{
				side = !side;
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
================
iceWeaponFist::Reload
================
*/
stateResult_t iceWeaponFist::Reload( stateParms_t* parms )
{
	return SRESULT_DONE;
}

/*
================
iceWeaponFist::GetFireAnim
================
*/
const char* iceWeaponFist::GetFireAnim()
{
	if( side )
	{
		return "punch_left";
	}
	else
	{
		return "punch_right";
	}
}