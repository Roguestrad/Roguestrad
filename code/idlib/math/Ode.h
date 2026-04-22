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

	//! Evaluates the Euler integration for the given state and time step.
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

	//! Evaluates the midpoint ODE integration step using the provided state and time values.
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

	//! Evaluates the Runge-Kutta 4th order solution for the given state and time interval
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

	//! Evaluates the adaptive Runge-Kutta 4th order differential equation solver and returns the next time step size.
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
