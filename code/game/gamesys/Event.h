/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

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
/*
sys_event.h

Event are used for scheduling tasks and for linking script commands.
*/
#ifndef __SYS_EVENT_H__
#define __SYS_EVENT_H__

#define D_EVENT_MAXARGS		8 // if changed, enable the CREATE_EVENT_CODE define in Event.cpp to generate switch statement for idClass::ProcessEventArgPtr.
// running the game will then generate c:\doom\base\events.txt, the contents of which should be copied into the switch statement.

// RB: from dhewm3
// stack size of idVec3, aligned to native pointer size
#define E_EVENT_SIZEOF_VEC	( ( sizeof( idVec3 ) + ( sizeof( intptr_t ) - 1 ) ) & ~( sizeof( intptr_t ) - 1 ) )
// RB end

#define D_EVENT_VOID		( ( char )0 )
#define D_EVENT_INTEGER		'd'
#define D_EVENT_FLOAT		'f'
#define D_EVENT_VECTOR		'v'
#define D_EVENT_STRING		's'
#define D_EVENT_ENTITY		'e'
#define D_EVENT_ENTITY_NULL 'E' // event can handle NULL entity pointers
#define D_EVENT_TRACE		't'

#define MAX_EVENTS			4096

class idClass;
class idTypeInfo;

/*!
	\class idEventDef
	\brief Defines event types and their argument formats for event-driven systems.

	This class represents event definitions used in an event-driven architecture, storing metadata about event commands including their names, argument formats, return types, and associated event
   numbers. It provides methods to query event properties such as argument count, sizes, and offsets. The class supports finding events by name and retrieving event definitions by number. It is
   designed to work with a system that manages event registration and dispatch, where each event has a unique identifier and predefined argument structure. Event definitions are typically created at
   initialization time and remain static during execution.

*/
class idEventDef
{
private:
	const char*		   name;
	const char*		   formatspec;
	unsigned int	   formatspecIndex;
	int				   returnType;
	int				   numargs;
	size_t			   argsize;
	int				   argOffset[D_EVENT_MAXARGS];
	int				   eventnum;
	const idEventDef*  next;

	static idEventDef* eventDefList[MAX_EVENTS];
	static int		   numEventDefs;

public:
	//! Initializes a new event definition with the specified command, argument format, and return type.
	idEventDef( const char* command, const char* formatspec = NULL, char returnType = 0 );

	//! Returns the name of the event definition.
	const char*				 GetName() const;

	//! Returns the argument format string for this event definition.
	const char*				 GetArgFormat() const;

	//! Returns the format specification index associated with this event definition.
	unsigned int			 GetFormatspecIndex() const;

	//! Returns the return type of the event definition.
	char					 GetReturnType() const;

	//! Returns the event number associated with this event definition.
	int						 GetEventNum() const;

	//! Returns the number of arguments associated with this event definition.
	int						 GetNumArgs() const;

	//! Returns the size of the arguments for the event definition.
	size_t					 GetArgSize() const;

	//! Returns the offset for a specific argument index in the event definition.
	int						 GetArgOffset( int arg ) const;

	//! Returns the number of event commands defined in the event definition.
	static int				 NumEventCommands();

	//! Retrieves the event definition command for the specified event number.
	static const idEventDef* GetEventCommand( int eventnum );

	//! Finds and returns the event definition with the specified name, or NULL if not found.
	static const idEventDef* FindEvent( const char* name );
};

class idSaveGame;
class idRestoreGame;

/*!
	\class idEvent
	\brief Manages event scheduling, execution, and persistence within the game engine.

	The idEvent class provides a comprehensive system for handling events in the game engine, including scheduling events to be executed at specific times, managing event arguments, and processing
   events during game updates. It supports both regular and fast event processing, maintains an event queue for pending operations, and provides functionality for saving and restoring event state
   during game loading. The class handles memory management for events through an internal pool and supports cancellation of events associated with specific objects. Event arguments are validated
   during copying to ensure type safety and correct argument counts. The system initializes and shuts down cleanly, and supports saving event data to save games while properly handling trace
   information.

*/
class idEvent
{
private:
	const idEventDef*								 eventdef;
	byte*											 data;
	int												 time;
	idClass*										 object;
	const idTypeInfo*								 typeinfo;

	idLinkList<idEvent>								 eventNode;

	static idDynamicBlockAlloc<byte, 16 * 1024, 256> eventDataAllocator;

public:
	static bool initialized;

	//! Destructor for the idEvent class that releases associated resources.
	~idEvent();

	//! Allocates a new event with the specified definition and arguments.
	static idEvent* Alloc( const idEventDef* evdef, int numargs, va_list args );

	/*!
		\brief Copies event arguments from a variable argument list to a fixed-size data array while validating argument types and counts.

		This function processes the arguments of an event by copying them from a va_list to an array of intptr_t values. It validates that the number of arguments matches the expected count defined by
	   the event definition and ensures each argument matches the expected type. The function is used internally for handling event argument passing in the game engine. It includes special handling
	   for NULL entity arguments which are cast to integer 0 to prevent false type errors.

		\param evdef Pointer to the event definition that defines the expected arguments
		\param numargs Number of arguments to copy
		\param args Variable argument list containing the arguments to copy
		\param data Target array to store the copied argument values
		\throws gameLocal.Error is called when the number of arguments doesn't match or when an argument type doesn't match the expected type
	*/
	static void		CopyArgs( const idEventDef* evdef, int numargs, va_list args, intptr_t data[D_EVENT_MAXARGS] );

	//! Frees the memory and resets the event data.
	void			Free();

	//! Schedules an event to be executed at a specified time relative to the game clock.
	void			Schedule( idClass* object, const idTypeInfo* cls, int time );

	//! Returns the data pointer of the event.
	byte*			GetData();

	//! Cancels all events associated with a given object and optional event definition.
	static void		CancelEvents( const idClass* obj, const idEventDef* evdef = NULL );

	//! Clears the event list by resetting all events in the event pool and clearing the free list and event queue.
	static void		ClearEventList();

	//! Processes all pending events in the event queue up to the current game time
	static void		ServiceEvents();

	//! Processes fast events in the event queue until the specified time limit is reached.
	static void		ServiceFastEvents();

	//! Initializes the event system and prepares it for use.
	static void		Init();

	//! Shuts down the event system and releases associated resources.
	static void		Shutdown();

	//! Saves the event queue to a save game file.
	static void		Save( idSaveGame* savefile );

	//! Restores event data from a save game file.
	static void		Restore( idRestoreGame* savefile );

	//! Saves a trace structure to a save game file
	static void		SaveTrace( idSaveGame* savefile, const trace_t& trace );

	//! Restores a trace from a save file
	static void		RestoreTrace( idRestoreGame* savefile, trace_t& trace );
};

ID_INLINE byte* idEvent::GetData()
{
	return data;
}

ID_INLINE const char* idEventDef::GetName() const
{
	return name;
}

ID_INLINE const char* idEventDef::GetArgFormat() const
{
	return formatspec;
}

ID_INLINE unsigned int idEventDef::GetFormatspecIndex() const
{
	return formatspecIndex;
}

ID_INLINE char idEventDef::GetReturnType() const
{
	return returnType;
}

ID_INLINE int idEventDef::GetNumArgs() const
{
	return numargs;
}

ID_INLINE size_t idEventDef::GetArgSize() const
{
	return argsize;
}

ID_INLINE int idEventDef::GetArgOffset( int arg ) const
{
	assert( ( arg >= 0 ) && ( arg < D_EVENT_MAXARGS ) );
	return argOffset[arg];
}

ID_INLINE int idEventDef::GetEventNum() const
{
	return eventnum;
}

#endif /* !__SYS_EVENT_H__ */
