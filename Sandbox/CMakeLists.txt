# Create Sandbox project, depending on the Lib project
project(Sandbox)

file(GLOB_RECURSE SANDBOX_SRC "src/*.cpp" "inc/*.h")

add_executable(Sandbox ${SANDBOX_SRC})

target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(Sandbox PRIVATE Lib)