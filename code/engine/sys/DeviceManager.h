/*
 * Copyright (c) 2014-2021, NVIDIA CORPORATION. All rights reserved.
 * Copyright (C) 2022 Stephen Pridham (id Tech 4x integration)
 * Copyright (C) 2023 Stephen Saunders (id Tech 4x integration)
 * Copyright (C) 2023 Robert Beckebans (id Tech 4x integration)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef SYS_DEVICE_MANAGER_H_
#define SYS_DEVICE_MANAGER_H_

#if USE_DX11 || USE_DX12
	#include <DXGI.h>
	#include <dxgi1_6.h>
#endif

#if USE_DX11
	#include <d3d11.h>
#endif

#if USE_DX12
	#include <d3d12.h>
#endif

#if USE_VK
	#include <nvrhi/vulkan.h>
#endif

struct DeviceCreationParameters {
	bool		  startMaximized			 = false;
	bool		  startFullscreen			 = false;
	bool		  allowModeSwitch			 = false;
	int			  windowPosX				 = -1; // -1 means use default placement
	int			  windowPosY				 = -1;
	uint32_t	  backBufferWidth			 = 1280;
	uint32_t	  backBufferHeight			 = 720;
	uint32_t	  backBufferSampleCount		 = 1; // optional HDR Framebuffer MSAA
	uint32_t	  refreshRate				 = 0;
	uint32_t	  swapChainBufferCount		 = NUM_FRAME_DATA;			   // SRS - default matches GPU frames, can be overridden by renderer
	nvrhi::Format swapChainFormat			 = nvrhi::Format::RGBA8_UNORM; // RB: don't do the sRGB gamma ramp with the swapchain
	uint32_t	  swapChainSampleCount		 = 1;
	uint32_t	  swapChainSampleQuality	 = 0;
	bool		  enableDebugRuntime		 = false;
	bool		  enableNvrhiValidationLayer = false;
	int			  vsyncEnabled				 = 0;
	bool		  enableRayTracingExtensions = false; // for vulkan
	bool		  enableComputeQueue		 = false;
	bool		  enableCopyQueue			 = false;

#if _WIN32
	HINSTANCE hInstance;
	HWND	  hWnd;
#endif

#if USE_DX11 || USE_DX12
	// Adapter to create the device on. Setting this to non-null overrides adapterNameSubstring.
	// If device creation fails on the specified adapter, it will *not* try any other adapters.
	IDXGIAdapter* adapter = nullptr;
#endif

	// For use in the case of multiple adapters; only effective if 'adapter' is null. If this is non-null, device creation will try to match
	// the given string against an adapter name.  If the specified string exists as a sub-string of the
	// adapter name, the device and window will be created on that adapter.  Case sensitive.
	std::wstring adapterNameSubstring = L"";

	// set to true to enable DPI scale factors to be computed per monitor
	// this will keep the on-screen window size in pixels constant
	//
	// if set to false, the DPI scale factors will be constant but the system
	// may scale the contents of the window based on DPI
	//
	// note that the backbuffer size is never updated automatically; if the app
	// wishes to scale up rendering based on DPI, then it must set this to true
	// and respond to DPI scale factor changes by resizing the backbuffer explicitly
	bool		 enablePerMonitorDPI = false;

#if USE_DX11 || USE_DX12
	DXGI_USAGE		  swapChainUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	D3D_FEATURE_LEVEL featureLevel	 = D3D_FEATURE_LEVEL_11_1;
#endif

#if USE_VK
	std::vector<std::string> requiredVulkanInstanceExtensions;
	std::vector<std::string> requiredVulkanDeviceExtensions;
	std::vector<std::string> requiredVulkanLayers;
	std::vector<std::string> optionalVulkanInstanceExtensions;
	std::vector<std::string> optionalVulkanDeviceExtensions;
	std::vector<std::string> optionalVulkanLayers;
	std::vector<size_t>		 ignoredVulkanValidationMessageLocations;
#endif

	// SRS - Used by idImage::AllocImage() to determine if format D24S8 is supported by device (default = true)
	bool enableImageFormatD24S8 = true;
};

struct DefaultMessageCallback : public nvrhi::IMessageCallback {
	//! Returns the singleton instance of the DefaultMessageCallback class.
	static DefaultMessageCallback& GetInstance();

	//! Handles messages from nvrhi by routing them to appropriate logging functions based on severity.
	void						   message( nvrhi::MessageSeverity severity, const char* messageText ) override;
};

class idRenderBackend;

/// Initialize the window and graphics device.
class DeviceManager
{
public:
	//! Creates a DeviceManager instance for the specified graphics API
	static DeviceManager* Create( nvrhi::GraphicsAPI api );

#if USE_VK && defined( VULKAN_USE_PLATFORM_SDL )
	// SRS - Helper method for creating SDL Vulkan surface within DeviceManager_VK()
	VkResult CreateSDLWindowSurface( VkInstance instance, VkSurfaceKHR* surface );
#endif

	//! Creates an SDL window with Vulkan support and initializes the graphics device and swap chain
	bool CreateWindowDeviceAndSwapChain( const glimpParms_t& params, const char* windowTitle );

	//! Returns the width and height of the window in screen coordinates.
	void GetWindowDimensions( int& width, int& height );

	//! Returns the screen coordinate to pixel coordinate scale factors for X and Y axes.
	void GetDPIScaleInfo( float& x, float& y ) const
	{
		x = m_DPIScaleFactorX;
		y = m_DPIScaleFactorY;
	}

	//! Updates the window size and handles back buffer resizing when window dimensions change.
	void		UpdateWindowSize( const glimpParms_t& params );

	//! Returns the Vulkan queue family index for graphics operations
	virtual int GetGraphicsFamilyIndex() const { return -1; }

protected:
	friend class idRenderBackend;
	friend class idImage;

	void*					 windowInstance;
	void*					 windowHandle;
	bool					 m_windowVisible = false;

	DeviceCreationParameters m_DeviceParams;

	float					 m_DPIScaleFactorX = 1.f;
	float					 m_DPIScaleFactorY = 1.f;
	int						 m_RequestedVSync  = 0;

	uint32_t				 m_FrameIndex = 0;

	DeviceManager() = default;

	//! Returns the graphics vendor type based on the provided vendor ID
	graphicsVendor_t getGPUVendor( uint32_t vendorID ) const;

	//! Shuts down the framebuffer when the back buffer is resizing.
	void			 BackBufferResizing();

	//! Notifies the device manager that the back buffer has been resized and updates framebuffers accordingly.
	void			 BackBufferResized();

	// device-specific methods
	virtual bool	 CreateDeviceAndSwapChain()	 = 0;
	virtual void	 DestroyDeviceAndSwapChain() = 0;
	virtual void	 ResizeSwapChain()			 = 0;
	virtual void	 BeginFrame()				 = 0;
	virtual void	 EndFrame()					 = 0; // RB: added for BFG edition
	virtual void	 Present()					 = 0;

public:
	[[nodiscard]] virtual nvrhi::IDevice*	 GetDevice() const		   = 0;
	[[nodiscard]] virtual const char*		 GetRendererString() const = 0;
	[[nodiscard]] virtual nvrhi::GraphicsAPI GetGraphicsAPI() const	   = 0;

	//! Returns the device creation parameters associated with the device manager.
	const DeviceCreationParameters&			 GetDeviceParams();

	//! Sets the vertical synchronization mode for the device manager.
	virtual void							 SetVsyncEnabled( int vsyncMode ) { m_RequestedVSync = vsyncMode; }

	//! Reports live objects managed by the device manager.
	virtual void							 ReportLiveObjects() { }

	//! Returns the current frame index used for temporal anti-aliasing.
	[[nodiscard]] uint32_t					 GetFrameIndex() const { return m_FrameIndex; }

	virtual nvrhi::ITexture*				 GetCurrentBackBuffer()			 = 0;
	virtual nvrhi::ITexture*				 GetBackBuffer( uint32_t index ) = 0;
	virtual uint32_t						 GetCurrentBackBufferIndex()	 = 0;
	virtual uint32_t						 GetBackBufferCount()			 = 0;

	//! Returns the current framebuffer for rendering.
	nvrhi::IFramebuffer*					 GetCurrentFramebuffer();

	//! Returns the framebuffer object at the specified index or null if the index is out of bounds.
	nvrhi::IFramebuffer*					 GetFramebuffer( uint32_t index );

	//! Shuts down the device manager by destroying the device and swap chain
	void									 Shutdown();
	virtual ~DeviceManager() = default;

	//! Sets the window title for the device manager's window.
	void		 SetWindowTitle( const char* title );

	//! Checks if a Vulkan instance extension is enabled by name
	virtual bool IsVulkanInstanceExtensionEnabled( const char* extensionName ) const { return false; }

	//! Checks if a Vulkan device extension is enabled by name.
	virtual bool IsVulkanDeviceExtensionEnabled( const char* extensionName ) const { return false; }

	//! Checks if a Vulkan layer is enabled by name
	virtual bool IsVulkanLayerEnabled( const char* layerName ) const { return false; }

	//! This function is intended to populate a vector with the names of Vulkan instance extensions that are enabled for the device manager.
	virtual void GetEnabledVulkanInstanceExtensions( std::vector<std::string>& extensions ) const { }

	//! Retrieves the list of enabled Vulkan device extensions.
	virtual void GetEnabledVulkanDeviceExtensions( std::vector<std::string>& extensions ) const { }

	//! Retrieves the list of enabled Vulkan layers into the provided vector.
	virtual void GetEnabledVulkanLayers( std::vector<std::string>& layers ) const { }

private:
	//! Creates and returns a new D3D11 device manager instance.
	static DeviceManager* CreateD3D11();

	//! Creates and returns a new instance of DeviceManager_DX12.
	static DeviceManager* CreateD3D12();

	//! Creates and returns a new instance of DeviceManager_VK.
	static DeviceManager* CreateVK();

	std::string			  m_WindowTitle;
};

#endif
