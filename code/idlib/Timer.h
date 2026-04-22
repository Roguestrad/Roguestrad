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

/*!
	\class idTimer
	\brief A timer class for measuring elapsed time in clock ticks and milliseconds.

	The idTimer class provides functionality for measuring elapsed time using clock ticks and converting those measurements to milliseconds. It supports starting, stopping, and clearing the timer, as
   well as performing arithmetic operations between timer instances. The class can be initialized with a specific clock tick value or created with a stopped state. It maintains an internal state to
   track whether the timer is running or stopped, and accumulates clock ticks during the active period. The timer's elapsed time can be retrieved in both raw clock tick values and converted
   milliseconds, making it suitable for performance monitoring and timing measurements.

*/
class idTimer
{
public:
	//! Initializes a new instance of the idTimer class with stopped state and zero clock ticks.
	idTimer();

	//! Constructs an idTimer object with the specified clock tick value.
	idTimer( double clockTicks );
	~idTimer();

	//! Returns a new timer with the combined clock ticks of this timer and the provided timer.
	idTimer	 operator+( const idTimer& t ) const;

	//! Returns the difference between this timer and another timer.
	idTimer	 operator-( const idTimer& t ) const;

	//! Adds the clock ticks from another timer to this timer and returns a reference to this timer.
	idTimer& operator+=( const idTimer& t );

	//! Subtracts the clock ticks of another timer from this timer.
	idTimer& operator-=( const idTimer& t );

	//! Starts the timer by setting its state to started and recording the current clock ticks.
	void	 Start();

	//! Stops the timer and records the elapsed time.
	void	 Stop();

	//! Resets the timer's accumulated clock tick count to zero.
	void	 Clear();

	//! Returns the elapsed clock ticks measured by the timer.
	double	 ClockTicks() const;

	//! Returns the elapsed time in milliseconds since the timer was last stopped.
	double	 Milliseconds() const;

private:
	static double base;
	enum { TS_STARTED, TS_STOPPED } state;
	double start;
	double clockTicks;

	//! Initializes the base clock ticks measurement for timing operations.
	void   InitBaseClockTicks() const;
};

ID_INLINE idTimer::idTimer()
{
	state	   = TS_STOPPED;
	clockTicks = 0.0;
}

ID_INLINE idTimer::idTimer( double _clockTicks )
{
	state	   = TS_STOPPED;
	clockTicks = _clockTicks;
}

ID_INLINE idTimer::~idTimer()
{
}

ID_INLINE idTimer idTimer::operator+( const idTimer& t ) const
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	return idTimer( clockTicks + t.clockTicks );
}

ID_INLINE idTimer idTimer::operator-( const idTimer& t ) const
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	return idTimer( clockTicks - t.clockTicks );
}

ID_INLINE idTimer& idTimer::operator+=( const idTimer& t )
{
	assert( state == TS_STOPPED && t.state == TS_STOPPED );
	clockTicks += t.clockTicks;
	return *this;
}

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

/*!
	\class idTimerReport
	\brief Manages and reports timing data for multiple timers.

	The idTimerReport class provides functionality to track, accumulate, and report timing data from multiple timers. It supports adding new timer entries, resetting accumulated data, and printing
   formatted reports. The class maintains a collection of timers identified by names and allows adding time from external timer objects to these entries. It can be used to profile performance by
   recording timing information across different parts of the application.

*/
class idTimerReport
{
public:
	//! Initializes a new instance of the idTimerReport class.
	idTimerReport();

	//! Destroys the timer report and clears its contents.
	~idTimerReport();

	//! Sets the name of the timer report to the specified value or defaults to "Timer Report" if null.
	void SetReportName( const char* name );

	//! Adds a new timer report with the specified name and returns its index.
	int	 AddReport( const char* name );

	//! Clears all timer data and resets the timer report state.
	void Clear();

	//! Resets all timers in the timer report by clearing their accumulated data.
	void Reset();

	//! Prints a timing report for all recorded timers.
	void PrintReport();

	//! Adds the time from the provided timer to an existing timer with the specified name or creates a new timer entry if it doesn't exist.
	void AddTime( const char* name, idTimer* time );

private:
	idList<idTimer*> timers;
	idStrList		 names;
	idStr			 reportName;
};

#endif /* !__TIMER_H__ */
