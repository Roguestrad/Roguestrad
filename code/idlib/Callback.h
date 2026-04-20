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
#ifndef __CALLBACK_H__
#define __CALLBACK_H__

/*
================================================================================================
This file defines a set of template functors for generating callbacks, specifically
the OnChange handlers in the CVar system.
================================================================================================
*/

/*
================================================
idCallback
================================================
*/
class idCallback
{
public:
	virtual ~idCallback() { }
	virtual void		Call()		  = 0;
	virtual idCallback* Clone() const = 0;
};

/*
================================================
idCallbackStatic

Callback class that forwards the call to a c-style function
================================================
*/
class idCallbackStatic : public idCallback
{
public:
	/*!
		\brief Initializes the callback object with the provided function pointer.

		This constructor initializes the callback object by storing the provided function pointer for later execution. The stored function pointer can be invoked through other methods of the class to
	   execute the callback.

		\param f The function pointer to be stored and invoked later.
	*/
	idCallbackStatic( void ( *f )() ) { this->f = f; }

	/*!
		\brief Calls the stored function callback.

		This function invokes the function pointer that was previously stored in the callback object. It is typically used in scripting systems to execute registered callbacks.

	*/
	void		Call() { f(); }

	/*!
		\brief Creates and returns a copy of this callback object

		This method implements a virtual clone pattern for callback objects. It allocates memory for a new instance of the same type and initializes it with the function pointer from the current
	   object. The clone is allocated on the global heap using a specific memory tag for callbacks. This method is typically used when a callback object needs to be duplicated for use in different
	   contexts or when the original object needs to be preserved while making modifications.

		\return A pointer to a new callback object of the same type as this object, initialized with the same function pointer
	*/
	idCallback* Clone() const
	{
		// idScopedGlobalHeap	everythingHereGoesInTheGlobalHeap;
		return new( TAG_FUNC_CALLBACK ) idCallbackStatic( f );
	}

private:
	void ( *f )();
};

/*!
	\brief Creates a copy of this callback object

	This function is used to create a deep copy of a callback object. It allocates memory for a new callback object of the same type and initializes it with the same parameters as the original object.
   This is useful for scenarios where a callback needs to be stored or passed around while maintaining its functionality

	\return A pointer to the newly created callback object
*/
template<class T>
class idCallbackBindMem : public idCallback
{
public:
	/*!
		\brief Constructs an idCallbackBindMem object that binds a member function to an object instance

		This constructor initializes a callback binding by storing a pointer to an object instance and a pointer to a member function. The stored member function will be invoked on the stored object
	   instance when the callback is executed. The template type T represents the class type that contains the member function.

		\param t Pointer to the object instance on which the member function will be called
		\param f Pointer to the member function to be bound
	*/
	idCallbackBindMem( T* t, void ( T::*f )() )
	{
		this->t = t;
		this->f = f;
	}

	/*!
		\brief Executes the bound callback function

		This function invokes the callback method that was previously bound to a specific object and function pointer. It is designed to be called as part of a callback mechanism where the function to
	   be executed is stored within the object and executed later.

	*/
	void		Call() { ( t->*f )(); }

	/*!
		\brief Creates and returns a new copy of this callback object

		This function implements the cloning mechanism for callback objects. It allocates memory for a new instance of the same callback type and initializes it with the stored object instance and
	   member function pointer. The newly created callback is meant to be used as a independent copy that maintains the same behavior as the original. The clone operation is typically used when
	   callbacks need to be stored or passed around while preserving their functionality.

		\return A pointer to a newly allocated callback object that is a copy of this instance
	*/
	idCallback* Clone() const { return new( TAG_FUNC_CALLBACK ) idCallbackBindMem( t, f ); }

private:
	T* t;
	void ( T::*f )();
};

/*!
	\brief Creates a copy of this callback object

	This method implements the cloning functionality for callback objects. It allocates memory for a new callback instance using the specified memory tag and constructs a new idCallbackBindMemArg1
   object with the same parameters as the current instance. The clone method is typically used when callbacks need to be copied or stored for later execution, allowing the callback to be invoked
   multiple times with the same parameters.

	\return A pointer to the newly created callback object that is a copy of this instance
*/
template<class T, typename A1>
class idCallbackBindMemArg1 : public idCallback
{
public:
	/*!
		\brief Constructs a callback binding a member function with one argument to an object instance

		This constructor initializes a callback object that binds a member function of a class to a specific object instance and its first argument. The member function is stored along with the object
	   instance and the argument value, allowing the callback to be invoked later with the stored values. The template parameters T and A1 represent the class type and the argument type respectively.

		\param t_ Pointer to the object instance on which the member function will be called
		\param f_ Pointer to the member function to be bound
		\param a1_ The first argument value to be bound to the member function
	*/
	idCallbackBindMemArg1( T* t_, void ( T::*f_ )( A1 ), A1 a1_ ) :
		t( t_ ),
		f( f_ ),
		a1( a1_ )
	{
	}

	/*!
		\brief Executes a stored member function call with a pre-bound argument

		This function performs a member function call using a stored object pointer and function pointer that were previously bound with the template arguments. The function is called with a single
	   pre-bound argument of type A1 that was provided during the binding process. The function uses a pointer-to-member function syntax to invoke the bound method on the stored object.

	*/
	void		Call() { ( t->*f )( a1 ); }

	/*!
		\brief Creates a copy of this callback object

		This function implements a clone pattern for callback objects, allowing for deep copying of callback instances. It allocates memory for a new callback object using the TAG_FUNC_CALLBACK memory
	   tag and constructs a new idCallbackBindMemArg1 object with the same parameters as the current object. This enables the callback to be safely copied and used in different contexts, such as when
	   storing callbacks in containers or passing them around in the system.

		\return A pointer to a newly allocated copy of this callback object
	*/
	idCallback* Clone() const { return new( TAG_FUNC_CALLBACK ) idCallbackBindMemArg1( t, f, a1 ); }

private:
	T* t;
	void ( T::*f )( A1 );
	A1 a1;

	// hack to get to compile on the 360 with reference arguments
	// with this on the PC, the MakeCallback function fails compilation because it's returning a copy
	// therefore, the Arg1 callbacks can't have arguments that are references
	// DISALLOW_COPY_AND_ASSIGN( idCallbackBindMemArg1 );
};

/*
================================================================================================

	These are needed because we can't derive the type of an object from the type passed to the
	constructor. If it weren't for these, we'd have to manually specify the type:

		idCallbackBindMem<idFoo>( this, &idFoo::MyFunction );
	becomes:
		MakeCallback( this, &idFoo::MyFunction );

================================================================================================
*/

//! Creates and returns an idCallbackStatic object initialized with the provided function pointer.
ID_INLINE_EXTERN idCallbackStatic MakeCallback( void ( *f )() )
{
	return idCallbackStatic( f );
}

/*!
	\brief Creates a callback object bound to a member function of the given object

	The MakeCallback function is a utility that constructs an idCallbackBindMem object, which binds a member function of a given object to that object instance. This allows for easy creation of
   callback objects that can be used to invoke member functions on objects without requiring explicit knowledge of the object's type at the call site. The returned callback object encapsulates both
   the object instance and the member function pointer, providing a clean way to defer execution of member functions.

	\param t Pointer to the object instance that the member function will be bound to
	\param f Pointer to the member function that will be invoked on the object
	\return A callback object of type idCallbackBindMem<T> that encapsulates the object instance and member function pointer
*/
template<class T>
ID_INLINE_EXTERN idCallbackBindMem<T> MakeCallback( T* t, void ( T::*f )() )
{
	return idCallbackBindMem<T>( t, f );
}

/*!
	\brief Creates and returns a callback object that binds a member function and its first argument to an object instance

	This function is a utility for creating callback objects that encapsulate a member function pointer along with an object instance and a single argument. The returned callback object can be used to
   invoke the bound member function with the specified argument at a later time. It is typically used in event handling or asynchronous operation scenarios where a function needs to be called with
   specific parameters.

	\param t Pointer to the object instance on which the member function will be called
	\param f Pointer to the member function to be invoked
	\param a1 First argument to be bound to the member function
	\return A callback object of type idCallbackBindMemArg1 that holds the object instance, member function pointer, and bound argument
*/
template<class T, typename A1>
ID_INLINE_EXTERN idCallbackBindMemArg1<T, A1> MakeCallback( T* t, void ( T::*f )( A1 ), A1 a1 )
{
	return idCallbackBindMemArg1<T, A1>( t, f, a1 );
}

#endif // __CALLBACK_H__
