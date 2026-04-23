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
#ifndef __AL_SOUNDVOICE_H__
#define __AL_SOUNDVOICE_H__

static const int MAX_QUEUED_BUFFERS = 3;

/*!
	\class idSoundVoice_OpenAL
	\brief Manages OpenAL audio playback for sound voices.

	This class provides the OpenAL-specific implementation for managing audio playback of sound voices. It handles the creation, configuration, and control of OpenAL audio sources, including setting
   position, gain, and pitch properties. The class supports lead-in and looping sound samples, and provides methods for starting, stopping, pausing, and resuming playback. It also handles buffer
   management and updates the hardware state to reflect current audio properties. The class is designed to work with OpenAL sound samples and can be reused with compatible samples to avoid unnecessary
   resource creation. Memory management is handled through the base class and OpenAL resource cleanup is performed through explicit destroy methods.

*/
class idSoundVoice_OpenAL : public idSoundVoice_Base
{
public:
	//! Initializes a new instance of the idSoundVoice_OpenAL class with default values.
	idSoundVoice_OpenAL();

	//! Destructor for the idSoundVoice_OpenAL class that cleans up internal resources.
	~idSoundVoice_OpenAL();

	//! Sets the position of the sound voice in 3D space for OpenAL audio rendering.
	void SetPosition( const idVec3& p )
	{
		idSoundVoice_Base::SetPosition( p );

		alSource3f( openalSource, AL_POSITION, -p.y, p.z, -p.x );
	}

	//! Sets the gain or volume level for the OpenAL sound voice.
	void SetGain( float gain )
	{
		idSoundVoice_Base::SetGain( gain );

		alSourcef( openalSource, AL_GAIN, ( gain ) < ( 1.0f ) ? ( gain ) : ( 1.0f ) );
	}

	//! Sets the pitch multiplier for the sound voice.
	void SetPitch( float p )
	{
		idSoundVoice_Base::SetPitch( p );

		alSourcef( openalSource, AL_PITCH, p );
	}

	//! Initializes a sound voice with lead-in and looping sound samples.
	void   Create( const idSoundSample* leadinSample, const idSoundSample* loopingSample );

	//! Start playing the sound at a specific time offset with given flags.
	void   Start( int offsetMS, int ssFlags );

	//! Stops the audio playback for this sound voice.
	void   Stop();

	//! Pauses the OpenAL sound source if it is active and not already paused.
	void   Pause();

	//! Resumes playback of the audio source if it is currently paused
	void   UnPause();

	//! Updates the sound voice hardware state with current position, volume, and pitch information.
	bool   Update();

	//! Returns the RMS amplitude of the most recently processed audio block for this sound voice.
	float  GetAmplitude();

	//! Returns true if the voice can be reused with the given sound sample.
	bool   CompatibleFormat( idSoundSample_OpenAL* s );

	//! Returns the sample rate of the audio voice.
	uint32 GetSampleRate() const { return sampleRate; }

	//! Handles the start of a sound buffer playback by preparing the next buffer for submission.
	void   OnBufferStart( idSoundSample_OpenAL* sample, int bufferNumber );

private:
	friend class idSoundHardware_OpenAL;

	//! Returns true when the sound voice is currently playing.
	bool				  IsPlaying();

	//! Flushes the audio buffers for the OpenAL source associated with this sound voice.
	void				  FlushSourceBuffers();

	//! Destroys the internal OpenAL hardware resource associated with this sound voice.
	void				  DestroyInternal();

	//! Restarts audio playback at the specified sample offset.
	int					  RestartAt( int offsetSamples );

	//! Submits a sound buffer to the OpenAL source for playback.
	int					  SubmitBuffer( idSoundSample_OpenAL* sample, int bufferNumber, int offset );

	//! Adjusts the voice frequency based on the new sample rate for the buffer.
	void				  SetSampleRate( uint32 newSampleRate, uint32 operationSet );

	// IXAudio2SourceVoice* 	pSourceVoice;
	bool				  triggered;
	ALuint				  openalSource;
	ALuint				  openalStreamingOffset;
	ALuint				  openalStreamingBuffer[3];
	ALuint				  lastopenalStreamingBuffer[3];

	idSoundSample_OpenAL* leadinSample;
	idSoundSample_OpenAL* loopingSample;

	// These are the fields from the sample format that matter to us for voice reuse
	uint16				  formatTag;
	uint16				  numChannels;

	uint32				  sourceVoiceRate;
	uint32				  sampleRate;

	bool				  hasVUMeter;
	bool				  paused;
};

/*!
	\class idSoundVoice
	\brief Manages the playback state and hardware interaction for a single sound instance.

*/
class idSoundVoice : public idSoundVoice_OpenAL
{
};

#endif
