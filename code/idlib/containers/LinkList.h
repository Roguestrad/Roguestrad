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
	\brief A doubly linked list implementation designed for efficient insertion and removal operations within a container of typed objects.

	This class provides a doubly linked list structure that allows for efficient insertion and removal of elements at any position within the list. It uses sentinel nodes to simplify list operations
   and avoid special cases for empty lists. The list maintains pointers to both the previous and next nodes, enabling traversal in both directions. Each node can be associated with an owner object to
   track which container it belongs to. The implementation supports common list operations such as adding elements to the front or back, inserting before or after a given node, and removing the
   current node from the list. The list is designed to be embedded within other objects, where each object can be part of multiple lists simultaneously.

*/
template<class type>
class idLinkList
{
public:
	//! Initializes an empty linked list with self-referencing sentinel nodes.
	idLinkList();

	//! Destroys the link list and clears all elements.
	~idLinkList();

	//! Checks whether the linked list is empty.
	bool		IsListEmpty() const;

	//! Checks if the list is in a linked state.
	bool		InList() const;

	//! Returns the number of elements in the linked list.
	int			Num() const;

	//! Clears all elements from the linked list
	void		Clear();

	//! Inserts this node before the specified node in the linked list.
	void		InsertBefore( idLinkList& node );

	//! Inserts this node after the specified node in a linked list.
	void		InsertAfter( idLinkList& node );

	//! Adds the specified linked list node to the end of this linked list.
	void		AddToEnd( idLinkList& node );

	//! Adds the specified node to the front of the list.
	void		AddToFront( idLinkList& node );

	//! Removes the current node from the linked list by updating adjacent node pointers.
	void		Remove();

	//! Returns the next element in the linked list or NULL if at the end.
	type*		Next() const;

	//! Returns the previous element in the linked list or NULL if there is no previous element.
	type*		Prev() const;

	//! Returns a pointer to the owner of the link list.
	type*		Owner() const;

	//! Sets the owner object for the link list.
	void		SetOwner( type* object );

	//! Returns a pointer to the head of the list.
	idLinkList* ListHead() const;

	//! Returns the next node in the list or null if at the end.
	idLinkList* NextNode() const;

	//! Returns the previous node in the linked list or NULL if at the head.
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
