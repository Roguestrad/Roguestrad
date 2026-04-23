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
#ifndef __COLORSPACE_H__
#define __COLORSPACE_H__

/*
================================================================================================
Contains the ColorSpace conversion declarations.
================================================================================================
*/

namespace idColorSpace
{

/*!
	\brief Converts RGB color space to YCoCg color space for a given image region

	This function performs color space conversion from RGB to YCoCg format. It processes each pixel in the input image and converts its components according to the YCoCg color space transformation.
   The conversion preserves the alpha channel as-is while transforming the red, green, and blue components into Y, Co, and Cg components respectively. Each component is clamped to the valid byte range
   after conversion.

	\param dst Output buffer where the converted YCoCg pixel data will be stored
	\param src Input buffer containing the RGB pixel data to be converted
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertRGBToYCoCg( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts YCoCg color space data to RGB color space.

	This function transforms image data from the YCoCg color space to the standard RGB color space. The input is expected to be a packed array of 4-byte values representing Y, Co, Cg, and alpha
   components. Each component is processed individually, with Co and Cg values being adjusted by subtracting 128 to center them around zero. The transformation is applied to each pixel in the image,
   converting the color representation while preserving the alpha channel. The result is clamped to valid byte values using the CLAMP_BYTE macro.

	\param dst Output buffer where the converted RGB data will be stored
	\param src Input buffer containing YCoCg data
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertYCoCgToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts RGB color space to CoCg_Y color space

	This function converts an RGB image to the CoCg_Y color space. The conversion uses the YCoCg color space transformation where Co represents the orange-chroma component, Cg represents the
   green-chroma component, and Y represents the luma component. The output is stored in a 4-channel format where the first channel is Co, the second is Cg, the third is zero, and the fourth is Y.

	\param dst output buffer for the CoCg_Y color space data
	\param src input buffer containing RGB color space data
	\param width width of the image in pixels
	\param height height of the image in pixels
*/
void ConvertRGBToCoCg_Y( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts CoCg_Y color space data to RGB format.

	This function transforms color data from the CoCg_Y color space to RGB format. The input data consists of four-component pixels where the first component represents chroma offset C0, the second
   represents chroma offset C1, the third represents alpha, and the fourth represents luminance Y. The function applies a color conversion formula to compute the RGB values while preserving the alpha
   component. All output values are clamped to the valid byte range 0-255.

	\param dst Output buffer for the converted RGB data
	\param src Input buffer containing CoCg_Y color data
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertCoCg_YToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts CoCgSY color space data to RGB format with scaling applied to color channels.

	This function processes pixel data in CoCgSY color space and converts it to RGB format. Each pixel consists of four components: Co (chrominance), Cg (chrominance), alpha (scale factor), and Y
   (luminance). The alpha component encodes a scale factor that is used to adjust the chrominance values for better compression. The conversion involves transforming the chrominance components using
   specific macros and applying clamping to ensure valid byte values for the RGB output.

	\param dst output buffer for RGB pixel data
	\param src input buffer containing CoCgSY pixel data
	\param width width of the image in pixels
	\param height height of the image in pixels
*/
void ConvertCoCgSYToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts RGB image data to YCoCg 4:2:0 format

	This function performs color space conversion from RGB to YCoCg format with 4:2:0 chroma subsampling. It processes the source image in 2x2 pixel blocks, converting each pixel to YCoCg components
   and then subsampling the chroma values. The Y component is stored as individual values for each pixel in the block, while the Co and Cg components are averaged from the four pixels in the block.
   The resulting data is written to the destination buffer in a format suitable for video compression or storage.

	\param dst Output buffer where the converted YCoCg 4:2:0 data will be stored
	\param src Input buffer containing RGB data to be converted
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertRGBToYCoCg420( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts YCoCg 4:2:0 format image data to RGB format

	This function performs conversion from YCoCg 4:2:0 chroma subsampling format to standard RGB format. The input data contains luminance (Y) values for each pixel and chroma (Co, Cg) values that are
   subsampled at half resolution in both dimensions. The function processes the data in 2x2 pixel blocks, decoding the chroma values and applying the YCoCg to RGB transformation. The output is stored
   as RGBA format with alpha channel set to 255.

	\param dst Output buffer for the RGB converted image data
	\param src Input buffer containing YCoCg 4:2:0 format image data
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertYCoCg420ToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts RGB color space to YCbCr color space for image data

	This function performs a color space conversion from RGB to YCbCr for a given image region. It processes each pixel in the source image and converts the RGB values to their corresponding YCbCr
   components. The conversion uses standard RGB to YCbCr transformation formulas where Y represents the luma component, and Cb and Cr represent the chroma components. The resulting YCbCr values are
   clamped to valid byte ranges and the alpha channel is preserved from the source. The conversion is performed in-place on a 4-channel pixel format where each pixel consists of four bytes (RGBA).

	\param dst output buffer for converted YCbCr pixel data
	\param src input buffer containing RGB pixel data
	\param width width of the image region in pixels
	\param height height of the image region in pixels
*/
void ConvertRGBToYCbCr( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts YCbCr color space data to RGB format

	This function performs conversion from YCbCr color space to RGB color space for image data. It processes each pixel in the source data, extracting the Y (luminance), Cb (blue-difference chroma),
   and Cr (red-difference chroma) components. The Y component is used directly, while the Cb and Cr components are adjusted by subtracting 128 to center the chroma values around zero. The conversion
   uses predefined macros CBCR_TO_R, CBCR_TO_G, and CBCR_TO_B to calculate the RGB values from the YCbCr components. Each converted RGB component is clamped to the valid byte range 0-255 to prevent
   overflow.

	\param dst Pointer to the destination buffer where the converted RGB data will be stored
	\param src Pointer to the source buffer containing YCbCr data with 4 bytes per pixel (Y, Cb, Cr, unused)
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertYCbCrToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts RGB color space to CbCr_Y color space with clamped byte output

	This function performs color space conversion from RGB to CbCr_Y format, where Cb and Cr represent chrominance components and Y represents luma. The conversion uses standard RGB to YCbCr
   conversion formulas. The output values are clamped to byte range and the alpha channel is preserved from the source. The dst buffer must be large enough to hold width * height * 4 bytes, and the
   src buffer must contain width * height * 4 bytes of RGB data with alpha. The conversion process handles each pixel individually, transforming RGB values to their corresponding YCbCr components
   while maintaining proper channel ordering in the output

	\param dst output buffer for converted color data in CbCr_Y format
	\param src input buffer containing RGB data with alpha channel
	\param width width of the image in pixels
	\param height height of the image in pixels
*/
void ConvertRGBToCbCr_Y( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts CbCr Y data to RGB format with proper byte clamping

	This function performs color space conversion from a combined CbCr and Y format to RGB format. The input data is organized as interleaved 4-byte blocks containing Cb, Cr, alpha, and Y components.
   Each component is processed individually, with Cb and Cr values adjusted by subtracting 128 to center the range around zero. The conversion uses specific macros CBCR_TO_R, CBCR_TO_G, and CBCR_TO_B
   to transform the color components. The resulting RGB values are clamped to valid byte ranges and the alpha component is preserved from the input

	\param dst Output buffer where the converted RGB data will be stored
	\param src Input buffer containing interleaved Cb, Cr, alpha, and Y components
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertCbCr_YToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts RGB image data to YCbCr 4:2:0 chroma subsampled format

	This function performs color space conversion from RGB to YCbCr format with 4:2:0 chroma subsampling. It processes the input RGB data in 2x2 pixel blocks, converting each pixel to YCbCr values and
   then subsampling the chroma components by averaging four neighboring chroma values. The luminance (Y) components are stored in the first four bytes of each 2x2 block, while the subsampled chroma
   (Cb and Cr) components are stored in the next two bytes.

	\param dst Output buffer for the YCbCr 4:2:0 formatted data
	\param src Input buffer containing RGB data with alpha channel
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertRGBToYCbCr420( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts YCbCr 4:2:0 format pixel data to RGB format

	This function performs color space conversion from YCbCr 4:2:0 format to RGB format. It processes the input YCbCr data, which is stored in a planar format with separate planes for luminance (Y)
   and chrominance (Cb, Cr), and converts it to RGB format with interleaved color components. The conversion uses fixed-point arithmetic with a 16-bit precision for intermediate calculations. The
   function handles subsampled chrominance data by duplicating chrominance values to match the resolution of the luminance data. The output is stored as 4-byte RGBA values with alpha set to 255.

	\param dst Output buffer where the converted RGB values will be stored
	\param src Input buffer containing YCbCr 4:2:0 format pixel data
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertYCbCr420ToRGB( byte* dst, const byte* src, int width, int height );

/*!
	\brief Converts a normal map to a stereographic height map using iterative reconstruction

	This function performs iterative reconstruction to convert a normal map into a height map using stereographic projection. It processes the normal map data through multiple iterations, updating
   height values based on the normal vector components. The algorithm follows a specific pattern of forward and backward passes in both horizontal and vertical directions. The resulting height map is
   normalized to byte values between 0 and 255, and the output scale parameter is set to the range of height values in the result.

	\param heightMap Output buffer where the resulting height map will be stored as bytes
	\param normalMap Input buffer containing the normal map data as bytes
	\param width Width of the normal map and output height map in pixels
	\param height Height of the normal map and output height map in pixels
	\param scale Reference to a float that will be set to the range of height values in the output map
*/
void ConvertNormalMapToStereographicHeightMap( byte* heightMap, const byte* normalMap, int width, int height, float& scale );

/*!
	\brief Converts a stereographic height map into a normal map using specified scale factor

	This function transforms a height map representation of a stereographic projection into a standard normal map. It processes each pixel by calculating gradients in the X and Y directions from
   neighboring height values, then applies a stereographic projection formula to compute the normal vector. The resulting normal vectors are converted to byte format and stored in the output normal
   map array. The height map is assumed to be organized as a 1D array with width and height dimensions, and the normal map is stored with 4 bytes per pixel (RGBA format) where the alpha channel is set
   to 255.

	\param normalMap Output buffer where the computed normal map will be stored
	\param heightMap Input buffer containing the height map data
	\param width Width of the height map and normal map in pixels
	\param height Height of the height map and normal map in pixels
	\param scale Scaling factor to adjust the influence of height differences on normal vectors
*/
void ConvertStereographicHeightMapToNormalMap( byte* normalMap, const byte* heightMap, int width, int height, float scale );

/*!
	\brief Converts an RGB image to monochrome by averaging the red, green, and blue components for each pixel.

	This function takes an RGB image represented as a 4-channel array (RGBA) and converts it to a monochrome image. Each pixel's intensity is calculated as the average of its red, green, and blue
   components. The input RGB array is expected to have 4 bytes per pixel with alpha channel data, but only the first three bytes are used for the conversion. The resulting monochrome values are stored
   in the output array, where each entry represents a single byte intensity value.

	\param mono Output array to store the monochrome pixel values
	\param rgb Input array containing RGB data with 4 bytes per pixel (RGBA)
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertRGBToMonochrome( byte* mono, const byte* rgb, int width, int height );

/*!
	\brief Converts monochrome pixel data to RGB format by setting each RGB component to the corresponding monochrome value.

	This function takes monochrome pixel data and converts it to RGB format. Each monochrome value is replicated across the red, green, and blue channels of the output RGB pixel. The input monochrome
   data is assumed to be stored as a single byte per pixel, and the output RGB data is stored as four bytes per pixel (RGBA format) with the alpha channel set to 255.

	\param rgb Output buffer where the converted RGB data will be stored
	\param mono Input buffer containing the monochrome pixel data
	\param width Width of the image in pixels
	\param height Height of the image in pixels
*/
void ConvertMonochromeToRGB( byte* rgb, const byte* mono, int width, int height );
};

#endif // !__COLORSPACE_H__
