/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

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

#include "precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

/*
===============================================================================

	Dynamic Obstacle Avoidance

	- assumes the AI lives inside a bounding box aligned with the gravity direction
	- obstacles in proximity of the AI are gathered
	- if obstacles are found the AAS walls are also considered as obstacles
	- every obstacle is represented by an oriented bounding box (OBB)
	- an OBB is projected onto a 2D plane orthogonal to AI's gravity direction
	- the 2D windings of the projections are expanded for the AI bbox
	- a path tree is build using clockwise and counter clockwise edge walks along the winding edges
	- the path tree is pruned and optimized
	- the shortest path is chosen for navigation

===============================================================================
*/

const float MAX_OBSTACLE_RADIUS	   = 256.0f;
const float PUSH_OUTSIDE_OBSTACLES = 0.5f;
const float CLIP_BOUNDS_EPSILON	   = 10.0f;
const int	MAX_AAS_WALL_EDGES	   = 256;
const int	MAX_OBSTACLES		   = 256;
const int	MAX_PATH_NODES		   = 256;
const int	MAX_OBSTACLE_PATH	   = 64;

typedef struct obstacle_s {
	idVec2		bounds[2];
	idWinding2D winding;
	idEntity*	entity;
} obstacle_t;

typedef struct pathNode_s {
	int				   dir;
	idVec2			   pos;
	idVec2			   delta;
	float			   dist;
	int				   obstacle;
	int				   edgeNum;
	int				   numNodes;
	struct pathNode_s* parent;
	struct pathNode_s* children[2];
	struct pathNode_s* next;

	//! Initializes all member variables of the pathNode_s structure to their default values.
	void			   Init();
} pathNode_t;

void pathNode_s::Init()
{
	dir = 0;
	pos.Zero();
	delta.Zero();
	obstacle = -1;
	edgeNum	 = -1;
	numNodes = 0;
	parent = children[0] = children[1] = next = NULL;
}

idBlockAlloc<pathNode_t, 128> pathNodeAllocator;

//! Checks if a line segment intersects with a path defined by a sequence of nodes.
bool						  LineIntersectsPath( const idVec2& start, const idVec2& end, const pathNode_t* node )
{
	float  d0, d1, d2, d3;
	idVec3 plane1, plane2;

	plane1 = idWinding2D::Plane2DFromPoints( start, end );
	d0	   = plane1.x * node->pos.x + plane1.y * node->pos.y + plane1.z;
	while( node->parent ) {
		d1 = plane1.x * node->parent->pos.x + plane1.y * node->parent->pos.y + plane1.z;
		if( IEEE_FLT_SIGNBITSET( d0 ) ^ IEEE_FLT_SIGNBITSET( d1 ) ) {
			plane2 = idWinding2D::Plane2DFromPoints( node->pos, node->parent->pos );
			d2	   = plane2.x * start.x + plane2.y * start.y + plane2.z;
			d3	   = plane2.x * end.x + plane2.y * end.y + plane2.z;
			if( IEEE_FLT_SIGNBITSET( d2 ) ^ IEEE_FLT_SIGNBITSET( d3 ) ) {
				return true;
			}
		}
		d0	 = d1;
		node = node->parent;
	}
	return false;
}

//! Checks if a point is inside any obstacle and returns the index of the first matching obstacle, or -1 if none are found.
int PointInsideObstacle( const obstacle_t* obstacles, const int numObstacles, const idVec2& point )
{
	int i;

	for( i = 0; i < numObstacles; i++ ) {
		const idVec2* bounds = obstacles[i].bounds;
		if( point.x < bounds[0].x || point.y < bounds[0].y || point.x > bounds[1].x || point.y > bounds[1].y ) {
			continue;
		}

		if( !obstacles[i].winding.PointInside( point, 0.1f ) ) {
			continue;
		}

		return i;
	}

	return -1;
}

/*!
	\brief Moves a point outside of obstacles by finding the closest valid position outside the obstacle structure.

	This function attempts to find a valid point outside of any obstacle that contains the input point. It first checks if the point is inside an obstacle and, if so, tries to push it out along the
   direction of the closest obstacle edge. If that fails, it performs a more complex search using a queue-based approach to explore neighboring obstacles and find a valid point outside the obstacle
   structure. It updates the point parameter to the new position and sets the obstacle and edgeNum parameters to indicate which obstacle and edge were used to find the new point.

	\param obstacles Array of obstacle structures to check against
	\param numObstacles Number of obstacles in the obstacles array
	\param point Input point to move outside obstacles, updated to the new valid position
	\param obstacle Optional pointer to store the index of the obstacle used to find the new point
	\param edgeNum Optional pointer to store the edge number used to find the new point
*/
void GetPointOutsideObstacles( const obstacle_t* obstacles, const int numObstacles, idVec2& point, int* obstacle, int* edgeNum )
{
	int			i, j, k, n, bestObstacle, bestEdgeNum, queueStart, queueEnd, edgeNums[2];
	float		d, bestd, scale[2];
	idVec3		plane, bestPlane;
	idVec2		newPoint, dir, bestPoint;
	int*		queue;
	bool*		obstacleVisited;
	idWinding2D w1, w2;

	if( obstacle ) {
		*obstacle = -1;
	}
	if( edgeNum ) {
		*edgeNum = -1;
	}

	bestObstacle = PointInsideObstacle( obstacles, numObstacles, point );
	if( bestObstacle == -1 ) {
		return;
	}

	const idWinding2D& w = obstacles[bestObstacle].winding;
	bestd				 = idMath::INFINITUM;
	bestEdgeNum			 = 0;
	for( i = 0; i < w.GetNumPoints(); i++ ) {
		plane = idWinding2D::Plane2DFromPoints( w[( i + 1 ) % w.GetNumPoints()], w[i], true );
		d	  = plane.x * point.x + plane.y * point.y + plane.z;
		if( d < bestd ) {
			bestd		= d;
			bestPlane	= plane;
			bestEdgeNum = i;
		}
		// if this is a wall always try to pop out at the first edge
		if( obstacles[bestObstacle].entity == NULL ) {
			break;
		}
	}

	newPoint = point - ( bestd + PUSH_OUTSIDE_OBSTACLES ) * bestPlane.ToVec2();
	if( PointInsideObstacle( obstacles, numObstacles, newPoint ) == -1 ) {
		point = newPoint;
		if( obstacle ) {
			*obstacle = bestObstacle;
		}
		if( edgeNum ) {
			*edgeNum = bestEdgeNum;
		}
		return;
	}

	queue			= ( int* )_alloca( numObstacles * sizeof( queue[0] ) );
	obstacleVisited = ( bool* )_alloca( numObstacles * sizeof( obstacleVisited[0] ) );

	queueStart = 0;
	queueEnd   = 1;
	queue[0]   = bestObstacle;

	memset( obstacleVisited, 0, numObstacles * sizeof( obstacleVisited[0] ) );
	assert( bestObstacle < numObstacles );
	obstacleVisited[bestObstacle] = true;

	bestd = idMath::INFINITUM;
	for( i = queue[0]; queueStart < queueEnd; i = queue[++queueStart] ) {
		w1 = obstacles[i].winding;
		w1.Expand( PUSH_OUTSIDE_OBSTACLES );

		for( j = 0; j < numObstacles; j++ ) {
			// if the obstacle has been visited already
			if( obstacleVisited[j] ) {
				continue;
			}
			// if the bounds do not intersect
			if( obstacles[j].bounds[0].x > obstacles[i].bounds[1].x || obstacles[j].bounds[0].y > obstacles[i].bounds[1].y || obstacles[j].bounds[1].x < obstacles[i].bounds[0].x ||
				obstacles[j].bounds[1].y < obstacles[i].bounds[0].y ) {
				continue;
			}

			assert( queueEnd < numObstacles );
			queue[queueEnd++]  = j;
			obstacleVisited[j] = true;

			w2 = obstacles[j].winding;
			w2.Expand( 0.2f );

			for( k = 0; k < w1.GetNumPoints(); k++ ) {
				dir = w1[( k + 1 ) % w1.GetNumPoints()] - w1[k];
				if( !w2.RayIntersection( w1[k], dir, scale[0], scale[1], edgeNums ) ) {
					continue;
				}
				for( n = 0; n < 2; n++ ) {
					newPoint = w1[k] + scale[n] * dir;
					if( PointInsideObstacle( obstacles, numObstacles, newPoint ) == -1 ) {
						d = ( newPoint - point ).LengthSqr();
						if( d < bestd ) {
							bestd		 = d;
							bestPoint	 = newPoint;
							bestEdgeNum	 = edgeNums[n];
							bestObstacle = j;
						}
					}
				}
			}
		}

		if( bestd < idMath::INFINITUM ) {
			point = bestPoint;
			if( obstacle ) {
				*obstacle = bestObstacle;
			}
			if( edgeNum ) {
				*edgeNum = bestEdgeNum;
			}
			return;
		}
	}
	gameLocal.Warning( "GetPointOutsideObstacles: no valid point found" );
}

/*!
	\brief Checks if a movement delta intersects with any obstacles and returns the closest blocking obstacle.

	This function determines if a movement vector from a starting position intersects with any of the provided obstacles. It tests each obstacle for intersection with the movement ray and tracks the
   closest intersection that blocks progress. The function skips a specified obstacle during testing and returns information about the first obstacle that blocks the movement path. The movement is
   defined by a start position and a delta vector. Intersection tests are performed using the obstacle's winding data and ray intersection methods.

	\param obstacles Array of obstacles to test against
	\param numObstacles Number of obstacles in the obstacles array
	\param skipObstacle Index of obstacle to skip during testing
	\param startPos Starting position for the movement ray
	\param delta Movement delta vector defining direction and distance
	\param blockingScale Output parameter for the scale of the blocking intersection
	\param blockingObstacle Output parameter for the index of the blocking obstacle
	\param blockingEdgeNum Output parameter for the edge number of the blocking intersection
	\return True if an obstacle blocks the movement path, false otherwise.
*/
bool GetFirstBlockingObstacle(
	const obstacle_t* obstacles, int numObstacles, int skipObstacle, const idVec2& startPos, const idVec2& delta, float& blockingScale, int& blockingObstacle, int& blockingEdgeNum )
{
	int	   i, edgeNums[2];
	float  dist, scale1, scale2;
	idVec2 bounds[2];

	// get bounds for the current movement delta
	bounds[0] = startPos - idVec2( CM_BOX_EPSILON, CM_BOX_EPSILON );
	bounds[1] = startPos + idVec2( CM_BOX_EPSILON, CM_BOX_EPSILON );
	bounds[IEEE_FLT_SIGNBITNOTSET( delta.x )].x += delta.x;
	bounds[IEEE_FLT_SIGNBITNOTSET( delta.y )].y += delta.y;

	// test for obstacles blocking the path
	blockingScale = idMath::INFINITUM;
	dist		  = delta.Length();
	for( i = 0; i < numObstacles; i++ ) {
		if( i == skipObstacle ) {
			continue;
		}
		if( bounds[0].x > obstacles[i].bounds[1].x || bounds[0].y > obstacles[i].bounds[1].y || bounds[1].x < obstacles[i].bounds[0].x || bounds[1].y < obstacles[i].bounds[0].y ) {
			continue;
		}
		if( obstacles[i].winding.RayIntersection( startPos, delta, scale1, scale2, edgeNums ) ) {
			if( scale1 < blockingScale && scale1 * dist > -0.01f && scale2 * dist > 0.01f ) {
				blockingScale	 = scale1;
				blockingObstacle = i;
				blockingEdgeNum	 = edgeNums[0];
			}
		}
	}
	return ( blockingScale < 1.0f );
}

/*!
	\brief Fills an array of obstacle structures for navigation purposes based on physics and AAS data

	This function collects and processes dynamic obstacles that may block movement between two points. It gathers clip models that touch the specified bounds, checks if they are valid obstacles (such
   as actors or moveables), and excludes those that can be stepped over or are irrelevant. For each valid obstacle, it creates a 2D winding representation that can be used for collision detection.
   Additionally, it adds obstacles representing AAS walls in the specified area. The function returns the number of obstacles found and stored in the output array.

	\param physics Physics object that defines the movement constraints
	\param aas AAS navigation data for wall obstacle creation
	\param ignore Entity to ignore during obstacle detection
	\param areaNum AAS area number for wall obstacle detection
	\param startPos Starting position for obstacle detection
	\param seekPos Target position for obstacle detection
	\param obstacles Output array to fill with obstacle data
	\param maxObstacles Maximum number of obstacles the array can hold
	\param clipBounds Bounds used to determine which clip models to consider
	\return Number of obstacles filled into the obstacles array
*/
int GetObstacles(
	const idPhysics* physics, const idAAS* aas, const idEntity* ignore, int areaNum, const idVec3& startPos, const idVec3& seekPos, obstacle_t* obstacles, int maxObstacles, idBounds& clipBounds )
{
	int			 i, j, numListedClipModels, numObstacles, numVerts, clipMask, blockingObstacle, blockingEdgeNum;
	int			 wallEdges[MAX_AAS_WALL_EDGES], numWallEdges, verts[2], lastVerts[2], nextVerts[2];
	float		 stepHeight, headHeight, blockingScale, min, max;
	idVec3		 seekDelta, silVerts[32], start, end, nextStart, nextEnd;
	idVec2		 expBounds[2], edgeDir, edgeNormal, nextEdgeDir, nextEdgeNormal, lastEdgeNormal;
	idVec2		 obDelta;
	idPhysics*	 obPhys;
	idBox		 box;
	idEntity*	 obEnt;
	idClipModel* clipModel;
	idClipModel* clipModelList[MAX_GENTITIES];

	numObstacles = 0;

	seekDelta	 = seekPos - startPos;
	expBounds[0] = physics->GetBounds()[0].ToVec2() - idVec2( CM_BOX_EPSILON, CM_BOX_EPSILON );
	expBounds[1] = physics->GetBounds()[1].ToVec2() + idVec2( CM_BOX_EPSILON, CM_BOX_EPSILON );

	physics->GetAbsBounds().AxisProjection( -physics->GetGravityNormal(), stepHeight, headHeight );
	stepHeight += aas->GetSettings()->maxStepHeight;

	// clip bounds for the obstacle search space
	clipBounds[0] = clipBounds[1] = startPos;
	clipBounds.AddPoint( seekPos );
	clipBounds.ExpandSelf( MAX_OBSTACLE_RADIUS );
	clipMask = physics->GetClipMask();

	// find all obstacles touching the clip bounds
	numListedClipModels = gameLocal.clip.ClipModelsTouchingBounds( clipBounds, clipMask, clipModelList, MAX_GENTITIES );

	for( i = 0; i < numListedClipModels && numObstacles < MAX_OBSTACLES; i++ ) {
		clipModel = clipModelList[i];
		obEnt	  = clipModel->GetEntity();

		if( !clipModel->IsTraceModel() ) {
			continue;
		}

		if( obEnt->IsType( idActor::Type ) ) {
			obPhys = obEnt->GetPhysics();
			// ignore myself, my enemy, and dead bodies
			if( ( obPhys == physics ) || ( obEnt == ignore ) || ( obEnt->health <= 0 ) ) {
				continue;
			}
			// if the actor is moving
			idVec3 v1 = obPhys->GetLinearVelocity();
			if( v1.LengthSqr() > Square( 10.0f ) ) {
				idVec3 v2 = physics->GetLinearVelocity();
				if( v2.LengthSqr() > Square( 10.0f ) ) {
					// if moving in about the same direction
					if( v1 * v2 > 0.0f ) {
						continue;
					}
				}
			}
		} else if( obEnt->IsType( idMoveable::Type ) ) {
			// moveables are considered obstacles
		} else {
			// ignore everything else
			continue;
		}

		// check if we can step over the object
		clipModel->GetAbsBounds().AxisProjection( -physics->GetGravityNormal(), min, max );
		if( max < stepHeight || min > headHeight ) {
			// can step over this one
			continue;
		}

		// project a box containing the obstacle onto the floor plane
		box		 = idBox( clipModel->GetBounds(), clipModel->GetOrigin(), clipModel->GetAxis() );
		numVerts = box.GetParallelProjectionSilhouetteVerts( physics->GetGravityNormal(), silVerts );

		// create a 2D winding for the obstacle;
		obstacle_t& obstacle = obstacles[numObstacles++];
		obstacle.winding.Clear();
		for( j = 0; j < numVerts; j++ ) {
			obstacle.winding.AddPoint( silVerts[j].ToVec2() );
		}

		if( ai_showObstacleAvoidance.GetBool() ) {
			for( j = 0; j < numVerts; j++ ) {
				silVerts[j].z = startPos.z;
			}
			for( j = 0; j < numVerts; j++ ) {
				gameRenderWorld->DebugArrow( colorWhite, silVerts[j], silVerts[( j + 1 ) % numVerts], 4 );
			}
		}

		// expand the 2D winding for collision with a 2D box
		obstacle.winding.ExpandForAxialBox( expBounds );
		obstacle.winding.GetBounds( obstacle.bounds );
		obstacle.entity = obEnt;
	}

	// if there are no dynamic obstacles the path should be through valid AAS space
	if( numObstacles == 0 ) {
		return 0;
	}

	// if the current path doesn't intersect any dynamic obstacles the path should be through valid AAS space
	if( PointInsideObstacle( obstacles, numObstacles, startPos.ToVec2() ) == -1 ) {
		if( !GetFirstBlockingObstacle( obstacles, numObstacles, -1, startPos.ToVec2(), seekDelta.ToVec2(), blockingScale, blockingObstacle, blockingEdgeNum ) ) {
			return 0;
		}
	}

	// create obstacles for AAS walls
	if( aas ) {
		float halfBoundsSize = ( expBounds[1].x - expBounds[0].x ) * 0.5f;

		numWallEdges = aas->GetWallEdges( areaNum, clipBounds, TFL_WALK, wallEdges, MAX_AAS_WALL_EDGES );
		aas->SortWallEdges( wallEdges, numWallEdges );

		lastVerts[0] = lastVerts[1] = 0;
		lastEdgeNormal.Zero();
		nextVerts[0] = nextVerts[1] = 0;
		for( i = 0; i < numWallEdges && numObstacles < MAX_OBSTACLES; i++ ) {
			aas->GetEdge( wallEdges[i], start, end );
			aas->GetEdgeVertexNumbers( wallEdges[i], verts );
			edgeDir = end.ToVec2() - start.ToVec2();
			edgeDir.Normalize();
			edgeNormal.x = edgeDir.y;
			edgeNormal.y = -edgeDir.x;
			if( i < numWallEdges - 1 ) {
				aas->GetEdge( wallEdges[i + 1], nextStart, nextEnd );
				aas->GetEdgeVertexNumbers( wallEdges[i + 1], nextVerts );
				nextEdgeDir = nextEnd.ToVec2() - nextStart.ToVec2();
				nextEdgeDir.Normalize();
				nextEdgeNormal.x = nextEdgeDir.y;
				nextEdgeNormal.y = -nextEdgeDir.x;
			}

			obstacle_t& obstacle = obstacles[numObstacles++];
			obstacle.winding.Clear();
			obstacle.winding.AddPoint( end.ToVec2() );
			obstacle.winding.AddPoint( start.ToVec2() );
			obstacle.winding.AddPoint( start.ToVec2() - edgeDir - edgeNormal * halfBoundsSize );
			obstacle.winding.AddPoint( end.ToVec2() + edgeDir - edgeNormal * halfBoundsSize );
			if( lastVerts[1] == verts[0] ) {
				obstacle.winding[2] -= lastEdgeNormal * halfBoundsSize;
			} else {
				obstacle.winding[1] -= edgeDir;
			}
			if( verts[1] == nextVerts[0] ) {
				obstacle.winding[3] -= nextEdgeNormal * halfBoundsSize;
			} else {
				obstacle.winding[0] += edgeDir;
			}
			obstacle.winding.GetBounds( obstacle.bounds );
			obstacle.entity = NULL;

			memcpy( lastVerts, verts, sizeof( lastVerts ) );
			lastEdgeNormal = edgeNormal;
		}
	}

	// show obstacles
	if( ai_showObstacleAvoidance.GetBool() ) {
		for( i = 0; i < numObstacles; i++ ) {
			obstacle_t& obstacle = obstacles[i];
			for( j = 0; j < obstacle.winding.GetNumPoints(); j++ ) {
				silVerts[j].ToVec2() = obstacle.winding[j];
				silVerts[j].z		 = startPos.z;
			}
			for( j = 0; j < obstacle.winding.GetNumPoints(); j++ ) {
				gameRenderWorld->DebugArrow( colorGreen, silVerts[j], silVerts[( j + 1 ) % obstacle.winding.GetNumPoints()], 4 );
			}
		}
	}

	return numObstacles;
}

//! Recursively frees all nodes in a path tree starting from the given node.
void FreePathTree_r( pathNode_t* node )
{
	if( node->children[0] ) {
		FreePathTree_r( node->children[0] );
	}
	if( node->children[1] ) {
		FreePathTree_r( node->children[1] );
	}
	pathNodeAllocator.Free( node );
}

//! Draws a visual representation of a path tree structure using debug arrows.
void DrawPathTree( const pathNode_t* root, const float height )
{
	int				  i;
	idVec3			  start, end;
	const pathNode_t* node;

	for( node = root; node; node = node->next ) {
		for( i = 0; i < 2; i++ ) {
			if( node->children[i] ) {
				start.ToVec2() = node->pos;
				start.z		   = height;
				end.ToVec2()   = node->children[i]->pos;
				end.z		   = height;
				gameRenderWorld->DebugArrow( node->edgeNum == -1 ? colorYellow : i ? colorBlue : colorRed, start, end, 1 );
				break;
			}
		}
	}
}

/*!
	\brief Computes and sets the delta vector for a path node based on obstacle boundaries and goal position

	This function calculates the movement delta for a path node by determining the closest edge of an obstacle winding. If the node is not blocked, it also checks if the current edge faces the seek
   position and whether the line to the seek position intersects the current path. If so, it updates the delta to point directly towards the seek position. The function also checks for overlapping
   edges in the path to avoid invalid routes. Returns true if the node is valid and can be used for pathfinding, false otherwise.

	\param node Pointer to the path node being processed
	\param obstacles Array of obstacle definitions containing winding data
	\param seekPos The target position to seek towards
	\param blocked Flag indicating if the current node is blocked by an obstacle
	\return True if the node delta was successfully computed and the node is valid for pathfinding, false if the node would create an invalid path due to overlapping edges
*/
bool GetPathNodeDelta( pathNode_t* node, const obstacle_t* obstacles, const idVec2& seekPos, bool blocked )
{
	int			numPoints, edgeNum;
	bool		facing;
	idVec2		seekDelta, dir;
	pathNode_t* n;

	numPoints = obstacles[node->obstacle].winding.GetNumPoints();

	// get delta along the current edge
	while( 1 ) {
		edgeNum		= ( node->edgeNum + node->dir ) % numPoints;
		node->delta = obstacles[node->obstacle].winding[edgeNum] - node->pos;
		if( node->delta.LengthSqr() > 0.01f ) {
			break;
		}
		node->edgeNum = ( node->edgeNum + numPoints + ( 2 * node->dir - 1 ) ) % numPoints;
	}

	// if not blocked
	if( !blocked ) {
		// test if the current edge faces the goal
		seekDelta = seekPos - node->pos;
		facing	  = ( ( 2 * node->dir - 1 ) * ( node->delta.x * seekDelta.y - node->delta.y * seekDelta.x ) ) >= 0.0f;

		// if the current edge faces goal and the line from the current
		// position to the goal does not intersect the current path
		if( facing && !LineIntersectsPath( node->pos, seekPos, node->parent ) ) {
			node->delta	  = seekPos - node->pos;
			node->edgeNum = -1;
		}
	}

	// if the delta is along the obstacle edge
	if( node->edgeNum != -1 ) {
		// if the edge is found going from this node to the root node
		for( n = node->parent; n; n = n->parent ) {
			if( node->obstacle != n->obstacle || node->edgeNum != n->edgeNum ) {
				continue;
			}

			// test whether or not the edge segments actually overlap
			if( n->pos * node->delta > ( node->pos + node->delta ) * node->delta ) {
				continue;
			}
			if( node->pos * node->delta > ( n->pos + n->delta ) * node->delta ) {
				continue;
			}

			break;
		}
		if( n ) {
			return false;
		}
	}
	return true;
}

/*!
	\brief Builds a path tree for obstacle avoidance navigation from a start position to a seek position.

	This function constructs a tree of path nodes that represent possible paths from the start position to the seek position, taking into account obstacles in the environment. It uses a queue-based
   approach to explore potential paths, dynamically branching when obstacles are encountered. The function returns the root node of the constructed path tree, which can be used to determine a valid
   path or to continue pathfinding.

	\param obstacles Array of obstacle data structures defining the environment's obstacles
	\param numObstacles Number of obstacles in the obstacles array
	\param clipBounds Bounds defining the clipping area within which the path must stay
	\param startPos Starting position for the pathfinding operation
	\param seekPos Target position to navigate towards
	\param path Output parameter containing the final obstacle path information
	\return Pointer to the root node of the constructed path tree
*/
pathNode_t* BuildPathTree( const obstacle_t* obstacles, int numObstacles, const idBounds& clipBounds, const idVec2& startPos, const idVec2& seekPos, obstaclePath_t& path )
{
	int														  blockingEdgeNum, blockingObstacle, obstaclePoints, bestNumNodes = MAX_OBSTACLE_PATH;
	float													  blockingScale;
	pathNode_t *											  root, *node, *child;
	// gcc 4.0
	idQueueTemplate<pathNode_t, offsetof( pathNode_t, next )> pathNodeQueue, treeQueue;

	root = pathNodeAllocator.Alloc();
	root->Init();
	root->pos = startPos;

	root->delta	   = seekPos - root->pos;
	root->numNodes = 0;
	pathNodeQueue.Add( root );

	for( node = pathNodeQueue.Get(); node != NULL && pathNodeAllocator.GetAllocCount() < MAX_PATH_NODES; node = pathNodeQueue.Get() ) {
		treeQueue.Add( node );

		// if this path has more than twice the number of nodes than the best path so far
		if( node->numNodes > bestNumNodes * 2 ) {
			continue;
		}

		// don't move outside of the clip bounds
		idVec2 endPos = node->pos + node->delta;
		if( endPos.x - CLIP_BOUNDS_EPSILON < clipBounds[0].x || endPos.x + CLIP_BOUNDS_EPSILON > clipBounds[1].x || endPos.y - CLIP_BOUNDS_EPSILON < clipBounds[0].y ||
			endPos.y + CLIP_BOUNDS_EPSILON > clipBounds[1].y ) {
			continue;
		}

		// if an obstacle is blocking the path
		if( GetFirstBlockingObstacle( obstacles, numObstacles, node->obstacle, node->pos, node->delta, blockingScale, blockingObstacle, blockingEdgeNum ) ) {
			if( path.firstObstacle == NULL ) {
				path.firstObstacle = obstacles[blockingObstacle].entity;
			}

			node->delta *= blockingScale;

			if( node->edgeNum == -1 ) {
				node->children[0] = pathNodeAllocator.Alloc();
				node->children[0]->Init();
				node->children[1] = pathNodeAllocator.Alloc();
				node->children[1]->Init();
				node->children[0]->dir	  = 0;
				node->children[1]->dir	  = 1;
				node->children[0]->parent = node->children[1]->parent = node;
				node->children[0]->pos = node->children[1]->pos = node->pos + node->delta;
				node->children[0]->obstacle = node->children[1]->obstacle = blockingObstacle;
				node->children[0]->edgeNum = node->children[1]->edgeNum = blockingEdgeNum;
				node->children[0]->numNodes = node->children[1]->numNodes = node->numNodes + 1;
				if( GetPathNodeDelta( node->children[0], obstacles, seekPos, true ) ) {
					pathNodeQueue.Add( node->children[0] );
				}
				if( GetPathNodeDelta( node->children[1], obstacles, seekPos, true ) ) {
					pathNodeQueue.Add( node->children[1] );
				}
			} else {
				node->children[node->dir] = child = pathNodeAllocator.Alloc();
				child->Init();
				child->dir		= node->dir;
				child->parent	= node;
				child->pos		= node->pos + node->delta;
				child->obstacle = blockingObstacle;
				child->edgeNum	= blockingEdgeNum;
				child->numNodes = node->numNodes + 1;
				if( GetPathNodeDelta( child, obstacles, seekPos, true ) ) {
					pathNodeQueue.Add( child );
				}
			}
		} else {
			node->children[node->dir] = child = pathNodeAllocator.Alloc();
			child->Init();
			child->dir		= node->dir;
			child->parent	= node;
			child->pos		= node->pos + node->delta;
			child->numNodes = node->numNodes + 1;

			// there is a free path towards goal
			if( node->edgeNum == -1 ) {
				if( node->numNodes < bestNumNodes ) {
					bestNumNodes = node->numNodes;
				}
				continue;
			}

			child->obstacle = node->obstacle;
			obstaclePoints	= obstacles[node->obstacle].winding.GetNumPoints();
			child->edgeNum	= ( node->edgeNum + obstaclePoints + ( 2 * node->dir - 1 ) ) % obstaclePoints;

			if( GetPathNodeDelta( child, obstacles, seekPos, false ) ) {
				pathNodeQueue.Add( child );
			}
		}
	}

	return root;
}

//! Prunes a path tree by removing branches that do not lead to the nearest node from the seek position.
void PrunePathTree( pathNode_t* root, const idVec2& seekPos )
{
	int			i;
	float		bestDist;
	pathNode_t *node, *lastNode, *n, *bestNode;

	node = root;
	while( node ) {
		node->dist = ( seekPos - node->pos ).LengthSqr();

		if( node->children[0] ) {
			node = node->children[0];
		} else if( node->children[1] ) {
			node = node->children[1];
		} else {
			// find the node closest to the goal along this path
			bestDist = idMath::INFINITUM;
			bestNode = node;
			for( n = node; n; n = n->parent ) {
				if( n->children[0] && n->children[1] ) {
					break;
				}
				if( n->dist < bestDist ) {
					bestDist = n->dist;
					bestNode = n;
				}
			}

			// free tree down from the best node
			for( i = 0; i < 2; i++ ) {
				if( bestNode->children[i] ) {
					FreePathTree_r( bestNode->children[i] );
					bestNode->children[i] = NULL;
				}
			}

			for( lastNode = bestNode, node = bestNode->parent; node; lastNode = node, node = node->parent ) {
				if( node->children[1] && ( node->children[1] != lastNode ) ) {
					node = node->children[1];
					break;
				}
			}
		}
	}
}

/*!
	\brief Optimizes a path by finding shortcut points between nodes while avoiding obstacles

	This function takes a path defined by a root node and a leaf node, along with a list of obstacles, and computes an optimized path that shortcuts when possible. It iterates through the path nodes,
   checking for potential shortcuts between current and next nodes. For each shortcut, it verifies that the path between the nodes does not intersect with any obstacles. The optimization process
   maintains the original starting point and adds shortcut points to reduce the overall path length.

	\param root Starting node of the path to optimize
	\param leafNode Ending node of the path to optimize
	\param obstacles Array of obstacles that may block the path
	\param numObstacles Number of obstacles in the obstacles array
	\param optimizedPath Output array where the optimized path points are stored
	\return Number of points in the optimized path
*/
int OptimizePath( const pathNode_t* root, const pathNode_t* leafNode, const obstacle_t* obstacles, int numObstacles, idVec2 optimizedPath[MAX_OBSTACLE_PATH] )
{
	int				  i, numPathPoints, edgeNums[2];
	const pathNode_t *curNode, *nextNode;
	idVec2			  curPos, curDelta, bounds[2];
	float			  scale1, scale2, curLength;

	optimizedPath[0] = root->pos;
	numPathPoints	 = 1;

	for( nextNode = curNode = root; curNode != leafNode; curNode = nextNode ) {
		for( nextNode = leafNode; nextNode->parent != curNode; nextNode = nextNode->parent ) {
			// can only take shortcuts when going from one object to another
			if( nextNode->obstacle == curNode->obstacle ) {
				continue;
			}

			curPos	  = curNode->pos;
			curDelta  = nextNode->pos - curPos;
			curLength = curDelta.Length();

			// get bounds for the current movement delta
			bounds[0] = curPos - idVec2( CM_BOX_EPSILON, CM_BOX_EPSILON );
			bounds[1] = curPos + idVec2( CM_BOX_EPSILON, CM_BOX_EPSILON );
			bounds[IEEE_FLT_SIGNBITNOTSET( curDelta.x )].x += curDelta.x;
			bounds[IEEE_FLT_SIGNBITNOTSET( curDelta.y )].y += curDelta.y;

			// test if the shortcut intersects with any obstacles
			for( i = 0; i < numObstacles; i++ ) {
				if( bounds[0].x > obstacles[i].bounds[1].x || bounds[0].y > obstacles[i].bounds[1].y || bounds[1].x < obstacles[i].bounds[0].x || bounds[1].y < obstacles[i].bounds[0].y ) {
					continue;
				}
				if( obstacles[i].winding.RayIntersection( curPos, curDelta, scale1, scale2, edgeNums ) ) {
					if( scale1 >= 0.0f && scale1 <= 1.0f && ( i != nextNode->obstacle || scale1 * curLength < curLength - 0.5f ) ) {
						break;
					}
					if( scale2 >= 0.0f && scale2 <= 1.0f && ( i != nextNode->obstacle || scale2 * curLength < curLength - 0.5f ) ) {
						break;
					}
				}
			}
			if( i >= numObstacles ) {
				break;
			}
		}

		// store the next position along the optimized path
		optimizedPath[numPathPoints++] = nextNode->pos;
	}

	return numPathPoints;
}

//! Calculates the total length of a path while adding a penalty if the path does not align with the current direction.
float PathLength( idVec2 optimizedPath[MAX_OBSTACLE_PATH], int numPathPoints, const idVec2& curDir )
{
	int	  i;
	float pathLength;

	// calculate the path length
	pathLength = 0.0f;
	for( i = 0; i < numPathPoints - 1; i++ ) {
		pathLength += ( optimizedPath[i + 1] - optimizedPath[i] ).LengthFast();
	}

	// add penalty if this path does not go in the current direction
	if( numPathPoints > 1 && curDir * ( optimizedPath[1] - optimizedPath[0] ) < 0.0f ) {
		pathLength += 100.0f;
	}
	return pathLength;
}

/*!
	\brief Finds an optimal path around obstacles from a root node toward a goal, updating the seek position with the next best point.

	This function performs a pathfinding operation to find the optimal path from a given root node to a goal while avoiding obstacles. It evaluates multiple potential paths and selects the one with
   the shortest length. The function updates the seekPos parameter with the next point in the calculated path. The path is optimized using the OptimizePath and PathLength helper functions and
   considers the current direction vector for path evaluation. The function returns true if a path to the goal exists, and false otherwise.

	\param root The starting node for the pathfinding operation
	\param obstacles Array of obstacle definitions to avoid
	\param numObstacles Number of obstacles in the obstacles array
	\param height The z-coordinate to set for the seek position
	\param curDir Current direction vector used for path optimization
	\param seekPos Output parameter updated with the next optimal position in the path
	\return true if a path to the goal exists, false otherwise
*/
bool FindOptimalPath( const pathNode_t* root, const obstacle_t* obstacles, int numObstacles, const float height, const idVec3& curDir, idVec3& seekPos )
{
	int				  i, numPathPoints, bestNumPathPoints;
	const pathNode_t *node, *lastNode, *bestNode;
	idVec2			  optimizedPath[MAX_OBSTACLE_PATH];
	float			  pathLength, bestPathLength;
	bool			  pathToGoalExists, optimizedPathCalculated;

	seekPos.Zero();
	seekPos.z = height;

	pathToGoalExists		= false;
	optimizedPathCalculated = false;

	bestNode		  = root;
	bestNumPathPoints = 0;
	bestPathLength	  = idMath::INFINITUM;

	node = root;
	while( node ) {
		pathToGoalExists |= ( node->dist < 0.1f );

		if( node->dist <= bestNode->dist ) {
			if( idMath::Fabs( node->dist - bestNode->dist ) < 0.1f ) {
				if( !optimizedPathCalculated ) {
					bestNumPathPoints = OptimizePath( root, bestNode, obstacles, numObstacles, optimizedPath );
					bestPathLength	  = PathLength( optimizedPath, bestNumPathPoints, curDir.ToVec2() );
					seekPos.ToVec2()  = optimizedPath[1];
				}

				numPathPoints = OptimizePath( root, node, obstacles, numObstacles, optimizedPath );
				pathLength	  = PathLength( optimizedPath, numPathPoints, curDir.ToVec2() );

				if( pathLength < bestPathLength ) {
					bestNode		  = node;
					bestNumPathPoints = numPathPoints;
					bestPathLength	  = pathLength;
					seekPos.ToVec2()  = optimizedPath[1];
				}
				optimizedPathCalculated = true;

			} else {
				bestNode				= node;
				optimizedPathCalculated = false;
			}
		}

		if( node->children[0] ) {
			node = node->children[0];
		} else if( node->children[1] ) {
			node = node->children[1];
		} else {
			for( lastNode = node, node = node->parent; node; lastNode = node, node = node->parent ) {
				if( node->children[1] && node->children[1] != lastNode ) {
					node = node->children[1];
					break;
				}
			}
		}
	}

	if( root != NULL ) {
		if( !pathToGoalExists ) {
			if( root->children[0] != NULL ) {
				seekPos.ToVec2() = root->children[0]->pos;
			} else {
				seekPos.ToVec2() = root->pos;
			}
		} else if( !optimizedPathCalculated ) {
			OptimizePath( root, bestNode, obstacles, numObstacles, optimizedPath );
			seekPos.ToVec2() = optimizedPath[1];
		}

		if( ai_showObstacleAvoidance.GetBool() ) {
			idVec3 start, end;
			start.z = end.z = height + 4.0f;
			numPathPoints	= OptimizePath( root, bestNode, obstacles, numObstacles, optimizedPath );
			for( i = 0; i < numPathPoints - 1; i++ ) {
				start.ToVec2() = optimizedPath[i];
				end.ToVec2()   = optimizedPath[i + 1];
				gameRenderWorld->DebugArrow( colorCyan, start, end, 1 );
			}
		}
	}

	return pathToGoalExists;
}

bool idAI::FindPathAroundObstacles( const idPhysics* physics, const idAAS* aas, const idEntity* ignore, const idVec3& startPos, const idVec3& seekPos, obstaclePath_t& path )
{
	int			numObstacles, areaNum, insideObstacle;
	obstacle_t	obstacles[MAX_OBSTACLES];
	idBounds	clipBounds;
	idBounds	bounds;
	pathNode_t* root;
	bool		pathToGoalExists;

	path.seekPos				  = seekPos;
	path.firstObstacle			  = NULL;
	path.startPosOutsideObstacles = startPos;
	path.startPosObstacle		  = NULL;
	path.seekPosOutsideObstacles  = seekPos;
	path.seekPosObstacle		  = NULL;

	if( !aas ) {
		return true;
	}

	bounds[1]	= aas->GetSettings()->boundingBoxes[0][1];
	bounds[0]	= -bounds[1];
	bounds[1].z = 32.0f;

	// get the AAS area number and a valid point inside that area
	areaNum = aas->PointReachableAreaNum( path.startPosOutsideObstacles, bounds, ( AREA_REACHABLE_WALK | AREA_REACHABLE_FLY ) );
	aas->PushPointIntoAreaNum( areaNum, path.startPosOutsideObstacles );

	// get all the nearby obstacles
	numObstacles = GetObstacles( physics, aas, ignore, areaNum, path.startPosOutsideObstacles, path.seekPosOutsideObstacles, obstacles, MAX_OBSTACLES, clipBounds );

	// get a source position outside the obstacles
	GetPointOutsideObstacles( obstacles, numObstacles, path.startPosOutsideObstacles.ToVec2(), &insideObstacle, NULL );
	if( insideObstacle != -1 ) {
		path.startPosObstacle = obstacles[insideObstacle].entity;
	}

	// get a goal position outside the obstacles
	GetPointOutsideObstacles( obstacles, numObstacles, path.seekPosOutsideObstacles.ToVec2(), &insideObstacle, NULL );
	if( insideObstacle != -1 ) {
		path.seekPosObstacle = obstacles[insideObstacle].entity;
	}

	// if start and destination are pushed to the same point, we don't have a path around the obstacle
	if( ( path.seekPosOutsideObstacles.ToVec2() - path.startPosOutsideObstacles.ToVec2() ).LengthSqr() < Square( 1.0f ) ) {
		if( ( seekPos.ToVec2() - startPos.ToVec2() ).LengthSqr() > Square( 2.0f ) ) {
			return false;
		}
	}

	// build a path tree
	root = BuildPathTree( obstacles, numObstacles, clipBounds, path.startPosOutsideObstacles.ToVec2(), path.seekPosOutsideObstacles.ToVec2(), path );

	// draw the path tree
	if( ai_showObstacleAvoidance.GetBool() ) {
		DrawPathTree( root, physics->GetOrigin().z );
	}

	// prune the tree
	PrunePathTree( root, path.seekPosOutsideObstacles.ToVec2() );

	// find the optimal path
	pathToGoalExists = FindOptimalPath( root, obstacles, numObstacles, physics->GetOrigin().z, physics->GetLinearVelocity(), path.seekPos );

	// free the tree
	FreePathTree_r( root );

	return pathToGoalExists;
}

void idAI::FreeObstacleAvoidanceNodes()
{
	pathNodeAllocator.Shutdown();
}

/*
===============================================================================

	Path Prediction

	Uses the AAS to quickly and accurately predict a path for a certain
	period of time based on an initial position and velocity.

===============================================================================
*/

const float OVERCLIP		= 1.001f;
const int	MAX_FRAME_SLIDE = 5;

typedef struct pathTrace_s {
	float			fraction;
	idVec3			endPos;
	idVec3			normal;
	const idEntity* blockingEntity;
} pathTrace_t;

/*!
	\brief Performs a path trace from start to end position, checking for stop events and AAS traversal.

	This function traces a path from a starting position to an ending position, utilizing both AAS (Area Awareness System) for efficient pathfinding and traditional clip tracing for obstacle
   detection. It checks for specific stop events like entering ledge areas or obstacles and updates the trace and path information accordingly. The function returns true if a stop event was triggered,
   otherwise false. The AAS system is used when available and properly initialized; otherwise, it falls back to standard collision detection.

	\param ent The entity for which the path is being traced
	\param aas Pointer to the AAS system for area traversal information
	\param start The starting position of the trace
	\param end The ending position of the trace
	\param stopEvent Bitmask of stop events to check for during the trace
	\param trace Structure to store the trace results including fraction, end position, normal, and blocking entity
	\param path Structure to store the predicted path information when a stop event occurs
	\return True if a stop event was triggered, false otherwise
*/
bool PathTrace( const idEntity* ent, const idAAS* aas, const idVec3& start, const idVec3& end, int stopEvent, struct pathTrace_s& trace, predictedPath_t& path )
{
	trace_t	   clipTrace;
	aasTrace_t aasTrace;

	memset( &trace, 0, sizeof( trace ) );

	if( !aas || !aas->GetSettings() ) {
		gameLocal.clip.Translation( clipTrace, start, end, ent->GetPhysics()->GetClipModel(), ent->GetPhysics()->GetClipModel()->GetAxis(), MASK_MONSTERSOLID, ent );

		// NOTE: could do (expensive) ledge detection here for when there is no AAS file

		trace.fraction		 = clipTrace.fraction;
		trace.endPos		 = clipTrace.endpos;
		trace.normal		 = clipTrace.c.normal;
		trace.blockingEntity = gameLocal.entities[clipTrace.c.entityNum];
	} else {
		aasTrace.getOutOfSolid = true;
		if( stopEvent & SE_ENTER_LEDGE_AREA ) {
			aasTrace.flags |= AREA_LEDGE;
		}
		if( stopEvent & SE_ENTER_OBSTACLE ) {
			aasTrace.travelFlags |= TFL_INVALID;
		}

		aas->Trace( aasTrace, start, end );

		gameLocal.clip.TranslationEntities( clipTrace, start, aasTrace.endpos, ent->GetPhysics()->GetClipModel(), ent->GetPhysics()->GetClipModel()->GetAxis(), MASK_MONSTERSOLID, ent );

		if( clipTrace.fraction >= 1.0f ) {
			trace.fraction		 = aasTrace.fraction;
			trace.endPos		 = aasTrace.endpos;
			trace.normal		 = aas->GetPlane( aasTrace.planeNum ).Normal();
			trace.blockingEntity = gameLocal.world;

			if( aasTrace.fraction < 1.0f ) {
				if( stopEvent & SE_ENTER_LEDGE_AREA ) {
					if( aas->AreaFlags( aasTrace.blockingAreaNum ) & AREA_LEDGE ) {
						path.endPos			= trace.endPos;
						path.endNormal		= trace.normal;
						path.endEvent		= SE_ENTER_LEDGE_AREA;
						path.blockingEntity = trace.blockingEntity;

						if( ai_debugMove.GetBool() ) {
							gameRenderWorld->DebugLine( colorRed, start, aasTrace.endpos );
						}
						return true;
					}
				}
				if( stopEvent & SE_ENTER_OBSTACLE ) {
					if( aas->AreaTravelFlags( aasTrace.blockingAreaNum ) & TFL_INVALID ) {
						path.endPos			= trace.endPos;
						path.endNormal		= trace.normal;
						path.endEvent		= SE_ENTER_OBSTACLE;
						path.blockingEntity = trace.blockingEntity;

						if( ai_debugMove.GetBool() ) {
							gameRenderWorld->DebugLine( colorRed, start, aasTrace.endpos );
						}
						return true;
					}
				}
			}
		} else {
			trace.fraction		 = clipTrace.fraction;
			trace.endPos		 = clipTrace.endpos;
			trace.normal		 = clipTrace.c.normal;
			trace.blockingEntity = gameLocal.entities[clipTrace.c.entityNum];
		}
	}

	if( trace.fraction >= 1.0f ) {
		trace.blockingEntity = NULL;
	}

	return false;
}

bool idAI::PredictPath( const idEntity* ent, const idAAS* aas, const idVec3& start, const idVec3& velocity, int totalTime, int frameTime, int stopEvent, predictedPath_t& path )
{
	int			i, j, step, numFrames, curFrameTime;
	idVec3		delta, curStart, curEnd, curVelocity, lastEnd, stepUp, tmpStart;
	idVec3		gravity, gravityDir, invGravityDir;
	float		maxStepHeight, minFloorCos;
	pathTrace_t trace;

	if( aas && aas->GetSettings() ) {
		gravity		  = aas->GetSettings()->gravity;
		gravityDir	  = aas->GetSettings()->gravityDir;
		invGravityDir = aas->GetSettings()->invGravityDir;
		maxStepHeight = aas->GetSettings()->maxStepHeight;
		minFloorCos	  = aas->GetSettings()->minFloorCos;
	} else {
		gravity		  = DEFAULT_GRAVITY_VEC3;
		gravityDir	  = idVec3( 0, 0, -1 );
		invGravityDir = idVec3( 0, 0, 1 );
		maxStepHeight = 14.0f;
		minFloorCos	  = 0.7f;
	}

	path.endPos		 = start;
	path.endVelocity = velocity;
	path.endNormal.Zero();
	path.endEvent		= 0;
	path.endTime		= 0;
	path.blockingEntity = NULL;

	curStart	= start;
	curVelocity = velocity;

	// RB: fixed integer division by 0
	if( frameTime != 0 ) {
		numFrames = ( totalTime + frameTime - 1 ) / frameTime;
	} else {
		numFrames = ( totalTime + frameTime - 1 );
	}
	// RB end
	curFrameTime = frameTime;
	for( i = 0; i < numFrames; i++ ) {
		if( i == numFrames - 1 ) {
			curFrameTime = totalTime - i * curFrameTime;
		}

		delta = curVelocity * curFrameTime * 0.001f;

		path.endVelocity = curVelocity;
		path.endTime	 = i * frameTime;

		// allow sliding along a few surfaces per frame
		for( j = 0; j < MAX_FRAME_SLIDE; j++ ) {
			idVec3 lineStart = curStart;

			// allow stepping up three times per frame
			for( step = 0; step < 3; step++ ) {
				curEnd = curStart + delta;
				if( PathTrace( ent, aas, curStart, curEnd, stopEvent, trace, path ) ) {
					return true;
				}

				if( step ) {
					// step down at end point
					tmpStart = trace.endPos;
					curEnd	 = tmpStart - stepUp;
					if( PathTrace( ent, aas, tmpStart, curEnd, stopEvent, trace, path ) ) {
						return true;
					}

					// if not moved any further than without stepping up, or if not on a floor surface
					if( ( lastEnd - start ).LengthSqr() > ( trace.endPos - start ).LengthSqr() - 0.1f || ( trace.normal * invGravityDir ) < minFloorCos ) {
						if( stopEvent & SE_BLOCKED ) {
							path.endPos	  = lastEnd;
							path.endEvent = SE_BLOCKED;

							if( ai_debugMove.GetBool() ) {
								gameRenderWorld->DebugLine( colorRed, lineStart, lastEnd );
							}

							return true;
						}

						curStart = lastEnd;
						break;
					}
				}

				path.endNormal		= trace.normal;
				path.blockingEntity = trace.blockingEntity;

				// if the trace is not blocked or blocked by a floor surface
				if( trace.fraction >= 1.0f || ( trace.normal * invGravityDir ) > minFloorCos ) {
					curStart = trace.endPos;
					break;
				}

				// save last result
				lastEnd = trace.endPos;

				// step up
				stepUp = invGravityDir * maxStepHeight;
				if( PathTrace( ent, aas, curStart, curStart + stepUp, stopEvent, trace, path ) ) {
					return true;
				}
				stepUp *= trace.fraction;
				curStart = trace.endPos;
			}

			if( ai_debugMove.GetBool() ) {
				gameRenderWorld->DebugLine( colorRed, lineStart, curStart );
			}

			if( trace.fraction >= 1.0f ) {
				break;
			}

			delta.ProjectOntoPlane( trace.normal, OVERCLIP );
			curVelocity.ProjectOntoPlane( trace.normal, OVERCLIP );

			if( stopEvent & SE_BLOCKED ) {
				// if going backwards
				if( ( curVelocity - gravityDir * curVelocity * gravityDir ) * ( velocity - gravityDir * velocity * gravityDir ) < 0.0f ) {
					path.endPos	  = curStart;
					path.endEvent = SE_BLOCKED;

					return true;
				}
			}
		}

		if( j >= MAX_FRAME_SLIDE ) {
			if( stopEvent & SE_BLOCKED ) {
				path.endPos	  = curStart;
				path.endEvent = SE_BLOCKED;
				return true;
			}
		}

		// add gravity
		curVelocity += gravity * frameTime * 0.001f;
	}

	path.endTime	 = totalTime;
	path.endVelocity = curVelocity;
	path.endPos		 = curStart;
	path.endEvent	 = 0;

	return false;
}

int Ballistics( const idVec3& start, const idVec3& end, float speed, float gravity, ballistics_t bal[2] )
{
	int	  n, i;
	float x, y, a, b, c, d, sqrtd, inva, p[2];

	x = ( end.ToVec2() - start.ToVec2() ).Length();
	y = end[2] - start[2];

	a = 4.0f * y * y + 4.0f * x * x;
	b = -4.0f * speed * speed - 4.0f * y * gravity;
	c = gravity * gravity;

	d = b * b - 4.0f * a * c;
	if( d <= 0.0f || a == 0.0f ) {
		return 0;
	}
	sqrtd = idMath::Sqrt( d );
	inva  = 0.5f / a;
	p[0]  = ( -b + sqrtd ) * inva;
	p[1]  = ( -b - sqrtd ) * inva;
	n	  = 0;
	for( i = 0; i < 2; i++ ) {
		if( p[i] <= 0.0f ) {
			continue;
		}
		d			 = idMath::Sqrt( p[i] );
		bal[n].angle = atan2( 0.5f * ( 2.0f * y * p[i] - gravity ) / d, d * x );
		bal[n].time	 = x / ( cos( bal[n].angle ) * speed );
		bal[n].angle = idMath::AngleNormalize180( RAD2DEG( bal[n].angle ) );
		n++;
	}

	return n;
}

#if 0
// not used
/*
=====================
HeightForTrajectory

Returns the maximum hieght of a given trajectory
=====================
*/
static float HeightForTrajectory( const idVec3& start, float zVel, float gravity )
{
	float maxHeight, t;

	t = zVel / gravity;
	// maximum height of projectile
	maxHeight = start.z - 0.5f * gravity * ( t * t );

	return maxHeight;
}
#endif

bool idAI::TestTrajectory( const idVec3& start,
	const idVec3&						 end,
	float								 zVel,
	float								 gravity,
	float								 time,
	float								 max_height,
	const idClipModel*					 clip,
	int									 clipmask,
	const idEntity*						 ignore,
	const idEntity*						 targetEntity,
	int									 drawtime )
{
	int		i, numSegments;
	float	maxHeight, t, t2;
	idVec3	points[5];
	trace_t trace;
	bool	result;

	t = zVel / gravity;
	// maximum height of projectile
	maxHeight = start.z - 0.5f * gravity * ( t * t );
	// time it takes to fall from the top to the end height
	t = idMath::Sqrt( ( maxHeight - end.z ) / ( 0.5f * -gravity ) );

	// start of parabolic
	points[0] = start;

	if( t < time ) {
		numSegments = 4;
		// point in the middle between top and start
		t2				   = ( time - t ) * 0.5f;
		points[1].ToVec2() = start.ToVec2() + ( end.ToVec2() - start.ToVec2() ) * ( t2 / time );
		points[1].z		   = start.z + t2 * zVel + 0.5f * gravity * t2 * t2;
		// top of parabolic
		t2				   = time - t;
		points[2].ToVec2() = start.ToVec2() + ( end.ToVec2() - start.ToVec2() ) * ( t2 / time );
		points[2].z		   = start.z + t2 * zVel + 0.5f * gravity * t2 * t2;
		// point in the middel between top and end
		t2				   = time - t * 0.5f;
		points[3].ToVec2() = start.ToVec2() + ( end.ToVec2() - start.ToVec2() ) * ( t2 / time );
		points[3].z		   = start.z + t2 * zVel + 0.5f * gravity * t2 * t2;
	} else {
		numSegments = 2;
		// point halfway through
		t2				   = time * 0.5f;
		points[1].ToVec2() = start.ToVec2() + ( end.ToVec2() - start.ToVec2() ) * 0.5f;
		points[1].z		   = start.z + t2 * zVel + 0.5f * gravity * t2 * t2;
	}

	// end of parabolic
	points[numSegments] = end;

	if( drawtime ) {
		for( i = 0; i < numSegments; i++ ) {
			gameRenderWorld->DebugLine( colorRed, points[i], points[i + 1], drawtime );
		}
	}

	// make sure projectile doesn't go higher than we want it to go
	for( i = 0; i < numSegments; i++ ) {
		if( points[i].z > max_height ) {
			// goes higher than we want to allow
			return false;
		}
	}

	result = true;
	for( i = 0; i < numSegments; i++ ) {
		gameLocal.clip.Translation( trace, points[i], points[i + 1], clip, mat3_identity, clipmask, ignore );
		if( trace.fraction < 1.0f ) {
			if( gameLocal.GetTraceEntity( trace ) == targetEntity ) {
				result = true;
			} else {
				result = false;
			}
			break;
		}
	}

	if( drawtime ) {
		if( clip ) {
			gameRenderWorld->DebugBounds( result ? colorGreen : colorYellow, clip->GetBounds().Expand( 1.0f ), trace.endpos, drawtime );
		} else {
			idBounds bnds( trace.endpos );
			bnds.ExpandSelf( 1.0f );
			gameRenderWorld->DebugBounds( result ? colorGreen : colorYellow, bnds, vec3_zero, drawtime );
		}
	}

	return result;
}

bool idAI::PredictTrajectory( const idVec3& firePos,
	const idVec3&							target,
	float									projectileSpeed,
	const idVec3&							projGravity,
	const idClipModel*						clip,
	int										clipmask,
	float									max_height,
	const idEntity*							ignore,
	const idEntity*							targetEntity,
	int										drawtime,
	idVec3&									aimDir )
{
	int			 n, i, j;
	float		 zVel, a, t, pitch, s, c;
	trace_t		 trace;
	ballistics_t ballistics[2];
	idVec3		 dir[2];
	idVec3		 velocity;
	idVec3		 lastPos, pos;

	if( targetEntity == NULL ) {
		return false;
	}

	// check if the projectile starts inside the target
	if( targetEntity->GetPhysics()->GetAbsBounds().IntersectsBounds( clip->GetBounds().Translate( firePos ) ) ) {
		aimDir = target - firePos;
		aimDir.Normalize();
		return true;
	}

	// if no velocity or the projectile is not affected by gravity
	if( projectileSpeed <= 0.0f || projGravity == vec3_origin ) {
		aimDir = target - firePos;
		aimDir.Normalize();

		gameLocal.clip.Translation( trace, firePos, target, clip, mat3_identity, clipmask, ignore );

		if( drawtime ) {
			gameRenderWorld->DebugLine( colorRed, firePos, target, drawtime );
			idBounds bnds( trace.endpos );
			bnds.ExpandSelf( 1.0f );
			gameRenderWorld->DebugBounds( ( trace.fraction >= 1.0f || ( gameLocal.GetTraceEntity( trace ) == targetEntity ) ) ? colorGreen : colorYellow, bnds, vec3_zero, drawtime );
		}

		return ( trace.fraction >= 1.0f || ( gameLocal.GetTraceEntity( trace ) == targetEntity ) );
	}

	n = Ballistics( firePos, target, projectileSpeed, projGravity[2], ballistics );
	if( n == 0 ) {
		// there is no valid trajectory
		aimDir = target - firePos;
		aimDir.Normalize();
		return false;
	}

	// make sure the first angle is the smallest
	if( n == 2 ) {
		if( ballistics[1].angle < ballistics[0].angle ) {
			a					= ballistics[0].angle;
			ballistics[0].angle = ballistics[1].angle;
			ballistics[1].angle = a;
			t					= ballistics[0].time;
			ballistics[0].time	= ballistics[1].time;
			ballistics[1].time	= t;
		}
	}

	// test if there is a collision free trajectory
	for( i = 0; i < n; i++ ) {
		pitch = DEG2RAD( ballistics[i].angle );
		idMath::SinCos( pitch, s, c );
		dir[i]	 = target - firePos;
		dir[i].z = 0.0f;
		dir[i] *= c * idMath::InvSqrt( dir[i].LengthSqr() );
		dir[i].z = s;

		zVel = projectileSpeed * dir[i].z;

		if( ai_debugTrajectory.GetBool() ) {
			t		 = ballistics[i].time / 100.0f;
			velocity = dir[i] * projectileSpeed;
			lastPos	 = firePos;
			pos		 = firePos;
			for( j = 1; j < 100; j++ ) {
				pos += velocity * t;
				velocity += projGravity * t;
				gameRenderWorld->DebugLine( colorCyan, lastPos, pos );
				lastPos = pos;
			}
		}

		if( TestTrajectory( firePos, target, zVel, projGravity[2], ballistics[i].time, firePos.z + max_height, clip, clipmask, ignore, targetEntity, drawtime ) ) {
			aimDir = dir[i];
			return true;
		}
	}

	aimDir = dir[0];

	// there is no collision free trajectory
	return false;
}

void idAI::idle_followPathEntities( idEntity* pathnode )
{
	idStr	  nodeaction;
	idStr	  triggername;
	idEntity* triggerent;
	idEntity* current_path;
	idEntity* next_path;

	current_path = pathnode;
	do {
		next_path  = idPathCorner::RandomPath( current_path, NULL );
		nodeaction = current_path->GetKey( "classname" );
		if( scriptObject.GetFunction( nodeaction ) ) {
			// #ifdef _D3XP
			//  trigger an entity right when the path corner is accepted
			idStr	  pretriggername;
			idEntity* pretriggerent;
			pretriggername = current_path->GetKey( "pretrigger" );
			if( pretriggername != "" ) {
				pretriggerent = gameLocal.GetEntity( pretriggername );
				if( pretriggerent ) {
					pretriggerent->Event_ActivateTargets( this );
				}
			}
			// #endif

			//			Event_CallFunction(nodeaction);
			scriptThread->ClearStack();
			scriptThread->PushEntity( this );
			scriptThread->CallFunction( scriptObject.GetFunction( nodeaction ), false );
		} else {
			idLib::Warning( "'%s' encountered an unsupported path entity '%s' on entity '%s'\n", GetName(), nodeaction.c_str(), current_path->GetName() );
			return;
		}

		if( checkForEnemy( true ) ) {
			return;
		}

		// trigger any entities the path had targeted
		triggername = current_path->GetKey( "trigger" );
		if( triggername != "" ) {
			triggerent = gameLocal.GetEntity( triggername );
			if( triggerent ) {
				triggerent->Event_ActivateTargets( this );
			}
		}

		current_path = next_path;
	} while( !( !current_path ) );
}
// jmarshall end
