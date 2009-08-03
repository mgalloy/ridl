static char *ridl_version = "0.1";
static char *ridl_prompt = "rIDL[%d]> ";
static char ridl_expandedprompt[1024];
static int ridl_cmdnumber = 0;

const int RIDL_MAX_LINE_LENGTH = 1024;
const int RIDL_RBUF_SIZE = 500;
