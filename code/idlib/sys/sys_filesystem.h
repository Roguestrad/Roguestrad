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

/*!
	\brief Creates a directory at the specified path with read, write, and execute permissions for all users.

	This function creates a directory at the given path with permissions set to 0777, which allows read, write, and execute access for the owner, group, and others. It uses the standard C library
   mkdir function to perform the actual directory creation operation.

	\param path The filesystem path where the directory should be created
*/
void Sys_Mkdir( const char* path );

/*!
	\brief Removes a directory at the specified path and returns true if successful.

	This function removes a directory identified by the given path. It returns true if the directory was successfully removed, or false if the operation failed. The implementation uses the standard
   system call rmdir and checks its return value to determine success.

	\param path The absolute or relative path to the directory that should be removed
	\return True if the directory was successfully removed, false otherwise
*/
bool Sys_Rmdir( const char* path );

/*!
	\brief Checks if a file is writable by verifying the write permission bit in its file mode

	This function determines if a file is writable by calling the stat system function to retrieve file information and then checking if the write permission bit is set in the file's mode. If the file
   cannot be accessed, the function returns true, indicating it is writable. This behavior may be specific to the implementation context where the function is used

	\param path The path to the file being checked for writability
	\return True if the file is writable or if the file cannot be accessed, false otherwise
*/
bool Sys_IsFileWritable( const char* path );

enum sysFolder_t { FOLDER_ERROR = -1, FOLDER_NO = 0, FOLDER_YES = 1 };

/*!
	\brief Checks whether the specified path corresponds to a folder

	This function determines if a given file system path refers to a directory or folder. It uses the stat system call to retrieve file information and checks the file mode flags to identify if the
   path represents a directory. The function returns FOLDER_YES if the path is a folder, FOLDER_NO if it is not a folder, and FOLDER_ERROR if the path cannot be accessed or analyzed.

	\param path The file system path to check for folder existence
	\return A sysFolder_t enum value indicating whether the path is a folder, not a folder, or if an error occurred during the check
*/
sysFolder_t Sys_IsFolder( const char* path );

/*!
	\brief Lists files in a directory matching a given extension pattern

	This function enumerates files in the specified directory and adds matching filenames to the provided list. It supports shell-style pattern matching through fnmatch and can filter for directories
   only by passing a forward slash as the extension. The function returns -1 if the directory cannot be opened, and the list is cleared in this case. When fs_debug cvar is enabled, it outputs debug
   information about the number of entries found.

	\param directory Path to the directory to search
	\param extension File extension pattern to match, or '/' to match directories only
	\param list List to append matching filenames to
	\return Number of matching files found, or -1 if directory could not be opened
*/
int			Sys_ListFiles( const char* directory, const char* extension, idList<class idStr>& list );

/*!
	\brief Returns the full path to the executable file currently running.

	This function retrieves the path to the currently executing binary by reading the symbolic link /proc/<pid>/exe on POSIX systems. The result is cached in a static buffer and returned as a
   null-terminated string. If the system call fails, an error message is printed and the buffer is cleared.

	\return A null-terminated string containing the full path to the executable file.
*/
const char* Sys_EXEPath();
const char* Sys_CWD();

const char* Sys_LaunchPath();

#endif
