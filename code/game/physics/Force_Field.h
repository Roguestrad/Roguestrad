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

#ifndef __FORCE_FIELD_H__
#define __FORCE_FIELD_H__

/*
===============================================================================

	Force field

===============================================================================
*/

enum forceFieldType { FORCEFIELD_UNIFORM, FORCEFIELD_EXPLOSION, FORCEFIELD_IMPLOSION };

enum forceFieldApplyType { FORCEFIELD_APPLY_FORCE, FORCEFIELD_APPLY_VELOCITY, FORCEFIELD_APPLY_IMPULSE };

/*!
	\class idForce_Field
	\brief Represents a force field that applies various types of forces within a defined clip model boundary.

	The idForce_Field class implements a force field that can apply different types of forces such as uniform, explosion, implosion, and random torque within the boundaries defined by a clip model. It
   supports configuration of force application types including force, velocity, or impulse, and can be restricted to affect only players or monsters. The class provides methods to initialize,
   configure, evaluate, and persist the force field state. It inherits from idForce and integrates with the engine's physics and save system through serialization methods.

*/
class idForce_Field : public idForce
{
public:
	CLASS_PROTOTYPE( idForce_Field );

	//! Saves the field force properties to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the field force state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes a new instance of the idForce_Field class with default values.
	idForce_Field();

	//! Destructor for the idForce_Field class that cleans up the associated clip model.
	virtual ~idForce_Field();

	//! Sets the force field to apply a uniform constant force.
	void Uniform( const idVec3& force );

	//! Sets the explosion force magnitude and type for the force field.
	void Explosion( float force );

	//! Sets the force magnitude and type for an implosion effect towards the clip model origin.
	void Implosion( float force );

	//! Sets the magnitude of random torque to be applied.
	void RandomTorque( float force );

	//! Sets the application type for the force field to determine whether it applies force, velocity, or impulse.
	void SetApplyType( const forceFieldApplyType type ) { applyType = type; }

	//! Sets whether the force field only affects players.
	void SetPlayerOnly( bool set ) { playerOnly = set; }

	//! Configures the force field to only affect monsters.
	void SetMonsterOnly( bool set ) { monsterOnly = set; }

	//! Sets the clip model that defines the extents of the force field.
	void SetClipModel( idClipModel* clipModel );

public:
	// common force interface ---------------------

	//! Evaluates the force field at the specified time
	virtual void Evaluate( int time );

private:
	// force properties
	forceFieldType		type;
	forceFieldApplyType applyType;
	float				magnitude;
	idVec3				dir;
	float				randomTorque;
	bool				playerOnly;
	bool				monsterOnly;
	idClipModel*		clipModel;
};

#endif /* !__FORCE_FIELD_H__ */
