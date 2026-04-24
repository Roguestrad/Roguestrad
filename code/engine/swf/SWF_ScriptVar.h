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
#ifndef __SWF_SCRIPTVAR_H__
#define __SWF_SCRIPTVAR_H__

class idSWFScriptObject;
class idSWFScriptFunction;

/*!
	\class idSWFScriptString
	\brief A reference counted string class derived from idStr.
*/
class idSWFScriptString : public idStr
{
public:
	//! Initializes a new idSWFScriptString with a copy of the provided idStr.
	idSWFScriptString( const idStr& s ) :
		idStr( s ),
		refCount( 1 )
	{
	}

	//! Allocates a new idSWFScriptString object initialized with the provided string value.
	static idSWFScriptString* Alloc( const idStr& s ) { return new( TAG_SWF ) idSWFScriptString( s ); }

	//! Increments the reference count of the SWF script string.
	ID_INLINE void			  AddRef() { refCount++; }

	//! Releases the SWF script string by decrementing its reference count and deleting it if the count reaches zero.
	ID_INLINE void			  Release()
	{
		if( --refCount == 0 ) { delete this; }
	}

private:
	std::atomic<int> refCount;
};

/*!
	\class idSWFScriptVar
	\brief A variable type that can hold different SWF script data types including numbers, strings, objects, and functions.

	This class represents a variable in a scripting environment that can store various data types such as integers, floats, strings, objects, and functions. It supports copy construction and
   assignment operations with proper reference counting for complex types. The class provides methods to convert between different types and to access nested properties within objects. It includes
   functionality for comparing variables according to ECMA 262 specifications and offers debugging capabilities through console output. The implementation handles memory management for referenced
   types like strings, objects, and functions through appropriate reference counting mechanisms.

*/
class idSWFScriptVar
{
public:
	//! Constructs a new SWF script variable with undefined type.
	idSWFScriptVar() :
		type( SWF_VAR_UNDEF )
	{
	}

	//! Creates a new SWF script variable as a copy of an existing one, handling reference counting for string, object, and function types.
	idSWFScriptVar( const idSWFScriptVar& other );

	//! Initializes a SWF script variable with the specified script object.
	idSWFScriptVar( idSWFScriptObject* o ) :
		type( SWF_VAR_UNDEF )
	{
		SetObject( o );
	}

	//! Constructs an idSWFScriptVar object with a string identifier.
	idSWFScriptVar( idStrId s ) :
		type( SWF_VAR_UNDEF )
	{
		SetString( s );
	}

	//! Constructs an idSWFScriptVar object and initializes its type to SWF_VAR_UNDEF before setting its string value.
	idSWFScriptVar( const idStr& s ) :
		type( SWF_VAR_UNDEF )
	{
		SetString( s );
	}

	//! Constructs an idSWFScriptVar object from a C-style string
	idSWFScriptVar( const char* s ) :
		type( SWF_VAR_UNDEF )
	{
		SetString( idStr( s ) );
	}

	//! Constructs an idSWFScriptVar object with a float value.
	idSWFScriptVar( float f ) :
		type( SWF_VAR_UNDEF )
	{
		SetFloat( f );
	}

	//! Constructs an idSWFScriptVar object and initializes it with a boolean value.
	idSWFScriptVar( bool b ) :
		type( SWF_VAR_UNDEF )
	{
		SetBool( b );
	}

	//! Initializes a SWF script variable with an integer value.
	idSWFScriptVar( int32 i ) :
		type( SWF_VAR_UNDEF )
	{
		SetInteger( i );
	}

	//! Constructs an idSWFScriptVar object with the specified script function.
	idSWFScriptVar( idSWFScriptFunction* nf ) :
		type( SWF_VAR_UNDEF )
	{
		SetFunction( nf );
	}

	//! Destructor for idSWFScriptVar that releases allocated resources.
	~idSWFScriptVar();

	//! Assigns the value of another SWF script variable to this variable.
	idSWFScriptVar& operator=( const idSWFScriptVar& other );

	//! Compares this SWF script variable with another for equality according to ECMA 262 11.9.3 specification
	bool			AbstractEquals( const idSWFScriptVar& other );

	//! Compares two SWF script variables for strict equality based on type and value.
	bool			StrictEquals( const idSWFScriptVar& other );

	//! Sets the script variable to hold a string identifier value.
	void			SetString( idStrId s )
	{
		Free();
		type	= SWF_VAR_STRINGID;
		value.i = s.GetIndex();
	}

	//! Sets the script variable's value to a copy of the provided string
	void SetString( const idStr& s )
	{
		Free();
		type		 = SWF_VAR_STRING;
		value.string = idSWFScriptString::Alloc( s );
	}

	//! Sets the script variable to hold a string value.
	void SetString( const char* s )
	{
		Free();
		type		 = SWF_VAR_STRING;
		value.string = idSWFScriptString::Alloc( s );
	}

	//! Sets the script variable to hold a string value and increments the string's reference count.
	void SetString( idSWFScriptString* s )
	{
		Free();
		type		 = SWF_VAR_STRING;
		value.string = s;
		s->AddRef();
	}

	//! Sets the result of the script variable to the provided string value.
	void SetResult( const idStr& s )
	{
		Free();
		type		 = SWF_VAR_RESULT;
		value.string = idSWFScriptString::Alloc( s );
	}

	//! Sets the result of the SWF script variable to the provided string value.
	void SetResult( const char* s )
	{
		Free();
		type		 = SWF_VAR_RESULT;
		value.string = idSWFScriptString::Alloc( s );
	}

	//! Sets the value of the SWF script variable to a float.
	void SetFloat( float f )
	{
		Free();
		type	= SWF_VAR_FLOAT;
		value.f = f;
	}

	//! Sets the script variable to a null type.
	void SetNULL()
	{
		Free();
		type = SWF_VAR_NULL;
	}

	//! Sets the script variable to an undefined state.
	void SetUndefined()
	{
		Free();
		type = SWF_VAR_UNDEF;
	}

	//! Sets the boolean value of the script variable.
	void SetBool( bool b )
	{
		Free();
		type	= SWF_VAR_BOOL;
		value.b = b;
	}

	//! Sets the script variable value to an integer.
	void SetInteger( int32 i )
	{
		Free();
		type	= SWF_VAR_INTEGER;
		value.i = i;
	}

	//! Sets the object value of this script variable, managing reference counting.
	void			   SetObject( idSWFScriptObject* o );

	//! Sets the function value of the script variable, managing reference counting and type assignment.
	void			   SetFunction( idSWFScriptFunction* f );

	//! Converts the script variable to its string representation based on its type.
	idStr			   ToString() const;

	//! Converts the script variable to a floating-point value.
	float			   ToFloat() const;

	//! Converts the script variable to a boolean value based on its type and content.
	bool			   ToBool() const;

	//! Converts the SWF script variable to an integer value
	int32			   ToInteger() const;

	//! Returns the SWF script object stored in this script variable.
	idSWFScriptObject* GetObject()
	{
		assert( type == SWF_VAR_OBJECT );
		return value.object;
	}

	//! Returns the SWF script object stored in this script variable.
	idSWFScriptObject* GetObject() const
	{
		assert( type == SWF_VAR_OBJECT );
		return value.object;
	}

	//! Retrieves the function object when the script variable type is SWF_VAR_FUNCTION
	idSWFScriptFunction* GetFunction()
	{
		assert( type == SWF_VAR_FUNCTION );
		return value.function;
	}

	//! Converts a script variable to a sprite instance if it is an object
	idSWFSpriteInstance* ToSprite();

	//! Converts the script variable to a text instance if it is an object, otherwise returns null.
	idSWFTextInstance*	 ToText();

	/*!
		\brief Retrieves a nested script variable by traversing a sequence of property names

		This function accesses a nested variable within a script object by following a path specified by the provided property names. It first checks if the current object is valid and returns an
	   empty script variable if it is not. Otherwise, it delegates the retrieval to the underlying object's GetNestedVar method, which performs the actual traversal of the nested structure. The
	   function accepts up to six property name arguments to specify the path to the desired variable.

		\param arg1 First property name in the nested path
		\param arg2 Second property name in the nested path
		\param arg3 Third property name in the nested path
		\param arg4 Fourth property name in the nested path
		\param arg5 Fifth property name in the nested path
		\param arg6 Sixth property name in the nested path
		\return The script variable at the specified nested path, or an empty script variable if the object is not valid or the path is invalid
	*/
	idSWFScriptVar		 GetNestedVar( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	/*!
		\brief Retrieves a nested SWF script object from this script variable using up to six property names.

		This function attempts to retrieve a nested object from the SWF script variable by traversing a property path specified by up to six string arguments. It first checks if the current script
	   variable holds an object. If not, it returns NULL immediately. Otherwise, it delegates the lookup to the underlying object's GetNestedObj method, which performs the actual traversal of the
	   nested properties.

		\param arg1 First property name in the path to traverse.
		\param arg2 Second property name in the path to traverse, or NULL if not used.
		\param arg3 Third property name in the path to traverse, or NULL if not used.
		\param arg4 Fourth property name in the path to traverse, or NULL if not used.
		\param arg5 Fifth property name in the path to traverse, or NULL if not used.
		\param arg6 Sixth property name in the path to traverse, or NULL if not used.
		\return A pointer to the found SWF script object, or NULL if the current variable is not an object or the nested path could not be resolved.
	*/
	idSWFScriptObject*	 GetNestedObj( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	/*!
		\brief Retrieves a nested sprite instance from a SWF script object using up to six hierarchical property names.

		This function attempts to access a nested sprite instance within a SWF script object by traversing a hierarchy of properties specified by the given argument strings. It first checks if the
	   current script variable holds an object type, returning NULL if it does not. If the variable is of object type, it delegates the actual lookup to the underlying object's GetNestedSprite method,
	   which handles the traversal of the property hierarchy. The function supports up to six levels of nesting, with each argument representing a level in the hierarchy. If any argument is NULL, it
	   is treated as a missing level in the hierarchy.

		\param arg1 First level property name in the hierarchy
		\param arg2 Second level property name in the hierarchy
		\param arg3 Third level property name in the hierarchy
		\param arg4 Fourth level property name in the hierarchy
		\param arg5 Fifth level property name in the hierarchy
		\param arg6 Sixth level property name in the hierarchy
		\return A pointer to the found idSWFSpriteInstance if the object is valid and the sprite is found, otherwise NULL.
	*/
	idSWFSpriteInstance* GetNestedSprite( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	/*!
		\brief Retrieves a nested text instance from a SWF script variable by traversing a path of object names.

		This function attempts to access a nested text instance within a SWF script variable by providing a sequence of object names. It first checks if the current script variable represents an
	   object. If not, it returns NULL. Otherwise, it delegates the operation to the underlying object's GetNestedText method, which performs the actual traversal and retrieval of the text instance.

		\param arg1 First name in the path to the nested text instance.
		\param arg2 Second name in the path to the nested text instance.
		\param arg3 Third name in the path to the nested text instance.
		\param arg4 Fourth name in the path to the nested text instance.
		\param arg5 Fifth name in the path to the nested text instance.
		\param arg6 Sixth name in the path to the nested text instance.
		\return A pointer to the retrieved idSWFTextInstance if successful, or NULL if the current script variable is not an object or the path does not lead to a valid text instance.
	*/
	idSWFTextInstance*	 GetNestedText( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	//! Returns the type of the script variable as a string.
	const char*			 TypeOf() const;

	//! Prints the value of this script variable to the console for debugging purposes.
	void				 PrintToConsole() const;

	//! Returns true if the script variable contains a string or string ID value.
	bool				 IsString() const { return ( type == SWF_VAR_STRING ) || ( type == SWF_VAR_STRINGID ); }

	//! Checks if the script variable is of NULL type
	bool				 IsNULL() const { return ( type == SWF_VAR_NULL ); }

	//! Checks if the script variable is undefined.
	bool				 IsUndefined() const { return ( type == SWF_VAR_UNDEF ); }

	//! Checks if the SWF script variable has a valid type and is not undefined or null.
	bool				 IsValid() const { return ( type != SWF_VAR_UNDEF ) && ( type != SWF_VAR_NULL ); }

	//! Returns true if the script variable represents a function
	bool				 IsFunction() const { return ( type == SWF_VAR_FUNCTION ); }

	//! Returns true if the script variable is of object type.
	bool				 IsObject() const { return ( type == SWF_VAR_OBJECT ); }

	//! Returns true if the script variable contains a numeric value.
	bool				 IsNumeric() const { return ( type == SWF_VAR_FLOAT ) || ( type == SWF_VAR_INTEGER ) || ( type == SWF_VAR_BOOL ); }

	//! Checks if the script variable represents a result type.
	bool				 IsResult() const { return ( type == SWF_VAR_RESULT ); }

	enum swfScriptVarType {
		SWF_VAR_STRINGID,
		SWF_VAR_STRING,
		SWF_VAR_FLOAT,
		SWF_VAR_NULL,
		SWF_VAR_UNDEF,
		SWF_VAR_BOOL,
		SWF_VAR_INTEGER,
		SWF_VAR_FUNCTION,
		SWF_VAR_OBJECT,
		SWF_VAR_RESULT // RB: for P-Code to Lua
	};

	//! Returns the type of the SWF script variable
	swfScriptVarType GetType() const { return type; }

private:
	//! Releases resources held by the SWF script variable based on its type
	void			 Free();
	swfScriptVarType type;

	union swfScriptVarValue_t {
		float				 f;
		int32				 i;
		bool				 b;
		idSWFScriptObject*	 object;
		idSWFScriptString*	 string;
		idSWFScriptFunction* function;
	} value;
};

#endif // !__SWF_SCRIPTVAR_H__
