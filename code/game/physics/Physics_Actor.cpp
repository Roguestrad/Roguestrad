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

CLASS_DECLARATION( idPhysics_Base, idPhysics_Actor )
END_CLASS

idPhysics_Actor::idPhysics_Actor()
{
	clipModel = NULL;
	SetClipModelAxis();
	mass			= 100.0f;
	invMass			= 1.0f / mass;
	masterEntity	= NULL;
	masterYaw		= 0.0f;
	masterDeltaYaw	= 0.0f;
	groundEntityPtr = NULL;
}

idPhysics_Actor::~idPhysics_Actor()
{
	if( clipModel ) {
		delete clipModel;
		clipModel = NULL;
	}
}

void idPhysics_Actor::Save( idSaveGame* savefile ) const
{
	savefile->WriteClipModel( clipModel );
	savefile->WriteMat3( clipModelAxis );

	savefile->WriteFloat( mass );
	savefile->WriteFloat( invMass );

	savefile->WriteObject( masterEntity );
	savefile->WriteFloat( masterYaw );
	savefile->WriteFloat( masterDeltaYaw );

	groundEntityPtr.Save( savefile );
}

void idPhysics_Actor::Restore( idRestoreGame* savefile )
{
	savefile->ReadClipModel( clipModel );
	savefile->ReadMat3( clipModelAxis );

	savefile->ReadFloat( mass );
	savefile->ReadFloat( invMass );

	savefile->ReadObject( reinterpret_cast<idClass*&>( masterEntity ) );
	savefile->ReadFloat( masterYaw );
	savefile->ReadFloat( masterDeltaYaw );

	groundEntityPtr.Restore( savefile );
}

void idPhysics_Actor::SetClipModelAxis()
{
	// align clip model to gravity direction
	if( ( gravityNormal[2] == -1.0f ) || ( gravityNormal == vec3_zero ) ) {
		clipModelAxis.Identity();
	} else {
		clipModelAxis[2] = -gravityNormal;
		clipModelAxis[2].NormalVectors( clipModelAxis[0], clipModelAxis[1] );
		clipModelAxis[1] = -clipModelAxis[1];
	}

	if( clipModel ) {
		clipModel->Link( gameLocal.clip, self, 0, clipModel->GetOrigin(), clipModelAxis );
	}
}

const idMat3& idPhysics_Actor::GetGravityAxis() const
{
	return clipModelAxis;
}

float idPhysics_Actor::GetMasterDeltaYaw() const
{
	return masterDeltaYaw;
}

idEntity* idPhysics_Actor::GetGroundEntity() const
{
	return groundEntityPtr.GetEntity();
}

void idPhysics_Actor::SetClipModel( idClipModel* model, const float density, int id, bool freeOld )
{
	assert( self );
	assert( model );				 // a clip model is required
	assert( model->IsTraceModel() ); // and it should be a trace model
	assert( density > 0.0f );		 // density should be valid

	if( clipModel && clipModel != model && freeOld ) {
		delete clipModel;
	}
	clipModel = model;
	clipModel->Link( gameLocal.clip, self, 0, clipModel->GetOrigin(), clipModelAxis );
}

idClipModel* idPhysics_Actor::GetClipModel( int id ) const
{
	return clipModel;
}

int idPhysics_Actor::GetNumClipModels() const
{
	return 1;
}

void idPhysics_Actor::SetMass( float _mass, int id )
{
	assert( _mass > 0.0f );
	mass	= _mass;
	invMass = 1.0f / _mass;
}

float idPhysics_Actor::GetMass( int id ) const
{
	return mass;
}

void idPhysics_Actor::SetContents( int contents, int id )
{
	clipModel->SetContents( contents );
}

int idPhysics_Actor::GetContents( int id ) const
{
	return clipModel->GetContents();
}

const idBounds& idPhysics_Actor::GetBounds( int id ) const
{
	return clipModel->GetBounds();
}

const idBounds& idPhysics_Actor::GetAbsBounds( int id ) const
{
	return clipModel->GetAbsBounds();
}

bool idPhysics_Actor::IsPushable() const
{
	return ( masterEntity == NULL );
}

const idVec3& idPhysics_Actor::GetOrigin( int id ) const
{
	return clipModel->GetOrigin();
}

const idMat3& idPhysics_Actor::GetAxis( int id ) const
{
	return clipModel->GetAxis();
}

void idPhysics_Actor::SetGravity( const idVec3& newGravity )
{
	if( newGravity != gravityVector ) {
		idPhysics_Base::SetGravity( newGravity );
		SetClipModelAxis();
	}
}

void idPhysics_Actor::ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const
{
	if( model ) {
		gameLocal.clip.TranslationModel(
			results, clipModel->GetOrigin(), clipModel->GetOrigin() + translation, clipModel, clipModel->GetAxis(), clipMask, model->Handle(), model->GetOrigin(), model->GetAxis() );
	} else {
		gameLocal.clip.Translation( results, clipModel->GetOrigin(), clipModel->GetOrigin() + translation, clipModel, clipModel->GetAxis(), clipMask, self );
	}
}

void idPhysics_Actor::ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const
{
	if( model ) {
		gameLocal.clip.RotationModel( results, clipModel->GetOrigin(), rotation, clipModel, clipModel->GetAxis(), clipMask, model->Handle(), model->GetOrigin(), model->GetAxis() );
	} else {
		gameLocal.clip.Rotation( results, clipModel->GetOrigin(), rotation, clipModel, clipModel->GetAxis(), clipMask, self );
	}
}

int idPhysics_Actor::ClipContents( const idClipModel* model ) const
{
	if( model ) {
		return gameLocal.clip.ContentsModel( clipModel->GetOrigin(), clipModel, clipModel->GetAxis(), -1, model->Handle(), model->GetOrigin(), model->GetAxis() );
	} else {
		return gameLocal.clip.Contents( clipModel->GetOrigin(), clipModel, clipModel->GetAxis(), -1, NULL );
	}
}

void idPhysics_Actor::DisableClip()
{
	clipModel->Disable();
}

void idPhysics_Actor::EnableClip()
{
	clipModel->Enable();
}

void idPhysics_Actor::UnlinkClip()
{
	clipModel->Unlink();
}

void idPhysics_Actor::LinkClip()
{
	clipModel->Link( gameLocal.clip, self, 0, clipModel->GetOrigin(), clipModel->GetAxis() );
}

bool idPhysics_Actor::EvaluateContacts()
{
	// get all the ground contacts
	ClearContacts();
	AddGroundContacts( clipModel );
	AddContactEntitiesForContacts();

	return ( contacts.Num() != 0 );
}
