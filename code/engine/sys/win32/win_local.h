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

#ifndef __WIN_LOCAL_H__
#define __WIN_LOCAL_H__

#include <windows.h>

#include "win_input.h"

#define WINDOW_STYLE ( WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME )

/*!
	\brief Queues a system event with the specified parameters into the event queue.

	This function adds a system event to the circular event queue. If the queue is full, it discards the oldest event and prints a warning. The event data is copied into the queue, and any previously
   allocated memory pointed to by evPtr is freed before discarding. The function handles the circular buffer management by updating the event head pointer and ensures that the event is properly
   initialized with all provided parameters including type, values, pointer length, pointer data, and input device number.

	\param type Type of the system event to be queued.
	\param value First integer value associated with the event.
	\param value2 Second integer value associated with the event.
	\param ptrLength Length of the data pointed to by ptr.
	\param ptr Pointer to event data, which can be null, or points to memory that can be freed later.
	\param inputDeviceNum Identifier of the input device that generated the event.
*/
void		Sys_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void* ptr, int inputDeviceNum );

//! Creates a console window for the application.
void		Sys_CreateConsole();

//! Destroys the console window if it exists.
void		Sys_DestroyConsole();

char*		Sys_ConsoleInput();

//! Returns the name of the current user.
char*		Sys_GetCurrentUser();

//! Sets the error text displayed in the Windows error box.
void		Win_SetErrorText( const char* text );

//! Returns the CPU identification flags indicating supported instruction sets and features
cpuid_t		Sys_GetCPUId();

// Input subsystem

void		IN_Init();
void		IN_Shutdown();

//! Deactivates the mouse input if the application is running in windowed mode.
void		IN_DeactivateMouseIfWindowed();

//! Deactivates the mouse by releasing the mouse acquisition and showing the cursor.
void		IN_DeactivateMouse();

//! Activates the mouse for input processing in the application.
void		IN_ActivateMouse();

//! Manages mouse grabbing state based on input and window conditions.
void		IN_Frame();

//! Disables or enables task keys and task manager based on the provided parameters.
void		DisableTaskKeys( BOOL bDisable, BOOL bBeep, BOOL bTaskMgr );

uint64		Sys_Microseconds();

/*!
	\brief Handles Windows messages for the main application window.

	This function processes Windows messages sent to the main application window. It handles various window events including resizing, moving, activation, keyboard input, character input, and system
   commands. The function updates window dimensions and positions, manages fullscreen toggling, handles application activation and deactivation, and queues input events for the game engine. It also
   manages Alt-Tab behavior and ensures proper window state management during fullscreen transitions.

	\param hWnd Handle to the window receiving the message
	\param uMsg Windows message identifier
	\param wParam Additional message-specific information
	\param lParam Additional message-specific information
	\return Message result depending on the handled message
*/
LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//! Appends text to the console buffer, handling line endings and overflow by replacing selection when necessary.
void		Conbuf_AppendText( const char* msg );

struct Win32Vars_t {
	HWND				  hWnd;
	HINSTANCE			  hInstance;

	bool				  activeApp;	 // changed with WM_ACTIVATE messages
	bool				  mouseReleased; // when the game has the console down or is doing a long operation
	bool				  movingWindow;	 // inhibit mouse grab when dragging the window
	bool				  mouseGrabbed;	 // current state of grab and hide

	OSVERSIONINFOEX		  osversion;

	cpuid_t				  cpuid;

	// when we get a windows message, we store the time off so keyboard processing
	// can know the exact time of an event (not really needed now that we use async direct input)
	int					  sysMsgTime;

	bool				  windowClassRegistered;

	WNDPROC				  wndproc;

	HDC					  hDC; // handle to device context
	PIXELFORMATDESCRIPTOR pfd;
	int					  pixelformat;

	HINSTANCE			  hinstOpenGL; // HINSTANCE for the OpenGL library

	int					  desktopBitsPixel;
	int					  desktopWidth, desktopHeight;

	int					  cdsFullscreen; // 0 = not fullscreen, otherwise monitor number

	idFileHandle		  log_fp;

	unsigned short		  oldHardwareGamma[3][256];
	// desktop gamma is saved here for restoration at exit

	static idCVar		  sys_arch;
	static idCVar		  sys_cpustring;
	static idCVar		  in_mouse;
	static idCVar		  win_allowAltTab;
	static idCVar		  win_notaskkeys;
	static idCVar		  win_username;
	static idCVar		  win_outputEditString;
	static idCVar		  win_viewlog;
	static idCVar		  win_timerUpdate;
	static idCVar		  win_allowMultipleInstances;

	static idCVar		  sys_useSteamPath;
	static idCVar		  sys_useGOGPath;

	CRITICAL_SECTION	  criticalSections[MAX_CRITICAL_SECTIONS];

	HINSTANCE			  hInstDI; // direct input

	LPDIRECTINPUT8		  g_pdi;
	LPDIRECTINPUTDEVICE8  g_pMouse;
	LPDIRECTINPUTDEVICE8  g_pKeyboard;
	idJoystickWin32		  g_Joystick;
};

extern Win32Vars_t win32;

#endif /* !__WIN_LOCAL_H__ */
