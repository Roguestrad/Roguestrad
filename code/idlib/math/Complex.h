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

#ifndef __MATH_COMPLEX_H__
#define __MATH_COMPLEX_H__

/*!
	\class idComplex
	\brief A class representing complex numbers with standard arithmetic operations and utilities.

	This class implements a complex number data type supporting basic arithmetic operations such as addition, subtraction, multiplication, and division. It provides both complex-to-complex and
   complex-to-scalar operations. The class includes methods for accessing and modifying the real and imaginary components through indexed access operators. Utility functions are included to compute
   the absolute value, square root, and reciprocal of a complex number. The implementation supports comparison operations with epsilon tolerance and string conversion for debugging and logging
   purposes. The class is designed for efficient in-place operations and follows standard C++ conventions for operator overloading.

*/
class idComplex
{
public:
	float r; // real part
	float i; // imaginary part

	//! Constructs a new complex number with default values.
	idComplex();

	//! Constructs a complex number with the specified real and imaginary parts.
	idComplex( const float r, const float i );

	//! Sets the real and imaginary parts of the complex number.
	void			 Set( const float r, const float i );

	//! Sets both real and imaginary components of the complex number to zero.
	void			 Zero();

	//! Returns the real or imaginary component of the complex number at the specified index.
	float			 operator[]( int index ) const;

	//! Provides indexed access to the real and imaginary components of a complex number
	float&			 operator[]( int index );

	//! Returns the negation of this complex number.
	idComplex		 operator-() const;

	//! Assigns the value of another complex number to this complex number and returns a reference to this.
	idComplex&		 operator=( const idComplex& a );

	//! Returns the product of this complex number and another complex number.
	idComplex		 operator*( const idComplex& a ) const;

	//! Returns the quotient of this complex number and another complex number.
	idComplex		 operator/( const idComplex& a ) const;

	//! Returns the sum of this complex number and another complex number.
	idComplex		 operator+( const idComplex& a ) const;

	//! Subtracts the real and imaginary components of the input complex number from this complex number and returns the result.
	idComplex		 operator-( const idComplex& a ) const;

	//! Multiplies this complex number by another and assigns the result to this complex number.
	idComplex&		 operator*=( const idComplex& a );

	//! Performs in-place division of this complex number by another complex number
	idComplex&		 operator/=( const idComplex& a );

	//! Adds the components of another complex number to this complex number and returns a reference to this object.
	idComplex&		 operator+=( const idComplex& a );

	//! Subtracts the components of another complex number from this complex number and returns a reference to this object.
	idComplex&		 operator-=( const idComplex& a );

	//! Multiplies this complex number by a scalar value and returns the result.
	idComplex		 operator*( const float a ) const;

	//! Returns a new complex number that is the result of dividing this complex number by a scalar value.
	idComplex		 operator/( const float a ) const;

	//! Returns a new complex number with the real part incremented by the given scalar value.
	idComplex		 operator+( const float a ) const;

	//! Subtracts a scalar value from the real component of the complex number and returns a new complex number.
	idComplex		 operator-( const float a ) const;

	//! Multiplies both the real and imaginary components of the complex number by the given scalar value.
	idComplex&		 operator*=( const float a );

	//! Divides both the real and imaginary parts of the complex number by the given scalar value.
	idComplex&		 operator/=( const float a );

	//! Adds a scalar value to the real component of the complex number and returns a reference to itself.
	idComplex&		 operator+=( const float a );

	//! Subtracts a scalar value from the real component of this complex number and returns a reference to itself.
	idComplex&		 operator-=( const float a );

	friend idComplex operator*( const float a, const idComplex& b );
	friend idComplex operator/( const float a, const idComplex& b );
	friend idComplex operator+( const float a, const idComplex& b );
	friend idComplex operator-( const float a, const idComplex& b );

	//! Compares this complex number with another for exact equality.
	bool			 Compare( const idComplex& a ) const;

	//! Compares this complex number with another complex number using an epsilon threshold.
	bool			 Compare( const idComplex& a, const float epsilon ) const;

	//! Compares two idComplex objects for exact equality.
	bool			 operator==( const idComplex& a ) const;

	//! Checks if this complex number is not equal to another complex number.
	bool			 operator!=( const idComplex& a ) const;

	//! Returns the reciprocal of this complex number.
	idComplex		 Reciprocal() const;

	//! Computes the complex square root of this complex number
	idComplex		 Sqrt() const;

	//! Computes the absolute value (magnitude) of the complex number.
	float			 Abs() const;

	//! Returns the dimension of the complex number, which is always 2.
	int				 GetDimension() const;

	//! Returns a pointer to the real component of the complex number.
	const float*	 ToFloatPtr() const;

	//! Returns a pointer to the real component of this complex number.
	float*			 ToFloatPtr();

	//! Converts the complex number to a string representation with the specified precision
	const char*		 ToString( int precision = 2 ) const;
};

extern idComplex complex_origin;
#define complex_zero complex_origin

ID_INLINE idComplex::idComplex()
{
}

ID_INLINE idComplex::idComplex( const float r, const float i )
{
	this->r = r;
	this->i = i;
}

ID_INLINE void idComplex::Set( const float r, const float i )
{
	this->r = r;
	this->i = i;
}

ID_INLINE void idComplex::Zero()
{
	r = i = 0.0f;
}

ID_INLINE float idComplex::operator[]( int index ) const
{
	assert( index >= 0 && index < 2 );
	return ( &r )[index];
}

ID_INLINE float& idComplex::operator[]( int index )
{
	assert( index >= 0 && index < 2 );
	return ( &r )[index];
}

ID_INLINE idComplex idComplex::operator-() const
{
	return idComplex( -r, -i );
}

ID_INLINE idComplex& idComplex::operator=( const idComplex& a )
{
	r = a.r;
	i = a.i;
	return *this;
}

ID_INLINE idComplex idComplex::operator*( const idComplex& a ) const
{
	return idComplex( r * a.r - i * a.i, i * a.r + r * a.i );
}

ID_INLINE idComplex idComplex::operator/( const idComplex& a ) const
{
	float s, t;
	if( idMath::Fabs( a.r ) >= idMath::Fabs( a.i ) ) {
		s = a.i / a.r;
		t = 1.0f / ( a.r + s * a.i );
		return idComplex( ( r + s * i ) * t, ( i - s * r ) * t );
	} else {
		s = a.r / a.i;
		t = 1.0f / ( s * a.r + a.i );
		return idComplex( ( r * s + i ) * t, ( i * s - r ) * t );
	}
}

ID_INLINE idComplex idComplex::operator+( const idComplex& a ) const
{
	return idComplex( r + a.r, i + a.i );
}

ID_INLINE idComplex idComplex::operator-( const idComplex& a ) const
{
	return idComplex( r - a.r, i - a.i );
}

ID_INLINE idComplex& idComplex::operator*=( const idComplex& a )
{
	*this = idComplex( r * a.r - i * a.i, i * a.r + r * a.i );
	return *this;
}

ID_INLINE idComplex& idComplex::operator/=( const idComplex& a )
{
	float s, t;
	if( idMath::Fabs( a.r ) >= idMath::Fabs( a.i ) ) {
		s	  = a.i / a.r;
		t	  = 1.0f / ( a.r + s * a.i );
		*this = idComplex( ( r + s * i ) * t, ( i - s * r ) * t );
	} else {
		s	  = a.r / a.i;
		t	  = 1.0f / ( s * a.r + a.i );
		*this = idComplex( ( r * s + i ) * t, ( i * s - r ) * t );
	}
	return *this;
}

ID_INLINE idComplex& idComplex::operator+=( const idComplex& a )
{
	r += a.r;
	i += a.i;
	return *this;
}

ID_INLINE idComplex& idComplex::operator-=( const idComplex& a )
{
	r -= a.r;
	i -= a.i;
	return *this;
}

ID_INLINE idComplex idComplex::operator*( const float a ) const
{
	return idComplex( r * a, i * a );
}

ID_INLINE idComplex idComplex::operator/( const float a ) const
{
	float s = 1.0f / a;
	return idComplex( r * s, i * s );
}

ID_INLINE idComplex idComplex::operator+( const float a ) const
{
	return idComplex( r + a, i );
}

ID_INLINE idComplex idComplex::operator-( const float a ) const
{
	return idComplex( r - a, i );
}

ID_INLINE idComplex& idComplex::operator*=( const float a )
{
	r *= a;
	i *= a;
	return *this;
}

ID_INLINE idComplex& idComplex::operator/=( const float a )
{
	float s = 1.0f / a;
	r *= s;
	i *= s;
	return *this;
}

ID_INLINE idComplex& idComplex::operator+=( const float a )
{
	r += a;
	return *this;
}

ID_INLINE idComplex& idComplex::operator-=( const float a )
{
	r -= a;
	return *this;
}

//! Multiplies a complex number by a scalar value.
ID_INLINE idComplex operator*( const float a, const idComplex& b )
{
	return idComplex( a * b.r, a * b.i );
}

//! Returns the complex number result of dividing a scalar by a complex number.
ID_INLINE idComplex operator/( const float a, const idComplex& b )
{
	float s, t;
	if( idMath::Fabs( b.r ) >= idMath::Fabs( b.i ) ) {
		s = b.i / b.r;
		t = a / ( b.r + s * b.i );
		return idComplex( t, -s * t );
	} else {
		s = b.r / b.i;
		t = a / ( s * b.r + b.i );
		return idComplex( s * t, -t );
	}
}

//! Returns a new complex number representing the sum of a scalar and a complex number.
ID_INLINE idComplex operator+( const float a, const idComplex& b )
{
	return idComplex( a + b.r, b.i );
}

//! Subtracts a complex number from a real number and returns the result as a new complex number.
ID_INLINE idComplex operator-( const float a, const idComplex& b )
{
	return idComplex( a - b.r, -b.i );
}

ID_INLINE idComplex idComplex::Reciprocal() const
{
	float s, t;
	if( idMath::Fabs( r ) >= idMath::Fabs( i ) ) {
		s = i / r;
		t = 1.0f / ( r + s * i );
		return idComplex( t, -s * t );
	} else {
		s = r / i;
		t = 1.0f / ( s * r + i );
		return idComplex( s * t, -t );
	}
}

ID_INLINE idComplex idComplex::Sqrt() const
{
	float x, y, w;

	if( r == 0.0f && i == 0.0f ) { return idComplex( 0.0f, 0.0f ); }
	x = idMath::Fabs( r );
	y = idMath::Fabs( i );
	if( x >= y ) {
		w = y / x;
		w = idMath::Sqrt( x ) * idMath::Sqrt( 0.5f * ( 1.0f + idMath::Sqrt( 1.0f + w * w ) ) );
	} else {
		w = x / y;
		w = idMath::Sqrt( y ) * idMath::Sqrt( 0.5f * ( w + idMath::Sqrt( 1.0f + w * w ) ) );
	}
	if( w == 0.0f ) { return idComplex( 0.0f, 0.0f ); }
	if( r >= 0.0f ) {
		return idComplex( w, 0.5f * i / w );
	} else {
		return idComplex( 0.5f * y / w, ( i >= 0.0f ) ? w : -w );
	}
}

ID_INLINE float idComplex::Abs() const
{
	float x, y, t;
	x = idMath::Fabs( r );
	y = idMath::Fabs( i );
	if( x == 0.0f ) {
		return y;
	} else if( y == 0.0f ) {
		return x;
	} else if( x > y ) {
		t = y / x;
		return x * idMath::Sqrt( 1.0f + t * t );
	} else {
		t = x / y;
		return y * idMath::Sqrt( 1.0f + t * t );
	}
}

ID_INLINE bool idComplex::Compare( const idComplex& a ) const
{
	return ( ( r == a.r ) && ( i == a.i ) );
}

ID_INLINE bool idComplex::Compare( const idComplex& a, const float epsilon ) const
{
	if( idMath::Fabs( r - a.r ) > epsilon ) { return false; }
	if( idMath::Fabs( i - a.i ) > epsilon ) { return false; }
	return true;
}

ID_INLINE bool idComplex::operator==( const idComplex& a ) const
{
	return Compare( a );
}

ID_INLINE bool idComplex::operator!=( const idComplex& a ) const
{
	return !Compare( a );
}

ID_INLINE int idComplex::GetDimension() const
{
	return 2;
}

ID_INLINE const float* idComplex::ToFloatPtr() const
{
	return &r;
}

ID_INLINE float* idComplex::ToFloatPtr()
{
	return &r;
}

#endif /* !__MATH_COMPLEX_H__ */
