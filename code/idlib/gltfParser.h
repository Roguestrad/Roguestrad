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
#include "gltfProperties.h"

#pragma region GLTF Types parsing

#pragma region Parser interfaces
struct parsable {
public:
	virtual ~parsable() { }

	virtual void   parse( idToken& token ) = 0;
	virtual void   parse( idToken& token, idLexer* parser ) {};
	virtual idStr& Name() = 0;
};

/*!
	\class parseType
	\brief A template class for managing type parsing operations.
*/
template<class T>
class parseType
{
public:
	/*!
		\brief Sets the internal item pointer to the provided type pointer

		This function is a simple setter that assigns the provided type pointer to the internal item member variable. It is typically used in the context of parsing type handling where a parser needs
	   to maintain a reference to the current type being processed.

		\param type Pointer to the type instance to be set as the current item
	*/
	void Set( T* type ) { item = type; }
	T*	 item;
};

/*!
	\class gltfItem
	\brief A GLTF item handler that manages named GLTF data with parsing capabilities.
*/
class gltfItem : public parsable, public parseType<idStr>
{
public:
	/*!
		\brief Constructs a gltfItem object with the specified name.

		The constructor initializes the gltfItem with the provided name and sets the item pointer to nullptr.

		\param Name The name to assign to the gltfItem.
	*/
	gltfItem( idStr Name ) :
		name( Name )
	{
		item = nullptr;
	}
	virtual void   parse( idToken& token ) { *item = token; };

	/*!
		\brief Returns a reference to the name of the GLTF item

		This function provides access to the name of the GLTF item stored in the object. It returns a reference to an idStr object which contains the name of the item. The name is typically used for
	   identification and lookup purposes within the model management system

		\return A reference to the idStr object containing the name of the GLTF item
	*/
	virtual idStr& Name() { return name; }
	~gltfItem() { }

private:
	idStr name;
};

/*!
	\class gltfObject
	\brief A class for managing GLTF objects with parsing capabilities.
*/
class gltfObject : public parsable, public parseType<idStr>
{
public:
	/*!
		\brief Constructs a gltfObject with the specified name.

		The constructor initializes the gltfObject with the provided name and sets the object member to a default value of "null".

		\param Name The name to assign to the gltfObject.
	*/
	gltfObject( idStr Name ) :
		name( Name ),
		object( "null" )
	{
	}

	/*!
		\brief Parses a token for the gltfObject.

		This virtual function serves as an interface for parsing tokens within the gltfObject class. It is intended to be overridden by derived classes to provide specific parsing logic for different
	   types of tokens encountered during glTF file processing. The function currently has an empty implementation and is meant to be extended by subclasses.

		\param token The token to be parsed by this function.
	*/
	virtual void parse( idToken& token ) { }

	/*!
		\brief Parses a GLTF object section using the provided token and lexer.

		The function unwinds the provided token back into the lexer stream and then parses a braced section from the lexer, storing the result in the object member. This is typically used when
	   encountering a GLTF object definition in a parser context.

		\param token The token to be unwound and used as a starting point for parsing.
		\param parser The lexer instance used to parse the braced section.
	*/
	virtual void parse( idToken& token, idLexer* parser )
	{
		parser->UnreadToken( &token );
		parser->ParseBracedSection( object );
	}

	/*!
		\brief Returns a reference to the name of the glTF object.

		This function provides access to the internal name string of the glTF object, which is typically used to identify the object within the rendering system. The returned reference allows for both
	   reading and modification of the name, though modifications should be done carefully to avoid breaking internal data structures or references.

		\return A reference to the idStr object containing the name of the glTF object.
	*/
	virtual idStr& Name() { return name; }

private:
	idStr name;
	idStr object;
};

/*!
	\class gltfItem_Extra
	\brief A class for handling GLTF item extra data parsing and storage.
*/
class gltfItem_Extra : public parsable, public parseType<gltfExtra>
{
public:
	/*!
		\brief Constructor for gltfItem_Extra that initializes the name and sets other members to default values.

		This constructor initializes a gltfItem_Extra object with the provided name. It sets the data and parser members to nullptr and item to nullptr as default values. The name is stored in the
	   name member variable for later use.

		\param Name The name to assign to this gltfItem_Extra instance
	*/
	gltfItem_Extra( idStr Name ) :
		name( Name ),
		data( nullptr ),
		parser( nullptr )
	{
		item = nullptr;
	}

	/*!
		\brief Parses a GLTF item extra section from a token and stores the parsed data

		This function processes a token to extract and parse GLTF item extra data. It first unread the token to allow for re-parsing, then parses a braced section into the item's JSON string. It then
	   creates a lexer to process the JSON data and fills an array of GLTF items with the parsed data. The function also prints the parsed JSON if verbose parsing is enabled

		\param token The token containing the GLTF item extra data to parse
	*/
	virtual void   parse( idToken& token );

	/*!
		\brief Returns a reference to the name of the GLTF item

		This function provides access to the name associated with a GLTF item. It is a virtual function that can be overridden by derived classes to provide custom name handling. The function returns
	   a reference to an idStr object which contains the name of the item.

		\return A reference to the name of the GLTF item stored in the name member variable
	*/
	virtual idStr& Name() { return name; }

	/*!
		\brief Initializes the gltfItem_Extra object with the specified gltfExtra type and idLexer parser

		This function sets up the gltfItem_Extra object by assigning the provided gltfExtra type and idLexer parser. It calls the base class Set method to initialize the parseType and then assigns the
	   parser pointer to the internal parser member variable. The function is typically used during the parsing of GLTF (GL Transmission Format) files to associate the parsed data with the appropriate
	   lexer for further processing.

		\param type Pointer to the gltfExtra type to be set
		\param lexer Pointer to the idLexer parser to be assigned
	*/
	void		   Set( gltfExtra* type, idLexer* lexer )
	{
		parseType::Set( type );
		parser = lexer;
	}
	static void Register( parsable* extra );

private:
	idStr	  name;
	gltfData* data;
	idLexer*  parser;
};

/*!
	\class gltfItem_uri
	\brief A class for handling glTF item URI references and data loading.
*/
class gltfItem_uri : public parsable, public parseType<idStr>
{
public:
	/*!
		\brief Constructs a gltfItem_uri object with the specified name and initializes the item pointer to nullptr.
		\param Name The name to assign to the gltfItem_uri object
	*/
	gltfItem_uri( idStr Name ) :
		name( Name )
	{
		item = nullptr;
	}
	virtual void parse( idToken& token )
	{
		*item = token;
		Convert();
	};

	/*!
		\brief Returns a reference to the name of the glTF item.

		This function provides access to the name of the glTF item by returning a reference to its internal name member. The name is typically used for identification and retrieval of the item within
	   a hash table or model manager. The function is marked as virtual to allow derived classes to override its behavior.

		\return A reference to the internal name string of the glTF item
	*/
	virtual idStr& Name() { return name; }

	/*!
		\brief Sets the type, buffer view, and data destination for a glTF item

		This function initializes the properties of a glTF item by storing the provided type, buffer view index, and data destination. It inherits the type from a base class and assigns the buffer
	   view and data pointers directly. The function is typically used during the parsing and construction of glTF assets to properly link the item's metadata with its associated data and buffer view.

		\param type Pointer to the type string for the glTF item
		\param targetBufferview Pointer to the buffer view index for this item
		\param dataDestination Pointer to the destination glTF data structure
	*/
	void		   Set( idStr* type, int* targetBufferview, gltfData* dataDestination )
	{
		parseType::Set( type );
		bufferView = targetBufferview;
		data	   = dataDestination;
	}

	/*!
		\brief Loads data from a URI file and appends it to the GLTF data buffer

		This function reads data from a file specified by the URI and appends it to the GLTF data buffer. It creates a new buffer entry and optionally sets up a buffer view if one is specified. The
	   function handles file reading and memory management for the data buffer. It returns false indicating that it does not complete the conversion process, but the data is loaded into the buffer and
	   buffer view is set accordingly.

		\return false indicating the conversion process is not fully completed, though data is loaded into the buffer
		\throws FatalError if the file cannot be read completely
	*/
	bool Convert();

private:
	idStr	  name;
	int*	  bufferView;
	gltfData* data;
};
#pragma endregion

#pragma region helper macro to define gltf data types with extra parsing context forced to be implemented externally
#define gltfItemClassParser( className, ptype )                           \
	class gltfItem_##className : public parsable, public parseType<ptype> \
	{                                                                     \
	public:                                                               \
		gltfItem_##className( idStr Name ) :                              \
			name( Name )                                                  \
		{                                                                 \
			item = nullptr;                                               \
		}                                                                 \
		virtual void   parse( idToken& token );                           \
		virtual idStr& Name()                                             \
		{                                                                 \
			return name;                                                  \
		}                                                                 \
		void Set( ptype* type, idLexer* lexer )                           \
		{                                                                 \
			parseType::Set( type );                                       \
			parser = lexer;                                               \
		}                                                                 \
                                                                          \
	private:                                                              \
		idStr	 name;                                                    \
		idLexer* parser;                                                  \
	}
#pragma endregion

/*!
	\brief Parses animation sampler data from a token into a list of animation samplers

	This function processes animation sampler properties from a given token and populates a list of animation sampler objects. It handles parsing of input, interpolation, output, extensions, and
   extras properties for each animation sampler. The function iterates through a property array and uses a lexer to parse each sampler's data. The interpolation type is resolved and stored for each
   sampler. The function expects to encounter a closing bracket token after processing all samplers.

	\param token The token containing animation sampler data to parse
*/
gltfItemClassParser( animation_sampler, idList<gltfAnimation_Sampler*> );

/*!
	\brief Parses an animation channel target from a token and resolves its transformation type

	This function processes a token to parse animation channel target data including node, path, extensions, and extras. It reads the token back into the parser, creates an animation channel target
   array, and parses the array items. The function then resolves the transformation type based on the parsed path and sets the extras information.

	\param token The token containing the animation channel target data to parse
*/
gltfItemClassParser( animation_channel_target, gltfAnimation_Channel_Target );

/*!
	\brief Parses animation channel data from a GLTF token into a list of animation channels

	This function processes a GLTF token to extract animation channel information including sampler, target, extensions, and extras. It iterates through the parsed properties and creates animation
   channel entries with their associated data. The function handles multiple animation channels within a single token and ensures proper memory allocation for each channel entry.

	\param token The GLTF token containing animation channel data to parse
*/
gltfItemClassParser( animation_channel, idList<gltfAnimation_Channel*> );

/*!
	\brief Parses a mesh primitive from a GLTF token, extracting attributes, indices, material, mode, target, extensions, and extras.

	This function processes a GLTF mesh primitive by parsing its various components including attributes, indices, material, mode, target, extensions, and extras. It uses a lexer to parse the token
   data and populates a list of mesh primitives with the parsed information. The function handles multiple properties and ensures proper allocation of memory for new mesh primitive items.

	\param token The GLTF token containing the mesh primitive data to parse
*/
gltfItemClassParser( mesh_primitive, idList<gltfMesh_Primitive*> );

/*!
	\brief Parses a mesh primitive attribute from a token stream

	This function processes a token stream to extract mesh primitive attribute data. It iterates through the token stream, parsing attribute semantic names and their associated accessor indices. The
   function handles comma-separated attribute entries and ensures proper JSON structure parsing by expecting closing braces. If verbose parsing is enabled, it prints the parsed token information.

	\param token The token containing the attribute data to parse
*/
gltfItemClassParser( mesh_primitive_attribute, idList<gltfMesh_Primitive_Attribute*> );

/*!
	\brief Parses an integer array from a token into the glTF item

	The function processes a token representing an integer array in a glTF file. It reads the array elements and allocates memory for each integer value. The parser handles negative numbers by
   checking for a minus sign before the number. It also supports verbose output for debugging purposes. The function expects the token to represent an array and validates the structure by ensuring the
   closing bracket is present after parsing all elements.

	\param token The token containing the integer array data to be parsed
	\throws FatalError if a parse error occurs during the parsing of the integer array
*/
gltfItemClassParser( integer_array, idList<int> );

/*!
	\brief Parses a GLTF number array from the lexer and stores it in the provided list

	This function serves as a parser for GLTF number arrays, reading data from a lexer and storing it in an idList of doubles. The implementation uses a macro gltfItemClassParser to handle the parsing
   logic, though there is a comment questioning whether floats would be sufficient for the data type. This function is typically used in the context of GLTF file parsing for handling numeric data
   arrays within GLTF items.

	\param type Pointer to an idList of doubles that will store the parsed number array
	\param lexer Pointer to an idLexer object used to read the GLTF data
*/

/*!
	\brief Parses a glTF number array item and returns its name

	This function is responsible for parsing a glTF number array item using the gltfItemClassParser with an idList<double> type. It handles the extraction and processing of numeric data from glTF
   files, specifically for arrays of numbers. The function name is inherited from a base class and returns a reference to an idStr object containing the parsed item's name. The implementation notes
   that a float type might be sufficient for the data, but this is currently uncertain and requires further investigation.

	\return Reference to an idStr object containing the name of the parsed glTF number array item
*/

/*!
	\brief Parses a JSON number array from a token, handling negative values and storing them in the item.

	This function processes a JSON array of numbers by reading tokens from the parser. It supports both positive and negative floating-point values. Each parsed number is stored in the item's
   allocated space. The function reads tokens until it encounters a closing bracket, ensuring proper array format. If the token is not a valid number or punctuation, it triggers a fatal error. The
   verbose parsing mode can be used to print each parsed value to the console.

	\param token The token representing the start of a number array in the JSON structure
	\throws A fatal error is thrown if the parsed token is not a valid number or punctuation.
*/

/*!
	\brief Constructs a gltfItem_number_array object with the specified name

	This constructor initializes a gltfItem_number_array object by parsing a number array using the gltfItemClassParser with idList<double> as the template type. The parser is designed to handle
   numeric data arrays from GLTF files. The comment indicates uncertainty about whether float precision is sufficient for the parsed values, suggesting a potential need for double precision.

	\param Name The name to assign to this gltfItem_number_array object
*/
gltfItemClassParser( number_array, idList<double> );

/*!
	\brief Parses a 4x4 matrix from a token and stores it in the item member.

	This function initializes a gltfItem_number_array to read numeric values from the provided token. It expects exactly 16 numeric values to form a 4x4 matrix. If the token does not contain exactly
   16 values, a fatal error is issued. The values are then used to construct an idMat4 matrix which is stored in the item member.

	\param token The token containing the 16 numeric values for the matrix
	\throws Fatal error if the token does not contain exactly 16 numeric values.
*/
gltfItemClassParser( mat4, idMat4 );

/*!
	\brief Parses a token to extract four numeric values and stores them as an idVec4.

	This function processes a token to extract four numeric values and sets the item to an idVec4 constructed from those values. It uses a temporary gltfItem_number_array to parse the token and
   validate that exactly four numbers are present. If the count is not exactly four, the program will terminate with a fatal error. The parsed values are then used to initialize the idVec4 item.

	\param token The token containing the numeric values to parse
	\throws A fatal error is thrown if the number of parsed values is not exactly four
*/
gltfItemClassParser( vec4, idVec4 );

/*!
	\brief Parses a GLTF token into a 3D vector item

	This function processes a GLTF token to extract a 3D vector value. It creates a temporary array of numbers, parses the token into this array, and then validates that exactly three numbers were
   parsed. If the count is incorrect, it triggers a fatal error. Otherwise, it constructs an idVec3 from the first three values and assigns it to the item member

	\param token The GLTF token containing the 3D vector data to parse
	\throws Fatal error if the parsed number array does not contain exactly 3 elements
*/
gltfItemClassParser( vec3, idVec3 );

/*!
	\brief Parses a GLTF token into a 2D vector item.

	This function processes a GLTF token to extract a 2D vector value. It creates a temporary array of numbers, parses the token into this array, and then validates that exactly two numbers were
   parsed. If the count is incorrect, it triggers a fatal error. Otherwise, it constructs an idVec2 from the first two values and assigns it to the item member.

	\param token The GLTF token containing the 2D vector data to parse
	\throws Fatal error if the parsed number array does not contain exactly 2 elements
*/
gltfItemClassParser( vec2, idVec2 );

/*!
	\brief Parses a quaternion from a token and stores the result in the item member.

	The function expects a token containing a sequence of four numbers representing quaternion components. It validates that exactly four numbers are provided, and if not, it triggers a fatal error.
   The parsed numbers are then used to construct an idQuat object which is stored in the item member.

	\param token The token containing the quaternion data to be parsed.
	\throws fatal error if the token does not contain exactly four numbers
*/
gltfItemClassParser( quat, idQuat );

/*!
	\brief Parses a sparse accessor token and initializes its indices and values

	This function processes a glTF sparse accessor token by parsing its count, indices, and values components. It warns that the gltfItem_accessor_sparse functionality is untested. The function uses a
   temporary sparse array to parse the components and then sets the indices and values of the current item using the parser. The extensions and extras are also handled but not directly assigned to the
   item. The parsing process includes checking for verbose output mode to print the token information.

	\param token The token containing the sparse accessor data to be parsed
*/
gltfItemClassParser( accessor_sparse, gltfAccessor_Sparse );

/*!
	\brief Parses the sparse indices data for a glTF accessor item

	This function parses the sparse indices data structure for a glTF accessor, extracting information about the buffer view, byte offset, and component type. It also handles extensions and extras
   metadata. The parsing process warns about untested functionality and includes verbose output when enabled.

	\param token The token containing the sparse indices data to parse
	\throws Warning is issued for untested functionality
*/
gltfItemClassParser( accessor_sparse_indices, gltfAccessor_Sparse_Indices );

/*!
	\brief Parses the sparse values accessor item from a GLTF token

	This function processes a GLTF token to parse the sparse values accessor item. It warns about untested functionality and handles parsing of buffer view, byte offset, extensions, and extras. The
   function uses helper macros to parse array items and sets up references for buffer view and byte offset. It also prints verbose output when enabled.

	\param token The GLTF token to parse for sparse values accessor information
*/
gltfItemClassParser( accessor_sparse_values, gltfAccessor_Sparse_Values );

/*!
	\brief Parses a camera perspective definition from a token and initializes its properties

	This function reads a camera perspective definition from the provided token and parses its associated properties including aspect ratio, field of view, near and far clipping distances, and
   extensions. It uses a parser to handle the parsing of nested elements and sets up the camera perspective with the parsed values. The function also handles extra data and optionally prints verbose
   output when parsing is enabled.

	\param token The token containing the camera perspective definition to be parsed
*/
gltfItemClassParser( camera_perspective, gltfCamera_Perspective );

/*!
	\brief Parses an orthographic camera definition from a token and outputs property information.

	This function processes a token to parse an orthographic camera definition. It creates a property array from the parser and iterates through each property, printing the item content. The function
   then expects a closing bracket token to complete the parsing process.

	\param token The token containing the orthographic camera definition to parse
*/
gltfItemClassParser( camera_orthographic, gltfCamera_Orthographic );

/*!
	\brief Parses a glTF PBR metallic roughness item from the provided token

	This function processes a glTF PBR metallic roughness definition by parsing various properties such as base color factor, base color texture, metallic factor, roughness factor, and metallic
   roughness texture. It reads the token, sets up the parsing structure, and handles the parsing of each component according to the glTF specification. The function also supports extensions and extra
   data parsing. If verbose parsing is enabled, it prints the token name during parsing.

	\param token The token containing the glTF PBR metallic roughness data to parse
*/
gltfItemClassParser( pbrMetallicRoughness, gltfMaterial_pbrMetallicRoughness );

/*!
	\brief Parses texture information from a token including index, texture coordinates, extensions, and extras

	This function processes a token to extract and parse texture information for GLTF items. It reads the token back into the parser, initializes a texture info array, and parses the index, texture
   coordinates, extensions, and extras fields. The parsed data is then stored in the corresponding member variables.

	\param token The token containing the texture information to be parsed
*/
gltfItemClassParser( texture_info, gltfTexture_Info );

/*!
	\brief Parses a normal texture item from a GLTF file using the provided token.

	This function processes a token representing a normal texture in a GLTF file. It reads the token back into the parser, then parses various properties of the texture including index, texCoord, and
   scale. It also handles extensions and extras associated with the texture. The function supports verbose parsing output when enabled.

	\param token The token containing the normal texture data to parse
*/
gltfItemClassParser( normal_texture, gltfNormalTexture_Info );

/*!
	\brief Parses an occlusion texture definition from a GLTF token

	This function processes a GLTF token to parse occlusion texture information. It first unwinds the token, then creates a texture info array to parse various components including index, texCoord,
   strength, extensions, and extras. The function reads and sets the parsed values into the texture info structure and handles extension and extra data parsing. It also prints verbose output in debug
   mode to show the token being parsed.

	\param token The GLTF token containing occlusion texture information to parse
*/
gltfItemClassParser( occlusion_texture, gltfOcclusionTexture_Info );

/*!
	\brief Parses the extensions for a glTF node using the provided token.

	This function processes the extensions associated with a glTF node by first unread the provided token and then parsing the extensions array. It specifically handles the KHR_lights_punctual
   extension and sets its value using the item and parser. The function also prints the token string if verbose parsing is enabled.

	\param token The token containing the extension data to be parsed
*/
gltfItemClassParser( node_extensions, gltfNode_Extensions );

/*!
	\brief Parses the material extensions token and sets up the specular glossiness parameters

	This function processes a material extensions token by unreadding it and then parsing a list of extensions. It specifically handles the KHR materials pbr specular glossiness extension, setting its
   parameters based on the provided item and parser. The function also prints verbose output if enabled.

	\param token The token to parse for material extensions
*/
gltfItemClassParser( material_extensions, gltfMaterial_Extensions );

/*!
	\brief Parses texture info extensions from a token, handling KHR_texture_transform and other extension items

	This function processes texture info extensions by first unread the provided token and then parses a list of extensions. It specifically handles KHR_texture_transform extension and sets its value
   based on the parsed item. The function supports verbose parsing output when enabled.

	\param token Input token containing the texture info extension data to parse
*/
gltfItemClassParser( texture_info_extensions, gltfTexture_Info_Extensions );

/*!
	\brief Parses light definitions from a GLTF file using the KHR_lights_punctual extension

	This function processes the lights array in a GLTF file that uses the KHR_lights_punctual extension. It expects a JSON structure with an array of light definitions, each containing properties such
   as color, intensity, type, range, and name. The function parses each light definition and stores the resulting light data in the item's KHR_lights_punctual list. It handles the parsing of nested
   properties and resolves light types to their appropriate enumerations.

	\param token The token containing the light data to parse
*/
gltfItemClassParser( KHR_lights_punctual, gltfExtensions );

/*!
	\brief Parses a token to extract light punctual data into a gltfNode_KHR_lights_punctual structure

	This function processes a token to read and parse light punctual data from a GLTF parser. It first unread the token to prepare for parsing, then creates a temporary array to hold the light data.
   The function sets up a new gltfNode_KHR_lights_punctual structure and assigns the parsed light data to it. The parsing is verbose when gltf_parseVerbose console variable is enabled, and prints the
   token content to the console.

	\param token The token containing the light punctual data to be parsed
*/
gltfItemClassParser( Node_KHR_lights_punctual, gltfNode_Extensions );

/*!
	\brief Parses a GLTF material extension for specular glossiness parameters from a token.

	This function processes a token to parse and set up the specular glossiness material properties for a GLTF item. It reads various parameters such as diffuse factor, diffuse texture, specular
   factor, glossiness factor, and specular glossiness texture from the parser. The function also handles extensions and extras for the material. It initializes a new
   gltfExt_KHR_materials_pbrSpecularGlossiness object and populates it with the parsed values.

	\param token The token containing the material data to be parsed
*/
gltfItemClassParser( Material_KHR_materials_pbrSpecularGlossiness, gltfMaterial_Extensions );

/*!
	\brief Parses a texture transform information from a token and initializes the corresponding texture transform data structure.

	This function processes a token to extract texture transform properties such as offset, rotation, scale, and texture coordinate index. It reads the token back into the parser, creates a new
   texture transform extension structure, and populates it with parsed values. The function handles parsing of vector and scalar values for the transform properties and includes optional verbose
   output for debugging purposes.

	\param token The token containing the texture transform data to be parsed
*/
gltfItemClassParser( TextureInfo_KHR_texture_transform, gltfTexture_Info_Extensions );

#undef gltfItemClassParser

#pragma region helper macro to define more gltf data types that only rely on token
#define gltfItemClass( className, type, function )                       \
	class gltfItem_##className : public parsable, public parseType<type> \
	{                                                                    \
	public:                                                              \
		gltfItem_##className( idStr Name ) :                             \
			name( Name )                                                 \
		{                                                                \
			item = nullptr;                                              \
		}                                                                \
		virtual void parse( idToken& token )                             \
		{                                                                \
			function                                                     \
		}                                                                \
		virtual idStr& Name()                                            \
		{                                                                \
			return name;                                                 \
		}                                                                \
                                                                         \
	private:                                                             \
		idStr name;                                                      \
	}
#pragma endregion

gltfItemClass( integer, int, *item = token.GetIntValue(); );
gltfItemClass( number, float, *item = token.GetFloatValue(); );
gltfItemClass(
	boolean, bool, if( token.Icmp( "true" ) == 0 ) * item = true; else {
		if( token.Icmp( "false" ) == 0 ) {
			*item = false;
		} else {
			idLib::FatalError( "parse error" );
		}
	} );
#undef gltfItemClass

/*!
	\class gltfItemArray
	\brief Container class for managing an array of glTF item definitions.

	This class provides functionality for storing and managing a collection of glTF item definitions, with methods for adding items, parsing JSON data into the collection, and retrieving specific
   items by name. The class handles memory management through its destructor which properly cleans up allocated items. The parsing functions support JSON-like structures with key-value pairs and
   nested objects, making it suitable for processing glTF file formats. The array maintains an internal collection of items that can be accessed by their string names for efficient retrieval during
   processing workflows.

*/
class gltfItemArray
{
public:
	/*!
		\brief Destructor for gltfItemArray that cleans up allocated memory.

		This destructor is responsible for cleaning up the memory allocated for the items in the gltfItemArray. It calls DeleteContents with true to ensure that all contained objects are properly
	   deleted.

	*/
	~gltfItemArray();
	gltfItemArray() {};

	/*!
		\brief Returns the number of items in the gltfItemArray

		The Num function provides the count of items stored in the internal items array. This is a simple wrapper function that delegates the call to the underlying Num() method of the items
	   collection.

		\return The total number of items contained within the gltfItemArray
	*/
	int	 Num() { return items.Num(); }

	/*!
		\brief Adds a parsable item to the internal array of items.

		The function appends the provided parsable item to the internal array managed by this class. It allocates space in the array for the new item and assigns the item to that location.

		\param item The parsable item to be added to the array.
	*/
	void AddItemDef( parsable* item ) { items.Alloc() = item; }

	/*!
		\brief Parses key-value pairs from a lexer and stores them in a dictionary

		This function reads tokens from the provided lexer and parses key-value pairs in a JSON-like format. It handles different data types including strings, arrays, and nested objects. The function
	   continues parsing until it encounters a closing brace or reaches the end of the token stream. The parsed key-value pairs are stored in the provided dictionary

		\param lexer Pointer to the lexer object used to read tokens from the input
		\param strPairs Pointer to the dictionary where parsed key-value pairs will be stored
		\return The number of key-value pairs successfully parsed and stored in the dictionary
	*/
	int	 Fill( idLexer* lexer, idDict* strPairs );

	/*!
		\brief Parses a JSON object from the provided lexer and updates the associated items, returning the count of parsed items.

		This function processes a JSON object structure, expecting an opening brace '{' and iterating through key-value pairs. For each key, it attempts to match against known item names and calls the
	   corresponding parse method. If no match is found, it skips the section. The function continues parsing until a closing brace '}' is encountered. The boolean parameter forwardLexer controls
	   whether the lexer state is forwarded to the parse method.

		\param lexer The lexer instance used to parse the JSON object
		\param forwardLexer Whether to forward the lexer state to the parse method
		\return The number of successfully parsed items in the JSON object
	*/
	int	 Parse( idLexer* lexer, bool forwardLexer = false );

	/*!
		\brief Retrieves a pointer to an item from the array by its name

		The function searches through the items in the array and returns a pointer to the first item whose name matches the provided name. If no matching item is found, it returns nullptr. The
	   function performs a linear search through the items array and uses the Name() method of each item to compare against the provided name

		\param name name of the item to search for
		\return Pointer to the found item if it exists, otherwise nullptr
	*/
	template<class T>
	T* Get( idStr name )
	{
		for( auto* item : items )
			if( item->Name() == name ) { return static_cast<T*>( item ); }
		return nullptr;
	}

private:
	idList<parsable*> items;
};
#pragma endregion

#pragma region GLTF Object parsing
class gltfPropertyArray;

/*!
	\class gltfPropertyItem
	\brief A class representing a property item with a null array pointer.
*/
class gltfPropertyItem
{
public:
	/*!
		\brief Initializes a gltfPropertyItem object with a null array pointer.

		This is the default constructor for the gltfPropertyItem class. It initializes the array member to nullptr, indicating that no array has been allocated or assigned to this property item.

	*/
	gltfPropertyItem() :
		array( nullptr )
	{
	}
	gltfPropertyArray* array;
	idToken			   item;
};

/*!
	\class gltfPropertyArray
	\brief A container for managing GLTF property arrays with iterator support for parsing and traversal.
*/
class gltfPropertyArray
{
public:
	/*!
		\brief Initializes a gltfPropertyArray object with the provided lexer parser and optional array-of-structs flag.

		The constructor initializes the gltfPropertyArray with a given lexer parser and an optional boolean flag indicating whether the data is stored as array-of-structs. It sets up internal state
	   including iterator flags, dirty flags, and index tracking. The constructor also allocates memory for properties list and initializes an end pointer for the array.

		\param Parser Pointer to the lexer used for parsing GLTF property data
		\param AoS Boolean flag indicating if data is stored as array-of-structs, defaults to true
	*/
	gltfPropertyArray( idLexer* Parser, bool AoS = true );

	/*!
		\brief Destructor for gltfPropertyArray that cleans up allocated memory and contents.

		The destructor releases the memory allocated for the end pointer and deletes all contained properties. It ensures proper cleanup of resources when the gltfPropertyArray object is destroyed.

	*/
	~gltfPropertyArray();
	struct Iterator {
		gltfPropertyArray* array;
		gltfPropertyItem*  p;
		gltfPropertyItem&  operator*() { return *p; }
		bool			   operator!=( Iterator& rhs ) { return p != rhs.p; }
		void			   operator++();
	};

	/*!
		\brief Returns an iterator pointing to the beginning of the glTF property array.

		The function initializes the iteration over the glTF property array. If the array is currently being iterated, it handles parsing of the array structure, ensuring proper formatting and
	   allocating memory for the array elements. It returns an Iterator object that points to the first element in the array. If the array is not currently being iterated, it returns an iterator
	   pointing to the initial element at index 0.

		\return An iterator pointing to the beginning of the glTF property array.
		\throws FatalError if the glTF array is malformed.
	*/
	gltfPropertyArray::Iterator begin();

	/*!
		\brief Returns an iterator pointing to the end of the gltfPropertyArray.

		This function provides access to the end iterator of the gltfPropertyArray, which is used to indicate the position just past the last element in the array. This is commonly used in range-based
	   loops and other iteration patterns to determine when to stop iterating over the elements.

		\return An iterator pointing to the end of the gltfPropertyArray
	*/
	gltfPropertyArray::Iterator end();

private:
	bool					  iterating;
	bool					  dirty;
	int						  index;
	idLexer*				  parser;
	idList<gltfPropertyItem*> properties;
	gltfPropertyItem*		  endPtr;
	bool					  isArrayOfStructs;
};
#pragma endregion

/*!
	\class GLTF_Parser
	\brief A parser for GLTF and GLB file formats that processes and structures 3D asset data.

	This class provides functionality to load and parse GLTF and GLB files, extracting 3D asset information including scenes, nodes, meshes, materials, textures, animations, and skeleton data. It
   handles both JSON-based GLTF files and binary GLB files, managing memory and internal state during the parsing process. The parser supports hierarchical node structures, proper parent-child
   relationships, and various GLTF extensions. It includes specific methods for parsing different sections of the GLTF format such as buffers, buffer views, cameras, materials, and animations. The
   class maintains internal tracking flags to manage parsing state and ensures proper cleanup of resources through its shutdown method. It provides detailed parsing capabilities with verbose output
   support for debugging purposes.

*/
class GLTF_Parser
{
public:
	~GLTF_Parser() { Shutdown(); }

	/*!
		\brief Initializes a new instance of the GLTF_Parser class with default settings.

		The constructor initializes the parser with specific lexical flags to handle GLTF file parsing. It also sets internal state variables to indicate that buffers and buffer views have not yet
	   been processed, and initializes the current asset pointer to null.

	*/
	GLTF_Parser();

	/*!
		\brief Cleans up and releases all resources used by the GLTF parser

		This function performs the necessary cleanup operations for the GLTF parser by freeing the current file data and deleting the current asset if it exists. It also resets internal tracking flags
	   to indicate that buffer and buffer view data have not been processed yet. This function is typically called during shutdown or cleanup operations to ensure proper resource management.

	*/
	void	  Shutdown();

	/*!
		\brief Parses GLTF data by processing tokens from the parser and handling different properties like buffers and buffer views.

		This function iterates through tokens from the parser to process GLTF data. It handles various properties such as buffers and buffer views, resetting the parser state when necessary. The
	   function checks for specific properties and manages parsing state flags to ensure proper handling of GLTF structure. It also includes verbose output when enabled and handles parser errors
	   appropriately.

		\return True if the parsing completes successfully, false otherwise
	*/
	bool	  Parse();

	/*!
		\brief Loads GLTF or GLB file data into the parser for further processing

		This function handles loading of both GLB binary and GLTF JSON format files. It first attempts to load the file using the gltfData::Data function, and if that fails, it processes the file
	   based on its extension. For GLB files, it calls loadGLB to handle the binary format, while for GLTF files it reads the file content into memory, parses it using the parser, and then performs
	   additional processing to fix node hierarchy and set skeleton IDs. The function also supports prefixing node names with IDs if enabled by a configuration parameter.

		\param filename The path to the GLTF or GLB file to load
		\return true if the file was loaded successfully, false otherwise
	*/
	bool	  Load( idStr filename );

	/*!
		\brief Loads a GLB file and parses its contents into the current asset data structure

		This function reads a GLB (GL Transmission Format Binary) file from the specified filename and parses its binary data. It first validates that the file exists and has sufficient size to be a
	   valid GLB file. The function checks for the proper GLB magic number and reads the file header to determine the version and total file length. It then processes each chunk in the file,
	   separating JSON metadata from binary data. The JSON chunk is loaded into the parser for further processing, while binary chunks are stored for later use. The function handles error cases such
	   as invalid file format, corrupt data, or missing chunks. After processing all chunks, it calls the Parse function to interpret the loaded data. The file is automatically closed and deleted
	   before returning.

		\param filename The path to the GLB file to load
		\return True if the GLB file was successfully loaded and parsed, false otherwise
	*/
	bool	  loadGLB( idStr filename );

	// current/last loaded gltf asset and index offsets
	gltfData* currentAsset;
	idStr	  currentFile;

private:
	/*!
		\brief Sets the parent node for a given node and recursively updates all its children to have the correct parent relationship.

		This function assigns the specified parent node to the given node and then recursively processes all child nodes of the given node to ensure they are properly linked to their new parent. It
	   maintains the hierarchical structure of nodes within the glTF asset.

		\param node Pointer to the node for which the parent needs to be set
		\param parent Pointer to the parent node, or nullptr if the node should have no parent
	*/
	void		 SetNodeParent( gltfNode* node, gltfNode* parent = nullptr );

	/*!
		\brief Parses the ASSET section from a glTF file and prints its content

		This function handles parsing of the ASSET section within a glTF file format. It retrieves the braced section content and outputs it to the console using the common Printf function. The
	   function is responsible for extracting and displaying asset information from the glTF file structure.

		\param token The token representing the ASSET section to be parsed
	*/
	void		 Parse_ASSET( idToken& token );

	/*!
		\brief Parses camera data from a GLTF file using the provided token.

		This function processes camera data stored in a GLTF file. It iterates through an array of camera properties, parsing each camera's orthographic and perspective projections, type, name,
	   extensions, and extra data. The function uses a lexer to parse the camera data and populates camera objects in the current asset. It expects the token to represent the start of a camera array
	   in the GLTF file and handles the parsing of individual camera items within that array.

		\param token The token containing the camera data to parse
	*/
	void		 Parse_CAMERAS( idToken& token );

	/*!
		\brief Parses the scene information from the GLTF file and stores the default scene index.

		This function reads the scene index from the GLTF file's token and sets it as the default scene in the current asset. It also prints a verbose message containing the scene index if the
	   gltf_parseVerbose console variable is enabled.

		\param token The token containing the scene data to parse
	*/
	void		 Parse_SCENE( idToken& token );

	/*!
		\brief Parses the SCENES array from a GLTF file and populates the corresponding scene data structures

		This function processes the SCENES section of a GLTF file, extracting information about individual scenes including their nodes, name, extensions, and extra data. It uses a lexer to parse the
	   GLTF property array and populates the current asset's scene data structures accordingly. The function handles various data types including arrays and string properties. If verbose parsing is
	   enabled, it prints the parsed scene data to the console.

		\param token token representing the current parsing position in the GLTF file
	*/
	void		 Parse_SCENES( idToken& token );

	/*!
		\brief Parses the NODES array from a GLTF file and populates the corresponding node data structures

		This function processes the NODES section of a GLTF file, extracting information about individual nodes including their camera references, children, skin, transformation matrix, mesh,
	   rotation, scale, translation, weights, name, extensions, and extra data. It uses a lexer to parse the GLTF property array and populates the current asset's node data structures accordingly. The
	   function handles various data types including integers, arrays, matrices, quaternions, and vectors. If verbose parsing is enabled, it prints the parsed node data to the console.

		\param token token representing the current parsing position in the GLTF file
	*/
	void		 Parse_NODES( idToken& token );

	/*!
		\brief Parses the materials section of a GLTF file and populates material properties

		This function processes the materials array from a GLTF file, parsing each material's properties including PBR metallic roughness, normal texture, occlusion texture, emissive texture, emissive
	   factor, alpha mode, alpha cutoff, double-sided flag, and name. It handles various texture information and material extensions, setting up the material data structures for use in rendering. The
	   function iterates through each material in the array, loads the material data into a lexer, and populates the corresponding gltfMaterial structure with parsed values. The alpha mode is resolved
	   into an integer type for efficient rendering.

		\param token The token representing the materials section in the GLTF file
	*/
	void		 Parse_MATERIALS( idToken& token );

	/*!
		\brief Parses mesh data from a GLTF token into the current asset's mesh collection

		This function processes mesh information from a GLTF file by iterating through an array of mesh properties. For each mesh property, it creates a lexer to parse the data and extracts primitive
	   data, weights, name, extensions, and extra information. The parsed mesh data is then stored in the current asset's mesh collection. The function expects the token to represent an array of mesh
	   objects and handles the parsing of each mesh's properties including primitives, weights, name, extensions, and extras. It also supports verbose output when the gltf_parseVerbose console
	   variable is enabled.

		\param token The GLTF token containing mesh data to be parsed
	*/
	void		 Parse_MESHES( idToken& token );

	/*!
		\brief Parses texture data from a GLTF file using the provided token.

		This function processes an array of texture definitions from a GLTF file. It iterates through each texture property in the array, extracts relevant information such as sampler, source, and
	   name, and sets up extension and extra data for each texture. The function uses a lexer to parse the texture properties and updates the current asset's texture information accordingly.

		\param token The token representing the start of the texture array in the GLTF file
	*/
	void		 Parse_TEXTURES( idToken& token );

	/*!
		\brief Parses image definitions from a GLTF file and populates the current asset with the parsed image data

		This function processes the image array section of a GLTF file, extracting information about image properties including URI, MIME type, buffer view, name, extensions, and extra properties. It
	   iterates through each image entry in the array, creates a lexer to parse the individual image properties, and sets up the corresponding fields in the image structure. The function handles
	   parsing of URI, MIME type, buffer view, name, extensions, and extra properties for each image in the GLTF file.

		\param token The token representing the start of the images array in the GLTF file
	*/
	void		 Parse_IMAGES( idToken& token );

	/*!
		\brief Parses accessor definitions from a GLTF file and populates the current asset with the parsed data

		This function processes the accessor array section of a GLTF file, extracting information about buffer views, byte offsets, component types, normalization flags, count values, data types,
	   maximum and minimum values, sparse accessors, names, extensions, and extra properties. It iterates through each accessor entry in the array, creates a lexer to parse the individual accessor
	   properties, and sets up the corresponding fields in the accessor structure. The function also determines the size of the component type for each accessor and optionally prints verbose output
	   during parsing.

		\param token The token representing the start of the accessors array in the GLTF file
	*/
	void		 Parse_ACCESSORS( idToken& token );

	/*!
		\brief Parses buffer view data from a GLTF file

		This function processes the buffer views section of a GLTF file, extracting information about buffer views including their associated buffer, byte length, byte stride, byte offset, target, and
	   name. It handles parsing of array items and sets up the buffer view properties within the current asset context.

		\param token Token representing the current parsing context for buffer views
	*/
	void		 Parse_BUFFERVIEWS( idToken& token );

	/*!
		\brief Parses the SAMPLERS array from a GLTF file and populates the corresponding sampler properties.

		This function processes the SAMPLERS array within a GLTF file, extracting individual sampler definitions and their properties such as filter settings and wrapping modes. It uses a lexer to
	   parse each sampler item and populates the current asset's sampler data structure with the parsed values. The function handles various sampler properties including magnification filter,
	   minification filter, and texture wrapping settings for both S and T coordinates.

		\param token The token representing the SAMPLERS array in the GLTF file
	*/
	void		 Parse_SAMPLERS( idToken& token );

	/*!
		\brief Parses GLTF buffer data from a token and populates buffer properties

		This function processes GLTF buffer information by parsing the provided token and extracting buffer properties such as URI, byte length, name, extensions, and extras. It handles multiple
	   buffer entries in an array format and initializes buffer data within the current asset context. The function uses a lexer to process the buffer data and populate the appropriate fields.

		\param token The token containing the GLTF buffer data to parse
	*/
	void		 Parse_BUFFERS( idToken& token );

	/*!
		\brief Parses animation data from GLTF files using the provided token

		This function processes animation data stored in GLTF format by iterating through a property array and parsing each animation item. It handles animation channels, samplers, names, extensions,
	   and extra data. The function uses a lexer to parse the animation data from memory and populates the current asset's animation structure with the parsed information. Debug output can be enabled
	   through the gltf_parseVerbose console variable.

		\param token The token identifying the animation data to parse
	*/
	void		 Parse_ANIMATIONS( idToken& token );

	/*!
		\brief Parses skin data from a GLTF file into the current asset

		This function processes the skin array from a GLTF file and populates the current asset's skin data. It iterates through each skin property in the array, extracts the relevant data such as
	   inverse bind matrices, skeleton, joints, name, extensions, and extras, and stores them in the appropriate skin structures. The function handles parsing of various GLTF skin properties including
	   joint arrays and additional metadata.

		\param token The token containing the skin data to parse
	*/
	void		 Parse_SKINS( idToken& token );

	/*!
		\brief Parses the EXTENSIONS_USED property from the GLTF file

		This function processes the EXTENSIONS_USED array in the GLTF file, iterating through each extension listed and storing it in the current asset's extensions used collection. It expects the
	   token to be the start of an array and processes until the closing bracket is encountered. The function also supports verbose parsing mode which prints each extension name to the console.

		\param token The token containing the EXTENSIONS_USED data to parse
	*/
	void		 Parse_EXTENSIONS( idToken& token );

	/*!
		\brief Parses the EXTENSIONS_USED property from the GLTF file

		This function processes the EXTENSIONS_USED array in the GLTF file, iterating through each extension listed and storing it in the current asset's extensions used collection. It expects the
	   token to be the start of an array and processes until the closing bracket is encountered. The function also supports verbose parsing mode which prints each extension name to the console.

		\param token The token containing the EXTENSIONS_USED data to parse
	*/
	void		 Parse_EXTENSIONS_USED( idToken& token );

	/*!
		\brief Parses the EXTENSIONS_REQUIRED array from a GLTF file and prints the extension names

		This function processes the EXTENSIONS_REQUIRED section of a GLTF file format. It expects an opening bracket token, then parses a list of string tokens representing required extensions until
	   it encounters a closing bracket. Each parsed extension name is stored and then printed to the output. The function handles parsing of comma-separated string values and includes error handling
	   for malformed arrays.

		\param token The token containing the EXTENSIONS_REQUIRED data to parse
		\throws FatalError when encountering malformed extensions_used array
	*/
	void		 Parse_EXTENSIONS_REQUIRED( idToken& token );

	/*!
		\brief Parses a GLTF property from the provided token and handles buffer and buffer view parsing.

		This function processes a GLTF property by first expecting a colon token, then resolving the property type. It handles special cases for buffers and buffer views, parsing them if needed or
	   skipping them if already processed. The function manages parsing of various GLTF components such as assets, cameras, scenes, nodes, materials, meshes, textures, images, accessors, samplers,
	   animations, skins, and extensions. It uses different parsing methods based on the resolved property type and will terminate with a fatal error for unsupported properties.

		\param token The token containing the GLTF property name to parse
		\return The parsed GLTF property type
		\throws This function may throw a fatal error if it encounters unsupported properties or incorrect parsing states for buffers and buffer views.
	*/
	gltfProperty ParseProp( idToken& token );

	/*!
		\brief Resolves a token string into its corresponding glTF property enum value

		This function takes a token representing a property name from a glTF file and maps it to the appropriate gltfProperty enumeration value. It performs string comparisons against known glTF
	   property names and returns the corresponding enum value. If the token does not match any known property, it returns gltfProperty::INVALID

		\param token The token string containing the property name to resolve
		\return The corresponding gltfProperty enum value for the given token, or gltfProperty::INVALID if no match is found
	*/
	gltfProperty ResolveProp( idToken& token );

	idLexer		 parser;
	idToken		 token;

	bool		 buffersDone;
	bool		 bufferViewsDone;
};

/*!
	\class gltfManager
	\brief Manages glTF file identifier extraction from filenames.
*/
class gltfManager
{
public:
	/*!
		\brief Extracts an identifier from a glTF filename, which can be either an integer ID or a name.

		This function processes a filename to extract a glTF identifier, which can be either an integer or a named identifier. It first checks for a valid file extension and then parses the filename
	   to isolate the identifier part. The identifier is extracted from the filename's extension portion and can be either a number or a name string. If the identifier format is invalid, the function
	   returns false and issues a warning.

		\param filename Input filename that should contain a glTF identifier, will be modified to remove the identifier part
		\param id Output integer identifier, set to -1 if not found
		\param name Output name identifier, will be empty if not found
		\return True if a valid identifier was extracted, false otherwise
	*/
	static bool ExtractIdentifier( idStr& filename, int& id, idStr& name );
};
