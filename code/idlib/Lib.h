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

/*!
	\class idLib
	\brief Provides core library functionality for initialization, shutdown, and system logging.

	The idLib class serves as a central hub for initializing and shutting down core system components including string pools, memory allocators, and SIMD engines. It offers standardized methods for
   reporting messages, warnings, and errors with formatted output capabilities. The class also includes utility functions for thread identification and program termination with error reporting. This
   design intent supports consistent system behavior and reliable error handling throughout the application lifecycle.

	The interface pointers idSys, idCommon, idCVarSystem and idFileSystem
	should be set before using idLib. The pointers stored here should not
	be used by any part of the engine except for idLib.
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

	//! Initializes the idLib library components.
	static void				   Init();

	//! Shuts down the idLib system by cleaning up string pools, memory allocator, and SIMD engine.
	static void				   ShutDown();

	//! Prints a formatted string to the console
	static void				   Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	//! Prints a formatted message to the console if the specified test condition is true
	static void				   PrintfIf( const bool test, VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 2, 3 );

	//! Terminates the program execution and reports an error message formatted with the provided arguments.
	NO_RETURN static void	   Error( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	//! Terminates the program with a formatted error message.
	NO_RETURN static void	   FatalError( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	//! Reports a warning message to the common system with formatted output.
	static void				   Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );

	//! Conditionally issues a warning message using a format string and variable arguments.
	static void				   WarningIf( const bool test, VERIFY_FORMAT_STRING const char* fmt, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 2, 3 );

	//! Checks if the current thread is the main thread
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

//! Packs color float values in the range [0,1] into a 32-bit integer.
dword				PackColor( const idVec3& color );

//! Unpacks a 32-bit color value into a 3-component vector.
void				UnpackColor( const dword color, idVec3& unpackedColor );

//! Packs a vector4 color into a 32-bit dword value
dword				PackColor( const idVec4& color );

//! Unpacks a packed 32-bit color value into a vector of four floating-point components.
void				UnpackColor( const dword color, idVec4& unpackedColor );

//! Converts a short integer from little-endian to big-endian byte order.
short				BigShort( short l );

//! Converts a short value from big-endian to little-endian byte order.
short				LittleShort( short l );

//! Converts an integer value to big-endian byte order.
int					BigLong( int l );

//! Converts a 32-bit integer from little-endian byte order to native byte order.
int					LittleLong( int l );

//! Returns the big float value of the given float parameter.
float				BigFloat( float l );

//! Converts a float value from big-endian to little-endian byte order.
float				LittleFloat( float l );

//! Reverses the byte order of elements in a buffer.
void				BigRevBytes( void* bp, int elsize, int elcount );

//! Reverses the byte order of elements in a buffer.
void				LittleRevBytes( void* bp, int elsize, int elcount );

//! Initializes a little bit field structure with the specified element size.
void				LittleBitField( void* bp, int elsize );

//! Initializes byte swapping functions based on the platform's endianness.
void				Swap_Init();

//! Returns true if the system uses big-endian byte order.
bool				Swap_IsBigEndian();

//! Encodes an integer into sixtets for base64 encoding.
void				SixtetsForInt( byte* out, int src );

//! Converts a byte array into a 32-bit integer using sixtets.
int					IntForSixtets( byte* in );

/*!
	\class idException
	\brief idException provides a mechanism for handling error conditions with textual messages.
*/
class idException
{
public:
	static const int MAX_ERROR_LEN = 2048;

	//! Constructs an idException object with an optional error text.
	idException( const char* text = "" ) { strncpy( error, text, MAX_ERROR_LEN ); }

	//! Returns the error message stored in this exception object
	const char* GetError() { return error; }

protected:
	//! Returns the error buffer associated with the exception.
	char* GetErrorBuffer() { return error; }

	//! Returns the maximum size of the error buffer used for exception messages.
	int	  GetErrorBufferSize() { return MAX_ERROR_LEN; }

private:
	friend class idFatalException;
	static char error[MAX_ERROR_LEN];
};

/*!
	\class idFatalException
	\brief Exception class for handling fatal errors with error text storage and retrieval.
*/
class idFatalException
{
public:
	static const int MAX_ERROR_LEN = 2048;

	//! Constructs an idFatalException with the specified error text.
	idFatalException( const char* text = "" ) { strncpy( idException::error, text, MAX_ERROR_LEN ); }

	//! Returns the error message stored in the exception
	const char* GetError() { return idException::error; }

protected:
	//! Returns the error buffer associated with the fatal exception.
	char* GetErrorBuffer() { return idException::error; }

	//! Returns the maximum error buffer size for fatal exception handling.
	int	  GetErrorBufferSize() { return MAX_ERROR_LEN; }
};

/*!
	\class idNetworkLoadException
	\brief Exception class for network loading errors.
*/
class idNetworkLoadException : public idException
{
public:
	//! Constructs an idNetworkLoadException object with an optional error message.
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
