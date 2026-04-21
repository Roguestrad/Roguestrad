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
#ifndef __MATH_LCP_H__
#define __MATH_LCP_H__

/*
================================================
The *LCP* class, idLCP, is a Box-Constrained Mixed Linear Complementarity Problem solver.

'A' is a matrix of dimension n*n and 'x', 'b', 'lo', 'hi' are vectors of dimension n.

Solve: Ax = b + t, where t is a vector of dimension n, with complementarity condition:

	(x[i] - lo[i]) * (x[i] - hi[i]) * t[i] = 0

such that for each 0 <= i < n one of the following holds:

	lo[i] < x[i] < hi[i], t[i] == 0
	x[i] == lo[i], t[i] >= 0
	x[i] == hi[i], t[i] <= 0

Partly-bounded or unbounded variables can have lo[i] and/or hi[i] set to negative/positive
idMath::INFITITY, respectively.

If boxIndex != NULL and boxIndex[i] != -1, then

	lo[i] = - fabs( lo[i] * x[boxIndex[i]] )
	hi[i] = fabs( hi[i] * x[boxIndex[i]] )
	boxIndex[boxIndex[i]] must be -1

Before calculating any of the bounded x[i] with boxIndex[i] != -1, the solver calculates all
unbounded x[i] and all x[i] with boxIndex[i] == -1.
================================================
*/

/*!
	\class idLCP
	\brief Linear Complementarity Problem solver interface for various LCP formulations.

	The idLCP class serves as an abstract base interface for solving Linear Complementarity Problems within the engine. It provides a unified method for configuring solver parameters such as maximum
   iterations and defines a virtual Solve method that must be implemented by derived classes. The class supports different LCP formulations through factory methods AllocSquare and AllocSymmetric,
   which return specialized solver instances for square and symmetric LCP problems respectively. The interface is designed to be used by physics and simulation systems that require solving LCP
   constraints, with the solver configurations and behavior controlled through the provided configuration methods. The Test_f method allows for debugging and validation of the underlying linear
   algebra operations.

*/
class idLCP
{
public:
	/*!
		\brief Allocates and returns a new square LCP solver instance.

		This function creates a new instance of a square LCP (Linear Complementarity Problem) solver. It initializes the solver with a maximum iteration count of 32 and returns a pointer to the newly
	   created solver instance.

		\return A pointer to the newly allocated idLCP_Square solver instance
	*/
	static idLCP* AllocSquare();

	/*!
		\brief Allocates and returns a new symmetric LCP solver instance.

		This function creates a new instance of a symmetric LCP (Linear Complementarity Problem) solver. It allocates memory for the solver, initializes it with a maximum iteration count of 32, and
	   returns a pointer to the newly created solver object.

		\return A pointer to a newly allocated idLCP object that implements a symmetric LCP solver
	*/
	static idLCP* AllocSymmetric();

	virtual ~idLCP();

	virtual bool Solve( const idMatX& A, idVecX& x, const idVecX& b, const idVecX& lo, const idVecX& hi, const int* boxIndex = NULL ) = 0;

	/*!
		\brief Sets the maximum number of iterations for the LCP solver.

		This function configures the maximum number of iterations that the LCP (Linear Complementarity Problem) solver will perform. It directly assigns the provided value to the internal
	   maxIterations member variable.

		\param max The maximum number of iterations allowed for the LCP solver
	*/
	virtual void SetMaxIterations( int max );

	//! Returns the maximum number of iterations allowed for the LCP solver.
	virtual int	 GetMaxIterations();

	/*!
		\brief Executes various linear compression tests if test code is enabled.

		This function runs a series of tests for linear compression algorithms including dot product calculations, lower triangular solving, and LDLT factorization. The tests are only executed when
	   the ENABLE_TEST_CODE macro is defined.

		\param args Command line arguments containing test configuration
	*/
	static void	 Test_f( const idCmdArgs& args );

protected:
	int maxIterations;
};

#endif // !__MATH_LCP_H__
