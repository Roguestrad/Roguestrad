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

#ifndef __PLAYERICON_H__
#define __PLAYERICON_H__

typedef enum { ICON_LAG, ICON_CHAT, ICON_TEAM_RED, ICON_TEAM_BLUE, ICON_NONE } playerIconType_t;

/*!
	\class idPlayerIcon
	\brief Manages rendering and updating of player icons in the game world.

	The idPlayerIcon class handles the creation, drawing, and management of player icons that represent players in the game world. It supports different icon types and can render icons at specific
   joint positions or arbitrary 3D origins. The class manages the underlying render world resources and ensures proper cleanup when icons are freed or replaced. Icons can be updated to reflect changes
   in player position or orientation. The implementation handles material assignment based on icon type and provides mechanisms to prevent unnecessary recreation of identical icons.

*/
class idPlayerIcon
{
public:
public:
	//! Constructs a new idPlayerIcon object with default values.
	idPlayerIcon();

	//! Destroys the player icon and frees associated resources.
	~idPlayerIcon();

	//! Draws a player icon at the specified joint position on the player.
	void Draw( idPlayer* player, jointHandle_t joint );

	//! Draws a player icon for the specified player at the given origin
	void Draw( idPlayer* player, const idVec3& origin );

public:
	playerIconType_t iconType;
	renderEntity_t	 renderEnt;
	qhandle_t		 iconHandle;

public:
	//! Releases the render world entity definition for the player icon and resets its type.
	void FreeIcon();

	/*!
		\brief Creates a player icon with the specified type, material, origin, and axis.

		This function initializes a player icon entity using the provided parameters. It first checks that the icon type is valid and not already set to the current type. If the icon needs to be
	   replaced, it frees the existing icon and sets up a new render entity with the specified properties. The icon is then added to the render world and stored with the given type.

		\param player The player associated with the icon
		\param type The type of icon to create
		\param mtr The material name for the icon's shader
		\param origin The 3D position of the icon
		\param axis The orientation of the icon as a 3x3 matrix
		\return True if the icon was successfully created, false if the icon type matches the current one
		\throws Asserts if the icon type is ICON_NONE
	*/
	bool CreateIcon( idPlayer* player, playerIconType_t type, const char* mtr, const idVec3& origin, const idMat3& axis );

	/*!
		\brief Creates a player icon with the specified type, origin, and axis for the given player.

		This function initializes a player icon using the provided player reference, icon type, origin position, and orientation axis. It retrieves the appropriate material string from the player's
	   spawn arguments based on the icon type, and then delegates to another CreateIcon method that uses the material string. The function asserts that the icon type is valid and less than ICON_NONE.

		\param player Pointer to the player for whom the icon is being created
		\param type Type of the player icon to create
		\param origin The 3D position where the icon will be placed
		\param axis The orientation matrix defining the icon's rotation
		\return True if the icon was successfully created, false otherwise
		\throws Assertion error if the icon type is invalid (greater than or equal to ICON_NONE)
	*/
	bool CreateIcon( idPlayer* player, playerIconType_t type, const idVec3& origin, const idMat3& axis );

	//! Updates the icon's position and orientation in the game world.
	void UpdateIcon( idPlayer* player, const idVec3& origin, const idMat3& axis );
};

#endif /* !_PLAYERICON_H_ */
