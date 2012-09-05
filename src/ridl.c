#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>

#include "idl_export.h" 
#include "readline/readline.h"
#include "readline/history.h"

#include "ridl.h"
#include "ridl_magic.h"
#include "ridl_history.h"
#include "ridl_strings.h"
#include "ridl_config.h"
#include "ridl_logging.h"
#include "ridl_completion.h"
#include "ridl_preferences.h"
#include "ridl_idlpreferences.h"


/// user information with fields: logname, homedir, pid, host, wd, date
static IDL_USER_INFO user_info;

static int ridl_options = IDL_INIT_CLARGS;

static int execute_cmd = 0;
static char *cmd;

static int execute_batch_file = 0;
static char *batch_file;

static int runtime_exec = 0;
static char *runtime_filename;

static int preferences_file_set = 0;
static char *preferences_filename;

static int use_colors = 1;
static int auto_compile = 1;


static IDL_MSG_DEF msg_arr[] = {
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." },
};
static IDL_MSG_BLOCK msg_block; 

/// set to indicate that rIDL should just exit when asked
int ridl_really_exit = 1;

#define N_FUNMAP_FUNCTIONS 4
char *funmap_function_names[] = { "ridl-stepinto",
                                  "ridl-stepover",
                                  "ridl-stepreturn",
                                  "ridl-savegraphic" }; 
rl_command_func_t *funmap_functions[] = {
  (rl_command_func_t *)ridl_stepinto_cmd,
  (rl_command_func_t *)ridl_stepover_cmd,
  (rl_command_func_t *)ridl_stepreturn_cmd,
  (rl_command_func_t *)ridl_savegraphic_cmd
};

  

/**
   IDL callback called when the initial IDL startup text has finished 
   printing.
*/
void ridl_inittextdone(void) {
}


char *ridl_ridlprompt(void) {
  return(ridl_prompt);
}


void ridl_updateprompt(void) {
  char *tmp = (char *)malloc(RIDL_PROMPT_LENGTH);
  char *tmp2 = (char *)malloc(RIDL_PROMPT_LENGTH);
  char *tmp3 = (char *)malloc(RIDL_PROMPT_LENGTH);
  char cmdnum[10];
  char *dots;
  int i;

  sprintf(cmdnum, "%d", ridl_cmdnumber);

  ridl_replacestr(tmp, ridl_prompt, "wdir", ridl_current_wdir);
  ridl_replacestr(ridl_expandedprompt, tmp, "cmdnum", cmdnum);

  dots = (char *)malloc(strlen(ridl_expandedprompt));
  for (i = 0; i < strlen(ridl_expandedprompt) - 1; i++) {
    dots[i] = '.';
  }
  dots[i] = '\0';
  
  ridl_replacestr(tmp2, ridl_prompt2, "wdir", ridl_current_wdir);
  ridl_replacestr(tmp3, tmp2, "cmdnum", cmdnum);
  ridl_replacestr(ridl_expandedprompt2, tmp3, "p1dots", dots);
  
  free(tmp);
  free(tmp2);
  free(tmp3);
  free(dots);
}


/**
   Registered to be called when the !prompt changes.
   
   @param[in] prompt string to change prompt to
*/
void ridl_changeprompt(IDL_STRING *prompt) {
  ridl_prompt = IDL_STRING_STR(prompt);
  ridl_updateprompt();
}


/**
   Registered to be called when the !prompt changes.
   
   @param[in] path string to change path to
*/
void ridl_changepath(IDL_STRING *path) {
  ridl_get_userdefinedroutines_list();
}


/**
   Registered to be called when the current working directory changes.
   
   @param[in] dir directory that working directory is changing to
*/
void ridl_changewdir(char *dir) {
  if (strncmp(user_info.homedir, dir, (int) strlen(user_info.homedir)) == 0) {
    ridl_current_wdir[0] = '~';
    strcpy(ridl_current_wdir + 1, dir + strlen(user_info.homedir));
  } else {
    strcpy(ridl_current_wdir, dir);
  }
  ridl_updateprompt();
  
  // get .pro files in the current directory for use in tab completion
  // TODO: put this back when fixed
  //ridl_get_currentdir_userdefinedroutines_list(); 
}


/**
   Variable argument wrapper to printf-like routine to print error messages.
*/
void ridl_warning(const char *format, ...) {
  va_list args;
  printf("%% ");
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n");
}


int ridl_file_exists(const char *filename) {
  FILE *file;
  if (file = fopen(filename, "r")) {
    fclose(file);
    return(1);
  }
  return(0);
}


/**
   Print the current line of source code that execution is stopped at (if it
   is stopped somewhere).
*/
void ridl_printsource(void) {
  if (IDL_DebugGetStackDepth() > 1) {
    int result = IDL_ExecuteStr("ridl_printsource");
  }
}


int ridl_stepinto_cmd(void) {
  char *cmd = ".step";
  printf("%s\n", cmd);  
  int status = ridl_executestr(cmd, 1);
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


int ridl_stepover_cmd(void) {
  char *cmd = ".stepover";
  printf("%s\n", cmd);  
  int status = ridl_executestr(cmd, 1);
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


int ridl_stepreturn_cmd(void) {
  char *cmd = ".return";
  printf("%s\n", cmd);   
  int status = ridl_executestr(cmd, 1);
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


int ridl_savegraphic_cmd(void) {
  char *cmd = ":save_graphic";
  printf("%s\n", cmd);   
  int status = ridl_executeline(cmd, 1);
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


/**
   Execute an IDL command.
   
   @return integer representing success
   
   @param[in] cmd IDL command to execute
   @param[in] save flag to indicate if the command should be saved in the 
              history and hence if command number should be incremented
*/
int ridl_executestr(char *cmd, int save) {
  int result;
  
  if (ridl_islogging()) ridl_logcmd(ridl_expandedprompt, cmd);
  if (ridl_isnotebooking()) ridl_notebookcmd(ridl_expandedprompt, cmd);
  
  result = IDL_ExecuteStr(cmd);
  
  // add line to both Readline's history and IDL's history file
  if (save) {
    ridl_addhistoryline(cmd);
    ridl_cmdnumber++;
  }

  // update prompt
  ridl_updateprompt();

  // reset colors if there was a compile error
  if (use_colors) fprintf(stderr, "\e[0m");
  
  return(result);
}


/**
   IDL callback registered to be called before a compiler error is shown.
*/
void ridl_show_compile_error(void) {
  if (use_colors) fprintf(stderr, "\e[31m");  // show compiler errors in red
}


/**
   IDL callback registered to be called after IDL_Cleanup is called. 
*/
void ridl_deathhint(void) {
}


/**
   IDL callback registered to be called when EXIT routine is encountered.
*/
void ridl_exit(void) {
  int status = IDL_Cleanup(IDL_FALSE);
  exit(status);  
}


/**
   IDL callback registered to be called when IDL is done.
*/
void ridl_exit_handler(void) {
  if (ridl_isnotebooking()) ridl_closenotebook();
  if (ridl_islogging()) ridl_closelog(); 
  if (ridl_isteeing()) ridl_closelog();
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
   Return the index of the last non-space character on the line. Returns -1
   if there is no last character, i.e., in an empty line or in a line with an 
   unmatched string delimiter like:
   
       IDL> s = 'Hello?
*/
int ridl_lastchar(char *line) {
  int i, pos, comment_pos = strlen(line) - 1;
  
  // return -1 if empty line
  if (strlen(line) == 0) return -1;
  
  // return -1 if the line ends with an unterminated string
  if (ridl_instring(line, strlen(line) - 1)) return -1;
  
  pos = strcspn(line, ";");
  while (ridl_instring(line, pos) && pos < comment_pos) {
    pos += strcspn(line + pos, ";");
  }
  comment_pos = pos;
  
  for (i = comment_pos - 1; i >= 0; i--) {
    if (line[i] != ' ') return(i);
  }
}


void ridl_setautocompile(int _auto_compile) {
  auto_compile = _auto_compile;
}


int ridl_getautocompile() {
  return auto_compile;
}


/**
   Launch editor in $EDITOR environment variable on the given filename.
   
   @param[in] filename file to launch editor on
*/
void ridl_launcheditor(char *filename) {
  int result;
  IDL_VPTR ridl_editfile;
  char *launchCmdFormat, *launchCmd, *compileCmdFormat, *compileCmd;
  
  launchCmdFormat = "_ridl_editfile = ridl_launcheditor('%s')";
  launchCmd = (char *)malloc(strlen(filename) + strlen(launchCmdFormat) + 1);
  sprintf(launchCmd, launchCmdFormat, filename);
  
  result = IDL_ExecuteStr(launchCmd);
  free(launchCmd);

  if (auto_compile) {
    compileCmdFormat = ".compile %s";
    compileCmd = (char *)malloc(strlen(filename) + strlen(compileCmdFormat) + 1);
    ridl_editfile = IDL_FindNamedVariable("_ridl_editfile", 0);  
    sprintf(compileCmd, compileCmdFormat, IDL_VarGetString(ridl_editfile));

    result = IDL_ExecuteStr(compileCmd);
    free(compileCmd);
  }
  
  result = IDL_ExecuteStr("delvar, _ridl_editfile");
}


/**
   Calls IDL routine IDL_GETEVENTS to check for widget events. 
   
   This is required to allow widget programs to receive events while the 
   command line is active.
*/
void ridl_getevents() {
  char *cmd = "ridl_getevents";
  int result = IDL_ExecuteStr(cmd);
}


/**
   This is the Readline event hook callback; called by Readline when waiting
   for input, but no more than 10 times per second.
*/
static int ridl_event_hook () {
  ridl_getevents();
  sleep(ridl_event_delay);
  return(0);
}


// Find the position of the continuation $, if present; -1 if not found
int ridl_findcontinuationpos(char *line) {
  int i;
  int only_space = 1;
  int after_char = 0;
  int inside_single_quotes = 0, inside_double_quotes = 0, inside_comment = 0;
  
  for (i = 0; i < strlen(line); i++) {
    switch (line[i]) {
      case '\t':
      case ' ':
        after_char = 0;
        break;
        
      case ';':
        only_space = 0;
        after_char = 0;
        if (!inside_single_quotes && !inside_double_quotes) return(-1);
        break;

      case '\'':
        only_space = 0;
        after_char = 0;
        if (inside_single_quotes) {
          inside_single_quotes = 0;
        } else if (!inside_double_quotes) {
          inside_single_quotes = 1;
        }
        break;
        
      case '"':
        only_space = 0;
        after_char = 0;
        if (inside_double_quotes) {
          inside_double_quotes = 0;
        } else if (!inside_single_quotes) {
          inside_double_quotes = 1;
        }
        break;
        
      case '$':      
        // quoted        
        if (inside_single_quotes || inside_double_quotes) break;
        
        // spawn to OS
        if (only_space && !continued) break;
        
        // in a variable name
        if (after_char) break;
        
        return(i);

      default:
        only_space = 0;
        after_char = 0;
        
        if (after_char && (line[i] >= '0' && line[i] <= '9')) after_char = 1;
        if ((line[i] == '_') 
              || (line[i] >= 'a' && line[i] <= 'z') 
              || (line[i] >= 'A' && line[i] <= 'Z')) after_char = 1;
    }
  }
  
  return(-1);
}

char *ridl_linecontinued(char *line) {
  int continuationPos = ridl_findcontinuationpos(line);

  if (continuationPos >= 0) {
    line[continuationPos] = '\0';
    return(line);
  } else {
    return(NULL);
  }
}

char *ridl_readline(void) {
  char *line;
  
  // print source code line if we are not at the main level
  ridl_printsource();
  
  // prompt changes depending on if this is a continuation line
  if (continued) {
    line = readline(ridl_expandedprompt2); 
  } else {
    line = readline(ridl_expandedprompt);
  }
  
  return(line);
}


void ridl_printfunmap(char *funmap_function_name, rl_command_func_t *funmap_function) {
  int binding;
  char **keyseqs = rl_invoking_keyseqs(funmap_function);
  if (keyseqs == 0) return;
  
  printf("  %s: ", funmap_function_name);
  for (binding = 0; keyseqs[binding] != 0; binding++) {
    printf("%s%s", binding == 0 ? "" : ", ", keyseqs[binding]);
  }
  printf("\n");  
}


int ridl_executeline(char *line, int flags) {
  int error = 0;

  // normal exit by hitting ^D
  if (line == NULL) { 
    printf("\n");
    return(IDL_Cleanup(IDL_FALSE)); 
  };
  
  // magic lines start with :, anything else is passed to IDL
  int firstcharIndex = ridl_firstchar(line);
  char firstchar = line[firstcharIndex];
  if (firstchar == ':') {
    if ((firstcharIndex + 1 < strlen(line)) 
          && (line[firstcharIndex + 1] == '!' || line[firstcharIndex + 1] == '^')) {
      // TODO: eventually this shouldn't be a magic command, but should be
      //       activated when a space is entered
      char *expansion;
      int expansion_result;
      char *expansion_line = (char *)malloc(strlen(line) + 1);
      strcpy(expansion_line, line + firstcharIndex + 1);
      expansion_result = history_expand(expansion_line, &expansion);
      switch (expansion_result) {
        case -1: 
          ridl_warning("error in expansion");
          break;
        case 2: 
          printf("%s\n", expansion);
          break;
        case 0: 
        case 1: 
          ridl_executestr(expansion, 1);
          break;
      }
      
      free(expansion_line);
      free(expansion);
    } else {   
      int search_length;                       
      char *cmd = ridl_getnextword(line, firstcharIndex, &search_length);
      if (strcmp(cmd, ":colors") == 0) {
        use_colors = use_colors ? 0 : 1;
        if (use_colors) {
          printf("colors on\n");
        } else {
          printf("colors off\n");
        }
      } else if (strcmp(cmd, ":doc") == 0) {
        int search_length;
        char *routine = ridl_getnextword(line, firstcharIndex + 5, &search_length);
        char *man = (char *)malloc(8 + strlen(routine));
        sprintf(man, "man, '%s'", routine);
        int error = IDL_ExecuteStr(man);
        free(man);
        free(routine);
      }  else if (strcmp(cmd, ":notebook") == 0) {
        if (ridl_isnotebooking()) ridl_closenotebook();

        int search_length;
        char *filename = ridl_getnextword(line, firstcharIndex + 9, &search_length);
        if (strlen(filename) == 0) {
          ridl_warning("must specify filename of notebook");
        } else {
          ridl_setnotebooking(1);
          ridl_initnotebook(filename);
        }
        free(filename);
      } else if (strcmp(cmd, ":pref") == 0) {
        int search_length;
        char *has_args = ridl_getnextword(line, firstcharIndex + 5, &search_length);

        if (strlen(has_args) == 0) {
          ridl_printpreferences();
        } else {
          char *pref_line = line + 6;
          ridl_process_pref_line(pref_line);          
        }
        free(has_args);        
      } else if (strcmp(cmd, ":log") == 0) {
        if (ridl_islogging()) ridl_closelog();

        int search_length;
        char *filename = ridl_getnextword(line, firstcharIndex + 4, &search_length);
        if (strlen(filename) == 0) {
          ridl_warning("must specify filename of log");
        } else {
          ridl_setlogging(1);
          ridl_initlog(filename);          
        }

        free(filename);
      } else if (strcmp(cmd, ":unnotebook") == 0) {
        if (ridl_isnotebooking()) {
          ridl_closenotebook();
          IDL_ToutPop();
          ridl_setnotebooking(0);
        } else ridl_warning("not currently notebooking");
      } else if (strcmp(cmd, ":unlog") == 0) {
        if (ridl_islogging()) {
          ridl_closelog();
          IDL_ToutPop();
          ridl_setlogging(0);
        } else ridl_warning("not currently logging");
      } else if (strcmp(cmd, ":tee") == 0) {
        if (ridl_isteeing()) ridl_closelog();

        int search_length;
        char *filename = ridl_getnextword(line, firstcharIndex + 4, &search_length);
        if (strlen(filename) == 0) {
        } else {
          ridl_setteeing(1);
          ridl_initlog(filename);
        }
        
        free(filename);
      } else if (strcmp(cmd, ":untee") == 0) {
        if (ridl_isteeing()) {
          ridl_closelog();
          IDL_ToutPop();
          ridl_setteeing(0);
        } else ridl_warning("not currently teeing");
      } else if (strcmp(cmd, ":save_graphic") == 0) {
        if (ridl_isnotebooking()) {
          ridl_notebookgraphic();
        } else {
          ridl_warning("turn on notebooking to save graphics");
        }
      } else if (strcmp(cmd, ":time") == 0) {
        char *command = line + 6;
        ridl_magic_time(command);
      } else if (strcmp(cmd, ":help") == 0) {
        int f;
        int display_keybindings = ridl_magic_help(line, firstcharIndex);

        if (display_keybindings) {
          printf("\nrIDL keybindings:\n");          
          for(f = 0; f < N_FUNMAP_FUNCTIONS; f++) {
            ridl_printfunmap(funmap_function_names[f], funmap_functions[f]);
          }
        }
      } else if (strcmp(cmd, ":history") == 0) {
        ridl_magic_history(line, firstcharIndex);
      } else if (strcmp(cmd, ":histedit") == 0) {
        ridl_magic_histedit(line, firstcharIndex);
      } else if (strcmp(cmd, ":version") == 0) {
        ridl_printversion();
      } else {
        ridl_warning("unknown magic command '%s'", cmd);
        error = 1;
      }
    }
  } else {
    if (line && *line) {      
      // check for .edit
      if (firstchar == '.') {
        int search_length;
        char *cmd = ridl_getnextword(line, firstcharIndex, &search_length);
        if (strcmp(cmd, ".edit") == 0 || strcmp(cmd, ".edi") == 0 
              || strcmp(cmd, ".ed") == 0 || strcmp(cmd, ".e") == 0) {
          char *file = ridl_getnextword(line, firstcharIndex + strlen(cmd) + 1, &search_length);
          ridl_launcheditor(file);              
          free(file);
          
          add_history(line);
          ridl_cmdnumber++;
          ridl_updateprompt();
        } else {
          // execute standard executive commands
          error = ridl_executestr(line, 1);
        }
        free(cmd);
      } else {
        int lastcharIndex = ridl_lastchar(line);
        char lastchar = lastcharIndex == -1 ? ' ' : line[lastcharIndex];
        if (lastchar == '?') {
          char cmd[1000];
          char varname[1000];
          strncpy(varname, line, lastcharIndex);
          varname[lastcharIndex] = '\0';
          sprintf(cmd, "ridl_varhelp, %s", varname);
          int status = IDL_ExecuteStr(cmd);
        } else {        
          // determine if line is continued
          char *line_continued = ridl_linecontinued(line);

          if (continued) {
            if (line_continued) {
              strcat(ridl_continuedline, line);
            } else {
              // execute IDL commands that were continued on several lines
              continued = 0;
              strcat(ridl_continuedline, line);
              error = ridl_executestr(ridl_continuedline, 1);
            }
          } else {
            if (line_continued) {
              continued = 1;
              strcpy(ridl_continuedline, line);
            } else {
              // execute normal IDL commands
              error = ridl_executestr(line, 1);
            }  
          }
        }
      }
    }
  }
  
  return(error);
}


int ridl_readline_callback(int cont, int widevint_allowed) {
  char *line;
  //printf("Before: %d, %d\n", cont, widevint_allowed);
  line = ridl_readline();
  //printf("After: %d, %d, %s\n", cont, widevint_allowed, line);

  return(line == 0 ? 0 : 1);
} 


void ridl_printridlversion(void) {
  printf("rIDL %s: Really Interactive Data Language. [Build: %s]\n", 
         RIDL_VERSION, RIDL_BUILD_DATE);
}


/**
   Print rIDL welcome message if not quiet.
*/
void ridl_welcome(void) {
  if (!(ridl_options & IDL_INIT_QUIET)) {
    ridl_printridlversion(); 
  }
}

            
/**
   Prints version information about rIDL and IDL to stdout.
*/
void ridl_printversion(void) {  
  ridl_printridlversion();
  printf("IDL Version %s, %s (%s %s m%d). [Build: %s]\n", 
         IDL_STRING_STR(&IDL_SysvVersion.release),
         IDL_STRING_STR(&IDL_SysvVersion.os_name),
         IDL_STRING_STR(&IDL_SysvVersion.os),
         IDL_STRING_STR(&IDL_SysvVersion.arch),
         IDL_SysvVersion.memory_bits,
         IDL_STRING_STR(&IDL_SysvVersion.build_date));
}


//
//   Print usage and options help for rIDL.
//
void ridl_printhelp(void) {
  char *indent = "  ";    // prefix for each line of switch help
  char *switch_format = "%s%-*s %s\n";
  int switch_width = 25;  // width of switch column in spaces
  
  ridl_printversion();
  
  printf("\nusage: ridl [options] [batch_filename]\n\n");

  printf("options:\n");
  printf(switch_format, indent, switch_width, "-32", 
         "start in 32-bit mode");
  printf(switch_format, indent, switch_width, "-arg value", 
         "saves a single command line argument");
  printf(switch_format, indent, switch_width, "-args value1 value2 ...", 
         "saves multiple command line arguments");
  printf(switch_format, indent, switch_width, "-demo", 
         "start in 7-minute demo mode");
  printf(switch_format, indent, switch_width, "-e command", 
         "executes command and exit");
  printf(switch_format, indent, switch_width, "-em=FILENAME", 
         "execute the given .sav file containing an embedded license");
  printf(switch_format, indent, switch_width, "-h", 
         "display this help message");
  printf(switch_format, indent, switch_width, "-IDL_PREF value", 
         "set the IDL_PREF preference for this session");         
  printf(switch_format, indent, switch_width, "-novm", 
         "use 7-minute demo mode instead of virtual machine with -rt");
  printf(switch_format, indent, switch_width, "-pref=FILENAME", 
         "load the given preference file");
  printf(switch_format, indent, switch_width, "-queue", 
         "wait for a license when none are available");
  printf(switch_format, indent, switch_width, "-quiet", 
         "suppress printing information about IDL on startup");
  printf(switch_format, indent, switch_width, "-rt=FILENAME", 
         "start with a runtime license on the given .sav file");
  printf(switch_format, indent, switch_width, "-student", 
         "start in student mode");
  printf(switch_format, indent, switch_width, "-ulicense", 
         "start IDL with a unique license");
  printf(switch_format, indent, switch_width, "-v", 
         "display version information");
  printf(switch_format, indent, switch_width, "-vm=FILENAME", 
         "start the virtual machine with the given .sav file");
}


void ridl_handleswitches(int argc, char *argv[], int before) {
  int a;
  char tmpcmd[RIDL_MAX_LINE_LENGTH];
  
  // first run through command line switches (some of them are needed when
  // initializing IDL)
  for (a = 1; a < argc; a++) {
    if (strcmp(argv[a], "-arg") == 0) {
      // TODO: handle
      a++;
    } else if (strcmp(argv[a], "-args") == 0) {
      // TODO: handle
      break;
    } else if (strcmp(argv[a], "-demo") == 0) {
      ridl_options |= IDL_INIT_DEMO;
    } else if (strcmp(argv[a], "-e") == 0) {
      if ((a + 1) < argc) {
        execute_cmd = 1;
        cmd = argv[++a];
      }
    } else if (strncmp(argv[a], "-em", 3) == 0) {
      ridl_options |= IDL_INIT_EMBEDDED;
      runtime_exec = 1;
      runtime_filename = argv[a] + 4;
    } else if (strcmp(argv[a], "-h") == 0) {
      ridl_printhelp();
      exit(EXIT_SUCCESS);
    } else if (strcmp(argv[a], "-novm") == 0) {
      ridl_options |= IDL_INIT_NOVM;
    } else if (strncmp(argv[a], "-pref", 5) == 0) {
      preferences_file_set = 1;
      preferences_filename = argv[a] + 6;
    } else if (strcmp(argv[a], "-quiet") == 0) {
      ridl_options |= IDL_INIT_QUIET;
    } else if (strcmp(argv[a], "-queue") == 0) {
      ridl_options |= IDL_INIT_LMQUEUE;
    } else if (strncmp(argv[a], "-rt", 3) == 0) {
      ridl_options |= IDL_INIT_RUNTIME;
      runtime_exec = 1;
      runtime_filename = argv[a] + 4;
    } else if (strcmp(argv[a], "-student") == 0) {
      ridl_options |= IDL_INIT_STUDENT;
    } else if (strcmp(argv[a], "-ulicense") == 0) {
      ridl_options |= IDL_INIT_NOLICALIAS;
    } else if (strcmp(argv[a], "-v") == 0) { 
      ridl_printversion();
      exit(EXIT_SUCCESS);
    } else if (strncmp(argv[a], "-vm", 3) == 0) {
      ridl_options |= IDL_INIT_VM;
      runtime_exec = 1;
      runtime_filename = argv[a] + 4;      
    } else if (strcmp(argv[a], "-32") == 0) {
      // need to accept -32, but don't need to do anything since this must be
      // handled by the launch script (if we have gotten here it is too late)
    } else if (argv[a][0] == '-') {
      // assuming that this is setting a preference
      if (before == 0) {
        ridl_setpreference(argv[a] + 1, argv[a + 1]);
      }
      a++;
    } else {
      execute_batch_file = 1;
      batch_file = argv[a];
    }
  }
  
}


/**
   rIDL main loop.
*/
int main(int argc, char *argv[]) {
  ridl_handleswitches(argc, argv, 1);
  
  IDL_INIT_DATA init_data;
  init_data.options = ridl_options;
  init_data.clargs.argc = argc;
  init_data.clargs.argv = argv;
  
  ridl_welcome();
  
  IDL_UicbRegInitTextDone(ridl_inittextdone);
                                               
  if (!IDL_Initialize(&init_data)) {
    ridl_warning("failed to initialize Callable IDL session.\n");
    return(EXIT_FAILURE);
  }
      
  IDL_ExitRegister(ridl_exit_handler);
  IDL_UicbRegExitDone(ridl_exit);
  IDL_UicbRegShowCompileErr(ridl_show_compile_error);
  IDL_UicbRegPromptChange(ridl_changeprompt);
  IDL_UicbRegDeathHint(ridl_deathhint);
  IDL_UicbRegWorkingDirChange(ridl_changewdir);
  
  // TODO: will need to use something like this to fix ticket #1, but this
  //       will crash now; this specifies a routine that will be called when
  //       IDL needs to do a readline (as in RIDL_STOPTEST)
  IDL_UicbRegRlineFromKbrd(ridl_readline_callback);

  ridl_handleswitches(argc, argv, 0);

  ridl_cmdnumber = ridl_initialize_history() + 1;
  
  if (!(msg_block = IDL_MessageDefineBlock("RIDL_MSG_BLOCK", 
                                           IDL_CARRAY_ELTS(msg_arr),  
                                           msg_arr))) return(1);
  
  IDL_GetUserInfo(&user_info);
  ridl_changewdir(user_info.wd);
  IDL_ExecuteStr("!prompt = !prompt");  // triggers prompt to be set

  rl_readline_name = "rIDL";
  ridl_completion_init();
  rl_attempted_completion_function = ridl_completion;
  ridl_get_userdefinedroutines_list();
  IDL_UicbRegPathChange(ridl_changepath); 
    
  ridl_read_preferences();
  
  // load -pref filename if present on the command line
  if (preferences_file_set) {
    // TODO: make this work (doing a PREF_SET will not change the preferences
    // values unless COMMIT is set, but doing so will change the user's 
    // preferences file permanently); how to change preferences just for this
    // session of IDL?
    
    //char *pref_set_format = "pref_set, filename='%s'";
    //char *pref_set_cmd = (char *)malloc(strlen(pref_set_format) - 2 + strlen(preferences_filename) + 1);
    //sprintf(pref_set_cmd, pref_set_format, preferences_filename);
    //int error = IDL_ExecuteStr(pref_set_cmd);
    //free(pref_set_cmd);
  }
  
  // handle -rt or -em options if one of them was present on the command line
  if (runtime_exec) {
    int error = IDL_RuntimeExec(runtime_filename);
    return(IDL_Cleanup(IDL_FALSE));
  }
  
  // handle -e option if it was present on the command line
  if (execute_cmd) {
    int error = IDL_ExecuteStr(cmd);
    return(IDL_Cleanup(IDL_FALSE));
  }
  
  // execute batch file if present on the command line
  if (execute_batch_file) {
    char *batch_cmd = (char *)malloc(strlen(batch_file) + 2);
    sprintf(batch_cmd, "@%s", batch_file);
    // TODO: should actually loop through the lines of the file and send each 
    // one to "ridl_executeline(line, 0)"
    int error = IDL_ExecuteStr(batch_cmd);
    free(batch_cmd);
  }
  
  ridl_updateprompt();
  
  rl_event_hook = ridl_event_hook;
  
  // bind these commands to keys in an Readline configuration file, i.e., 
  // like ~/.inputrc (an example inputrc is given in the src/ directory of the
  // distribution)
  int f;
  for (f = 0; f < N_FUNMAP_FUNCTIONS; f++) {
    rl_add_funmap_entry(funmap_function_names[f], funmap_functions[f]);
  }

  while (1) {
    char *line = ridl_readline();

    ridl_getevents();
    
    int error = ridl_executeline(line, 1);
        
    free(line);
  }    
  
  return(EXIT_FAILURE);
}
