/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014 Robert Beckebans

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
#ifndef __RENDERMATRIX_H__
#define __RENDERMATRIX_H__

static const int NUM_FRUSTUM_CORNERS = 8;

struct frustumCorners_t {
	float x[NUM_FRUSTUM_CORNERS];
	float y[NUM_FRUSTUM_CORNERS];
	float z[NUM_FRUSTUM_CORNERS];
};

enum frustumCull_t { FRUSTUM_CULL_FRONT = 1, FRUSTUM_CULL_BACK = 2, FRUSTUM_CULL_CROSS = 3 };

/*
================================================================================================

idRenderMatrix

This is a row-major matrix and transforms are applied with left-multiplication.

================================================================================================
*/
class idRenderMatrix
{
public:
	//! Initializes an empty idRenderMatrix object.
	idRenderMatrix() { }

	/*!
		\brief Initializes a 4x4 transformation matrix with the specified row-major values.

		This constructor sets up a 4x4 matrix used for 3D transformations. The parameters are organized as row-major values, where each group of four values represents a row of the matrix. The matrix
	   is stored in a flattened array format with 16 elements total.

		\param a0 First element of the first row
		\param a1 Second element of the first row
		\param a2 Third element of the first row
		\param a3 Fourth element of the first row
		\param b0 First element of the second row
		\param b1 Second element of the second row
		\param b2 Third element of the second row
		\param b3 Fourth element of the second row
		\param c0 First element of the third row
		\param c1 Second element of the third row
		\param c2 Third element of the third row
		\param c3 Fourth element of the third row
		\param d0 First element of the fourth row
		\param d1 Second element of the fourth row
		\param d2 Third element of the fourth row
		\param d3 Fourth element of the fourth row
	*/
	ID_INLINE	 idRenderMatrix( float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3, float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3 );

	const float* operator[]( int index ) const
	{
		assert( index >= 0 && index < 4 );
		return &m[index * 4];
	}
	float* operator[]( int index )
	{
		assert( index >= 0 && index < 4 );
		return &m[index * 4];
	}

	//! Sets all elements of the matrix to zero.
	void				  Zero() { memset( m, 0, sizeof( m ) ); }

	/*!
		\brief Sets the render matrix to the identity matrix.

		This function initializes the render matrix to represent an identity transformation, which means no scaling, rotation, or translation is applied. Each row of the 4x4 matrix is explicitly set
	   to the values corresponding to an identity matrix. The matrix is stored in row-major order, and this function directly modifies the internal memory representation of the matrix.

	*/
	ID_INLINE void		  Identity();

	/*!
		\brief Checks if all elements of the matrix are zero within a given epsilon tolerance.

		This function evaluates whether each element of the matrix is within the specified epsilon tolerance of zero. It iterates through all 16 elements of the matrix and returns false immediately if
	   any element exceeds the epsilon threshold. The function is designed for use in assertions and diagnostic checks to verify matrix properties.

		\param epsilon tolerance value for comparing matrix elements to zero
		\return true if all matrix elements are within epsilon of zero, false otherwise
	*/
	ID_INLINE bool		  IsZero( float epsilon ) const;

	/*!
		\brief Checks if the matrix is an identity matrix within a given epsilon tolerance

		This function verifies whether the current matrix represents an identity matrix by comparing each element against the expected identity values. For diagonal elements, it checks if they are
	   approximately equal to 1.0f, and for off-diagonal elements, it checks if they are approximately equal to 0.0f. The comparison uses the provided epsilon value to determine if the difference is
	   within acceptable floating-point precision

		\param epsilon tolerance value for floating-point comparison
		\return true if the matrix is identity within the specified epsilon, false otherwise
	*/
	ID_INLINE bool		  IsIdentity( float epsilon ) const;

	/*!
		\brief Checks if the matrix represents an affine transformation within the given epsilon tolerance.

		An affine transformation matrix has specific properties in its fourth row. This function verifies that the first three elements of the fourth row are close to zero and the fourth element is
	   close to one, within the provided epsilon tolerance. This ensures the matrix maintains the characteristics of an affine transformation.

		\param epsilon tolerance value for floating point comparison
		\return true if the matrix represents an affine transformation within the given epsilon tolerance, false otherwise
	*/
	ID_INLINE bool		  IsAffineTransform( float epsilon ) const;

	/*!
		\brief Determines if the matrix represents a uniform scale transformation within a given epsilon tolerance.

		This function checks whether the scaling factors along the three axes (x, y, z) are equal within the specified tolerance. It computes the inverse square root of the squared lengths of the
	   first three rows of the matrix to determine the scale factors, then compares them using the provided epsilon value. If all scale factors are within the epsilon tolerance of each other, the
	   matrix represents a uniform scale.

		\param epsilon The tolerance threshold for comparing scale factors
		\return True if the scale factors along all three axes are equal within the epsilon tolerance, false otherwise
	*/
	ID_INLINE bool		  IsUniformScale( float epsilon ) const;

	/*!
		\brief Transforms a 3D point using the matrix and stores the result in the output vector.

		This function applies a 4x4 transformation matrix to a 3D point, performing matrix-vector multiplication. The transformation includes both linear transformation and translation components from
	   the matrix. The function does not perform division by the W component, meaning it preserves the original homogeneous coordinate values. The input and output vectors must not point to the same
	   memory location to avoid undefined behavior. An assertion ensures the W component of the input point is normalized to 1.0.

		\param in The input 3D point to be transformed
		\param out The output 3D point after transformation
		\throws assertion failure if input and output vectors reference the same memory location, or if the W component of input point is not close to 1.0
	*/
	ID_INLINE void		  TransformPoint( const idVec3& in, idVec3& out ) const;

	/*!
		\brief Transforms a 3D point using the render matrix and stores the result in a 4D vector.

		This function applies a 4x4 transformation matrix to a 3D vector, treating the input as a 4D vector with a w-component of 1.0. The result is stored in the provided 4D output vector. The
	   function includes an assertion to ensure that the input and output vectors do not overlap in memory.

		\param in The input 3D vector to be transformed
		\param out The output 4D vector that will contain the transformed result
		\throws assertion failure if the input and output vectors overlap in memory
	*/
	ID_INLINE void		  TransformPoint( const idVec3& in, idVec4& out ) const;

	/*!
		\brief Transforms a point using the render matrix

		This function applies a 4x4 matrix transformation to a 4-dimensional vector representing a point in space. The transformation is performed by multiplying the input vector with the matrix,
	   where each component of the output vector is computed as a dot product of the input vector with the corresponding row of the matrix. The function includes an assertion to ensure that the input
	   and output vectors do not overlap in memory.

		\param in The input 4-dimensional vector to be transformed
		\param out The output 4-dimensional vector storing the transformed result
		\return This function does not return a value
		\throws This function throws an assertion error if the input and output vectors overlap in memory
	*/
	ID_INLINE void		  TransformPoint( const idVec4& in, idVec4& out ) const;

	/*!
		\brief Transforms a direction vector using the render matrix, optionally normalizing the result.

		This function applies a 3x3 transformation matrix to the input direction vector. The transformation assumes the matrix contains no non-uniform scaling or shearing. If the normalize parameter
	   is true, the resulting vector will be normalized to unit length. Note that the direction vector will only remain normalized if the matrix has no skewing or scaling operations.

		\param in The input direction vector to be transformed
		\param out The output transformed direction vector
		\param normalize If true, the result will be normalized to unit length
	*/
	ID_INLINE void		  TransformDir( const idVec3& in, idVec3& out, bool normalize ) const;

	/*!
		\brief Transforms a plane using the matrix and optionally normalizes the result.

		This function applies a linear transformation to a plane represented by its normal vector and distance from origin. The transformation accounts for the matrix's rotation and scaling
	   components. If the normalize flag is set to true, the resulting plane's normal vector will be normalized. The function assumes that the matrix has a uniform scale within a tolerance of 0.01f as
	   verified by the IsUniformScale check.

		\param in The input plane to be transformed, represented by its normal vector and distance.
		\param out The output plane after transformation, containing the transformed normal and distance.
		\param normalize Flag indicating whether to normalize the resulting plane's normal vector.
		\throws assertion failure if the matrix does not have a uniform scale within 0.01f tolerance
	*/
	ID_INLINE void		  TransformPlane( const idPlane& in, idPlane& out, bool normalize ) const;

	/*!
		\brief Applies the inverse transformation of this matrix to a direction vector, optionally normalizing the result.

		This function transforms a direction vector using the inverse of the current matrix. It is designed to handle non-uniform scaling and shearing by utilizing the transpose of the inverse matrix.
	   The input vector is transformed by multiplying it with the inverse matrix's transpose. If the normalize flag is set to true, the resulting vector is normalized to unit length. The function
	   asserts that the input and output vectors do not reference the same memory location to prevent overwriting during computation.

		\param in The input direction vector to be transformed.
		\param out The output vector that will contain the transformed direction.
		\param normalize Flag indicating whether the result should be normalized to unit length.
		\throws assertion if in and out refer to the same memory location
	*/
	ID_INLINE void		  InverseTransformDir( const idVec3& in, idVec3& out, bool normalize ) const;

	/*!
		\brief Transforms a plane by the inverse of this matrix, optionally normalizing the result.

		This function applies the inverse transformation of the matrix to the input plane and stores the result in the output plane. It performs a matrix multiplication with the plane's coefficients
	   to transform the plane. If the normalize flag is true, the resulting plane normal is normalized using the inverse square root calculation. The function ensures that the input and output planes
	   do not overlap in memory through an assertion.

		\param in The input plane to be transformed
		\param out The output plane that will contain the transformed result
		\param normalize Flag indicating whether to normalize the resulting plane normal
		\throws assertion failure if the input and output planes overlap in memory
	*/
	ID_INLINE void		  InverseTransformPlane( const idPlane& in, idPlane& out, bool normalize ) const;

	/*!
		\brief Transforms a 3D model space point to clip space using model and projection matrices.

		This function performs a two-step transformation of a 3D point from model space to clip space. First, it transforms the input point from model space to eye space by multiplying the point with
	   the model matrix. Then, it transforms the resulting eye space point to clip space by multiplying with the projection matrix. The function operates on the provided input point and writes the
	   intermediate eye space result and final clip space result to the specified output vectors.

		\param src The input 3D point in model space to be transformed
		\param modelMatrix The model matrix used to transform model space to eye space
		\param projectionMatrix The projection matrix used to transform eye space to clip space
		\param eye Output vector for the intermediate eye space result
		\param clip Output vector for the final clip space result
		\return This function does not return a value as it operates on the provided output parameters directly.
	*/
	static ID_INLINE void TransformModelToClip( const idVec3& src, const idRenderMatrix& modelMatrix, const idRenderMatrix& projectionMatrix, idVec4& eye, idVec4& clip );

	/*!
		\brief Transforms a clip space coordinate to device space by performing perspective division.

		This function takes a 4D clip space coordinate and converts it to 3D device space by dividing the x, y, and z components by the w component. The function assumes that the w component is
	   non-zero and within valid floating-point range to prevent division by zero or denormal numbers. The resulting 3D vector represents the normalized device coordinates.

		\param clip A 4D vector representing the clip space coordinate with components [x, y, z, w]
		\param ndc A 3D vector that will store the resulting normalized device coordinates [x, y, z] after perspective division
		\throws assertion failure if the w component of the clip vector is zero or a denormal number
	*/
	static ID_INLINE void TransformClipToDevice( const idVec4& clip, idVec3& ndc );

	/*!
		\brief Creates a 4x4 transformation matrix from an origin, axis, and scale.

		This function constructs a transformation matrix that represents a rigid body transformation in 3D space. The matrix combines rotation defined by the axis parameter, translation defined by the
	   origin parameter, and uniform scaling defined by the scale parameter. The resulting matrix can be used to transform vertices from local space to world space. The function populates the output
	   matrix with the combined transformation data.

		\param origin Position vector defining the translation component of the matrix
		\param axis Orientation matrix defining the rotation and basis vectors
		\param out Output render matrix that will be populated with the transformation data
	*/
	static void			  CreateFromOriginAxis( const idVec3& origin, const idMat3& axis, idRenderMatrix& out );

	/*!
		\brief Creates a render matrix from an origin, axis, and scale.

		This function constructs a 4x4 transformation matrix using the provided origin, axis, and scale parameters. The axis matrix defines the orientation, while the scale vector defines the scaling
	   factors along each axis. The origin vector defines the translation component of the matrix.

		\param origin Position vector defining the translation component of the matrix
		\param axis Orientation matrix defining the rotation and basis vectors
		\param scale Scaling factors along the x, y, and z axes
		\param out Output render matrix that will be populated with the transformation data
	*/
	static void			  CreateFromOriginAxisScale( const idVec3& origin, const idMat3& axis, const idVec3& scale, idRenderMatrix& out );

	/*!
		\brief Creates a view matrix that transforms global coordinates to view coordinates for OpenGL rendering

		This function generates a transformation matrix that converts global world coordinates into view space coordinates suitable for OpenGL rendering. The matrix is constructed using the provided
	   origin and axis vectors, where the axis represents the view orientation. The function sets up a transformation that accounts for the specific coordinate system conventions used in OpenGL,
	   particularly when looking down the negative Z axis. The resulting matrix has the view origin and orientation properly encoded in its elements, with the fourth row representing the homogeneous
	   coordinate system normalization.

		\param origin The global origin point of the view
		\param axis The view orientation represented as a 3x3 matrix
		\param out The output matrix that will contain the view transformation
	*/
	static void			  CreateViewMatrix( const idVec3& origin, const idMat3& axis, idRenderMatrix& out );

	/*!
		\brief Creates a projection matrix for rendering with the specified frustum parameters

		This function generates a 4x4 projection matrix that defines the viewing frustum for 3D rendering. The matrix is constructed using the given parameters that specify the boundaries of the
	   frustum in eye space. The function handles both finite and infinite far-plane cases, with special handling for DirectX clip space conventions. The resulting matrix is stored in the output
	   parameter and can be used directly for rendering operations.

		\param xMin Minimum X coordinate of the near clipping plane
		\param xMax Maximum X coordinate of the near clipping plane
		\param yMin Minimum Y coordinate of the near clipping plane
		\param yMax Maximum Y coordinate of the near clipping plane
		\param zNear Distance to the near clipping plane
		\param zFar Distance to the far clipping plane
		\param out Output parameter that will contain the generated projection matrix
	*/
	static void			  CreateProjectionMatrix( float xMin, float xMax, float yMin, float yMax, float zNear, float zFar, idRenderMatrix& out );

	/*!
		\brief Creates a projection matrix using field of view parameters and offset values

		This function constructs a projection matrix for rendering purposes based on the specified field of view angles in both x and y directions. It calculates the near and far clipping plane
	   boundaries and applies the specified offsets to the x and y dimensions. The resulting matrix is stored in the output parameter.

		\param xFovDegrees Horizontal field of view angle in degrees
		\param yFovDegrees Vertical field of view angle in degrees
		\param zNear Near clipping plane distance
		\param zFar Far clipping plane distance
		\param xOffset Horizontal offset applied to the projection
		\param yOffset Vertical offset applied to the projection
		\param out Output matrix that will contain the resulting projection matrix
	*/
	static void			  CreateProjectionMatrixFov( float xFovDegrees, float yFovDegrees, float zNear, float zFar, float xOffset, float yOffset, idRenderMatrix& out );

	/*!
		\brief Creates a DirectX-style projection matrix with the specified field of view, aspect ratio, and depth range.

		This function generates a projection matrix suitable for DirectX applications. It calculates the scaling factors based on the vertical field of view, aspect ratio, and near and far clipping
	   plane distances. The resulting matrix is stored in the output parameter and is configured for a left-handed coordinate system with depth values ranging from 0 to 1.

		\param verticalFov The vertical field of view in radians
		\param aspect The aspect ratio of the viewport (width/height)
		\param zNear The distance to the near clipping plane
		\param zFar The distance to the far clipping plane
		\param out The output matrix that will contain the computed projection matrix
	*/
	static void			  CreateProjD3DStyle( float verticalFov, float aspect, float zNear, float zFar, idRenderMatrix& out );

	/*!
		\brief Applies a depth hack to the z-coordinate of the source render matrix by scaling it by 25%.

		This function modifies the z-coordinate components of the source render matrix by scaling them down by 25%. It is used to adjust the depth of rendered models to achieve specific visual effects
	   in 3D space. The modification affects the projected z-coordinate which influences how the matrix is rendered.

		\param src The source render matrix whose z-coordinate will be modified
	*/
	static ID_INLINE void ApplyDepthHack( idRenderMatrix& src );

	/*!
		\brief Applies a depth hack offset to the z-coordinate of the source render matrix.

		This function modifies the z-coordinate of the source render matrix by subtracting the specified depth hack value. The modification affects the projected z-coordinate, which influences how the
	   matrix is rendered in 3D space. This is typically used to adjust the depth of rendered models to achieve specific visual effects.

		\param src The source render matrix whose z-coordinate will be modified
		\param value The depth hack value to subtract from the z-coordinate
	*/
	static ID_INLINE void ApplyModelDepthHack( idRenderMatrix& src, float value );

	/*!
		\brief Computes a transformation matrix that scales and offsets an input matrix to map a unit cube to the specified bounding volume.

		This function takes an input matrix and modifies it to create a new transformation matrix that scales and offsets the input such that the resulting matrix maps a unit cube to exactly cover the
	   provided bounds. The operation is performed in a manner that is optimized for performance using SSE intrinsics when available, falling back to standard floating-point operations otherwise.

		\param src The input transformation matrix to be modified.
		\param bounds The bounding volume that defines the target space for the transformation.
		\param out The output transformation matrix that results from the offset and scale operation.
		\throws Assertion failure if src and out refer to the same matrix.
	*/
	static void			  OffsetScaleForBounds( const idRenderMatrix& src, const idBounds& bounds, idRenderMatrix& out );

	/*!
		\brief Computes the inverse offset and scale transformation for a given matrix based on the specified bounds

		This function applies an inverse offset and scale transformation to the input matrix src using the provided bounds to calculate the transformation parameters. The transformation is applied to
	   map coordinates from the bounds space to a normalized space. The operation is performed either using SSE intrinsics for optimized performance or using standard C++ operations. The result is
	   stored in the output matrix out.

		\param src The input matrix to be transformed
		\param bounds The bounds used to calculate the offset and scale factors
		\param out The output matrix that will contain the transformed result
		\throws assertion failure if src and out refer to the same matrix
	*/
	static void			  InverseOffsetScaleForBounds( const idRenderMatrix& src, const idBounds& bounds, idRenderMatrix& out );

	/*!
		\brief Computes the transpose of a 4x4 matrix and stores the result in another matrix

		This function calculates the transpose of a 4x4 matrix, which involves swapping the rows and columns of the source matrix. The operation is performed in-place by loading matrix elements and
	   rearranging them using SIMD instructions when available, or through direct element assignment when SIMD is not enabled. The function asserts that the source and output matrices are different to
	   prevent unexpected behavior due to aliasing

		\param src The source matrix to be transposed
		\param out The matrix that will store the transposed result
		\throws Assertion failure if the source and output matrices are the same object
	*/
	static void			  Transpose( const idRenderMatrix& src, idRenderMatrix& out );

	/*!
		\brief Performs matrix multiplication of two 4x4 matrices and stores the result in a third matrix

		This function multiplies two 4x4 matrices a and b, storing the result in the out matrix. It provides two implementations: one using SSE intrinsics for optimized performance on compatible
	   hardware, and a standard C++ implementation that uses explicit loop unrolling for compatibility. The multiplication follows standard matrix multiplication rules where each element of the result
	   matrix is the dot product of a row from the first matrix with a column from the second matrix. The function is commonly used in graphics operations for combining transformation matrices.

		\param a First 4x4 matrix to multiply
		\param b Second 4x4 matrix to multiply
		\param out Output 4x4 matrix to store the result
	*/
	static void			  Multiply( const idRenderMatrix& a, const idRenderMatrix& b, idRenderMatrix& out );

	/*!
		\brief Computes the inverse of a 4x4 transformation matrix using SSE intrinsics and returns true if successful.

		This function calculates the inverse of a 4x4 matrix using SIMD instructions for performance optimization. It employs cofactor calculation and determinant computation via SSE2 intrinsics. The
	   function checks for numerical stability by examining the absolute value of the determinant against a predefined epsilon threshold. If the determinant is too close to zero, the function returns
	   false indicating that the matrix is singular and cannot be inverted. The result is stored in the output parameter.
	   
	   This code is based on the code written by Cédric Lallain, published on "Cell Performance"
	   (by Mike Acton) and released under the BSD 3-Clause ("BSD New" or "BSD Simplified") license.
	   https://code.google.com/p/cellperformance-snippets/
	   
	   	\note that large parallel lights can have very small values in the projection matrix, scaling tens of thousands of world units down to a 0-1 range, so the determinants can get really, really small.

		\param src The input 4x4 transformation matrix to be inverted
		\param out The resulting inverted matrix if successful, undefined if failed
		\return True if the matrix inversion was successful and the determinant was non-zero, false otherwise.
	*/
	static bool			  Inverse( const idRenderMatrix& src, idRenderMatrix& out );

	/*!
		\brief Computes the inverse transpose of an affine transformation matrix and stores the result in the output matrix.

		This function calculates the inverse transpose of a 4x4 matrix that represents an affine transformation. It assumes the input matrix is an affine transformation and performs the operation by
	   transposing the upper 3x3 submatrix and adjusting the translation component accordingly. The function modifies the output matrix in place and includes assertions to verify that the input is
	   valid and that the input and output matrices are distinct objects.

		\param src The input matrix which must be an affine transformation
		\param out The output matrix that will contain the inverse transpose of the input
		\throws assertion failure if src and out refer to the same matrix or if src is not an affine transformation
	*/
	static void			  InverseByTranspose( const idRenderMatrix& src, idRenderMatrix& out );

	/*!
		\brief Computes the inverse of a 4x4 matrix using double-precision arithmetic and stores the result in the output matrix

		This function calculates the inverse of a 4x4 matrix by computing the determinant and using cofactor expansion. It employs double-precision floating-point arithmetic for improved numerical
	   stability during the computation. The function returns true if the matrix is invertible (i.e., the determinant is non-zero) and false otherwise. The inverse matrix is stored in the output
	   parameter.
	   This should never be used at run-time. This is only for tools where more precision is needed.

		\param src The input 4x4 matrix to be inverted
		\param out The output matrix that will contain the inverse of the input matrix
		\return True if the matrix is invertible and the inverse has been computed, false if the matrix is singular (determinant is zero)
	*/
	static bool			  InverseByDoubles( const idRenderMatrix& src, idRenderMatrix& out );

	/*!
		\brief Copies matrix rows into four provided vector objects, using SSE intrinsics when available for optimized memory operations.

		This function extracts the four rows of the provided matrix and stores them in the specified row vectors. It uses SSE intrinsics when available for optimized memory operations, falling back to
	   standard array access when SSE is not enabled. All row vectors must be 16-byte aligned for proper SSE operation.

		\param matrix The matrix to extract rows from
		\param row0 First row of the matrix
		\param row1 Second row of the matrix
		\param row2 Third row of the matrix
		\param row3 Fourth row of the matrix
		\throws assertion failure if any of the row vectors are not 16-byte aligned
	*/
	static void			  CopyMatrix( const idRenderMatrix& matrix, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3 );

	/*!
		\brief Sets the model-view-projection matrix components into separate row vectors and determines if the determinant is negative

		This function extracts the four rows of the provided model-view-projection matrix and stores them in the specified row vectors. It also calculates whether the determinant of the matrix is
	   negative. The function uses SSE intrinsics when available for optimized memory operations, falling back to standard array access when SSE is not enabled. All row vectors must be 16-byte aligned
	   for proper SSE operation.

		\param mvp The model-view-projection matrix to extract rows from
		\param row0 First row of the matrix
		\param row1 Second row of the matrix
		\param row2 Third row of the matrix
		\param row3 Fourth row of the matrix
		\param negativeDeterminant Output parameter indicating if the matrix determinant is negative
	*/
	static void			  SetMVP( const idRenderMatrix& mvp, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	/*!
		\brief Computes a modified model-view-projection matrix for a given bounding volume.

		This function takes a model-view-projection matrix and a bounding volume, and computes a new set of matrix rows that represent the transformation of the bounding volume in the coordinate space
	   defined by the input matrix. The computation accounts for both scaling and offsetting of the bounding volume, and stores the resulting matrix rows in the provided output vectors. The function
	   also determines whether the determinant of the resulting matrix is negative, which can be used to detect orientation changes.

		\param mvp The input model-view-projection matrix to be used for transformation.
		\param bounds The bounding volume to be transformed.
		\param row0 Output vector for the first row of the resulting matrix.
		\param row1 Output vector for the second row of the resulting matrix.
		\param row2 Output vector for the third row of the resulting matrix.
		\param row3 Output vector for the fourth row of the resulting matrix.
		\param negativeDeterminant Output boolean indicating whether the determinant of the resulting matrix is negative.
	*/
	static void			  SetMVPForBounds( const idRenderMatrix& mvp, const idBounds& bounds, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	/*!
		\brief Computes the combined transformation matrix for model-view-projection and inverse projection, and determines if the determinant is negative.

		This function performs matrix multiplication between the model-view-projection matrix and the inverse projection matrix. It stores the result in four row vectors and also determines whether
	   the determinant of the resulting matrix is negative. The function supports both SSE intrinsics and standard scalar operations for performance optimization. All row vectors must be 16-byte
	   aligned for SSE operations.

		\param mvp The model-view-projection matrix to be multiplied
		\param inverseProject The inverse projection matrix to be multiplied
		\param row0 Output vector for the first row of the result matrix
		\param row1 Output vector for the second row of the result matrix
		\param row2 Output vector for the third row of the result matrix
		\param row3 Output vector for the fourth row of the result matrix
		\param negativeDeterminant Output boolean indicating if the determinant of the result matrix is negative
	*/
	static void			  SetMVPForInverseProject( const idRenderMatrix& mvp, const idRenderMatrix& inverseProject, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	/*!
		\brief Determines if a point is outside the viewing frustum defined by a Model-View-Projection matrix.

		This function checks if a given point is culled or outside the viewing frustum represented by the Model-View-Projection matrix. It is commonly used for early rejection of geometry that is not
	   visible to the camera. The function uses an internal bit-based culling mechanism for performance. When the zeroToOne parameter is true, it indicates that the coordinate system uses a
	   zero-to-one depth range instead of the typical negative-one-to-one range.

		\param mvp The Model-View-Projection matrix defining the viewing frustum.
		\param point The 3D point to test against the frustum.
		\param zeroToOne Flag indicating if the depth range is zero-to-one instead of negative-one-to-one.
		\return True if the point is outside the viewing frustum, false otherwise.
	*/
	static bool			  CullPointToMVP( const idRenderMatrix& mvp, const idVec3& point, bool zeroToOne = false );

	/*!
		\brief Determines if a point is inside or outside the clip space defined by the MVP matrix and returns culling bits.

		This function transforms a point using the provided Model-View-Projection matrix and checks whether the point lies within the clip space boundaries. It calculates culling bits that indicate
	   which planes the point is outside of. The function returns true if the point is completely outside any of the clip space bounds, and false otherwise. The zeroToOne parameter affects the minimum
	   W value used for comparison, which is relevant for different graphics API clip space conventions.

		\param mvp The Model-View-Projection matrix used to transform the point
		\param point The 3D point to check against the clip space
		\param outBits Pointer to store the resulting culling bits
		\param zeroToOne Flag indicating whether the clip space Z is in the range [0,1]
		\return True if the point is completely outside the clip space, false otherwise.
	*/
	static bool			  CullPointToMVPbits( const idRenderMatrix& mvp, const idVec3& point, byte* outBits, bool zeroToOne = false );

	/*!
		\brief Culls a bounding box against the model-view-projection matrix and returns true if the bounds are outside the view frustum.

		This function performs frustum culling on a given bounding box using the provided model-view-projection matrix. It determines whether the bounding box is completely outside the view frustum,
	   which can be used to skip rendering or shadow calculations for invisible objects. The function takes into account the coordinate system used, with the zeroToOne parameter controlling whether
	   the coordinate system is in the range [0,1] or [-1,1]. This is commonly used in graphics rendering to efficiently determine if geometry needs to be processed or if it's completely outside the
	   camera's field of view.

		\param mvp The model-view-projection matrix used for transforming the bounds into clip space
		\param bounds The bounding box to test against the view frustum
		\param zeroToOne Flag indicating whether to use [0,1] coordinate system (true) or [-1,1] coordinate system (false)
		\return True if the bounding box is completely outside the view frustum, false if it intersects or is inside the view frustum
	*/
	static bool			  CullBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, bool zeroToOne = false );

	/*!
		\brief Culls a bounding box against the view-projection matrix and returns bit flags indicating which sides of the frustum the bounds is outside.

		This function performs frustum culling on a given bounding box using the provided view-projection matrix. It determines whether the bounding box is entirely outside any of the six frustum
	   planes (left, right, bottom, top, near, far). The function supports both SSE optimized and fallback scalar implementations. The output bits indicate which sides of the frustum the bounds is
	   outside of, with each bit corresponding to a specific frustum plane. The zeroToOne parameter affects how the Z-coordinate is treated in the culling calculation.

		\param mvp The view-projection matrix to cull against
		\param bounds The bounding box to test
		\param outBits Output parameter that will contain the culling bit flags
		\param zeroToOne If true, Z-coordinates are in [0,1] range; if false, Z-coordinates are in [-1,1] range
		\return True if the bounding box is completely outside any of the frustum sides, false otherwise
	*/
	static bool			  CullBoundsToMVPbits( const idRenderMatrix& mvp, const idBounds& bounds, byte* outBits, bool zeroToOne = false );

	/*!
		\brief Determines if extruded bounds are culled against a model-view-projection matrix and clip plane.

		This function evaluates whether a given bounds, when extruded in a specified direction, is culled by the provided model-view-projection matrix and clip plane. It returns a boolean indicating
	   the culling result.

		\param mvp The model-view-projection matrix used for transformation and culling.
		\param bounds The bounding volume to be tested against the culling conditions.
		\param extrudeDirection The direction in which the bounds are extruded for culling.
		\param clipPlane The clipping plane used to determine culling.
		\param zeroToOne Flag indicating if the coordinate system is zero-to-one, affects the culling calculations.
		\return True if the extruded bounds are culled, false otherwise.
	*/
	static bool			  CullExtrudedBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool zeroToOne = false );

	/*!
		\brief Tests if an extruded bounding box is culled against a clip plane and stores the culling results in bits.

		This function performs culling operations on a bounding box that has been extruded in a specified direction. It uses the model-view-projection matrix to transform the bounding box and then
	   determines whether the extruded bounds are outside the clip space defined by the clip plane. The results are stored as bit flags indicating which sides of the bounding box are culled. The
	   function uses SSE intrinsics for optimized performance on compatible hardware.
	   
	   Normally the clip space extends from -1.0 to 1.0 on each axis, but by setting 'zeroToOne' to true, 
	   the clip space will extend from 0.0 to 1.0 on each axis for a light projection matrix.

		\param mvp The model-view-projection matrix used for transforming the bounding box
		\param bounds The original bounding box to be extruded
		\param extrudeDirection The direction vector used to extrude the bounding box
		\param clipPlane The clip plane against which to test culling
		\param outBits Pointer to store the resulting culling bit flags
		\param zeroToOne Flag indicating whether the clip space is in the range [0,1] instead of [-1,1]
		\return true if the extruded bounding box is completely outside the clip space, false otherwise
		\throws assertion failure if the dot product of extrudeDirection and clipPlane.Normal() is too close to zero, indicating they are nearly perpendicular
	*/
	static bool CullExtrudedBoundsToMVPbits( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, byte* outBits, bool zeroToOne = false );

	/*!
		\brief Computes the bounding box of a given bounds after being transformed by an MVP matrix and optionally converted to window space.

		This function calculates the projected bounding box of a 3D bounds object using a model-view-projection matrix. It supports both SIMD and scalar implementations for performance. The result is
	   stored in the projected bounds structure. If windowSpace is true, the projected bounds are normalized to the range [0, 1] in all three dimensions, which is useful for screen-space calculations.
	   The function handles edge cases such as division by zero in the perspective divide by using a minimum non-denormal float value for the w component.

		\param projected The resulting bounds after projection
		\param mvp The model-view-projection matrix used for transformation
		\param bounds The original 3D bounds to be projected
		\param windowSpace Whether to convert the result to window space coordinates [0, 1]
	*/
	static void ProjectedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Computes the projected and near-clipped bounds of a 3D bounding box using a model-view-projection matrix

		This function calculates the bounding box that results from projecting a 3D axis-aligned bounding box through a model-view-projection matrix and clipping against the near clipping plane. The
	   function uses SSE intrinsics for optimized performance and handles the mathematical computation of intersection points with the near plane. It takes into account the projection space convention
	   (D3D vs OpenGL) when determining the near clipping plane. The resulting projected bounds are stored in the output parameter, which can optionally be in window space coordinates if the
	   windowSpace flag is set to true.

		\param projected Output parameter that will store the resulting projected and clipped bounds
		\param mvp The model-view-projection matrix used for the transformation
		\param bounds The input 3D axis-aligned bounding box to be projected
		\param windowSpace Flag indicating whether the result should be in window space coordinates
	*/
	static void ProjectedNearClippedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Computes the projected bounds of a 3D bounding box after applying a model-view-projection matrix, with full clipping against the unit cube.

		This function calculates the projected bounding box of a given 3D bounds after transforming it by the provided model-view-projection matrix. It uses SSE intrinsics for optimized performance.
	   The function applies clipping against the unit cube to ensure that only points within the visible range are considered. The result is stored in the provided projected bounds object. The
	   windowSpace parameter determines whether the projection is done in window space or normalized device coordinates.
	   
	    \note This code assumes the MVP matrix has an infinite far clipping plane. When the far plane is at infinity the bounds are never far clipped and it is sufficient to test whether or not the center of the near clip plane is inside the bounds to calculate the correct minimum Z. If the far plane is not at infinity then this code would also have to test for the view frustum being completely contained inside the given bounds in which case the projected bounds should be set to fully cover the view frustum.

		\param projected Output parameter that will contain the resulting projected bounds
		\param mvp The model-view-projection matrix used for the transformation
		\param bounds The input 3D bounding box to be projected
		\param windowSpace Flag indicating whether the result should be in window space (true) or normalized device coordinates (false)
	*/
	static void ProjectedFullyClippedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Calculates the minimum and maximum depth values for a bounding box transformed by a model-view-projection matrix

		This function computes the depth bounds of a 3D bounding box after it has been transformed by a model-view-projection matrix. It supports both SSE optimized and scalar implementations. The
	   function processes all eight vertices of the bounding box and calculates their transformed Z coordinates. When windowSpace is true, the results are clamped to the [0,1] range and converted to
	   window coordinates. The function handles edge cases where the W component might be very small or zero by setting the Z value to negative infinity.

		\param min Output parameter for the minimum depth value
		\param max Output parameter for the maximum depth value
		\param mvp The model-view-projection matrix used for transformation
		\param bounds The 3D bounding box to compute depth bounds for
		\param windowSpace Flag indicating whether to convert results to window coordinates
	*/
	static void DepthBoundsForBounds( float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Computes the minimum and maximum depth values for a bounds extruded along a direction, projected through a model-view-projection matrix and clipped against a plane.

		This function calculates the depth bounds of a bounding box that has been extruded in a specified direction and then projected using a model-view-projection matrix. The projection is clipped
	   against a given plane to determine the final depth range. The results are stored in the provided min and max float references. An optimization using SSE intrinsics is employed when available.
	   If windowSpace is true, the depth values are clamped to the range [0, 1] and scaled to window space coordinates. The function assumes that the extrude direction and clip plane normal are not
	   parallel, as enforced by an assertion.

		\param min Reference to store the minimum depth value
		\param max Reference to store the maximum depth value
		\param mvp Model-view-projection matrix used for projection
		\param bounds Original bounding box to be projected
		\param extrudeDirection Direction along which the bounds are extruded
		\param clipPlane Plane used for clipping the extruded bounds
		\param windowSpace Flag indicating whether to scale results to window space [0,1]
		\throws Assertion failure if the dot product of extrudeDirection and clipPlane normal is too close to zero, indicating they are parallel.
	*/
	static void DepthBoundsForExtrudedBounds(
		float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool windowSpace = true );

	/*!
		\brief Computes the minimum and maximum depth values for shadow bounds using SSE intrinsics.

		This function calculates the depth bounds for shadows by transforming the bounding box vertices using the model-view-projection matrix and then clipping against near and far planes. It uses
	   SSE intrinsics for optimized computation of the transformed points and performs polygon clipping to determine the shadow bounds. The result is stored in the min and max parameters, which
	   represent the depth range for the shadow. The windowSpace parameter controls whether the results should be in window space coordinates.

		\param min Output parameter for the minimum depth value
		\param max Output parameter for the maximum depth value
		\param mvp The model-view-projection matrix used for transforming the bounding box vertices
		\param bounds The bounding box of the object for which shadow bounds are calculated
		\param localLightOrigin The origin of the light in local coordinates
		\param windowSpace Controls whether the results should be in window space coordinates
	*/
	static void			 DepthBoundsForShadowBounds( float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& localLightOrigin, bool windowSpace = true );

	/*!
		\brief Computes the six frustum planes from a transformation matrix.

		This function extracts the six frustum planes (left, right, bottom, top, near, far) from the provided transformation matrix. The planes are computed based on whether the coordinate system uses
	   zero-to-one or negative-one-to-one depth values. Optionally, the computed planes can be normalized.

		\param planes Output array of six frustum planes
		\param frustum Transformation matrix from which the frustum planes are derived
		\param zeroToOne Flag indicating whether the depth values are in zero-to-one range
		\param normalize Flag indicating whether the computed planes should be normalized
	*/
	static void			 GetFrustumPlanes( idPlane planes[6], const idRenderMatrix& frustum, bool zeroToOne, bool normalize );

	/*!
		\brief Computes the world-space corners of a frustum by transforming the frustum bounds using the provided transformation matrix

		This function calculates the eight corners of a frustum in world space by applying a transformation matrix to the frustum's bounding box. It uses SSE intrinsics for optimized performance when
	   available, otherwise falling back to scalar arithmetic. The function ensures proper alignment of the output corners structure and handles division by the w component for perspective divide. The
	   frustum bounds are treated as a 3D box defined by min and max coordinates for each axis, and the resulting corners represent the vertices of this transformed frustum.

		\param corners Output structure containing the 8 computed world-space frustum corner positions
		\param frustumTransform Transformation matrix to apply to the frustum bounds to compute world-space corners
		\param frustumBounds Bounding box defining the frustum in local space, specified as min and max coordinates for each axis
		\throws assertion failure if the corners parameter is not 16-byte aligned
	*/
	static void			 GetFrustumCorners( frustumCorners_t& corners, const idRenderMatrix& frustumTransform, const idBounds& frustumBounds );

	/*!
		\brief Determines the culling status of frustum corners relative to a given plane, using either SIMD intrinsics or scalar arithmetic.

		This function evaluates whether the corners of a frustum are in front of, behind, or intersecting a specified plane. It returns a culling result indicating if the frustum is entirely in front,
	   entirely behind, or crossing the plane. The implementation uses SSE intrinsics when available for performance, otherwise falling back to scalar calculations.

		\param corners The set of frustum corners to test against the plane
		\param plane The plane to test the frustum corners against
		\return A frustumCull_t value indicating if the frustum corners are entirely front, entirely back, or crossing the given plane
	*/
	static frustumCull_t CullFrustumCornersToPlane( const frustumCorners_t& corners, const idPlane& plane );

private:
	float m[16];
};

extern const idRenderMatrix renderMatrix_identity;
extern const idRenderMatrix renderMatrix_flipToOpenGL;
extern const idRenderMatrix renderMatrix_windowSpaceToClipSpace;
// RB begin
extern const idRenderMatrix renderMatrix_clipSpaceToWindowSpace;
extern const idRenderMatrix renderMatrix_fullscreen;

ID_INLINE					idRenderMatrix::idRenderMatrix(
	  float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3, float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3 )
{
	m[0 * 4 + 0] = a0;
	m[0 * 4 + 1] = a1;
	m[0 * 4 + 2] = a2;
	m[0 * 4 + 3] = a3;
	m[1 * 4 + 0] = b0;
	m[1 * 4 + 1] = b1;
	m[1 * 4 + 2] = b2;
	m[1 * 4 + 3] = b3;
	m[2 * 4 + 0] = c0;
	m[2 * 4 + 1] = c1;
	m[2 * 4 + 2] = c2;
	m[2 * 4 + 3] = c3;
	m[3 * 4 + 0] = d0;
	m[3 * 4 + 1] = d1;
	m[3 * 4 + 2] = d2;
	m[3 * 4 + 3] = d3;
}

ID_INLINE void idRenderMatrix::Identity()
{
	m[0 * 4 + 0] = 1.0f;
	m[0 * 4 + 1] = 0.0f;
	m[0 * 4 + 2] = 0.0f;
	m[0 * 4 + 3] = 0.0f;

	m[1 * 4 + 0] = 0.0f;
	m[1 * 4 + 1] = 1.0f;
	m[1 * 4 + 2] = 0.0f;
	m[1 * 4 + 3] = 0.0f;

	m[2 * 4 + 0] = 0.0f;
	m[2 * 4 + 1] = 0.0f;
	m[2 * 4 + 2] = 1.0f;
	m[2 * 4 + 3] = 0.0f;

	m[3 * 4 + 0] = 0.0f;
	m[3 * 4 + 1] = 0.0f;
	m[3 * 4 + 2] = 0.0f;
	m[3 * 4 + 3] = 1.0f;
}

ID_INLINE bool idRenderMatrix::IsZero( float epsilon ) const
{
	for( int i = 0; i < 16; i++ ) {
		if( idMath::Fabs( m[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idRenderMatrix::IsIdentity( float epsilon ) const
{
	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; j < 4; j++ ) {
			if( i == j ) {
				if( idMath::Fabs( m[i * 4 + j] - 1.0f ) > epsilon ) { return false; }
			} else {
				if( idMath::Fabs( m[i * 4 + j] ) > epsilon ) { return false; }
			}
		}
	}
	return true;
}

ID_INLINE bool idRenderMatrix::IsAffineTransform( float epsilon ) const
{
	if( idMath::Fabs( m[3 * 4 + 0] ) > epsilon || idMath::Fabs( m[3 * 4 + 1] ) > epsilon || idMath::Fabs( m[3 * 4 + 2] ) > epsilon || idMath::Fabs( m[3 * 4 + 3] - 1.0f ) > epsilon ) { return false; }
	return true;
}

ID_INLINE bool idRenderMatrix::IsUniformScale( float epsilon ) const
{
	float d0 = idMath::InvSqrt( m[0 * 4 + 0] * m[0 * 4 + 0] + m[1 * 4 + 0] * m[1 * 4 + 0] + m[2 * 4 + 0] * m[2 * 4 + 0] );
	float d1 = idMath::InvSqrt( m[0 * 4 + 1] * m[0 * 4 + 1] + m[1 * 4 + 1] * m[1 * 4 + 1] + m[2 * 4 + 1] * m[2 * 4 + 1] );
	float d2 = idMath::InvSqrt( m[0 * 4 + 2] * m[0 * 4 + 2] + m[1 * 4 + 2] * m[1 * 4 + 2] + m[2 * 4 + 2] * m[2 * 4 + 2] );
	if( idMath::Fabs( d0 - d1 ) > epsilon ) { return false; }
	if( idMath::Fabs( d1 - d2 ) > epsilon ) { return false; }
	if( idMath::Fabs( d0 - d2 ) > epsilon ) { return false; }
	return true;
}

ID_INLINE void idRenderMatrix::TransformPoint( const idVec3& in, idVec3& out ) const
{
	assert( in.ToFloatPtr() != out.ToFloatPtr() );
	const idRenderMatrix& matrix = *this;
	out[0]						 = in[0] * matrix[0][0] + in[1] * matrix[0][1] + in[2] * matrix[0][2] + matrix[0][3];
	out[1]						 = in[0] * matrix[1][0] + in[1] * matrix[1][1] + in[2] * matrix[1][2] + matrix[1][3];
	out[2]						 = in[0] * matrix[2][0] + in[1] * matrix[2][1] + in[2] * matrix[2][2] + matrix[2][3];
	assert( idMath::Fabs( in[0] * matrix[3][0] + in[1] * matrix[3][1] + in[2] * matrix[3][2] + matrix[3][3] - 1.0f ) < 0.01f );
}

ID_INLINE void idRenderMatrix::TransformPoint( const idVec3& in, idVec4& out ) const
{
	assert( in.ToFloatPtr() != out.ToFloatPtr() );
	const idRenderMatrix& matrix = *this;
	out[0]						 = in[0] * matrix[0][0] + in[1] * matrix[0][1] + in[2] * matrix[0][2] + matrix[0][3];
	out[1]						 = in[0] * matrix[1][0] + in[1] * matrix[1][1] + in[2] * matrix[1][2] + matrix[1][3];
	out[2]						 = in[0] * matrix[2][0] + in[1] * matrix[2][1] + in[2] * matrix[2][2] + matrix[2][3];
	out[3]						 = in[0] * matrix[3][0] + in[1] * matrix[3][1] + in[2] * matrix[3][2] + matrix[3][3];
}

ID_INLINE void idRenderMatrix::TransformPoint( const idVec4& in, idVec4& out ) const
{
	assert( in.ToFloatPtr() != out.ToFloatPtr() );
	const idRenderMatrix& matrix = *this;
	out[0]						 = in[0] * matrix[0][0] + in[1] * matrix[0][1] + in[2] * matrix[0][2] + in[3] * matrix[0][3];
	out[1]						 = in[0] * matrix[1][0] + in[1] * matrix[1][1] + in[2] * matrix[1][2] + in[3] * matrix[1][3];
	out[2]						 = in[0] * matrix[2][0] + in[1] * matrix[2][1] + in[2] * matrix[2][2] + in[3] * matrix[2][3];
	out[3]						 = in[0] * matrix[3][0] + in[1] * matrix[3][1] + in[2] * matrix[3][2] + in[3] * matrix[3][3];
}

ID_INLINE void idRenderMatrix::TransformDir( const idVec3& in, idVec3& out, bool normalize ) const
{
	const idRenderMatrix& matrix = *this;
	float				  p0	 = in[0] * matrix[0][0] + in[1] * matrix[0][1] + in[2] * matrix[0][2];
	float				  p1	 = in[0] * matrix[1][0] + in[1] * matrix[1][1] + in[2] * matrix[1][2];
	float				  p2	 = in[0] * matrix[2][0] + in[1] * matrix[2][1] + in[2] * matrix[2][2];
	if( normalize ) {
		float r = idMath::InvSqrt( p0 * p0 + p1 * p1 + p2 * p2 );
		p0 *= r;
		p1 *= r;
		p2 *= r;
	}
	out[0] = p0;
	out[1] = p1;
	out[2] = p2;
}

ID_INLINE void idRenderMatrix::TransformPlane( const idPlane& in, idPlane& out, bool normalize ) const
{
	assert( IsUniformScale( 0.01f ) );
	const idRenderMatrix& matrix = *this;
	float				  p0	 = in[0] * matrix[0][0] + in[1] * matrix[0][1] + in[2] * matrix[0][2];
	float				  p1	 = in[0] * matrix[1][0] + in[1] * matrix[1][1] + in[2] * matrix[1][2];
	float				  p2	 = in[0] * matrix[2][0] + in[1] * matrix[2][1] + in[2] * matrix[2][2];
	float				  d0	 = matrix[0][3] - p0 * in[3];
	float				  d1	 = matrix[1][3] - p1 * in[3];
	float				  d2	 = matrix[2][3] - p2 * in[3];
	if( normalize ) {
		float r = idMath::InvSqrt( p0 * p0 + p1 * p1 + p2 * p2 );
		p0 *= r;
		p1 *= r;
		p2 *= r;
	}
	out[0] = p0;
	out[1] = p1;
	out[2] = p2;
	out[3] = -p0 * d0 - p1 * d1 - p2 * d2;
}

ID_INLINE void idRenderMatrix::InverseTransformDir( const idVec3& in, idVec3& out, bool normalize ) const
{
	assert( in.ToFloatPtr() != out.ToFloatPtr() );
	const idRenderMatrix& matrix = *this;
	float				  p0	 = in[0] * matrix[0][0] + in[1] * matrix[1][0] + in[2] * matrix[2][0];
	float				  p1	 = in[0] * matrix[0][1] + in[1] * matrix[1][1] + in[2] * matrix[2][1];
	float				  p2	 = in[0] * matrix[0][2] + in[1] * matrix[1][2] + in[2] * matrix[2][2];
	if( normalize ) {
		float r = idMath::InvSqrt( p0 * p0 + p1 * p1 + p2 * p2 );
		p0 *= r;
		p1 *= r;
		p2 *= r;
	}
	out[0] = p0;
	out[1] = p1;
	out[2] = p2;
}

ID_INLINE void idRenderMatrix::InverseTransformPlane( const idPlane& in, idPlane& out, bool normalize ) const
{
	assert( in.ToFloatPtr() != out.ToFloatPtr() );
	const idRenderMatrix& matrix = *this;
	float				  p0	 = in[0] * matrix[0][0] + in[1] * matrix[1][0] + in[2] * matrix[2][0] + in[3] * matrix[3][0];
	float				  p1	 = in[0] * matrix[0][1] + in[1] * matrix[1][1] + in[2] * matrix[2][1] + in[3] * matrix[3][1];
	float				  p2	 = in[0] * matrix[0][2] + in[1] * matrix[1][2] + in[2] * matrix[2][2] + in[3] * matrix[3][2];
	float				  p3	 = in[0] * matrix[0][3] + in[1] * matrix[1][3] + in[2] * matrix[2][3] + in[3] * matrix[3][3];
	if( normalize ) {
		float r = idMath::InvSqrt( p0 * p0 + p1 * p1 + p2 * p2 );
		p0 *= r;
		p1 *= r;
		p2 *= r;
		p3 *= r;
	}
	out[0] = p0;
	out[1] = p1;
	out[2] = p2;
	out[3] = p3;
}

ID_INLINE void idRenderMatrix::TransformModelToClip( const idVec3& src, const idRenderMatrix& modelMatrix, const idRenderMatrix& projectionMatrix, idVec4& eye, idVec4& clip )
{
	for( int i = 0; i < 4; i++ ) {
		eye[i] = modelMatrix[i][0] * src[0] + modelMatrix[i][1] * src[1] + modelMatrix[i][2] * src[2] + modelMatrix[i][3];
	}
	for( int i = 0; i < 4; i++ ) {
		clip[i] = projectionMatrix[i][0] * eye[0] + projectionMatrix[i][1] * eye[1] + projectionMatrix[i][2] * eye[2] + projectionMatrix[i][3] * eye[3];
	}
}

ID_INLINE void idRenderMatrix::TransformClipToDevice( const idVec4& clip, idVec3& ndc )
{
	assert( idMath::Fabs( clip[3] ) > idMath::FLT_SMALLEST_NON_DENORMAL );
	float r = 1.0f / clip[3];
	ndc[0]	= clip[0] * r;
	ndc[1]	= clip[1] * r;
	ndc[2]	= clip[2] * r;
}

ID_INLINE void idRenderMatrix::ApplyDepthHack( idRenderMatrix& src )
{
	// scale projected z by 25%
	src.m[2 * 4 + 0] *= 0.25f;
	src.m[2 * 4 + 1] *= 0.25f;
	src.m[2 * 4 + 2] *= 0.25f;
	src.m[2 * 4 + 3] *= 0.25f;
}

ID_INLINE void idRenderMatrix::ApplyModelDepthHack( idRenderMatrix& src, float value )
{
	// offset projected z
	src.m[2 * 4 + 3] -= value;
}

ID_INLINE bool idRenderMatrix::CullPointToMVP( const idRenderMatrix& mvp, const idVec3& point, bool zeroToOne )
{
	byte bits;
	return CullPointToMVPbits( mvp, point, &bits, zeroToOne );
}

ID_INLINE bool idRenderMatrix::CullBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, bool zeroToOne )
{
	byte bits;
	return CullBoundsToMVPbits( mvp, bounds, &bits, zeroToOne );
}

ID_INLINE bool idRenderMatrix::CullExtrudedBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool zeroToOne )
{
	byte bits;
	return CullExtrudedBoundsToMVPbits( mvp, bounds, extrudeDirection, clipPlane, &bits, zeroToOne );
}

#endif // !__RENDERMATRIX_H__
