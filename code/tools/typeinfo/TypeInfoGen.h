/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU
General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __TYPEINFOGEN_H__
#define __TYPEINFOGEN_H__

/*
===================================================================================

	Type Info Generator

	- template classes are commented out (different instantiations are not identified)
	- bit fields are commented out (cannot get the address of bit fields)
	- multiple inheritance is not supported (only tracks a single super type)

===================================================================================
*/

/*!
	\class idConstantInfo
	\brief A class for managing constant information.
*/
class idConstantInfo
{
public:
	idStr name;
	idStr type;
	idStr value;
};

/*!
	\class idEnumValueInfo
	\brief A class for storing information about enum values.
*/
class idEnumValueInfo
{
public:
	idStr name;
	int	  value;
};

/*!
	\class idEnumTypeInfo
	\brief Type information container for enumerated types.
*/
class idEnumTypeInfo
{
public:
	idStr					typeName;
	idStr					scope;
	bool					unnamed;
	bool					isTemplate;
	idList<idEnumValueInfo> values;
};

/*!
	\class idClassVariableInfo
	\brief Manages information about class variables for reflection purposes.
*/
class idClassVariableInfo
{
public:
	idStr name;
	idStr type;
	int	  bits;
};

/*!
	\class rvmClassFunctionInfo
	\brief Provides information about class functions for runtime virtual machine operations.
*/
class rvmClassFunctionInfo
{
public:
	idStr returnType;
	idStr name;
	bool  isConst;
	bool  isStatic;
	idStr paramStateInput;
	bool  isValidFunction;
};

/*!
	\class idClassTypeInfo
	\brief Type information management for class hierarchies.
*/
class idClassTypeInfo
{
public:
	idStr						 typeName;
	idStr						 superType;
	idStr						 scope;
	bool						 unnamed;
	bool						 isTemplate;
	idList<idClassVariableInfo>	 variables;
	idList<rvmClassFunctionInfo> functions;
};

/*!
	\class idTypeInfoGen
	\brief A type information generator that parses and creates type metadata from source code.

	This class serves as a parser and generator for type information, processing C++ source files to extract class, enum, and constant definitions. It handles complex language features such as
   inheritance, templates, and scope resolution while building internal representations of type metadata. The generator supports parsing of nested structures, multiple inheritance, and various C++
   access specifiers. It provides functionality for evaluating constant expressions, handling preprocessor defines, and writing generated type information to output files. The class manages its own
   memory and resources, cleaning up allocated objects during destruction.

*/
class idTypeInfoGen
{
public:
	//! Constructs a new idTypeInfoGen instance.
	idTypeInfoGen();

	//! Destructor for idTypeInfoGen that cleans up allocated memory for constants, enums, and classes.
	~idTypeInfoGen();

	//! Adds a define string to the list of defines.
	void AddDefine( const char* define );

	//! Generates type information for classes and structures from a specified file path.
	void CreateTypeInfo( const char* path );

	//! Writes type information to C++ and header files for generated type info.
	void WriteTypeInfo( const char* fileName ) const;

private:
	idStrList				 defines;

	idList<idConstantInfo*>	 constants;
	idList<idEnumTypeInfo*>	 enums;
	idList<idClassTypeInfo*> classes;

	int						 numTemplates;
	int						 maxInheritance;
	idStr					 maxInheritanceClass;

	//! Returns the inheritance depth of a specified class type.
	int						 GetInheritance( const char* typeName ) const;

	//! Evaluates a string expression and returns its integer value.
	int						 EvaluateIntegerString( const idStr& string );

	//! Evaluates a float value from a string expression.
	float					 EvaluateFloatString( const idStr& string );

	//! Finds and returns a constant info object by its name from the type info generator's constants list.
	idConstantInfo*			 FindConstant( const char* name );

	//! Retrieves the integer value of a constant identified by scope and name from the parser input.
	int						 GetIntegerConstant( const char* scope, const char* name, idParser& src );

	//! Retrieves a float constant value by name from a specified scope or fallback scope.
	float					 GetFloatConstant( const char* scope, const char* name, idParser& src );

	//! Parses and evaluates the size of an array from a parser token stream.
	int						 ParseArraySize( const char* scope, idParser& src );

	//! Parses a constant value from a parser stream and appends it to a string.
	void					 ParseConstantValue( const char* scope, idParser& src, idStr& value );

	/*!
		\brief Parses an enumeration type from the provided source parser and returns its type information.

		This function processes an enumeration definition from the input stream, extracting the enumeration name, values, and their associated integer values. It handles both named and unnamed enums,
	   supports template enums, and can process typedef declarations for enums. The function also manages constant definitions for each enum value and ensures proper parsing of the enum's curly brace
	   delimited list of values.

		\param scope The namespace or scope in which the enum is defined
		\param isTemplate Indicates whether the enum is a template specialization
		\param typeDef Indicates whether the enum is declared with typedef
		\param src The parser object used to read and process the input source
		\return A pointer to the newly created idEnumTypeInfo object representing the parsed enum, or NULL if parsing fails
	*/
	idEnumTypeInfo*			 ParseEnumType( const char* scope, bool isTemplate, bool typeDef, idParser& src );

	/*!
		\brief Parses a class, struct, or namespace type definition from the provided parser, including inheritance and template information.

		This function is responsible for parsing type definitions such as classes, structs, and namespaces from a parser stream. It handles type names, scope resolution, inheritance information, and
	   template arguments. The function constructs an idClassTypeInfo object that represents the parsed type, including its name, scope, base class, and whether it is a template. If the parsing fails
	   at any point, the function returns NULL and cleans up any allocated memory. The function supports multiple inheritance but issues a warning when encountered.

		\param scope The scope in which the class is defined, used for fully-qualified names
		\param templateArgs The template arguments for the class, if any
		\param isTemplate Indicates whether the type being parsed is a template
		\param typeDef Indicates whether the parsing is part of a typedef declaration
		\param src The parser object used to read and parse the type definition
		\return A pointer to the newly created idClassTypeInfo object representing the parsed class, or NULL if parsing fails
		\throws NULL is returned if parsing fails, indicating an error occurred during parsing, such as missing tokens or syntax errors
	*/
	idClassTypeInfo*		 ParseClassType( const char* scope, const char* templateArgs, bool isTemplate, bool typeDef, idParser& src );

	/*!
		\brief Parses a scope of class type information from a parser stream and populates class, enum, and function data structures.

		This function processes tokens from a parser to extract class structure information, including class definitions, enums, templates, and methods. It handles nested scopes, template
	   declarations, and various C++ access specifiers. The parsed information is stored in class and enum collections associated with the type information.

		\param scope The scope name for the class being parsed
		\param isTemplate Flag indicating if the current scope is a template
		\param src The parser object used to read tokens from the source
		\param typeInfo The class type information object being populated
		\throws assertion failure when encountering an unmatched closing brace
	*/
	void					 ParseScope( const char* scope, bool isTemplate, idParser& src, idClassTypeInfo* typeInfo );
};

#endif /* !__TYPEINFOGEN_H__ */
