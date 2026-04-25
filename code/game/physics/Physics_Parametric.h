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

#ifndef __PHYSICS_PARAMETRIC_H__
#define __PHYSICS_PARAMETRIC_H__

/*
===================================================================================

	Parametric physics

	Used for predefined or scripted motion. The motion of an object is completely
	parametrized. By adjusting the parameters an object is forced to follow a
	predefined path. The parametric physics is typically used for doors, bridges,
	rotating fans etc.

===================================================================================
*/

typedef struct parametricPState_s {
	int										time;				  // physics time
	int										atRest;				  // set when simulation is suspended
	idVec3									origin;				  // world origin
	idAngles								angles;				  // world angles
	idMat3									axis;				  // world axis
	idVec3									localOrigin;		  // local origin
	idAngles								localAngles;		  // local angles
	idExtrapolate<idVec3>					linearExtrapolation;  // extrapolation based description of the position over time
	idExtrapolate<idAngles>					angularExtrapolation; // extrapolation based description of the orientation over time
	idInterpolateAccelDecelLinear<idVec3>	linearInterpolation;  // interpolation based description of the position over time
	idInterpolateAccelDecelLinear<idAngles> angularInterpolation; // interpolation based description of the orientation over time
	idCurve_Spline<idVec3>*					spline;				  // spline based description of the position over time
	idInterpolateAccelDecelLinear<float>	splineInterpolate;	  // position along the spline over time
	bool									useSplineAngles;	  // set the orientation using the spline
} parametricPState_t;

/*!
	\class idPhysics_Parametric
	\brief A parametric physics system that manages linear and angular motion through extrapolation, interpolation, and spline-based trajectories.

	This class implements a parametric physics system that controls the motion of objects through various interpolation and extrapolation techniques. It supports linear and angular motion with
   configurable acceleration and deceleration phases, as well as spline-based movement paths. The system maintains state for both linear and angular components, allowing for precise control over
   object positioning and orientation. It integrates with collision detection through clip models and provides mechanisms for saving, restoring, and synchronizing physics state. The class is designed
   to manage motion transitions and ensure smooth animation and physics interactions, particularly useful for objects that follow predefined motion paths or respond to timed events.

*/
class idPhysics_Parametric : public idPhysics_Base
{
public:
	CLASS_PROTOTYPE( idPhysics_Parametric );

	//! Initializes a new instance of the idPhysics_Parametric class with default values.
	idPhysics_Parametric();

	//! Destructor for the idPhysics_Parametric class that cleans up allocated resources.
	~idPhysics_Parametric();

	//! Saves the parametric physics state to the provided save file.
	void					Save( idSaveGame* savefile ) const;

	//! Restores the physics state of a parametric physics object from a save file.
	void					Restore( idRestoreGame* savefile );

	//! Sets the pusher flags for the physics parametric object.
	void					SetPusher( int flags );

	//! Returns true if this physics object is a pusher.
	bool					IsPusher() const;

	/*!
		\brief Initializes linear extrapolation for the physics parametric system with specified parameters.

		This function sets up linear extrapolation for a physics parametric system. It initializes the current extrapolation state with the provided time, duration, base position, speed, and base
	   speed parameters. The function also updates the local origin to the base position and activates the physics system. The extrapolation type determines how the system handles extrapolation beyond
	   the specified time range.

		\param type Type of extrapolation to use
		\param time Start time for the extrapolation
		\param duration Duration of the extrapolation
		\param base Base position for the extrapolation
		\param speed Speed vector for the extrapolation
		\param baseSpeed Base speed vector for the extrapolation
	*/
	void					SetLinearExtrapolation( extrapolation_t type, int time, int duration, const idVec3& base, const idVec3& speed, const idVec3& baseSpeed );

	/*!
		\brief Initializes the angular extrapolation for parametric physics.

		Configures the angular extrapolation parameters for a parametric physics object, setting up the time frame, base angles, and speed values for angular motion prediction. This function activates
	   the physics object after initialization.

		\param type Type of extrapolation to use
		\param time Start time for the extrapolation
		\param duration Duration of the extrapolation
		\param base Base angular values for the extrapolation
		\param speed Speed values for the extrapolation
		\param baseSpeed Base speed values for the extrapolation
	*/
	void					SetAngularExtrapolation( extrapolation_t type, int time, int duration, const idAngles& base, const idAngles& speed, const idAngles& baseSpeed );

	//! Returns the linear extrapolation type used by the parametric physics object.
	extrapolation_t			GetLinearExtrapolationType() const;

	//! Returns the type of extrapolation used for angular motion.
	extrapolation_t			GetAngularExtrapolationType() const;

	/*!
		\brief Initializes a linear interpolation path for physics movement with specified timing and start/end positions.

		Configures the physics system to execute a linear interpolation motion from a starting position to an ending position over a specified duration. The function sets up acceleration and
	   deceleration phases with defined time intervals, and activates the physics simulation to begin the movement. The timing parameters control how the motion is distributed across the total
	   duration, allowing for smooth acceleration and deceleration effects.

		\param time The time at which the interpolation begins
		\param accelTime The duration of the acceleration phase
		\param decelTime The duration of the deceleration phase
		\param duration The total duration of the interpolation
		\param startPos The starting position of the interpolation
		\param endPos The ending position of the interpolation
	*/
	void					SetLinearInterpolation( int time, int accelTime, int decelTime, int duration, const idVec3& startPos, const idVec3& endPos );

	/*!
		\brief Initializes angular interpolation for parametric physics with specified timing and angle values.

		Configures the angular interpolation parameters for a parametric physics object, setting up the animation timing and start and end angles. The function initializes the interpolation state,
	   sets the current local angles to the starting angle, and activates the physics object to begin the interpolation process.

		\param time The time offset for the interpolation
		\param accelTime The acceleration time for the angular interpolation
		\param decelTime The deceleration time for the angular interpolation
		\param duration The total duration of the angular interpolation
		\param startAng The starting angles for the interpolation
		\param endAng The ending angles for the interpolation
	*/
	void					SetAngularInterpolation( int time, int accelTime, int decelTime, int duration, const idAngles& startAng, const idAngles& endAng );

	/*!
		\brief Initializes the parametric physics object to follow a spline trajectory with specified acceleration and deceleration times

		Configures the physics object to move along a provided spline path using the specified acceleration and deceleration timing parameters. The function handles memory management for the existing
	   spline and initializes interpolation parameters based on the spline's time and length characteristics. When a spline is provided, it calculates the total movement duration and initializes the
	   spline interpolation with the given timing settings. The useSplineAngles parameter determines whether the object should orient itself according to the spline's calculated angles

		\param spline Pointer to the spline curve defining the trajectory path, or NULL to clear the current spline
		\param accelTime Time in milliseconds to accelerate to full speed along the spline
		\param decelTime Time in milliseconds to decelerate from full speed when reaching the end of the spline
		\param useSplineAngles Flag indicating whether to orient the object according to the spline's calculated angles
	*/
	void					SetSpline( idCurve_Spline<idVec3>* spline, int accelTime, int decelTime, bool useSplineAngles );

	//! Returns the spline curve associated with this parametric physics object.
	idCurve_Spline<idVec3>* GetSpline() const;

	//! Returns the acceleration value from the current spline interpolation.
	int						GetSplineAcceleration() const;

	//! Returns the deceleration value used for spline interpolation.
	int						GetSplineDeceleration() const;

	//! Returns true if the physics parametric is using spline angles.
	bool					UsingSplineAngles() const;

	//! Returns the local origin of the parametric physics object in the given vector.
	void					GetLocalOrigin( idVec3& curOrigin ) const;

	//! Sets the provided idAngles reference to the current local angles of the physics parametric object.
	void					GetLocalAngles( idAngles& curAngles ) const;

	//! Copies the current angular values into the provided idAngles object.
	void					GetAngles( idAngles& curAngles ) const;

public:
	// common physics interface ----------------------

	/*!
		\brief Sets the collision model for the parametric physics object with specified density and linking parameters.

		This function assigns a collision model to the physics object and links it to the game world. It handles the removal of the previous collision model if it differs from the new one and the
	   freeOld parameter is true. The function ensures the new model is properly linked with the object's current position and orientation.

		\param model The collision model to be set for this physics object
		\param density The density value to be used for the physics calculations
		\param id An identifier for the collision model, default is 0
		\param freeOld Flag indicating whether to free the old collision model if it differs from the new one
		\throws assertion failure if self or model is null
	*/
	void			SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model associated with this parametric physics object.
	idClipModel*	GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models associated with this physics object.
	int				GetNumClipModels() const;

	//! Sets the mass of the parametric physics object, optionally targeting a specific identifier.
	void			SetMass( float mass, int id = -1 );

	//! Returns the mass of the physics object.
	float			GetMass( int id = -1 ) const;

	//! Sets the contents of the collision model.
	void			SetContents( int contents, int id = -1 );

	//! Returns the contents of the collision model associated with this physics parameteric object.
	int				GetContents( int id = -1 ) const;

	//! Returns the bounding box of the physics object, optionally filtered by a specific clip model ID.
	const idBounds& GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics object or its collision model.
	const idBounds& GetAbsBounds( int id = -1 ) const;

	//! Evaluates the parametric physics simulation for a given time step and end time.
	bool			Evaluate( int timeStepMSec, int endTimeMSec );

	//! Interpolates the physics state based on the provided fraction and returns whether the state has changed.
	bool			Interpolate( const float fraction );

	//! Updates the time for the parametric physics simulation and adjusts all trajectory start times accordingly.
	void			UpdateTime( int endTimeMSec );

	//! Returns the current time value stored in the physics parameteric object.
	int				GetTime() const;

	//! Activates the physics parametric object and marks it as active.
	void			Activate();

	//! Returns true if the physics object is at rest.
	bool			IsAtRest() const;

	//! Returns the time when the physics object entered its rest state.
	int				GetRestStartTime() const;

	//! Returns false indicating the physics parametric object is not pushable.
	bool			IsPushable() const;

	//! Saves the current physics state to a backup copy.
	void			SaveState();

	//! Restores the physics state to the previously saved state and updates the collision model link.
	void			RestoreState();

	//! Sets the origin of the physics object, optionally relative to a master object.
	void			SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the parametric physics object, optionally relative to a master object.
	void			SetAxis( const idMat3& newAxis, int id = -1 );

	//! Translates the parametric physics object by the specified vector.
	void			Translate( const idVec3& translation, int id = -1 );

	//! Rotates the physics object by the specified rotation.
	void			Rotate( const idRotation& rotation, int id = -1 );

	//! Returns the origin position of the parametric physics object
	const idVec3&	GetOrigin( int id = 0 ) const;

	//! Returns the axis of the parametric physics object at the specified index.
	const idMat3&	GetAxis( int id = 0 ) const;

	//! Sets the linear velocity for the parametric physics object.
	void			SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Sets the angular velocity of the physics object to the specified value.
	void			SetAngularVelocity( const idVec3& newAngularVelocity, int id = 0 );

	//! Returns the current linear velocity of the parametric physics object
	const idVec3&	GetLinearVelocity( int id = 0 ) const;

	//! Returns the current angular velocity of the physics object.
	const idVec3&	GetAngularVelocity( int id = 0 ) const;

	//! Disables the clip model associated with this physics parameteric object.
	void			DisableClip();

	//! Enables the collision model for the physics parameteric object.
	void			EnableClip();

	//! Unlinks the physics parameteric clip model from the game world.
	void			UnlinkClip();

	//! Links the clip model to the game world.
	void			LinkClip();

	//! Sets the master entity for this physics object, optionally orienting it relative to the master.
	void			SetMaster( idEntity* master, const bool orientated = true );

	//! Returns the blocking information for the physics parametric object.
	const trace_t*	GetBlockingInfo() const;

	//! Returns the entity that is currently blocking this physics object, or NULL if not blocked.
	idEntity*		GetBlockingEntity() const;

	//! Returns the end time of the linear interpolation or extrapolation for the parametric physics object.
	int				GetLinearEndTime() const;

	//! Returns the end time of the angular interpolation or extrapolation.
	int				GetAngularEndTime() const;

	//! Writes the parametric physics state to a snapshot message.
	void			WriteToSnapshot( idBitMsg& msg ) const;

	//! Updates the physics state from a snapshot message
	void			ReadFromSnapshot( const idBitMsg& msg );

private:
	// parametric physics state
	parametricPState_t			current;
	parametricPState_t			saved;

	physicsInterpolationState_t previous;
	physicsInterpolationState_t next;

	// pusher
	bool						isPusher;
	idClipModel*				clipModel;
	int							pushFlags;

	// results of last evaluate
	trace_t						pushResults;
	bool						isBlocked;

	// master
	bool						hasMaster;
	bool						isOrientated;

private:
	//! Tests if the physics object is at rest.
	bool TestIfAtRest() const;

	//! Sets the physics object to rest state and deactivates it.
	void Rest();
};

#endif /* !__PHYSICS_PARAMETRIC_H__ */
