cmake_minimum_required(VERSION 4.0)

project(gl_playground)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

file(GLOB SRC_FILES src/*.c)
add_executable(gl_playground ${SRC_FILES} lib/glad/src/gl.c)
target_include_directories(gl_playground PRIVATE include lib/glad/include)
target_link_libraries(gl_playground PRIVATE SDL3::SDL3 m)
