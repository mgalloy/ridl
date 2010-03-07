#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "readline/history.h"

#include "ridl.h"


/**
   @file
   This file contains helper routines dealing with the magic commands.
*/


/**
   Print help for the magic commands.
*/
void ridl_magic_help(void) {
  char *indent = "  ";
  char *magic_format = "%s%-*s %s\n";
  int magic_width = 21;
  
  ridl_printversion();
  
  printf("\nmagic commands:\n");

  printf(magic_format, indent, magic_width, ":colors", 
         "toggle whether color is used");  
  printf(magic_format, indent, magic_width, ":doc routine", 
         "show calling syntax and comment header for the routine");
  printf(magic_format, indent, magic_width, ":help", 
         "show this help message");
  printf(magic_format, indent, magic_width, ":history [n]", 
         "show the last n commands; defaults to 10 lines");
  printf(magic_format, indent, magic_width, ":qhistory [n]", 
         "show the last n commands with no prefix; defaults to 10 lines");
  printf(magic_format, indent, magic_width, ":histedit n filename", 
         "send the last n commands to filename and launch editor");
  printf(magic_format, indent, magic_width, ":log filename", 
         "start logging all commands and output to filename");
  printf(magic_format, indent, magic_width, ":unlog", 
         "stop logging commands and output");
  printf(magic_format, indent, magic_width, ":tee filename", 
         "log output to filename");
  printf(magic_format, indent, magic_width, ":untee", 
         "stop logging output");
  printf(magic_format, indent, magic_width, ":version", 
         "print version information");}


/**
   Print history of last n commands.
   
   @param[in] line history magic command line
   @param[in] firstcharIndex index of magic command in line
   @param[in] length number of characters in in magic command
   @param[in] showCmdnum 1 if command number should be printed, 0 if not
*/
void ridl_magic_history(char *line, int firstcharIndex, int length, int showCmdnum) {
  char *nstr = ridl_getnextword(line, firstcharIndex + length);
  int i, n = 10; 
  
  HIST_ENTRY *h;
  HISTORY_STATE *hs;
  if (strlen(nstr) > 0) n = atoi(nstr);
  hs = history_get_history_state();
  for (i = n - 1; i >= 0; i--) {
    h = history_get(hs->offset - i); 
    if (showCmdnum) {
      printf("[%d]: %s\n", hs->offset - i, h == 0 ? "" : h->line);
    } else {
      printf("%s\n", h == 0 ? "" : h->line);
    }
  }
  free(nstr);
} 


/**
   Sends history of last n commands to a file and launches an editor.
   
   @param[in] line histedit magic command line
   @param[in] firstcharIndex index of magic command in line
*/
void ridl_magic_histedit(char *line, int firstcharIndex) {
  HIST_ENTRY *h;
  HISTORY_STATE *hs;
  char *snlines = ridl_getnextword(line, firstcharIndex + 10);
  char *filename = ridl_getnextword(line, firstcharIndex + 10 + strlen(snlines) + 1);
  int i, nlines = atoi(snlines);
  
  if (ridl_file_exists(filename)) {
    printf("File %s already exists\n", filename);
    return;
  }
  
  FILE *fp = fopen(filename, "w");
  
  hs = history_get_history_state();
  for (i = nlines - 1; i >= 0; i--) {
    h = history_get(hs->offset - i); 
    fprintf(fp, "%s\n", h == 0 ? "" : h->line);
  }
  free(snlines);
  free(filename);
  fclose(fp);
  
  ridl_launcheditor(filename); 
}   
