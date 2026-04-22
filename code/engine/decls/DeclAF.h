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

#ifndef __DECLAF_H__
#define __DECLAF_H__

/*
===============================================================================

	Articulated Figure

===============================================================================
*/

class idDeclAF;

typedef enum {
	DECLAF_CONSTRAINT_INVALID,
	DECLAF_CONSTRAINT_FIXED,
	DECLAF_CONSTRAINT_BALLANDSOCKETJOINT,
	DECLAF_CONSTRAINT_UNIVERSALJOINT,
	DECLAF_CONSTRAINT_HINGE,
	DECLAF_CONSTRAINT_SLIDER,
	DECLAF_CONSTRAINT_SPRING
} declAFConstraintType_t;

typedef enum { DECLAF_JOINTMOD_AXIS, DECLAF_JOINTMOD_ORIGIN, DECLAF_JOINTMOD_BOTH } declAFJointMod_t;

typedef bool ( *getJointTransform_t )( void* model, const idJointMat* frame, const char* jointName, idVec3& origin, idMat3& axis );

/*!
	\class idAFVector
	\brief Represents a vector in a physics simulation system that can be defined by coordinates, joints, or bone calculations.

	The idAFVector class encapsulates vector definitions that can be specified in multiple ways including direct coordinates, joint positions, bone center calculations, or bone direction calculations.
   It supports parsing from a text source, finalizing calculations using joint transformations, and serialization to files. The class handles different vector types through internal state management
   and provides methods for string representation and direct vector access. The Finish method performs the actual vector computation based on the specified type, using a callback mechanism to retrieve
   joint transformations and handling various vector calculation modes. The class maintains the computed vector value internally and allows both const and non-const access to this value.

*/
class idAFVector
{
public:
	enum { VEC_COORDS = 0, VEC_JOINT, VEC_BONECENTER, VEC_BONEDIR } type;
	idStr joint1;
	idStr joint2;

public:
	//! Initializes an idAFVector object with zero coordinates and positive sign.
	idAFVector();

	//! Parses a vector definition from a lexer input source.
	bool		  Parse( idLexer& src );

	/*!
		\brief Finalizes the vector calculation by processing the specified type and returning the computed vector value

		This function completes the vector calculation based on the type of vector specification stored in the object. It handles different vector types including coordinates, joint positions, bone
	   center calculations, and bone direction calculations. The function uses the provided joint transformation callback to retrieve joint positions and performs appropriate vector operations based
	   on the vector type. For joint-based calculations, it issues warnings if invalid joints are encountered. The final vector is optionally negated based on the negate flag and is always returned as
	   a valid vector.

		\param fileName Name of the file containing the vector specification
		\param GetJointTransform Callback function to retrieve joint transformations
		\param frame Pointer to the joint frame data
		\param model Pointer to the model data used in joint transformations
		\return Always returns true to indicate successful completion of the vector calculation process
	*/
	bool		  Finish( const char* fileName, const getJointTransform_t GetJointTransform, const idJointMat* frame, void* model ) const;

	//! Writes the vector data to the specified file.
	bool		  Write( idFile* f ) const;

	//! Converts the vector representation to a formatted string based on its type and precision.
	const char*	  ToString( idStr& str, const int precision = 8 );

	//! Returns a constant reference to the internal idVec3 vector stored in the idAFVector object.
	const idVec3& ToVec3() const { return vec; }

	//! Returns a reference to the internal idVec3 vector for the idAFVector object.
	idVec3&		  ToVec3() { return vec; }

private:
	mutable idVec3 vec;
	bool		   negate;
};

/*!
	\class idDeclAF_Body
	\brief Manages the configuration and default values for a physical body in a physics simulation.
*/
class idDeclAF_Body
{
public:
	idStr			 name;
	idStr			 jointName;
	declAFJointMod_t jointMod;
	int				 modelType;
	idAFVector		 v1, v2;
	int				 numSides;
	float			 width;
	float			 density;
	idAFVector		 origin;
	idAngles		 angles;
	int				 contents;
	int				 clipMask;
	bool			 selfCollision;
	idMat3			 inertiaScale;
	float			 linearFriction;
	float			 angularFriction;
	float			 contactFriction;
	idStr			 containedJoints;
	idAFVector		 frictionDirection;
	idAFVector		 contactMotorDirection;

public:
	//! Initializes the body properties with default values from the provided file
	void SetDefault( const idDeclAF* file );
};

/*!
	\class idDeclAF_Constraint
	\brief A class that manages constraint properties for articulated figure simulations.
*/
class idDeclAF_Constraint
{
public:
	idStr				   name;
	idStr				   body1;
	idStr				   body2;
	declAFConstraintType_t type;
	float				   friction;
	float				   stretch;
	float				   compress;
	float				   damping;
	float				   restLength;
	float				   minLength;
	float				   maxLength;
	idAFVector			   anchor;
	idAFVector			   anchor2;
	idAFVector			   shaft[2];
	idAFVector			   axis;
	enum { LIMIT_NONE = -1, LIMIT_CONE, LIMIT_PYRAMID } limit;
	idAFVector limitAxis;
	float	   limitAngles[3];

public:
	//! Initializes the constraint properties with default values based on the provided declaration file.
	void SetDefault( const idDeclAF* file );
};

/*!
	\class idDeclAF
	\brief Manages articulated figure declarations for physics-based animation systems.

	This class handles thedeclaration and configuration of articulated figures used in physics-driven animations. It provides functionality for parsing configuration data, managing bodies and
   constraints, and serializing the declaration to and from files. The class supports various constraint types including fixed, ball and socket joints, universal joints, hinges, sliders, and springs.
   It maintains a collection of bodies and constraints that define how different parts of a model connect and move relative to each other. The class also provides utilities for converting between
   string representations and enumerated values for contents and joint modifications, as well as methods for renaming and deleting bodies and constraints while maintaining consistency. The parsing and
   serialization methods ensure that articulated figure configurations can be loaded from text files and saved back to persistent storage.

*/
class idDeclAF : public idDecl
{
	friend class idAFFileManager;

public:
	//! Constructs an idDeclAF object and initializes its data.
	idDeclAF();

	//! Destructor for the idDeclAF class that cleans up allocated memory for bodies and constraints.
	virtual ~idDeclAF();

	//! Returns the size in bytes of the idDeclAF class instance
	virtual size_t			Size() const;

	//! Returns the default definition string for an AF declaration.
	virtual const char*		DefaultDefinition() const;

	//! Parses the given text to configure the articulated figure declaration.
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion );

	//! Resets all member variables to their default state.
	virtual void			FreeData();

	//! Finalizes the assembly and constraint processing for a physics-driven animation frame
	virtual void			Finish( const getJointTransform_t GetJointTransform, const idJointMat* frame, void* model ) const;

	//! Saves the affine field declaration to its source file
	bool					Save();

	//! Creates a new body for the articulated figure declaration.
	void					NewBody( const char* name );

	//! Renames a body in the physics simulation and updates all constraint references to the old body name.
	void					RenameBody( const char* oldName, const char* newName );

	//! Deletes a body with the specified name and removes all constraints referencing that body.
	void					DeleteBody( const char* name );

	//! Creates a new constraint with the specified name and adds it to the list of constraints.
	void					NewConstraint( const char* name );

	//! Renames a constraint in the AF declaration from an old name to a new name.
	void					RenameConstraint( const char* oldName, const char* newName );

	//! Removes a constraint with the specified name from the declaration.
	void					DeleteConstraint( const char* name );

	//! Converts a string representation of collision contents into an integer bitfield.
	static int				ContentsFromString( const char* str );

	//! Converts a contents bitmask into a string representation.
	static const char*		ContentsToString( const int contents, idStr& str );

	//! Converts a string representation of a joint modification type into its corresponding enumeration value.
	static declAFJointMod_t JointModFromString( const char* str );

	//! Converts a joint modification type to its corresponding string representation.
	static const char*		JointModToString( declAFJointMod_t jointMod );

public:
	bool												 modified;
	idStr												 model;
	idStr												 skin;
	float												 defaultLinearFriction;
	float												 defaultAngularFriction;
	float												 defaultContactFriction;
	float												 defaultConstraintFriction;
	float												 totalMass;
	idVec2												 suspendVelocity;
	idVec2												 suspendAcceleration;
	float												 noMoveTime;
	float												 noMoveTranslation;
	float												 noMoveRotation;
	float												 minMoveTime;
	float												 maxMoveTime;
	int													 contents;
	int													 clipMask;
	bool												 selfCollision;
	idList<idDeclAF_Body*, TAG_IDLIB_LIST_PHYSICS>		 bodies;
	idList<idDeclAF_Constraint*, TAG_IDLIB_LIST_PHYSICS> constraints;

private:
	//! Parses contents from a lexer stream and converts them to a content value.
	bool ParseContents( idLexer& src, int& c ) const;

	//! Parses a body definition from the given lexer source and adds it to the declaration
	bool ParseBody( idLexer& src );

	//! Parses a fixed constraint definition from the provided lexer.
	bool ParseFixed( idLexer& src );

	//! Parses a ball and socket joint constraint definition from the given lexer.
	bool ParseBallAndSocketJoint( idLexer& src );

	//! Parses a universal joint constraint from the provided lexer
	bool ParseUniversalJoint( idLexer& src );

	//! Parses a hinge constraint definition from the given lexer.
	bool ParseHinge( idLexer& src );

	//! Parses a slider constraint definition from the given lexer input
	bool ParseSlider( idLexer& src );

	//! Parses a spring constraint from the provided lexer input
	bool ParseSpring( idLexer& src );

	//! Parses AF settings from a token stream
	bool ParseSettings( idLexer& src );

	//! Writes the body definition to a file during AF declaration serialization.
	bool WriteBody( idFile* f, const idDeclAF_Body& body ) const;

	//! Writes a fixed constraint definition to the specified file
	bool WriteFixed( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes a ball and socket joint constraint to the specified file
	bool WriteBallAndSocketJoint( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes a universal joint constraint to the specified file
	bool WriteUniversalJoint( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes hinge constraint data to a file
	bool WriteHinge( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes slider constraint data to a file
	bool WriteSlider( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes spring constraint data to a file.
	bool WriteSpring( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes a constraint definition to a file based on its type.
	bool WriteConstraint( idFile* f, const idDeclAF_Constraint& c ) const;

	//! Writes the articulated figure settings to the specified file
	bool WriteSettings( idFile* f ) const;

	//! Rebuilds the text source for an articulated figure declaration.
	bool RebuildTextSource();
};

#endif /* !__DECLAF_H__ */
