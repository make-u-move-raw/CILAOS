cmake_minimum_required(VERSION 3.15)
project(MyProject)

# Get source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add custom libraries and headerfiles
add_library(ex_lib ${SRC_FILES})
target_include_directories(ex_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main app
add_executable(Cilaos src/main.cpp)
# Link libraries to the app
target_link_libraries(Cilaos PRIVATE ex_lib)


# Get test files
file(GLOB TEST_FILES tests/*.cpp)
add_executable(tests ${TEST_FILES})
# Include headers for tests
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE ex_lib)




# Toggle Ctest for testing framework
enable_testing()
add_test(NAME my_tests COMMAND tests)
include(CTest)

