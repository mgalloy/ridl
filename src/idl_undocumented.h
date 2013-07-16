// Undocumented IDL routines

typedef void (* IDL_REGISTER_ROUTINE)(void);

int IDL_DebugGetStackDepth(void);

void IDL_UicbRegInitTextDone(void (*ridl_inittextdone)(void));
void IDL_UicbRegExitDone(void (*ridl_exit)(void));
void IDL_UicbRegShowCompileErr(void (*ridl_show_compile_error)(void));
void IDL_UicbRegPromptChange(void (*ridl_changeprompt)(IDL_STRING *));
void IDL_UicbRegDeathHint(void (*ridl_deathhint)(void));
void IDL_UicbRegWorkingDirChange(void (*ridl_changewdir)(char *));
void IDL_UicbRegRlineFromKbrd(int (*ridl_readline_callback)(int, int));
void IDL_UicbRegPathChange(void (*ridl_changepath)(IDL_STRING *));