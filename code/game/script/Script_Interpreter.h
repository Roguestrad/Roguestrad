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

#ifndef __SCRIPT_INTERPRETER_H__
#define __SCRIPT_INTERPRETER_H__

#define MAX_STACK_DEPTH 64

// RB: doubled local stack size
#define LOCALSTACK_SIZE ( 6144 * 2 )
// RB end

typedef struct prstack_s {
	int				  s;
	const function_t* f;
	int				  stackbase;
} prstack_t;

/*!
	\class idInterpreter
	\brief An interpreter for executing scripted functions and managing script execution state.

	The idInterpreter class provides functionality for executing scripted functions within a game environment, managing the call stack, local variables, and script execution state. It handles the
   interpretation of script commands, manages the interaction with game entities through script objects, and supports both regular and system events. The interpreter maintains its own stack for
   managing local variables and function parameters, and provides mechanisms for entering and leaving functions, calling events, and managing multi-frame events. It also includes debugging and
   diagnostics capabilities such as stack tracing, error handling, and display of execution information. The class supports serialization for save/load operations and maintains association with a
   specific thread for execution context.

*/
class idInterpreter
{
private:
	prstack_t		  callStack[MAX_STACK_DEPTH];
	int				  callStackDepth;
	int				  maxStackDepth;

	byte			  localstack[LOCALSTACK_SIZE];
	int				  localstackUsed;
	int				  localstackBase;
	int				  maxLocalstackUsed;

	const function_t* currentFunction;
	int				  instructionPointer;

	int				  popParms;
	const idEventDef* multiFrameEvent;
	idEntity*		  eventEntity;

	idThread*		  thread;

	//! Pops a specified number of parameters from the local stack.
	void			  PopParms( int numParms );

	// RB begin
	// RB: 64 bit fix, changed int to intptr_t
public:
	//! Pushes a string value onto the local stack of the interpreter.
	void PushString( const char* string );

	//! Pushes a value onto the local stack of the interpreter
	void Push( intptr_t value );

private:
	//! Pushes a vector value onto the local stack for the interpreter.
	void			PushVector( const idVec3& vector );

	//! Converts a float value to its string representation.
	const char*		FloatToString( float value );

	//! Appends a string to a variable definition's string value, handling both stack and heap allocated strings.
	void			AppendString( idVarDef* def, const char* from );

	//! Sets the string value of a variable definition from a source string.
	void			SetString( idVarDef* def, const char* from );

	//! Returns the string value of a variable definition from the interpreter's stack or direct pointer.
	const char*		GetString( idVarDef* def );

	//! Returns the value of a variable defined by the given variable definition.
	varEval_t		GetVariable( idVarDef* def );

	//! Returns the entity at the specified index in the game entities array
	idEntity*		GetEntity( int entnum ) const;

	//! Returns the script object associated with the specified entity number
	idScriptObject* GetScriptObject( int entnum ) const;

	//! Sets the instruction pointer to the specified position minus one to compensate for the automatic increment during instruction execution.
	void			NextInstruction( int position );

	//! Exits the current function and returns to the calling function
	void			LeaveFunction( idVarDef* returnDef );

	//! Executes a scripted event by invoking the corresponding entity method with specified arguments.
	void			CallEvent( const function_t* func, int argsize );

	//! Executes a system event by processing its arguments and invoking the corresponding event handler.
	void			CallSysEvent( const function_t* func, int argsize );

public:
	bool doneProcessing;
	bool threadDying;
	bool terminateOnExit;
	bool debug;

	//! Initializes a new instance of the idInterpreter class.
	idInterpreter();

	//! Serializes the interpreter state to a save file
	void			  Save( idSaveGame* savefile ) const;

	//! Restores the interpreter state from a save game file.
	void			  Restore( idRestoreGame* savefile );

	//! Sets the thread associated with the interpreter.
	void			  SetThread( idThread* pThread );

	//! Prints the current call stack trace to the game log.
	void			  StackTrace() const;

	//! Returns the current line number of the interpreter's execution point.
	int				  CurrentLine() const;

	//! Returns the filename of the current script file being executed.
	const char*		  CurrentFile() const;

	//! Aborts the currently executing function with an error message.
	void			  Error( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Prints formatted warning messages with optional file and line number information.
	void			  Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) const;

	//! Displays interpreter stack and call depth information.
	void			  DisplayInfo() const;

	//! Begins a multi-frame event for the specified entity and event.
	bool			  BeginMultiFrameEvent( idEntity* ent, const idEventDef* event );

	//! Ends a multi-frame event that was previously started on the given entity.
	void			  EndMultiFrameEvent( idEntity* ent, const idEventDef* event );

	//! Returns true if a multi-frame event is currently in progress.
	bool			  MultiFrameEventInProgress() const;

	//! Copies arguments from the calling thread's stack and executes a function in the current thread.
	void			  ThreadCall( idInterpreter* source, const function_t* func, int args );

	//! Enters a function for execution, managing the call stack and local variables.
	void			  EnterFunction( const function_t* func, bool clearStack );

	//! Executes a function on the specified entity using the interpreter.
	void			  EnterObjectFunction( idEntity* self, const function_t* func, bool clearStack );

	//! Executes the script function that is currently loaded in the interpreter.
	bool			  Execute();

	//! Resets the interpreter state to its initial conditions
	void			  Reset();

	//! Retrieves the value of a script register by name and stores it in the provided string output
	bool			  GetRegisterValue( const char* name, idStr& out, int scopeDepth );

	//! Returns the current depth of the interpreter's call stack.
	int				  GetCallstackDepth() const;

	//! Returns a pointer to the first element of the interpreter's call stack.
	const prstack_t*  GetCallstack() const;

	//! Returns a pointer to the current function being executed by the interpreter.
	const function_t* GetCurrentFunction() const;

	//! Returns the thread associated with this interpreter.
	idThread*		  GetThread() const;
};

ID_INLINE void idInterpreter::PopParms( int numParms )
{
	// pop our parms off the stack
	if( localstackUsed < numParms ) { Error( "locals stack underflow\n" ); }

	localstackUsed -= numParms;
}

ID_INLINE void idInterpreter::Push( intptr_t value )
{
	if( localstackUsed + sizeof( intptr_t ) > LOCALSTACK_SIZE ) { Error( "Push: locals stack overflow\n" ); }
	*( intptr_t* )&localstack[localstackUsed] = value;
	localstackUsed += sizeof( intptr_t );
}

ID_INLINE void idInterpreter::PushVector( const idVec3& vector )
{
	if( localstackUsed + E_EVENT_SIZEOF_VEC > LOCALSTACK_SIZE ) { Error( "Push: locals stack overflow\n" ); }
	*( idVec3* )&localstack[localstackUsed] = vector;
	localstackUsed += E_EVENT_SIZEOF_VEC;
}

ID_INLINE void idInterpreter::PushString( const char* string )
{
	if( localstackUsed + MAX_STRING_LEN > LOCALSTACK_SIZE ) { Error( "PushString: locals stack overflow\n" ); }
	idStr::Copynz( ( char* )&localstack[localstackUsed], string, MAX_STRING_LEN );
	localstackUsed += MAX_STRING_LEN;
}

ID_INLINE const char* idInterpreter::FloatToString( float value )
{
	static char text[32];

	if( value == ( float )( int )value ) {
		idStr::snPrintf( text, sizeof( text ), "%d", ( int )value );
	} else {
		idStr::snPrintf( text, sizeof( text ), "%f", value );
	}
	return text;
}

ID_INLINE void idInterpreter::AppendString( idVarDef* def, const char* from )
{
	if( def->initialized == idVarDef::stackVariable ) {
		idStr::Append( ( char* )&localstack[localstackBase + def->value.stackOffset], MAX_STRING_LEN, from );
	} else {
		idStr::Append( def->value.stringPtr, MAX_STRING_LEN, from );
	}
}

ID_INLINE void idInterpreter::SetString( idVarDef* def, const char* from )
{
	if( def->initialized == idVarDef::stackVariable ) {
		idStr::Copynz( ( char* )&localstack[localstackBase + def->value.stackOffset], from, MAX_STRING_LEN );
	} else {
		idStr::Copynz( def->value.stringPtr, from, MAX_STRING_LEN );
	}
}

ID_INLINE const char* idInterpreter::GetString( idVarDef* def )
{
	if( def->initialized == idVarDef::stackVariable ) {
		return ( char* )&localstack[localstackBase + def->value.stackOffset];
	} else {
		return def->value.stringPtr;
	}
}

ID_INLINE varEval_t idInterpreter::GetVariable( idVarDef* def )
{
	if( def->initialized == idVarDef::stackVariable ) {
		varEval_t val;
		val.intPtr = ( int* )&localstack[localstackBase + def->value.stackOffset];
		return val;
	} else {
		return def->value;
	}
}

ID_INLINE void idInterpreter::NextInstruction( int position )
{
	// Before we execute an instruction, we increment instructionPointer,
	// therefore we need to compensate for that here.
	instructionPointer = position - 1;
}

#endif /* !__SCRIPT_INTERPRETER_H__ */
