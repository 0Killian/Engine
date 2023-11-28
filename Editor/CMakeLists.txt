project(Editor)

file(GLOB_RECURSE EDITOR_SRC "src/*.cpp" "inc/*.h")

add_executable(Editor ${EDITOR_SRC})

target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(Editor PUBLIC Lib)