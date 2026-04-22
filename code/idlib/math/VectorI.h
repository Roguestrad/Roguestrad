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
#ifndef __MATH_VECTORI_H__
#define __MATH_VECTORI_H__

//! Returns the smaller of two integer values.
static ID_INLINE int MinInt( int a, int b )
{
	return ( a ) < ( b ) ? ( a ) : ( b );
}

//! Returns the greater of two integer values.
static ID_INLINE int MaxInt( int a, int b )
{
	return ( a ) < ( b ) ? ( b ) : ( a );
}

/*!
	\class idVec2i
	\brief A 2D vector class for integer coordinates with various arithmetic and bitwise operations.

	This class represents a two-dimensional vector using integer coordinates and provides a comprehensive set of operators for arithmetic, bitwise, and comparison operations. It supports both
   component-wise operations and scalar operations, making it suitable for handling discrete 2D data such as pixel coordinates, grid positions, or bounded integer vectors. The class includes operators
   for basic arithmetic, bitwise manipulation, and comparison, enabling flexible vector computations in integer domains. It is designed to be used with integer values and provides both const and
   non-const access to its components through operator[], allowing both reading and modification of individual elements. The class follows standard C++ operator overloading conventions to enable
   intuitive mathematical expressions, supporting both prefix and postfix operations where applicable. The operators are implemented to work with both scalar values and other vectors, providing a
   flexible interface for various computational tasks.

*/
class idVec2i
{
public:
	int x, y;

	//! Default constructor for idVec2i that initializes an empty vector.
	idVec2i() { }

	//! Constructs an idVec2i object with the specified x and y coordinates.
	idVec2i( int _x, int _y ) :
		x( _x ),
		y( _y )
	{
	}

	//! Sets the x and y coordinates of the vector.
	void Set( int _x, int _y )
	{
		x = _x;
		y = _y;
	}
	int	 Area() const { return x * y; };

	//! Computes the component-wise minimum of this vector and the given vector.
	void Min( idVec2i& v )
	{
		x = MinInt( x, v.x );
		y = MinInt( y, v.y );
	}

	//! Sets this vector's components to the maximum of this vector's components and the given vector's components.
	void Max( idVec2i& v )
	{
		x = MaxInt( x, v.x );
		y = MaxInt( y, v.y );
	}

	//! Returns the x or y component of the vector based on the index
	int operator[]( const int index ) const
	{
		assert( index == 0 || index == 1 );
		return ( &x )[index];
	}

	//! Returns a reference to the x or y component of the vector based on the provided index.
	int& operator[]( const int index )
	{
		assert( index == 0 || index == 1 );
		return ( &x )[index];
	}

	//! Returns a new vector with negated x and y components.
	idVec2i	 operator-() const { return idVec2i( -x, -y ); }

	//! Returns a new vector with the boolean negation of each component of this vector.
	idVec2i	 operator!() const { return idVec2i( !x, !y ); }

	//! Returns a new vector with each component right-shifted by the specified amount.
	idVec2i	 operator>>( const int a ) const { return idVec2i( x >> a, y >> a ); }

	//! Returns a new idVec2i with both components left-shifted by the specified amount.
	idVec2i	 operator<<( const int a ) const { return idVec2i( x << a, y << a ); }

	//! Returns a new idVec2i with each component bitwise ANDed with the given integer.
	idVec2i	 operator&( const int a ) const { return idVec2i( x & a, y & a ); }

	//! Performs a bitwise OR operation between each component of the vector and the given integer.
	idVec2i	 operator|( const int a ) const { return idVec2i( x | a, y | a ); }

	//! Returns a new idVec2i with each component XOR'd with the given integer.
	idVec2i	 operator^( const int a ) const { return idVec2i( x ^ a, y ^ a ); }

	//! Returns a new vector with each component scaled by the given integer factor.
	idVec2i	 operator*( const int a ) const { return idVec2i( x * a, y * a ); }

	//! Returns a new vector with each component divided by the given integer.
	idVec2i	 operator/( const int a ) const { return idVec2i( x / a, y / a ); }

	//! Returns a new vector with both components incremented by the specified integer value.
	idVec2i	 operator+( const int a ) const { return idVec2i( x + a, y + a ); }

	//! Returns a new idVec2i with both components decremented by the given integer value.
	idVec2i	 operator-( const int a ) const { return idVec2i( x - a, y - a ); }

	bool	 operator==( const idVec2i& a ) const { return a.x == x && a.y == y; };
	bool	 operator!=( const idVec2i& a ) const { return a.x != x || a.y != y; };

	//! Returns a new idVec2i with each component bitwise right-shifted by the corresponding component of the input vector.
	idVec2i	 operator>>( const idVec2i& a ) const { return idVec2i( x >> a.x, y >> a.y ); }

	//! Returns a new idVec2i with each component bitwise shifted left by the corresponding component of the input vector.
	idVec2i	 operator<<( const idVec2i& a ) const { return idVec2i( x << a.x, y << a.y ); }

	//! Returns a new vector with the bitwise AND of the corresponding components of this vector and the given vector.
	idVec2i	 operator&( const idVec2i& a ) const { return idVec2i( x & a.x, y & a.y ); }

	//! Performs a bitwise OR operation on the components of this vector with another vector.
	idVec2i	 operator|( const idVec2i& a ) const { return idVec2i( x | a.x, y | a.y ); }

	//! Performs a bitwise XOR operation between the components of this vector and another vector.
	idVec2i	 operator^( const idVec2i& a ) const { return idVec2i( x ^ a.x, y ^ a.y ); }

	//! Multiplies this vector by another vector component-wise
	idVec2i	 operator*( const idVec2i& a ) const { return idVec2i( x * a.x, y * a.y ); }

	//! Returns a new vector with each component divided by the corresponding component of the input vector.
	idVec2i	 operator/( const idVec2i& a ) const { return idVec2i( x / a.x, y / a.y ); }

	//! Returns a new idVec2i with components x and y incremented by the corresponding components of the input vector.
	idVec2i	 operator+( const idVec2i& a ) const { return idVec2i( x + a.x, y + a.y ); }

	//! Returns a new idVec2i instance with components subtracted from the given idVec2i instance.
	idVec2i	 operator-( const idVec2i& a ) const { return idVec2i( x - a.x, y - a.y ); }

	//! Adds the given integer value to both components of the vector and returns a reference to the vector.
	idVec2i& operator+=( const int a )
	{
		x += a;
		y += a;
		return *this;
	}

	//! Subtracts the given integer value from both components of the vector and returns a reference to the vector.
	idVec2i& operator-=( const int a )
	{
		x -= a;
		y -= a;
		return *this;
	}

	//! Divides both components of the vector by the given integer value and returns a reference to the vector.
	idVec2i& operator/=( const int a )
	{
		x /= a;
		y /= a;
		return *this;
	}

	//! Multiplies the components of this idVec2i by the given integer.
	idVec2i& operator*=( const int a )
	{
		x *= a;
		y *= a;
		return *this;
	}

	//! Right shifts both components of the vector by the specified amount.
	idVec2i& operator>>=( const int a )
	{
		x >>= a;
		y >>= a;
		return *this;
	}

	//! Shifts the x and y components of the vector left by the specified number of bits.
	idVec2i& operator<<=( const int a )
	{
		x <<= a;
		y <<= a;
		return *this;
	}

	//! Performs a bitwise AND operation between each component of the vector and the given integer, modifying the vector in place.
	idVec2i& operator&=( const int a )
	{
		x &= a;
		y &= a;
		return *this;
	}

	//! Performs a bitwise OR operation between each component of the vector and the given integer, updating the vector in place.
	idVec2i& operator|=( const int a )
	{
		x |= a;
		y |= a;
		return *this;
	}

	//! Applies bitwise XOR operation between the vector components and the given integer.
	idVec2i& operator^=( const int a )
	{
		x ^= a;
		y ^= a;
		return *this;
	}

	//! Right-shifts the x and y components of this vector by the corresponding components of the given vector and returns a reference to this vector.
	idVec2i& operator>>=( const idVec2i& a )
	{
		x >>= a.x;
		y >>= a.y;
		return *this;
	}

	//! Performs a left bit shift operation on both components of the vector by the corresponding components of another vector.
	idVec2i& operator<<=( const idVec2i& a )
	{
		x <<= a.x;
		y <<= a.y;
		return *this;
	}

	//! Performs a bitwise AND operation between the components of this vector and another vector, modifying this vector in place and returning a reference to it.
	idVec2i& operator&=( const idVec2i& a )
	{
		x &= a.x;
		y &= a.y;
		return *this;
	}

	//! Performs a bitwise OR operation between this vector and another vector, storing the result in this vector.
	idVec2i& operator|=( const idVec2i& a )
	{
		x |= a.x;
		y |= a.y;
		return *this;
	}

	//! Performs a bitwise XOR operation between the components of this vector and another vector, storing the result in this vector.
	idVec2i& operator^=( const idVec2i& a )
	{
		x ^= a.x;
		y ^= a.y;
		return *this;
	}

	//! Adds the components of the given vector to this vector and returns a reference to this vector.
	idVec2i& operator+=( const idVec2i& a )
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	//! Subtracts the components of the given vector from this vector and returns a reference to this vector.
	idVec2i& operator-=( const idVec2i& a )
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}

	//! Divides the vector components by the corresponding components of another vector and returns a reference to this vector
	idVec2i& operator/=( const idVec2i& a )
	{
		x /= a.x;
		y /= a.y;
		return *this;
	}

	//! Multiplies the coordinates of this vector by the corresponding coordinates of another vector and returns a reference to this vector.
	idVec2i& operator*=( const idVec2i& a )
	{
		x *= a.x;
		y *= a.y;
		return *this;
	}
};

#endif
