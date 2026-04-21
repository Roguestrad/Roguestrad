/*
The MIT License (MIT)

Copyright (c) 2015 Yuriy O'Donnell
Copyright (c) 2021 Robert Beckebans (id Tech 4.x integration)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __MATH_SPHERICAL_HARMONICS_H__
#define __MATH_SPHERICAL_HARMONICS_H__

// RB: there is a very good talk by Yuriy O'Donnell that explains the the functions used in this library
// Precomputed Global Illumination in Frostbite (GDC 2018)
// https://www.gdcvault.com/play/1025214/Precomputed-Global-Illumination-in

// https://graphics.stanford.edu/papers/envmap/envmap.pdf

/*!
	\struct SphericalHarmonicsT
	\brief A template struct for storing and accessing spherical harmonics coefficients with degree and order indices.

	This class provides a structured way to store spherical harmonics coefficients in a flattened array format. The template parameters allow it to work with different data types T and a maximum
   degree L. The class supports both const and non-const access to coefficients via degree l and order m indices. The indexing scheme l * l + l + m efficiently maps the 2D degree-order indices to a 1D
   array position. This design is intended for use in lighting and shading calculations where spherical harmonics are employed to represent irradiance or other environmental lighting data.

*/
template<typename T, size_t L>
struct SphericalHarmonicsT {
	T		 data[( L + 1 ) * ( L + 1 )];

	const T& operator[]( size_t i ) const { return data[i]; }
	T&		 operator[]( size_t i ) { return data[i]; }

	/*!
		\brief Returns a reference to the spherical harmonics coefficient at the specified degree and order

		This function provides access to a specific coefficient in the spherical harmonics data structure using the degree l and order m indices. The data is stored in a flattened array where the
	   index is calculated as l * l + l + m. This allows for efficient access to the coefficients during spherical harmonics computations.

		\param l The degree of the spherical harmonics coefficient
		\param m The order of the spherical harmonics coefficient
		\return A reference to the spherical harmonics coefficient at the specified degree and order
	*/
	T&		 at( int l, int m ) { return data[l * l + l + m]; }

	/*!
		\brief Returns a const reference to the element at the specified spherical harmonics indices l and m

		This function provides access to elements stored in a flattened array representation of spherical harmonics data. The indexing formula computes the position in the array using the formula l *
	   l + l + m, which is typical for storing spherical harmonics coefficients in a compact linear structure. The function is marked as const, indicating it does not modify the object's state.

		\param l the degree index for the spherical harmonics coefficient
		\param m the order index for the spherical harmonics coefficient
		\return A const reference to the spherical harmonics coefficient at the specified indices l and m
	*/
	const T& at( int l, int m ) const { return data[l * l + l + m]; }
};

typedef SphericalHarmonicsT<float, 1>  SphericalHarmonicsL1;
typedef SphericalHarmonicsT<float, 2>  SphericalHarmonicsL2;
typedef SphericalHarmonicsT<idVec3, 1> SphericalHarmonicsL1RGB;
typedef SphericalHarmonicsT<idVec3, 2> SphericalHarmonicsL2RGB;

template<typename T, size_t L>
SphericalHarmonicsL1 shEvaluateL1( idVec3 p );
SphericalHarmonicsL2 shEvaluateL2( idVec3 p );

/*!
	\brief Returns the size of a spherical harmonics array for a given degree L

	This function calculates the required array size for storing spherical harmonics coefficients up to a given degree L. The formula (L + 1) * (L + 1) represents the number of coefficients needed for
   a spherical harmonics expansion of degree L, which is commonly used in lighting and rendering applications for representing irradiance or radiance data.

	\param L The degree of the spherical harmonics expansion
	\return The total number of coefficients required for a spherical harmonics array of degree L
*/
inline size_t		 shSize( size_t L )
{
	return ( L + 1 ) * ( L + 1 );
}

/*!
	\brief Adds a weighted spherical harmonics component to an accumulator.

	This function performs an in-place addition of a weighted spherical harmonics vector to an existing accumulator. Each coefficient of the input spherical harmonics vector is multiplied by the
   provided weight and added to the corresponding coefficient in the accumulator.

	\param accumulatorSh The spherical harmonics accumulator to which the weighted component will be added
	\param sh The spherical harmonics component to be added
	\param weight The weight to be applied to the spherical harmonics component before adding
*/
template<typename Ta, typename Tb, typename Tw, size_t L>
inline void shAddWeighted( SphericalHarmonicsT<Ta, L>& accumulatorSh, const SphericalHarmonicsT<Tb, L>& sh, const Tw& weight )
{
	for( size_t i = 0; i < shSize( L ); ++i ) {
		accumulatorSh[i] += ( sh[i] * weight );
	}
}

/*!
	\brief Computes the dot product of two spherical harmonics vectors.

	This function calculates the dot product of two spherical harmonics vectors by multiplying corresponding coefficients and summing the results. It iterates through all coefficients of the spherical
   harmonics up to the specified order L and computes their weighted sum.

	\param shA First spherical harmonics vector
	\param shB Second spherical harmonics vector
	\return The dot product result of the two spherical harmonics vectors
*/
template<typename Ta, typename Tb, size_t L>
inline Ta shDot( const SphericalHarmonicsT<Ta, L>& shA, const SphericalHarmonicsT<Tb, L>& shB )
{
	Ta result = Ta( 0 );
	for( size_t i = 0; i < shSize( L ); ++i ) {
		result += ( shA[i] * shB[i] );
	}
	return result;
}

/*!
	\brief Computes spherical harmonics coefficients for a given direction vector

	This function evaluates spherical harmonics up to degree L for a given 3D direction vector. It implements the mathematical computation of spherical harmonic basis functions using the standard
   formulas derived from Peter-Pike Sloan's work. The function supports degrees up to 4 and returns a SphericalHarmonicsT object containing the computed coefficients for the specified degree.

	\param dir The direction vector for which to evaluate the spherical harmonics coefficients
	\return A SphericalHarmonicsT object containing the computed spherical harmonics coefficients for the given direction vector
*/
template<size_t L>
inline SphericalHarmonicsT<float, L> shEvaluate( idVec3 dir )
{
	// From Peter-Pike Sloan's Stupid SH Tricks
	// http://www.ppsloan.org/publications/StupidSH36.pdf
	// https://github.com/dariomanesku/cmft/blob/master/src/cmft/cubemapfilter.cpp#L130

	static_assert( L <= 4, "Spherical Harmonics above L4 are not supported" );

	SphericalHarmonicsT<float, L> result;

	const float					  x = dir.x;
	const float					  y = dir.y;
	const float					  z = dir.z;

	const float					  x2 = x * x;
	const float					  y2 = y * y;
	const float					  z2 = z * z;

	const float					  z3 = z2 * z;

	const float					  x4 = x2 * x2;
	const float					  y4 = y2 * y2;
	const float					  z4 = z2 * z2;

	const float					  sqrtPi = sqrt( idMath::PI );

	size_t						  i = 0;

	result[i++] = 1.0f / ( 2.0f * sqrtPi );

	if( L >= 1 ) {
		result[i++] = -sqrt( 3.0f / ( 4.0f * idMath::PI ) ) * y;
		result[i++] = sqrt( 3.0f / ( 4.0f * idMath::PI ) ) * z;
		result[i++] = -sqrt( 3.0f / ( 4.0f * idMath::PI ) ) * x;
	}

	if( L >= 2 ) {
		result[i++] = sqrt( 15.0f / ( 4.0f * idMath::PI ) ) * y * x;
		result[i++] = -sqrt( 15.0f / ( 4.0f * idMath::PI ) ) * y * z;
		result[i++] = sqrt( 5.0f / ( 16.0f * idMath::PI ) ) * ( 3.0f * z2 - 1.0f );
		result[i++] = -sqrt( 15.0f / ( 4.0f * idMath::PI ) ) * x * z;
		result[i++] = sqrt( 15.0f / ( 16.0f * idMath::PI ) ) * ( x2 - y2 );
	}

	if( L >= 3 ) {
		result[i++] = -sqrt( 70.0f / ( 64.0f * idMath::PI ) ) * y * ( 3.0f * x2 - y2 );
		result[i++] = sqrt( 105.0f / ( 4.0f * idMath::PI ) ) * y * x * z;
		result[i++] = -sqrt( 21.0f / ( 16.0f * idMath::PI ) ) * y * ( -1.0f + 5.0f * z2 );
		result[i++] = sqrt( 7.0f / ( 16.0f * idMath::PI ) ) * ( 5.0f * z3 - 3.0f * z );
		result[i++] = -sqrt( 42.0f / ( 64.0f * idMath::PI ) ) * x * ( -1.0f + 5.0f * z2 );
		result[i++] = sqrt( 105.0f / ( 16.0f * idMath::PI ) ) * ( x2 - y2 ) * z;
		result[i++] = -sqrt( 70.0f / ( 64.0f * idMath::PI ) ) * x * ( x2 - 3.0f * y2 );
	}

	if( L >= 4 ) {
		result[i++] = 3.0f * sqrt( 35.0f / ( 16.0f * idMath::PI ) ) * x * y * ( x2 - y2 );
		result[i++] = -3.0f * sqrt( 70.0f / ( 64.0f * idMath::PI ) ) * y * z * ( 3.0f * x2 - y2 );
		result[i++] = 3.0f * sqrt( 5.0f / ( 16.0f * idMath::PI ) ) * y * x * ( -1.0f + 7.0f * z2 );
		result[i++] = -3.0f * sqrt( 10.0f / ( 64.0f * idMath::PI ) ) * y * z * ( -3.0f + 7.0f * z2 );
		result[i++] = ( 105.0f * z4 - 90.0f * z2 + 9.0f ) / ( 16.0f * sqrtPi );
		result[i++] = -3.0f * sqrt( 10.0f / ( 64.0f * idMath::PI ) ) * x * z * ( -3.0f + 7.0f * z2 );
		result[i++] = 3.0f * sqrt( 5.0f / ( 64.0f * idMath::PI ) ) * ( x2 - y2 ) * ( -1.0f + 7.0f * z2 );
		result[i++] = -3.0f * sqrt( 70.0f / ( 64.0f * idMath::PI ) ) * x * z * ( x2 - 3.0f * y2 );
		result[i++] = 3.0f * sqrt( 35.0f / ( 4.0f * ( 64.0f * idMath::PI ) ) ) * ( x4 - 6.0f * y2 * x2 + y4 );
	}

	return result;
}

/*!
	\brief Evaluates the first-order spherical harmonics at the given point

	This function computes the first-order spherical harmonics coefficients for a given 3D point. It serves as a specialized evaluation function for spherical harmonics of degree 1, which are commonly
   used in lighting and shading calculations for their ability to represent low-frequency lighting environments.

	\param p The 3D point at which to evaluate the spherical harmonics
	\return The computed first-order spherical harmonics coefficients at the specified point
*/
inline SphericalHarmonicsL1 shEvaluateL1( idVec3 p )
{
	return shEvaluate<1>( p );
}

/*!
	\brief Evaluates the spherical harmonics of degree 2 at the given point

	This function computes the spherical harmonics coefficients up to degree 2 for a given 3D point. It is a specialized inline implementation that leverages a generic template function shEvaluate to
   perform the actual computation. The result represents a second-degree spherical harmonic approximation suitable for lighting and shading calculations in 3D graphics applications.

	\param p The 3D point at which to evaluate the spherical harmonics
	\return The computed spherical harmonics coefficients up to degree 2
*/
inline SphericalHarmonicsL2 shEvaluateL2( idVec3 p )
{
	return shEvaluate<2>( p );
}

/*!
	\brief Evaluates the diffuse lighting contribution from a spherical harmonics L1 representation for a given surface normal.

	This function computes the diffuse lighting value using a spherical harmonics L1 representation and a surface normal. It implements a mathematical model for reconstructing diffuse lighting from
   spherical harmonic coefficients, based on the Geomerics paper reference. The calculation involves computing coefficients from the spherical harmonics data and applying a power function to determine
   the final lighting contribution for the given normal direction.

	\param sh The spherical harmonics L1 representation containing the lighting coefficients
	\param n The surface normal vector for which to evaluate the diffuse lighting
	\return The computed diffuse lighting contribution as a floating point value
*/
inline float shEvaluateDiffuseL1Geomerics( const SphericalHarmonicsL1& sh, const idVec3& n )
{
	// http://www.geomerics.com/wp-content/uploads/2015/08/CEDEC_Geomerics_ReconstructingDiffuseLighting1.pdf

	float  R0 = sh[0];

	idVec3 R1	 = 0.5f * idVec3( sh[3], sh[1], sh[2] );
	float  lenR1 = R1.Length();

	// float q = 0.5f * (1.0f + dot(R1 / lenR1, n));
	float  q = 0.5f * ( 1.0f + ( R1 / lenR1 ) * n );

	float  p = 1.0f + 2.0f * lenR1 / R0;
	float  a = ( 1.0f - lenR1 / R0 ) / ( 1.0f + lenR1 / R0 );

	return R0 * ( a + ( 1.0f - a ) * ( p + 1.0f ) * pow( q, p ) );
}

/*!
	\brief Computes a diffuse convolution of the input spherical harmonics coefficients.

	This function applies a diffuse convolution to the provided spherical harmonics coefficients, which is commonly used in lighting calculations to simulate diffuse irradiance. The convolution is
   performed using precomputed coefficients based on the spherical harmonic degree. The implementation follows the mathematical formulation described in the referenced paper for computing diffuse
   lighting from environment maps.

	\param sh The input spherical harmonics coefficients to be convolved
	\return A new set of spherical harmonics coefficients representing the diffuse convolution of the input
*/
template<typename T, size_t L>
inline SphericalHarmonicsT<T, L> shConvolveDiffuse( SphericalHarmonicsT<T, L>& sh )
{
	SphericalHarmonicsT<T, L> result;

	// https://cseweb.ucsd.edu/~ravir/papers/envmap/envmap.pdf equation 8

	const float				  A[5] = { idMath::PI, idMath::PI * 2.0f / 3.0f, idMath::PI * 1.0f / 4.0f, 0.0f, -idMath::PI * 1.0f / 24.0f };

	int						  i = 0;
	for( int l = 0; l <= ( int )L; ++l ) {
		for( int m = -l; m <= l; ++m ) {
			result[i] = sh[i] * A[l];
			++i;
		}
	}

	return result;
}

/*!
	\brief Evaluates the diffuse irradiance contribution of spherical harmonics in a given direction

	This function computes the diffuse irradiance contribution from spherical harmonics coefficients for a specific direction. It uses precomputed constants for different spherical harmonic bands and
   applies them to the input coefficients and direction. The function supports up to degree 4 spherical harmonics and uses a specific weighting scheme based on the harmonic degree. The implementation
   follows the mathematical formulation described in the referenced paper for evaluating diffuse irradiance from spherical harmonics.

	\param sh Spherical harmonics coefficients to evaluate
	\param direction The direction in which to evaluate the spherical harmonics
	\return The computed diffuse irradiance contribution as a scalar value of type T
	\throws Assertion error if the spherical harmonic degree L exceeds 4
*/
template<typename T, size_t L>
inline T shEvaluateDiffuse( const SphericalHarmonicsT<T, L>& sh, const idVec3& direction )
{
	static_assert( L <= 4, "Spherical Harmonics above L4 are not supported" );

	SphericalHarmonicsT<float, L> directionSh = shEvaluate<L>( direction );

	// https://cseweb.ucsd.edu/~ravir/papers/envmap/envmap.pdf equation 8

	const float					  A[5] = { idMath::PI, idMath::PI * 2.0f / 3.0f, idMath::PI * 1.0f / 4.0f, 0.0f, -idMath::PI * 1.0f / 24.0f };

	size_t						  i = 0;

	T							  result = sh[i] * directionSh[i] * A[0];
	++i;

	if( L >= 1 ) {
		result += sh[i] * directionSh[i] * A[1];
		++i;
		result += sh[i] * directionSh[i] * A[1];
		++i;
		result += sh[i] * directionSh[i] * A[1];
		++i;
	}

	if( L >= 2 ) {
		result += sh[i] * directionSh[i] * A[2];
		++i;
		result += sh[i] * directionSh[i] * A[2];
		++i;
		result += sh[i] * directionSh[i] * A[2];
		++i;
		result += sh[i] * directionSh[i] * A[2];
		++i;
		result += sh[i] * directionSh[i] * A[2];
		++i;
	}

	// L3 and other odd bands > 1 have 0 factor

	if( L >= 4 ) {
		i = 16;

		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
		result += sh[i] * directionSh[i] * A[4];
		++i;
	}

	return result;
}

/*!
	\brief Evaluates the diffuse lighting contribution from a spherical harmonics representation at a given direction

	This function computes the diffuse lighting value by evaluating the spherical harmonics coefficients at the specified direction. It is a specialized version for the first order spherical harmonics
   (L=1) which is commonly used for lighting calculations in computer graphics. The function takes the spherical harmonics representation and a direction vector, then returns the resulting luminance
   value.

	\param sh The spherical harmonics representation containing the lighting coefficients
	\param direction The direction vector to evaluate the lighting contribution at
	\return The computed diffuse lighting contribution as a scalar value of type T
*/
template<typename T>
inline T shEvaluateDiffuseL1( const SphericalHarmonicsT<T, 1>& sh, const idVec3& direction )
{
	return shEvaluateDiffuse<T, 1>( sh, direction );
}

/*!
	\brief Evaluates the diffuse lighting contribution from a spherical harmonics representation at a given direction
	\param sh The spherical harmonics representation of the lighting
	\param direction The direction to evaluate the lighting contribution
	\return The diffuse lighting contribution evaluated at the specified direction
*/
template<typename T>
inline T shEvaluateDiffuseL2( const SphericalHarmonicsT<T, 2>& sh, const idVec3& direction )
{
	return shEvaluateDiffuse<T, 2>( sh, direction );
}

/*!
	\brief Computes the windowing lambda value to reduce the squared Laplacian of spherical harmonics below a specified threshold

	This function calculates a lambda value used for windowing spherical harmonics to reduce their squared Laplacian below a given maximum threshold. It uses an iterative numerical method based on the
   appendix A7 of the referenced paper to solve for the optimal lambda value. The computation involves pre-calculating lookup tables for the squared Laplacian values and the sum of squared
   coefficients for each degree, then performing a Newton-Raphson style iteration to find the appropriate windowing factor.

	\param sh Input spherical harmonics coefficients
	\param maxLaplacian Maximum allowed squared Laplacian value
	\return The computed windowing lambda value that reduces the squared Laplacian below the specified threshold, or 0.0 if the current squared Laplacian is already below the threshold
*/
template<size_t L>
float shFindWindowingLambda( const SphericalHarmonicsT<float, L>& sh, float maxLaplacian )
{
	// http://www.ppsloan.org/publications/StupidSH36.pdf
	// Appendix A7 Solving for Lambda to Reduce the Squared Laplacian

	float tableL[L + 1];
	float tableB[L + 1];

	tableL[0] = 0.0f;
	tableB[0] = 0.0f;

	for( int l = 1; l <= ( int )L; ++l ) {
		tableL[l] = float( Square( l ) * Square( l + 1 ) );

		float B = 0.0f;
		for( int m = -1; m <= l; ++m ) {
			B += Square( sh.at( l, m ) );
		}
		tableB[l] = B;
	}

	float squaredLaplacian = 0.0f;

	for( int l = 1; l <= ( int )L; ++l ) {
		squaredLaplacian += tableL[l] * tableB[l];
	}

	const float targetSquaredLaplacian = maxLaplacian * maxLaplacian;
	if( squaredLaplacian <= targetSquaredLaplacian ) { return 0.0f; }

	float		   lambda = 0.0f;

	const uint32_t iterationLimit = 10000000;
	for( uint32_t i = 0; i < iterationLimit; ++i ) {
		float f	 = 0.0f;
		float fd = 0.0f;

		for( int l = 1; l <= ( int )L; ++l ) {
			f += tableL[l] * tableB[l] / Square( 1.0f + lambda * tableL[l] );
			fd += ( 2.0f * Square( tableL[l] ) * tableB[l] ) / Cube( 1.0f + lambda * tableL[l] );
		}

		f = targetSquaredLaplacian - f;

		float delta = -f / fd;
		lambda += delta;

		if( idMath::Fabs( delta ) < 1e-6f ) { break; }
	}

	return lambda;
}

/*!
	\brief Applies windowing to spherical harmonics coefficients to reduce high-frequency artifacts

	This function applies a windowing operation to the spherical harmonics coefficients to reduce high-frequency artifacts in the representation. The windowing is based on Peter-Pike Sloan's technique
   described in "Stupid SH Tricks". The lambda parameter controls the amount of windowing applied, with higher values leading to more aggressive filtering of high-frequency components. The function
   processes coefficients in order of increasing degree l, applying a scaling factor that depends on the degree and the lambda parameter.

	\param sh The spherical harmonics coefficients to be windowed
	\param lambda The windowing parameter that controls the amount of high-frequency artifact reduction
*/
template<typename T, size_t L>
void shApplyWindowing( SphericalHarmonicsT<T, L>& sh, float lambda )
{
	// From Peter-Pike Sloan's Stupid SH Tricks
	// http://www.ppsloan.org/publications/StupidSH36.pdf

	int i = 0;
	for( int l = 0; l <= ( int )L; ++l ) {
		float s = 1.0f / ( 1.0f + lambda * l * l * ( l + 1.0f ) * ( l + 1.0f ) );
		for( int m = -l; m <= l; ++m ) {
			sh[i++] *= s;
		}
	}
}

#if 0
template <typename T, size_t L>
inline T shMeanSquareError( const SphericalHarmonicsT<T, L>& sh, const idArray<RadianceSample>& radianceSamples )
{
	T errorSquaredSum = T( 0.0f );

	for( const RadianceSample& sample : radianceSamples )
	{
		auto directionSh = shEvaluate<L>( sample.direction );
		auto reconstructedValue = shDot( sh, directionSh );
		auto error = sample.value - reconstructedValue;
		errorSquaredSum += error * error;
	}

	float sampleWeight = 1.0f / radianceSamples.size();
	return errorSquaredSum * sampleWeight;
}

template <typename T, size_t L>
inline float shMeanSquareErrorScalar( const SphericalHarmonicsT<T, L>& sh, const idArray<RadianceSample>& radianceSamples )
{
	return dot( shMeanSquareError( sh, radianceSamples ), T( 1.0f / 3.0f ) );
}
#endif

/*!
	\brief Computes the luminance of a spherical harmonics representation by converting each coefficient from RGB to grayscale.

	This function takes a spherical harmonics representation containing RGB color values and converts each coefficient to its corresponding luminance value using the rgbLuminance function. The result
   is a new spherical harmonics representation where each coefficient is a grayscale value instead of a color. The function processes all coefficients in the spherical harmonics up to the specified
   degree L.

	\param sh Input spherical harmonics representation containing RGB color coefficients
	\return Spherical harmonics representation with luminance values instead of color values
*/
template<size_t L>
inline SphericalHarmonicsT<float, L> shLuminance( const SphericalHarmonicsT<idVec3, L>& sh )
{
	SphericalHarmonicsT<float, L> result;
	for( size_t i = 0; i < shSize( L ); ++i ) {
		result[i] = rgbLuminance( sh[i] );
	}
	return result;
}

#endif // __MATH_SPHERICAL_HARMONICS_H__
