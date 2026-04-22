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

//! Removes items from the queue that have a sequence number older than the specified value.
template<int maxItems, int maxBuffer>
class idDataQueue
{
	//! Initializes the data queue with zero length.
public:
	//! Appends data to the queue with optional second buffer
	idDataQueue() { dataLength = 0; }

	/*!
		\brief Appends data to the queue with the specified sequence number and returns true if successful

		This function adds data to a data queue structure. It takes two data segments, b1 and b2, along with their respective lengths, and appends them to an internal buffer. The data is associated
	   with a sequence number. The function checks if there is enough space in the queue and buffer before appending. If the queue is full or the buffer would overflow, the function returns false.
	   Otherwise, it returns true after successfully appending the data.

		\param sequence The sequence number to associate with the appended data
		\param b1 Pointer to the first segment of data to append
		\param b1Len Length of the first data segment
		\param b2 Pointer to the second segment of data to append, can be NULL
		\param b2Len Length of the second data segment, ignored if b2 is NULL
		\return True if the data was successfully appended to the queue, false otherwise
	*/
	bool		Append( int sequence, const byte* b1, int b1Len, const byte* b2 = NULL, int b2Len = 0 );

	//! Removes items from the queue that have a sequence number older than the specified value.
	void		RemoveOlderThan( int sequence );

	//! Returns the length of the data in the queue.
	int			GetDataLength() const { return dataLength; }

	//! Returns the number of items in the data queue
	int			Num() const { return items.Num(); }

	//! Returns the sequence number of the i-th item in the data queue.
	int			ItemSequence( int i ) const { return items[i].sequence; }

	//! Returns the length of the item at the specified index in the data queue
	int			ItemLength( int i ) const { return items[i].length; }

	//! Returns a pointer to the data of the item at the specified index in the queue
	const byte* ItemData( int i ) const { return &data[items[i].dataOffset]; }

	//! Clears all data, items, and resets the buffer.
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
