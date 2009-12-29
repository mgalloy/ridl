static char *ridl_version = "0.1";

#define RIDL_USE_RIDL_PROMPT 0
#define RIDL_USE_IDL_PROMPT 1
#define RIDL_USE_NUMBERED_PROMPT 2

static char *ridl_prompt = "%s> ";
static char *ridl_idl_prompt = "IDL> ";
static char *ridl_numbered_prompt = "[%d]: ";
static char ridl_expandedprompt[1024];

static int ridl_cmdnumber = 0;

const int RIDL_MAX_LINE_LENGTH = 1024;
const int RIDL_RBUF_SIZE = 500;

static float ridl_event_delay = 0.1;
