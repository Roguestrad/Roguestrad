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

#ifndef __ANIM_TESTMODEL_H__
#define __ANIM_TESTMODEL_H__

/*!
	\class idTestModel
	\brief A test model entity for debugging and animation testing purposes.

	This class represents a specialized test model entity that can be used for debugging and animation testing within the game. It inherits from idAnimatedEntity and provides functionality for
   animating test models, managing their physics, and controlling various aspects of their behavior through console commands. The class supports loading and displaying models, playing animations,
   blending between animations, and setting shader parameters or skins. It also handles saving and restoring the test model state and provides methods for advancing or reversing animations through
   console commands. The test model can be made permanent using specific commands and is designed to be removed when the game shuts down or when the object is destroyed. The class implements methods
   for handling footstep sounds and updating the animation and physics during the game loop. This entity is intended for use in development and testing scenarios where visual feedback of animations
   and model behavior is required.

*/
class idTestModel : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idTestModel );

	//! Initializes a new instance of the idTestModel class with default values.
	idTestModel();

	//! Destructor for the idTestModel class that cleans up sound and removes the test model from the game.
	~idTestModel();

	//! Saves the test model state to the provided save file
	void		 Save( idSaveGame* savefile );

	//! Deletes the test model instance during restore operations.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the test model entity with physics and optional head model.
	void		 Spawn();

	//! Returns false to indicate that the script object should not be constructed at spawn time.
	virtual bool ShouldConstructScriptObjectAtSpawn() const;

	//! Advances to the next animation in the sequence
	void		 NextAnim( const idCmdArgs& args );

	//! Moves to the previous animation in the sequence.
	void		 PrevAnim( const idCmdArgs& args );

	//! Advances to the next frame of the animation and prints the animation and frame information.
	void		 NextFrame( const idCmdArgs& args );

	//! Moves to the previous frame of the current animation.
	void		 PrevFrame( const idCmdArgs& args );

	//! Starts playing a specified animation on the test model.
	void		 TestAnim( const idCmdArgs& args );

	//! Sets up blending between two animations for testing purposes.
	void		 BlendAnim( const idCmdArgs& args );

	//! Makes the current test model permanent by keeping it in the game world.
	static void	 KeepTestModel_f( const idCmdArgs& args );

	//! Creates a static model definition in front of the player's current position for testing purposes
	static void	 TestModel_f( const idCmdArgs& args );

	//! Provides argument completion for test model commands by suggesting entity definitions, model definitions, and model file names.
	static void	 ArgCompletion_TestModel( const idCmdArgs& args, void ( *callback )( const char* s ) );

	//! Sets a skin on an existing testModel.
	static void	 TestSkin_f( const idCmdArgs& args );

	//! Sets a shader parameter on an existing test model
	static void	 TestShaderParm_f( const idCmdArgs& args );

	//! Sets the particle stop time for the active test model.
	static void	 TestParticleStopTime_f( const idCmdArgs& args );

	//! Executes the TestAnim function on the active test model with the provided command arguments.
	static void	 TestAnim_f( const idCmdArgs& args );

	//! Completes command line arguments for anim testing by suggesting animation names from the active test model.
	static void	 ArgCompletion_TestAnim( const idCmdArgs& args, void ( *callback )( const char* s ) );

	//! Sets up blending for the active test model using the provided command arguments.
	static void	 TestBlend_f( const idCmdArgs& args );

	//! Advances to the next animation for the active test model.
	static void	 TestModelNextAnim_f( const idCmdArgs& args );

	//! Sets the test model to the previous animation.
	static void	 TestModelPrevAnim_f( const idCmdArgs& args );

	//! Updates the test model to the next frame based on the provided command arguments.
	static void	 TestModelNextFrame_f( const idCmdArgs& args );

	//! Changes the test model to the previous frame based on the provided command arguments.
	static void	 TestModelPrevFrame_f( const idCmdArgs& args );

private:
	idEntityPtr<idEntity> head;
	idAnimator*			  headAnimator;
	idAnim				  customAnim;
	idPhysics_Parametric  physicsObj;
	idStr				  animname;
	int					  anim;
	int					  headAnim;
	int					  mode;
	int					  frame;
	int					  starttime;
	int					  animtime;

	idList<copyJoints_t>  copyJoints;

	//! Updates the test model animation and physics during the game loop
	virtual void		  Think();

	//! Plays a footstep sound effect.
	void				  Event_Footstep();
};

#endif /* !__ANIM_TESTMODEL_H__*/
