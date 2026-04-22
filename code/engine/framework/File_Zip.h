/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __FILE_ZIP_H__
#define __FILE_ZIP_H__

/*
==============================================================

  Zip containers based off idResourceContainer and dhewm3

==============================================================
*/
#define MAX_ZIPPED_FILE_NAME 2048

class idZipContainer;

/*!
	\class idZipCacheEntry
	\brief A cache entry for storing zip file data.
*/
class idZipCacheEntry
{
public:
	//! Constructs a new idZipCacheEntry object and initializes it by clearing its contents.
	idZipCacheEntry() { Clear(); }

	//! Clears all members of the zip cache entry
	void Clear()
	{
		filename.Empty();
		offset = 0;
		length = 0;
		owner  = NULL;
	}

	// part of .pk4 file format
	idStrStatic<MAX_ZIPPED_FILE_NAME> filename;
	ZPOS64_T						  offset;
	ZPOS64_T						  length; // uncompressed size

	// helpers only in memory
	idZipContainer*					  owner;
};

/*!
	\class idZipContainer
	\brief A container for managing zip file resources and providing access to compressed files.

	The idZipContainer class provides functionality for opening, parsing, and managing zip files. It maintains a cache of file resources and offers methods to initialize the container, open files from
   within the zip, and retrieve metadata about the container's contents. The class handles the lifecycle of the zip file handle and ensures proper cleanup through its destructor. This container is
   designed to provide efficient access to compressed file resources while maintaining an internal cache for performance optimization.

*/
class idZipContainer
{
	friend class idFileSystemLocal;

public:
	//! Initializes a new instance of the idZipContainer class with default values.
	idZipContainer()
	{
		zipFileHandle	 = NULL;
		checksum		 = 0;
		numFileResources = 0;
	}

	//! Destructor for idZipContainer that closes the zip file handle and clears the cache table.
	~idZipContainer()
	{
		unzClose( zipFileHandle );
		cacheTable.Clear();
	}

	//! Initializes the zip container by opening and parsing the specified zip file.
	bool		  Init( const char* fileName );

	//! Opens a file from a zip container and returns a handle to it
	idFile_InZip* OpenFile( const idZipCacheEntry& rt, const char* relativePath );

	//! Returns the file name stored in the zip container.
	const char*	  GetFileName() const { return fileName.c_str(); }

	//! Returns the number of file resources in the zip container.
	int			  GetNumFileResources() const { return numFileResources; }

	//! Returns the checksum value calculated for the zip container.
	int			  GetChecksum() const { return checksum; }

private:
	idStrStatic<256>					  fileName;		 // containst the full OS path unlike idResourcesContainer
	unzFile								  zipFileHandle; // open file handle
	int									  checksum;
	int									  numFileResources; // number of file resources in this container
	idList<idZipCacheEntry, TAG_RESOURCE> cacheTable;
	idHashIndex							  cacheHash;
};

#endif /* !__FILE_ZIP_H__ */
