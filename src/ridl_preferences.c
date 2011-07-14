#include <stdio.h>  
#include <stdlib.h> 

#include "idl_export.h"

#include "ridl.h"
#include "ridl_logging.h"
#include "ridl_idlpreferences.h"


void ridl_printpreferences(void) {
  char *indent = "  ";
  printf("rIDL preference values:\n");
  printf("%sNOTEBOOK_FORMAT=%s\n", 
         indent, 
         ridl_getloggingformat() ? "rst" : "HTML");
  printf("%sRIDL_PROMPT=%s\n", 
         indent, 
         ridl_ridlprompt());
  printf("%sAUTO_COMPILE=%s\n", 
         indent, 
         ridl_getautocompile() ? "yes" : "no");         
}


void ridl_process_pref_line(char *pref_line) {
  int i, value_index, len;
  char pref_name[RIDL_MAX_LINE_LENGTH] = "";
  char pref_value[RIDL_MAX_LINE_LENGTH] = "";
  
  // skip comment lines
  if (pref_line[0] == '#') return;
  
  len = strlen(pref_line);
  if (len == 0) {
    return;
  }
  if (pref_line[0] == ' ' || pref_line[0] == '\n') {
    return;
  }
  
  i = 0;
  while (i < len && (pref_name[i] = pref_line[i]) != '=') {
    i++;
  }
  if (i == len) {
    ridl_warning("invalid preference syntax '%s'", pref_line);
    return;
  }
  pref_name[i] = '\0';
  value_index = ++i;

  while (i < len && (pref_value[i - value_index] = pref_line[i]) != '\n') {
    i++;
  }
  pref_value[i - value_index] = '\0';

  if (strcmp(pref_name, "NOTEBOOK_FORMAT") == 0) {
    if (strcmp(pref_value, "HTML") == 0) {
      ridl_setloggingformat(0);
    } else if (strcmp(pref_value, "rst") == 0) {
      ridl_setloggingformat(1);
    }
  } else if (strcmp(pref_name, "RIDL_PROMPT") == 0) {
    ridl_setpreference("IDL_PROMPT", pref_value);
  } else if (strcmp(pref_name, "AUTO_COMPILE") == 0) {
    ridl_setautocompile(strcmp(pref_value, "yes") == 0 ? 1 : 0);
  } else {
    ridl_warning("unknown preference name '%s'", pref_name);
  }
}


void ridl_read_preferences(void) {
  IDL_VPTR pref_location;
  int status;
  FILE *fp;
  char line[RIDL_MAX_LINE_LENGTH];
  
  // find preferences file location
  status = IDL_ExecuteStr("_ridl_preflocation = ridl_preflocation()");
  pref_location = IDL_FindNamedVariable("_ridl_preflocation", 0);
  
  // create empty preferences file if none exists
  if (!ridl_file_exists(IDL_VarGetString(pref_location))) {
    fp = fopen(IDL_VarGetString(pref_location), "w");
    fclose(fp);
    ridl_warning("Created preferences file at %s", IDL_VarGetString(pref_location));
  }
    
  // read preferences line by line, setting preferences
  fp = fopen(IDL_VarGetString(pref_location), "r");
  while (fgets(line, RIDL_MAX_LINE_LENGTH, fp) != NULL) {
    ridl_process_pref_line(line);
  }
  fclose(fp);
    
  // free up IDL variables
  status = IDL_ExecuteStr("delvar, _ridl_preflocation");
}