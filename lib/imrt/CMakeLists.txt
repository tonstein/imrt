cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(imrt LANGUAGES CXX)

add_library(
   imrt

   STATIC

   ../imgui/imgui.cpp
   ../imgui/imgui.h
   ../imgui/imconfig.h
   ../imgui/imgui_draw.cpp
   ../imgui/imgui_internal.h
   ../imgui/imgui_tables.cpp
   ../imgui/imgui_widgets.cpp

   ../imgui/backends/imgui_impl_glfw.cpp
   ../imgui/backends/imgui_impl_opengl3.cpp

   ../implot/implot.cpp
   ../implot/implot.h
   ../implot/implot_items.cpp

   ../imgui-knobs/imgui-knobs.cpp

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

   ../imgui
   ../imgui/backends
   ../implot

   ../imgui-knobs

   ../choc

   include
)

target_link_libraries(imrt PUBLIC glfw rtaudio OpenGL::GL)

target_compile_definitions(imrt PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
