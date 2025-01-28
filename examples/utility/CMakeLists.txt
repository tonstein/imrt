cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(utility)

add_executable(
   ${PROJECT_NAME}
   src/main.cpp
   src/dsp.cpp
   src/gui.cpp
)

target_link_libraries(${PROJECT_NAME}
   PUBLIC
   imrt
)