/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2026 Robert Beckebans
Copyright (C) 2012 Daniel Gibson

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

//===============================================================
//
//	memory allocation all in one place
//
//===============================================================
#include <stdlib.h>
#undef new

//! Allocates 16-byte aligned memory for the specified size and tag.
void* Mem_Alloc16( const size_t size, const memTag_t tag )
{
	if( !size ) {
		return NULL;
	}
	const size_t paddedSize = ( size + 15 ) & ~15;

#ifdef _WIN32
	// this should work with MSVC and mingw, as long as __MSVCRT_VERSION__ >= 0x0700
	return _aligned_malloc( paddedSize, 16 );
#else
	// DG: the POSIX solution for linux etc
	void* ret;
	posix_memalign( &ret, 16, paddedSize );
	return ret;
#endif
}

//! Frees memory that was allocated with Mem_Alloc16.
void Mem_Free16( void* ptr )
{
	if( ptr == NULL ) {
		return;
	}

#ifdef _WIN32
	_aligned_free( ptr );
#else
	// DG: Linux/POSIX compatibility
	// can use normal free() for aligned memory
	free( ptr );
#endif
}

//! Allocates memory with specified size, alignment, and tag.
void* Mem_AllocAligned( const size_t size, const size_t alignment, const memTag_t tag )
{
	// For <= 16 byte alignment we can use the existing allocator.
	if( alignment <= 16 )
		return Mem_Alloc( size, tag );

#ifdef _WIN32
	// _aligned_malloc requires power-of-two alignment.
	return _aligned_malloc( size, alignment );
#else
	void* p = nullptr;
	if( posix_memalign( &p, alignment, size ) != 0 )
		p = nullptr;
	return p;
#endif
}

//! Frees a memory block that was allocated with Mem_AllocAligned.
void Mem_FreeAligned( void* ptr, const size_t alignment ) noexcept
{
	if( !ptr )
		return;

	if( alignment <= 16 ) {
		Mem_Free( ptr );
		return;
	}

#ifdef _WIN32
	_aligned_free( ptr );
#else
	free( ptr );
#endif
}

//! Overloaded new operator that allocates memory using Mem_Alloc and throws std::bad_alloc on failure
void* operator new( size_t s )
{
	void* p = Mem_Alloc( s, TAG_NEW );
	if( !p )
		throw std::bad_alloc();
	return p;
}

//! Allocates memory for an array of objects using a custom allocator.
void* operator new[]( size_t s )
{
	void* p = Mem_Alloc( s, TAG_NEW );
	if( !p )
		throw std::bad_alloc();
	return p;
}

//! Deallocates memory that was previously allocated by the corresponding new operator.
void operator delete( void* p ) noexcept
{
	Mem_Free( p );
}

//! Deallocates memory previously allocated by the matching new[] operator.
void operator delete[]( void* p ) noexcept
{
	Mem_Free( p );
}

//! Deletes memory that was allocated by the corresponding sized new operator
void operator delete( void* p, size_t ) noexcept
{
	Mem_Free( p );
}

//! Custom delete operator for arrays that frees memory using the engine's memory allocator.
void operator delete[]( void* p, size_t ) noexcept
{
	Mem_Free( p );
}

//! Provides aligned memory allocation for C++17 aligned new expressions.
void* operator new( size_t s, std::align_val_t al )
{
	const size_t alignment = ( size_t )al;
	void*		 p		   = Mem_AllocAligned( s, alignment, TAG_NEW );
	if( !p )
		throw std::bad_alloc();
	return p;
}

//! Custom memory allocator for aligned dynamic memory allocation
void* operator new[]( size_t s, std::align_val_t al )
{
	const size_t alignment = ( size_t )al;
	void*		 p		   = Mem_AllocAligned( s, alignment, TAG_NEW );
	if( !p )
		throw std::bad_alloc();
	return p;
}

//! Custom delete operator for aligned memory deallocation.
void operator delete( void* p, std::align_val_t al ) noexcept
{
	Mem_FreeAligned( p, ( size_t )al );
}

//! Deallocates memory that was previously allocated with the corresponding aligned new operator.
void operator delete[]( void* p, std::align_val_t al ) noexcept
{
	Mem_FreeAligned( p, ( size_t )al );
}

//! Custom aligned delete operator for memory deallocation.
void operator delete( void* p, size_t, std::align_val_t al ) noexcept
{
	Mem_FreeAligned( p, ( size_t )al );
}

//! Deallocates aligned memory previously allocated by a corresponding new[] expression.
void operator delete[]( void* p, size_t, std::align_val_t al ) noexcept
{
	Mem_FreeAligned( p, ( size_t )al );
}

//! Overloaded new operator that allocates memory with a specified tag and throws bad_alloc on failure.
void* operator new( size_t s, memTag_t tag )
{
	void* p = Mem_Alloc( s, tag );
	if( !p )
		throw std::bad_alloc();
	return p;
}

//! Allocates memory for an array with the specified size and memory tag, throwing std::bad_alloc on failure
void* operator new[]( size_t s, memTag_t tag )
{
	void* p = Mem_Alloc( s, tag );
	if( !p )
		throw std::bad_alloc();
	return p;
}

//! Deallocates memory previously allocated with the custom new operator.
void operator delete( void* p, memTag_t ) noexcept
{
	Mem_Free( p );
}

//! Deallocates memory previously allocated with a matching new[] operator using the specified memory tag.
void operator delete[]( void* p, memTag_t ) noexcept
{
	Mem_Free( p );
}

//! Allocates memory and initializes it to zero.
void* Mem_ClearedAlloc( const size_t size, const memTag_t tag )
{
	void* mem = Mem_Alloc( size, tag );
	SIMDProcessor->Memset( mem, 0, size );
	return mem;
}

//! Allocates memory and copies a string into it, returning a new null-terminated character array.
char* Mem_CopyString( const char* in )
{
	char* out = ( char* )Mem_Alloc( strlen( in ) + 1, TAG_STRING );
	strcpy( out, in );
	return out;
}
