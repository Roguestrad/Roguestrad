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
#include "precompiled.h"
#pragma hdrstop

#ifdef _MSC_VER // DG: dx SDK's xinput.h is incompatible with MinGW..
	#include <Xinput.h>
#else // .. so use the one from wine instead when not using MSVC
	#include "wine-xinput.h"
#endif // DG end

static const int MAX_JOYSTICKS = 4;

/*
================================================================================================

	Joystick Win32

================================================================================================
*/

struct controllerState_t {
	// the current states are updated by the input thread at 250 hz
	XINPUT_STATE current;

	// the previous state is latched at polling time
	XINPUT_STATE previous;

	// The current button bits are or'd into this at the high sampling rate, then
	// zero'd by the main thread when a usercmd_t is created.  This prevents the
	// complete missing of a button press that went down and up between two usercmd_t
	// creations, although it can add sn extra frame of latency to sensing a release.
	int			 buttonBits;

	// Only valid controllers will have their rumble set
	bool		 valid;
};

/*!
	\class idJoystickWin32
	\brief Windows-specific joystick input handler that processes and translates joystick events for game input.

	This class provides Windows-specific implementation for joystick input handling, inheriting from a base joystick interface. It manages initialization of the joystick subsystem, polls input events
   from connected devices, and translates these events into game actions or GUI button presses. The class supports rumble effects for joystick devices and maintains internal state for button presses
   and axis movements. It processes different types of joystick events including action buttons, axis movements, and directional pad inputs. The implementation handles event queuing and maintains a
   consistent interface for joystick input regardless of the specific device type connected.

*/
class idJoystickWin32 : idJoystick
{
public:
	//! Initializes a new instance of the idJoystickWin32 class.
	idJoystickWin32();

	//! Initializes the joystick subsystem for Windows
	virtual bool Init();

	//! Sets the rumble intensity for both motors of a specified joystick device.
	virtual void SetRumble( int deviceNum, int rumbleLow, int rumbleHigh );

	//! Polls input events from a specified joystick device and returns the number of events detected
	virtual int	 PollInputEvents( int inputDeviceNum );

	//! Retrieves the action and value for a specified joystick input event index
	virtual int	 ReturnInputEvent( const int n, int& action, int& value );

	//! Completes the input event processing cycle for the joystick device.
	virtual void EndInputEvents() { }

protected:
	friend void JoystickSamplingThread( void* data );

	//! Sets the state of a joystick button and queues a key event if the state changes
	void		PushButton( int inputDeviceNum, int key, bool value );

	/*!
		\brief Processes joystick input events and translates them into game input actions or GUI button presses.

		This function handles various joystick events by mapping them to specific game actions or GUI button states. It processes button press events and axis movements, updating internal state and
	   queuing events for the game engine. For axis events, it calculates a percentage value based on the input range and only queues a new event if the value has changed. The function distinguishes
	   between different types of events, such as action buttons, joystick axes, and directional pad inputs.

		\param inputDeviceNum The index of the input device
		\param event The type of joystick event that occurred
		\param value The value associated with the joystick event
		\param range The range of the joystick axis values, defaults to 16384
	*/
	void		PostInputEvent( int inputDeviceNum, int event, int value, int range = 16384 );

	idSysMutex	mutexXis; // lock this before using currentXis or stickIntegrations
	HANDLE		timer;	  // fire every 4 msec

	int			numEvents;

	struct {
		int event;
		int value;
	} events[MAX_JOY_EVENT];

	controllerState_t controllers[MAX_JOYSTICKS];

	// should these be per-controller?
	bool			  buttonStates[MAX_INPUT_DEVICES][K_LAST_KEY];	 // For keeping track of button up/down events
	int				  joyAxis[MAX_INPUT_DEVICES][MAX_JOYSTICK_AXIS]; // For keeping track of joystick axises
};
