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

#ifndef __MATH_CURVE_H__
#define __MATH_CURVE_H__

/*!
	\class idCurve
	\brief A templated curve class that represents and evaluates parametric curves with time-based values.

	This class provides a framework for managing and evaluating parametric curves where each control point has an associated time value and a corresponding value of a specified type. The curve
   supports various operations like adding, removing, and querying control points, as well as computing derivatives, arc length, and speed at specific time values. The templated nature allows it to
   work with different data types for the curve values while maintaining a consistent interface for time-based curve evaluation. It offers methods for modifying curve properties such as time
   distribution and value translation, making it suitable for animation, interpolation, and trajectory calculations.

*/
template<class type>
class idCurve
{
public:
	//! Initializes a new instance of the idCurve template class.
	idCurve();
	virtual ~idCurve();

	//! Adds a timed/value pair to the spline and returns the index of the inserted pair.
	virtual int	 AddValue( const float time, const type& value );

	//! Removes the element at the specified index from the curve
	virtual void RemoveIndex( const int index )
	{
		values.RemoveIndex( index );
		times.RemoveIndex( index );
		changed = true;
	}

	//! Clears all curve values and resets the curve state
	virtual void Clear()
	{
		values.Clear();
		times.Clear();
		currentIndex = -1;
		changed		 = true;
	}

	//! Returns the curve value at the specified time
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the curve at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the curve at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

	//! Checks if the curve has finished based on the given time value.
	virtual bool IsDone( const float time ) const;

	//! Returns the number of values stored in the curve.
	int			 GetNumValues() const { return values.Num(); }

	//! Sets the value at the specified index in the curve and marks the curve as changed.
	void		 SetValue( const int index, const type& value )
	{
		values[index] = value;
		changed		  = true;
	}

	//! Returns the value at the specified index in the curve
	type  GetValue( const int index ) const { return values[index]; }

	//! Returns a pointer to the value at the specified index in the curve.
	type* GetValueAddress( const int index ) { return &values[index]; }

	//! Returns the time value at the specified index from the curve
	float GetTime( const int index ) const { return times[index]; }

	//! Calculates the arc length of the curve up to a given time value.
	float GetLengthForTime( const float time ) const;

	//! Returns the time value corresponding to a given arc length along the curve.
	float GetTimeForLength( const float length, const float epsilon = 0.1f ) const;

	//! Calculates the arc length between two knots in a curve using Romberg integration.
	float GetLengthBetweenKnots( const int i0, const int i1 ) const;

	//! Sets the curve times to be uniformly distributed over the specified total time.
	void  MakeUniform( const float totalTime );

	//! Sets the curve to have constant speed over the specified total time.
	void  SetConstantSpeed( const float totalTime );

	//! Shifts all time values in the curve by the specified delta time.
	void  ShiftTime( const float deltaTime );

	//! Translates all curve values by the specified translation amount.
	void  Translate( const type& translation );

protected:
	idList<float> times;  // knots
	idList<type>  values; // knot values

	mutable int	  currentIndex; // cached index for fast lookup
	mutable bool  changed;		// set whenever the curve changes

	//! Returns the index of the first time value greater than or equal to the given time.
	int			  IndexForTime( const float time ) const;

	//! Returns the time value for a given index in the curve.
	float		  TimeForIndex( const int index ) const;

	//! Returns the curve value at the specified index, with linear extrapolation for out-of-bounds indices.
	type		  ValueForIndex( const int index ) const;

	//! Returns the speed of the curve at the given time.
	float		  GetSpeed( const float time ) const;

	//! Computes the Romberg integral of the curve speed between two time points using Richardson extrapolation.
	float		  RombergIntegral( const float t0, const float t1, const int order ) const;
};

template<class type>
ID_INLINE idCurve<type>::idCurve()
{
	currentIndex = -1;
	changed		 = false;
}

template<class type>
ID_INLINE idCurve<type>::~idCurve()
{
}

template<class type>
ID_INLINE int idCurve<type>::AddValue( const float time, const type& value )
{
	int i;

	i = IndexForTime( time );
	times.Insert( time, i );
	values.Insert( value, i );
	changed = true;
	return i;
}

template<class type>
ID_INLINE type idCurve<type>::GetCurrentValue( const float time ) const
{
	int i;

	i = IndexForTime( time );
	if( i >= values.Num() ) {
		return values[values.Num() - 1];
	} else {
		return values[i];
	}
}

template<class type>
ID_INLINE type idCurve<type>::GetCurrentFirstDerivative( const float time ) const
{
	return ( values[0] - values[0] ); //-V501
}

template<class type>
ID_INLINE type idCurve<type>::GetCurrentSecondDerivative( const float time ) const
{
	return ( values[0] - values[0] ); //-V501
}

template<class type>
ID_INLINE bool idCurve<type>::IsDone( const float time ) const
{
	return ( time >= times[times.Num() - 1] );
}

template<class type>
ID_INLINE float idCurve<type>::GetSpeed( const float time ) const
{
	int	  i;
	float speed;
	type  value;

	value = GetCurrentFirstDerivative( time );
	for( speed = 0.0f, i = 0; i < value.GetDimension(); i++ ) {
		speed += value[i] * value[i];
	}
	return idMath::Sqrt( speed );
}

template<class type>
ID_INLINE float idCurve<type>::RombergIntegral( const float t0, const float t1, const int order ) const
{
	int	   i, j, k, m, n;
	float  sum, delta;
	float* temp[2];

	temp[0] = ( float* )_alloca16( order * sizeof( float ) );
	temp[1] = ( float* )_alloca16( order * sizeof( float ) );

	delta	   = t1 - t0;
	temp[0][0] = 0.5f * delta * ( GetSpeed( t0 ) + GetSpeed( t1 ) );

	for( i = 2, m = 1; i <= order; i++, m *= 2, delta *= 0.5f ) {
		// approximate using the trapezoid rule
		sum = 0.0f;
		for( j = 1; j <= m; j++ ) {
			sum += GetSpeed( t0 + delta * ( j - 0.5f ) );
		}

		// Richardson extrapolation
		temp[1][0] = 0.5f * ( temp[0][0] + delta * sum );
		for( k = 1, n = 4; k < i; k++, n *= 4 ) {
			temp[1][k] = ( n * temp[1][k - 1] - temp[0][k - 1] ) / ( n - 1 );
		}

		for( j = 0; j < i; j++ ) {
			temp[0][j] = temp[1][j];
		}
	}
	return temp[0][order - 1];
}

template<class type>
ID_INLINE float idCurve<type>::GetLengthBetweenKnots( const int i0, const int i1 ) const
{
	float length = 0.0f;
	for( int i = i0; i < i1; i++ ) {
		length += RombergIntegral( times[i], times[i + 1], 5 );
	}
	return length;
}

template<class type>
ID_INLINE float idCurve<type>::GetLengthForTime( const float time ) const
{
	float length = 0.0f;
	int	  index	 = IndexForTime( time );
	for( int i = 0; i < index; i++ ) {
		length += RombergIntegral( times[i], times[i + 1], 5 );
	}
	length += RombergIntegral( times[index], time, 5 );
	return length;
}

template<class type>
ID_INLINE float idCurve<type>::GetTimeForLength( const float length, const float epsilon ) const
{
	int	   i, index;
	float *accumLength, totalLength, len0, len1, t, diff;

	if( length <= 0.0f ) { return times[0]; }

	accumLength = ( float* )_alloca16( values.Num() * sizeof( float ) );
	totalLength = 0.0f;
	for( index = 0; index < values.Num() - 1; index++ ) {
		totalLength += GetLengthBetweenKnots( index, index + 1 );
		accumLength[index] = totalLength;
		if( length < accumLength[index] ) { break; }
	}

	if( index >= values.Num() - 1 ) { return times[times.Num() - 1]; }

	if( index == 0 ) {
		len0 = length;
		len1 = accumLength[0];
	} else {
		len0 = length - accumLength[index - 1];
		len1 = accumLength[index] - accumLength[index - 1];
	}

	// invert the arc length integral using Newton's method
	t = ( times[index + 1] - times[index] ) * len0 / len1;
	for( i = 0; i < 32; i++ ) {
		diff = RombergIntegral( times[index], times[index] + t, 5 ) - len0;
		if( idMath::Fabs( diff ) <= epsilon ) { return times[index] + t; }
		t -= diff / GetSpeed( times[index] + t );
	}
	return times[index] + t;
}

template<class type>
ID_INLINE void idCurve<type>::MakeUniform( const float totalTime )
{
	int i, n;

	n = times.Num() - 1;
	for( i = 0; i <= n; i++ ) {
		times[i] = i * totalTime / n;
	}
	changed = true;
}

template<class type>
ID_INLINE void idCurve<type>::SetConstantSpeed( const float totalTime )
{
	int	   i, j;
	float *length, totalLength, scale, t;

	length		= ( float* )_alloca16( values.Num() * sizeof( float ) );
	totalLength = 0.0f;
	for( i = 0; i < values.Num() - 1; i++ ) {
		length[i] = GetLengthBetweenKnots( i, i + 1 );
		totalLength += length[i];
	}
	scale = totalTime / totalLength;
	for( t = 0.0f, i = 0; i < times.Num() - 1; i++ ) {
		times[i] = t;
		t += scale * length[i];
	}
	times[times.Num() - 1] = totalTime;
	changed				   = true;
}

template<class type>
ID_INLINE void idCurve<type>::ShiftTime( const float deltaTime )
{
	for( int i = 0; i < times.Num(); i++ ) {
		times[i] += deltaTime;
	}
	changed = true;
}

template<class type>
ID_INLINE void idCurve<type>::Translate( const type& translation )
{
	for( int i = 0; i < values.Num(); i++ ) {
		values[i] += translation;
	}
	changed = true;
}

template<class type>
ID_INLINE int idCurve<type>::IndexForTime( const float time ) const
{
	int len, mid, offset, res;

	if( currentIndex >= 0 && currentIndex <= times.Num() ) {
		// use the cached index if it is still valid
		if( currentIndex == 0 ) {
			if( time <= times[currentIndex] ) { return currentIndex; }
		} else if( currentIndex == times.Num() ) {
			if( time > times[currentIndex - 1] ) { return currentIndex; }
		} else if( time > times[currentIndex - 1] && time <= times[currentIndex] ) {
			return currentIndex;
		} else if( time > times[currentIndex] && ( currentIndex + 1 == times.Num() || time <= times[currentIndex + 1] ) ) {
			// use the next index
			currentIndex++;
			return currentIndex;
		}
	}

	// use binary search to find the index for the given time
	len	   = times.Num();
	mid	   = len;
	offset = 0;
	res	   = 0;
	while( mid > 0 ) {
		mid = len >> 1;
		if( time == times[offset + mid] ) {
			return offset + mid;
		} else if( time > times[offset + mid] ) {
			offset += mid;
			len -= mid;
			res = 1;
		} else {
			len -= mid;
			res = 0;
		}
	}
	currentIndex = offset + res;
	return currentIndex;
}

template<class type>
ID_INLINE type idCurve<type>::ValueForIndex( const int index ) const
{
	int n = values.Num() - 1;

	if( index < 0 ) {
		return values[0] + index * ( values[1] - values[0] );
	} else if( index > n ) {
		return values[n] + ( index - n ) * ( values[n] - values[n - 1] );
	}
	return values[index];
}

template<class type>
ID_INLINE float idCurve<type>::TimeForIndex( const int index ) const
{
	int n = times.Num() - 1;

	if( index < 0 ) {
		return times[0] + index * ( times[1] - times[0] );
	} else if( index > n ) {
		return times[n] + ( index - n ) * ( times[n] - times[n - 1] );
	}
	return times[index];
}

/*!
	\class idCurve_Bezier
	\brief A template class for managing Bezier curve interpolation and derivatives for generic types.

	This class provides functionality for evaluating Bezier curves of arbitrary order for a given type. It inherits from idCurve and implements methods for computing interpolated values along with
   first and second derivatives at specified time points. The implementation includes basis function calculations for different orders and derivative computations. It is designed to work with any type
   that supports the necessary arithmetic operations for interpolation. The class is intended for use in applications requiring smooth curve evaluation and derivative information, such as animation or
   trajectory planning.

*/
template<class type>
class idCurve_Bezier : public idCurve<type>
{
public:
	//! Constructs a new idCurve_Bezier object.
	idCurve_Bezier();

	//! Returns the interpolated value at the specified time using Bezier curve evaluation
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the Bezier curve at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the Bézier curve at the given time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	//! Computes the Bezier basis functions for a given order and parameter value.
	void Basis( const int order, const float t, float* bvals ) const;

	//! Computes the first derivative of Bezier basis functions for a given order and parameter value.
	void BasisFirstDerivative( const int order, const float t, float* bvals ) const;

	//! Calculates the second derivative of Bézier basis functions for a given order and parameter t.
	void BasisSecondDerivative( const int order, const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_Bezier<type>::idCurve_Bezier()
{
}

template<class type>
ID_INLINE type idCurve_Bezier<type>::GetCurrentValue( const float time ) const
{
	int	   i;
	float* bvals;
	type   v;

	bvals = ( float* )_alloca16( this->values.Num() * sizeof( float ) );

	Basis( this->values.Num(), time, bvals );
	v = bvals[0] * this->values[0];
	for( i = 1; i < this->values.Num(); i++ ) {
		v += bvals[i] * this->values[i];
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_Bezier<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	   i;
	float *bvals, d;
	type   v;

	bvals = ( float* )_alloca16( this->values.Num() * sizeof( float ) );

	BasisFirstDerivative( this->values.Num(), time, bvals );
	v = bvals[0] * this->values[0];
	for( i = 1; i < this->values.Num(); i++ ) {
		v += bvals[i] * this->values[i];
	}
	d = ( this->times[this->times.Num() - 1] - this->times[0] );
	return ( ( float )( this->values.Num() - 1 ) / d ) * v;
}

template<class type>
ID_INLINE type idCurve_Bezier<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	   i;
	float *bvals, d;
	type   v;

	bvals = ( float* )_alloca16( this->values.Num() * sizeof( float ) );

	BasisSecondDerivative( this->values.Num(), time, bvals );
	v = bvals[0] * this->values[0];
	for( i = 1; i < this->values.Num(); i++ ) {
		v += bvals[i] * this->values[i];
	}
	d = ( this->times[this->times.Num() - 1] - this->times[0] );
	return ( ( float )( this->values.Num() - 2 ) * ( this->values.Num() - 1 ) / ( d * d ) ) * v;
}

template<class type>
ID_INLINE void idCurve_Bezier<type>::Basis( const int order, const float t, float* bvals ) const
{
	int	   i, j, d;
	float *c, c1, c2, s, o, ps, po;

	bvals[0] = 1.0f;
	d		 = order - 1;
	if( d <= 0 ) { return; }

	c  = ( float* )_alloca16( ( d + 1 ) * sizeof( float ) );
	s  = ( float )( t - this->times[0] ) / ( this->times[this->times.Num() - 1] - this->times[0] );
	o  = 1.0f - s;
	ps = s;
	po = o;

	for( i = 1; i < d; i++ ) {
		c[i] = 1.0f;
	}
	for( i = 1; i < d; i++ ) {
		c[i - 1] = 0.0f;
		c1		 = c[i];
		c[i]	 = 1.0f;
		for( j = i + 1; j <= d; j++ ) {
			c2	 = c[j];
			c[j] = c1 + c[j - 1];
			c1	 = c2;
		}
		bvals[i] = c[d] * ps;
		ps *= s;
	}
	for( i = d - 1; i >= 0; i-- ) {
		bvals[i] *= po;
		po *= o;
	}
	bvals[d] = ps;
}

template<class type>
ID_INLINE void idCurve_Bezier<type>::BasisFirstDerivative( const int order, const float t, float* bvals ) const
{
	int i;

	Basis( order - 1, t, bvals + 1 );
	bvals[0] = 0.0f;
	for( i = 0; i < order - 1; i++ ) {
		bvals[i] -= bvals[i + 1];
	}
}

template<class type>
ID_INLINE void idCurve_Bezier<type>::BasisSecondDerivative( const int order, const float t, float* bvals ) const
{
	int i;

	BasisFirstDerivative( order - 1, t, bvals + 1 );
	bvals[0] = 0.0f;
	for( i = 0; i < order - 1; i++ ) {
		bvals[i] -= bvals[i + 1];
	}
}

/*!
	\class idCurve_QuadraticBezier
	\brief A template class for representing and evaluating quadratic Bézier curves.

	This class implements a quadratic Bézier curve interpolation mechanism, inheriting from a generic curve base class. It provides methods to evaluate the curve's value and its derivatives at
   specific time parameters. The implementation supports custom data types through templating, enabling use with various numeric types or vector types. The curve is defined by its control points and
   uses standard Bézier basis functions for interpolation. The class exposes internal evaluation methods for basis functions and their derivatives, allowing for flexible curve manipulation and
   computation. The design supports both direct curve evaluation and derivative computation for applications requiring smooth interpolation and motion paths.

*/
template<class type>
class idCurve_QuadraticBezier : public idCurve<type>
{
public:
	//! Constructs a new quadratic Bézier curve object.
	idCurve_QuadraticBezier();

	//! Returns the interpolated value at the given time for a quadratic Bézier curve
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the quadratic Bézier curve at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the quadratic Bézier curve at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	//! Evaluates the quadratic Bézier basis functions at the given parameter value.
	void Basis( const float t, float* bvals ) const;

	//! Computes the first derivative of the quadratic Bézier basis functions at the given parameter value.
	void BasisFirstDerivative( const float t, float* bvals ) const;

	//! Computes the second derivative of the quadratic Bézier basis functions at the given parameter value.
	void BasisSecondDerivative( const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_QuadraticBezier<type>::idCurve_QuadraticBezier()
{
}

template<class type>
ID_INLINE type idCurve_QuadraticBezier<type>::GetCurrentValue( const float time ) const
{
	float bvals[3];
	assert( this->values.Num() == 3 );
	Basis( time, bvals );
	return ( bvals[0] * this->values[0] + bvals[1] * this->values[1] + bvals[2] * this->values[2] );
}

template<class type>
ID_INLINE type idCurve_QuadraticBezier<type>::GetCurrentFirstDerivative( const float time ) const
{
	float bvals[3], d;
	assert( this->values.Num() == 3 );
	BasisFirstDerivative( time, bvals );
	d = ( this->times[2] - this->times[0] );
	return ( bvals[0] * this->values[0] + bvals[1] * this->values[1] + bvals[2] * this->values[2] ) / d;
}

template<class type>
ID_INLINE type idCurve_QuadraticBezier<type>::GetCurrentSecondDerivative( const float time ) const
{
	float bvals[3], d;
	assert( this->values.Num() == 3 );
	BasisSecondDerivative( time, bvals );
	d = ( this->times[2] - this->times[0] );
	return ( bvals[0] * this->values[0] + bvals[1] * this->values[1] + bvals[2] * this->values[2] ) / ( d * d );
}

template<class type>
ID_INLINE void idCurve_QuadraticBezier<type>::Basis( const float t, float* bvals ) const
{
	float s1 = ( float )( t - this->times[0] ) / ( this->times[2] - this->times[0] );
	float s2 = s1 * s1;
	bvals[0] = s2 - 2.0f * s1 + 1.0f;
	bvals[1] = -2.0f * s2 + 2.0f * s1;
	bvals[2] = s2;
}

template<class type>
ID_INLINE void idCurve_QuadraticBezier<type>::BasisFirstDerivative( const float t, float* bvals ) const
{
	float s1 = ( float )( t - this->times[0] ) / ( this->times[2] - this->times[0] );
	bvals[0] = 2.0f * s1 - 2.0f;
	bvals[1] = -4.0f * s1 + 2.0f;
	bvals[2] = 2.0f * s1;
}

template<class type>
ID_INLINE void idCurve_QuadraticBezier<type>::BasisSecondDerivative( const float t, float* bvals ) const
{
	float s1 = ( float )( t - this->times[0] ) / ( this->times[2] - this->times[0] );
	bvals[0] = 2.0f;
	bvals[1] = -4.0f;
	bvals[2] = 2.0f;
}

/*!
	\class idCurve_CubicBezier
	\brief A template class for representing and evaluating cubic Bézier curves with support for derivatives.

	This class implements a cubic Bézier curve interpolation method, inheriting from a generic curve base class. It provides methods to compute interpolated values, first and second derivatives at
   specified time points. The implementation includes basis function calculations for the curve evaluation and its derivatives. The class is templated to allow for different value types, making it
   flexible for various curve applications.

*/
template<class type>
class idCurve_CubicBezier : public idCurve<type>
{
public:
	//! Constructs a new cubic Bezier curve object.
	idCurve_CubicBezier();

	//! Returns the interpolated value at the given time for a cubic Bézier curve
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the cubic Bézier curve at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the cubic Bézier curve at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	//! Computes the cubic Bezier basis functions for the given parameter t and stores the results in bvals.
	void Basis( const float t, float* bvals ) const;

	//! Computes the first derivative of the cubic Bézier basis functions at the given parameter value.
	void BasisFirstDerivative( const float t, float* bvals ) const;

	//! Calculates the second derivative of the cubic Bezier basis functions at the given parameter value.
	void BasisSecondDerivative( const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_CubicBezier<type>::idCurve_CubicBezier()
{
}

template<class type>
ID_INLINE type idCurve_CubicBezier<type>::GetCurrentValue( const float time ) const
{
	float bvals[4];
	assert( this->values.Num() == 4 );
	Basis( time, bvals );
	return ( bvals[0] * this->values[0] + bvals[1] * this->values[1] + bvals[2] * this->values[2] + bvals[3] * this->values[3] );
}

template<class type>
ID_INLINE type idCurve_CubicBezier<type>::GetCurrentFirstDerivative( const float time ) const
{
	float bvals[4], d;
	assert( this->values.Num() == 4 );
	BasisFirstDerivative( time, bvals );
	d = ( this->times[3] - this->times[0] );
	return ( bvals[0] * this->values[0] + bvals[1] * this->values[1] + bvals[2] * this->values[2] + bvals[3] * this->values[3] ) / d;
}

template<class type>
ID_INLINE type idCurve_CubicBezier<type>::GetCurrentSecondDerivative( const float time ) const
{
	float bvals[4], d;
	assert( this->values.Num() == 4 );
	BasisSecondDerivative( time, bvals );
	d = ( this->times[3] - this->times[0] );
	return ( bvals[0] * this->values[0] + bvals[1] * this->values[1] + bvals[2] * this->values[2] + bvals[3] * this->values[3] ) / ( d * d );
}

template<class type>
ID_INLINE void idCurve_CubicBezier<type>::Basis( const float t, float* bvals ) const
{
	float s1 = ( float )( t - this->times[0] ) / ( this->times[3] - this->times[0] );
	float s2 = s1 * s1;
	float s3 = s2 * s1;
	bvals[0] = -s3 + 3.0f * s2 - 3.0f * s1 + 1.0f;
	bvals[1] = 3.0f * s3 - 6.0f * s2 + 3.0f * s1;
	bvals[2] = -3.0f * s3 + 3.0f * s2;
	bvals[3] = s3;
}

template<class type>
ID_INLINE void idCurve_CubicBezier<type>::BasisFirstDerivative( const float t, float* bvals ) const
{
	float s1 = ( float )( t - this->times[0] ) / ( this->times[3] - this->times[0] );
	float s2 = s1 * s1;
	bvals[0] = -3.0f * s2 + 6.0f * s1 - 3.0f;
	bvals[1] = 9.0f * s2 - 12.0f * s1 + 3.0f;
	bvals[2] = -9.0f * s2 + 6.0f * s1;
	bvals[3] = 3.0f * s2;
}

template<class type>
ID_INLINE void idCurve_CubicBezier<type>::BasisSecondDerivative( const float t, float* bvals ) const
{
	float s1 = ( float )( t - this->times[0] ) / ( this->times[3] - this->times[0] );
	bvals[0] = -6.0f * s1 + 6.0f;
	bvals[1] = 18.0f * s1 - 12.0f;
	bvals[2] = -18.0f * s1 + 6.0f;
	bvals[3] = 6.0f * s1;
}

//! Initializes a new instance of the idCurve_Spline class with default boundary type and close time.
template<class type>
class idCurve_Spline : public idCurve<type>
{
public:
	enum boundary_t { BT_FREE, BT_CLAMPED, BT_CLOSED };

	idCurve_Spline();

	//! Returns true if the spline curve has completed by the given time and the boundary type is not closed.
	virtual bool IsDone( const float time ) const;

	//! Sets the boundary type for the spline curve and marks the curve as changed.
	virtual void SetBoundaryType( const boundary_t bt )
	{
		boundaryType  = bt;
		this->changed = true;
	}

	//! Returns the boundary type of the spline curve.
	virtual boundary_t GetBoundaryType() const { return boundaryType; }

	//! Sets the close time value and marks the curve as changed.
	virtual void	   SetCloseTime( const float t )
	{
		closeTime	  = t;
		this->changed = true;
	}

	//! Returns the close time for a closed spline curve, or zero if the curve is not closed.
	virtual float GetCloseTime() { return boundaryType == BT_CLOSED ? closeTime : 0.0f; }

protected:
	boundary_t boundaryType;
	float	   closeTime;

	//! Returns the value at the specified index, handling boundary conditions for closed and open curves
	type	   ValueForIndex( const int index ) const;

	//! Returns the time value for a given index, handling boundary conditions for closed or open splines.
	float	   TimeForIndex( const int index ) const;

	//! Returns the clamped time value based on the boundary type and input time.
	float	   ClampedTime( const float t ) const;
};

template<class type>
ID_INLINE idCurve_Spline<type>::idCurve_Spline()
{
	boundaryType = BT_FREE;
	closeTime	 = 0.0f;
}

template<class type>
ID_INLINE type idCurve_Spline<type>::ValueForIndex( const int index ) const
{
	int n = this->values.Num() - 1;

	if( index < 0 ) {
		if( boundaryType == BT_CLOSED ) {
			return this->values[this->values.Num() + index % this->values.Num()];
		} else {
			return this->values[0] + index * ( this->values[1] - this->values[0] );
		}
	} else if( index > n ) {
		if( boundaryType == BT_CLOSED ) {
			return this->values[index % this->values.Num()];
		} else {
			return this->values[n] + ( index - n ) * ( this->values[n] - this->values[n - 1] );
		}
	}
	return this->values[index];
}

template<class type>
ID_INLINE float idCurve_Spline<type>::TimeForIndex( const int index ) const
{
	int n = this->times.Num() - 1;

	if( index < 0 ) {
		if( boundaryType == BT_CLOSED ) {
			return ( index / this->times.Num() ) * ( this->times[n] + closeTime ) - ( this->times[n] + closeTime - this->times[this->times.Num() + index % this->times.Num()] );
		} else {
			return this->times[0] + index * ( this->times[1] - this->times[0] );
		}
	} else if( index > n ) {
		if( boundaryType == BT_CLOSED ) {
			return ( index / this->times.Num() ) * ( this->times[n] + closeTime ) + this->times[index % this->times.Num()];
		} else {
			return this->times[n] + ( index - n ) * ( this->times[n] - this->times[n - 1] );
		}
	}
	return this->times[index];
}

template<class type>
ID_INLINE float idCurve_Spline<type>::ClampedTime( const float t ) const
{
	if( boundaryType == BT_CLAMPED ) {
		if( t < this->times[0] ) {
			return this->times[0];
		} else if( t >= this->times[this->times.Num() - 1] ) {
			return this->times[this->times.Num() - 1];
		}
	}
	return t;
}

template<class type>
ID_INLINE bool idCurve_Spline<type>::IsDone( const float time ) const
{
	return ( boundaryType != BT_CLOSED && time >= this->times[this->times.Num() - 1] );
}

/*!
	\class idCurve_NaturalCubicSpline
	\brief A template class for managing natural cubic spline interpolation curves with various boundary conditions.

	This class provides a specialized implementation of spline interpolation using natural cubic splines, inheriting from a base spline class. It supports different boundary conditions such as free,
   clamped, and closed curves, allowing for flexible curve fitting. The class maintains internal state for interpolation coefficients and provides methods to evaluate curve values and their
   derivatives at specific time points. The implementation handles the setup and computation of spline coefficients based on the chosen boundary type. It is designed for smooth curve interpolation and
   derivative evaluation in time-based applications.

*/
template<class type>
class idCurve_NaturalCubicSpline : public idCurve_Spline<type>
{
public:
	//! Constructs a new idCurve_NaturalCubicSpline object.
	idCurve_NaturalCubicSpline();

	//! Clears all data stored in the natural cubic spline curve.
	virtual void Clear()
	{
		idCurve_Spline<type>::Clear();
		this->values.Clear();
		b.Clear();
		c.Clear();
		d.Clear();
	}

	//! Returns the interpolated value for the given time using natural cubic spline interpolation.
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the spline at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the spline at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	mutable idList<type> b;
	mutable idList<type> c;
	mutable idList<type> d;

	//! Updates the spline setup based on the current boundary type and changed state.
	void				 Setup() const;

	//! Initializes the spline coefficients for free boundary conditions
	void				 SetupFree() const;

	//! Sets up the clamped natural cubic spline interpolation coefficients for the curve
	void				 SetupClamped() const;

	//! Prepares the spline for closed curve evaluation by computing coefficients.
	void				 SetupClosed() const;
};

template<class type>
ID_INLINE idCurve_NaturalCubicSpline<type>::idCurve_NaturalCubicSpline()
{
}

template<class type>
ID_INLINE type idCurve_NaturalCubicSpline<type>::GetCurrentValue( const float time ) const
{
	float clampedTime = this->ClampedTime( time );
	int	  i			  = this->IndexForTime( clampedTime );
	float s			  = time - this->TimeForIndex( i );
	Setup();
	return ( this->values[i] + s * ( b[i] + s * ( c[i] + s * d[i] ) ) );
}

template<class type>
ID_INLINE type idCurve_NaturalCubicSpline<type>::GetCurrentFirstDerivative( const float time ) const
{
	float clampedTime = this->ClampedTime( time );
	int	  i			  = this->IndexForTime( clampedTime );
	float s			  = time - this->TimeForIndex( i );
	Setup();
	return ( b[i] + s * ( 2.0f * c[i] + 3.0f * s * d[i] ) );
}

template<class type>
ID_INLINE type idCurve_NaturalCubicSpline<type>::GetCurrentSecondDerivative( const float time ) const
{
	float clampedTime = this->ClampedTime( time );
	int	  i			  = this->IndexForTime( clampedTime );
	float s			  = time - this->TimeForIndex( i );
	Setup();
	return ( 2.0f * c[i] + 6.0f * s * d[i] );
}

template<class type>
ID_INLINE void idCurve_NaturalCubicSpline<type>::Setup() const
{
	if( this->changed ) {
		switch( this->boundaryType ) {
			case idCurve_Spline<type>::BT_FREE:
				SetupFree();
				break;
			case idCurve_Spline<type>::BT_CLAMPED:
				SetupClamped();
				break;
			case idCurve_Spline<type>::BT_CLOSED:
				SetupClosed();
				break;
		}
		this->changed = false;
	}
}

template<class type>
ID_INLINE void idCurve_NaturalCubicSpline<type>::SetupFree() const
{
	int	   i;
	float  inv;
	float *d0, *d1, *beta, *gamma;
	type * alpha, *delta;

	d0	  = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	d1	  = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	alpha = ( type* )_alloca16( ( this->values.Num() - 1 ) * sizeof( type ) );
	beta  = ( float* )_alloca16( this->values.Num() * sizeof( float ) );
	gamma = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	delta = ( type* )_alloca16( this->values.Num() * sizeof( type ) );

	for( i = 0; i < this->values.Num() - 1; i++ ) {
		d0[i] = this->times[i + 1] - this->times[i];
	}

	for( i = 1; i < this->values.Num() - 1; i++ ) {
		d1[i] = this->times[i + 1] - this->times[i - 1];
	}

	for( i = 1; i < this->values.Num() - 1; i++ ) {
		type sum = 3.0f * ( d0[i - 1] * this->values[i + 1] - d1[i] * this->values[i] + d0[i] * this->values[i - 1] );
		inv		 = 1.0f / ( d0[i - 1] * d0[i] );
		alpha[i] = inv * sum;
	}

	beta[0]	 = 1.0f;
	gamma[0] = 0.0f;
	delta[0] = this->values[0] - this->values[0]; //-V501

	for( i = 1; i < this->values.Num() - 1; i++ ) {
		beta[i]	 = 2.0f * d1[i] - d0[i - 1] * gamma[i - 1];
		inv		 = 1.0f / beta[i];
		gamma[i] = inv * d0[i];
		delta[i] = inv * ( alpha[i] - d0[i - 1] * delta[i - 1] );
	}
	beta[this->values.Num() - 1]  = 1.0f;
	delta[this->values.Num() - 1] = this->values[0] - this->values[0]; //-V501

	b.AssureSize( this->values.Num() );
	c.AssureSize( this->values.Num() );
	d.AssureSize( this->values.Num() );

	c[this->values.Num() - 1] = this->values[0] - this->values[0]; //-V501

	for( i = this->values.Num() - 2; i >= 0; i-- ) {
		c[i] = delta[i] - gamma[i] * c[i + 1];
		inv	 = 1.0f / d0[i];
		b[i] = inv * ( this->values[i + 1] - this->values[i] ) - ( 1.0f / 3.0f ) * d0[i] * ( c[i + 1] + 2.0f * c[i] );
		d[i] = ( 1.0f / 3.0f ) * inv * ( c[i + 1] - c[i] );
	}
}

template<class type>
ID_INLINE void idCurve_NaturalCubicSpline<type>::SetupClamped() const
{
	int	   i;
	float  inv;
	float *d0, *d1, *beta, *gamma;
	type * alpha, *delta;

	d0	  = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	d1	  = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	alpha = ( type* )_alloca16( ( this->values.Num() - 1 ) * sizeof( type ) );
	beta  = ( float* )_alloca16( this->values.Num() * sizeof( float ) );
	gamma = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	delta = ( type* )_alloca16( this->values.Num() * sizeof( type ) );

	for( i = 0; i < this->values.Num() - 1; i++ ) {
		d0[i] = this->times[i + 1] - this->times[i];
	}

	for( i = 1; i < this->values.Num() - 1; i++ ) {
		d1[i] = this->times[i + 1] - this->times[i - 1];
	}

	inv							  = 1.0f / d0[0];
	alpha[0]					  = 3.0f * ( inv - 1.0f ) * ( this->values[1] - this->values[0] );
	inv							  = 1.0f / d0[this->values.Num() - 2];
	alpha[this->values.Num() - 1] = 3.0f * ( 1.0f - inv ) * ( this->values[this->values.Num() - 1] - this->values[this->values.Num() - 2] );

	for( i = 1; i < this->values.Num() - 1; i++ ) {
		type sum = 3.0f * ( d0[i - 1] * this->values[i + 1] - d1[i] * this->values[i] + d0[i] * this->values[i - 1] );
		inv		 = 1.0f / ( d0[i - 1] * d0[i] );
		alpha[i] = inv * sum;
	}

	beta[0]	 = 2.0f * d0[0];
	gamma[0] = 0.5f;
	inv		 = 1.0f / beta[0];
	delta[0] = inv * alpha[0];

	for( i = 1; i < this->values.Num() - 1; i++ ) {
		beta[i]	 = 2.0f * d1[i] - d0[i - 1] * gamma[i - 1];
		inv		 = 1.0f / beta[i];
		gamma[i] = inv * d0[i];
		delta[i] = inv * ( alpha[i] - d0[i - 1] * delta[i - 1] );
	}

	beta[this->values.Num() - 1]  = d0[this->values.Num() - 2] * ( 2.0f - gamma[this->values.Num() - 2] );
	inv							  = 1.0f / beta[this->values.Num() - 1];
	delta[this->values.Num() - 1] = inv * ( alpha[this->values.Num() - 1] - d0[this->values.Num() - 2] * delta[this->values.Num() - 2] );

	b.AssureSize( this->values.Num() );
	c.AssureSize( this->values.Num() );
	d.AssureSize( this->values.Num() );

	c[this->values.Num() - 1] = delta[this->values.Num() - 1];

	for( i = this->values.Num() - 2; i >= 0; i-- ) {
		c[i] = delta[i] - gamma[i] * c[i + 1];
		inv	 = 1.0f / d0[i];
		b[i] = inv * ( this->values[i + 1] - this->values[i] ) - ( 1.0f / 3.0f ) * d0[i] * ( c[i + 1] + 2.0f * c[i] );
		d[i] = ( 1.0f / 3.0f ) * inv * ( c[i + 1] - c[i] );
	}
}

template<class type>
ID_INLINE void idCurve_NaturalCubicSpline<type>::SetupClosed() const
{
	int	   i, j;
	float  c0, c1;
	float* d0;
	idMatX mat;
	idVecX x;

	d0 = ( float* )_alloca16( ( this->values.Num() - 1 ) * sizeof( float ) );
	x.SetData( this->values.Num(), VECX_ALLOCA( this->values.Num() ) );
	mat.SetData( this->values.Num(), this->values.Num(), MATX_ALLOCA( this->values.Num() * this->values.Num() ) );

	b.AssureSize( this->values.Num() );
	c.AssureSize( this->values.Num() );
	d.AssureSize( this->values.Num() );

	for( i = 0; i < this->values.Num() - 1; i++ ) {
		d0[i] = this->times[i + 1] - this->times[i];
	}

	// matrix of system
	mat[0][0]					   = 1.0f;
	mat[0][this->values.Num() - 1] = -1.0f;
	for( i = 1; i <= this->values.Num() - 2; i++ ) {
		mat[i][i - 1] = d0[i - 1];
		mat[i][i]	  = 2.0f * ( d0[i - 1] + d0[i] );
		mat[i][i + 1] = d0[i];
	}
	mat[this->values.Num() - 1][this->values.Num() - 2] = d0[this->values.Num() - 2];
	mat[this->values.Num() - 1][0]						= 2.0f * ( d0[this->values.Num() - 2] + d0[0] );
	mat[this->values.Num() - 1][1]						= d0[0];

	// right-hand side
	c[0].Zero();
	for( i = 1; i <= this->values.Num() - 2; i++ ) {
		c0	 = 1.0f / d0[i];
		c1	 = 1.0f / d0[i - 1];
		c[i] = 3.0f * ( c0 * ( this->values[i + 1] - this->values[i] ) - c1 * ( this->values[i] - this->values[i - 1] ) );
	}
	c0						  = 1.0f / d0[0];
	c1						  = 1.0f / d0[this->values.Num() - 2];
	c[this->values.Num() - 1] = 3.0f * ( c0 * ( this->values[1] - this->values[0] ) - c1 * ( this->values[0] - this->values[this->values.Num() - 2] ) );

	// solve system for each dimension
	mat.LU_Factor( NULL );
	for( i = 0; i < this->values[0].GetDimension(); i++ ) {
		for( j = 0; j < this->values.Num(); j++ ) {
			x[j] = c[j][i];
		}
		mat.LU_Solve( x, x, NULL );
		for( j = 0; j < this->values.Num(); j++ ) {
			c[j][i] = x[j];
		}
	}

	for( i = 0; i < this->values.Num() - 1; i++ ) {
		c0	 = 1.0f / d0[i];
		b[i] = c0 * ( this->values[i + 1] - this->values[i] ) - ( 1.0f / 3.0f ) * ( c[i + 1] + 2.0f * c[i] ) * d0[i];
		d[i] = ( 1.0f / 3.0f ) * c0 * ( c[i + 1] - c[i] );
	}
}

/*!
	\class idCurve_CatmullRomSpline
	\brief A template class implementing Catmull-Rom spline interpolation for smooth curve evaluation.

	This class provides a concrete implementation of Catmull-Rom spline interpolation, inheriting from a base spline curve class. It supports evaluation of curve values, first and second derivatives
   at specified times. The class is templated to work with different numeric types, making it flexible for various curve applications. The implementation includes methods for computing basis functions
   and their derivatives, which are fundamental to the Catmull-Rom spline interpolation algorithm. The class is designed for efficient curve evaluation and derivative computation in animation or path
   generation scenarios.

*/
template<class type>
class idCurve_CatmullRomSpline : public idCurve_Spline<type>
{
public:
	//! Constructs an instance of the Catmull-Rom spline curve.
	idCurve_CatmullRomSpline();

	//! Returns the interpolated value for the given time using Catmull-Rom spline interpolation.
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the curve at the given time
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the curve at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	//! Computes the Catmull-Rom spline basis functions for the given index and parameter.
	void Basis( const int index, const float t, float* bvals ) const;

	//! Computes the first derivative of Catmull-Rom spline basis functions for a given index and parameter
	void BasisFirstDerivative( const int index, const float t, float* bvals ) const;

	//! Computes the second derivative of the Catmull-Rom spline basis functions for a given index and parameter.
	void BasisSecondDerivative( const int index, const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_CatmullRomSpline<type>::idCurve_CatmullRomSpline()
{
}

template<class type>
ID_INLINE type idCurve_CatmullRomSpline<type>::GetCurrentValue( const float time ) const
{
	int	  i, j, k;
	float bvals[4], clampedTime;
	type  v;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	Basis( i - 1, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < 4; j++ ) {
		k = i + j - 2;
		v += bvals[j] * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_CatmullRomSpline<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	  i, j, k;
	float bvals[4], d, clampedTime;
	type  v;

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	BasisFirstDerivative( i - 1, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < 4; j++ ) {
		k = i + j - 2;
		v += bvals[j] * this->ValueForIndex( k );
	}
	d = ( this->TimeForIndex( i ) - this->TimeForIndex( i - 1 ) );
	return v / d;
}

template<class type>
ID_INLINE type idCurve_CatmullRomSpline<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	  i, j, k;
	float bvals[4], d, clampedTime;
	type  v;

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	BasisSecondDerivative( i - 1, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < 4; j++ ) {
		k = i + j - 2;
		v += bvals[j] * this->ValueForIndex( k );
	}
	d = ( this->TimeForIndex( i ) - this->TimeForIndex( i - 1 ) );
	return v / ( d * d );
}

template<class type>
ID_INLINE void idCurve_CatmullRomSpline<type>::Basis( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = ( ( -s + 2.0f ) * s - 1.0f ) * s * 0.5f;			  // -0.5f s * s * s + s * s - 0.5f * s
	bvals[1] = ( ( ( 3.0f * s - 5.0f ) * s ) * s + 2.0f ) * 0.5f; // 1.5f * s * s * s - 2.5f * s * s + 1.0f
	bvals[2] = ( ( -3.0f * s + 4.0f ) * s + 1.0f ) * s * 0.5f;	  // -1.5f * s * s * s - 2.0f * s * s + 0.5f s
	bvals[3] = ( ( s - 1.0f ) * s * s ) * 0.5f;					  // 0.5f * s * s * s - 0.5f * s * s
}

template<class type>
ID_INLINE void idCurve_CatmullRomSpline<type>::BasisFirstDerivative( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = ( -1.5f * s + 2.0f ) * s - 0.5f; // -1.5f * s * s + 2.0f * s - 0.5f
	bvals[1] = ( 4.5f * s - 5.0f ) * s;			// 4.5f * s * s - 5.0f * s
	bvals[2] = ( -4.5 * s + 4.0f ) * s + 0.5f;	// -4.5 * s * s + 4.0f * s + 0.5f
	bvals[3] = 1.5f * s * s - s;				// 1.5f * s * s - s
}

template<class type>
ID_INLINE void idCurve_CatmullRomSpline<type>::BasisSecondDerivative( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = -3.0f * s + 2.0f;
	bvals[1] = 9.0f * s - 5.0f;
	bvals[2] = -9.0f * s + 4.0f;
	bvals[3] = 3.0f * s - 1.0f;
}

/*!
	\class idCurve_KochanekBartelsSpline
	\brief A spline curve implementation using Kochanek-Bartels interpolation for smooth animation and trajectory generation.

	This class provides a template-based implementation of a Kochanek-Bartels spline curve, inheriting from a base spline class. It supports adding keyframe values with associated times and optional
   tension, continuity, and bias parameters to control the shape of the curve segments. The curve can compute interpolated values, first and second derivatives at given times, and basis function
   values for mathematical analysis. Keyframe data can be modified by removing specific indices or clearing all data entirely. The implementation is designed for efficient evaluation of smooth curves
   suitable for animation and simulation applications where precise control over curve behavior is required.

*/
template<class type>
class idCurve_KochanekBartelsSpline : public idCurve_Spline<type>
{
public:
	//! Constructs an empty Kochanek-Bartels spline curve.
	idCurve_KochanekBartelsSpline();

	//! Adds a timed/value pair to the Kochanek-Bartels spline and returns the index of the inserted pair.
	virtual int	 AddValue( const float time, const type& value );

	/*!
		\brief Adds a timed value pair to the Kochanek-Bartels spline and returns the index of the inserted pair.

		This function inserts a new keyframe into the spline with the specified time, value, and spline parameters. The spline parameters control the shape of the curve segments between keyframes. The
	   function determines the correct insertion position based on the time value and returns the index where the new keyframe was inserted. The tension parameter controls the tightness of the curve,
	   continuity controls the bias of the curve, and bias controls the direction of the curve segments.

		\param time The time value for the keyframe
		\param value The value for the keyframe
		\param tension The tension parameter for the spline curve
		\param continuity The continuity parameter for the spline curve
		\param bias The bias parameter for the spline curve
		\return The index where the new keyframe was inserted in the spline
	*/
	virtual int	 AddValue( const float time, const type& value, const float tension, const float continuity, const float bias );

	//! Removes the element at the specified index from all internal arrays of the spline.
	virtual void RemoveIndex( const int index )
	{
		this->values.RemoveIndex( index );
		this->times.RemoveIndex( index );
		tension.RemoveIndex( index );
		continuity.RemoveIndex( index );
		bias.RemoveIndex( index );
	}

	//! Clears all stored values, times, and curve data for the Kochanek-Bartels spline.
	virtual void Clear()
	{
		this->values.Clear();
		this->times.Clear();
		tension.Clear();
		continuity.Clear();
		bias.Clear();
		this->currentIndex = -1;
	}

	//! Returns the interpolated value for the given time using Kochanek-Bartels spline interpolation.
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the spline at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the spline at the specified time
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	idList<float> tension;
	idList<float> continuity;
	idList<float> bias;

	//! Computes outgoing and incoming tangents for a specified index in a Kochanek-Bartels spline.
	void		  TangentsForIndex( const int index, type& t0, type& t1 ) const;

	//! Computes the basis values for a Kochanek-Bartels spline at a given index and time.
	void		  Basis( const int index, const float t, float* bvals ) const;

	//! Computes the first derivative of the spline basis functions for a given index and time
	void		  BasisFirstDerivative( const int index, const float t, float* bvals ) const;

	//! Computes the second derivative of the spline basis functions for a given index and time.
	void		  BasisSecondDerivative( const int index, const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_KochanekBartelsSpline<type>::idCurve_KochanekBartelsSpline()
{
}

template<class type>
ID_INLINE int idCurve_KochanekBartelsSpline<type>::AddValue( const float time, const type& value )
{
	int i;

	i = this->IndexForTime( time );
	this->times.Insert( time, i );
	this->values.Insert( value, i );
	tension.Insert( 0.0f, i );
	continuity.Insert( 0.0f, i );
	bias.Insert( 0.0f, i );
	return i;
}

/*
====================
idCurve_KochanekBartelsSpline::AddValue

  add a timed/value pair to the spline
  returns the index to the inserted pair
====================
*/
template<class type>
ID_INLINE int idCurve_KochanekBartelsSpline<type>::AddValue( const float time, const type& value, const float tension, const float continuity, const float bias )
{
	int i;

	i = this->IndexForTime( time );
	this->times.Insert( time, i );
	this->values.Insert( value, i );
	this->tension.Insert( tension, i );
	this->continuity.Insert( continuity, i );
	this->bias.Insert( bias, i );
	return i;
}

template<class type>
ID_INLINE type idCurve_KochanekBartelsSpline<type>::GetCurrentValue( const float time ) const
{
	int	  i;
	float bvals[4], clampedTime;
	type  v, t0, t1;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	TangentsForIndex( i - 1, t0, t1 );
	Basis( i - 1, clampedTime, bvals );
	v = bvals[0] * this->ValueForIndex( i - 1 );
	v += bvals[1] * this->ValueForIndex( i );
	v += bvals[2] * t0;
	v += bvals[3] * t1;
	return v;
}

template<class type>
ID_INLINE type idCurve_KochanekBartelsSpline<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	  i;
	float bvals[4], d, clampedTime;
	type  v, t0, t1;

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	TangentsForIndex( i - 1, t0, t1 );
	BasisFirstDerivative( i - 1, clampedTime, bvals );
	v = bvals[0] * this->ValueForIndex( i - 1 );
	v += bvals[1] * this->ValueForIndex( i );
	v += bvals[2] * t0;
	v += bvals[3] * t1;
	d = ( this->TimeForIndex( i ) - this->TimeForIndex( i - 1 ) );
	return v / d;
}

template<class type>
ID_INLINE type idCurve_KochanekBartelsSpline<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	  i;
	float bvals[4], d, clampedTime;
	type  v, t0, t1;

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	TangentsForIndex( i - 1, t0, t1 );
	BasisSecondDerivative( i - 1, clampedTime, bvals );
	v = bvals[0] * this->ValueForIndex( i - 1 );
	v += bvals[1] * this->ValueForIndex( i );
	v += bvals[2] * t0;
	v += bvals[3] * t1;
	d = ( this->TimeForIndex( i ) - this->TimeForIndex( i - 1 ) );
	return v / ( d * d );
}

template<class type>
ID_INLINE void idCurve_KochanekBartelsSpline<type>::TangentsForIndex( const int index, type& t0, type& t1 ) const
{
	float dt, omt, omc, opc, omb, opb, adj, s0, s1;
	type  delta;

	delta = this->ValueForIndex( index + 1 ) - this->ValueForIndex( index );
	dt	  = this->TimeForIndex( index + 1 ) - this->TimeForIndex( index );

	omt = 1.0f - tension[index];
	omc = 1.0f - continuity[index];
	opc = 1.0f + continuity[index];
	omb = 1.0f - bias[index];
	opb = 1.0f + bias[index];
	adj = 2.0f * dt / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index - 1 ) );
	s0	= 0.5f * adj * omt * opc * opb;
	s1	= 0.5f * adj * omt * omc * omb;

	// outgoing tangent at first point
	t0 = s1 * delta + s0 * ( this->ValueForIndex( index ) - this->ValueForIndex( index - 1 ) );

	omt = 1.0f - tension[index + 1];
	omc = 1.0f - continuity[index + 1];
	opc = 1.0f + continuity[index + 1];
	omb = 1.0f - bias[index + 1];
	opb = 1.0f + bias[index + 1];
	adj = 2.0f * dt / ( this->TimeForIndex( index + 2 ) - this->TimeForIndex( index ) );
	s0	= 0.5f * adj * omt * omc * opb;
	s1	= 0.5f * adj * omt * opc * omb;

	// incoming tangent at second point
	t1 = s1 * ( this->ValueForIndex( index + 2 ) - this->ValueForIndex( index + 1 ) ) + s0 * delta;
}

template<class type>
ID_INLINE void idCurve_KochanekBartelsSpline<type>::Basis( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = ( ( 2.0f * s - 3.0f ) * s ) * s + 1.0f; // 2.0f * s * s * s - 3.0f * s * s + 1.0f
	bvals[1] = ( ( -2.0f * s + 3.0f ) * s ) * s;	   // -2.0f * s * s * s + 3.0f * s * s
	bvals[2] = ( ( s - 2.0f ) * s ) * s + s;		   // s * s * s - 2.0f * s * s + s
	bvals[3] = ( ( s - 1.0f ) * s ) * s;			   // s * s * s - s * s
}

template<class type>
ID_INLINE void idCurve_KochanekBartelsSpline<type>::BasisFirstDerivative( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = ( 6.0f * s - 6.0f ) * s;		   // 6.0f * s * s - 6.0f * s
	bvals[1] = ( -6.0f * s + 6.0f ) * s;	   // -6.0f * s * s + 6.0f * s
	bvals[2] = ( 3.0f * s - 4.0f ) * s + 1.0f; // 3.0f * s * s - 4.0f * s + 1.0f
	bvals[3] = ( 3.0f * s - 2.0f ) * s;		   // 3.0f * s * s - 2.0f * s
}

template<class type>
ID_INLINE void idCurve_KochanekBartelsSpline<type>::BasisSecondDerivative( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = 12.0f * s - 6.0f;
	bvals[1] = -12.0f * s + 6.0f;
	bvals[2] = 6.0f * s - 4.0f;
	bvals[3] = 6.0f * s - 2.0f;
}

/*!
	\class idCurve_BSpline
	\brief A templated B-spline curve implementation that provides smooth interpolation and derivatives for curve evaluation.

	This class implements a B-spline curve evaluator with support for different curve orders. It inherits from a spline base class and provides methods for setting and retrieving the curve order, as
   well as evaluating the curve's value and its first and second derivatives at specified time parameters. The implementation uses basis functions to compute smooth interpolations, making it suitable
   for generating continuous curves with desired smoothness properties. The class is templated to allow for different data types, enabling its use with various numeric types while maintaining the
   mathematical properties of B-spline curves.

*/
template<class type>
class idCurve_BSpline : public idCurve_Spline<type>
{
public:
	//! Initializes a new instance of the idCurve_BSpline class with a default cubic order.
	idCurve_BSpline();

	//! Returns the order of the B-spline curve.
	virtual int	 GetOrder() const { return order; }

	//! Sets the order of the B-spline curve.
	virtual void SetOrder( const int i )
	{
		assert( i > 0 && i < 10 );
		order = i;
	}

	//! Returns the interpolated value for the given time using B-spline curve evaluation
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the B-spline curve at the specified time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the B-spline curve at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	int	  order;

	//! Evaluates the basis function for a B-spline curve at a given index, order, and parameter t.
	float Basis( const int index, const int order, const float t ) const;

	//! Computes the first derivative of a spline basis function for the given index, order, and parameter t.
	float BasisFirstDerivative( const int index, const int order, const float t ) const;

	//! Computes the second derivative of a spline basis function for the given index, order, and time parameter.
	float BasisSecondDerivative( const int index, const int order, const float t ) const;
};

template<class type>
ID_INLINE idCurve_BSpline<type>::idCurve_BSpline()
{
	order = 4; // default to cubic
}

template<class type>
ID_INLINE type idCurve_BSpline<type>::GetCurrentValue( const float time ) const
{
	int	  i, j, k;
	float clampedTime;
	type  v;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	v			= this->values[0] - this->values[0]; //-V501
	for( j = 0; j < order; j++ ) {
		k = i + j - ( order >> 1 );
		v += Basis( k - 2, order, clampedTime ) * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_BSpline<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	  i, j, k;
	float clampedTime;
	type  v;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	v			= this->values[0] - this->values[0]; //-V501
	for( j = 0; j < order; j++ ) {
		k = i + j - ( order >> 1 );
		v += BasisFirstDerivative( k - 2, order, clampedTime ) * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_BSpline<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	  i, j, k;
	float clampedTime;
	type  v;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	v			= this->values[0] - this->values[0]; //-V501
	for( j = 0; j < order; j++ ) {
		k = i + j - ( order >> 1 );
		v += BasisSecondDerivative( k - 2, order, clampedTime ) * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE float idCurve_BSpline<type>::Basis( const int index, const int order, const float t ) const
{
	if( order <= 1 ) {
		if( this->TimeForIndex( index ) < t && t <= this->TimeForIndex( index + 1 ) ) {
			return 1.0f;
		} else {
			return 0.0f;
		}
	} else {
		float sum = 0.0f;
		float d1  = this->TimeForIndex( index + order - 1 ) - this->TimeForIndex( index );
		if( d1 != 0.0f ) { sum += ( float )( t - this->TimeForIndex( index ) ) * Basis( index, order - 1, t ) / d1; }

		float d2 = this->TimeForIndex( index + order ) - this->TimeForIndex( index + 1 );
		if( d2 != 0.0f ) { sum += ( float )( this->TimeForIndex( index + order ) - t ) * Basis( index + 1, order - 1, t ) / d2; }
		return sum;
	}
}

template<class type>
ID_INLINE float idCurve_BSpline<type>::BasisFirstDerivative( const int index, const int order, const float t ) const
{
	return ( Basis( index, order - 1, t ) - Basis( index + 1, order - 1, t ) ) * ( float )( order - 1 ) / ( this->TimeForIndex( index + ( order - 1 ) - 2 ) - this->TimeForIndex( index - 2 ) );
}

template<class type>
ID_INLINE float idCurve_BSpline<type>::BasisSecondDerivative( const int index, const int order, const float t ) const
{
	return ( BasisFirstDerivative( index, order - 1, t ) - BasisFirstDerivative( index + 1, order - 1, t ) ) * ( float )( order - 1 ) /
		   ( this->TimeForIndex( index + ( order - 1 ) - 2 ) - this->TimeForIndex( index - 2 ) );
}

/*!
	\class idCurve_UniformCubicBSpline
	\brief A uniform cubic B-spline curve implementation for smooth interpolation and derivative calculations.

	This class provides a specialized implementation of a uniform cubic B-spline curve, designed for smooth interpolation of values over time. It inherits from a base B-spline class and is templated
   to work with different types. The curve is initialized with a fixed order of four, making it suitable for smooth transitions in animation or simulation. The class supports retrieving interpolated
   values, first derivatives, and second derivatives at specified time points. It also provides methods for computing basis functions and their derivatives, enabling detailed control over the curve's
   behavior. The implementation is optimized for performance with inline functions, making it suitable for real-time applications where speed is critical.

*/
template<class type>
class idCurve_UniformCubicBSpline : public idCurve_BSpline<type>
{
public:
	//! Initializes a uniform cubic B-spline curve with a fixed order of four.
	idCurve_UniformCubicBSpline();

	//! Returns the interpolated value for the given time using uniform cubic B-spline interpolation.
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the curve at the specified time
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the curve at the given time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	//! Computes the basis values for a uniform cubic B-spline at the given index and parameter.
	void Basis( const int index, const float t, float* bvals ) const;

	//! Computes the first derivative of the spline basis functions for a given index and parameter value.
	void BasisFirstDerivative( const int index, const float t, float* bvals ) const;

	//! Computes the second derivatives of the spline basis functions for a given index and parameter.
	void BasisSecondDerivative( const int index, const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_UniformCubicBSpline<type>::idCurve_UniformCubicBSpline()
{
	this->order = 4; // always cubic
}

template<class type>
ID_INLINE type idCurve_UniformCubicBSpline<type>::GetCurrentValue( const float time ) const
{
	int	  i, j, k;
	float bvals[4], clampedTime;
	type  v;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	Basis( i - 1, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < 4; j++ ) {
		k = i + j - 2;
		v += bvals[j] * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_UniformCubicBSpline<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	  i, j, k;
	float bvals[4], d, clampedTime;
	type  v;

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	BasisFirstDerivative( i - 1, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < 4; j++ ) {
		k = i + j - 2;
		v += bvals[j] * this->ValueForIndex( k );
	}
	d = ( this->TimeForIndex( i ) - this->TimeForIndex( i - 1 ) );
	return v / d;
}

template<class type>
ID_INLINE type idCurve_UniformCubicBSpline<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	  i, j, k;
	float bvals[4], d, clampedTime;
	type  v;

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	BasisSecondDerivative( i - 1, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < 4; j++ ) {
		k = i + j - 2;
		v += bvals[j] * this->ValueForIndex( k );
	}
	d = ( this->TimeForIndex( i ) - this->TimeForIndex( i - 1 ) );
	return v / ( d * d );
}

template<class type>
ID_INLINE void idCurve_UniformCubicBSpline<type>::Basis( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = ( ( ( -s + 3.0f ) * s - 3.0f ) * s + 1.0f ) * ( 1.0f / 6.0f );
	bvals[1] = ( ( ( 3.0f * s - 6.0f ) * s ) * s + 4.0f ) * ( 1.0f / 6.0f );
	bvals[2] = ( ( ( -3.0f * s + 3.0f ) * s + 3.0f ) * s + 1.0f ) * ( 1.0f / 6.0f );
	bvals[3] = ( s * s * s ) * ( 1.0f / 6.0f );
}

template<class type>
ID_INLINE void idCurve_UniformCubicBSpline<type>::BasisFirstDerivative( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = -0.5f * s * s + s - 0.5f;
	bvals[1] = 1.5f * s * s - 2.0f * s;
	bvals[2] = -1.5f * s * s + s + 0.5f;
	bvals[3] = 0.5f * s * s;
}

template<class type>
ID_INLINE void idCurve_UniformCubicBSpline<type>::BasisSecondDerivative( const int index, const float t, float* bvals ) const
{
	float s	 = ( float )( t - this->TimeForIndex( index ) ) / ( this->TimeForIndex( index + 1 ) - this->TimeForIndex( index ) );
	bvals[0] = -s + 1.0f;
	bvals[1] = 3.0f * s - 2.0f;
	bvals[2] = -3.0f * s + 1.0f;
	bvals[3] = s;
}

/*!
	\class idCurve_NonUniformBSpline
	\brief A template class for managing non-uniform B-spline curves with interpolation and derivative calculation capabilities.

	This class implements a non-uniform B-spline curve that can interpolate values and compute derivatives at specified time points. It inherits from a base B-spline class and provides specialized
   methods for basis function calculations, first and second derivatives, and value interpolation. The implementation supports generic types through templating, allowing it to work with various data
   types that represent curve values. The class is designed to be used where smooth interpolation and derivative information are required for curve-based animations or simulations.

*/
template<class type>
class idCurve_NonUniformBSpline : public idCurve_BSpline<type>
{
public:
	//! Constructs an empty non-uniform B-spline curve.
	idCurve_NonUniformBSpline();

	//! Returns the interpolated value for the given time using a non-uniform B-spline curve.
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the spline at the given time.
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the spline at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Calculates the basis functions for a non-uniform B-spline at a given parameter value

		This function computes the basis functions for a non-uniform B-spline curve at a specific parameter value t. It uses the Cox-de Boor recursion formula to evaluate the basis functions. The
	   function takes an index, order, and parameter value, and stores the resulting basis values in the provided array. The index parameter determines the starting point in the knot vector, the order
	   specifies the degree of the B-spline, and t is the parameter value at which to evaluate the basis functions

		\param index starting index in the knot vector
		\param order degree of the B-spline
		\param t parameter value at which to evaluate the basis functions
		\param bvals output array to store the computed basis values
	*/
	void Basis( const int index, const int order, const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of the spline basis functions for a non-uniform B-spline

		This function calculates the first derivative of the basis functions used in non-uniform B-spline interpolation. It takes an index, order, and parameter t to compute the derivative values. The
	   computation uses a recursive approach based on the basis functions of a lower order and applies appropriate scaling factors derived from the time values of the spline knots. The results are
	   stored in the provided array bvals, where each element corresponds to the derivative of a basis function at the given parameter value.

		\param index starting index for the basis function computation
		\param order order of the B-spline basis functions
		\param t parameter value for which to compute the derivative
		\param bvals output array to store the computed derivative values
	*/
	void BasisFirstDerivative( const int index, const int order, const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the spline basis functions for a given index, order, and parameter value

		This function calculates the second derivative of the basis functions used in non-uniform B-spline interpolation. It takes an index, order, and parameter value to compute the second derivative
	   values. The calculation involves first computing the first derivative and then applying additional mathematical operations to derive the second derivative. The result is stored in the provided
	   array bvals where each element corresponds to the second derivative of a basis function at the specified parameter value.

		\param index The starting index for the basis function calculation
		\param order The order of the B-spline basis functions
		\param t The parameter value at which to evaluate the second derivative
		\param bvals Output array to store the computed second derivative values
	*/
	void BasisSecondDerivative( const int index, const int order, const float t, float* bvals ) const;
};

template<class type>
ID_INLINE idCurve_NonUniformBSpline<type>::idCurve_NonUniformBSpline()
{
}

template<class type>
ID_INLINE type idCurve_NonUniformBSpline<type>::GetCurrentValue( const float time ) const
{
	int	   i, j, k;
	float  clampedTime;
	type   v;
	float* bvals = ( float* )_alloca16( this->order * sizeof( float ) );

	if( this->times.Num() == 1 ) { return this->values[0]; }

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	Basis( i - 1, this->order, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < this->order; j++ ) {
		k = i + j - ( this->order >> 1 );
		v += bvals[j] * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_NonUniformBSpline<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	   i, j, k;
	float  clampedTime;
	type   v;
	float* bvals = ( float* )_alloca16( this->order * sizeof( float ) );

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	BasisFirstDerivative( i - 1, this->order, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < this->order; j++ ) {
		k = i + j - ( this->order >> 1 );
		v += bvals[j] * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE type idCurve_NonUniformBSpline<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	   i, j, k;
	float  clampedTime;
	type   v;
	float* bvals = ( float* )_alloca16( this->order * sizeof( float ) );

	if( this->times.Num() == 1 ) {
		return ( this->values[0] - this->values[0] ); //-V501
	}

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	BasisSecondDerivative( i - 1, this->order, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	for( j = 0; j < this->order; j++ ) {
		k = i + j - ( this->order >> 1 );
		v += bvals[j] * this->ValueForIndex( k );
	}
	return v;
}

template<class type>
ID_INLINE void idCurve_NonUniformBSpline<type>::Basis( const int index, const int order, const float t, float* bvals ) const
{
	int	  r, s, i;
	float omega;

	bvals[order - 1] = 1.0f;
	for( r = 2; r <= order; r++ ) {
		i				 = index - r + 1;
		bvals[order - r] = 0.0f;
		for( s = order - r + 1; s < order; s++ ) {
			i++;
			omega = ( float )( t - this->TimeForIndex( i ) ) / ( this->TimeForIndex( i + r - 1 ) - this->TimeForIndex( i ) );
			bvals[s - 1] += ( 1.0f - omega ) * bvals[s];
			bvals[s] *= omega;
		}
	}
}

template<class type>
ID_INLINE void idCurve_NonUniformBSpline<type>::BasisFirstDerivative( const int index, const int order, const float t, float* bvals ) const
{
	int i;

	Basis( index, order - 1, t, bvals + 1 );
	bvals[0] = 0.0f;
	for( i = 0; i < order - 1; i++ ) {
		bvals[i] -= bvals[i + 1];
		bvals[i] *= ( float )( order - 1 ) / ( this->TimeForIndex( index + i + ( order - 1 ) - 2 ) - this->TimeForIndex( index + i - 2 ) );
	}
	bvals[i] *= ( float )( order - 1 ) / ( this->TimeForIndex( index + i + ( order - 1 ) - 2 ) - this->TimeForIndex( index + i - 2 ) );
}

template<class type>
ID_INLINE void idCurve_NonUniformBSpline<type>::BasisSecondDerivative( const int index, const int order, const float t, float* bvals ) const
{
	int i;

	BasisFirstDerivative( index, order - 1, t, bvals + 1 );
	bvals[0] = 0.0f;
	for( i = 0; i < order - 1; i++ ) {
		bvals[i] -= bvals[i + 1];
		bvals[i] *= ( float )( order - 1 ) / ( this->TimeForIndex( index + i + ( order - 1 ) - 2 ) - this->TimeForIndex( index + i - 2 ) );
	}
	bvals[i] *= ( float )( order - 1 ) / ( this->TimeForIndex( index + i + ( order - 1 ) - 2 ) - this->TimeForIndex( index + i - 2 ) );
}

/*!
	\class idCurve_NURBS
	\brief A NURBS curve implementation that extends non-uniform B-splines with weighted control points.

	This class provides a NURBS curve functionality built upon a non-uniform B-spline foundation, enabling precise curve interpolation with weighted control points. It supports adding timed-value
   pairs with optional weights, removing entries, and clearing the entire curve. The class allows for evaluation of the curve's value, first derivative, and second derivative at any given time. The
   curve handles boundary conditions for closed splines when retrieving weights. The implementation is templated to support different data types for curve values, making it flexible for various curve
   applications.

*/
template<class type>
class idCurve_NURBS : public idCurve_NonUniformBSpline<type>
{
public:
	//! Constructs an empty NURBS curve.
	idCurve_NURBS();

	//! Adds a timed/value pair to the NURBS spline and returns the index of the inserted pair.
	virtual int	 AddValue( const float time, const type& value );

	//! Adds a timed/value pair to the NURBS spline and returns the index of the inserted pair.
	virtual int	 AddValue( const float time, const type& value, const float weight );

	//! Removes the element at the specified index from the NURBS curve data structures
	virtual void RemoveIndex( const int index )
	{
		this->values.RemoveIndex( index );
		this->times.RemoveIndex( index );
		weights.RemoveIndex( index );
	}

	//! Clears all curve data including values, times, weights, and resets the current index.
	virtual void Clear()
	{
		this->values.Clear();
		this->times.Clear();
		weights.Clear();
		this->currentIndex = -1;
	}

	//! Returns the interpolated value at the specified time using NURBS curve evaluation.
	virtual type GetCurrentValue( const float time ) const;

	//! Returns the first derivative of the NURBS curve at the specified time
	virtual type GetCurrentFirstDerivative( const float time ) const;

	//! Returns the second derivative of the NURBS curve at the specified time.
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	idList<float> weights;

	//! Returns the weight for the given index, handling boundary conditions for closed splines.
	float		  WeightForIndex( const int index ) const;
};

template<class type>
ID_INLINE idCurve_NURBS<type>::idCurve_NURBS()
{
}

template<class type>
ID_INLINE int idCurve_NURBS<type>::AddValue( const float time, const type& value )
{
	int i;

	i = this->IndexForTime( time );
	this->times.Insert( time, i );
	this->values.Insert( value, i );
	weights.Insert( 1.0f, i );
	return i;
}

/*
====================
idCurve_NURBS::AddValue

  add a timed/value pair to the spline
  returns the index to the inserted pair
====================
*/
template<class type>
ID_INLINE int idCurve_NURBS<type>::AddValue( const float time, const type& value, const float weight )
{
	int i;

	i = this->IndexForTime( time );
	this->times.Insert( time, i );
	this->values.Insert( value, i );
	weights.Insert( weight, i );
	return i;
}

template<class type>
ID_INLINE type idCurve_NURBS<type>::GetCurrentValue( const float time ) const
{
	int	  i, j, k;
	float w, b, *bvals, clampedTime;
	type  v;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	bvals = ( float* )_alloca16( this->order * sizeof( float ) );

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	this->Basis( i - 1, this->order, clampedTime, bvals );
	v = this->values[0] - this->values[0]; //-V501
	w = 0.0f;
	for( j = 0; j < this->order; j++ ) {
		k = i + j - ( this->order >> 1 );
		b = bvals[j] * WeightForIndex( k );
		w += b;
		v += b * this->ValueForIndex( k );
	}
	return v / w;
}

template<class type>
ID_INLINE type idCurve_NURBS<type>::GetCurrentFirstDerivative( const float time ) const
{
	int	  i, j, k;
	float w, wb, wd1, b, d1, *bvals, *d1vals, clampedTime;
	type  v, vb, vd1;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	bvals  = ( float* )_alloca16( this->order * sizeof( float ) );
	d1vals = ( float* )_alloca16( this->order * sizeof( float ) );

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	this->Basis( i - 1, this->order, clampedTime, bvals );
	this->BasisFirstDerivative( i - 1, this->order, clampedTime, d1vals );
	vb = vd1 = this->values[0] - this->values[0]; //-V501
	wb = wd1 = 0.0f;
	for( j = 0; j < this->order; j++ ) {
		k  = i + j - ( this->order >> 1 );
		w  = WeightForIndex( k );
		b  = bvals[j] * w;
		d1 = d1vals[j] * w;
		wb += b;
		wd1 += d1;
		v = this->ValueForIndex( k );
		vb += b * v;
		vd1 += d1 * v;
	}
	return ( wb * vd1 - vb * wd1 ) / ( wb * wb );
}

template<class type>
ID_INLINE type idCurve_NURBS<type>::GetCurrentSecondDerivative( const float time ) const
{
	int	  i, j, k;
	float w, wb, wd1, wd2, b, d1, d2, *bvals, *d1vals, *d2vals, clampedTime;
	type  v, vb, vd1, vd2;

	if( this->times.Num() == 1 ) { return this->values[0]; }

	bvals  = ( float* )_alloca16( this->order * sizeof( float ) );
	d1vals = ( float* )_alloca16( this->order * sizeof( float ) );
	d2vals = ( float* )_alloca16( this->order * sizeof( float ) );

	clampedTime = this->ClampedTime( time );
	i			= this->IndexForTime( clampedTime );
	this->Basis( i - 1, this->order, clampedTime, bvals );
	this->BasisFirstDerivative( i - 1, this->order, clampedTime, d1vals );
	this->BasisSecondDerivative( i - 1, this->order, clampedTime, d2vals );
	vb = vd1 = vd2 = this->values[0] - this->values[0]; //-V501
	wb = wd1 = wd2 = 0.0f;
	for( j = 0; j < this->order; j++ ) {
		k  = i + j - ( this->order >> 1 );
		w  = WeightForIndex( k );
		b  = bvals[j] * w;
		d1 = d1vals[j] * w;
		d2 = d2vals[j] * w;
		wb += b;
		wd1 += d1;
		wd2 += d2;
		v = this->ValueForIndex( k );
		vb += b * v;
		vd1 += d1 * v;
		vd2 += d2 * v;
	}
	return ( ( wb * wb ) * ( wb * vd2 - vb * wd2 ) - ( wb * vd1 - vb * wd1 ) * 2.0f * wb * wd1 ) / ( wb * wb * wb * wb );
}

template<class type>
ID_INLINE float idCurve_NURBS<type>::WeightForIndex( const int index ) const
{
	int n = weights.Num() - 1;

	if( index < 0 ) {
		if( this->boundaryType == idCurve_Spline<type>::BT_CLOSED ) {
			return weights[weights.Num() + index % weights.Num()];
		} else {
			return weights[0] + index * ( weights[1] - weights[0] );
		}
	} else if( index > n ) {
		if( this->boundaryType == idCurve_Spline<type>::BT_CLOSED ) {
			return weights[index % weights.Num()];
		} else {
			return weights[n] + ( index - n ) * ( weights[n] - weights[n - 1] );
		}
	}
	return weights[index];
}

#endif /* !__MATH_CURVE_H__ */
