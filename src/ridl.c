#include <stdio.h>  
#include <stdlib.h> 
#include <signal.h>

#include "idl_export.h" 
#include "readline/readline.h"
#include "readline/history.h"

#include "ridl.h"

static int ridl_options = IDL_INIT_CLARGS;
static int execute_cmd = 0;
static char *cmd;
static int logging = 0;
static char *log_file;
static FILE *log_fp;

static IDL_MSG_DEF msg_arr[] = {  
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." }, 
};
static IDL_MSG_BLOCK msg_block; 

// TODO: need to figure out what this should be
static char *history_file_location = "/Users/mgalloy/.idl/itt/rbuf/history";



void ridl_logoutput(int flags, char *buf, int n) {
  printf("%s", buf);
  if (flags & IDL_TOUT_F_NLPOST) printf("\n");

  fprintf(log_fp, "%s", buf);
  if (flags & IDL_TOUT_F_NLPOST) fprintf(log_fp, "\n");
}


/*
   Registered to be called when the !prompt changes.
*/
void ridl_changeprompt(IDL_STRING *prompt) {
  ridl_prompt = IDL_STRING_STR(prompt);
}


/*
   Reads IDL history file and populates the Readline history.
*/
void ridl_populatehistory(void) {
  FILE *fp = fopen(history_file_location, "r");
  int i, line_number = 0, rline_number;
  char history[RIDL_RBUF_SIZE][RIDL_MAX_LINE_LENGTH];
  char line[RIDL_MAX_LINE_LENGTH];
  
  while (fgets(line, RIDL_MAX_LINE_LENGTH, fp) != NULL) {
    for (i = strlen(line); i > 0; i--) {
      if (line[i] == '<') { 
        line[i] = '\0';
      }
    }
    strcpy(history[line_number++], line);
  }
  
  for (rline_number = line_number - 1; rline_number >= 0; rline_number--) {
    add_history(history[rline_number]);
  }
  
  fclose(fp);
}

/*
   Execute an IDL command typed at the command line by the user.
*/
int ridl_executestr(char *cmd) {
  if (logging) fprintf(log_fp, "%s%s\n", ridl_prompt, cmd);

  int result = IDL_ExecuteStr(cmd);

  add_history(cmd);
  // TODO: also add to IDL history

  fprintf(stderr, "\e[0m");   // reset colors if there was a compile error
  return(result);
}


/*
   Registered to be called before a compiler error is shown.
*/
void ridl_show_compile_error(void) {
  fprintf(stderr, "\e[31m");
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
  if (logging) fclose(log_fp);

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
   Launch editor in $EDITOR environment variable.
*/
void ridl_launcheditor(char *filename) {
  char *cmdFormat = "ridl_launcheditor, '%s'";
  char *cmd = (char *)malloc(strlen(filename) + strlen(cmdFormat) + 1);
  sprintf(cmd, cmdFormat, filename);
  int result = IDL_ExecuteStr(cmd);
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
    IDL_UicbRegPromptChange(ridl_changeprompt);
    
    if (!(msg_block = IDL_MessageDefineBlock("RIDL_MSG_BLOCK", 
                                             IDL_CARRAY_ELTS(msg_arr),  
                                             msg_arr))) return(1);                                            
    
    using_history();
    ridl_populatehistory();
    
    // TODO: add completers for routines and variables completion
    
    // handle -e option if it was present
    if (execute_cmd) {
      int error = IDL_ExecuteStr(cmd);
      return(IDL_Cleanup(IDL_FALSE));
    }
    
    while (1) {      
      char *line = readline (ridl_prompt);
      
      // normal exit by hitting ^D
      if (line == NULL) { 
        printf("\n");
        return(IDL_Cleanup(IDL_FALSE)); 
      };
      
      // magic lines start with :, anything else is passed to IDL
      int firstcharIndex = ridl_firstchar(line);
      char firstchar = line[firstcharIndex];
      if (firstchar == ':') {
        if ((firstcharIndex + 1 < strlen(line)) && (line[firstcharIndex + 1] == '!')) {
          char *expansion;
          int expansion_result;
          char *expansion_line = (char *) malloc(strlen(line) + 1);
          strcpy(expansion_line, line + firstcharIndex + 1);
          expansion_result = history_expand(expansion_line, &expansion);
          switch (expansion_result) {
            case -1: 
              printf("Error in expansion\n");
              break;
            case 2: 
              printf("%s%s\n", ridl_prompt, expansion);
              break;
            case 0: 
            case 1: 
              printf("%s%s\n", ridl_prompt, expansion);
              int error = ridl_executestr(expansion);
              break;
          }
          free(expansion_line);
          free(expansion);
        } else {                          
          char *cmd = ridl_getnextword(line, firstcharIndex);
          if (strcmp(cmd, ":doc") == 0) {
            char *routine = ridl_getnextword(line, firstcharIndex + 5);
            char *man = (char *)malloc(8 + strlen(routine));
            sprintf(man, "man, '%s'", routine);
            int error = IDL_ExecuteStr(man);
            free(man);
            free(routine);
          } else if (strcmp(cmd, ":log") == 0) {
            if (logging) fclose(log_fp);

            logging = 1;
            char *filename = ridl_getnextword(line, firstcharIndex + 5);
            log_fp = fopen(filename, "w");
            IDL_ToutPush(ridl_logoutput);
            free(filename);
          } else if (strcmp(cmd, ":unlog") == 0) {
            if (logging) {
              fclose(log_fp);
              IDL_ToutPop();
            }
            logging = 0;
          }
          free(cmd);
        }
      } else {
        if (line && *line) {           
          // check for .edit
          if (firstchar == '.') {
            char *cmd = ridl_getnextword(line, firstcharIndex);
            if (strcmp(cmd, ".edit") == 0) {
              char *file = ridl_getnextword(line, firstcharIndex + strlen(cmd) + 1);
              ridl_launcheditor(file);              
              free(file);
              
              add_history(line);
            } else {
              int error = ridl_executestr(line);
            }
            free(cmd);
          } else {
            int error = ridl_executestr(line);
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