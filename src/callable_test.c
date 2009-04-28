#include <stdio.h>   
#include "idl_export.h"  
   
static void free_callback(UCHAR *addr) {  
   printf("IDL released(%u)\n", addr);   
}  
   
int main(int argc, char **argv) {  
  IDL_INIT_DATA init_data;  
  float f[10];   
  int i;   
  
  IDL_VPTR v;   
  IDL_MEMINT dim[IDL_MAX_ARRAY_DIM];  
   
  static char *cmds[] = { "tmp2 = total(tmp)",   
    "print,'IDL total is ',tmp2", 
    "plot,tmp" };   
  static char *cmds2[] = { "a = widget_base()",   
    "b = widget_button(a, value='Press When Done', xsize=300, ysize=200)", 
    "widget_control,/realize, a",   
    "dummy = widget_event(a)",   
    "widget_control,/destroy, a" };   
   
   
  for (i=0; i < 10; i++) f[i] = (float) i;  
  init_data.options = IDL_INIT_CLARGS;  
  init_data.clargs.argc = argc;  
  init_data.clargs.argv = argv;  
  if (IDL_Initialize(&init_data)) {   
    dim[0] = 10;   
    printf("ARRAY ADDRESS(%u)\n", f);   
    if (v = IDL_ImportNamedArray("TMP", 1, dim, IDL_TYP_FLOAT,  
                 (UCHAR *) f, free_callback, (void *) 0)) {   
      (void) IDL_ExecuteStr("print, tmp");   
      (void) IDL_Execute(sizeof(cmds)/sizeof(char *), cmds);   
      (void) IDL_ExecuteStr("print, 'Free the user memory'");   
      (void) IDL_ExecuteStr("tmp = 0");   
      if (v = IDL_FindNamedVariable("tmp2", IDL_FALSE))   
        printf("Program total is %f\n", v->value.f);   
      (void) IDL_Execute(sizeof(cmds2)/sizeof(char *), cmds2);   
      IDL_Cleanup(IDL_FALSE);   /* Don't return */  
    }   
  } else {
    printf("Failed to initialize\n");
  }
   
  return 1;   
}  

