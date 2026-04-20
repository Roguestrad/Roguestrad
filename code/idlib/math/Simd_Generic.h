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

	/*!
		\brief Computes the minimum and maximum values from an array of floats.

		This function calculates the smallest and largest floating-point values contained within the provided array. It initializes the minimum value to positive infinity and the maximum value to
	   negative infinity, then iterates through the array to update these values accordingly. The function processes elements using an unrolled loop for performance optimization.

		\param min Reference to a float that will be updated with the minimum value found in the array
		\param max Reference to a float that will be updated with the maximum value found in the array
		\param src Pointer to the first element of the array of floats to process
		\param count Number of elements in the array pointed to by src
		\return void
	*/
	virtual void VPCALL		   MinMax( float& min, float& max, const float* src, const int count );

	/*!
		\brief Computes the minimum and maximum values of a set of 2D vectors

		This function calculates the bounding box for a set of 2D vectors by determining the minimum and maximum values for each component. The function initializes the minimum values to positive
	   infinity and maximum values to negative infinity, then iterates through the input vectors to update these bounds. Each vector component is compared against the current minimum and maximum
	   values and updated accordingly.

		\param min Output parameter that will contain the minimum values for each component
		\param max Output parameter that will contain the maximum values for each component
		\param src Pointer to the array of 2D vectors to process
		\param count Number of 2D vectors in the source array
	*/
	virtual void VPCALL		   MinMax( idVec2& min, idVec2& max, const idVec2* src, const int count );

	/*!
		\brief Computes the minimum and maximum coordinates from a list of 3D vectors.

		This function calculates the bounding box of a set of 3D vectors by determining the minimum and maximum values for each coordinate axis. It initializes the minimum values to positive infinity
	   and maximum values to negative infinity, then iterates through the input vector array to update these values. The function processes all components of each vector in a single loop iteration.

		\param min Reference to the vector that will store the minimum coordinates
		\param max Reference to the vector that will store the maximum coordinates
		\param src Pointer to the array of 3D vectors to process
		\param count Number of 3D vectors in the source array
	*/
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idVec3* src, const int count );

	/*!
		\brief Computes the minimum and maximum coordinates from a list of draw vertices to determine the bounding box.

		This function calculates the bounding box for a set of triangle vertices by finding the minimum and maximum values of the x, y, and z coordinates across all provided vertices. It initializes
	   the minimum values to positive infinity and maximum values to negative infinity, then iterates through each vertex to update these bounds. The function uses a macro to unroll the loop for
	   performance optimization.

		\param min Reference to the minimum coordinates of the bounding box
		\param max Reference to the maximum coordinates of the bounding box
		\param src Pointer to the array of draw vertices
		\param count Number of vertices in the array
	*/
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const int count );

	/*!
		\brief Computes the bounding box for a set of triangle vertices.

		This function calculates the minimum and maximum coordinates of a bounding box that encompasses a set of triangle vertices. It initializes the minimum values to positive infinity and maximum
	   values to negative infinity, then iterates through the specified vertices to update these values. The function processes vertices based on triangle indices provided in the index array.

		\param min Output parameter that will contain the minimum coordinates of the bounding box
		\param max Output parameter that will contain the maximum coordinates of the bounding box
		\param src Pointer to the array of draw vertices
		\param indexes Pointer to the array of triangle indices
		\param count Number of triangles to process
	*/
	virtual void VPCALL		   MinMax( idVec3& min, idVec3& max, const idDrawVert* src, const triIndex_t* indexes, const int count );

	/*!
		\brief Copies a specified number of bytes from the source memory location to the destination memory location.

		This function performs a memory copy operation by copying a given number of bytes from the source address to the destination address. It uses the standard C library memcpy function internally
	   to carry out the actual copying. The function is marked as VPCALL, indicating it may be called through a virtual function table. The count parameter specifies exactly how many bytes to copy,
	   and both source and destination pointers must be valid for the duration of the operation.

		\param dst Pointer to the destination memory location where data will be copied to
		\param src Pointer to the source memory location from where data will be copied from
		\param count Number of bytes to copy from source to destination
	*/
	virtual void VPCALL		   Memcpy( void* dst, const void* src, const int count );

	/*!
		\brief Sets count bytes of memory at dst to the value val.

		This function initializes a block of memory by setting each byte to the specified value. It uses the standard C library memset function to perform the memory assignment. The function is
	   typically used for initializing arrays or structures to a known state, or for clearing memory blocks. The value parameter is passed as an int but is converted to an unsigned char for the memory
	   setting operation.

		\param dst pointer to the destination memory block to be filled
		\param val the value to set each byte to
		\param count the number of bytes to set
	*/
	virtual void VPCALL		   Memset( void* dst, const int val, const int count );

	/*!
		\brief Performs spherical linear interpolation on joint rotations and linear interpolation on joint translations for a set of joints.

		This function blends two sets of joint transformations by interpolating between their quaternions using spherical linear interpolation and their translations using linear interpolation. The
	   blending is performed on a specified subset of joints indicated by the index array. Each joint's weight is set to zero after blending.

		\param joints Pointer to the destination joint array where the blended results will be stored.
		\param blendJoints Pointer to the source joint array containing the blend target transformations.
		\param lerp Interpolation factor between 0 and 1, where 0 means no blending and 1 means full blend.
		\param index Array of joint indices specifying which joints to process.
		\param numJoints Number of joints to process from the index array.
	*/
	virtual void VPCALL		   BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	/*!
		\brief Performs a fast joint blending operation on a set of joints using linear interpolation.

		This function takes an array of joint quaternions and blends them with corresponding blend joint quaternions using linear interpolation. The blending is performed for a specified number of
	   joints, with the index array determining which joints to process. Each joint's quaternion and translation are interpolated, and the weight is set to zero after blending.

		\param joints Output array of joint quaternions to be modified with blended values
		\param blendJoints Input array of joint quaternions to be blended with the joints
		\param lerp Interpolation factor between 0 and 1 for blending the joints
		\param index Array of indices indicating which joints to process
		\param numJoints Number of joints to process in the blending operation
		\return void
	*/
	virtual void VPCALL		   BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );

	/*!
		\brief Converts an array of joint quaternions to joint matrices by setting the rotation and translation components for each joint.

		This function takes an array of joint quaternions and converts them into joint matrices. Each joint matrix is constructed by setting the rotation component from the quaternion's 3x3 matrix
	   representation and the translation component from the quaternion's translation vector. The conversion is performed for a specified number of joints.

		\param jointMats Output array of joint matrices to be filled with converted data
		\param jointQuats Input array of joint quaternions to be converted
		\param numJoints Number of joints to process in the conversion
		\return void VPCALL
	*/
	virtual void VPCALL		   ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints );

	/*!
		\brief Converts an array of joint matrices to joint quaternions by transforming each joint matrix to a joint quaternion.

		This function takes an array of joint matrices and converts them into an array of joint quaternions. Each joint matrix is transformed into a joint quaternion using the ToJointQuat() method.
	   The conversion is performed for a specified number of joints, processing each joint in the array sequentially.

		\param jointQuats Output array where the converted joint quaternions will be stored
		\param jointMats Input array of joint matrices to be converted
		\param numJoints Number of joints to process in the conversion
	*/
	virtual void VPCALL		   ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints );

	/*!
		\brief Transforms joints by applying parent transformations to child joints in reverse order.

		This function processes a range of joints, starting from firstJoint up to and including lastJoint. For each joint, it applies the transformation from its parent joint to the joint's current
	   transformation matrix. The operation is performed in reverse order, meaning joints are processed from the last to the first in the specified range. This ensures that child joints are correctly
	   transformed relative to their parent joints.

		\param jointMats Array of joint transformation matrices to be updated
		\param parents Array of parent joint indices for each joint
		\param firstJoint Index of the first joint to process
		\param lastJoint Index of the last joint to process
		\throws assertion failure if a parent index is not less than the current joint index
	*/
	virtual void VPCALL		   TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );

	/*!
		\brief This function untransforms joints by dividing each joint matrix by its parent joint matrix in reverse order.

		The function processes a range of joints from lastJoint down to firstJoint, performing a division operation between each joint matrix and its parent joint matrix. The assertion ensures that
	   the parent index is always less than the current joint index, maintaining a valid hierarchical structure. This operation modifies the joint matrices in place to reflect their local
	   transformations relative to their parents.

		\param jointMats Array of joint matrices to be untransformed
		\param parents Array of parent indices for each joint
		\param firstJoint Index of the first joint to process
		\param lastJoint Index of the last joint to process
		\throws assertion failure if a parent index is not less than the current joint index
	*/
	virtual void VPCALL		   UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
};

#endif /* !__MATH_SIMD_GENERIC_H__ */
