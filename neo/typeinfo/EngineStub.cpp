/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall
Copyright (C) 2024-2025 Robert Beckebans

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

#include <shlwapi.h>  // for PathMatchSpecW

#include "../sys/sys_local.h"
#include "../framework/EventLoop.h"
#include "../framework/DeclManager.h"

#include <direct.h>
#include <io.h>

idEventLoop* eventLoop;
idDeclManager* declManager;
idSys* sys = NULL;

#define STDIO_PRINT( pre, post )	\
	va_list argptr;					\
	va_start( argptr, fmt );		\
	printf( pre );					\
	vprintf( fmt, argptr );			\
	printf( post );					\
	OutputDebugStringA(post);		\
	va_end( argptr )

idCVar com_productionMode( "com_productionMode", "0", CVAR_SYSTEM | CVAR_BOOL, "0 - no special behavior, 1 - building a production build, 2 - running a production build" );

/*
==============================================================

	idSys

==============================================================
*/

// RB: Returns absolute path with \\?\ prefix for long path support
wchar_t* MakeWindowsLongPathW( const char* utf8Path )
{
	if( !utf8Path || !utf8Path[0] )
	{
		return NULL;
	}

	int utf16Len = MultiByteToWideChar( CP_UTF8, 0, utf8Path, -1, NULL, 0 );
	if( utf16Len == 0 )
	{
		return NULL;
	}

	// Allocate temporary buffer for initial conversion
	wchar_t* tempPath = ( wchar_t* )malloc( utf16Len * sizeof( wchar_t ) );
	if( !tempPath )
	{
		return NULL;
	}

	// Perform UTF-8 to UTF-16 conversion
	if( MultiByteToWideChar( CP_UTF8, 0, utf8Path, -1, tempPath, utf16Len ) == 0 )
	{
		free( tempPath );
		return NULL;
	}

	// Replace forward slashes with backslashes
	for( int i = 0; tempPath[i]; i++ )
	{
		if( tempPath[i] == L'/' )
		{
			tempPath[i] = L'\\';
		}
	}

	// Convert to absolute path
	wchar_t fullPath[MAX_OSPATH];
	if( GetFullPathNameW( tempPath, MAX_OSPATH, fullPath, NULL ) == 0 )
	{
		free( tempPath );
		return NULL;
	}
	free( tempPath );

	// +4 for \\?\ prefix +1 for null terminator
	size_t fullPathLen = wcslen( fullPath );
	wchar_t* wPath = ( wchar_t* )malloc( ( fullPathLen + 5 ) * sizeof( wchar_t ) );
	if( !wPath )
	{
		return NULL;
	}

	// Add \\?\ prefix for long path support
	swprintf( wPath, fullPathLen + 5, L"\\\\?\\%s", fullPath );
	//wcscpy( wPath, L"\\\\?\\" );
	//wcscat( wPath, fullPath );

	return wPath;
}
// RB end

/*
==============
Sys_Mkdir
==============
*/
void Sys_Mkdir( const char* path )
{
	// RB: support paths longer than 260 characters

	// ignore pure drive-letter paths like "C:"
	if( strlen( path ) == 2 && path[1] == ':' && isalpha( path[0] ) )
	{
		return;
	}

	wchar_t* wPath = MakeWindowsLongPathW( path );
	if( !wPath )
	{
		common->FatalError( "Failed to convert path to wide string: '%s'", path );
		return;
	}

	if( !CreateDirectoryW( wPath, NULL ) )
	{
		DWORD err = GetLastError();

		// don't fatal if directory already exists
		if( err != ERROR_ALREADY_EXISTS )
		{
			common->FatalError( "CreateDirectoryW failed (error %lu)", err );
		}
	}

	free( wPath );
	// RB end
}


/*
========================
Sys_Rmdir
========================
*/
bool Sys_Rmdir( const char* path )
{
	// RB: support paths longer than 260 characters
	wchar_t* wPath = MakeWindowsLongPathW( path );
	if( !wPath )
	{
		common->FatalError( "Failed to convert path to wide string: '%s'", path );
		return false;
	}

	BOOL success = RemoveDirectoryW( wPath );
	if( success == 0 )
	{
		DWORD err = GetLastError();
		common->FatalError( "RemoveDirectoryW failed (error %lu)", err );
	}

	free( wPath );
	return success != 0;
}

/*
==============
Sys_EXEPath
==============
*/
const char* Sys_EXEPath()
{
	static char exe[MAX_OSPATH];
	GetModuleFileName( NULL, exe, sizeof( exe ) - 1 );
	return exe;
}

/*
==============
Sys_ListFiles
==============
*/
static void ListFilesRecursive( const wchar_t* baseDir, const wchar_t* pattern, idStrList& list, size_t baseLen )
{
	wchar_t searchPath[MAX_OSPATH];
	swprintf( searchPath, MAX_OSPATH, L"%s\\*", baseDir );

	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW( searchPath, &findData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do
	{
		if( wcscmp( findData.cFileName, L"." ) == 0 || wcscmp( findData.cFileName, L".." ) == 0 )
		{
			continue;
		}

		wchar_t fullPath[MAX_OSPATH];
		swprintf( fullPath, MAX_OSPATH, L"%s\\%s", baseDir, findData.cFileName );

		bool isDir = ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;

		if( isDir )
		{
			ListFilesRecursive( fullPath, pattern, list, baseLen );
		}
		else if( PathMatchSpecW( findData.cFileName, pattern ) )
		{
			char utf8Name[MAX_OSPATH];
			int len = WideCharToMultiByte( CP_UTF8, 0, fullPath + baseLen, -1, utf8Name, sizeof( utf8Name ), NULL, NULL );
			if( len > 0 )
			{
				list.Append( utf8Name );
			}
		}
	}
	while( FindNextFileW( hFind, &findData ) );

	FindClose( hFind );
}


int Sys_ListFiles( const char* directory, const char* extension, idStrList& list )
{
#if 0
	idStr		search;
	struct _finddata_t findinfo;
	// RB: 64 bit fixes, changed int to intptr_t
	intptr_t	findhandle;
	// RB end
	int			flag;

	if( !extension )
	{
		extension = "";
	}

	// passing a slash as extension will find directories
	if( extension[0] == '/' && extension[1] == 0 )
	{
		extension = "";
		flag = 0;
	}
	else
	{
		flag = _A_SUBDIR;
	}

	sprintf( search, "%s\\*%s", directory, extension );

	// search
	list.Clear();

	findhandle = _findfirst( search, &findinfo );
	if( findhandle == -1 )
	{
		return -1;
	}

	do
	{
		if( flag ^ ( findinfo.attrib & _A_SUBDIR ) )
		{
			list.Append( findinfo.name );
		}
	}
	while( _findnext( findhandle, &findinfo ) != -1 );

	_findclose( findhandle );

	return list.Num();
#else
	// RB: support paths longer than 260 characters
	wchar_t* wDir = MakeWindowsLongPathW( directory );
	if( !wDir )
	{
		common->FatalError( "Failed to convert path to wide string: '%s'", directory );
		return -1;
	}

	idStr extPattern = "*";
	if( extension && extension[0] )
	{
		extPattern += extension;
	}

	// convert extension pattern to wide string
	int extLen = MultiByteToWideChar( CP_UTF8, 0, extPattern.c_str(), -1, NULL, 0 );
	wchar_t* wPattern = ( wchar_t* )malloc( extLen * sizeof( wchar_t ) );
	if( !wPattern )
	{
		free( wDir );
		return -1;
	}
	MultiByteToWideChar( CP_UTF8, 0, extPattern.c_str(), -1, wPattern, extLen );

	idStrList tempList;
	size_t baseLen = wcslen( wDir );
	ListFilesRecursive( wDir, wPattern, tempList, baseLen + 1 ); // +1 to skip path separator

	// convert to forward slashes to tab completion works
	for( auto& s : tempList )
	{
		s.BackSlashesToSlashes();
		list.Append( s.c_str() );
	}

	free( wDir );
	free( wPattern );
	return list.Num();
	// RB end
#endif
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
	wchar_t* wPath = MakeWindowsLongPathW( path );
	if( !wPath )
	{
		return FOLDER_ERROR;
	}

	struct _stat buffer;
	int result = _wstat( wPath, &buffer );
	free( wPath );

	if( result < 0 )
	{
		return FOLDER_ERROR;
	}
	return ( buffer.st_mode & _S_IFDIR ) != 0 ? FOLDER_YES : FOLDER_NO;
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
	FILETIME writeTime;
	GetFileTime( fp, NULL, NULL, &writeTime );

	/*
		FILETIME = number of 100-nanosecond ticks since midnight
		1 Jan 1601 UTC. time_t = number of 1-second ticks since
		midnight 1 Jan 1970 UTC. To translate, we subtract a
		FILETIME representation of midnight, 1 Jan 1970 from the
		time in question and divide by the number of 100-ns ticks
		in one second.
	*/

	SYSTEMTIME base_st =
	{
		1970,   // wYear
		1,      // wMonth
		0,      // wDayOfWeek
		1,      // wDay
		0,      // wHour
		0,      // wMinute
		0,      // wSecond
		0       // wMilliseconds
	};

	FILETIME base_ft;
	SystemTimeToFileTime( &base_st, &base_ft );

	LARGE_INTEGER itime;
	itime.QuadPart = reinterpret_cast<LARGE_INTEGER&>( writeTime ).QuadPart;
	itime.QuadPart -= reinterpret_cast<LARGE_INTEGER&>( base_ft ).QuadPart;
	itime.QuadPart /= 10000000LL;
	return itime.QuadPart;
}

/*
==============
Sys_Cwd
==============
*/
const char* Sys_Cwd()
{
	static char cwd[MAX_OSPATH];

	_getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH - 1] = 0;

	return cwd;
}

/*
==============
Sys_DefaultBasePath
==============
*/
const char* Sys_DefaultBasePath()
{
	return Sys_Cwd();
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
int Sys_Milliseconds()
{
	static DWORD sys_timeBase = timeGetTime();
	return timeGetTime() - sys_timeBase;
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
