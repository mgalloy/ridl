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

# Freetype library not included in Mac OS X distribution of IDL
if (UNIX AND APPLE)
  set(Freetype_LIBRARY_DIR "/usr/X11/lib")
else ()
  set(Freetype_LIBRARY_DIR "${IDL_LIBRARY_DIR}")
endif ()

if (UNIX)
  set(X11_FLAGS "-lXm")
else ()
  set(X11_FLAGS "")
endif ()

set_target_properties(ridl 
                      PROPERTIES 
                        LINK_FLAGS "-L${IDL_LIBRARY_DIR} ${X11_FLAGS} -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -L${Freetype_LIBRARY_DIR} -lfreetype"
)                                   

install(TARGETS ridl DESTINATION bin/${IDL_BIN_DIR})
install(FILES idl_routines.txt DESTINATION share)
install(FILES ridl_verbosehelp.txt DESTINATION share)
