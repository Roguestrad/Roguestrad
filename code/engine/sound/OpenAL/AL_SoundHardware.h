/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans

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
#ifndef __AL_SOUNDHARDWARE_H__
#define __AL_SOUNDHARDWARE_H__

class idSoundSample_OpenAL;
class idSoundVoice_OpenAL;
class idSoundHardware_OpenAL;

/*!
	\class idSoundHardware_OpenAL
	\brief Provides OpenAL-based audio hardware abstraction and management for sound playback.

	This class serves as a bridge between the audio subsystem and OpenAL, handling device and context initialization, voice allocation and management, and system updates. It maintains pools of
   available and zombie voices for efficient audio resource utilization. The class initializes OpenAL hardware during startup and properly cleans up resources during shutdown. It provides methods for
   allocating voices for sound playback, freeing voices, and updating the hardware state. The implementation includes utility functions for printing device and implementation information for debugging
   purposes.

*/
class idSoundHardware_OpenAL
{
public:
	//! Initializes a new instance of the idSoundHardware_OpenAL class.
	idSoundHardware_OpenAL();

	//! Initializes the OpenAL sound hardware by creating a device and context, and sets up sound system components.
	void		  Init();

	//! Shuts down the OpenAL sound hardware and cleans up all associated resources.
	void		  Shutdown();

	//! Updates the OpenAL sound hardware state and handles zombie sound voices.
	void		  Update();

	//! Allocates a sound voice for playing a leadin sample with an optional looping sample.
	idSoundVoice* AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample );

	//! Frees the specified sound voice by stopping it and adding it to the zombie voices list.
	void		  FreeVoice( idSoundVoice* voice );

	// listDevices needs this
	ALCdevice*	  GetOpenALDevice() const { return openalDevice; };

	//! Returns the number of zombie voices managed by the OpenAL sound hardware.
	int			  GetNumZombieVoices() const { return zombieVoices.Num(); }

	//! Returns the number of free audio voices available for use.
	int			  GetNumFreeVoices() const { return freeVoices.Num(); }

	//! Prints a list of OpenAL audio devices to the console.
	static void	  PrintDeviceList( const char* list );

	//! Prints OpenAL and OpenAL Soft information for the specified device.
	static void	  PrintALCInfo( ALCdevice* device );

	//! Prints OpenAL implementation information including vendor, renderer, version, and extensions.
	static void	  PrintALInfo();

protected:
	friend class idSoundSample_OpenAL;
	friend class idSoundVoice_OpenAL;

private:
	ALCdevice*													openalDevice;
	ALCcontext*													openalContext;

	int															lastResetTime;

	// int				outputChannels;
	// int				channelMask;

	// idDebugGraph* 	vuMeterRMS;
	// idDebugGraph* 	vuMeterPeak;
	// int				vuMeterPeakTimes[ 8 ];

	// Can't stop and start a voice on the same frame, so we have to double this to handle the worst case scenario of stopping all voices and starting a full new set
	idStaticList<idSoundVoice_OpenAL, MAX_HARDWARE_VOICES * 2>	voices;
	idStaticList<idSoundVoice_OpenAL*, MAX_HARDWARE_VOICES * 2> zombieVoices;
	idStaticList<idSoundVoice_OpenAL*, MAX_HARDWARE_VOICES * 2> freeVoices;
};

/*!
	\class idSoundHardware
	\brief Manages audio hardware resources and playback for sound processing.
*/
class idSoundHardware : public idSoundHardware_OpenAL
{
};

#endif
