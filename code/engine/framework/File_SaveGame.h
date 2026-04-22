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
#ifndef __FILE_SAVEGAME_H__
#define __FILE_SAVEGAME_H__

#include <zlib.h>

// Listing of the types of files within a savegame package
enum saveGameType_t {
	SAVEGAMEFILE_NONE		  = 0,
	SAVEGAMEFILE_TEXT		  = BIT( 0 ), // implies that no checksum will be used
	SAVEGAMEFILE_BINARY		  = BIT( 1 ), // implies that a checksum will also be used
	SAVEGAMEFILE_COMPRESSED	  = BIT( 2 ),
	SAVEGAMEFILE_PIPELINED	  = BIT( 3 ),
	SAVEGAMEFILE_THUMB		  = BIT( 4 ), // for special processing on certain platforms
	SAVEGAMEFILE_BKGRND_IMAGE = BIT( 5 ), // for special processing on certain platforms, large background used on PS3
	SAVEGAMEFILE_AUTO_DELETE  = BIT( 6 ), // to be deleted automatically after completed
	SAVEGAMEFILE_OPTIONAL	  = BIT( 7 )  // if this flag is not set and missing, there is an error
};

/*!
	\class idFile_SaveGame
	\brief A file abstraction for save game data management.

	This class provides a file interface specifically designed for handling save game data, extending memory-based file functionality. It manages save game file names and types, supporting equality
   comparisons and name/type configuration. The class is intended to encapsulate the behavior of save game files within the application's file handling system.

*/
class idFile_SaveGame : public idFile_Memory
{
public:
	//! Constructs a new idFile_SaveGame object with default values.
	idFile_SaveGame() :
		type( SAVEGAMEFILE_NONE ),
		error( false )
	{
	}

	//! Initializes a save game file object with the specified name.
	idFile_SaveGame( const char* _name ) :
		idFile_Memory( _name ),
		type( SAVEGAMEFILE_NONE ),
		error( false )
	{
	}

	//! Constructs an idFile_SaveGame object with the specified file name and type.
	idFile_SaveGame( const char* _name, int type_ ) :
		idFile_Memory( _name ),
		type( type_ ),
		error( false )
	{
	}

	virtual ~idFile_SaveGame() { }

	//! Compares two idFile_SaveGame objects for equality based on their names.
	bool operator==( const idFile_SaveGame& other ) const { return idStr::Icmp( GetName(), other.GetName() ) == 0; }

	//! Compares the file name with the provided string for equality in a case-insensitive manner
	bool operator==( const char* _name ) const { return idStr::Icmp( GetName(), _name ) == 0; }

	//! Sets the name and type of the save game file.
	void SetNameAndType( const char* _name, int _type )
	{
		name = _name;
		type = _type;
	}

public:			// TODO_KC_CR for now...
	int	 type;	// helps platform determine what to do with the file (encrypt, checksum, etc.)
	bool error; // when loading, this is set if there is a problem
};

/*
================================================
idFile_SaveGamePipelined uses threads to pipeline overlap compression and IO
================================================
*/
class idSGFreadThread;
class idSGFwriteThread;
class idSGFdecompressThread;
class idSGFcompressThread;

struct blockForIO_t {
	byte*  data;
	size_t bytes;
};

/*!
	\class idFile_SaveGamePipelined
	\brief A file abstraction for handling save game data with pipelined compression and threading support.

	This class provides a file interface for reading and writing save game data with pipelined compression and decompression operations. It supports both reading and writing modes, with automatic
   handling of compression threading and block management. The class manages internal buffers for compressed and uncompressed data, facilitating efficient I/O operations during save game processing.
   It includes methods for opening files in different modes, reading and writing data with proper buffering, and managing the completion or abortion of ongoing operations. The implementation handles
   file metadata such as build version and save format version, allowing for compatibility checking. It also provides utilities for managing memory blocks during compression and decompression
   processes.

*/
class idFile_SaveGamePipelined : public idFile
{
public:
	// The buffers each hold two blocks of data, so one block can be operated on by
	// the next part of the generate / compress / IO pipeline.  The factor of two
	// size difference between the uncompressed and compressed blocks is unrelated
	// to the fact that there are two blocks in each buffer.
	static const int COMPRESSED_BLOCK_SIZE	 = 128 * 1024;
	static const int UNCOMPRESSED_BLOCK_SIZE = 256 * 1024;

	//! Initializes a new instance of the idFile_SaveGamePipelined class.
	idFile_SaveGamePipelined();

	//! Destroys the save game pipelined file object and cleans up all associated resources.
	virtual ~idFile_SaveGamePipelined();

	//! Opens a save game file for reading with optional native file handling and decompression threading.
	bool				OpenForReading( const char* const filename, bool useNativeFile );

	//! Opens a file for writing with optional pipelined compression and threading support
	bool				OpenForWriting( const char* const filename, bool useNativeFile );

	//! Opens the save game file for reading with pipelined decompression
	bool				OpenForReading( idFile* file );

	//! Opens the save game file for writing with optional pipelined compression and threading.
	bool				OpenForWriting( idFile* file );

	//! Completes any ongoing read or write operations and cleans up resources.
	void				Finish();

	//! Aborts any ongoing read or write operations.
	void				Abort();

	//! Cancels any ongoing read or write operations to prepare for application termination.
	static void			CancelToTerminate() { cancelToTerminate = true; }

	//! Reads the build version from the save game file and returns true if successful.
	bool				ReadBuildVersion();

	//! Returns the build version string stored in the save game pipelined file object.
	const char*			GetBuildVersion() const { return buildVersion; }

	//! Reads and validates the save format version from the file.
	bool				ReadSaveFormatVersion();

	//! Returns the save format version used by the save game pipelined file.
	int					GetSaveFormatVersion() const { return saveFormatVersion; }

	//! Returns the pointer size used for save game serialization.
	int					GetPointerSize() const;

	//! Returns the name of the save game pipelined file.
	virtual const char* GetName() const { return name.c_str(); }

	//! Returns the full path of the file as a null-terminated string.
	virtual const char* GetFullPath() const { return name.c_str(); }

	//! Reads data from a save game pipelined file into the provided buffer
	virtual int			Read( void* buffer, int len );

	//! Writes data to the save game pipelined file, buffering uncompressed data until block boundaries.
	virtual int			Write( const void* buffer, int len );

	//! Returns the length of the file in bytes.
	virtual int			Length() const
	{
		// RB: 64 bit fix, we don't need support for files bigger than 2 GB
		return ( int )compressedLength;
		// RB end
	}

	//! Sets the length of the save game pipelined file to the specified value.
	virtual void SetLength( size_t len ) { compressedLength = len; }

	//! Returns the current position in the file.
	virtual int	 Tell() const
	{
		assert( 0 );
		return 0;
	}

	//! This function is not implemented and always asserts.
	virtual int Seek( long offset, fsOrigin_t origin )
	{
		assert( 0 );
		return 0;
	}

	//! Returns the timestamp of the save game pipelined file.
	virtual ID_TIME_T Timestamp() const { return 0; }

	//------------------------
	// These can be used by a background thread to read/write data
	// when the file was opened with 'useNativeFile' set to false.
	//------------------------

	enum mode_t { CLOSED, WRITE, READ };

	//! Returns the file mode for the save game pipelined file, indicating whether it is in read or write mode.
	mode_t GetMode() const { return mode; }

	//! Retrieves the next block for writing from the pipeline, potentially blocking until one becomes available.
	bool   NextWriteBlock( blockForIO_t* block );

	//! Retrieves the next block for reading data into and reports the number of bytes written to the previous block, potentially blocking until space is available.
	bool   NextReadBlock( blockForIO_t* block, size_t lastReadBytes );

private:
	friend class idSGFreadThread;
	friend class idSGFwriteThread;
	friend class idSGFdecompressThread;
	friend class idSGFcompressThread;

	idStr				   name;   // Name of the file.
	idStr				   osPath; // OS path.
	mode_t				   mode;   // Open mode.
	size_t				   compressedLength;

	static const int	   COMPRESSED_BUFFER_SIZE	= COMPRESSED_BLOCK_SIZE * 2;
	static const int	   UNCOMPRESSED_BUFFER_SIZE = UNCOMPRESSED_BLOCK_SIZE * 2;

	byte				   uncompressed[UNCOMPRESSED_BUFFER_SIZE];
	size_t				   uncompressedProducedBytes; // not masked
	size_t				   uncompressedConsumedBytes; // not masked

	byte				   compressed[COMPRESSED_BUFFER_SIZE];
	size_t				   compressedProducedBytes; // not masked
	size_t				   compressedConsumedBytes; // not masked

	//------------------------
	// These variables are used to pass data between threads in a thread-safe manner.
	//------------------------

	byte*				   dataZlib;
	size_t				   bytesZlib;

	byte*				   dataIO;
	size_t				   bytesIO;

	//------------------------
	// These variables are used by CompressBlock() and DecompressBlock().
	//------------------------

	z_stream			   zStream;
	int					   zLibFlushType; // Z_NO_FLUSH or Z_FINISH
	bool				   zStreamEndHit;
	int					   numChecksums;

	//------------------------
	// These variables are used by WriteBlock() and ReadBlock().
	//------------------------

	idFile*				   nativeFile;
	bool				   nativeFileEndHit;
	bool				   finished;

	//------------------------
	// The background threads and signals for NextWriteBlock() and NextReadBlock().
	//------------------------

	idSGFreadThread*	   readThread;
	idSGFwriteThread*	   writeThread;

	idSGFdecompressThread* decompressThread;
	idSGFcompressThread*   compressThread;

	idSysSignal			   blockRequested;
	idSysSignal			   blockAvailable;
	idSysSignal			   blockFinished;

	idStrStatic<32>		   buildVersion; // build version this file was saved with
	int16		pointerSize; // the number of bytes in a pointer, because different pointer sizes mean different offsets into objects a 64 bit build cannot load games saved from a 32 bit build or vice
							 // version (a value of 0 is interpreted as 4 bytes)
	int16		saveFormatVersion; // version number specific to save games (for maintaining save compatibility across builds)

	//------------------------
	// These variables are used when we want to abort due to the termination of the application
	//------------------------
	static bool cancelToTerminate;

	//! Flushes the uncompressed block by waiting for the compression thread and preparing the next block for Zlib compression.
	void		FlushUncompressedBlock();

	//! Flushes the compressed block to the output file or buffer
	void		FlushCompressedBlock();

	//! Compresses a block of data using zlib compression
	void		CompressBlock();

	//! Writes the current block of data to the native file and resets the tracking variables.
	void		WriteBlock();

	//! Processes the next block of uncompressed data from the save game file.
	void		PumpUncompressedBlock();

	//! Processes the next compressed block of data for the save game pipelined file.
	void		PumpCompressedBlock();

	//! Decompresses a block of data using the zlib library.
	void		DecompressBlock();

	//! Reads a block of data from the native file into the compressed buffer.
	void		ReadBlock();
};

#endif // !__FILE_SAVEGAME_H__
