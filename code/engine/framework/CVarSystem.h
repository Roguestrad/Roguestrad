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

#ifndef __CVARSYSTEM_H__
#define __CVARSYSTEM_H__

/*
===============================================================================

	Console Variables (CVars) are used to hold scalar or string variables
	that can be changed or displayed at the console as well as accessed
	directly in code.

	CVars are mostly used to hold settings that can be changed from the
	console or saved to and loaded from configuration files. CVars are also
	occasionally used to communicate information between different modules
	of the program.

	CVars are restricted from having the same names as console commands to
	keep the console interface from being ambiguous.

	CVars can be accessed from the console in three ways:
	cvarName			prints the current value
	cvarName X			sets the value to X if the variable exists
	set cvarName X		as above, but creates the CVar if not present

	CVars may be declared in the global namespace, in classes and in functions.
	However declarations in classes and functions should always be static to
	save space and time. Making CVars static does not change their
	functionality due to their global nature.

	CVars should be contructed only through one of the constructors with name,
	value, flags and description. The name, value and description parameters
	to the constructor have to be static strings, do not use va() or the like
	functions returning a string.

	CVars may be declared multiple times using the same name string. However,
	they will all reference the same value and changing the value of one CVar
	changes the value of all CVars with the same name.

	CVars should always be declared with the correct type flag: CVAR_BOOL,
	CVAR_INTEGER or CVAR_FLOAT. If no such flag is specified the CVar
	defaults to type string. If the CVAR_BOOL flag is used there is no need
	to specify an argument auto-completion function because the CVar gets
	one assigned automatically.

	CVars are automatically range checked based on their type and any min/max
	or valid string set specified in the constructor.

	CVars are always considered cheats except when CVAR_NOCHEAT, CVAR_INIT,
	CVAR_ROM, CVAR_ARCHIVE, CVAR_SERVERINFO, CVAR_NETWORKSYNC
	is set.

===============================================================================
*/

typedef enum {
	CVAR_ALL	  = -1,		  // all flags
	CVAR_BOOL	  = BIT( 0 ), // variable is a boolean
	CVAR_INTEGER  = BIT( 1 ), // variable is an integer
	CVAR_FLOAT	  = BIT( 2 ), // variable is a float
	CVAR_SYSTEM	  = BIT( 3 ), // system variable
	CVAR_RENDERER = BIT( 4 ), // renderer variable
	CVAR_SOUND	  = BIT( 5 ), // sound variable
	CVAR_GUI	  = BIT( 6 ), // gui variable
	CVAR_GAME	  = BIT( 7 ), // game variable
	CVAR_TOOL	  = BIT( 8 ), // tool variable
	// original doom3 used to have CVAR_USERINFO ("sent to servers, available to menu") here
	CVAR_SERVERINFO	 = BIT( 10 ), // sent from servers, available to menu
	CVAR_NETWORKSYNC = BIT( 11 ), // cvar is synced from the server to clients
	CVAR_STATIC		 = BIT( 12 ), // statically declared, not user created
	CVAR_CHEAT		 = BIT( 13 ), // variable is considered a cheat
	CVAR_NOCHEAT	 = BIT( 14 ), // variable is not considered a cheat
	CVAR_INIT		 = BIT( 15 ), // can only be set from the command-line
	CVAR_ROM		 = BIT( 16 ), // display only, cannot be set by user at all
	CVAR_ARCHIVE	 = BIT( 17 ), // set to cause it to be saved to a config file
	CVAR_MODIFIED	 = BIT( 18 ), // set when the variable is modified
	CVAR_NEW		 = BIT( 19 )  // added for RBDoom
} cvarFlags_t;

/*!
	\class idCVar
	\brief A console variable class for managing configurable parameters with various data types and constraints.

	This class provides a comprehensive interface for handling console variables, which are used to configure runtime behavior. It supports different data types including strings, booleans, integers,
   and floats, with optional constraints such as numeric ranges and predefined value lists. The class maintains internal state for tracking modifications and provides methods for both getting and
   setting values. Construction methods allow for initialization with various parameter combinations, including optional value completion functions for command-line interaction. The class handles
   registration with a central console variable system and supports static variable registration through inline methods. Virtual methods enable extension for custom behavior during value setting
   operations.

*/
class idCVar
{
public:
	//! Prevents instantiation of the default constructor for idCVar.
	idCVar() { assert( typeid( this ) != typeid( idCVar ) ); }

	/*!
		\brief Constructs an idCVar object with the specified name, value, flags, description, and optional value completion function.

		This constructor initializes a console variable with the provided parameters. It automatically sets up the value completion function if the CVAR_BOOL flag is specified and no completion
	   function is provided. The constructor calls the Init method to perform the actual initialization of the console variable internal state.

		\param name Name of the console variable
		\param value Initial value of the console variable
		\param flags Flags that define the behavior and properties of the console variable
		\param description Description text for the console variable
		\param valueCompletion Optional function pointer for command line argument completion
	*/
	idCVar( const char* name, const char* value, int flags, const char* description, argCompletion_t valueCompletion = NULL );

	/*!
		\brief Constructs an idCVar object with a name, initial value, flags, description, and numeric range constraints

		This constructor initializes a console variable with a specified name, default value, and associated flags. It also sets up the variable's description, minimum and maximum allowed values, and
	   optional argument completion callback. The constructor is marked as ID_INLINE, indicating it should be inlined for performance. The internal initialization is delegated to the Init method,
	   which handles the actual setup of the console variable's properties and registration within the system

		\param name Name of the console variable
		\param value Initial value of the console variable
		\param flags Flags that define the behavior and properties of the console variable
		\param description Description text for the console variable
		\param valueMin Minimum allowed value for the console variable
		\param valueMax Maximum allowed value for the console variable
		\param valueCompletion Optional callback function for argument completion assistance
	*/
	idCVar( const char* name, const char* value, int flags, const char* description, float valueMin, float valueMax, argCompletion_t valueCompletion = NULL );

	/*!
		\brief Constructs an idCVar object with the specified name, value, flags, description, and value strings.

		This constructor initializes a console variable with the provided parameters, including the variable's name, initial value, flags, description, and a list of valid value strings. The
	   constructor delegates to the Init method to perform the actual initialization work.

		\param name Name of the console variable
		\param value Initial value of the console variable
		\param flags Flags that define the behavior and properties of the console variable
		\param description Description of the console variable
		\param valueStrings Array of strings representing the valid values for the console variable
		\param valueCompletion Optional function for argument completion
	*/
	idCVar( const char* name, const char* value, int flags, const char* description, const char** valueStrings, argCompletion_t valueCompletion = NULL );

	virtual ~idCVar() { }

	//! Returns the name of the console variable.
	const char*		GetName() const { return internalVar->name; }

	//! Returns the flags associated with this cvar
	int				GetFlags() const { return internalVar->flags; }

	//! Returns the description string associated with this console variable.
	const char*		GetDescription() const { return internalVar->description; }

	//! Returns the minimum allowed value for this CVar.
	float			GetMinValue() const { return internalVar->valueMin; }

	//! Retrieves the maximum allowed value for this CVar.
	float			GetMaxValue() const { return internalVar->valueMax; }

	//! Returns the array of strings representing the valid values for this cvar.
	const char**	GetValueStrings() const { return valueStrings; }

	//! Returns the argument completion data for the cvar's value.
	argCompletion_t GetValueCompletion() const { return valueCompletion; }

	//! Returns true if the console variable has been modified since the last clear.
	bool			IsModified() const { return ( internalVar->flags & CVAR_MODIFIED ) != 0; }

	//! Marks the console variable as modified.
	void			SetModified() { internalVar->flags |= CVAR_MODIFIED; }

	//! Clears the modified flag from the internal variable.
	void			ClearModified() { internalVar->flags &= ~CVAR_MODIFIED; }

	//! Returns the default string value of the console variable.
	const char*		GetDefaultString() const { return internalVar->InternalGetResetString(); }

	//! Returns the string value of the console variable.
	const char*		GetString() const { return internalVar->value; }

	//! Returns the boolean value of the console variable.
	bool			GetBool() const { return ( internalVar->integerValue != 0 ); }

	//! Returns the integer value of this cvar.
	int				GetInteger() const { return internalVar->integerValue; }

	//! Returns the float value stored in the cvar.
	float			GetFloat() const { return internalVar->floatValue; }

	//! Sets the string value of the cvar.
	void			SetString( const char* value ) { internalVar->InternalSetString( value ); }

	//! Sets the boolean value of the cvar.
	void			SetBool( const bool value ) { internalVar->InternalSetBool( value ); }

	//! Sets the integer value of the console variable.
	void			SetInteger( const int value ) { internalVar->InternalSetInteger( value ); }

	//! Sets the float value of the console variable.
	void			SetFloat( const float value ) { internalVar->InternalSetFloat( value ); }

	//! Sets the internal variable of this cvar to the provided cvar.
	void			SetInternalVar( idCVar* cvar ) { internalVar = cvar; }

	//! Registers static CVar variables with the CVar system.
	static void		RegisterStaticVars();

protected:
	const char*		name;			 // name
	const char*		value;			 // value
	const char*		description;	 // description
	int				flags;			 // CVAR_? flags
	float			valueMin;		 // minimum value
	float			valueMax;		 // maximum value
	const char**	valueStrings;	 // valid value strings
	argCompletion_t valueCompletion; // value auto-completion function
	int				integerValue;	 // atoi( string )
	float			floatValue;		 // atof( value )
	idCVar*			internalVar;	 // internal cvar
	idCVar*			next;			 // next statically declared cvar

private:
	/*!
		\brief Initializes a console variable with the specified name, value, flags, and other properties.

		This function sets up the internal state of a console variable object, including its name, initial value, flags, description, and value constraints. It also handles registration of the
	   variable with the console variable system, either through a static list or by calling the cvar system's register function. The function ensures that the variable is marked as static and
	   initializes various internal fields such as integer and float values, as well as pointers to related structures.

		\param name Name of the console variable
		\param value Initial value of the console variable
		\param flags Flags that define the behavior and properties of the console variable
		\param description Description text for the console variable
		\param valueMin Minimum allowed value for the console variable
		\param valueMax Maximum allowed value for the console variable
		\param valueStrings Array of strings that represent valid values for the console variable
		\param valueCompletion Function pointer for tab completion of the console variable value
	*/
	void				Init( const char* name, const char* value, int flags, const char* description, float valueMin, float valueMax, const char** valueStrings, argCompletion_t valueCompletion );

	//! Sets the internal string value of the console variable.
	virtual void		InternalSetString( const char* newValue ) { }

	//! Sets the console variable to the specified boolean value.
	virtual void		InternalSetBool( const bool newValue ) { }

	//! Sets the internal integer value of the console variable.
	virtual void		InternalSetInteger( const int newValue ) { }

	//! Sets the internal float value of the console variable.
	virtual void		InternalSetFloat( const float newValue ) { }

	//! Returns the reset string value of the console variable.
	virtual const char* InternalGetResetString() const { return value; }

	static idCVar*		staticVars;
};

ID_INLINE idCVar::idCVar( const char* name, const char* value, int flags, const char* description, argCompletion_t valueCompletion )
{
	if( !valueCompletion && ( flags & CVAR_BOOL ) ) { valueCompletion = idCmdSystem::ArgCompletion_Boolean; }
	Init( name, value, flags, description, 1, -1, NULL, valueCompletion );
}

ID_INLINE idCVar::idCVar( const char* name, const char* value, int flags, const char* description, float valueMin, float valueMax, argCompletion_t valueCompletion )
{
	Init( name, value, flags, description, valueMin, valueMax, NULL, valueCompletion );
}

ID_INLINE idCVar::idCVar( const char* name, const char* value, int flags, const char* description, const char** valueStrings, argCompletion_t valueCompletion )
{
	Init( name, value, flags, description, 1, -1, valueStrings, valueCompletion );
}

/*!
	\class idCVarSystem
	\brief System for managing configuration variables with registration, retrieval, and command-line interface support.

	Provides a centralized interface for handling configuration variables, allowing registration of cvars, setting and getting their values in various types, and integration with command-line argument
   processing. The system supports flagging variables for different purposes such as persistence or auto-completion, and offers methods to serialize and deserialize cvar states. It is designed to be
   extended by concrete implementations that handle the actual storage and logic for cvar management.

*/
class idCVarSystem
{
public:
	virtual ~idCVarSystem() { }

	virtual void		Init()				  = 0;
	virtual void		Shutdown()			  = 0;
	virtual bool		IsInitialized() const = 0;

	// Registers a CVar.
	virtual void		Register( idCVar* cvar ) = 0;

	// Finds the CVar with the given name.
	// Returns NULL if there is no CVar with the given name.
	virtual idCVar*		Find( const char* name ) = 0;

	// Sets the value of a CVar by name.
	virtual void		SetCVarString( const char* name, const char* value, int flags = 0 ) = 0;
	virtual void		SetCVarBool( const char* name, const bool value, int flags = 0 )	= 0;
	virtual void		SetCVarInteger( const char* name, const int value, int flags = 0 )	= 0;
	virtual void		SetCVarFloat( const char* name, const float value, int flags = 0 )	= 0;

	// Gets the value of a CVar by name.
	virtual const char* GetCVarString( const char* name ) const	 = 0;
	virtual bool		GetCVarBool( const char* name ) const	 = 0;
	virtual int			GetCVarInteger( const char* name ) const = 0;
	virtual float		GetCVarFloat( const char* name ) const	 = 0;

	// Called by the command system when argv(0) doesn't match a known command.
	// Returns true if argv(0) is a variable reference and prints or changes the CVar.
	virtual bool		Command( const idCmdArgs& args ) = 0;

	// Command and argument completion using callback for each valid string.
	virtual void		CommandCompletion( void ( *callback )( const char* s ) )					= 0;
	virtual void		ArgCompletion( const char* cmdString, void ( *callback )( const char* s ) ) = 0;

	// Sets/gets/clears modified flags that tell what kind of CVars have changed.
	virtual void		SetModifiedFlags( int flags )	= 0;
	virtual int			GetModifiedFlags() const		= 0;
	virtual void		ClearModifiedFlags( int flags ) = 0;

	// Resets variables with one of the given flags set.
	virtual void		ResetFlaggedVariables( int flags ) = 0;

	// Removes auto-completion from the flagged variables.
	virtual void		RemoveFlaggedAutoCompletion( int flags ) = 0;

	// Writes variables with one of the given flags set to the given file.
	virtual void		WriteFlaggedVariables( int flags, const char* setCmd, idFile* f ) const = 0;

	// Moves CVars to and from dictionaries.
	virtual void		MoveCVarsToDict( int flags, idDict& dict, bool onlyModified = false ) const = 0;
	virtual void		SetCVarsFromDict( const idDict& dict )										= 0;
};

extern idCVarSystem* cvarSystem;

ID_INLINE void		 idCVar::Init( const char* name, const char* value, int flags, const char* description, float valueMin, float valueMax, const char** valueStrings, argCompletion_t valueCompletion )
{
	this->name			  = name;
	this->value			  = value;
	this->flags			  = flags;
	this->description	  = description;
	this->flags			  = flags | CVAR_STATIC;
	this->valueMin		  = valueMin;
	this->valueMax		  = valueMax;
	this->valueStrings	  = valueStrings;
	this->valueCompletion = valueCompletion;
	this->integerValue	  = 0;
	this->floatValue	  = 0.0f;
	this->internalVar	  = this;
	if( staticVars != ( idCVar* )UINTPTR_MAX ) {
		this->next = staticVars;
		staticVars = this;
	} else {
		cvarSystem->Register( this );
	}
}

ID_INLINE void idCVar::RegisterStaticVars()
{
	if( staticVars != ( idCVar* )UINTPTR_MAX ) {
		for( idCVar* cvar = staticVars; cvar; cvar = cvar->next ) {
			cvarSystem->Register( cvar );
		}
		staticVars = ( idCVar* )UINTPTR_MAX;
	}
}

#endif /* !__CVARSYSTEM_H__ */
