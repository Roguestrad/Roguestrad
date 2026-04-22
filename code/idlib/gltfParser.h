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
	\brief A template class for managing type pointers in parsing operations.
*/
template<class T>
class parseType
{
public:
	//! Sets the internal type pointer to the provided value
	void Set( T* type ) { item = type; }
	T*	 item;
};

/*!
	\class gltfItem
	\brief A class for handling GLTF items with parsing capabilities.
*/
class gltfItem : public parsable, public parseType<idStr>
{
public:
	//! Constructs a gltfItem object with the specified name and initializes the item pointer to null.
	gltfItem( idStr Name ) :
		name( Name )
	{
		item = nullptr;
	}
	virtual void   parse( idToken& token ) { *item = token; };

	//! Returns a reference to the name of the GLTF item.
	virtual idStr& Name() { return name; }
	~gltfItem() { }

private:
	idStr name;
};

/*!
	\class gltfObject
	\brief A class for parsing and managing GLTF objects with name-based identification.
*/
class gltfObject : public parsable, public parseType<idStr>
{
public:
	//! Constructs a gltfObject with the specified name and initializes its object member to "null".
	gltfObject( idStr Name ) :
		name( Name ),
		object( "null" )
	{
	}

	//! Parses a token for the glTF object.
	virtual void parse( idToken& token ) { }

	//! Parses a GLTF object by reading the token and using the parser to extract the object data.
	virtual void parse( idToken& token, idLexer* parser )
	{
		parser->UnreadToken( &token );
		parser->ParseBracedSection( object );
	}

	//! Returns a reference to the name of the GLTF object.
	virtual idStr& Name() { return name; }

private:
	idStr name;
	idStr object;
};

/*!
	\class gltfItem_Extra
	\brief A class for handling extra data elements within glTF items.
*/
class gltfItem_Extra : public parsable, public parseType<gltfExtra>
{
public:
	//! Constructs a gltfItem_Extra object with the specified name.
	gltfItem_Extra( idStr Name ) :
		name( Name ),
		data( nullptr ),
		parser( nullptr )
	{
		item = nullptr;
	}

	//! Parses a glTF extra item from the provided token
	virtual void   parse( idToken& token );

	//! Returns the name of the GLTF item extra.
	virtual idStr& Name() { return name; }

	//! Initializes the gltfItem_Extra object with the specified type and lexer
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
	\brief A class for handling URI data in glTF items.
*/
class gltfItem_uri : public parsable, public parseType<idStr>
{
public:
	//! Initializes a gltfItem_uri object with the specified name.
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

	//! Returns the name of the GLTF item.
	virtual idStr& Name() { return name; }

	//! Sets the type, buffer view, and data destination for a glTF item.
	void		   Set( idStr* type, int* targetBufferview, gltfData* dataDestination )
	{
		parseType::Set( type );
		bufferView = targetBufferview;
		data	   = dataDestination;
	}

	//! Converts URI data by reading file contents into a buffer and setting up buffer view information
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

//! Parses animation sampler data from a GLTF token into a list of animation samplers.
gltfItemClassParser( animation_sampler, idList<gltfAnimation_Sampler*> );

//! Parses animation channel target data from a token.
gltfItemClassParser( animation_channel_target, gltfAnimation_Channel_Target );

//! Parses animation channel data from a token into a list of animation channels
gltfItemClassParser( animation_channel, idList<gltfAnimation_Channel*> );

//! Parses a mesh primitive item from a token, extracting attributes, indices, material, mode, target, extensions, and extras.
gltfItemClassParser( mesh_primitive, idList<gltfMesh_Primitive*> );

//! Parses a mesh primitive attribute from a token stream.
gltfItemClassParser( mesh_primitive_attribute, idList<gltfMesh_Primitive_Attribute*> );

//! Parses an integer array from a token, handling negative values and updating the item with parsed values.
gltfItemClassParser( integer_array, idList<int> );

//! Sets a number array from a lexer input into the provided list.

//! Parses a GLTF number array item and returns its name

//! Parses a JSON number array from a token and stores the values in the item.

//! Constructs a gltfItem_number_array object with the specified name
gltfItemClassParser( number_array, idList<double> );

//! Parses a 4x4 matrix from a token and stores it in the item member.
gltfItemClassParser( mat4, idMat4 );

//! Parses a token into a four-element vector.
gltfItemClassParser( vec4, idVec4 );

//! Parses a token into a 3-component vector value.
gltfItemClassParser( vec3, idVec3 );

//! Parses a vec2 value from a token and stores it in the item.
gltfItemClassParser( vec2, idVec2 );

//! Parses a quaternion from a token and stores the result in the item member.
gltfItemClassParser( quat, idQuat );

//! Parses a sparse accessor token and warns about untested functionality.
gltfItemClassParser( accessor_sparse, gltfAccessor_Sparse );

//! Parses the sparse indices data for a glTF accessor.
gltfItemClassParser( accessor_sparse_indices, gltfAccessor_Sparse_Indices );

//! Parses the sparse values section of a GLTF accessor item using the provided token.
gltfItemClassParser( accessor_sparse_values, gltfAccessor_Sparse_Values );

//! Parses a camera perspective item from a GLTF token
gltfItemClassParser( camera_perspective, gltfCamera_Perspective );

//! Parses an orthographic camera definition from a token and processes its properties.
gltfItemClassParser( camera_orthographic, gltfCamera_Orthographic );

//! Parses GLTF PBR metallic roughness data from a token.
gltfItemClassParser( pbrMetallicRoughness, gltfMaterial_pbrMetallicRoughness );

//! Parses texture information from a token
gltfItemClassParser( texture_info, gltfTexture_Info );

//! Parses a normal texture item from a GLTF token.
gltfItemClassParser( normal_texture, gltfNormalTexture_Info );

//! Parses an occlusion texture item from a GLTF file using the provided token.
gltfItemClassParser( occlusion_texture, gltfOcclusionTexture_Info );

//! Parses the KHR_lights_punctual extension from the provided token.
gltfItemClassParser( node_extensions, gltfNode_Extensions );

//! Parses a material extension token and processes associated GLTF items.
gltfItemClassParser( material_extensions, gltfMaterial_Extensions );

//! Parses texture information extensions from a token including KHR_texture_transform support
gltfItemClassParser( texture_info_extensions, gltfTexture_Info_Extensions );

//! Parses light punctual data from a GLTF token.
gltfItemClassParser( KHR_lights_punctual, gltfExtensions );

//! Parses a token to extract light punctual data into a gltfNode_KHR_lights_punctual structure.
gltfItemClassParser( Node_KHR_lights_punctual, gltfNode_Extensions );

//! Parses a GLTF material extension for specular glossiness parameters from a token.
gltfItemClassParser( Material_KHR_materials_pbrSpecularGlossiness, gltfMaterial_Extensions );

//! Parses texture transform information from a token and populates the KHR_texture_transform extension data.
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
	\brief Manages a collection of glTF item definitions with parsing and retrieval capabilities.

	The gltfItemArray class provides functionality for storing, parsing, and accessing glTF item definitions. It supports adding item definitions, parsing JSON data from a lexer, and retrieving items
   by name. The class maintains an internal array of item definitions and handles memory management through its destructor. The parsing methods allow for processing structured data from a lexer, while
   the retrieval method enables efficient access to specific items using their names. This class serves as a container for glTF item definitions within the parsing and loading pipeline.

*/
class gltfItemArray
{
public:
	//! Destructor for gltfItemArray that cleans up allocated items.
	~gltfItemArray();
	gltfItemArray() {};

	//! Returns the number of items in the array
	int	 Num() { return items.Num(); }

	//! Adds a parsable item to the internal array of item definitions.
	void AddItemDef( parsable* item ) { items.Alloc() = item; }

	//! Parses tokens from a lexer and populates string pairs into a dictionary.
	int	 Fill( idLexer* lexer, idDict* strPairs );

	//! Parses a JSON object from the provided lexer, counting successfully parsed items.
	int	 Parse( idLexer* lexer, bool forwardLexer = false );

	//! Retrieves a pointer to an item from the array by its name.
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
	\brief A class representing a GLTF property item with a null array pointer initialization.
*/
class gltfPropertyItem
{
public:
	//! Initializes a gltfPropertyItem with a null array pointer.
	gltfPropertyItem() :
		array( nullptr )
	{
	}
	gltfPropertyArray* array;
	idToken			   item;
};

/*!
	\class gltfPropertyArray
	\brief A container class for handling glTF property arrays with iterator support.
*/
class gltfPropertyArray
{
public:
	//! Constructs a gltfPropertyArray object with the specified parser and array-of-structures flag.
	gltfPropertyArray( idLexer* Parser, bool AoS = true );

	//! Destructor for gltfPropertyArray that cleans up allocated memory and contents.
	~gltfPropertyArray();
	struct Iterator {
		gltfPropertyArray* array;
		gltfPropertyItem*  p;

		//! Returns a reference to the gltfPropertyItem that the iterator is currently pointing to.
		gltfPropertyItem&  operator*() { return *p; }

		//! Compares two iterator objects for inequality.
		bool			   operator!=( Iterator& rhs ) { return p != rhs.p; }

		//! Increments the iterator to move to the next element in the GLTF property array.
		void			   operator++();
	};

	//! Returns an iterator pointing to the beginning of the glTF property array.
	gltfPropertyArray::Iterator begin();

	//! Returns an iterator pointing to the end of the gltfPropertyArray.
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
	\brief A parser for GLTF and GLB formatted files that extracts and processes 3D asset data.

	This class provides functionality for loading and parsing GLTF and GLB files, extracting various components such as scenes, nodes, meshes, materials, textures, and animations. It manages the
   internal state of parsed data and supports setting parent-child relationships between nodes. The parser handles different sections of the GLTF format including assets, cameras, extensions, and
   buffer data. It operates on a single GLTF asset at a time and maintains internal state during the parsing process. The class supports both ASCII GLTF files and binary GLB files, with separate
   loading methods for each format. Memory management is handled internally by the class and does not expose ownership semantics beyond the standard constructor and destructor.

*/
class GLTF_Parser
{
public:
	~GLTF_Parser() { Shutdown(); }

	//! Initializes a GLTF_Parser object with default settings and state flags.
	GLTF_Parser();

	//! Cleans up and frees all resources associated with the GLTF parser
	void	  Shutdown();

	//! Parses a GLTF file and returns true if successful.
	bool	  Parse();

	//! Loads a GLTF file from the specified filename and parses its contents
	bool	  Load( idStr filename );

	//! Loads a GLB file from the specified filename and parses its contents
	bool	  loadGLB( idStr filename );

	// current/last loaded gltf asset and index offsets
	gltfData* currentAsset;
	idStr	  currentFile;

private:
	//! Sets the parent node for a given node and recursively updates all its children.
	void		 SetNodeParent( gltfNode* node, gltfNode* parent = nullptr );

	//! Parses a GLTF asset section from the provided token and prints it.
	void		 Parse_ASSET( idToken& token );

	//! Parses camera data from a GLTF file using the provided token.
	void		 Parse_CAMERAS( idToken& token );

	//! Parses the scene index from the token and sets it as the default scene in the current asset.
	void		 Parse_SCENE( idToken& token );

	//! Parses the SCENES section of a GLTF file and populates the scene data.
	void		 Parse_SCENES( idToken& token );

	//! Parses the NODES array from a GLTF file and populates node data.
	void		 Parse_NODES( idToken& token );

	//! Parses material definitions from a GLTF file using the provided token.
	void		 Parse_MATERIALS( idToken& token );

	//! Parses mesh data from a GLTF token into the current asset.
	void		 Parse_MESHES( idToken& token );

	//! Parses texture data from a GLTF file
	void		 Parse_TEXTURES( idToken& token );

	//! Parses image data from a GLTF file using the provided token
	void		 Parse_IMAGES( idToken& token );

	//! Parses accessor data from a GLTF file.
	void		 Parse_ACCESSORS( idToken& token );

	//! Parses buffer view data from a GLTF token
	void		 Parse_BUFFERVIEWS( idToken& token );

	//! Parses sampler data from a GLTF file
	void		 Parse_SAMPLERS( idToken& token );

	//! Parses buffer data from a GLTF file using the provided token.
	void		 Parse_BUFFERS( idToken& token );

	//! Parses animation data from a GLTF token into the current asset.
	void		 Parse_ANIMATIONS( idToken& token );

	//! Parses skin data from a GLTF file.
	void		 Parse_SKINS( idToken& token );

	//! Parses the extensions section of a GLTF file using the provided token to extract and process extension data.
	void		 Parse_EXTENSIONS( idToken& token );

	//! Parses the EXTENSIONS_USED property from the glTF file.
	void		 Parse_EXTENSIONS_USED( idToken& token );

	//! Parses the EXTENSIONS_REQUIRED section of a glTF file
	void		 Parse_EXTENSIONS_REQUIRED( idToken& token );

	//! Parses a GLTF property from a token and handles different GLTF sections accordingly
	gltfProperty ParseProp( idToken& token );

	//! Resolves a token string into its corresponding glTF property enumeration value.
	gltfProperty ResolveProp( idToken& token );

	idLexer		 parser;
	idToken		 token;

	bool		 buffersDone;
	bool		 bufferViewsDone;
};

/*!
	\class gltfManager
	\brief Manages GLTF file identifier extraction.
*/
class gltfManager
{
public:
	//! Parses a GLTF filename to extract an identifier, which can be either an integer ID or a name.
	static bool ExtractIdentifier( idStr& filename, int& id, idStr& name );
};
