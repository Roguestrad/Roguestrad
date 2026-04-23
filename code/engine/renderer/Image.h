/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2021 Robert Beckebans
Copyright (C) 2016-2017 Dustin Land
Copyright (C) 2022 Stephen Pridham

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

#ifndef IMAGE_H_
#define IMAGE_H_

enum textureType_t {
	// RB: renamed to DDT_ to solve conflict with AMD compressonator
	DTT_DISABLED,
	DTT_2D,
	DTT_CUBIC,
	DTT_2D_ARRAY,		// new
	DTT_2D_MULTISAMPLE, // new
};

/*
================================================
The internal *Texture Format Types*, ::textureFormat_t, are:
================================================
*/
enum textureFormat_t {
	FMT_NONE,

	//------------------------
	// Standard color image formats
	//------------------------

	FMT_RGBA8, // 32 bpp
	FMT_XRGB8, // 32 bpp

	//------------------------
	// Alpha channel only
	//------------------------

	// Alpha ends up being the same as L8A8 in our current implementation, because straight
	// alpha gives 0 for color, but we want 1.
	FMT_ALPHA,

	//------------------------
	// Luminance replicates the value across RGB with a constant A of 255
	// Intensity replicates the value across RGBA
	//------------------------

	FMT_L8A8, // 16 bpp
	FMT_LUM8, //  8 bpp
	FMT_INT8, //  8 bpp

	//------------------------
	// Compressed texture formats
	//------------------------

	FMT_DXT1, // 4 bpp
	FMT_DXT5, // 8 bpp

	//------------------------
	// Depth buffer formats
	//------------------------

	FMT_DEPTH, // 24 bpp

	//------------------------
	//
	//------------------------

	FMT_X16,	 // 16 bpp
	FMT_Y16_X16, // 32 bpp
	FMT_RGB565,	 // 16 bpp

	// ^-- used in BFG edition, don't change above for original .bimage compatibility

	FMT_ETC1_RGB8_OES, // 4 bpp
	FMT_SHADOW_ARRAY,  // 32 bpp * 6
	FMT_RG16F,		   // 32 bpp
	FMT_RGBA16F,	   // 64 bpp
	FMT_RGBA32F,	   // 128 bpp
	FMT_R32F,		   // 32 bpp
	FMT_R11G11B10F,	   // 32 bpp

	// ^-- used up until RBDOOM-3-BFG 1.5 == BIMAGE_VERSION_BFG

	FMT_BC6H, // 8 bpp
	FMT_BC7,  // 8 bpp

	// ^-- introduced with RBDOOM-3-BFG 1.6 >= BIMAGE_VERSION_BC6

	//------------------------
	// Render targets only
	//------------------------

	FMT_R8F,		   // 8 bpp, RT only
	FMT_DEPTH_STENCIL, // 32 bpp, RT only
	FMT_RGBA16S,	   // 64 bpp, RT only
};

//! Returns the number of bits per pixel for a given texture format.
int BitsForFormat( textureFormat_t format );

//! Returns the row pitch in bytes for a texture given its format and width
int GetRowPitch( const textureFormat_t& format, int width );

/*
================================================
DXT5 color formats
================================================
*/
enum textureColor_t {
	CFM_DEFAULT,	 // RGBA
	CFM_NORMAL_DXT5, // XY format and use the fast DXT5 compressor
	CFM_YCOCG_DXT5,	 // convert RGBA to CoCg_Y format
	CFM_GREEN_ALPHA, // Copy the alpha channel to green
};

/*!
	\class idImageOpts
	\brief Holds parameters for texture operations.
*/
class idImageOpts
{
public:
	//! Initializes a new instance of the idImageOpts class with default values.
	idImageOpts();

	//! Compares two idImageOpts objects for equality by performing a byte-by-byte comparison of their contents.
	bool			operator==( const idImageOpts& opts );

	//---------------------------------------------------
	// these determine the physical memory size and layout
	//---------------------------------------------------

	textureType_t	textureType;
	textureFormat_t format;
	textureColor_t	colorFormat;
	uint			samples;
	int				width;
	int				height;	   // not needed for cube maps
	int				numLevels; // if 0, will be 1 for NEAREST / LINEAR filters, otherwise based on size
	bool			gammaMips; // if true, mips will be generated with gamma correction
	bool			readback;  // 360 specific - cpu reads back from this texture, so allocate with cached memory
	bool			isRenderTarget;
	bool			isUAV;
};

ID_INLINE idImageOpts::idImageOpts()
{
	format		   = FMT_NONE;
	colorFormat	   = CFM_DEFAULT;
	samples		   = 1;
	width		   = 0;
	height		   = 0;
	numLevels	   = 0;
	textureType	   = DTT_2D;
	gammaMips	   = false;
	readback	   = false;
	isRenderTarget = false;
	isUAV		   = false;
}

ID_INLINE bool idImageOpts::operator==( const idImageOpts& opts )
{
	return ( memcmp( this, &opts, sizeof( *this ) ) == 0 );
}

/*
====================================================================

IMAGE

idImage have a one to one correspondance with GL/DX/GCM textures.

No texture is ever used that does not have a corresponding idImage.

====================================================================
*/

static const int MAX_TEXTURE_LEVELS = 14;

// How is this texture used?  Determines the storage and color format
// NOTE: be very careful when editing these because it might break older .bimage files or the lookup name
// Only add new entries at the bottom
typedef enum {
	TD_SPECULAR,		   // may be compressed, and always zeros the alpha channel
	TD_DIFFUSE,			   // may be compressed
	TD_DEFAULT,			   // generic RGBA texture (particles, etc...)
	TD_BUMP,			   // may be compressed with 8 bit lookup
	TD_FONT,			   // Font image
	TD_LIGHT,			   // Light image
	TD_LOOKUP_TABLE_MONO,  // Mono lookup table (including alpha)
	TD_LOOKUP_TABLE_ALPHA, // Alpha lookup table with a white color channel
	TD_LOOKUP_TABLE_RGB1,  // RGB lookup table with a solid white alpha
	TD_LOOKUP_TABLE_RGBA,  // RGBA lookup table
	TD_COVERAGE,		   // coverage map for fill depth pass when YCoCG is used
	TD_DEPTH,			   // depth buffer copy for motion blur
	// RB begin
	TD_SPECULAR_PBR_RMAO,  // may be compressed, and always zeros the alpha channel, linear RGB R = roughness, G = metal, B = ambient occlusion
	TD_SPECULAR_PBR_RMAOD, // may be compressed, alpha channel contains displacement map
	TD_HIGHQUALITY_CUBE,   // motorsep - Uncompressed cubemap texture (RGB colorspace)
	TD_LOWQUALITY_CUBE,	   // motorsep - REMOVED
	TD_SHADOW_ARRAY,	   // 2D depth buffer array for shadow mapping
	TD_RG16F,			   // BRDF lookup table
	TD_RGBA16F,			   // RT = render target format only, not written to disk
	TD_RGBA16S,			   // RT only
	TD_RGBA32F,			   // RT only
	TD_HDR_LIGHTPROBE,	   // RB: 2D HDR octahedron probes stored as R11G11B10F in v1.3 - v1.5 and BC6 now
	// ^-- used up until RBDOOM-3-BFG 1.5
	TD_HDRI, // RB: R11G11B10F or BC6
	// RB end
	TD_R32F,		  // RT only
	TD_R8F,			  // SP: RT only, added for ambient occlusion
	TD_LDR,			  // SP: RT only, added for SRGB render target when tonemapping
	TD_DEPTH_STENCIL, // SP: RT only, depth buffer and stencil buffer
} textureUsage_t;

// NOTE: be very careful when editing these because it might break older .bimage files or the lookup name
// Only add new entries at the bottom
typedef enum {
	CF_2D,				   // not a cube map
	CF_NATIVE,			   // _px, _nx, _py, etc, directly sent to GL
	CF_CAMERA,			   // _forward, _back, etc, rotated and flipped as needed before sending to GL
	CF_PANORAMA,		   // RB: latlong encoded HDRI panorama typically used by Substance or Blender
	CF_2D_ARRAY,		   // not a cube map but not a single 2d texture either
	CF_2D_PACKED_MIPCHAIN, // usually 2d but can be an octahedron, packed mipmaps into single 2d texture atlas and limited to dim^2
	// ^-- used up until RBDOOM-3-BFG 1.3
	CF_SINGLE, // SP: A single texture cubemap. All six sides in one image.
	CF_QUAKE1, // RB: _ft, _bk, etc, rotated and flipped as needed before sending to GL
} cubeFiles_t;

typedef void ( *ImageGeneratorFunction )( idImage* image, nvrhi::ICommandList* commandList );

#include "BinaryImage.h"

#define MAX_IMAGE_NAME 256

/*!
	\class idImage
	\brief Manages GPU texture resources including loading, uploading, and rendering operations.

	The idImage class represents a GPU texture resource that handles various image operations such as loading from disk, uploading pixel data, and managing texture states. It supports both regular and
   specialized textures like shadow arrays, cube maps, and multisampled textures. The class provides methods for binding textures, copying framebuffer contents, and managing texture memory through
   allocation, purging, and resizing operations. Texture resources are managed through command lists for GPU operations, with support for NVRHI and legacy rendering paths. The class includes
   functionality for procedural texture generation, sampler creation, and integration with rendering systems including ImGui. It maintains metadata about texture dimensions, usage types, and
   compression status while providing mechanisms for reloading modified assets and tracking resource references.

*/
class idImage
{
	friend class Framebuffer;

public:
	//! Constructs an idImage object with the specified name and initializes its properties.
	idImage( const char* name );

	//! Destroys an idImage object and cleans up its resources.
	~idImage();

	//! Returns the name of the image.
	const char*		   GetName() const { return imgName; }

	//! Makes this image active on the current texture unit.
	void			   Bind();

	/*!
		\brief Generates a shadow array texture with specified dimensions and rendering parameters

		Initializes a shadow array texture with the given width and height dimensions. Sets the texture filtering, repeating behavior, and usage parameters. The texture is configured as a 2D array
	   with render target capabilities and is allocated for GPU usage. The actual GPU upload occurs at a deferred state.

		\param width The width dimension of the shadow array texture
		\param height The height dimension of the shadow array texture
		\param filter The filtering mode to apply to the texture
		\param repeat The repeating behavior for texture coordinates
		\param usage Usage flags specifying how the texture will be used
		\param commandList Command list for GPU operations
	*/
	void			   GenerateShadowArray( int width, int height, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, nvrhi::ICommandList* commandList );

	/*!
		\brief Copies a portion of the framebuffer to this image

		This function performs a framebuffer copy operation for a specified rectangular region defined by the x, y, width, and height parameters. The operation increments a performance counter
	   tracking framebuffer copy operations. The implementation currently only increments the counter and does not perform the actual framebuffer copy.

		\param x The x coordinate of the upper-left corner of the region to copy
		\param y The y coordinate of the upper-left corner of the region to copy
		\param width The width of the region to copy
		\param height The height of the region to copy
	*/
	void			   CopyFramebuffer( int x, int y, int width, int height );

	/*!
		\brief Copies a specified rectangular region from the depth buffer into this image.

		This function performs a copy operation of a rectangular region from the depth buffer into the current image. It takes the coordinates and dimensions of the region to be copied, and updates
	   the image data accordingly. The function increments a performance counter to track frame buffer copy operations.

		\param x The x-coordinate of the top-left corner of the region to copy
		\param y The y-coordinate of the top-left corner of the region to copy
		\param width The width of the region to copy
		\param height The height of the region to copy
	*/
	void			   CopyDepthbuffer( int x, int y, int width, int height );

	/*!
		\brief Uploads image data to a texture, handling both regular and cube map textures

		This function uploads image data to a texture, with special handling for cube map animations where the height is six times the width. For cube maps, it processes six separate image sections
	   and uploads them as a single texture. For regular images, it uploads the data directly. The function supports both NVRHI and legacy rendering paths, and handles texture state management through
	   command lists. It also updates internal texture dimensions and allocation when needed.

		\param pic Pointer to the image data to upload
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\param commandList Command list for texture state management
	*/
	void			   UploadScratch( const byte* pic, int width, int height, nvrhi::ICommandList* commandList );

	//! Returns the estimated storage size of the image in bytes
	int				   StorageSize() const;

	//! Prints a one line summary of the image to the console.
	void			   Print() const;

	//! Reloads the image from disk if its source file has been modified, or regenerates it if it has a generator function.
	void			   Reload( bool force, nvrhi::ICommandList* commandList );

	void			   AddReference() { refCount++; };

	//! Initializes the image with a default grid pattern using the provided command list.
	void			   MakeDefault( nvrhi::ICommandList* commandList );

	//! Returns the image options associated with this image.
	const idImageOpts& GetOpts() const { return opts; }

	//! Returns the width of the image as it was uploaded to GPU memory.
	int				   GetUploadWidth() const { return opts.width; }

	//! Returns the height of the image as it was uploaded to GPU memory
	int				   GetUploadHeight() const { return opts.height; }

	//! Returns the upload resolution of the image
	idVec2i			   GetUploadResolution() const { return idVec2i( opts.width, opts.height ); }

	//! Marks the image as referenced outside of level loading.
	void			   SetReferencedOutsideLevelLoad() { referencedOutsideLevelLoad = true; }

	//! Marks the image as referenced during level loading.
	void			   SetReferencedInsideLevelLoad() { levelLoadReferenced = true; }

	//! Loads the image data from disk or generates it if it's a procedural texture
	void			   ActuallyLoadImage( bool fromBackEnd, nvrhi::ICommandList* commandList );

	//! Adds the image to the list of images to load on the main thread to the GPU.
	void			   DeferredLoadImage();

	//! Removes the image from the list of images to load on the main thread to the GPU.
	void			   DeferredPurgeImage();

	//---------------------------------------------
	// Platform specific implementations
	//---------------------------------------------

#if defined( USE_AMD_ALLOCATOR )
	static void EmptyGarbage();
#endif

	//! Allocates an image with specified options, filter, and repeat settings.
	void				 AllocImage( const idImageOpts& imgOpts, textureFilter_t filter, textureRepeat_t repeat );

	//! Frees the texture resources while keeping the image structure available for reloading or resizing
	void				 PurgeImage();

	/*!
		\brief Uploads a sub-image region to a specific mip level of the image

		This function uploads a rectangular region of image data to a specified mip level of the image. The destination coordinates are relative to the beginning of the specified mip level. For 2D
	   textures, the z coordinate is 0. For cube maps, z ranges from 0 to 5 representing the six faces. For 3D textures, z ranges from 0 to the upload depth. The data must match the image format, and
	   can be in bytes, half floats, floats, or DXT compressed format. Data is expected in OpenGL RGBA format, but console platforms may need to reorganize it. The pixelPitch parameter is only
	   required when updating from a source subrectangle. When dealing with DXT compressed data, width, height, and destination coordinates must be multiples of four.

		\param mipLevel The mip level to upload the sub-image to
		\param destX The X coordinate of the destination top-left corner in the mip level
		\param destY The Y coordinate of the destination top-left corner in the mip level
		\param destZ The Z coordinate of the destination, 0 for 2D, 0-5 for cube maps, 0-uploadDepth for 3D textures
		\param width The width of the sub-image region to upload
		\param height The height of the sub-image region to upload
		\param data Pointer to the source image data to upload
		\param commandList The command list to use for the upload operation
		\param pixelPitch The pitch of the source data in pixels, used when updating from a subrectangle
		\throws assertion failure if x, y, mipLevel, width, or height are negative, or if mipLevel is not less than the number of mipmap levels
	*/
	void				 SubImageUpload( int mipLevel, int destX, int destY, int destZ, int width, int height, const void* data, nvrhi::ICommandList* commandList, int pixelPitch = 0 );

	//! Resizes the image to the specified width and height dimensions.
	void				 Resize( int width, int height );

	//! Returns true if the image uses a compressed format.
	bool				 IsCompressed() const { return ( opts.format == FMT_DXT1 || opts.format == FMT_DXT5 || opts.format == FMT_BC6H || opts.format == FMT_BC7 ); }

	//! Returns the usage category of the image texture.
	textureUsage_t		 GetUsage() const { return usage; }

	//! Returns true if the image has been successfully loaded.
	bool				 IsLoaded() const;

	//! Creates a sampler for this texture to use in the shader.
	void				 CreateSampler();

	//! Returns true if the image is using a default texture
	bool				 IsDefaulted() const { return defaulted; }

	//! Generates a unique name for a texture using usage and cube map information while preserving the original file extension.
	static void			 GetGeneratedName( idStr& _name, const textureUsage_t& _usage, const cubeFiles_t& _cube );

	/*!
		\brief Initializes an image resource with specified parameters and optional pixel data

		Creates or updates an image resource using provided pixel data, dimensions, and texture settings. The function handles both static image loading from memory and dynamic allocation for render
	   targets or UAVs. When pixel data is provided, it processes the image through the binary image loader to support various formats and mipmapping strategies. For multisampled textures, it
	   allocates the image without loading pixel data. The function also supports special cube file handling for packed mipchains and integrates with the NVRHI command list for texture state
	   management when available.

		\param pic Pointer to pixel data or NULL for internal framebuffer images
		\param width Width of the image in pixels
		\param height Height of the image in pixels
		\param filter Texture filtering mode to use
		\param repeat Texture repeating behavior
		\param usage Texture usage type for resource allocation
		\param commandList Command list for texture state management
		\param isRenderTarget Flag indicating if image is used as render target
		\param isUAV Flag indicating if image is used as unordered access view
		\param sampleCount Number of samples for multisampling
		\param cubeFiles Cube file handling mode
	*/
	void				 GenerateImage( const byte* pic,
						int							width,
						int							height,
						textureFilter_t				filter,
						textureRepeat_t				repeat,
						textureUsage_t				usage,
						nvrhi::ICommandList*		commandList,
						bool						isRenderTarget = false,
						bool						isUAV		   = false,
						uint						sampleCount	   = 1,
						cubeFiles_t					cubeFiles	   = CF_2D );

	void				 GenerateCubeImage( const byte* pic[6], int size, textureFilter_t filter, textureUsage_t usage, nvrhi::ICommandList* commandList );

	//! Updates the texture parameters for anisotropy and trilinear filtering.
	void				 SetTexParameters();

	//! Returns a texture ID for integration with ImGui.
	void*				 GetImGuiTextureID() { return nullptr; }

	//! Returns the texture handle associated with this image.
	nvrhi::TextureHandle GetTextureHandle() { return texture; }

	//! Returns the texture identifier for this image.
	void*				 GetTextureID() { return ( void* )texture.Get(); }

	//! Returns a sampler object for the image, configured based on the pixelated look setting.
	void*				 GetSampler( SamplerCache& samplerCache );

	//! Returns a GPU sampler object for this image, creating it if necessary
	void*				 GetSampler( nvrhi::IDevice* device )
	{
		if( !sampler ) { sampler = device->createSampler( samplerDesc ); }

		return ( void* )sampler;
	}

	//! Returns a pointer to the sampler description used by this image.
	nvrhi::SamplerDesc* GetSamplerDesc() { return &samplerDesc; }

	//! Sets the sampler handle for the image.
	void				SetSampler( nvrhi::SamplerHandle _sampler ) { sampler = _sampler; }

private:
	friend class idImageManager;

	//! Derives the optimal texture format and color format based on the image usage type and other properties.
	void				   DeriveOpts();

	//! Allocates and initializes a GPU texture resource based on image options.
	void				   AllocImage();

	//! Sets the texture filtering and repeat state for the image.
	void				   SetSamplerState( textureFilter_t tf, textureRepeat_t tr );

	// parameters that define this image
	idStr				   imgName;	  // game path, including extension (except for cube maps), may be an image program
	cubeFiles_t			   cubeFiles; // If this is a cube map, and if so, what kind
	int					   cubeMapSize;
	ImageGeneratorFunction generatorFunction; // NULL for files
	textureUsage_t		   usage;			  // Used to determine the type of compression to use
	idImageOpts			   opts;			  // Parameters that determine the storage method

	// Sampler settings
	textureFilter_t		   filter;
	textureRepeat_t		   repeat;

	bool				   isLoaded;
	bool				   referencedOutsideLevelLoad;
	bool				   levelLoadReferenced; // for determining if it needs to be purged
	bool				   defaulted;			// true if the default image was generated because a file couldn't be loaded
	ID_TIME_T			   sourceFileTime;		// the most recent of all images used in creation, for reloadImages command
	ID_TIME_T			   binaryFileTime;		// the time stamp of the binary file

	int					   refCount; // overall ref count

	static const uint32	   TEXTURE_NOT_LOADED = 0xFFFFFFFF;

	nvrhi::TextureHandle   texture;
	nvrhi::SamplerHandle   sampler;
	nvrhi::SamplerDesc	   samplerDesc;

#if defined( USE_AMD_ALLOCATOR )
	VkImage						 image;
	VmaAllocation				 allocation;

	static int					 garbageIndex;
	static idList<VkImage>		 imageGarbage[NUM_FRAME_DATA];
	static idList<VmaAllocation> allocationGarbage[NUM_FRAME_DATA];
#endif
};

/*!
	\brief Loads an RGBA8 image from a file using stb_image and allocates static memory for the image data

	This function loads an image file in RGBA8 format using the stb_image library. It reads the file into memory, decodes it as an RGBA image with 4 channels, and allocates static memory for the
   output image data. The function handles cases where only the file timestamp is needed by checking if the pic parameter is NULL. It also includes error handling for cases where the image cannot be
   decoded by stb_image, logging a warning message in such cases. The image data is allocated using R_StaticAlloc() to ensure proper memory management within the engine's allocation system.

	\param name File path to the image file to load
	\param pic Pointer to a pointer where the image data will be stored
	\param width Pointer to store the width of the loaded image
	\param height Pointer to store the height of the loaded image
	\param timestamp Pointer to store the file modification timestamp
*/
void LoadSTB_RGBA8( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp );

/*!
	\brief Loads a Targa image file and decompresses it into a raw RGBA pixel buffer

	This function reads a Targa image file from the filesystem and decodes it into a raw RGBA pixel buffer. It supports uncompressed RGB, grayscale, and runlength encoded RGB images. The function
   handles both 24-bit and 32-bit pixel formats, with the latter including an alpha channel. The loaded image data is allocated using a static allocator and can be flipped vertically if the image file
   has a flip attribute flag. The function updates the provided width and height parameters with the image dimensions and optionally retrieves the file timestamp

	\param name Name of the Targa image file to load
	\param pic Pointer to store the resulting pixel data buffer
	\param width Pointer to store the image width
	\param height Pointer to store the image height
	\param timestamp Pointer to store the file's last modification time
	\return NULL if the file fails to load or an error occurs during processing, otherwise the function sets the pic parameter to point to the loaded pixel data
	\throws Error message if the Targa file format is unsupported, invalid, or corrupted
*/
void LoadTGA( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp );

/*!
	\brief Writes image data to a TGA file with optional vertical flipping and base path specification

	This function creates a TGA (Truevision Advanced Raster Graphics Adapter) file from raw image data. It handles the conversion of RGB data to BGR format required by the TGA specification, and
   supports flipping the image vertically. The function allocates a buffer large enough to hold the TGA header and pixel data, then writes the complete file to the specified base path using the file
   system interface. The TGA file will be uncompressed with 32 bits per pixel (RGBA format).

	\param filename Name of the output TGA file
	\param data Pointer to the source image data in RGBA format
	\param width Width of the image in pixels
	\param height Height of the image in pixels
	\param flipVertical If true, the image will be vertically flipped
	\param basePath Base path for file output, defaults to fs_savepath
*/
void R_WriteTGA( const char* filename, const byte* data, int width, int height, bool flipVertical = false, const char* basePath = "fs_savepath" );

/*!
	\brief Writes image data to a PNG file using the provided parameters

	This function saves image data in PNG format to the specified file path. It validates that the number of bytes per pixel is either 3 or 4, and reports an error if not. The function uses the STBI
   library to write the PNG data. The output file is created relative to the specified base path, which defaults to fs_savepath if not provided. The function handles file opening errors gracefully by
   printing a message and returning without writing.

	\param filename The name of the file to write the PNG data to
	\param data Pointer to the raw image data to be written
	\param bytesPerPixel Number of bytes per pixel (must be 3 or 4)
	\param width Width of the image in pixels
	\param height Height of the image in pixels
	\param basePath Base path for the output file, defaults to fs_savepath
	\throws Error if bytesPerPixel is not 3 or 4, or if the file cannot be opened for writing
*/
void R_WritePNG( const char* filename, const byte* data, int bytesPerPixel, int width, int height, const char* basePath = "fs_savepath" );

/*!
	\brief Writes image data to an OpenEXR file with the specified parameters

	This function writes image data to an OpenEXR file format. It supports writing RGB data with half-precision floating-point values. The function handles both uncompressed and compressed output,
   with the latter being used by default to save disk space. It requires the image width and height to be valid and supports only 3 channels per pixel (RGB). The function uses TinyEXR library for the
   actual file creation and compression.

	\param filename The name of the file to write the EXR data to
	\param data Pointer to the image data in half-precision floating-point format
	\param channelsPerPixel Number of channels per pixel (must be 3 for RGB)
	\param width Width of the image in pixels
	\param height Height of the image in pixels
	\param basePath Base path for the file system, defaults to fs_savepath
	\throws Throws an error if channelsPerPixel is not 3
*/
void R_WriteEXR( const char* filename, const void* data, int channelsPerPixel, int width, int height, const char* basePath = "fs_savepath" );

/*!
	\class idImageManager
	\brief Manages loading, creation, and caching of image resources for the rendering system.

	The idImageManager class serves as the central registry for all image resources used by the rendering system. It handles loading images from files, generating images through callback functions,
   and managing scratch images for internal renderer operations. The class supports different texture filtering, repeating, and usage parameters to optimize image handling for various rendering
   scenarios. It provides mechanisms for preloading images, managing memory through purging and reloading operations, and tracking image references during level loading. The manager maintains a hash
   table of loaded images and ensures proper resource cleanup and allocation. It also supports special cases for default images, font textures, and light textures while managing cube map states and
   caching behavior. The class integrates with the graphics API through command lists for deferred image loading and provides functionality for debugging image memory usage.

*/
class idImageManager
{
	friend class idImage;

public:
	//! Initializes a new instance of the idImageManager class.
	idImageManager()
	{
		insideLevelLoad		= false;
		preloadingMapImages = false;
		cacheImages			= false;
		commandList			= nullptr;
	}

	//! Initializes the image manager and sets up necessary resources and commands.
	void								   Init();

	//! Shuts down the image manager and releases all allocated resources.
	void								   Shutdown();

	/*!
		\brief Retrieves or creates an image resource from a file, using specified filtering, repeating, and usage parameters.

		This function attempts to find an already-loaded image with the given name and parameters. If the image is not found or has conflicting parameters, a new image is created and loaded. The
	   function handles special cases for default images, font and light textures, and ensures proper loading behavior during level preloading. It also manages the image's usage flags and caching
	   state appropriately.

		\param name Name of the image file to load, or a special identifier like default or _default
		\param filter Texture filtering mode to apply to the image
		\param repeat Texture repeat behavior for the image
		\param usage Usage type of the texture affecting compression and swizzling
		\param cubeMap Cube map file type specifier, defaults to 2D
		\param cubeMapSize Size of the cube map if applicable, defaults to 0
		\return Pointer to an idImage object representing the requested image, or the default image if the file name is invalid or reserved
		\throws May throw an error if an image with conflicting cube map states is encountered
	*/
	idImage*							   ImageFromFile( const char* name, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, cubeFiles_t cubeMap = CF_2D, int cubeMapSize = 0 );

	//! Returns a loaded image by name, or the default image if the name is null, empty, or 'default'.
	idImage*							   GetImage( const char* name ) const;

	// look for a loaded image, whatever the parameters
	idImage*							   GetImageWithParameters( const char* name, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, cubeFiles_t cubeMap ) const;

	//! Creates or retrieves an image generated by a callback function, using the specified name.
	idImage*							   ImageFromFunction( const char* name, ImageGeneratorFunction generatorFunction );

	/*!
		\brief Creates or retrieves a scratch image with the specified parameters, used for internal renderer purposes

		This function manages scratch images that are intended for internal renderer use and should always begin with an underscore. It first checks if an image with the given name already exists in
	   the image hash table. If found, it verifies that the existing image matches the specified filter, repeat, and usage parameters. If the image doesn't match or if it's a built-in image (starting
	   with underscore), it returns the existing image. Otherwise, it creates a new image using the provided image options, filter, and repeat settings. For cubic textures, it enforces clamp repeat
	   mode to avoid border seam issues. The function also ensures that the image is properly allocated with the given parameters.

		\param name name of the image to create or retrieve
		\param imgOpts options for the image to be created
		\param filter texture filtering mode for the image
		\param repeat texture repeat mode for the image
		\param usage usage type for the image that affects compression and swizzling
		\return pointer to the created or retrieved scratch image
		\throws idLib::FatalError if called with an empty name or NULL imgOpts
	*/
	idImage*							   ScratchImage( const char* name, idImageOpts* imgOpts, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage );

	//! Allocates or reuses an image with the specified name and options for internal renderer use
	idImage*							   ScratchImage( const char* name, const idImageOpts& opts );

	//! Purges all loaded images before a video restart.
	void								   PurgeAllImages();

	//! Reloads all appropriate images after a video restart.
	void								   ReloadImages( bool all, nvrhi::ICommandList* commandList );

	//! Initializes the image manager for a new level load operation by purging unreferenced images.
	void								   BeginLevelLoad();

	//! Marks the end of a level load operation and prepares the image manager to free unreferenced media.
	void								   EndLevelLoad();

	//! Preloads image resources specified in a preload manifest for the current map.
	void								   Preload( const idPreloadManifest& manifest, const bool& mapPreload );

	//! Loads unloaded level images for the current level.
	int									   LoadLevelImages( bool pacifier );

	//! Writes image memory usage information to a file sorted by storage size.
	void								   PrintMemInfo( MemInfo_t* mi );

	//! Loads deferred images into GPU memory using the provided command list.
	void								   LoadDeferredImages( nvrhi::ICommandList* commandList = nullptr );

	//! Creates built-in intrinsic images used by the rendering system.
	void								   CreateIntrinsicImages();
	idImage*							   defaultImage;
	idImage*							   flatNormalMap;	  // 128 128 255 in all pixels
	idImage*							   alphaNotchImage;	  // 2x1 texture with just 1110 and 1111 with point sampling
	idImage*							   whiteImage;		  // full of 0xff
	idImage*							   blackImage;		  // full of 0x00
	idImage*							   blackDiffuseImage; // full of 0x00
	idImage*							   cyanImage;		  // cyan
	idImage*							   redClayImage;	  // dark red
	idImage*							   noFalloffImage;	  // all 255, but zero clamped
	idImage*							   fogImage;		  // increasing alpha is denser fog
	idImage*							   fogEnterImage;	  // adjust fogImage alpha based on terminator plane
	// RB begin
	idImage*							   shadowAtlasImage; // 8192 * 8192 for clustered forward shading
	idImage*							   shadowImage[5];
	idImage*							   jitterImage1; // shadow jitter
	idImage*							   jitterImage4;
	idImage*							   jitterImage16;
	idImage*							   grainImage1;
	idImage*							   randomImage256;
	idImage*							   blueNoiseImage256;
	idImage*							   currentRenderHDRImage;
	idImage*							   ldrImage;				 // tonemapped result which can be used for further post processing
	idImage*							   taaMotionVectorsImage[2]; // motion vectors for TAA projection, doubled for VR
	idImage*							   taaFeedback1Image[2];
	idImage*							   taaFeedback2Image[2];
	idImage*							   taaResolvedImage;
	idImage*							   bloomRenderImage[2];
	idImage*							   envprobeHDRImage;
	idImage*							   envprobeDepthImage;
	idImage*							   heatmap5Image;
	idImage*							   heatmap7Image;
	idImage*							   smaaInputImage;
	idImage*							   smaaAreaImage;
	idImage*							   smaaSearchImage;
	idImage*							   smaaEdgesImage;
	idImage*							   smaaBlendImage;
	idImage*							   gbufferNormalsRoughnessImage; // cheap G-Buffer replacement, holds normals and surface roughness
	idImage*							   ambientOcclusionImage[2];	 // contain AO and bilateral filtering keys
	idImage*							   hierarchicalZbufferImage;	 // zbuffer with mip maps to accelerate screen space ray tracing
	idImage*							   imguiFontImage;

	idImage*							   chromeSpecImage;	 // only for the PBR color checker chart
	idImage*							   plasticSpecImage; // only for the PBR color checker chart
	idImage*							   brdfLutImage;
	idImage*							   defaultUACIrradianceCube;
	idImage*							   defaultUACRadianceCube;

	idImage*							   vrPDAImage; // copy target of LDR FBO for displaying menus and PDA onto the PDA model
	idImage*							   vrHUDImage; // copy target of LDR FBO for displaying onto the 3D HUD model
	idImage*							   stereoRenderImages[2];
	idImage*							   hmdEyeImages[2];
	// RB end
	idImage*							   scratchImage;
	idImage*							   scratchImage2;
	idImage*							   accumImage;
	idImage*							   currentRenderImage;		   // for 3D scene SS_POST_PROCESS shaders for effects like heatHaze, in HDR now
	idImage*							   currentDepthImage;		   // for motion blur, SSAO and everything that requires depth to world pos reconstruction
	idImage*							   originalCurrentRenderImage; // currentRenderImage before any changes for stereo rendering
	idImage*							   loadingIconImage;		   // loading icon must exist always
	idImage*							   hellLoadingIconImage;	   // loading icon must exist always
	idImage*							   guiEdit;					   // SP: GUI editor image
	idImage*							   guiEditDepthStencilImage;   // SP: Gui-editor image depth-stencil

	//! Allocates and returns a new image with the specified name
	idImage*							   AllocImage( const char* name );

	//! Allocates and returns a new standalone image with the specified name
	idImage*							   AllocStandaloneImage( const char* name );

	//! Determines whether an image should be excluded from preloading based on its path
	bool								   ExcludePreloadImage( const char* name );

	idList<idImage*, TAG_IDLIB_LIST_IMAGE> images;

private:
	idHashIndex							   imageHash;

	//! Caches global illumination data from EXR files into bimage format.
	static void							   CacheGlobalIlluminationData_f( const idCmdArgs& args );

	//! Lists image resources with optional filtering and sorting options.
	static void							   R_ListImages_f( const idCmdArgs& args );

	// Transient list of images to load on the main thread to the gpu. Freed after images are loaded.
	idList<idImage*, TAG_IDLIB_LIST_IMAGE> imagesToLoad;

	bool								   insideLevelLoad;		// don't actually load images now
	bool								   preloadingMapImages; // unless this is set
	bool								   cacheImages;			// similar to preload but surpresses prints

	nvrhi::CommandListHandle			   commandList;
};

extern idImageManager* globalImages; // pointer to global list for the rest of the system

/*!
	\brief Resamples an input image to a specified output width and height using a simple drop-sampling algorithm

	This function performs image resampling by dropping samples from the input image to match the target dimensions. It allocates memory for the output image and iterates through each pixel of the
   output image, mapping it to the corresponding pixel in the input image using a simple scaling algorithm. The function is commonly used in image processing operations where images need to be resized
   while maintaining the original pixel data

	\param in Pointer to the input image data in RGBA format
	\param inwidth Width of the input image
	\param inheight Height of the input image
	\param outwidth Target width for the output image
	\param outheight Target height for the output image
	\return Pointer to the newly allocated output image data in RGBA format with the specified dimensions
*/
byte*				   R_Dropsample( const byte* in, int inwidth, int inheight, int outwidth, int outheight );

/*!
	\brief Resamples a texture from input dimensions to output dimensions using bilinear interpolation

	This function performs texture resampling by taking an input texture buffer and resizing it to specified output dimensions. It uses a bilinear interpolation algorithm that samples four neighboring
   pixels from the input texture for each output pixel. The implementation handles the conversion from input dimensions to output dimensions by calculating appropriate fractional steps for pixel
   interpolation. The function allocates memory for the output texture using R_StaticAlloc and returns a pointer to the newly created resampled texture data. The resampling algorithm is designed to
   maintain quality while scaling textures, with special handling for edge cases where the output dimensions exceed MAX_DIMENSION.

	\param in Pointer to the input texture data in RGBA format
	\param inwidth Width of the input texture
	\param inheight Height of the input texture
	\param outwidth Width of the output texture
	\param outheight Height of the output texture
	\return Pointer to the newly allocated resampled texture data in RGBA format
*/
byte*				   R_ResampleTexture( const byte* in, int inwidth, int inheight, int outwidth, int outheight );

//! Computes a mipmap with alpha specularity for a given texture
byte*				   R_MipMapWithAlphaSpecularity( const byte* in, int width, int height );

//! Returns a new copy of the texture, quartered in size with gamma correction.
byte*				   R_MipMapWithGamma( const byte* in, int width, int height );

//! Returns a new copy of the texture, quartered in size and filtered.
byte*				   R_MipMap( const byte* in, int width, int height );

//! Generates a mipmap level by downsampling R11G11B10F format texture data.
byte*				   R_MipMapR11G11B10F( const byte* in, int width, int height );

//! Applies a blending operation to an array of pixel data using pre-multiplied alpha values.
void				   R_BlendOverTexture( byte* data, int pixelCount, const byte blend[4] );

//! Flips the image data horizontally in place
void				   R_HorizontalFlip( byte* data, int width, int height );

//! Vertically flips the pixel data of an image in place.
void				   R_VerticalFlip( byte* data, int width, int height );

//! Vertically flips the RGB16F image data in place.
void				   R_VerticalFlipRGB16F( byte* data, int width, int height );

//! Rotates and horizontally flips image data by 90 degrees
void				   R_RotatePic( byte* data, int width );

//! Applies cube map transforms to image data based on the iteration index
void				   R_ApplyCubeMapTransforms( int i, byte* data, int size );

/*!
	\brief Extracts and returns a single side of a cube map from a combined cube map image.

	This function takes a single image containing all six sides of a cube map and extracts one specific side based on the side parameter. The input image is expected to contain six cube map sides
   arranged in a specific layout. The function calculates the correct region in the input image based on the side index and copies the corresponding pixels into a newly allocated output buffer. The
   function supports sides 0 through 5, corresponding to negative Z, positive Z, negative X, positive X, positive Y, and negative Y respectively. For invalid side indices, the function returns nullptr
   and logs a warning.

	\param in Pointer to the input image data containing all six cube map sides
	\param srcWidth Width of the source image in pixels
	\param srcHeight Height of the source image in pixels
	\param size Size of each individual cube map side in pixels
	\param side Index of the cube map side to extract (0-5)
	\return Pointer to the newly allocated output buffer containing the extracted cube map side, or nullptr if an invalid side index is provided
*/
byte*				   R_GenerateCubeMapSideFromSingleImage( const byte* in, int srcWidth, int srcHeight, int size, int side );

/*!
	\brief Generates a single cube map side from a panorama image by mapping each pixel to the corresponding cubemap face using spherical coordinates and a transformation matrix

	This function takes a panorama image and generates one side of a cube map by performing coordinate transformations from cubemap space to lat-long space and then mapping back to the source panorama
   image. The transformation includes a specific rotation matrix to align the coordinate systems properly. Each pixel in the output cube map side is sampled from the panorama image using the
   calculated source coordinates, with bilinear filtering implicitly handled by the nearest-neighbor sampling. The function allocates memory for the output cube map side and returns a pointer to it.

	\param in Pointer to the source panorama image data
	\param srcWidth Width of the source panorama image
	\param srcHeight Height of the source panorama image
	\param size Width and height of the output cube map side
	\param side Index of the cube map side to generate (0-5)
	\return Pointer to the generated cube map side image data
*/
byte*				   R_GenerateCubeMapSideFromPanoramaImage( const byte* in, int srcWidth, int srcHeight, int size, int side );

//! Calculates the UV rectangle for a specified mip level within a texture atlas.
idVec4				   R_CalculateMipRect( uint dimensions, uint mip );

//! Calculates the total number of pixels used in all mipmap levels for a given texture dimensions.
int					   R_CalculateUsedAtlasPixels( int dimensions );

/*!
	\brief Loads an image file and returns its pixel data, dimensions, and timestamp, with support for PBR texture hacks.

	This function loads an image file specified by name and retrieves its pixel data, width, height, and timestamp. It handles various image file extensions and supports a special PBR hack where
   textures with _rmao suffix are preferred for specular maps. The function also supports an option to convert image dimensions to power-of-two sizes, though this functionality is currently commented
   out. If the image cannot be loaded, the function will attempt to fall back to other formats if available.

	\param name The name of the image file to load
	\param pic Pointer to a byte pointer that will store the loaded image data
	\param width Pointer to an integer that will store the width of the loaded image
	\param height Pointer to an integer that will store the height of the loaded image
	\param timestamp Pointer to a timestamp value that will store the last modified time of the file
	\param makePowerOf2 Flag indicating whether to convert image dimensions to power-of-two sizes
	\param usage Pointer to a textureUsage_t value that will indicate the texture usage type
*/
void				   R_LoadImage( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp, bool makePowerOf2, textureUsage_t* usage );

/*!
	\brief Loads cube map images from files with specified extensions and returns the image data in a six-element array.

	This function loads six images that constitute a cube map, based on the provided base name and cube map file extensions. It supports multiple file naming conventions including Quake-style,
   camera-style, and axis-style. The function handles loading of individual images, panorama-based cube maps, and single image to cube map conversion. It returns the size of the cube map and updates
   the timestamp with the latest modification time of the loaded images. The loaded images are stored in top-to-bottom raster format.

	\param cname Base name of the cube map files
	\param extensions Type of cube map file naming convention to use
	\param pic Array of 6 pointers to store the loaded image data for each face
	\param size Pointer to store the size of the cube map
	\param timestamp Pointer to store the latest file modification time
	\param cubeMapSize Size of cube map faces when converting from single image or panorama
	\return True if all cube map faces were successfully loaded, false otherwise
*/
bool				   R_LoadCubeImages( const char* cname, cubeFiles_t extensions, byte* pic[6], int* size, ID_TIME_T* timestamp, int cubeMapSize = 0 );

/*!
	\brief Loads and parses an image program file, extracting texture data and metadata.

	This function processes an image program file by first creating a lexer to parse the file contents. It initializes the parse buffer and sets up the lexer with specific flags to control parsing
   behavior. The function then calls the recursive parsing function to extract the actual image data and metadata such as width, height, and timestamp. Finally, it cleans up the lexer resources.

	\param name Name of the image program file to load.
	\param pic Pointer to store the loaded image data.
	\param width Pointer to store the width of the loaded image.
	\param height Pointer to store the height of the loaded image.
	\param timestamp Pointer to store the timestamp of the image file, or NULL if not needed.
	\param usage Pointer to store the texture usage flags, or NULL if not needed.
*/
void				   R_LoadImageProgram( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp, textureUsage_t* usage = NULL );

//! Parses and returns the name of an image program from the given lexer
const char*			   R_ParsePastImageProgram( idLexer& src );

#endif
