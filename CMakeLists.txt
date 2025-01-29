cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(imrt-template LANGUAGES CXX)

set(RTAUDIO_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL "rtaudio-uninstall" CACHE BOOL "" FORCE)
add_subdirectory(lib/rtaudio)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_LIBRARY_TYPE STATIC CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/imrt)

option(IMRT_BUILD_EXAMPLES "Build examples")
set(IMRT_BUILD_EXAMPLES ON)

if(IMRT_BUILD_EXAMPLES)
   add_subdirectory(examples/utility)
   add_subdirectory(examples/oscilloscope)
endif()
