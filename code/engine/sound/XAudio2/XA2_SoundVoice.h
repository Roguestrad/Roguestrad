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
#ifndef __XA2_SOUNDVOICE_H__
#define __XA2_SOUNDVOICE_H__

static const int MAX_QUEUED_BUFFERS = 3;

/*!
	\class idSoundVoice_XAudio2
	\brief Manages XAudio2 audio playback for sound voices.

	This class provides a concrete implementation of sound voice management specifically for XAudio2 audio system. It handles the creation, initialization, and control of audio playback for sound
   samples, including lead-in and looping samples. The class manages the hardware resources through XAudio2 interfaces and provides methods for starting, stopping, pausing, and updating audio
   playback. It also handles buffer management and sample rate configuration. The class is designed to work with sound samples that have been prepared for XAudio2 playback and maintains state
   information about the current playback position, volume, and pitch. The implementation ensures proper cleanup of hardware resources when the sound voice is destroyed or flushed.

*/
class idSoundVoice_XAudio2 : public idSoundVoice_Base
{
public:
	//! Initializes a new instance of the idSoundVoice_XAudio2 class with default values.
	idSoundVoice_XAudio2();

	//! Destroys the XAudio2 sound voice and cleans up internal resources.
	~idSoundVoice_XAudio2();

	//! Initializes the sound voice with lead-in and looping sound samples for XAudio2 audio playback.
	void   Create( const idSoundSample* leadinSample, const idSoundSample* loopingSample );

	//! Starts playing a sound at a specified offset with given flags.
	void   Start( int offsetMS, int ssFlags );

	//! Stops the audio playback for the sound voice.
	void   Stop();

	//! Pauses the sound voice by stopping the source voice and setting the paused flag.
	void   Pause();

	//! Resumes audio playback for a sound voice that was previously paused.
	void   UnPause();

	//! Updates the sound voice hardware state with current position, volume, and pitch information.
	bool   Update();

	//! Returns the root mean square amplitude of the most recently processed audio block for this voice.
	float  GetAmplitude();

	//! Returns true if the voice can be reused for the given sound sample format
	bool   CompatibleFormat( idSoundSample_XAudio2* s );

	//! Returns the sample rate of the audio voice.
	uint32 GetSampleRate() const { return sampleRate; }

	//! Handles the start of a sound buffer by setting up the next buffer to play.
	void   OnBufferStart( idSoundSample_XAudio2* sample, int bufferNumber );

private:
	friend class idSoundHardware_XAudio2;

	//! Returns true when the audio source voice is currently playing audio buffers.
	bool				   IsPlaying();

	//! Flushes the source buffers of the XAudio2 voice after it has been stopped.
	void				   FlushSourceBuffers();

	//! Destroys the internal hardware resource for the sound voice.
	void				   DestroyInternal();

	//! Restarts sound playback at the specified sample offset.
	int					   RestartAt( int offsetSamples );

	//! Submits an audio buffer to the XAudio2 voice for playback
	int					   SubmitBuffer( idSoundSample_XAudio2* sample, int bufferNumber, int offset );

	//! Sets the sample rate for the audio voice and updates the filter and frequency ratio parameters accordingly.
	void				   SetSampleRate( uint32 newSampleRate, uint32 operationSet );

	IXAudio2SourceVoice*   pSourceVoice;
	idSoundSample_XAudio2* leadinSample;
	idSoundSample_XAudio2* loopingSample;

	// These are the fields from the sample format that matter to us for voice reuse
	uint16				   formatTag;
	uint16				   numChannels;

	uint32				   sourceVoiceRate;
	uint32				   sampleRate;

	bool				   hasVUMeter;
	bool				   paused;
};

/*
================================================
idSoundVoice
================================================
*/
class idSoundVoice : public idSoundVoice_XAudio2
{
};

#endif
