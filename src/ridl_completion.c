#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "idl_export.h" 
#include "readline/readline.h"

#include "ridl.h"
#include "ridl_strings.h"
#include "ridl_config.h"


/// list of system variables
char *system_variables[] = {
  "!c", "!color", "!const", "!cpu", "!d", "!dir", "!dlm_path", "!dpi", "!dtor",
  "!edit_input",
  "!err", "!error_state", "!except", "!help_path", "!journal", "!make_dll",
  "!map", "!more", "!mouse", "!null", "!order", "!p", "!path", "!pi", "!prompt",
  "!quiet", "!radeg", "!values", "!version", "!warn", "!x", "!y", "!z",
  (char *)NULL
};

/// list of reserved words for IDL language
char *reserved_words[] = {
  "begin", "break", "case", "common", "compile_opt", "continue", "do", "else",
  "end", "endcase", "endelse", "endif", "endfor", "endforeach", "endrep",
  "endswitch", "endwhile",
  "for", "foreach", "forward_function", "function", "goto", "inherits",
  "of", "on_ierror", "pro", "repeat", "return", "switch", "then", "while",
  "and", "eq", "ge", "gt", "lt", "le",  "mod", "ne", "not",  "or", "xor",
  (char *)NULL
};

/// list of executive commands
char *executive_cmds[] = {
  ".compile", ".continue", ".edit", ".full_reset_session", ".go", ".out",
  ".reset_session", ".return", ".rnew", ".run", ".skip", ".step", ".stepover",
  ".trace",
  (char *)NULL
};

/// list of magic commands
char *magic_cmds[] = {
  ":colors", ":doc", ":help", ":history", ":histedit", ":log", ":unlog",
  ":notebook", ":unnotebook", ":save_graphic", ":tee", ":untee", ":time",
  ":version",
  (char *)NULL
};

/// determines if the catalog of IDL library routines was found
int idl_routines_available = 1;
int idl_classes_available = 1;

/// list of IDL library routines
char *idl_routines[1540];
char *idl_classes[75];

/// IDL array of local variable names
IDL_VPTR local_variables;

/// IDL array of currently defined routine names
IDL_VPTR routine_names;

/// routine names in correctly named files in !path
IDL_VPTR userdefined_routine_names;
char **userdefined_routines;
int nuserdefined_routines = 0;

/// routine names in correctly named files in current directory
IDL_VPTR currentdir_userdefined_routine_names;
char **currentdir_userdefined_routines;
int ncurrentdir_userdefined_routines = 0;

/// IDL array of structure field names
IDL_VPTR structure_fields;

/// name of current structure to check field names of
char *current_struct;

/// IDL array of method names
IDL_VPTR method_names;

/// name of current object to check method names of
char *current_obj;


void ridl_get_userdefinedroutines_list() {
  char *cmd = "_ridl_userdefined_routine_names = ridl_getuserroutines(n_routines=_ridl_nuserdefined_routine_names)";
  int status, r;
  IDL_STRING *s;
  IDL_VPTR nuserdefined_routine_names;
  
  status = IDL_ExecuteStr(cmd);
  userdefined_routine_names = IDL_FindNamedVariable("_ridl_userdefined_routine_names", 0);
  nuserdefined_routine_names = IDL_FindNamedVariable("_ridl_nuserdefined_routine_names", 0);

  if (nuserdefined_routines > 0) {
    for (r = 0; r < nuserdefined_routines; r++) {
      free(userdefined_routines[r]);
    }
    free(userdefined_routines);
  }

  nuserdefined_routines = (int) (nuserdefined_routine_names->value.l);
  if (nuserdefined_routines) {
    userdefined_routines = (char **) malloc(nuserdefined_routines * sizeof(char *));
    s = (IDL_STRING *) userdefined_routine_names->value.arr->data;
  
    for (r = 0; r < nuserdefined_routines; r++) {
      userdefined_routines[r] = ridl_copystr((*s++).s);
    }
  }
  
  // clear IDL variable
  status = IDL_ExecuteStr("delvar, _ridl_userdefined_routine_names, _ridl_nuserdefined_routine_names");
}


void ridl_get_currentdir_userdefinedroutines_list() {
  char *cmd = "_ridl_userdefined_routine_names = ridl_getuserroutines(/current_directory, n_routines=_ridl_nuserdefined_routine_names)";
  int status, r;
  IDL_STRING *s;
  IDL_VPTR nuserdefined_routine_names;
  
  status = IDL_ExecuteStr(cmd);
  currentdir_userdefined_routine_names = IDL_FindNamedVariable("_ridl_userdefined_routine_names", 0);  
  nuserdefined_routine_names = IDL_FindNamedVariable("_ridl_nuserdefined_routine_names", 0);

  if (ncurrentdir_userdefined_routines > 0) {
    for (r = 0; r < ncurrentdir_userdefined_routines; r++) {
      free(currentdir_userdefined_routines[r]);
    }
    free(currentdir_userdefined_routines);
  }
  
  ncurrentdir_userdefined_routines = (int) (nuserdefined_routine_names->value.l);
  if (ncurrentdir_userdefined_routines > 0) {
    currentdir_userdefined_routines = (char **) malloc(ncurrentdir_userdefined_routines * sizeof(char *));
    s = (IDL_STRING *) currentdir_userdefined_routine_names->value.arr->data;

    for (r = 0; r < ncurrentdir_userdefined_routines; r++) {
      currentdir_userdefined_routines[r] = ridl_copystr((*s++).s);
    }
  }
  
  // clear IDL variable
  status = IDL_ExecuteStr("delvar, _ridl_userdefined_routine_names, _ridl_nuserdefined_routine_names");
}


/**
   Find field names of a structure.
   
   @param[in] varname name of structure variable to find field names of
*/
void ridl_get_structurefields_list(char *varname) {
  char cmd[1000];
  int status;

  // construct command to get field names
  sprintf(cmd, "_ridl_structurefields = ridl_getfields(%s)", varname);
  status = IDL_ExecuteStr(cmd);
  structure_fields = IDL_FindNamedVariable("_ridl_structurefields", 0);
}


/**
   Free IDL array variable containing name of structure variables field.
*/
void ridl_remove_structurefields_list(void) {
  int status = IDL_ExecuteStr("delvar, _ridl_structurefields");
}


char *ridl_structurefield_generator(const char *varname, int state) {
  static int list_index, len;
  int nfields = (int) (*structure_fields->value.arr).n_elts;
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
   Find method names for an object.
   
   @param[in] varname name of object variable to find field names of
*/
void ridl_get_methodnames_list(char *varname) {
  char cmd[1000];
  int status;

  // construct command to get field names
  sprintf(cmd, "_ridl_methodnames = ridl_getmethods(%s)", varname);
  status = IDL_ExecuteStr(cmd);
  method_names = IDL_FindNamedVariable("_ridl_methodnames", 0);
}


/**
   Free IDL array variable containing name of object method names.
*/
void ridl_remove_methodnames_list(void) {
  int status = IDL_ExecuteStr("delvar, _ridl_methodnames");
}


char *ridl_methodname_generator(const char *varname, int state) {
  static int list_index, len;
  int nnames = (int) (*method_names->value.arr).n_elts;
  IDL_STRING *s = (IDL_STRING *)method_names->value.arr->data;
  char *name;

  // state == 0 the first time this is called, non-zero on subsequent calls
  if (!state) {
    list_index = 0;
    len = strlen(varname);
  }

  while (list_index < nnames) {
    name = IDL_STRING_STR(&s[list_index]);

    list_index++;
    if (strncasecmp(name, varname, len) == 0) {
      char *obj_and_method = (char *)malloc(strlen(current_obj) + strlen(name) + 2 + 1);
      strcpy(obj_and_method, current_obj);
      strcpy(obj_and_method + strlen(current_obj), "->");
      strcpy(obj_and_method + strlen(current_obj) + 2, name);
      obj_and_method[strlen(current_obj) + strlen(name) + 2] = '\0';
      return(obj_and_method);
    }
  }

  return((char *)NULL);
}


void ridl_get_localvariables_list(void) {
  int status = IDL_ExecuteStr("_ridl_localvars = strlowcase(scope_varname())");
  local_variables = IDL_FindNamedVariable("_ridl_localvars", 0);
}


void ridl_remove_localvariables_list(void) {
  int status = IDL_ExecuteStr("delvar, _ridl_localvars");
}


void ridl_get_routinenames_list(void) {
  int status = IDL_ExecuteStr("_ridl_routinenames = ridl_getroutines()");
  routine_names = IDL_FindNamedVariable("_ridl_routinenames", 0);
}


void ridl_remove_routinenames_list(void) {
  int status = IDL_ExecuteStr("delvar, _ridl_routinenames");
}


/**
   mallocs return value, caller should free; used in Readline completion
   callback
   
   @return dynamically allocated string
   
   @param[in] text string to complete on
   @param[in] state 0 for the first call on an attempted completion, non-zero
                    for subsequent calls
*/
char *ridl_generator(const char *text, int state) {
  static int list_index, len, nlocals, nrnames, nudrnames;
  static IDL_STRING *locals;
  static IDL_STRING *rnames, *udrnames;
  
  static int processed_reservedwords;
  static int processed_systemvariables;
  static int processed_executivecmds;
  static int processed_magiccmds;
  static int processed_idlroutines;
  static int processed_idlclasses;
  static int processed_localvariables;
  static int processed_routinenames;
  static int processed_currentdir_userdefined_routines;
  static int processed_userdefined_routines;
  
  char *name;
  
  // state == 0 the first time this is called, non-zero on subsequent calls
  if (!state) {
    list_index = 0;
    len = strlen(text);
    
    processed_reservedwords = 0;
    processed_systemvariables = 0;
    processed_executivecmds = 0;
    processed_magiccmds = 0;
    processed_idlroutines = 0;
    processed_idlclasses = 0;
    processed_localvariables = 0;
    processed_routinenames = 0;
    processed_currentdir_userdefined_routines = 0;
    processed_userdefined_routines = 0;
  }

  if (!processed_reservedwords) {
    while ((name = reserved_words[list_index])) {
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_reservedwords) {
    //printf("processed reserved words...\n");
    processed_reservedwords = 1;
    list_index = 0;
  }


  if (!processed_systemvariables) {
    while ((name = system_variables[list_index])) {
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_systemvariables) {
    //printf("processed system variables...\n");
    processed_systemvariables = 1;
    list_index = 0;
  }


  if (!processed_executivecmds) {
    while ((name = executive_cmds[list_index])) {
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_executivecmds) {
    //printf("processed executive commands...\n");
    processed_executivecmds = 1;
    list_index = 0;
  }


  if (!processed_magiccmds) {
    while ((name = magic_cmds[list_index])) {
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_magiccmds) {
    //printf("processed magic commands...\n");
    processed_magiccmds = 1;
    list_index = 0;
  }
  
  
  if (!processed_idlroutines) {
    while ((name = idl_routines[list_index])) {
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_idlroutines) {
    //printf("processed IDL routines...\n");
    processed_idlroutines = 1;
    list_index = 0;
  }


  if (!processed_idlclasses) {
    while ((name = idl_classes[list_index])) {
      list_index++;
      if (strncasecmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_idlclasses) {
    //printf("processed IDL classes...\n");
    processed_idlclasses = 1;
    list_index = 0;
  }


  if (!processed_localvariables) {
    ridl_get_localvariables_list();
    nlocals = (int) (*local_variables->value.arr).n_elts;
    locals = (IDL_STRING *)local_variables->value.arr->data;
  }
  
  if (!processed_localvariables) {
    while (list_index < nlocals) {
      name = IDL_STRING_STR(&locals[list_index]);

      list_index++;
      if (strncasecmp(name, text, len) == 0 && strcasecmp(name, "_ridl_localvars") != 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_localvariables) {
    //printf("processed local variables...\n");
    ridl_remove_localvariables_list();
    processed_localvariables = 1;
    list_index = 0;
  }


  if (!processed_routinenames) {
    ridl_get_routinenames_list();
    nrnames = (int) (*routine_names->value.arr).n_elts;
    rnames = (IDL_STRING *)routine_names->value.arr->data;
  }
  
  if (!processed_routinenames) {
    while (list_index < nrnames) {
      name = IDL_STRING_STR(&rnames[list_index]);

      list_index++;
      if (strncasecmp(name, text, len) == 0 && strcasecmp(name, "_ridl_routinenames") != 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_routinenames) {
    //printf("processed routine names...\n");
    ridl_remove_routinenames_list();
    processed_routinenames = 1;
    list_index = 0;
  }

  if (!processed_currentdir_userdefined_routines) {
    while (list_index < ncurrentdir_userdefined_routines) {
      name = currentdir_userdefined_routines[list_index];
      
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_currentdir_userdefined_routines) {
    //printf("processed user defined routines...\n");
    processed_currentdir_userdefined_routines = 1;
    list_index = 0;
  }
  
  
  if (!processed_userdefined_routines) {
    while (list_index < nuserdefined_routines) {
      name = userdefined_routines[list_index];
      
      list_index++;
      if (strncmp(name, text, len) == 0) {
        return(ridl_copystr(name));
      }
    }
  }

  if (!processed_userdefined_routines) {
    //printf("processed user defined routines...\n");
    processed_userdefined_routines = 1;
    list_index = 0;
  }
   
  return((char *)NULL);
}


int ridl_instring(char *line, int start) {
  int in_single = 0, in_double = 0, i;
  
  for (i = 0; i < start; i++) {
    if (line[i] == '\'') {
      // if not in double quotes, switch single quotes var
      if (!in_double) in_single = 1 - in_single;
    }
    
    if (line[i] == '\"') {
      // if not in single quotes, switch double quotes var
      if (!in_single) in_double = 1 - in_double;
    }
  }
  
  return(in_single || in_double);
}


/**
   Readline attempted completion routine.
   
   @param[in] text current word to base completion on
   @param[in] start start position for `text` within the entire line
   @param[in] end end position for `text` within the entire line
*/
char **ridl_completion(const char *text, int start, int end) {
  char **matches = (char **)NULL;
  char *svarname, *ovarname;
  int svarlen, ovarlen;
  int loc, in_string;
  
  //printf("\ntext = %s, start = %d, end = %d\n", rl_line_buffer, start, end);
  //printf("\ntext = %s, start = %d, end = %d\n", text, start, end);
  //printf("\nline so far = '%s'\n", rl_line_buffer);
  //printf("end of string = '%d'\n", text[end]);
  
  // if no text, quit
  if (strlen(text) == 0) return(matches);
  
  in_string = ridl_instring(rl_line_buffer, start);
  rl_completion_append_character = '\0';

  // check for reserved words, system variables, executive commands, IDL
  // library routines, and local variables first
  if (matches == (char **)NULL) {
    matches = rl_completion_matches(text, ridl_generator);
  }
  
  // check for structure fields
  if (matches == (char **)NULL && !in_string) {
    char *dotpos = strstr(text, ".");
    if (dotpos != (char *)NULL) {
      svarlen = (int) (dotpos - text);
      svarname = (char *)malloc(svarlen + 1);
      strncpy(svarname, text, svarlen);
      svarname[svarlen] = '\0';
      current_struct = svarname;
      ridl_get_structurefields_list(svarname);
      matches = rl_completion_matches(dotpos + 1, ridl_structurefield_generator);
      ridl_remove_structurefields_list();
      current_struct = NULL;
      free(svarname);
    }
  }

  // check for method names
  if (matches == (char **)NULL && !in_string) {
    char *arrowpos = strstr(text, "->");
    if (arrowpos != (char *)NULL) {
      ovarlen = (int) (arrowpos - text);
      ovarname = (char *)malloc(ovarlen + 1);
      strncpy(ovarname, text, ovarlen);
      ovarname[ovarlen] = '\0';
      current_obj = ovarname;
      ridl_get_methodnames_list(ovarname);
      matches = rl_completion_matches(arrowpos + 2, ridl_methodname_generator);
      ridl_remove_methodnames_list();
      current_obj = NULL;
      free(ovarname);
    }
  }

  return(matches);
}


/**
   The routine is required to be called before the completion routine can be
   used.
*/
void ridl_completion_init(void) {
  char line[RIDL_MAX_LINE_LENGTH];
  char idl_routines_filename[RIDL_MAX_LINE_LENGTH];
  char idl_classes_filename[RIDL_MAX_LINE_LENGTH];
  FILE *fp;
  int r, i;
  
  sprintf(idl_routines_filename, "%s/share/idl_routines.txt", RIDL_DIR);
  if (!ridl_file_exists(idl_routines_filename)) {
    ridl_warning("catalog of IDL library routines not found, completion on routine names not available");
    idl_routines_available = 0;
    return;
  }

  fp = fopen(idl_routines_filename, "r");

  r = 0;
  while (fgets(line, RIDL_MAX_LINE_LENGTH, fp) != NULL) {
    idl_routines[r] = (char *)calloc(strlen(line), 1);
    for (i = 0; i < strlen(line) - 1; i++) {
      if (line[i] == '\n') break;
      idl_routines[r][i] = line[i];
    }
    r++;
  }
  fclose(fp);

  sprintf(idl_classes_filename, "%s/share/idl_classes.txt", RIDL_DIR);
  if (!ridl_file_exists(idl_classes_filename)) {
    ridl_warning("catalog of IDL library classes not found, completion on class names not available");
    idl_classes_available = 0;
    return;
  }

  fp = fopen(idl_classes_filename, "r");

  r = 0;
  while (fgets(line, RIDL_MAX_LINE_LENGTH, fp) != NULL) {
    idl_classes[r] = (char *)calloc(strlen(line), 1);
    for (i = 0; i < strlen(line) - 1; i++) {
      if (line[i] == '\n') break;
      idl_classes[r][i] = line[i];
    }
    r++;
  }
  fclose(fp);

  rl_basic_word_break_characters = " \t\n\"\\'`@$<=;|&{(";
}
