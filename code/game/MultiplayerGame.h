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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __MULTIPLAYERGAME_H__
#define __MULTIPLAYERGAME_H__

/*
===============================================================================

	Basic DOOM multiplayer

===============================================================================
*/

class idPlayer;
class idMenuHandler_HUD;
class idMenuHandler_Scoreboard;
class idItemTeam;

enum gameType_t {
	GAME_SP		= -2,
	GAME_RANDOM = -1,
	GAME_DM		= 0,
	GAME_TOURNEY,
	GAME_TDM,
	GAME_LASTMAN,
	GAME_CTF,
	GAME_COUNT,
};

// Used by the UI
typedef enum { FLAGSTATUS_INBASE = 0, FLAGSTATUS_TAKEN = 1, FLAGSTATUS_STRAY = 2, FLAGSTATUS_NONE = 3 } flagStatus_t;

// jmarshall
enum mpLeaderStatus_t { LEAD_STATUS_NOTSET, LEAD_STATUS_NOLEAD, LEAD_STATUS_INLEAD };
// jmarshall end

typedef struct mpPlayerState_s {
	int				 ping;			// player ping
	int				 fragCount;		// kills
	int				 teamFragCount; // team kills
	int				 wins;			// wins
	bool			 scoreBoardUp;	// toggle based on player scoreboard button, used to activate de-activate the scoreboard gui
	int				 deaths;
	// jmarshall
	int				 clientnum;
	mpLeaderStatus_t currentLeader; // true if is current leader.
	bool			 tiednotified;	// true if we have already notified the player of tied status.
									// jmarshall end
} mpPlayerState_t;

const int NUM_CHAT_NOTIFY = 5;
const int CHAT_FADE_TIME  = 400;
const int FRAGLIMIT_DELAY = 2000;

const int MP_PLAYER_MINFRAGS = -100;
const int MP_PLAYER_MAXFRAGS = 400; // in CTF frags are player points
const int MP_PLAYER_MAXWINS	 = 100;
const int MP_PLAYER_MAXPING	 = 999;
const int MP_CTF_MAXPOINTS	 = 400;

typedef struct mpChatLine_s {
	idStr line;
	short fade; // starts high and decreases, line is removed once reached 0
} mpChatLine_t;

typedef enum {
	SND_YOUWIN = 0,
	SND_YOULOSE,
	SND_FIGHT,
	SND_THREE,
	SND_TWO,
	SND_ONE,
	SND_SUDDENDEATH,
	SND_FLAG_CAPTURED_YOURS,
	SND_FLAG_CAPTURED_THEIRS,
	SND_FLAG_RETURN,
	SND_FLAG_TAKEN_YOURS,
	SND_FLAG_TAKEN_THEIRS,
	SND_FLAG_DROPPED_YOURS,
	SND_FLAG_DROPPED_THEIRS,
	// jmarshall
	SND_LEADGAINED,
	SND_LEADLOST,
	SND_LEADTIED,
	SND_WELCOMEDOM,
	SND_ONEFRAG,
	SND_TWOFRAG,
	SND_THREEFRAG,
	SND_PREPAREFORBATTLE,
	// jmarshall end
	SND_COUNT
} snd_evt_t;

/*!
	\class idMultiplayerGame
	\brief Manages multiplayer game state, rules, and client-server synchronization for networked gameplay.

	This class orchestrates the core logic and state management of multiplayer matches, handling player interactions, game rules, scoring, team management, and network communication. It coordinates
   between server and client components to maintain consistent game state across all connected players. The class supports various gameplay modes including team-based and flag-based objectives, and
   manages player states such as spawning, death, respawning, and spectating. It also handles user interface elements like scoreboards, HUD displays, and chat systems, ensuring proper rendering and
   updates for each client. Network synchronization is managed through snapshot messages for game state, player positions, and event notifications. Game events such as wins, losses, and achievements
   are tracked and processed, with support for tournament modes and matchmaking. The class also includes utilities for asset preloading, voice and text chat processing, and managing client connections
   and disconnections.

*/
class idMultiplayerGame
{
public:
	//! Initializes a new instance of the idMultiplayerGame class.
	idMultiplayerGame();

	//! Shuts down the multiplayer game by clearing all game state and resources.
	void Shutdown();

	//! Resets all match data and prepares the game for a new match
	void Reset();

	//! Initializes and spawns a player for the specified client number.
	void SpawnPlayer( int clientNum );

	//! Processes multiplayer game state and updates game rules.
	void Run();

	//! Draws the multiplayer HUD, scoreboard, and related UI elements for the specified client.
	bool Draw( int clientNum );

	//! Handles player death events in multiplayer game, updating scores and managing game state.
	void PlayerDeath( idPlayer* dead, idPlayer* killer, bool telefrag );

	//! Adds a formatted chat line to the multiplayer game chat history and displays it in the console.
	void AddChatLine( VERIFY_FORMAT_STRING const char* fmt, ... );

	//! Writes the multiplayer game state to a snapshot message for network synchronization.
	void WriteToSnapshot( idBitMsg& msg ) const;

	//! Reads game state and player information from a snapshot message.
	void ReadFromSnapshot( const idBitMsg& msg );

	// game state
	typedef enum {
		INACTIVE = 0, // not running
		WARMUP,		  // warming up
		COUNTDOWN,	  // post warmup pre-game
		GAMEON,		  // game is on
		SUDDENDEATH,  // game is on but in sudden death, first frag wins
		GAMEREVIEW,	  // game is over, scoreboard is up. we wait si_gameReviewPause seconds (which has a min value)
		NEXTGAME,
		STATE_COUNT
	} gameState_t;
	static const char*			   GameStateStrings[STATE_COUNT];

	//! Returns the current game state of the multiplayer game.
	idMultiplayerGame::gameState_t GetGameState() const;

	static const char*			   GlobalSoundStrings[SND_COUNT];

	//! Plays a global sound event or shader for a specific player in multiplayer
	void						   PlayGlobalSound( int toPlayerNum, snd_evt_t evt, const char* shader = NULL );

	//! Plays a sound event to all players on the specified team.
	void						   PlayTeamSound( int toTeam, snd_evt_t evt, const char* shader = NULL );

	// more compact than a chat line
	typedef enum {
		MSG_SUICIDE = 0,
		MSG_KILLED,
		MSG_KILLEDTEAM,
		MSG_DIED,
		MSG_SUDDENDEATH,
		MSG_JOINEDSPEC,
		MSG_TIMELIMIT,
		MSG_FRAGLIMIT,
		MSG_TELEFRAGGED,
		MSG_JOINTEAM,
		MSG_HOLYSHIT,
		MSG_POINTLIMIT,
		MSG_FLAGTAKEN,
		MSG_FLAGDROP,
		MSG_FLAGRETURN,
		MSG_FLAGCAPTURE,
		MSG_SCOREUPDATE,
		MSG_LEFTGAME,
		MSG_COUNT
	} msg_evt_t;

	//! Displays localized chat messages for various multiplayer game events.
	void		PrintMessageEvent( msg_evt_t evt, int parm1 = -1, int parm2 = -1 );

	//! Disconnects a client from the multiplayer game.
	void		DisconnectClient( int clientNum );

	//! Handles the client command to drop a weapon in multiplayer mode.
	static void DropWeapon_f( const idCmdArgs& args );

	//! Handles the message mode command for multiplayer games.
	static void MessageMode_f( const idCmdArgs& args );

	//! Handles the voice chat command for multiplayer games.
	static void VoiceChat_f( const idCmdArgs& args );

	//! Handles voice chat commands for team-based communication in multiplayer games.
	static void VoiceChatTeam_f( const idCmdArgs& args );

	//! Returns the number of actual clients in the multiplayer game, optionally counting spectators and tracking team distribution.
	int			NumActualClients( bool countSpectators, int* teamcount = NULL );

	//! Drops the weapon for the specified client in multiplayer mode.
	void		DropWeapon( int clientNum );

	//! Resets the multiplayer game state to warmup and balances the teams.
	void		MapRestart();

	//! Balances the teams in a multiplayer game by redistributing players between them.
	void		BalanceTeams();

	//! Changes the team of a player in a multiplayer game
	void		SwitchToTeam( int clientNum, int oldteam, int newteam );

	//! Returns whether the multiplayer game is ready for pure mode.
	bool		IsPureReady() const;

	/*!
		\brief Processes and broadcasts a chat message from a client, handling team and spectator chat appropriately.

		This function handles the processing of chat messages in a multiplayer game. It determines whether the message should be sent to all players, spectators, or team members based on the client's
	   status and the chat type. The function constructs a formatted message with a prefix indicating the sender's status (e.g., 'team' or 'spectating') and then sends the message to the appropriate
	   recipients. It also plays a sound effect if specified. The function ensures that messages are properly formatted and routed based on the game state and player roles.

		\param clientNum The index of the client sending the message, or -1 for server messages
		\param team True if the message is a team chat, false for global chat
		\param name The name of the player sending the message
		\param text The content of the chat message
		\param sound The sound effect to play when the message is sent, or NULL if no sound
		\throws assertion failure if the game is running in client mode
	*/
	void		ProcessChatMessage( int clientNum, bool team, const char* name, const char* text, const char* sound );

	//! Processes voice chat for a specified client, handling team or global messages with associated sound and text.
	void		ProcessVoiceChat( int clientNum, bool team, int index );

	//! Handles GUI events for the multiplayer game, specifically forwarding events to the scoreboard manager when it is active.
	bool		HandleGuiEvent( const sysEvent_t* sev );

	//! Checks whether the scoreboard is currently active.
	bool		IsScoreboardActive();

	//! Sets the active state of the scoreboard in the multiplayer game.
	void		SetScoreboardActive( bool active );

	//! Cleans up the scoreboard manager by deleting it and setting the pointer to NULL.
	void		CleanupScoreboard();

	//! Preloads assets and definitions needed for multiplayer game mode.
	void		Precache();

	//! Toggles the local player's spectate state in multiplayer mode.
	void		ToggleSpectate();

	//! Sets spectator text based on player state and game type
	void		GetSpectateText( idPlayer* player, idStr spectatetext[2], bool scoreboard );

	//! Clears the frag count for the specified client.
	void		ClearFrags( int clientNum );

	//! Returns true if the specified player is allowed to play, false if they want to spectate.
	bool		CanPlay( idPlayer* p );

	//! Returns true if the specified player wants to respawn.
	bool		WantRespawn( idPlayer* p );

	//! Transitions the multiplayer game to a new state and handles associated server-side logic
	void		NewState( gameState_t news, idPlayer* player = NULL );

	//! Sends initial reliable game state messages to a specified client
	void		ServerWriteInitialReliableMessages( int clientNum, lobbyUserID_t lobbyUserID );

	//! Reads the initial game state and player powerups from a network message.
	void		ClientReadStartState( const idBitMsg& msg );

	//! Sets the warmup end time from a network message.
	void		ClientReadWarmupTime( const idBitMsg& msg );

	//! Reads the match start time from a bit message.
	void		ClientReadMatchStartedTime( const idBitMsg& msg );

	//! Handles receiving and processing achievement unlock notifications from the server for a specific player.
	void		ClientReadAchievementUnlock( const idBitMsg& msg );

	//! Initializes the player state for a newly connecting client.
	void		ServerClientConnect( int clientNum );

	//! Returns the number of flag points for the specified team in a CTF game.
	int			GetFlagPoints( int team );

	//! Sets whether flag event messages are allowed to be sent.
	void		SetFlagMsg( bool b );

	//! Returns whether flag event messages should be displayed based on the current game state and flag message setting.
	bool		IsFlagMsgOn();

	int			player_red_flag;  // Ent num of red flag carrier for HUD
	int			player_blue_flag; // Ent num of blue flag carrier for HUD

	//! Retrieves and formats player statistics for a specified client into a data buffer.
	void		PlayerStats( int clientNum, char* data, const int len );

private:
	static const char*		  teamNames[];
	static const char*		  skinNames[];
	static const idVec3		  skinColors[];
	static const int		  numSkins;

	// jmarshall
	int						  killsRemainingMessageState;
	// jmarshall end

	// state vars
	gameState_t				  gameState; // what state the current game is in
	gameState_t				  nextState; // state to switch to when nextStateSwitch is hit

	mpPlayerState_t			  playerState[MAX_CLIENTS];

	// keep track of clients which are willingly in spectator mode
	// time related
	int						  nextStateSwitch;	// time next state switch
	int						  warmupEndTime;	// warmup till..
	int						  matchStartedTime; // time current match started

	// tourney
	int						  currentTourneyPlayer[2]; // our current set of players
	int						  lastWinner;			   // plays again

	// warmup
	bool					  one, two, three; // keeps count down voice from repeating

	// guis
	idMenuHandler_Scoreboard* scoreboardManager;

	// chat data
	mpChatLine_t			  chatHistory[NUM_CHAT_NOTIFY];
	int						  chatHistoryIndex;
	int						  chatHistorySize; // 0 <= x < NUM_CHAT_NOTIFY
	bool					  chatDataUpdated;
	int						  lastChatLineTime;

	// rankings are used by UpdateScoreboard and UpdateHud
	int						  numRankedPlayers; // ranked players, others may be empty slots or spectators
	idPlayer*				  rankedPlayers[MAX_CLIENTS];

	bool					  pureReady; // defaults to false, set to true once server game is running with pure checksums
	int						  fragLimitTimeout;

	int						  voiceChatThrottle;

	int						  startFragLimit; // synchronize to clients in initial state, set on -> GAMEON

	idItemTeam*				  teamFlags[2];
	int						  teamPoints[2];

	bool					  flagMsgOn;

private:
	//! Updates the player ranks based on frag counts and game type.
	void		UpdatePlayerRanks();

	//! Handles the game end event when a multiplayer game has been won.
	void		GameHasBeenWon();

	/*!
		\brief Updates the specified GUI with rank color information based on the provided vector values.

		This function iterates through three color components of the provided vector and sets corresponding state float values in the GUI. The mask parameter is used to format the state names, which
	   include the index i and a component index j. The function is used to update visual rank color information in multiplayer game interfaces.

		\param gui Pointer to the user interface object to be updated
		\param mask Format string used to construct state names for the GUI
		\param i Index used in the mask formatting to identify the specific rank
		\param vec Vector containing the RGB color values to be set in the GUI
	*/
	void		UpdateRankColor( idUserInterface* gui, const char* mask, int i, const idVec3& vec );

	//! Updates the scoreboard with player information and game state details.
	void		UpdateScoreboard( idMenuHandler_Scoreboard* scoreboard, idPlayer* owner );

	//! Draws the scoreboard for the specified player if the scoreboard is active.
	void		DrawScoreBoard( idPlayer* player );

	//! Updates the HUD for the specified player with game state information
	void		UpdateHud( idPlayer* player, idMenuHandler_HUD* hudManager );

	//! Returns true if the game state is currently in the warmup phase.
	bool		Warmup();

	//! Returns the player who has reached the frag limit in multiplayer game modes.
	idPlayer*	FragLimitHit();

	//! Returns the player with the most frags or NULL if there is a tie.
	idPlayer*	FragLeader();

	//! Checks whether the time limit has been reached in the multiplayer game.
	bool		TimeLimitHit();

	//! Checks if the point limit has been reached in a multiplayer game.
	bool		PointLimitHit();

	//! Returns the team with the most points in a multiplayer game.
	int			WinningTeam();

	//! Updates win/loss counts for players based on the winner of a match
	void		UpdateWinsLosses( idPlayer* winner );

	//! Fills empty tournament slots based on player ranks and time in game
	void		FillTourneySlots();

	//! Cycles through tournament players by updating their game state and ranks.
	void		CycleTourneyPlayers();

	//! Updates the tournament line by building a wait list for clients based on their tourney ranks.
	void		UpdateTourneyLine();

	//! Returns a string representation of the current game time or countdown timer.
	const char* GameTime();

	//! Resets all multiplayer game state variables to their default values.
	void		Clear();

	//! Checks if there are enough clients to start a multiplayer game.
	bool		EnoughClientsToPlay();

	//! Clears the chat data in the multiplayer game.
	void		ClearChatData();

	//! Updates and displays the chat history for a player in multiplayer game
	void		DrawChat( idPlayer* player );

	//! Checks if players should be respawned based on game state and respawn rules
	void		CheckRespawns( idPlayer* spectator = NULL );

	//! Checks if the game should be aborted based on player states and game type.
	void		CheckAbortGame();

	//! Sets the chatting state of the local player based on command arguments.
	void		MessageMode( const idCmdArgs& args );

	//! Updates the team score for a specified team by adding a delta value to the frag count of players on that team.
	void		TeamScore( int entityNumber, int team, int delta );

	//! Handles voice chat commands in multiplayer mode by validating input and sending the appropriate message to the host.
	void		VoiceChat( const idCmdArgs& args, bool team );

	//! Prints the tournament rank for each client in the multiplayer game.
	void		DumpTourneyLine();

	//! Resurrects players in Last Man Navigating mode who still have lives remaining after a game end event is aborted.
	void		SuddenRespawn();

	//! Finds and assigns the team flags for the red and blue teams.
	void		FindTeamFlags();

	//! Sets the scoreboard to inactive when the game state changes to WARMUP.
	void		NewState_Warmup_ServerAndClient();

	//! Sets the scoreboard to inactive when the game state changes to COUNTDOWN.
	void		NewState_Countdown_ServerAndClient();

	//! Initializes the game state when transitioning to GAMEON on both servers and clients.
	void		NewState_GameOn_ServerAndClient();

	//! Sets the scoreboard to active when the game state changes to GAMEREVIEW.
	void		NewState_GameReview_ServerAndClient();

public:
	//! Returns the name of the specified team.
	const char*	  GetTeamName( int team ) const;

	//! Returns the name of the skin at the specified index.
	const char*	  GetSkinName( int skin ) const;

	//! Returns the skin color for the specified skin index.
	const idVec3& GetSkinColor( int skin ) const;

	//! Returns the team flag object for the specified team in multiplayer game
	idItemTeam*	  GetTeamFlag( int team );

	//! Returns the status of the flag for the specified team in a multiplayer game.
	flagStatus_t  GetFlagStatus( int team );

	//! Updates the score for a specified team in CTF gameplay
	void		  TeamScoreCTF( int team, int delta );

	//! Updates the frag count for a specified player in a multiplayer CTF game.
	void		  PlayerScoreCTF( int playerIdx, int delta );

	//! Returns the entity number of the player carrying the flag for the specified team, or -1 if no such player exists.
	int			  GetFlagCarrier( int team );
	void		  UpdateScoreboardFlagStatus();

	//! Reloads the scoreboard display and initializes it with the scoreboard manager.
	void		  ReloadScoreboard();

	//! Returns the available game modes for multiplayer, optionally including CTF based on session settings.
	int			  GetGameModes( const char*** gameModes, const char*** gameModesDisplay );

	//! Returns true if the current game type is flag-based, false otherwise.
	bool		  IsGametypeFlagBased();

	//! Returns true if the current game type is team-based, false otherwise.
	bool		  IsGametypeTeamBased();
};

ID_INLINE idMultiplayerGame::gameState_t idMultiplayerGame::GetGameState() const
{
	return gameState;
}

ID_INLINE bool idMultiplayerGame::IsPureReady() const
{
	return pureReady;
}

ID_INLINE void idMultiplayerGame::ClearFrags( int clientNum )
{
	playerState[clientNum].fragCount = 0;
}

#endif /* !__MULTIPLAYERGAME_H__ */
