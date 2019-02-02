cmake_minimum_required(VERSION 3.2.2)
project(CGJDemo C CXX)

if( MSVC )
	list(APPEND CMAKE_PREFIX_PATH
		${CGJDemo_SOURCE_DIR}/dependencies/glew-2.1.0/ 
		${CGJDemo_SOURCE_DIR}/dependencies/freeglut/ )
endif (MSVC)

list(APPEND CMAKE_MODULE_PATH
	${CGJDemo_SOURCE_DIR}/CGJEngine/cmake/ ) 

set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm QUIET)

find_package(FreeImage)
if (FreeImage_FOUND)
	add_definitions(-DFREEIMG)
else(FreeImage_FOUND)
	find_package(JPEG REQUIRED)
	find_package(PNG REQUIRED)
endif (FreeImage_FOUND)

if(glm_FOUND)
	message(STATUS "Using system GLM")
	message(STATUS "GLM : " ${GLM_INCLUDE_DIRS})
else(glm_FOUND)
	message(STATUS "Using bundled GLM")
	set(GLM_INCLUDE_DIRS "${CGJDemo_SOURCE_DIR}/dependencies/glm-0.9.8.5/")
	message(STATUS "GLM : " ${GLM_INCLUDE_DIRS})
endif(glm_FOUND)

## OpenVR API path
set(WINDOWS_PATH_SUFFIXES win${PLATFORM} Win${PLATFORM} x${PLATFORM})
find_library(OPENVR_LIBRARIES
		NAMES
			openvr_api
		PATHS
			${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openvr/bin
			${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openvr/lib
		PATH_SUFFIXES
			osx32
			linux64
			${WINDOWS_PATH_SUFFIXES}
		NO_DEFAULT_PATH
		NO_CMAKE_FIND_ROOT_PATH
		)
set(OPENVR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openvr)

if(OPENVR_LIBRARIES-NOTFOUND)
	message(STATUS "Not using openvr")
else(OPENVR_LIBRARIES-NOTFOUND)
	message(STATUS "Using OpenVR")
    add_definitions(-DOPENVR)
endif(OPENVR_LIBRARIES-NOTFOUND)

find_library(OPENHMD_LIBRARIES NAMES openhmd)
set(OPENHMD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openhmd/include)
if(OPENHMD_LIBRARIES-NOTFOUND)
    message(STATUS "Not using openhmd")
else(OPENHMD_LIBRARIES-NOTFOUND)
    message(STATUS "Using openhmd")
    add_definitions(-DOPENHMD)
endif(OPENHMD_LIBRARIES-NOTFOUND)


if( MSVC )
	if(CMAKE_SIZEOF_VOID_P EQUAL 4) #HACK TO FIGURE OUT WETHER BUILDING x86 or x64
		set(GLEW_LIBRARIES ${CGJDemo_SOURCE_DIR}/dependencies/glew-2.1.0/lib/libx32/glew32s.lib)
		set(GLUT_LIBRARIES ${CGJDemo_SOURCE_DIR}/dependencies/freeglut/lib/freeglut.lib)
	else(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(GLEW_LIBRARIES ${CGJDemo_SOURCE_DIR}/dependencies/glew-2.1.0/lib/lib64/glew32s.lib)
		set(GLUT_LIBRARIES ${CGJDemo_SOURCE_DIR}/dependencies/freeglut/lib/x64/freeglut.lib)
	endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
endif (MSVC)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter -pedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif (UNIX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)

if( MSVC )
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		add_custom_target(InstallFreeGLUT ALL
	               COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${PROJECT_SOURCE_DIR}/dependencies/freeglut/bin/freeglut.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freeglut.dll)
	else(CMAKE_SIZEOF_VOID_P EQUAL 4)
		add_custom_target(InstallFreeGLUT ALL
	               COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${PROJECT_SOURCE_DIR}/dependencies/freeglut/bin/x64/freeglut.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freeglut.dll)
	endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
else(MSVC)
		add_custom_target(InstallFreeGLUT ALL)
endif (MSVC)


#set(ZLIB_INCLUDE_DIRS ${CGJDemo_SOURCE_DIR}/dependencies/zlib
#        CACHE INTERNAL "ZLIB: Include Directories" FORCE)
#set(JPEG_INCLUDE_DIR ${CGJDemo_SOURCE_DIR}/dependencies/libjpeg
#        CACHE INTERNAL "JPEG: Include Directories" FORCE)
#set(PNG_INCLUDE_DIR ${CGJDemo_SOURCE_DIR}/dependencies/libpng
#        CACHE INTERNAL "PNG: Include Directories" FORCE)
#add_subdirectory(dependencies/zlib)
#add_subdirectory(dependencies/libjpeg)
#add_subdirectory(dependencies/libpng)

list(INSERT CMAKE_PREFIX_PATH 0
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_subdirectory(CGJEngine)
add_subdirectory(Demo)
