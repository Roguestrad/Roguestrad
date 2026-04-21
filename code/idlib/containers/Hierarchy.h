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

#ifndef __HIERARCHY_H__
#define __HIERARCHY_H__

/*!
	\class idHierarchy
	\brief A templated hierarchy node class for managing parent-child relationships in a tree structure.

	The idHierarchy class provides a generic mechanism for constructing and manipulating hierarchical data structures. It supports standard tree operations such as parenting, sibling ordering, and
   traversal. The class is designed to work with any type that is compatible with the template parameter, making it flexible for various use cases within the engine. Each node maintains links to its
   parent, first child, and sibling nodes, enabling efficient navigation through the hierarchy. The implementation handles edge cases such as removing nodes from the hierarchy and reattaching children
   appropriately. The class provides methods for checking parent-child relationships and utility functions for traversing the hierarchy. Memory management is handled through the destructors which
   properly clean up node references and maintain hierarchy integrity during destruction.

*/
template<class type>
class idHierarchy
{
public:
	/*!
		\brief Initializes a new instance of the idHierarchy class with all pointers set to null.

		The constructor initializes all member pointers of the idHierarchy class to NULL. This sets up the hierarchy node with no owner, parent, sibling, or child relationships. The node is
	   effectively initialized as a standalone element in a hierarchical structure.

	*/
	idHierarchy();

	/*!
		\brief Destroys the hierarchy node and removes it from the hierarchy.

		The destructor for the idHierarchy template class cleans up the hierarchy node by removing it from the hierarchy structure. This ensures proper cleanup of resources and prevents dangling
	   references within the hierarchy.

	*/
	~idHierarchy();

	/*!
		\brief Sets the owner object for this hierarchy node.

		This function assigns the provided object as the owner of the hierarchy node. The owner is typically used to reference the parent or container object that manages this node.

		\param object The object to be set as the owner of this hierarchy node
	*/
	void  SetOwner( type* object );

	//! Returns the owner of the hierarchy.
	type* Owner() const;

	/*!
		\brief Moves this node to become a child of the specified node.

		This function reparents the current node to become a child of the given node. It first removes the current node from its existing parent, then updates the parent and sibling pointers to
	   reflect the new hierarchical relationship. The node becomes the first child of the target node, pushing any existing children down the sibling chain.

		\param node The target node to become the parent of this node
	*/
	void  ParentTo( idHierarchy& node );

	/*!
		\brief Moves this node to be a sibling after the specified node in the hierarchy.

		This function reorganizes the hierarchy by removing this node from its current parent and placing it as a sibling immediately after the given node. The node's parent is set to match the given
	   node's parent, and the sibling links are updated to maintain the correct order within the hierarchy.

		\param node The node after which this node should be placed as a sibling
	*/
	void  MakeSiblingAfter( idHierarchy& node );

	/*!
		\brief Checks if the current node is a child of the specified node in the hierarchy

		This function recursively traverses the hierarchy up from the current node to determine if the given node is its parent. It returns true if the specified node is directly or indirectly the
	   parent of the current node, and false otherwise. The function handles the case where the current node has no parent by returning false.

		\param node The node to check as a parent
		\return True if the specified node is a parent of the current node, false otherwise
	*/
	bool  ParentedBy( const idHierarchy& node ) const;

	/*!
		\brief Removes this node from its parent hierarchy and adjusts child links accordingly.

		This function detaches the current node from its parent by updating the sibling links of the previous sibling or the parent's child pointer. It then sets the parent and sibling pointers of the
	   current node to NULL, effectively removing it from the hierarchy. The function does not reattach children to the parent, as indicated by the implementation.

	*/
	void  RemoveFromParent();

	/*!
		\brief Removes this node from the hierarchy and reattaches its children to the parent node if one exists.

		This function first removes the current node from its parent by calling RemoveFromParent. If the node has a parent, it then iterates through all child nodes, removes each one from its parent,
	   and reattaches them to the original parent node. If the node does not have a parent, it simply removes all child nodes from the hierarchy without reattaching them.

	*/
	void  RemoveFromHierarchy();

	//! Returns the parent node of this hierarchy node.
	type* GetParent() const;

	//! Returns the first child node of this hierarchy node.
	type* GetChild() const;

	//! Returns the sibling node in the hierarchy with the same parent
	type* GetSibling() const;

	//! Returns the prior sibling node in the hierarchy.
	type* GetPriorSibling() const;

	//! Returns the next node in the hierarchy traversal order
	type* GetNext() const;

	//! Returns the next leaf node in the hierarchy traversal.
	type* GetNextLeaf() const;

private:
	idHierarchy*	   parent;
	idHierarchy*	   sibling;
	idHierarchy*	   child;
	type*			   owner;

	//! Returns the previous sibling node with the same parent, or NULL if this node is the first child.
	idHierarchy<type>* GetPriorSiblingNode() const;
};

template<class type>
idHierarchy<type>::idHierarchy()
{
	owner	= NULL;
	parent	= NULL;
	sibling = NULL;
	child	= NULL;
}

template<class type>
idHierarchy<type>::~idHierarchy()
{
	RemoveFromHierarchy();
}

template<class type>
type* idHierarchy<type>::Owner() const
{
	return owner;
}

template<class type>
void idHierarchy<type>::SetOwner( type* object )
{
	owner = object;
}

template<class type>
bool idHierarchy<type>::ParentedBy( const idHierarchy& node ) const
{
	if( parent == &node ) {
		return true;
	} else if( parent ) {
		return parent->ParentedBy( node );
	}
	return false;
}

template<class type>
void idHierarchy<type>::ParentTo( idHierarchy& node )
{
	RemoveFromParent();

	parent	   = &node;
	sibling	   = node.child;
	node.child = this;
}

template<class type>
void idHierarchy<type>::MakeSiblingAfter( idHierarchy& node )
{
	RemoveFromParent();
	parent		 = node.parent;
	sibling		 = node.sibling;
	node.sibling = this;
}

template<class type>
void idHierarchy<type>::RemoveFromParent()
{
	idHierarchy<type>* prev;

	if( parent ) {
		prev = GetPriorSiblingNode();
		if( prev ) {
			prev->sibling = sibling;
		} else {
			parent->child = sibling;
		}
	}

	parent	= NULL;
	sibling = NULL;
}

template<class type>
void idHierarchy<type>::RemoveFromHierarchy()
{
	idHierarchy<type>* parentNode;
	idHierarchy<type>* node;

	parentNode = parent;
	RemoveFromParent();

	if( parentNode ) {
		while( child ) {
			node = child;
			node->RemoveFromParent();
			node->ParentTo( *parentNode );
		}
	} else {
		while( child ) {
			child->RemoveFromParent();
		}
	}
}

template<class type>
type* idHierarchy<type>::GetParent() const
{
	if( parent ) { return parent->owner; }
	return NULL;
}

template<class type>
type* idHierarchy<type>::GetChild() const
{
	if( child ) { return child->owner; }
	return NULL;
}

template<class type>
type* idHierarchy<type>::GetSibling() const
{
	if( sibling ) { return sibling->owner; }
	return NULL;
}

template<class type>
idHierarchy<type>* idHierarchy<type>::GetPriorSiblingNode() const
{
	if( !parent || ( parent->child == this ) ) { return NULL; }

	idHierarchy<type>* prev;
	idHierarchy<type>* node;

	node = parent->child;
	prev = NULL;
	while( ( node != this ) && ( node != NULL ) ) {
		prev = node;
		node = node->sibling;
	}

	if( node != this ) { idLib::Error( "idHierarchy::GetPriorSibling: could not find node in parent's list of children" ); }

	return prev;
}

/*
================
idHierarchy<type>::GetPriorSibling

Returns NULL if no parent, or if it is the first child.
================
*/
template<class type>
type* idHierarchy<type>::GetPriorSibling() const
{
	idHierarchy<type>* prior;

	prior = GetPriorSiblingNode();
	if( prior ) { return prior->owner; }

	return NULL;
}

template<class type>
type* idHierarchy<type>::GetNext() const
{
	const idHierarchy<type>* node;

	if( child ) {
		return child->owner;
	} else {
		node = this;
		while( node && node->sibling == NULL ) {
			node = node->parent;
		}
		if( node ) {
			return node->sibling->owner;
		} else {
			return NULL;
		}
	}
}

template<class type>
type* idHierarchy<type>::GetNextLeaf() const
{
	const idHierarchy<type>* node;

	if( child ) {
		node = child;
		while( node->child ) {
			node = node->child;
		}
		return node->owner;
	} else {
		node = this;
		while( node && node->sibling == NULL ) {
			node = node->parent;
		}
		if( node ) {
			node = node->sibling;
			while( node->child ) {
				node = node->child;
			}
			return node->owner;
		} else {
			return NULL;
		}
	}
}

#endif /* !__HIERARCHY_H__ */
