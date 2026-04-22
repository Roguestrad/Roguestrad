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

#include "precompiled.h"
#pragma hdrstop

#ifdef ID_RETAIL
idCVar net_allowCheats( "net_allowCheats", "0", CVAR_BOOL | CVAR_ROM, "Allow cheats in multiplayer" );
#else
idCVar net_allowCheats( "net_allowCheats", "0", CVAR_BOOL | CVAR_NOCHEAT, "Allow cheats in multiplayer" );
#endif

/*
===============================================================================

	idCmdSystemLocal

===============================================================================
*/

typedef struct commandDef_s {
	struct commandDef_s* next;
	char*				 name;
	cmdFunction_t		 function;
	argCompletion_t		 argCompletion;
	int					 flags;
	char*				 description;
} commandDef_t;

/*!
	\class idCmdSystemLocal
	\brief Manages console commands and their execution within the system.

	Provides the core functionality for registering, executing, and managing console commands. It handles command registration with flags and descriptions, supports command argument completion, and
   maintains a command buffer for deferred execution. The system supports various command types including engine reloads, script execution, and command listing with filtering by flags. Command
   handlers are linked to command names and can be executed either immediately or buffered for later processing. The class also supports specialized completion functions for files, declarations, and
   specific command categories.

*/
class idCmdSystemLocal : public idCmdSystem
{
public:
	//! Initializes the command system by registering standard commands and linking declared commands.
	virtual void Init();

	//! Shuts down the command system and frees all allocated memory.
	virtual void Shutdown();

	/*!
		\brief Registers a new console command with the command system

		Adds a new command to the console command list with the specified name, function handler, flags, and description. The function checks if a command with the same name already exists and reports
	   an error if it does. The command is stored in a linked list structure for later execution

		\param cmdName Name of the command to register
		\param function Function pointer to the command handler
		\param flags Flags that control command execution behavior
		\param description Description text for the command shown in help
		\param argCompletion Optional function for argument completion suggestions
	*/
	virtual void AddCommand( const char* cmdName, cmdFunction_t function, int flags, const char* description, argCompletion_t argCompletion = NULL );

	//! Removes a command with the specified name from the command system.
	virtual void RemoveCommand( const char* cmdName );

	//! Removes all commands that have any of the specified flags set.
	virtual void RemoveFlaggedCommands( int flags );

	//! Populates a callback function with the names of all registered commands.
	virtual void CommandCompletion( void ( *callback )( const char* s ) );

	//! Performs command argument completion by matching the command string and calling a callback for each valid argument.
	virtual void ArgCompletion( const char* cmdString, void ( *callback )( const char* s ) );

	//! Executes a string of command text by tokenizing and processing each command.
	virtual void ExecuteCommandText( const char* text );

	//! Appends command text to the end of the command buffer
	virtual void AppendCommandText( const char* text );

	//! Adds command text to the command buffer for later execution with specified execution type.
	virtual void BufferCommandText( cmdExecution_t exec, const char* text );

	//! Executes all commands stored in the command buffer until it is empty
	virtual void ExecuteCommandBuffer();

	/*!
		\brief Provides command argument completion for files and folders with specified extensions, using a callback to handle each completed string.

		This function implements argument completion for commands that involve file or folder paths. It lists files and folders in the specified directory, filtering out certain entries like autosave
	   files and extra entity files. The completion results are passed to a provided callback function. The folder path and extension arguments are processed to build the completion strings, and the
	   stripFolder parameter controls whether to strip the base folder prefix from the results. The function supports variadic extension arguments for listing files with multiple extensions.

		\param args The command arguments provided to the completion function.
		\param callback A function to be called for each completed string.
		\param folder The base folder path to use for completion.
		\param stripFolder Controls whether to strip the base folder prefix from results.
		\param extension Variadic parameter list of file extensions to include in the file listing.
	*/
	virtual void ArgCompletion_FolderExtension( const idCmdArgs& args, void ( *callback )( const char* s ), const char* folder, int stripFolder, ... );

	//! Completes command arguments with declaration names of a specified type
	virtual void ArgCompletion_DeclName( const idCmdArgs& args, void ( *callback )( const char* s ), int type );

	//! Buffers command arguments for execution based on the specified execution type.
	virtual void BufferCommandArgs( cmdExecution_t exec, const idCmdArgs& args );

	//! Schedules a engine reload for the next command execution and stores the provided command arguments for post-reload execution.
	virtual void SetupReloadEngine( const idCmdArgs& args );

	//! Executes queued commands after an engine reload if they exist.
	virtual bool PostReloadEngine();

	//! Sets the number of frames to wait.
	void		 SetWait( int numFrames )
	{
		wait = numFrames;
	}

	//! Returns a pointer to the first command definition in the command list.
	commandDef_t* GetCommands() const
	{
		return commands;
	}

private:
	static const int  MAX_CMD_BUFFER = 0x10000;

	commandDef_t*	  commands;

	int				  wait;
	int				  textLength;
	byte			  textBuf[MAX_CMD_BUFFER];

	idStr			  completionString;
	idStrList		  completionParms;

	// piggybacks on the text buffer, avoids tokenize again and screwing it up
	idList<idCmdArgs> tokenizedCmds;

	// a command stored to be executed after a reloadEngine and all associated commands have been processed
	idCmdArgs		  postReload;

private:
	//! Executes a command string that has been tokenized into arguments.
	void		ExecuteTokenizedString( const idCmdArgs& args );

	//! Inserts command text into the command buffer, appending a newline character.
	void		InsertCommandText( const char* text );

	//! Lists commands filtered by specified flags and optional match string
	static void ListByFlags( const idCmdArgs& args, cmdFlags_t flags );

	//! Lists all console commands matching the specified flags.
	static void List_f( const idCmdArgs& args );

	//! Lists system commands that match the provided arguments.
	static void SystemList_f( const idCmdArgs& args );

	//! Lists renderer-related commands.
	static void RendererList_f( const idCmdArgs& args );

	//! Lists sound commands available in the command system.
	static void SoundList_f( const idCmdArgs& args );

	//! Lists game-related commands.
	static void GameList_f( const idCmdArgs& args );

	//! Lists commands that have the tool flag set.
	static void ToolList_f( const idCmdArgs& args );

	//! Executes a script file by reading and buffering its contents for command execution.
	static void Exec_f( const idCmdArgs& args );

	//! Executes a command text from a cvar value
	static void Vstr_f( const idCmdArgs& args );

	//! Prints all command arguments except the first one to the console.
	static void Echo_f( const idCmdArgs& args );

	//! Prints out how the command line was parsed for debugging purposes.
	static void Parse_f( const idCmdArgs& args );

	//! Sets the command system to wait for a specified number of frames before continuing execution.
	static void Wait_f( const idCmdArgs& args );
	static void PrintMemInfo_f( const idCmdArgs& args );
};

idCmdSystemLocal cmdSystemLocal;
idCmdSystem*	 cmdSystem = &cmdSystemLocal;

/*!
	\class idSort_CommandDef
	\brief The idSort_CommandDef class provides case-insensitive alphabetical sorting for command definition structures.
*/
class idSort_CommandDef : public idSort_Quick<commandDef_t, idSort_CommandDef>
{
public:
	//! Compares two command definition structures based on their names in a case-insensitive manner
	int Compare( const commandDef_t& a, const commandDef_t& b ) const
	{
		return idStr::Icmp( a.name, b.name );
	}
};

void idCmdSystemLocal::ListByFlags( const idCmdArgs& args, cmdFlags_t flags )
{
	int							i;
	idStr						match;
	const commandDef_t*			cmd;
	idList<const commandDef_t*> cmdList;

	if( args.Argc() > 1 ) {
		match = args.Args( 1, -1 );
		match.Replace( " ", "" );
	} else {
		match = "";
	}

	for( cmd = cmdSystemLocal.GetCommands(); cmd; cmd = cmd->next ) {
		if( !( cmd->flags & flags ) ) {
			continue;
		}
		if( match.Length() && idStr( cmd->name ).Filter( match, false ) == 0 ) {
			continue;
		}

		cmdList.Append( cmd );
	}

	// cmdList.SortWithTemplate( idSort_CommandDef() );

	for( i = 0; i < cmdList.Num(); i++ ) {
		cmd = cmdList[i];

		common->Printf( "  %-21s %s\n", cmd->name, cmd->description );
	}

	common->Printf( "%i commands\n", cmdList.Num() );
}

void idCmdSystemLocal::List_f( const idCmdArgs& args )
{
	idCmdSystemLocal::ListByFlags( args, CMD_FL_ALL );
}

void idCmdSystemLocal::SystemList_f( const idCmdArgs& args )
{
	idCmdSystemLocal::ListByFlags( args, CMD_FL_SYSTEM );
}

void idCmdSystemLocal::RendererList_f( const idCmdArgs& args )
{
	idCmdSystemLocal::ListByFlags( args, CMD_FL_RENDERER );
}

void idCmdSystemLocal::SoundList_f( const idCmdArgs& args )
{
	idCmdSystemLocal::ListByFlags( args, CMD_FL_SOUND );
}

void idCmdSystemLocal::GameList_f( const idCmdArgs& args )
{
	idCmdSystemLocal::ListByFlags( args, CMD_FL_GAME );
}

void idCmdSystemLocal::ToolList_f( const idCmdArgs& args )
{
	idCmdSystemLocal::ListByFlags( args, CMD_FL_TOOL );
}

void idCmdSystemLocal::Exec_f( const idCmdArgs& args )
{
	char* f;
	int	  len;
	idStr filename;

	if( args.Argc() != 2 ) {
		common->Printf( "exec <filename> : execute a script file\n" );
		return;
	}

	filename = args.Argv( 1 );
	filename.DefaultFileExtension( ".cfg" );
	len = fileSystem->ReadFile( filename, reinterpret_cast<void**>( &f ), NULL );
	if( !f ) {
		common->Printf( "couldn't exec %s\n", args.Argv( 1 ) );
		return;
	}
	common->Printf( "execing %s\n", args.Argv( 1 ) );

	cmdSystemLocal.BufferCommandText( CMD_EXEC_INSERT, f );

	fileSystem->FreeFile( f );
}

void idCmdSystemLocal::Vstr_f( const idCmdArgs& args )
{
	const char* v;

	if( args.Argc() != 2 ) {
		common->Printf( "vstr <variablename> : execute a variable command\n" );
		return;
	}

	v = cvarSystem->GetCVarString( args.Argv( 1 ) );

	cmdSystemLocal.BufferCommandText( CMD_EXEC_APPEND, va( "%s\n", v ) );
}

void idCmdSystemLocal::Echo_f( const idCmdArgs& args )
{
	int i;

	for( i = 1; i < args.Argc(); i++ ) {
		common->Printf( "%s ", args.Argv( i ) );
	}
	common->Printf( "\n" );
}

void idCmdSystemLocal::Wait_f( const idCmdArgs& args )
{
	if( args.Argc() == 2 ) {
		cmdSystemLocal.SetWait( atoi( args.Argv( 1 ) ) );
	} else {
		cmdSystemLocal.SetWait( 1 );
	}
}

void idCmdSystemLocal::Parse_f( const idCmdArgs& args )
{
	int i;

	for( i = 0; i < args.Argc(); i++ ) {
		common->Printf( "%i: %s\n", i, args.Argv( i ) );
	}
}

void idCmdSystemLocal::Init()
{
	AddCommand( "listCmds", List_f, CMD_FL_SYSTEM, "lists commands" );
	AddCommand( "listSystemCmds", SystemList_f, CMD_FL_SYSTEM, "lists system commands" );
	AddCommand( "listRendererCmds", RendererList_f, CMD_FL_SYSTEM, "lists renderer commands" );
	AddCommand( "listSoundCmds", SoundList_f, CMD_FL_SYSTEM, "lists sound commands" );
	AddCommand( "listGameCmds", GameList_f, CMD_FL_SYSTEM, "lists game commands" );
	AddCommand( "listToolCmds", ToolList_f, CMD_FL_SYSTEM, "lists tool commands" );
	AddCommand( "exec", Exec_f, CMD_FL_SYSTEM, "executes a config file", ArgCompletion_ConfigName );
	AddCommand( "vstr", Vstr_f, CMD_FL_SYSTEM, "inserts the current value of a cvar as command text" );
	AddCommand( "echo", Echo_f, CMD_FL_SYSTEM, "prints text" );
	AddCommand( "parse", Parse_f, CMD_FL_SYSTEM, "prints tokenized string" );
	AddCommand( "wait", Wait_f, CMD_FL_SYSTEM, "delays remaining buffered commands one or more frames" );

	// link in all the commands declared with static idCommandLink variables or CONSOLE_COMMAND macros
	for( idCommandLink* link = CommandLinks(); link != NULL; link = link->next ) {
		AddCommand( link->cmdName_, link->function_, CMD_FL_SYSTEM, link->description_, link->argCompletion_ );
	}

	completionString = "*";

	textLength = 0;
}

void idCmdSystemLocal::Shutdown()
{
	commandDef_t* cmd;

	for( cmd = commands; cmd; cmd = commands ) {
		commands = commands->next;
		Mem_Free( cmd->name );
		Mem_Free( cmd->description );
		delete cmd;
	}

	completionString.Clear();
	completionParms.Clear();
	tokenizedCmds.Clear();
	postReload.Clear();
}

void idCmdSystemLocal::AddCommand( const char* cmdName, cmdFunction_t function, int flags, const char* description, argCompletion_t argCompletion )
{
	commandDef_t* cmd;

	// fail if the command already exists
	for( cmd = commands; cmd; cmd = cmd->next ) {
		if( idStr::Cmp( cmdName, cmd->name ) == 0 ) {
			if( function != cmd->function ) {
				common->Printf( "idCmdSystemLocal::AddCommand: %s already defined\n", cmdName );
			}
			return;
		}
	}

	cmd				   = new( TAG_SYSTEM ) commandDef_t;
	cmd->name		   = Mem_CopyString( cmdName );
	cmd->function	   = function;
	cmd->argCompletion = argCompletion;
	cmd->flags		   = flags;
	cmd->description   = Mem_CopyString( description );
	cmd->next		   = commands;
	commands		   = cmd;
}

void idCmdSystemLocal::RemoveCommand( const char* cmdName )
{
	commandDef_t *cmd, **last;

	for( last = &commands, cmd = *last; cmd; cmd = *last ) {
		if( idStr::Cmp( cmdName, cmd->name ) == 0 ) {
			*last = cmd->next;
			Mem_Free( cmd->name );
			Mem_Free( cmd->description );
			delete cmd;
			return;
		}
		last = &cmd->next;
	}
}

void idCmdSystemLocal::RemoveFlaggedCommands( int flags )
{
	commandDef_t *cmd, **last;

	for( last = &commands, cmd = *last; cmd; cmd = *last ) {
		if( cmd->flags & flags ) {
			*last = cmd->next;
			Mem_Free( cmd->name );
			Mem_Free( cmd->description );
			delete cmd;
			continue;
		}
		last = &cmd->next;
	}
}

void idCmdSystemLocal::CommandCompletion( void ( *callback )( const char* s ) )
{
	commandDef_t* cmd;

	for( cmd = commands; cmd; cmd = cmd->next ) {
		callback( cmd->name );
	}
}

void idCmdSystemLocal::ArgCompletion( const char* cmdString, void ( *callback )( const char* s ) )
{
	commandDef_t* cmd;
	idCmdArgs	  args;

	args.TokenizeString( cmdString, false );

	for( cmd = commands; cmd; cmd = cmd->next ) {
		if( !cmd->argCompletion ) {
			continue;
		}
		if( idStr::Icmp( args.Argv( 0 ), cmd->name ) == 0 ) {
			cmd->argCompletion( args, callback );
			break;
		}
	}
}

void idCmdSystemLocal::ExecuteTokenizedString( const idCmdArgs& args )
{
	commandDef_t *cmd, **prev;

	// execute the command line
	if( !args.Argc() ) {
		return; // no tokens
	}

	// check registered command functions
	for( prev = &commands; *prev; prev = &cmd->next ) {
		cmd = *prev;
		if( idStr::Icmp( args.Argv( 0 ), cmd->name ) == 0 ) {
			// rearrange the links so that the command will be
			// near the head of the list next time it is used
			*prev	  = cmd->next;
			cmd->next = commands;
			commands  = cmd;

			if( ( cmd->flags & ( CMD_FL_CHEAT | CMD_FL_TOOL ) ) && common->IsMultiplayer() && !net_allowCheats.GetBool() ) {
				common->Printf( "Command '%s' not valid in multiplayer mode.\n", cmd->name );
				return;
			}
			// perform the action
			if( !cmd->function ) {
				break;
			} else {
				cmd->function( args );
			}
			return;
		}
	}

	// check cvars
	if( cvarSystem->Command( args ) ) {
		return;
	}

	common->Printf( "Unknown command '%s'\n", args.Argv( 0 ) );
}

void idCmdSystemLocal::ExecuteCommandText( const char* text )
{
	ExecuteTokenizedString( idCmdArgs( text, false ) );
}

void idCmdSystemLocal::InsertCommandText( const char* text )
{
	int len;
	int i;

	len = strlen( text ) + 1;
	if( len + textLength > ( int )sizeof( textBuf ) ) {
		common->Printf( "idCmdSystemLocal::InsertText: buffer overflow\n" );
		return;
	}

	// move the existing command text
	for( i = textLength - 1; i >= 0; i-- ) {
		textBuf[i + len] = textBuf[i];
	}

	// copy the new text in
	memcpy( textBuf, text, len - 1 );

	// add a \n
	textBuf[len - 1] = '\n';

	textLength += len;
}

void idCmdSystemLocal::AppendCommandText( const char* text )
{
	int l;

	l = strlen( text );

	if( textLength + l >= ( int )sizeof( textBuf ) ) {
		common->Printf( "idCmdSystemLocal::AppendText: buffer overflow\n" );
		return;
	}
	memcpy( textBuf + textLength, text, l );
	textLength += l;
}

void idCmdSystemLocal::BufferCommandText( cmdExecution_t exec, const char* text )
{
	switch( exec ) {
		case CMD_EXEC_NOW: {
			ExecuteCommandText( text );
			break;
		}
		case CMD_EXEC_INSERT: {
			InsertCommandText( text );
			break;
		}
		case CMD_EXEC_APPEND: {
			AppendCommandText( text );
			break;
		}
		default: {
			common->FatalError( "idCmdSystemLocal::BufferCommandText: bad exec type" );
		}
	}
}

void idCmdSystemLocal::BufferCommandArgs( cmdExecution_t exec, const idCmdArgs& args )
{
	switch( exec ) {
		case CMD_EXEC_NOW: {
			ExecuteTokenizedString( args );
			break;
		}
		case CMD_EXEC_APPEND: {
			AppendCommandText( "_execTokenized\n" );
			tokenizedCmds.Append( args );
			break;
		}
		default: {
			common->FatalError( "idCmdSystemLocal::BufferCommandArgs: bad exec type" );
		}
	}
}

void idCmdSystemLocal::ExecuteCommandBuffer()
{
	int		  i;
	char*	  text;
	int		  quotes;
	idCmdArgs args;

	while( textLength ) {
		if( wait ) {
			// skip out while text still remains in buffer, leaving it for next frame
			wait--;
			break;
		}

		// find a \n or ; line break
		text = ( char* )textBuf;

		quotes = 0;
		for( i = 0; i < textLength; i++ ) {
			if( text[i] == '"' ) {
				quotes++;
			}
			if( !( quotes & 1 ) && text[i] == ';' ) {
				break; // don't break if inside a quoted string
			}
			if( text[i] == '\n' || text[i] == '\r' ) {
				break;
			}
		}

		text[i] = 0;

		if( !idStr::Cmp( text, "_execTokenized" ) ) {
			args = tokenizedCmds[0];
			tokenizedCmds.RemoveIndex( 0 );
		} else {
			args.TokenizeString( text, false );
		}

		// delete the text from the command buffer and move remaining commands down
		// this is necessary because commands (exec) can insert data at the
		// beginning of the text buffer

		if( i == textLength ) {
			textLength = 0;
		} else {
			i++;
			textLength -= i;
			memmove( text, text + i, textLength );
		}

		// execute the command line that we have already tokenized
		ExecuteTokenizedString( args );
	}
}

void idCmdSystemLocal::ArgCompletion_FolderExtension( const idCmdArgs& args, void ( *callback )( const char* s ), const char* folder, int stripFolder, ... )
{
	int			i;
	idStr		string;
	const char* extension;
	va_list		argPtr;

	string = args.Argv( 0 );
	string += " ";
	string += args.Argv( 1 );

	if( string.Icmp( completionString ) != 0 ) {
		idStr		parm, path;
		idFileList* names;

		completionString = string;
		completionParms.Clear();

		parm = args.Argv( 1 );
		parm.ExtractFilePath( path );
		if( stripFolder || path.Length() == 0 ) {
			path = folder + path;
		}
		path.StripTrailing( '/' );

		// list folders
		names = fileSystem->ListFiles( path, "/", true, true );
		for( i = 0; i < names->GetNumFiles(); i++ ) {
			idStr name = names->GetFile( i );
			if( stripFolder ) {
				name.Strip( folder );
			} else {
				name.Strip( "/" );
			}
			name = args.Argv( 0 ) + ( " " + name ) + "/";
			completionParms.Append( name );
		}
		fileSystem->FreeFileList( names );

		// list files
		va_start( argPtr, stripFolder );
		for( extension = va_arg( argPtr, const char* ); extension; extension = va_arg( argPtr, const char* ) ) {
			names = fileSystem->ListFiles( path, extension, true, true );
			for( i = 0; i < names->GetNumFiles(); i++ ) {
				idStr name = names->GetFile( i );
				if( stripFolder ) {
					name.Strip( folder );
				} else {
					name.Strip( "/" );
				}

				// RB: skip entries that we don't need
				if( idStr::FindText( name, "autosave" ) != -1 || idStr::FindText( name, "_extra_ents" ) != -1 ) {
					continue;
				}

				name = args.Argv( 0 ) + ( " " + name );
				completionParms.Append( name );
			}
			fileSystem->FreeFileList( names );
		}
		va_end( argPtr );
	}
	for( i = 0; i < completionParms.Num(); i++ ) {
		callback( completionParms[i] );
	}
}

void idCmdSystemLocal::ArgCompletion_DeclName( const idCmdArgs& args, void ( *callback )( const char* s ), int type )
{
	int i, num;

	if( declManager == NULL ) {
		return;
	}
	num = declManager->GetNumDecls( ( declType_t )type );
	for( i = 0; i < num; i++ ) {
		callback( idStr( args.Argv( 0 ) ) + " " + declManager->DeclByIndex( ( declType_t )type, i, false )->GetName() );
	}
}

void idCmdSystemLocal::SetupReloadEngine( const idCmdArgs& args )
{
	BufferCommandText( CMD_EXEC_APPEND, "reloadEngine\n" );
	postReload = args;
}

bool idCmdSystemLocal::PostReloadEngine()
{
	if( !postReload.Argc() ) {
		return false;
	}
	BufferCommandArgs( CMD_EXEC_APPEND, postReload );
	postReload.Clear();
	return true;
}
