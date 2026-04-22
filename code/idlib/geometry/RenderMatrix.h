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

	//! Initializes a 4x4 transformation matrix with the specified float values for each cell.
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

	//! Projects a 3D model space point into clip space using model and projection matrices.
	static ID_INLINE void TransformModelToClip( const idVec3& src, const idRenderMatrix& modelMatrix, const idRenderMatrix& projectionMatrix, idVec4& eye, idVec4& clip );

	//! Transforms clip space coordinates to normalized device coordinates.
	static ID_INLINE void TransformClipToDevice( const idVec4& clip, idVec3& ndc );

	//! Creates a render matrix from an origin and axis
	static void			  CreateFromOriginAxis( const idVec3& origin, const idMat3& axis, idRenderMatrix& out );

	//! Creates a render matrix from origin, axis, and scale components.
	static void			  CreateFromOriginAxisScale( const idVec3& origin, const idMat3& axis, const idVec3& scale, idRenderMatrix& out );

	//! Creates a view matrix from an origin and axis for OpenGL coordinate transformation.
	static void			  CreateViewMatrix( const idVec3& origin, const idMat3& axis, idRenderMatrix& out );

	//! Creates a projection matrix with the specified frustum parameters and stores the result in the output matrix.
	static void			  CreateProjectionMatrix( float xMin, float xMax, float yMin, float yMax, float zNear, float zFar, idRenderMatrix& out );

	//! Creates a projection matrix using field of view parameters with optional offset.
	static void			  CreateProjectionMatrixFov( float xFovDegrees, float yFovDegrees, float zNear, float zFar, float xOffset, float yOffset, idRenderMatrix& out );

	//! Creates a D3D-style projection matrix with the specified field of view, aspect ratio, and depth range.
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

	//! Copies a render matrix into four row vectors, aligned for uniform setting.
	static void			  CopyMatrix( const idRenderMatrix& matrix, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3 );

	//! Sets the MVP matrix elements into the provided row vectors and determines if the determinant is negative.
	static void			  SetMVP( const idRenderMatrix& mvp, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	//! Computes a modified model-view-projection matrix for a given bounding volume.
	static void			  SetMVPForBounds( const idRenderMatrix& mvp, const idBounds& bounds, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	//! Sets the MVP for inverse project by computing the matrix product of mvp and inverseProject and determining if the result has a negative determinant.
	static void			  SetMVPForInverseProject( const idRenderMatrix& mvp, const idRenderMatrix& inverseProject, idVec4& row0, idVec4& row1, idVec4& row2, idVec4& row3, bool& negativeDeterminant );

	//! Tests if a point is culled by the Model-View-Projection matrix.
	static bool			  CullPointToMVP( const idRenderMatrix& mvp, const idVec3& point, bool zeroToOne = false );

	//! Determines if a point is inside or outside the clip space defined by the model-view-projection matrix and returns culling bits.
	static bool			  CullPointToMVPbits( const idRenderMatrix& mvp, const idVec3& point, byte* outBits, bool zeroToOne = false );

	//! Determines if a bounding box is culled by the projection matrix.
	static bool			  CullBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, bool zeroToOne = false );

	//! Determines if a bounding box is culled by the frustum using the Model-View-Projection matrix.
	static bool			  CullBoundsToMVPbits( const idRenderMatrix& mvp, const idBounds& bounds, byte* outBits, bool zeroToOne = false );

	//! Determines if extruded bounds are culled against a clip plane using the Model-View-Projection matrix.
	static bool			  CullExtrudedBoundsToMVP( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool zeroToOne = false );

	//! Performs conservative culling of an extruded bounding box against the frustum using MVP matrix bits
	static bool CullExtrudedBoundsToMVPbits( const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, byte* outBits, bool zeroToOne = false );

	//! Calculates the projected bounds of a given bounding box using a model-view-projection matrix
	static void ProjectedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	//! Computes the projected and near-clipped bounds of a 3D bounding box using a model-view-projection matrix
	static void ProjectedNearClippedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	//! Computes the projected and clipped bounds of a 3D bounding box using a model-view-projection matrix
	static void ProjectedFullyClippedBounds( idBounds& projected, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	//! Calculates the minimum and maximum depth bounds for a given bounding box transformed by a model-view-projection matrix.
	static void DepthBoundsForBounds( float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, bool windowSpace = true );

	//! Computes the minimum and maximum depth values for an extruded bounding box transformed by a model-view-projection matrix
	static void DepthBoundsForExtrudedBounds(
		float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& extrudeDirection, const idPlane& clipPlane, bool windowSpace = true );

	//! Computes the depth bounds for shadow mapping using SSE optimization based on the provided MVP matrix and bounding box
	static void			 DepthBoundsForShadowBounds( float& min, float& max, const idRenderMatrix& mvp, const idBounds& bounds, const idVec3& localLightOrigin, bool windowSpace = true );

	//! Extracts the six frustum planes from a given transformation matrix
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
