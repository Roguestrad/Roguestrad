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

	/*!
		\brief Computes the minimum and maximum values from an array of floats.

		The function calculates the minimum and maximum values from a given array of floating-point numbers. It initializes the minimum value to positive infinity and the maximum value to negative
	   infinity before iterating through the array. The function uses a macro-based unrolling technique to process the array elements efficiently.

		\param min Reference to store the computed minimum value
		\param max Reference to store the computed maximum value
		\param src Pointer to the array of floating-point numbers
		\param count Number of elements in the source array
		\return void VPCALL
	*/
	virtual void VPCALL		   MinMax( float& min, float& max, const float* src, const int count );

	/*!
		\brief Computes the minimum and maximum values of a set of 2D vectors.

		This function calculates the bounding box of a set of 2D vectors by determining the minimum and maximum values for each component (x and y) across all input vectors. The input vectors are
	   processed in a loop where each vector's components are compared against the current minimum and maximum values, updating them as needed. The function uses a macro to unroll the loop for better
	   performance.

		\param min Reference to the idVec2 that will store the minimum values
		\param max Reference to the idVec2 that will store the maximum values
		\param src Pointer to the array of idVec2 vectors to process
		\param count Number of vectors in the src array
	*/
	virtual void VPCALL		   MinMax( idVec2& min, idVec2& max, const idVec2* src, const int count );

	/*!
		\brief Computes the minimum and maximum values of a set of 3D vectors.

		This function calculates the bounding box of a set of 3D vectors by determining the minimum and maximum coordinates along each axis. It initializes the min and max values to positive and
	   negative infinity respectively, then iterates through the input array of vectors to update these values. The function processes all vectors in the input array and updates the provided min and
	   max vectors with the computed bounding box coordinates.

		\param min Reference to the vector that will store the minimum coordinates
		\param max Reference to the vector that will store the maximum coordinates
		\param src Pointer to the array of 3D vectors to process
		\param count Number of vectors in the source array
	*/
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idVec3* src, const int count );

	/*!
		\brief Computes the minimum and maximum coordinates from a set of vertices.

		The function calculates the bounding box coordinates by finding the minimum and maximum values for each component (x, y, z) across all provided vertices. It initializes the minimum values to
	   positive infinity and maximum values to negative infinity, then iterates through the vertex data to update these values. The results are stored in the provided min and max idVec3 objects.

		\param min Output parameter that will contain the minimum coordinates after processing
		\param max Output parameter that will contain the maximum coordinates after processing
		\param src Pointer to the array of draw vertices to process
		\param count Number of vertices in the source array
	*/
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const int count );

	/*!
		\brief Computes the minimum and maximum coordinates of a set of vertices.

		This function calculates the bounding box coordinates for a set of vertices indexed by the provided triangle indices. It initializes the minimum and maximum values to positive and negative
	   infinity respectively, then iterates through each indexed vertex to update these bounds. The function processes three-dimensional coordinates and updates the min and max vectors accordingly.

		\param min Output vector to store the minimum coordinates
		\param max Output vector to store the maximum coordinates
		\param src Pointer to the array of draw vertices
		\param indexes Pointer to the array of triangle indices
		\param count Number of triangles to process
	*/
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const triIndex_t* indexes, const int count );

	//! Copies count bytes from src to dst.
	virtual void VPCALL		   Memcpy( void* dst, const void* src, const int count );

	//! Sets a block of memory to a specific value.
	virtual void VPCALL		   Memset( void* dst, const int val, const int count );

	//! Performs spherical linear interpolation and vector linear interpolation on joints for blending animations.
	virtual void VPCALL		   BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	/*!
		\brief Blends joints using linear interpolation with the specified lerp factor

		This function performs a fast joint blending operation by interpolating the quaternion and translation components of joints. It uses the provided index array to determine which joints to
	   process and applies linear interpolation between the current joint values and the corresponding blend joint values. The w component of each joint is set to zero after blending.

		\param joints Output array of joints that will be modified with blended values
		\param blendJoints Input array of joints containing the target values for blending
		\param lerp Interpolation factor between 0 and 1, where 0 means no change and 1 means full blend
		\param index Array of joint indices specifying which joints to process
		\param numJoints Number of joints to process
	*/
	virtual void VPCALL		   BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	//! Converts an array of joint quaternions to joint matrices.
	virtual void VPCALL		   ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints );

	//! Converts an array of joint matrices to joint quaternions.
	virtual void VPCALL		   ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints );

	/*!
		\brief Transforms joint matrices by applying parent transformations to child joints

		This function processes a range of joint matrices, applying the transformation of each joint's parent to the joint itself. The operation modifies the joint matrices in-place, updating each
	   joint's transformation matrix to reflect its position in the joint hierarchy. The function assumes that the parent joint index for each joint is less than the joint index, ensuring a correct
	   processing order from root to leaf joints.

		\param jointMats Pointer to an array of joint matrices to be transformed
		\param parents Pointer to an array of parent indices for each joint
		\param firstJoint Index of the first joint to process
		\param lastJoint Index of the last joint to process
		\throws assertion failure if a parent index is greater than or equal to the joint index
	*/
	virtual void VPCALL		   TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );

	/*!
		\brief Computes inverse transformations for joints in a skeleton hierarchy by processing joints from last to first.

		This function performs backward traversal of the joint hierarchy to compute inverse transformations. It operates on a range of joints specified by firstJoint and lastJoint indices. For each
	   joint, it asserts that the parent index is valid (less than current joint index) and then divides the joint matrix by its parent's matrix to compute the inverse transformation. The operation
	   modifies the jointMats array in-place, with the results stored in the same location.

		\param jointMats Array of joint matrices to be updated with inverse transformations
		\param parents Array containing parent joint indices for each joint
		\param firstJoint Index of the first joint to process
		\param lastJoint Index of the last joint to process
		\throws assertion failure if a parent index is invalid (greater than or equal to the current joint index)
	*/
	virtual void VPCALL		   UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
};

#endif /* !__MATH_SIMD_GENERIC_H__ */
