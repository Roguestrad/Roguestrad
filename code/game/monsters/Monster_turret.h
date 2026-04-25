/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Harrie van Ginneken

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

#pragma once

/*!
	\class iceMonster_Turret
	\brief Represents anmonster turret AI with various states for waking up, idling, combat, and death.

	This class implements the behavior for anmonster turret AI, inheriting from idAI and defining multiple states for different phases of operation. The turret has distinct states for waking up,
   idling, entering combat, and handling death, each managed by dedicated state handler methods. The class also includes methods for initialization, enemy detection, and light management. The AI logic
   is driven by state machines that transition based on internal conditions and external events such as enemy presence or damage.

*/
class iceMonster_Turret : public idAI
{
	CLASS_PROTOTYPE( iceMonster_Turret );

public:
	//! Initializes the AI system for the monster turret.
	virtual void Init() override;

	//! Initializes the monster turret AI by setting its state to begin.
	virtual void AI_Begin() override;

protected:
	//
	// actions
	//

	void		  destory() {};

	//! Determines whether the monster turret can hit its target enemy from the current barrel position.
	bool		  canHit();
	void		  spawn_light() {};
	void		  light_off() {};
	void		  light_on() {};

	//! Handles the death state for the turret's torso component.
	stateResult_t Torso_Death( stateParms_t* parms );

	//! Executes the idle animation state for the turret's torso and transitions to attack if fire is triggered.
	stateResult_t Torso_Idle( stateParms_t* parms );

	//! Performs the turret's attack animation and sound effects with windup, fire, and winddown stages.
	stateResult_t Torso_Attack( stateParms_t* parms );

	//! Sets the torso animation state to idle and returns a done result.
	stateResult_t Torso_CustomCycle( stateParms_t* parms );

	//! Handles the killed state for the monster turret.
	stateResult_t state_Killed( stateParms_t* parms );

	//! Checks for the presence of an enemy within the specified field of view and sets the enemy if found.
	virtual bool  checkForEnemy( float use_fov ) override;

private:
	//! Sets the state to begin waking up the turret.
	stateResult_t		  state_WakeUp( stateParms_t* parms );

	//! Initializes the turret state and determines whether to wait for an enemy or transition to combat.
	stateResult_t		  state_Begin( stateParms_t* parms );

	//! Handles the idle state for the monster turret, managing sound and light initialization and enemy waiting logic.
	stateResult_t		  state_Idle( stateParms_t* parms );

	//! Handles the combat state for the monster turret AI.
	stateResult_t		  state_Combat( stateParms_t* parms );

	//! Handles the combat attack state for the monster turret AI.
	stateResult_t		  combat_attack( stateParms_t* parms );

	//! Handles the disabled state for the monster turret, managing shutdown sequence and activation wait.
	stateResult_t		  state_Disabled( stateParms_t* parms );

	bool				  fire;
	bool				  attack_monsters;
	idEntity			  light;
	bool				  light_is_on;
	float				  attackTime;

	int					  barrelCount;
	int					  currentBarrel;
	idStr				  currentBarrelStr;

	idList<jointHandle_t> flashJointWorldHandles;
};
