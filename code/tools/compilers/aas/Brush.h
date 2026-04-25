/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU
General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __BRUSH_H__
#define __BRUSH_H__

/*
===============================================================================

	Brushes

===============================================================================
*/

#define BRUSH_PLANESIDE_FRONT  1
#define BRUSH_PLANESIDE_BACK   2
#define BRUSH_PLANESIDE_BOTH   ( BRUSH_PLANESIDE_FRONT | BRUSH_PLANESIDE_BACK )
#define BRUSH_PLANESIDE_FACING 4

class idBrush;
class idBrushList;

void DisplayRealTimeString( const char* string, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

//===============================================================
//
//	idBrushSide
//
//===============================================================

#define SFL_SPLIT			0x0001
#define SFL_BEVEL			0x0002
#define SFL_USED_SPLITTER	0x0004
#define SFL_TESTED_SPLITTER 0x0008

/*!
	\class idBrushSide
	\brief Represents a single side of a brush geometry element with associated plane, winding, and flags.

	This class encapsulates the geometric and topological properties of a single side of a brush, including its plane equation, winding, and various flags. It provides methods for flag manipulation,
   plane access, and geometric operations such as copying and splitting. The winding memory is managed through the destructor which ensures proper cleanup. The class supports creating copies of brush
   sides and splitting them along a given plane, which is useful for BSP construction and geometric operations. The plane number and flags are used to maintain connectivity and attribute information
   for the brush side.

*/
class idBrushSide
{
	friend class idBrush;

public:
	//! Initializes a new instance of the idBrushSide class with default values.
	idBrushSide();

	//! Constructs an idBrushSide object with the specified plane and plane number.
	idBrushSide( const idPlane& plane, int planeNum );

	//! Destructor for idBrushSide that releases the associated winding memory.
	~idBrushSide();

	//! Returns the flags associated with this brush side
	int				 GetFlags() const { return flags; }

	//! Sets the specified flag on the brush side.
	void			 SetFlag( int flag ) { flags |= flag; }

	//! Removes a specified flag from the brush side flags.
	void			 RemoveFlag( int flag ) { flags &= ~flag; }

	//! Returns the plane equation of the brush side.
	const idPlane&	 GetPlane() const { return plane; }

	//! Sets the plane number for this brush side.
	void			 SetPlaneNum( int num ) { planeNum = num; }

	//! Returns the plane number associated with this brush side
	int				 GetPlaneNum() { return planeNum; }

	//! Returns the winding associated with this brush side.
	const idWinding* GetWinding() const { return winding; }

	//! Creates and returns a copy of this brush side instance
	idBrushSide*	 Copy() const;

	//! Splits this brush side using the given plane into front and back sides.
	int				 Split( const idPlane& splitPlane, idBrushSide** front, idBrushSide** back ) const;

private:
	int		   flags;
	int		   planeNum;
	idPlane	   plane;
	idWinding* winding;
};

//===============================================================
//
//	idBrush
//
//===============================================================

#define BFL_NO_VALID_SPLITTERS 0x0001

/*!
	\class idBrush
	\brief Represents a brush volume with sides, bounds, and geometric operations.

	The idBrush class encapsulates a volumetric brush used for level construction and collision detection. It maintains a collection of brush sides that define its geometric structure and provides
   methods for manipulating and transforming the brush in 3D space. The class supports various geometric operations including boolean operations, splitting, merging, and transformation. Each brush has
   associated flags, contents, and entity identifiers that define its properties and behavior within the level. The class manages its internal brush sides and windings, and provides functionality for
   creating, modifying, and evaluating brush geometry, including volume calculations and bounding box computations. The design supports both simple geometric construction from bounds or windings, as
   well as complex operations such as boolean subtraction and merging with other brushes.

*/
class idBrush
{
	friend class idBrushList;

public:
	//! Initializes a new instance of the idBrush class with default values.
	idBrush();

	//! Destructor for idBrush that cleans up all allocated brush sides.
	~idBrush();

	//! Returns the flags associated with this brush
	int				GetFlags() const { return flags; }

	//! Sets the specified flag in the brush's flags.
	void			SetFlag( int flag ) { flags |= flag; }

	//! Removes a specified flag from the brush flags.
	void			RemoveFlag( int flag ) { flags &= ~flag; }

	//! Sets the entity number for this brush.
	void			SetEntityNum( int num ) { entityNum = num; }

	//! Sets the primitive number for the brush.
	void			SetPrimitiveNum( int num ) { primitiveNum = num; }

	//! Sets the contents of the brush to the specified value.
	void			SetContents( int contents ) { this->contents = contents; }

	//! Returns the contents value stored in the idBrush object
	int				GetContents() const { return contents; }

	//! Returns the bounding box of the brush.
	const idBounds& GetBounds() const { return bounds; }

	//! Calculates and returns the volume of the brush by computing tetrahedron volumes from its windings.
	float			GetVolume() const;

	//! Returns the number of sides in the brush.
	int				GetNumSides() const { return sides.Num(); }

	//! Returns the brush side at the specified index
	idBrushSide*	GetSide( int i ) const { return sides[i]; }

	//! Sets the plane side indicator for the brush.
	void			SetPlaneSide( int s ) { planeSide = s; }

	//! Saves the current plane side state to a member variable.
	void			SavePlaneSide() { savedPlaneSide = planeSide; }

	//! Returns the saved plane side value stored in the brush.
	int				GetSavedPlaneSide() const { return savedPlaneSide; }

	//! Initializes the brush from a list of sides and creates windings for the brush.
	bool			FromSides( idList<idBrushSide*>& sideList );

	//! Creates a brush from a winding and a plane, returning true if successful.
	bool			FromWinding( const idWinding& w, const idPlane& windingPlane );

	//! Initializes the brush sides from the given bounding box.
	bool			FromBounds( const idBounds& bounds );

	//! Applies a transformation to the brush by rotating and translating its planes.
	void			Transform( const idVec3& origin, const idMat3& axis );

	//! Creates a deep copy of the brush object
	idBrush*		Copy() const;

	//! Attempts to merge another brush with this brush if they share a separating plane and all winding points are properly aligned.
	bool			TryMerge( const idBrush* brush, const idPlaneSet& planeList );

	//! Performs a boolean subtraction operation between this brush and another brush, returning the resulting brush fragments.
	bool			Subtract( const idBrush* b, idBrushList& list ) const;

	/*!
		\brief Splits the brush into front and back parts based on the provided plane.

		This function divides a brush into two parts, front and back, relative to a given plane. It returns an integer indicating the side relationship of the brush to the plane. If both front and
	   back pointers are provided, the original brush is split into two new brushes. The function handles edge cases where the brush is entirely on one side of the plane or when the split results in a
	   cross side. The function also manages winding clipping and updates the brush sides appropriately.

		\param plane The plane to split the brush with
		\param planeNum The number identifying the plane
		\param front Pointer to store the front brush part, or NULL if not needed
		\param back Pointer to store the back brush part, or NULL if not needed
		\return Integer representing the side relationship: PLANESIDE_FRONT if the brush is entirely in front of the plane, PLANESIDE_BACK if entirely behind, or PLANESIDE_CROSS if it crosses the
	   plane.
	*/
	int				Split( const idPlane& plane, int planeNum, idBrush** front, idBrush** back ) const;

	//! Expands the brush to accommodate an axial bounding box by adjusting side planes and recreating windings.
	void			ExpandForAxialBox( const idBounds& bounds );

	//! Returns the next brush in the linked list.
	idBrush*		Next() const { return next; }

private:
	mutable idBrush*	 next;			 // next brush in list
	int					 entityNum;		 // entity number in editor
	int					 primitiveNum;	 // primitive number in editor
	int					 flags;			 // brush flags
	bool				 windingsValid;	 // set when side windings are valid
	int					 contents;		 // contents of brush
	int					 planeSide;		 // side of a plane this brush is on
	int					 savedPlaneSide; // saved plane side
	idBounds			 bounds;		 // brush bounds
	idList<idBrushSide*> sides;			 // list with sides

private:
	//! Creates windings for all brush sides by clipping them against each other and validates the resulting bounds.
	bool CreateWindings();

	//! Sets the bounding box of the brush based on its windings.
	void BoundBrush( const idBrush* original = NULL );

	//! Adds bevel planes to the brush for axial box alignment.
	void AddBevelsForAxialBox();

	//! Removes brush sides that do not have a valid winding and returns true if at least four sides remain.
	bool RemoveSidesWithoutWinding();
};

/*!
	\class idBrushList
	\brief A container for managing a collection of brushes with various operations for manipulation and organization.

	This class provides a linked list implementation for managing brushes, supporting operations such as adding, removing, and splitting brushes. It maintains internal pointers to the head and tail of
   the list, allowing efficient insertion at both ends. The class supports various transformations including splitting brushes relative to a plane, merging adjacent brushes, and chopping brushes based
   on custom criteria. It also provides utility functions for retrieving bounds, checking emptiness, and generating brush map files. The design enables efficient batch operations on multiple brushes
   and supports both direct manipulation and more complex geometric operations like splitting and merging.

*/
class idBrushList
{
public:
	//! Initializes an empty brush list with zero brushes and NULL head and tail pointers.
	idBrushList();
	~idBrushList();

	//! Returns the number of brushes in the brush list.
	int		 Num() const { return numBrushes; }

	//! Returns the number of brush sides in the brush list.
	int		 NumSides() const { return numBrushSides; }

	//! Returns the first brush in the brush list.
	idBrush* Head() const { return head; }

	//! Returns the last brush in the brush list.
	idBrush* Tail() const { return tail; }

	//! Clears all brushes from the brush list.
	void	 Clear()
	{
		head = tail = NULL;
		numBrushes	= 0;
	}

	//! Checks if the brush list is empty.
	bool		 IsEmpty() const { return ( numBrushes == 0 ); }

	//! Returns the bounding box that encompasses all brushes in the list.
	idBounds	 GetBounds() const;

	//! Adds a brush to the tail of the brush list.
	void		 AddToTail( idBrush* brush );

	//! Adds all brushes from the provided list to the end of this list.
	void		 AddToTail( idBrushList& list );

	//! Adds a brush to the front of the brush list.
	void		 AddToFront( idBrush* brush );

	//! Adds all brushes from the provided list to the front of this list.
	void		 AddToFront( idBrushList& list );

	//! Removes the specified brush from the list
	void		 Remove( idBrush* brush );

	//! Removes a brush from the list and deletes it.
	void		 Delete( idBrush* brush );

	//! Returns a copy of the brush list.
	idBrushList* Copy() const;

	//! Frees all brushes in the brush list by deleting each brush node and clearing the list pointers.
	void		 Free();

	/*!
		\brief Splits brushes in the list into two separate lists based on their position relative to a given plane.

		This function divides all brushes in the current list into two new lists: one containing brushes that are in front of the plane, and another containing brushes that are behind the plane. If
	   the useBrushSavedPlaneSide parameter is true, it uses previously saved plane side information to avoid splitting brushes that are completely on one side. The function handles both cases where
	   brushes are split and where they are fully on one side of the plane, adding the appropriate copies to the front or back lists.

		\param plane The plane to split the brushes against
		\param planeNum The index number of the plane, used for optimization or identification
		\param frontList Output list containing brushes that are in front of the plane
		\param backList Output list containing brushes that are behind the plane
		\param useBrushSavedPlaneSide If true, uses previously saved plane side information to avoid unnecessary splits
	*/
	void		 Split( const idPlane& plane, int planeNum, idBrushList& frontList, idBrushList& backList, bool useBrushSavedPlaneSide = false );

	//! Chops brushes in the list using the provided chop allowance function to determine which brushes can interact.
	void		 Chop( bool ( *ChopAllowed )( idBrush* b1, idBrush* b2 ) );

	//! Merges brushes in the list based on a provided merge allowance function.
	void		 Merge( bool ( *MergeAllowed )( idBrush* b1, idBrush* b2 ) );

	//! Sets the given flag on all brush sides facing the specified plane.
	void		 SetFlagOnFacingBrushSides( const idPlane& plane, int flag );

	//! Creates a list of planes for all brushes in the list.
	void		 CreatePlaneList( idPlaneSet& planeList ) const;

	//! Writes a brush map file containing the brushes in this list
	void		 WriteBrushMap( const idStr& fileName, const idStr& ext ) const;

private:
	idBrush* head;
	idBrush* tail;
	int		 numBrushes;
	int		 numBrushSides;
};

/*!
	\class idBrushMap
	\brief A class for writing brush-based map data to a file.
*/
class idBrushMap
{
public:
	//! Constructs an idBrushMap object and initializes it for writing map data to a file
	idBrushMap( const idStr& fileName, const idStr& ext );

	//! Destructor for the idBrushMap class that closes the associated file pointer.
	~idBrushMap();

	//! Sets the texture name for the brush map.
	void SetTexture( const idStr& textureName ) { texture = textureName; }

	//! Writes a brush definition to the output file.
	void WriteBrush( const idBrush* brush );

	//! Writes all brushes from the provided brush list to the output file.
	void WriteBrushList( const idBrushList& brushList );

private:
	idFile* fp;
	idStr	texture;
	int		brushCount;
};

#endif /* !__BRUSH_H__ */
