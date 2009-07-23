#include <stdio.h>  
#include <stdlib.h> 
#include <signal.h>

#include "idl_export.h" 
#include "readline/readline.h"
#include "ridl.h"

static char *ridl_version = "0.1";
static int show_welcome = 1;

static IDL_MSG_DEF msg_arr[] = {  
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." }, 
};
static IDL_MSG_BLOCK msg_block; 


void ridl_exit_handler(void) {  
  exit(EXIT_SUCCESS);
}


void ridl_welcome(void) {
  if (show_welcome) {
    printf("rIDL 0.1: Really Interactive Data Language\n");  
  }
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


char *ridl_findpath(char *filename) {
  // TODO: find full path for filename referred to on .edit line
  char *fullpath = (char *)malloc(strlen(filename) + 1);
  strcpy(fullpath, filename);
}


void ridl_launcheditor(char *filename) {
  char *editor = "mate -w";    // TODO: change editor to use $EDITOR
  char *cmd = (char *)malloc(strlen(filename) + strlen(editor) + 1 + 1);
  sprintf(cmd, "%s %s", editor, filename);
  system(cmd);
  free(cmd);
}


void ridl_printversion(void) {
  IDL_STRING *version = IDL_SysvVersionRelease();
  IDL_STRING *os = IDL_SysvVersionOS();
  IDL_STRING *arch = IDL_SysvVersionArch();
  printf("rIDL %s: IDL %s %s %s\n", 
         ridl_version, 
         IDL_STRING_STR(version), 
         IDL_STRING_STR(os), 
         IDL_STRING_STR(arch));
}


void ridl_printhelp(void) {
  char *indent = "  ";
  int switch_width = 25;
  ridl_printversion();
  
  printf("usage: ridl [options]\n");
  printf("\n");
  printf("options:\n");
  printf("%s%-*s %s\n", indent, switch_width, "-32", 
         "start in 32-bit mode");
  printf("%s%-*s %s\n", indent, switch_width, "-arg value", 
         "saves a single command line argument");
  printf("%s%-*s %s\n", indent, switch_width, "-args value1 value2 ...", 
         "saves multiple command line arguments");
  printf("%s%-*s %s\n", indent, switch_width, "-demo", 
         "start in 7-minute demo mode");
  printf("%s%-*s %s\n", indent, switch_width, "-e command", 
         "executes command and exit");
  printf("%s%-*s %s\n", indent, switch_width, "-em=FILENAME", 
    "execute the given .sav file containing an embedded license");
  printf("%s%-*s %s\n", indent, switch_width, "-h", 
         "show this help message");
  printf("%s%-*s %s\n", indent, switch_width, "-novm", 
         "use 7-minute demo mode instead of virtual machine with -rt");
  printf("%s%-*s %s\n", indent, switch_width, "-pref=FILENAME", 
         "load the given preference file");
  printf("%s%-*s %s\n", indent, switch_width, "-queue", 
         "wait for a license when none are available");
  printf("%s%-*s %s\n", indent, switch_width, "-quiet", 
         "suppress printing information about IDL on startup");
  printf("%s%-*s %s\n", indent, switch_width, "-rt=FILENAME", 
         "start with a runtime license on the given .sav file");
  printf("%s%-*s %s\n", indent, switch_width, "-student", 
         "start in student mode");
  printf("%s%-*s %s\n", indent, switch_width, "-ulicense", 
         "start IDL with a unique license");
  printf("%s%-*s %s\n", indent, switch_width, "-v", 
         "print version information");
  printf("%s%-*s %s\n", indent, switch_width, "-vm=FILENAME", 
         "start the virtual machine with the given .sav file");
}


int main(int argc, char *argv[]) {  
  int a;
  for (a = 0; a < argc; a++) {
    if (strcmp(argv[a], "-v") == 0) { 
      ridl_printversion();
      exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[a], "-h") == 0) {
      ridl_printhelp();
      exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[a], "-quiet") == 0) {
      show_welcome = 0;
    }    
  }
  
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
              char *file = ridl_getnextword(line, firstcharIndex + strlen(cmd) + 1);
              char *fullpath = ridl_findpath(file);
              ridl_launcheditor(fullpath);
              free(file);
              free(fullpath);
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
    return(EXIT_FAILURE);
  }
  
  return(EXIT_FAILURE);
} 