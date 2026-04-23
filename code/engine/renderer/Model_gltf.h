/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2022 Harrie van Ginneken

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

#pragma once
#include "Model_local.h"

/*!
	\class idRenderModelGLTF
	\brief A render model implementation for GLTF format with support for animation and skeletal skinning.

	This class provides a concrete implementation of a render model specifically designed for GLTF files. It inherits from idRenderModelStatic and implements all necessary methods to load, manage, and
   render GLTF-based models with support for skeletal animation, joint transformations, and surface-level skinning. The class handles both binary and text-based GLTF formats, supports dynamic model
   instantiation, and provides methods to query joint information, default poses, and surface-level joint influence. It includes functionality for processing GLTF node hierarchies, converting mesh
   data to renderable surfaces, and updating model surfaces with skinning transformations. The class also manages memory, resource loading, and model state transitions, making it suitable for use in a
   3D rendering engine that utilizes GLTF as its primary model format.

*/
class idRenderModelGLTF : public idRenderModelStatic
{
public:
	//! Initializes a render model from a GLTF file with specified options
	virtual void			   InitFromFile( const char* fileName, const idImportOptions* options ) override;

	//! Loads a binary GLTF model from a file and initializes its animation and joint data.
	virtual bool			   LoadBinaryModel( idFile* file, const ID_TIME_T sourceTimeStamp, const ID_TIME_T declSourceTimeStamp ) override;

	//! Writes the binary model data to the specified file, including GLTF-specific headers and metadata.
	virtual void			   WriteBinaryModel( idFile* file, ID_TIME_T* _timeStamp = NULL ) const override;

	//! Returns the dynamic model state of this GLTF model
	virtual dynamicModel_t	   IsDynamicModel() const override;

	//! Returns the bounding box of the GLTF model, either for the reference pose or the specified entity.
	virtual idBounds		   Bounds( const struct renderEntity_s* ent ) const override;

	//! Prints the GLTF model data to the console
	virtual void			   Print() const override;

	//! Outputs the GLTF model name to the console.
	virtual void			   List() const override;

	//! Ensures all surfaces of the GLTF model have their associated shaders loaded by touching the data.
	virtual void			   TouchData() override;

	//! Frees all resources associated with the GLTF model and resets its internal state.
	virtual void			   PurgeModel() override;

	//! Loads and initializes the GLTF model data including bones and pose information
	virtual void			   LoadModel() override;

	//! Returns the memory usage of the GLTF render model.
	virtual int				   Memory() const override;

	//! Creates a dynamic instance of a GLTF model for rendering based on entity and view parameters
	virtual idRenderModel*	   InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel ) override;

	//! Returns the number of joints in the GLTF model
	virtual int				   NumJoints() const override;

	//! Returns a pointer to the first joint in the model's joint array or null if there are no joints.
	virtual const idMD5Joint*  GetJoints() const override;

	//! Returns the joint handle for a joint with the specified name, or INVALID_JOINT if not found.
	virtual jointHandle_t	   GetJointHandle( const char* name ) const override;

	//! Returns the name of the joint corresponding to the given handle
	virtual const char*		   GetJointName( jointHandle_t handle ) const override;

	//! Returns a pointer to the default pose animation data for this GLTF model
	virtual const idJointQuat* GetDefaultPose() const override;

	/*!
		\brief Returns the joint index with the highest weight for a given surface and triangle vertex indices

		This function determines the nearest joint for a specified surface and triangle vertex indices by examining the vertex weights. It processes the vertices of the surface and returns the joint
	   index associated with the highest weight. If no valid vertex indices are provided, it returns 0 and issues a warning.

		\param surfaceNum Index of the surface to process
		\param a Index of the first vertex in the triangle
		\param b Index of the second vertex in the triangle
		\param c Index of the third vertex in the triangle
		\return The joint index with the highest weight among the specified vertices
		\throws Warning message if the surface is not found or no valid vertex indices are provided
	*/
	virtual int				   NearestJoint( int surfaceNum, int a, int b, int c ) const override;

	//! Indicates whether the GLTF render model supports binary model format.
	virtual bool			   SupportsBinaryModel() override { return true; }

	//! Retrieves or generates an animation binary file for a given animation name from a GLTF model.
	static idFile_Memory*	   GetAnimBin( const idStr& animName, const ID_TIME_T sourceTimeStamp, const idImportOptions* options );

	//! Returns the root node ID of the GLTF model.
	int						   GetRootID() const { return rootID; }

private:
	/*!
		\brief Recursively processes a GLTF node and its children to convert mesh data into renderable surfaces

		This function recursively traverses the GLTF node hierarchy and converts mesh data into renderable surfaces. For each node that contains mesh data, it processes the primitives within the mesh
	   and converts them into triangle surfaces with appropriate materials. The function handles hierarchical transformations by accumulating the parent transformation matrix with the current node's
	   transformation matrix. Each processed primitive becomes a separate surface in the model, and the function recursively processes all child nodes to build the complete model hierarchy.

		\param modelNode Pointer to the current GLTF node being processed
		\param parentTransform Transformation matrix from the parent node in the hierarchy
		\param globalTransform Global transformation matrix applied to the entire model
		\param data Pointer to the GLTF data containing mesh and material information
	*/
	void				   ProcessNode_r( gltfNode* modelNode, const idMat4& parentTransform, const idMat4& globalTransform, gltfData* data );

	/*!
		\brief Updates a model surface by applying skinning and calculating bounds based on joint transformations.

		This function processes a model surface for rendering by applying skeletal skinning transformations to vertices. It handles both GPU and CPU skinning paths based on configuration. The function
	   allocates or reuses vertex and index buffers, copies vertex data, transforms vertices and tangents using joint matrices, and computes the bounding volume for the surface. The skinning operation
	   is performed using either GPU acceleration or traditional CPU-based matrix transformations. The function also calculates the final bounds of the skinned surface by examining all joint positions
	   and expanding them by a maximum joint vertex distance.

		\param ent Pointer to the render entity containing transformation and joint information
		\param entJoints Array of joint transformations for the entity
		\param entJointsInverted Array of inverted joint transformations used for skinning
		\param surf Pointer to the model surface to update
		\param sourceSurf Reference to the source surface containing original geometry data
	*/
	void				   UpdateSurface( const struct renderEntity_s* ent, const idJointMat* entJoints, const idJointMat* entJointsInverted, modelSurface_t* surf, const modelSurface_t& sourceSurf );

	//! Updates MD5 joints for the GLTF render model.
	void				   UpdateMd5Joints();

	//! Returns the MD5 joint with the specified name from the model
	const idMD5Joint*	   FindMD5Joint( const idStr& name ) const;

	gltfData*			   data;
	gltfNode*			   root;
	int					   rootID;

	bool				   fileExclusive;
	bool				   hasAnimations;

	float				   maxJointVertDist = 10.0f; // maximum distance a vertex is separated from a joint
	idList<int, TAG_MODEL> animIds;
	idList<int, TAG_MODEL> bones;
	idList<int, TAG_MODEL> MeshNodeIds;
	dynamicModel_t		   model_state;
	idStr				   rootName;
	idStr				   gltfFileName;
	idStr				   commandLine;

	idList<idMD5Joint, TAG_MODEL>  md5joints;
	idList<idJointQuat, TAG_MODEL> defaultPose;
	idList<idJointMat, TAG_MODEL>  invertedDefaultPose;
	gltfSkin*					   currentSkin;

	// derived reimport options
	idMat4						   globalTransform; // Blender to Doom + exta scaling, rotation
private:
	//! Draws joint visualization for a render entity
	void DrawJoints( const struct renderEntity_s* ent, const viewDef_t* view );
};
