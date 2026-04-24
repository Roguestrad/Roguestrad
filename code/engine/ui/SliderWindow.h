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

#ifndef __SLIDERWINDOW_H__
#define __SLIDERWINDOW_H__

class idUserInterfaceLocal;

/*!
	\class idSliderWindow
	\brief A slider window component for user interface interactions with customizable range and value handling.

	The idSliderWindow class implements a user interface element that allows users to select a value from a continuous range. It supports both horizontal and vertical orientation and provides methods
   to set and retrieve range limits, step size, and current value. The class handles UI events such as mouse input for value adjustment and integrates with console variables for persistent state
   management. It includes visual rendering capabilities and maintains references to associated windows and user interface contexts. The component can be configured with default properties and
   supports activation states that influence its behavior and visual representation.

*/
class idSliderWindow : public idWindow
{
public:
	//! Constructs an idSliderWindow object with the specified user interface
	idSliderWindow( idUserInterfaceLocal* gui );
	virtual ~idSliderWindow();

	/*!
		\brief Initializes the slider window with default settings and graphical properties.

		Configures the slider window using provided parameters including name, rectangle bounds, colors, background and thumb materials, and orientation settings. Sets up the visual properties such as
	   sort order for materials and captures the dimensions of the thumb image. The window is configured to hold capture during interaction.

		\param _name Name of the slider window
		\param rect Rectangle defining the window's position and size
		\param foreColor Foreground color of the slider
		\param matColor Material color used for rendering
		\param _background Name of the background material shader
		\param thumbShader Name of the thumb material shader
		\param _vertical Flag indicating if the slider is vertical
		\param _scrollbar Flag indicating if the slider acts as a scrollbar
	*/
	void InitWithDefaults(
		const char* _name, const idRectangle& rect, const idVec4& foreColor, const idVec4& matColor, const char* _background, const char* thumbShader, bool _vertical, bool _scrollbar );

	//! Sets the range and step size for the slider window.
	void				SetRange( float _low, float _high, float _step );

	//! Returns the lower bound value of the slider window.
	float				GetLow() { return low; }

	//! Returns the high value of the slider window.
	float				GetHigh() { return high; }

	//! Sets the current value of the slider window to the specified float value.
	void				SetValue( float _value );
	float				GetValue() { return value; };

	virtual size_t		Allocated() { return idWindow::Allocated(); };

	//! Retrieves a window variable by name from the slider window, returning built-in variables or falling back to the parent window's lookup.
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Handles UI events for a slider window, updating its value based on input.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Initializes the slider window after parsing, setting up its visual properties and configuration.
	virtual void		PostParse();

	//! Draws the slider window at the specified position and time
	virtual void		Draw( int time, float x, float y );

	//! Draws the background of the slider window within the specified rectangle.
	virtual void		DrawBackground( const idRectangle& drawRect );

	//! Processes mouse coordinates for slider window interaction and updates the slider value.
	virtual const char* RouteMouseCoords( float xd, float yd );

	//! Activates the slider window and updates its associated console variable.
	virtual void		Activate( bool activate, idStr& act );

	//! Sets the buddy window for this slider window.
	virtual void		SetBuddy( idWindow* buddy );

	//! Processes a named event for the slider window, specifically handling cvar read and write operations based on the event name.
	void				RunNamedEvent( const char* eventName );

private:
	//! Parses internal variables for the slider window, handling properties like step size, range, and visual settings.
	virtual bool	  ParseInternalVar( const char* name, idTokenParser* src );

	//! Initializes the slider window properties with default values.
	void			  CommonInit();

	//! Initializes the cvar reference for the slider window.
	void			  InitCvar();

	//! Updates the cvar value based on the specified read flag and force parameter.
	void			  UpdateCvar( bool read, bool force = false );

	idWinFloat		  value;
	float			  low;
	float			  high;
	float			  thumbWidth;
	float			  thumbHeight;
	float			  stepSize;
	float			  lastValue;
	idRectangle		  thumbRect;
	const idMaterial* thumbMat;
	bool			  vertical;
	bool			  verticalFlip;
	bool			  scrollbar;
	idWindow*		  buddyWin;
	idStr			  thumbShader;

	idWinStr		  cvarStr;
	idCVar*			  cvar;
	bool			  cvar_init;
	idWinBool		  liveUpdate;
	idWinStr		  cvarGroup;
};

#endif /* !__SLIDERWINDOW_H__ */
