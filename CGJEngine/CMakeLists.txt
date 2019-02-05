cmake_minimum_required(VERSION 3.2.2)
project(CGJEngine CXX)

include_directories(
	headers
	${GLEW_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${OPENVR_INCLUDE_DIR}
	${OPENHMD_INCLUDE_DIR}
	${FreeImage_INCLUDE_DIRS}
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
		Meshes/RectangleMesh.cpp
		Meshes/InfiniteQuadMesh.cpp
    	Cameras/SphereCamera.cpp
	    Cameras/FreeCamera.cpp
    	Cameras/HUDCamera.cpp
        Cameras/Camera.cpp
        Cameras/OpenVRCamera.cpp
        Cameras/OpenHMDCamera.cpp
    	SceneGraph/SceneGraph.cpp
	    SceneGraph/SceneNode.cpp
    	SceneGraph/LightNode.cpp
	    SceneGraph/LightNode.cpp
    	SceneGraph/ParticleEmitterNode.cpp
	    ResourceManager.cpp
    	InputManager.cpp
        Textures/Texture.cpp
        FBOs/FrameBuffer.cpp
	    ResourceFactory.cpp
		Lights/LightsManager.cpp
	    Particles/ParticlePool.cpp
        FBOs/MSFrameBuffer.cpp
        FBOs/ColorTextureFrameBuffer.cpp
		FBOs/DepthTextureFrameBuffer.cpp
		FBOs/DoubleColorMSFrameBuffer.cpp
        FBOs/GFrameBuffer.cpp
        Textures/Noise.cpp)
set(HEADER_FILES
        headers/Cameras/Camera.h
        headers/CGJengine.h
		headers/FBOs/FrameBuffer.h
        headers/glerrors.h
        headers/glm_wrapper.h
		headers/Meshes/H3DMesh.h
		headers/Cameras/HUDCamera.h
        headers/InputManager.h
		headers/SceneGraph/LightNode.h
		headers/Lights/LightsManager.h
		headers/Meshes/Mesh.h
		headers/Meshes/OBJMesh.h
		headers/Meshes/QuadMesh.h
		headers/Meshes/RectangleMesh.h
		headers/Meshes/InfiniteQuadMesh.h
        headers/ResourceManager.h
		headers/SceneGraph/SceneGraph.h
		headers/SceneGraph/SceneNode.h
        headers/Shader.h
        headers/Shader.h
		headers/Cameras/SphereCamera.h
		headers/Cameras/VRCamera.h
        headers/Cameras/OpenVRCamera.h
        headers/Cameras/OpenHMDCamera.h
        headers/Textures/Texture.h
		headers/Particles/ParticlePool.h
		headers/Particles/Particle.h
		headers/SceneGraph/ParticleEmitterNode.h
		headers/FBOs/MSFrameBuffer.h
		headers/FBOs/ColorTextureFrameBuffer.h
		headers/FBOs/DepthTextureFrameBuffer.h
		headers/FBOs/DoubleColorMSFrameBuffer.h
		headers/FBOs/GFrameBuffer.h
        headers/Textures/Noise.h)


#add_library(jpeg_lib STATIC IMPORTED)
#add_library(png_lib STATIC IMPORTED)
#add_library(z_lib STATIC IMPORTED)
#if(MSVC)
#	SET_TARGET_PROPERTIES(jpeg_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libjpeg.lib)
#	SET_TARGET_PROPERTIES(png_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/png.lib)
#	SET_TARGET_PROPERTIES(z_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/zlib.lib)
#else(MSVC)
#	SET_TARGET_PROPERTIES(jpeg_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/liblibjpeg.a)
#	SET_TARGET_PROPERTIES(png_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libpng.a)
#	SET_TARGET_PROPERTIES(z_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libzlib.a)
#endif(MSVC)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
#target_link_libraries(${PROJECT_NAME} jpeg_lib png_lib z_lib)
target_link_libraries(${PROJECT_NAME} jpeg png ${OPENVR_LIBRARIES} ${OPENHMD_LIBRARIES} ${FreeImage_LIBRARIES})
#add_dependencies(${PROJECT_NAME} libjpeg png zlib)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/headers ${PROJECT_SOURCE_DIR}/math/headers
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
