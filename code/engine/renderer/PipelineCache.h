/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
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
#ifndef RENDERER_PIPELINECACHE_H_
#define RENDERER_PIPELINECACHE_H_

struct PipelineKey {
	uint64		 state;
	int			 program;
	int			 depthBias;
	float		 slopeBias;

	Framebuffer* framebuffer;
};

//! Compares two PipelineKey objects for equality based on their state, program, framebuffer, depthBias, and slopeBias members.
inline bool operator==( const PipelineKey& lhs, const PipelineKey& rhs )
{
	return lhs.state == rhs.state && lhs.program == rhs.program && lhs.framebuffer == rhs.framebuffer && lhs.depthBias == rhs.depthBias && lhs.slopeBias == rhs.slopeBias;
}

//! Computes a hash value for a PipelineKey object using combined hash values of its components.
template<>
struct std::hash<PipelineKey> {
	std::size_t operator()( const PipelineKey& key ) const noexcept
	{
		std::size_t h = 0;
		nvrhi::hash_combine( h, key.state );
		nvrhi::hash_combine( h, key.program );
		nvrhi::hash_combine( h, key.framebuffer );
		nvrhi::hash_combine( h, key.depthBias );
		nvrhi::hash_combine( h, key.slopeBias );
		return h;
	}
};

/*!
	\class PipelineCache
	\brief Manages caching and creation of graphics pipelines for efficient rendering.

	The PipelineCache class provides a mechanism for storing and retrieving graphics pipeline objects to avoid redundant creation during rendering. It initializes with a device handle and maintains a
   collection of cached pipelines indexed by pipeline keys. The class supports creating new pipelines on demand, clearing the cache, and configuring render state objects based on state bits and
   pipeline keys. It also handles conversion of stencil operation bits into depth-stencil state descriptions for proper rendering configuration. The cache is designed to be cleared or invalidated
   during shutdown to ensure proper resource cleanup.

*/
class PipelineCache
{
public:
	//! Constructs a new PipelineCache instance.
	PipelineCache();

	//! Initializes the pipeline cache with the provided device handle.
	void						  Init( nvrhi::DeviceHandle deviceHandle );

	//! Invalidates the device reference to ensure proper cleanup during shutdown
	void						  Shutdown();

	//! Clears all cached pipelines and their hash entries.
	void						  Clear();

	//! Returns or creates a graphics pipeline handle based on the provided pipeline key
	nvrhi::GraphicsPipelineHandle GetOrCreatePipeline( const PipelineKey& key );

private:
	//! Configures a render state object based on state bits and a pipeline key
	void														  GetRenderState( uint64 stateBits, PipelineKey key, nvrhi::RenderState& renderState );

	//! Converts stencil operation bits into a depth-stencil state stencil operation description.
	nvrhi::DepthStencilState::StencilOpDesc						  GetStencilOpState( uint64 stateBits );

	nvrhi::DeviceHandle											  device;
	idHashIndex													  pipelineHash;
	idList<std::pair<PipelineKey, nvrhi::GraphicsPipelineHandle>> pipelines;
};

#endif
