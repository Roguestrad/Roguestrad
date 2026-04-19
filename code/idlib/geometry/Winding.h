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

#ifndef __WINDING_H__
#define __WINDING_H__

/*
===============================================================================

	A winding is an arbitrary convex polygon defined by an array of points.

===============================================================================
*/

class idWinding
{
public:
	/*!
		\brief Default constructor for the idWinding class that initializes internal state.

		This constructor initializes an idWinding object with no allocated points. It sets the number of points and allocated size to zero, and the point pointer to NULL. This is typically used to
	   create an empty winding object that can later be populated with points using other methods.

	*/
	idWinding();

	/*!
		\brief Constructs a winding object with space allocated for the specified number of points

		This constructor initializes a winding object by allocating memory for a specified number of points. The winding is initialized with zero points and the allocated size set to the provided
	   value. The actual point data is not populated until points are added later. This constructor is typically used when the number of points in the winding is known in advance, allowing for
	   efficient memory allocation.

		\param n The number of points to allocate space for in the winding
	*/
	explicit idWinding( const int n );

	/*!
		\brief Constructs a winding from an array of vertices

		Initializes a winding object by copying vertex data from a provided array. The function allocates memory for the winding points and copies the vertex coordinates, setting texture coordinates
	   to zero. If memory allocation fails, the winding is left in an empty state.

		\param verts Array of vertex points to initialize the winding
		\param n Number of vertices in the verts array
	*/
	explicit idWinding( const idVec3* verts, const int n );

	/*!
		\brief Creates a new winding based on a plane normal and distance

		This constructor initializes a winding object using a plane normal and distance to define the base plane for the winding. The winding is initialized with no points and a null point array. The
	   BaseForPlane method is called to set up the winding based on the provided plane parameters.

		\param normal The normal vector of the plane
		\param dist The distance from the origin to the plane along the normal
	*/
	explicit idWinding( const idVec3& normal, const float dist );

	/*!
		\brief Creates a winding from a plane

		Initializes a winding object based on the provided plane. The winding is constructed using the BaseForPlane method which sets up the vertices of the winding to match the given plane. This
	   constructor is typically used when a winding needs to be created directly from a plane definition rather than from a set of explicit points.

		\param plane The plane to create the winding from
	*/
	explicit idWinding( const idPlane& plane );

	/*!
		\brief Creates a new idWinding object as a copy of an existing idWinding object

		This constructor initializes a new idWinding object by copying all points from a source winding object. It first ensures that the new object has sufficient allocated memory to hold the points
	   from the source winding, and then copies each point individually. If memory allocation fails, the new winding will have zero points and the constructor will return early. The constructor
	   handles the case where the source winding might be empty or have no points allocated.

		\param winding The source idWinding object to copy points from
	*/
	explicit idWinding( const idWinding& winding );

	/*!
		\brief Destructor for the idWinding class that releases the memory allocated for the winding points.

		The destructor for idWinding is responsible for cleaning up the memory that was allocated for storing the points of the winding. It deallocates the memory block pointed to by the p member and
	   sets the pointer to NULL to avoid dangling references.

	*/
	virtual ~idWinding();

	idWinding&	  operator=( const idWinding& winding );
	const idVec5& operator[]( const int index ) const;
	idVec5&		  operator[]( const int index );

	// add a point to the end of the winding point array
	idWinding&	  operator+=( const idVec3& v );
	idWinding&	  operator+=( const idVec5& v );

	/*!
		\brief Adds a point to the winding after ensuring there is enough allocated space.

		This function appends a new point to the winding by first checking if there is sufficient allocated memory for the additional point. If memory allocation fails, the function returns early
	   without adding the point. Otherwise, it assigns the point to the next available slot in the point array and increments the point count.

		\param v The point to be added to the winding
	*/
	void		  AddPoint( const idVec3& v );

	/*!
		\brief Adds a point to the winding after ensuring sufficient memory allocation.

		This function appends a new point to the winding structure. It first checks if there is enough allocated memory for the additional point by calling EnsureAlloced with the new point count. If
	   memory allocation fails, the function returns early without adding the point. Otherwise, it assigns the given point to the next available slot in the point array and increments the point
	   counter.

		\param v The point to be added to the winding
	*/
	void		  AddPoint( const idVec5& v );

	//! Returns the number of points in the winding.
	int			  GetNumPoints() const;

	/*!
		\brief Sets the number of points in the winding to the specified value.

		This function adjusts the winding to contain exactly the specified number of points. It ensures that the internal memory allocation can accommodate the new number of points, and if the
	   allocation fails, the function returns without making any changes. The point count is updated to the specified value after ensuring adequate memory.

		\param n The desired number of points in the winding
	*/
	void		  SetNumPoints( int n );

	//! Resets the winding to an empty state by clearing the point count and freeing the memory.
	virtual void  Clear();

	/*!
		\brief Initializes a winding for a plane using the given normal and distance

		Creates a four-point winding that represents a large plane in 3D space. The winding is oriented such that its normal points in the same direction as the input normal vector. The points of the
	   winding are arranged counter clockwise when viewed from the front side of the plane. The winding is allocated with enough space for four points and is initialized with the appropriate
	   coordinates based on the plane's normal and distance from the origin. The s and t texture coordinates are set to zero for all points.

		\param normal The normal vector of the plane
		\param dist The distance from the origin to the plane along the normal vector
	*/
	void		  BaseForPlane( const idVec3& normal, const float dist );

	/*!
		\brief Computes the base winding for a plane defined by its normal and distance from origin.

		This function calculates the base winding for a plane using the provided normal vector and distance value. It delegates the actual computation to another BaseForPlane method that takes a
	   normal vector and distance as parameters. The resulting winding represents the base polygon for the given plane in the context of winding operations.

		\param plane The plane for which to compute the base winding, defined by its normal vector and distance from origin
	*/
	void		  BaseForPlane( const idPlane& plane );

	/*!
		\brief Splits the winding into front and back windings based on a clipping plane.

		This function divides the current winding into two new windings, one on each side of the provided clipping plane. The original winding remains unchanged. The function returns a side indicator
	   showing whether the winding is entirely on one side of the plane or crosses it. The front and back windings are allocated dynamically and must be managed by the caller. The epsilon parameter
	   controls the tolerance for considering a point as lying on the plane.

		\param plane The clipping plane used to split the winding
		\param epsilon Tolerance value for determining if a point lies on the plane
		\param front Pointer to store the resulting front winding
		\param back Pointer to store the resulting back winding
		\return An integer indicating the side relationship of the winding to the plane: SIDE_FRONT if the winding is entirely in front, SIDE_BACK if it's entirely behind, and SIDE_CROSS if it crosses
	   the plane. \throws May throw a fatal error if the number of points in the split windings exceeds the estimated maximum.
	*/
	int			  Split( const idPlane& plane, const float epsilon, idWinding** front, idWinding** back ) const;

	/*!
		\brief Clips the winding by the given plane and returns the fragment on the front side, or NULL if nothing remains.

		This function performs clipping of a winding against a plane. It determines which side of the plane each vertex lies on and generates new vertices at intersection points. The function modifies
	   the winding in place, keeping only the portion that lies on the front side of the plane. If the entire winding is on the back side, the winding is deleted and NULL is returned. If the winding
	   is entirely on the front side, the original winding is returned. The epsilon parameter controls the tolerance for considering points as lying on the plane, and keepOn determines whether to
	   return the original winding if all points are on the plane.

		\param plane The plane to clip the winding against
		\param epsilon Tolerance for considering points as lying on the plane
		\param keepOn If true, returns the original winding when all points are on the plane
		\return The clipped winding on the front side of the plane, or NULL if the entire winding was clipped away
		\throws NULL is returned if the winding is completely clipped away or if allocation fails
	*/
	idWinding*	  Clip( const idPlane& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	/*!
		\brief Clips the winding by the given plane, modifying the winding in place and returning true if any part remains on the front side of the plane.

		This function performs in-place clipping of a winding against a plane. It determines which side of the plane each point of the winding lies on and computes new intersection points where edges
	   cross the plane. The winding is modified to contain only the portion on the front side of the plane, or the portion on the front side excluding the portion on the back side, depending on the
	   epsilon and keepOn parameters. The function handles edge cases such as when the winding is entirely on the plane or when there is no intersection. If the result is empty, the number of points
	   in the winding is set to zero, and the function returns false.

		\param plane The plane to clip the winding against
		\param epsilon Epsilon value for determining if a point is on the plane
		\param keepOn If true, keeps the winding if it is entirely on the plane
		\return True if some part of the winding remains on the front side of the plane, false if the winding becomes empty or if there's no front part.
	*/
	bool		  ClipInPlace( const idPlane& plane, const float epsilon = ON_EPSILON, const bool keepOn = false );

	/*!
		\brief Returns a new copy of the winding object

		This function creates and returns a new idWinding object that is an exact copy of the current winding. The copy includes all the points from the original winding and maintains the same number
	   of points. The returned winding must be freed by the caller using the delete operator.

		\return A pointer to a new idWinding object that is an exact copy of this winding
	*/
	idWinding*	  Copy() const;

	/*!
		\brief Returns a new winding with the point order reversed from the original winding

		This function creates a new winding object that contains the same points as the original winding but with the order of the points reversed. The new winding is allocated using the
	   TAG_IDLIB_WINDING memory tag and has the same number of points as the original. The reversal is performed by copying points from the original winding in reverse order, so that the last point
	   becomes the first, and so on.

		\return A pointer to a new idWinding object with the points in reverse order
	*/
	idWinding*	  Reverse() const;

	/*!
		\brief Reverses the order of points in the winding.

		This function reverses the sequence of points stored in the winding object in place. It swaps points from the beginning with points from the end, moving towards the center. The winding is
	   modified directly without creating a new object.

	*/
	void		  ReverseSelf();

	/*!
		\brief Removes consecutive equal points from the winding within the specified epsilon tolerance.

		This function iterates through the winding points and removes any consecutive points that are within the given epsilon distance of each other. The removal is done in-place by shifting
	   subsequent points to fill the gap left by the removed point. The iteration continues until no more equal points are found.

		\param epsilon The tolerance value used to determine if two points are considered equal
	*/
	void		  RemoveEqualPoints( const float epsilon = ON_EPSILON );

	/*!
		\brief Removes colinear points from the winding by checking edge normals against the specified normal and epsilon tolerance.

		This function iterates through the points of the winding and identifies colinear points by computing the normal of each edge and checking if the next point lies within the specified epsilon
	   distance from the edge plane. When a colinear point is found, it is removed from the winding by shifting subsequent points to fill the gap. The process continues until no more colinear points
	   are detected.

		\param normal The normal vector of the winding plane used to compute edge normals
		\param epsilon The tolerance value for determining if points are colinear
	*/
	void		  RemoveColinearPoints( const idVec3& normal, const float epsilon = ON_EPSILON );

	/*!
		\brief Removes a point at the specified index from the winding

		This function removes a point from the winding by shifting all subsequent points one position to the left and decreasing the point count. It performs bounds checking to ensure the point index
	   is valid, and will cause a fatal error if the index is out of range. The function does not perform any memory deallocation, as it simply shifts elements in place.

		\param point The index of the point to remove from the winding
		\throws Fatal error if the point index is out of the valid range [0, numPoints)
	*/
	void		  RemovePoint( int point );

	/*!
		\brief Inserts a new point into the winding at the specified position.

		This function inserts a new point into the winding array at the given spot index. It first validates that the spot index is within the valid range [0, numPoints]. If the spot index is invalid,
	   the function will cause a fatal error. The function ensures that the winding array has enough allocated space for the new point, then shifts all existing points at and after the insertion spot
	   to make room for the new point. Finally, it assigns the new point to the specified spot and increments the point count.

		\param point The point to be inserted into the winding
		\param spot The index at which to insert the new point, must be between 0 and the current number of points inclusive
		\throws Fatal error if the spot index is negative or greater than the current number of points
	*/
	void		  InsertPoint( const idVec5& point, int spot );

	/*!
		\brief Inserts a point into the winding if it lies on an edge of the winding within the specified epsilon tolerance.

		This function determines whether a given point lies on an edge of the winding structure. It checks if the point is close enough to the winding's plane and if it lies within the bounds of any
	   edge. If so, it inserts the point into the winding at the appropriate position. The function uses a plane equation to test the point's position relative to the winding edges. The epsilon
	   parameter controls the tolerance for both plane distance and edge alignment. The function returns true if a point was successfully inserted.

		\param point The point to be inserted into the winding
		\param plane The plane that defines the winding's orientation
		\param epsilon The tolerance for checking if the point lies on the edge
		\return true if the point was successfully inserted into the winding, false otherwise
	*/
	bool		  InsertPointIfOnEdge( const idVec5& point, const idPlane& plane, const float epsilon = ON_EPSILON );

	/*!
		\brief Inserts a point into the winding if it lies on an edge of the winding within the specified tolerance.

		This function checks if the given point lies on an edge of the winding within the provided epsilon tolerance. If the point is on an edge and not already present, it inserts the point into the
	   winding. The function first verifies that the point is within the epsilon distance of the winding plane. Then it iterates through each edge of the winding, checking if the point lies on that
	   edge. If the point is found to be on an edge, it is inserted into the winding at the appropriate position. The function returns true if the point was successfully inserted, false otherwise.

		\param point The 3D point to be inserted into the winding
		\param plane The plane of the winding used for distance calculations
		\param epsilon The tolerance value for determining if the point lies on the edge of the winding
		\return True if the point was successfully inserted into the winding, false otherwise
	*/
	bool		  InsertPointIfOnEdge( const idVec3& point, const idPlane& plane, const float epsilon = ON_EPSILON );

	/*!
		\brief Adds a winding to the convex hull by computing the convex hull of the current winding and the provided winding

		This function computes the convex hull of the current winding and the provided winding. It iterates through each point of the provided winding and determines if the point lies outside the
	   current convex hull. If a point is outside, it adjusts the hull by inserting the point and removing any interior points that are no longer part of the hull. The function uses the provided
	   normal vector to calculate hull edge directions and determines point positions relative to these edges using the epsilon value for floating-point comparisons. The process effectively builds a
	   new convex hull that encompasses both the original and the provided winding.

		\param winding The winding to be added to the convex hull
		\param normal The normal vector used for calculating hull edge directions
		\param epsilon A small epsilon value for floating-point comparisons to determine if points are effectively inside or outside
	*/
	void		  AddToConvexHull( const idWinding* winding, const idVec3& normal, const float epsilon = ON_EPSILON );

	/*!
		\brief Adds a point to the convex hull defined by the winding, updating the hull structure accordingly.

		This function takes a point and normal vector to determine how to update the convex hull represented by the winding. It handles cases where the winding has 0, 1, or 2 points specially, and for
	   larger windings, computes the hull edge directions and sides relative to the new point. If the point is not outside the current hull, it is ignored. Otherwise, it finds the transition from back
	   to front side and constructs a new hull by inserting the point and removing any double front points. Memory is reallocated if necessary to fit the new hull.

		\param point The point to be added to the convex hull
		\param normal The normal vector defining the orientation of the hull
		\param epsilon A small tolerance value used for floating-point comparisons
	*/
	void		  AddToConvexHull( const idVec3& point, const idVec3& normal, const float epsilon = ON_EPSILON );

	/*!
		\brief Attempts to merge this winding with another winding along a shared edge, returning a new winding if successful.

		This function identifies a shared edge between the current winding and the provided winding 'w'. If a shared edge is found, it checks if the resulting polygon would remain convex. If the merge
	   is valid, it creates and returns a new winding that combines both polygons. The 'keep' parameter controls whether to preserve the connecting points even if they create colinear edges. Both
	   input windings remain unchanged.

		\param w The winding to merge with this winding
		\param normal The normal vector of the plane containing both windings
		\param keep Flag indicating whether to keep connecting points even if they result in colinear edges
		\return A new winding object representing the merged result, or NULL if the merge fails
	*/
	idWinding*	  TryMerge( const idWinding& w, const idVec3& normal, int keep = false ) const;

	/*!
		\brief Validates the winding for correctness and convexity

		Checks if the winding has a valid number of points, a sufficient area, points that lie on the plane, non-degenerate edges, and maintains convexity. The function performs multiple validation
	   steps including checking for points outside the world coordinates, ensuring the winding is not too small, verifying that all points are on the plane, confirming edges are not degenerate, and
	   validating that the winding is convex by ensuring all points are on the front side of each edge normal. The print parameter controls whether validation errors are reported to the console.

		\param print Controls whether validation errors are printed to the console
		\return True if the winding passes all validation checks, false otherwise
	*/
	bool		  Check( bool print = true ) const;

	//! Calculates and returns the area of the winding by summing triangle areas.
	float		  GetArea() const;

	//! Returns the center point of the winding by averaging all its vertices.
	idVec3		  GetCenter() const;

	/*!
		\brief Calculates and returns the radius of the winding as the distance from the specified center point to the farthest vertex

		This function computes the radius of a winding by finding the maximum distance from the provided center point to any vertex in the winding. It iterates through all vertices, calculates the
	   squared distance to the center, and keeps track of the maximum squared distance. Finally, it returns the square root of this maximum squared distance as the radius. The function is typically
	   used to determine the bounding radius of a winding relative to a given center point

		\param center The center point from which to calculate the radius to the farthest vertex
		\return The radius of the winding as the distance from the center point to the farthest vertex
	*/
	float		  GetRadius( const idVec3& center ) const;

	/*!
		\brief Computes the plane equation normal and distance from the winding's vertices

		This function calculates the plane equation that best fits the winding's vertices by using the first three points. It computes the normal vector by taking the cross product of two vectors
	   formed from the first three points relative to the winding's center. The distance from the origin is then calculated as the dot product of the first point and the normalized normal vector. If
	   the winding has fewer than three points, it sets the normal to zero and distance to zero.

		\param normal Output parameter that receives the computed plane normal vector
		\param dist Output parameter that receives the distance from origin to the plane
	*/
	void		  GetPlane( idVec3& normal, float& dist ) const;

	/*!
		\brief Computes and returns the plane equation that best fits the winding's vertices

		This function calculates the plane equation that represents the surface defined by the winding's vertices. It first checks if there are at least three points to compute a valid plane. If not,
	   it zeros the plane. Otherwise, it calculates the center of the winding, computes two vectors from the center to the first two points, and uses their cross product to determine the plane normal.
	   The normal is then normalized and the plane is fitted through the first point of the winding. This is commonly used in collision detection and rendering systems to convert polygonal data into
	   planar representations.

		\param plane Output parameter that will contain the computed plane equation
	*/
	void		  GetPlane( idPlane& plane ) const;

	/*!
		\brief Calculates and sets the bounding box for the winding object

		This function determines the minimum and maximum coordinates of all points in the winding and stores them in the provided bounds object. If the winding has no points, it clears the bounds. The
	   function iterates through all points in the winding and updates the bounds to encompass all points.

		\param bounds The bounds object that will be updated with the minimum and maximum coordinates of the winding points
	*/
	void		  GetBounds( idBounds& bounds ) const;

	//! Returns true if the winding is considered tiny based on its edge lengths.
	bool		  IsTiny() const;

	//! Checks if any vertex coordinate of the winding exceeds the world coordinate bounds.
	bool		  IsHuge() const;

	/*!
		\brief Prints the coordinates of all points in the winding to the standard output

		This function iterates through all points in the winding and prints their 3D coordinates in a formatted manner. Each point is displayed with 5.1f precision for x, y, and z components, followed
	   by a newline character. The output format is designed for readability and debugging purposes.

	*/
	void		  Print() const;

	/*!
		\brief Computes the minimum distance from the winding to the given plane, considering the winding's orientation relative to the plane.

		This function calculates the signed distance from the winding to the specified plane by examining the distances from each vertex of the winding to the plane. It returns the smallest distance
	   value, or zero if the winding straddles the plane. The function handles both positive and negative distances and returns zero when the winding intersects the plane, ensuring that the result
	   reflects the winding's position relative to the plane.

		\param plane The plane to calculate the distance to.
		\return The minimum distance from the winding to the plane, or zero if the winding intersects the plane.
	*/
	float		  PlaneDistance( const idPlane& plane ) const;

	/*!
		\brief Determines which side of the winding the given plane lies on, considering an epsilon tolerance for floating-point comparisons.

		This function evaluates the spatial relationship between a winding and a plane by checking the distance of each point in the winding from the plane. It returns one of the following values:
	   SIDE_FRONT if all points are in front of the plane, SIDE_BACK if all points are behind the plane, SIDE_ON if all points lie on the plane, or SIDE_CROSS if the winding crosses the plane. The
	   epsilon parameter controls the tolerance for determining if a point lies exactly on the plane.

		\param plane The plane to test against the winding
		\param epsilon A tolerance value for determining if a point lies exactly on the plane
		\return An integer indicating the side of the plane relative to the winding, with possible values SIDE_FRONT, SIDE_BACK, SIDE_ON, or SIDE_CROSS
	*/
	int			  PlaneSide( const idPlane& plane, const float epsilon = ON_EPSILON ) const;

	/*!
		\brief Determines whether two windings form a concave angle when their respective planes are considered.

		This function evaluates if the planes defined by two windings create a concave configuration. It checks if any point from the first winding lies on the back side of the second winding's plane,
	   and vice versa. If either condition is met, the function returns true, indicating that the planes are concave relative to each other. The comparison uses a small epsilon value to account for
	   floating-point precision errors.

		\param w2 The second winding to compare against
		\param normal1 The normal vector of the plane for the first winding
		\param normal2 The normal vector of the plane for the second winding
		\param dist1 The distance from the origin to the first winding's plane
		\param dist2 The distance from the origin to the second winding's plane
		\return true if the planes of the two windings form a concave angle, false otherwise
	*/
	bool		  PlanesConcave( const idWinding& w2, const idVec3& normal1, const idVec3& normal2, float dist1, float dist2 ) const;

	/*!
		\brief Checks if a point is inside the winding considering a given normal and epsilon tolerance

		This function determines whether a specified point lies within the winding by testing the point against each edge of the winding. It uses the provided normal to calculate the orientation of
	   each edge and checks if the point is on the correct side of each edge. The epsilon parameter defines the tolerance for the comparison, allowing for small numerical errors. The function returns
	   true if the point is inside the winding, and false otherwise.

		\param normal The normal vector used to determine the orientation of the edges
		\param point The point to be checked for containment within the winding
		\param epsilon The tolerance value for floating-point comparisons to account for numerical errors
		\return true if the point is inside the winding, false otherwise
	*/
	bool		  PointInside( const idVec3& normal, const idVec3& point, const float epsilon ) const;

	/*!
		\brief Checks if a line intersects with the winding, optionally culling back faces

		Determines whether a line segment defined by start and end points intersects with the winding plane. The function evaluates the positions of the start and end points relative to the winding
	   plane and calculates the intersection point. If the intersection point lies within the winding, the function returns true. Back face culling can be enabled to ignore intersections from the back
	   side of the winding plane

		\param windingPlane The plane of the winding to test against
		\param start The starting point of the line segment
		\param end The ending point of the line segment
		\param backFaceCull If true, back faces of the winding plane are culled
		\return True if the line segment intersects with the winding, false otherwise
	*/
	bool		  LineIntersection( const idPlane& windingPlane, const idVec3& start, const idVec3& end, bool backFaceCull = false ) const;

	/*!
		\brief Checks if a ray intersects with the winding and calculates the intersection scale factor.

		This function determines whether a ray defined by a starting point and direction intersects with the winding. It uses Pluecker coordinates to test for intersection with the winding edges. If
	   the ray intersects, the scale factor is calculated such that the intersection point is start + dir * scale. The function can optionally perform back face culling based on the winding plane
	   orientation. The winding plane is used to calculate the final intersection point.

		\param windingPlane The plane that defines the winding's orientation
		\param start The starting point of the ray
		\param dir The direction vector of the ray
		\param scale Output parameter that will contain the scale factor for the intersection point
		\param backFaceCull If true, back face culling is applied based on the winding plane orientation
		\return true if the ray intersects the winding, false otherwise
	*/
	bool		  RayIntersection( const idPlane& windingPlane, const idVec3& start, const idVec3& dir, float& scale, bool backFaceCull = false ) const;

	/*!
		\brief Computes the area of a triangle defined by three 3D vertices using the cross product method

		This function calculates the area of a triangle by computing the cross product of two vectors formed from the three input vertices. The cross product vector's length represents twice the area
	   of the triangle, so the result is divided by 2.0f to obtain the actual area. The function is commonly used in barycentric coordinate calculations and geometric computations. The implementation
	   handles degenerate cases where the three points are collinear by returning zero area, which is consistent with the behavior in the calling code that checks for zero denominator.

		\param a First vertex of the triangle
		\param b Second vertex of the triangle
		\param c Third vertex of the triangle
		\return The area of the triangle formed by the three input vertices
	*/
	static float  TriangleArea( const idVec3& a, const idVec3& b, const idVec3& c );

protected:
	int			 numPoints; // number of points
	idVec5*		 p;			// pointer to point data
	int			 allocedSize;

	/*!
		\brief Ensures the winding has allocated space for at least n points, resizing if necessary.

		This function checks if the current allocated size of the winding is less than the requested number of points. If so, it calls ReAllocate to expand the allocation. The keep parameter controls
	   whether existing data should be preserved during reallocation.

		\param n The minimum number of points the winding should be able to hold
		\param keep Whether to preserve existing data during reallocation
		\return true if the allocation is sufficient or was successfully expanded, false otherwise
	*/
	bool		 EnsureAlloced( int n, bool keep = false );

	/*!
		\brief Reallocates the winding point buffer to accommodate a specified number of points, optionally preserving existing point data.

		This function reallocates the internal buffer used to store the winding points. The new size is rounded up to the nearest multiple of four to ensure proper alignment. If the keep parameter is
	   true and there was existing data, the old point data is copied to the new buffer. The function updates the allocated size tracker and always returns true to indicate successful reallocation.

		\param n The desired number of points for the winding buffer, which will be rounded up to the next multiple of four
		\param keep If true, copies existing point data to the new buffer; if false, discards existing data
		\return True if the reallocation was successful, which it always is in this implementation.
	*/
	virtual bool ReAllocate( int n, bool keep = false );
};

ID_INLINE idWinding::idWinding()
{
	numPoints = allocedSize = 0;
	p						= NULL;
}

ID_INLINE idWinding::idWinding( int n )
{
	numPoints = allocedSize = 0;
	p						= NULL;
	EnsureAlloced( n );
}

ID_INLINE idWinding::idWinding( const idVec3* verts, const int n )
{
	int i;

	numPoints = allocedSize = 0;
	p						= NULL;
	if( !EnsureAlloced( n ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < n; i++ ) {
		p[i].ToVec3() = verts[i];
		p[i].s = p[i].t = 0.0f;
	}
	numPoints = n;
}

ID_INLINE idWinding::idWinding( const idVec3& normal, const float dist )
{
	numPoints = allocedSize = 0;
	p						= NULL;
	BaseForPlane( normal, dist );
}

ID_INLINE idWinding::idWinding( const idPlane& plane )
{
	numPoints = allocedSize = 0;
	p						= NULL;
	BaseForPlane( plane );
}

ID_INLINE idWinding::idWinding( const idWinding& winding )
{
	int i;
	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
}

ID_INLINE idWinding::~idWinding()
{
	delete[] p;
	p = NULL;
}

ID_INLINE idWinding& idWinding::operator=( const idWinding& winding )
{
	int i;

	if( !EnsureAlloced( winding.numPoints ) ) {
		numPoints = 0;
		return *this;
	}
	for( i = 0; i < winding.numPoints; i++ ) {
		p[i] = winding.p[i];
	}
	numPoints = winding.numPoints;
	return *this;
}

ID_INLINE const idVec5& idWinding::operator[]( const int index ) const
{
	// assert( index >= 0 && index < numPoints );
	return p[index];
}

ID_INLINE idVec5& idWinding::operator[]( const int index )
{
	// assert( index >= 0 && index < numPoints );
	return p[index];
}

ID_INLINE idWinding& idWinding::operator+=( const idVec3& v )
{
	AddPoint( v );
	return *this;
}

ID_INLINE idWinding& idWinding::operator+=( const idVec5& v )
{
	AddPoint( v );
	return *this;
}

ID_INLINE void idWinding::AddPoint( const idVec3& v )
{
	if( !EnsureAlloced( numPoints + 1, true ) ) { return; }
	p[numPoints] = v;
	numPoints++;
}

ID_INLINE void idWinding::AddPoint( const idVec5& v )
{
	if( !EnsureAlloced( numPoints + 1, true ) ) { return; }
	p[numPoints] = v;
	numPoints++;
}

ID_INLINE int idWinding::GetNumPoints() const
{
	return numPoints;
}

ID_INLINE void idWinding::SetNumPoints( int n )
{
	if( !EnsureAlloced( n, true ) ) { return; }
	numPoints = n;
}

ID_INLINE void idWinding::Clear()
{
	numPoints = 0;
	delete[] p;
	p = NULL;
}

ID_INLINE void idWinding::BaseForPlane( const idPlane& plane )
{
	BaseForPlane( plane.Normal(), plane.Dist() );
}

ID_INLINE bool idWinding::EnsureAlloced( int n, bool keep )
{
	if( n > allocedSize ) { return ReAllocate( n, keep ); }
	return true;
}

/*
===============================================================================

	idFixedWinding is a fixed buffer size winding not using
	memory allocations.

	When an operation would overflow the fixed buffer a warning
	is printed and the operation is safely cancelled.

===============================================================================
*/

#define MAX_POINTS_ON_WINDING 64

class idFixedWinding : public idWinding
{
public:
	/*!
		\brief Initializes a new instance of the idFixedWinding class with default values.

		This constructor initializes the winding object with zero points, sets the point pointer to the internal data buffer, and marks the allocated size as the maximum allowed points on a winding.

	*/
	idFixedWinding();

	/*!
		\brief Constructs an idFixedWinding object with a specified maximum number of points.

		Initializes the winding object with the given maximum point count. The actual number of points is set to zero, and the data pointer is set to the internal data buffer. The allocedSize is set
	   to the maximum allowed points for the winding.

		\param n The maximum number of points the winding can hold
	*/
	explicit idFixedWinding( const int n );

	/*!
		\brief Constructs a fixed winding from an array of vertices and initializes texture coordinates to zero.

		This constructor initializes a fixed winding object by copying vertex data from the provided array. It allocates memory for the winding points and sets the texture coordinates s and t to zero
	   for each point. The constructor ensures that the allocated size is sufficient for the specified number of points.

		\param verts Array of vertices to copy into the winding
		\param n Number of vertices in the verts array
	*/
	explicit idFixedWinding( const idVec3* verts, const int n );

	/*!
		\brief Constructs a fixed winding from a plane normal and distance

		Initializes a fixed winding object using the provided plane normal and distance. The winding is initialized with zero points and allocated memory for MAX_POINTS_ON_WINDING. The BaseForPlane
	   method is called to set up the winding based on the plane definition.

		\param normal The normal vector of the plane used to define the winding
		\param dist The distance from the origin to the plane along the normal vector
	*/
	explicit idFixedWinding( const idVec3& normal, const float dist );

	/*!
		\brief Initializes a fixed winding from a plane equation.

		Constructs a fixed winding object using the provided plane equation to define its base properties. The winding is initialized with zero points and allocated storage for the maximum number of
	   points allowed on a winding.

		\param plane The plane equation used to initialize the winding
	*/
	explicit idFixedWinding( const idPlane& plane );

	/*!
		\brief Constructs an idFixedWinding object by copying points from a given idWinding object.

		This constructor initializes a fixed winding by copying the points from another winding object. It first ensures that the internal buffer has sufficient capacity to hold all points from the
	   source winding, and then copies each point individually. If memory allocation fails, the winding will be left with zero points.

		\param winding Source winding object whose points are copied to this winding
	*/
	explicit idFixedWinding( const idWinding& winding );

	/*!
		\brief Initializes a new fixed winding object as a copy of an existing winding.

		The constructor creates a new idFixedWinding instance by copying all points from the provided source winding. It allocates memory for the points and initializes the internal data structure. If
	   memory allocation fails, the new winding will be left in an empty state with zero points.

		\param winding The source winding whose points will be copied to initialize this winding
	*/
	explicit idFixedWinding( const idFixedWinding& winding );

	/*!
		\brief Destructor for idFixedWinding that ensures the fixed buffer is not freed.

		The destructor for idFixedWinding sets the internal pointer p to NULL to prevent the fixed buffer from being freed. This is necessary because the buffer is fixed and should not be deallocated
	   by the destructor.

	*/
	virtual ~idFixedWinding();

	idFixedWinding& operator=( const idWinding& winding );

	//! Clears the winding by setting the point count to zero.
	virtual void	Clear();

	/*!
		\brief Splits the winding into front and back parts relative to a plane and returns which side the winding falls on or crosses.

		This function divides the current winding into two parts using the provided plane. The front part remains in the current winding object, while the back part is stored in the provided back
	   winding object. The function determines whether the winding lies entirely on one side of the plane or crosses it. The epsilon parameter controls the tolerance for determining if a point lies on
	   the plane. A return value of SIDE_FRONT indicates the winding is entirely in front of the plane, SIDE_BACK indicates it is entirely behind, SIDE_ON indicates it lies entirely on the plane, and
	   SIDE_CROSS indicates it crosses the plane.

		\param back Pointer to the winding that will store the back part of the split
		\param plane The plane to split the winding with
		\param epsilon The tolerance for determining if a point lies on the plane
		\return An integer indicating the side relationship between the winding and the plane, which can be SIDE_FRONT, SIDE_BACK, SIDE_ON, or SIDE_CROSS
	*/
	int				Split( idFixedWinding* back, const idPlane& plane, const float epsilon = ON_EPSILON );

protected:
	idVec5		 data[MAX_POINTS_ON_WINDING]; // point data

	/*!
		\brief Reallocates the winding buffer to accommodate the specified number of points

		This function checks if the requested number of points exceeds the maximum allowed limit for windings. If the limit is exceeded, it logs a warning and returns false. Otherwise, it returns true
	   indicating successful allocation. The keep parameter is not used in the current implementation

		\param n the number of points to allocate
		\param keep unused parameter, always false in current implementation
		\return true if the allocation is successful and the requested number of points is within the limit, false otherwise
	*/
	virtual bool ReAllocate( int n, bool keep = false );
};

ID_INLINE idFixedWinding::idFixedWinding()
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
}

ID_INLINE idFixedWinding::idFixedWinding( int n )
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
}

ID_INLINE idFixedWinding::idFixedWinding( const idVec3* verts, const int n )
{
	int i;

	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	if( !EnsureAlloced( n ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < n; i++ ) {
		p[i].ToVec3() = verts[i];
		p[i].s = p[i].t = 0;
	}
	numPoints = n;
}

ID_INLINE idFixedWinding::idFixedWinding( const idVec3& normal, const float dist )
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	BaseForPlane( normal, dist );
}

ID_INLINE idFixedWinding::idFixedWinding( const idPlane& plane )
{
	numPoints	= 0;
	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	BaseForPlane( plane );
}

ID_INLINE idFixedWinding::idFixedWinding( const idWinding& winding )
{
	int i;

	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
}

ID_INLINE idFixedWinding::idFixedWinding( const idFixedWinding& winding )
{
	int i;

	p			= data;
	allocedSize = MAX_POINTS_ON_WINDING;
	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
}

ID_INLINE idFixedWinding::~idFixedWinding()
{
	p = NULL; // otherwise it tries to free the fixed buffer
}

ID_INLINE idFixedWinding& idFixedWinding::operator=( const idWinding& winding )
{
	int i;

	if( !EnsureAlloced( winding.GetNumPoints() ) ) {
		numPoints = 0;
		return *this;
	}
	for( i = 0; i < winding.GetNumPoints(); i++ ) {
		p[i] = winding[i];
	}
	numPoints = winding.GetNumPoints();
	return *this;
}

ID_INLINE void idFixedWinding::Clear()
{
	numPoints = 0;
}
#endif /* !__WINDING_H__ */
