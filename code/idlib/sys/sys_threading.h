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
	\brief Manages thread-local storage keys for efficient thread-specific data access.
*/
class idSysThreadLocalStorage
{
public:
	//! Initializes a new thread local storage key.
	idSysThreadLocalStorage() { pthread_key_create( &key, NULL ); }

	//! Initializes a thread local storage entry with the specified value.
	idSysThreadLocalStorage( const ptrdiff_t& val )
	{
		pthread_key_create( &key, NULL );
		pthread_setspecific( key, ( const void* )val );
	}

	~idSysThreadLocalStorage() { pthread_key_delete( key ); }

	//! Converts the thread-local storage key to a ptrdiff_t value.
	operator ptrdiff_t() { return ( ptrdiff_t )pthread_getspecific( key ); }

	//! Assigns a value to the thread-local storage and returns the assigned value
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

//! Returns the unique identifier of the currently executing thread.
uintptr_t		 Sys_GetCurrentThreadID();

//! Creates a new thread with the specified parameters and returns a handle to it
uintptr_t		 Sys_CreateThread( xthread_t function, void* parms, xthreadPriority priority, const char* name, core_t core, int stackSize = DEFAULT_THREAD_STACK_SIZE, bool suspended = false );

//! Destroys a thread identified by its handle.
void			 Sys_DestroyThread( uintptr_t threadHandle );
void			 Sys_SetCurrentThreadName( const char* name );

//! Initializes a signal handle with the specified manual reset behavior.
void			 Sys_SignalCreate( signalHandle_t& handle, bool manualReset );

//! Destroys a signal handle by resetting its state and cleaning up associated synchronization primitives.
void			 Sys_SignalDestroy( signalHandle_t& handle );

//! Raises a signal handle, waking up waiting threads or marking it as signaled.
void			 Sys_SignalRaise( signalHandle_t& handle );

//! Clears the signaled state of the provided signal handle.
void			 Sys_SignalClear( signalHandle_t& handle );

//! Waits for a signal handle to be signaled or for a timeout to occur
bool			 Sys_SignalWait( signalHandle_t& handle, int timeout );

//! Initializes a mutex handle for synchronization.
void			 Sys_MutexCreate( mutexHandle_t& handle );

//! Destroys a mutex handle.
void			 Sys_MutexDestroy( mutexHandle_t& handle );

//! Acquires a mutex lock, optionally blocking if the lock is unavailable.
bool			 Sys_MutexLock( mutexHandle_t& handle, bool blocking );

//! Releases a previously acquired mutex lock.
void			 Sys_MutexUnlock( mutexHandle_t& handle );

//! Increments the given interlocked integer value atomically and returns the new value.
interlockedInt_t Sys_InterlockedIncrement( interlockedInt_t& value );

//! Decrements the value of an interlocked integer variable by one and returns the new value.
interlockedInt_t Sys_InterlockedDecrement( interlockedInt_t& value );

//! Performs an atomic addition operation on a shared integer variable and returns the updated value.
interlockedInt_t Sys_InterlockedAdd( interlockedInt_t& value, interlockedInt_t i );

//! Performs an atomic subtraction operation on a shared integer value.
interlockedInt_t Sys_InterlockedSub( interlockedInt_t& value, interlockedInt_t i );

//! Performs an atomic exchange operation on an interlocked integer value.
interlockedInt_t Sys_InterlockedExchange( interlockedInt_t& value, interlockedInt_t exchange );

//! Atomically compares and exchanges a value in memory based on a comparison.
interlockedInt_t Sys_InterlockedCompareExchange( interlockedInt_t& value, interlockedInt_t comparand, interlockedInt_t exchange );

//! Atomically exchanges a pointer value with a new value.
void*			 Sys_InterlockedExchangePointer( void*& ptr, void* exchange );

//! Performs an atomic compare-and-swap operation on a pointer.
void*			 Sys_InterlockedCompareExchangePointer( void*& ptr, void* comparand, void* exchange );

//! Yields the current thread to the operating system scheduler.
void			 Sys_Yield();

const int		 MAX_CRITICAL_SECTIONS = 4;

enum { CRITICAL_SECTION_ZERO = 0, CRITICAL_SECTION_ONE, CRITICAL_SECTION_TWO, CRITICAL_SECTION_THREE };

#endif // !__SYS_THREADING_H__
