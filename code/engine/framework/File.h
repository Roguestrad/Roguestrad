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

#ifndef __FILE_H__
#define __FILE_H__

/*
==============================================================

  File Streams.

==============================================================
*/

// mode parm for Seek
typedef enum { FS_SEEK_CUR, FS_SEEK_END, FS_SEEK_SET } fsOrigin_t;

class idFileSystemLocal;

/*!
	\class idFile
	\brief An abstract base class for file I/O operations with support for reading, writing, and formatting data.

	This class provides a standardized interface for file operations, including reading and writing raw data, formatted strings, and structured data types such as vectors and matrices. It supports
   various byte order conversions for cross-platform compatibility, with methods for both little-endian and big-endian data handling. The class is designed to be inherited by concrete file
   implementations that provide actual file system access. It includes functionality for seeking within files, retrieving file metadata like size and timestamp, and managing buffered I/O operations.

*/
class idFile
{
public:
	virtual ~idFile() {};

	//! Returns the name of the file.
	virtual const char* GetName() const;

	//! Returns the full path of the file.
	virtual const char* GetFullPath() const;

	//! Reads data from the file into the provided buffer.
	virtual int			Read( void* buffer, int len );

	//! Writes data from a buffer to the file.
	virtual int			Write( const void* buffer, int len );

	//! Returns the length of the file.
	virtual int			Length() const;

	//! Returns the timestamp of the file for reload operations.
	virtual ID_TIME_T	Timestamp() const;

	//! Returns the current offset in the file.
	virtual int			Tell() const;

	//! Forces a flush operation on the file being written to.
	virtual void		ForceFlush();

	//! Ensures buffered data is written to the file.
	virtual void		Flush();

	//! Moves the file pointer to a specified position based on the given offset and origin.
	virtual int			Seek( long offset, fsOrigin_t origin );

	//! Sets the file pointer back to the beginning of the file.
	virtual void		Rewind();

	//! Writes a formatted string to the file, similar to fprintf.
	virtual int			Printf( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Writes a formatted string to the file using a va_list argument pointer.
	virtual int			VPrintf( const char* fmt, va_list arg );

	//! Writes a formatted string with high precision floating point numbers to the file.
	virtual int			WriteFloatString( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Reads an integer from the file and converts its byte order to little-endian.
	virtual int			ReadInt( int& value );

	//! Reads an unsigned integer from the file and converts its byte order.
	virtual int			ReadUnsignedInt( unsigned int& value );

	//! Reads a short integer value from the file and converts its byte order.
	virtual int			ReadShort( short& value );

	//! Reads an unsigned short value from the file and converts it from little-endian format.
	virtual int			ReadUnsignedShort( unsigned short& value );

	//! Reads a single character from the file into the provided reference.
	virtual int			ReadChar( char& value );

	//! Reads an unsigned char value from the file.
	virtual int			ReadUnsignedChar( unsigned char& value );

	//! Reads a floating-point value from the file and converts its byte order.
	virtual int			ReadFloat( float& value );

	//! Reads a boolean value from the file stream
	virtual int			ReadBool( bool& value );

	//! Reads a string from the file into the provided idStr object
	virtual int			ReadString( idStr& string );

	//! Reads a 2D vector from the file and returns the number of bytes read.
	virtual int			ReadVec2( idVec2& vec );

	//! Reads a 3D vector from the file and returns the number of bytes read.
	virtual int			ReadVec3( idVec3& vec );

	//! Reads a 4-dimensional vector from the file.
	virtual int			ReadVec4( idVec4& vec );

	//! Reads a 6-dimensional vector from the file and returns the number of bytes read.
	virtual int			ReadVec6( idVec6& vec );

	//! Reads a 3x3 matrix from the file and returns the number of bytes read.
	virtual int			ReadMat3( idMat3& mat );

	//! Writes a 32-bit integer to the file in little-endian byte order
	virtual int			WriteInt( const int value );

	//! Writes an unsigned integer value to the file in little-endian byte order.
	virtual int			WriteUnsignedInt( const unsigned int value );

	//! Writes a short integer value to the file in little-endian byte order.
	virtual int			WriteShort( const short value );

	//! Writes an unsigned short value to the file in little-endian format
	virtual int			WriteUnsignedShort( unsigned short value );

	//! Writes a single character to the file.
	virtual int			WriteChar( const char value );

	//! Writes an unsigned char value to the file.
	virtual int			WriteUnsignedChar( const unsigned char value );

	//! Writes a float value to the file in little-endian format and returns the number of bytes written.
	virtual int			WriteFloat( const float value );

	//! Writes a boolean value to the file as an unsigned char.
	virtual int			WriteBool( const bool value );

	//! Writes a null-terminated string to the file, preceded by its length.
	virtual int			WriteString( const char* string );

	//! Writes a 2D vector to the file
	virtual int			WriteVec2( const idVec2& vec );

	//! Writes a 3D vector to the file in little-endian byte order
	virtual int			WriteVec3( const idVec3& vec );

	//! Writes a 4-dimensional vector to the file.
	virtual int			WriteVec4( const idVec4& vec );

	//! Writes a 6-dimensional vector to the file with byte order conversion.
	virtual int			WriteVec6( const idVec6& vec );

	//! Writes a 3x3 matrix to the file.
	virtual int			WriteMat3( const idMat3& mat );

	//! Reads a big-endian formatted value from the file and converts it to the host byte order
	template<class type>
	ID_INLINE size_t ReadBig( type& c )
	{
		size_t r = Read( &c, sizeof( c ) );
		idSwap::Big( c );
		return r;
	}

	//! Reads a big-endian array of elements from the file and converts their byte order.
	template<class type>
	ID_INLINE size_t ReadBigArray( type* c, int count )
	{
		size_t r = Read( c, sizeof( c[0] ) * count );
		idSwap::BigArray( c, count );
		return r;
	}

	//! Writes a big-endian representation of the given value to the file
	template<class type>
	ID_INLINE size_t WriteBig( const type& c )
	{
		type b = c;
		idSwap::Big( b );
		return Write( &b, sizeof( b ) );
	}

	//! Writes an array of big-endian values to the file
	template<class type>
	ID_INLINE size_t WriteBigArray( const type* c, int count )
	{
		size_t r = 0;
		for( int i = 0; i < count; i++ ) {
			r += WriteBig( c[i] );
		}
		return r;
	}
};

/*!
	\class idFile_Memory
	\brief A memory-based file implementation that provides file-like operations on allocated memory buffers.

	This class implements a file interface using memory buffers, allowing operations such as reading, writing, seeking, and length management. It supports both read-only and read-write modes, and can
   be configured with various memory management policies including pre-allocation and granularity settings. The class is designed to work with both owned and unowned memory buffers, providing
   flexibility in how memory is managed and accessed. It inherits from idFile, making it compatible with systems that expect standard file interface implementations.

*/
class idFile_Memory : public idFile
{
	friend class idFileSystemLocal;

public:
	//! Constructs a memory file for writing without a name
	idFile_Memory();

	//! Constructs a memory file object for writing with the specified name
	idFile_Memory( const char* name );

	//! Constructs a memory file object for writing data
	idFile_Memory( const char* name, char* data, int length );

	//! Initializes a memory file object for reading with the specified name, data, and length.
	idFile_Memory( const char* name, const char* data, int length );

	//! Destructor for the idFile_Memory class that frees the allocated memory if certain conditions are met.
	virtual ~idFile_Memory();

	//! Returns the name of the memory file.
	virtual const char* GetName() const { return name.c_str(); }

	//! Returns the full path of the file as a null-terminated string.
	virtual const char* GetFullPath() const { return name.c_str(); }

	//! Reads data from the memory file into a buffer
	virtual int			Read( void* buffer, int len );

	//! Writes data from a buffer to the memory file
	virtual int			Write( const void* buffer, int len );

	//! Returns the length of the memory file in bytes.
	virtual int			Length() const;

	//! Sets the length of the memory file to the specified size.
	virtual void		SetLength( size_t len );

	//! Returns the timestamp of the memory file, which is always zero.
	virtual ID_TIME_T	Timestamp() const;

	//! Returns the current position in the memory file.
	virtual int			Tell() const;

	//! Flushes any buffered data to the underlying memory file.
	virtual void		ForceFlush();

	//! Flushes the memory file buffer.
	virtual void		Flush();

	//! Sets the current position in the memory file to the specified offset relative to the given origin
	virtual int			Seek( long offset, fsOrigin_t origin );

	//! Sets the maximum length of the memory file and prevents it from growing.
	void				SetMaxLength( size_t len );

	//! Changes the memory file to read-only mode.
	void				MakeReadOnly();

	//! Changes the file to be writable.
	void				MakeWritable();

	//! Clears the memory file, optionally freeing the allocated memory.
	virtual void		Clear( bool freeMemory = true );

	//! Sets the data and length for reading from a memory file.
	void				SetData( const char* data, int length );

	//! Returns a const pointer to the memory buffer
	const char*			GetDataPtr() const { return filePtr; }

	//! Returns a pointer to the memory buffer
	char*				GetDataPtr() { return filePtr; }

	//! Sets the memory granularity for the file object.
	void				SetGranularity( int g )
	{
		assert( g > 0 );
		granularity = g;
	}

	//! Pre-allocates memory for the file up to the specified length.
	void   PreAllocate( size_t len );

	//! Truncates the file data to the specified length, ensuring it does not exceed the allocated size.
	void   TruncateData( size_t len );

	//! Function transfers data ownership to the file object, making it read-only.
	void   TakeDataOwnership();

	//! Returns the maximum length of the memory file.
	size_t GetMaxLength() { return maxSize; }

	//! Returns the amount of memory allocated by this memory file object.
	size_t GetAllocated() { return allocated; }

protected:
	idStr name; // name of the file
private:
	int	   mode;		// open mode
	size_t maxSize;		// maximum size of file
	size_t fileSize;	// size of the file
	size_t allocated;	// allocated size
	int	   granularity; // file granularity
	char*  filePtr;		// buffer holding the file data
	char*  curPtr;		// current read/write pointer
};

/*!
	\class idFile_BitMsg
	\brief A file interface that provides read and write access to bit message data.

	This class provides a file-like interface for interacting with bit message data, allowing it to be used in contexts that expect standard file operations. It can be constructed in both read-only
   and read-write modes, depending on whether a const or non-const idBitMsg is provided. The implementation bridges the gap between bit-level message handling and file system abstractions, enabling
   seamless integration with systems that operate on file streams. The class maintains internal state for tracking position within the message and supports standard file operations such as reading,
   writing, seeking, and length reporting. Memory management is handled through the underlying idBitMsg object, which owns the actual data.

*/
class idFile_BitMsg : public idFile
{
	friend class idFileSystemLocal;

public:
	//! Constructs an idFile_BitMsg object for writing to the provided idBitMsg
	idFile_BitMsg( idBitMsg& msg );

	//! Constructs a read-only file interface for the provided bit message
	idFile_BitMsg( const idBitMsg& msg );
	virtual ~idFile_BitMsg();

	//! Returns the name of the file associated with this bit message.
	virtual const char* GetName() const { return name.c_str(); }

	//! Returns the full path of the file as a null-terminated string.
	virtual const char* GetFullPath() const { return name.c_str(); }

	//! Reads data from the file into the provided buffer
	virtual int			Read( void* buffer, int len );

	//! Writes data to a file
	virtual int			Write( const void* buffer, int len );

	//! Returns the size of the message in bytes.
	virtual int			Length() const;

	//! Returns the timestamp of the file.
	virtual ID_TIME_T	Timestamp() const;

	//! Returns the current position in the bit message stream.
	virtual int			Tell() const;

	//! Forces the current file buffer to be flushed to disk
	virtual void		ForceFlush();

	//! Flushes the bit message file buffer
	virtual void		Flush();

	//! Sets the current position in the file to the specified offset relative to the origin and returns zero on success or -1 on failure
	virtual int			Seek( long offset, fsOrigin_t origin );

private:
	idStr	  name; // name of the file
	int		  mode; // open mode
	idBitMsg* msg;
};

/*!
	\class idFile_Permanent
	\brief Provides persistent file access and manipulation capabilities for permanent storage.

	This class implements a file abstraction for permanent storage, inheriting from idFile to provide standardized file operations. It supports reading, writing, seeking, and various file metadata
   operations. The class manages a file handle and provides methods to control file buffering and synchronization. It is designed for operations that require persistent file storage with full control
   over file positioning and data transfer. The implementation handles partial reads and ensures proper resource cleanup through its destructor.

*/
class idFile_Permanent : public idFile
{
	friend class idFileSystemLocal;

public:
	//! Initializes a new instance of the idFile_Permanent class with default values.
	idFile_Permanent();

	//! Destructor for idFile_Permanent that closes the associated file handle.
	virtual ~idFile_Permanent();

	//! Returns the name of the file.
	virtual const char* GetName() const { return name.c_str(); }

	//! Returns the full path of the file as a null-terminated string.
	virtual const char* GetFullPath() const { return fullPath.c_str(); }

	//! Reads data from the file into the provided buffer, handling partial reads correctly.
	virtual int			Read( void* buffer, int len );

	//! Writes data from a buffer to the file
	virtual int			Write( const void* buffer, int len );

	//! Returns the length of the file in bytes.
	virtual int			Length() const;

	//! Returns the timestamp of the file
	virtual ID_TIME_T	Timestamp() const;

	//! Returns the current position in the file.
	virtual int			Tell() const;

	//! Forces the file to flush its buffered data to disk.
	virtual void		ForceFlush();

	//! Flushes the file buffer to ensure all pending data is written to disk.
	virtual void		Flush();

	//! Sets the current file position to the specified offset relative to the given origin
	virtual int			Seek( long offset, fsOrigin_t origin );

	//! Returns the file pointer for the permanent file.
	idFileHandle		GetFilePtr() { return o; }

private:
	idStr		 name;		 // relative path of the file - relative path
	idStr		 fullPath;	 // full file path - OS path
	int			 mode;		 // open mode
	int			 fileSize;	 // size of the file
	idFileHandle o;			 // file handle
	bool		 handleSync; // true if written data is immediately flushed
};

/*!
	\class idFile_Cached
	\brief Provides cached file access by buffering specified sections of a file for improved read performance.
*/
class idFile_Cached : public idFile_Permanent
{
	friend class idFileSystemLocal;

public:
	//! Initializes a new instance of the idFile_Cached class.
	idFile_Cached();

	//! Destructor for the idFile_Cached class that releases the allocated buffered memory.
	virtual ~idFile_Cached();

	//! Buffers a section of the file starting at the specified offset and spanning the specified length.
	void		CacheData( uint64 offset, uint64 length );

	//! Reads data from the file into a buffer.
	virtual int Read( void* buffer, int len );

	//! Returns the current position in the cached file.
	virtual int Tell() const;

	//! Moves the file pointer to the specified position and returns zero on success or -1 on failure
	virtual int Seek( long offset, fsOrigin_t origin );

private:
	uint64 internalFilePos;
	uint64 bufferedStartOffset;
	uint64 bufferedEndOffset;
	byte*  buffered;
};

/*!
	\class idFile_InZip
	\brief Provides read-only access to files stored within compressed archives.

	This class serves as a file abstraction layer that enables reading data from files contained within compressed archive formats. It inherits from idFile to maintain a consistent interface for file
   operations while adding specific functionality for handling compressed file contents. The class supports reading, seeking, and retrieving file metadata such as size and timestamp. Writing
   operations are explicitly disabled as compressed archives typically do not support in-place modifications. The implementation manages the underlying archive handles and ensures proper resource
   cleanup through its destructor. The class is designed to work with archive formats that store compressed data, providing transparent access to file contents without requiring the user to manage
   decompression manually.

*/
class idFile_InZip : public idFile
{
	// friend class			idFileSystemLocal;
	friend class idZipContainer;

public:
	//! Initializes a new instance of the idFile_InZip class with default values.
	idFile_InZip();

	//! Destructor for idFile_InZip that closes the current file and archive handles.
	virtual ~idFile_InZip();

	//! Returns the name of the file in the zip archive.
	virtual const char* GetName() const { return name.c_str(); }

	//! Returns the full path of the file as a null-terminated string.
	virtual const char* GetFullPath() const { return fullPath.c_str(); }

	//! Reads data from the compressed file into a buffer
	virtual int			Read( void* buffer, int len );

	//! This function cannot write to zipped files and always fails.
	virtual int			Write( const void* buffer, int len );

	//! Returns the length of the file in the zip archive.
	virtual int			Length() const;

	//! Returns the timestamp of the file in the zip archive.
	virtual ID_TIME_T	Timestamp() const;

	//! Returns the current position in the zip file.
	virtual int			Tell() const;

	//! Raises a fatal error because zipped files cannot be flushed.
	virtual void		ForceFlush();

	//! Flushes the zipped file and reports a fatal error.
	virtual void		Flush();

	//! Sets the current file position within the zip file
	virtual int			Seek( long offset, fsOrigin_t origin );

private:
	idStr	 name;	   // name of the file in the pak
	idStr	 fullPath; // full file path including pak file name
	// DG: use ZPOS64_T, it's the type minizip uses and should also work with zip64 files > 2GB
	ZPOS64_T zipFilePos; // zip file info position in pak
	// DG end
	int		 fileSize; // size of the file
	void*	 z;		   // unzip info
};

/*!
	\class idFile_InnerResource
	\brief Provides access to files embedded within resource containers.

	This class represents a file that is embedded within a larger resource file, enabling access to specific portions of the resource container as individual files. It inherits from idFile, allowing
   it to be used in contexts where a standard file interface is expected. The class manages the offset and length within the resource file, maintaining its own file position indicator. Memory for the
   resource data is handled externally, with the SetResourceBuffer method allowing the buffer to be configured. The implementation supports standard file operations including reading, writing,
   seeking, and retrieving file properties such as length and timestamp. The destructor ensures proper cleanup of allocated resources.

*/
class idFile_InnerResource : public idFile
{
	friend class idFileSystemLocal;

public:
	/*!
		\brief Constructs an idFile_InnerResource object to represent a file within a resource file

		This constructor initializes the internal state of an idFile_InnerResource object. It sets up the file name, offset, and length within the resource file, along with a pointer to the parent
	   resource file. The internal file position is initialized to zero and the resource buffer is set to NULL. This object is typically used when accessing files that are embedded within larger
	   resource containers

		\param _name The name of the file within the resource
		\param rezFile Pointer to the idFile object representing the resource file
		\param _offset The byte offset within the resource file where this file's data begins
		\param _len The length in bytes of this file's data within the resource file
	*/
	idFile_InnerResource( const char* _name, idFile* rezFile, int _offset, int _len );

	//! Destructor for idFile_InnerResource that releases the resource buffer if it was allocated.
	virtual ~idFile_InnerResource();

	//! Returns the name of the inner resource.
	virtual const char* GetName() const { return name.c_str(); }

	//! Returns the full path of the file as a null-terminated string.
	virtual const char* GetFullPath() const { return name.c_str(); }

	//! Reads data from the resource file into a buffer
	virtual int			Read( void* buffer, int len );

	//! Writes data to the file resource and returns the number of bytes written
	virtual int			Write( const void* buffer, int len )
	{
		assert( false );
		return 0;
	}

	//! Returns the length of the file in bytes.
	virtual int		  Length() const { return length; }

	//! Returns the timestamp of the file resource.
	virtual ID_TIME_T Timestamp() const { return 0; }

	//! Returns the current position in the file.
	virtual int		  Tell() const;

	//! Sets the file position indicator for the file stream
	virtual int		  Seek( long offset, fsOrigin_t origin );

	//! Sets the resource buffer for the inner resource file and resets the internal file position.
	void			  SetResourceBuffer( byte* buf )
	{
		resourceBuffer	= buf;
		internalFilePos = 0;
	}

private:
	idStr	name;			 // name of the file in the pak
	int		offset;			 // offset in the resource file
	int		length;			 // size
	idFile* resourceFile;	 // actual file
	int		internalFilePos; // seek offset
	byte*	resourceBuffer;	 // if using the temp save memory
};

/*!
	\class idFileLocal
	\brief idFileLocal provides automatic resource management for file streams by closing the wrapped file when the wrapper goes out of scope.
*/
class idFileLocal
{
public:
	//! Constructs an idFileLocal object and stores the provided file pointer.
	idFileLocal( idFile* _file ) :
		file( _file )
	{
	}

	//! Destructor that will destroy the file when this wrapper class goes out of scope.
	~idFileLocal();

	//! Converts the idFileLocal object to a raw file pointer.
	operator idFile*() const { return file; }

	//! Returns the wrapped file pointer for member access.
	idFile* operator->() const { return file; }

protected:
	idFile* file; // The managed file pointer.
};

#endif /* !__FILE_H__ */
