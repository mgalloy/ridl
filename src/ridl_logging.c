#include <stdio.h>
#include <stdlib.h>

#include "idl_export.h"

#include "ridl_config.h"
#include "ridl_history.h"
#include "ridl_logging.h"

// 0 = HTML, 1 = rst
static int ridl_logging_format = 0;

static int ridl_new_codeblock = 0;

static int ridl_logging = 0;
static int ridl_teeing = 0;
static int ridl_notebooking = 0;

static char *log_file;
static FILE *log_fp;

static char *notebook_filename;
static FILE *notebook_fp;
static int notebook_image_counter = 0;

static char *rst_indent = "   ";


void ridl_setloggingformat(int format) {
  ridl_logging_format = format;
}


int ridl_getloggingformat(void) {
  return(ridl_logging_format);
}


/**
   @file
   This file contains helper routines dealing with the logging/teeing
   and notebooking commands.
*/

int ridl_isnotebooking(void) {
  return(ridl_notebooking);
}


void ridl_setnotebooking(int notebooking) {
  ridl_notebooking = notebooking;
}


void ridl_notebookcmd(char *prompt, char *cmd) {
  switch(ridl_logging_format) {
    case 0:
      fprintf(notebook_fp, "    <p class=\"command\"><span class=\"prompt\">%s</span>%s</p>\n", prompt, cmd);
      break;
    case 1:
      if (ridl_new_codeblock) {
        fprintf(notebook_fp, "::\n\n");
        ridl_new_codeblock = 0;
      }
      fprintf(notebook_fp, "%s%s%s\n", rst_indent, prompt, cmd);
      break;
  }

}


void ridl_notebookgraphic(void) {
  // save .png file
  char *savecmd_format = "ridl_savegraphic, '%s-%d.png'";
  int cmd_length = strlen(notebook_filename) + strlen(savecmd_format) - 4 + 1 + 1;
  char *savecmd = (char *)malloc(cmd_length);
  sprintf(savecmd, savecmd_format, notebook_filename, notebook_image_counter);
  int result = IDL_ExecuteStr(savecmd);
  free(savecmd);

  // put reference to .png file into notebook
  switch(ridl_logging_format) {
    case 0:
      fprintf(notebook_fp, "    <img src=\"%s-%d.png\"/>\n", notebook_filename, notebook_image_counter); 
      break;
    case 1:
      fprintf(notebook_fp, "\n.. image:: %s-%d.png\n\n", notebook_filename, notebook_image_counter);
      ridl_new_codeblock = 1;
      break;
  }
  
  // increment image counter
  notebook_image_counter++;
}


void ridl_notebookoutput(int flags, char *buf, int n) {
  char *output = (char *)malloc(strlen(buf) + 1);
  strncpy(output, buf, n);
  output[n] = '\0';
  
  printf("%s", output);
  if (flags & IDL_TOUT_F_NLPOST) printf("\n");

  //if (flags & IDL_TOUT_F_STDERR == 0) {
  switch(ridl_logging_format) {
    case 0:
      fprintf(notebook_fp, "    <pre class=\"output\">%s</pre>", output);
      break;
    case 1:
      fprintf(notebook_fp, "%s%s", rst_indent, output);
      break;
  }
  
  if (flags & IDL_TOUT_F_NLPOST) fprintf(notebook_fp, "\n");
  //}
  free(output);
}


void ridl_initnotebook(char *filename) {
  char *ts = ridl_currenttimestamp();
  
  notebook_filename = (char *)malloc(strlen(filename) + 1);
  strcpy(notebook_filename, filename);
  
  notebook_fp = fopen(filename, "w");
  IDL_ToutPush(ridl_notebookoutput);
  
  switch(ridl_logging_format) {
    case 0:
      fprintf(notebook_fp, "<html>\n");
  
      fprintf(notebook_fp, "  <head>\n");
      fprintf(notebook_fp, "    <title>Notebook from %s</title>\n", ts);
  
      // TODO: eventually this should be put into a stylesheet file and imported
      fprintf(notebook_fp, "    <style type=\"text/css\" media=\"all\">\n");
      fprintf(notebook_fp, "      p.command { whitespace: pre; font-family: Monaco; margin-top: 0em; margin-bottom: 0em; }\n");
      fprintf(notebook_fp, "      span.prompt { color: #C65D09; }\n");
      fprintf(notebook_fp, "      pre.output { color: #4A62A4; font-family: Monaco; margin-top: 0em; margin-bottom: 0em; }\n");
      fprintf(notebook_fp, "      p.date { color: #666; font-size: 0.9em; }\n");
      fprintf(notebook_fp, "    </style>\n");
      fprintf(notebook_fp, "  </head>\n");

      fprintf(notebook_fp, "  <body>\n");
      break;
    case 1:
      fprintf(notebook_fp, "Notebook from %s\n\n", ts);
      ridl_new_codeblock = 1;
      break;
  }
  
  free(ts);
}


void ridl_closenotebook(void) {
  char *ts = ridl_currenttimestamp();
  switch(ridl_logging_format) {
    case 0:
      fprintf(notebook_fp, "    <p class=\"date\">Notebook produced by rIDL %s with IDL %s on %s.</p>", 
              RIDL_VERSION, IDL_STRING_STR(&IDL_SysvVersion.release), ts);
  
      fprintf(notebook_fp, "  </body>\n");  
      fprintf(notebook_fp, "</html>\n");
      break;
    case 1:
      if (ridl_new_codeblock) {
        ridl_new_codeblock = 0;
      } else {
        fprintf(notebook_fp, "\n");
      }
      fprintf(notebook_fp, "Notebook produced by rIDL %s with IDL %s on %s.\n",
              RIDL_VERSION, IDL_STRING_STR(&IDL_SysvVersion.release), ts);
      break;
  }
  free(ts);
  fclose(notebook_fp);
}


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
