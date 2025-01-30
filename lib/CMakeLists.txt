cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(imrt-requirements LANGUAGES CXX)

# RtAudio
set(RTAUDIO_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL "rtaudio-uninstall" CACHE BOOL "" FORCE)
add_subdirectory(rtaudio)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_LIBRARY_TYPE STATIC CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# OpenGL
find_package(OpenGL REQUIRED)

# ImGui, ImGui backends, ImPlot, ImGui-Knobs, CHOC
add_library(
   imrt-requirements

   STATIC

   imgui/imgui.cpp
   imgui/imgui.h
   imgui/imconfig.h
   imgui/imgui_draw.cpp
   imgui/imgui_internal.h
   imgui/imgui_tables.cpp
   imgui/imgui_widgets.cpp

   imgui/backends/imgui_impl_glfw.cpp
   imgui/backends/imgui_impl_opengl3.cpp

   implot/implot.cpp
   implot/implot.h
   implot/implot_items.cpp

   imgui-knobs/imgui-knobs.cpp
)

target_include_directories(
   imrt-requirements

   PUBLIC

   imgui
   imgui/backends
   implot

   imgui-knobs

   choc
)

target_link_libraries(imrt-requirements PUBLIC glfw rtaudio OpenGL::GL)

target_compile_definitions(imrt-requirements PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

add_subdirectory(imrt)
