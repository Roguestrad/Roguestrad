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

#ifndef __PLANESET_H__
#define __PLANESET_H__

/*!
	\class idPlaneSet
	\brief A set implementation for managing planes with efficient lookup and insertion operations.

	This class provides a specialized container for managing a collection of planes, extending idList to include hash table based lookup for efficient plane finding and insertion. The design enables
   duplicate detection and automatic insertion of plane opposites to maintain consistency in plane representations. The hash table implementation allows for fast lookups when searching for existing
   planes within specified tolerance values for both normal vectors and distances. The class is designed for use in geometric computations where plane sets need to be efficiently managed and queried.
   The FindPlane method handles both searching for existing planes and adding new ones, ensuring that for each plane added, its opposite is also maintained in the set.

*/
class idPlaneSet : public idList<idPlane>
{
public:
	//! Clears all elements from the plane list and frees the hash table.
	void Clear()
	{
		idList<idPlane>::Clear();
		hash.Free();
	}

	/*!
		\brief Finds an existing plane in the set or adds a new one, returning its index.

		This function searches for a plane in the set that matches the given plane within the specified normal and distance tolerances. If no match is found, it adds the plane and its opposite to the
	   set. The function uses a hash table for efficient lookup and handles plane types specially by adding both the plane and its negation to maintain consistency. The normalEps and distEps
	   parameters control the tolerance for plane comparison, with distEps having an assertion that it must be less than or equal to 0.125f.

		\param plane The plane to find or add to the set
		\param normalEps Epsilon value for normal vector comparison
		\param distEps Epsilon value for distance comparison
		\return The index of the found or newly added plane in the set
		\throws assertion failure if distEps is greater than 0.125f
	*/
	int FindPlane( const idPlane& plane, const float normalEps, const float distEps );

private:
	idHashIndex hash;
};

ID_INLINE int idPlaneSet::FindPlane( const idPlane& plane, const float normalEps, const float distEps )
{
	int i, border, hashKey;

	assert( distEps <= 0.125f );

	hashKey = ( int )( idMath::Fabs( plane.Dist() ) * 0.125f );
	for( border = -1; border <= 1; border++ ) {
		for( i = hash.First( hashKey + border ); i >= 0; i = hash.Next( i ) ) {
			if( ( *this )[i].Compare( plane, normalEps, distEps ) ) { return i; }
		}
	}

	if( plane.Type() >= PLANETYPE_NEGX && plane.Type() < PLANETYPE_TRUEAXIAL ) {
		Append( -plane );
		hash.Add( hashKey, Num() - 1 );
		Append( plane );
		hash.Add( hashKey, Num() - 1 );
		return ( Num() - 1 );
	} else {
		Append( plane );
		hash.Add( hashKey, Num() - 1 );
		Append( -plane );
		hash.Add( hashKey, Num() - 1 );
		return ( Num() - 2 );
	}
}

#endif /* !__PLANESET_H__ */
