cmake_minimum_required(VERSION 3.10)

project(Engine)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_STANDARD 23)

# Add subdirectories for each project
add_subdirectory(Lib)
add_subdirectory(Sandbox)
add_subdirectory(Editor)