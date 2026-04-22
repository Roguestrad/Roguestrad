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

#ifndef __DECLSKIN_H__
#define __DECLSKIN_H__

/*
===============================================================================

	idDeclSkin

===============================================================================
*/

typedef struct {
	const idMaterial* from; // 0 == any unmatched shader
	const idMaterial* to;
} skinMapping_t;

/*!
	\class idDeclSkin
	\brief Manages skin declarations that map materials to models.

	The idDeclSkin class handles skin declarations which define how materials map to specific models. It provides functionality for parsing skin definition text, managing model associations, and
   remapping shaders based on skin mappings. The class supports generating default definitions when needed and can operate on both text and binary skin formats. It maintains internal data structures
   for tracking material-model mappings and provides methods to query and access this information. The FreeData method allows clearing all associated mapping data, while RemapShaderBySkin enables
   dynamic shader replacement based on skin configurations. The class extends idDecl and integrates with the engine's material and model systems to facilitate skin-based material application.

*/
class idDeclSkin : public idDecl
{
public:
	//! Returns the size in bytes of the idDeclSkin class instance.
	virtual size_t			  Size() const;

	//! Sets the default text for the skin declaration by generating an implicit material definition if a material with the same name exists.
	virtual bool			  SetDefaultText();

	//! Returns the default definition string for a skin declaration
	virtual const char*		  DefaultDefinition() const;

	//! Parses skin declaration text and populates associated models and material mappings.
	virtual bool			  Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Clears all mapping data associated with the skin declaration.
	virtual void			  FreeData();

	//! Returns a remapped material shader based on skin mappings, or the original shader if no mapping is found.
	virtual const idMaterial* RemapShaderBySkin( const idMaterial* shader ) const;

	//! Returns the number of model associations stored in the skin declaration.
	const int				  GetNumModelAssociations() const;

	//! Returns the model name associated with the specified index in the skin declaration.
	const char*				  GetAssociatedModel( int index ) const;

protected:
	idList<skinMapping_t, TAG_IDLIB_LIST_DECL> mappings;
	idStrList								   associatedModels;
};

/*!
	\class idDeclNullSkinBase
	\brief A placeholder declaration class representing a null skin to hide materials in VR like arms.
	\author Leyland

*/
class idDeclNullSkinBase : public idDeclBase
{
public:
	//! Returns the name of the null skin base declaration.
	virtual const char*		  GetName() const { return ""; }

	//! Returns the declaration type for the null skin base.
	virtual declType_t		  GetType() const { return DECL_SKIN; }

	//! Returns the parsed state of the declaration.
	virtual declState_t		  GetState() const { return DS_PARSED; }

	//! Returns false to indicate this declaration is not implicit.
	virtual bool			  IsImplicit() const { return false; }

	//! Returns true indicating that the null skin base declaration is valid.
	virtual bool			  IsValid() const { return true; }

	//! Marks the declaration as invalid and clears its internal state.
	virtual void			  Invalidate() { }

	//! This function performs no operation as it is a virtual placeholder implementation.
	virtual void			  Reload() { }

	//! Ensures that the null skin declaration is not purged from memory.
	virtual void			  EnsureNotPurged() { }

	//! Returns the index of the null skin base, which is always -1.
	virtual int				  Index() const { return -1; }

	//! Returns the line number where the declaration starts.
	virtual int				  GetLineNum() const { return 0; }

	//! Returns the file name associated with this declaration
	virtual const char*		  GetFileName() const { return ""; }

	//! Initializes the provided text buffer with an empty string
	virtual void			  GetText( char* text ) const { text[0] = '\0'; }

	//! Returns the length of the text representation for this declaration.
	virtual int				  GetTextLength() const { return 1; }

	//! Sets the text content of the declaration.
	virtual void			  SetText( const char* text ) { }

	//! Returns false indicating that source file text replacement is not supported.
	virtual bool			  ReplaceSourceFileText() { return false; }

	//! Returns false indicating that the source file has not changed.
	virtual bool			  SourceFileChanged() const { return false; }

	//! Initializes the declaration state to its default empty configuration.
	virtual void			  MakeDefault() { }

	//! Returns false indicating the decl was never referenced
	virtual bool			  EverReferenced() const { return false; }

	//! Sets the default text for the declaration, returning false to indicate no default text was generated.
	virtual bool			  SetDefaultText() { return false; }

	//! Returns the default definition string for a null skin declaration.
	virtual const char*		  DefaultDefinition() const { return ""; }

	//! Parses the declaration text and returns false to indicate parsing is not supported.
	virtual bool			  Parse( const char* text, const int textLength, bool allowBinaryVersion ) { return false; }

	//! Frees any data associated with this declaration
	virtual void			  FreeData() { }

	//! Returns the size of the declaration in memory
	virtual size_t			  Size() const { return 0; }

	//! Prints a brief listing of the declaration.
	virtual void			  List() const { }

	//! Prints the declaration data
	virtual void			  Print() const { }

	//! Returns the timestamp of the source file for this declaration.
	ID_TIME_T				  GetSourceFileTimestamp() const { return 0; }

	static idDeclNullSkinBase instance;
};

/*!
	\class idDeclSkinWrapper
	\brief A wrapper class that manages skin declarations by wrapping and remapping shader mappings.
	\author Leyland
*/
class idDeclSkinWrapper : public idDeclSkin
{
public:
	//! Initializes an idDeclSkinWrapper instance with default values.
	idDeclSkinWrapper();

	//! Returns the remapped shader for the given shader based on the skin wrapper's mapping rules.
	virtual const idMaterial* RemapShaderBySkin( const idMaterial* shader ) const;

	//! Sets the skin wrapper for this skin wrapper object.
	void					  SetWrapper( const idDeclSkin* skin );

	//! Sets the wrapped skin for this skin wrapper.
	void					  SetWrapped( const idDeclSkin* skin );

	//! Returns the wrapped skin declaration.
	const idDeclSkin*		  GetWrapper() { return wrapper; }

	//! Returns a pointer to the wrapped idDeclSkin object.
	const idDeclSkin*		  GetWrapped() { return wrapped; }

protected:
	const idDeclSkin* wrapper;
	const idDeclSkin* wrapped;
};
// Leyland end

#endif /* !__DECLSKIN_H__ */
