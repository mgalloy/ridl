static char *ridl_version = "0.1";

#define RIDL_PROMPT_LENGTH 1024

static char *ridl_prompt = "rIDL> ";
static char ridl_current_wdir[RIDL_PROMPT_LENGTH];
static char ridl_expandedprompt[RIDL_PROMPT_LENGTH];

static int ridl_cmdnumber = 0;

const int RIDL_MAX_LINE_LENGTH = 1024;
const int RIDL_RBUF_SIZE = 500;

static float ridl_event_delay = 0.1;
