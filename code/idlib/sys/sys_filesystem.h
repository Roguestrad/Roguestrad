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
#ifndef SYS_FILESYSTEM_H
#define SYS_FILESYSTEM_H

//! Creates a directory at the specified path.
void Sys_Mkdir( const char* path );

//! Removes a directory at the specified path
bool Sys_Rmdir( const char* path );

//! Checks if a file is writable by verifying the write permission bits in its metadata.
bool Sys_IsFileWritable( const char* path );

enum sysFolder_t { FOLDER_ERROR = -1, FOLDER_NO = 0, FOLDER_YES = 1 };

//! Returns FOLDER_YES if the specified path is a folder, FOLDER_NO if it is not, and FOLDER_ERROR if an error occurs.
sysFolder_t Sys_IsFolder( const char* path );

//! Lists files in a directory matching a given extension pattern.
int			Sys_ListFiles( const char* directory, const char* extension, idList<class idStr>& list );

//! Returns the absolute path to the executable file.
const char* Sys_EXEPath();
const char* Sys_CWD();

const char* Sys_LaunchPath();

#endif
