/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

idBotCharacterStatsManager botCharacterStatsManager;

/*
====================
idBotCharacterStatsManager::idBotCharacterStatsManager
====================
*/
idBotCharacterStatsManager::idBotCharacterStatsManager()
{
	default_char_profile = nullptr;
}

/*
====================
idBotCharacterStatsManager::Init
====================
*/
void idBotCharacterStatsManager::Init()
{
	default_char_profile = BotLoadCharacterFromFile( "bots/default_c.c", 1 );
	if( default_char_profile == NULL )
	{
		common->FatalError( "Failed to load default characteristic def file\n" );
	}
}

/*
============================
idBotCharacterStatsManager::AllocBotCharacter
============================
*/
bot_character_t* idBotCharacterStatsManager::AllocBotCharacter()
{
	bot_character_t* ch = NULL;

	for( int i = 0; i < MAX_CHAR_STATS; i++ )
	{
		if( charStatsList[i].inUse == false )
		{
			charStatsList[i].inUse = true;
			ch = &charStatsList[i];
			break;
		}
	}

	if( ch == nullptr )
	{
		gameLocal.Error( "idBotCharacterStatsManager::AllocBotCharacter: Too many bot characters!" );
		return nullptr;
	}

	if( default_char_profile == NULL )
	{
		memset( &ch->c[0], 0, MAX_CHARACTERISTICS * sizeof( bot_characteristic_t ) );
		ch->filename = "";
		ch->inUse = false;
		ch->skill = 0;
	}
	else
	{
		//memcpy( ch, default_char_profile, sizeof( bot_character_t ) + MAX_CHARACTERISTICS * sizeof( bot_characteristic_t ) );
		*ch = *default_char_profile;
	}



	return ch;
}

/*
============================
idBotCharacterStatsManager::FreeCharacterFile
============================
*/
void idBotCharacterStatsManager::FreeCharacterFile( bot_character_t* ch )
{
	ch->inUse = false;
}

/*
============================
idBotCharacterStatsManager::BotLoadCharacterFromFile
============================
*/
bot_character_t* idBotCharacterStatsManager::BotLoadCharacterFromFile( const char* charfile, int skill )
{
	int indent, index;
	bool foundcharacter;
	bot_character_t* ch;
	idParser parser;
	idToken token;

	foundcharacter = false;

	// Check to see if we already loaded this bot file.
	for( int i = 0; i < MAX_CHAR_STATS; i++ )
	{
		if( charStatsList[i].inUse && charStatsList[i].filename == charfile )
		{
			return &charStatsList[i];
		}
	}

	iceScopedLexerBaseFolder scopedBaseFolder( BOTFILESBASEFOLDER );

	//a bot character is parsed in two phases
	if( !parser.LoadFile( charfile ) )
	{
		common->Warning( "BotLoadCharacterFromFile: counldn't load %s\n", charfile );
		return NULL;
	}

	ch = AllocBotCharacter();
	ch->filename = charfile;

	while( parser.ReadToken( &token ) )
	{
		if( token == "skill" )
		{
			if( !parser.ExpectTokenType( TT_NUMBER, 0, &token ) )
			{
				FreeCharacterFile( ch );
				parser.Warning( "Expected token type number\n" );
				return NULL;
			}

			if( !parser.ExpectTokenString( "{" ) )
			{
				FreeCharacterFile( ch );
				parser.Warning( "Expected token {\n" );
				return NULL;
			}

			//if it's the correct skill
			if( skill < 0 || token.GetIntValue() == skill )
			{
				foundcharacter = true;
				ch->skill = token.GetIntValue();
				while( parser.ReadToken( &token ) )
				{
					if( token == "}" )
					{
						break;
					}

					if( token.type != TT_NUMBER || !( token.subtype & TT_INTEGER ) )
					{
						FreeCharacterFile( ch );
						parser.Error( "expected integer index, found %s\n", token.c_str() );
						return NULL;
					}

					index = token.GetIntValue();
					if( index < 0 || index > MAX_CHARACTERISTICS )
					{
						FreeCharacterFile( ch );
						parser.Error( "characteristic index out of range [0, %d]\n", MAX_CHARACTERISTICS );
						return NULL;
					}
					// jmarshall - not sure what we loose by removing this check, basically duplicate definition check?
					//if (ch->c[index].type)
					//{
					//	G_Error("characteristic %d already initialized\n", index);
					//	trap_PC_FreeSource(source);
					//	BotFreeCharacterStrings(ch);
					//	//FreeMemory(ch);
					//	return NULL;
					//}
					// jmarshall end

					if( !parser.ReadToken( &token ) )
					{
						FreeCharacterFile( ch );
						parser.Error( "Unexpected EOF during parse characesistic" );
						return NULL;
					}

					if( token.type == TT_NUMBER )
					{
						if( token.subtype & TT_FLOAT )
						{
							ch->c[index].value._float = token.GetFloatValue();
							ch->c[index].type = CT_FLOAT;
						}
						else
						{
							ch->c[index].value.integer = token.GetIntValue();
							ch->c[index].type = CT_INTEGER;
						}
					}
					else if( token.type == TT_STRING )
					{
						token.StripDoubleQuotes();
						ch->c[index].value.string = token;
						ch->c[index].type = CT_STRING;
					}
					else
					{
						FreeCharacterFile( ch );
						gameLocal.Error( "expected integer, float or string, found %s\n", token.c_str() );
						return NULL;
					}
				}
				break;
			}
			else
			{
				indent = 1;
				while( indent )
				{
					if( !parser.ReadToken( &token ) )
					{
						FreeCharacterFile( ch );
						return NULL;
					}
					if( token == "{" )
					{
						indent++;
					}
					else if( token == "}" )
					{
						indent--;
					}
				}
			}
		}
		else
		{
			FreeCharacterFile( ch );
			gameLocal.Error( "unknown definition %s\n", token.c_str() );
			return NULL;
		}
	}

	if( !foundcharacter )
	{
		FreeCharacterFile( ch );
		return NULL;
	}


	return ch;
}


/*
============================
idBotCharacterStatsManager::CheckCharacteristicIndex
============================
*/
int idBotCharacterStatsManager::CheckCharacteristicIndex( bot_character_t* ch, int index )
{
	if( index < 0 || index >= MAX_CHARACTERISTICS )
	{
		gameLocal.Error( "characteristic %d does not exist\n", index );
		return false;
	}
	if( !ch->c[index].type )
	{
		gameLocal.Error( "characteristic %d is not initialized\n", index );
		return false;
	}
	return true;
}

/*
============================
idBotCharacterStatsManager::Characteristic_Float
============================
*/
float idBotCharacterStatsManager::Characteristic_Float( bot_character_t* ch, int index )
{
	if( !ch )
	{
		return 0;
	}
	//check if the index is in range
	if( !CheckCharacteristicIndex( ch, index ) )
	{
		return 0;
	}

	if( ch->c[index].type == CT_INTEGER )
	{
		//an integer will be converted to a float
		return ( float )ch->c[index].value.integer;
	}
	else if( ch->c[index].type == CT_FLOAT )
	{
		//floats are just returned
		return ch->c[index].value._float;
	}

	//cannot convert a string pointer to a float
	gameLocal.Error( "characteristic %d is not a float\n", index );
	return 0;
}

/*
============================
Characteristic_String
============================
*/
void idBotCharacterStatsManager::Characteristic_String( bot_character_t* ch, int index, char* buf, int size )
{
	//check if the index is in range
	if( !CheckCharacteristicIndex( ch, index ) )
	{
		return;
	}

	//an integer will be converted to a float
	if( ch->c[index].type == CT_STRING )
	{
		strcpy( buf, ch->c[index].value.string );
		buf[size - 1] = '\0';
		return;
	}
	gameLocal.Error( "characteristic %d is not a string\n", index );
}

/*
============================
idBotCharacterStatsManager::Characteristic_BFloat
============================
*/
float idBotCharacterStatsManager::Characteristic_BFloat( bot_character_t* ch, int index, float min, float max )
{
	float value;

	if( min > max )
	{
		gameLocal.Error( "cannot bound characteristic %d between %f and %f\n", index, min, max );
		return 0;
	}

	value = Characteristic_Float( ch, index );
	if( value < min )
	{
		return min;
	}
	if( value > max )
	{
		return max;
	}
	return value;
}
