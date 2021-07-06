cmake_minimum_required(VERSION 3.1)

# Project name
project(nacho)

# Build for C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include all library files and source files
file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/src/glad.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Include libraries
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.0.14/include)

# Name of executable
add_executable(nacho ${SOURCE_FILES})

# Link SDL2
target_link_libraries(nacho "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.0.14/lib/x64/SDL2main.lib;${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.0.14/lib/x64/SDL2.lib")

