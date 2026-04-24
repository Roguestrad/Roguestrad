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
#ifndef IDRECTANGLE_H_
#define IDRECTANGLE_H_
//
// simple rectangle
//
extern void RotateVector( idVec3& v, idVec3 origin, float a, float c, float s );

/*!
	\class idRectangle
	\brief A class representing a 2D rectangle with position, width, and height.

	The idRectangle class encapsulates a 2D rectangle defined by its top-left corner coordinates, width, and height. It provides constructors for initializing rectangles with specific values, as well
   as methods for geometric operations such as offsetting, clipping, and rotation. The class supports various arithmetic operators for component-wise operations and provides access to rectangle data
   through indexed access and vector conversion. The Empty method appears to reset the rectangle to zero dimensions, though its precise behavior requires clarification. The class is designed for
   efficient manipulation and comparison of rectangular regions in 2D space.

*/
class idRectangle
{
public:
	float x; // horiz position
	float y; // vert position
	float w; // width
	float h; // height;

	//! Initializes a new idRectangle instance with all coordinates and dimensions set to zero.
	idRectangle() { x = y = w = h = 0.0; }

	/*!
		\brief Constructs an idRectangle object with specified position and dimensions.

		This constructor initializes the rectangle's position and size using the provided x, y, width, and height values. The x and y coordinates represent the top-left corner of the rectangle, while
	   w and h represent the width and height respectively.

		\param ix The x-coordinate of the rectangle's top-left corner
		\param iy The y-coordinate of the rectangle's top-left corner
		\param iw The width of the rectangle
		\param ih The height of the rectangle
	*/
	idRectangle( float ix, float iy, float iw, float ih )
	{
		x = ix;
		y = iy;
		w = iw;
		h = ih;
	}

	//! Returns the bottom coordinate of the rectangle.
	float Bottom() const { return y + h; }

	//! Returns the right edge coordinate of the rectangle.
	float Right() const { return x + w; }

	//! Moves the rectangle's position by the specified x and y offsets.
	void  Offset( float x, float y )
	{
		this->x += x;
		this->y += y;
	}

	//! Checks if the given point is contained within the rectangle.
	bool Contains( float xt, float yt )
	{
		if( w == 0.0 && h == 0.0 ) { return false; }
		if( xt >= x && xt <= Right() && yt >= y && yt <= Bottom() ) { return true; }
		return false;
	}
	void Empty() { x = y = w = h = 0.0; };

	//! Clips this rectangle against another rectangle, adjusting position and size based on the sizeOnly flag.
	void ClipAgainst( idRectangle r, bool sizeOnly )
	{
		if( !sizeOnly ) {
			if( x < r.x ) {
				w -= r.x - x;
				x = r.x;
			}
			if( y < r.y ) {
				h -= r.y - y;
				y = r.y;
			}
		}
		if( x + w > r.x + r.w ) { w = ( r.x + r.w ) - x; }
		if( y + h > r.y + r.h ) { h = ( r.y + r.h ) - y; }
	}

	//! Rotates the rectangle by the specified angle and stores the result in the output rectangle.
	void Rotate( float a, idRectangle& out )
	{
		idVec3 p1, p2, p3, p4, p5;
		float  c, s;
		idVec3 center;
		center.Set( ( x + w ) / 2.0, ( y + h ) / 2.0, 0 );
		p1.Set( x, y, 0 );
		p2.Set( Right(), y, 0 );
		p4.Set( x, Bottom(), 0 );
		if( a ) {
			s = sin( DEG2RAD( a ) );
			c = cos( DEG2RAD( a ) );
		} else {
			s = c = 0;
		}
		RotateVector( p1, center, a, c, s );
		RotateVector( p2, center, a, c, s );
		RotateVector( p4, center, a, c, s );
		out.x = p1.x;
		out.y = p1.y;
		out.w = ( p2 - p1 ).Length();
		out.h = ( p4 - p1 ).Length();
	}

	//! Adds the coordinates and dimensions of another rectangle to this rectangle and returns a reference to this rectangle.
	idRectangle&  operator+=( const idRectangle& a );

	//! Subtracts the components of another rectangle from this rectangle and returns a reference to this rectangle.
	idRectangle&  operator-=( const idRectangle& a );

	//! Divides the coordinates of this rectangle by the corresponding coordinates of another rectangle.
	idRectangle&  operator/=( const idRectangle& a );

	//! Divides all components of the rectangle by the given scalar value.
	idRectangle&  operator/=( const float a );

	//! Multiplies all components of the rectangle by a scalar value and returns a reference to the modified rectangle.
	idRectangle&  operator*=( const float a );

	//! Assignment operator that copies values from a vector to the rectangle
	idRectangle&  operator=( const idVec4 v );

	//! Compares two idRectangle objects for equality and returns true if all components (x, y, w, h) are equal.
	int			  operator==( const idRectangle& a ) const;

	//! Provides indexed access to the rectangle's coordinates
	float&		  operator[]( const int index );

	//! Returns a string representation of the rectangle coordinates.
	char*		  String() const;

	//! Returns a const reference to the rectangle data as an idVec4.
	const idVec4& ToVec4() const;
};

ID_INLINE const idVec4& idRectangle::ToVec4() const
{
	return *reinterpret_cast<const idVec4*>( &x );
}

ID_INLINE idRectangle& idRectangle::operator+=( const idRectangle& a )
{
	x += a.x;
	y += a.y;
	w += a.w;
	h += a.h;

	return *this;
}

ID_INLINE idRectangle& idRectangle::operator/=( const idRectangle& a )
{
	x /= a.x;
	y /= a.y;
	w /= a.w;
	h /= a.h;

	return *this;
}

ID_INLINE idRectangle& idRectangle::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	w *= inva;
	h *= inva;

	return *this;
}

ID_INLINE idRectangle& idRectangle::operator-=( const idRectangle& a )
{
	x -= a.x;
	y -= a.y;
	w -= a.w;
	h -= a.h;

	return *this;
}

ID_INLINE idRectangle& idRectangle::operator*=( const float a )
{
	x *= a;
	y *= a;
	w *= a;
	h *= a;

	return *this;
}

ID_INLINE idRectangle& idRectangle::operator=( const idVec4 v )
{
	x = v.x;
	y = v.y;
	w = v.z;
	h = v.w;
	return *this;
}

ID_INLINE int idRectangle::operator==( const idRectangle& a ) const
{
	return ( x == a.x && y == a.y && w == a.w && a.h );
}

ID_INLINE float& idRectangle::operator[]( int index )
{
	return ( &x )[index];
}

/*!
	\class idRegion
	\brief A collection of rectangular regions used for area-based operations.
*/
class idRegion
{
public:
	idRegion() {};

	//! Clears all rectangles from the region.
	void Empty() { rects.Clear(); }

	//! Checks if the given coordinates are within any of the rectangles in the region.
	bool Contains( float xt, float yt )
	{
		int c = rects.Num();
		for( int i = 0; i < c; i++ ) {
			if( rects[i].Contains( xt, yt ) ) { return true; }
		}
		return false;
	}

	/*!
		\brief Adds a rectangular region to the region collection.

		This function appends a new rectangle defined by its top-left corner coordinates (x, y) and its width (w) and height (h) to the internal collection of rectangles that make up the region.

		\param x The x-coordinate of the top-left corner of the rectangle
		\param y The y-coordinate of the top-left corner of the rectangle
		\param w The width of the rectangle
		\param h The height of the rectangle
	*/
	void		 AddRect( float x, float y, float w, float h ) { rects.Append( idRectangle( x, y, w, h ) ); }

	//! Returns the number of rectangles in the region.
	int			 GetRectCount() { return rects.Num(); }

	//! Returns a pointer to the rectangle at the specified index in the region.
	idRectangle* GetRect( int index )
	{
		if( index >= 0 && index < rects.Num() ) { return &rects[index]; }
		return NULL;
	}

protected:
	idList<idRectangle, TAG_OLD_UI> rects;
};

#endif
