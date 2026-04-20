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

#ifndef __TIMER_H__
#define __TIMER_H__

/*
===============================================================================

	Clock tick counter. Should only be used for profiling.

===============================================================================
*/

class idTimer
{
public:
	/*!
		\brief Initializes a new instance of the idTimer class.

		This constructor initializes the timer state to TS_STOPPED and sets the clock ticks to zero.

	*/
	idTimer();

	/*!
		\brief Constructs an idTimer object with the specified clock ticks value.
		\param clockTicks The initial clock ticks value to store in the timer
	*/
	idTimer( double clockTicks );
	~idTimer();

	idTimer	 operator+( const idTimer& t ) const;
	idTimer	 operator-( const idTimer& t ) const;
	idTimer& operator+=( const idTimer& t );
	idTimer& operator-=( const idTimer& t );

	/*!
		\brief Initializes the timer and sets its state to started

		This function is used to start a timer that was previously stopped. It validates that the timer is in a stopped state before starting it, and records the starting timestamp using the system
	   clock. The timer state is updated to indicate that it has started.

		\throws assertion failure if the timer is not in TS_STOPPED state
	*/
	void	 Start();

	/*!
		\brief Stops the timer and records the elapsed time

		This function stops a running timer and accumulates the elapsed time since the last start. It asserts that the timer is currently in the started state before performing the stop operation. The
	   function also handles base clock tick initialization and adjustment when stopping the timer.

		\throws assertion failure if the timer is not in TS_STARTED state
	*/
	void	 Stop();

	//! Clears the timer by resetting its accumulated clock ticks to zero.
	void	 Clear();

	/*!
		\brief Returns the elapsed time in milliseconds since the timer was started

		This function calculates and returns the elapsed time in milliseconds based on the timer's internal clock ticks. It asserts that the timer is in a stopped state before performing the
	   calculation. The calculation divides the accumulated clock ticks by the system's clock ticks per second multiplied by 0.001 to convert to milliseconds.

		\return The elapsed time in milliseconds as a double precision floating point number
		\throws assertion failure if the timer is not in a stopped state
	*/
	double	 ClockTicks() const;

	/*!
		\brief Returns the elapsed time in milliseconds since the timer was started

		This function calculates and returns the elapsed time in milliseconds based on the timer's internal clock ticks. It asserts that the timer is in a stopped state before performing the
	   calculation. The calculation divides the accumulated clock ticks by the system's clock ticks per second multiplied by 0.001 to convert to milliseconds.

		\return The elapsed time in milliseconds as a double precision floating point number
		\throws assertion failure if the timer is not in a stopped state
	*/
	double	 Milliseconds() const;

private:
	static double base;
	enum { TS_STARTED, TS_STOPPED } state;
	double start;
	double clockTicks;

	/*!
		\brief Initializes the base clock ticks value by measuring and finding the minimum clock ticks over 1000 iterations.

		This function is used to calibrate the timer by measuring the minimum clock ticks value over 1000 iterations. It creates a temporary timer object, starts and stops it repeatedly to measure the
	   clock ticks, and records the minimum value found. This minimum value is then assigned to the base member variable of the timer object. The function is marked as const because it only modifies
	   the base member of the object it's called on.

	*/
	void   InitBaseClockTicks() const;
};

ID_INLINE idTimer::idTimer()
{
	state	   = TS_STOPPED;
	clockTicks = 0.0;
}

/*
=================
idTimer::idTimer
=================
*/
ID_INLINE idTimer::idTimer( double _clockTicks )
{
	state	   = TS_STOPPED;
	clockTicks = _clockTicks;
}

/*
=================
idTimer::~idTimer
=================
*/
ID_INLINE idTimer::~idTimer()
{
}

/*
=================
idTimer::operator+
=================
*/
ID_INLINE idTimer idTimer::operator+( const idTimer& t ) const
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	return idTimer( clockTicks + t.clockTicks );
}

/*
=================
idTimer::operator-
=================
*/
ID_INLINE idTimer idTimer::operator-( const idTimer& t ) const
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	return idTimer( clockTicks - t.clockTicks );
}

/*
=================
idTimer::operator+=
=================
*/
ID_INLINE idTimer& idTimer::operator+=( const idTimer& t )
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	clockTicks += t.clockTicks;
	return *this;
}

/*
=================
idTimer::operator-=
=================
*/
ID_INLINE idTimer& idTimer::operator-=( const idTimer& t )
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	clockTicks -= t.clockTicks;
	return *this;
}

ID_INLINE void idTimer::Start()
{
	assert( state == TS_STOPPED );
	state = TS_STARTED;
	start = idLib::sys->GetClockTicks();
}

ID_INLINE void idTimer::Stop()
{
	assert( state == TS_STARTED );
	clockTicks += idLib::sys->GetClockTicks() - start;
	if( base < 0.0 ) { InitBaseClockTicks(); }
	if( clockTicks > base ) { clockTicks -= base; }
	state = TS_STOPPED;
}

ID_INLINE void idTimer::Clear()
{
	clockTicks = 0.0;
}

ID_INLINE double idTimer::ClockTicks() const
{
	assert( state == TS_STOPPED );
	return clockTicks;
}

ID_INLINE double idTimer::Milliseconds() const
{
	assert( state == TS_STOPPED );
	return clockTicks / ( idLib::sys->ClockTicksPerSecond() * 0.001 );
}

/*
===============================================================================

	Report of multiple named timers.

===============================================================================
*/

class idTimerReport
{
public:
	//! Initializes a new instance of the idTimerReport class.
	idTimerReport();

	/*!
		\brief Destroys the idTimerReport object and clears its internal data.

		The destructor for the idTimerReport class cleans up any allocated resources by calling the Clear method. This ensures that all timer data is properly released when the object goes out of
	   scope.

	*/
	~idTimerReport();

	/*!
		\brief Sets the name of the timer report to the specified string or defaults to "Timer Report" if null.
		\param name The name to assign to the timer report, or null to use the default name
	*/
	void SetReportName( const char* name );

	/*!
		\brief Adds a new timer report with the specified name and returns its index.

		This function appends a new timer report to the list of reports. It takes a name for the report and creates a new timer instance associated with it. If the name is valid and non-empty, the
	   function adds the name to the names list and initializes a new timer, returning its index in the timers array. If the name is invalid or empty, the function returns -1.

		\param name Name of the timer report to be added.
		\return Index of the newly added timer report, or -1 if the name is invalid or empty.
	*/
	int	 AddReport( const char* name );

	//! Clears all timer data and resets the timer report state.
	void Clear();

	/*!
		\brief Clears all timers in the timer report

		Resets all timers stored in the timer report by clearing their accumulated data. This function ensures that the timers are properly initialized and ready for new measurements.

		\throws assertion failure if the number of timers does not match the number of names
	*/
	void Reset();

	/*!
		\brief Prints a timing report for all registered timers.

		This function outputs a formatted timing report to the common log stream. It iterates through all registered timers, prints the name and execution time of each timer, and calculates the total
	   time for the entire report. The time is displayed in seconds with two decimal places for better readability. The function assumes that the number of timers matches the number of names, as
	   verified by an assertion.

		\throws assertion failure if the number of timers does not match the number of names
	*/
	void PrintReport();

	/*!
		\brief Adds timing data from a timer to the report, either updating an existing entry or creating a new one.

		This function adds the timing data from the provided timer to an existing timer entry in the report if a timer with the specified name already exists. If no such timer exists, it creates a new
	   entry for the timer name and initializes it with the provided timing data. Both the timers and names arrays are maintained in sync to ensure proper tracking.

		\param name Name of the timer to add time to or create.
		\param time Pointer to the timer containing the timing data to be added.
	*/
	void AddTime( const char* name, idTimer* time );

private:
	idList<idTimer*> timers;
	idStrList		 names;
	idStr			 reportName;
};

#endif /* !__TIMER_H__ */
