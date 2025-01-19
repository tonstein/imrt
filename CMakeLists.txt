cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_LIBRARY_TYPE STATIC CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)

set(RTAUDIO_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL "rtaudio-uninstall" CACHE BOOL "" FORCE)

add_subdirectory(lib/rtaudio)

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

   include/imrt.h
)

target_include_directories(imrt PUBLIC include lib/imgui lib/glfw lib/implot lib/rtaudio)

target_link_libraries(imrt PRIVATE glfw OpenGL::GL rtaudio)
