/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Daniel Gibson
Copyright (C) 2016-2023 Robert Beckebans

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

// a GUI light editor, based loosely on the one from original Doom3 (neo/tools/radiant/LightDlg.*)
// LightInfo was CLightInfo, the LightEditor itself was written from scratch.

#ifndef NEO_TOOLS_EDITORS_LIGHTEDITOR_H_
#define NEO_TOOLS_EDITORS_LIGHTEDITOR_H_

#include <idlib/Dict.h>
#include "../../edit_public.h"

#include "engine/imgui/BFGimgui.h"
#include "../imgui/ImGuizmo.h"

namespace ImGuiTools
{

enum ELightType { LIGHT_POINT, LIGHT_SPOT, LIGHT_SUN };

/*!
	\class ImGuiTools::LightInfo
	\brief Provides light configuration and management for ImGui-based tools.

	The LightInfo class encapsulates lighting parameters and provides functionality to initialize, modify, and serialize lighting settings. It supports various light types including point, projected,
   and sun lights, with methods to set default values and populate data from or to dictionaries. This class is designed to facilitate lighting configuration within an ImGui-based interface, allowing
   for easy manipulation and persistence of lighting data. The class maintains light properties such as color and origin while supporting different light type initializations.

*/
class LightInfo
{
public:
	ELightType lightType;

	idStr	   strTexture;
	bool	   equalRadius;
	bool	   explicitStartEnd;
	idVec3	   lightStart;
	idVec3	   lightEnd;
	idVec3	   lightUp;
	idVec3	   lightRight;
	idVec3	   lightTarget;
	idVec3	   lightCenter;
	idVec3	   color;

	bool	   hasLightOrigin;
	idVec3	   origin;
	idAngles   angles; // RBDOOM specific, saved to map as "angles"
	idVec3	   scale;  // not saved to .map

	idVec3	   lightRadius;
	bool	   castShadows;
	bool	   skipSpecular;
	bool	   hasCenter;
	int		   lightStyle; // RBDOOM specific, saved to map as "style"

	//! Initializes a new instance of the LightInfo class with default values.
	LightInfo();

	//! Initializes all lighting information to default values.
	void Defaults();

	//! Initializes a point light with default properties while preserving color and light origin settings.
	void DefaultPoint();

	//! Initializes default projected light settings while preserving color and light origin information.
	void DefaultProjected();

	//! Initializes the light information with default sun settings while preserving color and light origin.
	void DefaultSun();

	//! Populates light information from a dictionary.
	void FromDict( const idDict* e );

	//! Converts light information to a dictionary for entity storage.
	void ToDict( idDict* e );
};

/*!
	\class ImGuiTools::LightEditor
	\brief Provides a user interface for editing light entity properties and configurations.

	The LightEditor class serves as a dedicated editor for manipulating light entities within a graphical interface. It manages the display and interaction of light properties such as styles,
   textures, and spawn arguments. The class supports initializing with specific light entities, loading style definitions, handling texture loading, applying temporary changes, and saving or reverting
   modifications. It includes functionality for duplicating lights and managing the visibility of the editor UI. The class is implemented as a singleton to ensure a single instance controls the
   editing interface. The editor integrates with ImGui for rendering its user interface and provides methods for retrieving item names for combo boxes, enabling users to select from available light
   styles and textures.

*/
class LightEditor
{
private:
	bool				isShown;

	idStr				title;
	idStr				entityName;
	idVec3				entityPos;

	LightInfo			original;
	LightInfo			cur; // current status of the light
	LightInfo			curNotMoving;

	idEntity*			lightEntity;

	idList<idStr>		textureNames;
	int					currentTextureIndex;
	idImage*			currentTexture;
	const idMaterial*	currentTextureMaterial;

	// RB: light style support
	idList<idStr>		styleNames;
	int					currentStyleIndex;

	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE		mCurrentGizmoMode	   = ImGuizmo::WORLD;

	bool				useSnap			= false;
	float				gridSnap[3]		= { 4.0f, 4.0f, 4.0f };
	float				angleSnap		= 15.0f;
	float				scaleSnap		= 0.1f;
	float				bounds[6]		= { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	float				boundsSnap[3]	= { 0.1f, 0.1f, 0.1f };
	bool				boundSizing		= false;
	bool				boundSizingSnap = false;

	bool				shortcutSaveMapEnabled;
	bool				shortcutDuplicateLightEnabled;

	//! Loads light style definitions from the light entity definition or uses predefined Quake 1 styles as a fallback.
	void				LoadLightStyles();

	//! Retrieves the name of a light style item for use in an ImGui combo box.
	static bool			StyleItemsGetter( void* data, int idx, const char** out_text );

	//! Initializes the light editor with the specified dictionary and light entity.
	void				Init( const idDict* dict, idEntity* light );

	//! Resets the light editor state to its default values
	void				Reset();

	//! Loads and processes light texture names from material declarations for the light editor
	void				LoadLightTextures();

	//! Populates the texture items getter with texture names for the light editor
	static bool			TextureItemsGetter( void* data, int idx, const char** out_text );

	//! Loads the current texture for the light editor based on the active texture index and material.
	void				LoadCurrentTexture();

	//! Applies temporary changes to a light entity in the editor
	void				TempApplyChanges();

	//! Saves the current light editor changes to the map.
	void				SaveChanges( bool saveMap );

	//! Reverts the light entity's spawn arguments to their original values.
	void				CancelChanges();

	//! Duplicates the currently selected light entity in the editor.
	void				DuplicateLight();

	//! Initializes a new instance of the LightEditor class and resets its state.
	LightEditor()
	{
		isShown = false;

		Reset();
	}

public:
	//! Returns the singleton instance of the LightEditor class.
	static LightEditor& Instance();

	//! Reinitializes the light editor with the provided dictionary and light entity.
	static void			ReInit( const idDict* dict, idEntity* light );

	//! Sets the visibility state of the light editor UI element.
	inline void			ShowIt( bool show ) { isShown = show; }

	//! Returns whether the light editor window is currently shown.
	inline bool			IsShown() const { return isShown; }

	//! Displays the light editor interface in the ImGui window
	void				Draw();
};

} // namespace ImGuiTools

#endif
