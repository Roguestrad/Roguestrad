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

const char* bot_kill_insult[] =
{
	"%s: Try aiming next time %s",
	"%s: Trust me when I say this %s, you suck.",
	"%s: Maybe you should go back to go cart racing %s?",
	"%s: Your technique reminds me of a story %s, a very dull one."
};

const char* bot_death_insult[] =
{
	"%s: Mmmmmmm. Was it good for you too %s?",
	"%s: Well I guess %s won the lottery.",
	"%s: My mother fragged me once %s. Once.",
	"%s: I'm gonna pull out your bowels %s",
	"%s: Beginners luck %s... Again",
	"%s: So %s... you're up to what ... 3 frags an hour?"
};

const char* bot_death_praise[] =
{
	"%s: %s not bad for an amateur.",
	"%s: %s alright that was pretty good. Your still a dousche.",
	"%s: That was definitely ... um ... pretty good %s",
	"%s: I've seen better %s, but not many.",
	"%s: Take a moment to reflect on your accomplishment %s",
	"%s: Your pretty good for a dousche %s"
};

/*
====================
iceBot::BotSendChatMessage
====================
*/
void iceBot::BotSendChatMessage( botChat_t chat, const char* targetName )
{
	switch( chat )
	{
		case KILL:
			gameLocal.mpGame.AddChatLine( bot_kill_insult[rvRandom::irand( 0, 3 )], netname.c_str(), targetName );
			break;
		case DEATH:
			if( rvRandom::irand( 0, 10 ) < 5 )
			{
				gameLocal.mpGame.AddChatLine( bot_death_insult[rvRandom::irand( 0, 5 )], netname.c_str(), targetName );
			}
			else
			{
				gameLocal.mpGame.AddChatLine( bot_death_praise[rvRandom::irand( 0, 5 )], netname.c_str(), targetName );
			}
			break;
	}
}