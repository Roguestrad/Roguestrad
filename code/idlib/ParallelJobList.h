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
#ifndef __PARALLELJOBLIST_H__
#define __PARALLELJOBLIST_H__

struct CellSpursJob128;
class idColor;

typedef void ( *jobRun_t )( void* );

enum jobSyncType_t { SYNC_NONE, SYNC_SIGNAL, SYNC_SYNCHRONIZE };

// NOTE: keep in sync with jobNames[]
enum jobListId_t {
	JOBLIST_RENDERER_FRONTEND = 0,
	JOBLIST_RENDERER_BACKEND  = 1,
	JOBLIST_UTILITY			  = 9, // won't print over-time warnings

	MAX_JOBLISTS = 32 // the editor may cause quite a few to be allocated
};

compile_time_assert( CONST_ISPOWEROFTWO( MAX_JOBLISTS ) );

enum jobListPriority_t { JOBLIST_PRIORITY_NONE, JOBLIST_PRIORITY_LOW, JOBLIST_PRIORITY_MEDIUM, JOBLIST_PRIORITY_HIGH };

enum jobListParallelism_t {
	JOBLIST_PARALLELISM_DEFAULT		= -1, // use "jobs_numThreads" number of threads
	JOBLIST_PARALLELISM_MAX_CORES	= -2, // use a thread for each logical core (includes hyperthreads)
	JOBLIST_PARALLELISM_MAX_THREADS = -3  // use the maximum number of job threads, which can help if there is IO to overlap
};

#define assert_spu_local_store( ptr )
#define assert_not_spu_local_store( ptr )

// Admer: prevent Visual Studio from incorrectly highlighting AddJob, as the same is defined in winspool.h
#ifdef AddJob
	#undef AddJob
#endif

/*!
	\class idParallelJobList
	\brief Manages parallel execution of jobs with synchronization support

	Provides a mechanism for organizing and executing jobs in parallel while supporting synchronization points and performance tracking. The class maintains a list of jobs that can be submitted for
   parallel execution with optional dependencies on other job lists. It supports different synchronization types and provides detailed timing information for performance analysis. Jobs are executed
   using a thread pool managed by the underlying job list threads system. The class tracks execution statistics including job counts, timing data, and resource utilization across multiple processing
   units. Memory management is handled through the destructor which properly releases thread resources.

*/
class idParallelJobList
{
	friend class idParallelJobManagerLocal;

public:
	/*!
		\brief Adds a job to the parallel job list for execution

		This function registers a job with the parallel job list by adding it to the underlying job list threads structure. It first validates that the job function is properly registered before
	   adding it. The job will be executed in a parallel thread when the job list is processed.

		\param function The job function to be added to the job list
		\param data The data to be passed to the job function when executed
		\throws assertion failure if the job function is not registered
	*/
	void			 AddJob( jobRun_t function, void* data );

	//! Adds a SPURS job to the parallel job list and returns a pointer to the newly added job.
	CellSpursJob128* AddJobSPURS();

	/*!
		\brief Inserts a synchronization point into the job list using the specified sync type

		This function adds a synchronization point to the job list, which will be processed by the job list threads. The synchronization point is defined by the provided sync type parameter. The
	   function delegates the actual insertion to the internal job list threads object.

		\param syncType The synchronization type to use for the sync point
	*/
	void			 InsertSyncPoint( jobSyncType_t syncType );

	/*!
		\brief Submits the jobs in this list for execution

		This function submits all jobs contained within this job list to the parallel execution system. It can optionally wait for another job list to complete before starting execution. The
	   parallelism parameter controls how many threads can execute jobs concurrently. The function ensures that the wait list is not the same as this job list to prevent circular dependencies.

		\param waitForJobList Optional job list that must complete before this job list starts execution, can be NULL
		\param parallelism Controls the number of concurrent threads that will execute jobs, defaults to JOBLIST_PARALLELISM_DEFAULT
		\throws assertion failure if waitForJobList is the same object as this
	*/
	void			 Submit( idParallelJobList* waitForJobList = NULL, int parallelism = JOBLIST_PARALLELISM_DEFAULT );

	/*!
		\brief Waits for all jobs in the parallel job list to complete execution.

		This function waits for all jobs in the parallel job list to complete execution. It checks if the job list threads are initialized and then calls the Wait method on them. If the job list
	   threads are not initialized, the function does nothing. The function will block until all jobs are finished, potentially spinning in place if any jobs are not yet completed.

	*/
	void			 Wait();

	/*!
		\brief Attempts to wait for all jobs in the list to complete and returns immediately, indicating whether all jobs finished.

		This function checks if all jobs in the parallel job list have completed execution. It returns true if all jobs are done, or false if any job is still running. The function does not block and
	   will return immediately, making it suitable for non-blocking synchronization checks. If there are no threads associated with the job list, it will return true immediately.

		\return true if all jobs in the list have completed, false if any job is still running
	*/
	bool			 TryWait();

	//! Returns true if the job list has been submitted.
	bool			 IsSubmitted() const;

	//! Returns the number of jobs that have been executed in this job list.
	unsigned int	 GetNumExecutedJobs() const;

	//! Returns the number of sync points in the job list.
	unsigned int	 GetNumSyncs() const;

	//! Returns the time at which the job list was submitted in microseconds.
	uint64			 GetSubmitTimeMicroSec() const;

	//! Returns the time at which execution of this job list started in microseconds.
	uint64			 GetStartTimeMicroSec() const;

	//! Returns the time at which all jobs in the list were executed.
	uint64			 GetFinishTimeMicroSec() const;

	//! Returns the time the host thread waited for the job list to finish in microseconds.
	uint64			 GetWaitTimeMicroSec() const;

	//! Returns the total processing time in microseconds for all units in the job list.
	uint64			 GetTotalProcessingTimeMicroSec() const;

	//! Returns the total wasted time in microseconds across all threads in the job list.
	uint64			 GetTotalWastedTimeMicroSec() const;

	//! Returns the processing time in microseconds for the specified unit.
	uint64			 GetUnitProcessingTimeMicroSec( int unit ) const;

	//! Returns the amount of time a specific unit wasted while processing the job list.
	uint64			 GetUnitWastedTimeMicroSec( int unit ) const;

	//! Returns the ID of the job list.
	jobListId_t		 GetId() const;

	//! Returns the color used for profiling.
	const idColor*	 GetColor() const { return this->color; }

private:
	class idParallelJobList_Threads* jobListThreads;
	const idColor*					 color;

	/*!
		\brief Initializes a new parallel job list with the specified parameters.

		Constructs a new parallel job list object with the given job list ID, priority, maximum number of jobs, and maximum number of synchronizations. The color parameter is used to associate a color
	   with the job list for visualization purposes. The function asserts that the priority is not set to NONE.

		\param id Unique identifier for the job list
		\param priority Priority level for the job list
		\param maxJobs Maximum number of jobs that can be handled
		\param maxSyncs Maximum number of synchronizations allowed
		\param color Color associated with the job list for visualization
		\throws Assertion failure if the priority is set to JOBLIST_PRIORITY_NONE
	*/
	idParallelJobList( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs, const idColor* color );

	/*!
		\brief Destructor for idParallelJobList that cleans up the job list threads.

		This destructor is responsible for properly cleaning up the resources associated with the idParallelJobList instance. It specifically deletes the jobListThreads member, which manages the
	   threads used for parallel job execution. The destructor ensures that all allocated thread resources are released when the object goes out of scope.

	*/
	~idParallelJobList();
};

/*!
	\class idParallelJobManager
	\brief Manages parallel job execution and job list allocation for multi-threaded processing.

	The idParallelJobManager class serves as the central coordinator for parallel task execution within the engine, providing an interface for managing job lists and their associated processing units.
   It abstracts the underlying threading implementation, allowing different backends to handle the actual parallelization. The class supports job list allocation with specific priorities and resource
   limits, and provides mechanisms for synchronizing completion of all pending jobs. The manager maintains a registry of active job lists and exposes information about available processing units to
   optimize workload distribution. This design enables efficient utilization of multi-core systems while maintaining separation between task definition and execution scheduling.

*/
class idParallelJobManager
{
public:
	virtual ~idParallelJobManager() { }

	virtual void			   Init()	  = 0;
	virtual void			   Shutdown() = 0;

	virtual idParallelJobList* AllocJobList( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs, const idColor* color ) = 0;
	virtual void			   FreeJobList( idParallelJobList* jobList )																					 = 0;

	virtual int				   GetNumJobLists() const	  = 0;
	virtual int				   GetNumFreeJobLists() const = 0;
	virtual idParallelJobList* GetJobList( int index )	  = 0;

	virtual int				   GetNumProcessingUnits()	  = 0;
	virtual int				   GetLogicalCpuCores() const = 0; // RB

	virtual void			   WaitForAllJobLists() = 0;
};

extern idParallelJobManager* parallelJobManager;

/*!
	\brief Registers a job function with an associated debug name for parallel job tracking.

	This function allows registering job functions that can be tracked and debugged by associating them with a descriptive name. The registration is only performed if the job function is not already
   registered. The function stores the job function and its name in an internal array, incrementing a counter to track the number of registered jobs. This is typically used for debugging and
   monitoring parallel execution.

	\param function The job function to register
	\param name The debug name associated with the job function
*/
void						 RegisterJob( jobRun_t function, const char* name );

/*!
	\class idParallelJobRegistration
	\brief Manages registration of parallel jobs for execution within the engine's parallel processing system.

	This class serves as a registration mechanism for parallel jobs that can be executed concurrently within the engine's parallel processing framework. The class stores a function pointer and
   associated name to enable the parallel job system to identify and execute the job appropriately. It is designed to integrate with the engine's existing parallel processing infrastructure, allowing
   for efficient distribution of work across multiple threads. The registration process ensures that jobs are properly set up for concurrent execution while maintaining debugging and tracking
   capabilities through the provided name identifier.

*/
class idParallelJobRegistration
{
public:
	/*!
		\brief Constructs a parallel job registration object and registers the specified job function with the given name.

		This constructor initializes a parallel job registration by storing the provided job function and name. It internally calls the RegisterJob method to register the job with the parallel job
	   system. The job function will be executed in a parallel context, and the name is used for identification and debugging purposes.

		\param function The function pointer to the job that will be executed in parallel.
		\param name A string identifier for the job, used for debugging and tracking.
	*/
	idParallelJobRegistration( jobRun_t function, const char* name );
};

#define REGISTER_PARALLEL_JOB( function, name ) static idParallelJobRegistration register_##function( ( jobRun_t )function, name )

#endif // !__PARALLELJOBLIST_H__
