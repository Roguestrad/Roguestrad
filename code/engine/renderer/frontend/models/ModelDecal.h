/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

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

#ifndef __MODELDECAL_H__
#define __MODELDECAL_H__

/*
===============================================================================

	Decals are lightweight primitives for bullet / blood marks on static
	geometry. Decals with common materials will be merged together, but
	additional decals will be allocated as needed. The material should not
	be one that receives lighting, because no interactions are generated
	for these lightweight surfaces.

	FIXME:	Decals on models in portalled off areas do not get freed
			until the area becomes visible again.

===============================================================================
*/

static const int NUM_DECAL_BOUNDING_PLANES = 6;
#ifdef ID_PC
static const int MAX_DEFERRED_DECALS	= 16;
static const int DEFFERED_DECAL_TIMEOUT = 1000; // don't create a decal if it wasn't visible within the first second
static const int MAX_DECALS				= 128;
#else
static const int MAX_DEFERRED_DECALS	= 16;
static const int DEFFERED_DECAL_TIMEOUT = 200; // don't create a decal if it wasn't visible within the first 200 milliseconds
static const int MAX_DECALS				= 128;
#endif
static const int MAX_DECAL_VERTS   = 3 + NUM_DECAL_BOUNDING_PLANES + 3 + 6; // 3 triangle verts clipped NUM_DECAL_BOUNDING_PLANES + 3 times (plus 6 for safety)
static const int MAX_DECAL_INDEXES = ( MAX_DECAL_VERTS - 2 ) * 3;

compile_time_assert( CONST_ISPOWEROFTWO( MAX_DECALS ) );
// the max indices must be a multiple of 2 for copying indices to write-combined memory
compile_time_assert( ( ( MAX_DECAL_INDEXES * sizeof( triIndex_t ) ) & 15 ) == 0 );

struct decalProjectionParms_t {
	idPlane			  boundingPlanes[NUM_DECAL_BOUNDING_PLANES];
	idPlane			  fadePlanes[2];
	idPlane			  textureAxis[2];
	idVec3			  projectionOrigin;
	idBounds		  projectionBounds;
	const idMaterial* material;
	float			  fadeDepth;
	int				  startTime;
	bool			  parallel;
	bool			  force;
};

// RB begin
#if defined( _WIN32 )
ALIGNTYPE16 struct decal_t
#else
struct decal_t
#endif
{
	ALIGNTYPE16 idDrawVert verts[MAX_DECAL_VERTS];
	ALIGNTYPE16 triIndex_t indexes[MAX_DECAL_INDEXES];
	float				   vertDepthFade[MAX_DECAL_VERTS];
	int					   numVerts;
	int					   numIndexes;
	int					   startTime;
	const idMaterial*	   material;
}
#if !defined( _WIN32 )
ALIGNTYPE16
#endif
	// RB end
	;

/*!
	\class idRenderModelDecal
	\brief Manages decal rendering for 3D models by handling decal creation, tracking, and rendering.

	This class serves as a central manager for decals applied to 3D models, providing functionality to create, track, and render decals efficiently. It supports deferred decal creation, where
   projection parameters are stored and applied later to specific models. The class handles both parallel and perspective projections, computes necessary bounding and texture coordinate data, and
   manages the lifecycle of decals including fading and removal. Decals are organized by materials, and the class provides mechanisms to merge decals when possible to reduce rendering overhead. It
   also provides utilities to transform decal parameters between global and local coordinate spaces, and to create draw surfaces for rendering decals in the scene.

*/
class idRenderModelDecal
{
public:
	//! Initializes all decal-related member variables to their default values.
	idRenderModelDecal();
	~idRenderModelDecal();

	/*!
		\brief Creates decal projection parameters from a winding, projection origin, and material.

		This function initializes projection parameters for a decal based on the provided winding, projection origin, and material. It validates the winding size, computes the projection bounds, and
	   calculates the bounding planes, texture axes, and fade planes. The function supports both parallel and non-parallel projections, adjusting the bounds and plane calculations accordingly. It also
	   computes the texture coordinate axes for the decal based on the winding's texture coordinates.

		\param parms Output structure to store the computed projection parameters
		\param winding The winding defining the decal's boundary
		\param projectionOrigin The origin point from which the decal is projected
		\param parallel Flag indicating whether the projection is parallel or perspective
		\param fadeDepth The depth over which the decal fades
		\param material The material to be applied to the decal
		\param startTime The start time for the decal effect
		\return True if the projection parameters were successfully created, false otherwise.
	*/
	static bool CreateProjectionParms(
		decalProjectionParms_t& parms, const idFixedWinding& winding, const idVec3& projectionOrigin, const bool parallel, const float fadeDepth, const idMaterial* material, const int startTime );

	/*!
		\brief Transforms decal projection parameters from global space to local space using the provided origin and axis.

		This function converts projection parameters from global coordinate space to local coordinate space for a decal. It applies a transformation matrix derived from the provided origin and axis to
	   each of the bounding planes, fade planes, and texture axes. The projection origin is transformed using the model matrix, and the projection bounds are adjusted by translating and rotating them
	   according to the specified origin and axis. The material, parallel flag, fade depth, start time, and force parameters are copied directly from the global parameters.

		\param localParms Output structure containing the transformed decal projection parameters in local space
		\param globalParms Input structure containing the decal projection parameters in global space
		\param origin The origin of the local coordinate system
		\param axis The axis orientation of the local coordinate system
	*/
	static void		   GlobalProjectionParmsToLocal( decalProjectionParms_t& localParms, const decalProjectionParms_t& globalParms, const idVec3& origin, const idMat3& axis );

	//! Clears the model for reuse by resetting all decal tracking variables.
	void			   ReUse();

	//! Adds a decal projection parameters to the deferred decals list for later rendering.
	void			   AddDeferredDecal( const decalProjectionParms_t& localParms );

	//! Creates decals on the given model from deferred decal parameters.
	void			   CreateDeferredDecals( const idRenderModel* model );

	//! Removes decals that have fully faded away based on the provided time.
	void			   RemoveFadedDecals( int time );

	//! Returns the number of unique decal materials used in the decal model.
	unsigned int	   GetNumDecalDrawSurfs();

	//! Creates a decal draw surface for the specified view entity and decal index
	struct drawSurf_t* CreateDecalDrawSurf( const struct viewEntity_t* space, unsigned int index );

private:
	decal_t				   decals[MAX_DECALS];
	unsigned int		   firstDecal;
	unsigned int		   nextDecal;

	decalProjectionParms_t deferredDecals[MAX_DEFERRED_DECALS];
	unsigned int		   firstDeferredDecal;
	unsigned int		   nextDeferredDecal;

	const idMaterial*	   decalMaterials[MAX_DECALS];
	unsigned int		   numDecalMaterials;

	/*!
		\brief Creates a decal from a winding by adding vertices and indices to an existing decal or a new one

		The function attempts to merge the new decal with the previous one if conditions are met such as matching material, start time, and sufficient vertex/index capacity. Otherwise, it creates a
	   new decal entry. It processes the winding points to generate vertex data with depth fade values based on the provided fade planes and depth. The function also ensures that the index buffer size
	   is aligned to 16-byte boundaries by adding degenerate triangles as needed

		\param w The winding defining the decal shape
		\param decalMaterial The material to apply to the decal
		\param fadePlanes Two planes used to calculate depth fade values for vertices
		\param fadeDepth The depth over which the fade occurs
		\param startTime The start time for the decal
	*/
	void				   CreateDecalFromWinding( const idWinding& w, const idMaterial* decalMaterial, const idPlane fadePlanes[2], float fadeDepth, int startTime );

	//! Creates a decal on the given model using the specified projection parameters
	void				   CreateDecal( const idRenderModel* model, const decalProjectionParms_t& localParms );
};

#endif /* !__MODELDECAL_H__ */
