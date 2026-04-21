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

#ifndef __MATH_EXTRAPOLATE_H__
#define __MATH_EXTRAPOLATE_H__

/*
==============================================================================================

	Extrapolation

==============================================================================================
*/

typedef enum {
	EXTRAPOLATION_NONE		  = 0x01, // no extrapolation, covered distance = duration * 0.001 * ( baseSpeed )
	EXTRAPOLATION_LINEAR	  = 0x02, // linear extrapolation, covered distance = duration * 0.001 * ( baseSpeed + speed )
	EXTRAPOLATION_ACCELLINEAR = 0x04, // linear acceleration, covered distance = duration * 0.001 * ( baseSpeed + 0.5 * speed )
	EXTRAPOLATION_DECELLINEAR = 0x08, // linear deceleration, covered distance = duration * 0.001 * ( baseSpeed + 0.5 * speed )
	EXTRAPOLATION_ACCELSINE	  = 0x10, // sinusoidal acceleration, covered distance = duration * 0.001 * ( baseSpeed + sqrt( 0.5 ) * speed )
	EXTRAPOLATION_DECELSINE	  = 0x20, // sinusoidal deceleration, covered distance = duration * 0.001 * ( baseSpeed + sqrt( 0.5 ) * speed )
	EXTRAPOLATION_NOSTOP	  = 0x40  // do not stop at startTime + duration
} extrapolation_t;

/*!
	\class idExtrapolate
	\brief A template class for managing extrapolation of animated values with various interpolation types.

	The idExtrapolate class provides a flexible mechanism for animating values over time using different extrapolation methods. It supports linear and sine-based interpolation with acceleration and
   deceleration curves, making it suitable for smooth UI animations and cinematic effects. The class maintains internal state for start time, duration, initial value, and speed parameters to control
   the extrapolation behavior. It offers methods to query current values and speeds at specific time points, check if the extrapolation has completed, and modify key parameters such as start time and
   initial value. The template design allows it to work with various numeric types, enabling its use in different animation contexts throughout the engine.

*/
template<class type>
class idExtrapolate
{
public:
	/*!
		\brief Initializes an extrapolation object with default values

		Constructs an idExtrapolate object and initializes all internal parameters to their default states. The extrapolation type is set to EXTRAPOLATION_NONE, and all time-related and value-related
	   parameters are cleared to zero. This ensures the object is in a valid, uninitialized state ready for subsequent configuration

	*/
	idExtrapolate();

	/*!
		\brief Initializes the extrapolation parameters for the given start time, duration, values, and speed.

		This function sets up the internal state of the extrapolation object with the provided parameters. It configures the extrapolation type, start time, duration, initial value, base speed, and
	   current speed. The extrapolation type determines how the interpolation will behave, and the other parameters define the specific characteristics of the extrapolation curve.

		\param startTime The starting time for the extrapolation
		\param duration The total duration of the extrapolation
		\param startValue The initial value at the start time
		\param baseSpeed The base speed used for extrapolation calculations
		\param speed The current speed used for extrapolation calculations
		\param extrapolationType The type of extrapolation to perform
	*/
	void			Init( const int startTime, const int duration, const type& startValue, const type& baseSpeed, const type& speed, const extrapolation_t extrapolationType );

	/*!
		\brief Returns the interpolated value at the specified time based on the extrapolation configuration and type.

		This function computes an interpolated value for a given time based on the extrapolation type and parameters configured for the object. It handles multiple extrapolation types including
	   linear, acceleration and deceleration variants with both linear and sine curves. The function clamps the time to the duration if the extrapolation type does not allow stopping, and returns the
	   start value if the time is before the start time. The interpolation is performed using different mathematical formulas depending on the extrapolation type, with special handling for zero
	   duration cases.

		\param time The absolute time value for which to compute the interpolated value.
		\return The interpolated value at the given time based on the extrapolation configuration and type.
	*/
	type			GetCurrentValue( int time ) const;

	/*!
		\brief Returns the current speed at the given time based on the extrapolation type and parameters

		Checks if the extrapolation is complete based on the provided time value and returns the appropriate speed based on the extrapolation type. The function handles various extrapolation types
	   including linear, accelerated linear, decelerated linear, accelerated sine, and decelerated sine. If the time is outside the valid range or if the extrapolation is complete, it returns zero
	   speed. For valid times, it calculates the speed based on the specific extrapolation curve.

		\param time The time value for which to calculate the current speed
		\return The calculated speed at the given time based on the extrapolation type and parameters
	*/
	type			GetCurrentSpeed( int time ) const;

	/*!
		\brief Checks if the extrapolation is complete based on the provided time value

		This function determines whether an extrapolation process has finished by comparing the given time value against the start time plus duration. The check is skipped if the extrapolation type
	   has the EXTRAPOLATION_NOSTOP flag set. It is commonly used in UI animation systems to manage transition states and timing

		\param time The absolute time value to check against the extrapolation completion condition
		\return True if the extrapolation is complete and the time has exceeded the start time plus duration, false otherwise
	*/
	bool			IsDone( int time ) const { return ( !( extrapolationType & EXTRAPOLATION_NOSTOP ) && time >= startTime + duration ); }

	/*!
		\brief Sets the start time for the extrapolation.

		This function assigns the provided time value to the internal startTime member variable, which is used to track when the extrapolation process begins. The start time is typically used in
	   cinematic animations to control when effects or movements should commence.

		\param time The time value to set as the start time for extrapolation
	*/
	void			SetStartTime( int time ) { startTime = time; }

	//! Returns the start time of the cinematic animation.
	int				GetStartTime() const { return startTime; }

	//! Returns the end time of the extrapolation interval.
	int				GetEndTime() const { return ( !( extrapolationType & EXTRAPOLATION_NOSTOP ) && duration > 0 ) ? startTime + duration : 0; }

	//! Returns the duration value stored in the idExtrapolate object.
	int				GetDuration() const { return duration; }

	/*!
		\brief Sets the starting value used for extrapolation.

		This function assigns the provided value to the internal startValue member, which serves as the baseline for extrapolation calculations.

		\param value The value to be used as the starting point for extrapolation
	*/
	void			SetStartValue( const type& value ) { startValue = value; }

	//! Returns the starting value used for extrapolation.
	const type&		GetStartValue() const { return startValue; }

	//! Returns the base speed value stored in the idExtrapolate object.
	const type&		GetBaseSpeed() const { return baseSpeed; }

	//! Returns the speed value.
	const type&		GetSpeed() const { return speed; }

	//! Returns the extrapolation type of the object.
	extrapolation_t GetExtrapolationType() const { return extrapolationType; }

private:
	extrapolation_t extrapolationType;
	int				startTime;
	int				duration;
	type			startValue;
	type			baseSpeed;
	type			speed;
};

template<class type>
ID_INLINE idExtrapolate<type>::idExtrapolate()
{
	extrapolationType = EXTRAPOLATION_NONE;
	startTime = duration = 0.0f;
	memset( &startValue, 0, sizeof( startValue ) );
	memset( &baseSpeed, 0, sizeof( baseSpeed ) );
	memset( &speed, 0, sizeof( speed ) );
}

template<class type>
ID_INLINE void idExtrapolate<type>::Init( const int startTime, const int duration, const type& startValue, const type& baseSpeed, const type& speed, const extrapolation_t extrapolationType )
{
	this->extrapolationType = extrapolationType;
	this->startTime			= startTime;
	this->duration			= duration;
	this->startValue		= startValue;
	this->baseSpeed			= baseSpeed;
	this->speed				= speed;
}

template<class type>
ID_INLINE type idExtrapolate<type>::GetCurrentValue( int time ) const
{
	if( time < startTime ) { return startValue; }

	if( !( extrapolationType & EXTRAPOLATION_NOSTOP ) && ( time > startTime + duration ) ) { time = startTime + duration; }

	switch( extrapolationType & ~EXTRAPOLATION_NOSTOP ) {
		case EXTRAPOLATION_NONE: {
			const float deltaTime = ( time - startTime ) * 0.001f;
			return startValue + deltaTime * baseSpeed;
		}
		case EXTRAPOLATION_LINEAR: {
			const float deltaTime = ( time - startTime ) * 0.001f;
			return startValue + deltaTime * ( baseSpeed + speed );
		}
		case EXTRAPOLATION_ACCELLINEAR: {
			if( duration == 0 ) {
				return startValue;
			} else {
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s		  = ( 0.5f * deltaTime * deltaTime ) * ( ( float )duration * 0.001f );
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
		case EXTRAPOLATION_DECELLINEAR: {
			if( duration == 0 ) {
				return startValue;
			} else {
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s		  = ( deltaTime - ( 0.5f * deltaTime * deltaTime ) ) * ( ( float )duration * 0.001f );
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
		case EXTRAPOLATION_ACCELSINE: {
			if( duration == 0 ) {
				return startValue;
			} else {
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s		  = ( 1.0f - idMath::Cos( deltaTime * idMath::HALF_PI ) ) * ( float )duration * 0.001f * idMath::SQRT_1OVER2;
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
		case EXTRAPOLATION_DECELSINE: {
			if( duration == 0 ) {
				return startValue;
			} else {
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s		  = idMath::Sin( deltaTime * idMath::HALF_PI ) * ( float )duration * 0.001f * idMath::SQRT_1OVER2;
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
	}
	return startValue;
}

template<class type>
ID_INLINE type idExtrapolate<type>::GetCurrentSpeed( int time ) const
{
	if( time < startTime || duration == 0 ) {
		return ( startValue - startValue ); //-V501
	}

	if( !( extrapolationType & EXTRAPOLATION_NOSTOP ) && ( time > startTime + duration ) ) {
		return ( startValue - startValue ); //-V501
	}

	switch( extrapolationType & ~EXTRAPOLATION_NOSTOP ) {
		case EXTRAPOLATION_NONE: {
			return baseSpeed;
		}
		case EXTRAPOLATION_LINEAR: {
			return baseSpeed + speed;
		}
		case EXTRAPOLATION_ACCELLINEAR: {
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s		  = deltaTime;
			return baseSpeed + s * speed;
		}
		case EXTRAPOLATION_DECELLINEAR: {
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s		  = 1.0f - deltaTime;
			return baseSpeed + s * speed;
		}
		case EXTRAPOLATION_ACCELSINE: {
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s		  = idMath::Sin( deltaTime * idMath::HALF_PI );
			return baseSpeed + s * speed;
		}
		case EXTRAPOLATION_DECELSINE: {
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s		  = idMath::Cos( deltaTime * idMath::HALF_PI );
			return baseSpeed + s * speed;
		}
		default: {
			return baseSpeed;
		}
	}
}

#endif /* !__MATH_EXTRAPOLATE_H__ */
