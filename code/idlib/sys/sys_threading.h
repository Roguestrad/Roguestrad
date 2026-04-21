/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

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
#ifndef __SYS_THREADING_H__
#define __SYS_THREADING_H__

#ifndef __TYPEINFOGEN__

	/*
	================================================================================================

		Platform specific mutex, signal, atomic integer and memory barrier.

	================================================================================================
	*/

	// RB begin
	#if defined( _WIN32 )
typedef CRITICAL_SECTION mutexHandle_t;
typedef HANDLE			 signalHandle_t;
typedef LONG			 interlockedInt_t;
	#else

		#include <pthread.h>

/*!
	\struct signalHandle_t
	\brief Handle type for managing signal connections in the engine's event system.

	The signalHandle_t struct serves as a opaque handle type for managing signal connections within the engine's event system. It is used to uniquely identify and manage connections between signals
   and their respective callbacks. This handle type is typically returned when connecting a signal to a callback and can be used later to disconnect the signal. The structure itself contains no data
   members, making it essentially a type-safe identifier for signal management operations. This design allows the engine to maintain a clean abstraction for event handling while providing a simple
   interface for connecting and disconnecting signal observers.

*/
struct signalHandle_t {
	// DG: all this stuff is needed to emulate Window's Event API
	//     (CreateEvent(), SetEvent(), WaitForSingleObject(), ...)
	pthread_cond_t	cond;
	pthread_mutex_t mutex;
	int				waiting; // number of threads waiting for a signal
	bool			manualReset;
	bool			signaled; // is it signaled right now?
};

typedef pthread_mutex_t mutexHandle_t;
typedef int				interlockedInt_t;
	#endif
	// RB end

	// _ReadWriteBarrier() does not translate to any instructions but keeps the compiler
	// from reordering read and write instructions across the barrier.
	// MemoryBarrier() inserts and CPU instruction that keeps the CPU from reordering reads and writes.
	#if defined( _MSC_VER )
		#pragma intrinsic( _ReadWriteBarrier )
		#define SYS_MEMORYBARRIER \
			_ReadWriteBarrier();  \
			MemoryBarrier()
	#elif defined( __GNUC__ ) // FIXME: what about clang?
		// according to http://en.wikipedia.org/wiki/Memory_ordering the following should be equivalent to the stuff above..
		// #ifdef __sync_syncronize
		#define SYS_MEMORYBARRIER            \
			asm volatile( "" ::: "memory" ); \
			__sync_synchronize()
	#endif

	/*
	================================================================================================

		Platform specific thread local storage.
		Can be used to store either a pointer or an integer.

	================================================================================================
	*/

	// RB: added POSIX implementation
	#if defined( _WIN32 )
class idSysThreadLocalStorage
{
public:
	idSysThreadLocalStorage() { tlsIndex = TlsAlloc(); }

	idSysThreadLocalStorage( const ptrdiff_t& val )
	{
		tlsIndex = TlsAlloc();
		TlsSetValue( tlsIndex, ( LPVOID )val );
	}

	~idSysThreadLocalStorage() { TlsFree( tlsIndex ); }

	operator ptrdiff_t() { return ( ptrdiff_t )TlsGetValue( tlsIndex ); }

	const ptrdiff_t& operator=( const ptrdiff_t& val )
	{
		TlsSetValue( tlsIndex, ( LPVOID )val );
		return val;
	}

	DWORD tlsIndex;
};
	#else

/*!
	\class idSysThreadLocalStorage
	\brief Provides thread-local storage functionality using POSIX pthread library.

	This class encapsulates thread-local storage management through POSIX pthread APIs. It allows creation of thread-local storage keys that can hold data specific to each executing thread. The class
   supports both default initialization with no destructor and initialization with a specific value. Thread-local storage is commonly used in engine components that require per-thread state or data
   without requiring synchronization mechanisms. The implementation leverages pthread_key_create for key creation and pthread_setspecific for value assignment, making it suitable for scenarios where
   lightweight per-thread data access is needed. Memory management for stored data is not automatically handled by this class, requiring explicit management by the caller.

*/
class idSysThreadLocalStorage
{
public:
	/*!
		\brief Initializes a new thread-local storage key for the current thread.

		This constructor creates a new thread-local storage key using the POSIX pthread library. The key is used to store data that is specific to each thread. The destructor will automatically clean
	   up the key when the object is destroyed. The key is created with no destructor function, meaning that any data stored in the key will not be automatically freed when the thread exits.

	*/
	idSysThreadLocalStorage() { pthread_key_create( &key, NULL ); }

	/*!
		\brief Initializes a thread-local storage slot with the specified value.

		This constructor creates a new thread-local storage key using pthread_key_create and initializes it with the provided value using pthread_setspecific. The value is stored in the thread-local
	   storage and can be retrieved later using the key.

		\param val The initial value to store in the thread-local storage
	*/
	idSysThreadLocalStorage( const ptrdiff_t& val )
	{
		pthread_key_create( &key, NULL );
		pthread_setspecific( key, ( const void* )val );
	}

	~idSysThreadLocalStorage() { pthread_key_delete( key ); }

	operator ptrdiff_t() { return ( ptrdiff_t )pthread_getspecific( key ); }

	const ptrdiff_t& operator=( const ptrdiff_t& val )
	{
		pthread_setspecific( key, ( const void* )val );
		return val;
	}

	pthread_key_t key;
};
	#endif
	// RB end

	#define ID_TLS idSysThreadLocalStorage

#endif // __TYPEINFOGEN__

/*
================================================================================================

	Platform independent threading functions.

================================================================================================
*/

enum core_t { CORE_ANY = -1, CORE_0A, CORE_0B, CORE_1A, CORE_1B, CORE_2A, CORE_2B };

typedef unsigned int ( *xthread_t )( void* );

enum xthreadPriority { THREAD_LOWEST, THREAD_BELOW_NORMAL, THREAD_NORMAL, THREAD_ABOVE_NORMAL, THREAD_HIGHEST };

#define DEFAULT_THREAD_STACK_SIZE ( 256 * 1024 )

/*!
	\brief Returns the unique identifier for the currently executing thread.

	This function retrieves the thread identifier for the calling thread. On POSIX systems, it returns the result of pthread_self() cast to uintptr_t. The implementation ensures that the thread ID is
   properly converted from a pthread handle to an unsigned integer type suitable for thread identification.

	\return A unique identifier for the currently executing thread as an unsigned integer type
*/
uintptr_t		 Sys_GetCurrentThreadID();

/*!
	\brief Creates a new thread with the specified parameters and returns a handle to it

	This function creates a new thread using pthread_create and initializes its attributes. It sets the thread to be joinable and optionally assigns a name to it for debugging purposes. The function
   performs error checking and will trigger a fatal error if thread creation fails. The thread is created with a default scheduling policy and priority, though realtime scheduling is commented out but
   available for configuration. The thread handle is returned as a uintptr_t value for later use in thread management operations.

	\param function The entry point function for the new thread
	\param parms Pointer to parameters to pass to the thread function
	\param priority The scheduling priority for the thread
	\param name Name to assign to the thread for identification purposes
	\param core The core or CPU affinity for the thread
	\param stackSize The stack size for the new thread, defaults to 256KB
	\param suspended Whether the thread should be created in a suspended state
	\return A uintptr_t handle to the created thread that can be used for thread management operations
	\throws Fatal error if pthread_attr_setdetachstate, pthread_create, or pthread_setname_np fails
*/
uintptr_t		 Sys_CreateThread( xthread_t function, void* parms, xthreadPriority priority, const char* name, core_t core, int stackSize = DEFAULT_THREAD_STACK_SIZE, bool suspended = false );

/*!
	\brief Destroys a thread by joining it and optionally canceling it if debugging is enabled

	This function cleans up a thread by attempting to join it, which waits for the thread to finish execution. If debugging is enabled, it also attempts to cancel the thread before joining. The
   function retrieves the thread name for error messages. It is designed to handle thread cleanup safely, with error reporting via FatalError if operations fail. The thread handle is expected to be a
   pthread_t cast to uintptr_t.

	\param threadHandle The handle of the thread to be destroyed, typically a pthread_t cast to uintptr_t
	\throws FatalError if pthread_join fails
*/
void			 Sys_DestroyThread( uintptr_t threadHandle );
void			 Sys_SetCurrentThreadName( const char* name );

/*!
	\brief Creates a signal handle with specified reset behavior

	This function initializes a signal handle for synchronization purposes. The handle is configured based on the manualReset parameter to determine whether the signal is auto-reset or manually reset.
   The initial state of the signal is set to not signaled. The function sets up the necessary pthread synchronization primitives including a mutex and condition variable for thread waiting and
   signaling operations.

	\param handle Reference to the signal handle to be initialized
	\param manualReset If true, the signal remains signaled until explicitly cleared; if false, it auto-resets after a single waiting thread is released
*/
void			 Sys_SignalCreate( signalHandle_t& handle, bool manualReset );

/*!
	\brief Destroys a signal handle by resetting its state and cleaning up associated synchronization primitives.

	This function cleans up the resources associated with a signal handle. It resets the signaled state, clears the waiting count, and destroys the underlying mutex and condition variable used for
   synchronization.

	\param handle Reference to the signal handle to be destroyed
*/
void			 Sys_SignalDestroy( signalHandle_t& handle );

/*!
	\brief Sets the signaled state of a synchronization handle, waking waiting threads if necessary.

	This function signals a synchronization handle by setting its signaled flag to true while ensuring thread safety through mutex locking. For manual reset events, it wakes all waiting threads using
   broadcast. For automatic reset events, it wakes only one waiting thread if any exist, otherwise it sets the signaled flag to true to maintain the signaled state. The function ensures proper thread
   coordination by acquiring and releasing a mutex around the operation.

	\param handle Reference to the signal handle whose signaled state will be set
*/
void			 Sys_SignalRaise( signalHandle_t& handle );

/*!
	\brief Clears the signaled state of a synchronization handle

	This function resets the signaled state of a signal handle by setting its signaled flag to false while ensuring thread safety through mutex locking. The function operates on a signal handle that
   represents a synchronization primitive, typically used for thread coordination. It acquires a lock on the handle's mutex before modifying the signaled state and releases the lock afterward. The
   implementation uses a mutex to protect concurrent access to the handle's state, which is necessary for proper thread synchronization.

	\param handle Reference to the signal handle whose signaled state will be cleared
*/
void			 Sys_SignalClear( signalHandle_t& handle );

/*!
	\brief Waits for a signal handle to be signaled, with an optional timeout.

	This function waits for a signal handle to be signaled, using pthread synchronization primitives. It supports both infinite and finite timeouts. If the signal is already signaled, it returns
   immediately. Otherwise, it waits either indefinitely or until the specified timeout expires. For auto-reset signals, only one waiting thread is released. The function returns true if the signal was
   received, false if a timeout occurred.

	\param handle Reference to the signal handle to wait on
	\param timeout Timeout in milliseconds, or WAIT_INFINITE for no timeout
	\return True if the signal was received, false if a timeout occurred
*/
bool			 Sys_SignalWait( signalHandle_t& handle, int timeout );

/*!
	\brief Initializes a mutex handle for synchronization

	This function creates and initializes a mutex handle that can be used for thread synchronization. It sets up the mutex with error checking semantics which helps prevent deadlocks and invalid mutex
   operations. The function initializes the pthread mutex attributes, sets the mutex type to error checking, and then initializes the mutex itself. After initialization, the attributes are destroyed
   to free up resources.

	\param handle Reference to the mutex handle to be initialized
*/
void			 Sys_MutexCreate( mutexHandle_t& handle );

/*!
	\brief Destroys a mutex handle

	This function destroys a mutex that was previously initialized using Sys_MutexInit. It should be called when the mutex is no longer needed to free up system resources. The function expects a valid
   mutex handle that was initialized with Sys_MutexInit.

	\param handle Reference to the mutex handle to be destroyed
*/
void			 Sys_MutexDestroy( mutexHandle_t& handle );

/*!
	\brief Acquires a mutex lock, optionally blocking if the lock is unavailable.

	This function attempts to acquire a mutex lock represented by the handle parameter. If the lock is already held and blocking is false, the function returns false immediately. If blocking is true,
   the function will block until the lock becomes available. The function returns true if the lock was successfully acquired, and false if the lock could not be acquired within the specified blocking
   behavior.

	\param handle Reference to the mutex handle to lock
	\param blocking If true, blocks until the lock is acquired; if false, returns immediately if the lock is not available
	\return True if the mutex was successfully locked, false if the lock could not be acquired and blocking was false.
*/
bool			 Sys_MutexLock( mutexHandle_t& handle, bool blocking );

/*!
	\brief Releases a previously acquired mutex lock.

	This function unlocks the mutex identified by the provided handle, allowing other threads that are waiting to acquire the same mutex to proceed. It is the complement to Sys_MutexLock and must be
   called to release the lock acquired by a corresponding Sys_MutexLock call.

	\param handle Reference to the mutex handle to be unlocked.
*/
void			 Sys_MutexUnlock( mutexHandle_t& handle );

//! Increments the given interlocked integer value and returns the new value.
interlockedInt_t Sys_InterlockedIncrement( interlockedInt_t& value );

//! Decrements the value of the given interlocked integer variable by one and returns the new value.
interlockedInt_t Sys_InterlockedDecrement( interlockedInt_t& value );

//! Atomically adds a value to an interlocked integer and returns the new value.
interlockedInt_t Sys_InterlockedAdd( interlockedInt_t& value, interlockedInt_t i );

//! Performs an atomic subtraction operation on an interlocked integer value.
interlockedInt_t Sys_InterlockedSub( interlockedInt_t& value, interlockedInt_t i );

//! Performs an atomic exchange operation on an integer value.
interlockedInt_t Sys_InterlockedExchange( interlockedInt_t& value, interlockedInt_t exchange );

//! Atomically compares the value at address value with comparand and replaces it with exchange if they are equal.
interlockedInt_t Sys_InterlockedCompareExchange( interlockedInt_t& value, interlockedInt_t comparand, interlockedInt_t exchange );

//! Atomically exchanges the pointer value with a new value and returns the old value.
void*			 Sys_InterlockedExchangePointer( void*& ptr, void* exchange );

//! Performs an atomic compare-and-swap operation on a pointer.
void*			 Sys_InterlockedCompareExchangePointer( void*& ptr, void* comparand, void* exchange );

/*!
	\brief Yields the processor to other threads.

	This function allows the current thread to yield control to other threads that are ready to run. It is implemented using sched_yield() which is a POSIX function that provides a hint to the
   scheduler that the calling thread is willing to give up the processor.

*/
void			 Sys_Yield();

const int		 MAX_CRITICAL_SECTIONS = 4;

enum { CRITICAL_SECTION_ZERO = 0, CRITICAL_SECTION_ONE, CRITICAL_SECTION_TWO, CRITICAL_SECTION_THREE };

#endif // !__SYS_THREADING_H__
