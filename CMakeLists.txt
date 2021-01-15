cmake_minimum_required(VERSION 3.1.8)
project(GLRender)

# We'll use C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the project global output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds)

# Check the project builds type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
	if(MSVC)
		add_compile_options(/WX /W3)
	endif()	
endif()

# Configure glfw librarie
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXEMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Load 3rdparty CMakeLists.txt file
add_subdirectory(3rdparty/glfw/)
add_subdirectory(3rdparty/glm/)

add_executable(GLRender ${CMAKE_SOURCE_DIR}/GLRender.cpp 3rdparty/glad/src/glad.c)
target_include_directories(GLRender PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/glfw/include/)
target_include_directories(GLRender PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/glm/include/)
target_include_directories(GLRender PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/glad/include/)
target_link_libraries(GLRender glfw glm)
