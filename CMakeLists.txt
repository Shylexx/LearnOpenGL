cmake_minimum_required(VERSION 3.23)
project(LearnOpenGL VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

set(CMAKE_CXX_STANDARD 20)

set(HEADER_FILES

	# src/LearnOpenGL.h
	src/Shader.h
)

set(SOURCE_FILES

	# src/LearnOpenGL.cpp
	src/main.cpp
)

add_executable(${PROJECT_NAME}
	${HEADER_FILES}
	${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/src
	glfw
	glm
	external/glad/include
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	glfw
	glm
	glad
)

target_compile_definitions(${PROJECT_NAME}
	PUBLIC GLFW_INCLUDE_NONE
)
