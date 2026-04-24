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
#ifndef __SWF_TEXTINSTANCE_H__
#define __SWF_TEXTINSTANCE_H__

struct subTimingWordData_t {
	//! Initializes a subTimingWordData_t object with default values.
	subTimingWordData_t()
	{
		startTime  = 0;
		forceBreak = false;
	}

	idStr phrase;
	int	  startTime;
	bool  forceBreak;
};

/*!
	\class idSWFTextInstance
	\brief Manages SWF text instances with various text rendering and subtitle functionality.

	Provides comprehensive handling of SWF text instances including initialization, rendering, and subtitle management. Supports features such as random text generation, paragraph text handling,
   stroke effects, drop shadows, and subtitle display states. The class maintains state for text content, character positioning, and rendering parameters while offering methods to query and modify
   these properties. It interfaces with SWF edit text for configuration and manages both static and dynamic text content with timing and display considerations.

*/
class idSWFTextInstance
{
public:
	//! Initializes a new instance of the idSWFTextInstance class.
	idSWFTextInstance();

	//! Destructor for the idSWFTextInstance class that clears and releases associated resources.
	~idSWFTextInstance();

	//! Initializes the SWF text instance with the provided edit text and SWF context.
	void				 Init( idSWFEditText* editText, idSWF* _swf );

	//! Returns the script object associated with this text instance
	idSWFScriptObject*	 GetScriptObject() { return &scriptObject; }

	//! Returns whether the text instance has a drop shadow effect enabled.
	bool				 GetHasDropShadow() { return useDropShadow; }

	//! Returns whether the text instance has a stroke effect enabled.
	bool				 HasStroke() { return useStroke; }

	//! Returns the stroke strength value of the text instance.
	float				 GetStrokeStrength() { return strokeStrength; }

	//! Returns the stroke weight value for the SWF text instance
	float				 GetStrokeWeight() { return strokeWeight; }

	//! Returns true if the text instance is currently generating random text.
	bool				 IsGeneratingRandomText() { return generatingText; }

	//! Initializes the random text generation process with the specified time duration.
	void				 StartRandomText( int time );

	//! Returns the current random text state for the SWF text instance at the given time
	idStr				 GetRandomText( int time );

	//! Initializes the text generation process for a SWF text instance.
	void				 StartParagraphText( int time );

	//! Returns the paragraph text for a given time, handling text generation and rendering delays.
	idStr				 GetParagraphText( int time );

	//! Checks if random text needs to be generated for the SWF text instance.
	bool				 NeedsGenerateRandomText() { return triggerGenerate; }

	//! Determines whether the text instance needs to play a sound.
	bool				 NeedsSoundPlayed();

	//! Clears the play sound flag for the SWF text instance.
	void				 ClearPlaySound() { needsSoundUpdate = false; }

	//! Returns the sound clip associated with this SWF text instance.
	idStr				 GetSoundClip() { return soundClip; }

	//! Sets whether the text instance should ignore color settings.
	void				 SetIgnoreColor( bool ignore ) { ignoreColor = ignore; }

	//! Sets the stroke information for the SWF text instance.
	void				 SetStrokeInfo( bool use, float strength = 0.75f, float weight = 1.75f );

	//! Calculates the maximum scroll value for a text instance based on the number of lines or the text content.
	int					 CalcMaxScroll( int numLines = -1 );

	//! Calculates and returns the number of lines needed to display the text within the given bounds
	int					 CalcNumLines();

	//! Sets the subtitle switching flag to false.
	void				 SwitchSubtitleText( int time );

	//! Updates the subtitle display state based on the provided time value.
	bool				 UpdateSubtitle( int time );

	//! Returns true if the text instance is a subtitle.
	bool				 IsSubtitle() { return isSubtitle; }

	//! Checks if the subtitle is currently being updated.
	bool				 IsUpdatingSubtitle() { return subUpdating; }

	//! Sets the end index for subtitle text rendering with a specified time.
	void				 SetSubEndIndex( int endChar, int time );

	//! Returns the index of the last word in the text instance.
	int					 GetLastWordIndex() { return subLastWordIndex; }

	//! Returns the index of the last word in the previous text line.
	int					 GetPrevLastWordIndex() { return subPrevLastWordIndex; }

	//! Updates the last word index and timing information for subtitle display based on the provided word count and time.
	void				 LastWordChanged( int wordCount, int time );

	//! Sets the starting index for sub-character rendering.
	void				 SetSubStartIndex( int value ) { subCharStartIndex = value; }

	//! Returns the ending index of a subtitle character range.
	int					 GetSubEndIndex() { return subCharEndIndex; }

	//! Returns the starting index for subtitle character processing.
	int					 GetSubStartIndex() { return subCharStartIndex; }

	//! Sets the sub next start index value for the SWF text instance.
	void				 SetSubNextStartIndex( int value );

	//! Returns the approximate subtitle break index based on the provided time.
	int					 GetApporoximateSubtitleBreak( int time );

	//! Returns whether the subtitle needs to be switched based on the current state.
	bool				 SubNeedsSwitch() { return subNeedsSwitch; }

	//! Returns the previous text content of the SWF text instance.
	idStr				 GetPreviousText() { return subtitleText.c_str(); }

	//! Marks the subtitle as complete and resets all subtitle-related state variables.
	void				 SubtitleComplete();

	//! Returns the sub-alignment value of the SWF text instance.
	int					 GetSubAlignment() { return subAlign; }

	//! Returns the speaker string associated with this SWF text instance.
	idStr				 GetSpeaker() { return subSpeaker.c_str(); }

	//! Cleans up subtitle-related data by resetting source ID, alignment, and text fields.
	void				 SubtitleCleanup();

	//! Returns the calculated length of the text content for this SWF text instance.
	float				 GetTextLength();

	//! Returns the starting character index of the input text.
	int					 GetInputStartChar() { return inputTextStartChar; }

	//! Sets the starting character position for input text.
	void				 SetInputStartCharacter( int c ) { inputTextStartChar = c; }

	//! Returns a pointer to the edit text associated with this text instance.
	const idSWFEditText* GetEditText() const { return editText; }

	//! Sets the text content of the SWF text instance and marks the length as not calculated.
	void				 SetText( idStr val )
	{
		text			 = val;
		lengthCalculated = false;
	}

	// Removing the private access control statement due to cl 214702
	// Apparently MS's C++ compiler supports the newer C++ standard, and GCC supports C++03
	// In the new C++ standard, nested members of a friend class have access to private/protected members of the class granting friendship
	// In C++03, nested members defined in a friend class do NOT have access to private/protected members of the class granting friendship

	idSWFEditText*						 editText;
	idSWF*								 swf;

	// this text instance's script object
	idSWFScriptObject					 scriptObject;

	idStr								 text;
	idStr								 randomtext;
	idStr								 variable;
	swfColorRGBA_t						 color;

	bool								 visible;
	bool								 tooltip;

	int									 selectionStart;
	int									 selectionEnd;
	bool								 ignoreColor;

	int									 scroll;
	int									 scrollTime;
	int									 maxscroll;
	int									 maxLines;
	float								 glyphScale;
	swfRect_t							 bounds;
	float								 linespacing;

	bool								 shiftHeld;
	int									 lastInputTime;

	bool								 useDropShadow;
	bool								 useStroke;

	float								 strokeStrength;
	float								 strokeWeight;

	int									 textLength;
	bool								 lengthCalculated;

	swfTextRenderMode_t					 renderMode;
	bool								 generatingText;
	int									 rndSpotsVisible;
	int									 rndSpacesVisible;
	int									 charMultiplier;
	int									 textSpotsVisible;
	int									 rndTime;
	int									 startRndTime;
	int									 prevReplaceIndex;
	bool								 triggerGenerate;
	int									 renderDelay;
	bool								 scrollUpdate;
	idStr								 soundClip;
	bool								 needsSoundUpdate;
	idList<int, TAG_SWF>				 indexArray;
	idRandom2							 rnd;

	// used for subtitles
	bool								 isSubtitle;
	int									 subLength;
	int									 subCharDisplayTime;
	int									 subAlign;
	bool								 subUpdating;
	int									 subCharStartIndex;
	int									 subNextStartIndex;
	int									 subCharEndIndex;
	int									 subDisplayTime;
	int									 subStartTime;
	int									 subSourceID;
	idStr								 subtitleText;
	bool								 subNeedsSwitch;
	bool								 subForceKillQueued;
	bool								 subForceKill;
	int									 subKillTimeDelay;
	int									 subSwitchTime;
	int									 subLastWordIndex;
	int									 subPrevLastWordIndex;
	idStr								 subSpeaker;
	bool								 subWaitClear;
	bool								 subInitialLine;

	// input text
	int									 inputTextStartChar;

	idList<subTimingWordData_t, TAG_SWF> subtitleTimingInfo;
};

/*!
	\class idSWFScriptObject_TextInstancePrototype
	\brief Text instance prototype for SWF script objects.

	This class serves as the prototype for all text instance script objects within the SWF system. It establishes the foundational structure and provides access to various text-related properties and
   functions. The class initializes text functions and variables during construction, setting up the necessary framework for text manipulation within the scripting environment. It inherits from
   idSWFScriptObject, indicating its role within the broader SWF scripting hierarchy.

*/
class idSWFScriptObject_TextInstancePrototype : public idSWFScriptObject
{
public:
	//! Initializes the text instance prototype by setting up text functions and variables.
	idSWFScriptObject_TextInstancePrototype();

	//----------------------------------
	// Native Script Functions
	//----------------------------------
#define SWF_TEXT_FUNCTION_DECLARE( x )                                                    \
	class idSWFScriptFunction_##x : public idSWFScriptFunction_RefCounted                 \
	{                                                                                     \
	public:                                                                               \
		void AddRef()                                                                     \
		{                                                                                 \
		}                                                                                 \
		void Release()                                                                    \
		{                                                                                 \
		}                                                                                 \
		idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms ); \
	} scriptFunction_##x;

	//! Handles key input events for a text instance, updating selection and text content based on keyboard input.
	SWF_TEXT_FUNCTION_DECLARE( onKey );

	//! Handles the onChar event for a text instance, processing character input and updating the text content.
	SWF_TEXT_FUNCTION_DECLARE( onChar );

	//! Initializes text generation parameters for a text instance object.
	SWF_TEXT_FUNCTION_DECLARE( generateRnd );

	//! Calculates and returns the number of lines in the text instance.
	SWF_TEXT_FUNCTION_DECLARE( calcNumLines );

	SWF_NATIVE_VAR_DECLARE( text );
	SWF_NATIVE_VAR_DECLARE( autoSize );
	SWF_NATIVE_VAR_DECLARE( dropShadow );
	SWF_NATIVE_VAR_DECLARE( _stroke );
	SWF_NATIVE_VAR_DECLARE( _strokeStrength );
	SWF_NATIVE_VAR_DECLARE( _strokeWeight );
	SWF_NATIVE_VAR_DECLARE( variable );
	SWF_NATIVE_VAR_DECLARE( _alpha );
	SWF_NATIVE_VAR_DECLARE( textColor );
	SWF_NATIVE_VAR_DECLARE( _visible );
	SWF_NATIVE_VAR_DECLARE( scroll );
	SWF_NATIVE_VAR_DECLARE( maxscroll );
	SWF_NATIVE_VAR_DECLARE( selectionStart );
	SWF_NATIVE_VAR_DECLARE( selectionEnd );
	SWF_NATIVE_VAR_DECLARE( isTooltip );
	SWF_NATIVE_VAR_DECLARE( mode );
	SWF_NATIVE_VAR_DECLARE( delay );
	SWF_NATIVE_VAR_DECLARE( renderSound );
	SWF_NATIVE_VAR_DECLARE( updateScroll );
	SWF_NATIVE_VAR_DECLARE( subtitle );
	SWF_NATIVE_VAR_DECLARE( subtitleAlign );
	SWF_NATIVE_VAR_DECLARE( subtitleSourceID );
	SWF_NATIVE_VAR_DECLARE( subtitleSpeaker );

	SWF_NATIVE_VAR_DECLARE_READONLY( _textLength );

	//! Checks if the subtitle source ID matches the provided ID and handles subtitle completion logic.
	SWF_TEXT_FUNCTION_DECLARE( subtitleSourceCheck );

	//! Initializes subtitle display parameters and prepares the text instance for subtitle rendering.
	SWF_TEXT_FUNCTION_DECLARE( subtitleStart );

	//! Sets the subtitle length property of a text instance object.
	SWF_TEXT_FUNCTION_DECLARE( subtitleLength );

	//! Kills the subtitle by setting a flag to force kill the queued subtitle.
	SWF_TEXT_FUNCTION_DECLARE( killSubtitle );

	//! Sets the subtitle kill flag and resets the kill time delay for the text instance.
	SWF_TEXT_FUNCTION_DECLARE( forceKillSubtitle );

	//! Returns the last line of text from a text instance object.
	SWF_TEXT_FUNCTION_DECLARE( subLastLine );

	//! Adds subtitle information to the text instance with specified phrase, start time, and break flag.
	SWF_TEXT_FUNCTION_DECLARE( addSubtitleInfo );

	//! Terminates a subtitle by completing and cleaning up the subtitle process.
	SWF_TEXT_FUNCTION_DECLARE( terminateSubtitle );

	//! Clears the timing information associated with the text instance.
	SWF_TEXT_FUNCTION_DECLARE( clearTimingInfo );
};

#endif // !__SWF_TEXTINSTANCE_H__
