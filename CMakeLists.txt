# not really sure what the actual required version is, just using the same as 
# CMake tutorial
cmake_minimum_required(VERSION 2.6)

project(rIDL)

set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/CMake/Modules 
    ${CMAKE_ROOT}/Modules 
    ${CMAKE_MODULE_PATH}
    )
find_package(IDL REQUIRED)

set(rIDL_VERSION_MAJOR 0)
set(rIDL_VERSION_MINOR 1)

include_directories("${PROJECT_SOURCE_DIR}/src" 
                    "${IDL_INCLUDE_DIR}"
                    #TODO: add Readline includes
                    "/usr/local/Cellar/readline/6.1/include"
                    )

add_subdirectory(src)