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
#ifndef __FIELDWINDOW_H
#define __FIELDWINDOW_H

#include "Window.h"

/*!
	\class idFieldWindow
	\brief A window element for displaying and managing text input fields.
*/
class idFieldWindow : public idWindow
{
public:
	//! Constructs an idFieldWindow object with the specified user interface.
	idFieldWindow( idUserInterfaceLocal* gui );
	virtual ~idFieldWindow();

	//! Draws the field window text at the specified position with the given time parameter.
	virtual void Draw( int time, float x, float y );

private:
	//! Parses internal variables for the field window, handling cursor and show cursor settings.
	virtual bool ParseInternalVar( const char* name, idTokenParser* src );

	//! Initializes the field window common properties.
	void		 CommonInit();

	//! Calculates the paint offset for text rendering based on the provided length.
	void		 CalcPaintOffset( int len );
	int			 cursorPos;
	int			 lastTextLength;
	int			 lastCursorPos;
	int			 paintOffset;
	bool		 showCursor;
	idStr		 cursorVar;
};

#endif // __FIELDWINDOW_H
