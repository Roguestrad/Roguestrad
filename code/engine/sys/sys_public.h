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

#ifndef __SYS_PUBLIC__
#define __SYS_PUBLIC__

#include "../idlib/CmdArgs.h"

/*
===============================================================================

	Non-portable system services.

===============================================================================
*/

enum cpuid_t {
	CPUID_NONE		  = 0x00000,
	CPUID_UNSUPPORTED = 0x00001, // unsupported (386/486)
	CPUID_GENERIC	  = 0x00002, // unrecognized processor
	CPUID_INTEL		  = 0x00004, // Intel
	CPUID_AMD		  = 0x00008, // AMD
	CPUID_MMX		  = 0x00010, // Multi Media Extensions
	CPUID_3DNOW		  = 0x00020, // 3DNow!
	CPUID_SSE		  = 0x00040, // Streaming SIMD Extensions
	CPUID_SSE2		  = 0x00080, // Streaming SIMD Extensions 2
	CPUID_SSE3		  = 0x00100, // Streaming SIMD Extentions 3 aka Prescott's New Instructions
	CPUID_ALTIVEC	  = 0x00200, // AltiVec
	CPUID_HTT		  = 0x01000, // Hyper-Threading Technology
	CPUID_CMOV		  = 0x02000, // Conditional Move (CMOV) and fast floating point comparison (FCOMI) instructions
	CPUID_FTZ		  = 0x04000, // Flush-To-Zero mode (denormal results are flushed to zero)
	CPUID_DAZ		  = 0x08000, // Denormals-Are-Zero mode (denormal source operands are set to zero)
	CPUID_XENON		  = 0x10000, // Xbox 360
	CPUID_CELL		  = 0x20000	 // PS3
};

enum fpuExceptions_t {
	FPU_EXCEPTION_INVALID_OPERATION	   = 1,
	FPU_EXCEPTION_DENORMALIZED_OPERAND = 2,
	FPU_EXCEPTION_DIVIDE_BY_ZERO	   = 4,
	FPU_EXCEPTION_NUMERIC_OVERFLOW	   = 8,
	FPU_EXCEPTION_NUMERIC_UNDERFLOW	   = 16,
	FPU_EXCEPTION_INEXACT_RESULT	   = 32
};

enum fpuPrecision_t { FPU_PRECISION_SINGLE = 0, FPU_PRECISION_DOUBLE = 1, FPU_PRECISION_DOUBLE_EXTENDED = 2 };

enum fpuRounding_t { FPU_ROUNDING_TO_NEAREST = 0, FPU_ROUNDING_DOWN = 1, FPU_ROUNDING_UP = 2, FPU_ROUNDING_TO_ZERO = 3 };

enum joystickAxis_t { AXIS_LEFT_X, AXIS_LEFT_Y, AXIS_RIGHT_X, AXIS_RIGHT_Y, AXIS_LEFT_TRIG, AXIS_RIGHT_TRIG, MAX_JOYSTICK_AXIS };

enum sysEventType_t {
	SE_NONE,		   // evTime is still valid
	SE_KEY,			   // evValue is a key code, evValue2 is the down flag
	SE_CHAR,		   // evValue is an Unicode UTF-32 char (or non-surrogate UTF-16)
	SE_MOUSE,		   // evValue and evValue2 are relative signed x / y moves
	SE_MOUSE_ABSOLUTE, // evValue and evValue2 are absolute coordinates in the window's client area.
	SE_MOUSE_LEAVE,	   // evValue and evValue2 are meaninless, this indicates the mouse has left the client area.
	SE_JOYSTICK,	   // evValue is an axis number and evValue2 is the current state (-127 to 127)
	SE_CONSOLE		   // evPtr is a char*, from typing something at a non-game console
};

enum sys_mEvents {
	M_ACTION1,
	M_ACTION2,
	M_ACTION3,
	M_ACTION4,
	M_ACTION5,
	M_ACTION6,
	M_ACTION7,
	M_ACTION8,
	// DG: support some more mouse buttons
	M_ACTION9,
	M_ACTION10,
	M_ACTION11,
	M_ACTION12,
	M_ACTION13,
	M_ACTION14,
	M_ACTION15,
	M_ACTION16,
	// DG end
	M_DELTAX,
	M_DELTAY,
	M_DELTAZ,
	M_INVALID
};

enum sys_jEvents {
	J_ACTION1,
	J_ACTION2,
	J_ACTION3,
	J_ACTION4,
	J_ACTION5,
	J_ACTION6,
	J_ACTION7,
	J_ACTION8,
	J_ACTION9,
	J_ACTION10,
	J_ACTION11,
	J_ACTION12,
	J_ACTION13,
	J_ACTION14,
	J_ACTION15,
	J_ACTION16,
	J_ACTION17,
	J_ACTION18,
	J_ACTION19,
	J_ACTION20,
	J_ACTION21,
	J_ACTION22,
	J_ACTION23,
	J_ACTION24,
	J_ACTION25,
	J_ACTION26,
	J_ACTION27,
	J_ACTION28,
	J_ACTION29,
	J_ACTION30,
	J_ACTION31,
	J_ACTION32,
	J_ACTION_MAX = J_ACTION32,

	J_AXIS_MIN,
	J_AXIS_LEFT_X	  = J_AXIS_MIN + AXIS_LEFT_X,
	J_AXIS_LEFT_Y	  = J_AXIS_MIN + AXIS_LEFT_Y,
	J_AXIS_RIGHT_X	  = J_AXIS_MIN + AXIS_RIGHT_X,
	J_AXIS_RIGHT_Y	  = J_AXIS_MIN + AXIS_RIGHT_Y,
	J_AXIS_LEFT_TRIG  = J_AXIS_MIN + AXIS_LEFT_TRIG,
	J_AXIS_RIGHT_TRIG = J_AXIS_MIN + AXIS_RIGHT_TRIG,

	J_AXIS_MAX = J_AXIS_MIN + MAX_JOYSTICK_AXIS - 1,

	J_DPAD_UP,
	J_DPAD_DOWN,
	J_DPAD_LEFT,
	J_DPAD_RIGHT,

	MAX_JOY_EVENT
};

/*
================================================
The first part of this table maps directly to Direct Input scan codes (DIK_* from dinput.h)
But they are duplicated here for console portability
================================================
*/
enum keyNum_t {
	K_NONE,

	K_ESCAPE,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_6,
	K_7,
	K_8,
	K_9,
	K_0,
	K_MINUS,
	K_EQUALS,
	K_BACKSPACE,
	K_TAB,
	K_Q,
	K_W,
	K_E,
	K_R,
	K_T,
	K_Y,
	K_U,
	K_I,
	K_O,
	K_P,
	K_LBRACKET,
	K_RBRACKET,
	K_ENTER,
	K_LCTRL,
	K_A,
	K_S,
	K_D,
	K_F,
	K_G,
	K_H,
	K_J,
	K_K,
	K_L,
	K_SEMICOLON,
	K_APOSTROPHE,
	K_GRAVE,
	K_LSHIFT,
	K_BACKSLASH,
	K_Z,
	K_X,
	K_C,
	K_V,
	K_B,
	K_N,
	K_M,
	K_COMMA,
	K_PERIOD,
	K_SLASH,
	K_RSHIFT,
	K_KP_STAR,
	K_LALT,
	K_SPACE,
	K_CAPSLOCK,
	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_NUMLOCK,
	K_SCROLL,
	K_KP_7,
	K_KP_8,
	K_KP_9,
	K_KP_MINUS,
	K_KP_4,
	K_KP_5,
	K_KP_6,
	K_KP_PLUS,
	K_KP_1,
	K_KP_2,
	K_KP_3,
	K_KP_0,
	K_KP_DOT,
	K_OEM_102	 = 0x56, // from dinput: < > | on UK/German keyboards
	K_F11		 = 0x57,
	K_F12		 = 0x58,
	K_F13		 = 0x64,
	K_F14		 = 0x65,
	K_F15		 = 0x66,
	K_KANA		 = 0x70,
	K_ABNT_C1	 = 0x7E, // from dinput: ? on Portugese (Brazilian) keyboards
	K_CONVERT	 = 0x79,
	K_NOCONVERT	 = 0x7B,
	K_YEN		 = 0x7D,
	K_KP_EQUALS	 = 0x8D,
	K_CIRCUMFLEX = 0x90, // this is circumflex on japanese keyboards, ..
	K_PREVTRACK	 = 0x90, // from dinput: .. but also "Previous Track"
	K_AT		 = 0x91,
	K_COLON		 = 0x92,
	K_UNDERLINE	 = 0x93,
	K_KANJI		 = 0x94,
	K_STOP		 = 0x95,
	K_AX		 = 0x96,
	K_UNLABELED	 = 0x97,
	K_NEXTTRACK	 = 0x99, // from dinput
	K_KP_ENTER	 = 0x9C,
	K_RCTRL		 = 0x9D,
	// some more from dinput:
	K_MUTE		 = 0xA0,
	K_CALCULATOR = 0xA1,
	K_PLAYPAUSE	 = 0xA2,
	K_MEDIASTOP	 = 0xA4,
	K_VOLUMEDOWN = 0xAE,
	K_VOLUMEUP	 = 0xB0,
	K_WEBHOME	 = 0xB2,

	K_KP_COMMA	  = 0xB3,
	K_KP_SLASH	  = 0xB5,
	K_PRINTSCREEN = 0xB7, // aka SysRq
	K_RALT		  = 0xB8,
	K_PAUSE		  = 0xC5,
	K_HOME		  = 0xC7,
	K_UPARROW	  = 0xC8,
	K_PGUP		  = 0xC9,
	K_LEFTARROW	  = 0xCB,
	K_RIGHTARROW  = 0xCD,
	K_END		  = 0xCF,
	K_DOWNARROW	  = 0xD0,
	K_PGDN		  = 0xD1,
	K_INS		  = 0xD2,
	K_DEL		  = 0xD3,
	K_LWIN		  = 0xDB,
	K_RWIN		  = 0xDC,
	K_APPS		  = 0xDD,
	K_POWER		  = 0xDE,
	K_SLEEP		  = 0xDF,

	// DG: dinput has some more buttons, let's support them as well
	K_WAKE		   = 0xE3,
	K_WEBSEARCH	   = 0xE5,
	K_WEBFAVORITES = 0xE6,
	K_WEBREFRESH   = 0xE7,
	K_WEBSTOP	   = 0xE8,
	K_WEBFORWARD   = 0xE9,
	K_WEBBACK	   = 0xEA,
	K_MYCOMPUTER   = 0xEB,
	K_MAIL		   = 0xEC,
	K_MEDIASELECT  = 0xED,

	//------------------------
	// K_JOY codes must be contiguous, too
	//------------------------

	K_JOY1 = 256,
	K_JOY2,
	K_JOY3,
	K_JOY4,
	K_JOY5,
	K_JOY6,
	K_JOY7,
	K_JOY8,
	K_JOY9,
	K_JOY10,
	K_JOY11,
	K_JOY12,
	K_JOY13,
	K_JOY14,
	K_JOY15,
	K_JOY16,

	K_JOY_STICK1_UP,
	K_JOY_STICK1_DOWN,
	K_JOY_STICK1_LEFT,
	K_JOY_STICK1_RIGHT,

	K_JOY_STICK2_UP,
	K_JOY_STICK2_DOWN,
	K_JOY_STICK2_LEFT,
	K_JOY_STICK2_RIGHT,

	K_JOY_TRIGGER1,
	K_JOY_TRIGGER2,

	K_JOY_DPAD_UP,
	K_JOY_DPAD_DOWN,
	K_JOY_DPAD_LEFT,
	K_JOY_DPAD_RIGHT,

	// Leyland VR
	K_VR_FIRST_KEY,
	K_VR_LEFT_MENU = K_VR_FIRST_KEY,
	K_VR_LEFT_DPAD_LEFT,
	K_VR_LEFT_DPAD_UP,
	K_VR_LEFT_DPAD_RIGHT,
	K_VR_LEFT_DPAD_DOWN,
	K_VR_LEFT_AXIS,
	K_VR_LEFT_TRIGGER,
	K_VR_LEFT_GRIP,
	K_VR_LEFT_A,

	K_VR_RIGHT_MENU,
	K_VR_RIGHT_DPAD_LEFT,
	K_VR_RIGHT_DPAD_UP,
	K_VR_RIGHT_DPAD_RIGHT,
	K_VR_RIGHT_DPAD_DOWN,
	K_VR_RIGHT_AXIS,
	K_VR_RIGHT_TRIGGER,
	K_VR_RIGHT_GRIP,
	K_VR_RIGHT_A,
	K_VR_LAST_KEY = K_VR_RIGHT_A,
	// Leyland end

	//------------------------
	// K_MOUSE enums must be contiguous (no char codes in the middle)
	//------------------------

	K_MOUSE1,
	K_MOUSE2,
	K_MOUSE3,
	K_MOUSE4,
	K_MOUSE5,
	K_MOUSE6,
	K_MOUSE7,
	K_MOUSE8,

	// DG: add some more mouse buttons
	K_MOUSE9,
	K_MOUSE10,
	K_MOUSE11,
	K_MOUSE12,
	K_MOUSE13,
	K_MOUSE14,
	K_MOUSE15,
	K_MOUSE16,
	// DG end

	K_MWHEELDOWN,
	K_MWHEELUP,

	K_LAST_KEY
};

struct sysEvent_t {
	sysEventType_t evType;
	int			   evValue;
	int			   evValue2;
	int			   evPtrLength; // bytes of data pointed to by evPtr, for journaling
	void*		   evPtr;		// this must be manually freed if not NULL

	int			   inputDevice;

	//! Checks if the system event is a key event.
	bool		   IsKeyEvent() const { return evType == SE_KEY; }

	//! Returns true if the system event is a mouse event.
	bool		   IsMouseEvent() const { return evType == SE_MOUSE; }

	//! Returns true if the system event is a mouse absolute event.
	bool		   IsMouseAbsoluteEvent() const { return evType == SE_MOUSE_ABSOLUTE; }

	//! Returns true if the system event is a character event.
	bool		   IsCharEvent() const { return evType == SE_CHAR; }

	//! Returns true if the system event is a joystick event.
	bool		   IsJoystickEvent() const { return evType == SE_JOYSTICK; }

	//! Returns true if the system event represents a key press.
	bool		   IsKeyDown() const { return evValue2 != 0; }

	//! Returns true if the key is in the up state.
	bool		   IsKeyUp() const { return evValue2 == 0; }

	//! Returns the key value stored in the system event
	keyNum_t	   GetKey() const { return static_cast<keyNum_t>( evValue ); }

	//! Returns the X coordinate value stored in the system event.
	int			   GetXCoord() const { return evValue; }

	//! Returns the Y coordinate value stored in the system event.
	int			   GetYCoord() const { return evValue2; }
};

struct sysMemoryStats_t {
	int memoryLoad;
	int totalPhysical;
	int availPhysical;
	int totalPageFile;
	int availPageFile;
	int totalVirtual;
	int availVirtual;
	int availExtendedVirtual;
};

//! Initializes the system components.
void		Sys_Init();

//! Shuts down the system by clearing base and save paths and performing POSIX shutdown.
void		Sys_Shutdown();

//! Outputs an error message and terminates the application.
void		Sys_Error( const char* error, ... );

//! Returns the command line used to start the application.
const char* Sys_GetCmdLine();

//! Restarts the application in a new process with additional command-line arguments.
void		Sys_ReLaunch();

/*!
	\brief Launches an external process using the provided path and arguments.

	This function creates a new process by using the Windows API CreateProcess function. It constructs a command line string by combining the executable path with the provided arguments. If the
   process creation fails, an error message is logged and the function returns. The function also appends a quit command to the command system to terminate the current process.

	\param path The path to the executable to launch.
	\param args Command line arguments to pass to the launched process.
	\param launchData Additional data to be used in launching the process.
	\param launchDataSize Size of the launch data in bytes.
	\throws idLib::Error when the process cannot be started.
*/
void		Sys_Launch( const char* path, idCmdArgs& args, void* launchData, unsigned int launchDataSize );

//! Sets the system language from the default language configuration.
void		Sys_SetLanguageFromSystem();

//! Returns the default language string based on available language files and system settings.
const char* Sys_DefaultLanguage();

//! Exits the application with a successful status code.
void		Sys_Quit();

//! Returns true if there is a copy of D3 running already
bool		Sys_AlreadyRunning();

//! Retrieves the current text from the system clipboard and returns a copy of it
char*		Sys_GetClipboardData();

//! Sets the clipboard contents to the provided string.
void		Sys_SetClipboardData( const char* string );

// will go to the various text consoles
// NOT thread safe - never use in the async paths
void		Sys_Printf( VERIFY_FORMAT_STRING const char* msg, ... );

// guaranteed to be thread-safe
void		Sys_DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... );

//! Outputs a formatted debug message to the console or Android log.
void		Sys_DebugVPrintf( const char* fmt, va_list arg );

// a decent minimum sleep time to avoid going below the process scheduler speeds
#define SYS_MINSLEEP 20

//! Pauses execution for the specified number of milliseconds
void		Sys_Sleep( int msec );

//! Returns the number of milliseconds elapsed since the system started.
int			Sys_Milliseconds();
uint64		Sys_Microseconds();

//! Retrieves the current value of the CPU's performance counter for accurate timing measurements.
double		Sys_GetClockTicks();

//! Returns the measured CPU frequency in ticks per second.
double		Sys_ClockTicksPerSecond();

//! Returns a selection of the CPUID flags indicating the generic processor type
cpuid_t		Sys_GetProcessorId();

//! Returns a string identifier for the processor architecture.
const char* Sys_GetProcessorString();

//! Returns true if the FPU stack is empty.
bool		Sys_FPU_StackIsEmpty();

//! Empties the FPU stack.
void		Sys_FPU_ClearStack();

//! Returns a string representation of the current FPU state.
const char* Sys_FPU_GetState();

//! Enables the specified FPU exceptions.
void		Sys_FPU_EnableExceptions( int exceptions );

//! Sets the FPU precision to the specified value.
void		Sys_FPU_SetPrecision( int precision );

//! Sets the FPU rounding mode.
void		Sys_FPU_SetRounding( int rounding );

//! Sets the Flush-To-Zero mode for the FPU based on the enable parameter.
void		Sys_FPU_SetFTZ( bool enable );

//! Sets the Denormals-Are-Zero mode for the FPU based on the enable parameter
void		Sys_FPU_SetDAZ( bool enable );

//! Returns the amount of free space in megabytes on the drive containing the specified path
int			Sys_GetDriveFreeSpace( const char* path );

//! Returns the amount of free space in bytes available on the drive containing the specified path
int64		Sys_GetDriveFreeSpaceInBytes( const char* path );

//! Returns memory statistics.
void		Sys_GetCurrentMemoryStatus( sysMemoryStats_t& stats );

//! Returns placeholder information about executable launch memory status.
void		Sys_GetExeLaunchMemoryStatus( sysMemoryStats_t& stats );

//! Locks a specified block of memory in place
bool		Sys_LockMemory( void* ptr, int bytes );

//! Unlocks a previously locked memory region.
bool		Sys_UnlockMemory( void* ptr, int bytes );

//! Sets the amount of physical work memory to be used by the system.
void		Sys_SetPhysicalWorkMemory( int minBytes, int maxBytes );

//! Loads a dynamic link library from the specified file path and returns a handle to it
intptr_t	Sys_DLL_Load( const char* dllName );

//! Retrieves the address of a symbol from a dynamically loaded library.
void*		Sys_DLL_GetProcAddress( intptr_t dllHandle, const char* procName );

//! Unloads a dynamic library using the provided handle.
void		Sys_DLL_Unload( intptr_t dllHandle );

//! Generates system events including console input and SDL events.
void		Sys_GenerateEvents();

//! Retrieves the next system event from the event queue.
sysEvent_t	Sys_GetEvent();

//! Clears all pending system events from the event queue.
void		Sys_ClearEvents();

//! Initializes the input system including keyboard, mouse, and game controller support.
void		Sys_InitInput();

//! Shuts down the input system by clearing all input polls and closing the SDL joystick if opened.
void		Sys_ShutdownInput();

//! Returns the number of keyboard input events that have been polled.
int			Sys_PollKeyboardInputEvents();

//! Retrieves a keyboard input event from the polling buffer.
int			Sys_ReturnKeyboardInputEvent( const int n, int& ch, bool& state );

//! Ends keyboard input event processing by clearing the keyboard polls array.
void		Sys_EndKeyboardInputEvents();

// DG: currently this is only used by idKeyInput::LocalizedKeyName() for !windows
#ifndef _WIN32

//! Returns a human readable name for a key using the current keyboard layout
const char* Sys_GetKeyName( keyNum_t keynum );
#endif
// DG end

// mouse input polling
static const int MAX_MOUSE_EVENTS = 256;

//! Retrieves and clears the current mouse input events, returning the number of events processed.
int				 Sys_PollMouseInputEvents( int mouseEvents[MAX_MOUSE_EVENTS][2] );

//! Sets the rumble intensity for a specified input device.
void			 Sys_SetRumble( int device, int low, int hi );

//! Returns the number of joystick input events for the specified device.
int				 Sys_PollJoystickInputEvents( int deviceNum );

//! Returns joystick input event data for the specified index
int				 Sys_ReturnJoystickInputEvent( const int n, int& action, int& value );

//! Clears the joystick event container after all events have been processed.
void			 Sys_EndJoystickInputEvents();

//! Grabs or releases the mouse cursor based on the grabIt parameter.
void			 Sys_GrabMouseCursor( bool grabIt );

//! Shows or hides the application window based on the specified boolean value.
void			 Sys_ShowWindow( bool show );

//! Checks whether the application window is visible.
bool			 Sys_IsWindowVisible();

//! Displays or hides the console window based on the visibility level and quit behavior.
void			 Sys_ShowConsole( int visLevel, bool quitOnClose );

// This really isn't the right place to have this, but since this is the 'top level' include
// and has a function signature with 'FILE' in it, it kinda needs to be here =/

// RB begin
#if defined( _WIN32 )
typedef HANDLE idFileHandle;
#else
typedef FILE* idFileHandle;
#endif

//! Returns the last modification time of a file handle.
ID_TIME_T	Sys_FileTimeStamp( idFileHandle fp );

//! Converts a timestamp to a formatted date and time string based on the system language setting.
const char* Sys_TimeStampToStr( ID_TIME_T timeStamp );

//! Converts a time duration in seconds into a formatted string representation.
const char* Sys_SecToStr( int sec );

//! Returns the default base path for the application by checking multiple potential locations.
const char* Sys_DefaultBasePath();

//! Returns the default save path for the application.
const char* Sys_DefaultSavePath();

//! Sets a fatal error message that will be preserved during shutdown.
void		Sys_SetFatalError( const char* error );

// Execute the specified process and wait until it's done, calling workFn every waitMS milliseconds.
// If showOutput == true, std IO from the executed process will be output to the console.
// Note that the return value is not an indication of the exit code of the process, but is false
// only if the process could not be created at all. If you wish to check the exit code of the
// spawned process, check the value returned in exitCode.
typedef bool ( *execProcessWorkFunction_t )();
typedef void ( *execOutputFunction_t )( const char* text );

/*!
	\brief Executes an external process with specified parameters and optional output handling.

	This function creates and runs an external process using the Windows CreateProcess API. It sets up standard input and output handles through pipes for capturing process output and allows for
   optional work function callbacks during execution. The function supports waiting for process completion with a timeout, reporting exit codes, and handling command line arguments with proper quoting
   for paths containing spaces. The output function is called to report execution details and process output, while the work function can be used to provide progress updates or abort conditions.
   Memory for command line buffers is allocated from the temporary memory pool.

	\param appPath Path to the executable file to run
	\param workingPath Working directory for the process
	\param args Command line arguments to pass to the executable
	\param workFn Optional function called periodically during process execution
	\param outputFn Function to handle output messages from the process
	\param waitMS Timeout in milliseconds to wait for process completion, -1 to not wait, INFINITE to block indefinitely
	\param exitCode Output parameter receiving the process exit code
	\return True if the process was successfully started, false if creation failed
*/
bool Sys_Exec( const char* appPath, const char* workingPath, const char* args, execProcessWorkFunction_t workFn, execOutputFunction_t outputFn, const int waitMS, unsigned int& exitCode );

// localization

#define ID_LANG_ENGLISH	 "english"
#define ID_LANG_FRENCH	 "french"
#define ID_LANG_ITALIAN	 "italian"
#define ID_LANG_GERMAN	 "german"
#define ID_LANG_SPANISH	 "spanish"
#define ID_LANG_JAPANESE "japanese"

//! Returns the number of supported languages.
int			Sys_NumLangs();

//! Returns the language name at the specified index.
const char* Sys_Lang( int idx );

/*
==============================================================

	Networking

==============================================================
*/

typedef enum {
	NA_BAD, // an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP
} netadrtype_t;

typedef struct {
	netadrtype_t   type;
	unsigned char  ip[4];
	unsigned short port;
} netadr_t;

#define PORT_ANY -1

/*!
	\class idUDP
	\brief idUDP provides a network interface for sending and receiving UDP packets with configurable port binding and error handling.

	The idUDP class encapsulates UDP socket functionality for network communication, allowing initialization on specific ports, sending and receiving packets, and managing socket state. It supports
   both blocking and non-blocking packet retrieval with timeout handling. The class maintains internal tracking of packet statistics and provides methods to check socket validity and configure silent
   mode for debugging. Memory management is handled internally with no explicit ownership semantics, and the class is designed for use in network communication scenarios where reliable packet delivery
   and address tracking are required.

*/
class idUDP
{
public:
	//! Initializes the UDP socket with default values.
	idUDP();

	//! Destroys the UDP object and closes any open connections.
	virtual ~idUDP();

	//! Initializes the UDP socket for the specified port number and returns true if successful.
	bool	 InitForPort( int portNumber );

	//! Returns the port number that this UDP instance is bound to
	int		 GetPort() const { return bound_to.port; }

	//! Returns the network address the UDP socket is bound to.
	netadr_t GetAdr() const { return bound_to; }

	//! Returns the bound IP address as a 32-bit unsigned integer.
	uint32	 GetUIntAdr() const { return ( bound_to.ip[0] | bound_to.ip[1] << 8 | bound_to.ip[2] << 16 | bound_to.ip[3] << 24 ); }

	//! Closes the UDP socket if it is open
	void	 Close();

	/*!
		\brief Receives a UDP packet from the network socket

		This function retrieves a UDP packet from the initialized network socket. It populates the provided address structure with the sender's network address and copies the packet data into the
	   provided buffer. The function updates the size parameter with the actual received packet size. It returns false if no packet is available or if the packet is larger than the provided maximum
	   size, and true on successful reception. The function tracks the number of packets read and total bytes read for monitoring purposes.

		\param from Reference to a netadr_t structure that will be filled with the sender's network address
		\param data Pointer to the buffer where the packet data will be copied
		\param size Reference to an integer that specifies the maximum size of the buffer and is updated with the actual received size
		\param maxSize Maximum allowed size of the packet to be received
		\return true if a packet was successfully received and stored, false otherwise
	*/
	bool	 GetPacket( netadr_t& from, void* data, int& size, int maxSize );

	/*!
		\brief Attempts to receive a network packet in a blocking manner with a specified timeout

		This function waits for incoming network data with a specified timeout before attempting to retrieve a packet. It first checks if data is available using Net_WaitForData, and if data is
	   present, it retrieves the packet using GetPacket. The function returns false if no data is available within the timeout period or if the packet retrieval fails

		\param from Reference to store the source address of the received packet
		\param data Buffer to store the received packet data
		\param size Reference to store the actual size of the received packet
		\param maxSize Maximum size of the data buffer
		\param timeout Timeout value in milliseconds to wait for data
		\return True if a packet was successfully received, false otherwise
	*/
	bool	 GetPacketBlocking( netadr_t& from, void* data, int& size, int maxSize, int timeout );

	//! Sends a network packet to the specified address.
	void	 SendPacket( const netadr_t to, const void* data, int size );

	//! Sets the silent state of the UDP instance.
	void	 SetSilent( bool silent ) { this->silent = silent; }

	//! Returns the silent state of the UDP socket.
	bool	 GetSilent() const { return silent; }

	int		 packetsRead;
	int		 bytesRead;

	int		 packetsWritten;
	int		 bytesWritten;

	//! Checks if the UDP socket is open and valid for network communication.
	bool	 IsOpen() const { return netSocket > 0; }

private:
	netadr_t bound_to;	// interface and port
	int		 netSocket; // OS specific socket
	bool	 silent;	// don't emit anything ( black hole )
};

//! Converts a string representation of a network address to a netadr_t structure.
bool		Sys_StringToNetAdr( const char* s, netadr_t* a, bool doDNSResolve );

//! Converts a network address to a string representation
const char* Sys_NetAdrToString( const netadr_t a );

//! Determines if the given network address belongs to a local area network.
bool		Sys_IsLANAddress( const netadr_t a );

//! Compares two network addresses for equality, ignoring their port numbers
bool		Sys_CompareNetAdrBase( const netadr_t a, const netadr_t b );

//! Returns the number of local network interfaces available.
int			Sys_GetLocalIPCount();

//! Returns the local IP address at the specified index.
const char* Sys_GetLocalIP( int i );

//! Initializes the networking subsystem for the platform
void		Sys_InitNetworking();

//! Shuts down the networking subsystem.
void		Sys_ShutdownNetworking();

/*!
	\class idJoystick
	\brief Manages joystick input device control and event handling.

	This class provides an interface for joystick input device management, including initialization, shutdown, activation, and input event polling. It supports rumble effects and handles the retrieval
   of input events from joystick devices. The class is designed to be implemented by platform-specific system code to provide full joystick support.

*/
class idJoystick
{
public:
	virtual ~idJoystick() { }

	//! Initializes the joystick input device and returns true if successful.
	virtual bool Init() { return false; }

	//! Shuts down the joystick system
	virtual void Shutdown() { }

	//! Deactivates the joystick input device.
	virtual void Deactivate() { }

	//! Sets the rumble effect for a specified joystick device with low and high rumble intensities.
	virtual void SetRumble( int deviceNum, int rumbleLow, int rumbleHigh ) { }

	//! Polls input events from a specified joystick device and returns the number of events collected.
	virtual int	 PollInputEvents( int inputDeviceNum ) { return 0; }

	//! Returns input events from the joystick
	virtual int	 ReturnInputEvent( const int n, int& action, int& value ) { return 0; }

	//! Finalizes input event processing for the joystick.
	virtual void EndInputEvents() { }
};

/*!
	\class idSys
	\brief System abstraction layer providing low-level platform-specific functionality.

	This class serves as a system abstraction layer that provides an interface to platform-specific functionality. It encapsulates operations related to debugging output, timing, processor
   identification, FPU state management, memory locking, dynamic library loading, system events, and process control. The interface is designed to be implemented by platform-specific derived classes
   to provide concrete functionality for different operating systems. All methods are pure virtual, indicating that concrete implementations must be provided by derived classes to support the system's
   operation across different platforms.

*/
class idSys
{
public:
	virtual void		DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	virtual void		DebugVPrintf( const char* fmt, va_list arg )			 = 0;

	virtual double		GetClockTicks()			  = 0;
	virtual double		ClockTicksPerSecond()	  = 0;
	virtual cpuid_t		GetProcessorId()		  = 0;
	virtual const char* GetProcessorString()	  = 0;
	virtual const char* FPU_GetState()			  = 0;
	virtual bool		FPU_StackIsEmpty()		  = 0;
	virtual void		FPU_SetFTZ( bool enable ) = 0;
	virtual void		FPU_SetDAZ( bool enable ) = 0;

	virtual void		FPU_EnableExceptions( int exceptions ) = 0;

	virtual bool		LockMemory( void* ptr, int bytes )	 = 0;
	virtual bool		UnlockMemory( void* ptr, int bytes ) = 0;

	virtual int			DLL_Load( const char* dllName )										  = 0;
	virtual void*		DLL_GetProcAddress( int dllHandle, const char* procName )			  = 0;
	virtual void		DLL_Unload( int dllHandle )											  = 0;
	virtual void		DLL_GetFileName( const char* baseName, char* dllName, int maxLength ) = 0;

	virtual sysEvent_t	GenerateMouseButtonEvent( int button, bool down ) = 0;
	virtual sysEvent_t	GenerateMouseMoveEvent( int deltax, int deltay )  = 0;

	virtual void		OpenURL( const char* url, bool quit )		   = 0;
	virtual void		StartProcess( const char* exePath, bool quit ) = 0;
};

extern idSys* sys;

bool		  Sys_LoadOpenAL();
void		  Sys_FreeOpenAL();

#endif /* !__SYS_PUBLIC__ */
