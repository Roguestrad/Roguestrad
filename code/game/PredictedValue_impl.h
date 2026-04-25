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
#ifndef PREDICTED_VALUE_IMPL_H_
#define PREDICTED_VALUE_IMPL_H_

#include "PredictedValue.h"
#include "player/Player.h"

template<class type_>
idPredictedValue<type_>::idPredictedValue() :
	value(),
	clientPredictedMilliseconds( 0 )
{
}

template<class type_>
idPredictedValue<type_>::idPredictedValue( const type_& value_ ) :
	value( value_ ),
	clientPredictedMilliseconds( 0 )
{
}

template<class type_>
void idPredictedValue<type_>::UpdatePredictionTime()
{
	if( gameLocal.GetLocalPlayer() != NULL ) { clientPredictedMilliseconds = gameLocal.GetLocalPlayer()->usercmd.clientGameMilliseconds; }
}

template<class type_>
void idPredictedValue<type_>::Set( const type_& newValue )
{
	value = newValue;
	UpdatePredictionTime();
}

template<class type_>
idPredictedValue<type_>& idPredictedValue<type_>::operator=( const type_& newValue )
{
	Set( newValue );
	return *this;
}

template<class type_>
idPredictedValue<type_>& idPredictedValue<type_>::operator+=( const type_& toAdd )
{
	Set( value + toAdd );
	return *this;
}

template<class type_>
idPredictedValue<type_>& idPredictedValue<type_>::operator-=( const type_& toSubtract )
{
	Set( value - toSubtract );
	return *this;
}

template<class type_>
bool idPredictedValue<type_>::UpdateFromSnapshot( const type_& valueFromSnapshot, int clientNumber )
{
	if( clientNumber != gameLocal.GetLocalClientNum() ) {
		value = valueFromSnapshot;
		return true;
	}

	if( gameLocal.GetLastClientUsercmdMilliseconds( clientNumber ) >= clientPredictedMilliseconds ) {
		value = valueFromSnapshot;
		return true;
	}

	return false;
}

//! Compares two idPredictedValue objects for equality based on their stored values regardless of frame numbers.
template<class firstType_, class secondType_>
bool operator==( const idPredictedValue<firstType_>& lhs, const idPredictedValue<secondType_>& rhs )
{
	return lhs.Get() == rhs.Get();
}

//! Checks if two idPredictedValue objects are not equal by comparing their stored values.
template<class firstType_, class secondType_>
bool operator!=( const idPredictedValue<firstType_>& lhs, const idPredictedValue<secondType_>& rhs )
{
	return lhs.Get() != rhs.Get();
}

//! Compares a predicted value with another value for equality.
template<class firstType_, class secondType_>
bool operator==( const idPredictedValue<firstType_>& lhs, const secondType_& rhs )
{
	return lhs.Get() == rhs;
}

//! Overloads the equality operator for comparing a value with an idPredictedValue instance
template<class firstType_, class secondType_>
bool operator==( const firstType_ lhs, const idPredictedValue<secondType_>& rhs )
{
	return lhs == rhs.Get();
}

//! Checks if an idPredictedValue and a value are not equal.
template<class firstType_, class secondType_>
bool operator!=( const idPredictedValue<firstType_>& lhs, const secondType_& rhs )
{
	return lhs.Get() != rhs;
}

//! Overloads the not equal operator for comparing a firstType_ value with an idPredictedValue<secondType_> instance.
template<class firstType_, class secondType_>
bool operator!=( const firstType_ lhs, const idPredictedValue<secondType_>& rhs )
{
	return lhs != rhs.Get();
}

#endif
