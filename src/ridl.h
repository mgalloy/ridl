#define RIDL_PROMPT_LENGTH 1024
#define RIDL_MAX_LINE_LENGTH 1024

static const int RIDL_RBUF_SIZE = 500;

static char *ridl_prompt = "rIDL> ";
static char *ridl_prompt2 = "%p1dots ";
static char ridl_current_wdir[RIDL_PROMPT_LENGTH];
static char ridl_expandedprompt[RIDL_PROMPT_LENGTH];
static char ridl_expandedprompt2[RIDL_PROMPT_LENGTH];

static int continued = 0;
static char ridl_continuedline[RIDL_MAX_LINE_LENGTH];

static int ridl_cmdnumber = 1;

static float ridl_event_delay = 0.1;

char *ridl_currenttimestamp(void);
void ridl_printversion(void);
int ridl_file_exists(const char *filename);
void ridl_launcheditor(char *filename);
char *ridl_getnextword(char *line, int start);
