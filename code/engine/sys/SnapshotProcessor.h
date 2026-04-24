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
#ifndef __SNAP_PROCESSOR_H__
#define __SNAP_PROCESSOR_H__

/*!
	\class idSnapshotProcessor
	\brief Manages snapshot processing, delta application, and state synchronization for networked game objects.

	Handles the processing of snapshot data including delta application, sequence validation, and state management for networked game objects. Provides functionality for setting pending snapshots,
   applying delta updates, and managing snapshot queues. Supports operations for both sending and receiving snapshot deltas while maintaining consistency between client and server states. The
   processor maintains base state snapshots and tracks sequence numbers to ensure proper delta ordering and synchronization. Offers methods for debugging and validation of snapshot data including
   sequence consistency checks and delta queue management. The class is designed to work with networked game states where partial updates are applied to base snapshots to maintain efficient data
   transmission and synchronization.

*/
class idSnapshotProcessor
{
public:
	static const int INITIAL_SNAP_SEQUENCE = 42;

	//! Initializes a new instance of the snapshot processor.
	idSnapshotProcessor();

	//! Destructor for the idSnapshotProcessor class that frees the allocated job memory.
	~idSnapshotProcessor();

	//! Resets the snapshot processor state, optionally printing a debug message.
	void			 Reset( bool cstor = false );

	//! Attempts to set a pending snapshot, returning true if successful and false if there is already a pending snapshot.
	bool			 TrySetPendingSnapshot( idSnapShot& ss );

	/*!
		\brief Extracts the delta sequence and base sequence from the provided delta memory buffer

		This function reads the delta memory buffer to extract the sequence number and base sequence number. It is used to peek into delta data without fully processing it. The function calls the
	   static helper function from idSnapShot class to perform the actual extraction

		\param deltaMem Pointer to the delta memory buffer to read from
		\param deltaSize Size of the delta memory buffer in bytes
		\param deltaSequence Reference to store the extracted delta sequence number
		\param deltaBaseSequence Reference to store the extracted base sequence number
	*/
	void			 PeekDeltaSequence( const char* deltaMem, int deltaSize, int& deltaSequence, int& deltaBaseSequence );

	/*!
		\brief Applies a delta to the provided snapshot

		This function takes a snapshot and applies a delta update to it using the provided delta memory and size. The delta is applied for a specific visibility index and uses the template states for
	   reference. The function returns true if the delta was successfully applied, and false otherwise.

		\param snap The snapshot to apply the delta to
		\param deltaMem Pointer to the delta memory to be applied
		\param deltaSize Size of the delta memory in bytes
		\param visIndex Visibility index for the delta application
		\return True if the delta was successfully applied to the snapshot, false otherwise
	*/
	bool			 ApplyDeltaToSnapshot( idSnapShot& snap, const char* deltaMem, int deltaSize, int visIndex );

	/*!
		\brief Submits a pending snapshot for processing into job memory and prepares it for sending.

		This function is responsible for submitting a pending snapshot to a job for processing. It ensures that the provided memory and LZW data are properly aligned and validates that there is a
	   pending snapshot to submit. The function configures the LZW data structures within the job memory, copies the base state to avoid race conditions, and prepares the submission information for
	   the snapshot job. The actual submission is performed by calling SubmitWriteDeltaToJobs with the configured information.

		\param visIndex Index of the visibility data to be used for the snapshot
		\param objMemory Pointer to the object memory buffer for the snapshot
		\param objMemorySize Size of the object memory buffer in bytes
		\param lzwData Pointer to the LZW compression data structure for the snapshot
	*/
	void			 SubmitPendingSnap( int visIndex, uint8* objMemory, int objMemorySize, lzwCompressionData_t* lzwData );

	//! Returns the size of the next pending snapshot delta to send
	int				 GetPendingSnapDelta( byte* outBuffer, int maxLength );

	//! Returns true if there is a pending snapshot ready to be sent
	bool			 PendingSnapReadyToSend() const { return jobMemory->lzwInOutData.numlzwDeltas > 0; }

	/*!
		\brief Processes incoming snapshot delta data to update the client's game state

		Receives and applies a delta snapshot from the server to update the local game state. This function handles sequence validation, applies the delta to the base state, and returns a complete
	   snapshot if successful. The function validates that the incoming delta is not older than the current snapshot sequence and ensures the base state is properly synchronized before applying the
	   delta. It also manages the delta queue and handles cases where the delta queue becomes out of sync between client and server.

		\param deltaData Pointer to the delta data buffer
		\param deltaLength Length of the delta data in bytes
		\param visIndex Visibility index for the snapshot
		\param outSeq Output parameter for the delta sequence number
		\param outBaseSeq Output parameter for the base sequence number
		\param outSnap Output parameter for the resulting snapshot
		\param fullSnap Output parameter indicating if this was a full snapshot
		\return True if the delta was successfully processed and applied, false otherwise
	*/
	bool			 ReceiveSnapshotDelta( const byte* deltaData, int deltaLength, int visIndex, int& outSeq, int& outBaseSeq, idSnapShot& outSnap, bool& fullSnap );

	//! Applies a received delta to the base state for the specified visibility index and snapshot number
	bool			 ApplySnapshotDelta( int visIndex, int snapshotNumber );

	//! Removes snapshot deltas that reference obsolete base sequences.
	void			 RemoveDeltasForOldBaseSequence();

	//! Validates delta sequence and base sequence values for consistency and ordering.
	void			 SanityCheckDeltas();

	//! Returns true if there is a pending snapshot that needs to be sent.
	bool			 HasPendingSnap() const { return hasPendingSnap; }

	//! Returns a pointer to the base state snapshot
	idSnapShot*		 GetBaseState() { return &baseState; }

	//! Returns a pointer to the pending snapshot object.
	idSnapShot*		 GetPendingSnap() { return &pendingSnap; }

	//! Returns the current snapshot sequence number.
	int				 GetSnapSequence() { return snapSequence; }

	//! Returns the base sequence number used for snapshot delta compression.
	int				 GetBaseSequence() { return baseSequence; }

	//! Returns the base sequence number of the last full snapshot processed
	int				 GetFullSnapBaseSequence() { return lastFullSnapBaseSequence; }

	//! Returns the sequence number of the last appended delta or -1 if no deltas exist.
	int				 GetLastAppendedSequence() { return deltas.Num() == 0 ? -1 : deltas.ItemSequence( deltas.Num() - 1 ); }

	//! Returns the number of snapshot deltas in the queue.
	int				 GetSnapQueueSize() { return deltas.Num(); }

	//! Checks if the snapshot processor is currently busy confirming a partial snapshot.
	bool			 IsBusyConfirmingPartialSnap();

	//! Adds a snapshot object template to the snapshot processor
	void			 AddSnapObjTemplate( int objID, idBitMsg& msg );

	static const int MAX_SNAPSHOT_QUEUE = 64;

private:
	// Internal commands to set up, and flush the compressors
	static const int										MAX_SNAP_SIZE		   = idPacketProcessor::MAX_MSG_SIZE;
	static const int										MAX_SNAPSHOT_QUEUE_MEM = 64 * 1024; // 64k

	// sequence number of the last snapshot we sent/received
	// on the server, the sequencing is different for each network peer (net_verboseSnapshot 1)
	// on the jobbed snapshot compression path, the sequence is incremented in NewLZWStream and pulled into this in idSnapshotProcessor::GetPendingSnapDelta
	int														snapSequence;
	int														baseSequence;
	int														lastFullSnapBaseSequence; // Latest base sequence number that is a full snap

	idSnapShot												baseState; // known snapshot base on the client
	idDataQueue<MAX_SNAPSHOT_QUEUE, MAX_SNAPSHOT_QUEUE_MEM> deltas;	   // list of unacknowledged snapshot deltas

	idSnapShot												pendingSnap;	// Current snap waiting to be fully sent
	bool													hasPendingSnap; // true if pendingSnap is still waiting to be sent

	struct jobMemory_t {
		static const int					  MAX_LZW_DELTAS = 1; // FIXME: cleanup the old multiple delta support completely

		// @TODO this is a hack fix to allow online to load into coop (where there are lots of entities).
		// The real solution should be coming soon.
		// Doom MP: we encountered the same problem, going from 1024 to 4096 as well until a better solution is in place
		// (initial, useless, exchange of func_statics is killing us)
		static const int					  MAX_OBJ_PARMS = 4096;

		static const int					  MAX_LZW_PARMS	  = 32;
		static const int					  MAX_OBJ_HEADERS = 256;
		static const int					  MAX_LZW_MEM	  = 1024 * 8; // 8k in the byte * lzwMem buffers, must be <= PS3_DMA_MAX

		// Parm memory to jobs
		idArray<objParms_t, MAX_OBJ_PARMS>	  objParms;
		idArray<objHeader_t, MAX_OBJ_HEADERS> headers;
		idArray<lzwParm_t, MAX_LZW_PARMS>	  lzwParms;

		// Output memory from jobs
		idArray<lzwDelta_t, MAX_LZW_DELTAS>	  lzwDeltas; // Info about each pending delta output from jobs
		idArray<byte, MAX_LZW_MEM>			  lzwMem;	 // Memory for output from lzw jobs

		lzwInOutData_t						  lzwInOutData; // In/Out data used so lzw data can persist across lzw jobs
	};

	jobMemory_t* jobMemory;

	idSnapShot	 submittedState;

	idSnapShot	 templateStates; // holds default snapshot states for some newly spawned object
	idSnapShot	 submittedTemplateStates;

	int			 partialBaseSequence;
};

#endif /* !__SNAP_PROCESSOR_H__ */
