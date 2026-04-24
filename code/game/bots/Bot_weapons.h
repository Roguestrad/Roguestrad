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

#define BOT_MAX_WEAPONS	   32

// projectile flags
#define PFL_WINDOWDAMAGE   1 // projectile damages through window
#define PFL_RETURN		   2 // set when projectile returns to owner
// weapon flags
#define WFL_FIRERELEASED   1 // set when projectile is fired with key-up event
// damage types
#define DAMAGETYPE_IMPACT  1 // damage on impact
#define DAMAGETYPE_RADIAL  2 // radial damage
#define DAMAGETYPE_VISIBLE 4 // damage to all entities visible to the projectile

struct projectileinfo_t {
	//! Initializes a new instance of the projectileinfo_t struct with default values.
	projectileinfo_t()
	{
		name	   = "";
		model	   = "";
		flags	   = 0;
		gravity	   = 0;
		damage	   = 0;
		radius	   = 0;
		visdamage  = 0;
		damagetype = 0;
		healthinc  = 0;
		push	   = 0;
		detonation = 0;
		bounce	   = 0;
		bouncefric = 0;
		bouncestop = 0;
	}
	idStr name;
	idStr model;
	int	  flags;
	float gravity;
	int	  damage;
	float radius;
	int	  visdamage;
	int	  damagetype;
	int	  healthinc;
	float push;
	float detonation;
	float bounce;
	float bouncefric;
	float bouncestop;
};

struct bot_weaponinfo_t {
	//! Constructs a default bot weapon info object with all fields initialized to zero or empty values.
	bot_weaponinfo_t()
	{
		valid		   = 0;
		number		   = 0;
		name		   = "";
		model		   = "";
		level		   = 0;
		weaponindex	   = 0;
		flags		   = 0;
		projectile	   = "";
		numprojectiles = 0;
		hspread		   = 0;
		vspread		   = 0;
		speed		   = 0;
		acceleration   = 0;
		recoil.Zero();
		offset.Zero();
		angleoffset.Zero();
		extrazvelocity = 0;
		ammoamount	   = 0;
		ammoindex	   = 0;
		activate	   = 0;
		reload		   = 0;
		spinup		   = 0;
		spindown	   = 0;
	}

	int				 valid;	 // true if the weapon info is valid
	int				 number; // number of the weapon
	idStr			 name;
	idStr			 model;
	int				 level;
	int				 weaponindex;
	int				 flags;
	idStr			 projectile;
	int				 numprojectiles;
	float			 hspread;
	float			 vspread;
	float			 speed;
	float			 acceleration;
	idVec3			 recoil;
	idVec3			 offset;
	idVec3			 angleoffset;
	float			 extrazvelocity;
	int				 ammoamount;
	int				 ammoindex;
	float			 activate;
	float			 reload;
	float			 spinup;
	float			 spindown;
	projectileinfo_t proj; // pointer to the used projectile
};

struct bot_weaponstate_t {
	//! Constructs a new bot weapon state and initializes it to default values.
	bot_weaponstate_t() { Reset(); }

	//! Resets the bot weapon state to its initial condition
	void Reset()
	{
		inUse			   = false;
		weaponweightconfig = NULL;
		for( int i = 0; i < BOT_MAX_WEAPONS; i++ ) {
			weaponweightindex[i] = 0;
		}
	}

	bool			inUse;
	weightconfig_t* weaponweightconfig;
	int				weaponweightindex[BOT_MAX_WEAPONS]; // weapon weight index
};

/*!
	\class idBotWeaponInfoManager
	\brief Manages bot weapon information and configurations for different weapon states.

	The idBotWeaponInfoManager class is responsible for handling weapon configuration data and state management for bot entities. It provides functionality to initialize weapon information, load and
   parse weapon configurations from files, and manage different weapon states for bots. The class supports allocating and freeing weapon states, validating weapon numbers, and choosing the best fight
   weapon based on inventory and weapon weights. It also handles parsing of weapon and projectile information from parsers, and provides mechanisms to reset weapon states and free allocated resources.
   The class uses fuzzy weight management for determining optimal weapon selections and maintains a collection of weapon information structures indexed by weapon state and weapon number.

*/
class idBotWeaponInfoManager
{
public:
	//! Initializes the bot weapon info manager by loading weapon configuration.
	void Init();

	//! Loads weapon weight configurations for a specified weapon state from a given file.
	int	 BotLoadWeaponWeights( int weaponstate, char* filename );

	//! Retrieves weapon information for a specified weapon state and weapon number.
	void BotGetWeaponInfo( int weaponstate, int weapon, bot_weaponinfo_t* weaponinfo );

	//! Selects the best fight weapon for a bot based on weapon state and inventory.
	int	 BotChooseBestFightWeapon( int weaponstate, int* inventory );

	//! Resets the bot weapon state to its initial configuration.
	void BotResetWeaponState( int weaponstate );

	//! Allocates and returns an available weapon state index for a bot.
	int	 BotAllocWeaponState();

	//! Frees a bot weapon state by marking it as unused and resetting its properties.
	void BotFreeWeaponState( int ws );

private:
	//! Validates that a weapon number is within the acceptable range for bot weapons.
	int				   BotValidWeaponNumber( int weaponnum );

	//! Returns a pointer to the bot weapon state corresponding to the given handle.
	bot_weaponstate_t* BotWeaponStateFromHandle( int handle );

	//! Initializes the weapon weight index for each weapon in the bot's weapon state using fuzzy weight management.
	void			   WeaponWeightIndex( weightconfig_t* wwc, bot_weaponstate_t* weaponState );

	//! Frees the weapon weights for the specified bot weapon state
	void			   BotFreeWeaponWeights( int weaponstate );

private:
	//! Loads bot weapon configuration data from a specified file.
	void LoadWeaponConfig( char* filename );

	//! Parses weapon information from a parser and populates a bot weapon info structure.
	void ParseWeaponInfo( idParser& parser, bot_weaponinfo_t& newWeaponInfo );

	//! Parses projectile information from a parser and stores it in the provided projectile info structure
	void ParseProjectileInfo( idParser& parser, projectileinfo_t& newProjectileInfo );

private:
	idList<projectileinfo_t> projectileinfo;
	bot_weaponstate_t		 botweaponstates[MAX_CLIENTS + 1];

	bot_weaponinfo_t		 weaponinfo[BOT_MAX_WEAPONS];
};

extern idBotWeaponInfoManager botWeaponInfoManager;
