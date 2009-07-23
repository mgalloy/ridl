#include <stdio.h>  
#include <stdlib.h> 
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
  exit(0);
}


void ridl_welcome(void) {
  printf("rIDL 0.1: Really Interactive Data Language\n");  
}


int ridl_firstchar(char *line) {
  int i;
  for (i = 0; i < strlen(line); i++) {
    if (line[i] != ' ') return(i);
  }
}


char *ridl_getnextword(char *line, int start) {
  int i;
  for (i = start; i < strlen(line); i++) {
    if (line[i] == ' ') break;
  }
  
  int end = i;
  
  char *result = (char *)calloc(i - start + 1, 1);
  for (i = start; i < end; i++) {
    result[i - start] = line[i];
  }
  
  return(result);
}


int main(int argc, char *argv[]) {  
  // TODO: these command line args don't seem to be used; -32 should not be
  // expected to work, but not sure why the rest don't
  IDL_INIT_DATA init_data;    
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  
  // TODO: handle -h and -v also
  
  ridl_welcome();
                                               
  if (IDL_Initialize(&init_data)) {
    IDL_ExitRegister(ridl_exit_handler);

    if (!(msg_block = IDL_MessageDefineBlock("RIDL_MSG_BLOCK", 
                                             IDL_CARRAY_ELTS(msg_arr),  
                                             msg_arr))) return(1);                                            
    
    // TODO: add previous history to readline history
    // TODO: add completer for routines and variables
    
    while (1) {
      char *line = readline ("rIDL> ");
      
      // normal exit by hitting ^D
      if (line == NULL) { 
        printf("\n");
        return(IDL_Cleanup(IDL_FALSE)); 
      };
      
      // magic lines start with :, anything else is passed to IDL
      int firstcharIndex = ridl_firstchar(line);
      char firstchar = line[firstcharIndex];
      if (firstchar == ':') {
        // TODO: handle magic lines
        char *cmd = ridl_getnextword(line, firstcharIndex);
        printf("magic command '%s'\n", cmd);
        free(cmd);
      } else {
        if (line && *line) { 
          add_history(line);
          
          // check for .edit
          if (firstchar == '.') {
            char *cmd = ridl_getnextword(line, firstcharIndex);
            if (strcmp(cmd, ".edit") == 0) {
              // TODO: launch editor
              //    1. find file in !path
              //    2. lookup $EDITOR
              //    3. spawn editor on full filename
              char *file = ridl_getnextword(line, firstcharIndex + strlen(cmd) + 1);
              printf("launch EDITOR on %s.pro\n", file);
              free(file);
            } else {
              int error = IDL_ExecuteStr(line);
            }
            free(cmd);
          } else {
            int error = IDL_ExecuteStr(line);
          }
        }
      }
      
      free(line);
    }    
  } else {
    printf("Failed to initialize Callable IDL session.\n");
    return(1);
  }
  
  return(1);
} 