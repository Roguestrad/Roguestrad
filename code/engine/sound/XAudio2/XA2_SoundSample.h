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
#ifndef __XA2_SOUNDSAMPLE_H__
#define __XA2_SOUNDSAMPLE_H__

class idSampleInfo;

/*!
	\class idSoundSample_XAudio2
	\brief Manages sound sample data and provides functionality for loading, querying, and manipulating audio resources in XAudio2.

	This class serves as a container for sound sample data and provides methods for loading various audio formats, including WAV, Ogg Vorbis, and generated samples. It maintains metadata about the
   audio such as sample rate, number of channels, and buffer size, and offers methods to query the length, amplitude, and other properties of the sound sample. The class supports loading audio
   resources from files, initializing default samples, and managing the lifecycle of loaded data through load and free operations. Additionally, it tracks loading and playback state information, such
   as timestamps, last played time, and reference flags used during level loading operations. The class interfaces with XAudio2 for audio playback and resource management.

*/
class idSoundSample_XAudio2
{
public:
	//! Initializes a new instance of the idSoundSample_XAudio2 class with default values.
	idSoundSample_XAudio2();

	//! Loads and initializes the sound resource based on the sample name.
	virtual void LoadResource();

	//! Sets the name of the sound sample.
	void		 SetName( const char* n ) { name = n; }

	//! Returns the name of the sound sample.
	const char*	 GetName() const { return name; }

	//! Retrieves the timestamp value stored in the idSoundSample_XAudio2 instance.
	ID_TIME_T	 GetTimestamp() const { return timestamp; }

	//! Initializes the sound sample with default PCM audio data containing a beep pattern
	void		 MakeDefault();

	//! Frees all allocated data and resets the sound sample state
	void		 FreeData();

	//! Returns the length of the sound sample in milliseconds.
	int			 LengthInMsec() const { return SamplesToMsec( NumSamples(), SampleRate() ); }

	//! Returns the sample rate of the sound sample in samples per second.
	int			 SampleRate() const { return format.basic.samplesPerSec; }

	//! Returns the total number of samples in the sound sample.
	int			 NumSamples() const { return playLength; }

	//! Returns the number of audio channels in the sound sample.
	int			 NumChannels() const { return format.basic.numChannels; }

	//! Returns the total buffer size of the sound sample.
	int			 BufferSize() const { return totalBufferSize; }

	//! Returns true if the sound sample is in a compressed format
	bool		 IsCompressed() const { return ( format.basic.formatTag != idWaveFile::FORMAT_PCM ); }

	//! Returns true if the sound sample is the default sample.
	bool		 IsDefault() const { return timestamp == FILE_NOT_FOUND_TIMESTAMP; }

	//! Returns true if the sound sample has been successfully loaded.
	bool		 IsLoaded() const { return loaded; }

	//! Sets the sound sample to never be purged from memory.
	void		 SetNeverPurge() { neverPurge = true; }

	//! Returns whether the sound sample should never be purged during level load operations.
	bool		 GetNeverPurge() const { return neverPurge; }

	//! Marks the sound sample as referenced during level load.
	void		 SetLevelLoadReferenced() { levelLoadReferenced = true; }

	//! Resets the level load reference flag for the sound sample.
	void		 ResetLevelLoadReferenced() { levelLoadReferenced = false; }

	//! Returns whether the sound sample is referenced during level loading.
	bool		 GetLevelLoadReferenced() const { return levelLoadReferenced; }

	//! Returns the last played time of the sound sample.
	int			 GetLastPlayedTime() const { return lastPlayedTime; }

	//! Sets the last played time for the sound sample.
	void		 SetLastPlayedTime( int t ) { lastPlayedTime = t; }

	//! Retrieves the amplitude of the sound sample at a specified time in milliseconds.
	float		 GetAmplitude( int timeMS ) const;

protected:
	friend class idSoundHardware_XAudio2;
	friend class idSoundVoice_XAudio2;

	//! Destructor for the idSoundSample_XAudio2 class that releases allocated resources.
	~idSoundSample_XAudio2();

	//! Loads a WAV audio file into the sound sample for XAudio2.
	bool LoadWav( const idStr& name );

	//! Loads an Ogg Vorbis sound sample from the specified file name.
	bool LoadOgg( const idStr& name );

	//! Loads amplitude data from a file into the sound sample
	bool LoadAmplitude( const idStr& name );

	//! Writes all samples to a file
	void WriteAllSamples( const idStr& sampleName );

	//! Loads a generated sound sample from a file
	bool LoadGeneratedSample( const idStr& name );

	//! Writes the generated sound sample data to the specified file output stream.
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

/*
================================================
idSoundSample

This reverse-inheritance purportedly makes working on
multiple platforms easier.
================================================
*/
class idSoundSample : public idSoundSample_XAudio2
{
public:
};

#endif
