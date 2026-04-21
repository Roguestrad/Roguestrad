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

#ifndef __CMDARGS_H__
#define __CMDARGS_H__

/*!
	\class idCmdArgs
	\brief Command arguments parsing and management class for handling command line arguments and tokenized strings.

	The idCmdArgs class provides a robust interface for parsing, storing, and managing command line arguments and tokenized strings. It supports both initialization from raw text and dynamic appending
   of arguments, making it suitable for processing command input from various sources such as console commands, configuration files, or command line parameters. The class handles argument tokenization
   with support for preserving string literals and managing argument counts, while providing safe access methods through Argc() and Argv() functions. It also offers functionality to concatenate
   argument ranges into single strings with optional escaping for re-tokenization, which is useful for command reconstruction or forwarding. The Clear() method allows resetting the argument list, and
   GetArgs() provides direct access to the internal argument vector for integration with other systems that require array-based argument access. The implementation maintains internal buffers with
   limits and ensures safe bounds checking during argument access operations.

*/
class idCmdArgs
{
public:
	/*!
		\brief Initializes an empty command arguments object.

		The constructor initializes the command arguments object with zero arguments. This sets up the object for subsequent use where arguments can be added or parsed.

	*/
	idCmdArgs() { argc = 0; }

	/*!
		\brief Initializes the command arguments object by tokenizing the input text.

		Constructs an idCmdArgs object by parsing the provided text into individual command arguments. The keepAsStrings parameter determines whether the arguments should be stored as strings or
	   converted to their appropriate types. This constructor is typically used to process command line arguments or command strings from configuration files.

		\param text Input string containing the command arguments to be parsed
		\param keepAsStrings Flag indicating whether to keep arguments as strings or convert them to their appropriate types
	*/
	idCmdArgs( const char* text, bool keepAsStrings ) { TokenizeString( text, keepAsStrings ); }

	void			   operator=( const idCmdArgs& args );

	/*!
		\brief Returns the number of arguments in the command line arguments list

		This function provides access to the argument count stored in the idCmdArgs object. It is commonly used in command-line argument parsing to determine how many parameters were passed to a
	   command. The returned value corresponds to the total number of arguments including the command name itself. This is typically used in conjunction with Argv() to retrieve individual argument
	   values.

		\return The integer count of arguments in the command line arguments list
	*/
	int				   Argc() const { return argc; }

	/*!
		\brief Returns the argument string at the specified index, or an empty string if the index is out of bounds

		The Argv method retrieves the argument string at the given index from the command line arguments stored in the idCmdArgs object. If the provided index is negative or greater than or equal to
	   the total number of arguments (argc), it returns an empty string instead of NULL. This behavior ensures safe access to command line arguments without requiring explicit bounds checking by the
	   caller

		\param arg The index of the argument to retrieve
		\return A pointer to the argument string at the specified index, or an empty string if the index is out of bounds
	*/
	const char*		   Argv( int arg ) const { return ( arg >= 0 && arg < argc ) ? argv[arg] : ""; }

	/*!
		\brief Returns a single string containing arguments from start to end, with optional escaping for re-tokenization

		This function constructs a single string from a range of command arguments stored in the idCmdArgs object. It can optionally escape special characters to prepare the string for
	   re-tokenization. The function handles bounds checking for the start and end indices, ensuring they remain within valid ranges. When escapeArgs is true, backslashes are doubled and the resulting
	   string is wrapped in double quotes to make it suitable for parsing by the command system again.

		\param start starting argument index, defaults to 1
		\param end ending argument index, defaults to -1 (last argument)
		\param escapeArgs if true, escapes special characters and wraps result in quotes for re-tokenization
		\return const char * pointing to a static buffer containing the concatenated arguments
		\throws assertion failure if argc is greater than or equal to MAX_COMMAND_ARGS
	*/
	const char*		   Args( int start = 1, int end = -1, bool escapeArgs = false ) const;

	/*!
		\brief Parses a null-terminated string into command-line arguments, separating tokens by whitespace and comments while optionally preserving string literals.

		This function tokenizes a given string into individual arguments that can be used for command processing. It handles various parsing scenarios including negative numbers, cvar expansion, and
	   maintains a limit on the number of arguments. The keepAsStrings flag controls whether punctuation is considered as token separators or if only whitespace and comments are used for tokenization.
	   The function ensures that argument parsing does not exceed internal buffers.

		\param text The null-terminated string to tokenize into arguments
		\param keepAsStrings If true, treats punctuation as part of tokens and only uses whitespace and comments for separation
	*/
	void			   TokenizeString( const char* text, bool keepAsStrings );

	/*!
		\brief Appends a new argument to the command line arguments list.

		This function adds a new argument to the internal command line argument storage. It manages the argument count and updates the argument pointers accordingly. The function ensures that the
	   maximum number of command arguments is not exceeded. When adding the first argument, it initializes the argument list. For subsequent arguments, it calculates the correct memory offset within
	   the tokenized buffer and copies the new text.

		\param text The text to append as a new command argument
	*/
	void			   AppendArg( const char* text );

	//! Clears the command arguments list.
	void			   Clear() { argc = 0; }

	/*!
		\brief Returns a pointer to the argument vector and sets the argument count

		This function provides access to the internal argument vector stored in the idCmdArgs object. It writes the current argument count to the provided integer pointer and returns a pointer to the
	   first element of the argument vector. The returned pointer points to an array of string pointers, where each string represents a command line argument. The function is typically used during
	   command line parsing to obtain the arguments that were tokenized from a command line string.

		\param argc pointer to an integer where the argument count will be stored
		\return Pointer to the first element of the argument vector as an array of const char pointers
	*/
	const char* const* GetArgs( int* argc );

private:
	static const int MAX_COMMAND_ARGS	= 64;
	static const int MAX_COMMAND_STRING = 2 * MAX_STRING_CHARS;

	int				 argc;							// number of arguments
	char*			 argv[MAX_COMMAND_ARGS];		// points into tokenized
	char			 tokenized[MAX_COMMAND_STRING]; // will have 0 bytes inserted
};

#endif /* !__CMDARGS_H__ */
