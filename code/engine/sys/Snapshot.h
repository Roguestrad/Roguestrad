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
#ifndef __SNAPSHOT_H__
#define __SNAPSHOT_H__

#include "Snapshot_Jobs.h"

extern idCVar net_verboseSnapshot;
#define NET_VERBOSESNAPSHOT_PRINT \
	if( net_verboseSnapshot.GetInteger() > 0 ) idLib::Printf
#define NET_VERBOSESNAPSHOT_PRINT_LEVEL( X, Y ) \
	if( net_verboseSnapshot.GetInteger() >= ( X ) ) idLib::Printf( "%s", Y )

/*!
	\class idSnapShot
	\brief A snapshot contains a list of objects and their states

	The idSnapShot class manages collections of object states at specific points in time, used for network synchronization and state tracking. It supports operations for adding, updating, and
   comparing object states, as well as delta compression for efficient data transmission. The class handles both full snapshots and incremental deltas, with support for visibility tracking and memory
   management. It provides methods for reading and writing snapshot data from files, processing compressed delta packets, and managing object state lifecycle including creation, modification, and
   cleanup. The implementation includes features for tracking object changes, handling stale states, and performing efficient binary searches for object lookup.

*/
class idSnapShot
{
public:
	//! Constructs a new snapshot object with default time values.
	idSnapShot();

	//! Constructs a new snapshot by copying an existing snapshot.
	idSnapShot( const idSnapShot& other );

	//! Destructor for the idSnapShot class that clears the snapshot data.
	~idSnapShot();

	//! Assigns the contents of another snapshot to this snapshot object.
	void		operator=( const idSnapShot& other );

	//! Clears the snapshot data by resetting its internal state and freeing allocated object states.
	void		Clear();

	//! Returns the time value stored in the snapshot
	int			GetTime() const { return time; }

	//! Sets the snapshot time to the specified value.
	void		SetTime( int t ) { time = t; }

	//! Returns the received time of the snapshot.
	int			GetRecvTime() const { return recvTime; }

	//! Sets the receive time for the snapshot to the specified value.
	void		SetRecvTime( int t ) { recvTime = t; }

	/*!
		\brief Extracts sequence and baseSequence values from compressed delta memory.

		This function reads sequence and baseSequence values from a compressed data stream using LZW decompression. It initializes an LZW compressor with the provided compressed data, then reads two
	   agnostic integers representing the sequence and baseSequence values from the stream. The function does not modify the original compressed data but consumes it during the read operations.

		\param deltaMem Pointer to the compressed delta memory stream
		\param deltaSize Size of the compressed delta memory in bytes
		\param sequence Reference to store the extracted sequence value
		\param baseSequence Reference to store the extracted baseSequence value
	*/
	static void PeekDeltaSequence( const char* deltaMem, int deltaSize, int& sequence, int& baseSequence );

	/*!
		\brief Reads a delta-compressed snapshot packet and applies it to the current snapshot state

		This function processes a delta-compressed snapshot packet that contains changes to object states relative to a base snapshot. It decompresses the data using LZW and RLE compression
	   algorithms, updates object states accordingly, and handles various cases including new objects, deleted objects, stale objects, and normal state updates. The function tracks byte counts for
	   compression ratio calculation and performs checksum validation when enabled. Returns true if the delta represents a full snapshot, false if it's a partial delta.

		\param deltaMem Pointer to the compressed delta data
		\param deltaSize Size of the delta data in bytes
		\param visIndex Visibility index for the snapshot
		\param templateStates Pointer to the template snapshot containing base states
		\return True if the delta represents a full snapshot, false if it's a partial delta
	*/
	bool		ReadDeltaForJob( const char* deltaMem, int deltaSize, int visIndex, idSnapShot* templateStates );

	//! Reads a delta snapshot from a file for a specific visibility index
	bool		ReadDelta( idFile* file, int visIndex );

	// Writes an object state packet which is delta compressed against the old snapshot
	struct objectBuffer_t {
		//! Constructs an empty objectBuffer_t instance.
		objectBuffer_t() :
			data( NULL ),
			size( 0 )
		{
		}

		//! Initializes an object buffer with the specified size and allocates memory for it.
		objectBuffer_t( int s ) :
			data( NULL ),
			size( s )
		{
			Alloc( s );
		}

		//! Copy constructor for the objectBuffer_t struct that initializes a new instance by copying from another objectBuffer_t instance.
		objectBuffer_t( const objectBuffer_t& o ) :
			data( NULL ),
			size( 0 )
		{
			*this = o;
		}
		~objectBuffer_t() { _Release(); }

		//! Allocates memory for the object buffer with the specified size
		void		 Alloc( int size );

		//! Returns the number of references to the object buffer.
		int			 NumRefs() { return data == NULL ? 0 : data[size]; }

		//! Returns the size of the object in memory.
		objectSize_t Size() const { return size; }

		//! Returns a pointer to the data buffer or NULL if the buffer is empty.
		byte*		 Ptr() { return data == NULL ? NULL : data; }

		//! Provides indexed access to elements in the snapshot object buffer
		byte&		 operator[]( int i ) { return data[i]; }

		//! Assigns the contents of another object buffer to this object buffer.
		void		 operator=( const objectBuffer_t& other );

		//! Increases the reference count of the object buffer.
		void		 _AddRef();

		//! Releases the object buffer by decrementing its reference count and freeing memory if necessary
		void		 _Release();

	private:
		byte*		 data;
		objectSize_t size;
	};

	struct objectState_t {
		//! Initializes a new objectState_t instance with default values.
		objectState_t() :
			objectNum( 0 ),
			visMask( MAX_UNSIGNED_TYPE( uint32 ) ),
			stale( false ),
			deleted( false ),
			changedCount( 0 ),
			expectedSequence( 0 ),
			createdFromTemplate( false )
		{
		}

		//! Prints the contents of the object state buffer with a given name prefix for debugging purposes.
		void		   Print( const char* name );

		uint16		   objectNum;
		objectBuffer_t buffer;
		uint32		   visMask;
		bool		   stale; // easy way for clients to check if ss obj is stale. Probably temp till client side of vismask system is more fleshed out
		bool		   deleted;
		int			   changedCount; // Incremented each time the state changed
		int			   expectedSequence;
		bool		   createdFromTemplate;
	};

	struct submitDeltaJobsInfo_t {
		objParms_t*		objParms;	 // Start of object parms
		int				maxObjParms; // Max parms (which will dictate how many objects can be processed)
		uint8*			objMemory;	 // Memory that objects were written out to
		objHeader_t*	headers;	 // Memory for headers
		int				maxHeaders;
		int				maxObjMemory;  // Max memory (which will dictate when syncs need to occur)
		lzwParm_t*		lzwParms;	   // Start of lzw parms
		int				maxDeltaParms; // Max lzw parms (which will dictate how many syncs we can have)

		idSnapShot*		oldSnap; // snap we are comparing this snap to (to produce a delta)
		int				visIndex;
		int				baseSequence;

		idSnapShot*		templateStates; // states for new snapObj that arent in old states

		lzwInOutData_t* lzwInOutData;
	};

	//! Submits write delta jobs for object state differences between snapshots.
	void		   SubmitWriteDeltaToJobs( const submitDeltaJobsInfo_t& submitDeltaJobInfo );

	/*!
		\brief Writes a delta snapshot to a file, comparing the current snapshot with a previous one to minimize data size.

		This function generates a delta snapshot by comparing the current snapshot with an old snapshot. It writes object state data to the provided file, using difference compression to reduce data
	   size. The function handles added, modified, and removed objects. It checks for size limits and returns false if the maximum length is exceeded during writing.

		\param old The previous snapshot to compare against for generating the delta
		\param visIndex The visibility index for the snapshot
		\param file The file to write the delta snapshot data to
		\param maxLength The maximum length allowed for the delta snapshot data
		\param optimalLength The optimal length at which to stop writing, defaults to 0
		\return True if the delta was written successfully, false if the maximum length was exceeded
	*/
	bool		   WriteDelta( idSnapShot& old, int visIndex, idFile* file, int maxLength, int optimalLength = 0 );

	/*!
		\brief Adds an object to the snapshot state, overwriting any existing object with the same number

		This function inserts or updates an object state within the snapshot. It takes the object number, visibility mask, message data, and an optional tag. The function internally delegates to
	   another S_AddObject overload that handles the actual state management. If an object with the specified number already exists, it will be overwritten with the new data.

		\param objectNum The unique identifier for the object being added
		\param visMask The visibility mask associated with the object
		\param msg The message data containing the object's state information
		\param tag Optional tag for debugging or identification purposes
		\return A pointer to the objectState_t structure that was added or updated in the snapshot
	*/
	objectState_t* S_AddObject( int objectNum, uint32 visMask, const idBitMsg& msg, const char* tag = NULL ) { return S_AddObject( objectNum, visMask, msg.GetReadData(), msg.GetSize(), tag ); }

	/*!
		\brief Adds an object to the snapshot state, overwriting any existing object with the same number

		This function inserts or updates an object state within the snapshot. It takes an object number, visibility mask, buffer data, and size to create or modify an object state entry. The function
	   ensures that if the buffer size matches the existing buffer size and has only one reference, it reuses the existing buffer to avoid unnecessary allocations. Otherwise, it creates a new buffer
	   for the object data.

		\param objectNum The unique identifier for the object being added
		\param visMask The visibility mask associated with the object
		\param buffer Pointer to the buffer containing the object data
		\param size The size of the object data buffer
		\param tag Optional tag for debugging or identification purposes
		\return A pointer to the objectState_t structure representing the added or updated object state
	*/
	objectState_t* S_AddObject( int objectNum, uint32 visMask, const byte* buffer, int size, const char* tag = NULL ) { return S_AddObject( objectNum, visMask, ( const char* )buffer, size, tag ); }

	/*!
		\brief Adds or updates an object state in the snapshot with the provided data

		This function inserts a new object state or updates an existing one in the snapshot. It uses the object number to identify the state and associates it with a visibility mask. The function
	   handles buffer management efficiently by reusing existing buffers when possible, and only allocates new memory when the size changes or when the buffer is referenced multiple times. The buffer
	   data is copied from the provided input buffer and size parameters. The optional tag parameter can be used to annotate the object state for debugging or tracking purposes.

		\param objectNum The unique identifier for the object being added or updated
		\param visMask Visibility mask associated with the object state
		\param buffer Pointer to the data buffer containing the object state information
		\param size Size in bytes of the data pointed to by buffer
		\param tag Optional string tag for debugging or identification purposes
		\return Pointer to the objectState_t structure that was added or updated in the snapshot
	*/
	objectState_t* S_AddObject( int objectNum, uint32 visMask, const char* buffer, int size, const char* tag = NULL );

	//! Copies an object from another snapshot into this snapshot
	bool		   CopyObject( const idSnapShot& oldss, int objectNum, bool forceStale = false );

	/*!
		\brief Compares object state data between this snapshot and a previous one, returning the number of bytes that differ

		This function compares the state of a specific object between the current snapshot and a previous snapshot. It can optionally compare only a subsection of the object data by specifying start
	   and end indices. If the object doesn't exist in the old snapshot, it returns the full size of the new object. The comparison accounts for potential offset differences between the old and new
	   states.

		\param oldss Previous snapshot to compare against
		\param objectNum ID of the object to compare
		\param start Start index for comparison, defaults to 0
		\param end End index for comparison, defaults to end of buffer
		\param oldStart Offset into the old object buffer, defaults to 0
		\return Number of bytes that differ between the two object states, or the size of the object if it didn't exist in the previous snapshot
		\throws Assertion failure if the object ID is not found in the current snapshot
	*/
	int			   CompareObject( const idSnapShot* oldss, int objectNum, int start = 0, int end = 0, int oldStart = 0 );

	//! Returns the number of objects in this snapshot
	int			   NumObjects() const { return objectStates.Num(); }

	//! Returns the object number for the specified index and fills the provided bit message with the object data.
	int			   GetObjectMsgByIndex( int i, idBitMsg& msg, bool ignoreIfStale = false ) const;

	//! Returns true if the object with the specified ID is found in the snapshot
	bool		   GetObjectMsgByID( int objectNum, idBitMsg& msg, bool ignoreIfStale = false ) { return GetObjectMsgByIndex( FindObjectIndexByID( objectNum ), msg, ignoreIfStale ) == objectNum; }

	//! Returns the index of an object in the snapshot by its object number, or -1 if not found
	int			   FindObjectIndexByID( int objectNum ) const;

	//! Returns the object state by its ID, or NULL if not found
	objectState_t* FindObjectByID( int objectNum ) const;

	//! Returns whether or not an object at the specified index is stale
	bool		   ObjectIsStaleByIndex( int i ) const;

	//! Returns the change count for an object at the specified index in the snapshot.
	int			   ObjectChangedCountByIndex( int i ) const;

	//! Removes empty object states from the snapshot.
	void		   CleanupEmptyStates();

	//! Prints a report for the snapshot.
	void		   PrintReport();

	//! Updates the expected sequence number for object states that are currently set to -2.
	void		   UpdateExpectedSeq( int newSeq );

	//! Applies uncompressed state from a message to an existing object state by adding it to the current buffer.
	void		   ApplyToExistingState( int objId, idBitMsg& msg );

	//! Returns the template object state for a given object number from the provided snapshot, optionally comparing it with a new state for debugging purposes.
	objectState_t* GetTemplateState( int objNum, idSnapShot* templateStates, objectState_t* newState = NULL );

	void		   RemoveObject( int objId );

private:
	idList<objectState_t*, TAG_IDLIB_LIST_SNAPSHOT> objectStates;
	idBlockAlloc<objectState_t, 16, TAG_NETWORKING> allocatedObjs;

	int												time;
	int												recvTime;

	//! Performs a binary search to find the index of a given object number in the snapshot's object states.
	int												BinarySearch( int objectNum ) const;

	//! Returns a reference to the object state for the given object number, creating it if it doesn't exist
	objectState_t&									FindOrCreateObjectByID( int objectNum );

	/*!
		\brief Submits an object job for snapshot delta compression using the provided state information and buffer pointers

		This function processes object state information for snapshot delta compression. It handles buffer management and submission of jobs when memory limits are reached, ensuring proper alignment
	   and bounds checking. The function manages both new and old object states, sets up header and parameter information, and advances the relevant pointers for subsequent processing. It performs
	   validation checks to ensure proper memory alignment and buffer limits are maintained throughout the operation.

		\param submitDeltaJobsInfo Contains parameters and limits for the snapshot job submission process
		\param newState Pointer to the new object state data, may be NULL
		\param oldState Pointer to the old object state data, may be NULL
		\param baseObjParm Reference to the base object parameter pointer for job tracking
		\param curObjParm Reference to the current object parameter pointer for job tracking
		\param curHeader Reference to the current object header pointer for job tracking
		\param curObjDest Reference to the current object destination pointer for buffer management
		\param curlzwParm Reference to the LZW parameter pointer for compression job management
		\throws Error when out of parms for snapshot jobs
	*/
	void											SubmitObjectJob( const submitDeltaJobsInfo_t& submitDeltaJobsInfo,
												   objectState_t*								  newState,	   // New obj state (can be NULL, which means deleted)
												   objectState_t*								  oldState,	   // Old obj state (can be NULL, which means new)
												   objParms_t*&									  baseObjParm, // Starting obj parm of current stream
												   objParms_t*&									  curObjParm,  // Current obj parm of current stream
												   objHeader_t*&								  curHeader,   // Current header dest
												   uint8*&										  curObjDest,  // Current write pos of current obj
												   lzwParm_t*&									  curlzwParm );								   // Current delta parm for next lzw job

	/*!
		\brief Submits an LZW compression job for delta data between snapshots

		This function prepares and submits an LZW compression job to compress delta data between two snapshots. It calculates the number of objects to be compressed, validates that there are
	   sufficient LZW parameters available, and sets up the compression parameters including timing information and compression flags. The function then executes the LZW compression job and advances
	   the parameter pointer for the next job submission.

		\param writeDeltaInfo Structure containing parameters for the delta compression job including LZW parameters and headers
		\param baseObjParm Pointer to the first object parameter for the current data stream
		\param curObjParm Pointer to the current object parameter
		\param curlzwParm Pointer to the current LZW parameter structure
		\param saveDictionary Flag indicating whether to save the compression dictionary
		\throws Error when there are not enough LZW parameters available for the compression job
	*/
	void											SubmitLZWJob( const submitDeltaJobsInfo_t& writeDeltaInfo,
												   objParms_t*&								   baseObjParm,	  // Pointer to the first obj parm for the current stream
												   objParms_t*&								   curObjParm,	  // Current obj parm
												   lzwParm_t*&								   curlzwParm,	  // Current delta parm
												   bool										   saveDictionary // If true, this is the first of several calls which will be appended
											   );

	/*!
		\brief Writes an object state to a file, handling visibility changes and delta compression.

		This function serializes an object state to a file, managing visibility transitions and delta compression. It handles three cases: new objects, deleted objects, and modified objects.
	   Visibility changes are signaled with special size markers to indicate when an object becomes stale or no longer stale. For modified objects, it writes a delta of the state buffer. The function
	   tracks the last object number to enable delta encoding of object IDs.

		\param file Output file to write the object data to
		\param visIndex Visibility index for determining object visibility changes
		\param newState Current state of the object, or NULL if the object is being deleted
		\param oldState Previous state of the object, or NULL if the object is new
		\param lastobjectNum Reference to the last object number written, updated during the operation
		\throws assertion failure when newState or oldState is NULL and the function cannot proceed
	*/
	void											WriteObject( idFile* file, int visIndex, objectState_t* newState, objectState_t* oldState, int& lastobjectNum );

	//! Frees the object state at the specified index in the snapshot.
	void											FreeObjectState( int index );
};

#endif // __SNAPSHOT_H__
