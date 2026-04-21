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

#ifndef __LINKLIST_H__
#define __LINKLIST_H__

/*!
	\class idLinkList
	\brief A circular doubly-linked list implementation used for managing ordered collections of objects within the engine.

	The idLinkList class provides a template-based circular doubly-linked list structure that is commonly used throughout the engine for managing collections of objects with ordered relationships. It
   supports efficient insertion, removal, and traversal operations while maintaining list integrity through careful pointer management. The list is designed to be embedded within other objects, with
   each node maintaining a link to its owner object. The circular nature of the list allows for simple iteration from any node and provides a consistent interface for list manipulation. The class is
   intended to be used as a base class for other components that need to maintain ordered collections, such as entity lists, event queues, or resource managers. Memory management for nodes is not
   specified, but the list provides mechanisms for properly cleaning up elements during destruction. The implementation treats the list as a circular structure where the head pointer points to a
   special sentinel node that is used to mark the beginning and end of the list traversal.

*/
template<class type>
class idLinkList
{
public:
	/*!
		\brief Initializes a new instance of the idLinkList class with default values.

		The constructor sets the owner pointer to NULL and initializes the head, next, and prev pointers to point to the object itself, effectively creating an empty circular doubly-linked list.

	*/
	idLinkList();

	/*!
		\brief Destructor for the idLinkList class that clears all elements from the list.

		The destructor for idLinkList calls the Clear method to ensure all elements in the list are properly removed and cleaned up. This ensures that any resources associated with the elements are
	   released when the list goes out of scope.

	*/
	~idLinkList();

	//! Checks whether the linked list is empty.
	bool		IsListEmpty() const;

	/*!
		\brief Checks if the list is in a valid state by verifying that the head pointer does not point to itself.

		This function is used to determine whether the linked list is properly initialized and not in a corrupted state. It returns true if the list is valid, indicated by the head pointer not
	   pointing to the list object itself. This is a common idiom for detecting invalid or empty lists in doubly-linked list implementations.

		\return True if the list is valid and the head pointer points to a different object, false otherwise.
	*/
	bool		InList() const;

	//! Returns the total number of elements contained in the linked list.
	int			Num() const;

	//! Clears all elements from the linked list
	void		Clear();

	/*!
		\brief Inserts this node before the specified node in the linked list.

		This function removes the current node from its existing position in the linked list and inserts it before the given node. It updates all necessary pointers to maintain the integrity of the
	   linked list structure. The operation involves setting the next pointer of the current node to point to the specified node, updating the previous pointer of the specified node to point to the
	   current node, and adjusting the previous pointer of the node that was previously before the specified node to point to the current node.

		\param node The node before which this node will be inserted
	*/
	void		InsertBefore( idLinkList& node );

	/*!
		\brief Inserts this node into the linked list after the specified node.

		This function removes the current node from its existing position in the linked list and inserts it immediately after the provided node. The operation maintains the integrity of the linked
	   list structure by properly updating the previous and next pointers of the surrounding nodes. The head pointer of the current node is set to match the head pointer of the specified node,
	   ensuring proper list traversal.

		\param node The node after which this node will be inserted
	*/
	void		InsertAfter( idLinkList& node );

	/*!
		\brief Adds all elements from another linked list to the end of this list.

		This function transfers all elements from the specified linked list to the end of the current list. The elements are inserted in the same order as they appear in the source list. The source
	   list becomes empty after this operation.

		\param node The linked list whose elements will be added to the end of this list
	*/
	void		AddToEnd( idLinkList& node );

	/*!
		\brief Adds the specified node to the front of the list.

		This function inserts the given node at the beginning of the linked list by calling InsertAfter with the head node. The node being added becomes the new first element in the list.

		\param node The node to add to the front of the list
	*/
	void		AddToFront( idLinkList& node );

	/*!
		\brief Removes the current node from the linked list by updating the links of adjacent nodes

		This function removes the current node from a doubly-linked list structure by adjusting the next and previous pointers of the neighboring nodes. It also resets the current node's own next and
	   previous pointers to point to itself, and updates the list head pointer to reference the current node. This operation effectively detaches the node from the list while maintaining the integrity
	   of the remaining structure. The function is typically used during iteration or when cleaning up nodes in a list.

	*/
	void		Remove();

	/*!
		\brief Returns the next element in the linked list or NULL if at the end

		This method is used to iterate through a linked list structure. It checks if the next pointer is valid and not pointing back to the head of the list, which would indicate the end of the list.
	   If the conditions are met, it returns the owner of the next node, otherwise it returns NULL. This is commonly used in hash table implementations where linked lists are used to handle
	   collisions.

		\return A pointer to the next element in the list, or NULL if there are no more elements
	*/
	type*		Next() const;

	/*!
		\brief Returns the previous element in the linked list or NULL if there is no previous element.

		This function retrieves the previous element in the linked list structure. It checks if the current previous pointer is valid and not equal to the head of the list. If these conditions are not
	   met, it returns NULL. Otherwise, it returns the owner of the previous node.

		\return A pointer to the previous element in the list, or NULL if there is no previous element.
	*/
	type*		Prev() const;

	//! Returns the owner object of the link list.
	type*		Owner() const;

	/*!
		\brief Sets the owner object for this link list.

		This function assigns the provided object as the owner of the link list. The owner is typically used to track which entity or object owns this list, and is often used for memory management or
	   tracking purposes.

		\param object The object to set as the owner of the link list.
	*/
	void		SetOwner( type* object );

	//! Returns a pointer to the head of the link list.
	idLinkList* ListHead() const;

	/*!
		\brief Returns the next node in the linked list or NULL if the end has been reached.

		This function traverses the linked list by returning the next node in the sequence. It checks if the current node's next pointer points to the head of the list, which indicates the end of the
	   list. In such case, it returns NULL. Otherwise, it returns the next node in the list.

		\return A pointer to the next node in the linked list, or NULL if the end of the list has been reached
	*/
	idLinkList* NextNode() const;

	/*!
		\brief Returns the previous node in the linked list or NULL if at the head.

		This method retrieves the previous node in the linked list structure. It checks if the current node is at the head of the list and returns NULL in that case, otherwise it returns the previous
	   node. The method is const and does not modify the list structure.

		\return The previous node in the linked list or NULL if the current node is at the head
	*/
	idLinkList* PrevNode() const;

private:
	idLinkList* head;
	idLinkList* next;
	idLinkList* prev;
	type*		owner;
};

template<class type>
idLinkList<type>::idLinkList()
{
	owner = NULL;
	head  = this;
	next  = this;
	prev  = this;
}

template<class type>
idLinkList<type>::~idLinkList()
{
	Clear();
}

template<class type>
bool idLinkList<type>::IsListEmpty() const
{
	return head->next == head;
}

template<class type>
bool idLinkList<type>::InList() const
{
	return head != this;
}

template<class type>
int idLinkList<type>::Num() const
{
	idLinkList<type>* node;
	int				  num;

	num = 0;
	for( node = head->next; node != head; node = node->next ) {
		num++;
	}

	return num;
}

template<class type>
void idLinkList<type>::Clear()
{
	if( head == this ) {
		while( next != this ) {
			next->Remove();
		}
	} else {
		Remove();
	}
}

template<class type>
void idLinkList<type>::Remove()
{
	prev->next = next;
	next->prev = prev;

	next = this;
	prev = this;
	head = this;
}

template<class type>
void idLinkList<type>::InsertBefore( idLinkList& node )
{
	Remove();

	next	   = &node;
	prev	   = node.prev;
	node.prev  = this;
	prev->next = this;
	head	   = node.head;
}

template<class type>
void idLinkList<type>::InsertAfter( idLinkList& node )
{
	Remove();

	prev	   = &node;
	next	   = node.next;
	node.next  = this;
	next->prev = this;
	head	   = node.head;
}

template<class type>
void idLinkList<type>::AddToEnd( idLinkList& node )
{
	InsertBefore( *node.head );
}

template<class type>
void idLinkList<type>::AddToFront( idLinkList& node )
{
	InsertAfter( *node.head );
}

template<class type>
idLinkList<type>* idLinkList<type>::ListHead() const
{
	return head;
}

template<class type>
type* idLinkList<type>::Next() const
{
	if( !next || ( next == head ) ) { return NULL; }
	return next->owner;
}

template<class type>
type* idLinkList<type>::Prev() const
{
	if( !prev || ( prev == head ) ) { return NULL; }
	return prev->owner;
}

template<class type>
idLinkList<type>* idLinkList<type>::NextNode() const
{
	if( next == head ) { return NULL; }
	return next;
}

template<class type>
idLinkList<type>* idLinkList<type>::PrevNode() const
{
	if( prev == head ) { return NULL; }
	return prev;
}

template<class type>
type* idLinkList<type>::Owner() const
{
	return owner;
}

template<class type>
void idLinkList<type>::SetOwner( type* object )
{
	owner = object;
}

#endif /* !__LINKLIST_H__ */
