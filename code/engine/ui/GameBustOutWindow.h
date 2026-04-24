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
#ifndef __GAME_BUSTOUT_WINDOW_H__
#define __GAME_BUSTOUT_WINDOW_H__

class idGameBustOutWindow;

typedef enum { POWERUP_NONE = 0, POWERUP_BIGPADDLE, POWERUP_MULTIBALL } powerupType_t;

/*!
	\class BOEntity
	\brief BOEntity represents a drawable game entity with configurable properties and save/load capabilities.

	BOEntity serves as a base class for game entities that can be rendered and manipulated within the game world. It provides functionality for setting visual properties such as material, size, color,
   and visibility, as well as handling persistence through save and load operations. The class supports updating entity state based on time progression and rendering logic. The entity's behavior is
   managed through update and draw methods, with the ability to control visibility and fade-out effects. The class is designed to work in conjunction with a game window context for proper
   initialization and rendering.

*/
class BOEntity
{
public:
	bool				 visible;

	idStr				 materialName;
	const idMaterial*	 material;
	float				 width, height;
	idVec4				 color;
	idVec2				 position;
	idVec2				 velocity;

	powerupType_t		 powerup;

	bool				 removed;
	bool				 fadeOut;

	idGameBustOutWindow* game;

public:
	//! Initializes a new BOEntity instance with the specified game window.
	BOEntity( idGameBustOutWindow* _game );
	virtual ~BOEntity();

	//! Writes the entity's state to a save game file.
	virtual void WriteToSaveGame( idFile* savefile );

	//! Restores the entity's state from a save game file
	virtual void ReadFromSaveGame( idFile* savefile, idGameBustOutWindow* _game );

	//! Sets the material for the entity using the provided material name.
	void		 SetMaterial( const char* name );

	//! Sets the width and height dimensions of the entity
	void		 SetSize( float _width, float _height );

	/*!
		\brief Sets the color components for the entity

		Configures the color properties of the entity by setting the red, green, blue, and alpha components. The color values are typically expected to be in the range of 0.0 to 1.0, where 0.0
	   represents no intensity and 1.0 represents full intensity for each color component. This function directly assigns the provided values to the corresponding components of the entity's color
	   vector

		\param r Red color component value, typically in the range 0.0 to 1.0
		\param g Green color component value, typically in the range 0.0 to 1.0
		\param b Blue color component value, typically in the range 0.0 to 1.0
		\param a Alpha color component value, typically in the range 0.0 to 1.0
	*/
	void		 SetColor( float r, float g, float b, float a );

	//! Sets the visibility state of the entity.
	void		 SetVisible( bool isVisible );

	//! Updates the entity position and handles fade-out effects based on the time slice and GUI time.
	virtual void Update( float timeslice, int guiTime );

	//! Draws the entity if it is visible
	virtual void Draw();

private:
};

typedef enum { COLLIDE_NONE = 0, COLLIDE_DOWN, COLLIDE_UP, COLLIDE_LEFT, COLLIDE_RIGHT } collideDir_t;

/*!
	\class BOBrick
	\brief Represents a brick entity in the game with position, dimensions, and game properties.

	The BOBrick class manages the state and behavior of individual brick entities within the game. It handles initialization with specific position and dimensions, tracking of break status and powerup
   state, and serialization for save game functionality. The class maintains a reference to its associated game entity and provides methods for collision detection and color management. Brick entities
   are initialized with default properties including no powerup and unbroken status. The class supports persistence through save game operations that serialize position, dimensions, powerup
   information, and break state. Collision checking operates on position and velocity parameters to determine interactions with other game elements.

*/
class BOBrick
{
public:
	float		  x;
	float		  y;
	float		  width;
	float		  height;
	powerupType_t powerup;

	bool		  isBroken;

	BOEntity*	  ent;

public:
	//! Initializes a new BOBrick instance with default values.
	BOBrick();

	/*!
		\brief Initializes a new brick entity with specified position and dimensions, and sets up its game properties.

		The constructor initializes the brick's position, size, and game-related properties. It assigns the provided entity and sets its position and size. The brick is initialized with no powerup and
	   is marked as not broken. The entity is appended to the game's list of entities.

		\param _ent Pointer to the BOEntity that represents the brick in the game.
		\param _x The x-coordinate of the brick's position.
		\param _y The y-coordinate of the brick's position.
		\param _width The width of the brick.
		\param _height The height of the brick.
	*/
	BOBrick( BOEntity* _ent, float _x, float _y, float _width, float _height );
	~BOBrick();

	//! Writes brick data to a save game file
	virtual void WriteToSaveGame( idFile* savefile );

	//! Reads brick data from a save game file including position, dimensions, powerup, and break status.
	virtual void ReadFromSaveGame( idFile* savefile, idGameBustOutWindow* game );

	//! Sets the color of the brick entity using the provided RGBA values
	void		 SetColor( idVec4 bcolor );

	//! Checks for collision between a moving brick and a given position and velocity.
	collideDir_t checkCollision( idVec2 pos, idVec2 vel );

private:
};

#define BOARD_ROWS 12

/*!
	\class idGameBustOutWindow
	\brief Manages the BustOut game window functionality including game state, rendering, and event handling.

	The idGameBustOutWindow class implements the game window logic for a Breakout-style game, handling game state management, rendering, and user input. It inherits from idWindow and provides
   specialized functionality for managing game entities such as balls, paddles, bricks, and powerups. The class supports serialization to and from save files, initializes game assets, and processes
   game events including ball launches, collisions, and score updates. It manages multiple game levels through board data loaded from TGA files and handles the dynamic creation and destruction of game
   entities during gameplay. The implementation includes dedicated methods for updating different game components including paddle movement, ball physics, powerup behavior, and score tracking.

*/
class idGameBustOutWindow : public idWindow
{
public:
	//! Constructs an idGameBustOutWindow object with the specified user interface.
	idGameBustOutWindow( idUserInterfaceLocal* gui );

	//! Destructor for the idGameBustOutWindow class that cleans up allocated resources.
	~idGameBustOutWindow();

	//! Serializes the BustOut game window state to a save file.
	virtual void		WriteToSaveGame( idFile* savefile );

	//! Restores the state of the BustOut game window from a save file
	virtual void		ReadFromSaveGame( idFile* savefile );

	//! Handles input events for the BustOut window, managing ball launch and visual updates.
	virtual const char* HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Initializes the BustOut window after parsing.
	virtual void		PostParse();

	//! Draws the game entities after updating the game state
	virtual void		Draw( int time, float x, float y );

	//! Activates or deactivates the window and returns an empty string.
	virtual const char* Activate( bool activate );

	//! Returns a pointer to a window variable by its name, checking predefined variables first before falling back to the base implementation.
	virtual idWinVar*	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	idList<BOEntity*>	entities;

private:
	//! Initializes the BustOut game window by precaching assets and setting up the initial game state.
	void		 CommonInit();

	//! Resets the game state to its initial values.
	void		 ResetGameState();

	//! Clears the game board by removing all bricks and powerups.
	void		 ClearBoard();

	//! Removes all power-ups from the game window.
	void		 ClearPowerups();

	//! Removes all balls from the game window and resets the ball count.
	void		 ClearBalls();

	//! Loads the board data for all levels from TGA image files.
	void		 LoadBoardFiles();

	//! Initializes and sets up the current game board based on the level data.
	void		 SetCurrentBoard();

	//! Updates the game state including paddle, ball, powerups, and entities.
	void		 UpdateGame();

	//! Updates the state and behavior of powerups in the game window
	void		 UpdatePowerups();

	//! Updates the paddle position based on cursor input and paddle size adjustments.
	void		 UpdatePaddle();

	//! Updates the state and position of balls in the BustOut game window
	void		 UpdateBall();

	//! Updates the game score and handles game events based on score and brick status.
	void		 UpdateScore();

	//! Creates a new ball entity for the BustOut game window.
	BOEntity*	 CreateNewBall();

	//! Creates a power-up entity at the location of the given brick
	BOEntity*	 CreatePowerup( BOBrick* brick );

	//! Parses internal variables for the BustOut window, handling game state flags and level configuration.
	virtual bool ParseInternalVar( const char* name, idTokenParser* src );

private:
	idWinBool		  gamerunning;
	idWinBool		  onFire;
	idWinBool		  onContinue;
	idWinBool		  onNewGame;
	idWinBool		  onNewLevel;

	float			  timeSlice;
	bool			  gameOver;

	int				  numLevels;
	byte*			  levelBoardData;
	bool			  boardDataLoaded;

	int				  numBricks;
	int				  currentLevel;

	bool			  updateScore;
	int				  gameScore;
	int				  nextBallScore;

	int				  bigPaddleTime;
	float			  paddleVelocity;

	float			  ballSpeed;
	int				  ballsRemaining;
	int				  ballsInPlay;
	bool			  ballHitCeiling;

	idList<BOEntity*> balls;
	idList<BOEntity*> powerUps;

	BOBrick*		  paddle;
	idList<BOBrick*>  board[BOARD_ROWS];
};

#endif //__GAME_BUSTOUT_WINDOW_H__
