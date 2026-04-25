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

#ifndef __GAME_SECURITYCAMERA_H__
#define __GAME_SECURITYCAMERA_H__

/*!
	\class idSecurityCamera
	\brief A security camera entity that scans its environment and responds to player presence.

	The idSecurityCamera class represents a security camera entity that performs sweeping scans of its environment to detect player presence. The camera can operate in different alert modes and has
   visual and auditory feedback for player detection. It manages its own rendering, physics, and animation states during operation. The class handles initialization through Spawn, state persistence
   via Save/Restore, and updates through Think. The camera can be destroyed through the Killed method which handles physics and visual effects, or through the Pain method which handles damage events.
   Visual representation is updated through Present, and the camera can be controlled through various event handlers that manage its sweep animation and alert status. The camera's field of view can be
   debug drawn, and its sweep behavior can be configured through sweep speed and axis parameters.

*/
class idSecurityCamera : public idEntity
{
public:
	CLASS_PROTOTYPE( idSecurityCamera );

	//! Initializes the security camera's properties and starts its sweeping animation.
	void				  Spawn();

	//! Saves the security camera's state to a save file.
	void				  Save( idSaveGame* savefile ) const;

	//! Restores the security camera's state from a save file.
	void				  Restore( idRestoreGame* savefile );

	//! Updates the security camera's state and performs relevant actions based on its alert mode and visibility of the player.
	virtual void		  Think();

	//! Returns the render view for the security camera with updated field of view and orientation.
	virtual renderView_t* GetRenderView();

	/*!
		\brief Handles the destruction of a security camera when it is killed by an attacker.

		This function is called when a security camera entity is destroyed. It stops any active sounds, plays a destruction effect if defined, and initializes the physics for the camera to fall and
	   interact with the environment. The camera is set to a non-sweeping state and its physics are updated to allow it to drop to the floor with appropriate gravity and collision properties.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage that was dealt
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
	*/
	virtual void		  Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	/*!
		\brief Handles damage received by the security camera, optionally playing a damage effect.

		This function is called when the security camera takes damage. It retrieves the damage effect from the spawn arguments and plays it if one is defined. The function always returns true,
	   indicating that the damage was processed successfully.

		\param inflictor The entity that caused the damage
		\param attacker The entity that inflicted the damage
		\param damage The amount of damage taken
		\param dir The direction from which the damage came
		\param location The location on the entity that was hit
		\return Always returns true to indicate the damage was processed successfully
	*/
	virtual bool		  Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );

	//! Updates the security camera's visual representation in the render world.
	virtual void		  Present();

private:
	enum { SCANNING, LOSINGINTEREST, ALERT, ACTIVATED };

	float				angle;
	float				sweepAngle;
	int					modelAxis;
	bool				flipAxis;
	float				scanDist;
	float				scanFov;

	float				sweepStart;
	float				sweepEnd;
	bool				negativeSweep;
	bool				sweeping;
	int					alertMode;
	float				stopSweeping;
	float				scanFovCos;

	idVec3				viewOffset;

	int					pvsArea;
	idPhysics_RigidBody physicsObj;
	idTraceModel		trm;

	//! Initiates the sweeping animation and sound for the security camera.
	void				StartSweep();

	//! Determines whether the security camera can see any player within its scan range and field of view.
	bool				CanSeePlayer();

	//! Sets the alert mode of the security camera to the specified status.
	void				SetAlertMode( int status );

	//! Draws the field of view for the security camera using debug lines
	void				DrawFov();
	const idVec3		GetAxis() const;

	//! Returns the sweep speed value for the security camera.
	float				SweepSpeed() const;

	//! Reverses the sweep direction of the security camera.
	void				Event_ReverseSweep();

	//! Continues the security camera sweep animation.
	void				Event_ContinueSweep();

	//! Pauses the security camera sweeping animation and schedules a reverse sweep event.
	void				Event_Pause();

	//! Handles the alert event for a security camera by activating it and scheduling a continuation sweep.
	void				Event_Alert();

	//! Adds a light to the security camera.
	void				Event_AddLight();
};

#endif /* !__GAME_SECURITYCAMERA_H__ */
