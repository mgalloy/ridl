# required for setting policy CMP0017 (for using FindSubversion.cmake)
cmake_minimum_required(VERSION 2.8.4)

project(rIDL)

set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/CMake/Modules 
    ${CMAKE_ROOT}/Modules 
    ${CMAKE_MODULE_PATH}
    )
include(${CMAKE_SOURCE_DIR}/CMake/GetDate.cmake)
    
find_package(IDL REQUIRED)
find_package(Readline REQUIRED)

set(rIDL_VERSION_MAJOR 0)
set(rIDL_VERSION_MINOR 1)
set(rIDL_VERSION_PATCH 0)

include_directories("${PROJECT_SOURCE_DIR}/src" 
                    "${IDL_INCLUDE_DIR}"
                    "${Readline_INCLUDE_DIR}"
                     )

add_subdirectory(src)

# TODO: need to generate this file from ridl.sh.in
install(PROGRAMS ridl.sh DESTINATION bin)

# TODO: build/install docs

# TODO: add way to generate API docs