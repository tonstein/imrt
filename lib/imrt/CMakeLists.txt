cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(imrt LANGUAGES CXX)

add_library(
   imrt

   STATIC

   src/imrt-dsp.h

   src/imrt-gui.h
   assets/imrt-font.embed

   src/imrt-params.cpp
   src/imrt-params.h

   src/imrt-widgets.h
)

target_include_directories(imrt PUBLIC include)

target_link_libraries(imrt PUBLIC imrt-requirements)
