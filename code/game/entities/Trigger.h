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

#ifndef __GAME_TRIGGER_H__
#define __GAME_TRIGGER_H__

extern const idEventDef EV_Enable;
extern const idEventDef EV_Disable;

/*!
	\class idTrigger
	\brief A trigger entity that executes script functions when activated.

	This class represents a trigger entity that can execute associated script functions when activated. It inherits from idEntity and manages its own physics contents to control when it triggers. The
   trigger can be enabled or disabled, and supports saving and restoring its state. It provides mechanisms to call the associated script function and draw debug information. The trigger's
   functionality is controlled through physics contents and clipping settings.

*/
class idTrigger : public idEntity
{
public:
	CLASS_PROTOTYPE( idTrigger );

	//! Draws debug information for trigger entities in the game world
	static void DrawDebugInfo();

	//! Constructs an idTrigger object with no script function.
	idTrigger();

	//! Initializes the trigger entity by setting its physics contents and script function.
	void			  Spawn();

	//! Returns the script function associated with this trigger.
	const function_t* GetScriptFunction() const;

	//! Saves the trigger's script function name to the save file.
	void			  Save( idSaveGame* savefile ) const;

	//! Restores the trigger's script function from a save file.
	void			  Restore( idRestoreGame* savefile );

	//! Enables the trigger by setting its physics contents to CONTENTS_TRIGGER and enabling clipping.
	virtual void	  Enable();

	//! Disables the trigger by clearing its contents and disabling clipping.
	virtual void	  Disable();

protected:
	//! Calls the script function associated with this trigger.
	void			  CallScript() const;

	//! Enables the trigger functionality.
	void			  Event_Enable();

	//! Disables the trigger by calling the internal Disable method.
	void			  Event_Disable();

	const function_t* scriptFunction;
};

/*!
	\class idTrigger_Multi
	\brief A trigger entity that can activate multiple targets based on various conditions and timing.

	This class represents a multi-trigger entity that extends the basic trigger functionality. It manages trigger activation based on player interaction, timing, and facing conditions. The entity can
   be configured to activate multiple targets and handles complex triggering logic including delay mechanisms and facing angle checks. The class supports persistence through save and restore
   operations, allowing the trigger state to be maintained across game sessions. The trigger behavior is controlled through spawn arguments and can be activated through touch events or direct trigger
   events.

*/
class idTrigger_Multi : public idTrigger
{
public:
	CLASS_PROTOTYPE( idTrigger_Multi );

	//! Constructs a new idTrigger_Multi object with default initialization.
	idTrigger_Multi();

	//! Initializes the trigger multi with spawn arguments and sets up its triggering behavior.
	void Spawn();

	//! Saves the trigger multi state to the specified save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the trigger multi from a save file.
	void Restore( idRestoreGame* savefile );

protected:
	float wait;
	float random;
	float delay;
	float random_delay;
	int	  nextTriggerTime;
	idStr
		requires;
	int	 removeItem;
	bool touchClient;
	bool touchOther;
	bool triggerFirst;
	bool triggerWithSelf;

	//! Checks if the activator is facing the trigger within the specified angle limit
	bool CheckFacing( idEntity* activator );

	//! Executes the trigger action, activating targets and handling timing logic.
	void TriggerAction( idEntity* activator );

	//! Handles the trigger action event for the multi-trigger entity.
	void Event_TriggerAction( idEntity* activator );

	//! Handles the trigger activation event for a multi-trigger entity.
	void Event_Trigger( idEntity* activator );

	//! Handles touch events for trigger entities, determining whether to activate the trigger based on various conditions.
	void Event_Touch( idEntity* other, trace_t* trace );
};

/*!
	\class idTrigger_EntityName
	\brief A trigger entity that activates when a specific entity name enters its bounds.

	This class extends the base trigger functionality to specifically respond to entities with a designated name. It handles triggering actions when matching entities touch or activate the trigger,
   with support for delayed execution and script calling. The trigger maintains its state through save and restore operations, and initializes its properties during spawn. The implementation supports
   both direct activation and touch-based triggering events, with proper validation of activating entities.

*/
class idTrigger_EntityName : public idTrigger
{
public:
	CLASS_PROTOTYPE( idTrigger_EntityName );

	//! Constructs a new idTrigger_EntityName object with default values.
	idTrigger_EntityName();

	//! Saves the trigger entity name state to the provided save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the trigger entity name state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes the trigger entity by parsing spawn arguments and setting up its properties.
	void Spawn();

private:
	float wait;
	float random;
	float delay;
	float random_delay;
	int	  nextTriggerTime;
	bool  triggerFirst;
	idStr entityName;
	bool  testPartialName;

	//! Executes the trigger action, activating targets and calling the script, with optional timed removal.
	void  TriggerAction( idEntity* activator );

	//! Handles the trigger action event when the entity is activated.
	void  Event_TriggerAction( idEntity* activator );

	//! Handles the trigger event for an entity name trigger, validating the activating entity and managing delayed actions.
	void  Event_Trigger( idEntity* activator );

	//! Handles the touch event for an entity name trigger, executing actions when a valid entity touches it.
	void  Event_Touch( idEntity* other, trace_t* trace );
};

/*!
	\class idTrigger_Timer
	\brief A trigger entity that executes actions at timed intervals.

	The idTrigger_Timer class implements a trigger entity that can be configured to fire events at regular time intervals. It inherits from idTrigger and extends its functionality to support
   time-based triggering. The class manages the timer state including enabling, disabling, and scheduling of events. It supports serialization through Save and Restore methods for persistence. The
   Spawn method initializes the timer properties from spawn arguments, while Event_Timer handles the actual execution of triggered actions. Event_Use provides a toggle interface for controlling the
   timer state, allowing it to be started or stopped based on external activation.

*/
class idTrigger_Timer : public idTrigger
{
public:
	CLASS_PROTOTYPE( idTrigger_Timer );

	//! Initializes a new instance of the idTrigger_Timer class with default values.
	idTrigger_Timer();

	//! Saves the timer trigger state to a save file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the timer trigger state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Spawns the timer trigger and initializes its properties from spawn arguments.
	void		 Spawn();

	//! Enables the timer trigger if it is currently disabled.
	virtual void Enable();

	//! Disables the timer trigger if it is currently enabled.
	virtual void Disable();

private:
	float random;
	float wait;
	bool  on;
	float delay;
	idStr onName;
	idStr offName;

	//! Executes the timer event by activating targets and scheduling the next firing.
	void  Event_Timer();

	//! Toggles the timer state and schedules or cancels a timer event based on the current state.
	void  Event_Use( idEntity* activator );
};

/*!
	\class idTrigger_Count
	\brief A trigger entity that activates when a specified count is reached.

	This class extends the base trigger functionality to support count-based activation logic. The trigger maintains an internal counter that increments each time it is triggered, and activates
   associated actions when the counter reaches a predefined goal value. The class supports persistence through save and restore operations, allowing the trigger state to be maintained across game
   sessions. The trigger can be configured with a goal count and delay values to control its behavior.

*/
class idTrigger_Count : public idTrigger
{
public:
	CLASS_PROTOTYPE( idTrigger_Count );

	//! Initializes a new instance of the idTrigger_Count class with default values for goal, count, and delay.
	idTrigger_Count();

	//! Saves the trigger count state to the specified save file
	void Save( idSaveGame* savefile ) const;

	//! Restores the trigger count state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes the trigger count behavior with specified parameters.
	void Spawn();

private:
	int	  goal;
	int	  count;
	float delay;

	//! Handles the trigger event for a count-based trigger, incrementing the counter and activating associated actions when the goal is reached.
	void  Event_Trigger( idEntity* activator );

	//! Triggers the count trigger action, activating targets and calling the script, then removing the entity if the goal count has been reached.
	void  Event_TriggerAction( idEntity* activator );
};

/*!
	\class idTrigger_Hurt
	\brief A trigger entity that applies damage to entities touching it.

	This class implements a trigger entity that deals damage to any entity that comes into contact with it. It inherits from idTrigger and extends its functionality to include damage application. The
   trigger can be enabled or disabled through events, and its state is saved and restored during game sessions. The trigger's behavior is configured through spawn arguments during initialization.

*/
class idTrigger_Hurt : public idTrigger
{
public:
	CLASS_PROTOTYPE( idTrigger_Hurt );

	//! Initializes a new instance of the idTrigger_Hurt class.
	idTrigger_Hurt();

	//! Saves the hurt trigger state to the specified save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the state of the hurt trigger from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes the hurt trigger by parsing spawn arguments and enabling its functionality.
	void Spawn();

private:
	bool  on;
	float delay;
	int	  nextTime;

	//! Handles the touch event for a hurt trigger, applying damage to entities that touch it.
	void  Event_Touch( idEntity* other, trace_t* trace );

	//! Toggles the enabled state of the hurt trigger.
	void  Event_Toggle( idEntity* activator );
};

/*!
	\class idTrigger_Fade
	\brief A trigger entity that initiates a fade effect for the local player upon activation.
*/
class idTrigger_Fade : public idTrigger
{
public:
	CLASS_PROTOTYPE( idTrigger_Fade );

private:
	//! Triggers a fade effect for the local player when activated.
	void Event_Trigger( idEntity* activator );
};

/*!
	\class idTrigger_Touch
	\brief A trigger entity that activates when entities touch it.

	This class implements a trigger volume that becomes active when entities collide with it. The trigger maintains a collision model to detect touch events and manages its activation state through
   enable and disable methods. The touch events are handled by the TouchEntities method which activates associated targets. The spawn method initializes the trigger's collision model and sets its
   initial state. Think method processes the trigger's logic each frame. The class supports serialization through Save and Restore methods for game state persistence. The trigger can be toggled
   between active and inactive states using the Event_Trigger method.

*/
class idTrigger_Touch : public idTrigger
{
public:
	//! Destructor for idTrigger_Touch that cleans up the clipModel resource.
	~idTrigger_Touch();

	CLASS_PROTOTYPE( idTrigger_Touch );

	//! Initializes the idTrigger_Touch object with a null clip model.
	idTrigger_Touch();

	//! Initializes the trigger touch object by setting up its collision model and activation state.
	void		 Spawn();

	//! Executes the trigger's think logic, handling entity touching and updating the entity's state.
	virtual void Think();

	//! Saves the trigger touch clip model to the specified save file.
	void		 Save( idSaveGame* savefile );

	//! Restores the trigger touch state from a save file.
	void		 Restore( idRestoreGame* savefile );

	//! Enables the trigger to become active and start thinking.
	virtual void Enable();

	//! Disables the trigger by deactivating its thinking functionality.
	virtual void Disable();

	//! Activates targets for entities that touch the trigger.
	void		 TouchEntities();

private:
	idClipModel* clipModel;

	//! Toggles the trigger's active state based on its current think flag status.
	void		 Event_Trigger( idEntity* activator );
};

/*!
	\class idTrigger_Flag
	\brief A trigger entity that manages flag capture and team interactions.
*/
class idTrigger_Flag : public idTrigger_Multi
{
public:
	CLASS_PROTOTYPE( idTrigger_Flag );

	//! Initializes a new instance of the idTrigger_Flag class with default values.
	idTrigger_Flag();

	//! Initializes the trigger flag with team and player settings, and sets up the event flag if specified.
	void Spawn();

private:
	int				  team;
	bool			  player; // flag must be attached/carried by player

	const idEventDef* eventFlag;

	//! Handles touch events for triggering flag capture or interaction with team flags
	void			  Event_Touch( idEntity* other, trace_t* trace );
};

#endif /* !__GAME_TRIGGER_H__ */
