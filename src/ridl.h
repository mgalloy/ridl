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


// bindable routines

int ridl_stepinto_cmd(void);
int ridl_stepover_cmd(void);
int ridl_stepreturn_cmd(void);
int ridl_savegraphic_cmd(void);


// public routines

void ridl_setautocompile(int auto_compile);
int ridl_getautocompile(void);

char *ridl_ridlprompt(void);
void ridl_printversion(void);
void ridl_warning(const char *format, ...);
int ridl_file_exists(const char *filename);
void ridl_launcheditor(char *filename);

