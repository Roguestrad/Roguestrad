/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Stephen Pridham

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
#include "precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

/*!
	\brief Converts a heightmap into a normal map using gradient estimation

	This function transforms a heightmap represented as an array of bytes into a normal map by estimating gradients from neighboring pixels. It first converts the input data to grayscale, then
   computes normal vectors at each pixel by analyzing height differences in the x and y directions. The scale parameter controls the intensity of the normal map effect. The function handles edge cases
   by wrapping around the texture boundaries using bit masking operations. The resulting normal map is stored back in the input data array with RGBA format where the alpha channel is set to 255.

	\param data Pointer to the heightmap data in RGBA format
	\param width Width of the heightmap in pixels
	\param height Height of the heightmap in pixels
	\param scale Scaling factor for the normal map intensity
*/
static void R_HeightmapToNormalMap( byte* data, int width, int height, float scale )
{
	int	  i, j;
	byte* depth;

	scale = scale / 256;

	// copy and convert to grey scale
	j	  = width * height;
	depth = ( byte* )R_StaticAlloc( j, TAG_IMAGE );
	for( i = 0; i < j; i++ ) {
		depth[i] = ( data[i * 4] + data[i * 4 + 1] + data[i * 4 + 2] ) / 3;
	}

	idVec3 dir, dir2;
	for( i = 0; i < height; i++ ) {
		for( j = 0; j < width; j++ ) {
			int d1, d2, d3, d4;
			int a1, a2, a3, a4;

			// FIXME: look at five points?

			// look at three points to estimate the gradient
			a1 = d1 = depth[( i * width + j )];
			a2 = d2 = depth[( i * width + ( ( j + 1 ) & ( width - 1 ) ) )];
			a3 = d3 = depth[( ( ( i + 1 ) & ( height - 1 ) ) * width + j )];
			a4 = d4 = depth[( ( ( i + 1 ) & ( height - 1 ) ) * width + ( ( j + 1 ) & ( width - 1 ) ) )];

			d2 -= d1;
			d3 -= d1;

			dir[0] = -d2 * scale;
			dir[1] = -d3 * scale;
			dir[2] = 1;
			dir.NormalizeFast();

			a1 -= a3;
			a4 -= a3;

			dir2[0] = -a4 * scale;
			dir2[1] = a1 * scale;
			dir2[2] = 1;
			dir2.NormalizeFast();

			dir += dir2;
			dir.NormalizeFast();

			a1			 = ( i * width + j ) * 4;
			data[a1 + 0] = ( byte )( dir[0] * 127 + 128 );
			data[a1 + 1] = ( byte )( dir[1] * 127 + 128 );
			data[a1 + 2] = ( byte )( dir[2] * 127 + 128 );
			data[a1 + 3] = 255;
		}
	}

	R_StaticFree( depth );
}

/*!
	\brief Scales image data by applying a color channel scale factor to each pixel component.

	This function modifies image data in-place by scaling each pixel component with corresponding values from the scale array. The scale array contains four floating-point values that represent
   multipliers for the red, green, blue, and alpha channels respectively. Each pixel component is clamped between 0 and 255 after scaling.

	\param data Pointer to the image data array
	\param width Width of the image in pixels
	\param height Height of the image in pixels
	\param scale Array of four scale factors for RGB and alpha channels
*/
static void R_ImageScale( byte* data, int width, int height, float scale[4] )
{
	int i, j;
	int c;

	c = width * height * 4;

	for( i = 0; i < c; i++ ) {
		j = ( byte )( data[i] * scale[i & 3] );
		if( j < 0 ) {
			j = 0;
		} else if( j > 255 ) {
			j = 255;
		}
		data[i] = j;
	}
}

//! Inverts the alpha channel of an image buffer in place
static void R_InvertAlpha( byte* data, int width, int height )
{
	int i;
	int c;

	c = width * height * 4;

	for( i = 0; i < c; i += 4 ) {
		data[i + 3] = 255 - data[i + 3];
	}
}

//! Inverts the green channel of the provided image data
static void R_InvertGreen( byte* data, int width, int height )
{
	int i;
	int c;

	c = width * height * 4;

	for( i = 0; i < c; i += 4 ) {
		data[i + 1] = 255 - data[i + 1];
	}
}

//! Inverts the RGB color values of image data in place.
static void R_InvertColor( byte* data, int width, int height )
{
	int i;
	int c;

	c = width * height * 4;

	for( i = 0; i < c; i += 4 ) {
		data[i + 0] = 255 - data[i + 0];
		data[i + 1] = 255 - data[i + 1];
		data[i + 2] = 255 - data[i + 2];
	}
}

/*!
	\brief Adds normal map data from the second texture to the first texture and renormalizes the result.

	This function takes two normal maps as input and combines them by adding the normal vector from the second map to the first. The normal vectors are converted from byte values to normalized float
   vectors, the second map's normals are added to the first, and the result is renormalized to ensure it remains a valid unit vector. The function handles resampling of the second map to match the
   dimensions of the first if necessary. The resulting combined normal map is stored in the first map's data.

	\param data1 Pointer to the first normal map data
	\param width1 Width of the first normal map
	\param height1 Height of the first normal map
	\param data2 Pointer to the second normal map data
	\param width2 Width of the second normal map
	\param height2 Height of the second normal map
*/
static void R_AddNormalMaps( byte* data1, int width1, int height1, byte* data2, int width2, int height2 )
{
	int	  i, j;
	byte* newMap;

	// resample pic2 to the same size as pic1
	if( width2 != width1 || height2 != height1 ) {
		newMap = R_Dropsample( data2, width2, height2, width1, height1 );
		data2  = newMap;
	} else {
		newMap = NULL;
	}

	// add the normal change from the second and renormalize
	for( i = 0; i < height1; i++ ) {
		for( j = 0; j < width1; j++ ) {
			byte * d1, *d2;
			idVec3 n;
			float  len;

			d1 = data1 + ( i * width1 + j ) * 4;
			d2 = data2 + ( i * width1 + j ) * 4;

			n[0] = ( d1[0] - 128 ) / 127.0;
			n[1] = ( d1[1] - 128 ) / 127.0;
			n[2] = ( d1[2] - 128 ) / 127.0;

			// There are some normal maps that blend to 0,0,0 at the edges
			// this screws up compression, so we try to correct that here by instead fading it to 0,0,1
			len = n.LengthFast();
			if( len < 1.0f ) {
				n[2] = idMath::Sqrt( 1.0 - ( n[0] * n[0] ) - ( n[1] * n[1] ) );
			}

			n[0] += ( d2[0] - 128 ) / 127.0;
			n[1] += ( d2[1] - 128 ) / 127.0;
			n.Normalize();

			d1[0] = ( byte )( n[0] * 127 + 128 );
			d1[1] = ( byte )( n[1] * 127 + 128 );
			d1[2] = ( byte )( n[2] * 127 + 128 );
			d1[3] = 255;
		}
	}

	if( newMap ) {
		R_StaticFree( newMap );
	}
}

//! Applies smoothing to a normal map by averaging neighboring normal vectors.
static void R_SmoothNormalMap( byte* data, int width, int height )
{
	byte*		 orig;
	int			 i, j, k, l;
	idVec3		 normal;
	byte*		 out;
	static float factors[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };

	orig = ( byte* )R_StaticAlloc( width * height * 4, TAG_IMAGE );
	memcpy( orig, data, width * height * 4 );

	for( i = 0; i < width; i++ ) {
		for( j = 0; j < height; j++ ) {
			normal = vec3_origin;
			for( k = -1; k < 2; k++ ) {
				for( l = -1; l < 2; l++ ) {
					byte* in;

					in = orig + ( ( ( j + l ) & ( height - 1 ) ) * width + ( ( i + k ) & ( width - 1 ) ) ) * 4;

					// ignore 000 and -1 -1 -1
					if( in[0] == 0 && in[1] == 0 && in[2] == 0 ) {
						continue;
					}
					if( in[0] == 128 && in[1] == 128 && in[2] == 128 ) {
						continue;
					}

					normal[0] += factors[k + 1][l + 1] * ( in[0] - 128 );
					normal[1] += factors[k + 1][l + 1] * ( in[1] - 128 );
					normal[2] += factors[k + 1][l + 1] * ( in[2] - 128 );
				}
			}
			normal.Normalize();
			out	   = data + ( j * width + i ) * 4;
			out[0] = ( byte )( 128 + 127 * normal[0] );
			out[1] = ( byte )( 128 + 127 * normal[1] );
			out[2] = ( byte )( 128 + 127 * normal[2] );
		}
	}

	R_StaticFree( orig );
}

/*!
	\brief Adds the pixel data of two images together, with clamping to prevent overflow.

	This function takes two image buffers and adds their pixel values together. If the dimensions of the two images differ, the second image is resampled to match the first. The addition is performed
   component-wise on RGBA values, with each component clamped to the range [0, 255]. The result is stored in the first image buffer.

	\param data1 The first image buffer to store the result
	\param width1 Width of the first image
	\param height1 Height of the first image
	\param data2 The second image buffer to add
	\param width2 Width of the second image
	\param height2 Height of the second image
*/
static void R_ImageAdd( byte* data1, int width1, int height1, byte* data2, int width2, int height2 )
{
	int	  i, j;
	int	  c;
	byte* newMap;

	// resample pic2 to the same size as pic1
	if( width2 != width1 || height2 != height1 ) {
		newMap = R_Dropsample( data2, width2, height2, width1, height1 );
		data2  = newMap;
	} else {
		newMap = NULL;
	}

	c = width1 * height1 * 4;

	for( i = 0; i < c; i++ ) {
		j = data1[i] + data2[i];
		if( j > 255 ) {
			j = 255;
		}
		data1[i] = j;
	}

	if( newMap ) {
		R_StaticFree( newMap );
	}
}

/*!
	\brief Combines grayscale texture data from two or three input arrays into RGBA format in the first array

	This function takes three grayscale texture arrays and combines them into a single RGBA texture array. The first array's red channel is filled with data from the first input array. The second
   array's red channel is filled with data from the second input array. The third array's red channel is used for the blue channel of the output if the third array is provided and has matching
   dimensions with the first array. The alpha channel is set to 255 for all pixels. The function assumes all input arrays contain grayscale data and performs assertions to verify that the first two
   arrays have matching dimensions.

	\param data1 Output RGBA texture data array
	\param width1 Width of the first texture
	\param height1 Height of the first texture
	\param data2 Input grayscale texture data for green channel
	\param width2 Width of the second texture
	\param height2 Height of the second texture
	\param data3 Optional grayscale texture data for blue channel
	\param width3 Width of the third texture
	\param height3 Height of the third texture
	\throws assertion failure if width1 != width2 or height1 != height2
*/
static void R_CombineRgba( byte* data1, int width1, int height1, byte* data2, int width2, int height2, byte* data3, int width3, int height3 )
{
	assert( width1 == width2 );
	// assert(width2 == width3);
	assert( height1 == height2 );

	for( int j = 0; j < 4 * height1; j += 4 ) {
		for( int i = 0; i < 4 * width1; i += 4 ) {
			// Assume that these textures are all grayscale images. just take the r channel of each and set them to
			// the respective rgb.
			byte r = data1[i + j * width1];

			byte g = data2[i + j * width1];

			byte b = 255;

			if( data3 && width1 == width3 ) {
				b = data3[i + j * width1];
			}

			byte a = 255;

			data1[0 + i + j * width1] = r;
			data1[1 + i + j * width1] = g;
			data1[2 + i + j * width1] = b;
			data1[3 + i + j * width1] = a;
		}
	}
}
// SP end

// we build a canonical token form of the image program here
static char parseBuffer[MAX_IMAGE_NAME];

//! Appends a token to the parse buffer with a leading space if not at the beginning.
static void AppendToken( idToken& token )
{
	// add a leading space if not at the beginning
	if( parseBuffer[0] ) {
		idStr::Append( parseBuffer, MAX_IMAGE_NAME, " " );
	}
	idStr::Append( parseBuffer, MAX_IMAGE_NAME, token.c_str() );
}

//! Matches a token string from the lexer and appends it to the parse buffer.
static void MatchAndAppendToken( idLexer& src, const char* match )
{
	if( !src.ExpectTokenString( match ) ) {
		return;
	}
	// a matched token won't need a leading space
	idStr::Append( parseBuffer, MAX_IMAGE_NAME, match );
}

/*!
	\brief Parses an image program from a lexer and applies various image processing operations.

	This function recursively parses an image program specified in a lexer, applying various image processing operations such as heightmap conversion, normal map addition, smoothing, image addition,
   scaling, and color inversion. It handles special tokens like _black and _white by replacing them with specific texture paths. The function can operate in different modes depending on whether the
   output image pointer, width, height, and timestamps are provided. If pic is NULL, only timestamps are filled in without generating an image. If both pic and timestamps are NULL, it just advances
   through the input without processing.

	\param src Lexer object containing the image program tokens
	\param pic Pointer to store the resulting image data, or NULL to only parse without generating an image
	\param width Pointer to store the image width
	\param height Pointer to store the image height
	\param timestamps Pointer to store the timestamp of the image, or NULL to skip timestamp retrieval
	\param usage Pointer to store the texture usage flag, or NULL to skip usage flag setting
	\return True if the image program was parsed and processed successfully, false otherwise.
*/
static bool R_ParseImageProgram_r( idLexer& src, byte** pic, int* width, int* height, ID_TIME_T* timestamps, textureUsage_t* usage )
{
	idToken	  token;
	ID_TIME_T timestamp;

	src.ReadToken( &token );

	// Since all interaction shaders now assume YCoCG diffuse textures.  We replace all entries for the intrinsic
	// _black texture to the black texture on disk.  Doing this will cause a YCoCG compliant texture to be generated.
	// Without a YCoCG compliant black texture we will get color artifacts for any interaction
	// material that specifies the _black texture.
	if( token == "_black" ) {
		token = "textures\\black";
	}

	// also check for _white
	if( token == "_white" ) {
		token = "guis\\assets\\white";
	}

	AppendToken( token );

	if( !token.Icmp( "heightmap" ) ) {
		MatchAndAppendToken( src, "(" );

		if( !R_ParseImageProgram_r( src, pic, width, height, timestamps, usage ) ) {
			return false;
		}

		MatchAndAppendToken( src, "," );

		src.ReadToken( &token );
		AppendToken( token );
		float scale = token.GetFloatValue();

		// process it
		if( pic ) {
			R_HeightmapToNormalMap( *pic, *width, *height, scale );
			if( usage ) {
				*usage = TD_BUMP;
			}
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "addnormals" ) ) {
		byte* pic2 = NULL;
		int	  width2, height2;

		MatchAndAppendToken( src, "(" );

		if( !R_ParseImageProgram_r( src, pic, width, height, timestamps, usage ) ) {
			return false;
		}

		MatchAndAppendToken( src, "," );

		if( !R_ParseImageProgram_r( src, pic ? &pic2 : NULL, &width2, &height2, timestamps, usage ) ) {
			if( pic ) {
				R_StaticFree( *pic );
				*pic = NULL;
			}
			return false;
		}

		// process it
		if( pic ) {
			R_AddNormalMaps( *pic, *width, *height, pic2, width2, height2 );
			R_StaticFree( pic2 );
			if( usage ) {
				*usage = TD_BUMP;
			}
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "smoothnormals" ) ) {
		MatchAndAppendToken( src, "(" );

		if( !R_ParseImageProgram_r( src, pic, width, height, timestamps, usage ) ) {
			return false;
		}

		if( pic ) {
			R_SmoothNormalMap( *pic, *width, *height );
			if( usage ) {
				*usage = TD_BUMP;
			}
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "add" ) ) {
		byte* pic2 = NULL;
		int	  width2, height2;

		MatchAndAppendToken( src, "(" );

		if( !R_ParseImageProgram_r( src, pic, width, height, timestamps, usage ) ) {
			return false;
		}

		MatchAndAppendToken( src, "," );

		if( !R_ParseImageProgram_r( src, pic ? &pic2 : NULL, &width2, &height2, timestamps, usage ) ) {
			if( pic ) {
				R_StaticFree( *pic );
				*pic = NULL;
			}
			return false;
		}

		// process it
		if( pic ) {
			R_ImageAdd( *pic, *width, *height, pic2, width2, height2 );
			R_StaticFree( pic2 );
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "scale" ) ) {
		float scale[4];
		int	  i;

		MatchAndAppendToken( src, "(" );

		R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

		for( i = 0; i < 4; i++ ) {
			MatchAndAppendToken( src, "," );
			src.ReadToken( &token );
			AppendToken( token );
			scale[i] = token.GetFloatValue();
		}

		// process it
		if( pic ) {
			R_ImageScale( *pic, *width, *height, scale );
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "invertAlpha" ) ) {
		MatchAndAppendToken( src, "(" );

		R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

		// process it
		if( pic ) {
			R_InvertAlpha( *pic, *width, *height );
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	// RB: invertGreen to allow flipping the Y-Axis of normal maps
	if( !token.Icmp( "invertGreen" ) ) {
		MatchAndAppendToken( src, "(" );

		R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

		// process it
		if( pic ) {
			R_InvertGreen( *pic, *width, *height );
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "invertColor" ) ) {
		MatchAndAppendToken( src, "(" );

		R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

		// process it
		if( pic ) {
			R_InvertColor( *pic, *width, *height );
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "makeIntensity" ) ) {
		int i;

		MatchAndAppendToken( src, "(" );

		R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

		// copy red to green, blue, and alpha
		if( pic ) {
			int c;
			c = *width * *height * 4;
			for( i = 0; i < c; i += 4 ) {
				( *pic )[i + 1] = ( *pic )[i + 2] = ( *pic )[i + 3] = ( *pic )[i];
			}
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "makeAlpha" ) ) {
		int i;

		MatchAndAppendToken( src, "(" );

		R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

		// average RGB into alpha, then set RGB to white
		if( pic ) {
			int c;
			c = *width * *height * 4;
			for( i = 0; i < c; i += 4 ) {
				( *pic )[i + 3] = ( ( *pic )[i + 0] + ( *pic )[i + 1] + ( *pic )[i + 2] ) / 3;
				( *pic )[i + 0] = ( *pic )[i + 1] = ( *pic )[i + 2] = 255;
			}
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	if( !token.Icmp( "combineRgba" ) ) {
		byte* pic2 = nullptr;
		byte* pic3 = nullptr;
		int	  width2, height2;
		int	  width3, height3;

		MatchAndAppendToken( src, "(" );

		if( !R_ParseImageProgram_r( src, pic, width, height, timestamps, usage ) ) {
			return false;
		}

		MatchAndAppendToken( src, "," );

		if( !R_ParseImageProgram_r( src, pic ? &pic2 : NULL, &width2, &height2, timestamps, usage ) ) {
			if( pic ) {
				R_StaticFree( *pic );
				*pic = NULL;
			}
			return false;
		}

		MatchAndAppendToken( src, "," );

		if( !R_ParseImageProgram_r( src, pic2 ? &pic3 : NULL, &width3, &height3, timestamps, usage ) ) {
			if( pic ) {
				R_StaticFree( *pic );
				*pic = NULL;
			}
			return false;
		}

		// process it
		if( pic ) {
			R_CombineRgba( *pic, *width, *height, pic2, width2, height2, pic3, width3, height3 );
			R_StaticFree( pic2 );
			R_StaticFree( pic3 );
		}

		MatchAndAppendToken( src, ")" );
		return true;
	}

	// if we are just parsing instead of loading or checking,
	// don't do the R_LoadImage
	if( !timestamps && !pic ) {
		return true;
	}

	// load it as an image
	R_LoadImage( token.c_str(), pic, width, height, &timestamp, true, usage );

	if( timestamp == -1 ) {
		return false;
	}

	// add this to the timestamp
	if( timestamps ) {
		if( timestamp > *timestamps ) {
			*timestamps = timestamp;
		}
	}

	return true;
}

/*!
	\brief Loads and parses an image program file, extracting texture data and metadata.

	This function serves as the entry point for loading image programs, which are typically used for defining texture properties and behaviors in the engine. It initializes a lexer to process the
   input file, sets appropriate parsing flags, and calls the recursive parsing function to extract image data and associated metadata such as dimensions, timestamp, and usage flags. The function
   handles memory management for the lexer and ensures proper cleanup.

	\param name name
	\param pic byte **
	\param width int *
	\param height int *
	\param timestamps ID_TIME_T *
	\param usage textureUsage_t *
*/
void R_LoadImageProgram( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamps, textureUsage_t* usage )
{
	idLexer src;

	src.LoadMemory( name, strlen( name ), name );
	src.SetFlags( LEXFL_NOFATALERRORS | LEXFL_NOSTRINGCONCAT | LEXFL_NOSTRINGESCAPECHARS | LEXFL_ALLOWPATHNAMES );

	parseBuffer[0] = 0;
	if( timestamps ) {
		*timestamps = 0;
	}

	R_ParseImageProgram_r( src, pic, width, height, timestamps, usage );

	src.FreeSource();
}

const char* R_ParsePastImageProgram( idLexer& src )
{
	parseBuffer[0] = 0;
	R_ParseImageProgram_r( src, NULL, NULL, NULL, NULL, NULL );
	return parseBuffer;
}
