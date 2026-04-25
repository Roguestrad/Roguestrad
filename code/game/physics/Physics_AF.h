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

#ifndef __PHYSICS_AF_H__
#define __PHYSICS_AF_H__

/*
===================================================================================

	Articulated Figure physics

	Employs a constraint force based dynamic simulation using a lagrangian
	multiplier method to solve for the constraint forces.

===================================================================================
*/

class idAFConstraint;
class idAFConstraint_Fixed;
class idAFConstraint_BallAndSocketJoint;
class idAFConstraint_BallAndSocketJointFriction;
class idAFConstraint_UniversalJoint;
class idAFConstraint_UniversalJointFriction;
class idAFConstraint_CylindricalJoint;
class idAFConstraint_Hinge;
class idAFConstraint_HingeFriction;
class idAFConstraint_HingeSteering;
class idAFConstraint_Slider;
class idAFConstraint_Line;
class idAFConstraint_Plane;
class idAFConstraint_Spring;
class idAFConstraint_Contact;
class idAFConstraint_ContactFriction;
class idAFConstraint_ConeLimit;
class idAFConstraint_PyramidLimit;
class idAFConstraint_Suspension;
class idAFBody;
class idAFTree;
class idPhysics_AF;

typedef enum {
	CONSTRAINT_INVALID,
	CONSTRAINT_FIXED,
	CONSTRAINT_BALLANDSOCKETJOINT,
	CONSTRAINT_UNIVERSALJOINT,
	CONSTRAINT_HINGE,
	CONSTRAINT_HINGESTEERING,
	CONSTRAINT_SLIDER,
	CONSTRAINT_CYLINDRICALJOINT,
	CONSTRAINT_LINE,
	CONSTRAINT_PLANE,
	CONSTRAINT_SPRING,
	CONSTRAINT_CONTACT,
	CONSTRAINT_FRICTION,
	CONSTRAINT_CONELIMIT,
	CONSTRAINT_PYRAMIDLIMIT,
	CONSTRAINT_SUSPENSION
} constraintType_t;

/*!
	\class idAFConstraint
	\brief Base class for all constraints in a physics simulation system.

	This class serves as the foundation for various constraint types within a physics simulation framework. It provides fundamental functionality for managing constraint properties such as type, name,
   and associated bodies. The class supports operations for setting and retrieving physics simulation data, debugging visualization, force calculation, and geometric transformations. It includes
   mechanisms for serialization through save and restore operations, and maintains data structures for constraint evaluation and friction application. The design enables derived classes to implement
   specific constraint behaviors while inheriting common functionality for constraint management and interaction within the physics system.

*/
class idAFConstraint
{
	friend class idPhysics_AF;
	friend class idAFTree;

public:
	//! Initializes a new instance of the idAFConstraint class with default values.
	idAFConstraint();
	virtual ~idAFConstraint();

	//! Returns the type of this constraint
	constraintType_t GetType() const { return type; }

	//! Returns the name of the constraint
	const idStr&	 GetName() const { return name; }

	//! Returns the first body associated with this constraint.
	idAFBody*		 GetBody1() const { return body1; }

	//! Returns the second body associated with this constraint.
	idAFBody*		 GetBody2() const { return body2; }

	//! Sets the physics simulation for this constraint.
	void			 SetPhysics( idPhysics_AF* p ) { physics = p; }

	//! Returns the multiplier vector for the constraint.
	const idVecX&	 GetMultiplier();

	//! Sets the first body for the constraint.
	virtual void	 SetBody1( idAFBody* body );

	//! Sets the second body for the constraint and notifies the physics system of the change.
	virtual void	 SetBody2( idAFBody* body );

	//! Draws debug information for the constraint.
	virtual void	 DebugDraw();

	//! Calculates and retrieves the force acting on a specified body due to the constraint.
	virtual void	 GetForce( idAFBody* body, idVec6& force );

	//! Translates the constraint by the specified vector.
	virtual void	 Translate( const idVec3& translation );

	//! This function is currently unimplemented and will assert.
	virtual void	 Rotate( const idRotation& rotation );

	//! Sets the center vector to zero.
	virtual void	 GetCenter( idVec3& center );

	//! Saves the constraint type to the specified save file.
	virtual void	 Save( idSaveGame* saveFile ) const;

	//! Restores the constraint state from a saved game file.
	virtual void	 Restore( idRestoreGame* saveFile );

protected:
	constraintType_t type;	  // constraint type
	idStr			 name;	  // name of constraint
	idAFBody*		 body1;	  // first constrained body
	idAFBody*		 body2;	  // second constrained body, NULL for world
	idPhysics_AF*	 physics; // for adding additional constraints like limits

	// simulation variables set by Evaluate
	idMatX			 J1, J2;		// matrix with left hand side of constraint equations
	idVecX			 c1, c2;		// right hand side of constraint equations
	idVecX			 lo, hi, e;		// low and high bounds and lcp epsilon
	idAFConstraint*	 boxConstraint; // constraint the boxIndex refers to
	int				 boxIndex[6];	// indexes for special box constrained variables

	// simulation variables used during calculations
	idMatX			 invI;		 // transformed inertia
	idMatX			 J;			 // transformed constraint matrix
	idVecX			 s;			 // temp solution
	idVecX			 lm;		 // lagrange multipliers
	int				 firstIndex; // index of the first constraint row in the lcp matrix

	struct constraintFlags_s {
		bool allowPrimary	 : 1; // true if the constraint can be used as a primary constraint
		bool frameConstraint : 1; // true if this constraint is added to the frame constraints
		bool noCollision	 : 1; // true if body1 and body2 never collide with each other
		bool isPrimary		 : 1; // true if this is a primary constraint
		bool isZero			 : 1; // true if 's' is zero during calculations
	} fl;

protected:
	//! This function is a placeholder that currently asserts and does nothing.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the constraint using the inverse time step.
	virtual void ApplyFriction( float invTimeStep );

	//! Initializes the size of the constraint data structures.
	void		 InitSize( int size );
};

/*!
	\class idAFConstraint_Fixed
	\brief A constraint that fixes the position and orientation of one body relative to another in a physics simulation.

	This class implements a fixed constraint that eliminates all degrees of freedom between two rigid bodies, maintaining a constant relative position and orientation. The constraint is defined by a
   relative origin offset and a relative axis that determine how the first body is positioned relative to the second body. The constraint supports dynamic updates to the attached bodies, translation,
   and rotation of the constraint offset, as well as serialization for save/load operations. The fixed constraint is evaluated during physics simulation to enforce the rigid relationship between
   bodies, with the ApplyFriction method being a placeholder that does not actually apply any friction. Debug drawing capabilities are provided to visualize the constraint in the simulation.

*/
class idAFConstraint_Fixed : public idAFConstraint
{
public:
	//! Constructs a fixed constraint between two rigid bodies.
	idAFConstraint_Fixed( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Sets the relative origin offset for the fixed constraint.
	void		 SetRelativeOrigin( const idVec3& origin ) { this->offset = origin; }

	//! Sets the relative axis for the fixed constraint.
	void		 SetRelativeAxis( const idMat3& axis ) { this->relAxis = axis; }

	//! Sets the first body for the fixed constraint and updates associated physics state.
	virtual void SetBody1( idAFBody* body );

	//! Sets the second body for the fixed constraint.
	virtual void SetBody2( idAFBody* body );

	//! Draws a debug line from the first body to the master body or origin
	virtual void DebugDraw();

	//! Moves the constraint offset by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the constraint offset and relative axis by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Retrieves the world origin of the first body associated with this fixed constraint and stores it in the provided center vector.
	virtual void GetCenter( idVec3& center );

	//! Saves the fixed affine constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the state of the fixed constraint from a save file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3 offset;	// offset of body1 relative to body2 in body2 space
	idMat3 relAxis; // rotation of body1 relative to body2

protected:
	//! Evaluates the fixed constraint for the physics simulation using the inverse time step.
	virtual void Evaluate( float invTimeStep );

	//! This function is a placeholder that does not apply any friction to the fixed constraint.
	virtual void ApplyFriction( float invTimeStep );

	//! Initializes the offset and relative axis for the fixed attachment constraint.
	void		 InitOffset();
};

/*!
	\class idAFConstraint_BallAndSocketJoint
	\brief A constraint that implements a ball and socket joint with optional limit constraints between two physical bodies.

	This class represents a spherical joint constraint that allows three degrees of freedom between two physical bodies. It supports setting anchor points, configuring cone or pyramid limits, and
   managing friction properties. The constraint can be evaluated during physics simulation and supports debug visualization. The joint can be configured with various limit types to restrict the range
   of motion, and it provides methods for saving and restoring constraint state. The class inherits from idAFConstraint and implements the core functionality for managing the joint's behavior in the
   physics simulation.

*/
class idAFConstraint_BallAndSocketJoint : public idAFConstraint
{
public:
	//! Constructs a ball and socket joint constraint between two physical bodies.
	idAFConstraint_BallAndSocketJoint( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Destructor for the idAFConstraint_BallAndSocketJoint class that cleans up allocated memory for cone and pyramid limits.
	~idAFConstraint_BallAndSocketJoint();

	//! Sets the anchor point for the ball and socket joint constraint in world coordinates.
	void		 SetAnchor( const idVec3& worldPosition );

	//! Returns the anchor point of the ball and socket joint constraint.
	idVec3		 GetAnchor() const;

	//! Removes any existing cone or pyramid limit constraints from the ball and socket joint.
	void		 SetNoLimit();

	//! Configures the cone limit for a ball and socket joint constraint.
	void		 SetConeLimit( const idVec3& coneAxis, const float coneAngle, const idVec3& body1Axis );

	/*!
		\brief Configures a pyramid limit constraint for a ball and socket joint using specified axes and angles.

		This function sets up a pyramid limit constraint for a ball and socket joint, defining the allowable range of motion based on the provided axes and angular limits. It handles the setup of the
	   constraint either with or without a second body, adjusting the axes according to the body's world coordinate system.

		\param pyramidAxis The axis defining the direction of the pyramid limit
		\param baseAxis The base axis used to establish the pyramid's orientation
		\param angle1 The first angular limit defining the pyramid's extent
		\param angle2 The second angular limit defining the pyramid's extent
		\param body1Axis The axis orientation relative to body1's world coordinate system
	*/
	void		 SetPyramidLimit( const idVec3& pyramidAxis, const idVec3& baseAxis, const float angle1, const float angle2, const idVec3& body1Axis );

	//! Sets the epsilon value for limit constraints on the ball and socket joint.
	void		 SetLimitEpsilon( const float e );

	//! Sets the friction value for the ball and socket joint constraint.
	void		 SetFriction( const float f ) { friction = f; }

	//! Returns the friction value for the ball and socket joint constraint.
	float		 GetFriction() const;

	//! Renders debug visualization for the ball and socket joint constraint.
	virtual void DebugDraw();

	//! Retrieves the force applied by the ball and socket joint constraint for the specified body.
	virtual void GetForce( idAFBody* body, idVec6& force );

	//! Moves the constraint anchor point by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the ball and socket joint constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Computes and returns the world-space center point of the ball-and-socket joint constraint.
	virtual void GetCenter( idVec3& center );

	//! Saves the ball and socket joint constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the state of the ball and socket joint constraint from a saved game file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3									   anchor1;		 // anchor in body1 space
	idVec3									   anchor2;		 // anchor in body2 space
	float									   friction;	 // joint friction
	idAFConstraint_ConeLimit*				   coneLimit;	 // cone shaped limit
	idAFConstraint_PyramidLimit*			   pyramidLimit; // pyramid shaped limit
	idAFConstraint_BallAndSocketJointFriction* fc;			 // friction constraint

protected:
	//! Evaluates the ball and socket joint constraint for the physics simulation.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the ball and socket joint constraint based on the inverse time step.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_BallAndSocketJointFriction
	\brief A constraint class for implementing ball and socket joint friction in physics simulations.

	This class represents a friction constraint for ball and socket joints within a physics simulation system. It inherits from idAFConstraint and is designed to work with ball and socket joint
   constraints. The class provides initialization, addition to physics simulations, and transformation methods for positioning the constraint in 3D space. The Evaluate and ApplyFriction methods
   currently perform no operations, suggesting they may be placeholders or intended for future implementation. The constraint is initialized using a ball and socket joint object and can be translated
   or rotated to adjust its position and orientation in the simulation environment.

*/
class idAFConstraint_BallAndSocketJointFriction : public idAFConstraint
{
public:
	//! Initializes a new instance of the ball and socket joint friction constraint.
	idAFConstraint_BallAndSocketJointFriction();

	//! Initializes the friction constraint using the provided ball and socket joint.
	void		 Setup( idAFConstraint_BallAndSocketJoint* cc );

	//! Adds a ball and socket joint friction constraint to the physics simulation.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Translates the ball-and-socket joint friction constraint by the specified vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the ball and socket joint friction constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
	idAFConstraint_BallAndSocketJoint* joint;

protected:
	//! This function performs no operations.
	virtual void Evaluate( float invTimeStep );

	//! This function does nothing and is a placeholder for applying friction to a ball and socket joint constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_UniversalJoint
	\brief A constraint that implements a universal joint with configurable limit and friction properties.

	This class implements a universal joint constraint that allows two degrees of freedom while constraining rotation about specified cardan shafts. It inherits from idAFConstraint and provides
   functionality for setting anchor points, shaft vectors, limit constraints, and friction properties. The constraint can be configured with cone or pyramid limits to restrict angular motion within
   specified angles. It supports physics simulation evaluation and friction application, and includes debugging visualization capabilities. The class handles the management of limit constraints
   through dedicated methods for cone and pyramid limits, and provides mechanisms for saving and restoring constraint state during game sessions. The constraint operates in the context of articulated
   figure physics simulation.

*/
class idAFConstraint_UniversalJoint : public idAFConstraint
{
public:
	//! Constructs an universal joint constraint between two articulated figures.
	idAFConstraint_UniversalJoint( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Destroys the universal joint constraint and cleans up its associated limit and force constraint objects.
	~idAFConstraint_UniversalJoint();

	//! Sets the anchor point for the universal joint constraint in world coordinates.
	void   SetAnchor( const idVec3& worldPosition );

	//! Returns the anchor point of the universal joint in world coordinates.
	idVec3 GetAnchor() const;

	//! Sets the shaft vectors for a universal joint constraint.
	void   SetShafts( const idVec3& cardanShaft1, const idVec3& cardanShaft2 );

	//! Returns the shaft vectors for the universal joint constraint.
	void   GetShafts( idVec3& cardanShaft1, idVec3& cardanShaft2 )
	{
		cardanShaft1 = shaft1;
		cardanShaft2 = shaft2;
	}

	//! Removes any existing limit constraints from the universal joint.
	void		 SetNoLimit();

	//! Configures the cone limit constraint for a universal joint using the specified cone axis and angle.
	void		 SetConeLimit( const idVec3& coneAxis, const float coneAngle );

	/*!
		\brief Configures the pyramid limit for a universal joint constraint using specified axes and angles.

		The function initializes or reinitializes the pyramid limit constraint for a universal joint. It first checks if a cone limit exists and deletes it if present. Then, it ensures the pyramid
	   limit is allocated and sets up the constraint with the provided pyramid and base axes, along with the specified angular limits. The function adjusts the axes based on the world axis of body2 if
	   it exists, otherwise it uses the axes directly. It also takes into account the shaft1 parameter for constraint setup.

		\param pyramidAxis Axis defining the pyramid direction for the limit constraint
		\param baseAxis Axis defining the base of the pyramid limit constraint
		\param angle1 First angular limit for the pyramid constraint
		\param angle2 Second angular limit for the pyramid constraint
	*/
	void		 SetPyramidLimit( const idVec3& pyramidAxis, const idVec3& baseAxis, const float angle1, const float angle2 );

	//! Sets the epsilon value for limit constraints on the universal joint.
	void		 SetLimitEpsilon( const float e );

	//! Sets the friction value for the universal joint constraint.
	void		 SetFriction( const float f ) { friction = f; }

	//! Returns the friction value for the universal joint, either from a force friction setting or calculated from the joint's friction and physics scale.
	float		 GetFriction() const;

	//! Draws debug visualization for the universal joint constraint.
	virtual void DebugDraw();

	//! Computes and returns the force applied by the universal joint constraint for the specified body
	virtual void GetForce( idAFBody* body, idVec6& force );

	//! Moves the universal joint constraint by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the universal joint constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Calculates and returns the center point of the universal joint constraint.
	virtual void GetCenter( idVec3& center );

	//! Saves the universal joint constraint data to a save file
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the universal joint constraint state from a saved game file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3								   anchor1;		 // anchor in body1 space
	idVec3								   anchor2;		 // anchor in body2 space
	idVec3								   shaft1;		 // body1 cardan shaft in body1 space
	idVec3								   shaft2;		 // body2 cardan shaft in body2 space
	idVec3								   axis1;		 // cardan axis in body1 space
	idVec3								   axis2;		 // cardan axis in body2 space
	float								   friction;	 // joint friction
	idAFConstraint_ConeLimit*			   coneLimit;	 // cone shaped limit
	idAFConstraint_PyramidLimit*		   pyramidLimit; // pyramid shaped limit
	idAFConstraint_UniversalJointFriction* fc;			 // friction constraint

protected:
	//! Evaluates the universal joint constraint for physics simulation using the inverse time step.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the universal joint constraint based on the inverse time step.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_UniversalJointFriction
	\brief Manages friction constraints for universal joint connections in physics simulations.

	This class implements a friction constraint specifically designed for universal joints within a physics simulation framework. It inherits from idAFConstraint and provides functionality to
   initialize the constraint setup, add the constraint to physics calculations, and handle spatial transformations. The class serves as a specialized constraint handler that maintains the physical
   behavior of universal joint friction. The Evaluate and ApplyFriction methods are currently implemented as no-ops, suggesting they may be reserved for future functionality or represent placeholder
   implementations.

*/
class idAFConstraint_UniversalJointFriction : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_UniversalJointFriction object with default values for a universal joint friction constraint.
	idAFConstraint_UniversalJointFriction();

	//! Initializes the friction constraint setup for a universal joint by linking it to the joint and its constituent bodies.
	void		 Setup( idAFConstraint_UniversalJoint* cc );

	//! Adds a universal joint friction constraint to the physics simulation.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Translates the universal joint friction constraint by the specified vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the universal joint friction constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
	idAFConstraint_UniversalJoint* joint; // universal joint

protected:
	//! Does nothing.
	virtual void Evaluate( float invTimeStep );

	//! This function does nothing and serves as a placeholder for applying friction to a universal joint constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_CylindricalJoint
	\brief A constraint that implements a cylindrical joint allowing translation along and rotation about a line between two bodies.
*/
class idAFConstraint_CylindricalJoint : public idAFConstraint
{
public:
	//! Constructor for cylindrical joint constraint that currently has a placeholder implementation.
	idAFConstraint_CylindricalJoint( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! This function is a placeholder that currently asserts and does not implement cylindrical joint debugging visualization.
	virtual void DebugDraw();

	//! This function is currently unimplemented and will assert.
	virtual void Translate( const idVec3& translation );

	//! This function rotates the cylindrical joint constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
protected:
	//! This function is a placeholder that currently asserts and must be implemented.
	virtual void Evaluate( float invTimeStep );

	//! This function is a placeholder that asserts and needs implementation for applying friction to a cylindrical joint constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Hinge
	\brief A constraint that implements a hinge joint allowing rotation around a single axis while constraining all other motion between two rigid bodies.

	This class represents a hinge constraint used in physics simulation to connect two rigid bodies. It allows rotation about a single axis while preventing all other relative motion between the
   bodies. The constraint supports setting anchor points, hinge axes, angular limits, friction, and steering parameters. It provides methods for evaluating the constraint during physics simulation,
   applying friction, and retrieving forces. The class also includes debugging visualization capabilities and supports serialization for saving and restoring constraint state. The hinge constraint is
   commonly used to model joints like doors, wheels, or rotating components in physical simulations.

*/
class idAFConstraint_Hinge : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_Hinge object with the specified name and two associated bodies.
	idAFConstraint_Hinge( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Destructor for the idAFConstraint_Hinge class that cleans up allocated resources.
	~idAFConstraint_Hinge();

	//! Sets the anchor point for the hinge constraint in world coordinates.
	void   SetAnchor( const idVec3& worldPosition );

	//! Returns the anchor point of the hinge constraint in world space.
	idVec3 GetAnchor() const;

	//! Sets the hinge axis for the constraint, normalized and transformed into the coordinate systems of the connected bodies.
	void   SetAxis( const idVec3& axis );

	//! Returns the two axes of the hinge constraint.
	void   GetAxis( idVec3& a1, idVec3& a2 ) const
	{
		a1 = axis1;
		a2 = axis2;
	}

	//! Returns the axis of the hinge constraint.
	idVec3		 GetAxis() const;

	//! Removes the cone limit constraint from the hinge.
	void		 SetNoLimit();

	//! Sets the hinge limit for the constraint using the provided axis, angle, and body1 axis.
	void		 SetLimit( const idVec3& axis, const float angle, const idVec3& body1Axis );

	//! Sets the epsilon value for the hinge constraint limit.
	void		 SetLimitEpsilon( const float e );

	//! Returns the angle of the hinge constraint in degrees
	float		 GetAngle() const;

	//! Sets the steering angle for the hinge constraint.
	void		 SetSteerAngle( const float degrees );

	//! Sets the steering speed for the hinge constraint.
	void		 SetSteerSpeed( const float speed );

	//! Sets the friction value for the hinge constraint.
	void		 SetFriction( const float f ) { friction = f; }

	//! Returns the friction value for the hinge constraint.
	float		 GetFriction() const;

	//! Draws a visual representation of the hinge constraint for debugging purposes.
	virtual void DebugDraw();

	//! Computes and retrieves the force applied by the hinge constraint for the specified body.
	virtual void GetForce( idAFBody* body, idVec6& force );

	//! Translates the hinge constraint by the specified vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the hinge constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Calculates and returns the world-space center point of the hinge constraint based on the first body's origin and anchor.
	virtual void GetCenter( idVec3& center );

	//! Saves the hinge constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the hinge constraint state from a save file
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3						  anchor1;	   // anchor in body1 space
	idVec3						  anchor2;	   // anchor in body2 space
	idVec3						  axis1;	   // axis in body1 space
	idVec3						  axis2;	   // axis in body2 space
	idMat3						  initialAxis; // initial axis of body1 relative to body2
	float						  friction;	   // hinge friction
	idAFConstraint_ConeLimit*	  coneLimit;   // cone limit
	idAFConstraint_HingeSteering* steering;	   // steering
	idAFConstraint_HingeFriction* fc;		   // friction constraint

protected:
	//! Evaluates the hinge constraint for the physics simulation using the inverse time step.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the hinge constraint using the inverse time step.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_HingeFriction
	\brief Represents a hinge friction constraint used in physics simulations.

	This class implements a constraint for managing friction in hinge joints within a physics simulation. It inherits from idAFConstraint and provides functionality to initialize the constraint with a
   hinge, add it to the physics system, and transform its position. The class appears to be designed as a placeholder or stub implementation, with several methods containing no functional code. The
   constraint is intended to work in conjunction with hinge constraints to simulate friction effects in articulated physics systems. The class supports basic spatial transformations through
   translation and rotation operations, allowing the constraint to be positioned correctly in the simulation space.

*/
class idAFConstraint_HingeFriction : public idAFConstraint
{
public:
	//! Initializes a new instance of the hinge friction constraint.
	idAFConstraint_HingeFriction();

	//! Initializes the hinge friction constraint with the specified hinge constraint.
	void		 Setup( idAFConstraint_Hinge* cc );

	//! Adds a hinge friction constraint to the physics simulation.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Translates the hinge friction constraint by the specified vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the hinge friction constraint using the provided rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
	idAFConstraint_Hinge* hinge; // hinge

protected:
	//! This function performs no operation and is likely a placeholder or stub implementation.
	virtual void Evaluate( float invTimeStep );

	//! This function does nothing and serves as a placeholder for the hinge friction constraint application.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_HingeSteering
	\brief Hinge steering constraint that controls the orientation and movement of articulated bodies.

	This class implements a constraint that maintains a specified relative orientation between two articulated bodies connected by a hinge. It provides functionality to set steering angles and speeds,
   control constraint evaluation precision, and integrate with physics simulation. The constraint supports translation and rotation transformations, and can be saved and restored during game state
   persistence. The steering behavior allows for controlled movement of the hinge attachment point. This constraint is designed to work with articulated physics systems and is intended to be used as
   part of a larger physics simulation framework.

*/
class idAFConstraint_HingeSteering : public idAFConstraint
{
public:
	//! Initializes a new instance of the hinge steering constraint.
	idAFConstraint_HingeSteering();

	//! Initializes the hinge steering constraint with the specified hinge.
	void		 Setup( idAFConstraint_Hinge* cc );

	//! Sets the steering angle for the hinge constraint.
	void		 SetSteerAngle( const float degrees ) { steerAngle = degrees; }

	//! Sets the steering speed for the hinge steering constraint.
	void		 SetSteerSpeed( const float speed ) { steerSpeed = speed; }

	//! Sets the epsilon value used for constraint calculations.
	void		 SetEpsilon( const float e ) { epsilon = e; }

	//! Adds a hinge steering constraint to the physics simulation.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Translates the hinge steering constraint by the specified vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the hinge steering constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Saves the hinge steering constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the hinge steering constraint state from a save file
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idAFConstraint_Hinge* hinge;	  // hinge
	float				  steerAngle; // desired steer angle in degrees
	float				  steerSpeed; // steer speed
	float				  epsilon;	  // lcp epsilon

protected:
	//! Evaluates the hinge steering constraint using the inverse time step.
	virtual void Evaluate( float invTimeStep );

	//! This function does nothing.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Slider
	\brief A constraint that enforces translational motion along a fixed axis between two articulated figures.

	This constraint enables sliding motion along a specified axis while maintaining a fixed orientation relative to one of the connected bodies. It is typically used to model joints that allow
   translation in a single direction while constraining rotation. The constraint can be configured with a specific axis and can be adjusted through translation and rotation operations. During physics
   simulation, it evaluates the constraint forces and applies friction to maintain the correct motion. The class supports serialization for saving and restoring constraint states. Debug visualization
   is available to display the constraint's line of motion.

*/
class idAFConstraint_Slider : public idAFConstraint
{
public:
	//! Initializes a slider constraint between two articulated figures.
	idAFConstraint_Slider( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Sets the axis for the slider constraint based on the provided axis vector.
	void		 SetAxis( const idVec3& ax );

	//! Draws a debug line for the slider constraint.
	virtual void DebugDraw();

	//! Translates the constraint offset by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the slider constraint offset by the specified rotation if body2 is not set.
	virtual void Rotate( const idRotation& rotation );

	//! Retrieves the center point of the slider constraint in world coordinates.
	virtual void GetCenter( idVec3& center );

	//! Saves the slider constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the slider constraint state from a save file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3 axis;	// axis along which body1 slides in body2 space
	idVec3 offset;	// offset of body1 relative to body2
	idMat3 relAxis; // rotation of body1 relative to body2

protected:
	//! Evaluates the slider constraint for the physics system using the inverse time step.
	virtual void Evaluate( float invTimeStep );

	//! This function applies friction to the slider constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Line
	\brief A constraint that restricts motion along a line between two bodies while allowing rotation around that line.
*/
class idAFConstraint_Line : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_Line object with the specified name and two associated AF bodies.
	idAFConstraint_Line( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! DebugDraw is not implemented yet and will assert.
	virtual void DebugDraw();

	//! Placeholder function that currently does nothing but assert.
	virtual void Translate( const idVec3& translation );

	//! This function is currently unimplemented and should rotate the constraint using the provided rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
protected:
	//! This function evaluates the line constraint and is currently unimplemented.
	virtual void Evaluate( float invTimeStep );

	//! Placeholder implementation for applying friction to a line constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Plane
	\brief A constraint that restricts the motion of two bodies to lie within a specified plane.

	This class implements a plane constraint that allows two bodies to move freely within a defined plane while constraining their relative positions. It provides functionality to set the constraint
   plane using a normal vector and anchor point, and supports translation and rotation operations to modify the constraint. The constraint permits five degrees of freedom, meaning bodies can slide
   along the plane surface but cannot move away from it. The class includes methods for debugging visualization, serialization, and physics evaluation. The ApplyFriction method is currently
   unimplemented and does nothing.

*/
class idAFConstraint_Plane : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_Plane object with the specified name and two bodies.
	idAFConstraint_Plane( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Sets the plane constraint using a normal vector and anchor point.
	void		 SetPlane( const idVec3& normal, const idVec3& anchor );

	//! Draws a visual representation of the plane constraint for debugging purposes.
	virtual void DebugDraw();

	//! Translates the anchor point of the constraint plane by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the plane constraint using the provided rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Serializes the plane constraint data into a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the plane constraint state from a save file
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3 anchor1;		// anchor in body1 space
	idVec3 anchor2;		// anchor in body2 space
	idVec3 planeNormal; // plane normal in body2 space

protected:
	//! Evaluates the plane constraint for the physics system using the inverse time step.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the plane constraint, though currently does nothing.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Spring
	\brief A physics constraint that connects two bodies with a spring allowing flexible motion while maintaining distance limits.

	This class implements a spring-based constraint that connects two physics bodies, enabling them to move relative to each other while maintaining a defined distance range. The constraint supports
   configurable spring properties including stretch stiffness, compression stiffness, damping, and rest length. It also provides minimum and maximum length limits to control the range of motion. The
   spring constraint operates in world coordinates and supports translation and rotation operations for anchor points. The class inherits from idAFConstraint and implements physics evaluation and
   friction handling. The constraint can be debug-drawn to visualize its behavior in the game world.

*/
class idAFConstraint_Spring : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_Spring with the specified name and two associated bodies.
	idAFConstraint_Spring( const idStr& name, idAFBody* body1, idAFBody* body2 );

	//! Sets the anchor points for the spring constraint in world coordinates.
	void		 SetAnchor( const idVec3& worldAnchor1, const idVec3& worldAnchor2 );

	/*!
		\brief Configures the spring properties for a physics constraint.

		Sets the spring parameters including stretch stiffness, compression stiffness, damping coefficient, and rest length. The function validates that stretch and compression values are
	   non-negative, and that the rest length is also non-negative. These parameters define how the spring constraint behaves during physics simulation.

		\param stretch Stretch stiffness coefficient for the spring constraint
		\param compress Compression stiffness coefficient for the spring constraint
		\param damping Damping coefficient to control spring oscillation
		\param restLength Rest length of the spring when no force is applied
		\throws assertion failure if stretch, compress, or restLength are negative
	*/
	void		 SetSpring( const float stretch, const float compress, const float damping, const float restLength );

	//! Sets the minimum and maximum length limits for the spring constraint.
	void		 SetLimit( const float minLength, const float maxLength );

	//! Draws a debug visualization of the spring constraint in the game world
	virtual void DebugDraw();

	//! Translates the anchor point of the spring constraint by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the anchor point of the spring constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Calculates and returns the center point between two anchor points in world space.
	virtual void GetCenter( idVec3& center );

	//! Saves the spring constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the spring constraint state from a save file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3 anchor1;	   // anchor in body1 space
	idVec3 anchor2;	   // anchor in body2 space
	float  kstretch;   // spring constant when stretched
	float  kcompress;  // spring constant when compressed
	float  damping;	   // spring damping
	float  restLength; // rest length of spring
	float  minLength;  // minimum spring length
	float  maxLength;  // maximum spring length

protected:
	//! Evaluates the spring constraint forces and updates the constraint Jacobians based on the current positions and velocities of the connected bodies.
	virtual void Evaluate( float invTimeStep );

	//! This function applies friction to the spring constraint, but currently does nothing.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Contact
	\brief Represents a contact constraint between two physics bodies in a physics simulation.

	This class implements a constraint that maintains contact between two physics bodies, ensuring they remain in contact or move away from each other. The constraint is initialized with contact
   information between the bodies and provides methods for debugging visualization and physics evaluation. The class inherits from idAFConstraint and overrides specific methods to handle
   contact-specific behavior. The contact point is stored and can be retrieved for debugging purposes. The constraint is designed to be evaluated during physics simulation steps and can apply friction
   forces. The Translate and Rotate methods are overridden to prevent invalid operations on contact constraints, as these constraints should remain fixed in space relative to the bodies they connect.

*/
class idAFConstraint_Contact : public idAFConstraint
{
public:
	//! Initializes a new instance of the idAFConstraint_Contact class.
	idAFConstraint_Contact();

	//! Destroys the idAFConstraint_Contact object and cleans up its associated contact constraint data.
	~idAFConstraint_Contact();

	//! Initializes the contact constraint between two physics bodies using the provided contact information
	void				 Setup( idAFBody* b1, idAFBody* b2, contactInfo_t& c );

	//! Returns a constant reference to the contact information stored in this constraint.
	const contactInfo_t& GetContact() const { return contact; }

	//! Draws a visual representation of the contact constraint for debugging purposes.
	virtual void		 DebugDraw();

	//! This function should never be called as contact constraints should never be translated.
	virtual void		 Translate( const idVec3& translation );

	//! This function should never be called as contact constraints should never be rotated.
	virtual void		 Rotate( const idRotation& rotation );

	//! Sets the provided center vector to the contact point of this constraint.
	virtual void		 GetCenter( idVec3& center );

protected:
	contactInfo_t					contact; // contact information
	idAFConstraint_ContactFriction* fc;		 // contact friction

protected:
	//! This function performs no operation and is a placeholder for the contact constraint evaluation.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the contact constraint based on the inverse time step.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_ContactFriction
	\brief Handles contact friction constraints for articulated figure physics.

	This class implements contact friction constraints for articulated figure physics systems. It manages the interaction between rigid bodies when they come into contact, applying friction forces to
   prevent滑动. The constraint is initialized with a contact object and can be added to physics simulations. It provides methods for debugging visualization, transformation operations, and evaluation
   of friction forces. The class inherits from idAFConstraint, indicating it's part of a constraint-based physics system for articulated figures.

*/
class idAFConstraint_ContactFriction : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_ContactFriction object with default settings.
	idAFConstraint_ContactFriction();

	//! Initializes the contact friction constraint with the specified contact.
	void		 Setup( idAFConstraint_Contact* cc );

	//! Adds a contact friction constraint to the physics system for the given articulated figure.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Draws debug information for the contact friction constraint.
	virtual void DebugDraw();

	//! Translates the contact friction constraint by the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the contact friction constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
	idAFConstraint_Contact* cc; // contact constraint

protected:
	//! Evaluates the contact friction constraint.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the contact constraint
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_ConeLimit
	\brief Manages cone limit constraints between articulated frame bodies in physics simulations.

	This class implements a constraint that limits the orientation of one articulated frame body relative to another within a specified cone shape. The constraint is defined by an anchor point, cone
   axis, and angle in body2 space, with a preferred direction in body1 space. The class supports initialization with specific parameters, updating during physics simulation, and provides debugging
   visualization capabilities. It can be configured with custom anchor points, body axes, and friction settings, and includes persistence methods for saving and restoring constraint states.

*/
class idAFConstraint_ConeLimit : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_ConeLimit object
	idAFConstraint_ConeLimit();

	/*!
		\brief Initializes the cone limit constraint between two physics bodies using specified anchor, axis, and angle parameters.

		Sets up the constraint between two articulated frame bodies to limit the orientation of body1 relative to body2 within a defined cone. The cone is defined by its anchor point and axis in body2
	   space, and the angle determines the size of the cone. The body1Axis specifies the preferred direction in body1 space that should remain within the cone. The constraint uses trigonometric
	   calculations to determine the cosine and sine values needed for constraint enforcement.

		\param b1 Pointer to the first articulated frame body involved in the constraint
		\param b2 Pointer to the second articulated frame body involved in the constraint
		\param coneAnchor The top of the cone in body2 space
		\param coneAxis The axis of the cone in body2 space
		\param coneAngle The angle the cone hull makes at the top, defines the size of the cone
		\param body1Axis The axis in body1 space that should stay within the cone
	*/
	void		 Setup( idAFBody* b1, idAFBody* b2, const idVec3& coneAnchor, const idVec3& coneAxis, const float coneAngle, const idVec3& body1Axis );

	//! Sets the anchor point for the cone limit constraint.
	void		 SetAnchor( const idVec3& coneAnchor );

	//! Sets the axis for body 1 of the cone limit constraint.
	void		 SetBody1Axis( const idVec3& body1Axis );

	//! Sets the epsilon value for the cone limit constraint.
	void		 SetEpsilon( const float e ) { epsilon = e; }

	//! Adds a cone limit constraint to the physics simulation for a given frame.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Draws a visual representation of the cone limit constraint for debugging purposes.
	virtual void DebugDraw();

	//! Translates the cone anchor by the specified translation vector if body2 is not set.
	virtual void Translate( const idVec3& translation );

	//! Rotates the cone limit constraint using the provided rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Saves the cone limit constraint data to a save file.
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the cone limit constraint state from a saved game file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3 coneAnchor;	 // top of the cone in body2 space
	idVec3 coneAxis;	 // cone axis in body2 space
	idVec3 body1Axis;	 // axis in body1 space that should stay within the cone
	float  cosAngle;	 // cos( coneAngle / 2 )
	float  sinHalfAngle; // sin( coneAngle / 4 )
	float  cosHalfAngle; // cos( coneAngle / 4 )
	float  epsilon;		 // lcp epsilon

protected:
	//! This function does nothing and serves as a placeholder for the cone limit constraint evaluation.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the cone limit constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_PyramidLimit
	\brief A constraint that limits the orientation of two articulated bodies within a pyramidal space.

	This class implements a pyramid limit constraint that restricts the relative orientation of two articulated figure bodies within a defined pyramidal space. The constraint is initialized with
   geometric parameters including pyramid anchor point, axis directions, and angle limits that define the pyramidal constraint region. The constraint ensures that an axis attached to the first body
   remains within the pyramid defined relative to the second body. The class provides methods to configure the constraint parameters, add it to physics simulations, and handle debugging visualization.
   The constraint operates in a physics context where it influences the movement and interaction of articulated bodies while maintaining the specified geometric limitations.

*/
class idAFConstraint_PyramidLimit : public idAFConstraint
{
public:
	//! Constructs an idAFConstraint_PyramidLimit object with default settings.
	idAFConstraint_PyramidLimit();

	/*!
		\brief Initializes the pyramid limit constraint between two articulated figures using specified geometric parameters and angles.

		Sets up the constraint by defining the pyramid basis from the provided axis vectors, ensuring orthonormality, and calculating trigonometric values for angle-based computations. The constraint
	   limits the relative orientation of two bodies within a defined pyramidal space.

		\param b1 First articulated figure body involved in the constraint
		\param b2 Second articulated figure body involved in the constraint
		\param pyramidAnchor Point defining the apex of the pyramid constraint
		\param pyramidAxis Axis along which the pyramid extends
		\param baseAxis Axis defining the base orientation of the pyramid
		\param pyramidAngle1 First pyramid angle defining the constraint extent
		\param pyramidAngle2 Second pyramid angle defining the constraint extent
		\param body1Axis Axis on the first body used for constraint calculations
	*/
	void Setup(
		idAFBody* b1, idAFBody* b2, const idVec3& pyramidAnchor, const idVec3& pyramidAxis, const idVec3& baseAxis, const float pyramidAngle1, const float pyramidAngle2, const idVec3& body1Axis );

	//! Sets the pyramid axis anchor for the constraint.
	void		 SetAnchor( const idVec3& pyramidAxis );

	//! Sets the axis for body 1 of the pyramid limit constraint.
	void		 SetBody1Axis( const idVec3& body1Axis );

	//! Sets the epsilon value for the pyramid limit constraint.
	void		 SetEpsilon( const float e ) { epsilon = e; }

	//! Adds a pyramid limit constraint to the physics simulation for the given physics object.
	bool		 Add( idPhysics_AF* phys, float invTimeStep );

	//! Draws a visual representation of the pyramid limit constraint for debugging purposes.
	virtual void DebugDraw();

	//! Updates the pyramid anchor position by adding the specified translation vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the pyramid limit constraint components by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

	//! Saves the pyramid limit constraint data to a save file
	virtual void Save( idSaveGame* saveFile ) const;

	//! Restores the pyramid limit constraint state from a saved game file.
	virtual void Restore( idRestoreGame* saveFile );

protected:
	idVec3 pyramidAnchor;	// top of the pyramid in body2 space
	idMat3 pyramidBasis;	// pyramid basis in body2 space with base[2] being the pyramid axis
	idVec3 body1Axis;		// axis in body1 space that should stay within the cone
	float  cosAngle[2];		// cos( pyramidAngle / 2 )
	float  sinHalfAngle[2]; // sin( pyramidAngle / 4 )
	float  cosHalfAngle[2]; // cos( pyramidAngle / 4 )
	float  epsilon;			// lcp epsilon

protected:
	//! Does nothing.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the pyramid limit constraint.
	virtual void ApplyFriction( float invTimeStep );
};

/*!
	\class idAFConstraint_Suspension
	\brief Provides vehicle suspension constraint functionality for physics simulations.

	This class implements a suspension constraint for vehicle simulations, extending basic constraint functionality. It manages the physical properties and behaviors of suspension systems including
   spring compression, damping, friction, and motor control. The constraint is configured with a body it's attached to, wheel collision model, and suspension parameters such as limits, spring
   constant, damping, and friction. It supports steering angle control, motor enable/disable, and motor force/velocity settings. The class handles evaluation and friction application during physics
   simulation steps, while providing debug visualization capabilities. The suspension constraint operates in world space coordinates, with local transformations computed from world space inputs during
   setup. It includes methods for translating and rotating the constraint's position and orientation, and provides access to the wheel's world origin for external queries.

*/
class idAFConstraint_Suspension : public idAFConstraint
{
public:
	//! Initializes a new instance of the idAFConstraint_Suspension class.
	idAFConstraint_Suspension();

	/*!
		\brief Initializes the suspension constraint with the specified parameters

		Sets up the suspension constraint by storing the name, associated body, local origin and axis transformations, and the wheel collision model. The local origin is computed relative to the
	   body's world position and orientation, and the local axis is transformed to match the body's coordinate system

		\param name Unique identifier for the suspension constraint
		\param body The body this constraint is attached to
		\param origin World space position of the suspension point
		\param axis World space orientation of the suspension axis
		\param clipModel Collision model representing the wheel
	*/
	void		 Setup( const char* name, idAFBody* body, const idVec3& origin, const idMat3& axis, idClipModel* clipModel );

	/*!
		\brief Configures the suspension parameters for a suspension constraint.

		Sets the upward and downward suspension limits, the compression spring constant, the damping coefficient, and the friction value for the suspension constraint.

		\param up The upward suspension limit
		\param down The downward suspension limit
		\param k The compression spring constant
		\param d The damping coefficient
		\param f The friction value
	*/
	void		 SetSuspension( const float up, const float down, const float k, const float d, const float f );

	//! Sets the steer angle for the suspension constraint.
	void		 SetSteerAngle( const float degrees ) { steerAngle = degrees; }

	//! Enables or disables the motor state.
	void		 EnableMotor( const bool enable ) { motorEnabled = enable; }

	//! Sets the motor force for the suspension constraint.
	void		 SetMotorForce( const float force ) { motorForce = force; }

	//! Sets the motor velocity for the suspension constraint.
	void		 SetMotorVelocity( const float vel ) { motorVelocity = vel; }

	//! Sets the epsilon value used for suspension constraint calculations.
	void		 SetEpsilon( const float e ) { epsilon = e; }

	//! Returns the world origin of the wheel for this suspension constraint.
	const idVec3 GetWheelOrigin() const;

	//! Draws debug visualization for the suspension constraint.
	virtual void DebugDraw();

	//! Translates the suspension constraint by the specified vector.
	virtual void Translate( const idVec3& translation );

	//! Rotates the suspension constraint by the specified rotation.
	virtual void Rotate( const idRotation& rotation );

protected:
	idVec3		 localOrigin;		  // position of suspension relative to body1
	idMat3		 localAxis;			  // orientation of suspension relative to body1
	float		 suspensionUp;		  // suspension up movement
	float		 suspensionDown;	  // suspension down movement
	float		 suspensionKCompress; // spring compress constant
	float		 suspensionDamping;	  // spring damping
	float		 steerAngle;		  // desired steer angle in degrees
	float		 friction;			  // friction
	bool		 motorEnabled;		  // whether the motor is enabled or not
	float		 motorForce;		  // motor force
	float		 motorVelocity;		  // desired velocity
	idClipModel* wheelModel;		  // wheel model
	idVec3		 wheelOffset;		  // wheel position relative to body1
	trace_t		 trace;				  // contact point with the ground
	float		 epsilon;			  // lcp epsilon

protected:
	//! Evaluates the suspension constraint for a vehicle wheel based on contact information and physical properties.
	virtual void Evaluate( float invTimeStep );

	//! Applies friction to the suspension constraint.
	virtual void ApplyFriction( float invTimeStep );
};

//===============================================================
//
//	idAFBody
//
//===============================================================

typedef struct AFBodyPState_s {
	idVec3 worldOrigin;		// position in world space
	idMat3 worldAxis;		// axis at worldOrigin
	idVec6 spatialVelocity; // linear and rotational velocity of body
	idVec6 externalForce;	// external force and torque applied to body
} AFBodyPState_t;

/*!
	\class idAFBody
	\brief Represents a physics body with collision and dynamics properties for articulated figure simulation.

	The idAFBody class encapsulates the physical properties and behavior of a body within an articulated figure system. It manages collision models, physical state such as position, velocity, and
   inertia, as well as friction, bouncyness, and motor properties. The class supports initialization with specific parameters, dynamic modification of physical properties, and serialization for
   save/load operations. It provides methods to query the body's state, apply forces, and manage collision detection through its associated clip model.

*/
class idAFBody
{
	friend class idPhysics_AF;
	friend class idAFTree;

public:
	//! Initializes a new instance of the idAFBody class.
	idAFBody();

	//! Initializes a new AF body with the specified name, clip model, and density.
	idAFBody( const idStr& name, idClipModel* clipModel, float density );

	//! Destroys the idAFBody object and cleans up its associated clip model.
	~idAFBody();

	//! Initializes the body properties to their default values.
	void		  Init();

	//! Returns the name of the collision model body.
	const idStr&  GetName() const { return name; }

	//! Returns the world origin of the body.
	const idVec3& GetWorldOrigin() const { return current->worldOrigin; }

	//! Returns the world axis of the body.
	const idMat3& GetWorldAxis() const { return current->worldAxis; }

	//! Returns the linear velocity of the body as a constant reference to a 3D vector.
	const idVec3& GetLinearVelocity() const { return current->spatialVelocity.SubVec3( 0 ); }

	//! Returns the angular velocity of the body.
	const idVec3& GetAngularVelocity() const { return current->spatialVelocity.SubVec3( 1 ); }

	//! Returns the velocity of a point on the body expressed in the world coordinate frame.
	idVec3		  GetPointVelocity( const idVec3& point ) const;

	//! Returns a constant reference to the center of mass of the body.
	const idVec3& GetCenterOfMass() const { return centerOfMass; }

	//! Sets the clip model for this physics body, replacing any existing clip model.
	void		  SetClipModel( idClipModel* clipModel );

	//! Returns the clip model associated with this physics body.
	idClipModel*  GetClipModel() const { return clipModel; }

	//! Sets the clip mask for the body and marks it as set.
	void		  SetClipMask( const int mask )
	{
		clipMask	   = mask;
		fl.clipMaskSet = true;
	}

	//! Returns the clip mask of the body.
	int		GetClipMask() const { return clipMask; }

	//! Sets whether self-collision is enabled for the physics body.
	void	SetSelfCollision( const bool enable ) { fl.selfCollision = enable; }

	//! Sets the world origin of the physics body to the specified position.
	void	SetWorldOrigin( const idVec3& origin ) { current->worldOrigin = origin; }

	//! Sets the world axis of the physics body to the provided axis.
	void	SetWorldAxis( const idMat3& axis ) { current->worldAxis = axis; }

	//! Sets the linear velocity of the body.
	void	SetLinearVelocity( const idVec3& linear ) const { current->spatialVelocity.SubVec3( 0 ) = linear; }

	//! Sets the angular velocity of the body.
	void	SetAngularVelocity( const idVec3& angular ) const { current->spatialVelocity.SubVec3( 1 ) = angular; }

	//! Sets the linear, angular, and contact friction values for the physics body.
	void	SetFriction( float linear, float angular, float contact );

	//! Returns the contact friction value for this body.
	float	GetContactFriction() const { return contactFriction; }

	//! Sets the bouncyness value for the physics body with range validation.
	void	SetBouncyness( float bounce );

	//! Returns the bouncyness value of the body.
	float	GetBouncyness() const { return bouncyness; }

	//! Sets the density of the body and calculates mass properties.
	void	SetDensity( float density, const idMat3& inertiaScale = mat3_identity );

	//! Returns the inverse mass of the body.
	float	GetInverseMass() const { return invMass; }

	//! Returns the inverse world inertia tensor for the body.
	idMat3	GetInverseWorldInertia() const { return current->worldAxis.Transpose() * inverseInertiaTensor * current->worldAxis; }

	//! Sets the friction direction for the physics body.
	void	SetFrictionDirection( const idVec3& dir );

	//! Returns the friction direction of the body if friction is being used, otherwise returns false.
	bool	GetFrictionDirection( idVec3& dir ) const;

	//! Sets the contact motor direction for the physics body.
	void	SetContactMotorDirection( const idVec3& dir );

	//! Returns the contact motor direction if contact motor direction is being used.
	bool	GetContactMotorDirection( idVec3& dir ) const;

	//! Sets the contact motor velocity for the physics body.
	void	SetContactMotorVelocity( float vel ) { contactMotorVelocity = vel; }

	//! Returns the contact motor velocity of the physics body.
	float	GetContactMotorVelocity() const { return contactMotorVelocity; }

	//! Sets the contact motor force for the physics body.
	void	SetContactMotorForce( float force ) { contactMotorForce = force; }

	//! Returns the contact motor force value.
	float	GetContactMotorForce() const { return contactMotorForce; }

	//! Adds a force and torque to the body at the specified point.
	void	AddForce( const idVec3& point, const idVec3& force );

	//! Multiplies the inverse world spatial inertia matrix by the input vector v and stores the result in dst
	void	InverseWorldSpatialInertiaMultiply( idVecX& dst, const float* v ) const;

	//! Returns a reference to the response force vector at the specified index.
	idVec6& GetResponseForce( int index ) { return reinterpret_cast<idVec6&>( response[index * 8] ); }

	//! Saves the physical properties and state of the articulated figure body to a save file
	void	Save( idSaveGame* saveFile );

	//! Restores the state of the physics body from a save file
	void	Restore( idRestoreGame* saveFile );

private:
	// properties
	idStr											name;				  // name of body
	idAFBody*										parent;				  // parent of this body
	idList<idAFBody*, TAG_IDLIB_LIST_PHYSICS>		children;			  // children of this body
	idClipModel*									clipModel;			  // model used for collision detection
	idAFConstraint*									primaryConstraint;	  // primary constraint (this->constraint->body1 = this)
	idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS> constraints;		  // all constraints attached to this body
	idAFTree*										tree;				  // tree structure this body is part of
	float											linearFriction;		  // translational friction
	float											angularFriction;	  // rotational friction
	float											contactFriction;	  // friction with contact surfaces
	float											bouncyness;			  // bounce
	int												clipMask;			  // contents this body collides with
	idVec3											frictionDir;		  // specifies a single direction of friction in body space
	idVec3											contactMotorDir;	  // contact motor direction
	float											contactMotorVelocity; // contact motor velocity
	float											contactMotorForce;	  // maximum force applied to reach the motor velocity

	// derived properties
	float											mass;				  // mass of body
	float											invMass;			  // inverse mass
	idVec3											centerOfMass;		  // center of mass of body
	idMat3											inertiaTensor;		  // inertia tensor
	idMat3											inverseInertiaTensor; // inverse inertia tensor

	// physics state
	AFBodyPState_t									state[2];
	AFBodyPState_t*									current;	  // current physics state
	AFBodyPState_t*									next;		  // next physics state
	AFBodyPState_t									saved;		  // saved physics state
	idVec3											atRestOrigin; // origin at rest
	idMat3											atRestAxis;	  // axis at rest

	// simulation variables used during calculations
	idMatX											inverseWorldSpatialInertia; // inverse spatial inertia in world space
	idMatX											I, invI;					// transformed inertia
	idMatX											J;							// transformed constraint matrix
	idVecX											s;							// temp solution
	idVecX											totalForce;					// total force acting on body
	idVecX											auxForce;					// force from auxiliary constraints
	idVecX											acceleration;				// acceleration
	float*											response;					// forces on body in response to auxiliary constraint forces
	int*											responseIndex;				// index to response forces
	int												numResponses;				// number of response forces
	int												maxAuxiliaryIndex;			// largest index of an auxiliary constraint constraining this body
	int												maxSubTreeAuxiliaryIndex;	// largest index of an auxiliary constraint constraining this body or one of it's children

	struct bodyFlags_s {
		bool clipMaskSet		  : 1; // true if this body has a clip mask set
		bool selfCollision		  : 1; // true if this body can collide with other bodies of this AF
		bool spatialInertiaSparse : 1; // true if the spatial inertia matrix is sparse
		bool useFrictionDir		  : 1; // true if a single friction direction should be used
		bool useContactMotorDir	  : 1; // true if a contact motor should be used
		bool isZero				  : 1; // true if 's' is zero during calculations
	} fl;
};

/*!
	\class idAFTree
	\brief idAFTree manages the articulated figure tree structure for physics simulation.

	The idAFTree class represents the hierarchical structure of an articulated figure used in physics simulations. It organizes bodies in a tree format where each body can have children, and parent
   bodies must be processed before their children. The class provides methods for factorizing the system, solving constraints, calculating forces, and sorting bodies in the correct order for
   processing. It also supports debugging visualization of the body hierarchy and handles auxiliary indices for constraint solving. The tree structure allows for efficient computation of physics
   interactions within the articulated figure system.

*/
class idAFTree
{
	friend class idPhysics_AF;

public:
	//! Factors the matrix for the primary constraints in the tree
	void Factor() const;

	//! Solves primary constraints for the articulated figure tree structure
	void Solve( int auxiliaryIndex = 0 ) const;

	//! Calculates body forces in the tree in response to a constraint force
	void Response( const idAFConstraint* constraint, int row, int auxiliaryIndex ) const;

	//! Calculates forces on bodies within the AF tree using the provided time step.
	void CalculateForces( float timeStep ) const;

	//! Sets the maximum auxiliary index for each body in the tree structure based on its children.
	void SetMaxSubTreeAuxiliaryIndex();

	//! Sorts the bodies in the articulated figure tree to ensure parents come before their children.
	void SortBodies();

	//! Recursively appends child bodies to a sorted list and then recursively processes each child.
	void SortBodies_r( idList<idAFBody*>& sortedList, idAFBody* body );

	//! Draws debug arrows representing the body hierarchy of the AF tree.
	void DebugDraw( const idVec4& color ) const;

private:
	idList<idAFBody*, TAG_IDLIB_LIST_PHYSICS> sortedBodies;
};

//===============================================================
//
//	idPhysics_AF
//
//===============================================================

typedef struct AFPState_s {
	int	   atRest;		 // >= 0 if articulated figure is at rest
	float  noMoveTime;	 // time the articulated figure is hardly moving
	float  activateTime; // time since last activation
	float  lastTimeStep; // last time step
	idVec6 pushVelocity; // velocity with which the af is pushed
} AFPState_t;

typedef struct AFCollision_s {
	trace_t	  trace;
	idAFBody* body;
} AFCollision_t;

/*!
	\class idPhysics_AF
	\brief Manages articulated figure physics simulation with bodies, constraints, and collision detection.

	This class implements a physics simulation system for articulated figures, supporting complex rigid body dynamics with constraints, collision detection, and advanced physical behaviors. It
   provides methods for managing bodies and constraints, configuring collision properties, handling physics state, and integrating with the game engine's time and rendering systems. The class supports
   both simulation evaluation and state restoration, enabling complex physics interactions including gravity, friction, external forces, and collision responses. It is designed for use with entities
   requiring realistic physical simulation and can be integrated with network synchronization for multiplayer environments. The system handles various physics states, including active, resting, and
   pushed states, and provides mechanisms for debugging and visualizing the physics simulation.

*/
class idPhysics_AF : public idPhysics_Base
{
public:
	CLASS_PROTOTYPE( idPhysics_AF );

	//! Initializes an idPhysics_AF object with default values for all physics properties and data structures.
	idPhysics_AF();

	//! Destroys the idPhysics_AF object and cleans up all associated resources.
	~idPhysics_AF();

	//! Saves the articulated figure physics state to a save file.
	void			Save( idSaveGame* savefile ) const;

	//! Restores the articulated figure physics state from a save file
	void			Restore( idRestoreGame* savefile );

	//! Adds a body to the physics simulation and returns its assigned ID.
	int				AddBody( idAFBody* body );

	//! Adds a constraint to the articulated figure physics system.
	void			AddConstraint( idAFConstraint* constraint );

	//! Adds a frame constraint to the physics simulation.
	void			AddFrameConstraint( idAFConstraint* constraint );

	//! Forces a body to have a specified ID within the articulated figure.
	void			ForceBodyId( idAFBody* body, int newId );

	//! Returns the ID of the specified body within the articulated figure.
	int				GetBodyId( idAFBody* body ) const;

	//! Returns the index of the body with the specified name in the articulated figure.
	int				GetBodyId( const char* bodyName ) const;

	//! Returns the ID of the specified constraint within the articulated figure physics system
	int				GetConstraintId( idAFConstraint* constraint ) const;

	//! Returns the index of a constraint with the specified name in the articulated figure physics system.
	int				GetConstraintId( const char* constraintName ) const;

	//! Returns the number of bodies in the physics articulated figure.
	int				GetNumBodies() const;

	//! Returns the number of constraints in the physics articulated figure.
	int				GetNumConstraints() const;

	//! Retrieves a body from the physics system by its name
	idAFBody*		GetBody( const char* bodyName ) const;

	//! Returns a pointer to the body with the specified ID from the physics simulation.
	idAFBody*		GetBody( const int id ) const;

	//! Returns a pointer to the master body associated with this physics simulation.
	idAFBody*		GetMasterBody() const { return masterBody; }

	//! Returns the constraint with the specified name from the physics simulation.
	idAFConstraint* GetConstraint( const char* constraintName ) const;

	//! Returns the constraint with the specified id from the physics simulation
	idAFConstraint* GetConstraint( const int id ) const;

	//! Deletes a body from the articulated figure by its name.
	void			DeleteBody( const char* bodyName );

	//! Deletes a body from the articulated figure physics system by its ID.
	void			DeleteBody( const int id );

	//! Deletes a constraint with the specified name from the articulated figure physics.
	void			DeleteConstraint( const char* constraintName );

	//! Removes a constraint with the specified ID from the physics simulation.
	void			DeleteConstraint( const int id );

	//! Returns the number of contact constraints acting on a specified body
	int				GetBodyContactConstraints( const int id, idAFConstraint_Contact* contacts[], int maxContacts ) const;

	//! Sets the default friction values for linear, angular, and contact motion.
	void			SetDefaultFriction( float linear, float angular, float contact );

	//! Sets the suspend velocity and acceleration parameters for the physics affine fetch.
	void			SetSuspendSpeed( const idVec2& velocity, const idVec2& acceleration );

	//! Sets the time and tolerances used to determine if the simulation can be suspended when the figure hardly moves for a while
	void			SetSuspendTolerance( const float noMoveTime, const float translationTolerance, const float rotationTolerance );

	//! Sets the minimum and maximum simulation time for the physics af
	void			SetSuspendTime( const float minTime, const float maxTime );

	//! Sets the time scale value for the physics simulation.
	void			SetTimeScale( const float ts ) { timeScale = ts; }

	//! Sets the time scale ramp values for the physics simulation.
	void			SetTimeScaleRamp( const float start, const float end );

	//! Sets the joint friction scale for the physics simulation.
	void			SetJointFrictionScale( const float scale ) { jointFrictionScale = scale; }

	//! Sets the joint friction dent parameters for the physics articulated figure.
	void			SetJointFrictionDent( const float dent, const float start, const float end );

	//! Returns the current joint friction scale value.
	float			GetJointFrictionScale() const;

	//! Sets the contact friction scale for the physics simulation.
	void			SetContactFrictionScale( const float scale ) { contactFrictionScale = scale; }

	//! Sets the contact friction dent parameters for the physics simulation.
	void			SetContactFrictionDent( const float dent, const float start, const float end );

	//! Returns the current contact friction scale used by the physics system.
	float			GetContactFrictionScale() const;

	//! Sets whether collision detection is enabled.
	void			SetCollision( const bool enable ) { enableCollision = enable; }

	//! Sets whether self collision is enabled or disabled.
	void			SetSelfCollision( const bool enable ) { selfCollision = enable; }

	//! Enables or disables the physics simulation's tendency to come to a complete stop.
	void			SetComeToRest( bool enable ) { comeToRest = enable; }

	//! Marks the articulated figure as changed.
	void			SetChanged() { changedAF = true; }

	//! Enables activation by impact for the physics articulated figure.
	void			EnableImpact();

	//! Disables the impact functionality of the physics articulated figure.
	void			DisableImpact();

	//! Locks or unlocks the world constraints based on the provided boolean value.
	void			LockWorldConstraints( const bool lock ) { worldConstraintsLocked = lock; }

	//! Sets whether the physics object is force pushable.
	void			SetForcePushable( const bool enable ) { forcePushable = enable; }

	//! Updates the positions of all clip models for the physics articulated figure.
	void			UpdateClipModels();

public:
	// common physics interface ----------------------

	/*!
		\brief Sets the collision model for the articulated figure physics object with specified density and identifier.

		This function configures the collision model used by the articulated figure physics system. It allows specifying a custom clip model, its density for physics calculations, an identifier for
	   the model, and whether to free the previous model. The density parameter influences how the physics engine treats the mass and interaction of the collision model.

		\param model Pointer to the clip model to be used for collision detection
		\param density Density value used for physics calculations, affects mass and interaction
		\param id Identifier for the clip model, default is 0
		\param freeOld Flag indicating whether to free the previous clip model, default is true
	*/
	void			SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );

	//! Returns the clip model for the specified body index.
	idClipModel*	GetClipModel( int id = 0 ) const;

	//! Returns the number of clip models in the physics AF.
	int				GetNumClipModels() const;

	//! Sets the mass of a specific body or the total mass of the physics object.
	void			SetMass( float mass, int id = -1 );

	//! Returns the mass of a specific body or the total mass if the index is invalid.
	float			GetMass( int id = -1 ) const;

	//! Sets the contents flag for the clip model of a specific body or all bodies in the physics AF.
	void			SetContents( int contents, int id = -1 );

	//! Returns the contents of a specific body or the combined contents of all bodies in the physics AF system.
	int				GetContents( int id = -1 ) const;

	//! Returns the bounds of a specific body or the combined bounds of all bodies in the physics simulation.
	const idBounds& GetBounds( int id = -1 ) const;

	//! Returns the absolute bounds of the physics simulation, optionally filtered by body ID.
	const idBounds& GetAbsBounds( int id = -1 ) const;

	//! Evaluates the articulated figure physics simulation for a given time step and end time.
	bool			Evaluate( int timeStepMSec, int endTimeMSec );

	//! Updates the physics simulation time for the articulated figure.
	void			UpdateTime( int endTimeMSec );

	//! Returns the current game time value.
	int				GetTime() const;

	//! Retrieves impact information for a specified body at a given point.
	void			GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;

	//! Applies an impulse to a specific body in the physics simulation.
	void			ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );

	//! Adds a force to a specific body in the physics system
	void			AddForce( const int id, const idVec3& point, const idVec3& force );

	//! Returns true if the physics object is at rest.
	bool			IsAtRest() const;

	//! Returns the time when the physics object started resting.
	int				GetRestStartTime() const;

	//! Activates the articulated figure physics, applying gravity if it was at rest and marking the entity as active for physics simulation.
	void			Activate();

	//! Puts the physics object to rest until a collision occurs.
	void			PutToRest();

	//! Determines whether the physics object can be pushed by external forces.
	bool			IsPushable() const;

	//! Saves the current state of the physics simulation and all its bodies.
	void			SaveState();

	//! Restores the physics state from a previously saved state.
	void			RestoreState();

	//! Sets the origin of the physics object relative to the master body or the first body.
	void			SetOrigin( const idVec3& newOrigin, int id = -1 );

	//! Sets the axis of the physics object, optionally for a specific body ID.
	void			SetAxis( const idMat3& newAxis, int id = -1 );

	//! Translates the physics simulation by the specified vector, adjusting all bodies and constraints accordingly.
	void			Translate( const idVec3& translation, int id = -1 );

	//! Rotates the physics simulation by the specified rotation around the origin.
	void			Rotate( const idRotation& rotation, int id = -1 );

	//! Returns the world origin of a specific body in the physics AF object.
	const idVec3&	GetOrigin( int id = 0 ) const;

	//! Returns the world axis of the specified body in the physics simulation.
	const idMat3&	GetAxis( int id = 0 ) const;

	//! Sets the linear velocity of a specific body in the physics simulation.
	void			SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	//! Sets the angular velocity of a specific body in the physics simulation.
	void			SetAngularVelocity( const idVec3& newAngularVelocity, int id = 0 );

	//! Returns the linear velocity of a specific body in the physics simulation
	const idVec3&	GetLinearVelocity( int id = 0 ) const;

	//! Returns the angular velocity of the specified body in the physics simulation.
	const idVec3&	GetAngularVelocity( int id = 0 ) const;

	//! Performs translation clipping for the articulated figure physics system using the provided trace results and translation vector.
	void			ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;

	//! Tests a rotation for collision against a clip model and updates the trace results with the minimum collision fraction.
	void			ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;

	//! Returns the contents of the physics AF collision models that overlap with the given clip model.
	int				ClipContents( const idClipModel* model ) const;

	//! Disables the clip model for all bodies in the articulated figure physics system.
	void			DisableClip();

	//! Enables the collision model for all bodies in the physics system.
	void			EnableClip();

	//! Unlinks all clip models associated with the physics bodies.
	void			UnlinkClip();

	//! Links the physics affine frame to its collision model.
	void			LinkClip();

	//! Evaluates and processes collision contacts for the articulated figure physics system
	bool			EvaluateContacts();

	//! Sets the pushed velocity of the physics object based on the delta time.
	void			SetPushed( int deltaTime );

	//! Returns the linear velocity of the articulated figure physics object at the specified index.
	const idVec3&	GetPushedLinearVelocity( const int id = 0 ) const;

	//! Returns the angular velocity of the physics object at the specified index.
	const idVec3&	GetPushedAngularVelocity( const int id = 0 ) const;

	//! Sets the master entity for the physics articulated figure, optionally orienting it based on constraints.
	void			SetMaster( idEntity* master, const bool orientated = true );

	//! Writes the articulated figure physics state to a bit message for network synchronization.
	void			WriteToSnapshot( idBitMsg& msg ) const;

	//! Restores the articulated figure physics state from a network snapshot message
	void			ReadFromSnapshot( const idBitMsg& msg );

private:
	// articulated figure
	idList<idAFTree*, TAG_IDLIB_LIST_PHYSICS>				trees;				  // tree structures
	idList<idAFBody*, TAG_IDLIB_LIST_PHYSICS>				bodies;				  // all bodies
	idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS>			constraints;		  // all frame independent constraints
	idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS>			primaryConstraints;	  // list with primary constraints
	idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS>			auxiliaryConstraints; // list with auxiliary constraints
	idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS>			frameConstraints;	  // constraints that only live one frame
	idList<idAFConstraint_Contact*, TAG_IDLIB_LIST_PHYSICS> contactConstraints;	  // contact constraints
	idList<int, TAG_IDLIB_LIST_PHYSICS>						contactBodies;		  // body id for each contact
	idList<AFCollision_t, TAG_IDLIB_LIST_PHYSICS>			collisions;			  // collisions
	bool													changedAF;			  // true when the articulated figure just changed

	// properties
	float													linearFriction;	 // default translational friction
	float													angularFriction; // default rotational friction
	float													contactFriction; // default friction with contact surfaces
	float													bouncyness;		 // default bouncyness
	float													totalMass;		 // total mass of articulated figure
	float													forceTotalMass;	 // force this total mass

	idVec2													suspendVelocity;	 // simulation may not be suspended if a body has more velocity
	idVec2													suspendAcceleration; // simulation may not be suspended if a body has more acceleration
	float													noMoveTime;			 // suspend simulation if hardly any movement for this many seconds
	float													noMoveTranslation;	 // maximum translation considered no movement
	float													noMoveRotation;		 // maximum rotation considered no movement
	float													minMoveTime;		 // if > 0 the simulation is never suspended before running this many seconds
	float													maxMoveTime;		 // if > 0 the simulation is always suspeded after running this many seconds
	float													impulseThreshold;	 // threshold below which impulses are ignored to avoid continuous activation

	float													timeScale;			// the time is scaled with this value for slow motion effects
	float													timeScaleRampStart; // start of time scale change
	float													timeScaleRampEnd;	// end of time scale change

	float													jointFrictionScale;		// joint friction scale
	float													jointFrictionDent;		// joint friction dives from 1 to this value and goes up again
	float													jointFrictionDentStart; // start time of joint friction dent
	float													jointFrictionDentEnd;	// end time of joint friction dent
	float													jointFrictionDentScale; // dent scale

	float													contactFrictionScale;	  // contact friction scale
	float													contactFrictionDent;	  // contact friction dives from 1 to this value and goes up again
	float													contactFrictionDentStart; // start time of contact friction dent
	float													contactFrictionDentEnd;	  // end time of contact friction dent
	float													contactFrictionDentScale; // dent scale

	bool													enableCollision;		// if true collision detection is enabled
	bool													selfCollision;			// if true the self collision is allowed
	bool													comeToRest;				// if true the figure can come to rest
	bool													linearTime;				// if true use the linear time algorithm
	bool													noImpact;				// if true do not activate when another object collides
	bool													worldConstraintsLocked; // if true world constraints cannot be moved
	bool													forcePushable;			// if true can be pushed even when bound to a master

	// physics state
	AFPState_t												current;
	AFPState_t												saved;

	idAFBody*												masterBody; // master body
	idLCP*													lcp;		// linear complementarity problem solver

private:
	//! Constructs the tree structures for the articulated figure physics system
	void	  BuildTrees();

	//! Determines whether two rigid bodies belong to the same closed loop in a physics simulation.
	bool	  IsClosedLoop( const idAFBody* body1, const idAFBody* body2 ) const;

	//! Factors the primary trees in the physics simulation.
	void	  PrimaryFactor();

	//! Evaluates the physical properties of each body in the articulated figure physics simulation.
	void	  EvaluateBodies( float timeStep );

	//! Evaluates all constraints for the physics simulation using the provided time step.
	void	  EvaluateConstraints( float timeStep );

	//! Appends frame constraints to the auxiliary constraints list.
	void	  AddFrameConstraints();

	//! Removes all frame constraints from the auxiliary constraints and clears the frame constraints list.
	void	  RemoveFrameConstraints();

	//! Applies friction to the articulated figure physics simulation based on time steps and contact constraints.
	void	  ApplyFriction( float timeStep, float endTimeMSec );

	//! Calculates forces for all trees in the physics simulation using the provided time step.
	void	  PrimaryForces( float timeStep );

	//! Computes auxiliary forces for articulated figure physics simulation
	void	  AuxiliaryForces( float timeStep );

	//! Verifies contact constraints for the articulated figure physics simulation.
	void	  VerifyContactConstraints();

	//! Initializes and sets up contact constraints for the articulated figure physics system.
	void	  SetupContactConstraints();

	//! Applies contact forces to the physics object.
	void	  ApplyContactForces();

	//! Advances the physics simulation state by the specified time step for each body in the articulated figure.
	void	  Evolve( float timeStep );

	//! Configures collision settings for a physics body and returns an entity to pass collision detection.
	idEntity* SetupCollisionForBody( idAFBody* body ) const;

	//! Applies an impulse to colliding bodies during physics simulation
	bool	  CollisionImpulse( float timeStep, idAFBody* body, trace_t& collision );

	//! Applies collisions to the physics system and returns true if any collision was processed.
	bool	  ApplyCollisions( float timeStep );

	//! Checks for collisions between the current and next states of physics bodies and updates the next state to the collision impact point if a collision occurs.
	void	  CheckForCollisions( float timeStep );

	//! Clears the external force applied to all bodies in the physics simulation.
	void	  ClearExternalForce();

	//! Adds gravitational force to all bodies in the physics simulation.
	void	  AddGravity();

	//! Swaps the current and next states for all bodies in the physics simulation.
	void	  SwapStates();

	//! Tests if the articulated figure physics simulation has come to rest based on movement thresholds and time requirements.
	bool	  TestIfAtRest( float timeStep );

	//! Sets the physics simulation to rest state and clears external forces.
	void	  Rest();

	//! Adds a push velocity to all bodies in the physics simulation.
	void	  AddPushVelocity( const idVec6& pushVelocity );

	//! Draws debug visualization for the articulated figure physics simulation
	void	  DebugDraw();
};

#endif /* !__PHYSICS_AF_H__ */
