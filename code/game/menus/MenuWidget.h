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
#ifndef __MENU_WIDGET_H__
#define __MENU_WIDGET_H__

class idMenuHandler;
class idMenuWidget;

enum menuOption_t { OPTION_INVALID = -1, OPTION_BUTTON_TEXT, OPTION_SLIDER_BAR, OPTION_SLIDER_TEXT, OPTION_SLIDER_TOGGLE, OPTION_BUTTON_INFO, OPTION_BUTTON_FULL_TEXT_SLIDER, MAX_MENU_OPTION_TYPES };

enum widgetEvent_t {
	WIDGET_EVENT_PRESS,
	WIDGET_EVENT_RELEASE,
	WIDGET_EVENT_ROLL_OVER,
	WIDGET_EVENT_ROLL_OUT,
	WIDGET_EVENT_FOCUS_ON,
	WIDGET_EVENT_FOCUS_OFF,

	WIDGET_EVENT_SCROLL_UP_LSTICK,
	WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE,
	WIDGET_EVENT_SCROLL_DOWN_LSTICK,
	WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE,
	WIDGET_EVENT_SCROLL_LEFT_LSTICK,
	WIDGET_EVENT_SCROLL_LEFT_LSTICK_RELEASE,
	WIDGET_EVENT_SCROLL_RIGHT_LSTICK,
	WIDGET_EVENT_SCROLL_RIGHT_LSTICK_RELEASE,

	WIDGET_EVENT_SCROLL_UP_RSTICK,
	WIDGET_EVENT_SCROLL_UP_RSTICK_RELEASE,
	WIDGET_EVENT_SCROLL_DOWN_RSTICK,
	WIDGET_EVENT_SCROLL_DOWN_RSTICK_RELEASE,
	WIDGET_EVENT_SCROLL_LEFT_RSTICK,
	WIDGET_EVENT_SCROLL_LEFT_RSTICK_RELEASE,
	WIDGET_EVENT_SCROLL_RIGHT_RSTICK,
	WIDGET_EVENT_SCROLL_RIGHT_RSTICK_RELEASE,

	WIDGET_EVENT_SCROLL_UP,
	WIDGET_EVENT_SCROLL_UP_RELEASE,
	WIDGET_EVENT_SCROLL_DOWN,
	WIDGET_EVENT_SCROLL_DOWN_RELEASE,
	WIDGET_EVENT_SCROLL_LEFT,
	WIDGET_EVENT_SCROLL_LEFT_RELEASE,
	WIDGET_EVENT_SCROLL_RIGHT,
	WIDGET_EVENT_SCROLL_RIGHT_RELEASE,

	WIDGET_EVENT_DRAG_START,
	WIDGET_EVENT_DRAG_STOP,

	WIDGET_EVENT_SCROLL_PAGEDWN,
	WIDGET_EVENT_SCROLL_PAGEDWN_RELEASE,
	WIDGET_EVENT_SCROLL_PAGEUP,
	WIDGET_EVENT_SCROLL_PAGEUP_RELEASE,

	WIDGET_EVENT_SCROLL,
	WIDGET_EVENT_SCROLL_RELEASE,
	WIDGET_EVENT_BACK,
	WIDGET_EVENT_COMMAND,
	WIDGET_EVENT_TAB_NEXT,
	WIDGET_EVENT_TAB_PREV,
	MAX_WIDGET_EVENT
};

enum scrollType_t {
	SCROLL_SINGLE, // scroll a single unit
	SCROLL_PAGE,   // scroll a page
	SCROLL_FULL,   // scroll all the way to the end
	SCROLL_TOP,	   // scroll to the first selection
	SCROLL_END,	   // scroll to the last selection
};

enum widgetAction_t {
	WIDGET_ACTION_NONE,
	WIDGET_ACTION_COMMAND,
	WIDGET_ACTION_FUNCTION,		   // call the SWF function
	WIDGET_ACTION_SCROLL_VERTICAL, // scroll something. takes one param = amount to scroll (can be negative)
	WIDGET_ACTION_SCROLL_VERTICAL_VARIABLE,
	WIDGET_ACTION_SCROLL_PAGE,
	WIDGET_ACTION_SCROLL_HORIZONTAL, // scroll something. takes one param = amount to scroll (can be negative)
	WIDGET_ACTION_SCROLL_TAB,
	WIDGET_ACTION_START_REPEATER,
	WIDGET_ACTION_STOP_REPEATER,
	WIDGET_ACTION_ADJUST_FIELD,
	WIDGET_ACTION_PRESS_FOCUSED,
	WIDGET_ACTION_JOY3_ON_PRESS,
	WIDGET_ACTION_JOY4_ON_PRESS,
	//
	WIDGET_ACTION_GOTO_MENU,
	WIDGET_ACTION_GO_BACK,
	WIDGET_ACTION_EXIT_GAME,
	WIDGET_ACTION_LAUNCH_MULTIPLAYER,
	WIDGET_ACTION_MENU_BAR_SELECT,
	WIDGET_ACTION_EMAIL_HOVER,
	// PDA USER DATA ACTIONS
	WIDGET_ACTION_PDA_SELECT_USER,
	WIDGET_ACTION_SELECT_GAMERTAG,
	WIDGET_ACTION_PDA_SELECT_NAV,
	WIDGET_ACTION_SELECT_PDA_AUDIO,
	WIDGET_ACTION_SELECT_PDA_VIDEO,
	WIDGET_ACTION_SELECT_PDA_ITEM,
	WIDGET_ACTION_SCROLL_DRAG,
	// PDA EMAIL ACTIONS
	WIDGET_ACTION_PDA_SELECT_EMAIL,
	WIDGET_ACTION_PDA_CLOSE,
	WIDGET_ACTION_REFRESH,
	WIDGET_ACTION_MUTE_PLAYER,
	MAX_WIDGET_ACTION
};

enum actionHandler_t {
	WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER,
	WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER_VARIABLE,
	WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER,
	WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER_VARIABLE,
	WIDGET_ACTION_EVENT_SCROLL_LEFT_START_REPEATER,
	WIDGET_ACTION_EVENT_SCROLL_RIGHT_START_REPEATER,
	WIDGET_ACTION_EVENT_SCROLL_PAGE_DOWN_START_REPEATER,
	WIDGET_ACTION_EVENT_SCROLL_PAGE_UP_START_REPEATER,
	WIDGET_ACTION_EVENT_STOP_REPEATER,
	WIDGET_ACTION_EVENT_TAB_NEXT,
	WIDGET_ACTION_EVENT_TAB_PREV,
	WIDGET_ACTION_EVENT_DRAG_START,
	WIDGET_ACTION_EVENT_DRAG_STOP,
	WIDGET_ACTION_EVENT_JOY3_ON_PRESS,
};

struct widgetTransition_t {
	//! Initializes a widget transition with no animation name.
	widgetTransition_t() :
		animationName( NULL )
	{
	}

	const char*					 animationName; // name of the animation to run
	idStaticList<const char*, 4> prefixes;		// prefixes to try to use for animation
};

/*
================================================
scoreboardInfo_t
================================================
*/
struct scoreboardInfo_t {
	//! Initializes a scoreboardInfo_t object with default values.
	scoreboardInfo_t() :
		index( -1 ),
		voiceState( VOICECHAT_DISPLAY_NONE )
	{
	}

	idList<idStr, TAG_IDLIB_LIST_MENU> values;
	int								   index;
	voiceStateDisplay_t				   voiceState;
};

/*!
	\class idSort_SavesByDate
	\brief A sorting implementation for save game details ordered by date in descending order.
*/
class idSort_SavesByDate : public idSort_Quick<idSaveGameDetails, idSort_SavesByDate>
{
public:
	//! Compares two save game details by their date values in descending order.
	int Compare( const idSaveGameDetails& a, const idSaveGameDetails& b ) const { return b.date - a.date; }
};

/*!
	\class idMenuDataSource
	\brief Abstract base class for data sources used by menu systems.
*/
class idMenuDataSource
{
public:
	virtual ~idMenuDataSource() { }

	virtual void		   LoadData()												   = 0;
	virtual void		   CommitData()												   = 0;
	virtual bool		   IsDataChanged() const									   = 0;
	virtual idSWFScriptVar GetField( const int fieldIndex ) const					   = 0;
	virtual void		   AdjustField( const int fieldIndex, const int adjustAmount ) = 0;
};

/*!
	\class idWidgetEvent
	\brief Provides a container for widget events with associated type, argument, script object, and parameters.
*/
class idWidgetEvent
{
public:
	//! Initializes a new instance of the idWidgetEvent class with default values.
	idWidgetEvent() :
		type( WIDGET_EVENT_PRESS ),
		arg( 0 ),
		thisObject( NULL )
	{
	}

	/*!
		\brief Constructs an idWidgetEvent object with the specified type, argument, this object, and parameter list.

		The constructor initializes the event object with the provided widget event type, argument, script object, and parameter list. It sets up the internal members to store the event data for later
	   processing in the widget system.

		\param type_ The type of widget event being created
		\param arg_ An integer argument associated with the event
		\param thisObject_ Pointer to the SWF script object that this event is associated with
		\param parms_ The list of parameters for the event
	*/
	idWidgetEvent( const widgetEvent_t type_, const int arg_, idSWFScriptObject* thisObject_, const idSWFParmList& parms_ ) :
		type( type_ ),
		arg( arg_ ),
		thisObject( thisObject_ ),
		parms( parms_ )
	{
	}

	widgetEvent_t	   type;
	int				   arg;
	idSWFScriptObject* thisObject;
	idSWFParmList	   parms;
};

/*!
	\class idWidgetAction
	\brief Represents a widget action with associated script function and parameters for execution.

	The idWidgetAction class encapsulates a widget action including its type, associated script function, and parameters. It provides methods for setting and retrieving action properties, managing
   script function references, and handling parameter lists. The class supports copy construction, assignment, and comparison operations to facilitate safe usage in collections and as function
   parameters. The script function reference is properly managed through reference counting, and parameters can be set with up to four script variables. The class is designed to be lightweight and
   efficient for use in widget-based user interfaces where actions need to be stored, transmitted, and executed.

*/
class idWidgetAction
{
public:
	//! Initializes a new instance of the idWidgetAction class with default values.
	idWidgetAction() :
		action( WIDGET_ACTION_NONE ),
		scriptFunction( NULL )
	{
	}

	//! Copies the state of another widget action object.
	idWidgetAction( const idWidgetAction& src )
	{
		action		   = src.action;
		parms		   = src.parms;
		scriptFunction = src.scriptFunction;
		if( scriptFunction != NULL ) { scriptFunction->AddRef(); }
	}

	//! Destroys the widget action and releases its associated script function if it exists.
	~idWidgetAction()
	{
		if( scriptFunction != NULL ) { scriptFunction->Release(); }
	}

	//! Assigns the contents of another idWidgetAction instance to this instance
	void operator=( const idWidgetAction& src )
	{
		action		   = src.action;
		parms		   = src.parms;
		scriptFunction = src.scriptFunction;
		if( scriptFunction != NULL ) { scriptFunction->AddRef(); }
	}

	//! Compares this widget action with another for equality based on type and parameter values
	bool operator==( const idWidgetAction& otherAction ) const
	{
		if( GetType() != otherAction.GetType() || GetParms().Num() != otherAction.GetParms().Num() ) { return false; }

		// everything else is equal, so check all parms. NOTE: this assumes we are only sending
		// integral types.
		for( int i = 0; i < GetParms().Num(); ++i ) {
			if( GetParms()[i].GetType() != otherAction.GetParms()[i].GetType() || GetParms()[i].ToInteger() != otherAction.GetParms()[i].ToInteger() ) { return false; }
		}

		return true;
	}

	//! Sets the script function for this widget action and manages its reference count.
	void Set( idSWFScriptFunction* function )
	{
		action = WIDGET_ACTION_FUNCTION;
		if( scriptFunction != NULL ) { scriptFunction->Release(); }
		scriptFunction = function;
		scriptFunction->AddRef();
	}

	//! Sets the widget action and clears associated parameters
	void Set( widgetAction_t action_ )
	{
		action = action_;
		parms.Clear();
	}

	//! Sets the action and parameters for a widget action
	void Set( widgetAction_t action_, const idSWFScriptVar& var1 )
	{
		action = action_;
		parms.Clear();
		parms.Append( var1 );
	}

	//! Sets the widget action and its parameters.
	void Set( widgetAction_t action_, const idSWFScriptVar& var1, const idSWFScriptVar& var2 )
	{
		action = action_;
		parms.Clear();
		parms.Append( var1 );
		parms.Append( var2 );
	}

	/*!
		\brief Initializes the widget action with a specified action type and up to three parameters

		Sets the widget action type and appends up to three script variables to the parameter list. The parameter list is first cleared before appending the new parameters.

		\param action_ The widget action type to set
		\param var1 First script variable parameter
		\param var2 Second script variable parameter
		\param var3 Third script variable parameter
	*/
	void Set( widgetAction_t action_, const idSWFScriptVar& var1, const idSWFScriptVar& var2, const idSWFScriptVar& var3 )
	{
		action = action_;
		parms.Clear();
		parms.Append( var1 );
		parms.Append( var2 );
		parms.Append( var3 );
	}

	/*!
		\brief Initializes the widget action with the specified action type and up to four parameters

		Sets the action type and appends up to four script variables as parameters to the action. The parameters are stored in a collection for later execution or processing within the widget system.
	   This method clears any existing parameters before adding the new ones.

		\param action_ The widget action type to be set
		\param var1 First script variable parameter
		\param var2 Second script variable parameter
		\param var3 Third script variable parameter
		\param var4 Fourth script variable parameter
	*/
	void Set( widgetAction_t action_, const idSWFScriptVar& var1, const idSWFScriptVar& var2, const idSWFScriptVar& var3, const idSWFScriptVar& var4 )
	{
		action = action_;
		parms.Clear();
		parms.Append( var1 );
		parms.Append( var2 );
		parms.Append( var3 );
		parms.Append( var4 );
	}

	//! Returns the script function associated with this widget action.
	idSWFScriptFunction* GetScriptFunction() { return scriptFunction; }

	//! Returns the type of the widget action stored in the object.
	const widgetAction_t GetType() const { return action; }

	//! Returns a constant reference to the parameter list associated with the widget action.
	const idSWFParmList& GetParms() const { return parms; }

private:
	widgetAction_t		 action;
	idSWFParmList		 parms;
	idSWFScriptFunction* scriptFunction;
};

typedef idList<idMenuWidget*, TAG_IDLIB_LIST_MENU> idMenuWidgetList;

/*!
	\class idMenuWidget
	\brief Base class for menu widgets implementing a model/view architecture with SWF integration.

	This class serves as the foundation for menu widgets in a model/view architecture, combining both the data model and visual representation aspects. It manages the widget's lifecycle, state, and
   event handling while maintaining relationships with parent and child widgets. The class integrates with SWF-based visual elements through sprite binding and provides mechanisms for observing
   events, handling user interactions, and managing data sources. It supports reference counting for resource management and maintains a hierarchy of widgets through parent-child relationships. The
   widget system is designed to work with a menu handler for configuration and data binding, and it provides methods for managing focus, visibility, and animation states.

*/
class idMenuWidget
{
public:
	/*!
		\class idMenuWidget::WrapWidgetSWFEvent
		\brief A wrapper class for handling SWF widget events in a menu system.
	*/
	class WrapWidgetSWFEvent : public idSWFScriptFunction_RefCounted
	{
	public:
		//! Initializes a WrapWidgetSWFEvent object with the specified widget, event, and event argument.
		WrapWidgetSWFEvent( idMenuWidget* widget, const widgetEvent_t event, const int eventArg ) :
			targetWidget( widget ),
			targetEvent( event ),
			targetEventArg( eventArg )
		{
		}

		//! Executes a script function call with the specified object and parameters, though this implementation should never be reached.
		idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
		{
			targetWidget->ReceiveEvent( idWidgetEvent( targetEvent, targetEventArg, thisObject, parms ) );
			return idSWFScriptVar();
		}

	private:
		idMenuWidget* targetWidget;
		widgetEvent_t targetEvent;
		int			  targetEventArg;
	};

	enum widgetState_t {
		WIDGET_STATE_HIDDEN,	// hidden
		WIDGET_STATE_NORMAL,	// normal
		WIDGET_STATE_SELECTING, // going into the selected state
		WIDGET_STATE_SELECTED,	// fully selected
		WIDGET_STATE_DISABLED,	// disabled
		WIDGET_STATE_MAX
	};

	//! Constructs a new idMenuWidget object with default initial values.
	idMenuWidget();

	//! Destructor for the idMenuWidget class that cleans up resources.
	virtual ~idMenuWidget();

	//! Releases all observer and child widgets and clears their respective lists.
	void				 Cleanup();

	//! Initializes the menu widget with the provided menu handler data.
	virtual void		 Initialize( idMenuHandler* data ) { menuData = data; }

	//! Updates the widget's state.
	virtual void		 Update() { }

	//! Displays the menu widget by making it visible and playing the roll-on animation.
	virtual void		 Show();

	//! Hides the menu widget by playing a rollout animation frame.
	virtual void		 Hide();

	//! Returns the current state of the widget.
	widgetState_t		 GetState() const { return widgetState; }

	//! Sets the state of the menu widget and updates its visual representation accordingly.
	void				 SetState( const widgetState_t state );

	//! Returns the SWF sprite instance bound to this menu widget.
	idSWFSpriteInstance* GetSprite() { return boundSprite; }

	//! Returns the SWF object associated with this menu widget, searching through parent widgets and menu data if necessary.
	idSWF*				 GetSWFObject();

	//! Returns the menu data associated with this widget or its parent
	idMenuHandler*		 GetMenuData();

	//! Binds a sprite to the widget using the sprite path from the root SWF script object.
	bool				 BindSprite( idSWFScriptObject& root );

	//! Clears the sprite associated with the menu widget.
	void				 ClearSprite();

	/*!
		\brief Sets the sprite path for the menu widget by concatenating multiple string arguments.

		This function constructs a sprite path by appending non-null string arguments passed to it. It accepts up to five string arguments and builds a single path string by sequentially adding each
	   argument until a NULL argument is encountered. The function clears any existing sprite path data before starting the construction process.

		\param arg1 First string component of the sprite path
		\param arg2 Second string component of the sprite path
		\param arg3 Third string component of the sprite path
		\param arg4 Fourth string component of the sprite path
		\param arg5 Fifth string component of the sprite path
	*/
	void				 SetSpritePath( const char* arg1, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL );

	/*!
		\brief Sets the sprite path for a menu widget using a list of strings and optional arguments.

		This function initializes the sprite path with the provided list of strings and appends any additional arguments that are not null. It handles up to five optional arguments, appending them to
	   the sprite path list until a null argument is encountered.

		\param spritePath_ The initial list of strings forming the base sprite path.
		\param arg1 Optional first argument to append to the sprite path.
		\param arg2 Optional second argument to append to the sprite path.
		\param arg3 Optional third argument to append to the sprite path.
		\param arg4 Optional fourth argument to append to the sprite path.
		\param arg5 Optional fifth argument to append to the sprite path.
	*/
	void				 SetSpritePath( const idList<idStr>& spritePath_, const char* arg1 = NULL, const char* arg2 = NULL, const char* arg3 = NULL, const char* arg4 = NULL, const char* arg5 = NULL );

	//! Returns the sprite path list for the menu widget.
	idList<idStr, TAG_IDLIB_LIST_MENU>& GetSpritePath() { return spritePath; }

	//! Returns the current reference count for the menu widget.
	int									GetRefCount() const { return refCount; }

	//! Increments the reference count of the menu widget.
	void								AddRef() { refCount++; }

	//! Releases the object's resources and performs cleanup.
	void								Release()
	{
		assert( refCount > 0 );
		if( --refCount == 0 && !noAutoFree ) { delete this; }
	}

	/*!
		\brief Handles widget actions by delegating to parent or menu data handlers

		This function processes widget actions by first checking if the widget has a parent and delegating the handling to the parent if available. If there is no parent, it checks if forceHandled is
	   true and returns false in that case. Otherwise, it retrieves the menu data and delegates the action handling to it. The function returns whether the action was handled by the parent or menu
	   data handlers.

		\param action Reference to the widget action to be handled
		\param event Reference to the widget event associated with the action
		\param widget Pointer to the widget that triggered the action
		\param forceHandled Boolean flag indicating whether to force handling regardless of parent
		\return Boolean value indicating whether the action was handled by this function or its parent/menu data
	*/
	virtual bool								 HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles observed events from menu widgets.
	virtual void								 ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event ) { }

	//! Sends an event to all registered observers.
	void										 SendEventToObservers( const idWidgetEvent& event );

	//! Registers an event observer for this menu widget
	void										 RegisterEventObserver( idMenuWidget* observer );

	//! Handles a widget event by propagating it through the focus chain
	void										 ReceiveEvent( const idWidgetEvent& event );

	//! Executes an event in the context of this widget by processing associated actions and notifying observers
	virtual bool								 ExecuteEvent( const idWidgetEvent& event );

	//! Returns the list of actions for a given event, or NULL if no actions are registered for that event.
	idList<idWidgetAction, TAG_IDLIB_LIST_MENU>* GetEventActions( const widgetEvent_t eventType );

	//! Allocates an action for the given event type.
	idWidgetAction&								 AddEventAction( const widgetEvent_t eventType );

	//! Clears all event actions and resets the event action lookup table.
	void										 ClearEventActions();

	//! Sets the data source and field index for the menu widget.
	void										 SetDataSource( idMenuDataSource* dataSource, const int fieldIndex );

	//! Returns the data source associated with this menu widget.
	idMenuDataSource*							 GetDataSource() { return dataSource; }

	//! Sets the data source field index for the menu widget.
	void										 SetDataSourceFieldIndex( const int dataSourceFieldIndex_ ) { dataSourceFieldIndex = dataSourceFieldIndex_; }

	//! Returns the index of the data source field.
	int											 GetDataSourceFieldIndex() const { return dataSourceFieldIndex; }

	//! Returns the menu widget that currently has focus, or NULL if no widget has focus.
	idMenuWidget*								 GetFocus() { return ( focusIndex >= 0 && focusIndex < children.Num() ) ? children[focusIndex] : NULL; }

	//! Returns the index of the currently focused widget in the menu.
	int											 GetFocusIndex() const { return focusIndex; }

	//! Sets the focus index for a menu widget and handles focus events and sound playback.
	void										 SetFocusIndex( const int index, bool skipSound = false );

	//! Returns a reference to the list of child widgets.
	idMenuWidgetList&							 GetChildren() { return children; }

	//! Returns a constant reference to the list of child widgets.
	const idMenuWidgetList&						 GetChildren() const { return children; }

	//! Returns a reference to the child widget at the specified index.
	idMenuWidget&								 GetChildByIndex( const int index ) const { return *children[index]; }

	//! Adds a child widget to this widget's child list
	void										 AddChild( idMenuWidget* widget );

	//! Removes a child widget from this widget's hierarchy.
	void										 RemoveChild( idMenuWidget* widget );

	//! Checks if the specified widget is a child of this menu widget.
	bool										 HasChild( idMenuWidget* widget );

	//! Removes all child widgets from this menu widget.
	void										 RemoveAllChildren();

	//! Returns the parent widget of this menu widget.
	idMenuWidget*								 GetParent() { return parent; }

	//! Returns the parent widget of this menu widget.
	const idMenuWidget*							 GetParent() const { return parent; }

	//! Sets the parent widget for this menu widget.
	void										 SetParent( idMenuWidget* parent_ ) { parent = parent_; }

	//! Sets the SWF object associated with this menu widget.
	void										 SetSWFObj( idSWF* obj ) { swfObj = obj; }

	//! Returns whether the widget handler is a parent.
	bool										 GetHandlerIsParent() { return handlerIsParent; }

	//! Sets the handler is parent flag to the specified boolean value.
	void										 SetHandlerIsParent( bool val ) { handlerIsParent = val; }

	//! Sets whether the widget should not be automatically freed.
	void										 SetNoAutoFree( bool b ) { noAutoFree = b; }

protected:
	//! Sets the focus index to the specified value.
	void ForceFocusIndex( const int index ) { focusIndex = index; }

protected:
	bool																	 handlerIsParent;
	idMenuHandler*															 menuData;
	idSWF*																	 swfObj;
	idSWFSpriteInstance*													 boundSprite;
	idMenuWidget*															 parent;
	idList<idStr, TAG_IDLIB_LIST_MENU>										 spritePath;
	idMenuWidgetList														 children;
	idMenuWidgetList														 observers;

	static const int														 INVALID_ACTION_INDEX = -1;
	idList<idList<idWidgetAction, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> eventActions;
	idStaticList<int, MAX_WIDGET_EVENT>										 eventActionLookup;

	idMenuDataSource*														 dataSource;
	int																		 dataSourceFieldIndex;

	int																		 focusIndex;

	widgetState_t															 widgetState;
	int																		 refCount;
	bool																	 noAutoFree;
};

/*!
	\class idMenuWidget_Button
	\brief A button widget that supports label, image, description, and animation state management with event handling capabilities.

	This widget extends the base menu widget functionality to provide a button with configurable label, image, and description properties. It manages animation states and supports event handling
   through execution of associated actions. The widget can be configured with custom behavior on press and handles state transitions for animations. It is designed to work within a menu system and
   provides methods for updating its visual representation, managing its state, and processing user interactions. The implementation supports multiple values and can be configured to ignore color
   settings. The widget maintains a reference to an image material and can be set up for animated transitions between different visual states.

*/
class idMenuWidget_Button : public idMenuWidget
{
public:
	enum animState_t {
		ANIM_STATE_UP,	 // standard
		ANIM_STATE_DOWN, // pressed down
		ANIM_STATE_OVER, // hovered over this
		ANIM_STATE_MAX
	};

	//! Constructs a new button widget with default animation state and null image pointer.
	idMenuWidget_Button() :
		animState( ANIM_STATE_UP ),
		img( NULL ),
		ignoreColor( false )
	{
	}

	virtual ~idMenuWidget_Button() { }

	//! Processes widget events such as presses, releases, and hover states for the button widget.
	virtual bool	  ExecuteEvent( const idWidgetEvent& event );

	//! Updates the button widget's visual state and event bindings in the GUI.
	virtual void	  Update();

	//! Sets the button label to the specified string.
	void			  SetLabel( const idStr& label ) { btnLabel = label; }

	//! Returns the label string of the button widget.
	const idStr&	  GetLabel() const { return btnLabel; }

	//! Sets the button values from a list of strings.
	void			  SetValues( idList<idStr>& list );

	//! Returns the value at the specified index from the button's value collection.
	const idStr&	  GetValue( int index ) const;

	//! Sets the image material for the button widget.
	void			  SetImg( const idMaterial* val ) { img = val; }

	//! Returns the material image associated with the button widget.
	const idMaterial* GetImg() { return img; }

	//! Sets the description text for the button widget.
	void			  SetDescription( const char* desc_ ) { description = desc_; }

	//! Returns the description string for this menu button widget
	const idStr&	  GetDescription() const { return description; }

	//! Sets the ignore color flag for the button widget.
	void			  SetIgnoreColor( const bool b ) { ignoreColor = b; }

	//! Returns the animation state of the button widget.
	animState_t		  GetAnimState() const { return animState; }

	//! Sets the animation state of the button widget to the specified value.
	void			  SetAnimState( const animState_t state ) { animState = state; }

	//! Sets the function to be called when the button is pressed.
	void			  SetOnPressFunction( idSWFScriptFunction* func ) { scriptFunction = func; }

protected:
	/*!
		\brief Sets up transition information for a button widget based on the current button state and animation states

		This function configures the animation transition properties for a button widget by determining the appropriate animation name and prefixes based on the button's state and the source and
	   destination animation states. When the button is disabled, it uses a specific "disabled" animation name. Otherwise, it computes an animation index using the destination and source animation
	   states to look up the transition in a predefined table. If the button is in selecting state, it appends a "sel_" prefix to the animation name. The function ensures that an empty string is
	   appended as a final prefix to complete the transition setup.

		\param trans Transition information structure to be set up
		\param buttonState Current state of the button widget
		\param sourceAnimState Source animation state for the transition
		\param destAnimState Destination animation state for the transition
	*/
	void							   SetupTransitionInfo( widgetTransition_t& trans, const widgetState_t buttonState, const animState_t sourceAnimState, const animState_t destAnimState ) const;

	//! Animates the button from its current state to the specified target state.
	void							   AnimateToState( const animState_t targetState, const bool force = false );

	idList<idStr, TAG_IDLIB_LIST_MENU> values;
	idStr							   btnLabel;
	idStr							   description;
	animState_t						   animState;
	const idMaterial*				   img;
	idSWFScriptFunction*			   scriptFunction;
	bool							   ignoreColor;
};

/*!
	\class idMenuWidget_LobbyButton
	\brief A UI widget representing a lobby button with voice state display capabilities.
*/
class idMenuWidget_LobbyButton : public idMenuWidget_Button
{
public:
	//! Initializes a new instance of the idMenuWidget_LobbyButton class.
	idMenuWidget_LobbyButton() :
		voiceState( VOICECHAT_DISPLAY_NONE )
	{
	}

	//! Updates the lobby button's display and event handlers in the UI
	virtual void Update();

	//! Sets the button information including the name and voice state.
	void		 SetButtonInfo( idStr name_, voiceStateDisplay_t voiceState_ );

	//! Returns true if the lobby button has a valid name
	bool		 IsValid() { return !name.IsEmpty(); }

protected:
	idStr				name;
	voiceStateDisplay_t voiceState;
};

/*!
	\class idMenuWidget_ScoreboardButton
	\brief Represents a button widget for displaying player information in a scoreboard interface.
*/
class idMenuWidget_ScoreboardButton : public idMenuWidget_Button
{
public:
	//! Initializes a new instance of the ScoreboardButton widget with default voice state and index values.
	idMenuWidget_ScoreboardButton() :
		voiceState( VOICECHAT_DISPLAY_NONE ),
		index( -1 )
	{
	}

	//! Updates the scoreboard button's display and interactive elements
	virtual void Update();

	//! Sets the button information including index, voice state, and values from a list.
	void		 SetButtonInfo( int index_, idList<idStr>& list, voiceStateDisplay_t voiceState_ );

protected:
	voiceStateDisplay_t voiceState;
	int					index;
};

/*!
	\class idMenuWidget_ControlButton
	\brief A specialized button widget designed to handle control options with specific visual states and user interaction handling.
*/
class idMenuWidget_ControlButton : public idMenuWidget_Button
{
public:
	//! Initializes a new instance of the control button widget with default option type and disabled state.
	idMenuWidget_ControlButton() :
		optionType( OPTION_BUTTON_TEXT ),
		disabled( false )
	{
	}

	//! Updates the control button widget by setting its visual state and handling user interactions.
	virtual void Update();

	//! Sets the option type for the control button widget.
	void		 SetOptionType( const menuOption_t type ) { optionType = type; }

	//! Returns the option type of the control button.
	menuOption_t GetOptionType() const { return optionType; }

	//! Configures event handlers for scroll actions on a control button widget.
	void		 SetupEvents( int delay, int index );

	//! Sets the disabled state of the control button.
	void		 SetDisabled( bool disable ) { disabled = disable; }

protected:
	menuOption_t optionType;
	bool		 disabled;
};

/*!
	\class idMenuWidget_ServerButton
	\brief A UI widget for displaying and managing server button interactions in a networked game environment.
*/
class idMenuWidget_ServerButton : public idMenuWidget_Button
{
public:
	//! Constructs a new server button widget with default values.
	idMenuWidget_ServerButton() :
		index( 0 ),
		players( 0 ),
		maxPlayers( 0 ),
		joinable( false ),
		validMap( false )
	{
	}

	//! Updates the server button widget in the UI by setting its text and event handlers
	virtual void Update();

	/*!
		\brief Sets the button information for a server button widget.

		This function configures the server button widget with various server details including name, map name, mode name, player counts, and joinability status. It also constructs a descriptive
	   string that combines player information, map name, and mode name to be displayed in the button.

		\param name_ The name of the server
		\param mapName_ The map name identifier for the server
		\param modeName_ The game mode name for the server
		\param index_ The index of the server in the list
		\param players_ The current number of players on the server
		\param maxPlayers_ The maximum number of players allowed on the server
		\param joinable_ Indicates if the server is joinable
		\param validMap_ Indicates if the map is valid for the server
	*/
	void		 SetButtonInfo( idStr name_, idStrId mapName_, idStr modeName_, int index_ = 0, int players_ = 0, int maxPlayers_ = 0, bool joinable_ = false, bool validMap_ = false );

	//! Checks if the server button widget has a valid server name.
	bool		 IsValid() { return !serverName.IsEmpty(); }

	//! Returns true if the server is joinable and has a valid map.
	bool		 CanJoin() { return ( joinable && validMap ); }

protected:
	idStr	serverName;
	int		index;
	int		players;
	int		maxPlayers;
	bool	joinable;
	bool	validMap;
	idStrId mapName;
	idStr	modeName;
};

/*!
	\class idMenuWidget_NavButton
	\brief A navigation button widget that handles navigation-specific events and positioning within a menu system.
*/
class idMenuWidget_NavButton : public idMenuWidget_Button
{
public:
	enum navWidgetState_t {
		NAV_WIDGET_LEFT,	// option on left side
		NAV_WIDGET_RIGHT,	// option on right side
		NAV_WIDGET_SELECTED // option is selected
	};

	//! Initializes a new instance of the idMenuWidget_NavButton class with default values for navIndex and xPos.
	idMenuWidget_NavButton() :
		navIndex( 0 ),
		xPos( 0 )
	{
	}

	//! Processes widget events for a navigation button, handling press and roll-over events while delegating to the base class.
	virtual bool ExecuteEvent( const idWidgetEvent& event );

	//! Updates the navigation button's display and input handling in the menu system.
	virtual void Update();

	//! Sets the navigation index and state for the nav button widget.
	void		 SetNavIndex( int i, const navWidgetState_t type )
	{
		navIndex = i;
		navState = type;
	}

	//! Sets the x position of the navigation button to the specified value.
	void SetPosition( float pos ) { xPos = pos; }

private:
	int				 navIndex;
	float			 xPos;
	navWidgetState_t navState;
};

/*!
	\class idMenuWidget_MenuButton
	\brief A menu button widget that extends button functionality with navigation capabilities.
*/
class idMenuWidget_MenuButton : public idMenuWidget_Button
{
public:
	//! Initializes a new instance of the idMenuWidget_MenuButton class with default values.
	idMenuWidget_MenuButton() :
		xPos( 0 )
	{
	}

	//! Updates the menu button's visual state and text display.
	virtual void Update();

	//! Sets the horizontal position of the menu button to the specified value.
	void		 SetPosition( float pos ) { xPos = pos; }

private:
	float xPos;
};

/*!
	\class idMenuWidget_List
	\brief A widget that displays a scrolling list of elements with configurable visibility and navigation.

	This class implements a scrolling list widget that manages a collection of child widgets, displaying only a subset of them at any given time. It provides functionality for navigating through the
   list items using various scrolling methods including single, page, and full scrolls. The widget maintains internal state for view index and offset to determine which items are currently visible. It
   supports wrapping behavior when scrolling past list boundaries and allows configuration of the number of visible options. The class is designed to work with a parent menu system and handles widget
   actions for navigation while delegating unhandled actions to its parent. It provides methods to prepare individual list elements for display and to calculate new positions based on index or offset
   deltas.

   Provides a paged view of this widgets children.  Each child is expected to take on the following
   naming scheme.  Children outside of the given window size (NumVisibleOptions) are not rendered,
   and will affect which type of arrow indicators are shown.

   Future work:
	- Make upIndicator another kind of widget (Image widget?)

*/
class idMenuWidget_List : public idMenuWidget
{
public:
	//! Constructs a new idMenuWidget_List object with default values.
	idMenuWidget_List() :
		numVisibleOptions( 0 ),
		viewOffset( 0 ),
		viewIndex( 0 ),
		allowWrapping( false )
	{
	}

	//! Updates the list widget by binding sprites and updating child elements.
	virtual void Update();

	/*!
		\brief Handles widget actions for vertical scrolling in a list widget.

		This function processes scroll-related widget actions for a list widget. It determines the scroll type from the event argument and performs the appropriate scrolling operation based on whether
	   the scroll is single, page, or full. The function returns true if the action was handled, otherwise it delegates to the parent widget's HandleAction method.

		\param action The widget action to be processed
		\param event The widget event associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Optional parameter to force handling of the action
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles events observed by the list widget.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Updates the view index and offset based on the scroll amount and optional wrap-around behavior.
	virtual void Scroll( const int scrollIndexAmount, const bool wrapAround = false );

	//! Updates the view index and offset based on the provided scroll amount.
	virtual void ScrollOffset( const int scrollIndexAmount );

	//! Returns the total number of options available in the list widget.
	virtual int	 GetTotalNumberOfOptions() const { return GetChildren().Num(); }

	//! Prepares a list element for display.
	virtual bool PrepareListElement( idMenuWidget& widget, const int childIndex ) { return true; }

	//! Returns whether wrapping is allowed for the list widget.
	bool		 IsWrappingAllowed() const { return allowWrapping; }

	//! Sets whether wrapping is allowed for the list widget.
	void		 SetWrappingAllowed( const bool allow ) { allowWrapping = allow; }

	//! Sets the number of visible options for the list widget.
	void		 SetNumVisibleOptions( const int numVisibleOptions_ ) { numVisibleOptions = numVisibleOptions_; }

	//! Returns the number of visible options in the list widget.
	int			 GetNumVisibleOptions() const { return numVisibleOptions; }

	//! Returns the current view offset value.
	int			 GetViewOffset() const { return viewOffset; }

	//! Sets the view offset for the list widget to the specified integer value.
	void		 SetViewOffset( const int offset ) { viewOffset = offset; }

	//! Returns the view index of the list widget.
	int			 GetViewIndex() const { return viewIndex; }

	//! Sets the view index of the list widget to the specified value.
	void		 SetViewIndex( const int index ) { viewIndex = index; }

	/*!
		\brief Calculates a new list index and offset based on a delta movement, handling wrapping behavior and window size constraints

		This function updates the index and offset to reflect movement through a list based on a given delta. It handles both forward and backward movement, applying wrapping logic when allowed. The
	   function ensures that the resulting index and offset maintain a valid window size relationship, where the index is always within the window size of the offset. The wrapping behavior can be
	   either explicit (where the index wraps around to the beginning) or implicit (where the index clamps to the boundary).

		\param outIndex Output parameter for the calculated new index
		\param outOffset Output parameter for the calculated new offset
		\param currentIndex The current index in the list
		\param currentOffset The current offset in the list
		\param windowSize The size of the visible window in the list
		\param maxSize The total number of items in the list
		\param indexDelta The delta value to move through the list
		\param allowWrapping Whether wrapping is allowed when moving beyond list boundaries
		\param wrapAround Whether to perform explicit wrapping when allowed
		\throws assertion failure if indexDelta is zero or if calculated values violate window size constraints
	*/
	void		 CalculatePositionFromIndexDelta( int& outIndex,
				int&								   outOffset,
				const int							   currentIndex,
				const int							   currentOffset,
				const int							   windowSize,
				const int							   maxSize,
				const int							   indexDelta,
				const bool							   allowWrapping,
				const bool							   wrapAround = false ) const;

	/*!
		\brief Updates list display indices and offsets based on a delta value while maintaining window bounds.

		This function calculates new display indices and offsets for a list widget based on a given offset delta. It ensures that the displayed window of items remains within the valid range defined
	   by the maximum size and window size. The function handles edge cases where scrolling would exceed the list boundaries by clamping values appropriately. The calculation considers the current
	   index and offset to determine the new position, ensuring that the index and offset remain within the specified window size constraints.

		\param outIndex Output parameter that will contain the new index value
		\param outOffset Output parameter that will contain the new offset value
		\param currentIndex The current index position in the list
		\param currentOffset The current offset in the list
		\param windowSize The size of the visible window
		\param maxSize The maximum size of the list
		\param offsetDelta The delta value to adjust the offset by
		\throws assertion failure if offsetDelta is zero
	*/
	void CalculatePositionFromOffsetDelta( int& outIndex, int& outOffset, const int currentIndex, const int currentOffset, const int windowSize, const int maxSize, const int offsetDelta ) const;

private:
	int	 numVisibleOptions;
	int	 viewOffset;
	int	 viewIndex;
	bool allowWrapping;
};

/*!
	\class idBrowserEntry_t
	\brief Represents an entry in a file browser interface.
*/
class idBrowserEntry_t
{
public:
	idStr	serverName;
	int		index;
	int		players;
	int		maxPlayers;
	bool	joinable;
	bool	validMap;
	idStrId mapName;
	idStr	modeName;
};

/*!
	\class idMenuWidget_GameBrowserList
	\brief A widget for displaying and managing a list of game servers in a browser interface.
*/
class idMenuWidget_GameBrowserList : public idMenuWidget_List
{
public:
	//! Updates the game browser list widget by binding sprites and preparing list elements for display.
	virtual void Update();

	//! Prepares a list element for display in the game browser list widget.
	virtual bool PrepareListElement( idMenuWidget& widget, const int childIndex );

	//! Returns the total number of game options available in the browser list.
	virtual int	 GetTotalNumberOfOptions() const;

	//! Clears all game entries from the browser list.
	void		 ClearGames();

	/*!
		\brief Adds a game entry to the browser list with specified properties

		This function creates a new browser entry using the provided game details and appends it to the internal list of games. The entry includes server name, index, player counts, joinability
	   status, map validity, and mode information. The function is typically used when populating the game browser with server information fetched from a server query.

		\param name_ Name of the game server
		\param mapName_ Identifier for the map being played
		\param modeName_ Name of the game mode
		\param index_ Index identifier for the server
		\param players_ Current number of players on the server
		\param maxPlayers_ Maximum number of players allowed on the server
		\param joinable_ Flag indicating if the server is joinable
		\param validMap_ Flag indicating if the map is valid for the current game
	*/
	void		 AddGame( idStr name_, idStrId mapName_, idStr modeName_, int index_ = 0, int players_ = 0, int maxPlayers_ = 0, bool joinable_ = false, bool validMap_ = false );

	//! Returns the index of the server at the current view position in the game browser list.
	int			 GetServerIndex();

private:
	idList<idBrowserEntry_t> games;
};

/*!
	\class idMenuWidget_Carousel
	\brief Displays a list of items in a looping carousel pattern.

	This class implements a carousel widget that presents a collection of items in a cyclic manner, allowing users to navigate through the options sequentially. It inherits from idMenuWidget and
   provides specialized functionality for managing the display and interaction of carousel items. The widget supports setting visible options, moving to specific indices, and handling user actions
   through delegation to parent widgets or menu data handlers. It also manages the visual representation of items through list images and controls movement behavior with instant transitions and scroll
   flags. The carousel maintains a view index to track the current visible item and supports looping behavior through the movement operations.

*/
class idMenuWidget_Carousel : public idMenuWidget
{
public:
	//! Constructs an idMenuWidget_Carousel object with default values for its member variables.
	idMenuWidget_Carousel() :
		numVisibleOptions( 0 ),
		viewIndex( 0 ),
		moveToIndex( 0 ),
		moveDiff( 0 ),
		fastScroll( false ),
		scrollLeft( false )
	{
	}

	//! Initializes the carousel widget with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the carousel widget by binding sprites and setting up the visible options.
	virtual void Update();

	/*!
		\brief Handles widget actions by delegating to parent or menu data handlers

		This function processes widget actions by first checking if the widget has a parent and delegating the handling to the parent if available. If there is no parent, it checks if forceHandled is
	   true and returns false in that case. Otherwise, it retrieves the menu data and delegates the action handling to it. The function returns whether the action was handled by the parent or menu
	   data handlers.

		\param action Reference to the widget action to be handled
		\param event Reference to the widget event associated with the action
		\param widget Pointer to the widget that triggered the action
		\param forceHandled Boolean flag indicating whether to force handling regardless of parent
		\return Boolean value indicating whether the action was handled by this function or its parent/menu data
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Returns the total number of options in the carousel widget.
	virtual int	 GetTotalNumberOfOptions() const { return imgList.Num(); }

	//! Prepares a list element for display in the carousel widget.
	virtual bool PrepareListElement( idMenuWidget& widget, const int childIndex ) { return true; }

	//! Sets the number of visible options for the carousel widget.
	void		 SetNumVisibleOptions( const int numVisibleOptions_ ) { numVisibleOptions = numVisibleOptions_; }

	//! Returns the number of visible options in the carousel widget.
	int			 GetNumVisibleOptions() const { return numVisibleOptions; }

	//! Moves the carousel widget to the specified index, with an optional instant transition.
	void		 MoveToIndex( int index, bool instant = false );

	//! Moves the carousel widget to the first item, optionally with an instant transition.
	void		 MoveToFirstItem( bool instant = true );

	//! Moves the carousel widget to the last item, with an optional instant transition.
	void		 MoveToLastItem( bool instant = true );

	//! Returns the index of the item to move to in the carousel widget.
	int			 GetMoveToIndex() { return moveToIndex; }

	//! Sets the index to move to in the carousel widget.
	void		 SetMoveToIndex( int index ) { moveToIndex = index; }

	//! Sets the view index of the carousel widget to the specified value.
	void		 SetViewIndex( int index ) { viewIndex = index; }

	//! Returns the current view index of the carousel widget.
	int			 GetViewIndex() const { return viewIndex; }

	//! Sets the list of images for the carousel widget.
	void		 SetListImages( idList<const idMaterial*>& list );

	//! Sets the move difference value for the carousel widget.
	void		 SetMoveDiff( int val ) { moveDiff = val; }

	//! Returns the difference used for movement operations.
	int			 GetMoveDiff() { return moveDiff; }

	//! Returns the state of the scroll left flag.
	bool		 GetScrollLeft() { return scrollLeft; }

private:
	int						  numVisibleOptions;
	int						  viewIndex;
	int						  moveToIndex;
	int						  moveDiff;
	bool					  fastScroll;
	bool					  scrollLeft;
	idList<const idMaterial*> imgList;
};

/*!
	\class idMenuWidget_Help
	\brief Manages and displays help tooltips by observing widget events.
*/
class idMenuWidget_Help : public idMenuWidget
{
public:
	//! Updates the help widget display based on the current hover or focus message.
	virtual void Update();

	//! Handles widget events for help display, updating message visibility based on focus and hover states.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

private:
	idStr lastFocusedMessage; // message from last widget that had focus
	idStr lastHoveredMessage; // message from last widget that was hovered over
	bool  hideMessage;
};

/*!
	\class idMenuWidget_CommandBar
	\brief Manages a command bar widget with configurable buttons and alignment.

	The idMenuWidget_CommandBar class represents a command bar widget that can display and manage multiple command buttons. It handles button positioning, visibility, and event handling based on the
   current configuration. The widget supports different alignment options and provides methods to manipulate individual buttons. This class is designed to be used within a menu system where command
   bars are needed to provide quick access to various actions. The class inherits from idMenuWidget, indicating it follows a standard menu widget interface and behavior. Button management is handled
   through a structured approach with methods to clear all buttons or retrieve specific button information.

*/
class idMenuWidget_CommandBar : public idMenuWidget
{
public:
	enum button_t { BUTTON_JOY1, BUTTON_JOY2, BUTTON_JOY3, BUTTON_JOY4, BUTTON_JOY10, BUTTON_TAB, MAX_BUTTONS };

	enum alignment_t { LEFT, RIGHT };

	struct buttonInfo_t {
		idStr		   label; // empty labels are treated as hidden buttons
		idWidgetAction action;
	};

	//! Constructs an idMenuWidget_CommandBar object with default alignment and initializes the buttons array.
	idMenuWidget_CommandBar() :
		alignment( LEFT )
	{
		buttons.SetNum( MAX_BUTTONS );
	}

	//! Updates the command bar widget by setting up button positions, visibility, and event bindings based on the current configuration.
	virtual void  Update();

	//! Executes an event for the command bar widget, handling command events by triggering the corresponding button action.
	virtual bool  ExecuteEvent( const idWidgetEvent& event );

	//! Returns a pointer to the button information structure for the specified button index.
	buttonInfo_t* GetButton( const button_t button ) { return &buttons[button]; }

	//! Clears all buttons in the command bar by resetting their labels and actions.
	void		  ClearAllButtons();

	//! Returns the alignment of the command bar widget.
	alignment_t	  GetAlignment() const { return alignment; }

	//! Sets the alignment of the command bar to the specified value.
	void		  SetAlignment( const alignment_t alignment_ ) { alignment = alignment_; }

private:
	idStaticList<buttonInfo_t, MAX_BUTTONS> buttons;
	alignment_t								alignment;
};

/*!
	\class idMenuWidget_LobbyList
	\brief A widget for displaying and managing a list of lobby entries with associated voice state information.

	The lobby list widget inherits from the list widget and provides functionality for managing a dynamic collection of lobby entries. It supports setting and updating entry data, including names and
   voice states, as well as configuring heading information. The widget prepares individual list elements for display and maintains a count of total entries. The refresh function allows for updating
   the list contents dynamically, though its specific purpose is not fully clarified in the provided information.

*/
class idMenuWidget_LobbyList : public idMenuWidget_List
{
public:
	//! Initializes a new instance of the lobby list widget with zero entries.
	idMenuWidget_LobbyList() :
		numEntries( 0 )
	{
	}

	//! Updates the lobby list widget by binding sprites and setting text for headings and options.
	virtual void Update();

	//! Prepares a list element for display in the lobby list widget.
	virtual bool PrepareListElement( idMenuWidget& widget, const int childIndex );

	//! Returns the total number of lobby entries in the list.
	virtual int	 GetTotalNumberOfOptions() const { return numEntries; }

	//! Sets the data for a specific entry in the lobby list widget.
	void		 SetEntryData( int index, idStr name, voiceStateDisplay_t voiceState );

	//! Sets the heading information for the lobby list widget using the provided list of strings.
	void		 SetHeadingInfo( idList<idStr>& list );

	//! Sets the number of entries in the lobby list widget.
	void		 SetNumEntries( int num ) { numEntries = num; }

	//! Returns the number of entries in the lobby list.
	int			 GetNumEntries() { return numEntries; }
	void		 SetRefreshFunction( const char* func );

private:
	idList<idStr, TAG_IDLIB_LIST_MENU> headings;
	int								   numEntries;
};

/*!
	\class idMenuWidget_DynamicList
	\brief A dynamic list widget that manages and displays variable-length lists of items with configurable display options.

	This class extends the base list widget functionality to support dynamic content updates and management. It handles the binding of sprites and management of list element visibility and states
   during updates. The widget can be initialized with menu handler data and prepared for display with individual list elements. It supports setting list data from a two-dimensional string list,
   controlling whether the list is used for control purposes, and managing color ignore flags. The class recalculates layout and state information for child widgets, making it suitable for presenting
   configurable lists of items that may change during runtime.

*/
class idMenuWidget_DynamicList : public idMenuWidget_List
{
public:
	//! Initializes a new instance of the idMenuWidget_DynamicList class with default settings.
	idMenuWidget_DynamicList() :
		controlList( false ),
		ignoreColor( false )
	{
	}

	//! Updates the dynamic list widget by binding sprites and managing list element visibility and states.
	virtual void Update();

	//! Initializes the dynamic list widget with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Returns the total number of options available in the dynamic list.
	virtual int	 GetTotalNumberOfOptions() const;

	//! Prepares a list element for display by initializing its properties and values.
	virtual bool PrepareListElement( idMenuWidget& widget, const int childIndex );

	//! Updates the layout and state of child widgets in the dynamic list.
	virtual void Recalculate();

	//! Sets the list data for the dynamic list widget from a two-dimensional string list.
	virtual void SetListData( idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU>& list );

	//! Sets the control list flag to the specified boolean value.
	void		 SetControlList( bool val ) { controlList = val; }

	//! Sets the ignore color flag for the dynamic list widget.
	void		 SetIgnoreColor( bool val ) { ignoreColor = val; }

protected:
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> listItemInfo;
	bool															controlList;
	bool															ignoreColor;
};

/*!
	\class idMenuWidget_ScoreboardList
	\brief Manages the display and updating of scoreboard list elements.
*/
class idMenuWidget_ScoreboardList : public idMenuWidget_DynamicList
{
public:
	//! Updates the scoreboard list widget by binding sprites and preparing list elements for display
	virtual void Update();

	//! Returns the total number of options in the scoreboard list.
	virtual int	 GetTotalNumberOfOptions() const;
};

/*!
	\class idMenuWidget_SystemOptionsList
	\brief System options list widget for managing system settings.
*/
class idMenuWidget_SystemOptionsList : public idMenuWidget_DynamicList
{
public:
	//! Updates the system options list widget by binding sprites and managing visibility of options.
	virtual void Update() override;

	//! Scrolls the system options list by the specified amount with optional wrapping.
	virtual void Scroll( const int scrollAmount, const bool wrapAround = false ) override;
};

/*!
	\class idMenuWidget_NavBar
	\brief A navigation bar widget that displays a list of options with specified spacing and positioning.

	The navigation bar widget is designed to display a series of navigation options, with the main option positioned at the safe frame line. It inherits from the dynamic list widget and provides
   functionality for initializing the widget with menu handler data, updating the display by binding sprites, and preparing list elements for visualization. The widget supports setting initial x
   position, button spacing, and list headings. The PrepareListElement method allows for customizing individual navigation bar elements, while GetTotalNumberOfOptions returns the total count of
   available navigation options. This widget is intended for use in user interfaces where a navigational element needs to be presented with specific layout constraints and positioning requirements.

*/
class idMenuWidget_NavBar : public idMenuWidget_DynamicList
{
public:
	//! Initializes a new instance of the idMenuWidget_NavBar class with default values for all member variables.
	idMenuWidget_NavBar() :
		initialPos( 0.0f ),
		buttonPos( 0.0f ),
		leftSpacer( 0.0f ),
		rightSpacer( 0.0f ),
		selectedSpacer( 0.0f )
	{
	}

	//! Updates the navigation bar widget by binding sprites and preparing list elements for display.
	virtual void Update();

	//! Initializes the navigation bar widget with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Sets the initial x position for the navigation bar.
	virtual void SetInitialXPos( float pos ) { initialPos = pos; }

	//! Sets the spacing for navigation bar buttons.
	virtual void SetButtonSpacing( float lSpace, float rSpace, float sSpace )
	{
		leftSpacer	   = lSpace;
		rightSpacer	   = rSpace;
		selectedSpacer = sSpace;
	}

	//! Prepares a navigation bar element for display with the specified widget and navigation index.
	virtual bool PrepareListElement( idMenuWidget& widget, const int navIndex );

	//! Sets the navigation bar headings from a list of strings.
	virtual void SetListHeadings( idList<idStr>& list );

	//! Returns the total number of navigation options available in the navigation bar.
	virtual int	 GetTotalNumberOfOptions() const;

private:
	idList<idStr, TAG_IDLIB_LIST_MENU> headings;
	float							   initialPos;
	float							   buttonPos;
	float							   leftSpacer;
	float							   rightSpacer;
	float							   selectedSpacer;
};

/*!
	\class idMenuWidget_MenuBar
	\brief A menu bar widget that manages a dynamic list of navigation options with customizable spacing and positioning.

	This class implements a menu bar widget that inherits from a dynamic list, allowing it to display multiple navigation options in a horizontal layout. The widget is initialized with a menu handler
   and can be configured with custom button spacing. It manages the positioning and display of child elements through its update and prepare list element methods. The menu bar maintains a list of
   headings that define the available options, and provides a method to determine the total number of options. The navigation bar is positioned with the main option aligned to the safe frame line,
   ensuring proper layout within the user interface.

*/
class idMenuWidget_MenuBar : public idMenuWidget_DynamicList
{
public:
	//! Constructs a new idMenuWidget_MenuBar object with default values for its member variables.
	idMenuWidget_MenuBar() :
		totalWidth( 0.0f ),
		buttonPos( 0.0f ),
		rightSpacer( 0.0f )
	{
	}

	//! Updates the menu bar widget by processing child elements and adjusting their positions.
	virtual void Update();

	//! Initializes the menu bar widget with the provided menu handler data
	virtual void Initialize( idMenuHandler* data );

	//! Sets the horizontal spacing between buttons in the menu bar.
	virtual void SetButtonSpacing( float rSpace ) { rightSpacer = rSpace; }

	//! Prepares a list element for display in the menu bar by setting its label and position based on the navigation index.
	virtual bool PrepareListElement( idMenuWidget& widget, const int navIndex );

	//! Sets the list of headings for the menu bar by copying the provided list of strings.
	virtual void SetListHeadings( idList<idStr>& list );

	//! Returns the total number of options in the menu bar.
	virtual int	 GetTotalNumberOfOptions() const;

private:
	idList<idStr, TAG_IDLIB_LIST_MENU> headings;
	float							   totalWidth;
	float							   buttonPos;
	float							   rightSpacer;
};

/*!
	\class idMenuWidget_PDA_UserData
	\brief A widget for displaying and updating PDA user data in the player's inventory.
*/
class idMenuWidget_PDA_UserData : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuWidget_PDA_UserData class with default values.
	idMenuWidget_PDA_UserData() :
		pdaIndex( 0 )
	{
	}
	virtual ~idMenuWidget_PDA_UserData() { }

	//! Updates the PDA user data display with information from the player's inventory.
	virtual void Update();

	//! Handles widget events for the PDA user data screen, updating the display based on focus and rollover events.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

private:
	int pdaIndex;
};

/*!
	\class idMenuWidget_ScrollBar
	\brief A scroll bar widget implementation for menu systems that handles user interaction and position calculations.

	This class provides a scroll bar widget that integrates with menu systems and responds to user interactions. It inherits from idMenuWidget and implements specific behavior for handling drag
   operations and updating scroll position. The widget manages its own state during dragging operations and coordinates with parent widgets to maintain consistent UI behavior. Initialization sets up
   the widget with menu handler data, while update operations synchronize the widget's state with its parent. The class supports observation of events from other widgets to react to scroll-related
   changes in the menu system. Position calculations are handled through dedicated methods that determine visual placement and coordinate-based updates.

*/
class idMenuWidget_ScrollBar : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuWidget_ScrollBar class with default values.
	idMenuWidget_ScrollBar() :
		yTop( 0.0f ),
		yBot( 0.0f ),
		dragging( false )
	{
	}

	//! Initializes the scroll bar widget with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the scroll bar widget based on the parent widget's state
	virtual void Update();

	/*!
		\brief Handles UI actions for a scroll bar widget, including drag operations and drag stop events.

		This function processes widget actions specific to a scroll bar, such as dragging the scroll handle and stopping the drag operation. It updates the scroll position based on drag coordinates
	   and manages the dragging state. For actions not handled locally, it delegates to the parent class implementation.

		\param action The widget action being processed
		\param event The event data associated with the action
		\param widget The menu widget that triggered the action
		\param forceHandled Whether to force the action as handled regardless of processing outcome
		\return True if the action was handled by this function or its delegates, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles scroll events for the scrollbar widget
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Calculates the top and bottom Y positions of the scroll bar widget.
	void		 CalcTopAndBottom();

	//! Updates the scroll bar position based on the provided coordinates
	void		 CalculatePosition( float x, float y );

	float		 yTop;
	float		 yBot;
	bool		 dragging;
};

/*!
	\class idMenuWidget_InfoBox
	\brief A widget for displaying scrollable information with heading and body text.

	This class implements a specialized widget for presenting information in a scrollable format with distinct heading and body sections. It inherits from idMenuWidget and provides functionality for
   managing scroll position, handling user interactions such as scrolling actions, and updating display content. The widget maintains its own scroll state and can be associated with a scrollbar widget
   to provide visual scrolling indicators. It supports setting and retrieving scroll values, resetting scroll position, and handling focus events that may affect scrolling behavior.

*/
class idMenuWidget_InfoBox : public idMenuWidget
{
public:
	//! Constructs an instance of the info box widget.
	idMenuWidget_InfoBox() :
		scrollbar( NULL )
	{
	}

	//! Initializes the info box widget with the provided menu handler data.
	virtual void Initialize( idMenuHandler* data );

	//! Updates the info box widget with heading and body text
	virtual void Update();

	/*!
		\brief Handles widget actions for scrolling vertical information boxes

		This function processes widget actions specifically for vertical scrolling in information boxes. When a vertical scroll action is detected, it retrieves the scroll parameters and performs a
	   single scroll operation based on the provided parameter. The function returns true to indicate that the action was handled, or delegates the handling to the parent class when the action is not
	   a vertical scroll.

		\param action The widget action being processed
		\param event The widget event associated with the action
		\param widget The widget that triggered the action
		\param forceHandled Whether to force the action to be handled regardless of normal conditions
		\return True if the action was handled, false otherwise
	*/
	virtual bool HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );

	//! Handles widget events for the info box, specifically resetting scroll when focus is gained.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Sets the heading text for the info box widget.
	void		 SetHeading( idStr val ) { heading = val; }

	//! Sets the body text of the info box to the provided string value.
	void		 SetBody( idStr val ) { info = val; }

	//! Resets the scroll position of the info box text and updates the scrollbar.
	void		 ResetInfoScroll();

	//! Updates the scroll position of the info box text by the specified delta amount.
	void		 Scroll( int d );

	//! Returns the current scroll value of the info box text body.
	int			 GetScroll();

	//! Returns the maximum scroll value for the info box text body.
	int			 GetMaxScroll();

	//! Sets the scroll position of the info box text body.
	void		 SetScroll( int scroll );

	//! Sets the scrollbar for the info box widget.
	void		 SetScrollbar( idMenuWidget_ScrollBar* bar );

private:
	idMenuWidget_ScrollBar* scrollbar;
	idStr					heading;
	idStr					info;
};

/*!
	\class idMenuWidget_PDA_Objective
	\brief A widget for displaying and managing PDA objectives in a user interface.
*/
class idMenuWidget_PDA_Objective : public idMenuWidget
{
public:
	//! Constructs a new instance of the PDA objective widget and initializes the pdaIndex member to zero.
	idMenuWidget_PDA_Objective() :
		pdaIndex( 0 )
	{
	}

	//! Updates the objective display in the PDA menu based on the player's current objectives.
	virtual void Update();

	//! Handles widget events for the PDA objective interface
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

private:
	int pdaIndex;
};

/*!
	\class idMenuWidget_Shell_SaveInfo
	\brief Manages and displays save game information for shell interfaces.
*/
class idMenuWidget_Shell_SaveInfo : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuWidget_Shell_SaveInfo class with default values for load index and save screen flag.
	idMenuWidget_Shell_SaveInfo() :
		loadIndex( 0 ),
		forSaveScreen( false )
	{
	}

	//! Updates the save game information display with details from the sorted save games.
	virtual void Update();

	//! Handles widget events for save information display
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Sets the for save screen flag to the specified boolean value.
	void		 SetForSaveScreen( bool val ) { forSaveScreen = val; }

private:
	int	 loadIndex;
	bool forSaveScreen;
};

/*!
	\class idMenuWidget_PDA_AudioFiles
	\brief Manages the display and interaction of audio files within a PDA interface.
*/
class idMenuWidget_PDA_AudioFiles : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuWidget_PDA_AudioFiles class with default values.
	idMenuWidget_PDA_AudioFiles() :
		pdaIndex( 0 )
	{
	}
	virtual ~idMenuWidget_PDA_AudioFiles();

	//! Updates the audio file list display for a PDA widget.
	virtual void Update();

	//! Initializes the audio files widget for the PDA interface
	virtual void Initialize( idMenuHandler* data );

	//! Handles UI events for audio file list items in the PDA audio files menu.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

private:
	int																pdaIndex;
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> audioFileNames;
};

/*!
	\class idMenuWidget_PDA_EmailInbox
	\brief A widget for displaying and managing email inbox content within a PDA interface.
*/
class idMenuWidget_PDA_EmailInbox : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuWidget_PDA_EmailInbox class.
	idMenuWidget_PDA_EmailInbox() :
		emailList( NULL ),
		scrollbar( NULL ),
		pdaIndex( 0 )
	{
	}

	//! Updates the PDA email inbox widget display.
	virtual void			  Update();

	//! Initializes the email inbox widget with the provided menu handler data.
	virtual void			  Initialize( idMenuHandler* data );

	//! Handles widget events for the PDA email inbox, updating the view and focus index when widgets gain or lose focus.
	virtual void			  ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

	//! Returns a pointer to the email list widget.
	idMenuWidget_DynamicList* GetEmailList() { return emailList; }

	//! Returns the scrollbar widget associated with the email inbox menu.
	idMenuWidget_ScrollBar*	  GetScrollbar() { return scrollbar; }

private:
	idMenuWidget_DynamicList*										emailList;
	idMenuWidget_ScrollBar*											scrollbar;
	int																pdaIndex;
	idList<idList<idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU> emailInfo;
};

/*!
	\class idMenuWidget_ItemAssignment
	\brief A widget for managing item assignments in game menus.
*/
class idMenuWidget_ItemAssignment : public idMenuWidget
{
public:
	//! Initializes a new instance of the idMenuWidget_ItemAssignment class with default values.
	idMenuWidget_ItemAssignment() :
		slotIndex( 0 )
	{
	}

	//! Updates the item assignment widget display based on the current slot index and assigned items.
	virtual void Update();

	//! Sets the icon material for a specified quick slot index.
	void		 SetIcon( int index, const idMaterial* icon );

	//! Finds the first available slot index in the item assignment widget.
	void		 FindFreeSpot();

	//! Returns the slot index of the item assignment widget.
	int			 GetSlotIndex() { return slotIndex; }

	//! Sets the slot index for the item assignment widget.
	void		 SetSlotIndex( int num ) { slotIndex = num; }

private:
	const idMaterial* images[NUM_QUICK_SLOTS];
	int				  slotIndex;
};

/*!
	\class idMenuWidget_PDA_VideoInfo
	\brief Provides video information display functionality within a PDA menu system.
*/
class idMenuWidget_PDA_VideoInfo : public idMenuWidget
{
public:
	//! Updates the video information display in the PDA menu.
	virtual void Update();

	//! Handles widget events for video information display, updating video preview and screen state when a video gains focus.
	virtual void ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event );

private:
	int videoIndex;
};

/*!
	\class idWidgetActionHandler
	\brief Manages widget action handling by associating widgets with specific action events and widget events.
*/
class idWidgetActionHandler : public idSWFScriptFunction_RefCounted
{
public:
	//! Constructs an idWidgetActionHandler object with the specified widget, action event type, and widget event.
	idWidgetActionHandler( idMenuWidget* widget, actionHandler_t actionEventType, widgetEvent_t _event ) :
		targetWidget( widget ),
		type( actionEventType ),
		targetEvent( _event )
	{
	}

	//! Calls a script function with the specified object and parameters, handling various widget actions.
	idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
	{
		idWidgetAction action;
		bool		   handled = false;
		switch( type ) {
			case WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_VERTICAL, 1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_VERTICAL, -1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER_VARIABLE: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_VERTICAL_VARIABLE, 1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER_VARIABLE: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_VERTICAL_VARIABLE, -1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_PAGE_DOWN_START_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_PAGE, 1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_PAGE_UP_START_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_PAGE, -1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_STOP_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_STOP_REPEATER );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_TAB_NEXT: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_SCROLL_TAB, 1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_TAB_PREV: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_SCROLL_TAB, -1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_JOY3_ON_PRESS: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_JOY3_ON_PRESS );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_LEFT_START_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_HORIZONTAL, -1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_SCROLL_RIGHT_START_REPEATER: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_START_REPEATER, WIDGET_ACTION_SCROLL_HORIZONTAL, 1 );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_DRAG_START: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_SCROLL_DRAG );
				handled = true;
				break;
			}
			case WIDGET_ACTION_EVENT_DRAG_STOP: {
				action.Set( ( widgetAction_t )WIDGET_ACTION_EVENT_DRAG_STOP );
				handled = true;
				break;
			}
		}

		if( handled ) { targetWidget->HandleAction( action, idWidgetEvent( targetEvent, 0, thisObject, parms ), targetWidget ); }

		return idSWFScriptVar();
	}

private:
	idMenuWidget*	targetWidget;
	actionHandler_t type;
	widgetEvent_t	targetEvent;
};

#endif
