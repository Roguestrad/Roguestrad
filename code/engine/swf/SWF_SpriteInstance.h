/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans

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
#ifndef __SWF_SPRITEINSTANCE_H__
#define __SWF_SPRITEINSTANCE_H__

// RB: moved here
#define PlaceFlagHasClipActions	   BIT( 7 )
#define PlaceFlagHasClipDepth	   BIT( 6 )
#define PlaceFlagHasName		   BIT( 5 )
#define PlaceFlagHasRatio		   BIT( 4 )
#define PlaceFlagHasColorTransform BIT( 3 )
#define PlaceFlagHasMatrix		   BIT( 2 )
#define PlaceFlagHasCharacter	   BIT( 1 )
#define PlaceFlagMove			   BIT( 0 )

#define PlaceFlagPad0			   BIT( 7 )
#define PlaceFlagPad1			   BIT( 6 )
#define PlaceFlagPad2			   BIT( 5 )
#define PlaceFlagHasImage		   BIT( 4 )
#define PlaceFlagHasClassName	   BIT( 3 )
#define PlaceFlagCacheAsBitmap	   BIT( 2 )
#define PlaceFlagHasBlendMode	   BIT( 1 )
#define PlaceFlagHasFilterList	   BIT( 0 )

/*!
	\class idSWFSpriteInstance
	\brief Manages SWF sprite instances with animation, display list operations, and script bindings.

	Handles the runtime behavior of SWF sprite instances including animation playback, display list management, and script integration. Supports operations such as frame control, positioning, scaling,
   rotation, and visibility management. Provides methods for initializing instances with parent-child relationships, executing actions and Lua scripts, and managing the display list through
   depth-based operations. The class maintains a connection to underlying sprite data and supports various SWF control commands for interactive media playback.

*/
class idSWFSpriteInstance
{
public:
	//! Constructs a new SWF sprite instance with default values.
	idSWFSpriteInstance();

	//! Destructor for idSWFSpriteInstance that cleans up resources and clears script bindings.
	~idSWFSpriteInstance();

	//! Initializes a SWF sprite instance with the given sprite, parent, and depth.
	void			   Init( idSWFSprite* sprite, idSWFSpriteInstance* parent, int depth );

	//! Runs the sprite instance animation and returns true if it is still active.
	bool			   Run();

	//! Executes the stored actions and Lua actions for this sprite instance
	bool			   RunActions();

	//! Returns the name of the sprite instance as a null-terminated string.
	const char*		   GetName() const { return name.c_str(); }

	//! Returns the script object associated with this sprite instance
	idSWFScriptObject* GetScriptObject() { return scriptObject; }

	//! Sets the alignment offsets for the sprite instance.
	void			   SetAlignment( float x, float y )
	{
		xOffset = x;
		yOffset = y;
	}

	//! Sets the material override for this sprite instance and updates its dimensions
	void SetMaterial( const idMaterial* material, int width = -1, int height = -1 );

	//! Sets the visibility state of the sprite instance.
	void SetVisible( bool visible );

	//! Returns true if the sprite instance is visible.
	bool IsVisible() { return isVisible; }

	//! Plays a frame from the sprite instance based on the provided parameters.
	void PlayFrame( const idSWFParmList& parms );

	//! Plays a specific frame by name on the sprite instance.
	void PlayFrame( const char* frameName )
	{
		idSWFParmList parms;
		parms.Append( frameName );
		PlayFrame( parms );
	}

	//! Plays a specific frame of the sprite instance by setting the frame number.
	void PlayFrame( const int frameNum )
	{
		idSWFParmList parms;
		parms.Append( frameNum );
		PlayFrame( parms );
	}

	//! Stops the sprite instance at a specified frame.
	void StopFrame( const idSWFParmList& parms );

	//! Stops playback of the specified frame on the sprite instance
	void StopFrame( const char* frameName )
	{
		idSWFParmList parms;
		parms.Append( frameName );
		StopFrame( parms );
	}

	//! Stops the sprite instance at the specified frame number.
	void StopFrame( const int frameNum )
	{
		idSWFParmList parms;
		parms.Append( frameNum );
		StopFrame( parms );
	}

	//! Returns the x position of the sprite instance.
	float							   GetXPos() const;

	//! Returns the Y position of the sprite instance.
	float							   GetYPos( bool overallPos = false ) const;

	//! Sets the x position of the sprite instance within its parent's display list.
	void							   SetXPos( float xPos = -1.0f );

	//! Sets the y position of the sprite instance.
	void							   SetYPos( float yPos = -1.0f );

	//! Sets the position of the sprite instance in the SWF display list.
	void							   SetPos( float xPos = -1.0f, float yPos = -1.0f );

	//! Sets the alpha value of the sprite instance.
	void							   SetAlpha( float val );

	//! Sets the scale of the SWF sprite instance using the provided x and y factors
	void							   SetScale( float x = -1.0f, float y = -1.0f );

	//! Sets the scale factors for move-to operations on the sprite instance.
	void							   SetMoveToScale( float x = -1.0f, float y = -1.0f );

	//! Updates the sprite instance scale towards a target scale over time based on the provided speed parameter and returns true if the update was successful.
	bool							   UpdateMoveToScale( float speed );

	//! Sets the rotation angle of the SWF sprite instance.
	void							   SetRotation( float rot );

	//! Returns the current frame number of the sprite instance.
	uint16							   GetCurrentFrame() { return currentFrame; }

	//! Returns true if the sprite instance is currently playing.
	bool							   IsPlaying() const { return isPlaying; }

	//! Returns the stereo depth value for the SWF sprite instance.
	int								   GetStereoDepth() { return stereoDepth; }

	// private:
	//  Removing the private access control statement due to cl 214702
	//  Apparently MS's C++ compiler supports the newer C++ standard, and GCC supports C++03
	//  In the new C++ standard, nested members of a friend class have access to private/protected members of the class granting friendship
	//  In C++03, nested members defined in a friend class do NOT have access to private/protected members of the class granting friendship	friend class idSWF;

	bool							   isPlaying;
	bool							   isVisible;
	bool							   childrenRunning;
	bool							   firstRun;

	// currentFrame is the frame number currently in the displayList
	// we use 1 based frame numbers because currentFrame = 0 means nothing is in the display list
	// it's also convenient because Flash also uses 1 based frame numbers
	uint16							   currentFrame;
	uint16							   frameCount;

	// the sprite this is an instance of
	idSWFSprite*					   sprite;

	// sprite instances can be nested
	idSWFSpriteInstance*			   parent;

	// depth of this sprite instance in the parent's display list
	int								   depth;

	// if this is set, apply this material when rendering any child shapes
	int								   itemIndex;

	const idMaterial*				   materialOverride;
	uint16							   materialWidth;
	uint16							   materialHeight;

	float							   xOffset;
	float							   yOffset;

	float							   moveToXScale;
	float							   moveToYScale;
	float							   moveToSpeed;

	int								   stereoDepth;

	idSWFScriptObject*				   scriptObject;

	// children display entries
	idList<swfDisplayEntry_t, TAG_SWF> displayList;

	//! Finds and returns a display entry at the specified depth in the sprite instance's display list
	swfDisplayEntry_t*				   FindDisplayEntry( int depth );

	// name of this sprite instance
	idStr							   name;
#ifdef _DEBUG
	idStr globalName;
#endif

	struct swfAction_t {
		const byte* data;
		uint32		dataLength;
	};
	idList<swfAction_t, TAG_SWF> actions;
	idList<swfAction_t, TAG_SWF> luaActions;

	idSWFScriptFunction_Script*	 actionScript;

	idSWFScriptVar				 onEnterFrame;
	//----------------------------------
	// SWF_PlaceObject.cpp
	//----------------------------------

	//! Processes a PlaceObject2 command from a SWF bitstream to update or create display entries
	void						 PlaceObject2( idSWFBitStream& bitstream );

	//! Processes a PlaceObject3 tag from a SWF bitstream to update or create display entries in a sprite instance.
	void						 PlaceObject3( idSWFBitStream& bitstream );

	//! Removes a display entry from the sprite instance using the ID read from the bitstream.
	void						 RemoveObject2( idSWFBitStream& bitstream );

	//----------------------------------
	// SWF_Sounds.cpp
	//----------------------------------

	//! Starts playing a sound from the provided bitstream.
	void						 StartSound( idSWFBitStream& bitstream );

	//----------------------------------
	// SWF_SpriteInstance.cpp
	//----------------------------------

	//! Advances the sprite instance to the next frame if not at the last frame.
	void						 NextFrame();

	//! Moves the sprite instance to the previous frame in the animation sequence.
	void						 PrevFrame();

	//! Executes sprite commands up to the specified frame number.
	void						 RunTo( int frameNum );

	//! Starts playing the SWF sprite instance and its parent chain.
	void						 Play();

	//! Stops the sprite instance from playing.
	void						 Stop();

	//! Frees the memory allocated for the display list entries and resets the current frame.
	void						 FreeDisplayList();

	//! Adds a display entry to the sprite instance at the specified depth with the given character ID, returning a pointer to the new entry or NULL if an entry already exists at that depth
	swfDisplayEntry_t*			 AddDisplayEntry( int depth, int characterID );

	//! Removes a display entry at the specified depth from the sprite instance.
	void						 RemoveDisplayEntry( int depth );

	//! Swaps the depths of two display list entries in the sprite instance.
	void						 SwapDepths( int depth1, int depth2 );

	//! Allocates and initializes an action from the provided bitstream data
	void						 DoAction( idSWFBitStream& bitstream );

	//! Loads and stores the Lua action data from the provided bitstream into the sprite instance's action list.
	void						 DoLua( idSWFBitStream& bitstream );

	//! Finds and returns a child sprite instance by its name
	idSWFSpriteInstance*		 FindChildSprite( const char* childName );

	//! Resolves a target sprite instance from a target name string
	idSWFSpriteInstance*		 ResolveTarget( const char* targetName );

	//! FindFrame searches for a frame by label or returns the frame number if the label is a numeric string.
	uint32						 FindFrame( const char* frameLabel ) const;

	//! Checks if a frame with the specified label exists in the sprite instance.
	bool						 FrameExists( const char* frameLabel ) const;

	//! Checks if the current frame is between the given inclusive range defined by two frame labels.
	bool						 IsBetweenFrames( const char* frameLabel1, const char* frameLabel2 ) const;

	// RB begin
	static int					 Lua_new( lua_State* L );

	//! Handles garbage collection for SWF sprite instances in Lua
	static int					 Lua_gc( lua_State* L );

	//! Handles Lua indexing operations for SWF sprite instances
	static int					 Lua_index( lua_State* L );

	//! Handles setting properties on SWF sprite instances from Lua
	static int					 Lua_newindex( lua_State* L );

	//! Converts a SWF sprite instance to a string representation for Lua.
	static int					 Lua_tostring( lua_State* L );

	//! Stops the sprite instance animation.
	static int					 Lua_stop( lua_State* L );

	//! Plays the SWF sprite animation.
	static int					 Lua_play( lua_State* L );

	//! Moves the sprite to a specified frame and starts playing
	static int					 Lua_gotoAndPlay( lua_State* L );

	//! Sets the sprite to a specific frame and stops playback.
	static int					 Lua_gotoAndStop( lua_State* L );

	//! Registers the SWFSpriteInstance Lua class with the provided Lua state.
	static int					 LuaRegister_idSWFSpriteInstance( lua_State* L );
	// RB end
};

/*!
	\class idSWFScriptObject_SpriteInstancePrototype
	\brief Provides a prototype for SWF sprite instance script objects with standardized properties and functions.

	This class serves as the prototype for SWF sprite instance script objects, establishing a common structure and set of properties that all sprite instances can reference. It initializes with all
   supported functions and variables, including both read-only and read-write properties. The class uses a macro-based approach to declare various native variables and functions, providing a
   standardized interface for sprite instance manipulation within the SWF scripting environment. The prototype ensures consistent behavior across different sprite instances by defining their shared
   characteristics and capabilities.

*/
class idSWFScriptObject_SpriteInstancePrototype : public idSWFScriptObject
{
public:
	//! Initializes a new instance of the SWF sprite object prototype with all supported functions and variables.
	idSWFScriptObject_SpriteInstancePrototype();

#define SWF_SPRITE_FUNCTION_DECLARE( x )                                                  \
	class idSWFScriptFunction_##x : public idSWFScriptFunction                            \
	{                                                                                     \
	public:                                                                               \
		void AddRef()                                                                     \
		{                                                                                 \
		}                                                                                 \
		void Release()                                                                    \
		{                                                                                 \
		}                                                                                 \
		idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms ); \
	} scriptFunction_##x

	//! Duplicates a movie clip instance with the specified name and depth
	SWF_SPRITE_FUNCTION_DECLARE( duplicateMovieClip );

	//! Moves the sprite to a specified frame and starts playing from that frame.
	SWF_SPRITE_FUNCTION_DECLARE( gotoAndPlay );

	//! Jumps to a specified frame and stops playback of a sprite instance.
	SWF_SPRITE_FUNCTION_DECLARE( gotoAndStop );

	//! Swaps the depth of this sprite instance with another sprite at the specified depth
	SWF_SPRITE_FUNCTION_DECLARE( swapDepths );

	//! Advances the sprite instance to the next frame in its timeline.
	SWF_SPRITE_FUNCTION_DECLARE( nextFrame );

	//! Moves the sprite to the previous frame in its timeline
	SWF_SPRITE_FUNCTION_DECLARE( prevFrame );

	//! Plays the animation of the sprite instance.
	SWF_SPRITE_FUNCTION_DECLARE( play );

	//! Stops the sprite instance associated with this object.
	SWF_SPRITE_FUNCTION_DECLARE( stop );

	SWF_NATIVE_VAR_DECLARE( _x );
	SWF_NATIVE_VAR_DECLARE( _y );
	SWF_NATIVE_VAR_DECLARE( _xscale );
	SWF_NATIVE_VAR_DECLARE( _yscale );
	SWF_NATIVE_VAR_DECLARE( _alpha );
	SWF_NATIVE_VAR_DECLARE( _brightness );
	SWF_NATIVE_VAR_DECLARE( _visible );
	SWF_NATIVE_VAR_DECLARE( _width );
	SWF_NATIVE_VAR_DECLARE( _height );
	SWF_NATIVE_VAR_DECLARE( _rotation );

	SWF_NATIVE_VAR_DECLARE_READONLY( _name );
	SWF_NATIVE_VAR_DECLARE_READONLY( _currentframe );
	SWF_NATIVE_VAR_DECLARE_READONLY( _totalframes );
	SWF_NATIVE_VAR_DECLARE_READONLY( _target );
	SWF_NATIVE_VAR_DECLARE_READONLY( _framesloaded );
	SWF_NATIVE_VAR_DECLARE_READONLY( _droptarget );
	SWF_NATIVE_VAR_DECLARE_READONLY( _url );
	SWF_NATIVE_VAR_DECLARE_READONLY( _highquality );
	SWF_NATIVE_VAR_DECLARE_READONLY( _focusrect );
	SWF_NATIVE_VAR_DECLARE_READONLY( _soundbuftime );
	SWF_NATIVE_VAR_DECLARE_READONLY( _quality );
	SWF_NATIVE_VAR_DECLARE_READONLY( _mousex );
	SWF_NATIVE_VAR_DECLARE_READONLY( _mousey );

	SWF_NATIVE_VAR_DECLARE( _stereoDepth );
	SWF_NATIVE_VAR_DECLARE( _itemindex );

	SWF_NATIVE_VAR_DECLARE( material );
	SWF_NATIVE_VAR_DECLARE( materialWidth );
	SWF_NATIVE_VAR_DECLARE( materialHeight );

	SWF_NATIVE_VAR_DECLARE( xOffset );
	SWF_NATIVE_VAR_DECLARE( onEnterFrame );
	// SWF_NATIVE_VAR_DECLARE( onLoad );
};

extern idSWFScriptObject_SpriteInstancePrototype spriteInstanceScriptObjectPrototype;

#endif
