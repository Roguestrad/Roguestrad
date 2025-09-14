/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2024 Robert Beckebans

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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#pragma hdrstop

#include "../sys/sys_local.h"
#include "../framework/EventLoop.h"
#include "../framework/DeclManager.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <fnmatch.h>

idEventLoop* eventLoop;
idDeclManager* declManager;
idSys* sys = NULL;

#define STDIO_PRINT( pre, post )	\
	va_list argptr;					\
	va_start( argptr, fmt );		\
	printf( pre );					\
	vprintf( fmt, argptr );			\
	printf( post );					\
	printf(post);		\
	va_end( argptr )

idCVar com_productionMode( "com_productionMode", "0", CVAR_SYSTEM | CVAR_BOOL, "0 - no special behavior, 1 - building a production build, 2 - running a production build" );

/*
==============================================================

	idSys

==============================================================
*/

void Sys_Printf( const char* fmt, ... )
{
	va_list argptr;

	//tty_Hide();
	va_start( argptr, fmt );
	vprintf( fmt, argptr );
	va_end( argptr );
	//tty_Show();
}

/*
==============
Sys_Mkdir
==============
*/
void Sys_Mkdir( const char* path )
{
	mkdir( path, 0777 );
}


/*
========================
Sys_Rmdir
========================
*/
bool Sys_Rmdir( const char* path )
{
	return ( rmdir( path ) == 0 );
}

/*
==============
Sys_EXEPath
==============
*/
const char* Sys_EXEPath()
{
	static char	buf[ 1024 ];
	idStr		linkpath;
	int			len;

	buf[ 0 ] = '\0';
	sprintf( linkpath, "/proc/%d/exe", getpid() );
	len = readlink( linkpath.c_str(), buf, sizeof( buf ) );
	if( len == -1 )
	{
		Sys_Printf( "couldn't stat exe path link %s\n", linkpath.c_str() );
		// RB: fixed array subscript is below array bounds
		buf[ 0 ] = '\0';
		// RB end
	}
	return buf;
}

/*
================
Posix_Cwd
================
*/
const char* Posix_Cwd()
{
	static char cwd[MAX_OSPATH];

	getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH - 1] = 0;

	return cwd;
}

/*
==============
Sys_ListFiles
==============
*/
int Sys_ListFiles( const char* directory, const char* extension, idStrList& list )
{
	struct dirent* d;
	DIR* fdir;
	bool dironly = false;
	char search[MAX_OSPATH];
	struct stat st;
	bool debug;

	list.Clear();

	debug = cvarSystem->GetCVarBool( "fs_debug" );
	// DG: we use fnmatch for shell-style pattern matching
	// so the pattern should at least contain "*" to match everything,
	// the extension will be added behind that (if !dironly)
	idStr pattern( "*" );

	// passing a slash as extension will find directories
	if( extension[0] == '/' && extension[1] == 0 )
	{
		dironly = true;
	}
	else
	{
		// so we have *<extension>, the same as in the windows code basically
		pattern += extension;
	}
	// DG end

	// NOTE: case sensitivity of directory path can screw us up here
	if( ( fdir = opendir( directory ) ) == NULL )
	{
		if( debug )
		{
			common->Printf( "Sys_ListFiles: opendir %s failed\n", directory );
		}
		return -1;
	}

	// DG: use readdir_r instead of readdir for thread safety
	// the following lines are from the readdir_r manpage.. fscking ugly.
	int nameMax = pathconf( directory, _PC_NAME_MAX );
	if( nameMax == -1 )
	{
		nameMax = 255;
	}
	int direntLen = offsetof( struct dirent, d_name ) + nameMax + 1;

	struct dirent* entry = ( struct dirent* )Mem_Alloc( direntLen, TAG_CRAP );

	if( entry == NULL )
	{
		common->Warning( "Sys_ListFiles: Mem_Alloc for entry failed!" );
		closedir( fdir );
		return 0;
	}

	while( readdir_r( fdir, entry, &d ) == 0 && d != NULL )
	{
		// DG end
		idStr::snPrintf( search, sizeof( search ), "%s/%s", directory, d->d_name );
		if( stat( search, &st ) == -1 )
		{
			continue;
		}
		if( !dironly )
		{
			// DG: the original code didn't work because d3 bfg abuses the extension
			// to match whole filenames and patterns in the savegame-code, not just file extensions...
			// so just use fnmatch() which supports matching shell wildcard patterns ("*.foo" etc)
			// if we should ever need case insensitivity, use FNM_CASEFOLD as third flag
			if( fnmatch( pattern.c_str(), d->d_name, 0 ) != 0 )
			{
				continue;
			}
			// DG end
		}
		if( ( dironly && !( st.st_mode & S_IFDIR ) ) ||
				( !dironly && ( st.st_mode & S_IFDIR ) ) )
		{
			continue;
		}

		list.Append( d->d_name );
	}

	closedir( fdir );
	Mem_Free( entry );

	if( debug )
	{
		common->Printf( "Sys_ListFiles: %d entries in %s\n", list.Num(), directory );
	}

	return list.Num();
}



int idEventLoop::JournalLevel() const
{
	return 0;
}

/*
========================
Sys_IsFolder
========================
*/
sysFolder_t Sys_IsFolder( const char* path )
{
	struct stat buffer;

	if( stat( path, &buffer ) < 0 )
	{
		return FOLDER_ERROR;
	}

	return ( buffer.st_mode & S_IFDIR ) != 0 ? FOLDER_YES : FOLDER_NO;
}

const char* Sys_DefaultSavePath()
{
	return "";
}

const char* Sys_Lang( int )
{
	return "";
}


/*
=================
Sys_FileTimeStamp
=================
*/
ID_TIME_T Sys_FileTimeStamp( idFileHandle fp )
{
	struct stat st;
	fstat( fileno( fp ), &st );
	return st.st_mtime;
}

/*
================
Sys_DefaultBasePath

Get the default base path
- binary image path
- current directory
- macOS app bundle resources directory path			// SRS - added macOS app bundle resources path
- build directory path								// SRS - added build directory path
- hardcoded
Try to be intelligent: if there is no BASE_GAMEDIR, try the next path
================
*/
static idStr basepath;

const char* Sys_DefaultBasePath()
{
	struct stat st;
	idStr testbase, exepath = {};
	basepath = Sys_EXEPath();
	if( basepath.Length() )
	{
		exepath = basepath.StripFilename();
		testbase = basepath;
		testbase += "/";
		testbase += BASE_GAMEDIR;
		if( stat( testbase.c_str(), &st ) != -1 && S_ISDIR( st.st_mode ) )
		{
			return basepath.c_str();
		}
		else
		{
			common->Printf( "no '%s' directory in exe path %s, skipping\n", BASE_GAMEDIR, basepath.c_str() );
		}
	}
	if( basepath != Posix_Cwd() )
	{
		basepath = Posix_Cwd();
		testbase = basepath;
		testbase += "/";
		testbase += BASE_GAMEDIR;
		if( stat( testbase.c_str(), &st ) != -1 && S_ISDIR( st.st_mode ) )
		{
			return basepath.c_str();
		}
		else
		{
			common->Printf( "no '%s' directory in cwd path %s, skipping\n", BASE_GAMEDIR, basepath.c_str() );
		}
	}
	if( exepath.Length() )
	{
#if defined(__APPLE__)
		// SRS - Check for macOS app bundle resources path (up one dir level and down to Resources dir)
		basepath = exepath;
		basepath = basepath.StripFilename() + "/Resources";
		testbase = basepath;
		testbase += "/";
		testbase += BASE_GAMEDIR;
		if( stat( testbase.c_str(), &st ) != -1 && S_ISDIR( st.st_mode ) )
		{
			return basepath.c_str();
		}
		else
		{
			common->Printf( "no '%s' directory in macOS app bundle resources path %s, skipping\n", BASE_GAMEDIR, basepath.c_str() );
		}
#endif
		// SRS - Check for linux/macOS build path (directory structure with build dir and possible config suffix)
		basepath = exepath;
		basepath.StripFilename();						// up 1st dir level for single-config dev builds
#if !defined( NO_MULTI_CONFIG )
		basepath.StripFilename();						// up 2nd dir level for multi-config dev builds with Debug/Release/etc suffix
#endif
		testbase = basepath;
		testbase += "/";
		testbase += BASE_GAMEDIR;
		if( stat( testbase.c_str(), &st ) != -1 && S_ISDIR( st.st_mode ) )
		{
			return basepath.c_str();
		}
		else
		{
			common->Printf( "no '%s' directory in build path %s, skipping\n", BASE_GAMEDIR, basepath.c_str() );
		}
	}
	common->Printf( "WARNING: using hardcoded default base path %s\n", DEFAULT_BASEPATH );
	return DEFAULT_BASEPATH;
}

int Sys_NumLangs()
{
	return 0;
}

/*
================
Sys_Milliseconds
================
*/
/* base time in seconds, that's our origin
   timeval:tv_sec is an int:
   assuming this wraps every 0x7fffffff - ~68 years since the Epoch (1970) - we're safe till 2038
   using unsigned long data type to work right with Sys_XTimeToSysTime */

#ifdef CLOCK_MONOTONIC_RAW
	// use RAW monotonic clock if available (=> not subject to NTP etc)
	#define D3_CLOCK_TO_USE CLOCK_MONOTONIC_RAW
#else
	#define D3_CLOCK_TO_USE CLOCK_MONOTONIC
#endif

// RB: changed long to int
unsigned int sys_timeBase = 0;
// RB end
/* current time in ms, using sys_timeBase as origin
   NOTE: sys_timeBase*1000 + curtime -> ms since the Epoch
     0x7fffffff ms - ~24 days
		 or is it 48 days? the specs say int, but maybe it's casted from unsigned int?
*/
int Sys_Milliseconds()
{
	// DG: use clock_gettime on all platforms
	int curtime;
	struct timespec ts;

	clock_gettime( D3_CLOCK_TO_USE, &ts );

	if( !sys_timeBase )
	{
		sys_timeBase = ts.tv_sec;
		return ts.tv_nsec / 1000000;
	}

	curtime = ( ts.tv_sec - sys_timeBase ) * 1000 + ts.tv_nsec / 1000000;

	return curtime;
	// DG end
}

/*
==============================================================

	idCommon

==============================================================
*/
class idCommonLocal : public idCommon
{
public:

	// Initialize everything.
	// if the OS allows, pass argc/argv directly (without executable name)
	// otherwise pass the command line in a single string (without executable name)
	virtual void				Init( int argc, const char* const* argv, const char* cmdline ) {}

	// Shuts down everything.
	virtual void				Shutdown() {}
	virtual bool				IsShuttingDown() const
	{
		return false;
	};

	virtual	void				CreateMainMenu() {}

	// Shuts down everything.
	virtual void				Quit() {}

	// Returns true if common initialization is complete.
	virtual bool				IsInitialized() const
	{
		return true;
	};

	// Called repeatedly as the foreground thread for rendering and game logic.
	virtual void				Frame() {}

	// Redraws the screen, handling games, guis, console, etc
	// in a modal manner outside the normal frame loop
	virtual void				UpdateScreen( bool captureToImage, bool releaseMouse = true ) {}

	virtual void				UpdateLevelLoadPacifier() {}
	virtual void				LoadPacifierInfo( VERIFY_FORMAT_STRING const char* fmt, ... ) {}
	virtual void				LoadPacifierProgressTotal( int total ) {}
	virtual void				LoadPacifierProgressIncrement( int step ) {}
	virtual bool				LoadPacifierRunning()
	{
		return false;
	}


	// Checks for and removes command line "+set var arg" constructs.
	// If match is NULL, all set commands will be executed, otherwise
	// only a set with the exact name.
	virtual void				StartupVariable( const char* match ) {}

	// Begins redirection of console output to the given buffer.
	virtual void				BeginRedirect( char* buffer, int buffersize, void ( *flush )( const char* ) ) {}

	// Stops redirection of console output.
	virtual void				EndRedirect() {}

	// Update the screen with every message printed.
	virtual void				SetRefreshOnPrint( bool set ) {}

	virtual void			Printf( const char* fmt, ... )
	{
		STDIO_PRINT( "", "" );
	}
	virtual void			VPrintf( const char* fmt, va_list arg )
	{
		vprintf( fmt, arg );
	}
	virtual void			DPrintf( const char* fmt, ... )
	{
		/*STDIO_PRINT( "", "" );*/
	}
	virtual void			VerbosePrintf( const char* fmt, ... )
	{
		/*STDIO_PRINT( "", "" );*/
	}
	virtual void			Warning( const char* fmt, ... )
	{
		STDIO_PRINT( "WARNING: ", "\n" );
	}
	virtual void			DWarning( const char* fmt, ... )
	{
		/*STDIO_PRINT( "WARNING: ", "\n" );*/
	}

	// Prints all queued warnings.
	virtual void				PrintWarnings() {}

	// Removes all queued warnings.
	virtual void				ClearWarnings( const char* reason ) {}

	virtual void			Error( const char* fmt, ... )
	{
		STDIO_PRINT( "ERROR: ", "\n" );
		exit( 0 );
	}
	virtual void			FatalError( const char* fmt, ... )
	{
		STDIO_PRINT( "FATAL ERROR: ", "\n" );
		exit( 0 );
	}

	// Returns key bound to the command
	virtual const char* KeysFromBinding( const char* bind )
	{
		return NULL;
	};

	// Returns the binding bound to the key
	virtual const char* BindingFromKey( const char* key )
	{
		return NULL;
	};

	// Directly sample a button.
	virtual int					ButtonState( int key )
	{
		return 0;
	};

	// Directly sample a keystate.
	virtual int					KeyState( int key )
	{
		return 0;
	};

	// Returns true if a multiplayer game is running.
	// CVars and commands are checked differently in multiplayer mode.
	virtual bool				IsMultiplayer()
	{
		return false;
	};
	virtual bool				IsServer()
	{
		return false;
	};
	virtual bool				IsClient()
	{
		return false;
	};

	// Returns true if the player has ever enabled the console
	virtual bool				GetConsoleUsed()
	{
		return false;
	};

	// Returns the rate (in ms between snaps) that we want to generate snapshots
	virtual int					GetSnapRate()
	{
		return 0;
	};

	virtual void				NetReceiveReliable( int peer, int type, idBitMsg& msg ) { };
	virtual void				NetReceiveSnapshot( class idSnapShot& ss ) { };
	virtual void				NetReceiveUsercmds( int peer, idBitMsg& msg ) { };

	// Processes the given event.
	virtual	bool				ProcessEvent( const sysEvent_t* event )
	{
		return false;
	};

	virtual bool				LoadGame( const char* saveName )
	{
		return false;
	};
	virtual bool				SaveGame( const char* saveName )
	{
		return false;
	};

	virtual idGame* Game()
	{
		return NULL;
	};
	virtual idRenderWorld* RW()
	{
		return NULL;
	};
	virtual idSoundWorld* SW()
	{
		return NULL;
	};
	virtual idSoundWorld* MenuSW()
	{
		return NULL;
	};
	virtual idSession* Session()
	{
		return NULL;
	};
	virtual idCommonDialog& Dialog()
	{
		static idCommonDialog useless;
		return useless;
	};

	virtual void				OnSaveCompleted( idSaveLoadParms& parms ) {}
	virtual void				OnLoadCompleted( idSaveLoadParms& parms ) {}
	virtual void				OnLoadFilesCompleted( idSaveLoadParms& parms ) {}
	virtual void				OnEnumerationCompleted( idSaveLoadParms& parms ) {}
	virtual void				OnDeleteCompleted( idSaveLoadParms& parms ) {}
	virtual void				TriggerScreenWipe( const char* _wipeMaterial, bool hold ) {}

	virtual void				OnStartHosting( idMatchParameters& parms ) {}

	virtual int					GetGameFrame()
	{
		return 0;
	};

	virtual void				LaunchExternalTitle( int titleIndex, int device, const lobbyConnectInfo_t* const connectInfo ) { };

	virtual void				InitializeMPMapsModes() { };
	virtual const idStrList& GetModeList() const
	{
		static idStrList useless;
		return useless;
	};
	virtual const idStrList& GetModeDisplayList() const
	{
		static idStrList useless;
		return useless;
	};
	virtual const idList<mpMap_t>& GetMapList() const
	{
		static idList<mpMap_t> useless;
		return useless;
	};

	virtual void				ResetPlayerInput( int playerIndex ) {}

	virtual bool				JapaneseCensorship() const
	{
		return false;
	};

	virtual void				QueueShowShell() { };		// Will activate the shell on the next frame.
	virtual void				InitTool( const toolFlag_t, const idDict*, idEntity* ) {}

	virtual void				LoadPacifierBinarizeFilename( const char* filename, const char* reason ) {}
	virtual void				LoadPacifierBinarizeInfo( const char* info ) {}
	virtual void				LoadPacifierBinarizeMiplevel( int level, int maxLevel ) {}
	virtual void				LoadPacifierBinarizeProgress( float progress ) {}
	virtual void				LoadPacifierBinarizeEnd() { };
	virtual void				LoadPacifierBinarizeProgressTotal( int total ) {}
	virtual void				LoadPacifierBinarizeProgressIncrement( int step ) {}

	virtual void				DmapPacifierFilename( const char* filename, const char* reason ) {}
	virtual void				DmapPacifierInfo( VERIFY_FORMAT_STRING const char* fmt, ... ) {}
	virtual void				DmapPacifierCompileProgressTotal( int total ) {}
	virtual void				DmapPacifierCompileProgressIncrement( int step ) {}
};

idCommonLocal		commonLocal;
idCommon* common = &commonLocal;
