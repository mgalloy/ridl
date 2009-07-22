#include <stdio.h>   
#include "idl_export.h" 
#include "ridl.h"


void ridl_exit_handler(void) {  
  printf("Bye!");
}


void ridl_welcome(void) {
  printf("rIDL 0.1: Really Interactive Data Language\n");  
}


int ridl_cleanup(void) {
  return(IDL_Cleanup(IDL_FALSE));
}


void ridl_print_prompt(void) {
  printf("rIDL> ");
}


int main(int argc, char **argv) {  
  IDL_INIT_DATA init_data;  
  char line[MAX_LINE_LENGTH];
  int error, exit_value;
  
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  ridl_welcome();
  
  if (IDL_Initialize(&init_data)) {
    IDL_ExitRegister(ridl_exit_handler);
    
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
    
    exit_value = ridl_cleanup();
  } else {
    printf("Failed to initialize Callable IDL session\n");
    exit_value = 0;
  }
   
  return exit_value;
} 