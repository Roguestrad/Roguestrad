/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2026 Robert Beckebans

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
#ifndef __SWF_H__
#define __SWF_H__

#include "SWF_Enums.h"
#include "SWF_Types.h"
#include "SWF_Bitstream.h"
// RB begin
#include "SWF_File.h"
#include "../libs/lua/src/lua.hpp"
extern "C" {
#include "../libs/luasocket/src/luasocket.h"
}
// RB end

class idSWFShape;
class idSWFSprite;
class idSWFFont;
class idSWFText;
class idSWFEditText;

namespace pugi
{
class xml_node;
class xml_document;
}

/*
================================================
svgSplitContext_t – passed through the recursive SVG export
to control splitting large SVGs into per-sprite sub-files.
================================================
*/
struct svgSplitContext_t {
	bool		 enabled;			 // false = no splitting (backwards compatible)
	idStr		 basePath;			 // e.g. "exported/swf/shell" (without .svg)
	idStr		 filenameWithoutExt; // e.g. "shell" (for image hrefs in defs)
	int			 splitDepth;		 // indent level at which to split (typically 1)
	class idSWF* swf;				 // pointer to idSWF for calling WriteSVGDefs
	bool		 noAnims;			 // propagate noAnims flag
};

/*!
	\class idSWFDictionaryEntry
	\brief A class representing an entry in a SWF dictionary.
*/
class idSWFDictionaryEntry
{
public:
	//! Constructs a new SWF dictionary entry with default values.
	idSWFDictionaryEntry();

	//! Destructor for idSWFDictionaryEntry that cleans up all member objects.
	~idSWFDictionaryEntry();

	//! Assigns the contents of another idSWFDictionaryEntry to this entry and returns a reference to this entry
	idSWFDictionaryEntry& operator=( idSWFDictionaryEntry& other );

	//! Moves the contents of another SWF dictionary entry to this entry.
	idSWFDictionaryEntry& operator=( idSWFDictionaryEntry&& other );

	swfDictType_t		  type;
	const idMaterial*	  material;
	idSWFShape*			  shape;
	idSWFSprite*		  sprite;
	idSWFFont*			  font;
	idSWFText*			  text;
	idSWFEditText*		  edittext;

	idVec2				  svgDisplaySize; // SVG display width/height from <image> attributes (may differ from pixel size)
	idVec2i				  imageSize;
	idVec2i				  imageAtlasOffset;
	// the compressed images are normalize to reduce compression artifacts,
	// color must be scaled down by this
	idVec4				  channelScale;
};

#include "SWF_ScriptVar.h"
#include "SWF_Sprites.h"
#include "SWF_ScriptObject.h"
#include "SWF_ParmList.h"
#include "SWF_ScriptFunction.h"
#include "SWF_SpriteInstance.h"
#include "SWF_ShapeParser.h"
#include "SWF_TextInstance.h"

struct purgableSwfImage_t {
	//! Initializes a purgableSwfImage_t object with null image pointer and zero frame number.
	purgableSwfImage_t()
	{
		image		= NULL;
		swfFrameNum = 0;
	}
	idImage* image;
	unsigned swfFrameNum;
};

/*!
	\class idSWF
	\brief Handles loading, rendering, and execution of SWF-based user interfaces and animations.

	This class manages SWF file operations including loading SWF, JSON, SVG, and binary formats, as well as rendering SWF content with support for animations, sounds, and interactive elements. It
   provides APIs for script variable access, function invocation, event handling, and resource management. The class supports multiple export formats including JSON, SWF, and SVG, and includes
   functionality for parsing and rendering various SWF elements such as shapes, sprites, text, and fonts. It also handles sound playback, random number generation, platform identification, and memory
   management for SWF resources. The class maintains a dictionary of SWF elements and provides methods for hit-testing, rendering states, and display list management.

*/
class idSWF
{
public:
	/*!
		\brief Constructs an idSWF object from a filename, initializing SWF data and optional export settings

		Initializes an SWF object by loading SWF, JSON, or SVG data based on file extensions and provided flags. Sets up materials and resources needed for rendering SWF content, including handling of
	   atlas materials, cursor images, and font resources. Supports optional export of JSON, SWF, and SVG representations of the loaded data.

		\param filename Path to the SWF file, which can be relative or absolute
		\param soundWorld Sound world context for the SWF
		\param exportJSON Flag to control exporting SWF data to JSON format
		\param exportSWF Flag to control exporting SWF data to SWF format
		\param exportSVG Flag to control exporting SWF data to SVG format
		\param noAnims Flag to disable animation exports in SVG
		\param splitSVG Flag to control splitting SVG export into multiple files
	*/
	idSWF( const char* filename, idSoundWorld* soundWorld, bool exportJSON = false, bool exportSWF = false, bool exportSVG = false, bool noAnims = false, bool splitSVG = false );

	//! Destructor for the idSWF class that cleans up allocated resources.
	~idSWF();

	//! Returns true if the SWF file has been successfully loaded and contains valid data.
	bool		IsLoaded() { return ( frameRate > 0 ); }

	//! Returns true if the SWF interface is currently active
	bool		IsActive() { return isActive; }

	//! Activates or deactivates the SWF, rewinding the timeline back to the start when deactivated.
	void		Activate( bool b );

	//! Returns the name of the SWF file.
	const char* GetName() { return filename; }

	//! Pauses the SWF animation by stopping the main sprite instance and setting the paused flag.
	void		Pause()
	{
		mainspriteInstance->Stop();
		paused = true;
	}

	//! Resumes the SWF animation by playing the main sprite instance and setting the paused state to false.
	void Resume()
	{
		mainspriteInstance->Play();
		paused = false;
	}

	//! Returns whether the sound world is currently paused.
	bool			   IsPaused() { return paused; }

	//! Sets the paused rendering state of the SWF.
	void			   SetPausedRender( bool valid ) { pausedRender = valid; }

	//! Returns the current paused rendering state.
	bool			   GetPausedRender() { return pausedRender; }

	//! Renders the SWF GUI element using the provided render system and time parameters.
	void			   Render( idRenderSystem* gui, int time = 0, bool isSplitscreen = false );

	//! Handles system events for the SWF user interface, processing mouse and keyboard input to trigger UI interactions.
	bool			   HandleEvent( const sysEvent_t* event );

	//! Returns whether control inhibition is active for the SWF.
	bool			   InhibitControl();

	//! Sets the control inhibition state for the SWF interface.
	void			   ForceInhibitControl( bool val ) { inhibitControl = val; }

	//! Sets a global variable in the SWF script environment
	void			   SetGlobal( const char* name, const idSWFScriptVar& value );

	//! Registers a native script variable with the global scope of the SWF.
	void			   SetGlobalNative( const char* name, idSWFScriptNativeVariable* native ) { globals->SetNative( name, native ); }

	//! Retrieves a global variable from the SWF script environment by name.
	idSWFScriptVar	   GetGlobal( const char* name ) { return globals->Get( name ); }

	//! Returns the root script object of the SWF instance
	idSWFScriptObject& GetRootObject()
	{
		assert( mainspriteInstance->GetScriptObject() != NULL );
		return *( mainspriteInstance->GetScriptObject() );
	}

	//! Invokes a function by name with the provided parameters on the main sprite instance's script object.
	void				  Invoke( const char* functionName, const idSWFParmList& parms );

	//! Invokes a function by name on a SWF script variable with provided parameters
	void				  Invoke( const char* functionName, const idSWFParmList& parms, idSWFScriptVar& scriptVar );

	//! Invokes a function by name with the provided parameters and indicates if the function exists.
	void				  Invoke( const char* functionName, const idSWFParmList& parms, bool& functionExists );

	//! Plays a sound using the sound system's sound world or returns -1 if inactive or no sound world.
	int					  PlaySound( const char* sound, int channel = SCHANNEL_ANY, bool blocking = false );

	//! Stops sound playback on the specified channel.
	void				  StopSound( int channel = SCHANNEL_ANY );

	//! Returns the width of the SWF frame in pixels.
	float				  GetFrameWidth() const { return frameWidth; }

	//! Returns the height of the SWF frame in pixels.
	float				  GetFrameHeight() const { return frameHeight; }

	//! Returns the x-coordinate of the mouse position within the SWF context
	int					  GetMouseX() { return mouseX; }

	//! Retrieves the current Y coordinate of the mouse pointer within the SWF interface.
	int					  GetMouseY() { return mouseY; }

	//! Returns false to indicate that circle input is not used for accept actions.
	bool				  UseCircleForAccept();

	//! Sets the SWF scale factor to the specified value.
	void				  SetSWFScale( float scale ) { swfScale = scale; }

	//! Sets a flag to force non-PC platform behavior for GetPlatform calls.
	void				  SetForceNonPCGetPlatform() { forceNonPCPlatform = true; }

	//! Returns a reference to the random number generator used by the SWF system.
	idRandom2&			  GetRandom() { return random; }

	//! Returns the platform identifier for SWF functionality.
	int					  GetPlatform();

	//----------------------------------
	// SWF_Dictionary.cpp
	//----------------------------------

	//! Adds a new dictionary entry for the specified character ID and type, returning a pointer to the entry or NULL if a duplicate character ID is encountered.
	idSWFDictionaryEntry* AddDictionaryEntry( int characterID, swfDictType_t type );

	//! Finds and returns a dictionary entry with the specified character ID and type from the SWF dictionary
	idSWFDictionaryEntry* FindDictionaryEntry( int characterID, swfDictType_t type );

	//! Finds and returns a dictionary entry for the specified character ID
	idSWFDictionaryEntry* FindDictionaryEntry( int characterID );

	//! Returns a pointer to the dictionary entry at the specified index.
	idSWFDictionaryEntry* GetDictionaryEntry( int index ) { return &dictionary[index]; }

	//! Returns the number of entries in the SWF dictionary.
	int					  GetNumDictionaryEntry() { return dictionary.Num(); }

	/*!
		\brief Performs a hit test on SWF sprite instances and shapes at the specified coordinates to determine which object is under the cursor.

		This function checks if a given point (x, y) intersects with any visible SWF sprite or shape elements within the specified sprite instance. It recursively traverses the display list of the
	   sprite, checking both nested sprites and shape elements. For shapes, it performs triangle-based hit testing to determine if the point lies within the filled regions. The function returns the
	   last interactive object found during the traversal, or NULL if no interactive elements are found at the specified coordinates. It also considers the visibility and script properties of objects,
	   setting the parentObject for interactive elements.

		\param spriteInstance The sprite instance to test for intersection
		\param renderState The rendering state for transformation calculations
		\param x The x-coordinate of the test point
		\param y The y-coordinate of the test point
		\param parentObject The parent object to associate with interactive elements
		\return The script object representing the last interactive SWF object found at the specified coordinates, or NULL if no object was found.
	*/
	idSWFScriptObject*	  HitTest( idSWFSpriteInstance* spriteInstance, const swfRenderState_t& renderState, int x, int y, idSWFScriptObject* parentObject );

private:
	idStr								  filename;
	ID_TIME_T							  timestamp;

	bool								  isHUD; // Leyland VR

	float								  frameWidth;
	float								  frameHeight;
	uint16								  frameRate;
	float								  renderBorder;
	float								  swfScale;

	idVec2								  scaleToVirtual;

	int									  lastRenderTime;

	bool								  isActive;
	bool								  inhibitControl;
	bool								  useInhibtControl;

	// certain screens need to be rendered when the pause menu is up so if this flag is
	// set on the gui we will allow it to render at a paused state;
	bool								  pausedRender;

	bool								  mouseEnabled;
	bool								  useMouse;

	bool								  blackbars;
	bool								  crop;
	bool								  paused;
	bool								  hasHitObject;

	bool								  forceNonPCPlatform;

	idRandom2							  random;

	static int							  mouseX; // mouse x coord for all flash files
	static int							  mouseY; // mouse y coord for all flash files
	static bool							  isMouseInClientArea;

	idSWFScriptObject*					  mouseObject;
	idSWFScriptObject*					  hoverObject;

	idSWFSprite*						  mainsprite;
	idSWFSpriteInstance*				  mainspriteInstance;

	idSWFScriptObject*					  globals;
	idSWFScriptObject*					  shortcutKeys;

	idSoundWorld*						  soundWorld;

	const idMaterial*					  atlasMaterial;

	idBlockAlloc<idSWFSpriteInstance, 16> spriteInstanceAllocator;
	idBlockAlloc<idSWFTextInstance, 16>	  textInstanceAllocator;

#define SWF_NATIVE_FUNCTION_SWF_DECLARE( x )                                              \
	class idSWFScriptFunction_##x : public idSWFScriptFunction_Nested<idSWF>              \
	{                                                                                     \
	public:                                                                               \
		idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms ); \
	} scriptFunction_##x;

	//! Clears and resets the shortcut key mappings for the SWF interface.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( shortcutKeys_clear );

	//! Deactivates the SWF script object by setting its active state to false.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( deactivate );

	//! Sets the inhibitControl flag based on the first parameter and returns an empty script variable
	SWF_NATIVE_FUNCTION_SWF_DECLARE( inhibitControl );

	//! Sets the use inhibit control flag based on the provided parameter.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( useInhibit );

	//! Pre-caches a sound shader by name and returns its string identifier
	SWF_NATIVE_FUNCTION_SWF_DECLARE( precacheSound );

	//! Plays a sound using the specified sound name and optional channel.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( playSound );

	//! Stops sounds playing on a specified channel or all channels if no channel is specified
	SWF_NATIVE_FUNCTION_SWF_DECLARE( stopSounds );

	//! Returns the platform information associated with the specified SWF script object.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( getPlatform );

	//! Returns the integer value 2, likely representing a platform identifier.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( getTruePlatform );

	//! Returns a localized string value based on the first parameter passed to the function.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( getLocalString );

	//! Returns the result of calling UseCircleForAccept() on the SWF instance.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( swapPS3Buttons );

	//! Retrieves an integer cvar value by its string name from the parameter list.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( getCVarInteger );

	//! Sets a console variable to an integer value using parameters from the SWF script.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( setCVarInteger );

	//! Replaces all occurrences of a substring with another substring in the input string and returns the modified string.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( strReplace );

	//! Calculates the arc cosine of the input parameter and returns the result as a script variable.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( acos );

	//! Calculates the cosine of the first parameter in the parameter list
	SWF_NATIVE_FUNCTION_SWF_DECLARE( cos );

	//! Returns the sine of the first parameter in the parameter list
	SWF_NATIVE_FUNCTION_SWF_DECLARE( sin );

	//! Rounds a floating-point number to the nearest integer
	SWF_NATIVE_FUNCTION_SWF_DECLARE( round );

	//! Computes the power of a value raised to a given exponent using the SWF script function interface
	SWF_NATIVE_FUNCTION_SWF_DECLARE( pow );

	//! Computes the square root of a float value passed as a parameter
	SWF_NATIVE_FUNCTION_SWF_DECLARE( sqrt );

	//! Returns the absolute value of a numeric parameter passed to the function.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( abs );

	//! Returns a random floating-point number within a specified range
	SWF_NATIVE_FUNCTION_SWF_DECLARE( rand );

	//! Returns the floor of a numeric parameter from a SWF script function call
	SWF_NATIVE_FUNCTION_SWF_DECLARE( floor );

	//! Calls the ceiling function on a numeric parameter and returns the result.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( ceil );

	//! Converts a string parameter to uppercase.
	SWF_NATIVE_FUNCTION_SWF_DECLARE( toUpper );

	SWF_NATIVE_VAR_DECLARE_NESTED_READONLY( platform, idSWFScriptFunction_getPlatform, Call( object, idSWFParmList() ) );

	//! Returns the blackbars script variable from the SWF script object

	//! Sets the blackbars state on the specified SWF script object using the provided value
	SWF_NATIVE_VAR_DECLARE_NESTED( blackbars, idSWF );

	//! Returns the crop value stored in the native variable object

	//! Sets the crop property of the SWF script object based on the provided boolean value
	SWF_NATIVE_VAR_DECLARE_NESTED( crop, idSWF );

	/*!
		\class idSWF::idSWFScriptFunction_Object
		\brief The idSWFScriptFunction_Object class represents a script function object within the SWF system, handling function calls and object references.
	*/
	class idSWFScriptFunction_Object : public idSWFScriptFunction
	{
	public:
		//! Calls a script function with the specified object and parameters and returns the result.
		idSWFScriptVar	   Call( idSWFScriptObject* thisObject, const idSWFParmList& parms ) { return idSWFScriptVar(); }

		//! Increments the reference count of the SWF script function object.
		void			   AddRef() { }

		//! Releases the SWF script function object
		void			   Release() { }

		//! Returns the prototype object associated with this script function
		idSWFScriptObject* GetPrototype() { return &object; }

		//! This function is not implemented and always asserts.
		void			   SetPrototype( idSWFScriptObject* _object ) { assert( false ); }
		idSWFScriptObject  object;
	} scriptFunction_Object;

	idList<idSWFDictionaryEntry, TAG_SWF> dictionary;

	struct keyButtonImages_t {
		//! Initializes a new instance of the keyButtonImages_t struct with default values.
		keyButtonImages_t()
		{
			key		 = "";
			xbImage	 = "";
			psImage	 = "";
			width	 = 0;
			height	 = 0;
			baseline = 0;
		}

		/*!
			\brief Initializes a keyButtonImages_t structure with specified key and image paths along with dimensions and baseline.

			This constructor initializes a keyButtonImages_t structure with the provided key, Xbox image path, PlayStation image path, width, height, and baseline values. The key represents the button
		   identifier, while xbImage and psImage represent the image paths for Xbox and PlayStation platforms respectively. The width and height define the dimensions of the button images and baseline
		   specifies the vertical alignment baseline.

			\param _key The button identifier string
			\param _xbImage Path to the Xbox platform image file
			\param _psImage Path to the PlayStation platform image file
			\param w Width of the button images
			\param h Height of the button images
			\param _baseline Vertical baseline alignment for the button images
		*/
		keyButtonImages_t( const char* _key, const char* _xbImage, const char* _psImage, int w, int h, int _baseline )
		{
			key		 = _key;
			xbImage	 = _xbImage;
			psImage	 = _psImage;
			width	 = w;
			height	 = h;
			baseline = _baseline;
		}

		const char* key;
		const char* xbImage;
		const char* psImage;
		int			width;
		int			height;
		int			baseline;
	};
	idList<keyButtonImages_t, TAG_SWF> tooltipButtonImage;

	struct tooltipIcon_t {
		tooltipIcon_t()
		{
			startIndex	= -1;
			endIndex	= -1;
			material	= NULL;
			imageWidth	= 0;
			imageHeight = 0;
			baseline	= 0;
		};

		int				  startIndex;
		int				  endIndex;
		const idMaterial* material;
		short			  imageWidth;
		short			  imageHeight;
		int				  baseline;
	};
	idList<tooltipIcon_t, TAG_SWF> tooltipIconList;

	const idMaterial*			   guiSolid;
	const idMaterial*			   guiCursor_arrow;
	const idMaterial*			   guiCursor_hand;
	const idMaterial*			   white;
	// RB begin
	const idFont*				   debugFont;
	// RB end

private:
	friend class idSWFSprite;
	friend class idSWFSpriteInstance;

	// RB begin
	idHashTableT<idStr, swfColorXform_t> svgFilterColorXforms;

	//! Base directory of the main SVG file, used to resolve sub-file paths.
	//! e.g. "exported/swf/" from "exported/swf/shell.svg"
	idStr								 svgBaseDir;

	//! Sub-documents loaded by LoadSVGSub – kept alive until LoadSVG finishes
	//! so that pugi::xml_node references (e.g. in svgAnimations) remain valid.
	idList<pugi::xml_document*>			 svgSubDocuments;

	// Maps string-based SVG IDs (e.g. "background", "intro_placeholder") to internal
	// numeric dictionary indices.  For engine-exported SVGs the IDs are already numeric
	// and this table is not needed.  For externally authored SVGs this allows <use
	// xlink:href="#intro_placeholder"> to resolve to the correct character ID.
	// When an image has an auto-generated bitmap shape, the name maps to the *shape* ID
	// (not the image ID) so that <use> references work transparently.
	idHashTableT<idStr, int>			 svgNameToCharID;

	//! Loads an SWF file from the specified path
	bool								 LoadSWF( const char* fullpath );

	/*!
		\brief Writes a SWF file with embedded image data from an atlas texture.

		This function creates a SWF file containing bitmap data extracted from an atlas image. It processes each image entry in the SWF dictionary, extracts the relevant portion of the atlas, and
	   compresses it using deflate before writing it to the SWF file. The function also writes metadata and file attributes required for Flash compatibility. Exported images are saved as PNG files in
	   an 'exported' directory.

		\param filename The base filename for the SWF output and exported images
		\param atlasImageRGBA Pointer to the RGBA atlas image data
		\param atlasImageWidth Width of the atlas image in pixels
		\param atlasImageHeight Height of the atlas image in pixels
	*/
	void								 WriteSWF( const char* filename, const byte* atlasImageRGBA, int atlasImageWidth, int atlasImageHeight );

	//! Loads a binary SWF file with the specified filename and source timestamp.
	bool								 LoadBinary( const char* bfilename, ID_TIME_T sourceTime );

	//! Writes the SWF data to a binary file with the specified filename.
	void								 WriteBinary( const char* bfilename );

	//! Sets the bitstream position to skip 5 boolean values for file attributes.
	void								 FileAttributes( idSWFBitStream& bitstream );

	//! Reads and processes the XML metadata string from the provided bitstream.
	void								 Metadata( idSWFBitStream& bitstream );

	//! Sets the background color using data from the provided bitstream.
	void								 SetBackgroundColor( idSWFBitStream& bitstream );

	//! \brief Loads an SVG file and parses its contents into the SWF structure.
	//! \param filename The path to the SVG file to load.
	//! \return true if the file was successfully loaded and parsed, false otherwise.
	bool								 LoadSVG( const char* filename );

	//! \brief Resolves an xlink:href (or href) attribute on an SVG node to a numeric character ID.
	//! Tries xlink:href first, falls back to href.  Strips the leading '#'.
	//! Looks up the name in svgNameToCharID; if not found, falls back to atoi.
	//! \return The resolved numeric character ID, or -1 if the node has no href attribute.
	int									 ResolveSVGHref( const pugi::xml_node& node ) const;

	//! Parses an SVG image node and initializes the dictionary entry with material and display size information.
	void								 ParseSVG_Image( const pugi::xml_node& node, int characterID, idSWFDictionaryEntry& entry );

	//! Parses an SVG shape element from a pugi xml node and populates a SWF shape structure
	void								 ParseSVG_Shape( const pugi::xml_node& node, idSWFShape* shape );

	//! Parses SVG text node attributes and populates the SWF edit text control with parsed values
	void								 ParseSVG_Text( const pugi::xml_node& node, idSWFEditText* et );

	//! Parses an SVG font from an XML node and registers it with the rendering system.
	void								 ParseSVG_Font( const pugi::xml_node& node, idSWFFont* font );

	//! Writes the SWF content to an SVG file with optional animation and splitting support
	void								 WriteSVG( const char* filename, bool noAnims = false, bool splitSVG = false );

	/*!
		\brief Writes the <defs> section containing all dictionary entries to the given file

		This function exports dictionary entries from an SWF file to an SVG definitions block. It handles multiple entry types including images, shapes, morphs, sprites, fonts, and edit text. For
	   images, it writes image elements with proper href attributes. For shapes and morphs, it exports fill and line draws as SVG polygons and polylines. Sprites are skipped in unfolded mode. Font
	   entries are written with associated CSS font face definitions. Edit text entries are processed with alignment and styling information.

		\param file Output file to write the SVG defs
		\param filenameWithoutExt Base filename without extension used for constructing image hrefs
		\param exportUnfolded If true, exports sprites as unfolded elements; if false, skips sprite export
		\param imageHrefPrefix Optional prefix for image href attributes, defaults to filenameWithoutExt if not provided
	*/
	void								 WriteSVGDefs( idFile* file, const char* filenameWithoutExt, bool exportUnfolded, const char* imageHrefPrefix = NULL );

	//! \brief Loads a sub-SVG file exported by the split exporter.
	//! Parses its <defs>, skipping entries that already exist in the dictionary.
	//! Parses its body <g> as a new sprite and adds it to the dictionary.
	//! \return The character ID of the new sprite, or -1 on failure.
	int									 LoadSVGSub(
										 const char* subFilename, idList<idSWFDictionaryEntry>& dict, bool isUnfolded, idHashTableT<idStr, idSWFSprite::svgAnimTarget_t>& svgTargetMap, idList<pugi::xml_node>& svgAnimations );

	//! Loads SWF data from a JSON file
	bool	  LoadJSON( const char* filename );

	//! Writes SWF data to both JSON and Lua format files.
	void	  WriteJSON( const char* filename );

	//----------------------------------
	// SWF_Shapes.cpp
	//----------------------------------

	//! Parses and defines a shape from the provided bitstream data.
	void	  DefineShape( idSWFBitStream& bitstream );

	//! Parses and defines a shape from the provided bitstream with version 2 format.
	void	  DefineShape2( idSWFBitStream& bitstream );

	//! Parses and defines a SWF shape from the provided bitstream.
	void	  DefineShape3( idSWFBitStream& bitstream );

	//! Parses and defines a shape from the provided bitstream with shape version 4.
	void	  DefineShape4( idSWFBitStream& bitstream );

	//! Parses and defines a morph shape from the provided bitstream.
	void	  DefineMorphShape( idSWFBitStream& bitstream );

	//----------------------------------
	// SWF_Sprites.cpp
	//----------------------------------

	//! Parses and loads sprite data from a bitstream into the SWF dictionary.
	void	  DefineSprite( idSWFBitStream& bitstream );

	//----------------------------------
	// SWF_Sounds.cpp
	//----------------------------------

	//! Defines a sound from the provided bitstream data.
	void	  DefineSound( idSWFBitStream& bitstream );

	//----------------------------------
	// SWF_Render.cpp
	//----------------------------------

	/*!
		\brief Renders a textured rectangle using the specified material and texture coordinates, scaled to virtual coordinates.

		This function draws a textured rectangle on the screen using the provided material and texture coordinates. The rectangle is defined by its position (x, y) and dimensions (w, h), while the
	   texture coordinates are specified by s1, t1, s2, and t2. The coordinates are scaled to virtual screen coordinates using the scaleToVirtual member variables before being passed to the render
	   system.

		\param x X coordinate of the rectangle's top-left corner
		\param y Y coordinate of the rectangle's top-left corner
		\param w Width of the rectangle
		\param h Height of the rectangle
		\param s1 First texture coordinate for the S axis
		\param t1 First texture coordinate for the T axis
		\param s2 Second texture coordinate for the S axis
		\param t2 Second texture coordinate for the T axis
		\param material Material to use for rendering the textured rectangle
	*/
	void	  DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material );

	/*!
		\brief Renders a textured quad with specified corner positions and texture coordinates using the given material

		This function draws a textured quad on screen by specifying the four corner positions with their respective texture coordinates. The corner positions are defined by idVec4 values where x and y
	   represent screen coordinates, z represents texture coordinate s, and w represents texture coordinate t. The function internally scales the screen coordinates based on a virtual scaling factor
	   before passing the data to the rendering system. The material parameter determines the texture and rendering properties applied to the quad.

		\param topLeft Position and texture coordinates for the top-left corner (x, y screen coords, z s texture coord, w t texture coord)
		\param topRight Position and texture coordinates for the top-right corner (x, y screen coords, z s texture coord, w t texture coord)
		\param bottomRight Position and texture coordinates for the bottom-right corner (x, y screen coords, z s texture coord, w t texture coord)
		\param bottomLeft Position and texture coordinates for the bottom-left corner (x, y screen coords, z s texture coord, w t texture coord)
		\param material Material to use for rendering the quad, determining texture and rendering properties
	*/
	void	  DrawStretchPic( const idVec4& topLeft, const idVec4& topRight, const idVec4& bottomRight, const idVec4& bottomLeft, const idMaterial* material );

	/*!
		\brief Renders a SWF sprite with the specified render state and time, handling visibility, alpha blending, masking, and stereo depth effects.

		This function processes a SWF sprite instance for rendering by applying the provided render state, handling display list entries, managing active masks, and applying transformations. It
	   supports stereo depth rendering, material overrides, and alignment adjustments based on sprite names. The function ensures proper rendering of sprites while handling edge cases such as
	   invisible sprites, alpha testing, and mask cleanup.

		\param gui Pointer to the render system used for rendering
		\param sprite Pointer to the SWF sprite instance to render
		\param renderState Reference to the render state containing transformation and blend information
		\param time Time value used for rendering operations
		\param isSplitscreen Boolean flag indicating if the rendering is for splitscreen mode
	*/
	void	  RenderSprite( idRenderSystem* gui, idSWFSpriteInstance* sprite, const swfRenderState_t& renderState, int time, bool isSplitscreen = false );

	/*!
		\brief Renders a mask for SWF GUI elements using the provided render state and stencil mode

		This function processes a mask entry from the SWF dictionary and renders it using either a shape or morph shape rendering method. The function sets up a new render state based on the mask's
	   properties and the provided render state, then delegates to the appropriate rendering function based on the dictionary entry type. The stencil mode controls how the mask is applied during
	   rendering.

		\param gui Render system to use for rendering
		\param mask Display entry containing mask properties and character ID
		\param renderState Current render state to be modified and used for rendering
		\param stencilMode Stencil buffer mode to control mask application
	*/
	void	  RenderMask( idRenderSystem* gui, const swfDisplayEntry_t* mask, const swfRenderState_t& renderState, const int stencilMode );

	//! Renders a SWF shape using the provided GUI render system and rendering state
	void	  RenderShape( idRenderSystem* gui, const idSWFShape* shape, const swfRenderState_t& renderState );

	//! Renders a morph shape using the provided GUI render system and rendering state
	void	  RenderMorphShape( idRenderSystem* gui, const idSWFShape* shape, const swfRenderState_t& renderState );

	/*!
		\brief Draws an edit cursor on the GUI using the provided dimensions and transformation matrix

		This function transforms the corners of a rectangular cursor area using the provided matrix and then draws a textured quad to represent the cursor. The cursor is drawn using the white color
	   and the matrix defines both the position and orientation of the cursor on screen.

		\param gui Pointer to the render system for drawing operations
		\param x X coordinate of the cursor rectangle
		\param y Y coordinate of the cursor rectangle
		\param w Width of the cursor rectangle
		\param h Height of the cursor rectangle
		\param matrix Transformation matrix for positioning and orienting the cursor
	*/
	void	  DrawEditCursor( idRenderSystem* gui, float x, float y, float w, float h, const swfMatrix_t& matrix );
	void	  DrawLine( idRenderSystem* gui, const idVec2& p1, const idVec2& p2, float width, const swfMatrix_t& matrix );

	/*!
		\brief Renders editable text elements for SWF GUI systems with support for various text modes and cursor handling.

		This function handles the rendering of editable text instances within the SWF GUI system. It processes text content from either static strings or dynamic variables, applies appropriate font
	   scaling and transformations, and manages cursor visibility and selection highlighting. The function supports multiple rendering modes including paragraph, random appearance, and standard text
	   rendering. It also handles split-screen display adjustments, tooltip icon integration, and subtitle text rendering. The function performs validation checks for null instances and visibility
	   flags, and manages sound playback for text elements.

		\param gui Render system interface for drawing operations
		\param textInstance Pointer to the text instance being rendered
		\param renderState Current rendering state including transform matrix and color information
		\param time Current time value for animation and timing
		\param isSplitscreen Flag indicating whether to apply split-screen adjustments to text rendering
	*/
	void	  RenderEditText( idRenderSystem* gui, idSWFTextInstance* textInstance, const swfRenderState_t& renderState, int time, bool isSplitscreen = false );

	//! Converts a SWF render state into a GL state configuration for rendering.
	uint64	  GLStateForRenderState( const swfRenderState_t& renderState );

	//! Replaces text placeholders with platform-specific button icons and text
	void	  FindTooltipIcons( idStr* text );

	//! Calculates the bounding rectangle for a sprite instance based on its display list and render state.
	swfRect_t CalcRect( const idSWFSpriteInstance* sprite, const swfRenderState_t& renderState );

	//! Draws a rectangular outline using the specified render system, rectangle dimensions, and color.
	void	  DrawRect( idRenderSystem* gui, const swfRect_t& rect, const idVec4& color );

	/*!
		\brief Draws text using the SWF system with specified position, scale, color, and formatting options

		This function renders text using the SWF (Shockwave Flash) system within the Roguestrad engine. It takes a text string and renders it at the specified coordinates with the given scale and
	   color. The function handles Unicode characters through UTF-8 decoding and supports text color codes embedded within the text. It uses a debug font system to retrieve glyph information for
	   rendering and creates textured quads for each character. The function also supports limiting the text length and adjusting character spacing. The rendering is performed through the provided
	   render system interface, and it returns the number of characters drawn.

		\param gui Render system interface used for drawing operations
		\param x X coordinate for text position
		\param y Y coordinate for text position
		\param scale Scaling factor for text size
		\param color RGBA color values for text rendering
		\param text UTF-8 encoded text string to be drawn
		\param adjust Adjustment value for character spacing
		\param limit Maximum number of characters to draw, 0 for no limit
		\param style Text style identifier
		\return Number of characters successfully drawn
	*/
	int		  DrawText( idRenderSystem* gui, float x, float y, float scale, idVec4 color, const char* text, float adjust, int limit, int style );

	/*!
		\brief Draws text within a specified rectangle using the SWF system, handling word wrapping and cursor positioning

		This function renders text within a given rectangular area, supporting features such as word wrapping, text alignment, color formatting, and cursor positioning. It calculates text metrics and
	   determines how many characters can fit within the given rectangle. The function supports both drawing the text and calculating text metrics without rendering. It handles special cases like
	   newlines, escape sequences for colors, and breaks text at appropriate word boundaries when wrapping is enabled. The function also manages breaking points for text rendering and cursor
	   positioning within the text block.

		\param gui Render system interface for drawing operations
		\param text Text string to be rendered
		\param textScale Scaling factor for text size
		\param textAlign Text alignment value (0=left, 1=center, 2=right)
		\param color Color of the text as RGBA values
		\param rectDraw Rectangle defining the drawing area
		\param wrap Whether to wrap text at word boundaries
		\param cursor Position of cursor within the text (-1 for none)
		\param calcOnly If true, only calculate metrics without drawing
		\param breaks Optional list to store break positions in text
		\param limit Maximum number of characters to process (0 for unlimited)
		\return Number of characters that could fit in the specified rectangle based on the provided text scale
	*/
	int		  DrawText( idRenderSystem* gui,
			  const char*				text,
			  float						textScale,
			  int						textAlign,
			  idVec4					color,
			  const swfRect_t&			rectDraw,
			  bool						wrap,
			  int						cursor	 = -1,
			  bool						calcOnly = false,
			  idList<int>*				breaks	 = NULL,
			  int						limit	 = 0 );
	//----------------------------------
	// SWF_Image.cpp
	//----------------------------------

	/*!
		\class idSWF::idDecompressJPEG
		\brief A class for decompressing JPEG images in SWF format.
	*/
	class idDecompressJPEG
	{
	public:
		//! Initializes a JPEG decompression object for SWF image processing.
		idDecompressJPEG();

		//! Destroys the JPEG decompression object and cleans up associated resources.
		~idDecompressJPEG();

		/*!
			\brief Decompresses a JPEG image from memory and returns the raw RGBA pixel data.

			This function loads a JPEG image from a byte buffer in memory, decompresses it, and returns the pixel data in RGBA format. The function supports both libjpeg and stb_image backends
		   depending on compilation flags. If the JPEG is not in RGBA format, it warns and returns NULL. The width and height parameters are updated with the dimensions of the decompressed image.

			\param input Pointer to the byte buffer containing the JPEG data
			\param inputSize Size of the input buffer in bytes
			\param width Reference to an integer that will be set to the width of the decompressed image
			\param height Reference to an integer that will be set to the height of the decompressed image
			\return Pointer to the decompressed RGBA pixel data, or NULL if decompression fails
			\throws May throw exceptions from idException handler in libjpeg path, or from Mem_Alloc or stbi_load_from_memory in fallback path
		*/
		byte* Load( const byte* input, int inputSize, int& width, int& height );

	private:
		void* vinfo;
	};

	idDecompressJPEG jpeg;

	/*!
		\brief Packs image data for a SWF character into a temporary storage for later atlas allocation

		This function prepares image data for inclusion in a texture atlas by allocating memory for the image, computing appropriate allocation sizes based on DXT block alignment, and adding a border
	   around the image to prevent edge bleeding. The function stores the image data temporarily in a global list for later processing during atlas creation.

		\param characterID The unique identifier for the SWF character this image belongs to
		\param imageData Pointer to the raw image data to be packed
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void			 PackImage( int characterID, const byte* imageData, int width, int height );

	//! Reads JPEG table data from the provided bitstream for SWF file processing.
	void			 JPEGTables( idSWFBitStream& bitstream );

	//! Reads a partial JPEG image from the bitstream and loads it into the SWF
	void			 DefineBits( idSWFBitStream& bitstream );

	//! Decodes a JPEG image from the bitstream and stores it with the specified character ID.
	void			 DefineBitsJPEG2( idSWFBitStream& bitstream );

	//! Processes a DefineBitsJPEG3 SWF tag to load JPEG image data with an additional zlib compressed alpha map.
	void			 DefineBitsJPEG3( idSWFBitStream& bitstream );

	//! Parses and processes lossless bitmap data from a SWF bitstream to create an image character.
	void			 DefineBitsLossless( idSWFBitStream& bitstream );

	//! Parses and processes lossless bitmap data from a SWF bitstream
	void			 DefineBitsLossless2( idSWFBitStream& bitstream );

	// per-swf image atlas
	struct imageToPack_t {
		int		characterID;
		idVec2i trueSize;  // in texels
		byte*	imageData; // trueSize.x * trueSize.y * 4
		idVec2i allocSize; // in DXT tiles, includes a border texel and rounding up to DXT blocks
	};

	/*!
		\class idSWF::idSortBlocks
		\brief A sorting implementation for image packing blocks based on allocated size.
	*/
	class idSortBlocks : public idSort_Quick<imageToPack_t, idSortBlocks>
	{
	public:
		//! Compares two imageToPack_t structures based on their allocated sizes
		int Compare( const imageToPack_t& a, const imageToPack_t& b ) const { return ( b.allocSize.x * b.allocSize.y ) - ( a.allocSize.x * a.allocSize.y ); }
	};

	idList<imageToPack_t, TAG_SWF> packImages; // only used during creation

	//! Writes SWF image atlas data to a file
	void						   WriteSwfImageAtlas( const char* filename );

	//----------------------------------
	// SWF_Text.cpp
	//----------------------------------

	//! Parses and defines a SWF font from the provided bitstream data
	void						   DefineFont2( idSWFBitStream& bitstream );

	//! Defines a font version 3 using the provided bitstream.
	void						   DefineFont3( idSWFBitStream& bitstream );

	//! Parses and defines a text element from a SWF bitstream with optional RGBA color support.
	void						   DefineTextX( idSWFBitStream& bitstream, bool rgba );

	//! Defines a text element using the provided bitstream.
	void						   DefineText( idSWFBitStream& bitstream );

	//! Parses and defines a text element from the SWF bitstream with additional formatting options.
	void						   DefineText2( idSWFBitStream& bitstream );

	//! Parses and defines an edit text element from a SWF bitstream
	void						   DefineEditText( idSWFBitStream& bitstream );

	//----------------------------------
	// SWF_Zlib.cpp
	//----------------------------------

	/*!
		\brief Decompresses input data using zlib inflate algorithm into the provided output buffer

		This function performs decompression of zlib compressed data. It initializes a zlib stream with custom memory allocation functions, calls the inflate function to decompress the input data, and
	   then cleans up the stream. The function returns true if decompression was successful and the entire input stream was consumed, false otherwise. This is used primarily for decompressing SWF
	   image data such as JPEG alpha channels and lossless bitmap data.

		\param input Pointer to the compressed input data
		\param inputSize Size of the input data in bytes
		\param output Pointer to the output buffer where decompressed data will be written
		\param outputSize Size of the output buffer in bytes
		\return True if decompression succeeded and the entire input was processed, false otherwise
	*/
	bool						   Inflate( const byte* input, int inputSize, byte* output, int outputSize );

	/*!
		\brief Compresses input data using zlib deflate algorithm and stores the result in the output buffer

		This function performs compression of input data using the zlib deflate algorithm. It takes an input buffer and its size, and attempts to compress it into an output buffer. The function
	   handles memory allocation and deallocation internally using custom allocation functions. The compression is performed with default compression level. The actual output size is written back to
	   the outputSize parameter after compression. The function returns true if compression was successful and reached the end of the stream, false otherwise.

		\param input Pointer to the input data to be compressed
		\param inputSize Size of the input data in bytes
		\param output Pointer to the buffer where compressed data will be stored
		\param outputSize Reference to variable containing the size of the output buffer, updated with actual compressed size
		\return True if compression was successful and completed, false if compression failed
	*/
	bool						   Deflate( const byte* input, int inputSize, byte* output, int& outputSize );
	// RB end

public:
	//----------------------------------
	// SWF_Names.cpp
	//----------------------------------

	//! Returns the string name of a given SWF dictionary type.
	static const char* GetDictTypeName( swfDictType_t type );

	//! Returns the string name of the given SWF edit text alignment value.
	static const char* GetEditTextAlignName( swfEditTextAlign_t align );

	//! Returns the name of the font associated with the given font ID
	const char*		   GetFontName( int fontID );

	//! Returns the name of the blend mode corresponding to the given blend mode identifier.
	const char*		   GetBlendModeName( uint8 blendMode );

	//! Returns the string name of a SWF tag constant
	static const char* GetTagName( swfTag_t tag );

	//! Returns the name of the given SWF action as a string
	static const char* GetActionName( swfAction_t action );

	// RB: LUA INTEGRATION ---------------------------

	/*!
		\brief Custom memory allocator function for Lua integration that handles memory allocation and reallocation while using the engine's memory management system

		This function serves as a custom allocator for Lua's garbage collector, implementing memory management using the engine's Mem_Alloc and Mem_Free functions. When deallocating memory (nsize is
	   zero), it calls Mem_Free directly. When allocating or resizing memory, it allocates new memory with the specified tag, copies existing data if present, and frees the old memory. The function
	   uses SIMDProcessor for memory copying when resizing memory blocks, ensuring efficient data transfer during reallocations.

		\param ud User data pointer passed to Lua allocator
		\param ptr Pointer to existing memory block, or NULL if allocating new memory
		\param osize Size of existing memory block in bytes
		\param nsize Requested new size in bytes, or 0 if deallocating
		\return Pointer to the new memory block, or NULL if deallocation was requested
	*/
	static void*	   LuaAlloc( void* ud, void* ptr, size_t osize, size_t nsize );

	//! Handles Lua API panic errors by reporting them and terminating execution.
	static int		   LuaPanic( lua_State* L );

public:
	//! Returns the Lua state associated with this SWF.
	lua_State* GetLuaState() const { return luaState; }

private:
	//! Handles Lua script function calls from SWF animations by converting Lua parameters to Flash parameters and executing corresponding SWF functions
	static int					LuaNativeScriptFunctionCall( lua_State* L );

	//! Handles Lua global variable callback invocation and debugging output
	static int					LuaGlobalVarCallback( lua_State* L );
	static idSWFSpriteInstance* luaSpriteInstance;

	//! Sets the Lua sprite instance for the SWF.
	static inline void			SetLuaSpriteInstance( idSWFSpriteInstance* spriteInstance ) { luaSpriteInstance = spriteInstance; }

	lua_State*					luaState;
	// RB end
};

//! Prints the current Lua stack contents to the console.
void lua_printstack( lua_State* L );

#endif // !__SWF_H__
