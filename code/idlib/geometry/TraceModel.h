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
	/*!
		\brief Initializes an idTraceModel object with default values.

		This constructor initializes all members of the idTraceModel class to their default states. The type is set to TRM_INVALID, indicating that the trace model is not valid. The vertex, edge, and
	   polygon counts are initialized to zero, and the bounding box is set to zero.

	*/
	idTraceModel();

	/*!
		\brief Initializes a trace model from an axial bounding box.

		Constructs a trace model by initializing a box shape and setting up its boundaries using the provided axial bounding box.

		\param boxBounds The axial bounding box used to define the dimensions of the trace model.
	*/
	idTraceModel( const idBounds& boxBounds );

	/*!
		\brief Constructs a cylinder approximation trace model from the given bounds and number of sides

		The constructor initializes a trace model that approximates a cylinder using the specified bounds and number of sides for the approximation

		\param cylBounds The bounds that define the cylinder's dimensions
		\param numSides The number of sides to use for the cylinder approximation
	*/
	idTraceModel( const idBounds& cylBounds, const int numSides );

	/*!
		\brief Initializes a trace model with specified length and width for bone-based collision detection.

		Constructs a trace model object using the provided length and width parameters. The constructor initializes the bone structure and sets up the bone dimensions accordingly. This is typically
	   used for creating collision models that represent objects with specific geometric properties.

		\param length The length dimension of the bone-based trace model
		\param width The width dimension of the bone-based trace model
	*/
	idTraceModel( const float length, const float width );

	/*!
		\brief Initializes a box trace model using the provided bounds.

		Sets up a box trace model by computing vertex positions and polygon distances based on the provided axis-aligned bounding box. The function calculates the center offset and populates the
	   vertices of the box. It also initializes the polygon data for each face of the box, including their distance from the origin and bounding volumes. The provided bounds are used to define the
	   dimensions and position of the box in 3D space.

		\param boxBounds Axis-aligned bounding box defining the dimensions and position of the box.
	*/
	void  SetupBox( const idBounds& boxBounds );

	/*!
		\brief Initializes a box trace model with the specified size

		This function sets up a box-shaped trace model by calculating the appropriate bounds based on the provided size parameter. It creates a box centered at the origin with dimensions defined by
	   the input size value. The function internally calls SetupBox with the calculated bounds to complete the initialization of the trace model.

		\param size The size of the box, used to determine the half-size for the bounding box bounds
	*/
	void  SetupBox( const float size );

	/*!
		\brief Initializes and sets up an octahedral trace model based on the provided bounding box.

		This function configures the trace model to represent an octahedron defined by the given bounding box. It calculates the vertices of the octahedron relative to the center of the bounding box
	   and sets up the polygon planes and normals. The function ensures the trace model is initialized as an octahedron type if it wasn't already. It also updates the model's bounding volume and
	   generates edge normals for proper collision detection.

		\param octBounds The bounding box that defines the shape and size of the octahedron
	*/
	void  SetupOctahedron( const idBounds& octBounds );

	/*!
		\brief Initializes the octahedron trace model with the specified size.

		Configures the octahedron trace model by calculating the half size and setting up the bounding box based on the provided size parameter. The function then delegates to the SetupOctahedron
	   method that takes an idBounds parameter to complete the initialization.

		\param size the total size of the octahedron, used to calculate the half size for the bounding box
	*/
	void  SetupOctahedron( const float size );

	/*!
		\brief Initializes and sets up a dodecahedron trace model based on the provided bounding box.

		This function configures a dodecahedron trace model using the specified bounding box coordinates. It computes vertex positions based on mathematical constants and scales them according to the
	   bounding box dimensions. The function also calculates polygon normals and bounds for accurate collision detection. If the trace model is not already of type dodecahedron, it initializes the
	   dodecahedron structure first.

		\param dodBounds The bounding box that defines the size and position of the dodecahedron
	*/
	void  SetupDodecahedron( const idBounds& dodBounds );

	/*!
		\brief Initializes a dodecahedron trace model with the specified size.

		This function sets up a dodecahedron trace model by calculating the bounds based on the provided size and then calling the bounds-based setup function. The dodecahedron is centered at the
	   origin with all vertices equidistant from the center.

		\param size The size of the dodecahedron, which determines the extent of its bounds
	*/
	void  SetupDodecahedron( const float size );

	/*!
		\brief Initializes a trace model to represent a cylinder approximation with the specified bounds and number of sides

		This function sets up a trace model to approximate a cylinder using a polyhedral representation. It takes the bounding box of the cylinder and the number of sides to use for the approximation.
	   The function calculates vertices, edges, and polygons to form the cylinder structure, ensuring the number of vertices, edges, and polygons stays within predefined limits. The cylinder is
	   centered around the midpoint of the input bounds, with the height determined by the z extents of the bounds.

		\param cylBounds The bounding box that defines the cylinder's dimensions and position
		\param numSides The number of sides to use for the cylinder approximation
	*/
	void  SetupCylinder( const idBounds& cylBounds, const int numSides );

	/*!
		\brief Initializes a cylindrical trace model with specified height, width, and number of sides.

		This function sets up a cylindrical trace model by calculating the bounding box based on the provided height and width, then delegates to another SetupCylinder function that uses the
	   calculated bounds and the number of sides to construct the cylinder.

		\param height The total height of the cylinder
		\param width The diameter of the cylinder
		\param numSides The number of sides for the cylindrical approximation
	*/
	void  SetupCylinder( const float height, const float width, const int numSides );

	/*!
		\brief Sets up a cone trace model using the specified bounds and number of sides.

		This function initializes a cone-shaped trace model by calculating vertex positions, edge connections, and polygon definitions based on the provided bounds and number of sides. The cone is
	   centered at the midpoint of the bounds, with the apex at the top and the base at the bottom. The function handles edge cases where the number of sides might exceed predefined limits for
	   vertices, edges, or polygons. It also computes polygon normals and distances for proper geometric representation.

		\param coneBounds The bounding box defining the cone's size and position
		\param numSides The number of sides to approximate the cone's circular base
	*/
	void  SetupCone( const idBounds& coneBounds, const int numSides );

	/*!
		\brief Initializes a conical trace model with specified height, width, and number of sides

		This function sets up a conical trace model by calculating the bounding box based on the provided height and width parameters, then calls the appropriate SetupCone function with the calculated
	   bounds and the specified number of sides

		\param height the vertical extent of the cone
		\param width the diameter of the cone's base
		\param numSides the number of sides for the conical approximation
	*/
	void  SetupCone( const float height, const float width, const int numSides );

	/*!
		\brief Sets up the bone trace model with specified length and width.

		This function initializes and configures the bone trace model by setting the vertex positions, bounding box, and polygon plane normals based on the provided length and width parameters. It
	   ensures that the model is properly structured for collision detection with a tetrahedral shape formed by two attached tetrahedrons.

		\param length The total length of the bone trace model
		\param width The width of the bone trace model
	*/
	void  SetupBone( const float length, const float width );

	/*!
		\brief Initializes a trace model from a polygon defined by a list of vertices.

		Sets up the trace model to represent a convex polygon by defining its vertices, edges, and bounding information. The polygon is converted into a volume with two opposing polygonal faces. The
	   function calculates edge normals and the center point of the polygon for use in collision detection. The number of vertices is limited to prevent exceeding the maximum trace model edges.

		\param v Pointer to an array of polygon vertices
		\param count Number of vertices in the polygon
	*/
	void  SetupPolygon( const idVec3* v, const int count );

	/*!
		\brief Initializes a polygon TraceModel from a winding object.

		This function takes a winding object and converts its points into a vertex array, then calls the SetupPolygon function with the vertex array and the number of points. The winding is expected
	   to represent a polygon that will be used for collision detection.

		\param w The winding object containing the polygon points
	*/
	void  SetupPolygon( const idWinding& w );

	/*!
		\brief Generates normals for edges in a trace model by averaging adjacent polygon normals and handling sharp edges.

		This function calculates edge normals for a trace model by processing all polygons and their edges. It initializes all edge normals to zero, then iterates through each polygon and its edges.
	   For each edge, if it doesn't already have a normal, it assigns the polygon's normal. If the edge already has a normal, it checks the dot product between the existing normal and the polygon's
	   normal. If the dot product is below a sharp edge threshold, it computes a new normal that accounts for the sharp edge, otherwise it averages the normals. The function returns the count of sharp
	   edges detected during processing.

		\return The number of sharp edges detected during the normal generation process
	*/
	int	  GenerateEdgeNormals();

	/*!
		\brief Translates all vertices, polygons, and bounds of the trace model by the given translation vector.

		This function updates the position of all vertices in the trace model by adding the translation vector to each vertex. It also adjusts the distances and bounds of all polygons in the model,
	   updating their normal distances and bounding boxes. Additionally, it updates the overall offset and bounds of the entire trace model to reflect the translation.

		\param translation The vector by which to translate all elements of the trace model
	*/
	void  Translate( const idVec3& translation );

	/*!
		\brief Rotates the vertices and polygons of the trace model by the provided rotation matrix.

		This function applies a 3x3 rotation matrix to all vertices of the trace model, updating the bounds of each polygon and recalculating edge normals. It ensures that the geometric representation
	   of the trace model is properly transformed according to the given rotation.

		\param rotation The rotation matrix to apply to the trace model vertices and polygons
	*/
	void  Rotate( const idMat3& rotation );

	/*!
		\brief Shrinks the trace model by a specified distance on all sides

		This function reduces the size of a trace model by moving its vertices inward by the specified distance. For polygonal models, it adjusts the vertices of each edge by moving them along the
	   edge direction. For other types of models, it moves the vertices of each polygon's edges inward along the polygon normal. The function handles both polygonal and non-polygonal trace models
	   differently to ensure proper shrinking behavior.

		\param m The distance to shrink the model by on all sides
	*/
	void  Shrink( const float m );

	/*!
		\brief Compares this trace model with another trace model for equality.

		This function checks if the current trace model is identical to the provided trace model. It compares basic properties such as type, number of vertices, edges, and polygons, as well as bounds
	   and offset. For certain trace model types that contain vertex data, it also compares the actual vertex positions.

		\param trm The trace model to compare against
		\return True if the trace models are identical, false otherwise.
	*/
	bool  Compare( const idTraceModel& trm ) const;
	bool  operator==( const idTraceModel& trm ) const;
	bool  operator!=( const idTraceModel& trm ) const;

	/*!
		\brief Calculates and returns the area of a specified polygon within the trace model.

		This function computes the area of a polygon identified by its index within the trace model. It uses the vertices of the polygon's edges to calculate the cross product for each triangle formed
	   by the base vertex and two consecutive vertices. The sum of these cross product magnitudes is then halved to obtain the total polygon area. The function performs bounds checking to ensure the
	   polygon index is valid.

		\param polyNum Index of the polygon for which to calculate the area
		\return The area of the specified polygon, or 0.0 if the polygon index is out of bounds
	*/
	float GetPolygonArea( int polyNum ) const;

	/*!
		\brief Computes and returns the silhouette edges of the trace model based on a projection origin

		This function calculates the silhouette edges of the trace model by projecting from a given origin point onto the model's polygons. It identifies which edges are part of the silhouette by
	   checking the dot product of the edge direction and the polygon normal. The function uses a bit manipulation technique to mark silhouette edges and then orders them for return.

		\param projectionOrigin The origin point from which the projection is calculated
		\param silEdges Output array to store the silhouette edge indices
		\return The number of silhouette edges found and stored in the silEdges array
	*/
	int	  GetProjectionSilhouetteEdges( const idVec3& projectionOrigin, int silEdges[MAX_TRACEMODEL_EDGES] ) const;

	/*!
		\brief Calculates and returns the silhouette edges of the trace model for a given projection direction

		This function determines the edges of the trace model that form the silhouette when projected along the specified direction. It processes all polygons in the trace model and identifies edges
	   that are part of the silhouette by checking the dot product of the polygon normal with the projection direction. The result is stored in the provided array of silhouette edges and the function
	   returns the number of silhouette edges found.

		\param projectionDir The direction vector used for calculating the silhouette edges
		\param silEdges Output array to store the silhouette edges
		\return The number of silhouette edges found and stored in the silEdges array
	*/
	int	  GetParallelProjectionSilhouetteEdges( const idVec3& projectionDir, int silEdges[MAX_TRACEMODEL_EDGES] ) const;

	/*!
		\brief Calculates the mass properties including mass, center of mass, and inertia tensor for the trace model based on a given density

		This function computes the physical mass properties of a trace model using volume integrals derived from the model's geometry. It handles both polygonal and non-polygonal trace models by
	   delegating to appropriate internal functions. For polygonal models, it first converts the model to a volume representation before calculating the properties. For non-polygonal models, it
	   directly computes the volume integrals using the model's existing data. The function ensures numerical stability by handling cases with zero volume by setting default values for mass, center of
	   mass, and inertia tensor. The computed inertia tensor is translated to the center of mass to ensure accurate physical simulation.

		\param density The density value used to compute the mass properties
		\param mass Output parameter for the calculated mass of the model
		\param centerOfMass Output parameter for the calculated center of mass of the model
		\param inertiaTensor Output parameter for the calculated inertia tensor of the model
	*/
	void  GetMassProperties( const float density, float& mass, idVec3& centerOfMass, idMat3& inertiaTensor ) const;

private:
	/*!
		\brief Initializes the trace model as a box with vertices, edges, and polygons.

		This function sets up the internal data structures for a box-shaped trace model. It initializes the type to TRM_BOX, defines the number of vertices (8), edges (12), and polygons (6). The
	   function populates the edges array with connections between vertices that form the edges of a box. It then defines the six polygon faces of the box, each with four edges and a normal vector.
	   The polygons are oriented such that their edges follow a counter-clockwise direction when viewed from the outside. Finally, it marks the model as convex and calculates the edge normals.

	*/
	void InitBox();

	/*!
		\brief Initializes the octahedron trace model by setting up its vertices, edges, and polygons.

		This function configures the internal data structures of the trace model to represent a regular octahedron. It sets the number of vertices, edges, and polygons, and defines the connectivity of
	   edges and polygons. The octahedron is represented as a convex polyhedron with 6 vertices, 12 edges, and 8 triangular faces. The function also marks the model as convex.

	*/
	void InitOctahedron();

	/*!
		\brief Initializes the dodecahedron trace model with vertices, edges, and polygons.

		This function sets up the internal data structures for a dodecahedron trace model. It initializes the model type to TRM_DODECAHEDRON and sets the number of vertices, edges, and polygons to
	   their respective dodecahedron values. The function then populates the edges array with vertex indices defining the edges of the dodecahedron, and the polygons array with edge indices defining
	   the faces of the dodecahedron. All edges of a polygon are oriented counter-clockwise. The model is marked as convex.

	*/
	void InitDodecahedron();

	/*!
		\brief Initializes the bone trace model with vertex, edge, and polygon data for a skeletal structure.

		This function sets up the internal data structures for a bone trace model used in collision detection. It defines the number of vertices, edges, and polygons that make up the bone structure.
	   The bone is represented as a convex polyhedron with 5 vertices, 9 edges, and 6 triangular polygons. The function initializes the edges and polygons that define the shape of the bone in 3D
	   space.

	*/
	void InitBone();

	/*!
		\brief Computes projection integrals for a polygon edge in a trace model

		This function calculates various projection integrals for a given polygon and edge indices. It iterates through all edges of the specified polygon and computes mathematical integrals involving
	   coordinates along the specified axes. The results are accumulated in the provided integrals structure. The computation involves polynomial terms and cross-products of coordinates to calculate
	   the integrals for different powers and combinations. The final results are scaled by specific coefficients to produce the correct integral values.

		\param polyNum index of the polygon in the trace model
		\param a first axis index for coordinate calculations
		\param b second axis index for coordinate calculations
		\param integrals output structure to store the computed projection integrals
	*/
	void ProjectionIntegrals( int polyNum, int a, int b, struct projectionIntegrals_s& integrals ) const;

	/*!
		\brief Computes polygon integrals for a given triangle face of a trace model

		This function calculates various polygon integrals used in physics and rendering calculations for a triangular face of a trace model. It takes the polygon number and three vertex indices
	   defining a triangle, then computes integral values based on the polygon's normal and distance from origin. The calculations involve projection integrals and use the normal vector components to
	   compute coefficients for the final integral values.

		\param polyNum Index of the polygon in the trace model
		\param a First vertex index of the triangle
		\param b Second vertex index of the triangle
		\param c Third vertex index of the triangle
		\param integrals Output structure to store the computed polygon integrals
	*/
	void PolygonIntegrals( int polyNum, int a, int b, int c, struct polygonIntegrals_s& integrals ) const;

	/*!
		\brief Computes volume integrals for the trace model using polygon data and normal vectors

		This function calculates various volume integrals for a trace model by iterating through all polygons and accumulating contributions from each polygon's normal vector components. The
	   calculations involve computing polygon integrals for each face and combining them according to the face's orientation relative to the coordinate axes. The results are stored in the provided
	   integrals structure, which contains several different integral types (T0, T1, T2, TP) that represent different mathematical computations of the volume.

		\param integrals Output structure that will contain the computed volume integrals
	*/
	void VolumeIntegrals( struct volumeIntegrals_s& integrals ) const;

	/*!
		\brief Creates a polygon volume from the current polygon by extruding it along its normal direction with the specified thickness.

		This function generates a volume representation of a polygon by extruding it along the normal direction of the original polygon. The extrusion depth is determined by the thickness parameter.
	   The resulting trace model contains a polygon volume with additional vertices and edges to represent the extruded shape. The function modifies the provided trace model to contain the new volume
	   geometry.

		\param trm Output trace model that will contain the polygon volume representation
		\param thickness Distance to extrude the polygon along its normal direction
	*/
	void VolumeFromPolygon( idTraceModel& trm, float thickness ) const;

	/*!
		\brief Orders silhouette edges from an unsorted list based on vertex connectivity

		This function takes an array indicating which edges are silhouette edges and produces a sorted array of those edges. The sorting is done based on the connectivity of vertices in the
	   silhouette, where each subsequent edge shares a vertex with the previous one in the sequence. The function processes the edges to form a continuous chain from the first silhouette edge,
	   ensuring proper ordering. The signed edge numbers are used to indicate the direction in which the edge should be traversed.

		\param edgeIsSilEdge An array indicating which edges are silhouette edges, with indices from 1 to numEdges
		\param silEdges Output array that will contain the ordered silhouette edges
		\return The total number of silhouette edges that were ordered
	*/
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
