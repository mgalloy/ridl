#include <stdio.h>  
#include <stdlib.h> 
#include <signal.h>
#include <string.h>
#include <time.h>

#include "idl_export.h" 
#include "readline/readline.h"
#include "readline/history.h"

#include "ridl.h"
#include "ridl_magic.h"
#include "ridl_history.h"


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

static int logging = 0;
static int teeing = 0;
static char *log_file;
static FILE *log_fp;

static int use_colors = 1;

static IDL_MSG_DEF msg_arr[] = {  
#define M_RIDL_SIGNAL_REG       0
  {  "M_RIDL_SIGNAL_REG",   "%NSignal registration problem." }, 
};
static IDL_MSG_BLOCK msg_block; 

int ridl_really_exit = 1;


/*
  char *s = (char *)malloc(1024);
  char *t = (char *)malloc(1024);
  strcpy(t, "Hello my name is %name and I live in %city!\n");
  ridl_replacestr(s, t, "name", "Mike");
  printf("%s", s);
 
  strcpy(t, s);
  ridl_replacestr(s, t, "city", "Boulder");
  printf("%s", s);
*/
int ridl_replacestr(char *result, char *text, char *name, char *value) {
  char *pos, *original_name, *original_value;
  int i, matches, matched = 0;
  
  original_name = name;
  original_value = value;
  
  while (*text) {
    if (*text == '%') {
      pos = text;
      text++;
      
      matches = 1;
      for (name = original_name; *name; name++, text++) {
        if (*name != *text) {
          matches = 0;
          break;
        }
      }

      if (matches) {
        matched = 1;
        while (*result++ = *value++) 
          ;
        *result--;   // remove the null
        value = original_value;
      } else {
        *result++ = *pos;
      }
    } else {
      *result = *text;
      text++;
      result++;
    }
  }
  *result = '\0';
  
  return(matched);
}


/*
   Called when the initial IDL startup text has finished printing.
*/
void ridl_inittextdone(void) {
  
}


// mallocs return value, caller should free
char *ridl_command_generator(const char *text, int state) {
  if (!state) {
    char *result = (char *)malloc(6);
    strcpy(result, "print");
    return(result);
  } else { 
    return((char *)NULL);
  }
}


char **ridl_completion(const char *text, int start, int end) {
  char **matches = (char **) NULL;
  // determines state and calls the correct generator
    
  //matches = rl_completion_matches(text, ridl_command_generator);
  //return(matches);
  
  //printf("\ntext = '%s', start = %d, end = %d\n", text, start, end);
  return((char **) NULL);
}


void ridl_logoutput(int flags, char *buf, int n) {
  char *output = (char *)malloc(strlen(buf) + 1);
  strncpy(output, buf, n);
  output[n] = '\0';
  
  printf("%s", output);
  if (flags & IDL_TOUT_F_NLPOST) printf("\n");

  fprintf(log_fp, "%s", output);
  if (flags & IDL_TOUT_F_NLPOST) fprintf(log_fp, "\n");
  
  free(output);
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


/*
   Registered to be called when the !prompt changes.
*/
void ridl_changeprompt(IDL_STRING *prompt) {
  ridl_prompt = IDL_STRING_STR(prompt);
  ridl_updateprompt();
}


/* 
   Registered to be called when the current working directory changes.
*/
void ridl_changewdir(char *dir) {
  strcpy(ridl_current_wdir, dir);
  ridl_updateprompt();
}


char *ridl_currenttimestamp(void) {
  char *timestamp = (char *)malloc(25);
  char *monthname[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  struct tm *stime;
  char *date; 
  time_t timer;
  timer = time(NULL);
  stime = localtime(&timer);
  date = asctime(stime);
  
  sprintf(timestamp, "%2d-%s-%4d %2d:%02d:%05.2f", 
          stime->tm_mday,
          monthname[stime->tm_mon],
          1900 + stime->tm_year,
          stime->tm_hour,
          stime->tm_min,
          (float) stime->tm_sec);
  return(timestamp);
}


int ridl_file_exists(const char *filename) {
  FILE *file;
  if (file = fopen(filename, "r")) {
    fclose(file);
    return(1);
  }
  return(0);
}


void ridl_printsource(void) {
  if (IDL_DebugGetStackDepth() > 1) {
    int result = IDL_ExecuteStr("ridl_printsource");
  }
}


int ridl_stepinto(void) {
  char *cmd = ".step";
  printf("%s\n", cmd);  
  int status = ridl_executestr(cmd);
  ridl_cmdnumber++;
  ridl_updateprompt();
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


int ridl_stepover(void) {
  char *cmd = ".stepover";
  printf("%s\n", cmd);  
  int status = ridl_executestr(cmd);
  ridl_cmdnumber++;
  ridl_updateprompt();
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


int ridl_stepreturn(void) {
  char *cmd = ".return";
  printf("%s\n", cmd);   
  int status = ridl_executestr(cmd);
  ridl_cmdnumber++;
  ridl_updateprompt();
  ridl_printsource();  
  printf("%s", ridl_expandedprompt);
}


/*
   Execute an IDL command typed at the command line by the user.
*/
int ridl_executestr(char *cmd) {
  int result;
  
  if (logging) fprintf(log_fp, "%s%s\n", ridl_expandedprompt, cmd);

  result = IDL_ExecuteStr(cmd);
  
  // add line to both Readline's history and IDL's history file
  ridl_addhistoryline(cmd);

  // update prompt
  ridl_updateprompt();

  // reset colors if there was a compile error
  if (use_colors) fprintf(stderr, "\e[0m");
  
  return(result);
}


//
// Registered to be called before a compiler error is shown.
//
void ridl_show_compile_error(void) {
  if (use_colors) fprintf(stderr, "\e[31m");  // will show compiler errors in red
}


/*
   Registered to be called after IDL_Cleanup is called. 
*/
void ridl_deathhint(void) {
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
  if (teeing) fclose(log_fp);

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


void ridl_getevents() {
  char *cmd = "ridl_getevents";
  int result = IDL_ExecuteStr(cmd);
}


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
  // print source code line if we are not at the main level
  ridl_printsource();
  
  // prompt changes depending on if this is a continuation line
  if (continued) {
    char *line = readline(ridl_expandedprompt2); 
  } else {
    char *line = readline(ridl_expandedprompt);  
  }
}


int ridl_readline_callback(int cont, int  widevint_allowed) {
  char *line;
  printf("Before: %d, %d\n", cont, widevint_allowed);
  line = ridl_readline();
  printf("After: %d, %d, %s\n", cont, widevint_allowed, line);

  return(line == 0 ? 0 : 1);
}


void ridl_magic_history(char *line, int firstcharIndex, int length, int printLineNum) {
  char *nstr = ridl_getnextword(line, firstcharIndex + length);
  int i, n = 10; 
  
  HIST_ENTRY *h;
  HISTORY_STATE *hs;
  if (strlen(nstr) > 0) n = atoi(nstr);
  hs = history_get_history_state();
  for (i = n - 1; i >= 0; i--) {
    h = history_get(hs->offset - i); 
    if (printLineNum) {
      printf("[%d]: %s\n", hs->offset - i, h == 0 ? "" : h->line);
    } else {
      printf("%s\n", h == 0 ? "" : h->line);
    }
  }
  free(nstr);
}  


void ridl_magic_histedit(char *line, int firstcharIndex) {
  HIST_ENTRY *h;
  HISTORY_STATE *hs;
  char *snlines = ridl_getnextword(line, firstcharIndex + 10);
  char *filename = ridl_getnextword(line, firstcharIndex + 10 + strlen(snlines) + 1);
  int i, nlines = atoi(snlines);
  
  if (ridl_file_exists(filename)) {
    printf("File %s already exists\n", filename);
    return;
  }
  
  FILE *fp = fopen(filename, "w");
  
  hs = history_get_history_state();
  for (i = nlines - 1; i >= 0; i--) {
    h = history_get(hs->offset - i); 
    fprintf(fp, "%s\n", h == 0 ? "" : h->line);
  }
  free(snlines);
  free(filename);
  fclose(fp);
  
  ridl_launcheditor(filename); 
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
    } else if (strcmp(argv[a], "-args") == 0) {
      // TODO: handle
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
        // TODO: convert each preference name to a system variable name
        if (strcmp(argv[a] + 1, "IDL_PROMPT") == 0) {
          char space[2];
          space[0] = argv[a + 1][strlen(argv[a + 1]) - 1] == ' ' ? '\0' : ' ';
          space[1] = '\0';
          sprintf(tmpcmd, "!prompt = '%s%s'", argv[a + 1], space);
          IDL_ExecuteStr(tmpcmd);
        }
      }
      a++;
    } else {
      execute_batch_file = 1;
      batch_file = argv[a];
    }
  }
  
}


//
//   rIDL main loop.
//
int main(int argc, char *argv[]) { 

  ridl_handleswitches(argc, argv, 1);
  
  IDL_INIT_DATA init_data;
  init_data.options = ridl_options;
  init_data.clargs.argc = argc;
  init_data.clargs.argv = argv;
  
  ridl_welcome();
  
  IDL_UicbRegInitTextDone(ridl_inittextdone);
                                               
  if (!IDL_Initialize(&init_data)) {
    printf("Failed to initialize Callable IDL session.\n");
    return(EXIT_FAILURE);
  }
      
  IDL_ExitRegister(ridl_exit_handler);
  IDL_UicbRegExitDone(ridl_exit);
  IDL_UicbRegShowCompileErr(ridl_show_compile_error);
  IDL_UicbRegPromptChange(ridl_changeprompt);
  IDL_UicbRegWorkingDirChange(ridl_changewdir);
  IDL_UicbRegDeathHint(ridl_deathhint);

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

  strcpy(ridl_current_wdir, user_info.wd);
  IDL_ExecuteStr("!prompt = !prompt");  // triggers prompt to be set

  rl_attempted_completion_function = ridl_completion;
  
  // TODO: add completers for routines and variables completion
  
  // load -pref filename if present on the command line
  if (preferences_file_set) {
    // TODO: make this work (doing a PREF_SET will not change the preferences
    // values unless COMMIT is set, but doing so will change the user's 
    // preferences file permanently); how to change preferences just for this
    // session of IDL?
    
    //char *pref_set_format = "pref_set, filename='%s'";
    //char *pref_set_cmd = (char *)malloc(strlen(pref_set_format) - 2 + strlen(preferences_filename) + 1);
    ///sprintf(pref_set_cmd, pref_set_format, preferences_filename);
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
    int error = IDL_ExecuteStr(batch_cmd);
    free(batch_cmd);
  }
  
  ridl_updateprompt();
  
  rl_event_hook = ridl_event_hook;
  
  // bind these commands to keys in an Readline configuration file, i.e., 
  // like ~/.inputrc (an example inputrc is given in the src/ directory of the
  // distribution)
  rl_add_funmap_entry("ridl-stepinto", (rl_command_func_t *)ridl_stepinto);
  rl_add_funmap_entry("ridl-stepover", (rl_command_func_t *)ridl_stepover);
  rl_add_funmap_entry("ridl-stepreturn", (rl_command_func_t *)ridl_stepreturn);

  while (1) {
    char *line = ridl_readline();
    ridl_getevents();
    
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
            printf("Error in expansion\n");
            break;
          case 2: 
            printf("%s\n", expansion);
            break;
          case 0: 
          case 1: 
            ridl_executestr(expansion);
            break;
        }
        ridl_cmdnumber++;
        ridl_updateprompt();
        
        free(expansion_line);
        free(expansion);
      } else {                          
        char *cmd = ridl_getnextword(line, firstcharIndex);
        if (strcmp(cmd, ":colors") == 0) {
          use_colors = use_colors ? 0 : 1;
          if (use_colors) {
            printf("colors on\n");
          } else {
            printf("colors off\n");
          }
        } else if (strcmp(cmd, ":doc") == 0) {
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
        } else if (strcmp(cmd, ":tee") == 0) {
          if (teeing) fclose(log_fp);

          teeing = 1;
          char *filename = ridl_getnextword(line, firstcharIndex + 5);
          log_fp = fopen(filename, "w");
          IDL_ToutPush(ridl_logoutput);
          free(filename);
        } else if (strcmp(cmd, ":untee") == 0) {
          if (teeing) {
            fclose(log_fp);
            IDL_ToutPop();
          }
          teeing = 0;          
        } else if (strcmp(cmd, ":help") == 0) {
          ridl_printmagichelp();
        } else if (strcmp(cmd, ":history") == 0) {
          ridl_magic_history(line, firstcharIndex, 9, 1);
        } else if (strcmp(cmd, ":qhistory") == 0) {
          ridl_magic_history(line, firstcharIndex, 10, 0);
        } else if (strcmp(cmd, ":histedit") == 0) {
          ridl_magic_histedit(line, firstcharIndex);
        }
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
            ridl_cmdnumber++;
            ridl_updateprompt();
          } else {
            // execute standard executive commands
            int error = ridl_executestr(line);
            ridl_cmdnumber++;
            ridl_updateprompt();            
          }
          free(cmd);
        } else {
          // determine if line is continued
          char *line_continued = ridl_linecontinued(line);

          if (continued) {
            if (line_continued) {
              strcat(ridl_continuedline, line);
            } else {
              // execute IDL commands that were continued on several lines
              int error;
              continued = 0;
              strcat(ridl_continuedline, line);
              error = ridl_executestr(ridl_continuedline);
              ridl_cmdnumber++;
              ridl_updateprompt();
            }
          } else {
            if (line_continued) {
              continued = 1;
              strcpy(ridl_continuedline, line);
            } else {
              // execute normal IDL commands
              int error = ridl_executestr(line);
              ridl_cmdnumber++;
              ridl_updateprompt();
            }  
          }
        }
      }
    }
    
    free(line);
  }    
  
  return(EXIT_FAILURE);
}
