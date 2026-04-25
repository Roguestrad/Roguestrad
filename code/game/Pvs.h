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

#ifndef __GAME_PVS_H__
#define __GAME_PVS_H__

/*
===================================================================================

	PVS

	Note: mirrors and other special view portals are not taken into account

===================================================================================
*/

typedef struct pvsHandle_s {
	int			 i; // index to current pvs
	unsigned int h; // handle for current pvs
} pvsHandle_t;

typedef struct pvsCurrent_s {
	pvsHandle_t handle; // current pvs handle
	byte*		pvs;	// current pvs bit string
} pvsCurrent_t;

#define MAX_CURRENT_PVS 64 // must be a power of 2

typedef enum {
	PVS_NORMAL			 = 0, // PVS through portals taking portal states into account
	PVS_ALL_PORTALS_OPEN = 1, // PVS through portals assuming all portals are open
	PVS_CONNECTED_AREAS	 = 2  // PVS considering all topologically connected areas visible
} pvsType_t;

/*!
	\class idPVS
	\brief Manages potentially visible set calculations for determining which areas or objects are visible from a given position.

	The idPVS class implements a system for computing and managing potentially visible sets to optimize rendering by determining which areas or objects are visible from a specific source. It supports
   various methods for setting up PVS handles based on different source types including positions, bounds, and area numbers. The class provides functionality to check visibility for individual points,
   bounds, or area sets against a given PVS handle, and includes methods for visualizing the PVS results. The system handles multiple PVS types and supports merging of PVS handles. Memory management
   is handled through allocation and deallocation of PVS handles, with support for allocating handles for specific PVS entries. The implementation includes algorithms for computing visibility through
   portals and passages, supporting both recursive flooding and passage-based calculations. The class also includes utilities for creating and destroying portal and passage data structures, and
   provides methods for retrieving connected areas and checking portal sky visibility.

*/
class idPVS
{
public:
	//! Initializes a new instance of the idPVS class.
	idPVS();

	//! Destructor for the idPVS class that shuts down the PVS system.
	~idPVS();

	//! Initializes the PVS system for the current map.
	void		Init();

	//! Deallocates all memory resources used by the PVS system.
	void		Shutdown();

	//! Returns the area number that contains the specified point.
	int			GetPVSArea( const idVec3& point ) const;

	//! Returns the number of PVS areas that intersect with the given bounds.
	int			GetPVSAreas( const idBounds& bounds, int* areas, int maxAreas ) const;

	//! Sets up the current potentially visible set for the given source position and PVS type.
	pvsHandle_t SetupCurrentPVS( const idVec3& source, const pvsType_t type = PVS_NORMAL ) const;

	//! Sets up the PVS for the given source bounds and PVS type.
	pvsHandle_t SetupCurrentPVS( const idBounds& source, const pvsType_t type = PVS_NORMAL ) const;

	//! Sets up and returns a PVS handle for the specified source area and PVS type
	pvsHandle_t SetupCurrentPVS( const int sourceArea, const pvsType_t type = PVS_NORMAL ) const;

	//! Sets up and returns a handle to the potentially visible set for the given source areas and PVS type.
	pvsHandle_t SetupCurrentPVS( const int* sourceAreas, const int numSourceAreas, const pvsType_t type = PVS_NORMAL ) const;

	//! Merges two PVS handles into a new PVS handle using a bitwise OR operation
	pvsHandle_t MergeCurrentPVS( pvsHandle_t pvs1, pvsHandle_t pvs2 ) const;

	//! Frees a PVS handle that was previously allocated
	void		FreeCurrentPVS( pvsHandle_t handle ) const;

	//! Returns true if the target is within the current PVS.
	bool		InCurrentPVS( const pvsHandle_t handle, const idVec3& target ) const;

	//! Checks if the given target bounds are visible from the current PVS handle.
	bool		InCurrentPVS( const pvsHandle_t handle, const idBounds& target ) const;

	//! Checks if a target area is visible from a given PVS handle
	bool		InCurrentPVS( const pvsHandle_t handle, const int targetArea ) const;

	//! Checks if the specified target areas are visible from the given PVS handle.
	bool		InCurrentPVS( const pvsHandle_t handle, const int* targetAreas, int numTargetAreas ) const;

	//! Draws all portals within the PVS of the given source position.
	void		DrawPVS( const idVec3& source, const pvsType_t type = PVS_NORMAL ) const;

	//! Draws the potentially visible set for a given source bounds and type.
	void		DrawPVS( const idBounds& source, const pvsType_t type = PVS_NORMAL ) const;

	//! Visualizes the PVS the handle points to
	void		DrawCurrentPVS( const pvsHandle_t handle, const idVec3& source ) const;

	//! Checks if the given origin is visible from the specified PVS handle with portal sky.
	bool		CheckAreasForPortalSky( const pvsHandle_t handle, const idVec3& origin );

private:
	int					 numAreas;
	int					 numPortals;
	bool*				 connectedAreas;
	int*				 areaQueue;
	byte*				 areaPVS;
	// current PVS for a specific source possibly taking portal states (open/closed) into account
	mutable pvsCurrent_t currentPVS[MAX_CURRENT_PVS];
	// used to create PVS
	int					 portalVisBytes;
	int					 portalVisLongs; // DG: Note: these are really ints now..
	int					 areaVisBytes;
	int					 areaVisLongs; // DG: Note: these are really ints now..
	struct pvsPortal_s*	 pvsPortals;
	struct pvsArea_s*	 pvsAreas;

private:
	//! Returns the total number of portals in all areas of the render world.
	int				   GetPortalCount() const;

	//! Initializes and allocates data structures for potential visibility system calculations
	void			   CreatePVSData();

	//! Frees all memory allocated for the PVS data structures.
	void			   DestroyPVSData();

	//! Copies portal visibility data to might see data for all portals.
	void			   CopyPortalPVSToMightSee() const;

	//! Recursively floods PVS through portal frontiers starting from a given portal and area.
	void			   FloodFrontPortalPVS_r( struct pvsPortal_s* portal, int areaNum ) const;

	//! Computes potentially visible sets for portal fronts in the PVS system
	void			   FrontPortalPVS() const;

	//! Recursively floods through PVS passages to determine visibility between portals
	struct pvsStack_s* FloodPassagePVS_r( struct pvsPortal_s* source, const struct pvsPortal_s* portal, struct pvsStack_s* prevStack ) const;

	//! Calculates portal PVS by flooding through passages.
	void			   PassagePVS() const;

	/*!
		\brief Adds passage boundaries between source and pass windings by computing separating planes

		This function computes separating planes between two windings to determine passage boundaries in a visibility processing system. It iterates through vertices of both windings to find valid
	   separating planes that put all source vertices on one side and all pass vertices on the other side. The function supports flipping the clipping normal and checks for duplicate planes to avoid
	   redundant boundaries. The computed planes are stored in the bounds array up to the maximum allowed bounds.

		\param source The source winding for boundary calculation
		\param pass The pass winding for boundary calculation
		\param flipClip Flag indicating whether to flip the clip normal
		\param bounds Array to store the computed boundary planes
		\param numBounds Reference to the number of computed bounds
		\param maxBounds Maximum number of bounds that can be stored
		\throws Warning message when maximum passage boundaries are exceeded
	*/
	void			   AddPassageBoundaries( const idWinding& source, const idWinding& pass, bool flipClip, idPlane* bounds, int& numBounds, int maxBounds ) const;

	//! Creates passages for the PVS system by calculating visibility between portals.
	void			   CreatePassages() const;

	//! Destroys all passage data associated with the portals in the PVS.
	void			   DestroyPassages() const;

	//! Computes the total number of visible areas based on portal visibility information.
	int				   AreaPVSFromPortalPVS() const;

	//! Fills a boolean array with connected areas reachable from a source area, excluding areas blocked by view-blocking portals.
	void			   GetConnectedAreas( int srcArea, bool* connectedAreas ) const;

	//! Allocates and returns a handle for a PVS entry.
	pvsHandle_t		   AllocCurrentPVS( unsigned int h ) const;
};

#endif /* !__GAME_PVS_H__ */
