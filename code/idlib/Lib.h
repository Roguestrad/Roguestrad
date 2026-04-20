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

#ifndef __LIB_H__
#define __LIB_H__

#include <stddef.h>

/*
===============================================================================

	idLib contains stateless support classes and concrete types. Some classes
	do have static variables, but such variables are initialized once and
	read-only after initialization (they do not maintain a modifiable state).

	The interface pointers idSys, idCommon, idCVarSystem and idFileSystem
	should be set before using idLib. The pointers stored here should not
	be used by any part of the engine except for idLib.

===============================================================================
*/

class idLib
{
private:
	static bool	  mainThreadInitialized;
	static ID_TLS isMainThread;

public:
	static class idSys*		   sys;
	static class idCommon*	   common;
	static class idCVarSystem* cvarSystem;
	static class idFileSystem* fileSystem;
	static int				   frameNumber;

	/*!
		\brief Initializes the core libraries and subsystems used by the engine.

		This function performs essential initialization tasks for the engine's core systems. It sets up thread-local variables to identify the main thread, initializes endian conversion utilities,
	   configures string memory allocation, and initializes the SIMD implementation. Additionally, it initializes mathematical libraries and performs optional testing of polynomial functions in debug
	   builds. Finally, it initializes dictionary string pools for efficient string management.

	*/
	static void				   Init();

	/*!
		\brief Shuts down the string pools, memory allocator, and SIMD engine.

		This function is responsible for cleaning up and shutting down various core components of the idLib library. It first shuts down the dictionary string pools, then the string memory allocator,
	   and finally the SIMD engine. This ensures that all allocated resources are properly deallocated and that the system is left in a clean state.

	*/
	static void				   ShutDown();

	/*!
		\brief Prints a formatted message to the console using a variadic argument list

		This function serves as a wrapper to the idCommon::VPrintf function, providing a formatted output to the console. It accepts a format string and a variable number of arguments, which are then
	   processed and displayed according to the format specification. The actual printing is delegated to the common->VPrintf function. This function is commonly used for debug output and general
	   messaging within the engine, particularly when specific conditions are met as shown in the call examples.

		\param fmt Format string specifying how to format the output
	*/
	static void				   Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	/*!
		\brief Prints a formatted message to the console if the specified test condition is true

		This function provides a conditional printf implementation that only outputs information when a boolean test evaluates to true. It uses variadic arguments to accept a format string and any
	   additional arguments needed for formatting. The actual printing is delegated to the common->VPrintf function which handles the core formatting and output operations. This is useful for debug
	   output that should only appear when specific conditions are met, helping to reduce noise in regular operation while maintaining visibility during development or debugging sessions.

		\param test Boolean condition that determines whether to print the message
		\param fmt Format string specifying how to format the output
	*/
	static void				   PrintfIf( const bool test, VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 2, 3 );

	/*!
		\brief Reports an error with a formatted message and terminates the program.

		This function takes a format string and a variable number of arguments to construct an error message. It then passes this message to the common error handling system. The function is marked as
	   no-return and will terminate the program after reporting the error. On non-Windows platforms, it explicitly calls exit to ensure the function does not return.

		\param fmt A format string specifying how to format the error message, followed by a variable number of arguments
	*/
	NO_RETURN static void	   Error( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	/*!
		\brief Terminates the program with a formatted error message

		This function accepts a format string and a variable number of arguments, formats them into a single string, and then passes that string to the common error system for display. It is used
	   throughout the codebase to report fatal errors that cause the program to terminate. The formatted string is limited to MAX_STRING_CHARS in length. It uses va_list to handle the variadic
	   arguments and idStr::vsnPrintf for safe string formatting.

		\param fmt Format string specifying how to interpret the following arguments
	*/
	NO_RETURN static void	   FatalError( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	/*!
		\brief Outputs a formatted warning message to the common warning system

		This function accepts a format string and a variable number of arguments, formats them into a single string, and then passes that string to the common warning system for display. It is used
	   throughout the codebase to report non-fatal issues or potential problems. The formatted string is limited to MAX_STRING_CHARS in length. It uses va_list to handle the variadic arguments and
	   idStr::vsnPrintf for safe string formatting.

		\param fmt Format string specifying how to interpret the following arguments
	*/
	static void				   Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	/*!
		\brief Issues a warning message if the specified condition is true.

		This function checks the given boolean test condition. If the condition evaluates to true, it formats a warning message using the provided format string and variable arguments, then outputs
	   the message through the common warning system. If the condition is false, the function returns without issuing any warning.

		\param test Boolean condition that determines whether to issue a warning
		\param fmt Format string for the warning message, followed by variable arguments
	*/
	static void				   WarningIf( const bool test, VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 2, 3 );

	/*!
		\brief Checks whether the current thread is the main thread

		The function determines if the calling thread is the main thread by evaluating the mainThreadInitialized and isMainThread flags. It returns true if the current thread is the main thread, and
	   false otherwise. The extra check for mainThreadInitialized is necessary to ensure accuracy when called during startup code that occurs before idLib::Init is invoked.

		\return true if the current thread is the main thread, false otherwise
	*/
	static bool				   IsMainThread() { return ( 0 == mainThreadInitialized ) || ( 1 == isMainThread ); }
};

/*
===============================================================================

	Types and defines used throughout the engine.

===============================================================================
*/

typedef int qhandle_t;

class idFile;
class idVec3;
class idVec4;

#ifndef NULL
	#define NULL ( ( void* )0 )
#endif

#ifndef BIT
	#define BIT( num ) ( 1ULL << ( num ) )
#endif

#define MAX_STRING_CHARS 1024  // max length of a string
#define MAX_PRINT_MSG	 16384 // buffer size for our various printf routines

// maximum world size
#define MAX_WORLD_COORD	 ( 128 * 1024 )
#define MIN_WORLD_COORD	 ( -128 * 1024 )
#define MAX_WORLD_SIZE	 ( MAX_WORLD_COORD - MIN_WORLD_COORD )

#define SIZE_KB( x )	 ( ( ( x ) + 1023 ) / 1024 )
#define SIZE_MB( x )	 ( ( ( SIZE_KB( x ) ) + 1023 ) / 1024 )
#define SIZE_GB( x )	 ( ( ( SIZE_MB( x ) ) + 1023 ) / 1024 )

// Color declarations (alphabetical order)
extern const idVec4 colorAqua;
extern const idVec4 colorBlack;
extern const idVec4 colorBlue;
extern const idVec4 colorBrown;
extern const idVec4 colorBurlyWood;
extern const idVec4 colorCoral;
extern const idVec4 colorCrimson;
extern const idVec4 colorCyan;
extern const idVec4 colorDarkBlue;
extern const idVec4 colorDarkCyan;
extern const idVec4 colorDarkGoldenRod;
extern const idVec4 colorDarkKhaki;
extern const idVec4 colorDarkSalmon;
extern const idVec4 colorDarkSlateGray;
extern const idVec4 colorDkGrey;
extern const idVec4 colorDodgerBlue;
extern const idVec4 colorFuchsia;
extern const idVec4 colorGold;
extern const idVec4 colorGray;
extern const idVec4 colorGreen;
extern const idVec4 colorLightSeaGreen;
extern const idVec4 colorLightSteelBlue;
extern const idVec4 colorLtGrey;
extern const idVec4 colorLime;
extern const idVec4 colorMagenta;
extern const idVec4 colorMaroon;
extern const idVec4 colorMdGrey;
extern const idVec4 colorNavy;
extern const idVec4 colorOlive;
extern const idVec4 colorOrange;
extern const idVec4 colorPink;
extern const idVec4 colorPurple;
extern const idVec4 colorRed;
extern const idVec4 colorSilver;
extern const idVec4 colorTeal;
extern const idVec4 colorWhite;
extern const idVec4 colorYellow;

/*!
	\brief Packs RGB color components from a vector into a 32-bit integer

	This function takes a color vector with components in the range [0,1] and packs them into a 32-bit integer value. Each component is scaled by 255.0f and converted to a byte, with the red component
   stored in the least significant 8 bits, green in the next 8 bits, and blue in the next 8 bits. The alpha component is not included in the output.

	\param color A vector containing RGB color values in the range [0,1]
	\return A 32-bit integer value with the RGB components packed in the lower 24 bits
*/
dword				PackColor( const idVec3& color );

/*!
	\brief Unpacks a 32-bit color value into a 3D vector with RGB components normalized to the range [0, 1].

	This function takes a 32-bit color value where the RGB components are packed in the lower 24 bits, with red in the least significant 8 bits, green in the next 8 bits, and blue in the most
   significant 8 bits. It extracts these components and normalizes them to floating-point values in the range [0, 1] to form a 3D vector representing the color.

	\param color The 32-bit color value to unpack, with RGB components in the lower 24 bits.
	\param unpackedColor The output 3D vector that will contain the normalized RGB components.
*/
void				UnpackColor( const dword color, idVec3& unpackedColor );

/*!
	\brief Packs a four-component color vector into a 32-bit dword value

	This function takes a color vector with components in the range 0.0 to 1.0 and packs them into a 32-bit unsigned integer. Each color component is scaled by 255.0 and converted to an 8-bit byte
   value. The components are then arranged in little-endian byte order within the dword, with the x component in the least significant byte and the w component in the most significant byte.

	\param color A four-component color vector with values typically in the range 0.0 to 1.0
	\return A 32-bit unsigned integer value representing the packed color with components arranged in RGBA byte order
*/
dword				PackColor( const idVec4& color );

/*!
	\brief Unpacks a packed 32-bit color value into a four-component vector with normalized floating-point components

	This function takes a 32-bit color value where each component is stored in 8 bits and unpacks it into an idVec4 structure. The color components are expected to be in the order: alpha, blue, green,
   red. Each component is normalized from the range 0-255 to 0.0-1.0.

	\param color A 32-bit packed color value where each component occupies 8 bits
	\param unpackedColor The output vector that will contain the unpacked color components as floating-point values
*/
void				UnpackColor( const dword color, idVec4& unpackedColor );

//! Converts a short integer from little-endian to big-endian byte order or vice versa.
short				BigShort( short l );

//! Converts a short value from big-endian to little-endian byte order.
short				LittleShort( short l );

//! Converts an integer from little-endian to big-endian byte order.
int					BigLong( int l );

//! Converts a 32-bit integer from little-endian to native byte order.
int					LittleLong( int l );

//! Returns the big float representation of the given float value.
float				BigFloat( float l );

//! Converts a float value from little-endian byte order to the native byte order.
float				LittleFloat( float l );

/*!
	\brief Reverses the byte order of elements in a buffer for big-endian systems

	This function performs byte order reversal on a buffer of elements, typically used when converting data between different endianness formats. It takes a buffer pointer, the size of each element,
   and the number of elements to process. The implementation delegates to an internal helper function _BigRevBytes to perform the actual byte reversal operation.

	\param bp pointer to the buffer containing elements to reverse
	\param elsize size in bytes of each individual element
	\param elcount number of elements in the buffer to process
*/
void				BigRevBytes( void* bp, int elsize, int elcount );

/*!
	\brief Reverses the byte order of elements in a buffer for little-endian operations

	This function reverses the byte order of elements in a buffer to ensure proper little-endian bit field operations. It prepares the buffer for operations by delegating to an internal helper
   function that handles the actual byte reversal. The function takes a buffer pointer, the size of each element in bytes, and the number of elements to process. It is typically used when working with
   binary data that needs to be interpreted in little-endian format.

	\param bp Pointer to the buffer containing the data to be reversed
	\param elsize Size in bytes of each element in the buffer
	\param elcount Number of elements in the buffer to process
*/
void				LittleRevBytes( void* bp, int elsize, int elcount );

/*!
	\brief Initializes a bit field structure for little-endian bit field operations.

	This function sets up the necessary internal state for performing bit field operations on the provided buffer. It prepares the bit field structure to handle operations on elements of the specified
   size. The function delegates the actual implementation to the internal helper function _LittleBitField.

	\param bp Pointer to the buffer to be used for bit field operations
	\param elsize Size of the elements in the bit field in bytes
*/
void				LittleBitField( void* bp, int elsize );

/*!
	\brief Initializes byte swapping functions based on the endianness of the current system.

	This function determines whether the system uses little-endian or big-endian byte order by testing a short integer value. Depending on the result, it sets appropriate swapping functions for short
   integers, long integers, floats, and other data types to ensure proper byte order handling across different architectures.

*/
void				Swap_Init();

/*!
	\brief Checks whether the current system uses big-endian byte order.

	The function determines the endianness of the system by testing a short integer value. It creates a byte array with values 1 and 0, then casts it to a short pointer and checks if the resulting
   short value is equal to 1. If the system is big-endian, the byte order will cause the short value to be interpreted as 256 instead of 1, and thus the function returns false. If the system is
   little-endian, the short value will be 1, and the function will return true.

	\return true if the system is little-endian, false if it is big-endian
*/
bool				Swap_IsBigEndian();

/*!
	\brief Encodes an integer into six 6-bit values stored in a byte array for base64 encoding.

	This function takes an integer input and converts it into six 6-bit values, which are stored in the provided byte array. These values represent the base64 encoding of the input integer. The
   function is a wrapper around the internal _SixtetsForInt function and is used to prepare data for base64 encoding operations.

	\param out Pointer to a byte array where the six 6-bit values will be stored
	\param src The integer value to be encoded into six 6-bit values
*/
void				SixtetsForInt( byte* out, int src );

//! Converts a byte array into an integer value.
int					IntForSixtets( byte* in );

/*
================================================
idException
================================================
*/
class idException
{
public:
	static const int MAX_ERROR_LEN = 2048;

	/*!
		\brief Constructs an idException object with the specified error text.

		The constructor initializes the exception object with the provided error text. If no text is provided, it initializes the error message to an empty string. The error text is copied into an
	   internal buffer with a maximum length of MAX_ERROR_LEN characters.

		\param text The error text to initialize the exception with, or an empty string if no text is provided
	*/
	idException( const char* text = "" ) { strncpy( error, text, MAX_ERROR_LEN ); }

	/*!
		\brief Returns the error message stored in the exception object

		This function provides access to the error message that was set when the exception was created. It is a const function that safely returns the internal error string without modifying the
	   exception object. The function is referenced in many places throughout the codebase, which is why it cannot currently be marked as const.

		\return A pointer to a null-terminated string containing the error message
	*/
	const char* GetError() { return error; }

protected:
	/*!
		\brief Returns a pointer to the error buffer used for storing exception messages.

		This function provides access to the internal error buffer where exception messages are stored. The returned pointer points to a character array that contains the error message associated with
	   the exception. The function is named GetErrorBuffer to avoid conflicts with a non-const version that might be intended for use in const contexts, as indicated by the original comment.

		\return Pointer to the character array containing the error message
	*/
	char* GetErrorBuffer() { return error; }

	/*!
		\brief Returns the maximum size of the error buffer used for exception messages.
		\return The maximum error buffer size in characters, defined by the MAX_ERROR_LEN constant
	*/
	int	  GetErrorBufferSize() { return MAX_ERROR_LEN; }

private:
	friend class idFatalException;
	static char error[MAX_ERROR_LEN];
};

/*
================================================
idFatalException
================================================
*/
class idFatalException
{
public:
	static const int MAX_ERROR_LEN = 2048;

	/*!
		\brief Constructs an idFatalException object with the specified error text.

		The constructor initializes the exception object by copying the provided error text into the internal error buffer. If no text is provided, it initializes the buffer with an empty string. The
	   error text is limited by the MAX_ERROR_LEN constant to prevent buffer overflows.

		\param text Error message to be stored in the exception object
	*/
	idFatalException( const char* text = "" ) { strncpy( idException::error, text, MAX_ERROR_LEN ); }

	/*!
		\brief Returns the error message stored in the exception

		This function provides access to the error message that was set within the exception object. It is a const function that safely retrieves the error string without modifying the exception
	   state. The function is primarily used in error handling scenarios where the specific error details need to be reported or logged.

		\return A pointer to a constant character string containing the error message
	*/
	const char* GetError() { return idException::error; }

protected:
	/*!
		\brief Returns the error buffer string from the exception.

		The function provides access to the internal error buffer that stores the error message associated with the exception. This buffer is typically populated when an exception is thrown and
	   contains the descriptive text of the error.

		\return A pointer to the character buffer containing the error message
	*/
	char* GetErrorBuffer() { return idException::error; }

	/*!
		\brief Returns the maximum size of the error buffer used for fatal exception handling.
		\return The maximum error buffer size defined by the MAX_ERROR_LEN constant
	*/
	int	  GetErrorBufferSize() { return MAX_ERROR_LEN; }
};

/*
================================================
idNetworkLoadException
================================================
*/
class idNetworkLoadException : public idException
{
public:
	/*!
		\brief Constructs an idNetworkLoadException object with an optional text message.

		This constructor initializes an idNetworkLoadException object by calling the base idException constructor with the provided text message. It allows for the creation of exception objects that
	   can carry descriptive information about network loading errors.

		\param text Optional text message describing the exception.
	*/
	idNetworkLoadException( const char* text = "" ) :
		idException( text )
	{
	}
};

/*
===============================================================================

	idLib headers.

===============================================================================
*/

// System
#include "sys/sys_assert.h"
#include "sys/sys_threading.h"

// memory management and arrays
#include "Heap.h"
#include "containers/Sort.h"
#include "containers/List.h"

// math
#include "math/Simd.h"
#include "math/Math.h"
#include "math/Random.h"
#include "math/Complex.h"
#include "math/Vector.h"
#include "math/VecX.h"
#include "math/VectorI.h"
#include "math/Matrix.h"
#include "math/MatX.h"
#include "math/Angles.h"
#include "math/Quat.h"
#include "math/Rotation.h"
#include "math/Plane.h"
#include "math/Pluecker.h"
#include "math/Polynomial.h"
#include "math/Extrapolate.h"
#include "math/Interpolate.h"
#include "math/Curve.h"
#include "math/Ode.h"
#include "math/Lcp.h"
#include "math/SphericalHarmonics.h"

// bounding volumes
#include "bv/Sphere.h"
#include "bv/Bounds.h"
#include "bv/Box.h"
#include "bv/BoxOctree.h"

// geometry
#include "geometry/RenderMatrix.h"
#include "geometry/JointTransform.h"
#include "geometry/DrawVert.h"
#include "geometry/Winding.h"
#include "geometry/Winding2D.h"
#include "geometry/Surface.h"
#include "geometry/Surface_Patch.h"
#include "geometry/Surface_Polytope.h"
#include "geometry/Surface_SweptSpline.h"
#include "geometry/TraceModel.h"

// text manipulation
#include "Str.h"
#include "StrStatic.h"
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Base64.h"
#include "CmdArgs.h"

// containers
#include "containers/Array.h"
#include "containers/BTree.h"
#include "containers/BinSearch.h"
#include "containers/HashIndex.h"
#include "containers/HashTable.h"
#include "containers/StaticList.h"
#include "containers/LinkList.h"
#include "containers/Hierarchy.h"
#include "containers/Queue.h"
#include "containers/Stack.h"
#include "containers/StrList.h"
#include "containers/StrPool.h"
#include "containers/VectorSet.h"
#include "containers/PlaneSet.h"
#include "containers/FlexList.h"

// hashing
#include "hashing/CRC32.h"
#include "hashing/MD4.h"
#include "hashing/MD5.h"

// misc
#include "Dict.h"
#include "LangDict.h"
#include "DataQueue.h"
#include "BitMsg.h"
#include "MapFile.h"
#include "Timer.h"
#include "Thread.h"
#include "Swap.h"
#include "Callback.h"
#include "ParallelJobList.h"
#include "SoftwareCache.h"
#include "TileMap.h" // RB

#endif /* !__LIB_H__ */
