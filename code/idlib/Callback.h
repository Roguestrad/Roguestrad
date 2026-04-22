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

/*!
	\class idCallback
	\brief Abstract base class for callback objects.
*/
class idCallback
{
public:
	virtual ~idCallback() { }
	virtual void		Call()		  = 0;
	virtual idCallback* Clone() const = 0;
};

/*!
	\class idCallbackStatic
	\brief A callback class that forwards calls to a C-style function pointer.
*/
class idCallbackStatic : public idCallback
{
public:
	//! Constructs an idCallbackStatic object and initializes its function pointer with the provided function.
	idCallbackStatic( void ( *f )() ) { this->f = f; }

	//! Invokes the stored function callback
	void		Call() { f(); }

	//! Creates a copy of this callback object
	idCallback* Clone() const
	{
		// idScopedGlobalHeap	everythingHereGoesInTheGlobalHeap;
		return new( TAG_FUNC_CALLBACK ) idCallbackStatic( f );
	}

private:
	void ( *f )();
};

//! Constructs a callback binding object that ties a member function to an object instance.
template<class T>
class idCallbackBindMem : public idCallback
{
public:
	//! Constructs a callback binding with a target object and a member function pointer.
	idCallbackBindMem( T* t, void ( T::*f )() )
	{
		this->t = t;
		this->f = f;
	}

	//! Executes the bound member function call.
	void		Call() { ( t->*f )(); }
	idCallback* Clone() const { return new( TAG_FUNC_CALLBACK ) idCallbackBindMem( t, f ); }

private:
	T* t;
	void ( T::*f )();

	//! Executes the bound function call
};

//! Creates a copy of this callback object
template<class T, typename A1>
class idCallbackBindMemArg1 : public idCallback
{
public:
	//! Constructs an idCallbackBindMemArg1 object that binds a member function and its first argument.
	idCallbackBindMemArg1( T* t_, void ( T::*f_ )( A1 ), A1 a1_ ) :
		t( t_ ),
		f( f_ ),
		a1( a1_ )
	{
	}

	//! Constructs a callback binding for a member function with one argument.
	void		Call() { ( t->*f )( a1 ); }

	//! Creates and returns a copy of this callback object
	idCallback* Clone() const { return new( TAG_FUNC_CALLBACK ) idCallbackBindMemArg1( t, f, a1 ); }

private:
	T* t;
	void ( T::*f )( A1 );

	//! Executes a bound method call with a pre-defined argument.
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

//! Creates and returns an idCallbackStatic object from a provided function pointer.
ID_INLINE_EXTERN idCallbackStatic MakeCallback( void ( *f )() )
{
	return idCallbackStatic( f );
}

//! Creates a callback binding for a member function of type T.
template<class T>
ID_INLINE_EXTERN idCallbackBindMem<T> MakeCallback( T* t, void ( T::*f )() )
{
	return idCallbackBindMem<T>( t, f );
}

//! Creates a callback object bound to a member function and its first argument.
template<class T, typename A1>
ID_INLINE_EXTERN idCallbackBindMemArg1<T, A1> MakeCallback( T* t, void ( T::*f )( A1 ), A1 a1 )
{
	return idCallbackBindMemArg1<T, A1>( t, f, a1 );
}

#endif // __CALLBACK_H__
