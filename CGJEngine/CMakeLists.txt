cmake_minimum_required(VERSION 3.2.2)
project(CGJEngine CXX)

include_directories(
	headers
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
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
	Shader.cpp
	glerror.cpp
	Meshes/OBJMesh.cpp
	Meshes/H3DMesh.cpp
	Meshes/QuadMesh.cpp
	Cameras/SphereCamera.cpp
	Cameras/FreeCamera.cpp
	Cameras/HUDCamera.cpp
    Cameras/Camera.cpp
	SceneGraph/SceneGraph.cpp
	SceneGraph/SceneNode.cpp
	SceneGraph/LightNode.cpp
	SceneGraph/LightNode.cpp
	SceneGraph/ParticleEmitterNode.cpp
	ResourceManager.cpp
	InputManager.cpp
        Texture.cpp
        FBOs/FrameBuffer.cpp
	ResourceFactory.cpp
	LightsManager.cpp
	Particles/ParticlePool.cpp
        FBOs/MSFrameBuffer.cpp headers/MSFrameBuffer.h FBOs/TextureFrameBuffer.cpp headers/TextureFrameBuffer.h)

set(HEADER_FILES
		headers/Camera.h
        headers/CGJengine.h
        headers/FrameBuffer.h
        headers/glerrors.h
        headers/glm_wrapper.h
        headers/H3DMesh.h
        headers/HUDCamera.h
        headers/InputManager.h
        headers/LightNode.h
        headers/LightsManager.h
        headers/Mesh.h
        headers/OBJMesh.h
        headers/QuadMesh.h
        headers/ResourceManager.h
        headers/SceneGraph.h
        headers/SceneNode.h
        headers/Shader.h
        headers/Shader.h
        headers/SphereCamera.h
        headers/Texture.h
		headers/ParticlePool.h
		headers/Particle.h
		headers/ParticleEmitterNode.h
        FBOs/MSFrameBuffer.cpp headers/MSFrameBuffer.h FBOs/TextureFrameBuffer.cpp headers/TextureFrameBuffer.h)


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

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} jpeg_lib png_lib z_lib)
add_dependencies(${PROJECT_NAME} libjpeg png zlib)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/headers ${PROJECT_SOURCE_DIR}/math/headers
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
