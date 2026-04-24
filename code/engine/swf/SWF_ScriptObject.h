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
#ifndef __SWF_SCRIPTOBJECT_H__
#define __SWF_SCRIPTOBJECT_H__

class idSWFSpriteInstance;

/*!
	\class idSWFScriptNativeVariable
	\brief Base class for script variables implemented in code.
*/
class idSWFScriptNativeVariable
{
public:
	//! Returns false indicating the variable is not read-only
	virtual bool		   IsReadOnly() { return false; }
	virtual void		   Set( class idSWFScriptObject* object, const idSWFScriptVar& value ) = 0;
	virtual idSWFScriptVar Get( class idSWFScriptObject* object )							   = 0;
};

#define SWF_NATIVE_VAR_DECLARE( x )                                                         \
	class idSWFScriptNativeVar_##x : public idSWFScriptNativeVariable                       \
	{                                                                                       \
	public:                                                                                 \
		void		   Set( class idSWFScriptObject* object, const idSWFScriptVar& value ); \
		idSWFScriptVar Get( class idSWFScriptObject* object );                              \
	} swfScriptVar_##x;

#define SWF_NATIVE_VAR_DECLARE_READONLY( x )                                     \
	class idSWFScriptNativeVar_##x : public idSWFScriptNativeVariable            \
	{                                                                            \
	public:                                                                      \
		bool IsReadOnly()                                                        \
		{                                                                        \
			return true;                                                         \
		}                                                                        \
		void Set( class idSWFScriptObject* object, const idSWFScriptVar& value ) \
		{                                                                        \
			assert( false );                                                     \
		}                                                                        \
		idSWFScriptVar Get( class idSWFScriptObject* object );                   \
	} swfScriptVar_##x;

/*!
	\class idSWFScriptNativeVariable_Nested
	\brief A helper class for managing native variables with access to parent class objects in script environments.
*/
template<typename T>
class idSWFScriptNativeVariable_Nested : public idSWFScriptNativeVariable
{
public:
	//! Initializes a new instance of the idSWFScriptNativeVariable_Nested class with a null pointer.
	idSWFScriptNativeVariable_Nested() :
		pThis( NULL )
	{
	}

	//! Binds a nested class object to the script function.
	idSWFScriptNativeVariable_Nested* Bind( T* p )
	{
		pThis = p;
		return this;
	}
	virtual void		   Set( class idSWFScriptObject* object, const idSWFScriptVar& value ) = 0;
	virtual idSWFScriptVar Get( class idSWFScriptObject* object )							   = 0;

protected:
	T* pThis;
};

#define SWF_NATIVE_VAR_DECLARE_NESTED( x, y )                                               \
	class idSWFScriptNativeVar_##x : public idSWFScriptNativeVariable_Nested<y>             \
	{                                                                                       \
	public:                                                                                 \
		void		   Set( class idSWFScriptObject* object, const idSWFScriptVar& value ); \
		idSWFScriptVar Get( class idSWFScriptObject* object );                              \
	} swfScriptVar_##x;

#define SWF_NATIVE_VAR_DECLARE_NESTED_READONLY( x, y, z )                        \
	class idSWFScriptNativeVar_##x : public idSWFScriptNativeVariable_Nested<y>  \
	{                                                                            \
	public:                                                                      \
		bool IsReadOnly()                                                        \
		{                                                                        \
			return true;                                                         \
		}                                                                        \
		void Set( class idSWFScriptObject* object, const idSWFScriptVar& value ) \
		{                                                                        \
			assert( false );                                                     \
		}                                                                        \
		idSWFScriptVar Get( class idSWFScriptObject* object )                    \
		{                                                                        \
			return pThis->z;                                                     \
		}                                                                        \
	} swfScriptVar_##x;

/*!
	\class idSWFScriptObject
	\brief A container for SWF script variables that supports property access, nesting, and reference counting.

	Represents a script object within the SWF (Shockwave Flash) action script environment, serving as a container for variables and functions. The class maintains a collection of named variables and
   supports hierarchical property access through nested object traversal. It implements reference counting for memory management and provides mechanisms for setting and retrieving various data types
   including sprites, text instances, and generic script variables. The object supports prototype-based inheritance and can be used to represent complex SWF scripting structures with nested
   properties. The class facilitates interaction with SWF elements through direct access methods for sprites, text instances, and native variables, while also supporting dynamic property checking and
   manipulation. Functionality includes allocation, clearing, and console output for debugging purposes.

*/
class idSWFScriptObject
{
public:
	//! Initializes a new instance of the idSWFScriptObject class.
	idSWFScriptObject();

	//! Destructor for idSWFScriptObject that releases the prototype if it exists.
	virtual ~idSWFScriptObject();

	//! Allocates and returns a new instance of idSWFScriptObject.
	static idSWFScriptObject* Alloc();

	//! Increments the reference count of the SWF script object.
	void					  AddRef();

	//! Decrements the reference count and deletes the object if the count reaches zero.
	void					  Release();

	//! Sets the noAutoDelete flag to the specified boolean value.
	void					  SetNoAutoDelete( bool b ) { noAutoDelete = b; }

	//! Clears all variables and resets the hash table for the SWF script object.
	void					  Clear();

	//! Initializes the script object as an array with a length of zero.
	void					  MakeArray();

	//! Sets the sprite object type and data for this script object.
	void					  SetSprite( idSWFSpriteInstance* s )
	{
		objectType	= SWF_OBJECT_SPRITE;
		data.sprite = s;
	}

	//! Returns the sprite instance if the object type is a sprite, otherwise returns NULL.
	idSWFSpriteInstance* GetSprite() { return ( objectType == SWF_OBJECT_SPRITE ) ? data.sprite : NULL; }

	//! Sets the text data for this SWF script object
	void				 SetText( idSWFTextInstance* t )
	{
		objectType = SWF_OBJECT_TEXT;
		data.text  = t;
	}

	//! Returns the text instance if the object type is text, otherwise returns null.
	idSWFTextInstance* GetText() { return ( objectType == SWF_OBJECT_TEXT ) ? data.text : NULL; }

	//! Returns the prototype object associated with this script object.
	idSWFScriptObject* GetPrototype() { return prototype; }

	//! Sets the prototype for this script object
	void			   SetPrototype( idSWFScriptObject* _prototype )
	{
		assert( prototype == NULL );
		prototype = _prototype;
		prototype->AddRef();
	}

	//! Retrieves a script variable from the object at the specified index.
	idSWFScriptVar		 Get( int index );

	//! Retrieves a script variable from the object by name, returning a default variable if not found.
	idSWFScriptVar		 Get( const char* name );

	//! Retrieves a sprite instance from the script object at the specified index.
	idSWFSpriteInstance* GetSprite( int index );

	//! Returns the sprite instance associated with the specified name from the script object
	idSWFSpriteInstance* GetSprite( const char* name );

	//! Returns the object at the specified index in the script object.
	idSWFScriptObject*	 GetObject( int index );

	//! Returns the object associated with the specified name, or NULL if the name does not exist or is not an object.
	idSWFScriptObject*	 GetObject( const char* name );

	//! Retrieves a text instance from the script object at the specified index.
	idSWFTextInstance*	 GetText( int index );

	//! Retrieves a text instance from the script object by name.
	idSWFTextInstance*	 GetText( const char* name );

	//! Sets a value at the specified index in the SWF script object.
	void				 Set( int index, const idSWFScriptVar& value );

	//! Sets a script variable value by name in the SWF script object.
	void				 Set( const char* name, const idSWFScriptVar& value );

	//! Sets a native variable on the SWF script object with the specified name and native variable pointer.
	void				 SetNative( const char* name, idSWFScriptNativeVariable* native );

	//! Checks if a property with the specified name exists in the script object.
	bool				 HasProperty( const char* name );

	//! Checks if a property with the given name exists and has a valid value in the script object.
	bool				 HasValidProperty( const char* name );

	//! Returns the default string representation of the SWF script object, optionally using the toString or valueOf methods
	idSWFScriptVar		 DefaultValue( bool stringHint );

	//! Returns the number of variables in the SWF script object
	int					 NumVariables() { return variables.Num(); }

	//! Returns the name of the variable at the specified index in the script object's variable array.
	const char*			 EnumVariable( int i ) { return variables[i].name; }

	/*!
		\brief Retrieves a nested script variable by traversing a path of object properties specified by the provided argument strings

		This function traverses a chain of object properties using the provided argument strings as keys. It starts from the current object and follows the path defined by the arguments until it
	   reaches the last element. If any element in the path is not an object or if the path is invalid, it returns an empty script variable. The function accepts up to six arguments representing the
	   path components, with trailing NULL arguments being ignored

		\param arg1 First property name in the path
		\param arg2 Second property name in the path
		\param arg3 Third property name in the path
		\param arg4 Fourth property name in the path
		\param arg5 Fifth property name in the path
		\param arg6 Sixth property name in the path
		\return The script variable found at the end of the specified path, or an empty script variable if the path is invalid or leads to a non-object
	*/
	idSWFScriptVar		 GetNestedVar( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	/*!
		\brief Retrieves a nested script object from this object using up to six nested property names.

		This function traverses nested properties of the script object using the provided property names. It first retrieves a script variable using the GetNestedVar method with the same parameters,
	   then checks if the retrieved variable is an object. If it is not an object, the function returns NULL. Otherwise, it returns the script object associated with the variable.

		\param arg1 First property name to traverse
		\param arg2 Second property name to traverse, optional
		\param arg3 Third property name to traverse, optional
		\param arg4 Fourth property name to traverse, optional
		\param arg5 Fifth property name to traverse, optional
		\param arg6 Sixth property name to traverse, optional
		\return Pointer to the nested script object if all nested properties exist and the final result is an object, otherwise NULL
	*/
	idSWFScriptObject*	 GetNestedObj( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	/*!
		\brief Retrieves a nested sprite instance from the SWF script object using a variable path.

		This function accesses a nested sprite instance by resolving a path specified by up to six string arguments. It first retrieves a script variable using the GetNestedVar method and then
	   converts that variable to a sprite instance. The function is designed to work with the SWF (Shockwave Flash) scripting system and is part of the RogueStrad engine's implementation of SWF
	   functionality. The arguments represent levels of nesting in the SWF object hierarchy, with null values indicating optional path components. If the path resolution fails or the resulting
	   variable is not a sprite, the function returns NULL.

		\param arg1 First component of the path to the nested sprite
		\param arg2 Second component of the path to the nested sprite
		\param arg3 Third component of the path to the nested sprite
		\param arg4 Fourth component of the path to the nested sprite
		\param arg5 Fifth component of the path to the nested sprite
		\param arg6 Sixth component of the path to the nested sprite
		\return Pointer to the idSWFSpriteInstance if the path is valid and resolves to a sprite, otherwise NULL
	*/
	idSWFSpriteInstance* GetNestedSprite( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	/*!
		\brief Retrieves a nested text instance from a SWF script object using a chain of property names

		This function navigates through a hierarchy of SWF script variables using a sequence of property names to locate and return a specific text instance. It internally calls GetNestedVar to
	   traverse the object hierarchy and then converts the resulting variable to a text instance. The function accepts up to six parameter names that specify the path through the object structure.

		\param arg1 First property name in the hierarchy path
		\param arg2 Second property name in the hierarchy path
		\param arg3 Third property name in the hierarchy path
		\param arg4 Fourth property name in the hierarchy path
		\param arg5 Fifth property name in the hierarchy path
		\param arg6 Sixth property name in the hierarchy path
		\return A pointer to the located idSWFTextInstance if found, or NULL if the path does not exist or the final object is not a text instance
	*/
	idSWFTextInstance*	 GetNestedText( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL, const char* arg6 = NULL );

	//! Prints all script object variables to the console in a formatted table.
	void				 PrintToConsole() const;

private:
	std::atomic<int> refCount;
	bool			 noAutoDelete;

	enum swfNamedVarFlags_t { SWF_VAR_FLAG_NONE = 0, SWF_VAR_FLAG_READONLY = BIT( 1 ), SWF_VAR_FLAG_DONTENUM = BIT( 2 ) };
	struct swfNamedVar_t {
		//! Initializes a new instance of the swfNamedVar_t structure with default values.
		swfNamedVar_t() :
			native( NULL )
		{
		}
		~swfNamedVar_t();

		//! Assignment operator for swfNamedVar_t that copies data from another instance
		swfNamedVar_t&			   operator=( const swfNamedVar_t& other );

		int						   index;
		int						   hashNext;
		idStr					   name;
		idSWFScriptVar			   value;
		idSWFScriptNativeVariable* native;
		int						   flags;
	};
	idList<swfNamedVar_t, TAG_SWF> variables;

	static const int			   VARIABLE_HASH_BUCKETS = 16;
	int							   variablesHash[VARIABLE_HASH_BUCKETS];

	idSWFScriptObject*			   prototype;

	enum swfObjectType_t { SWF_OBJECT_OBJECT, SWF_OBJECT_ARRAY, SWF_OBJECT_SPRITE, SWF_OBJECT_TEXT } objectType;

	union swfObjectData_t {
		idSWFSpriteInstance* sprite; // only valid if objectType == SWF_OBJECT_SPRITE
		idSWFTextInstance*	 text;	 // only valid if objectType == SWF_OBJECT_TEXT
	} data;

	//! Retrieves or creates a script variable by its index from the SWF script object
	swfNamedVar_t* GetVariable( int index, bool create );

	//! Retrieves a script variable by name from the object, creating it if it doesn't exist and the create flag is true.
	swfNamedVar_t* GetVariable( const char* name, bool create );
};

#endif // !__SWF_SCRIPTOBJECT_H__
