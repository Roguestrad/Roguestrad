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
	\brief A class representing an ordinary differential equation solver for numerical integration.
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
	\brief Euler integration solver for ordinary differential equations.
*/
class idODE_Euler : public idODE
{
public:
	//! Initializes a new instance of the idODE_Euler class with the specified dimension, derivative function, and user data.
	idODE_Euler( const int dim, const deriveFunction_t dr, const void* ud );

	//! Destructor for the idODE_Euler class that cleans up allocated memory for derivatives.
	virtual ~idODE_Euler();

	/*!
		\brief Evaluates Euler integration for the given state and time step by computing new state values based on derivatives and time delta.

		This function performs Euler integration to update the state of a system. It calculates the time step delta between t0 and t1, computes the derivatives of the system at time t0 using the
	   provided user data, and then updates each state component by adding the product of the time delta and the corresponding derivative to the initial state value.

		\param state Current state values of the system
		\param newState Output array containing the updated state values
		\param t0 Initial time value
		\param t1 Final time value
		\return The time step delta between t1 and t0.
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

protected:
	float* derivatives; // space to store derivatives
};

/*!
	\class idODE_Midpoint
	\brief Midpoint ODE solver implementation for numerical integration.
*/
class idODE_Midpoint : public idODE
{
public:
	//! Constructs an idODE_Midpoint object with the specified dimension, derivative function, and user data.
	idODE_Midpoint( const int dim, const deriveFunction_t dr, const void* ud );

	//! Destroys the idODE_Midpoint object and cleans up allocated memory.
	virtual ~idODE_Midpoint();

	/*!
		\brief Evaluates the midpoint ordinary differential equation integration step using the provided state and time values

		This function performs a midpoint integration step for solving ordinary differential equations. It takes the current state and time values, computes intermediate derivatives, and updates the
	   newState array with the integrated values. The method uses a two-stage approach where the first stage computes an intermediate state halfway through the time interval, and the second stage uses
	   this intermediate state to compute the final result. The function returns the time interval delta which was used in the computation.

		\param state Current state values for the ODE system
		\param newState Output array containing the integrated state values
		\param t0 Initial time value
		\param t1 Final time value
		\return The time interval delta (t1 - t0) used in the integration process
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

protected:
	float* tmpState;
	float* derivatives; // space to store derivatives
};

/*!
	\class idODE_RK4
	\brief A class implementing the 4th order Runge-Kutta numerical integration method for solving ordinary differential equations.
*/
class idODE_RK4 : public idODE
{
public:
	//! Initializes a new instance of the idODE_RK4 class with the specified dimension, derivative function, and user data.
	idODE_RK4( const int dim, const deriveFunction_t dr, const void* ud );

	//! Destructor for the idODE_RK4 class that cleans up dynamically allocated memory for state and derivative arrays.
	virtual ~idODE_RK4();

	/*!
		\brief Evaluates the Runge-Kutta 4th order solution for the given state and time interval

		This function performs a fourth-order Runge-Kutta numerical integration step to advance a system state from time t0 to t1. It uses the derivative function to compute intermediate values and
	   combines them to produce a more accurate solution than simpler methods. The function stores the result in the newState array and returns the time step size

		\param state current state of the system
		\param newState output array containing the state at the new time
		\param t0 initial time
		\param t1 final time
		\return the time step size (t1 - t0) used in the integration
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
	\brief Adaptive Runge-Kutta 4th order solver for ordinary differential equations.
*/
class idODE_RK4Adaptive : public idODE
{
public:
	//! Initializes a new instance of the idODE_RK4Adaptive class with the specified dimension, derivative function, and user data.
	idODE_RK4Adaptive( const int dim, const deriveFunction_t dr, const void* ud );

	//! Destructor for the idODE_RK4Adaptive class that releases all dynamically allocated memory.
	virtual ~idODE_RK4Adaptive();

	/*!
		\brief Evaluates the adaptive Runge-Kutta 4th order differential equation solver and returns the next time step size.

		This function implements an adaptive Runge-Kutta 4th order method for solving ordinary differential equations. It takes the current state vector and computes a new state vector using the RK4
	   method with adaptive step sizing. The function performs multiple stages of computation to estimate the local truncation error and adjusts the time step accordingly. It returns the next time
	   step size based on the estimated error and the maximum allowed error tolerance.

		\param state The current state vector of the differential equation system
		\param newState The computed next state vector after integration
		\param t0 The starting time of the integration step
		\param t1 The ending time of the integration step
		\return The next time step size for the adaptive integration method
	*/
	virtual float Evaluate( const float* state, float* newState, float t0, float t1 );

	//! Sets the maximum error tolerance for the adaptive Runge-Kutta 4 solver.
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
