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
	\brief A class for handling extra data in glTF format.
*/
class gltfExtra
{
public:
	//! Constructs a new gltfExtra object.
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
	//! Default constructor for gltfExtensions.
	gltfExtensions() { }
	idList<gltfExt_KHR_lights_punctual*> KHR_lights_punctual;
};

/*!
	\class gltfNode_KHR_lights_punctual
	\brief This class represents a node with punctual light information in a glTF asset.
*/
class gltfNode_KHR_lights_punctual
{
public:
	int light;
};

/*!
	\class gltfNode_Extensions
	\brief A class for managing extensions in GLTF node data.
*/
class gltfNode_Extensions
{
public:
	//! Initializes a gltfNode_Extensions object with default values.
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
	//! Constructs a gltfMaterial_Extensions object with default values.
	gltfMaterial_Extensions() :
		KHR_materials_pbrSpecularGlossiness( nullptr )
	{
	}
	gltfExt_KHR_materials_pbrSpecularGlossiness* KHR_materials_pbrSpecularGlossiness;
};

/*!
	\class gltfNode
	\brief A class representing a node in a glTF file structure.
*/
class gltfNode
{
public:
	//! Initializes a new gltfNode object with default values.
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

	//! Compares two gltfNode objects for equality based only on their name member.
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
	\brief A class for managing and organizing 3D scene data imported from glTF files.
*/
class gltfScene
{
public:
	//! Constructs a new gltfScene object with default initialization.
	gltfScene() { }
	idList<int> nodes;
	idStr		name;
	idStr		extensions;
	gltfExtra	extras;
};

/*!
	\class gltfMesh_Primitive_Attribute
	\brief A class representing a mesh primitive attribute in a glTF file.
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

	//! Constructs a gltfMesh_Primitive_Attribute object with default values.
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
	\brief A class representing a primitive element within a glTF mesh.
*/
class gltfMesh_Primitive
{
public:
	//! Initializes a new instance of the gltfMesh_Primitive class with default values for indices, material, and mode.
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
	\brief A class representing a mesh data structure for GLTF format.
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
	\brief A structure for representing orthographic camera parameters in GLTF format.
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
	\brief Represents a perspective camera configuration for GLTF model loading.
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
	\brief A class representing a camera within a glTF scene.
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
	\brief Represents the target of an animation channel in a glTF file.
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

	//! Resolves a string type to its corresponding gltfTRS enum value.
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
	\brief Represents a channel in a glTF animation, defining how a specific property of an animation target is animated.
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
	\brief Provides functionality for handling animation samplers in GLTF files.
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

	//! Resolves a string type to its corresponding gltfInterpType enumeration value.
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
	\brief A class representing a glTF animation with limited support for animation data extraction.

	This class provides a representation of glTF animations and supports basic frame counting functionality. It is designed to handle animation data from glTF files, though it does not support
   extracting bounds, origin, or rotation information from the animation. The class maintains reference counts for memory management purposes, with methods to increase and decrease these counts. The
   animation data is expected to be used primarily for frame-based operations, with other animation properties intentionally left unsupported.

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

	//! Returns false indicating that getting bounds is not supported.
	bool						   GetBounds( idBounds& bnds, int time, int cyclecount ) const { return false; }

	//! Returns false indicating that getting the origin rotation is not supported.
	bool						   GetOriginRotation( idQuat& rotation, int time, int cyclecount ) const { return false; }

	//! Returns false, indicating that getting the origin for a glTF animation is not supported or implemented.
	bool						   GetOrigin( idVec3& offset, int time, int cyclecount ) const { return false; }

	//! Returns the total number of frames in the animation.
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
	\brief A class representing the indices component of sparse accessor data in GLTF format.
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
	\brief A class representing sparse accessors for GLTF data.
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
	\brief A class for handling GLTF accessor data structures.
*/
class gltfAccessor
{
public:
	//! Initializes a gltfAccessor object with default values.
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
	\brief A class representing a buffer view in a glTF file.
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
	\brief A class representing a buffer for storing and managing GLTF data.
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
	\brief A class representing a texture sampler configuration for glTF assets.
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
	\brief A class for handling glTF image data.
*/
class gltfImage
{
public:
	//! Constructs a gltfImage object with default values.
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
	\brief A class for handling skinning data from GLTF files.
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
	\brief A class representing texture information extensions for GLTF assets.
*/
class gltfTexture_Info_Extensions
{
public:
	//! Default constructor for gltfTexture_Info_Extensions that initializes the KHR_texture_transform member to nullptr.
	gltfTexture_Info_Extensions() :
		KHR_texture_transform( nullptr )
	{
	}
	gltfExt_KHR_texture_transform* KHR_texture_transform;
};

/*!
	\class gltfOcclusionTexture_Info
	\brief Manages information about occlusion textures used in GLTF models.
*/
class gltfOcclusionTexture_Info
{
public:
	//! Initializes a gltfOcclusionTexture_Info object with default values.
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
	\brief A structure for storing normal texture information from GLTF assets.
*/
class gltfNormalTexture_Info
{
public:
	//! Initializes a gltfNormalTexture_Info object with default values for index, texCoord, and scale.
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
	\brief A structure for storing texture information within a glTF file.
*/
class gltfTexture_Info
{
public:
	//! Initializes a gltfTexture_Info object with default values.
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
	\brief A class representing a texture element within a GLTF asset.
*/
class gltfTexture
{
public:
	//! Initializes a gltfTexture object with default sampler and source values.
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
	\brief Represents a physically based rendering material with metallic-roughness workflow.
*/
class gltfMaterial_pbrMetallicRoughness
{
public:
	//! Constructs a new gltfMaterial_pbrMetallicRoughness object with default values.
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
	\brief A class representing a material definition for glTF assets.
*/
class gltfMaterial
{
public:
	enum gltfAlphaMode { gltfOPAQUE, gltfMASK, gltfBLEND, count };

	//! Initializes a gltfMaterial object with default values.
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

	//! Resolves a glTF alpha mode from a string type.
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
	\brief The gltfAsset class represents and manages a GLTF asset for 3D model loading and rendering.
*/
class gltfAsset
{
public:
	//! Constructs a new gltfAsset object.
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
	\brief A class representing GLTF extensions that are used in the asset pipeline.
*/
class gltfExtensionsUsed
{
public:
	//! Constructor for gltfExtensionsUsed class.
	gltfExtensionsUsed() { }
	idStr extension;
};

/*!
	\class gltfExt_KHR_materials_pbrSpecularGlossiness
	\brief Provides support for theKHR materials pbr specular glossiness extension in glTF assets.
*/
class gltfExt_KHR_materials_pbrSpecularGlossiness
{
public:
	//! Constructs a new gltfExt_KHR_materials_pbrSpecularGlossiness object with default values.
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
	\brief A structure representing a spot light extension for glTF lighting.
*/
class gltfExt_KHR_lights_punctual_spot
{
public:
	//! Initializes a new instance of the gltfExt_KHR_lights_punctual_spot structure with default inner and outer cone angles.
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
	\brief Represents a punctual light definition compatible with the KHR_lights_punctual extension.
*/
class gltfExt_KHR_lights_punctual
{
public:
	enum Type { Directional, Point, Spot, count };

	//! Initializes a gltfExt_KHR_lights_punctual object with default values for color, intensity, range, and light type.
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

	//! Resolves a string representation of a light type into its corresponding enumeration value.
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
	\brief Provides texture transformation functionality for glTF files.
*/
class gltfExt_KHR_texture_transform
{
public:
	//! Initializes a new instance of the gltfExt_KHR_texture_transform class with default values.
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
	\brief Manages GLTF data parsing, storage, and retrieval for 3D assets.

	This class serves as a central manager for GLTF asset data, handling parsing, storage, and retrieval of various GLTF components such as scenes, nodes, meshes, cameras, materials, animations, and
   skins. It maintains lists of different GLTF elements and provides methods to access and manipulate them. The class supports both loading and creating GLTF data from files, with mechanisms to
   resolve URI references during parsing. It includes functionality for managing buffer data, accessors, samplers, and other GLTF structures. The design supports hierarchical node traversal, animation
   target resolution, and efficient data access patterns for 3D rendering.

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

	//! Destructor for gltfData that frees allocated memory and clears data.
	~gltfData();

	//! Allocates and returns a pointer to a memory buffer of specified size for GLTF data
	byte* AddData( int size, int* bufferID = nullptr );

	//! Returns the JSON data and size of the glTF file.
	byte* GetJsonData( int& size )
	{
		size = jsonDataLength;
		return json;
	}

	//! Returns a pointer to the data at the specified index.
	byte* GetData( int index ) { return data[index]; }

	//! Sets the file name and its hash for the glTF data.
	void  FileName( const idStr& file, int hash )
	{
		fileName	 = file;
		fileNameHash = hash;
	}

	//! Returns the hash value of the file name.
	int						 FileNameHash() { return fileNameHash; }

	//! Returns a reference to the file name string.
	idStr&					 FileName() { return fileName; }

	static idHashIndex		 fileDataHash;
	static idList<gltfData*> dataList;

	//! Retrieves or creates glTF data for the specified file name.
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

	//! Returns a constant reference to the global list of glTF data objects.
	static const idList<gltfData*>& DataList() { return dataList; }

	//! Clears GLTF data associated with the specified file name.
	static void						ClearData( idStr& fileName );

	//! Returns the GLTF node that controls the specified camera.
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

	//! Retrieves a node from a scene that is associated with a specific mesh, optionally returning the node's index.
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

	//! Retrieves a glTF node by scene name and ID, optionally returning the node's name
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

	//! Retrieves a node from the glTF data by its name, with optional ID retrieval and case sensitivity control
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

	//! Retrieves a mesh node from the glTF data by its name, with optional ID retrieval and case sensitivity.
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

	//! Retrieves a node from a specified scene by name, with optional case sensitivity and ID retrieval
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

	//! Returns the index of the specified node in the glTF data's node list
	int GetNodeIndex( gltfNode* node )
	{
		int index = -1;
		for( auto& it : nodes ) {
			index++;
			if( it == node ) { return index; }
		}
		return -1;
	}

	//! Checks if an animation exists for the specified node ID.
	bool HasAnimation( int nodeID )
	{
		for( auto anim : animations ) {
			for( auto channel : anim->channels ) {
				if( channel->target.node == nodeID ) { return true; }
			}
		}
		return false;
	}

	//! Returns a pointer to the animation with the specified name from the glTF data
	gltfAnimation* GetAnimation( const idStr& animName )
	{
		for( auto* anim : animations ) {
			if( anim->name == animName ) { return anim; }
		}
		return nullptr;
	}

	//! Retrieves a GLTF animation by name that targets the specified node.
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

	//! Returns the index of the scene with the specified name, or -1 if not found.
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

	//! Collects all unique mesh IDs from a GLTF node and its child nodes.
	void GetAllMeshes( gltfNode* node, idList<int>& meshIds )
	{
		if( node->mesh != -1 ) { meshIds.AddUnique( GetNodeIndex( node ) ); }

		for( auto child : node->children ) {
			GetAllMeshes( nodes[child], meshIds );
		}
	}

	//! Populates the provided list with IDs of all mesh nodes.
	void GetAllMeshes( idList<int>& meshIds )
	{
		for( int i = 0; i < nodes.Num(); i++ ) {
			auto* node = nodes[i];

			if( node->mesh != -1 ) { meshIds.AddUnique( i ); }
		}
	}

	//! Collects all mesh IDs from skinned nodes in the GLTF hierarchy starting from the given node.
	void GetAllSkinnedMeshes( gltfNode* node, idList<int>& meshIds )
	{
		if( node->mesh != -1 && node->skin != -1 ) { meshIds.AddUnique( GetNodeIndex( node ) ); }

		for( auto child : node->children ) {
			GetAllSkinnedMeshes( nodes[child], meshIds );
		}
	}

	//! Collects all mesh IDs that are skinned by the specified skin into the provided list.
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

	//! Retrieves the indices of all skinned meshes from the glTF data.
	void GetAllSkinnedMeshes( idList<int>& meshIds )
	{
		for( int i = 0; i < nodes.Num(); i++ ) {
			auto* node = nodes[i];

			if( node->mesh != -1 && node->skin != -1 ) { meshIds.AddUnique( i ); }
		}
	}

	//! Returns a skin from the glTF data by its name
	gltfSkin* GetSkin( const idStr& name )
	{
		for( auto skin : skins ) {
			if( skin->name == name ) { return skin; }
		}

		return nullptr;
	}

	//! Returns the skin associated with the specified bone node ID, or nullptr if no such skin exists.
	gltfSkin* GetSkin( int boneNodeId )
	{
		for( auto skin : skins ) {
			if( skin->joints.Find( boneNodeId ) ) { return skin; }
		}

		return nullptr;
	}

	//! Retrieves the skin associated with the given animation by checking its target nodes
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

	//! Returns a list of unique node indices targeted by the specified animation channels.
	idList<int> GetAnimTargets( gltfAnimation* anim ) const
	{
		idList<int> result;

		for( auto channel : anim->channels ) {
			result.AddUnique( channel->target.node );
		}

		return result;
	}

	//! Returns a list of channel IDs from the specified animation that target the given node.
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

	//! Retrieves the animation IDs associated with a given GLTF node and its children
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

	//! Returns the view matrix for a specified camera ID by traversing the node hierarchy and accumulating transformations.
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

	//! Returns the transformation matrix for a light node in the GLTF hierarchy.
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

	//! Resolves the transformation matrix for a GLTF node and its hierarchy
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

	//! Returns a cached view of the float data contained in the specified GLTF accessor
	template<class T>
	idList<T*>&		GetAccessorView( gltfAccessor* accessor );
	idList<float>&	GetAccessorView( gltfAccessor* accessor );
	idList<idMat4>& GetAccessorViewMat( gltfAccessor* accessor );

	int&			DefaultScene() { return scene; }

	//! Returns a pointer to the newly added buffer in the gltfData object.
	gltfBuffer*		Buffer()
	{
		buffers.AssureSizeAlloc( buffers.Num() + 1, idListNewElement<gltfBuffer> );
		return buffers[buffers.Num() - 1];
	}

	//! Returns a constant reference to the list of glTF buffers.
	const inline idList<gltfBuffer*>& BufferList() { return buffers; }

	//! Returns a pointer to the next available gltfSampler in the samplers list.
	gltfSampler*					  Sampler()
	{
		samplers.AssureSizeAlloc( samplers.Num() + 1, idListNewElement<gltfSampler> );
		return samplers[samplers.Num() - 1];
	}

	//! Returns a constant reference to the list of samplers in the glTF data.
	const inline idList<gltfSampler*>& SamplerList() { return samplers; }

	//! Returns a pointer to the last buffer view in the buffer views list.
	gltfBufferView*					   BufferView()
	{
		bufferViews.AssureSizeAlloc( bufferViews.Num() + 1, idListNewElement<gltfBufferView> );
		return bufferViews[bufferViews.Num() - 1];
	}

	//! Returns a constant reference to the list of buffer views in the glTF data.
	const inline idList<gltfBufferView*>& BufferViewList() { return bufferViews; }

	//! Returns a pointer to the last image in the images list, ensuring the list has enough capacity.
	gltfImage*							  Image()
	{
		images.AssureSizeAlloc( images.Num() + 1, idListNewElement<gltfImage> );
		return images[images.Num() - 1];
	}

	//! Returns a constant reference to the list of images in the glTF data.
	const inline idList<gltfImage*>& ImageList() { return images; }

	//! Returns a pointer to a newly allocated texture element from the textures list.
	gltfTexture*					 Texture()
	{
		textures.AssureSizeAlloc( textures.Num() + 1, idListNewElement<gltfTexture> );
		return textures[textures.Num() - 1];
	}

	//! Returns a constant reference to the list of textures in the glTF data.
	const inline idList<gltfTexture*>& TextureList() { return textures; }

	//! Returns a pointer to a newly created gltfAccessor object.
	gltfAccessor*					   Accessor()
	{
		accessors.AssureSizeAlloc( accessors.Num() + 1, idListNewElement<gltfAccessor> );
		return accessors[accessors.Num() - 1];
	}

	//! Returns the list of accessors from the glTF data
	const inline idList<gltfAccessor*>& AccessorList() { return accessors; }

	//! Returns a pointer to the last element in the extensionsUsed list.
	gltfExtensionsUsed*					ExtensionsUsed()
	{
		extensionsUsed.AssureSizeAlloc( accessors.Num() + 1, idListNewElement<gltfExtensionsUsed> );
		return extensionsUsed[extensionsUsed.Num() - 1];
	}

	//! Returns a constant reference to the list of GLTF extensions used.
	const inline idList<gltfExtensionsUsed*>& ExtensionsUsedList() { return extensionsUsed; }

	//! Returns a pointer to the mesh at the end of the meshes list, ensuring the list is large enough to accommodate a new mesh.
	gltfMesh*								  Mesh()
	{
		meshes.AssureSizeAlloc( meshes.Num() + 1, idListNewElement<gltfMesh> );
		return meshes[meshes.Num() - 1];
	}

	//! Returns a constant reference to the list of meshes in the glTF data.
	const inline idList<gltfMesh*>& MeshList() { return meshes; }

	//! Returns a pointer to the last scene in the glTF data.
	gltfScene*						Scene()
	{
		scenes.AssureSizeAlloc( scenes.Num() + 1, idListNewElement<gltfScene> );
		return scenes[scenes.Num() - 1];
	}

	//! Returns a constant reference to the list of scenes in the glTF data.
	const inline idList<gltfScene*>& SceneList() { return scenes; }

	//! Returns a pointer to a newly allocated node in the gltfData node list.
	gltfNode*						 Node()
	{
		nodes.AssureSizeAlloc( nodes.Num() + 1, idListNewElement<gltfNode> );
		return nodes[nodes.Num() - 1];
	}

	//! Returns a reference to the list of nodes in the glTF data
	const inline idList<gltfNode*>& NodeList() { return nodes; }

	//! Returns a pointer to the camera at the end of the cameras list.
	gltfCamera*						Camera()
	{
		cameras.AssureSizeAlloc( cameras.Num() + 1, idListNewElement<gltfCamera> );
		return cameras[cameras.Num() - 1];
	}

	//! Returns a constant reference to the list of cameras in the glTF data.
	const inline idList<gltfCamera*>& CameraList() { return cameras; }

	//! Returns a pointer to a newly allocated material from the gltfData object.
	gltfMaterial*					  Material()
	{
		materials.AssureSizeAlloc( materials.Num() + 1, idListNewElement<gltfMaterial> );
		return materials[materials.Num() - 1];
	}

	//! Returns a constant reference to the list of materials in the glTF data.
	const inline idList<gltfMaterial*>& MaterialList() { return materials; }

	//! Returns a pointer to the extensions data.
	gltfExtensions*						Extensions()
	{
		extensions.AssureSizeAlloc( extensions.Num() + 1, idListNewElement<gltfExtensions> );
		return extensions[extensions.Num() - 1];
	}

	//! Returns a constant reference to the list of extensions in the glTF data.
	const inline idList<gltfExtensions*>& ExtensionsList() { return extensions; }

	//! Returns a pointer to the newly added animation in the glTF data.
	gltfAnimation*						  Animation()
	{
		animations.AssureSizeAlloc( animations.Num() + 1, idListNewElement<gltfAnimation> );
		return animations[animations.Num() - 1];
	}

	//! Returns a constant reference to the list of animations in the glTF data.
	const inline idList<gltfAnimation*>& AnimationList() { return animations; }

	//! Returns a pointer to the skin at the end of the skins list.
	gltfSkin*							 Skin()
	{
		skins.AssureSizeAlloc( skins.Num() + 1, idListNewElement<gltfSkin> );
		return skins[skins.Num() - 1];
	}

	//! Returns a constant reference to the list of skins contained in the glTF data.
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
