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

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Rectangle.h"
#include "DeviceContext.h"
#include "RegExp.h"
#include "Winvar.h"
#include "GuiScript.h"
#include "SimpleWindow.h"

const int  WIN_CHILD		= 0x00000001;
const int  WIN_CAPTION		= 0x00000002;
const int  WIN_BORDER		= 0x00000004;
const int  WIN_SIZABLE		= 0x00000008;
const int  WIN_MOVABLE		= 0x00000010;
const int  WIN_FOCUS		= 0x00000020;
const int  WIN_CAPTURE		= 0x00000040;
const int  WIN_HCENTER		= 0x00000080;
const int  WIN_VCENTER		= 0x00000100;
const int  WIN_MODAL		= 0x00000200;
const int  WIN_INTRANSITION = 0x00000400;
const int  WIN_CANFOCUS		= 0x00000800;
const int  WIN_SELECTED		= 0x00001000;
const int  WIN_TRANSFORM	= 0x00002000;
const int  WIN_HOLDCAPTURE	= 0x00004000;
const int  WIN_NOWRAP		= 0x00008000;
const int  WIN_NOCLIP		= 0x00010000;
const int  WIN_INVERTRECT	= 0x00020000;
const int  WIN_NATURALMAT	= 0x00040000;
const int  WIN_NOCURSOR		= 0x00080000;
const int  WIN_MENUGUI		= 0x00100000;
const int  WIN_ACTIVE		= 0x00200000;
const int  WIN_SHOWCOORDS	= 0x00400000;
const int  WIN_SHOWTIME		= 0x00800000;
const int  WIN_WANTENTER	= 0x01000000;

const int  WIN_DESKTOP = 0x10000000;

const char CAPTION_HEIGHT[] = "16.0";
const char SCROLLER_SIZE[]	= "16.0";
const int  SCROLLBAR_SIZE	= 16;

const int  MAX_WINDOW_NAME = 32;
const int  MAX_LIST_ITEMS  = 1024;

const char DEFAULT_BACKCOLOR[]	 = "1 1 1 1";
const char DEFAULT_FORECOLOR[]	 = "0 0 0 1";
const char DEFAULT_BORDERCOLOR[] = "0 0 0 1";
const char DEFAULT_TEXTSCALE[]	 = "0.4";

typedef enum {
	WOP_TYPE_ADD,
	WOP_TYPE_SUBTRACT,
	WOP_TYPE_MULTIPLY,
	WOP_TYPE_DIVIDE,
	WOP_TYPE_MOD,
	WOP_TYPE_TABLE,
	WOP_TYPE_GT,
	WOP_TYPE_GE,
	WOP_TYPE_LT,
	WOP_TYPE_LE,
	WOP_TYPE_EQ,
	WOP_TYPE_NE,
	WOP_TYPE_AND,
	WOP_TYPE_OR,
	WOP_TYPE_VAR,
	WOP_TYPE_VARS,
	WOP_TYPE_VARF,
	WOP_TYPE_VARI,
	WOP_TYPE_VARB,
	WOP_TYPE_COND
} wexpOpType_t;

typedef enum { WEXP_REG_TIME, WEXP_REG_NUM_PREDEFINED } wexpRegister_t;

typedef struct {
	wexpOpType_t opType;
	// RB: 64 bit fixes, changed int to intptr_t
	intptr_t	 a, b, c, d;
	// RB end
} wexpOp_t;

struct idRegEntry {
	const char*			name;
	idRegister::REGTYPE type;
	int					index;
};

class rvGEWindowWrapper;
class idWindow;

struct idTimeLineEvent {
	//! Initializes a new timeline event with a new GUI script list.
	idTimeLineEvent() { event = new( TAG_OLD_UI ) idGuiScriptList; }
	~idTimeLineEvent() { delete event; }
	int				 time;
	idGuiScriptList* event;
	bool			 pending;

	//! Returns the total memory size of the timeline event including its embedded event data.
	size_t			 Size() { return sizeof( *this ) + event->Size(); }
};

/*!
	\class rvNamedEvent
	\brief A named event container that manages event data with associated metadata.
*/
class rvNamedEvent
{
public:
	//! Constructs a new named event with the specified name
	rvNamedEvent( const char* name )
	{
		mEvent = new( TAG_OLD_UI ) idGuiScriptList;
		mName  = name;
	}
	~rvNamedEvent() { delete mEvent; }

	//! Returns the total memory size occupied by this named event object and its associated event data.
	size_t			 Size() { return sizeof( *this ) + mEvent->Size(); }

	idStr			 mName;
	idGuiScriptList* mEvent;
};

struct idTransitionData {
	idWinVar*							  data;
	int									  offset;
	idInterpolateAccelDecelLinear<idVec4> interp;
};

class idUserInterfaceLocal;

/*!
	\class idWindow
	\brief A window management class for handling graphical user interface elements and their interactions.

	The idWindow class serves as the core component for managing graphical user interface elements within a windowing system. It handles the creation, positioning, sizing, and visual rendering of UI
   components while maintaining relationships with parent and child windows. The class supports event handling, focus management, scripting, and state transitions for interactive UI elements. It
   provides functionality for parsing GUI definitions from token-based sources, managing window variables and expressions, and coordinating complex operations like animations and transitions. The
   window system maintains hierarchical relationships between elements, supports various interaction modes including mouse capture and focus, and provides mechanisms for debugging and serialization of
   UI state. The class integrates with a scripting system to handle user interactions and dynamic UI behavior, while also managing resources such as fonts, materials, and visual transformations.

*/
class idWindow
{
public:
	//! Constructs an idWindow object with the specified user interface.
	idWindow( idUserInterfaceLocal* gui );

	//! Destroys the window and cleans up resources.
	virtual ~idWindow();

	enum { ON_MOUSEENTER = 0, ON_MOUSEEXIT, ON_ACTION, ON_ACTIVATE, ON_DEACTIVATE, ON_ESC, ON_FRAME, ON_TRIGGER, ON_ACTIONRELEASE, ON_ENTER, ON_ENTERRELEASE, SCRIPT_COUNT };

	enum { ADJUST_MOVE = 0, ADJUST_TOP, ADJUST_RIGHT, ADJUST_BOTTOM, ADJUST_LEFT, ADJUST_TOPLEFT, ADJUST_BOTTOMRIGHT, ADJUST_TOPRIGHT, ADJUST_BOTTOMLEFT };

	static const char*		ScriptNames[SCRIPT_COUNT];

	static const idRegEntry RegisterVars[];
	static const int		NumRegisterVars;

	//! Sets the focus to the specified window and optionally runs focus-related scripts.
	idWindow*				SetFocus( idWindow* w, bool scripts = true );

	//! Sets the capture for the window, releasing it from any previously captured child.
	idWindow*				SetCapture( idWindow* w );

	//! Sets the parent window of this window.
	void					SetParent( idWindow* w );

	//! Sets the specified flag for the window.
	void					SetFlag( unsigned int f );

	//! Clears the specified flag in the window's flag set.
	void					ClearFlag( unsigned int f );
	unsigned				GetFlags() { return flags; };

	//! Moves the window to the specified coordinates.
	void					Move( float x, float y );

	//! Moves the specified window to the top of the window stack
	void					BringToTop( idWindow* w );
	void					Adjust( float xd, float yd );
	void					SetAdjustMode( idWindow* child );

	/*!
		\brief Sets the position and size of the window using the specified coordinates and dimensions.

		This function updates the rectangle coordinates of the window by setting the x, y, width, and height values. It then recalculates the client rectangle to reflect the new size and position. The
	   function is typically used to resize and reposition UI elements within the window system.

		\param x The x-coordinate of the window's position
		\param y The y-coordinate of the window's position
		\param w The width of the window
		\param h The height of the window
	*/
	void					Size( float x, float y, float w, float h );

	//! Initializes window properties based on current state settings.
	void					SetupFromState();

	//! Initializes the background material for the window based on the background name.
	void					SetupBackground();

	//! Finds and returns a child window by its name, searching recursively through nested windows.
	drawWin_t*				FindChildByName( const char* name );

	//! Returns the simple window with the specified name from the list of drawn windows.
	idSimpleWindow*			FindSimpleWinByName( const char* _name );

	//! Returns the parent window of this window.
	idWindow*				GetParent() { return parent; }
	idUserInterfaceLocal*	GetGui() { return gui; };

	//! Checks if the specified point is contained within the window's bounds
	bool					Contains( float x, float y );

	//! Returns the total size in bytes of the window and its children
	size_t					Size();

	//! Returns the total memory allocated by the window object and its associated data structures.
	virtual size_t			Allocated();

	//! Returns a pointer to an idStr by its name, or NULL if not found.
	idStr*					GetStrPtrByName( const char* _name );

	//! Retrieves a window variable by its name, handling both built-in and user-defined variables.
	virtual idWinVar*		GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

	//! Calculates the byte offset of a window variable within the window object.
	intptr_t				GetWinVarOffset( idWinVar* wv, drawWin_t* dw );

	//! Returns the maximum height of characters in the current font.
	float					GetMaxCharHeight();

	//! Returns the maximum character width for the current font and text scale
	float					GetMaxCharWidth();

	//! Sets the font for the window.
	void					SetFont();

	//! Initializes the window state with the specified name and default properties.
	void					SetInitialState( const char* _name );

	//! Adds a child window to this window.
	void					AddChild( idWindow* win );

	//! Draws debug information for the window including rectangles and text output
	void					DebugDraw( int time, float x, float y );

	//! Calculates the client rectangle for the window based on positioning and border settings.
	void					CalcClientRect( float xofs, float yofs );

	//! Initializes the window properties to their default values.
	void					CommonInit();

	//! Cleans up all resources used by the window, including children, scripts, and various lists.
	void					CleanUp();

	//! Draws the border and caption for the window using the specified rectangle.
	void					DrawBorderAndCaption( const idRectangle& drawRect );
	void					DrawCaption( int time, float x, float y );

	//! Sets up transformation information for the window based on the provided offsets.
	void					SetupTransforms( float x, float y );

	//! Checks if a point is contained within a transformed rectangle
	bool					Contains( const idRectangle& sr, float x, float y );
	const char*				GetName() { return name; };

	//! Parses GUI window definitions from a token parser, optionally rebuilding the window structure
	virtual bool			Parse( idTokenParser* src, bool rebuild = true );

	//! Processes a system event for the window and returns a command string.
	virtual const char*		HandleEvent( const sysEvent_t* event, bool* updateVisuals );

	//! Calculates the rectangles for the window and its child windows based on the provided coordinates.
	void					CalcRects( float x, float y );

	//! Redraws the window and its children with optional HUD scaling and coordinate adjustments.
	virtual void			Redraw( float x, float y, bool hud );

	//! Archives the window and its children to a dictionary
	virtual void			ArchiveToDictionary( idDict* dict, bool useNames = true );

	//! Initializes window children from a dictionary with optional name-based retrieval.
	virtual void			InitFromDictionary( idDict* dict, bool byName = true );

	//! Sorts the timeline events for the window.
	virtual void			PostParse();

	//! Activates or deactivates the window and its children, updating window variables and running corresponding scripts.
	virtual void			Activate( bool activate, idStr& act );

	//! Executes the onTrigger script and recursively triggers child windows.
	virtual void			Trigger();

	//! Sets the window as the currently focused window.
	virtual void			GainFocus();

	//! Removes focus from the window.
	virtual void			LoseFocus();

	//! Gives capture to the window.
	virtual void			GainCapture();

	//! Removes the capture flag from the window.
	virtual void			LoseCapture();

	//! Notifies the window that its size has changed.
	virtual void			Sized();

	//! Handles the window movement event.
	virtual void			Moved();

	//! Draws the window text at the specified position with optional shadow and debug information
	virtual void			Draw( int time, float x, float y );

	//! Executes the mouse exit script when the mouse leaves the window area.
	virtual void			MouseExit();

	//! Executes the mouse enter script when the mouse enters the window area.
	virtual void			MouseEnter();

	//! Draws the background of the window using the specified rectangle.
	virtual void			DrawBackground( const idRectangle& drawRect );

	//! Returns the child window that contains the specified coordinates and has an on-action handler
	virtual idWindow*		GetChildWithOnAction( float xd, float yd );

	//! Routes mouse coordinates to the appropriate child window for handling
	virtual const char*		RouteMouseCoords( float xd, float yd );
	virtual void			SetBuddy( idWindow* buddy ) {};
	virtual void			HandleBuddyUpdate( idWindow* buddy ) {};

	//! Updates window state and propagates state changes to child windows.
	virtual void			StateChanged( bool redraw );

	//! Writes a string to a save game file with its length prefix.
	void					WriteSaveGameString( const char* string, idFile* savefile );

	//! Writes transition data to a save game file for a window
	void					WriteSaveGameTransition( idTransitionData& trans, idFile* savefile );

	//! Writes the window's state and associated data to a save game file.
	virtual void			WriteToSaveGame( idFile* savefile );

	//! Reads a save game string from a file into the provided string object.
	void					ReadSaveGameString( idStr& string, idFile* savefile );

	//! Reads transition data from a save game file for a window.
	void					ReadSaveGameTransition( idTransitionData& trans, idFile* savefile );

	//! Restores the window state from a save game file.
	virtual void			ReadFromSaveGame( idFile* savefile );

	//! Updates transition references to point to correct target window properties after window initialization.
	void					FixupTransitions();
	virtual void			HasAction() {};
	virtual void			HasScripts() {};

	//! Recursively fixes up parameters for child windows, scripts, timeline events, named events, and operations.
	void					FixupParms();
	void					GetScriptString( const char* name, idStr& out );
	void					SetScriptParams();
	bool					HasOps() { return ( ops.Num() > 0 ); };

	//! Evaluates and returns the value of a specified register from the window's expression registers.
	float					EvalRegs( int test = -1, bool force = false );

	//! Initializes a window transition by setting the in-transition flag.
	void					StartTransition();

	/*!
		\brief Adds a transition animation to the window that interpolates between two vector values over a specified time period.

		The function sets up a transition animation between two idVec4 values, from and to, with configurable acceleration and deceleration timing. The transition is associated with a specific window
	   variable and is added to the window's list of active transitions.

		\param dest The window variable to which the transition applies
		\param from The starting vector value for the transition
		\param to The ending vector value for the transition
		\param time The total duration of the transition in milliseconds
		\param accelTime The time spent accelerating as a fraction of total time
		\param decelTime The time spent decelerating as a fraction of total time
	*/
	void					AddTransition( idWinVar* dest, idVec4 from, idVec4 to, int time, float accelTime, float decelTime );

	//! Resets the window's time line to the specified time value.
	void					ResetTime( int time );

	//! Resets the cinematic time for the window's background if it exists.
	void					ResetCinematics();

	//! Returns the total number of transitions in this window and its child windows.
	int						NumTransitions();

	/*!
		\brief Parses a script block from a token parser, handling commands and conditional statements.

		This function reads tokens from a parser to construct a list of GUI script commands. It handles nested blocks enclosed in braces, processes if/else conditional statements, and supports command
	   parsing with optional time parameters. The function supports both standard script parsing and parsing of else blocks when called recursively. It returns false if the script structure is invalid
	   or if parsing fails.

		\param src Pointer to the token parser that provides the script tokens
		\param list Reference to the list where parsed GUI script commands are stored
		\param timeParm Optional pointer to an integer that can store a time parameter, may be null
		\param allowIf Boolean flag indicating whether to allow parsing of if/else blocks
		\return True if the script was parsed successfully, false otherwise
	*/
	bool					ParseScript( idTokenParser* src, idGuiScriptList& list, int* timeParm = NULL, bool allowIf = false );

	//! Executes a script handler for a specified window event.
	bool					RunScript( int n );

	//! Executes a GUI script list associated with the window and returns true if successful.
	bool					RunScriptList( idGuiScriptList* src );
	void					SetRegs( const char* key, const char* val );

	//! Parses a mathematical expression from a token parser and returns the resulting register index.
	intptr_t				ParseExpression( idTokenParser* src, idWinVar* var = NULL, intptr_t component = 0 );

	//! Returns the index of an existing expression register with the specified float value or creates a new one
	int						ExpressionConstant( float f );

	//! Returns a pointer to the register list associated with the window.
	idRegisterList*			RegList() { return &regList; }

	//! Adds a command to the window's command list.
	void					AddCommand( const char* cmd );

	//! Adds a window variable to the update list if it is not already present.
	void					AddUpdateVar( idWinVar* var );

	//! Determines if the window or any of its child windows have interactive elements.
	bool					Interactive();

	//! Checks if the window or its children contain any state variables.
	bool					ContainsStateVars();

	//! Sets a variable value for a child window by name.
	void					SetChildWinVarVal( const char* name, const char* var, const char* val );

	//! Returns the currently focused child window if this window is a desktop, otherwise returns NULL.
	idWindow*				GetFocusedChild();

	//! Returns the child window that currently has mouse capture, or NULL if no child has capture.
	idWindow*				GetCaptureChild();

	//! Returns the comment associated with the window.
	const char*				GetComment() { return comment; }

	//! Sets the comment text for the window.
	void					SetComment( const char* p ) { comment = p; }

	idStr					cmd;

	//! Executes the named event by searching through registered events and running the matching one, then propagating the event to all child windows.
	virtual void			RunNamedEvent( const char* eventName );

	//! Adds a window variable to the list of defined variables, ensuring no duplicates.
	void					AddDefinedVar( idWinVar* var );

	//! Finds the child window under the given point, considering a starting window to skip.
	idWindow*				FindChildByPoint( float x, float y, idWindow* below = NULL );

	//! Returns the index of the given child window
	int						GetChildIndex( idWindow* window );

	//! Returns the number of child windows
	int						GetChildCount();

	//! Returns the child window at the specified index.
	idWindow*				GetChild( int index );

	//! Removes a child window from the list of children.
	void					RemoveChild( idWindow* win );

	//! Inserts the given window as a child into the specified location in the z-order.
	bool					InsertChild( idWindow* win, idWindow* before );

	//! Converts screen coordinates to client coordinates for the specified rectangle.
	void					ScreenToClient( idRectangle* rect );

	//! Converts rectangle coordinates from client space to screen space
	void					ClientToScreen( idRectangle* rect );

	//! Updates the window properties from a dictionary by parsing its key-value pairs.
	bool					UpdateFromDictionary( idDict& dict );

protected:
	friend class rvGEWindowWrapper;

	//! Finds the window under the given point
	idWindow* FindChildByPoint( float x, float y, idWindow** below );

	//! Initializes window properties to their default values.
	void	  SetDefaults();

	friend class idSimpleWindow;
	friend class idUserInterfaceLocal;

	//! Determines if the window has no complex operations or child elements.
	bool								 IsSimple();

	//! Updates all window variables managed by this window.
	void								 UpdateWinVars();

	//! Disables a register with the specified name.
	void								 DisableRegister( const char* _name );

	//! Updates the window's transition animations and clears completed transitions.
	void								 Transition();

	//! Updates the window's time-based events and command execution.
	void								 Time();

	//! Executes runtime events and updates window state for the specified time.
	bool								 RunTimeEvents( int time );
	void								 Dump();

	//! Returns a temporary expression register index for GUI expression evaluation.
	int									 ExpressionTemporary();

	//! Returns a pointer to a new expression operation structure from the window's operation list.
	wexpOp_t*							 ExpressionOp();

	/*!
		\brief EmitOp creates a new operation node for expression evaluation and returns the register index of the result

		This function generates a new operation node for expression evaluation in the shader compilation process. It takes two input operands and an operation type, creates a new operation node with
	   the specified parameters, and allocates a temporary register for the result. The function optimizes away certain identity operations such as adding zero or multiplying by one, but these
	   optimizations are currently commented out in the implementation. The result register index is returned, which can be used for subsequent expression operations. The optional opp parameter allows
	   the caller to receive a direct pointer to the created operation node.

		\param a first operand register index
		\param b second operand register index
		\param opType type of operation to perform
		\param opp optional pointer to receive the created operation node
		\return register index of the temporary result created for the operation
	*/
	intptr_t							 EmitOp( intptr_t a, intptr_t b, wexpOpType_t opType, wexpOp_t** opp = NULL );

	/*!
		\brief Parses an expression and emits an operation with the given parameters

		This function parses an expression with a specified priority level using the provided token parser and emits an operation of the given type. It takes the result of parsing the expression with
	   the specified priority and combines it with the passed operand 'a' using the operation type to produce a new operand. The function can optionally store the resulting operation in the provided
	   operation pointer.

		\param src The token parser used to parse the expression
		\param a The left operand for the operation, represented as an intptr_t
		\param opType The type of operation to emit
		\param priority The priority level used to parse the expression
		\param opp Optional pointer to store the resulting operation
		\return The result of the emitted operation as an intptr_t value
	*/
	intptr_t							 ParseEmitOp( idTokenParser* src, intptr_t a, wexpOpType_t opType, int priority, wexpOp_t** opp = NULL );

	//! Parses a term expression from the token parser and returns a register index.
	intptr_t							 ParseTerm( idTokenParser* src, idWinVar* var = NULL, intptr_t component = 0 );

	/*!
		\brief Parses an expression with the specified priority level from the token parser

		This function recursively parses an expression with the given priority level by first parsing a lower priority expression and then applying operators of the current priority level. The
	   function handles different operator precedence levels from 0 to 4, where 0 is the lowest precedence and 4 is the highest. It supports arithmetic operations like multiplication, division, and
	   modulo, comparison operations like greater than, less than, and equality, as well as logical operations like AND and OR. The function also handles conditional expressions with the ternary
	   operator '?'. It returns a register index representing the parsed expression.

		\param src Token parser used to read tokens from the input
		\param priority The priority level of the operation to parse
		\param var Optional variable context for parsing
		\param component Component index to parse from the variable
		\return Register index representing the parsed expression, or 0 in case of parse errors
	*/
	intptr_t							 ParseExpressionPriority( idTokenParser* src, int priority, idWinVar* var = NULL, intptr_t component = 0 );

	//! Evaluates expression registers for the window
	void								 EvaluateRegisters( float* registers );

	//! Saves the current expression parse state by copying temporary register flags.
	void								 SaveExpressionParseState();

	//! Restores the expression parse state by restoring temporary register flags and freeing the saved temporary memory.
	void								 RestoreExpressionParseState();

	//! Parses a braced expression from the token parser input.
	void								 ParseBracedExpression( idTokenParser* src );

	//! Parses a script entry by name using the provided token parser and returns true if successful.
	bool								 ParseScriptEntry( const char* name, idTokenParser* src );

	//! Parses a register entry from the given token source and associates it with the specified name.
	bool								 ParseRegEntry( const char* name, idTokenParser* src );

	//! Parses internal window variables from a token parser and updates window properties accordingly.
	virtual bool						 ParseInternalVar( const char* name, idTokenParser* src );

	//! Parses a string token from the provided token parser and stores it in the output string.
	void								 ParseString( idTokenParser* src, idStr& out );

	//! Parses a 4-dimensional vector from a token parser and stores the result in the provided vector.
	void								 ParseVec4( idTokenParser* src, idVec4& out );
	void								 ConvertRegEntry( const char* name, idTokenParser* src, idStr& out, int tabs );

	float								 actualX;	  // physical coords
	float								 actualY;	  // ''
	int									 childID;	  // this childs id
	unsigned int						 flags;		  // visible, focus, mouseover, cursor, border, etc..
	int									 lastTimeRun; //
	idRectangle							 drawRect;	  // overall rect
	idRectangle							 clientRect;  // client area
	idVec2								 origin;

	int									 timeLine; // time stamp used for various fx
	float								 xOffset;
	float								 yOffset;
	float								 forceAspectWidth;
	float								 forceAspectHeight;
	float								 matScalex;
	float								 matScaley;
	float								 borderSize;
	float								 textAlignx;
	float								 textAligny;
	idStr								 name;
	idStr								 comment;
	idVec2								 shear;

	class idFont*						 font;
	signed char							 textShadow;
	unsigned char						 cursor; //
	signed char							 textAlign;

	idWinBool							 noTime;  //
	idWinBool							 visible; //
	idWinBool							 noEvents;
	idWinRectangle						 rect; // overall rect
	idWinVec4							 backColor;
	idWinVec4							 matColor;
	idWinVec4							 foreColor;
	idWinVec4							 hoverColor;
	idWinVec4							 borderColor;
	idWinFloat							 textScale;
	idWinFloat							 rotate;
	idWinStr							 text;
	idWinBackground						 backGroundName; //

	idList<idWinVar*, TAG_OLD_UI>		 definedVars;
	idList<idWinVar*, TAG_OLD_UI>		 updateVars;

	idRectangle							 textRect;	 // text extented rect
	const idMaterial*					 background; // background asset

	idWindow*							 parent;   // parent window
	idList<idWindow*, TAG_OLD_UI>		 children; // child windows
	idList<drawWin_t, TAG_OLD_UI>		 drawWindows;

	idWindow*							 focusedChild; // if a child window has the focus
	idWindow*							 captureChild; // if a child window has mouse capture
	idWindow*							 overChild;	   // if a child window has mouse capture
	bool								 hover;

	idUserInterfaceLocal*				 gui;

	static idCVar						 gui_debug;
	static idCVar						 gui_edit;

	idGuiScriptList*					 scripts[SCRIPT_COUNT];
	bool*								 saveTemps;

	idList<idTimeLineEvent*, TAG_OLD_UI> timeLineEvents;
	idList<idTransitionData, TAG_OLD_UI> transitions;

	static bool							 registerIsTemporary[MAX_EXPRESSION_REGISTERS]; // statics to assist during parsing

	idList<wexpOp_t, TAG_OLD_UI>		 ops; // evaluate to make expressionRegisters
	idList<float, TAG_OLD_UI>			 expressionRegisters;
	idList<wexpOp_t, TAG_OLD_UI>*		 saveOps;	  // evaluate to make expressionRegisters
	idList<rvNamedEvent*, TAG_OLD_UI>	 namedEvents; //  added named events
	idList<float, TAG_OLD_UI>*			 saveRegs;

	idRegisterList						 regList;

	idWinBool							 hideCursor;
};

ID_INLINE void idWindow::AddDefinedVar( idWinVar* var )
{
	definedVars.AddUnique( var );
}

#endif /* !__WINDOW_H__ */
