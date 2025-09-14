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

CLASS_DECLARATION( iceWeaponObject, iceWeaponHandgrenade )
END_CLASS

#define HANDGRENADE_MINRELEASETIME		0.05
#define HANDGRENADE_FUSE				3
#define HANDGRENADE_QUICKTHROWTIME		.2
#define HANDGRENADE_MINPOWER			1.5
#define HANDGRENADE_MAXPOWER			6
#define HANDGRENADE_QUICKTHROWLAUNCH	0.55
#define HANDGRENADE_NORMALTHROWLAUNCH	0.3
#define HANDGRENADE_NUMPROJECTILES		1

// blend times
#define HANDGRENADE_IDLE_TO_LOWER		4
#define HANDGRENADE_IDLE_TO_FIRE		4
#define HANDGRENADE_RAISE_TO_IDLE		4
#define HANDGRENADE_FIRE_TO_IDLE		4

/*
===============
iceWeaponHandgrenade::Init
===============
*/
void iceWeaponHandgrenade::Init( idWeapon* weapon )
{
	iceWeaponObject::Init( weapon );

	projectile = NULL;
	spread = owner->GetFloatKey( "spread" );
	skin_nade = owner->GetKey( "skin_nade" );
	skin_nade_invis = owner->GetKey( "skin_nade_invis" );
	skin_nonade = owner->GetKey( "skin_nonade" );
	skin_nonade_invis = owner->GetKey( "skin_nonade_invis" );

	GrenadeNade();
}

/*
===============
iceWeaponHandgrenade::GrenadeNade
===============
*/
void iceWeaponHandgrenade::GrenadeNade()
{
	show_grenade = true;
	UpdateSkin();
}

/*
===============
iceWeaponHandgrenade::GrenadeNoNade
===============
*/
void iceWeaponHandgrenade::GrenadeNoNade()
{
	show_grenade = false;
	UpdateSkin();
}

/*
===============
iceWeaponHandgrenade::GrenadeNoNade
===============
*/
void iceWeaponHandgrenade::UpdateSkin()
{
	if( !show_grenade )
	{
		if( owner->Event_IsInvisible() )
		{
			owner->Event_SetSkin( skin_nonade_invis );
		}
		else
		{
			owner->Event_SetSkin( skin_nonade );
		}
	}
	else
	{
		if( owner->Event_IsInvisible() )
		{
			owner->Event_SetSkin( skin_nade_invis );
		}
		else
		{
			owner->Event_SetSkin( skin_nade );
		}
	}
}


/*
===============
iceWeaponHandgrenade::Raise
===============
*/
stateResult_t iceWeaponHandgrenade::Raise( stateParms_t* parms )
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
			if( owner->Event_AnimDone( ANIMCHANNEL_ALL, HANDGRENADE_RAISE_TO_IDLE ) )
			{
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}

	return SRESULT_ERROR;
}


/*
===============
iceWeaponHandgrenade::Lower
===============
*/
stateResult_t iceWeaponHandgrenade::Lower( stateParms_t* parms )
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
iceWeaponHandgrenade::Idle
===============
*/
stateResult_t iceWeaponHandgrenade::Idle( stateParms_t* parms )
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
iceWeaponHandgrenade::OwnerDied
===============
*/
void iceWeaponHandgrenade::OwnerDied()
{
	float time_held;

	if( projectile )
	{
		time_held = gameLocal.SysScriptTime() - fuse_start;
		projectile->Show();
		projectile->Unbind();

		// allow grenade to drop
		owner->Event_LaunchProjectiles( HANDGRENADE_NUMPROJECTILES, spread, time_held, 0, 1.0 );

		projectile = NULL;
	}
}

/*
===============
iceWeaponHandgrenade::Fire
===============
*/
stateResult_t iceWeaponHandgrenade::Fire( stateParms_t* parms )
{
	if( parms->stage == 0 )
	{
		projectile = ( idProjectile* )owner->CreateProjectile();

		if( projectile )
		{
			projectile->Event_StartSound( "snd_throw", SND_CHANNEL_BODY, true );
		}

		owner->Event_PlayAnim( ANIMCHANNEL_ALL, "throw_start", false );

		current_time = gameLocal.SysScriptTime();
		fuse_start = current_time;
		fuse_end = current_time + HANDGRENADE_FUSE;
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if( parms->stage == 1 )
	{
		if( current_time < fuse_end )
		{
			if( ( current_time > fuse_start + HANDGRENADE_MINRELEASETIME ) )
			{
				parms->stage = 2;
				return SRESULT_WAIT;
			}
			current_time = gameLocal.SysScriptTime();
			return SRESULT_WAIT;
		}

		parms->stage = 2;
		return SRESULT_WAIT;
	}

	if( parms->stage == 2 )
	{
		time_held = current_time - fuse_start;
		power = time_held + HANDGRENADE_MINPOWER;
		if( power > HANDGRENADE_MAXPOWER )
		{
			power = HANDGRENADE_MAXPOWER;
		}

		if( time_held < HANDGRENADE_QUICKTHROWTIME )
		{
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "throw_quick", false );
			parms->Wait( HANDGRENADE_QUICKTHROWLAUNCH );
			exploded = false;
		}
		else if( time_held < HANDGRENADE_FUSE )
		{
			owner->Event_PlayAnim( ANIMCHANNEL_ALL, "throw", false );
			parms->Wait( HANDGRENADE_NORMALTHROWLAUNCH );
			exploded = false;
		}
		else
		{
			// no anim.  grenade just blows up
			ExplodeInHand();
			exploded = true;
		}

		parms->stage = 3;
		return SRESULT_WAIT;
	}

	if( parms->stage == 3 )
	{
		if( !exploded )
		{
			GrenadeNoNade();
			current_time = gameLocal.SysScriptTime();
			if( projectile )
			{
				projectile->Show();
				projectile->Unbind();
				owner->Event_LaunchProjectiles( HANDGRENADE_NUMPROJECTILES, spread, current_time - fuse_start, power, 1.0 );
				projectile = NULL;
			}

			parms->stage = 4;
			return SRESULT_WAIT;
		}

		parms->stage = 5;
		return SRESULT_WAIT;
	}

	if( parms->stage == 4 )
	{
		if( owner->Event_AnimDone( ANIMCHANNEL_ALL, HANDGRENADE_FIRE_TO_IDLE ) )
		{
			parms->stage = 5;
			return SRESULT_WAIT;
		}

		return SRESULT_WAIT;
	}

	if( !owner->AmmoAvailable() )
	{
		stateThread.Clear();
		stateThread.SetState( "Holstered" );
		owner->GetOwner()->NextWeapon();
	}
	else
	{
		GrenadeNade();
	}

	return SRESULT_DONE;
}

/*
===============
iceWeaponHandgrenade::Reload
===============
*/
stateResult_t iceWeaponHandgrenade::Reload( stateParms_t* parms )
{
	return SRESULT_DONE;
}

/*
===============
iceWeaponHandgrenade::ExplodeInHand
===============
*/
void iceWeaponHandgrenade::ExplodeInHand()
{
	idStr	entname;
	idEntity* explosion;
	idVec3	forward;
	idAngles	angles;
	idPlayer* owner;

	if( projectile )
	{
		projectile->Event_Remove();
		projectile = NULL;
	}

	owner = this->owner->GetOwner();

	GrenadeNoNade();
	this->owner->Event_StartSound( "snd_explode", SND_CHANNEL_ANY, false );

	if( !common->IsClient() )
	{
		this->owner->Event_UseAmmo( HANDGRENADE_NUMPROJECTILES );

		angles = owner->viewAngles;
		forward = angles.ToForward();

		entname = this->owner->GetKey( "def_explode_inhand" );
		explosion = gameLocal.Spawn( entname );
		explosion->SetOrigin( this->owner->GetOrigin() + forward * 16 );
		explosion->SetShaderParm( SHADERPARM_TIMEOFFSET, -gameLocal.SysScriptTime() );
		gameLocal.DelayRemoveEntity( explosion, 2 );

		// this should kill us
		gameLocal.RadiusDamage( this->owner->GetOrigin(), this->owner, owner, NULL, NULL, this->owner->GetKey( "def_damage_inhand" ), 1.0f );
	}
}