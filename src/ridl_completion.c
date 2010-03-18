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

IDL_VPTR structure_fields;
char *current_struct;


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
  int status = IDL_ExecuteStr("_ridl_localvars = strlowcase(scope_varname())");
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

  while (list_index < nlocals) {
    name = IDL_STRING_STR(&s[list_index]);

    list_index++;
    if (strncasecmp(name, text, len) == 0) {
      return(ridl_copystr(name));
    }
  }
    
  return((char *)NULL);
}


void ridl_get_structurefields_list(char *varname) {
  char cmd[1000];
  int status;

  // construct command to get field names
  sprintf(cmd, "_ridl_structurefields = ridl_getfields(%s)", varname);
  status = IDL_ExecuteStr(cmd);
  structure_fields = IDL_FindNamedVariable("_ridl_structurefields", 0);
}


void ridl_remove_structurefields_list(void) {
  int status = IDL_ExecuteStr("delvar, _ridl_structurefields");
}


char *ridl_structurefield_generator(const char *varname, int state) {
  static int list_index, len;  
  int nfields = (int) (*structure_fields->value.arr).n_elts;
  int i;
  IDL_STRING *s = (IDL_STRING *)structure_fields->value.arr->data;
  char *name;
  
  // state == 0 the first time this is called, non-zero on subsequent calls
  if (!state) {
    list_index = 0;
    len = strlen(varname);
  }

  while (list_index < nfields) {
    name = IDL_STRING_STR(&s[list_index]);

    list_index++;
    if (strncasecmp(name, varname, len) == 0) {
      char *struct_and_field = (char *)malloc(strlen(current_struct) + strlen(name) + 1 + 1);
      strcpy(struct_and_field, current_struct);
      strcpy(struct_and_field + strlen(current_struct), ".");      
      strcpy(struct_and_field + strlen(current_struct) + 1, name);
      struct_and_field[strlen(current_struct) + strlen(name) + 1] = '\0';
      return(struct_and_field);
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
  
  // check for structure fields
  if (matches == (char **)NULL) {
    char *dotpos = strstr(text, ".");
    if (dotpos != (char *)NULL) {
      int varlen = (int) (dotpos - text);
      char *varname = (char *)malloc(varlen + 1);
      strncpy(varname, text, varlen);
      varname[varlen] = '\0';
      current_struct = varname;
      ridl_get_structurefields_list(varname);
      matches = rl_completion_matches(dotpos + 1, ridl_structurefield_generator);
      ridl_remove_structurefields_list();
      free(varname);
    }
  }
  
  // TODO: check for routine names if nothing found yet
  if (matches == (char **)NULL) {
    
  }
  
  return(matches);
}
