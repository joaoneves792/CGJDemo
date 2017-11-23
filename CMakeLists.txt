cmake_minimum_required(VERSION 3.2.2)
project(CGJDemo C CXX)

if( MSVC )
	list(APPEND CMAKE_PREFIX_PATH 
		${CGJDemo_SOURCE_DIR}/dependencies/glew-2.1.0/ 
		${CGJDemo_SOURCE_DIR}/dependencies/freeglut/ )
endif (MSVC)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(JPEG)
find_package(PNG)

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

if(NOT JPEG_FOUND)
	add_subdirectory(dependencies/libjpeg)
	set(JPEG_INCLUDE_DIR dependencies/libjpeg)
endif(NOT JPEG_FOUND)
if(NOT PNG_FOUND)
	add_subdirectory(dependencies/libpng)
	set(PNG_INCLUDE_DIR dependencies/libpng)
endif(NOT PNG_FOUND)
add_subdirectory(CGJEngine) 
add_subdirectory(Demo) 
