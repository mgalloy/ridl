# not really sure what the actual required version is, just using the same as 
# CMake tutorial
cmake_minimum_required(VERSION 2.6)

project(rIDL)

# needed to use Subversion package
cmake_policy(SET CMP0017 NEW)

set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/CMake/Modules 
    ${CMAKE_ROOT}/Modules 
    ${CMAKE_MODULE_PATH}
    )
INCLUDE(${CMAKE_SOURCE_DIR}/CMake/GetDate.cmake)
    
find_package(IDL REQUIRED)
find_package(Readline REQUIRED)

set(rIDL_VERSION_MAJOR 0)
set(rIDL_VERSION_MINOR 1)

message(STATUS "Readline_INCLUDE_DIR: ${Readline_INCLUDE_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/src" 
                    "${IDL_INCLUDE_DIR}"
                    "${Readline_INCLUDE_DIR}"
                    #"/usr/local/Cellar/readline/6.1/include"
                    )

add_subdirectory(src)