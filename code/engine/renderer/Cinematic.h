/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014 Carl Kenner

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

#ifndef __CINEMATIC_H__
#define __CINEMATIC_H__

/*
===============================================================================

	cinematic

	Multiple idCinematics can run simultaniously.
	A single idCinematic can be reused for multiple files if desired.

===============================================================================
*/

// cinematic states
typedef enum {
	FMV_IDLE,
	FMV_PLAY, // play
	FMV_EOF,  // all other conditions, i.e. stop/EOF/abort
	FMV_ID_BLT,
	FMV_ID_IDLE,
	FMV_LOOPED,
	FMV_ID_WAIT
} cinStatus_t;

class idImage;

// a cinematic stream generates an image buffer, which the caller will upload to a texture
typedef struct {
	int		 imageWidth;
	int		 imageHeight; // will be a power of 2
	idImage* imageY;
	idImage* imageCr;
	idImage* imageCb;
	idImage* image;
	int		 status;
} cinData_t;

/*!
	\class idCinematic
	\brief Manages cinematic playback including initialization, rendering, and resource management.

	The idCinematic class provides functionality for loading, playing, and rendering cinematic sequences. It handles initialization from file, memory management, and frame rendering using provided
   command lists. The class supports looping playback, time reset, and frame rate control. It includes methods for exporting frames to TGA format and querying cinematic properties such as length and
   start time. The class uses a factory pattern for allocation with a private subclass implementation. Memory cleanup is handled through explicit shutdown and close methods.

*/
class idCinematic
{
public:
	//! Initializes cinematic playback data including YUV conversion tables and memory allocations for video processing.
	static void			InitCinematic();

	//! Shuts down the cinematic playback data by freeing allocated memory.
	static void			ShutdownCinematic();

	//! Allocates and returns a private subclass that implements the cinematic methods
	static idCinematic* Alloc();

	//! Cleans up all allocated memory for the cinematic.
	virtual ~idCinematic();

	//! Initializes the cinematic from a file and returns true if successful
	virtual bool	  InitFromFile( const char* qpath, bool looping, nvrhi::ICommandList* commandList );

	//! Returns the length of the animation in milliseconds
	virtual int		  AnimationLength();

	//! Returns true if the cinematic is currently playing, false otherwise.
	virtual bool	  IsPlaying() const;

	//! Returns cinematic image data for the specified time using the provided command list.
	virtual cinData_t ImageForTime( int milliseconds, nvrhi::ICommandList* commandList );

	//! Closes the cinematic file and frees all allocated memory.
	virtual void	  Close();

	//! Resets the cinematic time to the specified milliseconds value, allowing the cinematic to start at that time, even if it's in the past.
	virtual void	  ResetTime( int time );

	//! Returns the start time of the cinematic
	virtual int		  GetStartTime();

	//! Exports cinematic frames to TGA image files
	virtual void	  ExportToTGA( bool skipExisting = true );

	//! Returns the fixed frame rate of 30.0 for the cinematic playback.
	virtual float	  GetFrameRate() const;
};

/*!
	\class idSndWindow
	\brief Manages sound window visualization and playback functionality.
*/
class idSndWindow : public idCinematic
{
public:
	//! Initializes a new instance of the idSndWindow class with waveform display disabled.
	idSndWindow() { showWaveform = false; }
	~idSndWindow() { }

	//! Initializes the sound window from a file path, setting waveform display based on the file name.
	bool	  InitFromFile( const char* qpath, bool looping );

	//! Returns cinematic image data for the specified time in milliseconds.
	cinData_t ImageForTime( int milliseconds );

	//! Returns the animation length of the sound window.
	int		  AnimationLength();

private:
	bool showWaveform;
};

#endif /* !__CINEMATIC_H__ */
