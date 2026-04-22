/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015-2023 Robert Beckebans

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

#ifndef __MAPFILE_H__
#define __MAPFILE_H__
#include "gltfProperties.h"

/*
===============================================================================

	Reads or writes the contents of .map files into a standard internal
	format, which can then be moved into private formats for collision
	detection, map processing, or editor use.

	No validation (duplicate planes, null area brushes, etc) is performed.
	There are no limits to the number of any of the elements in maps.
	The order of entities, brushes, and sides is maintained.

===============================================================================
*/

const int	OLD_MAP_VERSION = 1;
// RB: added new map format
const int	DOOM3_MAP_VERSION	= 2;
const int	CURRENT_MAP_VERSION = 3;
// RB end
const int	DEFAULT_CURVE_SUBDIVISION	= 4;
const float DEFAULT_CURVE_MAX_ERROR		= 4.0f;
const float DEFAULT_CURVE_MAX_ERROR_CD	= 24.0f;
const float DEFAULT_CURVE_MAX_LENGTH	= -1.0f;
const float DEFAULT_CURVE_MAX_LENGTH_CD = -1.0f;

/*!
	\class idMapPrimitive
	\brief A base class for map primitives with type identification.
*/
class idMapPrimitive
{
public:
	// RB: new mesh primitive to work with Blender Ngons
	enum { TYPE_INVALID = -1, TYPE_BRUSH, TYPE_PATCH, TYPE_MESH };
	// RB end

	idDict epairs;

	//! Initializes an idMapPrimitive object with an invalid type.
	idMapPrimitive() { type = TYPE_INVALID; }
	virtual ~idMapPrimitive() { }

	//! Returns the type of the map primitive.
	int GetType() const { return type; }

protected:
	int type;
};

/*!
	\class idMapBrushSide
	\brief A class representing a side of a brush in a map, storing its material, plane, and texture properties.

	The idMapBrushSide class encapsulates the properties of a single side of a brush within a map structure. It maintains information about the material applied to the side, the geometric plane that
   defines its surface, and the texture mapping parameters including texture matrix and vectors. This class serves as a data container for brush side information during map processing and rendering.
   The class provides methods to access and modify these properties, including setting and retrieving material names, plane definitions, texture matrices, and texture coordinate vectors. It also
   includes functionality for converting texture projection formats to a specific Valve 220 format, which is useful for compatibility with certain map import/export workflows.

*/
class idMapBrushSide
{
	friend class idMapBrush;

public:
	//! Constructs a new idMapBrushSide object with default values.
	idMapBrushSide();
	~idMapBrushSide() { }

	//! Returns the material associated with this brush side.
	const char*	   GetMaterial() const { return material; }

	//! Sets the material of the map brush side to the specified value.
	void		   SetMaterial( const char* p ) { material = p; }

	//! Returns the plane data for this brush side.
	const idPlane& GetPlane() const { return plane; }

	//! Sets the plane of the map brush side to the specified plane.
	void		   SetPlane( const idPlane& p ) { plane = p; }

	//! Sets the texture matrix for the brush side using the provided 2D array of idVec3 vectors.
	void		   SetTextureMatrix( const idVec3 mat[2] )
	{
		texMat[0] = mat[0];
		texMat[1] = mat[1];
	}

	//! Retrieves the texture matrix from the brush side and stores it in two idVec3 vectors.
	void GetTextureMatrix( idVec3& mat1, idVec3& mat2 )
	{
		mat1 = texMat[0];
		mat2 = texMat[1];
	}

	//! Returns the texture coordinate vectors for this brush side.
	void GetTextureVectors( idVec4 v[2] ) const;

	// RB: support Valve 220 projection by TrenchBroom
	enum ProjectionType { PROJECTION_BP = 0, PROJECTION_VALVE220 = 1 };

	//! Returns the projection type of the map brush side.
	ProjectionType GetProjectionType() const { return projection; }

	//! Returns the texture size of the brush side.
	const idVec2i& GetTextureSize() const { return texSize; }

	//! Converts the brush side texture projection to Valve 220 format using the provided entity transform and texture collections
	void		   ConvertToValve220Format( const idMat4& entityTransform, idStrList& textureCollections );
	// RB end

protected:
	idStr	material;
	idPlane plane;
	idVec3	texMat[2];
	idVec3	origin;

public:
	// RB
	idVec3		   planepts[3]; // for writing back original planepts
	ProjectionType projection;
	idVec4		   texValve[2]; // alternative texture coordinate mapping
	idVec2		   texScale;
	idVec2i		   texSize;
};

ID_INLINE idMapBrushSide::idMapBrushSide()
{
	plane.Zero();
	texMat[0].Zero();
	texMat[1].Zero();
	origin.Zero();

	projection = PROJECTION_BP;
	texValve[0].Zero();
	texValve[1].Zero();
	texScale[0] = 1.0f;
	texScale[1] = 1.0f;
	texSize[0]	= 32;
	texSize[1]	= 32;
}

/*!
	\class idMapBrush
	\brief A class representing a brush primitive in a map, used for defining solid geometry with sides and planes.

	The idMapBrush class extends idMapPrimitive and provides functionality for managing brush geometry composed of multiple sides. It supports parsing brushes from different formats including standard
   map format, Q3 format, and Valve 220 format. The class handles operations such as adding brush sides, computing plane points from windings, and writing brush data to files in various formats. It
   also includes utilities for checking if a brush contains an origin marker and calculating checksums for geometry validation. The class is designed to work with lexer-based input streams for parsing
   and supports origin offsetting during parsing operations.

*/
class idMapBrush : public idMapPrimitive
{
public:
	//! Initializes a new instance of the idMapBrush class.
	idMapBrush()
	{
		type = TYPE_BRUSH;
		sides.Resize( 8, 4 );
	}
	~idMapBrush() { sides.DeleteContents( true ); }

	//! Parses a map brush from a lexer input stream with optional origin offset and format version.
	static idMapBrush* Parse( idLexer& src, const idVec3& origin, bool newFormat = true, int version = CURRENT_MAP_VERSION );

	//! Parses a Q3 map brush from the given lexer and origin.
	static idMapBrush* ParseQ3( idLexer& src, const idVec3& origin );

	//! Parses a Valve 220 format brush from the given lexer and applies the specified origin offset.
	static idMapBrush* ParseValve220( idLexer& src, const idVec3& origin );

	//! Writes brush data to a file
	bool			   Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Writes a brush in Valve 220 format to the specified file.
	bool			   WriteValve220( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Creates and returns an origin brush at the specified position with optional scaling
	static idMapBrush* MakeOriginBrush( const idVec3& origin, const idVec3& scale = vec3_one );

	//! Computes and sets plane points for brush sides from windings using the specified origin and entity/primitive identifiers.
	void			   SetPlanePointsFromWindings( const idVec3& origin, int entityNum, int primitiveNum );

	//! Returns the number of sides in the map brush.
	int				   GetNumSides() const { return sides.Num(); }

	//! Adds a brush side to the brush and returns the index of the added side.
	int				   AddSide( idMapBrushSide* side ) { return sides.Append( side ); }

	//! Returns the brush side at the specified index
	idMapBrushSide*	   GetSide( int i ) const { return sides[i]; }

	//! Calculates and returns the CRC checksum of the brush geometry.
	unsigned int	   GetGeometryCRC() const;

	//! Checks if the brush contains a side with the CONTENTS_ORIGIN material flag.
	bool			   IsOriginBrush() const;

protected:
	int											numSides;
	idList<idMapBrushSide*, TAG_IDLIB_LIST_MAP> sides;
};

/*!
	\class idMapPatch
	\brief A class representing a patch primitive for map data, inheriting from idMapPrimitive and idSurface_Patch.

	The idMapPatch class encapsulates patch geometry data used in map files, providing functionality to create, manipulate, and serialize patch primitives. It inherits from idMapPrimitive and
   idSurface_Patch, combining map primitive behavior with surface patch functionality. The class supports initialization with specific dimensions, material assignment, subdivision controls, and file
   I/O operations for patch data. It also provides methods for parsing patch data from a lexer and calculating geometric checksums. The patch can be explicitly subdivided, with controls for both
   horizontal and vertical subdivisions. Memory management is handled through standard C++ mechanisms without explicit ownership annotations.

*/
class idMapPatch : public idMapPrimitive, public idSurface_Patch
{
public:
	//! Constructs a new idMapPatch object with default values.
	idMapPatch();

	//! Initializes a new idMapPatch object with specified maximum width and height dimensions.
	idMapPatch( int maxPatchWidth, int maxPatchHeight );
	~idMapPatch() { }

	//! Parses patch data from a lexer, creating and returning a new idMapPatch object.
	static idMapPatch* Parse( idLexer& src, const idVec3& origin, bool patchDef3, int version );

	//! Writes patch data to a file with specified primitive number and origin offset.
	bool			   Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Returns the material string associated with the map patch.
	const char*		   GetMaterial() const { return material; }

	//! Sets the material of the map patch to the specified string.
	void			   SetMaterial( const char* p ) { material = p; }

	//! Returns the horizontal subdivisions value.
	int				   GetHorzSubdivisions() const { return horzSubdivisions; }

	//! Returns the number of vertical subdivisions for the map patch.
	int				   GetVertSubdivisions() const { return vertSubdivisions; }

	//! Returns true if the patch has explicit subdivisions defined, false otherwise.
	bool			   GetExplicitlySubdivided() const { return explicitSubdivisions; }

	//! Sets the horizontal subdivisions value for the map patch.
	void			   SetHorzSubdivisions( int n ) { horzSubdivisions = n; }

	//! Sets the vertical subdivisions value for the map patch.
	void			   SetVertSubdivisions( int n ) { vertSubdivisions = n; }

	//! Sets the explicitly subdivided flag for the map patch.
	void			   SetExplicitlySubdivided( bool b ) { explicitSubdivisions = b; }

	//! Calculates and returns a CRC value representing the geometric properties and material of the map patch.
	unsigned int	   GetGeometryCRC() const;

protected:
	idStr material;
	int	  horzSubdivisions;
	int	  vertSubdivisions;
	bool  explicitSubdivisions;
};

ID_INLINE idMapPatch::idMapPatch()
{
	type			 = TYPE_PATCH;
	horzSubdivisions = vertSubdivisions = 0;
	explicitSubdivisions				= false;
	width = height = 0;
	maxWidth = maxHeight = 0;
	expanded			 = false;
}

ID_INLINE idMapPatch::idMapPatch( int maxPatchWidth, int maxPatchHeight )
{
	type			 = TYPE_PATCH;
	horzSubdivisions = vertSubdivisions = 0;
	explicitSubdivisions				= false;
	width = height = 0;
	maxWidth	   = maxPatchWidth;
	maxHeight	   = maxPatchHeight;
	verts.SetNum( maxWidth * maxHeight );
	expanded = false;
}

/*!
	\class MapPolygon
	\brief A class representing a polygon in a map with associated material and vertex indexes.

	The MapPolygon class stores information about a polygon within a map, including its material properties and a list of vertex indexes that define its shape. It provides methods for initializing the
   polygon with a specific number of indexes, setting and retrieving the material, adding vertex indexes, and accessing the complete list of indexes. The class is designed to manage polygon data
   efficiently and support operations needed for map processing and rendering. The constructor allows for both empty initialization and initialization with a predefined number of indexes, while the
   destructor is defined but its specific purpose is unclear from the provided information.

*/
class MapPolygon
{
	friend class MapPolygonMesh;

public:
	//! Constructs an empty MapPolygon object.
	MapPolygon();

	//! Initializes a MapPolygon with a specified number of indexes.
	MapPolygon( int numIndexes );
	~MapPolygon() { }

	//! Returns the material string associated with this map polygon.
	const char* GetMaterial() const { return material; }

	//! Sets the material of the map polygon to the specified string.
	void		SetMaterial( const char* p ) { material = p; }

	//! Appends the provided index to the internal list of polygon indexes.
	void		AddIndex( int index ) { indexes.Append( index ); }

	//! Sets the vertex indexes of the polygon from the provided array.
	void		SetIndexes( const idTempArray<int>& _indexes )
	{
		indexes.Resize( _indexes.Num() );

		for( unsigned int i = 0; i < _indexes.Num(); i++ ) {
			indexes[i] = _indexes[i];
		}
	}

	//! Returns the list of indexes used by the map polygon
	const idList<int>& GetIndexes() const { return indexes; }

protected:
	idStr		material;
	idList<int> indexes; // [3..n] references to vertices for each face
};

ID_INLINE MapPolygon::MapPolygon()
{
}

ID_INLINE MapPolygon::MapPolygon( int numIndexes )
{
	// indexes.AssureSize( 3 );
}

/*!
	\class MapPolygonMesh
	\brief A mesh representation for map polygons that supports conversion from brushes, patches, and glTF data.

	The MapPolygonMesh class provides a structured representation of polygonal mesh data derived from map primitives such as brushes and patches. It encapsulates vertex and face data necessary for
   rendering and supports various conversion methods from different source formats. The class manages vertex lists and polygonal faces, allowing for operations like writing mesh data to files in
   different formats, calculating geometric checksums, and determining mesh properties such as opacity and areaportal status. It also includes functionality for parsing mesh definitions from lexer
   streams and converting glTF primitive data into its internal representation.

*/
class MapPolygonMesh : public idMapPrimitive
{
public:
	//! Initializes a new instance of the MapPolygonMesh class with default values.
	MapPolygonMesh();
	~MapPolygonMesh()
	{
		// verts.DeleteContents();
		// polygons.DeleteContents( true );
	}

	//! Converts a map brush into a polygon mesh representation for rendering.
	void					  ConvertFromBrush( const idMapBrush* brush, int entityNum, int primitiveNum );

	//! Converts a map patch into a polygon mesh representation.
	void					  ConvertFromPatch( const idMapPatch* patch, int entityNum, int primitiveNum );

	//! Converts a glTF mesh primitive into a MapPolygonMesh object with the specified transform applied
	static MapPolygonMesh*	  ConvertFromMeshGltf( const gltfMesh_Primitive* prim, gltfData* _data, const idMat4& transform );

	//! Parses a mesh definition from a lexer stream and returns a new MapPolygonMesh object.
	static MapPolygonMesh*	  Parse( idLexer& src, const idVec3& origin, float version = CURRENT_MAP_VERSION );

	//! Writes a mesh primitive to a file with specified origin offset
	bool					  Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Parses a JSON representation of a polygon mesh from the provided lexer and returns a new MapPolygonMesh object.
	static MapPolygonMesh*	  ParseJSON( idLexer& src );

	//! Writes polygon mesh data in JSON format to a file
	bool					  WriteJSON( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Returns the number of vertices in the polygon mesh.
	int						  GetNumVertices() const { return verts.Num(); }

	//! Adds a vertex to the mesh and returns its index.
	int						  AddVertex( const idDrawVert& v ) { return verts.Append( v ); }

	//! Appends a list of vertices to the mesh and returns the index of the first added vertex.
	int						  AddVertices( const idList<idDrawVert>& v ) { return verts.Append( v ); }

	//! Returns the number of polygons in the mesh.
	int						  GetNumPolygons() const { return polygons.Num(); }

	//! Returns the face at the specified index from the polygon mesh
	const MapPolygon&		  GetFace( int i ) const { return polygons[i]; }

	//! Calculates and returns a CRC value representing the geometric data of the mesh.
	unsigned int			  GetGeometryCRC() const;

	//! Returns a constant reference to the list of draw vertices.
	const idList<idDrawVert>& GetDrawVerts() const { return verts; }

	//! Returns true if the polygon mesh is opaque, false otherwise.
	bool					  IsOpaque() const { return opaque; }

	//! Returns true if the polygon mesh represents an areaportal.
	bool					  IsAreaportal() const;

	//! Computes and stores the bounding box of the mesh vertices in the provided bounds parameter
	void					  GetBounds( idBounds& bounds ) const;

private:
	//! Sets the contents and opacity flags for the polygon mesh based on its polygons.
	void SetContents();

	int	 originalType;

protected:
	idList<idDrawVert> verts; // vertices can be shared between polygons
	idList<MapPolygon> polygons;

	// derived data after parsing

	// material surface flags
	int				   contents;
	bool			   opaque;
};

/*!
	\class idMapEntity
	\brief Represents a map entity that can contain primitives and key-value pairs.

	This class serves as a container for map entities, managing a collection of primitives and associated key-value pairs. It supports parsing from different formats including traditional map format
   and JSON, and provides functionality to serialize entities to files. The class handles geometric data through its primitives and offers methods to calculate properties like CRC checksums for
   geometry validation. It also includes methods for managing primitive data, such as adding and removing primitives, as well as parsing and writing entity data to various formats.

*/
class idMapEntity
{
	friend class idMapFile;

public:
	typedef idList<idMapEntity*, TAG_IDLIB_LIST_MAP>  EntityList;
	typedef idList<idMapEntity*, TAG_IDLIB_LIST_MAP>& EntityListRef;
	typedef idList<idMapEntity*, TAG_IDLIB_LIST_MAP>* EntityListPtr;

	idDict											  epairs;
	idVec3											  originOffset { vec3_origin };

public:
	//! Constructs a new idMapEntity instance with an initialized hash size for its epairs collection.
	idMapEntity() { epairs.SetHashSize( 64 ); }
	~idMapEntity() { primitives.DeleteContents( true ); }

	//! Returns the count of entities found in the specified GLTF scene
	static int			GetEntities( gltfData* data, EntityListRef entities, int scene = 0 );

	//! Parses a map entity from a lexer stream, handling brushes, patches, and key-value pairs.
	static idMapEntity* Parse( idLexer& src, bool worldSpawn = false, int version = CURRENT_MAP_VERSION );

	//! Writes entity data to a file descriptor, including key-value pairs and primitive geometry.
	bool				Write( idFile* fp, int entityNum, bool valve220 ) const;

	//! Parses a JSON object from the lexer and constructs an idMapEntity object with its contents.
	static idMapEntity* ParseJSON( idLexer& src );

	//! Writes entity data in JSON format to the specified file.
	bool				WriteJSON( idFile* fp, int entityNum, int numEntities ) const;

	//! Returns the number of primitives contained in the map entity.
	int					GetNumPrimitives() const { return primitives.Num(); }

	//! Returns the primitive at the specified index from the map entity.
	idMapPrimitive*		GetPrimitive( int i ) const { return primitives[i]; }

	//! Adds a map primitive to the entity's list of primitives.
	void				AddPrimitive( idMapPrimitive* p ) { primitives.Append( p ); }

	//! Calculates and returns the CRC checksum for the geometry of all primitives in the map entity.
	unsigned int		GetGeometryCRC() const;

	//! Removes all primitive data associated with the map entity.
	void				RemovePrimitiveData();

protected:
	//! Calculates the average center position of all origin brushes in the map entity.
	void										CalculateBrushOrigin();

	idList<idMapPrimitive*, TAG_IDLIB_LIST_MAP> primitives;
};

/*!
	\class idMapFile
	\brief A class for managing and manipulating map files with support for multiple formats and conversion operations.

	The idMapFile class provides comprehensive functionality for loading, parsing, and saving map files in various formats including .map, .json, .gltf, and .glb. It maintains a collection of map
   entities and offers methods for entity manipulation, conversion between different map formats, and file I/O operations. The class supports operations such as converting brush and patch primitives
   to polygon mesh format, converting to Valve 220 format, and generating unique entity names. It also includes functionality for classifying entities for TrenchBroom compatibility and handling WAD
   texture name conversions to material names. The class tracks file modifications and provides CRC validation for map geometry. Memory management is handled through standard C++ practices, with
   methods for adding, removing, and finding entities within the map.

*/
class idMapFile
{
public:
	//! Initializes a new instance of the idMapFile class with default values.
	idMapFile();
	~idMapFile() { entities.DeleteContents( true ); }

	//! Parses a map file from the given filename, supporting multiple formats including .map, .json, .gltf, and .glb
	bool		 Parse( const char* filename, bool ignoreRegion = false, bool osPath = false, bool ignoreExtraEnts = false );

	//! Writes the map data to a file with the specified name and extension.
	bool		 Write( const char* fileName, const char* ext, bool fromBasePath = true );

	//! Writes the map data to a JSON file with the specified file name and extension.
	bool		 WriteJSON( const char* fileName, const char* ext, bool fromBasePath = true );

	//! Writes a diff file comparing this map to another map
	bool		 WriteDiff( const idMapFile* other, const char* fileName, const char* ext, bool fromBasePath = true );

	//! Converts brush and patch primitives to polygon mesh format for all entities in the map.
	bool		 ConvertToPolygonMeshFormat();

	//! Converts the map file to Valve 220 format and optionally recalculates plane points.
	bool		 ConvertToValve220Format( bool recalcPlanePoints );

	//! Classifies map entities as either brush or point entities for TrenchBroom compatibility.
	void		 ClassifyEntitiesForTrenchBroom( idDict& classTypeOverview );

	//! Converts Wad texture names to valid Doom 3 materials and gives every entity a unique name
	bool		 ConvertQuakeToDoom();

	//! Fixes duplicated names in group instances by generating unique names for conflicting entities.
	void		 FixDuplicatedNamesInGroupInstances();

	//! Returns the number of entities in the map file.
	int			 GetNumEntities() const { return entities.Num(); }

	//! Returns the entity at the specified index from the map file.
	idMapEntity* GetEntity( int i ) const { return entities[i]; }

	//! Returns the name of the map file without the file extension
	const char*	 GetName() const { return name; }

	//! Returns the file time stamp of the map file.
	ID_TIME_T	 GetFileTime() const { return fileTime; }

	//! Retrieves the CRC value for the map geometry.
	unsigned int GetGeometryCRC() const { return geometryCRC; }

	//! Returns true if the map file on disk has changed since it was last loaded.
	bool		 NeedsReload();

	//! Appends a map entity to the list of entities and returns the index of the newly added entity.
	int			 AddEntity( idMapEntity* mapentity );

	//! Finds and returns a map entity with the specified name, or NULL if not found.
	idMapEntity* FindEntity( const char* name ) const;

	//! Finds and returns the map entity located at the specified origin position.
	idMapEntity* FindEntityAtOrigin( const idVec3& org ) const;

	//! Removes a map entity from the file and deletes it.
	void		 RemoveEntity( idMapEntity* mapEnt );

	//! Removes all map entities with the specified class name.
	void		 RemoveEntities( const char* classname );

	//! Removes all entities from the map file.
	void		 RemoveAllEntities();

	//! Removes primitive data from all entities in the map file.
	void		 RemovePrimitiveData();

	//! Returns true if the map file has primitive data.
	bool		 HasPrimitiveData() { return hasPrimitiveData; }

	//! Returns true if the map file is in GLTF format.
	bool		 IsGLTF() const { return gltfFormat; }

	//! Adds a material to a collection of texture collections, ensuring uniqueness.
	static void	 AddMaterialToCollection( const char* material, idStrList& textureCollections );

	//! Converts a WAD texture name to a material name using predefined conversion rules.
	static void	 WadTextureToMaterial( const char* material, idStr& matName );

protected:
	int						version;
	ID_TIME_T				fileTime;
	unsigned int			geometryCRC;
	idMapEntity::EntityList entities;
	idStr					name;
	bool					hasPrimitiveData;
	bool					valve220Format; // RB: for TrenchBroom support
	bool					gltfFormat;

private:
	//! Computes and sets the cumulative CRC value for all geometry in the map file.
	void		SetGeometryCRC();

	//! Generates a unique entity name for a given class name.
	const char* GetUniqueEntityName( const char* classname ) const;
};

ID_INLINE idMapFile::idMapFile()
{
	version		= CURRENT_MAP_VERSION;
	fileTime	= 0;
	geometryCRC = 0;
	entities.Resize( 1024, 256 );
	hasPrimitiveData = false;
	valve220Format	 = false;
	gltfFormat		 = false;
}

#endif /* !__MAPFILE_H__ */
