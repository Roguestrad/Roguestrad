/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

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

#ifndef __SND_LOCAL_H__
#define __SND_LOCAL_H__

#include "WaveFile.h"

// Maximum number of voices we can have allocated
#define MAX_HARDWARE_VOICES	   48

// A single voice can play multiple channels (up to 5.1, but most commonly stereo)
// This is the maximum number of channels which can play simultaneously
// This is limited primarily by seeking on the optical drive, secondarily by memory consumption, and tertiarily by CPU time spent mixing
#define MAX_HARDWARE_CHANNELS  64

// We may need up to 3 buffers for each hardware voice if they are all long sounds
#define MAX_SOUND_BUFFERS	   ( MAX_HARDWARE_VOICES * 3 )

// Maximum number of channels in a sound sample
#define MAX_CHANNELS_PER_VOICE 8

//! Converts milliseconds to audio sample count using the given sample rate.
ID_INLINE_EXTERN uint32 MsecToSamples( uint32 msec, uint32 sampleRate )
{
	return ( msec * ( sampleRate / 100 ) ) / 10;
}

//! Converts a number of audio samples to milliseconds based on the sample rate.
ID_INLINE_EXTERN uint32 SamplesToMsec( uint32 samples, uint32 sampleRate )
{
	return sampleRate < 100 ? 0 : ( samples * 10 ) / ( sampleRate / 100 );
}

//! Converts a decibel value to its linear equivalent.
ID_INLINE_EXTERN float DBtoLinear( float db )
{
	return idMath::Pow( 2.0f, db * ( 1.0f / 6.0f ) );
}

//! Converts a linear volume value to decibels.
ID_INLINE_EXTERN float LinearToDB( float linear )
{
	return ( linear > 0.0f ) ? ( idMath::Log( linear ) * ( 6.0f / 0.693147181f ) ) : -999.0f;
}

// demo sound commands
typedef enum {
	SCMD_STATE, // followed by a load game state
	SCMD_PLACE_LISTENER,
	SCMD_ALLOC_EMITTER,
	SCMD_FREE,
	SCMD_UPDATE,
	SCMD_START,
	SCMD_MODIFY,
	SCMD_STOP,
	SCMD_FADE,
	SCMD_CACHESOUNDSHADER,
} soundDemoCommand_t;

#include "SoundVoice.h"

#if defined( USE_OPENAL )

	// #define AL_ALEXT_PROTOTYPES

	// SRS - Added check on OSX for OpenAL Soft headers vs macOS SDK headers
	#if defined( __APPLE__ ) && !defined( USE_OPENAL_SOFT_INCLUDES )
		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>
	#else
		#include <AL/al.h>
		#include <AL/alc.h>
		#include <AL/alext.h>
	#endif

	#include "OpenAL/AL_SoundSample.h"
	#include "OpenAL/AL_SoundVoice.h"
	#include "OpenAL/AL_SoundHardware.h"

ID_INLINE_EXTERN ALenum CheckALErrors_( const char* filename, int line )
{
	ALenum err = alGetError();
	if( err != AL_NO_ERROR ) { idLib::Printf( "OpenAL Error: %s (0x%x), @ %s %d\n", alGetString( err ), err, filename, line ); }
	return err;
}
	#define CheckALErrors() CheckALErrors_( __FILE__, __LINE__ )

ID_INLINE_EXTERN ALCenum CheckALCErrors_( ALCdevice* device, const char* filename, int linenum )
{
	ALCenum err = alcGetError( device );
	if( err != ALC_NO_ERROR ) { idLib::Printf( "ALC Error: %s (0x%x), @ %s %d\n", alcGetString( device, err ), err, filename, linenum ); }
	return err;
}
	#define CheckALCErrors( x ) CheckALCErrors_( ( x ), __FILE__, __LINE__ )

#elif defined( _MSC_VER ) // DG: stub out xaudio for MinGW etc

	#define OPERATION_SET 1

	// RB: not available on Windows 8 SDK
	#if defined( USE_WINRT ) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
		#include <mmdeviceapi.h>
		#include <initguid.h> // For the pkey defines to be properly instantiated.
		#include <propkeydef.h>
		#include "functiondiscoverykeys_devpkey.h"
		#include <string>
		#include <vector>

DEFINE_PROPERTYKEY( PKEY_AudioEndpoint_Path, 0x9c119480, 0xddc2, 0x4954, 0xa1, 0x50, 0x5b, 0xd2, 0x40, 0xd4, 0x54, 0xad, 1 );

		#pragma comment( lib, "xaudio2.lib" )

struct AudioDevice {
	std::wstring name;
	std::wstring id;
};
	#else
		#include <dxsdkver.h>
	#endif
	// RB end

	#include <xaudio2.h>
	#include <xaudio2fx.h>
	#include <X3DAudio.h>

	// RB: not available on Windows 8 SDK
	#if !defined( USE_WINRT ) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
		#include <xma2defs.h>
	#endif
	// RB end

	#include "XAudio2/XA2_SoundSample.h"
	#include "XAudio2/XA2_SoundVoice.h"
	#include "XAudio2/XA2_SoundHardware.h"

#else // not _MSC_VER => MinGW, GCC, ...
	// just a stub for now
	#include "stub/SoundStub.h"
#endif // _MSC_VER ; DG end

#include "../libs/oggvorbis/ogg/ogg.h"
#include "../libs/oggvorbis/vorbis/vorbisfile.h"

/*!
	\class idSoundDecoder_Vorbis
	\brief Provides decoding functionality for Vorbis audio files.

	This class implements a sound decoder specifically designed for Vorbis audio format files. It offers functionality to open, seek, and read audio data from Vorbis files while providing format
   information. The decoder manages its own resources and is intended to be used as a component for handling audio playback in a sound system. The class supports basic audio stream operations
   including end-of-stream detection and sample-level positioning within the audio file.

*/
class idSoundDecoder_Vorbis
{
public:
	//! Initializes a new instance of the idSoundDecoder_Vorbis class with all member pointers set to nullptr.
	idSoundDecoder_Vorbis();

	//! Destroys the Vorbis sound decoder and releases all associated resources.
	~idSoundDecoder_Vorbis();

	//! Opens a Vorbis audio file for decoding.
	virtual bool	Open( const char* fileName );

	//! Checks if the Vorbis audio decoder has reached the end of the sound stream.
	virtual bool	IsEOS();

	//! Seeks to a specific sample position in the Vorbis audio file.
	virtual void	Seek( int samplePos );

	//! Reads audio data from a Vorbis file into a provided buffer.
	virtual int		Read( void* buffer, int bufferSize );

	//! Returns the total size in bytes of the audio data decoded by this Vorbis decoder
	virtual int64_t Size();

	//! Returns the total number of compressed audio samples in the Vorbis audio file.
	virtual int64_t CompressedSize();

	//! Retrieves the audio format information from the Vorbis file and populates the provided format structure.
	virtual void	GetFormat( idWaveFile::waveFmt_t& format );

private:
	idSoundSample*	sample;
	OggVorbis_File* vorbisFile;
	idFile*			mhmmio;
};

//------------------------
// Listener data
//------------------------
struct listener_t {
	idMat3 axis; // orientation of the listener
	idVec3 pos;	 // position in meters
	int	   id;	 // the entity number, used to detect when a sound is local
	int	   area; // area number the listener is in
};

/*!
	\class idSoundFade
	\brief Manages sound fade effects with volume transitions over time.
*/
class idSoundFade
{
public:
	int	  fadeStartTime;
	int	  fadeEndTime;
	float fadeStartVolume;
	float fadeEndVolume;

public:
	//! Initializes a sound fade object and clears its state.
	idSoundFade() { Clear(); }

	//! Clears all fade timing and volume parameters.
	void  Clear();

	//! Sets the volume for the sound fade effect.
	void  SetVolume( float to );

	//! Configures the sound fade effect to transition to a target volume over a specified duration.
	void  Fade( float to, int length, int soundTime );

	//! Returns the volume for a given sound time based on fade start and end parameters
	float GetVolume( int soundTime ) const;
};

/*!
	\class idSoundChannel
	\brief Manages the state and playback of individual sound channels.

	This class represents a single sound channel that handles the lifecycle and properties of audio playback. It manages the channel's mute state, completion checking, volume updates, and hardware
   voice synchronization. The channel supports looping playback and can be muted to stop audio output. The class maintains internal state for tracking playback progress and volume adjustments. The
   implementation handles the interaction with hardware audio resources for actual sound output.

*/
class idSoundChannel
{
public:
	//! Returns true if the sound channel can be muted, which is always the case for this implementation.
	bool					   CanMute() const;

	//! Mutes the sound channel by freeing the hardware voice if it exists.
	void					   Mute();

	//! Checks if a sound channel has completed playback based on the current time and end time.
	bool					   CheckForCompletion( int currentTime );

	//! Updates the volume of a sound channel based on various factors including fade, attenuation, and sound class.
	void					   UpdateVolume( int currentTime );

	//! Updates the hardware audio voice state for a sound channel based on current parameters and volume settings.
	void					   UpdateHardware( float volumeAdd, int currentTime );

	//! Returns true if the sound channel is marked as looping.
	bool					   IsLooping() const;

	class idSoundEmitterLocal* emitter;

	int						   startTime;
	int						   endTime;
	int						   logicalChannel;
	bool					   allowSlow;

	soundShaderParms_t		   parms; // combines shader parms and per-channel overrides
	const idSoundShader*	   soundShader;
	idSoundSample*			   leadinSample;
	idSoundSample*			   loopingSample;
	idSoundFade				   volumeFade;

	float					   volumeDB;		 // last volume at which this channel will play (calculated in UpdateVolume)
	float					   currentAmplitude; // current amplitude on the hardware voice

	// hardwareVoice will be freed and NULL'd when a sound is out of range,
	// and reallocated when it comes back in range
	idSoundVoice*			   hardwareVoice;

	//! Constructs a new sound channel with default values.
	idSoundChannel();
	~idSoundChannel();
};

// Maximum number of SoundChannels for a single SoundEmitter.
// This is probably excessive...
const int MAX_CHANNELS_PER_EMITTER = 16;

/*!
	\class idSoundWorldLocal
	\brief Manages sound emulation and spatialization for a virtual sound world.

	The idSoundWorldLocal class provides comprehensive sound management capabilities for a virtual sound world, handling sound emitter allocation, spatialization through portal tracing, and channel
   management. It maintains the state of all active sounds and emitters, supports sound fading, pause/resume functionality, and handles save/load operations for sound state. The class implements
   spatialized sound positioning by tracing through portal areas to determine optimal sound origins, taking into account distance, portal occlusion, and recursion depth limits. Sound emitters are
   allocated and managed through dedicated methods, with support for direct shader playback and environment-specific sound adjustments such as slow-motion and environmental suit effects. The class
   updates all active sound emitters and manages hardware audio channel allocation for efficient sound playback.

*/
class idSoundWorldLocal : public idSoundWorld
{
public:
	//! Initializes a new instance of the idSoundWorldLocal class.
	idSoundWorldLocal();

	//! Destructor for the idSoundWorldLocal class that cleans up sound emitters and channels.
	virtual ~idSoundWorldLocal();

	//! Clears all sound emitters and resets the sound world state.
	virtual void			ClearAllSoundEmitters();

	//! Stops all playing sounds by resetting all sound emitters.
	virtual void			StopAllSounds();

	//! Allocates and initializes a new sound emitter for playing sounds in the world
	virtual idSoundEmitter* AllocSoundEmitter();

	//! Returns a sound emitter for the specified index, or NULL if the index is invalid.
	virtual idSoundEmitter* EmitterForIndex( int index );

	//! Returns the current shake amplitude for the sound world.
	virtual float			CurrentShakeAmplitude();

	//! Places the sound listener at the specified origin and axis with the given listener ID.
	virtual void			PlaceListener( const idVec3& origin, const idMat3& axis, const int listenerId );

	//! Fades all sounds with a specified sound class to a target volume over a given time period.
	virtual void			FadeSoundClasses( const int soundClass, const float to, const float over );

	//! Plays a sound shader directly by name, returning the channel it was played on or 0 if failed
	virtual int				PlayShaderDirectly( const char* name, int channel = -1 );

	//! Advances the sound world time when cinematics are skipped.
	virtual void			Skip( int time );

	//! Pauses the sound world, stopping playback of non-mutable sounds while preserving the pause state.
	virtual void			Pause();

	//! Resumes sound playback if the sound world is currently paused.
	virtual void			UnPause();

	//! Returns whether the sound world is currently paused
	virtual bool			IsPaused() { return isPaused; }

	//! Returns the current sound time in the sound world, accounting for pause states.
	virtual int				GetSoundTime();

	//! Writes the current sound world state to a save game file.
	virtual void			WriteToSaveGame( idFile* savefile );

	//! Restores the sound world state from a save game file.
	virtual void			ReadFromSaveGame( idFile* savefile );

	//! Sets the slow-motion speed multiplier for the sound world.
	virtual void			SetSlowmoSpeed( float speed );

	//! Sets the environmental suit active state for the sound world.
	virtual void			SetEnviroSuit( bool active );

	//! Updates all sound emitters and manages hardware audio channel allocation.
	void					Update();

	//! Notifies all sound emitters to reload sound data when a sound declaration is reloaded.
	void					OnReloadSound( const idDecl* decl );

	//! Allocates and returns a sound channel from the sound world's channel allocator.
	idSoundChannel*			AllocSoundChannel();

	//! Frees the specified sound channel by muting it and returning it to the allocator
	void					FreeSoundChannel( idSoundChannel* );

public:
	// even though all these variables are public, nobody outside the sound system includes SoundWorld_local.h
	// so this is equivalent to making it private and friending all the other classes in the sound system

	idSoundFade								volumeFade; // master volume knob for the entire world
	idSoundFade								soundClassFade[SOUND_MAX_CLASSES];

	idRenderWorld*							renderWorld; // for debug visualization and light amplitude sampling

	float									currentCushionDB; // channels at or below this level will be faded to 0
	float									shakeAmp;		  // last calculated shake amplitude

	listener_t								listener;
	idList<idSoundEmitterLocal*, TAG_AUDIO> emitters;

	idSoundEmitter*							localSound; // for PlayShaderDirectly()

	idBlockAlloc<idSoundEmitterLocal, 16>	emitterAllocator;
	idBlockAlloc<idSoundChannel, 16>		channelAllocator;

	idSoundFade								pauseFade;
	int										pausedTime;
	int										accumulatedPauseTime;
	bool									isPaused;

	float									slowmoSpeed;
	bool									enviroSuitActive;

public:
	struct soundPortalTrace_t {
		int						  portalArea;
		const soundPortalTrace_t* prevStack;
	};

	/*!
		\brief Resolves the spatialized origin of a sound emitter by tracing through portal areas to determine the closest accessible point

		This function recursively traces sound propagation through portal areas in a sound world to find the optimal spatialized origin for a sound emitter. It considers the distance to the listener,
	   portal occlusion, and prevents infinite recursion by limiting the portal trace depth. The function updates the sound emitter's spatialized distance and origin when a closer accessible point is
	   found.

		\param stackDepth Current depth of the portal trace recursion
		\param prevStack Pointer to the previous portal trace stack entry
		\param soundArea Area where the sound originates
		\param dist Distance traveled through portals so far
		\param soundOrigin Original position of the sound source
		\param def Pointer to the sound emitter being processed
	*/
	void ResolveOrigin( const int stackDepth, const soundPortalTrace_t* prevStack, const int soundArea, const float dist, const idVec3& soundOrigin, idSoundEmitterLocal* def );
};

/*!
	\class idSoundEmitterLocal
	\brief Manages sound playback and audio properties for a single sound emitter in the audio system.

	The idSoundEmitterLocal class handles the lifecycle and operation of individual sound emitters within the audio system. It provides methods to start, stop, modify, and fade sounds on specific
   channels, as well as to update audio properties based on time and listener position. The class supports sound diversity, parameter overrides, and proper cleanup of audio channels. It integrates
   with a sound world to manage playback and handles event-based updates like shader reloads. Initialization requires an index and sound world reference, and the emitter can be marked for freeing or
   reset to its initial state. The implementation ensures efficient channel management, prevents duplicate sound starts, and handles looping and lead-in samples appropriately.

*/
class idSoundEmitterLocal : public idSoundEmitter
{
public:
	//! Marks the sound emitter for freeing, optionally resetting it immediately.
	virtual void											Free( bool immediate );

	//! Resets the sound emitter by freeing its channels and reinitializing it.
	virtual void											Reset();

	//! Updates the sound emitter's origin, listener ID, and sound parameters.
	virtual void											UpdateEmitter( const idVec3& origin, int listenerId, const soundShaderParms_t* parms );

	/*!
		\brief Starts playing a sound from a shader on the specified channel with optional diversity and shader flags

		The function begins playback of a sound defined by the provided shader on the specified channel. It handles various sound parameters including diversity for randomization, shader flags for
	   special behavior, and a flag to allow slow sound processing. The function performs several checks to prevent duplicate sounds, manages channel allocation, and handles looping and lead-in
	   samples. It returns the length of the sound in milliseconds. The function ensures that sounds are not started multiple times in the same frame and properly manages channel overrides for sounds
	   already playing on the same logical channel. For looping sounds, it sets a random start offset to prevent synchronization issues.

		\param shader The sound shader defining the sound to play
		\param channel The logical channel to play the sound on
		\param diversity A value used to randomize which entry from the shader is selected
		\param shaderFlags Additional flags to modify the behavior of the sound
		\param allowSlow Whether to allow slow sound processing if hardware resources are limited
		\return The length of the started sound in milliseconds
	*/
	virtual int												StartSound( const idSoundShader* shader, const s_channelType channel, float diversity = 0, int shaderFlags = 0, bool allowSlow = true );

	//! Modifies sound parameters for a specific channel or all channels on the sound emitter.
	virtual void											ModifySound( const s_channelType channel, const soundShaderParms_t* parms );

	//! Stops sound playback on the specified channel or all channels if SCHANNEL_ANY is passed.
	virtual void											StopSound( const s_channelType channel );

	//! Fades the volume of sounds played on the specified channel over a given time period.
	virtual void											FadeSound( const s_channelType channel, float to, float over );

	//! Returns true if any sounds are currently playing on the specified channel, or any channel if SCHANNEL_ANY is specified.
	virtual bool											CurrentlyPlaying( const s_channelType channel = SCHANNEL_ANY ) const;

	//! Returns the current amplitude of the sound emitter.
	virtual float											CurrentAmplitude();

	//! Returns the index of this sound emitter within the sound world.
	virtual int												Index() const;

	//! Initializes a sound emitter with the given index and sound world.
	void													Init( int i, idSoundWorldLocal* sw );

	//! Checks if the sound emitter has completed playback and should be freed.
	bool													CheckForCompletion( int currentTime );

	//! Combines base and override sound parameters, with override values taking precedence.
	void													OverrideParms( const soundShaderParms_t* base, const soundShaderParms_t* over, soundShaderParms_t* out );

	//! Updates the sound emitter's audio properties and channel volumes based on the current time and listener position.
	void													Update( int currentTime );

	//! Handles sound shader reload events by restarting the currently playing sound if it matches the updated declaration.
	void													OnReloadSound( const idDecl* decl );

	//----------------------------------------------

	idSoundWorldLocal*										soundWorld; // the world that holds this emitter

	int														index;	 // in world emitter list
	bool													canFree; // if true, this emitter can be canFree (once channels.Num() == 0)

	// a single soundEmitter can have many channels playing from the same point
	idStaticList<idSoundChannel*, MAX_CHANNELS_PER_EMITTER> channels;

	//----- set by UpdateEmitter -----
	idVec3													origin;
	soundShaderParms_t										parms;
	int														emitterId; // sounds will be full volume when emitterId == listenerId

	//----- set by Update -----
	int														lastValidPortalArea;
	float													directDistance;
	float													spatializedDistance;
	idVec3													spatializedOrigin;

	//! Constructs a sound emitter object.
	idSoundEmitterLocal();

	//! Destructor for the idSoundEmitterLocal class that ensures all audio channels are properly cleaned up.
	virtual ~idSoundEmitterLocal();
};

/*!
	\class idSoundSystemLocal
	\brief Manages sound system initialization, playback, and resource management for audio rendering.

	The idSoundSystemLocal class serves as the primary interface for sound system operations, handling initialization, shutdown, and management of sound worlds, voices, and samples. It coordinates
   audio rendering with the graphics system through sound worlds and provides mechanisms for loading, preloading, and managing sound assets. The class supports muting, stopping all sounds, and
   restarting the sound system while maintaining synchronization between the audio hardware and the game state. It also handles stream buffer management for audio processing and provides access to
   underlying audio APIs like XAudio2 and OpenAL.

*/
class idSoundSystemLocal : public idSoundSystem
{
public:
	//! Initializes the sound system hardware and sets up command bindings.
	virtual void		  Init();

	//! Shuts down the sound system by cleaning up hardware, stream buffers, and sample data.
	virtual void		  Shutdown();

	//! Creates and returns a new sound world associated with the specified render world
	virtual idSoundWorld* AllocSoundWorld( idRenderWorld* rw );

	//! Frees the memory associated with a sound world object
	virtual void		  FreeSoundWorld( idSoundWorld* sw );

	//! Sets the current sound world for playback
	virtual void		  SetPlayingSoundWorld( idSoundWorld* soundWorld );

	//! Returns the currently active sound world being used for playback.
	virtual idSoundWorld* GetPlayingSoundWorld();

	//! Updates the sound system and sends current sound world information to the sound hardware.
	virtual void		  Render();

	//! Mutes or unmutes the background music based on the provided boolean parameter.
	virtual void		  MuteBackgroundMusic( bool mute ) { musicMuted = mute; }

	//! Sets the mute state of the sound system.
	virtual void		  SetMute( bool mute ) { muted = mute; }

	//! Returns whether the sound system is currently muted.
	virtual bool		  IsMuted() { return muted; }

	//! Notifies all sound worlds to reload the specified sound declaration.
	virtual void		  OnReloadSound( const idDecl* sound );

	//! Stops all sounds in all sound worlds and updates the hardware state.
	virtual void		  StopAllSounds();

	//! Initializes or resets the stream buffer contexts for audio processing.
	virtual void		  InitStreamBuffers();

	//! Clears all stream buffer contexts and unlocks the stream buffer mutex.
	virtual void		  FreeStreamBuffers();

	//! Returns the XAudio2 interface pointer used by the sound system
	virtual void*		  GetIXAudio2() const;

	//! Returns the OpenAL device handle used by the sound system
	virtual void*		  GetOpenALDevice() const;

	//! Returns cinematic image data for the specified time in milliseconds
	virtual cinData_t	  ImageForTime( const int milliseconds, const bool waveform );

	//! Prepares the sound system for a new level load by freeing unreferenced sound data.
	virtual void		  BeginLevelLoad();

	//! Marks the end of a level load operation and prepares the sound system to free unreferenced media.
	virtual void		  EndLevelLoad();

	//! Prints memory usage information for sound assets
	virtual void		  PrintMemInfo( MemInfo_t* mi );

	//! Stops all sound voices that are using the specified sample
	void				  StopVoicesWithSample( const idSoundSample* const sample );

	//! Restarts the sound system by muting all channels and reinitializing the sound hardware.
	void				  Restart();

	//! Marks the sound system as needing a restart.
	void				  SetNeedsRestart() { needsRestart = true; }

	//! Returns the current sound system time in milliseconds
	int					  SoundTime() const;

	//! Allocates a voice for playing sound with specified leadin and looping samples, returning NULL if no voices are available.
	idSoundVoice*		  AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample );

	//! Frees the hardware resources associated with a sound voice.
	void				  FreeVoice( idSoundVoice* );

	//! Loads a sound sample by name, returning a pointer to the sample object.
	idSoundSample*		  LoadSample( const char* name );

	//! Preloads sound samples specified in the preload manifest.
	virtual void		  Preload( idPreloadManifest& preload );

	struct bufferContext_t {
		//! Initializes a bufferContext_t object with default values.
		bufferContext_t() :
			voice( NULL ),
			sample( NULL ),
			bufferNumber( 0 )
		{
		}

#if defined( USE_OPENAL )
		idSoundVoice_OpenAL*  voice;
		idSoundSample_OpenAL* sample;
#elif defined( _MSC_VER ) // XAudio backend
		// DG: because the inheritance is kinda strange (idSoundVoice is derived
		// from idSoundVoice_XAudio2), casting the latter to the former isn't possible
		// so we need this ugly #ifdef ..
		idSoundVoice_XAudio2*  voice;
		idSoundSample_XAudio2* sample;
#else					  // not _MSC_VER
						  // from stub or something..
		idSoundVoice*  voice;
		idSoundSample* sample;
#endif					  // _MSC_VER ; DG end

		int bufferNumber;
	};

	//! Retrieves a stream buffer context from the free pool, returning NULL if none are available.
	bufferContext_t*								  ObtainStreamBufferContext();

	//! Releases a stream buffer back to the free pool
	void											  ReleaseStreamBufferContext( bufferContext_t* p );

	idSysMutex										  streamBufferMutex;
	idStaticList<bufferContext_t*, MAX_SOUND_BUFFERS> freeStreamBufferContexts;
	idStaticList<bufferContext_t*, MAX_SOUND_BUFFERS> activeStreamBufferContexts;
	idStaticList<bufferContext_t, MAX_SOUND_BUFFERS>  bufferContexts;

	idSoundWorldLocal*								  currentSoundWorld;
	idStaticList<idSoundWorldLocal*, 32>			  soundWorlds;

	idList<idSoundSample*, TAG_AUDIO>				  samples;
	idHashIndex										  sampleHash;

	idSoundHardware									  hardware;

	idRandom2										  random;

	int												  soundTime;
	bool											  muted;
	bool											  musicMuted;
	bool											  needsRestart;

	bool											  insideLevelLoad;

	//! Initializes a new instance of the idSoundSystemLocal class with default values.
	idSoundSystemLocal() :
		currentSoundWorld( NULL ),
		soundTime( 0 ),
		muted( false ),
		musicMuted( false ),
		needsRestart( false )
	{
	}
};

extern idSoundSystemLocal soundSystemLocal;

#endif /* !__SND_LOCAL_H__ */
