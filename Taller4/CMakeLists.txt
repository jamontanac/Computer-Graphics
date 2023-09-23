cmake_minimum_required(VERSION 3.10)

## -- Project name
project(Stickman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
## -- Check output vs. source dirs
if("${PROJECT_BINARY_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")
  message(FATAL_ERROR "Source and build dirs should not be the same")
endif()

## -- Output dirs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

## -- Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif()
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(
    CMAKE_BUILD_TYPE "${default_build_type}" CACHE
    STRING "Choose the type of build." FORCE
    )
  # Set the possible values of build type for cmake-gui
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
    )
endif()

## -- CMake directives
include(GenerateExportHeader)

## -- Find required packages
# find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
if(APPLE)
  find_package(OpenGL REQUIRED)
  find_package(GLUT REQUIRED)
else()
  find_package(OpenGL COMPONENTS OpenGL REQUIRED)
  find_package(GLUT REQUIRED)
endif()

## -- Subdirs
add_subdirectory(app)
add_subdirectory(lib)

## eof - CMakeLists.txt
