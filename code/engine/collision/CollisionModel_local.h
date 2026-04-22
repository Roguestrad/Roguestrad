/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2015 Robert Beckebans

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

/*
===============================================================================

	Trace model vs. polygonal model collision detection.

===============================================================================
*/

#include "CollisionModel.h"

#define MIN_NODE_SIZE				64.0f
#define MAX_NODE_POLYGONS			128
#define CM_MAX_POLYGON_EDGES		64
#define CIRCLE_APPROXIMATION_LENGTH 64.0f

#define MAX_SUBMODELS				2048
#define TRACE_MODEL_HANDLE			MAX_SUBMODELS

#define VERTEX_HASH_BOXSIZE			( 1 << 6 ) // must be power of 2
#define VERTEX_HASH_SIZE			( VERTEX_HASH_BOXSIZE * VERTEX_HASH_BOXSIZE )
#define EDGE_HASH_SIZE				( 1 << 14 )

#define NODE_BLOCK_SIZE_SMALL		8
#define NODE_BLOCK_SIZE_LARGE		256
#define REFERENCE_BLOCK_SIZE_SMALL	8
#define REFERENCE_BLOCK_SIZE_LARGE	256

#define MAX_WINDING_LIST			128 // quite a few are generated at times
#define INTEGRAL_EPSILON			0.01f
#define VERTEX_EPSILON				0.1f
#define CHOP_EPSILON				0.1f

typedef struct cm_windingList_s {
	int			   numWindings;			// number of windings
	idFixedWinding w[MAX_WINDING_LIST]; // windings
	idVec3		   normal;				// normal for all windings
	idBounds	   bounds;				// bounds of all windings in list
	idVec3		   origin;				// origin for radius
	float		   radius;				// radius relative to origin for all windings
	int			   contents;			// winding surface contents
	int			   primitiveNum;		// number of primitive the windings came from
} cm_windingList_t;

/*
===============================================================================

Collision model

===============================================================================
*/

typedef struct cm_vertex_s {
	idVec3		 p;			 // vertex point
	int			 checkcount; // for multi-check avoidance
	// DG: use int instead of long for 64bit compatibility
	unsigned int side;	  // each bit tells at which side this vertex passes one of the trace model edges
	unsigned int sideSet; // each bit tells if sidedness for the trace model edge has been calculated yet
						  // DG end
} cm_vertex_t;

typedef struct cm_edge_s {
	int			   checkcount; // for multi-check avoidance
	unsigned short internal;   // a trace model can never collide with internal edges
	unsigned short numUsers;   // number of polygons using this edge
	// DG: use int instead of long for 64bit compatibility
	unsigned int   side;	// each bit tells at which side of this edge one of the trace model vertices passes
	unsigned int   sideSet; // each bit tells if sidedness for the trace model vertex has been calculated yet
	// DG end
	int			   vertexNum[2]; // start and end point of edge
	idVec3		   normal;		 // edge normal
} cm_edge_t;

typedef struct cm_polygonBlock_s {
	int	  bytesRemaining;
	byte* next;
} cm_polygonBlock_t;

typedef struct cm_polygon_s {
	idBounds		  bounds;	  // polygon bounds
	int				  checkcount; // for multi-check avoidance
	int				  contents;	  // contents behind polygon
	const idMaterial* material;	  // material
	idPlane			  plane;	  // polygon plane
	int				  numEdges;	  // number of edges
	int				  edges[1];	  // variable sized, indexes into cm_edge_t list
} cm_polygon_t;

typedef struct cm_polygonRef_s {
	cm_polygon_t*			p;	  // pointer to polygon
	struct cm_polygonRef_s* next; // next polygon in chain
} cm_polygonRef_t;

typedef struct cm_polygonRefBlock_s {
	cm_polygonRef_t*			 nextRef; // next polygon reference in block
	struct cm_polygonRefBlock_s* next;	  // next block with polygon references
} cm_polygonRefBlock_t;

typedef struct cm_brushBlock_s {
	int	  bytesRemaining;
	byte* next;
} cm_brushBlock_t;

typedef struct cm_brush_s {
	//! Initializes a new instance of the cm_brush_s structure with default values.
	cm_brush_s()
	{
		checkcount	 = 0;
		contents	 = 0;
		material	 = NULL;
		primitiveNum = 0;
		numPlanes	 = 0;
	}
	int				  checkcount;	// for multi-check avoidance
	idBounds		  bounds;		// brush bounds
	int				  contents;		// contents of brush
	const idMaterial* material;		// material
	int				  primitiveNum; // number of brush primitive
	int				  numPlanes;	// number of bounding planes
	idPlane			  planes[1];	// variable sized
} cm_brush_t;

typedef struct cm_brushRef_s {
	cm_brush_t*			  b;	// pointer to brush
	struct cm_brushRef_s* next; // next brush in chain
} cm_brushRef_t;

typedef struct cm_brushRefBlock_s {
	cm_brushRef_t*			   nextRef; // next brush reference in block
	struct cm_brushRefBlock_s* next;	// next block with brush references
} cm_brushRefBlock_t;

typedef struct cm_node_s {
	int				  planeType;   // node axial plane type
	float			  planeDist;   // node plane distance
	cm_polygonRef_t*  polygons;	   // polygons in node
	cm_brushRef_t*	  brushes;	   // brushes in node
	struct cm_node_s* parent;	   // parent of this node
	struct cm_node_s* children[2]; // node children
} cm_node_t;

typedef struct cm_nodeBlock_s {
	cm_node_t*			   nextNode; // next node in block
	struct cm_nodeBlock_s* next;	 // next block with nodes
} cm_nodeBlock_t;

typedef struct cm_model_s {
	idStr				  name;		// model name
	idBounds			  bounds;	// model bounds
	int					  contents; // all contents of the model ored together
	bool				  isConvex; // set if model is convex
	// model geometry
	int					  maxVertices; // size of vertex array
	int					  numVertices; // number of vertices
	cm_vertex_t*		  vertices;	   // array with all vertices used by the model
	int					  maxEdges;	   // size of edge array
	int					  numEdges;	   // number of edges
	cm_edge_t*			  edges;	   // array with all edges used by the model
	cm_node_t*			  node;		   // first node of spatial subdivision
	// blocks with allocated memory
	cm_nodeBlock_t*		  nodeBlocks;		// list with blocks of nodes
	cm_polygonRefBlock_t* polygonRefBlocks; // list with blocks of polygon references
	cm_brushRefBlock_t*	  brushRefBlocks;	// list with blocks of brush references
	cm_polygonBlock_t*	  polygonBlock;		// memory block with all polygons
	cm_brushBlock_t*	  brushBlock;		// memory block with all brushes
	// statistics
	int					  numPolygons;
	int					  polygonMemory;
	int					  numBrushes;
	int					  brushMemory;
	int					  numNodes;
	int					  numBrushRefs;
	int					  numPolygonRefs;
	int					  numInternalEdges;
	int					  numSharpEdges;
	int					  numRemovedPolys;
	int					  numMergedPolys;
	int					  usedMemory;
} cm_model_t;

/*
===============================================================================

Data used during collision detection calculations

===============================================================================
*/

typedef struct cm_trmVertex_s {
	int		   used;		   // true if this vertex is used for collision detection
	idVec3	   p;			   // vertex position
	idVec3	   endp;		   // end point of vertex after movement
	int		   polygonSide;	   // side of polygon this vertex is on (rotational collision)
	idPluecker pl;			   // pluecker coordinate for vertex movement
	idVec3	   rotationOrigin; // rotation origin for this vertex
	idBounds   rotationBounds; // rotation bounds for this vertex
} cm_trmVertex_t;

typedef struct cm_trmEdge_s {
	int			   used;		   // true when vertex is used for collision detection
	idVec3		   start;		   // start of edge
	idVec3		   end;			   // end of edge
	int			   vertexNum[2];   // indexes into cm_traceWork_t->vertices
	idPluecker	   pl;			   // pluecker coordinate for edge
	idVec3		   cross;		   // (z,-y,x) of cross product between edge dir and movement dir
	idBounds	   rotationBounds; // rotation bounds for this edge
	idPluecker	   plzaxis;		   // pluecker coordinate for rotation about the z-axis
	unsigned short bitNum;		   // vertex bit number
} cm_trmEdge_t;

typedef struct cm_trmPolygon_s {
	int		 used;
	idPlane	 plane;							  // polygon plane
	int		 numEdges;						  // number of edges
	int		 edges[MAX_TRACEMODEL_POLYEDGES]; // index into cm_traceWork_t->edges
	idBounds rotationBounds;				  // rotation bounds for this polygon
} cm_trmPolygon_t;

typedef struct cm_traceWork_s {
	int				numVerts;
	cm_trmVertex_t	vertices[MAX_TRACEMODEL_VERTS]; // trm vertices
	int				numEdges;
	cm_trmEdge_t	edges[MAX_TRACEMODEL_EDGES + 1]; // trm edges
	int				numPolys;
	cm_trmPolygon_t polys[MAX_TRACEMODEL_POLYS]; // trm polygons
	cm_model_t*		model;						 // model colliding with
	idVec3			start;						 // start of trace
	idVec3			end;						 // end of trace
	idVec3			dir;						 // trace direction
	idBounds		bounds;						 // bounds of full trace
	idBounds		size;						 // bounds of transformed trm relative to start
	idVec3			extents;					 // largest of abs(size[0]) and abs(size[1]) for BSP trace
	int				contents;					 // ignore polygons that do not have any of these contents flags
	trace_t			trace;						 // collision detection result

	bool			rotation;		   // true if calculating rotational collision
	bool			pointTrace;		   // true if only tracing a point
	bool			positionTest;	   // true if not tracing but doing a position test
	bool			isConvex;		   // true if the trace model is convex
	bool			axisIntersectsTrm; // true if the rotation axis intersects the trace model
	bool			getContacts;	   // true if retrieving contacts
	bool			quickExit;		   // set to quickly stop the collision detection calculations

	idVec3			origin;				 // origin of rotation in model space
	idVec3			axis;				 // rotation axis in model space
	idMat3			matrix;				 // rotates axis of rotation to the z-axis
	float			angle;				 // angle for rotational collision
	float			maxTan;				 // max tangent of half the positive angle used instead of fraction
	float			radius;				 // rotation radius of trm start
	idRotation		modelVertexRotation; // inverse rotation for model vertices

	contactInfo_t*	contacts;	 // array with contacts
	int				maxContacts; // max size of contact array
	int				numContacts; // number of contacts found

	idPlane			heartPlane1; // polygons should be near anough the trace heart planes
	float			maxDistFromHeartPlane1;
	idPlane			heartPlane2;
	float			maxDistFromHeartPlane2;
	idPluecker		polygonEdgePlueckerCache[CM_MAX_POLYGON_EDGES];
	idPluecker		polygonVertexPlueckerCache[CM_MAX_POLYGON_EDGES];
	idVec3			polygonRotationOriginCache[CM_MAX_POLYGON_EDGES];
} cm_traceWork_t;

/*
===============================================================================

Collision Map

===============================================================================
*/

typedef struct cm_procNode_s {
	idPlane plane;
	int		children[2]; // negative numbers are (-1 - areaNumber), 0 = solid
} cm_procNode_t;

/*!
	\class idCollisionModelManagerLocal
	\brief Manages collision models for a map-based 3D environment with support for loading, tracing, and rendering.

	This class serves as the primary interface for collision detection within a 3D environment, handling the management of collision models derived from map files. It provides functionality for
   loading, preprocessing, and efficiently tracing collision models against various geometric primitives. The class supports both static and dynamic collision detection through translation and
   rotation operations, allowing for accurate physics simulation and spatial queries. It also includes utilities for model conversion, memory management, and visualization of collision geometry.

*/
class idCollisionModelManagerLocal : public idCollisionModelManager
{
public:
	//! Loads collision models from a map file
	void		LoadMap( const idMapFile* mapFile, bool ignoreOldCollisionFile );

	//! Frees all collision models and cleans up associated resources.
	void		FreeMap();

	//! Preloads collision models for a specified map by processing a preload manifest file.
	void		Preload( const char* mapName );

	//! Loads a collision model by name, returning its handle.
	cmHandle_t	LoadModel( const char* modelName, const bool precache );

	//! Sets up a trace model for collision with other trace models.
	cmHandle_t	SetupTrmModel( const idTraceModel& trm, const idMaterial* material );

	//! Creates a trace model from a collision model and returns true if successful.
	bool		TrmFromModel( const char* modelName, idTraceModel& trm );

	//! Returns the name of the collision model associated with the given handle
	const char* GetModelName( cmHandle_t model ) const;

	//! Retrieves the bounding box of a specified collision model.
	bool		GetModelBounds( cmHandle_t model, idBounds& bounds ) const;

	//! Retrieves the contents flags of a collision model by combining all brush and polygon contents.
	bool		GetModelContents( cmHandle_t model, int& contents ) const;

	//! Retrieves a vertex from a collision model by its handle and vertex number.
	bool		GetModelVertex( cmHandle_t model, int vertexNum, idVec3& vertex ) const;

	/*!
		\brief Retrieves the start and end vertices of a specified edge from a collision model.

		This function fetches the geometric representation of a given edge from a collision model. It takes a model handle and an edge number, then populates the provided vectors with the coordinates
	   of the edge's start and end vertices. The function performs validation on the model handle and edge number to ensure they are within valid ranges. If the input parameters are invalid, an error
	   message is printed and the function returns false. Otherwise, it returns true after successfully retrieving the edge data.

		\param model Handle to the collision model from which to retrieve the edge
		\param edgeNum Index of the edge to retrieve, negative values are converted to positive
		\param start Output vector that will contain the start vertex of the edge
		\param end Output vector that will contain the end vertex of the edge
		\return True if the edge data was successfully retrieved, false otherwise.
		\throws This function does not explicitly throw exceptions, but may print error messages to the console if invalid parameters are provided.
	*/
	bool		GetModelEdge( cmHandle_t model, int edgeNum, idVec3& start, idVec3& end ) const;

	//! Retrieves the polygon data for a specified model and polygon number
	bool		GetModelPolygon( cmHandle_t model, int polygonNum, idFixedWinding& winding ) const;

	/*!
		\brief Translates a trace model from a start to end position and reports the first collision if any

		Performs a collision trace of a trace model moving from a start to end position relative to a collision model. The function handles both rotated and non-rotated models and trace models,
	   applying appropriate transformations. It computes the intersection between the moving trace model and the static collision model, returning detailed contact information including the fraction
	   of traversal, collision point, and surface normal. Special handling is included for point traces and cases where the trace distance exceeds a maximum threshold.

		\param results Output structure containing the trace results including fraction, end position, and contact information
		\param start Starting position of the trace model
		\param end Ending position of the trace model
		\param trm Trace model to be translated
		\param trmAxis Axis of the trace model for rotation
		\param contentMask Mask of content types to be checked for collision
		\param model Handle to the collision model to be traced against
		\param modelOrigin Origin of the collision model
		\param modelAxis Axis of the collision model for rotation
	*/
	void		Translation( trace_t* results,
			   const idVec3&		  start,
			   const idVec3&		  end,
			   const idTraceModel*	  trm,
			   const idMat3&		  trmAxis,
			   int					  contentMask,
			   cmHandle_t			  model,
			   const idVec3&		  modelOrigin,
			   const idMat3&		  modelAxis );

	/*!
		\brief Rotates a trace model and reports the first collision if any

		This function performs a rotation trace of a collision model using a specified rotation. It handles special cases such as zero angle rotations by delegating to the contents function, and
	   manages large rotations by breaking them into smaller steps. The function updates the results parameter with collision information, including the fraction of the rotation completed before
	   collision, and the end position and axis of the trace model.

		\param results Output structure containing the collision results
		\param start Starting position of the trace model
		\param rotation Rotation to apply to the trace model
		\param trm Trace model to rotate
		\param trmAxis Initial axis of the trace model
		\param contentMask Mask of content types to test against
		\param model Handle to the collision model to test against
		\param modelOrigin Origin of the collision model
		\param modelAxis Axis of the collision model
		\throws assertion failures if the start or trmAxis parameters overlap with the results buffer, or if the function is called with invalid parameters
	*/
	void		Rotation( trace_t* results,
			   const idVec3&	   start,
			   const idRotation&   rotation,
			   const idTraceModel* trm,
			   const idMat3&	   trmAxis,
			   int				   contentMask,
			   cmHandle_t		   model,
			   const idVec3&	   modelOrigin,
			   const idMat3&	   modelAxis );

	//! Returns the contents of the collision model at the specified point.
	int			PointContents( const idVec3 p, cmHandle_t model );

	/*!
		\brief Returns the contents the trace model is stuck in or 0 if the trace model is in free space

		This function determines the contents of the world that a trace model occupies at a given position and orientation. It performs validation on the model handle and checks if the model is valid
	   before proceeding with the contents check. The function uses the ContentsTrm helper to perform the actual tracing operation.

		\param start Starting position of the trace model
		\param trm Trace model to check contents for
		\param trmAxis Axis orientation of the trace model
		\param contentMask Mask to filter content types
		\param model Handle to the collision model to test against
		\param modelOrigin Origin of the model in world space
		\param modelAxis Axis orientation of the model in world space
		\return The contents bitmask of the trace model at the given position, or 0 if in free space
	*/
	int			Contents( const idVec3& start, const idTraceModel* trm, const idMat3& trmAxis, int contentMask, cmHandle_t model, const idVec3& modelOrigin, const idMat3& modelAxis );

	/*!
		\brief Stores all contact points of the trace model with the specified model and returns the number of contacts.

		This function traces a collision model along a directional path and collects all contact points encountered. It uses the provided start position, direction vector, and depth to determine the
	   end position of the trace. The function then performs a translation operation to detect all collisions, storing contact information in the provided array. The rotational component of the
	   direction vector is currently not handled.

		\param contacts Array to store contact information for all collisions detected
		\param maxContacts Maximum number of contacts to store in the contacts array
		\param start Starting position of the trace
		\param dir Direction vector for the trace, where the first 3 components represent linear motion and the last 3 represent rotational motion
		\param depth Maximum distance to trace
		\param trm Trace model used for collision detection
		\param trmAxis Axis orientation of the trace model
		\param contentMask Content mask to filter which surfaces are considered
		\param model Handle to the collision model to test against
		\param modelOrigin Origin of the collision model
		\param modelAxis Axis orientation of the collision model
		\return Number of contacts detected during the trace operation
	*/
	int			Contacts( contactInfo_t* contacts,
				const int				 maxContacts,
				const idVec3&			 start,
				const idVec6&			 dir,
				const float				 depth,
				const idTraceModel*		 trm,
				const idMat3&			 trmAxis,
				int						 contentMask,
				cmHandle_t				 model,
				const idVec3&			 modelOrigin,
				const idMat3&			 modelAxis );

	//! Tests collision detection and outputs performance statistics for translation and rotation operations.
	void		DebugOutput( const idVec3& origin );

	/*!
		\brief Draws a collision model with the specified origin, axis, and view parameters.

		This function renders a collision model using the provided model handle, origin, axis, view origin, and radius. It first validates the model handle against the valid range of models. If the
	   draw color has been modified, it updates the color from the configuration. The function calculates the view position relative to the model origin and axis, increments a check counter, and then
	   draws the model's polygons using the node structure.

		\param model Handle to the collision model to draw
		\param origin The origin position of the model in world space
		\param axis The orientation axis of the model in world space
		\param viewOrigin The origin of the viewer or camera in world space
		\param radius The radius of the view frustum or drawing area
	*/
	void		DrawModel( cmHandle_t model, const idVec3& origin, const idMat3& axis, const idVec3& viewOrigin, const float radius );

	//! Prints information about the specified collision model, or accumulated model information if the handle is -1.
	void		ModelInfo( cmHandle_t model );

	//! Lists all loaded collision models and their memory usage.
	void		ListModels();

	//! Writes a collision model file for the given map entity
	bool		WriteCollisionModelForMapEntity( const idMapEntity* mapEnt, const char* filename, const bool testTraceModel = true );

private:
	// CollisionMap_translate.cpp ----------------------------

	/*!
		\brief Calculates the fraction of translation at which two edges collide using Pluecker coordinates.

		This function determines the time fraction along a movement vector at which two edges, represented as Pluecker coordinates, will intersect. It is used in collision detection to compute when a
	moving edge intersects with a static edge. The function returns true if a collision is possible and sets the fraction parameter to the time of intersection, or false if the edges do not
	collide. The cross parameter is used to store intermediate cross product results during computation.

		\param cross Stores the cross product of the direction vector and the difference between edge endpoints
		\param l1 Pluecker coordinate representing the first edge (the moving edge)
		\param l2 Pluecker coordinate representing the second edge (the static edge)
		\param fraction Pointer to a float that will store the fraction of movement at which collision occurs
		\return Integer indicating whether a collision was detected; true if collision occurs, false otherwise
	*/
	int	 TranslateEdgeThroughEdge( idVec3& cross, idPluecker& l1, idPluecker& l2, float* fraction );

	//! Processes a trace model edge through a polygon to detect and record collision information
	void TranslateTrmEdgeThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* poly, cm_trmEdge_t* trmEdge );

	/*!
		\brief Updates the collision trace fraction and contact information when a trace vertex moves through a polygon

		This function determines if a trace vertex traveling from its start position to its end position intersects with a polygon. If an intersection occurs and the hit fraction is closer than the
	   current trace fraction, the function updates the trace information including the collision normal, distance, contents, material, and contact points. The function also handles contact generation
	   when needed. The bitNum parameter is used to track sidedness of edges during the collision test.

		\param tw Trace work structure containing model and trace state
		\param poly Polygon being tested against
		\param v Trace vertex being tested
		\param bitNum Bit number used to track edge sidedness
	*/
	void TranslateTrmVertexThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* poly, cm_trmVertex_t* v, int bitNum );

	//! Updates the trace fraction and contact information when a translation trace intersects with a polygon.
	void TranslatePointThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* poly, cm_trmVertex_t* v );

	/*!
		\brief Processes the translation of a vertex through a triangle mesh polygon during collision detection

		This function determines if a vertex translation intersects with a triangle mesh polygon and updates the trace results accordingly. It calculates the fraction of the translation that occurs
	   before collision, checks edge sidedness to ensure proper intersection detection, and sets up the contact information when a collision occurs. The function is used in the context of physics and
	   collision detection for 3D models.

		\param tw Trace work structure containing the current trace state and model data
		\param trmpoly Pointer to the triangle mesh polygon being tested against
		\param poly Pointer to the polygon structure containing material and content information
		\param v Pointer to the vertex being translated
		\param endp Ending position of the vertex translation
		\param pl Pluecker coordinate of the vertex for sidedness calculations
	*/
	void TranslateVertexThroughTrmPolygon( cm_traceWork_t* tw, cm_trmPolygon_t* trmpoly, cm_polygon_t* poly, cm_vertex_t* v, idVec3& endp, idPluecker& pl );

	//! Checks if a polygon blocks a translation trace and updates the trace information accordingly
	bool TranslateTrmThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* p );

	//! Sets up the heart planes for a trace based on the translation direction.
	void SetupTranslationHeartPlanes( cm_traceWork_t* tw );

	//! Initializes trace work data structures from a trace model.
	void SetupTrm( cm_traceWork_t* tw, const idTraceModel* trm );

private:
	// CollisionMap_rotate.cpp -------------------------

	/*!
		\brief Determines if a collision occurs between two edges within their bounds and calculates the collision point and normal.

		This function verifies whether two edges, defined by their endpoints, collide within their respective bounds. It takes into account edge rotation based on a specified tangent angle. If a
	collision is detected, the function computes the exact point of collision and the normal vector at that point. The function uses Pluecker coordinates to perform geometric calculations for edge
	intersections. It returns true if a collision is found within the bounds, false otherwise.

		\param tw Trace work structure containing origin, axis, and angle information for the collision test
		\param va Start point of the first edge
		\param vb End point of the first edge
		\param vc Start point of the second edge
		\param vd End point of the second edge
		\param tanHalfAngle Tangent of half the angle for edge rotation, used to expand the edge bounds
		\param collisionPoint Output parameter for the point of collision
		\param collisionNormal Output parameter for the normal vector at the collision point
		\return True if a collision occurs within the edge bounds, false otherwise
	*/
	int	 CollisionBetweenEdgeBounds( cm_traceWork_t* tw, const idVec3& va, const idVec3& vb, const idVec3& vc, const idVec3& vd, float tanHalfAngle, idVec3& collisionPoint, idVec3& collisionNormal );

	/*!
		\brief Computes the tangent of half the rotation angle at which two edges collide during a rotational trace operation

		This function calculates the tangent of half the rotation angle for a rotating edge to collide with another edge. It transforms the rotation axis to the z-axis, computes pluecker coordinates
	   for both edges, and solves a quadratic equation to find the rotation angles. The function returns the smallest tangent value that results in a collision, considering a minimum tangent
	   threshold. It's used in collision detection for rotational movements in the engine's physics system.

		\param tw Trace work structure containing rotation information and state
		\param pl1 Pluecker coordinate for the rotating edge
		\param vc Start point of the edge being tested for collision
		\param vd End point of the edge being tested for collision
		\param minTan Minimum tangent value to consider for collision
		\param tanHalfAngle Output parameter for the computed tangent of half the rotation angle
		\return Integer result indicating success (true) or failure (false) of the computation
	*/
	int	 RotateEdgeThroughEdge( cm_traceWork_t* tw, const idPluecker& pl1, const idVec3& vc, const idVec3& vd, const float minTan, float& tanHalfAngle );

	/*!
		\brief Calculates the direction of motion at the initial position and determines the tangent of half the rotation angle at which edges are furthest apart.

		This function computes the direction of motion for edges during rotation and determines the maximum separation angle between two edges. It uses Pluecker coordinates to represent edges and
	   performs computations in a transformed coordinate system. The function returns true if a valid separation angle is found, false otherwise. The direction parameter indicates whether edges are
	   moving towards or away from each other at the initial position, while the tangent of half-angle represents the maximum separation during rotation. The function handles edge cases where edges
	   may be moving towards each other or where no valid separation angle exists.

		\param tw trace work structure containing model transformation and rotation data
		\param pl1 Pluecker coordinate of the first edge
		\param vc Starting vertex position of the first edge
		\param vd Ending vertex position of the first edge
		\param tanHalfAngle Output parameter for the tangent of half the rotation angle at maximum separation
		\param dir Output parameter for the direction of motion at initial position
		\return Integer value indicating success (true) or failure (false) of the calculation
	*/
	int	 EdgeFurthestFromEdge( cm_traceWork_t* tw, const idPluecker& pl1, const idVec3& vc, const idVec3& vd, float& tanHalfAngle, float& dir );

	//! Rotates a trace model edge through a polygon to detect collisions.
	void RotateTrmEdgeThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* poly, cm_trmEdge_t* trmEdge );

	/*!
		\brief Calculates the tangent of half the rotation angle at which a point collides with a plane during a rotation.

		This function determines the smallest tangent value of half the rotation angle that results in a collision between a point and a plane. It transforms the point and plane into a coordinate
	   system aligned with the rotation axis, then solves a quadratic equation to find the possible rotation angles. The function returns the smallest valid tangent value that satisfies the collision
	   criteria. The calculation takes into account the direction of rotation and ensures the result is within acceptable bounds.

		\param tw trace work structure containing rotation information and transformation matrix
		\param point the point to be rotated and tested for collision
		\param plane the plane against which collision is tested
		\param angle the total rotation angle
		\param minTan minimum allowed tangent value for the result
		\param tanHalfAngle output parameter containing the calculated tangent of half the rotation angle
		\return 1 if a valid collision tangent is found, 0 otherwise
	*/
	int	 RotatePointThroughPlane( const cm_traceWork_t* tw, const idVec3& point, const idPlane& plane, const float angle, const float minTan, float& tanHalfAngle );

	/*!
		\brief Calculates the direction of motion at the initial position and determines the tangent of half the rotation angle at which a point is furthest from a plane

		This function computes whether a point is moving towards or away from a plane and, if moving away, calculates the tangent of half the rotation angle at which the point reaches maximum distance
	   from the plane. It transforms the point and plane into a local coordinate system defined by the trace work parameters. The calculation involves solving a quadratic equation derived from
	   trigonometric relationships. The function returns true if the calculation is successful, false otherwise. The dir parameter indicates the direction of motion, where negative values mean
	   movement towards the plane.

		\param tw Trace work structure containing transformation matrix and origin for coordinate system
		\param point Input point in global coordinates to analyze
		\param plane Plane definition in global coordinates
		\param angle Rotation angle for the current trace operation
		\param tanHalfAngle Output parameter for tangent of half rotation angle at maximum distance
		\param dir Output parameter for direction of motion at initial position
		\return Integer value indicating success (1) or failure (0) of the calculation
	*/
	int	 PointFurthestFromPlane( const cm_traceWork_t* tw, const idVec3& point, const idPlane& plane, const float angle, float& tanHalfAngle, float& dir );

	/*!
		\brief Rotates a point through an epsilon-expanded plane during collision detection, calculating the tangent of half the rotation and collision point.

		This function performs rotational collision detection for a vertex moving along a rotation axis. It determines whether the vertex intersects with an epsilon-expanded plane during rotation and
	   calculates the appropriate collision parameters. The function handles cases where the vertex movement is away from or toward the polygon plane, ensuring proper collision detection for convex
	   polygons. It returns false when no collision occurs or when the rotation angle exceeds the maximum allowed tangent value, otherwise returning true and updating the collision point and
	   direction.

		\param tw trace work structure containing rotation and collision data
		\param point starting position of the vertex
		\param endPoint final position of the vertex
		\param plane polygon plane for collision detection
		\param angle rotation angle in radians
		\param origin rotation origin point
		\param tanHalfAngle output parameter for tangent of half rotation angle
		\param collisionPoint output parameter for the calculated collision point
		\param endDir output parameter for direction of motion at collision point
		\return 1 if a collision occurs and parameters are properly calculated, 0 if no collision occurs
	*/
	int	 RotatePointThroughEpsilonPlane( const cm_traceWork_t* tw,
		 const idVec3&										   point,
		 const idVec3&										   endPoint,
		 const idPlane&										   plane,
		 const float										   angle,
		 const idVec3&										   origin,
		 float&												   tanHalfAngle,
		 idVec3&											   collisionPoint,
		 idVec3&											   endDir );

	/*!
		\brief Rotates a trace model vertex through a polygon to check for collision and updates the trace result if a valid intersection is found.

		This function performs a collision check between a trace model vertex and a polygon by rotating the vertex through the polygon's plane. It first verifies that the vertex is within the
	   polygon's bounds and that the rotation bounds intersect with the polygon. It then determines the collision point by rotating the vertex through an epsilon plane, and checks if the resulting
	   point lies within the polygon's edges using Pluecker coordinates. If all conditions are met, the trace result is updated with the collision data including normal, distance, contents, material,
	   and feature information. The function updates the maximum tangent value to determine the earliest collision.

		\param tw Pointer to the trace work structure containing the current trace state and configuration
		\param poly Pointer to the polygon structure being tested against
		\param v Pointer to the trace model vertex being rotated
		\param vertexNum Index of the vertex within the trace model
	*/
	void RotateTrmVertexThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* poly, cm_trmVertex_t* v, int vertexNum );

	/*!
		\brief Rotates a vertex through a TRM polygon to test for collisions within rotation bounds

		This function performs a collision test by rotating a vertex through a TRM polygon using the specified rotation parameters. It first checks if the vertex is within the valid rotation bounds
	   and not behind the TRM plane. It then calculates the end position of the vertex after rotation and tests if the collision point lies within the polygon edges. If the test passes, it updates the
	   trace information with collision details including normal, distance, contents, material, and feature indices. The function uses plucker coordinates to efficiently test edge intersections.

		\param tw trace work structure containing rotation and collision data
		\param trmpoly TRM polygon to test against
		\param poly polygon containing collision information
		\param v vertex to rotate and test
		\param rotationOrigin origin point for the rotation around the axis
	*/
	void RotateVertexThroughTrmPolygon( cm_traceWork_t* tw, cm_trmPolygon_t* trmpoly, cm_polygon_t* poly, cm_vertex_t* v, idVec3& rotationOrigin );

	//! Returns true if the polygon blocks the complete rotation of the trace model.
	bool RotateTrmThroughPolygon( cm_traceWork_t* tw, cm_polygon_t* p );

	/*!
		\brief Computes the bounding box for a rotation around an origin axis from a start to an end point

		This function calculates the axis-aligned bounding box that encompasses the volume swept by a rotation from a start point to an end point around a given origin and axis. The rotation is
	   assumed to be less than 180 degrees. The calculation accounts for the changing derivative along each axis during the rotation and expands the bounds to include collision margins. The function
	   works by determining the radius squared from the origin to the start point, computing cross products of the start and end vectors with the rotation axis, and then iterating over each axis to
	   determine the appropriate bounds based on the sign changes and relative positions of the start and end points.

		\param origin The center point around which the rotation occurs
		\param axis The axis of rotation
		\param start The starting point of the rotation
		\param end The ending point of the rotation
		\param bounds The output bounding box that will contain the swept volume
	*/
	void BoundsForRotation( const idVec3& origin, const idVec3& axis, const idVec3& start, const idVec3& end, idBounds& bounds );

	/*!
		\brief Performs a traced rotation collision test for a model around an axis, calculating the collision results between the rotation start and end positions.

		This function handles the collision detection for a rotating trace model around a specified axis. It calculates the rotation bounds and performs a trace through the collision model to detect
	   any collisions during the rotation. The function supports both point traces and full trace models, handling the necessary transformations for rotated models and trace models. It accounts for
	   the maximum error in circle approximation during the BSP tree traversal and computes the final collision results including fraction, normal, and point of contact.

		\param results Output structure containing the collision trace results
		\param rorg Rotation origin point for the axis of rotation
		\param axis Normalized axis of rotation
		\param startAngle Starting angle of the rotation in degrees
		\param endAngle Ending angle of the rotation in degrees
		\param start Starting position of the trace model
		\param trm Pointer to the trace model to use for the rotation test
		\param trmAxis Axis transformation for the trace model
		\param contentMask Collision contents mask to filter which surfaces to test against
		\param model Handle to the collision model to test against
		\param origin Origin of the model in world space
		\param modelAxis Axis transformation for the model
		\throws Asserts if the model handle is invalid or out of bounds.
	*/
	void Rotation180( trace_t* results,
		const idVec3&		   rorg,
		const idVec3&		   axis,
		const float			   startAngle,
		const float			   endAngle,
		const idVec3&		   start,
		const idTraceModel*	   trm,
		const idMat3&		   trmAxis,
		int					   contentMask,
		cmHandle_t			   model,
		const idVec3&		   origin,
		const idMat3&		   modelAxis );

private:
	// CollisionMap_contents.cpp -------------------------

	//! Tests if any of the trace model vertices are inside the given brush
	bool	   TestTrmVertsInBrush( cm_traceWork_t* tw, cm_brush_t* b );

	//! Tests if a trace model intersects with a polygon and returns true if they do.
	bool	   TestTrmInPolygon( cm_traceWork_t* tw, cm_polygon_t* p );

	//! Finds the leaf node in a collision model that contains the specified point.
	cm_node_t* PointNode( const idVec3& p, cm_model_t* model );

	/*!
		\brief Returns the contents of the collision model at a transformed point

		This function calculates the contents at a point in a collision model after applying the specified origin offset and rotation axis transformations. It first subtracts the model origin from the
	   input point, then applies the model axis transformation if the axis is rotated, and finally queries the contents at that transformed point using the standard point contents function

		\param p The input point in world space coordinates
		\param model Handle to the collision model to test against
		\param origin The origin offset to apply to the point
		\param modelAxis The rotation axis to apply to the point
		\return The contents bitmask at the transformed point within the specified collision model
	*/
	int		   TransformedPointContents( const idVec3& p, cmHandle_t model, const idVec3& origin, const idMat3& modelAxis );

	/*!
		\brief Tests if a trace model intersects with a collision model at a specified position and orientation, returning the contents of the intersected geometry.

		This function performs a point or trace model contents test against a collision model. It handles both rotated and non-rotated trace models and collision models. The function calculates the
	   position and orientation of the trace model relative to the collision model, transforms the vertices and polygons accordingly, and then traces through the model to determine which contents are
	   intersected. It supports convex trace models and returns the contents bitmask of the intersected geometry.

		\param results Pointer to a trace_t structure to store the results of the contents test
		\param start The starting position of the trace model
		\param trm Pointer to the trace model to test against the collision model
		\param trmAxis The axis orientation of the trace model
		\param contentMask Bitmask of content types to test for collision
		\param model Handle to the collision model to test against
		\param modelOrigin The origin position of the collision model
		\param modelAxis The axis orientation of the collision model
		\return An integer representing the contents bitmask of the intersected geometry, or 0 if no intersection occurred
	*/
	int ContentsTrm( trace_t* results, const idVec3& start, const idTraceModel* trm, const idMat3& trmAxis, int contentMask, cmHandle_t model, const idVec3& modelOrigin, const idMat3& modelAxis );

private:
	// CollisionMap_trace.cpp -------------------------

	//! Tests collision for a trace model through a collision node
	void TraceTrmThroughNode( cm_traceWork_t* tw, cm_node_t* node );

	/*!
		\brief Recursively traces a collision trace through an axial BSP tree node structure to determine collision points and fractions.

		This function performs a recursive traversal of an axial BSP tree to test for collisions along a trace path. It handles both the testing of collision data at nodes and the subdivision of the
	   trace path based on the BSP tree structure. The function considers the position test flag and quick exit conditions to optimize performance. It calculates intersection points with the BSP tree
	   nodes and updates the trace information accordingly.

		\param tw Trace work structure containing the collision data and state
		\param node Current BSP tree node being processed
		\param p1f Start fraction of the trace
		\param p2f End fraction of the trace
		\param p1 Start position of the trace
		\param p2 End position of the trace
	*/
	void TraceThroughAxialBSPTree_r( cm_traceWork_t* tw, cm_node_t* node, float p1f, float p2f, idVec3& p1, idVec3& p2 );

	//! Traces a collision model through a BSP tree for a given trace work structure.
	void TraceThroughModel( cm_traceWork_t* tw );
	void RecurseProcBSP_r( trace_t* results, int parentNodeNum, int nodeNum, float p1f, float p2f, const idVec3& p1, const idVec3& p2 );

private:
	// CollisionMap_load.cpp -------------------------

	//! Clears all collision model manager data and resets internal state.
	void			 Clear();

	//! Frees the trace model structure memory.
	void			 FreeTrmModelStructure();

	//! Removes references to a polygon from the collision model tree starting at the given node.
	void			 RemovePolygonReferences_r( cm_node_t* node, cm_polygon_t* p );

	//! Removes references to a specific brush from the collision model tree starting at the given node.
	void			 RemoveBrushReferences_r( cm_node_t* node, cm_brush_t* b );

	//! Frees the memory associated with a collision model node.
	void			 FreeNode( cm_node_t* node );

	//! Frees a polygon reference by doing nothing.
	void			 FreePolygonReference( cm_polygonRef_t* pref );

	//! Does not free the brush reference as it is allocated in blocks that are freed when the model is freed.
	void			 FreeBrushReference( cm_brushRef_t* bref );

	//! Frees a collision polygon from the specified model, updating memory tracking and allocation.
	void			 FreePolygon( cm_model_t* model, cm_polygon_t* poly );

	//! Frees a brush from a collision model.
	void			 FreeBrush( cm_model_t* model, cm_brush_t* brush );

	//! Recursively frees all memory associated with a collision model tree node and its children.
	void			 FreeTree_r( cm_model_t* model, cm_node_t* headNode, cm_node_t* node );

	//! Frees the memory associated with a collision model.
	void			 FreeModel( cm_model_t* model );

	/*!
		\brief Replaces occurrences of two specified polygons with a new polygon throughout a collision model tree structure

		This function recursively traverses a collision model tree to find and replace all references to two specified polygons with a single new polygon. It handles both leaf and internal nodes in
	   the tree structure. The function manages polygon references by removing duplicate references and updating the model's polygon reference count. It processes the tree by checking bounds against
	   splitting plane distances to determine which child nodes to process next.

		\param model The collision model containing the polygons and references to be modified
		\param node The current node in the collision model tree being processed
		\param p1 First polygon to be replaced
		\param p2 Second polygon to be replaced
		\param newp New polygon that will replace occurrences of p1 and p2
	*/
	void			 ReplacePolygons( cm_model_t* model, cm_node_t* node, cm_polygon_t* p1, cm_polygon_t* p2, cm_polygon_t* newp );

	//! Attempts to merge two polygons into a single convex polygon if they share edges and maintain convexity.
	cm_polygon_t*	 TryMergePolygons( cm_model_t* model, cm_polygon_t* p1, cm_polygon_t* p2 );

	//! Attempts to merge a polygon with other polygons in the collision model tree and returns true if a merge operation was successful.
	bool			 MergePolygonWithTreePolygons( cm_model_t* model, cm_node_t* node, cm_polygon_t* polygon );

	//! Merges polygons in the collision model tree that have the same surface flags and contents
	void			 MergeTreePolygons( cm_model_t* model, cm_node_t* node );

	//! Checks if a point is inside a polygon within a collision model
	bool			 PointInsidePolygon( cm_model_t* model, cm_polygon_t* p, idVec3& v );

	//! Finds and marks internal edges between two polygons in a collision model
	void			 FindInternalEdgesOnPolygon( cm_model_t* model, cm_polygon_t* p1, cm_polygon_t* p2 );

	//! Finds internal edges between polygons in a collision model.
	void			 FindInternalPolygonEdges( cm_model_t* model, cm_node_t* node, cm_polygon_t* polygon );

	//! Finds internal edges within the collision model by traversing the spatial partitioning tree and processing polygons at each node.
	void			 FindInternalEdges( cm_model_t* model, cm_node_t* node );

	//! Determines which edges of a polygon are contained within the polygon's bounds and marks them as internal.
	void			 FindContainedEdges( cm_model_t* model, cm_polygon_t* p );

	//! Parses procedural BSP tree nodes from a lexer input stream.
	void			 ParseProcNodes( idLexer* src );

	//! Loads a procedural BSP file with the specified name for collision detection.
	void			 LoadProcBSP( const char* name );

	/*!
		\brief Determines if a winding is completely removed by a procedural BSP tree structure based on containment and splitting.

		This function recursively traverses a procedural BSP tree to check if a winding is entirely outside or removed by the tree structure. It splits the winding against each BSP node plane and
	   continues traversal based on the resulting side relationships. The function returns false if the winding is determined to be completely outside the structure, and true if it remains inside or
	   is partially intersecting but not fully removed. The process considers the winding's normal, origin, and radius for proper containment checks.

		\param nodeNum Index of the current BSP node in the procNodes array
		\param w Pointer to the winding being checked for removal
		\param normal Normal vector of the winding plane
		\param origin Origin point used for distance calculations
		\param radius Radius value used to determine containment distance
		\return True if the winding is not completely removed by the BSP tree structure, false otherwise
	*/
	int				 R_ChoppedAwayByProcBSP( int nodeNum, idFixedWinding* w, const idVec3& normal, const idVec3& origin, const float radius );

	//! Determines if a winding is completely removed by the procedural BSP tree.
	int				 ChoppedAwayByProcBSP( const idFixedWinding& w, const idPlane& plane, int contents );

	//! Chops a list of windings with a brush to minimize the number of fragments outside the brush.
	void			 ChopWindingListWithBrush( cm_windingList_t* list, cm_brush_t* b );

	//! Recursively chops a winding list using brushes from a collision model tree.
	void			 R_ChopWindingListWithTreeBrushes( cm_windingList_t* list, cm_node_t* node );

	/*!
		\brief Returns a winding that is not fully contained within brushes, processing the winding against the collision model tree.

		This function processes a winding against the collision model tree to determine if it is fully contained within brushes. It handles various cases including windings that are completely chopped
	   away, windings that are partially contained, and windings that are completely outside brushes. When the winding is partially contained, it attempts to chop away the portions that are inside
	   brushes while avoiding creating multiple winding fragments. The function is used primarily for processing polygons in the collision model system, particularly for world models and BSP tree
	   processing.

		\param w Input winding to process
		\param plane Plane defining the winding's orientation
		\param contents Content flags for the winding
		\param patch Primitive number for the winding
		\param headNode Root node of the collision model tree to process against
		\return Pointer to a winding that is not fully contained within brushes, or NULL if the winding is completely chopped away
		\throws NULL pointer if the winding is completely chopped away by the collision tree
	*/
	idFixedWinding*	 WindingOutsideBrushes( idFixedWinding* w, const idPlane& plane, int contents, int patch, cm_node_t* headNode );

	//! Allocates and initializes a new collision model instance.
	cm_model_t*		 AllocModel();

	//! Allocates a collision model node from a memory block within the specified model.
	cm_node_t*		 AllocNode( cm_model_t* model, int blockSize );

	//! Allocates and returns a polygon reference from a specified collision model block
	cm_polygonRef_t* AllocPolygonReference( cm_model_t* model, int blockSize );

	//! Allocates and returns a brush reference from a memory block within the specified collision model
	cm_brushRef_t*	 AllocBrushReference( cm_model_t* model, int blockSize );

	//! Allocates and returns a new collision polygon with the specified number of edges from the given model.
	cm_polygon_t*	 AllocPolygon( cm_model_t* model, int numEdges );

	//! Allocates and returns a new collision brush with the specified number of planes from the given model's memory pool.
	cm_brush_t*		 AllocBrush( cm_model_t* model, int numPlanes );

	//! Adds a polygon to a collision model node.
	void			 AddPolygonToNode( cm_model_t* model, cm_node_t* node, cm_polygon_t* p );

	//! Adds a brush to a node's list of brushes in the collision model.
	void			 AddBrushToNode( cm_model_t* model, cm_node_t* node, cm_brush_t* b );

	//! Initializes the trace model structure for collision detection.
	void			 SetupTrmModelStructure();

	/*!
		\brief Recursively filters a polygon into the collision model tree based on its bounding box and node plane splits

		This function traverses the collision model tree to determine where a polygon should be placed. It recursively splits the polygon based on the node's splitting plane until it reaches a leaf
	   node. When the polygon's bounding box is completely inside all child nodes, the traversal stops. The function handles cases where the polygon straddles the splitting plane by recursively
	   processing both children. Finally, the polygon is added to the appropriate node in the collision model.

		\param model Pointer to the collision model being processed
		\param node Pointer to the current node in the collision tree
		\param pref Pointer to a polygon reference structure or NULL
		\param p Pointer to the polygon being filtered into the tree
		\throws assertion failure when node is NULL
	*/
	void			 R_FilterPolygonIntoTree( cm_model_t* model, cm_node_t* node, cm_polygonRef_t* pref, cm_polygon_t* p );

	/*!
		\brief Recursively filters a brush into the collision model tree based on its bounds and the current node's plane.

		This function traverses the collision model tree recursively to determine where a brush should be placed. It evaluates the brush's bounding box against the current node's splitting plane. If
	   the brush is fully inside all children, the function stops traversal. Otherwise, it continues down the appropriate child nodes. If the brush cannot be fully classified, it is split and added to
	   both children. Finally, the brush is either linked to a brush reference list or directly added to the node.

		\param model The collision model to which the brush belongs
		\param node The current node in the collision tree where the brush should be placed
		\param pref A pointer to the brush reference list to which the brush may be added
		\param b The brush to be filtered into the tree
		\throws assertion failure if node is NULL
	*/
	void			 R_FilterBrushIntoTree( cm_model_t* model, cm_node_t* node, cm_brushRef_t* pref, cm_brush_t* b );

	//! Creates an axial BSP tree node by recursively splitting the given bounds based on the optimal split plane.
	cm_node_t*		 R_CreateAxialBSPTree( cm_model_t* model, cm_node_t* node, const idBounds& bounds );

	//! Creates an axial BSP tree for the specified collision model node.
	cm_node_t*		 CreateAxialBSPTree( cm_model_t* model, cm_node_t* node );

	//! Initializes the hash tables and winding lists used for collision detection and polygon processing.
	void			 SetupHash();

	//! Frees all memory allocated for the hash tables used in collision detection.
	void			 ShutdownHash();

	//! Clears the hash tables and updates the model bounds and vertex shift values based on the provided bounding box.
	void			 ClearHash( idBounds& bounds );

	//! Computes a hash value for a 3D vector using bit shifting and masking operations
	int				 HashVec( const idVec3& vec );

	//! Returns the vertex number for a given vertex in a collision model, creating a new vertex if necessary.
	int				 GetVertex( cm_model_t* model, const idVec3& v, int* vertexNum );

	/*!
		\brief Retrieves or creates an edge between two vertices in a collision model, returning whether the edge already existed.

		This function manages the edge data structure for a collision model. It either finds an existing edge between the two specified vertices or creates a new one. The function handles vertex
	   lookup and edge hashing to maintain efficient access. If both vertices are the same or snapped together, it returns immediately with a special edge number. The function also manages dynamic
	   resizing of the edge array when needed.

		\param model Pointer to the collision model containing the edge
		\param v1 First vertex of the edge
		\param v2 Second vertex of the edge
		\param edgeNum Output parameter that receives the edge number
		\param v1num Vertex index of the first vertex, or -1 if not known
		\return Returns true if the edge already existed in the model, false if a new edge was created.
	*/
	int				 GetEdge( cm_model_t* model, const idVec3& v1, const idVec3& v2, int* edgeNum, int v1num );

	/*!
		\brief Creates a polygon from a winding and adds it to the collision model

		This function processes a winding to create a polygon structure that represents a collision surface within the specified model. It converts the winding into a sequence of edges, validates the
	   polygon structure, and inserts the polygon into the collision model's spatial tree structure. The function handles edge validation to ensure no edge appears twice and enforces a maximum edge
	   count limit.

		\param model The collision model to which the polygon will be added
		\param w The winding defining the polygon shape
		\param plane The plane equation defining the polygon's surface
		\param material The material properties associated with the polygon
		\param primitiveNum The primitive number identifier for the polygon
	*/
	void			 CreatePolygon( cm_model_t* model, idFixedWinding* w, const idPlane& plane, const idMaterial* material, int primitiveNum );

	/*!
		\brief Creates a collision polygon from a winding, handling proc BSP culling and two-sided material processing

		This function processes a winding to create a collision polygon for a given model. It first checks if the polygon should be removed due to proc BSP tree culling, and if so increments the
	   removed polygons counter and returns early. Otherwise, it removes windings that are fully contained within brushes. If the winding is degenerate or fully contained, it increments the removed
	   polygons counter and returns. Otherwise, it creates the polygon and handles two-sided materials by creating a reversed copy of the polygon.

		\param model The collision model to add the polygon to
		\param w The winding defining the polygon shape
		\param plane The plane equation defining the polygon's orientation
		\param material The material properties for the polygon
		\param primitiveNum The primitive number, with negative values indicating patches
	*/
	void			 PolygonFromWinding( cm_model_t* model, idFixedWinding* w, const idPlane& plane, const idMaterial* material, int primitiveNum );

	//! Calculates edge normals for polygons in a collision model tree node
	void			 CalculateEdgeNormals( cm_model_t* model, cm_node_t* node );

	/*!
		\brief Creates polygonal collision surfaces from a patch mesh by tessellating it into triangles and quads.

		This function takes a patch mesh and converts it into collision polygons by dividing each quad cell into triangles. It evaluates whether the four vertices of a quad form a planar surface, and
	   if so, creates a quad polygon. Otherwise, it splits the quad into two triangles. The normal of each polygon is computed from the cross product of two edges, and the plane is fitted through one
	   of the vertices. The primitive number is negated when passed to the polygon creation function, likely to distinguish collision primitives from other types of geometry.

		\param model The collision model to which the generated polygons will be added
		\param mesh The patch mesh from which polygons are generated
		\param material The material associated with the polygons, used for collision behavior determination
		\param primitiveNum The primitive number used to identify the polygons; will be negated when passed to internal functions
	*/
	void			 CreatePatchPolygons( cm_model_t* model, idSurface_Patch& mesh, const idMaterial* material, int primitiveNum );

	//! Converts a map patch into collision polygons for the specified model.
	void			 ConvertPatch( cm_model_t* model, const idMapPatch* patch, int primitiveNum );

	/*!
		\brief Converts brush sides from a map brush into collision polygons for a given model

		This function processes each side of a map brush to generate collision polygons for the specified collision model. It handles degenerate planes by fixing them and applying an offset to account
	   for the brush's positioning. For each brush side that has the CONTENTS_REMOVE_UTIL content flag, it creates a winding from the plane and clips it against other brush sides to form a polygon.
	   The resulting polygon is then converted into a collision primitive and added to the model.

		\param model The collision model to add the generated polygons to
		\param mapBrush The map brush containing the sides to convert
		\param primitiveNum The primitive number to use for the generated collision primitives
		\param originOffset The offset to apply to the brush sides for proper positioning
	*/
	void			 ConvertBrushSides( cm_model_t* model, const idMapBrush* mapBrush, int primitiveNum, const idVec3& originOffset );

	/*!
		\brief Converts a map brush into a collision model brush with specified origin offset

		This function takes a map brush and converts it into a collision model brush structure. It processes the brush sides to fix degenerate planes and calculates the bounding volume. The function
	   also determines the contents flags based on material properties and constructs the brush with appropriate plane data. The origin offset is applied to account for brush positioning in the world
	   space. If no contents are found, the function returns early without creating a brush.

		\param model Collision model to add the brush to
		\param mapBrush Map brush data to convert
		\param primitiveNum Primitive number for the brush
		\param originOffset Offset to apply to the brush planes
	*/
	void			 ConvertBrush( cm_model_t* model, const idMapBrush* mapBrush, int primitiveNum, const idVec3& originOffset );

	//! Converts a mesh polygon to a collision model primitive.
	void			 ConvertMesh( cm_model_t* model, const MapPolygonMesh* mesh, int primitiveNum );

	//! Prints detailed memory usage information for a collision model
	void			 PrintModelInfo( const cm_model_t* model );

	//! Accumulates statistics from all loaded collision models into a single model structure.
	void			 AccumulateModelInfo( cm_model_t* model );

	//! Remaps edge indices for polygons in a collision model tree node.
	void			 RemapEdges( cm_node_t* node, int* edgeRemap );

	//! Optimizes vertex and edge arrays by removing unused entries from the collision model.
	void			 OptimizeArrays( cm_model_t* model );

	//! Finalizes a collision model by processing its polygons, edges, and bounds.
	void			 FinishModel( cm_model_t* model );

	//! Builds collision models for the entities in the given map file
	void			 BuildModels( const idMapFile* mapFile, bool ignoreOldCollisionFile );

	//! Finds and returns the handle to a collision model by its name, or -1 if not found.
	cmHandle_t		 FindModel( const char* name );

	//! Creates a collision model from a map entity containing brushes and patches.
	cm_model_t*		 CollisionModelForMapEntity( const idMapEntity* mapEnt );

	//! Loads a render model from a file and converts it to a collision model
	cm_model_t*		 LoadRenderModel( const char* fileName );

	//! Loads a binary collision model from a file
	cm_model_t*		 LoadBinaryModel( const char* fileName, ID_TIME_T sourceTimeStamp );

	//! Loads a binary collision model from a file, verifying the file format and timestamp
	cm_model_t*		 LoadBinaryModelFromFile( idFile* fileIn, ID_TIME_T sourceTimeStamp );

	//! Writes a collision model to a binary file with the specified file name and source time stamp.
	void			 WriteBinaryModel( cm_model_t* model, const char* fileName, ID_TIME_T sourceTimeStamp );

	//! Writes a collision model to a binary file with associated metadata and geometry data.
	void			 WriteBinaryModelToFile( cm_model_t* model, idFile* fileOut, ID_TIME_T sourceTimeStamp );

	//! Converts a collision model node into a trace model by recursively processing polygons and edges
	bool			 TrmFromModel_r( idTraceModel& trm, cm_node_t* node );

	//! Converts a collision model into a trace model for use in collision detection.
	bool			 TrmFromModel( const cm_model_t* model, idTraceModel& trm );

private:
	// CollisionMap_files.cpp -------------------------

	//! Writes collision model node data to a file
	void		WriteNodes( idFile* fp, cm_node_t* node );

	//! Counts the memory used by polygons in a collision model node and its children.
	int			CountPolygonMemory( cm_node_t* node ) const;

	//! Writes polygon data for a collision model node to a file
	void		WritePolygons( idFile* fp, cm_node_t* node );

	//! Counts the memory used by brushes in a collision model node and its children.
	int			CountBrushMemory( cm_node_t* node ) const;

	//! Writes brush data to a file descriptor starting from a given node in the collision model.
	void		WriteBrushes( idFile* fp, cm_node_t* node );

	//! Writes a collision model to a file in a human-readable format
	void		WriteCollisionModel( idFile* fp, cm_model_t* model );

	/*!
		\brief Writes collision models to a file with the specified parameters

		This function outputs collision model data to a file with a specific format including file ID, version, and map file CRC. It iterates through a range of collision models and writes each one to
	   the file. The filename is automatically appended with the collision model file extension. The function provides feedback through the common system's printf and warning mechanisms.

		\param filename Name of the file to write collision models to
		\param firstModel Index of the first collision model to write
		\param lastModel Index of the last collision model to write (exclusive)
		\param mapFileCRC CRC value of the map file for verification purposes
	*/
	void		WriteCollisionModelsToFile( const char* filename, int firstModel, int lastModel, unsigned int mapFileCRC );

	//! Parses collision model nodes from a token stream and constructs a node tree
	cm_node_t*	ParseNodes( idLexer* src, cm_model_t* model, cm_node_t* parent );

	//! Parses vertex data from a lexer into a collision model.
	void		ParseVertices( idLexer* src, cm_model_t* model );

	//! Parses edge data from a lexer into a collision model, including vertex connections and internal edge flags.
	void		ParseEdges( idLexer* src, cm_model_t* model );

	//! Parses polygon data from a lexer source into a collision model.
	void		ParsePolygons( idLexer* src, cm_model_t* model );

	//! Parses brush data from a lexer source into a collision model.
	void		ParseBrushes( idLexer* src, cm_model_t* model );

	//! Parses a collision model from a token stream.
	cm_model_t* ParseCollisionModel( idLexer* src );

	//! Loads a collision model file from disk, either from a binary generated file or a source file.
	bool		LoadCollisionModelFile( const char* name, unsigned int mapFileCRC );

private:
	// CollisionMap_debug -------------------------

	//! Converts a string representation of collision contents into a bitwise integer flag.
	int			ContentsFromString( const char* string ) const;

	//! Converts a collision contents integer into a comma-separated string of content type names.
	const char* StringFromContents( const int contents ) const;

	/*!
		\brief Draws a collision model edge with optional normal visualization

		This function renders a visual representation of a collision model edge using the rendering world debug drawing capabilities. It handles both internal and external edges differently, with
	   internal edges drawn in green when enabled by the cm_drawInternal cvar. External edges are drawn in blue if they have more than two users, otherwise in the default collision model color. The
	   function also supports drawing edge normals when enabled by cm_drawNormals cvar, using the edge's normal vector scaled by a factor of 5. The edge coordinates are transformed by the provided
	   axis and offset by the origin parameter.

		\param model Pointer to the collision model containing the edge to be drawn
		\param edgeNum Index of the edge to draw, negative values indicate reversed direction
		\param origin Translation offset to apply to the edge vertices
		\param axis Rotation matrix to transform the edge vertices
	*/
	void		DrawEdge( cm_model_t* model, int edgeNum, const idVec3& origin, const idMat3& axis );

	/*!
		\brief Draws a collision polygon with optional backface culling, normal visualization, and filled or wireframe rendering.

		This function renders a polygon from a collision model, applying transformations based on the provided origin and axis. It supports backface culling to avoid rendering polygons facing away
	   from the view origin. If enabled, it can draw normal vectors for the polygon. The polygon can be rendered either filled with a color or as a wireframe using individual edges. The rendering is
	   done through the graphics system's debug drawing functions.

		\param model Pointer to the collision model containing the polygon and associated edge and vertex data
		\param p Pointer to the polygon to be drawn
		\param origin Translation offset applied to the polygon vertices
		\param axis Rotation matrix applied to the polygon vertices
		\param viewOrigin Origin of the camera or view point used for backface culling
	*/
	void		DrawPolygon( cm_model_t* model, cm_polygon_t* p, const idVec3& origin, const idMat3& axis, const idVec3& viewOrigin );

	/*!
		\brief Draws polygons for a collision model node within a specified radius from the view origin

		This function recursively traverses a collision model tree and draws polygons that are visible within the specified radius. It checks if polygons are within the view bounds and filters them
	   based on content flags. The function uses a check count to avoid redundant drawing of the same polygon

		\param model Pointer to the collision model structure
		\param node Pointer to the current node in the collision model tree
		\param origin Origin offset for the drawing
		\param axis Transformation axis for the drawing
		\param viewOrigin The origin of the viewer or camera position
		\param radius The radius within which polygons are considered for drawing
	*/
	void		DrawNodePolygons( cm_model_t* model, cm_node_t* node, const idVec3& origin, const idMat3& axis, const idVec3& viewOrigin, const float radius );

private: // collision map data
	idStr			  mapName;
	ID_TIME_T		  mapFileTime;
	int				  loaded;
	// for multi-check avoidance
	int				  checkCount;
	// models
	int				  maxModels;
	int				  numModels;
	cm_model_t**	  models;
	// polygons and brush for trm model
	cm_polygonRef_t*  trmPolygons[MAX_TRACEMODEL_POLYS];
	cm_brushRef_t*	  trmBrushes[1];
	const idMaterial* trmMaterial;
	// for data pruning
	int				  numProcNodes;
	cm_procNode_t*	  procNodes;
	// for retrieving contact points
	bool			  getContacts;
	contactInfo_t*	  contacts;
	int				  maxContacts;
	int				  numContacts;
};

// for debugging
extern idCVar cm_debugCollision;
