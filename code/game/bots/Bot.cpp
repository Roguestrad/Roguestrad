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

idCVar bot_pathdebug( "bot_pathdebug", "0", CVAR_BOOL | CVAR_CHEAT, "force the bot to path to player" );
idCVar bot_goaldist( "bot_goaldist", "20", CVAR_INTEGER | CVAR_CHEAT, "" );
idCVar bot_debugnav( "bot_debugnav", "0", CVAR_BOOL | CVAR_CHEAT, "draws navmesh paths for the bot" );
idCVar bot_showstate( "bot_showstate", "0", CVAR_BOOL | CVAR_CHEAT, "draws the bot state above the bot" );
idCVar bot_debug( "bot_debug", "0", CVAR_BOOL, "shows debug info for the bot" );
idCVar bot_skill( "bot_skill", "3", CVAR_INTEGER, "" );

CLASS_DECLARATION( idPlayer, iceBot )
END_CLASS

/*
===================
iceBot::iceBot
===================
*/
iceBot::iceBot()
{
	//bs.action = NULL;
	hasSpawned = false;
	gameLocal.RegisterBot( this );
}

/*
===================
iceBot::~iceBot
===================
*/
iceBot::~iceBot()
{
	gameLocal.UnRegisterBot( this );
}

/*
==================
iceBot::SetEnemy
==================
*/
void iceBot::SetEnemy( idPlayer* player, idVec3 origin )
{
	if( bs.enemy == -1 )
	{
		bs.enemy = player->entityNumber;
		bs.aggressiveAttackTime = gameLocal.SysScriptTime() + 2.0f;
		bs.lastenemyorigin = origin;
		//bs.action = &botAIBattleRetreat;
		stateThread.SetState( "state_Attacked" );
	}
}

/*
==================
iceBot::BotUpdateInventory
==================
*/
void iceBot::BotUpdateInventory()
{
	bs.inventory[INVENTORY_ARMOR]			= inventory.armor;
	bs.inventory[INVENTORY_GAUNTLET]		= 1;
	bs.inventory[INVENTORY_SHOTGUN]			= HasWeapon( weapon_shotgun );
	bs.inventory[INVENTORY_MACHINEGUN]		= HasWeapon( weapon_machinegun );
	bs.inventory[INVENTORY_GRENADELAUNCHER] = 0;
	bs.inventory[INVENTORY_ROCKETLAUNCHER]  = HasWeapon( weapon_rocketlauncher );
	bs.inventory[INVENTORY_LIGHTNING]		= 0;
	bs.inventory[INVENTORY_RAILGUN]			= 0;
	bs.inventory[INVENTORY_PLASMAGUN]		= HasWeapon( weapon_plasmagun );
	bs.inventory[INVENTORY_BFG10K]			= 0;
	bs.inventory[INVENTORY_GRAPPLINGHOOK]	= 0;
	bs.inventory[INVENTORY_SHELLS]			= inventory.ammo[idWeapon::GetAmmoNumForName( "ammo_shells" )].Get();
	bs.inventory[INVENTORY_BULLETS]			= inventory.ammo[idWeapon::GetAmmoNumForName( "ammo_clip" )].Get();
	bs.inventory[INVENTORY_GRENADES]		= 0;
	bs.inventory[INVENTORY_CELLS]			= inventory.ammo[idWeapon::GetAmmoNumForName( "ammo_cells" )].Get();
	bs.inventory[INVENTORY_LIGHTNINGAMMO]	= 0;
	bs.inventory[INVENTORY_ROCKETS]			= inventory.ammo[idWeapon::GetAmmoNumForName( "ammo_rockets" )].Get();
	bs.inventory[INVENTORY_SLUGS]			= 0;
	bs.inventory[INVENTORY_BFGAMMO]			= 0;
	bs.inventory[INVENTORY_HEALTH]			= health;
	bs.inventory[INVENTORY_TELEPORTER]		= 0;
	bs.inventory[INVENTORY_MEDKIT]			= 0;
	bs.inventory[INVENTORY_QUAD]			= 0;
	bs.inventory[INVENTORY_ENVIRONMENTSUIT] = 0;
	bs.inventory[INVENTORY_HASTE]			= 0;
	bs.inventory[INVENTORY_INVISIBILITY]	= 0;
	bs.inventory[INVENTORY_REGEN]			= 0;
	bs.inventory[INVENTORY_FLIGHT]			= 0;
	bs.inventory[INVENTORY_REDFLAG]			= 0;
	bs.inventory[INVENTORY_BLUEFLAG]		= 0;
}


/*
===================
iceBot::Spawn
===================
*/
void iceBot::Spawn()
{
	idStr botName;
	char filename[256];
	int errnum;

	stateThread.SetOwner( this );

	idPlayer::Spawn();

	if( common->IsServer() )
	{
		weapon_machinegun = SlotForWeapon( "weapon_machinegun" );
		weapon_shotgun = SlotForWeapon( "weapon_shotgun" );
		weapon_plasmagun = SlotForWeapon( "weapon_plasmagun" );
		weapon_rocketlauncher = SlotForWeapon( "weapon_rocketlauncher" );

		WP_MACHINEGUN = weapon_machinegun;
		WP_SHOTGUN = weapon_shotgun;
		WP_PLASMAGUN = weapon_plasmagun;
		WP_ROCKET_LAUNCHER = weapon_rocketlauncher;

		botName = spawnArgs.GetString( "botname" );

		aas = gameLocal.GetBotAAS();
		if( aas == NULL )
		{
			gameLocal.Error( "Missing AAS\n" );
			return;
		}

		// Load in the bot character.
		bs.character = botCharacterStatsManager.BotLoadCharacterFromFile( va( "bots/%s_c.c", botName.c_str() ), 1 );
		if( !bs.character )
		{
			gameLocal.Error( "Failed to load character file for bot %s\n", botName.c_str() );
		}

		// Allocate the goal state.
		bs.gs = botGoalManager.BotAllocGoalState( entityNumber );

		// Get the bot items weights file name.
		botCharacterStatsManager.Characteristic_String( bs.character, CHARACTERISTIC_ITEMWEIGHTS, filename, 256 );
		errnum = botGoalManager.BotLoadItemWeights( bs.gs, filename );
		if( errnum != BLERR_NOERROR )
		{
			gameLocal.Error( "Failed to load bot item weights!" );
			botGoalManager.BotFreeGoalState( bs.gs );
			return;
		}

		//allocate a weapon state
		bs.ws = botWeaponInfoManager.BotAllocWeaponState();

		//load the weapon weights
		botCharacterStatsManager.Characteristic_String( bs.character, CHARACTERISTIC_WEAPONWEIGHTS, filename, 256 );
		errnum = botWeaponInfoManager.BotLoadWeaponWeights( bs.ws, filename );
		if( errnum != BLERR_NOERROR )
		{
			//	trap_BotFreeGoalState(bs->gs);
			botWeaponInfoManager.BotFreeWeaponState( bs.ws );
			return;
		}

		bs.client = entityNumber;
		bs.entitynum = entityNumber;
		bs.setupcount = 4;
		bs.entergame_time = Bot_Time();

		hasSpawned = true;

		bs.botinput.respawn = true;

		stateThread.SetState( "state_Respawn" );
	}
}

/*
===================
iceBot::Think
===================
*/
void iceBot::BotMoveToGoalOrigin( idVec3 goalOrigin )
{
	bs.botinput.dir = ( goalOrigin - firstPersonViewOrigin );
	idAngles desiredAngles = bs.botinput.dir.ToAngles();
	if( bs.enemy >= 0 )
	{
		idPlayer* enemy = gameLocal.entities[bs.enemy]->Cast<idPlayer>();
		if( enemy )
		{
			desiredAngles = ( enemy->firstPersonViewOrigin - firstPersonViewOrigin ).ToAngles();
		}
	}

	bs.botinput.viewangles = desiredAngles;

	bs.botinput.speed = pm_runspeed.GetInteger();

	bs.botinput.dir.Normalize();
}

/*
===================
iceBot::SpawnToPoint
===================
*/
void iceBot::SpawnToPoint( const idVec3& spawn_origin, const idAngles& spawn_angles )
{
	idPlayer::SpawnToPoint( spawn_origin, spawn_angles );

	if( common->IsServer() )
	{
		bs.ltg_time = 0;
		stateThread.SetState( "state_SeekLTG" );
	}
}
/*
===================
iceBot::StateThreadChanged
===================
*/
void iceBot::StateThreadChanged()
{
	// Ensure if we are switching states, pop the last goal.
	bs.ltg_time = 0;
}

/*
===================
iceBot::ServerThink
===================
*/
void iceBot::ServerThink()
{
	bs.origin = GetPhysics()->GetOrigin();
	bs.eye = GetEyePosition();
	bs.thinktime = Bot_Time();
	bs.botinput.actionflags = 0;

	BotUpdateInventory();

	if( bot_pathdebug.IsModified() )
	{
		if( bot_pathdebug.GetBool() )
		{
			bs.currentGoal.origin = gameLocal.GetLocalPlayer()->GetPhysics()->GetOrigin();
			bs.currentGoal.framenum = gameLocal.framenum;
		}

		bot_pathdebug.ClearModified();
		bot_pathdebug.SetBool( false );
	}

	stateThread.Execute();

	// If we are moving along a set of waypoints, let's move along.
	aasPath_t path;
	//int myArea = aas->PointAreaNum(GetOrigin());
	int goalArea = aas->PointAreaNum( bs.currentGoal.origin );
	idVec3 org = bs.origin;
	int curAreaNum = aas->AdjustPositionAndGetArea( org );

	if( bot_debug.GetBool() )
	{
		if( bs.useRandomPosition )
		{
			aas->ShowWalkPath( GetOrigin(), goalArea, bs.random_move_position );
		}
		else
		{
			aas->ShowWalkPath( GetOrigin(), goalArea, bs.currentGoal.origin );
		}

		aas->ShowArea( GetOrigin() );
	}

	if( bs.useRandomPosition )
	{
		aas->WalkPathToGoal( path, curAreaNum, org, goalArea, bs.random_move_position, TFL_WALK | TFL_AIR );
	}
	else
	{
		aas->WalkPathToGoal( path, curAreaNum, org, goalArea, bs.currentGoal.origin, TFL_WALK | TFL_AIR );
	}

	idVec3 moveGoal = path.moveGoal;
	BotMoveToGoalOrigin( path.moveGoal );

	bs.viewangles = bs.botinput.viewangles;

	bs.useRandomPosition = false;
	bs.attackerEntity = NULL; // Has to be consumed immedaitly.
	bs.botinput.weapon = bs.weaponnum;
}

/*
=======================
iceBot::Damage
=======================
*/
void iceBot::Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location )
{
	idPlayer::Damage( inflictor, attacker, dir, damageDefName, damageScale, location );

	idPlayer* player = attacker->Cast<idPlayer>();
	if( health <= 0 )
	{
		if( player )
		{
			BotSendChatMessage( DEATH, player->netname );
		}
	}

	if( attacker == NULL )
	{
		return;
	}

	//bs.attackerEntity = attacker;
	SetEnemy( player, attacker->GetOrigin() );
}

/*
=======================
iceBot::InflictedDamageEvent
=======================
*/
void iceBot::InflictedDamageEvent( idEntity* target )
{
	idPlayer* player = target->Cast<idPlayer>();

	// Don't flood the chat with death and insults.
	if( !player->IsBot() && player->health <= 0 )
	{
		BotSendChatMessage( KILL, player->netname );
	}
}

/*
=======================
iceBot::PresenceTypeBoundingBox
=======================
*/
void iceBot::PresenceTypeBoundingBox( int presencetype, idVec3& mins, idVec3& maxs )
{
	int index;
	//bounding box size for each presence type
	//idVec3 boxmins[3] = { {0, 0, 0}, {-15, -15, -24}, {-15, -15, -24} };
	//idVec3 boxmaxs[3] = { {0, 0, 0}, { 15,  15,  32}, { 15,  15,   8} };

	idVec3 boxmins[3];
	idVec3 boxmaxs[3];

	boxmins[0] = idVec3( 0, 0, 0 );
	boxmins[1] = idVec3( -15, -15, -24 );
	boxmins[2] = idVec3( -15, -15, -24 );

	boxmaxs[0] = idVec3( 0, 0, 0 );
	boxmaxs[1] = idVec3( 15, 15, 32 );
	boxmaxs[2] = idVec3( 15, 15, 8 );


	if( presencetype == PRESENCE_NORMAL )
	{
		index = 1;
	}
	else if( presencetype == PRESENCE_CROUCH )
	{
		index = 2;
	}
	else
	{
		//botimport.Print(PRT_FATAL, "AAS_PresenceTypeBoundingBox: unknown presence type\n");
		index = 2;
	}
	mins = boxmins[index];
	maxs = boxmaxs[index];
}


/*
===================
iceBot::Think
===================
*/
void iceBot::Think()
{
	if( !hasSpawned )
	{
		return;
	}

	if( common->IsServer() )
	{
		ServerThink();

		if( bot_debug.GetBool() )
		{
			idVec4 color;
			color = idVec4( 1, 1, 1, 1 );
			if( bs.enemy >= 0 )
			{
				color = idVec4( 1, 0, 0, 1 );
			}

			idBounds bounds = idBounds( idVec3( -10, -10, -10 ), idVec3( 10, 10, 10 ) );
			common->RW()->DebugBounds( color, bounds, GetOrigin() );

			idMat3 axis = viewAngles.ToMat3();
			common->RW()->DrawText( stateThread.GetState()->state.c_str(), GetOrigin(), 1.0f, color, axis );
		}
	}

	deltaViewAngles.Zero();

	idPlayer::Think();
}