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
#ifndef __PACKET_PROCESSOR_H__
#define __PACKET_PROCESSOR_H__

/*!
	\class idPacketProcessor
	\brief A packet processor that manages outgoing and incoming network communication with rate limiting and reliable message handling.

	This class handles network packet processing for both outgoing and incoming messages, including reliable message queuing, rate limiting, and packet fragmentation. It maintains statistics for
   outgoing and incoming data rates and provides methods for queuing and processing reliable acknowledgments. The processor supports both in-band and out-of-band communication and handles
   connectionless packets for peer-to-peer communication scenarios. It tracks packet transmission times, manages reliable message state, and ensures proper message sequencing and reassembly of
   fragmented packets. The class also provides functionality for verifying and cleaning the reliable message queue, as well as processing connectionless outgoing and incoming messages with appropriate
   headers.

*/
class idPacketProcessor
{
public:
	// DG: workaround for GCC bug (can't link when compiling with -O0): put definitions in PacketProcessor.cpp
	static const int		 RETURN_TYPE_NONE;	 //			= 0;
	static const int		 RETURN_TYPE_OOB;	 //			= 1;
	static const int		 RETURN_TYPE_INBAND; //			= 2;
	// DG end
	typedef uint16			 sessionId_t;

	static const int		 NUM_LOBBY_TYPE_BITS = 2;
	static const int		 LOBBY_TYPE_MASK	 = ( 1 << NUM_LOBBY_TYPE_BITS ) - 1;

	static const sessionId_t SESSION_ID_INVALID					  = 0;
	static const sessionId_t SESSION_ID_CONNECTIONLESS_PARTY	  = 1;
	static const sessionId_t SESSION_ID_CONNECTIONLESS_GAME		  = 2;
	static const sessionId_t SESSION_ID_CONNECTIONLESS_GAME_STATE = 3;

	static const int		 BANDWIDTH_AVERAGE_PERIOD = 250;

	//! Constructs an idPacketProcessor object and initializes it.
	idPacketProcessor() { Reset(); }

	//! Resets all internal state variables and data queues of the packet processor.
	void Reset()
	{
		msgWritePos		 = 0;
		fragmentSequence = 0;
		droppedFrags	 = 0;
		fragmentedSend	 = false;

		reliable = idDataQueue<MAX_RELIABLE_QUEUE, MAX_MSG_SIZE>();

		reliableSequenceSend = 1;
		reliableSequenceRecv = 0;

		numReliable = 0;

		queuedReliableAck = -1;

		unsentMsg = idBitMsg();

		lastSendTime = 0;

		outgoingRateTime  = 0;
		outgoingRateBytes = 0.0f;
		incomingRateTime  = 0;
		incomingRateBytes = 0.0f;

		outgoingBytes = 0;
		incomingBytes = 0;

		currentOutgoingRate	 = 0;
		lastOutgoingRateTime = 0;
		lastOutgoingBytes	 = 0;
		currentIncomingRate	 = 0;
		lastIncomingRateTime = 0;
		lastIncomingBytes	 = 0;

		fragmentAccumulator = 0;
	}

	static const int MAX_MSG_SIZE = 8000; // This is the max size you can pass into ProcessOutgoing
	static const int MAX_FINAL_PACKET_SIZE =
		1200; // Lowest/safe MTU across all our platforms to avoid fragmentation at the transport layer (which is poorly supported by consumer hardware and may cause nasty latency side effects)
	static const int MAX_RELIABLE_QUEUE = 64;

	// TypeInfo doesn't like sizeof( sessionId_t )?? and then fails to understand the #ifdef/#else/#endif
	// static const int MAX_PACKET_SIZE		= MAX_FINAL_PACKET_SIZE - 6 - sizeof( sessionId_t );	// Largest possible packet before headers and such applied (subtract some for various internal
	// header data, and session id)
	static const int MAX_PACKET_SIZE  = MAX_FINAL_PACKET_SIZE - 6 - 2; // Largest possible packet before headers and such applied (subtract some for various internal header data, and session id)
	static const int MAX_OOB_MSG_SIZE = MAX_PACKET_SIZE - 1;		   // We don't allow fragmentation for out-of-band msg's, and we need a byte for the header

private:
	//! Queues a reliable acknowledgment for the specified sequence number.
	void QueueReliableAck( int lastReliable );

	//! Finalizes reading a packet by processing its header and extracting reliable data.
	int	 FinalizeRead( idBitMsg& inMsg, idBitMsg& outMsg, int& userValue );

public:
	//! Returns true if more data can be sent based on the rate limit.
	bool CanSendMoreData() const;

	//! Updates the outgoing packet rate calculation based on the given time and size parameters.
	void UpdateOutgoingRate( const int time, const int size );

	//! Updates the incoming packet rate calculation based on the provided time and size parameters.
	void UpdateIncomingRate( const int time, const int size );

	//! Refreshes the outgoing and incoming packet rates.
	void RefreshRates( int time )
	{
		UpdateOutgoingRate( time, 0 );
		UpdateIncomingRate( time, 0 );
	}

	//! Queues a reliable message to be sent on the next process outgoing call.
	bool			   QueueReliableMessage( byte type, const byte* data, int dataLen );

	/*!
		\brief Processes an outgoing message for sending, potentially fragmenting it if necessary.

		This function prepares an outgoing message for transmission by initializing the unsent message buffer and handling reliable data accumulation. It supports both in-band and out-of-band packet
	   types, with special handling for reliable acknowledgments. For in-band packets, reliable messages are compressed using LZW compression before being sent. The function ensures that fragmentation
	   is properly handled for large messages. It returns false if the message cannot be processed due to internal state violations or size constraints.

		\param time The current time for processing
		\param msg The message data to be sent
		\param isOOB Flag indicating whether this is an out-of-band packet
		\param userData User data to be included in the packet header
		\return True if the message was successfully processed for sending, false otherwise
	*/
	bool			   ProcessOutgoing( const int time, const idBitMsg& msg, bool isOOB, int userData );

	//! Retrieves the next send fragment for transmission through the network connection
	bool			   GetSendFragment( const int time, sessionId_t sessionID, idBitMsg& outMsg );

	/*!
		\brief Processes an incoming packet, handling both regular and fragmented messages, and returns the appropriate processing result type.

		This function processes incoming network packets, handling both regular and fragmented packets. It validates the packet session ID against the expected one, checks for connectionless IDs, and
	   manages fragmented packet reconstruction. For fragmented packets, it tracks sequence numbers and rebuilds the complete message before finalizing the read operation. The function returns
	   different values based on whether the packet was processed successfully, requires more fragments, or indicates an error.

		\param time Current system time in milliseconds
		\param expectedSessionID Expected session ID to validate against the packet's session ID
		\param msg Input message containing the packet data
		\param out Output message for the reconstructed packet data
		\param userData Reference to user data associated with the packet
		\param peerNum Peer number identifier for the sender
		\return Return type indicating the result of packet processing: RETURN_TYPE_NONE for incomplete fragments or errors, RETURN_TYPE_OOB for out-of-band connectionless packets, or other values for
	   successfully processed regular packets. \throws Throws an error when there is a fragmented message buffer overflow.
	*/
	int				   ProcessIncoming( int time, sessionId_t expectedSessionID, idBitMsg& msg, idBitMsg& out, int& userData, const int peerNum );

	//! Returns true if there are more message fragments remaining to be sent
	bool			   HasMoreFragments() const { return ( unsentMsg.GetRemainingData() > 0 ); }

	//! Returns the number of reliable messages that are currently queued and not yet acknowledged.
	int				   NumQueuedReliables() { return reliable.Num(); }

	//! Returns true if a reliable ack needs to be sent
	int				   NeedToSendReliableAck() { return queuedReliableAck >= 0 ? true : false; }

	/*!
		\brief Processes connectionless outgoing messages by wrapping them with appropriate headers for out-of-band communication.

		This function prepares a connectionless outgoing message by adding outer and inner packet headers. It constructs an outer header using a session ID derived from the lobby type and an inner
	   header with a specified packet type and user data. The original message data is then appended to the output buffer. The function is used for communicating with non-connected peers in
	   out-of-band scenarios.

		\param msg The input message data to be sent
		\param out The output buffer where the complete packet will be written
		\param lobbyType The lobby type identifier used to derive the session ID
		\param userData User-defined data to be included in the inner packet header
		\return true indicating successful processing of the connectionless outgoing message
	*/
	static bool		   ProcessConnectionlessOutgoing( idBitMsg& msg, idBitMsg& out, int lobbyType, int userData );

	//! Processes incoming connectionless packets and extracts user data and message content
	static bool		   ProcessConnectionlessIncoming( idBitMsg& msg, idBitMsg& out, int& userData );

	//! Retrieves the session ID from a message without consuming the read state
	static sessionId_t GetSessionID( idBitMsg& msg );

	//! Returns the number of reliable packets in the packet processor.
	int				   GetNumReliables() const { return numReliable; }

	//! Returns a pointer to the reliable message at the specified index.
	const byte*		   GetReliable( int i ) const { return reliableMsgPtrs[i]; }

	//! Returns the size of a reliable message at the specified index.
	int				   GetReliableSize( int i ) const { return reliableMsgSize[i]; }

	//! Sets the last send time value to the provided integer.
	void			   SetLastSendTime( int i ) { lastSendTime = i; }

	//! Returns the timestamp of the last send operation performed by this packet processor.
	int				   GetLastSendTime() const { return lastSendTime; }

	//! Returns the outgoing data rate in bytes per second.
	float			   GetOutgoingRateBytes() const { return outgoingRateBytes; }

	//! Returns the number of outgoing bytes processed by this packet processor.
	int				   GetOutgoingBytes() const { return outgoingBytes; }

	//! Returns the incoming data rate in bytes per second.
	float			   GetIncomingRateBytes() const { return incomingRateBytes; }

	//! Returns the number of incoming bytes processed by the packet processor.
	int				   GetIncomingBytes() const { return incomingBytes; }

	//! Returns the current outgoing packet rate computed over a small interval for more reliable network statistics.
	int				   GetOutgoingRate2() const { return currentOutgoingRate; }

	//! Returns the current incoming data rate in bytes per second.
	int				   GetIncomingRate2() const { return currentIncomingRate; }

	//! Decrements a fragmentation counter and returns true if the counter was greater than zero.
	bool			   TickFragmentAccumulator()
	{
		if( fragmentAccumulator > 0 ) {
			fragmentAccumulator--;
			return true;
		}
		return false;
	}

	//! Returns the size of reliable data in the packet processor.
	int	 GetReliableDataSize() const { return reliable.GetDataLength(); }

	//! Verifies and cleans the reliable message queue by keeping messages below a threshold and replacing others with a specified message.
	void VerifyEmptyReliableQueue( byte keepMsgBelowThis, byte replaceWithThisMsg );

private:
	// Packet header types
	static const int PACKET_TYPE_INBAND		  = 0; // In-band. Number of reliable msg's stored in userData portion of header
	static const int PACKET_TYPE_OOB		  = 1; // Out-of-band. userData free to use by the caller. Cannot fragment.
	static const int PACKET_TYPE_RELIABLE_ACK = 2; // Header type used to piggy-back on top of msgs to ack reliable msg's
	static const int PACKET_TYPE_FRAGMENTED	  = 3; // The msg is fragmented, fragment type stored in the userData portion of header

	// PACKET_TYPE_FRAGMENTED userData values
	// DG: workaround for GCC bug (can't link when compiling with -O0): put definitions in PacketProcessor.cpp
	static const int FRAGMENT_START;  //				= 0;
	static const int FRAGMENT_MIDDLE; //			= 1;
	static const int FRAGMENT_END;	  //			= 2;

	/*!
		\class idPacketProcessor::idOuterPacketHeader
		\brief A header structure for outer packets that contains session identification information.
	*/
	class idOuterPacketHeader
	{
	public:
		//! Initializes a new instance of idOuterPacketHeader with an invalid session ID.
		idOuterPacketHeader() :
			sessionID( SESSION_ID_INVALID )
		{
		}

		//! Initializes a new outer packet header with the specified session ID.
		idOuterPacketHeader( sessionId_t sessionID_ ) :
			sessionID( sessionID_ )
		{
		}

		//! Writes the session ID to the message
		void		WriteToMsg( idBitMsg& msg ) { msg.WriteUShort( sessionID ); }

		//! Reads the session ID from the provided bit message.
		void		ReadFromMsg( idBitMsg& msg ) { sessionID = msg.ReadUShort(); }

		//! Returns the session ID stored in the outer packet header
		sessionId_t GetSessionID() { return sessionID; }

	private:
		sessionId_t sessionID;
	};

	/*!
		\class idPacketProcessor::idInnerPacketHeader
		\brief A class representing the header structure for inner packet data within a packet processing system.
	*/
	class idInnerPacketHeader
	{
	public:
		//! Initializes a new instance of the idInnerPacketHeader class with default values for type and userData fields.
		idInnerPacketHeader() :
			type( 0 ),
			userData( 0 )
		{
		}

		//! Constructs an inner packet header with the specified type and user data.
		idInnerPacketHeader( int inType, int inData ) :
			type( inType ),
			userData( inData )
		{
		}

		//! Writes the packet header data to the provided message buffer.
		void WriteToMsg( idBitMsg& msg )
		{
			msg.WriteBits( type, 2 );
			msg.WriteBits( userData, 6 );
		}

		//! Reads packet header information from a bit message.
		void ReadFromMsg( idBitMsg& msg )
		{
			type	 = msg.ReadBits( 2 );
			userData = msg.ReadBits( 6 );
		}

		//! Returns the type identifier of the inner packet header.
		int Type() { return type; }

		//! Returns the user data value stored in the inner packet header
		int Value() { return userData; }

	private:
		int type;
		int userData;
	};

	byte										  msgBuffer[MAX_MSG_SIZE]; // Buffer used to reconstruct the msg
	int											  msgWritePos;			   // Write position into the msg reconstruction buffer
	int											  fragmentSequence;		   // Fragment sequence number
	int											  droppedFrags;			   // Number of dropped fragments
	bool										  fragmentedSend;		   // Used to determine if the current send requires fragmenting

	idDataQueue<MAX_RELIABLE_QUEUE, MAX_MSG_SIZE> reliable; // list of unacknowledged reliable messages

	int											  reliableSequenceSend; // sequence number of the next reliable packet we're going to send to this peer
	int											  reliableSequenceRecv; // sequence number of the last reliable packet we received from this peer

	// These are for receiving reliables, you need to get these before the next process call or they will get cleared
	int											  numReliable;
	byte										  reliableBuffer[MAX_MSG_SIZE]; // We shouldn't have to hold more than this
	const byte*									  reliableMsgPtrs[MAX_RELIABLE_QUEUE];
	int											  reliableMsgSize[MAX_RELIABLE_QUEUE];

	int											  queuedReliableAck; // Used to piggy back on the next send to ack reliables

	idBitMsg									  unsentMsg;
	byte										  unsentBuffer[MAX_MSG_SIZE]; // Buffer used hold the current msg until it's all sent

	int											  lastSendTime;

	// variables to keep track of the rate
	int											  outgoingRateTime;
	float										  outgoingRateBytes; // B/S
	int											  incomingRateTime;
	float										  incomingRateBytes; // B/S

	int											  outgoingBytes;
	int											  incomingBytes;

	int											  currentOutgoingRate;
	int											  lastOutgoingRateTime;
	int											  lastOutgoingBytes;
	int											  currentIncomingRate;
	int											  lastIncomingRateTime;
	int											  lastIncomingBytes;

	int											  fragmentAccumulator; // counts max size packets we are sending for the net debug hud
};

#endif /* !__PACKET_PROCESSOR_H__ */
