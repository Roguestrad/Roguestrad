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

/*!
	\class idLCP
	\brief A base class for Linear Complementarity Problem solvers with support for bounded and unbounded variables.

	The idLCP class serves as an abstract base interface for solving Linear Complementarity Problems, which are mathematical optimization problems that arise in physics simulation and other domains.
   The class supports both bounded and unbounded variables through the boxIndex parameter, where variables with boxIndex[i] != -1 are treated as bounded and variables with boxIndex[i] == -1 are
   treated as unbounded. The solver first calculates all unbounded variables and those with boxIndex[i] == -1 before processing bounded variables. The interface provides methods for allocating
   different types of LCP solvers, configuring maximum iterations, and executing test functions for debugging purposes. The Solve method is pure virtual, requiring implementations to define the
   specific algorithm for solving the LCP. The class is designed to be extended by concrete implementations that provide the actual numerical solution algorithms.

*/
class idLCP
{
public:
	//! Allocates and returns a new square LCP solver instance.
	static idLCP* AllocSquare();

	//! Allocates and returns a new symmetric LCP solver instance.
	static idLCP* AllocSymmetric();

	virtual ~idLCP();

	virtual bool Solve( const idMatX& A, idVecX& x, const idVecX& b, const idVecX& lo, const idVecX& hi, const int* boxIndex = NULL ) = 0;

	//! Sets the maximum number of iterations for the LCP solver.
	virtual void SetMaxIterations( int max );

	//! Returns the maximum number of iterations allowed for the LCP solver.
	virtual int	 GetMaxIterations();

	//! Executes LCP test functions if ENABLE_TEST_CODE is defined.
	static void	 Test_f( const idCmdArgs& args );

protected:
	int maxIterations;
};

#endif // !__MATH_LCP_H__
