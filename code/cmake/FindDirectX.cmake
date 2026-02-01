if(WIN32)

	set(_DXSDK_DIR $ENV{DXSDK_DIR})
	set(_WINSDK_DIR $ENV{WindowsSdkDir})
	set(_WINSDK_VER $ENV{WindowsSdkVersion})

	if(NOT _WINSDK_DIR)
		set(_WINSDK_DIR "C:/Program Files (x86)/Windows Kits/10/")
	endif()

	if(NOT _WINSDK_VER AND DEFINED CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
		set(_WINSDK_VER ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
	endif()

	if(_WINSDK_VER)
		string(REGEX REPLACE "[/\\\\]$" "" _WINSDK_VER "${_WINSDK_VER}")
	endif()

	if(_WINSDK_DIR AND NOT _WINSDK_VER)
		file(GLOB _WINSDK_VER_LIST RELATIVE "${_WINSDK_DIR}Include" "${_WINSDK_DIR}Include/*")
		if(_WINSDK_VER_LIST)
			list(SORT _WINSDK_VER_LIST)
			list(REVERSE _WINSDK_VER_LIST)
			list(GET _WINSDK_VER_LIST 0 _WINSDK_VER)
		endif()
	endif()

	if(_DXSDK_DIR)

		find_path(DirectX_INCLUDE_DIR NAMES d3d9.h
			HINTS
			${_DXSDK_DIR}
			PATH_SUFFIXES "include"
			)

		if(CMAKE_CL_64)
			set(DirectX_LIBPATH_SUFFIX "lib/x64")
		else(CMAKE_CL_64)
			set(DirectX_LIBPATH_SUFFIX "lib/x86")
		endif(CMAKE_CL_64)

		# dsound dxguid DxErr
		find_library(DirectX_DINPUT8_LIBRARY NAMES dinput8 HINTS ${_DXSDK_DIR} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_DSOUND_LIBRARY NAMES dsound HINTS ${_DXSDK_DIR} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_DXGUID_LIBRARY NAMES dxguid HINTS ${_DXSDK_DIR} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		#find_library(DirectX_DXERR_LIBRARY NAMES dxerr HINTS ${_DXSDK_DIR} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_XINPUT_LIBRARY NAMES Xinput HINTS ${_DXSDK_DIR} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_X3DAUDIO_LIBRARY NAMES x3daudio HINTS ${_DXSDK_DIR} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})

	else()

		set(_WINSDK_INCLUDE_HINTS "")
		set(_WINSDK_LIB_HINTS "")

		if(_WINSDK_DIR AND _WINSDK_VER)
			list(APPEND _WINSDK_INCLUDE_HINTS
				"${_WINSDK_DIR}Include/${_WINSDK_VER}/um"
				"${_WINSDK_DIR}Include/${_WINSDK_VER}/shared")
		endif()

		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(_WINSDK_LIB_ARCH "x64")
		else()
			set(_WINSDK_LIB_ARCH "x86")
		endif()

		if(_WINSDK_DIR AND _WINSDK_VER)
			list(APPEND _WINSDK_LIB_HINTS
				"${_WINSDK_DIR}Lib/${_WINSDK_VER}/um/${_WINSDK_LIB_ARCH}")
		endif()

		find_path(DirectX_INCLUDE_DIR NAMES d3d9.h HINTS ${_WINSDK_INCLUDE_HINTS})
		find_library(DirectX_DINPUT8_LIBRARY NAMES dinput8 HINTS ${_WINSDK_LIB_HINTS})
		find_library(DirectX_DSOUND_LIBRARY NAMES dsound HINTS ${_WINSDK_LIB_HINTS})
		find_library(DirectX_DXGUID_LIBRARY NAMES dxguid HINTS ${_WINSDK_LIB_HINTS})
		#find_library(DirectX_DXERR_LIBRARY NAMES dxerr HINTS ${_WINSDK_LIB_HINTS})
		find_library(DirectX_XINPUT_LIBRARY NAMES Xinput HINTS ${_WINSDK_LIB_HINTS})
		find_library(DirectX_X3DAUDIO_LIBRARY NAMES x3daudio HINTS ${_WINSDK_LIB_HINTS})

	endif()

	set(DirectX_LIBRARIES
		${DirectX_DINPUT8_LIBRARY}
		${DirectX_DSOUND_LIBRARY}
		${DirectX_DXGUID_LIBRARY}
		#${DirectX_DXERR_LIBRARY}
		${DirectX_XINPUT_LIBRARY}
		${DirectX_X3DAUDIO_LIBRARY}
		)

	# handle the QUIETLY and REQUIRED arguments and set DirectX_FOUND to TRUE if
	# all listed variables are TRUE
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(DirectX
		DEFAULT_MSG
		DirectX_INCLUDE_DIR
		DirectX_DINPUT8_LIBRARY
		DirectX_DSOUND_LIBRARY
		DirectX_DXGUID_LIBRARY
		#DirectX_DXERR_LIBRARY
		DirectX_XINPUT_LIBRARY
		)

	mark_as_advanced(DirectX_LIBRARIES DirectX_INCLUDE_DIR)

endif(WIN32)
