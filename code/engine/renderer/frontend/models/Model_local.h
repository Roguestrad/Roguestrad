/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2021 Robert Beckebans
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

#ifndef __MODEL_LOCAL_H__
#define __MODEL_LOCAL_H__

/*
===============================================================================

	Static model

===============================================================================
*/

class idJointMat;
struct deformInfo_t;
struct objModel_t; // RB: Wavefront OBJ support

/*!
	\class idRenderModelStatic
	\brief A static render model class that manages geometric data and surfaces for fixed geometry in the engine.

	This class represents a static render model that stores geometry data and surfaces for fixed objects in the engine. It handles loading models from various file formats, managing surface data, and
   preparing geometry for rendering. The class supports both binary and text-based model formats, and provides functionality for exporting models to OBJ format. It maintains a collection of surfaces
   that can be added, modified, or removed, and includes methods for memory management, model validation, and rendering preparation. The class supports different loading strategies including fast
   loading and full loading modes, and provides mechanisms for tracking model references during level loading. The implementation handles both basic model operations and specialized features like
   joint support for animation data.

*/
class idRenderModelStatic : public idRenderModel
{
public:
	// the inherited public interface
	static idRenderModel* Alloc();

	//! Initializes a new instance of the idRenderModelStatic class with default values.
	idRenderModelStatic();

	//! Destroys the idRenderModelStatic object and purges the associated model data.
	virtual ~idRenderModelStatic();

	//! Initializes a static render model from a file
	virtual void				  InitFromFile( const char* fileName, const idImportOptions* options );

	//! Loads a binary model from a file, validating the file format and timestamp.
	virtual bool				  LoadBinaryModel( idFile* file, const ID_TIME_T sourceTimeStamp, const ID_TIME_T declSourceTimeStamp );

	//! Writes the binary representation of this static render model to the specified file
	virtual void				  WriteBinaryModel( idFile* file, ID_TIME_T* _timeStamp = NULL ) const;

	//! Indicates whether the static render model supports binary model format.
	virtual bool				  SupportsBinaryModel() { return true; }

	//! Exports the static render model data to OBJ and MTL files.
	virtual void				  ExportOBJ( idFile* objFile, idFile* mtlFile, ID_TIME_T* _timeStamp = NULL );

	//! Initializes a static render model from a file with fast loading enabled
	virtual void				  PartialInitFromFile( const char* fileName );

	//! Frees all geometry data associated with the model surfaces and clears the surfaces list
	virtual void				  PurgeModel();
	virtual void				  Reset() {};

	//! Loads the render model data from file
	virtual void				  LoadModel();

	//! Returns true if the static render model has been loaded and not purged.
	virtual bool				  IsLoaded();

	//! Sets whether the static render model is referenced during level loading.
	virtual void				  SetLevelLoadReferenced( bool referenced );

	//! Returns true if the static render model was referenced during level loading.
	virtual bool				  IsLevelLoadReferenced();

	//! Ensures all surfaces in the static render model have their materials referenced and kept loaded.
	virtual void				  TouchData();

	//! Creates GPU buffers for the static render model using the provided command list.
	virtual void				  CreateBuffers( nvrhi::ICommandList* commandList );

	//! Initializes an empty render model with the specified name.
	virtual void				  InitEmpty( const char* name );

	//! Adds a surface to the static render model.
	virtual void				  AddSurface( modelSurface_t surface );

	//! Finalizes model surfaces by cleaning geometry and preparing them for rendering.
	virtual void				  FinishSurfaces( bool useMikktspace );

	//! Frees the vertex cache for all surfaces in the static render model.
	virtual void				  FreeVertexCache();

	//! Returns the name of the static render model.
	virtual const char*			  Name() const;

	//! Prints static model information to the console
	virtual void				  Print() const;

	//! Prints information about the static render model to the console
	virtual void				  List() const;

	//! Returns the total memory usage of the static render model in bytes.
	virtual int					  Memory() const;

	//! Returns the timestamp of the render model static.
	virtual ID_TIME_T			  Timestamp() const;

	//! Returns the timestamp of the model definition file associated with this static render model.
	virtual ID_TIME_T			  DeclTimestamp() const;

	//! Returns the name of the model definition this model was loaded from.
	virtual const char*			  GetModelDefName() const;

	//! Returns the number of surfaces in the static render model.
	virtual int					  NumSurfaces() const;

	//! Returns the number of base surfaces in the static render model
	virtual int					  NumBaseSurfaces() const;

	//! Returns a pointer to the surface at the specified index in the static render model.
	virtual const modelSurface_t* Surface( int surfaceNum ) const;

	//! Allocates and returns a new triangle surface with specified vertex and index counts
	virtual srfTriangles_t*		  AllocSurfaceTriangles( int numVerts, int numIndexes ) const;

	//! Frees the memory associated with the given surface triangles structure.
	virtual void				  FreeSurfaceTriangles( srfTriangles_t* tris ) const;

	//! Returns true if the render model is a static world model.
	virtual bool				  IsStaticWorldModel() const;

	//! Returns DM_STATIC indicating that this model is a static render model.
	virtual dynamicModel_t		  IsDynamicModel() const;

	//! Returns true if this render model is a default model.
	virtual bool				  IsDefaultModel() const;

	//! Returns whether the static render model can be reloaded.
	virtual bool				  IsReloadable() const;

	//! Returns null and errors because static models cannot instantiate dynamic models.
	virtual idRenderModel*		  InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );

	//! Returns the number of joints in the static render model.
	virtual int					  NumJoints() const;

	//! Returns the MD5 joints for the static model or NULL if the model is not an MD5.
	virtual const idMD5Joint*	  GetJoints() const;

	//! Returns the joint handle for a given joint name, or INVALID_JOINT if not found
	virtual jointHandle_t		  GetJointHandle( const char* name ) const;

	//! Returns the name of the joint specified by the given handle
	virtual const char*			  GetJointName( jointHandle_t handle ) const;

	//! Returns the default animation pose for the model or NULL if the model is not an MD5.
	virtual const idJointQuat*	  GetDefaultPose() const;

	/*!
		\brief Returns the joint index closest to the specified triangle vertices on a given surface

		This function is intended to find the nearest joint to a triangle defined by three vertices on a specific surface of the model. The implementation currently returns INVALID_JOINT, suggesting
	   that the actual functionality may be implemented in derived classes like idRenderModelMD5. The parameters represent the surface number and three vertex indices that define a triangle in the
	   mesh. The function is called during ray tracing operations to determine which joint is closest to the intersection point of a trace with the model surface.

		\param surfaceNum Index of the surface to search within
		\param a First vertex index of the triangle
		\param b Second vertex index of the triangle
		\param c Third vertex index of the triangle
		\return The joint index that is closest to the specified triangle vertices, or INVALID_JOINT if no valid joint is found
	*/
	virtual int					  NearestJoint( int surfaceNum, int a, int b, int c ) const;

	//! Returns the bounding box of the static render model.
	virtual idBounds			  Bounds( const struct renderEntity_s* ent ) const;

	//! Returns the depth hack value for the static render model, which is always zero.
	virtual float				  DepthHack() const;

	virtual bool				  ModelHasDrawingSurfaces() const { return hasDrawingSurfaces; };
	virtual bool				  ModelHasInteractingSurfaces() const { return hasInteractingSurfaces; };
	virtual bool				  ModelHasShadowCastingSurfaces() const { return hasShadowCastingSurfaces; };

	//! Initializes the render model with a default cube geometry and material.
	void						  MakeDefaultModel();

	//! Loads a model from an ASE file format.
	bool						  LoadASE( const char* fileName, ID_TIME_T* sourceTimeStamp );

	//! Loads a LightWave Object file for rendering.
	bool						  LoadLWO( const char* fileName, ID_TIME_T* sourceTimeStamp );

	//! Loads a model from a file in the MA format
	bool						  LoadMA( const char* filename, ID_TIME_T* sourceTimeStamp );

	//! Loads a wavefront obj model file and converts it to render model surfaces
	bool						  LoadOBJ( const char* fileName, ID_TIME_T* sourceTimeStamp );

	//! Converts an OBJ model to render model surfaces
	bool						  ConvertOBJToModelSurfaces( const objModel_t* obj );

	//! Converts ASE model data into render model surfaces.
	bool						  ConvertASEToModelSurfaces( const struct aseModel_s* ase );

	//! Converts an LWO object into model surfaces for rendering
	bool						  ConvertLWOToModelSurfaces( const struct st_lwObject* lwo );

	//! Converts a MA model to render model surfaces
	bool						  ConvertMAToModelSurfaces( const struct maModel_s* ma );

	//! Converts a glTF mesh to model surfaces and returns whether the conversion was successful.
	bool						  ConvertGltfMeshToModelsurfaces( const gltfMesh* mesh );

	//! Converts an LWO model object to an ASE model structure
	struct aseModel_s*			  ConvertLWOToASE( const struct st_lwObject* obj, const char* fileName );

	//! Deletes a surface with the specified ID from the static render model.
	bool						  DeleteSurfaceWithId( int id );

	//! Removes surface geometry with negative IDs from the static render model.
	void						  DeleteSurfacesWithNegativeId();

	//! Finds a surface with the specified ID and returns its index.
	bool						  FindSurfaceWithId( int id, int& surfaceNum ) const;

public:
	idList<modelSurface_t, TAG_MODEL> surfaces;
	idBounds						  bounds;
	int								  overlaysAdded;

	// when an md5 is instantiated, the inverted joints array is stored to allow GPU skinning
	int								  numInvertedJoints;
	idJointMat*						  jointsInverted;
	vertCacheHandle_t				  jointsInvertedBuffer;

protected:
	int			  lastModifiedFrame;
	int			  lastArchivedFrame;

	idStr		  name;
	bool		  isStaticWorldModel;
	bool		  defaulted;
	bool		  purged;			   // eventually we will have dynamic reloading
	bool		  fastLoad;			   // don't generate tangents and shadow data
	bool		  reloadable;		   // if not, reloadModels won't check timestamp
	bool		  levelLoadReferenced; // for determining if it needs to be freed
	bool		  hasDrawingSurfaces;
	bool		  hasInteractingSurfaces;
	bool		  hasShadowCastingSurfaces;
	ID_TIME_T	  timeStamp;
	ID_TIME_T	  declTimeStamp;	// RB: only != 0 if initialized from modelDef
	idStr		  declModelDefName; // RB

	static idCVar r_mergeModelSurfaces; // combine model surfaces with the same material
	static idCVar r_slopVertex;			// merge xyz coordinates this far apart
	static idCVar r_slopTexCoord;		// merge texture coordinates this far apart
	static idCVar r_slopNormal;			// merge normals that dot less than this
};

#if !defined( DMAP )

/*!
	\class idMD5Mesh
	\brief Provides functionality for parsing and updating MD5 mesh data with joint-based deformations.

	The idMD5Mesh class manages MD5 format mesh data including parsing mesh information from a lexer, updating surface geometry with joint transformations, and calculating bounding boxes. It supports
   both GPU and CPU skinning modes for vertex deformation. The class handles memory management for vertex and index buffers during surface updates, reusing existing geometry when dimensions match or
   allocating new memory when necessary. It maintains mesh structure information such as vertices, triangles, and weight data for joint-based deformations. The class is designed to work with joint
   transformation matrices to compute final mesh positions and bounds, making it suitable for skeletal animation scenarios.

*/
class idMD5Mesh
{
	friend class idRenderModelMD5;
	friend class idRenderModelGLTF;

public:
	//! Initializes all member variables of the idMD5Mesh object to their default values.
	idMD5Mesh();

	//! Destructor for idMD5Mesh that frees allocated memory for mesh joints and deformation information.
	~idMD5Mesh();

	//! Parses MD5 mesh data from a lexer, including vertices, triangles, and weight information.
	void ParseMesh( idLexer& parser, int numJoints, const idJointMat* joints );

	//! Returns the number of vertices in the mesh.
	int	 NumVerts() const { return numVerts; }

	//! Returns the number of triangles in the MD5 mesh.
	int	 NumTris() const { return numTris; }

	/*!
		\brief Updates the surface geometry for an MD5 mesh using joint transformations and deformation information.

		This function updates the surface data for an MD5 mesh by processing joint transformations and applying deformation information. It handles memory management for vertex and index buffers,
	   determines whether to reuse existing geometry or allocate new memory based on the dimensions of the current and new data. The function supports both GPU and CPU skinning modes, with different
	   code paths for each. When GPU skinning is enabled, vertex data is referenced directly. Otherwise, vertices are transformed on the CPU and copied to newly allocated memory. The function also
	   calculates the bounding box for the updated surface using the provided joint information.

		\param ent Render entity containing transformation data for the surface
		\param joints Array of joint matrices for the entity
		\param entJointsInverted Inverted joint matrices for the entity used for vertex transformation
		\param surf Pointer to the model surface structure to be updated
	*/
	void UpdateSurface( const struct renderEntity_s* ent, const idJointMat* joints, const idJointMat* entJointsInverted, modelSurface_t* surf );

	//! Calculates the bounding box of the mesh using the provided joint transformations.
	void CalculateBounds( const idJointMat* entJoints, idBounds& bounds ) const;

	//! Returns the joint index of the vertex with the highest weight among the three input vertices.
	int	 NearestJoint( int a, int b, int c ) const;

private:
	const idMaterial* shader;			// material applied to mesh
	int				  numVerts;			// number of vertices
	int				  numTris;			// number of triangles
	byte*			  meshJoints;		// the joints used by this mesh
	int				  numMeshJoints;	// number of mesh joints
	float			  maxJointVertDist; // maximum distance a vertex is separated from a joint
	deformInfo_t*	  deformInfo;		// used to create srfTriangles_t from base frames and new vertexes
	int				  surfaceNum;		// number of the static surface created for this mesh
};

/*!
	\class idRenderModelMD5
	\brief A render model implementation for MD5 format meshes with joint animation support.

	This class provides a concrete implementation of a render model specifically designed for MD5 format data, supporting both static and dynamic model operations. It handles loading, memory
   management, and rendering of MD5 meshes with joint animations, including default pose handling and joint lookups. The class supports various file operations such as binary loading and saving, OBJ
   export, and provides debugging visualization capabilities for joints. It is designed to work with a graphics command list for GPU buffer creation and maintains proper model state for dynamic
   instantiation. The implementation includes methods for bounding box calculation, memory usage reporting, and surface-to-joint distance computations.

*/
class idRenderModelMD5 : public idRenderModelStatic
{
	friend class idRenderModelGLTF;

public:
	//! Initializes the MD5 render model from a file.
	void			   InitFromFile( const char* fileName, const idImportOptions* options ) override;

	//! Loads a binary MD5 model from a file, returning true on success.
	bool			   LoadBinaryModel( idFile* file, const ID_TIME_T sourceTimeStamp, const ID_TIME_T declSourceTimeStamp ) override;

	//! Writes the MD5 model data to a binary file including joint information, pose data, and mesh details.
	void			   WriteBinaryModel( idFile* file, ID_TIME_T* _timeStamp = NULL ) const override;

	//! Returns the dynamic model type for the MD5 model, indicating it is cached.
	dynamicModel_t	   IsDynamicModel() const override;

	//! Returns the bounding box for the MD5 model, using either the reference pose bounds or entity-specific bounds.
	idBounds		   Bounds( const struct renderEntity_s* ent ) const override;

	//! Prints information about the MD5 render model to the console
	void			   Print() const override;

	//! Prints a summary of the MD5 model's memory usage, mesh count, vertex count, and triangle count to the console.
	void			   List() const override;

	//! Ensures that all materials used by the model's meshes are kept loaded.
	void			   TouchData() override;

	//! Frees all the model data while keeping the class instance for potential regeneration.
	void			   PurgeModel() override;

	//! Loads the MD5 model data from file or uses a default model if loading fails
	void			   LoadModel() override;

	//! Creates GPU buffers for all meshes in the MD5 model using the provided command list.
	void			   CreateBuffers( nvrhi::ICommandList* commandList ) override;

	//! Returns the total memory usage of this MD5 render model in bytes
	int				   Memory() const override;

	//! Creates a dynamic instance of an MD5 model for rendering based on entity and view parameters
	idRenderModel*	   InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel ) override;

	//! Returns the number of joints in the MD5 model.
	int				   NumJoints() const override;

	//! Returns a pointer to the array of joints for this MD5 model.
	const idMD5Joint*  GetJoints() const override;

	//! Returns the handle for the joint with the specified name
	jointHandle_t	   GetJointHandle( const char* name ) const override;

	//! Returns the name of the joint specified by the given handle.
	const char*		   GetJointName( jointHandle_t handle ) const override;

	//! Returns the default animation pose for the MD5 model.
	const idJointQuat* GetDefaultPose() const override;

	/*!
		\brief Returns the index of the joint nearest to a specified triangle on a given surface

		This function identifies the nearest joint to a triangle defined by three vertex indices on a specified surface. It searches through the mesh data to find the matching surface number and then
	   delegates the calculation to the corresponding mesh's NearestJoint method. The function performs bounds checking on the surface number and will trigger an error if the surface number exceeds
	   the available meshes.

		\param surfaceNum Index of the surface to search within
		\param a First vertex index of the triangle
		\param b Second vertex index of the triangle
		\param c Third vertex index of the triangle
		\return The index of the joint that is nearest to the specified triangle on the given surface
		\throws Error message if surfaceNum exceeds the number of available meshes
	*/
	int				   NearestJoint( int surfaceNum, int a, int b, int c ) const override;

	//! Returns true indicating that the MD5 render model supports binary model format.
	bool			   SupportsBinaryModel() override { return true; }

	//! Exports the MD5 model data to OBJ and MTL files.
	void			   ExportOBJ( idFile* objFile, idFile* mtlFile, ID_TIME_T* _timeStamp = NULL ) override;
	// RB end

private:
	idList<idMD5Joint, TAG_MODEL>  joints;
	idList<idJointQuat, TAG_MODEL> defaultPose;
	idList<idJointMat, TAG_MODEL>  invertedDefaultPose;
	idList<idMD5Mesh, TAG_MODEL>   meshes;

	//! Draws debug visualization of joints for a render model
	void						   DrawJoints( const renderEntity_t* ent, const viewDef_t* view ) const;

	//! Parses joint data from the parser and populates the joint structure with name, parent, and default pose information.
	void						   ParseJoint( idLexer& parser, idMD5Joint* joint, idJointQuat* defaultPose );
};

/*
===============================================================================

	MD3 animated model

===============================================================================
*/

struct md3Header_s;
struct md3Surface_s;

/*!
	\class idRenderModelMD3
	\brief A render model class for handling MD3 format models with dynamic instantiation capabilities.

	This class implements a render model specifically designed for MD3 format assets, extending the functionality of a static render model. It provides initialization from file, dynamic model
   instantiation, and animation interpolation capabilities for mesh surfaces. The class supports binary model format and maintains a cached dynamic model type. It is intended for use in rendering
   systems where MD3 models need to be instantiated dynamically for specific entities and views while maintaining proper animation interpolation between frames.

*/
class idRenderModelMD3 : public idRenderModelStatic
{
public:
	//! Initializes all member variables of the idRenderModelMD3 class to their default values.
	idRenderModelMD3();

	//! Initializes the MD3 model from a file using the specified import options.
	virtual void		   InitFromFile( const char* fileName, const idImportOptions* options );

	//! Indicates whether the MD3 render model supports binary model format.
	virtual bool		   SupportsBinaryModel() { return false; }

	//! Returns the dynamic model type for this MD3 model, which is always DM_CACHED.
	virtual dynamicModel_t IsDynamicModel() const;

	//! Creates a dynamic instance of an MD3 model for rendering with specified entity and view parameters.
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );

	//! Returns the bounding box of the MD3 model for the specified entity.
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

private:
	int						  index;	// model = tr.models[model->index]
	int						  dataSize; // just for listing purposes
	struct md3Header_s*		  md3;		// only if type == MOD_MESH
	int						  numLods;
	idList<const idMaterial*> shaders; // DG: md3Shader_t::shaderIndex indexes into this array

	/*!
		\brief Computes interpolated vertex positions for a mesh surface using MD3 animation data.

		This function calculates the vertex positions for a mesh surface by either copying vertex data directly when no interpolation is needed, or by interpolating between two frames of animation
	   data. The interpolation is controlled by the backlerp parameter which determines the weight of the current frame versus the previous frame. The function updates the triangle structure with the
	   computed vertex positions.

		\param tri Pointer to the triangle structure where computed vertices will be stored
		\param surf Pointer to the MD3 surface data containing vertex information
		\param backlerp Interpolation factor between the current and previous frame (0.0 = current frame only, 1.0 = previous frame only)
		\param frame Index of the current animation frame
		\param oldframe Index of the previous animation frame
	*/
	void					  LerpMeshVertexes( srfTriangles_t* tri, const struct md3Surface_s* surf, const float backlerp, const int frame, const int oldframe ) const;
};

/*!
	\class idRenderModelLiquid
	\brief A specialized render model class for handling liquid physics and deformation effects.

	This class extends the static render model functionality to support dynamic liquid simulation with deformation capabilities. It provides methods for initializing liquid models from files, creating
   dynamic instances for rendering, and updating the simulation state. The class supports bounds calculations, buffer creation for graphics rendering, and various liquid effects such as water drops.
   It is designed to work with a command list for graphics operations and integrates with the rendering system through entity and view parameters. The implementation handles continuous model updates
   and deformation through physics calculations and displacement updates.

*/
class idRenderModelLiquid : public idRenderModelStatic
{
public:
	//! Initializes a new instance of the idRenderModelLiquid class with default values.
	idRenderModelLiquid();

	//! Initializes a liquid render model from a specified file.
	virtual void		   InitFromFile( const char* fileName, nvrhi::ICommandList* commandList, const idImportOptions* options );

	//! Returns false indicating that the liquid render model does not support binary model format.
	virtual bool		   SupportsBinaryModel() { return false; }

	//! Returns the dynamic model type for the liquid model, indicating it is a continuously updating model.
	virtual dynamicModel_t IsDynamicModel() const;

	//! Creates a dynamic instance of a liquid render model for the given entity and view.
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );

	//! Returns the bounding box of the liquid render model.
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

	//! Creates graphics buffers for liquid model deformation using the provided command list
	virtual void		   CreateBuffers( nvrhi::ICommandList* commandList );

	//! Resets the liquid model data and internal state.
	virtual void		   Reset();

	//! Updates the liquid surface displacement within the specified bounds.
	void				   IntersectBounds( const idBounds& bounds, float displacement );

private:
	//! Generates a deformed surface for a liquid model using the specified lerp value
	modelSurface_t				  GenerateSurface( float lerp );

	//! Adds a water drop effect at the specified coordinates on the liquid render model.
	void						  WaterDrop( int x, int y, float* page );

	//! Updates the liquid simulation by applying physics calculations and handling water drops.
	void						  Update();

	int							  verts_x;
	int							  verts_y;
	float						  scale_x;
	float						  scale_y;
	int							  time;
	int							  liquid_type;
	int							  update_tics;
	int							  seed;

	idRandom					  random;

	const idMaterial*			  shader;
	deformInfo_t*				  deformInfo; // used to create srfTriangles_t from base frames
	// and new vertexes

	float						  density;
	float						  drop_height;
	int							  drop_radius;
	float						  drop_delay;

	idList<float, TAG_MODEL>	  pages;
	float*						  page1;
	float*						  page2;

	idList<idDrawVert, TAG_MODEL> verts;

	int							  nextDropTime;
};

/*!
	\class idRenderModelPrt
	\brief A render model implementation for particle systems that supports dynamic instantiation and rendering.

	This class provides a specialized render model for particle systems, extending the functionality of static render models to handle dynamic particle effects. It supports loading particle system
   definitions from files and creating dynamic instances for rendering based on entity and view parameters. The model maintains particle system data and provides bounds information for rendering and
   culling. It does not support binary model format but can be instantiated dynamically for continuous animation behavior. Memory usage is tracked and reported, and the model can be touched to ensure
   proper loading of associated particle data.

*/
class idRenderModelPrt : public idRenderModelStatic
{
public:
	//! Initializes a new instance of the idRenderModelPrt class with particleSystem set to NULL.
	idRenderModelPrt();

	//! Initializes the particle render model from a particle system definition file.
	virtual void		   InitFromFile( const char* fileName, const idImportOptions* options );

	//! Returns false indicating that this model does not support binary model format.
	virtual bool		   SupportsBinaryModel() { return false; }

	//! Ensures the particle system associated with the model is loaded and referenced.
	virtual void		   TouchData();

	//! Returns the dynamic model type, indicating continuous animation behavior
	virtual dynamicModel_t IsDynamicModel() const;

	//! Creates or updates a dynamic particle model instance for rendering based on entity and view parameters
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );

	//! Returns the bounding box of the particle system associated with this render model.
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

	//! Returns the depth hack value for the particle system model.
	virtual float		   DepthHack() const;

	//! Returns the total memory usage of the particle render model in bytes.
	virtual int			   Memory() const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool		   ModelHasDrawingSurfaces() const { return true; };
	virtual bool		   ModelHasInteractingSurfaces() const { return false; };
	virtual bool		   ModelHasShadowCastingSurfaces() const { return false; };

private:
	const idDeclParticle* particleSystem;
};

/*!
	\class idRenderModelBeam
	\brief idRenderModelBeam represents a dynamic beam model that can be instantiated and updated for rendering effects.

	This class extends idRenderModelStatic to provide specialized behavior for beam rendering effects. It is designed to handle dynamic beam models that require regeneration for each view, indicated
   by its DM_CONTINUOUS return value. The class supports instantiation of dynamic model instances based on entity and view parameters, and provides bounds calculation for world-space positioning. The
   model is always considered loaded and supports binary model format loading. The class is intended for use in rendering systems that require continuous updates of beam effects, such as laser beams
   or similar visual effects.

*/
class idRenderModelBeam : public idRenderModelStatic
{
public:
	//! Returns DM_CONTINUOUS to indicate that the beam model should be regenerated for every view.
	virtual dynamicModel_t IsDynamicModel() const;

	//! Indicates whether the beam model supports binary model format loading.
	virtual bool		   SupportsBinaryModel() { return false; }

	//! Returns true indicating the beam model is always considered loaded.
	virtual bool		   IsLoaded() const;

	//! Creates or updates a dynamic model instance for a beam effect based on entity and view parameters
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );

	//! Calculates the world-space bounds of a beam render model, optionally using the provided render entity for precise positioning.
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool		   ModelHasDrawingSurfaces() const { return true; };
	virtual bool		   ModelHasInteractingSurfaces() const { return false; };
	virtual bool		   ModelHasShadowCastingSurfaces() const { return false; };
};

	#define MAX_TRAIL_PTS 20

struct Trail_t {
	int	   lastUpdateTime;
	int	   duration;

	idVec3 pts[MAX_TRAIL_PTS];
	int	   numPoints;
};

/*!
	\class idRenderModelTrail
	\brief A static render model implementation for managing trail effects.

	This class provides a specialized static render model for handling trail effects in the rendering system. It extends the base static render model functionality to support dynamic trail creation
   and rendering. The class is designed to manage multiple trails with specified durations and update their positions over time. It overrides standard model behavior to indicate static nature and does
   not support binary model formats. The trail management methods allow for creating new trails, updating existing trail points, and drawing individual trails with alpha blending. The model cannot
   instantiate dynamic versions and provides bounds for the static representation.

*/
class idRenderModelTrail : public idRenderModelStatic
{
	idList<Trail_t, TAG_MODEL> trails;
	int						   numActive;
	idBounds				   trailBounds;

public:
	idRenderModelTrail();

	virtual dynamicModel_t IsDynamicModel() const;

	//! Returns false indicating that the trail model does not support binary model format.
	virtual bool		   SupportsBinaryModel() { return false; }
	virtual bool		   IsLoaded() const;
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool		   ModelHasDrawingSurfaces() const { return true; };
	virtual bool		   ModelHasInteractingSurfaces() const { return false; };
	virtual bool		   ModelHasShadowCastingSurfaces() const { return false; };

	int					   NewTrail( idVec3 pt, int duration );
	void				   UpdateTrail( int index, idVec3 pt );
	void				   DrawTrail( int index, const struct renderEntity_s* ent, srfTriangles_t* tri, float globalAlpha );
};

/*!
	\class idRenderModelLightning
	\brief A static render model implementation for lightning effects.

	This class represents a specialized static render model designed for lightning effects. It inherits from idRenderModelStatic and provides specific implementations for static model behaviors. The
   class indicates it cannot support dynamic model instantiation and does not support binary model format. It provides bounding volume calculations for static rendering purposes. The model's surface
   interaction capabilities are defined but their specific purposes require clarification. The IsDynamicModel method explicitly identifies this as a static model type.

*/
class idRenderModelLightning : public idRenderModelStatic
{
public:
	virtual dynamicModel_t IsDynamicModel() const;

	//! Returns false indicating that this render model does not support binary model format.
	virtual bool		   SupportsBinaryModel() { return false; }
	virtual bool		   IsLoaded() const;
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool		   ModelHasDrawingSurfaces() const { return true; };
	virtual bool		   ModelHasInteractingSurfaces() const { return false; };
	virtual bool		   ModelHasShadowCastingSurfaces() const { return false; };
};

/*!
	\class idRenderModelSprite
	\brief A render model implementation for sprite-based graphics that supports dynamic updates and various rendering surfaces.

	This class provides a specialized render model for sprite-based graphics, extending the functionality of a static render model. It supports dynamic model instantiation and provides methods to
   determine the model's capabilities regarding drawing, interaction, and shadow casting surfaces. The class is designed to handle sprite-specific rendering requirements including bounds calculation
   and dynamic model updates based on entity and view parameters. The model type indicates continuous updates through the dynamic model flag, making it suitable for animated or frequently changing
   sprite representations.

*/
class idRenderModelSprite : public idRenderModelStatic
{
public:
	//! Returns the dynamic model type for the sprite model, indicating it continuously updates.
	virtual dynamicModel_t IsDynamicModel() const;

	//! Indicates whether the sprite model supports binary model format.
	virtual bool		   SupportsBinaryModel() { return false; }

	//! Returns true if the sprite model is loaded
	virtual bool		   IsLoaded() const;

	//! Creates or updates a dynamic model representation for a sprite entity based on rendering parameters and view information.
	virtual idRenderModel* InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );

	//! Returns the bounding box of the sprite model, with optional expansion based on entity parameters.
	virtual idBounds	   Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool		   ModelHasDrawingSurfaces() const { return true; };
	virtual bool		   ModelHasInteractingSurfaces() const { return false; };
	virtual bool		   ModelHasShadowCastingSurfaces() const { return false; };
};

#endif // #if !defined( DMAP )

#endif /* !__MODEL_LOCAL_H__ */
