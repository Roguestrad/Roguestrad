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

#ifndef __SCRIPT_THREAD_H__
#define __SCRIPT_THREAD_H__

extern const idEventDef EV_Thread_Execute;
extern const idEventDef EV_Thread_SetCallback;
extern const idEventDef EV_Thread_TerminateThread;
extern const idEventDef EV_Thread_Pause;
extern const idEventDef EV_Thread_Wait;
extern const idEventDef EV_Thread_WaitFrame;
extern const idEventDef EV_Thread_WaitFor;
extern const idEventDef EV_Thread_WaitForThread;
extern const idEventDef EV_Thread_Print;
extern const idEventDef EV_Thread_PrintLn;
extern const idEventDef EV_Thread_Say;
extern const idEventDef EV_Thread_Assert;
extern const idEventDef EV_Thread_Trigger;
extern const idEventDef EV_Thread_SetCvar;
extern const idEventDef EV_Thread_GetCvar;
extern const idEventDef EV_Thread_Random;
extern const idEventDef EV_Thread_GetTime;
extern const idEventDef EV_Thread_KillThread;
extern const idEventDef EV_Thread_SetThreadName;
extern const idEventDef EV_Thread_GetEntity;
extern const idEventDef EV_Thread_Spawn;
extern const idEventDef EV_Thread_SetSpawnArg;
extern const idEventDef EV_Thread_SpawnString;
extern const idEventDef EV_Thread_SpawnFloat;
extern const idEventDef EV_Thread_SpawnVector;
extern const idEventDef EV_Thread_AngToForward;
extern const idEventDef EV_Thread_AngToRight;
extern const idEventDef EV_Thread_AngToUp;
extern const idEventDef EV_Thread_Sine;
extern const idEventDef EV_Thread_Cosine;
extern const idEventDef EV_Thread_Normalize;
extern const idEventDef EV_Thread_VecLength;
extern const idEventDef EV_Thread_VecDotProduct;
extern const idEventDef EV_Thread_VecCrossProduct;
extern const idEventDef EV_Thread_OnSignal;
extern const idEventDef EV_Thread_ClearSignal;
extern const idEventDef EV_Thread_SetCamera;
extern const idEventDef EV_Thread_FirstPerson;
extern const idEventDef EV_Thread_TraceFraction;
extern const idEventDef EV_Thread_TracePos;
extern const idEventDef EV_Thread_FadeIn;
extern const idEventDef EV_Thread_FadeOut;
extern const idEventDef EV_Thread_FadeTo;
extern const idEventDef EV_Thread_Restart;

/*!
	\class idThread
	\brief Manages script execution threads with event handling and synchronization capabilities.

	Provides comprehensive thread management for executing scripted events within the engine, supporting stack operations, event synchronization, and interaction with game entities. Threads can be
   paused, resumed, waited upon, and terminated while maintaining their execution state. The class handles script function calls, argument passing, and return value management through a stack-based
   system. It supports debugging features, serialization for save games, and integration with the engine's physics and rendering systems for operations like tracing, drawing debug visuals, and
   managing entity interactions.

*/
class idThread : public idClass
{
private:
	static idThread*					 currentThread;

	idThread*							 waitingForThread;
	int									 waitingFor;
	int									 waitingUntil;
	idInterpreter						 interpreter;

	idDict								 spawnArgs;

	int									 threadNum;
	idStr								 threadName;

	int									 lastExecuteTime;
	int									 creationTime;

	bool								 manualControl;

	static int							 threadIndex;
	static idList<idThread*, TAG_THREAD> threadList;

	static trace_t						 trace;

	//! Initializes the thread with a unique thread number and adds it to the thread list.
	void								 Init();

	//! Pauses the thread execution by clearing the wait state and marking the interpreter as done processing.
	void								 Pause();

	//! Executes the thread's event.
	void								 Event_Execute();

	//! Sets the name of the current thread to the provided string.
	void								 Event_SetThreadName( const char* name );

public:
	//! Pushes a boolean value onto the thread's stack.
	void PushBool( bool value );

	//! Pushes a string value onto the thread's execution stack.
	void PushString( const char* string );

	//! Pushes the entity number onto the thread's stack.
	void PushEntity( const idEntity* ent );

	//! Pushes the components of a 3D vector onto the thread's stack.
	void PushVec3( const idVec3& value );

	//! Pushes a floating point value onto the thread's execution stack.
	void PushFloat( float value );

	//! Pushes an integer value onto the thread's execution stack.
	void PushInt( int value );

	//! Clears the thread's execution stack by resetting the interpreter.
	void ClearStack();

public:
	//! Terminates a specified thread by its identifier.
	void Event_TerminateThread( int num );

	//! Pauses the thread execution.
	void Event_Pause();

	//! Pauses the execution of the thread for the specified time interval.
	void Event_Wait( float time );

	//! Pauses execution for one frame.
	void Event_WaitFrame();

	//! Pauses the thread and waits for the specified entity to finish a callback event.
	void Event_WaitFor( idEntity* ent );

	//! Pauses the current thread and waits for the specified thread to complete execution.
	void Event_WaitForThread( int num );

	//! Prints the provided text to the game console.
	void Event_Print( const char* text );

	//! Prints the provided text followed by a newline to the game console.
	void Event_PrintLn( const char* text );

	//! Executes a say command with the provided text input.
	void Event_Say( const char* text );

	//! Checks that the given floating-point value is truthy and asserts if it is not.
	void Event_Assert( float value );

	//! Triggers an entity by signaling it, processing activation, and triggering its GUIs.
	void Event_Trigger( idEntity* ent );

	//! Sets a console variable to the specified string value.
	void Event_SetCvar( const char* name, const char* value ) const;

	//! Retrieves the string value of a console variable by its name.
	void Event_GetCvar( const char* name ) const;

	//! Returns a random floating-point number within the specified range.
	void Event_Random( float range ) const;

	//! Returns a random integer between 0 and the specified range minus one.
	void Event_RandomInt( int range ) const;

	//! Returns the current time in seconds based on the real client time.
	void Event_GetTime();

	//! Terminates a thread with the specified name.
	void Event_KillThread( const char* name );

	//! Retrieves an entity by its name or entity number and returns it.
	void Event_GetEntity( const char* name );

	//! Spawns an entity with the specified class name and returns a reference to it.
	void Event_Spawn( const char* classname );

	//! Copies the spawn arguments from the specified entity to the current thread's spawn arguments.
	void Event_CopySpawnArgs( idEntity* ent );

	//! Sets a spawn argument with the specified key and value.
	void Event_SetSpawnArg( const char* key, const char* value );

	//! Returns the string value of a spawn argument key, or the default value if the key is not found.
	void Event_SpawnString( const char* key, const char* defaultvalue );

	//! Retrieves a float value from the spawn arguments using the specified key, defaulting to a provided value if the key is not found.
	void Event_SpawnFloat( const char* key, float defaultvalue );

	//! Returns a vector value from spawn arguments, using a default value if the key is not found.
	void Event_SpawnVector( const char* key, idVec3& defaultvalue );

	//! Clears the persistent level information stored in the game local data.
	void Event_ClearPersistantArgs();

	//! Sets a persistent argument with the specified key and value in the game local persistent level information.
	void Event_SetPersistantArg( const char* key, const char* value );

	//! Returns the persistent string value associated with the given key from the game world's persistent level information.
	void Event_GetPersistantString( const char* key );

	//! Retrieves a floating-point value from the persistent level information using the specified key.
	void Event_GetPersistantFloat( const char* key );

	//! Retrieves a persistent vector value by key and returns it.
	void Event_GetPersistantVector( const char* key );

	//! Converts the input angles to a forward vector and returns it.
	void Event_AngToForward( idAngles& ang );

	//! Converts an angle structure to its right vector component.
	void Event_AngToRight( idAngles& ang );

	//! Converts an angles object to its up vector component.
	void Event_AngToUp( idAngles& ang );

	//! Returns the sine of the specified angle in radians.
	void Event_GetSine( float angle );

	//! Returns the cosine of the specified angle in degrees.
	void Event_GetCosine( float angle );

	//! Returns the arcsine of the given value in degrees.
	void Event_GetArcSine( float a );

	//! Calculates the arc cosine of the given value and returns the result in degrees.
	void Event_GetArcCosine( float a );

	//! Returns the square root of the given floating-point value.
	void Event_GetSquareRoot( float theSquare );

	//! Normalizes the input vector and returns the normalized result.
	void Event_VecNormalize( idVec3& vec );

	//! Returns the length of the provided vector.
	void Event_VecLength( idVec3& vec );

	//! Computes and returns the dot product of two 3D vectors.
	void Event_VecDotProduct( idVec3& vec1, idVec3& vec2 );

	//! Computes the cross product of two 3D vectors and returns the result.
	void Event_VecCrossProduct( idVec3& vec1, idVec3& vec2 );

	//! Converts a vector toAngles and returns the resulting angles as a vector.
	void Event_VecToAngles( idVec3& vec );

	//! Converts a vector to orthogonal basis angles and returns the corresponding Euler angles.
	void Event_VecToOrthoBasisAngles( idVec3& vec );

	//! Rotates a vector by a given angle and returns the result.
	void Event_RotateVector( idVec3& vec, idVec3& ang );

	//! Sets up a signal handler for an entity that triggers a function when the signal is received.
	void Event_OnSignal( int signal, idEntity* ent, const char* func );

	//! Clears a signal thread for the specified entity and signal number.
	void Event_ClearSignalThread( int signal, idEntity* ent );

	//! Sets the camera for the game world to the specified entity if it is a valid camera type.
	void Event_SetCamera( idEntity* ent );

	//! Sets the camera to NULL for the first person view.
	void Event_FirstPerson();

	/*!
		\brief Performs a trace operation between two points or bounds, returning the fraction of the trace distance.

		This function executes a trace from a start point to an end point, or between two bounding boxes defined by mins and maxs. The trace is performed using the game's collision system. If the mins
	   and maxs vectors are both at the origin, a point trace is performed. Otherwise, a bounds trace is performed. The result is the fraction of the trace distance, which indicates how far the trace
	   traveled before hitting something. The function returns this fraction as a floating-point value.

		\param start The starting position of the trace
		\param end The ending position of the trace
		\param mins The minimum extents of the bounding box for the trace, or vec3_origin to perform a point trace
		\param maxs The maximum extents of the bounding box for the trace, or vec3_origin to perform a point trace
		\param contents_mask The contents mask to filter the trace against
		\param passEntity The entity to pass during the trace, or NULL to not pass any entity
		\return The fraction of the trace distance, indicating how far the trace traveled before hitting something.
	*/
	void Event_Trace( const idVec3& start, const idVec3& end, const idVec3& mins, const idVec3& maxs, int contents_mask, idEntity* passEntity );

	/*!
		\brief Performs a point trace between two positions and returns the fraction of the trace distance.

		This function executes a point trace from the start position to the end position using the specified contents mask and pass entity. It utilizes the gameLocal clip system to perform the trace
	   operation and returns the fraction of the distance that was successfully traced. The fraction represents how far along the trace line the collision occurred, with 1.0 meaning no collision and
	   0.0 meaning a collision at the start position.

		\param start The starting position for the trace
		\param end The ending position for the trace
		\param contents_mask The contents mask used for collision detection
		\param passEntity The entity to pass through during the trace
		\return The fraction of the trace distance that was successfully traced, where 1.0 indicates no collision and 0.0 indicates a collision at the start position
	*/
	void Event_TracePoint( const idVec3& start, const idVec3& end, int contents_mask, idEntity* passEntity );

	//! Returns the fraction of a trace event.
	void Event_GetTraceFraction();

	//! Returns the end position of the last trace operation performed by this thread.
	void Event_GetTraceEndPos();

	//! Returns the normal vector of the last trace result, or the origin vector if the trace did not hit anything.
	void Event_GetTraceNormal();

	//! Returns the entity that was hit by the last trace operation.
	void Event_GetTraceEntity();

	//! Returns the name of the joint from a trace result.
	void Event_GetTraceJoint();

	//! Returns the name of the body involved in a trace collision
	void Event_GetTraceBody();

	//! Fades the player's view to the specified color over the given time period.
	void Event_FadeIn( idVec3& color, float time );

	//! Fades the player's view to the specified color over the given time period.
	void Event_FadeOut( idVec3& color, float time );

	//! Sets the fade color and time for the local player's view.
	void Event_FadeTo( idVec3& color, float alpha, float time );

	//! Sets a global shader parameter value at the specified parameter number.
	void Event_SetShaderParm( int parmnum, float value );

	//! Starts playing a music track directly using the specified shader name.
	void Event_StartMusic( const char* name );

	//! Issues a warning message using the provided text.
	void Event_Warning( const char* text );

	//! Causes an error with the specified text.
	void Event_Error( const char* text );

	//! Returns the length of the provided string.
	void Event_StrLen( const char* string );

	//! Returns the leftmost 'num' characters from the input string.
	void Event_StrLeft( const char* string, int num );

	//! Returns the rightmost num characters of the input string.
	void Event_StrRight( const char* string, int num );

	//! Skips a specified number of characters from the beginning of a string and returns the remaining portion.
	void Event_StrSkip( const char* string, int num );

	//! Extracts a substring from the given string starting at the specified index for the specified number of characters.
	void Event_StrMid( const char* string, int start, int num );

	//! Converts a string to a floating-point number and returns the result.
	void Event_StrToFloat( const char* string );

	/*!
		\brief Performs radius damage calculation and applies it to entities within the specified range.

		This function executes a radius damage event by calling the game local radius damage system. It takes in the origin point of the damage, the entity causing the damage, the attacker entity, an
	   entity to ignore during damage application, the name of the damage definition to use, and the power of the damage. The damage is applied to all entities within the radius of the origin point,
	   excluding the ignored entity.

		\param origin The origin point from which the radius damage is calculated
		\param inflictor The entity that is causing the damage
		\param attacker The entity that is the source of the damage
		\param ignore The entity to be ignored during damage application
		\param damageDefName The name of the damage definition to be used for applying damage
		\param dmgPower The power or strength of the damage to be applied
	*/
	void Event_RadiusDamage( const idVec3& origin, idEntity* inflictor, idEntity* attacker, idEntity* ignore, const char* damageDefName, float dmgPower );

	//! Returns whether the game is running as a client.
	void Event_IsClient();

	//! Returns whether the game is currently running in multiplayer mode.
	void Event_IsMultiplayer();

	//! Returns the time elapsed since the previous frame in seconds.
	void Event_GetFrameTime();

	//! Returns the latched engine tics per second value.
	void Event_GetTicsPerSecond();

	//! Caches a sound shader by its name.
	void Event_CacheSoundShader( const char* soundName );

	/*!
		\brief Draws a debug line in the game world with the specified color, start and end positions, and lifetime

		This function creates a visual debug line in the game world using the provided color, start and end positions, and lifetime parameters. The color is converted from idVec3 to idVec4 by setting
	   the alpha component to 0.0. The lifetime is converted from seconds to milliseconds using the SEC2MS macro. The debug line is rendered by the game render world system

		\param color The RGB color values for the debug line
		\param start The starting position of the debug line in world coordinates
		\param end The ending position of the debug line in world coordinates
		\param lifetime The duration in seconds for which the debug line should be visible
	*/
	void Event_DebugLine( const idVec3& color, const idVec3& start, const idVec3& end, const float lifetime );

	/*!
		\brief Displays a debug arrow in the game world using the provided color, start and end positions, size, and lifetime

		This function creates a visual debug arrow in the game world for debugging purposes. It takes color, start and end positions, size, and lifetime parameters to define the appearance and
	   duration of the arrow. The color is converted from idVec3 to idVec4 by setting the w component to 0.0f. The lifetime is converted from seconds to milliseconds using the SEC2MS macro. The arrow
	   is rendered using the gameRenderWorld debug drawing functionality

		\param color Color of the debug arrow defined as RGB values
		\param start Starting position of the arrow in world coordinates
		\param end Ending position of the arrow in world coordinates
		\param size Size of the arrow in pixels
		\param lifetime Duration in seconds for which the arrow will be visible
	*/
	void Event_DebugArrow( const idVec3& color, const idVec3& start, const idVec3& end, const int size, const float lifetime );

	/*!
		\brief Draws a debug circle in the game world with specified color, position, direction, radius, steps, and lifetime

		This function creates a visual debug circle in the game world using the render world interface. It converts the input color to a 4-component vector with alpha set to zero and schedules the
	   circle to be rendered for a specified duration. The circle is defined by its center position, direction, radius, and number of steps to render. The lifetime parameter controls how long the
	   circle will be visible before being automatically removed from the debug display.

		\param color Color of the debug circle as RGB values
		\param origin Center position of the debug circle
		\param dir Direction vector defining the orientation of the circle
		\param radius Radius of the debug circle
		\param numSteps Number of steps used to approximate the circular shape
		\param lifetime Duration in seconds for which the circle will be visible
	*/
	void Event_DebugCircle( const idVec3& color, const idVec3& origin, const idVec3& dir, const float radius, const int numSteps, const float lifetime );

	/*!
		\brief Displays a debug bounding box with the specified color, size, and lifetime in the game world.

		This function creates a debug visualization of a bounding box using the provided color, minimum and maximum coordinates, and lifetime. The bounding box is rendered in the game world for
	   debugging purposes. The color is converted from a 3-component vector to a 4-component vector with the alpha component set to zero. The lifetime is converted from seconds to milliseconds for the
	   rendering system. The bounding box is drawn using the game render world's debug drawing capabilities.

		\param color The color of the bounding box as a 3D vector (RGB components)
		\param mins The minimum coordinates of the bounding box
		\param maxs The maximum coordinates of the bounding box
		\param lifetime The duration in seconds for which the bounding box should be displayed
	*/
	void Event_DebugBounds( const idVec3& color, const idVec3& mins, const idVec3& maxs, const float lifetime );

	/*!
		\brief Displays text in the game world at the specified location with the given properties

		This function renders text in the game world at a specific 3D position with customizable scale, color, and alignment. The text is displayed for a specified duration before disappearing. The
	   function uses the current player's view angles for proper orientation in the world space. The text is rendered with an alpha value of 0 which makes it fully opaque. The lifetime parameter is
	   converted from seconds to milliseconds for internal use. The alignment parameter controls how the text is positioned relative to the origin point.

		\param text The text string to display
		\param origin The 3D position in the world where the text will be drawn
		\param scale The scaling factor for the text size
		\param color The RGB color values for the text
		\param align The alignment of the text relative to the origin point
		\param lifetime How long the text should remain visible in seconds
	*/
	void Event_DrawText( const char* text, const idVec3& origin, float scale, const idVec3& color, const int align, const float lifetime );

	//! Returns whether the local player is currently under influence.
	void Event_InfluenceActive();

public:
	CLASS_PROTOTYPE( idThread );

	//! Constructs a new thread object and initializes it.
	idThread();

	//! Initializes a new thread for executing a script function on the given entity.
	idThread( idEntity* self, const function_t* func );

	//! Constructs a new thread and initializes it with the specified function.
	idThread( const function_t* func );

	//! Initializes a new thread with the specified interpreter, function, and argument count.
	idThread( idInterpreter* source, const function_t* func, int args );

	/*!
		\brief Constructs a new thread for executing a script function with the specified parameters.

		The constructor initializes a new thread that will execute a script function. It sets up the thread name based on the entity name, and calls the interpreter to begin execution of the specified
	   function with the given arguments. If debug scripting is enabled, it prints a message indicating the creation of the thread.

		\param source The interpreter to use for executing the script
		\param self The entity that owns this thread
		\param func The script function to execute
		\param args The number of arguments to pass to the function
		\throws The function asserts that self is not null, and will throw an assertion failure if this condition is not met.
	*/
	idThread( idInterpreter* source, idEntity* self, const function_t* func, int args );

	//! Destroys a thread and cleans up its resources.
	virtual ~idThread();

	//! Prevents the thread manager from automatically deleting this thread when it terminates.
	void					  ManualDelete();

	//! Saves the thread state to a save game file.
	void					  Save( idSaveGame* savefile ) const;

	//! Restores the thread state from a save game file.
	void					  Restore( idRestoreGame* savefile );

	void					  EnableDebugInfo() { interpreter.debug = true; };
	void					  DisableDebugInfo() { interpreter.debug = false; };

	//! Pauses the thread execution for a specified number of milliseconds.
	void					  WaitMS( int time );

	//! Pauses the thread execution for the specified time in seconds.
	void					  WaitSec( float time );

	//! Pauses the thread for one frame.
	void					  WaitFrame();

	//! Calls the specified function with the option to clear the stack.
	void					  CallFunction( const function_t* func, bool clearStack );

	//! Executes a function on the specified entity using the thread's interpreter.
	void					  CallFunction( idEntity* obj, const function_t* func, bool clearStack );

	//! Displays information about the thread including its status and waiting conditions.
	void					  DisplayInfo();

	//! Returns the thread with the specified thread number from the thread list.
	static idThread*		  GetThread( int num );

	//! Displays information about all active threads in the game.
	static void				  ListThreads_f( const idCmdArgs& args );

	//! Resets the thread system by clearing all threads and resetting the thread index
	static void				  Restart();

	//! Completes an object move operation for the specified thread and entity.
	static void				  ObjectMoveDone( int threadnum, idEntity* obj );

	//! Returns a reference to the list of threads managed by this thread object.
	static idList<idThread*>& GetThreads();

	//! Returns true if the thread has finished processing all scheduled tasks.
	bool					  IsDoneProcessing();

	//! Returns true if the thread is in the process of being destroyed.
	bool					  IsDying();

	//! Ends the thread execution by pausing it and marking it as dying
	void					  End();

	//! Kills all threads whose names match the given name pattern, supporting wildcard matching.
	static void				  KillThread( const char* name );

	//! Kills the specified thread by ending it if it exists.
	static void				  KillThread( int num );

	//! Executes the thread's script interpreter and manages its execution state.
	bool					  Execute();
	void					  ManualControl()
	{
		manualControl = true;
		CancelEvents( &EV_Thread_Execute );
	};
	void			 DoneProcessing() { interpreter.doneProcessing = true; };
	void			 ContinueProcessing() { interpreter.doneProcessing = false; };
	bool			 ThreadDying() { return interpreter.threadDying; };
	void			 EndThread() { interpreter.threadDying = true; };

	//! Checks if the thread is currently waiting for an event or timeout.
	bool			 IsWaiting();

	//! Clears the wait-for state of the thread.
	void			 ClearWaitFor();

	//! Checks if the thread is waiting for a specific entity.
	bool			 IsWaitingFor( idEntity* obj );

	//! Handles completion of an object movement for the thread
	void			 ObjectMoveDone( idEntity* obj );

	//! Handles thread callback events for the idThread class.
	void			 ThreadCallback( idThread* thread );

	//! Schedules the thread to execute after a specified delay.
	void			 DelayedStart( int delay );

	//! Starts the thread execution and returns whether it was successful.
	bool			 Start();

	//! Returns the thread that the current thread is waiting for.
	idThread*		 WaitingOnThread();

	//! Sets the thread number for this thread instance.
	void			 SetThreadNum( int num );

	//! Returns the thread number of this thread instance.
	int				 GetThreadNum();

	//! Sets the name of the current thread to the provided string.
	void			 SetThreadName( const char* name );

	//! Returns the name of the thread as a C-style string.
	const char*		 GetThreadName();

	//! Reports an error message to the interpreter
	void			 Error( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Outputs a formatted warning message through the thread's interpreter.
	void			 Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Returns a pointer to the currently executing thread object.
	static idThread* CurrentThread();

	//! Returns the thread number of the currently active thread.
	static int		 CurrentThreadNum();

	//! Begins a multi-frame event on the specified entity.
	static bool		 BeginMultiFrameEvent( idEntity* ent, const idEventDef* event );

	//! Ends a multi-frame event for the specified entity and event.
	static void		 EndMultiFrameEvent( idEntity* ent, const idEventDef* event );

	//! Sets the return value of the thread to the specified string.
	static void		 ReturnString( const char* text );

	//! Sets the return value of the current thread to the specified float value.
	static void		 ReturnFloat( float value );

	//! Sets the return value of the thread to the specified integer.
	static void		 ReturnInt( int value );

	//! Sets the return value of the current thread to the specified vector.
	static void		 ReturnVector( idVec3 const& vec );

	//! Sets the return value of the thread to the specified entity.
	static void		 ReturnEntity( idEntity* ent );
};

ID_INLINE idThread* idThread::WaitingOnThread()
{
	return waitingForThread;
}

ID_INLINE void idThread::SetThreadNum( int num )
{
	threadNum = num;
}

ID_INLINE int idThread::GetThreadNum()
{
	return threadNum;
}

ID_INLINE const char* idThread::GetThreadName()
{
	return threadName.c_str();
}

ID_INLINE idList<idThread*>& idThread::GetThreads()
{
	return threadList;
}

ID_INLINE bool idThread::IsDoneProcessing()
{
	return interpreter.doneProcessing;
}

ID_INLINE bool idThread::IsDying()
{
	return interpreter.threadDying;
}

#endif /* !__SCRIPT_THREAD_H__ */
