#include <stdio.h>   
#include <signal.h>
#include "idl_export.h" 
#include "readline/readline.h"
#include "ridl.h"

static int exit_value;

static IDL_MSG_DEF msg_arr[] = {  
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." }, 
};
static IDL_MSG_BLOCK msg_block; 


void ridl_signal_handler(int signo) {
  //printf("Received signal %d\n", signo);
  
  exit_value = IDL_Cleanup(IDL_FALSE);  
}


void ridl_exit_handler(void) {  
  printf("Bye!\n");
  //exit(exit_value);
}


void ridl_welcome(void) {
  printf("rIDL 0.1: Really Interactive Data Language\n");  
}


void ridl_print_prompt(void) {
  printf("rIDL> ");
}


int main(int argc, char *argv[]) {  
  IDL_INIT_DATA init_data;  
  char *line;
  int error;
  IDL_SignalSet_t *set;
  
  // TODO: these command line args don't seem to be used
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  
  ridl_welcome();
                                               
  if (IDL_Initialize(&init_data)) {
    IDL_ExitRegister(ridl_exit_handler);

    if (!(msg_block = IDL_MessageDefineBlock("RIDL_MSG_BLOCK", 
                                             IDL_CARRAY_ELTS(msg_arr),  
                                             msg_arr))) return IDL_FALSE; 
                                           
    // TODO: right now you have to cntrl-C to exit ridl
    //IDL_SignalRegister(SIGINT, ridl_signal_handler, M_RIDL_SIGNAL_REG);
    //IDL_SignalRegister(SIGABRT, ridl_signal_handler, M_RIDL_SIGNAL_REG);
    //IDL_SignalRegister(SIGKILL, ridl_signal_handler, M_RIDL_SIGNAL_REG);
    //IDL_SignalRegister(SIGQUIT, ridl_signal_handler, M_RIDL_SIGNAL_REG);
    //IDL_SignalRegister(SIGTERM, ridl_signal_handler, M_RIDL_SIGNAL_REG);
    //IDL_SignalRegister(SIGSTOP, ridl_signal_handler, M_RIDL_SIGNAL_REG);
    
    while (1) {
      char *line = readline ("rIDL> ");
      if (line == NULL) { 
        printf("\n");
        exit_value = IDL_Cleanup(IDL_FALSE); 
      };
      
      // TODO: check for "magic" line that begins with ":"
      
      add_history(line);
      error = IDL_ExecuteStr(line);
      free(line);
      //printf("code = %d\n", error);
    }    
  } else {
    printf("Failed to initialize Callable IDL session.\n");
    exit_value = 1;
  }
   
  return(exit_value);
} 