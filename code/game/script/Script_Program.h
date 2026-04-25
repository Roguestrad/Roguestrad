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

#ifndef __SCRIPT_PROGRAM_H__
#define __SCRIPT_PROGRAM_H__

class idScriptObject;
class idEventDef;
class idVarDef;
class idTypeDef;
class idEntity;
class idThread;
class idSaveGame;
class idRestoreGame;

#define MAX_STRING_LEN 128
#define MAX_GLOBALS	   296608 * 2 // in bytes, RB: doubled for Runners 2 mod #835
#define MAX_STRINGS	   1024
#define MAX_FUNCS	   3584
#define MAX_STATEMENTS 131072 // statement_t - 18 bytes last I checked

typedef enum {
	ev_error = -1,
	ev_void,
	ev_scriptevent,
	ev_namespace,
	ev_string,
	ev_float,
	ev_vector,
	ev_entity,
	ev_field,
	ev_function,
	ev_virtualfunction,
	ev_pointer,
	ev_object,
	ev_jumpoffset,
	ev_argsize,
	ev_boolean
} etype_t;

/*!
	\class function_t
	\brief A class representing a function object with name management and memory tracking capabilities.
*/
class function_t
{
public:
	//! Initializes a new instance of function_t and clears its state.
	function_t();

	//! Returns the total allocated memory size for the function_t object
	size_t		Allocated() const;

	//! Sets the name of the function object to the provided string.
	void		SetName( const char* name );

	//! Returns the name of the function
	const char* Name() const;

	//! Clears all member variables of the function_t object to their default states.
	void		Clear();

private:
	idStr name;

public:
	const idEventDef*		eventdef;
	idVarDef*				def;
	const idTypeDef*		type;
	int						firstStatement;
	int						numStatements;
	int						parmTotal;
	int						locals;	 // total ints of parms + locals
	int						filenum; // source file defined in
	idList<int, TAG_SCRIPT> parmSize;
};

typedef union eval_s {
	const char* stringPtr;
	float		_float;
	float		vector[3];
	function_t* function;
	int			_int;
	int			entity;
} eval_t;

/***********************************************************************

idTypeDef

Contains type information for variables and functions.

***********************************************************************/

class idTypeDef
{
private:
	etype_t								  type;
	idStr								  name;
	int									  size;

	// function types are more complex
	idTypeDef*							  auxType; // return type
	idList<idTypeDef*, TAG_SCRIPT>		  parmTypes;
	idStrList							  parmNames;
	idList<const function_t*, TAG_SCRIPT> functions;

public:
	idVarDef* def; // a def that points to this type

	//! Constructs a new idTypeDef object as a copy of another idTypeDef object.
	idTypeDef( const idTypeDef& other );

	/*!
		\brief Constructs an idTypeDef object with the specified type, definition, name, size, and auxiliary type.

		Initializes the idTypeDef object by setting its name, type, definition, size, and auxiliary type. Additionally, it configures the granularity of parameter types, parameter names, and functions
	   collections to optimize memory allocation.

		\param etype The type of the definition being constructed
		\param edef Pointer to the variable definition associated with this type
		\param ename Name of the type being constructed
		\param esize Size of the type being constructed
		\param aux Pointer to the auxiliary type definition
	*/
	idTypeDef( etype_t etype, idVarDef* edef, const char* ename, int esize, idTypeDef* aux );

	//! Assigns the contents of another idTypeDef instance to this instance.
	void			  operator=( const idTypeDef& other );

	//! Returns the total allocated memory size for the type definition object.
	size_t			  Allocated() const;

	//! Returns true if the specified type is an ancestor of this type.
	bool			  Inherits( const idTypeDef* basetype ) const;

	//! Returns true if both types' base types and parameters match
	bool			  MatchesType( const idTypeDef& matchtype ) const;

	//! Returns true if both functions' base types and parameters match.
	bool			  MatchesVirtualFunction( const idTypeDef& matchfunc ) const;

	//! Adds a new parameter to a function type definition.
	void			  AddFunctionParm( idTypeDef* parmtype, const char* name );

	//! Adds a new field to an object type.
	void			  AddField( idTypeDef* fieldtype, const char* name );

	//! Sets the name of the type definition to the provided string.
	void			  SetName( const char* newname );

	//! Returns the name string of this type definition
	const char*		  Name() const;

	//! Returns the type of the data definition.
	etype_t			  Type() const;

	//! Returns the size in bytes of the type definition
	int				  Size() const;

	//! Returns the superclass of an object type.
	idTypeDef*		  SuperClass() const;

	//! Returns the return type of a function type, or throws an error if the type is not a function.
	idTypeDef*		  ReturnType() const;

	//! Sets the return type for a function type definition
	void			  SetReturnType( idTypeDef* type );

	//! Returns the type of a field if the current type is a field.
	idTypeDef*		  FieldType() const;

	//! Sets the return type for a function type.
	void			  SetFieldType( idTypeDef* type );

	//! Returns the type that this pointer type points to.
	idTypeDef*		  PointerType() const;

	//! Sets the type of a pointer if the current type is a pointer.
	void			  SetPointerType( idTypeDef* type );

	//! Returns the number of parameters defined for this type definition.
	int				  NumParameters() const;

	//! Returns the type of the specified parameter.
	idTypeDef*		  GetParmType( int parmNumber ) const;

	//! Returns the name of a parameter at the specified index.
	const char*		  GetParmName( int parmNumber ) const;

	//! Returns the number of functions associated with this type definition.
	int				  NumFunctions() const;

	//! Returns the index of the specified function within the type definition's function list, or -1 if not found.
	int				  GetFunctionNumber( const function_t* func ) const;

	//! Returns a pointer to the function_t structure at the specified index in the functions array.
	const function_t* GetFunction( int funcNumber ) const;

	//! Adds a function to the type definition, replacing any existing function with the same name.
	void			  AddFunction( const function_t* func );
};

/***********************************************************************

idScriptObject

In-game representation of objects in scripts.  Use the idScriptVariable template
(below) to access variables.

***********************************************************************/

class idScriptObject
{
private:
	idTypeDef* type;

public:
	byte* data;

	//! Constructs a new idScriptObject instance with null data and the object type.
	idScriptObject();

	//! Destructor for idScriptObject that frees associated resources.
	~idScriptObject();

	//! Archives object for save game file
	void			  Save( idSaveGame* savefile ) const;

	//! Restores object state from a save game file.
	void			  Restore( idRestoreGame* savefile );

	//! Frees the memory allocated for the script object data and resets its state.
	void			  Free();

	//! Sets the type of the script object to the specified type name and initializes its memory.
	bool			  SetType( const char* typeName );

	//! Resets the memory for the script object without changing its type.
	void			  ClearObject();

	//! Returns true if the script object has a valid object type.
	bool			  HasObject() const;

	//! Returns the type definition associated with this script object.
	idTypeDef*		  GetTypeDef() const;

	//! Returns the name of the type for this script object.
	const char*		  GetTypeName() const;

	//! Returns the constructor function for the script object.
	const function_t* GetConstructor() const;

	//! Returns the destructor function for the script object.
	const function_t* GetDestructor() const;

	//! Returns the function pointer for a given function name from the script object
	const function_t* GetFunction( const char* name ) const;

	//! Retrieves a script variable by name and type from the object's type definition and data.
	byte*			  GetVariable( const char* name, etype_t etype ) const;
};

/***********************************************************************

idScriptVariable

Helper template that handles looking up script variables stored in objects.
If the specified variable doesn't exist, or is the wrong data type, idScriptVariable
will cause an error.

***********************************************************************/

template<class type, etype_t etype, class returnType>
class idScriptVariable
{
private:
	type* data;

public:
	//! Initializes a new instance of the idScriptVariable class with data set to NULL.
	idScriptVariable();

	//! Returns true if the script variable is linked to data.
	bool			  IsLinked() const;

	//! Removes the link to the script variable data by setting it to NULL.
	void			  Unlink();

	//! Links this variable to a field in the given script object
	void			  LinkTo( idScriptObject& obj, const char* name );

	//! Assigns a value to the script variable and returns a reference to itself
	idScriptVariable& operator=( const returnType& value );

	//! Converts the script variable to its underlying return type.
	operator returnType() const;
};

template<class type, etype_t etype, class returnType>
ID_INLINE idScriptVariable<type, etype, returnType>::idScriptVariable()
{
	data = NULL;
}

template<class type, etype_t etype, class returnType>
ID_INLINE bool idScriptVariable<type, etype, returnType>::IsLinked() const
{
	return ( data != NULL );
}

template<class type, etype_t etype, class returnType>
ID_INLINE void idScriptVariable<type, etype, returnType>::Unlink()
{
	data = NULL;
}

template<class type, etype_t etype, class returnType>
ID_INLINE void idScriptVariable<type, etype, returnType>::LinkTo( idScriptObject& obj, const char* name )
{
	data = ( type* )obj.GetVariable( name, etype );
	if( !data ) { gameError( "Missing '%s' field in script object '%s'", name, obj.GetTypeName() ); }
}

template<class type, etype_t etype, class returnType>
ID_INLINE idScriptVariable<type, etype, returnType>& idScriptVariable<type, etype, returnType>::operator=( const returnType& value )
{
	// check if we attempt to access the object before it's been linked
	assert( data );

	// make sure we don't crash if we don't have a pointer
	if( data ) { *data = ( type )value; }
	return *this;
}

template<class type, etype_t etype, class returnType>
ID_INLINE idScriptVariable<type, etype, returnType>::operator returnType() const
{
	// check if we attempt to access the object before it's been linked
	assert( data );

	// make sure we don't crash if we don't have a pointer
	if( data ) {
		return ( const returnType )*data;
	} else {
		// reasonably safe value
		return ( const returnType )0;
	}
}

/***********************************************************************

Script object variable access template instantiations

These objects will automatically handle looking up of the current value
of a variable in a script object.  They can be stored as part of a class
for up-to-date values of the variable, or can be used in functions to
sample the data for non-dynamic values.

***********************************************************************/

typedef idScriptVariable<int, ev_boolean, int>			idScriptBool;
typedef idScriptVariable<float, ev_float, float>		idScriptFloat;
typedef idScriptVariable<float, ev_float, int>			idScriptInt;
typedef idScriptVariable<idVec3, ev_vector, idVec3>		idScriptVector;
typedef idScriptVariable<idStr, ev_string, const char*> idScriptString;

/***********************************************************************

idCompileError

Causes the compiler to exit out of compiling the current function and
display an error message with line and file info.

***********************************************************************/

class idCompileError : public idException
{
public:
	//! Constructs an idCompileError object with the specified error text.
	idCompileError( const char* text ) :
		idException( text )
	{
	}
};

/***********************************************************************

idVarDef

Define the name, type, and location of variables, functions, and objects
defined in script.

***********************************************************************/

typedef union varEval_s {
	idScriptObject** objectPtrPtr;
	char*			 stringPtr;
	float*			 floatPtr;
	idVec3*			 vectorPtr;
	function_t*		 functionPtr;
	int*			 intPtr;
	byte*			 bytePtr;
	int*			 entityNumberPtr;
	int				 virtualFunction;
	int				 jumpOffset;
	int				 stackOffset; // offset in stack for local variables
	int				 argSize;
	varEval_s*		 evalPtr;
	int				 ptrOffset;
} varEval_t;

class idVarDefName;

/*!
	\class idVarDef
	\brief Represents a variable definition with type and scope information.

	The idVarDef class encapsulates the definition of a variable within a scripting system, maintaining its type, name, scope, and value. It provides mechanisms to manage variable properties such as
   type definition, function association, object reference, and value assignment. The class supports scope management through depth calculation and maintains linked list functionality for variable
   names in hash chains. It also offers facilities for printing variable information and managing initialization status. The class is designed to work within a script compilation and execution
   environment where variables may have associated function contexts or object references.

*/
class idVarDef
{
	friend class idVarDefName;

public:
	int		  num; // global index/ID of variable
	varEval_t value;
	idVarDef* scope;	// function, namespace, or object the var was defined in
	int		  numUsers; // number of users if this is a constant

	typedef enum { uninitialized, initializedVariable, initializedConstant, stackVariable } initialized_t;

	initialized_t initialized;

public:
	//! Constructs an idVarDef object with an optional type definition pointer.
	idVarDef( idTypeDef* typeptr = NULL );

	//! Destroys the variable definition and removes it from its name's definition list.
	~idVarDef();

	//! Returns the name of the variable definition.
	const char* Name() const;

	//! Returns the global name of the variable definition.
	const char* GlobalName() const;

	//! Sets the type definition for this variable definition.
	void		SetTypeDef( idTypeDef* _type ) { typeDef = _type; }

	//! Returns the type definition associated with this variable definition.
	idTypeDef*	TypeDef() const { return typeDef; }

	//! Returns the type of the variable definition.
	etype_t		Type() const { return ( typeDef != NULL ) ? typeDef->Type() : ev_void; }

	//! Returns the depth of the specified scope relative to this scope.
	int			DepthOfScope( const idVarDef* otherScope ) const;

	//! Sets the function pointer for this variable definition
	void		SetFunction( function_t* func );

	//! Sets the object value for this variable definition
	void		SetObject( idScriptObject* object );

	//! Sets the value of a variable definition based on its type and initialization status.
	void		SetValue( const eval_t& value, bool constant );

	//! Sets the string value of the variable definition and marks it as constant or variable.
	void		SetString( const char* string, bool constant );

	//! Returns the next variable definition with the same name in the hash chain.
	idVarDef*	Next() const
	{
		return next; // next var def with same name
	}

	//! Prints information about the variable definition to the specified file.
	void PrintInfo( idFile* file, int instructionPointer ) const;

private:
	idTypeDef*	  typeDef;
	idVarDefName* name; // name of this var
	idVarDef*	  next; // next var with the same name
};

/***********************************************************************

  idVarDefName

***********************************************************************/

class idVarDefName
{
public:
	//! Initializes a new instance of idVarDefName with defs set to NULL.
	idVarDefName() { defs = NULL; }

	//! Initializes a new idVarDefName object with the specified name.
	idVarDefName( const char* n )
	{
		name = n;
		defs = NULL;
	}

	//! Returns the name of the variable definition.
	const char* Name() const { return name; }

	//! Returns the variable definitions stored in this name.
	idVarDef*	GetDefs() const { return defs; }

	//! Adds a variable definition to the list of definitions for this name.
	void		AddDef( idVarDef* def );

	//! Removes a variable definition from the list of definitions associated with this name.
	void		RemoveDef( idVarDef* def );

private:
	idStr	  name;
	idVarDef* defs;
};

/***********************************************************************

  Variable and type defintions

***********************************************************************/

extern idTypeDef type_void;
extern idTypeDef type_scriptevent;
extern idTypeDef type_namespace;
extern idTypeDef type_string;
extern idTypeDef type_float;
extern idTypeDef type_vector;
extern idTypeDef type_entity;
extern idTypeDef type_field;
extern idTypeDef type_function;
extern idTypeDef type_virtualfunction;
extern idTypeDef type_pointer;
extern idTypeDef type_object;
extern idTypeDef type_jumpoffset; // only used for jump opcodes
extern idTypeDef type_argsize;	  // only used for function call and thread opcodes
extern idTypeDef type_boolean;

extern idVarDef	 def_void;
extern idVarDef	 def_scriptevent;
extern idVarDef	 def_namespace;
extern idVarDef	 def_string;
extern idVarDef	 def_float;
extern idVarDef	 def_vector;
extern idVarDef	 def_entity;
extern idVarDef	 def_field;
extern idVarDef	 def_function;
extern idVarDef	 def_virtualfunction;
extern idVarDef	 def_pointer;
extern idVarDef	 def_object;
extern idVarDef	 def_jumpoffset; // only used for jump opcodes
extern idVarDef	 def_argsize;	 // only used for function call and thread opcodes
extern idVarDef	 def_boolean;

typedef struct statement_s {
	unsigned short op;
	unsigned short flags; // DG: added this for ugly hacks
	enum {
		// op is OP_OBJECTCALL and when the statement was created the function/method
		// implementation hasn't been parsed yet (only the declaration/prototype)
		// see idCompiler::EmitFunctionParms() and idProgram::CalculateChecksum()
		FLAG_OBJECTCALL_IMPL_NOT_PARSED_YET = 1,
	};
	// DG: moved linenumber and file up here to prevent wasting 8 bytes of padding on 64bit
	unsigned short linenumber;
	unsigned short file;
	idVarDef*	   a;
	idVarDef*	   b;
	idVarDef*	   c;
} statement_t;

/***********************************************************************

idProgram

Handles compiling and storage of script data.  Multiple idProgram objects
would represent separate programs with no knowledge of each other.  Scripts
meant to access shared data and functions should all be compiled by a
single idProgram.

***********************************************************************/

class idProgram
{
private:
	idStrList								  fileList;
	idStr									  filename;
	int										  filenum;

	int										  numVariables;
	byte									  variables[MAX_GLOBALS];
	idStaticList<byte, MAX_GLOBALS>			  variableDefaults;
	idStaticList<function_t, MAX_FUNCS>		  functions;
	idStaticList<statement_t, MAX_STATEMENTS> statements;
	idList<idTypeDef*, TAG_SCRIPT>			  types;
	idHashIndex								  typesHash;
	idList<idVarDefName*, TAG_SCRIPT>		  varDefNames;
	idHashIndex								  varDefNameHash;
	idList<idVarDef*, TAG_SCRIPT>			  varDefs;

	idVarDef*								  sysDef;

	int										  top_functions;
	int										  top_statements;
	int										  top_types;
	int										  top_defs;
	int										  top_files;

	//! Reports memory usage statistics after all source files have been compiled.
	void									  CompileStats();

	//! Reserves memory for program definitions and returns a pointer to the allocated memory.
	byte*									  ReserveDefMemory( int size );

	//! Allocates a new variable definition with the specified type, name, and scope.
	idVarDef*								  AllocVarDef( idTypeDef* type, const char* name, idVarDef* scope );

public:
	idVarDef* returnDef;
	idVarDef* returnStringDef;

	//! Initializes a new instance of the idProgram class.
	idProgram();

	//! Destructor for the idProgram class that cleans up allocated data.
	~idProgram();

	//! Saves the program state to a save file
	void			  Save( idSaveGame* savefile ) const;

	//! Restores the program state from a save file.
	bool			  Restore( idRestoreGame* savefile );

	//! Calculates a checksum for the program's bytecode, optionally compatible with old savegames.
	int				  CalculateChecksum( bool forOldSavegame ) const;

	//! Initializes the script system and loads the default script file.
	void			  Startup( const char* defaultScript );

	//! Resets all script variables to their initial values and cleans up dynamically allocated script resources.
	void			  Restart();

	//! Compiles textual program code from a source file and optional console output
	bool			  CompileText( const char* source, const char* text, bool console );

	//! Compiles a function from text and returns a pointer to the compiled function.
	const function_t* CompileFunction( const char* functionName, const char* text );

	//! Compiles a program file specified by filename.
	void			  CompileFile( const char* filename );

	//! Initializes the program compilation state and clears previous data.
	void			  BeginCompilation();

	//! Finalizes the compilation process by setting up required data structures and default variable values.
	void			  FinishCompilation();

	//! Disassembles and writes a single statement from the program to the specified file
	void			  DisassembleStatement( idFile* file, int instructionPointer ) const;

	//! Writes the disassembled code of all functions to a file named disasm.txt.
	void			  Disassemble() const;

	//! Frees all dynamically allocated data within the program, resetting internal state to a clean condition.
	void			  FreeData();

	//! Returns the filename at the specified index from the program's file list.
	const char*		  GetFilename( int num );

	//! Returns the file number for a given file name, using a cached result when possible.
	int				  GetFilenum( const char* name );

	//! Returns the line number for a specified statement index.
	int				  GetLineNumberForStatement( int index );

	//! Returns the filename for a given statement index.
	const char*		  GetFilenameForStatement( int index );

	//! Allocates a new type definition and adds it to the program's type hash table.
	idTypeDef*		  AllocType( idTypeDef& type );

	/*!
		\brief Allocates and initializes a new type definition with the specified parameters.

		This function creates a new instance of idTypeDef using the provided type information and registers it in the program's type hash table. The new type is appended to the types list and indexed
	   by the hash of its name for fast lookup. The function is typically used during script compilation to create new type definitions for variables, functions, and other script elements.

		\param etype The type of the new definition
		\param edef The variable definition associated with this type
		\param ename The name of the new type
		\param esize The size of the type in bytes
		\param aux An auxiliary type definition that may be referenced by this type
		\return A pointer to the newly created and registered type definition
	*/
	idTypeDef*		  AllocType( etype_t etype, idVarDef* edef, const char* ename, int esize, idTypeDef* aux );

	//! Returns an existing type that matches the provided type or allocates a new one if no match is found.
	idTypeDef*		  GetType( idTypeDef& type, bool allocate );

	//! Returns a preexisting complex type that matches the name, or returns NULL if not found
	idTypeDef*		  FindType( const char* name );

	/*!
		\brief Allocates and initializes a new variable definition with optional vector component definitions.

		This function creates a new variable definition for use in the program's symbol table. It handles special cases for vector types by creating additional definitions for the _x, _y, and _z
	   components. For object variables, it sets the appropriate memory offset within the object structure. For stack variables, it allocates space within the function's local stack frame. For global
	   variables, it reserves memory in the global data segment. The function also manages the initialization state and memory offsets for the created definition and its components.

		\param type The type of the variable to be defined
		\param name The name of the variable to be defined
		\param scope The scope in which the variable is defined, determining memory allocation strategy
		\param constant Flag indicating whether the variable is constant
		\return A pointer to the newly allocated and initialized variable definition
	*/
	idVarDef*		  AllocDef( idTypeDef* type, const char* name, idVarDef* scope, bool constant );

	//! Returns the variable definition with the specified name and type, considering scoping rules
	idVarDef*		  GetDef( const idTypeDef* type, const char* name, const idVarDef* scope ) const;

	//! Frees a variable definition and its associated vector components if applicable.
	void			  FreeDef( idVarDef* d, const idVarDef* scope );

	/*!
		\brief Finds a free variable definition with the specified type, name, and scope, or allocates a new one if none is available.

		The function searches through a list of existing variable definitions to find one that matches the provided type, name, and scope, while excluding definitions a and b. It ensures that the
	   found definition has more than one user to avoid conflicts. If no suitable definition is found, it allocates a new one with the specified parameters.

		\param type The type of the variable definition to find or create
		\param name The name of the variable definition to find or create
		\param scope The scope in which the variable definition should be found or created
		\param a A variable definition to exclude from consideration
		\param b Another variable definition to exclude from consideration
		\return A pointer to an existing or newly allocated variable definition that matches the criteria, or NULL if allocation fails.
	*/
	idVarDef*		  FindFreeResultDef( idTypeDef* type, const char* name, idVarDef* scope, const idVarDef* a, const idVarDef* b );

	//! Retrieves the definition list for a variable name from the program's hash table.
	idVarDef*		  GetDefList( const char* name ) const;

	//! Adds a variable definition to the name list for the given name.
	void			  AddDefToNameList( idVarDef* def, const char* name );

	//! Returns a pointer to a function definition if found, otherwise returns NULL.
	function_t*		  FindFunction( const char* name ) const;

	//! Returns the function pointer for a given function name and type, or NULL if not found.
	function_t*		  FindFunction( const char* name, const idTypeDef* type ) const;

	//! Allocates and initializes a new function entry in the program's function list
	function_t&		  AllocFunction( idVarDef* def );

	//! Returns a pointer to the function at the specified index in the program's function table.
	function_t*		  GetFunction( int index );

	//! Returns the index of a function within the program's function array.
	int				  GetFunctionIndex( const function_t* func );

	//! Sets an entity variable in the program with the specified name.
	void			  SetEntity( const char* name, idEntity* ent );

	//! Allocates and returns a new statement from the program's statement pool.
	statement_t*	  AllocStatement();

	//! Returns a reference to the statement at the specified index in the program.
	statement_t&	  GetStatement( int index );

	//! Returns the number of statements in the program.
	int				  NumStatements() { return statements.Num(); }

	//! Returns the integer value that was previously set as the return value for the program.
	int				  GetReturnedInteger();

	//! Returns the float value that was previously set as the return value for the program.
	float			  GetReturnedFloat();

	//! Returns the vector value that was set as the return value of the program.
	idVec3			  GetReturnedVec3();
	idEntity*		  GetReturnedEntity();

	//! Returns the string value that was last returned by the program.
	const char*		  GetReturnedString();

	//! Returns the boolean value of the most recently returned integer from the program.
	bool			  GetReturnedBool();

	//! Sets the return value of the current function to the specified float value.
	void			  ReturnFloat( float value );

	//! Sets the return value for the current program execution to the specified integer.
	void			  ReturnInteger( int value );

	//! Sets the return value of the program to the specified vector.
	void			  ReturnVector( idVec3 const& vec );

	//! Sets the return string value for the program.
	void			  ReturnString( const char* string );

	//! Sets the return value of the program to the entity number of the specified entity or zero if the entity is null.
	void			  ReturnEntity( idEntity* ent );

	//! Returns the number of filenames in the program's file list.
	int				  NumFilenames() { return fileList.Num(); }
};

ID_INLINE statement_t& idProgram::GetStatement( int index )
{
	return statements[index];
}

ID_INLINE function_t* idProgram::GetFunction( int index )
{
	return &functions[index];
}

ID_INLINE int idProgram::GetFunctionIndex( const function_t* func )
{
	return func - &functions[0];
}

ID_INLINE int idProgram::GetReturnedInteger()
{
	return *returnDef->value.intPtr;
}

ID_INLINE float idProgram::GetReturnedFloat()
{
	return *returnDef->value.floatPtr;
}

ID_INLINE idVec3 idProgram::GetReturnedVec3()
{
	return *returnDef->value.vectorPtr;
}

ID_INLINE const char* idProgram::GetReturnedString()
{
	return returnDef->value.stringPtr;
}

ID_INLINE bool idProgram::GetReturnedBool()
{
	return GetReturnedInteger() != 0;
}

ID_INLINE void idProgram::ReturnFloat( float value )
{
	*returnDef->value.floatPtr = value;
}

ID_INLINE void idProgram::ReturnInteger( int value )
{
	*returnDef->value.intPtr = value;
}

ID_INLINE void idProgram::ReturnVector( idVec3 const& vec )
{
	*returnDef->value.vectorPtr = vec;
}

ID_INLINE void idProgram::ReturnString( const char* string )
{
	idStr::Copynz( returnStringDef->value.stringPtr, string, MAX_STRING_LEN );
}

ID_INLINE const char* idProgram::GetFilename( int num )
{
	return fileList[num];
}

ID_INLINE int idProgram::GetLineNumberForStatement( int index )
{
	return statements[index].linenumber;
}

ID_INLINE const char* idProgram::GetFilenameForStatement( int index )
{
	return GetFilename( statements[index].file );
}

#endif /* !__SCRIPT_PROGRAM_H__ */
