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
	\brief A cross-platform mutex synchronization primitive for thread safety.

	The idSysMutex class provides a thread synchronization mechanism that allows exclusive access to shared resources across multiple threads. It abstracts the underlying operating system mutex
   implementation and is designed for use in multi-threaded environments where resource contention needs to be managed. The class supports both blocking and non-blocking lock acquisition modes, making
   it suitable for different synchronization requirements. The mutex can be locked to prevent other threads from accessing a critical section, and unlocked to allow other threads to proceed. The copy
   constructor and assignment operator are present but appear to be implemented as no-ops, suggesting that mutex objects should not be copied in a traditional sense, with the underlying system
   primitives handling synchronization state appropriately.

*/
class idSysMutex
{
public:
	/*!
		\brief Initializes a new mutex object

		This constructor creates a new mutex object using the system's mutex creation function. The mutex handle is initialized during construction and is ready to be used for synchronization
	   purposes. The implementation calls Sys_MutexCreate to set up the underlying system mutex.

	*/
	idSysMutex() { Sys_MutexCreate( handle ); }
	~idSysMutex() { Sys_MutexDestroy( handle ); }

	/*!
		\brief Acquires the mutex lock, optionally blocking if the lock is unavailable.

		The function attempts to acquire the mutex lock. If the blocking parameter is true and the lock is currently held by another thread, the function will block until the lock becomes available.
	   If blocking is false, the function will return immediately with a false value if the lock cannot be acquired. This function is typically used to ensure exclusive access to a shared resource.

		\param blocking Specifies whether to block the calling thread if the mutex is already locked
		\return True if the mutex was successfully acquired, false if the mutex was not acquired (only when blocking is false and the lock is unavailable)
	*/
	bool Lock( bool blocking = true ) { return Sys_MutexLock( handle, blocking ); }

	/*!
		\brief Releases the lock on the mutex.

		This function unlocks the mutex that was previously locked by a call to Lock. It allows other threads that are waiting to acquire the lock to proceed. The function does not return any value
	   and does not perform any error checking.

	*/
	void Unlock() { Sys_MutexUnlock( handle ); }

private:
	mutexHandle_t handle;

	/*!
		\brief Copy constructor for idSysMutex that initializes from another idSysMutex instance.

		This is the copy constructor for the idSysMutex class. It takes another idSysMutex instance as a parameter and initializes the current instance with its values. The implementation appears to
	   be empty, suggesting that the mutex synchronization primitives are not being copied or that the copying is handled at a lower level by the underlying operating system primitives. This could
	   indicate that mutex objects should not be copied in a traditional sense, or that the copy operation is a no-op.

		\param s The idSysMutex instance to copy from
	*/
	idSysMutex( const idSysMutex& s ) { }
	void operator=( const idSysMutex& s ) { }
};

/*!
	\class idScopedCriticalSection
	\brief A scoped critical section that automatically manages mutex locking and unlocking.

	The idScopedCriticalSection class provides a RAII-based solution for managing mutex locking and unlocking in a thread-safe manner. It automatically acquires a lock on a provided mutex during
   construction and releases the lock upon destruction, ensuring proper cleanup even if exceptions occur. This design intent promotes safe concurrent access to shared resources by encapsulating the
   locking mechanism within the object's lifetime. The class is intended to be used in scenarios where critical sections need to be managed without manual lock/unlock calls, reducing the risk of
   deadlocks and improving code maintainability. The mutex reference passed to the constructor is used exclusively for lock management, with no ownership transfer implied.

*/
class idScopedCriticalSection
{
public:
	/*!
		\brief Constructs a scoped critical section and locks the provided mutex.
		\param m Reference to the mutex to be locked and managed by this scoped critical section
	*/
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
	\brief A cross-platform synchronization primitive for thread coordination.

	The idSysSignal class provides a wrapper around system-level signal primitives to enable thread synchronization and coordination across different execution contexts. It supports both automatic and
   manual reset modes, allowing for flexible signaling behavior in multithreaded applications. The class is designed to work with platform-specific system calls through the Sys_SignalCreate,
   Sys_SignalWait, and associated functions, making it a low-level synchronization mechanism for engine components that require thread-safe communication. The class handles the creation and management
   of underlying system signal resources, providing methods to raise signals, clear them, and wait for them with optional timeouts. The copy constructor and assignment operator are implemented
   trivially, indicating that copying of signal objects should be avoided as it may lead to undefined behavior.

*/
class idSysSignal
{
public:
	static const int WAIT_INFINITE = -1;

	/*!
		\brief Constructs an idSysSignal object and initializes the system signal handle.

		The constructor initializes a system signal object by calling Sys_SignalCreate to set up the underlying system signal mechanism. The manualReset parameter determines whether the signal should
	   be automatically reset after a wait operation or manually reset by the caller.

		\param manualReset Specifies whether the signal should be manually reset after a wait operation, defaults to false for automatic reset
	*/
	idSysSignal( bool manualReset = false ) { Sys_SignalCreate( handle, manualReset ); }
	~idSysSignal() { Sys_SignalDestroy( handle ); }

	/*!
		\brief Raises the system signal associated with this signal object

		This function is used to raise or trigger a system-level signal that was previously initialized with a handle. It is typically used in multi-threaded contexts to signal events between threads.
	   The function directly calls the platform-specific system signal raising function with the internal handle.

	*/
	void Raise() { Sys_SignalRaise( handle ); }

	//! Clears the system signal handle.
	void Clear() { Sys_SignalClear( handle ); }

	/*!
		\brief Waits for the signal object to enter a signalled state or until the specified timeout period expires

		The function blocks execution until the signal object is in a signalled state or the timeout period has elapsed. When the signal is received within the timeout period, the signalled state is
	   cleared. This function is commonly used for thread synchronization and coordination between different parts of a multithreaded application.

		\param timeout The maximum time to wait for the signal in milliseconds, or WAIT_INFINITE to wait indefinitely
		\return True if the signal object was in a signalled state when the function returned, false if the wait timed out
	*/
	bool Wait( int timeout = WAIT_INFINITE ) { return Sys_SignalWait( handle, timeout ); }

private:
	signalHandle_t handle;

	/*!
		\brief Constructs a new idSysSignal object as a copy of an existing idSysSignal object.

		The constructor initializes a new idSysSignal object by copying the state of an existing idSysSignal object passed as a parameter. This is a trivial copy constructor that performs no
	   additional operations beyond the default copy behavior.

		\param s The existing idSysSignal object to copy from
	*/
	idSysSignal( const idSysSignal& s ) { }
	void operator=( const idSysSignal& s ) { }
};

/*!
	\class idSysInterlockedInteger
	\brief Thread-safe interlocked integer implementation for atomic operations.

	This class provides a thread-safe implementation of an integer value with atomic operations for concurrent access. It is designed to support safe increment, decrement, addition, and subtraction
   operations across multiple threads without external synchronization. The class is intended for use in scenarios where atomic counters or coordination variables are needed in multi-threaded
   environments. The GetValue and SetValue methods provide access to the underlying integer value, though SetValue is not atomic and should be used carefully in concurrent contexts. The implementation
   leverages interlocked operations to ensure that all modification operations are completed atomically, preventing race conditions and ensuring data consistency in multi-threaded applications.

*/
class idSysInterlockedInteger
{
public:
	/*!
		\brief Initializes an idSysInterlockedInteger object with a zero value.

		This constructor initializes the internal value of the idSysInterlockedInteger object to zero. It is used to create a new instance of the interlocked integer class, which provides thread-safe
	   operations for integer values.

	*/
	idSysInterlockedInteger() :
		value( 0 )
	{
	}

	/*!
		\brief Atomically increments the integer and returns the new value.

		This function performs an atomic increment operation on the internal integer value. It ensures that the increment is thread-safe and can be safely called from multiple threads concurrently.
	   The function returns the new value after incrementing.

		\return The new value of the integer after incrementing
	*/
	int	 Increment() { return Sys_InterlockedIncrement( value ); }

	/*!
		\brief Atomically decrements the integer and returns the new value.

		This function performs an atomic decrement operation on the integer value. It ensures that the decrement is thread-safe by using interlocked operations. The function returns the new value of
	   the integer after the decrement has been applied.

		\return The new value of the integer after the decrement operation has been completed.
	*/
	int	 Decrement() { return Sys_InterlockedDecrement( value ); }

	/*!
		\brief Atomically adds a value to the integer and returns the new value

		This function performs an atomic addition operation on an integer value. It takes the current value of the integer, adds the specified value to it, and returns the new resulting value. The
	   operation is guaranteed to be atomic, meaning it will complete without interference from other threads. This is useful for thread-safe counting and coordination operations.

		\param v the value to be added to the integer
		\return the new value of the integer after adding the specified value
	*/
	int	 Add( int v ) { return Sys_InterlockedAdd( value, ( interlockedInt_t )v ); }

	/*!
		\brief Atomically subtracts a value from the integer and returns the new value.

		This function performs an atomic subtraction operation on the internal integer value. It subtracts the specified value from the current value and returns the updated result. The operation is
	   thread-safe and ensures that no other thread can modify the value during the subtraction.

		\param v The value to subtract from the integer
		\return The new value of the integer after subtracting v
	*/
	int	 Sub( int v ) { return Sys_InterlockedSub( value, ( interlockedInt_t )v ); }

	//! Returns the current value of the interlocked integer.
	int	 GetValue() const { return value; }

	/*!
		\brief Sets a new value for the interlocked integer, with the operation not being atomic.

		This function assigns a new integer value to the internal interlocked integer storage. The operation is not atomic, meaning it may not be thread-safe and should be used with caution in
	   multi-threaded environments. The value parameter is cast to the interlockedInt_t type before assignment.

		\param v The new integer value to set
	*/
	void SetValue( int v ) { value = ( interlockedInt_t )v; }

private:
	interlockedInt_t value;
};

/*!
	\class idSysInterlockedPointer
	\brief Thread-safe interlocked pointer class for atomic pointer operations in multi-threaded environments.

	This class provides a thread-safe mechanism for managing pointer values through atomic operations. It is designed to support concurrent access to pointer data structures where atomic updates,
   compares, and reads are required. The implementation leverages system-level interlocked operations to ensure that pointer modifications occur atomically, preventing data races in multi-threaded
   scenarios. The class is templated to work with any pointer type, making it flexible for various use cases throughout the engine. It supports three primary operations: setting a new pointer value
   atomically, comparing and exchanging pointer values only when a condition is met, and retrieving the current pointer value in a thread-safe manner. The interlocked pointer is intended to be used in
   performance-critical sections where thread safety is paramount but explicit locking mechanisms are undesirable. This design allows for efficient concurrent access patterns while maintaining the
   integrity of pointer data across multiple threads.

*/
template<typename T>
class idSysInterlockedPointer
{
public:
	/*!
		\brief Initializes the interlocked pointer with a null value.

		This constructor initializes the internal pointer to NULL, setting up the interlocked pointer object for use. The interlocked pointer is designed to provide thread-safe operations on the
	   contained pointer value.

	*/
	idSysInterlockedPointer() :
		ptr( NULL )
	{
	}

	/*!
		\brief Atomically sets the pointer to a new value and returns the previous pointer value

		This function performs an atomic operation to replace the current pointer value with a new pointer value. It is designed to be thread-safe and ensures that only one thread can modify the
	   pointer at a time. The function returns the value of the pointer before the update occurred, allowing callers to observe the previous state. This implementation uses the
	   Sys_InterlockedExchangePointer system call to achieve atomicity.

		\param newPtr The new pointer value to set
		\return The previous value of the pointer before it was updated
	*/
	T* Set( T* newPtr ) { return ( T* )Sys_InterlockedExchangePointer( ( void*& )ptr, newPtr ); }

	/*!
		\brief Atomically exchanges the pointer value only if the current pointer equals the comparison pointer

		This function performs an atomic compare-and-exchange operation on the stored pointer. It compares the current pointer value with the provided comparison pointer, and if they are equal, it
	   replaces the current pointer with the new pointer value. The operation is atomic and thread-safe, ensuring consistency in multi-threaded environments. If the current pointer does not match the
	   comparison pointer, the function returns the current pointer without modification.

		\param comparePtr The pointer value to compare against the current stored pointer
		\param newPtr The new pointer value to set if the comparison succeeds
		\return The original pointer value if the comparison fails, or the new pointer value if the comparison succeeds
	*/
	T* CompareExchange( T* comparePtr, T* newPtr ) { return ( T* )Sys_InterlockedCompareExchangePointer( ( void*& )ptr, comparePtr, newPtr ); }

	/*!
		\brief Returns the current value of the pointer in a thread-safe manner

		This function provides a thread-safe way to retrieve the current value of the pointer stored in the idSysInterlockedPointer instance. It is designed to be used in multi-threaded environments
	   where atomic operations are required to ensure consistency. The function simply returns the internal pointer value without performing any modifications, making it a read-only operation.

		\return A pointer to the current value stored in the interlocked pointer
	*/
	T* Get() const { return ptr; }

private:
	T* ptr;
};

/*!
	\class idSysThread
	\brief idSysThread is a cross-platform thread management class that provides functionality for creating, controlling, and synchronizing system threads within the Roguestrad engine.

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

*/
class idSysThread
{
public:
	/*!
		\brief Initializes a new instance of the idSysThread class with default values for all member variables.

		This constructor initializes the idSysThread object by setting all its internal flags and handles to their default states. The threadHandle is initialized to zero, indicating no active thread.
	   The isWorker flag is set to false, meaning this thread is not yet configured as a worker. The isRunning flag is false, indicating the thread has not started executing. The isTerminating flag is
	   false, meaning the thread is not in the process of being terminated. The moreWorkToDo flag is false, signifying there is no pending work. Finally, the signalWorkerDone flag is true, indicating
	   that the worker thread is ready to process tasks.

	*/
	idSysThread();

	/*!
		\brief Destroys the system thread and ensures proper cleanup of thread resources.

		The destructor stops the thread if it is running and cleans up the thread handle. If a thread handle exists, it calls the system-specific function to destroy the thread, releasing all
	   associated system resources.

	*/
	virtual ~idSysThread();

	//! Returns the name of the thread as a null-terminated string.
	const char* GetName() const { return name.c_str(); }

	//! Returns the thread handle for this system thread
	uintptr_t	GetThreadHandle() const { return threadHandle; }

	//! Checks whether the system thread is currently running.
	bool		IsRunning() const { return isRunning; }

	//! Checks whether the thread is in the process of terminating.
	bool		IsTerminating() const { return isTerminating; }

	/*!
		\brief Initializes and starts a new system thread with the specified parameters.

		This function creates and starts a new thread using the system's threading API. It first checks if a thread is already running and returns false if so. It then destroys any existing thread
	   handle and creates a new one using the provided thread name, core affinity, priority, and stack size. The thread procedure is set to the ThreadProc method of the class instance.

		\param name name
		\param core core_t
		\param priority xthreadPriority
		\param stackSize int
		\return true if the thread was successfully started, false if a thread is already running
	*/
	bool		StartThread( const char* name, core_t core, xthreadPriority priority = THREAD_NORMAL, int stackSize = DEFAULT_THREAD_STACK_SIZE );

	/*!
		\brief Starts a worker thread with the specified parameters and waits for it to initialize.

		This function initializes a worker thread by setting the isWorker flag to true and then calling StartThread to create the thread. It waits indefinitely for a signal indicating that the worker
	   thread has completed its initialization before returning. The function returns false if the thread is already running, otherwise it returns the result of the StartThread call.

		\param name Name of the thread to be created
		\param core The core the thread should run on
		\param priority The priority level for the thread
		\param stackSize The stack size for the thread
		\return True if the thread was successfully started, false if a thread is already running.
	*/
	bool		StartWorkerThread( const char* name, core_t core, xthreadPriority priority = THREAD_NORMAL, int stackSize = DEFAULT_THREAD_STACK_SIZE );

	/*!
		\brief Stops the system thread, optionally waiting for its termination.

		This function stops the system thread by setting the termination flag. If the thread is a worker thread, it signals the worker to stop and waits for completion. For non-worker threads, it
	   simply sets the termination flag. If the wait parameter is true, the function will block until the thread has fully terminated.

		\param wait If true, the function will wait for the thread to fully terminate before returning.
	*/
	void		StopThread( bool wait = true );

	/*!
		\brief Waits for a thread to complete its execution

		This function handles waiting for thread completion in different scenarios. For worker threads, it waits for a signal indicating work completion. For regular running threads, it destroys the
	   thread handle and resets it to zero. This function can be called from multiple threads and is used to ensure proper synchronization between threads, particularly in the context of render and
	   game thread coordination

	*/
	void		WaitForThread();

	/*!
		\brief Signals the worker thread that new work is available to process.

		This function is used to notify a worker thread that work is available. It should only be called on worker threads and is typically invoked from other threads to indicate that new tasks are
	   ready for processing. The function sets a flag indicating work is available and raises a signal to wake up the worker thread.

	*/
	void		SignalWork();

	/*!
		\brief Checks if the worker thread has completed its assigned work without blocking.

		This function determines whether the worker thread has finished its task by checking if the signal indicating work completion has been set. It is designed to be called from other threads and
	   returns immediately without waiting. The function only checks the completion signal when the current thread is operating as a worker. Note that the meaning of work being "done" may be limited
	   if other threads are continuously signaling new work.

		\return true if the worker thread has completed its work and the completion signal has been set, false otherwise
	*/
	bool		IsWorkDone();

protected:
	/*!
		\brief Executes the thread's main routine and returns an integer exit code.

		This function serves as the primary execution point for threads derived from idSysThread. It is not a pure virtual function to ensure proper cleanup during object destruction. The function
	   returns zero by default, indicating successful completion, though derived classes may override this behavior to implement specific thread logic.

		\return Integer exit code representing the result of thread execution, typically zero for success.
	*/
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

	/*!
		\brief Executes the thread's main loop and handles thread lifecycle management including worker thread coordination and exception handling.

		This function serves as the entry point for thread execution and manages the core thread loop logic. For worker threads, it continuously waits for work signals, executes tasks via the Run()
	   method, and coordinates with other threads through mutex and signal mechanisms. Non-worker threads simply execute the Run() method once. The function handles thread termination gracefully and
	   manages exception scenarios by logging warnings and terminating the process in case of unexpected errors.

		\param thread Pointer to the thread instance being executed
		\return Return value from the thread's Run() method
		\throws Throws an idException if an exception occurs during thread execution
	*/
	static int	  ThreadProc( idSysThread* thread );

	/*!
		\brief Copy constructor for idSysThread that initializes the object with values from another idSysThread instance.

		This is the copy constructor for the idSysThread class. It takes an existing idSysThread object and initializes the new object with its values. The implementation appears to be a placeholder
	   or default constructor, as it contains no body.

		\param s The idSysThread object to copy values from
	*/
	idSysThread( const idSysThread& s ) { }
	void operator=( const idSysThread& s ) { }
};

/*!
	\class idSysWorkerThreadGroup
	\brief Manages a group of worker threads for parallel execution.

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
*/
template<class threadType>
class idSysWorkerThreadGroup
{
public:
	/*!
		\brief Constructs a worker thread group with the specified name, number of threads, priority, and stack size.

		This constructor initializes a group of worker threads of type threadType. If numThreads is negative, it will run one thread inline. The actual number of threads created is the absolute value
	   of numThreads. Each thread is started with a unique name, core, priority, and stack size.

		\param name A string identifier used to name each thread in the group
		\param numThreads The number of worker threads to create, negative value indicates inline execution
		\param priority The thread priority level for the worker threads
		\param stackSize The stack size for each worker thread
	*/
	idSysWorkerThreadGroup( const char* name, int numThreads, xthreadPriority priority = THREAD_NORMAL, int stackSize = DEFAULT_THREAD_STACK_SIZE );

	/*!
		\brief Destroys the worker thread group and cleans up its thread list.

		This destructor releases all resources associated with the worker thread group, including deleting all threads in the internal thread list. It ensures proper cleanup of worker threads when the
	   group is no longer needed.

	*/
	virtual ~idSysWorkerThreadGroup();

	//! Returns the number of worker threads in the group.
	int			GetNumThreads() const { return threadList.Num(); }

	/*!
		\brief Returns a reference to the thread at the specified index in the thread group.

		This function retrieves the thread object at the given index from the internal thread list. The index must be within the valid range of the thread group to avoid undefined behavior. The
	   returned reference allows direct access to the thread object for manipulation or querying its state.

		\param i The index of the thread to retrieve from the group
		\return A reference to the thread object at the specified index
	*/
	threadType& GetThread( int i ) { return *threadList[i]; }

	/*!
		\brief Initializes a worker thread group with the specified name, number of threads, priority, and stack size.

		This constructor sets up a group of worker threads of the specified type. The number of threads can be negative, which will result in inline execution. The threads are started with the
	   provided name, priority, and stack size. Each thread is given a unique name by appending an index to the provided name.

		\param name name of the thread group
		\param numThreads number of threads to create, can be negative for inline execution
		\param priority priority level for the threads
		\param stackSize stack size for the threads
	*/
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
	singleThreaded	   = false;
	numThreads		   = abs( numThreads );
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
	\brief A thread synchronization utility that manages multiple signals for coordinating thread execution.

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

	/*!
		\brief Configures the synchronizer to use the specified number of threads by allocating or deallocating signal objects.

		This function adjusts the internal signal array to match the requested number of threads. It ensures that the number of signals matches the number of threads by either creating new signal
	   objects or deleting existing ones. The function also updates the busy count to reflect the new thread count and ensures memory synchronization through a memory barrier. The assert statement
	   verifies that the current busy count matches the number of signals before any modification.

		\param num The desired number of threads to configure the synchronizer for
	*/
	ID_INLINE void	 SetNumThreads( unsigned int num );

	/*!
		\brief Signals a synchronization point for the specified thread number

		This function decrements an internal busy counter and when it reaches zero, it resets the counter to the number of signals and raises all registered signals. This is typically used in
	   multi-threaded environments to synchronize thread execution

		\param threadNum The thread number to signal
	*/
	ID_INLINE void	 Signal( unsigned int threadNum );

	/*!
		\brief Waits for a specific thread to signal completion or until a timeout occurs.

		This function synchronizes with a thread by waiting for it to signal completion. It waits on a specific signal object associated with the given thread number. The function will block until the
	   thread signals completion or the specified timeout period expires. If the timeout value is set to WAIT_INFINITE, the function will wait indefinitely for the signal.

		\param threadNum The index of the thread to synchronize with
		\param timeout The maximum time to wait in milliseconds, or WAIT_INFINITE to wait indefinitely
		\return True if the thread signaled completion within the timeout period, false if the timeout expired
	*/
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
