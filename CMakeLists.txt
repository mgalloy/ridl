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

if (UNIX)
  if (APPLE)
    set(rIDL_LIBRARY_PATH "DYLD_LIBRARY_PATH")
  else ()
    set(rIDL_LIBRARY_PATH "LD_LIBRARY_PATH")    
  endif ()
endif ()

configure_file("ridl.sh.in" "ridl.sh")

add_custom_command(
  OUTPUT api-docs/index.html
  COMMAND idl ridl_build_docs
  DEPENDS src/*.pro
 )
   
add_custom_target(apidocs DEPENDS api-docs/index.html)

add_custom_command(
  OUTPUT capi-docs/html/index.html
  COMMAND doxygen doxygen.config
  DEPENDS src/*.c
 )
   
add_custom_target(capidocs DEPENDS capi-docs/html/index.html)

add_custom_command(
  OUTPUT docs/manual.pdf
  COMMAND make_docs.sh
  DEPENDS docs/manual.rst
 )
   
add_custom_target(docs DEPENDS docs/manual.pdf)

install(PROGRAMS ridl.sh DESTINATION bin)
install(FILES docs/manual.pdf DESTINATION docs OPTIONAL)
install(FILES docs/ridl.1 DESTINATION man/man1 OPTIONAL)
