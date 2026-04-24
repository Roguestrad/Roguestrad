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

#ifndef __SYS_LOCAL__
#define __SYS_LOCAL__

/*!
	\class idSysLocal
	\brief Provides low-level system services and hardware abstraction for application runtime behavior.

	This class implements system-level functionality that serves as a bridge between the application and the underlying operating system. It handles hardware-specific operations such as processor
   identification, FPU state management, and memory locking for performance-critical sections. The class also manages dynamic library loading and provides timing capabilities using high-resolution
   performance counters. It supports system event generation for input handling and provides utilities for launching external processes and opening URLs. The interface is designed to abstract
   platform-specific implementations while maintaining direct access to system resources for optimal performance.

*/
class idSysLocal : public idSys
{
public:
	//! Outputs a formatted debug message to the console
	virtual void		DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Outputs a debug message with a variable argument list.
	virtual void		DebugVPrintf( const char* fmt, va_list arg );

	//! Returns the current value of the high-resolution performance counter.
	virtual double		GetClockTicks();

	//! Returns the number of clock ticks per second.
	virtual double		ClockTicksPerSecond();

	//! Returns the processor identifier for the current system.
	virtual cpuid_t		GetProcessorId();

	//! Returns a string describing the processor.
	virtual const char* GetProcessorString();

	//! Returns a string representation of the current FPU state.
	virtual const char* FPU_GetState();

	//! Checks if the FPU stack is empty
	virtual bool		FPU_StackIsEmpty();

	//! Sets the FPU flush-to-zero flag state
	virtual void		FPU_SetFTZ( bool enable );

	//! Sets the DAZ (Denormal Always Zero) flag in the FPU control word to enable or disable denormal number handling.
	virtual void		FPU_SetDAZ( bool enable );

	//! Enables specified floating-point unit exceptions.
	virtual void		FPU_EnableExceptions( int exceptions );

	//! Attempts to lock a specified memory region to prevent it from being swapped out by the operating system.
	virtual bool		LockMemory( void* ptr, int bytes );

	//! Unlocks a previously locked memory block
	virtual bool		UnlockMemory( void* ptr, int bytes );

	//! Loads a dynamic library by name and returns a handle to it.
	virtual int			DLL_Load( const char* dllName );

	//! Retrieves a function pointer from a dynamically loaded library module
	virtual void*		DLL_GetProcAddress( int dllHandle, const char* procName );

	//! Unloads a dynamic library handle.
	virtual void		DLL_Unload( int dllHandle );

	//! Constructs a dynamic library file name using the provided base name and platform-specific suffix.
	virtual void		DLL_GetFileName( const char* baseName, char* dllName, int maxLength );

	//! Generates a system event for a mouse button state change.
	virtual sysEvent_t	GenerateMouseButtonEvent( int button, bool down );

	//! Creates a system event representing a mouse movement with the specified delta coordinates.
	virtual sysEvent_t	GenerateMouseMoveEvent( int deltax, int deltay );

	//! Opens a URL using a system script, optionally quitting the application afterward.
	virtual void		OpenURL( const char* url, bool quit );

	//! Starts an external process, optionally deferring execution until program exit.
	virtual void		StartProcess( const char* exeName, bool quit );
};

#endif /* !__SYS_LOCAL__ */
