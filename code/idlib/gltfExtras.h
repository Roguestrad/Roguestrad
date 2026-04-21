/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2022 - 2023 Harrie van Ginneken

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

#include "gltfParser.h"

#ifndef gltfExtraParser
	#define gltfExtraParser( className, ptype )                                \
		class gltfExtra_##className : public parsable, public parseType<ptype> \
		{                                                                      \
		public:                                                                \
			gltfExtra_##className( idStr Name ) :                              \
				name( Name )                                                   \
			{                                                                  \
				item = nullptr;                                                \
			}                                                                  \
			virtual void parse( idToken& token )                               \
			{                                                                  \
				parse( token, nullptr );                                       \
			}                                                                  \
			virtual void   parse( idToken& token, idLexer* parser );           \
			virtual idStr& Name()                                              \
			{                                                                  \
				return name;                                                   \
			}                                                                  \
                                                                               \
		private:                                                               \
			idStr name;                                                        \
		}

#endif

// Helper macros for gltf data deserialize
#define GLTFARRAYITEM( target, name, type ) \
	auto* name = new type( #name );         \
	target.AddItemDef( ( parsable* )name )
#define GLTFARRAYITEMREF( target, name, type, ref ) \
	auto* name = new type( #name );                 \
	target.AddItemDef( ( parsable* )name );         \
	name->Set( &ref )

#ifndef GLTF_EXTRAS_H
	#define GLTF_EXTRAS_H

/*!
	\class gltfExtraStub
	\brief A stub class for handling extra GLTF data.
*/
class gltfExtraStub
{
public:
	//! Constructs a new gltfExtraStub object.
	gltfExtraStub() { }
};

/*!
	\brief Parses scatter information from a GLTF file using the provided token and parser.

	This function prepares for parsing by unread the provided token and then creates a scatter information array to hold the parsed data. It utilizes the GLTF array item parsing mechanism to parse the
   emitter object within the scatter information.

	\param token The token to be unread and used as a starting point for parsing.
	\param parser The lexer used to parse the GLTF file content.
*/
gltfExtraParser( Scatter, gltfExtraStub );

/*!
	\brief Parses camera lens frame data from a GLTF file using the provided token and parser

	This function initializes a new list to store double precision numbers and a number array item parser. It then uses the number array parser to populate the list with data from the parser based on
   the provided token

	\param token The token containing the data to parse
	\param parser The lexer parser used to extract the camera lens frame data
*/
gltfExtraParser( CameraLensFrames, idList<double> );

#endif // GLTF_EXTRAS_H
#undef gltfExtraParser
