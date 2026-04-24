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

#ifndef __SIMPLEWIN_H__
#define __SIMPLEWIN_H__

class idUserInterfaceLocal;
class idDeviceContext;
class idSimpleWindow;

typedef struct {
	idWindow*		win;
	idSimpleWindow* simp;
} drawWin_t;

/*!
	\class idSimpleWindow
	\brief A lightweight window wrapper that provides simplified access to window properties and rendering functionality.

	This class serves as a simplified interface for window management, offering access to window variables and basic rendering operations. It maintains a reference to an underlying window object and
   provides methods to query and modify window properties, calculate display coordinates, and handle rendering operations. The class supports serialization for save game functionality and provides
   mechanisms for updating window state and drawing operations. It is designed to facilitate efficient window handling while maintaining compatibility with existing window systems through the idWindow
   interface.

*/
class idSimpleWindow
{
	friend class idWindow;

public:
	//! Initializes a simple window by copying properties from an existing window.
	idSimpleWindow( idWindow* win );
	virtual ~idSimpleWindow();

	//! Redraws the window at the specified offset position.
	void		 Redraw( float x, float y );

	//! Updates the window state and optionally redraws the window.
	void		 StateChanged( bool redraw );

	idStr		 name;

	//! Returns a pointer to the window variable with the specified name, or NULL if not found.
	idWinVar*	 GetWinVarByName( const char* _name );

	//! Returns the offset of a specified window variable within the class instance
	int			 GetWinVarOffset( idWinVar* wv, drawWin_t* owner );

	//! Returns the total memory size of the simple window object including its member strings
	size_t		 Size();

	//! Returns the parent window of this simple window
	idWindow*	 GetParent() { return mParent; }

	//! Writes the window's state to a save game file.
	virtual void WriteToSaveGame( idFile* savefile );

	//! Loads window state information from a save game file
	virtual void ReadFromSaveGame( idFile* savefile );

protected:
	//! Calculates the client rectangle for the window based on the provided offsets and window properties.
	void				  CalcClientRect( float xofs, float yofs );

	//! Sets up transformation matrices for the window based on the provided offsets and current window properties.
	void				  SetupTransforms( float x, float y );

	//! Draws the background of the window using the specified rectangle area.
	void				  DrawBackground( const idRectangle& drawRect );

	//! Draws the border and caption for the simple window using the provided rectangle dimensions.
	void				  DrawBorderAndCaption( const idRectangle& drawRect );

	idUserInterfaceLocal* gui;
	int					  flags;
	idRectangle			  drawRect;	  // overall rect
	idRectangle			  clientRect; // client area
	idRectangle			  textRect;
	idVec2				  origin;
	class idFont*		  font;
	float				  matScalex;
	float				  matScaley;
	float				  borderSize;
	int					  textAlign;
	float				  textAlignx;
	float				  textAligny;
	int					  textShadow;

	idWinStr			  text;
	idWinBool			  visible;
	idWinRectangle		  rect; // overall rect
	idWinVec4			  backColor;
	idWinVec4			  matColor;
	idWinVec4			  foreColor;
	idWinVec4			  borderColor;
	idWinFloat			  textScale;
	idWinFloat			  rotate;
	idWinVec2			  shear;
	idWinBackground		  backGroundName;

	const idMaterial*	  background;

	idWindow*			  mParent;

	idWinBool			  hideCursor;
};

#endif /* !__SIMPLEWIN_H__ */
