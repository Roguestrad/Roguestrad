/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

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

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#include "../sys/sys_local.h"
#pragma hdrstop

#include "TypeInfoGen.h"

#define SOURCE_CODE_BASE_FOLDER "neo"

idSession* 			session = NULL;

/*
==============================================================

	main

==============================================================
*/

int main( int argc, char** argv )
{
	idStr fileName, sourcePath;
	idTypeInfoGen* generator;

	idLib::common = common;
	idLib::cvarSystem = cvarSystem;
	idLib::fileSystem = fileSystem;
	//idLib::sys = sys;

	idLib::Init();
	cmdSystem->Init();
	cvarSystem->Init();
	idCVar::RegisterStaticVars();

	cvarSystem->SetCVarString( "fs_game", "neo" );
	fileSystem->Init();

	generator = new idTypeInfoGen;

	sourcePath = "d3xp";
	fileName = "d3xp/gamesys/GameTypeInfo";

	generator->AddDefine( "__cplusplus" );
	generator->AddDefine( "GAME_DLL" );
	generator->AddDefine( "ID_TYPEINFO" );
	generator->AddDefine( "TYPEINFO" );
	generator->AddDefine( "__TYPEINFOGEN__" );
	generator->AddDefine( "_WIN32" );
	generator->AddDefine( "ID_VULKAN" );
	//generator->AddDefine( "VR_OPTIONS" );

	generator->CreateTypeInfo( sourcePath );
	generator->WriteTypeInfo( fileName );

	delete generator;

	fileName.Clear();
	sourcePath.Clear();

	fileSystem->Shutdown( false );
	cvarSystem->Shutdown();
	cmdSystem->Shutdown();
	idLib::ShutDown();

	return 0;
}
