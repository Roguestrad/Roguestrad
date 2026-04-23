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

#ifndef __GLMATRIX_H__
#define __GLMATRIX_H__

//! Converts an axis and origin into a 4x4 model matrix.
void R_AxisToModelMatrix( const idMat3& axis, const idVec3& origin, float modelMatrix[16] );

//! Transposes a 4x4 matrix stored as a flat array of 16 floats.
void R_MatrixTranspose( const float in[16], float out[16] );
void R_MatrixMultiply( const float* a, const float* b, float* out );

/*!
	\brief Transforms a 3D model space point into clip space using model and projection matrices

	This function performs a two-step transformation process. First, it transforms the input 3D point from model space to eye space using the provided model matrix. Then, it transforms the resulting
   eye space point to clip space using the projection matrix. The function operates on 4-component vectors where the fourth component is implicitly set to 1.0 for the input point.

	\param src The input 3D point in model space coordinates
	\param modelMatrix A 4x4 matrix that transforms model space to eye space
	\param projectionMatrix A 4x4 matrix that transforms eye space to clip space
	\param eye Intermediate eye space coordinates (output parameter)
	\param dst Final clip space coordinates (output parameter)
*/
void R_TransformModelToClip( const idVec3& src, const float* modelMatrix, const float* projectionMatrix, idPlane& eye, idPlane& dst );

//! Transforms a clip-space plane to normalized device coordinates.
void R_TransformClipToDevice( const idPlane& clip, idVec3& ndc );

//! Converts global world coordinates to normalized device coordinates using the current view and projection matrices.
void R_GlobalToNormalizedDeviceCoordinates( const idVec3& global, idVec3& ndc );

//! Converts a global point to local coordinates using the provided model matrix
void R_GlobalPointToLocal( const float modelMatrix[16], const idVec3& in, idVec3& out );

//! Transforms a local point to global space using the provided model matrix.
void R_LocalPointToGlobal( const float modelMatrix[16], const idVec3& in, idVec3& out );

//! Converts a vector from global space to local space using a model matrix.
void R_GlobalVectorToLocal( const float modelMatrix[16], const idVec3& in, idVec3& out );

//! Transforms a local vector to global space using a model matrix.
void R_LocalVectorToGlobal( const float modelMatrix[16], const idVec3& in, idVec3& out );

//! Converts a global plane to local space using the provided model matrix.
void R_GlobalPlaneToLocal( const float modelMatrix[16], const idPlane& in, idPlane& out );

//! Transforms a local plane into global space using the provided model matrix
void R_LocalPlaneToGlobal( const float modelMatrix[16], const idPlane& in, idPlane& out );

//! Sets up the world to view matrix for a given view definition and stereo origin.
void R_SetupViewMatrix( viewDef_t* viewDef, stereoOrigin_t so );

//! Sets up the projection matrix for a view definition with optional stereo eye handling and jittering.
void R_SetupProjectionMatrix( viewDef_t* viewDef, bool doJitter, const int stereoEye );

//! Initializes unprojection matrices for the specified view definition.
void R_SetupUnprojection( viewDef_t* viewDef );

/*!
	\brief Sets up a projection matrix for rendering based on view definition and near/far clipping distances

	This function calculates and populates a 4x4 projection matrix for OpenGL rendering using the provided view definition, near and far clipping distances. It takes into account field of view
   parameters, viewport dimensions, and jittering for antialiasing. The matrix is constructed to handle stereo rendering through the stereoScreenSeparation parameter and supports flipping the
   projection for special rendering effects. The function operates on a pre-allocated 16-element float array that will contain the resulting matrix

	\param viewDef Pointer to the view definition containing rendering parameters and viewport information
	\param zNear Near clipping distance for the projection
	\param zFar Far clipping distance for the projection
	\param out Output array that will contain the 4x4 projection matrix (16 floats)
*/
void R_SetupProjectionMatrix2( const viewDef_t* viewDef, const float zNear, const float zFar, float out[16] );

//! Computes the inverse of a 4x4 matrix and stores the result in the output matrix
void R_MatrixFullInverse( const float in[16], float r[16] );

//! Applies oblique projection adjustment to the given view definition using the first clip plane.
void R_ObliqueProjection( viewDef_t* viewDef );

#endif /* !__GLMATRIX_H__ */
