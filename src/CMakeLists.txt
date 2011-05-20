TODAY(rIDL_BUILD_DATE)

# needed to prevent warnings generated when using the Subversion package
cmake_policy(SET CMP0017 NEW)
find_package(Subversion)
if (Subversion_FOUND)
  Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} rIDL)
endif ()

# configure a header file to pass some CMake settings to the source code
configure_file (
  "ridl_config.h.in"
  "ridl_config.h"
  )
  
add_executable(ridl 
               ridl.c
               ridl_completion.c
               ridl_history.c
               ridl_logging.c
               ridl_magic.c
               ridl_preferences.c
               ridl_strings.c
               )
               
target_link_libraries(ridl
                      "${Readline_LIBRARY}"
                      "${IDL_LIBRARY}"
                      )  

# TODO: get rid of this!
set_target_properties(ridl 
                      PROPERTIES 
                        LINK_FLAGS "-L/Applications/itt/idl/idl/bin/bin.darwin.x86_64  -L/usr/X11/lib -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lreadline -lfreetype"
)                                   

# TODO: fix this!
install(TARGETS ridl DESTINATION bin/bin.darwin.x86_64)
install(FILES idl_routines.txt DESTINATION share)
