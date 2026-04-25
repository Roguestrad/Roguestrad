/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

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
	\class iceWeaponGrabber
	\brief A class representing an ice weapon grabber that manages weapon states, visual effects, and sound behaviors.

	This class extends iceWeaponObject to implement the behavior of an ice weapon grabber, handling various weapon states including raising, lowering, idle, firing, and reloading. It manages the
   initialization and activation of visual effects such as particle systems and lighting, as well as warning sounds and GUI light updates. The class coordinates with weapon-related states and
   animations to provide a complete interactive weapon experience.

*/
class iceWeaponGrabber : public iceWeaponObject
{
public:
	CLASS_PROTOTYPE( iceWeaponGrabber );

	//! Initializes the ice weapon grabber with the specified weapon
	virtual void  Init( idWeapon* weapon );

	//! Raises the ice weapon grabber animation state.
	stateResult_t Raise( stateParms_t* parms );

	//! Changes the weapon state to holstered after playing the putaway animation.
	stateResult_t Lower( stateParms_t* parms );

	//! Handles the idle state for the ice weapon grabber, managing weapon animation and sound effects based on target detection and weapon state.
	stateResult_t Idle( stateParms_t* parms );

	//! Executes the firing state logic for the ice weapon grabber.
	stateResult_t Fire( stateParms_t* parms );

	//! Completes the reload state transition for the ice weapon grabber.
	stateResult_t Reload( stateParms_t* parms );

private:
	//! Initializes and activates the weapon grabber's visual effects and lighting.
	void				 StartActive();

	//! Stops the active particle and light effects associated with the weapon grabber.
	void				 StopActive();

	//! Initializes the warning sound state for the ice weapon grabber.
	void				 StartWarningSound();

	//! Updates the warning sound for the ice weapon grabber based on elapsed time
	void				 UpdateWarningSound();

	//! Updates the GUI light state based on the grabber's target status
	void				 UpdateGuiLight();

	const idSoundShader* snd_fireloop;
	const idSoundShader* snd_electroloop;
	const idSoundShader* snd_mainfire;
	const idSoundShader* snd_cangrab;
	const idSoundShader* snd_warning;
	const idSoundShader* snd_stopfire;

private:
	float next_attack;

	float fireStartTime;
	bool  warningBeep1;
	bool  warningBeep2;
	bool  warningBeep3;
	bool  warningBeep4;

	float grabberState;
};
