/**
 * Copyright (C) 2021 George Kalmpokis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to
 * do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. As clarification, there
 * is no requirement that the copyright notice and permission be included in
 * binary distributions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <precompiled.h>
#include "../CinematicAudio.h"
#include <xaudio2.h>
#ifndef __CINEMATIC_AUDIO_XA2_H__
	#define __CINEMATIC_AUDIO_XA2_H__

/*!
	\class CinematicAudio_XAudio2
	\brief Provides XAudio2-based audio playback for cinematic content.
*/
class CinematicAudio_XAudio2 : public CinematicAudio
{
public:
	//! Initializes a new instance of the CinematicAudio_XAudio2 class.
	CinematicAudio_XAudio2();

	//! Initializes audio playback for cinematic content using the provided audio context.
	void InitAudio( void* audioContext );

	//! Submits audio data to the XAudio2 buffer and plays it
	void PlayAudio( uint8_t* data, int size );

	//! Stops and flushes the audio source voice if it exists.
	void ResetAudio();

	//! Stops and cleans up the audio source voice used for cinematic music playback.
	void ShutdownAudio();

private:
	WAVEFORMATEX		 voiceFormatcine = { 0 };
	IXAudio2SourceVoice* pMusicSourceVoice1;
	XAUDIO2_BUFFER		 Packet = { 0 };
};

#endif
