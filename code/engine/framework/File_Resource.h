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

#ifndef __FILE_RESOURCE_H__
#define __FILE_RESOURCE_H__

/*
==============================================================

  Resource containers

==============================================================
*/
class idResourceContainer;

/*!
	\class idResourceCacheEntry
	\brief Manages resource cache entries for storing and retrieving cached resource data.
*/
class idResourceCacheEntry
{
public:
	//! Initializes a new resource cache entry and clears its contents.
	idResourceCacheEntry() { Clear(); }

	//! Clears all fields of the resource cache entry.
	void Clear()
	{
		filename.Empty();
		offset = 0;
		length = 0;
		owner  = NULL;
	}

	//! Reads resource cache entry data from a file
	size_t Read( idFile* f )
	{
		size_t sz = f->ReadString( filename );
		sz += f->ReadBig( offset );
		sz += f->ReadBig( length );
		return sz;
	}

	//! Writes resource cache entry data to a file
	size_t Write( idFile* f )
	{
		size_t sz = f->WriteString( filename );
		sz += f->WriteBig( offset );
		sz += f->WriteBig( length );
		return sz;
	}

	// part of .resources file format
	idStrStatic<256>	 filename;
	int					 offset; // into the resource file
	int					 length;

	// helpers only in memory
	idResourceContainer* owner;
};

static const uint32 RESOURCE_FILE_MAGIC = 0xD000000D;

/*!
	\class idResourceContainer
	\brief Manages resource containers including file loading, parsing, and extraction.

	The idResourceContainer class provides functionality for handling resource containers, which are files containing organized collections of resources. It supports initialization by loading and
   parsing resource files, opening individual files within the container, and managing the container's lifecycle through opening and closing operations. The class also offers methods for writing new
   resource files and manifest files, as well as reading existing manifest files. It can extract resources from a container file to a specified output path, with support for converting audio files and
   filtering extraction results. Additionally, it provides capabilities for updating resource files by adding or modifying entries.

*/
class idResourceContainer
{
	friend class idFileSystemLocal;
	// friend class	idReadSpawnThread;
public:
	//! Initializes a new instance of the idResourceContainer class.
	idResourceContainer()
	{
		resourceFile	 = NULL;
		tableOffset		 = 0;
		tableLength		 = 0;
		resourceMagic	 = 0;
		numFileResources = 0;
	}

	//! Destructor for idResourceContainer that cleans up resource file and clears the cache table.
	~idResourceContainer()
	{
		delete resourceFile;
		cacheTable.Clear();
	}

	//! Initializes the resource container by loading and parsing the resource file.
	bool		Init( const char* fileName );

	//! Writes a resource file containing the specified manifest entries, split into 1GB chunks if necessary.
	static void WriteResourceFile( const char* fileName, const idStrList& manifest, const bool& _writeManifest );

	//! Writes a manifest file containing a list of resource paths with the specified name.
	static void WriteManifestFile( const char* name, const idStrList& list );

	//! Reads a manifest file and populates a string list with its contents.
	static int	ReadManifestFile( const char* filename, idStrList& list );

	/*!
		\brief Extracts resource files from a container file, optionally converting WAV files and filtering output based on flags

		This function reads a resource container file and extracts its contents to the specified output path. It handles both binary resource files and audio files, with special processing for WAV
	   files that converts them from internal formats to standard WAV files. The function supports filtering of output files through the 'all' parameter and can optionally copy WAV files with
	   additional processing. Progress tracking is enabled when copying WAV files.

		\param fileName Path to the resource container file to extract from
		\param outPath Destination path where extracted files will be written
		\param copyWavs Flag indicating whether to process and convert WAV files
		\param all Flag indicating whether to extract all files or filter out certain categories
	*/
	static void ExtractResourceFile( const char* fileName, const char* outPath, bool copyWavs, bool all );

	//! Updates a resource file with new or modified files.
	static void UpdateResourceFile( const char* filename, const idStrList& filesToAdd );
	idFile*		OpenFile( const char* fileName );

	//! Returns the file name associated with the resource container.
	const char* GetFileName() const { return fileName.c_str(); }

	//! Reopens the resource file by closing the current handle and creating a new one.
	void		ReOpen();

	//! Returns the number of file resources in the container.
	int			GetNumFileResources() const { return numFileResources; }

private:
	idStrStatic<256>						   fileName;
	idFile*									   resourceFile; // open file handle
	// offset should probably be a 64 bit value for development, but 4 gigs won't fit on
	// a DVD layer, so it isn't a retail limitation.
	int										   tableOffset;		 // table offset
	int										   tableLength;		 // table length
	int										   resourceMagic;	 // magic
	int										   numFileResources; // number of file resources in this container
	idList<idResourceCacheEntry, TAG_RESOURCE> cacheTable;
	idHashIndex								   cacheHash;
};

#endif /* !__FILE_RESOURCE_H__ */
