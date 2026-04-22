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

#ifndef __COLLISIONMODELMANAGER_H__
#define __COLLISIONMODELMANAGER_H__

/*
===============================================================================

	Trace model vs. polygonal model collision detection.

	Short translations are the least expensive. Retrieving contact points is
	about as cheap as a short translation. Position tests are more expensive
	and rotations are most expensive.

	There is no position test at the start of a translation or rotation. In other
	words if a translation with start != end or a rotation with angle != 0 starts
	in solid, this goes unnoticed and the collision result is undefined.

	A translation with start == end or a rotation with angle == 0 performs
	a position test and fills in the trace_t structure accordingly.

===============================================================================
*/

// contact type
typedef enum {
	CONTACT_NONE,		 // no contact
	CONTACT_EDGE,		 // trace model edge hits model edge
	CONTACT_MODELVERTEX, // model vertex hits trace model polygon
	CONTACT_TRMVERTEX	 // trace model vertex hits model polygon
} contactType_t;

// contact info
typedef struct {
	contactType_t	  type;			// contact type
	idVec3			  point;		// point of contact
	idVec3			  normal;		// contact plane normal
	float			  dist;			// contact plane distance
	int				  contents;		// contents at other side of surface
	const idMaterial* material;		// surface material
	int				  modelFeature; // contact feature on model
	int				  trmFeature;	// contact feature on trace model
	int				  entityNum;	// entity the contact surface is a part of
	int				  id;			// id of clip model the contact surface is part of
} contactInfo_t;

// trace result
typedef struct trace_s {
	float		  fraction; // fraction of movement completed, 1.0 = didn't hit anything
	idVec3		  endpos;	// final position of trace model
	idMat3		  endAxis;	// final axis of trace model
	contactInfo_t c;		// contact information, only valid if fraction < 1.0
} trace_t;

typedef int cmHandle_t;

#define CM_CLIP_EPSILON	  0.25f	  // always stay this distance away from any model
#define CM_BOX_EPSILON	  1.0f	  // should always be larger than clip epsilon
#define CM_MAX_TRACE_DIST 4096.0f // maximum distance a trace model may be traced, point traces are unlimited

/*!
	\class idCollisionModelManager
	\brief Manages collision models and provides collision detection services.

	This class serves as an abstract interface for handling collision models within the engine. It provides methods for loading, managing, and performing collision detection operations with various
   types of models including map geometry and trace models. The interface supports operations like translation, rotation, content checking, and contact detection. It also provides debugging and
   visualization capabilities for collision models. The class is designed to be implemented by concrete collision system backends that provide the actual collision detection functionality.

*/
class idCollisionModelManager
{
public:
	virtual ~idCollisionModelManager() { }

	//! Loads collision models from a map file
	virtual void		LoadMap( const idMapFile* mapFile, bool ignoreOldCollisionFile ) = 0;

	//! Frees all collision models and cleans up associated resources.
	virtual void		FreeMap() = 0;

	//! Preloads collision models for a specified map by processing a preload manifest file.
	virtual void		Preload( const char* mapName ) = 0;

	//! Loads a collision model by name, returning its handle.
	virtual cmHandle_t	LoadModel( const char* modelName, const bool precache ) = 0;

	//! Sets up a trace model for collision with other trace models.
	virtual cmHandle_t	SetupTrmModel( const idTraceModel& trm, const idMaterial* material ) = 0;

	//! Creates a trace model from a collision model and returns true if successful.
	virtual bool		TrmFromModel( const char* modelName, idTraceModel& trm ) = 0;

	//! Returns the name of the collision model associated with the given handle
	virtual const char* GetModelName( cmHandle_t model ) const = 0;

	//! Retrieves the bounding box of a specified collision model.
	virtual bool		GetModelBounds( cmHandle_t model, idBounds& bounds ) const = 0;

	//! Retrieves the contents flags of a collision model by combining all brush and polygon contents.
	virtual bool		GetModelContents( cmHandle_t model, int& contents ) const = 0;

	//! Retrieves a vertex from a collision model by its handle and vertex number.
	virtual bool		GetModelVertex( cmHandle_t model, int vertexNum, idVec3& vertex ) const = 0;

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
	virtual bool		GetModelEdge( cmHandle_t model, int edgeNum, idVec3& start, idVec3& end ) const = 0;

	//! Retrieves the polygon data for a specified model and polygon number
	virtual bool		GetModelPolygon( cmHandle_t model, int polygonNum, idFixedWinding& winding ) const = 0;

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
	virtual void		Translation( trace_t* results,
			   const idVec3&				  start,
			   const idVec3&				  end,
			   const idTraceModel*			  trm,
			   const idMat3&				  trmAxis,
			   int							  contentMask,
			   cmHandle_t					  model,
			   const idVec3&				  modelOrigin,
			   const idMat3&				  modelAxis ) = 0;

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
	virtual void		Rotation( trace_t* results,
			   const idVec3&			   start,
			   const idRotation&		   rotation,
			   const idTraceModel*		   trm,
			   const idMat3&			   trmAxis,
			   int						   contentMask,
			   cmHandle_t				   model,
			   const idVec3&			   modelOrigin,
			   const idMat3&			   modelAxis ) = 0;

	//! Returns the contents of the collision model at the specified point.
	virtual int			PointContents( const idVec3 p, cmHandle_t model ) = 0;

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
	virtual int			Contents( const idVec3& start, const idTraceModel* trm, const idMat3& trmAxis, int contentMask, cmHandle_t model, const idVec3& modelOrigin, const idMat3& modelAxis ) = 0;

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
	virtual int			Contacts( contactInfo_t* contacts,
				const int						 maxContacts,
				const idVec3&					 start,
				const idVec6&					 dir,
				const float						 depth,
				const idTraceModel*				 trm,
				const idMat3&					 trmAxis,
				int								 contentMask,
				cmHandle_t						 model,
				const idVec3&					 modelOrigin,
				const idMat3&					 modelAxis ) = 0;

	//! Tests collision detection and outputs performance statistics for translation and rotation operations.
	virtual void		DebugOutput( const idVec3& origin ) = 0;

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
	virtual void		DrawModel( cmHandle_t model, const idVec3& modelOrigin, const idMat3& modelAxis, const idVec3& viewOrigin, const float radius ) = 0;

	//! Prints information about the specified collision model, or accumulated model information if the handle is -1.
	virtual void		ModelInfo( cmHandle_t model ) = 0;

	//! Lists all loaded collision models and their memory usage.
	virtual void		ListModels() = 0;

	//! Writes a collision model file for the given map entity
	virtual bool		WriteCollisionModelForMapEntity( const idMapEntity* mapEnt, const char* filename, const bool testTraceModel = true ) = 0;
};

extern idCollisionModelManager* collisionModelManager;

#endif /* !__COLLISIONMODELMANAGER_H__ */
