cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(imrt LANGUAGES CXX)

set(RTAUDIO_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL "rtaudio-uninstall" CACHE BOOL "" FORCE)
add_subdirectory(lib/rtaudio)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_LIBRARY_TYPE STATIC CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

find_package(OpenGL REQUIRED)

add_library(
   imrt

   STATIC

   lib/imgui/imgui.cpp
   lib/imgui/imgui.h
   lib/imgui/imconfig.h
   lib/imgui/imgui_draw.cpp
   lib/imgui/imgui_internal.h
   lib/imgui/imgui_tables.cpp
   lib/imgui/imgui_widgets.cpp

   lib/imgui/backends/imgui_impl_glfw.cpp
   lib/imgui/backends/imgui_impl_opengl3.cpp

   lib/implot/implot.cpp
   lib/implot/implot.h
   lib/implot/implot_items.cpp

   lib/imgui-knobs/imgui-knobs.cpp

   src/imrt-dsp.h

   src/imrt-gui.h
   src/imrt-font.embed

   src/imrt-params.cpp
   src/imrt-params.h

   src/imrt-widgets.h
)

target_include_directories(
   imrt

   PUBLIC

   lib/imgui
   lib/imgui/backends
   lib/implot

   lib/imgui-knobs

   lib/choc

   include
)

target_link_libraries(imrt PUBLIC glfw rtaudio OpenGL::GL)

target_compile_definitions(imrt PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

option(IMRT_BUILD_EXAMPLES "Build examples")
set(IMRT_BUILD_EXAMPLES ON)

if(IMRT_BUILD_EXAMPLES)
   add_subdirectory(examples/utility)
   add_subdirectory(examples/oscilloscope)
endif()