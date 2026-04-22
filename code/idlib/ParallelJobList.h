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
	\brief Manages a list of parallel jobs that can be executed with synchronization points.

	Provides a mechanism for organizing and executing parallel jobs with support for synchronization points and detailed timing information. Jobs can be added individually, and the
   list can be submitted for execution with optional waiting behavior. The class tracks execution metrics including processing time, wasted time, and synchronization points. It supports configuration
   of job list properties such as maximum jobs, maximum synchronizations, and priority. The class is designed to balance work load across multiple processing units while minimizing overhead from job
   switching. Memory management is handled through the constructor and destructor, with no explicit ownership semantics described for the job data.

*/
class idParallelJobList
{
	friend class idParallelJobManagerLocal;

public:
	//! Adds a job to the parallel job list for execution.
	void		   AddJob( jobRun_t function, void* data );

	//! Inserts a synchronization point into the job list with the specified sync type.
	void		   InsertSyncPoint( jobSyncType_t syncType );

	//! Submits the jobs in this list for execution.
	void		   Submit( idParallelJobList* waitForJobList = NULL, int parallelism = JOBLIST_PARALLELISM_DEFAULT );

	//! Waits for all jobs in the parallel job list to complete.
	void		   Wait();

	//! Attempts to wait for all jobs in the list to finish and returns immediately with a boolean indicating if all jobs are done.
	bool		   TryWait();

	//! Returns true if the job list has been submitted.
	bool		   IsSubmitted() const;

	//! Retrieves the number of jobs that have been executed in this job list.
	unsigned int   GetNumExecutedJobs() const;

	//! Returns the number of synchronization points in the job list.
	unsigned int   GetNumSyncs() const;

	//! Returns the time at which the job list was submitted in microseconds.
	uint64		   GetSubmitTimeMicroSec() const;

	//! Returns the time at which execution of this job list started in microseconds.
	uint64		   GetStartTimeMicroSec() const;

	//! Returns the time at which all jobs in the list were executed.
	uint64		   GetFinishTimeMicroSec() const;

	//! Returns the time the host thread waited for the job list to finish in microseconds.
	uint64		   GetWaitTimeMicroSec() const;

	//! Returns the total processing time in microseconds for all threads in the job list.
	uint64		   GetTotalProcessingTimeMicroSec() const;

	//! Returns the total time wasted by all threads while processing the job list.
	uint64		   GetTotalWastedTimeMicroSec() const;

	//! Returns the processing time in microseconds for the specified unit.
	uint64		   GetUnitProcessingTimeMicroSec( int unit ) const;

	//! Returns the amount of time a specified unit wasted while processing this job list.
	uint64		   GetUnitWastedTimeMicroSec( int unit ) const;

	//! Retrieves the ID of the job list.
	jobListId_t	   GetId() const;

	//! Returns the color used for profiling in the parallel job list.
	const idColor* GetColor() const { return this->color; }

private:
	class idParallelJobList_Threads* jobListThreads;
	const idColor*					 color;

	/*!
		\brief Constructs a parallel job list with the specified parameters.

		The constructor initializes a parallel job list with the provided identifier, priority, maximum number of jobs, and maximum number of synchronizations. It also assigns the provided color to
	   the job list. The priority must be greater than JOBLIST_PRIORITY_NONE, as enforced by an assertion. The actual job list implementation is delegated to an internal idParallelJobList_Threads
	   object.

		\param id Unique identifier for the job list
		\param priority Priority level for the job list, must be greater than JOBLIST_PRIORITY_NONE
		\param maxJobs Maximum number of jobs that can be processed in parallel
		\param maxSyncs Maximum number of synchronization points allowed
		\param color Color associated with the job list for visualization or identification
		\throws Throws an assertion error if the priority is not greater than JOBLIST_PRIORITY_NONE.
	*/
	idParallelJobList( jobListId_t id, jobListPriority_t priority, unsigned int maxJobs, unsigned int maxSyncs, const idColor* color );

	//! Destroys the idParallelJobList and cleans up its associated threads.
	~idParallelJobList();
};

/*!
	\class idParallelJobManager
	\brief Manages parallel job execution through a job list interface.

	This class provides a unified interface for managing parallel job execution threads. It handles the allocation and deallocation of job lists which contain work units to be processed in parallel.
   The manager oversees the scheduling and execution of these jobs across multiple processing units. Job lists can be configured with specific priorities and resource limits. The interface supports
   tracking the number of active job lists and provides synchronization mechanisms to ensure proper execution order. The manager is responsible for initializing and shutting down the parallel
   processing infrastructure, and can wait for all job lists to complete their execution.

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

//! Registers a job function with an associated debug name.
void						 RegisterJob( jobRun_t function, const char* name );

/*!
	\class idParallelJobRegistration
	\brief Manages registration of parallel jobs for concurrent execution.
*/
class idParallelJobRegistration
{
public:
	//! Constructs an idParallelJobRegistration object and registers a job with the specified function and name.
	idParallelJobRegistration( jobRun_t function, const char* name );
};

#define REGISTER_PARALLEL_JOB( function, name ) static idParallelJobRegistration register_##function( ( jobRun_t )function, name )

#endif // !__PARALLELJOBLIST_H__
