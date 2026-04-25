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

#ifndef __CLIP_H__
#define __CLIP_H__

/*
===============================================================================

  Handles collision detection with the world and between physics objects.

===============================================================================
*/

#define CLIPMODEL_ID_TO_JOINT_HANDLE( id ) ( ( id ) >= 0 ? INVALID_JOINT : ( ( jointHandle_t )( -1 - id ) ) )
#define JOINT_HANDLE_TO_CLIPMODEL_ID( id ) ( -1 - id )

class idClip;
class idClipModel;
class idEntity;

/*!
	\class idClipModel
	\brief Provides collision detection capabilities through various model types and management functions.

	The idClipModel class serves as a core component for collision detection, supporting different model types including trace models and render models. It manages the linking and unlinking of
   collision models within spatial sectors, enabling efficient collision queries. The class supports loading models from various sources, such as file names, trace models, or render model handles, and
   provides methods to update model properties like position, orientation, and material. It also handles persistence of models through save states and includes utility functions for mass properties
   calculation and trace model caching. The class integrates with a clip system to manage model linkage and enables or disables collision detection for individual models.

*/
class idClipModel
{
	friend class idClip;

public:
	//! Initializes a new instance of the idClipModel class.
	idClipModel();

	//! Constructs an idClipModel object and initializes it with the specified model name.
	explicit idClipModel( const char* name );

	//! Initializes a new clip model instance from the provided trace model.
	explicit idClipModel( const idTraceModel& trm );

	//! Constructs an idClipModel instance using the provided trace model and persistence setting.
	explicit idClipModel( const idTraceModel& trm, bool persistantThroughSave );

	//! Constructs an idClipModel object using a render model handle.
	explicit idClipModel( const int renderModelHandle );

	//! Constructs a new clip model by copying the properties and state from an existing clip model.
	explicit idClipModel( const idClipModel* model );

	//! Destroys the clip model and ensures it is unlinked and its trace model is freed.
	~idClipModel();

	//! Loads a collision model by name and initializes its bounds and contents
	bool				LoadModel( const char* name );

	//! Loads a trace model into the clip model, optionally making it persistent through save states.
	void				LoadModel( const idTraceModel& trm, bool persistantThroughSave = true );

	//! Initializes the clip model using a render model handle.
	void				LoadModel( const int renderModelHandle );

	//! Saves the clip model state to a save file.
	void				Save( idSaveGame* savefile ) const;

	//! Restores the clip model state from a saved game file
	void				Restore( idRestoreGame* savefile );

	//! Links the clip model to the specified clip object.
	void				Link( idClip& clp );

	/*!
		\brief Links the clip model to an entity with specified origin, axis, and optional render model handle.

		This function associates the clip model with an entity and updates its properties including the entity reference, ID, origin, and axis. If a valid render model handle is provided, it also
	   updates the render model handle and calculates the bounds based on the render entity. Finally, it calls the Link method of the clip object to register the model within the collision system.

		\param clp Reference to the clip object used for linking
		\param ent Pointer to the entity associated with this clip model
		\param newId New ID to assign to the clip model
		\param newOrigin New origin position for the clip model
		\param newAxis New orientation axis for the clip model
		\param renderModelHandle Optional handle to the render model, defaults to -1 if not provided
	*/
	void				Link( idClip& clp, idEntity* ent, int newId, const idVec3& newOrigin, const idMat3& newAxis, int renderModelHandle = -1 );

	//! Unlinks the clip model from all sectors it is currently linked to.
	void				Unlink();

	//! Updates the position and orientation of the clip model.
	void				SetPosition( const idVec3& newOrigin, const idMat3& newAxis );

	//! Moves the clip model by the specified translation vector.
	void				Translate( const idVec3& translation );

	//! Rotates the clip model using the provided rotation transformation.
	void				Rotate( const idRotation& rotation );

	//! Enables the clip model for clipping operations.
	void				Enable();

	//! Disables the clip model for clipping operations while keeping it linked.
	void				Disable();

	//! Sets the material for the clip model.
	void				SetMaterial( const idMaterial* m );

	//! Returns the material associated with this clip model.
	const idMaterial*	GetMaterial() const;

	//! Sets the contents of the clip model to the specified value.
	void				SetContents( int newContents );

	//! Returns the contents bitmask of the clip model.
	int					GetContents() const;

	//! Sets the entity associated with this clip model.
	void				SetEntity( idEntity* newEntity );

	//! Returns the entity associated with this collision model.
	idEntity*			GetEntity() const;

	//! Sets the ID of the clip model to the specified value.
	void				SetId( int newId );

	//! Returns the unique identifier of this clip model.
	int					GetId() const;

	//! Sets the owner entity for this clip model.
	void				SetOwner( idEntity* newOwner );

	//! Returns the entity that owns this clip model.
	idEntity*			GetOwner() const;

	//! Returns the bounding box of the clip model.
	const idBounds&		GetBounds() const;

	//! Returns the absolute bounds of the clip model.
	const idBounds&		GetAbsBounds() const;

	//! Returns the origin of the clip model.
	const idVec3&		GetOrigin() const;

	//! Returns the axis of the clip model.
	const idMat3&		GetAxis() const;

	//! Returns true if this clip model is a trace model.
	bool				IsTraceModel() const;

	//! Returns true if this clip model is associated with a render model.
	bool				IsRenderModel() const;

	//! Returns true if the clip model is linked.
	bool				IsLinked() const;

	//! Returns true if the clip model is enabled for collision detection.
	bool				IsEnabled() const;

	//! Checks if the clip model's cached trace model is equal to the provided trace model.
	bool				IsEqual( const idTraceModel& trm ) const;

	//! Returns the handle used to collide versus this model.
	cmHandle_t			Handle() const;

	//! Returns the trace model associated with this clip model, or NULL if no trace model is set.
	const idTraceModel* GetTraceModel() const;

	/*!
		\brief Calculates mass properties including mass, center of mass, and inertia tensor for a clip model based on its trace model and the given density.

		This function retrieves mass properties for a clip model by using the cached trace model data. It computes the mass by multiplying the trace model's volume with the provided density. The
	   center of mass is taken directly from the trace model cache, and the inertia tensor is scaled by the density. The function requires the clip model to be a valid trace model, and will generate
	   an error if it is not.

		\param density The density used to calculate the mass properties
		\param mass Output parameter for the calculated mass
		\param centerOfMass Output parameter for the center of mass
		\param inertiaTensor Output parameter for the inertia tensor
		\throws Error when the clip model is not a trace model
	*/
	void				GetMassProperties( const float density, float& mass, idVec3& centerOfMass, idMat3& inertiaTensor ) const;

	//! Loads a collision model by name without forcing a reload
	static cmHandle_t	CheckModel( const char* name );

	//! Clears the trace model cache and unsaved trace model cache, and frees the trace model hash tables.
	static void			ClearTraceModelCache();

	//! Returns the total memory size occupied by all trace models in the cache.
	static int			TraceModelCacheSize();

	//! Saves the trace models cached by this clip model to the given save file.
	static void			SaveTraceModels( idSaveGame* savefile );

	//! Restores cached trace models from a save file
	static void			RestoreTraceModels( idRestoreGame* savefile );

private:
	bool					  enabled;				// true if this clip model is used for clipping
	idEntity*				  entity;				// entity using this clip model
	int						  id;					// id for entities that use multiple clip models
	idEntity*				  owner;				// owner of the entity that owns this clip model
	idVec3					  origin;				// origin of clip model
	idMat3					  axis;					// orientation of clip model
	idBounds				  bounds;				// bounds
	idBounds				  absBounds;			// absolute bounds
	const idMaterial*		  material;				// material for trace models
	int						  contents;				// all contents ored together
	cmHandle_t				  collisionModelHandle; // handle to collision model
	int						  traceModelIndex;		// trace model used for collision detection
	int						  renderModelHandle;	// render model def handle

	struct clipLink_s*		  clipLinks; // links into sectors
	int						  touchCount;

	//! Initializes the clip model properties to their default values.
	void					  Init();

	//! Recursively links the clip model into the appropriate sectors based on its bounding box.
	void					  Link_r( struct clipSector_s* node );

	//! Allocates and returns an index for a trace model, with an option to persist through saves.
	static int				  AllocTraceModel( const idTraceModel& trm, bool persistantThroughSaves = true );

	//! Frees a trace model by decreasing its reference count and removing it from the appropriate cache if the count reaches zero.
	static void				  FreeTraceModel( int traceModelIndex );

	//! Returns a cached trace model for the specified index, retrieving from either saved or unsaved cache based on the trace model index flags.
	static idTraceModel*	  GetCachedTraceModel( int traceModelIndex );

	//! Returns a hash key for the trace model based on its properties and vertices.
	static int				  GetTraceModelHashKey( const idTraceModel& trm );

	//! Retrieves a trace model entry from either the saved or unsaved cache based on the trace model index.
	static struct trmCache_s* GetTraceModelEntry( int traceModelIndex );
};

ID_INLINE void idClipModel::Translate( const idVec3& translation )
{
	Unlink();
	origin += translation;
}

ID_INLINE void idClipModel::Rotate( const idRotation& rotation )
{
	Unlink();
	origin *= rotation;
	axis *= rotation.ToMat3();
}

ID_INLINE void idClipModel::Enable()
{
	enabled = true;
}

ID_INLINE void idClipModel::Disable()
{
	enabled = false;
}

ID_INLINE void idClipModel::SetMaterial( const idMaterial* m )
{
	material = m;
}

ID_INLINE const idMaterial* idClipModel::GetMaterial() const
{
	return material;
}

ID_INLINE void idClipModel::SetContents( int newContents )
{
	contents = newContents;
}

ID_INLINE int idClipModel::GetContents() const
{
	return contents;
}

ID_INLINE void idClipModel::SetEntity( idEntity* newEntity )
{
	entity = newEntity;
}

ID_INLINE idEntity* idClipModel::GetEntity() const
{
	return entity;
}

ID_INLINE void idClipModel::SetId( int newId )
{
	id = newId;
}

ID_INLINE int idClipModel::GetId() const
{
	return id;
}

ID_INLINE void idClipModel::SetOwner( idEntity* newOwner )
{
	owner = newOwner;
}

ID_INLINE idEntity* idClipModel::GetOwner() const
{
	return owner;
}

ID_INLINE const idBounds& idClipModel::GetBounds() const
{
	return bounds;
}

ID_INLINE const idBounds& idClipModel::GetAbsBounds() const
{
	return absBounds;
}

ID_INLINE const idVec3& idClipModel::GetOrigin() const
{
	return origin;
}

ID_INLINE const idMat3& idClipModel::GetAxis() const
{
	return axis;
}

ID_INLINE bool idClipModel::IsRenderModel() const
{
	return ( renderModelHandle != -1 );
}

ID_INLINE bool idClipModel::IsTraceModel() const
{
	return ( traceModelIndex != -1 );
}

ID_INLINE bool idClipModel::IsLinked() const
{
	return ( clipLinks != NULL );
}

ID_INLINE bool idClipModel::IsEnabled() const
{
	return enabled;
}

ID_INLINE bool idClipModel::IsEqual( const idTraceModel& trm ) const
{
	return ( traceModelIndex != -1 && *GetCachedTraceModel( traceModelIndex ) == trm );
}

ID_INLINE const idTraceModel* idClipModel::GetTraceModel() const
{
	if( !IsTraceModel() ) { return NULL; }
	return idClipModel::GetCachedTraceModel( traceModelIndex );
}

/*!
	\class idClip
	\brief Collision detection and tracing system for 3D environments.

	Provides comprehensive collision detection and tracing capabilities for 3D environments, handling translation, rotation, and combined motion operations. Supports both world-based and entity-based
   collision queries with detailed contact information. The system uses a spatial subdivision approach for efficient collision detection and can trace against various collision models including world
   geometry, entities, and render models. It offers functions for point tracing, bounds tracing, contact detection, and content queries, supporting different content masks for filtering collision
   types. The implementation handles complex scenarios like large translations through step-by-step processing and manages both static and dynamic collision models. Additional utilities include entity
   and clip model querying, drawing functionality for debugging, and statistical reporting for performance analysis.

*/
class idClip
{
	friend class idClipModel;

public:
	//! Initializes an idClip object with default values.
	idClip();

	//! Initializes the clip system by setting up collision sectors and loading the world collision model.
	void Init();

	//! Cleans up and deallocates all resources used by the clip system
	void Shutdown();

	/*!
		\brief Performs a collision trace of a trace model moving from a start to end position, handling both rotated and non-rotated models and returning detailed contact information.

		This function clips a trace model against the world and other collision models, determining if and where a translation collision occurs. It computes the intersection between the moving trace
	   model and static collision models, returning detailed contact information including the fraction of traversal, collision point, and surface normal. The function handles special cases such as
	   immediate blocking by the world, and manages large translations by breaking them into smaller steps. It also handles render model traces separately from regular collision models.

		\param results Output structure containing the trace results including fraction, end position, and contact information
		\param start Starting position of the trace model
		\param end Ending position of the trace model
		\param mdl Collision model to be traced against
		\param trmAxis Axis of the trace model for rotation
		\param contentMask Mask of content types to be checked for collision
		\param passEntity Entity to be passed through during the trace, or NULL for world
		\return True if a collision occurred, false otherwise
	*/
	bool Translation( trace_t& results, const idVec3& start, const idVec3& end, const idClipModel* mdl, const idMat3& trmAxis, int contentMask, const idEntity* passEntity );

	/*!
		\brief Performs a rotation trace using the specified parameters and returns true if a collision occurs.

		This function traces the rotation of a collision model from a starting position and axis through a specified rotation. It tests against the world and any collision models in the specified
	   content mask. The function updates the results parameter with information about the collision, including the fraction of the rotation completed before collision, and the end position and axis
	   of the trace model. If a collision occurs with a model, the results structure is updated with the entity number and id of the colliding model. The function returns true if the rotation trace
	   resulted in a collision, false otherwise.

		\param results Output structure containing the collision results including fraction, end position, and axis
		\param start Starting position of the trace model
		\param rotation Rotation to apply to the trace model
		\param mdl Collision model to test against, or NULL for world collision
		\param trmAxis Initial axis of the trace model
		\param contentMask Mask of content types to test against
		\param passEntity Entity to pass through the trace, or NULL for world collision
		\return True if a collision occurred during the rotation trace, false otherwise
		\throws Assertion failures if the start or trmAxis parameters overlap with the results buffer, or if the function is called with invalid parameters
	*/
	bool Rotation( trace_t& results, const idVec3& start, const idRotation& rotation, const idClipModel* mdl, const idMat3& trmAxis, int contentMask, const idEntity* passEntity );

	/*!
		\brief Performs combined translational and rotational collision detection for a clip model moving from a start to end position with rotation

		This function handles motion detection by first checking for pure translation or rotation cases, then performing combined collision detection. It calculates the resulting trace information
	   including fraction, end position, and end axis. The function accounts for both world collisions and entity collisions, using separate collision models for translation and rotation detection.
	   The trace is calculated from the start position to the end position while considering the given rotation and the clip model's trace model. The function returns true if any collision occurred,
	   false otherwise. The function handles special cases like huge translations and different content masks for filtering collisions.

		\param results Structure to store the collision trace results including fraction, end position, end axis, and collision information
		\param start Starting position of the motion
		\param end Ending position of the motion
		\param rotation Rotation to apply during motion
		\param mdl Clip model to test against
		\param trmAxis Initial axis of the trace model
		\param contentMask Content mask to filter collisions
		\param passEntity Entity to pass through during collision detection
		\return True if a collision occurred during the motion, false otherwise
		\throws assertion failure if rotation origin does not match start position
	*/
	bool Motion( trace_t& results, const idVec3& start, const idVec3& end, const idRotation& rotation, const idClipModel* mdl, const idMat3& trmAxis, int contentMask, const idEntity* passEntity );

	/*!
		\brief Returns the number of contacts detected during a trace operation with the specified parameters

		This function performs collision detection by tracing a line or path defined by the start position, direction vector, and depth. It checks for contacts with the world and with any collision
	   models that intersect the trace bounds. The direction vector contains both linear and rotational components, where the first three components represent linear motion and the last three
	   represent rotational motion. The function accumulates contact information in the provided array up to the maximum specified limit and returns the total number of contacts found.

		\param contacts Array to store contact information for all collisions detected
		\param maxContacts Maximum number of contacts to store in the contacts array
		\param start Starting position of the trace
		\param dir Direction vector for the trace, where the first 3 components represent linear motion and the last 3 represent rotational motion
		\param depth Maximum distance to trace
		\param mdl Trace model used for collision detection
		\param trmAxis Axis orientation of the trace model
		\param contentMask Content mask to filter which surfaces are considered
		\param passEntity Entity to ignore during collision detection, or NULL to test against all entities
		\return Number of contacts detected during the trace operation
	*/
	int	 Contacts( contactInfo_t* contacts,
		 const int				  maxContacts,
		 const idVec3&			  start,
		 const idVec6&			  dir,
		 const float			  depth,
		 const idClipModel*		  mdl,
		 const idMat3&			  trmAxis,
		 int					  contentMask,
		 const idEntity*		  passEntity );

	/*!
		\brief Determines the contents bitmask of a trace model at a given position and orientation, checking against collision models

		This function evaluates the contents of the world that a trace model occupies at a specified position and orientation. It first checks if the trace model intersects with the world contents,
	   and then performs additional checks against a list of collision models. The function handles both world contents and entity contents, filtering results based on the provided content mask. It
	   returns a bitmask representing all the content types the trace model occupies, or 0 if the trace model is in free space.

		\param start Starting position of the trace model
		\param mdl Collision model to use for the trace
		\param trmAxis Axis orientation of the trace model
		\param contentMask Mask to filter content types
		\param passEntity Entity to pass through the contents check, or NULL to test world contents
		\return The contents bitmask of the trace model at the given position, or 0 if in free space
	*/
	int	 Contents( const idVec3& start, const idClipModel* mdl, const idMat3& trmAxis, int contentMask, const idEntity* passEntity );

	//! Returns the contents of the collision model at the specified point.
	int	 PointContents( const idVec3 p );

	/*!
		\brief Performs a point trace from start to end, returning true if the point intersects with any geometry in the specified content mask.

		This function traces a point from the start position to the end position, checking for collisions with geometry that matches the provided content mask. It uses the Translation function to
	   perform the actual tracing operation. The results parameter is populated with information about the trace result, including the collision fraction and normal. The passEntity parameter allows
	   specifying an entity to ignore during the trace, useful for avoiding self-collisions. This is a specialized trace function that treats the point as a single location rather than a volume,
	   making it suitable for detecting collisions with surfaces or entities that might be missed by volume-based tracing.

		\param results Output structure containing the trace results including fraction, normal, and entity hit
		\param start Starting position of the point trace
		\param end Ending position of the point trace
		\param contentMask Bitmask specifying which collision types to check against
		\param passEntity Entity to ignore during the trace, or NULL to trace against all entities
		\return True if the point trace intersected with any geometry, false if the point moved freely from start to end
	*/
	bool TracePoint( trace_t& results, const idVec3& start, const idVec3& end, int contentMask, const idEntity* passEntity );

	/*!
		\brief Performs a trace operation between two points using the specified bounding box as the collision model.

		This function traces a line from start to end using the provided bounds as the collision geometry. It loads the bounds into a temporary clip model and then performs a translation trace. The
	   function returns true if the trace was successful and hit something, false otherwise.

		\param results Output structure containing the trace results including fraction, normal, and hit entity
		\param start Starting position of the trace
		\param end Ending position of the trace
		\param bounds Bounding box defining the collision geometry
		\param contentMask Bitmask of content types to trace against
		\param passEntity Entity to exclude from the trace or NULL
		\return True if the trace hit something, false if it reached the end position without collision
	*/
	bool TraceBounds( trace_t& results, const idVec3& start, const idVec3& end, const idBounds& bounds, int contentMask, const idEntity* passEntity );

	/*!
		\brief Performs a translation trace against a specific collision model.

		This function executes a collision trace between two points using a specified trace model and collision model. It handles the translation of a trace model from a start position to an end
	   position while checking for collisions with the provided model. The function tracks the number of translations performed and uses the collision model manager to perform the actual trace
	   operation.

		\param results Output structure containing the trace results including hit information
		\param start Starting position of the trace
		\param end Ending position of the trace
		\param mdl Clip model to use for the trace
		\param trmAxis Axis orientation for the trace model
		\param contentMask Mask specifying which content types to check for collision
		\param model Handle to the collision model to trace against
		\param modelOrigin Origin position of the collision model
		\param modelAxis Axis orientation of the collision model
	*/
	void TranslationModel( trace_t& results,
		const idVec3&				start,
		const idVec3&				end,
		const idClipModel*			mdl,
		const idMat3&				trmAxis,
		int							contentMask,
		cmHandle_t					model,
		const idVec3&				modelOrigin,
		const idMat3&				modelAxis );

	/*!
		\brief Performs a rotational collision trace using the specified parameters and updates the results with collision data.

		This function executes a rotational trace against a collision model, taking into account the start position, rotation, and transformation matrices. It utilizes the collision model manager to
	   perform the actual trace operation and increments a rotation counter for tracking purposes. The function is typically used to detect collisions when an object rotates through space, such as
	   when an entity rotates or when a weapon swings. The trace considers the content mask to filter which types of collision objects are considered during the trace.

		\param results Output parameter that will contain the trace results including collision information
		\param start Starting position for the rotation trace
		\param rotation Rotation to be applied during the trace
		\param mdl Pointer to the clip model used for the trace
		\param trmAxis Transformation axis for the trace model
		\param contentMask Mask that specifies which content types to collide with
		\param model Handle to the collision model being traced against
		\param modelOrigin Origin of the model in world space
		\param modelAxis Axis of the model in world space
	*/
	void RotationModel( trace_t& results,
		const idVec3&			 start,
		const idRotation&		 rotation,
		const idClipModel*		 mdl,
		const idMat3&			 trmAxis,
		int						 contentMask,
		cmHandle_t				 model,
		const idVec3&			 modelOrigin,
		const idMat3&			 modelAxis );

	/*!
		\brief Calculates contact information between a trace model and a collision model

		This function performs contact calculations between a trace model and a specified collision model. It takes into account the starting position, direction, depth, and transformation properties
	   of both the trace model and the collision model. The function uses the collision model manager to perform the actual contact detection and returns the number of contacts found

		\param contacts Output array to store contact information
		\param maxContacts Maximum number of contacts to calculate
		\param start Starting position for the trace
		\param dir Direction and length of the trace
		\param depth Depth parameter for the trace
		\param mdl Clip model to use for the trace
		\param trmAxis Transformation axis for the trace model
		\param contentMask Content mask for determining which surfaces to contact
		\param model Handle to the collision model
		\param modelOrigin Origin of the collision model
		\param modelAxis Axis of the collision model
		\return Number of contacts found between the trace model and collision model
	*/
	int	 ContactsModel( contactInfo_t* contacts,
		 const int					   maxContacts,
		 const idVec3&				   start,
		 const idVec6&				   dir,
		 const float				   depth,
		 const idClipModel*			   mdl,
		 const idMat3&				   trmAxis,
		 int						   contentMask,
		 cmHandle_t					   model,
		 const idVec3&				   modelOrigin,
		 const idMat3&				   modelAxis );

	/*!
		\brief Returns the contents of the collision model at the specified start position, considering the given parameters and content mask.

		This function retrieves the contents of a collision model at a specified start position. It uses the provided trace model, transformation matrix, and content mask to determine what the model
	   contains. The function increments a content counter each time it's called, which could be used for profiling or debugging purposes. The actual content query is delegated to the collision model
	   manager.

		\param start The starting position to check for contents
		\param mdl The clip model to check contents for
		\param trmAxis The transformation matrix for the trace model
		\param contentMask The content mask to filter the contents
		\param model The handle to the collision model
		\param modelOrigin The origin of the collision model
		\param modelAxis The axis of the collision model
		\return The contents of the collision model at the specified start position, filtered by the content mask
	*/
	int	 ContentsModel( const idVec3& start, const idClipModel* mdl, const idMat3& trmAxis, int contentMask, cmHandle_t model, const idVec3& modelOrigin, const idMat3& modelAxis );

	/*!
		\brief Performs a collision trace for entity translation between start and end positions, checking against clip models with specified content mask.

		This function traces the movement of an entity from a start position to an end position, checking for collisions with other entities in the world. It considers the entity's clip model and
	   orientation during the trace. The function handles both render model traces and regular collision model translations, accumulating results into the provided trace_t structure. It skips the
	   world during collision checks.

		\param results Output structure containing the trace results including fraction, end position, and collision information
		\param start Starting position of the translation
		\param end Ending position of the translation
		\param mdl The clip model used for the trace
		\param trmAxis The axis orientation for the trace model
		\param contentMask Mask specifying which content types to collide with
		\param passEntity Entity to ignore during the trace
	*/
	void TranslationEntities( trace_t& results, const idVec3& start, const idVec3& end, const idClipModel* mdl, const idMat3& trmAxis, int contentMask, const idEntity* passEntity );

	//! Retrieves the contact feature from a collision model and stores it in a winding
	bool GetModelContactFeature( const contactInfo_t& contact, const idClipModel* clipModel, idFixedWinding& winding ) const;

	/*!
		\brief Returns the number of entities touching the given bounds

		This function retrieves all clip models that touch or are within the specified bounds and then filters them to return a list of unique entities. It ensures that each entity appears only once
	   in the returned list even if it has multiple clip models touching the bounds.

		\param bounds the bounding box to check for entity intersections
		\param contentMask the content mask to filter clip models
		\param entityList output array to store the resulting entities
		\param maxCount maximum number of entities to return
		\return the number of entities found that touch the given bounds
		\throws Warning message when the maximum count is exceeded
	*/
	int	 EntitiesTouchingBounds( const idBounds& bounds, int contentMask, idEntity** entityList, int maxCount ) const;

	/*!
		\brief Returns the number of clip models touching the specified bounds.

		This function finds all clip models that intersect with the given bounding box. It performs a recursive search through the clip sector tree to identify intersecting models. The function
	   handles degenerate or backwards bounds by asserting and returning zero. The search is constrained by the content mask to only consider models with matching content types. The results are stored
	   in the provided clipModelList array up to the maximum count specified.

		\param bounds The bounding box to test for intersections
		\param contentMask Mask specifying which content types to consider
		\param clipModelList Output array to store the clip models that touch the bounds
		\param maxCount Maximum number of clip models to store in the output array
		\return The number of clip models that intersect with the specified bounds, or zero if the bounds are degenerate
		\throws assertion failure when bounds are degenerate or backwards
	*/
	int	 ClipModelsTouchingBounds( const idBounds& bounds, int contentMask, idClipModel** clipModelList, int maxCount ) const;

	//! Returns a constant reference to the world bounds of the clip model.
	const idBounds& GetWorldBounds() const;

	//! Returns a pointer to the default clip model used by the engine.
	idClipModel*	DefaultClipModel();

	//! Prints translation, rotation, motion, and other collision statistics.
	void			PrintStatistics();

	//! Draws clip models that are within a specified radius of the eye position, excluding a given entity.
	void			DrawClipModels( const idVec3& eye, const float radius, const idEntity* passEntity );

	//! Draws a visual representation of a contact feature between a clip model and a contact point
	bool			DrawModelContactFeature( const contactInfo_t& contact, const idClipModel* clipModel, int lifetime ) const;

private:
	int					 numClipSectors;
	struct clipSector_s* clipSectors;
	idBounds			 worldBounds;
	idClipModel			 temporaryClipModel;
	idClipModel			 defaultClipModel;
	cmHandle_t			 worldCollisionModel;
	mutable int			 touchCount;
	// statistics
	int					 numTranslations;
	int					 numRotations;
	int					 numMotions;
	int					 numRenderModelTraces;
	int					 numContents;
	int					 numContacts;

private:
	//! Creates a uniformly subdivided tree for the given world size
	struct clipSector_s* CreateClipSectors_r( const int depth, const idBounds& bounds, idVec3& maxSector );

	//! Recursively finds clip models touching a given bounds within a spatial subdivision node.
	void				 ClipModelsTouchingBounds_r( const struct clipSector_s* node, struct listParms_s& parms ) const;

	//! Returns the trace model for a given clip model.
	const idTraceModel*	 TraceModelForClipModel( const idClipModel* mdl ) const;

	/*!
		\brief Returns the number of clip models touching the specified bounds while excluding certain entities based on the pass entity and its owner.

		This function retrieves a list of clip models that touch the given bounds and are filtered based on the content mask. It excludes clip models that belong to the pass entity, its owner, or
	   related entities such as missiles owned by the same entity. The filtering ensures that entities do not interact with themselves or their projectiles. The function modifies the clipModelList by
	   setting certain entries to NULL when they match exclusion criteria.

		\param bounds The bounding volume used to query for intersecting clip models
		\param contentMask A bitmask specifying which content types to include in the query
		\param passEntity The entity whose clip models should be excluded from the results
		\param clipModelList An array to be filled with pointers to the matching clip models
		\return The number of clip models found that match the bounds and content mask, excluding those specified by the pass entity.
	*/
	int					 GetTraceClipModels( const idBounds& bounds, int contentMask, const idEntity* passEntity, idClipModel** clipModelList ) const;

	/*!
		\brief Performs a trace against a render model using the specified parameters and updates the trace result structure.

		This function traces a line from start to end with a specified radius against a render model associated with the touch clip model. It first checks if the line intersects with the expanded
	   bounding box of the clip model, and if so, performs a more precise trace using the render model. The results of the trace are stored in the provided trace structure, including fraction, end
	   position, normal, and material information. The function updates the trace with contact information and sets the joint number as the clip model id.

		\param trace Output trace structure that will be updated with the results of the trace operation
		\param start Starting point of the trace line
		\param end Ending point of the trace line
		\param radius Radius of the trace line
		\param axis Orientation axis for the trace
		\param touch Clip model whose render model will be traced against
	*/
	void				 TraceRenderModel( trace_t& trace, const idVec3& start, const idVec3& end, const float radius, const idMat3& axis, idClipModel* touch ) const;
};

ID_INLINE bool idClip::TracePoint( trace_t& results, const idVec3& start, const idVec3& end, int contentMask, const idEntity* passEntity )
{
	Translation( results, start, end, NULL, mat3_identity, contentMask, passEntity );
	return ( results.fraction < 1.0f );
}

ID_INLINE bool idClip::TraceBounds( trace_t& results, const idVec3& start, const idVec3& end, const idBounds& bounds, int contentMask, const idEntity* passEntity )
{
	temporaryClipModel.LoadModel( idTraceModel( bounds ) );
	Translation( results, start, end, &temporaryClipModel, mat3_identity, contentMask, passEntity );
	return ( results.fraction < 1.0f );
}

ID_INLINE const idBounds& idClip::GetWorldBounds() const
{
	return worldBounds;
}

ID_INLINE idClipModel* idClip::DefaultClipModel()
{
	return &defaultClipModel;
}

#endif /* !__CLIP_H__ */
