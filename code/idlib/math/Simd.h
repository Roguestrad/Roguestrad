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

#ifndef __MATH_SIMD_H__
#define __MATH_SIMD_H__

/*!
	\class idSIMD
	\brief Provides SIMD (Single Instruction, Multiple Data) processing capabilities with dynamic implementation selection based on CPU capabilities.

	The idSIMD class serves as the core interface for managing SIMD processing within the engine, automatically selecting the most appropriate implementation based on available CPU extensions. It
   handles initialization, shutdown, and performance testing of different SIMD implementations, including generic fallback options. The class initializes the SIMD processor by detecting CPU
   capabilities such as MMX and SSE support, and can be forced to use generic implementations when needed. During initialization, it also manages FPU modes like Flush-To-Zero and Denormals-Are-Zero
   for optimal performance on supporting hardware. The class supports performance testing through the Test_f method, which evaluates different SIMD implementations and selects the most efficient one
   based on benchmark results. Proper cleanup is handled during shutdown by deallocating all allocated resources and resetting pointers to prevent dangling references. The system maintains a singleton
   pattern where initialization is performed once based on module context and CPU capabilities.

*/
class idSIMD
{
public:
	/*!
		\brief Initializes the SIMD processor based on CPU capabilities and module context

		This function sets up the SIMD processor implementation by creating a generic SIMD handler and initializing the processor pointer. It does not perform any CPU capability detection or selection
	   of optimized implementations, as that is handled elsewhere in the system.

	*/
	static void Init();

	/*!
		\brief Initializes the SIMD processor based on CPU capabilities and module context

		This function determines the appropriate SIMD processor implementation to use based on the CPU's capabilities and the specified module. It first retrieves the CPUID information to check for
	   available processor extensions such as MMX and SSE. If forced to use generic implementation or if no specific processor is available, it defaults to the generic implementation. The function
	   also handles enabling Flush-To-Zero and Denormals-Are-Zero FPU modes when supported by the CPU. It prints diagnostic information about the chosen processor and enabled FPU modes.

		\param module Name of the module calling this initialization
		\param forceGeneric Flag to force usage of generic implementation regardless of CPU capabilities
	*/
	static void InitProcessor( const char* module, bool forceGeneric );

	/*!
		\brief Shuts down the SIMD processing system by deallocating processor and generic objects.

		This function cleans up the SIMD (Single Instruction, Multiple Data) processing system by deallocating the processor and generic objects that were previously allocated. It checks if the
	   current processor is not the generic one and deletes it if necessary. Then it deletes the generic object and sets all related pointers to NULL to prevent dangling references. This function is
	   typically called during system shutdown to ensure proper cleanup of SIMD resources.

	*/
	static void Shutdown();

	/*!
		\brief Executes SIMD performance tests and selects the appropriate SIMD implementation based on CPU capabilities.

		This function performs a series of performance tests to evaluate different SIMD implementations available on the system. It first sets the thread priority to time-critical on Windows platforms
	   to ensure accurate timing measurements. The function then determines which SIMD implementation to use based on command-line arguments, checking CPU capabilities such as MMX and SSE support.
	   After selecting the appropriate implementation, it runs various tests including math operations, min/max comparisons, memory copy and memset operations, and joint transformation operations. The
	   results of these tests are printed to the console. Finally, it cleans up by deleting the SIMD implementation if it's different from the default processor implementation and resets the thread
	   priority on Windows platforms.

		\param args Command line arguments that may specify which SIMD implementation to test
	*/
	static void Test_f( const class idCmdArgs& args );
};

/*
===============================================================================

	virtual base class for different SIMD processors

===============================================================================
*/

// RB begin
#ifdef _WIN32
	#define VPCALL __fastcall
#else
	#define VPCALL
#endif
// RB end

class idVec2;
class idVec3;
class idVec4;
class idVec5;
class idVec6;
class idVecX;
class idMat2;
class idMat3;
class idMat4;
class idMat5;
class idMat6;
class idMatX;
class idPlane;
class idDrawVert;
class idJointQuat;
class idJointMat;
struct dominantTri_t;

/*!
	\class idSIMDProcessor
	\brief Abstract base class for SIMD processor implementations providing optimized mathematical operations.

	The idSIMDProcessor class serves as an abstract base class for implementing SIMD-optimized mathematical operations used throughout the engine. It defines a common interface for various
   processor-specific optimizations, including min/max calculations, memory operations, and joint transformation functions. The class is designed to be inherited by concrete implementations that
   provide optimized versions of these operations based on detected CPU capabilities. The virtual destructor ensures proper cleanup of derived implementations, while the pure virtual methods enforce
   that each implementation provides concrete functionality for all required operations. This design enables the engine to select the most appropriate optimized implementation at runtime based on
   available CPU instruction sets.

*/
class idSIMDProcessor
{
public:
	/*!
		\brief Initializes the idSIMDProcessor object with default CPUID_NONE value.

		The constructor sets the cpuid member variable to CPUID_NONE, indicating that no specific CPU instruction set has been detected or enabled.

	*/
	idSIMDProcessor() { cpuid = CPUID_NONE; }

	cpuid_t cpuid;

	virtual ~idSIMDProcessor() { } // SRS - Added virtual destructor

	virtual const char* VPCALL GetName() const = 0;

	virtual void VPCALL		   MinMax( float& min, float& max, const float* src, const int count )									 = 0;
	virtual void VPCALL		   MinMax( idVec2& min, idVec2& max, const idVec2* src, const int count )								 = 0;
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idVec3* src, const int count )								 = 0;
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const int count )							 = 0;
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const triIndex_t* indexes, const int count ) = 0;

	virtual void VPCALL		   Memcpy( void* dst, const void* src, const int count ) = 0;
	virtual void VPCALL		   Memset( void* dst, const int val, const int count )	 = 0;

	// animation
	virtual void VPCALL		   BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints )	   = 0;
	virtual void VPCALL		   BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints ) = 0;
	virtual void VPCALL		   ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints )					   = 0;
	virtual void VPCALL		   ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints )					   = 0;
	virtual void VPCALL		   TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint )						   = 0;
	virtual void VPCALL		   UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint )					   = 0;
};

// pointer to SIMD processor
extern idSIMDProcessor* SIMDProcessor;

#endif /* !__MATH_SIMD_H__ */
