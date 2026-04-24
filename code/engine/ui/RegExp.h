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

#ifndef __REGEXP_H__
#define __REGEXP_H__

class idTokenParser;
class idWindow;
class idWinVar;

/*!
	\class idRegister
	\brief The idRegister class manages register configurations and data for variable storage and retrieval.

	This class provides functionality for managing register values, including initialization, copying, enabling/disabling, and serialization to save files. The register can be configured with a name
   and type, and supports operations to set and get values from arrays of floats. It is designed to handle variable data in a structured way, allowing for persistence through save game operations. The
   class supports different register types and can be enabled or disabled to control whether the register is active. The implementation uses inline functions for performance-critical operations.

*/
class idRegister
{
public:
	//! Initializes a new instance of the idRegister class.
	idRegister();
	idRegister( const char* p, int t );

	enum REGTYPE { VEC4 = 0, FLOAT, BOOL, INT, STRING, VEC2, VEC3, RECTANGLE, NUMTYPES };
	static int	   REGCOUNT[NUMTYPES];

	bool		   enabled;
	short		   type;
	idStr		   name;
	int			   regCount;
	unsigned short regs[4];
	idWinVar*	   var;

	//! Sets register values from variable data into the provided registers array.
	void		   SetToRegs( float* registers );

	//! Copies register values into a variable based on the register type and configuration.
	void		   GetFromRegs( float* registers );

	//! Copies register values from the source register object to this register object.
	void		   CopyRegs( idRegister* src );

	//! Enables or disables the register based on the provided boolean value.
	void		   Enable( bool b ) { enabled = b; }

	//! Writes the register state to a save game file.
	void		   WriteToSaveGame( idFile* savefile );

	//! Restores the state of the register from a save game file.
	void		   ReadFromSaveGame( idFile* savefile );
};

ID_INLINE idRegister::idRegister()
{
}

ID_INLINE idRegister::idRegister( const char* p, int t )
{
	name = p;
	type = t;
	assert( t >= 0 && t < NUMTYPES );
	regCount = REGCOUNT[t];
	enabled	 = ( type == STRING ) ? false : true;
	var		 = NULL;
};

ID_INLINE void idRegister::CopyRegs( idRegister* src )
{
	regs[0] = src->regs[0];
	regs[1] = src->regs[1];
	regs[2] = src->regs[2];
	regs[3] = src->regs[3];
}

/*!
	\class idRegisterList
	\brief A container for managing a collection of named registers with various data types.

	The idRegisterList class provides functionality for storing, retrieving, and managing a collection of registers that can hold different types of data. It supports adding registers with values
   parsed from token streams, initializing registers with predefined data, and looking up registers by name. The class maintains an internal hash table for efficient register lookup and provides
   methods for serializing register states to and from save files. Registers can be reset to clear all stored data, and the class handles both textual and vector-based initialization of register
   values.

*/
class idRegisterList
{
public:
	//! Initializes a new instance of the idRegisterList class.
	idRegisterList();
	~idRegisterList();

	/*!
		\brief Adds a register to the register list with the specified name, type, and associated parser and window data

		This function adds a new register to the register list if it doesn't already exist, or updates an existing register with new values. The register is initialized with the provided name and
	   type, and its value is parsed from the token parser based on the register type. For string registers, the value is read as a token from the source and localized. For other types, expressions
	   are parsed from the source and stored in the register. The function handles both cases where the register already exists and where it needs to be created.

		\param name name of the register to add or update
		\param type type identifier for the register
		\param src pointer to the token parser for reading register values
		\param win pointer to the window context for parsing expressions
		\param var pointer to the window variable associated with this register
		\throws assertion failure if the type is out of valid range
	*/
	void		AddReg( const char* name, int type, idTokenParser* src, idWindow* win, idWinVar* var );

	/*!
		\brief Adds a new register to the register list with the specified name, type, and data

		This function adds a new register to the register list if a register with the given name does not already exist. It creates a new idRegister object with the provided name and type, initializes
	   its variables, and populates the register data by evaluating the expression constants from the provided idVec4 data. The register is then added to the hash table for fast lookups.

		\param name name of the register to add
		\param type type identifier for the register
		\param data vector data to initialize the register with
		\param win window context used to evaluate expression constants
		\param var window variable associated with this register
		\throws assertion failure if the type is out of valid range
	*/
	void		AddReg( const char* name, int type, idVec4 data, idWindow* win, idWinVar* var );

	//! Finds and returns a register with the specified name from the register list
	idRegister* FindReg( const char* name );

	//! Sets register values for all contained register objects from the provided float array.
	void		SetToRegs( float* registers );

	//! Copies register values from the provided array into the list of register objects.
	void		GetFromRegs( float* registers );

	//! Resets the register list by clearing all registers and the register hash.
	void		Reset();

	//! Writes the register list data to a save game file
	void		WriteToSaveGame( idFile* savefile );

	//! Restores the state of register entries from a save game file
	void		ReadFromSaveGame( idFile* savefile );

private:
	idList<idRegister*> regs;
	idHashIndex			regHash;
};

ID_INLINE idRegisterList::idRegisterList()
{
	regs.SetGranularity( 4 );
	regHash.SetGranularity( 4 );
	regHash.Clear( 32, 4 );
}

ID_INLINE idRegisterList::~idRegisterList()
{
}

#endif /* !__REGEXP_H__ */
