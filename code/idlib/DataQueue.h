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
#ifndef DATAQUEUE_H
#define DATAQUEUE_H

/*!
	\class idDataQueue
	\brief A fixed-size template queue for managing ordered data items with sequence numbers.

	The idDataQueue class provides a fixed-size templated data structure for managing ordered collections of binary data items, each associated with a sequence number. It is designed to support
   network communication scenarios where data sequencing and ordering are critical, such as in snapshot delta management. The queue maintains two internal buffers: one for metadata items and another
   for the actual data, allowing efficient storage and retrieval of data blocks. The class supports appending items with two data blocks, removing older items based on sequence numbers, and retrieving
   item metadata such as sequence numbers, data lengths, and data pointers. It enforces bounds checking during append operations and maintains data consistency during removals. The template parameters
   maxItems and maxBuffer define compile-time limits for the number of items and total data buffer size respectively, making the queue suitable for scenarios with predictable memory constraints.

*/
template<int maxItems, int maxBuffer>
class idDataQueue
{
public:
	/*!
		\brief Initializes an empty data queue with zero data length.

		The constructor initializes the data queue object and sets its data length to zero. This ensures that the queue starts in a valid empty state.

	*/
	idDataQueue() { dataLength = 0; }

	/*!
		\brief Appends data to the queue with the specified sequence number, returning true if successful.

		This function adds a new item to the data queue, consisting of two data blocks. It checks if there is enough space in both the items array and the data buffer before proceeding. The data
	   is copied into the buffer, and the item's metadata is updated with the sequence number, data length, and offset. Returns false if the queue is full or if the data would exceed the maximum
	   buffer size.

		\param sequence The sequence number associated with the data to be appended
		\param b1 Pointer to the first block of data to append, or NULL if not used
		\param b1Len Length of the first data block in bytes
		\param b2 Pointer to the second block of data to append, or NULL if not used
		\param b2Len Length of the second data block in bytes
		\return True if the data was successfully appended to the queue, false otherwise
	*/
	bool		Append( int sequence, const byte* b1, int b1Len, const byte* b2 = NULL, int b2Len = 0 );

	/*!
		\brief Removes items from the queue that have a sequence number smaller than the specified value

		This function removes all items from the data queue that have a sequence number less than the provided sequence parameter. It adjusts the internal data buffer and updates the offsets of
	   remaining items accordingly. The function ensures that the data structure remains consistent after items are removed.

		\param sequence The sequence number threshold, items with smaller sequence numbers will be removed
		\throws assertion failure if the data structure becomes inconsistent during removal
	*/
	void		RemoveOlderThan( int sequence );

	//! Returns the length of the data in the queue.
	int			GetDataLength() const { return dataLength; }

	//! Returns the number of items in the data queue
	int			Num() const { return items.Num(); }

	/*!
		\brief Returns the sequence number of the item at the specified index in the data queue

		This function retrieves the sequence number associated with an item stored in the data queue at the given index. The sequence number is typically used to track the order and identification
	   of snapshot deltas in network communication. It is commonly used for validation and ordering checks within the snapshot processing system.

		\param i The index of the item in the data queue for which to retrieve the sequence number
		\return The sequence number of the item at the specified index
	*/
	int			ItemSequence( int i ) const { return items[i].sequence; }

	/*!
		\brief Returns the length of the item at the specified index in the data queue

		This function retrieves the length of a specific item stored in the data queue. It is used to access the size information of queued data items for processing or compression purposes. The
	   function performs a simple array access to return the length field of the item at the given index.

		\param i The index of the item in the queue for which to retrieve the length
		\return The length of the item at the specified index
	*/
	int			ItemLength( int i ) const { return items[i].length; }

	/*!
		\brief Returns a pointer to the data of the item at the specified index in the data queue

		This function provides access to the raw data buffer of a specific item stored in the data queue. The returned pointer points to the actual data offset within the internal data buffer as
	   determined by the item's dataOffset member. The function is const, meaning it does not modify the queue state. It is typically used to retrieve message data for processing or transmission,
	   such as in network packet handling where messages need to be examined or copied. The function assumes the index is valid and within the bounds of the queue items, as no bounds checking is
	   performed internally.

		\param i The index of the item in the queue for which to retrieve the data pointer
		\return A pointer to the beginning of the data buffer for the specified item
	*/
	const byte* ItemData( int i ) const { return &data[items[i].dataOffset]; }

	//! Clears all data from the queue
	void		Clear()
	{
		dataLength = 0;
		items.Clear();
		memset( data, 0, sizeof( data ) );
	}

private:
	struct msgItem_t {
		int sequence;
		int length;
		int dataOffset;
	};
	idStaticList<msgItem_t, maxItems> items;
	int								  dataLength;
	byte							  data[maxBuffer];
};

template<int maxItems, int maxBuffer>
void idDataQueue<maxItems, maxBuffer>::RemoveOlderThan( int sequence )
{
	int length = 0;
	while( items.Num() > 0 && items[0].sequence < sequence ) {
		length += items[0].length;
		items.RemoveIndex( 0 );
	}
	if( length >= dataLength ) {
		assert( items.Num() == 0 );
		assert( dataLength == length );
		dataLength = 0;
	} else if( length > 0 ) {
		memmove( data, data + length, dataLength - length );
		dataLength -= length;
	}
	length = 0;
	for( int i = 0; i < items.Num(); i++ ) {
		items[i].dataOffset = length;
		length += items[i].length;
	}
	assert( length == dataLength );
}

template<int maxItems, int maxBuffer>
bool idDataQueue<maxItems, maxBuffer>::Append( int sequence, const byte* b1, int b1Len, const byte* b2, int b2Len )
{
	if( items.Num() == items.Max() ) { return false; }
	if( dataLength + b1Len + b2Len >= maxBuffer ) { return false; }
	msgItem_t& item = *items.Alloc();
	item.length		= b1Len + b2Len;
	item.sequence	= sequence;
	item.dataOffset = dataLength;
	if( b1 != NULL ) {
		memcpy( data + dataLength, b1, b1Len );
		dataLength += b1Len;
	}
	if( b2 != NULL ) {
		memcpy( data + dataLength, b2, b2Len );
		dataLength += b2Len;
	}
	return true;
}

#endif // DATAQUEUE_H
