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

#ifndef __DECLMANAGER_H__
#define __DECLMANAGER_H__

/*
===============================================================================

	Declaration Manager

	All "small text" data types, like materials, sound shaders, fx files,
	entity defs, etc. are managed uniformly, allowing reloading, purging,
	listing, printing, etc. All "large text" data types that never have more
	than one declaration in a given file, like maps, models, AAS files, etc.
	are not handled here.

	A decl will never, ever go away once it is created. The manager is
	garranteed to always return the same decl pointer for a decl type/name
	combination. The index of a decl in the per type list also stays the
	same throughout the lifetime of the engine. Although the pointer to
	a decl always stays the same, one should never maintain pointers to
	data inside decls. The data stored in a decl is not garranteed to stay
	the same for more than one engine frame.

	The decl indexes of explicitely defined decls are garrenteed to be
	consistent based on the parsed decl files. However, the indexes of
	implicit decls may be different based on the order in which levels
	are loaded.

	The decl namespaces are separate for each type. Comments for decls go
	above the text definition to keep them associated with the proper decl.

	During decl parsing, errors should never be issued, only warnings
	followed by a call to MakeDefault().

===============================================================================
*/

typedef enum {
	DECL_TABLE = 0,
	DECL_MATERIAL,
	DECL_SKIN,
	DECL_SOUND,
	DECL_ENTITYDEF,
	DECL_MODELDEF,
	DECL_FX,
	DECL_PARTICLE,
	DECL_AF,
	DECL_PDA,
	DECL_VIDEO,
	DECL_AUDIO,
	DECL_EMAIL,
	DECL_MODELEXPORT,
	DECL_MAPDEF,

	// new decl types can be added here

	DECL_MAX_TYPES = 32
} declType_t;

typedef enum {
	DS_UNPARSED,
	DS_DEFAULTED, // set if a parse failed due to an error, or the lack of any source
	DS_PARSED
} declState_t;

const int DECL_LEXER_FLAGS = LEXFL_NOSTRINGCONCAT |				// multiple strings separated by whitespaces are not concatenated
							 LEXFL_NOSTRINGESCAPECHARS |		// no escape characters inside strings
							 LEXFL_ALLOWPATHNAMES |				// allow path seperators in names
							 LEXFL_ALLOWMULTICHARLITERALS |		// allow multi character literals
							 LEXFL_ALLOWBACKSLASHSTRINGCONCAT | // allow multiple strings separated by '\' to be concatenated
							 LEXFL_NOFATALERRORS;				// just set a flag instead of fatal erroring

/*!
	\class idDeclBase
	\brief Base class for all declaration types in the engine.

	This abstract base class defines the interface for all declaration types used within the engine, providing a standardized way to manage and interact with various declaration resources. The class
   encapsulates the core functionality needed to handle declarations such as materials, sounds, and other engine assets. It offers methods for retrieving declaration properties, managing their
   lifecycle, and controlling their loading and parsing behavior. The interface supports operations like validation, text management, source file handling, and resource state tracking. All concrete
   declaration implementations must provide concrete realizations of the pure virtual methods defined in this base class.

*/
class idDeclBase
{
public:
	virtual ~idDeclBase() {};
	virtual const char* GetName() const															 = 0;
	virtual declType_t	GetType() const															 = 0;
	virtual declState_t GetState() const														 = 0;
	virtual bool		IsImplicit() const														 = 0;
	virtual bool		IsValid() const															 = 0;
	virtual void		Invalidate()															 = 0;
	virtual void		Reload()																 = 0;
	virtual void		EnsureNotPurged()														 = 0;
	virtual int			Index() const															 = 0;
	virtual int			GetLineNum() const														 = 0;
	virtual const char* GetFileName() const														 = 0;
	virtual void		GetText( char* text ) const												 = 0;
	virtual int			GetTextLength() const													 = 0;
	virtual void		SetText( const char* text )												 = 0;
	virtual bool		ReplaceSourceFileText()													 = 0;
	virtual bool		SourceFileChanged() const												 = 0;
	virtual void		MakeDefault()															 = 0;
	virtual bool		EverReferenced() const													 = 0;
	virtual bool		SetDefaultText()														 = 0;
	virtual const char* DefaultDefinition() const												 = 0;
	virtual bool		Parse( const char* text, const int textLength, bool allowBinaryVersion ) = 0;
	virtual void		FreeData()																 = 0;
	virtual size_t		Size() const															 = 0;
	virtual void		List() const															 = 0;
	virtual void		Print() const															 = 0;
	virtual ID_TIME_T	GetSourceFileTimestamp() const											 = 0; // RB
};

/*!
	\class idDecl
	\brief Base class for all declaration types in the engine.

	The idDecl class serves as the foundation for all declaration types within the engine, providing common functionality for managing declaration state, text content, and file metadata. It maintains
   various states such as parsed, valid, and purged, and offers methods to control these states and query declaration properties. The class supports parsing from text, freeing of internal data, and
   retrieval of declaration information like name, type, file location, and source timestamp. Subclasses are expected to implement specific parsing and data management behavior through virtual
   methods. The class also handles implicit declarations and provides mechanisms to ensure declarations remain valid and accessible.

*/
class idDecl
{
public:
	//! Initializes the declaration with a null base pointer.
	idDecl() { base = NULL; }
	virtual ~idDecl() {};

	//! Returns the name of the declaration.
	const char* GetName() const { return base->GetName(); }

	//! Returns the type of the declaration.
	declType_t	GetType() const { return base->GetType(); }

	//! Returns the state of the declaration.
	declState_t GetState() const { return base->GetState(); }

	//! Returns true if the declaration was defaulted or created with a call to SetDefaultText
	bool		IsImplicit() const { return base->IsImplicit(); }

	//! Checks if the declaration is valid and has been properly initialized.
	bool		IsValid() const { return base->IsValid(); }

	//! Invalidates the declaration state, resetting it to unparsed.
	void		Invalidate() { base->Invalidate(); }

	//! Ensures the declaration is not purged and remains valid for use.
	void		EnsureNotPurged() { base->EnsureNotPurged(); }

	//! Returns the index in the per-type list.
	int			Index() const { return base->Index(); }

	//! Returns the line number where the declaration starts.
	int			GetLineNum() const { return base->GetLineNum(); }

	//! Returns the name of the file in which the declaration is defined.
	const char* GetFileName() const { return base->GetFileName(); }

	//! Returns the declaration text into the provided character buffer.
	void		GetText( char* text ) const { base->GetText( text ); }

	//! Returns the length of the declaration text.
	int			GetTextLength() const { return base->GetTextLength(); }

	//! Sets the text content of the declaration.
	void		SetText( const char* text ) { base->SetText( text ); }

	//! Replaces the text of this declaration in its source file.
	bool		ReplaceSourceFileText() { return base->ReplaceSourceFileText(); }

	//! Returns true if the source file has changed since it was loaded and parsed.
	bool		SourceFileChanged() const { return base->SourceFileChanged(); }

	//! Initializes the declaration data to its default state.
	void		MakeDefault() { base->MakeDefault(); }

	//! Returns true if the declaration was ever referenced.
	bool		EverReferenced() const { return base->EverReferenced(); }

	//! Returns the timestamp of the source file associated with this declaration.
	ID_TIME_T	GetSourceFileTimestamp() const { return base->GetSourceFileTimestamp(); }

public:
	/*!
		\brief Sets the text source to a default text if necessary

		This may be overridden to provide a default definition based on the
		decl name. For instance materials may default to an implicit definition
		using a texture with the same name as the decl.
	*/
	virtual bool		SetDefaultText() { return base->SetDefaultText(); }

	/*!
		\brief Returns the default definition string that can be parsed to recreate a declaration's default state.

		Each declaration type must have a default string that it is guaranteed
		to parse acceptably. When a decl is not explicitly found, is purged, or
		has an error while parsing, MakeDefault() will do a FreeData(), then a
		Parse() with DefaultDefinition(). The defaultDefintion should start with
		an open brace and end with a close brace.
	*/
	virtual const char* DefaultDefinition() const { return base->DefaultDefinition(); }

	/*!
		\brief Parses the given text data for the declaration, returning true if successful.

		The manager will have already parsed past the type, name and opening brace.
		All necessary media will be touched before return.
		The manager will have called FreeData() before issuing a Parse().
		The subclass can call MakeDefault() internally at any point if there are parse errors.
	*/
	virtual bool		Parse( const char* text, const int textLength, bool allowBinaryVersion = false ) { return base->Parse( text, textLength, allowBinaryVersion ); }

	/*!
		\brief Frees any pointers held by the subclass and prepares the declaration for re-parsing.

		Frees any pointers held by the subclass. This may be called before
		any Parse(), so the constructor must have set sane values. The decl will be
		invalid after issuing this call, but it will always be immediately followed by a Parse()
	*/
	virtual void		FreeData() { base->FreeData(); }

	//! Returns the size of the declaration in memory.
	virtual size_t		Size() const { return base->Size(); }

	//! Outputs the declaration name to the console.
	virtual void		List() const { base->List(); }

	//! Prints the declaration data to the console.
	virtual void		Print() const { base->Print(); }

public:
	idDeclBase* base;
};

//! Creates and returns a new instance of the declared type.
template<class type>
ID_INLINE idDecl* idDeclAllocator()
{
	return new( TAG_DECL ) type;
}

class idMaterial;
class idDeclSkin;
class idSoundShader;

/*!
	\class idDeclManager
	\brief Manages declaration types, loading, and retrieval of game assets.

	The idDeclManager serves as a central registry and manager for various declaration types used in the engine. It handles initialization, shutdown, reloading, and organization of different asset
   types such as materials, skins, and sound shaders. The manager supports registration of new declaration types and folders for asset discovery. It provides methods to find, create, and enumerate
   declarations by type, as well as utility functions for listing, printing, and precaching assets. The interface is designed to be extended by implementations that handle specific asset loading and
   management strategies.

*/
class idDeclManager
{
public:
	virtual ~idDeclManager() { }

	//! Initializes the declaration manager for the engine
	virtual void				 Init() = 0;

	//! Initializes additional declaration folders for skins and sound shaders.
	virtual void				 Init2() = 0;

	//! Initializes the declaration manager tool environment.
	virtual void				 InitTool() = 0;

	//! Shuts down the declaration manager and frees all allocated memory for declarations and related resources.
	virtual void				 Shutdown() = 0;

	//! Reloads all loaded declaration files.
	virtual void				 Reload( bool force ) = 0;

	//! Initializes the declaration manager for a new level load operation.
	virtual void				 BeginLevelLoad() = 0;

	//! Marks the end of a level load operation and prepares managers to free unreferenced media.
	virtual void				 EndLevelLoad() = 0;

	//! Registers a new declaration type with the manager
	virtual void				 RegisterDeclType( const char* typeName, declType_t type, idDecl* ( *allocator )() ) = 0;

	//! Registers a folder containing declaration files with a specific extension and default type.
	virtual void				 RegisterDeclFolder( const char* folder, const char* extension, declType_t defaultType ) = 0;

	//! Returns a checksum value calculated from the loaded declarations.
	virtual int					 GetChecksum() const = 0;

	//! Returns the number of declaration types registered in the manager.
	virtual int					 GetNumDeclTypes() const = 0;

	//! Returns the number of declarations of the specified type managed by the declaration manager.
	virtual const char*			 GetDeclNameFromType( declType_t type ) const = 0;

	//! Returns the name of a declaration type given its type identifier
	virtual declType_t			 GetDeclTypeFromName( const char* typeName ) const = 0;

	/*!
		\brief Finds and returns a declaration of the specified type by name, parsing it if necessary.

		If makeDefault is true, a default decl of appropriate type will be created
		if an explicit one isn't found. If makeDefault is false, NULL will be returned
		if the decl wasn't explcitly defined.
	*/
	virtual const idDecl*		 FindType( declType_t type, const char* name, bool makeDefault = true ) = 0;

	//! Finds a declaration by type and name without parsing its content, optionally creating a default declaration if not found.
	virtual const idDecl*		 FindDeclWithoutParsing( declType_t type, const char* name, bool makeDefault = true ) = 0;

	/*!
		\brief Returns a declaration by its type and index, optionally parsing it if needed.

		The complete lists of decls can be walked to populate editor browsers.
		If forceParse is set false, you can get the decl to check name / filename / etc.
		without causing it to parse the source and load media.
	*/
	virtual const idDecl*		 DeclByIndex( declType_t type, int index, bool forceParse = true ) = 0;

	virtual void				 ReloadFile( const char* filename, bool force ) = 0;

	// Returns the number of decls of the given type.
	virtual int					 GetNumDecls( declType_t type ) = 0;

	//! Lists declarations of a specified type based on command arguments
	virtual void				 ListType( const idCmdArgs& args, declType_t type ) = 0;

	//! Prints detailed information about a specific declaration of a given type
	virtual void				 PrintType( const idCmdArgs& args, declType_t type ) = 0;

	//! Creates a new declaration of the specified type with the given name and file name.
	virtual idDecl*				 CreateNewDecl( declType_t type, const char* name, const char* fileName ) = 0;

	// BSM - Added for the material editors rename capabilities
	virtual bool				 RenameDecl( declType_t type, const char* oldName, const char* newName ) = 0;

	//! Prints formatted media caching information with proper indentation.
	virtual void				 MediaPrint( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;

	//! Writes precache commands for referenced declarations to the provided file.
	virtual void				 WritePrecacheCommands( idFile* f ) = 0;

	// Convenience functions for specific types ---------------------

	//! Finds and returns a material declaration by name, optionally creating a default material if it does not exist.
	virtual const idMaterial*	 FindMaterial( const char* name, bool makeDefault = true ) = 0;

	//! Retrieves a material declaration by its index in the declaration manager.
	virtual const idMaterial*	 MaterialByIndex( int index, bool forceParse = true ) = 0;

	//! Returns a skin declaration by name, optionally creating a default skin if it doesn't exist.
	virtual const idDeclSkin*	 FindSkin( const char* name, bool makeDefault = true ) { return NULL; }

	//! Finds and returns a sound shader declaration by name, creating a default if specified.
	virtual const idSoundShader* FindSound( const char* name, bool makeDefault = true ) { return NULL; };

	//! Returns a skin declaration by its index in the manager
	virtual const idDeclSkin*	 SkinByIndex( int index, bool forceParse = true ) { return NULL; };

	//! Returns a sound shader declaration by its index
	virtual const idSoundShader* SoundByIndex( int index, bool forceParse = true ) { return NULL; };

	//! Touches a declaration to ensure it is parsed if it hasn't been already.
	virtual void				 Touch( const idDecl* decl ) = 0;
};

extern idDeclManager* declManager;

//! Lists declarations of a specific type based on command line arguments.
template<declType_t type>
ID_INLINE void idListDecls_f( const idCmdArgs& args )
{
	declManager->ListType( args, type );
}

//! Prints declarations of a specified type based on command line arguments
template<declType_t type>
ID_INLINE void idPrintDecls_f( const idCmdArgs& args )
{
	declManager->PrintType( args, type );
}

#endif /* !__DECLMANAGER_H__ */
