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

#define MAX_BOT_ITEM_INFOS	2
#define MAX_BOT_ITEM_INFO	256
#define MAX_BOT_LEVEL_ITEMS 256

// #define DEBUG_AI_GOAL
#ifdef RANDOMIZE
	#define UNDECIDEDFUZZY
#endif // RANDOMIZE
#define DROPPEDWEIGHT
// minimum avoid goal time
#define AVOID_MINIMUM_TIME 10
// default avoid goal time
#define AVOID_DEFAULT_TIME 30
// avoid dropped goal time
#define AVOID_DROPPED_TIME 10
//
#define TRAVELTIME_SCALE   0.01
// item flags
#define IFL_NOTFREE		   1  // not in free for all
#define IFL_NOTTEAM		   2  // not in team play
#define IFL_NOTSINGLE	   4  // not in single player
#define IFL_NOTBOT		   8  // bot should never go for this
#define IFL_ROAM		   16 // bot roam goal

// a bot goal
struct bot_goal_t {
	//! Initializes a new instance of the bot_goal_t class and resets its state.
	bot_goal_t() { Reset(); }

	//! Resets all member variables of the bot_goal_t structure to their default values
	void Reset()
	{
		origin.Zero();
		areanum = 0;
		mins.Zero();
		maxs.Zero();
		entitynum = 0;
		number	  = 0;
		flags	  = 0;
		iteminfo  = 0;
		framenum  = -1;
	}

	int	   framenum;
	idVec3 origin;	   // origin of the goal
	int	   areanum;	   // area number of the goal
	idVec3 mins, maxs; // mins and maxs of the goal
	int	   entitynum;  // number of the goal entity
	int	   number;	   // goal number
	int	   flags;	   // goal flags
	int	   iteminfo;   // item information
};

// location in the map "target_location"
struct maplocation_t {
	//! Default constructor for maplocation_t that initializes all members to their default values.
	maplocation_t() { Reset(); }

	//! Resets the map location data to its default state.
	void Reset()
	{
		origin.Zero();
		areanum = 0;
		name	= "";
		next	= nullptr;
	}

	idVec3		   origin;
	int			   areanum;
	idStr		   name;
	maplocation_t* next;
};

// camp spots "info_camp"
struct campspot_t {
	//! Initializes a new instance of the campspot_t class and resets its state.
	campspot_t() { Reset(); }

	//! Resets all members of the campspot_t structure to their default values
	void Reset()
	{
		origin.Zero();
		areanum = 0;
		name	= "";
		range	= 0;
		weight	= 0;
		random	= 0;
	}

	idVec3 origin;
	int	   areanum;
	idStr  name;
	float  range;
	float  weight;
	float  wait;
	float  random;
};

struct levelitem_t {
	//! Initializes a new instance of levelitem_t and resets its state.
	levelitem_t() { Reset(); }

	//! Resets all member variables of the levelitem_t object to their default values
	void Reset()
	{
		number	 = 0;
		iteminfo = 0;
		flags	 = 0;
		weight	 = 0;
		origin.Zero();
		goalorigin.Zero();
		item	= nullptr;
		timeout = 0;
		prev	= nullptr;
		next	= nullptr;
	}

	idStr		 name;
	int			 number;	 // number of the level item
	int			 iteminfo;	 // index into the item info
	int			 flags;		 // item flags
	float		 weight;	 // fixed roam weight
	idVec3		 origin;	 // origin of the item
	idVec3		 goalorigin; // goal origin within the area
	// int entitynum;						//entity number
	idItem*		 item;
	float		 timeout; // item is removed after this time
	levelitem_t *prev, *next;
};

struct iteminfo_t {
	//! Initializes a new instance of iteminfo_t and resets its values.
	iteminfo_t() { Reset(); }

	//! Resets all member variables of the iteminfo_t structure to their default values.
	void Reset()
	{
		classname	= "";
		name		= "";
		model		= "";
		modelindex	= 0;
		type		= 0;
		index		= 0;
		respawntime = 0;
		mins.Zero();
		maxs.Zero();
		number = 0;
	}

	idStr  classname;	// classname of the item
	idStr  name;		// name of the item
	idStr  model;		// model of the item
	int	   modelindex;	// model index
	int	   type;		// item type
	int	   index;		// index in the inventory
	float  respawntime; // respawn time
	idVec3 mins;		// mins of the item
	idVec3 maxs;		// maxs of the item
	int	   number;		// number of the item info
};

//
// itemconfig_t
//
struct itemconfig_t {
	//! Initializes a new instance of itemconfig_t and resets its state.
	itemconfig_t() { Reset(); }

	//! Resets all item information fields and clears the item info array.
	void Reset()
	{
		numiteminfo = 0;
		for( int i = 0; i < MAX_BOT_ITEM_INFO; i++ ) {
			iteminfo[i].Reset();
		}
	}

	int		   numiteminfo;
	iteminfo_t iteminfo[MAX_BOT_ITEM_INFO];
};

// goal state
struct bot_goalstate_t {
	//! Initializes a new instance of the bot goal state and resets its internal data.
	bot_goalstate_t()
	{
		itemweightindex = NULL;
		Reset();
	}

	//! Resets the bot goal state to its initial empty configuration.
	void Reset()
	{
		itemweightconfig = NULL;

		if( itemweightindex != NULL ) { delete itemweightindex; }
		itemweightindex		 = NULL;
		client				 = -1;
		lastreachabilityarea = 0;
		goalstacktop		 = 0;

		for( int i = 0; i < MAX_AVOIDGOALS; i++ ) {
			avoidgoals[i]	  = 0;
			avoidgoaltimes[i] = 0;
		}
	}

	//! Destructor for the bot goal state structure that cleans up allocated memory for item weight indexing.
	~bot_goalstate_t()
	{
		if( itemweightindex != NULL ) { delete itemweightindex; }

		itemweightindex = NULL;
	}

	//! Checks if the bot goal state is currently in use by verifying that the client ID is not -1.
	bool			InUse() { return client != -1; }

	weightconfig_t* itemweightconfig; // weight config
	int*			itemweightindex;  // index from item to weight

	int				client;				  // client using this goal state
	int				lastreachabilityarea; // last area with reachabilities the bot was in

	bot_goal_t		goalstack[MAX_GOALSTACK]; // goal stack
	int				goalstacktop;			  // the top of the goal stack

	int				avoidgoals[MAX_AVOIDGOALS];		// goals to avoid
	float			avoidgoaltimes[MAX_AVOIDGOALS]; // times to avoid the goals
};

/*!
	\class idBotGoalManager
	\brief Manages bot goal states, item tracking, and goal selection for AI behavior.

	The idBotGoalManager class serves as the central component for handling bot goal-related functionality within the AI system. It manages the allocation and deallocation of goal states, tracks level
   items, and provides methods for selecting appropriate goals based on bot inventory, travel time, and item weights. The class supports operations for pushing and popping goals from a stack,
   resetting goal states, and avoiding specific goals for a defined period. It also handles loading and saving fuzzy logic configurations for goal selection, as well as parsing item and map location
   information from game entities. The system initializes and maintains lists of level items and entity information, supporting both immediate and long-term goal selection. Memory management for level
   items and goal states is handled through dedicated allocation and freeing methods.

*/
class idBotGoalManager
{
public:
	//! Initializes a new instance of the idBotGoalManager class.
	idBotGoalManager();

	//! Initializes the bot goal AI system by loading item configuration.
	int	 BotSetupGoalAI();

	//! Initializes the level items for bot goal management
	void InitLevelItems();

	//! Updates the list of entity items for bot goal management by checking existing items and adding new ones.
	void UpdateEntityItems();

	//! Adds a goal to the bot's goal stack for the specified goal state.
	void BotPushGoal( int goalstate, bot_goal_t* goal );

	//! Removes the top goal from the goal stack for the specified goal state
	void BotPopGoal( int goalstate );

	//! Clears the goal stack for a specified bot goal state.
	void BotEmptyGoalStack( int goalstate );

	//! Loads item weights for a bot goal state from a specified file
	int	 BotLoadItemWeights( int goalstate, char* filename );

	//! Resets the goal state for a bot, clearing its goal stack and avoiding goals.
	void BotResetGoalState( int goalstate );

	/*!
		\brief Checks if a bot item goal is visible in the bot's field of view but not currently visible to the player

		This function determines whether a bot item goal is within the bot's visual field but is not directly visible to the player. It calculates the middle point of the goal's bounding box and
	   performs a trace from the bot's eye position to this middle point. If the trace is unobstructed (fraction >= 1), it checks if the goal entity is valid. If the entity is valid, the function
	   returns true, indicating that the item is in view but not visible. This is useful for bots to detect items that are occluded but potentially accessible. The function is specifically designed
	   for item goals and will return false for non-item goals.

		\param viewer The entity number of the viewer
		\param eye The eye position of the viewer
		\param viewangles The view angles of the viewer
		\param goal Pointer to the bot goal structure to check
		\return True if the bot item goal is in the viewer's field of view but not currently visible, false otherwise
	*/
	int	 BotItemGoalInVisButNotVisible( int viewer, idVec3 eye, idAngles viewangles, bot_goal_t* goal );

	/*!
		\brief Chooses the best item goal for a bot based on inventory and travel factors.

		This function evaluates all items in the current level that are suitable for the bot to pursue. It considers the bot's inventory, travel time to the item, and various flags such as item
	   availability and respawn time. The function selects the item that offers the highest weighted value considering travel time and item importance. If a suitable item is found, it is added to the
	   bot's goal stack for pursuit.

		\param goalstate Handle to the bot goal state used to store and manage the bot's current goals
		\param origin Position of the bot in the world
		\param inventory Array containing the bot's current inventory items
		\param travelflags Travel flags that define the allowed travel types for the bot to reach the item
		\return The function returns true if a suitable item goal is found and added to the bot's goal stack, otherwise false
	*/
	int	 BotChooseLTGItem( int goalstate, idVec3 origin, int* inventory, int travelflags );

	/*!
		\brief Selects the best item goal for a bot based on proximity, weight, and travel time.

		This function evaluates all items in the current level that are eligible for bot interaction, taking into account the bot's inventory, travel flags, and the time it would take to reach each
	   item. It prioritizes items that are reachable within the specified maximum time and avoids items that are currently being avoided. If a long-term goal is specified, the function also considers
	   the travel time from the item back to that goal. The best item is then pushed onto the bot's goal stack for pursuit.

		\param goalstate Handle to the bot's goal state used to reference the bot's current goals and inventory.
		\param origin The 3D position from which the bot is evaluating item goals.
		\param inventory An array representing the bot's current inventory items.
		\param travelflags Flags that define the travel constraints for the bot when calculating travel time.
		\param ltg Pointer to a long-term goal structure used to evaluate if travel back to this goal is feasible after reaching an item.
		\param maxtime Maximum travel time allowed to reach an item before it is discarded as a viable goal.
		\return True if a suitable item goal was found and pushed onto the bot's goal stack, false otherwise.
	*/
	int	 BotChooseNBGItem( int goalstate, idVec3 origin, int* inventory, int travelflags, bot_goal_t* ltg, float maxtime );

	//! Checks if a bot touching a goal based on origin and goal parameters
	int	 BotTouchingGoal( idVec3 origin, bot_goal_t* goal );

	//! Allocates and returns a new goal state for the specified client.
	int	 BotAllocGoalState( int client );

	//! Retrieves the name of a bot goal item by its number and stores it in the provided buffer.
	void BotGoalName( int number, char* name, int size );

	//! Frees the bot goal state associated with the given handle.
	void BotFreeGoalState( int handle );

	//! Shuts down the bot goal AI system and frees all associated resources.
	void BotShutdownGoalAI();

	//! Frees the item weights associated with the specified bot goal state.
	void BotFreeItemWeights( int goalstate );

public:
	//! Checks if two 3D points are within 50 units of each other in 2D space.
	bool BotNearGoal( idVec3 p1, idVec3 p2 );

	//! Retrieves the top goal from the bot goal stack for the specified goal state.
	int	 BotGetTopGoal( int goalstate, bot_goal_t* goal );

	//! Retrieves the second most recent goal from the bot goal stack.
	int	 BotGetSecondGoal( int goalstate, bot_goal_t* goal );

public:
	//! Prints the current goal stack for a specified bot goal state.
	void BotDumpGoalStack( int goalstate );

public:
	//! Retrieves the goal information for a specified level item based on its index and name.
	int	  BotGetLevelItemGoal( int index, char* name, bot_goal_t* goal );

	//! Sets the avoid time for a specific goal in the bot's goal state.
	void  BotSetAvoidGoalTime( int goalstate, int number, float avoidtime );

	//! Returns the remaining time until a bot's avoid goal expires.
	float BotAvoidGoalTime( int goalstate, int number );

	//! Resets the avoid goals for a specified bot goal state.
	void  BotResetAvoidGoals( int goalstate );

	//! Dumps information about avoid goals for a specified bot goal state
	void  BotDumpAvoidGoals( int goalstate );

	//! Adds a goal number to the bot's avoid goals list with a specified avoid time.
	void  BotAddToAvoidGoals( bot_goalstate_t* gs, int number, float avoidtime );

	//! Removes a goal from the list of goals to avoid for a specified bot goal state.
	void  BotRemoveFromAvoidGoals( int goalstate, int number );

	//! Retrieves the goal information for a specified map location name.
	int	  BotGetMapLocationGoal( char* name, bot_goal_t* goal );

	//! Retrieves the next camp spot goal for a bot, returning the index of the next camp spot or zero if no valid camp spot exists.
	int	  BotGetNextCampSpotGoal( int num, bot_goal_t* goal );

	//! Finds a matching entity for a given level item based on model index and proximity.
	void  BotFindEntityForLevelItem( levelitem_t* li );

private:
	//! Allocates and returns a new level item from the free list.
	levelitem_t*	 AllocLevelItem();

	//! Frees a level item by adding it to the free list.
	void			 FreeLevelItem( levelitem_t* li );

	//! Adds a level item to the beginning of the level items list.
	void			 AddLevelItemToList( levelitem_t* li );

	//! Removes a level item from the linked list managed by the bot goal manager.
	void			 RemoveLevelItemFromList( levelitem_t* li );

	//! Frees all memory allocated for map location and camp spot entities.
	void			 BotFreeInfoEntities();

	//! Initializes bot goal entities by parsing map locations and camp spots from game entities.
	void			 BotInitInfoEntities();

	//! Initializes the level item heap for bot goal management.
	void			 InitLevelItemHeap();

	//! Returns the weight index for items based on the provided weight and item configuration.
	int*			 ItemWeightIndex( weightconfig_t* iwc, itemconfig_t* ic );

	//! Loads item configuration from a specified file and returns a pointer to the parsed configuration data.
	itemconfig_t*	 LoadItemConfig( char* filename );

	//! Saves the fuzzy logic configuration for a bot goal state to a file.
	void			 BotSaveGoalFuzzyLogic( int goalstate, char* filename );

	//! Mutates the fuzzy logic weights for a bot goal state within the specified range.
	void			 BotMutateGoalFuzzyLogic( int goalstate, float range );

	//! Retrieves a goal state from a handle, returning NULL if the handle is invalid.
	bot_goalstate_t* BotGoalStateFromHandle( int handle );

	//! Interbreeds fuzzy logic configurations from two parent bot goal states to create a child bot goal state.
	void			 BotInterbreedGoalFuzzyLogic( int parent1, int parent2, int child );

private:
	//! Parses item information from a parser and stores it in the provided item info structure.
	void ParseItemInfo( idParser& parser, iteminfo_t* itemInfo );

private:
	bot_goalstate_t		  botgoalstates[MAX_CLIENTS + 1];

	// item configuration
	itemconfig_t		  itemconfiglocal;
	itemconfig_t*		  itemconfig;

	// level items
	levelitem_t			  levelitemheap[MAX_BOT_LEVEL_ITEMS];
	levelitem_t*		  freelevelitems;
	levelitem_t*		  levelitems;
	int					  numlevelitems;

	// map locations
	idList<maplocation_t> maplocations;
	idList<campspot_t>	  campspots;
};

extern idBotGoalManager botGoalManager;
