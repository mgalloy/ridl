#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "ridl.h"


/*
   Prepends a new command line with time stamp to the history file.
*/
void ridl_addhistoryline(char *line, char *history_file_location, char *history_file_backup_location) {
  char history[RIDL_RBUF_SIZE][RIDL_MAX_LINE_LENGTH];
  char tmpline[RIDL_MAX_LINE_LENGTH];
  FILE *fp; 
  int i, line_number = 0;
  
  // create history line with time stamp
  char *timestamp = ridl_currenttimestamp();
  char historyline[RIDL_MAX_LINE_LENGTH];
  
  sprintf(historyline, "%s <!-- %s -->", line, timestamp);
  free(timestamp);
  
  // add history line to the history file
  
  // read history file into a buffer
  fp = fopen(history_file_location, "r");
  while (fgets(tmpline, RIDL_MAX_LINE_LENGTH, fp) != NULL && line_number < RIDL_RBUF_SIZE) {
    strcpy(history[line_number++], tmpline);
  }
  fclose(fp);
    
  // write new command line plus buffer to the backup location
  fp = fopen(history_file_backup_location, "w");
  fprintf(fp, "%s\n", historyline);
  for (i = 0; i < line_number; i++) {
    fprintf(fp, "%s", history[i]); 
  }
  fclose(fp);
  
  // write new command line plus buffer
  fp = fopen(history_file_location, "w");
  fprintf(fp, "%s\n", historyline);
  for (i = 0; i < line_number; i++) {
    fprintf(fp, "%s", history[i]); 
  }
  fclose(fp);
}