#include <stdio.h>   
#include "idl_export.h" 
#include "ridl.h"


void ridl_welcome() {
  printf("rIDL 0.1: Really Interactive Data Language\n");  
}

void ridl_cleanup() {
  IDL_Cleanup(IDL_FALSE);
}

void ridl_print_prompt() {
  printf("rIDL> ");
}

int main(int argc, char **argv) {  
  IDL_INIT_DATA init_data;  
  char line[MAX_LINE_LENGTH];
      
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  
  if (IDL_Initialize(&init_data)) {
    ridl_welcome();
    
    // loop while active
    ridl_print_prompt();
    fgets(line, MAX_LINE_LENGTH, stdin);
    IDL_ExecuteStr(line);
    
    ridl_cleanup();
  } else {
    printf("Failed to initialize Callable IDL session\n");
  }
   
  return 1;   
} 