cmake_minimum_required(VERSION 4.0)

project(gl_playground)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL_image EXCLUDE_FROM_ALL)

file(GLOB SRC_FILES src/*.c)

add_compile_options(-g -Wall -Wextra)
add_executable(gl_playground WIN32 src/camera.c src/game.c src/grid.c src/main.c src/matrix.c src/shader.c src/vector.c src/window.c src/mesh.c src/mesh_data.c lib/glad/src/gl.c)
target_include_directories(gl_playground PRIVATE include lib/glad/include)
target_link_libraries(gl_playground PRIVATE SDL3_image::SDL3_image SDL3::SDL3)

if(NOT WIN32)
    target_link_libraries(gl_playground PRIVATE m)
endif()

add_custom_command(TARGET gl_playground POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:gl_playground>/res/)
