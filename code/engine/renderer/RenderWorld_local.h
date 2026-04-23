/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2021 Robert Beckebans

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

#ifndef __RENDERWORLDLOCAL_H__
#define __RENDERWORLDLOCAL_H__

// assume any lightDef or entityDef index above this is an internal error
const int LUDICROUS_INDEX = 10000;

typedef struct portal_s {
	int					   intoArea; // area this portal leads to
	idWinding*			   w;		 // winding points have counter clockwise ordering seen this area
	idPlane				   plane;	 // view must be on the positive side of the plane to cross
	struct portal_s*	   next;	 // next portal of the area
	struct doublePortal_s* doublePortal;
} portal_t;

typedef struct doublePortal_s {
	struct portal_s*	   portals[2];
	int					   blockingBits; // PS_BLOCK_VIEW, PS_BLOCK_AIR, etc, set by doors that shut them off

	// A portal will be considered closed if it is past the
	// fog-out point in a fog volume.  We only support a single
	// fog volume over each portal.
	idRenderLightLocal*	   fogLight;
	struct doublePortal_s* nextFoggedPortal;
} doublePortal_t;

// RB: added Quake 3 style light grid
// however this 2021 version features Spherical Harmonics instead of ambient + directed color
struct lightGridPoint_t {
	idVec3						   origin; // not saved to .proc
	byte						   valid;  // is not in the void

	SphericalHarmonicsT<idVec3, 4> shRadiance; // L4 Spherical Harmonics
};

/*!
	\class LightGrid
	\brief A class for managing light grid structures used in lighting calculations.

	The LightGrid class provides functionality for creating and managing light grid structures within a 3D rendering environment. It handles the setup of grid dimensions based on spatial bounds and
   desired cell sizes, while accommodating a specified maximum number of probes. The class supports coordinate conversion between grid positions and probe indices, and provides debugging visualization
   capabilities through color mapping. It also includes methods for calculating light grid point positions and setting up entity lighting. The grid is configured for a specific area within a
   multi-area lighting system and can be integrated with a render world context for accurate lighting computations.

*/
class LightGrid
{
public:
	idVec3					 lightGridOrigin;
	idVec3					 lightGridSize;
	int						 lightGridBounds[3];

	idList<lightGridPoint_t> lightGridPoints;

	int						 area;
	idImage*				 irradianceImage;
	int						 imageSingleProbeSize; // including border
	int						 imageBorderSize;

	//! Initializes a new instance of the LightGrid class with default settings.
	LightGrid();

	/*!
		\brief Initializes and configures a light grid for lighting calculations within the specified bounds and area

		Sets up the light grid structure by calculating the grid dimensions based on the provided bounds and grid size parameters. The function determines the optimal grid spacing to accommodate the
	   specified maximum number of probes while ensuring the grid fits within the given bounds. It calculates the grid point positions and prepares the light grid for subsequent lighting computations.

		\param bounds The 3D bounds that define the volume for which the light grid is being created
		\param baseName The name of the map or scene being processed
		\param world Pointer to the render world context for lighting calculations
		\param gridSize The desired size of each grid cell in the light grid
		\param _area The current area number being processed in a multi-area lighting setup
		\param totalAreas The total number of areas in the lighting system
		\param maxProbes Maximum number of grid points to be allocated for this area
		\param printToConsole Flag indicating whether to output progress information to the console
	*/
	void	 SetupLightGrid( const idBounds& bounds, const char* baseName, const idRenderWorld* world, const idVec3& gridSize, int _area, int totalAreas, int maxProbes, bool printToConsole );

	//! Calculates the base grid coordinates for a given origin position within the light grid
	void	 GetBaseGridCoord( const idVec3& origin, int gridCoord[3] );

	//! Converts grid coordinates to a probe index.
	int		 GridCoordToProbeIndex( int gridCoord[3] );

	//! Converts a probe index to its corresponding grid coordinates in the light grid.
	void	 ProbeIndexToGridCoord( const int probeIndex, int gridCoord[3] );

	//! Returns a debug color for a given grid coordinate in the light grid
	idVec3	 GetGridCoordDebugColor( int gridCoord[3] );

	//! Returns the debug color for a given probe index in the light grid.
	idVec3	 GetProbeIndexDebugColor( const int probeIndex );

	//! Returns the count of valid light grid points in the light grid.
	int		 CountValidGridPoints() const;

	//! Returns the irradiance image used for light grid rendering.
	idImage* GetIrradianceImage() const { return irradianceImage; }

	// fetch grid lighting on a per object basis
	void	 SetupEntityGridLighting( idRenderEntityLocal* def );

private:
	//! Calculates the positions of light grid points for a specified area in the render world
	void CalculateLightGridPointPositions( const idRenderWorld* world, int area );
};
// RB end

typedef struct portalArea_s {
	int				areaNum;
	int				connectedAreaNum[NUM_PORTAL_ATTRIBUTES]; // if two areas have matching connectedAreaNum, they are
	// not separated by a portal with the apropriate PS_BLOCK_* blockingBits

	idBounds		globalBounds; // RB: AABB of the BSP area used for light grid density

	LightGrid		lightGrid;

	int				viewCount;	  // set by R_FindViewLightsAndEntities
	portal_t*		portals;	  // never changes after load
	areaReference_t entityRefs;	  // head/tail of doubly linked list, may change
	areaReference_t lightRefs;	  // head/tail of doubly linked list, may change
	areaReference_t envprobeRefs; // head/tail of doubly linked list, may change
} portalArea_t;

static const int CHILDREN_HAVE_MULTIPLE_AREAS = -2;
static const int AREANUM_SOLID				  = -1;
typedef struct {
	idPlane plane;
	int		children[2];		// negative numbers are (-1 - areaNumber), 0 = solid
	int		commonChildrenArea; // if all children are either solid or a single area,
								// this is the area number, else CHILDREN_HAVE_MULTIPLE_AREAS
} areaNode_t;

struct reusableDecal_t {
	qhandle_t			entityHandle;
	int					lastStartTime;
	idRenderModelDecal* decals;
};

struct reusableOverlay_t {
	qhandle_t			  entityHandle;
	int					  lastStartTime;
	idRenderModelOverlay* overlays;
};

struct portalStack_t;

/*!
	\class idRenderWorldLocal
	\brief A local rendering world implementation that manages entities, lights, and environment probes for scene rendering.

	This class provides a complete implementation of a rendering world that handles the management of renderable objects including entities, lights, and environment probes. It supports loading and
   parsing of map data, maintaining portal areas for visibility determination, and managing the rendering of scenes through multiple subviews. The class handles both static and dynamic object
   management, including decals and overlays that can be projected onto surfaces. It supports tracing operations for collision detection and visibility testing, and includes debugging visualization
   capabilities for rendering and spatial queries. The implementation also handles light grid management for lighting calculations and supports multiple render views through portal and mirror
   rendering. Memory management is handled through dedicated allocation and freeing methods for each object type, with automatic cleanup during destruction.

*/
class idRenderWorldLocal : public idRenderWorld
{
public:
	//! Initializes a new instance of the idRenderWorldLocal class.
	idRenderWorldLocal();

	//! Destructor for the idRenderWorldLocal class that cleans up all rendering resources.
	virtual ~idRenderWorldLocal();

	//! Initializes the render world from a map file with the specified name.
	virtual bool							InitFromMap( const char* mapName );

	//! Clears the local render models list to prevent crashes when switching between expansion packs.
	virtual void							ResetLocalRenderModels();

	//! Adds a new entity definition to the render world and returns a handle to it.
	virtual qhandle_t						AddEntityDef( const renderEntity_t* re );

	//! Updates the render entity definition for a given entity handle with the provided render entity data.
	virtual void							UpdateEntityDef( qhandle_t entityHandle, const renderEntity_t* re );

	//! Frees the entity definition associated with the given handle and clears its entry in the world list
	virtual void							FreeEntityDef( qhandle_t entityHandle );

	//! Retrieves the render entity parameters for a given entity handle.
	virtual const renderEntity_t*			GetRenderEntity( qhandle_t entityHandle ) const;

	//! Adds a light definition to the render world and returns a handle to it
	virtual qhandle_t						AddLightDef( const renderLight_t* rlight );

	//! Updates the specified light definition with new parameters
	virtual void							UpdateLightDef( qhandle_t lightHandle, const renderLight_t* rlight );

	//! Frees the resources associated with a light definition handle.
	virtual void							FreeLightDef( qhandle_t lightHandle );

	//! Retrieves the render light parameters for a given light handle.
	virtual const renderLight_t*			GetRenderLight( qhandle_t lightHandle ) const;

	//! Adds a new environment probe definition and returns a handle to it
	virtual qhandle_t						AddEnvprobeDef( const renderEnvironmentProbe_t* ep );

	//! Updates an environment probe definition in the render world
	virtual void							UpdateEnvprobeDef( qhandle_t envprobeHandle, const renderEnvironmentProbe_t* ep );

	//! Frees the environment probe definition associated with the given handle
	virtual void							FreeEnvprobeDef( qhandle_t envprobeHandle );

	//! Retrieves the environment probe parameters for the given handle.
	virtual const renderEnvironmentProbe_t* GetRenderEnvprobe( qhandle_t envprobeHandle ) const;

	//! Checks if a given area needs portal sky rendering.
	virtual bool							CheckAreaForPortalSky( int areaNum );

	//! Forces the generation of all light/surface interactions at the start of a level
	virtual void							GenerateAllInteractions();

	//! Regenerates the world by freeing derived data and recreating world references.
	virtual void							RegenerateWorld();

	/*!
		\brief Projects a decal onto world surfaces based on a winding and projection parameters

		This function creates decals on world geometry by projecting a decal from a winding plane towards a specified origin. The decal is faded based on depth from the winding plane over a specified
	   distance. The function processes all areas that intersect with the projection volume and checks each model in those areas for compatibility with decals. Only static models that allow overlays
	   and are within the projection bounds will receive decals. Dynamic or callback models are ignored. The projection is performed in local space for each applicable model.

		\param winding The winding that defines the decal shape and plane
		\param projectionOrigin The origin point from which to project the decal
		\param parallel Whether to project parallel to the winding plane
		\param fadeDepth The depth over which the decal fades from the winding plane
		\param material The material to use for the decal
		\param startTime The start time for the decal effect
	*/
	virtual void ProjectDecalOntoWorld( const idFixedWinding& winding, const idVec3& projectionOrigin, const bool parallel, const float fadeDepth, const idMaterial* material, const int startTime );

	/*!
		\brief Creates a decal on a static model by projecting it onto the model surface from a specified winding and origin.

		This function projects a decal onto a static model using a winding, projection origin, and material. The decal is created between the winding plane and the projection origin, with depth fading
	   applied based on the fade depth parameter. The function performs various checks to ensure the entity handle is valid, the model is static, and the projection bounds intersect with the model
	   bounds before creating the decal. The decal is added to the entity's decals list for deferred rendering.

		\param entityHandle Handle to the entity definition for the model to project the decal onto
		\param winding Winding that defines the shape and bounds of the decal projection
		\param projectionOrigin Origin point from which the decal is projected onto the model
		\param parallel Flag indicating whether the projection is parallel
		\param fadeDepth Depth over which the decal fades from the winding plane
		\param material Material to use for the decal
		\param startTime Start time for the decal effect
		\throws Error if the entity handle is out of bounds
	*/
	virtual void ProjectDecal(
		qhandle_t entityHandle, const idFixedWinding& winding, const idVec3& projectionOrigin, const bool parallel, const float fadeDepth, const idMaterial* material, const int startTime );

	/*!
		\brief Projects a texture overlay onto a dynamic model entity.

		This function projects a texture overlay onto a specified dynamic model entity using the provided texture coordinates and material. The overlay is applied to the entity's model and is
	   scheduled for rendering at the specified start time. The function performs validation checks to ensure the entity handle is valid and that the model is a dynamic model. If the entity does not
	   already have overlay data, it allocates new overlay data for the entity.

		\param entityHandle Handle to the entity definition for which the overlay is to be projected
		\param localTextureAxis Texture coordinate axes defining the overlay's projection on the model
		\param material Material to be used for the overlay texture
		\param startTime Start time for rendering the overlay
	*/
	virtual void		 ProjectOverlay( qhandle_t entityHandle, const idPlane localTextureAxis[2], const idMaterial* material, const int startTime );

	//! Removes all decals and overlays from the specified entity definition
	virtual void		 RemoveDecals( qhandle_t entityHandle );

	//! Sets the current render view parameters for subsequent rendering operations.
	virtual void		 SetRenderView( const renderView_t* renderView );

	//! Renders a scene using the provided view parameters, handling multiple subviews for mirrors and portals.
	virtual void		 RenderScene( const renderView_t* renderView );

	//! Returns the total number of portal areas in the render world.
	virtual int			 NumAreas() const;

	//! Returns the area number containing the specified point, or -1 if the point is not in any area
	virtual int			 PointInArea( const idVec3& point ) const;

	//! Returns the number of portal areas the given bounds intersect with and fills the provided array with their numbers
	virtual int			 BoundsInAreas( const idBounds& bounds, int* areas, int maxAreas ) const;

	//! Returns the number of portals in the specified portal area.
	virtual int			 NumPortalsInArea( int areaNum );

	//! Returns the portal data for a specified area and portal number.
	virtual exitPortal_t GetPortal( int areaNum, int portalNum );

	//! Returns the bounding box for the specified portal area.
	virtual idBounds	 AreaBounds( int areaNum ) const;

	//! Performs a ray trace against GUI surfaces in a specified entity and returns the normalized texture coordinates of the hit point.
	virtual guiPoint_t	 GuiTrace( qhandle_t entityHandle, const idVec3 start, const idVec3 end ) const;

	/*!
		\brief Performs a trace against the model of a specified entity, checking for intersections with collision surfaces or visible surfaces based on shader properties.

		This function traces a ray from start to end position against the model of an entity specified by entityHandle. It supports tracing with a specified radius and handles both collision surfaces
	   and regular drawn surfaces. The function transforms the global coordinates into local model space for tracing, and then converts the results back to global coordinates for the output trace
	   structure. It returns true if a collision or intersection occurs and sets the trace information accordingly, including fraction, point, normal, material, entity, and joint number.

		\param trace Output structure to store the trace results
		\param entityHandle Handle to the entity whose model is traced against
		\param start Start position of the ray in global coordinates
		\param end End position of the ray in global coordinates
		\param radius Radius of the ray for swept volume collision detection
		\return True if the ray intersects with the entity's model, false otherwise.
	*/
	virtual bool		 ModelTrace( modelTrace_t& trace, qhandle_t entityHandle, const idVec3& start, const idVec3& end, const float radius ) const;

	/*!
		\brief Performs a trace operation from start to end, checking for intersections with model surfaces.

		This function traces a line from the start point to the end point, checking for intersections with model surfaces. It supports an optional radius for the trace, and can skip dynamic models and
	   player models based on the provided flags. The function updates the trace structure with information about the first intersection encountered, including the fraction of the trace completed, the
	   intersection point, surface normal, material, entity, and joint number. The trace bounds are calculated based on the start and end points, and only models and surfaces within these bounds are
	   checked for intersections.

		\param trace Structure to store the results of the trace operation
		\param start Starting point of the trace
		\param end Ending point of the trace
		\param radius Radius of the trace, used for collision detection
		\param skipDynamic Flag indicating whether to skip dynamic models during the trace
		\param skipPlayer Flag indicating whether to skip player models during the trace
		\return True if an intersection was found, False otherwise
	*/
	virtual bool		 Trace( modelTrace_t& trace, const idVec3& start, const idVec3& end, const float radius, bool skipDynamic = true, bool skipPlayer = false ) const;

	//! Performs a fast ray trace against the world's BSP tree and returns true if a collision occurred
	virtual bool		 FastWorldTrace( modelTrace_t& trace, const idVec3& start, const idVec3& end ) const;

	//! Clears debug lines and text that were drawn during the current frame.
	virtual void		 DebugClearLines( int time );

	/*!
		\brief Adds a debug line to the rendering world with specified color, start and end points, lifetime, and depth testing options.

		This function is used to visualize debug lines in the rendering world. It takes a color, start and end points, an optional lifetime for how long the line should persist, and a boolean flag to
	   enable or disable depth testing. The line is added to the render backend for display during debugging sessions.

		\param color The color of the debug line specified as RGBA values
		\param start The starting point of the debug line in world space coordinates
		\param end The ending point of the debug line in world space coordinates
		\param lifetime How long the line should remain visible (0 means infinite lifetime)
		\param depthTest Whether depth testing should be applied to the line rendering
	*/
	virtual void		 DebugLine( const idVec4& color, const idVec3& start, const idVec3& end, const int lifetime = 0, const bool depthTest = false );

	/*!
		\brief Draws a debug arrow from start to end position with optional size and lifetime.

		This function renders a visual arrow representation between two 3D points using the specified color. The arrow consists of a main line from start to end, and additional lines forming a cone at
	   the end to indicate direction. The arrow includes a configurable size and a lifetime parameter that determines how long the arrow will be visible. The arrow rendering is optimized by caching
	   trigonometric values for performance.

		\param color Color of the arrow as RGBA values
		\param start Starting position of the arrow in world coordinates
		\param end Ending position of the arrow in world coordinates
		\param size Size of the arrowhead cone in world units
		\param lifetime Time in milliseconds for which the arrow will be visible, 0 means infinite
	*/
	virtual void		 DebugArrow( const idVec4& color, const idVec3& start, const idVec3& end, int size, const int lifetime = 0 );

	/*!
		\brief Draws a winding as a series of connected lines for debugging visualization.

		This function takes a winding object and renders it as a sequence of line segments using the DebugLine function. Each vertex of the winding is transformed by the provided origin and axis to
	   determine its position in world space. The winding is closed by connecting the last point to the first point. This is useful for visualizing the shape and structure of geometric primitives
	   during debugging.

		\param color The color to use for drawing the winding lines
		\param w The winding object containing the vertices to be drawn
		\param origin The origin point to translate the winding vertices by
		\param axis The transformation matrix to apply to the winding vertices
		\param lifetime How long the winding will remain visible in milliseconds, or 0 for infinite visibility
		\param depthTest Whether to enable depth testing for the lines, making them properly occluded by other geometry
	*/
	virtual void		 DebugWinding( const idVec4& color, const idWinding& w, const idVec3& origin, const idMat3& axis, const int lifetime = 0, const bool depthTest = false );

	/*!
		\brief Draws a debug circle using line segments in the render world

		This function renders a circular debug visualization by computing points along the circumference using trigonometric functions and connecting them with line segments. The circle is oriented
	   perpendicular to the provided direction vector and centered at the specified origin. The rendering uses the DebugLine function to draw each segment of the circle, allowing for customization of
	   color, radius, number of steps, lifetime, and depth testing.

		\param color Color of the circle lines
		\param origin Center point of the circle
		\param dir Direction vector perpendicular to the circle plane
		\param radius Radius of the circle
		\param numSteps Number of line segments to use for drawing the circle
		\param lifetime Duration in milliseconds to display the circle, or 0 for permanent
		\param depthTest Whether to perform depth testing for the circle rendering
	*/
	virtual void		 DebugCircle( const idVec4& color, const idVec3& origin, const idVec3& dir, const float radius, const int numSteps, const int lifetime = 0, const bool depthTest = false );

	/*!
		\brief Draws a wireframe sphere for debug visualization using debug lines

		This function renders a sphere as a collection of line segments to visualize its shape in the debug rendering system. The sphere is constructed using latitude and longitude lines, with each
	   line segment drawn using the DebugLine function. The sphere is defined by its center point and radius, and rendered using the specified color. The lifetime parameter controls how long the
	   sphere remains visible, and the depthTest parameter determines whether depth testing is applied to the rendered lines.

		\param color The color to use for drawing the sphere lines
		\param sphere The sphere definition containing origin and radius
		\param lifetime How long the sphere lines remain visible (0 means permanent)
		\param depthTest Whether depth testing is applied to the rendered lines
	*/
	virtual void		 DebugSphere( const idVec4& color, const idSphere& sphere, const int lifetime = 0, bool depthTest = false );

	/*!
		\brief Draws the bounds of a 3D volume as a wireframe cube using the specified color and optional offset.

		This function visualizes a 3D bounding box by drawing its edges as line segments. It takes a color, a bounds object, an optional origin offset, and a lifetime parameter for how long the
	   visualization should persist. The bounds are expanded by the origin offset to position the cube in world space. Each edge of the cube is drawn using the DebugLine function, which allows for
	   persistent visualization of bounding volumes for debugging purposes.

		\param color The color to use for drawing the bounding box lines
		\param bounds The bounds that define the shape and size of the cube
		\param org An optional offset to position the cube in world space, defaults to vec3_origin
		\param lifetime How long the visualization should persist in milliseconds, defaults to 0 for infinite
	*/
	virtual void		 DebugBounds( const idVec4& color, const idBounds& bounds, const idVec3& org = vec3_origin, const int lifetime = 0 );

	//! Draws a debug visualization of a box using lines.
	virtual void		 DebugBox( const idVec4& color, const idBox& box, const int lifetime = 0 );

	/*!
		\brief Draws a cone shape in the debug visualization using two radii and a direction from an apex point.

		The function creates a cone shape defined by an apex point, a direction vector, and two radii. The first radius is at the apex and the second at the point defined by adding the direction
	   vector to the apex. If radius1 is zero, a cone with a pointed tip is drawn. Otherwise, a frustum-like shape with both radii is created. The cone is drawn using DebugLine calls to render the
	   boundary edges and connecting lines between the two radii.

		\param color The color of the cone lines
		\param apex The starting point of the cone
		\param dir The direction vector defining the axis of the cone
		\param radius1 The radius at the apex point
		\param radius2 The radius at the point defined by apex + dir
		\param lifetime How long the cone will be visible in the debug visualization
	*/
	virtual void		 DebugCone( const idVec4& color, const idVec3& apex, const idVec3& dir, float radius1, float radius2, const int lifetime = 0 );

	/*!
		\brief Draws a rectangular debug overlay on the screen using the specified color and screen coordinates

		This function creates a debug rectangle overlay on the screen by converting screen space coordinates to world space positions using the provided view definition. It calculates the rectangle
	   boundaries based on the screen viewport and field of view, then draws the rectangle outline using existing debug line functionality. The rectangle is projected into 3D space and rendered as a
	   wireframe overlay on top of the current rendered view.

		\param color Color of the rectangle overlay
		\param rect Screen coordinates defining the rectangle area
		\param viewDef View definition containing viewport and projection information
		\param lifetime Duration in milliseconds to display the rectangle (0 = infinite)
	*/
	virtual void		 DebugScreenRect( const idVec4& color, const idScreenRect& rect, const viewDef_t* viewDef, const int lifetime = 0 );

	//! Draws a visual representation of a coordinate axis at the specified origin using three colored arrows
	virtual void		 DebugAxis( const idVec3& origin, const idMat3& axis );

	//! Clears debug polygons from the render world with the specified time parameter.
	virtual void		 DebugClearPolygons( int time );

	/*!
		\brief Adds a debug polygon to the rendering world for visualization purposes

		This function adds a polygon debug primitive to the rendering world that will be displayed during rendering. The polygon is defined by a winding and colored with the specified color. The
	   polygon will remain visible for a specified time period or indefinitely if the lifetime is zero. Depth testing can be enabled to ensure the polygon is properly occluded by other geometry.

		\param color The color to draw the polygon with
		\param winding The winding defining the polygon vertices
		\param lifeTime How long the polygon should remain visible in milliseconds, 0 means forever
		\param depthTest Whether depth testing should be enabled for the polygon rendering
	*/
	virtual void		 DebugPolygon( const idVec4& color, const idWinding& winding, const int lifeTime = 0, const bool depthTest = false );

	/*!
		\brief Draws debug text at a specified position in the 3D world with optional alignment, scaling, and lifetime.

		This function renders a text string in the 3D world at the given origin position, oriented according to the provided view axis. The text can be scaled, colored, and aligned left, center, or
	   right. An optional lifetime can be specified to control how long the text remains visible, and depth testing can be enabled to ensure proper rendering relative to other objects in the scene.

		\param text The text string to be rendered
		\param origin The 3D position in the world where the text will be drawn
		\param scale A scaling factor for the text size
		\param color The color of the text as RGBA values
		\param viewAxis The orientation matrix that determines how the text is aligned in the world
		\param align Text alignment where 0 is left, 1 is center (default), and 2 is right
		\param lifetime The time in milliseconds for how long the text should remain visible, or 0 for permanent visibility
		\param depthTest Whether depth testing should be applied to ensure correct rendering relative to other 3D objects
	*/
	virtual void	DrawText( const char* text, const idVec3& origin, float scale, const idVec4& color, const idMat3& viewAxis, const int align = 1, const int lifetime = 0, bool depthTest = false );

	//-----------------------

	idStr			mapName;	  // ie: maps/tim_dm2.proc, written to demoFile
	ID_TIME_T		mapTimeStamp; // for fast reloads of the same level

	areaNode_t*		areaNodes;
	int				numAreaNodes;

	portalArea_t*	portalAreas;
	int				numPortalAreas;
	int				connectedAreaNum; // incremented every time a door portal state changes

	idScreenRect*	areaScreenRect;

	doublePortal_t* doublePortals;
	int				numInterAreaPortals;

	idList<idRenderModel*, TAG_MODEL>		   localModels;

	idList<idRenderEntityLocal*, TAG_ENTITY>   entityDefs;
	idList<idRenderLightLocal*, TAG_LIGHT>	   lightDefs;
	idList<RenderEnvprobeLocal*, TAG_ENVPROBE> envprobeDefs; // RB

	idBlockAlloc<areaReference_t, 1024>		   areaReferenceAllocator;
	idBlockAlloc<idInteraction, 256>		   interactionAllocator;

#if 1 // def ID_PC
	static const int MAX_DECAL_SURFACES = 32;
#else
	static const int MAX_DECAL_SURFACES = 16;
#endif
	idArray<reusableDecal_t, MAX_DECAL_SURFACES>   decals;
	idArray<reusableOverlay_t, MAX_DECAL_SURFACES> overlays;

	// all light / entity interactions are referenced here for fast lookup without
	// having to crawl the doubly linked lists.  EnntityDefs are sequential for better
	// cache access, because the table is accessed by light in idRenderWorldLocal::CreateLightDefInteractions()
	// Growing this table is time consuming, so we add a pad value to the number
	// of entityDefs and lightDefs
	idInteraction**								   interactionTable;
	int											   interactionTableWidth;  // entityDefs
	int											   interactionTableHeight; // lightDefs

	bool										   generateAllInteractionsCalled;

	/*!
		\brief Parses a model from a source lexer, initializes it with the given parameters, and returns a new render model instance.

		This function reads model data from a lexer, parses the model name, allocates a new render model, and processes surface data including vertices and indices. It handles texture coordinate
	   centering for 16-bit precision and manages memory for static triangle surfaces. The model is initialized with a name and supports binary model output if configured. The function ensures proper
	   surface addition and model finish when parsing is complete.

		\param src Lexer object containing the model data to parse
		\param mapName Name of the map the model belongs to
		\param mapTimeStamp Timestamp of the map for version control
		\param fileOut Optional output file handle to write binary model data
		\return Pointer to the newly parsed and initialized render model instance
		\throws Error thrown if the lexer encounters unexpected token structure during parsing
	*/
	idRenderModel*								   ParseModel( idLexer* src, const char* mapName, ID_TIME_T mapTimeStamp, idFile* fileOut );

	//! Initializes area references for all portal areas in the render world.
	void										   SetupAreaRefs();

	//! Parses inter-area portals from a lexer input and optionally writes them to a file output
	void										   ParseInterAreaPortals( idLexer* src, idFile* fileOut );

	//! Parses area node data from a lexer source and optionally writes it to a file.
	void										   ParseNodes( idLexer* src, idFile* fileOut );

	//! Computes the common area index for the children of a given area node.
	int											   CommonChildrenArea_r( areaNode_t* node );

	//! Frees all memory and resources associated with the render world
	void										   FreeWorld();

	//! Initializes the render world with a single portal area and sets up the area reference structure.
	void										   ClearWorld();

	//! Frees all render definitions and interaction data associated with this render world.
	void										   FreeDefs();

	//! Ensures all world models are loaded by checking each model in the local models array.
	void										   TouchWorldModels();

	//! Initializes and adds world model entities for each portal area in the render world
	void										   AddWorldModelEntities();

	//! Initializes all portal states to open and performs flood fill to establish area connections.
	void										   ClearPortalStates();

	//! Reads binary area portal data from a file and initializes the portal area structures.
	void										   ReadBinaryAreaPortals( idFile* file );

	//! Reads binary node data from a file into the render world.
	void										   ReadBinaryNodes( idFile* file );

	//! Loads a binary model from a file and returns a pointer to the loaded model or NULL if loading fails.
	idRenderModel*								   ReadBinaryModel( idFile* file );

	//--------------------------
	// RenderWorld_portals.cpp

	// if we hit this many planes, we will just stop cropping the
	// view down, which is still correct, just conservative
	static const int							   MAX_PORTAL_PLANES = 20;

	struct portalStack_t {
		const portal_t*		 p;
		const portalStack_t* next;
		// positive side is outside the visible frustum
		int					 numPortalPlanes;
		idPlane				 portalPlanes[MAX_PORTAL_PLANES + 1];
		idScreenRect		 rect;
	};

	//! Determines if an entity's bounds are completely outside the current portal chain.
	bool				  CullEntityByPortals( const idRenderEntityLocal* entity, const portalStack_t* ps );

	//! Adds entities from a specified area to the view, updating their scissor rectangles based on the portal stack.
	void				  AddAreaViewEntities( int areaNum, const portalStack_t* ps );

	//! Determines if a light frustum is culled by portal boundaries.
	bool				  CullLightByPortals( const idRenderLightLocal* light, const portalStack_t* ps );

	//! Adds lights from a specific area to the view lights list based on portal visibility.
	void				  AddAreaViewLights( int areaNum, const portalStack_t* ps );

	//! Determines if an environment probe should be culled based on portal visibility.
	bool				  CullEnvprobeByPortals( const RenderEnvprobeLocal* probe, const portalStack_t* ps );

	//! Adds environment probes for a specified area to the view, updating their scissor rectangles.
	void				  AddAreaViewEnvprobes( int areaNum, const portalStack_t* ps );

	//! Adds an area to the view by marking its view count and including entities, lights, and envprobes.
	void				  AddAreaToView( int areaNum, const portalStack_t* ps );

	//! Computes the screen-space bounding rectangle for a winding in the given view space
	idScreenRect		  ScreenRectFromWinding( const idWinding* w, const viewEntity_t* space );

	//! Determines if a portal is completely fogged out by checking if all points of the portal winding are beyond the fog distance
	bool				  PortalIsFoggedOut( const portal_t* p );

	//! Recursively floods the view through portals starting from a given area.
	void				  FloodViewThroughArea_r( const idVec3& origin, int areaNum, const portalStack_t* ps );

	//! Determines visible areas and entities by flowing through portals from a given origin point
	void				  FlowViewThroughPortals( const idVec3& origin, int numPlanes, const idPlane* planes );

	//! Recursively builds connected areas for the renderer by traversing non-blocked portals from a given area number.
	void				  BuildConnectedAreas_r( int areaNum );

	//! Builds a list of connected portal areas for the current view definition.
	void				  BuildConnectedAreas();

	//! Updates cached view lights and entities for the current view.
	void				  FindViewLightsAndEntities();

	//! Recursively floods light through portals starting from a given area
	void				  FloodLightThroughArea_r( idRenderLightLocal* light, int areaNum, const portalStack_t* ps );

	//! Adds area references to areas that the light center flows into for shadow casting.
	void				  FlowLightThroughPortals( idRenderLightLocal* light );

	//! Returns the number of portals in the render world.
	int					  NumPortals() const;

	//! Returns the handle of the first portal that intersects the given bounds, or zero if none intersect.
	qhandle_t			  FindPortal( const idBounds& b ) const;

	//! Sets the blocking state of a portal to control area connections.
	void				  SetPortalState( qhandle_t portal, int blockingBits );

	//! Returns the blocking state of a specified portal.
	int					  GetPortalState( qhandle_t portal );

	//! Determines if two portal areas are connected through a chain of portals that do not block the specified connection type.
	bool				  AreasAreConnected( int areaNum1, int areaNum2, portalConnection_t connection ) const;

	//! Flood fills connected areas starting from the given area and propagates through portals based on the specified portal attribute index.
	void				  FloodConnectedAreas( portalArea_t* area, int portalAttributeIndex );

	//! Returns the screen rectangle for the specified area number.
	idScreenRect&		  GetAreaScreenRect( int areaNum ) const { return areaScreenRect[areaNum]; }

	//! Resizes the interaction table when it overflows.
	void				  ResizeInteractionTable();

	//! Adds an entity reference to a specified portal area
	void				  AddEntityRefToArea( idRenderEntityLocal* def, portalArea_t* area );

	//! Adds a light reference to a specific area in the render world.
	void				  AddLightRefToArea( idRenderLightLocal* light, portalArea_t* area );

	//! Adds an environment probe reference to a specified portal area
	void				  AddEnvprobeRefToArea( RenderEnvprobeLocal* probe, portalArea_t* area );

	/*!
		\brief Performs recursive BSP traversal for model tracing operations

		This function recursively traverses a BSP (Binary Space Partitioning) tree to determine the intersection of a line segment with the world geometry. It handles both solid and empty leaf nodes,
	   computes intersection points with plane boundaries, and continues traversal based on the relative positioning of the trace endpoints. The function updates the trace results with the closest
	   intersection found.

		\param results Output structure containing the trace results including fraction, point, and normal
		\param parentNodeNum Index of the parent node in the areaNodes array
		\param nodeNum Index of the current node in the areaNodes array
		\param p1f Start fraction of the trace segment
		\param p2f End fraction of the trace segment
		\param p1 Start point of the trace segment
		\param p2 End point of the trace segment
	*/
	void				  RecurseProcBSP_r( modelTrace_t* results, int parentNodeNum, int nodeNum, float p1f, float p2f, const idVec3& p1, const idVec3& p2 ) const;

	/*!
		\brief Recursively determines which areas in a render world intersect with a given bounding box.

		This function traverses a spatial partitioning structure to find all areas that contain parts of the specified bounding box. It handles recursive traversal of the tree structure, checking
	   which side of each plane the bounds fall on, and collects area indices that intersect with the bounds. The function avoids duplicate area entries and respects a maximum area count limit.

		\param nodeNum Index of the current node in the area tree structure
		\param bounds Bounding box to test against the area structure
		\param areas Output array to store the indices of intersecting areas
		\param numAreas Pointer to the count of areas found so far
		\param maxAreas Maximum number of areas to store in the output array
	*/
	void				  BoundsInAreas_r( int nodeNum, const idBounds& bounds, int* areas, int* numAreas, int maxAreas ) const;

	//! Returns the length of the given text when rendered with the specified scale and length parameters.
	float				  DrawTextLength( const char* text, float scale, int len = 0 );

	//! Frees all interactions associated with entity definitions in the render world.
	void				  FreeInteractions();

	/*!
		\brief Recursively pushes a frustum into the portal area tree for visibility optimization.

		This function traverses the portal area tree to determine which areas a frustum intersects with. It adds references to entities and lights in the relevant areas, avoiding redundant processing
	   by tracking view counts. The function uses plane culling to efficiently navigate the tree structure and only processes child nodes when necessary based on frustum-plane intersection tests. The
	   commonChildrenArea optimization allows early termination when all children are known to belong to the same area, improving performance.

		\param def Pointer to the render entity to add to the area, or NULL if none
		\param light Pointer to the render light to add to the area, or NULL if none
		\param corners The frustum corners defining the volume to test against the tree
		\param nodeNum The current node number in the portal area tree to process
	*/
	void				  PushFrustumIntoTree_r( idRenderEntityLocal* def, idRenderLightLocal* light, const frustumCorners_t& corners, int nodeNum );

	/*!
		\brief Pushes a frustum into the spatial acceleration tree for the given entity or light.

		This function takes a frustum defined by a transformation matrix and bounds, calculates its corners in world space, and recursively inserts these corners into the spatial acceleration tree.
	   The frustum is used for visibility determination and spatial partitioning within the render world. The function is designed to work with either an entity or a light, but not both
	   simultaneously.

		\param def Pointer to the render entity associated with the frustum, or NULL if only a light is involved
		\param light Pointer to the render light associated with the frustum, or NULL if only an entity is involved
		\param frustumTransform The transformation matrix that defines the frustum's orientation and position in world space
		\param frustumBounds The bounds that define the frustum's size and shape in view space
	*/
	void				  PushFrustumIntoTree( idRenderEntityLocal* def, idRenderLightLocal* light, const idRenderMatrix& frustumTransform, const idBounds& frustumBounds );

	//! Recursively pushes an environment probe into the portal area tree based on its origin relative to node planes
	void				  PushEnvprobeIntoTree_r( RenderEnvprobeLocal* probe, int nodeNum );

	//! Allocates and returns a decal for the specified entity handle and start time, reusing the oldest decal if necessary
	idRenderModelDecal*	  AllocDecal( qhandle_t newEntityHandle, int startTime );

	//! Allocates and returns an overlay for a given entity handle and start time, reusing the oldest overlay if necessary.
	idRenderModelOverlay* AllocOverlay( qhandle_t newEntityHandle, int startTime );

	//-------------------------------
	// tr_light.c
	void				  CreateLightDefInteractions( idRenderLightLocal* const ldef, const int renderViewID );

	//! Initializes or loads the light grid for the rendering world.
	void				  SetupLightGrid();

	//! Writes light grid data to a file with the specified filename
	void				  WriteLightGridsToFile( const char* filename );

	//! Writes light grid data to a file.
	void				  WriteLightGrid( idFile* fp, const LightGrid& lightGrid );

	//! Loads a light grid file and returns true if successful.
	bool				  LoadLightGridFile( const char* name );

	//! Loads light grid images for all portal areas in the current map.
	void				  LoadLightGridImages();

	//! Parses light grid point data from a lexer input and optionally writes it to a file output
	void				  ParseLightGridPoints( idLexer* src, idFile* fileOut );

	//! Reads binary light grid point data from a file into the specified portal area
	void				  ReadBinaryLightGridPoints( idFile* file );
	// RB end
};

// if an entity / light combination has been evaluated and found to not genrate any surfaces or shadows,
// the constant INTERACTION_EMPTY will be stored in the interaction table, int contrasts to NULL, which
// means that the combination has not yet been tested for having surfaces.
static idInteraction* const INTERACTION_EMPTY = ( idInteraction* )1;

//! Lists the render light definitions and their interaction and reference counts.
void						R_ListRenderLightDefs_f( const idCmdArgs& args );

//! Lists all render entity definitions in the primary world with their interaction and reference counts
void						R_ListRenderEntityDefs_f( const idCmdArgs& args );

#endif /* !__RENDERWORLDLOCAL_H__ */
