/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2022 Robert Beckebans
Copyright (C) 2022 Stephen Pridham

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
#include <ShaderMake/ShaderBlob.h>
#pragma hdrstop

#include "../frontend/tr_local.h"
#include <engine/sys/DeviceManager.h>

void idRenderProgManager::StartFrame()
{
}

void idRenderProgManager::BindProgram( int index )
{
	if( currentIndex == index ) {
		return;
	}

	currentIndex = index;
}

void idRenderProgManager::Unbind()
{
	currentIndex = -1;
}

void idRenderProgManager::LoadShader( int index, rpStage_t stage )
{
	if( shaders[index].handle ) {
		return; // Already loaded
	}

	LoadShader( shaders[index] );
}

extern DeviceManager*	   deviceManager;

/*!
	\brief Creates a shader permutation by finding the appropriate binary code and returning a shader handle

	This function searches for a specific shader permutation within a provided binary blob using the given constants and shader description. If found, it creates and returns a shader handle using the
   device. If the permutation is not found and errorIfNotFound is true, it logs an error message. The function takes ownership of the binary data by copying it into the ownedByteCode vector

	\param device The device to create the shader on
	\param d The shader description to use for creation
	\param blob The binary blob containing shader permutations
	\param blobSize The size of the binary blob
	\param constants Array of shader constants used to find the permutation
	\param numConstants Number of constants in the constants array
	\param ownedByteCode Vector to store the copied binary code for ownership
	\param errorIfNotFound Whether to log an error if the permutation is not found
	\return A shader handle for the created shader permutation, or nullptr if not found and errorIfNotFound is false
*/
static nvrhi::ShaderHandle CreateShaderPermutation( nvrhi::IDevice* device,
	const nvrhi::ShaderDesc&										d,
	const void*														blob,
	size_t															blobSize,
	const ShaderMake::ShaderConstant*								constants,
	uint32_t														numConstants,
	std::vector<uint8>&												ownedByteCode,
	bool															errorIfNotFound = true )
{
	// ShaderMake::ShaderBinaryView view = {};

	const void* binary	   = nullptr;
	size_t		binarySize = 0;

	// if( ShaderMake::FindPermutationInBlob( blob, blobSize, constants, numConstants, view ) ) {
	if( ShaderMake::FindPermutationInBlob( blob, blobSize, constants, numConstants, &binary, &binarySize ) ) {
		// Take ownership explicitly by copying
		// std::vector<uint8_t> ownedBinary(
		// ownedByteCode.assign( static_cast<const uint8_t*>( view.data ), static_cast<const uint8_t*>( view.data ) + view.size );
		// return device->createShader( d, ownedByteCode.data(), ownedByteCode.size() );
		return device->createShader( d, binary, binarySize );
	}

	if( errorIfNotFound ) {
		std::string message = ShaderMake::FormatShaderNotFoundMessage( blob, blobSize, constants, numConstants );

		device->getMessageCallback()->message( nvrhi::MessageSeverity::Error, message.c_str() );
	}

	return nullptr;
}

void idRenderProgManager::LoadShader( shader_t& shader )
{
	idStr			  stage;
	nvrhi::ShaderType shaderType {};

	if( shader.stage == SHADER_STAGE_VERTEX ) {
		stage	   = "vs";
		shaderType = nvrhi::ShaderType::Vertex;
	} else if( shader.stage == SHADER_STAGE_FRAGMENT ) {
		stage	   = "ps";
		shaderType = nvrhi::ShaderType::Pixel;
	} else if( shader.stage == SHADER_STAGE_COMPUTE ) {
		stage	   = "cs";
		shaderType = nvrhi::ShaderType::Compute;
	}

	idStr adjustedName = shader.name;
	adjustedName.StripFileExtension();
	if( deviceManager->GetGraphicsAPI() == nvrhi::GraphicsAPI::D3D12 ) {
		adjustedName = idStr( "renderprogs2/dxil/" ) + adjustedName + "." + stage + ".bin";
	} else if( deviceManager->GetGraphicsAPI() == nvrhi::GraphicsAPI::VULKAN ) {
		adjustedName = idStr( "renderprogs2/spirv/" ) + adjustedName + "." + stage + ".bin";
	} else {
		common->FatalError( "Unsupported graphics api" );
	}

	ShaderBlob shaderBlob = GetBytecode( adjustedName );

	if( !shaderBlob.data ) {
		return;
	}

	idList<ShaderMake::ShaderConstant> constants;

	for( int i = 0; i < shader.macros.Num(); i++ ) {
		constants.Append( ShaderMake::ShaderConstant { shader.macros[i].name.c_str(), shader.macros[i].definition.c_str() } );
	}

	nvrhi::ShaderDesc desc = nvrhi::ShaderDesc( shaderType );
	desc.debugName		   = ( idStr( shader.name ) + idStr( shader.nameOutSuffix ) ).c_str();

	nvrhi::ShaderDesc descCopy = desc;
	// TODO(Stephen): Might not want to hard-code this.
	descCopy.entryName = "main";

	ShaderMake::ShaderConstant* shaderConstants = nullptr;
	if( constants.Num() > 0 ) {
		shaderConstants = &constants[0];
	}
	nvrhi::ShaderHandle shaderHandle = CreateShaderPermutation( device, descCopy, shaderBlob.data, shaderBlob.size, shaderConstants, uint32_t( constants.Num() ), shader.ownedBytecode );

	shader.handle = shaderHandle;

	// SRS - Free the shader blob data, otherwise a leak will occur
	Mem_Free( shaderBlob.data );
}

ShaderBlob idRenderProgManager::GetBytecode( const char* fileName )
{
	ShaderBlob blob;

	blob.size = fileSystem->ReadFile( fileName, &blob.data );

	if( !blob.data ) {
		common->FatalError( "Couldn't read the binary file for shader %s", fileName );
	}

	return blob;
}

void idRenderProgManager::LoadProgram( const int programIndex, const int vertexShaderIndex, const int fragmentShaderIndex )
{
	renderProg_t& prog		 = renderProgs[programIndex];
	prog.fragmentShaderIndex = fragmentShaderIndex;
	prog.vertexShaderIndex	 = vertexShaderIndex;
	if( prog.vertexLayout != LAYOUT_UNKNOWN ) {
		prog.inputLayout = device->createInputLayout( &vertexLayoutDescs[prog.vertexLayout][0], vertexLayoutDescs[prog.vertexLayout].Num(), shaders[prog.vertexShaderIndex].handle );
	}
	prog.bindingLayouts = bindingLayouts[prog.bindingLayoutType];
}

void idRenderProgManager::LoadComputeProgram( const int programIndex, const int computeShaderIndex )
{
	renderProg_t& prog		= renderProgs[programIndex];
	prog.computeShaderIndex = computeShaderIndex;
	if( prog.vertexLayout != LAYOUT_UNKNOWN ) {
		prog.inputLayout = device->createInputLayout( &vertexLayoutDescs[prog.vertexLayout][0], vertexLayoutDescs[prog.vertexLayout].Num(), shaders[prog.vertexShaderIndex].handle );
	}
	prog.bindingLayouts = bindingLayouts[prog.bindingLayoutType];
}

int idRenderProgManager::FindProgram( const char* name, int vIndex, int fIndex, bindingLayoutType_t bindingType )
{
	for( int i = 0; i < renderProgs.Num(); ++i ) {
		if( ( renderProgs[i].vertexShaderIndex == vIndex ) && ( renderProgs[i].fragmentShaderIndex == fIndex ) ) {
			return i;
		}
	}

	renderProg_t program;
	program.name			  = name;
	program.vertexLayout	  = LAYOUT_DRAW_VERT;
	program.bindingLayoutType = bindingType;
	int index				  = renderProgs.Append( program );
	LoadProgram( index, vIndex, fIndex );
	return index;
}

int idRenderProgManager::UniformSize()
{
	return uniforms.Allocated();
}

void idRenderProgManager::CommitUniforms( uint64 stateBits )
{
}

void idRenderProgManager::KillAllShaders()
{
	Unbind();

	backEnd.ResetPipelineCache();

	for( int i = 0; i < shaders.Num(); i++ ) {
		if( shaders[i].handle ) {
			shaders[i].handle.Reset();
		}
	}
}

void idRenderProgManager::SetUniformValue( const renderParm_t rp, const float value[4] )
{
	for( int i = 0; i < 4; i++ ) {
		uniforms[rp][i] = value[i];
	}

	uniformsChanged = true;
}

void idRenderProgManager::ZeroUniforms()
{
	memset( uniforms.Ptr(), 0, uniforms.Allocated() );

	uniformsChanged = true;
}

bool idRenderProgManager::CommitConstantBuffer( nvrhi::ICommandList* commandList, bool bindingLayoutTypeChanged )
{
	// RB: It would be better to NUM_BINDING_LAYOUTS uniformsChanged entrys but we don't know the current binding layout type when we set the uniforms.
	// The vkDoom3 backend even didn't bother with this and always fired the uniforms for each draw call.
	if( uniformsChanged || bindingLayoutTypeChanged ) {
		commandList->writeBuffer( constantBuffer /*[BindingLayoutType()]*/, uniforms.Ptr(), uniforms.Allocated() );

		uniformsChanged = false;

		// SRS - Writing to a volatile constant buffer no longer ends the renderpass, so indicate state change only if binding layout type has changed
		//     - for nvrhi Vulkan see related commit https://github.com/NVIDIA-RTX/NVRHI/commit/dafbd407f6fb8b91078da72ca1712dbbd6ac2496
		//     - for nvrhi DX12 a state change has never been required or activated when writing uniforms to volatile constant buffers
		return bindingLayoutTypeChanged;
	}

	return false;
}
