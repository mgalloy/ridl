#include <stdio.h>  
#include <stdlib.h> 

#include "idl_export.h"


static int ridl_logging = 0;
static int ridl_teeing = 0;
static char *log_file;
static FILE *log_fp;


/**
   @file
   This file contains helper routines dealing with the logging/teeing 
   commands.
*/


int ridl_islogging(void) {
  return(ridl_logging);
}


void ridl_setlogging(int logging) {
  ridl_logging = logging;
}


int ridl_isteeing(void) {
  return(ridl_teeing);
}


void ridl_setteeing(int teeing) {
  ridl_teeing = teeing;
}


void ridl_logcmd(char *prompt, char *cmd) {
  fprintf(log_fp, "%s%s\n", prompt, cmd);
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


void ridl_initlog(char *filename) {
  log_fp = fopen(filename, "w");
  IDL_ToutPush(ridl_logoutput);
}


void ridl_closelog(void) {
  fclose(log_fp);
}