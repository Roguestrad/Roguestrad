/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2015 Robert Beckebans

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
#ifndef __SWF_SPRITES_H__
#define __SWF_SPRITES_H__

// TODO: move rapidjson include to cpp so this does not pollute into precompiled.h
#undef Bool
#include "rapidjson/document.h"

namespace pugi
{
class xml_node;
}

struct svgSplitContext_t;

//! Returns the CSS color name for the given RGBA color value, or a fallback rgba() string if no match is found.
const char* cssNameFromRGBA( const swfColorRGBA_t& col );

/*!
	\class idSWFSprite
	\brief A class representing a sprite within a SWF file, handling loading, writing, and conversion between SWF and SVG formats.

	This class manages sprite data within a SWF file, supporting operations such as loading from bitstreams and files, writing to various formats including SWF, SVG, and JSON, and converting between
   these formats. It handles frame information, command buffers, initialization actions, and sprite-specific data including character placement, removal, and animation. The class supports recursive
   processing of SVG nodes to build SWF commands, manages animation targets and frame labels, and provides functionality for exporting sprite timelines to SVG format. It also includes methods for
   parsing and writing SWF tags in JSON format, facilitating integration with external tools and systems.

   What the swf file format calls a "sprite" is known as a "movie clip" in Flash.
   There is one main sprite, and many many sub-sprites. Only the main sprite is allowed to add things to the dictionary

*/
class idSWFSprite
{
public:
	//! Constructs an idSWFSprite object with the specified SWF parent
	idSWFSprite( class idSWF* swf );

	//! Destructor for the idSWFSprite class that releases the allocated command buffer memory.
	~idSWFSprite();

	//! Loads sprite data from a bitstream, optionally parsing definition tags into a dictionary.
	void Load( idSWFBitStream& bitstream, bool parseDictionary );

	//! Reads sprite data from a file including frame information, command buffers, and initialization actions.
	void Read( idFile* f );

	//! Writes sprite data to a file including frame counts, offsets, labels, and command streams.
	void Write( idFile* f );

	//! Reads sprite data from a JSON entry including frame counts, offsets, labels, and commands.
	void ReadJSON( rapidjson::Value& entry );

	//! Writes sprite data in JSON format to the specified file
	void WriteJSON( idFile* f, idFile* luaFile, int characterID );

	struct parsedAnim_t {
		int			  depth;
		idList<idStr> valueList;
		idList<idStr> mulColorList; // full R,G,B,A mul-color per frame for roundtrip fidelity
		idStr		  attributeName;
		bool		  isTransform;
		idStr		  transformType;
		bool		  isAdditive;
	};

	struct svgAnimTarget_t {
		idSWFSprite*		 owner;
		int					 depth;
		idList<parsedAnim_t> parsedAnims;
	};

	/*!
		\brief Recursively loads and processes SVG nodes, building SWF commands and animation data.

		This function recursively processes XML nodes from an SVG structure, handling various node types such as 'use', 'filter', and 'g'. It builds SWF commands for placing objects, manages animation
	   targets, processes filters, and handles frame labels and Lua markers. The function manages character IDs, depth counters, and frame offsets while constructing SWF commands from SVG elements. It
	   also handles data imports by loading sub-SVG files and merges their content into the current sprite. For each 'use' node, it creates deferred commands that are later resolved during the SWF
	   compilation process.

		\param node The current XML node being processed
		\param dict Dictionary containing SWF dictionary entries
		\param isUnfolded Flag indicating whether the SVG is unfolded
		\param targetMap Map of animation targets for SVG elements
		\param animations List of animation nodes to be processed
	*/
	void		LoadSVGNode_r( const pugi::xml_node& node, idList<idSWFDictionaryEntry>& dict, bool isUnfolded, idHashTableT<idStr, svgAnimTarget_t>& targetMap, idList<pugi::xml_node>& animations );

	/*!
		\brief Parses SVG animation nodes into parsed animation entries for each target

		Iterates all collected animation nodes and populates parsedAnims on each matching svgAnimTarget_t in the global target map.
		Call this once after all LoadSVGNode_r calls have finished, before ApplySVGAnimationTargets.
	*/
	static void ParseSVGAnimations( idHashTableT<idStr, svgAnimTarget_t>& targetMap, const idList<pugi::xml_node>& animations );

	/*!
		\brief Processes parsed animation data to build SWF frame commands for this sprite.

		Builds SWF frame commands for this sprite from pre-parsed animation data.
		parsedAnims must only contain entries whose depth belongs to this sprite
		(i.e. collected from svgAnimTarget_t entries where owner == this).
	*/
	void		ApplySVGAnimationTargets( const idList<parsedAnim_t>& parsedAnims );

	//! Writes an SVG representation of a sprite to the specified file.
	void		WriteSVG( idFile* f, int characterID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict );

	/*!
		\brief Writes SVG representation of a SWF sprite's unfolded timeline to a file

		This function recursively processes SWF sprite commands to generate an SVG representation of the sprite's animation timeline. It simulates the timeline by processing commands at each frame,
	   tracks object placement and removal, and exports the resulting visual structure to SVG format. The function handles different SWF tag types such as PlaceObject2, PlaceObject3, RemoveObject2,
	   DoAction, and DoLua. It also supports animation export, frame labels, and proper SVG group hierarchy with indentation. The function maintains a local depth map to track which objects exist at
	   each frame and handles removals by creating SVG animate elements to hide objects at specific times.

		\param f Output file to write the SVG data to
		\param characterID Identifier of the character/sprite being exported
		\param dict Dictionary containing SWF sprite entries
		\param characterMap Map of character IDs to SVG display entries
		\param frameDur Duration of each frame in seconds
		\param prefix String prefix to use for SVG identifiers
		\param indent Number of tab characters to indent the output
		\param writeGroupTag Whether to write an SVG group tag at the beginning
		\param noAnims If true, skips animation export
		\param splitCtx Context for splitting SVG elements, can be NULL
	*/
	void		WriteSVGUnfolded_r( idFile*					f,
			   int											characterID,
			   const idList<idSWFDictionaryEntry, TAG_SWF>& dict,
			   idHashTableT<int, svgDisplayEntry_t>&		characterMap,
			   float										frameDur,
			   const idStr&									prefix,
			   int											indent,
			   bool											writeGroupTag,
			   bool											noAnims	 = false,
			   const svgSplitContext_t*						splitCtx = NULL );

private:
	/*!
		\brief Emits a place character command for SWF animation with specified parameters

		This function generates a place character command for SWF animation processing. It handles various animation properties such as blend mode, transformation matrices, color transformations, and
	   ratio settings. The function uses an XML node to extract animation attributes and constructs appropriate SWF tag data based on the found properties. It also manages depth counter tracking and
	   registers animation targets for later use in the animation system.

		\param s XML node containing animation properties
		\param newCharID ID of the character to place
		\param currentFrame Current frame number for animation
		\param depthCounter Reference to depth counter that gets updated
		\param targetMap Hash table mapping animation targets to their properties
	*/
	void EmitPlaceCharacter( const pugi::xml_node& s, int newCharID, int currentFrame, int& depthCounter, idHashTableT<idStr, svgAnimTarget_t>& targetMap );

	/*!
		\brief Writes a SWF PlaceObject2 tag as JSON to the specified file.

		This function serializes a SWF PlaceObject2 tag into JSON format, including flags, depth, character ID, matrix, color transform, ratio, name, clip depth, and other optional fields. The output
	   is formatted with the provided indentation prefix.

		\param f The file to write the JSON output to
		\param luaFile Unused parameter, likely for future Lua integration
		\param bitstream The bitstream containing the SWF tag data to parse
		\param characterID The source character ID for the tag
		\param commandID The command ID used for formatting the JSON output
		\param indentPrefix The indentation prefix string to use for JSON formatting
	*/
	void WriteJSON_PlaceObject2( idFile* f, idFile* luaFile, idSWFBitStream& bitstream, int characterID, int commandID, const char* indentPrefix = "" );

	/*!
		\brief Writes a PlaceObject3 tag in JSON format to the specified file

		This function processes a PlaceObject3 tag from a SWF file and writes its serialized data to a JSON output file. It reads various flags to determine which properties are present in the tag,
	   including character ID, matrix, color transform, ratio, name, clip depth, filter list, blend mode, and clip actions. The function handles the parsing of these properties and outputs them in a
	   structured JSON format. The character ID is only read if the appropriate flag is set in the flags1 field. The function does not support filter lists, blend modes, or clip actions, though it
	   acknowledges their presence in the SWF stream.

		\param f Output file to write the JSON data to
		\param luaFile Unused parameter in current implementation
		\param bitstream Input bitstream containing the SWF tag data
		\param characterID Source character ID, not directly used in output
		\param commandID Command ID for formatting purposes
		\param indentPrefix Indentation prefix for JSON formatting
	*/
	void WriteJSON_PlaceObject3( idFile* f, idFile* luaFile, idSWFBitStream& bitstream, int characterID, int commandID, const char* indentPrefix = "" );

	/*!
		\brief Writes a JSON representation of a RemoveObject2 tag to the specified file.

		This function reads the depth value from the bitstream and writes a formatted JSON object to the provided file. The JSON object describes a RemoveObject2 tag with its type and depth. If the
	   commandID is non-zero, a comma and newline are prepended to the output for proper JSON formatting. An optional indent prefix can be provided for formatting.

		\param f The file to write the JSON output to
		\param luaFile A secondary file handle, possibly for Lua-related data (purpose is unclear from implementation)
		\param bitstream The bitstream containing the encoded RemoveObject2 data
		\param characterID TODO: clarify the purpose of this parameter as it's not used in the implementation
		\param commandID Used to determine if a comma and newline should be prepended for JSON formatting
		\param indentPrefix Optional indentation prefix for proper JSON formatting
	*/
	void WriteJSON_RemoveObject2( idFile* f, idFile* luaFile, idSWFBitStream& bitstream, int characterID, int commandID, const char* indentPrefix = "" );

	/*!
		\brief Writes a DoAction tag in JSON format to the specified file, converting the action data to Lua code when possible.

		This function encodes the binary action data from the bitstream using base64 encoding and writes it to the provided file in a JSON format. If the encoded data matches a specific pattern, it
	   writes a special Tag_DoLua entry with a 'just_stop' function. Otherwise, it creates a script object and function to convert the action data into readable Lua code, which is then written to the
	   Lua file. The function also outputs a Tag_DoLua entry referencing the generated Lua function.

		\param f Output file for the JSON representation of the DoAction tag
		\param luaFile Output file for the generated Lua code
		\param bitstream Bitstream containing the action data to be processed
		\param characterID ID of the character associated with this action
		\param commandID ID of the command associated with this action
		\param indentPrefix String to be used as indentation prefix in the output JSON
	*/
	void WriteJSON_DoAction( idFile* f, idFile* luaFile, idSWFBitStream& bitstream, int characterID, int commandID, const char* indentPrefix = "" );

	/*!
		\brief Writes a DoLua tag in JSON format to the specified file

		This function writes a DoLua tag to a file in JSON format, including the tag type and the Lua function name. The function extracts the Lua function string from the bitstream and formats it
	   into a JSON object with proper indentation. The output includes the character ID and command ID for reference, and handles the comma separator for multiple tags.

		\param f Output file to write the JSON tag to
		\param luaFile Unused parameter in current implementation
		\param bitstream Bitstream containing the Lua function data
		\param characterID ID of the character this tag belongs to
		\param commandID ID of the command this tag represents
		\param indentPrefix Indentation prefix for formatting the JSON output
	*/
	void WriteJSON_DoLua( idFile* f, idFile* luaFile, idSWFBitStream& bitstream, int characterID, int commandID, const char* indentPrefix = "" );

	/*!
		\brief Writes an SVG representation of a PlaceObject2 tag to the given file

		This function processes a PlaceObject2 SWF tag and writes the corresponding SVG elements to the output file. It reads flags from the bitstream to determine which properties are present, and
	   then writes appropriate SVG attributes including transform matrices, color transformations, and other properties. The function handles different dictionary entry types such as shapes, text, and
	   sprites by writing appropriate SVG <use> elements with href attributes pointing to the character ID.

		\param f Output file to write the SVG content to
		\param bitstream Bitstream containing the SWF tag data
		\param characterID ID of the character being placed
		\param commandID ID of the command being processed
		\param dict Dictionary containing character definitions
	*/
	void WriteSVG_PlaceObject2( idFile* f, idSWFBitStream& bitstream, int characterID, int commandID, const idList<idSWFDictionaryEntry, TAG_SWF>& dict );

	/*!
		\brief Processes PlaceObject2 and PlaceObject3 SWF tags to manage sprite placement and animation frame data.

		This function handles the preprocessing of SWF PlaceObject2 and PlaceObject3 tags during timeline simulation. It reads various flags and data from the bitstream to determine what properties of
	   a sprite to set or update. The function manages both new sprite entries and updates to existing ones, storing animation frame data for matrices, opacities, and color transformations. It ensures
	   that each depth level gets a unique identifier for proper SVG rendering and animation tracking.

		\param tag The SWF tag type, either Tag_PlaceObject2 or Tag_PlaceObject3
		\param bitstream The bitstream containing the SWF tag data to be parsed
		\param sourceCharacterID The ID of the source character being placed
		\param sourcePrefix A prefix to be prepended to sprite names for unique identification
		\param commandID The index of the command within the timeline
		\param dict The SWF dictionary used for character lookups
		\param characterMap A hash table mapping character IDs to display entries
		\param localDepthMap A hash table mapping depth levels to display entries
		\param currentFrame The current frame number for animation frame tracking
	*/
	void PreRun_PlaceObject2_3( swfTag_t			 tag,
		idSWFBitStream&								 bitstream,
		int											 sourceCharacterID,
		const idStr&								 sourcePrefix,
		int											 commandID,
		const idList<idSWFDictionaryEntry, TAG_SWF>& dict,
		idHashTableT<int, svgDisplayEntry_t>&		 characterMap,
		idHashTableT<int, svgDisplayEntry_t*>&		 localDepthMap,
		int											 currentFrame );

	/*!
		\brief Writes SVG representation for SWF PlaceObject2 and PlaceObject3 tags, handling character placement, transformations, and animations

		This function processes SWF PlaceObject2 and PlaceObject3 tags to generate SVG output. It reads placement flags and data from the bitstream, handles matrix transformations, color
	   transformations, and blend modes. The function supports animation detection and properly formats SVG elements including use tags for shapes and sprites. It also manages clipping depths and
	   filters for complex visual effects.

		\param tag SWF tag identifier indicating PlaceObject2 or PlaceObject3
		\param f Output file handle for writing SVG data
		\param bitstream Bitstream containing SWF tag data
		\param sourceCharacterID ID of the source character being processed
		\param sourcePrefix Prefix for identifying the source
		\param commandID Unique identifier for the command within the sprite
		\param dict Dictionary containing SWF character definitions
		\param characterMap Map linking character IDs to SVG display entries
		\param localDepthMap Map for tracking display depths
		\param currentFrame Current frame number for animation tracking
		\param frameDur Duration of the current frame
		\param indent Indentation level for SVG formatting
		\param noAnims Flag to disable animation handling
		\param splitCtx Context for splitting SVG output
	*/
	void WriteSVGUnfolded_PlaceObject2_3( swfTag_t	 tag,
		idFile*										 f,
		idSWFBitStream&								 bitstream,
		int											 sourceCharacterID,
		const idStr&								 sourcePrefix,
		int											 commandID,
		const idList<idSWFDictionaryEntry, TAG_SWF>& dict,
		idHashTableT<int, svgDisplayEntry_t>&		 characterMap,
		idHashTableT<int, svgDisplayEntry_t*>&		 localDepthMap,
		int											 currentFrame,
		float										 frameDur,
		int											 indent,
		bool										 noAnims  = false,
		const svgSplitContext_t*					 splitCtx = NULL );

	/*!
		\brief Writes an SVG unfolded DoLua tag to the specified file with formatted indentation and Lua script data.

		This function generates an SVG element representing a DoLua command with associated Lua script information and writes it to the provided file. The output includes indentation, character ID,
	   command ID, frame duration, and a prefix for scoping. The Lua script content is extracted from the bitstream and embedded within the SVG tag.

		\param f Output file to write the SVG tag
		\param bitstream Bitstream containing the Lua script data
		\param characterID ID of the character associated with this command
		\param commandID ID of the command being processed
		\param frameDur Duration of each frame in seconds
		\param currentFrame Current frame number for timing
		\param prefix Scope prefix for the Lua function
		\param indent Number of tab characters for indentation
	*/
	void WriteSVGUnfolded_DoLua( idFile* f, idSWFBitStream& bitstream, int characterID, int commandID, float frameDur, int currentFrame, const idStr& prefix, int indent );

	/*!
		\brief Writes an SVG representation of a DoAction tag for a SWF sprite.

		This function generates SVG markup for a SWF DoAction tag, writing it to the provided file. It encodes the bitstream data using base64 and determines a function name based on the encoded data.
	   The output includes metadata such as the tag type, Lua function name, scope, and trigger time.

		\param f Output file to write the SVG markup
		\param bitstream Bitstream containing the action data to encode and write
		\param characterID ID of the character this action belongs to
		\param commandID ID of the command within the character
		\param frameDur Duration of each frame in seconds
		\param currentFrame Current frame number to calculate trigger time
		\param prefix Prefix string for the Lua scope
		\param indent Number of tab characters to indent the output
	*/
	void WriteSVGUnfolded_DoAction( idFile* f, idSWFBitStream& bitstream, int characterID, int commandID, float frameDur, int currentFrame, const idStr& prefix, int indent );

public:
	//! Writes sprite data to a SWF file including frame commands and initialization actions
	void		 WriteSWF( idFile_SWF& f, int characterID );

	//! Returns the total number of frames in the sprite.
	uint16		 GetFrameCount() { return frameCount; }

	//! Returns the SWF object associated with this sprite
	class idSWF* GetSWF() { return swf; }

private:
	friend class idSWFSpriteInstance;
	friend class idSWFScriptFunction_Script;

	class idSWF*			swf; // this is required so things can access the dictionary, it would be kind of nice if we just had an idSWFDictionary pointer instead

	uint16					frameCount;

	// frameOffsets contains offsets into the commands list for each frame
	// the first command for frame 3 is frameOffsets[2] and the last command is frameOffsets[3]
	idList<uint32, TAG_SWF> frameOffsets;

	struct swfFrameLabel_t {
		idStr  frameLabel;
		uint32 frameNum;
	};
	idList<swfFrameLabel_t, TAG_SWF> frameLabels;

	struct swfSpriteCommand_t {
		swfTag_t	   tag;
		idSWFBitStream stream;
	};
	idList<swfSpriteCommand_t, TAG_SWF> commands;

	struct svgLuaMarker_t {
		int	  frame;
		idStr fn;
	};
	idList<svgLuaMarker_t, TAG_SWF> svgLuaMarkers;

	struct svgRemoveMarker_t {
		int frame;
		int depth;
	};
	idList<svgRemoveMarker_t, TAG_SWF> svgRemoveMarkers;

	struct svgDeferredCommand_t {
		int			   frame;
		int			   orderIndex;
		swfTag_t	   tag;
		idSWFBitStream stream;
	};
	idList<svgDeferredCommand_t, TAG_SWF> svgDeferredCommands;
	int									  svgOrderIndexCounter;

	//// [ES-BrianBugh 1/16/10] - There can be multiple DoInitAction tags, and all need to be executed.
	idList<idSWFBitStream, TAG_SWF>		  doInitActions;

	byte*								  commandBuffer;
};

#endif // !__SWF_SPRITES_H__
