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
#ifndef __SYS_SESSION_SAVEGAMES_H__
#define __SYS_SESSION_SAVEGAMES_H__

/*!
	\class idSaveGameProcessorLoadFiles
	\brief Handles asynchronous loading of save game files from a specified folder.
*/
class idSaveGameProcessorLoadFiles : public idSaveGameProcessor
{
public:
	DEFINE_CLASS( idSaveGameProcessorLoadFiles );

	//! Initializes the load files processor for loading save game files from a specified folder
	virtual bool InitLoadFiles( const char* folder, const saveFileEntryList_t& files, idSaveGameManager::packageType_t type = idSaveGameManager::PACKAGE_GAME );

	//! This function processes save game file loading asynchronously
	virtual bool Process();
};

/*!
	\class idSaveGameProcessorDelete
	\brief A processor for asynchronously deleting save game files.
*/
class idSaveGameProcessorDelete : public idSaveGameProcessor
{
public:
	DEFINE_CLASS( idSaveGameProcessorDelete );

	//! Initializes the delete processor for a specified save game folder.
	bool		 InitDelete( const char* folder, idSaveGameManager::packageType_t type = idSaveGameManager::PACKAGE_GAME );

	//! Performs asynchronous deletion of a save game file
	virtual bool Process();
};

/*!
	\class idSaveGameProcessorSaveFiles
	\brief A save game processor that handles saving files to a specified directory with given metadata.
*/
class idSaveGameProcessorSaveFiles : public idSaveGameProcessor
{
public:
	DEFINE_CLASS( idSaveGameProcessorSaveFiles );

	/*!
		\brief Initializes a save operation with the specified folder, files, description, and package type.

		This function sets up the save system by preparing the directory path, configuring the save mode to prevent deletion of existing files, and appending the provided file entries to the save
	   parameters. It also copies the save game details and sets the slot name to the provided folder. The function returns true if the initialization is successful, or false if the base
	   initialization fails or if no files are provided.

		\param folder The directory path where the save files will be stored
		\param files A list of file entries to be included in the save operation
		\param description Details about the save game, including descriptors and other metadata
		\param type The package type indicating the category of the save operation
		\return True if the save initialization is successful, false otherwise
	*/
	bool		 InitSave( const char* folder, const saveFileEntryList_t& files, const idSaveGameDetails& description, idSaveGameManager::packageType_t type = idSaveGameManager::PACKAGE_GAME );

	//! This function initiates an asynchronous save game operation and returns false.
	virtual bool Process();
};

/*!
	\class idSaveGameProcessorEnumerateGames
	\brief A processor for asynchronously enumerating save games.
*/
class idSaveGameProcessorEnumerateGames : public idSaveGameProcessor
{
public:
	DEFINE_CLASS( idSaveGameProcessorEnumerateGames );

	//! Initializes and starts an asynchronous savegame enumeration process.
	virtual bool Process();
};

#endif
