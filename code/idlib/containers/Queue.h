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

/*
===============================================================================

	Queue template

===============================================================================
*/
template<class type, int nextOffset>
class idQueueTemplate
{
public:
	/*!
		\brief Initializes an empty queue template instance.

		This constructor initializes the queue by setting both the first and last pointers to NULL, effectively creating an empty queue structure.

	*/
	idQueueTemplate();

	/*!
		\brief Adds an element to the end of the queue.

		This function appends a new element to the end of the queue structure. It sets the next pointer of the new element to NULL, and updates the links in the queue so that the new element becomes
	   the new last element. If the queue was empty, the new element becomes both the first and last element. The function uses a macro QUEUE_NEXT_PTR to access the next pointer field of the element.

		\param element Pointer to the element to be added to the queue
	*/
	void  Add( type* element );

	/*!
		\brief Retrieves and removes the first element from the queue template

		This function retrieves the first element from a queue template data structure and removes it from the queue. It returns a pointer to the retrieved element or NULL if the queue is empty. The
	   function updates the internal pointers to maintain the queue structure after removal

		\return A pointer to the first element in the queue, or NULL if the queue is empty
	*/
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

/*
================================================
A node of a Queue
================================================
*/
template<typename type>
class idQueueNode
{
public:
	/*!
		\brief Initializes a new instance of idQueueNode by setting the next pointer to NULL.

		This constructor initializes a new node in a queue data structure. It sets the next pointer to NULL, indicating that this node does not point to any subsequent node in the queue. This is a
	   fundamental part of queue node initialization, ensuring that newly created nodes start in a consistent state.

	*/
	idQueueNode() { next = NULL; }

	//! Returns the next node in the queue.
	type* GetNext() const { return next; }

	/*!
		\brief Sets the next node in the queue to the specified node.

		This function updates the next pointer of the current queue node to point to the provided node. It is typically used to link nodes together in a queue data structure. The function performs a
	   simple assignment operation and does not perform any validation on the input pointer.

		\param next Pointer to the node that should follow this node in the queue
	*/
	void  SetNext( type* next ) { this->next = next; }

private:
	type* next;
};

/*
================================================
A Queue, idQueue, is a template Container class implementing the Queue abstract data
type.
================================================
*/
template<typename type, idQueueNode<type> type::*nodePtr>
class idQueue
{
public:
	/*!
		\brief Initializes an empty queue with null first and last pointers.

		This constructor initializes the queue by setting both the first and last pointers to NULL, indicating that the queue is empty and contains no elements.

	*/
	idQueue();

	/*!
		\brief Adds an element to the end of the queue

		This function appends a new element to the end of the queue data structure. It sets the next pointer of the new element to NULL, and updates the queue's last pointer to reference the new
	   element. If the queue was empty, it also updates the first pointer to point to the new element

		\param element Pointer to the element to be added to the queue
	*/
	void		Add( type* element );

	/*!
		\brief Removes and returns the first element from the queue

		This function removes the first element from the queue and returns a pointer to it. If the queue is empty, it returns NULL. The function updates the internal pointers to maintain the queue
	   structure after removal

		\return Pointer to the removed element, or NULL if the queue was empty
	*/
	type*		RemoveFirst();

	//! Returns a pointer to the first element in the queue without removing it.
	type*		Peek() const;

	/*!
		\brief Checks whether the queue is empty by verifying if the first element pointer is null

		This function determines if the queue contains no elements by checking if the first node pointer is null. It is commonly used in rendering systems to verify if interaction queues are empty
	   before processing

		\return True if the queue is empty, false otherwise
	*/
	bool		IsEmpty();

	/*!
		\brief Tests the functionality of the idQueue class with a sample type and operation

		This function demonstrates the basic usage of the idQueue template class by creating a simple test case. It defines a test class idMyType that contains a queue node, creates a queue of this
	   type, adds an element to the queue, removes it, and deletes it. This test ensures that the queue operations work correctly with the specified node pointer template parameter

	*/
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
