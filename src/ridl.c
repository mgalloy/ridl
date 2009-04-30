#include <stdio.h>   
#include "idl_export.h" 
#include "ridl.h"


void ridl_welcome(void) {
  printf("rIDL 0.1: Really Interactive Data Language\n");  
}

void ridl_cleanup(void) {
  IDL_Cleanup(IDL_FALSE);
}

void ridl_print_prompt(void) {
  printf("rIDL> ");
}

int main(int argc, char **argv) {  
  IDL_INIT_DATA init_data;  
  char line[MAX_LINE_LENGTH];
  int error;
  
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  
  if (IDL_Initialize(&init_data)) {
    ridl_welcome();
    
    // loop while active
    ridl_print_prompt();
    fgets(line, MAX_LINE_LENGTH, stdin);
    error = IDL_ExecuteStr(line);
    printf("code = %d\n", error);
    
    ridl_print_prompt();
    fgets(line, MAX_LINE_LENGTH, stdin);
    error = IDL_ExecuteStr(line);
    printf("code = %d\n", error);

    ridl_print_prompt();
    fgets(line, MAX_LINE_LENGTH, stdin);
    error = IDL_ExecuteStr(line);
    printf("code = %d\n", error);
    
    ridl_cleanup();
  } else {
    printf("Failed to initialize Callable IDL session\n");
  }
   
  return 1;   
} 