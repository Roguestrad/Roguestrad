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

#ifndef __MATH_SIMD_GENERIC_H__
#define __MATH_SIMD_GENERIC_H__

/*!
	\class idSIMD_Generic
	\brief Generic SIMD implementation providing basic computational methods for various data types.

	This class serves as a fallback SIMD implementation that provides basic computational methods for processing various data types including floats, vectors, and joint transformations. It inherits
   from idSIMDProcessor and implements methods for finding minimum and maximum values in arrays, memory operations like copy and set, and joint animation blending techniques. The implementation is
   designed to work with generic data structures without relying on specific hardware optimizations. The class is intended to provide consistent behavior across different platforms while maintaining
   simplicity and reliability in its operations.

*/
class idSIMD_Generic : public idSIMDProcessor
{
public:
	//! Returns the name of the SIMD generic implementation.
	virtual const char* VPCALL GetName() const;

	//! Computes the minimum and maximum values from an array of floats.
	virtual void VPCALL		   MinMax( float& min, float& max, const float* src, const int count );

	//! Computes the minimum and maximum values of a set of 2D vectors.
	virtual void VPCALL		   MinMax( idVec2& min, idVec2& max, const idVec2* src, const int count );

	//! Computes the minimum and maximum values of a set of 3D vectors.
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idVec3* src, const int count );

	//! Computes the minimum and maximum coordinates from a set of vertices.
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const int count );

	//! Computes the minimum and maximum coordinates of a set of vertices.
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const triIndex_t* indexes, const int count );

	//! Copies count bytes from src to dst.
	virtual void VPCALL		   Memcpy( void* dst, const void* src, const int count );

	//! Sets a block of memory to a specific value.
	virtual void VPCALL		   Memset( void* dst, const int val, const int count );

	//! Performs spherical linear interpolation and vector linear interpolation on joints for blending animations.
	virtual void VPCALL		   BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	//! Blends joints using linear interpolation with the specified lerp factor
	virtual void VPCALL		   BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	//! Converts an array of joint quaternions to joint matrices.
	virtual void VPCALL		   ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints );

	//! Converts an array of joint matrices to joint quaternions.
	virtual void VPCALL		   ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints );

	//! Transforms joint matrices by applying parent transformations to child joints.
	virtual void VPCALL		   TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );

	//! Computes inverse transformations for joints in a skeleton hierarchy
	virtual void VPCALL		   UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
};

#endif /* !__MATH_SIMD_GENERIC_H__ */
