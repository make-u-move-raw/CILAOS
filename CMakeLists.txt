cmake_minimum_required(VERSION 3.15)
project(MyProject)

# Get source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Main app
add_executable(Cilaos src/main.cpp)