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
#ifndef __GAME_BEARSHOOT_WINDOW_H__
#define __GAME_BEARSHOOT_WINDOW_H__

class idGameBearShootWindow;

/*!
	\class BSEntity
	\brief BSEntity represents a game entity with visual and positional properties.

	BSEntity manages the state and rendering of game objects, including their material, dimensions, visibility, and appearance. The class supports serialization through save game functionality and
   provides methods for updating the entity's state based on time progression. It is designed to be used within a game window context, with initialization requiring a reference to the game window. The
   entity's visual properties such as material, size, and visibility can be modified, and it supports drawing operations when visible.

*/
class BSEntity
{
public:
	const idMaterial*	   material;
	idStr				   materialName;
	float				   width, height;
	bool				   visible;

	idVec4				   entColor;
	idVec2				   position;
	float				   rotation;
	float				   rotationSpeed;
	idVec2				   velocity;

	bool				   fadeIn;
	bool				   fadeOut;

	idGameBearShootWindow* game;

public:
	//! Initializes a new BSEntity object with the specified game window.
	BSEntity( idGameBearShootWindow* _game );
	virtual ~BSEntity();

	//! Writes entity data to a save game file.
	virtual void WriteToSaveGame( idFile* savefile );

	//! Restores the entity's state from a save game file.
	virtual void ReadFromSaveGame( idFile* savefile, idGameBearShootWindow* _game );

	//! Sets the material for the entity using the provided material name.
	void		 SetMaterial( const char* name );

	//! Sets the width and height dimensions of the entity
	void		 SetSize( float _width, float _height );

	//! Sets the visibility state of the entity.
	void		 SetVisible( bool isVisible );

	//! Updates the entity's state including fading, position, and rotation based on the time slice.
	virtual void Update( float timeslice );

	//! Draws the entity on screen if it is visible.
	virtual void Draw();

private:
};

/*!
	\class idGameBearShootWindow
	\brief Manages the user interface and game logic for a bear shooting window interaction.

	This class implements a specialized user interface window for a bear shooting game mechanic, inheriting from a base window class to handle rendering and event management. It maintains game state
   variables, manages entities such as bears, helicopters, and turrets, and coordinates the overall game flow including physics updates, scoring, and level progression. The class initializes game
   assets, handles system events, and provides methods for saving and loading game state. It integrates with a user interface system to display visual elements and respond to user input.

*/
class idGameBearShootWindow : public idWindow
{
public:
	//! Initializes a new instance of the idGameBearShootWindow class with the specified user interface.
	idGameBearShootWindow( idUserInterfaceLocal* gui );

	//! Destructor for the idGameBearShootWindow class that cleans up its entities.
	~idGameBearShootWindow();

	//! Writes the game state to a save file for the bear shoot window UI
	virtual void		WriteToSaveGame( idFile* savefile );

	//! Loads the state of the bear shoot window from a save game file
	virtual void		ReadFromSaveGame( idFile* savefile );

	//! Handles system events for the bear shoot window interface
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Performs post-parsing operations for the Bear Shoot window.
	virtual void		PostParse();

	//! Draws the game window elements at the specified time and position
	virtual void		Draw( int time, float x, float y );

	//! Activates or deactivates the game bear shoot window and returns an empty string.
	virtual const char* Activate( bool activate );

	//! Retrieves a window variable by name, checking specific member variables first before falling back to the base implementation.
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

private:
	//! Initializes the bear shoot game window by precaching assets and setting up game entities.
	void		 CommonInit();

	//! Resets the game state variables to their initial values.
	void		 ResetGameState();

	//! Updates the bear's physics and state in the bear shoot game window.
	void		 UpdateBear();

	//! Updates the helicopter's position and state based on game conditions and level progression.
	void		 UpdateHelicopter();

	//! Updates the turret angle based on the cursor position
	void		 UpdateTurret();

	//! Updates the button states and performs firing actions for the bear shooting window.
	void		 UpdateButtons();

	//! Updates the game state including physics, rendering, and input handling
	void		 UpdateGame();

	//! Updates the game score and handles level progression when goals are hit.
	void		 UpdateScore();

	//! Parses internal variables for the Bear Shoot window, setting boolean flags based on the variable name and source token parser.
	virtual bool ParseInternalVar( const char* name, idTokenParser* src );

private:
	idWinBool		  gamerunning;
	idWinBool		  onFire;
	idWinBool		  onContinue;
	idWinBool		  onNewGame;

	float			  timeSlice;
	float			  timeRemaining;
	bool			  gameOver;

	int				  currentLevel;
	int				  goalsHit;
	bool			  updateScore;
	bool			  bearHitTarget;

	float			  bearScale;
	bool			  bearIsShrinking;
	int				  bearShrinkStartTime;

	float			  turretAngle;
	float			  turretForce;

	float			  windForce;
	int				  windUpdateTime;

	idList<BSEntity*> entities;

	BSEntity*		  turret;
	BSEntity*		  bear;
	BSEntity*		  helicopter;
	BSEntity*		  goal;
	BSEntity*		  wind;
	BSEntity*		  gunblast;
};

#endif //__GAME_BEARSHOOT_WINDOW_H__
