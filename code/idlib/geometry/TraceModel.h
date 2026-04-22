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

#ifndef __TRACEMODEL_H__
#define __TRACEMODEL_H__

/*
===============================================================================

	A trace model is an arbitrary polygonal model which is used by the
	collision detection system to find collisions, contacts or the contents
	of a volume. For collision detection speed reasons the number of vertices
	and edges are limited. The trace model can have any shape. However convex
	models are usually preferred.

===============================================================================
*/

class idVec3;
class idMat3;
class idBounds;

// trace model type
typedef enum {
	TRM_INVALID,	   // invalid trm
	TRM_BOX,		   // box
	TRM_OCTAHEDRON,	   // octahedron
	TRM_DODECAHEDRON,  // dodecahedron
	TRM_CYLINDER,	   // cylinder approximation
	TRM_CONE,		   // cone approximation
	TRM_BONE,		   // two tetrahedrons attached to each other
	TRM_POLYGON,	   // arbitrary convex polygon
	TRM_POLYGONVOLUME, // volume for arbitrary convex polygon
	TRM_CUSTOM		   // loaded from map model or ASE/LWO
} traceModel_t;

// these are bit cache limits
#define MAX_TRACEMODEL_VERTS	 64
#define MAX_TRACEMODEL_EDGES	 64
#define MAX_TRACEMODEL_POLYS	 32
#define MAX_TRACEMODEL_POLYEDGES 32

typedef idVec3 traceModelVert_t;

typedef struct {
	int	   v[2];
	idVec3 normal;
} traceModelEdge_t;

typedef struct {
	idVec3	 normal;
	float	 dist;
	idBounds bounds;
	int		 numEdges;
	int		 edges[MAX_TRACEMODEL_POLYEDGES];
} traceModelPoly_t;

/*!
	\class idTraceModel
	\brief A trace model class that represents geometric shapes for collision detection and physics simulations.

	This class provides functionality to create and manipulate various geometric trace models including boxes, octahedrons, dodecahedrons, cylinders, cones, and bone-shaped forms. The models can be
   initialized from bounding boxes, sizes, or vertex data, and support transformations such as translation, rotation, and shrinking. Methods are available to compute mass properties, generate edge
   normals, and calculate projection integrals for physics computations. The class supports both explicit construction and initialization of different geometric forms, enabling efficient collision
   detection and simulation scenarios.

*/
class idTraceModel
{
public:
	traceModel_t	 type;
	int				 numVerts;
	traceModelVert_t verts[MAX_TRACEMODEL_VERTS];
	int				 numEdges;
	traceModelEdge_t edges[MAX_TRACEMODEL_EDGES + 1];
	int				 numPolys;
	traceModelPoly_t polys[MAX_TRACEMODEL_POLYS];
	idVec3			 offset;   // offset to center of model
	idBounds		 bounds;   // bounds of model
	bool			 isConvex; // true when model is convex

public:
	//! Constructs an invalid trace model with zeroed dimensions and bounds.
	idTraceModel();

	//! Initializes a trace model from an axial bounding box.
	idTraceModel( const idBounds& boxBounds );

	//! Constructs a cylinder approximation trace model from the given bounds and number of sides.
	idTraceModel( const idBounds& cylBounds, const int numSides );

	//! Initializes a bone-shaped trace model with the specified length and width.
	idTraceModel( const float length, const float width );

	//! Initializes or updates the trace model to represent a box defined by the given bounds.
	void  SetupBox( const idBounds& boxBounds );

	//! Initializes a box trace model with the specified size centered at the origin.
	void  SetupBox( const float size );

	//! Initializes and sets up an octahedron trace model using the provided bounding box.
	void  SetupOctahedron( const idBounds& octBounds );

	//! Initializes the octahedron trace model with the specified size centered at the origin.
	void  SetupOctahedron( const float size );

	//! Initializes a dodecahedron trace model using the specified bounding box.
	void  SetupDodecahedron( const idBounds& dodBounds );

	//! Initializes a dodecahedron trace model with the specified size.
	void  SetupDodecahedron( const float size );

	//! Initializes a cylindrical trace model using the specified bounds and number of sides
	void  SetupCylinder( const idBounds& cylBounds, const int numSides );

	//! Initializes the trace model as a cylinder with specified dimensions and side count.
	void  SetupCylinder( const float height, const float width, const int numSides );

	//! Sets up a cone trace model using the specified bounds and number of sides.
	void  SetupCone( const idBounds& coneBounds, const int numSides );

	//! Sets up a cone trace model with the specified height, width, and number of sides.
	void  SetupCone( const float height, const float width, const int numSides );

	//! Initializes a bone-shaped trace model with specified length and width
	void  SetupBone( const float length, const float width );

	//! Initializes a trace model as a convex polygon from a set of vertices.
	void  SetupPolygon( const idVec3* v, const int count );

	//! Sets up a polygon for tracing using the provided winding.
	void  SetupPolygon( const idWinding& w );

	//! Generates edge normals for the trace model.
	int	  GenerateEdgeNormals();

	//! Translates the trace model by the specified vector.
	void  Translate( const idVec3& translation );

	//! Rotates the trace model by the specified rotation matrix.
	void  Rotate( const idMat3& rotation );

	//! Shrinks the trace model by the specified amount on all sides.
	void  Shrink( const float m );

	//! Compare this trace model with another trace model for equality.
	bool  Compare( const idTraceModel& trm ) const;

	//! Compares this trace model with another for equality.
	bool  operator==( const idTraceModel& trm ) const;

	//! Returns true if this trace model is not equal to the given trace model.
	bool  operator!=( const idTraceModel& trm ) const;

	//! Returns the area of a specified polygon in the trace model.
	float GetPolygonArea( int polyNum ) const;

	//! Returns the silhouette edges of the trace model projected from a given origin.
	int	  GetProjectionSilhouetteEdges( const idVec3& projectionOrigin, int silEdges[MAX_TRACEMODEL_EDGES] ) const;

	//! Returns the silhouette edges of the trace model for a parallel projection direction.
	int	  GetParallelProjectionSilhouetteEdges( const idVec3& projectionDir, int silEdges[MAX_TRACEMODEL_EDGES] ) const;

	//! Computes mass properties including mass, center of mass, and inertia tensor for a trace model given a density
	void  GetMassProperties( const float density, float& mass, idVec3& centerOfMass, idMat3& inertiaTensor ) const;

private:
	//! Initializes a box trace model with vertices, edges, and polygons.
	void InitBox();

	//! Initializes the octahedron trace model with its geometric properties and topology.
	void InitOctahedron();

	//! Initializes the dodecahedron trace model with its geometric properties and topology.
	void InitDodecahedron();

	//! Initializes the bone trace model with predefined vertices, edges, and polygons.
	void InitBone();

	//! Computes projection integrals for a polygon edge in the trace model
	void ProjectionIntegrals( int polyNum, int a, int b, struct projectionIntegrals_s& integrals ) const;

	//! Computes polygon integrals for a given polygon and indices.
	void PolygonIntegrals( int polyNum, int a, int b, int c, struct polygonIntegrals_s& integrals ) const;

	//! Computes volume integrals for the trace model using polygon data and normal vectors
	void VolumeIntegrals( struct volumeIntegrals_s& integrals ) const;

	//! Creates a polygon volume from the current polygon by extruding it along its normal by the specified thickness.
	void VolumeFromPolygon( idTraceModel& trm, float thickness ) const;

	//! Returns the number of silhouette edges for the trace model.
	int	 GetOrderedSilhouetteEdges( const int edgeIsSilEdge[MAX_TRACEMODEL_EDGES + 1], int silEdges[MAX_TRACEMODEL_EDGES] ) const;
};

ID_INLINE idTraceModel::idTraceModel()
{
	type	 = TRM_INVALID;
	numVerts = numEdges = numPolys = 0;
	bounds.Zero();
}

ID_INLINE idTraceModel::idTraceModel( const idBounds& boxBounds )
{
	InitBox();
	SetupBox( boxBounds );
}

ID_INLINE idTraceModel::idTraceModel( const idBounds& cylBounds, const int numSides )
{
	SetupCylinder( cylBounds, numSides );
}

ID_INLINE idTraceModel::idTraceModel( const float length, const float width )
{
	InitBone();
	SetupBone( length, width );
}

ID_INLINE bool idTraceModel::operator==( const idTraceModel& trm ) const
{
	return Compare( trm );
}

ID_INLINE bool idTraceModel::operator!=( const idTraceModel& trm ) const
{
	return !Compare( trm );
}

#endif /* !__TRACEMODEL_H__ */
