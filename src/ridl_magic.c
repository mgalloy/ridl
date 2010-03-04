#include <stdio.h>  
#include <stdlib.h> 


//
// Print help for the magic commands.
//
void ridl_printmagichelp(void) {
  char *indent = "  ";
  char *magic_format = "%s%-*s %s\n";
  int magic_width = 16;
  
  ridl_printversion();
  
  printf("\nmagic commands:\n");

  printf(magic_format, indent, magic_width, ":colors", 
         "toggle whether color is used");  
  printf(magic_format, indent, magic_width, ":doc routine", 
         "show calling syntax and comment header for the routine");
  printf(magic_format, indent, magic_width, ":help", 
         "show this help message");
  printf(magic_format, indent, magic_width, ":history [n]", 
         "show the last n commands prefixed with the command number");
  printf(magic_format, indent, magic_width, ":qhistory [n]", 
         "show the last n commands with no prefix");
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
}