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
	\brief Base class for map primitives used in the Roguestrad engine.

	The idMapPrimitive class serves as the abstract base class for all map primitive types within the Roguestrad engine's map system. It provides the fundamental interface and common functionality for
   geometric elements that can be stored and manipulated within map data structures. The class maintains a type identifier that determines the specific geometric nature of each primitive instance,
   with the constructor initializing this to an invalid state to ensure proper initialization before use. Virtual destructor allows for proper cleanup of derived primitive types. The GetType method
   provides a consistent interface for querying the primitive's geometric type, enabling type-specific processing throughout the engine's map handling systems.

*/
class idMapPrimitive
{
public:
	// RB: new mesh primitive to work with Blender Ngons
	enum { TYPE_INVALID = -1, TYPE_BRUSH, TYPE_PATCH, TYPE_MESH };
	// RB end

	idDict epairs;

	/*!
		\brief Initializes a new instance of idMapPrimitive with an invalid type.

		This constructor sets the type member variable of the idMapPrimitive object to TYPE_INVALID, indicating that the primitive has not been properly initialized or does not represent a valid
	   geometric type.

	*/
	idMapPrimitive() { type = TYPE_INVALID; }
	virtual ~idMapPrimitive() { }

	//! Returns the type of the map primitive.
	int GetType() const { return type; }

protected:
	int type;
};

/*!
	\class idMapBrushSide
	\brief Represents a single side of a brush in a map file with material, plane, and texture coordinate information.

	The idMapBrushSide class encapsulates the geometric and rendering properties of a single side of a brush within a map file. It maintains the plane equation that defines the bounding surface, the
   material name for rendering, and texture coordinate transformation information. The class supports different texture projection types and provides methods for converting between various texture
   coordinate formats, including Valve 220 format. This class is designed to be used internally by map parsing and preprocessing systems within the engine, serving as a fundamental building block for
   brush-based geometry representation in map files.

*/
class idMapBrushSide
{
	friend class idMapBrush;

public:
	/*!
		\brief Initializes a new instance of the idMapBrushSide class with default values.

		This constructor initializes all members of the idMapBrushSide class to their default values. The plane is zeroed, texture matrix components are set to zero, the origin is zeroed, the
	   projection type is set to PROJECTION_BP, texture valve components are zeroed, texture scale values are set to 1.0f, and texture size values are set to 32.

	*/
	idMapBrushSide();
	~idMapBrushSide() { }

	//! Returns the material name associated with this brush side.
	const char*	   GetMaterial() const { return material; }

	/*!
		\brief Sets the material of the brush side to the provided string.

		This function assigns the material property of the brush side to the string provided as the parameter. It is a simple assignment operation that updates the internal material reference.

		\param p The string representing the new material for the brush side
	*/
	void		   SetMaterial( const char* p ) { material = p; }

	//! Returns the plane of the map brush side.
	const idPlane& GetPlane() const { return plane; }

	/*!
		\brief Sets the plane equation for this brush side.

		This function assigns the provided plane equation to the internal plane member of the brush side. The plane equation is typically used to define the bounding surface of a brush in a map file.

		\param p The plane equation to assign to this brush side
	*/
	void		   SetPlane( const idPlane& p ) { plane = p; }

	/*!
		\brief Sets the texture matrix for the brush side using the provided 2D transformation matrix.

		This function assigns the two vector components of the input 2D matrix to the internal texture matrix storage of the brush side. The matrix is typically used for applying texture coordinates
	   transformations in rendering.

		\param mat A 2D array of idVec3 vectors representing the texture transformation matrix
	*/
	void		   SetTextureMatrix( const idVec3 mat[2] )
	{
		texMat[0] = mat[0];
		texMat[1] = mat[1];
	}

	/*!
		\brief Returns the texture coordinate transformation matrix for this brush side as two idVec3 vectors.

		This function extracts the texture matrix components from the internal texture matrix representation of the brush side. The matrix is stored as two idVec3 vectors, where mat1 represents the
	   first row and mat2 represents the second row of the transformation matrix used for texture coordinate mapping.

		\param mat1 First row of the texture matrix returned as an idVec3 vector
		\param mat2 Second row of the texture matrix returned as an idVec3 vector
	*/
	void GetTextureMatrix( idVec3& mat1, idVec3& mat2 )
	{
		mat1 = texMat[0];
		mat2 = texMat[1];
	}

	/*!
		\brief Computes and returns the texture coordinate vectors for this brush side

		This function calculates the texture coordinate vectors for a brush side based on the projection type. For Valve220 projection, it uses precomputed texture values and scale factors. For other
	   projections, it computes the texture vectors using axis-based calculations and the texture matrix. The resulting vectors are stored in the provided array of idVec4 structures, where each vector
	   represents the U and V texture coordinate directions respectively

		\param v output array of two texture coordinate vectors
	*/
	void GetTextureVectors( idVec4 v[2] ) const;

	// RB: support Valve 220 projection by TrenchBroom
	enum ProjectionType { PROJECTION_BP = 0, PROJECTION_VALVE220 = 1 };

	//! Returns the projection type of the map brush side.
	ProjectionType GetProjectionType() const { return projection; }

	//! Returns the texture size of the brush side.
	const idVec2i& GetTextureSize() const { return texSize; }

	/*!
		\brief Converts the brush side texture projection to Valve 220 format using the provided entity transformation and texture collections.

		This function transforms the current brush side's texture coordinates into the Valve 220 format. It computes the appropriate texture matrix based on the plane normal and the entity's
	   transformation, and then adjusts for texture scaling and shifting. The function also ensures that the material is added to the provided texture collections and updates internal texture size
	   information based on the image dimensions.

		\param entityTransform The transformation matrix of the entity this brush side belongs to
		\param textureCollections List of texture collections to add the material to
	*/
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
	\brief A map brush primitive that represents a solid volume with textured sides in a level map.

	The idMapBrush class represents a solid brush primitive used in level mapping, inheriting from idMapPrimitive and specialized for handling brush geometry with multiple textured sides. It supports
   parsing from various map formats including Doom 3, Quake 3, and Valve 220, and provides functionality for serializing brush data to file formats. The class maintains a collection of brush sides and
   handles geometric operations such as setting plane points from windings, calculating geometry checksums, and identifying special brush types like origin brushes. Brush parsing methods handle
   different map versions and format variations, while write methods support outputting brush definitions in multiple formats for tool compatibility.

*/
class idMapBrush : public idMapPrimitive
{
public:
	/*!
		\brief Initializes a new instance of the idMapBrush class.

		The constructor initializes the type member to TYPE_BRUSH and resizes the sides collection to accommodate up to 8 sides with an initial capacity of 4.

	*/
	idMapBrush()
	{
		type = TYPE_BRUSH;
		sides.Resize( 8, 4 );
	}
	~idMapBrush() { sides.DeleteContents( true ); }

	/*!
		\brief Parses a map brush from a lexer stream, including its sides, planes, textures, and associated epairs.

		This function reads a map brush definition from the provided lexer stream. It handles both new and old format brush definitions, parsing brush sides with their plane definitions, texture
	   matrices, and materials. The function also processes epairs that are used for editor properties. The brush is constructed with the provided origin offset and can handle different map versions.
	   Memory is allocated for the brush and its sides, and all parsed data is properly initialized.

		\param src Lexer stream to read the brush definition from
		\param origin Origin offset to apply to plane points
		\param newFormat Flag indicating whether to parse in new or old format
		\param version Map version to determine parsing behavior for materials
		\return Pointer to the newly created idMapBrush object, or NULL if parsing fails
	*/
	static idMapBrush* Parse( idLexer& src, const idVec3& origin, bool newFormat = true, int version = CURRENT_MAP_VERSION );

	/*!
		\brief Parses a brush from a Q3 map file using the provided lexer and origin offset.

		This function reads a brush definition from a map file using the provided lexer. It processes each brush side by reading the plane points, material name, and texture coordinates. The function
	   handles the conversion of Q3-specific brush format to the internal map brush representation. The origin parameter is used to offset all plane points in the brush.

		\param src Lexer object used to parse the map file
		\param origin Offset to be applied to all brush plane points
		\return Pointer to the newly created idMapBrush object, or NULL if parsing fails
	*/
	static idMapBrush* ParseQ3( idLexer& src, const idVec3& origin );

	/*!
		\brief Parses a Valve 220 format brush from the provided lexer stream and origin.

		This function reads brush side definitions from a lexer stream in Valve 220 format, including plane points, materials, texture coordinates, and scaling information. It constructs an idMapBrush
	   object with the parsed sides and returns it. The function handles parsing of texture matrix values, material names, and applies the specified origin offset to the brush sides. Texture
	   parameters are processed to build proper texture transformation matrices. The function manages memory allocation for brush sides and handles errors by cleaning up allocated memory and returning
	   null on failure.

		\param src Lexer stream containing the brush definition
		\param origin Origin offset to apply to brush plane points
		\return Pointer to the newly created idMapBrush object, or NULL if parsing fails
	*/
	static idMapBrush* ParseValve220( idLexer& src, const idVec3& origin );

	/*!
		\brief Writes the brush data to a file including its epairs, sides, and texture coordinates.

		This function serializes the brush data into a formatted text representation that can be written to a file. It starts by writing the primitive number and opening the brush definition block,
	   then iterates through all key-value pairs stored in epairs to write them. Next, it processes each brush side, writing the plane equation coefficients and texture matrix information along with
	   the material name. Finally, it closes the brush definition block. The function returns true upon successful completion.

		\param fp File pointer to which the brush data will be written
		\param primitiveNum The primitive number assigned to this brush for identification
		\param origin The origin point used for positioning the brush in the map
		\return True if the brush data was successfully written to the file, false otherwise
	*/
	bool			   Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	/*!
		\brief Writes a brush in Valve 220 format to the specified file

		This function outputs a brush definition in the Valve 220 map format to the provided file handle. It writes the brush's epairs (key-value properties) followed by each side of the brush. Each
	   side includes three points defining the plane, the texture name (with leading "textures/" stripped if present), and texture coordinate transformation data. The function returns true to indicate
	   successful completion

		\param fp File handle to write the brush data to
		\param primitiveNum The brush number to use in the output
		\param origin The origin point for the brush (unused in current implementation)
		\return True if the brush data was successfully written to the file
	*/
	bool			   WriteValve220( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	/*!
		\brief Creates and returns an origin brush for use in map parsing, with optional scaling

		This function generates an origin brush used in map parsing, particularly for TrenchBroom compatibility. It creates a brush with specific geometry and texture coordinates, then applies a
	   transformation based on the provided origin and scale parameters. The brush is constructed with a fixed set of six sides that define a cubic structure, and the scaling is applied through a
	   transformation matrix that modifies the brush's dimensions along each axis.

		\param origin The world position where the brush should be centered
		\param scale The scaling factor for each axis of the brush
		\return A pointer to the newly created idMapBrush object representing the origin brush
	*/
	static idMapBrush* MakeOriginBrush( const idVec3& origin, const idVec3& scale = vec3_one );

	/*!
		\brief Sets plane points for brush sides using windings derived from the brush's planes and origin

		This function computes the plane points for each side of a map brush by generating base windings from the brush's planes and clipping them against other brush planes. It handles degenerate
	   planes by fixing them and ensures that the resulting windings have at least three points before setting the plane points. The points are adjusted by adding the specified origin to each vertex.
	   The function reports warnings when base windings have no points or when there's no intersection with other brush planes. This is used for generating proper brush geometry from plane definitions
	   in map files.

		\param origin The origin offset to be added to each computed plane point
		\param entityNum The entity number for debugging output
		\param primitiveNum The primitive number for debugging output
	*/
	void			   SetPlanePointsFromWindings( const idVec3& origin, int entityNum, int primitiveNum );

	//! Returns the number of sides in the map brush
	int				   GetNumSides() const { return sides.Num(); }

	/*!
		\brief Adds a brush side to the brush and returns the index of the newly added side.

		This function appends the provided brush side to the internal collection of sides for the brush. It returns the index at which the side was added, allowing callers to reference the side later.
	   The function assumes that the brush side pointer is valid and that the internal collection can accommodate the new side.

		\param side Pointer to the brush side to be added to the brush.
		\return The index of the newly added brush side within the internal collection of sides.
	*/
	int				   AddSide( idMapBrushSide* side ) { return sides.Append( side ); }

	/*!
		\brief Returns a pointer to the brush side at the specified index

		This function provides access to individual brush sides stored in the map brush. The returned pointer can be used to retrieve information about the brush side such as its plane equation and
	   material. The function performs no bounds checking, so the caller must ensure that the index i is valid and within the range of available brush sides

		\param i Index of the brush side to retrieve, must be non-negative and less than the number of brush sides
		\return Pointer to the idMapBrushSide object at the specified index, or NULL if the index is out of bounds
	*/
	idMapBrushSide*	   GetSide( int i ) const { return sides[i]; }

	//! Calculates and returns a CRC value representing the geometric properties of the map brush.
	unsigned int	   GetGeometryCRC() const;

	//! Returns true if the brush contains a side with the CONTENTS_ORIGIN material flag.
	bool			   IsOriginBrush() const;

protected:
	int											numSides;
	idList<idMapBrushSide*, TAG_IDLIB_LIST_MAP> sides;
};

/*!
	\class idMapPatch
	\brief A map patch primitive that represents a subdivided surface patch with material properties and geometric data.

	The idMapPatch class represents a patch primitive used in map data structures, inheriting from both idMapPrimitive and idSurface_Patch. It encapsulates patch geometry with subdivision parameters,
   material information, and vertex data for rendering. The class supports both explicit subdivision control and automatic subdivision based on geometric complexity. It provides functionality for
   parsing patch data from map files, writing patch data to files in different formats, and managing material assignments. The patch can be configured with maximum dimensions and subdivision counts to
   control the level of detail in the rendered surface. The class maintains CRC hashes for geometry and material data to support caching and change detection. Memory management is handled through
   standard C++ object lifetime management with no explicit ownership transfer semantics.

*/
class idMapPatch : public idMapPrimitive, public idSurface_Patch
{
public:
	/*!
		\brief Constructs an idMapPatch object with default values.

		The constructor initializes all member variables to their default states. The type is set to TYPE_PATCH, and subdivision counts are initialized to zero. Explicit subdivision flags, dimensions,
	   and expansion state are also set to their default values.

	*/
	idMapPatch();

	/*!
		\brief Constructs an idMapPatch object with specified maximum width and height.

		Initializes the patch with the given maximum dimensions and sets up internal data structures for vertex storage. The patch type is set to TYPE_PATCH, and subdivision parameters are initialized
	   to default values.

		\param maxPatchWidth Maximum width of the patch
		\param maxPatchHeight Maximum height of the patch
	*/
	idMapPatch( int maxPatchWidth, int maxPatchHeight );
	~idMapPatch() { }

	/*!
		\brief Parses a map patch from the provided lexer source with optional origin offset and version handling.

		This function is responsible for parsing map patch data from a lexer source. It handles both patchDef2 and patchDef3 formats, reading patch information, material name, and vertex data. The
	   patch data is parsed with optional origin offset applied to vertex positions. The function also manages key-value pairs associated with the patch. It performs error checking and returns NULL on
	   parse failures.

		\param src Lexer source to parse the patch data from
		\param origin Origin offset to apply to vertex positions
		\param patchDef3 Flag indicating whether patchDef3 format is expected
		\param version Version number for compatibility handling
		\return Pointer to the newly created idMapPatch object on successful parsing, or NULL on failure
	*/
	static idMapPatch* Parse( idLexer& src, const idVec3& origin, bool patchDef3, int version );

	/*!
		\brief Writes patch data to a file in a specific format based on subdivision state and primitive number

		This function writes patch data to the provided file handle in either patchDef2 or patchDef3 format depending on whether the patch is explicitly subdivided. It outputs the material name,
	   dimensions, and vertex data including coordinates and texture coordinates, with an optional origin offset applied to the vertex positions. The function returns true upon successful completion

		\param fp File handle to write the patch data to
		\param primitiveNum The primitive number to be used in the output comment
		\param origin Offset to be added to the vertex coordinates when writing to file
		\return True if the patch data was successfully written to the file
	*/
	bool			   Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Returns the material name associated with this patch.
	const char*		   GetMaterial() const { return material; }

	/*!
		\brief Sets the material of the map patch to the specified material name.

		This function assigns the provided material name to the internal material member of the map patch object. The material name is typically a string identifier that refers to a texture or shader
	   used for rendering the patch.

		\param p The name of the material to set for the map patch.
	*/
	void			   SetMaterial( const char* p ) { material = p; }

	//! Returns the number of horizontal subdivisions for the map patch.
	int				   GetHorzSubdivisions() const { return horzSubdivisions; }

	//! Returns the vertical subdivisions of the map patch.
	int				   GetVertSubdivisions() const { return vertSubdivisions; }

	//! Returns whether the patch has explicit subdivisions specified.
	bool			   GetExplicitlySubdivided() const { return explicitSubdivisions; }

	/*!
		\brief Sets the horizontal subdivision count for the patch geometry.

		This function assigns the specified integer value to the horzSubdivisions member variable of the idMapPatch class. It is used to control the level of subdivision in the horizontal direction
	   for patch geometry.

		\param n The number of horizontal subdivisions to set
	*/
	void			   SetHorzSubdivisions( int n ) { horzSubdivisions = n; }

	/*!
		\brief Sets the vertical subdivision count for the patch.

		This function assigns the specified integer value to the vertSubdivisions member variable of the idMapPatch class. It is used to control the level of subdivision in the vertical direction for
	   patch geometry.

		\param n The number of vertical subdivisions to set
	*/
	void			   SetVertSubdivisions( int n ) { vertSubdivisions = n; }

	/*!
		\brief Sets whether the patch has explicitly subdivided control points.
		\param b true if the patch control points have been explicitly subdivided, false otherwise
	*/
	void			   SetExplicitlySubdivided( bool b ) { explicitSubdivisions = b; }

	//! Returns the CRC hash of the patch geometry and material data
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
	\brief A polygon class for managing vertex indexes and material properties in map geometry.

	The MapPolygon class represents a polygon in the map geometry, storing vertex indexes that define its shape and a material name that defines its appearance. It provides methods for constructing
   polygons with a specific number of indexes, adding individual vertex indexes, setting all indexes at once, and retrieving the list of indexes. The class also allows setting and getting the material
   name associated with the polygon, which is used to reference material definitions in the game's asset system. The implementation uses an internal array to store vertex indexes and ensures that
   polygons are initialized with at least 3 elements to form a valid polygon structure.

*/
class MapPolygon
{
	friend class MapPolygonMesh;

public:
	//! Constructor for the MapPolygon class that initializes an empty polygon.
	MapPolygon();

	/*!
		\brief Constructs a MapPolygon object with a specified number of indexes.

		The constructor initializes a MapPolygon object with the given number of indexes. The actual index storage is managed by the indexes member, which is assured to have at least 3 elements.

		\param numIndexes The initial number of indexes for the polygon
	*/
	MapPolygon( int numIndexes );
	~MapPolygon() { }

	//! Returns the material name associated with the map polygon.
	const char* GetMaterial() const { return material; }

	/*!
		\brief Sets the material of the map polygon to the specified material name.

		This function assigns the provided material name to the material member of the MapPolygon instance. The material name is typically a string identifier that refers to a specific material
	   definition in the game's asset system. The function performs a simple assignment operation without any validation or additional processing.

		\param p The name of the material to set for this map polygon
	*/
	void		SetMaterial( const char* p ) { material = p; }

	/*!
		\brief Adds a vertex index to the polygon's internal index array.

		This function appends the provided vertex index to the internal array of indexes that define the polygon's structure. It is used to build up the list of vertex indices that make up the
	   polygon's shape.

		\param index The vertex index to add to the polygon's index list
	*/
	void		AddIndex( int index ) { indexes.Append( index ); }

	/*!
		\brief Sets the vertex indexes for the polygon using the provided array of integers.

		This function updates the internal vertex index array of the polygon by copying the contents of the provided index array. It first resizes the internal array to match the size of the input
	   array and then copies each element from the input array to the internal array.

		\param _indexes Array of vertex indexes to assign to the polygon
	*/
	void		SetIndexes( const idTempArray<int>& _indexes )
	{
		indexes.Resize( _indexes.Num() );

		for( unsigned int i = 0; i < _indexes.Num(); i++ ) {
			indexes[i] = _indexes[i];
		}
	}

	//! Returns the list of vertex indexes used by the map polygon.
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
	\brief A class representing polygon mesh data for map primitives in the Roguestrad engine.

	The MapPolygonMesh class serves as a container for polygon mesh data used in map primitives, supporting conversion from various source formats including brushes, patches, glTF meshes, and
   text-based representations. It maintains vertex and polygon information with support for texture coordinates, normals, and material assignments. The class provides functionality for parsing mesh
   data from lexer streams, writing mesh data to files in both binary and JSON formats, and converting between different mesh representations. It supports operations for adding vertices, retrieving
   polygon data, calculating bounding volumes, and determining mesh properties such as opacity and content flags. The mesh data is organized as a collection of polygons with shared vertex data,
   enabling efficient rendering and spatial operations within the engine's map system.

*/
class MapPolygonMesh : public idMapPrimitive
{
public:
	/*!
		\brief Initializes a new MapPolygonMesh object with default settings.

		Constructs a MapPolygonMesh object and sets its type to TYPE_MESH. The original type is also initialized to TYPE_MESH. The polygons container is resized to accommodate 8 polygons with 4
	   vertices each. The contents are set to CONTENTS_SOLID and the opaque flag is set to true.

	*/
	MapPolygonMesh();
	~MapPolygonMesh()
	{
		// verts.DeleteContents();
		// polygons.DeleteContents( true );
	}

	/*!
		\brief Converts a map brush into a polygon mesh representation for use in the engine

		This function processes a map brush and converts it into a polygon mesh structure. It handles degenerate plane fixing, computes winding intersections, and builds the vertex and index data for
	   the resulting mesh. The function processes each brush side, calculates texture coordinates using the brush's texture matrix, and supports Valve 220 projection type for texture mapping. The
	   resulting mesh contains polygons with proper vertices, texture coordinates, and normals. If a brush is found to be invalid, it returns early without processing further.

		\param brush Pointer to the map brush object to convert
		\param entityNum The entity number this brush belongs to
		\param primitiveNum The primitive number of this brush within the entity
	*/
	void					  ConvertFromBrush( const idMapBrush* brush, int entityNum, int primitiveNum );

	/*!
		\brief Converts a patch mesh from a map patch into a polygon mesh representation.

		This function transforms a patch mesh defined by an idMapPatch object into a polygon mesh structure. It creates a new surface patch, subdivides it based on whether it was explicitly subdivided
	   or not, and then converts the patch data into vertex and polygon arrays. The function also sets the appropriate material for the polygons and marks the mesh as non-opaque to prevent BSP splits.

		\param patch Pointer to the map patch to convert
		\param entityNum Entity number associated with the patch
		\param primitiveNum Primitive number for the patch within the entity
	*/
	void					  ConvertFromPatch( const idMapPatch* patch, int entityNum, int primitiveNum );

	/*!
		\brief Converts a glTF mesh primitive into a MapPolygonMesh with optional transformation and material assignment

		This function processes a glTF mesh primitive and converts it into a MapPolygonMesh object that can be used within the Doom 3 BFG engine. It handles vertex data including positions, normals,
	   texture coordinates, tangents, and weights, while applying the provided transformation matrix to position and normal data. The function also assigns materials from the glTF data or falls back
	   to a default material if none is specified. The conversion respects the glTF accessor and buffer view structure to properly extract vertex attributes.

		\param prim Pointer to the glTF mesh primitive containing the mesh data to convert
		\param _data Pointer to the glTF data structure containing all buffer views, accessors, and materials
		\param transform Transformation matrix to apply to vertex positions and normals during conversion
		\return Pointer to the newly created MapPolygonMesh object containing the converted mesh data
	*/
	static MapPolygonMesh*	  ConvertFromMeshGltf( const gltfMesh_Primitive* prim, gltfData* _data, const idMat4& transform );

	/*!
		\brief Parses a map polygon mesh from a lexer stream with optional version parameter.

		This function reads mesh data from a lexer stream, including vertex information and polygon definitions. It handles parsing of vertex coordinates, texture coordinates, normals, and polygon
	   material assignments. The function allocates a new MapPolygonMesh object and populates it with the parsed data. It returns NULL on parsing errors or if the input stream is malformed.

		\param src Lexer stream to parse the mesh data from
		\param origin Origin vector to be subtracted from vertex coordinates
		\param version Version number of the mesh format (defaults to CURRENT_MAP_VERSION)
		\return Pointer to the newly created MapPolygonMesh object on successful parsing, or NULL on failure
	*/
	static MapPolygonMesh*	  Parse( idLexer& src, const idVec3& origin, float version = CURRENT_MAP_VERSION );

	/*!
		\brief Writes polygon mesh data to a file pointer, including vertex and polygon information with an optional origin offset

		This function serializes polygon mesh data to a file handle, outputting vertex coordinates, texture coordinates, normals, and polygon material assignments. The vertex positions are written
	   relative to the specified origin offset, and the output format follows a specific mesh definition structure used by the engine. The function returns true upon successful completion of the write
	   operation

		\param fp File pointer to write the mesh data to
		\param primitiveNum Identifier number for the mesh primitive being written
		\param origin Origin offset to apply to vertex positions when writing
		\return True if the mesh data was successfully written to the file
	*/
	bool					  Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	/*!
		\brief Parses a JSON formatted map polygon mesh from the provided lexer stream and returns a new mesh object

		This function reads a JSON formatted mesh definition from the given lexer stream and constructs a MapPolygonMesh object. The mesh can contain vertex data including position, texture
	   coordinates, and normals, as well as polygon definitions with material references and index lists. The function handles nested structures and properly processes the data to build a complete
	   mesh representation. Memory management is handled internally with proper cleanup on error conditions.

		\param src Lexer stream containing the JSON formatted mesh data to parse
		\return A pointer to the newly created MapPolygonMesh object parsed from the JSON input, or NULL if parsing fails
		\throws NULL pointer if parsing fails due to malformed JSON or missing required elements
	*/
	static MapPolygonMesh*	  ParseJSON( idLexer& src );

	/*!
		\brief Writes polygon mesh data in JSON format to a file, including vertex positions, texture coordinates, normals, and polygon indices.

		This function serializes the mesh data of a polygon into a JSON structure. It outputs the primitive number, original type (brush or patch), vertex data with XYZ coordinates, texture
	   coordinates (st), and normals, followed by polygon data with material names and indices. The mesh data is written to the provided file pointer with proper JSON formatting and indentation. The
	   vertex indices are written in reverse order to maintain correct winding order.

		\param fp File pointer to write the JSON data to
		\param primitiveNum The primitive number to be written in the JSON output
		\param origin The origin vector used for positioning the mesh vertices
		\return Always returns true, indicating successful completion of the write operation
	*/
	bool					  WriteJSON( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	//! Returns the number of vertices in the mesh.
	int						  GetNumVertices() const { return verts.Num(); }

	/*!
		\brief Adds a vertex to the mesh and returns its index.

		This function appends a vertex to the internal vertex list of the mesh and returns the index at which the vertex was added. The vertex is copied from the input parameter and stored in the
	   mesh's vertex collection.

		\param v The vertex to be added to the mesh
		\return The index of the newly added vertex in the mesh's vertex list
	*/
	int						  AddVertex( const idDrawVert& v ) { return verts.Append( v ); }

	/*!
		\brief Adds a list of vertices to the mesh and returns the index of the first added vertex.

		This function appends the provided list of vertices to the internal vertex buffer of the mesh. It is used to incrementally build the mesh by adding new vertex data. The returned index
	   represents the position in the vertex buffer where the first of the new vertices was inserted.

		\param v The list of vertices to append to the mesh
		\return The index of the first vertex in the mesh's vertex buffer that was added by this operation
	*/
	int						  AddVertices( const idList<idDrawVert>& v ) { return verts.Append( v ); }

	//! Returns the number of polygons in the mesh.
	int						  GetNumPolygons() const { return polygons.Num(); }

	/*!
		\brief Retrieves a specific polygon from the mesh by its index.

		This function returns a constant reference to the polygon at the specified index within the mesh. It is used to access individual polygons for rendering or processing. The index must be valid,
	   otherwise the behavior is undefined.

		\param i The index of the polygon to retrieve.
		\return A constant reference to the MapPolygon at the specified index.
	*/
	const MapPolygon&		  GetFace( int i ) const { return polygons[i]; }

	//! Calculates and returns a CRC value representing the geometry data of the mesh.
	unsigned int			  GetGeometryCRC() const;

	//! Returns a constant reference to the list of draw vertices.
	const idList<idDrawVert>& GetDrawVerts() const { return verts; }

	//! Returns true if the polygon mesh is opaque, false otherwise.
	bool					  IsOpaque() const { return opaque; }

	//! Returns true if the map polygon mesh represents an area portal.
	bool					  IsAreaportal() const;

	/*!
		\brief Computes and returns the bounding box of all vertices in the mesh

		This function calculates the axis-aligned bounding box that contains all vertices in the mesh. If the mesh has no vertices, it clears the provided bounds object. The function iterates through
	   all vertices to find the minimum and maximum coordinates along each axis, updating the bounds accordingly. The resulting bounds represent the tightest fitting axis-aligned bounding box for the
	   mesh geometry.

		\param bounds The bounds object to be updated with the mesh bounding box
	*/
	void					  GetBounds( idBounds& bounds ) const;

private:
	/*!
		\brief Sets the contents and opaque flags for the map polygon mesh based on its polygons.

		The function determines the content flags and opacity of the mesh by examining all polygons in the mesh. If there are no polygons, the mesh is marked as solid and opaque. Otherwise, it
	   iterates through each polygon to collect content flags and check for opacity. If any polygon has different content flags, the mesh will have mixed content. The mesh is marked as opaque only if
	   all polygons are opaque.

	*/
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
	\brief Manages map entity data including primitives, key-value pairs, and geometric information for the engine.

	This class represents a map entity used within the engine's map processing and rendering systems. It maintains a collection of primitives such as brushes, patches, and meshes that define the
   entity's geometry, along with key-value pairs that store entity properties. The class supports parsing map data from lexer streams, writing entity data to files in both traditional and JSON
   formats, and managing primitive collections through addition and removal operations. It also provides functionality for calculating geometric data like CRC checksums and average brush origins. The
   class is designed to work with the engine's map loading system, particularly during level initialization and collision model generation, and supports conversion from glTF data structures for modern
   asset import workflows.

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
	/*!
		\brief Initializes a new idMapEntity instance with a hash size of 64 for its epairs.

		This constructor initializes a new instance of idMapEntity. It sets the hash size of the epairs member to 64, which is used to store entity key-value pairs. The constructor is typically used
	   when creating map entities for the game engine, particularly in scenarios involving map file processing and entity management.

	*/
	idMapEntity() { epairs.SetHashSize( 64 ); }
	~idMapEntity() { primitives.DeleteContents( true ); }

	/*!
		\brief Populates a list of entities from a glTF scene data structure

		This function processes a glTF data structure to extract entities and meshes, adding them to the provided entity list. It handles special cases for worldspawn entities and processes child
	   nodes recursively. The function manages the conversion of glTF nodes into Doom 3 BFG map entities, including handling of mesh data and entity properties. It supports both direct entities and
	   inline meshes that are attached to entities. The function returns the total count of entities found in the scene.

		\param data Pointer to the glTF data structure containing the scene information
		\param entities Reference to the list where entities will be added
		\param scene Index of the scene within the glTF data to process
		\return The total number of entities found and added to the entities list
	*/
	static int			GetEntities( gltfData* data, EntityListRef entities, int scene = 0 );

	/*!
		\brief Parses a map entity from a lexer input stream, handling brushes, patches, and key-value pairs.

		This function reads map entity data from a lexer, which is typically used in parsing map files. It processes opening and closing braces to identify the start and end of the entity definition.
	   Inside the entity definition, it parses brushes, patches, and mesh primitives based on their type. It also handles key-value pairs, particularly parsing the 'origin' and 'classname' keys. If
	   the worldSpawn flag is set, it initializes the primitive list with a specific size. The function returns a pointer to the parsed map entity or null if parsing fails.

		\param src The lexer input stream from which the map entity data is read.
		\param worldSpawn Flag indicating whether this entity is a worldspawn entity, affecting primitive list size initialization.
		\param version The version number of the map format being parsed, influencing how data is interpreted.
		\return A pointer to the parsed idMapEntity object, or NULL if parsing fails.
	*/
	static idMapEntity* Parse( idLexer& src, bool worldSpawn = false, int version = CURRENT_MAP_VERSION );

	/*!
		\brief Writes map entity data to a file, including entity key-value pairs and primitives.

		This function writes the map entity data to the provided file handle. It first writes the entity header with the entity number, then writes all key-value pairs associated with the entity. It
	   handles the origin key properly, potentially modifying it based on the origin offset. Finally, it writes all the primitives (brushes, patches, and meshes) that belong to the entity, using
	   different write methods depending on the primitive type and the valve220 flag.

		\param fp File pointer to write the map entity data to
		\param entityNum The entity number to be written in the map file
		\param valve220 Flag indicating whether to use Valve 220 format for brush writing
		\return True if the operation was successful, false otherwise
	*/
	bool				Write( idFile* fp, int entityNum, bool valve220 ) const;

	/*!
		\brief Parses a JSON object from the lexer and constructs an idMapEntity object with its data.

		This function reads a JSON structure from the provided lexer to create an idMapEntity instance. It handles parsing of primitives such as mesh data and key-value pairs. The function processes
	   the JSON tokens to build the entity's properties and primitives, including handling of the 'origin' key for positioning and 'classname' key for identifying worldspawn entities. The parsing
	   continues until the closing brace of the JSON object is encountered.

		\param src The lexer object used to read and parse the JSON input stream
		\return A pointer to the newly created idMapEntity object, or NULL if parsing fails
	*/
	static idMapEntity* ParseJSON( idLexer& src );

	/*!
		\brief Writes entity data in JSON format to a file, including key-value pairs and primitive geometry.

		This function serializes an entity's data into a JSON structure and writes it to the provided file handle. It outputs the entity number, all key-value pairs stored in the entity's epairs, and
	   any associated primitive geometry such as mesh primitives. The output is formatted with proper indentation and commas to create a valid JSON structure. The function handles both regular
	   key-value pairs and special 'origin' vector.

		\param fp File handle to write the JSON data to
		\param entityNum The numeric identifier of the entity being written
		\param numEntities Total number of entities in the collection being processed
		\return Always returns true indicating successful completion of the write operation
	*/
	bool				WriteJSON( idFile* fp, int entityNum, int numEntities ) const;

	//! Returns the number of primitives contained in the map entity.
	int					GetNumPrimitives() const { return primitives.Num(); }

	/*!
		\brief Returns a pointer to the primitive at the specified index in the map entity

		This function provides access to a specific primitive stored within the map entity by its index. It is used primarily during collision model loading and processing to iterate through the
	   primitives associated with a map entity. The function performs a simple array lookup without any bounds checking, so the caller must ensure the index is valid.

		\param i The index of the primitive to retrieve
		\return A pointer to the idMapPrimitive at the specified index, or nullptr if the index is out of bounds
	*/
	idMapPrimitive*		GetPrimitive( int i ) const { return primitives[i]; }

	/*!
		\brief Adds a map primitive to the entity's list of primitives.

		This function appends the provided map primitive to the internal list of primitives associated with the map entity. It is used during the parsing or construction of map data to accumulate
	   primitives that define the geometry or properties of the entity.

		\param p Pointer to the map primitive to be added
	*/
	void				AddPrimitive( idMapPrimitive* p ) { primitives.Append( p ); }

	//! Calculates and returns the CRC checksum of the geometric data for all primitives in the map entity.
	unsigned int		GetGeometryCRC() const;

	/*!
		\brief Removes all primitive data associated with the map entity.

		This function clears the primitives collection by deleting all contained elements. It is used to free memory and clean up primitive data that was previously allocated for the map entity.

	*/
	void				RemovePrimitiveData();

protected:
	/*!
		\brief Calculates the average center position of all origin brushes in the map entity

		This function iterates through all primitives in the map entity and identifies those that are origin brushes. It then calculates the average center position of all identified origin brushes
	   and stores the result in the originOffset member variable. If no origin brushes are found, the function returns early without making any changes.

	*/
	void										CalculateBrushOrigin();

	idList<idMapPrimitive*, TAG_IDLIB_LIST_MAP> primitives;
};

/*!
	\class idMapFile
	\brief A class for loading, parsing, and manipulating map files in various formats including .map, .json, .gltf, and .glb.

	The idMapFile class provides comprehensive functionality for handling map data within the engine, supporting multiple file formats and conversion operations. It manages a collection of map
   entities and provides methods for parsing, writing, and modifying map content. The class supports loading maps from different sources including JSON, GLTF/GLB, and traditional .map formats, with
   automatic format detection and conversion capabilities. Key features include entity management through addition, removal, and search operations, as well as specialized conversion functions for
   different map formats such as Valve 220 and TrenchBroom compatibility. The class also handles material conversions from WAD textures to Doom 3 materials, name uniquification, and geometry CRC
   calculation for map integrity checking. Memory management is handled through standard C++ object ownership patterns, with explicit deletion of entities when removed from the collection. The design
   supports both read and write operations with flexible path handling and file system integration.

*/
class idMapFile
{
public:
	/*!
		\brief Initializes a new instance of the idMapFile class with default values.

		This constructor initializes the idMapFile object with default settings. It sets the version to the current map version, initializes file time and geometry CRC to zero, resizes the entities
	   collection to accommodate 1024 entities with a granularity of 256, and marks that primitive data is not present. It also initializes format flags to false, indicating the map is not in Valve
	   220 or GLTF format.

	*/
	idMapFile();
	~idMapFile() { entities.DeleteContents( true ); }

	/*!
		\brief Parses a map file from the specified filename, supporting multiple formats including .map, .json, .gltf, and .glb

		This function loads and parses map data from a file, supporting multiple file formats. It first attempts to load a JSON file with the same base name, then tries GLTF/GLB formats, and finally
	   falls back to the standard .map format. The function handles different map versions and can process entities with various properties including materials, names, and positioning. It supports
	   optional parameters to control behavior such as ignoring regions, using OS paths, and ignoring additional entities

		\param filename The name of the file to parse
		\param ignoreRegion If true, skip loading region files
		\param osPath If true, treat paths using OS-specific conventions
		\param ignoreExtraEnts If true, skip loading extra entity files
		\return True if the file was successfully parsed, false otherwise
	*/
	bool		 Parse( const char* filename, bool ignoreRegion = false, bool osPath = false, bool ignoreExtraEnts = false );

	/*!
		\brief Writes the map data to a file with the specified filename and extension

		This function writes the map data to a file using the provided filename and extension. It handles opening the file for writing, either from the base path or explicitly, and writes the map
	   version header followed by each entity in the map. The function returns true on successful write, or false if the file cannot be opened

		\param fileName The base name of the file to write
		\param ext The file extension to use
		\param fromBasePath Whether to write to the base path or use explicit file path
		\return true if the file was successfully written, false otherwise
	*/
	bool		 Write( const char* fileName, const char* ext, bool fromBasePath = true );

	/*!
		\brief Writes map data to a JSON file with the specified filename and extension.

		This function serializes the map entities into a JSON format and writes them to a file. It constructs the full file path using the provided filename and extension, then opens the file for
	   writing. The function outputs a JSON structure containing the map version and a list of entities. The file can be written to either the base path or explicitly based on the fromBasePath
	   parameter. The function returns true on successful write, false otherwise.

		\param fileName The base name of the file to write
		\param ext The file extension to append to the filename
		\param fromBasePath Whether to write to the base path or use explicit file writing
		\return True if the file was successfully written, false otherwise.
	*/
	bool		 WriteJSON( const char* fileName, const char* ext, bool fromBasePath = true );

	/*!
		\brief Writes a diff file comparing this map to another map, capturing changes between the two.

		This function generates a diff file that highlights the differences between the current map and another map. It writes entity data to the output file, including both new entities and modified
	   entities. For entities that exist in both maps, it compares key-value pairs and only writes the differences. The function handles file operations using the file system interface, and supports
	   writing to either a base path or an explicit path based on the fromBasePath parameter.

		\param other The map file to compare against
		\param fileName The base name of the file to write the diff to
		\param ext The file extension for the output file
		\param fromBasePath Whether to write the file to the base path or an explicit path
		\return true if the diff file was successfully written, false otherwise
	*/
	bool		 WriteDiff( const idMapFile* other, const char* fileName, const char* ext, bool fromBasePath = true );

	/*!
		\brief Converts brush and patch primitives in the map entities to polygon mesh format.

		This function iterates through all entities in the map file and converts brush and patch primitives to polygon mesh format. For each entity, it checks if the entity has a valid primitive at
	   each index, and if so, it determines the type of primitive. If the primitive is a brush, it creates a new MapPolygonMesh object and calls ConvertFromBrush to convert the brush data. If the
	   primitive is a patch, it creates a new MapPolygonMesh object and calls ConvertFromPatch to convert the patch data. The original primitive is then deleted and replaced with the new polygon mesh
	   primitive.

		\return true if the conversion process completes successfully
	*/
	bool		 ConvertToPolygonMeshFormat();

	/*!
		\brief Converts the map file to Valve 220 format and optionally recalculates plane points.

		This function updates the map file to be compatible with Valve 220 format. It sets a flag indicating the format conversion and processes entities within the map. For brush models, it handles
	   light entities by splitting them into separate func_static and light entities, adjusting their properties and ensuring proper linking. It also converts certain model file extensions to .obj
	   proxies for TrenchBroom compatibility and manages entity transformations and group IDs. The recalcPlanePoints parameter determines whether to recalculate plane points during the conversion.

		\param recalcPlanePoints Flag indicating whether to recalculate plane points during conversion
		\return True if the conversion to Valve 220 format was successful, false otherwise
	*/
	bool		 ConvertToValve220Format( bool recalcPlanePoints );

	/*!
		\brief Classifies map entities into brush-based or point-based classes for TrenchBroom compatibility

		This function analyzes all entities in the map file and determines whether they should be classified as brush-based classes or point-based classes for compatibility with TrenchBroom. It
	   examines each entity's properties, particularly the model and classname attributes, to make this determination. The classification is stored in the provided idDict which serves as an overview
	   of class types.

		\param classTypeOverview Dictionary that will store the classification results for each classname
	*/
	void		 ClassifyEntitiesForTrenchBroom( idDict& classTypeOverview );

	/*!
		\brief Converts Quake map entities to Doom 3 format by renaming entities, fixing light colors, and mapping WAD textures to Doom 3 materials.

		This function processes all entities in the map file to convert them from Quake format to Doom 3 format. It ensures each entity has a unique name, updates the classname for certain entity
	   types like func_wall and func_detail, corrects light color values, and maps WAD texture names to appropriate Doom 3 material names. It also sets up texture collections for the worldspawn
	   entity.

		\return Always returns true, indicating successful conversion of the map file.
	*/
	bool		 ConvertQuakeToDoom();

	/*!
		\brief Fixes duplicated names in group instances by generating unique names for entities that share the same name

		This function iterates through all entities in the map file and checks for name collisions among entities that belong to linked group lists. For entities without a name, it assigns a unique
	   name based on the classname. For entities with existing names that collide with other entities, it generates a new unique name to resolve the conflict. Additionally, it normalizes light color
	   values from 0-255 range to 0-1 range for lighting entities.

	*/
	void		 FixDuplicatedNamesInGroupInstances();

	//! Returns the number of entities in the map file.
	int			 GetNumEntities() const { return entities.Num(); }

	/*!
		\brief Returns the entity at the specified index from the map file

		This function retrieves a map entity from the internal entities collection using the provided index. It is commonly used in map file processing operations where individual entities need to be
	   accessed for further processing, such as collision model generation or AAS building. The function performs a direct index-based lookup without any bounds checking, so the caller must ensure the
	   index is valid.

		\param i The index of the entity to retrieve
		\return A pointer to the idMapEntity at the specified index
	*/
	idMapEntity* GetEntity( int i ) const { return entities[i]; }

	//! Returns the name of the map file without the file extension.
	const char*	 GetName() const { return name; }

	//! Returns the file time associated with the map file.
	ID_TIME_T	 GetFileTime() const { return fileTime; }

	//! Returns the CRC value for the map geometry.
	unsigned int GetGeometryCRC() const { return geometryCRC; }

	/*!
		\brief Checks if the map file needs to be reloaded by comparing its modification time with the stored file time.

		This function determines whether a map file has been modified on disk since it was last loaded or cached. It compares the file's last modification timestamp with the stored fileTime value. If
	   the file name is empty, it returns true to indicate that a reload is needed. The function returns true when the disk file has been modified more recently than the cached version, indicating
	   that the map data should be reloaded to reflect the latest changes.

		\return true if the file on disk has been modified since it was last loaded, false otherwise
	*/
	bool		 NeedsReload();

	/*!
		\brief Adds a map entity to the collection and returns its index.

		This function appends the provided map entity to the internal collection of entities stored in the map file. It returns the index at which the entity was added. The function is typically used
	   when loading or constructing map data from external sources.

		\param mapentity Pointer to the map entity to be added to the collection
		\return The index of the newly added entity within the collection
	*/
	int			 AddEntity( idMapEntity* mapentity );

	/*!
		\brief Finds and returns a map entity with the specified name

		This function searches through all entities in the map file and returns the first entity whose name matches the provided name. The search is case-insensitive. If no matching entity is found,
	   the function returns NULL

		\param name name of the entity to search for
		\return pointer to the found entity or NULL if no entity with the specified name exists
	*/
	idMapEntity* FindEntity( const char* name ) const;

	/*!
		\brief Finds and returns the map entity located at or near the specified origin position.

		This function searches through all entities in the map file to find one whose origin is within a small bounding box centered on the provided origin. The search uses a tolerance of 0.125 units
	   to determine if an entity is considered to be at the specified position. The function returns the first entity that satisfies this condition, or NULL if no such entity is found.

		\param org The 3D position to search for a map entity at.
		\return A pointer to the map entity found at or near the specified origin, or NULL if no entity is found.
	*/
	idMapEntity* FindEntityAtOrigin( const idVec3& org ) const;

	/*!
		\brief Removes a map entity from the map file and deletes its memory.

		This function removes the specified map entity from the internal list of entities within the map file and then deletes the entity object to free up memory. It is typically used when an entity
	   needs to be deleted or destroyed during map processing or cleanup.

		\param mapEnt Pointer to the map entity to be removed and deleted
	*/
	void		 RemoveEntity( idMapEntity* mapEnt );

	/*!
		\brief Removes all map entities with the specified class name

		This function iterates through all entities in the map file and deletes those whose classname matches the provided string. The comparison is case-insensitive. When an entity is found and
	   removed, the index is adjusted to account for the removal, ensuring all entities are properly checked.

		\param classname The class name of entities to remove
	*/
	void		 RemoveEntities( const char* classname );

	/*!
		\brief Removes all entities from the map file and clears the primitive data flag.

		This function deletes all entities stored in the map file and sets the hasPrimitiveData flag to false, indicating that no primitive data exists in the map.

	*/
	void		 RemoveAllEntities();

	/*!
		\brief Removes primitive data from all entities in the map file and clears the primitive data flag.

		This function iterates through all entities in the map file and calls RemovePrimitiveData on each one. It then sets the hasPrimitiveData flag to false, indicating that no primitive data exists
	   in the map.

	*/
	void		 RemovePrimitiveData();

	/*!
		\brief Checks if the map file contains primitive data.

		This function returns a boolean value indicating whether the map file has been loaded with primitive data. The primitive data is typically used for rendering and game logic purposes. The
	   function simply returns the internal flag that tracks this state.

		\return true if the map file contains primitive data, false otherwise
	*/
	bool		 HasPrimitiveData() { return hasPrimitiveData; }

	//! Returns true if the map file is in GLTF format.
	bool		 IsGLTF() const { return gltfFormat; }

	/*!
		\brief Adds a material to a collection of texture collections, ensuring uniqueness.

		This function processes a material name by stripping its path component and then constructs a texture collection path by removing the filename from the material path. The resulting texture
	   collection path is added to the provided list of texture collections only if it is not already present.

		\param material The full path to the material file.
		\param textureCollections The list of texture collections to which the material's collection path will be added.
	*/
	static void	 AddMaterialToCollection( const char* material, idStrList& textureCollections );

	/*!
		\brief Converts a WAD texture name to a material name using predefined conversion mappings

		This function performs a case-insensitive comparison of the input WAD texture name against a list of known texture conversion mappings. If a match is found, the corresponding Doom material
	   name is assigned to the output parameter. If no match is found, the original WAD texture name is used as the material name. The conversion mappings are stored in the textureConvertNames array
	   which contains quakeName and doomName pairs.

		\param material The WAD texture name to convert
		\param matName The resulting material name after conversion
	*/
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
	/*!
		\brief Computes and sets the geometry CRC for the map file by combining CRC values from all entities.

		This function calculates a combined CRC value for the map's geometry by iterating through all entities in the map and XORing their individual geometry CRC values. The result is stored in the
	   geometryCRC member variable of the map file.

	*/
	void		SetGeometryCRC();

	/*!
		\brief Generates a unique entity name by appending an incrementing ID to the given class name.

		This function attempts to find a unique name for an entity by combining the provided class name with an incremental ID. It searches for available names up to a maximum ID of 99999. If no
	   available name is found within this range, it returns the name with the maximum ID. The generated name is stored in a static buffer and will be overwritten on subsequent calls.

		\param classname The base class name to use for generating the unique entity name
		\return A pointer to a static character buffer containing the unique entity name
	*/
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
