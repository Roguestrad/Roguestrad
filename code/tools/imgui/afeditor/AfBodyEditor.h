/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Stephen Pridham

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU
General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma once

#include "../util/Imgui_IdWidgets.h"

namespace ImGuiTools
{

/*!
	\class ImGuiTools::AfBodyEditor
	\brief Provides a user interface for editing rigid body properties within an affine field system.

	The AfBodyEditor class offers a graphical interface for modifying the properties of rigid bodies in an affine field context. It supports editing position, orientation, and collision model settings
   through interactive ImGui controls. The editor initializes joint lists from associated render models and tracks changes to body properties. The class is designed to be used within a larger editor
   framework where affine field declarations and their constituent bodies can be modified visually. The UI components provide feedback on modification status through return values from various
   property editing functions.

*/
class AfBodyEditor
{
public:
	//! Constructs an AfBodyEditor instance initialized with the provided declaration and body.
	AfBodyEditor( idDeclAF* newDecl, idDeclAF_Body* newBody );
	~AfBodyEditor();

	//! Displays and handles the UI for editing a rigid body's properties in the AfEditor.
	bool Do();

private:
	//! Initializes the joint lists for the body editor by populating them with joint names from the associated render model.
	void			  InitJointLists();

	//! Returns true if the position properties of the body have been modified.
	bool			  Position();

	//! Displays and handles the UI controls for editing a body's position property.
	bool			  PositionProperty();

	//! Returns true if any of the pitch, yaw, or roll values have been modified.
	bool			  PitchYawRoll();

	//! Returns true if any collision model properties were modified
	bool			  CollisionModel();

	//! Displays and handles user input for collision model size parameters based on the model type
	bool			  CollisionModelSize();

	//! Displays an ImGui drag float3 input field for editing an idAFVector value.
	bool			  InputAfVector( const char* label, idAFVector* vec );

	idDeclAF*		  decl;
	idDeclAF_Body*	  body;
	int				  positionType;
	int				  modifyJointType;
	int				  comboJoint1;
	int				  comboJoint2;
	int				  originBoneCenterJoint1;
	int				  originBoneCenterJoint2;
	int				  originJoint;
	MultiSelectWidget contentWidget;

	idStr			  modifiedJoint;
	idStr			  renameBody;
	idStrList		  joints;
};

}
