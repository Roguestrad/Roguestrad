/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2015 Daniel Gibson

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

#ifndef NEO_IMGUI_BFGIMGUI_H_
#define NEO_IMGUI_BFGIMGUI_H_

#include "imgui.h"

#include "../idlib/math/Vector.h"

// add custom functions for imgui
namespace ImGui
{

/*!
	\brief Creates a draggable vector input widget for a 3D vector with individual X, Y, Z components

	This function generates a group of three draggable float controls for editing the components of a 3D vector. Each component has its own label prefix (X:, Y:, Z:) and can be adjusted using the
   provided speed and range parameters. The function handles layout and alignment of the controls within the ImGui context.

	\param label Unique identifier for the widget group
	\param v Reference to the 3D vector to edit
	\param v_speed Speed factor for dragging adjustments
	\param v_min Minimum allowed value for components
	\param v_max Maximum allowed value for components
	\param display_format Format string for displaying component values
	\param power Exponent for non-linear scaling of values
	\param ignoreLabelWidth Whether to ignore label width when calculating component widths
	\return True if any of the vector components were modified during the current frame
*/
bool DragVec3( const char* label, idVec3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.1f", float power = 1.0f, bool ignoreLabelWidth = true );

/*!
	\brief Provides a drag control for a 3D vector that adjusts its width to fit the label and prevents text cutoff.

	This function is a variant of DragVec3 that ensures the widget width accommodates the label length to avoid truncation. It serves as a shortcut for DragXYZ with ignoreLabelWidth set to false. The
   function behaves similarly to DragVec3 but with automatic width adjustment based on the label width, which may cause misalignment with consecutive value+label widgets such as other Drag* or
   ColorEdit* controls.

	\param label Identifier for the widget displayed next to the input field
	\param v Reference to the 3D vector value being edited
	\param v_speed Speed factor for the drag operation
	\param v_min Minimum allowed value for the vector components
	\param v_max Maximum allowed value for the vector components
	\param display_format Format string for displaying the vector components
	\param power Exponent for the slider's power curve
	\return True if the vector value was modified during the operation
*/
bool DragVec3fitLabel( const char* label, idVec3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.1f", float power = 1.0f );

}

#endif /* NEO_IMGUI_BFGIMGUI_H_ */
