#include <stdio.h>  
#include <stdlib.h> 
#include <signal.h>

#include "idl_export.h" 
#include "readline/readline.h"
#include "ridl.h"

static int ridl_options = IDL_INIT_CLARGS;
static int execute_cmd = 0;
static char *cmd;

static IDL_MSG_DEF msg_arr[] = {  
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." }, 
};
static IDL_MSG_BLOCK msg_block; 


void ridl_show_compile_error(void) {
  //printf("Showing a compiler error...\n");
}


/*
   Called when EXIT routine is encountered.
*/
void ridl_exit(void) {
  int result = IDL_Cleanup(IDL_FALSE);
}


/*
   Handle any rIDL cleanup before exiting.
*/
void ridl_exit_handler(void) {  
  exit(EXIT_SUCCESS);
}


/*
   Print rIDL welcome message if not quiet.
*/
void ridl_welcome(void) {
  if (!(ridl_options & IDL_INIT_QUIET)) {
    printf("rIDL 0.1: Really Interactive Data Language\n");  
  }
}


/*
   Return the index of the first non-space character on the line.
*/
int ridl_firstchar(char *line) {
  int i;
  for (i = 0; i < strlen(line); i++) {
    if (line[i] != ' ') return(i);
  }
}


/*
   Return the word starting from index start on line. The result is 
   dynamically allocated, so should be freed by the calling routine.
*/
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


/*
   Use IDL's FILE_WHICH to find filename in IDL path including the current
   directory. If not found, then filename is copied and returned.
   
   This routine creates a _$RIDL_EDIT variable that will be left in the 
   current IDL scope.
*/
char *ridl_findpath(char *filename) {
  char *cmdFormat = "_$ridl_edit = file_which('%s.pro', /include_current_dir)";
  char *cmd = (char *)malloc(strlen(cmdFormat) - 2 + strlen(filename) + 1);
  sprintf(cmd, cmdFormat, filename);  
  int result = IDL_ExecuteStr(cmd);
  free(cmd);
  
  // TODO: it would be nice to not leave this variable around
  // IDL_CallRoutineByString("MG_FILE_WHICH", &result, 1, params, NULL)
  IDL_VPTR ridl_filename = IDL_FindNamedVariable("_$ridl_edit", IDL_FALSE);

  char *ridl_filename_str = IDL_STRING_STR(&ridl_filename->value.str);
  char *file;
  if (strlen(ridl_filename_str) == 0) {
    file = (char *)malloc(strlen(filename) + 4 + 1);
    sprintf(file, "%s.pro", filename);
  } else {
    file = (char *)malloc(strlen(ridl_filename_str) + 1);
    strcpy(file, ridl_filename_str);
  }

  return(file);
}


/*
   Launch editor in $EDITOR environment variable.
*/
void ridl_launcheditor(char *filename) {
  char *editor = "$EDITOR";
  char *cmd = (char *)malloc(strlen(filename) + strlen(editor) + 1 + 1);
  sprintf(cmd, "%s %s", editor, filename);
  system(cmd);
  free(cmd);
}


/*
   Prints version information about rIDL and IDL to stdout.
*/
void ridl_printversion(void) {
  IDL_STRING *version = IDL_SysvVersionRelease();
  IDL_STRING *os = IDL_SysvVersionOS();
  IDL_STRING *arch = IDL_SysvVersionArch();
  printf("rIDL %s: Really Interactive Data Language\n", ridl_version);
  printf("IDL %s %s %s\n", 
         IDL_STRING_STR(version), 
         IDL_STRING_STR(os), 
         IDL_STRING_STR(arch));
}


/*
   Print usage and options for rIDL.
*/
void ridl_printhelp(void) {
  char *indent = "  ";    // prefix for each line of switch help
  int switch_width = 25;  // width of switch column in spaces
  
  ridl_printversion();
  
  printf("\n");
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
         "display this help message");
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
         "display version information");
  printf("%s%-*s %s\n", indent, switch_width, "-vm=FILENAME", 
         "start the virtual machine with the given .sav file");
}


/*
   rIDL main loop.
*/
int main(int argc, char *argv[]) {  
  int a;
  for (a = 0; a < argc; a++) {
    if (strcmp(argv[a], "-arg") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-args") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-demo") == 0) {
      // TODO: handle
    }    
    if (strcmp(argv[a], "-e") == 0) {
      if (a + 1 < argc) {
        execute_cmd = 1;
        cmd = argv[a + 1];
        printf("picking up cmd %s\n", cmd);
      }
    }    
    if (strcmp(argv[a], "-em") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-h") == 0) {
      ridl_printhelp();
      exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[a], "-novm") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-pref") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-quiet") == 0) {
      ridl_options |= IDL_INIT_QUIET;
    } 
    if (strcmp(argv[a], "-queue") == 0) {
      ridl_options |= IDL_INIT_LMQUEUE;
    }    
    if (strcmp(argv[a], "-rt") == 0) {
      // TODO: handle
      // use: int IDL_RuntimeExec(char * file)
    }        
    if (strcmp(argv[a], "-student") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-ulicense") == 0) {
      // TODO: handle
    }        
    if (strcmp(argv[a], "-v") == 0) { 
      ridl_printversion();
      exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[a], "-vm") == 0) {
      // TODO: handle
    }   
    
    // TODO: handle positional parameter i.e. batch file to execute     
  }
  
  IDL_INIT_DATA init_data;
  init_data.options = ridl_options;
  init_data.clargs.argc = argc;
  init_data.clargs.argv = argv;
  
  ridl_welcome();
                                               
  if (IDL_Initialize(&init_data)) {
    IDL_ExitRegister(ridl_exit_handler);
    IDL_UicbRegExitDone(ridl_exit);
    IDL_UicbRegShowCompileErr(ridl_show_compile_error);
    
    if (!(msg_block = IDL_MessageDefineBlock("RIDL_MSG_BLOCK", 
                                             IDL_CARRAY_ELTS(msg_arr),  
                                             msg_arr))) return(1);                                            
    
    // TODO: add previous history to readline history
    //       figure out IDL_RbufRecall, parse .idl/itt/rbuf/history file, or
    //       use RECALL_COMMANDS()
    
    // TODO: add completer for routines and variables
    
    // handle -e option if it was present
    if (execute_cmd) {
      int error = IDL_ExecuteStr(cmd);
      return(IDL_Cleanup(IDL_FALSE));
    }
    
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
        char *cmd = ridl_getnextword(line, firstcharIndex);
        if (strcmp(cmd, ":doc") == 0) {
          char *routine = ridl_getnextword(line, firstcharIndex + 5);
          char *man = (char *)malloc(8 + strlen(routine));
          sprintf(man, "man, '%s'", routine);
          int error = IDL_ExecuteStr(man);
          free(man);
          free(routine);
        }
        free(cmd);
      } else {
        if (line && *line) { 
          add_history(line);
          
          // check for .edit
          // TODO: see if this can be simplified by using IDL_TypeIDEUserEditFunc
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