# configure a header file to pass some of the CMake settings to the source 
# code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/ridl_config.h.in"
  "${PROJECT_BINARY_DIR}/src/ridl_config.h"
  )
  
add_executable(ridl ridl.c)
