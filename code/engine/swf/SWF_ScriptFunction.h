/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans

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
#ifndef __SWF_SCRIPTFUNCTION_H__
#define __SWF_SCRIPTFUNCTION_H__

/*!
	\class idSWFScriptFunction
	\brief Interface for calling functions from script.
*/
class idSWFScriptFunction
{
public:
	virtual ~idSWFScriptFunction() {};

	//! Calls the script function with the specified object and parameters, though this implementation should never be reached.
	virtual idSWFScriptVar	   Call( idSWFScriptObject* thisObject, const idSWFParmList& parms ) { return idSWFScriptVar(); };
	virtual void			   AddRef() {};
	virtual void			   Release() {};

	//! Returns the prototype object for this SWF script function, or NULL if not implemented.
	virtual idSWFScriptObject* GetPrototype() { return NULL; }

	//! Sets the prototype object for this script function.
	virtual void			   SetPrototype( idSWFScriptObject* _object ) { }
};

/*!
	\class idSWFScriptFunction_Static
	\brief Provides a static interface for calling functions from script.
*/
class idSWFScriptFunction_Static : public idSWFScriptFunction
{
public:
	//! Constructs a new instance of idSWFScriptFunction_Static.
	idSWFScriptFunction_Static() { }

	//! Adds a reference to this script function object
	virtual void AddRef() { }

	//! Releases the object's resources and performs cleanup.
	virtual void Release() { }
};

/*!
	\class idSWFScriptFunction_Nested
	\brief A template class that provides an interface for calling functions from script, implemented natively on a nested class object.
*/
template<typename T>
class idSWFScriptFunction_Nested : public idSWFScriptFunction
{
protected:
	T* pThis;

public:
	//! Initializes a new instance of the idSWFScriptFunction_Nested template class.
	idSWFScriptFunction_Nested() :
		pThis( NULL )
	{
	}

	//! Binds the nested script function to a parent instance and returns a pointer to itself.
	idSWFScriptFunction* Bind( T* _pThis )
	{
		pThis = _pThis;
		return this;
	}

	//! Increments the reference count of the SWF script function object.
	virtual void AddRef() { }

	//! Releases any resources held by the nested script function object
	virtual void Release() { }
};

/*!
	\class idSWFScriptFunction_RefCounted
	\brief Provides a reference-counted interface for SWF script functions.
*/
class idSWFScriptFunction_RefCounted : public idSWFScriptFunction
{
public:
	//! Constructs a new SWF script function with a zero reference count.
	idSWFScriptFunction_RefCounted() :
		refCount( 0 )
	{
	}

	//! Increments the reference count of the SWF script function.
	void AddRef() { refCount++; }

	//! Decrements the reference count and deletes the object if the count reaches zero.
	void Release()
	{
		if( --refCount <= 0 ) { delete this; }
	}

private:
	std::atomic<int> refCount;
};

/*!
	\class idSWFConstantPool
	\brief Manages a collection of constant values used within action script execution.
*/
class idSWFConstantPool
{
public:
	//! Initializes an empty SWF constant pool.
	idSWFConstantPool();
	~idSWFConstantPool() { Clear(); }

	//! Clears all entries from the SWF constant pool
	void			   Clear();

	//! Copies the contents of another constant pool into this one
	void			   Copy( const idSWFConstantPool& other );

	//! Returns the SWF script string at the specified index from the constant pool.
	idSWFScriptString* Get( int n ) { return pool[n]; }

	//! Appends a script string to the constant pool
	void			   Append( idSWFScriptString* s ) { pool.Append( s ); }

private:
	idList<idSWFScriptString*, TAG_SWF> pool;
};

/*!
	\class idSWFStack
	\brief A stack implementation built on top of idList for managing script variables.
*/
class idSWFStack : public idList<idSWFScriptVar>
{
public:
	//! Returns a reference to the top element of the stack
	idSWFScriptVar& A() { return operator[]( Num() - 1 ); }

	//! Returns a reference to the second-to-top element on the stack.
	idSWFScriptVar& B() { return operator[]( Num() - 2 ); }

	//! Returns a reference to the third element from the top of the stack
	idSWFScriptVar& C() { return operator[]( Num() - 3 ); }

	//! Returns a reference to the fourth element from the top of the stack.
	idSWFScriptVar& D() { return operator[]( Num() - 4 ); }

	//! Pops n elements from the stack.
	void			Pop( int n ) { SetNum( Num() - n ); }
};

/*!
	\class idSWFScriptFunction_Script
	\brief Represents a script function implemented in action script with support for execution, debugging, and code export capabilities.

	This class implements a script function that is defined in action script, providing functionality for execution within a SWF environment. It manages script execution context including scope,
   parameters, and registers, while supporting reference counting for memory management. The class handles script function calls, parameter setup, and execution with various flags controlling how
   'this' context and built-in objects are handled. It includes capabilities for building and exporting script actions to string representations, which is useful for debugging and code generation
   purposes. The implementation supports nested action blocks and proper indentation for readable script output. The class maintains a constant pool for script constants and provides mechanisms for
   setting up execution contexts with appropriate scope and variable mappings.

*/
class idSWFScriptFunction_Script : public idSWFScriptFunction
{
public:
	//! Initializes a new instance of the idSWFScriptFunction_Script class with default values.
	idSWFScriptFunction_Script() :
		refCount( 1 ),
		flags( 0 ),
		data( NULL ),
		length( 0 ),
		prototype( NULL ),
		defaultSprite( NULL )
	{
		registers.SetNum( 4 );
	}

	//! Destructor for the idSWFScriptFunction_Script class that releases all referenced objects.
	virtual ~idSWFScriptFunction_Script();

	//! Allocates and returns a new instance of idSWFScriptFunction_Script
	static idSWFScriptFunction_Script* Alloc() { return new( TAG_SWF ) idSWFScriptFunction_Script; }

	//! Increments the reference count for this script function.
	void							   AddRef() { refCount++; }

	//! Decrements the reference count and deletes the object if the count reaches zero.
	void							   Release()
	{
		if( --refCount == 0 ) { delete this; }
	}

	//! Sets the flags for the script function.
	void SetFlags( uint16 _flags ) { flags = _flags; }

	//! Sets the data and length for the script function.
	void SetData( const byte* _data, uint32 _length )
	{
		data   = _data;
		length = _length;
	}

	//! Sets the scope for the script function using the provided list of script objects.
	void SetScope( idList<idSWFScriptObject*>& scope );

	//! Copies the provided constant pool into the script function's constants.
	void SetConstants( const idSWFConstantPool& _constants ) { constants.Copy( _constants ); }

	//! Sets the default sprite for the script function.
	void SetDefaultSprite( idSWFSpriteInstance* _sprite ) { defaultSprite = _sprite; }

	//! Allocates a specified number of registers for the script function.
	void AllocRegisters( int numRegs ) { registers.SetNum( numRegs ); }

	//! Allocates memory for the specified number of script function parameters.
	void AllocParameters( int numParms ) { parameters.SetNum( numParms ); }

	//! Sets the register and name for a script function parameter at the specified index.
	void SetParameter( uint8 n, uint8 r, const char* name )
	{
		parameters[n].reg  = r;
		parameters[n].name = name;
	}

	//! Returns the prototype object associated with this script function.
	idSWFScriptObject* GetPrototype() { return prototype; }

	//! Sets the prototype for this script function object
	void			   SetPrototype( idSWFScriptObject* _prototype )
	{
		_prototype->AddRef();
		assert( prototype == NULL );
		prototype = _prototype;
	}

	//! Executes a script function with the specified object and parameters.
	virtual idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms );

	/*!
		\brief Executes a script function within the SWF environment and returns the result as a string.

		This function processes a script call by setting up the execution context with the provided parameters, managing local variables and scope, and then invoking the script execution. It handles
	   various script execution flags to preload registers with 'this', 'arguments', 'super', '_root', '_parent', and '_global' objects. The function manages the script stack and scope properly,
	   ensuring the correct execution environment is established before calling the script and cleaning up afterward.

		\param thisObject The script object that represents the 'this' context for the function call
		\param parms The list of parameters to pass to the script function
		\param filename The name of the file containing the script definition
		\param characterID The ID of the character associated with the script
		\param actionID The ID of the action being executed
		\return A string containing the result of executing the script function
	*/
	idStr				   CallToScript( idSWFScriptObject* thisObject, const idSWFParmList& parms, const char* filename, int characterID, int actionID );

private:
	//! Executes SWF script actions using the provided stack and bitstream.
	idSWFScriptVar Run( idSWFScriptObject* thisObject, idSWFStack& stack, idSWFBitStream& bitstream );

	struct ActionBlock {
		ActionBlock*		parent;
		idStr				line;
		idList<ActionBlock> blocks;

		//! Initializes a new instance of the ActionBlock class with parent set to NULL.
		ActionBlock() { parent = NULL; }
	};
	idList<ActionBlock> actionBlocks;
	ActionBlock*		currentBlock;

	//! Returns a string containing a specified number of tab characters for indentation.
	idStr				UpdateIndent( int indentLevel ) const;

	//! Adds a line to the current action block, stripping the '_global.' prefix from the line.
	void				AddLine( const idStr& line );

	//! Adds a new action block with the specified line to the current block.
	void				AddBlock( const idStr& line );

	//! Moves the current execution block to its parent block.
	void				QuitCurrentBlock();

	//! Moves the current block pointer to the root block by traversing up the parent chain.
	void				QuitAllBlocks();

	//! Builds action code from a list of action blocks with proper indentation.
	idStr				BuildActionCode( const idList<ActionBlock>& blocks, int level );

	/*!
		\brief Exports SWF script actions to a string representation for a given SWF sprite object

		This function converts SWF script actions from a bitstream into a string representation that can be used for debugging or code generation. It processes the SWF actions, maintains a call stack,
	   and handles various action types such as frame navigation, sound control, and variable operations. The function also includes special handling for certain known problematic functions and
	   provides detailed logging when debugging is enabled. The output string represents the decompiled script actions for a specific sprite and action.

		\param thisObject The SWF script object that contains the script to export
		\param stack The current SWF stack used for script execution and value storage
		\param bitstream The bitstream containing the raw SWF action data
		\param filename The name of the SWF file being processed
		\param characterID The character ID of the SWF sprite being processed
		\param actionID The action ID of the script being processed
		\return The function returns an idStr containing the exported script actions as a string
		\throws The function may throw an assertion error if the record length for certain actions is not as expected, specifically for Action_GotoFrame
	*/
	idStr				ExportToScript( idSWFScriptObject* thisObject, idSWFStack& stack, idSWFBitStream& bitstream, const char* filename, int characterID, int actionID );
	// RB end

private:
	std::atomic<int>					refCount;

	uint16								flags;
	const byte*							data;
	uint32								length;
	idSWFScriptObject*					prototype;

	idSWFSpriteInstance*				defaultSprite; // some actions have an implicit sprite they work off of (e.g. Action_GotoFrame outside of object scope)

	idList<idSWFScriptObject*, TAG_SWF> scope;

	idSWFConstantPool					constants;
	idList<idSWFScriptVar, TAG_SWF>		registers;

	struct parmInfo_t {
		const char* name;
		uint8		reg;
	};
	idList<parmInfo_t, TAG_SWF> parameters;
};

#endif // !__SWF_SCRIPTFUNCTION_H__
