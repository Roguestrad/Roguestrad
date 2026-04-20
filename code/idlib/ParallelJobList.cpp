/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2020 Robert Beckebans

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
#include "precompiled.h"
#pragma hdrstop
#include "ParallelJobList.h"

/*
================================================================================================

	Job and Job-List names

================================================================================================
*/

const char* jobNames[] = {
	ASSERT_ENUM_STRING( JOBLIST_RENDERER_FRONTEND, 0 ),
	ASSERT_ENUM_STRING( JOBLIST_RENDERER_BACKEND, 1 ),
	ASSERT_ENUM_STRING( JOBLIST_UTILITY, 9 ),
};

static const int MAX_REGISTERED_JOBS = 128;
struct registeredJob {
	jobRun_t	function;
	const char* name;
} registeredJobs[MAX_REGISTERED_JOBS];
static int	numRegisteredJobs;

/*!
	\brief Returns the name of a job list identified by the given ID

	This function retrieves the name associated with a specific job list ID from an internal array of job names

	\param id The ID of the job list to retrieve the name for
	\return The name of the job list as a null-terminated string
*/
const char* GetJobListName( jobListId_t id )
{
	return jobNames[id];
}

/*!
	\brief Checks if a job function is registered in the job system

	This function iterates through the list of registered jobs to determine if a specific job function has been registered. It compares the provided function pointer against all registered job
   functions and returns true if a match is found, otherwise false.

	\param function The job function pointer to check for registration
	\return True if the job function is registered, false otherwise
*/
static bool IsRegisteredJob( jobRun_t function )
{
	for( int i = 0; i < numRegisteredJobs; i++ ) {
		if( registeredJobs[i].function == function ) {
			return true;
		}
	}
	return false;
}

void RegisterJob( jobRun_t function, const char* name )
{
	if( IsRegisteredJob( function ) ) {
		return;
	}
	registeredJobs[numRegisteredJobs].function = function;
	registeredJobs[numRegisteredJobs].name	   = name;
	numRegisteredJobs++;
}

/*!
	\brief Returns the name of the job function

	The function searches through the registered jobs array to find a job that matches the provided function pointer and returns its name

	\param function pointer to the job function to look up
	\return The name of the job function if found, otherwise returns 'unknown'
*/
const char* GetJobName( jobRun_t function )
{
	for( int i = 0; i < numRegisteredJobs; i++ ) {
		if( registeredJobs[i].function == function ) {
			return registeredJobs[i].name;
		}
	}
	return "unknown";
}

idParallelJobRegistration::idParallelJobRegistration( jobRun_t function, const char* name )
{
	RegisterJob( function, name );
}

int				 globalSpuLocalStoreActive;
void*			 globalSpuLocalStoreStart;
void*			 globalSpuLocalStoreEnd;
idSysMutex		 globalSpuLocalStoreMutex;

/*
================================================================================================

	PS3

================================================================================================
*/

/*
================================================================================================

idParallelJobList_Threads

================================================================================================
*/

static idCVar	 jobs_longJobMicroSec( "jobs_longJobMicroSec", "10000", CVAR_INTEGER, "print a warning for jobs that take more than this number of microseconds" );

const static int MAX_THREADS = 32;

struct threadJobListState_t {
	/*!
		\brief Initializes a threadJobListState_t object with default values.

		Constructs a threadJobListState_t object and initializes all member variables to their default states. The jobList is set to NULL, version is initialized to 0xFFFFFFFF, signalIndex and
	   lastJobIndex are set to 0, and nextJobIndex is initialized to -1.

	*/
	threadJobListState_t() :
		jobList( NULL ),
		version( 0xFFFFFFFF ),
		signalIndex( 0 ),
		lastJobIndex( 0 ),
		nextJobIndex( -1 )
	{
	}

	/*!
		\brief Constructs a threadJobListState_t object with the specified version.

		Initializes the thread job list state with the given version and sets default values for other member variables.

		\param _version The version number for the thread job list state.
	*/
	threadJobListState_t( int _version ) :
		jobList( NULL ),
		version( _version ),
		signalIndex( 0 ),
		lastJobIndex( 0 ),
		nextJobIndex( -1 )
	{
	}
	idParallelJobList_Threads* jobList;
	int						   version;
	int						   signalIndex;
	int						   lastJobIndex;
	int						   nextJobIndex;
};

struct threadStats_t {
	unsigned int numExecutedJobs;
	unsigned int numExecutedSyncs;
	uint64		 submitTime;
	uint64		 startTime;
	uint64		 endTime;
	uint64		 waitTime;
	uint64		 threadExecTime[MAX_THREADS];
	uint64		 threadTotalTime[MAX_THREADS];
};

class idParallelJobList_Threads
{
public:
	/*!
		\brief Constructor for idParallelJobList_Threads that initializes the job list with specified parameters and allocates memory for jobs and syncs.

		This constructor initializes a parallel job list for threaded execution. It sets up the job list with the specified ID, priority, maximum number of jobs, and maximum number of syncs. The
	   constructor ensures proper allocation of memory for the job list and signal job counts, and initializes various internal state variables. The job list is initialized with a size that
	   accommodates the maximum number of jobs plus twice the maximum number of syncs plus one extra slot for updating the done count.

		\param id Unique identifier for the job list
		\param priority Priority level for the job list execution
		\param maxJobs Maximum number of jobs that can be stored in the list
		\param maxSyncs Maximum number of synchronization points that can be handled
		\throws Assertion error if the job list priority is set to NONE
	*/
	idParallelJobList_Threads( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs );

	/*!
		\brief Destroys the idParallelJobList_Threads object and ensures all pending jobs are completed.

		The destructor waits for all currently queued and executing jobs to finish before the object is destroyed. This ensures proper cleanup of resources and prevents any potential race conditions
	   or undefined behavior due to unfinished jobs during destruction.

	*/
	~idParallelJobList_Threads();

	/*!
		\brief Adds a job to the parallel job list for execution.

		This function adds a job to the job list for parallel execution. It first asserts that the job list is done adding jobs, then performs a debug check to ensure no duplicate job entries exist.
	   The function allocates space for the new job and initializes its function pointer and data pointer. If the job list is full, it prints debug information about the job types and throws an error.

		\param function Function pointer to the job to be executed
		\param data Pointer to the data to be passed to the job function
		\throws Error when the job list is full and cannot accommodate the new job
	*/
	ID_INLINE void AddJob( jobRun_t function, void* data );

	/*!
		\brief Inserts a synchronization point into the job list based on the specified sync type.

		This function adds a synchronization point to the job list, which can be either a signal or a synchronization barrier. When a SYNC_SIGNAL is specified, it allocates a new job with a signal
	   function and updates the signal job count. When a SYNC_SYNCHRONIZE is specified and there is an active signal, it adds a synchronization job and resets the signal state. The function asserts
	   that the job list is in a valid state before proceeding.

		\param syncType The type of synchronization point to insert, either SYNC_SIGNAL or SYNC_SYNCHRONIZE.
		\throws assertion failure if done is false or if hasSignal is true when inserting a SYNC_SIGNAL
	*/
	ID_INLINE void InsertSyncPoint( jobSyncType_t syncType );

	/*!
		\brief Submits a parallel job list for execution, optionally waiting for another job list to complete first.

		This function prepares and submits a list of parallel jobs for execution. It initializes the job list state, sets up synchronization mechanisms, and either delegates execution to a job manager
	   in threaded mode or runs jobs directly in the calling thread. The function handles waiting for a specified job list to complete before proceeding, if provided. It also collects timing
	   statistics for the job execution.

		\param waitForJobList_ Pointer to another job list that this job list should wait for completion before starting execution, or NULL if no waiting is required
		\param parallelism Specifies the level of parallelism to use for job execution
		\throws assertion failure if done is false, or if numSyncs exceeds maxSyncs, or if jobList.Num() exceeds maxJobs + numSyncs * 2, or if fetchLock.GetValue() is not zero
	*/
	void		   Submit( idParallelJobList_Threads* waitForJobList_, int parallelism );

	/*!
		\brief Waits for all parallel jobs in the job list to complete execution

		This function waits for all parallel jobs that have been submitted to the job list to finish executing. It first checks if there are any jobs in the list, and if so, verifies that the job list
	   was properly submitted. It then waits for all jobs to complete by monitoring the signalJobCount and numThreadsExecuting counters. The function also updates timing statistics and resets internal
	   state variables after waiting for job completion

	*/
	void		   Wait();

	/*!
		\brief Attempts to wait for all jobs in the job list to complete without blocking

		Returns true if all jobs have completed or if the job list is empty, otherwise returns false indicating that not all jobs have finished yet. This function is non-blocking and will return
	   immediately if jobs are still pending.

		\return true if all jobs in the list have completed or the list is empty, false if jobs are still pending
	*/
	bool		   TryWait();

	//! Returns true if the job list has been submitted for execution.
	bool		   IsSubmitted() const;

	//! Returns the number of jobs that have been executed by the thread pool.
	unsigned int   GetNumExecutedJobs() const
	{
		return threadStats.numExecutedJobs;
	}

	//! Returns the number of synchronization operations that have been executed.
	unsigned int GetNumSyncs() const
	{
		return threadStats.numExecutedSyncs;
	}

	//! Returns the submission time of the job list in microseconds.
	uint64 GetSubmitTimeMicroSec() const
	{
		return threadStats.submitTime;
	}

	//! Returns the start time of the thread in microseconds.
	uint64 GetStartTimeMicroSec() const
	{
		return threadStats.startTime;
	}

	//! Returns the finish time of the thread job list in microseconds.
	uint64 GetFinishTimeMicroSec() const
	{
		return threadStats.endTime;
	}

	//! Returns the wait time in microseconds for the thread statistics.
	uint64 GetWaitTimeMicroSec() const
	{
		return threadStats.waitTime;
	}

	//! Returns the total processing time across all threads in microseconds.
	uint64		GetTotalProcessingTimeMicroSec() const;

	//! Returns the total wasted time in microseconds across all threads in the parallel job list.
	uint64		GetTotalWastedTimeMicroSec() const;

	/*!
		\brief Returns the processing time in microseconds for a specific unit

		The function retrieves the execution time for a given unit identified by its index. It performs a bounds check to ensure the unit index is valid before accessing the corresponding entry in the
	   thread statistics array. If the unit index is out of bounds, the function returns zero

		\param unit The index of the unit for which to retrieve the processing time
		\return The processing time in microseconds for the specified unit, or zero if the unit index is invalid
	*/
	uint64		GetUnitProcessingTimeMicroSec( int unit ) const;

	/*!
		\brief Returns the amount of wasted time for a specific thread unit in microseconds.

		This function calculates the difference between the total time a thread unit was active and the actual execution time spent on tasks. The wasted time represents idle or overhead time for the
	   specified thread unit. The function performs bounds checking to ensure the unit index is valid, returning zero if the index is out of range.

		\param unit Index of the thread unit to query for wasted time
		\return The amount of time in microseconds that the specified thread unit was not actively executing tasks
	*/
	uint64		GetUnitWastedTimeMicroSec( int unit ) const;

	//! Returns the unique identifier of the job list.
	jobListId_t GetId() const
	{
		return listId;
	}

	//! Returns the priority of the job list.
	jobListPriority_t GetPriority() const
	{
		return listPriority;
	}

	//! Returns the version value stored in the version member variable.
	int GetVersion()
	{
		return version.GetValue();
	}

	/*!
		\brief Waits for another job list to complete and returns true if the wait was successful.

		This function checks if there is a guard object that tracks the completion status of another job list. If the guard exists and its value is greater than zero, it indicates that the other job
	   list has completed, and the function returns true. Otherwise, it returns false, indicating that the wait was not successful or the guard is not set.

		\return true if the wait was successful and the other job list has completed, false otherwise
	*/
	bool WaitForOtherJobList();

	//------------------------
	// This is thread safe and called from the job threads.
	//------------------------
	enum runResult_t { RUN_OK = 0, RUN_PROGRESS = BIT( 0 ), RUN_DONE = BIT( 1 ), RUN_STALLED = BIT( 2 ) };

	/*!
		\brief Executes a list of jobs in parallel on a specified thread

		This function runs a set of jobs on a worker thread, tracking execution time and updating thread statistics. It increments the thread counter before execution and decrements it after
	   completion, ensuring proper synchronization. The function measures execution time using system microsecond precision timing and accumulates the total time for each thread in deferred
	   statistics.

		\param threadNum The index of the thread executing the jobs
		\param state Reference to the state object controlling job execution
		\param singleJob Flag indicating whether to process a single job or multiple jobs
		\return The result of the internal job execution process
	*/
	int RunJobs( unsigned int threadNum, threadJobListState_t& state, bool singleJob );

private:
	static const int		 NUM_DONE_GUARDS = 4; // cycle through 4 guards so we can cyclicly chain job lists

	bool					 threaded;
	bool					 done;
	bool					 hasSignal;
	jobListId_t				 listId;
	jobListPriority_t		 listPriority;
	unsigned int			 maxJobs;
	unsigned int			 maxSyncs;
	unsigned int			 numSyncs;
	int						 lastSignalJob;
	idSysInterlockedInteger* waitForGuard;
	idSysInterlockedInteger	 doneGuards[NUM_DONE_GUARDS];
	int						 currentDoneGuard;
	idSysInterlockedInteger	 version;
	struct job_t {
		jobRun_t function;
		void*	 data;
		int		 executed;
	};
	idList<job_t, TAG_JOBLIST>					 jobList;
	idList<idSysInterlockedInteger, TAG_JOBLIST> signalJobCount;
	idSysInterlockedInteger						 currentJob;
	idSysInterlockedInteger						 fetchLock;
	idSysInterlockedInteger						 numThreadsExecuting;

	threadStats_t								 deferredThreadStats;
	threadStats_t								 threadStats;

	/*!
		\brief Executes jobs from a parallel job list on a specific thread, handling synchronization and tracking job execution state.

		This function runs jobs from a job list on the specified thread, managing synchronization points and tracking execution progress. It processes jobs in order, handling signals and
	   synchronization barriers. The function supports single job execution or continuous execution until completion. It tracks execution time for each thread and handles job completion by
	   decrementing signal job counts. Returns a status indicating the result of the job execution, such as progress, stall, or completion.

		\param threadNum The index of the thread executing this function
		\param state Reference to the job list state tracking execution progress
		\param singleJob If true, executes only one job; if false, continues until all jobs are processed
		\return Integer status indicating the result of job execution, which can include RUN_OK, RUN_STALLED, RUN_DONE, or RUN_PROGRESS flags combined using bitwise OR
	*/
	int											 RunJobsInternal( unsigned int threadNum, threadJobListState_t& state, bool singleJob );

	/*!
		\brief This function serves as a no-operation placeholder that does nothing when called.
		\param data Unused parameter that would typically hold data for processing
	*/
	static void									 Nop( void* data )
	{
	}

	static int JOB_SIGNAL;
	static int JOB_SYNCHRONIZE;
	static int JOB_LIST_DONE;
};

int idParallelJobList_Threads::JOB_SIGNAL;
int idParallelJobList_Threads::JOB_SYNCHRONIZE;
int idParallelJobList_Threads::JOB_LIST_DONE;

idParallelJobList_Threads::idParallelJobList_Threads( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs ) :
	threaded( true ),
	done( true ),
	hasSignal( false ),
	listId( id ),
	listPriority( priority ),
	numSyncs( 0 ),
	lastSignalJob( 0 ),
	waitForGuard( NULL ),
	currentDoneGuard( 0 ),
	jobList()
{
	assert( listPriority != JOBLIST_PRIORITY_NONE );

	this->maxJobs  = maxJobs;
	this->maxSyncs = maxSyncs;
	jobList.AssureSize( maxJobs + maxSyncs * 2 + 1 ); // syncs go in as dummy jobs and one more to update the doneCount
	jobList.SetNum( 0 );
	signalJobCount.AssureSize( maxSyncs + 1 ); // need one extra for submit
	signalJobCount.SetNum( 0 );

	memset( &deferredThreadStats, 0, sizeof( threadStats_t ) );
	memset( &threadStats, 0, sizeof( threadStats_t ) );
}

idParallelJobList_Threads::~idParallelJobList_Threads()
{
	Wait();
}

ID_INLINE void idParallelJobList_Threads::AddJob( jobRun_t function, void* data )
{
	assert( done );
#if defined( _DEBUG )
	// make sure there isn't already a job with the same function and data in the list
	if( jobList.Num() < 1000 ) // don't do this N^2 slow check on big lists
	{
		for( int i = 0; i < jobList.Num(); i++ ) {
			assert( jobList[i].function != function || jobList[i].data != data );
		}
	}
#endif
	if( 1 ) // JDC: this never worked in tech5!  !jobList.IsFull() ) {
	{
		job_t& job	 = jobList.Alloc();
		job.function = function;
		job.data	 = data;
		job.executed = 0;
	} else {
		// debug output to show us what is overflowing
		int currentJobCount[MAX_REGISTERED_JOBS] = {};

		for( int i = 0; i < jobList.Num(); ++i ) {
			const char* jobName = GetJobName( jobList[i].function );
			for( int j = 0; j < numRegisteredJobs; ++j ) {
				if( jobName == registeredJobs[j].name ) {
					currentJobCount[j]++;
					break;
				}
			}
		}

		// print the quantity of each job type
		for( int i = 0; i < numRegisteredJobs; ++i ) {
			if( currentJobCount[i] > 0 ) {
				idLib::Printf( "Job: %s, # %d", registeredJobs[i].name, currentJobCount[i] );
			}
		}
		idLib::Error( "Can't add job '%s', too many jobs %d", GetJobName( function ), jobList.Num() );
	}
}

ID_INLINE void idParallelJobList_Threads::InsertSyncPoint( jobSyncType_t syncType )
{
	assert( done );
	switch( syncType ) {
		case SYNC_SIGNAL: {
			assert( !hasSignal );
			if( jobList.Num() ) {
				assert( !hasSignal );
				signalJobCount.Alloc();
				signalJobCount[signalJobCount.Num() - 1].SetValue( jobList.Num() - lastSignalJob );
				lastSignalJob = jobList.Num();
				job_t& job	  = jobList.Alloc();
				job.function  = Nop;
				job.data	  = &JOB_SIGNAL;
				hasSignal	  = true;
			}
			break;
		}
		case SYNC_SYNCHRONIZE: {
			if( hasSignal ) {
				job_t& job	 = jobList.Alloc();
				job.function = Nop;
				job.data	 = &JOB_SYNCHRONIZE;
				hasSignal	 = false;
				numSyncs++;
			}
			break;
		}
	}
}

void idParallelJobList_Threads::Submit( idParallelJobList_Threads* waitForJobList, int parallelism )
{
	assert( done );
	assert( numSyncs <= maxSyncs );
	assert( ( unsigned int )jobList.Num() <= maxJobs + numSyncs * 2 );
	assert( fetchLock.GetValue() == 0 );

	done = false;
	currentJob.SetValue( 0 );

	memset( &deferredThreadStats, 0, sizeof( deferredThreadStats ) );
	deferredThreadStats.numExecutedJobs	 = jobList.Num() - numSyncs * 2;
	deferredThreadStats.numExecutedSyncs = numSyncs;
	deferredThreadStats.submitTime		 = Sys_Microseconds();
	deferredThreadStats.startTime		 = 0;
	deferredThreadStats.endTime			 = 0;
	deferredThreadStats.waitTime		 = 0;

	if( jobList.Num() == 0 ) {
		return;
	}

	if( waitForJobList != NULL ) {
		waitForGuard = &waitForJobList->doneGuards[waitForJobList->currentDoneGuard];
	} else {
		waitForGuard = NULL;
	}

	currentDoneGuard = ( currentDoneGuard + 1 ) & ( NUM_DONE_GUARDS - 1 );
	doneGuards[currentDoneGuard].SetValue( 1 );

	signalJobCount.Alloc();
	signalJobCount[signalJobCount.Num() - 1].SetValue( jobList.Num() - lastSignalJob );

	job_t& job	 = jobList.Alloc();
	job.function = Nop;
	job.data	 = &JOB_LIST_DONE;

	if( threaded ) {
		// hand over to the manager
		void SubmitJobList( idParallelJobList_Threads * jobList, int parallelism );
		SubmitJobList( this, parallelism );
	} else {
		// run all the jobs right here
		threadJobListState_t state( GetVersion() );
		RunJobs( 0, state, false );
	}
}

void idParallelJobList_Threads::Wait()
{
	if( jobList.Num() > 0 ) {
		// don't lock up but return if the job list was never properly submitted
		if( !verify( !done && signalJobCount.Num() > 0 ) ) {
			return;
		}

		bool   waited	 = false;
		uint64 waitStart = Sys_Microseconds();

		while( signalJobCount[signalJobCount.Num() - 1].GetValue() > 0 ) {
			Sys_Yield();
			waited = true;
		}
		version.Increment();
		while( numThreadsExecuting.GetValue() > 0 ) {
			Sys_Yield();
			waited = true;
		}

		jobList.SetNum( 0 );
		signalJobCount.SetNum( 0 );
		numSyncs	  = 0;
		lastSignalJob = 0;

		uint64 waitEnd				 = Sys_Microseconds();
		deferredThreadStats.waitTime = waited ? ( waitEnd - waitStart ) : 0;
	}
	memcpy( &threadStats, &deferredThreadStats, sizeof( threadStats ) );
	done = true;
}

bool idParallelJobList_Threads::TryWait()
{
	if( jobList.Num() == 0 || signalJobCount[signalJobCount.Num() - 1].GetValue() <= 0 ) {
		Wait();
		return true;
	}
	return false;
}

bool idParallelJobList_Threads::IsSubmitted() const
{
	return !done;
}

uint64 idParallelJobList_Threads::GetTotalProcessingTimeMicroSec() const
{
	uint64 total = 0;
	for( int unit = 0; unit < MAX_THREADS; unit++ ) {
		total += threadStats.threadExecTime[unit];
	}
	return total;
}

uint64 idParallelJobList_Threads::GetTotalWastedTimeMicroSec() const
{
	uint64 total = 0;
	for( int unit = 0; unit < MAX_THREADS; unit++ ) {
		total += threadStats.threadTotalTime[unit] - threadStats.threadExecTime[unit];
	}
	return total;
}

uint64 idParallelJobList_Threads::GetUnitProcessingTimeMicroSec( int unit ) const
{
	if( unit < 0 || unit >= MAX_THREADS ) {
		return 0;
	}
	return threadStats.threadExecTime[unit];
}

uint64 idParallelJobList_Threads::GetUnitWastedTimeMicroSec( int unit ) const
{
	if( unit < 0 || unit >= MAX_THREADS ) {
		return 0;
	}
	return threadStats.threadTotalTime[unit] - threadStats.threadExecTime[unit];
}

#ifndef _DEBUG
volatile float	  longJobTime;
volatile jobRun_t longJobFunc;
volatile void*	  longJobData;
#endif

int idParallelJobList_Threads::RunJobsInternal( unsigned int threadNum, threadJobListState_t& state, bool singleJob )
{
	if( state.version != version.GetValue() ) {
		// trying to run an old version of this list that is already done
		return RUN_DONE;
	}

	assert( threadNum < MAX_THREADS );

	if( deferredThreadStats.startTime == 0 ) {
		deferredThreadStats.startTime = Sys_Microseconds(); // first time any thread is running jobs from this list
	}

	int result = RUN_OK;

	do {
		// run through all signals and syncs before the last job that has been or is being executed
		// this loop is really an optimization to minimize the time spent in the fetchLock section below
		for( ; state.lastJobIndex < ( int )currentJob.GetValue() && state.lastJobIndex < jobList.Num(); state.lastJobIndex++ ) {
			if( jobList[state.lastJobIndex].data == &JOB_SIGNAL ) {
				state.signalIndex++;
				assert( state.signalIndex < signalJobCount.Num() );
			} else if( jobList[state.lastJobIndex].data == &JOB_SYNCHRONIZE ) {
				assert( state.signalIndex > 0 );
				if( signalJobCount[state.signalIndex - 1].GetValue() > 0 ) {
					// stalled on a synchronization point
					return ( result | RUN_STALLED );
				}
			} else if( jobList[state.lastJobIndex].data == &JOB_LIST_DONE ) {
				if( signalJobCount[signalJobCount.Num() - 1].GetValue() > 0 ) {
					// stalled on a synchronization point
					return ( result | RUN_STALLED );
				}
			}
		}

		// try to lock to fetch a new job
		if( fetchLock.Increment() == 1 ) {
			// grab a new job
			state.nextJobIndex = currentJob.Increment() - 1;

			// run through any remaining signals and syncs (this should rarely iterate more than once)
			for( ; state.lastJobIndex <= state.nextJobIndex && state.lastJobIndex < jobList.Num(); state.lastJobIndex++ ) {
				if( jobList[state.lastJobIndex].data == &JOB_SIGNAL ) {
					state.signalIndex++;
					assert( state.signalIndex < signalJobCount.Num() );
				} else if( jobList[state.lastJobIndex].data == &JOB_SYNCHRONIZE ) {
					assert( state.signalIndex > 0 );
					if( signalJobCount[state.signalIndex - 1].GetValue() > 0 ) {
						// return this job to the list
						currentJob.Decrement();
						// release the fetch lock
						fetchLock.Decrement();
						// stalled on a synchronization point
						return ( result | RUN_STALLED );
					}
				} else if( jobList[state.lastJobIndex].data == &JOB_LIST_DONE ) {
					if( signalJobCount[signalJobCount.Num() - 1].GetValue() > 0 ) {
						// return this job to the list
						currentJob.Decrement();
						// release the fetch lock
						fetchLock.Decrement();
						// stalled on a synchronization point
						return ( result | RUN_STALLED );
					}
					// decrement the done count
					doneGuards[currentDoneGuard].Decrement();
				}
			}
			// release the fetch lock
			fetchLock.Decrement();
		} else {
			// release the fetch lock
			fetchLock.Decrement();
			// another thread is fetching right now so consider stalled
			return ( result | RUN_STALLED );
		}

		// if at the end of the job list we're done
		if( state.nextJobIndex >= jobList.Num() ) {
			return ( result | RUN_DONE );
		}

		// execute the next job
		{
			uint64 jobStart = Sys_Microseconds();

			jobList[state.nextJobIndex].function( jobList[state.nextJobIndex].data );
			jobList[state.nextJobIndex].executed = 1;

			uint64 jobEnd = Sys_Microseconds();
			deferredThreadStats.threadExecTime[threadNum] += jobEnd - jobStart;

#ifndef _DEBUG
			if( jobs_longJobMicroSec.GetInteger() > 0 ) {
				if( jobEnd - jobStart > jobs_longJobMicroSec.GetInteger() && GetId() != JOBLIST_UTILITY ) {
					longJobTime				= ( jobEnd - jobStart ) * ( 1.0f / 1000.0f );
					longJobFunc				= jobList[state.nextJobIndex].function;
					longJobData				= jobList[state.nextJobIndex].data;
					const char* jobName		= GetJobName( jobList[state.nextJobIndex].function );
					const char* jobListName = GetJobListName( GetId() );
					idLib::Printf( "%1.1f milliseconds for a single '%s' job from job list %s on thread %d\n", longJobTime, jobName, jobListName, threadNum );
				}
			}
#endif
		}

		result |= RUN_PROGRESS;

		// decrease the job count for the current signal
		if( signalJobCount[state.signalIndex].Decrement() == 0 ) {
			// if this was the very last job of the job list
			if( state.signalIndex == signalJobCount.Num() - 1 ) {
				deferredThreadStats.endTime = Sys_Microseconds();
				return ( result | RUN_DONE );
			}
		}

	} while( !singleJob );

	return result;
}

/*
========================
idParallelJobList_Threads::RunJobs
========================
*/
int idParallelJobList_Threads::RunJobs( unsigned int threadNum, threadJobListState_t& state, bool singleJob )
{
	uint64 start = Sys_Microseconds();

	numThreadsExecuting.Increment();

	int result = RunJobsInternal( threadNum, state, singleJob );

	numThreadsExecuting.Decrement();

	deferredThreadStats.threadTotalTime[threadNum] += Sys_Microseconds() - start;

	return result;
}

bool idParallelJobList_Threads::WaitForOtherJobList()
{
	if( waitForGuard != NULL ) {
		if( waitForGuard->GetValue() > 0 ) {
			return true;
		}
	}
	return false;
}

idParallelJobList::idParallelJobList( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs, const idColor* color )
{
	assert( priority > JOBLIST_PRIORITY_NONE );
	this->jobListThreads = new( TAG_JOBLIST ) idParallelJobList_Threads( id, priority, maxJobs, maxSyncs );
	this->color			 = color;
}

idParallelJobList::~idParallelJobList()
{
	delete jobListThreads;
}

void idParallelJobList::AddJob( jobRun_t function, void* data )
{
	assert( IsRegisteredJob( function ) );
	jobListThreads->AddJob( function, data );
}

CellSpursJob128* idParallelJobList::AddJobSPURS()
{
	return NULL;
}

void idParallelJobList::InsertSyncPoint( jobSyncType_t syncType )
{
	jobListThreads->InsertSyncPoint( syncType );
}

void idParallelJobList::Wait()
{
	if( jobListThreads != NULL ) {
		jobListThreads->Wait();
	}
}

bool idParallelJobList::TryWait()
{
	bool done = true;
	if( jobListThreads != NULL ) {
		done &= jobListThreads->TryWait();
	}
	return done;
}

void idParallelJobList::Submit( idParallelJobList* waitForJobList, int parallelism )
{
	assert( waitForJobList != this );
	jobListThreads->Submit( ( waitForJobList != NULL ) ? waitForJobList->jobListThreads : NULL, parallelism );
}

bool idParallelJobList::IsSubmitted() const
{
	return jobListThreads->IsSubmitted();
}

unsigned int idParallelJobList::GetNumExecutedJobs() const
{
	return jobListThreads->GetNumExecutedJobs();
}

unsigned int idParallelJobList::GetNumSyncs() const
{
	return jobListThreads->GetNumSyncs();
}

uint64 idParallelJobList::GetSubmitTimeMicroSec() const
{
	return jobListThreads->GetSubmitTimeMicroSec();
}

uint64 idParallelJobList::GetStartTimeMicroSec() const
{
	return jobListThreads->GetStartTimeMicroSec();
}

uint64 idParallelJobList::GetFinishTimeMicroSec() const
{
	return jobListThreads->GetFinishTimeMicroSec();
}

uint64 idParallelJobList::GetWaitTimeMicroSec() const
{
	return jobListThreads->GetWaitTimeMicroSec();
}

uint64 idParallelJobList::GetTotalProcessingTimeMicroSec() const
{
	return jobListThreads->GetTotalProcessingTimeMicroSec();
}

uint64 idParallelJobList::GetTotalWastedTimeMicroSec() const
{
	return jobListThreads->GetTotalWastedTimeMicroSec();
}

uint64 idParallelJobList::GetUnitProcessingTimeMicroSec( int unit ) const
{
	return jobListThreads->GetUnitProcessingTimeMicroSec( unit );
}

uint64 idParallelJobList::GetUnitWastedTimeMicroSec( int unit ) const
{
	return jobListThreads->GetUnitWastedTimeMicroSec( unit );
}

jobListId_t idParallelJobList::GetId() const
{
	return jobListThreads->GetId();
}

/*
================================================================================================

idJobThread

================================================================================================
*/

const int JOB_THREAD_STACK_SIZE = 256 * 1024; // same size as the SPU local store

struct threadJobList_t {
	idParallelJobList_Threads* jobList;
	int						   version;
};

static idCVar jobs_prioritize( "jobs_prioritize", "1", CVAR_BOOL | CVAR_NOCHEAT, "prioritize job lists" );

class idJobThread : public idSysThread
{
public:
	/*!
		\brief Constructs a new job thread object.

		Initializes the job thread with default values for the job list pointers and thread number.

	*/
	idJobThread();
	~idJobThread();

	/*!
		\brief Initializes and starts a job thread on the specified core with the given thread number

		This function sets up a worker thread for processing job lists. It configures the thread name to follow the JLProc_%d pattern, ensuring compatibility with Linux thread name length limits. The
	   thread is started with normal priority and a specific stack size defined by JOB_THREAD_STACK_SIZE. The core parameter specifies which CPU core the thread should be bound to for optimal
	   performance.

		\param core CPU core number to bind the thread to
		\param threadNum Thread identifier used in the naming convention
	*/
	void Start( core_t core, unsigned int threadNum );

	/*!
		\brief Adds a job list to the queue for processing by the thread.

		This function adds a job list to the internal queue that is processed by the thread. It ensures thread safety by locking the mutex before adding the job list. The function waits if the queue
	   is full to prevent overflow. The job list is stored with its version information, and the internal counter is updated to reflect the new job list.

		\param jobList The job list to be added to the queue.
	*/
	void AddJobList( idParallelJobList_Threads* jobList );

private:
	threadJobList_t jobLists[MAX_JOBLISTS]; // cyclic buffer with job lists
	unsigned int	firstJobList;			// index of the last job list the thread grabbed
	unsigned int	lastJobList;			// index where the next job list to work on will be added
	idSysMutex		addJobMutex;

	unsigned int	threadNum;

	/*!
		\brief Executes the job processing loop for the thread, handling job lists and their execution with priority-based scheduling.

		The function implements the main execution loop for a job thread, where it processes job lists according to their priority. It fetches new job lists from a global queue, schedules jobs based
	   on priority, and executes them. The function handles job list completion, stalling, and yielding to avoid blocking other threads. It maintains a local list of job lists assigned to this thread
	   and manages their execution until all are completed or the thread is terminated.

		\return The return value is always 0, indicating successful completion of the thread execution loop.
	*/
	virtual int		Run();
};

idJobThread::idJobThread() :
	firstJobList( 0 ),
	lastJobList( 0 ),
	threadNum( 0 )
{
}

idJobThread::~idJobThread()
{
}

void idJobThread::Start( core_t core, unsigned int threadNum )
{
	this->threadNum = threadNum;
	// DG: change threadname from "JobListProcessor_%d" to "JLProc_%d", because Linux
	// has a 15 (+ \0) char limit for threadnames.
	// furthermore: va is not thread safe, use snPrintf instead
	char name[16];
	idStr::snPrintf( name, 16, "JLProc_%d", threadNum );
	StartWorkerThread( name, core, THREAD_NORMAL, JOB_THREAD_STACK_SIZE );
	// DG end
}

void idJobThread::AddJobList( idParallelJobList_Threads* jobList )
{
	// must lock because multiple threads may try to add new job lists at the same time
	addJobMutex.Lock();
	// wait until there is space available because in rare cases multiple versions of the same job lists may still be queued
	while( lastJobList - firstJobList >= MAX_JOBLISTS ) {
		Sys_Yield();
	}
	assert( lastJobList - firstJobList < MAX_JOBLISTS );
	jobLists[lastJobList & ( MAX_JOBLISTS - 1 )].jobList = jobList;
	jobLists[lastJobList & ( MAX_JOBLISTS - 1 )].version = jobList->GetVersion();
	lastJobList++;
	addJobMutex.Unlock();
}

int idJobThread::Run()
{
	threadJobListState_t threadJobListState[MAX_JOBLISTS];
	int					 numJobLists		= 0;
	int					 lastStalledJobList = -1;

	while( !IsTerminating() ) {
		// fetch any new job lists and add them to the local list
		if( numJobLists < MAX_JOBLISTS && firstJobList < lastJobList ) {
			threadJobListState[numJobLists].jobList		 = jobLists[firstJobList & ( MAX_JOBLISTS - 1 )].jobList;
			threadJobListState[numJobLists].version		 = jobLists[firstJobList & ( MAX_JOBLISTS - 1 )].version;
			threadJobListState[numJobLists].signalIndex	 = 0;
			threadJobListState[numJobLists].lastJobIndex = 0;
			threadJobListState[numJobLists].nextJobIndex = -1;
			numJobLists++;
			firstJobList++;
		}
		if( numJobLists == 0 ) {
			break;
		}

		int				  currentJobList = 0;
		jobListPriority_t priority		 = JOBLIST_PRIORITY_NONE;
		if( lastStalledJobList < 0 ) {
			// find the job list with the highest priority
			for( int i = 0; i < numJobLists; i++ ) {
				if( threadJobListState[i].jobList->GetPriority() > priority && !threadJobListState[i].jobList->WaitForOtherJobList() ) {
					priority	   = threadJobListState[i].jobList->GetPriority();
					currentJobList = i;
				}
			}
		} else {
			// try to hide the stall with a job from a list that has equal or higher priority
			currentJobList = lastStalledJobList;
			priority	   = threadJobListState[lastStalledJobList].jobList->GetPriority();
			for( int i = 0; i < numJobLists; i++ ) {
				if( i != lastStalledJobList && threadJobListState[i].jobList->GetPriority() >= priority && !threadJobListState[i].jobList->WaitForOtherJobList() ) {
					priority	   = threadJobListState[i].jobList->GetPriority();
					currentJobList = i;
				}
			}
		}

		// if the priority is high then try to run through the whole list to reduce the overhead
		// otherwise run a single job and re-evaluate priorities for the next job
		bool singleJob = ( priority == JOBLIST_PRIORITY_HIGH ) ? false : jobs_prioritize.GetBool();

		// try running one or more jobs from the current job list
		int	 result = threadJobListState[currentJobList].jobList->RunJobs( threadNum, threadJobListState[currentJobList], singleJob );

		if( ( result & idParallelJobList_Threads::RUN_DONE ) != 0 ) {
			// done with this job list so remove it from the local list
			for( int i = currentJobList; i < numJobLists - 1; i++ ) {
				threadJobListState[i] = threadJobListState[i + 1];
			}
			numJobLists--;
			lastStalledJobList = -1;
		} else if( ( result & idParallelJobList_Threads::RUN_STALLED ) != 0 ) {
			// yield when stalled on the same job list again without making any progress
			if( currentJobList == lastStalledJobList ) {
				if( ( result & idParallelJobList_Threads::RUN_PROGRESS ) == 0 ) {
					Sys_Yield();
				}
			}
			lastStalledJobList = currentJobList;
		} else {
			lastStalledJobList = -1;
		}
	}
	return 0;
}

/*!
	\brief Retrieves CPU core and package count information by parsing /proc/cpuinfo and using system calls

	This function determines the number of logical CPU cores, physical CPU cores, and CPU packages available on the system. It first attempts to parse the /proc/cpuinfo file to extract core and
   sibling information, which provides details about hyper-threading and CPU topology. If parsing fails, it falls back to using sysconf() to determine the number of processors. The results are stored
   in the provided reference parameters after being computed once and cached for subsequent calls.

	\param logicalNum Output parameter for the number of logical CPU cores
	\param coreNum Output parameter for the number of physical CPU cores
	\param packageNum Output parameter for the number of CPU packages
*/
extern void Sys_CPUCount( int& logicalNum, int& coreNum, int& packageNum );

// WINDOWS LOGICAL PROCESSOR LIMITS:
//
// http://download.microsoft.com/download/5/7/7/577a5684-8a83-43ae-9272-ff260a9c20e2/Hyper-thread_Windows.doc
//
//											Physical	Logical (Cores + HT)
// Windows XP Home Edition					1			2
// Windows XP Professional					2			4
// Windows Server 2003, Standard Edition	4			8
// Windows Server 2003, Enterprise Edition	8			16
// Windows Server 2003, Datacenter Edition	32			32
//
// Windows Vista							?			?
//
// Windows 7 Starter						1			32/64
// Windows 7 Home Basic						1			32/64
// Windows 7 Professional					2			32/64
//
//
// Hyperthreading is not dead yet.  Intel's Core i7 Processor is quad-core with HT for 8 logicals.

// DOOM3: We don't have that many jobs, so just set this fairly low so we don't spin up a ton of idle threads
#define MAX_JOB_THREADS 32
#define NUM_JOB_THREADS "2"
// clang-format off
#define JOB_THREAD_CORES	{	CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY,	\
								CORE_ANY, CORE_ANY, CORE_ANY, CORE_ANY }
// clang-format on

idCVar jobs_numThreads( "jobs_numThreads", NUM_JOB_THREADS, CVAR_INTEGER | CVAR_NOCHEAT, "number of threads used to crunch through jobs", 0, MAX_JOB_THREADS );

class idParallelJobManagerLocal : public idParallelJobManager
{
public:
	virtual ~idParallelJobManagerLocal()
	{
	}

	/*!
		\brief Initializes the parallel job manager by setting up worker threads and CPU core configurations

		This function initializes the parallel job manager by configuring worker threads for processing jobs. It determines the appropriate CPU cores for each thread based on the JOB_THREAD_CORES
	   constant, ensuring that at least MAX_JOB_THREADS cores are available. On consoles, threads are assigned to specific cores, while on PC systems, threads will utilize CORE_ANY. The function also
	   retrieves CPU information including logical cores, physical cores, and CPU packages.

	*/
	virtual void			   Init();

	/*!
		\brief Stops all worker threads managed by the parallel job manager.

		This function iterates through all worker threads managed by the idParallelJobManagerLocal instance and stops each thread by calling StopThread() on them. It is typically called during system
	   shutdown to ensure all parallel processing threads are properly terminated before the application exits.

	*/
	virtual void			   Shutdown();

	/*!
		\brief Allocates and initializes a new job list with the specified parameters.

		This function creates a new job list identified by the given ID and configures it with the specified priority, maximum number of jobs, and maximum number of synchronizations. It also
	   associates a color with the job list for visualization purposes. The function checks if a job list with the same ID already exists and skips allocation if it does. The newly created job list is
	   added to the manager's internal list of job lists.

		\param id Unique identifier for the job list
		\param priority Priority level for the job list scheduling
		\param maxJobs Maximum number of jobs that can be added to this job list
		\param maxSyncs Maximum number of synchronization points for this job list
		\param color Color associated with the job list for debugging or visualization
		\return A pointer to the newly allocated and initialized job list
	*/
	virtual idParallelJobList* AllocJobList( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs, const idColor* color );

	/*!
		\brief Frees a parallel job list by waiting for all threads to finish and then deleting the list.

		This function cleans up a job list by first ensuring all worker threads have finished executing their tasks. It then finds the specified job list in the internal collection, validates its
	   presence, waits for the list to complete, and finally deletes it. The function is not thread-safe and requires all threads to finish before proceeding with deletion.

		\param jobList Pointer to the job list to be freed, can be null
	*/
	virtual void			   FreeJobList( idParallelJobList* jobList );

	//! Returns the number of job lists managed by the parallel job manager.
	virtual int				   GetNumJobLists() const;

	//! Returns the number of free job lists available in the parallel job manager.
	virtual int				   GetNumFreeJobLists() const;

	/*!
		\brief Returns a pointer to the job list at the specified index.

		This function retrieves a job list from an internal array of job lists using the provided index. The index must be within valid bounds, otherwise the behavior is undefined.

		\param index The index of the job list to retrieve
		\return A pointer to the idParallelJobList at the specified index
	*/
	virtual idParallelJobList* GetJobList( int index );

	//! Returns the number of processing units available for parallel job management.
	virtual int				   GetNumProcessingUnits();

	//! Returns the number of logical CPU cores available to the system.
	virtual int				   GetLogicalCpuCores() const
	{
		return numLogicalCpuCores;
	}

	/*!
		\brief Waits for all job lists managed by this parallel job manager to complete their execution.

		This function iterates through all job lists stored in the manager and waits for each one to finish its execution. It ensures that no job list is left running before proceeding, making it
	   useful for synchronization purposes when all parallel tasks need to be completed before continuing with the next phase of execution.

	*/
	virtual void WaitForAllJobLists();

	/*!
		\brief Submits a job list to be processed by a specified number of threads.

		This function handles the submission of parallel jobs to a thread pool. It first checks if the maximum number of threads has been modified and updates the limit accordingly. Then it determines
	   the actual number of threads to use based on the provided parallelism parameter, which can be set to default, maximum cores, or maximum threads, or a specific number. The function ensures that
	   the number of threads does not exceed the maximum allowed. If the number of threads is zero or less, it runs the jobs sequentially on the calling thread. Otherwise, it distributes the job list
	   among the specified number of worker threads and signals them to start processing.

		\param jobList Pointer to the job list to be submitted for processing
		\param parallelism Specifies the number of threads to use for processing, or a special constant indicating how to determine the number
	*/
	void		 Submit( idParallelJobList_Threads* jobList, int parallelism );

private:
	idJobThread									   threads[MAX_JOB_THREADS];
	unsigned int								   maxThreads;
	int											   numPhysicalCpuCores;
	int											   numLogicalCpuCores;
	int											   numCpuPackages;
	idStaticList<idParallelJobList*, MAX_JOBLISTS> jobLists;
};

idParallelJobManagerLocal parallelJobManagerLocal;
idParallelJobManager*	  parallelJobManager = &parallelJobManagerLocal;

/*!
	\brief Submits a job list for parallel execution with the specified degree of parallelism

	This function dispatches a job list to the parallel job manager for execution across multiple threads. The job list contains tasks that will be processed in parallel according to the specified
   parallelism level. The function delegates the actual submission to the parallel job manager local instance.

	\param jobList Pointer to the job list containing tasks to be executed in parallel
	\param parallelism The degree of parallelism to use for executing the jobs
*/
void					  SubmitJobList( idParallelJobList_Threads* jobList, int parallelism )
{
	parallelJobManagerLocal.Submit( jobList, parallelism );
}

void idParallelJobManagerLocal::Init()
{
	// on consoles this will have specific cores for the threads, but on PC they will all be CORE_ANY
	core_t cores[] = JOB_THREAD_CORES;
	assert( sizeof( cores ) / sizeof( cores[0] ) >= MAX_JOB_THREADS );

	for( int i = 0; i < MAX_JOB_THREADS; i++ ) {
		threads[i].Start( cores[i], i );
	}
	maxThreads = jobs_numThreads.GetInteger();

	Sys_CPUCount( numLogicalCpuCores, numPhysicalCpuCores, numCpuPackages );
}

void idParallelJobManagerLocal::Shutdown()
{
	for( int i = 0; i < MAX_JOB_THREADS; i++ ) {
		threads[i].StopThread();
	}
}

idParallelJobList* idParallelJobManagerLocal::AllocJobList( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs, const idColor* color )
{
	for( int i = 0; i < jobLists.Num(); i++ ) {
		if( jobLists[i]->GetId() == id ) {
			// idStudio may cause job lists to be allocated multiple times
		}
	}
	idParallelJobList* jobList = new( TAG_JOBLIST ) idParallelJobList( id, priority, maxJobs, maxSyncs, color );
	jobLists.Append( jobList );
	return jobList;
}

void idParallelJobManagerLocal::FreeJobList( idParallelJobList* jobList )
{
	if( jobList == NULL ) {
		return;
	}
	// wait for all job threads to finish because job list deletion is not thread safe
	for( unsigned int i = 0; i < maxThreads; i++ ) {
		threads[i].WaitForThread();
	}
	int index = jobLists.FindIndex( jobList );
	assert( index >= 0 && jobLists[index] == jobList );
	jobLists[index]->Wait();
	delete jobLists[index];
	jobLists.RemoveIndexFast( index );
}

int idParallelJobManagerLocal::GetNumJobLists() const
{
	return jobLists.Num();
}

int idParallelJobManagerLocal::GetNumFreeJobLists() const
{
	return MAX_JOBLISTS - jobLists.Num();
}

idParallelJobList* idParallelJobManagerLocal::GetJobList( int index )
{
	return jobLists[index];
}

int idParallelJobManagerLocal::GetNumProcessingUnits()
{
	return maxThreads;
}

void idParallelJobManagerLocal::WaitForAllJobLists()
{
	// wait for all job lists to complete
	for( int i = 0; i < jobLists.Num(); i++ ) {
		jobLists[i]->Wait();
	}
}

void idParallelJobManagerLocal::Submit( idParallelJobList_Threads* jobList, int parallelism )
{
	if( jobs_numThreads.IsModified() ) {
		maxThreads = idMath::ClampInt( 0, MAX_JOB_THREADS, jobs_numThreads.GetInteger() );
		jobs_numThreads.ClearModified();
	}

	// determine the number of threads to use
	int numThreads = maxThreads;
	if( parallelism == JOBLIST_PARALLELISM_DEFAULT ) {
		numThreads = maxThreads;
	} else if( parallelism == JOBLIST_PARALLELISM_MAX_CORES ) {
		numThreads = numLogicalCpuCores;
	} else if( parallelism == JOBLIST_PARALLELISM_MAX_THREADS ) {
		numThreads = MAX_JOB_THREADS;
	} else if( parallelism > MAX_JOB_THREADS ) {
		numThreads = MAX_JOB_THREADS;
	} else {
		numThreads = parallelism;
	}

	// RB: users might have processors these days with more than 32 logical cores
	if( numThreads > MAX_JOB_THREADS ) {
		numThreads = MAX_JOB_THREADS;
	}

	if( numThreads <= 0 ) {
		threadJobListState_t state( jobList->GetVersion() );
		jobList->RunJobs( 0, state, false );
		return;
	}

	for( int i = 0; i < numThreads; i++ ) {
		threads[i].AddJobList( jobList );
		threads[i].SignalWork();
	}
}
