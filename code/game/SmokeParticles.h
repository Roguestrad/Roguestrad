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

#ifndef __SMOKEPARTICLES_H__
#define __SMOKEPARTICLES_H__

typedef struct singleSmoke_s {
	struct singleSmoke_s* next;
	int					  privateStartTime; // start time for this particular particle
	int					  index;			// particle index in system, 0 <= index < stage->totalParticles
	idRandom			  random;
	idVec3				  origin;
	idMat3				  axis;
	int					  timeGroup;
} singleSmoke_t;

typedef struct {
	const idParticleStage* stage;
	singleSmoke_t*		   smokes;
} activeSmokeStage_t;

/*!
	\class idSmokeParticles
	\brief Manages smoke particle effects for the rendering system.

	Smoke systems are for particles that are emitted off of things that are
	constantly changing position and orientation, like muzzle smoke coming
	from a bone on a weapon, blood spurting from a wound, or particles
	trailing from a monster limb.

	The smoke particles are always evaluated and rendered each tic, so there
	is a performance cost with using them for continuous effects. The general
	particle systems are completely parametric, and have no performance
	overhead when not in view.

	All smoke systems share the same shaderparms, so any coloration must be
	done in the particle definition.

	Each particle model has its own shaderparms, which can be used by the
	particle materials.
*/
class idSmokeParticles
{
public:
	//! Initializes a new instance of the idSmokeParticles class.
	idSmokeParticles();

	//! Initializes the smoke particles system
	void Init();

	//! Shuts down the smoke particles system by freeing associated render resources and marking the system as uninitialized.
	void Shutdown();

	/*!
		\brief Emits smoke particles from a specified source with given parameters.

		This function generates smoke particles based on a particle system definition. It handles the timing and distribution of particles across multiple stages of the smoke system. The function
	   returns whether the smoke system will continue emitting particles in future frames. It performs various checks including validity of the smoke definition, game state, and client connection
	   status before proceeding with particle generation. The function manages active smoke stages and allocates particles from a pool of free smoke entities.

		\param smoke Pointer to the particle definition describing the smoke effect to emit
		\param startTime The start time for the smoke system in game time units
		\param diversity A factor controlling randomness in particle placement and timing
		\param origin The 3D position where the smoke should originate
		\param axis The orientation matrix defining the direction and rotation of the smoke
		\param timeGroup Group identifier for time-based operations
		\return True if the smoke system will continue emitting particles in future frames, false otherwise
	*/
	bool EmitSmoke( const idDeclParticle* smoke, const int startTime, const float diversity, const idVec3& origin, const idMat3& axis, int timeGroup );

	//! Frees smoke particle objects that have exceeded their lifetime.
	void FreeSmokes();

private:
	bool									 initialized;

	renderEntity_t							 renderEntity;		 // used to present a model to the renderer
	int										 renderEntityHandle; // handle to static renderer model

	static const int						 MAX_SMOKE_PARTICLES = 10000;
	singleSmoke_t							 smokes[MAX_SMOKE_PARTICLES];

	idList<activeSmokeStage_t, TAG_PARTICLE> activeStages;
	singleSmoke_t*							 freeSmokes;
	int										 numActiveSmokes;
	int										 currentParticleTime; // don't need to recalculate if == view time

	//! Updates the render entity for smoke particle effects based on the current render view.
	bool									 UpdateRenderEntity( renderEntity_s* renderEntity, const renderView_t* renderView );

	//! Updates the smoke particles render entity based on the provided render view.
	static bool								 ModelCallback( renderEntity_s* renderEntity, const renderView_t* renderView );
};

#endif /* !__SMOKEPARTICLES_H__ */
