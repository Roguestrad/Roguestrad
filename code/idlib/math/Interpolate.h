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

#ifndef __MATH_INTERPOLATE_H__
#define __MATH_INTERPOLATE_H__

/*
==============================================================================================

	Linear interpolation.

==============================================================================================
*/

template<class type>
class idInterpolate
{
public:
	/*!
		\brief Initializes an interpolation object with default values.

		This constructor initializes an interpolation object by setting the start time and duration to zero, and clearing the memory for both start and end values to ensure they contain default
	   initialized data.

	*/
	idInterpolate();

	/*!
		\brief Initializes an interpolation with start time, duration, and start and end values.

		This function sets up the interpolation parameters including the start time, duration, and the values at the start and end of the interpolation period. It is used to configure the
	   interpolation state before computing intermediate values.

		\param startTime The start time for the interpolation period
		\param duration The total duration of the interpolation
		\param startValue The value at the beginning of the interpolation
		\param endValue The value at the end of the interpolation
	*/
	void		Init( const int startTime, const int duration, const type& startValue, const type& endValue );

	/*!
		\brief Sets the start time for the interpolation.

		This function assigns the provided time value to the internal start time member of the interpolation object. It is used to define when the interpolation process should begin.

		\param time The time value to set as the start time for the interpolation
	*/
	void		SetStartTime( int time ) { this->startTime = time; }

	/*!
		\brief Sets the duration of the interpolation to the specified value.

		This function configures the duration parameter for the interpolation operation. The duration controls how long the interpolation effect lasts, typically measured in game time units or frames.
	   The function also initializes the starting value for the interpolation process.

		\param duration The duration value to set for the interpolation
	*/
	void		SetDuration( int duration ) { this->duration = duration; }

	/*!
		\brief Sets the starting value for the interpolation.

		This function assigns the provided starting value to the internal startValue member of the interpolation object. It is typically used to define the initial state of an interpolation process.

		\param startValue The value to set as the starting point for the interpolation.
	*/
	void		SetStartValue( const type& startValue ) { this->startValue = startValue; }

	/*!
		\brief Sets the end value for the interpolation.

		This function updates the end value used in the interpolation process. The interpolation will transition from the current start value to this new end value over the specified time period.

		\param endValue The new end value to be used for interpolation
	*/
	void		SetEndValue( const type& endValue ) { this->endValue = endValue; }

	/*!
		\brief Returns the interpolated value at the specified time between the start and end values

		This function calculates and returns an interpolated value between the start and end values based on the provided time. If the time is less than or equal to the start time, it returns the
	   start value. If the time is greater than or equal to the end time (start time plus duration), it returns the end value. Otherwise, it performs linear interpolation between the start and end
	   values using the time ratio

		\param time The absolute time value to calculate the interpolation for
		\return The interpolated value at the specified time, which will be of the same type as the start and end values
	*/
	type		GetCurrentValue( int time ) const;

	/*!
		\brief Determines whether an interpolation has completed by comparing the given time with the start time and duration.

		This function checks if the specified time has exceeded the total duration of the interpolation, which starts at startTime. It is used to manage animation transitions and determine when an
	   interpolation should no longer be applied.

		\param time The current time to check against the interpolation's start time and duration
		\return True if the interpolation has completed (time is greater than or equal to start time plus duration), false otherwise
	*/
	bool		IsDone( int time ) const { return ( time >= startTime + duration ); }

	//! Returns the start time of the cinematic animation.
	int			GetStartTime() const { return startTime; }

	//! Returns the end time of the interpolation interval.
	int			GetEndTime() const { return startTime + duration; }

	//! Returns the duration of the interpolation.
	int			GetDuration() const { return duration; }

	//! Returns a const reference to the starting value of the interpolation.
	const type& GetStartValue() const { return startValue; }

	//! Returns the end value of the interpolation.
	const type& GetEndValue() const { return endValue; }

private:
	int	 startTime;
	int	 duration;
	type startValue;
	type endValue;
};

template<class type>
ID_INLINE idInterpolate<type>::idInterpolate()
{
	startTime = duration = 0;
	memset( &startValue, 0, sizeof( startValue ) );
	memset( &endValue, 0, sizeof( endValue ) );
}

template<class type>
ID_INLINE void idInterpolate<type>::Init( const int startTime, const int duration, const type& startValue, const type& endValue )
{
	this->startTime	 = startTime;
	this->duration	 = duration;
	this->startValue = startValue;
	this->endValue	 = endValue;
}

template<class type>
ID_INLINE type idInterpolate<type>::GetCurrentValue( int time ) const
{
	if( time <= startTime ) {
		return startValue;
	} else if( time >= startTime + duration ) {
		return endValue;
	} else {
		const float deltaTime = time - startTime;
		const float f		  = deltaTime / ( float )duration;
		const type	range	  = ( endValue - startValue );
		return startValue + ( range * f );
	}
}

/*
==============================================================================================

	Continuous interpolation with linear acceleration and deceleration phase.
	The velocity is continuous but the acceleration is not.

==============================================================================================
*/

template<class type>
class idInterpolateAccelDecelLinear
{
public:
	/*!
		\brief Initializes the interpolation parameters to their default values.

		Initializes the acceleration, deceleration, and linear interpolation parameters for a value over time. Sets all time parameters to zero and clears the start value structure.

	*/
	idInterpolateAccelDecelLinear();

	/*!
		\brief Initializes acceleration, deceleration, and linear interpolation parameters for time-based value transitions.

		This function sets up the interpolation parameters for a value that transitions from a start value to an end value over a specified duration. It handles the distribution of acceleration and
	   deceleration times, and initializes the underlying extrapolation mechanism. The function ensures that the total time allocated for acceleration and deceleration does not exceed the overall
	   duration, adjusting them proportionally if necessary. It also calculates the speed needed for the linear portion of the interpolation and configures the extrapolation based on whether there is
	   acceleration, linear, or deceleration phase.

		\param startTime The start time of the interpolation
		\param accelTime The duration of the acceleration phase
		\param decelTime The duration of the deceleration phase
		\param duration The total duration of the interpolation
		\param startValue The initial value of the interpolation
		\param endValue The final value of the interpolation
	*/
	void Init( const int startTime, const int accelTime, const int decelTime, const int duration, const type& startValue, const type& endValue );

	/*!
		\brief Sets the start time for the interpolation and invalidates the current state.

		This function updates the start time used for interpolation calculations and marks the current interpolation state as invalid. This ensures that any subsequent interpolation operations will
	   recalculate their values based on the new start time.

		\param time The new start time value to be set for the interpolation
	*/
	void SetStartTime( int time )
	{
		startTime = time;
		Invalidate();
	}

	/*!
		\brief Sets the starting value for the interpolation and invalidates the current interpolation state

		This function updates the starting value used for interpolation calculations and ensures that any existing interpolation state is invalidated. This is typically called when beginning a new
	   interpolation sequence or when the initial value needs to be changed mid-interpolation

		\param startValue the new starting value to be used for interpolation
	*/
	void SetStartValue( const type& startValue )
	{
		this->startValue = startValue;
		Invalidate();
	}

	/*!
		\brief Sets the end value for the interpolation and invalidates the current interpolation state.

		This function updates the end value used for interpolation and marks the current interpolation state as invalid. This invalidation typically triggers a recalculation of the interpolation
	   parameters when the interpolation is next evaluated.

		\param endValue The new end value to be used for the interpolation
	*/
	void SetEndValue( const type& endValue )
	{
		this->endValue = endValue;
		Invalidate();
	}

	/*!
		\brief Returns the current interpolated value at the specified time using acceleration and deceleration timing

		This function calculates and returns the interpolated value for a given time point. It first updates the internal phase state based on the provided time, then uses the extrapolation mechanism
	   to compute and return the current value. The interpolation follows an acceleration and deceleration curve for smooth transitions.

		\param time The time value for which the interpolated result is calculated
		\return The interpolated value at the specified time based on acceleration and deceleration timing
	*/
	type		GetCurrentValue( int time ) const;

	/*!
		\brief Returns the current speed at the specified time during interpolation.

		This function calculates and returns the speed at a given time during an interpolation process. It first updates the internal phase state based on the provided time, then retrieves the current
	   speed from the extrapolation component.

		\param time The time value at which to calculate the current speed
		\return The speed value at the specified time
	*/
	type		GetCurrentSpeed( int time ) const;

	/*!
		\brief Determines whether the interpolation animation has completed by the specified time.

		This function evaluates if the interpolation process, which includes acceleration, linear, and deceleration phases, has finished by the given time. It compares the input time against the total
	   duration of the interpolation, which is the sum of the start time, acceleration time, linear time, and deceleration time.

		\param time The time value to check against the interpolation completion threshold
		\return True if the interpolation is complete at the specified time, false otherwise
	*/
	bool		IsDone( int time ) const { return ( time >= startTime + accelTime + linearTime + decelTime ); }

	//! Returns the start time of the cinematic animation.
	int			GetStartTime() const { return startTime; }

	//! Returns the end time of the interpolation process.
	int			GetEndTime() const { return startTime + accelTime + linearTime + decelTime; }

	//! Returns the total duration of the acceleration, linear, and deceleration phases.
	int			GetDuration() const { return accelTime + linearTime + decelTime; }

	//! Returns the acceleration time value.
	int			GetAcceleration() const { return accelTime; }

	//! Returns the deceleration time value.
	int			GetDeceleration() const { return decelTime; }

	//! Returns the starting value of the interpolation.
	const type& GetStartValue() const { return startValue; }

	//! Returns the end value of the interpolation.
	const type& GetEndValue() const { return endValue; }

private:
	int							startTime;
	int							accelTime;
	int							linearTime;
	int							decelTime;
	type						startValue;
	type						endValue;
	mutable idExtrapolate<type> extrapolate;

	/*!
		\brief Initializes the extrapolation state to an invalid configuration.

		This function resets the extrapolation parameters to an invalid state by initializing the extrapolate object with zero values for time and distance, and setting the extrapolation type to
	   EXTRAPOLATION_NONE. The function is typically used to reset the interpolation state when the interpolation data becomes invalid or needs to be restarted.

	*/
	void						Invalidate();

	/*!
		\brief Sets the phase of the acceleration/deceleration linear interpolation based on the given time

		This function determines which phase of the interpolation the given time falls into and initializes the appropriate extrapolation type. It handles three phases: acceleration, linear motion,
	   and deceleration. The function checks the current time against the start time and the defined acceleration and deceleration time periods to decide which phase to initialize.

		\param time The time value to determine the interpolation phase
	*/
	void						SetPhase( int time ) const;
};

template<class type>
ID_INLINE idInterpolateAccelDecelLinear<type>::idInterpolateAccelDecelLinear()
{
	startTime = accelTime = linearTime = decelTime = 0;
	memset( &startValue, 0, sizeof( startValue ) );
	endValue = startValue;
}

template<class type>
ID_INLINE void idInterpolateAccelDecelLinear<type>::Init( const int startTime, const int accelTime, const int decelTime, const int duration, const type& startValue, const type& endValue )
{
	this->startTime	 = startTime;
	this->accelTime	 = accelTime;
	this->decelTime	 = decelTime;
	this->startValue = startValue;
	this->endValue	 = endValue;

	if( duration <= 0 ) { return; }

	if( this->accelTime + this->decelTime > duration ) {
		this->accelTime = this->accelTime * duration / ( this->accelTime + this->decelTime );
		this->decelTime = duration - this->accelTime;
	}
	this->linearTime = duration - this->accelTime - this->decelTime;
	const type speed = ( endValue - startValue ) * ( 1000.0f / ( ( float )this->linearTime + ( this->accelTime + this->decelTime ) * 0.5f ) );

	if( this->accelTime ) {
		extrapolate.Init( startTime, this->accelTime, startValue, ( startValue - startValue ), speed, EXTRAPOLATION_ACCELLINEAR ); //-V501
	} else if( this->linearTime ) {
		extrapolate.Init( startTime, this->linearTime, startValue, ( startValue - startValue ), speed, EXTRAPOLATION_LINEAR ); //-V501
	} else {
		extrapolate.Init( startTime, this->decelTime, startValue, ( startValue - startValue ), speed, EXTRAPOLATION_DECELLINEAR ); //-V501
	}
}

template<class type>
ID_INLINE void idInterpolateAccelDecelLinear<type>::Invalidate()
{
	extrapolate.Init( 0, 0, extrapolate.GetStartValue(), extrapolate.GetBaseSpeed(), extrapolate.GetSpeed(), EXTRAPOLATION_NONE );
}

template<class type>
ID_INLINE void idInterpolateAccelDecelLinear<type>::SetPhase( int time ) const
{
	const float deltaTime = time - startTime;
	if( deltaTime < accelTime ) {
		if( extrapolate.GetExtrapolationType() != EXTRAPOLATION_ACCELLINEAR ) {
			extrapolate.Init( startTime, accelTime, startValue, extrapolate.GetBaseSpeed(), extrapolate.GetSpeed(), EXTRAPOLATION_ACCELLINEAR );
		}
	} else if( deltaTime < accelTime + linearTime ) {
		if( extrapolate.GetExtrapolationType() != EXTRAPOLATION_LINEAR ) {
			extrapolate.Init(
				startTime + accelTime, linearTime, startValue + extrapolate.GetSpeed() * ( accelTime * 0.001f * 0.5f ), extrapolate.GetBaseSpeed(), extrapolate.GetSpeed(), EXTRAPOLATION_LINEAR );
		}
	} else {
		if( extrapolate.GetExtrapolationType() != EXTRAPOLATION_DECELLINEAR ) {
			extrapolate.Init( startTime + accelTime + linearTime,
				decelTime,
				endValue - ( extrapolate.GetSpeed() * ( decelTime * 0.001f * 0.5f ) ),
				extrapolate.GetBaseSpeed(),
				extrapolate.GetSpeed(),
				EXTRAPOLATION_DECELLINEAR );
		}
	}
}

template<class type>
ID_INLINE type idInterpolateAccelDecelLinear<type>::GetCurrentValue( int time ) const
{
	SetPhase( time );
	return extrapolate.GetCurrentValue( time );
}

template<class type>
ID_INLINE type idInterpolateAccelDecelLinear<type>::GetCurrentSpeed( int time ) const
{
	SetPhase( time );
	return extrapolate.GetCurrentSpeed( time );
}

/*
==============================================================================================

	Continuous interpolation with sinusoidal acceleration and deceleration phase.
	Both the velocity and acceleration are continuous.

==============================================================================================
*/

template<class type>
class idInterpolateAccelDecelSine
{
public:
	/*!
		\brief Initializes all time and value members to their default states for acceleration and deceleration sine interpolation.

		The constructor initializes the timing parameters startTime, accelTime, linearTime, and decelTime to zero. It also clears the startValue and endValue members to their default states using
	   memset. This setup prepares the interpolation object for subsequent configuration with specific timing and value parameters.

	*/
	idInterpolateAccelDecelSine();

	/*!
		\brief Initializes acceleration and deceleration sine interpolation with specified timing and value parameters.

		Configures the interpolation parameters including start time, acceleration time, deceleration time, and duration. The function adjusts the acceleration and deceleration times if they exceed
	   the total duration, and calculates the linear time segment. It then sets up an extrapolation object with appropriate interpolation type based on the timing parameters. The function handles
	   three interpolation types: acceleration sine, linear, and deceleration sine depending on the provided timing values. The start and end values are stored for use in the interpolation process.

		\param startTime The start time for the interpolation
		\param accelTime The duration of the acceleration phase
		\param decelTime The duration of the deceleration phase
		\param duration The total duration of the interpolation
		\param startValue The starting value of the interpolation
		\param endValue The ending value of the interpolation
	*/
	void Init( const int startTime, const int accelTime, const int decelTime, const int duration, const type& startValue, const type& endValue );

	/*!
		\brief Sets the start time for the interpolation and invalidates the current state.

		This function updates the start time of the interpolation to the specified time value and invalidates the current interpolation state to ensure a fresh calculation on the next update.

		\param time The new start time for the interpolation
	*/
	void SetStartTime( int time )
	{
		startTime = time;
		Invalidate();
	}

	/*!
		\brief Sets the starting value for the interpolation and invalidates the current interpolation state

		This function updates the starting value used for interpolation calculations and ensures that any previously computed interpolation data is invalidated

		\param startValue the new starting value to be set for interpolation
	*/
	void SetStartValue( const type& startValue )
	{
		this->startValue = startValue;
		Invalidate();
	}

	/*!
		\brief Sets the end value for the interpolation and invalidates the current interpolation state

		This function updates the end value used in the interpolation process and invalidates the current interpolation state to ensure that any subsequent interpolation operations start fresh with
	   the new end value

		\param endValue the new end value to be set for the interpolation
	*/
	void SetEndValue( const type& endValue )
	{
		this->endValue = endValue;
		Invalidate();
	}

	/*!
		\brief Returns the current interpolated value based on the provided time using acceleration and deceleration with a sine curve

		This function calculates and returns the current value of an interpolation that follows an acceleration and deceleration pattern based on a sine curve. It first sets the interpolation phase
	   based on the provided time, then uses an extrapolation object to compute and return the final interpolated value. The function is typically used in user interface animations or transitions
	   where smooth movement is desired.

		\param time The current time value used to calculate the interpolation phase and determine the current interpolated value
		\return The current interpolated value at the specified time, following an acceleration and deceleration pattern with a sine curve
	*/
	type		GetCurrentValue( int time ) const;

	/*!
		\brief Returns the current speed based on the interpolation phase and time

		This function calculates and returns the current speed for interpolation at a given time. It first updates the interpolation phase using the provided time value, then uses the extrapolation
	   object to determine the actual speed. The function is designed to work within animation or interpolation systems where smooth transitions are required.

		\param time The absolute time value used to calculate the current interpolation phase and speed
		\return The current speed value at the specified time based on the interpolation and extrapolation calculations
	*/
	type		GetCurrentSpeed( int time ) const;

	/*!
		\brief Checks if the interpolation animation is complete at the given time

		This function determines whether an interpolation animation has finished based on the provided time value. It compares the given time against the sum of the start time and all animation
	   duration components including acceleration, linear, and deceleration phases

		\param time The absolute time value to check against the animation completion
		\return True if the animation is complete at the specified time, false otherwise
	*/
	bool		IsDone( int time ) const { return ( time >= startTime + accelTime + linearTime + decelTime ); }

	//! Returns the start time of the cinematic animation.
	int			GetStartTime() const { return startTime; }

	//! Returns the end time of the interpolation cycle.
	int			GetEndTime() const { return startTime + accelTime + linearTime + decelTime; }

	//! Returns the total duration of the interpolation by summing the acceleration, linear, and deceleration time components.
	int			GetDuration() const { return accelTime + linearTime + decelTime; }

	//! Returns the acceleration time value.
	int			GetAcceleration() const { return accelTime; }

	//! Returns the deceleration time value.
	int			GetDeceleration() const { return decelTime; }

	//! Returns the starting value of the interpolation.
	const type& GetStartValue() const { return startValue; }

	//! Returns the end value of the interpolation curve
	const type& GetEndValue() const { return endValue; }

private:
	int							startTime;
	int							accelTime;
	int							linearTime;
	int							decelTime;
	type						startValue;
	type						endValue;
	mutable idExtrapolate<type> extrapolate;

	/*!
		\brief Initializes the extrapolation state to an invalid or neutral condition.

		This function resets the extrapolation state by initializing it with default parameters that represent an invalid or neutral condition. It sets up the extrapolation with zero start time, zero
	   base speed, and the current start value, effectively clearing any previous extrapolation data.

	*/
	void						Invalidate();

	/*!
		\brief Sets the phase of the interpolation animation based on the provided time.

		This function determines which phase of the interpolation animation should be used based on the given time value. It checks if the current time falls within the acceleration, linear, or
	   deceleration phases and initializes the appropriate extrapolation type. The function updates the extrapolation state to reflect the current phase of the interpolation.

		\param time The time value used to determine which phase of the interpolation animation to set
	*/
	void						SetPhase( int time ) const;
};

template<class type>
ID_INLINE idInterpolateAccelDecelSine<type>::idInterpolateAccelDecelSine()
{
	startTime = accelTime = linearTime = decelTime = 0;
	memset( &startValue, 0, sizeof( startValue ) );
	memset( &endValue, 0, sizeof( endValue ) );
}

template<class type>
ID_INLINE void idInterpolateAccelDecelSine<type>::Init( const int startTime, const int accelTime, const int decelTime, const int duration, const type& startValue, const type& endValue )
{
	this->startTime	 = startTime;
	this->accelTime	 = accelTime;
	this->decelTime	 = decelTime;
	this->startValue = startValue;
	this->endValue	 = endValue;

	if( duration <= 0 ) { return; }

	if( this->accelTime + this->decelTime > duration ) {
		this->accelTime = this->accelTime * duration / ( this->accelTime + this->decelTime );
		this->decelTime = duration - this->accelTime;
	}
	this->linearTime = duration - this->accelTime - this->decelTime;
	const type speed = ( endValue - startValue ) * ( 1000.0f / ( ( float )this->linearTime + ( this->accelTime + this->decelTime ) * idMath::SQRT_1OVER2 ) );

	if( this->accelTime ) {
		extrapolate.Init( startTime, this->accelTime, startValue, ( startValue - startValue ), speed, EXTRAPOLATION_ACCELSINE ); //-V501
	} else if( this->linearTime ) {
		extrapolate.Init( startTime, this->linearTime, startValue, ( startValue - startValue ), speed, EXTRAPOLATION_LINEAR ); //-V501
	} else {
		extrapolate.Init( startTime, this->decelTime, startValue, ( startValue - startValue ), speed, EXTRAPOLATION_DECELSINE ); //-V501
	}
}

template<class type>
ID_INLINE void idInterpolateAccelDecelSine<type>::Invalidate()
{
	extrapolate.Init( 0, 0, extrapolate.GetStartValue(), extrapolate.GetBaseSpeed(), extrapolate.GetSpeed(), EXTRAPOLATION_NONE );
}

template<class type>
ID_INLINE void idInterpolateAccelDecelSine<type>::SetPhase( int time ) const
{
	const float deltaTime = time - startTime;
	if( deltaTime < accelTime ) {
		if( extrapolate.GetExtrapolationType() != EXTRAPOLATION_ACCELSINE ) {
			extrapolate.Init( startTime, accelTime, startValue, extrapolate.GetBaseSpeed(), extrapolate.GetSpeed(), EXTRAPOLATION_ACCELSINE );
		}
	} else if( deltaTime < accelTime + linearTime ) {
		if( extrapolate.GetExtrapolationType() != EXTRAPOLATION_LINEAR ) {
			extrapolate.Init( startTime + accelTime,
				linearTime,
				startValue + extrapolate.GetSpeed() * ( accelTime * 0.001f * idMath::SQRT_1OVER2 ),
				extrapolate.GetBaseSpeed(),
				extrapolate.GetSpeed(),
				EXTRAPOLATION_LINEAR );
		}
	} else {
		if( extrapolate.GetExtrapolationType() != EXTRAPOLATION_DECELSINE ) {
			extrapolate.Init( startTime + accelTime + linearTime,
				decelTime,
				endValue - ( extrapolate.GetSpeed() * ( decelTime * 0.001f * idMath::SQRT_1OVER2 ) ),
				extrapolate.GetBaseSpeed(),
				extrapolate.GetSpeed(),
				EXTRAPOLATION_DECELSINE );
		}
	}
}

template<class type>
ID_INLINE type idInterpolateAccelDecelSine<type>::GetCurrentValue( int time ) const
{
	SetPhase( time );
	return extrapolate.GetCurrentValue( time );
}

template<class type>
ID_INLINE type idInterpolateAccelDecelSine<type>::GetCurrentSpeed( int time ) const
{
	SetPhase( time );
	return extrapolate.GetCurrentSpeed( time );
}

#endif /* !__MATH_INTERPOLATE_H__ */
