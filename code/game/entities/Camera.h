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

#ifndef __GAME_CAMERA_H__
#define __GAME_CAMERA_H__

/*!
	\class idCamera
	\brief A camera entity that manages rendering view parameters and visualization.
*/
class idCamera : public idEntity
{
public:
	ABSTRACT_PROTOTYPE( idCamera );

	//! Initializes the camera object.
	void				  Spawn();
	virtual void		  GetViewParms( renderView_t* view ) = 0;

	//! Returns the render view for this camera entity.
	virtual renderView_t* GetRenderView();
	virtual void		  Stop() {};
};

/*!
	\class idCameraView
	\brief The idCameraView class manages camera behavior and view parameters for rendering within the engine.

	This class extends idCamera to provide specific functionality for managing camera views, including saving and restoring state, activating the camera, setting attachments, and retrieving view
   parameters for rendering. It handles camera initialization, target management, and field of view configuration. The class supports integration with save game systems and can be triggered to
   activate or deactivate its camera view. Camera view parameters are populated into a render view structure for use in the rendering pipeline. The class is designed to be instantiated and managed
   through the engine's entity system.

*/
class idCameraView : public idCamera
{
public:
	CLASS_PROTOTYPE( idCameraView );

	//! Initializes a new instance of the idCameraView class.
	idCameraView();

	//! Saves the camera view configuration to a save game file.
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the camera view state from a save game file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the camera view by setting up its target and field of view.
	void		 Spawn();

	//! Retrieves the camera view parameters and populates the provided render view structure.
	virtual void GetViewParms( renderView_t* view );

	//! Stops the camera view and activates targets.
	virtual void Stop();

protected:
	//! Activates the camera view, setting it as the current camera or clearing it based on trigger state and current camera settings.
	void	  Event_Activate( idEntity* activator );

	//! Sets the attachments for the camera view.
	void	  Event_SetAttachments();

	//! Sets the camera attachment entity based on the specified spawn argument.
	void	  SetAttachment( idEntity** e, const char* p );
	float	  fov;
	idEntity* attachedTo;
	idEntity* attachedView;
};

/*
===============================================================================

A camera which follows a path defined by an animation.

===============================================================================
*/

typedef struct {
	idCQuat q;
	idVec3	t;
	float	fov;
} cameraFrame_t;

/*!
	\class idCameraAnim
	\brief A camera animation class that handles playback and management of animated camera sequences.

	The idCameraAnim class extends idCamera to provide functionality for loading, playing, and managing camera animations. It supports multiple animation formats including MD5 and GLTF, with optional
   binary caching for performance. The class handles the lifecycle of camera animations from loading through playback to cleanup. It integrates with the rendering system by providing view parameters
   for the current animation state. The class manages its own internal state and provides events for controlling animation playback and setting callbacks for animation completion or other events. It
   can be activated and deactivated through entity interactions, making it suitable for use in game environments where camera behavior needs to be dynamically controlled.

*/
class idCameraAnim : public idCamera
{
public:
	CLASS_PROTOTYPE( idCameraAnim );

	//! Initializes a new instance of the idCameraAnim class with default values.
	idCameraAnim();

	//! Destroys the camera animation and clears it from the game camera if it was set.
	~idCameraAnim();

	//! Saves the camera animation data to a save game file
	void		 Save( idSaveGame* savefile ) const;

	//! Restores the camera animation state from a save game file.
	void		 Restore( idRestoreGame* savefile );

	//! Initializes the camera animation object.
	void		 Spawn();

	//! Populates the render view parameters based on the camera animation state
	virtual void GetViewParms( renderView_t* view );

private:
	int					  threadNum;
	idVec3				  offset;
	int					  frameRate;
	int					  starttime;
	int					  cycle;
	idList<int>			  cameraCuts;
	idList<cameraFrame_t> camera;
	idEntityPtr<idEntity> activator;

	//! Starts the camera animation and initializes its playback.
	void				  Start();

	//! Stops the camera animation and cleans up associated resources.
	void				  Stop();

	//! Performs camera animation think logic.
	void				  Think();

	//! Loads a camera animation from a file, either in MD5 format or GLTF format, with optional binary caching.
	void				  LoadAnim();

	//! Starts the camera animation.
	void				  Event_Start();

	//! Stops the camera animation.
	void				  Event_Stop();

	//! Sets a callback for the camera animation.
	void				  Event_SetCallback();

	//! Toggles the camera animation state based on whether it is already active.
	void				  Event_Activate( idEntity* activator );

	//! Loads a camera animation from a GLTF file with the specified animation name.
	void				  gltfLoadAnim( idStr gltfFileName, idStr animName );

	//! Writes binary camera animation data to the specified file.
	void				  WriteBinaryCamAnim( idFile* file, ID_TIME_T* _timeStamp = NULL );

	//! Loads a binary camera animation from a file and returns true if successful.
	bool				  LoadBinaryCamAnim( idFile* file, const ID_TIME_T sourceTimeStamp );
};

#endif /* !__GAME_CAMERA_H__ */
