/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2014-2020 Robert Beckebans

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
#ifndef __DXTCODEC_H__
#define __DXTCODEC_H__

/*!
	\class idDxtEncoder
	\brief A texture compression utility class providing various DXT and specialized format compression methods for image data.

	This class serves as a comprehensive texture compression utility offering multiple compression algorithms for different texture formats. It provides both high-quality and fast compression methods
   for various DXT formats including DXT1, DXT5, and specialized formats like DXN1, DXN2, and CTX1. The implementation includes both generic algorithms and SSE2-optimized variants for
   performance-critical applications. The class supports compression of standard RGB images, normal maps, and YCoCg color space representations, with methods for both high-quality exhaustive search
   approaches and fast approximation methods. It also includes utility functions for format conversion, alpha encoding, and color space transformations. The design emphasizes flexibility through
   multiple compression strategies while maintaining performance through hardware-specific optimizations.

*/
class idDxtEncoder
{
public:
	//! Constructs an idDxtEncoder object and initializes srcPadding and dstPadding to zero.
	idDxtEncoder() { srcPadding = dstPadding = 0; }
	~idDxtEncoder() { }

	//! Sets the source padding value for the DXT encoder.
	void SetSrcPadding( int pad ) { srcPadding = pad; }

	//! Sets the destination padding value for the DXT encoder.
	void SetDstPadding( int pad ) { dstPadding = pad; }

	/*!
		\brief Compresses an image to DXT1 format with high quality, using exhaustive search for optimal color encoding.

		This function performs high quality DXT1 compression on the input image data without alpha channel support. It uses an exhaustive search algorithm to find the optimal line through color space
	   for each 4x4 block, which makes the compression process slow but produces better visual quality. The function handles blocks smaller than 4x4 by using a specialized method for tiny color
	   blocks. The compression works by finding minimum error color indices for two possible color orderings and selecting the one with lower error.

		\param inBuf Input buffer containing RGBA pixel data
		\param outBuf Output buffer where compressed DXT1 data will be written
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageDXT1HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses an image to DXT1 format using fast compression methods suitable for real-time use.

		This function performs DXT1 compression on the input image data, with a fast compression method that trades some quality for speed. It uses SSE2 intrinsics if available, otherwise falls back
	   to a generic implementation. The function is designed for real-time compression scenarios where performance is more critical than maximum quality.

		\param inBuf Input buffer containing the image data to compress
		\param outBuf Output buffer where the compressed DXT1 data will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageDXT1Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 block of RGBA image data into DXT1 format using a fast generic algorithm.

		This function performs DXT1 texture compression on a portion of an image buffer. It processes the input image in 4x4 pixel blocks and produces compressed output in DXT1 format. The compression
	   uses a simple color quantization method that selects a minimum and maximum color from each block to define a color range. The function handles padding for both source and destination buffers to
	   ensure proper alignment. The compression is optimized for speed rather than maximum quality.

		\param inBuf Pointer to the input RGBA image data
		\param outBuf Pointer to the output buffer for compressed DXT1 data
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws assertion failure if width or height is less than 4, or not a multiple of 4
	*/
	void CompressImageDXT1Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 block of RGBA image data into DXT1 format using SSE2 optimizations

		This function performs DXT1 texture compression on image data using SSE2 instructions for improved performance. It processes the input image in 4x4 pixel blocks, extracting color information
	   and generating compressed output. The function assumes the input width and height are multiples of 4 and handles padding for the output buffer. The compression uses a min/max color box approach
	   to determine the two endpoint colors for each block. The function includes a test mode that validates the SSE2 implementation against a generic reference implementation to ensure correctness.

		\param inBuf pointer to the input RGBA image data
		\param outBuf pointer to the output compressed DXT1 data
		\param width width of the input image in pixels
		\param height height of the input image in pixels
		\throws assertion failure if width or height is less than 4, or if width or height are not divisible by 4
	*/
	void CompressImageDXT1Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses input image data into DXT1 format with alpha channel using high quality exhaustive search method.

		This function performs high quality DXT1 compression on image data that includes an alpha channel. It uses an exhaustive search algorithm to find optimal color representations in color space,
	   which results in higher quality compression but at a significant performance cost. The function is currently not implemented and will assert on execution.

		\param inBuf Input buffer containing the source image data
		\param outBuf Output buffer where the compressed DXT1 data will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws assertion failure when executed due to unimplemented functionality
	*/
	void CompressImageDXT1AlphaHQ( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Compresses an image into DXT1 format with alpha channel using a fast algorithm suitable for real-time use.

		This function performs DXT1 compression on an input image buffer with alpha channel. It selects between an SSE2-optimized implementation or a generic implementation based on available hardware
	   features. The compression is optimized for speed rather than quality, making it suitable for real-time applications.

		\param inBuf Input buffer containing the source image data
		\param outBuf Output buffer where the compressed DXT1 data will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageDXT1AlphaFast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 pixel block of image data into DXT1 format with alpha handling using a fast generic algorithm

		This function performs DXT1 compression on image data using a fast generic algorithm that processes 4x4 pixel blocks. It extracts color information from each block, determines the minimum and
	   maximum colors, and then encodes the color indices based on alpha values. The function handles alpha transparency by checking if the minimum alpha value is greater than or equal to 128, and
	   emits color indices accordingly.

		\param inBuf input image data buffer containing 4-byte RGBA pixels
		\param outBuf output buffer for compressed DXT1 data
		\param width width of the image in pixels, must be multiple of 4
		\param height height of the image in pixels, must be multiple of 4
		\throws assertion failure if width or height is less than 4, or not divisible by 4
	*/
	void CompressImageDXT1AlphaFast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 block of RGBA image data into DXT1 format with alpha channel using SSE2 optimization.

		This function performs DXT1 alpha compression on a 4x4 pixel block of image data using SSE2 instruction set for optimization. The input image data is expected to be in RGBA format with 4 bytes
	   per pixel. The function processes the image in 4x4 pixel blocks and emits 64-bit compressed blocks to the output buffer. It uses minimum and maximum color bounding box computation to determine
	   color indices for compression. The compression handles alpha channel by examining the minimum alpha value and choosing appropriate color encoding. The function includes a test mode that
	   validates the SSE2 implementation against a generic implementation.

		\param inBuf pointer to the input RGBA image data
		\param outBuf pointer to the output compressed DXT1 data
		\param width width of the image in pixels, must be multiple of 4
		\param height height of the image in pixels, must be multiple of 4
		\throws Assertion failure if width or height is less than 4 or not divisible by 4
	*/
	void CompressImageDXT1AlphaFast_SSE2( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses an image to DXT5 format with high quality using exhaustive search for optimal color interpolation.

		This function performs high quality DXT5 texture compression on an input image buffer. It processes the image in 4x4 pixel blocks and uses exhaustive search to find optimal color endpoints for
	   interpolation. The function handles both large and small images, with special handling for images smaller than 4x4 pixels. It also includes a hardware bug fix for NVIDIA 7x series GPUs. The
	   compression process involves finding minimum error alpha indices and color indices to achieve the best visual quality at the expense of performance.

		\param inBuf Input buffer containing the source image data in RGBA format
		\param outBuf Output buffer where the compressed DXT5 data will be written
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageDXT5HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses an image into DXT5 format using a fast algorithm suitable for real-time applications.

		This function performs DXT5 texture compression on input image data. It uses SSE2 instructions if available for faster processing, otherwise falls back to a generic implementation. The
	   compression is optimized for speed rather than quality, making it appropriate for real-time rendering scenarios where performance is more critical than visual fidelity.

		\param inBuf Pointer to the input image data
		\param outBuf Pointer to the output compressed data buffer
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 block of image data into DXT5 format using a fast generic algorithm.

		This function performs DXT5 texture compression on a block of image data. It processes the input image in 4x4 pixel blocks, extracting color information and generating compressed output. The
	   compression uses a limited color palette with alpha channel handling. The function assumes input dimensions are multiples of 4 pixels in both width and height. It handles padding for both
	   source and destination buffers during processing.

		\param inBuf Pointer to the input image data
		\param outBuf Pointer to the output compressed data buffer
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws assertion failure if width or height is less than 4, or not divisible by 4
	*/
	void CompressImageDXT5Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 block of RGBA image data into DXT5 format using SSE2 optimization.

		This function performs DXT5 texture compression on a 4x4 pixel block of image data. It uses SSE2 instructions for optimization and assumes the input width and height are multiples of 4. The
	   function processes the image in 4x4 pixel chunks, extracting color blocks, computing minimum and maximum colors, adjusting color bounds, and emitting compressed color and alpha data to the
	   output buffer. Padding is handled for both source and destination buffers during processing.

		\param inBuf Pointer to the source image data in RGBA format
		\param outBuf Pointer to the output buffer where compressed DXT5 data will be written
		\param width Width of the input image, must be a multiple of 4
		\param height Height of the input image, must be a multiple of 4
		\throws assertion failure if width or height is less than 4 or not a multiple of 4
	*/
	void CompressImageDXT5Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses input image data using high quality BC6 compression with R11G11B10 format.

		This function performs high quality BC6 compression on image data that is formatted as R11G11B10. The input buffer contains the raw pixel data while the output buffer will contain the
	   compressed data. The function is currently not implemented and will cause a fatal error if called.

		\param inBuf Input buffer containing the raw image data in R11G11B10 format
		\param outBuf Output buffer that will contain the compressed BC6 data
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws Fatal error indicating the function is not implemented
	*/
	void CompressImageR11G11B10_BC6HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses RGB images with 11-bit R, 11-bit G, and 10-bit B components into BC6 format using fast SIMD or generic implementation.

		This function performs fast compression of RGB images with 11-bit R, 11-bit G, and 10-bit B components into BC6 format. It uses SIMD intrinsics if available, otherwise falls back to a generic
	   implementation. The compression is optimized for real-time use, trading minor quality for performance.

		\param inBuf Input buffer containing the source image data
		\param outBuf Output buffer where the compressed BC6 data will be written
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageR11G11B10_BC6Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief This function is a placeholder that currently asserts and does not perform any actual compression.

		The function is intended to compress image data using the BC6 format with R11G11B10 layout, but the current implementation only asserts and does not perform the actual compression. It takes an
	   input buffer containing image data and an output buffer for the compressed result, along with the width and height of the image.

		\param inBuf Pointer to the input buffer containing the image data to be compressed
		\param outBuf Pointer to the output buffer where the compressed image data will be stored
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws Asserts when called, indicating that the function is not yet implemented
	*/
	void CompressImageR11G11B10_BC6Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

#if( defined( USE_INTRINSICS_SSE ) || defined( USE_INTRINSICS_NEON ) )

	/*!
		\brief Compresses R11G11B10 image data to BC6H format using SIMD optimization

		This function performs BC6H compression on R11G11B10 format image data using SIMD optimization. It validates the input dimensions to ensure they are multiples of 4, allocates a temporary
	   half-precision floating-point buffer, converts the input image data from R11G11B10 format to FP16, and then compresses the data using BC6H algorithm. The function requires input width and
	   height to be at least 4 pixels and divisible by 4 to function correctly.

		\param inBuf Input buffer containing R11G11B10 format image data
		\param outBuf Output buffer to store the BC6H compressed data
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws Error when allocation of FP16 buffer fails
	*/
	void CompressImageR11G11B10_BC6Fast_SIMD( const byte* inBuf, byte* outBuf, int width, int height );
#endif

	/*!
		\brief Compresses an image using high quality CTX1 compression with exhaustive search for optimal color line determination.

		This function performs DXT1-based texture compression on a 4x4 pixel block using an exhaustive search approach to determine the best color line in 2D color space. The compression process
	   involves extracting 4x4 pixel blocks from the input buffer, finding minimum and maximum colors, and determining optimal color indices for each block. The function handles edge cases where the
	   image dimensions are smaller than 4x4 by using a specialized tiny color compression method. The output is written to the provided output buffer in a format compatible with DXT1 texture
	   compression standards.

		\param inBuf Input buffer containing the source image data in RGBA format
		\param outBuf Output buffer where the compressed DXT1 data will be written
		\param width Width of the source image in pixels
		\param height Height of the source image in pixels
	*/
	void CompressImageCTX1HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Fast CTX1 compression for real-time use

		This function performs fast CTX1 compression suitable for real-time applications. It takes an input buffer containing image data and compresses it into aoutput buffer using the CTX1
	   compression format. The function asserts that it is never called, indicating it may be a placeholder or stub implementation.

		\param inBuf Input buffer containing the image data to be compressed
		\param outBuf Output buffer where the compressed data will be stored
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws Assertation failure when called
	*/
	void CompressImageCTX1Fast( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief This function is a placeholder that currently asserts and does not perform any actual compression.

		The function is intended to compress image data using a fastCTX1 algorithm but currently throws an assertion error. It takes an input buffer containing image data, an output buffer for the
	   compressed data, and the dimensions of the image. The function signature suggests it's part of a texture compression system, likely for use in the Roguestrad engine's rendering pipeline.

		\param inBuf Input buffer containing the uncompressed image data
		\param outBuf Output buffer where the compressed data will be written
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws Assertion failure when called
	*/
	void CompressImageCTX1Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief This function is a placeholder that currently asserts and does not perform any actual compression.

		The function is intended to compress image data using a fast SSE2 implementation for the CTX1 format, but currently it only contains an assertion that always fails. This suggests that the
	   actual implementation is either incomplete or has been disabled for some reason. The parameters specify the input buffer, output buffer, width, and height of the image to be compressed.

		\param inBuf Pointer to the input image data buffer
		\param outBuf Pointer to the output compressed data buffer
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws Assertion failure when called
	*/
	void CompressImageCTX1Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Compresses an image using high quality DXN1 compression with exhaustive search for optimal color line placement.

		This function performs high quality DXN1 compression, also known as DXT5A or ATI1N compression. It uses an exhaustive search algorithm to find the optimal line through the color space, which
	   results in superior quality but significantly slower performance compared to standard compression methods. The compression is designed for use with normal maps or other textures where quality
	   is prioritized over speed. The implementation currently contains an assertion that will always fail, indicating this function is not yet implemented or is intentionally disabled.

		\param inBuf Input buffer containing the source image data
		\param outBuf Output buffer where the compressed image will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws Assertion failure due to assert( 0 ) statement indicating the function is not implemented
	*/
	void CompressImageDXN1HQ( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Compresses a single channel image into DXN1 format for real-time use.

		This function performs fast compression of a single channel image into DXN1 format, which is also known as DXT5A or ATI1N. It is optimized for real-time processing scenarios. The compression
	   is implemented through a generic compression routine that handles the actual encoding process.

		\param inBuf Input buffer containing the source image data
		\param outBuf Output buffer where the compressed DXN1 data will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressImageDXN1Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses an image using the DXN1 format with a fast generic algorithm.

		This function performs image compression using the DXN1 format. It processes the input image in 4x4 pixel blocks and writes compressed output to the provided buffer. The function assumes that
	   both width and height are multiples of 4. It extracts color blocks, determines min/max values for each block, and emits alpha indices. The compression is optimized for speed at the expense of
	   some quality. The function updates internal state variables for width, height, and output data pointer during processing. It also handles padding for both source and destination buffers.

		\param inBuf input image buffer containing pixel data
		\param outBuf output buffer for compressed data
		\param width width of the image in pixels
		\param height height of the image in pixels
		\throws assertion failure if width or height is less than 4, or if width or height is not a multiple of 4
	*/
	void CompressImageDXN1Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief This function is a placeholder that currently does nothing but assert false, indicating it is not yet implemented.

		The function idDxtEncoder::CompressImageDXN1Fast_SSE2 is intended to compress image data using the DXN1 format with SSE2 optimizations. It takes an input buffer containing image data, an
	   output buffer for the compressed data, and the dimensions of the image. However, the current implementation only contains an assertion that always fails, suggesting that this functionality has
	   not been implemented yet.

		\param inBuf Pointer to the input buffer containing the image data to be compressed
		\param outBuf Pointer to the output buffer where the compressed data will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws Assertion failure due to unimplemented function
	*/
	void CompressImageDXN1Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Compresses input image data into YCoCg DXT5 format with high quality using exhaustive search for optimal color representation

		This function performs high quality compression of image data into DXT5 format using YCoCg color space representation. The compression process involves finding optimal color endpoints through
	   exhaustive search to minimize error. The function handles blocks of 4x4 pixels and processes the entire image by subdividing it into these blocks. For small images less than 4x4 pixels, it uses
	   a specialized method for tiny color DXT5 encoding. The function also handles alpha channel compression by trying two encoding methods and selecting the one with minimum error. The process
	   includes scaling to YCoCg color space, finding min/max colors and alpha values, and then encoding both color and alpha data with proper indexing.

		\param inBuf Input buffer containing RGBA pixel data to be compressed
		\param outBuf Output buffer where compressed DXT5 data will be written
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws assertion failure if the input data does not maintain constant values per 4x4 block
	*/
	void CompressYCoCgDXT5HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses input image data into YCoCg DXT5 format using fast algorithm suitable for real-time use.

		This function performs fast YCoCg DXT5 compression on the provided input buffer. The input data is expected to be in CoCg_Y format. The function uses SSE2 intrinsics if available, otherwise
	   falls back to a generic implementation. The output is written to the provided output buffer in DXT5 format.

		\param inBuf Input buffer containing image data in CoCg_Y format
		\param outBuf Output buffer where compressed DXT5 data will be written
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
	*/
	void CompressYCoCgDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses an image buffer into DXT5 format using YCoCg color space with fast compression algorithm

		This function performs DXT5 compression on image data using the YCoCg color space transformation for improved compression quality. The input image must have dimensions that are multiples of 4
	   pixels in both width and height. The function processes the image in 4x4 pixel blocks and applies various color optimization techniques including min/max bounding box calculation, YCoCg
	   scaling, and diagonal selection for optimal color representation. The compression uses 565 format for color representation and employs alpha channel indices for transparency handling.

		\param inBuf pointer to the input image buffer containing RGBA pixel data
		\param outBuf pointer to the output buffer where compressed DXT5 data will be stored
		\param width width of the input image in pixels, must be multiple of 4
		\param height height of the input image in pixels, must be multiple of 4
		\throws assertion failure if width or height are less than 4 or not multiples of 4
	*/
	void CompressYCoCgDXT5Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a 4x4 block of RGBA image data into DXT5 format using YCoCg color space with SSE2 optimizations

		This function performs DXT5 compression on image data using the YCoCg color space representation for improved quality. It processes the input image in 4x4 pixel blocks, applying SSE2
	   optimizations for performance. The function extracts color blocks, determines min/max color values, scales colors in YCoCg space, and encodes alpha and color indices according to the DXT5
	   format specification. The implementation includes validation checks for block dimensions and uses aligned memory allocation for SSE2 operations. A test mode is included that validates the SSE2
	   implementation against a generic reference implementation.

		\param inBuf pointer to input RGBA image data
		\param outBuf pointer to output compressed DXT5 data
		\param width width of the image in pixels
		\param height height of the image in pixels
		\throws assertion failures when width or height are not multiples of 4, or when block validation fails
	*/
	void CompressYCoCgDXT5Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses YCoCg alpha texture data into DXT5 format using a fast algorithm suitable for real-time applications.

		This function performs fast compression of YCoCg alpha texture data into DXT5 format. It processes the input buffer in 4x4 pixel blocks and uses a specialized algorithm optimized for real-time
	   rendering. The input is expected to be in CoCgAY format, and the function applies chroma scaling to texels that are close to gray with low luminance. The compression algorithm extracts color
	   information, determines minimum and maximum bounding colors, and encodes the data using the DXT5 compression format.

		\param inBuf Input buffer containing YCoCg alpha texture data
		\param outBuf Output buffer for the compressed DXT5 data
		\param width Width of the texture in pixels, must be multiple of 4
		\param height Height of the texture in pixels, must be multiple of 4
		\throws assertion failure if width or height is less than 4 or not multiple of 4
	*/
	void CompressYCoCgAlphaDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses input image data using high quality YCoCg CTX1 with DXT5A encoding for alpha channels.

		This function performs high quality compression of image data using the YCoCg color space transformation combined with CTX1 and DXT5A encoding methods. It processes the input buffer in 4x4
	   pixel blocks and applies exhaustive search techniques to find optimal encoding parameters for both color and alpha channels. The function handles edge cases where image dimensions are smaller
	   than 4x4 pixels by using a specialized tiny color encoding method. The compression process includes finding minimum error alpha indices and color indices through exhaustive search, which makes
	   this method computationally expensive but produces high quality results.

		\param inBuf input buffer containing pixel data in RGBA format
		\param outBuf output buffer receiving compressed DXT5 data
		\param width width of the input image in pixels
		\param height height of the input image in pixels
		\throws assertion failure if input data doesn't maintain constant values per 4x4 block
	*/
	void CompressYCoCgCTX1DXT5AHQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses input data in YCoCg CTX1 format to DXT5A format for real-time use.

		This function performs fast compression of input data that is expected to be in CoCg_Y format. It uses a generic compression method to convert the data into DXT5A format, suitable for
	   real-time applications.

		\param inBuf Input buffer containing data in CoCg_Y format
		\param outBuf Output buffer where compressed DXT5A data will be stored
		\param width Width of the input data in pixels
		\param height Height of the input data in pixels
	*/
	void CompressYCoCgCTX1DXT5AFast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses an image using YCoCg CTX1 DXT5A format with fast generic algorithm

		This function performs compression of image data using the YCoCg CTX1 DXT5A format. It processes the input image in 4x4 pixel blocks and applies specialized color compression techniques. The
	   function assumes that the input image dimensions are divisible by 4 and that each 4x4 block contains constant values in the alpha channel. The compression process extracts color blocks,
	   determines min/max color bounds, adjusts colors according to YCoCg diagonal, and emits compressed data in the DXT5A format including alpha and color indices.

		\param inBuf pointer to input image data
		\param outBuf pointer to output compressed data buffer
		\param width width of the image in pixels
		\param height height of the image in pixels
		\throws assertion failures if input dimensions are invalid or if constant value assumption is violated
	*/
	void CompressYCoCgCTX1DXT5AFast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief This function is a placeholder that currently asserts and does not perform any compression operation.

		The function is intended to compress image data using the YCoCgCTX1 DXT5A format with SSE2 optimization. It takes an input buffer containing image data and an output buffer to store the
	   compressed result. The width and height parameters specify the dimensions of the input image. However, the current implementation only contains an assertion that fails, indicating that the
	   function is not yet implemented.

		\param inBuf Pointer to the input buffer containing image data to be compressed
		\param outBuf Pointer to the output buffer where the compressed data will be stored
		\param width Width of the input image in pixels
		\param height Height of the input image in pixels
		\throws Assertion failure when called
	*/
	void CompressYCoCgCTX1DXT5AFast_SSE2( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Compresses a normal map into DXT1 format with high quality

		This function performs high quality compression of a normal map into DXT1 format. The input is a buffer containing normal map data with RGBA channels, where the Z component is not used in the
	   compression. The output is a compressed DXT1 format buffer. The function handles edge cases where the width or height are less than 4 by using a specialized tiny color compression method. For
	   larger images, it processes the image in 4x4 pixel blocks, extracting color information and finding optimal color indices for DXT1 compression. The function also manages padding for both input
	   and output buffers.

		\param inBuf Input buffer containing normal map data with RGBA channels
		\param outBuf Output buffer for the compressed DXT1 data
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void CompressNormalMapDXT1HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a normal map image to DXT1 format with high quality renormalization

		This function takes a normal map image buffer and compresses it into DXT1 format with high quality renormalization. It processes the input image in 4x4 pixel blocks and handles images smaller
	   than 4x4 pixels separately. The function ensures that the image dimensions are multiples of 4, returning early if they are not. For each 4x4 block, it extracts the pixel data, clears the alpha
	   channel, and computes min/max normals for color interpolation. The resulting color indices and color values are then written to the output buffer in DXT1 format. The function updates a progress
	   indicator during processing.

		\param inBuf input image buffer containing normal map data
		\param outBuf output buffer for compressed DXT1 data
		\param width width of the input image in pixels
		\param height height of the input image in pixels
	*/
	void CompressNormalMapDXT1RenormalizeHQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Fast compression of tangent space normal maps into DXT1 format for real-time use

		This function performs fast compression of normal maps stored in tangent space into DXT1 format. The Nz component is not used in the compression process. The implementation currently asserts
	   and does not perform actual compression, suggesting it may be a placeholder or stub function.

		\param inBuf Input buffer containing the normal map data
		\param outBuf Output buffer where the compressed DXT1 data will be written
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
		\throws assertion failure when called
	*/
	void CompressNormalMapDXT1Fast( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief This function is a placeholder that always asserts and does not perform any actual compression of normal map data.

		The function is declared to compress normal map data using DXT1 format but currently does nothing more than assert. It takes an input buffer containing normal map data, and output buffer for
	   the compressed result, along with width and height dimensions. The actual implementation is not provided and always triggers an assertion failure.

		\param inBuf Input buffer containing normal map data to be compressed
		\param outBuf Output buffer where the compressed DXT1 data will be stored
		\param width Width of the input normal map in pixels
		\param height Height of the input normal map in pixels
		\throws assertion failure
	*/
	void CompressNormalMapDXT1Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief This function is intended to compress a normal map into DXT1 format using SSE2 optimizations, but currently asserts and does nothing.

		The function is designed to compress normal map data from a source buffer into DXT1 compressed format using SSE2 instructions for optimization. It takes an input buffer containing normal map
	   data, an output buffer for the compressed data, and the dimensions of the map. The implementation currently contains an assertion that always fails, indicating this function is either not
	   implemented or not meant to be used in the current build. The function signature suggests it's part of a texture compression utility class.

		\param inBuf Pointer to the input buffer containing normal map data
		\param outBuf Pointer to the output buffer where compressed DXT1 data will be stored
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
		\throws Assertion failure when the function is called
	*/
	void CompressNormalMapDXT1Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Compresses a normal map into DXT5 format with high quality, using tangent space normal data.

		This function takes input normal map data and compresses it into DXT5 format. It processes the input in 4x4 pixel blocks and performs high quality compression, including handling of alpha
	   values and color indices. The function accounts for hardware-specific fixes for NVIDIA 7x series GPUs. It extracts and encodes block data with optimized error handling for alpha values to
	   determine the best encoding method.

		\param inBuf Pointer to the input buffer containing the normal map data in RGBA format.
		\param outBuf Pointer to the output buffer where the compressed DXT5 data will be written.
		\param width Width in pixels of the input normal map.
		\param height Height in pixels of the input normal map.
	*/
	void CompressNormalMapDXT5HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a normal map into DXT5 format with high-quality renormalization.

		This function compresses a normal map represented as a byte buffer into the DXT5 compression format. It handles images of any size, but will only process images where both width and height are
	   multiples of 4, or less than 4 pixels in either dimension. For smaller images, a special tiny color compression method is used. For larger images, it processes the image in 4x4 pixel blocks.
	   The function extracts blocks from the input buffer, performs component swizzling, and computes min/max normals for DXT5 compression. It accounts for padding in both source and destination
	   buffers and updates progress during compression.

		\param inBuf Input buffer containing the normal map data
		\param outBuf Output buffer where the compressed DXT5 data will be written
		\param width Width of the input normal map in pixels
		\param height Height of the input normal map in pixels
	*/
	void CompressNormalMapDXT5RenormalizeHQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a tangent space normal map into DXT5 format using fast methods suitable for real-time applications.

		This function performs fast compression of normal map data into DXT5 format, designed for real-time use cases. It automatically selects between SSE2 optimized and generic implementations based
	   on available hardware instructions. The input is expected to be in tangent space normal map format with _Ny_Nx components, and the output is a compressed DXT5 block suitable for texture
	   streaming.

		\param inBuf Pointer to the input normal map data in tangent space format
		\param outBuf Pointer to the output buffer where compressed DXT5 data will be written
		\param width Width of the input normal map in pixels
		\param height Height of the input normal map in pixels
	*/
	void CompressNormalMapDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a normal map into DXT5 format using a fast generic algorithm

		This function performs DXT5 compression on a normal map image buffer. It processes the input image in 4x4 pixel blocks and compresses each block according to the DXT5 format specification. The
	   function extracts normal information from the input buffer and writes compressed data to the output buffer. The input image dimensions must be multiples of 4 in both width and height. The
	   compression uses a fast algorithm that prioritizes performance over maximum quality.

		\param inBuf input image buffer containing normal map data
		\param outBuf output buffer for the compressed DXT5 data
		\param width width of the input image in pixels
		\param height height of the input image in pixels
		\throws assertion failure if width or height is less than 4 or not a multiple of 4
	*/
	void CompressNormalMapDXT5Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a normal map into DXT5 format using SSE2 optimizations for faster processing.

		This function takes a normal map image buffer and compresses it into DXT5 format. It processes the input buffer in 4x4 pixel blocks, using SSE2 instructions for optimized performance. The
	   function assumes the input dimensions are multiples of 4. It uses a combination of minimum and maximum bounding box calculations, along with color indexing, to determine the optimal compression
	   for each block. The function also includes a test mode that compares the SSE2 output with a generic implementation for verification.

		\param inBuf Input buffer containing the normal map data in _y_x component order
		\param outBuf Output buffer where the compressed DXT5 data will be written
		\param width Width of the input normal map, must be a multiple of 4
		\param height Height of the input normal map, must be a multiple of 4
		\throws assertions for invalid input dimensions
	*/
	void CompressNormalMapDXT5Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a tangent space normal map into DXN2 (3Dc, ATI2N) format with high quality.

		This function performs high quality compression of a tangent space normal map into the DXN2 format, which is a 3Dc or ATI2N format. It processes the input buffer in 4x4 pixel blocks,
	   extracting and encoding alpha components for each block. The function handles edge cases where the texture dimensions are not multiples of four by returning early. For small textures (less than
	   4x4 pixels), it uses a specialized function for tiny color compression. The compression process involves finding minimum error alpha indices for two encoding methods and selecting the method
	   with lower error.

		\param inBuf Input buffer containing the normal map data
		\param outBuf Output buffer to store the compressed DXN2 data
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void CompressNormalMapDXN2HQ( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a normal map into DXN2 format for real-time use.

		This function performs fast compression of a normal map into the DXN2 format, which is suitable for real-time rendering applications. It utilizes a generic implementation for the actual
	   compression process.

		\param inBuf Input buffer containing the normal map data
		\param outBuf Output buffer where the compressed DXN2 data will be stored
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void CompressNormalMapDXN2Fast( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Compresses a normal map using the DXN2 format with fast generic algorithm

		This function performs compression of a normal map stored in the input buffer into the DXN2 format. The compression uses a fast generic algorithm that processes the input data in 4x4 pixel
	   blocks. The function extracts blocks from the input buffer, determines minimum and maximum bounding boxes for normals, and then encodes the results into the output buffer with specific handling
	   of the X and Y components as alpha channels. The function asserts that the width and height are both multiples of 4 and greater than or equal to 4.

		\param inBuf input buffer containing the normal map data to be compressed
		\param outBuf output buffer where the compressed DXN2 data will be stored
		\param width width of the input normal map in pixels
		\param height height of the input normal map in pixels
		\throws assertion failure if width or height is less than 4 or not divisible by 4
	*/
	void CompressNormalMapDXN2Fast_Generic( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief This function is a placeholder that currently does nothing but assert.

		The function is intended to compress a normal map using the DXN2 format with SSE2 optimization, but it is currently unimplemented and only contains an assertion that always fails.

		\param inBuf Input buffer containing the uncompressed normal map data
		\param outBuf Output buffer where the compressed DXN2 normal map will be stored
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
		\throws Assertion failure when called
	*/
	void CompressNormalMapDXN2Fast_SSE2( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Converts a single channel image from DXN1 format to DXT1 format

		This function performs a fast conversion of single channel image data from DXN1 format (also known as DXT5A or ATI1N) to DXT1 format. The conversion is optimized for performance and can work
	   in-place. The input buffer contains DXT5 alpha channel data that is decoded and then re-encoded into DXT1 format. The function processes the image in 4x4 pixel blocks and handles proper
	   alignment and padding for the output data.

		\param inBuf Input buffer containing DXN1 format data
		\param outBuf Output buffer for DXT1 format data
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws assertion failure when image dimensions are less than 4 pixels in either dimension
	*/
	void ConvertImageDXN1_DXT1( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Converts an image from DXT1 format to DXN1 format with reasonable performance.

		This function performs a fast single channel conversion from DXT1 to DXN1 format. The conversion is reasonably fast and supports in-place operations. The function is currently unimplemented
	   and will assert on execution.

		\param inBuf Input buffer containing the DXT1 formatted image data
		\param outBuf Output buffer where the DXN1 formatted image data will be stored
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws assert( 0 ) when called, indicating the function is not implemented
	*/
	void ConvertImageDXT1_DXN1( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Converts a DXN normal map to DXT5 format, processing 4x4 pixel blocks

		This function performs conversion of normal maps from DXN format (also known as 3Dc or ATI2N) to DXT5 format. The conversion processes the input data in 4x4 pixel blocks. The function handles
	   the decoding of Y component stored as DXT5 alpha channel and encodes the normal X component directly. It also computes the minimum and maximum Y values for each block to properly encode the
	   normal data into DXT5 color channels. The function requires both width and height to be multiples of 4, and will assert if this condition is not met. Padding is handled internally by the
	   function for proper block alignment.

		\param inBuf Input buffer containing the DXN normal map data
		\param outBuf Output buffer where the DXT5 normal map will be written
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
		\throws assertion failure if width or height is less than 4
	*/
	void ConvertNormalMapDXN2_DXT5( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Converts a DXT5 normal map to DXN format with fast tangent space NxNyNz conversion.

		This function performs a conversion from DXT5 normal map format to DXN (3Dc, ATI2N) format. It processes the input buffer containing DXT5 compressed normal data and outputs the corresponding
	   DXN compressed data. The conversion involves decoding the Y component from the DXT5 alpha channel and encoding it into the DXN green channel. The function handles block-aligned dimensions and
	   performs in-place processing when possible.

		\param inBuf Input buffer containing DXT5 compressed normal map data
		\param outBuf Output buffer for DXN compressed normal map data
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
		\throws assertion failure when width or height is less than 4 pixels
	*/
	void ConvertNormalMapDXT5_DXN2( const byte* inBuf, byte* outBuf, int width, int height );

private:
	int					  width;
	int					  height;
	byte*				  outData;
	int					  srcPadding;
	int					  dstPadding;

	//! Writes a single byte to the output data stream
	void				  EmitByte( byte b );

	//! Writes a 16-bit unsigned integer value to the output data buffer and advances the buffer pointer.
	void				  EmitUShort( unsigned short s );

	//! Writes a 32-bit unsigned integer value to the output data stream
	void				  EmitUInt( unsigned int i );

	//! Computes the squared difference between two alpha values.
	unsigned int		  AlphaDistance( const byte a1, const byte a2 ) const;

	//! Computes the squared Euclidean distance between two RGB color values.
	unsigned int		  ColorDistance( const byte* c1, const byte* c2 ) const;

	//! Computes a weighted color distance between two RGB color values.
	unsigned int		  ColorDistanceWeighted( const byte* c1, const byte* c2 ) const;

	//! Computes the squared Euclidean distance between two color vectors in a 2D color space.
	unsigned int		  CTX1Distance( const byte* c1, const byte* c2 ) const;

	//! Converts a 3-byte color to a 16-bit 565 format.
	unsigned short		  ColorTo565( const byte* color ) const;

	//! Converts RGB color components to a 16-bit 565 format.
	unsigned short		  ColorTo565( byte r, byte g, byte b ) const;

	//! Converts a 16-bit 565-format color value to an RGB color array.
	void				  ColorFrom565( unsigned short c565, byte* color ) const;

	//! Extracts the green component from a 16-bit 565 format color value
	byte				  GreenFrom565( unsigned short c565 ) const;

	//! Fixes a hardware bug in NV4X graphics drivers related to color encoding.
	void				  NV4XHardwareBugFix( byte* minColor, byte* maxColor ) const;

	/*!
		\brief Checks if the specified channel in the input buffer has a constant value across each 4x4 block.

		This function evaluates whether the pixel data in the provided buffer maintains a constant value for a specific color channel within each 4x4 block. If the dimensions are less than 4x4, it
	   checks the entire buffer for a constant value. For larger buffers, it iterates through each 4x4 block and verifies that all pixels within that block have the same value for the specified
	   channel.

		\param inBuf Pointer to the input buffer containing pixel data
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\param channel Index of the color channel to check (0-3 for RGBA)
		\return True if the specified channel has a constant value across each 4x4 block, false otherwise.
	*/
	bool				  HasConstantValuePer4x4Block( const byte* inBuf, int width, int height, int channel ) const;

	//! Encodes pixel data into DXT1 format using a simplified method that uses the color at position 0,0 for all blocks
	void				  WriteTinyColorDXT1( const byte* inBuf, int width, int height );

	//! Writes a DXT5 encoded texture block with a single color and dummy alpha values
	void				  WriteTinyColorDXT5( const byte* inBuf, int width, int height );

	//! Encodes a texture into DXT5A format with a simplified color approach.
	void				  WriteTinyColorCTX1DXT5A( const byte* inBuf, int width, int height );

	//! Writes a DXT5-encoded tiny normal map from input normal data.
	void				  WriteTinyNormalMapDXT5( const byte* NxNy, int width, int height );

	//! Writes a DXN format tiny normal map from input normal data
	void				  WriteTinyNormalMapDXN( const byte* NxNy, int width, int height );

	//! Writes a TinyDXT5A encoded block for normal data
	void				  WriteTinyDXT5A( const byte* NxNy, int width, int height );

	//! Finds the minimum and maximum colors in a 4x4 color block based on maximum distance criteria.
	void				  GetMinMaxColorsMaxDist( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Finds the minimum and maximum colors in a 4x4 color block based on luminance and alpha values
	void				  GetMinMaxColorsLuminance( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	/*!
		\brief Calculates the squared alpha error for a color block using specified alpha range and previous error threshold.

		This function computes the total squared error between the actual alpha values in a 4x4 color block and the optimal alpha values derived from the provided minimum and maximum alpha. It uses a
	   predefined set of 8 alpha values generated from the min and max alpha range. The function stops early if the error exceeds the provided lastError threshold for optimization purposes.

		\param colorBlock Pointer to the 16-pixel color block data with alpha values
		\param alphaOffset Offset into the color block where alpha values begin
		\param minAlpha Minimum alpha value to be considered for error calculation
		\param maxAlpha Maximum alpha value to be considered for error calculation
		\param lastError Previous error threshold for early termination optimization
		\return The calculated squared alpha error for the given color block and alpha range
	*/
	int					  GetSquareAlphaError( const byte* colorBlock, const int alphaOffset, const byte minAlpha, const byte maxAlpha, int lastError ) const;

	/*!
		\brief Computes the optimal minimum and maximum alpha values for a 4x4 color block to minimize quantization error

		This function analyzes a 4x4 pixel color block where each pixel is represented by 4 bytes and finds the best minimum and maximum alpha values that minimize the squared error in alpha
	   quantization. It first determines the initial range of alpha values in the block, then expands this range by a fixed amount to allow for better quantization. The function then performs an
	   exhaustive search over possible min/max pairs within this expanded range to find the combination that yields the lowest error. The alpha values are extracted from the color block starting at a
	   specified offset, and the resulting min/max values are written to the provided minColor and maxColor buffers. The function returns the best error value found during the search.

		\param colorBlock Pointer to a 4x4 pixel color block with 4 bytes per pixel
		\param alphaOffset Byte offset within each pixel where alpha values begin
		\param minColor Output buffer to store the resulting minimum color values
		\param maxColor Output buffer to store the resulting maximum color values
		\return The minimum squared error found during the optimization process
	*/
	int					  GetMinMaxAlphaHQ( const byte* colorBlock, const int alphaOffset, byte* minColor, byte* maxColor ) const;

	/*!
		\brief Computes the error for a DXT color block using the specified color endpoints and returns the accumulated error.

		This function calculates the color error for a 4x4 pixel block by interpolating colors between two given endpoints. It generates four colors from the two endpoints, then computes the minimum
	   color distance for each pixel in the block and sums these distances to determine the total error. Early termination occurs if the error exceeds the provided lastError threshold.

		\param colorBlock Pointer to the 16 pixel block for which to compute color error
		\param color0 First color endpoint in 565 format
		\param color1 Second color endpoint in 565 format
		\param lastError Maximum allowed error before early termination
		\return The computed total error for the color block based on the given color endpoints
	*/
	int					  GetSquareColorsError( const byte* colorBlock, const unsigned short color0, const unsigned short color1, int lastError ) const;

	/*!
		\brief Computes the optimal minimum and maximum colors for DXT compression of a 4x4 color block with high quality algorithm

		This function determines the best fitting minimum and maximum colors in 565 format for DXT compression by evaluating all possible combinations within a constrained bounding box. It considers
	   the color distribution in the input block and applies specific rules to ensure proper color encoding. The algorithm iterates through all valid 565 color combinations within the expanded
	   bounding box, calculating error metrics to find the optimal pair that minimizes color distortion. The function handles the special case where black colors are excluded when noBlack is true.

		\param colorBlock Pointer to 16-color 4-byte RGB values representing a 4x4 color block
		\param minColor Output pointer to store the 3-byte minimum color in RGB format
		\param maxColor Output pointer to store the 3-byte maximum color in RGB format
		\param noBlack Flag indicating whether black color combinations should be avoided
		\return The minimum error value found during the optimization process, representing the quality of the color approximation
	*/
	int					  GetMinMaxColorsHQ( const byte* colorBlock, byte* minColor, byte* maxColor, bool noBlack ) const;

	/*!
		\brief Computes the error for a square block using CTX1 color interpolation with two given colors and a last error threshold.

		This function calculates the total error between a given color block and the colors generated from two input colors using CTX1 interpolation. It iterates through each pixel in the block, finds
	   the closest color from the four interpolated colors, and accumulates the distance. The function early exits if the accumulated error exceeds the provided last error threshold for optimization.

		\param colorBlock 16 pixel block for which to compute the error
		\param color0 minimum color for interpolation
		\param color1 maximum color for interpolation
		\param lastError threshold error value for early termination
		\return The accumulated error between the color block and the interpolated colors
	*/
	int					  GetSquareCTX1Error( const byte* colorBlock, const byte* color0, const byte* color1, int lastError ) const;

	//! Computes the optimal minimum and maximum colors for a DXT1 texture compression block.
	int					  GetMinMaxCTX1HQ( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	/*!
		\brief Computes the accumulated squared error for the Y (green) channel of a color block when approximating it with two given colors.

		This function calculates the error between the actual green channel values of a 4x4 pixel color block and the green channel values of colors interpolated from two specified 16-bit color
	   values. It uses a weighted average to determine the four possible color values from the two input colors. The error is accumulated across all 16 pixels and returned. If the accumulated error
	   exceeds a given threshold, the function returns early to optimize performance.

		\param colorBlock Pointer to the 16 pixel block for which to compute the Y channel error
		\param color0 The first 16-bit color value (min color) used to generate interpolated colors
		\param color1 The second 16-bit color value (max color) used to generate interpolated colors
		\param lastError Threshold error value to possibly terminate early
		\param scale Scaling factor used to normalize color values for comparison
		\return The accumulated squared error for the Y (green) channel of the color block.
	*/
	int					  GetSquareNormalYError( const byte* colorBlock, const unsigned short color0, const unsigned short color1, int lastError, int scale ) const;

	/*!
		\brief Computes the minimum and maximum Y components of normals for a 4x4 color block using 565 color encoding with high quality estimation

		This function analyzes a 4x4 color block to determine the optimal minimum and maximum Y color values for normal encoding using 565 format. It calculates the bounding box of the Y components,
	   expands it for encoding, and performs exhaustive search to find the best color representation. The function handles special cases for black color exclusion and applies scaling for error
	   calculation. The results are written to minColor and maxColor output parameters, with the R and B components pre-filled from the first pixel of the input block.

		\param colorBlock Input 4x4 color block with 4 bytes per pixel
		\param minColor Output minimum color values
		\param maxColor Output maximum color values
		\param noBlack Flag to indicate exclusion of black color
		\param scale Scaling factor for error calculation
		\return The best error value found during the optimization process
	*/
	int					  GetMinMaxNormalYHQ( const byte* colorBlock, byte* minColor, byte* maxColor, bool noBlack, int scale ) const;

	/*!
		\brief Computes the DXT1 error for a color block using two specified colors and returns the optimal color indices.

		This function takes a 4x4 color block and two colors (color0 and color1) to determine the optimal color indices for DXT1 compression. It calculates interpolated colors based on the two input
	   colors and assigns each pixel in the block to the closest interpolated color. The error is computed as the sum of squared differences between the original and compressed colors. If the
	   accumulated error exceeds the lastError, the function terminates early to optimize performance. The function is designed for use in texture compression algorithms where minimizing visual error
	   is important.

		\param colorBlock Pointer to the 4x4 input tile containing 16 pixels, with 4 bytes per pixel representing RGB values
		\param color0 The minimum color value in 565 format used for generating interpolated colors
		\param color1 The maximum color value in 565 format used for generating interpolated colors
		\param lastError The maximum allowed error threshold; if the computed error exceeds this value, computation may be terminated early
		\param colorIndices Reference to an unsigned integer that will store the computed 4-bit color indices for each of the 16 pixels
		\return The accumulated error value representing the sum of squared differences between original and compressed colors
	*/
	int					  GetSquareNormalsDXT1Error( const int* colorBlock, const unsigned short color0, const unsigned short color1, int lastError, unsigned int& colorIndices ) const;

	/*!
		\brief Finds the optimal minimum and maximum colors for a DXT1 texture compression block with high quality normal encoding.

		This function analyzes a 4x4 block of pixel data to determine the best fitting minimum and maximum colors in 565 RGB format for DXT1 compression. It considers the color distribution within the
	   block and computes the optimal color pair that minimizes the compression error. The function also handles special cases like excluding black colors when noBlack is true. The color indices are
	   stored in the colorIndices parameter which indicate how each pixel in the block is represented using the computed colors.

		\param colorBlock Pointer to a 4x4 block of pixel data with 4 bytes per pixel (RGBA)
		\param minColor Output pointer to store the 3-byte minimum color in 565 format
		\param maxColor Output pointer to store the 3-byte maximum color in 565 format
		\param colorIndices Output reference to store the indices indicating pixel representation
		\param noBlack Boolean flag to indicate whether black colors should be excluded from consideration
		\return The minimum error value achieved during the color optimization process
	*/
	int					  GetMinMaxNormalsDXT1HQ( const byte* colorBlock, byte* minColor, byte* maxColor, unsigned int& colorIndices, bool noBlack ) const;

	/*!
		\brief Computes the error for a DXT5-encoded normal block using the provided min and max normals, and returns the optimal color and alpha indices.

		This function takes a 16-pixel normal block and finds the optimal color and alpha indices for DXT5 compression. It calculates the DXT5 color and alpha lookup tables from the provided min and
	   max normals, then iterates through each pixel to determine the closest matching color and alpha values. The function stops early if the accumulated error exceeds the last error threshold for
	   performance optimization. The resulting color and alpha indices are stored in the provided output parameters.

		\param normalBlock 16 pixel block for which to find normal indexes
		\param minNormal Min normal found
		\param maxNormal Max normal found
		\param lastError The maximum error threshold for early termination
		\param colorIndices Output parameter for the color indices
		\param alphaIndices Output parameter for the alpha indices
		\return The computed error value for the DXT5-encoded normal block
	*/
	int					  GetSquareNormalsDXT5Error( const int* normalBlock, const byte* minNormal, const byte* maxNormal, int lastError, unsigned int& colorIndices, byte* alphaIndices ) const;

	/*!
		\brief Computes the optimal minimum and maximum colors for a DXT5 HDR normal block with high quality, returning the quantization error.

		This function determines the best fitting minimum and maximum colors for a 4x4 block of normalized vectors stored in DXT5 format. It analyzes the input normal block data to find the optimal
	   color range that minimizes the quantization error when encoding the normals. The function also calculates the corresponding color and alpha indices for the block.

		The algorithm iterates through possible color ranges within the bounding box of the input data, expanding it by fixed amounts for each component to ensure proper encoding. For each candidate
	   range, it computes the error using a helper function and keeps track of the best result. The input normal block is expected to contain 16 pixels, each with four components (RGBA) with values
	   ranging from 0 to 255.

		\param normalBlock Pointer to a 4x4 block of input normal data, with 4 bytes per pixel (RGBA)
		\param minColor Pointer to output buffer where the minimum color will be stored as a 4-byte value
		\param maxColor Pointer to output buffer where the maximum color will be stored as a 4-byte value
		\param colorIndices Reference to an unsigned integer where the color indices for the block will be stored
		\param alphaIndices Pointer to output buffer where the alpha indices will be stored as 6 bytes
		\return The quantization error of the best fitting color range, represented as an integer value
	*/
	int					  GetMinMaxNormalsDXT5HQ( const byte* normalBlock, byte* minColor, byte* maxColor, unsigned int& colorIndices, byte* alphaIndices ) const;

	/*!
		\brief Computes the optimal minimum and maximum colors for a DXT5 normal map block with high quality fast algorithm.

		This function analyzes a 4x4 block of normal map data to determine the best fitting minimum and maximum colors for DXT5 compression. It uses a fast algorithm to evaluate color combinations and
	   finds the encoding that minimizes error. The function also computes the alpha indices needed for the DXT5 compression scheme. The normal block is expected to contain 16 pixels, each with 4
	   bytes representing RGBA components. The algorithm first computes a bounding box for the color data, then iterates through possible color combinations to find the optimal encoding.

		\param normalBlock 4x4 input tile of normal map data, 4 bytes per pixel
		\param minColor Output pointer to store the 4-byte minimum color
		\param maxColor Output pointer to store the 4-byte maximum color
		\param colorIndices Reference to store the color indices for DXT1 encoding
		\param alphaIndices Output pointer to store the 6-byte alpha indices for DXT5 encoding
		\return The minimum error found during the optimization process
	*/
	int					  GetMinMaxNormalsDXT5HQFast( const byte* normalBlock, byte* minColor, byte* maxColor, unsigned int& colorIndices, byte* alphaIndices ) const;

	//! Scales color values in a 16-pixel block using YCoCg color space transformation.
	void				  ScaleYCoCg( byte* colorBlock ) const;

	/*!
		\brief Extracts a 4x4 block from a source image with padding for dimensions smaller than 4

		This function extracts a 4x4 pixel block from a source image located at coordinates x, y with specified width and height. The function handles cases where the requested block extends beyond
	   the source image boundaries by padding with appropriate values. The source image is assumed to be in RGBA format with 4 bytes per pixel. The block data is stored in a flattened 4x4x4 byte array
	   with each pixel represented as 4 consecutive bytes.

		\param src Pointer to the source image data in RGBA format
		\param x X coordinate of the top-left corner of the block to extract
		\param y Y coordinate of the top-left corner of the block to extract
		\param w Width of the source image
		\param h Height of the source image
		\param block Output buffer for the 4x4 extracted block, 64 bytes for 16 pixels in RGBA format
	*/
	void				  ExtractBlockGimpDDS( const byte* src, int x, int y, int w, int h, byte* block );

	//! Encodes an alpha block in DXT5 format for GIMP DDS files
	void				  EncodeAlphaBlockBC3GimpDDS( byte* dst, const byte* block, const int offset );

	//! Finds the minimum and maximum values for the green and blue channels in a 4x4 pixel block.
	void				  GetMinMaxYCoCgGimpDDS( const byte* block, byte* mincolor, byte* maxcolor );

	//! Scales YCoCg color values in a DDS block using specified min and max color parameters
	void				  ScaleYCoCgGimpDDS( byte* block, byte* mincolor, byte* maxcolor );

	//! Adjusts the color bounding box for YCoCgGimp DDS encoding by applying inset operations to the green and blue color components.
	void				  InsetBBoxYCoCgGimpDDS( byte* mincolor, byte* maxcolor );

	//! Selects diagonal color components for YCoCg GIMP DDS encoding
	void				  SelectDiagonalYCoCgGimpDDS( const byte* block, byte* mincolor, byte* maxcolor );

	//! Performs linear interpolation at a 1/3 point between two RGB color values.
	void				  LerpRGB13GimpDDS( byte* dst, byte* a, byte* b );

	//! Multiplies two 8-bit values with GIMP-style rounding and returns the result.
	inline int			  Mul8BitGimpDDS( int a, int b );

	//! Packs BGR8 color components into a RGB565 format unsigned short value.
	inline unsigned short PackRGB565GimpDDS( const byte* c );

	//! Encodes a YCoCg color block using Gimp DDS format into the destination buffer.
	void				  EncodeYCoCgBlockGimpDDS( byte* dst, byte* block );

	//! Adjusts the bias and scale of the Y component in a color block for normal mapping
	void				  BiasScaleNormalY( byte* colorBlock ) const;

	//! Rotates normal vectors in a DXT1 block to optimize color compression.
	void				  RotateNormalsDXT1( byte* block ) const;

	//! Rotates normal vectors in a DXT5 block to minimize error
	void				  RotateNormalsDXT5( byte* block ) const;

	/*!
		\brief Finds the optimal color indices for a 4x4 pixel block using two given colors and returns the accumulated error.

		This function determines the best color indices for each pixel in a 4x4 color block based on two reference colors. It calculates interpolated colors for a 4-color palette and assigns each
	   pixel in the block to the nearest color. The function returns the total error calculated as the sum of squared differences between original and interpolated colors. The result is stored as a
	   32-bit integer with each pixel's index encoded in 2 bits.

		\param colorBlock Pointer to the 16 pixel block where each pixel is represented by 4 bytes (RGBA)
		\param color0 First reference color in 565 format
		\param color1 Second reference color in 565 format
		\param result Output parameter that receives the encoded color indices as a 32-bit integer
		\return The accumulated error calculated as the sum of squared differences between original and interpolated colors for all pixels in the block.
	*/
	int					  FindColorIndices( const byte* colorBlock, const unsigned short color0, const unsigned short color1, unsigned int& result ) const;

	/*!
		\brief Finds the optimal alpha indices for a DXT5 compressed texture block by minimizing reconstruction error.

		This function takes a 4x4 pixel color block and determines the best 6-byte alpha index encoding that minimizes the error between the original alpha values and the reconstructed values using
	   the provided alpha range. It uses a weighted average of the two given alpha values to generate 8 possible alpha values for reconstruction, then assigns each pixel's alpha value to the closest
	   reconstructed value. The function returns the total error metric for the assignment, which helps in selecting the optimal alpha range for DXT5 compression.

		\param colorBlock Pointer to the 16 pixel block data in RGBA format
		\param alphaOffset Byte offset into the color block where alpha data is located (typically 3 for RGBA)
		\param alpha0 Minimum alpha value in the block to be used for generating alpha lookup table
		\param alpha1 Maximum alpha value in the block to be used for generating alpha lookup table
		\param indexes Output buffer for the 6-byte alpha index block that encodes the alpha indices for all 16 pixels
		\return The total error metric representing how well the reconstructed alpha values match the original alpha values
	*/
	int					  FindAlphaIndices( const byte* colorBlock, const int alphaOffset, const byte alpha0, const byte alpha1, byte* indexes ) const;

	/*!
		\brief Finds the optimal CTX1 color indices for a 4x4 pixel color block based on two anchor colors

		This function determines the best color indices from a set of four colors that approximate the colors in a 4x4 pixel block. The four colors are derived from two anchor colors provided as
	   input, with the first color being the maximum color and the second color being the minimum color. The function calculates interpolated colors to form a palette of four colors, then assigns each
	   pixel in the block to the closest color in the palette. The accumulated error of this approximation is returned as the function result, while the indices are stored in the result parameter.

		\param colorBlock Pointer to the 16 pixel block for which color indexes are to be found
		\param color0 Maximum color found in the block
		\param color1 Minimum color found in the block
		\param result Reference to store the resulting 4 byte color index block
		\return The accumulated error of the color approximation for the entire block
	*/
	int					  FindCTX1Indices( const byte* colorBlock, const byte* color0, const byte* color1, unsigned int& result ) const;

	//! Extracts a 4x4 color block from the input image pointer.
	void				  ExtractBlock( const byte* inPtr, int width, byte* colorBlock ) const;

	//! Computes the minimum and maximum color values for each channel within a 4x4 color block.
	void				  GetMinMaxBBox( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Adjusts the minimum and maximum color values by insetting them inward based on color differences.
	void				  InsetColorsBBox( byte* minColor, byte* maxColor ) const;

	//! Selects color values for a DXT compression diagonal based on a color block
	void				  SelectColorsDiagonal( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Scales YCoCg color values in a color block based on min and max color coordinates
	void				  ScaleYCoCg( byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Adjusts the color bounding box by insetting the color and alpha values.
	void				  InsetYCoCgAlpaBBox( byte* minColor, byte* maxColor ) const;

	//! Adjusts the minimum and maximum color values for YCoCg color space bounding box encoding.
	void				  InsetYCoCgBBox( byte* minColor, byte* maxColor ) const;

	//! Selects the YCoCg diagonal for color block encoding.
	void				  SelectYCoCgDiagonal( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Adjusts the normal bounding box for DXT5 encoding by insetting the alpha and color values.
	void				  InsetNormalsBBoxDXT5( byte* minNormal, byte* maxNormal ) const;

	//! Adjusts the normal bounding box by insetting its dimensions based on alpha shift parameters
	void				  InsetNormalsBBox3Dc( byte* minNormal, byte* maxNormal ) const;

	//! Encodes color indices for a 4x4 pixel block using min and max color values
	void				  EmitColorIndices( const byte* colorBlock, const byte* minColor, const byte* maxColor );

	//! Encodes color and alpha indices for a 4x4 pixel block using min and max color values
	void				  EmitColorAlphaIndices( const byte* colorBlock, const byte* minColor, const byte* maxColor );

	//! EmitCTX1Indices computes and emits color indices for a 4x4 pixel block using min and max color values.
	void				  EmitCTX1Indices( const byte* colorBlock, const byte* minColor, const byte* maxColor );

	/*!
		\brief Encodes alpha indices for a 4x4 pixel block using the specified min and max alpha values.

		This function takes a 4x4 pixel color block and determines the optimal alpha index for each pixel based on the provided minimum and maximum alpha values. It uses a specific algorithm to map
	   the alpha values to 3-bit indices which are then emitted as bytes. The algorithm computes intermediate alpha values and compares each pixel's alpha against these values to determine the index.
	   The results are packed into 6 bytes for transmission.

		\param colorBlock Pointer to the 16 pixel block for which alpha indexes need to be calculated
		\param channel The channel offset to use when reading alpha values from the color block
		\param minAlpha Minimum alpha value found in the block
		\param maxAlpha Maximum alpha value found in the block
		\throws assertion failure if maxAlpha is less than minAlpha
	*/
	void				  EmitAlphaIndices( const byte* colorBlock, const int channel, const byte minAlpha, const byte maxAlpha );

	/*!
		\brief Encodes and emits green color indices for a DXT block based on the provided min and max green values

		This function processes a 4x4 pixel block to determine the green color indices for each pixel. It uses a weighted averaging approach to compute three threshold values that are used to classify
	   pixel green values into four possible indices. The function employs bit manipulation to efficiently pack these indices into a 32-bit result which is then emitted. The implementation includes
	   two code paths, with the primary path using a specific mathematical formula for threshold calculation, while the alternative path uses a brute force nearest neighbor approach.

		\param block Pointer to the block data containing pixel information
		\param channel Offset into the block data to start processing
		\param minGreen Minimum green value found in the block
		\param maxGreen Maximum green value found in the block
		\throws assertion failure if maxGreen is less than minGreen
	*/
	void				  EmitGreenIndices( const byte* block, const int channel, const byte minGreen, const byte maxGreen );

	//! Extracts a 4x4 block of pixel data using SSE2 intrinsics.
	void				  ExtractBlock_SSE2( const byte* inPtr, int width, byte* colorBlock ) const;

	//! Computes the minimum and maximum colors from a 4x4 DXT color block using SSE2 instructions
	void				  GetMinMaxBBox_SSE2( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Adjusts the minimum and maximum color values for a bounding box using SSE2 instructions.
	void				  InsetColorsBBox_SSE2( byte* minColor, byte* maxColor ) const;

	//! Adjusts normal bounding box values for DXT5 compression using SSE2 instructions.
	void				  InsetNormalsBBoxDXT5_SSE2( byte* minNormal, byte* maxNormal ) const;

	//! Computes and emits color indices for a DXT color block using SSE2 optimizations.
	void				  EmitColorIndices_SSE2( const byte* colorBlock, const byte* minColor, const byte* maxColor );

	//! Encodes color indices for a DXT texture block using SSE2 instructions.
	void				  EmitColorAlphaIndices_SSE2( const byte* colorBlock, const byte* minColor, const byte* maxColor );

	//! Computes and emits color indices for a DXT color block using SSE2 instructions.
	void				  EmitCoCgIndices_SSE2( const byte* colorBlock, const byte* minColor, const byte* maxColor );

	//! Encodes alpha indices for a DXT alpha block using SSE2 instructions
	void				  EmitAlphaIndices_SSE2( const byte* colorBlock, const int minAlpha, const int maxAlpha );

	/*!
		\brief Encodes alpha indices for a DXTC texture block using SSE2 instructions.

		This function processes a color block to extract and encode alpha indices using SSE2 SIMD instructions. It shifts the input data by a specified bit offset, masks the results to extract byte
	   values, and then performs a series of arithmetic and comparison operations to determine the appropriate alpha indices. The function computes weighted averages to determine the alpha values,
	   compares them with the input data, and finally packs the results into a 32-bit output value that represents the encoded alpha indices.

		\param colorBlock Pointer to the input color block data
		\param channelBitOffset Bit offset used to extract the alpha channel data
		\param minAlpha Minimum alpha value for the block
		\param maxAlpha Maximum alpha value for the block
	*/
	void				  EmitAlphaIndices_SSE2( const byte* colorBlock, const int channelBitOffset, const int minAlpha, const int maxAlpha );

	/*!
		\brief Encodes green color indices for a DXT block using SSE2 instructions based on the provided block data and color range.

		This function processes a 16-byte block of pixel data to extract and encode green color indices using SSE2 SIMD instructions. It takes the block data, applies bit shifting and masking to
	   extract the green channel values, then uses the provided minimum and maximum green values to calculate and emit the encoded indices. The function performs multiple steps including bit
	   manipulation, arithmetic operations, and vector comparisons to determine the appropriate color indices for compression.

		\param block Pointer to the 16-byte block data containing pixel information
		\param channelBitOffset Bit offset to apply when extracting the green channel values
		\param minGreen Minimum green value in the block
		\param maxGreen Maximum green value in the block
	*/
	void				  EmitGreenIndices_SSE2( const byte* block, const int channelBitOffset, const int minGreen, const int maxGreen );

	//! This function scales YCoCg color values using SSE2 instructions for a color block with specified minimum and maximum colors.
	void				  ScaleYCoCg_SSE2( byte* colorBlock, byte* minColor, byte* maxColor ) const;

	//! Computes and updates the minimum and maximum color values using SSE2 instructions for YCoCg color space bounding box adjustment.
	void				  InsetYCoCgBBox_SSE2( byte* minColor, byte* maxColor ) const;

	//! Selects the YCoCg diagonal for a color block using SSE2 instructions
	void				  SelectYCoCgDiagonal_SSE2( const byte* colorBlock, byte* minColor, byte* maxColor ) const;

	void				  EmitNormalYIndices( const byte* normalBlock, const int offset, const byte minNormalY, const byte maxNormalY );
	void				  EmitNormalYIndices_SSE2( const byte* normalBlock, const int offset, const byte minNormalY, const byte maxNormalY );

	//! Decodes DXN alpha values from a compressed input buffer into an array of byte values.
	void				  DecodeDXNAlphaValues( const byte* inBuf, byte* values );

	/*!
		\brief Encodes alpha values using DXN format with specified min and max values

		This function performs alpha value encoding using the DXN (DXT5) alpha compression format. It takes a set of 16 alpha values and compresses them into a 4-byte output buffer using two boundary
	   values and 8 weighted alpha values for quantization. The algorithm calculates the optimal index for each input alpha value by finding the minimum distance to the 8 weighted alpha values. The
	   final output consists of the min and max boundary values followed by 6 bits of index data for each of the 16 alpha values, arranged in a specific bit packing pattern.

		\param outBuf Output buffer where the encoded DXN alpha values will be stored
		\param min Minimum alpha value for the compression range
		\param max Maximum alpha value for the compression range
		\param values Array of 16 input alpha values to be encoded
	*/
	void				  EncodeDXNAlphaValues( byte* outBuf, const byte min, const byte max, const byte* values );

	/*!
		\brief Decodes Y-component normal values from a DXT compressed buffer using specified min and max values.

		This function takes a DXT compressed buffer and decodes the Y-component normal values for 16 texels. It extracts two initial normal values from a 16-bit word pair, computes two additional
	   interpolated values, and then uses bit indexing to determine which of the four Y-values to assign to each texel. The min and max values are set to the first and second computed Y-values
	   respectively, representing the range of normals in the compressed data.

		\param inBuf Input buffer containing DXT compressed normal data
		\param min Output parameter for the minimum Y-normal value
		\param max Output parameter for the maximum Y-normal value
		\param values Output buffer for the decoded Y-normal values
		\throws Assertion failure if the first normal value is less than the second normal value
	*/
	void				  DecodeNormalYValues( const byte* inBuf, byte& min, byte& max, byte* values );

	/*!
		\brief Encodes normal RGB indices for a 16-element normal block using min and max grayscale values

		This function takes a set of 16 grayscale values representing normal data and encodes them into a compact format using the provided minimum and maximum grayscale values. It calculates three
	   threshold values based on the min and max, then determines which of three quantization bins each input value falls into. The resulting indices are packed into a 32-bit value and combined with
	   the min/max values to produce an 8-byte output buffer. The function is designed for efficient storage of normal map data in DXT compression formats.

		\param outBuf Output buffer where the encoded result will be written (8 bytes)
		\param min Minimum grayscale value used for quantization
		\param max Maximum grayscale value used for quantization
		\param values Array of 16 grayscale values representing normal data to encode
	*/
	void				  EncodeNormalRGBIndices( byte* outBuf, const byte min, const byte max, const byte* values );
};

ID_INLINE void idDxtEncoder::CompressImageDXT1Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
#if defined( USE_INTRINSICS_SSE )
	CompressImageDXT1Fast_SSE2( inBuf, outBuf, width, height );
#else
	CompressImageDXT1Fast_Generic( inBuf, outBuf, width, height );
#endif
}

ID_INLINE void idDxtEncoder::CompressImageDXT1AlphaFast( const byte* inBuf, byte* outBuf, int width, int height )
{
#if defined( USE_INTRINSICS_SSE )
	CompressImageDXT1AlphaFast_SSE2( inBuf, outBuf, width, height );
#else
	CompressImageDXT1AlphaFast_Generic( inBuf, outBuf, width, height );
#endif
}

ID_INLINE void idDxtEncoder::CompressImageDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
#if defined( USE_INTRINSICS_SSE )
	CompressImageDXT5Fast_SSE2( inBuf, outBuf, width, height );
#else
	CompressImageDXT5Fast_Generic( inBuf, outBuf, width, height );
#endif
}

ID_INLINE void idDxtEncoder::CompressImageR11G11B10_BC6Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
#if( defined( USE_INTRINSICS_SSE ) || defined( USE_INTRINSICS_NEON ) )
	CompressImageR11G11B10_BC6Fast_SIMD( inBuf, outBuf, width, height );
#else
	CompressImageR11G11B10_BC6Fast_Generic( inBuf, outBuf, width, height );
#endif
}

ID_INLINE void idDxtEncoder::CompressImageDXN1Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
	CompressImageDXN1Fast_Generic( inBuf, outBuf, width, height );
}

ID_INLINE void idDxtEncoder::CompressYCoCgDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
#if defined( USE_INTRINSICS_SSE )
	CompressYCoCgDXT5Fast_SSE2( inBuf, outBuf, width, height );
#else
	CompressYCoCgDXT5Fast_Generic( inBuf, outBuf, width, height );
#endif
}

ID_INLINE void idDxtEncoder::CompressYCoCgCTX1DXT5AFast( const byte* inBuf, byte* outBuf, int width, int height )
{
	CompressYCoCgCTX1DXT5AFast_Generic( inBuf, outBuf, width, height );
}

ID_INLINE void idDxtEncoder::CompressNormalMapDXT5Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
#if defined( USE_INTRINSICS_SSE )
	CompressNormalMapDXT5Fast_SSE2( inBuf, outBuf, width, height );
#else
	CompressNormalMapDXT5Fast_Generic( inBuf, outBuf, width, height );
#endif
}

ID_INLINE void idDxtEncoder::CompressNormalMapDXN2Fast( const byte* inBuf, byte* outBuf, int width, int height )
{
	CompressNormalMapDXN2Fast_Generic( inBuf, outBuf, width, height );
}

ID_INLINE void idDxtEncoder::EmitByte( byte b )
{
	*outData = b;
	outData += 1;
}

ID_INLINE void idDxtEncoder::EmitUShort( unsigned short s )
{
	*( ( unsigned short* )outData ) = s;
	outData += 2;
}

ID_INLINE void idDxtEncoder::EmitUInt( unsigned int i )
{
	*( ( unsigned int* )outData ) = i;
	outData += 4;
}

ID_INLINE unsigned int idDxtEncoder::AlphaDistance( const byte a1, const byte a2 ) const
{
	return ( a1 - a2 ) * ( a1 - a2 );
}

ID_INLINE unsigned int idDxtEncoder::ColorDistance( const byte* c1, const byte* c2 ) const
{
	return ( ( c1[0] - c2[0] ) * ( c1[0] - c2[0] ) ) + ( ( c1[1] - c2[1] ) * ( c1[1] - c2[1] ) ) + ( ( c1[2] - c2[2] ) * ( c1[2] - c2[2] ) );
}

ID_INLINE unsigned int idDxtEncoder::ColorDistanceWeighted( const byte* c1, const byte* c2 ) const
{
	int r, g, b;
	int rmean;

	// http://www.compuphase.com/cmetric.htm
	rmean = ( ( int )c1[0] + ( int )c2[0] ) / 2;
	r	  = ( int )c1[0] - ( int )c2[0];
	g	  = ( int )c1[1] - ( int )c2[1];
	b	  = ( int )c1[2] - ( int )c2[2];
	return ( ( ( 512 + rmean ) * r * r ) >> 8 ) + 4 * g * g + ( ( ( 767 - rmean ) * b * b ) >> 8 );
}

ID_INLINE unsigned int idDxtEncoder::CTX1Distance( const byte* c1, const byte* c2 ) const
{
	return ( ( c1[0] - c2[0] ) * ( c1[0] - c2[0] ) ) + ( ( c1[1] - c2[1] ) * ( c1[1] - c2[1] ) );
}

ID_INLINE unsigned short idDxtEncoder::ColorTo565( const byte* color ) const
{
	return ( ( color[0] >> 3 ) << 11 ) | ( ( color[1] >> 2 ) << 5 ) | ( color[2] >> 3 );
}

ID_INLINE void idDxtEncoder::ColorFrom565( unsigned short c565, byte* color ) const
{
	color[0] = byte( ( ( c565 >> 8 ) & ( ( ( 1 << ( 8 - 3 ) ) - 1 ) << 3 ) ) | ( ( c565 >> 13 ) & ( ( 1 << 3 ) - 1 ) ) );
	color[1] = byte( ( ( c565 >> 3 ) & ( ( ( 1 << ( 8 - 2 ) ) - 1 ) << 2 ) ) | ( ( c565 >> 9 ) & ( ( 1 << 2 ) - 1 ) ) );
	color[2] = byte( ( ( c565 << 3 ) & ( ( ( 1 << ( 8 - 3 ) ) - 1 ) << 3 ) ) | ( ( c565 >> 2 ) & ( ( 1 << 3 ) - 1 ) ) );
}

/*
========================
idDxtEncoder::ColorTo565
========================
*/
ID_INLINE unsigned short idDxtEncoder::ColorTo565( byte r, byte g, byte b ) const
{
	return ( ( r >> 3 ) << 11 ) | ( ( g >> 2 ) << 5 ) | ( b >> 3 );
}

ID_INLINE byte idDxtEncoder::GreenFrom565( unsigned short c565 ) const
{
	byte c = byte( ( c565 & ( ( ( 1 << 6 ) - 1 ) << 5 ) ) >> 3 );
	return ( c | ( c >> 6 ) );
}

/*!
	\class idDxtDecoder
	\brief A utility class for decompressing DXT-compressed image and normal map data into raw pixel formats.

	This class provides functionality for decompressing various DXT-compressed image formats including DXT1, DXT5, and specialized formats for normal maps. It supports both standard and renormalized
   normal map decompression, as well as specialized formats like YCoCg and DXN2. The decompression functions handle different texture compression schemes and produce output in standard 4-byte RGBA
   format for regular images and tangent-space normal vectors for normal maps. The class also includes helper methods for reading and manipulating compressed data formats, including utilities for
   color and normal component conversions between different representations. All decompression operations process image data in 4x4 pixel blocks, with appropriate handling of dimensions and memory
   layout for the output buffers. The implementation covers multiple compression variants while maintaining consistency in output format and coordinate handling.

*/
class idDxtDecoder
{
public:
	/*!
		\brief Decompresses DXT1 format image data into raw pixel bytes.

		This function performs DXT1 decompression on image data, converting compressed blocks into uncompressed RGBA pixel data. The input buffer contains DXT1 compressed data, and the output buffer
	   receives the decompressed pixel values. Each 4x4 block of pixels is processed independently, with the decompression handling the color values specifically for DXT1 format without alpha channel
	   support. The function processes the image row by row and column by column, decoding each 4x4 block and placing the resulting pixels at the appropriate positions in the output buffer. The width
	   and height parameters define the dimensions of the image being decompressed.

		\param inBuf Pointer to the input buffer containing DXT1 compressed image data
		\param outBuf Pointer to the output buffer where decompressed pixel data will be written
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void DecompressImageDXT1( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses DXT5 compressed image data into raw pixel format.

		This function performs decompression of DXT5 formatted image data. It processes the input buffer in 4x4 pixel blocks, decoding both alpha and color information for each block. The decompressed
	   output is written to the specified output buffer with the provided width and height dimensions. The function handles the internal state management for width and height, and uses helper
	   functions to decode alpha and color values before emitting the final pixel data.

		\param inBuf Input buffer containing DXT5 compressed image data
		\param outBuf Output buffer where decompressed pixel data will be written
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void DecompressImageDXT5( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses DXT5 formatted image data with nVidia 7x hardware bug compatibility.

		This function performs DXT5 image decompression using a specific algorithm that accounts for a hardware bug present in nVidia 7x graphics chips. The decompression processes the input data in
	   4x4 pixel blocks, decoding both alpha and color information for each block. The output buffer must be large enough to hold the decompressed pixel data.

		\param inBuf Pointer to the input compressed DXT5 data
		\param outBuf Pointer to the output decompressed pixel data buffer
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void DecompressImageDXT5_nVidia7x( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief This function is intended to decompress images using the CTX1 format but currently has no implementation.

		The function is declared to decompress image data using the CTX1 compression format. It takes an input buffer containing compressed image data, an output buffer for the decompressed result,
	   and the dimensions of the image. The function uses assert to ensure it is not called, indicating that the implementation is incomplete or not yet supported in this version of the engine. The
	   CTX1 format is a specific compression method for texture data.

		\param inBuf Pointer to the input buffer containing compressed image data
		\param outBuf Pointer to the output buffer where decompressed image data will be stored
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws assert failure if the function is called
	*/
	void DecompressImageCTX1( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief This function is intended to decompress DXN1 format image data but currently has no implementation.

		The function is designed to decompress image data in DXN1 format, which is a specific texture compression format. The function takes input buffer containing compressed data, output buffer for
	   decompressed data, and dimensions of the image. The current implementation contains only an assertion that always fails, indicating this functionality is not yet implemented or supported.

		\param inBuf Pointer to the input buffer containing compressed DXN1 image data
		\param outBuf Pointer to the output buffer where decompressed image data will be stored
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\throws Assertion failure due to unimplemented functionality
	*/
	void DecompressImageDXN1( const byte* inBuf, byte* outBuf, int width, int height ) { assert( 0 ); }

	/*!
		\brief Decompresses YCoCg DXT5 format image data into RGBA format with scaled color components

		This function takes DXT5 compressed image data in YCoCg format and decompresses it into a standard RGBA format. The decompression uses the nVidia7x algorithm as the base implementation. After
	   decompression, the function scales the CoCg color components by adjusting them based on the scale factor derived from the blue channel, effectively setting the final scale factor to 1 for the
	   uncompressed output.

		\param inBuf Input buffer containing DXT5 compressed YCoCg data
		\param outBuf Output buffer where decompressed RGBA data will be written
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void DecompressYCoCgDXT5( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses YCoCg CTX1 DXT5A format texture data into CoCg_Y format output buffer

		This function processes texture data in YCoCg CTX1 DXT5A format and decompresses it into a CoCg_Y format output buffer. It iterates through the input texture block by block, decoding alpha
	   values and CTX1 color values for each 4x4 block. The function handles the width and height dimensions by processing the data in chunks of 4 pixels in both directions. The output buffer is
	   expected to be large enough to hold the decompressed data.

		\param inBuf Input buffer containing DXT5A compressed texture data
		\param outBuf Output buffer where decompressed CoCg_Y format data will be written
		\param width Width of the texture in pixels
		\param height Height of the texture in pixels
	*/
	void DecompressYCoCgCTX1DXT5A( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses DXT1 compressed normal map data into tangent space normal vectors.

		This function processes a DXT1 compressed normal map by decompressing each 4x4 block of pixels. It decodes the color values and converts them to normal vectors using a custom rotation and Z
	   component calculation. The output buffer receives the decompressed normal vectors in tangent space format.

		\param inBuf Pointer to the input DXT1 compressed data
		\param outBuf Pointer to the output buffer where decompressed normal vectors will be written
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void DecompressNormalMapDXT1( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses a DXT1 compressed normal map and renormalizes the resulting normals.

		This function takes a DXT1 compressed normal map as input and decompresses it into a standard RGBA format. During decompression, each normal vector is renormalized to ensure it has unit
	   length. The input buffer contains the compressed data while the output buffer receives the decompressed and renormalized normal data. The function processes the image in 4x4 pixel blocks.

		\param inBuf Pointer to the input buffer containing DXT1 compressed normal map data
		\param outBuf Pointer to the output buffer where the decompressed and renormalized normal map will be stored
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void DecompressNormalMapDXT1Renormalize( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses DXT5 format normal map data into tangent space normal vectors

		This function decompresses normal map data stored in DXT5 format into tangent space normal vectors. It processes the input data block by block, decoding alpha and normal Y values, then
	   computes the Z component of normals based on the constraint that normals are unit vectors. The output is stored in RGBA format where each byte represents a normalized component of the normal
	   vector.

		\param inBuf Input buffer containing DXT5 compressed normal map data
		\param outBuf Output buffer where decompressed normal data will be written
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void DecompressNormalMapDXT5( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses DXT5 normal map data with renormalization applied to each pixel

		This function processes DXT5 compressed normal map data by decompressing it into a raw format. It reads 4x4 pixel blocks from the input buffer, decodes both alpha and color values, then
	   renormalizes each normal vector to ensure it maintains unit length. The renormalization is applied to the RGB components of each pixel, with the red component corresponding to the x-axis, green
	   to the y-axis, and blue to the z-axis. The resulting normalized normal vectors are then packed back into the output buffer.

		\param inBuf Input buffer containing DXT5 compressed normal map data
		\param outBuf Output buffer where decompressed and renormalized normal map data will be stored
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void DecompressNormalMapDXT5Renormalize( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decompresses a DXN2 format normal map into tangent space normal values

		This function processes a compressed normal map in DXN2 format and converts it into a decompressed tangent space normal map. The input buffer contains compressed normal data that is decoded in
	   4x4 blocks. Each block is processed to extract x and y normal components from the alpha values, then the z component is calculated from the constraint that normals should be unit vectors. The
	   final result is stored in the output buffer with all components normalized to the range [0, 255]. The function handles the width and height parameters to properly iterate through the image
	   blocks and decompress the complete texture.

		\param inBuf Input buffer containing compressed DXN2 normal map data
		\param outBuf Output buffer to store decompressed tangent space normal map
		\param width Width of the normal map in pixels
		\param height Height of the normal map in pixels
	*/
	void DecompressNormalMapDXN2( const byte* inBuf, byte* outBuf, int width, int height );

	/*!
		\brief Decomposes a DXT1 compressed image into color indices and two color images.

		This function takes a DXT1 compressed image buffer and decomposes it into three components: color indices that define how pixels are colored, and two separate image buffers containing the
	   color data. The function processes the input image in 4x4 blocks, extracting color information and index data from each block. The width and height parameters define the dimensions of the image
	   being processed.

		\param inBuf Input buffer containing the DXT1 compressed image data
		\param colorIndices Output buffer to store the color indices for each pixel
		\param pic1 Output buffer to store the first set of color data
		\param pic2 Output buffer to store the second set of color data
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void DecomposeImageDXT1( const byte* inBuf, byte* colorIndices, byte* pic1, byte* pic2, int width, int height );

	/*!
		\brief Decomposes a DXT5 compressed image into color and alpha index data along with two color sample arrays.

		This function processes a DXT5 compressed image buffer and extracts color and alpha index information. It decomposes the image into 4x4 blocks and populates the provided output buffers with
	   color indices, alpha indices, and two color sample arrays. The function handles the full image dimensions by processing 4x4 pixel blocks and organizing the data accordingly.

		\param inBuf Input buffer containing the DXT5 compressed image data
		\param colorIndices Output buffer to store color indices for each pixel block
		\param alphaIndices Output buffer to store alpha indices for each pixel block
		\param pic1 Output buffer to store the first set of color samples
		\param pic2 Output buffer to store the second set of color samples
		\param width Width of the image in pixels
		\param height Height of the image in pixels
	*/
	void DecomposeImageDXT5( const byte* inBuf, byte* colorIndices, byte* alphaIndices, byte* pic1, byte* pic2, int width, int height );

private:
	int			   width;
	int			   height;
	const byte*	   inData;

	//! Reads a single byte from the input data stream and advances the stream pointer
	byte		   ReadByte();

	//! Reads a 16-bit unsigned integer from the input data stream and advances the data pointer.
	unsigned short ReadUShort();

	//! Reads a 32-bit unsigned integer from the input data stream and advances the data pointer
	unsigned int   ReadUInt();

	//! Converts a 3-byte color to a 16-bit 565 format.
	unsigned short ColorTo565( const byte* color ) const;

	//! Converts a 16-bit 565-encoded color value to an RGB byte array.
	void		   ColorFrom565( unsigned short c565, byte* color ) const;

	//! Converts a byte-sized Y normal component to a 565 format unsigned short.
	unsigned short NormalYTo565( byte y ) const;

	//! Extracts and expands the Y component from a 16-bit 565 format color value.
	byte		   NormalYFrom565( unsigned short c565 ) const;

	//! Returns the scaled normal value from a 565 format color component.
	byte		   NormalScaleFrom565( unsigned short c565 ) const;

	//! Converts a 565-encoded normal vector component to a biased byte value.
	byte		   NormalBiasFrom565( unsigned short c565 ) const;

	/*!
		\brief Writes a 4x4 block of RGBA pixel data from a color block to the output pointer at the specified coordinates

		This function takes a color block containing 16 pixels of RGBA data and copies it to the output buffer at the specified x,y coordinates. The output buffer is assumed to be a linear array of
	   RGBA pixels, with each pixel being 4 bytes. The function calculates the appropriate memory offset based on the current width and destination coordinates, then copies 4 rows of 16 bytes (4
	   pixels) each from the source color block to the output buffer

		\param outPtr Pointer to the beginning of the output pixel buffer
		\param x X coordinate of the destination block
		\param y Y coordinate of the destination block
		\param colorBlock Pointer to the source color block data containing 16 RGBA pixels
	*/
	void		   EmitBlock( byte* outPtr, int x, int y, const byte* colorBlock );

	//! Decodes alpha values from a color block using DXT compression.
	void		   DecodeAlphaValues( byte* colorBlock, const int offset );

	//! Decodes color values from a DXT color block into a byte array.
	void		   DecodeColorValues( byte* colorBlock, bool noBlack, bool writeAlpha );

	//! DecodesCTX1 color values from a color block.
	void		   DecodeCTX1Values( byte* colorBlock );

	//! Decomposes a DXT color block into color values and indices.
	void		   DecomposeColorBlock( byte colors[2][4], byte colorIndices[16], bool noBlack );

	//! Decomposes an alpha block from DXT data into color and alpha indices
	void		   DecomposeAlphaBlock( byte colors[2][4], byte alphaIndices[16] );

	/*!
		\brief Decodes Y-component normal values from a normal block with specified offset and retrieves bias and scale values.

		This function extracts Y-component normal data from a DXT-encoded normal block. It reads two 16-bit normal values, computes intermediate normal Y-values using weighted averages, and stores the
	   resulting Y-values in the normal block at the specified offset. The function also calculates and returns the bias and scale values derived from the first normal value.

		\param normalBlock Pointer to the block containing normal data
		\param offsetY Offset within the normal block where Y-values should be written
		\param bias Reference to store the bias value extracted from the normal data
		\param scale Reference to store the scale value extracted from the normal data
		\throws assertion failure if the first normal value is less than the second
	*/
	void		   DecodeNormalYValues( byte* normalBlock, const int offsetY, byte& bias, byte& scale );

	//! Computes and sets the Z component of normal vectors in a DXTC normal block from the X and Y components.
	void		   DeriveNormalZValues( byte* normalBlock );
};

ID_INLINE byte idDxtDecoder::ReadByte()
{
	byte b = *inData;
	inData += 1;
	return b;
}

ID_INLINE unsigned short idDxtDecoder::ReadUShort()
{
	unsigned short s = *( ( unsigned short* )inData );
	inData += 2;
	return s;
}

ID_INLINE unsigned int idDxtDecoder::ReadUInt()
{
	unsigned int i = *( ( unsigned int* )inData );
	inData += 4;
	return i;
}

ID_INLINE unsigned short idDxtDecoder::ColorTo565( const byte* color ) const
{
	return ( ( color[0] >> 3 ) << 11 ) | ( ( color[1] >> 2 ) << 5 ) | ( color[2] >> 3 );
}

ID_INLINE void idDxtDecoder::ColorFrom565( unsigned short c565, byte* color ) const
{
	color[0] = byte( ( ( c565 >> 8 ) & ( ( ( 1 << ( 8 - 3 ) ) - 1 ) << 3 ) ) | ( ( c565 >> 13 ) & ( ( 1 << 3 ) - 1 ) ) );
	color[1] = byte( ( ( c565 >> 3 ) & ( ( ( 1 << ( 8 - 2 ) ) - 1 ) << 2 ) ) | ( ( c565 >> 9 ) & ( ( 1 << 2 ) - 1 ) ) );
	color[2] = byte( ( ( c565 << 3 ) & ( ( ( 1 << ( 8 - 3 ) ) - 1 ) << 3 ) ) | ( ( c565 >> 2 ) & ( ( 1 << 3 ) - 1 ) ) );
}

ID_INLINE unsigned short idDxtDecoder::NormalYTo565( byte y ) const
{
	return ( ( y >> 2 ) << 5 );
}

ID_INLINE byte idDxtDecoder::NormalYFrom565( unsigned short c565 ) const
{
	byte c = byte( ( c565 & ( ( ( 1 << 6 ) - 1 ) << 5 ) ) >> 3 );
	return ( c | ( c >> 6 ) );
}

ID_INLINE byte idDxtDecoder::NormalBiasFrom565( unsigned short c565 ) const
{
	byte c = byte( ( c565 & ( ( ( 1 << 5 ) - 1 ) << 11 ) ) >> 8 );
	return ( c | ( c >> 5 ) );
}

ID_INLINE byte idDxtDecoder::NormalScaleFrom565( unsigned short c565 ) const
{
	byte c = byte( ( c565 & ( ( ( 1 << 5 ) - 1 ) << 0 ) ) << 3 );
	return ( c | ( c >> 5 ) );
}

#endif // !__DXTCODEC_H__
