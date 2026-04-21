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
	\brief Manages hierarchical relationships between objects in a tree-like structure.

	The idHierarchy class provides a generic mechanism for organizing objects in a parent-child tree structure, allowing for complex hierarchical relationships. It supports establishing parent-child
   relationships, traversing the hierarchy in various orders, and managing the placement of nodes within the hierarchy. Each node maintains references to its parent, first child, and sibling nodes,
   enabling traversal and manipulation of the tree structure. The template parameter type specifies the type of object that owns the hierarchy node, facilitating type-safe operations. The class is
   designed for use with objects that need to participate in hierarchical structures, such as scene graph nodes or component hierarchies.

*/
template<class type>
class idHierarchy
{
public:
	//! Initializes a new hierarchy node with all pointers set to null.
	idHierarchy();

	//! Destroys the hierarchy node and removes it from the hierarchy.
	~idHierarchy();

	//! Sets the owner object for the hierarchy node.
	void  SetOwner( type* object );

	//! Returns the owner of the hierarchy.
	type* Owner() const;

	//! Establishes a parent-child relationship between this node and the specified node.
	void  ParentTo( idHierarchy& node );

	//! Moves this node to be a sibling after the specified node in the hierarchy.
	void  MakeSiblingAfter( idHierarchy& node );

	//! Checks if the current node is a child of the specified node in the hierarchy.
	bool  ParentedBy( const idHierarchy& node ) const;

	//! Removes the current node from its parent in the hierarchy.
	void  RemoveFromParent();

	//! Removes the hierarchy node from its parent and reattaches its children to the parent if it exists.
	void  RemoveFromHierarchy();

	//! Returns the parent node of this hierarchy node.
	type* GetParent() const;

	//! Returns the first child node of this hierarchy node.
	type* GetChild() const;

	//! Returns the sibling node of the current hierarchy node.
	type* GetSibling() const;

	//! Returns the prior sibling node in the hierarchy.
	type* GetPriorSibling() const;

	//! Retrieves the next node in the hierarchy traversal order.
	type* GetNext() const;

	//! Traverses the hierarchy to retrieve the next leaf node.
	type* GetNextLeaf() const;

private:
	idHierarchy*	   parent;
	idHierarchy*	   sibling;
	idHierarchy*	   child;
	type*			   owner;

	//! Returns the previous sibling node in the hierarchy with the same parent
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
