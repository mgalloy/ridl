TODAY(rIDL_BUILD_DATE)

find_package(Subversion)
if(Subversion_FOUND)
  Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} rIDL)
endif(Subversion_FOUND)

# configure a header file to pass some of the CMake settings to the source 
# code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/ridl_config.h.in"
  "${PROJECT_BINARY_DIR}/src/ridl_config.h"
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
                      #"/usr/local/Cellar/readline/6.1/lib/libreadline.a"
                      "${Readline_LIBRARY}"
                      "${IDL_LIBRARY}"
                      )  

set_target_properties(ridl 
                      PROPERTIES 
                        LINK_FLAGS "-L/Applications/itt/idl/idl/bin/bin.darwin.x86_64  -L/usr/X11/lib -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lreadline -lfreetype"
)                                   
