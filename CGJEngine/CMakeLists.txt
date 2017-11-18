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
	SphereCamera.cpp
	FreeCamera.cpp
	Camera.cpp
	SceneGraph.cpp
	ResourceManager.cpp
	InputManager.cpp
    Textures.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
#add_executable(scene_viewer ${SOURCE_FILES} ${TEST_FILES})
#target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
#add_dependencies(${PROJECT_NAME} InstallFreeGLUT)

#add_custom_command(TARGET scene_viewer POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#		   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:shaderTest>/res)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/headers ${PROJECT_SOURCE_DIR}/math/headers
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
