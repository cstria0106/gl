cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

project(gl)

if (APPLE)
    LINK_DIRECTORIES(/usr/local/lib)
endif ()

find_package(PkgConfig)
pkg_check_modules(GLFW glfw3 REQUIRED)
pkg_check_modules(GLEW glew REQUIRED)

add_executable(gl main.cpp file.cpp file.h shader.cpp shader.h number.h)

if (APPLE)
    target_link_libraries(gl "-framework OpenGL")
endif ()

include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
target_link_libraries(gl ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})