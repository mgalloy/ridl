#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "readline/readline.h"

#include "ridl_strings.h"


/// TODO: system variables to complete on
char *system_variables[] = { 
  "!c", "!cpu", "!d", "!dir", "!dlm_path", "!dpi", "!dtor",  "!edit_input", 
  "!err", "!error_state", "!except", "!help_path", "!journal", "!make_dll",
  "!map", "!more", "!mouse", "!order", "!p", "!path", "!pi", "!prompt", 
  "!quiet", "!radeg", "!values", "!version", "!warn", "!x", "!y", "!z",
  (char *)NULL
};


/* Look up NAME as the name of a command, and return a pointer to that 
   command. Return a NULL pointer if NAME isn’t a command name. 
*/
char **ridl_find_systemvariable(char *name) {
  register int i;
  
  for (i = 0; system_variables[i]; i++) {
    if (strcmp(name, system_variables[i]) == 0) {
      return(&system_variables[i]); 
    }
  }
  
  return ((char **)NULL);
}


/**
   mallocs return value, caller should free; used in Readline completion
   callback
   
   @return dynamically allocated string
   
   @param[in] text string to complete on
   @param[in] state 0 for the first call on an attempted completion, non-zero
                    for subsequent calls
*/
char *ridl_systemvariable_generator(const char *text, int state) {
  static int list_index, len;
  char *name;
  
  // state == 0 the first time this is called, non-zero on subsequent calls
  if (!state) {
    list_index = 0;
    len = strlen(text);
  }
  
  while (name = system_variables[list_index]) {
    list_index++;
    if (strncmp(name, text, len) == 0) {
      return(ridl_copystr(name));
    }
  }
  
  return((char *)NULL);
}


char **ridl_completion(const char *text, int start, int end) {
  char **matches = (char **)NULL;
  int loc;
  
  //printf("\ntext = %s, start = %d, end = %d\n", text, start, end);
  
  // check for system variables first
  rl_completion_append_character = '\0';
  matches = rl_completion_matches(text, ridl_systemvariable_generator);
  
  return(matches);
}
