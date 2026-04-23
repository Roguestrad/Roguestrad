/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022-2025 Robert Beckebans

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

#ifndef __MODELMANAGER_H__
#define __MODELMANAGER_H__

/*!
	\class idNamePair
	\brief A simple class to store a name-value pair.
*/
class idNamePair
{
public:
	idStr from;
	idStr to;
};

/*!
	\class idAnimGroup
	\brief A set of joint names forming a group.
*/
class idAnimGroup
{
public:
	idStr	  name;
	idStrList joints;
};

/*!
	\class idImportOptions
	\brief A class for managing import options with initialization and reset capabilities.
*/
class idImportOptions
{
public:
	idStr				 commandLine;
	idStr				 src;
	idStr				 dest;
	idStr				 game;
	idStr				 prefix;
	float				 scale;
	bool				 ignoreMeshes;
	bool				 clearOrigin;
	bool				 clearOriginAxis;
	bool				 addOrigin;
	idStr				 transferRootMotion;
	bool				 ignoreScale;
	int					 startframe;
	int					 endframe;
	int					 framerate;
	float				 xyzPrecision;
	float				 quatPrecision;
	idStr				 align;
	idList<idNamePair>	 renamejoints;
	idList<idNamePair>	 remapjoints;
	idStrList			 keepjoints;
	idStrList			 skipmeshes;
	idStrList			 keepmeshes;
	idList<idAnimGroup*> exportgroups;
	idList<idAnimGroup>	 groups;
	float				 rotate;
	float				 jointThreshold;
	int					 cycleStart;
	idAngles			 reOrient;

	// RB begin
	idStr				 armature;
	bool				 noMikktspace;
	idStr				 modelDefName;
	idStr				 modelDefFileName;
	ID_TIME_T			 declSourceTimeStamp; // timestamp of the .def where this modelDef option comes from

	//! Initializes a new instance of the idImportOptions class.
	idImportOptions();

	//! Resets all import options to their default values.
	void Reset();

	//! Initializes import options using the provided command line and OS path.
	void Init( const char* commandline, const char* ospath );
};

/*!
	\class idRenderModelManager
	\brief Manager for handling render models within the engine.

	This class serves as an abstract interface for managing render models, providing methods for initialization, shutdown, model loading, and memory management. It supports operations like finding,
   checking, and reloading models, as well as handling level transitions and precaching. The interface is designed to be implemented by concrete renderer-specific managers.

*/
class idRenderModelManager
{
public:
	virtual ~idRenderModelManager() { }

	// registers console commands and clears the list
	virtual void		   Init() = 0;

	// frees all the models
	virtual void		   Shutdown() = 0;

	// called only by renderer::BeginLevelLoad
	virtual void		   BeginLevelLoad() = 0;

	// called only by renderer::EndLevelLoad
	virtual void		   EndLevelLoad() = 0;

	// called only by renderer::Preload
	virtual void		   Preload( const idPreloadManifest& manifest ) = 0;

	// allocates a new empty render model.
	virtual idRenderModel* AllocModel() = 0;

	// frees a render model
	virtual void		   FreeModel( idRenderModel* model ) = 0;

	// returns NULL if modelName is NULL or an empty string, otherwise
	// it will create a default model if not loadable
	virtual idRenderModel* FindModel( const char* modelName, const idImportOptions* options = NULL ) = 0;

	// returns NULL if not loadable
	virtual idRenderModel* CheckModel( const char* modelName ) = 0;

	// returns the default cube model
	virtual idRenderModel* DefaultModel() = 0;

	// world map parsing will add all the inline models with this call
	virtual void		   AddModel( idRenderModel* model ) = 0;

	// when a world map unloads, it removes its internal models from the list
	// before freeing them.
	// There may be an issue with multiple renderWorlds that share data...
	virtual void		   RemoveModel( idRenderModel* model ) = 0;

	// the reloadModels console command calls this, but it can
	// also be explicitly invoked
	virtual void		   ReloadModels( bool forceAll = false ) = 0;

	// loads the mesh vertices/indices into the vertex cache.
	virtual void		   CreateMeshBuffers( nvrhi::ICommandList* commandList ) = 0;

	// write "touchModel <model>" commands for each non-world-map model
	virtual void		   WritePrecacheCommands( idFile* f ) = 0;

	// called during vid_restart
	virtual void		   FreeModelVertexCaches() = 0;

	// print memory info
	virtual void		   PrintMemInfo( MemInfo_t* mi ) = 0;
};

// this will be statically pointed at a private implementation
extern idRenderModelManager* renderModelManager;

#endif /* !__MODELMANAGER_H__ */
