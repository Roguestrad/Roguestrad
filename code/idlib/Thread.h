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
#ifndef __THREAD_H__
#define __THREAD_H__

/*!
	\class idSysMutex
	\brief Provides a C++ wrapper for system-level mutex synchronization primitives.
*/
class idSysMutex
{
public:
	//! Initializes a new mutex handle.
	idSysMutex() { Sys_MutexCreate( handle ); }
	~idSysMutex() { Sys_MutexDestroy( handle ); }

	//! Acquires the mutex, optionally blocking if it cannot be acquired immediately.
	bool Lock( bool blocking = true ) { return Sys_MutexLock( handle, blocking ); }

	//! Releases the lock on the mutex.
	void Unlock() { Sys_MutexUnlock( handle ); }

private:
	mutexHandle_t handle;

	//! Constructs a new mutex object as a copy of an existing mutex object.
	idSysMutex( const idSysMutex& s ) { }

	//! Assignment operator for idSysMutex that copies the state from another mutex
	void operator=( const idSysMutex& s ) { }
};

/*!
	\class idScopedCriticalSection
	\brief A helper class that automatically manages mutex locking and unlocking through RAII.
*/
class idScopedCriticalSection
{
public:
	//! Acquires a lock on the provided mutex during the lifetime of the scoped critical section.
	idScopedCriticalSection( idSysMutex& m ) :
		mutex( &m )
	{
		mutex->Lock();
	}
	~idScopedCriticalSection() { mutex->Unlock(); }

private:
	idSysMutex* mutex; // NOTE: making this a reference causes a TypeInfo crash
};

/*!
	\class idSysSignal
	\brief Provides a C++ wrapper for low-level system signal functions to synchronize thread operations.

	idSysSignal serves as a C++ wrapper for system-level signal objects that enable thread synchronization. The class enables threads to wait for specific events to occur, such as data becoming
   available or reaching a particular processing point. It supports both automatic and manual reset behaviors through its constructor parameter. The signal can be raised to indicate an event has
   occurred, cleared to reset its state, and waited upon with optional timeout functionality. Copy construction and assignment are supported, allowing for signal object duplication. This wrapper
   abstracts the underlying system signal mechanisms to provide a portable interface for thread coordination.

*/
class idSysSignal
{
public:
	static const int WAIT_INFINITE = -1;

	//! Creates a system signal object with an optional manual reset behavior.
	idSysSignal( bool manualReset = false ) { Sys_SignalCreate( handle, manualReset ); }
	~idSysSignal() { Sys_SignalDestroy( handle ); }

	//! Raises the system signal associated with this object.
	void Raise() { Sys_SignalRaise( handle ); }

	//! Clears the system signal handle
	void Clear() { Sys_SignalClear( handle ); }

	//! Waits for the signal object to enter a signalled state or until the timeout period expires
	bool Wait( int timeout = WAIT_INFINITE ) { return Sys_SignalWait( handle, timeout ); }

private:
	signalHandle_t handle;

	//! Copy constructs a system signal object from another system signal object.
	idSysSignal( const idSysSignal& s ) { }

	//! Assigns the value of another idSysSignal object to this object.
	void operator=( const idSysSignal& s ) { }
};

/*!
	\class idSysInterlockedInteger
	\brief Provides atomic operations for integer values in a multithreaded environment.

	This class serves as a wrapper for low-level system interlocked integer routines, enabling safe atomic operations on integer values across multiple threads. It supports increment, decrement,
   addition, and subtraction operations that are guaranteed to be atomic. The class maintains an internal integer value that can be queried or modified using atomic operations. The implementation
   ensures thread safety for all operations while providing a simple interface for managing shared integer state. The class is designed to be used in scenarios where multiple threads need to
   coordinate access to a shared counter or flag.

*/
class idSysInterlockedInteger
{
public:
	//! Initializes the interlocked integer with a value of zero.
	idSysInterlockedInteger() :
		value( 0 )
	{
	}

	//! Atomically increments the integer and returns the new value.
	int	 Increment() { return Sys_InterlockedIncrement( value ); }

	//! Atomically decrements the integer and returns the new value.
	int	 Decrement() { return Sys_InterlockedDecrement( value ); }

	//! Atomically adds a value to the integer and returns the new value
	int	 Add( int v ) { return Sys_InterlockedAdd( value, ( interlockedInt_t )v ); }

	//! Atomically subtracts a value from the integer and returns the new value
	int	 Sub( int v ) { return Sys_InterlockedSub( value, ( interlockedInt_t )v ); }

	//! Returns the current value of the integer
	int	 GetValue() const { return value; }

	//! Sets the integer value to the specified value.
	void SetValue( int v ) { value = ( interlockedInt_t )v; }

private:
	interlockedInt_t value;
};

/*!
	\class idSysInterlockedPointer
	\brief A thread-safe wrapper for atomic pointer operations.
*/
template<typename T>
class idSysInterlockedPointer
{
public:
	//! Initializes the interlocked pointer with a null value.
	idSysInterlockedPointer() :
		ptr( NULL )
	{
	}

	//! Atomically sets the pointer to a new value and returns the previous pointer value.
	T* Set( T* newPtr ) { return ( T* )Sys_InterlockedExchangePointer( ( void*& )ptr, newPtr ); }

	//! Atomically exchanges the pointer value only if it matches the expected value.
	T* CompareExchange( T* comparePtr, T* newPtr ) { return ( T* )Sys_InterlockedCompareExchangePointer( ( void*& )ptr, comparePtr, newPtr ); }

	//! Returns the current value of the pointer.
	T* Get() const { return ptr; }

private:
	T* ptr;
};

/*
================================================
idSysThread is an abstract base class, to be extended by classes implementing the
idSysThread::Run() method.

	class idMyThread : public idSysThread {
	public:
		virtual int Run() {
			// run thread code here
			return 0;
		}
		// specify thread data here
	};

	idMyThread thread;
	thread.Start( "myThread" );

A worker thread is a thread that waits in place (without consuming CPU)
until work is available. A worker thread is implemented as normal, except that, instead of
calling the Start() method, the StartWorker() method is called to start the thread.
Note that the Sys_CreateThread function does not support the concept of worker threads.

	class idMyWorkerThread : public idSysThread {
	public:
		virtual int Run() {
			// run thread code here
			return 0;
		}
		// specify thread data here
	};

	idMyWorkerThread thread;
	thread.StartThread( "myWorkerThread" );

	// main thread loop
	for ( ; ; ) {
		// setup work for the thread here (by modifying class data on the thread)
		thread.SignalWork();           // kick in the worker thread
		// run other code in the main thread here (in parallel with the worker thread)
		thread.WaitForThread();        // wait for the worker thread to finish
		// use results from worker thread here
	}

In the above example, the thread does not continuously run in parallel with the main Thread,
but only for a certain period of time in a very controlled manner. Work is set up for the
Thread and then the thread is signalled to process that work while the main thread continues.
After doing other work, the main thread can wait for the worker thread to finish, if it has not
finished already. When the worker thread is done, the main thread can safely use the results
from the worker thread.

Note that worker threads are useful on all platforms but they do not map to the SPUs on the PS3.
================================================
*/

/*!
	\class idSysThread
	\brief Manages system threads with support for worker threads and thread synchronization.

	The idSysThread class provides a cross-platform interface for creating and managing system threads. It supports both regular and worker threads, with methods to start, stop, and synchronize thread
   execution. The class includes functionality to check thread state, retrieve thread handles, and manage thread work completion. Worker threads are designed to handle background tasks and are
   particularly useful for offloading work from the main thread. The class handles thread lifecycle management including initialization, execution, and cleanup. Thread synchronization is supported
   through signaling and waiting mechanisms. The implementation is designed to work across different platforms while maintaining consistent behavior for thread operations.

*/
class idSysThread
{
public:
	//! Initializes a new instance of the idSysThread class.
	idSysThread();

	//! Destroys the system thread and ensures it is properly stopped.
	virtual ~idSysThread();

	//! Returns the name of the thread as a C-string.
	const char* GetName() const { return name.c_str(); }

	//! Returns the handle of the thread.
	uintptr_t	GetThreadHandle() const { return threadHandle; }

	//! Checks whether the system thread is currently running.
	bool		IsRunning() const { return isRunning; }

	//! Returns true if the thread is in the process of terminating.
	bool		IsTerminating() const { return isTerminating; }

	/*!
		\brief Starts a new thread with the specified parameters and returns true on success.

		This function initializes and starts a new thread using the system's thread creation API. It first checks if a thread is already running and returns false if so. The function sets up the
	   thread name, clears the termination flag, and destroys any existing thread handle. It then creates a new thread using the provided parameters including core affinity, priority, stack size, and
	   a reference to the current object as the thread procedure. The function returns true upon successful thread creation.

		\param name Name of the thread to be created
		\param core Core affinity for the thread
		\param priority Priority level for the thread
		\param stackSize Stack size for the thread in bytes
		\return True if the thread was successfully started, false if a thread is already running
	*/
	bool		StartThread( const char* name, core_t core, xthreadPriority priority = THREAD_NORMAL, int stackSize = DEFAULT_THREAD_STACK_SIZE );

	/*!
		\brief Starts a worker thread with the specified parameters and waits for it to initialize

		This function initializes a worker thread with the provided name, core assignment, priority, and stack size. It first checks if a thread is already running and returns false if so. It then
	   sets the thread type to worker, starts the thread, and waits indefinitely for a signal indicating the worker has completed its initialization phase

		\param name thread name
		\param core core assignment for the thread
		\param priority thread priority level
		\param stackSize stack size for the thread
		\return true if the worker thread was successfully started and initialized, false if a thread is already running
	*/
	bool		StartWorkerThread( const char* name, core_t core, xthreadPriority priority = THREAD_NORMAL, int stackSize = DEFAULT_THREAD_STACK_SIZE );

	//! Stops the system thread, with an optional wait for completion.
	void		StopThread( bool wait = true );

	//! Waits for the thread to complete execution, either by signaling completion or by destroying the thread handle.
	void		WaitForThread();

	//! Signals the thread to notify work is available and can be called from multiple other threads.
	void		SignalWork();

	//! Returns true if the work is done without waiting.
	bool		IsWorkDone();

protected:
	//! Executes the thread's main routine and returns an integer result.
	virtual int Run();

private:
	idStr		  name;
	uintptr_t	  threadHandle;
	bool		  isWorker;
	bool		  isRunning;
	volatile bool isTerminating;
	volatile bool moreWorkToDo;
	idSysSignal	  signalWorkerDone;
	idSysSignal	  signalMoreWorkToDo;
	idSysMutex	  signalMutex;

	//! Executes the thread's main routine and handles thread termination and exceptions.
	static int	  ThreadProc( idSysThread* thread );

	//! Constructs a new idSysThread object as a copy of an existing idSysThread object.
	idSysThread( const idSysThread& s ) { }

	//! Assigns the contents of another idSysThread object to this object.
	void operator=( const idSysThread& s ) { }
};

/*! \class idSysWorkerThreadGroup
	\brief idSysWorkerThreadGroup implements a group of worker threads that typically crunch through a collection of similar tasks.

	class idMyWorkerThread : public idSysThread {
	public:
		virtual int Run() {
			// run thread code here
			return 0;
		}
		// specify thread data here
	};

	idSysWorkerThreadGroup<idMyWorkerThread> workers( "myWorkers", 4 );
	for ( ; ; ) {
		for ( int i = 0; i < workers.GetNumThreads(); i++ ) {
			// workers.GetThread( i )-> // setup work for this thread
		}
		workers.SignalWorkAndWait();
		// use results from the worker threads here
	}

	The concept of worker thread Groups is probably most useful for tools and compilers.
	For instance, the AAS Compiler is using a worker thread group. Although worker threads
	will work well on the PC, Mac and the 360, they do not directly map to the PS3,
	in that the worker threads won't automatically run on the SPUs.

*/
template<class threadType>
class idSysWorkerThreadGroup
{
public:
	/*!
	\brief Constructs a worker thread group with the specified name, number of threads, priority, and stack size.

	The constructor initializes a group of worker threads with the given configuration. If the number of threads is negative, it will run as a single thread inline. The actual thread objects are
	created and started with the provided name, priority, and stack size. The thread names are generated by appending "_worker{i}" to the base name, where {i} is the thread index.

	\param name Name of the thread group
	\param numThreads Number of threads to create, negative value runs as single thread inline
	\param priority Thread priority level
	\param stackSize Stack size for each thread
*/
	idSysWorkerThreadGroup( const char* name, int numThreads, xthreadPriority priority = THREAD_NORMAL, int stackSize = DEFAULT_THREAD_STACK_SIZE );

	//! Destroys the worker thread group and cleans up its thread list.
	virtual ~idSysWorkerThreadGroup();

	//! Returns the number of threads in the worker thread group.
	int			GetNumThreads() const { return threadList.Num(); }

	//! Returns a reference to the thread at the specified index in the thread list.
	threadType& GetThread( int i ) { return *threadList[i]; }

	//! Signals work to worker threads and waits for their completion.
	void		SignalWorkAndWait();

private:
	idList<threadType*, TAG_THREAD> threadList;
	bool							runOneThreadInline; // use the signalling thread as one of the threads
	bool							singleThreaded;		// set to true for debugging
};

template<class threadType>
ID_INLINE idSysWorkerThreadGroup<threadType>::idSysWorkerThreadGroup( const char* name, int numThreads, xthreadPriority priority, int stackSize )
{
	runOneThreadInline = ( numThreads < 0 );

	//! Constructs a worker thread group with the specified name, number of threads, priority, and stack size.
	singleThreaded = false;
	numThreads	   = abs( numThreads );
	for( int i = 0; i < numThreads; i++ ) {
		threadType* thread = new( TAG_THREAD ) threadType;
		thread->StartWorkerThread( va( "%s_worker%i", name, i ), ( core_t )i, priority, stackSize );
		threadList.Append( thread );
	}
}

template<class threadType>
ID_INLINE idSysWorkerThreadGroup<threadType>::~idSysWorkerThreadGroup()
{
	threadList.DeleteContents();
}

template<class threadType>
ID_INLINE void idSysWorkerThreadGroup<threadType>::SignalWorkAndWait()
{
	if( singleThreaded ) {
		for( int i = 0; i < threadList.Num(); i++ ) {
			threadList[i]->Run();
		}
		return;
	}
	for( int i = 0; i < threadList.Num() - runOneThreadInline; i++ ) {
		threadList[i]->SignalWork();
	}
	if( runOneThreadInline ) { threadList[threadList.Num() - 1]->Run(); }
	for( int i = 0; i < threadList.Num() - runOneThreadInline; i++ ) {
		threadList[i]->WaitForThread();
	}
}

/*!
	\class idSysThreadSynchronizer
	\brief idSysThreadSynchronizer, allows a group of threads to synchronize with each other half-way through execution.
	idSysThreadSynchronizer sync;

	class idMyWorkerThread : public idSysThread {
	public:
		virtual int Run() {
			// perform first part of the work here
			sync.Synchronize( threadNum );	// synchronize all threads
			// perform second part of the work here
			return 0;
		}
		// specify thread data here
		unsigned int threadNum;
	};

	idSysWorkerThreadGroup<idMyWorkerThread> workers( "myWorkers", 4 );
	for ( int i = 0; i < workers.GetNumThreads(); i++ ) {
		workers.GetThread( i )->threadNum = i;
	}

	for ( ; ; ) {
		for ( int i = 0; i < workers.GetNumThreads(); i++ ) {
			// workers.GetThread( i )-> // setup work for this thread
		}
		workers.SignalWorkAndWait();
		// use results from the worker threads here
	}
*/
class idSysThreadSynchronizer
{
public:
	static const int WAIT_INFINITE = -1;

	//! Sets the number of threads for the synchronizer.
	ID_INLINE void	 SetNumThreads( unsigned int num );

	//! Signals a thread synchronization event.
	ID_INLINE void	 Signal( unsigned int threadNum );

	//! Waits for a specific thread signal with an optional timeout.
	ID_INLINE bool	 Synchronize( unsigned int threadNum, int timeout = WAIT_INFINITE );

private:
	idList<idSysSignal*, TAG_THREAD> signals;
	idSysInterlockedInteger			 busyCount;
};

ID_INLINE void idSysThreadSynchronizer::SetNumThreads( unsigned int num )
{
	assert( busyCount.GetValue() == signals.Num() );
	if( ( int )num != signals.Num() ) {
		signals.DeleteContents();
		signals.SetNum( ( int )num );
		for( unsigned int i = 0; i < num; i++ ) {
			signals[i] = new( TAG_THREAD ) idSysSignal();
		}
		busyCount.SetValue( num );
		SYS_MEMORYBARRIER;
	}
}

ID_INLINE void idSysThreadSynchronizer::Signal( unsigned int threadNum )
{
	if( busyCount.Decrement() == 0 ) {
		busyCount.SetValue( ( unsigned int )signals.Num() );
		SYS_MEMORYBARRIER;
		for( int i = 0; i < signals.Num(); i++ ) {
			signals[i]->Raise();
		}
	}
}

ID_INLINE bool idSysThreadSynchronizer::Synchronize( unsigned int threadNum, int timeout )
{
	return signals[threadNum]->Wait( timeout );
}

#endif // !__THREAD_H__
