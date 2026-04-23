/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2025 Robert Beckebans
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

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

/*
===============================================================================

	Material

===============================================================================
*/

class idImage;
class idCinematic;
class idUserInterface;

// clang-format off

// moved from image.h for default parm
typedef enum
{
	TF_LINEAR,
	TF_NEAREST,
	TF_NEAREST_MIPMAP,		// RB: no linear interpolation but explicit mip-map levels for hierarchical depth buffer
	TF_DEFAULT				// use the user-specified r_textureFilter
} textureFilter_t;

typedef enum
{
	TR_REPEAT,
	TR_CLAMP,
	TR_CLAMP_TO_ZERO,		// guarantee 0,0,0,255 edge for projected textures
	TR_CLAMP_TO_ZERO_ALPHA	// guarantee 0 alpha edge for projected textures
} textureRepeat_t;

typedef struct
{
	int		stayTime;		// msec for no change
	int		fadeTime;		// msec to fade vertex colors over
	float	start[4];		// vertex color at spawn (possibly out of 0.0 - 1.0 range, will clamp after calc)
	float	end[4];			// vertex color at fade-out (possibly out of 0.0 - 1.0 range, will clamp after calc)
} decalInfo_t;

typedef enum
{
	DFRM_NONE,
	DFRM_SPRITE,
	DFRM_TUBE,
	DFRM_FLARE,
	DFRM_EXPAND,
	DFRM_MOVE,
	DFRM_EYEBALL,
	DFRM_PARTICLE,
	DFRM_PARTICLE2,
	DFRM_TURB
} deform_t;

typedef enum
{
	DI_STATIC,
	DI_SCRATCH,		// video, screen wipe, etc
	DI_CUBE_RENDER,
	DI_MIRROR_RENDER,
	DI_XRAY_RENDER,
	DI_REMOTE_RENDER,
	DI_GUI_RENDER,
	DI_RENDER_TARGET,
} dynamicidImage_t;

// note: keep opNames[] in sync with changes
typedef enum
{
	OP_TYPE_ADD,
	OP_TYPE_SUBTRACT,
	OP_TYPE_MULTIPLY,
	OP_TYPE_DIVIDE,
	OP_TYPE_MOD,
	OP_TYPE_TABLE,
	OP_TYPE_GT,
	OP_TYPE_GE,
	OP_TYPE_LT,
	OP_TYPE_LE,
	OP_TYPE_EQ,
	OP_TYPE_NE,
	OP_TYPE_AND,
	OP_TYPE_OR,
	OP_TYPE_SOUND
} expOpType_t;

typedef enum
{
	EXP_REG_TIME,

	EXP_REG_PARM0,
	EXP_REG_PARM1,
	EXP_REG_PARM2,
	EXP_REG_PARM3,
	EXP_REG_PARM4,
	EXP_REG_PARM5,
	EXP_REG_PARM6,
	EXP_REG_PARM7,
	EXP_REG_PARM8,
	EXP_REG_PARM9,
	EXP_REG_PARM10,
	EXP_REG_PARM11,

	EXP_REG_GLOBAL0,
	EXP_REG_GLOBAL1,
	EXP_REG_GLOBAL2,
	EXP_REG_GLOBAL3,
	EXP_REG_GLOBAL4,
	EXP_REG_GLOBAL5,
	EXP_REG_GLOBAL6,
	EXP_REG_GLOBAL7,

	EXP_REG_NUM_PREDEFINED
} expRegister_t;

typedef struct
{
	expOpType_t		opType;
	int				a, b, c;
} expOp_t;

typedef struct
{
	int				registers[4];
} colorStage_t;

typedef enum
{
	TG_EXPLICIT,
	TG_DIFFUSE_CUBE,
	TG_REFLECT_CUBE,
	TG_REFLECT_CUBE2,	// RB: used for new blood effects, interpolates 3 env_probes + SSR
	TG_SKYBOX_CUBE,
	TG_WOBBLESKY_CUBE,
	TG_SCREEN,			// screen aligned, for mirrorRenders and screen space temporaries
	TG_SCREEN2,
	TG_GLASSWARP,
} texgen_t;

typedef struct
{
	idCinematic* 		cinematic;
	idImage* 			image;
	texgen_t			texgen;
	bool				hasMatrix;
	int					matrix[2][3];	// we only allow a subset of the full projection matrix

	// dynamic image variables
	dynamicidImage_t	dynamic;
	int					width, height;
	int					dynamicFrameCount;
} textureStage_t;

// the order BUMP / DIFFUSE / SPECULAR is necessary for interactions to draw correctly on low end cards
typedef enum
{
	SL_AMBIENT,						// execute after lighting
	SL_BUMP,
	SL_DIFFUSE,
	SL_SPECULAR,
	SL_COVERAGE,
} stageLighting_t;

// cross-blended terrain textures need to modulate the color by
// the vertex color to smoothly blend between two textures
typedef enum
{
	SVC_IGNORE,
	SVC_MODULATE,
	SVC_INVERSE_MODULATE
} stageVertexColor_t;

// SP Begin
typedef enum
{
	STENCIL_COMP_GREATER,
	STENCIL_COMP_GEQUAL,
	STENCIL_COMP_LESS,
	STENCIL_COMP_LEQUAL,
	STENCIL_COMP_EQUAL,
	STENCIL_COMP_NOTEQUAL,
	STENCIL_COMP_ALWAYS,
	STENCIL_COMP_NEVER
} stencilComp_t;

typedef enum
{
	STENCIL_OP_KEEP,
	STENCIL_OP_ZERO,
	STENCIL_OP_REPLACE,
	STENCIL_OP_INCRSAT,
	STENCIL_OP_DECRSAT,
	STENCIL_OP_INVERT,
	STENCIL_OP_INCRWRAP,
	STENCIL_OP_DECRWRAP
} stencilOperation_t;

// clang-format on

struct stencilStage_t {
	// The value to be compared against (if Comp is anything else than always) and/or the value to be written to the buffer
	// (if either Pass, Fail or ZFail is set to replace).
	byte			   ref = 0;

	// An 8 bit mask as an 0–255 integer, used when comparing the reference value with the contents of the buffer
	// (referenceValue & readMask) comparisonFunction (stencilBufferValue & readMask).
	byte			   readMask = 255;

	// An 8 bit mask as an 0–255 integer, used when writing to the buffer.Note that, like other write masks,
	// it specifies which bits of stencil buffer will be affected by write
	// (i.e.WriteMask 0 means that no bits are affected and not that 0 will be written).
	byte			   writeMask = 255;

	// Function used to compare the reference value to the current contents of the buffer.
	stencilComp_t	   comp = STENCIL_COMP_ALWAYS;

	// What to do with the contents of the buffer if the stencil test(and the depth test) passes.
	stencilOperation_t pass = STENCIL_OP_KEEP;

	// What to do with the contents of the buffer if the stencil test fails.
	stencilOperation_t fail = STENCIL_OP_KEEP;

	// What to do with the contents of the buffer if the stencil test passes, but the depth test fails.
	stencilOperation_t zFail = STENCIL_OP_KEEP;
};
// SP End

static const int MAX_FRAGMENT_IMAGES = 8;
static const int MAX_VERTEX_PARMS	 = 4;

typedef struct {
	int		 vertexProgram;
	int		 numVertexParms;
	int		 vertexParms[MAX_VERTEX_PARMS][4]; // evaluated register indexes

	int		 fragmentProgram;
	int		 glslProgram;
	int		 numFragmentProgramImages;
	idImage* fragmentProgramImages[MAX_FRAGMENT_IMAGES];
} newShaderStage_t;

typedef struct {
	int				   conditionRegister; // if registers[conditionRegister] == 0, skip stage
	stageLighting_t	   lighting;		  // determines which passes interact with lights
	uint64			   drawStateBits;
	colorStage_t	   color;
	bool			   hasAlphaTest;
	int				   alphaTestRegister;
	textureStage_t	   texture;
	stageVertexColor_t vertexColor;
	bool			   ignoreAlphaTest; // this stage should act as translucent, even
	// if the surface is alpha tested
	float			   privatePolygonOffset; // a per-stage polygon offset

	stencilStage_t*	   stencilStage;
	newShaderStage_t*  newStage; // vertex / fragment program based stage
} shaderStage_t;

typedef enum {
	MC_BAD,
	MC_OPAQUE,	   // completely fills the triangle, will have black drawn on fillDepthBuffer
	MC_PERFORATED, // may have alpha tested holes
	MC_TRANSLUCENT // blended with background
} materialCoverage_t;

typedef enum {
	SS_SUBVIEW = -3, // mirrors, viewscreens, etc
	SS_GUI	   = -2, // guis
	SS_BAD	   = -1,
	SS_OPAQUE, // opaque

	SS_PORTAL_SKY,

	SS_DECAL, // scorch marks, etc.

	SS_FAR,
	SS_MEDIUM, // normal translucent
	SS_CLOSE,

	SS_ALMOST_NEAREST, // gun smoke puffs

	SS_NEAREST, // screen blood blobs

	SS_POST_PROCESS = 100 // after a screen copy to texture
} materialSort_t;

enum SubViewType : uint16_t { SUBVIEW_NONE, SUBVIEW_MIRROR, SUBVIEW_DIRECT_PORTAL };

typedef enum { CT_FRONT_SIDED, CT_BACK_SIDED, CT_TWO_SIDED } cullType_t;

// these don't effect per-material storage, so they can be very large
const int MAX_SHADER_STAGES = 256;

const int MAX_TEXGEN_REGISTERS = 4;

const int MAX_ENTITY_SHADER_PARMS = 12;
const int MAX_GLOBAL_SHADER_PARMS = 12; // ? this looks like it should only be 8

// material flags
typedef enum {
	MF_DEFAULTED	  = BIT( 0 ),
	MF_POLYGONOFFSET  = BIT( 1 ),
	MF_NOSHADOWS	  = BIT( 2 ),
	MF_FORCESHADOWS	  = BIT( 3 ),
	MF_NOSELFSHADOW	  = BIT( 4 ),
	MF_NOPORTALFOG	  = BIT( 5 ), // this fog volume won't ever consider a portal fogged out
	MF_EDITOR_VISIBLE = BIT( 6 ), // in use (visible) per editor
	// motorsep 11-23-2014; material LOD keys that define what LOD iteration the surface falls into
	MF_LOD1_SHIFT	  = 7,
	MF_LOD1			  = BIT( 7 ),  // motorsep 11-24-2014; material flag for LOD1 iteration
	MF_LOD2			  = BIT( 8 ),  // motorsep 11-24-2014; material flag for LOD2 iteration
	MF_LOD3			  = BIT( 9 ),  // motorsep 11-24-2014; material flag for LOD3 iteration
	MF_LOD4			  = BIT( 10 ), // motorsep 11-24-2014; material flag for LOD4 iteration
	MF_LOD_PERSISTENT = BIT( 11 ), // motorsep 11-24-2014; material flag for persistent LOD iteration
	MF_ORIGIN		  = BIT( 12 ), // Admer: for origin brushes
	MF_UNLIT		  = BIT( 13 ), // RB: receive no lighting
} materialFlags_t;

// contents flags, NOTE: make sure to keep the defines in doom_defs.script up to date with these!
typedef enum {
	CONTENTS_SOLID				= BIT( 0 ),	 // an eye is never valid in a solid
	CONTENTS_OPAQUE				= BIT( 1 ),	 // blocks visibility (for ai)
	CONTENTS_WATER				= BIT( 2 ),	 // used for water
	CONTENTS_PLAYERCLIP			= BIT( 3 ),	 // solid to players
	CONTENTS_MONSTERCLIP		= BIT( 4 ),	 // solid to monsters
	CONTENTS_MOVEABLECLIP		= BIT( 5 ),	 // solid to moveable entities
	CONTENTS_IKCLIP				= BIT( 6 ),	 // solid to IK
	CONTENTS_BLOOD				= BIT( 7 ),	 // used to detect blood decals
	CONTENTS_BODY				= BIT( 8 ),	 // used for actors
	CONTENTS_PROJECTILE			= BIT( 9 ),	 // used for projectiles
	CONTENTS_CORPSE				= BIT( 10 ), // used for dead bodies
	CONTENTS_RENDERMODEL		= BIT( 11 ), // used for render models for collision detection
	CONTENTS_TRIGGER			= BIT( 12 ), // used for triggers
	CONTENTS_AAS_SOLID			= BIT( 13 ), // solid for AAS
	CONTENTS_AAS_OBSTACLE		= BIT( 14 ), // used to compile an obstacle into AAS that can be enabled/disabled
	CONTENTS_FLASHLIGHT_TRIGGER = BIT( 15 ), // used for triggers that are activated by the flashlight

	// jmarshall: used by Quake 3 bots
	CONTENTS_SLIME = BIT( 16 ), // used for slime
	CONTENTS_FOG   = BIT( 17 ), // used for fog
	CONTENTS_LAVA  = BIT( 18 ),
	// jmarshall end

	// contents used by utils
	CONTENTS_AREAPORTAL = BIT( 20 ), // portal separating renderer areas
	CONTENTS_NOCSG		= BIT( 21 ), // don't cut this brush with CSG operations in the editor
	CONTENTS_ORIGIN		= BIT( 22 ),

	CONTENTS_REMOVE_UTIL = ~( CONTENTS_AREAPORTAL | CONTENTS_NOCSG )
} contentsFlags_t;

// surface types
const int NUM_SURFACE_BITS	= 4;
const int MAX_SURFACE_TYPES = 1 << NUM_SURFACE_BITS;

typedef enum {
	SURFTYPE_NONE, // default type
	SURFTYPE_METAL,
	SURFTYPE_STONE,
	SURFTYPE_FLESH,
	SURFTYPE_WOOD,
	SURFTYPE_CARDBOARD,
	SURFTYPE_LIQUID,
	SURFTYPE_GLASS,
	SURFTYPE_PLASTIC,
	SURFTYPE_RICOCHET,
	SURFTYPE_10,
	SURFTYPE_11,
	SURFTYPE_12,
	SURFTYPE_13,
	SURFTYPE_14,
	SURFTYPE_15
} surfTypes_t;

// surface flags
typedef enum {
	SURF_TYPE_BIT0 = BIT( 0 ), // encodes the material type (metal, flesh, concrete, etc.)
	SURF_TYPE_BIT1 = BIT( 1 ), // "
	SURF_TYPE_BIT2 = BIT( 2 ), // "
	SURF_TYPE_BIT3 = BIT( 3 ), // "
	SURF_TYPE_MASK = ( 1 << NUM_SURFACE_BITS ) - 1,

	SURF_NODAMAGE	= BIT( 4 ),	 // never give falling damage
	SURF_SLICK		= BIT( 5 ),	 // effects game physics
	SURF_COLLISION	= BIT( 6 ),	 // collision surface
	SURF_LADDER		= BIT( 7 ),	 // player can climb up this surface
	SURF_NOIMPACT	= BIT( 8 ),	 // don't make missile explosions
	SURF_NOSTEPS	= BIT( 9 ),	 // no footstep sounds
	SURF_DISCRETE	= BIT( 10 ), // not clipped or merged by utilities
	SURF_NOFRAGMENT = BIT( 11 ), // dmap won't cut surface at each bsp boundary
	SURF_NULLNORMAL = BIT( 12 ), // renderbump will draw this surface as 0x80 0x80 0x80, which won't collect light from any angle
	SURF_OCCLUSION	= BIT( 13 ), // RB: occluder surface
} surfaceFlags_t;

class idSoundEmitter;
// clang-format off

// RB: predefined Quake 1 light styles
static const char* predef_lightstyles[] =
{
	"m",
	"mmnmmommommnonmmonqnmmo",
	"abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba",
	"mmmmmaaaaammmmmaaaaaabcdefgabcdefg",
	"mamamamamama",
	"jklmnopqrstuvwxyzyxwvutsrqponmlkj",
	"nmonqnmomnmomomno",
	"mmmaaaabcdefgmmmmaaaammmaamm",
	"mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa",
	"aaaaaaaazzzzzzzz",
	"mmamammmmammamamaaamammma",
	"abcdefghijklmnopqrrqponmlkjihgfedcba"
};

static const char* predef_lightstylesinfo[] =
{
	"Normal",
	"Flicker A",
	"Slow Strong Pulse",
	"Candle A",
	"Fast Strobe",
	"Gentle Pulse",
	"Flicker B",
	"Candle B",
	"Candle C",
	"Slow Strobe",
	"Fluorescent Flicker",
	"Slow Pulse (no black)"
};
// clang-format on

/*!
	\class idMaterial
	\brief Represents a material definition used for rendering surfaces with various properties and shader stages.

	This class encapsulates the definition of a material used in rendering, including its shader stages, surface properties, lighting interactions, and rendering parameters. It provides functionality
   for parsing material definitions from text, managing shader stages and their properties, and determining how the material should be rendered. The class supports various rendering features such as
   blending modes, stencil operations, deformations, and cinematic effects. It also handles material flags, content and surface flags, and provides methods for retrieving material properties used
   during the rendering pipeline. The class inherits from idDecl, indicating it's a declaration that can be loaded and managed by the engine's resource system. It supports loading and saving material
   definitions, reloading images, and managing references to associated textures.

*/
class idMaterial : public idDecl
{
public:
	//! Initializes a new instance of the idMaterial class.
	idMaterial();
	virtual ~idMaterial();

	//! Returns the size of the material declaration in memory
	virtual size_t		 Size() const;

	//! Sets the text source to a default text if necessary, generating an implicit material definition based on the material name.
	virtual bool		 SetDefaultText();

	//! Returns the default definition string that can be parsed to recreate a declaration's default state
	virtual const char*	 DefaultDefinition() const;

	//! Parses the given text data for the material declaration, returning true if successful.
	virtual bool		 Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees any pointers held by the material and prepares it for re-parsing.
	virtual void		 FreeData();

	//! Prints the material declaration data to the console
	virtual void		 Print() const;

	//! Saves the material to a file.
	bool				 Save( const char* fileName = NULL );

	//! Returns the internal image name for stage 0, which can be used for the renderer CaptureRenderToImage() call
	virtual const char*	 ImageName() const;

	//! Reloads all images used by the material stages.
	void				 ReloadImages( bool force, nvrhi::ICommandList* commandList ) const;

	//! Returns the number of stages contained in this material.
	const int			 GetNumStages() const { return numStages; }

	// if the material is simple, all that needs to be known are
	// the images for drawing.
	// These will either all return valid images, or all return NULL
	idImage*			 GetFastPathBumpImage() const { return fastPathBumpImage; };
	idImage*			 GetFastPathDiffuseImage() const { return fastPathDiffuseImage; };
	idImage*			 GetFastPathSpecularImage() const { return fastPathSpecularImage; };

	//! Returns a specific shader stage from the material by index
	const shaderStage_t* GetStage( const int index ) const
	{
		assert( index >= 0 && index < numStages );
		return &stages[index];
	}

	//! Returns the first bump map stage from the material, or NULL if no bump stage is present.
	const shaderStage_t* GetBumpStage() const;

	//! Returns true if the material will draw anything at all
	bool				 IsDrawn() const { return ( numStages > 0 || entityGui != 0 || gui != NULL ); }

	//! Returns true if the material will draw any non light interaction stages.
	bool				 HasAmbient() const { return ( numAmbientStages > 0 ); }

	//! Returns true if the material has a GUI.
	bool				 HasGui() const { return ( entityGui != 0 || gui != NULL ); }

	//! Returns true if the material will generate another view, either as a mirror or dynamic rendered image.
	bool				 HasSubview() const { return hasSubview; }

	//! Returns true if the material is a portal subview.
	bool				 IsPortalSubView() const { return subViewType == SubViewType::SUBVIEW_DIRECT_PORTAL; }

	//! Returns true if the material is configured as a mirror sub-view, otherwise false.
	bool				 IsMirrorSubView() const { return subViewType == SubViewType::SUBVIEW_MIRROR; }

	//! Returns true if the material will generate shadows, without distinguishing between global and self-shadowing.
	bool				 SurfaceCastsShadow() const { return TestMaterialFlag( MF_FORCESHADOWS ) || !TestMaterialFlag( MF_NOSHADOWS ); }

	//! Returns true if the material will generate interactions with fog or blend lights
	bool				 ReceivesFog() const { return ( IsDrawn() && !noFog && coverage != MC_TRANSLUCENT ); }

	//! Returns true if the material will generate interactions with normal lights and receives lighting.
	bool				 ReceivesLighting() const { return ( numAmbientStages != numStages ) && ( materialFlags & MF_UNLIT ) == 0; }

	//! Returns true if the material should generate lighting interactions on sides facing away from light centers.
	bool				 ReceivesLightingOnBackSides() const { return ( materialFlags & ( MF_NOSELFSHADOW | MF_NOSHADOWS ) ) != 0; }

	//! Returns true if back sides should be created for this material, typically for alpha tested surfaces with two-sided lighting.
	bool				 ShouldCreateBackSides() const { return shouldCreateBackSides; }

	//! Returns whether the material uses unsmoothed tangents for tangent and normal vector generation.
	bool				 UseUnsmoothedTangents() const { return unsmoothedTangents; }

	//! Returns whether the material uses the Mikkelsen tangent space standard for normal map calculations.
	bool				 UseMikkTSpace() const { return mikktspace; }

	//! Returns whether the material allows overlays to be placed on it
	bool				 AllowOverlays() const { return allowOverlays; }

	//! Returns the coverage type of the material, which determines how it interacts with the depth buffer and rendering pipeline.
	materialCoverage_t	 Coverage() const { return coverage; }

	//! Returns true if this material takes precedence over the other material in coplanar cases based on drawing status and coverage.
	bool				 HasHigherRogmapPriority( const idMaterial& other ) const { return ( IsDrawn() && !other.IsDrawn() ) || ( Coverage() < other.Coverage() ); }

	//! Returns the global user interface associated with this material, or NULL if no global GUI is defined.
	idUserInterface*	 GlobalGui() const { return gui; }

	//! Determines whether the material represents a discrete surface that should not be merged with others during rendering.
	bool				 IsDiscrete() const { return ( entityGui || gui || deform != DFRM_NONE || sort == SS_SUBVIEW || ( surfaceFlags & SURF_DISCRETE ) != 0 ); }

	//! Returns true if the material has the no fragment flag set, indicating that the surface should not be split by BSP boundaries during dmap processing.
	bool				 NoFragment() const { return ( surfaceFlags & SURF_NOFRAGMENT ) != 0; }

	//! Returns true if the material is an occluder surface that renders to the masked occlusion depth buffer
	bool				 IsOccluder() const { return ( surfaceFlags & SURF_OCCLUSION ) != 0; }

	//! Returns true if the material is configured as a fog light shader.
	bool				 IsFogLight() const { return fogLight; }

	//! Returns true if the material uses blend lighting instead of normal light interaction.
	bool				 IsBlendLight() const { return blendLight; }

	//! Returns true if the material is an ambient light.
	bool				 IsAmbientLight() const { return ambientLight; }

	//! Determines whether the material casts shadows based on its flags and lighting properties.
	bool				 LightCastsShadows() const { return TestMaterialFlag( MF_FORCESHADOWS ) || ( !fogLight && !ambientLight && !blendLight && !TestMaterialFlag( MF_NOSHADOWS ) ); }

	//! Returns true if the material's lighting effects should be applied to back faces of geometry.
	bool				 LightEffectsBackSides() const { return fogLight || ambientLight || blendLight; }

	//! Returns the light falloff image associated with this material, or NULL if not explicitly specified.
	idImage*			 LightFalloffImage() const { return lightFalloffImage; }

	//------------------------------------------------------------------

	// returns the renderbump command line for this shader, or an empty string if not present
	const char*			 GetRenderBump() const { return renderBump; };

	//! Sets the specified material flag(s) on the material.
	void				 SetMaterialFlag( const int flag ) const { materialFlags |= flag; }

	//! Clears the specified material flag(s) from the material.
	void				 ClearMaterialFlag( const int flag ) const { materialFlags &= ~flag; }

	//! Tests whether a specific material flag is set
	bool				 TestMaterialFlag( const int flag ) const { return ( materialFlags & flag ) != 0; }

	//! Retrieves the content flags associated with the material.
	const int			 GetContentFlags() const { return contentFlags; }

	//! Returns the surface flags of the material
	const int			 GetSurfaceFlags() const { return surfaceFlags; }

	//! Returns the surface type constant for this material.
	const surfTypes_t	 GetSurfaceType() const { return static_cast<surfTypes_t>( surfaceFlags & SURF_TYPE_MASK ); }

	//! Returns the material description string.
	const char*			 GetDescription() const { return desc; }

	//! Returns the sort order value for the material.
	const float			 GetSort() const { return sort; }

	//! Returns the stereo eye value for the material.
	const int			 GetStereoEye() const { return stereoEye; }

	// this is only used by the gui system to force sorting order
	// on images referenced from tga's instead of materials.
	// this is done this way as there are 2000 tgas the guis use
	void				 SetSort( float s ) const { sort = s; };

	//! Returns the deformation type of the material.
	deform_t			 Deform() const { return deform; }

	//! Returns the deform register value at the specified index.
	const int			 GetDeformRegister( int index ) const { return deformRegisters[index]; }

	//! Returns the deformation declaration associated with this material.
	const idDecl*		 GetDeformDecl() const { return deformDecl; }

	//! Returns the texture generation type used by the material's stages.
	texgen_t			 Texgen() const;

	//! Returns a pointer to the texture generation registers used for wobble sky parameters.
	const int*			 GetTexGenRegisters() const { return texGenRegisters; }

	//! Returns the cull type of the material.
	const cullType_t	 GetCullType() const { return cullType; }

	//! Returns the editor alpha value for the material.
	float				 GetEditorAlpha() const { return editorAlpha; }

	//! Returns the GUI entity ID associated with this material.
	int					 GetEntityGui() const { return entityGui; }

	//! Returns the decal information associated with this material.
	decalInfo_t			 GetDecalInfo() const { return decalInfo; }

	//! Returns the spectrum value used for invisible writing illumination matching.
	int					 Spectrum() const { return spectrum; }

	//! Returns the polygon offset value for the material.
	float				 GetPolygonOffset() const { return polygonOffset; }

	//! Returns the surface area of the material
	float				 GetSurfaceArea() const { return surfaceArea; }

	//! Adds the specified area value to the material's surface area.
	void				 AddToSurfaceArea( float area ) { surfaceArea += area; }

	//! Returns the length in milliseconds of the cinematic animation on this material, or zero if it doesn't have one.
	int					 CinematicLength() const;

	//! Closes and cleans up any cinematic textures associated with the material stages.
	void				 CloseCinematic() const;

	//! Resets the cinematic time for all stages that have cinematic textures.
	void				 ResetCinematicTime( int time ) const;

	//! Returns the start time of the cinematic associated with the material
	int					 GetCinematicStartTime() const;

	//! Updates the cinematic state of the material based on the provided time value.
	void				 UpdateCinematic( int time ) const;

	//! Checks if the cinematic playback is currently active for this material.
	bool				 CinematicIsPlaying() const;

	//! Returns the editor image for this material, potentially deriving it from stage textures if no explicit editor image is defined.
	idImage*			 GetEditorImage() const;

	//! Returns the first available image from the material's stages for use in the light editor, or the default editor image if none are found.
	idImage*			 GetLightEditorImage() const;

	//! Returns the width of the image associated with the first stage of this material
	int					 GetImageWidth() const;

	//! Returns the height of the image associated with the first stage of this material
	int					 GetImageHeight() const;

	//! Sets the GUI for this material using the provided GUI name.
	void				 SetGui( const char* _gui ) const;

	//! Returns the number of registers this material contains
	const int			 GetNumRegisters() const { return numRegisters; }

	/*!
		\brief Evaluates shader expression registers using local and global parameters, time, and sound emitter data.

		This function computes the final values of shader expression registers by first copying predefined material constants, then applying local and global shader parameters, and finally executing a
	   series of operations defined in the material's expression bytecode. The operations include arithmetic, comparison, modulo, table lookups, and sound amplitude evaluation. The floatTime parameter
	   allows entities running in parallel to be in different time groups.

		\param registers Output array where computed register values will be stored
		\param localShaderParms Array of local shader parameters for the entity
		\param globalShaderParms Array of global shader parameters for the current frame
		\param floatTime Current time value used for time-based expressions
		\param soundEmitter Pointer to the sound emitter for sound-based register evaluation
		\throws FatalError if an invalid opcode is encountered during expression evaluation
	*/
	void				 EvaluateRegisters(
						float* registers, const float localShaderParms[MAX_ENTITY_SHADER_PARMS], const float globalShaderParms[MAX_GLOBAL_SHADER_PARMS], const float floatTime, idSoundEmitter* soundEmitter ) const;

	// if a material only uses constants (no entityParm or globalparm references), this
	// will return a pointer to an internal table, and EvaluateRegisters will not need
	// to be called.  If NULL is returned, EvaluateRegisters must be used.
	const float* ConstantRegisters() const { return constantRegisters; };

	bool		 SuppressInSubview() const { return suppressInSubview; };
	bool		 IsPortalSky() const { return portalSky; };

	//! Increments the reference count of the material and its associated textures.
	void		 AddReference();

	//! Determines if the material is a level of detail (LOD) surface.
	bool		 IsLOD() const { return ( materialFlags & ( MF_LOD1 | MF_LOD2 | MF_LOD3 | MF_LOD4 ) ) != 0; }

	//! Determines if a material's level of detail is visible based on distance and LOD settings.
	bool		 IsLODVisibleForDistance( float distance, float lodBase ) const
	{
		int	  bit = ( materialFlags & ( MF_LOD1 | MF_LOD2 | MF_LOD3 | MF_LOD4 ) ) >> MF_LOD1_SHIFT;
		float m1  = lodBase * ( bit >> 1 );
		float m2  = lodBase * bit;
		return distance >= m1 && ( distance < m2 || ( materialFlags & ( MF_LOD_PERSISTENT ) ) );
	}

	//! Exports material data to a JSON file for use in Blender.
	void ExportJSON( idFile* file, bool lastEntry ) const;

private:
	//! Initializes all member variables of the material to their default values.
	void	 CommonInit();

	//! Parses a material definition from a lexer token stream
	void	 ParseMaterial( idLexer& src );

	//! Checks if the next token in the lexer matches the specified string, setting a default flag and returning false if it does not match.
	bool	 MatchToken( idLexer& src, const char* match );

	//! Parses the sort parameter for the material from the provided lexer token stream.
	void	 ParseSort( idLexer& src );

	//! Parses the stereo eye parameter from the lexer input.
	void	 ParseStereoEye( idLexer& src );

	//! Parses blend mode settings from a lexer token and configures the shader stage draw state bits accordingly.
	void	 ParseBlend( idLexer& src, shaderStage_t* stage );

	//! Parses a vertex parameter for a shader stage from the given lexer input.
	void	 ParseVertexParm( idLexer& src, newShaderStage_t* newStage );

	//! Parses a vertex parameter configuration from the lexer input and stores it in the shader stage.
	void	 ParseVertexParm2( idLexer& src, newShaderStage_t* newStage );

	//! Parses a fragment map definition from the source lexer and sets up the corresponding image parameters in the shader stage.
	void	 ParseFragmentMap( idLexer& src, newShaderStage_t* newStage );

	//! Parses a stencil comparison function token and sets the corresponding stencil comparison value.
	void	 ParseStencilCompare( const idToken& token, stencilComp_t* stencilComp );

	//! Parses a stencil operation token and sets the corresponding stencil operation value.
	void	 ParseStencilOperation( const idToken& token, stencilOperation_t* stencilOp );

	//! Parses stencil configuration parameters from a lexer token stream into a stencil stage structure.
	void	 ParseStencil( idLexer& src, stencilStage_t* stencilStage );

	//! Parses a single stage definition from the material definition lexer input
	void	 ParseStage( idLexer& src, const textureRepeat_t trpDefault = TR_REPEAT );

	//! Parses a deform type from a lexer token and sets the corresponding deform parameters.
	void	 ParseDeform( idLexer& src );

	//! Parses decal information from a lexer including stay time, fade time, and start/end color values.
	void	 ParseDecalInfo( idLexer& src );

	//! Checks if the provided token matches any surface parameter bit flags and updates the material's surface and content flags accordingly.
	bool	 CheckSurfaceParm( idToken* token );

	//! Returns the index of a material expression register that contains the specified float value, or creates a new one if it doesn't exist.
	int		 GetExpressionConstant( float f );

	//! Returns a temporary register index for expression evaluation in the material.
	int		 GetExpressionTemporary();

	//! Returns a pointer to the next available expression operation slot in the material's shader operations array.
	expOp_t* GetExpressionOp();

	//! Emit a bytecode operation for the material expression parser.
	int		 EmitOp( int a, int b, expOpType_t opType );

	/*!
		\brief Parses an expression operation and emits the corresponding opcode.

		This function parses the next operand in the expression using the specified priority level, then emits an operation opcode combining the current operand 'a' with the parsed operand 'b'. It is
	   used during material expression parsing to handle binary operations with proper operator precedence.

		\param src Lexer object used to parse the input source for the expression
		\param a The left operand of the operation
		\param opType The type of operation to emit
		\param priority The priority level used to determine which operands to parse
		\return The result of emitting the operation opcode combining operands a and b
	*/
	int		 ParseEmitOp( idLexer& src, int a, expOpType_t opType, int priority );

	//! Parses a term from the lexer and returns a register index or expression constant
	int		 ParseTerm( idLexer& src );

	//! Parses a mathematical expression with specified priority level and returns the register index of the result
	int		 ParseExpressionPriority( idLexer& src, int priority );

	//! Parses a mathematical expression from the lexer and returns a register index.
	int		 ParseExpression( idLexer& src );

	//! Clears the shader stage by resetting its draw state bits and color registers.
	void	 ClearStage( shaderStage_t* ss );

	//! Converts a string representation of a source blend mode to its corresponding OpenGL blend mode constant.
	int		 NameToSrcBlendMode( const idStr& name );

	//! Converts a blend mode name string to its corresponding destination blend mode constant
	int		 NameToDstBlendMode( const idStr& name );

	//! Multiplies the texture matrix of a texture stage with the provided registers
	void	 MultiplyTextureMatrix( textureStage_t* ts, int registers[2][3] );

	//! Sorts interaction stages in the material, ensuring proper ordering of lighting stages.
	void	 SortInteractionStages();

	//! Adds implicit shader stages for bump, diffuse, and specular lighting when they are missing from the material.
	void	 AddImplicitStages( const textureRepeat_t trpDefault = TR_REPEAT );

	//! Checks if material registers are constant and evaluates them once for optimization
	void	 CheckForConstantRegisters();

	//! Initializes fast path image references for materials that qualify for optimized rendering.
	void	 SetFastPathImages();

private:
	idStr					 desc;		 // description
	idStr					 renderBump; // renderbump command options, without the "renderbump" at the start

	idImage*				 lightFalloffImage; // only for light shaders

	idImage*				 fastPathBumpImage; // if any of these are set, they all will be
	idImage*				 fastPathDiffuseImage;
	idImage*				 fastPathSpecularImage;

	int						 entityGui; // draw a gui with the idUserInterface from the renderEntity_t
	// non zero will draw gui, gui2, or gui3 from renderEnitty_t
	mutable idUserInterface* gui; // non-custom guis are shared by all users of a material

	bool					 noFog; // surface does not create fog interactions

	int						 spectrum; // for invisible writing, used for both lights and surfaces

	float					 polygonOffset;

	int						 contentFlags;	// content flags
	int						 surfaceFlags;	// surface flags
	mutable int				 materialFlags; // material flags

	decalInfo_t				 decalInfo;

	mutable float			 sort; // lower numbered shaders draw before higher numbered
	int						 stereoEye;
	deform_t				 deform;
	int						 deformRegisters[4]; // numeric parameter for deforms
	const idDecl*			 deformDecl;		 // for surface emitted particle deforms and tables

	int						 texGenRegisters[MAX_TEXGEN_REGISTERS]; // for wobbleSky

	materialCoverage_t		 coverage;
	cullType_t				 cullType;	  // CT_FRONT_SIDED, CT_BACK_SIDED, or CT_TWO_SIDED
	SubViewType				 subViewType; // SP added
	bool					 shouldCreateBackSides;

	bool					 fogLight;
	bool					 blendLight;
	bool					 ambientLight;
	bool					 unsmoothedTangents;
	bool					 mikktspace; // RB: use Mikkelsen tangent space standard for normal mapping
	bool					 hasSubview; // mirror, remote render, etc
	bool					 allowOverlays;

	int						 numOps;
	expOp_t*				 ops; // evaluate to make expressionRegisters

	int						 numRegisters; //
	float*					 expressionRegisters;

	float*					 constantRegisters; // NULL if ops ever reference globalParms or entityParms

	int						 numStages;
	int						 numAmbientStages;

	shaderStage_t*			 stages;

	struct mtrParsingData_s* pd; // only used during parsing

	float					 surfaceArea; // only for listSurfaceAreas

	// we defer loading of the editor image until it is asked for, so the game doesn't load up
	// all the invisible and uncompressed images.
	// If editorImage is NULL, it will atempt to load editorImageName, and set editorImage to that or defaultImage
	idStr					 editorImageName;
	mutable idImage*		 editorImage; // image used for non-shaded preview
	float					 editorAlpha;

	bool					 suppressInSubview;
	bool					 portalSky;
	int						 refCount;
};

typedef idList<const idMaterial*, TAG_MATERIAL> idMatList;

#endif /* !__MATERIAL_H__ */
