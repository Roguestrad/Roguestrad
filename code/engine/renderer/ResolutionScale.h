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
#ifndef __RESOLUTIONSCALE_H__
#define __RESOLUTIONSCALE_H__

/*!
	\class idResolutionScale
	\brief Manages dynamic resolution scaling based on performance metrics.
*/
class idResolutionScale
{
public:
	//! Constructs an idResolutionScale object with default values.
	idResolutionScale();

	//! Initializes the resolution scale settings for the specified map.
	void InitForMap( const char* mapName );

	//! Retrieves the current horizontal and vertical resolution scale factors.
	void GetCurrentResolutionScale( float& x, float& y );

	//! Resets the resolution scale to full resolution.
	void ResetToFullResolution();

	//! Sets the current GPU frame time and adjusts the resolution scale based on the timing.
	void SetCurrentGPUFrameTime( int microseconds );

	//! Populates the provided string with console display text representing the current resolution scale settings.
	void GetConsoleText( idStr& s );

private:
	float dropMilliseconds;
	float raiseMilliseconds;
	int	  framesAboveRaise;
	float currentResolution;
};

extern idResolutionScale resolutionScale;

#endif // __RESOLUTIONSCALE_H__
