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

#ifndef __DECLPARTICLE_H__
#define __DECLPARTICLE_H__

/*
===============================================================================

	idDeclParticle

===============================================================================
*/

static const int MAX_PARTICLE_STAGES = 32;

/*!
	\class idParticleParm
	\brief A class representing particle parameters that can be evaluated and integrated over time.
*/
class idParticleParm
{
public:
	//! Initializes a particle parameter with default values.
	idParticleParm()
	{
		table = NULL;
		from = to = 0.0f;
	}

	const idDeclTable* table;
	float			   from;
	float			   to;

	//! Evaluates a particle parameter value based on fraction and random number generator.
	float			   Eval( float frac, idRandom& rand ) const;

	//! Computes an integrated value for a particle parameter based on fraction and random input
	float			   Integrate( float frac, idRandom& rand ) const;
};

typedef enum {
	PDIST_RECT,		// ( sizeX sizeY sizeZ )
	PDIST_CYLINDER, // ( sizeX sizeY sizeZ )
	PDIST_SPHERE	// ( sizeX sizeY sizeZ ringFraction )
					// a ringFraction of zero allows the entire sphere, 0.9 would only
					// allow the outer 10% of the sphere
} prtDistribution_t;

typedef enum {
	PDIR_CONE,	 // parm0 is the solid cone angle
	PDIR_OUTWARD // direction is relative to offset from origin, parm0 is an upward bias
} prtDirection_t;

typedef enum {
	PPATH_STANDARD,
	PPATH_HELIX, // ( sizeX sizeY sizeZ radialSpeed climbSpeed )
	PPATH_FLIES,
	PPATH_ORBIT,
	PPATH_DRIP
} prtCustomPth_t;

typedef enum {
	POR_VIEW,
	POR_AIMED, // angle and aspect are disregarded
	POR_X,
	POR_Y,
	POR_Z
} prtOrientation_t;

typedef struct renderEntity_s renderEntity_t;
typedef struct renderView_s	  renderView_t;

typedef struct {
	const renderEntity_t* renderEnt; // for shaderParms, etc
	const renderView_t*	  renderView;
	int					  index; // particle number in the system
	float				  frac;	 // 0.0 to 1.0
	idRandom			  random;
	idVec3				  origin; // dynamic smoke particles can have individual origins and axis
	idMat3				  axis;

	float				  age;				  // in seconds, calculated as fraction * stage->particleLife
	idRandom			  originalRandom;	  // needed so aimed particles can reset the random for another origin calculation
	float				  animationFrameFrac; // set by ParticleTexCoords, used to make the cross faded version
} particleGen_t;

/*!
	\class idParticleStage
	\brief Manages individual particle generation stages with configurable properties and rendering behavior.

	Handles the configuration and execution of particle generation stages, including vertex creation, positioning, texturing, and coloring. Supports custom path types and parameter management for
   flexible particle system control. Each stage defines how particles are generated and rendered, including properties like quads per particle, vertex count, and visual appearance. The class provides
   methods for initializing default settings, calculating particle positions and attributes, and managing custom path configurations. It acts as a building block for complex particle effects by
   defining individual stage behaviors within a larger particle system.

*/
class idParticleStage
{
public:
	//! Initializes all member variables of the idParticleStage class to their default values.
	idParticleStage();
	~idParticleStage() { }

	//! Initializes the particle stage with default configuration values.
	void			  Default();

	//! Returns the number of quads used per particle, including trails and cross-faded animations.
	int				  NumQuadsPerParticle() const;

	//! Creates a particle by generating vertex data for rendering based on particle generation parameters and returns the number of vertices created
	int				  CreateParticle( particleGen_t* g, idDrawVert* verts ) const;

	//! Calculates the origin position for a particle based on distribution and path type.
	void			  ParticleOrigin( particleGen_t* g, idVec3& origin ) const;

	//! Computes vertex positions for a particle based on its stage properties and origin.
	int				  ParticleVerts( particleGen_t* g, const idVec3 origin, idDrawVert* verts ) const;

	//! Calculates and sets texture coordinates for particle vertices based on animation frame and particle generation data.
	void			  ParticleTexCoords( particleGen_t* g, idDrawVert* verts ) const;

	//! Sets the color values for particle vertices based on generation parameters and fading effects.
	void			  ParticleColors( particleGen_t* g, idDrawVert* verts ) const;

	//! Returns the name of the custom particle path type.
	const char*		  GetCustomPathName();

	//! Returns the description string for the custom particle path type.
	const char*		  GetCustomPathDesc();

	//! Returns the number of custom path parameters for the particle stage.
	int				  NumCustomPathParms();

	//! Sets the custom path type for a particle stage based on the provided string identifier.
	void			  SetCustomPathType( const char* p );

	//! Assigns all member variables from another idParticleStage object to this object.
	void			  operator=( const idParticleStage& src );

	//------------------------------

	const idMaterial* material;

	int				  totalParticles; // total number of particles, although some may be invisible at a given time
	float			  cycles;		  // allows things to oneShot ( 1 cycle ) or run for a set number of cycles
	// on a per stage basis

	int				  cycleMsec; // ( particleLife + deadTime ) in msec

	float			  spawnBunching; // 0.0 = all come out at first instant, 1.0 = evenly spaced over cycle time
	float			  particleLife;	 // total seconds of life for each particle
	float			  timeOffset;	 // time offset from system start for the first particle to spawn
	float			  deadTime;		 // time after particleLife before respawning

	//-------------------------------	// standard path parms

	prtDistribution_t distributionType;
	float			  distributionParms[4];

	prtDirection_t	  directionType;
	float			  directionParms[4];

	idParticleParm	  speed;
	float			  gravity;			  // can be negative to float up
	bool			  worldGravity;		  // apply gravity in world space
	bool			  randomDistribution; // randomly orient the quad on emission ( defaults to true )
	bool			  entityColor;		  // force color from render entity ( fadeColor is still valid )

	//------------------------------	// custom path will completely replace the standard path calculations

	prtCustomPth_t	  customPathType; // use custom C code routines for determining the origin
	float			  customPathParms[8];

	//--------------------------------

	idVec3			  offset; // offset from origin to spawn all particles, also applies to customPath

	int				  animationFrames; // if > 1, subdivide the texture S axis into frames and crossfade
	float			  animationRate;   // frames per second

	float			  initialAngle;	 // in degrees, random angle is used if zero ( default )
	idParticleParm	  rotationSpeed; // half the particles will have negative rotation speeds

	prtOrientation_t  orientation; // view, aimed, or axis fixed
	float			  orientationParms[4];

	idParticleParm	  size;
	idParticleParm	  aspect; // greater than 1 makes the T axis longer

	idVec4			  color;
	idVec4			  fadeColor;		 // either 0 0 0 0 for additive, or 1 1 1 0 for blended materials
	float			  fadeInFraction;	 // in 0.0 to 1.0 range
	float			  fadeOutFraction;	 // in 0.0 to 1.0 range
	float			  fadeIndexFraction; // in 0.0 to 1.0 range, causes later index smokes to be more faded

	bool			  hidden; // for editor use
	//-----------------------------------

	float			  boundsExpansion; // user tweak to fix poorly calculated bounds

	idBounds		  bounds; // derived
};

/*!
	\class idDeclParticle
	\brief Manages particle system declarations including parsing, loading, saving, and managing particle stages.

	This class represents a particle system declaration that contains multiple particle stages. It handles the parsing of particle definitions from text, loading and saving operations from binary
   files, and manages the lifecycle of particle stages. The class inherits from idDecl and extends its functionality to specifically handle particle system data. It provides methods for parsing
   individual particle stages, managing parameter lists, and writing particle data to files. The class supports both text and binary format operations, including checksum validation during loading.
   Memory management is handled through the FreeData method which cleans up particle stage resources, and the class maintains a collection of particle stages that can be manipulated and queried for
   bounding volumes.

*/
class idDeclParticle : public idDecl
{
public:
	//! Returns the size in bytes of the idDeclParticle object
	virtual size_t								  Size() const;

	//! Returns the default definition string for particle declarations.
	virtual const char*							  DefaultDefinition() const;

	//! Parses particle declaration data from text, optionally handling binary version loading and generation.
	virtual bool								  Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees the particle system data by deleting all stages.
	virtual void								  FreeData();

	//! Saves the particle declaration to a file.
	bool										  Save( const char* fileName = NULL );

	//! Loads particle data from a binary file and validates its checksum.
	bool										  LoadBinary( idFile* file, unsigned int checksum );

	//! Writes the particle declaration data to a binary file with the specified checksum
	void										  WriteBinary( idFile* file, unsigned int checksum );

	idList<idParticleStage*, TAG_IDLIB_LIST_DECL> stages;
	idBounds									  bounds;
	float										  depthHack;

private:
	//! Rebuilds the text source for a particle declaration.
	bool			 RebuildTextSource();

	//! Calculates and sets the bounding volume for a particle stage based on particle origin and size evaluations.
	void			 GetStageBounds( idParticleStage* stage );

	//! Parses a particle stage definition from a token stream and returns a new particle stage object.
	idParticleStage* ParseParticleStage( idLexer& src );

	//! Parses a variable length list of parameters from a lexer into a float array.
	void			 ParseParms( idLexer& src, float* parms, int maxParms );

	//! Parses a parametric parameter for a particle effect from the given lexer input.
	void			 ParseParametric( idLexer& src, idParticleParm* parm );

	//! Writes a particle stage definition to a file
	void			 WriteStage( idFile* f, idParticleStage* stage );

	//! Writes particle parameter data to a file with specified name and formatting
	void			 WriteParticleParm( idFile* f, idParticleParm* parm, const char* name );
};

#endif /* !__DECLPARTICLE_H__ */
