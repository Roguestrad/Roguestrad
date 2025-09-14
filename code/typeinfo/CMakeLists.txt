
add_definitions(-DTYPEINFOPROJECT -D__DOOM_DLL__)

set(TP_INCLUDES
	TypeInfoGen.h
	)
set(TP_SOURCES
	TypeInfoGen.cpp
	main.cpp
	precompiled.cpp
	)

if(WIN32)
	list(APPEND TP_SOURCES EngineStub.cpp)
else()
list(APPEND TP_SOURCES EngineStubPosix.cpp)
endif()

set(TP_FRAMEWORK_INCLUDES 
	../engine/framework/CmdSystem.h
	../engine/framework/CVarSystem.h
	../engine/framework/File.h
	../engine/framework/File_Manifest.h
	../engine/framework/File_Resource.h
	../engine/framework/File_Zip.h
	../engine/framework/FileSystem.h
	../engine/framework/Licensee.h
	../idlib/precompiled.h
	)
set(TP_FRAMEWORK_SOURCES
	../engine/framework/CmdSystem.cpp
	../engine/framework/CVarSystem.cpp
	../engine/framework/File.cpp
	../engine/framework/File_Manifest.cpp
	../engine/framework/File_Resource.cpp
	../engine/framework/File_Zip.cpp
	../engine/framework/FileSystem.cpp
	)
	

file(GLOB TP_ZLIB_INCLUDES ../libs/zlib/*.h)
file(GLOB TP_ZLIB_SOURCES ../libs/zlib/*.c)
list(APPEND TP_ZLIB_SOURCES ../libs/zlib/minizip/unzip.cpp)
list(APPEND TP_ZLIB_SOURCES ../libs/zlib/minizip/ioapi.c)

set(TP_INCLUDES_ALL
			${TP_INCLUDES}
			${TP_FRAMEWORK_INCLUDES}
			${TP_ZLIB_INCLUDES}
			)
			
set(TP_SOURCES_ALL
			${TP_SOURCES}
			${TP_FRAMEWORK_SOURCES}
			${TP_ZLIB_SOURCES}
			)

source_group("typeinfo" FILES ${TP_INCLUDES})
source_group("typeinfo" FILES ${TP_SOURCES})
source_group("framework" FILES ${TP_FRAMEWORK_INCLUDES})
source_group("framework" FILES ${TP_FRAMEWORK_SOURCES})
source_group("zlib" FILES ${TP_ZLIB_INCLUDES})
source_group("zlib" FILES ${TP_ZLIB_SOURCES})

set(TP_PRECOMPILED_SOURCES ${TP_SOURCES_ALL})
list(REMOVE_ITEM TP_PRECOMPILED_SOURCES ${TP_ZLIB_SOURCES})

include_directories(
	.
	../idlib
	../libs/zlib
	)

if(MSVC)
	list(REMOVE_ITEM TP_PRECOMPILED_SOURCES precompiled.cpp) 

	#foreach( src_file ${TP_PRECOMPILED_SOURCES} )
	#	message(STATUS "-include precompiled.h for ${src_file}")
	#endforeach()

    #set_target_properties(idlib PROPERTIES COMPILE_FLAGS "/Yuprecompiled.h")
    set_source_files_properties(precompiled.cpp
        PROPERTIES
        COMPILE_FLAGS "/Ycprecompiled.h"
        OBJECT_OUTPUTS "precompiled.pch"
        )
		
	set_source_files_properties(
		${TP_PRECOMPILED_SOURCES}
		PROPERTIES
		COMPILE_FLAGS "/Yuprecompiled.h"
		OBJECT_DEPENDS "precompiled.pch"
		)

    add_executable(typeinfogen ${TP_SOURCES_ALL} ${TP_INCLUDES_ALL})
	add_dependencies(typeinfogen idlib)
	target_link_libraries(typeinfogen idlib winmm Shlwapi)
	
else()
	if (USE_PRECOMPILED_HEADERS)
	foreach( src_file ${TP_PRECOMPILED_SOURCES} )
		#message(STATUS "-include precompiled.h for ${src_file}")
		set_source_files_properties(
			${src_file}
			PROPERTIES
			COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/precompiled.h"
			)
	endforeach()
	endif()

	include_directories(.)
	
	if (USE_PRECOMPILED_HEADERS)
	# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
	STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
	SET(_compiler_FLAGS "${${_flags_var_name}} -std=c++${CMAKE_CXX_STANDARD}")
	GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
	FOREACH(item ${_directory_flags})
		LIST(APPEND _compiler_FLAGS " -I${item}")
	ENDFOREACH(item)
	endif()

	GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
	LIST(APPEND _compiler_FLAGS ${_directory_flags})
	
	SEPARATE_ARGUMENTS(_compiler_FLAGS)
	
	if (USE_PRECOMPILED_HEADERS)
	add_custom_target(precomp_header_typeinfo ALL
	                  COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header precompiled.h -o precompiled.h.gch
	                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	                  COMMENT "Creating idlib/precompiled.h.gch for typeinfogen"
	                  )
	endif()
	
	add_executable(typeinfogen ${TP_SOURCES_ALL} ${TP_INCLUDES_ALL})
	add_dependencies(typeinfogen idlib)
	if (USE_PRECOMPILED_HEADERS)
	add_dependencies(typeinfogen precomp_header_typeinfo)
	endif()
	target_link_libraries(typeinfogen idlib)
	
endif()
	

