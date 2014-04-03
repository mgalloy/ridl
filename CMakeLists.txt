# required for setting policy CMP0017 (for using FindSubversion.cmake)
cmake_minimum_required(VERSION 2.8.4)

project(rIDL)

set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/mgcmake/Modules
  ${CMAKE_ROOT}/Modules
  ${CMAKE_MODULE_PATH}
)
include(${CMAKE_SOURCE_DIR}/mgcmake/GetDate.cmake)

# 32- or 64-bit build (in bytes)
if (NOT rIDL_SIZEOF_VOID_P)
  set(rIDL_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
endif ()

math(EXPR rIDL_SIZEOF_VOID_P_BITS "8 * ${rIDL_SIZEOF_VOID_P}")
message(STATUS "Pointer size: ${rIDL_SIZEOF_VOID_P_BITS}-bit build")
message(STATUS "Install location: ${CMAKE_INSTALL_PREFIX}")

# used in the launch script, if this is a 32-bit build then launch with -32
# option
if (rIDL_SIZEOF_VOID_P_BITS STREQUAL "32")
  set(rIDL_PREFER_32 "1")
else ()
  set(rIDL_PREFER_32 "0")
endif ()

find_package(IDL REQUIRED "${rIDL_SIZEOF_VOID_P}")
find_package(Readline 6.0 REQUIRED)

set(rIDL_VERSION_MAJOR 0)
set(rIDL_VERSION_MINOR 2)
set(rIDL_VERSION_PATCH 0)
set(VERSION "${rIDL_VERSION_MAJOR}.${rIDL_VERSION_MINOR}.${rIDL_VERSION_PATCH}")

find_package(Git)
if (GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
    OUTPUT_VARIABLE REVISION
  )
else ()
  set(REVISION "")
endif ()

set(CMAKE_BUILD_TYPE "Distribution")
set(CMAKE_C_FLAGS_DISTRIBUTION "-DNDEBUG")

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
  OUTPUT online_help
  COMMAND "${CMAKE_SOURCE_DIR}/idlhelp2txt.py" "${IDL_ROOT_DIR}/help/online_help/IDL/Content/Reference Material" online_help
  DEPENDS "${IDL_ROOT_DIR}/help/online_help/IDL/Content/Reference Material"
)
add_custom_target(onlinehelp DEPENDS online_help)

add_custom_command(
  OUTPUT api-docs/index.html
  COMMAND ${Idl_EXECUTABLE} ridl_build_docs
  DEPENDS src/*.pro
)
add_custom_target(apidocs DEPENDS api-docs/index.html)

add_custom_command(
  OUTPUT capi-docs/html/index.html
  COMMAND doxygen doxygen.config
  DEPENDS src/*.c
)
add_custom_target(capidocs DEPENDS capi-docs/html/index.html)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "api-docs;capi-docs")

add_custom_command(
  OUTPUT docs/ridl_manual.pdf
  COMMAND make_docs.sh
  DEPENDS docs/ridl_manual.rst docs/ridl.rst
)
add_custom_target(docs DEPENDS docs/ridl_manual.pdf)


# setup installer

if (WIN32)
  set(CPACK_GENERATOR ZIP)
else ()
  set(CPACK_GENERATOR TGZ)
endif ()

string(TOLOWER "${CMAKE_SYSTEM_NAME}" rIDL_SYSTEM_NAME)
string(TOLOWER "${CMAKE_PROJECT_NAME}" rIDL_PACKAGE_NAME)
set(rIDL_PACKAGE_VERSION "${VERSION}.${REVISION}")
set(CPACK_PACKAGE_FILE_NAME "${rIDL_PACKAGE_NAME}-${rIDL_PACKAGE_VERSION}-${rIDL_SYSTEM_NAME}")

include(CPack)

install(FILES
          COPYING.rst
          README.rst
          RELEASE.rst
        DESTINATION .)
install(CODE "MESSAGE(STATUS \"Checking/updating rIDL preferences configuration file\")")
install(CODE "execute_process(COMMAND idl -IDL_PATH \"${CMAKE_SOURCE_DIR}:<IDL_DEFAULT>\" -IDL_STARTUP \"\" -quiet ridl_make_appdir)")
  
install(PROGRAMS ${CMAKE_BINARY_DIR}/ridl.sh DESTINATION bin)
install(FILES docs/ridl_manual.pdf DESTINATION docs OPTIONAL)
install(FILES docs/ridl.1 DESTINATION share/man/man1 OPTIONAL)
install(DIRECTORY ${CMAKE_BINARY_DIR}/online_help/
        DESTINATION online_help
        OPTIONAL)

