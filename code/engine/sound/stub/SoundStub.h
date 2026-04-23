/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Daniel Gibson

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

/*
 * DG: a stub to get d3 bfg to compile without XAudio2, because that doesn't work with MinGW
 * or on non-Windows platforms.
 *
 * Please note that many methods are *not* virtual, so just inheriting from the stubs for the
 * actual implementations may *not* work!
 * (Making them virtual should be evaluated for performance-loss though, it would make the code
 *  cleaner and may be feasible)
 */

#ifndef SOUNDSTUB_H_
#define SOUNDSTUB_H_

#include "idlib/precompiled.h" // TIME_T
#include "../WaveFile.h"

class idSoundVoice : public idSoundVoice_Base
{
public:
	//! Initializes the sound voice with lead-in and looping sound samples
	void   Create( const idSoundSample* leadinSample, const idSoundSample* loopingSample ) { }

	//! Starts playing sound at a specified offset with given flags.
	void   Start( int offsetMS, int ssFlags ) { }

	//! Stops the sound voice from playing.
	void   Stop() { }

	//! Pauses the sound voice execution.
	void   Pause() { }

	//! Resumes sound processing after a pause.
	void   UnPause() { }

	//! Updates the sound voice hardware state with new position, volume, and pitch information.
	bool   Update() { return false; }

	//! Returns the RMS amplitude of the most recently processed audio block
	float  GetAmplitude() { return 0.0f; }

	//! Returns true if the voice can be reused for the given sound sample.
	bool   CompatibleFormat( idSoundSample* s ) { return false; }

	//! Returns the sample rate of the sound voice.
	uint32 GetSampleRate() const { return 0; }

	//! Callback function invoked when a sound buffer starts playing.
	void   OnBufferStart( idSoundSample* sample, int bufferNumber ) { }
};

class idSoundHardware
{
public:
	//! Constructs a new instance of the sound hardware class.
	idSoundHardware() { }

	//! Initializes the sound hardware interface
	void		  Init() { }

	//! Shuts down the sound hardware system
	void		  Shutdown() { }

	//! Updates the sound hardware state.
	void		  Update() { }

	//! Returns a void pointer to the XAudio2 interface, or NULL if not available.
	void*		  GetIXAudio2() const { return NULL; }

	//! Allocates a sound voice for playback of lead-in and looping sound samples
	idSoundVoice* AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample ) { return NULL; }

	//! Frees the specified sound voice resource.
	void		  FreeVoice( idSoundVoice* voice ) { }

	//! Returns the number of zombie voices currently in use by the sound hardware.
	int			  GetNumZombieVoices() const { return 0; }

	//! Returns the number of free audio voices available for playback.
	int			  GetNumFreeVoices() const { return 0; }
};

// ok, this one isn't really a stub, because it seems to be XAudio-independent,
// I just copied the class from idSoundSample_XAudio2 and renamed it
class idSoundSample
{
public:
	//! Constructs a new idSoundSample object with default values.
	idSoundSample();

	//! Destroys the sound sample and frees its associated data.
	~idSoundSample();

	//! Loads and initializes sound resource data
	virtual void LoadResource();

	//! Sets the name of the sound sample to the provided string.
	void		 SetName( const char* n ) { name = n; }

	//! Returns the name of the sound sample.
	const char*	 GetName() const { return name; }

	//! Retrieves the timestamp associated with the sound sample.
	ID_TIME_T	 GetTimestamp() const { return timestamp; }

	//! Initializes the sound sample with default beep-like audio data
	void		 MakeDefault();

	//! Frees all allocated data buffers and resets the sound sample state.
	void		 FreeData();

	//! Returns the length of the sound sample in milliseconds.
	int			 LengthInMsec() const { return SamplesToMsec( NumSamples(), SampleRate() ); }

	//! Returns the sample rate of the sound sample in Hz.
	int			 SampleRate() const { return format.basic.samplesPerSec; }

	//! Returns the number of samples in the sound sample.
	int			 NumSamples() const { return playLength; }

	//! Returns the number of audio channels in the sound sample.
	int			 NumChannels() const { return format.basic.numChannels; }

	//! Returns the total buffer size of the sound sample.
	int			 BufferSize() const { return totalBufferSize; }

	//! Returns true if the sound sample is in a compressed format
	bool		 IsCompressed() const { return ( format.basic.formatTag != idWaveFile::FORMAT_PCM ); }

	//! Returns true if the sound sample is the default sample, indicating it was not found.
	bool		 IsDefault() const { return timestamp == FILE_NOT_FOUND_TIMESTAMP; }

	//! Returns true if the sound sample has been successfully loaded.
	bool		 IsLoaded() const { return loaded; }

	//! Sets the sound sample to never be purged from memory.
	void		 SetNeverPurge() { neverPurge = true; }

	//! Returns whether the sound sample should never be purged during level load operations.
	bool		 GetNeverPurge() const { return neverPurge; }

	//! Marks the sound sample as referenced during level load.
	void		 SetLevelLoadReferenced() { levelLoadReferenced = true; }

	//! Resets the level load referenced flag to false.
	void		 ResetLevelLoadReferenced() { levelLoadReferenced = false; }

	//! Returns whether the sound sample was referenced during level loading.
	bool		 GetLevelLoadReferenced() const { return levelLoadReferenced; }

	//! Returns the last time the sound sample was played.
	int			 GetLastPlayedTime() const { return lastPlayedTime; }

	//! Sets the last played time for the sound sample.
	void		 SetLastPlayedTime( int t ) { lastPlayedTime = t; }

	//! Returns the amplitude of the sound sample at the specified time in milliseconds
	float		 GetAmplitude( int timeMS ) const;

protected:
	//! Loads a WAV audio file and prepares its data for playback.
	bool LoadWav( const idStr& name );

	//! Loads amplitude data from a file into the sound sample
	bool LoadAmplitude( const idStr& name );

	//! Writes all samples to a file with the specified name.
	void WriteAllSamples( const idStr& sampleName );

	//! Loads a generated sound sample from a file
	bool LoadGeneratedSample( const idStr& name );

	//! Writes the generated sound sample data to the specified file output.
	void WriteGeneratedSample( idFile* fileOut );

	struct sampleBuffer_t {
		void* buffer;
		int	  bufferSize;
		int	  numSamples;
	};

	idStr							  name;

	ID_TIME_T						  timestamp;
	bool							  loaded;

	bool							  neverPurge;
	bool							  levelLoadReferenced;
	bool							  usesMapHeap;

	uint32							  lastPlayedTime;

	int								  totalBufferSize; // total size of all the buffers
	idList<sampleBuffer_t, TAG_AUDIO> buffers;

	int								  playBegin;
	int								  playLength;

	idWaveFile::waveFmt_t			  format;

	idList<byte, TAG_AMPLITUDE>		  amplitude;
};

#endif /* SOUNDSTUB_H_ */
