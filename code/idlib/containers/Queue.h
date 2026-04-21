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
#ifndef __QUEUE_H__
#define __QUEUE_H__

/*!
	\class idQueueTemplate
	\brief A generic queue template implementation for managing sequential data elements.
*/
template<class type, int nextOffset>
class idQueueTemplate
{
public:
	//! Initializes an empty queue.
	idQueueTemplate();

	//! Adds an element to the end of the queue.
	void  Add( type* element );

	//! Returns and removes the first element from the queue
	type* Get();

private:
	type* first;
	type* last;
};

#define QUEUE_NEXT_PTR( element ) ( *( ( type** )( ( ( byte* )element ) + nextOffset ) ) )

template<class type, int nextOffset>
idQueueTemplate<type, nextOffset>::idQueueTemplate()
{
	first = last = NULL;
}

template<class type, int nextOffset>
void idQueueTemplate<type, nextOffset>::Add( type* element )
{
	QUEUE_NEXT_PTR( element ) = NULL;
	if( last ) {
		QUEUE_NEXT_PTR( last ) = element;
	} else {
		first = element;
	}
	last = element;
}

template<class type, int nextOffset>
type* idQueueTemplate<type, nextOffset>::Get()
{
	type* element;

	element = first;
	if( element ) {
		first = QUEUE_NEXT_PTR( first );
		if( last == element ) { last = NULL; }
		QUEUE_NEXT_PTR( element ) = NULL;
	}
	return element;
}

/*!
	\class idQueueNode
	\brief A template-based linked list node used within queue implementations.
*/
template<typename type>
class idQueueNode
{
public:
	//! Constructs a new queue node with no next node.
	idQueueNode() { next = NULL; }

	//! Returns the next node in the linked list
	type* GetNext() const { return next; }

	//! Sets the next node in the queue.
	void  SetNext( type* next ) { this->next = next; }

private:
	type* next;
};

/*!
	\class idQueue
	\brief A template container class implementing the queue abstract data type.
*/
template<typename type, idQueueNode<type> type::*nodePtr>
class idQueue
{
public:
	//! Initializes an empty queue with null first and last pointers.
	idQueue();

	//! Adds an element to the end of the queue
	void		Add( type* element );

	//! Removes and returns the first element from the queue.
	type*		RemoveFirst();

	//! Returns a pointer to the first element in the queue without removing it.
	type*		Peek() const;

	//! Checks if the queue is empty by verifying if the first element is NULL.
	bool		IsEmpty();

	//! Tests the queue functionality with a sample type and operations.
	static void Test();

private:
	type* first;
	type* last;
};

template<typename type, idQueueNode<type> type::*nodePtr>
idQueue<type, nodePtr>::idQueue()
{
	first = last = NULL;
}

template<typename type, idQueueNode<type> type::*nodePtr>
void idQueue<type, nodePtr>::Add( type* element )
{
	( element->*nodePtr ).SetNext( NULL );
	if( last ) {
		( last->*nodePtr ).SetNext( element );
	} else {
		first = element;
	}
	last = element;
}

template<typename type, idQueueNode<type> type::*nodePtr>
type* idQueue<type, nodePtr>::RemoveFirst()
{
	type* element;

	element = first;
	if( element ) {
		first = ( first->*nodePtr ).GetNext();
		if( last == element ) { last = NULL; }
		( element->*nodePtr ).SetNext( NULL );
	}
	return element;
}

template<typename type, idQueueNode<type> type::*nodePtr>
type* idQueue<type, nodePtr>::Peek() const
{
	return first;
}

template<typename type, idQueueNode<type> type::*nodePtr>
bool idQueue<type, nodePtr>::IsEmpty()
{
	return ( first == NULL );
}

template<typename type, idQueueNode<type> type::*nodePtr>
void idQueue<type, nodePtr>::Test()
{
	class idMyType
	{
	public:
		idQueueNode<idMyType> queueNode;
	};

	idQueue<idMyType, &idMyType::queueNode> myQueue;

	idMyType*								element = new( TAG_IDLIB ) idMyType;
	myQueue.Add( element );
	element = myQueue.RemoveFirst();
	delete element;
}

#endif // !__QUEUE_H__
