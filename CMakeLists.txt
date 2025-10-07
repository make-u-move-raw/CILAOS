cmake_minimum_required(VERSION 3.31)
set(PROJECT_NAME Cilaos)

project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##########################################################################################
# Add dependencies with FetchContent
##########################################################################################
include(FetchContent)
function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
    # Deactivate warnings for externals dependices
    if(MSVC)
        target_compile_options(${libName} PRIVATE "/W0")
    else()
        target_compile_options(${libName} PRIVATE "-w")
    endif()
endfunction()

####################################
# Fetch raylib for graphics
####################################
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
add_git_dependency(
    raylib
    https://github.com/raysan5/raylib.git
    5.5
)

# specific configfor raygui
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG        4.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

####################################
# Doxygen for documentation
####################################
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

####################################
# CTest for testing
####################################
# Get test files
file(GLOB TEST_FILES tests/*.cpp)
if(TEST_FILES)
    add_executable(tests ${TEST_FILES})
    target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(tests PRIVATE ex_lib)
    
    enable_testing()
    add_test(NAME my_tests COMMAND tests)
    include(CTest)
endif()

####################################
# Source file & library linking
####################################

# Sources
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/")

# Get source files
set(SRC_FILES src/main.cpp)

# Get custom library files
file(GLOB CUSTOM_LIB_SRC src/my_lib/*.cpp)
file(GLOB CUSTOM_LIB_HDR include/my_lib/*.h)

# Add custom library
add_library(ex_lib ${CUSTOM_LIB_SRC} ${CUSTOM_LIB_HDR})
target_include_directories(ex_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main app
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Link libraries (external & custom)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ex_lib
    raylib
)

# Configuration base on the os
if(APPLE)
    # macOS - Frameworks required
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        "-framework IOKit"
        "-framework Cocoa" 
        "-framework OpenGL"
    )
elseif(WIN32)
    # Added Windows dependencies
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        opengl32
        gdi32
        winmm
    )
    
else()
    # Linux - commons libs
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        X11
        GL
        m
        pthread
        dl
    )
endif()