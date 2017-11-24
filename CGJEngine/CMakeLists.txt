cmake_minimum_required(VERSION 3.2.2)
project(CGJEngine CXX)

include_directories(
	headers
	math/headers
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
	)
set(PNG_INCLUDE_FILES
    ${PNG_INCLUDE_DIR}/pngconf.h
    ${PNG_INCLUDE_DIR}/pngdebug.h
    ${PNG_INCLUDE_DIR}/png.h
    ${PNG_INCLUDE_DIR}/pnginfo.h
    ${PNG_INCLUDE_DIR}/pngpriv.h
    ${PNG_INCLUDE_DIR}/pngstruct.h
    )
set(SOURCE_FILES
	math/Vec2.cpp
	math/Vec3.cpp
	math/Vec4.cpp
	math/Mat2.cpp
	math/Mat3.cpp
	math/Mat4.cpp
	math/transforms.cpp
	math/quat.cpp
	Shader.cpp
	glerror.cpp
	OBJMesh.cpp
	H3DMesh.cpp
	QuadMesh.cpp
	SphereCamera.cpp
	FreeCamera.cpp
	HUDCamera.cpp
	Camera.cpp
	SceneGraph.cpp
	SceneNode.cpp
    LightNode.cpp
	ResourceManager.cpp
	InputManager.cpp
    Textures.cpp
	FrameBuffer.cpp
	ResourceFactory.cpp
	LightNode.cpp
	LightsManager.cpp
        )

add_library(jpeg_lib STATIC IMPORTED)
add_library(png_lib STATIC IMPORTED)
add_library(z_lib STATIC IMPORTED)
if(MSVC)
	SET_TARGET_PROPERTIES(jpeg_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libjpeg.lib)
	SET_TARGET_PROPERTIES(png_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/png.lib)
	SET_TARGET_PROPERTIES(z_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/zlib.lib)
else(MSVC)
	SET_TARGET_PROPERTIES(jpeg_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/liblibjpeg.a)
	SET_TARGET_PROPERTIES(png_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libpng.a)
	SET_TARGET_PROPERTIES(z_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libzlib.a)
endif(MSVC)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} jpeg_lib png_lib z_lib)
add_dependencies(${PROJECT_NAME} libjpeg png zlib)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/headers ${PROJECT_SOURCE_DIR}/math/headers
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
