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

/*!
	\class idRenderMatrix
	\brief A 4x4 matrix class for rendering transformations and mathematical operations.

	This class provides a comprehensive implementation of 4x4 transformation matrices used in rendering operations. It supports initialization, mathematical operations like multiplication and
   inversion, and various transformation functions for points, vectors, and planes. The class is designed for use in graphics rendering pipelines, offering methods to create matrices from geometric
   components like origins, axes, and scales, as well as projection matrices for different coordinate system styles. It includes functionality for matrix transposition, determining matrix properties
   such as being identity or affine, and performing geometric operations like culling and projection. The implementation supports both inline and standard methods for performance-critical operations
   and provides utilities for working with bounding volumes and frustum calculations.

*/
class idRenderMatrix
{
public:
	//! Initializes an empty idRenderMatrix object.
	idRenderMatrix() { }

	/*!
		\brief Initializes a 4x4 transformation matrix with the specified float values for each cell.

		This constructor initializes a 4x4 matrix by setting each element of the matrix using the 16 float parameters. The matrix is stored in row-major order where m[row * 4 + column] = value.

		\param a0 The value for the element at row 0, column 0 of the matrix
		\param a1 The value for the element at row 0, column 1 of the matrix
		\param a2 The value for the element at row 0, column 2 of the matrix
		\param a3 The value for the element at row 0, column 3 of the matrix
		\param b0 The value for the element at row 1, column 0 of the matrix
		\param b1 The value for the element at row 1, column 1 of the matrix
		\param b2 The value for the element at row 1, column 2 of the matrix
		\param b3 The value for the element at row 1, column 3 of the matrix
		\param c0 The value for the element at row 2, column 0 of the matrix
		\param c1 The value for the element at row 2, column 1 of the matrix
		\param c2 The value for the element at row 2, column 2 of the matrix
		\param c3 The value for the element at row 2, column 3 of the matrix
		\param d0 The value for the element at row 3, column 0 of the matrix
		\param d1 The value for the element at row 3, column 1 of the matrix
		\param d2 The value for the element at row 3, column 2 of the matrix
		\param d3 The value for the element at row 3, column 3 of the matrix
	*/
	ID_INLINE	 idRenderMatrix( float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3, float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3 );

	//! Returns a pointer to the specified row of the matrix
	const float* operator[]( int index ) const
	{
		assert( index >= 0 && index < 4 );
		return &m[index * 4];
	}

	//! Returns a pointer to the specified row of the matrix
	float* operator[]( int index )
	{
		assert( index >= 0 && index < 4 );
		return &m[index * 4];
	}

	//! Sets all elements of the matrix to zero.
	void				  Zero() { memset( m, 0, sizeof( m ) ); }

	//! Sets the render matrix to the identity matrix.
	ID_INLINE void		  Identity();

	//! Determines if all elements of the matrix are zero within a specified epsilon tolerance.
	ID_INLINE bool		  IsZero( float epsilon ) const;

	//! Checks if the matrix is an identity matrix within the given epsilon tolerance.
	ID_INLINE bool		  IsIdentity( float epsilon ) const;

	//! Checks if the matrix represents an affine transformation within the given epsilon tolerance.
	ID_INLINE bool		  IsAffineTransform( float epsilon ) const;

	//! Checks if the matrix has a uniform scale factor within the given epsilon tolerance.
	ID_INLINE bool		  IsUniformScale( float epsilon ) const;

	//! Transforms a 3D point using the render matrix and stores the result in the output vector.
	ID_INLINE void		  TransformPoint( const idVec3& in, idVec3& out ) const;

	//! Transforms a 3D point using the matrix and stores the result in a 4D vector.
	ID_INLINE void		  TransformPoint( const idVec3& in, idVec4& out ) const;

	//! Transforms a 4D point using the render matrix.
	ID_INLINE void		  TransformPoint( const idVec4& in, idVec4& out ) const;

	//! Transforms a direction vector by the render matrix, optionally normalizing the result.
	ID_INLINE void		  TransformDir( const idVec3& in, idVec3& out, bool normalize ) const;

	//! Transforms a plane using the matrix and optionally normalizes the result
	ID_INLINE void		  TransformPlane( const idPlane& in, idPlane& out, bool normalize ) const;

	//! Transforms a direction vector by the inverse of this matrix, optionally normalizing the result.
	ID_INLINE void		  InverseTransformDir( const idVec3& in, idVec3& out, bool normalize ) const;

	//! Transforms a plane using the inverse of this matrix
	ID_INLINE void		  InverseTransformPlane( const idPlane& in, idPlane& out, bool normalize ) const;

	/*!
		\brief Transforms a 3D model space point into clip space using model and projection matrices.

		This function performs two matrix multiplications to transform a 3D point from model space to clip space. First, it transforms the input point from model space to eye space by multiplying the
	   point with the model matrix. Then, it transforms the resulting eye space point to clip space by multiplying with the projection matrix.

		\param src The input 3D point in model space
		\param modelMatrix The model matrix for transforming model space to eye space
		\param projectionMatrix The projection matrix for transforming eye space to clip space
		\param eye Output vector for the intermediate eye space coordinates
		\param clip Output vector for the final clip space coordinates
	*/
	static ID_INLINE void TransformModelToClip( const idVec3& src, const idRenderMatrix& modelMatrix, const idRenderMatrix& projectionMatrix, idVec4& eye, idVec4& clip );

	//! Transforms clip space coordinates to normalized device coordinates.
	static ID_INLINE void TransformClipToDevice( const idVec4& clip, idVec3& ndc );

	//! Creates a render matrix from an origin and axis
	static void			  CreateFromOriginAxis( const idVec3& origin, const idMat3& axis, idRenderMatrix& out );

	/*!
		\brief Creates a render matrix from origin, axis, and scale components.

		This function constructs a 4x4 transformation matrix using the provided origin position, orientation axis, and scale factors. The matrix is populated such that the first three columns
	   represent the scaled axis vectors, and the fourth column represents the translation origin. The last row is set to the standard homogeneous coordinate values [0, 0, 0, 1].

		\param origin The translation component of the transformation matrix
		\param axis The 3x3 orientation matrix defining the axis directions
		\param scale The scaling factors applied to each axis direction
		\param out The resulting render matrix that will contain the transformation
	*/
	static void			  CreateFromOriginAxisScale( const idVec3& origin, const idMat3& axis, const idVec3& scale, idRenderMatrix& out );

	//! Creates a view matrix from an origin and axis for OpenGL coordinate transformation.
	static void			  CreateViewMatrix( const idVec3& origin, const idMat3& axis, idRenderMatrix& out );

	/*!
		\brief Creates a projection matrix with the specified frustum parameters and stores the result in the output matrix

		This function generates a projection matrix for a perspective or orthographic projection based on the provided frustum parameters. The matrix is stored in the output parameter. The function
	   handles both infinite far plane cases and finite far plane cases, with different behavior based on the CLIP_SPACE_D3D definition. The input parameters define the boundaries of the viewing
	   frustum in world space coordinates.

		\param xMin Minimum x-coordinate of the viewing frustum
		\param xMax Maximum x-coordinate of the viewing frustum
		\param yMin Minimum y-coordinate of the viewing frustum
		\param yMax Maximum y-coordinate of the viewing frustum
		\param zNear Distance to the near clipping plane
		\param zFar Distance to the far clipping plane
		\param out Output matrix to store the resulting projection matrix
	*/
	static void			  CreateProjectionMatrix( float xMin, float xMax, float yMin, float yMax, float zNear, float zFar, idRenderMatrix& out );

	/*!
		\brief Creates a projection matrix using field of view parameters with optional offset

		This function generates a projection matrix based on the specified field of view angles for both X and Y axes, along with near and far clipping distances. The function also supports optional
	   offset values for the X and Y axes to adjust the projection. It calculates the frustum boundaries based on the field of view and then delegates to CreateProjectionMatrix to construct the final
	   matrix.

		\param xFovDegrees Horizontal field of view angle in degrees
		\param yFovDegrees Vertical field of view angle in degrees
		\param zNear Near clipping plane distance
		\param zFar Far clipping plane distance
		\param xOffset Horizontal offset for the projection frustum
		\param yOffset Vertical offset for the projection frustum
		\param out Output matrix that will contain the resulting projection matrix
	*/
	static void			  CreateProjectionMatrixFov( float xFovDegrees, float yFovDegrees, float zNear, float zFar, float xOffset, float yOffset, idRenderMatrix& out );

	/*!
		\brief Creates a D3D-style projection matrix with the specified field of view, aspect ratio, and depth range

		This function generates a projection matrix suitable for Direct3D-style rendering. It takes a vertical field of view angle, aspect ratio, and near and far depth clipping planes to construct
	   the matrix. The resulting matrix is stored in the output parameter and can be used for perspective projection in 3D graphics rendering

		\param verticalFov Vertical field of view angle in radians
		\param aspect Aspect ratio of the viewport (width/height)
		\param zNear Near clipping plane distance
		\param zFar Far clipping plane distance
		\param out Output matrix to store the resulting projection matrix
	*/
	static void			  CreateProjD3DStyle( float verticalFov, float aspect, float zNear, float zFar, idRenderMatrix& out );

	//! Applies a depth hack to the provided projection matrix by scaling its z-components by 25%.
	static ID_INLINE void ApplyDepthHack( idRenderMatrix& src );

	//! Applies a depth hack to the source render matrix by adjusting its projected z-coordinate.
	static ID_INLINE void ApplyModelDepthHack( idRenderMatrix& src, float value );

	//! Offsets and scales a matrix to transform the unit cube to exactly cover the given bounds
	static void			  OffsetScaleForBounds( const idRenderMatrix& src, const idBounds& bounds, idRenderMatrix& out );

	//! Computes a transformation matrix that offsets and inversely scales the input matrix based on the given bounds to map the bounds to a unit cube.
	static void			  InverseOffsetScaleForBounds( const idRenderMatrix& src, const idBounds& bounds, idRenderMatrix& out );

	//! Computes the transpose of a matrix and stores the result in the output matrix.
	static void			  Transpose( const idRenderMatrix& src, idRenderMatrix& out );

	//! Performs matrix multiplication of two 4x4 matrices and stores the result in a third matrix.
	static void			  Multiply( const idRenderMatrix& a, const idRenderMatrix& b, idRenderMatrix& out );

	//! Computes the inverse of a 4x4 transformation matrix used in rendering.
	static bool			  Inverse( const idRenderMatrix& src, idRenderMatrix& out );

	//! Computes the inverse transpose of an affine transformation matrix.
	static void			  InverseByTranspose( const idRenderMatrix& src, idRenderMatrix& out );

	//! Computes the inverse of a 4x4 matrix using double precision arithmetic and stores the result in the output matrix
	static bool			  InverseByDoubles( const idRenderMatrix& src, idRenderMatrix& out );

	/*!
		\brief Copies a render matrix into four row vectors with 16-byte alignment

		This function extracts the elements of a render matrix and copies them into four separate row vectors. The function uses SSE intrinsics when available to optimize memory operations, otherwise
	   falling back to standard element-by-element copying. All row vectors must be 16-byte aligned for optimal performance.

		\param matrix The source render matrix to copy from
		\param row0 First row vector to store the first four matrix elements
		\param row1 Second row vector to store the next four matrix elements
		\param row2 Third row vector to store the next four matrix elements
		\param row3 Fourth row vector to store the last four matrix elements
		\throws assertion failure if any of the row vector pointers are not 16-byte aligned
	*/
	static void			  CopyMatrix( const idRenderMatrix& matrix, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3 );

	/*!
		\brief Sets the MVP matrix elements into the provided row vectors and determines if the determinant is negative

		This function extracts the four rows of the provided MVP matrix and stores them into the corresponding row vectors. It also determines whether the determinant of the matrix is negative. The
	   function uses SSE intrinsics for optimized performance on supported platforms, with a fallback implementation for other platforms. All row vectors must be 16-byte aligned for proper SSE
	   operation.

		\param mvp The input MVP matrix to extract rows from
		\param row0 The first row of the matrix
		\param row1 The second row of the matrix
		\param row2 The third row of the matrix
		\param row3 The fourth row of the matrix
		\param negativeDeterminant Output parameter indicating whether the determinant of the matrix is negative
	*/
	static void			  SetMVP( const idRenderMatrix& mvp, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	/*!
		\brief Computes a modified model-view-projection matrix for a given bounding volume.

		This function calculates a transformed MVP matrix that accounts for the specified bounding volume's offset and scale. It processes the input matrix and bounds to produce four row vectors
	   representing the transformed matrix. The function also determines whether the determinant of the resulting matrix is negative, which can be useful for culling or other geometric operations. The
	   implementation uses SSE intrinsics for optimization when available, otherwise falling back to standard scalar operations.

		\param mvp The input model-view-projection matrix to be transformed
		\param bounds The bounding volume that defines the offset and scale for the transformation
		\param row0 Output vector representing the first row of the transformed matrix
		\param row1 Output vector representing the second row of the transformed matrix
		\param row2 Output vector representing the third row of the transformed matrix
		\param row3 Output vector representing the fourth row of the transformed matrix
		\param negativeDeterminant Output boolean indicating whether the determinant of the resulting matrix is negative
		\throws assertion failure if any of the row vectors are not 16-byte aligned
	*/
	static void			  SetMVPForBounds( const idRenderMatrix& mvp, const idBounds& bounds, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	/*!
		\brief Computes the matrix product of mvp and inverseProject and determines if the resulting matrix has a negative determinant.

		This function calculates the product of two 4x4 matrices, mvp and inverseProject, storing the result in four row vectors. It also evaluates the determinant of the resulting matrix to determine
	   if it is negative. The function uses SIMD intrinsics for optimized performance on supported platforms, falling back to standard scalar operations when SIMD is not available. All row vectors
	   must be 16-byte aligned for proper SIMD operation.

		\param mvp The first 4x4 matrix to be multiplied
		\param inverseProject The second 4x4 matrix to be multiplied
		\param row0 Output vector for the first row of the result matrix
		\param row1 Output vector for the second row of the result matrix
		\param row2 Output vector for the third row of the result matrix
		\param row3 Output vector for the fourth row of the result matrix
		\param negativeDeterminant Output boolean indicating if the determinant of the result matrix is negative
	*/
	static void			  SetMVPForInverseProject( const idRenderMatrix& mvp, const idRenderMatrix& inverseProject, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	//! Tests if a point is culled by the Model-View-Projection matrix.
	static bool			  CullPointToMVP( const idRenderMatrix& mvp, const idVec3& point, bool zeroToOne = false );

	/*!
		\brief Determines if a point is inside or outside the clip space defined by the model-view-projection matrix and returns culling bits

		This function transforms a 3D point using the provided model-view-projection matrix and performs frustum culling checks. It evaluates the transformed point against clip space boundaries and
	   returns a bit mask indicating which sides of the frustum the point is outside of. The function supports both traditional and D3D-style clip space conventions through the zeroToOne parameter.
	   The output bits are inverted (XOR with 63) so that a bit set to 1 indicates the point is outside the corresponding frustum plane, and a bit set to 0 indicates the point is inside or on the
	   plane.

		\param mvp The model-view-projection matrix used to transform the point
		\param point The 3D point to test against the clip space
		\param outBits Pointer to storage for the resulting culling bit mask
		\param zeroToOne Flag indicating whether clip space Z is in [0,1] range (D3D convention) or [-1,1] range (OpenGL convention)
		\return True if the point is completely inside the frustum, false if the point is outside one or more frustum planes
	*/
	static bool			  CullPointToMVPbits( const idRenderMatrix& mvp, const idVec3& point, byte* outBits, bool zeroToOne = false );

	//! Determines if a bounding box is culled by the projection matrix.
	static bool			  CullBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, bool zeroToOne = false );

	/*!
		\brief Determines if a bounding box is culled by the frustum using the Model-View-Projection matrix.

		This function performs frustum culling on a bounding box by transforming its eight corner points using the provided Model-View-Projection matrix and checking if all points are outside the clip
	   space boundaries. It supports both SIMD (SSE) and scalar implementations for performance optimization. The function returns true if the bounding box is partially or fully inside the view
	   frustum, and false if it's completely outside.

		\param mvp The Model-View-Projection matrix used to transform the bounding box coordinates
		\param bounds The bounding box to test against the frustum
		\param outBits Pointer to store the culling results as a bit mask where each bit represents a frustum plane test
		\param zeroToOne Flag indicating whether clip space values are in range [0,1] (true) or [-1,1] (false)
		\return True if the bounding box is partially or fully inside the view frustum, false if it's completely outside
	*/
	static bool			  CullBoundsToMVPbits( const idRenderMatrix& mvp, const idBounds& bounds, byte* outBits, bool zeroToOne = false );

	/*!
		\brief Determines if extruded bounds are culled against a clip plane using the Model-View-Projection matrix

		This function evaluates whether a bounding volume, when extruded in a specified direction, is culled by a given clipping plane. It uses the Model-View-Projection matrix to perform the culling
	   calculation. The extrusion direction and the clip plane are used to define the bounds that are tested for culling

		\param mvp The Model-View-Projection matrix used for the culling calculation
		\param bounds The bounding volume to be tested for culling
		\param extrudeDirection The direction in which the bounds are extruded for the culling test
		\param clipPlane The clipping plane against which the extruded bounds are tested
		\param zeroToOne If true, the depth range is treated as [0, 1] instead of [-1, 1]
		\return True if the extruded bounds are culled by the clip plane, false otherwise
	*/
	static bool			  CullExtrudedBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool zeroToOne = false );

	/*!
		\brief Performs conservative culling of an extruded bounding box against the frustum using MVP matrix bits

		This function determines whether an extruded bounding box, defined by a base bounding box and an extrusion direction, is culled against the view frustum. It uses the Model-View-Projection
	   matrix to transform the bounding box vertices and performs SIMD-based comparisons against the clip planes. The function calculates culling bits for the bounding box corners and incorporates
	   additional culling based on the extrusion direction and a clipping plane. The results are stored in the provided byte array, with an optional parameter to control the coordinate system
	   handling.

		\param mvp The Model-View-Projection matrix used for transforming bounding box vertices
		\param bounds The base bounding box to be extruded
		\param extrudeDirection The direction along which the bounding box is extruded
		\param clipPlane The clipping plane used for extrusion-based culling
		\param outBits Pointer to the output array where culling bits will be stored
		\param zeroToOne Flag indicating whether the coordinate system uses [0,1] or [-1,1] range for Z coordinates
		\return True if the extruded bounding box is completely outside the view frustum, false otherwise
		\throws Assertion failure if the extrusion direction and clip plane normal are parallel or nearly parallel
	*/
	static bool CullExtrudedBoundsToMVPbits( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, byte* outBits, bool zeroToOne = false );

	/*!
		\brief Calculates the projected bounding box of a given bounds using a model-view-projection matrix, with optional window space normalization.

		This function computes the projected bounds of a 3D bounding box by transforming it through a model-view-projection matrix. The computation is optimized using SSE intrinsics when available.
	   The function takes into account the four corners of the X-Y plane of the input bounds and applies the transformation to each corner to determine the minimum and maximum values of the projected
	   coordinates. If windowSpace is true, the resulting coordinates are normalized to the [0,1] range, which is typically used for screen-space operations. The function handles cases where the
	   projection might result in invalid values by clamping them to appropriate infinity values.

		\param projected The resulting bounds after projection
		\param mvp The model-view-projection matrix to use for transformation
		\param bounds The input bounding box to project
		\param windowSpace Whether to normalize the results to [0,1] window space
	*/
	static void ProjectedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Computes the projected and near-clipped bounds of a 3D bounding box using a model-view-projection matrix.

		This function calculates the bounds of a 3D bounding box after it has been projected using a model-view-projection matrix. It applies near-clipping to handle depth culling and can optionally
	   return the result in window space coordinates. The implementation uses SSE intrinsics for optimized computational performance.

		\param projected The resulting projected and near-clipped bounds
		\param mvp The model-view-projection matrix used for the projection
		\param bounds The original 3D bounding box to be projected
		\param windowSpace Whether to return results in window space coordinates (true) or normalized device coordinates (false)
	*/
	static void ProjectedNearClippedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Computes the projected and clipped bounds of a 3D bounding box using a model-view-projection matrix with SSE optimizations.

		This function takes a 3D bounding box and projects it using the provided model-view-projection matrix. It then clips the projected points against the unit cube to determine the final bounds.
	   The function uses SSE intrinsics for optimized computation. The projected bounds are stored in the provided idBounds object. The windowSpace parameter controls whether the final bounds are in
	   window space coordinates.

		\param projected The resulting projected and clipped bounds
		\param mvp The model-view-projection matrix used for projection
		\param bounds The 3D bounding box to be projected and clipped
		\param windowSpace Controls whether the result is in window space coordinates
	*/
	static void ProjectedFullyClippedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Calculates the minimum and maximum depth bounds for a given bounding box transformed by a model-view-projection matrix

		This function computes the depth bounds of a 3D bounding box after applying a model-view-projection transformation. It handles both SSE optimized and scalar implementations. The function
	   supports conversion to window space coordinates if specified. The computation takes into account perspective division and clamps results to valid ranges

		\param min Output parameter for the minimum depth bound
		\param max Output parameter for the maximum depth bound
		\param mvp The model-view-projection matrix used for transformation
		\param bounds The 3D bounding box to compute depth bounds for
		\param windowSpace Flag indicating whether to convert results to window space coordinates
	*/
	static void DepthBoundsForBounds( float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	/*!
		\brief Computes the minimum and maximum depth values for an extruded bounding box transformed by a model-view-projection matrix

		This function calculates the depth bounds of a bounding box that has been extruded along a given direction and then transformed by a model-view-projection matrix. The calculation takes into
	   account a clipping plane that affects the extrusion direction. The results are stored in the provided min and max float references. When windowSpace is true, the depth values are clamped to the
	   range [0,1] and scaled to fit within the normalized device coordinate system. The implementation uses SSE intrinsics for optimized performance.

		\param min Reference to store the minimum depth value
		\param max Reference to store the maximum depth value
		\param mvp Model-view-projection matrix used for transformation
		\param bounds Original bounding box to be extruded
		\param extrudeDirection Direction along which the bounding box is extruded
		\param clipPlane Clipping plane that influences the extrusion calculation
		\param windowSpace Whether to clamp and scale results to window space coordinates [0,1]
		\throws Assertion failure if the dot product of extrudeDirection and clipPlane normal is less than the smallest non-denormal float value
	*/
	static void DepthBoundsForExtrudedBounds(
		float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool windowSpace = true );

	/*!
		\brief Computes depth bounds for shadow mapping using SSE optimization based on the provided MVP matrix and bounding box

		This function calculates the minimum and maximum depth values for shadow mapping by transforming the bounding box vertices using the provided model-view-projection matrix. The computation
	   takes into account the light position and can optionally return values in window space. The function uses SSE intrinsics for optimized performance when processing the bounding box vertices and
	   their projections.

		\param min Output parameter for the minimum depth value
		\param max Output parameter for the maximum depth value
		\param mvp The model-view-projection matrix used for transforming bounding box vertices
		\param bounds The bounding box defining the object's spatial extent
		\param localLightOrigin The light position in local space coordinates
		\param windowSpace Flag indicating whether the result should be in window space (default true)
	*/
	static void			 DepthBoundsForShadowBounds( float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& localLightOrigin, bool windowSpace = true );

	/*!
		\brief Extracts the six frustum planes from a given transformation matrix

		This function computes the six frustum planes (left, right, bottom, top, near, far) from a transformation matrix that represents a view-projection or model-view-projection transformation. The
	   function handles both zero-to-one and negative-one-to-one depth coordinate systems, and can optionally normalize the plane equations. The implementation uses different methods for computing the
	   planes based on whether the depth coordinate system is zero-to-one or negative-one-to-one. The function is designed to work with matrices that are not necessarily MVP matrices, and includes a
	   FIXME comment indicating that more checks are needed to determine if the matrix is a D3D MVP type, especially when zeroToOne is false.

		\param planes Output array of six plane equations representing the frustum boundaries
		\param frustum Transformation matrix from which to extract the frustum planes
		\param zeroToOne Flag indicating whether the depth coordinate system uses zero-to-one range
		\param normalize Flag indicating whether to normalize the resulting plane equations
	*/
	static void			 GetFrustumPlanes( idPlane planes[6], const idRenderMatrix& frustum, bool zeroToOne, bool normalize );

	//! Computes the world-space corners of a frustum defined by the given transform and bounds.
	static void			 GetFrustumCorners( frustumCorners_t& corners, const idRenderMatrix& frustumTransform, const idBounds& frustumBounds );

	//! Determines the culling state of frustum corners relative to a given plane using SSE intrinsics or scalar computation.
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

/*
========================
idRenderMatrix::TransformPoint
========================
*/
ID_INLINE void idRenderMatrix::TransformPoint( const idVec3& in, idVec4& out ) const
{
	assert( in.ToFloatPtr() != out.ToFloatPtr() );
	const idRenderMatrix& matrix = *this;
	out[0]						 = in[0] * matrix[0][0] + in[1] * matrix[0][1] + in[2] * matrix[0][2] + matrix[0][3];
	out[1]						 = in[0] * matrix[1][0] + in[1] * matrix[1][1] + in[2] * matrix[1][2] + matrix[1][3];
	out[2]						 = in[0] * matrix[2][0] + in[1] * matrix[2][1] + in[2] * matrix[2][2] + matrix[2][3];
	out[3]						 = in[0] * matrix[3][0] + in[1] * matrix[3][1] + in[2] * matrix[3][2] + matrix[3][3];
}

/*
========================
idRenderMatrix::TransformPoint
========================
*/
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
