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

#ifndef __SAVEGAME_H__
#define __SAVEGAME_H__

/*!
	\class idSaveGame
	\brief A class for managing the serialization and saving of game state data.

	This class provides functionality for writing various data types and game objects to a save file, supporting both binary and text-based storage formats. It handles the serialization of game
   objects, their properties, and related data such as materials, skins, particles, and sound shaders. The class manages internal object lists and ensures proper ordering and indexing when writing
   objects to the save file. It supports writing primitive types, vectors, matrices, and complex game-specific structures like render entities, sound references, and trace models. The implementation
   includes methods for adding objects to the save list, writing declaration references, and handling string interning for efficient storage.

*/
class idSaveGame
{
public:
	//! Initializes a new instance of the idSaveGame class with the specified save file, string file, and version.
	idSaveGame( idFile* savefile, idFile* stringFile, int inVersion );

	//! Destructor for the idSaveGame class that cleans up resources.
	~idSaveGame();

	//! Finalizes and closes the save game file, writing all saved data and cleaning up resources.
	void Close();

	//! Writes all loaded declarations to the save game file.
	void WriteDecls();

	//! Adds a unique object to the save game list.
	void AddObject( const idClass* obj );

	//! Resizes the internal objects array to the specified count.
	void Resize( const int count ) { objects.Resize( count ); }

	//! Writes the list of objects to the save game file.
	void WriteObjectList();

	//! Writes data to the save game file
	void Write( const void* buffer, int len );

	//! Writes a 32-bit integer value to the save game file in big-endian format.
	void WriteInt( const int value );

	//! Writes a joint handle value to the save game file.
	void WriteJoint( const jointHandle_t value );

	//! Writes a short integer value to the save game file in big-endian format.
	void WriteShort( const short value );

	//! Writes a single byte value to the save game file.
	void WriteByte( const byte value );

	//! Writes a signed char value to the save game file.
	void WriteSignedChar( const signed char value );

	//! Writes a floating-point value to the save game file.
	void WriteFloat( const float value );

	//! Writes a boolean value to the save file.
	void WriteBool( const bool value );

	//! Writes a string to the save game file, using string interning to optimize storage
	void WriteString( const char* string );

	//! Writes a 2D vector to the save game file.
	void WriteVec2( const idVec2& vec );

	//! Writes a 3D vector to the save game file
	void WriteVec3( const idVec3& vec );

	//! Writes a 4-dimensional vector to the save game file.
	void WriteVec4( const idVec4& vec );

	//! Writes a 6-dimensional vector to the save game file.
	void WriteVec6( const idVec6& vec );

	//! Writes a winding object to the save game file
	void WriteWinding( const idWinding& winding );

	//! Writes the specified bounds to the save file.
	void WriteBounds( const idBounds& bounds );

	//! Writes a 3x3 matrix to the save file.
	void WriteMat3( const idMat3& mat );

	//! Writes the specified angles to the save game file.
	void WriteAngles( const idAngles& angles );

	//! Writes an object index to the save game stream.
	void WriteObject( const idClass* obj );

	//! Writes a static object to the save game file.
	void WriteStaticObject( const idClass& obj );

	//! Writes a dictionary to the save game file.
	void WriteDict( const idDict* dict );

	//! Writes the name of a material to the save game file, or an empty string if the material is null.
	void WriteMaterial( const idMaterial* material );

	//! Writes the name of a skin declaration to the save game file.
	void WriteSkin( const idDeclSkin* skin );

	//! Writes the name of a particle declaration to the save game file, or an empty string if the particle is null.
	void WriteParticle( const idDeclParticle* particle );

	//! Writes the name of the given FX declaration to the save game.
	void WriteFX( const idDeclFX* fx );

	//! Writes the name of a sound shader to the save game file, or an empty string if the shader is null.
	void WriteSoundShader( const idSoundShader* shader );

	//! Writes the name of a model definition to the save game file, or an empty string if the model definition is null.
	void WriteModelDef( const class idDeclModelDef* modelDef );

	//! Writes the name of a render model to the save game file
	void WriteModel( const idRenderModel* model );

	//! Writes a user interface object to the save game file
	void WriteUserInterface( const idUserInterface* ui, bool unique );

	//! Writes a render entity's data to the save game file.
	void WriteRenderEntity( const renderEntity_t& renderEntity );

	//! Writes a renderLight_t structure to the save game file
	void WriteRenderLight( const renderLight_t& renderLight );

	//! Writes a render environment probe definition to the save game file.
	void WriteRenderEnvprobe( const renderEnvironmentProbe_t& renderEnvprobe );

	//! Writes a reference sound to the save game file.
	void WriteRefSound( const refSound_t& refSound );

	//! Writes a render view configuration to the save game file
	void WriteRenderView( const renderView_t& view );

	//! Writes a user command structure to the save game file.
	void WriteUsercmd( const usercmd_t& usercmd );

	//! Writes contact information to the save game file.
	void WriteContactInfo( const contactInfo_t& contactInfo );

	//! Writes a trace element to the save game file.
	void WriteTrace( const trace_t& trace );

	//! Writes a trace model to the save game file
	void WriteTraceModel( const idTraceModel& trace );

	//! Writes a clip model to the save game, including a boolean indicator of whether the clip model is valid.
	void WriteClipModel( const class idClipModel* clipModel );

	//! Writes sound commands to the save game file.
	void WriteSoundCommands();

	//! Writes the build number to the save game file.
	void WriteBuildNumber( const int value );

	//! Returns the build number stored in the save game.
	int	 GetBuildNumber() const { return version; }

	//! Returns the current size of the save game file in bytes.
	int	 GetCurrentSaveSize() const { return file->Length(); }

private:
	idFile*				   file;
	idFile*				   stringFile;
	idCompressor*		   compressor;

	idList<const idClass*> objects;
	int					   version;

	//! Recursively saves an object and its inheritance hierarchy by calling the appropriate save methods.
	void				   CallSave_r( const idTypeInfo* cls, const idClass* obj );

	struct stringTableIndex_s {
		idStr string;
		int	  offset;
	};

	idHashIndex				   stringHash;
	idList<stringTableIndex_s> stringTable;
	int						   curStringTableOffset;
};

/*!
	\class idRestoreGame
	\brief Manages restoration of game state from save data including objects, declarations, and various data types.

	Provides functionality to restore game state from saved data by reading serialized information from file streams. Handles deserialization of various data types including primitives, vectors,
   matrices, game objects, and engine-specific structures. Supports reading of declarations such as materials, models, sounds, and UI elements. The class coordinates the restoration process through
   methods that create and initialize objects, read declaration data, and manage the overall restore operation. It also provides error handling capabilities that clean up resources before reporting
   errors. The restore process involves reading from a save file and a string table file, with a specified save version to ensure compatibility. Object restoration includes proper handling of
   inheritance hierarchies through recursive restore calls. Memory management is handled through the underlying file system and engine object lifecycle management.

*/
class idRestoreGame
{
public:
	//! Initializes a new restore game context with the specified save file, string table file, and save version.
	idRestoreGame( idFile* savefile, idFile* stringTableFile, int saveVersion );
	~idRestoreGame();

	//! Reads declarations from the restore game data.
	void ReadDecls();

	//! Creates game objects from serialized data during restore operations.
	void CreateObjects();

	//! Restores all game objects from the save file and updates their visual state.
	void RestoreObjects();

	//! Removes and deletes all objects stored in the restore game system.
	void DeleteObjects();

	//! Reports an error with a formatted message and cleans up game objects before calling the main error handler.
	void Error( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Reads data from the underlying file into the provided buffer
	void Read( void* buffer, int len );

	//! Reads an integer value from the restore game file.
	void ReadInt( int& value );

	//! Reads a joint handle value from the restore game file.
	void ReadJoint( jointHandle_t& value );

	//! Reads a 16-bit signed integer value from the restore game file
	void ReadShort( short& value );

	//! Reads a single byte value from the restore game file
	void ReadByte( byte& value );

	//! Reads a signed character value from the restore game file.
	void ReadSignedChar( signed char& value );

	//! Reads a float value from the restore game file.
	void ReadFloat( float& value );

	//! Reads a boolean value from the restore game file
	void ReadBool( bool& value );

	//! Reads a string from the restore game file into the provided string parameter.
	void ReadString( idStr& string );

	//! Reads a 2D vector from the restore game file.
	void ReadVec2( idVec2& vec );

	//! Reads a 3D vector from the restore game file.
	void ReadVec3( idVec3& vec );

	//! Reads a 4-dimensional vector from the restore game file
	void ReadVec4( idVec4& vec );

	//! Reads a 6-dimensional vector from the restore game file.
	void ReadVec6( idVec6& vec );

	//! Reads a winding from the restore game file and populates the provided winding object.
	void ReadWinding( idWinding& winding );

	//! Reads bounding box data from the restore game file into the provided bounds object.
	void ReadBounds( idBounds& bounds );

	//! Reads a 3x3 matrix from the restore game file into the provided matrix object.
	void ReadMat3( idMat3& mat );

	//! Reads angle values from the restore game file into the provided angles object.
	void ReadAngles( idAngles& angles );

	//! Reads an object from the restore game data and assigns it to the provided pointer.
	void ReadObject( idClass*& obj );

	//! Reads a static object from the restore game data
	void ReadStaticObject( idClass& obj );

	//! Reads a dictionary from the restore game stream and populates the provided dictionary object.
	void ReadDict( idDict* dict );

	//! Reads a material from the restore game stream and sets the material pointer to the found material or NULL.
	void ReadMaterial( const idMaterial*& material );

	//! Reads a skin declaration from the restore game data
	void ReadSkin( const idDeclSkin*& skin );

	//! Reads a particle declaration from the restore game data
	void ReadParticle( const idDeclParticle*& particle );

	//! Reads an FX declaration from the restore game data and sets the output pointer to the found declaration or NULL if not found.
	void ReadFX( const idDeclFX*& fx );

	//! Reads a sound shader from the restore game data
	void ReadSoundShader( const idSoundShader*& shader );

	//! Reads a model definition from the restore game data
	void ReadModelDef( const idDeclModelDef*& modelDef );

	//! Reads a render model from the restore game data.
	void ReadModel( idRenderModel*& model );

	//! Reads a user interface from the save game file and initializes it.
	void ReadUserInterface( idUserInterface*& ui );

	//! Reads render entity data from the restore game stream into the provided render entity structure.
	void ReadRenderEntity( renderEntity_t& renderEntity );

	//! Reads render light data from the restore game stream into the provided renderLight_t structure
	void ReadRenderLight( renderLight_t& renderLight );

	//! Reads environment probe data from the restore game stream into the provided render environment probe structure.
	void ReadRenderEnvprobe( renderEnvironmentProbe_t& renderEnvprobe );

	//! Reads a reference sound from the restore game data
	void ReadRefSound( refSound_t& refSound );

	//! Reads render view data from the restore game stream into the provided render view object.
	void ReadRenderView( renderView_t& view );

	//! Reads a user command from the restore game data stream into the provided usercmd_t structure.
	void ReadUsercmd( usercmd_t& usercmd );

	//! Reads contact information from the restore game data into the provided contactInfo_t structure.
	void ReadContactInfo( contactInfo_t& contactInfo );

	//! Reads a trace_t structure from the restore game data
	void ReadTrace( trace_t& trace );

	//! Reads a trace model from the restore game data
	void ReadTraceModel( idTraceModel& trace );

	//! Restores a clip model from the save game data
	void ReadClipModel( idClipModel*& clipModel );

	//! Reads sound commands from the save game file and applies them to the game sound world.
	void ReadSoundCommands();

	//! Returns the build number stored in the restore game object.
	int	 GetBuildNumber() const { return version; }

private:
	idFile*							file;
	idFile*							stringFile;
	idList<idClass*, TAG_SAVEGAMES> objects;
	int								version;
	int								stringTableOffset;

	//! Recursively calls the restore function for a class and its superclasses.
	void							CallRestore_r( const idTypeInfo* cls, idClass* obj );
};

#endif /* !__SAVEGAME_H__*/
