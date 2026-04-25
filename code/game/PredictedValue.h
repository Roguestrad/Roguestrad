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
#ifndef PREDICTED_VALUE_H_
#define PREDICTED_VALUE_H_

// #include "Game_local.h"
#pragma once

/*!
	\class idPredictedValue
	\brief Template class for managing predicted values with snapshot-based updates.

	The idPredictedValue class template is designed to maintain predicted values that can be updated from snapshots while ensuring synchronization with client-side prediction timing. It supports
   initialization with default or specified values, assignment operations, arithmetic operations, and explicit updates from snapshot data. The class tracks prediction time to determine when updates
   from snapshots are valid. The UpdateFromSnapshot method verifies that updates occur only when the snapshot is newer than the current prediction or when it matches the expected client number,
   preventing stale data from overwriting valid predictions. This design enables accurate client-side prediction with server reconciliation.

*/
template<class type_>
class idPredictedValue
{
public:
	//! Initializes an idPredictedValue object with default values.
	explicit idPredictedValue();

	//! Constructs an idPredictedValue object with the specified initial value.
	explicit idPredictedValue( const type_& value_ );

	//! Sets the predicted value and updates the prediction time.
	void					 Set( const type_& newValue );

	//! Assigns a new value to the predicted value object and returns a reference to itself.
	idPredictedValue<type_>& operator=( const type_& value );

	//! Adds the specified value to the current value and returns a reference to this object
	idPredictedValue<type_>& operator+=( const type_& toAdd );

	//! Subtracts the given value from the stored predicted value and returns a reference to this instance.
	idPredictedValue<type_>& operator-=( const type_& toSubtract );

	//! Updates the predicted value from a snapshot if the client number matches or the snapshot is newer than the predicted value.
	bool					 UpdateFromSnapshot( const type_& valueFromSnapshot, int clientNumber );

	//! Returns the stored value of type_
	type_					 Get() const { return value; }

private:
	// Noncopyable
	idPredictedValue( const idPredictedValue<type_>& other );
	idPredictedValue<type_>& operator=( const idPredictedValue<type_>& other );

	type_					 value;
	int						 clientPredictedMilliseconds; // The time in which the client predicted the value.

	//! Updates the prediction time for the predicted value based on the local player's client game milliseconds.
	void					 UpdatePredictionTime();
};

#endif
