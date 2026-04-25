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

/*
===============================================================================

	Grabber Object - Class to extend idWeapon to include functionality for
						manipulating physics objects.

===============================================================================
*/

class idBeam;

/*!
	\class idGrabber
	\brief Manages entity grabbing and dragging functionality with visual beam effects.

	The idGrabber class implements the logic for grabbing and dragging entities within the game world. It handles the initialization of grabber states, updates based on player input, and maintains
   visual beam effects to indicate the grabber's state. The class supports setting drag distances, managing drag operations with specific entities, and applying forces like shaking to grabbed objects.
   It integrates with the game's save and restore system to persist grabber state. The visual beam effects are updated continuously to reflect the current grabber state and target position. The class
   is designed to work in conjunction with player entities and other game objects to provide interactive grabbing mechanics.

*/
class idGrabber : public idEntity
{
public:
	CLASS_PROTOTYPE( idGrabber );

	//! Initializes a new instance of the idGrabber class.
	idGrabber();

	//! Destroys the idGrabber instance and cleans up associated resources.
	~idGrabber();

	//! Saves the grabber state to a save file.
	void Save( idSaveGame* savefile ) const;

	//! Restores the grabber state from a save file.
	void Restore( idRestoreGame* savefile );

	//! Initializes the grabber by setting up beam and beam target entities.
	void Initialize();

	//! Sets the drag distance for the grabber.
	void SetDragDistance( float dist );

	//! Updates the grabber state based on player input and returns the current state.
	int	 Update( idPlayer* player, bool hide );

private:
	idEntityPtr<idEntity> dragEnt; // entity being dragged
	idForce_Grab		  drag;
	idVec3				  saveGravity;

	int					  id;				// id of body being dragged
	idVec3				  localPlayerPoint; // dragged point in player space
	idEntityPtr<idPlayer> owner;
	int					  oldImpulseSequence;
	bool				  holdingAF;
	bool				  shakeForceFlip;
	int					  endTime;
	int					  lastFiredTime;
	int					  dragFailTime;
	int					  startDragTime;
	float				  dragTraceDist;
	int					  savedContents;
	int					  savedClipmask;

	idBeam*				  beam;
	idBeam*				  beamTarget;

	int					  warpId;

	//! Checks if the specified AI name corresponds to a grabbable entity.
	bool				  grabbableAI( const char* aiName );

	//! Initializes a drag operation with the specified entity and identifier.
	void				  StartDrag( idEntity* grabEnt, int id );

	//! Stops the dragging of an entity, either dropping it or throwing it based on the dropOnly parameter.
	void				  StopDrag( bool dropOnly );

	//! Updates the visual beam effects based on the grabber's current state and target position.
	void				  UpdateBeams();

	//! Applies a shaking force to the grabbed entity.
	void				  ApplyShake();
};
