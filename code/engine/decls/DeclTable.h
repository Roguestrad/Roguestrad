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

#ifndef __DECLTABLE_H__
#define __DECLTABLE_H__

/*!
	\class idDeclTable
	\brief A class for managing declaration tables with parsing, lookup, and memory management capabilities.

	Tables are used to map a floating point input value to a floating point
	output value, with optional wrap / clamp and interpolation in material files.

	This is the more flexible replacement for the Quake 3 fixed function tables like rgbGen wave.
*/
class idDeclTable : public idDecl
{
public:
	//! Returns the total memory size occupied by this declaration table instance
	virtual size_t		Size() const;

	//! Returns the default definition string for declaration table entries.
	virtual const char* DefaultDefinition() const;

	//! Parses a table declaration from text, extracting values and optional snap and clamp flags.
	virtual bool		Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees the data associated with the declaration table.
	virtual void		FreeData();

	//! Performs table lookup interpolation for a given index value
	float				TableLookup( float index ) const;

private:
	bool							   clamp;
	bool							   snap;
	idList<float, TAG_IDLIB_LIST_DECL> values;
};

#endif /* !__DECLTABLE_H__ */
