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
#ifndef __RENDERWINDOW_H
#define __RENDERWINDOW_H

class idUserInterfaceLocal;

/*!
	\class idRenderWindow
	\brief A render window class that manages rendering operations and window variables for a user interface.

	This class represents a specialized window implementation designed for rendering graphical content within a user interface system. It extends the base window functionality to provide rendering
   capabilities and manages the association with a user interface instance. The class handles initialization, parsing of internal variables, animation building, and actual rendering operations. It
   maintains a connection to a render world that is properly freed when the window is destroyed. The window variable management supports both regular and predefined variable lookups with special
   handling for specific parameters like animClass. Memory allocation tracking is provided through the allocated method, which accounts for the window object and its associated data structures. The
   rendering pipeline includes pre-render initialization, animation state updates, and frame rendering with timing information.

*/
class idRenderWindow : public idWindow
{
public:
	//! Initializes a render window with the specified user interface.
	idRenderWindow( idUserInterfaceLocal* gui );

	//! Destroys the render window and frees the associated render world.
	virtual ~idRenderWindow();

	//! Initializes the render window after parsing.
	virtual void	  PostParse();

	//! Renders the window content with specified time and position parameters
	virtual void	  Draw( int time, float x, float y );
	virtual size_t	  Allocated() { return idWindow::Allocated(); };

	//! Returns a window variable by name from the render window, with special handling for predefined variables.
	virtual idWinVar* GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );
	//

private:
	//! Initializes the render window common properties.
	void			 CommonInit();

	//! Parses internal variables for the render window, specifically handling the animClass parameter.
	virtual bool	 ParseInternalVar( const char* name, idTokenParser* src );

	//! Renders the current frame with the specified time parameter.
	void			 Render( int time );

	//! Initializes the render window's world and entities before rendering.
	void			 PreRender();

	//! Updates the animation state for the render window based on the provided time.
	void			 BuildAnimation( int time );
	renderView_t	 refdef;
	idRenderWorld*	 world;
	renderEntity_t	 worldEntity;
	renderLight_t	 rLight;
	const idMD5Anim* modelAnim;

	qhandle_t		 worldModelDef;
	qhandle_t		 lightDef;
	qhandle_t		 modelDef;
	idWinStr		 modelName;
	idWinStr		 animName;
	idStr			 animClass;
	idWinVec4		 lightOrigin;
	idWinVec4		 lightColor;
	idWinVec4		 modelOrigin;
	idWinVec4		 modelRotate;
	idWinVec4		 viewOffset;
	idWinBool		 needsRender;
	int				 animLength;
	int				 animEndTime;
	bool			 updateAnimation;
};

#endif // __RENDERWINDOW_H
