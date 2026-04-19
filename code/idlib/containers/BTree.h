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

#ifndef __BTREE_H__
#define __BTREE_H__

/*
===============================================================================

	Balanced Search Tree

===============================================================================
*/

// #define BTREE_CHECK

template<class objType, class keyType>
class idBTreeNode
{
public:
	keyType		 key;		  // key used for sorting
	objType*	 object;	  // if != NULL pointer to object stored in leaf node
	idBTreeNode* parent;	  // parent node
	idBTreeNode* next;		  // next sibling
	idBTreeNode* prev;		  // prev sibling
	int			 numChildren; // number of children
	idBTreeNode* firstChild;  // first child
	idBTreeNode* lastChild;	  // last child
};

template<class objType, class keyType, int maxChildrenPerNode>
class idBTree
{
public:
	/*!
		\brief Constructs an empty B-tree with the specified template parameters.

		Initializes the B-tree by setting the root node to NULL and ensuring that the maximum number of children per node is at least four.

		\throws Assertion failure if maxChildrenPerNode is less than four.
	*/
	idBTree();

	/*!
		\brief Destroys the binary tree and releases its resources.

		The destructor for the idBTree class cleans up all dynamically allocated memory and releases any resources held by the tree. It calls the Shutdown method to ensure proper cleanup of the tree's
	   internal structure before the object is destroyed.

	*/
	~idBTree();

	/*!
		\brief Initializes the B-tree by allocating and setting the root node

		This function initializes the B-tree data structure by allocating a new node and setting it as the root node. It is typically called during the construction or reset of the B-tree to establish
	   its initial state. The function assumes that the B-tree is being constructed from scratch and does not perform any cleanup or deallocation of existing nodes.

	*/
	void						   Init();

	/*!
		\brief Clears all nodes and resets the root pointer of the B-Tree data structure

		This function deallocates all memory used by the nodes in the B-Tree by calling the allocator's Shutdown method. It also sets the root pointer to NULL, effectively resetting the tree to an
	   empty state. This is typically called during the shutdown phase of the application to properly clean up the B-Tree data structure.

	*/
	void						   Shutdown();

	/*!
		\brief Adds an object to the B-tree with the specified key and returns a pointer to the new node.

		This function inserts a new object into the B-tree data structure. The object is associated with a key, and the tree is maintained in a balanced manner. If the root node is empty, it allocates
	   a new node and sets it as the root. The function handles splitting nodes when they exceed the maximum number of children allowed per node. It traverses the tree to find the appropriate location
	   for the new object based on the key value, ensuring the tree structure remains valid.

		\param object Pointer to the object to be added to the tree
		\param key Key value used to determine the position of the object in the tree
		\return Pointer to the newly created node in the B-tree that contains the added object
	*/
	idBTreeNode<objType, keyType>* Add( objType* object, keyType key );

	/*!
		\brief Removes a node from the B-tree structure and maintains tree properties.

		This function removes a specified node from the B-tree by unlinking it from its parent and adjusting the parent's child links. It then handles the reorganization of parent nodes to ensure that
	   no parent has fewer than two children, merging nodes when necessary. The function also updates the key values of parent nodes to ensure they remain consistent with their children. After
	   removing the node, it frees the memory allocated for the node. If the root node ends up with only one internal child, the function promotes that child to become the new root.

		\param node Pointer to the node to be removed from the tree
		\throws assertion failure if the node's object pointer is null
	*/
	void						   Remove( idBTreeNode<objType, keyType>* node );

	/*!
		\brief Finds a node in the B-tree with a key greater than or equal to the specified key

		This function searches the B-tree structure to locate a node with the smallest key that is greater than or equal to the given key value. It traverses the tree starting from the root node and
	   follows the child pointers until it finds a suitable node. If the tree is empty, it returns NULL. The function ensures that the found node either has a key exactly equal to the given key or a
	   key that is greater than the given key. If no such node can be found, it returns NULL

		\param key The key value to search for in the B-tree
		\return A pointer to the B-tree node with the smallest key greater than or equal to the given key, or NULL if no such node exists
	*/
	idBTreeNode<objType, keyType>* NodeFind( keyType key ) const;

	/*!
		\brief Finds the node with the smallest key that is greater than or equal to the given key.

		This function searches through the B-tree structure to locate the node with the smallest key that is greater than or equal to the specified key value. It starts from the root node and
	   traverses down the tree until it finds a suitable node. If the tree is empty, it returns NULL. The function ensures that the found node either has a key exactly equal to the given key or a key
	   that is greater than the given key. If no such node can be found, it returns NULL.

		\param key The key value to search for in the B-tree
		\return A pointer to the B-tree node with the smallest key greater than or equal to the given key, or NULL if no such node exists
	*/
	idBTreeNode<objType, keyType>* NodeFindSmallestLargerEqual( keyType key ) const;

	/*!
		\brief Finds the node with the largest key less than or equal to the given key in the B-tree.

		This function traverses the B-tree to locate the node with the largest key that is less than or equal to the specified key value. It starts from the root node and navigates through the tree
	   structure, examining each node's key values. If no such node exists, it returns NULL. The function handles edge cases such as an empty tree or when no suitable node is found.

		\param key The key value to search for in the B-tree
		\return A pointer to the B-tree node with the largest key less than or equal to the given key, or NULL if no such node exists
	*/
	idBTreeNode<objType, keyType>* NodeFindLargestSmallerEqual( keyType key ) const;

	/*!
		\brief Finds and returns an object in the B-tree using the specified key

		This function searches for an object in the B-tree data structure using the provided key. It first locates the appropriate node using the NodeFind method, and then returns the object stored in
	   that node. If no node is found for the given key, the function returns NULL

		\param key The key value used to search for the object in the B-tree
		\return A pointer to the found object, or NULL if no object with the specified key exists in the B-tree
	*/
	objType*					   Find( keyType key ) const;

	/*!
		\brief Finds the object with the smallest key that is greater than or equal to the given key.

		This function searches through the b-tree structure to locate the object whose key is the smallest among all keys that are greater than or equal to the specified key. If no such object exists,
	   it returns NULL.

		\param key The key to compare against for finding the smallest larger or equal key.
		\return A pointer to the object with the smallest key greater than or equal to the specified key, or NULL if no such object exists.
	*/
	objType*					   FindSmallestLargerEqual( keyType key ) const;

	/*!
		\brief Finds an object with the largest key smaller than or equal to the given key

		This function searches the B-tree structure to locate an object whose key is the largest value that is less than or equal to the specified key. It returns a pointer to the object if found, or
	   NULL if no such object exists in the tree

		\param key The key value to search for in the B-tree
		\return A pointer to the object with the largest key smaller than or equal to the given key, or NULL if no such object exists
	*/
	objType*					   FindLargestSmallerEqual( keyType key ) const;

	//! Returns the root node of the tree.
	idBTreeNode<objType, keyType>* GetRoot() const;

	//! Returns the total number of nodes in the tree.
	int							   GetNodeCount() const;

	/*!
		\brief Returns the next node in the tree traversal order starting from the given node

		This function performs a tree traversal by returning the next node in the natural order of the tree structure. If the given node has a first child, it returns that child. Otherwise, it
	   traverses up the tree to find the next sibling or parent node that has a next sibling, returning that node. This approach allows for visiting all nodes in the tree in a depth-first manner.

		\param node The current node in the tree traversal
		\return The next node in the tree traversal sequence, or NULL if there are no more nodes
	*/
	idBTreeNode<objType, keyType>* GetNext( idBTreeNode<objType, keyType>* node ) const;

	/*!
		\brief Returns the next leaf node in the tree traversal order following the given leaf node.

		This function traverses the tree structure to find the next leaf node in a specific order. It first checks if the given node has any children. If it does, it moves down to the leftmost child.
	   If the node has no children, it backtracks up the tree until it finds a node with a next sibling, then moves to that sibling and continues down to the leftmost child of the sibling. If no such
	   node exists, it returns NULL.

		\param node The starting leaf node from which the next leaf node is to be found
		\return A pointer to the next leaf node in the tree, or NULL if there are no more leaf nodes.
	*/
	idBTreeNode<objType, keyType>* GetNextLeaf( idBTreeNode<objType, keyType>* node ) const;

private:
	idBTreeNode<objType, keyType>*					 root;
	idBlockAlloc<idBTreeNode<objType, keyType>, 128> nodeAllocator;

	/*!
		\brief Allocates and initializes a new B-tree node from the node allocator.

		This function allocates a new B-tree node using the internal node allocator and initializes all its member variables to their default values. The allocated node is initialized with null
	   parent, no children, no associated object, and zero child count. This function is typically used when constructing the collision model data structure during file loading or parsing operations.

		\return A pointer to the newly allocated and initialized B-tree node.
	*/
	idBTreeNode<objType, keyType>*					 AllocNode();

	/*!
		\brief Frees the memory allocated for a B-tree node using the node allocator

		This function releases the memory occupied by a B-tree node back to the allocator pool. It is used internally by the B-tree implementation to manage memory for nodes that are no longer needed.
	   The function takes a pointer to the node to be freed and uses the nodeAllocator to return the memory to the system

		\param node Pointer to the B-tree node to be freed
	*/
	void											 FreeNode( idBTreeNode<objType, keyType>* node );

	/*!
		\brief Splits a B-tree node into two nodes when the node exceeds the maximum number of children

		This function splits a B-tree node when it exceeds the maximum number of children allowed per node. It creates a new node and redistributes the children between the original node and the new
	   node. The split occurs at the middle of the children, with the new node containing the second half of the children. The function properly updates parent pointers and sibling links to maintain
	   the B-tree structure. The parent node's child count is incremented to account for the new child node.

		\param node The node to be split, which must be full and exceed the maximum number of children allowed
		\throws assertion failure if the parent node exceeds the maximum number of children allowed
	*/
	void											 SplitNode( idBTreeNode<objType, keyType>* node );

	/*!
		\brief Merges two b-tree nodes by combining their children and updating parent references

		This function merges two consecutive sibling nodes in a b-tree structure. It takes two nodes that are adjacent siblings with no objects and at least one child each. The function transfers all
	   children from the first node to the second node, updates all parent pointers of the children, and maintains the correct tree structure by removing the first node from the parent's child list.
	   The merged node is returned, while the first node is freed.

		\param node1 First b-tree node to be merged, must be adjacent to node2
		\param node2 Second b-tree node to be merged, will contain the merged children
		\return The merged b-tree node that contains all children from both input nodes
		\throws assertion failure if node1 and node2 do not have the same parent, are not adjacent siblings, or have objects
	*/
	idBTreeNode<objType, keyType>*					 MergeNodes( idBTreeNode<objType, keyType>* node1, idBTreeNode<objType, keyType>* node2 );

	/*!
		\brief Validates the structure and properties of a B-tree node and its descendants

		This function recursively checks the integrity of a B-tree structure starting from a given node. It ensures that each node adheres to the B-tree properties, including the correct number of
	   children, proper linking between nodes, and key ordering. The function increments a node counter for each node processed and performs multiple assertions to validate the tree's structure. The
	   root node or leaf nodes may have zero children, while internal nodes must have between 2 and maxChildrenPerNode children. Keys within nodes must be properly ordered relative to their children.

		\param node Pointer to the B-tree node to validate
		\param numNodes Reference to an integer that gets incremented for each node processed
		\throws Assertion failures if the B-tree structure violates any of the expected properties
	*/
	void											 CheckTree_r( idBTreeNode<objType, keyType>* node, int& numNodes ) const;

	/*!
		\brief Validates the integrity of the B-tree structure by checking node counts and leaf ordering

		This function performs a comprehensive check of the B-tree data structure to ensure its integrity. It first counts all nodes in the tree and compares this count with the number of allocated
	   nodes to verify no memory leaks or double-free errors occurred. Then it traverses all leaf nodes in order and verifies that their keys are properly sorted in ascending order. The function is
	   designed to be called in debug builds to catch structural inconsistencies in the tree.

		\throws assertion failure if the tree structure is invalid or if node counts don't match expected values
	*/
	void											 CheckTree() const;
};

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTree<objType, keyType, maxChildrenPerNode>::idBTree()
{
	assert( maxChildrenPerNode >= 4 );
	root = NULL;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTree<objType, keyType, maxChildrenPerNode>::~idBTree()
{
	Shutdown();
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::Init()
{
	root = AllocNode();
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::Shutdown()
{
	nodeAllocator.Shutdown();
	root = NULL;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::Add( objType* object, keyType key )
{
	idBTreeNode<objType, keyType>*node, *child, *newNode;

	if( root == NULL ) { root = AllocNode(); }

	if( root->numChildren >= maxChildrenPerNode ) {
		newNode				 = AllocNode();
		newNode->key		 = root->key;
		newNode->firstChild	 = root;
		newNode->lastChild	 = root;
		newNode->numChildren = 1;
		root->parent		 = newNode;
		SplitNode( root );
		root = newNode;
	}

	newNode			= AllocNode();
	newNode->key	= key;
	newNode->object = object;

	for( node = root; node->firstChild != NULL; node = child ) {
		if( key > node->key ) { node->key = key; }

		// find the first child with a key larger equal to the key of the new node
		for( child = node->firstChild; child->next; child = child->next ) {
			if( key <= child->key ) { break; }
		}

		if( child->object ) {
			if( key <= child->key ) {
				// insert new node before child
				if( child->prev ) {
					child->prev->next = newNode;
				} else {
					node->firstChild = newNode;
				}
				newNode->prev = child->prev;
				newNode->next = child;
				child->prev	  = newNode;
			} else {
				// insert new node after child
				if( child->next ) {
					child->next->prev = newNode;
				} else {
					node->lastChild = newNode;
				}
				newNode->prev = child;
				newNode->next = child->next;
				child->next	  = newNode;
			}

			newNode->parent = node;
			node->numChildren++;

#ifdef BTREE_CHECK
			CheckTree();
#endif

			return newNode;
		}

		// make sure the child has room to store another node
		if( child->numChildren >= maxChildrenPerNode ) {
			SplitNode( child );
			if( key <= child->prev->key ) { child = child->prev; }
		}
	}

	// we only end up here if the root node is empty
	newNode->parent	 = root;
	root->key		 = key;
	root->firstChild = newNode;
	root->lastChild	 = newNode;
	root->numChildren++;

#ifdef BTREE_CHECK
	CheckTree();
#endif

	return newNode;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::Remove( idBTreeNode<objType, keyType>* node )
{
	idBTreeNode<objType, keyType>* parent;

	assert( node->object != NULL );

	// unlink the node from it's parent
	if( node->prev ) {
		node->prev->next = node->next;
	} else {
		node->parent->firstChild = node->next;
	}
	if( node->next ) {
		node->next->prev = node->prev;
	} else {
		node->parent->lastChild = node->prev;
	}
	node->parent->numChildren--;

	// make sure there are no parent nodes with a single child
	for( parent = node->parent; parent != root && parent->numChildren <= 1; parent = parent->parent ) {
		if( parent->next ) {
			parent = MergeNodes( parent, parent->next );
		} else if( parent->prev ) {
			parent = MergeNodes( parent->prev, parent );
		}

		// a parent may not use a key higher than the key of it's last child
		if( parent->key > parent->lastChild->key ) { parent->key = parent->lastChild->key; }

		if( parent->numChildren > maxChildrenPerNode ) {
			SplitNode( parent );
			break;
		}
	}
	for( ; parent != NULL && parent->lastChild != NULL; parent = parent->parent ) {
		// a parent may not use a key higher than the key of it's last child
		if( parent->key > parent->lastChild->key ) { parent->key = parent->lastChild->key; }
	}

	// free the node
	FreeNode( node );

	// remove the root node if it has a single internal node as child
	if( root->numChildren == 1 && root->firstChild->object == NULL ) {
		idBTreeNode<objType, keyType>* oldRoot = root;
		root->firstChild->parent			   = NULL;
		root								   = root->firstChild;
		FreeNode( oldRoot );
	}

#ifdef BTREE_CHECK
	CheckTree();
#endif
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::NodeFind( keyType key ) const
{
	idBTreeNode<objType, keyType>* node;

	for( node = root->firstChild; node != NULL; node = node->firstChild ) {
		while( node->next ) {
			if( node->key >= key ) { break; }
			node = node->next;
		}
		if( node->object ) {
			if( node->key == key ) {
				return node;
			} else {
				return NULL;
			}
		}
	}
	return NULL;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::NodeFindSmallestLargerEqual( keyType key ) const
{
	idBTreeNode<objType, keyType>* node;

	if( root == NULL ) { return NULL; }

	for( node = root->firstChild; node != NULL; node = node->firstChild ) {
		while( node->next ) {
			if( node->key >= key ) { break; }
			node = node->next;
		}
		if( node->object ) {
			if( node->key >= key ) {
				return node;
			} else {
				return NULL;
			}
		}
	}
	return NULL;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::NodeFindLargestSmallerEqual( keyType key ) const
{
	idBTreeNode<objType, keyType>* node;

	if( root == NULL ) { return NULL; }

	idBTreeNode<objType, keyType>* smaller = NULL;
	for( node = root->firstChild; node != NULL; node = node->firstChild ) {
		while( node->next ) {
			if( node->key >= key ) { break; }
			smaller = node;
			node	= node->next;
		}
		if( node->object ) {
			if( node->key <= key ) {
				return node;
			} else if( smaller == NULL ) {
				return NULL;
			} else {
				node = smaller;
				if( node->object ) { return node; }
			}
		}
	}
	return NULL;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE objType* idBTree<objType, keyType, maxChildrenPerNode>::Find( keyType key ) const
{
	idBTreeNode<objType, keyType>* node = NodeFind( key );
	if( node == NULL ) {
		return NULL;
	} else {
		return node->object;
	}
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE objType* idBTree<objType, keyType, maxChildrenPerNode>::FindSmallestLargerEqual( keyType key ) const
{
	idBTreeNode<objType, keyType>* node = NodeFindSmallestLargerEqual( key );
	if( node == NULL ) {
		return NULL;
	} else {
		return node->object;
	}
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE objType* idBTree<objType, keyType, maxChildrenPerNode>::FindLargestSmallerEqual( keyType key ) const
{
	idBTreeNode<objType, keyType>* node = NodeFindLargestSmallerEqual( key );
	if( node == NULL ) {
		return NULL;
	} else {
		return node->object;
	}
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::GetRoot() const
{
	return root;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE int idBTree<objType, keyType, maxChildrenPerNode>::GetNodeCount() const
{
	return nodeAllocator.GetAllocCount();
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::GetNext( idBTreeNode<objType, keyType>* node ) const
{
	if( node->firstChild ) {
		return node->firstChild;
	} else {
		while( node && node->next == NULL ) {
			node = node->parent;
		}
		return node;
	}
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::GetNextLeaf( idBTreeNode<objType, keyType>* node ) const
{
	if( node->firstChild ) {
		while( node->firstChild ) {
			node = node->firstChild;
		}
		return node;
	} else {
		while( node && node->next == NULL ) {
			node = node->parent;
		}
		if( node ) {
			node = node->next;
			while( node->firstChild ) {
				node = node->firstChild;
			}
			return node;
		} else {
			return NULL;
		}
	}
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::AllocNode()
{
	idBTreeNode<objType, keyType>* node = nodeAllocator.Alloc();
	node->key							= 0;
	node->parent						= NULL;
	node->next							= NULL;
	node->prev							= NULL;
	node->numChildren					= 0;
	node->firstChild					= NULL;
	node->lastChild						= NULL;
	node->object						= NULL;
	return node;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::FreeNode( idBTreeNode<objType, keyType>* node )
{
	nodeAllocator.Free( node );
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::SplitNode( idBTreeNode<objType, keyType>* node )
{
	int							  i;
	idBTreeNode<objType, keyType>*child, *newNode;

	// allocate a new node
	newNode			= AllocNode();
	newNode->parent = node->parent;

	// divide the children over the two nodes
	child		  = node->firstChild;
	child->parent = newNode;
	for( i = 3; i < node->numChildren; i += 2 ) {
		child		  = child->next;
		child->parent = newNode;
	}

	newNode->key		 = child->key;
	newNode->numChildren = node->numChildren / 2;
	newNode->firstChild	 = node->firstChild;
	newNode->lastChild	 = child;

	node->numChildren -= newNode->numChildren;
	node->firstChild = child->next;

	child->next->prev = NULL;
	child->next		  = NULL;

	// add the new child to the parent before the split node
	assert( node->parent->numChildren < maxChildrenPerNode );

	if( node->prev ) {
		node->prev->next = newNode;
	} else {
		node->parent->firstChild = newNode;
	}
	newNode->prev = node->prev;
	newNode->next = node;
	node->prev	  = newNode;

	node->parent->numChildren++;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE idBTreeNode<objType, keyType>* idBTree<objType, keyType, maxChildrenPerNode>::MergeNodes( idBTreeNode<objType, keyType>* node1, idBTreeNode<objType, keyType>* node2 )
{
	idBTreeNode<objType, keyType>* child;

	assert( node1->parent == node2->parent );
	assert( node1->next == node2 && node2->prev == node1 );
	assert( node1->object == NULL && node2->object == NULL );
	assert( node1->numChildren >= 1 && node2->numChildren >= 1 );

	for( child = node1->firstChild; child->next; child = child->next ) {
		child->parent = node2;
	}
	child->parent			= node2;
	child->next				= node2->firstChild;
	node2->firstChild->prev = child;
	node2->firstChild		= node1->firstChild;
	node2->numChildren += node1->numChildren;

	// unlink the first node from the parent
	if( node1->prev ) {
		node1->prev->next = node2;
	} else {
		node1->parent->firstChild = node2;
	}
	node2->prev = node1->prev;
	node2->parent->numChildren--;

	FreeNode( node1 );

	return node2;
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::CheckTree_r( idBTreeNode<objType, keyType>* node, int& numNodes ) const
{
	int							   numChildren;
	idBTreeNode<objType, keyType>* child;

	numNodes++;

	// the root node may have zero children and leaf nodes always have zero children, all other nodes should have at least 2 and at most maxChildrenPerNode children
	assert( ( node == root ) || ( node->object != NULL && node->numChildren == 0 ) || ( node->numChildren >= 2 && node->numChildren <= maxChildrenPerNode ) );
	// the key of a node may never be larger than the key of it's last child
	assert( ( node->lastChild == NULL ) || ( node->key <= node->lastChild->key ) );

	numChildren = 0;
	for( child = node->firstChild; child; child = child->next ) {
		numChildren++;
		// make sure the children are properly linked
		if( child->prev == NULL ) {
			assert( node->firstChild == child );
		} else {
			assert( child->prev->next == child );
		}
		if( child->next == NULL ) {
			assert( node->lastChild == child );
		} else {
			assert( child->next->prev == child );
		}
		// recurse down the tree
		CheckTree_r( child, numNodes );
	}
	// the number of children should equal the number of linked children
	assert( numChildren == node->numChildren );
}

template<class objType, class keyType, int maxChildrenPerNode>
ID_INLINE void idBTree<objType, keyType, maxChildrenPerNode>::CheckTree() const
{
	int							  numNodes = 0;
	idBTreeNode<objType, keyType>*node, *lastNode;

	CheckTree_r( root, numNodes );

	// the number of nodes in the tree should equal the number of allocated nodes
	assert( numNodes == nodeAllocator.GetAllocCount() );

	// all the leaf nodes should be ordered
	lastNode = GetNextLeaf( GetRoot() );
	if( lastNode ) {
		for( node = GetNextLeaf( lastNode ); node; lastNode = node, node = GetNextLeaf( node ) ) {
			assert( lastNode->key <= node->key );
		}
	}
}

#endif /* !__BTREE_H__ */
