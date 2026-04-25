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

#ifndef __GAME_EDIT_H__
#define __GAME_EDIT_H__

/*!
	\class idCursor3D
	\brief 3D cursor entity for drag evaluation and visualization.
*/
class idCursor3D : public idEntity
{
public:
	CLASS_PROTOTYPE( idCursor3D );

	//! Initializes a new instance of the idCursor3D class with the dragged position set to zero.
	idCursor3D();
	~idCursor3D();

	//! Initializes the 3D cursor object.
	void		 Spawn();

	//! Renders a 3D cursor with debug arrows representing its orientation and drag position.
	void		 Present();

	//! Updates the cursor position based on drag evaluation and presents it.
	void		 Think();

	idForce_Drag drag;
	idVec3		 draggedPosition;
};

/*!
	\class idDragEntity
	\brief Manages entity dragging operations including selection, binding, and deletion.

	The idDragEntity class provides functionality for selecting, moving, and manipulating entities within the engine. It handles the entire drag operation lifecycle from selection to binding and
   deletion. The class maintains state about the currently selected entity and manages the cursor used during dragging operations. It supports both binding entities to the world using constraints and
   removing those constraints. The Update method processes input to move selected entities, while Clear and StopDrag methods handle cleanup operations.

*/
class idDragEntity
{
public:
	//! Constructs a new idDragEntity object and initializes its internal state.
	idDragEntity();

	//! Destructor for the idDragEntity class that stops dragging, clears selection, and deletes the cursor.
	~idDragEntity();

	//! Clears all member variables of the idDragEntity object
	void	  Clear();

	//! Updates the dragging state for an entity based on player input and view position.
	void	  Update( idPlayer* player );

	//! Sets the entity that is currently selected for dragging.
	void	  SetSelected( idEntity* ent );

	//! Returns the entity that is currently selected by this drag entity.
	idEntity* GetSelected() const { return selected.GetEntity(); }

	//! Deletes the currently selected entity and clears the selection.
	void	  DeleteSelected();

	//! Binds the selected entity to the world using a ballAndSocket constraint
	void	  BindSelected();

	//! Removes binding constraints from the currently selected entity.
	void	  UnbindSelected();

private:
	idEntityPtr<idEntity> dragEnt;			// entity being dragged
	jointHandle_t		  joint;			// joint being dragged
	int					  id;				// id of body being dragged
	idVec3				  localEntityPoint; // dragged point in entity space
	idVec3				  localPlayerPoint; // dragged point in player space
	idStr				  bodyName;			// name of the body being dragged
	idCursor3D*			  cursor;			// cursor entity
	idEntityPtr<idEntity> selected;			// last dragged entity

	//! Stops the entity dragging operation and deactivates the cursor.
	void				  StopDrag();
};

/*
===============================================================================

	Handles ingame entity editing.

===============================================================================
*/
typedef struct selectedTypeInfo_s {
	idTypeInfo* typeInfo;
	idStr		textKey;
} selectedTypeInfo_t;

/*!
	\class idEditEntities
	\brief Manages entity selection and display in an editing environment.

	The idEditEntities class provides functionality for selecting, managing, and displaying entities within an editor environment. It handles the logic for determining which entities are selectable,
   maintaining a list of currently selected entities, and rendering entities based on the current edit mode. The class supports operations to add or remove entities from the selection, clear all
   selections, and check the selectability of individual entities with optional visual feedback. The selection logic considers spatial positioning and direction to determine entity selection, and
   provides methods to query selection state and visual properties of entities.

*/
class idEditEntities
{
public:
	//! Initializes a new instance of the idEditEntities class.
	idEditEntities();

	//! Selects an entity at the given origin and direction, skipping a specified entity.
	bool SelectEntity( const idVec3& origin, const idVec3& dir, const idEntity* skip );

	//! Marks the specified entity as selected and adds it to the list of selected entities.
	void AddSelectedEntity( idEntity* ent );

	//! Removes the specified entity from the list of selected entities.
	void RemoveSelectedEntity( idEntity* ent );

	//! Clears the selection state of all entities in the editor.
	void ClearSelectedEntities();

	//! Displays selectable entities in the game world based on the current edit mode.
	void DisplayEntities();

	//! Checks if an entity is selectable and optionally retrieves its selection color and text label.
	bool EntityIsSelectable( idEntity* ent, idVec4* color = NULL, idStr* text = NULL );

private:
	int						   nextSelectTime;
	idList<selectedTypeInfo_t> selectableEntityClasses;
	idList<idEntity*>		   selectedEntities;
};

#endif /* !__GAME_EDIT_H__ */
