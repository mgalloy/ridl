#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

#include "readline/readline.h"


typedef struct { 
  char *name; /* User printable name of the function. */ 
  rl_icpfunc_t *func; /* Function to call to do the job. */ 
  char *doc; /* Documentation for this function. */
} COMMAND;


/**
   mallocs return value, caller should free; used in Readline completion
   callback
   
   @return dynamically allocated string
   
   @param[in] text
   @param[in] state
*/
char *ridl_command_generator(const char *text, int state) {
  if (!state) {
    char *result = (char *)malloc(6);
    strcpy(result, "print");
    return(result);
  } else { 
    return((char *)NULL);
  }
}


char **ridl_completion(const char *text, int start, int end) {
  char **matches = (char **) NULL;
  // determines state and calls the correct generator
    
  //matches = rl_completion_matches(text, ridl_command_generator);
  //return(matches);
  
  //printf("\ntext = '%s', start = %d, end = %d\n", text, start, end);
  return((char **) NULL);
}