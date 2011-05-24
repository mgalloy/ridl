TODAY(rIDL_BUILD_DATE)

# needed to prevent warnings generated when using the Subversion package
cmake_policy(SET CMP0017 NEW)

# configure a header file to pass some CMake settings to the source code
configure_file("ridl_config.h.in" "ridl_config.h")

if (UNIX AND NOT APPLE)
  add_definitions(-m64)
endif ()

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
  set(rIDL_LD_FLAGS "-L${IDL_LIBRARY_DIR} -lXm -L/usr/X11/lib -lfreetype -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2")
else ()
  set(rIDL_LD_FLAGS "")
endif ()

set_target_properties(ridl 
                      PROPERTIES 
                        LINK_FLAGS "${rIDL_LD_FLAGS}"
)                                   

install(TARGETS ridl DESTINATION bin/${IDL_BIN_DIR})
install(FILES idl_routines.txt DESTINATION share)
install(FILES ridl_verbosehelp.txt DESTINATION share)

install(DIRECTORY . 
        DESTINATION share/idl        
        FILES_MATCHING PATTERN "*.pro"
        PATTERN ".svn" EXCLUDE
        PATTERN "CMakeFiles" EXCLUDE)
