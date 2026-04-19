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

/*
===============================================================================

	Generic implementation of idSIMDProcessor

===============================================================================
*/

class idSIMD_Generic : public idSIMDProcessor
{
public:
	//! Returns the name of the SIMD generic code implementation.
	virtual const char* VPCALL GetName() const;

	//! Computes the minimum and maximum values from an array of floats.
	virtual void VPCALL		   MinMax( float& min, float& max, const float* src, const int count );

	//! Computes the minimum and maximum values of a set of 2D vectors.
	virtual void VPCALL		   MinMax( idVec2& min, idVec2& max, const idVec2* src, const int count );

	//! Computes the minimum and maximum values of a set of 3D vectors.
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idVec3* src, const int count );

	//! Computes the minimum and maximum coordinates from a list of draw vertices.
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const int count );

	//! Computes the bounding box for a set of triangle vertices.
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const triIndex_t* indexes, const int count );

	//! Copies a specified number of bytes from the source memory location to the destination memory location.
	virtual void VPCALL		   Memcpy( void* dst, const void* src, const int count );

	//! Sets count bytes of memory at dst to the value val.
	virtual void VPCALL		   Memset( void* dst, const int val, const int count );

	//! Blends joints using spherical linear interpolation for quaternions and linear interpolation for translations.
	virtual void VPCALL		   BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	//! Performs a fast joint blending operation on a set of joints using linear interpolation.
	virtual void VPCALL		   BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	//! Converts joint quaternions to joint matrices by setting rotation and translation components for each joint.
	virtual void VPCALL		   ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints );

	//! Converts an array of joint matrices to joint quaternions.
	virtual void VPCALL		   ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints );

	//! Transforms joints by applying parent transformations to child joints.
	virtual void VPCALL		   TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );

	//! This function untransforms joints by dividing each joint matrix by its parent joint matrix in reverse order.
	virtual void VPCALL		   UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
};

#endif /* !__MATH_SIMD_GENERIC_H__ */
