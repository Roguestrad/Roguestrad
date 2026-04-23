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
#ifndef __BINARYIMAGE_H__
#define __BINARYIMAGE_H__

#include "BinaryImageData.h"

/*!
	\class idBinaryImage
	\brief A class for managing binary image data including texture loading, mipmap generation, and file I/O operations.

	This class serves as a container for binary image data and provides functionality for loading images from memory in various formats, generating mipmaps, and handling file I/O operations for
   preprocessed image data. It supports 2D, 2D atlas, and cubic environment map image formats with multiple compression options including DXT1, DXT5, BC6H, and BC7. The class maintains image headers
   and data for multiple images within a single binary structure and provides mechanisms for generating file names and managing timestamps for cached image files. It is designed to work with different
   texture and color formats, supports gamma correction for mipmaps, and handles memory allocation for image data and mip levels using the engine's custom allocator. The class can be used both as a
   regular object and in a memory-mapped form for offline megatexture generation.

*/
class idBinaryImage
{
public:
	//! Constructs an idBinaryImage object with the specified name.
	idBinaryImage( const char* name ) :
		imgName( name )
	{
	}

	//! Returns the name of the binary image.
	const char*			 GetName() const { return imgName.c_str(); }

	//! Sets the name of the binary image to the provided string.
	void				 SetName( const char* _name ) { imgName = _name; }

	/*!
		\brief Loads a 2D image from memory, processing it according to specified texture and color formats while supporting multiple mipmap levels.

		This function initializes a 2D binary image from raw pixel data provided in memory. It handles various color format conversions such as YCoCg and normal map formats, and supports different
	   texture formats including DXT1, DXT5, BC6H, and others. The function also manages mipmap generation by allocating appropriate memory for each level and applying high-quality or fast compression
	   techniques based on configuration settings. Padding is applied to ensure dimensions are multiples of 4 for DXT compression. Memory allocation is performed using the engine's custom allocator,
	   and progress indicators are updated during processing.

		\param width The width of the image in pixels
		\param height The height of the image in pixels
		\param pic_const Pointer to the source pixel data in memory
		\param numLevels The number of mipmap levels to generate
		\param textureFormat Reference to the output texture format identifier
		\param colorFormat Reference to the output color format identifier
		\param gammaMips Flag indicating whether gamma correction should be applied to mipmap levels
	*/
	void				 Load2DFromMemory( int width, int height, const byte* pic_const, int numLevels, textureFormat_t& textureFormat, textureColor_t& colorFormat, bool gammaMips );

	/*!
		\brief Loads a 2D atlas mipchain from memory, processing image data and generating compressed texture levels.

		This function processes image data from memory to create a texture mipchain for 2D atlases. It handles various color format conversions such as YCoCg, normal maps, and green-alpha formats. The
	   function allocates memory for source image data, applies necessary color space conversions, and then generates multiple mip levels by scaling and compressing the image data. The compression
	   utilizes different algorithms based on the specified texture format including DXT1, DXT5, BC6H, and BC7 formats. The function supports high-quality and fast compression modes based on a global
	   setting.

		\param width The width of the source image
		\param height The height of the source image
		\param pic_const Pointer to the source image data in memory
		\param numLevels Number of mip levels to generate
		\param textureFormat Reference to the texture format to use for compression
		\param colorFormat Reference to the color format for processing
	*/
	void				 Load2DAtlasMipchainFromMemory( int width, int height, const byte* pic_const, int numLevels, textureFormat_t& textureFormat, textureColor_t& colorFormat );

	/*!
		\brief Loads cubic environment map data from memory into the binary image structure.

		This function processes six image sides (representing a cube map) from memory and compresses them into the specified texture format. It supports multiple compression modes including DXT1,
	   DXT5, BC6H, and BC7. The function handles mipmapping and gamma correction based on the provided parameters. Each side is processed through multiple mipmap levels, and the resulting compressed
	   data is stored internally for later use.

		\param width The width dimension of the square cube map faces
		\param pics An array of pointers to the six image sides (faces) of the cube map
		\param numLevels Number of mipmap levels to generate for each cube face
		\param textureFormat Reference to the texture format that will be used for compression
		\param gammaMips Flag indicating whether gamma correction should be applied during mipmap generation
	*/
	void				 LoadCubeFromMemory( int width, const byte* pics[6], int numLevels, textureFormat_t& textureFormat, bool gammaMips );

	//! Loads a preprocessed binary image from a generated file, validating the file format and timestamp.
	bool				 LoadFromGeneratedFile( idFile* f, ID_TIME_T sourceFileTime );

	//! Loads a preprocessed image from the generated folder using the specified source file time.
	ID_TIME_T			 LoadFromGeneratedFile( ID_TIME_T sourceFileTime );

	//! Writes a generated binary image file and returns its timestamp
	ID_TIME_T			 WriteGeneratedFile( ID_TIME_T sourceFileTime );

	//! Returns a constant reference to the file header data of the binary image.
	const bimageFile_t&	 GetFileHeader() { return fileData; }

	//! Returns the number of images contained in the binary image object.
	int					 NumImages() { return images.Num(); }

	//! Returns a reference to the image header at the specified index.
	const bimageImage_t& GetImageHeader( int i ) const { return images[i]; }

	//! Returns a pointer to the image data for the specified image index.
	const byte*			 GetImageData( int i ) const { return images[i].data; }

	//! Generates a file name for a binary image resource based on the provided image name.
	static void			 GetGeneratedFileName( idStr& gfn, const char* imageName );

private:
	idStr		 imgName; // game path, including extension (except for cube maps), may be an image program
	bimageFile_t fileData;

	/*!
		\class idBinaryImage::idBinaryImageData
		\brief Represents the data storage for binary image objects.
	*/
	class idBinaryImageData : public bimageImage_t
	{
	public:
		byte* data;

		//! Initializes a new instance of the idBinaryImageData class with null data pointer.
		idBinaryImageData() :
			data( NULL )
		{
		}
		~idBinaryImageData() { Free(); }

		//! Assigns the contents of another binary image data object to this object
		idBinaryImageData& operator=( idBinaryImageData& other )
		{
			if( this == &other ) { return *this; }

			Alloc( other.dataSize );
			memcpy( data, other.data, other.dataSize );
			return *this;
		}

		//! Moves the data from another idBinaryImageData instance to this instance.
		idBinaryImageData& operator=( idBinaryImageData&& other )
		{
			if( this == &other ) { return *this; }
			dataSize   = other.dataSize;
			data	   = other.data;
			other.data = NULL;
			return *this;
		}

		//! Frees the allocated memory for the binary image data.
		void Free()
		{
			if( data != NULL ) {
				Mem_Free( data );
				data	 = NULL;
				dataSize = 0;
			}
		}

		//! Allocates memory for the binary image data with the specified size.
		void Alloc( int size )
		{
			Free();
			dataSize = size;
			data	 = ( byte* )Mem_Alloc( size, TAG_CRAP );
		}
	};

	idList<idBinaryImageData, TAG_IDLIB_LIST_IMAGE> images;

private:
	//! Sets the generated file name for this binary image.
	void MakeGeneratedFileName( idStr& gfn );
};

#endif // __BINARYIMAGE_H__
