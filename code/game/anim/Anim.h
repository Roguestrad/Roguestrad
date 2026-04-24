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
#ifndef __ANIM_H__
#define __ANIM_H__

//
// animation channels
// these can be changed by modmakers and licensees to be whatever they need.
const int	  ANIM_NumAnimChannels	  = 5;
const int	  ANIM_MaxAnimsPerChannel = 3;
const int	  ANIM_MaxSyncedAnims	  = 3;

//
// animation channels.  make sure to change script/doom_defs.script if you add any channels, or change their order
//
const int	  ANIMCHANNEL_ALL	  = 0;
const int	  ANIMCHANNEL_TORSO	  = 1;
const int	  ANIMCHANNEL_LEGS	  = 2;
const int	  ANIMCHANNEL_HEAD	  = 3;
const int	  ANIMCHANNEL_EYELIDS = 4;

//! Converts a frame number from 24 frames per second to milliseconds.
ID_INLINE int FRAME2MS( int framenum )
{
	return ( framenum * 1000 ) / 24;
}

class idRenderModel;
class idAnimator;
class idAnimBlend;
class function_t;
class idEntity;
class idSaveGame;
class idRestoreGame;

typedef struct {
	int	  cycleCount; // how many times the anim has wrapped to the begining (0 for clamped anims)
	int	  frame1;
	int	  frame2;
	float frontlerp;
	float backlerp;
} frameBlend_t;

typedef struct {
	int nameIndex;
	int parentNum;
	int animBits;
	int firstComponent;
} jointAnimInfo_t;

typedef struct {
	jointHandle_t num;
	jointHandle_t parentNum;
	int			  channel;
} jointInfo_t;

//
// joint modifier modes.  make sure to change script/doom_defs.script if you add any, or change their order.
//
typedef enum {
	JOINTMOD_NONE,			 // no modification
	JOINTMOD_LOCAL,			 // modifies the joint's position or orientation in joint local space
	JOINTMOD_LOCAL_OVERRIDE, // sets the joint's position or orientation in joint local space
	JOINTMOD_WORLD,			 // modifies joint's position or orientation in model space
	JOINTMOD_WORLD_OVERRIDE	 // sets the joint's position or orientation in model space
} jointModTransform_t;

typedef struct {
	jointHandle_t		jointnum;
	idMat3				mat;
	idVec3				pos;
	jointModTransform_t transform_pos;
	jointModTransform_t transform_axis;
} jointMod_t;

#define ANIM_BIT_TX 0
#define ANIM_BIT_TY 1
#define ANIM_BIT_TZ 2
#define ANIM_BIT_QX 3
#define ANIM_BIT_QY 4
#define ANIM_BIT_QZ 5

#define ANIM_TX		BIT( ANIM_BIT_TX )
#define ANIM_TY		BIT( ANIM_BIT_TY )
#define ANIM_TZ		BIT( ANIM_BIT_TZ )
#define ANIM_QX		BIT( ANIM_BIT_QX )
#define ANIM_QY		BIT( ANIM_BIT_QY )
#define ANIM_QZ		BIT( ANIM_BIT_QZ )

typedef enum {
	FC_SCRIPTFUNCTION,
	FC_SCRIPTFUNCTIONOBJECT,
	FC_EVENTFUNCTION,
	FC_SOUND,
	FC_SOUND_VOICE,
	FC_SOUND_VOICE2,
	FC_SOUND_BODY,
	FC_SOUND_BODY2,
	FC_SOUND_BODY3,
	FC_SOUND_WEAPON,
	FC_SOUND_ITEM,
	FC_SOUND_GLOBAL,
	FC_SOUND_CHATTER,
	FC_SKIN,
	FC_TRIGGER,
	FC_TRIGGER_SMOKE_PARTICLE,
	FC_MELEE,
	FC_DIRECTDAMAGE,
	FC_BEGINATTACK,
	FC_ENDATTACK,
	FC_MUZZLEFLASH,
	FC_CREATEMISSILE,
	FC_LAUNCHMISSILE,
	FC_FIREMISSILEATTARGET,
	FC_FOOTSTEP,
	FC_LEFTFOOT,
	FC_RIGHTFOOT,
	FC_ENABLE_EYE_FOCUS,
	FC_DISABLE_EYE_FOCUS,
	FC_FX,
	FC_DISABLE_GRAVITY,
	FC_ENABLE_GRAVITY,
	FC_JUMP,
	FC_ENABLE_CLIP,
	FC_DISABLE_CLIP,
	FC_ENABLE_WALK_IK,
	FC_DISABLE_WALK_IK,
	FC_ENABLE_LEG_IK,
	FC_DISABLE_LEG_IK,
	FC_RECORDDEMO,
	FC_AVIGAME,
	FC_LAUNCH_PROJECTILE,
	FC_TRIGGER_FX,
	FC_START_EMITTER,
	FC_STOP_EMITTER,
} frameCommandType_t;

typedef struct {
	int num;
	int firstCommand;
} frameLookup_t;

typedef struct {
	frameCommandType_t type;
	idStr*			   string;

	union {
		const idSoundShader* soundShader;
		const function_t*	 function;
		const idDeclSkin*	 skin;
		int					 index;
	};
} frameCommand_t;

typedef struct {
	bool prevent_idle_override : 1;
	bool random_cycle_start	   : 1;
	bool ai_no_turn			   : 1;
	bool anim_turn			   : 1;
} animFlags_t;

/*!
	\class idMD5Anim
	\brief Provides management and playback functionality for MD5 animation data.

	The idMD5Anim class encapsulates MD5 animation data and provides methods for loading, reloading, and managing animation resources. It supports both binary and ASCII file formats for animation data
   loading, and includes functionality for frame interpolation, joint transformation decoding, and animation playback calculations. The class maintains reference counts for resource management and
   includes validation methods to ensure consistency between animation data and associated model hierarchies. It offers methods to retrieve specific animation frames, calculate movement deltas, origin
   offsets, and bounding volumes at different animation times, making it suitable for real-time animation playback and manipulation within a graphics system.

*/
class idMD5Anim
{
private:
	int									  numFrames;
	int									  frameRate;
	int									  animLength;
	int									  numJoints;
	int									  numAnimatedComponents;
	idList<idBounds, TAG_MD5_ANIM>		  bounds;
	idList<jointAnimInfo_t, TAG_MD5_ANIM> jointInfo;
	idList<idJointQuat, TAG_MD5_ANIM>	  baseFrame;
	idList<float, TAG_MD5_ANIM>			  componentFrames;
	idStr								  name;
	idVec3								  totaldelta;
	mutable int							  ref_count;
	// RB
	idImportOptions						  importOptions;

public:
	//! Initializes a new instance of the idMD5Anim class with default values.
	idMD5Anim();

	//! Destroys the idMD5Anim object and frees its resources.
	~idMD5Anim();

	//! Initializes the MD5 animation data by resetting all internal state and clearing associated memory.
	void		  Free();

	//! Reloads the MD5 animation by freeing the current instance and loading the animation from its original file.
	bool		  Reload();

	//! Returns the total allocated memory size for the MD5 animation data.
	size_t		  Allocated() const;
	size_t		  Size() const { return sizeof( *this ) + Allocated(); };

	//! Loads an MD5 animation from a file, either from binary format or by parsing an ASCII definition file.
	bool		  LoadAnim( const char* filename, const idImportOptions* options );

	//! Loads animation data from a binary file for MD5 animation.
	bool		  LoadBinary( idFile* file, ID_TIME_T sourceTimeStamp );

	//! Writes the animation data to a binary file with the specified timestamp.
	void		  WriteBinary( idFile* file, ID_TIME_T sourceTimeStamp );

	//! Increments the reference count of the MD5 animation object.
	void		  IncreaseRefs() const;

	//! Decrements the reference count of the MD5 animation.
	void		  DecreaseRefs() const;

	//! Returns the number of references to this MD5 animation object.
	int			  NumRefs() const;

	//! Validates that the joint hierarchy of the animation matches the joint hierarchy of the provided model.
	void		  CheckModelHierarchy( const idRenderModel* model ) const;

	/*!
		\brief Computes and returns an interpolated animation frame for a model based on the provided frame blend information and joint data

		The function begins by copying the base frame data into the output joints array. If there are no animated components, it simply returns the base frame. Otherwise, it allocates temporary memory
	   for blending joints and lerp indices. It then retrieves the animation data for the two frames being interpolated and decodes the interpolated frames. The joints are then blended using SIMD
	   processing, and if there is a cycle count, it adjusts the translation of the first joint accordingly

		\param frame The frame blend information containing frame indices and blending factors
		\param joints Output array of joint quaternions representing the interpolated frame
		\param index Array of joint indices to interpolate
		\param numIndexes Number of joint indices to process
	*/
	void		  GetInterpolatedFrame( frameBlend_t& frame, idJointQuat* joints, const int* index, int numIndexes ) const;

	/*!
		\brief Retrieves a specific animation frame by decoding animation data into joint transformations.

		This function retrieves a single animation frame from the MD5 animation by first copying the base frame data and then applying animated components if present. It allows for selective decoding
	   of joint transformations based on a provided index array. The function handles the case where no animation is present by returning the base frame directly.

		\param framenum The frame number to retrieve from the animation
		\param joints Output array to store the decoded joint transformations
		\param index Array of joint indices to decode, or NULL to decode all joints
		\param numIndexes Number of joints to decode, or zero to decode all joints
	*/
	void		  GetSingleFrame( int framenum, idJointQuat* joints, const int* index, int numIndexes ) const;

	//! Returns the length of the animation in this MD5 animation object.
	int			  Length() const;

	//! Returns the number of frames in the MD5 animation.
	int			  NumFrames() const;

	//! Returns the number of joints in the MD5 animation
	int			  NumJoints() const;

	//! Returns the total movement delta of the MD5 animation.
	const idVec3& TotalMovementDelta() const;

	//! Returns the name of the MD5 animation.
	const char*	  Name() const;

	//! Retrieves the frame blend information for a specified frame number.
	void		  GetFrameBlend( int framenum, frameBlend_t& frame ) const;

	//! Converts a given time value into frame indices and blending factors for animation playback.
	void		  ConvertTimeToFrame( int time, int cyclecount, frameBlend_t& frame ) const;

	//! Calculates the origin offset for a given animation time and cycle count.
	void		  GetOrigin( idVec3& offset, int currentTime, int cyclecount ) const;

	//! Retrieves the rotation of the origin joint at a specific time and cycle count
	void		  GetOriginRotation( idQuat& rotation, int time, int cyclecount ) const;

	//! Calculates and returns the bounding box for the animation at a specific time.
	void		  GetBounds( idBounds& bounds, int currentTime, int cyclecount ) const;
};

/*!
	\class idAnim
	\brief Manages animation data including MD5 animations, frame commands, and animation flags for model definitions.

	This class encapsulates animation data for model definitions, supporting multiple synchronized animations and frame-based commands. It provides methods to initialize animations with MD5 data,
   retrieve animation properties such as length, frame count, and movement deltas, and handle origin offsets and rotations for animation positioning. The class also supports frame command management,
   allowing execution of commands at specific animation frames, and maintains animation flags for various animation behaviors. Animation data is associated with a model definition and can be copied or
   destroyed as needed.

*/
class idAnim
{
private:
	const class idDeclModelDef*		 modelDef;
	const idMD5Anim*				 anims[ANIM_MaxSyncedAnims];
	int								 numAnims;
	idStr							 name;
	idStr							 realname;
	idList<frameLookup_t, TAG_ANIM>	 frameLookup;
	idList<frameCommand_t, TAG_ANIM> frameCommands;
	animFlags_t						 flags;

public:
	//! Constructs an idAnim object with default initial state.
	idAnim();

	//! Constructs a new animation object as a copy of an existing animation object.
	idAnim( const idDeclModelDef* modelDef, const idAnim* anim );

	//! Destroys an idAnim object and cleans up its associated animation data.
	~idAnim();

	/*!
		\brief Initializes an animation with the specified model definition, source name, animation name, and array of MD5 animations.

		This function sets up the animation data by assigning the provided model definition and animation names. It clears any existing animation references, validates the number of animations, and
	   copies the provided MD5 animation references. The function also resets animation flags and clears frame-related data structures.

		\param modelDef The model definition this animation belongs to
		\param sourcename The source name of the animation
		\param animname The name of the animation
		\param num The number of animations to set, must be between 1 and ANIM_MaxSyncedAnims
		\param md5anims Array of MD5 animation pointers to be associated with this animation
		\throws assertion failure if num is not within the valid range of 1 to ANIM_MaxSyncedAnims
	*/
	void				  SetAnim( const idDeclModelDef* modelDef, const char* sourcename, const char* animname, int num, const idMD5Anim* md5anims[ANIM_MaxSyncedAnims] );

	//! Returns the name of the animation.
	const char*			  Name() const;

	//! Returns the full name of the animation as a null-terminated string.
	const char*			  FullName() const;

	//! Returns the MD5 animation at the specified index, or NULL if the index is out of bounds.
	const idMD5Anim*	  MD5Anim( int num ) const;

	//! Returns the model definition associated with this animation.
	const idDeclModelDef* ModelDef() const;

	//! Returns the length of the animation data
	int					  Length() const;

	//! Returns the number of frames in the animation.
	int					  NumFrames() const;

	//! Returns the number of animations in the animation set.
	int					  NumAnims() const;

	//! Returns the total movement delta for the animation.
	const idVec3&		  TotalMovementDelta() const;

	/*!
		\brief Retrieves the origin offset for a specified animation at a given time and cycle count

		This function fetches the origin offset for a specific animation frame, taking into account the animation number, current time, and cycle count. It returns false if the animation data is
	   invalid, otherwise it populates the offset parameter with the calculated origin and returns true. The function is used for animation positioning and is typically called during rendering or
	   animation update processes

		\param offset Output parameter that will contain the calculated origin offset
		\param animNum Index of the animation to query
		\param time Current time position within the animation
		\param cyclecount Number of cycles to apply to the animation
		\return True if the animation data is valid and the offset was successfully calculated, false otherwise
	*/
	bool				  GetOrigin( idVec3& offset, int animNum, int time, int cyclecount ) const;

	/*!
		\brief Retrieves the origin rotation for a specified animation at a given time and cycle count.

		This function fetches the rotation of the animation's origin for a specified animation number at a given current time and cycle count. It returns false if the animation is invalid, and sets
	   the rotation to the identity quaternion in that case. If the animation is valid, it retrieves the rotation from the animation data and returns true.

		\param rotation Output parameter that will hold the resulting rotation quaternion
		\param animNum Index of the animation to retrieve the rotation from
		\param currentTime Current time in the animation cycle
		\param cyclecount Number of cycles to consider for the animation
		\return True if the animation is valid and the rotation is successfully retrieved, false otherwise.
	*/
	bool				  GetOriginRotation( idQuat& rotation, int animNum, int currentTime, int cyclecount ) const;

	/*!
		\brief Retrieves the bounding box for a specific animation frame and cycle count

		This function retrieves the bounding box for a given animation number, current time, and cycle count. It checks if the animation exists before attempting to retrieve the bounds. The function
	   returns true if successful and false if the animation does not exist.

		\param bounds Output parameter containing the resulting bounding box
		\param animNum Index of the animation to retrieve bounds for
		\param time Current time in the animation
		\param cyclecount Number of cycles to consider for the animation bounds
		\return True if the animation exists and bounds were successfully retrieved, false otherwise
	*/
	bool				  GetBounds( idBounds& bounds, int animNum, int time, int cyclecount ) const;

	/*!
		\brief Adds a frame command to an animation frame with the specified parameters

		This function processes a frame command definition from a lexer input and adds it to the specified animation frame. It supports various command types including script function calls, event
	   triggers, and multiple sound types. The function validates frame number bounds and handles different token types to determine the appropriate command type and associated data. Error messages
	   are returned as const char pointers when validation fails or tokens are missing.

		\param modelDef Model definition containing the animation data
		\param framenum Frame number to add the command to (1-based)
		\param src Lexer object containing the command definition tokens
		\param def Dictionary containing additional definition parameters
		\return NULL if successful, otherwise returns an error message string describing the failure
	*/
	const char*			  AddFrameCommand( const class idDeclModelDef* modelDef, int framenum, idLexer& src, const idDict* def );

	//! Executes frame commands for an animation between specified frame indices on an entity
	void				  CallFrameCommands( idEntity* ent, int from, int to ) const;

	//! Returns true if the animation has frame commands associated with it.
	bool				  HasFrameCommands() const;

	//! Returns the first frame number where the specified frame command occurs, or -1 if not found.
	int					  FindFrameForFrameCommand( frameCommandType_t framecommand, const frameCommand_t** command ) const;

	//! Sets the animation flags for this animation.
	void				  SetAnimFlags( const animFlags_t& animflags );

	//! Returns the animation flags for this animation.
	const animFlags_t&	  GetAnimFlags() const;
};

/*!
	\class idDeclModelDef
	\brief Manages model definition declarations including joints, animations, and rendering data.

	This class represents a model definition declaration that encapsulates all the data needed to load and render a 3D model. It inherits from idDecl and provides functionality for parsing model data,
   managing joints and their hierarchy, handling animations, and retrieving rendering information. The class supports operations like setting up joint matrices for animation, accessing joint
   information by name or handle, and managing skin and animation data. It also provides methods to retrieve import options, visual offsets, and default poses. The declaration can be parsed from text
   data and maintains references to associated render models and skins. Memory management is handled through standard RAII principles with proper initialization and cleanup routines.

*/
class idDeclModelDef : public idDecl
{
public:
	//! Initializes a new instance of the idDeclModelDef class with default values.
	idDeclModelDef();

	//! Destructor for the idDeclModelDef class that frees associated data.
	~idDeclModelDef();

	//! Returns the size of the declaration in memory.
	virtual size_t			   Size() const;

	//! Returns the default definition string that can be parsed to recreate a declaration's default state.
	virtual const char*		   DefaultDefinition() const;

	//! Parses the given text data for the declaration, returning true if successful.
	virtual bool			   Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees any pointers held by the model definition and prepares it for re-parsing.
	virtual void			   FreeData();

	//! Ensures the model associated with this declaration is parsed and loaded if it exists.
	void					   Touch() const;

	//! Returns the default skin associated with this model definition.
	const idDeclSkin*		   GetDefaultSkin() const;

	//! Returns the default animation pose for the model.
	const idJointQuat*		   GetDefaultPose() const;

	/*!
		\brief Initializes and sets up joint matrices for a model definition based on its default pose and joint hierarchy

		This function prepares joint transformations for a model by converting joint quaternions to joint matrices and applying the model's joint hierarchy. It handles special cases like origin offset
	   removal and initializes the joint list with proper transformations. The function also calculates and returns the bounding box of the model's default pose.

		\param numJoints Pointer to store the number of joints in the model
		\param jointList Pointer to store the allocated joint matrix list
		\param frameBounds Reference to store the calculated bounds of the model's default pose
		\param removeOriginOffset Flag indicating whether to remove the origin offset from the model's transformation
		\throws gameLocal.Error when the model has no joints
	*/
	void					   SetupJoints( int* numJoints, idJointMat** jointList, idBounds& frameBounds, bool removeOriginOffset ) const;

	//! Returns the render model handle associated with this model definition.
	idRenderModel*			   ModelHandle() const;

	//! Parses a string of joint names and populates a list with corresponding joint handles
	void					   GetJointList( const char* jointnames, idList<jointHandle_t>& jointList ) const;

	//! Returns the joint info for the specified joint name
	const jointInfo_t*		   FindJoint( const char* name ) const;

	//! Returns the total number of animations in the model definition.
	int						   NumAnims() const;

	//! Returns the animation at the specified index, or NULL if the index is out of bounds.
	const idAnim*			   GetAnim( int index ) const;

	//! Returns the index of a specific animation by name, or zero if not found.
	int						   GetSpecificAnim( const char* name ) const;

	//! Returns the index of a specific animation by name, or a random one if multiple exist.
	int						   GetAnim( const char* name ) const;

	//! Checks if an animation with the specified name exists in the model definition.
	bool					   HasAnim( const char* name ) const;

	//! Returns the skin associated with this model definition.
	const idDeclSkin*		   GetSkin() const;

	//! Returns the name of the model associated with this model definition.
	const char*				   GetModelName() const;

	//! Returns the list of joint information for the model definition.
	const idList<jointInfo_t>& Joints() const;

	//! Returns a pointer to the array of joint parent indices for the model definition.
	const int*				   JointParents() const;

	//! Returns the number of joints in the model definition.
	int						   NumJoints() const;

	//! Returns a pointer to the joint information for the specified joint handle.
	const jointInfo_t*		   GetJoint( int jointHandle ) const;

	//! Returns the name of the joint specified by the given handle.
	const char*				   GetJointName( int jointHandle ) const;

	//! Returns the number of joints on the specified animation channel.
	int						   NumJointsOnChannel( int channel ) const;

	//! Returns the joints associated with the specified animation channel.
	const int*				   GetChannelJoints( int channel ) const;

	//! Returns the visual offset vector for the model definition.
	const idVec3&			   GetVisualOffset() const;

	//! Returns a pointer to the import options associated with this model definition.
	const idImportOptions*	   GetImportOptions() const { return &importOptions; }

private:
	//! Copies the data from another model definition declaration into this one.
	void CopyDecl( const idDeclModelDef* decl );

	//! Parses animation data from a source lexer and associates it with the model definition.
	bool ParseAnim( idLexer& src, int numDefaultAnims, const idStr& defaultCommands );

private:
	idVec3						  offset;
	idList<jointInfo_t, TAG_ANIM> joints;
	idList<int, TAG_ANIM>		  jointParents;
	idList<int, TAG_ANIM>		  channelJoints[ANIM_NumAnimChannels];
	idRenderModel*				  modelHandle;
	idList<idAnim*, TAG_ANIM>	  anims;
	const idDeclSkin*			  skin;
	bool						  hasCustomRotationSet; // HarrievG
	idAngles					  originalRotation;		// HarrievG

	idImportOptions				  importOptions; // RB
};

/*!
	\class idAnimBlend
	\brief Manages animation blending operations for skeletal animations with support for multiple channels and frame-based control.

	The idAnimBlend class provides functionality for blending skeletal animations across multiple channels, handling frame-based playback, weight management, and animation state transitions. It
   supports various animation modes including single frame setting, cycling, and continuous playback with optional blending. The class maintains animation state information such as start time, end
   time, frame count, and playback rate, while offering methods to query current animation status, manage blend weights, and handle synchronization between multiple animations. It includes support for
   frame commands, origin offset removal, and delta calculations for smooth animation transitions. The class is designed to work with model definitions and animation data, providing validation and
   error handling for joint count mismatches and invalid animation states. Memory management is handled through standard construction and destruction without explicit ownership patterns.

*/
class idAnimBlend
{
private:
	const class idDeclModelDef* modelDef;
	int							starttime;
	int							endtime;
	int							timeOffset;
	float						rate;

	int							blendStartTime;
	int							blendDuration;
	float						blendStartValue;
	float						blendEndValue;

	float						animWeights[ANIM_MaxSyncedAnims];
	short						cycle;
	short						frame;
	short						animNum;
	bool						allowMove;
	bool						allowFrameCommands;

	friend class idAnimator;

	//! Resets the animation blend state using the provided model definition.
	void Reset( const idDeclModelDef* _modelDef );

	//! Executes frame commands for an animation blend within a specified time range on an entity.
	void CallFrameCommands( idEntity* ent, int fromtime, int totime ) const;

	/*!
		\brief Sets a specific frame for an animation on a model definition with blending

		Initializes the animation blend state for a given model definition and animation number, validating the animation and joint counts. The function configures the animation to play a specific
	   frame, handling edge cases where the frame number is invalid by clamping it to the valid range. It also sets up the blending parameters for smooth transitions during animation playback.

		\param modelDef Pointer to the model definition containing the animation data
		\param animnum Index of the animation within the model definition
		\param frame The frame number to set for the animation, adjusted if out of bounds
		\param currenttime Current time in milliseconds for setting up animation start time
		\param blendtime Duration in milliseconds for the blend transition
		\throws Warning message if the model and animation joint counts do not match
	*/
	void SetFrame( const idDeclModelDef* modelDef, int animnum, int frame, int currenttime, int blendtime );

	/*!
		\brief Sets up an animation to cycle continuously from the start time with optional blending.

		This function initializes an animation to play in a continuous loop starting at a specified time. It validates the animation and model definition, ensures joint counts match, and sets up the
	   blending parameters for smooth transitions. If the animation has the random cycle start flag, it begins at a random point in the animation to avoid synchronized movement.

		\param modelDef Pointer to the model definition containing the animation
		\param animnum Index of the animation to cycle
		\param currenttime Current time in the animation system
		\param blendtime Duration of the blending transition in milliseconds
	*/
	void CycleAnim( const idDeclModelDef* modelDef, int animnum, int currenttime, int blendtime );

	/*!
		\brief Plays an animation on a model with optional blending

		Initializes the animation blend state to play a specified animation on a given model definition. The function verifies that the animation and model have matching joint counts, and sets up the
	   animation timing and blending parameters. It resets the current animation state before starting the new animation.

		\param modelDef Pointer to the model definition containing the animation data
		\param animnum Index of the animation to play within the model definition
		\param currenttime Start time for the animation in milliseconds
		\param blendtime Duration of the blend transition in milliseconds
		\throws Warning message when the model and animation joint counts do not match
	*/
	void PlayAnim( const idDeclModelDef* modelDef, int animnum, int currenttime, int blendtime );

	/*!
		\brief Blends animation data for a specified channel into a joint frame with optional weight and origin offset removal

		This function performs animation blending for a given animation channel by interpolating or sampling animation frames based on the current time and animation parameters. It handles both single
	   and multiple animation mixing, applies weight calculations, and optionally removes origin offsets. The function supports different blending modes based on whether the blend weight is zero or
	   not, and allows for printing debug information if requested. The blended results are stored in the provided blendFrame array, with joint information updated according to the animation data.

		\param currentTime Current time in the animation timeline
		\param channel Animation channel to blend
		\param numJoints Number of joints in the skeleton
		\param blendFrame Output array to store the blended joint frames
		\param blendWeight Reference to the blend weight, updated during blending
		\param removeOrigin Flag to indicate if the origin offset should be removed
		\param overrideBlend Flag to indicate if the blend weight should be overridden
		\param printInfo Flag to enable or disable printing of debug information
		\return True if the blending operation was successful, false otherwise
	*/
	bool BlendAnim( int currentTime, int channel, int numJoints, idJointQuat* blendFrame, float& blendWeight, bool removeOrigin, bool overrideBlend, bool printInfo ) const;

	/*!
		\brief Combines the origin position from an animation with a blend position and weight.

		This function blends the origin position from an animation with an existing blend position and weight. It calculates the animation position based on the current time and animation cycle, then
	   updates the blend position and weight using a weighted average. The function handles cases where the animation is not active or when the weight is zero, returning early in such scenarios. If
	   the blend weight is zero, the position is directly assigned; otherwise, a weighted interpolation is performed.

		\param currentTime The current time in the animation cycle
		\param blendPos The current blended position to be updated
		\param blendWeight The current blended weight to be updated
		\param removeOriginOffset Flag indicating if the origin offset should be removed
	*/
	void BlendOrigin( int currentTime, idVec3& blendPos, float& blendWeight, bool removeOriginOffset ) const;

	/*!
		\brief Computes and accumulates positional blend delta information for animation transitions between two time points.

		This function calculates the positional difference between two animation states at specified time points and accumulates this information with any previously calculated deltas. It handles
	   blending weights and supports multi-animation sequences. The function is typically used during animation blending operations to determine how much an animation should influence the overall
	   position of a model. It checks for valid animation state and time boundaries, and ensures proper weight handling during blend operations.

		\param fromtime Start time for the animation sample
		\param totime End time for the animation sample
		\param blendDelta Reference to store the accumulated positional delta
		\param blendWeight Reference to store the accumulated blend weight
	*/
	void BlendDelta( int fromtime, int totime, idVec3& blendDelta, float& blendWeight ) const;

	/*!
		\brief Computes and blends rotation deltas between two animation times for skeletal animation

		This function calculates the rotational difference between two points in an animation cycle and blends it with previous rotation data. It handles multiple animation layers and performs
	   spherical linear interpolation to smoothly transition between rotations. The function is used in skeletal animation systems to maintain proper orientation during animation blending. It checks
	   for valid animation state and applies weight-based blending to combine multiple animation influences.

		\param fromtime Starting animation time in milliseconds
		\param totime Ending animation time in milliseconds
		\param blendDelta Reference to store the resulting rotation delta quaternion
		\param blendWeight Reference to store the accumulated weight for blending
	*/
	void BlendDeltaRotation( int fromtime, int totime, idQuat& blendDelta, float& blendWeight ) const;

	//! Adds the bounds of the animation to the provided bounds object.
	bool AddBounds( int currentTime, idBounds& bounds, bool removeOriginOffset ) const;

public:
	//! Constructs an idAnimBlend object and resets it.
	idAnimBlend();

	//! Saves the animation blend state to a save game file.
	void		  Save( idSaveGame* savefile ) const;

	//! Restores the animation state from a save game file
	void		  Restore( idRestoreGame* savefile, const idDeclModelDef* modelDef );

	//! Returns the name of the animation associated with this animation blend instance.
	const char*	  AnimName() const;

	//! Returns the full name of the animation associated with this animation blend.
	const char*	  AnimFullName() const;

	//! Returns the blend weight for the given current time.
	float		  GetWeight( int currenttime ) const;

	//! Returns the final weight value of the animation blend.
	float		  GetFinalWeight() const;

	//! Sets the weight for an animation blend with optional blending time.
	void		  SetWeight( float newweight, int currenttime, int blendtime );

	//! Returns the number of synced animations in the animation blend.
	int			  NumSyncedAnims() const;

	//! Sets the weight for a specific synced animation
	bool		  SetSyncedAnimWeight( int num, float weight );

	//! Clears the animation blend by resetting or fading out based on the specified time parameters.
	void		  Clear( int currentTime, int clearTime );

	//! Returns true if the animation blend is complete based on the current time and blend parameters.
	bool		  IsDone( int currentTime ) const;

	//! Determines if the animation frame has changed based on the current time and animation state.
	bool		  FrameHasChanged( int currentTime ) const;

	//! Returns the number of cycles for the animation blend.
	int			  GetCycleCount() const;

	//! Sets the cycle count for the animation blend.
	void		  SetCycleCount( int count );

	//! Sets the playback rate for the animation blend.
	void		  SetPlaybackRate( int currentTime, float newRate );

	//! Returns the playback rate of the animation blend.
	float		  GetPlaybackRate() const;

	//! Sets the start time for the animation blend.
	void		  SetStartTime( int startTime );

	//! Returns the start time of the animation blend.
	int			  GetStartTime() const;

	//! Returns the end time of the animation blend.
	int			  GetEndTime() const;

	//! Returns the frame number for the given current time.
	int			  GetFrameNumber( int currenttime ) const;

	//! Calculates the animation time in milliseconds based on the current time and animation properties.
	int			  AnimTime( int currenttime ) const;

	//! Returns the number of frames in the animation.
	int			  NumFrames() const;

	//! Returns the length of the animation in the blend.
	int			  Length() const;

	//! Returns the length of the animation play time.
	int			  PlayLength() const;

	//! Sets whether movement is allowed for the animation blend.
	void		  AllowMovement( bool allow );

	//! Sets whether frame commands are allowed for this animation blend.
	void		  AllowFrameCommands( bool allow );

	//! Returns the animation associated with this anim blend node.
	const idAnim* Anim() const;

	//! Returns the animation number associated with this animation blend instance.
	int			  AnimNum() const;
};

/*
==============================================================================================

	idAFPoseJointMod

==============================================================================================
*/

typedef enum { AF_JOINTMOD_AXIS, AF_JOINTMOD_ORIGIN, AF_JOINTMOD_BOTH } AFJointModType_t;

/*!
	\class idAFPoseJointMod
	\brief Represents a joint modification applied to a pose in a physics simulation.
*/
class idAFPoseJointMod
{
public:
	//! Initializes a new instance of the idAFPoseJointMod class with default axis, origin, and joint modification type.
	idAFPoseJointMod();

	AFJointModType_t mod;
	idMat3			 axis;
	idVec3			 origin;
};

ID_INLINE idAFPoseJointMod::idAFPoseJointMod()
{
	mod = AF_JOINTMOD_AXIS;
	axis.Identity();
	origin.Zero();
}

/*!
	\class idAnimator
	\brief Manages animation playback and joint transformations for animated entities.

	The idAnimator class handles the playback of animations and management of joint transformations for animated entities. It provides methods for setting and playing animations on different channels,
   retrieving joint information, and managing animation states including blending and synchronization. The class supports both standard animation playback and articulated figure (AF) pose management.
   It maintains a model definition and associated render model to provide animation data and joint information. The animator can be associated with an entity, which allows it to trigger updates and
   maintain synchronization with the entity's state. Joint modifications can be set and cleared to override default animation behavior. The class also provides functionality for calculating movement
   deltas, bounds, and transformations for joints at specific times. Memory management is handled through allocation tracking and data freeing methods.

*/
class idAnimator
{
public:
	//! Initializes a new instance of the idAnimator class.
	idAnimator();

	//! Destructor for the idAnimator class that releases allocated resources.
	~idAnimator();

	//! Returns the total memory allocated by the animator instance.
	size_t				  Allocated() const;

	//! Returns the total size in bytes of the animator object including allocated memory
	size_t				  Size() const;

	//! Archives object for save game file
	void				  Save( idSaveGame* savefile ) const;

	//! Restores the animator state from a save game file
	void				  Restore( idRestoreGame* savefile );

	//! Sets the entity associated with this animator.
	void				  SetEntity( idEntity* ent );

	//! Returns the entity associated with this animator.
	idEntity*			  GetEntity() const;

	//! Sets whether to remove the origin offset from the animation.
	void				  RemoveOriginOffset( bool remove );

	//! Returns the current state of the remove origin offset flag.
	bool				  RemoveOrigin() const;

	//! Retrieves the list of joint handles for the specified joint names.
	void				  GetJointList( const char* jointnames, idList<jointHandle_t>& jointList ) const;

	//! Returns the number of animations in the animator's model definition.
	int					  NumAnims() const;

	//! Returns the animation at the specified index from the model definition.
	const idAnim*		  GetAnim( int index ) const;

	//! Returns the animation index for the given animation name.
	int					  GetAnim( const char* name ) const;

	//! Checks if the animator has an animation with the specified name.
	bool				  HasAnim( const char* name ) const;

	//! Services animations by calling frame commands and managing animation state updates.
	void				  ServiceAnims( int fromtime, int totime );

	//! Returns true if the animator is currently playing any animations.
	bool				  IsAnimating( int currentTime ) const;

	//! Retrieves the joint count and joint array from the animator.
	void				  GetJoints( int* numJoints, idJointMat** jointsPtr );

	//! Returns the number of joints in the animator.
	int					  NumJoints() const;

	//! Returns the first child joint of the specified joint
	jointHandle_t		  GetFirstChild( jointHandle_t jointnum ) const;

	//! Retrieves the handle of the first child joint for the specified joint name.
	jointHandle_t		  GetFirstChild( const char* name ) const;

	//! Sets the model for the animator using the specified model name and returns a pointer to the render model.
	idRenderModel*		  SetModel( const char* modelname );

	//! Returns the render model handle associated with this animator's model definition
	idRenderModel*		  ModelHandle() const;

	//! Returns the model definition associated with this animator.
	const idDeclModelDef* ModelDef() const;

	//! Forces an immediate update of the animator's state.
	void				  ForceUpdate();

	//! Clears the force update flag in the animator.
	void				  ClearForceUpdate();

	//! Creates a frame of animation for the specified animation time
	bool				  CreateFrame( int animtime, bool force );

	//! Determines if any animation frame has changed since the last update for the given animation time.
	bool				  FrameHasChanged( int animtime ) const;

	//! Computes the positional delta between two animation times.
	void				  GetDelta( int fromtime, int totime, idVec3& delta ) const;

	//! Calculates the delta rotation between two animation times and returns true if successful.
	bool				  GetDeltaRotation( int fromtime, int totime, idMat3& delta ) const;

	//! Calculates and returns the origin position of an animated object at a given time.
	void				  GetOrigin( int currentTime, idVec3& pos ) const;

	//! Retrieves the world bounds of the animated model at the specified time.
	bool				  GetBounds( int currentTime, idBounds& bounds );

	//! Returns the current animation for the specified channel.
	idAnimBlend*		  CurrentAnim( int channelNum );

	//! Clears animation data for a specified channel within the animator.
	void				  Clear( int channelNum, int currentTime, int cleartime );

	/*!
		\brief Sets the frame for a specified animation channel with optional blending.

		This function configures a specific animation channel to display a given frame from an animation. It first validates that the channel number is within the valid range, and checks if the
	   animation definition exists. If valid, it pushes existing animations and sets the new frame. The entity associated with the animator is activated to ensure animation updates occur.

		\param channelNum The animation channel number to set the frame on
		\param animnum The animation number to use for the frame
		\param frame The frame number to set
		\param currenttime The current time for the animation update
		\param blendtime The blend time for smooth transitions between animations
		\throws This function will cause a game error if the channel number is out of range.
	*/
	void				  SetFrame( int channelNum, int animnum, int frame, int currenttime, int blendtime );

	/*!
		\brief Sets up a cycling animation on the specified channel with the given animation number, current time, and blend time.

		This function configures a channel to play a specified animation in a cycling manner. It first validates that the channel number is within the accepted range defined by ANIM_NumAnimChannels.
	   If the channel is invalid, an error is reported. It then checks if the model definition and the specified animation exist. If either is missing, the function returns without performing any
	   action. The function pushes the animation onto the channel and initializes the cycling animation. If an entity is associated with the animator, it marks the entity as active for animation
	   updates.

		\param channelNum The channel number to set up the cycling animation on.
		\param animnum The animation number to cycle.
		\param currenttime The current time in milliseconds for the animation.
		\param blendtime The time in milliseconds for blending between animations.
		\throws error if channel number is out of range
	*/
	void				  CycleAnim( int channelNum, int animnum, int currenttime, int blendtime );

	/*!
		\brief Plays an animation on the specified channel with the given parameters.

		This function plays an animation on the specified channel number. It first validates that the channel number is within the acceptable range. If the channel is valid, it checks if the model
	   definition and animation exist. If both exist, it pushes the animations and plays the specified animation on the given channel. If an entity is associated with the animator, it activates the
	   entity for animation.

		\param channelNum The channel number on which to play the animation
		\param animnum The animation number to play
		\param currenttime The current time for the animation
		\param blendTime The blend time for transitioning the animation
		\throws idAnimator::PlayAnim : channel out of range
	*/
	void				  PlayAnim( int channelNum, int animnum, int currenttime, int blendTime );

	/*!
		\brief Copies an animation from one channel to another while synchronizing weights and disabling frame commands to prevent duplicate execution.

		This function synchronizes animation data from a source channel to a target channel. It ensures that the animation parameters including weight, start time, end time, and animation type are
	   properly copied. If the animations differ, it pushes the new animation and resets the blend values. The function also disables frame commands on the target channel to prevent them from being
	   executed twice, which could happen if the same animation was being played on both channels.

		\param channelNum The target animation channel to copy the animation to
		\param fromChannelNum The source animation channel to copy the animation from
		\param currenttime The current time for blending operations
		\param blendTime The duration over which to blend the animation transition
		\throws gameLocal.Error is called if either channel number is out of valid range
	*/
	void				  SyncAnimChannels( int channelNum, int fromChannelNum, int currenttime, int blendTime );

	//! Sets the position of a specified joint with the given transform type.
	void				  SetJointPos( jointHandle_t jointnum, jointModTransform_t transform_type, const idVec3& pos );

	//! Sets the axis transformation for a specified joint with the given matrix and transformation type.
	void				  SetJointAxis( jointHandle_t jointnum, jointModTransform_t transform_type, const idMat3& mat );

	//! Clears the joint modification for the specified joint number.
	void				  ClearJoint( jointHandle_t jointnum );

	//! Clears all joint modifications from the animator.
	void				  ClearAllJoints();

	//! Initializes the animation system's AF pose joint data structures.
	void				  InitAFPose();

	/*!
		\brief Sets the animation force pose joint modification for a specified joint

		This function configures the modification type, axis, and origin for a specific joint in the animation force pose system. It updates the joint modification data structure and ensures the joint
	   is properly registered in the pose joints list. The function maintains a sorted list of joints for efficient lookup and updates.

		\param jointNum The handle of the joint to modify
		\param mod The type of joint modification to apply
		\param axis The axis matrix defining the joint orientation
		\param origin The origin vector defining the joint position
	*/
	void				  SetAFPoseJointMod( const jointHandle_t jointNum, const AFJointModType_t mod, const idMat3& axis, const idVec3& origin );

	//! Finalizes an animated physics pose by processing joint transformations and updating the animation state
	void				  FinishAFPose( int animnum, const idBounds& bounds, const int time );

	//! Sets the blend weight for the AF pose.
	void				  SetAFPoseBlendWeight( float blendWeight );

	//! Blends the physics pose into the provided joint frame.
	bool				  BlendAFPose( idJointQuat* blendFrame ) const;

	//! Clears the articulated figure pose stored in the animator.
	void				  ClearAFPose();

	//! Clears all animations on all channels and the AF pose.
	void				  ClearAllAnims( int currentTime, int cleartime );

	//! Returns the joint handle for a given joint name, or INVALID_JOINT if the joint is not found.
	jointHandle_t		  GetJointHandle( const char* name ) const;

	//! Returns the name of the joint specified by the given handle
	const char*			  GetJointName( jointHandle_t handle ) const;

	//! Returns the animation channel index for a specified joint handle.
	int					  GetChannelForJoint( jointHandle_t joint ) const;

	/*!
		\brief Retrieves the transformation matrix and offset for a specified joint at a given time.

		This function fetches the position offset and orientation axis of a joint identified by jointHandle. It validates the joint handle against the model definition and creates a frame at the
	   specified time. The resulting transformation is stored in the provided offset and axis parameters. The function returns true if the joint handle is valid and the frame is successfully created,
	   otherwise it returns false.

		\param jointHandle The handle identifying the joint in the model definition
		\param currenttime The time at which to retrieve the joint's transformation
		\param offset Output parameter containing the joint's position offset
		\param axis Output parameter containing the joint's orientation axis
		\return True if the joint handle is valid and transformation data was successfully retrieved, false otherwise
	*/
	bool				  GetJointTransform( jointHandle_t jointHandle, int currenttime, idVec3& offset, idMat3& axis );

	/*!
		\brief Retrieves the local transformation of a specified joint at a given time

		The function calculates and returns the local position offset and orientation axis for a joint in the model. It first validates that the model definition exists and the joint handle is within
	   valid bounds. The function creates a frame for the specified time and then computes the local transformation by dividing the joint's transformation matrix by its parent joint's transformation
	   matrix. Special handling is included for the root joint (joint handle 0) to avoid unnecessary computations. The result is stored in the provided offset and axis parameters

		\param jointHandle The handle identifying which joint to retrieve the local transform for
		\param currentTime The time index to calculate the joint transform for
		\param offset Output parameter that receives the local position offset of the joint
		\param axis Output parameter that receives the local orientation axis of the joint
		\return True if the joint local transform was successfully retrieved, false otherwise
	*/
	bool				  GetJointLocalTransform( jointHandle_t jointHandle, int currentTime, idVec3& offset, idMat3& axis );

	//! Returns the animation flags for the specified animation number.
	const animFlags_t	  GetAnimFlags( int animnum ) const;

	//! Returns the number of frames in the specified animation.
	int					  NumFrames( int animnum ) const;

	//! Returns the number of synced animations for the specified animation number.
	int					  NumSyncedAnims( int animnum ) const;

	//! Returns the name of the animation at the specified index.
	const char*			  AnimName( int animnum ) const;

	//! Returns the full name of the animation with the specified index.
	const char*			  AnimFullName( int animnum ) const;

	//! Returns the length of the specified animation in milliseconds.
	int					  AnimLength( int animnum ) const;

	//! Returns the total movement delta for the specified animation.
	const idVec3&		  TotalMovementDelta( int animnum ) const;

private:
	//! Frees all data associated with the animator.
	void FreeData();

	//! Pushes animations into the specified channel with blending.
	void PushAnims( int channel, int currentTime, int blendTime );

private:
	const idDeclModelDef*			   modelDef;
	idEntity*						   entity;

	idAnimBlend						   channels[ANIM_NumAnimChannels][ANIM_MaxAnimsPerChannel];
	idList<jointMod_t*, TAG_ANIM>	   jointMods;
	int								   numJoints;
	idJointMat*						   joints;

	mutable int						   lastTransformTime; // mutable because the value is updated in CreateFrame
	mutable bool					   stoppedAnimatingUpdate;
	bool							   removeOriginOffset;
	bool							   forceUpdate;

	idBounds						   frameBounds;

	float							   AFPoseBlendWeight;
	idList<int, TAG_ANIM>			   AFPoseJoints;
	idList<idAFPoseJointMod, TAG_ANIM> AFPoseJointMods;
	idList<idJointQuat, TAG_ANIM>	   AFPoseJointFrame;
	idBounds						   AFPoseBounds;
	int								   AFPoseTime;
};

/*!
	\class idAnimManager
	\brief Manages animation assets and joint information for skeletal animations.

	The animation manager handles loading, storage, and retrieval of animation data while maintaining a registry of joint names and their corresponding indices. It supports preloading of animation
   resources, reloading of animations, and cleanup of unused animations. The manager provides access to animation data through named references and maintains internal mapping between joint names and
   indices to facilitate skeletal animation processing. It also offers debugging capabilities to list loaded animations and joint information.

*/
class idAnimManager
{
public:
	//! Constructs a new animation manager instance.
	idAnimManager();

	//! Destroys the animation manager and cleans up its resources.
	~idAnimManager();

	static bool forceExport;

	//! Shuts down the animation manager and releases all allocated resources.
	void		Shutdown();

	//! Retrieves or loads an animation by name, using the specified import options.
	idMD5Anim*	GetAnim( const char* name, const idImportOptions* options );

	//! Preloads animation resources specified in the manifest.
	void		Preload( const idPreloadManifest& manifest );

	//! Reloads all animations managed by the animation manager.
	void		ReloadAnims();

	//! Prints information about all loaded animations and joint names to the game log
	void		ListAnims() const;

	//! Returns the index of a joint by its name, creating a new index if the joint does not yet exist
	int			JointIndex( const char* name );

	//! Returns the name of the joint at the specified index
	const char* JointName( int index ) const;

	void		ClearAnimsInUse();

	//! Removes and cleans up animations that are no longer referenced.
	void		FlushUnusedAnims();

private:
	idHashTable<idMD5Anim*> animations;
	idStrList				jointnames;
	idHashIndex				jointnamesHash;
};

#endif /* !__ANIM_H__ */
