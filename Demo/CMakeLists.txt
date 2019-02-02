cmake_minimum_required(VERSION 3.2.2)
project(CGJMDemoApp CXX)

include_directories(
	includes/
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
	${OPENVR_INCLUDE_DIR}
	${OPENHMD_INCLUDE_DIR}
   	${CGJEngine_INCLUDE_DIRS})


set(SOURCE_FILES
		main.cpp
		scene.cpp
		scene.h
		actions.cpp
		actions.h
		meshes.cpp
		meshes.h
		shaders.cpp
		shaders.h
		constants.h
        movement.cpp
        movement.h pipeline.cpp pipeline.h)

add_executable(demo ${SOURCE_FILES})
target_link_libraries(demo ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES}
                           ${OPENVR_LIBRARIES} ${OPENHMD_LIBRARIES} CGJEngine)
add_dependencies(demo InstallFreeGLUT CGJEngine)

add_custom_command(TARGET demo POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:demo>/res)
