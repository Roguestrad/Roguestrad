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

CLASS_DECLARATION( iceWeaponObject, iceWeaponChainGun )
END_CLASS

#define CHAINGUN_FIRE_SKIPFRAMES	7		// 6 shots per second
#define CHAINGUN_LOWAMMO			10
#define CHAINGUN_NUMPROJECTILES		1
#define	CHAINGUN_BARREL_SPEED		( 60 * ( USERCMD_HZ / CHAINGUN_FIRE_SKIPFRAMES ) )
#define	CHAINGUN_BARREL_ACCEL_TIME	0.4
#define CHAINGUN_BARREL_DECCEL_TIME	1.0
#define	CHAINGUN_BARREL_ACCEL		( CHAINGUN_BARREL_SPEED / CHAINGUN_BARREL_ACCEL_TIME )
#define CHAINGUN_BARREL_DECCEL		( CHAINGUN_BARREL_SPEED / CHAINGUN_BARREL_DECCEL_TIME )

// blend times
#define CHAINGUN_IDLE_TO_LOWER		4
#define CHAINGUN_IDLE_TO_FIRE		0
#define	CHAINGUN_IDLE_TO_RELOAD		4
#define CHAINGUN_RAISE_TO_IDLE		0
#define CHAINGUN_WINDDOWN_TO_IDLE	0
#define CHAINGUN_RELOAD_TO_IDLE		0


/*
===============
iceWeaponChainGun::Init
===============
*/
void iceWeaponChainGun::Init( idWeapon* weapon )
{
	iceWeaponObject::Init( weapon );

	world_model = ( idAnimatedEntity* )owner->GetWorldModel();
	world_barrel_joint = owner->GetAnimator()->GetJointHandle( "toob" );
	barrel_joint = owner->GetAnimator()->GetJointHandle( "spinner" );
	barrel_angle = 0;
	current_rate = 0;
	start_rate = 0;
	end_rate = 0;
	spin_start = 0;
	numSkipFrames = 0;
	spin_end = 0;
	spread = weapon->GetFloat( "spread" ); // weapon->GetFloat("spread")
	snd_windup = FindSound( "snd_windup" );
	snd_winddown = FindSound( "snd_winddown" );
}

/*
===============
iceWeaponChainGun::UpdateBarrel
===============
*/
void iceWeaponChainGun::UpdateBarrel()
{
	float currentTime;
	//float t;
	idAngles ang;

	currentTime = gameLocal.time;
	//if (currentTime < spin_end) {
	//	t = (currentTime - spin_start) / (spin_end - spin_start);
	//	current_rate = start_rate + t * (end_rate - start_rate);
	//}
	//else {
	current_rate = end_rate;
	//}

	if( current_rate )
	{
		barrel_angle = barrel_angle + current_rate * GAME_FRAMETIME;

		ang.pitch = 0;
		ang.yaw = 0;
		ang.roll = barrel_angle;
		owner->Event_SetJointAngle( barrel_joint, JOINTMOD_LOCAL, ang );

		ang.yaw = barrel_angle;
		ang.roll = 0;
		world_model->Event_SetJointAngle( world_barrel_joint, JOINTMOD_LOCAL, ang );
	}
}

/*
===============
iceWeaponChainGun::SpinUp
===============
*/
void iceWeaponChainGun::SpinUp()
{
	start_rate = current_rate;
	end_rate = CHAINGUN_BARREL_SPEED;
	spin_start = gameLocal.time;
	spin_end = spin_start + ( end_rate - current_rate ) / CHAINGUN_BARREL_ACCEL;
	owner->Event_StartSound( "snd_windup", SND_CHANNEL_BODY3, false );
}

/*
===============
iceWeaponChainGun::SpinDown
===============
*/
void iceWeaponChainGun::SpinDown()
{
	start_rate = current_rate;
	end_rate = 0;
	spin_start = gameLocal.time;
	spin_end = spin_start + ( current_rate - end_rate ) / CHAINGUN_BARREL_DECCEL;
	owner->Event_StartSound( "snd_winddown", SND_CHANNEL_BODY3, false );
}

/*
===============
iceWeaponChainGun::Raise
===============
*/
stateResult_t iceWeaponChainGun::Raise( stateParms_t* parms )
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
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, CHAINGUN_RAISE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}


/*
===============
iceWeaponChainGun::Lower
===============
*/
stateResult_t iceWeaponChainGun::Lower( stateParms_t* parms )
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
iceWeaponChainGun::Idle
===============
*/
stateResult_t iceWeaponChainGun::Idle( stateParms_t* parms )
{
	enum IdleState
	{
		IDLE_NOTSET = 0,
		IDLE_WAIT
	};

	switch( parms->stage )
	{
		case IDLE_NOTSET:

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
iceWeaponChainGun::Fire
===============
*/
stateResult_t iceWeaponChainGun::Fire( stateParms_t* parms )
{
	float ammoClip;
//	float currentTime;
//	float skip;

	ammoClip = owner->AmmoInClip();

	enum FireState
	{
		FIRE_NOTSET = 0,
		FIRE_SPIN1,
		FIRE_FIRE,
		FIRE_SKIPFRAMES
	};

	switch( parms->stage )
	{
		case FIRE_NOTSET:
			SpinUp();
			UpdateBarrel();
			if( current_rate >= end_rate )
			{
				if( ammoClip > 0 )
				{
					parms->stage = FIRE_SPIN1;
				}
				else
				{
					parms->stage = 0;
					return SRESULT_DONE;
				}
			}
			return SRESULT_WAIT;

		case FIRE_SPIN1:
			owner->Event_StartSound( "snd_spin", SND_CHANNEL_BODY3, false );
			parms->stage = FIRE_FIRE;
			return SRESULT_WAIT;

		case FIRE_FIRE:
			owner->Event_LaunchProjectiles( CHAINGUN_NUMPROJECTILES, spread, 0, 1, 1 );
			owner->Event_StartSound( "snd_fire", SND_CHANNEL_BODY3, false );
			if( ammoClip == CHAINGUN_LOWAMMO )
			{
				owner->Event_StartSound( "snd_lowammo", SND_CHANNEL_ITEM, false );
			}
			numSkipFrames = 0;
			parms->stage = FIRE_SKIPFRAMES;
			return SRESULT_WAIT;

		case FIRE_SKIPFRAMES:
			UpdateBarrel();
			numSkipFrames++;
			if( numSkipFrames >= CHAINGUN_FIRE_SKIPFRAMES )
			{
				if( owner->IsFiring() )
				{
					parms->stage = FIRE_FIRE;
					numSkipFrames = 0;
				}
				else
				{
					SpinDown();
					return SRESULT_DONE;
				}
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}

/*
===============
iceWeaponChainGun::Reload
===============
*/
stateResult_t iceWeaponChainGun::Reload( stateParms_t* parms )
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
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}