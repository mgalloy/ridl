# not really sure what the actual required version is, just using the same as 
# CMake tutorial
cmake_minimum_required(VERSION 2.6)

project(rIDL)

set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/CMake/Modules 
    ${CMAKE_ROOT}/Modules 
    ${CMAKE_MODULE_PATH})
find_package(IDL REQUIRED)


set(rIDL_VERSION_MAJOR 0)
set(rIDL_VERSION_MINOR 1)

  
add_subdirectory(src)

# add the binary tree to the search path for include files so that we will
# find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")


