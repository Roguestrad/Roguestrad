/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
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
/*

Base class for all game objects.  Provides fast run-time type checking and run-time
instancing of objects.

*/

#ifndef __SYS_CLASS_H__
#define __SYS_CLASS_H__

class idClass;
class idTypeInfo;

extern const idEventDef EV_Remove;
extern const idEventDef EV_SafeRemove;

// typedef void ( idClass::*eventCallback_t )();
#include <cstdint>
#include <cstring>
#include <type_traits>

// RB: Store event callbacks opaquely because MSVC/clang-cl member function pointer sizes can differ
// (multiple inheritance/adjustor). Casting to `void (idClass::*)()` is not ABI-safe and fails.
// This affects all idClass-derived types that register EVENT callbacks (the eventCallbacks tables),
// including the base idClass entries and any subclasses that add events.
struct eventCallback_t {
	// 16 bytes are usually enough for MSVC x64 member function pointers including adjustor.
	// If a static_assert fires for you: increase to 24/32.
	std::uintptr_t raw[2];

	//! Constructs an event callback with default raw values set to zero.
	eventCallback_t() :
		raw { 0, 0 }
	{
	}

	//! Creates an eventCallback_t from a member function pointer.
	template<typename T>
	static eventCallback_t From( T fn )
	{
		static_assert( std::is_member_function_pointer<T>::value, "T must be a member function pointer" );
		eventCallback_t out;
		static_assert( sizeof( T ) <= sizeof( out.raw ), "eventCallback_t storage too small" );
		std::memcpy( out.raw, &fn, sizeof( T ) );
		return out;
	}

	//! Converts the stored member function pointer to the specified type and returns it.
	template<typename T>
	T As() const
	{
		static_assert( std::is_member_function_pointer<T>::value, "T must be a member function pointer" );
		T fn;
		static_assert( sizeof( T ) <= sizeof( raw ), "eventCallback_t storage too small" );
		std::memcpy( &fn, raw, sizeof( T ) );
		return fn;
	}

	//! Provides a truthiness test for the event callback.
	explicit operator bool() const { return raw[0] != 0 || raw[1] != 0; }
};

template<class Type>
struct idEventFunc {
	const idEventDef* event;
	eventCallback_t	  function;
};

#define EVENT( event, function ) { &( event ), eventCallback_t::From( &function ) },
#define END_CLASS               \
	{                           \
		NULL, eventCallback_t() \
	}                           \
	}                           \
	;

/*!
	\class idEventArg
	\brief A variant-like class for passing arbitrary data as event arguments.

	This class serves as a flexible container for event argument data, supporting multiple fundamental types including integers, floats, vectors, strings, character pointers, entity pointers, and
   trace structures. It is designed to facilitate communication of varying data types through an event system, where the specific type of data being passed is determined at runtime. The constructors
   provide type-safe initialization from various data sources, enabling seamless integration with different kinds of event data. This implementation emphasizes convenience and type safety in event
   handling scenarios.

*/
class idEventArg
{
public:
	int		 type;
	// RB: 64 bit fix, changed int to intptr_t
	intptr_t value;
	// RB end

	idEventArg()
	{
		type  = D_EVENT_INTEGER;
		value = 0;
	};
	idEventArg( int data )
	{
		type  = D_EVENT_INTEGER;
		value = data;
	};
	idEventArg( float data )
	{
		type  = D_EVENT_FLOAT;
		value = *reinterpret_cast<int*>( &data );
	};
	// RB: 64 bit fixes, changed int to intptr_t
	idEventArg( idVec3& data )
	{
		type  = D_EVENT_VECTOR;
		value = reinterpret_cast<intptr_t>( &data );
	};
	idEventArg( const idStr& data )
	{
		type  = D_EVENT_STRING;
		value = reinterpret_cast<intptr_t>( data.c_str() );
	};
	idEventArg( const char* data )
	{
		type  = D_EVENT_STRING;
		value = reinterpret_cast<intptr_t>( data );
	};
	idEventArg( const class idEntity* data )
	{
		type  = D_EVENT_ENTITY;
		value = reinterpret_cast<intptr_t>( data );
	};
	idEventArg( const struct trace_s* data )
	{
		type  = D_EVENT_TRACE;
		value = reinterpret_cast<intptr_t>( data );
	};
	// RB end
};

/*!
	\class idAllocError
	\brief Exception class for allocation errors.
*/
class idAllocError : public idException
{
public:
	//! Constructs an idAllocError exception with an optional error message.
	idAllocError( const char* text = "" ) :
		idException( text )
	{
	}
};

/***********************************************************************

  idClass

***********************************************************************/

/*
================
CLASS_PROTOTYPE

This macro must be included in the definition of any subclass of idClass.
It prototypes variables used in class instanciation and type checking.
Use this on single inheritance concrete classes only.
================
*/
#define CLASS_PROTOTYPE( nameofclass )                                                         \
public:                                                                                        \
	static idTypeInfo				Type;                                                      \
	static idClass*					CreateInstance();                                          \
	virtual idTypeInfo*				GetType() const;                                           \
	virtual intptr_t				Invoke( const char* functionName, void* param1 ) override; \
	virtual bool					HasNativeFunction( const char* functionName ) override;    \
	static idEventFunc<nameofclass> eventCallbacks[]

/*
================
CLASS_DECLARATION

This macro must be included in the code to properly initialize variables
used in type checking and run-time instanciation.  It also defines the list
of events that the class responds to.  Take special care to ensure that the
proper superclass is indicated or the run-time type information will be
incorrect.  Use this on concrete classes only.
================
*/
// RB: made exceptions optional
#if defined( USE_EXCEPTIONS )
	#define CLASS_DECLARATION( nameofsuperclass, nameofclass )                 \
		idTypeInfo nameofclass::Type( #nameofclass,                            \
			#nameofsuperclass,                                                 \
			( idEventFunc<idClass>* )nameofclass::eventCallbacks,              \
			nameofclass::CreateInstance,                                       \
			( void( idClass::* )() ) & nameofclass::Spawn,                     \
			( void( idClass::* )( idSaveGame* ) const ) & nameofclass::Save,   \
			( void( idClass::* )( idRestoreGame* ) ) & nameofclass::Restore ); \
		idClass*   nameofclass::CreateInstance()                               \
		{                                                                      \
			try {                                                              \
				nameofclass* ptr = new nameofclass;                            \
				ptr->FindUninitializedMemory();                                \
				return ptr;                                                    \
			} catch( idAllocError& ) {                                         \
				return NULL;                                                   \
			}                                                                  \
		}                                                                      \
		idTypeInfo* nameofclass::GetType() const                               \
		{                                                                      \
			return &( nameofclass::Type );                                     \
		}                                                                      \
		idEventFunc<nameofclass> nameofclass::eventCallbacks[] = {
#else
	#define CLASS_DECLARATION( nameofsuperclass, nameofclass )                 \
		idTypeInfo nameofclass::Type( #nameofclass,                            \
			#nameofsuperclass,                                                 \
			( idEventFunc<idClass>* )nameofclass::eventCallbacks,              \
			nameofclass::CreateInstance,                                       \
			( void( idClass::* )() ) & nameofclass::Spawn,                     \
			( void( idClass::* )( idSaveGame* ) const ) & nameofclass::Save,   \
			( void( idClass::* )( idRestoreGame* ) ) & nameofclass::Restore ); \
		idClass*   nameofclass::CreateInstance()                               \
		{                                                                      \
			nameofclass* ptr = new nameofclass;                                \
			ptr->FindUninitializedMemory();                                    \
			return ptr;                                                        \
		}                                                                      \
		idTypeInfo* nameofclass::GetType() const                               \
		{                                                                      \
			return &( nameofclass::Type );                                     \
		}                                                                      \
		idEventFunc<nameofclass> nameofclass::eventCallbacks[] = {
#endif
// RB end

/*
================
ABSTRACT_PROTOTYPE

This macro must be included in the definition of any abstract subclass of idClass.
It prototypes variables used in class instanciation and type checking.
Use this on single inheritance abstract classes only.
================
*/
#define ABSTRACT_PROTOTYPE( nameofclass )             \
public:                                               \
	static idTypeInfo				Type;             \
	static idClass*					CreateInstance(); \
	virtual idTypeInfo*				GetType() const;  \
	static idEventFunc<nameofclass> eventCallbacks[]

/*
================
ABSTRACT_DECLARATION

This macro must be included in the code to properly initialize variables
used in type checking.  It also defines the list of events that the class
responds to.  Take special care to ensure that the proper superclass is
indicated or the run-time tyep information will be incorrect.  Use this
on abstract classes only.
================
*/
#define ABSTRACT_DECLARATION( nameofsuperclass, nameofclass )                     \
	idTypeInfo nameofclass::Type( #nameofclass,                                   \
		#nameofsuperclass,                                                        \
		( idEventFunc<idClass>* )nameofclass::eventCallbacks,                     \
		nameofclass::CreateInstance,                                              \
		( void( idClass::* )() ) & nameofclass::Spawn,                            \
		( void( idClass::* )( idSaveGame* ) const ) & nameofclass::Save,          \
		( void( idClass::* )( idRestoreGame* ) ) & nameofclass::Restore );        \
	idClass*   nameofclass::CreateInstance()                                      \
	{                                                                             \
		gameLocal.Error( "Cannot instanciate abstract class %s.", #nameofclass ); \
		return NULL;                                                              \
	}                                                                             \
	idTypeInfo* nameofclass::GetType() const                                      \
	{                                                                             \
		return &( nameofclass::Type );                                            \
	}                                                                             \
	idEventFunc<nameofclass> nameofclass::eventCallbacks[] = {
typedef void ( idClass::*classSpawnFunc_t )();

class idSaveGame;
class idRestoreGame;

/*!
	\class idClass
	\brief Base class for all engine objects with event handling, type information, and serialization capabilities.

	Provides core object functionality including type identification, event system integration, memory management, and serialization. The class serves as the foundation for all engine objects,
   supporting dynamic type information, event-driven programming, and proper object lifecycle management. It handles class registration, instance creation, and maintains type hierarchies through
   idTypeInfo structures. The event system allows for delayed execution, argument passing, and event cancellation. Serialization methods enable saving and restoring object state. Utility methods
   support class hierarchy traversal, casting, and memory debugging. The class is designed to work within a larger engine framework where objects need to respond to events, maintain type safety, and
   support runtime class discovery.

*/
class idClass
{
public:
	//! Returns the type information for the class

	//! Creates and returns a new instance of the class.
	ABSTRACT_PROTOTYPE( idClass );

	//! Overrides the global new operator for idClass to provide custom memory allocation tracking.
	void* operator new( size_t );

	//! Deallocates memory previously allocated by the corresponding new operator.
	void operator delete( void* );

	//! Destructor for object that cancels any events depending on this object.
	virtual ~idClass();

	//! Initializes a class instance for use.
	void		 Spawn();

	//! Calls the spawn function for the class type.
	void		 CallSpawn();

	//! Checks if the object's class is a subclass of the class defined by the passed idTypeInfo.
	bool		 IsType( const idTypeInfo& c ) const;

	//! Returns the text classname of the object.
	const char*	 GetClassname() const;

	//! Returns the text classname of the superclass.
	const char*	 GetSuperclass() const;

	//! Checks for uninitialized memory in the class instance.
	void		 FindUninitializedMemory();

	//! Performs shared thinking logic for the class.
	virtual void SharedThink() { }

	//! Attempts to cast this object to the specified type T, returning a pointer to T if successful or NULL otherwise.
	template<typename T>
	T* Cast()
	{
		return IsType( T::Type ) ? static_cast<T*>( this ) : NULL;
	}

	//! Returns a pointer to the object cast to type T if it is of that type, otherwise returns NULL.
	template<typename T>
	const T* Cast() const
	{
		return IsType( T::Type ) ? static_cast<const T*>( this ) : NULL;
	}

	virtual void	 StateThreadChanged() {};

	//! Returns a null pointer to an idClass instance.
	virtual idClass* InvokeChild() { return NULL; }
	virtual intptr_t Invoke( const char* functionName, void* param1 );
	virtual bool	 HasNativeFunction( const char* functionName );

	void			 Save( idSaveGame* savefile ) const {};
	void			 Restore( idRestoreGame* savefile ) {};

	//! Checks if the class responds to a given event.
	bool			 RespondsTo( const idEventDef& ev ) const;

	//! Posts an event to be executed after a specified time delay in milliseconds.
	bool			 PostEventMS( const idEventDef* ev, int time );

	//! Posts a timed event with a single argument to the class's event queue.
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1 );

	/*!
		\brief Posts a timed event with two arguments to the class event queue

		This function schedules an event to be posted to the class's event queue after a specified delay. The event will be executed at the given time offset from the current time. The function takes
	   two arguments which are passed to the event handler. It is a convenience wrapper around PostEventArgs that handles the argument counting and packaging.

		\param ev Pointer to the event definition to post
		\param time Time offset in milliseconds before the event should be executed
		\param arg1 First argument to pass to the event handler
		\param arg2 Second argument to pass to the event handler
		\return True if the event was successfully posted to the queue, false otherwise
	*/
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2 );

	/*!
		\brief Posts a delayed event with up to three arguments to be executed after a specified time interval.

		This function schedules an event to be executed at a future time, specified in milliseconds. The event will be posted to the class instance and will include up to three arguments. The function
	   delegates the actual posting to PostEventArgs, which handles the argument management and event queuing.

		\param ev The event definition to be posted
		\param time The delay in milliseconds before the event is executed
		\param arg1 First argument for the event
		\param arg2 Second argument for the event
		\param arg3 Third argument for the event
		\return True if the event was successfully posted, false otherwise.
	*/
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2, idEventArg arg3 );

	/*!
		\brief Posts a delayed event with up to four arguments to this class instance

		This function schedules an event to be executed after a specified time delay. The event is posted to the class instance that calls this method. The event execution will be delayed by the
	   specified time in milliseconds. The function supports up to four event arguments which are passed by reference.

		\param ev Pointer to the event definition to post
		\param time Delay time in milliseconds before the event is executed
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\return True if the event was successfully posted, false otherwise
	*/
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4 );

	/*!
		\brief Posts a delayed event with up to five arguments to be executed after a specified time interval.

		This function schedules an event to be posted to the object after a delay specified in milliseconds. The event is defined by the idEventDef pointer and includes up to five arguments. The
	   function returns true if the event was successfully posted, or false if it failed.

		\param ev Pointer to the event definition to be posted
		\param time Delay in milliseconds before the event is executed
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\return True if the event was successfully posted, false otherwise
	*/
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5 );
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6 );

	/*!
		\brief Posts a timed event with up to seven arguments to the object's event queue

		This function schedules an event to be executed at a specific time in the future. It takes an event definition, a time offset in milliseconds, and up to seven event arguments. The event will
	   be processed by the object's event system at the specified time. The function delegates the actual posting operation to PostEventArgs which handles the argument management.

		\param ev Pointer to the event definition to post
		\param time Time offset in milliseconds from current time when the event should be executed
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\param arg6 Sixth event argument
		\param arg7 Seventh event argument
		\return True if the event was successfully posted to the queue, false otherwise
	*/
	bool			 PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6, idEventArg arg7 );

	/*!
		\brief Posts a delayed event with up to 8 arguments to be executed after a specified time interval.

		This function schedules an event to be posted to the object after a specified delay in milliseconds. The event is identified by the event definition ev and will be executed with the provided
	   arguments. The time parameter specifies the delay in milliseconds before the event is posted.

		\param ev Pointer to the event definition to be posted
		\param time Delay in milliseconds before the event is posted
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\param arg6 Sixth event argument
		\param arg7 Seventh event argument
		\param arg8 Eighth event argument
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventMS( const idEventDef* ev, int time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6, idEventArg arg7, idEventArg arg8 );

	//! Posts an event to be executed after a specified time interval in seconds.
	bool PostEventSec( const idEventDef* ev, float time );

	//! Posts an event to be executed after a specified time delay in seconds.
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1 );

	/*!
		\brief Posts a delayed event with two arguments, converting the time from seconds to milliseconds

		This function schedules an event to be posted to the class instance after a specified delay. The delay time is provided in seconds and is internally converted to milliseconds before being
	   passed to the underlying event posting mechanism. The event may have up to two arguments associated with it.

		\param ev Pointer to the event definition to be posted
		\param time Delay time in seconds before the event is posted
		\param arg1 First argument for the event
		\param arg2 Second argument for the event
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2 );

	/*!
		\brief Posts a delayed event with three arguments, converting the time from seconds to milliseconds.

		This function schedules an event to be executed after a specified delay in seconds. The event is posted with three arguments and the time is internally converted from seconds to milliseconds
	   using the SEC2MS macro. The function returns immediately after scheduling the event.

		\param ev Pointer to the event definition to be posted
		\param time Delay in seconds before the event is executed
		\param arg1 First argument for the event
		\param arg2 Second argument for the event
		\param arg3 Third argument for the event
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2, idEventArg arg3 );

	/*!
		\brief Posts an event to be executed after a specified time delay given in seconds

		This function schedules an event to be posted to the object after a delay specified in seconds. The event will be executed at the specified time relative to the current time. The function
	   converts the time delay from seconds to milliseconds using the SEC2MS macro before calling the underlying PostEventArgs function. The event takes up to four arguments which are passed to the
	   event handler when it executes.

		\param ev The event definition to post
		\param time The delay in seconds before the event is executed
		\param arg1 First argument for the event
		\param arg2 Second argument for the event
		\param arg3 Third argument for the event
		\param arg4 Fourth argument for the event
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4 );

	/*!
		\brief Posts a timed event with up to five arguments, converting the time from seconds to milliseconds.

		This function schedules an event to be executed after a specified time delay. The time is provided in seconds and is converted to milliseconds internally. The event can carry up to five
	   arguments, which are passed to the event handler. The function returns true if the event was successfully posted, and false otherwise.

		\param ev Pointer to the event definition to be posted
		\param time Time delay in seconds before the event is executed
		\param arg1 First argument to be passed to the event handler
		\param arg2 Second argument to be passed to the event handler
		\param arg3 Third argument to be passed to the event handler
		\param arg4 Fourth argument to be passed to the event handler
		\param arg5 Fifth argument to be passed to the event handler
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5 );

	/*!
		\brief Posts a timed event with up to six arguments, converting the time from seconds to milliseconds

		This function schedules an event to be executed after a specified time delay. The time parameter is provided in seconds and is internally converted to milliseconds. The function accepts up to
	   six event arguments and uses the PostEventArgs internal method to handle the actual event posting. The event is scheduled to be executed by the game's event system at the specified time in the
	   future.

		\param ev Pointer to the event definition that specifies which event to post
		\param time Time delay in seconds before the event is executed
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\param arg6 Sixth event argument
		\return True if the event was successfully posted to the event queue, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6 );

	/*!
		\brief Posts a delayed event to this object with arguments, where the delay is specified in seconds.

		This function schedules an event to be executed after a specified time delay, measured in seconds. The event will be posted to the object that calls this function. The delay is converted to
	   milliseconds internally before posting the event. Up to seven arguments can be passed along with the event.

		\param ev The event definition to post
		\param time The delay before the event is executed, specified in seconds
		\param arg1 First argument for the event
		\param arg2 Second argument for the event
		\param arg3 Third argument for the event
		\param arg4 Fourth argument for the event
		\param arg5 Fifth argument for the event
		\param arg6 Sixth argument for the event
		\param arg7 Seventh argument for the event
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6, idEventArg arg7 );

	/*!
		\brief Posts a delayed event with the specified time in seconds and up to eight arguments

		The function schedules an event to be posted to this object after a delay specified in seconds. The delay is converted to milliseconds internally. The event can carry up to eight arguments of
	   type idEventArg. The function returns true if the event was successfully posted, or false if posting failed.

		\param ev Pointer to the event definition to post
		\param time Delay in seconds before the event is posted
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\param arg6 Sixth event argument
		\param arg7 Seventh event argument
		\param arg8 Eighth event argument
		\return True if the event was successfully posted, false otherwise
	*/
	bool PostEventSec( const idEventDef* ev, float time, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6, idEventArg arg7, idEventArg arg8 );

	//! Processes a given event and returns true if the event was handled.
	bool ProcessEvent( const idEventDef* ev );

	//! Processes a single event with one argument and returns true if the event was handled.
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1 );

	//! Processes a given event with two arguments and returns true if the event was handled.
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2 );

	/*!
		\brief Processes a system event with up to three arguments and returns true if the event was handled.

		This function serves as a central event processing method for idClass objects, handling system events by delegating them to the internal ProcessEventArgs method. It accepts a pointer to an
	   event definition and up to three event arguments, then forwards these parameters to the argument processing function. The function is commonly used in the game's input and event handling
	   systems to process keyboard, mouse, and other system events.

		\param ev Pointer to the event definition being processed
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\return True if the event was successfully processed and handled by the object, false otherwise
	*/
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2, idEventArg arg3 );

	/*!
		\brief Processes a system event with up to four arguments and returns true if the event was handled.

		This function serves as a central event processing method for idClass objects, handling system events by delegating to ProcessEventArgs with a fixed argument count of four. It is commonly used
	   in the engine's input and event handling systems to process various types of system events such as key presses, mouse movements, and other user interactions. The function is typically called
	   from higher-level event processing functions in the framework and is essential for proper event routing within the engine's architecture.

		\param ev The event definition to process
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\return True if the event was successfully handled by the class, false otherwise
	*/
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4 );

	/*!
		\brief Processes a system event with up to five arguments and returns true if the event was handled.

		This function serves as a wrapper for processing system events within the engine. It takes a pointer to an event definition and up to five event arguments, then forwards them to the internal
	   ProcessEventArgs function. The function is typically used in event handling chains where various components need to process input events, such as keyboard, mouse, or console events. The return
	   value indicates whether the event was successfully processed by this class or any of its subclasses.

		\param ev Pointer to the event definition describing the type of event to process
		\param arg1 First event argument, typically containing event-specific data
		\param arg2 Second event argument, typically containing event-specific data
		\param arg3 Third event argument, typically containing event-specific data
		\param arg4 Fourth event argument, typically containing event-specific data
		\param arg5 Fifth event argument, typically containing event-specific data
		\return True if the event was handled by this class or its subclasses, false otherwise
	*/
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5 );

	/*!
		\brief Processes a system event by dispatching it to the appropriate event handler

		This function serves as the primary event processing entry point for idClass objects, handling system events such as input and console interactions. It forwards the event to the internal event
	   processing system with all six arguments, making it suitable for handling complex event scenarios in the game framework. The function is typically called from central event handling loops where
	   system events need to be dispatched to various game components and UI elements.

		\param ev The event definition specifying the type of event to process
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\param arg6 Sixth event argument
		\return True if the event was handled successfully, false otherwise
	*/
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6 );

	/*!
		\brief Processes a system event by dispatching it to the appropriate event handler

		This function serves as the primary event processing mechanism for the idClass hierarchy, taking a system event and its associated arguments and forwarding them to the event handling system.
	   It is designed to handle up to seven event arguments and returns true if the event was successfully processed, false otherwise. The function delegates the actual event processing work to
	   ProcessEventArgs, which handles the more complex logic of matching events with their handlers and executing the appropriate callbacks.

		\param ev Pointer to the event definition describing the type of event being processed
		\param arg1 First event argument
		\param arg2 Second event argument
		\param arg3 Third event argument
		\param arg4 Fourth event argument
		\param arg5 Fifth event argument
		\param arg6 Sixth event argument
		\param arg7 Seventh event argument
		\return True if the event was successfully processed by an event handler, false otherwise
	*/
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6, idEventArg arg7 );

	/*!
		\brief Processes a system event by dispatching it to the appropriate event handler.

		This function handles the processing of a system event by forwarding it to the event argument processing mechanism. It takes a pointer to an event definition and up to eight event arguments,
	   then delegates the actual processing to ProcessEventArgs with a count of 8 arguments. The function is typically used in the game loop to handle user input, system events, and other asynchronous
	   events. It returns true if the event was handled, false otherwise.

		\param ev Pointer to the event definition that describes the type of event being processed
		\param arg1 First event argument, typically containing data related to the event
		\param arg2 Second event argument, typically containing data related to the event
		\param arg3 Third event argument, typically containing data related to the event
		\param arg4 Fourth event argument, typically containing data related to the event
		\param arg5 Fifth event argument, typically containing data related to the event
		\param arg6 Sixth event argument, typically containing data related to the event
		\param arg7 Seventh event argument, typically containing data related to the event
		\param arg8 Eighth event argument, typically containing data related to the event
		\return True if the event was successfully handled by an event handler, false otherwise
	*/
	bool ProcessEvent( const idEventDef* ev, idEventArg arg1, idEventArg arg2, idEventArg arg3, idEventArg arg4, idEventArg arg5, idEventArg arg6, idEventArg arg7, idEventArg arg8 );

	//! Processes an event with the specified event definition and argument pointer, returning true if the event was handled.
	bool ProcessEventArgPtr( const idEventDef* ev, intptr_t* data );

	//! Cancels events of the specified type associated with this object.
	void CancelEvents( const idEventDef* ev );

	//! Removes the object by deleting it.
	void Event_Remove();

	//! Initializes the class hierarchy and event callback tables for all registered classes.
	static void		   Init();

	//! Shuts down the class by cleaning up type information and resetting the initialized flag.
	static void		   Shutdown();

	//! Returns the idTypeInfo for the specified class name
	static idTypeInfo* GetClass( const char* name );

	//! Prints the class memory usage status including total bytes allocated and number of objects.
	static void		   DisplayInfo_f( const idCmdArgs& args );

	//! Lists all registered classes in the engine with their hierarchy and type information.
	static void		   ListClasses_f( const idCmdArgs& args );

	//! Exports script event definitions to multiple language files including DoomScript, DoomSharp, and Mermaid formats.
	static void		   ExportScriptEvents_f( const idCmdArgs& args );

	//! Creates an instance of a class by its registered name
	static idClass*	   CreateInstance( const char* name );

	//! Returns the number of class types registered in the system.
	static int		   GetNumTypes() { return types.Num(); }

	//! Returns the number of bits used to represent the type number for the class.
	static int		   GetTypeNumBits();

	//! Retrieves the type information for a class given its type number
	static idTypeInfo* GetType( int num );

	//! Schedules the object for removal at a safe time.
	void			   Event_SafeRemove();

private:
	//! Calls the spawn function for the specified class type, handling inheritance by traversing the class hierarchy until a non-inherited spawn function is found.
	classSpawnFunc_t						CallSpawnFunc( idTypeInfo* cls );

	/*!
		\brief Posts an event with arguments to be processed at a specified time

		This function handles posting an event with a variable number of arguments to the event system. It first validates the event definition and checks if the event system is initialized. It then
	   verifies that the class responds to the event by checking the event map. The function also handles special cases for replicated events, particularly for entities with skipReplication set. It
	   prevents event processing on the client side during normal gameplay unless the event is related to map loading or is for a thread. The function allocates the event with the provided arguments
	   and schedules it for execution at the specified time

		\param ev Event definition for the event to be posted
		\param time Time at which the event should be scheduled
		\param numargs Number of arguments following this parameter
		\param  Variable argument list following numargs
		\return True if the event was successfully posted, false if the event system is not initialized or if the event is ignored due to replication settings
	*/
	bool									PostEventArgs( const idEventDef* ev, int time, int numargs, ... );

	//! Processes event arguments for a class event.
	bool									ProcessEventArgs( const idEventDef* ev, int numargs, ... );

	static bool								initialized;
	static idList<idTypeInfo*, TAG_IDCLASS> types;
	static idList<idTypeInfo*, TAG_IDCLASS> typenums;
	static int								typeNumBits;
	static int								memused;
	static int								numobjects;
};

/***********************************************************************

  idTypeInfo

***********************************************************************/

class idTypeInfo
{
public:
	const char* classname;
	const char* superclass;
	idClass* ( *CreateInstance )();
	void ( idClass::*Spawn )();
	void ( idClass::*Save )( idSaveGame* savefile ) const;
	void ( idClass::*Restore )( idRestoreGame* savefile );

	idEventFunc<idClass>*	eventCallbacks;
	eventCallback_t*		eventMap;
	idTypeInfo*				super;
	idTypeInfo*				next;
	bool					freeEventMap;
	int						typeNum;
	int						lastChild;

	idHierarchy<idTypeInfo> node;

	/*!
		\brief Initializes a class type information entry and links it into the class hierarchy, handling cases where subclasses are initialized before their superclasses.

		This constructor sets up the type information for a class, including its name, superclass, event callbacks, and member function pointers for spawn, save, and restore operations. It properly
	   links the class into the global type list while maintaining alphabetical order. The constructor is designed to handle initialization order issues where subclasses may be initialized before
	   their superclasses, which can happen with static variable initialization. This is typically called only from the CLASS_DECLARATION macro during program startup.

		\param classname Name of the class being registered
		\param superclass Name of the superclass in the inheritance hierarchy
		\param eventCallbacks Pointer to the event function table for this class
		\param CreateInstance Function pointer to create an instance of the class
		\param Spawn Pointer to the spawn member function
		\param Save Pointer to the save member function
		\param Restore Pointer to the restore member function
		\throws assertion failure if a class with the same name is already registered
	*/
	idTypeInfo( const char*	  classname,
		const char*			  superclass,
		idEventFunc<idClass>* eventCallbacks,
		idClass* ( *CreateInstance )(),
		void ( idClass::*Spawn )(),
		void ( idClass::*Save )( idSaveGame* savefile ) const,
		void ( idClass::*Restore )( idRestoreGame* savefile ) );

	//! Destructor for idTypeInfo that shuts down the type information system.
	~idTypeInfo();

	//! Initializes the event callback table for a class to enable fast event function lookups.
	void Init();

	//! Frees memory used for event lookups and resets type information
	void Shutdown();

	//! Checks if the object's class is a subclass of the class defined by the passed in idTypeInfo.
	bool IsType( const idTypeInfo& superclass ) const;

	//! Checks if this type responds to the specified event.
	bool RespondsTo( const idEventDef& ev ) const;
};

ID_INLINE bool idTypeInfo::IsType( const idTypeInfo& type ) const
{
	return ( ( typeNum >= type.typeNum ) && ( typeNum <= type.lastChild ) );
}

ID_INLINE bool idTypeInfo::RespondsTo( const idEventDef& ev ) const
{
	assert( idEvent::initialized );
	if( !eventMap[ev.GetEventNum()] ) {
		// we don't respond to this event
		return false;
	}

	return true;
}

ID_INLINE bool idClass::IsType( const idTypeInfo& superclass ) const
{
	idTypeInfo* subclass;

	subclass = GetType();
	return subclass->IsType( superclass );
}

ID_INLINE bool idClass::RespondsTo( const idEventDef& ev ) const
{
	const idTypeInfo* c;

	assert( idEvent::initialized );
	c = GetType();
	return c->RespondsTo( ev );
}

#endif /* !__SYS_CLASS_H__ */
