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

#ifndef __STACK_H__
#define __STACK_H__

/*
===============================================================================

	Stack template

===============================================================================
*/

#define idStack( type, next ) idStackTemplate<type, ( int )&( ( ( type* )NULL )->next )>

/*!
	\class idStackTemplate
	\brief A template-based stack implementation for managing elements in a last-in-first-out structure.

	This class provides a generic stack template that supports dynamic element insertion and removal. The template parameters define the element type and the offset to the next element within the
   structure. The implementation manages a linked list of elements where each element points to the next one in the stack. The Add method inserts elements at the top of the stack, while the Get method
   retrieves and removes the top element. The stack maintains both top and bottom pointers to efficiently handle operations on both ends of the structure.

*/
template<class type, int nextOffset>
class idStackTemplate
{
public:
	/*!
		\brief Initializes an empty stack template with both top and bottom pointers set to NULL.

		This constructor initializes the stack by setting both the top and bottom pointers to NULL, indicating that the stack is empty. The template parameters type and nextOffset are used to define
	   the type of elements stored in the stack and the offset for the next element in the stack structure respectively.

	*/
	idStackTemplate();

	/*!
		\brief Adds an element to the top of the stack template

		This function inserts a new element at the top of the stack template structure. It updates the next pointer of the new element to point to the current top of the stack, then updates the top
	   pointer to reference the new element. If the stack was previously empty, it also sets the bottom pointer to the new element.

		\param element pointer to the element to be added to the stack
	*/
	void  Add( type* element );

	/*!
		\brief Retrieves and removes the top element from the stack template

		This function pops the top element from a stack template data structure and returns a pointer to it. If the stack is empty, it returns NULL. The function updates the stack pointers
	   appropriately and ensures proper memory management by setting the next pointer of the popped element to NULL

		\return A pointer to the top element of the stack, or NULL if the stack is empty
	*/
	type* Get();

private:
	type* top;
	type* bottom;
};

#define STACK_NEXT_PTR( element ) ( *( type** )( ( ( byte* )element ) + nextOffset ) )

template<class type, int nextOffset>
idStackTemplate<type, nextOffset>::idStackTemplate()
{
	top = bottom = NULL;
}

template<class type, int nextOffset>
void idStackTemplate<type, nextOffset>::Add( type* element )
{
	STACK_NEXT_PTR( element ) = top;
	top						  = element;
	if( !bottom ) { bottom = element; }
}

template<class type, int nextOffset>
type* idStackTemplate<type, nextOffset>::Get()
{
	type* element;

	element = top;
	if( element ) {
		top = STACK_NEXT_PTR( top );
		if( bottom == element ) { bottom = NULL; }
		STACK_NEXT_PTR( element ) = NULL;
	}
	return element;
}

#endif /* !__STACK_H__ */
