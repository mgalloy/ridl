// public routines

int ridl_isnotebooking(void);
int ridl_setnotebooking(int notebooking);
void ridl_notebookcmd(char *prompt, char *cmd);
void ridl_notebookoutput(int flags, char *buf, int n);
void ridl_notebookgraphic(void);

int ridl_islogging(void);
void ridl_setlogging(int logging);

int ridl_isteeing(void);
void ridl_setteeing(int teeing);

void ridl_logcmd(char *prompt, char *cmd);
void ridl_logoutput(int flags, char *buf, int n);

void ridl_initlog(char *filename);
void ridl_closelog(void);