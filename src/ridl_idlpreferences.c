#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "idl_export.h"

#include "ridl.h"

/*
  - IDL_DATAFILE_USER_COMMENTS
  + IDL_DEVICE
  + IDL_EDIT_INPUT
  + IDL_EXCEPT
  + IDL_MORE
  + IDL_MSG_PREFIX
  + IDL_PROMPT
  + IDL_QUIET
  - IDL_RBUF_PERSIST
  - IDL_RBUF_SIZE

  + IDL_DIR
  + IDL_DLM_PATH
  + IDL_HELP_PATH
  + IDL_MAKE_DLL_COMPILE_DIRECTORY
  + IDL_PATH
  + IDL_PATH_CACHE_DISABLE
  - IDL_STARTUP
  - IDL_START_DIR
  - IDL_TMPDIR

  + IDL_CPU_TPOOL_MAX_ELTS
  + IDL_CPU_TPOOL_MIN_ELTS
  + IDL_CPU_TPOOL_NTHREADS
  + IDL_CPU_VECTOR_ENABLE

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

  if (strcmp(name, "IDL_DATAFILE_USER_COMMENTS") == 0) {
    // TODO: implement
    ridl_warning("IDL_DATAFILE_USER_COMMENTS pref not implemented");
  } else if (strcmp(name, "IDL_DEVICE") == 0) {
    cmd = (char *)calloc(strlen(value) + 13, 1);
    sprintf(cmd, "set_plot, '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_EDIT_INPUT") == 0) {
    cmd = (char *)calloc(strlen(value) + 14, 1);
    sprintf(cmd, "!edit_input = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_EXCEPT") == 0) {
    cmd = (char *)calloc(strlen(value) + 11, 1);
    sprintf(cmd, "!except = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_MORE") == 0) {
    cmd = (char *)calloc(strlen(value) + 9, 1);
    sprintf(cmd, "!more = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_MSG_PREFIX") == 0) {
    cmd = (char *)calloc(strlen(value) + 29, 1);
    sprintf(cmd, "!error_state.msg_prefix = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_PROMPT") == 0) {
    char space = value[strlen(value) - 1] == ' ' ? '\0' : ' ';
    cmd = (char *)calloc(strlen(value) + 13, 1);
    sprintf(cmd, "!prompt = '%s%c'", value, space);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_QUIET") == 0) {
    cmd = (char *)calloc(strlen(value) + 10, 1);
    sprintf(cmd, "!quiet = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_RBUF_PERSIST") == 0) {
    // TODO: implement
    ridl_warning("IDL_RBUF_PERSIST pref not implemented");
  } else if (strcmp(name, "IDL_RBUF_SIZE") == 0) {
    // TODO: implement
    ridl_warning("IDL_RBUF_SIZE pref not implemented");
  } else if (strcmp(name, "IDL_DIR") == 0) {
    cmd = (char *)calloc(strlen(value) + 10, 1);
    sprintf(cmd, "!dir = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_DLM_PATH") == 0) {
    cmd = (char *)calloc(strlen(value) + 15, 1);
    sprintf(cmd, "!dlm_path = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_HELP_PATH") == 0) {
    cmd = (char *)calloc(strlen(value) + 16, 1);
    sprintf(cmd, "!help_path = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_MAKE_DLL_COMPILE_DIRECTORY") == 0) {
    cmd = (char *)calloc(strlen(value) + 33, 1);
    sprintf(cmd, "!make_dll.compile_directory = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_PATH") == 0) {
    cmd = (char *)calloc(strlen(value) + 11, 1);
    sprintf(cmd, "!path = '%s'", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_PATH_CACHE_DISABLE") == 0) {
    cmd = (char *)calloc(strlen(value) + 26, 1);
    sprintf(cmd, "path_cache, enable=%s eq 0B", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_STARTUP") == 0) {
    // TODO: implement
    ridl_warning("IDL_STARTUP pref not implemented");
  } else if (strcmp(name, "IDL_START_DIR") == 0) {
    // TODO: implement
    ridl_warning("IDL_START_DIR pref not implemented");
  } else if (strcmp(name, "IDL_TMPDIR") == 0) {
    // TODO: implement
    ridl_warning("IDL_TMPDIR pref not implemented");
  } else if (strcmp(name, "IDL_CPU_TPOOL_MAX_ELTS") == 0) {
    cmd = (char *)calloc(strlen(value) + 23, 1);
    sprintf(cmd, "!cpu.tpool_max_elts = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_CPU_TPOOL_MIN_ELTS") == 0) {
    cmd = (char *)calloc(strlen(value) + 23, 1);
    sprintf(cmd, "!cpu.tpool_min_elts = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_CPU_TPOOL_NTHREADS") == 0) {
    cmd = (char *)calloc(strlen(value) + 23, 1);
    sprintf(cmd, "!cpu.tpool_nthreads = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else if (strcmp(name, "IDL_CPU_VECTOR_ENABLE") == 0) {
    cmd = (char *)calloc(strlen(value) + 22, 1);
    sprintf(cmd, "!cpu.vector_enable = %s", value);
    status = IDL_ExecuteStr(cmd);
    free(cmd);
  } else {
    ridl_warning("unknown preference: %s", name);
  }
}
