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
#include "containers/StrList.h"
#include <functional>
#include "math/Quat.h"
#include "Lib.h"
#include "containers/List.h"

enum gltfProperty {
	INVALID,
	ASSET,
	ACCESSOR,
	CAMERAS,
	SCENE,
	SCENES,
	NODES,
	MATERIALS,
	MESHES,
	TEXTURES,
	IMAGES,
	ACCESSORS,
	BUFFERVIEWS,
	SAMPLERS,
	BUFFERS,
	ANIMATIONS,
	SKINS,
	EXTENSIONS,
	EXTENSIONS_USED,
	EXTENSIONS_REQUIRED
};

class gltfData;

struct gltf_accessor_component {
	enum Type { _byte, _uByte, _short, _uShort, _uInt, _float, _double, Count };
};

template<class T>
struct gltf_accessor_component_type_map {
	idStr stringID;
	int	  id;
	T	  type;
	uint  sizeInBytes; // single element
};

/*!
	\class gltfExtra
	\brief The gltfExtra class provides a container for additional data associated with glTF assets.
*/
class gltfExtra
{
public:
	//! Default constructor for the gltfExtra class.
	gltfExtra() { }
	// entire extra json scope
	idStr  json;
	// str:str pairs of each item
	idDict strPairs;
};

class gltfExt_KHR_lights_punctual;

/*!
	\class gltfExtensions
	\brief A class for handling glTF extensions.
*/
class gltfExtensions
{
public:
	//! Initializes a new instance of the gltfExtensions class.
	gltfExtensions() { }
	idList<gltfExt_KHR_lights_punctual*> KHR_lights_punctual;
};

/*!
	\class gltfNode_KHR_lights_punctual
	\brief Represents a node with punctual light information in a glTF asset.
*/
class gltfNode_KHR_lights_punctual
{
public:
	int light;
};

/*!
	\class gltfNode_Extensions
	\brief A class representing extensions for glTF nodes.
*/
class gltfNode_Extensions
{
public:
	//! Default constructor for gltfNode_Extensions that initializes the KHR_lights_punctual pointer to nullptr.
	gltfNode_Extensions() :
		KHR_lights_punctual( nullptr )
	{
	}
	gltfNode_KHR_lights_punctual* KHR_lights_punctual;
};

class gltfExt_KHR_materials_pbrSpecularGlossiness;

/*!
	\class gltfMaterial_Extensions
	\brief A class for handling GLTF material extensions.
*/
class gltfMaterial_Extensions
{
public:
	/*!
		\brief Constructs a gltfMaterial_Extensions object with default values.

		Initializes the gltfMaterial_Extensions object by setting the KHR_materials_pbrSpecularGlossiness member to nullptr.

	*/
	gltfMaterial_Extensions() :
		KHR_materials_pbrSpecularGlossiness( nullptr )
	{
	}
	gltfExt_KHR_materials_pbrSpecularGlossiness* KHR_materials_pbrSpecularGlossiness;
};

/*!
	\class gltfNode
	\brief A node structure for GLTF asset hierarchy management.
*/
class gltfNode
{
public:
	/*!
		\brief Constructs a new gltfNode object with default values.

		Initializes all member variables of the gltfNode class to their default states. The camera, skin, and mesh indices are set to -1, indicating uninitialized state. The transformation matrix is
	   initialized to mat4_zero, and the rotation, scale, and translation are set to identity or zero values. The parent pointer is set to nullptr, and the dirty flag is initialized to true.

	*/
	gltfNode() :
		camera( -1 ),
		skin( -1 ),
		matrix( mat4_zero ),
		mesh( -1 ),
		rotation( 0.f, 0.f, 0.f, 1.f ),
		scale( 1.f, 1.f, 1.f ),
		translation( vec3_zero ),
		parent( nullptr ),
		dirty( true )
	{
	}
	// Only checks name!
	bool				operator==( const gltfNode& rhs ) { return name == rhs.name; }
	int					camera;
	idList<int>			children;
	int					skin;
	idMat4				matrix;
	int					mesh;
	idQuat				rotation;
	idVec3				scale;
	idVec3				translation;
	idList<double>		weights;
	idStr				name;
	gltfNode_Extensions extensions;
	gltfExtra			extras;

	//
	gltfNode*			parent;
	bool				dirty;
};

struct gltfCameraNodePtrs {
	gltfNode* translationNode = nullptr;
	gltfNode* orientationNode = nullptr;
};

/*!
	\class gltfScene
	\brief A class for managing and organizing 3D scene data loaded from GLTF files.
*/
class gltfScene
{
public:
	//! Default constructor for the gltfScene class.
	gltfScene() { }
	idList<int> nodes;
	idStr		name;
	idStr		extensions;
	gltfExtra	extras;
};

/*!
	\class gltfMesh_Primitive_Attribute
	\brief A class representing a mesh primitive attribute within a glTF structure.
*/
class gltfMesh_Primitive_Attribute
{
public:
	enum Type {
		Position,
		Normal,
		Tangent,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,
		Color0,
		Color1,
		Color2,
		Color3,
		Weight,
		Joints, // joint indices
		Count
	};

	/*!
		\brief Initializes a gltfMesh_Primitive_Attribute object with default values.

		The constructor initializes the accessorIndex to -1, elementSize to 0, and type to Type::Count, setting up the attribute with default state values.

	*/
	gltfMesh_Primitive_Attribute() :
		accessorIndex( -1 ),
		elementSize( 0 ),
		type( gltfMesh_Primitive_Attribute::Type::Count )
	{
	}
	idStr attributeSemantic;
	int	  accessorIndex;
	uint  elementSize;

	Type  type;
};

struct gltf_mesh_attribute_map {
	idStr							   stringID;
	gltfMesh_Primitive_Attribute::Type attib;
	uint							   elementSize;
};

/*!
	\class gltfMesh_Primitive
	\brief Represents a primitive element within a GLTF mesh structure.
*/
class gltfMesh_Primitive
{
public:
	//! Constructs a new gltfMesh_Primitive object with default values for indices, material, and mode.
	gltfMesh_Primitive() :
		indices( -1 ),
		material( -1 ),
		mode( -1 )
	{
	}
	idList<gltfMesh_Primitive_Attribute*> attributes;
	int									  indices;
	int									  material;
	int									  mode;
	idStr								  target;
	idStr								  extensions;
	gltfExtra							  extras;
};

/*!
	\class gltfMesh
	\brief A class representing a mesh loaded from a GLTF file.
*/
class gltfMesh
{
public:
	gltfMesh() {};

	idList<gltfMesh_Primitive*> primitives; // gltfMesh_Primitive[1,*]
	idList<double>				weights;	// number[1,*]
	idStr						name;
	idStr						extensions;
	gltfExtra					extras;
};

/*!
	\class gltfCamera_Orthographic
	\brief Orthographic camera configuration class for GLTF-based rendering.
*/
class gltfCamera_Orthographic
{
public:
	gltfCamera_Orthographic() :
		xmag( 0.0f ),
		ymag( 0.0f ),
		zfar( 0.0f ),
		znear( 0.0f ) {};
	float	  xmag;
	float	  ymag;
	float	  zfar;
	float	  znear;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfCamera_Perspective
	\brief A class representing a perspective camera configuration for GLTF model loading.
*/
class gltfCamera_Perspective
{
public:
	gltfCamera_Perspective() :
		aspectRatio( 0.0f ),
		yfov( 0.0f ),
		zfar( 0.0f ),
		znear( 0.0f ) {};
	float	  aspectRatio;
	float	  yfov;
	float	  zfar;
	float	  znear;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfCamera
	\brief A class representing a camera in a GLTF structure.
*/
class gltfCamera
{
public:
	gltfCamera() {};
	gltfCamera_Orthographic orthographic;
	gltfCamera_Perspective	perspective;
	idStr					type;
	idStr					name;
	idStr					extensions;
	gltfExtra				extras;
};

/*!
	\class gltfAnimation_Channel_Target
	\brief Represents a target for animation channels in glTF files.
*/
class gltfAnimation_Channel_Target
{
public:
	gltfAnimation_Channel_Target() :
		node( -1 ),
		TRS( gltfTRS::count ) {};
	int		  node;
	idStr	  path;
	idStr	  extensions;
	gltfExtra extras;

	enum gltfTRS { none, rotation, translation, scale, weights, count };

	gltfTRS		   TRS;

	/*!
		\brief Resolves a string type to its corresponding gltfTRS enumeration value

		This function takes a string representation of a transformation type and maps it to the appropriate gltfTRS enumeration value. It handles four specific cases: "translation", "rotation",
	   "scale", and "weights". If the input type does not match any of these cases, it returns gltfTRS::count as a default value

		\param type String representation of the transformation type
		\return The corresponding gltfTRS enumeration value for the given type string
	*/
	static gltfTRS resolveType( idStr type )
	{
		if( type == "translation" ) {
			return gltfTRS::translation;
		} else if( type == "rotation" ) {
			return gltfTRS::rotation;
		} else if( type == "scale" ) {
			return gltfTRS::scale;
		} else if( type == "weights" ) {
			return gltfTRS::weights;
		}
		return gltfTRS::count;
	}
};

/*!
	\class gltfAnimation_Channel
	\brief Represents a channel in an animation within a glTF file.
*/
class gltfAnimation_Channel
{
public:
	gltfAnimation_Channel() :
		sampler( -1 ) {};
	int							 sampler;
	gltfAnimation_Channel_Target target;
	idStr						 extensions;
	gltfExtra					 extras;
};

/*!
	\class gltfAnimation_Sampler
	\brief A utility class for handling GLTF animation sampler interpolation types.
*/
class gltfAnimation_Sampler
{
public:
	gltfAnimation_Sampler() :
		input( -1 ),
		interpolation( "LINEAR" ),
		output( -1 ),
		intType( gltfInterpType::count ) {};
	int		  input;
	idStr	  interpolation;
	int		  output;
	idStr	  extensions;
	gltfExtra extras;

	enum gltfInterpType { linear, step, cubicSpline, count };

	gltfInterpType		  intType;

	/*!
		\brief Resolves a string type identifier into its corresponding gltfInterpType enumeration value.

		This function maps string representations of interpolation types used in GLTF animations to their respective enumeration values. It supports three interpolation types: LINEAR, STEP, and
	   CUBICSPLINE. If the input string does not match any known type, it returns gltfInterpType::count as a default or invalid value.

		\param type String identifier for the interpolation type to resolve.
		\return The corresponding gltfInterpType enumeration value for the given string type, or gltfInterpType::count if the type is unrecognized.
	*/
	static gltfInterpType resolveType( idStr type )
	{
		if( type == "LINEAR" ) {
			return gltfInterpType::linear;
		} else if( type == "STEP" ) {
			return gltfInterpType::step;
		} else if( type == "CUBICSPLINE" ) {
			return gltfInterpType::cubicSpline;
		}
		return gltfInterpType::count;
	}
};

/*!
	\class gltfAnimation
	\brief Provides animation data and querying capabilities for GLTF animations.

	This class represents animation data for GLTF animations and offers methods to query animation properties such as bounds, origin offset, and origin rotation at specific times and cycle counts. It
   also tracks reference counts to manage animation lifecycle. The class is designed to be a const interface for retrieving animation state information, with some methods currently returning false to
   indicate incomplete implementation. The NumFrames method provides the total frame count for the animation.

*/
class gltfAnimation
{
public:
	gltfAnimation() :
		maxTime( 0.0f ),
		numFrames( 0 ) {};
	idList<gltfAnimation_Channel*> channels;
	idList<gltfAnimation_Sampler*> samplers;
	idStr						   name;
	idStr						   extensions;
	gltfExtra					   extras;

	float						   maxTime;

	// id specific
	mutable int					   ref_count;
	int							   numFrames;
	void						   DecreaseRefs() const { ref_count--; };
	void						   IncreaseRefs() const { ref_count++; };

	/*!
		\brief Determines the bounding volume of the animation at a specific time and cycle count

		This function computes and returns the bounding volume of the animation at the specified time and cycle count. It is typically used to determine the spatial extent of an animated object for
	   culling or collision detection purposes. The function returns false indicating that the actual implementation is not provided or not functional in this specific case

		\param bnds The bounding volume to be set by the function
		\param time The time value to determine the animation state
		\param cyclecount The number of cycles to consider when calculating the bounds
		\return false indicating the function does not provide a valid bounding volume computation
	*/
	bool						   GetBounds( idBounds& bnds, int time, int cyclecount ) const { return false; }

	/*!
		\brief Returns false, indicating that origin rotation data is not available or cannot be retrieved.

		This function is a placeholder implementation that always returns false. It is intended to retrieve the rotation value for a given time and cycle count from an animation, but currently does
	   not perform any meaningful operation. The function takes a quaternion reference to store the result, a time value, and a cycle count parameter, but ignores them in the current implementation.

		\param rotation Reference to store the resulting rotation quaternion
		\param time The time value for which to retrieve the rotation
		\param cyclecount The cycle count to consider when retrieving the rotation
		\return False, indicating that the operation failed or was not implemented.
	*/
	bool						   GetOriginRotation( idQuat& rotation, int time, int cyclecount ) const { return false; }

	/*!
		\brief Retrieves the origin offset for a given time and cycle count from a GLTF animation

		This function attempts to calculate and return the origin offset for a specific time and cycle count within a GLTF animation. It is designed to be a const member function, indicating it does
	   not modify the object's state. The function returns false in its current implementation, suggesting that the actual implementation may still be pending or incomplete. The parameters include an
	   output reference to an idVec3 for the offset, an integer time value, and an integer cycle count.

		\param offset Output parameter that will contain the calculated origin offset
		\param time The time value for which the origin offset is calculated
		\param cyclecount The cycle count used in the calculation of the origin offset
		\return False indicating that the function does not successfully retrieve the origin offset in its current implementation
	*/
	bool						   GetOrigin( idVec3& offset, int time, int cyclecount ) const { return false; }

	/*!
		\brief Returns the number of frames in the animation.
		\return The total number of frames contained in this animation
	*/
	int							   NumFrames() const { return numFrames; }
};

/*!
	\class gltfAccessor_Sparse_Values
	\brief A class representing sparse values in a glTF accessor.
*/
class gltfAccessor_Sparse_Values
{
public:
	gltfAccessor_Sparse_Values() :
		bufferView( -1 ),
		byteOffset( -1 ) {};
	int		  bufferView;
	int		  byteOffset;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfAccessor_Sparse_Indices
	\brief A class representing sparse index data for GPU mesh rendering.
*/
class gltfAccessor_Sparse_Indices
{
public:
	gltfAccessor_Sparse_Indices() :
		bufferView( -1 ),
		byteOffset( -1 ),
		componentType( -1 ) {};
	int		  bufferView;
	int		  byteOffset;
	int		  componentType;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfAccessor_Sparse
	\brief A class representing a sparse accessor for glTF data.
*/
class gltfAccessor_Sparse
{
public:
	gltfAccessor_Sparse() :
		count( -1 ) {};
	int							count;
	gltfAccessor_Sparse_Indices indices;
	gltfAccessor_Sparse_Values	values;
	idStr						extensions;
	gltfExtra					extras;
};

/*!
	\class gltfAccessor
	\brief A class representing a glTF accessor for handling buffer data in a 3D graphics context.
*/
class gltfAccessor
{
public:
	/*!
		\brief Initializes a gltfAccessor object with default values for its member variables.

		This constructor initializes all member variables of the gltfAccessor class to their default values. The bufferView, componentType, and count are set to -1 to indicate uninitialized state. The
	   normalized flag is set to false. All pointer members (floatView, vecView, quatView, matView) are initialized to nullptr.

	*/
	gltfAccessor() :
		bufferView( -1 ),
		byteOffset( 0 ),
		componentType( -1 ),
		normalized( false ),
		count( -1 ),
		floatView( nullptr ),
		vecView( nullptr ),
		quatView( nullptr ),
		matView( nullptr )
	{
	}
	int					bufferView;
	int					byteOffset;
	int					componentType;
	bool				normalized;
	int					count;
	idStr				type;
	idList<double>		max;
	idList<double>		min;
	gltfAccessor_Sparse sparse;
	idStr				name;
	idStr				extensions;
	gltfExtra			extras;

	uint				typeSize;

	idList<float>*		floatView;
	idList<idVec3*>*	vecView;
	idList<idQuat*>*	quatView;
	idList<idMat4>*		matView;
};

/*!
	\class gltfBufferView
	\brief A class representing a buffer view in a GLTF asset.
*/
class gltfBufferView
{
public:
	gltfBufferView() :
		buffer( -1 ),
		byteLength( -1 ),
		byteStride( 0 ),
		byteOffset( 0 ),
		target( -1 ) {};
	int		  buffer;
	int		  byteLength;
	int		  byteStride;
	int		  byteOffset;
	int		  target;
	idStr	  name;
	idStr	  extensions;
	gltfExtra extras;
	//
	gltfData* parent;
};

/*!
	\class gltfBuffer
	\brief A class for managing glTF buffer data.
*/
class gltfBuffer
{
public:
	gltfBuffer() :
		byteLength( -1 ),
		parent( nullptr ) {};
	idStr	  uri;
	int		  byteLength;
	idStr	  name;
	idStr	  extensions;
	gltfExtra extras;
	//
	gltfData* parent;
};

/*!
	\class gltfSampler
	\brief A class for managing GLTF sampler data.
*/
class gltfSampler
{
public:
	gltfSampler() :
		magFilter( 0 ),
		minFilter( 0 ),
		wrapS( 10497 ),
		wrapT( 10497 ) {};
	int		  magFilter;
	int		  minFilter;
	int		  wrapS;
	int		  wrapT;
	idStr	  name;
	idStr	  extensions;
	gltfExtra extras;
	//
	uint	  bgfxSamplerFlags;
};

/*!
	\class gltfImage
	\brief A class representing an image resource within a GLTF asset.
*/
class gltfImage
{
public:
	/*!
		\brief Initializes a gltfImage object with default values.

		The constructor initializes the bufferView member to -1, which typically indicates an invalid or uninitialized buffer view index in GLTF asset handling.

	*/
	gltfImage() :
		bufferView( -1 )
	{
	}
	idStr	  uri;
	idStr	  mimeType;
	int		  bufferView;
	idStr	  name;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfSkin
	\brief Manages skinning data for GLTF models.
*/
class gltfSkin
{
public:
	gltfSkin() :
		inverseBindMatrices( -1 ),
		skeleton( -1 ),
		name( "unnamedSkin" ) {};
	int			inverseBindMatrices;
	int			skeleton; // node ID
	idList<int> joints;	  // integer[1,*]
	idStr		name;
	idStr		extensions;
	gltfExtra	extras;
};

class gltfExt_KHR_texture_transform;

/*!
	\class gltfTexture_Info_Extensions
	\brief A class representing texture extension information for GLTF texture data.
*/
class gltfTexture_Info_Extensions
{
public:
	/*!
		\brief Constructs a gltfTexture_Info_Extensions object with default values.

		Initializes the KHR_texture_transform member to nullptr in the constructor's initializer list. This sets up the object with its default state, where no texture transform extension data is
	   associated with the texture information.

	*/
	gltfTexture_Info_Extensions() :
		KHR_texture_transform( nullptr )
	{
	}
	gltfExt_KHR_texture_transform* KHR_texture_transform;
};

/*!
	\class gltfOcclusionTexture_Info
	\brief This class encapsulates information about occlusion textures used in GLTF file processing.
*/
class gltfOcclusionTexture_Info
{
public:
	/*!
		\brief Initializes a gltfOcclusionTexture_Info object with default values for index, texCoord, and strength.

		The constructor sets the index to -1, texCoord to 0, and strength to 1.0f, providing default initialization for occlusion texture information used in GLTF file processing.

	*/
	gltfOcclusionTexture_Info() :
		index( -1 ),
		texCoord( 0 ),
		strength( 1.0f )
	{
	}
	int							index;
	int							texCoord;
	float						strength;
	gltfTexture_Info_Extensions extensions;
	gltfExtra					extras;
};

/*!
	\class gltfNormalTexture_Info
	\brief A structure for storing normal texture information in GLTF format.
*/
class gltfNormalTexture_Info
{
public:
	/*!
		\brief Initializes a new instance of the gltfNormalTexture_Info struct with default values for index, texCoord, and scale.

		This constructor sets the index to -1, texCoord to 0, and scale to 1.0f, providing default values for a normal texture info structure used in GLTF format processing.

	*/
	gltfNormalTexture_Info() :
		index( -1 ),
		texCoord( 0 ),
		scale( 1.0f )
	{
	}
	int							index;
	int							texCoord;
	float						scale;
	gltfTexture_Info_Extensions extensions;
	gltfExtra					extras;
};

/*!
	\class gltfTexture_Info
	\brief Manages texture information for GLTF file processing.
*/
class gltfTexture_Info
{
public:
	/*!
		\brief Constructs a default gltfTexture_Info object with default index and texCoord values.

		Initializes the gltfTexture_Info object with index set to -1 and texCoord set to 0. This default constructor is used to create texture information objects that will later be populated with
	   actual texture data from GLTF files.

	*/
	gltfTexture_Info() :
		index( -1 ),
		texCoord( 0 )
	{
	}
	int							index;
	int							texCoord;
	gltfTexture_Info_Extensions extensions;
	gltfExtra					extras;
};

/*!
	\class gltfTexture
	\brief A class representing a texture component within a glTF asset.
*/
class gltfTexture
{
public:
	/*!
		\brief Initializes a gltfTexture object with default sampler and source values.

		The constructor initializes the sampler and source member variables to -1, indicating that no valid sampler or source has been assigned yet. This is a default constructor for the gltfTexture
	   class.

	*/
	gltfTexture() :
		sampler( -1 ),
		source( -1 )
	{
	}
	int							sampler;
	int							source;
	idStr						name;
	gltfTexture_Info_Extensions extensions;
	gltfExtra					extras;
};

/*!
	\class gltfMaterial_pbrMetallicRoughness
	\brief A struct representing physically based rendering material properties for metallic-roughness workflow.
*/
class gltfMaterial_pbrMetallicRoughness
{
public:
	/*!
		\brief Initializes a gltfMaterial_pbrMetallicRoughness object with default base color, metallic, and roughness factors.

		This constructor initializes the base color factor to white, the metallic factor to 1.0, and the roughness factor to 1.0, providing default values for physically based rendering material
	   properties.

	*/
	gltfMaterial_pbrMetallicRoughness() :
		baseColorFactor( vec4_one ),
		metallicFactor( 1.0f ),
		roughnessFactor( 1.0f )
	{
	}
	idVec4			 baseColorFactor;
	gltfTexture_Info baseColorTexture;
	float			 metallicFactor;
	float			 roughnessFactor;
	gltfTexture_Info metallicRoughnessTexture;
	idStr			 extensions;
	gltfExtra		 extras;
};

/*!
	\class gltfMaterial
	\brief A material class for handling GLTF material properties and alpha mode resolution.
*/
class gltfMaterial
{
public:
	enum gltfAlphaMode { gltfOPAQUE, gltfMASK, gltfBLEND, count };

	//! Initializes a new gltfMaterial instance with default values for all properties.
	gltfMaterial() :
		emissiveFactor( vec3_zero ),
		alphaMode( "OPAQUE" ),
		alphaCutoff( 0.5f ),
		doubleSided( false )
	{
	}
	gltfMaterial_pbrMetallicRoughness pbrMetallicRoughness;
	gltfNormalTexture_Info			  normalTexture;
	gltfOcclusionTexture_Info		  occlusionTexture;
	gltfTexture_Info				  emissiveTexture;
	idVec3							  emissiveFactor;
	idStr							  alphaMode;
	float							  alphaCutoff;
	bool							  doubleSided;
	idStr							  name;
	gltfMaterial_Extensions			  extensions;
	gltfExtra						  extras;

	gltfAlphaMode					  intType;

	/*!
		\brief Resolves a string representation of an alpha mode into its corresponding enumerated value.

		This function takes a string input representing an alpha mode and maps it to the appropriate enumerated value from the gltfAlphaMode enumeration. It supports three specific modes: OPAQUE,
	   MASK, and BLEND. If the input string does not match any of these known modes, it returns the count value, which typically indicates an invalid or unspecified mode.

		\param type String representation of the alpha mode to resolve.
		\return The corresponding gltfAlphaMode enumeration value for the given string, or gltfAlphaMode::count if the string does not match any known mode.
	*/
	static gltfAlphaMode			  resolveAlphaMode( idStr type )
	{
		if( type == "OPAQUE" ) {
			return gltfAlphaMode::gltfOPAQUE;
		} else if( type == "MASK" ) {
			return gltfAlphaMode::gltfMASK;
		} else if( type == "BLEND" ) {
			return gltfAlphaMode::gltfBLEND;
		}
		return gltfAlphaMode::count;
	}
};

/*!
	\class gltfAsset
	\brief A class for managing glTF asset data.
*/
class gltfAsset
{
public:
	//! Constructor for the gltfAsset class that initializes an empty glTF asset.
	gltfAsset() { }
	idStr	  copyright;
	idStr	  generator;
	idStr	  version;
	idStr	  minVersion;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfExtensionsUsed
	\brief Container for tracking used GLTF extensions.
*/
class gltfExtensionsUsed
{
public:
	//! Initializes an empty gltfExtensionsUsed object.
	gltfExtensionsUsed() { }
	idStr extension;
};

/*!
	\class gltfExt_KHR_materials_pbrSpecularGlossiness
	\brief Manages GLTF material properties for specular glossiness workflow.
*/
class gltfExt_KHR_materials_pbrSpecularGlossiness
{
public:
	/*!
		\brief Constructs a new instance of the gltfExt_KHR_materials_pbrSpecularGlossiness class.

		This is a default constructor for the gltfExt_KHR_materials_pbrSpecularGlossiness class. It initializes a new instance with default values for all member variables. The constructor body is
	   empty, indicating that no specific initialization is required beyond default construction of member variables.

	*/
	gltfExt_KHR_materials_pbrSpecularGlossiness() { }
	idVec4			 diffuseFactor;
	gltfTexture_Info diffuseTexture;
	idVec3			 specularFactor;
	float			 glossinessFactor;
	gltfTexture_Info specularGlossinessTexture;
	idStr			 extensions;
	gltfExtra		 extras;
};

/*!
	\class gltfExt_KHR_lights_punctual_spot
	\brief A class representing GLTF punctual light spot light configuration with default cone angles.
*/
class gltfExt_KHR_lights_punctual_spot
{
public:
	/*!
		\brief Initializes a gltfExt_KHR_lights_punctual_spot object with default cone angles.

		The constructor initializes the inner cone angle to 0.0f and the outer cone angle to one-fourth of pi, which represents a standard spot light configuration. This setup provides a default
	   lighting behavior for punctual lights in GLTF files.

	*/
	gltfExt_KHR_lights_punctual_spot() :
		innerConeAngle( 0.0f ),
		outerConeAngle( idMath::ONEFOURTH_PI )
	{
	}
	float	  innerConeAngle;
	float	  outerConeAngle;
	idStr	  extensions;
	gltfExtra extras;
};

/*!
	\class gltfExt_KHR_lights_punctual
	\brief A struct for handling punctual light data in GLTF extensions.
*/
class gltfExt_KHR_lights_punctual
{
public:
	enum Type { Directional, Point, Spot, count };

	/*!
		\brief Initializes a new instance of the gltfExt_KHR_lights_punctual struct with default values for color, intensity, range, and light type.

		The constructor initializes the color to vec3_one, intensity to 1.0f, range to -1.0f, and intType to -1. These default values represent a basic punctual light configuration with no specific
	   lighting parameters set.

	*/
	gltfExt_KHR_lights_punctual() :
		color( vec3_one ),
		intensity( 1.0f ),
		range( -1.0f ),
		intType( -1 )
	{
	}
	idVec3							 color;
	float							 intensity;
	gltfExt_KHR_lights_punctual_spot spot;
	idStr							 type; // directional=0,point=1,spot=2
	float							 range;
	idStr							 name;
	idStr							 extensions;
	gltfExtra						 extras;

	int								 intType;

	/*!
		\brief Resolves a string representation of a light type into its corresponding enumerated value.

		This function takes a string identifier for a light type and maps it to a predefined enumeration value. It supports the light types 'directional', 'point', and 'spot'. If the input string does
	   not match any of these known types, it returns a default count value indicating an invalid or unrecognized type.

		\param type A string representing the type of light to resolve.
		\return The resolved Type enumeration value corresponding to the input string, or Type::count if the type is unrecognized.
	*/
	static Type						 resolveType( idStr type )
	{
		if( type == "directional" ) {
			return Type::Directional;
		} else if( type == "point" ) {
			return Type::Point;
		} else if( type == "spot" ) {
			return Type::Spot;
		}
		return Type::count;
	}
};

/*!
	\class gltfExt_KHR_texture_transform
	\brief Provides texture transformation parameters for GLTF materials.
*/
class gltfExt_KHR_texture_transform
{
public:
	/*!
		\brief Initializes a gltfExt_KHR_texture_transform object with default values for texture transformation parameters.

		The constructor initializes all member variables to their default states. The offset is set to zero, rotation to zero degrees, scale to one (no scaling), texCoord to -1 indicating no specific
	   texture coordinate set, index to zero, and resolved to false indicating that the transformation has not been processed yet.

	*/
	gltfExt_KHR_texture_transform() :
		offset( vec2_zero ),
		rotation( 0.0f ),
		scale( vec2_one ),
		texCoord( -1 ),
		index( 0 ),
		resolved( false )
	{
	}
	idVec2	  offset;
	float	  rotation;
	idVec2	  scale;
	int		  texCoord;
	idStr	  extensions;
	gltfExtra extras;

	// for shader
	uint	  index;
	bool	  resolved;
};

/*!
	\class gltfData
	\brief A container class for managing glTF asset data including nodes, meshes, materials, and animations.

	This class serves as a comprehensive data structure for storing and managing glTF asset information. It maintains collections of various glTF elements such as nodes, meshes, materials, animations,
   and scenes while providing methods for accessing, modifying, and organizing this data. The class supports both reading and building glTF data structures, with functionality for managing memory
   allocation, data views, and hierarchical node relationships. It provides accessors for all major glTF components and includes utility methods for finding specific elements by name or ID, traversing
   node hierarchies, and managing data views for efficient access to binary buffer contents. The class is designed to support the parsing and construction of glTF assets, with methods for handling
   transformations, animations, and skinning data.

*/
class gltfData
{
public:
	gltfData() :
		fileName( "" ),
		fileNameHash( 0 ),
		json( nullptr ),
		data( nullptr ),
		totalChunks( -1 ) {};

	/*!
		\brief Destructor for gltfData that releases all allocated memory and clears the data

		The destructor cleans up all dynamically allocated memory for the gltfData object. It first frees all memory chunks stored in the data array, then frees the data array itself. After that, it
	   frees the json buffer if it exists. Finally, it sets all pointers to nullptr and clears the file name

	*/
	~gltfData();

	/*!
		\brief Adds data to the glTF structure and returns a pointer to the allocated memory.

		This function allocates memory for data to be stored within the glTF structure. If this is the first chunk of data, it allocates memory for the JSON section. Otherwise, it allocates memory for
	   a new data chunk. The function can optionally return the buffer ID of the newly created chunk through the bufferID parameter.

		\param size The size of the data to be allocated in bytes
		\param bufferID Optional pointer to store the buffer ID of the newly allocated chunk
		\return A pointer to the allocated memory block for the data.
	*/
	byte* AddData( int size, int* bufferID = nullptr );

	/*!
		\brief Returns the JSON data buffer and updates the size parameter with the length of the data.

		This function provides access to the JSON data that was parsed from a glTF file. The size parameter is updated to reflect the actual length of the JSON data in bytes. The returned pointer
	   points to the internal buffer containing the JSON data.

		\param size Reference to an integer that will be updated with the size of the JSON data
		\return Pointer to the byte buffer containing the JSON data
	*/
	byte* GetJsonData( int& size )
	{
		size = jsonDataLength;
		return json;
	}

	/*!
		\brief Returns a pointer to the data at the specified index in the gltfData array.

		This function accesses the internal data array at the given index and returns a pointer to the data element.

		\param index The index of the data element to retrieve
		\return A pointer to the data element at the specified index
	*/
	byte* GetData( int index ) { return data[index]; }

	/*!
		\brief Sets the file name and its hash for the glTF data.

		This function assigns the provided file name and its corresponding hash value to the internal members of the gltfData object. The file name is stored as an idStr object, and the hash is stored
	   as an integer. This is typically used to associate a file path with its hash for efficient lookup or identification purposes.

		\param file The file name to be set for the glTF data
		\param hash The hash value corresponding to the file name
	*/
	void  FileName( const idStr& file, int hash )
	{
		fileName	 = file;
		fileNameHash = hash;
	}

	/*!
		\brief Returns the hash value of the file name.

		This function provides access to the precomputed hash of the file name stored in the gltfData class. The hash is typically used for efficient lookup or identification of the file within a
	   system.

		\return An integer representing the hash of the file name.
	*/
	int						 FileNameHash() { return fileNameHash; }

	/*!
		\brief Returns a reference to the file name string associated with the gltfData object.
		\return A reference to the fileName member variable of the gltfData object
	*/
	idStr&					 FileName() { return fileName; }

	static idHashIndex		 fileDataHash;
	static idList<gltfData*> dataList;

	/*!
		\brief Returns the gltfData instance for the specified file name, creating it if requested.

		This function retrieves or creates a gltfData instance associated with the given file name. It uses a hash table for efficient lookup and maintains a list of all data instances. If the create
	   parameter is true and no existing instance is found, a new instance is created and added to the list. The function ensures that the internal data structures are initialized only once.

		\param fileName The name of the file to retrieve or create data for
		\param create Flag indicating whether to create a new instance if one doesn't exist
		\return The gltfData instance for the specified file name, or nullptr if not found and create is false
	*/
	static gltfData*		 Data( idStr& fileName, bool create = false )
	{
		static bool intialized = false;
		if( !intialized ) {
			dataList.SetGranularity( 1 );
			intialized = true;
		}
		int key	  = fileDataHash.GenerateKey( fileName );
		int index = fileDataHash.GetFirst( key );

		if( create && index == -1 ) {
			index = dataList.Num();
			dataList.AssureSizeAlloc( index + 1, idListNewElement<gltfData> );
			dataList[index]->FileName( fileName, key );
			fileDataHash.Add( key, index );
		}

		if( !create && index < 0 ) { return nullptr; }

		return dataList[index];
	}

	/*!
		\brief Returns a constant reference to the static list of gltfData pointers.
		\return A constant reference to the static idList containing pointers to gltfData objects.
	*/
	static const idList<gltfData*>& DataList() { return dataList; }

	/*!
		\brief Clears the GLTF data associated with the specified file name from the internal data structures.

		This function removes the GLTF data that was previously loaded for the given file name. It first generates a hash key for the file name and then looks up the corresponding index in the file
	   data hash table. If the data is found, it removes the data from the data list and updates the hash table. If no data is found for the file name, it issues a warning message indicating that no
	   data was loaded for the specified file.

		\param fileName The name of the file for which to clear the GLTF data
	*/
	static void						ClearData( idStr& fileName );

	/*!
		\brief Returns the GLTF node that controls the specified camera

		This function retrieves the GLTF node that is associated with the given camera. It searches through the list of cameras to find the camera index, then iterates through the nodes to find a node
	   that references the camera by its index. If a matching node is found, it is returned. Otherwise, the function returns nullptr. The function does not account for overrides when determining the
	   node association.

		\param camera Pointer to the camera for which to find the controlling node
		\return Pointer to the GLTF node that controls the specified camera, or nullptr if no such node exists
		\throws assertion failure if the camera parameter is null
	*/
	gltfNode*						GetCameraNodes( gltfCamera* camera )
	{
		gltfCameraNodePtrs result;

		assert( camera );
		int camId = -1;
		for( auto* cam : cameras ) {
			camId++;
			if( cam == camera ) { break; }
		}

		for( int i = 0; i < nodes.Num(); i++ ) {
			if( nodes[i]->camera != -1 && nodes[i]->camera == camId ) { return nodes[i]; }
		}

		return nullptr;
	}

	/*!
		\brief Retrieves a glTF node that references the specified mesh within the given scene

		This function searches through the nodes of a glTF scene to find the first node that references the specified mesh. It can optionally return the index of the found node. The function performs
	   assertions to ensure the scene and mesh parameters are valid. It iterates through all meshes in the scene and checks each node to see if it references the target mesh. If a matching node is
	   found, it returns the node pointer; otherwise, it returns nullptr.

		\param scene Pointer to the glTF scene to search within
		\param mesh Pointer to the glTF mesh to find a referencing node for
		\param id Optional pointer to store the index of the found node, or nullptr to ignore
		\return Pointer to the glTF node that references the specified mesh, or nullptr if no such node is found
		\throws assertion failure if scene or mesh parameters are null
	*/
	gltfNode* GetNode( gltfScene* scene, gltfMesh* mesh, int* id = nullptr )
	{
		assert( scene );
		assert( mesh );

		auto& nodeList = scene->nodes;

		for( gltfMesh* meshIt : meshes ) {
			if( meshIt != mesh ) { continue; }

			int nodeCnt = 0;
			for( auto& nodeId : nodeList ) {
				if( nodes[nodeId]->mesh != -1 && meshes[nodes[nodeId]->mesh] == meshIt ) {
					if( id != nullptr ) { *id = nodeCnt; }

					return nodes[nodeId];
				}
				nodeCnt++;
			}
		}
		return nullptr;
	}

	/*!
		\brief Retrieves a node from a specified scene by its ID, optionally returning the node's name

		This function searches for a node within a given scene by its ID. It first validates the scene ID and ensures the scene exists. If the node is found, it returns a pointer to the node. If a
	   name pointer is provided, it populates the name with the node's name. The function handles invalid scene IDs and ensures node ID validity through assertions.

		\param sceneName Name of the scene to search within
		\param id ID of the node to retrieve
		\param name Optional pointer to store the node's name
		\return Pointer to the retrieved node if found, or nullptr if the scene or node is not found
	*/
	gltfNode* GetNode( const idStr& sceneName, int id, idStr* name = nullptr )
	{
		int sceneId = GetSceneId( sceneName );
		if( sceneId < 0 || sceneId > scenes.Num() ) { return nullptr; }

		gltfScene* scene = scenes[sceneId];

		assert( scene );
		assert( id >= 0 );

		auto& nodeList = scene->nodes;
		for( auto& nodeId : nodeList ) {
			if( nodeId == id ) {
				if( name != nullptr ) { *name = nodes[nodeId]->name; }

				return nodes[nodeId];
			}
		}

		return nullptr;
	}

	/*!
		\brief Retrieves a node from the glTF data by name, with optional ID retrieval and case sensitivity

		This function searches for a node in the glTF data structure by its name. It supports case-sensitive and case-insensitive matching depending on the caseSensitive parameter. If the node is
	   found, the function can optionally return the node's ID through the id parameter. The function returns nullptr if no matching node is found.

		\param name Name of the node to search for
		\param id Pointer to an integer where the node ID will be stored, or nullptr if not needed
		\param caseSensitive Flag indicating whether the name comparison should be case-sensitive
		\return Pointer to the found gltfNode, or nullptr if no node with the specified name exists
		\throws Assertion failure if the name string is empty
	*/
	gltfNode* GetNode( const idStr& name, int* id = nullptr, bool caseSensitive = false )
	{
		assert( name[0] );

		auto& nodeList = NodeList();
		for( auto* node : nodes ) {
			int nodeId = GetNodeIndex( node );
			if( caseSensitive ? nodes[nodeId]->name.Cmp( name ) : nodes[nodeId]->name.Icmp( name ) == 0 ) {
				if( id != nullptr ) { *id = nodeId; }

				return nodes[nodeId];
			}
		}

		return nullptr;
	}

	/*!
		\brief Retrieves a mesh node from the GLTF data by its name, optionally returning the node's index.

		This function searches through the list of nodes in the GLTF data to find a node that represents a mesh with the specified name. It supports case-sensitive and case-insensitive matching. If a
	   matching node is found, the function returns a pointer to that node. If the id parameter is provided, it will be filled with the index of the found node. If no matching node is found, the
	   function returns nullptr.

		\param meshName The name of the mesh to search for
		\param id Optional pointer to an integer that will be set to the index of the found node, or nullptr if not needed
		\param caseSensitive Flag to determine if the search should be case-sensitive
		\return A pointer to the gltfNode that matches the specified mesh name, or nullptr if no match is found.
	*/
	gltfNode* GetMeshNode( const idStr& meshName, int* id = nullptr, bool caseSensitive = false )
	{
		int nodeCnt = 0;
		for( auto* node : nodes ) {
			if( node->mesh != -1 && ( caseSensitive ? node->name.Cmp( meshName ) : node->name.Icmp( meshName ) ) == 0 ) {
				if( id != nullptr ) { *id = nodeCnt; }

				return node;
			}
			nodeCnt++;
		}

		return nullptr;
	}

	/*!
		\brief Retrieves a node from a specified scene by name, with optional ID retrieval and case sensitivity

		This function searches for a node within a given scene by its name. It supports case-sensitive and case-insensitive matching. If the node is found, it can optionally return the node's ID
	   through the provided pointer. The function performs bounds checking on the scene ID and validates the input parameters. If the scene is invalid or the node is not found, the function returns
	   nullptr.

		\param sceneName Name of the scene to search within
		\param name Name of the node to find
		\param id Pointer to store the node ID if found, can be nullptr
		\param caseSensitive Flag to determine if the name comparison should be case sensitive
		\return Pointer to the found gltfNode, or nullptr if the node is not found or the scene is invalid
	*/
	gltfNode* GetNode( const idStr& sceneName, const idStr& name, int* id = nullptr, bool caseSensitive = false )
	{
		int sceneId = GetSceneId( sceneName );
		if( sceneId < 0 || sceneId > scenes.Num() ) { return nullptr; }

		gltfScene* scene = scenes[sceneId];

		assert( scene );
		assert( name[0] );

		auto& nodeList = scene->nodes;
		for( auto nodeId : nodeList ) {
			if( caseSensitive ? nodes[nodeId]->name.Cmp( name ) : nodes[nodeId]->name.Icmp( name ) == 0 ) {
				if( id != nullptr ) { *id = nodeId; }

				return nodes[nodeId];
			}
		}

		return nullptr;
	}

	/*!
		\brief Returns the index of a given gltfNode in the nodes collection, or -1 if not found

		This function searches through the nodes collection to find the specified gltfNode and returns its index. It iterates through all nodes in the collection and compares each node with the
	   provided node pointer. If a match is found, the function immediately returns the index. If no match is found after checking all nodes, it returns -1 to indicate that the node was not found in
	   the collection. The function is commonly used to map node pointers to indices for operations like bone remapping and node traversal in GLTF model processing.

		\param node Pointer to the gltfNode to search for in the nodes collection
		\return The index of the node in the nodes collection if found, or -1 if the node is not present
	*/
	int GetNodeIndex( gltfNode* node )
	{
		int index = -1;
		for( auto& it : nodes ) {
			index++;
			if( it == node ) { return index; }
		}
		return -1;
	}

	/*!
		\brief Checks whether a node in the GLTF data has an associated animation.

		This function iterates through all animations and their channels to determine if any channel targets the specified node ID. It returns true if an animation is found that affects the given
	   node, and false otherwise.

		\param nodeID The ID of the node to check for animation presence.
		\return True if the specified node has an animation associated with it, false otherwise.
	*/
	bool HasAnimation( int nodeID )
	{
		for( auto anim : animations ) {
			for( auto channel : anim->channels ) {
				if( channel->target.node == nodeID ) { return true; }
			}
		}
		return false;
	}

	/*!
		\brief Returns a pointer to the animation with the specified name from the glTF data

		This function searches through the list of animations stored in the glTF data structure to find and return a pointer to the animation that matches the given name. If no matching animation is
	   found, it returns nullptr

		\param animName The name of the animation to search for
		\return A pointer to the gltfAnimation object if found, or nullptr if no animation with the specified name exists
	*/
	gltfAnimation* GetAnimation( const idStr& animName )
	{
		for( auto* anim : animations ) {
			if( anim->name == animName ) { return anim; }
		}
		return nullptr;
	}

	/*!
		\brief Retrieves a GLTF animation by name that targets a specific node

		This function searches through the list of animations in the GLTF data structure to find an animation that matches the given name and has a channel targeting the specified node ID. It iterates
	   through all animations and their channels to verify if any channel targets the given node. If found, it returns a pointer to the animation; otherwise, it returns nullptr.

		\param animName Name of the animation to search for
		\param target ID of the target node that the animation should channel to
		\return Pointer to the matching gltfAnimation object if found, otherwise nullptr
	*/
	gltfAnimation* GetAnimation( const idStr& animName, int target )
	{
		for( auto* anim : animations ) {
			if( anim->name == animName ) {
				bool hasTarget = false;
				for( auto* channel : anim->channels ) {
					if( channel->target.node == target ) {
						hasTarget = true;
						break;
					}
				}
				if( hasTarget ) { return anim; }
			}
		}
		return nullptr;
	}

	/*!
		\brief Returns the index of the scene with the specified name, or -1 if not found.

		This function searches through the list of scenes stored in the gltfData object to find a scene with a matching name. If a matching scene is found, the function can optionally populate the
	   result parameter with a pointer to that scene. The function returns the index of the scene in the internal scenes array, or -1 if no matching scene is found.

		\param sceneName The name of the scene to search for
		\param result Optional pointer to store the found scene, may be null
		\return The index of the scene in the scenes array if found, otherwise -1
	*/
	int GetSceneId( const idStr& sceneName, gltfScene* result = nullptr ) const
	{
		for( int i = 0; i < scenes.Num(); i++ ) {
			if( scenes[i]->name == sceneName ) {
				if( result != nullptr ) { result = scenes[i]; }

				return i;
			}
		}
		return -1;
	}

	/*!
		\brief Collects all unique mesh IDs from a node and its child nodes in a glTF structure

		This function traverses the glTF node hierarchy starting from the provided node and collects all mesh IDs that are associated with nodes in the hierarchy. It uses a depth-first search approach
	   to visit all child nodes and adds unique mesh IDs to the provided list. The function checks if a node has a valid mesh (indicated by a mesh ID not equal to -1) and then adds the node's index to
	   the mesh IDs list. The traversal continues recursively for all child nodes.

		\param node The starting node in the glTF hierarchy to begin collecting mesh IDs from
		\param meshIds Reference to a list that will store the collected unique mesh IDs
	*/
	void GetAllMeshes( gltfNode* node, idList<int>& meshIds )
	{
		if( node->mesh != -1 ) { meshIds.AddUnique( GetNodeIndex( node ) ); }

		for( auto child : node->children ) {
			GetAllMeshes( nodes[child], meshIds );
		}
	}

	/*!
		\brief Retrieves the IDs of all meshes contained within the glTF data

		This function iterates through all nodes in the glTF data and collects the indices of nodes that are associated with meshes. A node is considered to contain a mesh if its mesh field is not set
	   to -1. The collected mesh IDs are added to the provided list, ensuring no duplicates are present

		\param meshIds A reference to a list that will store the unique indices of all meshes found in the glTF data
	*/
	void GetAllMeshes( idList<int>& meshIds )
	{
		for( int i = 0; i < nodes.Num(); i++ ) {
			auto* node = nodes[i];

			if( node->mesh != -1 ) { meshIds.AddUnique( i ); }
		}
	}

	/*!
		\brief Collects all mesh IDs from nodes that have both mesh and skin data

		This function traverses the node hierarchy starting from the provided node and collects unique mesh IDs for nodes that have both mesh and skin data. It checks if a node has a valid mesh index
	   and skin index, then adds the node's index to the provided list. The function recursively processes all child nodes to ensure all skinned meshes in the hierarchy are included.

		\param node The root node to start traversing the hierarchy from
		\param meshIds The list to store unique mesh IDs of nodes with both mesh and skin data
	*/
	void GetAllSkinnedMeshes( gltfNode* node, idList<int>& meshIds )
	{
		if( node->mesh != -1 && node->skin != -1 ) { meshIds.AddUnique( GetNodeIndex( node ) ); }

		for( auto child : node->children ) {
			GetAllSkinnedMeshes( nodes[child], meshIds );
		}
	}

	/*!
		\brief Retrieves all mesh IDs that are skinned by the specified skin from the glTF data.

		This function iterates through all nodes in the glTF data and checks if each node has a valid mesh and skin index. If the node's skin matches the provided skin parameter, the node's index is
	   added to the meshIds list. The function ensures that each mesh ID is only added once to the list.

		\param skin Pointer to the skin to filter meshes by
		\param meshIds Reference to a list that will contain the IDs of meshes skinned by the specified skin
	*/
	void GetAllSkinnedMeshes( gltfSkin* skin, idList<int>& meshIds )
	{
		for( int i = 0; i < nodes.Num(); i++ ) {
			auto* node = nodes[i];

			if( node->mesh != -1 && node->skin != -1 ) {
				gltfSkin* meshSkin = skins[node->skin];

				if( meshSkin == skin ) { meshIds.AddUnique( i ); }
			}
		}
	}

	/*!
		\brief Populates a list with the indices of all nodes that represent skinned meshes.

		This function iterates through all nodes in the glTF data structure and identifies those that have both a valid mesh index and a valid skin index. Such nodes are considered to represent
	   skinned meshes. The function adds the indices of these nodes to the provided list, ensuring no duplicates are added.

		\param meshIds A reference to a list that will be populated with the indices of nodes representing skinned meshes.
	*/
	void GetAllSkinnedMeshes( idList<int>& meshIds )
	{
		for( int i = 0; i < nodes.Num(); i++ ) {
			auto* node = nodes[i];

			if( node->mesh != -1 && node->skin != -1 ) { meshIds.AddUnique( i ); }
		}
	}

	/*!
		\brief Returns a skin from the glTF data by its name, or nullptr if not found

		This function searches through the list of skins contained in the glTF data structure and returns the first skin that matches the provided name. If no skin with the specified name is found, it
	   returns nullptr. The function performs a linear search through the skins collection, making it suitable for small to moderately sized collections. The search is case-sensitive and exact match.

		\param name name of the skin to search for
		\return A pointer to the gltfSkin object if found, otherwise nullptr
	*/
	gltfSkin* GetSkin( const idStr& name )
	{
		for( auto skin : skins ) {
			if( skin->name == name ) { return skin; }
		}

		return nullptr;
	}

	/*!
		\brief Returns a skin from the glTF data that contains the specified bone node ID

		This function searches through all skins in the glTF data to find one that contains the specified bone node ID in its joint list. It returns a pointer to the first matching skin, or nullptr if
	   no skin contains the bone node ID. The function is used to associate bone nodes with skin information during glTF model processing, particularly when determining the skeleton structure for
	   animation and rendering.

		\param boneNodeId The ID of the bone node to search for within the skins
		\return A pointer to the gltfSkin object that contains the specified bone node ID, or nullptr if no such skin exists
	*/
	gltfSkin* GetSkin( int boneNodeId )
	{
		for( auto skin : skins ) {
			if( skin->joints.Find( boneNodeId ) ) { return skin; }
		}

		return nullptr;
	}

	/*!
		\brief Returns the skin associated with the given animation by checking its animation targets

		The function searches for a skin that is associated with the given animation by first retrieving the animation targets and then checking each target node for an associated skin. If no skin is
	   found, the function returns nullptr. This approach allows the function to determine the skin that should be used for applying the animation to the model.

		\param anim The animation for which to find the associated skin
		\return The skin associated with the given animation, or nullptr if no skin is found
	*/
	gltfSkin* GetSkin( gltfAnimation* anim )
	{
		auto animTargets = GetAnimTargets( anim );

		if( !animTargets.Num() ) { return nullptr; }

		for( int nodeID : animTargets ) {
			gltfSkin* foundSkin = GetSkin( nodeID );
			if( foundSkin != nullptr ) { return foundSkin; }
		}

		return nullptr;
	}

	/*!
		\brief Returns a list of unique node indices that are targeted by the channels of the specified animation.

		The function iterates through all channels in the provided animation and collects the unique node indices that are targeted by these channels. This is useful for determining which bones or
	   nodes are affected by a specific animation, particularly when processing GLTF animation data for model rendering.

		\param anim Pointer to the GLTF animation object to process
		\return A list containing unique node indices that are targeted by the animation channels
	*/
	idList<int> GetAnimTargets( gltfAnimation* anim ) const
	{
		idList<int> result;

		for( auto channel : anim->channels ) {
			result.AddUnique( channel->target.node );
		}

		return result;
	}

	/*!
		\brief Returns a list of channel IDs from the specified animation that target the given node

		The function iterates through all channels in the provided animation and checks if the channel's target node matches the specified node. It collects the indices of matching channels and
	   returns them in a list. The function stops searching after finding the first matching channel, as indicated by the break statement in the loop. The nodes array is accessed using the target node
	   index from the channel to perform the comparison.

		\param anim Pointer to the animation containing the channels to be checked
		\param node Pointer to the node that channels should target
		\return List of integer indices representing the channels in the animation that target the specified node
	*/
	idList<int> GetChannelIds( gltfAnimation* anim, gltfNode* node ) const
	{
		idList<int> result;
		int			channelIdx = 0;

		for( auto channel : anim->channels ) {
			if( channel->target.node >= 0 && nodes[channel->target.node] == node ) {
				result.Append( channelIdx );
				break;
			}
			channelIdx++;
		}

		return result;
	}

	/*!
		\brief Retrieves the animation IDs associated with a given node and its children

		This function iterates through all animations and checks if any animation channel targets the specified node or its children. It collects unique animation indices in the result list. The
	   function also recursively processes the node's children to ensure all animations for the subtree are accounted for. The result list is modified in place to contain the animation IDs.

		\param node Pointer to the gltfNode for which to find animations
		\param result Reference to an idList where the found animation IDs will be added
		\return The number of unique animation IDs found and added to the result list
	*/
	int GetAnimationIds( gltfNode* node, idList<int>& result )
	{
		int animIdx = 0;
		for( auto anim : animations ) {
			for( auto channel : anim->channels ) {
				if( channel->target.node >= 0 && nodes[channel->target.node] == node ) {
					result.AddUnique( animIdx );
					break;
				}
			}
			animIdx++;
		}

		for( int nodeId : node->children ) {
			GetAnimationIds( nodes[nodeId], result );
		}

		return result.Num();
	}

	/*!
		\brief Returns the view matrix for a specified camera ID by traversing the node hierarchy and accumulating transformations

		This function retrieves the view matrix for a given camera ID by first locating the camera node within the scene hierarchy. It then traverses up the node tree from the camera node to the root,
	   building a list of nodes in the hierarchy. The function then iterates through this hierarchy in reverse order, resolving each node's transformation matrix and applying it to the result. The
	   final matrix represents the complete transformation from the camera's local space to world space

		\param camId ID of the camera for which to retrieve the view matrix
		\return The view matrix as an idMat4 object representing the transformation from camera local space to world space
	*/
	idMat4 GetViewMatrix( int camId ) const
	{
		// if (cameraManager->HasOverideID(camId) )
		//{
		//	auto overrideCam = cameraManager->GetOverride( camId );
		//	camId = overrideCam.newCameraID;
		// }

		idMat4			  result = mat4_identity;

		idList<gltfNode*> hierachy( 2 );
		gltfNode*		  parent = nullptr;

		for( int i = 0; i < nodes.Num(); i++ ) {
			if( nodes[i]->camera != -1 && nodes[i]->camera == camId ) {
				parent = nodes[i];
				while( parent ) {
					hierachy.Append( parent );
					parent = parent->parent;
				}
				break;
			}
		}

		for( int i = hierachy.Num() - 1; i >= 0; i-- ) {
			ResolveNodeMatrix( hierachy[i] );
			result *= hierachy[i]->matrix;
		}

		return result;
	}

	/*!
		\brief Returns the transformation matrix for a light node in the glTF data structure by traversing the node hierarchy.

		This function retrieves the transformation matrix for a specified light by first locating the node associated with that light ID. It then traverses up the node hierarchy from the light node to
	   the root, collecting all nodes in the path. Finally, it multiplies the matrices of these nodes in reverse order to compute the final transformation matrix for the light. The function assumes
	   that all nodes are not dirty and does not perform any dirty flag checks.

		\param lightId The ID of the light for which to retrieve the transformation matrix
		\return The transformation matrix for the specified light node, computed by multiplying the matrices of all nodes in the hierarchy from the light node up to the root.
	*/
	idMat4 GetLightMatrix( int lightId ) const
	{
		idMat4			  result = mat4_identity;

		idList<gltfNode*> hierachy;
		gltfNode*		  parent = nullptr;
		hierachy.SetGranularity( 2 );

		for( int i = 0; i < nodes.Num(); i++ ) {
			if( nodes[i]->extensions.KHR_lights_punctual && nodes[i]->extensions.KHR_lights_punctual->light == lightId ) {
				parent = nodes[i];
				while( parent ) {
					hierachy.Append( parent );
					parent = parent->parent;
				}
				break;
			}
		}

		for( int i = hierachy.Num() - 1; i >= 0; i-- ) {
			result *= hierachy[i]->matrix;
		}

		return result;
	}

	/*!
		\brief Resolves the full transformation matrix for a GLTF node, including its hierarchy up to the specified root node

		This function computes the final transformation matrix for a given GLTF node by applying scale, rotation, and translation transformations. It handles the hierarchy by traversing up to the
	   specified root node and accumulating matrices in reverse order. The function also updates the node's dirty flag after computation. The matrix computation follows GLTF conventions where the
	   transformation is applied in the order of scale, rotation, and translation, assuming column-major matrix layout.

		\param node The GLTF node for which to resolve the matrix
		\param mat Pointer to the output matrix, or NULL if only updating the node's dirty flag
		\param root Root node of the hierarchy to traverse upwards, or NULL to traverse to the top of the hierarchy
	*/
	static void ResolveNodeMatrix( gltfNode* node, idMat4* mat = nullptr, gltfNode* root = nullptr )
	{
		if( node->dirty ) {
			idMat4 scaleMat = idMat4( node->scale.x, 0, 0, 0, 0, node->scale.y, 0, 0, 0, 0, node->scale.z, 0, 0, 0, 0, 1 );

			node->matrix = idMat4( mat3_identity, node->translation ) * node->rotation.ToMat4().Transpose() * scaleMat;

			node->dirty = false;
		}

		// resolve full hierarchy
		if( mat != nullptr ) {
			// collect hierarchy upwards
			idList<gltfNode*> hierachy( 2 );
			gltfNode*		  parent = node;
			while( parent ) {
				ResolveNodeMatrix( parent );
				hierachy.Append( parent );
				if( parent == root ) { break; }
				parent = parent->parent;
			}

			// build world transform from up to down
			for( int i = hierachy.Num() - 1; i >= 0; i-- ) {
				*mat *= hierachy[i]->matrix;
			}
		}
	}

	/*!
		\brief Returns a cached view of matrix data from a glTF accessor, populating it if necessary

		This function retrieves a cached view of matrix data associated with a glTF accessor. If the view has not been previously populated, it reads the data from the underlying buffer,
	   parses it as idMat4 matrices, and caches the result for subsequent calls. The function ensures that the matrix data is properly aligned and formatted according to the accessor's
	   specifications.

		\param accessor Pointer to the glTF accessor containing the matrix data and buffer information
		\return Reference to an idList containing the matrix data from the accessor
	*/
	template<class T>
	idList<T*>&		GetAccessorView( gltfAccessor* accessor );
	idList<float>&	GetAccessorView( gltfAccessor* accessor );
	idList<idMat4>& GetAccessorViewMat( gltfAccessor* accessor );

	int&			DefaultScene() { return scene; }

	/*!
		\brief Returns a pointer to the last buffer in the buffers list, allocating a new buffer if necessary.

		This function ensures that the buffers list has at least one element by allocating a new buffer if needed. It then returns a pointer to the last buffer in the list. The function is typically
	   used when building GLTF data structures and requires that the buffer list is properly initialized.

		\return A pointer to the last buffer in the buffers list, which may be a newly allocated buffer.
	*/
	gltfBuffer*		Buffer()
	{
		buffers.AssureSizeAlloc( buffers.Num() + 1, idListNewElement<gltfBuffer> );
		return buffers[buffers.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of buffers in the glTF data.

		This function provides access to the internal list of buffers that are part of the glTF data structure. The returned reference allows read-only access to the buffer objects without copying the
	   entire list. The buffers contain the actual binary data used by the glTF asset.

		\return A constant reference to the idList containing pointers to gltfBuffer objects
	*/
	const inline idList<gltfBuffer*>& BufferList() { return buffers; }

	/*!
		\brief Returns a pointer to the next available gltfSampler in the samplers list.

		This function ensures that the samplers list has enough capacity to hold a new element, then returns a pointer to the newly added gltfSampler. The function is typically used when constructing
	   GLTF data structures that require sampler definitions. The returned pointer can be used to configure the properties of the sampler.

		\return A pointer to a gltfSampler object which can be used to configure sampler properties
	*/
	gltfSampler*					  Sampler()
	{
		samplers.AssureSizeAlloc( samplers.Num() + 1, idListNewElement<gltfSampler> );
		return samplers[samplers.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of glTF samplers contained in the gltfData object.

		The function provides access to the internal list of glTF sampler objects stored within the gltfData instance. This allows external code to iterate over or query the samplers without modifying
	   the underlying data structure. The returned reference is const, ensuring that the sampler list cannot be modified through this accessor.

		\return A constant reference to an idList containing pointers to gltfSampler objects
	*/
	const inline idList<gltfSampler*>& SamplerList() { return samplers; }

	/*!
		\brief Returns a pointer to a newly added buffer view object

		This function allocates and returns a new buffer view object within the glTF data structure. It ensures that the bufferViews list has sufficient capacity to accommodate the new element, then
	   returns a pointer to the newly created buffer view. The function is typically used when constructing glTF data models to add new buffer view entries.

		\return A pointer to the newly created gltfBufferView object
	*/
	gltfBufferView*					   BufferView()
	{
		bufferViews.AssureSizeAlloc( bufferViews.Num() + 1, idListNewElement<gltfBufferView> );
		return bufferViews[bufferViews.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of buffer views in the glTF data.

		This function provides access to the buffer views stored within the glTF data structure. The returned reference allows read-only access to the collection of buffer views without creating a
	   copy. Buffer views represent chunks of binary data within the glTF file, typically referencing data from buffer objects.

		\return A constant reference to an idList containing pointers to gltfBufferView objects
	*/
	const inline idList<gltfBufferView*>& BufferViewList() { return bufferViews; }

	/*!
		\brief Returns a pointer to the image data for the glTF data

		This function ensures that the images list has enough space allocated to accommodate a new image element. It then returns a pointer to the last image in the list, which is typically the newly
	   allocated image. The function is used to prepare for adding new image data to the glTF structure.

		\return Pointer to a gltfImage object that represents the image data in the glTF structure
	*/
	gltfImage*							  Image()
	{
		images.AssureSizeAlloc( images.Num() + 1, idListNewElement<gltfImage> );
		return images[images.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of images in the glTF data.
		\return A constant reference to the idList containing pointers to gltfImage objects.
	*/
	const inline idList<gltfImage*>& ImageList() { return images; }

	/*!
		\brief Returns a pointer to a new texture element in the gltfData texture list.

		The function ensures that the textures list has enough capacity to hold a new texture element. It then allocates a new texture element and returns a pointer to it. The texture element is added
	   to the end of the textures list.

		\return A pointer to the newly allocated gltfTexture element in the textures list.
	*/
	gltfTexture*					 Texture()
	{
		textures.AssureSizeAlloc( textures.Num() + 1, idListNewElement<gltfTexture> );
		return textures[textures.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of textures stored in the gltfData object.

		The function provides access to the internal list of textures that have been parsed from a glTF file. The returned reference allows direct access to the texture objects without copying them,
	   enabling efficient traversal and usage of the texture data. The list is populated during the parsing process of the glTF file and remains valid for the lifetime of the gltfData object.

		\return A constant reference to an idList containing pointers to gltfTexture objects
	*/
	const inline idList<gltfTexture*>& TextureList() { return textures; }

	/*!
		\brief Returns a pointer to a newly created gltfAccessor object from the gltfData instance.

		The function ensures that the accessors list has enough capacity to hold a new element and then returns a pointer to the last element in the list, which is newly created and initialized.

		\return A pointer to a newly created gltfAccessor object.
	*/
	gltfAccessor*					   Accessor()
	{
		accessors.AssureSizeAlloc( accessors.Num() + 1, idListNewElement<gltfAccessor> );
		return accessors[accessors.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of accessors in the glTF data.

		This function provides access to the internal list of glTF accessors stored within the gltfData object. The accessors represent the data access patterns for various elements in the glTF asset,
	   such as vertex positions, normals, and texture coordinates. The returned reference allows for efficient traversal and interrogation of the accessor data without copying the underlying list.

		\return A constant reference to an idList containing pointers to gltfAccessor objects.
	*/
	const inline idList<gltfAccessor*>& AccessorList() { return accessors; }

	/*!
		\brief Returns a pointer to the last extensions used entry in the glTF data.

		This function ensures that the extensionsUsed list has enough allocated space to accommodate the current number of accessors plus one additional element. It then returns a pointer to the last
	   element in the extensionsUsed list.

		\return A pointer to the last gltfExtensionsUsed entry in the extensionsUsed list.
	*/
	gltfExtensionsUsed*					ExtensionsUsed()
	{
		extensionsUsed.AssureSizeAlloc( accessors.Num() + 1, idListNewElement<gltfExtensionsUsed> );
		return extensionsUsed[extensionsUsed.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of GLTF extensions used.
		\return A constant reference to the idList containing pointers to gltfExtensionsUsed objects
	*/
	const inline idList<gltfExtensionsUsed*>& ExtensionsUsedList() { return extensionsUsed; }

	/*!
		\brief Returns a pointer to the last mesh in the meshes list, ensuring the list has enough capacity.

		This function ensures that the meshes list has enough capacity to hold a new element, then returns a pointer to the last mesh in the list. It is used to add new meshes to the gltfData object
	   by accessing the next available slot in the meshes array.

		\return A pointer to the last mesh in the meshes list, which can be used to access or modify the mesh data.
	*/
	gltfMesh*								  Mesh()
	{
		meshes.AssureSizeAlloc( meshes.Num() + 1, idListNewElement<gltfMesh> );
		return meshes[meshes.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of meshes stored in the gltfData object

		This function provides access to the mesh data stored within a gltfData object. It returns a constant reference to an idList containing pointers to gltfMesh objects. The returned reference
	   allows for efficient access to the mesh collection without copying the data. This is commonly used in the gltf model processing pipeline to iterate over meshes when converting gltf data to
	   renderable model surfaces

		\return A constant reference to an idList containing pointers to gltfMesh objects
	*/
	const inline idList<gltfMesh*>& MeshList() { return meshes; }

	/*!
		\brief Returns a pointer to the last scene in the scenes list, creating it if necessary.

		This function ensures that the scenes list has enough capacity to hold the new scene, then returns a pointer to the most recently added scene. It uses AssureSizeAlloc to manage memory
	   allocation and idListNewElement to create new elements when needed.

		\return A pointer to the last scene in the scenes list, which may be newly created
	*/
	gltfScene*						Scene()
	{
		scenes.AssureSizeAlloc( scenes.Num() + 1, idListNewElement<gltfScene> );
		return scenes[scenes.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of scenes in the glTF data

		This function provides access to the collection of scenes stored within the glTF data structure. The returned reference allows for read-only access to the scenes without copying the data. The
	   scenes are organized as a list of pointers to gltfScene objects, which can be iterated or indexed to access individual scenes.

		\return A constant reference to an idList containing pointers to gltfScene objects
	*/
	const inline idList<gltfScene*>& SceneList() { return scenes; }

	/*!
		\brief Returns a pointer to a newly allocated gltfNode element from the nodes list

		This function allocates a new gltfNode element in the internal nodes list, ensuring the list has enough capacity to hold the new element. It then returns a pointer to the newly added node,
	   which is the last element in the list. The function is typically used when constructing GLTF node hierarchies, such as when adding new nodes to represent transformations or object relationships
	   in a 3D model

		\return A pointer to a newly created gltfNode element that has been added to the internal nodes list
	*/
	gltfNode*						 Node()
	{
		nodes.AssureSizeAlloc( nodes.Num() + 1, idListNewElement<gltfNode> );
		return nodes[nodes.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of GLTF nodes stored in the gltfData object

		This function provides access to the internal list of GLTF nodes that have been parsed from a GLTF file. The returned reference allows direct access to the node data without copying. The nodes
	   contain hierarchical transformation information and mesh references that are used during the rendering process. This is a getter function for the nodes member variable of the gltfData class.

		\return A constant reference to an idList containing pointers to gltfNode objects
	*/
	const inline idList<gltfNode*>& NodeList() { return nodes; }

	/*!
		\brief Returns a pointer to the camera object at the last index of the cameras list.

		This function ensures that the cameras list has enough space allocated to hold a new camera object. It then returns a pointer to the camera at the last index of the list. The function is
	   typically used to access or modify the most recently added camera in the scene.

		\return A pointer to the camera object at the last index of the cameras list.
	*/
	gltfCamera*						Camera()
	{
		cameras.AssureSizeAlloc( cameras.Num() + 1, idListNewElement<gltfCamera> );
		return cameras[cameras.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of cameras stored in the gltfData object.

		The function provides access to the internal list of camera objects contained within the gltfData instance. This allows external code to iterate over or query the cameras without modifying the
	   container itself. The returned reference remains valid for the lifetime of the gltfData object.

		\return A constant reference to an idList containing pointers to gltfCamera objects
	*/
	const inline idList<gltfCamera*>& CameraList() { return cameras; }

	/*!
		\brief Returns a pointer to a newly created material in the glTF data

		This function ensures that a new material is available in the materials list of the glTF data structure. It allocates memory for the new material and returns a pointer to it. The material is
	   added to the end of the materials list.

		\return A pointer to the newly created gltfMaterial object
	*/
	gltfMaterial*					  Material()
	{
		materials.AssureSizeAlloc( materials.Num() + 1, idListNewElement<gltfMaterial> );
		return materials[materials.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of materials stored in the glTF data.

		This function provides access to the materials contained within the glTF data structure. The returned reference allows for iteration over the materials without creating a copy. The materials
	   are typically used to determine the shader to apply to different parts of a mesh during rendering.

		\return A constant reference to an idList containing pointers to gltfMaterial objects
	*/
	const inline idList<gltfMaterial*>& MaterialList() { return materials; }

	/*!
		\brief Returns a pointer to the extensions data structure for the glTF data.

		The function ensures that the extensions list has enough capacity to hold at least one more element. It then returns a pointer to the last element in the extensions list. The extensions list
	   is managed as a dynamic array using an idList structure.

		\return A pointer to the gltfExtensions object in the extensions list.
	*/
	gltfExtensions*						Extensions()
	{
		extensions.AssureSizeAlloc( extensions.Num() + 1, idListNewElement<gltfExtensions> );
		return extensions[extensions.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of glTF extensions.

		This function provides access to the internal list of glTF extensions stored in the gltfData object. The returned reference allows read-only access to the extensions without copying the data.

		\return A constant reference to the idList containing pointers to gltfExtensions objects
	*/
	const inline idList<gltfExtensions*>& ExtensionsList() { return extensions; }

	/*!
		\brief Returns a pointer to the last animation in the animations list, ensuring the list has space allocated for it.

		This function ensures that the animations list has sufficient space allocated to accommodate a new element. It then returns a pointer to the newly added animation element at the end of the
	   list. The function is typically used when constructing a new animation object in the glTF data structure.

		\return A pointer to the last animation element in the animations list
	*/
	gltfAnimation*						  Animation()
	{
		animations.AssureSizeAlloc( animations.Num() + 1, idListNewElement<gltfAnimation> );
		return animations[animations.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of animations stored in the gltfData object.

		This function provides access to the animations contained within the gltfData object. The returned reference allows for reading the animation data without modification. The animations are
	   stored in an idList container, which holds pointers to gltfAnimation objects.

		\return A constant reference to an idList containing pointers to gltfAnimation objects
	*/
	const inline idList<gltfAnimation*>& AnimationList() { return animations; }

	/*!
		\brief Returns a pointer to the skin data for the glTF model.

		This function ensures that there is space allocated for a new skin in the skins list and returns a pointer to the last skin in the list. The skin data is used to define the joints and matrices
	   for skeleton-based animations in the glTF model.

		\return Pointer to the skin data for the glTF model.
	*/
	gltfSkin*							 Skin()
	{
		skins.AssureSizeAlloc( skins.Num() + 1, idListNewElement<gltfSkin> );
		return skins[skins.Num() - 1];
	}

	/*!
		\brief Returns a constant reference to the list of skins in the glTF data.

		This function provides access to the collection of skin definitions contained within the glTF data structure. Skins are used to define the hierarchical bone structure for skinning animations
	   in 3D models. The returned reference allows read-only access to the list of skin objects without creating a copy.

		\return A constant reference to an idList containing pointers to gltfSkin objects
	*/
	const inline idList<gltfSkin*>& SkinList() { return skins; }

	/*
	GLTFCACHEITEM( Buffer, buffers )
	GLTFCACHEITEM( Sampler, samplers )
	GLTFCACHEITEM( BufferView, bufferViews )
	GLTFCACHEITEM( Image, images )
	GLTFCACHEITEM( Texture, textures )
	GLTFCACHEITEM( Accessor, accessors )
	GLTFCACHEITEM( ExtensionsUsed, extensionsUsed )
	GLTFCACHEITEM( Mesh, meshes )
	GLTFCACHEITEM( Scene, scenes )
	GLTFCACHEITEM( Node, nodes )
	GLTFCACHEITEM( Camera, cameras )
	GLTFCACHEITEM( Material, materials )
	GLTFCACHEITEM( Extensions, extensions )
	GLTFCACHEITEM( Animation, animations )
	GLTFCACHEITEM( Skin, skins )
	*/

	// gltfCameraManager * cameraManager;
private:
	idStr						fileName;
	int							fileNameHash;

	byte*						json;
	byte**						data;
	int							jsonDataLength;
	int							totalChunks;

	idList<gltfBuffer*>			buffers;
	idList<gltfImage*>			images;
	idList<gltfData*>			assetData;
	idList<gltfSampler*>		samplers;
	idList<gltfBufferView*>		bufferViews;
	idList<gltfTexture*>		textures;
	idList<gltfAccessor*>		accessors;
	idList<gltfExtensionsUsed*> extensionsUsed;
	idList<gltfMesh*>			meshes;
	int							scene;
	idList<gltfScene*>			scenes;
	idList<gltfNode*>			nodes;
	idList<gltfCamera*>			cameras;
	idList<gltfMaterial*>		materials;
	idList<gltfExtensions*>		extensions;
	idList<gltfAnimation*>		animations;
	idList<gltfSkin*>			skins;
};

#undef GLTFCACHEITEM
