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

#ifndef __WINVAR_H__
#define __WINVAR_H__

#include "Rectangle.h"

static const char* VAR_GUIPREFIX	 = "gui::";
static const int   VAR_GUIPREFIX_LEN = strlen( VAR_GUIPREFIX );

class idWindow;

/*!
	\class idWinVar
	\brief idWinVar serves as a base class for window variables that support GUI interaction and data synchronization.

	The idWinVar class provides a foundation for managing window variables that are used in GUI systems. It maintains a name and a reference to a dictionary for GUI information. The class supports
   operations for setting and getting the variable's name, managing its dictionary, checking if an update is needed, and handling evaluation flags. It also includes pure virtual methods for string
   conversion, data updates, serialization, and size calculation, indicating that this is an abstract base class meant to be inherited by specific implementations. The class is designed to work with a
   GUI dictionary and window objects, facilitating communication between GUI elements and underlying data structures.

*/
class idWinVar
{
public:
	//! Initializes a new instance of the idWinVar class with default values.
	idWinVar();

	//! Destructor for idWinVar that releases the allocated name string.
	virtual ~idWinVar();

	//! Sets the GUI dictionary and name for the window variable.
	void		SetGuiInfo( idDict* gd, const char* _name );

	//! Returns the name of the Windows variable.
	const char* GetName() const
	{
		if( name ) {
			if( guiDict && *name == '*' ) { return guiDict->GetString( &name[1] ); }
			return name;
		}
		return "";
	}

	//! Sets the name of the idWinVar object to the provided string.
	void SetName( const char* _name )
	{
		delete[] name;
		name = NULL;
		if( _name ) {
			name = new( TAG_OLD_UI ) char[strlen( _name ) + 1];
			strcpy( name, _name );
		}
	}

	//! Assigns the value of another idWinVar object to this object.
	idWinVar& operator=( const idWinVar& other )
	{
		guiDict = other.guiDict;
		SetName( other.name );
		return *this;
	}

	//! Returns the dictionary associated with this win variable.
	idDict*				GetDict() const { return guiDict; }

	//! Returns true if the GUI dictionary is not null, indicating an update is needed.
	bool				NeedsUpdate() { return ( guiDict != NULL ); }

	virtual void		Init( const char* _name, idWindow* win ) = 0;
	virtual void		Set( const char* val )					 = 0;
	virtual void		Update()								 = 0;
	virtual const char* c_str() const							 = 0;

	//! Returns the total memory size occupied by this idWinVar instance
	virtual size_t		Size()
	{
		size_t sz = ( name ) ? strlen( name ) : 0;
		return sz + sizeof( *this );
	}

	virtual void  WriteToSaveGame( idFile* savefile )  = 0;
	virtual void  ReadFromSaveGame( idFile* savefile ) = 0;

	virtual float x() const = 0;

	//! Sets the evaluation flag for the WinVar object.
	void		  SetEval( bool b ) { eval = b; }

	//! Returns the evaluation state of the WinVar.
	bool		  GetEval() { return eval; }

protected:
	idDict* guiDict;
	char*	name;
	bool	eval;
};

/*!
	\class idWinBool
	\brief A boolean variable class that integrates with GUI windows and supports serialization.

	This class represents a boolean variable that is associated with a graphical user interface window. It inherits from idWinVar and provides functionality to initialize, update, compare, and assign
   boolean values. The class supports conversion to and from string representations, and can serialize its state to and from save game files. It also provides integration with GUI dictionaries for
   updating values from the interface. The class is designed to be used in environments where boolean state needs to be managed and synchronized with GUI elements.

*/
class idWinBool : public idWinVar
{
public:
	idWinBool() :
		idWinVar() {};
	~idWinBool() {};

	//! Initializes the boolean variable with the specified name and window
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) { data = guiDict->GetBool( GetName() ); }
	}

	//! Compares the boolean value of this object with another boolean value for equality.
	int	  operator==( const bool& other ) { return ( other == data ); }

	//! Assigns a boolean value to the internal data and updates the associated GUI dictionary if present.
	bool& operator=( const bool& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetBool( GetName(), data ); }
		return data;
	}

	//! Assigns the contents of another idWinBool object to this object
	idWinBool& operator=( const idWinBool& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Converts the idWinBool object to a boolean value.
	operator bool() const { return data; }

	//! Sets the boolean value of the idWinBool object from a string representation.
	virtual void Set( const char* val )
	{
		data = ( atoi( val ) != 0 );
		if( guiDict ) { guiDict->SetBool( GetName(), data ); }
	}

	//! Updates the boolean value from the GUI dictionary using the name returned by GetName().
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetBool( s ); }
	}

	//! Returns a string representation of the boolean value as an integer.
	virtual const char* c_str() const { return va( "%i", data ); }

	//! Writes the object's state to a save game file.
	virtual void		WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Loads the object's state from a save game file
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

	virtual float x() const { return data ? 1.0f : 0.0f; };

protected:
	bool data;
};

/*!
	\class idWinStr
	\brief A window string variable class that manages string data for GUI elements with dictionary integration.

	This class extends idWinVar to provide string-specific functionality for window elements. It maintains string data that can be synchronized with a GUI dictionary, allowing for dynamic updates and
   persistence. The class supports standard string operations including assignment, comparison, and conversion to different string types. It provides methods to handle color formatting in strings,
   such as removing color codes and calculating lengths excluding color characters. The implementation supports serialization for save game functionality and integrates with window management through
   initialization and update mechanisms. Memory management is handled through the base class and standard string operations.

*/
class idWinStr : public idWinVar
{
public:
	idWinStr() :
		idWinVar() {};
	~idWinStr() {};

	//! Initializes the window string variable with the given name and window pointer
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) {
			const char* name = GetName();
			if( name[0] == 0 ) {
				data = "";
			} else {
				data = guiDict->GetString( name );
			}
		}
	}

	//! Compares this string with another string for equality and returns the result.
	int	   operator==( const idStr& other ) const { return ( other == data ); }

	//! Compares the string data with another C-style string for equality
	int	   operator==( const char* other ) const { return ( data == other ); }

	//! Assigns the contents of another idStr object to this object and updates the GUI dictionary if present.
	idStr& operator=( const idStr& other )
	{
		data = other;
		if( guiDict ) { guiDict->Set( GetName(), data ); }
		return data;
	}

	//! Assigns the contents of another idWinStr instance to this instance
	idWinStr& operator=( const idWinStr& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Converts the idWinStr object to a C-style string pointer.
	operator const char*() const { return data.c_str(); }

	//! Converts the idWinStr object to a const idStr reference.
	operator const idStr&() const { return data; }

	//! Returns the length of the string data excluding color formatting characters.
	int LengthWithoutColors()
	{
		if( guiDict && name && *name ) { data = guiDict->GetString( GetName() ); }
		return data.LengthWithoutColors();
	}

	//! Returns the length of the string data stored in this idWinStr object
	int Length()
	{
		if( guiDict && name && *name ) { data = guiDict->GetString( GetName() ); }
		return data.Length();
	}

	//! Removes color codes from the string data.
	void RemoveColors()
	{
		if( guiDict && name && *name ) { data = guiDict->GetString( GetName() ); }
		data.RemoveColors();
	}

	//! Returns a null-terminated character array representing the string data
	virtual const char* c_str() const { return data.c_str(); }

	//! Sets the string value and updates the associated GUI dictionary if available.
	virtual void		Set( const char* val )
	{
		data = val;
		if( guiDict ) { guiDict->Set( GetName(), data ); }
	}

	//! Updates the window by retrieving its string data from the GUI dictionary
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetString( s ); }
	}

	//! Returns the total size in bytes of the string data and associated memory usage
	virtual size_t Size()
	{
		size_t sz = idWinVar::Size();
		return sz + data.Allocated();
	}

	//! Writes the sound system state to a save game file.
	virtual void WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );

		int len = data.Length();
		savefile->Write( &len, sizeof( len ) );
		if( len > 0 ) { savefile->Write( data.c_str(), len ); }
	}

	//! Reads saved game data from the provided file handle
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );

		int len;
		savefile->Read( &len, sizeof( len ) );
		if( len > 0 ) {
			data.Fill( ' ', len );
			savefile->Read( &data[0], len );
		}
	}

	// return wether string is emtpy
	virtual float x() const { return data[0] ? 1.0f : 0.0f; };

protected:
	idStr data;
};

/*!
	\class idWinInt
	\brief A window variable class that stores and manages integer values within a graphical user interface system.

	This class represents an integer window variable that can be used to store and manage integer values within a graphical user interface system. It inherits from idWinVar and provides functionality
   for initialization, assignment, conversion, string representation, and persistence. The class maintains synchronization with a GUI dictionary and supports integration with save game systems. It
   allows for updating its internal state based on GUI dictionary values and provides methods for serializing its state to and from save files. The class supports both direct assignment and assignment
   from other idWinInt instances.

*/
class idWinInt : public idWinVar
{
public:
	idWinInt() :
		idWinVar() {};
	~idWinInt() {};

	//! Initializes the window variable with the given name and window pointer.
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) { data = guiDict->GetInt( GetName() ); }
	}

	//! Assigns the value of another integer to this integer and updates the associated GUI dictionary if present.
	int& operator=( const int& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetInt( GetName(), data ); }
		return data;
	}

	//! Assigns the contents of another idWinInt instance to this instance and returns a reference to this instance.
	idWinInt& operator=( const idWinInt& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Converts the idWinInt object to an integer value.
	operator int() const { return data; }

	//! Sets the integer value of the window variable from a string representation
	virtual void Set( const char* val )
	{
		data = atoi( val );
		;
		if( guiDict ) { guiDict->SetInt( GetName(), data ); }
	}

	//! Updates the internal state based on the GUI dictionary value associated with the window name
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetInt( s ); }
	}

	//! Returns a string representation of the integer data stored in the object.
	virtual const char* c_str() const { return va( "%i", data ); }

	//! Writes the object's state to a save game file
	virtual void		WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Reads evaluation and data from a save file
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

	virtual float x() const
	{
		assert( false );
		return 0.0f;
	};

protected:
	int data;
};

/*!
	\class idWinFloat
	\brief A window variable class that manages float values within a GUI context.

	The idWinFloat class extends idWinVar to provide specific functionality for handling float values in a windowing system. It maintains a float value that can be initialized with a name and window
   context, updated from GUI dictionaries, and serialized to save games. The class supports assignment operations from both other idWinFloat instances and float values, and provides conversion to
   float type. It is designed to integrate with GUI systems where float values need to be managed, updated, and persisted across sessions. The class handles synchronization between internal float
   values and external GUI state through the Update method.

*/
class idWinFloat : public idWinVar
{
public:
	idWinFloat() :
		idWinVar() {};
	~idWinFloat() {};

	//! Initializes the float window variable with the specified name and window context
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) { data = guiDict->GetFloat( GetName() ); }
	}

	//! Assigns the contents of another idWinFloat instance to this instance
	idWinFloat& operator=( const idWinFloat& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Assigns the value of other to this float and updates the GUI dictionary if available
	float& operator=( const float& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetFloat( GetName(), data ); }
		return data;
	}

	//! Converts the idWinFloat object to a float value.
	operator float() const { return data; }

	//! Sets the float value of this variable from a string representation.
	virtual void Set( const char* val )
	{
		data = atof( val );
		if( guiDict ) { guiDict->SetFloat( GetName(), data ); }
	}

	//! Updates the float value from the GUI dictionary using the window name as the key.
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetFloat( s ); }
	}

	//! Returns a string representation of the floating-point value stored in the object.
	virtual const char* c_str() const { return va( "%f", data ); }

	//! Writes the float evaluation and data to the specified save file.
	virtual void		WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Reads floating-point evaluation and data from a save game file.
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

	virtual float x() const { return data; };

protected:
	float data;
};

/*!
	\class idWinRectangle
	\brief A window variable class that encapsulates rectangle data with GUI integration and serialization capabilities.

	This class serves as a specialized window variable for managing rectangle data within a graphical user interface. It inherits from idWinVar and provides functionality to initialize with window
   context, update from GUI dictionaries, and serialize to/from save files. The class supports conversion to and from idVec4 representation, and offers accessors for individual rectangle components
   including x, y, width, and height. It maintains synchronization between internal rectangle data and GUI configuration, ensuring that changes persist across save/load operations. The implementation
   allows for direct assignment from various rectangle and vector types, facilitating easy manipulation within the GUI system.

*/
class idWinRectangle : public idWinVar
{
public:
	idWinRectangle() :
		idWinVar() {};
	~idWinRectangle() {};

	//! Initializes the rectangle variable with the given name and window context, loading values from the GUI dictionary if available.
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) {
			idVec4 v = guiDict->GetVec4( GetName() );
			data.x	 = v.x;
			data.y	 = v.y;
			data.w	 = v.z;
			data.h	 = v.w;
		}
	}

	//! Compares this rectangle with another for equality and returns true if they are equal
	int				operator==( const idRectangle& other ) const { return ( other == data ); }

	//! Assigns the contents of another idWinRectangle instance to this instance.
	idWinRectangle& operator=( const idWinRectangle& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Assigns the values from a vector to this rectangle and updates the GUI dictionary if available
	idRectangle& operator=( const idVec4& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetVec4( GetName(), other ); }
		return data;
	}

	//! Assigns the contents of another rectangle to this rectangle
	idRectangle& operator=( const idRectangle& other )
	{
		data = other;
		if( guiDict ) {
			idVec4 v = data.ToVec4();
			guiDict->SetVec4( GetName(), v );
		}
		return data;
	}

	//! Converts the idWinRectangle object to a const idRectangle reference.
	operator const idRectangle&() const { return data; }

	//! Returns the x-coordinate value stored in the data member.
	float	x() const { return data.x; }

	//! Returns the y-coordinate value stored in the data member
	float	y() const { return data.y; }

	//! Returns the width component of the rectangle.
	float	w() const { return data.w; }

	//! Returns the height of the rectangle.
	float	h() const { return data.h; }

	//! Returns the right edge coordinate of the rectangle.
	float	Right() const { return data.Right(); }

	//! Returns the bottom coordinate of the rectangle.
	float	Bottom() const { return data.Bottom(); }

	//! Returns a reference to the idVec4 representation of the rectangle data.
	idVec4& ToVec4()
	{
		static idVec4 ret;
		ret = data.ToVec4();
		return ret;
	}

	//! Sets the rectangle data from a string representation with comma or space separation
	virtual void Set( const char* val )
	{
		if( strchr( val, ',' ) ) {
			sscanf( val, "%f,%f,%f,%f", &data.x, &data.y, &data.w, &data.h );
		} else {
			sscanf( val, "%f %f %f %f", &data.x, &data.y, &data.w, &data.h );
		}
		if( guiDict ) {
			idVec4 v = data.ToVec4();
			guiDict->SetVec4( GetName(), v );
		}
	}

	//! Updates the rectangle data from the GUI dictionary using the window name
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) {
			idVec4 v = guiDict->GetVec4( s );
			data.x	 = v.x;
			data.y	 = v.y;
			data.w	 = v.z;
			data.h	 = v.w;
		}
	}

	//! Returns a null-terminated character array representation of the rectangle data
	virtual const char* c_str() const { return data.ToVec4().ToString(); }

	//! Writes the rectangle data to a save game file
	virtual void		WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Restores the object's state from a save game file
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

protected:
	idRectangle data;
};

/*!
	\class idWinVec2
	\brief A window vector2 variable that interfaces with GUI dictionaries and supports serialization.

	This class represents a vector2 variable that is integrated with GUI window systems, allowing it to retrieve and update values from a dictionary associated with a window context. It supports
   various operations for assignment, comparison, and conversion between vector and string representations. The class maintains synchronization with a GUI dictionary, ensuring that changes to the
   vector are reflected in the dictionary and vice versa. It also provides methods for serialization to and from save game files, enabling persistence of vector2 values within saved game states. The
   class inherits from idWinVar, indicating its role as a specialized variable type within the windowing system.

*/
class idWinVec2 : public idWinVar
{
public:
	idWinVec2() :
		idWinVar() {};
	~idWinVec2() {};

	//! Initializes the vector2 variable with a name and window context, retrieving its value from a dictionary if available.
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) { data = guiDict->GetVec2( GetName() ); }
	}

	//! Compares this idWinVec2 object with another for equality and returns an integer result.
	int		   operator==( const idVec2& other ) const { return ( other == data ); }

	//! Assigns the contents of another idWinVec2 object to this object and returns a reference to this object
	idWinVec2& operator=( const idWinVec2& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Assigns the value of another idVec2 to this idWinVec2 instance and updates the associated GUI dictionary if present.
	idVec2& operator=( const idVec2& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetVec2( GetName(), data ); }
		return data;
	}

	//! Returns the x component of the vector.
	float		 x() const { return data.x; }

	//! Returns the y-component of the vector
	float		 y() const { return data.y; }

	//! Sets the vector2 value from a string representation
	virtual void Set( const char* val )
	{
		if( strchr( val, ',' ) ) {
			sscanf( val, "%f,%f", &data.x, &data.y );
		} else {
			sscanf( val, "%f %f", &data.x, &data.y );
		}
		if( guiDict ) { guiDict->SetVec2( GetName(), data ); }
	}

	//! Returns a const reference to the underlying idVec2 data.
	operator const idVec2&() const { return data; }

	//! Updates the vector2 data from the GUI dictionary using the name returned by GetName.
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetVec2( s ); }
	}

	//! Returns a null-terminated character string representation of the vector data.
	virtual const char* c_str() const { return data.ToString(); }

	//! Sets all components of the vector to zero.
	void				Zero() { data.Zero(); }

	//! Writes the object's data to a save game file
	virtual void		WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Reads object data from a save game file
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

protected:
	idVec2 data;
};

/*!
	\class idWinVec4
	\brief A window variable class that encapsulates a 4D vector value for GUI applications.

	This class extends idWinVar to provide specialized handling of 4D vector values within a windowing system. It maintains a vector4 data member and provides accessors for individual components while
   supporting initialization, assignment, comparison, and serialization operations. The class integrates with GUI dictionaries for automatic synchronization and supports conversion to and from string
   representations. It is designed to work with windowing contexts and provides mechanisms for updating from GUI state and saving/restoring vector values.

*/
class idWinVec4 : public idWinVar
{
public:
	idWinVec4() :
		idWinVar() {};
	~idWinVec4() {};

	//! Initializes the vector4 window variable with the given name and window context.
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) { data = guiDict->GetVec4( GetName() ); }
	}

	//! Compares this vector with another vector for equality and returns true if they are equal.
	int		   operator==( const idVec4& other ) const { return ( other == data ); }

	//! Assigns the contents of another idWinVec4 object to this object and returns a reference to this object
	idWinVec4& operator=( const idWinVec4& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Assigns the contents of another idVec4 to this idWinVec4 instance and updates the associated GUI dictionary if present
	idVec4& operator=( const idVec4& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetVec4( GetName(), data ); }
		return data;
	}

	//! Converts the WinVec4 object to a const reference of Vec4.
	operator const idVec4&() const { return data; }

	//! Returns the x component of the vector.
	float		 x() const { return data.x; }

	//! Returns the y component of the vector
	float		 y() const { return data.y; }

	//! Returns the z-component of the vector.
	float		 z() const { return data.z; }

	//! Returns the w component of the vector
	float		 w() const { return data.w; }

	//! Sets the vector4 data from a string representation
	virtual void Set( const char* val )
	{
		if( strchr( val, ',' ) ) {
			sscanf( val, "%f,%f,%f,%f", &data.x, &data.y, &data.z, &data.w );
		} else {
			sscanf( val, "%f %f %f %f", &data.x, &data.y, &data.z, &data.w );
		}
		if( guiDict ) { guiDict->SetVec4( GetName(), data ); }
	}

	//! Updates the vector4 data from the GUI dictionary using the name returned by GetName.
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetVec4( s ); }
	}

	//! Returns a null-terminated string representation of the vector data
	virtual const char* c_str() const { return data.ToString(); }

	//! Sets all components of the vector to zero.
	void				Zero()
	{
		data.Zero();
		if( guiDict ) { guiDict->SetVec4( GetName(), data ); }
	}

	//! Returns a constant reference to the 3D vector component of this 4D vector.
	const idVec3& ToVec3() const { return data.ToVec3(); }

	//! Writes the vector data to a save game file.
	virtual void  WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Restores the object's state from a save game file
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

protected:
	idVec4 data;
};

/*!
	\class idWinVec3
	\brief A window vector variable that interfaces with GUI elements and supports serialization.

	This class represents a vector variable that is bound to a window GUI element, allowing for synchronization between vector data and graphical interface components. It inherits from idWinVar and
   provides functionality to initialize, update, and serialize vector data. The class supports assignment operations from both other idWinVec3 and idVec3 objects, and includes methods to access
   individual vector components. It maintains a connection to a GUI dictionary for updating values and provides string representation capabilities. The class is designed to work within a GUI system
   where vector properties need to be stored, retrieved, and synchronized with visual elements. Memory management is handled through the base class idWinVar, and the class supports save game
   functionality for persisting vector state.

*/
class idWinVec3 : public idWinVar
{
public:
	idWinVec3() :
		idWinVar() {};
	~idWinVec3() {};

	//! Initializes the vector variable with the given name and window.
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinVar::Init( _name, win );
		if( guiDict ) { data = guiDict->GetVector( GetName() ); }
	}

	//! Compares this idWinVec3 object with another for equality and returns true if they are equal.
	int		   operator==( const idVec3& other ) const { return ( other == data ); }

	//! Assigns the contents of another idWinVec3 instance to this instance.
	idWinVec3& operator=( const idWinVec3& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		return *this;
	}

	//! Assigns the contents of another idVec3 to this idWinVec3 instance and updates the associated GUI dictionary if it exists.
	idVec3& operator=( const idVec3& other )
	{
		data = other;
		if( guiDict ) { guiDict->SetVector( GetName(), data ); }
		return data;
	}

	//! Converts the idWinVec3 object to a const idVec3 reference.
	operator const idVec3&() const { return data; }

	//! Returns the x component of the vector
	float		 x() const { return data.x; }

	//! Returns the y component of the vector.
	float		 y() const { return data.y; }

	//! Returns the z-component of the vector.
	float		 z() const { return data.z; }

	//! Sets the vector components from a string formatted as three space-separated floats.
	virtual void Set( const char* val )
	{
		sscanf( val, "%f %f %f", &data.x, &data.y, &data.z );
		if( guiDict ) { guiDict->SetVector( GetName(), data ); }
	}

	//! Updates the vector data from the GUI dictionary using the name returned by GetName().
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) { data = guiDict->GetVector( s ); }
	}

	//! Returns a null-terminated string representation of the vector data
	virtual const char* c_str() const { return data.ToString(); }

	//! Sets all components of the vector to zero.
	void				Zero()
	{
		data.Zero();
		if( guiDict ) { guiDict->SetVector( GetName(), data ); }
	}

	//! Writes the vector data to a save game file.
	virtual void WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );
		savefile->Write( &data, sizeof( data ) );
	}

	//! Reads the object's data from a save game file.
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );
		savefile->Read( &data, sizeof( data ) );
	}

protected:
	idVec3 data;
};

/*!
	\class idWinBackground
	\brief A class for managing background elements in a windowing system.

	This class extends idWinStr to provide specialized functionality for handling background elements within a graphical user interface. It manages string data representing background properties and
   ensures proper synchronization with associated GUI elements and material references. The class supports initialization with window references, string comparison operations, assignment from other
   instances or strings, and conversion to C-style strings. It also provides methods for updating the background GUI element, setting material pointers, and handling save game serialization.

*/
class idWinBackground : public idWinStr
{
public:
	idWinBackground() :
		idWinStr()
	{
		mat = NULL;
	};
	~idWinBackground() {};

	//! Initializes the background with a name and window reference.
	virtual void Init( const char* _name, idWindow* win )
	{
		idWinStr::Init( _name, win );
		if( guiDict ) { data = guiDict->GetString( GetName() ); }
	}

	//! Compares this string with another string for equality and returns the result.
	int	   operator==( const idStr& other ) const { return ( other == data ); }

	//! Compares the string data of this object with another string for equality.
	int	   operator==( const char* other ) const { return ( data == other ); }

	//! Assigns the value of another idStr object to this object and updates associated GUI and material references
	idStr& operator=( const idStr& other )
	{
		data = other;
		if( guiDict ) { guiDict->Set( GetName(), data ); }
		if( mat ) {
			if( data == "" ) {
				( *mat ) = NULL;
			} else {
				( *mat ) = declManager->FindMaterial( data );
			}
		}
		return data;
	}

	//! Assigns the contents of another idWinBackground instance to this instance.
	idWinBackground& operator=( const idWinBackground& other )
	{
		idWinVar::operator=( other );
		data = other.data;
		mat	 = other.mat;
		if( mat ) {
			if( data == "" ) {
				( *mat ) = NULL;
			} else {
				( *mat ) = declManager->FindMaterial( data );
			}
		}
		return *this;
	}

	//! Converts the idWinBackground object to a C-style string.
	operator const char*() const { return data.c_str(); }

	//! Converts the idWinBackground object to a const reference to idStr.
	operator const idStr&() const { return data; }

	//! Returns the length of the data stored in the idWinBackground object.
	int Length()
	{
		if( guiDict ) { data = guiDict->GetString( GetName() ); }
		return data.Length();
	}

	//! Returns a pointer to the null-terminated character array representing the string data.
	virtual const char* c_str() const { return data.c_str(); }

	//! Sets the value of the background element and updates the associated material if needed.
	virtual void		Set( const char* val )
	{
		data = val;
		if( guiDict ) { guiDict->Set( GetName(), data ); }
		if( mat ) {
			if( data == "" ) {
				( *mat ) = NULL;
			} else {
				( *mat ) = declManager->FindMaterial( data );
			}
		}
	}

	//! Updates the background GUI element by retrieving and applying material data from the GUI dictionary.
	virtual void Update()
	{
		const char* s = GetName();
		if( guiDict && s[0] != '\0' ) {
			data = guiDict->GetString( s );
			if( mat ) {
				if( data == "" ) {
					( *mat ) = NULL;
				} else {
					( *mat ) = declManager->FindMaterial( data );
				}
			}
		}
	}

	//! Returns the total size in bytes of the background window data structure
	virtual size_t Size()
	{
		size_t sz = idWinVar::Size();
		return sz + data.Allocated();
	}

	//! Sets the material pointer for the background.
	void		 SetMaterialPtr( const idMaterial** m ) { mat = m; }

	//! Writes the background sound data to a save game file
	virtual void WriteToSaveGame( idFile* savefile )
	{
		savefile->Write( &eval, sizeof( eval ) );

		int len = data.Length();
		savefile->Write( &len, sizeof( len ) );
		if( len > 0 ) { savefile->Write( data.c_str(), len ); }
	}

	//! Reads background evaluation data and material reference from a save game file
	virtual void ReadFromSaveGame( idFile* savefile )
	{
		savefile->Read( &eval, sizeof( eval ) );

		int len;
		savefile->Read( &len, sizeof( len ) );
		if( len > 0 ) {
			data.Fill( ' ', len );
			savefile->Read( &data[0], len );
		}
		if( mat ) {
			if( len > 0 ) {
				( *mat ) = declManager->FindMaterial( data );
			} else {
				( *mat ) = NULL;
			}
		}
	}

protected:
	idStr			   data;
	const idMaterial** mat;
};

/*!
	\class idMultiWinVar
	\brief Manages a collection of window variables that can be updated and configured together.
*/
class idMultiWinVar : public idList<idWinVar*>
{
public:
	//! Sets the value of all elements in the multi-window variable collection to the specified string.
	void Set( const char* val );

	//! Updates all elements in the multi-window variable collection.
	void Update();

	//! Sets GUI information for all elements in the multi-window variable.
	void SetGuiInfo( idDict* dict );
};

#endif /* !__WINVAR_H__ */
