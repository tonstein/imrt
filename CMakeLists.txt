cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(imrt-template LANGUAGES CXX)

add_subdirectory(lib/imrt)

option(IMRT_BUILD_EXAMPLES "Build examples")
set(IMRT_BUILD_EXAMPLES ON)

if(IMRT_BUILD_EXAMPLES)
   add_subdirectory(examples/utility)
   add_subdirectory(examples/oscilloscope)
endif()
