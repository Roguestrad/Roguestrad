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

#ifndef __FILE_MANIFEST_H__
#define __FILE_MANIFEST_H__

/*!
	\class idFileManifest
	\brief A class for managing file manifests with caching and hashing capabilities.

	The idFileManifest class provides functionality for loading, storing, and managing file manifest data. It maintains an internal cache table and hash set with a granularity of 4096 for efficient
   file lookups. The class supports loading manifests from files, writing manifests to disk, and retrieving file information by index or name. It also offers methods for adding and removing files from
   the manifest, as well as populating lists with manifest entries. The manifest can be printed to the console for debugging purposes.

*/
class idFileManifest
{
public:
	//! Initializes the file manifest with cache table and hash set to a granularity of 4096.
	idFileManifest()
	{
		cacheTable.SetGranularity( 4096 );
		cacheHash.SetGranularity( 4096 );
	}
	~idFileManifest() { }

	//! Loads a manifest file from the specified file name and returns true if successful
	bool		 LoadManifest( const char* fileName );

	//! Loads a file manifest from the provided file handle and deletes the file upon completion.
	bool		 LoadManifestFromFile( idFile* file );

	//! Writes the manifest file contents to the specified file.
	void		 WriteManifestFile( const char* fileName );

	//! Returns the number of files in the manifest.
	int			 NumFiles() { return cacheTable.Num(); }

	//! Finds the index of a file in the manifest by its name, returning -1 if not found.
	int			 FindFile( const char* fileName );

	//! Returns the file name at the specified index from the manifest's cache table
	const idStr& GetFileNameByIndex( int idx ) const;

	//! Returns the name of the manifest file.
	const char*	 GetManifestName() { return filename; }

	//! Removes all entries with the specified filename from the manifest cache.
	void		 RemoveAll( const char* filename );

	//! Adds a file to the manifest cache.
	void		 AddFile( const char* filename );

	//! Populates the provided list with all entries from the internal cache table.
	void		 PopulateList( idStaticList<idStr, 16384>& dest )
	{
		dest.Clear();
		for( int i = 0; i < cacheTable.Num(); i++ ) {
			dest.Append( cacheTable[i] );
		}
	}

	//! Prints the manifest file names to the console
	void Print()
	{
		idLib::Printf( "dump for manifest %s\n", GetManifestName() );
		idLib::Printf( "---------------------------------------\n" );
		for( int i = 0; i < NumFiles(); i++ ) {
			const idStr& name = GetFileNameByIndex( i );
			if( name.Find( ".idwav", false ) >= 0 ) { idLib::Printf( "%s\n", GetFileNameByIndex( i ).c_str() ); }
		}
	}

private:
	idStrList	cacheTable;
	idHashIndex cacheHash;
	idStr		filename;
};

// image preload
struct imagePreload_s {
	//! Initializes an imagePreload_s structure with default values.
	imagePreload_s()
	{
		filter	= 0;
		repeat	= 0;
		usage	= 0;
		cubeMap = 0;
	}

	//! Writes image preload data to a file
	void Write( idFile* f )
	{
		f->WriteBig( filter );
		f->WriteBig( repeat );
		f->WriteBig( usage );
		f->WriteBig( cubeMap );
	}

	//! Reads image preload data from a file
	void Read( idFile* f )
	{
		f->ReadBig( filter );
		f->ReadBig( repeat );
		f->ReadBig( usage );
		f->ReadBig( cubeMap );
	}

	//! Compares this imagePreload_s object with another for equality.
	bool operator==( const imagePreload_s& b ) const { return ( filter == b.filter && repeat == b.repeat && usage == b.usage && cubeMap == b.cubeMap ); }
	int	 filter;
	int	 repeat;
	int	 usage;
	int	 cubeMap;
};

enum preloadType_t { PRELOAD_IMAGE, PRELOAD_MODEL, PRELOAD_SAMPLE, PRELOAD_ANIM, PRELOAD_COLLISION, PRELOAD_PARTICLE };

// preload
struct preloadEntry_s {
	//! Constructs a preload entry with default resource type set to zero.
	preloadEntry_s() { resType = 0; }

	//! Compares this preload entry with another for equality based on resource name and image data when the resource type is PRELOAD_IMAGE.
	bool operator==( const preloadEntry_s& b ) const
	{
		bool ret = ( resourceName.Icmp( b.resourceName ) == 0 );
		if( ret && resType == PRELOAD_IMAGE ) {
			// this should never matter but...
			ret &= ( imgData == b.imgData );
		}
		return ret;
	}

	//! Writes the preload entry data to the specified output file.
	void Write( idFile* outFile )
	{
		outFile->WriteBig( resType );
		outFile->WriteString( resourceName );
		imgData.Write( outFile );
	}

	//! Reads preload entry data from a file.
	void Read( idFile* inFile )
	{
		inFile->ReadBig( resType );
		inFile->ReadString( resourceName );
		imgData.Read( inFile );
	}

	int			   resType;		 // type
	idStr		   resourceName; // resource name
	imagePreload_s imgData;		 // image specific data
};

struct preloadSort_t {
	int idx;
	int ofs;
};

/*!
	\class idSort_Preload
	\brief A comparator class for sorting preload entries by their offset values.
*/
class idSort_Preload : public idSort_Quick<preloadSort_t, idSort_Preload>
{
public:
	//! Compares two preload sort entries based on their offset values.
	int Compare( const preloadSort_t& a, const preloadSort_t& b ) const { return a.ofs - b.ofs; }
};

/*!
	\class idPreloadManifest
	\brief A manifest for tracking and managing preload resources with various asset types.

	This class provides functionality for loading, storing, and writing preload manifest files that specify which resources should be preloaded. It supports adding different types of resources such as
   models, sounds, particles, animations, collision models, and images with specific properties. The manifest can be loaded from files or written to files for later use. It maintains an internal list
   of preload entries and allows for querying and clearing the contents. The class is designed to work with a specified memory allocation granularity for efficient resource management. The image
   addition function supports detailed configuration including filtering, repetition, usage flags, and cube map settings for proper texture handling.

*/
class idPreloadManifest
{
public:
	//! Initializes a preload manifest object with a specified memory allocation granularity.
	idPreloadManifest() { entries.SetGranularity( 2048 ); }
	~idPreloadManifest() { }

	//! Loads a preload manifest file from the specified file name and returns true if successful.
	bool LoadManifest( const char* fileName );
	bool LoadManifestFromFile( idFile* file );

	//! Writes the preload manifest data to a file with the specified file name.
	void WriteManifest( const char* fileName );

	//! Writes the preload manifest entries to the specified output file
	void WriteManifestToFile( idFile* outFile )
	{
		if( outFile == NULL ) { return; }
		filename = outFile->GetName();
		outFile->WriteBig( ( int )entries.Num() );
		outFile->WriteString( filename );
		for( int i = 0; i < entries.Num(); i++ ) {
			entries[i].Write( outFile );
		}
	}

	//! Returns the number of resources in the preload manifest
	int					  NumResources() const { return entries.Num(); }

	//! Returns the preload entry at the specified index.
	const preloadEntry_s& GetPreloadByIndex( int idx ) const { return entries[idx]; }

	//! Returns the resource name at the specified index from the preload manifest.
	const idStr&		  GetResourceNameByIndex( int idx ) const { return entries[idx].resourceName; }

	//! Returns the name of the manifest file.
	const char*			  GetManifestName() const { return filename; }

	//! Adds a preload entry to the manifest if it is not already present.
	void				  Add( const preloadEntry_s& p ) { entries.AddUnique( p ); }

	//! Adds a sound sample resource to the preload manifest.
	void				  AddSample( const char* _resourceName )
	{
		static preloadEntry_s pe;
		pe.resType		= PRELOAD_SAMPLE;
		pe.resourceName = _resourceName;
		entries.Append( pe );
	}

	//! Adds a model resource to the preload manifest.
	void AddModel( const char* _resourceName )
	{
		static preloadEntry_s pe;
		pe.resType		= PRELOAD_MODEL;
		pe.resourceName = _resourceName;
		entries.Append( pe );
	}

	//! Adds a particle resource to the preload manifest.
	void AddParticle( const char* _resourceName )
	{
		static preloadEntry_s pe;
		pe.resType		= PRELOAD_PARTICLE;
		pe.resourceName = _resourceName;
		entries.Append( pe );
	}

	//! Adds an animation resource to the preload manifest.
	void AddAnim( const char* _resourceName )
	{
		static preloadEntry_s pe;
		pe.resType		= PRELOAD_ANIM;
		pe.resourceName = _resourceName;
		entries.Append( pe );
	}

	//! Adds a collision model resource to the preload manifest.
	void AddCollisionModel( const char* _resourceName )
	{
		static preloadEntry_s pe;
		pe.resType		= PRELOAD_COLLISION;
		pe.resourceName = _resourceName;
		entries.Append( pe );
	}

	/*!
		\brief Adds an image resource to the preload manifest with specified filtering, repetition, usage, and cube map settings

		This function appends a new image preload entry to the manifest using the provided resource name and image-specific parameters. The image data includes filter settings, repeat behavior, usage
	   flags, and cube map configuration. The function constructs a preload entry structure and adds it to the internal list of entries for later processing during resource loading.

		\param _resourceName Name of the image resource to preload
		\param _filter Filtering mode for the image
		\param _repeat Repeat behavior for texture coordinates
		\param _usage Usage flags specifying how the image will be used
		\param _cube Cube map flag indicating if the image is a cube map
	*/
	void AddImage( const char* _resourceName, int _filter, int _repeat, int _usage, int _cube )
	{
		static preloadEntry_s pe;
		pe.resType		   = PRELOAD_IMAGE;
		pe.resourceName	   = _resourceName;
		pe.imgData.filter  = _filter;
		pe.imgData.repeat  = _repeat;
		pe.imgData.usage   = _usage;
		pe.imgData.cubeMap = _cube;
		entries.Append( pe );
	}

	//! Clears all entries from the preload manifest.
	void Clear() { entries.Clear(); }

	//! Returns the index of a resource with the specified name in the preload manifest, or -1 if not found
	int	 FindResource( const char* name )
	{
		for( int i = 0; i < entries.Num(); i++ ) {
			if( idStr::Icmp( name, entries[i].resourceName ) == 0 ) { return i; }
		}
		return -1;
	}

	//! Prints the contents of the preload manifest to the console.
	void Print()
	{
		idLib::Printf( "dump for preload manifest %s\n", GetManifestName() );
		idLib::Printf( "---------------------------------------\n" );
		for( int i = 0; i < NumResources(); i++ ) {
			idLib::Printf( "%s\n", GetResourceNameByIndex( i ).c_str() );
		}
	}

private:
	idList<preloadEntry_s> entries;
	idStr				   filename;
};

#endif /* !__FILE_MANIFEST_H__ */
