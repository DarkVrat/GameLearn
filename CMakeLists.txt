cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

project(Game)

set(CMAKE_CXX_STANDART 17)
add_executable(Game 
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(Game glfw)

add_subdirectory(external/glad)
target_link_libraries(Game glad)