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

#include "precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

CLASS_DECLARATION( idPhysics_Base, idPhysics_RigidBody )
END_CLASS

const float STOP_SPEED = 10.0f;

#undef RB_TIMINGS

#ifdef RB_TIMINGS
static int	   lastTimerReset = 0;
static int	   numRigidBodies = 0;
static idTimer timer_total, timer_collision;
#endif

/*!
	\brief Computes the time derivatives of a rigid body's state variables for physics simulation

	This function calculates the rates of change for linear velocity, angular matrix, force, and torque of a rigid body based on its current state and physical properties. It is used in numerical
   integration for rigid body dynamics simulation. The function takes the current time, physics object data, current state values, and computes the derivatives that represent how the state will evolve
   over time. The computation involves inverse inertia tensor calculations, skew-symmetric matrices, and applies friction forces and external forces to determine the rate of change of momentum and
   orientation.

	\param t Current simulation time
	\param clientData Pointer to the rigid body physics object containing physical properties like mass, inertia tensor, friction coefficients, and external forces
	\param state Current state of the rigid body containing linear momentum, angular momentum, and orientation
	\param derivatives Output array containing computed derivatives for linear velocity, angular matrix, force, and torque
*/
void RigidBodyDerivatives( const float t, const void* clientData, const float* state, float* derivatives )
{
	const idPhysics_RigidBody* p = ( idPhysics_RigidBody* )clientData;
	rigidBodyIState_t*		   s = ( rigidBodyIState_t* )state;
	// NOTE: this struct should be build conform rigidBodyIState_t
	struct rigidBodyDerivatives_s {
		idVec3 linearVelocity;
		idMat3 angularMatrix;
		idVec3 force;
		idVec3 torque;
	}*	   d = ( struct rigidBodyDerivatives_s* )derivatives;
	idVec3 angularVelocity;
	idMat3 inverseWorldInertiaTensor;

	inverseWorldInertiaTensor = s->orientation * p->inverseInertiaTensor * s->orientation.Transpose();
	angularVelocity			  = inverseWorldInertiaTensor * s->angularMomentum;
	// derivatives
	d->linearVelocity = p->inverseMass * s->linearMomentum;
	d->angularMatrix  = SkewSymmetric( angularVelocity ) * s->orientation;
	d->force		  = -p->linearFriction * s->linearMomentum + p->current.externalForce;
	d->torque		  = -p->angularFriction * s->angularMomentum + p->current.externalTorque;
}

void idPhysics_RigidBody::Integrate( float deltaTime, rigidBodyPState_t& next_ )
{
	idVec3 position;

	position = current.i.position;
	current.i.position += centerOfMass * current.i.orientation;

	current.i.orientation.TransposeSelf();

	integrator->Evaluate( ( float* )&current.i, ( float* )&next_.i, 0, deltaTime );
	next_.i.orientation.OrthoNormalizeSelf();

	// apply gravity
	next_.i.linearMomentum += deltaTime * gravityVector * mass;

	current.i.orientation.TransposeSelf();
	next_.i.orientation.TransposeSelf();

	current.i.position = position;
	next_.i.position -= centerOfMass * next_.i.orientation;

	next_.atRest = current.atRest;
}

bool idPhysics_RigidBody::CollisionImpulse( const trace_t& collision, idVec3& impulse )
{
	idVec3		 r, linearVelocity, angularVelocity, velocity;
	idMat3		 inverseWorldInertiaTensor;
	float		 impulseNumerator, impulseDenominator, vel;
	impactInfo_t info;
	idEntity*	 ent;

	// get info from other entity involved
	ent = gameLocal.entities[collision.c.entityNum];
	ent->GetImpactInfo( self, collision.c.id, collision.c.point, &info );

	// collision point relative to the body center of mass
	r = collision.c.point - ( current.i.position + centerOfMass * current.i.orientation );
	// the velocity at the collision point
	linearVelocity			  = inverseMass * current.i.linearMomentum;
	inverseWorldInertiaTensor = current.i.orientation.Transpose() * inverseInertiaTensor * current.i.orientation;
	angularVelocity			  = inverseWorldInertiaTensor * current.i.angularMomentum;
	velocity				  = linearVelocity + angularVelocity.Cross( r );
	// subtract velocity of other entity
	velocity -= info.velocity;

	// velocity in normal direction
	vel = velocity * collision.c.normal;

	if( vel > -STOP_SPEED ) {
		impulseNumerator = STOP_SPEED;
	} else {
		impulseNumerator = -( 1.0f + bouncyness ) * vel;
	}
	impulseDenominator = inverseMass + ( ( inverseWorldInertiaTensor * r.Cross( collision.c.normal ) ).Cross( r ) * collision.c.normal );
	if( info.invMass ) {
		impulseDenominator += info.invMass + ( ( info.invInertiaTensor * info.position.Cross( collision.c.normal ) ).Cross( info.position ) * collision.c.normal );
	}
	impulse = ( impulseNumerator / impulseDenominator ) * collision.c.normal;

	// update linear and angular momentum with impulse
	current.i.linearMomentum += impulse;
	current.i.angularMomentum += r.Cross( impulse );

	// if no movement at all don't blow up
	if( collision.fraction < 0.0001f ) {
		current.i.linearMomentum *= 0.5f;
		current.i.angularMomentum *= 0.5f;
	}

	// callback to self to let the entity know about the collision
	return self->Collide( collision, velocity );
}

bool idPhysics_RigidBody::CheckForCollisions( const float deltaTime, rigidBodyPState_t& next_, trace_t& collision )
{
	// #define TEST_COLLISION_DETECTION
	idMat3	   axis;
	idRotation rotation;
	bool	   collided = false;

#ifdef TEST_COLLISION_DETECTION
	bool startsolid;
	if( gameLocal.clip.Contents( current.i.position, clipModel, current.i.orientation, clipMask, self ) ) {
		startsolid = true;
	}
#endif

	TransposeMultiply( current.i.orientation, next_.i.orientation, axis );
	rotation = axis.ToRotation();
	rotation.SetOrigin( current.i.position );

	// if there was a collision
	if( gameLocal.clip.Motion( collision, current.i.position, next_.i.position, rotation, clipModel, current.i.orientation, clipMask, self ) ) {
		// set the next state to the state at the moment of impact
		next_.i.position		= collision.endpos;
		next_.i.orientation		= collision.endAxis;
		next_.i.linearMomentum	= current.i.linearMomentum;
		next_.i.angularMomentum = current.i.angularMomentum;
		collided				= true;
	}

#ifdef TEST_COLLISION_DETECTION
	if( gameLocal.clip.Contents( next.i.position, clipModel, next_.i.orientation, clipMask, self ) ) {
		if( !startsolid ) {
			int bah = 1;
		}
	}
#endif
	return collided;
}

void idPhysics_RigidBody::ContactFriction( float deltaTime )
{
	int	   i;
	float  magnitude, impulseNumerator, impulseDenominator;
	idMat3 inverseWorldInertiaTensor;
	idVec3 linearVelocity, angularVelocity;
	idVec3 massCenter, r, velocity, normal, impulse, normalVelocity;

	inverseWorldInertiaTensor = current.i.orientation.Transpose() * inverseInertiaTensor * current.i.orientation;

	massCenter = current.i.position + centerOfMass * current.i.orientation;

	for( i = 0; i < contacts.Num(); i++ ) {
		r = contacts[i].point - massCenter;

		// calculate velocity at contact point
		linearVelocity	= inverseMass * current.i.linearMomentum;
		angularVelocity = inverseWorldInertiaTensor * current.i.angularMomentum;
		velocity		= linearVelocity + angularVelocity.Cross( r );

		// velocity along normal vector
		normalVelocity = ( velocity * contacts[i].normal ) * contacts[i].normal;

		// calculate friction impulse
		normal			   = -( velocity - normalVelocity );
		magnitude		   = normal.Normalize();
		impulseNumerator   = contactFriction * magnitude;
		impulseDenominator = inverseMass + ( ( inverseWorldInertiaTensor * r.Cross( normal ) ).Cross( r ) * normal );
		impulse			   = ( impulseNumerator / impulseDenominator ) * normal;

		// apply friction impulse
		current.i.linearMomentum += impulse;
		current.i.angularMomentum += r.Cross( impulse );

		// if moving towards the surface at the contact point
		if( normalVelocity * contacts[i].normal < 0.0f ) {
			// calculate impulse
			normal			   = -normalVelocity;
			impulseNumerator   = normal.Normalize();
			impulseDenominator = inverseMass + ( ( inverseWorldInertiaTensor * r.Cross( normal ) ).Cross( r ) * normal );
			impulse			   = ( impulseNumerator / impulseDenominator ) * normal;

			// apply impulse
			current.i.linearMomentum += impulse;
			current.i.angularMomentum += r.Cross( impulse );
		}
	}
}

bool idPhysics_RigidBody::TestIfAtRest() const
{
	int			   i;
	float		   gv;
	idVec3		   v, av, normal, point;
	idMat3		   inverseWorldInertiaTensor;
	idFixedWinding contactWinding;

	if( current.atRest >= 0 ) {
		return true;
	}

	// need at least 3 contact points to come to rest
	if( contacts.Num() < 3 ) {
		return false;
	}

	// get average contact plane normal
	normal.Zero();
	for( i = 0; i < contacts.Num(); i++ ) {
		normal += contacts[i].normal;
	}
	normal /= ( float )contacts.Num();
	normal.Normalize();

	// if on a too steep surface
	if( ( normal * gravityNormal ) > -0.7f ) {
		return false;
	}

	// create bounds for contact points
	contactWinding.Clear();
	for( i = 0; i < contacts.Num(); i++ ) {
		// project point onto plane through origin orthogonal to the gravity
		point = contacts[i].point - ( contacts[i].point * gravityNormal ) * gravityNormal;
		contactWinding.AddToConvexHull( point, gravityNormal );
	}

	// need at least 3 contact points to come to rest
	if( contactWinding.GetNumPoints() < 3 ) {
		return false;
	}

	// center of mass in world space
	point = current.i.position + centerOfMass * current.i.orientation;
	point -= ( point * gravityNormal ) * gravityNormal;

	// if the point is not inside the winding
	if( !contactWinding.PointInside( gravityNormal, point, 0 ) ) {
		return false;
	}

	// linear velocity of body
	v = inverseMass * current.i.linearMomentum;
	// linear velocity in gravity direction
	gv = v * gravityNormal;
	// linear velocity orthogonal to gravity direction
	v -= gv * gravityNormal;

	// if too much velocity orthogonal to gravity direction
	if( v.Length() > STOP_SPEED ) {
		return false;
	}
	// if too much velocity in gravity direction
	if( gv > 2.0f * STOP_SPEED || gv < -2.0f * STOP_SPEED ) {
		return false;
	}

	// calculate rotational velocity
	inverseWorldInertiaTensor = current.i.orientation * inverseInertiaTensor * current.i.orientation.Transpose();
	av						  = inverseWorldInertiaTensor * current.i.angularMomentum;

	// if too much rotational velocity
	if( av.LengthSqr() > STOP_SPEED ) {
		return false;
	}

	return true;
}

void idPhysics_RigidBody::DropToFloorAndRest()
{
	idVec3	down;
	trace_t tr;

	if( testSolid ) {
		testSolid = false;

		if( gameLocal.clip.Contents( current.i.position, clipModel, current.i.orientation, clipMask, self ) ) {
			gameLocal.DWarning( "rigid body in solid for entity '%s' type '%s' at (%s)", self->name.c_str(), self->GetType()->classname, current.i.position.ToString( 0 ) );
			Rest();
			dropToFloor = false;
			return;
		}
	}

	// put the body on the floor
	down = current.i.position + gravityNormal * 128.0f;
	gameLocal.clip.Translation( tr, current.i.position, down, clipModel, current.i.orientation, clipMask, self );
	current.i.position = tr.endpos;
	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), tr.endpos, current.i.orientation );

	// if on the floor already
	if( tr.fraction == 0.0f ) {
		// test if we are really at rest
		EvaluateContacts();
		if( !TestIfAtRest() ) {
			gameLocal.DWarning( "rigid body not at rest for entity '%s' type '%s' at (%s)", self->name.c_str(), self->GetType()->classname, current.i.position.ToString( 0 ) );
		}
		Rest();
		dropToFloor = false;
	} else if( IsOutsideWorld() ) {
		gameLocal.Warning( "rigid body outside world bounds for entity '%s' type '%s' at (%s)", self->name.c_str(), self->GetType()->classname, current.i.position.ToString( 0 ) );
		Rest();
		dropToFloor = false;
	}
}

void idPhysics_RigidBody::DebugDraw()
{
	if( rb_showBodies.GetBool() || ( rb_showActive.GetBool() && current.atRest < 0 ) ) {
		collisionModelManager->DrawModel( clipModel->Handle(), clipModel->GetOrigin(), clipModel->GetAxis(), vec3_origin, 0.0f );
	}

	if( rb_showMass.GetBool() ) {
		gameRenderWorld->DrawText( va( "\n%1.2f", mass ), current.i.position, 0.08f, colorCyan, gameLocal.GetLocalPlayer()->viewAngles.ToMat3(), 1 );
	}

	if( rb_showInertia.GetBool() ) {
		idMat3& I = inertiaTensor;
		gameRenderWorld->DrawText( va( "\n\n\n( %.1f %.1f %.1f )\n( %.1f %.1f %.1f )\n( %.1f %.1f %.1f )", I[0].x, I[0].y, I[0].z, I[1].x, I[1].y, I[1].z, I[2].x, I[2].y, I[2].z ),
			current.i.position,
			0.05f,
			colorCyan,
			gameLocal.GetLocalPlayer()->viewAngles.ToMat3(),
			1 );
	}

	if( rb_showVelocity.GetBool() ) {
		DrawVelocity( clipModel->GetId(), 0.1f, 4.0f );
	}
}

idPhysics_RigidBody::idPhysics_RigidBody()
{
	// set default rigid body properties
	SetClipMask( MASK_SOLID );
	SetBouncyness( 0.6f );
	SetFriction( 0.6f, 0.6f, 0.0f );
	clipModel = NULL;

	current.atRest		 = -1;
	current.lastTimeStep = 0.0f;

	current.i.position.Zero();
	current.i.orientation.Identity();

	current.i.linearMomentum.Zero();
	current.i.angularMomentum.Zero();

	saved = current;

	mass		= 1.0f;
	inverseMass = 1.0f;
	centerOfMass.Zero();
	inertiaTensor.Identity();
	inverseInertiaTensor.Identity();

	// use the least expensive euler integrator
	integrator = new( TAG_PHYSICS ) idODE_Euler( sizeof( rigidBodyIState_t ) / sizeof( float ), RigidBodyDerivatives, this );

	dropToFloor = false;
	noImpact	= false;
	noContact	= false;

	hasMaster	 = false;
	isOrientated = false;

#ifdef RB_TIMINGS
	lastTimerReset = 0;
#endif
}

idPhysics_RigidBody::~idPhysics_RigidBody()
{
	if( clipModel ) {
		delete clipModel;
		clipModel = NULL;
	}
	delete integrator;
}

//! Saves the rigid body physics state to a save file.
void idPhysics_RigidBody_SavePState( idSaveGame* savefile, const rigidBodyPState_t& state )
{
	savefile->WriteInt( state.atRest );
	savefile->WriteFloat( state.lastTimeStep );
	savefile->WriteVec3( state.localOrigin );
	savefile->WriteMat3( state.localAxis );
	savefile->WriteVec6( state.pushVelocity );
	savefile->WriteVec3( state.externalForce );
	savefile->WriteVec3( state.externalTorque );

	savefile->WriteVec3( state.i.position );
	savefile->WriteMat3( state.i.orientation );
	savefile->WriteVec3( state.i.linearMomentum );
	savefile->WriteVec3( state.i.angularMomentum );
}

//! Restores the rigid body physics state from a save file.
void idPhysics_RigidBody_RestorePState( idRestoreGame* savefile, rigidBodyPState_t& state )
{
	savefile->ReadInt( state.atRest );
	savefile->ReadFloat( state.lastTimeStep );
	savefile->ReadVec3( state.localOrigin );
	savefile->ReadMat3( state.localAxis );
	savefile->ReadVec6( state.pushVelocity );
	savefile->ReadVec3( state.externalForce );
	savefile->ReadVec3( state.externalTorque );

	savefile->ReadVec3( state.i.position );
	savefile->ReadMat3( state.i.orientation );
	savefile->ReadVec3( state.i.linearMomentum );
	savefile->ReadVec3( state.i.angularMomentum );
}

void idPhysics_RigidBody::Save( idSaveGame* savefile ) const
{
	idPhysics_RigidBody_SavePState( savefile, current );
	idPhysics_RigidBody_SavePState( savefile, saved );

	savefile->WriteFloat( linearFriction );
	savefile->WriteFloat( angularFriction );
	savefile->WriteFloat( contactFriction );
	savefile->WriteFloat( bouncyness );
	savefile->WriteClipModel( clipModel );

	savefile->WriteFloat( mass );
	savefile->WriteFloat( inverseMass );
	savefile->WriteVec3( centerOfMass );
	savefile->WriteMat3( inertiaTensor );
	savefile->WriteMat3( inverseInertiaTensor );

	savefile->WriteBool( dropToFloor );
	savefile->WriteBool( testSolid );
	savefile->WriteBool( noImpact );
	savefile->WriteBool( noContact );

	savefile->WriteBool( hasMaster );
	savefile->WriteBool( isOrientated );
}

void idPhysics_RigidBody::Restore( idRestoreGame* savefile )
{
	idPhysics_RigidBody_RestorePState( savefile, current );
	idPhysics_RigidBody_RestorePState( savefile, saved );

	savefile->ReadFloat( linearFriction );
	savefile->ReadFloat( angularFriction );
	savefile->ReadFloat( contactFriction );
	savefile->ReadFloat( bouncyness );
	savefile->ReadClipModel( clipModel );

	savefile->ReadFloat( mass );
	savefile->ReadFloat( inverseMass );
	savefile->ReadVec3( centerOfMass );
	savefile->ReadMat3( inertiaTensor );
	savefile->ReadMat3( inverseInertiaTensor );

	savefile->ReadBool( dropToFloor );
	savefile->ReadBool( testSolid );
	savefile->ReadBool( noImpact );
	savefile->ReadBool( noContact );

	savefile->ReadBool( hasMaster );
	savefile->ReadBool( isOrientated );
}

/*
================
idPhysics_RigidBody::SetClipModel
================
*/
#define MAX_INERTIA_SCALE 10.0f

void idPhysics_RigidBody::SetClipModel( idClipModel* model, const float density, int id, bool freeOld )
{
	int	   minIndex;
	idMat3 inertiaScale;

	assert( self );
	assert( model );				 // we need a clip model
	assert( model->IsTraceModel() ); // and it should be a trace model
	assert( density > 0.0f );		 // density should be valid

	if( clipModel && clipModel != model && freeOld ) {
		delete clipModel;
	}
	clipModel = model;
	clipModel->Link( gameLocal.clip, self, 0, current.i.position, current.i.orientation );

	// get mass properties from the trace model
	clipModel->GetMassProperties( density, mass, centerOfMass, inertiaTensor );

	// check whether or not the clip model has valid mass properties
	if( mass <= 0.0f || IEEE_FLT_IS_NAN( mass ) ) {
		gameLocal.Warning( "idPhysics_RigidBody::SetClipModel: invalid mass for entity '%s' type '%s'", self->name.c_str(), self->GetType()->classname );
		mass = 1.0f;
		centerOfMass.Zero();
		inertiaTensor.Identity();
	}

	// check whether or not the inertia tensor is balanced
	minIndex = Min3Index( inertiaTensor[0][0], inertiaTensor[1][1], inertiaTensor[2][2] );
	inertiaScale.Identity();
	inertiaScale[0][0] = inertiaTensor[0][0] / inertiaTensor[minIndex][minIndex];
	inertiaScale[1][1] = inertiaTensor[1][1] / inertiaTensor[minIndex][minIndex];
	inertiaScale[2][2] = inertiaTensor[2][2] / inertiaTensor[minIndex][minIndex];

	if( inertiaScale[0][0] > MAX_INERTIA_SCALE || inertiaScale[1][1] > MAX_INERTIA_SCALE || inertiaScale[2][2] > MAX_INERTIA_SCALE ) {
		gameLocal.DWarning( "idPhysics_RigidBody::SetClipModel: unbalanced inertia tensor for entity '%s' type '%s'", self->name.c_str(), self->GetType()->classname );
		float min												 = inertiaTensor[minIndex][minIndex] * MAX_INERTIA_SCALE;
		inertiaScale[( minIndex + 1 ) % 3][( minIndex + 1 ) % 3] = min / inertiaTensor[( minIndex + 1 ) % 3][( minIndex + 1 ) % 3];
		inertiaScale[( minIndex + 2 ) % 3][( minIndex + 2 ) % 3] = min / inertiaTensor[( minIndex + 2 ) % 3][( minIndex + 2 ) % 3];
		inertiaTensor *= inertiaScale;
	}

	inverseMass			 = 1.0f / mass;
	inverseInertiaTensor = inertiaTensor.Inverse() * ( 1.0f / 6.0f );

	current.i.linearMomentum.Zero();
	current.i.angularMomentum.Zero();
}

idClipModel* idPhysics_RigidBody::GetClipModel( int id ) const
{
	return clipModel;
}

int idPhysics_RigidBody::GetNumClipModels() const
{
	return 1;
}

void idPhysics_RigidBody::SetMass( float mass, int id )
{
	assert( mass > 0.0f );
	inertiaTensor *= mass / this->mass;
	inverseInertiaTensor = inertiaTensor.Inverse() * ( 1.0f / 6.0f );
	this->mass			 = mass;
	inverseMass			 = 1.0f / mass;
}

float idPhysics_RigidBody::GetMass( int id ) const
{
	return mass;
}

void idPhysics_RigidBody::SetFriction( const float linear, const float angular, const float contact )
{
	if( linear < 0.0f || linear > 1.0f || angular < 0.0f || angular > 1.0f || contact < 0.0f || contact > 1.0f ) {
		return;
	}
	linearFriction	= linear;
	angularFriction = angular;
	contactFriction = contact;
}

void idPhysics_RigidBody::SetBouncyness( const float b )
{
	if( b < 0.0f || b > 1.0f ) {
		return;
	}
	bouncyness = b;
}

void idPhysics_RigidBody::Rest()
{
	current.atRest = gameLocal.time;
	current.i.linearMomentum.Zero();
	current.i.angularMomentum.Zero();
	self->BecomeInactive( TH_PHYSICS );
}

void idPhysics_RigidBody::DropToFloor()
{
	dropToFloor = true;
	testSolid	= true;
}

void idPhysics_RigidBody::NoContact()
{
	noContact = true;
}

void idPhysics_RigidBody::Activate()
{
	current.atRest = -1;
	self->BecomeActive( TH_PHYSICS );
}

void idPhysics_RigidBody::PutToRest()
{
	Rest();
}

void idPhysics_RigidBody::EnableImpact()
{
	noImpact = false;
}

void idPhysics_RigidBody::DisableImpact()
{
	noImpact = true;
}

void idPhysics_RigidBody::SetContents( int contents, int id )
{
	clipModel->SetContents( contents );
}

int idPhysics_RigidBody::GetContents( int id ) const
{
	return clipModel->GetContents();
}

const idBounds& idPhysics_RigidBody::GetBounds( int id ) const
{
	return clipModel->GetBounds();
}

const idBounds& idPhysics_RigidBody::GetAbsBounds( int id ) const
{
	return clipModel->GetAbsBounds();
}

bool idPhysics_RigidBody::Evaluate( int timeStepMSec, int endTimeMSec )
{
	rigidBodyPState_t next_step;
	idAngles		  angles;
	trace_t			  collision;
	idVec3			  impulse;
	idEntity*		  ent;
	idVec3			  oldOrigin, masterOrigin;
	idMat3			  oldAxis, masterAxis;
	float			  timeStep;
	bool			  collided, cameToRest = false;

	timeStep			 = MS2SEC( timeStepMSec );
	current.lastTimeStep = timeStep;

	if( hasMaster ) {
		oldOrigin = current.i.position;
		oldAxis	  = current.i.orientation;
		self->GetMasterPosition( masterOrigin, masterAxis );
		current.i.position = masterOrigin + current.localOrigin * masterAxis;
		if( isOrientated ) {
			current.i.orientation = current.localAxis * masterAxis;
		} else {
			current.i.orientation = current.localAxis;
		}
		clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, current.i.orientation );
		current.i.linearMomentum  = mass * ( ( current.i.position - oldOrigin ) / timeStep );
		current.i.angularMomentum = inertiaTensor * ( ( current.i.orientation * oldAxis.Transpose() ).ToAngularVelocity() / timeStep );
		current.externalForce.Zero();
		current.externalTorque.Zero();

		return ( current.i.position != oldOrigin || current.i.orientation != oldAxis );
	}

	// if the body is at rest
	if( current.atRest >= 0 || timeStep <= 0.0f ) {
		DebugDraw();
		return false;
	}

	// if putting the body to rest
	if( dropToFloor ) {
		DropToFloorAndRest();
		current.externalForce.Zero();
		current.externalTorque.Zero();
		return true;
	}

#ifdef RB_TIMINGS
	timer_total.Start();
#endif

	// move the rigid body velocity into the frame of a pusher
	//	current.i.linearMomentum -= current.pushVelocity.SubVec3( 0 ) * mass;
	//	current.i.angularMomentum -= current.pushVelocity.SubVec3( 1 ) * inertiaTensor;

	clipModel->Unlink();

	next_step = current;

	// calculate next position and orientation
	Integrate( timeStep, next_step );

#ifdef RB_TIMINGS
	timer_collision.Start();
#endif

	// check for collisions from the current to the next state
	collided = CheckForCollisions( timeStep, next_step, collision );

#ifdef RB_TIMINGS
	timer_collision.Stop();
#endif

	// set the new state
	current = next_step;

	if( collided ) {
		// apply collision impulse
		if( CollisionImpulse( collision, impulse ) ) {
			current.atRest = gameLocal.time;
		}
	}

	// update the position of the clip model
	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, current.i.orientation );

	DebugDraw();

	if( !noContact ) {
#ifdef RB_TIMINGS
		timer_collision.Start();
#endif
		// get contacts
		EvaluateContacts();

#ifdef RB_TIMINGS
		timer_collision.Stop();
#endif

		// check if the body has come to rest
		if( TestIfAtRest() ) {
			// put to rest
			Rest();
			cameToRest = true;
		} else {
			// apply contact friction
			ContactFriction( timeStep );
		}
	}

	if( current.atRest < 0 ) {
		ActivateContactEntities();
	}

	if( collided ) {
		// if the rigid body didn't come to rest or the other entity is not at rest
		ent = gameLocal.entities[collision.c.entityNum];
		if( ent && ( !cameToRest || !ent->IsAtRest() ) ) {
			// apply impact to other entity
			ent->ApplyImpulse( self, collision.c.id, collision.c.point, -impulse );
		}
	}

	// move the rigid body velocity back into the world frame
	//	current.i.linearMomentum += current.pushVelocity.SubVec3( 0 ) * mass;
	//	current.i.angularMomentum += current.pushVelocity.SubVec3( 1 ) * inertiaTensor;
	current.pushVelocity.Zero();

	current.lastTimeStep = timeStep;
	current.externalForce.Zero();
	current.externalTorque.Zero();

	if( IsOutsideWorld() ) {
		gameLocal.Warning( "rigid body moved outside world bounds for entity '%s' type '%s' at (%s)", self->name.c_str(), self->GetType()->classname, current.i.position.ToString( 0 ) );
		Rest();
	}

#ifdef RB_TIMINGS
	timer_total.Stop();

	if( rb_showTimings->integer == 1 ) {
		gameLocal.Printf( "%12s: t %1.4f cd %1.4f\n", self->name.c_str(), timer_total.Milliseconds(), timer_collision.Milliseconds() );
		lastTimerReset = 0;
	} else if( rb_showTimings->integer == 2 ) {
		numRigidBodies++;
		if( endTimeMSec > lastTimerReset ) {
			gameLocal.Printf( "rb %d: t %1.4f cd %1.4f\n", numRigidBodies, timer_total.Milliseconds(), timer_collision.Milliseconds() );
		}
	}
	if( endTimeMSec > lastTimerReset ) {
		lastTimerReset = endTimeMSec;
		numRigidBodies = 0;
		timer_total.Clear();
		timer_collision.Clear();
	}
#endif

	return true;
}

bool idPhysics_RigidBody::Interpolate( const float fraction )
{
	if( !self ) {
		return false;
	}

	if( self->GetInterpolationBehavior() == idEntity::USE_LATEST_SNAP_ONLY ) {
		current = next;
		return true;
	} else if( self->GetInterpolationBehavior() == idEntity::USE_INTERPOLATION ) {
		current.i.position		 = Lerp( previous.i.position, next.i.position, fraction );
		current.i.orientation	 = idQuat().Slerp( previous.i.orientation.ToQuat(), next.i.orientation.ToQuat(), fraction ).ToMat3();
		current.i.linearMomentum = Lerp( previous.i.linearMomentum, next.i.linearMomentum, fraction );
		return true;
	}

	return false;
}

void idPhysics_RigidBody::ResetInterpolationState( const idVec3& origin, const idMat3& axis )
{
	previous = current;
	next	 = current;
}

void idPhysics_RigidBody::UpdateTime( int endTimeMSec )
{
}

int idPhysics_RigidBody::GetTime() const
{
	return gameLocal.time;
}

void idPhysics_RigidBody::GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const
{
	idVec3 linearVelocity, angularVelocity;
	idMat3 inverseWorldInertiaTensor;

	linearVelocity			  = inverseMass * current.i.linearMomentum;
	inverseWorldInertiaTensor = current.i.orientation.Transpose() * inverseInertiaTensor * current.i.orientation;
	angularVelocity			  = inverseWorldInertiaTensor * current.i.angularMomentum;

	info->invMass		   = inverseMass;
	info->invInertiaTensor = inverseWorldInertiaTensor;
	info->position		   = point - ( current.i.position + centerOfMass * current.i.orientation );
	info->velocity		   = linearVelocity + angularVelocity.Cross( info->position );
}

void idPhysics_RigidBody::ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse )
{
	if( noImpact ) {
		return;
	}
	current.i.linearMomentum += impulse;
	current.i.angularMomentum += ( point - ( current.i.position + centerOfMass * current.i.orientation ) ).Cross( impulse );
	Activate();
}

void idPhysics_RigidBody::AddForce( const int id, const idVec3& point, const idVec3& force )
{
	if( noImpact ) {
		return;
	}
	current.externalForce += force;
	current.externalTorque += ( point - ( current.i.position + centerOfMass * current.i.orientation ) ).Cross( force );
	Activate();
}

bool idPhysics_RigidBody::IsAtRest() const
{
	return current.atRest >= 0;
}

int idPhysics_RigidBody::GetRestStartTime() const
{
	return current.atRest;
}

bool idPhysics_RigidBody::IsPushable() const
{
	return ( !noImpact && !hasMaster );
}

void idPhysics_RigidBody::SaveState()
{
	saved = current;
}

void idPhysics_RigidBody::RestoreState()
{
	current = saved;

	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, current.i.orientation );

	EvaluateContacts();
}

void idPhysics_RigidBody::SetOrigin( const idVec3& newOrigin, int id )
{
	idVec3 masterOrigin;
	idMat3 masterAxis;

	current.localOrigin = newOrigin;
	if( hasMaster ) {
		self->GetMasterPosition( masterOrigin, masterAxis );
		current.i.position = masterOrigin + newOrigin * masterAxis;
	} else {
		current.i.position = newOrigin;
	}

	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, clipModel->GetAxis() );

	Activate();
}

void idPhysics_RigidBody::SetAxis( const idMat3& newAxis, int id )
{
	idVec3 masterOrigin;
	idMat3 masterAxis;

	current.localAxis = newAxis;
	if( hasMaster && isOrientated ) {
		self->GetMasterPosition( masterOrigin, masterAxis );
		current.i.orientation = newAxis * masterAxis;
	} else {
		current.i.orientation = newAxis;
	}

	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), clipModel->GetOrigin(), current.i.orientation );

	Activate();
}

void idPhysics_RigidBody::Translate( const idVec3& translation, int id )
{
	current.localOrigin += translation;
	current.i.position += translation;

	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, clipModel->GetAxis() );

	Activate();
}

void idPhysics_RigidBody::Rotate( const idRotation& rotation, int id )
{
	idVec3 masterOrigin;
	idMat3 masterAxis;

	current.i.orientation *= rotation.ToMat3();
	current.i.position *= rotation;

	if( hasMaster ) {
		self->GetMasterPosition( masterOrigin, masterAxis );
		current.localAxis *= rotation.ToMat3();
		current.localOrigin = ( current.i.position - masterOrigin ) * masterAxis.Transpose();
	} else {
		current.localAxis	= current.i.orientation;
		current.localOrigin = current.i.position;
	}

	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, current.i.orientation );

	Activate();
}

const idVec3& idPhysics_RigidBody::GetOrigin( int id ) const
{
	return current.i.position;
}

const idMat3& idPhysics_RigidBody::GetAxis( int id ) const
{
	return current.i.orientation;
}

void idPhysics_RigidBody::SetLinearVelocity( const idVec3& newLinearVelocity, int id )
{
	current.i.linearMomentum = newLinearVelocity * mass;
	Activate();
}

void idPhysics_RigidBody::SetAngularVelocity( const idVec3& newAngularVelocity, int id )
{
	current.i.angularMomentum = newAngularVelocity * inertiaTensor;
	Activate();
}

const idVec3& idPhysics_RigidBody::GetLinearVelocity( int id ) const
{
	static idVec3 curLinearVelocity;
	curLinearVelocity = current.i.linearMomentum * inverseMass;
	return curLinearVelocity;
}

const idVec3& idPhysics_RigidBody::GetAngularVelocity( int id ) const
{
	static idVec3 curAngularVelocity;
	idMat3		  inverseWorldInertiaTensor;

	inverseWorldInertiaTensor = current.i.orientation.Transpose() * inverseInertiaTensor * current.i.orientation;
	curAngularVelocity		  = inverseWorldInertiaTensor * current.i.angularMomentum;
	return curAngularVelocity;
}

void idPhysics_RigidBody::ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const
{
	if( model ) {
		gameLocal.clip.TranslationModel(
			results, clipModel->GetOrigin(), clipModel->GetOrigin() + translation, clipModel, clipModel->GetAxis(), clipMask, model->Handle(), model->GetOrigin(), model->GetAxis() );
	} else {
		gameLocal.clip.Translation( results, clipModel->GetOrigin(), clipModel->GetOrigin() + translation, clipModel, clipModel->GetAxis(), clipMask, self );
	}
}

void idPhysics_RigidBody::ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const
{
	if( model ) {
		gameLocal.clip.RotationModel( results, clipModel->GetOrigin(), rotation, clipModel, clipModel->GetAxis(), clipMask, model->Handle(), model->GetOrigin(), model->GetAxis() );
	} else {
		gameLocal.clip.Rotation( results, clipModel->GetOrigin(), rotation, clipModel, clipModel->GetAxis(), clipMask, self );
	}
}

int idPhysics_RigidBody::ClipContents( const idClipModel* model ) const
{
	if( model ) {
		return gameLocal.clip.ContentsModel( clipModel->GetOrigin(), clipModel, clipModel->GetAxis(), -1, model->Handle(), model->GetOrigin(), model->GetAxis() );
	} else {
		return gameLocal.clip.Contents( clipModel->GetOrigin(), clipModel, clipModel->GetAxis(), -1, NULL );
	}
}

void idPhysics_RigidBody::DisableClip()
{
	clipModel->Disable();
}

void idPhysics_RigidBody::EnableClip()
{
	clipModel->Enable();
}

void idPhysics_RigidBody::UnlinkClip()
{
	clipModel->Unlink();
}

void idPhysics_RigidBody::LinkClip()
{
	clipModel->Link( gameLocal.clip, self, clipModel->GetId(), current.i.position, current.i.orientation );
}

bool idPhysics_RigidBody::EvaluateContacts()
{
	idVec6 dir;
	int	   num;

	ClearContacts();

	contacts.SetNum( 10 );

	dir.SubVec3( 0 ) = current.i.linearMomentum + current.lastTimeStep * gravityVector * mass;
	dir.SubVec3( 1 ) = current.i.angularMomentum;
	dir.SubVec3( 0 ).Normalize();
	dir.SubVec3( 1 ).Normalize();
	num = gameLocal.clip.Contacts( &contacts[0], 10, clipModel->GetOrigin(), dir, CONTACT_EPSILON, clipModel, clipModel->GetAxis(), clipMask, self );
	contacts.SetNum( num );

	AddContactEntitiesForContacts();

	return ( contacts.Num() != 0 );
}

void idPhysics_RigidBody::SetPushed( int deltaTime )
{
	idRotation rotation;

	rotation = ( saved.i.orientation * current.i.orientation ).ToRotation();

	// velocity with which the af is pushed
	current.pushVelocity.SubVec3( 0 ) += ( current.i.position - saved.i.position ) / ( deltaTime * idMath::M_MS2SEC );
	current.pushVelocity.SubVec3( 1 ) += rotation.GetVec() * -DEG2RAD( rotation.GetAngle() ) / ( deltaTime * idMath::M_MS2SEC );
}

const idVec3& idPhysics_RigidBody::GetPushedLinearVelocity( const int id ) const
{
	return current.pushVelocity.SubVec3( 0 );
}

const idVec3& idPhysics_RigidBody::GetPushedAngularVelocity( const int id ) const
{
	return current.pushVelocity.SubVec3( 1 );
}

void idPhysics_RigidBody::SetMaster( idEntity* master, const bool orientated )
{
	idVec3 masterOrigin;
	idMat3 masterAxis;

	if( master ) {
		if( !hasMaster ) {
			// transform from world space to master space
			self->GetMasterPosition( masterOrigin, masterAxis );
			current.localOrigin = ( current.i.position - masterOrigin ) * masterAxis.Transpose();
			if( orientated ) {
				current.localAxis = current.i.orientation * masterAxis.Transpose();
			} else {
				current.localAxis = current.i.orientation;
			}
			hasMaster	 = true;
			isOrientated = orientated;
			ClearContacts();
		}
	} else {
		if( hasMaster ) {
			hasMaster = false;
			Activate();
		}
	}
}

const float RB_VELOCITY_MAX			  = 16000;
const int	RB_VELOCITY_TOTAL_BITS	  = 16;
const int	RB_VELOCITY_EXPONENT_BITS = idMath::BitsForInteger( idMath::BitsForFloat( RB_VELOCITY_MAX ) ) + 1;
const int	RB_VELOCITY_MANTISSA_BITS = RB_VELOCITY_TOTAL_BITS - 1 - RB_VELOCITY_EXPONENT_BITS;
const float RB_MOMENTUM_MAX			  = 1e20f;
const int	RB_MOMENTUM_TOTAL_BITS	  = 16;
const int	RB_MOMENTUM_EXPONENT_BITS = idMath::BitsForInteger( idMath::BitsForFloat( RB_MOMENTUM_MAX ) ) + 1;
const int	RB_MOMENTUM_MANTISSA_BITS = RB_MOMENTUM_TOTAL_BITS - 1 - RB_MOMENTUM_EXPONENT_BITS;
const float RB_FORCE_MAX			  = 1e20f;
const int	RB_FORCE_TOTAL_BITS		  = 16;
const int	RB_FORCE_EXPONENT_BITS	  = idMath::BitsForInteger( idMath::BitsForFloat( RB_FORCE_MAX ) ) + 1;
const int	RB_FORCE_MANTISSA_BITS	  = RB_FORCE_TOTAL_BITS - 1 - RB_FORCE_EXPONENT_BITS;

void		idPhysics_RigidBody::WriteToSnapshot( idBitMsg& msg ) const
{
	idCQuat quat, localQuat;

	quat = current.i.orientation.ToCQuat();

	msg.WriteFloat( current.i.position[0] );
	msg.WriteFloat( current.i.position[1] );
	msg.WriteFloat( current.i.position[2] );
	msg.WriteFloat( quat.x );
	msg.WriteFloat( quat.y );
	msg.WriteFloat( quat.z );
	msg.WriteFloat( current.i.linearMomentum[0], RB_MOMENTUM_EXPONENT_BITS, RB_MOMENTUM_MANTISSA_BITS );
	msg.WriteFloat( current.i.linearMomentum[1], RB_MOMENTUM_EXPONENT_BITS, RB_MOMENTUM_MANTISSA_BITS );
	msg.WriteFloat( current.i.linearMomentum[2], RB_MOMENTUM_EXPONENT_BITS, RB_MOMENTUM_MANTISSA_BITS );
}

void idPhysics_RigidBody::ReadFromSnapshot( const idBitMsg& msg )
{
	idCQuat quat, localQuat;

	previous = next;

	next.i.position[0]		 = msg.ReadFloat();
	next.i.position[1]		 = msg.ReadFloat();
	next.i.position[2]		 = msg.ReadFloat();
	quat.x					 = msg.ReadFloat();
	quat.y					 = msg.ReadFloat();
	quat.z					 = msg.ReadFloat();
	next.i.linearMomentum[0] = msg.ReadFloat( RB_MOMENTUM_EXPONENT_BITS, RB_MOMENTUM_MANTISSA_BITS );
	next.i.linearMomentum[1] = msg.ReadFloat( RB_MOMENTUM_EXPONENT_BITS, RB_MOMENTUM_MANTISSA_BITS );
	next.i.linearMomentum[2] = msg.ReadFloat( RB_MOMENTUM_EXPONENT_BITS, RB_MOMENTUM_MANTISSA_BITS );

	next.i.orientation = quat.ToMat3();

	// Make sure to initially set them up. Dont try to interpolate yet.
	if( self->GetNumSnapshotsReceived() <= 1 ) {
		current = next;
	}

	if( clipModel ) {
		clipModel->Link( gameLocal.clip, self, clipModel->GetId(), next.i.position, next.i.orientation );
	}
}
