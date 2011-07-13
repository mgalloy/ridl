# convenience variable for IDL's default install directories
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(_IDL_PROGRAM_FILES_DIR "C:/Program Files/ITT/IDL")
  set(_IDL_NAME "IDL")
  set(_IDL_OS "")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(_IDL_PROGRAM_FILES_DIR "/Applications/itt/idl")
  set(_IDL_NAME "idl")
  set(_IDL_OS ".darwin")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(_IDL_PROGRAM_FILES_DIR "/usr/local/itt/idl")
  set(_IDL_NAME "idl")
  set(_IDL_OS ".linux")
endif ()


# find IDL based on version numbers; if you want a specific one, set
# it prior to running configure
if (NOT DEFINED IDL_FIND_VERSION)
  set(_IDL_KNOWN_VERSIONS "81" "80" "71" "706" "64")

  set(_IDL_SEARCH_DIRS "${_IDL_PROGRAM_FILES_DIR}")
  foreach (_IDL_KNOWN_VERSION ${_IDL_KNOWN_VERSIONS})
    list(APPEND _IDL_SEARCH_DIRS
         "${_IDL_PROGRAM_FILES_DIR}/${_IDL_NAME}${_IDL_KNOWN_VERSION}")
  endforeach ()
  
  set(APPEND _IDL_SEARCH_DIRS "${_IDL_PROGRAM_FILES_DIR}/${_IDL_NAME}")
  foreach (_IDL_KNOWN_VERSION ${_IDL_KNOWN_VERSIONS})
    list(APPEND _IDL_SEARCH_DIRS
         "${_IDL_PROGRAM_FILES_DIR}${_IDL_KNOWN_VERSION}")
  endforeach ()
endif ()

find_path(IDL_INCLUDE_DIR
  idl_export.h
  PATHS ${_IDL_SEARCH_DIRS}
  HINTS ${IDL_ROOT}
  PATH_SUFFIXES external/include
  )

if (IDL_FIND_COMPONENTS EQUAL 8)
  set(_IDL_BIN_SUFFIXES /bin/bin${_IDL_OS}.x86_64)
elseif (IDL_FIND_COMPONENTS EQUAL 4)
  set(_IDL_BIN_SUFFIXES /bin/bin${_IDL_OS}.x86 /bin/bin${_IDL_OS}.i386)
else ()
  math(EXPR IDL_FIND_COMPONENTS_BITS "8 * ${IDL_FIND_COMPONENTS}") 
  message(FATAL_ERROR "${IDL_FIND_COMPONENTS_BITS}-bit build invalid")
endif ()

find_library(IDL_LIBRARY
  NAMES idl
  PATHS ${_IDL_SEARCH_DIRS}
  HINTS ${IDL_ROOT}
  PATH_SUFFIXES ${_IDL_BIN_SUFFIXES}
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
      message(FATAL_ERROR "Could not find IDL")
   endif ()
endif ()

