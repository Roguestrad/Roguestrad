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

#ifndef __MATH_POLYNOMIAL_H__
#define __MATH_POLYNOMIAL_H__

/*!
	\class idPolynomial
	\brief A class representing polynomial functions with support for evaluation, arithmetic operations, and root finding.

	This class provides a comprehensive interface for working with polynomial functions, including initialization with various degrees and coefficients, arithmetic operations such as addition,
   subtraction, and scalar multiplication, and evaluation at both real and complex values. It supports finding roots for polynomials of different degrees, including specialized methods for linear,
   quadratic, cubic, and quartic equations. The class also provides functionality for computing derivatives and antiderivatives of polynomials. Memory management is handled internally with no explicit
   ownership semantics, and the class maintains an internal coefficient array that can be resized while optionally preserving existing values. The interface is designed to be both efficient and easy
   to use, supporting both inline operations and more complex computations.

*/
class idPolynomial
{
public:
	//! Initializes a new polynomial with no coefficients and a degree of -1.
	idPolynomial();

	//! Initializes a polynomial with the specified maximum degree.
	explicit idPolynomial( int d );

	//! Initializes a polynomial with linear coefficients a and b.
	explicit idPolynomial( float a, float b );

	//! Initializes a polynomial with coefficients a, b, and c for terms x^2, x, and the constant term respectively.
	explicit idPolynomial( float a, float b, float c );

	/*!
		\brief Initializes a polynomial with coefficients for terms a*x^3 + b*x^2 + c*x + d

		The constructor initializes a polynomial object with the given coefficients, where a, b, c, and d correspond to the coefficients of the terms x^3, x^2, x, and the constant term respectively.
	   The polynomial is initialized with a degree of 3, and the coefficients are stored in the order [d, c, b, a] to match the standard polynomial representation.

		\param a coefficient for the x^3 term
		\param b coefficient for the x^2 term
		\param c coefficient for the x term
		\param d constant term
	*/
	explicit idPolynomial( float a, float b, float c, float d );

	/*!
		\brief Initializes a polynomial with coefficients for terms of degree 0 through 4.

		The constructor initializes a polynomial object with the specified coefficients for terms of degree 0 through 4. The coefficients are stored in the order of increasing degree, where
	   coefficient[0] corresponds to the constant term (degree 0), coefficient[1] to the linear term (degree 1), and so on up to coefficient[4] for the quartic term (degree 4). The polynomial is
	   initialized with a degree of -1 and no allocated memory, then resized to accommodate 5 coefficients (degrees 0-4) and the coefficients are set accordingly.

		\param a coefficient for the quartic term (degree 4)
		\param b coefficient for the cubic term (degree 3)
		\param c coefficient for the quadratic term (degree 2)
		\param d coefficient for the linear term (degree 1)
		\param e coefficient for the constant term (degree 0)
	*/
	explicit idPolynomial( float a, float b, float c, float d, float e );

	// SRS - Added destructor, otherwise idPolynomial() will leak memory
	~idPolynomial() { Mem_Free16( coefficient ); };

	//! Provides access to a specific coefficient in the polynomial by index
	float		  operator[]( int index ) const;

	//! Provides access to a coefficient of the polynomial at the specified index
	float&		  operator[]( int index );

	//! Returns a new polynomial with all coefficients negated.
	idPolynomial  operator-() const;

	//! Assigns the contents of another polynomial to this polynomial
	idPolynomial& operator=( const idPolynomial& p );

	//! Returns the sum of this polynomial and another polynomial.
	idPolynomial  operator+( const idPolynomial& p ) const;

	//! Returns a new polynomial that is the result of subtracting the given polynomial from this polynomial.
	idPolynomial  operator-( const idPolynomial& p ) const;

	//! Returns a new polynomial that is the result of multiplying this polynomial by a scalar value.
	idPolynomial  operator*( const float s ) const;

	//! Returns a new polynomial that is the result of dividing this polynomial by a scalar value
	idPolynomial  operator/( const float s ) const;

	//! Adds the coefficients of another polynomial to this polynomial and returns a reference to this polynomial.
	idPolynomial& operator+=( const idPolynomial& p );

	//! Subtracts the coefficients of another polynomial from this polynomial in place
	idPolynomial& operator-=( const idPolynomial& p );

	//! Multiplies the polynomial coefficients by a scalar value and updates the degree if the scalar is zero.
	idPolynomial& operator*=( const float s );

	//! Divides all coefficients of the polynomial by the given scalar value.
	idPolynomial& operator/=( const float s );

	//! Compares this polynomial with another polynomial for exact equality.
	bool		  Compare( const idPolynomial& p ) const;

	//! Compares this polynomial with another polynomial using the specified epsilon tolerance.
	bool		  Compare( const idPolynomial& p, const float epsilon ) const;

	//! Compares two polynomial objects for equality.
	bool		  operator==( const idPolynomial& p ) const;

	//! Checks if this polynomial is not equal to another polynomial.
	bool		  operator!=( const idPolynomial& p ) const;

	//! Sets the degree of the polynomial to zero, effectively clearing it.
	void		  Zero();

	//! Sets all coefficients of the polynomial to zero up to the specified degree.
	void		  Zero( int d );

	//! Returns the degree of the polynomial.
	int			  GetDimension() const;

	//! Returns the degree of the polynomial.
	int			  GetDegree() const;

	//! Evaluates the polynomial at the given real value x.
	float		  GetValue( const float x ) const;

	//! Evaluates the polynomial using the provided complex value.
	idComplex	  GetValue( const idComplex& x ) const;

	//! Returns the first derivative of the polynomial.
	idPolynomial  GetDerivative() const;

	//! Returns the anti-derivative of the polynomial.
	idPolynomial  GetAntiDerivative() const;

	//! Computes and returns all roots of the polynomial
	int			  GetRoots( idComplex* roots ) const;

	//! Returns the number of real roots of the polynomial, storing them in the provided array
	int			  GetRoots( float* roots ) const;

	//! Returns the root of a linear equation ax + b = 0
	static int	  GetRoots1( float a, float b, float* roots );

	/*!
		\brief Calculates the real roots of a quadratic equation ax^2 + bx + c = 0

		This function solves for the roots of a quadratic equation using the quadratic formula. It handles three cases: no real roots (when the discriminant is negative), one real root (when the
	   discriminant is zero), and two real roots (when the discriminant is positive). The function automatically normalizes the equation if the leading coefficient 'a' is not 1, and it ensures that
	   'a' is non-zero during normalization.

		\param a Leading coefficient of the quadratic term
		\param b Coefficient of the linear term
		\param c Constant term
		\param roots Pointer to array where computed roots will be stored
		\return Number of real roots found (0, 1, or 2)
		\throws Asserts if the leading coefficient 'a' is zero during normalization
	*/
	static int	  GetRoots2( float a, float b, float c, float* roots );

	/*!
		\brief Solves a cubic polynomial equation and returns the number of real roots found

		This function computes the real roots of a cubic polynomial equation of the form ax^3 + bx^2 + cx + d = 0. It handles three cases based on the discriminant: three distinct real roots when the
	   discriminant is negative, one real root when positive, and three real roots with possible repeated values when zero. The function normalizes the polynomial if the leading coefficient is not
	   unity, then applies the cubic formula using trigonometric or radical methods depending on the discriminant value. The roots are stored in the provided array with a maximum of three roots
	   returned.

		\param a Leading coefficient of the cubic term
		\param b Coefficient of the quadratic term
		\param c Coefficient of the linear term
		\param d Constant term
		\param roots Output array to store the computed real roots
		\return The number of real roots found, which can be 1 or 3 depending on the discriminant value
		\throws assertion failure when a is zero and the polynomial is not linear
	*/
	static int	  GetRoots3( float a, float b, float c, float d, float* roots );

	/*!
		\brief Computes the real roots of a quartic polynomial equation of the form ax^4 + bx^3 + cx^2 + dx + e = 0

		This function solves quartic polynomial equations by reducing them to cubic equations and then solving those using the GetRoots3 method. The algorithm handles cases where the leading
	   coefficient a is not equal to 1 by normalizing the equation. It computes up to four real roots and returns the count of valid roots found. The function uses numerical methods to find the roots
	   and handles special cases like negative discriminants which result in no real roots.

		\param a Leading coefficient of the quartic term
		\param b Coefficient of the cubic term
		\param c Coefficient of the quadratic term
		\param d Coefficient of the linear term
		\param e Constant term
		\param roots Output array to store the computed real roots
		\return The number of real roots found (between 0 and 4)
		\throws Assertion error if the leading coefficient a is zero
	*/
	static int	  GetRoots4( float a, float b, float c, float d, float e, float* roots );

	//! Returns a pointer to the coefficient array of the polynomial.
	const float*  ToFloatPtr() const;

	//! Returns a pointer to the coefficient array of the polynomial.
	float*		  ToFloatPtr();

	//! Returns a string representation of the polynomial with the specified decimal precision
	const char*	  ToString( int precision = 2 ) const;

	//! Tests the polynomial root-finding functionality with various polynomial configurations
	static void	  Test();

private:
	int	   degree;
	int	   allocated;
	float* coefficient;

	//! Resizes the polynomial to the specified degree while optionally preserving existing coefficients.
	void   Resize( int d, bool keep );

	//! Computes a root of a polynomial using Laguerre's method
	int	   Laguer( const idComplex* coef, const int degree, idComplex& r ) const;
};

ID_INLINE idPolynomial::idPolynomial()
{
	degree		= -1;
	allocated	= 0;
	coefficient = NULL;
}

ID_INLINE idPolynomial::idPolynomial( int d )
{
	degree		= -1;
	allocated	= 0;
	coefficient = NULL;
	Resize( d, false );
}

ID_INLINE idPolynomial::idPolynomial( float a, float b )
{
	degree		= -1;
	allocated	= 0;
	coefficient = NULL;
	Resize( 1, false );
	coefficient[0] = b;
	coefficient[1] = a;
}

ID_INLINE idPolynomial::idPolynomial( float a, float b, float c )
{
	degree		= -1;
	allocated	= 0;
	coefficient = NULL;
	Resize( 2, false );
	coefficient[0] = c;
	coefficient[1] = b;
	coefficient[2] = a;
}

ID_INLINE idPolynomial::idPolynomial( float a, float b, float c, float d )
{
	degree		= -1;
	allocated	= 0;
	coefficient = NULL;
	Resize( 3, false );
	coefficient[0] = d;
	coefficient[1] = c;
	coefficient[2] = b;
	coefficient[3] = a;
}

ID_INLINE idPolynomial::idPolynomial( float a, float b, float c, float d, float e )
{
	degree		= -1;
	allocated	= 0;
	coefficient = NULL;
	Resize( 4, false );
	coefficient[0] = e;
	coefficient[1] = d;
	coefficient[2] = c;
	coefficient[3] = b;
	coefficient[4] = a;
}

ID_INLINE float idPolynomial::operator[]( int index ) const
{
	assert( index >= 0 && index <= degree );
	return coefficient[index];
}

ID_INLINE float& idPolynomial::operator[]( int index )
{
	assert( index >= 0 && index <= degree );
	return coefficient[index];
}

ID_INLINE idPolynomial idPolynomial::operator-() const
{
	int			 i;
	idPolynomial n;

	n = *this;
	for( i = 0; i <= degree; i++ ) {
		n[i] = -n[i];
	}
	return n;
}

ID_INLINE idPolynomial& idPolynomial::operator=( const idPolynomial& p )
{
	Resize( p.degree, false );
	for( int i = 0; i <= degree; i++ ) {
		coefficient[i] = p.coefficient[i];
	}
	return *this;
}

ID_INLINE idPolynomial idPolynomial::operator+( const idPolynomial& p ) const
{
	int			 i;
	idPolynomial n;

	if( degree > p.degree ) {
		n.Resize( degree, false );
		for( i = 0; i <= p.degree; i++ ) {
			n.coefficient[i] = coefficient[i] + p.coefficient[i];
		}
		for( ; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i];
		}
		n.degree = degree;
	} else if( p.degree > degree ) {
		n.Resize( p.degree, false );
		for( i = 0; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i] + p.coefficient[i];
		}
		for( ; i <= p.degree; i++ ) {
			n.coefficient[i] = p.coefficient[i];
		}
		n.degree = p.degree;
	} else {
		n.Resize( degree, false );
		n.degree = 0;
		for( i = 0; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i] + p.coefficient[i];
			if( n.coefficient[i] != 0.0f ) { n.degree = i; }
		}
	}
	return n;
}

ID_INLINE idPolynomial idPolynomial::operator-( const idPolynomial& p ) const
{
	int			 i;
	idPolynomial n;

	if( degree > p.degree ) {
		n.Resize( degree, false );
		for( i = 0; i <= p.degree; i++ ) {
			n.coefficient[i] = coefficient[i] - p.coefficient[i];
		}
		for( ; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i];
		}
		n.degree = degree;
	} else if( p.degree >= degree ) {
		n.Resize( p.degree, false );
		for( i = 0; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i] - p.coefficient[i];
		}
		for( ; i <= p.degree; i++ ) {
			n.coefficient[i] = -p.coefficient[i];
		}
		n.degree = p.degree;
	} else {
		n.Resize( degree, false );
		n.degree = 0;
		for( i = 0; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i] - p.coefficient[i];
			if( n.coefficient[i] != 0.0f ) { n.degree = i; }
		}
	}
	return n;
}

ID_INLINE idPolynomial idPolynomial::operator*( const float s ) const
{
	idPolynomial n;

	if( s == 0.0f ) {
		n.degree = 0;
	} else {
		n.Resize( degree, false );
		for( int i = 0; i <= degree; i++ ) {
			n.coefficient[i] = coefficient[i] * s;
		}
	}
	return n;
}

ID_INLINE idPolynomial idPolynomial::operator/( const float s ) const
{
	float		 invs;
	idPolynomial n;

	assert( s != 0.0f );
	n.Resize( degree, false );
	invs = 1.0f / s;
	for( int i = 0; i <= degree; i++ ) {
		n.coefficient[i] = coefficient[i] * invs;
	}
	return n;
}

ID_INLINE idPolynomial& idPolynomial::operator+=( const idPolynomial& p )
{
	int i;

	if( degree > p.degree ) {
		for( i = 0; i <= p.degree; i++ ) {
			coefficient[i] += p.coefficient[i];
		}
	} else if( p.degree > degree ) {
		Resize( p.degree, true );
		for( i = 0; i <= degree; i++ ) {
			coefficient[i] += p.coefficient[i];
		}
		for( ; i <= p.degree; i++ ) {
			coefficient[i] = p.coefficient[i];
		}
	} else {
		for( i = 0; i <= degree; i++ ) {
			coefficient[i] += p.coefficient[i];
			if( coefficient[i] != 0.0f ) { degree = i; }
		}
	}
	return *this;
}

ID_INLINE idPolynomial& idPolynomial::operator-=( const idPolynomial& p )
{
	int i;

	if( degree > p.degree ) {
		for( i = 0; i <= p.degree; i++ ) {
			coefficient[i] -= p.coefficient[i];
		}
	} else if( p.degree > degree ) {
		Resize( p.degree, true );
		for( i = 0; i <= degree; i++ ) {
			coefficient[i] -= p.coefficient[i];
		}
		for( ; i <= p.degree; i++ ) {
			coefficient[i] = -p.coefficient[i];
		}
	} else {
		for( i = 0; i <= degree; i++ ) {
			coefficient[i] -= p.coefficient[i];
			if( coefficient[i] != 0.0f ) { degree = i; }
		}
	}
	return *this;
}

ID_INLINE idPolynomial& idPolynomial::operator*=( const float s )
{
	if( s == 0.0f ) {
		degree = 0;
	} else {
		for( int i = 0; i <= degree; i++ ) {
			coefficient[i] *= s;
		}
	}
	return *this;
}

ID_INLINE idPolynomial& idPolynomial::operator/=( const float s )
{
	float invs;

	assert( s != 0.0f );
	invs = 1.0f / s;
	for( int i = 0; i <= degree; i++ ) {
		coefficient[i] = invs;
	}
	return *this;
	;
}

ID_INLINE bool idPolynomial::Compare( const idPolynomial& p ) const
{
	if( degree != p.degree ) { return false; }
	for( int i = 0; i <= degree; i++ ) {
		if( coefficient[i] != p.coefficient[i] ) { return false; }
	}
	return true;
}

ID_INLINE bool idPolynomial::Compare( const idPolynomial& p, const float epsilon ) const
{
	if( degree != p.degree ) { return false; }
	for( int i = 0; i <= degree; i++ ) {
		if( idMath::Fabs( coefficient[i] - p.coefficient[i] ) > epsilon ) { return false; }
	}
	return true;
}

ID_INLINE bool idPolynomial::operator==( const idPolynomial& p ) const
{
	return Compare( p );
}

ID_INLINE bool idPolynomial::operator!=( const idPolynomial& p ) const
{
	return !Compare( p );
}

ID_INLINE void idPolynomial::Zero()
{
	degree = 0;
}

ID_INLINE void idPolynomial::Zero( int d )
{
	Resize( d, false );
	for( int i = 0; i <= degree; i++ ) {
		coefficient[i] = 0.0f;
	}
}

ID_INLINE int idPolynomial::GetDimension() const
{
	return degree;
}

ID_INLINE int idPolynomial::GetDegree() const
{
	return degree;
}

ID_INLINE float idPolynomial::GetValue( const float x ) const
{
	float y, z;
	y = coefficient[0];
	z = x;
	for( int i = 1; i <= degree; i++ ) {
		y += coefficient[i] * z;
		z *= x;
	}
	return y;
}

ID_INLINE idComplex idPolynomial::GetValue( const idComplex& x ) const
{
	idComplex y, z;
	y.Set( coefficient[0], 0.0f );
	z = x;
	for( int i = 1; i <= degree; i++ ) {
		y += coefficient[i] * z;
		z *= x;
	}
	return y;
}

ID_INLINE idPolynomial idPolynomial::GetDerivative() const
{
	idPolynomial n;

	if( degree == 0 ) { return n; }
	n.Resize( degree - 1, false );
	for( int i = 1; i <= degree; i++ ) {
		n.coefficient[i - 1] = i * coefficient[i];
	}
	return n;
}

ID_INLINE idPolynomial idPolynomial::GetAntiDerivative() const
{
	idPolynomial n;

	if( degree == 0 ) { return n; }
	n.Resize( degree + 1, false );
	n.coefficient[0] = 0.0f;
	for( int i = 0; i <= degree; i++ ) {
		n.coefficient[i + 1] = coefficient[i] / ( i + 1 );
	}
	return n;
}

ID_INLINE int idPolynomial::GetRoots1( float a, float b, float* roots )
{
	assert( a != 0.0f );
	roots[0] = -b / a;
	return 1;
}

ID_INLINE int idPolynomial::GetRoots2( float a, float b, float c, float* roots )
{
	float inva, ds;

	if( a != 1.0f ) {
		assert( a != 0.0f );
		inva = 1.0f / a;
		c *= inva;
		b *= inva;
	}
	ds = b * b - 4.0f * c;
	if( ds < 0.0f ) {
		return 0;
	} else if( ds > 0.0f ) {
		ds		 = idMath::Sqrt( ds );
		roots[0] = 0.5f * ( -b - ds );
		roots[1] = 0.5f * ( -b + ds );
		return 2;
	} else {
		roots[0] = 0.5f * -b;
		return 1;
	}
}

ID_INLINE int idPolynomial::GetRoots3( float a, float b, float c, float d, float* roots )
{
	float inva, f, g, halfg, ofs, ds, dist, angle, cs, ss, t;

	if( a != 1.0f ) {
		assert( a != 0.0f );
		inva = 1.0f / a;
		d *= inva;
		c *= inva;
		b *= inva;
	}

	f	  = ( 1.0f / 3.0f ) * ( 3.0f * c - b * b );
	g	  = ( 1.0f / 27.0f ) * ( 2.0f * b * b * b - 9.0f * c * b + 27.0f * d );
	halfg = 0.5f * g;
	ofs	  = ( 1.0f / 3.0f ) * b;
	ds	  = 0.25f * g * g + ( 1.0f / 27.0f ) * f * f * f;

	if( ds < 0.0f ) {
		dist	 = idMath::Sqrt( ( -1.0f / 3.0f ) * f );
		angle	 = ( 1.0f / 3.0f ) * idMath::ATan( idMath::Sqrt( -ds ), -halfg );
		cs		 = idMath::Cos( angle );
		ss		 = idMath::Sin( angle );
		roots[0] = 2.0f * dist * cs - ofs;
		roots[1] = -dist * ( cs + idMath::SQRT_THREE * ss ) - ofs;
		roots[2] = -dist * ( cs - idMath::SQRT_THREE * ss ) - ofs;
		return 3;
	} else if( ds > 0.0f ) {
		ds = idMath::Sqrt( ds );
		t  = -halfg + ds;
		if( t >= 0.0f ) {
			roots[0] = idMath::Pow( t, ( 1.0f / 3.0f ) );
		} else {
			roots[0] = -idMath::Pow( -t, ( 1.0f / 3.0f ) );
		}
		t = -halfg - ds;
		if( t >= 0.0f ) {
			roots[0] += idMath::Pow( t, ( 1.0f / 3.0f ) );
		} else {
			roots[0] -= idMath::Pow( -t, ( 1.0f / 3.0f ) );
		}
		roots[0] -= ofs;
		return 1;
	} else {
		if( halfg >= 0.0f ) {
			t = -idMath::Pow( halfg, ( 1.0f / 3.0f ) );
		} else {
			t = idMath::Pow( -halfg, ( 1.0f / 3.0f ) );
		}
		roots[0] = 2.0f * t - ofs;
		roots[1] = -t - ofs;
		roots[2] = roots[1];
		return 3;
	}
}

ID_INLINE int idPolynomial::GetRoots4( float a, float b, float c, float d, float e, float* roots )
{
	int	  count;
	float inva, y, ds, r, s1, s2, t1, t2, tp, tm;
	float roots3[3];

	if( a != 1.0f ) {
		assert( a != 0.0f );
		inva = 1.0f / a;
		e *= inva;
		d *= inva;
		c *= inva;
		b *= inva;
	}

	count = 0;

	GetRoots3( 1.0f, -c, b * d - 4.0f * e, -b * b * e + 4.0f * c * e - d * d, roots3 );
	y  = roots3[0];
	ds = 0.25f * b * b - c + y;

	if( ds < 0.0f ) {
		return 0;
	} else if( ds > 0.0f ) {
		r  = idMath::Sqrt( ds );
		t1 = 0.75f * b * b - r * r - 2.0f * c;
		t2 = ( 4.0f * b * c - 8.0f * d - b * b * b ) / ( 4.0f * r );
		tp = t1 + t2;
		tm = t1 - t2;

		if( tp >= 0.0f ) {
			s1			   = idMath::Sqrt( tp );
			roots[count++] = -0.25f * b + 0.5f * ( r + s1 );
			roots[count++] = -0.25f * b + 0.5f * ( r - s1 );
		}
		if( tm >= 0.0f ) {
			s2			   = idMath::Sqrt( tm );
			roots[count++] = -0.25f * b + 0.5f * ( s2 - r );
			roots[count++] = -0.25f * b - 0.5f * ( s2 + r );
		}
		return count;
	} else {
		t2 = y * y - 4.0f * e;
		if( t2 >= 0.0f ) {
			t2 = 2.0f * idMath::Sqrt( t2 );
			t1 = 0.75f * b * b - 2.0f * c;
			if( t1 + t2 >= 0.0f ) {
				s1			   = idMath::Sqrt( t1 + t2 );
				roots[count++] = -0.25f * b + 0.5f * s1;
				roots[count++] = -0.25f * b - 0.5f * s1;
			}
			if( t1 - t2 >= 0.0f ) {
				s2			   = idMath::Sqrt( t1 - t2 );
				roots[count++] = -0.25f * b + 0.5f * s2;
				roots[count++] = -0.25f * b - 0.5f * s2;
			}
		}
		return count;
	}
}

ID_INLINE const float* idPolynomial::ToFloatPtr() const
{
	return coefficient;
}

ID_INLINE float* idPolynomial::ToFloatPtr()
{
	return coefficient;
}

ID_INLINE void idPolynomial::Resize( int d, bool keep )
{
	int alloc = ( d + 1 + 3 ) & ~3;
	if( alloc > allocated ) {
		float* ptr = ( float* )Mem_Alloc16( alloc * sizeof( float ), TAG_MATH );
		if( coefficient != NULL ) {
			if( keep ) {
				for( int i = 0; i <= degree; i++ ) {
					ptr[i] = coefficient[i];
				}
			}
			Mem_Free16( coefficient );
		}
		allocated	= alloc;
		coefficient = ptr;
	}
	degree = d;
}

#endif /* !__MATH_POLYNOMIAL_H__ */
