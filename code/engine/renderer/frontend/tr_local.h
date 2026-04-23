/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2023 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __TR_LOCAL_H__
#define __TR_LOCAL_H__

#include "precompiled.h"

#include "../GLState.h"
#include "../ScreenRect.h"
#include "../Image.h"
#include "../Font.h"

// maximum texture units
const int	MAX_PROG_TEXTURE_PARMS = 16;

const int	FALLOFF_TEXTURE_SIZE = 64;

const float DEFAULT_FOG_DISTANCE = 500.0f;

// picky to get the bilerp correct at terminator
const int	FOG_ENTER_SIZE = 64;
const float FOG_ENTER	   = ( FOG_ENTER_SIZE + 1.0f ) / ( FOG_ENTER_SIZE * 2 );

/*
==============================================================================

SURFACES

==============================================================================
*/

#include "models/ModelDecal.h"
#include "models/ModelOverlay.h"
#include "Interaction.h"

// RB begin
#define MOC_MULTITHREADED 0

#if MOC_MULTITHREADED
class CullingThreadpool;
#endif
class MaskedOcclusionCulling;
// RB end

class Framebuffer;
class idRenderWorldLocal;
struct viewEntity_t;
struct viewLight_t;
struct viewEnvprobe_t;

// drawSurf_t structures command the back end to render surfaces
// a given srfTriangles_t may be used with multiple viewEntity_t,
// as when viewed in a subview or multiple viewport render, or
// with multiple shaders when skinned, or, possibly with multiple
// lights, although currently each lighting interaction creates
// unique srfTriangles_t
// drawSurf_t are always allocated and freed every frame, they are never cached

struct drawSurf_t {
	const srfTriangles_t*	   frontEndGeo; // don't use on the back end, it may be updated by the front end!
	int						   numIndexes;
	vertCacheHandle_t		   indexCache;	 // triIndex_t
	vertCacheHandle_t		   ambientCache; // idDrawVert
	vertCacheHandle_t		   jointCache;	 // idJointMat
	const viewEntity_t*		   space;
	const idMaterial*		   material;		// may be NULL for shadow volumes
	uint64					   extraGLState;	// Extra GL state |'d with material->stage[].drawStateBits
	float					   sort;			// material->sort, modified by gui / entity sort offsets
	const float*			   shaderRegisters; // evaluated and adjusted for referenceShaders
	drawSurf_t*				   nextOnLight;		// viewLight chains
	drawSurf_t**			   linkChain;		// defer linking to lights to a serial section to avoid a mutex
	idScreenRect			   scissorRect;		// for scissor clipping, local inside renderView viewport
	const struct portalArea_s* area;			// RB: if != NULL then the area provides valid lightgrid
};

// areas have references to hold all the lights and entities in them
struct areaReference_t {
	areaReference_t*	 areaNext; // chain in the area
	areaReference_t*	 areaPrev;
	areaReference_t*	 ownerNext; // chain on either the entityDef or lightDef

	idRenderEntityLocal* entity;   // only one of entity / light / envprobe will be non-NULL
	idRenderLightLocal*	 light;	   // only one of entity / light / envprobe will be non-NULL
	RenderEnvprobeLocal* envprobe; // only one of entity / light / envprobe will be non-NULL

	struct portalArea_s* area; // so owners can find all the areas they are in
};

/*!
	\class idRenderLight
	\brief Abstract base class defining the interface for render light objects.
*/
class idRenderLight
{
public:
	virtual ~idRenderLight() { }

	virtual void FreeRenderLight()														= 0;
	virtual void UpdateRenderLight( const renderLight_t* re, bool forceUpdate = false ) = 0;
	virtual void GetRenderLight( renderLight_t* re )									= 0;
	virtual void ForceUpdate()															= 0;
	virtual int	 GetIndex()																= 0;
};

/*!
	\class RenderEnvprobe
	\brief Provides a virtual interface for environment probe rendering management.
*/
class RenderEnvprobe
{
public:
	virtual ~RenderEnvprobe() { }

	virtual void FreeRenderEnvprobe()																  = 0;
	virtual void UpdateRenderEnvprobe( const renderEnvironmentProbe_t* ep, bool forceUpdate = false ) = 0;
	virtual void GetRenderEnvprobe( renderEnvironmentProbe_t* ep )									  = 0;
	virtual void ForceUpdate()																		  = 0;
	virtual int	 GetIndex()																			  = 0;
};

/*!
	\class idRenderEntity
	\brief idRenderEntity serves as a public interface for render entity management within the rendering system.

	This class defines the interface for render entities, providing methods to manage entity state, update rendering information, and interact with the rendering world. The interface includes virtual
   functions for freeing entity resources, updating entity data, retrieving entity information, forcing updates, and managing decals and overlays. The class is designed to replace the older qhandle_t
   based entity definition system with a more structured approach to entity management. The pure virtual methods indicate that concrete implementations must provide specific behavior for each
   function, making this class a contract for render entity functionality.

*/
class idRenderEntity
{
public:
	virtual ~idRenderEntity() { }

	virtual void FreeRenderEntity()														  = 0;
	virtual void UpdateRenderEntity( const renderEntity_t* re, bool forceUpdate = false ) = 0;
	virtual void GetRenderEntity( renderEntity_t* re )									  = 0;
	virtual void ForceUpdate()															  = 0;
	virtual int	 GetIndex()																  = 0;

	// overlays are extra polygons that deform with animating models for blood and damage marks
	virtual void ProjectOverlay( const idPlane localTextureAxis[2], const idMaterial* material ) = 0;
	virtual void RemoveDecals()																	 = 0;
};

/*!
	\class idRenderLightLocal
	\brief Manages local render light state and updates for rendering.

	The idRenderLightLocal class provides functionality for maintaining and updating the local state of a render light used in the rendering system. It handles initialization, resource management
   through freeing, and synchronization with render data structures. The class supports updating light parameters from external sources and can force updates when necessary. It also provides access to
   light casting shadow behavior and retrieves the light's index within the rendering system. The class serves as a bridge between the core light data and the rendering pipeline, ensuring that light
   properties are accurately reflected in the rendered output.

*/
class idRenderLightLocal : public idRenderLight
{
public:
	//! Initializes a new instance of the idRenderLightLocal class with default values.
	idRenderLightLocal();

	//! Frees the render light resources.
	virtual void		   FreeRenderLight();

	//! Updates the render light parameters with the provided data and optionally forces an update
	virtual void		   UpdateRenderLight( const renderLight_t* re, bool forceUpdate = false );

	//! Populates the provided renderLight_t structure with the light's render data
	virtual void		   GetRenderLight( renderLight_t* re );

	//! Forces an update of the render light local state.
	virtual void		   ForceUpdate();

	//! Returns the index of the render light local object.
	virtual int			   GetIndex();

	//! Determines whether the light casts shadows based on its parameters and shader settings.
	bool				   LightCastsShadows() const { return parms.forceShadows || ( !parms.noShadows && lightShader->LightCastsShadows() ); }

	renderLight_t		   parms; // specification

	bool				   lightHasMoved; // the light has changed its position since it was
	// first added, so the prelight model is not valid
	idRenderWorldLocal*	   world;
	int					   index; // in world lightdefs

	int					   areaNum; // if not -1, we may be able to cull all the light's
	// interactions if !viewDef->connectedAreas[areaNum]

	int					   lastModifiedFrameNum; // to determine if it is constantly changing,
	// and should go in the dynamic frame memory, or kept
	// in the cached memory

	// derived information
	idPlane				   lightProject[4];			// old style light projection where Z and W are flipped and projected lights lightProject[3] is divided by ( zNear + zFar )
	idRenderMatrix		   baseLightProject;		// global xyz1 to projected light strq
	idRenderMatrix		   inverseBaseLightProject; // transforms the zero-to-one cube to exactly cover the light in world space

	const idMaterial*	   lightShader; // guaranteed to be valid, even if parms.shader isn't
	idImage*			   falloffImage;

	idVec3				   globalLightOrigin; // accounting for lightCenter and parallel
	idBounds			   globalLightBounds;

	int					   viewCount; // if == tr.viewCount, the light is on the viewDef->viewLights list
	viewLight_t*		   viewLight;

	areaReference_t*	   references;		 // each area the light is present in will have a lightRef
	idInteraction*		   firstInteraction; // doubly linked list
	idInteraction*		   lastInteraction;

	struct doublePortal_s* foggedPortals;
};

/*!
	\class RenderEnvprobeLocal
	\brief A local implementation of an environment probe renderer.
*/
class RenderEnvprobeLocal : public RenderEnvprobe
{
public:
	//! Initializes a RenderEnvprobeLocal object with default values.
	RenderEnvprobeLocal();

	//! Frees the environment probe resources.
	virtual void			 FreeRenderEnvprobe() override;

	//! Updates the render environment probe with the specified parameters and optional forced update flag
	virtual void			 UpdateRenderEnvprobe( const renderEnvironmentProbe_t* ep, bool forceUpdate = false ) override;

	//! Copies the environment probe data into the provided render environment probe structure.
	virtual void			 GetRenderEnvprobe( renderEnvironmentProbe_t* ep ) override;

	//! Forces an update of the environment probe rendering.
	virtual void			 ForceUpdate() override;

	//! Returns the index of the render environment probe.
	virtual int				 GetIndex() override;

	renderEnvironmentProbe_t parms; // specification

	bool					 envprobeHasMoved; // the light has changed its position since it was
	// first added, so the prelight model is not valid
	idRenderWorldLocal*		 world;
	int						 index; // in world envprobeDefs

	int						 areaNum; // if not -1, we may be able to cull all the envprobe's
	// interactions if !viewDef->connectedAreas[areaNum]

	int						 lastModifiedFrameNum; // to determine if it is constantly changing,
	// and should go in the dynamic frame memory, or kept
	// in the cached memory

	// derived information
	idRenderMatrix			 inverseBaseProbeProject; // transforms the zero-to-one cube to exactly cover the light in world space

	idBounds				 globalProbeBounds;

	areaReference_t*		 references; // each area the light is present in will have a lightRef

	idImage*				 irradianceImage; // cubemap image used for diffuse IBL by backend
	idImage*				 radianceImage;	  // cubemap image used for specular IBL by backend

	// temporary helpers
	int						 viewCount; // if == tr.viewCount, the envprobe is on the viewDef->viewEnvprobes list
	viewEnvprobe_t*			 viewEnvprobe;
};

/*!
	\class idRenderEntityLocal
	\brief Manages local rendering state and updates for render entities.

	This class provides the local implementation for rendering entities, handling updates, visibility determination, and rendering data management. It maintains the connection between entity data and
   the rendering system by providing methods to update rendering parameters, retrieve current rendering state, and manage visual overlays like decals and texture projections. The class is designed to
   integrate with the rendering pipeline through its inheritance from idRenderEntity and supports forced updates and direct visibility checks for optimization purposes.

*/
class idRenderEntityLocal : public idRenderEntity
{
public:
	//! Initializes a new instance of the idRenderEntityLocal class.
	idRenderEntityLocal();

	//! Frees the render entity resources.
	virtual void		  FreeRenderEntity();

	//! Updates the render entity with the provided parameters and optional force update flag.
	virtual void		  UpdateRenderEntity( const renderEntity_t* re, bool forceUpdate = false );

	//! Populates the provided renderEntity_t structure with the entity's rendering data
	virtual void		  GetRenderEntity( renderEntity_t* re );

	//! Forces an update of the render entity local.
	virtual void		  ForceUpdate();

	//! Returns the index of the render entity local.
	virtual int			  GetIndex();

	//! Projects an overlay onto the entity using the specified texture coordinates and material.
	virtual void		  ProjectOverlay( const idPlane localTextureAxis[2], const idMaterial* material );

	//! Removes all decals from the render entity
	virtual void		  RemoveDecals();

	//! Returns true if the render entity is directly visible to the current view
	bool				  IsDirectlyVisible() const;
	renderEntity_t		  parms;

	float				  modelMatrix[16]; // this is just a rearrangement of parms.axis and parms.origin
	idRenderMatrix		  modelRenderMatrix;
	idRenderMatrix		  inverseBaseModelProject; // transforms the unit cube to exactly cover the model in world space

	idRenderWorldLocal*	  world;
	int					  index; // in world entityDefs

	int					  lastModifiedFrameNum; // to determine if it is constantly changing,
	// and should go in the dynamic frame memory, or kept
	// in the cached memory

	idRenderModel*		  dynamicModel;			  // if parms.model->IsDynamicModel(), this is the generated data
	int					  dynamicModelFrameCount; // continuously animating dynamic models will recreate
	// dynamicModel if this doesn't == tr.viewCount
	idRenderModel*		  cachedDynamicModel;

	// the local bounds used to place entityRefs, either from parms for dynamic entities, or a model bounds
	idBounds			  localReferenceBounds;

	// axis aligned bounding box in world space, derived from refernceBounds and
	// modelMatrix in R_CreateEntityRefs()
	idBounds			  globalReferenceBounds;

	// a viewEntity_t is created whenever a idRenderEntityLocal is considered for inclusion
	// in a given view, even if it turns out to not be visible
	int					  viewCount; // if tr.viewCount == viewCount, viewEntity is valid,
	// but the entity may still be off screen
	viewEntity_t*		  viewEntity; // in frame temporary memory

	idRenderModelDecal*	  decals;	// decals that have been projected on this model
	idRenderModelOverlay* overlays; // blood overlays on animated models

	areaReference_t*	  entityRefs;		// chain of all references
	idInteraction*		  firstInteraction; // doubly linked list
	idInteraction*		  lastInteraction;

	bool				  needsPortalSky;
};

struct shadowOnlyEntity_t {
	shadowOnlyEntity_t*	 next;
	idRenderEntityLocal* edef;
};

// viewLights are allocated on the frame temporary stack memory
// a viewLight contains everything that the back end needs out of an idRenderLightLocal,
// which the front end may be modifying simultaniously if running in SMP mode.
// a viewLight may exist even without any surfaces, and may be relevent for fogging,
// but should never exist if its volume does not intersect the view frustum
struct viewLight_t {
	viewLight_t*		next;

	// back end should NOT reference the lightDef, because it can change when running SMP
	idRenderLightLocal* lightDef;

	// for scissor clipping, local inside renderView viewport
	// scissorRect.Empty() is true if the viewEntity_t was never actually
	// seen through any portals
	idScreenRect		scissorRect;

	// R_AddSingleLight() determined that the light isn't actually needed
	bool				removeFromList;

	// R_AddSingleLight builds this list of entities that need to be added
	// to the viewEntities list because they potentially cast shadows into
	// the view, even though the aren't directly visible
	shadowOnlyEntity_t* shadowOnlyViewEntities;

	enum interactionState_t { INTERACTION_UNCHECKED, INTERACTION_NO, INTERACTION_YES };
	byte*			  entityInteractionState; // [numEntities]

	idVec3			  globalLightOrigin; // global light origin used by backend
	idPlane			  lightProject[4];	 // light project used by backend
	idPlane			  fogPlane;			 // fog plane for backend fog volume rendering
	// RB: added for shadow mapping
	idRenderMatrix	  baseLightProject; // global xyz1 to projected light strq
	bool			  pointLight;		// otherwise a projection light (should probably invert the sense of this, because points are way more common)
	bool			  parallel;			// lightCenter gives the direction to the light at infinity
	idVec3			  lightCenter;		// offset the lighting direction for shading and
	int				  shadowLOD;		// level of detail for shadowmap selection
	float			  shadowFadeOut;	// blending from last shadow LOD to invisible
	idRenderMatrix	  shadowV[6];		// shadow depth view matrix for lighting pass
	idRenderMatrix	  shadowP[6];		// shadow depth projection matrix for lighting pass
	idVec2i			  imageSize;
	idVec2i			  imageAtlasOffset[6];
	// RB end
	idRenderMatrix	  inverseBaseLightProject; // the matrix for deforming the 'zeroOneCubeModel' to exactly cover the light volume in world space
	const idMaterial* lightShader;			   // light shader used by backend
	const float*	  shaderRegisters;		   // shader registers used by backend
	idImage*		  falloffImage;			   // falloff image used by backend

	drawSurf_t*		  globalShadows;		   // shadow everything
	drawSurf_t*		  localInteractions;	   // don't get local shadows
	drawSurf_t*		  localShadows;			   // don't shadow local surfaces
	drawSurf_t*		  globalInteractions;	   // get shadows from everything
	drawSurf_t*		  translucentInteractions; // translucent interactions don't get shadows

	//! Checks if the light's image atlas coordinates have been set.
	bool			  ImageAtlasPlaced() const { return ( imageSize.x != -1 ) && ( imageSize.y != -1 ); }
};

// a viewEntity is created whenever a idRenderEntityLocal is considered for inclusion
// in the current view, but it may still turn out to be culled.
// viewEntity are allocated on the frame temporary stack memory
// a viewEntity contains everything that the back end needs out of a idRenderEntityLocal,
// which the front end may be modifying simultaneously if running in SMP mode.
// A single entityDef can generate multiple viewEntity_t in a single frame, as when seen in a mirror
struct viewEntity_t {
	viewEntity_t*		 next;

	// back end should NOT reference the entityDef, because it can change when running SMP
	idRenderEntityLocal* entityDef;

	// for scissor clipping, local inside renderView viewport
	// scissorRect.Empty() is true if the viewEntity_t was never actually
	// seen through any portals, but was created for shadow casting.
	// a viewEntity can have a non-empty scissorRect, meaning that an area
	// that it is in is visible, and still not be visible.
	idScreenRect		 scissorRect;

	bool				 isGuiSurface; // force two sided and vertex colors regardless of material setting

	bool				 skipMotionBlur;

	bool				 weaponDepthHack;
	float				 modelDepthHack;

	float				 modelMatrix[16];	  // local coords to global coords
	float				 modelViewMatrix[16]; // local coords to eye coords

	idRenderMatrix		 mvp;
	idRenderMatrix		 unjitteredMVP; // no TAA subpixel jittering

	// parallelAddModels will build a chain of surfaces here that will need to
	// be linked to the lights or added to the drawsurf list in a serial code section
	drawSurf_t*			 drawSurfs;
};

// RB: viewEnvprobes are allocated on the frame temporary stack memory
// a viewEnvprobe contains everything that the back end needs out of an RenderEnvprobeLocal,
// which the front end may be modifying simultaniously if running in SMP mode.

// this structure will be especially helpful when we switch RBDOOM-3-BFG to forward cluster shading
// because then we can evaluate all viewEnvprobes properly in each pixel shader along with all other lighting information
struct viewEnvprobe_t {
	viewEnvprobe_t*		 next;

	// back end should NOT reference the lightDef, because it can change when running SMP
	RenderEnvprobeLocal* envprobeDef;

	// for scissor clipping, local inside renderView viewport
	// scissorRect.Empty() is true if the viewEntity_t was never actually
	// seen through any portals
	idScreenRect		 scissorRect;

	// R_AddSingleEnvprobe() determined that the light isn't actually needed
	bool				 removeFromList;

	idVec3				 globalOrigin; // global envprobe origin used by backend
	idBounds			 globalProbeBounds;

	idRenderMatrix		 inverseBaseProbeProject; // the matrix for deforming the 'zeroOneCubeModel' to exactly cover the light volume in world space
	idImage*			 irradianceImage;		  // cubemap image used for diffuse IBL by backend
	idImage*			 radianceImage;			  // cubemap image used for specular IBL by backend
};

struct calcEnvprobeParms_t {
	// input
	byte*		 radiance[6]; // HDR RGB16F standard OpenGL cubemap sides
	int			 freeRadiance;
	int			 samples;

	int			 outWidth;
	int			 outHeight;

	bool		 printProgress;
	int			 printWidth;
	int			 printHeight;

	idStr		 filename;

	// output
	halfFloat_t* outBuffer; // HDR RGB16F packed octahedron atlas
	int			 time;		// execution time in milliseconds
};

#define STORE_LIGHTGRID_SHDATA 0

static const int LIGHTGRID_IRRADIANCE_BORDER_SIZE = 2; // one pixel border all around the octahedron so 2 on each side
static const int LIGHTGRID_IRRADIANCE_SIZE		  = 14 + LIGHTGRID_IRRADIANCE_BORDER_SIZE;

struct calcLightGridPointParms_t {
	// input
	byte* radiance[6]; // HDR RGB16F standard OpenGL cubemap sides
	int	  gridCoord[3];

	int	  outWidth; // LIGHTGRID_IRRADIANCE_SIZE
	int	  outHeight;

	// output
#if STORE_LIGHTGRID_SHDATA
	SphericalHarmonicsT<idVec3, 4> shRadiance; // L4 Spherical Harmonics
#endif

	halfFloat_t* outBuffer; // HDR RGB16F octahedron LIGHTGRID_IRRADIANCE_SIZE^2
	int			 time;		// execution time in milliseconds
};
// RB end

const int MAX_CLIP_PLANES = 1; // we may expand this to six for some subview issues

// RB: added multiple subfrustums for cascaded shadow mapping
enum frustumPlanes_t {
	FRUSTUM_PLANE_LEFT,
	FRUSTUM_PLANE_RIGHT,
	FRUSTUM_PLANE_BOTTOM,
	FRUSTUM_PLANE_TOP,
	FRUSTUM_PLANE_NEAR,
	FRUSTUM_PLANE_FAR,
	FRUSTUM_PLANES	= 6,
	FRUSTUM_CLIPALL = 1 | 2 | 4 | 8 | 16 | 32
};

enum {
	FRUSTUM_PRIMARY,
	FRUSTUM_CASCADE1,
	FRUSTUM_CASCADE2,
	FRUSTUM_CASCADE3,
	FRUSTUM_CASCADE4,
	FRUSTUM_CASCADE5,
	MAX_FRUSTUMS,
};

typedef idPlane frustum_t[FRUSTUM_PLANES];
// RB end

// viewDefs are allocated on the frame temporary stack memory
struct viewDef_t {
	// specified in the call to DrawScene()
	renderView_t		renderView;

	float				projectionMatrix[16];
	idRenderMatrix		projectionRenderMatrix; // tech5 version of projectionMatrix

	// RB begin
	float				unjitteredProjectionMatrix[16]; // second version without TAA subpixel jittering
	idRenderMatrix		unjitteredProjectionRenderMatrix;

	float				unprojectionToCameraMatrix[16];
	idRenderMatrix		unprojectionToCameraRenderMatrix;

	float				unprojectionToWorldMatrix[16];
	idRenderMatrix		unprojectionToWorldRenderMatrix;
	// RB end

	viewEntity_t		worldSpace;

	idRenderWorldLocal* renderWorld;

	idVec3				initialViewAreaOrigin;
	// Used to find the portalArea that view flooding will take place from.
	// for a normal view, the initialViewOrigin will be renderView.viewOrg,
	// but a mirror may put the projection origin outside
	// of any valid area, or in an unconnected area of the map, so the view
	// area must be based on a point just off the surface of the mirror / subview.
	// It may be possible to get a failed portal pass if the plane of the
	// mirror intersects a portal, and the initialViewAreaOrigin is on
	// a different side than the renderView.viewOrg is.

	bool				isSubview; // true if this view is not the main view
	bool				isMirror;  // the portal is a mirror, invert the face culling
	bool				isXraySubview;

	bool				isEditor;
	guiMode_t			guiMode; // Leyland VR

	bool				isObliqueProjection;		 // true if this view has an oblique projection
	int					numClipPlanes;				 // mirrors will often use a single clip plane
	idPlane				clipPlanes[MAX_CLIP_PLANES]; // in world space, the positive side
	// of the plane is the visible side
	idScreenRect		viewport; // in real pixels and proper Y flip

	idScreenRect		scissor;
	// for scissor clipping, local inside renderView viewport
	// subviews may only be rendering part of the main view
	// these are real physical pixel values, possibly scaled and offset from the
	// renderView x/y/width/height

	viewDef_t*			superView; // never go into an infinite subview loop
	const drawSurf_t*	subviewSurface;

	// drawSurfs are the visible surfaces of the viewEntities, sorted
	// by the material sort parameter
	drawSurf_t**		drawSurfs;	  // we don't use an idList for this, because
	int					numDrawSurfs; // it is allocated in frame temporary memory
	int					maxDrawSurfs; // may be resized

	viewLight_t*		viewLights;	 // chain of all viewLights effecting view
	viewEntity_t*		viewEntitys; // chain of all viewEntities effecting view, including off screen ones casting shadows
	// we use viewEntities as a check to see if a given view consists solely
	// of 2D rendering, which we can optimize in certain ways.  A 2D view will
	// not have any viewEntities

	// RB begin
	frustum_t			frustums[MAX_FRUSTUMS]; // positive sides face outward, [4] is the front clip plane
	float				frustumSplitDistances[MAX_FRUSTUMS];
	idRenderMatrix		frustumMVPs[MAX_FRUSTUMS];
	// RB end

	int					areaNum; // -1 = not in a valid area

	// An array in frame temporary memory that lists if an area can be reached without
	// crossing a closed door.  This is used to avoid drawing interactions
	// when the light is behind a closed door.
	bool*				connectedAreas;

	// RB: collect environment probes like lights
	viewEnvprobe_t*		viewEnvprobes;

	// RB: nearest 3 probes for now
	idBounds			globalProbeBounds;
	idRenderMatrix		inverseBaseEnvProbeProject; // the matrix for deforming the 'zeroOneCubeModel' to exactly cover the environent probe volume in world space
	idImage*			irradianceImage;			// cubemap image used for diffuse IBL by backend
	idImage*			radianceImages[3];			// cubemap image used for specular IBL by backend
	idVec4				radianceImageBlends;		// blending weights
	idVec4				probePositions[3];			// only used by parallax correction

	Framebuffer*		targetRender; // SP: The framebuffer to render to

	int					taaFrameCount; // RB: so we have the same frame index in frontend and backend
};

// complex light / surface interactions are broken up into multiple passes of a
// simple interaction shader
struct drawInteraction_t {
	const viewLight_t* vLight;

	const drawSurf_t*  surf;

	idImage*		   bumpImage;
	idImage*		   diffuseImage;
	idImage*		   specularImage;

	idVec4			   diffuseColor;  // may have a light color baked into it
	idVec4			   specularColor; // may have a light color baked into it
	stageVertexColor_t vertexColor;	  // applies to both diffuse and specular

	// these are loaded into the vertex program
	idVec4			   bumpMatrix[2];
	idVec4			   diffuseMatrix[2];
	idVec4			   specularMatrix[2];
};

/*
=============================================================

RENDERER BACK END COMMAND QUEUE

TR_CMDS

=============================================================
*/

enum renderCommand_t {
	RC_NOP,
	RC_DRAW_VIEW_3D,  // may be at a reduced resolution, will be upsampled before 2D GUIs
	RC_DRAW_VIEW_GUI, // not resolution scaled
	RC_SET_BUFFER,
	RC_COPY_RENDER,
	RC_POST_PROCESS,	 // postfx after scene rendering is done but before GUI rendering
	RC_CRT_POST_PROCESS, // CRT simulation after everything has been rendered on the final swapchain image
};

struct emptyCommand_t {
	renderCommand_t	 commandId;
	renderCommand_t* next;
};

struct setBufferCommand_t {
	renderCommand_t	 commandId;
	renderCommand_t* next;
	int				 buffer;
};

struct drawSurfsCommand_t {
	renderCommand_t	 commandId;
	renderCommand_t* next;
	viewDef_t*		 viewDef;
};

struct copyRenderCommand_t {
	renderCommand_t	 commandId;
	renderCommand_t* next;
	int				 viewEyeBuffer; // Leyland VR
	int				 x;
	int				 y;
	int				 imageWidth;
	int				 imageHeight;
	idImage*		 image;
	int				 cubeFace; // when copying to a cubeMap
	bool			 clearColorAfterCopy;
};

struct postProcessCommand_t {
	renderCommand_t	 commandId;
	renderCommand_t* next;
	viewDef_t*		 viewDef;
};

struct crtPostProcessCommand_t {
	renderCommand_t	 commandId;
	renderCommand_t* next;
	int				 padding;
};

//=======================================================================

// this is the inital allocation for max number of drawsurfs
// in a given view, but it will automatically grow if needed
const int INITIAL_DRAWSURFS = 2048;

enum frameAllocType_t {
	FRAME_ALLOC_VIEW_DEF,
	FRAME_ALLOC_VIEW_ENTITY,
	FRAME_ALLOC_VIEW_LIGHT,
	FRAME_ALLOC_SURFACE_TRIANGLES,
	FRAME_ALLOC_DRAW_SURFACE,
	FRAME_ALLOC_INTERACTION_STATE,
	FRAME_ALLOC_SHADOW_ONLY_ENTITY,
	FRAME_ALLOC_SHADOW_VOLUME_PARMS,
	FRAME_ALLOC_SHADER_REGISTER,
	FRAME_ALLOC_DRAW_SURFACE_POINTER,
	FRAME_ALLOC_DRAW_COMMAND,
	FRAME_ALLOC_UNKNOWN,
	FRAME_ALLOC_MAX
};

/*!
	\class idFrameData
	\brief Container for backend rendering data that supports parallel execution on SMP machines.
*/
class idFrameData
{
public:
	idSysInterlockedInteger frameMemoryAllocated;
	idSysInterlockedInteger frameMemoryUsed;
	byte*					frameMemory;

	int						highWaterAllocated; // max used on any frame
	int						highWaterUsed;

	// the currently building command list commands can be inserted
	// at the front if needed, as required for dynamically generated textures
	emptyCommand_t*			cmdHead; // may be of other command type based on commandId
	emptyCommand_t*			cmdTail;
};

extern idFrameData* frameData;

//! Adds a draw view command to the rendering queue with optional GUI-only rendering.
void				R_AddDrawViewCmd( viewDef_t* parms, bool guiOnly );

//! Issues a command to perform post-processing after all views have been rendered
void				R_AddDrawPostProcess( viewDef_t* parms );

//! Reloads GUI files based on the provided command arguments.
void				R_ReloadGuis_f( const idCmdArgs& args );

//! Lists all available GUIs.
void				R_ListGuis_f( const idCmdArgs& args );

//! Returns memory for a command buffer and links it to the end of the current command chain
void*				R_GetCommandBuffer( int bytes );

//! Applies a global shader override to the provided material if applicable.
bool				R_GlobalShaderOverride( const idMaterial** shader );

//! Remaps a shader based on a custom skin or custom shader, with various validation checks.
const idMaterial*	R_RemapShaderBySkin( const idMaterial* shader, const idDeclSkin* customSkin, const idMaterial* customShader );

//====================================================

enum vertexLayoutType_t {
	LAYOUT_UNKNOWN = 0, // RB: TODO -1
	LAYOUT_DRAW_VERT,
	LAYOUT_DRAW_IMGUI_VERT, // unused
	NUM_VERTEX_LAYOUTS
};

enum bindingLayoutType_t {
	// REGULAR AND SKINNED VERSIONS
	BINDING_LAYOUT_DEFAULT,
	BINDING_LAYOUT_DEFAULT_SKINNED,

	BINDING_LAYOUT_CONSTANT_BUFFER_ONLY,
	BINDING_LAYOUT_CONSTANT_BUFFER_ONLY_SKINNED,

	BINDING_LAYOUT_AMBIENT_LIGHTING_IBL,
	BINDING_LAYOUT_AMBIENT_LIGHTING_IBL_SKINNED,

	BINDING_LAYOUT_DRAW_INTERACTION,
	BINDING_LAYOUT_DRAW_INTERACTION_SKINNED,
	BINDING_LAYOUT_DRAW_INTERACTION_SM,
	BINDING_LAYOUT_DRAW_INTERACTION_SM_SKINNED,

	BINDING_LAYOUT_FOG,
	BINDING_LAYOUT_FOG_SKINNED,
	BINDING_LAYOUT_BLENDLIGHT,
	BINDING_LAYOUT_BLENDLIGHT_SKINNED,

	BINDING_LAYOUT_NORMAL_CUBE,
	BINDING_LAYOUT_NORMAL_CUBE_SKINNED,

	BINDING_LAYOUT_OCTAHEDRON_CUBE,
	BINDING_LAYOUT_OCTAHEDRON_CUBE_SKINNED,

	// NO GPU SKINNING ANYMORE
	BINDING_LAYOUT_POST_PROCESS_INGAME,
	BINDING_LAYOUT_POST_PROCESS_FINAL,
	BINDING_LAYOUT_POST_PROCESS_FINAL2,
	BINDING_LAYOUT_POST_PROCESS_CRT,

	BINDING_LAYOUT_BLIT,
	BINDING_LAYOUT_DRAW_AO,
	BINDING_LAYOUT_DRAW_AO1,

	BINDING_LAYOUT_BINK_VIDEO,

	// SMAA
	BINDING_LAYOUT_SMAA_EDGE_DETECTION,
	BINDING_LAYOUT_SMAA_WEIGHT_CALC,

	// NVRHI render passes specific
	BINDING_LAYOUT_TAA_MOTION_VECTORS,
	BINDING_LAYOUT_TAA_RESOLVE,

	BINDING_LAYOUT_TONEMAP,
	BINDING_LAYOUT_HISTOGRAM,
	BINDING_LAYOUT_EXPOSURE,

	NUM_BINDING_LAYOUTS
};

class idParallelJobList;

const int		 MAX_GUI_SURFACES = 1024; // default size of the drawSurfs list for guis, will
// be automatically expanded as needed

static const int MAX_RENDER_CROPS = 8;

#include "../backend/RenderBackend.h" // for idRenderBackend

/*!
	\class idRenderSystemLocal
	\brief Local implementation of the rendering system interface with graphics resource management and rendering pipeline control.

	This class provides the concrete implementation of the rendering system interface, managing graphics resources, rendering state, and the overall rendering pipeline. It handles initialization and
   shutdown of the graphics subsystem, manages render worlds and their lifecycle, and provides methods for drawing operations including geometric primitives, textured quads, and text rendering. The
   class maintains internal state for rendering parameters such as color, OpenGL state, and stereo rendering settings. It also implements command buffer management for parallel rendering operations
   and provides functionality for screenshot capture and image uploading. The implementation coordinates with backend systems for actual graphics rendering while maintaining frontend-facing interfaces
   for game logic to interact with the rendering subsystem.

*/
class idRenderSystemLocal : public idRenderSystem
{
public:
	//! Initializes the render system by setting up internal state, allocating frame data, and preparing graphical resources.
	virtual void				  Init();

	//! Shuts down the rendering system and frees all allocated resources.
	virtual void				  Shutdown();

	//! Returns true if the render system has been initialized and is ready for use.
	virtual bool				  IsInitialized() const { return bInitialized; }

	//! Resets the GUI models by deleting the existing model, creating a new one, and initializing it.
	virtual void				  ResetGuiModels();

	//! Initializes the rendering backend if it has not already been initialized.
	virtual void				  InitBackend();

	//! Shuts down the OpenGL rendering system
	virtual void				  ShutdownOpenGL();

	//! Checks whether OpenGL is currently running and initialized.
	virtual bool				  IsOpenGLRunning() const;

	//! Returns true if the render system is currently in fullscreen mode.
	virtual bool				  IsFullScreen() const;

	//! Returns the width of the rendering resolution, taking into account VR display settings if active.
	virtual int					  GetWidth() const;

	//! Returns the height of the rendering resolution, accounting for VR mode.
	virtual int					  GetHeight() const;

	//! Returns the native width of the swap chain.
	virtual int					  GetNativeWidth() const;

	//! Returns the native height of the swap chain
	virtual int					  GetNativeHeight() const;

	//! Returns the virtual width of the render system, adjusted for VR or screen resolution settings.
	virtual int					  GetVirtualWidth() const;

	//! Returns the virtual screen height used for rendering and UI layout.
	virtual int					  GetVirtualHeight() const;

	//! Returns the pixel aspect ratio for the current rendering context
	virtual float				  GetPixelAspect() const;

	//! Returns the physical screen width in centimeters, either forced by a console variable or retrieved from the OpenGL configuration.
	virtual float				  GetPhysicalScreenWidthInCentimeters() const;

	//! Allocates and returns a new render world instance for use in rendering operations.
	virtual idRenderWorld*		  AllocRenderWorld();

	//! Frees a render world by removing it from the systems tracking and deleting it
	virtual void				  FreeRenderWorld( idRenderWorld* rw );

	//! Initializes resources and clears caches for a new level load operation.
	virtual void				  BeginLevelLoad();

	//! Finalizes the level loading process by notifying the render model manager and global images manager to complete their respective loading operations.
	virtual void				  EndLevelLoad();

	//! Loads level images for the render system.
	virtual void				  LoadLevelImages();

	//! Preloads rendering assets for a specified map using the given preload manifest and map name.
	virtual void				  Preload( const idPreloadManifest& manifest, const char* mapName );

	//! Initializes automatic background swaps with an optional icon type
	virtual void				  BeginAutomaticBackgroundSwaps( autoRenderIconType_t icon = AUTORENDER_DEFAULTICON );

	//! Ends automatic background swaps for rendering.
	virtual void				  EndAutomaticBackgroundSwaps();

	//! Returns false indicating that automatic background swaps are not running
	virtual bool				  AreAutomaticBackgroundSwapsRunning( autoRenderIconType_t* usingAlternateIcon = NULL ) const;

	//! Registers a font by its name and returns a pointer to the font object.
	virtual idFont*				  RegisterFont( const char* fontName );

	//! Resets the font system by deleting all registered fonts.
	virtual void				  ResetFonts();

	//! Prints memory usage information for images and models
	virtual void				  PrintMemInfo( MemInfo_t* mi );

	//! Sets the current render color using the provided RGBA vector.
	virtual void				  SetColor( const idVec4& color );

	//! Returns the current color value used for rendering.
	virtual uint32				  GetColor();

	//! Sets the OpenGL state for the renderer.
	virtual void				  SetGLState( const uint64 glState );

	//! Sets the stereo depth type for rendering.
	virtual void				  SetStereoDepth( enum stereoDepthType_t );

	/*!
		\brief Draws a filled rectangle with the specified color and dimensions.

		This function renders a solid rectangle on the screen using the provided color and positional dimensions. It sets the color using the internal SetColor method and then draws a stretched
	   picture using a white material with full texture coordinates.

		\param color The color to fill the rectangle with, specified as RGBA values
		\param x The x-coordinate of the rectangle's top-left corner
		\param y The y-coordinate of the rectangle's top-left corner
		\param w The width of the rectangle
		\param h The height of the rectangle
	*/
	virtual void				  DrawFilled( const idVec4& color, float x, float y, float w, float h );

	/*!
		\brief Draws a texture-stretched quad with specified coordinates and texture coordinates using the given material.

		This function renders a textured quad by stretching a texture across a rectangular area defined by screen coordinates. The quad is defined by four corners with associated texture coordinates.
	   The z parameter controls the depth of the quad in the rendering space. It internally calls the vector-based DrawStretchPic function with computed corner positions.

		\param x The x-coordinate of the top-left corner of the quad
		\param y The y-coordinate of the top-left corner of the quad
		\param w The width of the quad
		\param h The height of the quad
		\param s1 The starting s texture coordinate for the top-left corner
		\param t1 The starting t texture coordinate for the top-left corner
		\param s2 The ending s texture coordinate for the bottom-right corner
		\param t2 The ending t texture coordinate for the bottom-right corner
		\param material Pointer to the material to be used for rendering the quad
		\param z The z-coordinate depth value for the quad, default is 0.0f
	*/
	virtual void				  DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, float z = 0.0f );

	/*!
		\brief Draws a stretched textured quad using the specified vertices and material

		This function renders a textured quad by defining four corner vertices with their respective texture coordinates and colors. The quad is drawn using the provided material and is positioned in
	   3D space according to the vertex coordinates. The function handles initialization checks and validates that the material is not null before proceeding with rendering. The z-coordinate controls
	   the depth of the quad in the scene. The function uses a local array of draw vertices to store the quad data before writing it to the graphics buffer through the AllocTris and WriteDrawVerts16
	   functions.

		\param topLeft Position and texture coordinates for the top-left corner of the quad
		\param topRight Position and texture coordinates for the top-right corner of the quad
		\param bottomRight Position and texture coordinates for the bottom-right corner of the quad
		\param bottomLeft Position and texture coordinates for the bottom-left corner of the quad
		\param material The material to apply to the quad
		\param z The depth coordinate for all vertices of the quad
	*/
	virtual void				  DrawStretchPic( const idVec4& topLeft, const idVec4& topRight, const idVec4& bottomRight, const idVec4& bottomLeft, const idMaterial* material, float z = 0.0f );

	/*!
		\brief Draws a textured triangle using three points and their corresponding texture coordinates with the specified material

		This function renders a triangle on the screen using the provided vertex positions and texture coordinates. It allocates vertex data from a GUI model, sets up the vertex information including
	   positions, texture coordinates, and color data, then writes the vertices to the GPU buffer. The function performs initialization checks and validates that the material is not null before
	   proceeding with the rendering operation. The triangle is defined by three points in 2D space and their corresponding texture coordinates, using the specified material for rendering.

		\param p1 First vertex position in 2D space
		\param p2 Second vertex position in 2D space
		\param p3 Third vertex position in 2D space
		\param t1 Texture coordinate for first vertex
		\param t2 Texture coordinate for second vertex
		\param t3 Texture coordinate for third vertex
		\param material Material to be used for rendering the triangle
	*/
	virtual void				  DrawStretchTri( const idVec2& p1, const idVec2& p2, const idVec2& p3, const idVec2& t1, const idVec2& t2, const idVec2& t3, const idMaterial* material );

	/*!
		\brief Allocates and initializes vertex data for rendering triangles with specified indexes and material

		This function allocates vertex data for rendering triangle primitives using the provided vertex count, index data, material, and stereo depth type. It serves as a wrapper that delegates the
	   actual allocation to the GUI model's AllocTris method. The function is used for rendering graphical elements like stretch pics and triangles by preparing the necessary vertex data structure for
	   OpenGL rendering.

		\param numVerts Number of vertices to allocate
		\param indexes Pointer to the triangle index array defining vertex relationships
		\param numIndexes Number of index values in the indexes array
		\param material Pointer to the material to be used for rendering the triangles
		\param stereoType Type of stereo depth to use for rendering
		\return Pointer to the allocated idDrawVert array containing the vertex data for rendering, or NULL if allocation fails
	*/
	virtual idDrawVert*			  AllocTris( int numVerts, const triIndex_t* indexes, int numIndexes, const idMaterial* material, const stereoDepthType_t stereoType = STEREO_DEPTH_TYPE_NONE );

	//! Draws a small character at the specified screen coordinates using a character set texture
	virtual void				  DrawSmallChar( int x, int y, int ch );

	/*!
		\brief Draws a small string at the specified screen coordinates using the provided color and optional color overrides.

		This function renders a string using small character glyphs at the given screen coordinates. The text color is initially set using the setColor parameter, but can be overridden by color escape
	   sequences within the string if forceColor is false. The function handles color changes by parsing color escape sequences and updating the rendering color accordingly. The coordinates are based
	   on a 640 by 480 virtual resolution.

		\param x The x-coordinate of the starting position for the string
		\param y The y-coordinate of the starting position for the string
		\param string The null-terminated string to render
		\param setColor The base color to use for the text
		\param forceColor If true, prevents color overrides from the string; if false, allows color escape sequences in the string to change the text color
	*/
	virtual void				  DrawSmallStringExt( int x, int y, const char* string, const idVec4& setColor, bool forceColor );

	//! Draws a large character at the specified screen position using a character set texture.
	virtual void				  DrawBigChar( int x, int y, int ch );

	/*!
		\brief Renders a big character string at the specified coordinates with optional color formatting

		This function draws a string using large characters at the given screen position. It supports color formatting through the setColor parameter and can optionally force color changes regardless
	   of color codes in the string. The coordinates are interpreted relative to a 640 by 480 virtual resolution. The function processes each character in the string, handling color change codes and
	   rendering each character with the appropriate color settings.

		\param x Horizontal screen coordinate for the start of the string
		\param y Vertical screen coordinate for the start of the string
		\param string The text string to be rendered using big characters
		\param setColor Default color to be used for rendering the string
		\param forceColor If true, ignores color codes in the string and uses setColor for all characters
	*/
	virtual void				  DrawBigStringExt( int x, int y, const char* string, const idVec4& setColor, bool forceColor );

	//! Renders ImGui draw lists using the provided draw data
	virtual void				  ImGui_RenderDrawLists( ImDrawData* draw_data );

	//! Draws CRT post-processing effects to the screen
	virtual void				  DrawCRTPostFX();

	/*!
		\brief Closes the current command buffers and prepares new ones for parallel rendering.

		This function finalizes the current rendering command buffers and returns the head of the command list that was just closed off. It is designed to work in parallel with the rendering of the
	   closed-off command buffers by RenderCommandBuffers(). The function synchronizes with the GPU and waits for the previous frame's rendering to complete before proceeding. It also collects timing
	   information and performance counters from the previous frame. For non-SMP rendering, this operation can be split into two parts using SwapCommandBuffers_FinishRendering and
	   SwapCommandBuffers_FinishCommandBuffers.

		\param frontEndMicroSec Pointer to store frontend rendering time
		\param backEndMicroSec Pointer to store backend rendering time
		\param mocMicroSec Pointer to store moc rendering time
		\param gpuMicroSec Pointer to store GPU rendering time
		\param bc Pointer to backend counters structure
		\param pc Pointer to performance counters structure
		\return Pointer to the head of the closed command buffer list ready for rendering
	*/
	virtual const emptyCommand_t* SwapCommandBuffers( uint64* frontEndMicroSec, uint64* backEndMicroSec, uint64* mocMicroSec, uint64* gpuMicroSec, backEndCounters_t* bc, performanceCounters_t* pc );

	/*!
		\brief Finalizes rendering by waiting for GPU completion, collects timing statistics, and prepares for the next frame.

		This function handles the final steps of the rendering pipeline after command buffers have been swapped. It ensures GPU synchronization by blocking until the swap buffer operation completes,
	   then collects performance metrics such as front-end and back-end execution times, GPU time, and various counters. The collected data is stored in the provided output parameters. It also manages
	   frame-specific cleanup tasks including printing performance statistics, checking for dynamic configuration changes, and resizing HDR framebuffers. The function is designed to work with both
	   single and multi-threaded rendering environments and is part of a two-phase swap buffer operation.

		\param frontEndMicroSec Output pointer for front-end CPU execution time in microseconds
		\param backEndMicroSec Output pointer for back-end CPU execution time in microseconds
		\param mocMicroSec Output pointer for moc execution time in microseconds
		\param gpuMicroSec Output pointer for GPU execution time in microseconds
		\param bc Output pointer for backend counters structure
		\param pc Output pointer for performance counters structure
	*/
	virtual void SwapCommandBuffers_FinishRendering( uint64* frontEndMicroSec, uint64* backEndMicroSec, uint64* mocMicroSec, uint64* gpuMicroSec, backEndCounters_t* bc, performanceCounters_t* pc );

	//! Finalizes command buffers for rendering and returns the head of the closed command list.
	virtual const emptyCommand_t* SwapCommandBuffers_FinishCommandBuffers();

	//! Renders a list of command buffers if a draw view command is present, skipping rendering if r_skipBackEnd is enabled.
	virtual void				  RenderCommandBuffers( const emptyCommand_t* commandBuffers );

	/*!
		\brief Captures a screenshot of the current rendered frame or a specific view, with optional cropping and file output.

		This function handles the process of taking a screenshot either of the entire rendered scene or a specific view defined by the renderView_t structure. When a renderView_t is provided, it uses
	   that view to render the scene without HUD or other effects, otherwise it renders the current frame using common drawing functions. The screenshot is saved to the specified file name.

		\param width The width of the screenshot to be taken
		\param height The height of the screenshot to be taken
		\param fileName The name of the file to save the screenshot to
		\param ref Pointer to a renderView_t structure defining a specific view to capture, or NULL to capture the current frame
	*/
	virtual void				  TakeScreenshot( int width, int height, const char* fileName, renderView_t* ref );

	//! Returns true if the render system is currently taking a screenshot.
	virtual bool				  IsTakingScreenshot() { return takingScreenshot; }

	//! Returns a buffer containing the captured rendered image data
	virtual byte*				  CaptureRenderToBuffer( int width, int height, renderView_t* ref );

	//! Sets the rendering crop size to the specified width and height.
	virtual void				  CropRenderSize( int width, int height );

	/*!
		\brief Sets the rendering crop region to a specified rectangle either relative to the top-left corner or relative to the current crop region.

		This function configures the rendering system to output to a cropped portion of the screen defined by the provided coordinates and dimensions. The cropping can be specified in two ways: either
	   using absolute coordinates relative to the top-left corner of the screen if topLeftAncor is true, or relative to the current crop region if topLeftAncor is false. The function ensures that the
	   specified crop region is valid and throws an error if the width or height is less than 1. It also clears any existing GUI drawing before applying the new crop.

		\param x The x-coordinate of the crop region
		\param y The y-coordinate of the crop region
		\param width The width of the crop region
		\param height The height of the crop region
		\param topLeftAncor Flag indicating whether the crop is relative to the top-left corner (true) or relative to the current crop (false)
		\throws Error if the specified width or height is less than 1.
	*/
	virtual void				  CropRenderSize( int x, int y, int width, int height, bool topLeftAncor );

	//! Captures the current render output to a specified image file
	virtual void				  CaptureRenderToImage( const char* imageName, bool clearColorAfterCopy = false );

	//! Removes the current render crop and restores the full screen rendering area.
	virtual void				  UnCrop();

	/*!
		\brief Uploads image data to a previously loaded image resource by its name

		This function takes the name of an existing image resource and uploads new pixel data to it. The image must already be loaded in the global images manager. The function uses the command list
	   to queue the upload operation and then executes it on the device. The texture filtering, mipmapping, and repeat settings of the image remain unchanged during the upload process.

		\param imageName Name of the existing image resource to upload data to
		\param data Pointer to the raw pixel data to upload
		\param width Width of the pixel data in pixels
		\param height Height of the pixel data in pixels
		\return True if the image was successfully found and the data was uploaded, false otherwise
	*/
	virtual bool				  UploadImage( const char* imageName, const byte* data, int width, int height );

	//! Prints performance counters for render system debugging
	void						  PrintPerformanceCounters();

	//! Marks the render system as initialized.
	void						  SetInitialized() { bInitialized = true; }

	//! Marks swap buffers as invalid to be omitted in the next frame.
	void						  InvalidateSwapBuffers() { omitSwapBuffers = true; }

	//! Enables buffer swapping for rendering.
	void						  SetReadyToPresent() { omitSwapBuffers = false; }

public:
	//! Initializes the render system local instance.
	idRenderSystemLocal();
	~idRenderSystemLocal();

	//! Clears all rendering system state and resets internal data structures
	void Clear();

	//! Returns the current cropped pixel coordinates in the provided viewport rectangle.
	void GetCroppedViewport( idScreenRect* viewport );

	int	 GetFrameCount() const { return frameCount; };

	//! Handles frame-specific rendering operations including surface tracing and information display.
	void OnFrame();

public:
	// renderer globals

	nvrhi::CommandListHandle		   commandList;

	bool							   registered; // cleared at shutdown, set at InitOpenGL

	bool							   takingScreenshot;
	bool							   takingEnvprobe;

	int								   frameCount; // incremented every frame
	int								   viewCount;  // incremented every view (twice a scene if subviewed)
	// and every R_MarkFragments call

	float							   frameShaderTime; // shader time for all non-world 2D rendering

	idVec4							   ambientLightVector; // used for "ambient bump mapping"

	idList<idRenderWorldLocal*>		   worlds;

	idRenderWorldLocal*				   primaryWorld;
	renderView_t					   primaryRenderView;
	viewDef_t*						   primaryView;
	// many console commands need to know which world they should operate on

	const idMaterial*				   whiteMaterial;
	const idMaterial*				   charSetMaterial;
	const idMaterial*				   imgGuiMaterial; // RB
	const idMaterial*				   defaultPointLight;
	const idMaterial*				   defaultProjectedLight;
	const idMaterial*				   defaultMaterial;
	const idDeclSkin*				   vrSkin; // Leyland VR
	idImage*						   testImage;
	idCinematic*					   testVideo;
	int								   testVideoStartTime;

	idImage*						   ambientCubeImage; // hack for testing dependent ambient lighting

	viewDef_t*						   viewDef;

	performanceCounters_t			   pc; // performance counters

	viewEntity_t					   identitySpace; // can use if we don't know viewDef->worldSpace is valid

	idScreenRect					   renderCrops[MAX_RENDER_CROPS];
	int								   currentRenderCrop;

	// GUI drawing variables for surface creation
	int								   guiRecursionLevel; // to prevent infinite overruns
	uint32							   currentColorNativeBytesOrder;
	uint64							   currentGLState;
	enum stereoDepthType_t			   currentStereoDepth; // Leyland VR
	class idGuiModel*				   guiModel;

	idList<idFont*, TAG_FONT>		   fonts;

	unsigned short					   gammaTable[256]; // brightness / gamma modify this

	idMat3							   cubeAxis[6]; // RB

	srfTriangles_t*					   unitSquareTriangles;
	srfTriangles_t*					   zeroOneCubeTriangles;
	srfTriangles_t*					   zeroOneSphereTriangles;
	srfTriangles_t*					   testImageTriangles;

	// these are allocated at buffer swap time, but
	// the back end should only use the ones in the backEnd stucture,
	// which are copied over from the frame that was just swapped.
	drawSurf_t						   unitSquareSurface_;
	drawSurf_t						   zeroOneCubeSurface_;
	drawSurf_t						   zeroOneSphereSurface_;
	drawSurf_t						   testImageSurface_;

	idParallelJobList*				   frontEndJobList;

	// RB irradiance and GGX background jobs
	idParallelJobList*				   envprobeJobList;
	idList<calcEnvprobeParms_t*>	   envprobeJobs;
	idList<calcLightGridPointParms_t*> lightGridJobs;

#if defined( USE_INTRINSICS_SSE )

	#if MOC_MULTITHREADED
	CullingThreadpool* maskedOcclusionThreaded;
	#endif
	MaskedOcclusionCulling* maskedOcclusionCulling;
	idVec4					maskedUnitCubeVerts[8];
	idVec4					maskedZeroOneCubeVerts[8];
	unsigned int			maskedZeroOneCubeIndexes[36];
#endif

private:
	bool bInitialized;
	bool omitSwapBuffers;
};

extern idRenderSystemLocal tr;
extern idRenderBackend	   backEnd;
extern glconfig_t		   glConfig; // outside of TR since it shouldn't be cleared during ref re-init

//
// cvars
//
extern idCVar			   r_windowX;
extern idCVar			   r_windowY;
extern idCVar			   r_windowWidth;
extern idCVar			   r_windowHeight;

extern idCVar			   r_debugContext; // enable various levels of context debug
extern idCVar			   r_useValidationLayers;
extern idCVar			   r_vidMode;		 // video mode number
extern idCVar			   r_displayRefresh; // optional display refresh rate option for vid mode
extern idCVar			   r_fullscreen;	 // 0 = windowed, 1 = full screen
extern idCVar			   r_antiAliasing;	 // anti aliasing mode, SMAA, TXAA, MSAA etc.

extern idCVar			   r_znear; // near Z clip plane

extern idCVar			   r_swapInterval;	 // changes wglSwapIntarval
extern idCVar			   r_offsetFactor;	 // polygon offset parameter
extern idCVar			   r_offsetUnits;	 // polygon offset parameter
extern idCVar			   r_singleTriangle; // only draw a single triangle per primitive
extern idCVar			   r_clear;			 // force screen clear every frame
extern idCVar			   r_subviewOnly;	 // 1 = don't render main view, allowing subviews to be debugged
extern idCVar			   r_lightScale;	 // all light intensities are multiplied by this, which is normally 3
extern idCVar			   r_flareSize;		 // scale the flare deforms from the material def

extern idCVar			   r_gamma;		 // changes gamma tables
extern idCVar			   r_brightness; // changes gamma tables

extern idCVar			   r_checkBounds;			  // compare all surface bounds with precalculated ones
extern idCVar			   r_maxAnisotropicFiltering; // texture filtering parameter
extern idCVar			   r_useTrilinearFiltering;	  // Extra quality filtering
extern idCVar			   r_lodBias;				  // lod bias

extern idCVar			   r_useLightPortalFlow;	  // 1 = do a more precise area reference determination
extern idCVar			   r_useShadowSurfaceScissor; // 1 = scissor shadows by the scissor rect of the interaction surfaces
extern idCVar			   r_useConstantMaterials;	  // 1 = use pre-calculated material registers if possible
extern idCVar			   r_useNodeCommonChildren;	  // stop pushing reference bounds early when possible
extern idCVar			   r_useSilRemap;			  // 1 = consider verts with the same XYZ, but different ST the same for shadows
extern idCVar			   r_useLightPortalCulling;	  // 0 = none, 1 = box, 2 = exact clip of polyhedron faces, 3 MVP to plane culling
extern idCVar			   r_useLightAreaCulling;	  // 0 = off, 1 = on
extern idCVar			   r_useLightScissors;		  // 1 = use custom scissor rectangle for each light
extern idCVar			   r_useEntityPortalCulling;  // 0 = none, 1 = box
extern idCVar			   r_useCachedDynamicModels;  // 1 = cache snapshots of dynamic models
extern idCVar			   r_useScissor;			  // 1 = scissor clip as portals and lights are processed
extern idCVar			   r_usePortals;			  // 1 = use portals to perform area culling, otherwise draw everything
extern idCVar			   r_useStateCaching;		  // avoid redundant state changes in GL_*() calls
extern idCVar			   r_useEntityCallbacks;	  // if 0, issue the callback immediately at update time, rather than defering
extern idCVar			   r_lightAllBackFaces;		  // light all the back faces, even when they would be shadowed
extern idCVar			   r_useLightDepthBounds;	  // use depth bounds test on lights to reduce both shadow and interaction fill
extern idCVar			   r_useShadowDepthBounds;	  // use depth bounds test on individual shadows to reduce shadow fill
// RB begin
extern idCVar			   r_useShadowAtlas; // temporary for perf testing: pack shadow maps into big atlas
// RB end

extern idCVar			   r_skipStaticInteractions;  // skip interactions created at level load
extern idCVar			   r_skipDynamicInteractions; // skip interactions created after level load
extern idCVar			   r_skipPostProcess;		  // skip all post-process renderings
extern idCVar			   r_skipBloom;				  // Admer: skip bloom
extern idCVar			   r_skipSuppress;			  // ignore the per-view suppressions
extern idCVar			   r_skipInteractions;		  // skip all light/surface interaction drawing
extern idCVar			   r_skipFrontEnd;			  // bypasses all front end work, but 2D gui rendering still draws
extern idCVar			   r_skipBackEnd;			  // don't draw anything
extern idCVar			   r_skipCopyTexture;		  // do all rendering, but don't actually copyTexSubImage2D
extern idCVar			   r_skipRender;			  // skip 3D rendering, but pass 2D
extern idCVar			   r_skipTranslucent;		  // skip the translucent interaction rendering
extern idCVar			   r_skipAmbient;			  // bypasses all non-interaction drawing
extern idCVar			   r_skipNewAmbient;		  // bypasses all vertex/fragment program ambients
extern idCVar			   r_skipBlendLights;		  // skip all blend lights
extern idCVar			   r_skipFogLights;			  // skip all fog lights
extern idCVar			   r_skipSubviews;			  // 1 = don't render any mirrors / cameras / etc
extern idCVar			   r_skipGuiShaders;		  // 1 = don't render any gui elements on surfaces
extern idCVar			   r_skipParticles;			  // 1 = don't render any particles
extern idCVar			   r_skipUpdates;			  // 1 = don't accept any entity or light updates, making everything static
extern idCVar			   r_skipDeforms;			  // leave all deform materials in their original state
extern idCVar			   r_skipDynamicTextures;	  // don't dynamically create textures
extern idCVar			   r_skipBump;				  // uses a flat surface instead of the bump map
extern idCVar			   r_skipSpecular;			  // use black for specular
extern idCVar			   r_skipDiffuse;			  // use black for diffuse
extern idCVar			   r_skipDecals;			  // skip decal surfaces
extern idCVar			   r_skipOverlays;			  // skip overlay surfaces
extern idCVar			   r_skipShadows;			  // disable shadows

extern idCVar			   r_screenFraction;		 // for testing fill rate, the resolution of the entire screen can be changed
extern idCVar			   r_showUnsmoothedTangents; // highlight geometry rendered with unsmoothed tangents
extern idCVar			   r_showSilhouette;		 // highlight edges that are casting shadow planes
extern idCVar			   r_showVertexColor;		 // draws all triangles with the solid vertex color
extern idCVar			   r_showUpdates;			 // report entity and light updates and ref counts
extern idCVar			   r_showDynamic;			 // report stats on dynamic surface generation
extern idCVar			   r_showIntensity;			 // draw the screen colors based on intensity, red = 0, green = 128, blue = 255
extern idCVar			   r_showTrace;				 // show the intersection of an eye trace with the world
extern idCVar			   r_showDepth;				 // display the contents of the depth buffer and the depth range
extern idCVar			   r_showTris;				 // enables wireframe rendering of the world
extern idCVar			   r_showSurfaceInfo;		 // show surface material name under crosshair
extern idCVar			   r_showNormals;			 // draws wireframe normals
extern idCVar			   r_showEdges;				 // draw the sil edges
extern idCVar			   r_showViewEntitys;		 // displays the bounding boxes of all view models and optionally the index
extern idCVar			   r_showTexturePolarity;	 // shade triangles by texture area polarity
extern idCVar			   r_showTangentSpace;		 // shade triangles by tangent space
extern idCVar			   r_showDominantTri;		 // draw lines from vertexes to center of dominant triangles
extern idCVar			   r_showTextureVectors;	 // draw each triangles texture (tangent) vectors
extern idCVar			   r_showLights;			 // 1 = print light info, 2 = also draw volumes
extern idCVar			   r_showLightCount;		 // colors surfaces based on light count
extern idCVar			   r_showShadows;			 // visualize the stencil shadow volumes
extern idCVar			   r_showLightScissors;		 // show light scissor rectangles
extern idCVar			   r_showMemory;			 // print frame memory utilization
extern idCVar			   r_showCull;				 // report sphere and box culling stats
extern idCVar			   r_showAddModel;			 // report stats from tr_addModel
extern idCVar			   r_showSurfaces;			 // report surface/light/shadow counts
extern idCVar			   r_showPrimitives;		 // report vertex/index/draw counts
extern idCVar			   r_showPortals;			 // draw portal outlines in color based on passed / not passed
extern idCVar			   r_showSkel;				 // draw the skeleton when model animates
extern idCVar			   r_showOverDraw;			 // show overdraw
// RB begin
extern idCVar			   r_showShadowMaps;
extern idCVar			   r_showShadowMapLODs;
// RB end
extern idCVar			   r_jointNameScale;  // size of joint names when r_showskel is set to 1
extern idCVar			   r_jointNameOffset; // offset of joint names when r_showskel is set to 1

extern idCVar			   r_testGamma;		// draw a grid pattern to test gamma levels
extern idCVar			   r_testGammaBias; // draw a grid pattern to test gamma levels

extern idCVar			   r_singleLight;	 // suppress all but one light
extern idCVar			   r_singleEntity;	 // suppress all but one entity
extern idCVar			   r_singleEnvprobe; // suppress all but one envprobe
extern idCVar			   r_singleArea;	 // only draw the portal area the view is actually in
extern idCVar			   r_singleSurface;	 // suppress all but one surface on each entity

extern idCVar			   r_orderIndexes; // perform index reorganization to optimize vertex use

extern idCVar			   r_debugLineDepthTest; // perform depth test on debug lines
extern idCVar			   r_debugLineWidth;	 // width of debug lines
extern idCVar			   r_debugArrowStep;	 // step size of arrow cone line rotation in degrees
extern idCVar			   r_debugPolygonFilled;

extern idCVar			   r_materialOverride; // override all materials

extern idCVar			   r_debugRenderToTexture;

// RB begin
extern idCVar			   r_useGPUSkinning;

extern idCVar			   r_shadowMapAtlasSize;
extern idCVar			   r_shadowMapFrustumFOV;
extern idCVar			   r_shadowMapSingleSide;
extern idCVar			   r_shadowMapImageSize;
extern idCVar			   r_shadowMapJitterScale;
// extern idCVar r_shadowMapBiasScale;
extern idCVar			   r_shadowMapRandomizeJitter;
extern idCVar			   r_shadowMapSamples;
extern idCVar			   r_shadowMapSplits;
extern idCVar			   r_shadowMapSplitWeight;
extern idCVar			   r_shadowMapLodScale;
extern idCVar			   r_shadowMapLodBias;
extern idCVar			   r_shadowMapPolygonFactor;
extern idCVar			   r_dxShadowMapPolygonOffset;
extern idCVar			   r_vkShadowMapPolygonOffset;
extern idCVar			   r_shadowMapOccluderFacing;
extern idCVar			   r_shadowMapRegularDepthBiasScale;
extern idCVar			   r_shadowMapSunDepthBiasScale;

extern idCVar			   r_hdrAutoExposure;
extern idCVar			   r_hdrAdaptionRate;
extern idCVar			   r_hdrMinLuminance;
extern idCVar			   r_hdrMaxLuminance;
extern idCVar			   r_hdrKey;
extern idCVar			   r_hdrContrastDynamicThreshold;
extern idCVar			   r_hdrContrastStaticThreshold;
extern idCVar			   r_hdrContrastOffset;
extern idCVar			   r_hdrGlarePasses;
extern idCVar			   r_hdrDebug;

extern idCVar			   r_ldrContrastThreshold;
extern idCVar			   r_ldrContrastOffset;

extern idCVar			   r_forceAmbient;

extern idCVar			   r_useSSAO;
extern idCVar			   r_ssaoDebug;
extern idCVar			   r_ssaoFiltering;
extern idCVar			   r_useHierarchicalDepthBuffer;

extern idCVar			   r_pbrDebug;
extern idCVar			   r_showViewEnvprobes;
extern idCVar			   r_showLightGrid; // show Quake 3 style light grid points

extern idCVar			   r_useLightGrid;

extern idCVar			   r_exposure;

extern idCVar			   r_useSSR;
extern idCVar			   r_ssrJitter;
extern idCVar			   r_ssrMaxDistance;
extern idCVar			   r_ssrMaxSteps;
extern idCVar			   r_ssrStride;
extern idCVar			   r_ssrZThickness;

extern idCVar			   r_useTemporalAA;
extern idCVar			   r_taaJitter;
extern idCVar			   r_taaEnableHistoryClamping;
extern idCVar			   r_taaClampingFactor;
extern idCVar			   r_taaNewFrameWeight;
extern idCVar			   r_taaMaxRadiance;
extern idCVar			   r_taaMotionVectors;

extern idCVar			   r_useFilmicPostFX;
extern idCVar			   r_useCRTPostFX;
extern idCVar			   r_crtCurvature;
extern idCVar			   r_crtVignette;

extern idCVar			   r_useMaskedOcclusionCulling;

extern idCVar			   r_retroDitherScale;

extern idCVar			   r_renderMode;
extern idCVar			   image_pixelLook;

extern idCVar			   r_psxVertexJitter;
extern idCVar			   r_psxAffineTextures;

//! Returns true if the pixelated look rendering mode is enabled.
bool					   R_UsePixelatedLook();

//! Determines whether temporal antialiasing should be used based on various render settings.
bool					   R_UseTemporalAA();

//! Determines whether the hierarchical depth buffer feature should be used.
bool					   R_UseHiZ();

//! Returns the number of multisamples used for antialiasing.
uint					   R_GetMSAASamples();

//! Sets the rendering mode with optional full initialization.
void					   R_SetNewMode( const bool fullInit );

//! Sets the gamma table for color mapping based on brightness and gamma settings.
void					   R_SetColorMappings();

//! Takes a screenshot of the current rendered view with optional custom filename or resolution.
void					   R_ScreenShot_f( const idCmdArgs& args );

/*!
	\brief Reads pixel data from a texture and saves it as a PNG image file

	This function retrieves pixel data from a specified texture, converts it to RGBA format if needed, and saves the result as a PNG file. It handles texture format conversion by creating a temporary
   render target texture when the input texture is not in RGBA8_UNORM or SRGBA8_UNORM format. The function manages command lists for texture operations and ensures proper resource state handling. The
   saved image is written to the specified file path using the appropriate base path based on the platform.

	\param device The device used for GPU operations
	\param pPasses Common render passes object for texture blitting operations
	\param texture The source texture to read pixels from
	\param textureState The current resource state of the texture
	\param fullname Full path and filename for the output PNG file
	\return True if the pixel data was successfully read and saved as a PNG file, false otherwise
*/
bool					   R_ReadPixelsRGB8( nvrhi::IDevice* device, CommonRenderPasses* pPasses, nvrhi::ITexture* texture, nvrhi::ResourceStates textureState, const char* fullname );

/*!
	\brief Reads pixel data from a texture and converts it from RGBA16F format to RGB16F format, storing the result in a buffer.

	This function reads pixel data from a specified texture using a staging texture and transfers the data to a user-provided buffer. It performs format conversion from RGBA16F to RGB16F. The function
   also includes error correction for corrupted pixel data by resetting invalid values to black. It is typically used in graphics rendering contexts for capturing and processing high dynamic range
   (HDR) texture data.

	The function first ensures proper texture state tracking and creates temporary resources for the read operation. It then copies the texture data to a staging texture, maps it, and performs the
   conversion from RGBA16F to RGB16F. If the row pitch does not match the expected size, it reallocates and copies data to ensure correct alignment. The function also performs validation checks on the
   pixel data to detect and correct corrupted pixels based on luminance thresholds.

	\param device The graphics device used for creating temporary resources and executing GPU commands.
	\param pPasses A pointer to the common render passes used for texture blitting operations.
	\param texture The source texture from which pixel data is read.
	\param textureState The current resource state of the source texture.
	\param pic A pointer to a buffer where the converted RGB16F pixel data will be stored.
	\param picWidth The width of the pixel data to be read.
	\param picHeight The height of the pixel data to be read.
	\return True if the pixel data was successfully read and processed, false otherwise.
*/
bool R_ReadPixelsRGB16F( nvrhi::IDevice* device, CommonRenderPasses* pPasses, nvrhi::ITexture* texture, nvrhi::ResourceStates textureState, byte** pic, int picWidth, int picHeight );

/*
====================================================================

IMPLEMENTATION SPECIFIC FUNCTIONS

====================================================================
*/

//! Retrieves the list of video modes available for a specified display.
bool R_GetModeListForDisplay( const int displayNum, idList<vidMode_t>& modeList );

// Eric: If on Linux using Vulkan use the sdl_vkimp.cpp methods
// SRS - Generalized Vulkan SDL platform
#if defined( VULKAN_USE_PLATFORM_SDL )
	#include <vector>

	#define CLAMP( x, lo, hi ) ( ( x ) < ( lo ) ? ( lo ) : ( x ) > ( hi ) ? ( hi ) : ( x ) )
// Helper function for using SDL2 and Vulkan on Linux.
std::vector<const char*> get_required_extensions();

// DG: R_GetModeListForDisplay is called before GLimp_Init(), but SDL needs SDL_Init() first.
// So add PreInit for platforms that need it, others can just stub it.
void					 VKimp_PreInit();

// If the desired mode can't be set satisfactorily, false will be returned.
// If succesful, sets glConfig.nativeScreenWidth, glConfig.nativeScreenHeight, and glConfig.pixelAspect
// The renderer will then reset the glimpParms to "safe mode" of 640x480
// fullscreen and try again.  If that also fails, the error will be fatal.
bool					 VKimp_Init( glimpParms_t parms );

// will set up gl up with the new parms
bool					 VKimp_SetScreenParms( glimpParms_t parms );

// Destroys the rendering context, closes the window, resets the resolution,
// and resets the gamma ramps.  SRS - Optionally shuts down SDL for quit.
void					 VKimp_Shutdown( bool shutdownSDL );

// Sets the hardware gamma ramps for gamma and brightness adjustment.
// These are now taken as 16 bit values, so we can take full advantage
// of dacs with >8 bits of precision
void					 VKimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] );
#else

//! Performs pre-initialization tasks for OpenGL on platforms that require them.
void GLimp_PreInit();

//! Initializes the graphics library with the specified parameters and sets up the rendering context.
bool GLimp_Init( glimpParms_t parms );

//! Sets the graphics screen parameters for the OpenGL implementation.
bool GLimp_SetScreenParms( glimpParms_t parms );

//! Destroys the rendering context, closes the window, resets the resolution, and resets the gamma ramps.
void GLimp_Shutdown();

//! Sets the hardware gamma ramps for gamma and brightness adjustment using 16-bit values.
void GLimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] );

#endif

//! Updates the render entity's model matrix and bounds based on its parameters and local reference bounds.
void			R_DeriveEntityData( idRenderEntityLocal* def );

//! Creates entity references for the given render entity by computing bounds and deriving entity data
void			R_CreateEntityRefs( idRenderEntityLocal* def );

//! Frees derived data associated with a render entity definition
void			R_FreeEntityDefDerivedData( idRenderEntityLocal* def, bool keepDecals, bool keepCachedDynamicModel );
void			R_FreeEntityDefCachedDynamicModel( idRenderEntityLocal* def );

//! Frees the decals associated with a render entity definition.
void			R_FreeEntityDefDecals( idRenderEntityLocal* def );

//! Frees the overlay data associated with a render entity.
void			R_FreeEntityDefOverlay( idRenderEntityLocal* def );

//! Removes faded decals from the specified render entity at the given time.
void			R_FreeEntityDefFadedDecals( idRenderEntityLocal* def, int time );

//! Initializes and derives light data for rendering from light parameters.
void			R_DeriveLightData( idRenderLightLocal* light );

//! Computes the frustum planes for a given light volume.
void			R_RenderLightFrustum( const renderLight_t& renderLight, idPlane lightFrustum[6] );

//! Creates a triangular surface representation of a polytope defined by a set of planes
srfTriangles_t* R_PolytopeSurface( int numPlanes, const idPlane* planes, idWinding** windings );

//! Creates environment probe references by deriving probe data and pushing it into the BSP tree.
void			R_CreateEnvprobeRefs( RenderEnvprobeLocal* probe );

//! Frees all area references associated with the given environment probe.
void			R_FreeEnvprobeDefDerivedData( RenderEnvprobeLocal* probe );

//! Creates light references for the given render light by deriving light data and determining area visibility.
void			R_CreateLightRefs( idRenderLightLocal* light );

//! Frees all references and interactions associated with a render light definition
void			R_FreeLightDefDerivedData( idRenderLightLocal* light );

//! Frees derived data for all render entities, lights, and environment probes across all render worlds.
void			R_FreeDerivedData();

//! Recreates world references for all render worlds.
void			R_ReCreateWorldReferences();

//! Checks if any entity definitions in the world are using the specified render model and frees their derived data if found.
void			R_CheckForEntityDefsUsingModel( idRenderModel* model );

//! Modifies the shader parameters of all lights in the primary world by multiplying them with the specified RGB modulation values.
void			R_ModulateLights_f( const idCmdArgs& args );

//! Sets up and returns the view entity for a render entity.
viewEntity_t*	R_SetEntityDefViewEntity( idRenderEntityLocal* def );

//! Creates or retrieves a viewLight for the given light definition and adds it to the view light chain.
viewLight_t*	R_SetLightDefViewLight( idRenderLightLocal* def );

/*!
	\brief Samples a cube map with HDR data at the specified direction and returns the result along with texture coordinates.

	This function determines which face of the cube map to sample based on the input direction vector. It calculates texture coordinates for the chosen face and extracts pixel data from the
   appropriate buffer. The function converts the pixel data from R11G11B10_FLOAT format to standard 3-component floating point values. The texture coordinates are returned in the u and v parameters,
   while the sampled color values are stored in the result array.

	\param dir The direction vector for sampling the cube map
	\param size The size of the cube map face in pixels
	\param buffers Array of pointers to the 6 cube map faces
	\param result Output array for the sampled color values
	\param u Output texture coordinate u
	\param v Output texture coordinate v
*/
void			R_SampleCubeMapHDR( const idVec3& dir, int size, byte* buffers[6], float result[3], float& u, float& v );

/*!
	\brief Samples a cube map with HDR16F format using the given direction and size to compute texture coordinates and retrieve color values.

	This function determines the appropriate cube map face based on the input direction vector, calculates texture coordinates within that face, and retrieves the corresponding color values from the
   provided buffer array. The function handles all six cube map faces and performs proper clamping of coordinates to ensure valid indexing. The retrieved color values are converted from half-precision
   floating-point format to single-precision floating-point format.

	\param dir The direction vector used to determine which cube map face to sample
	\param size The size of the cube map face in pixels
	\param buffers Array of pointers to the cube map face buffers, where each buffer contains 16-bit half-floating-point color values
	\param result Output array where the sampled color values will be stored as single-precision floats
	\param u Output parameter containing the computed U texture coordinate
	\param v Output parameter containing the computed V texture coordinate
*/
void			R_SampleCubeMapHDR16F( const idVec3& dir, int size, halfFloat_t* buffers[6], float result[3], float& u, float& v );

//! Computes normalized octahedral coordinates for a given pixel position within a probe boundary
idVec2			NormalizedOctCoord( int x, int y, const int probeSideLength );

//! Initializes frame data for rendering
void			R_InitFrameData();

//! Initializes the renderer frame data structures.
void			R_ShutdownFrameData();

//! Toggles to the next rendering frame and resets memory allocation tracking.
void			R_ToggleSmpFrame();

//! Allocates frame-ordered memory with cache-line clearing for performance
void*			R_FrameAlloc( int bytes, frameAllocType_t type = FRAME_ALLOC_UNKNOWN );

//! Allocates cleared memory from the frame allocator for rendering purposes.
void*			R_ClearedFrameAlloc( int bytes, frameAllocType_t type = FRAME_ALLOC_UNKNOWN );

//! Allocates a block of memory with error checking and optional memory tag
void*			R_StaticAlloc( int bytes, const memTag_t tag = TAG_RENDER_STATIC );

//! Allocates and clears a block of memory of the specified size
void*			R_ClearedStaticAlloc( int bytes );

//! Frees memory that was allocated with R_StaticAlloc.
void			R_StaticFree( void* data );

//! Renders a view with the specified view definition parameters
void			R_RenderView( viewDef_t* parms );

//! Renders post-process effects for the specified view definition.
void			R_RenderPostProcess( viewDef_t* parms );

/*!
	\brief Computes the bounding box of a shadow cast by a model based on the model bounds, light bounds, and light origin.

	This function calculates the shadow bounds by comparing the model bounds with the light bounds relative to the light origin. For each axis, it determines the appropriate boundary point by
   selecting between the model's boundary and the light's boundary based on the relative position of the light origin. The function uses low-level floating-point selection operations to efficiently
   compute the result.

	\param modelBounds The bounding box of the model that casts the shadow
	\param lightBounds The bounding box of the light source
	\param lightOrigin The position of the light source
	\param shadowBounds The output bounding box of the computed shadow
*/
void			R_ShadowBounds( const idBounds& modelBounds, const idBounds& lightBounds, const idVec3& lightOrigin, idBounds& shadowBounds );

//! Determines if a model's bounds are culled by a light's frustum.
ID_INLINE bool	R_CullModelBoundsToLight( const idRenderLightLocal* light, const idBounds& localBounds, const idRenderMatrix& modelRenderMatrix )
{
	idRenderMatrix modelLightProject;
	idRenderMatrix::Multiply( light->baseLightProject, modelRenderMatrix, modelLightProject );
	return idRenderMatrix::CullBoundsToMVP( modelLightProject, localBounds, true );
}

//! Adds lights to the current view for rendering
void			R_AddLights();

//! Optimizes the order of visible lights in the view list based on screen area to reduce GPU pipeline bubbles
void			R_OptimizeViewLightsList();

//! Executes a callback function for a render entity to update its parameters and returns whether an update occurred
bool			R_IssueEntityDefCallback( idRenderEntityLocal* def );

//! Returns the dynamic model for a render entity, creating it if necessary.
idRenderModel*	R_EntityDefDynamicModel( idRenderEntityLocal* def );

//! Clears the dynamic model data for a render entity
void			R_ClearEntityDefDynamicModel( idRenderEntityLocal* def );

//! Initializes shader-related data for a draw surface using the provided material and render entity.
void			R_SetupDrawSurfShader( drawSurf_t* drawSurf, const idMaterial* shader, const renderEntity_t* renderEntity );

/*!
	\brief Sets up joint cache data for a draw surface when GPU skinning is available

	This function configures the joint cache for a draw surface by checking if GPU skinning is enabled and if the geometry has joint information. If these conditions are met, it allocates or reuses
   joint data from the vertex cache. Otherwise, it clears the joint cache reference

	\param drawSurf pointer to the draw surface structure to configure
	\param tri pointer to triangle mesh data containing joint information
	\param shader pointer to the material shader being used for rendering
	\param commandList optional command list for GPU command submission
	\throws assertion failure if the model does not have inverted joints data
*/
void			R_SetupDrawSurfJoints( drawSurf_t* drawSurf, const srfTriangles_t* tri, const idMaterial* shader, nvrhi::ICommandList* commandList = nullptr );

//! Links a draw surface to a view definition for rendering.
void			R_LinkDrawSurfToView( drawSurf_t* drawSurf, viewDef_t* viewDef );

//! Adds models to the rendering system for the current view
void			R_AddModels();

//! Populates the masked occlusion buffer with rendered models for the given view definition.
void			R_FillMaskedOcclusionBufferWithModels( viewDef_t* viewDef );

//! Returns a deformed version of the input draw surface, or NULL if the material is invalid or no deformation is needed.
drawSurf_t*		R_DeformDrawSurf( drawSurf_t* drawSurf );

//! Applies a deformation to a draw surface based on the specified deformation type and returns the modified surface.
drawSurf_t*		R_DeformDrawSurf( drawSurf_t* drawSurf, deform_t deformType );

//! Computes the texture coordinate axis vectors and origin for a surface defined by triangle data.
void			R_SurfaceToTextureAxis( const srfTriangles_t* tri, idVec3& origin, idVec3 axis[3] );

//! Processes and renders GUI surfaces for in-game elements.
void			R_AddInGameGuis( const drawSurf_t* const drawSurfs[], const int numDrawSurfs );

//! Determines if a surface should be culled based on precise clipping and backface culling calculations.
bool			R_PreciseCullSurface( const drawSurf_t* drawSurf, idBounds& ndcBounds );

//! Generates subviews for draw surfaces that have subview materials
bool			R_GenerateSubViews( const drawSurf_t* const drawSurfs[], const int numDrawSurfs );

//! Allocates and returns a new static triangle surface structure.
srfTriangles_t* R_AllocStaticTriSurf();

//! Allocates memory for vertex data of a triangle surface
void			R_AllocStaticTriSurfVerts( srfTriangles_t* tri, int numVerts );

//! Allocates memory for triangle surface indexes
void			R_AllocStaticTriSurfIndexes( srfTriangles_t* tri, int numIndexes );

//! Allocates memory for silhouette indexes of a triangle surface
void			R_AllocStaticTriSurfSilIndexes( srfTriangles_t* tri, int numIndexes );

//! Allocates memory for dominant triangle data for a static triangle surface.
void			R_AllocStaticTriSurfDominantTris( srfTriangles_t* tri, int numVerts );

//! Allocates memory for mirrored vertex indices on a static triangle surface.
void			R_AllocStaticTriSurfMirroredVerts( srfTriangles_t* tri, int numMirroredVerts );

//! Allocates memory for duplicate vertex data for a triangle surface.
void			R_AllocStaticTriSurfDupVerts( srfTriangles_t* tri, int numDupVerts );

//! Allocates memory for motion-occlusion culling indexes for a triangle surface.
void			R_AllocStaticTriSurfMocIndexes( srfTriangles_t* tri, int numIndexes );

//! Allocates memory for motion-optimized vertices for a triangle surface
void			R_AllocStaticTriSurfMocVerts( srfTriangles_t* tri, int numVerts );

//! Creates a copy of a static triangle surface with the same vertex and index data.
srfTriangles_t* R_CopyStaticTriSurf( const srfTriangles_t* tri );

//! Resizes the vertex array of a triangle surface to the specified number of vertices
void			R_ResizeStaticTriSurfVerts( srfTriangles_t* tri, int numVerts );

//! Resizes the index buffer of a static triangle surface to the specified number of indexes.
void			R_ResizeStaticTriSurfIndexes( srfTriangles_t* tri, int numIndexes );

//! Sets the vertex data of a triangle surface to reference another triangle surface's vertex data.
void			R_ReferenceStaticTriSurfVerts( srfTriangles_t* tri, const srfTriangles_t* reference );

//! Sets the index buffer of a triangle surface to reference another triangle surface's index buffer.
void			R_ReferenceStaticTriSurfIndexes( srfTriangles_t* tri, const srfTriangles_t* reference );

//! Frees the silhouette indexes memory of a static triangle surface.
void			R_FreeStaticTriSurfSilIndexes( srfTriangles_t* tri );

//! Frees the memory allocated for a static triangle surface
void			R_FreeStaticTriSurf( srfTriangles_t* tri );

//! Frees the vertex memory of a static triangle surface.
void			R_FreeStaticTriSurfVerts( srfTriangles_t* tri );

//! Frees the static vertex caches associated with the given triangle surface.
void			R_FreeStaticTriSurfVertexCaches( srfTriangles_t* tri );

//! Calculates the total memory usage of a triangle surface structure for profiling purposes.
int				R_TriSurfMemory( const srfTriangles_t* tri );

//! Computes the bounding box for a triangle surface.
void			R_BoundTriSurf( srfTriangles_t* tri );

//! Removes duplicated triangles from a triangle surface.
void			R_RemoveDuplicatedTriangles( srfTriangles_t* tri );

//! Creates silhouette indexes for triangle surfaces by remapping vertex indices
void			R_CreateSilIndexes( srfTriangles_t* tri );

//! Initializes masked occlusion culling vertex and index data for a triangle surface.
void			R_CreateMaskedOcclusionCullingTris( srfTriangles_t* tri );

//! Removes degenerate triangles from the triangle mesh by eliminating triangles with duplicate vertex indices.
void			R_RemoveDegenerateTriangles( srfTriangles_t* tri );

//! Removes unused vertices from a triangle surface
void			R_RemoveUnusedVerts( srfTriangles_t* tri );

//! Performs range checking on triangle indexes to ensure they are valid.
void			R_RangeCheckIndexes( const srfTriangles_t* tri );

//! Computes and sets vertex normals for triangle data by calculating face normals and averaging them per vertex.
void			R_CreateVertexNormals( srfTriangles_t* tri );

/*!
	\brief Cleans up triangle data by removing duplicates, degenerates, and processing tangents and silhouettes

	This function performs various cleanup operations on triangle data including range checking, silhouette index creation, removal of degenerate triangles, and texture space testing. It handles
   tangent derivation with options for smoothed or unsmoothed tangents, and supports Mikktspace tangent calculation. The function also creates masked occlusion culling data suitable for MOC SIMD
   operations.

	\param tri Pointer to the triangle surface data to be cleaned up
	\param createNormals Flag indicating whether to create normals during tangent calculation
	\param identifySilEdges Flag indicating whether to identify silhouette edges
	\param useUnsmoothedTangents Flag indicating whether to use unsmoothed tangents for derivation
	\param useMikktspace Flag indicating whether to use Mikktspace algorithm for tangent calculation
*/
void			R_CleanupTriangles( srfTriangles_t* tri, bool createNormals, bool identifySilEdges, bool useUnsmoothedTangents, bool useMikktspace );

//! Reverses the triangle orientations by flipping vertex normals and index order.
void			R_ReverseTriangles( srfTriangles_t* tri );

//! Merges multiple triangle surfaces into a single surface
srfTriangles_t* R_MergeSurfaceList( const srfTriangles_t** surfaces, int numSurfaces );

//! Merges two triangle surfaces into a single triangle surface
srfTriangles_t* R_MergeTriangles( const srfTriangles_t* tri1, const srfTriangles_t* tri2 );

//! Computes tangents for triangle data, skipping calculation if already computed.
void			R_DeriveTangents( srfTriangles_t* tri );

//! Initializes a back-end draw surface from front-end triangle data
void			R_InitDrawSurfFromTri( drawSurf_t& ds, srfTriangles_t& tri, nvrhi::ICommandList* commandList );

//! Creates static vertex and index buffers for a triangle surface
void			R_CreateStaticBuffersForTri( srfTriangles_t& tri, nvrhi::ICommandList* commandList );

/*!
	\brief Computes the closest point on a triangle to a given point using barycentric coordinates and geometric projections.

	This function implements the closest point calculation from a point to a triangle, as described in Real-Time Collision Detection by Christer Ericson. It determines the closest point on the
   triangle surface to the specified input point by checking the point's position relative to the triangle's vertices and edges. The algorithm uses barycentric coordinates to compute the result,
   handling all cases including when the closest point lies on a vertex, edge, or within the triangle's interior. The function can be used for various applications such as environment probe
   interpolation and collision detection.

	\param point The point for which to find the closest point on the triangle
	\param vertex1 The first vertex of the triangle
	\param vertex2 The second vertex of the triangle
	\param vertex3 The third vertex of the triangle
	\return The closest point on the triangle to the specified input point, represented as an idVec3.
*/
idVec3			R_ClosestPointPointTriangle( const idVec3& point, const idVec3& vertex1, const idVec3& vertex2, const idVec3& vertex3 );

/*!
	\brief Calculates the closest point on a line segment to a given point and returns the parameter t of the closest point.

	This function computes the closest point on a line segment defined by lineStart and lineEnd to the specified point.
	It returns the closest point on the line segment and stores the parameter t which represents the position along the line segment.
	When the line segment is degenerate (start and end points are the same), t is set to 0 and the start point is returned.
	The parameter t is clamped to the range [0, 1] to ensure the result lies on the line segment rather than on the infinite line.

	\param point The point for which to find the closest point on the line segment
	\param lineStart The starting point of the line segment
	\param lineEnd The ending point of the line segment
	\param t The parameter representing the closest point's position along the line segment, clamped to [0, 1]
	\return The closest point on the line segment to the specified point.
*/
idVec3			R_ClosestPointOnLineSegment( const idVec3& point, const idVec3& lineStart, const idVec3& lineEnd, float& t );

// deformable meshes precalculate as much as possible from a base frame, then generate
// complete srfTriangles_t from just a new set of vertexes
struct deformInfo_t {
	int				  numSourceVerts;

	// numOutputVerts may be smaller if the input had duplicated or degenerate triangles
	// it will often be larger if the input had mirrored texture seams that needed
	// to be busted for proper tangent spaces
	int				  numOutputVerts;
	idDrawVert*		  verts;

	int				  numIndexes;
	triIndex_t*		  indexes;

	triIndex_t*		  silIndexes; // indexes changed to be the first vertex with same XYZ, ignoring normal and texcoords

	int				  numMirroredVerts; // this many verts at the end of the vert list are tangent mirrors
	int*			  mirroredVerts;	// tri->mirroredVerts[0] is the mirror of tri->numVerts - tri->numMirroredVerts + 0

	int				  numDupVerts; // number of duplicate vertexes
	int*			  dupVerts;	   // pairs of the number of the first vertex and the number of the duplicate vertex

	vertCacheHandle_t staticIndexCache;	  // GL_INDEX_TYPE
	vertCacheHandle_t staticAmbientCache; // idDrawVert
};

/*!
	\brief Creates deformation information for vertex data including tangent calculations and index handling

	Builds deformation information for mesh data by processing vertices and indices to generate tangents, silhouette edges, and mirrored vertex handling. The function allocates and populates a
   deformInfo_t structure that contains all necessary data for vertex deformation operations. It supports both smoothed and unsmoothed tangent calculations based on the useUnsmoothedTangents
   parameter. The function handles various mesh processing steps including index range checking, silhouette edge creation, and duplication of mirrored vertices. The output includes both original and
   processed vertex data with proper memory management and allocation.

	\param numVerts Number of vertices in the input mesh
	\param verts Pointer to the input vertex data
	\param numIndexes Number of indices in the index buffer
	\param indexes Pointer to the index data
	\param useUnsmoothedTangents Flag indicating whether to use unsmoothed tangent calculations
	\return Pointer to the allocated deformInfo_t structure containing processed mesh data
*/
deformInfo_t* R_BuildDeformInfo( int numVerts, const idDrawVert* verts, int numIndexes, const int* indexes, bool useUnsmoothedTangents );

//! Uploads static vertices to the vertex cache.
void		  R_CreateDeformStaticVertices( deformInfo_t* deform, nvrhi::ICommandList* commandList );

//! Frees all memory allocated for a deform info structure including its vertices, indexes, and related data.
void		  R_FreeDeformInfo( deformInfo_t* deformInfo );

//! Returns the total memory used by a deformInfo_t structure and its associated data buffers
int			  R_DeformInfoMemoryUsed( deformInfo_t* deformInfo );

/*
=============================================================

TR_TRACE

=============================================================
*/

struct localTrace_t {
	float  fraction;
	// only valid if fraction < 1.0
	idVec3 point;
	idVec3 normal;
	int	   indexes[3];
};

/*!
	\brief Performs a ray-triangle intersection test with a specified radius around the ray, returning the closest intersection point.

	This function traces a ray from start to end against a triangle mesh, considering a cylindrical volume defined by the radius. It uses a series of plane culling operations to quickly eliminate
   triangles that cannot possibly intersect the ray. For remaining triangles, it performs precise intersection testing with the cylindrical ray. The function handles both skinned and static geometry
   by checking for GPU skinning availability. It returns a localTrace_t structure containing the intersection fraction, point, and surface normal.

	\param start The starting point of the ray in world space
	\param end The ending point of the ray in world space
	\param radius The radius of the cylindrical volume around the ray
	\param tri Pointer to the triangle mesh data to trace against
	\return A localTrace_t structure containing the intersection information, including fraction, point, normal, and triangle indices. Fraction is 1.0 if no intersection occurs.
*/
localTrace_t R_LocalTrace( const idVec3& start, const idVec3& end, const float radius, const srfTriangles_t* tri );

/*
============================================================

TR_BACKEND_DRAW

============================================================
*/

void		 RB_SetMVP( const idRenderMatrix& mvp );

//! Calculates and returns the pixel length of a given text string when rendered with specified scaling and length parameters.
float		 RB_DrawTextLength( const char* text, float scale, int len );

/*!
	\brief Adds debug text to be rendered during rendering

	This function queues debug text for rendering by storing the provided text information in a global debug text array. The text will be displayed at the specified origin position with the given
   scale, color, and alignment. The render time is determined by the lifetime parameter and the current debug text time. The text can optionally be depth tested to ensure proper rendering order.

	\param text The text to be displayed
	\param origin The 3D position where the text will be rendered
	\param scale The scaling factor for the text size
	\param color The color of the text as RGBA values
	\param viewAxis The view axis used for alignment of the text
	\param align The alignment of the text (e.g., left, center, right)
	\param lifetime The duration in milliseconds for which the text will be displayed
	\param depthTest Flag to enable or disable depth testing for the text rendering
*/
void		 RB_AddDebugText( const char* text, const idVec3& origin, float scale, const idVec4& color, const idMat3& viewAxis, const int align, const int lifetime, const bool depthTest );

//! Clears debug text from the render backend, optionally retaining text with a specified lifetime.
void		 RB_ClearDebugText( int time );

/*!
	\brief Adds a debug line to be rendered during rendering

	This function adds a debug line to the list of debug lines that will be rendered during the next frame. The line is defined by its start and end positions, color, lifetime, and whether it should
   be depth-tested. The function checks if there is space available in the debug lines array before adding the new line.

	\param color The color of the debug line
	\param start The starting position of the debug line
	\param end The ending position of the debug line
	\param lifeTime The lifetime of the debug line in milliseconds
	\param depthTest Whether the debug line should be depth-tested
*/
void		 RB_AddDebugLine( const idVec4& color, const idVec3& start, const idVec3& end, const int lifeTime, const bool depthTest );

//! Clears debug lines that have expired based on the provided time value.
void		 RB_ClearDebugLines( int time );

/*!
	\brief Adds a debug polygon to be rendered with specified color, winding, lifetime, and depth testing parameters.

	This function creates a debug polygon visualization by storing its properties in a global array of debug polygons. The polygon is only added if the maximum number of debug polygons has not been
   reached. Each debug polygon has a color, winding, lifetime, and depth testing flag. The lifetime is relative to the current debug polygon time counter. The function does not perform any rendering
   itself but prepares the data for later rendering.

	\param color Color of the debug polygon
	\param winding Geometric winding defining the polygon shape
	\param lifeTime Duration for which the polygon should remain visible
	\param depthTest Whether depth testing should be applied to the polygon
*/
void		 RB_AddDebugPolygon( const idVec4& color, const idWinding& winding, const int lifeTime, const bool depthTest );

//! Clears debug polygons that have expired based on the provided time value.
void		 RB_ClearDebugPolygons( int time );

//! Draws the specified bounds if they are not cleared
void		 RB_DrawBounds( const idBounds& bounds );

//! Clears all debug polygons used for rendering debugging visuals.
void		 RB_ShutdownDebugTools();
void		 RB_SetVertexColorParms( stageVertexColor_t svc );

/*
=============================================================

VR

=============================================================
*/

// #include "../vr/VRSystem.h"

/*!
	\class idDeclSkinVR
	\brief idDeclSkinVR serves as a specialized skin declaration class for virtual reality content.
*/
class idDeclSkinVR : public idDeclSkin
{
public:
	//! Initializes a new instance of the idDeclSkinVR class with default skin mappings.
	idDeclSkinVR()
	{
		base = &idDeclNullSkinBase::instance;

		const idMaterial* nodraw = declManager->FindMaterial( "textures/common/nodraw" );

		skinMapping_t	  map;

		map.from = declManager->FindMaterial( "models/characters/player/arm2" );
		map.to	 = nodraw;
		mappings.Append( map );

		map.from = declManager->FindMaterial( "models/weapons/berserk/fist" );
		map.to	 = nodraw;
		mappings.Append( map );

		map.from = declManager->FindMaterial( "models/weapons/hands/hand" );
		map.to	 = nodraw;
		mappings.Append( map );
	}
};

//=============================================

#include "../backend/RenderLog.h"
#include "GLMatrix.h"

#include "../BufferObject.h"
#include "../backend/RenderProgs.h"
#include "RenderWorld_local.h"
#include "GuiModel.h"
#include "../VertexCache.h"

#endif /* !__TR_LOCAL_H__ */
