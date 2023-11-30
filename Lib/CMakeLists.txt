project(Lib)

if (WIN32)
    file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "inc/*.h" "public/*.h" "win32/*.cpp")
else ()
    # Not implemented : throw error
    message(FATAL_ERROR "Not implemented")
endif ()

add_library(Lib SHARED ${LIB_SOURCES})

target_include_directories(Lib PRIVATE inc)
target_include_directories(Lib PUBLIC public)

target_compile_definitions(Lib PRIVATE NGN_EXPORT)