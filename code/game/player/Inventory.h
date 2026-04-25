/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2026 Robert Beckebans

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
#ifndef __PLAYER_INVENTORY_H__
#define __PLAYER_INVENTORY_H__

const int MAX_WEAPONS = 32;

// powerups - the "type" in item .def must match
enum {
	BERSERK = 0,
	INVISIBILITY,
	MEGAHEALTH,
	ADRENALINE,
	INVULNERABILITY,
	HELLTIME,
	ENVIROSUIT,
	// HASTE,
	ENVIROTIME,
	MAX_POWERUPS
};

// powerup modifiers
enum { SPEED = 0, PROJECTILE_DAMAGE, MELEE_DAMAGE, MELEE_DISTANCE };

typedef struct {
	int	 ammo;
	int	 rechargeTime;
	char ammoName[128];
} RechargeAmmo_t;

struct idObjectiveInfo {
	idStr			  title;
	idStr			  text;
	const idMaterial* screenshot;
};

struct idLevelTriggerInfo {
	idStr levelName;
	idStr triggerName;
};

/*!
	\class idInventory
	\brief Manages player inventory including items, ammo, armor, and power-ups.

	Handles the storage, retrieval, and modification of player inventory data such as weapons, ammo, armor, and temporary power-ups. Supports saving and restoring inventory states, giving and taking
   items, managing ammo counts and weapon clips, and updating inventory display elements. Provides functionality for checking inventory limits, managing ammo consumption and replenishment, and
   synchronizing inventory state across networked clients.

*/
class idInventory
{
public:
	int												  maxHealth;
	int												  weapons;
	int												  powerups;
	int												  armor;
	int												  maxarmor;
	int												  powerupEndTime[MAX_POWERUPS];

	RechargeAmmo_t									  rechargeAmmo[AMMO_NUMTYPES];

	// mp
	int												  ammoPredictTime; // Unused now but kept for save file compatibility.

	int												  deplete_armor;
	float											  deplete_rate;
	int												  deplete_ammount;
	int												  nextArmorDepleteTime;

	int												  pdasViewed[4]; // 128 bit flags for indicating if a pda has been viewed

	int												  selPDA;
	int												  selEMail;
	int												  selVideo;
	int												  selAudio;
	bool											  pdaOpened;
	idList<idDict*>									  items;
	idList<idStr>									  pdaSecurity;
	idList<const idDeclPDA*>						  pdas;
	idList<const idDeclVideo*>						  videos;
	idList<const idDeclEmail*>						  emails;

	bool											  ammoPulse;
	bool											  weaponPulse;
	bool											  armorPulse;
	int												  lastGiveTime;

	idList<idLevelTriggerInfo, TAG_IDLIB_LIST_PLAYER> levelTriggers;

	//! Initializes a new instance of the idInventory class.
	idInventory() { Clear(); }
	~idInventory() { Clear(); }

	//! Saves the inventory state to a save game file.
	void					Save( idSaveGame* savefile ) const;

	//! Restores the inventory state from a save game file.
	void					Restore( idRestoreGame* savefile );

	//! Resets all inventory data to its initial state.
	void					Clear();

	//! Sets a power-up for the specified player with a duration in milliseconds.
	void					GivePowerUp( idPlayer* player, int powerup, int msec );

	//! Clears all power-ups by resetting their end times and disabling all active power-ups.
	void					ClearPowerUps();

	//! Populates a dictionary with persistent inventory data for saving.
	void					GetPersistantData( idDict& dict );

	//! Restores the inventory state for a player from a dictionary.
	void					RestoreInventory( idPlayer* owner, const idDict& dict );

	/*!
		\brief Gives inventory items to a player based on the specified statname and value, with optional hud updates and weapon switching.

		This function handles the distribution of various inventory items such as ammo, armor, power-ups, and weapons to a player. It supports different flags for updating game state and providing
	   feedback. The function checks for maximum limits on ammo and armor and handles weapon pickup logic including setting ideal weapon and updating quick slots. It returns true if a weapon was
	   picked up, false otherwise.

		\param owner The player receiving the inventory item
		\param spawnArgs Dictionary containing spawn arguments for entity definitions
		\param statname Name of the stat being given (e.g., ammo_, armor, weapon)
		\param value Value to be assigned or added for the specified stat
		\param idealWeapon Pointer to predicted value for ideal weapon selection
		\param updateHud Whether to update the HUD with the new item
		\param giveFlags Flags that control how the item is given (state update, feedback)
		\return True if a weapon was taken, false otherwise
	*/
	bool					Give( idPlayer* owner, const idDict& spawnArgs, const char* statname, const char* value, idPredictedValue<int>* idealWeapon, bool updateHud, unsigned int giveFlags );

	//! Removes a weapon and its associated ammo from the inventory.
	void					Drop( const idDict& spawnArgs, const char* weapon_classname, int weapon_index );

	//! Returns the ammo index for the specified ammo class name.
	ammo_t					AmmoIndexForAmmoClass( const char* ammo_classname ) const;

	//! Returns the maximum ammo value for a given ammo class name from the owner's spawn arguments.
	int						MaxAmmoForAmmoClass( const idPlayer* owner, const char* ammo_classname ) const;

	//! Returns the weapon index for a given ammo class from spawn arguments.
	int						WeaponIndexForAmmoClass( const idDict& spawnArgs, const char* ammo_classname ) const;

	//! Returns the ammo index for a given weapon class name.
	ammo_t					AmmoIndexForWeaponClass( const char* weapon_classname, int* ammoRequired );

	//! Returns the ammo pickup name for the specified ammo index.
	const char*				AmmoPickupNameForIndex( ammo_t ammonum ) const;

	//! Adds a pickup name to the inventory, avoiding duplicates.
	void					AddPickupName( const char* name, idPlayer* owner );

	//! Returns the number of shots that can be fired with the specified ammo type and amount.
	int						HasAmmo( ammo_t type, int amount );

	//! Attempts to use the specified amount of ammo of the given type, returning true if successful.
	bool					UseAmmo( ammo_t type, int amount );

	//! Returns the total amount of ammo available for a specified weapon class, optionally including the ammo in the weapon's clip.
	int						HasAmmo( const char* weapon_classname, bool includeClip = false, idPlayer* owner = NULL );

	//! Determines if a weapon's clip is empty and cannot be refilled due to insufficient ammo.
	bool					HasEmptyClipCannotRefill( const char* weapon_classname, idPlayer* owner );

	//! Updates the armor value by depleting it over time based on configured rates and thresholds.
	void					UpdateArmor();

	//! Sets the ammo amount for a specific ammo type in the inventory.
	void					SetInventoryAmmoForType( const int ammoType, const int amount );

	//! Sets the clip ammo amount for a specified weapon
	void					SetClipAmmoForWeapon( const int weapon, const int amount );

	//! Retrieves the amount of ammo of the specified type stored in the inventory.
	int						GetInventoryAmmoForType( const int ammoType ) const;

	//! Returns the clip ammo amount for the specified weapon.
	int						GetClipAmmoForWeapon( const int weapon ) const;

	//! Writes all ammo and clip information to a snapshot message for network synchronization.
	void					WriteAmmoToSnapshot( idBitMsg& msg ) const;

	//! Reads ammo and clip data from a snapshot message for the specified owner entity
	void					ReadAmmoFromSnapshot( const idBitMsg& msg, int ownerEntityNumber );

	//! Sets the ammo count for all ammo types in the inventory to 999 for the specified owner entity.
	void					SetRemoteClientAmmo( const int ownerEntityNumber );

	int						nextItemPickup;
	int						nextItemNum;
	int						onePickupTime;
	idList<idStr>			pickupItemNames;
	idList<idObjectiveInfo> objectiveNames;

	//! Initializes recharge ammo values for the given player by parsing spawn arguments.
	void					InitRechargeAmmo( idPlayer* owner );

	//! Updates ammo amounts for types that recharge based on elapsed time since last recharge
	void					RechargeAmmo( idPlayer* owner );

	/*!
		\brief Checks if the inventory can give a specified item or ammo to a player based on max ammo limits and statname conditions

		This function determines whether an item or ammo can be given to a player by checking the statname against specific conditions. For the "ammo_bloodstone" statname, it evaluates whether the
	   current ammo level is below the maximum allowed. If the maximum is exceeded, it sets the ammo to the maximum and returns false. For other statnames like "item", "icon", or "name", it returns
	   false to prevent them from being considered successful gives. For all other cases, it returns true, allowing the item to be given

		\param owner Player object that owns the inventory
		\param spawnArgs Dictionary containing spawn arguments for the item being given
		\param statname Name of the stat being checked
		\param value Value associated with the stat being checked
		\return True if the item or ammo can be given, false otherwise
	*/
	bool					CanGive( idPlayer* owner, const idDict& spawnArgs, const char* statname, const char* value );

public:
	idArray<idPredictedValue<int>, AMMO_NUMTYPES> ammo;
	idArray<idPredictedValue<int>, MAX_WEAPONS>	  clip;
};

#endif /* !__PLAYER_INVENTORY_H__ */
