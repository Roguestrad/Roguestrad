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

#ifndef __MATH_ODE_H__
#define __MATH_ODE_H__

/*
===============================================================================

	Numerical solvers for ordinary differential equations.

===============================================================================
*/

//===============================================================
//
//	idODE
//
//===============================================================

typedef void ( *deriveFunction_t )( const float t, const void* userData, const float* state, float* derivatives );

/*!
	\class idODE
	\brief A base class for defining ordinary differential equations used in physics simulation.

	The idODE class serves as an abstract interface for implementing ordinary differential equation solvers within the engine's physics system. It defines the core contract for evaluating differential
   equations at specific time steps, enabling various numerical integration methods to be applied to physical simulations. The virtual destructor ensures proper cleanup of derived implementations,
   while the evaluate method provides the fundamental mechanism for advancing simulation states. This interface is designed to support flexible physics modeling where different types of differential
   equations can be plugged into the same numerical solver framework.

*/
class idODE
{
public:
	virtual ~idODE() { }

	virtual float Evaluate( const float* state, float* newState, float t0, float t1 ) = 0;

protected:
	int				 dimension; // dimension in floats allocated for
	deriveFunction_t derive;	// derive function
	const void*		 userData;	// client data
};

/*!
	\class idODE_Euler
	\brief Euler integration ODE solver for numerical simulation.

	The idODE_Euler class provides a concrete implementation of the idODE interface using the Euler numerical integration method. It is designed to solve systems of ordinary differential equations by
   advancing the system state through discrete time steps. The class is initialized with the dimension of the ODE system, a derivative function pointer, and optional user data for custom calculations.
   During each evaluation step, it computes the derivatives at the current state and updates the system state using the simple forward Euler method. This approach is suitable for cases where
   computational simplicity outweighs accuracy requirements, and it provides a basic foundation for more complex integration schemes that may build upon this solver.

*/
class idODE_Euler : public idODE
{
public:
	/*!
		\brief Constructs an idODE_Euler object with the specified dimension, derivative function, and user data.

		The constructor initializes the ODE solver with the given dimension of the system, the derivative function to be used for solving the equations, and optional user data that can be passed to
	   the derivative function. It allocates memory for the derivatives array based on the specified dimension.

		\param dim The dimension of the system of ordinary differential equations
		\param dr Pointer to the derivative function that computes the derivatives
		\param ud Pointer to user data that will be passed to the derivative function
	*/
	idODE_Euler( const int dim, const deriveFunction_t dr, const void* ud );

	/*!
		\brief Destructor for the idODE_Euler class that cleans up the derivatives memory.

		This destructor is responsible for releasing the memory allocated for the derivatives array. It is called when an instance of the idODE_Euler class is being destroyed, ensuring proper cleanup
	   of dynamically allocated resources.

	*/
	virtual ~idODE_Euler();

	/*!
		\brief Evaluates the Euler integration step for the given state and time interval.

		This function performs a single Euler integration step to advance the system state from time t0 to time t1. It computes the derivatives of the current state at time t0 using the derived
	   function, then updates the state by advancing it along the derivative direction with a step size equal to the time difference. The function returns the time interval used for the integration
	   step.

		\param state Current system state vector
		\param newState Output vector containing the integrated state
		\param t0 Starting time of the integration step
		\param t1 Ending time of the integration step
		\return The time interval (t1 - t0) used for the Euler integration step
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

protected:
	float* derivatives; // space to store derivatives
};

/*!
	\class idODE_Midpoint
	\brief A midpoint ordinary differential equation solver implementation.

	The idODE_Midpoint class provides a numerical integration implementation for solving ordinary differential equations using the midpoint method. This class inherits from idODE and is designed to
   handle systems of differential equations with a specified dimensionality. The solver requires a derivative function and optional user data to compute the integration steps. The implementation
   manages temporary memory for state and derivative calculations during the integration process. The Evaluate method performs the actual midpoint integration step, taking the current state and
   computing the new state based on the midpoint method for numerical stability. The class is intended for use in physics simulations and other computational modeling scenarios where accurate ODE
   integration is required. Memory management is handled internally by the class, with cleanup performed in the destructor.

*/
class idODE_Midpoint : public idODE
{
public:
	/*!
		\brief Constructs an idODE_Midpoint object with the specified dimension, derivative function, and user data.

		The constructor initializes the ODE solver with the given dimensionality of the system, the derivative function to be used for calculations, and optional user data that can be passed to the
	   derivative function. It allocates memory for temporary state and derivatives arrays based on the specified dimension.

		\param dim The dimensionality of the ODE system
		\param dr Pointer to the derivative function used for solving the ODE
		\param ud Pointer to user-defined data that will be passed to the derivative function
	*/
	idODE_Midpoint( const int dim, const deriveFunction_t dr, const void* ud );

	/*!
		\brief Destructor for the idODE_Midpoint class that cleans up allocated memory.

		This destructor cleans up the dynamically allocated memory for temporary state and derivatives arrays that were used during the midpoint ordinary differential equation integration process.

	*/
	virtual ~idODE_Midpoint();

	/*!
		\brief Evaluates the midpoint ordinary differential equation integration step and returns the time step size.

		This function performs a midpoint integration step for solving ordinary differential equations. It takes the current state, computes intermediate derivatives, and calculates the new state
	   using the midpoint method. The function returns the time step size used in the integration.

		\param state Current state vector
		\param newState Output vector containing the new state after integration
		\param t0 Initial time
		\param t1 Final time
		\return The time step size used in the integration
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

protected:
	float* tmpState;
	float* derivatives; // space to store derivatives
};

/*!
	\class idODE_RK4
	\brief Runge-Kutta 4th order ordinary differential equation solver implementation.

	The idODE_RK4 class provides a concrete implementation of an ordinary differential equation solver using the fourth-order Runge-Kutta numerical integration method. This solver is designed to
   advance the state of a system from one time point to another by computing intermediate derivative evaluations at four different points within the integration interval. The class requires the
   dimensionality of the ODE system, a derivative function pointer, and user data to be provided during construction. During evaluation, it takes the current state vector and computes a new state
   vector at the target time, returning the time step delta used in the integration process. The solver manages temporary memory for state and derivative vectors required during the integration
   computations. This implementation is suitable for simulating physical systems or other dynamic behaviors where fourth-order accuracy is desired.

*/
class idODE_RK4 : public idODE
{
public:
	/*!
		\brief Initializes a new instance of the idODE_RK4 class with the specified dimension, derivative function, and user data.

		This constructor sets up the RK4 ordinary differential equation solver with the given dimensionality, derivative function, and user data. It allocates memory for temporary state vectors and
	   derivative vectors needed for the Runge-Kutta 4th order numerical integration method.

		\param dim The dimension of the ODE system
		\param dr Pointer to the derivative function used to compute derivatives
		\param ud Pointer to user data that will be passed to the derivative function
	*/
	idODE_RK4( const int dim, const deriveFunction_t dr, const void* ud );

	/*!
		\brief Destructor for the idODE_RK4 class that releases all allocated memory.

		Releases all dynamically allocated memory for temporary state and derivative arrays used during RK4 ODE integration.

	*/
	virtual ~idODE_RK4();

	/*!
		\brief Evaluates a Runge-Kutta 4th order ordinary differential equation integration step from t0 to t1.

		This function performs a single integration step using the fourth-order Runge-Kutta method to advance the state of a system from time t0 to t1. It computes four derivative evaluations at
	   different points in the integration interval and combines them to produce a new state vector. The function returns the time step delta which is the difference between t1 and t0.

		\param state Current state vector of the system
		\param newState Output vector containing the integrated state at time t1
		\param t0 Starting time of the integration step
		\param t1 Ending time of the integration step
		\return The time step delta (t1 - t0) used for the integration step
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

protected:
	float* tmpState;
	float* d1; // derivatives
	float* d2;
	float* d3;
	float* d4;
};

/*!
	\class idODE_RK4Adaptive
	\brief Adaptive Runge-Kutta 4th order ordinary differential equation solver with dynamic step size adjustment.

	The idODE_RK4Adaptive class implements a numerical integration method for solving systems of ordinary differential equations using the adaptive fourth-order Runge-Kutta technique. It is designed
   to automatically adjust the step size during integration to maintain a specified error tolerance, making it suitable for solving ODE systems where high accuracy is required. The solver operates on
   systems of a fixed dimension and requires a user-provided derivative function to compute state transitions. The class manages temporary memory for intermediate calculations during the integration
   process, with proper cleanup in its destructor. The adaptive nature of the solver allows it to dynamically increase or decrease step size based on local error estimates, improving both efficiency
   and accuracy of the numerical solution.

*/
class idODE_RK4Adaptive : public idODE
{
public:
	/*!
		\brief Initializes a new instance of the idODE_RK4Adaptive class with the specified dimension, derivative function, and user data.

		This constructor sets up the adaptive Runge-Kutta 4th order ordinary differential equation solver with the given parameters. It allocates memory for temporary state arrays used during the
	   numerical integration process. The solver uses the provided derivative function to compute the next state values and maintains the specified dimension for the ODE system being solved.

		\param dim The dimension of the ODE system
		\param dr Pointer to the derivative function that computes the rate of change
		\param ud User data pointer that will be passed to the derivative function
	*/
	idODE_RK4Adaptive( const int dim, const deriveFunction_t dr, const void* ud );

	/*!
		\brief Destructor for the idODE_RK4Adaptive class that cleans up dynamically allocated memory.

		This destructor is responsible for properly releasing all dynamically allocated memory blocks that were used during the adaptive Runge-Kutta 4th order numerical integration process. It ensures
	   that no memory leaks occur by deleting all temporary state arrays and derivative arrays that were allocated during the object's lifetime.

	*/
	virtual ~idODE_RK4Adaptive();

	/*!
		\brief Evaluates an adaptive Runge-Kutta 4th order solution for the given state and time interval

		This function implements a fourth-order Runge-Kutta adaptive numerical integration method for solving ordinary differential equations. It computes a new state vector based on the current state
	   and time interval, using an adaptive step size selection to maintain accuracy within specified error tolerances. The method performs multiple sub-steps to estimate the solution and its error,
	   adjusting the step size based on the estimated error relative to the maximum allowed error. The function returns an adaptive step size, which may be larger or smaller than the input time
	   interval depending on the error estimate.

		\param state Current state vector for the ODE system
		\param newState Output vector containing the new state after integration
		\param t0 Initial time
		\param t1 Final time
		\return Adaptive step size for the next integration step
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

	/*!
		\brief Sets the maximum error tolerance for the adaptive Runge-Kutta 4th order ODE solver to the specified positive value.

		This function configures the maximum allowable error for the adaptive Runge-Kutta 4th order ordinary differential equation solver. The error tolerance is used to control the step size
	   adaptation during numerical integration. The function only accepts positive error values, ignoring any non-positive inputs.

		\param err The maximum error tolerance to be set for the solver
	*/
	void		  SetMaxError( const float err );

protected:
	float  maxError; // maximum allowed error
	float* tmpState;
	float* d1; // derivatives
	float* d1half;
	float* d2;
	float* d3;
	float* d4;
};

#endif /* !__MATH_ODE_H__ */
