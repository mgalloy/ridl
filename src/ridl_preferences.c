#include <stdio.h>  
#include <stdlib.h> 

#include "idl_export.h"

#include "ridl.h"
#include "ridl_logging.h"


void ridl_readpreferences(void) {
  IDL_VPTR pref_location;
  int status, i, value_index;
  FILE *fp;
  char line[RIDL_MAX_LINE_LENGTH];
  char pref_name[RIDL_MAX_LINE_LENGTH] = "";
  char pref_value[RIDL_MAX_LINE_LENGTH] = "";
  
  // find preferences file location
  status = IDL_ExecuteStr("_ridl_preflocation = ridl_preflocation()");
  pref_location = IDL_FindNamedVariable("_ridl_preflocation", 0);
  
  // create empty preferences file if none exists
  if (!ridl_file_exists(IDL_VarGetString(pref_location))) {
    fp = fopen(IDL_VarGetString(pref_location), "w");
    fclose(fp);
  }
    
  // read preferences line by line, setting preferences
  fp = fopen(IDL_VarGetString(pref_location), "r");
  while (fgets(line, RIDL_MAX_LINE_LENGTH, fp) != NULL) {
    // skip comment lines
    if (line[0] == '#') continue;
    
    i = 0;
    while ((pref_name[i] = line[i]) != '=') {
      i++;
    }
    pref_name[i] = '\0';
    value_index = ++i;

    while ((pref_value[i - value_index] = line[i]) != '\n') {
      i++;
    }
    pref_value[i - value_index] = '\0';
        
    if (strcmp(pref_name, "NOTEBOOK_FORMAT") == 0) {
      if (strcmp(pref_value, "HTML") == 0) {
        ridl_setloggingformat(0);
      } else if (strcmp(pref_value, "rst") == 0) {
        ridl_setloggingformat(1);
      }
    }
  }
  fclose(fp);
    
  // free up IDL variables
  status = IDL_ExecuteStr("delvar, _ridl_preflocation");
}