/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2023 Robert Beckebans
Copyright (C) 2016-2017 Dustin Land
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
#ifndef __RENDERPROGS_H__
#define __RENDERPROGS_H__

static const int PC_ATTRIB_INDEX_VERTEX	 = 0;
static const int PC_ATTRIB_INDEX_NORMAL	 = 2;
static const int PC_ATTRIB_INDEX_COLOR	 = 3;
static const int PC_ATTRIB_INDEX_COLOR2	 = 4;
static const int PC_ATTRIB_INDEX_ST		 = 8;
static const int PC_ATTRIB_INDEX_TANGENT = 9;

/*
================================================
vertexMask_t

NOTE: There is a PS3 dependency between the bit flag specified here and the vertex
attribute index and attribute semantic specified in DeclRenderProg.cpp because the
stored render prog vertexMask is initialized with cellCgbGetVertexConfiguration().
The ATTRIB_INDEX_ defines are used to make sure the vertexMask_t and attrib assignment
in DeclRenderProg.cpp are in sync.

Even though VERTEX_MASK_XYZ_SHORT and VERTEX_MASK_ST_SHORT are not real attributes,
they come before the VERTEX_MASK_MORPH to reduce the range of vertex program
permutations defined by the vertexMask_t bits on the Xbox 360 (see MAX_VERTEX_DECLARATIONS).
================================================
*/
enum vertexMask_t {
	VERTEX_MASK_XYZ		= BIT( PC_ATTRIB_INDEX_VERTEX ),
	VERTEX_MASK_ST		= BIT( PC_ATTRIB_INDEX_ST ),
	VERTEX_MASK_NORMAL	= BIT( PC_ATTRIB_INDEX_NORMAL ),
	VERTEX_MASK_COLOR	= BIT( PC_ATTRIB_INDEX_COLOR ),
	VERTEX_MASK_TANGENT = BIT( PC_ATTRIB_INDEX_TANGENT ),
	VERTEX_MASK_COLOR2	= BIT( PC_ATTRIB_INDEX_COLOR2 ),
};

// This enum list corresponds to the global constant register indecies as defined in global.inc for all
// shaders.  We used a shared pool to keeps things simple.  If something changes here then it also
// needs to change in global.inc and vice versa
enum renderParm_t {
	// For backwards compatibility, do not change the order of the first 17 items
	RENDERPARM_SCREENCORRECTIONFACTOR = 0,
	RENDERPARM_WINDOWCOORD,
	RENDERPARM_DIFFUSEMODIFIER,
	RENDERPARM_SPECULARMODIFIER,

	RENDERPARM_LOCALLIGHTORIGIN,
	RENDERPARM_LOCALVIEWORIGIN,

	RENDERPARM_LIGHTPROJECTION_S,
	RENDERPARM_LIGHTPROJECTION_T,
	RENDERPARM_LIGHTPROJECTION_Q,
	RENDERPARM_LIGHTFALLOFF_S,

	RENDERPARM_BUMPMATRIX_S,
	RENDERPARM_BUMPMATRIX_T,

	RENDERPARM_DIFFUSEMATRIX_S,
	RENDERPARM_DIFFUSEMATRIX_T,

	RENDERPARM_SPECULARMATRIX_S,
	RENDERPARM_SPECULARMATRIX_T,

	RENDERPARM_VERTEXCOLOR_MODULATE,
	RENDERPARM_VERTEXCOLOR_ADD,

	// The following are new and can be in any order

	RENDERPARM_COLOR,
	RENDERPARM_VIEWORIGIN,
	RENDERPARM_GLOBALEYEPOS,

	RENDERPARM_MVPMATRIX_X,
	RENDERPARM_MVPMATRIX_Y,
	RENDERPARM_MVPMATRIX_Z,
	RENDERPARM_MVPMATRIX_W,

	RENDERPARM_MODELMATRIX_X,
	RENDERPARM_MODELMATRIX_Y,
	RENDERPARM_MODELMATRIX_Z,
	RENDERPARM_MODELMATRIX_W,

	RENDERPARM_PROJMATRIX_X,
	RENDERPARM_PROJMATRIX_Y,
	RENDERPARM_PROJMATRIX_Z,
	RENDERPARM_PROJMATRIX_W,

	RENDERPARM_MODELVIEWMATRIX_X,
	RENDERPARM_MODELVIEWMATRIX_Y,
	RENDERPARM_MODELVIEWMATRIX_Z,
	RENDERPARM_MODELVIEWMATRIX_W,

	RENDERPARM_TEXTUREMATRIX_S,
	RENDERPARM_TEXTUREMATRIX_T,

	RENDERPARM_TEXGEN_0_S,
	RENDERPARM_TEXGEN_0_T,
	RENDERPARM_TEXGEN_0_Q,
	RENDERPARM_TEXGEN_0_ENABLED,

	RENDERPARM_TEXGEN_1_S,
	RENDERPARM_TEXGEN_1_T,
	RENDERPARM_TEXGEN_1_Q,
	RENDERPARM_TEXGEN_1_ENABLED,

	RENDERPARM_WOBBLESKY_X,
	RENDERPARM_WOBBLESKY_Y,
	RENDERPARM_WOBBLESKY_Z,

	RENDERPARM_OVERBRIGHT,
	RENDERPARM_ENABLE_SKINNING,
	RENDERPARM_ALPHA_TEST,

	// RB begin
	RENDERPARM_AMBIENT_COLOR,

	RENDERPARM_GLOBALLIGHTORIGIN,
	RENDERPARM_JITTERTEXSCALE,
	RENDERPARM_JITTERTEXOFFSET,

	RENDERPARM_PSX_DISTORTIONS,

	RENDERPARM_CASCADEDISTANCES,

	RENDERPARM_SHADOW_MATRIX_0_X, // rpShadowMatrices[6 * 4]
	RENDERPARM_SHADOW_MATRIX_0_Y,
	RENDERPARM_SHADOW_MATRIX_0_Z,
	RENDERPARM_SHADOW_MATRIX_0_W,

	RENDERPARM_SHADOW_MATRIX_1_X,
	RENDERPARM_SHADOW_MATRIX_1_Y,
	RENDERPARM_SHADOW_MATRIX_1_Z,
	RENDERPARM_SHADOW_MATRIX_1_W,

	RENDERPARM_SHADOW_MATRIX_2_X,
	RENDERPARM_SHADOW_MATRIX_2_Y,
	RENDERPARM_SHADOW_MATRIX_2_Z,
	RENDERPARM_SHADOW_MATRIX_2_W,

	RENDERPARM_SHADOW_MATRIX_3_X,
	RENDERPARM_SHADOW_MATRIX_3_Y,
	RENDERPARM_SHADOW_MATRIX_3_Z,
	RENDERPARM_SHADOW_MATRIX_3_W,

	RENDERPARM_SHADOW_MATRIX_4_X,
	RENDERPARM_SHADOW_MATRIX_4_Y,
	RENDERPARM_SHADOW_MATRIX_4_Z,
	RENDERPARM_SHADOW_MATRIX_4_W,

	RENDERPARM_SHADOW_MATRIX_5_X,
	RENDERPARM_SHADOW_MATRIX_5_Y,
	RENDERPARM_SHADOW_MATRIX_5_Z,
	RENDERPARM_SHADOW_MATRIX_5_W,

	RENDERPARM_SHADOW_ATLAS_OFFSET_0, // rpShadowAtlasOffsets[6]
	RENDERPARM_SHADOW_ATLAS_OFFSET_1,
	RENDERPARM_SHADOW_ATLAS_OFFSET_2,
	RENDERPARM_SHADOW_ATLAS_OFFSET_3,
	RENDERPARM_SHADOW_ATLAS_OFFSET_4,
	RENDERPARM_SHADOW_ATLAS_OFFSET_5,

	RENDERPARM_USER0,
	RENDERPARM_USER1,
	RENDERPARM_USER2,
	RENDERPARM_USER3,
	RENDERPARM_USER4,
	RENDERPARM_USER5,
	RENDERPARM_USER6,
	RENDERPARM_USER7,
	// RB end

	RENDERPARM_TOTAL,
};

enum rpStage_t {
	SHADER_STAGE_VERTEX	  = BIT( 0 ),
	SHADER_STAGE_FRAGMENT = BIT( 1 ),
	SHADER_STAGE_COMPUTE  = BIT( 2 ), // RB: for future use

	SHADER_STAGE_DEFAULT = SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT
};

enum rpBinding_t { BINDING_TYPE_UNIFORM_BUFFER, BINDING_TYPE_SAMPLER, BINDING_TYPE_MAX };

struct ShaderBlob {
	void*  data = nullptr;
	size_t size = 0;
};

#define VERTEX_UNIFORM_ARRAY_NAME	"_va_"
#define FRAGMENT_UNIFORM_ARRAY_NAME "_fa_"

static const int AT_VS_IN			= BIT( 1 );
static const int AT_VS_OUT			= BIT( 2 );
static const int AT_PS_IN			= BIT( 3 );
static const int AT_PS_OUT			= BIT( 4 );
static const int AT_VS_OUT_RESERVED = BIT( 5 );
static const int AT_PS_IN_RESERVED	= BIT( 6 );
static const int AT_PS_OUT_RESERVED = BIT( 7 );

struct attribInfo_t {
	const char* type;
	const char* name;
	const char* semantic;
	const char* glsl;
	int			bind;
	int			flags;
	int			vertexMask;
};

extern attribInfo_t attribsPC[];

// Shader macros are used to pick which permutation of a shader to load from a ShaderBlob
// binary file.
struct shaderMacro_t {
	idStr name;
	idStr definition;

	//! Default constructor for shaderMacro_t that initializes name and definition with default values
	shaderMacro_t() :
		name(),
		definition()
	{
	}

	//! Constructs a shader macro with the specified name and definition.
	shaderMacro_t( const idStr& _name, const idStr& _definition ) :
		name( _name ),
		definition( _definition )
	{
	}
};

struct programInfo_t {
	int																	  bindingLayoutType;
	nvrhi::ShaderHandle													  vs;
	nvrhi::ShaderHandle													  ps;
	nvrhi::ShaderHandle													  cs;
	nvrhi::InputLayoutHandle											  inputLayout;
	idStaticList<nvrhi::BindingLayoutHandle, nvrhi::c_MaxBindingLayouts>* bindingLayouts;
};

enum {
	BUILTIN_GUI,
	BUILTIN_COLOR,
	// RB begin
	BUILTIN_COLOR_SKINNED,
	BUILTIN_VERTEX_COLOR,

	BUILTIN_AMBIENT_LIGHTING_IBL,
	BUILTIN_AMBIENT_LIGHTING_IBL_SKINNED,
	BUILTIN_AMBIENT_LIGHTING_IBL_PBR,
	BUILTIN_AMBIENT_LIGHTING_IBL_PBR_SKINNED,

	BUILTIN_AMBIENT_LIGHTGRID_IBL,
	BUILTIN_AMBIENT_LIGHTGRID_IBL_SKINNED,
	BUILTIN_AMBIENT_LIGHTGRID_IBL_PBR,
	BUILTIN_AMBIENT_LIGHTGRID_IBL_PBR_SKINNED,

	BUILTIN_SMALL_GEOMETRY_BUFFER,
	BUILTIN_SMALL_GEOMETRY_BUFFER_SKINNED,
	// RB end
	BUILTIN_TEXTURED,
	BUILTIN_TEXTURE_VERTEXCOLOR,
	BUILTIN_TEXTURE_VERTEXCOLOR_SRGB,
	BUILTIN_TEXTURE_VERTEXCOLOR_SKINNED,
	BUILTIN_TEXTURE_TEXGEN_VERTEXCOLOR,

	BUILTIN_INTERACTION,
	BUILTIN_INTERACTION_SKINNED,
	BUILTIN_INTERACTION_AMBIENT,
	BUILTIN_INTERACTION_AMBIENT_SKINNED,

	// RB begin
	BUILTIN_PBR_INTERACTION,
	BUILTIN_PBR_INTERACTION_SKINNED,
	BUILTIN_PBR_INTERACTION_AMBIENT,
	BUILTIN_PBR_INTERACTION_AMBIENT_SKINNED,

	BUILTIN_INTERACTION_SHADOW_MAPPING_SPOT,
	BUILTIN_INTERACTION_SHADOW_MAPPING_SPOT_SKINNED,
	BUILTIN_INTERACTION_SHADOW_MAPPING_POINT,
	BUILTIN_INTERACTION_SHADOW_MAPPING_POINT_SKINNED,
	BUILTIN_INTERACTION_SHADOW_MAPPING_PARALLEL,
	BUILTIN_INTERACTION_SHADOW_MAPPING_PARALLEL_SKINNED,

	BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_SPOT,
	BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_SPOT_SKINNED,
	BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_POINT,
	BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_POINT_SKINNED,
	BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_PARALLEL,
	BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_PARALLEL_SKINNED,

	BUILTIN_INTERACTION_SHADOW_ATLAS_SPOT,
	BUILTIN_INTERACTION_SHADOW_ATLAS_SPOT_SKINNED,
	BUILTIN_INTERACTION_SHADOW_ATLAS_POINT,
	BUILTIN_INTERACTION_SHADOW_ATLAS_POINT_SKINNED,
	BUILTIN_INTERACTION_SHADOW_ATLAS_PARALLEL,
	BUILTIN_INTERACTION_SHADOW_ATLAS_PARALLEL_SKINNED,

	BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_SPOT,
	BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_SPOT_SKINNED,
	BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_POINT,
	BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_POINT_SKINNED,
	BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_PARALLEL,
	BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_PARALLEL_SKINNED,

	BUILTIN_DEBUG_LIGHTGRID,
	BUILTIN_DEBUG_LIGHTGRID_SKINNED,

	BUILTIN_DEBUG_OCTAHEDRON,
	BUILTIN_DEBUG_OCTAHEDRON_SKINNED,
	// RB end

	BUILTIN_ENVIRONMENT,
	BUILTIN_ENVIRONMENT_SKINNED,
	BUILTIN_BUMPY_ENVIRONMENT,
	BUILTIN_BUMPY_ENVIRONMENT_SKINNED,
	BUILTIN_BUMPY_ENVIRONMENT2,				// RB
	BUILTIN_BUMPY_ENVIRONMENT2_SKINNED,		// RB
	BUILTIN_BUMPY_ENVIRONMENT2_SSR,			// RB
	BUILTIN_BUMPY_ENVIRONMENT2_SSR_SKINNED, // RB

	BUILTIN_DEPTH,
	BUILTIN_DEPTH_SKINNED,

	BUILTIN_BLENDLIGHT,
	BUILTIN_BLENDLIGHT_SKINNED,
	BUILTIN_FOG,
	BUILTIN_FOG_SKINNED,
	BUILTIN_SKYBOX,
	BUILTIN_WOBBLESKY,
	BUILTIN_POSTPROCESS,
	// RB begin
	BUILTIN_POSTPROCESS_RETRO_2BIT,	   // CGA, Gameboy, cool for Gamejams
	BUILTIN_POSTPROCESS_RETRO_C64,	   // Commodore 64
	BUILTIN_POSTPROCESS_RETRO_CPC,	   // Amstrad 6128
	BUILTIN_POSTPROCESS_RETRO_GENESIS, // Sega Genesis / Megadrive
	BUILTIN_POSTPROCESS_RETRO_PSX,	   // Sony Playstation 1
	BUILTIN_CRT_MATTIAS,
	BUILTIN_CRT_NUPIXIE,
	BUILTIN_CRT_EASYMODE,
	BUILTIN_SCREEN,
	BUILTIN_TONEMAP,
	BUILTIN_BRIGHTPASS,
	BUILTIN_HDR_GLARE_CHROMATIC,
	BUILTIN_HDR_DEBUG,

	BUILTIN_SMAA_EDGE_DETECTION,
	BUILTIN_SMAA_BLENDING_WEIGHT_CALCULATION,
	BUILTIN_SMAA_NEIGHBORHOOD_BLENDING,

	BUILTIN_TAA_MOTION_VECTORS,
	BUILTIN_TAA_RESOLVE,
	BUILTIN_TAA_RESOLVE_MSAA_2X,
	BUILTIN_TAA_RESOLVE_MSAA_4X,
	BUILTIN_TAA_RESOLVE_MSAA_8X,

	BUILTIN_AMBIENT_OCCLUSION,
	BUILTIN_AMBIENT_OCCLUSION_AND_OUTPUT,
	BUILTIN_AMBIENT_OCCLUSION_BLUR,
	BUILTIN_AMBIENT_OCCLUSION_BLUR_AND_OUTPUT,

	BUILTIN_DEEP_GBUFFER_RADIOSITY_SSGI,
	BUILTIN_DEEP_GBUFFER_RADIOSITY_BLUR,
	BUILTIN_DEEP_GBUFFER_RADIOSITY_BLUR_AND_OUTPUT,
	// RB end
	BUILTIN_STEREO_DEGHOST,
	BUILTIN_STEREO_WARP,
	BUILTIN_BINK,
	BUILTIN_BINK_SRGB, // SRS - Added Bink shader without sRGB to linear conversion for testVideo cmd
	BUILTIN_BINK_GUI,
	BUILTIN_STEREO_INTERLACE,
	BUILTIN_MOTION_BLUR,

	BUILTIN_DEBUG_SHADOWMAP,

	// SP Begin
	BUILTIN_BLIT,
	BUILTIN_RECT,
	BUILTIN_TONEMAPPING,
	BUILTIN_TONEMAPPING_TEX_ARRAY,
	BUILTIN_HISTOGRAM_CS,
	BUILTIN_HISTOGRAM_TEX_ARRAY_CS,
	BUILTIN_EXPOSURE_CS,
	// SP End

	MAX_BUILTINS
};

/*!
	\class idRenderProgManager
	\brief Manages shader programs and rendering state for the graphics pipeline.

	The idRenderProgManager class is responsible for organizing and managing shader programs, uniform parameters, and rendering state within the graphics pipeline. It provides functionality to
   initialize, load, and bind various shader programs for different rendering stages, including built-in shaders for common rendering tasks such as GUI elements, lighting, and post-processing effects.
   The manager handles shader creation, loading, and binding, as well as uniform parameter management for shader inputs. It supports multiple shader stages, vertex layouts, and shader macros to enable
   flexible rendering configurations. The class also includes methods for managing render parameters, committing uniform values, and handling binding layouts for different shader types. This manager
   acts as a central coordinator for all shader-related operations, ensuring proper state management and efficient shader program usage during rendering.

*/
class idRenderProgManager
{
public:
	//! Initializes a new instance of the idRenderProgManager class.
	idRenderProgManager();
	virtual ~idRenderProgManager();

	//! Initializes the render shader manager with the specified graphics device.
	void Init( nvrhi::IDevice* device );

	//! Shuts down the render program manager by killing all shaders and resetting binding layouts
	void Shutdown();

	//! Initializes the render program manager for a new frame.
	void StartFrame();

	//! Sets a render parameter value for the specified render parameter.
	void SetRenderParm( renderParm_t rp, const float value[4] );

	//! Sets multiple render parameters from an array of float values.
	void SetRenderParms( renderParm_t rp, const float values[], int numValues );

	//! Finds or creates a shader with the specified name and stage
	int	 FindShader( const char* name, rpStage_t stage );
	int	 FindShader( const char* name, rpStage_t stage, const char* nameOutSuffix, uint32 features, bool builtin, vertexLayoutType_t vertexLayout = LAYOUT_DRAW_VERT );

	/*!
		\brief Finds or creates a shader with the specified parameters and returns its index in the shader list

		This function searches for an existing shader that matches the provided name, stage, suffix, macros, and builtin flag. If a matching shader is found, it loads the shader and returns its index.
	   If no matching shader is found, it creates a new shader entry with the provided parameters, appends it to the shader list, loads it, and returns the new index. The shader name is stripped of
	   its file extension before comparison

		\param name Name of the shader to find or create
		\param stage The rendering stage for the shader
		\param nameOutSuffix Suffix to append to the shader name for output
		\param macros List of shader macros to use for the shader
		\param builtin Flag indicating if the shader is built-in
		\param vertexLayout The vertex layout type for the shader, defaults to LAYOUT_DRAW_VERT
		\return The index of the found or created shader in the shaders list
	*/
	int	 FindShader( const char* name, rpStage_t stage, const char* nameOutSuffix, const idList<shaderMacro_t>& macros, bool builtin, vertexLayoutType_t vertexLayout = LAYOUT_DRAW_VERT );

	//! Returns the shader handle at the specified index from the shader collection
	nvrhi::ShaderHandle GetShader( int index );

	//! Retrieves the program information for a specified index from the render program manager.
	programInfo_t		GetProgramInfo( int index );

	//! Returns the index of the currently active render program.
	int					CurrentProgram() const { return currentIndex; }

	//! Binds the program at the specified index for rendering.
	void				BindProgram( int progIndex );

	//! Binds the built-in GUI shader for rendering GUI elements.
	void				BindShader_GUI() { BindShader_Builtin( BUILTIN_GUI ); }

	//! Binds the built-in color shader for rendering.
	void				BindShader_Color() { BindShader_Builtin( BUILTIN_COLOR ); }

	//! Binds the color skinned shader program for rendering.
	void				BindShader_ColorSkinned() { BindShader_Builtin( BUILTIN_COLOR_SKINNED ); }

	//! Binds the built-in vertex color shader for rendering.
	void				BindShader_VertexColor() { BindShader_Builtin( BUILTIN_VERTEX_COLOR ); }

	//! Binds the image-based lighting shader for rendering.
	void				BindShader_ImageBasedLighting() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTING_IBL ); }

	//! Binds the image-based lighting shader for skinned models.
	void				BindShader_ImageBasedLightingSkinned() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTING_IBL_SKINNED ); }

	//! Binds the image-based lighting PBR shader program for rendering.
	void				BindShader_ImageBasedLighting_PBR() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTING_IBL_PBR ); }

	//! Binds the image-based lighting shader for skinned PBR rendering.
	void				BindShader_ImageBasedLightingSkinned_PBR() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTING_IBL_PBR_SKINNED ); }

	//! Binds the image-based light grid shader program.
	void				BindShader_ImageBasedLightGrid() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTGRID_IBL ); }

	//! Binds the image-based light grid skinned shader for rendering.
	void				BindShader_ImageBasedLightGridSkinned() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTGRID_IBL_SKINNED ); }

	//! Binds the image-based light grid PBR shader for rendering.
	void				BindShader_ImageBasedLightGrid_PBR() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTGRID_IBL_PBR ); }

	//! Binds the image-based light grid skinned PBR shader.
	void				BindShader_ImageBasedLightGridSkinned_PBR() { BindShader_Builtin( BUILTIN_AMBIENT_LIGHTGRID_IBL_PBR_SKINNED ); }

	//! Binds the small geometry buffer builtin shader.
	void				BindShader_SmallGeometryBuffer() { BindShader_Builtin( BUILTIN_SMALL_GEOMETRY_BUFFER ); }

	//! Binds the small geometry buffer skinned shader for rendering.
	void				BindShader_SmallGeometryBufferSkinned() { BindShader_Builtin( BUILTIN_SMALL_GEOMETRY_BUFFER_SKINNED ); }

	//! Binds the built-in textured shader for rendering.
	void				BindShader_Texture() { BindShader_Builtin( BUILTIN_TEXTURED ); }

	//! Binds the built-in shader for texture with vertex color.
	void				BindShader_TextureVertexColor() { BindShader_Builtin( BUILTIN_TEXTURE_VERTEXCOLOR ); }

	//! Binds the texture vertex color sRGB shader program.
	void				BindShader_TextureVertexColor_sRGB() { BindShader_Builtin( BUILTIN_TEXTURE_VERTEXCOLOR_SRGB ); }

	//! Binds the texture vertex color skinned shader program for rendering.
	void				BindShader_TextureVertexColorSkinned() { BindShader_Builtin( BUILTIN_TEXTURE_VERTEXCOLOR_SKINNED ); }

	//! Binds the texture texture generation vertex color shader.
	void				BindShader_TextureTexGenVertexColor() { BindShader_Builtin( BUILTIN_TEXTURE_TEXGEN_VERTEXCOLOR ); }

	//! Binds the interaction shader program for rendering.
	void				BindShader_Interaction() { BindShader_Builtin( BUILTIN_INTERACTION ); }

	//! Binds the interaction skinned shader for rendering.
	void				BindShader_InteractionSkinned() { BindShader_Builtin( BUILTIN_INTERACTION_SKINNED ); }

	//! Binds the ambient interaction shader for rendering.
	void				BindShader_InteractionAmbient() { BindShader_Builtin( BUILTIN_INTERACTION_AMBIENT ); }

	//! Binds the interaction ambient skinned shader program.
	void				BindShader_InteractionAmbientSkinned() { BindShader_Builtin( BUILTIN_INTERACTION_AMBIENT_SKINNED ); }

	//! Binds the PBR interaction shader for rendering.
	void				BindShader_PBR_Interaction() { BindShader_Builtin( BUILTIN_PBR_INTERACTION ); }

	//! Binds the PBR interaction skinned shader for rendering.
	void				BindShader_PBR_InteractionSkinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SKINNED ); }

	//! Binds the PBR interaction ambient shader for rendering.
	void				BindShader_PBR_InteractionAmbient() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_AMBIENT ); }

	//! Binds the PBR interaction ambient skinned built-in shader.
	void				BindShader_PBR_InteractionAmbientSkinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_AMBIENT_SKINNED ); }

	//! Binds the shader for interaction shadow mapping with spot light.
	void				BindShader_Interaction_ShadowMapping_Spot() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_MAPPING_SPOT ); }

	//! Binds the shader for spot shadow mapping with skinned geometry.
	void				BindShader_Interaction_ShadowMapping_Spot_Skinned() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_MAPPING_SPOT_SKINNED ); }

	//! Binds the shader for point light shadow mapping interaction.
	void				BindShader_Interaction_ShadowMapping_Point() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_MAPPING_POINT ); }

	//! Binds the shader for point light shadow mapping with skinned geometry.
	void				BindShader_Interaction_ShadowMapping_Point_Skinned() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_MAPPING_POINT_SKINNED ); }

	//! Binds the shader for interaction shadow mapping in parallel mode.
	void				BindShader_Interaction_ShadowMapping_Parallel() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_MAPPING_PARALLEL ); }

	//! Binds the shader for interaction shadow mapping in parallel skinned rendering.
	void				BindShader_Interaction_ShadowMapping_Parallel_Skinned() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_MAPPING_PARALLEL_SKINNED ); }

	//! Binds the PBR interaction shadow mapping spot shader for rendering.
	void				BindShader_PBR_Interaction_ShadowMapping_Spot() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_SPOT ); }

	//! Binds the PBR interaction shadow mapping spot skinned shader for rendering.
	void				BindShader_PBR_Interaction_ShadowMapping_Spot_Skinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_SPOT_SKINNED ); }

	//! Binds the PBR interaction shadow mapping point shader for rendering.
	void				BindShader_PBR_Interaction_ShadowMapping_Point() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_POINT ); }

	//! Binds the PBR interaction shadow mapping point skinned shader for rendering.
	void				BindShader_PBR_Interaction_ShadowMapping_Point_Skinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_POINT_SKINNED ); }

	//! Binds the PBR interaction shadow mapping parallel shader for rendering.
	void				BindShader_PBR_Interaction_ShadowMapping_Parallel() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_PARALLEL ); }

	//! Binds the PBR interaction shadow mapping parallel skinned shader for rendering.
	void				BindShader_PBR_Interaction_ShadowMapping_Parallel_Skinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_MAPPING_PARALLEL_SKINNED ); }

	//! Binds the shader for interaction shadow mapping using a spot light atlas.
	void				BindShader_Interaction_ShadowAtlas_Spot() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_ATLAS_SPOT ); }

	//! Binds the shader for skinned spot shadow interactions.
	void				BindShader_Interaction_ShadowAtlas_Spot_Skinned() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_ATLAS_SPOT_SKINNED ); }

	//! Binds the interaction shadow atlas point builtin shader.
	void				BindShader_Interaction_ShadowAtlas_Point() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_ATLAS_POINT ); }

	//! Binds the shader for interaction shadow atlas point skinned rendering.
	void				BindShader_Interaction_ShadowAtlas_Point_Skinned() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_ATLAS_POINT_SKINNED ); }

	//! Binds the shadow atlas parallel interaction shader.
	void				BindShader_Interaction_ShadowAtlas_Parallel() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_ATLAS_PARALLEL ); }

	//! Binds the shader for interaction shadow atlas parallel skinned rendering.
	void				BindShader_Interaction_ShadowAtlas_Parallel_Skinned() { BindShader_Builtin( BUILTIN_INTERACTION_SHADOW_ATLAS_PARALLEL_SKINNED ); }

	//! Binds the PBR interaction shadow atlas spot shader.
	void				BindShader_PBR_Interaction_ShadowAtlas_Spot() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_SPOT ); }

	//! Binds the PBR interaction shadow atlas spot skinned shader.
	void				BindShader_PBR_Interaction_ShadowAtlas_Spot_Skinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_SPOT_SKINNED ); }

	//! Binds the PBR interaction shadow atlas point shader for rendering.
	void				BindShader_PBR_Interaction_ShadowAtlas_Point() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_POINT ); }

	//! Binds the PBR interaction shadow atlas point skinned shader for rendering.
	void				BindShader_PBR_Interaction_ShadowAtlas_Point_Skinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_POINT_SKINNED ); }

	//! Binds the PBR interaction shadow atlas parallel shader for rendering.
	void				BindShader_PBR_Interaction_ShadowAtlas_Parallel() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_PARALLEL ); }

	//! Binds the PBR interaction shadow atlas parallel skinned shader for rendering.
	void				BindShader_PBR_Interaction_ShadowAtlas_Parallel_Skinned() { BindShader_Builtin( BUILTIN_PBR_INTERACTION_SHADOW_ATLAS_PARALLEL_SKINNED ); }

	//! Binds the debug light grid shader for rendering.
	void				BindShader_DebugLightGrid() { BindShader_Builtin( BUILTIN_DEBUG_LIGHTGRID ); }

	//! Binds the debug light grid skinned shader for rendering.
	void				BindShader_DebugLightGridSkinned() { BindShader_Builtin( BUILTIN_DEBUG_LIGHTGRID_SKINNED ); }

	//! Binds the debug octahedron shader for rendering.
	void				BindShader_DebugOctahedron() { BindShader_Builtin( BUILTIN_DEBUG_OCTAHEDRON ); }

	//! Binds the debug octahedron skinned shader for rendering.
	void				BindShader_DebugOctahedronSkinned() { BindShader_Builtin( BUILTIN_DEBUG_OCTAHEDRON_SKINNED ); }

	//! Binds the environment shader program for rendering.
	void				BindShader_Environment() { BindShader_Builtin( BUILTIN_ENVIRONMENT ); }

	//! Binds the environment skinned shader program for rendering.
	void				BindShader_EnvironmentSkinned() { BindShader_Builtin( BUILTIN_ENVIRONMENT_SKINNED ); }

	//! Binds the built-in bumpy environment shader for rendering.
	void				BindShader_BumpyEnvironment() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT ); }

	//! Binds the bumpy environment skinned shader for rendering.
	void				BindShader_BumpyEnvironmentSkinned() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT_SKINNED ); }

	//! Binds the bumpy environment shader program for rendering.
	void				BindShader_BumpyEnvironment2() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT2 ); }

	//! Binds the bumpy environment2 skinned shader for rendering.
	void				BindShader_BumpyEnvironment2Skinned() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT2_SKINNED ); }

	//! Binds the BumpyEnvironment2_SSR shader program for rendering.
	void				BindShader_BumpyEnvironment2_SSR() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT2_SSR ); }

	//! Binds the BumpyEnvironment2_SSR_Skinned shader program for rendering.
	void				BindShader_BumpyEnvironment2_SSR_Skinned() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT2_SSR_SKINNED ); }

	//! Binds the depth rendering shader.
	void				BindShader_Depth() { BindShader_Builtin( BUILTIN_DEPTH ); }

	//! Binds the depth skinned shader for rendering.
	void				BindShader_DepthSkinned() { BindShader_Builtin( BUILTIN_DEPTH_SKINNED ); }

	//! Binds the blend light shader for rendering.
	void				BindShader_BlendLight() { BindShader_Builtin( BUILTIN_BLENDLIGHT ); }

	//! Binds the fog shader program for rendering.
	void				BindShader_Fog() { BindShader_Builtin( BUILTIN_FOG ); }

	//! Binds the fog skinned shader program.
	void				BindShader_FogSkinned() { BindShader_Builtin( BUILTIN_FOG_SKINNED ); }

	//! Binds the skybox shader for rendering.
	void				BindShader_SkyBox() { BindShader_Builtin( BUILTIN_SKYBOX ); }

	//! Binds the wobble sky shader for rendering.
	void				BindShader_WobbleSky() { BindShader_Builtin( BUILTIN_WOBBLESKY ); }

	//! Binds the stereo deghosting shader for rendering.
	void				BindShader_StereoDeGhost() { BindShader_Builtin( BUILTIN_STEREO_DEGHOST ); }

	//! Binds the stereo warp shader for rendering.
	void				BindShader_StereoWarp() { BindShader_Builtin( BUILTIN_STEREO_WARP ); }

	//! Binds the stereo interlace shader for rendering.
	void				BindShader_StereoInterlace() { BindShader_Builtin( BUILTIN_STEREO_INTERLACE ); }

	//! Binds the post-process shader program for rendering.
	void				BindShader_PostProcess() { BindShader_Builtin( BUILTIN_POSTPROCESS ); }

	//! Binds the Retro C64 post-process shader for rendering.
	void				BindShader_PostProcess_RetroC64() { BindShader_Builtin( BUILTIN_POSTPROCESS_RETRO_C64 ); }

	//! Binds the Retro CPC post-process shader for rendering.
	void				BindShader_PostProcess_RetroCPC() { BindShader_Builtin( BUILTIN_POSTPROCESS_RETRO_CPC ); }

	//! Binds the retro 2-bit post-process shader for rendering.
	void				BindShader_PostProcess_Retro2Bit() { BindShader_Builtin( BUILTIN_POSTPROCESS_RETRO_2BIT ); }

	//! Binds the Retro Genesis post-process shader for rendering.
	void				BindShader_PostProcess_RetroGenesis() { BindShader_Builtin( BUILTIN_POSTPROCESS_RETRO_GENESIS ); }

	//! Binds the Retro PSX post-process shader for rendering.
	void				BindShader_PostProcess_RetroPSX() { BindShader_Builtin( BUILTIN_POSTPROCESS_RETRO_PSX ); }

	//! Binds the CRT Mattias shader program for rendering.
	void				BindShader_CrtMattias() { BindShader_Builtin( BUILTIN_CRT_MATTIAS ); }

	//! Binds the CRT new pixie shader for rendering.
	void				BindShader_CrtNewPixie() { BindShader_Builtin( BUILTIN_CRT_NUPIXIE ); }

	//! Binds the CRT easy mode shader for rendering.
	void				BindShader_CrtEasyMode() { BindShader_Builtin( BUILTIN_CRT_EASYMODE ); }

	//! Binds the screen shader program for rendering.
	void				BindShader_Screen() { BindShader_Builtin( BUILTIN_SCREEN ); }

	//! Binds the tonemap shader program for use in rendering.
	void				BindShader_Tonemap() { BindShader_Builtin( BUILTIN_TONEMAP ); }

	//! Binds the builtin brightpass shader for rendering.
	void				BindShader_Brightpass() { BindShader_Builtin( BUILTIN_BRIGHTPASS ); }

	//! Binds the built-in shader for chromatic glare effects.
	void				BindShader_HDRGlareChromatic() { BindShader_Builtin( BUILTIN_HDR_GLARE_CHROMATIC ); }

	//! Binds the HDR debug shader for rendering.
	void				BindShader_HDRDebug() { BindShader_Builtin( BUILTIN_HDR_DEBUG ); }

	//! Binds the shader used for SMAA edge detection.
	void				BindShader_SMAA_EdgeDetection() { BindShader_Builtin( BUILTIN_SMAA_EDGE_DETECTION ); }

	//! Binds the SMAA blending weight calculation shader.
	void				BindShader_SMAA_BlendingWeightCalculation() { BindShader_Builtin( BUILTIN_SMAA_BLENDING_WEIGHT_CALCULATION ); }

	//! Binds the SMAA neighborhood blending shader for rendering.
	void				BindShader_SMAA_NeighborhoodBlending() { BindShader_Builtin( BUILTIN_SMAA_NEIGHBORHOOD_BLENDING ); }

	//! Binds the ambient occlusion shader for rendering.
	void				BindShader_AmbientOcclusion() { BindShader_Builtin( BUILTIN_AMBIENT_OCCLUSION ); }

	//! Binds the ambient occlusion and output shader program.
	void				BindShader_AmbientOcclusionAndOutput() { BindShader_Builtin( BUILTIN_AMBIENT_OCCLUSION_AND_OUTPUT ); }

	//! Binds the ambient occlusion blur shader for rendering.
	void				BindShader_AmbientOcclusionBlur() { BindShader_Builtin( BUILTIN_AMBIENT_OCCLUSION_BLUR ); }

	//! Binds the ambient occlusion blur and output shader for rendering.
	void				BindShader_AmbientOcclusionBlurAndOutput() { BindShader_Builtin( BUILTIN_AMBIENT_OCCLUSION_BLUR_AND_OUTPUT ); }

	//! Binds the deep G-buffer radiosity shader for rendering.
	void				BindShader_DeepGBufferRadiosity() { BindShader_Builtin( BUILTIN_DEEP_GBUFFER_RADIOSITY_SSGI ); }

	//! Binds the deep G-buffer radiosity blur shader for rendering.
	void				BindShader_DeepGBufferRadiosityBlur() { BindShader_Builtin( BUILTIN_DEEP_GBUFFER_RADIOSITY_BLUR ); }

	//! Binds the deep G-buffer radiosity blur and output shader program.
	void				BindShader_DeepGBufferRadiosityBlurAndOutput() { BindShader_Builtin( BUILTIN_DEEP_GBUFFER_RADIOSITY_BLUR_AND_OUTPUT ); }

	//! Binds the BINK shader program for rendering.
	void				BindShader_Bink() { BindShader_Builtin( BUILTIN_BINK ); }

	//! Binds the Bink sRGB shader program for rendering.
	void				BindShader_Bink_sRGB() { BindShader_Builtin( BUILTIN_BINK_SRGB ); }

	//! Binds the built-in shader for Bink GUI rendering.
	void				BindShader_BinkGUI() { BindShader_Builtin( BUILTIN_BINK_GUI ); }

	//! Binds the motion blur shader for rendering.
	void				BindShader_MotionBlur() { BindShader_Builtin( BUILTIN_MOTION_BLUR ); }

	//! Binds the motion vectors shader for use in rendering.
	void				BindShader_MotionVectors() { BindShader_Builtin( BUILTIN_TAA_MOTION_VECTORS ); }

	//! Binds the debug shadow map shader.
	void				BindShader_DebugShadowMap() { BindShader_Builtin( BUILTIN_DEBUG_SHADOWMAP ); }

	//! Returns true if the current shader program uses joint data for skinning.
	bool				ShaderUsesJoints() const { return renderProgs[currentIndex].usesJoints; }

	//! Returns true if the current shader has optional skinning support.
	bool				ShaderHasOptionalSkinning() const
	{
		// #if defined( USE_NVRHI )
		//  FIXME
		return false;
		// #else
		//		return renderProgs[current].optionalSkinning;
		// #endif
	}

	//! Unbinds the currently bound render program.
	void			 Unbind();

	//! Checks whether a shader is currently bound to the rendering pipeline.
	bool			 IsShaderBound() const;

	//! Loads all shaders and render programs managed by the renderer.
	void			 LoadAllShaders();

	//! Destroys all shaders managed by the render program manager
	void			 KillAllShaders();

	static const int MAX_GLSL_USER_PARMS = 8;
	const char*		 GetGLSLParmName( int rp ) const;

	//! Sets a uniform value for a specified render parameter.
	void			 SetUniformValue( const renderParm_t rp, const float value[4] );

	//! Commits uniform values based on the provided state bits.
	void			 CommitUniforms( uint64 stateBits );

	/*!
		\brief Finds or creates a graphics program with the specified vertex and fragment shader indices and binding type.

		This function searches for an existing graphics program that matches the provided vertex shader index and fragment shader index. If a matching program is not found, it creates a new program
	   entry, loads the program with the specified shaders, and returns the index of the program. The program is identified by its name and uses the specified binding layout type.

		\param name The name of the program to find or create
		\param vIndex Index of the vertex shader to use for this program
		\param fIndex Index of the fragment shader to use for this program
		\param bindingType The binding layout type to use for this program, defaults to BINDING_LAYOUT_DEFAULT
		\return Index of the found or newly created graphics program
	*/
	int				 FindProgram( const char* name, int vIndex, int fIndex, bindingLayoutType_t bindingType = BINDING_LAYOUT_DEFAULT );

	//! Initializes all uniform values to zero
	void			 ZeroUniforms();

	//! Returns the allocated size of the uniforms array.
	int				 UniformSize();

	//! Updates the constant buffer if changes have been made
	bool			 CommitConstantBuffer( nvrhi::ICommandList* commandList, bool bindingLayoutTypeChanged );

	//! Returns the constant buffer used for shader parameter bindings in the rendering pipeline.
	ID_INLINE nvrhi::IBuffer* ConstantBuffer()
	{
		return constantBuffer; //[BindingLayoutType()];
	}

	//! Returns a reference to the uniform buffer for the current binding layout type.
	ID_INLINE idUniformBuffer& BindingParamUbo() { return bindingParmUbo[BindingLayoutType()]; }

	//! Returns the input layout handle for the current render program.
	ID_INLINE nvrhi::InputLayoutHandle InputLayout() { return renderProgs[currentIndex].inputLayout; }

	//! Returns the binding layout type for the current render program.
	ID_INLINE int					   BindingLayoutType() { return renderProgs[currentIndex].bindingLayoutType; }

	//! Returns a pointer to the binding layout list for the specified layout type.
	ID_INLINE idStaticList<nvrhi::BindingLayoutHandle, nvrhi::c_MaxBindingLayouts>* GetBindingLayout( int layoutType ) { return &bindingLayouts[layoutType]; }

	idUniformBuffer																	renderParmUbo;
	idArray<idUniformBuffer, NUM_BINDING_LAYOUTS>									bindingParmUbo;
	idArray<idVec4*, NUM_BINDING_LAYOUTS>											mappedRenderParms;

	static const char*																FindEmbeddedSourceShader( const char* name );

private:
	//! Loads a shader at the specified index and stage if it hasn't already been loaded.
	void			  LoadShader( int index, rpStage_t stage );

	//! Reads a binary shader file and returns its contents in a ShaderBlob structure.
	ShaderBlob		  GetBytecode( const char* fileName );

	int				  builtinShaders[MAX_BUILTINS];

	//! Binds a built-in shader program by its index.
	void			  BindShader_Builtin( int i ) { BindProgram( i ); }

	//! Loads a shader program with the specified vertex and fragment shader indices into the given program index.
	void			  LoadProgram( const int programIndex, const int vertexShaderIndex, const int fragmentShaderIndex );

	//! Loads a compute program with the specified indices into the render program manager.
	void			  LoadComputeProgram( const int programIndex, const int computeShaderIndex );

	static const uint INVALID_PROGID = 0xFFFFFFFF;

	struct shader_t {
		//! Initializes a new shader_t object with default values.
		shader_t() :
			name(),
			nameOutSuffix(),
			shaderFeatures( 0 ),
			builtin( false ),
			macros(),
			handle( nullptr ),
			stage( SHADER_STAGE_DEFAULT )
		{
		}

		idStr				  name;
		idStr				  nameOutSuffix;
		uint32				  shaderFeatures;
		bool				  builtin;
		idList<shaderMacro_t> macros;
		nvrhi::ShaderHandle	  handle;
		std::vector<uint8_t>  ownedBytecode;
		rpStage_t			  stage;
	};

	struct renderProg_t {
		//! Initializes a new instance of the renderProg_t struct with default values.
		renderProg_t() :
			name(),
			vertexShaderIndex( -1 ),
			fragmentShaderIndex( -1 ),
			computeShaderIndex( -1 ),
			builtin( true ),
			usesJoints( false ),
			vertexLayout( LAYOUT_UNKNOWN ),
			bindingLayoutType( BINDING_LAYOUT_DEFAULT ),
			inputLayout( nullptr ),
			bindingLayouts()
		{
		}

		idStr																 name;
		int																	 vertexShaderIndex;
		int																	 fragmentShaderIndex;
		int																	 computeShaderIndex;
		bool																 builtin;
		bool																 usesJoints;
		vertexLayoutType_t													 vertexLayout;
		bindingLayoutType_t													 bindingLayoutType;
		nvrhi::InputLayoutHandle											 inputLayout;
		idStaticList<nvrhi::BindingLayoutHandle, nvrhi::c_MaxBindingLayouts> bindingLayouts;
	};

	//! Loads a shader from a bytecode file and creates a shader handle for the specified shader stage.
	void								   LoadShader( shader_t& shader );

	int									   currentIndex;
	idList<renderProg_t, TAG_RENDER>	   renderProgs;
	idList<shader_t, TAG_RENDER>		   shaders;
	idStaticList<idVec4, RENDERPARM_TOTAL> uniforms;
	bool								   uniformsChanged;
	nvrhi::IDevice*						   device;

	using VertexAttribDescList = idList<nvrhi::VertexAttributeDesc>;
	idStaticList<VertexAttribDescList, NUM_VERTEX_LAYOUTS>													vertexLayoutDescs;

	idStaticList<idStaticList<nvrhi::BindingLayoutHandle, nvrhi::c_MaxBindingLayouts>, NUM_BINDING_LAYOUTS> bindingLayouts;

	// idArray<nvrhi::BufferHandle, NUM_BINDING_LAYOUTS>	constantBuffer;
	nvrhi::BufferHandle																						constantBuffer;
};

extern idRenderProgManager renderProgManager;

#endif
