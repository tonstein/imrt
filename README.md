# ImRt

A framework for the developement of audio applications using __Dear ImGui__ with the backends __GLFW__ and __OpenGL__ as well as __ImPlot__ and __RtAudio__.

## Usage

In your terminal:

```bash
mkdir ____ # Use project name.
cd ____ # Use project name.
git init
git submodule add https://github.com/tonstein/imrt.git lib/imrt
git submodule update --init --recursive
touch CMakeLists.txt
code .
```

In your CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(____) # Use project name.

add_subdirectory(lib/imrt)

add_executable(
   ${PROJECT_NAME}

   ____.cpp # Add project files.
)

target_link_libraries(${PROJECT_NAME} PUBLIC imrt)
```