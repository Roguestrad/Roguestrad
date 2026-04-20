/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

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

// RB: missing __analysis_assume
#if defined( __MINGW32__ )
	#include <sal.h>
#endif
// RB end

#include "ParallelJobList_JobHeaders.h"

#ifdef _WIN32
	#include <windows.h> // for DebugBreak
#else					 // POSIX for raise()
	#include <signal.h>
#endif

/*
================================================================================================

	Software Cache

================================================================================================
*/

uint32 globalDmaTag;

/*!
	\brief Handles assertion failure during Spurs emulation by breaking into the debugger and returning true.

	This function is called when an assertion fails during Spurs emulation. It first checks a static boolean flag to determine whether to halt execution. If the flag is set to true, the function will
   break into the debugger based on the platform: Windows with MSVC uses __debugbreak(), Windows with MinGW uses DebugBreak(), and POSIX systems use raise(SIGTRAP). The function always returns true to
   indicate that the assertion failure has been handled.

	\param filename The name of the file where the assertion failed.
	\param line The line number in the file where the assertion failed.
	\param expression The expression that failed the assertion.
	\return True, indicating that the assertion failure has been processed.
	\throws This function may throw a SIGTRAP signal on POSIX systems when calling raise().
*/
bool   SpursEmulationAssertFailed( const char* filename, int line, const char* expression )
{
	static bool halt = true;
	if( halt ) {
#ifdef _WIN32
	#ifdef _MSC_VER
		__debugbreak();
	#else
		// DG: mingw support
		DebugBreak();
	#endif
#else
		// DG: POSIX support
		raise( SIGTRAP );
		// DG: end
#endif
	}
	return true;
}
