#ifndef __SYS_STATE_H__
#define __SYS_STATE_H__

typedef enum {
	SRESULT_OK,		   // Call was made successfully
	SRESULT_ERROR,	   // An unrecoverable error occurred
	SRESULT_DONE,	   // Done with current state, move to next
	SRESULT_DONE_WAIT, // Done with current state, wait a frame then move to next
	SRESULT_WAIT,	   // Wait a frame and re-run current state
	SRESULT_IDLE,	   // State thread is currently idle (ie. no states)
	SRESULT_SETSTAGE,  // Sets the current stage of the current state and reruns the state
	SRESULT_DONE_FRAME,
	// NOTE: this has to be the last result becuase the stage is added to
	//		 the result.
	SRESULT_SETDELAY = SRESULT_SETSTAGE + 20
} stateResult_t;

#define MAX_STATE_CALLS	   50

#define SRESULT_STAGE( x ) ( ( stateResult_t )( ( int )SRESULT_SETSTAGE + ( int )( x ) ) )
#define SRESULT_DELAY( x ) ( ( stateResult_t )( ( int )SRESULT_SETDELAY + ( int )( x ) ) )

struct stateParms_t {
	int	  blendFrames;
	int	  time;
	int	  stage;
	int	  substage;
	float param1;
	float param2;

	float subparam1;
	float subparam2;

	//! Pauses execution for the specified number of seconds.
	void  Wait( float seconds );

	//! Saves the state parameters to the specified save file.
	void  Save( idSaveGame* saveFile ) const;

	//! Restores the state parameters from a save file.
	void  Restore( idRestoreGame* saveFile );
};

/*
================
CLASS_STATES_PROTOTYPE

This macro must be included in the definition of any subclass of idClass that
wishes to have its own custom states.  Its prototypes variables used in the process
of managing states.
================
*/
#define CLASS_STATES_PROTOTYPE( nameofclass ) \
protected:                                    \
	static rvStateFunc<nameofclass> stateCallbacks[]

/*
================
CLASS_STATES_DECLARATION

This macro must be included in the code to properly initialize variables
used in state processing for a idClass dervied class
================
*/
#define CLASS_STATES_DECLARATION( nameofclass ) rvStateFunc<nameofclass> nameofclass::stateCallbacks[] = {
/*
================
STATE

This macro declares a single state.  It must be surrounded by the CLASS_STATES_DECLARATION
and END_CLASS_STATES macros.
================
*/
#define STATE( statename, function )			{ statename, ( stateCallback_t )( &function ) },

/*
================
END_CLASS_STATES

Terminates a state block
================
*/
#define END_CLASS_STATES \
	{                    \
		NULL, NULL       \
	}                    \
	}                    \
	;

struct stateCall_t {
	idStr					state;
	idLinkList<stateCall_t> node;
	int						flags;
	int						delay;
	stateParms_t			parms;

	//! Saves the state call information to a save file
	void					Save( idSaveGame* saveFile ) const;

	//! Restores the state call information from a save file
	void					Restore( idRestoreGame* saveFile, const idClass* owner );
};

class idClass;

const int SFLAG_ONCLEAR		= BIT( 0 ); // Executes, even if the state queue is cleared
const int SFLAG_ONCLEARONLY = BIT( 1 ); // Executes only if the state queue is cleared

/*!
	\class rvStateThread
	\brief Manages state transitions and execution for objects with a finite state machine.

	The rvStateThread class provides a mechanism for managing state transitions and execution within objects that utilize a finite state machine approach. It handles queuing, blending, and delaying
   state changes while maintaining the execution context. The class supports interrupting current states to immediately execute new ones, and can be configured with various flags to control transition
   behavior. It is designed to work with idClass-based objects and maintains a clear distinction between idle and executing states. The implementation supports both asynchronous state execution and
   direct state setting operations, providing flexibility for different game logic requirements. State transitions can be saved and restored, enabling persistent state management across game sessions.

*/
class rvStateThread
{
public:
	//! Initializes a new rvStateThread instance with default values.
	rvStateThread();

	//! Destructor for rvStateThread that clears the state thread.
	~rvStateThread();

	//! Sets the name of the state thread to the provided string.
	void		  SetName( const char* name );

	//! Sets the owner of the state thread to the specified object.
	void		  SetOwner( idClass* owner );

	bool		  Interrupt();

	/*!
		\brief Interrupts the current state and posts a new state to be executed.

		This function interrupts the currently executing state by moving all pending states to the front of the interrupted list while preserving their order. It then clears the insertion point and
	   marks the state as interrupted. Finally, it posts the new state specified by the name parameter with the provided blend frames, delay, and flags.

		\param state Name of the state to be posted after interrupting the current state.
		\param blendFrames Number of frames over which to blend the state transition.
		\param delay Delay in game frames before the state execution begins.
		\param flags Additional flags that control state execution behavior.
		\return The result of the PostState call which indicates the success or failure of posting the new state.
	*/
	stateResult_t InterruptState( const char* state, int blendFrames = 0, int delay = 0, int flags = 0 );

	/*!
		\brief Posts a state change request to the state thread with optional blending, delay, and flags.

		This function creates a new state call request and adds it to the state thread queue. The state change will be executed asynchronously. The function allows setting blending frames, delay, and
	   additional flags to control the state transition behavior. If the state thread is currently executing and an insertAfter marker is set, the new call will be inserted after that marker.
	   Otherwise, it will be added to the end of the queue. The function initializes various parameters for the state call including blend frames, time, stage, substage, and additional parameters.

		\param state The name of the state to transition to
		\param blendFrames Number of frames to blend the transition over
		\param delay Delay in milliseconds before executing the state change
		\param flags Additional flags to control the state transition behavior
		\return A stateResult_t value indicating the success of the operation, typically SRESULT_OK if successful
	*/
	stateResult_t PostState( const char* state, int blendFrames = 0, int delay = 0, int flags = 0 );

	/*!
		\brief Sets the state of the state thread, clearing any existing state and posting the new state with specified parameters.

		This function clears the current state of the rvStateThread object and then posts a new state with the provided name, blend frames, delay, and flags. The function is typically used to
	   transition the state thread to a new state in response to game events or system changes. The state name is used to identify the target state, while blendFrames, delay, and flags control how the
	   state transition is performed.

		\param state Name of the state to set
		\param blendFrames Number of frames over which to blend the state transition
		\param delay Delay in frames before starting the state transition
		\param flags Flags controlling the state transition behavior
		\return The result of the state transition operation as a stateResult_t enum value
	*/
	stateResult_t SetState( const char* state, int blendFrames = 0, int delay = 0, int flags = 0 );

	//! Returns the current state call object from the state thread.
	stateCall_t*  GetState() const;

	//! Checks if the current state of the state thread matches the given state name.
	bool		  CurrentStateIs( const char* name ) const;

	//! Executes the state machine thread, processing queued state calls and managing state transitions.
	stateResult_t Execute();

	//! Clears all state calls from the state thread, optionally ignoring state call processing.
	void		  Clear( bool ignoreStateCalls = false );

	//! Returns true if the state thread is currently idle.
	bool		  IsIdle() const;

	//! Returns true if the state thread is currently executing.
	bool		  IsExecuting() const;

	//! Saves the state thread information to a save file.
	void		  Save( idSaveGame* saveFile ) const;

	//! Restores the state thread's saved state from a save file.
	void		  Restore( idRestoreGame* saveFile, idClass* owner );

protected:
	struct flags {
		bool stateCleared	  : 1; // State list was cleared
		bool stateInterrupted : 1; // State list was interrupted
		bool executing		  : 1; // Execute is currently processing states
	} fl;

	idStr					name;
	idClass*				owner;
	idLinkList<stateCall_t> states;
	idLinkList<stateCall_t> interrupted;
	stateCall_t*			insertAfter;
	stateResult_t			lastResult;
};

ID_INLINE void rvStateThread::SetName( const char* _name )
{
	name = _name;
}

ID_INLINE stateCall_t* rvStateThread::GetState() const
{
	return states.Next();
}

ID_INLINE bool rvStateThread::IsIdle() const
{
	return !states.Next() && !interrupted.Next();
}

ID_INLINE bool rvStateThread::IsExecuting() const
{
	return fl.executing;
}

#endif // __SYS_STATE_H__
