#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#include "idl_export.h" 
#include "readline/history.h"

#include "ridl.h"


/**
   @file
   This file contains helper routines dealing with the rIDL command history
   system.
*/


/// Filename of file containing IDL's command history
static char history_filename[1024]; 

/// rIDL backup of history file
static char historybackup_filename[1024];


/**
   Returns the current time in the form:
   @code
   25-Feb-2010 16:42:57.00
   @endcode
   
   @return string representing the current time
*/
char *ridl_currenttimestamp(void) {
  char *timestamp = (char *)malloc(25);
  char *monthname[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  struct tm *stime;
  char *date; 
  time_t timer;
  timer = time(NULL);
  stime = localtime(&timer);
  date = asctime(stime);
  
  sprintf(timestamp, 
          "%2d-%s-%4d %2d:%02d:%05.2f", 
          stime->tm_mday,
          monthname[stime->tm_mon],
          1900 + stime->tm_year,
          stime->tm_hour,
          stime->tm_min,
          (float) stime->tm_sec);
  return(timestamp);
}


/**
   Reads IDL history file and populates the Readline history.
   
   @return integer representing number of lines in the command history loaded
*/
int ridl_populatehistory(void) {
  FILE *fp = fopen(history_filename, "r");
  int i, cmdnum = 0, line_number = 0, rline_number;
  char history[RIDL_RBUF_SIZE][RIDL_MAX_LINE_LENGTH];
  char line[RIDL_MAX_LINE_LENGTH];
  
  while (fgets(line, RIDL_MAX_LINE_LENGTH, fp) != NULL && line_number < RIDL_RBUF_SIZE) {
    for (i = strlen(line); i > 0; i--) {
      if (line[i] == '<') { 
        line[i - 1] = '\0'; // i - 1 one because of the space between cmd and <!--
      }
    }
    strcpy(history[line_number++], line);
  }
  
  for (rline_number = line_number - 1; rline_number >= 0; rline_number--) {
    add_history(history[rline_number]);
    cmdnum++;
  }
  
  fclose(fp);
  
  return(cmdnum);
}


/**
   Initialize the rIDL history system.
   
   @return integer representing number of lines in the command history loaded
*/
int ridl_initialize_history(void) {
  IDL_USER_INFO user_info;
  IDL_GetUserInfo(&user_info);

  sprintf(history_filename, "%s/.idl/itt/rbuf/history", user_info.homedir);
  sprintf(historybackup_filename, "%s/.idl/itt/rbuf/#history", user_info.homedir);
  
  using_history();
  return(ridl_populatehistory());
}


/**
   Adds a line to the history system. Prepends a new command line with time 
   stamp to the history file and adds it to readline's history.
   
   @param[in] line line to add to the history
*/
void ridl_addhistoryline(char *line) {
  char history[RIDL_RBUF_SIZE][RIDL_MAX_LINE_LENGTH];
  char tmpline[RIDL_MAX_LINE_LENGTH];
  FILE *fp; 
  int i, line_number = 0;
  
  // add line to Readline's history
  add_history(line);
    
  // create history line with time stamp
  char *timestamp = ridl_currenttimestamp();
  char historyline[RIDL_MAX_LINE_LENGTH];
  
  sprintf(historyline, "%s <!-- %s -->", line, timestamp);
  free(timestamp);
  
  // add history line to the history file
  
  // read history file into a buffer
  fp = fopen(history_filename, "r");
  while (fgets(tmpline, RIDL_MAX_LINE_LENGTH, fp) != NULL 
           && line_number < RIDL_RBUF_SIZE) {
    strcpy(history[line_number++], tmpline);
  }
  fclose(fp);
    
  // write new command line plus buffer to the backup location
  fp = fopen(historybackup_filename, "w");
  fprintf(fp, "%s\n", historyline);
  for (i = 0; i < line_number; i++) {
    fprintf(fp, "%s", history[i]); 
  }
  fclose(fp);
  
  // write new command line plus buffer
  fp = fopen(history_filename, "w");
  fprintf(fp, "%s\n", historyline);
  for (i = 0; i < line_number; i++) {
    fprintf(fp, "%s", history[i]); 
  }
  fclose(fp);
}