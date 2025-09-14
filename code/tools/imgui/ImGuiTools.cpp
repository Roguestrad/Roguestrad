/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2016 Daniel Gibson
Copyright (C) 2022 Stephen Pridham

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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "../imgui/BFGimgui.h"
#include "../idlib/CmdArgs.h"

#include "afeditor/AfEditor.h"
#include "lighteditor/LightEditor.h"


extern idCVar g_editEntityMode;

static bool releaseMouse = false;


namespace ImGuiTools
{

void SetReleaseToolMouse( bool doRelease )
{
	releaseMouse = doRelease;
}

bool AreEditorsActive()
{
	// FIXME: this is not exactly clean and must be changed if we ever support game dlls
	return g_editEntityMode.GetInteger() > 0 || com_editors != 0;
}

bool ReleaseMouseForTools()
{
	// RB: ignore everything as long right mouse button is pressed
	return AreEditorsActive() && releaseMouse && !ImGuiHook::RightMouseActive();
}

void DrawToolWindows()
{
	if( !AreEditorsActive() )
	{
		return;
	}

	if( LightEditor::Instance().IsShown() )
	{
		LightEditor::Instance().Draw();
	}
	else if( AfEditor::Instance().IsShown() )
	{
		AfEditor::Instance().Draw();
	}
}

void LightEditorInit( const idDict* dict, idEntity* ent )
{
	if( dict == NULL || ent == NULL )
	{
		return;
	}

	// NOTE: we can't access idEntity (it's just a declaration), because it should
	// be game/mod specific. but we can at least check the spawnclass from the dict.
	idassert( idStr::Icmp( dict->GetString( "spawnclass" ), "idLight" ) == 0
			  && "LightEditorInit() must only be called with light entities or NULL!" );


	LightEditor::Instance().ShowIt( true );
	SetReleaseToolMouse( true );

	LightEditor::ReInit( dict, ent );
}

void AfEditorInit()
{
	AfEditor::Instance().ShowIt( true );
	SetReleaseToolMouse( true );
}

} //namespace ImGuiTools
