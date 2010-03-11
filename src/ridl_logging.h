// public routines

int ridl_islogging(void);
void ridl_setlogging(int logging);

int ridl_isteeing(void);
void ridl_setteeing(int teeing);

void ridl_logcmd(char *prompt, char *cmd);
void ridl_logoutput(int flags, char *buf, int n);

void ridl_initlog(char *filename);
void ridl_closelog(void);