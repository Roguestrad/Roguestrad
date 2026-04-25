/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#ifndef __GAME_ITEM_H__
#define __GAME_ITEM_H__

/*
===============================================================================

  Items the player can pick up or use.

===============================================================================
*/

/*
================================================
These flags are passed to the Give functions
to set their behavior. We need to be able to
separate the feedback from the actual
state modification so that we can hide lag
on MP clients.

For the previous behavior of functions which
take a giveFlags parameter (this is usually
desired on the server too) pass
ITEM_GIVE_FEEDBACK | ITEM_GIVE_UPDATE_STATE.
================================================
*/
enum itemGiveFlags_t {
	ITEM_GIVE_FEEDBACK	   = BIT( 0 ),
	ITEM_GIVE_UPDATE_STATE = BIT( 1 ),
	ITEM_GIVE_FROM_WEAPON  = BIT( 2 ), // indicates this was given via a weapon's launchPowerup (for bloodstone powerups)
};

/*!
	\class idItem
	\brief Represents an interactive item entity in the game world that can be picked up, spawned, and managed by players.

	The idItem class serves as a base implementation for interactive objects within the game world, handling their spawning, player interaction, and state persistence. It inherits from idEntity and
   provides mechanisms for item pickup logic, rendering updates, and client-server synchronization. The class manages item-specific behaviors such as spinning animations, highlight effects, and
   respawn mechanics. It supports both server-side and client-side operations including prediction, event handling, and snapshot serialization for networked gameplay. The item's state can be saved and
   restored, making it compatible with game save systems. It also provides callbacks for rendering and model management to ensure proper visual presentation.

*/
class idItem : public idEntity
{
public:
	CLASS_PROTOTYPE( idItem );

	//! Initializes a new instance of the idItem class with default values.
	idItem();

	//! Destructor for the idItem class that cleans up the highlight shell entity.
	virtual ~idItem();

	//! Saves the item's state to a save game file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the item's state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the item entity when it is spawned in the game world
	void		 Spawn();

	//! Populates the given attributes dictionary with inventory-related key-value pairs from the item's spawn arguments.
	void		 GetAttributes( idDict& attributes ) const;

	//! Transfers the item to the specified player, returning true if the transfer was successful.
	virtual bool GiveToPlayer( idPlayer* player, unsigned int giveFlags );

	//! Handles the pickup logic for an item by giving it to a player and managing related game state.
	virtual bool Pickup( idPlayer* player );

	//! Updates the item's think state and applies spinning animation if enabled.
	virtual void Think();

	//! Updates the item's presentation state and renders any highlight shell effects.
	virtual void Present();

	//! Returns the model index of the item.
	int			 GetModelIndex() const { return modelindex; }
	// jmarshall end

	enum { EVENT_PICKUP = idEntity::EVENT_MAXEVENTS, EVENT_RESPAWN, EVENT_RESPAWNFX, EVENT_TAKEFLAG, EVENT_DROPFLAG, EVENT_FLAGRETURN, EVENT_FLAGCAPTURE, EVENT_MAXEVENTS };

	//! Executes client-side thinking logic for the item, considering the current time, interpolation fraction, and prediction status.
	void		 ClientThink( const int curTime, const float fraction, const bool predict );

	//! Executes client-side prediction thinking for the item.
	virtual void ClientPredictionThink();

	//! Handles client-side events for item entities, returning true if the event was processed, false otherwise.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Writes the hidden state of the item to a snapshot message.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads item state from a snapshot message, handling visibility and prediction fixes.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

protected:
	//! Returns the number of milliseconds to predict item pickup.
	int GetPredictPickupMilliseconds() const { return clientPredictPickupMilliseconds; }

private:
	idVec3			  orgOrigin;
	bool			  spin;
	bool			  pulse;
	bool			  canPickUp;
	// jmarshall
	int				  modelindex;
	// jmarshall end

	// for item pulse effect
	int				  itemShellHandle;
	const idMaterial* shellMaterial;

	// used to update the item pulse effect
	mutable bool	  inView;
	mutable int		  inViewTime;
	mutable int		  lastCycle;
	mutable int		  lastRenderViewTime;

	// used for prediction in mp
	int				  clientPredictPickupMilliseconds;

	//! Updates the render entity for the item based on the current render view and returns true if updated
	bool			  UpdateRenderEntity( renderEntity_s* renderEntity, const renderView_t* renderView ) const;

	//! Handles model callback events for item entities during rendering
	static bool		  ModelCallback( renderEntity_s* renderEntity, const renderView_t* renderView );

	//! Drops the item to the floor by tracing down from its current position and setting the origin to the trace end position.
	void			  Event_DropToFloor();

	//! Handles the event when the item is touched by another entity.
	void			  Event_Touch( idEntity* other, trace_t* trace );

	//! Handles the trigger event for an item, allowing it to be picked up by a player.
	void			  Event_Trigger( idEntity* activator );

	//! Respawns the item on the server and makes it active for interaction.
	void			  Event_Respawn();

	//! Displays the respawn effect for the item.
	void			  Event_RespawnFx();
};

/*!
	\class idItemPowerup
	\brief A class representing a power-up item that can be given to players in the game.
*/
class idItemPowerup : public idItem
{
public:
	CLASS_PROTOTYPE( idItemPowerup );

	//! Initializes a new instance of the idItemPowerup class with default values for time and type.
	idItemPowerup();

	//! Saves the item powerup data to a save file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the powerup state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the power-up item with its time and type properties from spawn arguments.
	void		 Spawn();

	//! Gives the power-up item to the specified player and returns true if successful.
	virtual bool GiveToPlayer( idPlayer* player, unsigned int giveFlags );

private:
	int time;
	int type;
};

/*!
	\class idObjective
	\brief Manages in-game objectives and their display states.

	The idObjective class handles the lifecycle and visualization of game objectives. It inherits from idItem and provides functionality for saving and restoring objective state, initializing
   objective properties during spawning, and managing objective display through events. The class supports triggering objectives to show text and update related objectives, as well as hiding
   objectives based on player proximity or scheduled delays. It maintains the objective's screenshot material for visualization purposes.

*/
class idObjective : public idItem
{
public:
	CLASS_PROTOTYPE( idObjective );

	//! Initializes a new instance of the idObjective class.
	idObjective();

	//! Saves the objective data to a save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the objective state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes the objective by setting up its screenshot material.
	void Spawn();

private:
	idVec3			  playerPos;
	const idMaterial* screenshot;

	//! Triggers the objective event, displaying objective text and updating related objectives.
	void			  Event_Trigger( idEntity* activator );

	//! Hides the objective widget for the player if the player is far enough away, otherwise schedules a delayed hide event.
	void			  Event_HideObjective( idEntity* e );

	//! Retrieves the player's position and hides the objective after a short delay.
	void			  Event_GetPlayerPos();
};

/*!
	\class idVideoCDItem
	\brief Represents an item that can be given to a player and provides transformation functionality.
*/
class idVideoCDItem : public idItem
{
public:
	CLASS_PROTOTYPE( idVideoCDItem );

	//! Returns true if the video CD item is successfully given to the player, false otherwise.
	virtual bool GiveToPlayer( idPlayer* player, unsigned int giveFlags );

	//! Returns the transformation from physics to visual space for the video CD item.
	virtual bool GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );
};

/*!
	\class idPDAItem
	\brief Represents a PDA item that can be given to a player.
*/
class idPDAItem : public idItem
{
public:
	CLASS_PROTOTYPE( idPDAItem );

	//! Gives the PDA item to the specified player
	virtual bool GiveToPlayer( idPlayer* player, unsigned int giveFlags );
};

/*!
	\class idMoveableItem
	\brief A moveable item entity that handles physics, collision, and interactive behavior in the game world.

	This class represents a moveable item that can be interacted with by players and affected by physics simulation. It inherits from idItem and extends its functionality to support dynamic movement,
   collision detection, and pickup behavior. The class manages the item's physical properties including mass, friction, and bounce characteristics. It handles both client-side and server-side updates
   for synchronization and supports save/restore functionality. The item can be picked up by players, dropped at specific locations with physics properties, and can gib under certain damage
   conditions. The class also supports trigger-based interactions and particle effects for visual feedback during gameplay.

*/
class idMoveableItem : public idItem
{
public:
	CLASS_PROTOTYPE( idMoveableItem );

	//! Initializes a new instance of the idMoveableItem class.
	idMoveableItem();

	//! Destructor for the idMoveableItem class that cleans up the trigger member.
	virtual ~idMoveableItem();

	//! Saves the moveable item's state to a save file
	void			 Save( idSaveGame* savefile ) const;

	//! Restores the state of the moveable item from a save file
	void			 Restore( idRestoreGame* savefile );

	//! Initializes the moveable item's physics and collision properties.
	void			 Spawn();

	//! Updates the moveable item's physics and particle effects.
	virtual void	 Think();

	//! Updates the moveable item's client-side state including physics interpolation and trigger positioning.
	void			 ClientThink( const int curTime, const float fraction, const bool predict );

	//! Determines if a moveable item should collide with a surface based on impact velocity and triggers a bounce sound if appropriate.
	virtual bool	 Collide( const trace_t& collision, const idVec3& velocity );

	//! Attempts to pick up the moveable item for the specified player and clears its contents upon successful pickup.
	virtual bool	 Pickup( idPlayer* player );

	//! Drops items from an animated entity at specified joints with given rotations and offsets.
	static void		 DropItems( idAnimatedEntity* ent, const char* type, idList<idEntity*>* list );

	/*!
		\brief Spawns and returns a new entity of the specified class at the given origin with the provided physics properties and delays.

		This function creates a new entity with the specified classname and sets its properties including position, orientation, and velocity. The entity is spawned with the dropped flag set to true
	   and nodrop flag set to true to prevent it from settling on the floor. If an activate delay is specified, the entity will be activated after the delay period. A default remove delay of 5 minutes
	   is applied if no remove delay is specified, ensuring that dropped items are cleaned up even if they end up in unreachable locations.

		\param classname The class name of the entity to be spawned
		\param origin The world coordinates where the entity will be positioned
		\param axis The orientation of the entity as a rotation matrix
		\param velocity The initial linear velocity of the entity
		\param activateDelay The delay in milliseconds before activating the spawned entity, or 0 if no delay
		\param removeDelay The delay in milliseconds before removing the spawned entity, or 0 if using the default 5 minute delay
		\return A pointer to the newly spawned entity, or NULL if the spawn failed.
	*/
	static idEntity* DropItem( const char* classname, const idVec3& origin, const idMat3& axis, const idVec3& velocity, int activateDelay, int removeDelay );

	//! Writes the moveable item's state to a snapshot message.
	virtual void	 WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the moveable item's state from a snapshot message and updates visibility and trigger contents accordingly.
	virtual void	 ReadFromSnapshot( const idBitMsg& msg );

protected:
	idPhysics_RigidBody	  physicsObj;
	idClipModel*		  trigger;
	const idDeclParticle* smoke;
	int					  smokeTime;

	int					  nextSoundTime;
	bool				  repeatSmoke; // never stop updating the particles

	//! Makes the moveable item gib by spawning smoke and removing the entity.
	void				  Gib( const idVec3& dir, const char* damageDefName );

	//! Drops the moveable item to the floor using physics.
	void				  Event_DropToFloor();

	//! Causes the moveable item to gib using the specified damage definition.
	void				  Event_Gib( const char* damageDefName );
};

/*!
	\class idItemTeam
	\brief Manages team-based items such as flags in multiplayer game modes.

	The idItemTeam class extends idMoveableItem to provide functionality for team-based items like flags in multiplayer environments. It handles the core behaviors including pickup, dropping,
   returning, and capturing of team items. The class manages item state synchronization across the network through snapshot mechanisms and updates player interfaces to reflect flag status and team
   scores. It supports spawning nugget items at drop locations and handles various game events related to flag actions such as taking, dropping, returning, and capturing. The class integrates with the
   physics system to manage item movement and with the rendering system to present visual effects like glowing lights. Memory management is handled through standard destructor and light definition
   freeing mechanisms.

*/
class idItemTeam : public idMoveableItem
{
public:
	CLASS_PROTOTYPE( idItemTeam );

	//! Initializes a new instance of the idItemTeam class with default values.
	idItemTeam();

	//! Destroys an idItemTeam object and frees its associated light definition.
	virtual ~idItemTeam();

	//! Initializes the item team object with team-specific properties and physics settings
	void		 Spawn();

	//! Attempts to pick up the team item for the specified player.
	virtual bool Pickup( idPlayer* player );

	//! Handles client-side events related to team items like flag capture, drop, and take actions.
	virtual bool ClientReceiveEvent( int event, int time, const idBitMsg& msg );

	//! Executes the team item's think logic including trigger touching and nugget spawning
	virtual void Think();

	//! Drops the item team flag, optionally indicating if the drop was caused by the death of the carrier.
	void		 Drop( bool death = false );

	//! Returns the item team flag.
	void		 Return( idPlayer* player = NULL );

	//! Notifies the team that a flag has been captured.
	void		 Capture();

	//! Frees the light definition handle for the item glow effect.
	virtual void FreeLightDef();

	//! Updates the visual presentation of the item team entity.
	virtual void Present();

	//! Writes the item team's state to a network snapshot message.
	virtual void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads the team item state from a network snapshot message.
	virtual void ReadFromSnapshot( const idBitMsg& msg );

public:
	int	 team;
	// TODO : turn this into a state :
	bool carried; // is it beeing carried by a player?
	bool dropped; // was it dropped?

private:
	idVec3			  returnOrigin;
	idMat3			  returnAxis;
	int				  lastDrop;

	const idDeclSkin* skinDefault;
	const idDeclSkin* skinCarried;

	const function_t* scriptTaken;
	const function_t* scriptDropped;
	const function_t* scriptReturned;
	const function_t* scriptCaptured;

	renderLight_t	  itemGlow; // Used by flags when they are picked up
	int				  itemGlowHandle;

	int				  lastNuggetDrop;
	const char*		  nuggetName;

private:
	//! Handles the event when a player takes the team flag in multiplayer mode.
	void		Event_TakeFlag( idPlayer* player );

	//! Handles the event when a team flag is dropped, either by death or normal drop.
	void		Event_DropFlag( bool death );

	//! Handles the event when a flag is returned in a team-based game mode.
	void		Event_FlagReturn( idPlayer* player = NULL );

	//! Handles the event when a team captures a flag in a multiplayer game.
	void		Event_FlagCapture();

	//! Returns the team item to its original position and resets its state.
	void		PrivateReturn();

	//! Loads a script function by name from the spawn arguments.
	function_t* LoadScript( const char* script );

	//! Spawns a nugget item at the specified position with random orientation and velocity.
	void		SpawnNugget( idVec3 pos );

	//! Updates the HUDs of all players with the current flag status and team scores.
	void		UpdateGuis();
};

/*!
	\class idMoveablePDAItem
	\brief A moveable item that represents a PDA item which can be given to players.
*/
class idMoveablePDAItem : public idMoveableItem
{
public:
	CLASS_PROTOTYPE( idMoveablePDAItem );

	//! Gives the PDA item to the specified player, updating the player's PDA state if requested.
	virtual bool GiveToPlayer( idPlayer* player, unsigned int giveFlags );
};

/*!
	\class idItemRemover
	\brief Manages the removal of inventory items from players.
*/
class idItemRemover : public idEntity
{
public:
	CLASS_PROTOTYPE( idItemRemover );

	//! Initializes the item remover entity.
	void Spawn();

	//! Removes a specified inventory item from the player.
	void RemoveItem( idPlayer* player );

private:
	//! Removes an item from the player when triggered.
	void Event_Trigger( idEntity* activator );
};

/*!
	\class idObjectiveComplete
	\brief Manages objective completion events and player interaction.

	This class handles the logic for completing objectives within the game, including triggering events when objectives are met and managing the visibility of objective-related UI elements. It
   inherits from idItemRemover, suggesting it may be involved in removing or managing items related to completed objectives. The class is designed to initialize objective completion entities, save and
   restore their state, and respond to player actions such as triggering events or getting player position. It manages the hiding of objective UI elements based on player distance to provide a
   seamless gameplay experience.

*/
class idObjectiveComplete : public idItemRemover
{
public:
	CLASS_PROTOTYPE( idObjectiveComplete );

	//! Initializes a new instance of the idObjectiveComplete class.
	idObjectiveComplete();

	//! Saves the player position to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the objective complete state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes the objective completion entity by disabling it and hiding it from view.
	void Spawn();

private:
	idVec3 playerPos;

	//! Processes the trigger event for an objective completion entity.
	void   Event_Trigger( idEntity* activator );

	//! Hides the objective UI element for the player when the player is far enough away from the objective.
	void   Event_HideObjective( idEntity* e );

	//! Retrieves the player's position and hides the objective
	void   Event_GetPlayerPos();
};

#endif /* !__GAME_ITEM_H__ */
