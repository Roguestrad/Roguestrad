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

#ifndef __INTERACTION_H__
#define __INTERACTION_H__

/*
===============================================================================

	Interaction between static entityDef surfaces and a static lightDef.

	Interactions with no lightTris and no shadowTris are still
	valid, because they show that a given entityDef / lightDef
	do not interact, even though they share one or more areas.

===============================================================================
*/

#define LIGHT_CULL_ALL_FRONT ( ( byte* )-1 )
#define LIGHT_CLIP_EPSILON	 0.1f

// enabling this define allows the precise inside shadow volume test
// to be performed on interaction (static) shadow volumes
#define KEEP_INTERACTION_CPU_DATA

struct srfCullInfo_t {
	// For each triangle a byte set to 1 if facing the light origin.
	byte*	facing;

	// For each vertex a byte with the bits [0-5] set if the
	// vertex is at the back side of the corresponding clip plane.
	// If the 'cullBits' pointer equals LIGHT_CULL_ALL_FRONT all
	// vertices are at the front of all the clip planes.
	byte*	cullBits;

	// Clip planes in surface space used to calculate the cull bits.
	idPlane localClipPlanes[6];
};

// Pre-generated shadow volumes from dmap are not present in surfaceInteraction_t,
// they are added separately.
struct surfaceInteraction_t {
	// The vertexes for light tris will always come from ambient triangles.
	// For interactions created at load time, the indexes will be uniquely
	// generated in static vertex memory.
	int				  numLightTrisIndexes;
	vertCacheHandle_t lightTrisIndexCache;
};

class idRenderEntityLocal;
class idRenderLightLocal;
class RenderEnvprobeLocal;

/*!
	\class idInteraction
	\brief Manages interactions between render entities and light definitions.

	The idInteraction class represents and manages the relationship between render entities and light definitions, handling the linking and un-linking of interactions within the rendering system. It
   supports operations for creating static interactions, freeing surface data, and managing the lifecycle of interaction objects. The class provides functionality to determine if an interaction is
   empty or deferred, and whether it has shadow data. It handles memory management through allocation and freeing mechanisms, allowing interactions to be properly linked and unlinked from entity and
   light lists. This class is designed to support efficient rendering by maintaining proper interaction state and enabling the creation of static interactions for optimized rendering.

*/
class idInteraction
{
public:
	// this may be 0 if the light and entity do not actually intersect
	// -1 = an untested interaction
	int					  numSurfaces;

	// if there is a whole-entity optimized shadow hull, it will
	// be present as a surfaceInteraction_t with a NULL ambientTris, but
	// possibly having a shader to specify the shadow sorting order
	// (FIXME: actually try making shadow hulls?  we never did.)
	surfaceInteraction_t* surfaces;

	// get space from here, if NULL, it is a pre-generated shadow volume from dmap
	idRenderEntityLocal*  entityDef;
	idRenderLightLocal*	  lightDef;

	idInteraction*		  lightNext; // for lightDef chains
	idInteraction*		  lightPrev;
	idInteraction*		  entityNext; // for entityDef chains
	idInteraction*		  entityPrev;

	bool				  staticInteraction; // true if the interaction was created at map load time in static buffer space

public:
	//! Initializes a new instance of the idInteraction class with default values.
	idInteraction();

	//! Allocates and links a new interaction between a render entity and light definition.
	static idInteraction* AllocAndLink( idRenderEntityLocal* edef, idRenderLightLocal* ldef );

	//! Unlinks the interaction from entity and light, frees all surface interactions, and puts it back on the free list
	void				  UnlinkAndFree();

	//! Frees the interaction surfaces and marks the interaction as not static.
	void				  FreeSurfaces();

	//! Makes the interaction empty by clearing its surfaces and relinking it at the end of both entity's and light's interaction lists.
	void				  MakeEmpty();

	//! Returns true if the interaction is empty
	bool				  IsEmpty() const { return ( numSurfaces == 0 ); }

	//! Returns true if the interaction is not yet completely created.
	bool				  IsDeferred() const { return ( numSurfaces == -1 ); }

	//! Returns true if the interaction has shadows.
	bool				  HasShadows() const;

	//! Initializes a static interaction for the entity with the specified command list
	void				  CreateStaticInteraction( nvrhi::ICommandList* commandList );

private:
	//! Unlinks the interaction from both the entity's and light's interaction lists.
	void Unlink();
};

//! Displays memory usage statistics for light interactions and related data structures
void R_ShowInteractionMemory_f( const idCmdArgs& args );

#endif /* !__INTERACTION_H__ */
