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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

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

typedef struct
{
	const idMaterial* 		from;			// 0 == any unmatched shader
	const idMaterial* 		to;
} skinMapping_t;

class idDeclSkin : public idDecl
{
public:
	virtual size_t			Size() const;
	virtual bool			SetDefaultText();
	virtual const char* 	DefaultDefinition() const;
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion );
	virtual void			FreeData();

	virtual const idMaterial* 		RemapShaderBySkin( const idMaterial* shader ) const;// Leyland VR

	// model associations are just for the preview dialog in the editor
	const int				GetNumModelAssociations() const;
	const char* 			GetAssociatedModel( int index ) const;

protected:
	idList<skinMapping_t, TAG_IDLIB_LIST_DECL>	mappings;
	idStrList				associatedModels;
};

// Leyland VR
class idDeclNullSkinBase : public idDeclBase
{
public:
	virtual const char* 	GetName() const
	{
		return "";
	}
	virtual declType_t		GetType() const
	{
		return DECL_SKIN;
	}
	virtual declState_t		GetState() const
	{
		return DS_PARSED;
	}
	virtual bool			IsImplicit() const
	{
		return false;
	}
	virtual bool			IsValid() const
	{
		return true;
	}
	virtual void			Invalidate() {}
	virtual void			Reload() {}
	virtual void			EnsureNotPurged() {}
	virtual int				Index() const
	{
		return -1;
	}
	virtual int				GetLineNum() const
	{
		return 0;
	}
	virtual const char* 	GetFileName() const
	{
		return "";
	}
	virtual void			GetText( char* text ) const
	{
		text[0] = '\0';
	}
	virtual int				GetTextLength() const
	{
		return 1;
	}
	virtual void			SetText( const char* text ) {}
	virtual bool			ReplaceSourceFileText()
	{
		return false;
	}
	virtual bool			SourceFileChanged() const
	{
		return false;
	}
	virtual void			MakeDefault() {}
	virtual bool			EverReferenced() const
	{
		return false;
	}
	virtual bool			SetDefaultText()
	{
		return false;
	}
	virtual const char* 	DefaultDefinition() const
	{
		return "";
	}
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion )
	{
		return false;
	}
	virtual void			FreeData() {}
	virtual size_t			Size() const
	{
		return 0;
	}
	virtual void			List() const {}
	virtual void			Print() const {}

	// RB
	ID_TIME_T				GetSourceFileTimestamp() const
	{
		return 0;
	}

	static idDeclNullSkinBase instance;
};

class idDeclSkinWrapper : public idDeclSkin
{
public:
	idDeclSkinWrapper();

	virtual const idMaterial* RemapShaderBySkin( const idMaterial* shader ) const;
	void SetWrapper( const idDeclSkin* skin );
	void SetWrapped( const idDeclSkin* skin );

	const idDeclSkin* GetWrapper()
	{
		return wrapper;
	}

	const idDeclSkin* GetWrapped()
	{
		return wrapped;
	}

protected:
	const idDeclSkin* wrapper;
	const idDeclSkin* wrapped;
};
// Leyland end

#endif /* !__DECLSKIN_H__ */
