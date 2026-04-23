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
#ifndef __AL_SOUNDSAMPLE_H__
#define __AL_SOUNDSAMPLE_H__

class idSampleInfo;

/*!
	\class idSoundSample_OpenAL
	\brief Manages OpenAL sound sample resources including loading, playback, and memory management.

	This class represents a sound sample specifically designed for OpenAL audio playback. It handles the loading and management of audio data from various sources including WAV files, generated
   samples, and amplitude data. The class maintains metadata about the sound sample such as its name, timestamp, sample rate, and buffer size while providing methods to control memory behavior and
   playback state. It supports both compressed and uncompressed audio formats and includes functionality for creating OpenAL buffers, decoding MS ADPCM data, and tracking when samples were last
   played. The implementation handles resource cleanup through its destructor and provides methods to mark samples as never-purge or referenced during level loading. Memory management is primarily
   focused on OpenAL buffer allocation and deallocation without explicit ownership transfer.

*/
class idSoundSample_OpenAL
{
public:
	//! Initializes a new instance of the idSoundSample_OpenAL class with default values.
	idSoundSample_OpenAL();

	//! Loads and initializes sound sample data for OpenAL playback
	virtual void LoadResource();

	//! Sets the name of the sound sample.
	void		 SetName( const char* n ) { name = n; }

	//! Returns the name of the sound sample.
	const char*	 GetName() const { return name; }

	//! Returns the timestamp associated with this sound sample.
	ID_TIME_T	 GetTimestamp() const { return timestamp; }

	//! Initializes the sound sample with default audio data consisting of a beep waveform
	void		 MakeDefault();

	//! Frees all allocated OpenAL buffers and resets the sound sample data state.
	void		 FreeData();

	//! Returns the length of the sound sample in milliseconds.
	int			 LengthInMsec() const { return SamplesToMsec( NumSamples(), SampleRate() ); }

	//! Returns the sample rate of the sound sample in Hertz.
	int			 SampleRate() const { return format.basic.samplesPerSec; }

	//! Returns the number of samples in the sound data.
	int			 NumSamples() const { return playLength; }

	//! Returns the number of audio channels in the sound sample.
	int			 NumChannels() const { return format.basic.numChannels; }

	//! Returns the total buffer size of the sound sample in bytes.
	int			 BufferSize() const { return totalBufferSize; }

	//! Returns true if the sound sample is compressed
	bool		 IsCompressed() const { return ( format.basic.formatTag != idWaveFile::FORMAT_PCM ); }

	//! Checks if the sound sample is the default sample.
	bool		 IsDefault() const { return timestamp == FILE_NOT_FOUND_TIMESTAMP; }

	//! Returns true if the sound sample has been successfully loaded.
	bool		 IsLoaded() const { return loaded; }

	//! Marks the sound sample to never be purged from memory.
	void		 SetNeverPurge() { neverPurge = true; }

	//! Returns whether the sound sample should never be purged during level loading.
	bool		 GetNeverPurge() const { return neverPurge; }

	//! Marks the sound sample as referenced during level loading.
	void		 SetLevelLoadReferenced() { levelLoadReferenced = true; }

	//! Resets the level load reference flag to false.
	void		 ResetLevelLoadReferenced() { levelLoadReferenced = false; }

	//! Returns whether the sound sample was referenced during level loading.
	bool		 GetLevelLoadReferenced() const { return levelLoadReferenced; }

	//! Returns the last played time of the sound sample.
	int			 GetLastPlayedTime() const { return lastPlayedTime; }

	//! Sets the last played time for the sound sample.
	void		 SetLastPlayedTime( int t ) { lastPlayedTime = t; }

	//! Returns the amplitude of the sound sample at the specified time in milliseconds
	float		 GetAmplitude( int timeMS ) const;

#if 0 // defined(AL_SOFT_buffer_samples)
	const char*		OpenALSoftChannelsName( ALenum chans ) const;

	const char*		OpenALSoftTypeName( ALenum type ) const;

	ALsizei			FramesToBytes( ALsizei size, ALenum channels, ALenum type ) const;
	ALsizei			BytesToFrames( ALsizei size, ALenum channels, ALenum type ) const;

	/* Retrieves a compatible buffer format given the channel configuration and
	 * sample type. If an alIsBufferFormatSupportedSOFT-compatible function is
	 * provided, it will be called to find the closest-matching format from
	 * AL_SOFT_buffer_samples. Returns AL_NONE (0) if no supported format can be
	 * found. */
	ALenum			GetOpenALSoftFormat( ALenum channels, ALenum type ) const;
#endif

	//! Returns the OpenAL buffer format enum based on the audio format and number of channels.
	ALenum GetOpenALBufferFormat() const;

	//! Creates an OpenAL buffer for the sound sample
	void   CreateOpenALBuffer();

protected:
	friend class idSoundHardware_OpenAL;
	friend class idSoundVoice_OpenAL;

	//! Destroys the idSoundSample_OpenAL object and frees its allocated data.
	~idSoundSample_OpenAL();

	//! Loads a WAV audio file and prepares it for playback.
	bool LoadWav( const idStr& name );

	//! Loads amplitude data from a file into the sound sample
	bool LoadAmplitude( const idStr& name );

	//! Writes all samples to a file with the specified name.
	void WriteAllSamples( const idStr& sampleName );

	//! Loads a generated sound sample from a specified file path.
	bool LoadGeneratedSample( const idStr& name );

	//! Writes the generated sound sample data to the specified file.
	void WriteGeneratedSample( idFile* fileOut );

	struct MS_ADPCM_decodeState_t {
		uint8  hPredictor;
		int16  coef1;
		int16  coef2;

		uint16 iDelta;
		int16  iSamp1;
		int16  iSamp2;
	};

	//! Decodes a single nybble of MS ADPCM data and returns the resulting audio sample
	int32 MS_ADPCM_nibble( MS_ADPCM_decodeState_t* state, int8 nybble );

	//! Decodes MS ADPCM audio data into PCM format
	int	  MS_ADPCM_decode( uint8** audio_buf, uint32* audio_len );

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

	// OpenAL buffer that contains all buffers
	ALuint							  openalBuffer;

	int								  playBegin;
	int								  playLength;

	idWaveFile::waveFmt_t			  format;

	idList<byte, TAG_AMPLITUDE>		  amplitude;
};

/*!
	\class idSoundSample
	\brief Manages sound sample data including loading, playback parameters, and resource management.
*/
class idSoundSample : public idSoundSample_OpenAL
{
public:
};

#endif
