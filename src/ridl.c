#include <stdio.h>   
#include <signal.h>
#include "idl_export.h" 
#include "readline/readline.h"
#include "ridl.h"

static IDL_MSG_DEF msg_arr[] = {  
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." }, 
};
static IDL_MSG_BLOCK msg_block; 


void ridl_exit_handler(void) {  
  //printf("Bye!\n");
}


void ridl_welcome(void) {
  printf("rIDL 0.1: Really Interactive Data Language\n");  
}


int main(int argc, char *argv[]) {  
  // TODO: these command line args don't seem to be used
  IDL_INIT_DATA init_data;    
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  
  ridl_welcome();
                                               
  if (IDL_Initialize(&init_data)) {
    IDL_ExitRegister(ridl_exit_handler);

    if (!(msg_block = IDL_MessageDefineBlock("RIDL_MSG_BLOCK", 
                                             IDL_CARRAY_ELTS(msg_arr),  
                                             msg_arr))) return(1);                                            
    
    // TODO: add previous history to readline history
    // TODO: add completer for routines
    
    while (1) {
      char *line = readline ("rIDL> ");
      
      // normal exit by hitting ^D
      if (line == NULL) { 
        printf("\n");
        return(IDL_Cleanup(IDL_FALSE)); 
      };
      
      // magic lines start with :, anything else is passed to IDL
      if (line[0] == ':') {
        // TODO: handle magic lines
      } else {
        if (line && *line) { 
          add_history(line);
          int error = IDL_ExecuteStr(line);
        }
      }
      
      free(line);
    }    
  } else {
    printf("Failed to initialize Callable IDL session.\n");
    return(1);
  }
  
  // TODO: exit values don't seem to be passed to shell
  return(1);
} 