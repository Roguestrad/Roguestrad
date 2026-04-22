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

#ifndef __DECLPDA_H__
#define __DECLPDA_H__

/*!
	\class idDeclEmail
	\brief Manages email declaration data with parsing and retrieval capabilities.

	The idDeclEmail class extends idDecl to handle email-related data declarations. It provides functionality to parse email text into structured components such as sender, recipient, subject, date,
   and body. The class supports loading email definitions from text, retrieving individual components, and managing the associated data lifecycle. Parsing extracts standard email fields while
   maintaining a clean interface for accessing the parsed information. The implementation includes placeholder methods for printing and listing functionality, indicating that full operational
   capabilities may be under development. Memory management is handled through the base idDecl class, with explicit freeing of parsed data when necessary.

*/
class idDeclEmail : public idDecl
{
public:
	//! Constructs a new instance of the idDeclEmail class.
	idDeclEmail() { }

	//! Returns the size in bytes of the idDeclEmail class
	virtual size_t		Size() const;

	//! Returns the default definition string for an email declaration.
	virtual const char* DefaultDefinition() const;

	//! Parses email declaration text and extracts email components like subject, to, from, date, and text.
	virtual bool		Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees the data associated with the email declaration.
	virtual void		FreeData();

	//! Prints a placeholder message indicating the implementation is incomplete.
	virtual void		Print() const;

	//! Prints a message indicating that the implementation is incomplete.
	virtual void		List() const;

	//! Returns the sender address of the email declaration.
	const char*			GetFrom() const { return from; }

	//! Returns the body text of the email declaration.
	const char*			GetBody() const { return text; }

	//! Returns the subject string of the email declaration.
	const char*			GetSubject() const { return subject; }

	//! Returns the date associated with the email declaration.
	const char*			GetDate() const { return date; }

	//! Returns the recipient address of the email declaration.
	const char*			GetTo() const { return to; }

private:
	idStr text;
	idStr subject;
	idStr date;
	idStr to;
	idStr from;
};

/*!
	\class idDeclVideo
	\brief Video declaration class for managing video assets and their associated data.

	This class represents a video declaration that manages video assets and their associated data including materials, sound shaders, and preview images. It inherits from idDecl and provides
   functionality for parsing video declaration data from text, managing the video's lifecycle, and retrieving various properties of the video declaration. The class handles different aspects of video
   asset management such as Roq videos, wave sounds, and preview materials. It provides methods to access the video's name, information string, and associated resources like materials and sound
   shaders.

*/
class idDeclVideo : public idDecl
{
public:
	idDeclVideo() :
		preview( NULL ),
		video( NULL ),
		audio( NULL ) {};

	//! Returns the size in bytes of the idDeclVideo class instance.
	virtual size_t		 Size() const;

	//! Returns the default definition string for a video declaration
	virtual const char*	 DefaultDefinition() const;

	//! Parses video declaration data from text.
	virtual bool		 Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees the data associated with the video declaration.
	virtual void		 FreeData();

	//! Prints a placeholder implementation message to the common output.
	virtual void		 Print() const;

	//! Prints a placeholder message indicating the function is not yet implemented
	virtual void		 List() const;

	//! Returns the material associated with the Roq video.
	const idMaterial*	 GetRoq() const { return video; }

	//! Returns a pointer to the sound shader associated with the video declaration.
	const idSoundShader* GetWave() const { return audio; }

	//! Returns the name of the video declaration.
	const char*			 GetVideoName() const { return videoName; }

	//! Returns the info string associated with the video declaration.
	const char*			 GetInfo() const { return info; }

	//! Returns the preview material for this video declaration.
	const idMaterial*	 GetPreview() const { return preview; }

private:
	const idMaterial*	 preview;
	const idMaterial*	 video;
	idStr				 videoName;
	idStr				 info;
	const idSoundShader* audio;
};

/*!
	\class idDeclAudio
	\brief Manages audio declaration data including parsing and retrieval of sound shader information.

	This class handles audio declaration data by parsing audio definition text and storing associated sound shader information. It inherits from idDecl and provides methods for managing the audio data
   lifecycle, including parsing from text, freeing allocated resources, and retrieving various audio properties such as name, sound shader, and info string. The class serves as a container for audio
   declaration data within the engine's asset management system.

*/
class idDeclAudio : public idDecl
{
public:
	idDeclAudio() :
		audio( NULL ) {};

	//! Returns the size in bytes of the idDeclAudio class structure.
	virtual size_t		 Size() const;

	//! Returns the default definition string for an audio declaration.
	virtual const char*	 DefaultDefinition() const;

	//! Parses audio declaration data from a text buffer.
	virtual bool		 Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees the data associated with the audio declaration.
	virtual void		 FreeData();

	//! Prints a placeholder message indicating the function needs implementation.
	virtual void		 Print() const;

	//! Prints a placeholder message indicating the function is not yet implemented.
	virtual void		 List() const;

	//! Returns the audio name associated with this audio declaration.
	const char*			 GetAudioName() const { return audioName; }

	//! Returns a pointer to the sound shader associated with this audio declaration.
	const idSoundShader* GetWave() const { return audio; }

	//! Returns the info string associated with this audio declaration.
	const char*			 GetInfo() const { return info; }

private:
	const idSoundShader* audio;
	idStr				 audioName;
	idStr				 info;
};

/*!
	\class idDeclPDA
	\brief A class representing a PDA declaration that manages multimedia assets and related data.

	This class implements a PDA (Personal Digital Assistant) declaration that handles various multimedia assets such as videos, audios, and emails. It provides functionality for parsing PDA data from
   text, managing asset additions and removals, and retrieving information about the PDA's attributes like name, security level, and associated icons. The class extends idDecl and offers methods to
   query the number and specific assets stored within the declaration. It also supports setting and retrieving security configurations and maintaining a list of added assets for potential removal. The
   implementation includes placeholder methods for printing and listing functionality that may be further developed.

*/
class idDeclPDA : public idDecl
{
public:
	idDeclPDA() { originalEmails = originalVideos = 0; };

	//! Returns the size in bytes of the idDeclPDA class instance
	virtual size_t		Size() const;

	//! Returns the default definition string for a PDA declaration.
	virtual const char* DefaultDefinition() const;

	//! Parses PDA declaration data from a text buffer.
	virtual bool		Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Frees all data members of the idDeclPDA class.
	virtual void		FreeData();

	//! Prints a placeholder message indicating the implementation is incomplete
	virtual void		Print() const;

	//! Prints a placeholder message indicating the function is not yet implemented
	virtual void		List() const;

	//! Adds a video to the PDA declaration, with an option to ensure uniqueness.
	virtual void		AddVideo( const idDeclVideo* video, bool unique = true ) const
	{
		if( unique ) {
			videos.AddUnique( video );
		} else {
			videos.Append( video );
		}
	}

	//! Adds an audio declaration to the PDA, optionally ensuring uniqueness.
	virtual void AddAudio( const idDeclAudio* audio, bool unique = true ) const
	{
		if( unique ) {
			audios.AddUnique( audio );
		} else {
			audios.Append( audio );
		}
	}

	//! Adds an email to the PDA declaration, with an option to ensure uniqueness.
	virtual void AddEmail( const idDeclEmail* email, bool unique = true ) const
	{
		if( unique ) {
			emails.AddUnique( email );
		} else {
			emails.Append( email );
		}
	}

	//! Removes emails and videos from the PDA declaration that were added after the original state.
	virtual void			   RemoveAddedEmailsAndVideos() const;

	//! Returns the number of videos in the PDA declaration.
	virtual const int		   GetNumVideos() const { return videos.Num(); }

	//! Returns the number of audio assets associated with this PDA declaration.
	virtual const int		   GetNumAudios() const { return audios.Num(); }

	//! Returns the number of emails in the PDA declaration.
	virtual const int		   GetNumEmails() const { return emails.Num(); }

	//! Returns the video at the specified index or NULL if the index is out of bounds.
	virtual const idDeclVideo* GetVideoByIndex( int index ) const { return ( index < 0 || index > videos.Num() ? NULL : videos[index] ); }

	//! Returns the audio declaration at the specified index, or NULL if the index is out of bounds.
	virtual const idDeclAudio* GetAudioByIndex( int index ) const { return ( index < 0 || index > audios.Num() ? NULL : audios[index] ); }

	//! Returns the email at the specified index or NULL if the index is out of bounds.
	virtual const idDeclEmail* GetEmailByIndex( int index ) const { return ( index < 0 || index > emails.Num() ? NULL : emails[index] ); }

	//! Sets the security value for the PDA declaration.
	virtual void			   SetSecurity( const char* sec ) const;

	//! Returns the PDA name string stored in the declaration.
	const char*				   GetPdaName() const { return pdaName; }

	//! Returns the security level of the PDA declaration as a string.
	const char*				   GetSecurity() const { return security; }

	//! Returns the full name of the PDA declaration.
	const char*				   GetFullName() const { return fullName; }

	//! Returns the icon associated with this PDA declaration.
	const char*				   GetIcon() const { return icon; }

	//! Returns the post string associated with this PDA declaration.
	const char*				   GetPost() const { return post; }

	//! Returns the ID string associated with this PDA declaration.
	const char*				   GetID() const { return id; }

	//! Returns the title string associated with this PDA declaration.
	const char*				   GetTitle() const { return title; }

private:
	mutable idList<const idDeclVideo*> videos;
	mutable idList<const idDeclAudio*> audios;
	mutable idList<const idDeclEmail*> emails;
	idStr							   pdaName;
	idStr							   fullName;
	idStr							   icon;
	idStr							   id;
	idStr							   post;
	idStr							   title;
	mutable idStr					   security;
	mutable int						   originalEmails;
	mutable int						   originalVideos;
};

#endif /* !__DECLPDA_H__ */
