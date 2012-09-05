# Exports several useful variables:
#
#   IDL_BIN32_PATH
#      relative path from the root of the IDL distribution to the directory 
#      containing the 32-bit binaries, i.e.::
#
#        bin/bin.linux.x86
#
#   IDL_BIN64_PATH
#      relative path from the root of the IDL distribution to the directory 
#      containing the 64-bit binaries, i.e.::
#
#        bin/bin.linux.x86_64
#
#   IDL_BIN_PATH
#      relative path from the root of the IDL distribution to the directory 
#      containing the binaries to be used in the build, i.e.::
#
#        bin/bin.linux.x86_64
#
#   IDL_BIN_DIR
#      relative path from the bin directory of the IDL distribution to the 
#      directory containing the binaries to be used in the build, i.e.::
#
#        bin.linux.x86_64
#
#   IDL_LIBRARY
#      full path to the IDL shared object file, i.e.::
# 
#        /usr/local/itt/idl/idl80/bin/bin.darwin.x86_64/libidl.dylib
#
#   IDL_LIBRARY_DIR
#      full path to the directory containing the libidl.so file, i.e.::
#
#        /usr/local/itt/idl/idl80/bin/bin.darwin.x86_64
#
#   IDL_INCLUDE_DIR
#      full path to the directory containing idl_export.h, i.e.::
#
#        /usr/local/itt/idl/idl80/external/include
#
#   IDL_DIR
#      full path to the root of an IDL installation, i.e.::
#
#        /usr/local/itt/idl/idl80

# convenience variable for ITT's install dir
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(_IDL_PROGRAM_FILES_DIR "C:/Program Files")
  set(_IDL_NAME "IDL")
  set(_IDL_OS "")
  set(_IDL_KNOWN_COMPANIES "Exelis" "ITT")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(_IDL_PROGRAM_FILES_DIR "/Applications")
  set(_IDL_NAME "idl")
  set(_IDL_OS ".darwin")
  set(_IDL_KNOWN_COMPANIES "exelis" "itt")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(_IDL_PROGRAM_FILES_DIR "/usr/local")
  set(_IDL_NAME "idl")
  set(_IDL_OS ".linux")
  set(_IDL_KNOWN_COMPANIES "exelis" "itt")
endif ()


# find IDL based on version numbers, if you want a specific one, set it prior 
# to running configure
if (NOT DEFINED IDL_FIND_VERSION)
  set(_IDL_KNOWN_VERSIONS "82" "71" "706")
  set(_IDL_KNOWN_VERSIONS_EXTRA_DIR "81" "80")
  # IDL 8.0 and 8.1 is in a different location than other versions
  foreach (_IDL_COMPANY ${_IDL_KNOWN_COMPANIES})
    foreach (_IDL_KNOWN_VERSION ${_IDL_KNOWN_VERSIONS_EXTRA_DIR})
      list(APPEND _IDL_SEARCH_DIRS
           "${_IDL_PROGRAM_FILES_DIR}/${_IDL_COMPANY}/${_IDL_NAME}/${_IDL_NAME}${_IDL_KNOWN_VERSION}")
    endforeach (_IDL_KNOWN_VERSION ${_IDL_KNOWN_VERSIONS})
    foreach (_IDL_KNOWN_VERSION ${_IDL_KNOWN_VERSIONS})
      list(APPEND _IDL_SEARCH_DIRS
           "${_IDL_PROGRAM_FILES_DIR}/${_IDL_COMPANY}/${_IDL_NAME}${_IDL_KNOWN_VERSION}")
    endforeach (_IDL_KNOWN_VERSION ${_IDL_KNOWN_VERSIONS})
  endforeach (_IDL_COMPANY ${_IDL_KNOWN_COMPANIES})
endif ()

# override search path if IDL_DIR environment variable is set
if (NOT "$ENV{IDL_DIR}" STREQUAL "")
  set(_IDL_SEARCH_DIRS "$ENV{IDL_DIR}")
endif ()

find_path(IDL_INCLUDE_DIR
  idl_export.h
  PATHS ${_IDL_SEARCH_DIRS}
  HINTS ${IDL_ROOT}
  PATH_SUFFIXES external/include
)

set(IDL_BIN64_PATH /bin/bin${_IDL_OS}.x86_64)
  if (APPLE)
    set(IDL_BIN32_PATH /bin/bin${_IDL_OS}.i386)
  else ()
    set(IDL_BIN32_PATH /bin/bin${_IDL_OS}.x86)
  endif ()
  
if (IDL_FIND_COMPONENTS EQUAL 8)
  set(IDL_BIN_PATH ${IDL_BIN64_PATH})
elseif (IDL_FIND_COMPONENTS EQUAL 4)
  set(IDL_BIN_PATH ${IDL_BIN32_PATH})
else ()
  math(EXPR IDL_FIND_COMPONENTS_BITS "8 * ${IDL_FIND_COMPONENTS}") 
  message(FATAL_ERROR "${IDL_FIND_COMPONENTS_BITS}-bit build invalid")
endif ()

find_library(IDL_LIBRARY
  NAMES idl
  PATHS ${_IDL_SEARCH_DIRS}
  HINTS ${IDL_ROOT}
  PATH_SUFFIXES ${IDL_BIN_PATH}
  )

if (IDL_INCLUDE_DIR AND IDL_LIBRARY)
   set(IDL_FOUND TRUE)
endif ()

if (IDL_FOUND)
  if (NOT IDL_FIND_QUIETLY)
    message(STATUS "IDL library: ${IDL_LIBRARY}")
    message(STATUS "IDL include: ${IDL_INCLUDE_DIR}")
  endif ()

  set(HAVE_IDL 1 CACHE BOOL "Whether have IDL")
  get_filename_component(IDL_LIBRARY_DIR ${IDL_LIBRARY} PATH)
  get_filename_component(IDL_BIN_DIR ${IDL_LIBRARY_DIR} NAME)
  get_filename_component(_IDL_BIN_DIR ${IDL_LIBRARY_DIR} PATH)
  get_filename_component(IDL_DIR ${_IDL_BIN_DIR} PATH)
else ()
   if (IDL_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find IDL. Use -DIDL_INCLUDE_DIR and -DIDL_LIBRARY to specify IDL location.")
   endif ()
endif ()

