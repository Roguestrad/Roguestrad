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

/*
===============================================================================

	Curve base template.

===============================================================================
*/

template<class type>
class idCurve
{
public:
	/*!
		\brief Initializes a new instance of the idCurve template class.

		This constructor initializes the internal state of the idCurve template class. It sets the current index to -1 and the changed flag to false, indicating that no curve data has been processed
	   or modified yet.

	*/
	idCurve();
	virtual ~idCurve();

	/*!
		\brief Adds a new value to the curve at the specified time and returns its index.

		This function inserts a new time-value pair into the curve at the correct position based on the time value. It determines the appropriate index for insertion using the IndexForTime method,
	   then inserts both the time and value at that index in their respective arrays. The function marks the curve as changed to indicate that modifications have been made.

		\param time The time at which to add the value
		\param value The value to add to the curve
		\return The index where the new value was inserted in the curve
	*/
	virtual int	 AddValue( const float time, const type& value );

	/*!
		\brief Removes the curve point at the specified index from the curve.

		This function removes a curve point from the curve by removing the value and time at the specified index. It also marks the curve as changed to indicate that the curve has been modified.

		\param index The index of the curve point to remove
	*/
	virtual void RemoveIndex( const int index )
	{
		values.RemoveIndex( index );
		times.RemoveIndex( index );
		changed = true;
	}

	//! Clears all values and times from the curve and resets the current index and changed flag.
	virtual void Clear()
	{
		values.Clear();
		times.Clear();
		currentIndex = -1;
		changed		 = true;
	}

	/*!
		\brief Returns the current value from the curve at the specified time

		This function retrieves the value from the curve at a given time by first determining the appropriate index using the IndexForTime method. If the calculated index exceeds the number of values
	   in the curve, it returns the last value in the curve. Otherwise, it returns the value at the calculated index.

		\param time The time value for which to retrieve the curve value
		\return The value from the curve at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the curve at the specified time

		This function computes and returns the first derivative of the curve at the given time parameter. Based on the implementation, it appears to be returning the difference between the first two
	   values in the curve's value array. The current implementation seems to have a potential bug as it subtracts a value from itself, which would always return zero. This needs to be reviewed and
	   corrected to properly calculate the derivative.

		\param time The time at which to calculate the first derivative
		\return The first derivative of the curve at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the curve at the specified time.

		The function calculates and returns the second derivative of the curve at the given time. It performs a check to ensure the curve is complete at the specified time before computing the
	   derivative. The implementation currently appears to return zero due to a potential bug in the calculation.

		\param time The time at which to calculate the second derivative
		\return The second derivative of the curve at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

	/*!
		\brief Checks if the curve has completed by the specified time.

		This function determines whether the curve has finished its animation or interpolation by comparing the given time against the end time of the curve. It returns true if the specified time is
	   greater than or equal to the final time in the curve's time array, indicating the curve is done. Otherwise, it returns false, meaning the curve is still in progress.

		\param time The time value to check against the curve's end time
		\return True if the curve is complete at the specified time, false otherwise
	*/
	virtual bool IsDone( const float time ) const;

	//! Returns the number of values in the curve.
	int			 GetNumValues() const { return values.Num(); }

	/*!
		\brief Sets the value at the specified index in the curve.

		This function updates the value at the given index in the curve's data structure. It marks the curve as changed to indicate that modifications have been made.

		\param index The index in the curve where the value should be set
		\param value The new value to assign at the specified index
	*/
	void		 SetValue( const int index, const type& value )
	{
		values[index] = value;
		changed		  = true;
	}

	/*!
		\brief Retrieves the value at the specified index from the curve data

		This method accesses the internal array of curve values and returns the element at the given index. The function is marked as const, indicating it does not modify the object state. It directly
	   returns the value stored at the specified index without any bounds checking, so the index must be valid to avoid undefined behavior. The curve data is expected to be populated prior to calling
	   this method.

		\param index The zero-based index of the value to retrieve from the curve data
		\return The value at the specified index in the curve data
	*/
	type  GetValue( const int index ) const { return values[index]; }

	/*!
		\brief Returns a pointer to the value at the specified index in the curve

		The function retrieves a pointer to the value stored at the given index within the curve data structure. It directly accesses the internal array of values using the provided index and returns
	   a pointer to that specific element

		\param index The index of the value to retrieve a pointer to
		\return A pointer to the value at the specified index in the curve
	*/
	type* GetValueAddress( const int index ) { return &values[index]; }

	/*!
		\brief Retrieves the time value at the specified index from the curve.

		This function accesses the time array of the curve at the given index and returns the corresponding time value. It is a simple accessor method that provides read-only access to the time data
	   stored within the curve.

		\param index The index of the time value to retrieve from the curve.
		\return The time value at the specified index in the curve's time array.
	*/
	float GetTime( const int index ) const { return times[index]; }

	/*!
		\brief Calculates the arc length of the curve up to a given time value.

		This function computes the total arc length of the curve from the start up to the specified time value. It uses the Romberg integration method to calculate the length between time intervals.
	   The calculation is performed by summing the lengths of segments from the beginning of the curve up to the given time, where the final segment is partially calculated up to the specified time.

		\param time The time value up to which the arc length is calculated.
		\return The arc length of the curve from the start up to the specified time value.
	*/
	float GetLengthForTime( const float time ) const;

	/*!
		\brief Returns the time value corresponding to a given arc length along the curve by using Newton's method to invert the arc length integral.

		This function calculates the time along a curve that corresponds to a specific arc length. It first determines which segment of the curve contains the desired length by accumulating segment
	   lengths. Then it uses Newton's method to solve for the time by inverting the arc length integral. The process involves Romberg integration for numerical accuracy and includes a convergence
	   check based on the provided epsilon value. If the length is less than or equal to zero, the function returns the time at the start of the curve. If the length exceeds the total curve length, it
	   returns the time at the end of the curve.

		\param length The arc length along the curve for which to find the corresponding time value
		\param epsilon The tolerance for convergence of Newton's method when inverting the arc length integral
		\return The time value along the curve corresponding to the specified arc length
	*/
	float GetTimeForLength( const float length, const float epsilon = 0.1f ) const;

	/*!
		\brief Calculates the arc length between two knots in a curve using Romberg integration.

		This function computes the total arc length between two specified knots in a curve by numerically integrating the curve's derivative using Romberg integration. The integration is performed
	   over each sub-interval between consecutive knots from i0 to i1. The curve times are assumed to be evenly spaced over the given total time.

		\param i0 Start knot index for arc length calculation
		\param i1 End knot index for arc length calculation
		\return The total arc length between the specified knots
	*/
	float GetLengthBetweenKnots( const int i0, const int i1 ) const;

	/*!
		\brief Sets the curve times to be evenly spaced over the given total time.

		Adjusts the time knots of the curve so that the curve has constant speed over the specified total time. The curve times are distributed evenly across the provided total time interval.

		\param totalTime The total time over which the curve times should be evenly distributed
	*/
	void  MakeUniform( const float totalTime );

	/*!
		\brief Adjusts the time knots of the curve to ensure constant speed over the specified total time

		This function modifies the time values associated with the curve knots to make the curve travel at a constant speed over the given total time. It calculates the length between consecutive
	   knots and scales the time intervals accordingly. The function updates the internal time array and marks the curve as changed to reflect the modifications.

		\param totalTime The total time over which the curve should be traversed at constant speed
	*/
	void  SetConstantSpeed( const float totalTime );

	/*!
		\brief Shifts all time values in the curve by the specified delta time.

		This function adjusts all time values stored in the curve by adding the specified delta time value. It is typically used to translate the entire curve along the time axis. The function updates
	   the internal time array and marks the curve as changed to indicate that its state has been modified.

		\param deltaTime The amount of time to shift all time values in the curve
	*/
	void  ShiftTime( const float deltaTime );

	/*!
		\brief Translates all curve values by the specified translation amount.

		This function modifies all values in the curve by adding the provided translation amount to each value. It marks the curve as changed to indicate that the values have been modified.

		\param translation The value to be added to each curve value
	*/
	void  Translate( const type& translation );

protected:
	idList<float> times;  // knots
	idList<type>  values; // knot values

	mutable int	  currentIndex; // cached index for fast lookup
	mutable bool  changed;		// set whenever the curve changes

	/*!
		\brief Returns the curve index for the given time value, using a cached index when possible and falling back to binary search.

		This function efficiently determines the appropriate curve index for a specified time value by first checking if a previously computed index can be reused. It uses a cached index when valid,
	   otherwise performs a binary search to locate the correct index in the time array. The function updates the cached index for subsequent calls to improve performance.

		\param time The time value for which to find the corresponding curve index
		\return The curve index that corresponds to the given time value
	*/
	int			  IndexForTime( const float time ) const;

	/*!
		\brief Returns the time value for a given index in the curve, with linear extrapolation for out-of-bounds indices.

		This function retrieves the time value associated with a specified index in the curve. If the index is within the valid range of the curve's time array, it returns the exact time value at that
	   index. For indices before the start of the curve, it performs linear extrapolation using the first two time values. For indices beyond the end of the curve, it performs linear extrapolation
	   using the last two time values.

		\param index The index for which to retrieve the time value
		\return The time value corresponding to the specified index, with extrapolation applied for out-of-bounds indices
	*/
	float		  TimeForIndex( const int index ) const;

	/*!
		\brief Returns the curve value at the specified index with linear extrapolation for out-of-bounds indices

		This function retrieves the curve value at the given index. If the index is less than zero, it performs linear extrapolation using the first two values. If the index exceeds the valid range,
	   it performs linear extrapolation using the last two values. Otherwise, it directly returns the value at the specified index

		\param index The index of the curve value to retrieve
		\return The curve value at the specified index, with linear extrapolation for out-of-bounds indices
	*/
	type		  ValueForIndex( const int index ) const;

	/*!
		\brief Returns the speed of the curve at the given time by calculating the magnitude of the first derivative.

		The function computes the speed by first calculating the first derivative of the curve at the specified time. It then calculates the magnitude of this derivative by summing the squares of each
	   component and taking the square root of the result. This approach effectively determines the instantaneous speed of the curve at the provided time value.

		\param time The time value at which to calculate the speed of the curve
		\return The speed of the curve at the specified time, calculated as the magnitude of the first derivative
	*/
	float		  GetSpeed( const float time ) const;

	/*!
		\brief Computes the Romberg integral of the curve speed function over the specified time interval using Richardson extrapolation

		This function calculates the integral of the curve's speed function between two time points using the Romberg integration method. It employs Richardson extrapolation to improve the accuracy of
	   the numerical integration. The algorithm starts with the trapezoid rule and then applies successive extrapolations to refine the estimate. The integration is performed over the interval [t0,
	   t1] using the specified order of extrapolation.

		\param t0 Start time of the integration interval
		\param t1 End time of the integration interval
		\param order Order of the Romberg integration, determines the number of extrapolation steps
		\return The computed integral value representing the arc length or accumulated speed over the time interval
	*/
	float		  RombergIntegral( const float t0, const float t1, const int order ) const;
};

template<class type>
ID_INLINE idCurve<type>::idCurve()
{
	currentIndex = -1;
	changed		 = false;
}

/*
====================
idCurve::~idCurve
====================
*/
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

/*
===============================================================================

	Bezier Curve template.
	The degree of the polynomial equals the number of knots minus one.

===============================================================================
*/

template<class type>
class idCurve_Bezier : public idCurve<type>
{
public:
	/*!
		\brief Constructs a new Bezier curve with default control points.

		This constructor initializes a new Bezier curve object with default control points. It is intended to create a curve that can be used for interpolation or animation purposes.

	*/
	idCurve_Bezier();

	/*!
		\brief Returns the interpolated value from the Bezier curve at the specified time

		This function computes the interpolated value along a Bezier curve at a given time parameter. It uses the Bernstein basis polynomials to calculate the contribution of each control point to the
	   final curve value. The calculation involves precomputing the basis values and then performing a weighted sum of the control points.

		\param time The time parameter for which to compute the curve value, typically normalized between 0 and 1
		\return The interpolated value on the Bezier curve at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the Bezier curve at the specified time

		This function calculates the first derivative of a Bezier curve at a specified time value. It uses the basis functions for the first derivative to compute the result. The calculation takes
	   into account the number of control points and the time range of the curve. The result is scaled by a factor that depends on the number of control points and the time span of the curve.

		\param time The time value at which to calculate the first derivative
		\return The first derivative of the Bezier curve at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the Bezier curve at the given time

		This function calculates the second derivative of a Bezier curve at a specified time value. It uses the basis functions for the second derivative to compute the result. The calculation takes
	   into account the number of control points and the time range of the curve. The result is scaled by a factor that depends on the number of control points and the time span of the curve.

		\param time The time value at which to calculate the second derivative
		\return The second derivative of the Bezier curve at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Computes the basis values for a Bezier curve of the specified order at the given parameter value

		This function calculates the basis functions for a Bezier curve of a given order at a specific parameter value t. It uses a recursive approach to compute the binomial coefficients and
	   evaluates the basis polynomials. The computation involves normalizing the parameter t based on the curve's time range and then applying the recursive formula for binomial coefficients to
	   determine the basis values

		\param order The order of the Bezier curve
		\param t The parameter value at which to compute the basis functions
		\param bvals Output array to store the computed basis values
	*/
	void Basis( const int order, const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of the basis functions for a Bézier curve of the specified order at parameter t

		This function calculates the first derivative of the basis functions used in Bézier curve evaluation. It takes the order of the Bézier curve, a parameter t, and computes the derivatives of the
	   basis functions at that point. The implementation uses a recursive approach by leveraging the Basis function for one order lower, then adjusts the values to compute the derivative.

		\param order The order of the Bézier curve
		\param t The parameter at which to evaluate the derivative
		\param bvals Output array to store the derivative values of the basis functions
	*/
	void BasisFirstDerivative( const int order, const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the basis functions for a Bézier curve of the specified order at parameter t

		This function calculates the second derivative values for the basis functions of a Bézier curve. It first computes the first derivative values for a curve of one lower order, then adjusts
	   these values to obtain the second derivative results. The computation is performed in-place on the provided array of basis values, with the first element set to zero

		\param order The order of the Bézier curve
		\param t The parameter value at which to compute the second derivative
		\param bvals Pointer to an array where the second derivative basis function values will be stored
	*/
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

/*
===============================================================================

	Quadratic Bezier Curve template.
	Should always have exactly three knots.

===============================================================================
*/

template<class type>
class idCurve_QuadraticBezier : public idCurve<type>
{
public:
	/*!
		\brief Constructs a new quadratic Bézier curve with default control points.

		This constructor initializes a quadratic Bézier curve with default control point values. It sets up the internal state of the curve object without requiring any explicit parameters. The curve
	   is defined by three control points: the start point, a control point, and an end point, all initialized to default values.

	*/
	idCurve_QuadraticBezier();

	/*!
		\brief Returns the current value of the quadratic Bézier curve at the specified time

		This function calculates and returns the value of a quadratic Bézier curve at a given time parameter. It uses the basis functions to compute the weighted sum of three control points. The
	   function assumes that exactly three control points have been set in the curve

		\param time The time parameter at which to evaluate the curve, typically between 0 and 1
		\return The interpolated value of the quadratic Bézier curve at the specified time
		\throws assertion failure if the number of control points is not exactly 3
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the quadratic Bézier curve at the given time

		This function computes the first derivative of a quadratic Bézier curve at a specified time value. It uses the basis first derivative coefficients and the control point values to calculate the
	   result. The computation accounts for the time interval between control points by normalizing the result with the time difference

		\param time The time value at which to compute the first derivative, typically in the range [0, 1]
		\return The first derivative value of the quadratic Bézier curve at the specified time
		\throws assertion failure if the number of values is not equal to 3
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the quadratic Bézier curve at the specified time.

		This function computes the second derivative of a quadratic Bézier curve at a given time value. It uses the basis second derivative coefficients and the control point values to calculate the
	   result. The computation accounts for the time interval between control points by normalizing the result with the square of the time difference.

		\param time The time value at which to compute the second derivative, typically in the range [0, 1]
		\return The second derivative value of the quadratic Bézier curve at the specified time
		\throws assertion failure if the number of values is not equal to 3
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Computes the quadratic Bézier basis values for the given parameter t and stores them in the provided array.

		This function calculates the basis values for a quadratic Bézier curve at a specific parameter value t. The basis values are computed using the standard quadratic Bézier basis functions. The
	   parameter t is normalized using the curve's time range, and the resulting values are stored in the bvals array. The function assumes that the bvals array has at least three elements to store
	   the computed basis values.

		\param t The parameter value for which to compute the basis values, typically in the range [0, 1]
		\param bvals Pointer to an array where the computed basis values will be stored
	*/
	void Basis( const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of the quadratic Bézier curve basis functions at the given parameter value

		This function calculates the first derivative of the basis functions for a quadratic Bézier curve at a specified parameter value t. The computation uses the parameter t normalized to the
	   interval [0, 1] based on the curve's time range. The derivative values are stored in the bvals array.

		\param t The parameter value at which to compute the first derivative, typically in the range [0, 1]
		\param bvals Pointer to an array where the computed derivative values will be stored
	*/
	void BasisFirstDerivative( const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the quadratic Bézier basis functions at the given parameter value.

		This function calculates the second derivatives of the basis functions for a quadratic Bézier curve at a specified parameter value t. The second derivatives are constant values for quadratic
	   Bézier curves and do not depend on the parameter t. The results are stored in the provided array bvals, which must have at least three elements.

		\param t The parameter value at which to compute the second derivatives, typically in the range [0, 1]
		\param bvals Pointer to an array where the second derivative values will be stored
	*/
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

/*
===============================================================================

	Cubic Bezier Curve template.
	Should always have exactly four knots.

===============================================================================
*/

template<class type>
class idCurve_CubicBezier : public idCurve<type>
{
public:
	/*!
		\brief Initializes a new instance of the cubic Bézier curve class.

		This constructor initializes a new instance of the cubic Bézier curve class. It performs no additional operations beyond default initialization.

	*/
	idCurve_CubicBezier();

	/*!
		\brief Returns the interpolated value at the given time for a cubic Bézier curve

		This function calculates and returns the interpolated value along a cubic Bézier curve at the specified time parameter. It uses the basis functions to compute the contribution of each control
	   point and sums their weighted contributions to produce the final interpolated value. The curve is defined by four control points stored in the class instance.

		\param time The time parameter at which to evaluate the Bézier curve, typically in the range [0, 1]
		\return The interpolated value at the given time along the cubic Bézier curve
		\throws assertion failure if the number of control points is not exactly 4
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the cubic Bézier curve at the specified time

		This function calculates and returns the first derivative of a cubic Bézier curve at a given time value. The calculation uses the basis functions for the first derivative and divides the
	   result by the time difference between the last and first control points to normalize the derivative. The curve is defined by four control points stored in the values array, and the time values
	   are stored in the times array. The function asserts that there are exactly four values in the curve.

		\param time The time value at which to calculate the first derivative
		\return The first derivative of the cubic Bézier curve at the specified time
		\throws assertion failure if the number of values in the curve is not exactly 4
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the cubic Bezier curve at the specified time.

		This function calculates the second derivative of a cubic Bezier curve at a given time parameter. It uses the basis functions for the second derivative and applies them to the control points
	   of the curve. The result is scaled by the square of the time range to account for the curve's parameterization.

		\param time The time parameter at which to evaluate the second derivative, typically in the range [0, 1]
		\return The second derivative value of the cubic Bezier curve at the specified time
		\throws assertion failure if the number of control points is not exactly 4
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Computes the cubic Bézier basis functions for a given parameter value and stores the results in the provided array

		This function evaluates the four cubic Bézier basis functions at the specified parameter value t. The basis functions are computed using the standard cubic Bézier polynomial expressions. The
	   results are stored in the bvals array, where each element corresponds to one of the four basis functions. The parameter t is normalized based on the time range defined by the curve's control
	   points, and the computation uses the standard cubic Bézier blending functions to calculate the weights for the interpolation

		\param t The parameter value at which to evaluate the basis functions, typically normalized to the curve's time range
		\param bvals Pointer to an array where the four basis function values will be stored
	*/
	void Basis( const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivatives of the cubic Bézier basis functions at the given parameter value

		This function calculates the first derivatives of the four cubic Bézier basis functions for a given parameter t. The computation is based on the normalized parameter s1 which represents the
	   position along the curve relative to the time interval defined by the curve's control points. The results are stored in the bvals array where each element corresponds to the derivative of a
	   specific basis function

		\param t The parameter value at which to compute the derivatives
		\param bvals Output array to store the computed first derivative values
	*/
	void BasisFirstDerivative( const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the cubic Bézier basis functions at the given parameter value

		This function calculates the second derivative values for the four basis functions of a cubic Bézier curve at the specified parameter value t. The computation uses a normalized parameter s1
	   derived from the time values of the curve. The results are stored in the provided array bvals, which should have at least four elements. Each element corresponds to the second derivative of one
	   of the four Bézier basis functions

		\param t The parameter value at which to compute the second derivatives
		\param bvals Pointer to an array where the results will be stored
	*/
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

/*
===============================================================================

	Spline base template.

===============================================================================
*/

template<class type>
class idCurve_Spline : public idCurve<type>
{
public:
	enum boundary_t { BT_FREE, BT_CLAMPED, BT_CLOSED };

	/*!
		\brief Initializes a new instance of the idCurve_Spline class with default boundary type and close time.

		The constructor sets the boundary type to BT_FREE and the close time to 0.0f. This initializes the spline with default values suitable for general use.

	*/
	idCurve_Spline();

	/*!
		\brief Determines if the spline interpolation is complete at the specified time.

		This function checks whether the spline interpolation has reached its end based on the boundary type and the given time. If the boundary type is not BT_CLOSED and the specified time is greater
	   than or equal to the last time value in the spline, the function returns true, indicating the interpolation is complete. Otherwise, it returns false.

		\param time The time value to check against the spline's time range
		\return True if the spline interpolation is complete at the given time, false otherwise
	*/
	virtual bool IsDone( const float time ) const;

	/*!
		\brief Sets the boundary type for the spline curve and marks it as changed.

		This function updates the boundary type of the spline curve to the specified value and flags the curve as having changed. This is typically used to configure how the spline handles its
	   endpoints or boundaries.

		\param bt The boundary type to set for the spline curve
	*/
	virtual void SetBoundaryType( const boundary_t bt )
	{
		boundaryType  = bt;
		this->changed = true;
	}

	//! Returns the boundary type of the spline curve.
	virtual boundary_t GetBoundaryType() const { return boundaryType; }

	/*!
		\brief Sets the close time value for the spline curve and marks it as changed.

		This function updates the close time parameter of the spline curve to the specified value and signals that the curve has been modified. The close time is typically used to control how the
	   spline handles closing or connecting points in a curve.

		\param t The new close time value to set for the spline curve
	*/
	virtual void	   SetCloseTime( const float t )
	{
		closeTime	  = t;
		this->changed = true;
	}

	/*!
		\brief Returns the closing time for a spline curve when it is in closed boundary type, otherwise returns zero.

		This function checks the boundary type of the spline curve and returns the closeTime value if the boundary type is BT_CLOSED, otherwise it returns zero. This is used to determine the time at
	   which the spline curve closes when it is in a closed state.

		\return The closing time of the spline curve if the boundary type is BT_CLOSED, otherwise zero
	*/
	virtual float GetCloseTime() { return boundaryType == BT_CLOSED ? closeTime : 0.0f; }

protected:
	boundary_t boundaryType;
	float	   closeTime;

	/*!
		\brief Returns the value at the specified index, handling boundary conditions for closed and open curves.

		This function retrieves the value at a given index from the spline curve, taking into account the boundary type of the curve. For indices outside the valid range [0, n], where n is the number
	   of values, the function applies special handling based on the boundary type. In closed curves, it uses modular arithmetic to wrap around the indices, while in open curves, it extends the curve
	   linearly based on the first or last segments.

		\param index The index for which the value is to be computed
		\return The value at the specified index, adjusted for boundary conditions
	*/
	type	   ValueForIndex( const int index ) const;

	/*!
		\brief Returns the time value for a given index, handling boundary conditions for closed and open curves.

		This function computes the time value corresponding to a given index in the spline curve. It takes into account the boundary type of the curve, which can be either closed or open. For indices
	   outside the valid range [0, n], where n is the number of time points, the function applies special handling based on the boundary type. In closed curves, it uses modular arithmetic to wrap
	   around the indices, while in open curves, it extends the curve linearly based on the first or last time segments.

		\param index The index for which the time value is to be computed
		\return The time value corresponding to the given index, adjusted for boundary conditions
	*/
	float	   TimeForIndex( const int index ) const;

	/*!
		\brief Returns the clamped time value based on the boundary type and input time.

		This function checks if the boundary type is set to clamped and if the input time is outside the valid range defined by the spline's time array. If the input time is less than the first time
	   value, it returns the first time value. If the input time is greater than or equal to the last time value, it returns the last time value. Otherwise, it returns the input time unchanged.

		\param t The input time value to be clamped
		\return The clamped time value if the boundary type is BT_CLAMPED and the input time is outside the valid range, otherwise returns the input time unchanged
	*/
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

/*
===============================================================================

	Cubic Interpolating Spline template.
	The curve goes through all the knots.

===============================================================================
*/

template<class type>
class idCurve_NaturalCubicSpline : public idCurve_Spline<type>
{
public:
	/*!
		\brief Constructs a new instance of the natural cubic spline curve and clears all stored data.

		This constructor initializes a new instance of the natural cubic spline curve class. It performs no additional operations beyond the default initialization and clears any existing data that
	   might be present in the curve.

	*/
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

	/*!
		\brief Returns the interpolated value at the given time from a natural cubic spline curve.

		This function evaluates a natural cubic spline curve at the specified time parameter. It first clamps the input time to the valid range, determines the appropriate segment index, and then
	   computes the interpolated value using precomputed spline coefficients. The spline is defined by the curve's control points and their associated tangent vectors, which are computed during the
	   setup phase.

		\param time The time value at which to evaluate the spline curve
		\return The interpolated value at the given time from the natural cubic spline curve
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the natural cubic spline at the given time

		This function computes the first derivative of a natural cubic spline at a specified time value. It first clamps the input time to the valid range of the spline, determines the appropriate
	   segment index, and calculates the relative time within that segment. The function then uses precomputed coefficients to evaluate the derivative using the formula b[i] + s * (2.0f * c[i] + 3.0f
	   * s * d[i]), where s is the relative time within the segment

		\param time the time value at which to compute the first derivative
		\return the first derivative of the spline at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the natural cubic spline at the given time.

		This function calculates and returns the second derivative of the natural cubic spline curve at a specified time value. The calculation uses the precomputed coefficients stored in the curve
	   object and performs interpolation based on the time parameter. The input time is first clamped to the valid range of the curve before processing. The result represents the acceleration or
	   curvature of the spline at the given point in time.

		\param time The time value at which to calculate the second derivative, will be clamped to valid range
		\return The second derivative value of the spline at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	mutable idList<type> b;
	mutable idList<type> c;
	mutable idList<type> d;

	/*!
		\brief Sets up the spline curve boundary conditions based on the current boundary type.

		This function initializes the spline curve by configuring the boundary conditions according to the specified boundary type. It checks if the curve has been modified and, if so, calls the
	   appropriate setup function for the current boundary type. The boundary types supported are free, clamped, and closed, each requiring different boundary condition calculations. After setting up
	   the boundary conditions, it marks the curve as unchanged.

	*/
	void				 Setup() const;

	/*!
		\brief Sets up the free boundary conditions for the natural cubic spline curve interpolation.

		This function initializes the coefficients required for natural cubic spline interpolation with free boundary conditions. It computes the necessary intermediate values and stores them in
	   internal arrays for later use in spline evaluation. The function uses a tridiagonal system solver to determine the spline coefficients based on the control points and their time values. The
	   boundary conditions are set to natural (second derivative is zero at the endpoints).

	*/
	void				 SetupFree() const;

	/*!
		\brief Configures the spline coefficients for clamped natural cubic spline interpolation

		This function sets up the necessary coefficients for performing clamped natural cubic spline interpolation on a curve. It calculates the beta, gamma, and delta values using a tridiagonal
	   system solver, then computes the b and d coefficients needed for spline evaluation. The function allocates temporary memory for intermediate calculations and ensures that the resulting
	   coefficient arrays are properly sized for the number of curve values.

	*/
	void				 SetupClamped() const;

	/*!
		\brief Sets up the coefficients for a closed natural cubic spline interpolation.

		This function prepares the necessary coefficients for performing closed natural cubic spline interpolation on a set of values. It constructs a tridiagonal matrix system based on the time
	   values and solves for the spline coefficients. The function handles the boundary conditions for a closed spline, where the first and last points are connected seamlessly. Memory is allocated
	   using alloca16 for temporary storage during matrix computation. The solution involves solving a linear system of equations for each dimension of the value vectors. The resulting coefficients
	   are stored in member variables b, c, and d for later use in spline evaluation.

	*/
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

/*
===============================================================================

	Uniform Cubic Interpolating Spline template.
	The curve goes through all the knots.

===============================================================================
*/

template<class type>
class idCurve_CatmullRomSpline : public idCurve_Spline<type>
{
public:
	/*!
		\brief Constructs an empty Catmull-Rom spline curve.

		This constructor initializes an empty Catmull-Rom spline curve. The curve is typically used for interpolation between control points with smooth transitions. The spline is defined by a series
	   of control points and uses the Catmull-Rom algorithm to calculate intermediate values.

	*/
	idCurve_CatmullRomSpline();

	/*!
		\brief Returns the current value of the Catmull-Rom spline at the specified time.

		This function evaluates the Catmull-Rom spline at the given time parameter and returns the interpolated value. It handles edge cases where there is only one control point by returning that
	   point's value directly. The function uses a basis function to compute the influence of neighboring control points on the current position, making it suitable for smooth curve interpolation in
	   animation and other applications.

		\param time The time value at which to evaluate the spline
		\return The interpolated value of the spline at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the curve at the given time

		This function calculates the first derivative of a Catmull-Rom spline at the specified time parameter. It uses basis functions to compute the derivative by evaluating the spline's control
	   points and their influence at the given time. The derivative represents the rate of change of the spline at that point, which is useful for calculating velocity or tangent vectors along the
	   curve.

		\param time The time parameter at which to evaluate the first derivative
		\return The first derivative of the curve at the specified time, representing the rate of change of the spline
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the Catmull-Rom spline at the given time.

		This function calculates the second derivative of the Catmull-Rom spline at the specified time. It first clamps the time to the valid range of the curve, then determines the appropriate
	   segment index. It uses basis functions to compute the second derivative contribution from the relevant control points. The result is normalized by the square of the time difference between
	   adjacent control points to account for non-uniform spacing.

		\param time The time at which to evaluate the second derivative
		\return The second derivative value of the spline at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Computes the Catmull-Rom spline basis functions for the given index and time parameter

		This function calculates the four Catmull-Rom spline basis functions used for interpolation. The basis functions are computed based on the normalized time parameter s, which represents the
	   position along the curve segment between two control points. The function uses the standard Catmull-Rom basis function equations to determine the weights for the four control points that
	   influence the curve at the specified time.

		\param index The index of the current segment in the spline
		\param t The time parameter for which to compute the basis functions
		\param bvals Output array to store the four computed basis function values
	*/
	void Basis( const int index, const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of Catmull-Rom spline basis functions for a given index and time parameter

		This function calculates the first derivative of the Catmull-Rom spline basis functions at a specified index and time value. The computation uses the standard Catmull-Rom spline derivative
	   formulas, where the time parameter is normalized within the interval defined by the current and next time indices. The results are stored in the provided array of four floating-point values
	   representing the derivatives of the four basis functions.

		\param index The index of the current knot in the spline
		\param t The time parameter for which to compute the derivative
		\param bvals Pointer to an array of four floats that will store the computed derivative values
	*/
	void BasisFirstDerivative( const int index, const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the Catmull-Rom spline basis functions for a given index and time parameter

		This function calculates the second derivative values for the Catmull-Rom spline basis functions at a specific index and time. The calculation uses a normalized time parameter derived from the
	   index and the time interval. The result is stored in the provided array of basis values.

		\param index The index of the spline segment to calculate the second derivative for
		\param t The time parameter for which to calculate the second derivative
		\param bvals Pointer to an array where the calculated second derivative values will be stored
	*/
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

/*
===============================================================================

	Cubic Interpolating Spline template.
	The curve goes through all the knots.
	The curve becomes the Catmull-Rom spline if the tension,
	continuity and bias are all set to zero.

===============================================================================
*/

template<class type>
class idCurve_KochanekBartelsSpline : public idCurve_Spline<type>
{
public:
	/*!
		\brief Constructs a new Kochanek-Bartels spline curve.

		This inline constructor initializes a new instance of the Kochanek-Bartels spline curve class. The spline is used for interpolation between keyframes and supports tension, continuity, and bias
	   parameters to control the shape of the curve segments.

	*/
	idCurve_KochanekBartelsSpline();

	/*!
		\brief Adds a new value to the Kochanek-Bartels spline curve at the specified time and returns its index.

		This function inserts a new value into the spline curve at the given time position. The function determines the correct insertion index based on the time value, then inserts the time, value,
	   and default tension, continuity, and bias parameters into their respective arrays. The returned index can be used to reference the newly added value in the spline.

		\param time The time value at which to add the new spline point
		\param value The value to add at the specified time
		\return The index of the newly added value in the spline curve
	*/
	virtual int	 AddValue( const float time, const type& value );

	/*!
		\brief Adds a new value to the Kochanek-Bartels spline curve at the specified time with the given tension, continuity, and bias parameters

		This function inserts a new keyframe into the spline curve at the specified time. The function calculates the appropriate index for insertion based on the time value and then inserts the time,
	   value, tension, continuity, and bias into their respective arrays. The function returns the index where the value was inserted

		\param time The time at which to add the new value
		\param value The value to add at the specified time
		\param tension The tension parameter for the Kochanek-Bartels spline
		\param continuity The continuity parameter for the Kochanek-Bartels spline
		\param bias The bias parameter for the Kochanek-Bartels spline
		\return The index in the arrays where the new value was inserted
	*/
	virtual int	 AddValue( const float time, const type& value, const float tension, const float continuity, const float bias );

	/*!
		\brief Removes the element at the specified index from all internal arrays of the spline curve.

		This function removes elements at the given index from all internal arrays of the spline curve, including values, times, tension, continuity, and bias arrays. It is designed to maintain
	   consistency across all arrays that represent the spline curve data.

		\param index The index of the element to be removed from all internal arrays
	*/
	virtual void RemoveIndex( const int index )
	{
		this->values.RemoveIndex( index );
		this->times.RemoveIndex( index );
		tension.RemoveIndex( index );
		continuity.RemoveIndex( index );
		bias.RemoveIndex( index );
	}

	//! Clears all stored values, times, and control points for the Kochanek-Bartels spline.
	virtual void Clear()
	{
		this->values.Clear();
		this->times.Clear();
		tension.Clear();
		continuity.Clear();
		bias.Clear();
		this->currentIndex = -1;
	}

	/*!
		\brief Returns the interpolated value at the specified time using Kochanek-Bartels spline interpolation.

		This function calculates and returns the value of the spline at the given time using Kochanek-Bartels interpolation method. It first clamps the time to the valid range, determines the
	   appropriate segment, and then computes the interpolated value using the basis functions and control points. The function handles edge cases such as when there is only one data point by
	   returning that point directly.

		\param time The time value at which to evaluate the spline
		\return The interpolated value at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the Kochanek-Bartels spline at the given time

		This function calculates the first derivative of a Kochanek-Bartels spline at a specified time value. It clamps the input time to the valid range of the spline, determines the appropriate
	   segment index, retrieves the tangent values for the segment, and computes the basis functions for the first derivative. The result is then combined with the spline values and tangents, and
	   normalized by the time interval to produce the final derivative value

		\param time The time value at which to compute the first derivative
		\return The first derivative of the spline at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the spline at the given time

		This function calculates the second derivative of a Kochanek-Bartels spline at a specified time value. It first clamps the input time to the valid range of the spline, then determines the
	   appropriate segment index. It retrieves the tangent values for the segment and computes the basis functions for the second derivative. The result is then combined with the spline values and
	   tangents, and normalized by the square of the time interval to produce the final second derivative value.

		\param time The time value at which to compute the second derivative
		\return The second derivative of the spline at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	idList<float> tension;
	idList<float> continuity;
	idList<float> bias;

	/*!
		\brief Computes the outgoing and incoming tangent vectors for a specified index in a Kochanek-Bartels spline interpolation.

		This function calculates tangent vectors at a given index in a Kochanek-Bartels spline by using tension, continuity, and bias parameters. It computes the tangent for the current point based on
	   neighboring points and the spline parameters. The function handles the outgoing tangent at the current index and the incoming tangent at the next index, adjusting for the time differences
	   between control points.

		\param index The index in the spline for which to compute the tangents
		\param t0 The outgoing tangent vector at the specified index
		\param t1 The incoming tangent vector at the next index
	*/
	void		  TangentsForIndex( const int index, type& t0, type& t1 ) const;

	/*!
		\brief Computes the basis values for Kochanek-Bartels spline interpolation at a given index and time

		This function evaluates the four basis functions that define a Kochanek-Bartels spline segment for a specific segment index and normalized time parameter. The computation is performed within a
	   given spline segment, where the normalized time parameter s is calculated from the absolute time t and the time range of the segment. The results are stored in the provided array of four float
	   values, corresponding to the four basis functions.

		\param index The index of the spline segment to evaluate
		\param t The absolute time parameter for the evaluation
		\param bvals Output array to store the four basis function values
	*/
	void		  Basis( const int index, const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of the basis functions for a Kochanek-Bartels spline at the specified index and time

		This function evaluates the first derivative of the four basis functions that define a Kochanek-Bartels spline segment. The computation is performed for a specific segment index and normalized
	   time parameter within that segment. The results are stored in the provided array of four float values, corresponding to the derivatives of the four basis functions. The function uses a
	   normalized time parameter s, calculated from the absolute time t and the time range of the segment

		\param index The index of the spline segment to evaluate
		\param t The absolute time parameter for the evaluation
		\param bvals Output array to store the four first derivative values of the basis functions
	*/
	void		  BasisFirstDerivative( const int index, const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the basis functions for a Kochanek-Bartels spline at the specified index and time.

		This function calculates the second derivatives of the basis functions for a Kochanek-Bartels spline. It takes an index and a time parameter to compute the second derivative values for the
	   basis functions. The computation uses a normalized time value 's' derived from the input time and the time range for the given index. The results are stored in the provided array 'bvals' which
	   must be large enough to hold four float values.

		\param index The index of the segment in the spline
		\param t The time value for which to compute the second derivative
		\param bvals Pointer to an array to store the computed second derivative values
	*/
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

/*
===============================================================================

	B-Spline base template. Uses recursive definition and is slow.
	Use idCurve_UniformCubicBSpline or idCurve_NonUniformBSpline instead.

===============================================================================
*/

template<class type>
class idCurve_BSpline : public idCurve_Spline<type>
{
public:
	/*!
		\brief Initializes a new instance of the idCurve_BSpline class with a default cubic order.

		This constructor initializes the B-spline curve with a default order of 4, which corresponds to a cubic spline. The order determines the degree of the polynomial used in the curve definition.

	*/
	idCurve_BSpline();

	//! Returns the order of the B-spline curve.
	virtual int	 GetOrder() const { return order; }

	/*!
		\brief Sets the order of the B-spline curve to the specified value

		The function asserts that the provided order is greater than 0 and less than 10 before setting the order of the B-spline curve

		\param i The order to set for the B-spline curve
	*/
	virtual void SetOrder( const int i )
	{
		assert( i > 0 && i < 10 );
		order = i;
	}

	/*!
		\brief Returns the current value of the B-spline curve at the specified time.

		This function evaluates the B-spline curve at the given time parameter and returns the interpolated value. It handles edge cases where there is only one control point, and uses a clamping
	   mechanism to ensure valid time values. The function uses a basis function to compute the weighted sum of control points surrounding the given time.

		\param time The time value at which to evaluate the B-spline curve
		\return The interpolated value of the B-spline curve at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the B-spline curve at the specified time

		This function calculates the first derivative of a B-spline curve at a given time value. It handles edge cases where there is only one control point by returning that control point directly.
	   The calculation uses a basis function derivative to compute the derivative at the specified time, taking into account the order of the spline and the clamped time value.

		\param time The time value at which to calculate the first derivative
		\return The first derivative of the B-spline curve at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the B-spline curve at the specified time

		This function calculates the second derivative of a B-spline curve at a given time value. It first clamps the time to the valid range and determines the appropriate index for the time value.
	   The second derivative is computed using a weighted sum of the basis functions and the control point values. The calculation accounts for the spline's order and handles edge cases where there is
	   only one control point.

		\param time The time value at which to calculate the second derivative
		\return The second derivative of the B-spline curve at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	int	  order;

	/*!
		\brief Evaluates the basis function for a B-spline curve at a given index, order, and parameter value.

		This function computes the value of a B-spline basis function using recursive evaluation. For order 1, it returns 1.0 if the parameter t lies within the support interval of the basis function,
	   otherwise 0.0. For higher orders, it recursively computes the basis function value using the Cox-de Boor recursion formula, with special handling for division by zero.

		\param index The index of the basis function to evaluate
		\param order The order of the basis function to evaluate
		\param t The parameter value at which to evaluate the basis function
		\return The computed value of the B-spline basis function at the specified parameter
	*/
	float Basis( const int index, const int order, const float t ) const;

	/*!
		\brief Computes the first derivative of a basis function for a B-spline curve

		This function calculates the first derivative of a basis function used in B-spline curve evaluation. It uses the recursive relationship between basis functions of different orders to compute
	   the derivative. The calculation involves the difference between two basis functions of a lower order, scaled by a factor based on the order and time differences between control points.

		\param index The index of the basis function to evaluate
		\param order The order of the basis function
		\param t The parameter value at which to evaluate the derivative
		\return The first derivative of the basis function at the specified parameter value
	*/
	float BasisFirstDerivative( const int index, const int order, const float t ) const;

	/*!
		\brief Calculates the second derivative of a basis function for a B-spline curve.

		This function computes the second derivative of a basis function used in B-spline curve interpolation. It utilizes the first derivative calculations to determine the second derivative by
	   applying a finite difference approach. The calculation depends on the index of the basis function, the order of the spline, and the parameter t which represents the position along the curve.

		\param index The index of the basis function to calculate the second derivative for
		\param order The order of the B-spline curve
		\param t The parameter value along the curve for which to calculate the second derivative
		\return The second derivative of the basis function at the specified parameter value
	*/
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

/*
===============================================================================

	Uniform Non-Rational Cubic B-Spline template.

===============================================================================
*/

template<class type>
class idCurve_UniformCubicBSpline : public idCurve_BSpline<type>
{
public:
	/*!
		\brief Constructs a new uniform cubic B-spline curve with a fixed order of four

		This constructor initializes a uniform cubic B-spline curve and sets its order to four, which is characteristic of cubic B-splines. The curve is designed to provide smooth interpolation
	   between control points with uniform spacing.

	*/
	idCurve_UniformCubicBSpline();

	/*!
		\brief Returns the interpolated value at the specified time from a uniform cubic B-spline curve.

		This function calculates and returns the interpolated value at a given time using a uniform cubic B-spline curve. It first clamps the input time to the valid range, then determines the
	   appropriate index for the time value. The function uses basis functions to compute the contribution of control points to the final interpolated value. Special handling is included for cases
	   where there is only one control point.

		\param time The time value for which to calculate the interpolated value
		\return The interpolated value at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the curve at the given time

		This function computes the first derivative of a uniform cubic B-spline curve at a specified time value. It calculates the derivative by evaluating the basis functions for the first derivative
	   and combining them with the control point values. The result is then scaled by the time interval between control points to provide the correct derivative magnitude. The function handles edge
	   cases where there is only one control point by returning a zero vector.

		\param time The time value at which to compute the first derivative
		\return The first derivative of the curve at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the spline at the given time.

		This function calculates the second derivative of a uniform cubic B-spline at a specified time value. It first clamps the time to the valid range of the spline, determines the appropriate
	   segment, and then computes the second derivative using basis functions. The result is normalized by the square of the time interval between control points to provide the correct second
	   derivative value.

		\param time The time value at which to calculate the second derivative
		\return The second derivative of the spline at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Evaluates the cubic B-spline basis functions for a given index and parameter

		This function computes the four basis functions for a uniform cubic B-spline at a specified index and parameter value. The basis functions are used to interpolate control points in B-spline
	   curve evaluation. The parameter t is normalized relative to the time interval defined by the current index and the next index. The results are stored in the bvals array, which must be able to
	   hold at least four float values.

		\param index The index of the basis function to evaluate
		\param t The parameter value for which to evaluate the basis functions
		\param bvals Output array to store the computed basis function values
	*/
	void Basis( const int index, const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of the basis functions for a uniform cubic B-spline at the given index and parameter.

		This function calculates the first derivatives of the four basis functions for a uniform cubic B-spline curve. It takes an index representing the knot interval and a parameter t within that
	   interval, then computes the derivatives of the four cubic B-spline basis functions at that point.

		\param index The index of the knot interval
		\param t The parameter value within the interval
		\param bvals Output array to store the computed derivatives of the basis functions
	*/
	void BasisFirstDerivative( const int index, const float t, float* bvals ) const;

	/*!
		\brief Computes the second derivative of the basis functions for a uniform cubic B-spline at the specified index and parameter value

		This function calculates the second derivative values for the basis functions of a uniform cubic B-spline curve. It takes an index and a parameter value t, and computes the corresponding
	   second derivative values into the provided array bvals. The computation is performed in the local parameter space defined by the time interval between consecutive keyframes

		\param index The index of the knot interval to evaluate
		\param t The parameter value within the specified interval
		\param bvals Output array to store the second derivative values
	*/
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

/*
===============================================================================

	Non-Uniform Non-Rational B-Spline (NUBS) template.

===============================================================================
*/

template<class type>
class idCurve_NonUniformBSpline : public idCurve_BSpline<type>
{
public:
	/*!
		\brief Constructs an empty non-uniform B-spline curve.

		This constructor initializes an empty non-uniform B-spline curve object. The curve is initially empty and can be populated with control points and knot values to define the spline geometry.
	   The curve is designed to represent a smooth interpolating curve through a set of control points using B-spline basis functions with non-uniform knot vectors.

	*/
	idCurve_NonUniformBSpline();

	/*!
		\brief Returns the current value of the non-uniform B-spline curve at the specified time.

		This function evaluates the non-uniform B-spline curve at the given time parameter and returns the interpolated value. It handles edge cases where there is only one control point by returning
	   that point's value directly. The function uses basis functions to compute the weighted sum of control points based on the current time.

		\param time The time parameter at which to evaluate the curve
		\return The interpolated value of the curve at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the non-uniform B-spline curve at the given time

		This function calculates the first derivative of a non-uniform B-spline curve at a specified time value. It uses the curve's control points and time knots to compute the derivative. The
	   function handles edge cases where there is only one control point by returning a zero vector. The calculation uses basis functions to determine the contribution of each control point to the
	   derivative at the given time.

		\param time The time value at which to compute the first derivative of the curve
		\return The first derivative of the B-spline curve at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the spline at the given time.

		This function calculates the second derivative of a non-uniform B-spline at a specified time value. It first clamps the input time to the valid range of the spline, determines the appropriate
	   index for the time value, and then evaluates the second derivative using the basis functions. The result is computed as a weighted sum of the control point values based on the basis function
	   evaluations.

		\param time The time value at which to calculate the second derivative
		\return The second derivative of the spline at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	/*!
		\brief Computes the basis values for a non-uniform B-spline at a given index and time

		This function calculates the basis values for a non-uniform B-spline using the Cox-de Boor recursion algorithm. It takes an index, order, and time parameter to compute the corresponding basis
	   function values. The computation uses the time values associated with the spline to determine the weights for each basis function. The function modifies the bvals array in-place, storing the
	   computed basis values for the specified order.

		\param index The index of the basis function to evaluate
		\param order The order of the B-spline
		\param t The time value at which to evaluate the basis functions
		\param bvals Pointer to the array where basis values will be stored
	*/
	void Basis( const int index, const int order, const float t, float* bvals ) const;

	/*!
		\brief Computes the first derivative of the basis functions for a non-uniform B-spline.

		This function calculates the first derivative of the basis functions used in non-uniform B-spline interpolation. It takes an index, order, and parameter t to compute the derivative values and
	   stores them in the provided array bvals. The calculation involves using the basis function values from a previous order and applying a specific weighting based on time values from the spline.

		\param index Starting index for the basis function calculation
		\param order Order of the B-spline basis functions
		\param t Parameter value for which to compute the derivative
		\param bvals Output array to store the computed derivative values
	*/
	void BasisFirstDerivative( const int index, const int order, const float t, float* bvals ) const;

	/*!
		\brief Calculates the second derivative of the basis functions for a non-uniform B-spline curve.

		This function computes the second derivative values for the basis functions used in non-uniform B-spline curve evaluation. It builds upon the first derivative calculations and applies
	   additional mathematical operations to derive the second derivative. The computation takes into account the order of the spline, the index of the basis function, and the parameter t. The results
	   are stored in the bvals array, which must be pre-allocated to accommodate the necessary number of values.

		\param index Index of the basis function to evaluate
		\param order Order of the B-spline
		\param t Parameter value for which to calculate the derivative
		\param bvals Pointer to array where the second derivative values will be stored
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

/*
===============================================================================

	Non-Uniform Rational B-Spline (NURBS) template.

===============================================================================
*/

template<class type>
class idCurve_NURBS : public idCurve_NonUniformBSpline<type>
{
public:
	/*!
		\brief Constructs a new NURBS curve object.

		This constructor initializes a new NURBS curve object. It sets up the internal data structures required for managing control points and curve evaluation. The curve is initially empty and
	   control points can be added using the appropriate methods.

	*/
	idCurve_NURBS();

	/*!
		\brief Adds a new control point with the given time and value to the NURBS curve and returns its index.

		This function inserts a new control point into the NURBS curve data structures. It determines the appropriate index for the new point based on the provided time value, then inserts the time,
	   value, and a default weight of 1.0f at that index in their respective arrays. The function returns the index where the new control point was inserted.

		\param time The time value for the new control point
		\param value The value for the new control point
		\return The index where the new control point was inserted in the curve data structures
	*/
	virtual int	 AddValue( const float time, const type& value );

	/*!
		\brief Adds a new control point with the specified time, value, and weight to the NURBS curve and returns its index.

		This function inserts a new control point into the NURBS curve data structures. It determines the appropriate index for the new point based on the provided time value, then inserts the time,
	   value, and weight at that index in their respective arrays. The function returns the index where the new control point was inserted.

		\param time The time value for the new control point
		\param value The value for the new control point
		\param weight The weight for the new control point
		\return The index where the new control point was inserted in the curve data structures
	*/
	virtual int	 AddValue( const float time, const type& value, const float weight );

	/*!
		\brief Removes the element at the specified index from the NURBS curve data structures

		This function removes elements at the given index from three internal data structures: values, times, and weights. It is designed to maintain the integrity of the NURBS curve by ensuring all
	   corresponding elements are removed together. The function assumes the index is valid and within the bounds of the data structures.

		\param index The index of the element to be removed from all NURBS curve data structures
	*/
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

	/*!
		\brief Returns the current value of the NURBS curve at the specified time

		This function calculates and returns the interpolated value of a NURBS curve at a given time. It uses the curve's control points, knot vector, and basis functions to compute the result. The
	   function clamps the input time to the valid range and performs basis function evaluation to determine the contribution of each control point. It handles edge cases where there is only one
	   control point and returns that point's value directly. The computation involves weighted averaging of control points based on the basis function values and weights.

		\param time The time value at which to evaluate the NURBS curve
		\return The interpolated value of the NURBS curve at the specified time
	*/
	virtual type GetCurrentValue( const float time ) const;

	/*!
		\brief Returns the first derivative of the NURBS curve at the given time

		This function calculates the first derivative of a Non-Uniform Rational B-Spline (NURBS) curve at a specified time value. It uses the basis functions and their first derivatives to compute the
	   result. The calculation takes into account the order of the curve, clamped time values, and weights for each control point. The function handles special cases like when there is only one time
	   value by returning the corresponding value directly.

		\param time The time value at which to calculate the first derivative of the curve
		\return The first derivative of the NURBS curve at the specified time
	*/
	virtual type GetCurrentFirstDerivative( const float time ) const;

	/*!
		\brief Returns the second derivative of the NURBS curve at the given time.

		This function computes the second derivative of a Non-Uniform Rational B-Spline (NURBS) curve at a specified time value. It uses basis functions and their derivatives to calculate the result.
	   The function first clamps the input time to valid range and determines the appropriate index for the curve segment. It then evaluates the basis functions and their first and second derivatives
	   at the given time. The computation involves weighted sums of these values and the control points to determine the final second derivative. Special handling is included for cases where there is
	   only one control point.

		\param time The time value at which to compute the second derivative of the curve
		\return The second derivative of the NURBS curve at the specified time
	*/
	virtual type GetCurrentSecondDerivative( const float time ) const;

protected:
	idList<float> weights;

	/*!
		\brief Returns the weight for the given index, handling boundary conditions for closed and open splines

		This function retrieves the weight value at the specified index from the weights array. It handles boundary conditions for both closed and open spline types. For indices less than zero, it
	   either wraps around for closed splines or extrapolates linearly for open splines. For indices greater than the maximum valid index, it either wraps around for closed splines or extrapolates
	   linearly for open splines. The function assumes that the weights array has been properly initialized and populated.

		\param index The index of the weight to retrieve
		\return The weight value at the specified index, potentially adjusted for boundary conditions
	*/
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
