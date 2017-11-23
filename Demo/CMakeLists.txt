cmake_minimum_required(VERSION 3.2.2)
project(CGJMDemoApp CXX)

include_directories(
	includes/
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
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
		ResourceNames.h
		Road.cpp
		Road.h)

add_executable(demo ${SOURCE_FILES})
#target_link_libraries(demo ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} CGJEngine)
target_link_libraries(demo ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} png jpeg CGJEngine)
add_dependencies(demo InstallFreeGLUT)

add_custom_command(TARGET demo POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:demo>/res)
