#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>


/*
  IDL_DATAFILE_USER_COMMENTS    
  IDL_DEVICE                    
  IDL_EDIT_INPUT                
  IDL_EXCEPT                    
  IDL_MORE                      
  IDL_MSG_PREFIX                
  + IDL_PROMPT                    
  IDL_QUIET                     
  IDL_RBUF_PERSIST              
  IDL_RBUF_SIZE                 
  IDL_DIR                       
  IDL_DLM_PATH                  
  IDL_HELP_PATH                 
  IDL_MAKE_DLL_COMPILE_DIRECTORY                
  + IDL_PATH                      
  IDL_PATH_CACHE_DISABLE        
  IDL_STARTUP                   
  IDL_START_DIR                 
  IDL_TMPDIR                    
  IDL_CPU_TPOOL_MAX_ELTS        
  IDL_CPU_TPOOL_MIN_ELTS        
  IDL_CPU_TPOOL_NTHREADS        
  IDL_CPU_VECTOR_ENABLE         
  IDL_GR_TILECACHESIZE          
  IDL_GR_TTCACHESIZE            
  IDL_GR_X_COLORS               
  IDL_GR_X_DEPTH                
  IDL_GR_X_HEIGHT               
  IDL_GR_X_ONTOP                
  IDL_GR_X_QSCREEN              
  IDL_GR_X_RENDERER             
  IDL_GR_X_RETAIN               
  IDL_GR_X_VISUAL               
  IDL_GR_X_WIDTH                
  IDL_PREF_OBSOLETE_MIGRATE     
  IDL_PREF_OBSOLETE_WARN
  
  + Windows-specific preferences
*/
 
void ridl_readpreferencefile(char *filename) {
  
}



void ridl_setpreference(char *name, char *value) {
  char *cmd;
  int status;
  
  if (strcmp(name, "IDL_PATH") == 0) {
    cmd = (char *)calloc(strlen(value) + 11, 1);
    sprintf(cmd, "!path = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_PROMPT") == 0) {
    char space = value[strlen(value) - 1] == ' ' ? '\0' : ' ';
    cmd = (char *)calloc(strlen(value) + 13, 1);
    
    sprintf(cmd, "!prompt = '%s%c'", value, space);
    status = IDL_ExecuteStr(cmd);
    free(cmd);    
  } else {
    ridl_warning("unknown preference: %s", name);
  }
}
