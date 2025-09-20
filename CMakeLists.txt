cmake_minimum_required(VERSION 3.14)
project(Cilaos)

# External dependencies
include(FetchContent)

set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")   
FetchContent_MakeAvailable(raylib)

# Get source files
set(SRC_FILES src/main.cpp)
# Get custom library files
file(GLOB CUSTOM_LIB_SRC src/my_lib/*.cpp)
file(GLOB CUSTOM_LIB_HDR include/my_lib/*.h include/my_lib/*.hpp)

# Add custom library
add_library(ex_lib ${CUSTOM_LIB_SRC} ${CUSTOM_LIB_HDR})
target_include_directories(ex_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main app
add_executable(Cilaos ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ex_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

########################## Testing ##########################
# Get test files
file(GLOB TEST_FILES tests/*.cpp)
add_executable(tests ${TEST_FILES})

# Include headers for tests
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE ex_lib)

# Enable CTest framework
enable_testing()
add_test(NAME my_tests COMMAND tests)
include(CTest)

########################## Documentation ##########################
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Configure Doxyfile
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(DOC_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs)
    file(MAKE_DIRECTORY ${DOC_OUTPUT_DIR})

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Add custom target for docs
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found. 'doc' target will not be available.")
endif()
