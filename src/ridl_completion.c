#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "idl_export.h" 
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

IDL_VPTR local_variables;


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


void ridl_get_localvariables_list(void) {
  int status = IDL_ExecuteStr("_ridl_localvars = scope_varname()");
   local_variables = IDL_FindNamedVariable("_ridl_localvars", 0);
}


void ridl_remove_localvariables_list(void) {
  int status = IDL_ExecuteStr("delvar, _ridl_localvars");
}

char *ridl_localvariable_generator(const char *text, int state) {
  static int list_index, len;  
  int nlocals = (int) (*local_variables->value.arr).n_elts;
  int i;
  IDL_STRING *s = (IDL_STRING *)local_variables->value.arr->data;
  char *name;
  
  // state == 0 the first time this is called, non-zero on subsequent calls
  if (!state) {
    list_index = 0;
    len = strlen(text);
  }
    
  //printf("\nnumber of local variables = %d\n", nlocals);
  
  //for (i = 0; i < nlocals; i++) {
  //  printf("  %s\n", IDL_STRING_STR(&s[i]));
  //}

  while (list_index < nlocals) {
    name = IDL_STRING_STR(&s[list_index]);
    //printf("checking against %s...\n", name);
    list_index++;
    if (strncmp(name, text, len) == 0) {
      return(ridl_copystr(name));
    }
  }
    
  return((char *)NULL);
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

  rl_completion_append_character = '\0';
  
  // check for system variables first
  matches = rl_completion_matches(text, ridl_systemvariable_generator);

  // check for local variables if no system variables found
  if (matches == (char **)NULL) {
    ridl_get_localvariables_list();
    matches = rl_completion_matches(text, ridl_localvariable_generator);
    ridl_remove_localvariables_list();
  }
  
  // TODO: check for structure fields
  
  // TODO: check for routine names if nothing found yet
  if (matches == (char **)NULL) {
    
  }
  
  return(matches);
}
